# What have been done since PADS 2015

1. Explore concurrent traffic flow emulation.
        * What factor influence emulation fidelity?
                * Kernel schedules TX and RX software interrupt __softirq__
                * Per CPU's network stack backlog queue, its length
        * Results: tunes these parameters vis sysctl but didn't see any improvement from experiment result

2. Proc file system interface for virtual time
        * /proc/$pid/dilation
        * /proc/$pid/freeze

3. Freeze/unfreeze new feature

4. Implementation of virtual time system
        * Where to modify?
                * ktime() family, monotonic time since boot (still needs working)
                * gettimeofday() family, EPOCH time since 1970
        * Dilation: change TDF without pausing emulation
        * Freeze/unfreeze: signal based implementation
        * Mininet Integration: add new feature to _Host_ and _Mininet_ class (needs working)

5. Auxiliary changes for transparency
        * Traffic control __tc__, htb and netem
                * How to dilate input bandwidth and latency parameters (still needs working)
                * __Watchdog__, e.g. hardware interrupt based timer, will prevent packets in transmission from being frozen
        * Ping uses _SOCK\_RAW_ time which is from ktime_get_real()

6. Evaluations
        * Error of dilation of 1 10 20 50 with different durations
        * Hard to evaluate error of freeze
        * Overhead of freeze
        * Mininet experiments: freeze ping and iperf

7. User land utilities
        * Command line tool: freeze\_all\_proc, dilate\_all\_proc
        * Take advantage of parallelism using pthread

8. Clock namespace, haven't got time to work on...
        * Motivation?
        * Advantage/disadvantage?
        * Implementation effort



