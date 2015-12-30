#!/bin/bash

freeze_iperf() {
        server_pid=$1
        client_pid=$2

        # freeze
        echo "echo 1 > /proc/$server_pid/freeze"
        echo "echo 1 > /proc/$client_pid/freeze"
        echo 1 > /proc/$server_pid/freeze
        echo 1 > /proc/$client_pid/freeze
        
        sleep 8 

        # unfreeze
        echo "echo 0 > /proc/$server_pid/freeze"
        echo "echo 0 > /proc/$client_pid/freeze"
        echo 0 > /proc/$server_pid/freeze
        echo 0 > /proc/$client_pid/freeze
}

TIME=30
rm Time${TIME}.client Time${TIME}.server

setsid iperf3 -s --logfile Time${TIME}.server &
server_pid=$!
# server enter virtual time
echo "echo 1000 > /proc/$server_pid/dilation"
echo 1000 > /proc/$server_pid/dilation
sleep 1

setsid iperf3 -c 127.0.0.1 -t $TIME --logfile Time${TIME}.client &
client_pid=$!
# client enter virtual time
echo "echo 1000 > /proc/$client_pid/dilation"
echo 1000 > /proc/$client_pid/dilation
sleep 10

freeze_iperf $server_pid $client_pid
sleep 2 
freeze_iperf $server_pid $client_pid
sleep 2
freeze_iperf $server_pid $client_pid
sleep 2
freeze_iperf $server_pid $client_pid
sleep 2
freeze_iperf $server_pid $client_pid

wait $client_pid
kill -s SIGKILL $server_pid

cat Time${TIME}.client

