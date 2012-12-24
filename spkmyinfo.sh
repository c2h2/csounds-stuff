#!/bin/sh

sleep 10
IP=`/sbin/ifconfig | grep -Eo 'inet (addr:)?([0-9]*\.){3}[0-9]*' | grep -Eo '([0-9]*\.){3}[0-9]*' | grep -v '127.0.0.1'`
GATE=`/sbin/ip route | awk '/default/ { print $3 }'`
MASK=`/sbin/ifconfig  | sed -rn '2s/ .*:(.*)$/\1/p'`

str="Initiation complete, IP address is: $IP, Gateway is: $GATE, Netmask is: $MASK"
/usr/bin/espeak "$str"
exit 0
