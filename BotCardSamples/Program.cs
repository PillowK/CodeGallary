using BotCardSamples.Handlers;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc.Authorization;
using Microsoft.Bot.Builder;
using Microsoft.Bot.Builder.Integration.AspNet.Core;
using Microsoft.Bot.Connector.Authentication;
using Microsoft.Identity.Web;

var builder = WebApplication.CreateBuilder(args);
IEnumerable<string> initialScopes = builder.Configuration["DownstreamApi:Scopes"]?.Split(' ');

builder.Services.AddMicrosoftIdentityWebAppAuthentication(builder.Configuration, "AzureAd")
    .EnableTokenAcquisitionToCallDownstreamApi(initialScopes)    
    .AddDownstreamApi("DownstreamApi", builder.Configuration.GetSection("DownstreamApi"))        
    .AddInMemoryTokenCaches();

// Add services to the container.
builder.Services
    .AddSingleton<ServiceClientCredentialsFactory, ConfigurationPasswordCredentialFactory>()
    .AddSingleton<BotFrameworkAuthentication, ConfigurationBotFrameworkAuthentication>()
    .AddSingleton<IBotFrameworkHttpAdapter, AdapterWithErrorHandler>()
    .AddTransient<IBot, TeamsConversationBot>()
    .AddControllersWithViews()    
    .AddMvcOptions(options =>
    {
        var policy = new AuthorizationPolicyBuilder()
                  .RequireAuthenticatedUser()
                  .Build();
        options.Filters.Add(new AuthorizeFilter(policy));
    });


var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Bot/Error");
}
app.UseAuthentication();
app.UseStaticFiles();
app.UseRouting();
app.UseAuthorization();

app.UseEndpoints(endpoints =>
{
    endpoints.MapControllerRoute(
        name: "AreaReouteTemplate",
        pattern: "{area:exists}/{controller=Home}/{action=Index}/{id?}");

    endpoints.MapControllerRoute(
        name: "DefaultReouteTemplate",
        pattern: "{controller=Home}/{action=Index}/{id?}");
});

app.Run();
