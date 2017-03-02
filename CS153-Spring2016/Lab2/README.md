<center>
<h1>Lab 2: Concurrency and Memory</h1>
<p>
<b>Handed out Monday, May 9, 2016<br />
Walkthrough due May 23, 2016 <br />
Due Tuesday Friday June 3, 2016<br /></b>
</center>




<center>
<font color="#77aacc">
<h1>Lab 2: 
</font> 
</center> 

<h2>Objectives</h2> 

<p>There are two components to this assignment:</p> 

<ul>
<li>Implement a Semaphore synchronization primitive and use it to solve some sychronization problems

<li>Understand virtual memory and add new VM features to xv6 that is common in modern OSes.</li> 
</ul> 



<h2> Part 1: Semaphore and Synchronization problems</h2>

Songwei will release a version of xv6 which has an implementation of a
simple kernel level thread library.  In particular, the library just
supports a thread_create function that starts a new thread at a
specified function and passes a single argument to it.  More details
about the thread code will be provided by Songwei.

Your job in this part of the assignment is to implement a Semaphore
synchronization primitive.  Because the semaphore requires that we
block threads/wake them up its easier to implement it in the kernel
(like spinlock).  This makes it difficult to use them in user mode
(without creating a lot of system calls and some other various
difficulties).  Therefore, we will just use the sempahores and write
our synchronization problems in the kernel.

Simply speaking, each synchronization problem will be implemented
inside the kernel.  We will have one system call that you can call
with a parameter which is the synchronization problem number.  The
synchronization problem will be written in the kernel and started when
the system call is called.  Your user program will test the problems
by calling the system call with the appropriate parameter.  Its a hack
but it should work.



<h3> Part 1.1: Semaphore implementation</h3> 

<p> You should define a Semaphore data structure including the
semaphore count integer and an array holding the threads (which are
implemented as processes that share the same address space).  You
should then implement two functions, sem_acquire(Semaphore *s) and
sem_signal(Semaphore *s) to support the standard semaphore operations
on the semaphore.  Your implementation should work (be atomic) but it
should not be conservative (preventing activities unecessarily).

<h3> Part 1.2: Implement thread_yield(): add a function called
thread_yield() with the standard semantics of thread_yield.

  <h3> Part
1.3: Synchronization problems </h3>

