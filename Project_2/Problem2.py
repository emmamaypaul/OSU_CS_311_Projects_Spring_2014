#1)Emma Paul 
#2)paule@onid.oregonstate.edu
#3)CS 311-400
#4)Homework #2 
#Note: I worked with Ian Paul on this assignment discussing ideas for code solutions, as well as shared references 

import sys, os, getopt, optparse 

#https://docs.python.org/2/library/optparse.html
from optparse import OptionParser 
usage = "usage: %prog [options] arg"
parser = OptionParser(usage) 						 #if not specified, optparse assumes type string (but I put it in anyway) 
parser.add_option("-t", "--term", action="store", type="string", dest="term")#note: -x -f is equivalent to -xf
parser.add_option("-c", "--course", action="store", type="string", dest="course")
(options, args) = parser.parse_args()				        # instructs optparse to parse programs command line 

if len(sys.argv) != 5:
	parser.error("Error, expected argument syntax is: python fileName.py -t <term> -c <course>")
	sys.exit(1)						        #not sure if this is right, should exit running script 
else: 
	term = options.term 						#stored variables from command line arguments 
	course = options.course 
	pathName = ("%s/%s"%(term,course))		     		#create directories from command line arguments 

	
					 				#function to make check if directory exists, if not it creates it 
					 				#http://justgagan.wordpress.com/2010/09/22/python-create-path-or-directories-if-not-exist/
def assure_path_exists(path): 		 				#python functions take path argument of directory
	dir = os.path.dirname(path)	 				#dir is parent directory 
	if os.path.exists(dir):						#if term does exists, check if class exists 
		if not os.path.exists(path):				#if term/class doesn't exists
			os.mkdir(path) 
		else:
			print ("Directory %s already created"%(path)) 	#notify it exists			
			print ("Subdirectories already created within %s"%(path))
			sys.exit(1)	 				#exit ie don't make subdirectories 
	if not os.path.exists(dir):		 			#if directory doesn't exist 
			os.makedirs(path)	 			#it is created 



#call function to create term and class directory 
assure_path_exists(pathName)            		  
						        
#function to create subdirectories 
def make_sub_directories(path,name):
	dir = os.path.dirname(path)
	if os.path.exists(dir):
		os.mkdir('%s/%s'%(path,name))
	else:
		print ("Parent directory %s not created"%(path))		


	
#subdirectory variables 
sub1 = ("assignments")
sub2 = ("examples")							#https://docs.python.org/2/tutorial/introduction.html
sub3 = ("exams")
sub4 = ("lecture_notes")
sub5 = ("submissions")

#call function create subdirectories 
make_sub_directories(pathName, sub1)
make_sub_directories(pathName, sub2)
make_sub_directories(pathName, sub3)
make_sub_directories(pathName, sub4)
make_sub_directories(pathName, sub5)

print "Directory %s created, as well as subdirectories %s, %s, %s, %s, %s"%(pathName, sub1, sub2, sub3, sub4, sub5)

#create symbolic link 
src1 = '/usr/local/classes/eecs/%s/src/README'%(pathName)  #http://www.tutorialspoint.com/python/os_symlink.html
dst1 = 'README'
os.symlink(src1, dst1)
src2 = '/usr/local/classes/eecs/%s/src'%(pathName)
dst2 = 'src_class'
os.symlink(src2, dst2)

print "Symbolic links %s and %s created."%(dst1, dst2)
