#include "intern.h"
#include <stdio.h>
#include <pthread.h>
#include "despatcher.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdlib.h>
#include <unistd.h>
#include "unistd.h"
#include "watchDog.h"
#include "../../advance_c/HashMap/HashMap.h" 
#include "../../advance_c/ipcQueue/queue.h"
#define PROJ_ID "A"
#define FILE "file1"
#define ALIVE 1

struct WatchDog
{
	IpcQueue* m_queue;
	pthread_t m_rt_thread;
	int m_shmId;
	int m_isAlive;
};

static void* KeepAlive(WatchDog* _watchDog)
{
	int ping;

	printf("Watchdog keep alive function\n");
	while(_watchDog->m_isAlive)
	{
		printf("Watchdog will remove from queue now\n");
		IpcQueueRemove(_watchDog->m_queue, &ping);
		printf("%x\n", ping);
	}
}

WatchDog* WatchDogCreate(const char* _configFile)
{
	WatchDog* watchdog;
	char* projId;
	char* fileName;
	IpcQueue* queue;
	projId = PROJ_ID;
	fileName = FILE;
	watchdog = (WatchDog*) malloc(sizeof(WatchDog));
	if(NULL == watchdog)
	{
		return NULL;
	}
	watchdog->m_shmId = GetShmId(projId, fileName);
	queue = GetSharedIPCQueue(watchdog->m_shmId);	
	if(NULL == queue)
	{
		free(watchdog);
		return NULL;
	}	
	watchdog->m_queue = queue;
	watchdog->m_isAlive = ALIVE;
	return watchdog;
}

WatchDogStatus WatchDogRun(WatchDog* _watchDog)
{
	pid_t pid;
	int status;
	char* arg[2];
	arg[0] = "./rt";
	arg[1] = NULL;
	pid = fork();
	if(pid)
	{
		printf("Watchdog. Will create thread now\n");
		pthread_create(&_watchDog->m_rt_thread, NULL, (void*(*)(void*))KeepAlive, _watchDog);
		waitpid(pid, &status, 0); 
	}
	else
	{
		execvp(arg[0], arg);
	}	
}


void WatchDogDestroy(WatchDog* _watchDog)
{
	void* buff;
	pthread_join(_watchDog->m_rt_thread, &buff);
	_watchDog->m_isAlive = 0;
	shmctl(_watchDog->m_shmId, IPC_RMID, NULL);
	free(_watchDog);
}


