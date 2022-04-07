$source = $MyInvocation.MyCommand.Source
$source = $source -replace $MyInvocation.MyCommand.Name, "Code.exe"

if([System.IO.File]::Exists($source))
{
    $command = '"' + $source + '" ' + '"%1"'; 

    $regHkcr = [Microsoft.Win32.RegistryKey]::OpenBaseKey('ClassesRoot', 'Default');
    $subKey = $regHkcr.OpenSubKey("*\shell\Open with Code");

    if($subKey)
    {
        Write-Host "모든 파일에 메뉴가 이미 등록되어 있습니다."
    }
    else 
    {
        $subKey = $regHkcr.CreateSubKey("*\shell\Open with Code", $true);
        $newKey = $subKey.CreateSubKey("command", $true);
        $newKey.SetValue($null, $command, 'String');
    }

    $subKey = $regHkcr.OpenSubKey("Directory\shell\Open with Code");

    if($subKey)
    {
        Write-Host "폴더에 메뉴가 이미 등록되어 있습니다."
    }
    else 
    {
        $subKey = $regHkcr.CreateSubKey("Directory\shell\Open with Code", $true);
        $newKey = $subKey.CreateSubKey("command", $true);
        $newKey.SetValue($null, $command, 'String');
    }
}
else 
{
    Write-Host "실행 위치에서 VisualStudio Code 실행파일을 찾을 수 없습니다." -ForegroundColor Red
}
