#!/bin/sh
set -e

SQ=sqrl

for t in test-*.nut; do
    echo $t
    $SQ $t
done

echo "All tests ran OK"
