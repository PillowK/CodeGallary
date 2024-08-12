using System.Text.Json;

namespace BotCardSamples.Helper
{
    public class JsonHelper
    {
        private static void SearchJsonRecursive(JsonElement element, string searchValue, ref List<JsonElement> returnList)
        {
            if (element.ValueKind == JsonValueKind.Object)
            {
                foreach (var property in element.EnumerateObject())
                {
                    if (property.Value.ValueKind == JsonValueKind.String && property.Value.GetString().Contains(searchValue))
                    {
                        returnList.Add(element.Clone());
                    }
                    else
                    {
                        SearchJsonRecursive(property.Value, searchValue, ref returnList);
                    }
                }
            }
            else if (element.ValueKind == JsonValueKind.Array)
            {
                foreach (var item in element.EnumerateArray())
                {
                    SearchJsonRecursive(item, searchValue, ref returnList);
                }
            }
        }

        public static List<JsonElement> SearchJson(string jsonString, string value)
        {
            List<JsonElement> returnElements = new List<JsonElement>();

            using (JsonDocument jsonDoc = JsonDocument.Parse(jsonString)) 
            {
                SearchJsonRecursive(jsonDoc.RootElement, value, ref returnElements);
                return returnElements;
            }
        }
    }
}
