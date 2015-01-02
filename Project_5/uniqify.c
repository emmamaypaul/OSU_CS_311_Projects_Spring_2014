/*1)Emma Paul 
 *2)paule@onid.oregonstate.edu
 *3)CS311-400
 *4)Homework #5
 *Note: I worked with Ian Paul on this assignment by sharing helpful reference websites and using a tutor
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
#include <sys/msg.h>
#include <sys/ipc.h>
#include <ctype.h> //C book pg 249 to to 
#include "tlpi_hdr.h"
#include <search.h>
#include <math.h>
#include <assert.h>
#include <signal.h> //for signal functions
#include <string.h>


#define MAX_WORD 50 //no word will be more than 50 letters 
#define ARGS "n" 
#define QUEUE_PERMS 0666

static void *root = NULL;

typedef struct msg //TLPI pg 940: 
{
	long mtype; //message type 
	char mtext[MAX_WORD]; //message body 
}mess_t;//that makes it so you don't have to write struct 



struct DynArr //note: I copied my 261 dynamicArray.c functions 
{
	char **data; //an array of pointers, each pointer points to a different string (ie word)
	int size; /* Number of elements in the array */
	int capacity; /* capacity ofthe array */
};

static int kill_signal = 0;
 static void sigHandler_SIGUSR1(int sig) //hanlder function for SIGUSR1
 {
 	kill_signal = 1;
 	printf("Caught kill signal");
 }


int message_num;//the number id the message queue is 

int delimiterFnc(char *c); //down casts letters, then for any letter returns 1 on success or -1 for anything else (ie any delimeter)
//void send_to_queue(char *word, int message_q);//takes parsed word to sort it to binary seach tree 
void send_to_queue(char *word, int message_q, mess_t msg_send, int mtype);
void receive_to_sort(int num_procs, int message_q, int *rcv_queue_array);

//void print_sorted();//prints the sorted list 
//static void action(const void *nodep, const VISIT which, const int depth);
//static int compare(const void *pa, const void *pb);

//Dynamic array functions to hold the the parsed words so that I can use qsort on the dynamic array to sort the words 
void parseFnc(int message_q, mess_t msg_send, int num_procs);
void initDynArr(struct DynArr *v, int capacity);
struct DynArr* createDynArr(int cap);
void freeDynArr(struct DynArr *v);
void deleteDynArr(struct DynArr *v);
void _dynArrSetCapacity(struct DynArr *v, int newCap);
int sizeDynArr(struct DynArr *v);
void addDynArr(struct DynArr *v, char *val);
//struct getDynArr(struct DynArr *v, int pos);

 static int compare(const void *pa, const void *pb)
  {
     return strcmp( * (char * const *)pa, * (char * const *) pb );
  }

 
