Abstract
========

A testbed offering scalable and high fidelity network environment is critical to evaluate network protocol, architecture design and application test.
Container-based __network emulation__ is attractive because it bridges the gap between in-house research idea to real-lift deployment.
As a successful demonstration, Mininet enables network researchers execute unmodified binary on a large-scale emulated network using lightweight OS-level virtualization.

However, the OS-wide wall clock in emulation brings two major problem.
* First, time elapses in those containers that demand running but scheduled not to run.
This leads to the issue of temporal fidelity, especially with high workloads.
* Second, while many testing scenarios requires cooperation of simulation and emulation, they are running with different type of clock.
Event driven simulation advance time as long as new event occurs.

Virtual time sheds lights on both issues.
* It trades time with system resources via precisely scaling the time of interactions between containers and physical devices by a factor of _n_.
Hence an emulated network appears to be _n_ times faster from the viewpoints of applications inside the container.
* It also make synchronization with simulation possible since container's clock, as well as execution, could be frozen.

In this paper, we develop a lightweight Linux-container-based virtual time system.
To demonstrate its easy usage, we integrate it to Mininet for fidelity and scalability enhancement.
We also propose a solution for synchronization between general simulation and emulation clocks, based on the classic __Observer_ software design pattern.
Experimental results shows that 
1. With virtual time, Mininet is able to accurately emulate a network _n_ times larger in scale, 
where _n_ is the scaling factor, with the system behaviors closely match data obtained from a physical testbed
2. With the adaptive time dilation scheduling, we reduce the running time by 46\% with little accuracy loss.
3. The feasibility of synchronize simulation with emulation using freeze/unfreeze virtual time feature.
4. The overhead of both time dilation and freeze/unfreeze are trivial.
5. We present a case study using the virtual-time-enabled Mininet to evaluate the limitations of equal-cost multi-path (ECMP) routing in a data center network. 

Finally, we analysis how hardware timer limits our current virtual time system.


