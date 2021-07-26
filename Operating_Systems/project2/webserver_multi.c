#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include "webserver.h"
#include <semaphore.h>

#define MAX_REQUEST 100

int port, numThread,in,out;
int array[MAX_REQUEST];
sem_t mutex,empty,full;

void *mythread()
{
    while(1)
    {
        int ciccio;
        sem_wait(&full);
        sem_wait(&mutex);
        ciccio=array[out++];
        if(out==MAX_REQUEST)out=0;
        sem_post(&mutex);
        sem_post(&empty);
        process(ciccio);
    }
}

void *listener()
{
    int r;
    struct sockaddr_in sin;
    struct sockaddr_in peer;
    int peer_len = sizeof(peer);
    int sock;


    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    r = bind(sock, (struct sockaddr *) &sin, sizeof(sin));
    if(r < 0)
    {
        perror("Error binding socket:");
        return;
    }

    r = listen(sock, 5);
    if(r < 0)
    {
        perror("Error listening socket:");
        return;
    }

    printf("HTTP server listening on port %d\n", port);

    sem_init(&mutex,0,1);
    sem_init(&empty,0,numThread);
    sem_init(&full,0,0);
    while (1)
    {
        int s;
        s = accept(sock, NULL, NULL);
        if (s < 0) break;
        sem_wait(&empty);
        sem_wait(&mutex);
        array[in++]=s;
        if(in==MAX_REQUEST)in=0;
        sem_post(&mutex);
        sem_post(&full);
    }
    close(sock);
}

void thread_control()
{  /* ----- */
    pthread_t v[numThread],listenert;
    int t;
    in=0;
    out=0;

    pthread_create(&listenert,NULL,listener,NULL);

    for(t=0; t<numThread; t++)
    {
        v[t]=t;
        pthread_create(&v[t],NULL,mythread,NULL);
    }
    while(1)
    {
        for(t=0; t<numThread ; t++)
            if(pthread_tryjoin_np(v[t],NULL)==0)pthread_create(&v[t],NULL,mythread,NULL);
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3 || atoi(argv[1]) < 2000 || atoi(argv[1]) > 50000)
    {
        fprintf(stderr, "./webserver PORT(2001 ~ 49999) #_of_threads\n");
        return 0;
    }

    int i;

    port = atoi(argv[1]);
    numThread = atoi(argv[2]);
    if(argc > 3)
        CRASH = atoi(argv[3]);
    if(CRASH > 50) CRASH = 50;

    thread_control();

    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&mutex);
    return 0;
}

