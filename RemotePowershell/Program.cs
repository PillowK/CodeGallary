using Microsoft.PowerShell;
using RemotePowershellNet.Repository;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Management.Automation;
using System.Management.Automation.Runspaces;
using System.Reflection;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace RemotePowershellNet
{
    class Program
    {
        static void Main(string[] args)
        {
            ExchangeOnlineRepository exoRepository = new ExchangeOnlineRepository();
            if(exoRepository.ConnectToExchangeOnline(@"D:\exocert.pfx", "908910", "175dea63-f81e-42c6-90c4-cc04ac9bcf85", "feelanetRND.onmicrosoft.com"))
            {
                var mailbox = exoRepository.GetMailbox("yichoi");
                var mailboxStatistics = exoRepository.GetMailboxStatistics("yichoi");

                exoRepository.DisconnectExchangeOnline();
            }
        }
    }
}
