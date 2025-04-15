using Microsoft.AspNetCore.Mvc;
using Microsoft.Identity.Web;
using Newtonsoft.Json;
using PowerBIWebApp.Models;
using RestSharp;
using System.Diagnostics;

namespace PowerBIWebApp.Controllers;

public class HomeController : Controller
{
    private readonly ILogger<HomeController> _logger;
    private readonly ITokenAcquisition _tokenAcquisition;
    private readonly string[] scopes = new string[] { "https://analysis.windows.net/powerbi/api/.default" };

    public HomeController(
        ILogger<HomeController> logger,
        ITokenAcquisition tokenAcquisition)
    {
        _logger = logger;
        _tokenAcquisition = tokenAcquisition;
    }

    [AuthorizeForScopes(Scopes = new string[] { "https://analysis.windows.net/powerbi/api/.default" })]
    public async Task<IActionResult> Index([FromQuery] string reportId = "ec7ae70b-aad8-44e3-ba6b-ab1ea66d9d64")
    {        
        var accessToken = await _tokenAcquisition.GetAccessTokenForUserAsync(scopes);

        RestClient restClient = new RestClient("https://api.powerbi.com/v1.0");
        RestRequest restRequest = new RestRequest($"/myorg/reports/{reportId}", Method.Get);
        restRequest.AddHeader("Authorization", $"Bearer {accessToken}");
        RestResponse response = restClient.Execute(restRequest);
        var reportModel = JsonConvert.DeserializeObject<PowerBIReportModel>(response.Content);
        reportModel.accessToken = accessToken;
        reportModel.TokenType = TokenType.Aad;
        reportModel.type = "report";

        return View(reportModel);
    }

    public IActionResult Privacy()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
