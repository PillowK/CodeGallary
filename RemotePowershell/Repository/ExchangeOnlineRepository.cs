using RemotePowershellNet.Model;
using System;
using System.Collections.ObjectModel;
using System.Management.Automation;

namespace RemotePowershellNet.Repository
{
    public interface IExchangeOnlineRepository
    {
        bool ConnectToExchangeOnline(string certFilePath, string certPassword, string appId, string organization);
        Mailbox GetMailbox(string email);
        MailboxStatistics GetMailboxStatistics(string email);
        void DisconnectExchangeOnline();
    }

    public class ExchangeOnlineRepository : PowershellRepository, IExchangeOnlineRepository
    {        
        public ExchangeOnlineRepository() 
        {

        }         

        public void DisconnectExchangeOnline()
        {
            Collection<ErrorRecord> errors;
            this.ExecuteScript($"Disconnect-ExchangeOnline -Confirm:$false", out errors);
        }

        public bool ConnectToExchangeOnline(string certFilePath, string certPassword, string appId, string organization)
        {
            try
            {
                Collection<ErrorRecord> errors;
                string script = $"Connect-ExchangeOnline -CertificateFilePath \"{certFilePath}\" -CertificatePassword (ConvertTo-SecureString -String \"{certPassword}\" -AsPlainText -Force) -AppId {appId} -Organization {organization}";

                this.ExecuteScript(script, out errors);
                return true;
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.ToString());
                return false;
            }
        }

        public Mailbox GetMailbox(string email)
        {
            Collection<ErrorRecord> errors;
            Collection<PSObject> results = this.ExecuteScript($"Get-Mailbox -Identity {email}", out errors);

            if (results != null && results.Count > 0)
            {
                return results[0].Get<Mailbox>();
            }
            else
            {
                return null;
            }
        }

        public MailboxStatistics GetMailboxStatistics(string email)
        {
            Collection<ErrorRecord> errors;
            Collection<PSObject> results = this.ExecuteScript($"Get-MailboxStatistics -Identity {email}", out errors);

            if (results != null && results.Count > 0)
            {
                return results[0].Get<MailboxStatistics>();
            }
            else
            {
                return null;
            }
        }        
    }
}
