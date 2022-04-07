using Microsoft.PowerShell;
using System;
using System.Collections.ObjectModel;
using System.Management.Automation;
using System.Management.Automation.Runspaces;

namespace RemotePowershellNet.Repository
{
    public interface IPowershellRepository
    {
        Collection<PSObject> ExecuteScript(string script, out Collection<ErrorRecord> errors);
    }

    public class PowershellRepository : IPowershellRepository, IDisposable
    {        
        private Runspace _rs;
       
        public PowershellRepository()
        {
            InitRunspace();
        }

        private void InitRunspace()
        {
            try
            {
                InitialSessionState initialState = InitialSessionState.CreateDefault();
                initialState.ExecutionPolicy = ExecutionPolicy.RemoteSigned;
                initialState.ImportPSModule(new string[] {
                    @"C:\Program Files\WindowsPowerShell\Modules\ExchangeOnlineManagement\2.0.5\netFramework\ExchangeOnlineManagement.psm1"
                });

                _rs = RunspaceFactory.CreateRunspace(initialState);
                _rs.Open();
            }
            catch (Exception ex)
            {               
                if(_rs != null)
                {
                    _rs.Dispose();
                }

                Console.WriteLine(ex.ToString());
                throw ex;
            }
        }

        public Collection<PSObject> ExecuteScript(string script, out Collection<ErrorRecord> errors)
        {
            using (PowerShell powerShell = PowerShell.Create())
            {
                powerShell.Runspace = _rs;
                powerShell.AddScript(script);

                Collection<PSObject> results = powerShell.Invoke();
                errors = powerShell.Streams.Error.ReadAll();

                return results;
            }
        }

        public void Dispose()
        {
            if (_rs != null)
            {
                _rs.Dispose();
            }
        }
    }
}
