/*************************************************
	* C program to count no of lines, words and 	 *
	* characters in a file.			 *
	*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_PROC 100
#define MAX_FORK 100

typedef struct count_t {
    int linecount;
    int wordcount;
    int charcount;
} count_t;


typedef struct plist_t {
		int pid;
		int offset;
	//	int pipefd[2];
        int size;
        int pipefd;
} plist_t;

int CRASH = 0;

count_t word_count(FILE* fp, long offset, long size)
{
		char ch;
		long rbytes = 0;

		count_t count;
		// Initialize counter variables
		count.linecount = 0;
		count.wordcount = 0;
		count.charcount = 0;

		printf("[pid %d] reading %ld bytes from offset %ld\n", getpid(), size, offset);

		if(fseek(fp, offset, SEEK_SET) < 0) {
				printf("[pid %d] fseek error!\n", getpid());
		}
		int words=0;

		while ((ch=getc(fp)) != EOF && rbytes < size) {
				// Increment character count if NOT new line or space
				if (ch != ' ' && ch != '\n') { ++count.charcount; }

				// Increment word count if new line or space character
				if (ch == ' ' || ch == '\n') {++count.wordcount;words++; }

				// Increment line count if new line character
				if (ch == '\n') { ++count.linecount;}
				rbytes++;
		}

		srand(getpid());
		if(CRASH > 0 && (rand()%100 < CRASH))
		{
				printf("[pid %d] crashed.\n", getpid());
				abort();
		}

		return count;
}

int main(int argc, char **argv)
{
                long fsize,tsize,offset=0;
		FILE *fp;
		int numJobs,fd[MAX_PROC][2],cnt=0,nFork = 0,i, pid, status;
		plist_t plist[MAX_PROC];
		count_t total, count;

		if(argc < 3) {
				printf("usage: wc <# of processes> <filname>\n");
				return 0;
		}
		if(argc > 3) {
				CRASH = atoi(argv[3]);
				if(CRASH < 0) CRASH = 0;
				if(CRASH > 50) CRASH = 50;
		}
		printf("CRASH RATE: %d\n", CRASH);


		numJobs = atoi(argv[1]);
		if(numJobs > MAX_PROC) numJobs = MAX_PROC;

		total.linecount = 0;
		total.wordcount = 0;
		total.charcount = 0;

		// Open file in read-only mode
		fp = fopen(argv[2], "r");

		if(fp == NULL) {
				printf("File open error: %s\n", argv[2]);
				printf("usage: wc <# of processes> <filname>\n");
				return 0;
		}

		fseek(fp, 0L, SEEK_END);
		fsize = ftell(fp);
		fclose(fp);
		// calculate file offset and size to read for each child
        tsize=fsize/numJobs;
		for(i = 0; i < numJobs; i++) {
                offset=i*tsize;
                if(i==numJobs-1) tsize=fsize-(numJobs-1)*tsize;
				//set pipe;
                if(pipe(fd[i])==-1)printf("error creating pipe\n");
				if(nFork++ > MAX_FORK) return 0;
				pid = fork();
				if(pid < 0) {
						printf("Fork failed.\n");
				} else if(pid == 0) {
						// Child
						fp = fopen(argv[2], "r");
						count = word_count(fp, offset, tsize);
						// send the result to the parent through pipe
						close(fd[i][0]);
						write(fd[i][1],&count,sizeof(count_t));
						close(fd[i][1]);
						fclose(fp);
						return 0;
				}
				if(pid>0){
                    plist[i].offset=offset;
                    plist[i].pid=pid;
                    plist[i].pipefd=i;
                    plist[i].size=tsize;
				}
		}
		// Parent
		// wait for all children
		// check their exit status
		// read the result of normalliy terminated child
		// re-create new child if there is one or more failed child
		// close pipe
while(cnt<numJobs){
    pid=wait(&status);
    i=0;
    while(pid!=plist[i].pid){
            i++;
            }
            if(WIFEXITED(status)){
            cnt++;
            close(fd[plist[i].pipefd][1]);
            read(fd[plist[i].pipefd][0],&count,sizeof(count_t));
            close(fd[plist[i].pipefd][0]);
            total.charcount=total.charcount+count.charcount;
            total.linecount=total.linecount+count.linecount;
            total.wordcount=total.wordcount+count.wordcount;
        }else{
        if(pipe(fd[plist[i].pipefd])==-1)printf("error creating pipe 2");
        nFork=0;
if(nFork++ > MAX_FORK) return 0;
        pid=fork();
        if(pid==0){
        //child
        fp = fopen(argv[2], "r");
        count = word_count(fp, plist[i].offset, plist[i].size);
        // send the result to the parent through pipe
        close(fd[plist[i].pipefd][0]);
        write(fd[plist[i].pipefd][1],&count,sizeof(count_t));
        close(fd[plist[i].pipefd][1]);
        fclose(fp);
        return 0;
        }    else{
        plist[i].pid=pid;
        }
    }
}

		printf("\n========== Final Results ================\n");
		printf("Total Lines : %d \n", total.linecount);
		printf("Total Words : %d \n",total.wordcount);
		printf("Total Characters : %d \n", total.charcount);
		printf("=========================================\n");
		return(0);
}

