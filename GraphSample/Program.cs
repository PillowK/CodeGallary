using Azure.Identity;
using GraphSample;
using Microsoft.Graph;
using Microsoft.Kiota.Abstractions.Authentication;

string tenentId = "{TenentId}";
string clientId = "{ClientId}";
string secret = "{ClientSecret}";
string userAssert = "{UserAssertFromTeamsAuth}";

var authenticationProvider = new BaseBearerTokenAuthenticationProvider(
    new TokenProvider(clientId, tenentId, secret, userAssert));

RequestInterceptor requestInterceptor = new RequestInterceptor();
HttpClient httpClient = new HttpClient(requestInterceptor);

GraphServiceClient graphClient = new GraphServiceClient(httpClient, authenticationProvider);

var me = graphClient.Me.GetAsync().GetAwaiter().GetResult();
var drive = graphClient.Me.Drive.GetAsync()
    .GetAwaiter()
    .GetResult();

var recent = graphClient.Drives[drive.Id].Recent
    .GetAsRecentGetResponseAsync()
    .GetAwaiter()
    .GetResult();

ClientSecretCredential teamsplus = new ClientSecretCredential(
    "{TenantId}",
    "{ClientId}",
    "{SecretKey}");

GraphServiceClient teamsplusClient = new GraphServiceClient(httpClient, teamsplus);
var messageCollection2 = await teamsplusClient.Users["haha@teamsplus.kr"].Messages.GetAsync((config) =>
{
    //config.QueryParameters.Expand = new string[] { "singleValueExtendedProperties($filter=id eq 'Binary 0x300b')" };
    config.QueryParameters.Filter = "singleValueExtendedProperties/Any(ep: ep/id eq 'Binary 0x300b' and cast(ep/value,Edm.Binary) eq binary'6OQvbMJ+EkqhEi/DnKRrmw==')";
});

Console.ReadLine();

Console.ReadLine();
