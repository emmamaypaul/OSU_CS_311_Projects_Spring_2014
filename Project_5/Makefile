#1)Emma Paul 
#2)paule@onid.oregonstate.edu
#3)CS311-400
#4)Homework #5
#
# $RCSfile$
# $Revision$
# $Author$
# $Date$
# $Log$
#

CC=gcc
DEBUG=-g
CFLAGS=$(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement
PROGS=pipeline uniqify 

all: $(PROGS)

pipeline: pipeline.o
	$(CC) $(CFLAGS) -o pipeline pipeline.o

pipeline.o: pipeline.c 
	$(CC) $(CFLASG) -c pipeline.c 

uniqify: uniqify.o
	$(CC) $(CFLAGS) -o uniqify uniqify.o

uniqify.o: uniqify.c 
	$(CC) $(CFLASG) -c uniqify.c 

test: pipeline.o
	@rm -f bash_outfile_1 bash_outfile_2 pipe_outfile_1 pipe_outfile_2
	@pipeline.bash -f bash_outfile_2 < words.txt > bash_outfile_1
	@pipeline -f pipe_outfile_2 < words.txt > pipe_outfile_1
	diff bash_outfile_1 pipe_outfile_1
	diff bash_outfile_2 pipe_outfile_2

clean: 
	rm -f $(PROGS) *.o *~ *#