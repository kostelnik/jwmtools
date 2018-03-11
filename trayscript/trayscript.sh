#!/bin/bash

# uncomments what's works for your pc

# get some temperatures
T1=0
T2=0
T3=0
#[ -f "/sys/class/hwmon/hwmon0/device/temp1_input" ] && T1=$((`cat /sys/class/hwmon/hwmon0/device/temp1_input`/1000))
#[ -f "/sys/class/hwmon/hwmon1/device/temp1_input" ] && T2=$((`cat /sys/class/hwmon/hwmon1/device/temp1_input`/1000))
#[ -f "/sys/class/hwmon/hwmon1/device/temp2_input" ] && T3=$((`cat /sys/class/hwmon/hwmon1/device/temp2_input`/1000))
CORE=`sensors | grep '°C' | awk '{ print $3 }' | sed 's/\+//;s/\.0//'`
HD=`hddtemp /dev/sda 2>/dev/null | awk '{ print $4 }'`

#find maximal temp
MAX=`echo -e "$CORE Core\n$HD HDD" | sort -n | tail -1`

# free disk volume
#DF=`df | grep '^\/dev' | awk '{ printf("%1.1f\n",$4/1000/1000) }' | head -2 | while read a; do echo -n $a"GB "; done`

# free disk space for multiple partitions, print smallest
DF1=`df /home | tail -1 | awk '{ printf("%1.1f\n",$4/1000/1000) }'`
DF2=`df / | tail -1 | awk '{ printf("%1.1f\n",$4/1000/1000) }'`
DF=`echo -e "$DF1\n$DF2" | sort -n | head -1`

# finally print it
echo -n "($MAX, free $DF GB)"
#echo -n "`date +%H:%M:%S`: Core:$T1°C, T2:$T2°C, T3:$T3°C, HDD:$HD, Max $MAX, Disk: $DF"

