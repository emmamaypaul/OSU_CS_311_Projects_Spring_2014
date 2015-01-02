/*
1) Emma Paul 
2) paule@onid.oregonstate.edu
3) CS311-400
4) Homework # 1

  $RCSfile$
  $Revision$
  $Author$
  $Date$
  $Log$

  from:
  http://www.programmingsimplified.com/c-program-read-file

 */

#include "read_file.h"

int main()
{
   char ch, file_name[25];
  // FILE *fp;
   int fd;
   int num_read;

   printf("Enter the name of file you wish to see\n");
   gets(file_name);

   fd = open(file_name, O_RDONLY);
   if (fd < 0)
     {
       perror("Error while opening the file.\n");
       exit(EXIT_FAILURE);
     }
   printf("The contents of %s file are :\n", file_name);

   for (;;)
     {

//http://bash.cyberciti.biz/guide/Reads_from_the_file_descriptor_%28fd%29
//http://linuxreviews.org/man/stdin/index.html.en
//have to use fd rather than STDIN, from the websites above it seems as though 
//STDIN and STDOUT are to be used with fopen, so when you omit those and call read 
//you have to use fd (file descriptor) to read the file line by line 
       num_read = read(fd, &ch, sizeof(ch));
       if (num_read < 0)
	 {
	   perror("Error while reading the file.\n");
	   exit(EXIT_FAILURE);
	 }
       if (num_read == 0)
	 {
	   break;
	 }
//reference:http://www.unix.com/programming/7093-read-write-stdout_fileno.html
//about write: http://linux.about.com/od/commands/l/blcmdl1_write.htm
       write(1, &ch, sizeof(ch));//1 is standout number 
     }
   return ch;
}
