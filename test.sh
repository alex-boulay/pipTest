#!/bin/bash

rm -f testlog.txt

(qemu-system-i386 -kernel build/x86_multiboot/meso.bin -serial stdio -m 1024 -nographic -net nic,model=rtl8139 -net dump,file=./netdump.pcap -net user -cpu Westmere  PARTITION=pipTest partition kernel > testlog.txt) & pid=$!


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
