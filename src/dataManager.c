#include "dataManager.h"
#include "intern.h"
#include <pthread.h>
#include <stdlib.h>

struct DataManager
{
	Queues* m_queues;
	pthread_t* m_threads;
	int m_isAlive;
	size_t m_numOfThreads;
};

static void DataManagerFunction(DataManager* _dataManager)
{
	DataAndContext* dataAndContext;
	while(_dataManager->m_isAlive)
	{
	        SafeQueueRemove(_dataManager->m_queues->m_dataManagerQueue, (void**) &dataAndContext);
/*		printf ("data: %p", dataAndContext->m_context);*/
		if(ROW_DATA == dataAndContext->m_context)
		{
			SafeQueueInsert(_dataManager->m_queues->m_readerQueue, dataAndContext);
/*			printf("Data manager just inserted item inside readerQueue\n");*/
		}
		else if(PROCESSED_DATA == dataAndContext->m_context)
		{
			SafeQueueInsert(_dataManager->m_queues->m_aggregatorQueue, dataAndContext);
/*			printf("Data manager just inserted item inside aggregator Queue\n");*/
		}
	}
}

DataManager* DataManagerCreate(Queues* _queues, size_t _threadsNum)
{
	DataManager* dataManager;

	dataManager = (DataManager*) malloc(sizeof(DataManager));
	if(NULL == dataManager)
	{
		return NULL;
	}
	
	dataManager->m_threads = (pthread_t*) malloc(_threadsNum * sizeof(pthread_t));
	if(NULL == dataManager->m_threads)
	{
		free(dataManager);
		return NULL;
	}
	
	dataManager->m_numOfThreads = _threadsNum;
	dataManager->m_queues = _queues;
	dataManager->m_isAlive = 1;

	return dataManager;
}

DataManagerStatus DataManagerRun(DataManager* _dataManager)
{
	int index;
	for(index = 0; index < _dataManager->m_numOfThreads; ++index)
	{
		pthread_create(&_dataManager->m_threads[index], NULL, (void*(*)(void*)) DataManagerFunction, _dataManager);
	}
	return DM_OK;
}

void DataManagerDestroy(DataManager* _dataManager)
{
	int index;
	void* buff;
	for(index = 0; index < _dataManager->m_numOfThreads; ++index)
	{
		pthread_join(_dataManager->m_threads[index], &buff);
	}
	free(_dataManager);
}
