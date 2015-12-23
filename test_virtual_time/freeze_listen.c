//#include <sys/stat.h>
//#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
//#include <assert.h>
#include <stdlib.h>

#include "vtutil.h"

#define MAX_NUM_PIDS 512

#define SHOW_OVHD 1

/*  Author channon@iit.edu
 *  Listens on /tmp/fifo.tmp
 *    for a signal
 *  CLA: pids space delimited
 */

int main (int argc, char **argv){

  char buf[1];
  
  //read in pids
  pid_t pid;

  pid_t pid_list[MAX_NUM_PIDS];
  size_t pid_size = argc -1; // -1 because program name is argv[0]
  //char bufprev[1];
  int fd;
  int l; // read length

  // Create FIFO
  char *filename = "/tmp/fifo.tmp";
  int i;

#ifdef SHOW_OVHD
  
  struct timeval start, end, ovhd;
  long usec;

#endif

  for(i=1;i<argc;i++)
    {
      pid_list[i-1] = atoi(argv[i]);
      printf("%s\n", argv[i]);
    }
  
  fd = open(filename, O_RDONLY);

  printf("%d\n",argc);
  printf("\n");
  
  
  while (1){

    
#ifdef SHOW_OVHD
    // get timeBerfore
    gettimeofday(&start, NULL);

#endif

    l = read (fd, buf, 1);//MAX_BUF);
    
    if(l)

      {
	//printf("Recieved: %c\n", buf[0]);

       if(buf[0]=='p')
	{
	  //pause
	  //printf("kicking\n");
	  kickoff_pthreads(pid_list, pid_size, freeze_work, NULL);
	  
	  //printf("kicked\n");
	  //exit(0);
	}
      else
	{
	  //resume
	      
	  //printf("kicking");
	  kickoff_pthreads(pid_list, pid_size, unfreeze_work, NULL);
	      
	  //rintf("kicked");
	  //exit();
	}
      
       //get time after
       //log
#ifdef SHOW_OVHD
       gettimeofday(&end, NULL);
       timeval_substract(&ovhd, &end, &start);
       usec = timeval_to_usec(ovhd);
       printf("%ld\n",usec);
#endif
      }
    //} 
  }


  close(fd); 
  // Close and delete FIFO
}
