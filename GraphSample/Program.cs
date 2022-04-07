using Microsoft.Graph;
using Microsoft.Graph.Auth;
using Microsoft.Identity.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Headers;
using System.Security;
using System.Security.Cryptography.X509Certificates;
using System.Threading.Tasks;

namespace GraphSample
{
    class Program
    {
        static string appId = "3b94e9a7-086f-4566-b359-bbdca7d08ddf";
        static string tenentId = "0f7b4e1c-344e-4923-aaf0-6fca9e6700c8";
        static string userName = "hcc_teams_admin@hanwha.com";
        static string password = "hcc1234!@#$";
        static void Main(string[] args)
        {
            CallCertificate();
            //CallGraphMe().GetAwaiter().GetResult();
            //CallClientSecret().GetAwaiter().GetResult();
        }

        static void CallCertificate()
        {
            SecureString secString = new SecureString();
            "908910".ToList().ForEach(c => secString.AppendChar(c));

            X509Certificate2 appcert = new X509Certificate2(@"D:\exocert.pfx", secString);

            List<string> scopeURL = new List<string>();
            scopeURL.Add("https://outlook.office.com/.default");


            IConfidentialClientApplication confidentialClient = ConfidentialClientApplicationBuilder
                .Create("175dea63-f81e-42c6-90c4-cc04ac9bcf85")
                .WithTenantId("60bf846f-b960-4abf-b752-5086ae3870a8")
                .WithCertificate(appcert)
                .Build();

            var token = confidentialClient.AcquireTokenForClient(scopeURL)
                .ExecuteAsync()
                .GetAwaiter()
                .GetResult();

            Console.WriteLine(token.AccessToken);
        }


        static async Task CallClientSecret()
        {
            List<string> scopeURL = new List<string>();
            scopeURL.Add("https://graph.microsoft.com/.default");

            IConfidentialClientApplication confidentialClient = ConfidentialClientApplicationBuilder
                .Create(appId)
                .WithTenantId(tenentId)
                .WithClientSecret("EOHjYaR2ixfDfj9Vy4ECfAZvPaNdZbLZn0euqoC196s=")
                .Build();

            var token = confidentialClient.AcquireTokenForClient(scopeURL)
                .ExecuteAsync()
                .GetAwaiter()
                .GetResult();

            GraphServiceClient graphClient = new GraphServiceClient(
                new DelegateAuthenticationProvider(
                    (request) =>
                    {
                        request.Headers.Authorization = new AuthenticationHeaderValue("bearer", token.AccessToken);
                        return Task.FromResult(0);
                    }));

            var user = await graphClient
                .Users["202102346@hanwha.com"]
                .Request()
                .Select(u => new
                {
                    u.AssignedLicenses,
                    u.Id
                })
                .GetAsync();
        }

        static async Task CallGraphMe()
        {
            List<string> scopeURL = new List<string>();
            scopeURL.Add("https://graph.microsoft.com/.default");

            
            SecureString secPassword = new SecureString();
            password.ToCharArray().ToList().ForEach(c => secPassword.AppendChar(c));
            

            IPublicClientApplication publicClient = PublicClientApplicationBuilder
                .Create(appId)
                .WithTenantId(tenentId)
                .Build();
                
            var userToken = publicClient.AcquireTokenByUsernamePassword(scopeURL, userName, secPassword)
                .ExecuteAsync()
                .GetAwaiter()
                .GetResult();

            var tokenExpiresOn = userToken.ExpiresOn;
            var localTime = DateTime.Now.ToUniversalTime();

            if(tokenExpiresOn.CompareTo(localTime) > 0)
            {
                Console.WriteLine("userToken valid");
            } else
            {
                Console.WriteLine("userToken expired");
            }


            GraphServiceClient graphClient = new GraphServiceClient(
                new DelegateAuthenticationProvider(
                    (request) =>
                    {
                        request.Headers.Authorization = new AuthenticationHeaderValue("bearer", userToken.AccessToken);
                        return Task.FromResult(0);
                    }));

            var user = await graphClient
                .Users["202102346@hanwha.com"]
                .Request()
                .Select(u => new
                    {
                        u.AssignedLicenses,
                        u.Id
                    })
                .GetAsync();
        }
    }
}
