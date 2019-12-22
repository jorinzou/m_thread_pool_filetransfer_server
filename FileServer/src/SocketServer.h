#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <syslog.h>
#include <sys/epoll.h>

#define FileDataBaseDir "../FileDataBase"

#define FileNameLen 64
#define FileDirLen 128
struct FileHead{
    char FileName[FileNameLen];
    int FileSize;
    char *mmapfd;
};

typedef struct {
	void *next;
	int SocketFd;
	int ClientAddrLen;
	struct sockaddr_in ClientAddr;
    struct FileHead *pFileHeadInfo;
    char FileDir[FileDirLen];
    pthread_mutex_t ReceDataLock;
    int ReceFlag;
    int IsDataComing;//是否有数据到来
} SocketRecord_t;

#define MAX_CLIENTS 50
#define LISTEN 20
#define EVENT_SIZE LISTEN
#define EPOLL_SIZE EVENT_SIZE
#define MAXLINE 5

SocketRecord_t *GetAuthBySockfd(int ClinetFd);
int SocketSeverInit(int port);
void GetClientFds(int *fds, int maxFds);
int GetNumClients(void);
void SeverPoll(int ClinetFd, int revent);
int SeverSend(uint8_t* buf, int len, int ClientFd);
void CloseSocketFd(void);
void DeleteSocketRecord(int RmSocketFd);
int CreateSocketRecord(void);
void AddEpollEvent(int SockFd);
void DelEpollEvent(int SockFd);
void SetLinger(const int sock, unsigned l_onoff);
void* SeverEpoll(void* userData);
extern int epfd;
extern SocketRecord_t *socketRecordHead;
#endif
