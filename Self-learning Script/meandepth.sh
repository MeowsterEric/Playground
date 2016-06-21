#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 db" >&2
    exit 1
fi

cd "$1" || exit 1

files=`find . -type f -print | wc -l`
slashes=`find . -type f -print | tr -dc / | wc -c`

if [ $files -gt 0 ]
then
    expr ${slashes}0 / $files | sed 's/.$/.&/'
fi
