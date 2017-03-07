#include "aggregator.h"
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../advance_c/SafeQueue/safeQ.h"
#include <stdio.h>
#include <unistd.h>

struct Aggregator
{
	Queues* m_queues;
	pthread_t* m_threads;
	int m_isAlive;
	size_t m_numOfThreads;
	pthread_t* m_rep_threads;		
};

static void AggregatorRepFunction (Aggregator* _agg) 
{
	Command* command;
	while (_agg->m_isAlive)
	{
		SafeQueueRemove (_agg->m_queues->m_aggWithDespQueue, (void**)&command);
		switch (command->m_typeOfCommand)
		{
			case REP_ONE_SUB:
				SafeQueueInsert(_agg->m_queues->m_subAggWithAggForRepReqQueue, command);
				printf("Data aggregator sent command to subscriber data aggregator\n");
				break;
			case REP_OPER:
			SafeQueueInsert(_agg->m_queues->m_opAggWithAggForRepReqQueue, command);
			printf("Data aggregator sent command to operator data aggregator\n");
			break;
		}
	}
}

static void AggregatorFunction (Aggregator* _agg) 
{
	DataAndContext* dataAndContext;
	while (_agg->m_isAlive) 
	{
		SafeQueueRemove (_agg->m_queues->m_aggregatorQueue, (void**)&dataAndContext);
			dataAndContext->m_context == DATA_TO_INSERT;
			SafeQueueInsert (_agg->m_queues->m_subAggrQueue, dataAndContext);
			printf("Aggregator send data to subscriber aggregator queue. Data will be updated. \n");
			SafeQueueInsert (_agg->m_queues->m_opAggrQueue, dataAndContext);
			printf("Aggregator sent to operators queue . Data will be updated.\n");
	}
}

Aggregator* AggregatorCreate (Queues* _queues, size_t _threadsNum)
{
	Aggregator* aggregator;
	aggregator = (Aggregator*)malloc(sizeof(Aggregator));
	if (NULL == aggregator)
	{
		return NULL;
	}
	aggregator->m_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == aggregator->m_threads)
	{
		free (aggregator);
		return NULL;
	}
	aggregator->m_rep_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == aggregator->m_rep_threads)
	{
		free (aggregator->m_rep_threads);
		free (aggregator);
		return NULL;
	}
	aggregator->m_queues = _queues;
	aggregator->m_isAlive = 1;
	aggregator->m_numOfThreads = _threadsNum;
	return aggregator;
}

void AggregatorDestroy(Aggregator* _aggregator)
{
	int index;
	void* buff;
	for (index = 0; index < _aggregator->m_numOfThreads; ++index)
	{
		pthread_join(_aggregator->m_threads[index], &buff);
		pthread_join(_aggregator->m_rep_threads[index], &buff);
	}
	free (_aggregator);
}

AggStatus AggregatorRun (Aggregator* _aggregator)
{
	int index = 0;
	for (index = 0; index <_aggregator->m_numOfThreads; ++ index)
	{
		pthread_create (&_aggregator->m_threads[index], NULL, (void*(*)(void*))AggregatorFunction, _aggregator);
		sleep (2);
		pthread_create (&_aggregator->m_rep_threads[index], NULL, (void*(*)(void*))AggregatorRepFunction, _aggregator);
	}
	return AGGREGATOR_OK ;
}
