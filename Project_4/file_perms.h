/*1)Emma Paul 
 *2)paule@onid.oregonstate.edu
 *3)CS311-400
 *4)Homework #4
 *Note: I copied this from the TLPI book pg 296 as suggested in the lecture 
 */

 #ifndef FILE_PERMS_H
 #define FILE_PERMS_H

 #include <sys/types.h>

 #define FP_SPEACIAl 1 //Include set-user-ID, set-group-ID, and stick bit information in retured string 
//FP_SPECIAL flag is set in the filePermStr() flags argument, then the returned string includes the settings of the set-user-ID, set-group-ID, and sticky bits, like the style of ls(1)

 char *filePermStr(mode_t perm, int flags);

 #endif 