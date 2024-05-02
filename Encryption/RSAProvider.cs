using System.Security.Cryptography;
using System.Text;

namespace Encryption
{
#pragma warning disable CA1416
    public class RSAProvider
    {
        private readonly RSACng rsaProvider;
        private readonly string privateKey;

        public RSAProvider(string privateKey)
        {
            //IIS 응용프로그램 풀 > 고급설정 > 프로세스 모델 > 사용자 프로필 로드 = True설정 필요
            this.rsaProvider = new RSACng();
            this.privateKey = privateKey;
        }

        public static Tuple<string, string> GetKeyPair()
        {
            var rsaProvider = new RSACng();
            var spki = Convert.ToBase64String(rsaProvider.ExportSubjectPublicKeyInfo());
            var encodedPrivateKey = Convert.ToBase64String(rsaProvider.ExportPkcs8PrivateKey());
            return Tuple.Create(spki, encodedPrivateKey);
        }
        
        public string Decrypt(string encryptedString)
        {
            var span = (ReadOnlySpan<byte>)Convert.FromBase64String(privateKey);
            rsaProvider.ImportPkcs8PrivateKey(span, out int keyBytes);
            var encryptedBytes = Convert.FromBase64String(encryptedString);
            var decryptedBytes = rsaProvider.Decrypt(encryptedBytes, RSAEncryptionPadding.OaepSHA256);
            return Encoding.UTF8.GetString(decryptedBytes);
        }
    }
#pragma warning restore CA1416
}
