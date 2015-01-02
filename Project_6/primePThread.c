/*
1)Emma Paul 
2)paule@onid.oregonstate.edu
3)CS311-400
4)Homework #6 
Note: I worked on this assignment with Ian Paul by sharing helpful resources, and we had a tutor Alex help us with the logic of our prime functions
*/


#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/utsname.h>
#include <ar.h>
#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <dirent.h>
#include <string.h>
#include <utime.h>
#include <sys/time.h>
#include <time.h>
#include <dirent.h>
#include <limits.h>//has UINT_MAX
#include <math.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h> 

#define ARGS "qm:c:"

//variables that need to be global so the calling function used by the thread can access them 
pthread_mutex_t mutex; 
unsigned int max_prime_num;
unsigned int *bit_array; //Bit Array notes: an array of bits A is an array where A[i] = 0 or 1 --> http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
unsigned int num_threads;
unsigned int next_prime;//global variable to pass between threads
unsigned int divisor;


//declare functions 
void yes_prime(unsigned int A[ ], unsigned int k);
void no_prime(unsigned int A[ ], unsigned int k);
int prime_check(unsigned int A[ ], unsigned int k);
void *mark_not_prime(void *thread_pointer);

int main(int argc, char *argv[]) //arg c is num of arguments, argv is pointer array to each command line  argument
{
	char c;//variable for command line option (characters) 
	optarg = NULL; //to get argument after command line char   
	num_threads = 1; //if not specified should be 1 
	max_prime_num = UINT_MAX; //this is the max num, 4294967295, for the max twin prime number, NOT max number of twin primes 
	int qflag = 0;
	unsigned long i;

//time variables 
	time_t start;
	time_t end; 
	double seconds = 0;
	double start_time;
	double end_time;


	//while loop to process command line arguments, http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
	while((c = getopt(argc, argv, ARGS))!= -1) //while there are command line options , argument expected after all but v (indicated by :)
	{	
		switch(c)
		{ 
			case 'q': //q option is used to collect timing values 
				qflag = 1;
				break;
			case 'm'://m is for the maximum size of the twin prime numbers to check 
				max_prime_num = atoi(optarg);
				if(max_prime_num > UINT_MAX)
				{
					printf("The max prime number cannot exceed: %u \n", UINT_MAX);
				}
				break;
			case 'c': //c is for the number of threads 
				num_threads = atoi(optarg); //if not provided is 1
				if(num_threads > 20)
				{
					printf("The number of threads cannot exceed 20, exiting program. \n");
					exit(-1);
				}
				break;
			default: 
				printf("Error reading command line argument. Exiting program. \n");
				exit(-1);

		}
	}
	

if(qflag == 1)//start time if -q command was specified 
{
	start_time = time(&start);//TLPI pg 187, returns number of seconds since EPOCH
}

//set the divisor to be the square root of the max prime num because the the least available number does not exceed the square root of N, because a composite number A = a*b less or equal to N (max prime num), the factors a and b can't both exceed square root of N (http://www.cut-the-knot.org/Curriculum/Arithmetic/Eratosthenes.shtml)
	divisor = sqrt(max_prime_num);
	
//allocate enough memory for bit array, have to dynamically allocate like this because size is so big cant do bit_array[size]
	unsigned int size = (max_prime_num/32); //max number divided by 32 because each array index has decimal size of 32 --> http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html, (+1 for offset to find the max space needed for the number of primes up to the max  
	bit_array = calloc(size,(sizeof(unsigned int))); 
   
//set all numbers to be 1, or yes prime, so that the threads mark them off and then in the end only the prime numbers will be marked 1 and we use that to print them 	
     for ( i = 0; i < max_prime_num; i++ )
     {
     	 yes_prime(bit_array, i); //first mark all as prime 
     }

//0 and 1 as not prime and is crossed out right away  
	no_prime(bit_array, 0);
	no_prime(bit_array, 1);//1 is not a prime

//set multiples of 2 up to max prime num as non prime since all even numbers are not prime  
	unsigned int mult = 2; 
	unsigned int divisor = max_prime_num;
	divisor = floor(max_prime_num / mult);//need to round down to find lowest common denomenator to go up to to multiply 2 by
	
	for(i = 1; i <= divisor; i++)
	{
		if((i*mult) != mult)//don't want to set multiplies as a prime
		{
			no_prime(bit_array, (i*mult));
		}	
	}

//declare and allocate memory for number of threads  
	pthread_t *thread = (pthread_t*) malloc(sizeof(pthread_t) * num_threads); //thread allocation resource: https://github.com/beckysag/prime-finder/blob/master/primePThread.c

//create threads with the detached attribute, TLPI pg 628 and from lecture 
	pthread_attr_t attr;

	if(pthread_attr_init(&attr) != 0)//returns 0 on success
	{
		printf("Error initializing pthread attribute, exiting program. \n");
		exit(-1);
	}
	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0)//returns 0 on success, through the default attribute is joinable, using it allows you to call pthread_join on the thread, also if you don't use PTHREAD_CREATE_DETACHED then you need to have join : http://stackoverflow.com/questions/11806793/what-is-the-usage-of-pthread-create-joinable-in-pthread
	{
		printf("Error setting detach state, exiting program. \n");
		exit(-1);
	}

