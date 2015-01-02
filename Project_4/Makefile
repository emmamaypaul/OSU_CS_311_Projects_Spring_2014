#1)Emma Paul 
#2)paule@onid.oregonstate.edu
#3)CS311-400
#4)Homework #4
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
PROGS=sig_demo myar 

all: $(PROGS)

sig_demo: sig_demo.o
	$(CC) $(CFLAGS) -o sig_demo sig_demo.o

sig_demo.o: sig_demo.c
	$(CC) $(CFLASG) -c sig_demo.c 

myar: myar.o
	$(CC) $(CFLAGS) -o myar myar.o

myar.o: myar.c
	$(CC) $(CFLAGS) -c myar.c

#remove previous test files, and remake with files 1,2,3,4,5.
# @suppresses output... I only want to see the diff part

testq12345: myar.o
	@rm -f myar12345.ar ar12345.ar
	@ar q ar12345.ar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	@./myar -q myar12345.ar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	diff ar12345.ar myar12345.ar

testq135: myar.o
	@rm -f myar135.ar ar135.ar
	@ar q ar135.ar 1-s.txt 3-s.txt 5-s.txt
	@./myar -q myar135.ar 1-s.txt 3-s.txt 5-s.txt
	diff ar135.ar myar135.ar

testq24: myar.o
	@rm -f myar24.ar ar24.ar
	@ar q ar24.ar 2-s.txt 4-s.txt
	@./myar -q myar24.ar 2-s.txt 4-s.txt
	diff ar24.ar myar24.ar
	
testq: testq12345 testq135 testq24


testt12345: myar.o
	@rm -f ar12345.ar
	@ar q ar12345.ar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	@ar t ar12345.ar > ar-ctoc.txt
	@./myar -t ar12345.ar > myar-ctoc.txt
	diff ar-ctoc.txt myar-ctoc.txt

testt135: myar.o
	@rm -f ar135.ar
	@ar q ar135.ar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	@ar t ar135.ar > ar-ctoc.txt
	@./myar -t ar135.ar > myar-ctoc.txt
	diff ar-ctoc.txt myar-ctoc.txt

testt24: myar.o
	@rm -f ar24.ar
	@ar q ar24.ar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	@ar t ar24.ar > ar-ctoc.txt
	@./myar -t ar24.ar > myar-ctoc.txt
	diff ar-ctoc.txt myar-ctoc.txt

testt: testt12345 testt135 testt24

testv12345: myar.o
	@rm -f ar12345.ar
	@ar q ar12345.ar 1-s.txt 2-s.txt 3-s.txt 4-s.txt 5-s.txt
	@ar tv ar12345.ar > ar-vtoc.txt
	@./myar -v ar12345.ar > myar-vtoc.txt
	diff ar-vtoc.txt myar-vtoc.txt

testv135: myar.o
	@rm -f ar135.ar
	@ar q ar135.ar 1-s.txt 3-s.txt 5-s.txt
	@ar tv ar135.ar > ar-vtoc.txt
	@./myar -v ar135.ar > myar-vtoc.txt
	diff ar-vtoc.txt myar-vtoc.txt

testv24: myar.o
	@rm -f ar24.ar
	@ar q ar24.ar 2-s.txt 4-s.txt
	@ar tv ar24.ar > ar-vtoc.txt
	@./myar -v ar24.ar > myar-vtoc.txt
	diff ar-vtoc.txt myar-vtoc.txt

testv: testv12345 testv135 testv24

tests: myar testq testt testv

clean:
	rm -f $(PROGS) *.o *~ *#