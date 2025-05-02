#!/bin/bash
for file in $(find / -type f -name "*.so" 2>/dev/null);
do
    nm $1 "$file" > test1 ;
    ./ft_nm $1 "$file" > test2;
    if ! diff -q test1 test2 > /dev/null; then
        echo "$file"
        echo "$file" >> difffs
        diff test1 test2 | head -n 5 >> difffs
    fi
    rm test1 test2
done
