/*
1)Emma Paul 
2)paule@onid.oregonstate.edu
3)CS311-400
4)Homework #6 
Note: I worked on this assignment with Ian Paul by sharing helpful resources and a tutor 
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
#include <sys/mman.h>
#include <semaphore.h>

#define ARGS "qm:c:"

//global variables 
unsigned int next_prime;
unsigned int divisor;
unsigned int *bitmap;
void *addr;
int shmem_fd;
unsigned int *value;
unsigned int max_prime_num;
unsigned int num_sub_processes;
sem_t *semaphore;

//declare functions 
void yes_prime(unsigned int A[ ], unsigned int k);
void no_prime(unsigned int A[ ], unsigned int k);
int prime_check(unsigned int A[ ], unsigned int k);
void *mark_not_prime(unsigned int *bitmap);
void *mount_shmem(char *path, int object_size);

struct data
{
	unsigned int current_prime;
};

int main(int argc, char *argv[]) //arg c is num of arguments, argv is pointer array to each command line  argument
{
	
	unsigned long i;
	unsigned int mult;
	int j; 
	
	unsigned long thread_pointer;//TLPI pg 625, pthread_t is actually a pointer that has been cast to unsigned long 
	unsigned int bitmap_size;
	pid_t pid; //process id 

	char c;//variable for command line option (characters) 
	optarg = NULL; //to get argument after command line char   
	num_sub_processes = 1; //if not specified should be 1 
	max_prime_num = UINT_MAX; //this is the max num, 4294967295, for the max twin prime number, NOT max number of twin primes 
	
	int qflag = 0;
	time_t start;
	time_t end; //time variables 
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
					printf("The max prime number cannot exceed: %u \n");
				}
				break;
			case 'c': //c is for the number of threads 
				num_sub_processes = atoi(optarg); //if not provided is 1
				if(num_sub_processes > 20)
				{
					printf("The number of sub processes cannot exceed 20, exiting program. \n");
					exit(-1);
				}
				break;
			default: 
				printf("Error reading command line argument. Exiting program. \n");
				exit(-1);

		}
	}

if(qflag == 1)
{
	start_time = time(&start);//TLPI pg 187, returns number of seconds since EPOCH
}
	
//allocate enough memory for bit array, have to dynamically allocate like this because size is so big cant do bit_array[size], reference for how to allocate enough memory and syntax: https://github.com/beckysag/prime-finder/blob/master/primePThread.c
	unsigned int size = ((max_prime_num/ 32)); //max number divided by 32 because each array index has decimal size of 32 --> http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html, (+1 for offset to find the max space needed for the number of primes up to the max  
	bitmap_size = size * sizeof(unsigned int);//enough space for the number of bytes in array to have size of unsigned int 
	bitmap_size = bitmap_size + sizeof(unsigned int); //add one more for cushion like we do with an array

//the address in shared memory with enough size for the bitmap, which I locate at the address on the second line    
	void *addr = mount_shmem("/primeMProc", bitmap_size);
	bitmap = (unsigned int*)addr;//bitmap located at the address of shared memory 

//this is used to hold the current variable state 
	void *addr2 = mount_shmem("/primeMProc2", sizeof(int));
	value = (int *)addr2;//attach value to the address in memory 

	semaphore = sem_open("/my_own_semaphore", O_CREAT, S_IRUSR | S_IWUSR, 1); //initialized to 1, created semaphore 
	if(!semaphore)
	{
		printf("Error creating and initializing semaphore, exiting program");
		exit(EXIT_FAILURE);
	}

	
//set all numbers to be 1, or yes, so that I can mark them off as I go  	
     for ( i = 0; i < max_prime_num; i++ )
     {
     	 yes_prime(bitmap, i); //first mark all as prime 
     }

//set 0 and 1 as not prime right off the bat 
    no_prime(bitmap, 0);//1 is not a prime
	no_prime(bitmap, 1);//1 is not a prime

//set multiples of 2 as non prime 
	mult = 2; 
	unsigned int divisor = max_prime_num;
	divisor = floor(max_prime_num / mult);//need to round down to find lowest common denomenatory 

	for(i = 1; i <= divisor; i++)
	{
		if((i*mult) != mult)//don't want to set multiplies as a prime
		{
			no_prime(bitmap, (i*mult));
		}	
	}
	
*value = 3; //set next prime value to 3 since that is what the next prime is after 2

//create processes 
	for(i = 0; i < num_sub_processes; i++)
	{
		switch(pid = fork())
		{
			case -1: 
				printf("Error creating child process, exiting program.\n");
				exit(-1);
				break;
			case 0: 
				
				mark_not_prime(bitmap);//child process calls mark not prime function 

				//before the parent process in the multi-process application exits (after waiting on its child processes of course), it should close and unmap the shared memory segment 
				j = munmap(addr, bitmap_size);//have to unmap shapred memory after we are done to free up the memory again  
				if (j != 0)
				{
					printf("Error unmapping child process, exiting program.\n");
					exit(-1);
				}
				j = munmap(addr2, sizeof(int));//have to unmap shapred memory after we are done to free up the memory again  
				if (j != 0)
				{
					printf("Error unmapping child process, exiting program.\n");
					exit(-1);
				}
				_Exit(1);//terminates child process --> http://stackoverflow.com/questions/2329640/how-to-exit-a-child-process-exit-vs-exit
				break;

			default:  
				break;
		}
	}

	if(close(shmem_fd == -1))//close file descriptors of the shared memory segments 
	{
		printf("Error closing file descriptor, exiting program.\n");
		exit(-1);
	}

//wait for all the child processes to die 
int genocide;
for(genocide = 0; genocide < num_sub_processes; genocide++)
{
	wait(NULL);
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
	for(i = 0; i < (max_prime_num -1); i++)
	{
		if (prime_check(bitmap, i) == 1) //if it's a prime 
		{
			if(prime_check(bitmap, (i+2)) == 1)
			{
				printf("%u %u\n", i, (i+2));
			}	
		}
	}	
}
	
	return 0;
}

//http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
void yes_prime(unsigned int A[ ], unsigned int k)
{
	unsigned int i = k/32; //k -1 to go to correct ith position  
	unsigned int pos = k%32;
	unsigned int flag = 1;
	flag = flag << pos;
	A[i] = A[i] | flag; 
}
void no_prime(unsigned int A[ ], unsigned int k)
{
     unsigned int i = k/32;
     unsigned int pos = k%32;
     unsigned int flag = 1;  
     flag = flag << pos;     
     flag = ~flag;           
     A[i] = A[i] & flag;     
}
int prime_check(unsigned int A[ ], unsigned int k)
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

void *mount_shmem(char *path, int object_size)//from lecture 
{
	shmem_fd = shm_open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); //from lecture, does read write permissions for users 
	if(shmem_fd == -1)
	{
		printf("Failed to open shared memory object, exiting program.\n");
		exit(-1);
	}

	if(ftruncate(shmem_fd, object_size) == -1) //resizes shared memory object 
	{
		printf("Failed to resize shared memory object, exiting program.\n");
		exit(-1);
	}

	addr = mmap(NULL, object_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmem_fd, 0);//map shared memory space to the address 

	if(addr == MAP_FAILED)
	{
		printf("Failed to map shared memory object, exiting program.\n");
		exit(-1);
	}

	return addr;//returns address after mapped shared memory space 
}

void *mark_not_prime(unsigned int *bitmap)
{
	unsigned int current;
	unsigned int max;

	divisor = sqrt(max_prime_num);
	while(*value <= divisor)
	{	
		sem_wait(semaphore); //wait for semaphore decrments the value, or blucs until the value becomes greater than 0 
		current = *value;
		do 
		{
			*value =  *value + 2;//increments by 2 until it finds a non prime then will to up through while loop again 
		} while( prime_check(bitmap, *value) != 1 );
		sem_post(semaphore);
			
		max = floor(max_prime_num / current);
		int z;
		for(z = 2; z <= max ; z++)
		{
			no_prime(bitmap, (z*current));	
		}
	}
}



