#1)Emma Paul 
#2)paule@onid.oregonstate.edu
#3)CS311-400
#4)Homework #1
#Note: I discussed ideas and sources of reference regarding error handling with Ian Paul 
#reference from reading for prime alrogithm: https://docs.python.org/release/2.6.7/tutorial/controlflow.html#for-statements
#adding to a set: http://stackoverflow.com/questions/9299919/add-number-to-set
#command line reference: http://stackoverflow.com/questions/16179875/command-line-input-in-python
#cast command line as int reference: http://stackoverflow.com/questions/10751948/command-line-arguments-in-python
#length reference: http://www.tutorialspoint.com/python/list_len.htm
#function reference: http://www.tutorialspoint.com/python/python_functions.htm
#reference for if statement syntax: http://stackoverflow.com/questions/7141208/python-simple-if-or-logic-statement
#error handling: https://docs.python.org/2/tutorial/errors.html
#to run will be: python primeNumbers.py n (where n is command line argument and the number for the range)

import sys

while True:
    try:
        arg1 = int(sys.argv[1])
        break
    except (RuntimeError, TypeError, ValueError, NameError):
	exit()

def primeFnc(arg1):
    a = []

    for n in range(2, arg1): #outside for loop will go from 2...arg1, next loop from 3...arg1, ect 
        for x in range(2, n): #x is from 2 to n
            if n % x == 0: #n is divided by x for every value of x to n within inner for loop. If n is 5, will be divided by 2, 3, 4, and 5  
	        break
        else: #if number is not divisible by any number leading up to it then it is prime and is appeneded to the list a 
            a.append(n)

    print a
    print "The number of prime numbers found is: ", len(a);
    return 

primeFnc(arg1) 
