#!/bin/bash
if [$# -lt 2]; then
	echo "Usage: (A) (B)";
	exit 1;
fi
for((i=0;i<$1;++i)); do
	echo -n "1";
done;
echo -n "*";
for((i=0;i<$2;++i)); do
	echo -n "1";
done;
echo "=";