For each problem, you will start the necessary number of threads
executing at the appropriate functions (e.g., if you have different
types of threads such as producers or consumers they will start at the
functions that implement these produces or consumers.  The threads
should print messages at the appropriate points in their execution
stream and the output should match the problem statement.  The code
should work with arbitrary thread yields.  

<p>Problem 1: H2O problem from the exam to help you get started; check
the answer in the key: You've just been hired by Mother Nature to help
her out with the chemical reaction to form water, which she doesn't
seem to be able to get right due to synchronization problems. The
trick is to get two Hydrogen (H) atoms and one Oxygen (O) atom
together at the same time to produce H2O. The atoms are threads. Each
H atom (thread) starts at procedure hReady when it is ready to react,
and each O atom (thread) starts at procedure oReady when it's
ready. (Credit: This problem is from the NachOS OS assignment 1)


<p>Problem 2: Implement the monkey colony problem that we discussed on
piazza.  Implement the first part, and the final part (with the
dominant monkey).

<!-- A number of cars are going around in a circle in two directions (some -->
<!-- cars clockwise and some cars counter clockwise).  At one point of the -->
<!-- circle there is a bridge that can only be traversed in one direction -->
<!-- at a time.  The bridge can only carry three cars at a time.  Each car -->
<!-- executes a thread_yield after every event (entering bridge, traversing -->
<!-- bridge, leaving bridge, going around circle and back again).  To make -->
<!-- things interesting, you may make the yield non-deterministic. -->

<!-- In a standard implementation, we could end up with the cars going in -->
<!-- one direction dominating the bridge (like readers in readers writers -->
<!-- locks).  Think of a solution to prevent this problem and implement it. -->


<p>Problem 3: A particular river crossing is shared by both
cannibals and missionaries.  A boat is used to cross the river, but
it only seats three people, and must always carry a full load.  In
order to guarantee the safety of the missionaries, you cannot put
one missionary and two cannibals in the same boat (because the cannibals would
gang up and eat the missionary), but all other combinations are legal.
You are to write two procedures: <b> MissionaryArrives</b> and 
<b>CannibalArrives</b> called by a missionary or cannibal when it
arrives at the river bank.  The procedures arrange the arriving
missionaries and cannibals into safe boatloads; once the boat is full,
one thread calls <b> RowBoat</b> and after the call to RowBoat, the
three procedures then return.  There should also be no undue waiting:
missionaries and cannibals should not wait if there are enough of them
for a safe boatload. (Credit: Problem from NachOS OS assignments).


<h2>Part 2: Null pointer dereference</h2>

 <h3>Overview</h3>

<p>In this part, you'll be changing xv6 to support a feature available
in virtually every modern OS: raising an exception when your program
dereferences a null pointer.  Sound simple? Well, there are a few
tricky details.</p>

<h2>Details</h2> 

<h3>Part 2.1: Null-pointer Dereference</h3> 

<p>In xv6, the VM system uses a simple two-level page table.  If you
do not remember the details, read <a
href="http://pages.cs.wisc.edu/~remzi/OSTEP/vm-smalltables.pdf">Section
20.3 of OS 3 easy steps</a>.  However, you may find the description in
Chapter 1 of the xv6 manual sufficient (and more relevant to the assignment).

As it currently is structured, user code is loaded into the very first
part of the address space. Thus, if you dereference a null pointer,
you will not see an exception (as you might expect); rather, you will
see whatever code is the first bit of code in the program that is
running. Try it and see!</p>

<p>Thus, the first thing you might do is create a program that dereferences a
null pointer. It is simple! See if you can do it. Then run it on Linux as well
as xv6, to see the difference.</p> 

<p>Your job here will be to figure out how xv6 sets up a page table. Thus,
once again, this project is mostly about understanding the code, and not
writing very much. Look at how <code>exec()</code> works to better understand how
address spaces get filled with code and in general initialized. That will get
you most of the way.</p> 

<p>You should also look at <code>fork()</code> , in particular the part where the
address space of the child is created by copying the address space of the
parent. What needs to change in there?</p> 

<p>The rest of your task will be completed by looking through the code
to figure out where there are checks or assumptions made about the
address space. Think about what happens when you pass a parameter into
the kernel (using a system call), for example; if passing a pointer,
the kernel needs to be very careful with it, to ensure you haven't
passed it a bad pointer. How does it do this now? Does this code need
to change in order to work in your new version of xv6?</p>

<p>One last hint: you'll have to look at the xv6 makefile as well. In there
user programs are compiled so as to set their entry point (where the first
instruction is) to 0. If you change xv6 to make the first page invalid,
clearly the entry point will have to be somewhere else (e.g., the next page,
or 0x1000). Thus, something in the makefile will need to change to reflect
this as well.</p> 

<p>You should be able to demonstrate what happens when user code tries to
access a null pointer. If you do this part correctly, xv6 should trap and kill
the process without too much trouble on your part.</p> 

<h3>Part 2.2: Stack Rearrangement (Challenge: 5% + 5% bonus)</h3> 

<p>The xv6 address space is currently set up like this:
</p><pre>code
stack (fixed-sized, one page)
heap (grows towards the high-end of the address space)
</pre> 
<p></p> 

<p>In this part of the xv6 project, you'll rearrange the address space to look
more like Linux:
</p><pre>code
heap (grows towards the high-end of the address space)
... (gap)
stack (at end of address space; grows backwards)
</pre> 
<p></p> 

<p>This will take a little work on your part. First, you'll have to figure out
where xv6 allocates and initializes the user stack; then, you'll have to
figure out how to change that to use a page at the high-end of the xv6 user
address space, instead of one between the code and heap.</p> 

<p>Some tricky parts: one thing you'll have to be very careful with is how xv6
currently tracks the size of a process's address space (currently with the
<code>sz</code> field in the <code>proc</code> struct). There are a number of places in
the code where this is used (e.g., to check whether an argument passed into
the kernel is valid; to copy the address space). We recommend keeping this
field to track the size of the code and heap, but doing some other accounting
to track the stack, and changing all relevant code (i.e., that used to deal
with <code>sz</code> ) to now work with your new accounting.
</p> 

<p>You should also be wary of growing your heap and overwriting your stack.
In fact, you should always leave an unallocated (invalid) page between the
stack and heap.</p> 

<p>The high end of the xv6 user address space is 640KB (see the USERTOP value
defined in the xv6 code). Thus your stack page should live at 636KB-640KB.</p> 

<p>One final part of this project, which is challenging: automatically growing
the stack backwards when needed. Doing so would require you to see if a fault
occurred on the page above the stack and then, instead of killing the
offending process, allocating a new page, mapping it into the address space,
and continuing to run. Getting this to work will make you into a kernel boss,
and also get you those last few precious points.</p> 

<h2>The Code</h2> 

<p>You may also find the following readings about xv6 useful:
<a href="http://pdos.csail.mit.edu/6.828/2011/xv6/book-rev6.pdf">xv6 book.</a> </p> 

<p><b>Particularly useful for this project:</b> Chapter 1 + anything else about fork() and exec(),
as well as virtual memory.</p> 

<h2> Submission(s)</h2>

Like Lab1, there are two submissions, a walkthrough/design document
and the final submission.  The walkthrough is due on May 23, and the
final submission on June 3.

The walkthrough tentatively consists of the following questions (may
be subject ot modifications until May 11).  Where an outline of an
implementation is requested, you have to list all the major items
clearly:

<ul>
<li>Outline the implementation of the semaphore acquire function using pseudocode.  Also present the pseudocode for problem 3 of the synchronization problems.

<li>Reach chapter 2 in the xv6 book.  Briefly explain the operation of
kvmalloc() and mappages() and Figure 2-2.

<li>Outline your plan to implement Part 2.1
</ul>




