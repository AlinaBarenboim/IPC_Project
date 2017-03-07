#include "reportGenerator.h"
#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"
#include "intern.h"
#include "reportGenerator.h"
#include "../../advance_c/HashMap/HashMap.h"
#include "intern.h"
#include "dataManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
struct ReportGenerator 
{
	Queues* m_queues;
	pthread_t* m_threads;
	int m_isAlive;
	size_t m_numOfThreads;	
};


static void ReportGeneratorFunction (ReportGenerator* _reportGen)
{
	SubData* data;
	while (_reportGen->m_isAlive)
	{
		SafeQueueRemove (_reportGen->m_queues->m_repGenQueue, (void**)&data); /*FIXME */
/*		printf("Report generator sends  data to sab rep queue\n");*/
		SafeQueueInsert(_reportGen->m_queues->m_subRepQueue, data);
	}		
}



		
ReportGenerator* ReportGeneratorCreate (Queues* _queue, size_t _threadsNum)
{
	ReportGenerator* reportGen;
	reportGen = (ReportGenerator*)malloc(sizeof(ReportGenerator));
	if (NULL == reportGen)
	{
		return NULL;
	}
	reportGen->m_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == reportGen->m_threads)
	{
		free (reportGen);
		return NULL;
	}
	reportGen->m_queues = _queue;
	reportGen->m_isAlive = 1;
	reportGen->m_numOfThreads = _threadsNum;
	return reportGen;
}


void DestroyReportGenerator (ReportGenerator* _reportGen)
{

	int index;
	void* buff;
	for (index = 0; index < _reportGen->m_numOfThreads; ++index)
	{
		pthread_join(_reportGen->m_threads[index], &buff);
	}
	free (_reportGen);
	
}

repGenStatus ReportGeneratorRun (ReportGenerator* _reportGen)
{
	int index = 0;
/*	printf ("in report generator rub function!!!! \n");*/
	for (index = 0; index <_reportGen->m_numOfThreads; ++ index)
	{
		pthread_create (&_reportGen->m_threads[index], NULL, (void*(*)(void*))ReportGeneratorFunction, _reportGen);
	}
	return REPORT_GENERATOR_OK ;
}


