// See https://aka.ms/new-console-template for more information

using Encryption;

var keyPair = RSAProvider.GetKeyPair();
string publicKey = keyPair.Item1;
string privateKey = keyPair.Item2;
string encryptedMessage = "";

RSAProvider rsaProvider = new RSAProvider(privateKey);
string plainPassword = rsaProvider.Decrypt(encryptedMessage);

