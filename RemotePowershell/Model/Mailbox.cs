namespace RemotePowershellNet.Model
{
    public class Mailbox
    {
        public string PrimarySmtpAddress { get; set; }
        public string UserPrincipalName { get; set; }
        public string ProhibitSendQuota { get; set; }
        public string ProhibitSendReceiveQuota { get; set; }
        public string IssueWarningQuota { get; set; }
    }
}
