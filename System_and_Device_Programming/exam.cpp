//Exercise 1 (this exercise was slighlty refined after the exam to check it with an example)
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <process.h>


#define N 3 // semaphore value
#define TNUM 8// number of threads

int cnt;
HANDLE cntmtx;
void WINAPI threadF(LPVOID);
//HANDLE se; //SEMPAHORES


//initialiazion
HANDLE *mutexes; //could be also inside of _tmain and then passed as param to threadF

INT _tmain() {

	
	int i = 0;
	HANDLE  *threadHandle;
	DWORD *threadId;
	INT * threadData;
	threadId = (DWORD *)malloc(TNUM * sizeof(DWORD));
	threadData = (INT *)malloc(TNUM * sizeof(INT));
	cnt = 0;
	cntmtx = (HANDLE)malloc(sizeof(HANDLE));
	//se = CreateSemaphore(NULL, 3, 3, NULL);//SEMAPHORES
	

	mutexes = (HANDLE *)malloc(N * sizeof(HANDLE));

	cntmtx = CreateMutex(NULL, FALSE, NULL);
	
	for (i = 0; i < N; i++)
		mutexes[i] = CreateMutex(NULL, FALSE, NULL);

	//... call threadF ecc...

	threadHandle = (HANDLE *)malloc(TNUM * sizeof(HANDLE));

	for (i = 0; i < TNUM; i++) {
		
		threadData[i] = i+1;
		threadHandle[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) threadF, &threadData[i], 0, &threadId[i]);

		if (threadHandle[i] == NULL) {
			ExitProcess(0);
		}
	}

	WaitForMultipleObjects(TNUM, threadHandle, TRUE, INFINITE);
	for (i = 0; i < TNUM; i++) {
		CloseHandle(threadHandle[i]);
	}
	
	free(threadHandle);
	free(threadId);
	free(threadData);
	for (i = 0; i < N; i++) CloseHandle(mutexes[i]);
	CloseHandle(cntmtx);

	//missing mutex distruction
	free(mutexes);

	return 0;
}


//inside thread function for example:
void WINAPI threadF(LPVOID lpParam) {
	INT *p;
	p = (INT *)lpParam;
	INT threadId = *p ;
	INT nOfIterations=10;
	

	int index, ind;

	while (--nOfIterations>0) { 

		//WaitForSingleObject(se, INFINITE);//SEMPAHORES

		//prologue I wait for an array of mutexes. If all mutexes are acquired the WFMO will stop the thread wrking as a sempahore
		//in this case there is no need to remove the waited mutex from the array after the WFMO

		ind = WaitForMultipleObjects(N, mutexes, FALSE, INFINITE); // false because I need to acquire onlly one mutex
		index = (int)ind - (int)WAIT_OBJECT_0; //get which mutex acquired to later release it after CS

		//Critical section !!

		
		WaitForSingleObject(cntmtx, INFINITE);
		cnt++;
		ReleaseMutex(cntmtx);

		_tprintf("Currently %d threads in Critical Section, printed by thread n %d\n", cnt, threadId);

		WaitForSingleObject(cntmtx, INFINITE);
		cnt--;
		ReleaseMutex(cntmtx);

		//epilogue
		ReleaseMutex(mutexes[index]);

		//ReleaseSemaphore(se, 1, NULL);//SEMPAHORES
	}
	ExitThread(0);
}



//Exercise 2

/*barrier is needed between A and B. After the barrier is intiialized between A and B the following
code has to be inserted if we implemented the barrier from scratch supposing we defined the
following data struct:*/

typedef struct b{
int cnt; //to check how many threadsw reached the barrier
sem_t * barrier_sem;
pthread_mutex_t *mtx; // to protect cnt
} barrier_t;

barrier_t * barrier; //allocations and inits missing since only code between A and B was requested!!!

//CODE BETWEEN A AND B: ( in each thread function)

//Q1.
pthread_mutex_lock(barrier->mtx);
if (++barrier->cnt == N){ //last thread enters the if and let all threads (that are waiting on sem-wait)
go to section B
for(i=0; i<N; i++)
sem_post(barrier->barrier_sem);
}
pthread_mutex_unlock(barrier->mutex);
sem_wait(barrier->barrier_sem);

//Q2.
/*In case this code is inserted into a loop another barrier is needed to decrease the counter and
As a matter of fact with the code shown above one faster thread after being let go by the
sem_post could
start another iteration by arriving twice to sem_post while the loop of sem_post is not finished yet.
The following code should be added to the first part shown before*/

pthread_mutex_lock(barrier1->mtx);
if(--barrier1->cnt==0){
for (i=0;i<N; i++)
sem_post(barrier1->barrier_sem);
}

pthread_mutex_unlock(barrier1->mtx);
sem_wait(barrier1->barrier_sem)
	//and after this the loop can start again



//Exercise 3

	void admin_f()
{
	std::unique_lock<std::mutex> admin_lock{m};
	var = 10;
	adderCV.notify_all();

	adminCV.wait(admin_lock);
	cout << var << 
	
	return;
}
void adder_f()
{
	std::unique_lock<std::mutex> adder_lock{m};
	adderCV.wait(adder_lock); // wait for initialization on the conditional variable
	while (1)
	{
		m.lock();
		if (var < 15)
		{
			var += rand() % 5;
			if (var >= 15) // if var is over the threshold, notify admin and exit
			{
				adminCV.notify_one();
				m.unlock();
				return;
			}
		}
		else
		{
			m.unlock();
			return;
		}
		m.unlock();
	}
	//the notify part of the termination conditipn "when the 3 thread finishes the admin thread prints final value
	//is already handled in the main so here I just have to notify the admin when var is changed and it	is higer tahn 15 so that the main thread check if the value is higher return; //()
}

//not ok check sol



// Exercise 5

//(closing of all handles missing just code of the exam)



INT cntlastbyte(TCHAR *filename)
{
	HANDLE file, hMao;
	DWORD FileSize, FilePos;
	DWORD StartingOffset;
	LONG_INT offset, size;
	ov = {0, 0, 0, 0, NULL} 
	TCHAR * pFile, start;
	TCHAR c;
	int lastequalzero = 0, lastequalone = 0;
	int i;

	offset.QuadPart = 100*1<<20;
	size.QuadPart = 1<<a30; 
	file = CreateFile(filename, GENERIC_READ, OPEN_EXISTING, 0, NULL, FILE_ATTRIBUTE_NORMAL, NULL); 
	GetFileSize(file, &FileSize);
	hMap = CreateFileMapping(file, NULL, 0, FileSize.HighPat, FileSize.LowPart);
	FilePos.QuadPart = OFFSET * sizeof(TCHAR);
	pFile = (TCHAR *)MapViewOfFile(hMap, 0, FilePos.HighPart, FilePos.LowPart, (FULLOFFSET) * sizeof(TCHAR));
	i = 0;
	
	while (pFile < (offset.QuadPart + size.QuadPart))
	{
		c = pFile;
		//shift left and the right back t get only last bit
		c = c << 7;
		c = c >> 7;

		if(c == 1) // I don't know how to check the value of the remaining bit -> don't remember ASCII
			lastequalone++;
		else
			lastequalzero++,

		pFile++;
	}

	_tprintf("Contains %d last bits equal to 1 and %d last bits equal to 0", lastequalone, lastequalzero);

	return 0;

}

