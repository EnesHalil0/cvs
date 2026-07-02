$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$emsdkDir = Join-Path $repoRoot ".tools\emsdk"

if (-not (Test-Path (Join-Path $emsdkDir ".git"))) {
    New-Item -ItemType Directory -Force -Path (Split-Path $emsdkDir) | Out-Null
    git clone https://github.com/emscripten-core/emsdk.git $emsdkDir
}

Push-Location $emsdkDir
try {
    .\emsdk.bat install latest
    .\emsdk.bat activate latest
}
finally {
    Pop-Location
}

Write-Host "Repo-local emsdk is ready under $emsdkDir"
Write-Host "No global PATH changes were made. Run make web from the repository root."
