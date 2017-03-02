<head>
<title>CS153 Lab 1: Fun with processes</title>
</head>
<body>
<h1>Lab 1: Fun with processes</h1>
<p>
<b>Handed out Sunday, April 10, 2016<br />
Design document due April 20, 2016</br />
Due Friday, April 29, 2016</b>

</p>

<h2>Introduction</h2>
<p>
There are two parts to this assignment: (1) Add a system call to xv6,
and (2) Change the scheduler to implement priority scheduling and priority inheritence.


<h3>Part 1: Add a new system call</h3>

In this part of the assignment, you need to understand the system call
implementation and add a new system call.  Repeating from the optional
lab:


<p>Open two terminal windows.  In one, enter <kbd>make
qemu-gdb</kbd> (or <kbd>make qemu-nox-gdb</kbd>).  This starts up
QEMU, but QEMU stops just before the
processor executes the first instruction and waits for a debugging
connection from GDB.  In the second terminal, from the same directory
you ran <tt>make</tt>, run <kbd>gdb</kbd>.  (Briefly, <tt>gdb -q -iex "set auto-load safe-path /home/csprofs/nael/xv6-master/"
</tt>.  Change the last part to your path to the xv6 directory.   You should see something
like this,</p>

<pre>
sledge% <kbd>gdb</kbd>
GNU gdb (GDB) 6.8-debian
Copyright (C) 2008 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later &lt;http://gnu.org/licenses/gpl.html&gt;
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i486-linux-gnu".
+ target remote localhost:26000
The target architecture is assumed to be i8086
[f000:fff0] 0xffff0:ljmp   $0xf000,$0xe05b
0x0000fff0 in ?? ()
+ symbol-file obj/kern/kernel
(gdb) 
</pre>

<p> Now set a breakpoint on exec() by typing 

<tt>break exec</tt> in the gdb window

type <tt>continue</tt> 

You should see something like:
<pre>
(gdb) cont
Continuing.
[New Thread 2]
[Switching to Thread 2]
The target architecture is assumed to be i386
=> 0x80100af8 <exec>:push   %ebp

Breakpoint 1, exec (path=0x1c "/init", argv=0x8dfffe98) at exec.c:12
12{
(gdb)
</pre>

<p>
Here we stop execution after the OS is initialized at the stage where
it is starting the first process (init).  If you type continue again,
you will break again as follows:

<pre>
gdb) cont
Continuing.
[Switching to Thread 1]
=> 0x80100af8 <exec>:push   %ebp

Breakpoint 1, exec (path=0x8c3 "sh", argv=0x8dffee98) at exec.c:12
12{
</pre>

<p>
As you can see, at this stage, init started a shell process which is
the xv6 shell we get when the OS boots.  If you continue again, gdb
will not return since it is waiting for a command to be started in the
shell.  Switch to the other window and try typing a command (fore
example, <tt>cat README</tt>) at which time you will get another break
as the shell forks then execs the cat program.

Feel free to look around at the program when it breaks to see how we
reach the system call which should give you ideas about how to add
one.

<h3>Assignment for this part</h3>

<p>First, extend the current xv6 process implementation to maintain an
exit status.  To get this done, add a field to the process structure
(see proc.h) in order to save an exit status of the terminated
process.  Next, you have to change all system calls affected by this
change (e.g., exit, wait etc.). 

<p>a) Change the exit system call signature to void exit(int status). The
exit system call must act as previously defined (i.e., terminate the
current process) but it must also store the exit status of the
terminated process in the corresponding structure.  In order to make
the changes in exit system call you must update the following files:
user.h, defs.h, sysproc.c, proc.c and all the user space programs that
uses exit system call.  Note, from now on, all user space programs
must supply an exit status when terminated.

<p>b) Update the wait system call signature to int wait(int *status). The
wait system call must prevent the current process from execution until
any of its child processes is terminated (if any exists) and return
the terminated child exit status through the status argument.  The
system call must return the process id of the child that was
terminated or -1 if no child exists (or unexpected error occurred).
Note that the wait system call can receive NULL as an argument. In
this case the childâ€™s exit status must be discarded.

<p>3) Add a waitpid system call: int waitpid(int pid, int *status, int
options).  This system call must act like wait system call with the
following additional properties: The system call must wait for a
process (not necessary a child process) with a pid that equals to one
provided by the pid argument.  The return value must be the process id
of the process that was terminated or -1 if this process does not
exist or if an unexpected error occurred.  We are required only to
implement a nonblocking waitpid where the kernel prevents
the current process from execution until a process with the given pid
terminates.  

<p>
Write a an example program to illustrate that your waitpid works.  You
have to modify the makefile to add your example program so that it can
be executed from inside the shell once xv6 boots.


<h3>Part 2: Implement priority scheduling</h3>

<p>In this part of the assignment, you will change the scheduler from a
simple round-robin to a priority scheduler.  Add a priority value to
each process that can be anywhere between 0 to 63.  When scheduling
from the ready list you will always schedule the highest priority
thread/process first.

<p>Add a system call to chance the priority of a process.  A process can
change its priority at any time.  If the priority becomes lower than
any process on the ready list, you must switch to that process.

<p> To get started, look at the file proc.c

<p> Explain how you would implement priority donation/priority
inheritence, or for 5\% bonus implement it. (Bonus 1)

<p> Add fields to track the scheduling performance of each process.  These
values should allow you to compute the turnaround time and wait time
for each process.  Add a system call to extract these values or
alternatively print them out when the process exits. 5\% bonus (Bonus 2)


<h3>Deliverables</h3>

There are two deliverables for this project, a walk-through and the
final program with two separate due dates outlined at the top of the
assignment.

The walk-through is a text document answering the following questions:
<ul>
<li> Explain how you would add your own hello world program so that you are able to execute it from the xv6 shell.

<li> Track the operation of the wait system call using the debugger.  Explain what happens when the process has no childern, and when it has a child process.

<li> Explain how the current scheduler works, including interaction with the timer.
</ul>

</body> 
