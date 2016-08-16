#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 db" >&2
    exit 1
fi

set -e   # Exit immediately if a command exits with a non-zero status
cd "$1"

find . -type f -print | (
	max=0

   	while read pathname
   	do
   		n=`echo "$pathname" | tr -dc '/' | wc -c`

   		if [ $n -gt $max ]
   		then
   			max=$n
   		fi
   	done

   	echo $max
)
