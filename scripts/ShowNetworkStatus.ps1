$ipArray = @();

for($i=0;;$i++) {
    $ipArray += (Get-NetTCPConnection -RemotePort 443 -State Established -ErrorAction Ignore)
    $ipArray = $ipArray | sort -Unique
    cls
    $ipArray
    Start-Sleep -Seconds 1
}

