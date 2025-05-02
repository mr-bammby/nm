#!/bin/bash
IFS=':'
for dir in $PATH; do
    for f in $dir/*; do
        nm $1 "$f" > test1 ;
        ./ft_nm $1 "$f" > test2;
        if ! diff -q test1 test2 > /dev/null; then
            echo "$f"
            echo "$f" >> difffs
            diff test1 test2 | head -n 5 >> difffs
        fi
        rm test1 test2
    done
done
