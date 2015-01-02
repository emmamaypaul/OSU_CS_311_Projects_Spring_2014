#1) Emma Paul 
#2) paule@onid.oregonstate.edu
#3) CS311-400 
#4) Homework #2
#Note: I worked with Ian Paul on this assignment discussing ideas for code solutions, as well as shared references 

 
# use a command line argument to determine which prime number to print so when running program can do "python Problem4.py 2300"

import sys 

while True: 
    try: 
        arg1 = int(sys.argv[1])
	break 
    except (RuntimeError, TypeError, ValueError, NameError): 
	print "An integer argument is expected"
	exit() 


def primeFnc(arg1): 
    primeList = [] 
    
    maxNum = 30000 # I  picked a number that's very large so numbers greater than the 2300th prime number (20357) can be tested 
    if arg1 > 30000: #30,000 is the max number that will be tested, so if argument it greater than that program should exit 
        exit() 
    
    for n in range(2, maxNum): 
        for x in range(2, n): 
	    if n % x == 0: 
		break 
	else: 
	    primeList.append(n) 
 
    result =  primeList[arg1-1] #should print the 2300th prime number if command line argument is 2300
    print '%d' %result # http://stackoverflow.com/questions/11488523/how-do-you-get-a-decimal-in-python
    return 

primeFnc(arg1) 
    

 
