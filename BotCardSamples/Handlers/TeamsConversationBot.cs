using AdaptiveCards.Templating;
using BotCardSamples.Helper;
using BotCardSamples.Models;
using Microsoft.Bot.Builder;
using Microsoft.Bot.Builder.Teams;
using Microsoft.Bot.Schema;
using Microsoft.Bot.Schema.Teams;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Diagnostics.Metrics;

namespace BotCardSamples.Handlers
{
    public class TeamsConversationBot : TeamsActivityHandler
    {
        private readonly string tenentId;
        private readonly string clientId;
        private readonly string clientSecret;
        private readonly string templatePath;

        public TeamsConversationBot(
            IConfiguration configuration)
        {
            tenentId = configuration.GetValue("Chat:TenentId", "");
            clientId = configuration.GetValue("Chat:MicrosoftAppId", "");
            clientSecret = configuration.GetValue("Chat:MicrosoftAppPassword", "");

            templatePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "CardTemplates");            
        }

        private async Task<TeamsChannelAccount> GetMember(ITurnContext<IMessageActivity> turnContext, string aadObjectId, CancellationToken cancellationToken)
        {
            return await TeamsInfo.GetMemberAsync(turnContext, turnContext.Activity.From.Id, cancellationToken);
        }

        protected override async Task OnMessageActivityAsync(ITurnContext<IMessageActivity> turnContext, CancellationToken cancellationToken)
        {                     
            string inputText = turnContext.Activity.Text;

            if(string.IsNullOrEmpty(inputText))
            {

            }
            else
            {
                if (inputText.ToLower().Contains("hi"))
                {
                    await OnInformationInputActivityAsync(turnContext, cancellationToken);
                }
                else if (inputText.ToLower().Contains("login"))
                {
                    await PromptLoginActivityAsync(turnContext, cancellationToken);
                }
                else if (inputText.ToLower().Contains("dynamic search"))
                {
                    await DynamicSearchActivityAsync(turnContext, cancellationToken);
                }
                else if(inputText.ToLower().Contains("static search"))
                {
                    await StaticSearchActivityAsync(turnContext, cancellationToken);
                }
            }
            
            await base.OnMessageActivityAsync(turnContext, cancellationToken);
        }
      
       
        private async Task OnInformationInputActivityAsync(ITurnContext<IMessageActivity> turnContext, CancellationToken cancellationToken)
        {
            
            string cardTemplate = File.ReadAllText(Path.Combine(templatePath, "InformationInputCard.json"));
            var inputs = JsonHelper.SearchJson(cardTemplate, "Input.");

            AdaptiveCardTemplate template = new AdaptiveCardTemplate(cardTemplate);
            var adaptiveCardAttachment = new Attachment
            {
                ContentType = "application/vnd.microsoft.card.adaptive",
                Content = JsonConvert.DeserializeObject(template.Expand()),
            };

            IMessageActivity infomationInputActivity = MessageFactory.Attachment(adaptiveCardAttachment);            
            var response = await turnContext.SendActivityAsync(infomationInputActivity, cancellationToken);           
        }

        private async Task PromptLoginActivityAsync(ITurnContext<IMessageActivity> turnContext, CancellationToken cancellationToken)
        {   
            try
            {                               
                List<CardAction> cardButtons = new List<CardAction>();
                CardAction plButton = new CardAction()
                {
                    Value = "https://localhost:5001",
                    Type = ActionTypes.OpenUrl,
                    Title = "Signin"
                };
                cardButtons.Add(plButton);

                SigninCard plCard = new SigninCard("Please click to sign in localhost", cardButtons);
                IMessageActivity loginActivity = MessageFactory.Attachment(plCard.ToAttachment());
                var response = await turnContext.SendActivityAsync(loginActivity, cancellationToken);
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }            
        }

        public async Task StaticSearchActivityAsync(ITurnContext<IMessageActivity> turnContext, CancellationToken cancellationToken)
        {
            var member = await GetMember(turnContext, turnContext.Activity.From.AadObjectId, cancellationToken);
            string cardTemplate = File.ReadAllText(Path.Combine(templatePath, "StaticSearchCard.json"));
         
            AdaptiveCardTemplate template = new AdaptiveCardTemplate(cardTemplate);
            var payloadData = new
            {
                createdById = member.Id,
                createdBy = member.Name
            };

            var cardJsonstring = template.Expand(payloadData);
            var adaptiveCardAttachment = new Attachment()
            {
                ContentType = "application/vnd.microsoft.card.adaptive",
                Content = JsonConvert.DeserializeObject(cardJsonstring),
            };

            IMessageActivity loginActivity = MessageFactory.Attachment(adaptiveCardAttachment);
            var response = await turnContext.SendActivityAsync(loginActivity, cancellationToken);
        }

        private async Task DynamicSearchActivityAsync(ITurnContext<IMessageActivity> turnContext, CancellationToken cancellationToken)
        {
            var member = await GetMember(turnContext, turnContext.Activity.From.AadObjectId, cancellationToken);

            string cardTemplate = File.ReadAllText(Path.Combine(templatePath, "DynamicSearchCard.json"));
            AdaptiveCardTemplate template = new AdaptiveCardTemplate(cardTemplate);
            var payloadData = new
            {
                createdById = member.Id,
                createdBy = member.Name
            };

            var cardJsonstring = template.Expand(payloadData);
            var adaptiveCardAttachment = new Attachment()
            {
                ContentType = "application/vnd.microsoft.card.adaptive",
                Content = JsonConvert.DeserializeObject(cardJsonstring),
            };

            IMessageActivity loginActivity = MessageFactory.Attachment(adaptiveCardAttachment);
            var response = await turnContext.SendActivityAsync(loginActivity, cancellationToken);
        }

        protected override async Task<InvokeResponse> OnInvokeActivityAsync(ITurnContext<IInvokeActivity> turnContext, CancellationToken cancellationToken)
        {
            InvokeResponse adaptiveCardResponse;
            if (turnContext.Activity.Name == "application/search")
            {
                var searchData = JsonConvert.DeserializeObject<DynamicSearchCard>(turnContext.Activity.Value.ToString());
                var packageResult = JObject.Parse(await (new HttpClient()).GetStringAsync($"https://azuresearch-usnc.nuget.org/query?q=id:{searchData.queryText}&prerelease=true"));
                if (packageResult == null)
                {
                    var searchResponseData = new
                    {
                        type = "application/vnd.microsoft.search.searchResponse"
                    };

                    var jsonString = JsonConvert.SerializeObject(searchResponseData);
                    JObject jsonData = JObject.Parse(jsonString);

                    adaptiveCardResponse = new InvokeResponse()
                    {
                        Status = 204,
                        Body = jsonData
                    };
                }
                else
                {
                    var packages = packageResult["data"].Select(item => (item["id"].ToString(), item["description"].ToString()));
                    var packageList = packages.Select(item => { var obj = new { title = item.Item1, value = item.Item1 + " - " + item.Item2 }; return obj; }).ToList();
                    var searchResponseData = new
                    {
                        type = "application/vnd.microsoft.search.searchResponse",
                        value = new
                        {
                            results = packageList
                        }
                    };

                    var jsonString = JsonConvert.SerializeObject(searchResponseData);
                    JObject jsonData = JObject.Parse(jsonString);

                    adaptiveCardResponse = new InvokeResponse()
                    {
                        Status = 200,
                        Body = jsonData
                    };
                }

                return adaptiveCardResponse;
            }

            return null;
        }

    }
}
