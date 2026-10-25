param([string]$Config = "Release")

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
if (-not $msbuild) { throw "MSBuild not found. Install Visual Studio with the C++ desktop workload." }

& $msbuild "$PSScriptRoot\interim.vcxproj" /p:Configuration=$Config /p:Platform=Win32 /m /v:minimal
exit $LASTEXITCODE
