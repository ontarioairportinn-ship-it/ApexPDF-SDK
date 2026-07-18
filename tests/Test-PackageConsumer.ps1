[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string] $Package,

    [Parameter(Mandatory = $true)]
    [string] $WorkDirectory
)

$ErrorActionPreference = 'Stop'
$packagePath = (Resolve-Path -LiteralPath $Package).Path
$sourcePath = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot 'package_consumer')).Path
$workPath = [IO.Path]::GetFullPath($WorkDirectory)
$repositoryPath = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (-not $workPath.StartsWith($repositoryPath + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
    throw "WorkDirectory must remain beneath the SDK repository: $workPath"
}

if (Test-Path -LiteralPath $workPath) {
    Remove-Item -LiteralPath $workPath -Recurse -Force
}
New-Item -ItemType Directory -Path $workPath | Out-Null
$extractPath = Join-Path $workPath 'package'
Expand-Archive -LiteralPath $packagePath -DestinationPath $extractPath
$packageRoots = @(Get-ChildItem -LiteralPath $extractPath -Directory -Force)
if ($packageRoots.Count -ne 1) {
    throw "Package must contain exactly one root directory; found $($packageRoots.Count)"
}
$sdkRoot = $packageRoots[0].FullName
$buildPath = Join-Path $workPath 'build'

cmake -S $sourcePath -B $buildPath -G Ninja -DCMAKE_BUILD_TYPE=Release "-DAPX_SDK_ROOT=$sdkRoot"
if ($LASTEXITCODE -ne 0) { throw "Package consumer configuration failed: $LASTEXITCODE" }
cmake --build $buildPath
if ($LASTEXITCODE -ne 0) { throw "Package consumer build failed: $LASTEXITCODE" }
ctest --test-dir $buildPath --output-on-failure
if ($LASTEXITCODE -ne 0) { throw "Package consumer tests failed: $LASTEXITCODE" }
