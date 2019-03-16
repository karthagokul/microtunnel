#!/bin/bash

RETURN=0
CPPCHECK=$(which cppcheck)
if [ $? -ne 0 ]; then
	echo "[!] cppcheck not installed. Unable to check ." >&2
	exit 1
fi

FILES=`find -name *.cpp`
for FILE in $FILES; do
	$CPPCHECK  $FILE 
done
