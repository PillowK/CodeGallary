// See https://aka.ms/new-console-template for more information

using Serilog;
using Microsoft.Extensions.Configuration;
using DirectoryWatcher;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.DependencyInjection;

const string configFilename = "appsettings.json";

var configuration = new ConfigurationBuilder()
    .AddJsonFile(configFilename)
    .Build();

Log.Logger = SerilogLogger.ConfigureLogger(configuration);

Host.CreateDefaultBuilder(args)
    .ConfigureLogging(option => option.AddSerilog())
    .ConfigureServices((host, services) =>
    {
        services.AddHostedService<DirectoryWatchWorker>();
    }).UseWindowsService()
    .Build()
    .Run();