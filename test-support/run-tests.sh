#!/bin/sh

SQ=sqrl

base=$(dirname $0)

if [ ! -f $base/assert-check.nut ]; then
    echo "$base/assert-check.nut not found"
    exit 1
fi

$SQ $base/assert-check.nut
if [ $? -eq 0 ]; then
    echo "FAILED: Squirrel interpreter doesn't properly return script exit status"
    exit 1
fi

set -e
for t in test-*.nut; do
    echo $t
    $SQ $t
done

echo "All tests ran OK"
