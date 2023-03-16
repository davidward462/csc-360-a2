#/usr/bin/bash

count=$1

echo " running program $count times..."

for i in $(seq 1 $count)
do
    ./data_av -mp >> multiResults.txt
done

echo "done."


