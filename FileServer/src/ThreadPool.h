#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>
#include <stdlib.h>
#include "SocketServer.h"

struct thInfo{
    pthread_mutex_t mutex_lock;
    pthread_cond_t  cond; 
    pthread_t ThreadId;
    int Iswork;
};

struct ThPool{
   struct thInfo *thinfo;
   int ThreadNum;
};

extern struct ThPool *thpool;

void* ThreadRoutine(void *arg);
int ThreadPoolCreate(int ThreadNum);
SocketRecord_t *TraverseSockClient(void);
void ShowWorkTthread(void);

#endif