//initialize mutex TLPI pg 642
	pthread_mutex_init(&mutex, NULL); //TLPI pg 639 if the second argument, attr, is NULL, then the mutex is assigned various default attributes  

next_prime = 3; //set the next prime to be 3 since that is what it is after 2 
divisor = sqrt(max_prime_num);

//create threads 
	for (i = 0; i < num_threads; i++)
	{
		//the first argument is the adress of the thread we are on 
		//the second is the attr, in this case joinable 
		//the third is a function to mark multiples not primes which has to take a void * 
		if (pthread_create(&thread[i], &attr, mark_not_prime, NULL) != 0)
		{
			printf("Error creating thread.\n");
		}	
	}

//join threads, this allows them to "speak" to eachother in a sense, letting the other threads know when one is not using/manipulating/updating the shared variable 
	for (i = 0; i < num_threads; i++) 
	{
		if (pthread_join(thread[i], NULL) != 0)
		{
			printf("Error joining thread.\n");
		}
	}

if(qflag == 1)//timing reference: https://github.com/beckysag/prime-finder/blob/master/primePThread.c
{
	end_time = time(&end);
	seconds = (end_time - start_time);
	printf("Time to complete: %.f seconds \n", seconds);
}

if(qflag == 0) //if q specified on commands line then we won't print 
{
//print twin primes 
	for(i = 0; i < max_prime_num; i++)
	{
		if (prime_check(bit_array, i) == 1) //if it's a prime 
		{
			if(prime_check(bit_array, (i+2)) == 1)
			{
				printf("%u %u\n", i, (i+2));
			}		
		}
	}	
}

//Destroy threads 
	if (pthread_attr_destroy(&attr) != 0)
	{
		printf("Error destroying pthread attribute.\n");
	}
	if (pthread_mutex_destroy(&mutex) != 0)
	{
		printf("Error destroying mutex.\n");
	}

//Free up memory 
	free(bit_array); //free the array 
	free(thread); //free the threads 
	pthread_exit(NULL);//exit and destroy the threads, though somewhat redunant becasue technically when the main returns all threads die 

	return 0;

}

//yes_prime, no_prime, and prime_check taken from: http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
void yes_prime(unsigned int A[ ], unsigned int k)//marks the (decial) position k in the array to be 1
{
	unsigned int i = k/32; //k -1 to go to correct ith position  
	unsigned int pos = k%32;
	unsigned int flag = 1;
	flag = flag << pos;
	A[i] = A[i] | flag; 
}
void no_prime(unsigned int A[ ], unsigned int k)//marks the position k in the array to be 0
{
     unsigned int i = k/32;
     unsigned int pos = k%32;
     unsigned int flag = 1;  
     flag = flag << pos;     
     flag = ~flag;           
     A[i] = A[i] & flag;     
}
int prime_check(unsigned int A[ ], unsigned int k)//checks to see if it is marked 1, it returns 1, if 0, retruns 0
{
     unsigned int i = k/32;
     unsigned  int pos = k%32;
     unsigned int flag = 1; // flag = 0000.....00001
     flag = flag << pos;    // flag = 0000...010...000   (shifted k positions)
     if ( A[i] & flag )      // Test the bit at the k-th position in A[i]
         return 1; //returns 1 if marked prime 
     else
         return 0;
}
void *mark_not_prime(void * dummy)//has to be void * for pthread create, thread pointer is updated every time this function is called so it's a new thread 
{
	unsigned int current = 3;//first prime after 2 is three 
	unsigned int max;
	
	while(next_prime <= divisor) 
	{
		pthread_mutex_lock(&mutex);//TLPI pg 646, lock shared variable to be in the desired state
		current = next_prime;//get global variable next prime to be current
		do //find next prime to update it before we unlock so that another thread can can get it and start doing work while this thread is marking off all the multiples of current 
		{
				next_prime =  next_prime + 2;//increments by 2 until it finds a non prime then will to up through while loop again 
		} while( prime_check(bit_array, next_prime) != 1 );	
		pthread_mutex_unlock(&mutex);//unlock after next prime has been updated 
			
		max = floor(max_prime_num / current);//go up to the next prime's highest multiple that is less than the number which is why I use floor to round down 
		unsigned int z;

		for(z = 2; z <= max ; z++)
		{
			no_prime(bit_array, (z*current));//mark all of it's multiples as prime 		
		}
	}
}