int main(int argc, char *argv[]) //arg c is num of arguments, argv is pointer array to each command line  argument
{
	char c;//variable for command line option (characters)  
	int nflag = 0;
	int num_procs = 0;
	char buf[1];//buffer to read one byte (ie one character) at at time

	int **sorter_in; //fd array to hold parse's output for sorter's input, reference: https://github.com/balajiathreya/CS-311/blob/master/HW2/uniqify.c
	int **sorter_out; //fd array to hold sorter's output and surpressor's input 
	int sorter_in_status; //variable for status when pipes are created 
	int sorter_out_status; //variable to hold the status when pipes are created  

	mess_t msg_send;
	mess_t msg_rcv; 
	 
	signal(SIGUSR1, sigHandler_SIGUSR1);//set up signal 

//create key:
	key_t key_name = ftok(__FILE__, 's'); //__FILE__ is a handy macro that makes file expand filename of it's process at the time 
//create message queue for each process :
	int message_q = msgget(key_name, QUEUE_PERMS | IPC_CREAT);//mssget returns message queue identifier on success, -1 on error 
//connect to an existing message queue 
	if((message_q = msgget(key_name, QUEUE_PERMS)) == -1)
	{
		printf("Error calling message.\n");
	}

	while((c = getopt(argc, argv, ARGS))!= -1) //while there are command line options , argument expected after all but v (indicated by :)
	{	
		switch(c)
		{ 
			case 'n':
				nflag = 1;
				if(argc == 2)//only 2 args provided, uniqify and -n, no number included 
				{
					num_procs = 1; 
				}
				else 
				{
					num_procs = atoi(argv[2]); 
					if(num_procs > 50)
					{
						printf("Please enter a number less than or equal to 50. Exiting program\n");
						exit(-1);
					}
				}
				break;	
			default: 
				printf("Error reading command line. Exiting program.\n");
				exit(-1);	
		}
	}

if (nflag == 1)
{

	//dynamic array declaration 
		struct DynArr *word_dArray; 
		void *val;

//create unique message queues for each sub sort process 
	int rcv_queue_array[num_procs];	
	int i = 0;
	for (i ; i < num_procs; i++)
	{
	//create message queue for each process :
		rcv_queue_array[i] = msgget((key_name + i + 1), QUEUE_PERMS | IPC_CREAT);//mssget returns message queue identifier on success, -1 on error 
	//connect to an existing message queue 
		if((rcv_queue_array[i] = msgget(key_name + i + 1, QUEUE_PERMS)) == -1)
		{
			printf("Error calling message.\n");
		}
	}


	//technicall at this point all the parsed words are in the message queue 
	receive_to_sort(num_procs, message_q, rcv_queue_array);

	
	//printf("Done creating child processes\n");


	parseFnc(message_q, msg_send, num_procs); //call the sort function that parses the string 

	msg_send.mtext[0] = '\0';
	for(i=0;i< num_procs; i++ )
	{
		msg_send.mtype = i+1;
		msgsnd(message_q, &msg_send, 1, 0);
	}

//	printf("Done receiving parsed words\n");


	

	int d = 0;
	for (d; d < num_procs; d++)
	{
		wait(NULL);
	}

/*
	
sleep(4);





//Message queue to recieve sub sorted processed words
	mess_t rcv_queues;

	mess_t end;

	end.mtext[0] = '\0';
	for(i=0;i< num_procs; i++ )
	{
		end.mtype = 1;
		msgsnd(rcv_queue_array[i], &end, 1, 0);
	}


	int c = 0;
		for (c ; c < num_procs; c++)
		{
				
			while (msgrcv(rcv_queue_array[c], &rcv_queues, sizeof(rcv_queues.mtext), 0, MSG_NOERROR) != -1) //reads until the message queue is empty 
			{
				if( strlen(rcv_queues.mtext) == 0 ) 
				{
					msgrcv(rcv_queue_array[c], &rcv_queues, sizeof(rcv_queues.mtext), 0, MSG_NOERROR);
					char * temp = malloc(strlen(rcv_queues.mtext + 1));
					strcpy(temp, rcv_queues.mtext);
					//printf("%s \n", temp);
					//addDynArr(compare_words_array, temp);
				printf("Message queue id: %d for sort subprocess: %d, Word: %s \n", rcv_queue_array[c], c + 1, temp);
				}

			}			

				
		}
	

//Note: below was my attempt to print out the correct input. It will print the word count in sorted order on smaller files, but on words.txt it didn't work because the recieve queue stopped around the letter e when it was too full
//however everything I tried could not fix it and I ran out of time 
//At one point I could recieve all of the words from each of the sub sort process message queues and print them out, however that copy that I saved to my computer somehow did not get transfered and was lost or overwritten somehow so I had to abandon that as well 



	//at this point technically all the sort sub processes have sent their sorted words to their own message queue, so we need to combine and print 
 //   struct DynArr *compare_words_array; 
	//compare_words_array = createDynArr(1);

 //	char * temp_word[50];

 	
		

	

		int y = 0;
				for (y; y < compare_words_array->size ; y++)
				{
					printf("%s \n", compare_words_array->data[y]);
				}

		

			//Note: though the instructions say not to resort, I spent two days on the combiner stage trying to pull the top message off each queue 
			///put them into an array that I sort, but I could not figure out how to take finally take the smallest word and then replace it's position in the pointer array with the next word from the queue
			//I gave up and resorted to resorting again in order to produce the 
			//qsort(compare_words_array->data, compare_words_array->size, sizeof(char *), compare);
			qsort(compare_words_array->data, compare_words_array->size, sizeof(char *), compare ); //sort the words 


		
		for (y = 0; y < compare_words_array->size ; y++)
		{
			printf("%s ", compare_words_array->data[y]);
		}
		printf("\n");

		int count = 1;

		for (i = 0; i < compare_words_array->size; i++)
		{

			for (i = 0; i < compare_words_array->size -1; i++) //-1 for when you get to the last iteration 
			{	
				if((strcmp(compare_words_array->data[i], compare_words_array->data[i+1])) == 0)
				{
					count++;
				}
				else 
				{


					printf("\t%d\t%s\n", count, compare_words_array->data[i]);
					count = 1;
					 
				}
			}
			printf("\t%d\t%s\n", count, compare_words_array->data[i]);
		}

*/
    
//delete queues, note: it only gets here if the file isn't too big 
	if (msgctl(message_q, IPC_RMID, NULL) == -1) 
	{
        printf("Error deleting parent message queue.\n");
        exit(-1);
    }


	for (i=0; i<num_procs; i++)
	{
		   if (msgctl(rcv_queue_array[i], IPC_RMID, NULL) == -1) 
		   {
		        printf("Error deleting sort sub processes message queue id: %d.\n", rcv_queue_array[i]);
		        exit(-1);
		   }
	}

}





	return 0; 

}


