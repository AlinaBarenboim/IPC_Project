#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"
#include "intern.h"
#include "subscriberReport.h"
#define LENGHT 124
#include <time.h>
#define  FILE_PATH "./Output/"
#include "../../advance_c/HashMap/HashMap.h"
#include "intern.h"
#include "dataManager.h"
#include <time.h>
#include <unistd.h> 
#include <fcntl.h> 
#include "subscriberAggregator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
struct SubReportGenerator 
{
	Queues* m_queues;
	pthread_t* m_threads;
	int m_isAlive;
	size_t m_numOfThreads;
	char*  m_path ;
	
};

static void CreateReport ( SubAggData* _report, FILE* _fp)
{
/*	char* subName;*/
/*	struct tm* timeInfo;*/
/*      time_t rawTime = time(0);*/
/*      rawTime = time(NULL);*/
/*      timeInfo = localtime(&rawTime);*/
/*	subName = _report->m_MSISDN;*/
	fprintf (_fp, "Successfully created Subscruber report for %s \n", _report->m_MSISDN);
/*	fprintf (_fp," %d.%m.%y %H:%M:%S", asctime (timeInfo) ) FIXME;*/
	fprintf(_fp, "Outgoing calls within operator: %d\n", _report->m_OutCallsWithMobOper);
	fprintf(_fp, "Incoming calls within operator: %d\n", _report->m_InCallsWithMobOper);
	fprintf(_fp, "Outgoing calls outside operator: %d\n", _report->m_OutCallsOutOper);
	fprintf(_fp, "Incoming calls outside operator: %d\n", _report->m_OutCallsWithMobOper);
	fprintf(_fp, "Outgoing SMS within operator: %d\n", _report->m_SMS_sentWithOper);
	fprintf(_fp, "Incoming SMS within operator: %d\n", _report->m_SMS_receivedWithOper);
	fprintf(_fp, "Outgoing SMS outside operator: %d\n", _report->m_SMS_sentOutOper);
	fprintf(_fp, "Incoming SMS outside operator: %d\n", _report->m_SMS_receivedOutOper);
	fprintf(_fp, "MB Downloaded: %d\n", _report->m_MB_Download);
	fprintf(_fp, "MB Uploaded: %d\n", _report->m_MB_Download);
	fclose(_fp);

}


    
static void SubReportGeneratorFunction (SubReportGenerator* _reportGen)
{
	FILE* fp;
	char fileName[LENGHT];
	SubAggData* data;
	while (_reportGen->m_isAlive)
	{

		SafeQueueRemove (_reportGen->m_queues->m_subRepQueue, (void**)&data);
		strcpy (fileName, _reportGen->m_path);
		strcat (fileName, "/");
		strcat (fileName, data->m_MSISDN);
		strcat (fileName, "_report");
		strcat(fileName, ".txt");
		printf("File Name: %s\n", fileName);
		fp = fopen(fileName, "w+");
		CreateReport(data, fp);
	}
}

SubReportGenerator* SubReportGeneratorCreate (Queues* _queue, size_t _threadsNum)
{
	SubReportGenerator* reportGen;
	reportGen = (SubReportGenerator*)malloc(sizeof(SubReportGenerator));
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
	reportGen->m_path = (char*)malloc(LENGHT *sizeof(char));
	reportGen->m_path = FILE_PATH;
	reportGen->m_queues = _queue;
	reportGen->m_isAlive = 1;
	reportGen->m_numOfThreads = _threadsNum;
	return reportGen;
}


subRepGenStatus SubReportGeneratorRun (SubReportGenerator* _subReportGenerator)
{
	int index = 0;
	for (index = 0; index <_subReportGenerator->m_numOfThreads; ++ index)
	{
		pthread_create (&_subReportGenerator->m_threads[index], NULL, (void*(*)(void*))SubReportGeneratorFunction,_subReportGenerator);
	}
	return SUB_REPORT_GENERATOR_OK;
}



void DestroySubReportGenerator (SubReportGenerator* _subReportGenerator)
{
	int index;
	void* buff;
	for (index = 0; index < _subReportGenerator->m_numOfThreads; ++index)
	{
		pthread_join(_subReportGenerator->m_threads[index], &buff);
	}
	free (_subReportGenerator);
}
