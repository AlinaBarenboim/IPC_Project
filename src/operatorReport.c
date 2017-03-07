#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"
#include "intern.h"
#include "operatorReport.h"
#define LENGHT 124
#include <time.h>
#define  FILE_PATH "./Output/"
#include "../../advance_c/HashMap/HashMap.h"
#include "intern.h"
#include "dataManager.h"
#include <time.h>
#include <unistd.h> /* open, write */
#include <fcntl.h> /* open */
#include "operatorAggregator.h"
#include <stdlib.h>
#include <stdio.h>

struct OperReport 
{
	Queues* m_queues;
	pthread_t* m_threads;
	int m_isAlive;
	size_t m_numOfThreads;
	char*  m_path ;
	
};


static void CreateReport ( OpAggData* _report, FILE* _fp)
{
	char* opName;
	struct tm* timeInfo;
        time_t rawTime = time(0);
        rawTime = time(NULL);
        timeInfo = localtime(&rawTime);
	opName = _report->m_BrandName;
	fprintf (_fp, "Successfully created Operator report for %s \n", opName);
/*	fprintf (_fp," %d.%m.%y %H:%M:%S", asctime (timeInfo) ); FIXME*/
	fprintf(_fp, "Outgoing calls duration: %d\n", _report->m_OutCallsDuration);
	fprintf(_fp, "Incoming calls duration: %d\n", _report->m_InCallsDuration);
	fprintf(_fp, "Outgoing SMS : %d\n", _report->m_SMS_out);
	fprintf(_fp, "Incoming SMS : %d\n", _report->m_SMS_in);
	fprintf(_fp, "MCC/MNC: %d\n", _report->m_IMSI);

}


    
static void OpReportGeneratorFunction (OperReport* _reportGen)
{
	OpAggData* report;
	FILE* fp;
	char fileName[LENGHT];
	while (_reportGen->m_isAlive)
	{
		report = (OpAggData*)malloc(sizeof(OpAggData));
		SafeQueueRemove (_reportGen->m_queues->m_opRepQueue, (void**)&report);
		strcpy (fileName, _reportGen->m_path);
		strcat (fileName, "/");
		strcat (fileName, report->m_BrandName);
		strcat (fileName, "_report");
		strcat(fileName, ".txt");
		printf("File Name: %s\n", fileName);
		fp = fopen(fileName, "w+");
		CreateReport(report, fp);
		fclose(fp);	
	}
}

OperReport* OprReportGeneratorCreate (SafeQueue* _queue, size_t _threadsNum)
{
	OperReport* reportGen;
	reportGen = (OperReport*)malloc(sizeof(OperReport));
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


oprRepGenStatus OprReportGeneratorRun (OperReport* _operRep)
{
	int index = 0;
	for (index = 0; index < _operRep->m_numOfThreads; ++ index)
	{
		pthread_create (&_operRep->m_threads[index], NULL, (void*(*)(void*))OpReportGeneratorFunction,_operRep);
	}
	return OPR_REPORT_GENERATOR_OK;
}



void DestroyOperReportGenerator (OperReport* _operRep)
{
	int index;
	void* buff;
	for (index = 0; index < _operRep->m_numOfThreads; ++index)
	{
		pthread_join(_operRep->m_threads[index], &buff);
	}
	free (_operRep);
}
