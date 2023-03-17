*******************************
* CSC 360 assignment 2 Report *
*******************************

Name: David Ward
VNum: V00920409

----- Compiling and running -----

to build and then clean:
$ make
$ make clean

to run in linear mode:
$ ./data_av

to run in multithreading mode:
$ ./data_av -m


----- Execution time -----

Linear execution:

Program run 200 times
mean: 84372.14 clock cycles
standard deviation: 2482.46

Multithreding:

Program run 200 times
mean: 130840.18 clock cycles
standard deviation: 8543.43 clock cycles

I suspect the higher clock cycles are due to the locks. Evne though the threads are running concurrently, the locks on the global variables which need to be determined by the end of the program slow down execution. All threads need to access them, likely many times during their execution, so if they are locked, the thread will not continue.

----- Synchronization issues -----

The non-multithreaded function of the program did not have any issues at any point.

Initially, my process_file function took the file path as it's argument, at at the time there didn't seem to be any synchronization issues, but I was not using locks.

I changed the process_file function to take an integer which indicated the city name in a global array. At this time, my multithreading had issues, which was where 10 file info blocks would print, which is the expected number, but the last file (corresponding to the integer 9 in the array) would be what most of them were. As if a thread was running too many times, or was getting the wrong argument and therefore opening the wrong file.

I added locks onto my global variables, thinking they may be causing issues, but that did not fix it. I added an extra variable defined within the for-loop which was creating the threads, assuming the index in the for-loop was changing, but the bug remained. Eventually I moved the position of my pthread_join function to be inside the for-loop where the threads were being created, and that fixed the problem. 

The problem was that threads were not allowed to return until after all the threads had first been created. I think multiple threads were trying to join the same thread at once, which the man page says can result in undefinied behaviour.

----- Inconsistenties -----

While running with multithreading, sometimes the minimum total temperature would be different on some runs, but not all. This was fixed after I moved my pthread_join function.
