#include <stdio.h>
#include "intern.h"
#include "dataReader.h"
#include "../../advance_c/SafeQueue/safeQ.h"
#include <stdlib.h>
#define MAX_STRING_LEN 256
#include <pthread.h>

struct DataReader
{
	Queues* m_queues;
	pthread_t* m_threads;
	int m_isAlive;
	size_t m_numOfThreads;	
};


static void SendAllStringsToParser (DataAndContext* _dataAndContext, DataReader* _reader)
{

/*	printf ("just entered into send all strings to parser function \n");*/
	char* stringToParse;
	int res;
	stringToParse = (char*)malloc(MAX_STRING_LEN*sizeof(char));
	if (NULL == stringToParse)
	{
		printf ("stringToParse is null \n");
		return;
	}
	while (NULL !=fgets (stringToParse, MAX_STRING_LEN -1,_dataAndContext->m_data ))
	{
		printf("%s\n", stringToParse);
		SafeQueueInsert (_reader->m_queues->m_parserQueue, stringToParse);
/*		printf("string just inserted onto parser queue \n");*/
		stringToParse = (char*)malloc(MAX_STRING_LEN*sizeof(char));
	}
	free (stringToParse);
}

static void ReaderFunction (DataReader* _reader)
{
	DataAndContext* dataAndContext;
	while (_reader->m_isAlive)
	{
/*		printf ("in reader function \n");*/
		dataAndContext = (DataAndContext*)malloc(sizeof(DataAndContext));
		SafeQueueRemove(_reader->m_queues->m_readerQueue, (void**)&dataAndContext);
		if (dataAndContext->m_context == ROW_DATA)
		{
/*			printf ("now reader sends data to parser\n");*/
			SendAllStringsToParser(dataAndContext, _reader);
		}
	        else if (PROCESSED_DATA == dataAndContext->m_context)
		{
/*			printf ("now reader sends data to data manager\n");*/
			SafeQueueInsert(_reader->m_queues->m_dataManagerQueue, dataAndContext);
		}
	}
}

DataReader* DataReaderCreate (Queues* _queues, size_t _threadsNum)
{
	DataReader* reader;
	
	reader = (DataReader*)malloc(sizeof(DataReader));
	if (NULL == reader)
	{
		return NULL;
	}
	reader->m_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == reader->m_threads)
	{
		free (reader);
		return NULL;
	}
	reader->m_queues = _queues;
	reader->m_isAlive = 1;
	reader->m_numOfThreads = _threadsNum;
	return reader;
	
}

dRStatus DataReaderRun (DataReader* _dataReader)
{
	int index = 0;
	void* buff;
	for (index = 0; index <_dataReader->m_numOfThreads; index ++)
	{
		pthread_create (&_dataReader->m_threads[index], NULL, (void*(*)(void*))ReaderFunction, _dataReader);
	}
	return DATA_READER_OK;
}

void DestroyDataReader (DataReader* _dataReader)
{
	int index;
	void* buff;
	for (index = 0; index < _dataReader->m_numOfThreads; ++index)
	{
		pthread_join(_dataReader->m_threads[index], &buff);
	}
	free (_dataReader);
}