/* Below was my attempt to do the combiner stage. I tried for hours and finally had to abandon it
It correctly prints out the right input if the array was able to be filled but it doesn't work with big data 
char * tWord[50];

	tWord = 'a';
	int tempNumCount = 1;

		for(i; i< compare_words_array->size; i++)
		{
			//strcmp returns -1 if a < b 
			//returns 1 if b < a 
			//return 0 if a == b 

			if(strcmp(tWord,compare_words_array[i]) == 0)
			{
				tempNumCount++;

			}
			else if (strcmp(tWord,compare_words_array[i]) == 1)
			{
				tWord = compare_words_array[i];
				i = 0;
				
				tempNumCount = 1;
			}	
			else if (strcmp(tWord,compare_words_array[i]) == -1)
			{
				printf("\t%d\t%s\n", tempNumCount, tWord);
			}

			/*
			if((strcmp(compare_words_array->data[i], compare_words_array->data[i+1] )) == 0)
			{
				tempNumCount++;
			}
			else if ((strcmp(compare_words_array->data[i], compare_words_array->data[i+1] )) == -1)
			{
				printf("\t%d\t%s\n", tempNumCount, compare_words_array->data[i]);
				tempNumCount = 1;
			}
			else
			{
				printf("\t%d\t%s\n", tempNumCount, compare_words_array->data[i+1]);
				tempNumCount = 1;

			}
			


		}
*/




void parseFnc(int message_q, mess_t msg_send, int num_procs)
{
	char word_array[MAX_WORD]; //array to put in the words 
	char word_copy[MAX_WORD]; //array to put in the words 
	void *val;
	char buf[1];//buffer to read one byte (ie one character) at at time

	int i = 0;
	int num_read;
	int count = 0;

//for the parsing section belowe Ian Paul and I both got help from tutor Alex Eustir (from wyzant.com a tutor finder website)
		do //read one character at a time, http://stackoverflow.com/questions/15883568/reading-from-stdin
		{
			num_read = read(STDIN_FILENO, buf, sizeof(buf));
			if (num_read == 0 || delimiterFnc(&buf[0]) == -1)//if we are at the end of the file or found a delimeter 
			{
				if(i > 0) //make sure the array isn't empty 
				{
					
					//printf("%s\n", word_array);
					memcpy(word_copy, word_array, sizeof(word_array));
					//printf("Word copy: %s\n", word_copy);

					++count;
					send_to_queue(word_copy, message_q, msg_send, 1+ (count % num_procs) );
						
					i=0; //reset i so that the word array position goes back to 0
					int k=0;
					for (k; k <sizeof(word_array); k++) //frees up the array 
					{
						word_array[k] = 0;
					}
						k=0;		
				}
			}
			else
			{
				int j = 0;
				for (j = 0; j < 1; j++)
				{
					word_array[i] = buf[0];//fill array with character
					word_array[i+1] = '\0'; //put in the null terminator for the next character so that when delimeter func returns -1 for a delimeter the word array has the null terminator already set 
					i++; //go to next position in array 	
				}
	 		}
	 	} while(num_read > 0);//had to put do while loop so that when it reaches the end of file it will still read the last letter if there is a letter 
		
}



void send_to_queue(char *word, int message_q, mess_t msg_send, int mtype)
{

	memcpy(msg_send.mtext, word, MAX_WORD); //copy word to message structure 
	msg_send.mtype = mtype;//941 TLPI we have to set mtype to be greater than 0 

	if(msgsnd(message_q, &msg_send, strlen(msg_send.mtext) + 1, 0) == -1)
	{
		printf("Error sending message\n");
	} 
	
}

