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
import threading
import subprocess
import argparse

def pidList(net):
    pIDs = ''
    for host in net.hosts:
        pIDs += ' %s' % host.pid
    for s in net.switches:
        pIDs += ' %s' % s.pid
    for c in net.controllers:
        pIDs += ' %s' % c.pid
    # freeze ovs-switchd ovsdb-server
    pIDs += ' 1666 1667 '
    return pIDs

def pause(pIDs): 
    NPAUSE = 'sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -f -p %s' % pIDs
    NRESUME = 'sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -u -p %s' % pIDs
    output = subprocess.check_output(NPAUSE, shell=True)
    print 'freeze\n', output
    time.sleep(duration)
    output = subprocess.check_output(NRESUME, shell=True)
    print 'unfreeze\n', output

class DssTopo(Topo):
    "DSS custom topo"
    def build(self):
        host1 = self.addHost('h1')
        host2 = self.addHost('h2')
        switchList = []
        link_opts = {'bw':bandwidth, 'delay':'10us'}

        for i in range(num_sw):
            s = self.addSwitch('switch%s' % i)
            switchList.append(s)
            if len(switchList) == 1:
                self.addLink(host1, s, **link_opts)
            else:
                self.addLink(switchList[len(switchList)-2], s, **link_opts)
        self.addLink(host2, switchList[len(switchList)-1], **link_opts)

class iperfThread(threading.Thread):
    def __init__(self, net, perf_time, file_name):
        super(iperfThread, self).__init__()
        self.net = net
        self.perf_time = perf_time
        self.file_name = file_name

    def run(self):
        # self.net.get('h2').cmd('iperf3 -c 10.0.0.1 -t %s -b 100M > %sVir.log 2>&1' \
                # % (self.perf_time, self.file_name))
        self.net.get('h2').cmd('iperf3 -c 10.0.0.1 -t %s > %sVir.log 2>&1' \
                % (self.perf_time, self.file_name))
        return

def test():
    topo = DssTopo()
    net = Mininet(topo, link=TCLink)
    net.start()

    pIDs = pidList(net)

    # start iperf server
    net.get('h1').cmd('iperf3 -s > %sSrv.log &' % file_out)
    # make sure server has started
    time.sleep(0.5)
    
    start = time.time()
    net.get('h2').cmd('iperf3 -c 10.0.0.1 -t %s > %sBsl.log 2>&1' \
            % (perf_time, file_out))
    baseline_runtime = time.time() - start
    print "Runtime of baseline iperf = %f" % baseline_runtime

    frozen_iperf = iperfThread(net, perf_time, file_out)
    frozen_iperf.start()
    time.sleep(0.1)
    start = time.time()
    
    for x in range(0, num_pause):
        pause(pIDs)
        time.sleep(interval)
 
    frozen_iperf.join()
    frozen_runtime = time.time() - start
    print "Runtime of frozen iperf = %f" % frozen_runtime
    print "Time error = %f" \
            % (frozen_runtime - num_pause * duration - baseline_runtime)
    net.stop()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--num_sw', type=int, help='number of switches')
    parser.add_argument('-t', '--perf_time', type=int, help='iperf time', default=60)
    parser.add_argument('-b', '--bw', type=int, help='link bandwidth', default=100)
    parser.add_argument('-d', '--duration', type=float, \
            help='durtation of each pause in seconds', default=1.0)
    parser.add_argument('-i', '--interval', type=float, \
            help='interval between pauses in seconds', default=1.0)

    args = parser.parse_args()
    num_sw = args.num_sw
    perf_time = args.perf_time
    bandwidth = args.bw
    interval = args.interval
    duration = args.duration

    file_out = 'Sw%sBw%sFrz%sInt%s' % (num_sw, bandwidth, duration, interval)
    num_pause = int((perf_time - 2) / interval) - 1
    print "Schedule %d freeze operations" % num_pause
    setLogLevel('info')
    test()
 
