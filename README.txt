NAME OF PROJECT:
================
Project 2: Kernel Module and Processes

MEMBERS:
========
Jesse Roberts, Tony Rodriguez, Eli Hoehne

STATEMENT:
==========
We have neither given nor received unauthorized assistance on this work.


VIRTUAL MACHINE (VM) INFORMATION:
=================================
NAME OF VIRTUAL MACHINE (VM): group_12_vm
USENAME: trodriguez
PASSWORD: macrohard

Describe where the files can be found.
The files can be found in the /home/trodriguez/OS_P2 directory where 
each c file and its respective makefile are under their dedicated subdirectory
(Part1, part2, etc..)

Describe each file and the purpose it serves. 

Provide any special instructions to access or run your program.

The programs for part 1 and 2 are run using the basic commands (make, insmod, dmesg)

Part 3 is run like so: 
Load our module like so: insmod print_other.ko target_pid=$(cat random.txt)
Use the dmesg command as usual
If you would like to run the module again and want it to use a different arbitrary PID then after removing the module and running make clean, manually remove the random_pid.txt file and follow steps 1 - 3 again. 

ROOT PASSWORD: 
==============
macrohard

DESCRIPTION AND REMEDIATION:
============================
More thorough descriptions and problems faced for Part 1, Part 2, Part 3 and Part 4 are written the in the report