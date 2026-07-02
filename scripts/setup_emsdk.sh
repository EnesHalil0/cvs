#!/usr/bin/env sh
set -eu

REPO_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
EMSDK_DIR="$REPO_ROOT/.tools/emsdk"

if [ ! -d "$EMSDK_DIR/.git" ]; then
    mkdir -p "$(dirname "$EMSDK_DIR")"
    git clone https://github.com/emscripten-core/emsdk.git "$EMSDK_DIR"
fi

cd "$EMSDK_DIR"
./emsdk install latest
./emsdk activate latest

echo "Repo-local emsdk is ready under $EMSDK_DIR"
echo "No global PATH changes were made. Run make web from the repository root."
