#!/bin/sh

SQ=sqrl

$SQ assert-check.nut
if [ $? == 0 ]; then
    echo "FAILED: Squirrel interpreter doesn't properly return script exit status"
    exit 1
fi

set -e
for t in test-*.nut; do
    echo $t
    $SQ $t
done

echo "All tests ran OK"
