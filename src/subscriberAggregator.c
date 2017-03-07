#include "aggregator.h"
#include "intern.h"
#include "../../advance_c/HashMap/HashMap.h"
#define HASH_CAP 10000
#include "dataManager.h"
#include <stdlib.h>
#include <stdio.h>
#include "subscriberAggregator.h"
#include <pthread.h>
#define KEY_LEN 128
struct SubAggregator
{
	Queues* m_queues;
	pthread_t* m_threads;
	pthread_t* m_rep_threads;
	int m_isAlive;
	size_t m_numOfThreads;	
	HashMap* m_SubAggHash;
};

static int Hash(char *str)
{
	int hash = 5381;
	int c;

	while (c = *(str++))
	hash = ((hash << 5) + hash) + c; 

	return hash;
}

static int KeyEqualFunc(int* _num1, int* _num2)
{
	return *_num1 == *_num2;
}

static void InsertSubDataToHashMap(SubData* _data, HashMap* _map)
{
	SubAggData* subData;	
	if(HashMap_Find(_map, _data->m_MSISDN, (void**) &subData) != MAP_SUCCESS)	
	{
		subData = (SubAggData*) calloc(1, sizeof(SubAggData));
		
		subData->m_MSISDN = _data->m_MSISDN;
		
		HashMap_Insert(_map, _data->m_MSISDN, subData);
	}

	if(_data->m_isCall)
	{
		if(_data->m_isIncomming)
		{
			if(_data->m_isCrossOp)
			{
				subData->m_InCallsOutOper += _data->m_Duration;
			}
			else
			{
				subData->m_InCallsWithMobOper += _data->m_Duration;
			}
		}
		else
		{
			if(_data->m_isCrossOp)
			{
				subData->m_OutCallsOutOper += _data->m_Duration;
			}
			else
			{
				subData->m_OutCallsWithMobOper += _data->m_Duration;
			}
		}
	}
	else if(_data->m_isSMS)
	{
		if(_data->m_isIncomming)
		{
			if(_data->m_isCrossOp)
			{
				subData->m_SMS_receivedOutOper += 1;
			}
			else
			{
				subData->m_SMS_receivedWithOper += 1;
			}
		}
		else
		{
			if(_data->m_isCrossOp)
			{
				subData->m_SMS_sentOutOper += 1;
			}
			else
			{
				subData->m_SMS_sentWithOper += 1;
			}
		}
	}
	else if(_data->m_isGPRS)
	{
		subData->m_MB_Download += _data->m_Download;
		subData->m_MB_Upload += _data->m_Upload;
	}
	
}

static void SubAggregatorRepFunction (SubAggregator* _subAgg)
{
	Command* command;
	char MSISDN[KEY_LEN];
	void* data;
	
	while (_subAgg->m_isAlive)
	{
		
		SafeQueueRemove(_subAgg->m_queues->m_subAggWithAggForRepReqQueue, (void**)&command);
		sprintf(MSISDN, "%d", command->m_Name);
		printf("Subscriber data aggregator recieved command. MSISDN: %s\n", MSISDN);
		switch(command->m_typeOfCommand)
		{
			case REP_ONE_SUB:
				if(HashMap_Remove(_subAgg->m_SubAggHash, MSISDN, &data) == MAP_SUCCESS)
				{
					printf("Subscriber data aggregator will now send data to report generator. SubID: %d\n", command->m_Name);
					SafeQueueInsert(_subAgg->m_queues->m_repGenQueue, data);
					printf("Aggregator inserted data into Reporter queue\n");
				}
				break;
		}
	}
}
	
static void SubAggregatorFunction (SubAggregator* _subAgg)
{
	DataAndContext* dataAndContext;
	SubData* subData;
	while (_subAgg->m_isAlive)
	{

		SafeQueueRemove(_subAgg->m_queues->m_subAggrQueue, (void**)&dataAndContext);
		printf ("HASH SIZE: %lu \n", (HashMap_Size(_subAgg->m_SubAggHash)));
		InsertSubDataToHashMap (((ProcessedData*)dataAndContext->m_data)->m_SubData, _subAgg->m_SubAggHash);
	}
}


SubAggregator* SubAggregatorCreate (Queues* _queues, size_t _threadsNum)
{
	SubAggregator* subAggregator;
	subAggregator = (SubAggregator*)malloc(sizeof(SubAggregator));
	if (NULL == subAggregator)
	{
		return NULL;
	}
	subAggregator->m_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == subAggregator->m_threads)
	{
		free (subAggregator);
		return NULL;
	}
	subAggregator->m_rep_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == subAggregator->m_rep_threads)
	{
		free (subAggregator->m_threads);
		free (subAggregator);
		return NULL;
	}

	subAggregator->m_SubAggHash = HashMap_Create(HASH_CAP, (HashFunction) Hash, (EqualityFunction) KeyEqualFunc);
	subAggregator->m_queues = _queues;
	subAggregator->m_isAlive = 1;
	subAggregator->m_numOfThreads = _threadsNum;
	return subAggregator;
}


void DestroySubAggregator (SubAggregator* _subAgg)
{
	int index;
	void* buff;
	for (index = 0; index < _subAgg->m_numOfThreads ; ++index)
	{
		pthread_join(_subAgg->m_threads[index], &buff);
	}
	for (index = 0; index < _subAgg->m_numOfThreads ; ++index)
	{
		pthread_join(_subAgg->m_rep_threads[index], &buff);
	}
	free (_subAgg);
}

subAggStatus SubAggregatorRun (SubAggregator* _subAgg)
{
	int index = 0;
	for (index = 0; index <_subAgg->m_numOfThreads; index ++)
	{
		pthread_create (&_subAgg->m_threads[index], NULL, (void*(*)(void*))SubAggregatorFunction, _subAgg);
		pthread_create (&_subAgg->m_rep_threads[index], NULL, (void*(*)(void*))SubAggregatorRepFunction, _subAgg);
	}
	
	return  SUB_AGGREGATOR_OK;
}
