#David Ward
#V00920409
#CSC 360 A2

all: data_av

data_av: data_av.o
	gcc -o data_av data_av.o -pthread

data_av.o: data_av.c
	gcc -c data_av.c

clean:
	rm data_av.o
