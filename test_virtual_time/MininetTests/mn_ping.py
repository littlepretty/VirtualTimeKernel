#!/usr/bin/python

#####################
#  channon@iit.edu  #
#####################

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost, Controller, OVSKernelSwitch, RemoteController, Host
from mininet.cli import CLI
from mininet.link import TCLink
from mininet.util import irange, dumpNodeConnections
from mininet.log import setLogLevel, info

####################
#  System Imports  #
####################

import sys
import time
import os
import subprocess
import argparse

######################
#  helper functions  #
######################
def pidList(net):
    pIDS = ''
    for host in net.hosts :
        pIDS += ' %s' % host.pid
    for s in net.switches :
        pIDS += ' %s' % s.pid
    for c in net.controllers:
        pIDS += ' %s' %c.pid
    return pIDS

def pause():
    process = subprocess.call(NPAUSE, shell=True)
    time.sleep(duration)
    process = subprocess.call(NRESUME, shell=True)
    # time.sleep(0.1)

class DssTopo(Topo):
    "DSS custom topo, just a chain switches with 2 hosts"
    def build(self):
        """start TOPO"""
        host1= self.addHost('h1')
        host2= self.addHost('h2')

        switchList=[]
        link_opts=dict(bw=10,delay='%sms' % latency)
        for i in range(num_sw):
            s = self.addSwitch('switch%s' % i)
            switchList.append(s)
            if len(switchList) == 1 :
                self.addLink(host1, s, **link_opts)
            else :
                self.addLink(switchList[len(switchList)-2], s, **link_opts)
        self.addLink(host2,switchList[len(switchList)-1], **link_opts)

def test():
    topo = DssTopo()
    net = Mininet(topo, link=TCLink)
    net.start()

    pIDS = pidList(net)

    global NPAUSE
    global NRESUME
    NPAUSE = 'sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -f -p %s' % pIDS
    NRESUME = 'sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -u -p %s' % pIDS
    
    print(net.get('h1').cmd('ping -c 1 10.0.0.2'))
    info('\n*** Start baseline test ***\n')
    net.get('h1').cmd('ping -c %s 10.0.0.2 > %sBsl.log' % (ping_count, file_out))
    
    # don't block
    info('\n*** Start freeze/virtual time test with custom Ping ***\n')
    net.get('h1').cmd('/home/kd/VirtualTimeKernel/iputils/ping -c %s 10.0.0.2 > %sVir.log &' % (ping_count, file_out))
    # make sure ping started before freeze
    time.sleep(1.2)

    for x in range(0, num_pause):
        # print 'pausing'
        pause()
        time.sleep(interval)
	# print 'resumed'

    net.stop()    

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--num_sw', type=int, help='number of switches')
    parser.add_argument('-c', '--ping_count', type=int, help='number of pings', default=40)
    # parser.add_argument('-f', '--file_out', type=str, help='name of output log file')
    # parser.add_argument('-p', '--num_pause', type=int, help='number of pauses during test', default=10)
    parser.add_argument('-d', '--duration', type=float, help='durtation of each pause in seconds', default=0.1)
    parser.add_argument('-l', '--latency', type=float, help='link delay in milliseconds', default=20)
    parser.add_argument('-i', '--interval', type=float, help='interval between pauses in seconds', default=0.1)

    args = parser.parse_args()
    
    num_sw = args.num_sw
    ping_count = args.ping_count
    duration = args.duration
    latency = args.latency
    interval = args.interval
    
    file_out = 'Sw%sLat%sFrz%sInt%s' % (num_sw, latency, duration, interval)
    num_pause = int((ping_count - 10) / interval) - 1

    setLogLevel('info')
    test()
