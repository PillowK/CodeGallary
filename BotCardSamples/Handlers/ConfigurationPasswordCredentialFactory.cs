using Microsoft.Bot.Connector.Authentication;

namespace BotCardSamples.Handlers
{
    public class ConfigurationPasswordCredentialFactory : PasswordServiceClientCredentialFactory
    {
        public ConfigurationPasswordCredentialFactory(
            IConfiguration configuration)
        {
            this.AppId = configuration.GetValue("Chat:MicrosoftAppId", "");
            this.Password = configuration.GetValue("Chat:MicrosoftAppPassword", "");
        }
    }
}
