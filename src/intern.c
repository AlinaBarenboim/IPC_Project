#include "intern.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int GetShmId( const char* _projId, const char* _fileName)
{
	key_t key;
	int res;
	char* fileName = _fileName;
	char* projId = _projId;
	int shMemId;
	key = ftok(fileName, *projId);
	printf("KEY: %d\n", key);
	res = shmget(key, 20, IPC_CREAT | 0666);
	printf ("smget: %d", res);
	return res;
}

IpcQueue* GetSharedIPCQueue(int _shmId)
{
	IpcQueue* sharedIPCQueue;
	sharedIPCQueue = shmat(_shmId, NULL, 0);
	if((int)sharedIPCQueue == -1)
	{
		return NULL;
	}
	IpcQueueCreate(sharedIPCQueue);
	return sharedIPCQueue;
}


/*ipcrm -M +key*/
