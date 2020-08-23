#! /bin/bash

set -eux -o pipefail

SCRIPT_DIR="$(readlink -f "$(dirname "$0")")"

rm -rf work
cp -a "$SCRIPT_DIR/work" work

./testTrees "$SCRIPT_DIR/trees.cfg"
