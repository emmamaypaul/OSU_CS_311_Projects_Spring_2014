/*1)Emma Paul 
 *2)paule@onid.oregonstate.edu
 *3)CS311-400
 *4)Homework #5
 *Note: I worked with Ian Paul on this assignment by sharing helpful reference websites 
 */

#include "tlpi_hdr.h" //has typical includes 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

 #define ARGS "f:"

 int main(int argc, char *argv[])
 {
 	char c;//variable for command line option (-f)
	char * outfile2 = NULL; //pointer to get filename provided by user that we will be writing out to   
	int fflag = 0;

//declare pipes 
 	int pfd_1[2]; //Pipe 1: declare 2 pipe file descriptors , one for write end one for read end 
  	int pfd_2[2]; //Pipe 2
  	int pfd_3[2]; //Pipe 3 	
  	int pfd_4[2]; //Pipe 4
 	
	while((c = getopt(argc, argv, ARGS))!= -1) //while there are command line options , argument expected after all but v (indicated by :)	
	{
		switch(c)
		{ 
			case 'f':
				outfile2 = argv[2];//file name is second argument
				fflag = 1;
				break;
			case '?':
				printf("Error reading command line argument.");
				break;
		}
	}

if (fflag == 1)
{

//Set up all the pipes
	if (pipe(pfd_1) == -1) //create pipe using pipe system call, (it returns 0 on success and -1 on failures)
 	{
 		printf("Error creating pipe");
 	}
 	
	if (pipe(pfd_2) == -1) //create pipe using pipe system call, (it returns 0 on success and -1 on failures)
 	{
 		printf("Error creating pipe");
 	}
	if (pipe(pfd_3) == -1) //create pipe using pipe system call, (it returns 0 on success and -1 on failures)
 	{
 		printf("Error creating pipe");
 	}

	if (pipe(pfd_4) == -1) //create pipe using pipe system call, (it returns 0 on success and -1 on failures)
 	{
 		printf("Error creating pipe");
 	}
 	
//TLPI pg 892 //a successful call to pipe() returns two open file descriptors n the array filedes:
// one for the read end of the pipe → filedes[0], and one for write end → filedes[1]; 
//code below mainly referenced from TLIP ch 44 

 //Fork 1 
 	switch (fork()) //after fork the parent and child process file descripts both read and write in pipe 
 	//we have to close unused file descriptors to create the flow we want like pg 893
 	{
 		case -1: //when fork returns -1 it was not successful 
 			printf("Error forking child process for rev");

 		case 0: //when fork returns 0 it was successful. 
 		//The firt child binds its standard output to the write end of the pipe and then execs rev 
 			if(close(pfd_1[0]) == -1) //read end of child process is unused so we close it 
 			{
 				printf("Close 1: Error closing read end of parent process to pipe 1");
 			}
 			//duplicate stdout on write end of pipe, close duplicated descriptor 
 			if(pfd_1[1] != STDOUT_FILENO) //this is the defensive check in the case that standard input and output had both beed closed 
 			{
 				//like the image shows in TLPI pg 893, the code below duplicates the parent process file descriptors, then closes the unused write end of the child process 
 				if(dup2(pfd_1[1], STDOUT_FILENO) == -1) //this makes STDOUT_FILENO (new file descriptor) be the copy of pfd[1] (old file descriptor) which is the write end of the pipe
 				{
 					printf("Error making old file descriptor new file descriptor");
 				}
 				if(close(pfd_1[1]) == -1) //close unused write end of child process  
 				{
 					printf("Close 2: Error closing write end of child process to pipe 1");
 				}
 			}

			execlp("rev", "rev", (char *) NULL); //writes to pipe 
 			
 		default: //default is parents case, parent falls through to create next child  
 			break; //put break for good measure according to lecture 
 			
 	}

 //Fork 2
 	switch (fork()) //after fork the parent and child process file descripts both read and write in pipe 
 	//we have to close unused file descriptors to create the flow we want like pg 893
 	{
 		case -1: //when fork returns -1 it was not successful 
 			printf("Error forking child process for sort");

 		case 0: 
 			if(close(pfd_1[1]) == -1) //write end is unused so we close it 
 			{
 				printf("Close 3: Error closing write end of parent process to pipe 1");
 			}
 			if(close(pfd_2[0]) == -1) //read end is unused so we close it 
 			{
 				printf("Close 4: Error closing read end of parent process to pipe 2");
 			}
 			//duplicate stdout on write end of pipe, close duplicated descriptor 
 			if(pfd_1[0] != STDIN_FILENO) 
 			{
 				//like the image shows in TLPI pg 893, the code below duplicates the parent process file descriptors, then closes the unused write end of the child process 
 				if(dup2(pfd_1[0], STDIN_FILENO) == -1) //this makes STDOUT_FILENO (new file descriptor) be the copy of pfd[1] (old file descriptor) which is the write end of the pipe
 				{
 					printf("Error making old file descriptor new file descriptor");
 				}
 				if(close(pfd_1[0]) == -1) //close unused write end of child process  
 				{
 					printf("Close 5: Error closing write end of child proccess for pipe 1");
 				}
 			}

 			if(pfd_2[1] != STDOUT_FILENO) //this is the defensive check in the case that standard input and output had both beed closed 
 			{
 				//like the image shows in TLPI pg 893, the code below duplicates the parent process file descriptors, then closes the unused write end of the child process 
 				if(dup2(pfd_2[1], STDOUT_FILENO) == -1) //this makes STDOUT_FILENO (new file descriptor) be the copy of pfd[1] (old file descriptor) which is the write end of the pipe
 				{
 					printf("Error making old file descriptor new file descriptor");
 				}
 				if(close(pfd_2[1]) == -1) //close unused write end of child process  
 				{
 					printf("Close 6: Error closing write end of child process for pipe 2");
 				}
 			}


			execlp("sort", "sort", (char *) NULL); //writes to pipe 
 			
 		default: //default is parents case, parent falls through to create next child  
 			break; //put break for good measure according to lecture 
 			
 	}

 //Fork 3
 	switch (fork()) //after fork the parent and child process file descripts both read and write in pipe 
 	//we have to close unused file descriptors to create the flow we want like pg 893
 	{
 		case -1: //when fork returns -1 it was not successful 
 			printf("Error forking child process for uniq");

 		case 0: 
 			if(close(pfd_1[0] == -1)) //close pipe 1 file descriptor read end
 			{
 				printf("Close 7: Error closing read end of parent process of pipe 1"); //this was already closed but we re-close it to make sure 
 			}
 			if(close(pfd_1[1]) == -1) //write end is unused so we close it 
 			{
 				printf("Close 8: Error closing write end of child process of pipe 1"); //this was already closed too but we re-closed it to make sure 
 			}
 			if(close(pfd_2[1]) == -1) //close pipe 2 file descripts write end 
 			{
 				printf("Close 9: Error closing write end of parent process of pipe 2");
 			}
 			if(close(pfd_3[0]) == -1) //close pipe 3 file descripts read end 
 			{
 				printf("Close 10: Error closing read end of parent process of pipe 3");
 			}

 			//duplicate stdout on write end of pipe, close duplicated descriptor 
 			if(pfd_2[0] != STDIN_FILENO) 
 			{
 				//like the image shows in TLPI pg 893, the code below duplicates the parent process file descriptors, then closes the unused write end of the child process 
 				if(dup2(pfd_2[0], STDIN_FILENO) == -1) //this makes STDOUT_FILENO (new file descriptor) be the copy of pfd[1] (old file descriptor) which is the write end of the pipe
 				{
 					printf("Error making old file descriptor new file descriptor");
 				}
 				if(close(pfd_2[0]) == -1) //close unused write end of child process  
 				{
 					printf("Close 11: Error closing read end of child process of pipe 2");
 				}
 			}

 			if(pfd_3[1] != STDOUT_FILENO) //this is the defensive check in the case that standard input and output had both beed closed 
 			{
 				//like the image shows in TLPI pg 893, the code below duplicates the parent process file descriptors, then closes the unused write end of the child process 
 				if(dup2(pfd_3[1], STDOUT_FILENO) == -1) //this makes STDOUT_FILENO (new file descriptor) be the copy of pfd[1] (old file descriptor) which is the write end of the pipe
 				{
 					printf("Error making old file descriptor new file descriptor");
 				}
 				if(close(pfd_3[1]) == -1) //close unused write end of child process  
 				{
 					printf("Close 12: Error closing write end of child process of pipe 3");
 				}
 			}

			execlp("uniq", "uniq", "-c", (char *) NULL); //writes to pipe 
 			
 		default: //default is parents case, parent falls through to create next child  
 			break; //put break for good measure according to lecture 
 			
 	}


//Fork 4
	switch(fork())//after fork the parent and child process file descripts both read and write in pipe 
	{
		case -1:
			printf("Error forking child process for tee"); //when fork returns -1 it was not successful

		case 0:
			if(close(pfd_1[1]) == -1) //close pipe 1 file descriptor write end
			{
				printf("Close 13: Error closing read end of parent process of pipe 1");//this was already closed too but we re-closed it to make sure
			}
			if(close(pfd_1[0]) == -1)//close pipe 1 file descripts read end 
			{
				printf("Close 14: Error closing write end of child process of pipe 1");
			}

			if(close(pfd_2[1]) == -1)//close pipe 2 file descriptor write end
			{
				printf("Close 15: Error closing write end parent process of pipe 2");//this was already closed too but we re-closed it to make sure 
			}

			if(close(pfd_2[0]) == -1)//close pipe 2 file descripts read end 
			{
				printf("Close 16: Error closing read end child process of pipe 2");//this was already closed too but we re-closed it to make sure 
			}
			
			if(close(pfd_3[1]) == -1)//close pipe 3 file descriptor write end
			{
				printf("Close 17: Error closing write end parent process of pipe 3");
			}
			if(close(pfd_4[0]) == -1)//close pipe 4 file descriptor read end
			{
				printf("Close 18: Error closing read end of parent process of pipe 4");
			}


			if(pfd_3[0] != STDIN_FILENO)
			{
				if(dup2(pfd_3[0], STDIN_FILENO) == -1)//this makes STDIN_FILENO (new file descriptor) be the copy of pfd_3[0] (old file descriptor) which is the write end of the pipe
				{
					printf("Error making old file descriptor new file descriptor");
				}

				if(close(pfd_3[0]) == -1)
				{
					printf("Close 19: Error closing read end of child process of pipe 3");
				}
			}


	if(pfd_4[1] != STDOUT_FILENO)
	{
		if(dup2(pfd_4[1], STDOUT_FILENO) == -1)//this makes STDOUT_FILENO (new file descriptor) be the copy of pfd[1] (old file descriptor) which is the write end of the pipe
		{
			printf("Error making old file descriptor new file descriptor");
		}

		if(close(pfd_4[1]) == -1)
		{
			printf("Close 20: Error closing write end of parent process of pipe 4");
		}
	}	


	execlp("tee", "tee", outfile2, (char *) NULL);


	default:
		break;
}


 //Fork 5
 	switch(fork())
 	{
 		case -1: 
 			printf("Error making fork for wc");

 		case 0: 
 			if(close(pfd_1[0] == -1)) 
 			{
 				printf("Close 21: Error closing read end of child process of pipe 1");
 			}
 			if(close(pfd_1[1]) == -1) 
 			{
 				printf("Close 22: Error closing write end of parent process of pipe 1");
 			}
 			if(close(pfd_2[0] == -1)) 
 			{
 				printf("Close 23: Error closing read end of child process of pipe 2");
 			}
 			if(close(pfd_2[1]) == -1) 
 			{
 				printf("Close 24: Error closing write end of parent process of  pipe 2");
 			}
 			if(close(pfd_3[0] == -1)) 
 			{
 				printf("Close 25: Error closing read end of child process of pipe 3");
 			}
 			if(close(pfd_3[1]) == -1) 
 			{
 				printf("Close 26: Error closing write end of parent process of pipe 3");
 			}
 			if(close(pfd_4[1]) == -1) 
 			{
 				printf("Close 27: Error closing write end of parent process of pipe 4");
 			}

 			if(pfd_4[0] != STDIN_FILENO) //this is the defensive check in the case that standard input and output had both beed closed 
 			{
 				//like the image shows in TLPI pg 893, the code below duplicates the parent process file descriptors, then closes the unused write end of the child process 
 				if(dup2(pfd_4[0], STDIN_FILENO) == -1) //this makes STDIN_FILENO (new file descriptor) be the copy of pfd_3[0] (old file descriptor) which is the write end of the pipe
 				{
 					printf("Error making old file descriptor new file descriptor");
 				}
 				if(close(pfd_4[0]) == -1) //close unused read end of child process  
 				{
 					printf("Error closing old file descriptor");
 				}
 			}

 			execlp("wc", "wc", (char *) NULL);

 		default: //default is parents case, parent falls through to create next child  
 			break; //put break for good measure according to lecture 

 	}



 	//parent closes unused file descriptors for pipe, and waits for children 
 	if(close(pfd_1[0]) == -1) //close pipe 1
 	{
 		printf("Error closing unused file descriptor for pipe 1");
 	}
 	if(close(pfd_1[1]) == -1)
 	{
 		printf("Error closing unused file descriptor for pipe 1");
 	}

 	if(close(pfd_2[0]) == -1) //close pipe 2
 	{
 		printf("Error closing unused file descriptor for pipe 2");
 	}

 	if(close(pfd_2[1]) == -1)
 	{
 		printf("Error closing unused file descriptor for pipe 2");
 	}

 	if(close(pfd_3[0]) == -1) //close pipe 3
 	{
 		printf("Error closing unused file descriptor for pipe 3");
 	}

 	if(close(pfd_3[1]) == -1)
 	{
 		printf("Error closing unused file descriptor for pipe 3");
 	}

 	if(close(pfd_4[0]) == -1) //close pipe 4
 	{
 		printf("Error closing unused file descriptor for pipe 4");
 	}
 	if(close(pfd_4[1]) == -1)
 	{
 		printf("Error closing unused file descriptor for pipe 4");
 	}

 	if(wait(NULL) == -1)
 	{
 		printf("wait 1");
 	}
 	if(wait(NULL) == -1)
 	{
 		printf("wait 2");
 	}
 	if(wait(NULL) == -1)
 	{
 		printf("wait 3");
 	}
 	if(wait(NULL) == -1)
 	{
 		printf("wait 4");
 	}
 	
 }

 return 0; 

}

