Install-Module AzureAD
$cred = Get-Credential

Connect-AzureAD -Credential $cred

$startDate = Get-Date
$endDate = $startDate.AddYears(98)
$aadAppsecret01 = New-AzureADApplicationPasswordCredential -ObjectId b09d3e1b-417d-425c-be05-9e46943d7207 -StartDate $startDate -EndDate $endDate