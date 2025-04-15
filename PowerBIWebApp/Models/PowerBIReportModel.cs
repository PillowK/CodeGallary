namespace PowerBIWebApp.Models
{
    public enum TokenType
    {
        Aad = 0,
        Embed = 1
    }

    public class PowerBIReportModel
    {
        public string type { get; set; }
        public string id { get; set; }       
        public string embedUrl { get; set; }                
        public string accessToken { get; set; }
        public TokenType? TokenType { get; set; }
    }
}
