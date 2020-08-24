#! /bin/bash

set -eux -o pipefail

SCRIPT_DIR="$(readlink -f "$(dirname "$0")")"

rm -rf work
cp -a "$SCRIPT_DIR/work" work
chmod -R u=rwx,g=rx,o=rx work work/nested
chmod -R u=rw,g=r,o=r work/file work/nested/other

./testTrees "$SCRIPT_DIR/trees.cfg"
