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

----- Synchronization issues -----

The non-multithreaded function of the program did not have any issues at any point.

Initially, my process_file function took the file path as it's argument, at at the time there didn't seem to be any synchronization issues, but I was not using locks.

I changed the process_file function to take an integer which indicated the city name in a global array. At this time, my multithreading had issues, which was where 10 file info blocks would print, which is the expected number, but the last file (corresponding to the integer 9 in the array) would be what most of them were. As if a thread was running too many times, or was getting the wrong argument and therefore opening the wrong file.

----- Inconsistenties -----

While using multithreading, the main error was that some of the print statements showed the wrong files being read, specifically the last file output being duplicated throughout the output.
