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

pIDS=''
linkOps=dict(bw=100)
numOfSwitches = int(sys.argv[1])
FileOut= sys.argv[2]
stime= float(sys.argv[4])

def pidList(net):
    global pIDS
    for host in net.hosts :
        pIDS += ' %s' % host.pid
    # just print hosts' pids
    print (pIDS)
    for s in net.switches :
        pIDS += ' %s' % s.pid
    for c in net.controllers:
        pIDS += ' %s' %c.pid
    

def pause():
    print NPAUSE
    process = subprocess.call(NPAUSE, shell=True)
    time.sleep(stime)
    print NRESUME
    process = subprocess.call(NRESUME, shell=True)
    #time.sleep(0.1)                            

class DssTopo(Topo):
    "DSS custom topo"

    def build(self):

        ################                                                                                                   
        #  start TOPO  #                                                                                                   
        ################                                                                                                   
        host1= self.addHost('h1')
        host2= self.addHost('h2')

        switchList=[]
        for i in range(numOfSwitches):

            s=self.addSwitch('switch%s'%i)
            switchList.append(s)
            if len(switchList) == 1 :
                self.addLink(host1, s, **linkOps)
            else :
                self.addLink(switchList[len(switchList)-2],s,**linkOps)
        self.addLink(host2,switchList[len(switchList)-1] , **linkOps)

def test():
    topo = DssTopo()
    net = Mininet(topo, link=TCLink)
    net.start()

    pidList(net)

    global NPAUSE
    global NRESUME

    NPAUSE = 'sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -f -p %s'%pIDS
    NRESUME ='sudo /home/kd/VirtualTimeKernel/test_virtual_time/freeze_all_procs -u -p %s'%pIDS

    net.get('h1').cmd('iperf3 -s > %s_server.iperf&' % FileOut)
    time.sleep(0.5)
    net.get('h2').cmd('iperf3 -c 10.0.0.1 -t 30 > %sbl.iperf 2>&1' % FileOut)
    
#    net.get('h1').cmd('iperf3 -s &')
    net.get('h2').cmd('iperf3 -c 10.0.0.1 -t 30 > %svt.iperf 2>&1 &' % FileOut)
    
    time.sleep(5)
    for x in range(0,int(sys.argv[3])):
        print 'pausing'
        pause()
        print 'resumed'
        time.sleep(stime)
        
    time.sleep(25) 
    net.stop()

if __name__ == '__main__':
   setLogLevel('info')
   test()
