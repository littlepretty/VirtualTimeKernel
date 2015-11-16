#!/usr/bin/python

#####################
#  channon@iit.edu  #
##################### 

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Controller, OVSKernelSwitch, Host
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


def pidList(net):
    pIDS = ''
    for host in net.hosts:
        pIDS += ' %s' % host.pid
    for s in net.switches:
        pIDS += ' %s' % s.pid
    for c in net.controllers:
        pIDS += ' %s' % c.pid
    return pIDS

def pause(): 
    process = subprocess.call(NPAUSE, shell=True)
    time.sleep(duration)
    process = subprocess.call(NRESUME, shell=True)

class DssTopo(Topo):
    "DSS custom topo"
    def build(self):
        host1 = self.addHost('h1')
        host2 = self.addHost('h2')
        switchList = []
        link_opts = dict(bw='%smb' % bw)

        for i in range(num_sw):
            s = self.addSwitch('switch%s' % i)
            switchList.append(s)
            if len(switchList) == 1:
                self.addLink(host1, s, **link_opts)
            else:
                self.addLink(switchList[len(switchList)-2], s, **link_opts)
        self.addLink(host2, switchList[len(switchList)-1], **link_opts)

def test():
    topo = DssTopo()
    net = Mininet(topo, link=TCLink)
    net.start()

    pIDS = pidList(net)

    global NPAUSE
    global NRESUME

    NPAUSE = 'sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -f -p %s' % pIDS
    NRESUME = 'sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -u -p %s' % pIDS

    # start iperf server
    net.get('h1').cmd('iperf3 -s > %s_server.iperf&' % file_out)

    # make sure server has started
    time.sleep(0.5)
    net.get('h2').cmd('iperf3 -c 10.0.0.1 -t %s > %sBsl.iperf 2>&1' % (perf_time, file_out))
    net.get('h2').cmd('iperf3 -c 10.0.0.1 -t %s > %sVir.iperf 2>&1 &' % (perf_time, file_out))

    time.sleep(0.8)
    for x in range(0, num_pause):
        pause()
        time.sleep(interval)

    net.stop()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--num_sw', type=int, help='number of switches')
    parser.add_argument('-t', '--perf_time', type=int, help='iperf time', default=60)
    parser.add_argument('-b', '--bw', type=int, help='link bandwidth', default=100)
    parser.add_argument('-d', '--duration', type=float, help='durtation of each pause in seconds', default=1.0)
    parser.add_argument('-i', '--interval', type=float, help='interval between pauses in seconds', default=1.0)

    args = parser.parse_args()
    num_sw = args.num_sw
    perf_time = args.perf_time
    bw = args.bw
    interval = args.interval
    duration = args.duration

    file_out = 'Sw%sLnk%sFrz%sInt%s' % (num_sw, bw, duration, interval)
    num_pause = int((perf_time - 2) / interval) - 1

    setLogLevel('info')
    test()
