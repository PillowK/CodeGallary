
namespace GraphSample
{
    public class RequestInterceptor : DelegatingHandler
    {
        public RequestInterceptor()
        {
            InnerHandler = new HttpClientHandler();
        }

        protected override Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, CancellationToken cancellationToken)
        {
            Console.WriteLine($"{request.Method} {request.RequestUri}");
            return base.SendAsync(request, cancellationToken);
        }
    }
}
