#!/usr/bin/env bash -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd "${SCRIPT_DIR}"

if [ ! -d "${SCRIPT_DIR}/build" ]; then
    mkdir "${SCRIPT_DIR}/build"
fi

pushd "${SCRIPT_DIR}/build"
cmake -G "Unix Makefiles" "$@" . "${SCRIPT_DIR}"
make
popd

echo "Binaries are in build/bin"