void receive_to_sort(int num_procs, int message_q, int *rcv_queue_array)
{

	mess_t msg_receive;
    struct DynArr *word_dArray;


   
//fork message queue for sort stage
    int i=0;
    int process[num_procs]; //integer array to be used in for loop for the process that we are currently on 
    for(i = 0; i < num_procs; i++) //for loop to go through the given number of sub processes to perform
    {
        //this is where we should fork and make the child processes do the sorting function
        switch(process[i] = fork()) //do i-1 so that you get at process 0 (this is so that we can have the right process count but be in the right spot in the array)   
        {
            case -1: //when fork returns -1 it was not successful
                 printf("Error forking child process for rev");

             case 0: //when fork returns 0 it was successful.
                 sleep(1);

                 printf("I am sub sort process number: %d \n", i + 1);

                word_dArray = createDynArr(1);

                    //receive message m
                    while( msgrcv(message_q, &msg_receive, sizeof(msg_receive.mtext), i + 1, MSG_NOERROR) != -1 )//reads until the message queue is empty
                    {
                        if( strlen(msg_receive.mtext) == 0 )
                        {
                            //printf("End of words reached: process %d\n", i);

                            //at this point all parsed words have been sent and recieved by child process
                            qsort(word_dArray->data, word_dArray->size, sizeof(char *), compare ); //sort the words

                           // printf("Made it after qsort\n");

                             //printf("I am sub sort process number: %d, and these are my sorted words that I send to my own message queue\n", i + 1);
                           
                            //printf("Length of sort sub process array: %d, process number: \n", sizeDynArr(word_dArray), i+1);
                                int z = 0;
                                for(z; z < word_dArray->size; z++)
                                {
                                   printf(" %s \n", word_dArray->data[z]);

                                    strcpy(msg_receive.mtext, word_dArray->data[z]); //copy word to message structure
                                    msg_receive.mtype = 1;//941 TLPI we have to set mtype to be greater than 0

                                    if(msgsnd(rcv_queue_array[i], &msg_receive, strlen(msg_receive.mtext) + 1, 0) == -1)
                                    {
                                        printf("Error sending message\n");
                                    }

                                }
                                    _exit(EXIT_SUCCESS);
                                   // printf("ALL IS LOST\n");
                        }
                        char * temp = malloc(strlen(msg_receive.mtext + 1));
                   
                        strcpy(temp, msg_receive.mtext);
                        addDynArr(word_dArray, temp);
                       // printf("Message received: %s\n", temp);
                    }
                   
                   

                _exit(EXIT_SUCCESS); //child processes should close using _exit
           
             default: //default is parents case, parent falls through to create next child 
                // printf( "Made it to default\n");
                 break; //put break for good measure according to lecture
         }           
     }
   //  printf("end of receive_to_sort\n");




	/*mess_t msg_receive;
	struct DynArr *word_dArray;
	//char buf[1];
	//buf[0] = '\0';

	
//fork message queue for sort stage 
	int i=0;
	int process[num_procs]; //integer array to be used in for loop for the process that we are currently on  
	for(i = 0; i < num_procs; i++) //for loop to go through the given number of sub processes to perform 
	{
		//this is where we should fork and make the child processes do the sorting function 
		switch(process[i] = fork()) //do i-1 so that you get at process 0 (this is so that we can have the right process count but be in the right spot in the array)	
		{
			case -1: //when fork returns -1 it was not successful 
	 			printf("Error forking child process for rev");

	 		case 0: //when fork returns 0 it was successful.
	 			sleep(1);

	 			printf("I am process number: %d\n", i);

				word_dArray = createDynArr(1);

					//receive message m

				wait(2);
					//while( msgrcv(message_q, &msg_receive, sizeof(msg_receive.mtext), i + 1,  MSG_NOERROR) != -1 )//reads until the message queue is empty 
					for(;;)

					{

						msgrcv(message_q, &msg_receive, sizeof(msg_receive.mtext), i + 1,  IPC_NOWAIT | MSG_NOERROR);//reads until the message queue is empty
						printf("Made it into msg recieve");
						if( strlen(msg_receive.mtext) == 0 ) 
						{
							
							printf("End of words reached: process %d\n", i);

							//at this point all parsed words have been sent and recieved by child process 
							qsort(word_dArray->data, word_dArray->size, sizeof(char *), compare ); //sort the words 

							//addDynArr(word_dArray, buf);

							printf("Made it after qsort\n");
							
							printf("Length of word_dArray: %d\n", sizeDynArr(word_dArray));
								int z = 0;
								for(z; z < word_dArray->size; z++)
								{
									printf("word: %s \n", word_dArray->data[z]);

									strcpy(msg_receive.mtext, word_dArray->data[z]); //copy word to message structure 
									msg_receive.mtype = 1;//941 TLPI we have to set mtype to be greater than 0 

									if(msgsnd(rcv_queue_array[i], &msg_receive, strlen(msg_receive.mtext) + 1, 0) == -1)
									{
										printf("Error sending message\n");
									} 
								}

/*
								printf("Made it after sending all words, not adding delimeter to each queue\n");

									msg_receive.mtext[0] = '\0';
									for(i=0;i< num_procs; i++ )
									{
										msg_receive.mtype = i+1;
										msgsnd(rcv_queue_array[i], &msg_receive, 1, 0);
									} 
																
									_exit(EXIT_SUCCESS);
									
						}
						char * temp = malloc(strlen(msg_receive.mtext + 1));
					
						strcpy(temp, msg_receive.mtext);
						addDynArr(word_dArray, temp);
						printf("Message received: %s\n", temp);
					}
					
					

				_exit(EXIT_SUCCESS); //child processes should close using _exit 
			
	 		default: //default is parents case, parent falls through to create next child  
	 			printf( "Made it to default\n");
	 			break; //put break for good measure according to lecture 
	 	}			
 	}
 	printf("end of receive_to_sort\n");
 	int d = 0;
 	for (d = 0; d < num_procs; d++)
	{
		wait(NULL);
	}*/
 }

 
