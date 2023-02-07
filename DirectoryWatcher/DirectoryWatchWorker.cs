using DirectoryWatcher.Models;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System.Collections.Concurrent;

namespace DirectoryWatcher
{
    public class DirectoryWatchWorker : BackgroundService
    {
        private readonly int _interval = 1;
        private readonly ILogger<DirectoryWatchWorker> _logger;
        private List<FileSystemWatcher> _watchers = new List<FileSystemWatcher>();
        private List<RunItem> _runners = new List<RunItem>();        
        private List<string> _ignores = new List<string>();
        private ConcurrentDictionary<string, RunItem> _runnings = new ConcurrentDictionary<string, RunItem>();
        private ConcurrentQueue<string> _changes = new ConcurrentQueue<string>();

        public DirectoryWatchWorker(
            ILogger<DirectoryWatchWorker> logger,
            IConfiguration configuration)
        {
            _logger = logger;
            _interval = configuration.GetValue("Settings:runnerInterval", 1);

            List<string> watchPathes = configuration.GetSection("Watch")
                .AsEnumerable()
                .Select(o => o.Value ?? "")
                .Where(x => !string.IsNullOrEmpty(x))
                .ToList();

            List<string> ignores = configuration.GetSection("Ignore")
                .AsEnumerable()
                .Select(o => o.Value?.ToLower() ?? "")
                .Where(x => !string.IsNullOrEmpty(x))
                .ToList();

            _ignores.AddRange(ignores);

            List<RunItem> runners = configuration.GetSection("Run")
                .GetChildren()
                .AsEnumerable()
                .Select(o => new RunItem(
                    o.GetValue("file", "") ?? "", 
                    o.GetValue("argument", "") ?? ""))               
                .ToList();

            _runners.AddRange(runners);

            watchPathes.ForEach(p =>
            {
                var watcher = new FileSystemWatcher(p);
                watcher.IncludeSubdirectories = true;
                watcher.EnableRaisingEvents = true;
                watcher.Changed += Watcher_ChangeDetected;
                watcher.Renamed += Watcher_ChangeDetected;
                watcher.Created += Watcher_ChangeDetected;
                watcher.Deleted += Watcher_ChangeDetected;
                _watchers.Add(watcher);
            });
        }        

        private void Watcher_ChangeDetected(object sender, FileSystemEventArgs e)
        {            
            if (_ignores.Contains(e.Name.ToLower()))
                return;            
            
            _logger.LogInformation($"Detect: {e.FullPath}:{e.ChangeType}");

            lock(_changes)
            {
                _changes.Enqueue(e.FullPath);
            }            
        }                

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            while(!stoppingToken.IsCancellationRequested)
            {
                try
                {
                    if (_changes.Count > 0)
                    {
                        lock(_changes)
                        {
                            _changes.Clear();
                        }
                        
                        _runners.ForEach(r =>
                        {
                            try
                            {
                                string commandLine = $"\"{r.Filepath}\" {r.Argument}";

                                if (File.Exists(r.Filepath))
                                {
                                    ThreadPool.QueueUserWorkItem((a) =>
                                    {
                                        if (_runnings.TryAdd(r.RunId, r))
                                        {
                                            _logger.LogInformation($"Run-{r.RunId}: {commandLine}");
                                            ApplicationLoader.PROCESS_INFORMATION procInfo;
                                            ApplicationLoader.StartProcessAndBypassUAC(commandLine, out procInfo);

                                            _runnings.Remove(r.RunId, out var removed);
                                        }
                                        else
                                        {
                                            _logger.LogInformation($"Run-{r.RunId}: AlreadyRun Skip");
                                        }
                                    });
                                }
                            }
                            catch (Exception ex)
                            {
                                _logger.LogError(ex.ToString());
                            }
                        });


                        GC.Collect();
                        await Task.Delay(1000 * 60 * _interval, stoppingToken);
                    }
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex.ToString());
                }
            }
        }
    }
}
