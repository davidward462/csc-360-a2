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
n = 200
mean: 84372.14 clock cycles
standard deviation: 2482.46

Multithreding:
n = 200
mean: 130840.18
standard deviation: 8543.43 clock cycles

----- Synchronization issues -----

The non-multithreaded function of the program did not have any issues at any point.

Early in development, my process_file() function took the file path (e.g. data_files/Victoria.dat) and it seemed to work without issues when using multithreading mode. However, in order to use locks, and at the recommendation of the professor, I changed process_file() to take an integer which ranged from 0 to 9, which is the index of a global array holding the names of the cities. 

This new structure of process_file() now caused syncronization issues, where it seemed some city files were read more than once, and their data was printed multiple times. Generally this will be on index 9, the final iteration of the loop. 

----- Inconsistenties -----

While using multithreading, the main error was that some of the print statements showed the wrong files being read, specifically the last file output being duplicated throughout the output.
