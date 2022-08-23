$PasswordEncryptedAsHex = "01000000D08C9DDF0115D1118C7A00C04FC297EB01000000D43F1D3529BDC94BA1C547B9F9F9611800000000020000000000106600000001000020000000BBB57E271CDAD2BBF53BFDA716EBBA4F3B2F47187517B0251C9E08F2943F4370000000000E8000000002000020000000EC47A34D9581CB8833C186B60F16598DA25E0FED603999A8B45F01E76DE5F40220000000528B1A769B75DCCDEB3ED2549B597EC02F483F86AF6DCD513DB4887A71C856BC4000000025F08F62E23547E822B1EFE93F7AA1AFEA66B51EF1F5EF417B6ADF0E1A6AB251864BB01B820269386A58750F793E0D4AB8738D9E2B239C9CB5EA5E6921B977E7"

Add-Type -AssemblyName System.Security

[System.Text.UnicodeEncoding]$encoding = [System.Text.Encoding]::Unicode
[int]$passwordEncryptedAsBytesLength = $PasswordEncryptedAsHex.Length / 2
[byte[]]$passwordEncryptedAsBytes = New-Object -TypeName byte[] -ArgumentList $passwordEncryptedAsBytesLength
for ($i = 0; $i -lt $passwordEncryptedAsBytesLength; $i++) {
    $passwordEncryptedAsBytes[$i] = [Convert]::ToByte($PasswordEncryptedAsHex.Substring($i * 2, 2), 16)
}
[byte[]]$passwordAsBytes = [System.Security.Cryptography.ProtectedData]::Unprotect($passwordEncryptedAsBytes, $null, [System.Security.Cryptography.DataProtectionScope]::CurrentUser)
[string]$password = $encoding.GetString($passwordAsBytes)
return $password