$Password = "TeamsPlus12#$"

Add-Type -AssemblyName System.Security

[System.Text.UnicodeEncoding]$encoding = [System.Text.Encoding]::Unicode
[byte[]]$passwordAsBytes = $encoding.GetBytes($Password)
[byte[]]$passwordEncryptedAsBytes = [System.Security.Cryptography.ProtectedData]::Protect($passwordAsBytes, $null, [System.Security.Cryptography.DataProtectionScope]::CurrentUser)
[string]$passwordEncryptedAsHex = -join ($passwordEncryptedAsBytes | ForEach-Object { $_.ToString("X2") })


$passwordEncryptedAsHex