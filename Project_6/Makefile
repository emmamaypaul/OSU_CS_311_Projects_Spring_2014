#1)Emma Paul 
#2)paule@onid.oregonstate.edu
#3)CS311-400
#4)Homework #6
#
# $RCSfile$
# $Revision$
# $Author$
# $Date$
# $Log$
#

CC=gcc
DEBUG=-g
CFLAGS=$(DEBUG) -Wall 
PROGS=primePThread primeMProc
LDFLAGS = -lm -lpthread -lrt
# 
#-Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement



all: $(PROGS)

primePThread: primePThread.o
	$(CC) $(CFLAGS) -o primePThread primePThread.o $(LDFLAGS)

primePThread.o: primePThread.c 
	$(CC) $(CFLASG) -c primePThread.c 

primeMProc: primeMProc.o
	$(CC) $(CFLAGS) -o primeMProc primeMProc.o $(LDFLAGS)

primeMProc.o: primeMProc.c 
	$(CC) $(CFLASG) -c primeMProc.c 



clean: 
	rm -f $(PROGS) *.o *~ *#