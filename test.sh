#!/bin/bash

rm -f testlog.txt

( make qemu > testlog.txt) & pid=$!


return_code=1
while [ $return_code -ne 0 ]
do
grep -q "#AT#TOKEN" testlog.txt
return_code=$?
sleep 1
done

kill -9 $pid
pkill -n  -9 qemu

EXPECT="#AT#TOKEN0"
CAR="$(tail -n 1 testlog.txt)"

if [ "$CAR"  = "$EXPECT" ]
then
echo "Tests succeeded, see testlogs.txt for more information"
exit 0
fi
echo $CAR "Tests Failed, see testlogs.txt for more information" | cut -d'N' -f 2
exit 1