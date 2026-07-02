$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$emsdkDir = Join-Path $repoRoot ".tools\emsdk"
$localPython = Join-Path $emsdkDir "python\3.13.3_64bit\python.exe"
$localNodeDir = Join-Path $emsdkDir "node\22.16.0_64bit\bin"
$localNode = Join-Path $localNodeDir "node.exe"
$localEmscripten = Join-Path $emsdkDir "upstream\emscripten"
$localEmccPy = Join-Path $localEmscripten "emcc.py"
$webBuildDir = Join-Path $repoRoot "build\web"
$webTemplate = Join-Path $repoRoot "web\index.html"
$webOutput = Join-Path $webBuildDir "cvs_wasm.js"

if ((Test-Path $localPython) -and (Test-Path $localEmccPy)) {
    $env:EMSDK = $emsdkDir
    $env:EMSDK_NODE = $localNode
    $env:EMSDK_PYTHON = $localPython
    $env:PATH = "$emsdkDir;$localEmscripten;$localNodeDir;$env:PATH"
    $emccCommand = $localPython
    $emccPrefixArgs = @($localEmccPy)
}
else {
    $systemEmcc = Get-Command emcc -ErrorAction SilentlyContinue

    if (-not $systemEmcc) {
        Write-Error "Emscripten emcc not found. Install repo-local emsdk with scripts/setup_emsdk.sh or scripts/setup_emsdk.ps1, then rerun make web."
    }

    $emccCommand = $systemEmcc.Source
    $emccPrefixArgs = @()
}

New-Item -ItemType Directory -Force -Path $webBuildDir | Out-Null

$cvsSources = Get-ChildItem -Path (Join-Path $repoRoot "src\cvs") -Filter "*.c" | ForEach-Object { $_.FullName }
$sources = @($cvsSources) + @(Join-Path $repoRoot "src\web\cvs_wasm_api.c")

$args = @(
    $sources
    "-I", (Join-Path $repoRoot "src\cvs")
    "-I", (Join-Path $repoRoot "src\web")
    "-O3"
    "-s", "WASM=1"
    "-s", "MODULARIZE=1"
    "-s", "EXPORT_NAME=createCVSModule"
    "-s", "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    "-s", "EXPORTED_FUNCTIONS=['_cvs_wasm_init','_cvs_wasm_init_ex','_cvs_wasm_init_solver','_cvs_wasm_step','_cvs_wasm_run_steps','_cvs_wasm_get_time','_cvs_wasm_get_volume','_cvs_wasm_get_pressure','_cvs_wasm_get_flow','_cvs_wasm_get_e_lv','_cvs_wasm_get_e_rv']"
    "-o", $webOutput
)

& $emccCommand @emccPrefixArgs @args
Copy-Item -LiteralPath $webTemplate -Destination (Join-Path $webBuildDir "index.html") -Force
