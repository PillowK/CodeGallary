using Microsoft.AspNetCore.Mvc;
using Microsoft.Bot.Builder.Integration.AspNet.Core;
using Microsoft.Bot.Builder;
using Microsoft.AspNetCore.Authorization;

namespace BotCardSamples.Controllers
{
    [Route("[controller]")]
    [ApiController]
    public class BotController : Controller
    {
        private readonly IBot _bot;
        private readonly IBotFrameworkHttpAdapter _adapter;

        public BotController(
            IBot bot,
            IBotFrameworkHttpAdapter adapter) 
        {
            _bot = bot;
            _adapter= adapter;
        }

        [AllowAnonymous]
        [HttpPost("api/messages")]        
        public async Task ProcessMessages()
        {
            await _adapter.ProcessAsync(Request, Response, _bot);
        }        
    }
}
