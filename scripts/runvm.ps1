$HostName = "TestK-PC"
$VMName = "Windows 7"
$VM = Get-VM -Name $VMName

$VMStateString = $VM.Name + " is " + $VM.State

write-host $VMStateString -ForegroundColor Yellow

if($VM.State -eq "Running")
{
    $ConnectString = "Connect to " + $HostName
    write-host $ConnectString -ForegroundColor Yellow
    mstsc /v $HostName
}
else
{
    Start-VM -Name $VMName
    write-host "Waking up VirtualMachine ... " -ForegroundColor Yellow

    Start-Sleep -s 5

    while($true)
    {
        $result = Test-NetConnection $HostName -Port 3389 -WarningAction SilentlyContinue
        
        if($result.TcpTestSucceeded -eq "True")
        {
            write-host "Wait until VirtualMachine is ready ... " -ForegroundColor Yellow
            Start-Sleep -s 10
            break
        }

        Start-Sleep -s 5
    }
    
    $ConnectString = "Connect to " + $HostName
    write-host $ConnectString -ForegroundColor Yellow

    mstsc /v $HostName
}