// Initialize (including allocation of data array) dynamic array.
void initDynArr(struct DynArr *v, int capacity)
{
	assert(capacity > 0);
	assert(v!= 0);
	v->data = malloc(sizeof(char) * capacity * MAX_WORD); //allocate enough room for the size of each word 
	assert(v->data != 0);
	v->size = 0;
	v->capacity = capacity;
}

// Allocate and initialize dynamic array.
struct DynArr* createDynArr(int cap)
{
	assert(cap > 0);
	struct DynArr *r = (struct DynArr *)malloc(sizeof(struct DynArr));
	assert(r != 0);
	initDynArr(r,cap);
	return r;
}

// Deallocate data array in dynamic array.
void freeDynArr(struct DynArr *v)
{
	if(v->data != 0)
	{
	free(v->data); /* free the space on the heap */
	v->data = 0;   /* make it point to null */
	}
	v->size = 0;
	v->capacity = 0;
}

// Deallocate data array and the dynamic array ure.
void deleteDynArr(struct DynArr *v)
{
	freeDynArr(v);
	free(v);
}

void _dynArrSetCapacity(struct DynArr *v, int newCap)
{
	char **newData = malloc(newCap *sizeof (struct DynArr));

	int i;
	for (i=0; i< v->size; i++)
	{
		newData[i] = v->data[i];
	}
	free(v->data);
	v->data=newData;
	v->capacity=newCap;



}

// Get the size of the dynamic array
int sizeDynArr(struct DynArr *v)
{
	return v->size;
}

// Adds an element to the end of the dynamic array
void addDynArr(struct DynArr *v, char *val)
{
	
	assert(v !=0);//testing the precondition "the dynArray is not null"

	if(v->size>=v->capacity)
	_dynArrSetCapacity(v, 2 *v->capacity);

	v->data[v->size]=val;
	v->size++;

}
int delimiterFnc(char *c)//each character of Standard Input will go through this check 
{
	if((*c >= 'a') && (*c <='z')) //http://www.asciitable.com/, ASCII value for a = 97, and z = 122. Since ASCII characters have their corresponding decimal number, we can see if it's in the alphabet by check if the char's decimal value is between a ( ie 97) or z (ie 122)
	{
		return 1; 
	}
	else if((*c >= 'A') && (*c <= 'Z')) //A is 65, Z is 90, so if it's between those it is a capital letter 
	{
		*c += 32; //a is 97, A is 65 so A + 32 gives 97 (a) 
		return 1;
	}
	else //else it is a delimeter 
	{
		return -1;	
	}
}


/*

void delete_queues(void)
{
		//use this to delete message keys TLPI pg 931
	if(msgctl(message_q, IPC_RMID, NULL) == -1) 
	{
		printf("Error deleting message key");
	}	

	//for loop for receive queue array to delete them 
}

  static void sigHandler_SIGINT(int sig) //handler function for SIGINT (passing in signal, TLPI pg 401)
 {
 	printf("SIGINT has been caught, terminating the program\n");
 	exit(0); //exit on success
 }



// Get an element from the dynamic array from a specified position
void getDynArr(struct DynArr *v, int pos)
{
	assert(v !=0);
	assert(v->size>0);
	assert(pos<v->size && pos>=0);

	if(pos<=v->size)
	return v->data[pos];
}


void print_sorted()
{
	twalk(root, action);
	tdestroy(root, free);
}

 

 static void action(const void *nodep, const VISIT which, const int depth)
 {
     char *datap;

     switch (which) 
     {
           case preorder:
               break;
           case postorder:
               datap = *(char **) nodep;
               printf("%s\n", datap);
               break;
           case endorder:
               break;
           case leaf:
               datap = *(char **) nodep;
               printf("%s\n", datap);
               break;
      }
}

*/


