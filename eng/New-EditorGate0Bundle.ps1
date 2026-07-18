[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string] $NativePackage,

    [Parameter(Mandatory = $true)]
    [string] $ManagedPackage,

    [string] $OutputDirectory = 'artifacts/editor-gate-0'
)

$ErrorActionPreference = 'Stop'
$repositoryRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$outputPath = [IO.Path]::GetFullPath((Join-Path $repositoryRoot $OutputDirectory))
if (-not $outputPath.StartsWith($repositoryRoot + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
    throw "OutputDirectory must remain beneath the SDK repository: $outputPath"
}

$nativePath = (Resolve-Path -LiteralPath $NativePackage).Path
$managedPath = (Resolve-Path -LiteralPath $ManagedPackage).Path
if (Test-Path -LiteralPath $outputPath) {
    Remove-Item -LiteralPath $outputPath -Recurse -Force
}
New-Item -ItemType Directory -Path $outputPath | Out-Null

$nativeCopy = Copy-Item -LiteralPath $nativePath -Destination $outputPath -PassThru
$managedCopy = Copy-Item -LiteralPath $managedPath -Destination $outputPath -PassThru
Copy-Item -LiteralPath (Join-Path $repositoryRoot 'docs\integration\editor-gate-0-handoff.md') -Destination $outputPath

$hashes = @($nativeCopy, $managedCopy) | ForEach-Object {
    $hash = Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256
    "{0}  {1}" -f $hash.Hash, $_.Name
}
$hashes | Set-Content -LiteralPath (Join-Path $outputPath 'SHA256SUMS.txt') -Encoding ascii

$manifest = [ordered]@{
    schema = 1
    gate = 'Editor Integration Gate 0'
    sdkVersion = '0.1.0'
    abiVersion = '1.1.0'
    architecture = 'win-x64'
    ownedCapabilities = @()
    nativePackage = $nativeCopy.Name
    managedPackage = $managedCopy.Name
}
$manifest | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $outputPath 'integration-manifest.json') -Encoding utf8

Get-ChildItem -LiteralPath $outputPath | Select-Object Name, Length
