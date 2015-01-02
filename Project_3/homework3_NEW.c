/*1)Emma Paul 
 *2)paule@onid.oregonstate.edu
 *3)CS311-400
 *4)Homework #3
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define ARGS "vf:l:o:O:e:" //http://h30097.www3.hp.com/docs/base_doc/DOCUMENTATION/V50_HTML/MAN/MAN3/0353____.HTM

int main(int argc, char *argv[]) //arg c is num of arguments, argv is pointer array to each command line  argument
{
	//variables for command line arguments 
	char c;//variable for command line option (characters)
	char *fileName = NULL; //pointer to get filename passed 
	int len, offset1, offset2; //believe should be an int for len (length of bytes read)
	//bool verbose = false; //to be used with verbose???, https://docs.python.org/2/library/getopt.html
	optarg = NULL; //seen in an example, not sure if necessary 
	struct stat sb; //to be used for symbolic 
	struct stat s; //used for length of file 
	int fd;
	int out_fd;
	char *buffer;
	int newOffset;
	int newOffset2;
	int newOffset3;
	int endOffset;

	//status flag variables each command line options (-v, -f, -l, -o, -O)  
	int vflg, fflg, lflg, oflg, Oflg, eflg = 0;

	//while loop to process command line arguments 
	while((c = getopt(argc, argv, ARGS))!= -1) //while there are command line options , argument expected after all but v (indicated by :)
	{	
		switch(c)
		{ 
			case 'v':
				vflg = 1;
				break;
			case 'f':
				fflg = 1;
				fileName = optarg;
				break;
			case 'l':
				lflg = 1;
				len = atoi(optarg);
				if(len < 0)
					printf("Number of bytes to read must be greater than 0");
				break;
			case 'o':
				oflg = 1;
				offset1 = atoi(optarg);
				if(offset1 < 0)
					printf("Argument for -o must be greater than 0");
				break;
			case 'O': 
				Oflg = 1;
				offset2 = atoi(optarg);
				break;
			case 'e':
				eflg = 1; 
				endOffset = atoi(optarg);
				break;
			case '?':
				if(optopt == 'f' || optopt == 'l' || optopt == 'o' || optopt == 'O')
					fprintf(stderr, "Option -%c requires an argument\n", optopt);
				break;
				
		}
	}


	//if statements to handle various command line arguments 
	if(vflg == 1)
	{
		//Verbose, show all the other command line arguments and any other print statements you find usefule 
		//output other text to the terminal (for verbose), use printf 
		//should we do a lot of if statements here using the status flags?
		printf("Command line options:\n\n");
		printf("verbose (shows command line options and argument info)--> -v [no argument]\n");
		printf("file (to be read) --> -f [argument=fileName]\n");
		printf("length (of bytes to read) --> -l [argument=number of bytes to be read]\n");
		printf("offset1 (fom beginning of file) --> -o [argument=number of bytes for offset1 (+)]\n");
		printf("offset2 (from current position of file) --> -O [argument=number of bytes for offset2 (+ or -)]\n");
	}
	if(fflg == 1)
	{
		fd=open(fileName, O_RDONLY);//fileName from command line argument 
		if(fd == -1)
		{
			printf("Error. Cannot open file. Exiting program."); 
			exit(1);
		} 
		else
		{
			printf("%s", fileName);
			printf("File successfully opened. \n\n");
		}	
	}
	if(lflg == 1)
	{
		if(oflg != 1 || Oflg != 1)
		{
	
			char buf[len]; //pg 71 TLPI, I think of buf like an array, allocating the number of bytes given after the -l command 

			if (fd == -1) //if file doesn’t exist 
			{
    			printf("Can’t open input file");
   				exit(1);
			}
			
			ssize_t numRead;

			numRead = read(fd, buf, len);
			if (write(out_fd, buf, len) != numRead)
				printf("Error writing whole buffer");
			if(numRead != -1) //if successfully read 
				printf("\n\n%d bytes read from file %s", len, fileName);

 			printf("%d", numRead);

		}
	}
	if(oflg == 1)
	{	
		if(lflg != 1)
		{
			printf("Please include -l [length].");
			exit(1);
		}

/*
			char *buffer;
			int out_fd; 

	
		newOffset = lseek(fd, offset1, SEEK_CUR);
		buffer = malloc(sizeof(char) * len); //find example google 
		printf("\n<offset1>-----------------------------------------------------\n");
		read(fd, buffer, len);
		write(1, buffer, len);
		printf("\n<END offset1>-----------------------------------------------------\n");
	
*/

		printf("\n<offset1>----------------------------------------------------\n");
		newOffset = lseek(fd, offset1, SEEK_CUR);
		char buf[len];
		read(fd, buf, len);
		write(1, buf, len);//arguments are (int file descriptor, const void *buf, size in bytes (given by user)
		printf("\n<END offset1>------------------------------------------------\n");
	}
	if (Oflg == 1)
	{
		if(lflg != 1)
		{
			printf("Please include -l [length].");
			exit(1);
		}

/*
		printf("\n<offset2>-----------------------------------------------------\n");
		newOffset2 = lseek(fd, offset2, SEEK_CUR);
		buffer = malloc(sizeof(char) * len); //find example google 
		read(fd, buffer, len);
		write(1, buffer, len);
		printf("\n<END offset2>-----------------------------------------------------\n");

*/
		printf("\n<offset2>----------------------------------------------------\n");
		newOffset2 = lseek(fd, offset2, SEEK_CUR);//from current position to offset given by user 
		char buf[len];
		read(fd, buf, len);
		write(1, buf, len);
		printf("\n<END offset2>------------------------------------------------\n");

	}
	if (eflg == 1)
	{
		if(lflg != 1)
		{
			printf("Please include -l [length].");
			exit(1);
		}
/*

		printf("\n<e>-----------------------------------------------------\n");
		newOffset3 = lseek(fd, -(endOffset), SEEK_END);
		buffer = malloc(sizeof(char) * len); //find example google 
		read(fd, buffer, len);
		write(1, buffer, len);
		printf("\n<END e>-----------------------------------------------------\n");

*/
		printf("\n<e>----------------------------------------------------------\n");
		newOffset3 = lseek(fd, -(endOffset), SEEK_END);//end minus end offset bytes --> http://cobweb.cs.uga.edu/~luper/1730/Notes/files-UNIX_6per.pdf
		char buf[endOffset];
		read(fd, buf, endOffset);
		write(1, buf, endOffset);
		printf("\n<END e>-----------------------------------------------------\n");

	}


	return 0; 

}






