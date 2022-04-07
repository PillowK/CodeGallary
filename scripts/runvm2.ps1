$hostName = "PillowK-FL"
$vmName = "Windows 10"
$vm = Get-VM -Name $vmName

$vmStateString = $vm.Name + " is " + $vm.State
write-host $vmStateString -ForegroundColor Yellow

if($vm.State -eq "Running") {
    vmconnect $hostName $vmName
} else {
    write-host "Waking up VirtualMachine ... " -ForegroundColor Yellow
    Start-VM -Name $vmName
    Start-Sleep -s 5
    vmconnect $hostName $vmName
}

