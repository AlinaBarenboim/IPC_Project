#include "aggregator.h"
#include "intern.h"
#include "../../advance_c/HashMap/HashMap.h"
#define HASH_CAP 10000
#include "dataManager.h"
#include "operatorAggregator.h"
#define KEY_LEN 128
#include <string.h>
#include <stdlib.h>
#define Cellcom 1
#define Orange 2
#define Pelephone 3
#define Parnter 4
#define Vodafone 5

struct  OperAggregator
{
	Queues* m_queues;
	pthread_t* m_threads;
	pthread_t* m_comm_threads;
	int m_isAlive;
	size_t m_numOfThreads;	
	HashMap* m_OpAggHash;
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


static void InsertOpDataToHashMap(OperData* _data, HashMap* _map)
{
	OpAggData* opData;
		
	if(HashMap_Find(_map, _data->m_OperatorBrandName, (void**) &opData) != MAP_SUCCESS)	
	{
		opData = (OpAggData*) calloc(1, sizeof(OpAggData));
		
		opData->m_BrandName = _data->m_OperatorBrandName;
		
		HashMap_Insert(_map, _data->m_OperatorBrandName, opData);
	}

	if(_data->m_isCall)
	{
		if(_data->m_isIncomming)
		{
		
				opData->m_InCallsDuration += _data->m_Duration;
			
		}
		else
		{
			
				opData->m_OutCallsDuration+= _data->m_Duration;

		}
	}
	else if(_data->m_isSMS)
	{
		if(_data->m_isIncomming)
		{
			
				opData->m_SMS_in += 1;
		}
		else
		{
			opData->m_SMS_out += 1;
		}
	}
	
	opData->m_IMSI = _data->m_IMSI;
}
static void OpAggregatorRepFunction (OperAggregator* _opAgg)
{
	Command* command;
	char* names[] = { "Golan", "Cellcom", "Orange", "Pelephone", "Vodafone", "Partner"};
	char* name ;
	
	void* data;
	while (_opAgg->m_isAlive)
	{
		
		SafeQueueRemove(_opAgg->m_queues->m_opAggWithAggForRepReqQueue, (void**)&command);
		
		printf("Operator data aggregator recieved command. NAME: %d\n", command->m_Name);
		
			name = names[command->m_Name];
		
		switch(command->m_typeOfCommand)
		{
			case REP_OPER:
				if(HashMap_Remove(_opAgg->m_OpAggHash, name/*names[command->m_Name]*/, &data) == MAP_SUCCESS)
				{
					printf("Operator data aggregator will now send data to report generator. OpID: %d\n", command->m_Name);
					SafeQueueInsert(_opAgg->m_queues->m_repGenQueue, data);
					printf("Aggregator inserted data into Reporter queue\n");
				}
				break;
		}
	}
}

static void OpAggregatorFunction (OperAggregator* _opAgg)
{
	DataAndContext* dataAndContext;
	OperData* opData;
	while (_opAgg->m_isAlive)
	{
		SafeQueueRemove(_opAgg->m_queues->m_opAggrQueue, (void**)&dataAndContext);
		InsertOpDataToHashMap (((ProcessedData*)dataAndContext->m_data)->m_OperData, _opAgg->m_OpAggHash);
	}

}

OperAggregator* OprAggregatorCreate(Queues* _queues, size_t _threadsNum)
{
	OperAggregator* opAggregator;
	opAggregator = (OperAggregator*)malloc(sizeof(OperAggregator));
	if (NULL == opAggregator)
	{
		return NULL;
	}
	opAggregator->m_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == opAggregator->m_threads)
	{
		free (opAggregator);
		return NULL;
	}
	opAggregator->m_comm_threads = (pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == opAggregator->m_comm_threads)
	{
		free ( opAggregator->m_threads);
		free (opAggregator);
		return NULL;
	}
	
	opAggregator->m_OpAggHash = HashMap_Create(HASH_CAP, (HashFunction) Hash, (EqualityFunction) KeyEqualFunc);
	opAggregator->m_queues = _queues;
	opAggregator->m_isAlive = 1;
	opAggregator->m_numOfThreads = _threadsNum;
	return opAggregator;
}


void DestroyOperAggregator (OperAggregator* _opAgg)
{
	int index;
	void* buff;
	for (index = 0; index < _opAgg->m_numOfThreads; ++index)
	{
		pthread_join(_opAgg->m_threads, &buff);
		pthread_join(_opAgg->m_comm_threads, &buff);
		
	}
	free (_opAgg);
}

oprAggStatus OprAggregatorRun(OperAggregator* _opAgg)
{
	int index = 0;
	for (index = 0; index <_opAgg->m_numOfThreads; ++index )
	{
		pthread_create (&_opAgg->m_threads[index], NULL, (void*(*)(void*))OpAggregatorFunction, _opAgg);
		pthread_create (&_opAgg->m_comm_threads[index], NULL, (void*(*)(void*))OpAggregatorRepFunction, _opAgg);
		
	}
	return OPER_AGGREGATOR_OK ;
}
