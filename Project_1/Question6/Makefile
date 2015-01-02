#1) Emma Paul 
#2) paule@onid.oregonstate.edu
#3) CS211-400
#4) Homework#1
#
#  $RCSfile$
#  $Revision$
#  $Author$
#  $Date$
#  $Log$
#

CC=gcc
DEBUG=-g
CFLAGS=$(DEBUG) -Wall
PROGS=odd_even check_vowel helloworld read_file


all: $(PROGS)

odd_even: odd_even.o
	$(CC) $(CFLAGS) -o odd_even odd_even.o

odd_even.o: odd_even.c
	$(CC) $(CFLAGS) -c odd_even.c

check_vowel: check_vowel.o
	$(CC) $(CFLAGS) -o check_vowel check_vowel.o

check_vowel.o: check_vowel.c
	$(CC) $(CFLAGS) -c check_vowel.c

helloworld: helloworld.o
	$(CC) $(CFLAGS) -o helloworld helloworld.o

helloworld.o: helloworld.c
	$(CC) $(CFLAGS) -c helloworld.c

read_file: read_file.o 
	$(CC) $(CFLAGS) -o read_file read_file.o

read_file.o: read_file.c
	$(CC) $(CFLAGS) -c read_file.c

clean:
	rm -f $(PROGS) *.o *~

