$counter = (Get-Counter "web service(eumweb)\current connections" -ComputerName EumWeb)
$connectionCount = $counter.CounterSamples.CookedValue
$sqlConn = New-Object System.Data.SqlClient.SqlConnection "Data Source=172.16.28.186, 1433; Initial Catalog=EumBoard; User ID=sa; Password=Pa`$`$w0rd@;"

$sqlConn.Open();
$sqlComm = $sqlConn.CreateCommand()
$sqlComm.CommandText = "UPDATE T_BB_BOARD_SYSTEM_CONFIG SET ConfigValue = $($connectionCount) WHERE ConfigName = 'ConnectionCount'"
$sqlComm.ExecuteNonQuery()

$sqlComm.Dispose()

$sqlConn.Close()
$sqlConn.Dispose()
