#1) Emma Paul 
#2) paule@onid.oregonstate.edu
#3) CS311-400 
#4) Homework #2
#Note: I worked with Ian Paul and Andrew Goetz on this assignment discussing ideas for code solutions, as well as shared references 

#https://docs.python.org/2/tutorial/inputoutput.html
fileToOpen = open('1000_Digit.txt') #open text and put data into a list 
numberList = fileToOpen.read() #read() puts each character into a string array  
maxNum = 0 #start max number to be 0, but will change 

 
for n in range(0, 996): #this does i++ for you, remember! Since it's 1000 numbers and doing product of first 5 in list (list[0:4]), range is 996 (1000-4)
	numProduct = (int(numberList[n]) * int(numberList[n+1]) * int(numberList[n+2]) * int(numberList[n+3]) * int(numberList[n+4]))
	if numProduct > maxNum:#when there is a product that is biggest
		maxNum = numProduct #that is the max 

print maxNum

#note: a preferred method over what I have is change the text document to be a module (not really sure how to make a parsed list from the number list, info here: https://docs.python.org/2/tutorial/modules.html

 
