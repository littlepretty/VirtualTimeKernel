# VirtualTimeKernel

This repository is based on the [VT-Mininet](https://github.com/littlepretty/VirtualTimeForMininet), which
aims to improve the performance fidelity and scalability of [Mininet](http://mininet.org/).

## Motivation
Virtual time technique are mainly used for two applications in emulation/simulation:

1. Dilate emulation time so that more resources are available in emulation, e.g. link bandwidth.

2. Synchronize simulation and emulation, if they somehow need to work together, by freezing/unfreezing emulation freely.

In this project, we implement virtual time in the Linux kernel.
Mininet can use the exported virtual time features through /proc virtual file system so that
clocks inside emulated hosts/containers could be dilated/frozen/unfrozen.

## Installation
### Install Virtual Time Patch to Linux Kernel
* First, run `./install.sh both` at the root directory of this repo, which will automatically:
    * Download and unpack Linux kernel version 3.16.3
    * Copy source files to the unpacked kernel
    * Build new kernel

* Configure GRUB so that you can select which kernel to boot
```
sudo vim /etc/default/grub
# edit following lines
GRUB_HIDDEN_TIMEOUT=15
GRUB_HIDDEN_TIMEOUT_QUIET=false
# exit editing
update-grub
```

* Reboot and select the newly built kernel image in grub menu.

### Install VT-Mininet
You can install by rebuild Mininet:
```
cd mininet # enter the mininet dir inside this repo
sudo make clean
sudo make install
```

I will make a pull request to Mininet repo after more tests.

## What's New (Comparing to [VT-Mininet](https://github.com/littlepretty/VirtualTimeForMininet))
* We implement a different interface via /proc virtual file system.
Two entries are added to each process: dilation and freeze.
    * Now dilating a process/container will be as simple as:
    ```
    echo $TDF > /proc/$PID/dilation
    ```
    * Freezing a process/container can be done using:
    ```
    echo 1 > /proc/$PID/freeze # freeze/pause
    echo 0 > /proc/$PID/freeze # unfreeze/resume
    ```

* In `mnexec.c`, starting a container will call `unshare` with an extra flag `CLONE_NEWTIME`.

* With the /proc interface, we can parallelize the operation of dilating/freeze a number of hosts in Mininet.
Two utility programs, `dilate_all_proc` and `freeze_all_proc`, use pthread library to do "echoing" with multi-threads.

* In `net.py` I added two new method: `dilateEmulation(self, tdf)` and `freezeEmulation(self, op='freeze')`. Mininet users can use them to dilate/freeze emulation.


## Experiments with Virtual Time
Under `test_vtmininet` there are two experiments using [iperf3](https://github.com/esnet/iperf):

* `vtmn_fidelity_tcp.py` will run a network with string topology and test TCP throughput between 2 hosts.
* `vtmn_fidelity_udp.py` will run a network with string topology and test UDP throughput between 2 hosts.
* `vtmn_freeze_iperf.py` runs similar string-topo network and freeze emulation periodically.

More tests are on the way:

* `vtmn_freeze_ping.py`
* `vtmn_scalability.py`

## Known Issues
* Dilated host will accidentally exit clock namespace after iperf3 exits.
* iperf2 is not tested yet.

## Acknowledge
My implementation of virtual time is inspired by TimeKeeper wrote by Jeremy Lamps.
Great thanks for sharing his source code and providing a very detailed documentation.

