/** Multiprocessing IPC example via pipes in C 
    To compile (Unix-like system): gcc -o pipe_ls_wc pipe_ls_wc.c
**/
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ReadEnd  0                                                             
#define WriteEnd 1 


/* Report error and exit program */
void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1);    /** failure **/
}


int main() {
  int pipeFDs[2];                                          /* Pipe file descriptors */         

  if (pipe(pipeFDs) < 0) report_and_exit("pipe()");        /* Create unnamed pipe */        

  pid_t pid = fork();                                      /* Create first child process */                     
  if (pid < 0) report_and_exit("fork()");                           

  /*** First child: executes ls to write to pipe ***/
  if (pid == 0) {                                                  
    dup2(pipeFDs[WriteEnd], STDOUT_FILENO);                /* Duplicate stdout on write end of pipe */
    close(pipeFDs[ReadEnd]); 
    execlp("ls", "ls", (char *) NULL);                     /* Writes to pipe  */                                  
    close(pipeFDs[WriteEnd]);                                    
    _exit(0);                                              /* Signal parent of immediate exit **/         
  }
  else {
    pid_t pid = fork();                                    /* Create second child process */

    /*** Second child: executes wc to read from pipe ***/
    if (pid == 0) {
      dup2(pipeFDs[ReadEnd], STDIN_FILENO);                /* Duplicate stdin on read end of pipe */
      close(pipeFDs[WriteEnd]);                                   
      execlp("wc", "wc", "-l", (char *) NULL);             /* Reads from pipe */
      close(pipeFDs[ReadEnd]);            
      _exit(0);                                            /* Signal parent of immediate exit **/         
    } 
    /*** Parent: cleans up annd waits from both childs to finish ***/
    else {                                                            
      if (close(pipeFDs[ReadEnd]) < 0) report_and_exit("close ReadEnd");                                   
      if (close(pipeFDs[WriteEnd]) < 0) report_and_exit("close WriteEnd");                                   
      if(wait(0) < 0) report_and_exit("wait for first child");
      if(wait(0) < 0) report_and_exit("wait for second child");
      exit(0);                               
    }
  } 
  
  return 0;
}
