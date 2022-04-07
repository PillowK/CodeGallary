Import-Module WebAdministration
$ip = [System.Net.Dns]::GetHostAddresses("pillowk.iptime.org") | Select-Object -ExpandProperty IPAddressToString
Set-WebConfigurationProperty -filter "/system.applicationHost/sites/site[@name='SharedFiles' and @id='1']/ftpServer/firewallSupport" -name externalIp4Address -Value $ip
