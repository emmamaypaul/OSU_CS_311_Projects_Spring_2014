/*1)Emma Paul
 *2)paule@onid.oregonstate.edu
 *3)CS 311-400
 *4)Homework #2
 *Note: I worked with Ian Paul and Andrew Goetz on this assignment discussing ideas, and shared references  
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <unistd.h> //http://www.gnu.org/software/libc/manual/html_node/Using-Getopt.html#Using-Getopt
#include <time.h>


int main(int argc, char *argv[])//argc is number of arguments, arggv[] is a pointer array which points to each argument passed in from something like Problem7.c -h -t -f
{

	//variables given 
	char c;
	struct utsname uname_pointer;
	time_t time_raw_format;
	struct stat s;
	char *fileName = NULL; //a pointer that we use from opttag to get the file name passed in the command line 

	//http://www.tutorialspoint.com/cprogramming/c_command_line_arguments.htm
	if(argc == 0)
	{
		printf("No arguments supplied, exiting program.");
		exit(0);//http://stackoverflow.com/questions/2425167/use-of-exit-function 
	}
	

	//https://www.youtube.com/watch?v=15nClbf4gAY 
	int hflg = 0;
	int tflg = 0;
	int fflg = 0;

	while ( (c = getopt(argc, argv, "htf:" )) != -1) //get opt equals -1 when no more options are present, so while there are options present //: after f means it takes an arguement (ie filename) 
	{
		switch(c) //use switch the switch c to be "h", "t", or "f"
		{
			case 'h':
				hflg=1;
				break; 
			case 't':
				tflg=1; 
				break; 
			case 'f':
				fflg = 1;
				fileName = optarg; //http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
				break; 
			case '?':
				if(optopt == 'f')
					fprintf(stderr, "Option -%c needs argument\n", optopt);
				else  //if command line option isn't h, t, or f 
					fprintf(stderr, "Unknown option -%c. \n", optopt);
			
				break; 
			default://if another error goes to default error 
				fprintf(stderr, "getopt"); 

		}
	
	}

	if(hflg == 1)
	{
		//print the hostname using the following commands: 
		uname(&uname_pointer);
		printf("Hostname = %s \n", uname_pointer.nodename); 
	}

	if(tflg == 1)
	{
		//print the current time using the following commands: 
		time(&time_raw_format); 
		printf("The current local time: %s", ctime(&time_raw_format));
	}

	if(fflg == 1)
	{
		//print the file size using the following commands: 
		if (stat(fileName, &s) == 0) 
		{
			printf("size of file '%s' is %d bytes\n", fileName, (int) s.st_size); 
		} 
		else
	        {
			printf("file '%s' not found\n", optarg); 
		}
	}
	return 0; //need to have a return I believe (like lecture) 

}
