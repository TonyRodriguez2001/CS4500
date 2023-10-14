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

Part 3 is run like so: Use the Makefile as normal: make
Load our module like so: insmod print_other.ko target_pid=$(cat random.txt)
Use the dmesg command as usual
If you would like to run the module again and want it to use a different arbitrary PID then after removing the module and running make clean, manually remove the random_pid.txt file and follow steps 1 - 3 again. 

ROOT PASSWORD: 
==============
macrohard

DESCRIPTION AND REMEDIATION:
============================Over the course of the project, we faced various problems. One such problem was the "No such file or directory," error where the kernel headers required for building kernel modules couldnt be found. We ended up solving this issue by running yum update which update our OS to the newest version and fixed the issue. Another major issue we encountered was when trying to load our module for task 3. When we would run insmod with the kernel object and the parameter needed for task 3, our VM would freeze and time out. We found that the while loop that traverses the parent tree ended up becoming an infinite loop and we were able to solve this by rewriting the condition for the loop. Overall, this project allowed us to learn 2 major concepts. Firstly, we learned how to write a Linux kernel module and secondly, we were able to learn how to write makefiles for the kernel modules. 
More thorough descriptions and problems faced for Part 1, Part 2, Part 3 and Part 4 are written the in the report
