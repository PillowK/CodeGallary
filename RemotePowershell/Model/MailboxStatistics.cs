using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RemotePowershellNet.Model
{
    public class MailboxStatistics
    {
        public int ItemCount { get; set; }
        public string TotalItemSize { get; set; }
        public string OriginatingServer { get; set; }
        public string DatabaseIssueWarningQuota { get; set; }
        public string DatabaseProhibitSendQuota { get; set; }
        public string DatabaseProhibitSendReceiveQuota { get; set; }
    }
}
