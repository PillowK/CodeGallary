using Azure.Identity;
using GraphSample.Models;
using Microsoft.Graph;

namespace GraphSample
{
    public interface IGraphPublicClientFactory
    {
        GraphServiceClient GetPublicClient(string username, string password);
    }

    public interface IGraphConfidentialClientFactory
    {
        GraphServiceClient GetConfidentialClient();
    }


    internal class GraphPublicClientFactory : IGraphPublicClientFactory
    {
        private readonly string applicationId;
        private readonly string tenentId;


        internal GraphPublicClientFactory(PublicClientOption option)
        {
            applicationId = option.ApplicationId;
            tenentId = option.TenentId;
        }

        public GraphServiceClient GetPublicClient(string username, string password)
        {
            List<string> scopeURL = new List<string>();
            scopeURL.Add("User.Read");

            UsernamePasswordCredential credential = new UsernamePasswordCredential(username, password, tenentId, applicationId);
            GraphServiceClient graphClient = new GraphServiceClient(credential);

            return graphClient;
        }
    }

    internal class GraphConfidentialClientFactory : IGraphConfidentialClientFactory
    {
        private readonly string applicationId;
        private readonly string tenentId;
        private readonly string clientSecret;
        private GraphServiceClient client = null;

        internal GraphConfidentialClientFactory(ConfidentialAppOption option)
        {
            applicationId = option.ApplicationId;
            tenentId = option.TenentId;
            clientSecret = option.ClientSecret;
        }

        public GraphServiceClient GetConfidentialClient()
        {
            if (client == null)
            {
                ClientSecretCredential credential = new ClientSecretCredential(tenentId, applicationId, clientSecret);
                client = new GraphServiceClient(credential);
            }

            return client;
        }
    }
}
