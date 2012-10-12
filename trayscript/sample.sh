#!/bin/bash

# get some temperatures
T1=$((`cat /sys/class/hwmon/hwmon0/device/temp1_input`/1000))
T2=$((`cat /sys/class/hwmon/hwmon1/device/temp1_input`/1000))
T3=$((`cat /sys/class/hwmon/hwmon1/device/temp2_input`/1000))
HD=`hddtemp /dev/sda | awk '{ print $4 }'`

#find maximal temp
MAX=`echo -e "$T1°C Core\n$T2°C T1\n$T3°C T2\n$HD HDD" | sort -n | tail -1`

# free disk volume
DF=`df | grep '^\/dev' | awk '{ printf("%1.1f\n",$4/1000/1000) }' | head -2 | while read a; do echo -n $a"GB "; done`

# finally print it
echo -n "`date +%H:%M:%S`: Core:$T1°C, T2:$T2°C, T3:$T3°C, HDD:$HD, Max $MAX, Disk: $DF"

