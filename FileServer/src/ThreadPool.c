#include "ThreadPool.h"
#include "debug.h"

struct ThPool *thpool;

void ShowWorkTthread(void)
{
    int i;
    for(i = 0; i < thpool->ThreadNum; i++){
        DEBUG_INFO("Iswork=%d",thpool->thinfo[i].Iswork);
    }
}


/*工作线程*/
void* ThreadRoutine(void *arg)
{
   struct thInfo *thinfo = (struct thInfo *)arg;
   while(1){
        pthread_mutex_lock(&(thinfo->mutex_lock));
        pthread_cond_wait(&(thinfo->cond), &(thinfo->mutex_lock));
        DEBUG_INFO("test3");
        thinfo->Iswork = 1;
        SocketRecord_t *SrcRecord = TraverseSockClient();
        if (SrcRecord != NULL){
            RecvFileDataThread(SrcRecord);
        }
        else{
			thinfo->Iswork = 0;
			pthread_mutex_unlock(&(thinfo->mutex_lock));
            continue;
        }
        SrcRecord->IsDataComing = 0;
        thinfo->Iswork = 0;
        pthread_mutex_unlock(&(thinfo->mutex_lock));
   }
}

/*创建线程池*/
int ThreadPoolCreate(int ThreadNum)
{
    thpool = calloc(1,sizeof(struct ThPool));
    if(thpool == NULL){
        DEBUG_INFO("create thpool failed");
        return -1;
    }

    thpool->thinfo = calloc(ThreadNum,sizeof(struct thInfo));
    if (thpool->thinfo == NULL){
        DEBUG_INFO("create ThIdArr failed");
        return -1; 
    }

     thpool->ThreadNum = ThreadNum;

    int i = 0;
    for(i; i < ThreadNum; ++i){
       thpool->thinfo[i].Iswork = 0;
       pthread_mutex_init(&(thpool->thinfo->mutex_lock), NULL);
       pthread_cond_init(&(thpool->thinfo->cond), NULL);
       int ret = pthread_create(&(thpool->thinfo[i].ThreadId),NULL,ThreadRoutine,(void*)&(thpool->thinfo[i]));
        if (ret < 0){
            DEBUG_INFO("pthread create failed");
            return -1;
        }
    }
}

SocketRecord_t *TraverseSockClient(void)
{
    SocketRecord_t *SrcRecord =  NULL;
	SrcRecord = socketRecordHead;
	while (SrcRecord) {
        if (SrcRecord->ReceFlag == 0 && SrcRecord->IsDataComing == 1){
            return SrcRecord;
        }
		SrcRecord = SrcRecord->next;
	}
    return NULL;
}


