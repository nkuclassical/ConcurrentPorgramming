/*
 *  Assignment #1 Practice with UNIX Process Mechanisms
 *  CS511
 *  Yanqiao Zhan
 *
 *  Two kinds of interprocess Communication, for odd-process using pipe, and
 *  even-process using fifo(named pipe).
 *  Using the common prime produce function, passing the start number and
 *  end number
 *
 *  Personlly speaking, I should say fifo seems more general, and when I
 *  manipulate fifo it makes me feel manipulating a normal file I/O.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_RANGE 1000
#define MAX_PIDS 200

int primeResult[MAX_RANGE];
void prime(int start,int end){		//calculate all prime numbers between(and include) start number and end number.
	if(start>end)return;
    int count=1,i,j;
    if(start<=2)primeResult[count++]=2;
    if(start%2==0)start+=1;
    for(i=start;i<=end;i+=2){
        int isPrime=1;
        for(j=3;j*j<=i;j+=2){
            if(i%j==0){
                isPrime=0;
                break;
            }
        }
        if(isPrime==1){
            primeResult[count++]=i;
        }
    }
}

int main(int argc, char*argv[]){

	int fdpipes[MAX_PIDS][2],fdfifos[MAX_PIDS];
	struct timeval tvptr;
	int max_fd;
	fd_set rset;
	int selectret;
	pid_t parentPid=getpid(),childPid[MAX_PIDS];
	int range[2];
	int curfd;
	int i,j;
	char fifopath[MAX_RANGE][MAX_RANGE];
	if(argc==1){
		printf("usage: %s <increasing positive integers>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	for(i=1;i<argc;i++){
		if(pipe(fdpipes[i])<0){
			printf("pipe error\n");
			exit(EXIT_FAILURE);
		}
		char tempbuffer[MAX_PIDS];
		strcat(fifopath[i],"fifofile");
		sprintf(tempbuffer,"%d",i);
		strcat(fifopath[i],tempbuffer);
		mkfifo(fifopath[i],0755);
	}

	range[0]=2;
	for(i=1;i<argc;i++){
		if((childPid[i-1]=fork())<0){ 		//handle error
			printf("fork error\n");
			exit(EXIT_FAILURE);
		}else if(childPid[i-1]==0){ 		//child process
			close(fdpipes[i][0]);
			if(getpid()%2==0){				//use fifo
				close(fdpipes[i][1]);
			}
			curfd=i;
			range[1]=atoi(argv[i]);
			printf("child %d:bottom=%d,top=%d\n",getpid(),range[0],range[1]);
			break;
			
		}else { 							//parent process
			close(fdpipes[i][1]);
			range[0]=atoi(argv[i])+1;
		}
	}

	if(parentPid==getpid()){                //Parent process select
        /*  1.we must zero the set using FD_ZERO
         *  2.set a certain bit using FD_SET
         *  3.check whether a given bit is still on using FD_ISSET, if yes
         *  read the return value from child process.
         */
		FD_ZERO(&rset);
		max_fd=-1;
		for(i=1;i<argc;i++){
			if(childPid[i-1]%2==1){
				max_fd=(fdpipes[i][0]>max_fd)?fdpipes[i][0]:max_fd;
				FD_SET(fdpipes[i][0], &rset);
			}else{
				if((fdfifos[i]=open(fifopath[i],O_RDONLY))==-1){
					fprintf(stderr,"Unable to open %s : %s\n",fifopath[i],strerror(errno));
					exit(EXIT_FAILURE);
				}
				FD_SET(fdfifos[i], &rset);
				max_fd=(fdfifos[i]>max_fd)?fdfifos[i]:max_fd;
			}
		}
		
		tvptr.tv_sec=1;
		tvptr.tv_usec=0;
		if((selectret=select(max_fd+1,&rset,NULL,NULL,&tvptr))<0){
				printf("select error\n");
				exit(EXIT_FAILURE);
		}

		for(i=0;i<=max_fd;i++){
			if(FD_ISSET(i,&rset)){
				read(i,primeResult,MAX_RANGE);
				for(j=1;j<MAX_RANGE && primeResult[j]!=0;j++){
					printf("%d is prime\n", primeResult[j]);	
				}
				printf("Child %d exit correctly\n", primeResult[0]);
				FD_CLR(i,&rset);
			}
		}
		
        for(i=1;i<argc;i++){
			if(childPid[i]%2==1)
				close(fdpipes[i][0]);	//close pipe fd
			else {
				close(fdfifos[i]);
			}
			unlink(fifopath[i]);		//close all fifo file
		}

	}else{                              //Child process
		prime(range[0],range[1]);		
		if(getpid()%2==0){				//use fifo
			printf("use fifo\n");
			primeResult[0]=getpid();	//put child process's id at first, and then is real prime numbers
			if((fdfifos[curfd]=open(fifopath[curfd],O_WRONLY))==-1){
				printf("Open fifo %s fail.\n", fifopath[curfd]);
				exit(EXIT_FAILURE);
			}
			write(fdfifos[curfd],primeResult,MAX_RANGE);
			close(fdfifos[curfd]);
			
		}else{							//use pipe
			printf("use pipe\n");
			primeResult[0]=getpid();	//same as fifo
			write(fdpipes[curfd][1],primeResult,MAX_RANGE);
			close(fdpipes[curfd][1]);
		}
		exit(EXIT_SUCCESS);
	}
	return 0;
}
