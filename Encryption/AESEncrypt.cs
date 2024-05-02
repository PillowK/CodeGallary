using System.Security.Cryptography;
using System.Text;

namespace Encryption
{
    public class AESEncrypt
    {
        Aes _aes;
        private readonly byte[] passBytes;
        private readonly byte[] ivBytes;

        public AESEncrypt(string key)
        {
            this.passBytes = Encoding.UTF8.GetBytes(key);
            this.ivBytes = new byte[0x10];
            Array.Copy(passBytes, ivBytes, 16);

            _aes = Aes.Create("AesManaged");
            _aes.Mode = CipherMode.CBC;
            _aes.Padding = PaddingMode.PKCS7;
            _aes.KeySize = 0x80;
            _aes.BlockSize = 0x80;
            _aes.Key = passBytes;
            _aes.IV = ivBytes;
        }

        public string Encrypt(string plainText)
        {
            ICryptoTransform objtransform = _aes.CreateEncryptor();
            byte[] textDataByte = Encoding.UTF8.GetBytes(plainText);
            byte[] cipherBytes = objtransform.TransformFinalBlock(textDataByte, 0, textDataByte.Length);

            return Convert.ToBase64String(cipherBytes);
        }

        public bool TryEncrypt(string plainText, out string cipherString)
        {
            try
            {
                cipherString = Encrypt(plainText);
                return true;
            }
            catch (Exception)
            {
                cipherString = null;
                return false;
            }
        }

        public string Decrypt(string cipherString)
        {
            ICryptoTransform objtransform = _aes.CreateDecryptor();
            byte[] encryptedTextByte = Convert.FromBase64String(cipherString);
            byte[] TextByte = objtransform.TransformFinalBlock(encryptedTextByte, 0, encryptedTextByte.Length);

            return Encoding.UTF8.GetString(TextByte);
        }

        public bool TryDecrypt(string cipherString, out string plainText)
        {
            try
            {
                plainText = Decrypt(cipherString);
                return true;
            }
            catch (Exception)
            {
                plainText = null;
                return false;
            }
        }
    }
}
