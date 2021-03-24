ihm.o     : ihm.c ihm.h
	gcc -c ihm.c

client.o  : client.c ihm.h
	gcc -c client.c

client    : client.o ihm.o
	gcc -o client client.o ihm.o -lcurses

all:client