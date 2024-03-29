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
        static string appId = "{ClientId}";
        static string tenentId = "{TenentId}";
        static string appSecret = "{AppSecret}";
        static string userName = "{UserPrincipalName}";
        static string password = "{UserPassword}";
        static void Main(string[] args)
        {
            CallCertificate();
            //CallGraphMe().GetAwaiter().GetResult();
            //CallClientSecret().GetAwaiter().GetResult();
        }

        static void CallUserAssert()
        {

        }

        static void CallCertificate()
        {
            SecureString secString = new SecureString();
            "908910".ToList().ForEach(c => secString.AppendChar(c));

            X509Certificate2 appcert = new X509Certificate2(@"D:\exocert.pfx", secString);

            List<string> scopeURL = new List<string>();
            scopeURL.Add("https://outlook.office.com/.default");


            IConfidentialClientApplication confidentialClient = ConfidentialClientApplicationBuilder
                .Create(appId)
                .WithTenantId(tenentId)
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
                .WithClientSecret(appSecret)
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
                .Users[userName]
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
                .Users[userName]
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
