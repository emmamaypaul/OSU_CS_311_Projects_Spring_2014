/*
 *
 1) Emma Paul 
 2) paule@onid.oregonstate.edu
 3) CS311-400
 4) Homework #1

  $RCSfile$
  $Revision$
  $Author$
  $Date$
  $Log$

  from:
  http://www.programmingsimplified.com/c/source-code/c-program-check-vowel

 */


#include <stdio.h>
 
int main()
{
  char ch;
 
  printf("Input a character\n");
  scanf("%c",  &ch);
 
  switch(ch)
  {
    case 'a':
    case 'A':
    case 'e':
    case 'E':
    case 'i':
    case 'I':
    case 'o':
    case 'O':
    case 'u':
    case 'U':
      printf("%c is a vowel.\n", ch);
      break;
    default:
      printf("%c is not a vowel.\n", ch);
  }              
 
  return ch;
}
