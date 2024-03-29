using Microsoft.Identity.Client;
using Microsoft.Kiota.Abstractions.Authentication;

namespace GraphSample
{
    public class TokenProvider : IAccessTokenProvider
    {
        private readonly string _userAssert;
        public IConfidentialClientApplication Application { get; set; }
        public AllowedHostsValidator AllowedHostsValidator { get; }

        public TokenProvider(string clientId, string tenentId, string clientSecret, string userAssert)
        {
            this.AllowedHostsValidator = new AllowedHostsValidator();
            this.Application = ConfidentialClientApplicationBuilder
                .Create(clientId)
                .WithTenantId(tenentId)
                .WithClientSecret(clientSecret)
                .Build();

            _userAssert = userAssert;
        }

        public async Task<string> GetAuthorizationTokenAsync(
            Uri uri,
            Dictionary<string, object> additionalAuthenticationContext = null,
            CancellationToken cancellationToken = default)
        {
            List<string> scopeURL = new List<string>();
            scopeURL.Add("https://graph.microsoft.com/.default");

            var accessToken = await this.Application.AcquireTokenOnBehalfOf(
                scopeURL,
                new UserAssertion())
                .ExecuteAsync();

            return accessToken.AccessToken;
        }
    }
}
