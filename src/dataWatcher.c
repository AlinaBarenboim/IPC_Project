#include "dataWatcher.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../advance_c/SafeQueue/safeQ.h"

struct DataWatcher
{
	Queues* m_queues;
	pthread_t* m_threads;
	pthread_t* m_commThreads;
	pthread_mutex_t m_mutex;
	size_t m_numOfThreads;
	int m_isAlive;
};

void static DataWatcherFunction (DataWatcher* _dataWatcher)
{
	
	void* data;
	while (_dataWatcher->m_isAlive)
	{
		SafeQueueRemove(_dataWatcher->m_queues->m_dataWatcherQueue, &data);
		SafeQueueInsert(_dataWatcher->m_queues->m_dataManagerQueue, data);
		printf("Data watcher just sent data to Data Manager\n");
	}
}

static void DataWatcherCommandFunction (DataWatcher* _dataWatcher)
{
	Command* command;
	
	while(_dataWatcher->m_isAlive)
	{
		SafeQueueRemove(_dataWatcher->m_queues->m_dataWatcherWithDesQueue, (void**) &command);	
		
		printf("Data watcher recieved a command \n ");
		
		switch (command->m_typeOfCommand)
		{
			case PAUSE:
				printf("Data watcher will now be paused now\n");
				pthread_mutex_lock(&_dataWatcher->m_mutex);
				break;
			case RESUME:
				printf("Data watcher will now be resumed now\n");
				pthread_mutex_unlock(&_dataWatcher->m_mutex);
				break;
		}
	}
}


DataWatcher* DataWatcherCreate(Queues* _queues, size_t _threadsNum)
{
	DataWatcher* dataWatcher;
	
	dataWatcher = (DataWatcher*) malloc(sizeof(DataWatcher));
	if(NULL == dataWatcher)
	{
		return NULL;
	}
	
	dataWatcher->m_threads = (pthread_t*) malloc(_threadsNum * sizeof(pthread_t));
	if (NULL == dataWatcher->m_threads)
	{
		free (dataWatcher);
		return NULL;
	}
	dataWatcher->m_commThreads = (pthread_t*) malloc(_threadsNum * sizeof(pthread_t));
	if (NULL == dataWatcher->m_commThreads)
	{
		free (dataWatcher);
		free (dataWatcher->m_threads);
		return NULL;
	}
	dataWatcher->m_queues = _queues;
	dataWatcher->m_numOfThreads = _threadsNum;
	dataWatcher->m_isAlive = 1;
	pthread_mutex_init(&dataWatcher->m_mutex, NULL);
	return dataWatcher;
}

dWStatus DataWatcherRun(DataWatcher* _dataWatcher)
{
	int index;
	void* buff;
	for(index = 0; index < _dataWatcher->m_numOfThreads; ++index)
	{
		pthread_create(&_dataWatcher->m_threads[index], NULL, (void*(*)(void*))DataWatcherFunction, _dataWatcher);
	}
	for(index = 0; index < _dataWatcher->m_numOfThreads; ++index)
	{
		pthread_create(&_dataWatcher->m_commThreads[index], NULL, (void*(*)(void*))DataWatcherCommandFunction, _dataWatcher);
	}
	return DATA_WATCHER_OK;
}

void DataWatcherDestroy(DataWatcher* _dataWatcher)
{
	int index;
	void* buff;
	
	for(index = 0; index < _dataWatcher->m_numOfThreads; ++index)
	{
		pthread_join(_dataWatcher->m_threads[index], &buff);
	}
	for(index = 0; index < _dataWatcher->m_numOfThreads; ++index)
	{
		pthread_join(_dataWatcher->m_commThreads[index], &buff);
	}
	free(_dataWatcher);
}
