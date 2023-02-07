using DirectoryWatcher.Models;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System.Collections.Concurrent;

namespace DirectoryWatcher
{
    public class DirectoryWatchWorker : BackgroundService
    {
        private readonly ILogger<DirectoryWatchWorker> _logger;
        private List<FileSystemWatcher> _watchers = new List<FileSystemWatcher>();
        private List<RunItem> _runners = new List<RunItem>();
        private List<string> _ignores = new List<string>();
        private ConcurrentDictionary<string, RunItem> runnings = new ConcurrentDictionary<string, RunItem>();

        public DirectoryWatchWorker(
            ILogger<DirectoryWatchWorker> logger,
            IConfiguration configuration)
        {
            _logger = logger;

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

            var watcher = sender as FileSystemWatcher;
            _logger.LogInformation($"Detect: {e.FullPath}:{e.ChangeType}");
            

            _runners.ForEach(r =>
            {
                try
                {                  
                    string commandLine = $"\"{r.Filepath}\" {r.Argument}";

                    if (File.Exists(r.Filepath))
                    {
                        ThreadPool.QueueUserWorkItem((a) =>
                        {
                            if (runnings.TryAdd(r.RunId, r))
                            {
                                _logger.LogInformation($"Run-{r.RunId}: {commandLine}");
                                ApplicationLoader.PROCESS_INFORMATION procInfo;
                                ApplicationLoader.StartProcessAndBypassUAC(commandLine, out procInfo);

                                runnings.Remove(r.RunId, out var removed);
                            }
                            else
                            {
                                _logger.LogInformation($"Run-{r.RunId}: AlreadyRun Skip");
                            }
                        });                                                                                           
                    }
                    else
                    {
                        _logger.LogInformation($"FileNotFound-Skip: {commandLine}");
                    }

                }
                catch (Exception ex)
                {
                    _logger.LogError(ex.ToString());
                }
            });
        }                

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            while(!stoppingToken.IsCancellationRequested)
            {
                try
                {
                    GC.Collect();
                    await Task.Delay(1000 * 60, stoppingToken);
                }
                catch(Exception ex)
                {
                    _logger.LogError(ex.ToString());
                }
            }
        }
    }
}
