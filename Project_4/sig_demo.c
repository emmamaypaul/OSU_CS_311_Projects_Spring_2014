/*1)Emma Paul 
 *2)paule@onid.oregonstate.edu
 *3)CS311-400
 *4)Homework #4
 * Note: I discussed what was supposed to be happening in this hw assignment with Ian Paul to further our understanding of the functions 
 */


#include <signal.h> //for signal functions
#include <stdio.h> //for printf
#include <stdlib.h> //for exiting program function


 static void sigHandler_SIGUSR1(int sig) //hanlder function for SIGUSR1
 {
 	printf("SIGUSR1 has been caught\n");
 }
 static void sigHandler_SIGUSR2(int sig) //handler function for SIGUSR2
 {
 	printf("SIGUSR2 has been caught\n");
 }
  static void sigHandler_SIGINT(int sig) //handler function for SIGINT (passing in signal, TLPI pg 401)
 {
 	printf("SIGINT has been caught, terminating the program\n");
 	exit(0); //exit on success
 }

 int main(int argc, char *argv[])
 {

 	//what is happening below is that we are setting up the sigHandler_[SIGUSR1 || SIGUSR2 || SIGINT] functions to be called when they are signaled, and the functions just notify that they were signaled 
 	//then we are using the kill system call to signal the various signals (based on their signal numbers), and since our functions are called and print the signals have been caught, we know it works! 

 	signal(SIGUSR1, sigHandler_SIGUSR1); //this is isnstalling the signal handler, ie when there is a SIGUSR1 (user defined signal) it will will call the sigHandler_SIGUSR! function by going to it's address 
 	if (kill(getpid(), 10) == -1)//kill sends the signal, getpid returns the process ID of the calling process, 10 for SIGUSR1
	{
		printf("SIGUSR1 has not been caught, error has occured.\n");
	}

 	signal(SIGUSR2, sigHandler_SIGUSR2);//SIGUSR2 is user defined signal 
 	if (kill(getpid(), 12) == -1) //kill system call can send a signal to another process 
 	{
 		printf("SIGUSR2 has not been caught, error has occured.\n");
 	}

 	//SIGINT is "terminal interrupt signal" --> http://lasr.cs.ucla.edu/vahab/resources/signals.html
	signal(SIGINT, sigHandler_SIGINT);//TLPI pg 397, signal takes sig (signal whose disposition with to change, and adress of handler function) that should be called when signal is delivered. 
 	if (kill(getpid(), 2) == -1) //TLPI pg 392 sigint signal number is 2, TLIP pg 402 kill returns 0 on success and -1 on error
 	{
 		printf("SIGINT has not been caught, error has occured.\n");
 	}

 	return 0;

 }