using Microsoft.Extensions.Configuration;
using Serilog;
using Serilog.Core;
using Serilog.Events;
using System.Text;

namespace DirectoryWatcher
{
    public class SerilogLogger
    {
        public static ILogger ConfigureLogger(IConfigurationRoot config)
        {
            string logPath = config.GetSection("Logger:logPath").Value ?? 
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");

            Logger logger = new LoggerConfiguration()
                .MinimumLevel.Information()
                .WriteTo.Logger(c =>
                    c.Filter.ByIncludingOnly(e => e.Level == LogEventLevel.Information)
                    .WriteTo.RollingFile(
                        Path.Combine(logPath, "info", "watchlog.log"),
                        restrictedToMinimumLevel: LogEventLevel.Information,
                        retainedFileCountLimit: 10))
                .WriteTo.Logger(c =>
                    c.Filter.ByIncludingOnly(e => e.Level == LogEventLevel.Error)
                    .WriteTo.RollingFile(
                        Path.Combine(logPath, "error", "error.log"),
                        restrictedToMinimumLevel: LogEventLevel.Error,
                        retainedFileCountLimit: 10))
                .WriteTo.Console()
                .CreateLogger();

            return logger;
        }
    }
}
