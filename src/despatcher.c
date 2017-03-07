#include <stdio.h>
#include "despatcher.h"
#include "../../advance_c/SafeQueue/safeQ.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "../../advance_c/HashMap/HashMap.h" 
#include "../../advance_c/ipcQueue/queue.h"
#include <stdlib.h>
#define PING 0xBABE
#include <unistd.h>
#define SLEEP 1
#define ALIVE 1
#include "intern.h"
#define PROJ_ID "B"
#define FILE "file2"
#define PROJ_ID2 "A"
#define FILE2 "file1"
#include <stdlib.h>
#include "fileWatcher.h"
#include <string.h>
struct Dispatcher
{
	IpcQueue* m_wd_IPCQueue;
	IpcQueue* m_UI_IPCQueue;
	pthread_t m_watchDogThread;
	pthread_t m_UIThread;
	int m_shmId_wd;
	int m_shmId_ui;
	int m_isAlive;
	Queues* m_queues;
	
};

static void SendPing(Dispatcher* _dispatcher)
{
	while(_dispatcher->m_isAlive)
	{
		sleep(1);
		IpcQueueInsert(_dispatcher->m_wd_IPCQueue, PING);
	}
}

static void GetMsg(Dispatcher* _dispatcher)
{
	int msg;
	while(_dispatcher->m_isAlive)
	{
		Command* command;
		IpcQueueRemove(_dispatcher->m_UI_IPCQueue, &msg);
		printf("Dispatcher got message:  \n");
		command =(Command*)(&msg);
		printf ("command->m_command: %d \n" , command->m_typeOfCommand);
		printf ("command->m_name : %d \n" , command->m_Name);

		switch (command->m_typeOfCommand) 
		{
			case REP_ONE_SUB:
				printf("Dispatcher sending message to aggregator.One subscriber report will be created.\n");
				SafeQueueInsert(_dispatcher->m_queues->m_aggWithDespQueue, command);
				break;
			case REP_MANY_SUBS:
				printf("Dispatcher sending message to aggregator.Many subscribers report will be created.\n");
				SafeQueueInsert(_dispatcher->m_queues->m_aggWithDespQueue, command);
				break;
			case REP_OPER:
			
				printf("Dispatcher sending message to aggregator.One oper report will be created.\n");
				SafeQueueInsert(_dispatcher->m_queues->m_aggWithDespQueue, command);
				break;	
				
			case REP_MANY_OPERS:
				printf("Dispatcher sending message to aggregator.Many opers report will be created.\n");
				SafeQueueInsert(_dispatcher->m_queues->m_aggWithDespQueue, command);
				break;
				
			case PAUSE:
				printf("Dispatcher sending message to data watcher. System will pause.\n");
				SafeQueueInsert(_dispatcher->m_queues->m_dataWatcherWithDesQueue, command);
				FileWatcherPause ();
				SafeQueueInsert(_dispatcher->m_queues->m_fileWatcherWithDesQueue, command);
				break;
				
			case RESUME:
				printf("Dispatcher sending message to data watcher. System will resume.\n");
				SafeQueueInsert(_dispatcher->m_queues->m_dataWatcherWithDesQueue, command);
				FileWatcherWakeUp();
				SafeQueueInsert(_dispatcher->m_queues->m_fileWatcherWithDesQueue, command);
				break;
		}	
	}
}

Dispatcher* DispatcherCreate(const char* _configFile, Queues* _queue)
{

	HashMap* configMap;
	IpcQueue* queue;
	IpcQueue* UIqueue;
	Dispatcher* dispatcher;
	char* projId;
	char* fileName;
	projId = PROJ_ID;
	fileName = FILE;
	dispatcher = (Dispatcher*) malloc(sizeof(Dispatcher));
	if(NULL == dispatcher)
	{
		return NULL;
	}
	
	dispatcher->m_shmId_ui = GetShmId(projId, fileName);
	queue = GetSharedIPCQueue(dispatcher->m_shmId_ui);
	if(NULL == queue)
	{
		free(dispatcher);
		return NULL;
	}
	dispatcher->m_UI_IPCQueue = queue;
	dispatcher->m_isAlive = ALIVE;
	dispatcher->m_queues = _queue;
	return dispatcher;
}

DispStatus DispatcherRun(Dispatcher* _dispatcher)
{	
/*	printf ("Despatcher run!!!! \n");*/
	int res;
/*	pthread_create(&_dispatcher->m_watchDogThread, NULL, (void*(*)(void*))SendPing, _dispatcher);*/
	res = pthread_create(&_dispatcher->m_UIThread, NULL, (void*(*)(void*))GetMsg, _dispatcher);
/*	printf ("pthread res: %d \n", res);*/
	return DESPATCHER_OK;
}

void DispatcherDestroy(Dispatcher* _dispatcher)
{
	void* buff;
/*	pthread_join(_dispatcher->m_watchDogThread, &junk);*/
	pthread_join(_dispatcher->m_UIThread, &buff);
	shmctl(_dispatcher->m_shmId_ui, IPC_RMID, NULL);
	_dispatcher->m_isAlive = 0;
	free(_dispatcher);
}





