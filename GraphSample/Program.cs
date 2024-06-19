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
HttpClient httpClient = new HttpClient();

GraphServiceClient graphClient = new GraphServiceClient(httpClient, authenticationProvider);

var me = graphClient.Me.GetAsync().GetAwaiter().GetResult();
var drive = graphClient.Me.Drive.GetAsync()
    .GetAwaiter()
    .GetResult();

var recent = graphClient.Drives[drive.Id].Recent
    .GetAsRecentGetResponseAsync()
    .GetAwaiter()
    .GetResult();

Console.ReadLine();
