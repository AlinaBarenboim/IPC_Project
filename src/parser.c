#include "../../advance_c/SafeQueue/safeQ.h"
#include "intern.h"
#include <stdio.h>
#include "parser.h"
#include "dataReader.h"
#include <stdlib.h>
#include <string.h>
#include "dataManager.h"
#include <pthread.h>
#define STRING_LEN 64

struct Parser
{
	Queues* m_queues;
	pthread_t* m_threads;
	int m_isAlive;
	size_t m_numOfThreads;
};

static void ProcessData(ParsedData* _parsedData, ProcessedData** _processedData)
{
	SubData* subData;
	OperData* opData;
	ProcessedData* processedData;
	int isCross;
	subData = (SubData*)calloc(1, sizeof(SubData));
	opData = (OperData*)calloc(1, sizeof(SubData));
	
	processedData = (ProcessedData*) malloc(sizeof(ProcessedData));
	
	if(strcmp(_parsedData->m_OperatorBrandName, _parsedData->m_PartyOperator) != 0)
	{
		subData->m_isCrossOp = 1;
	}
	isCross = subData->m_isCrossOp;
	
	opData->m_OperatorBrandName = _parsedData->m_OperatorBrandName;
	subData->m_MSISDN = _parsedData->m_MSISDN;
	opData->m_IMSI =atoi (_parsedData->m_IMSI);
	
	if(strcmp(_parsedData->m_CallType, "MOC") == 0)
	{
		subData->m_isCall = 1;
		subData->m_Duration = atoi(_parsedData->m_Duration);
		if (isCross == 1)
		{
			opData->m_isCall = 1;
			opData->m_Duration = atoi(_parsedData->m_Duration);
		
		}
		
	}
	
	else if(strcmp(_parsedData->m_CallType, "MTC") == 0)
	{
		subData->m_isCall = 1;
		subData->m_isIncomming = 1;
		subData->m_Duration = atoi(_parsedData->m_Duration);
		if (isCross == 1)
		{
			opData->m_isCall = 1;
			opData->m_isIncomming = 1;
			opData->m_Duration = atoi(_parsedData->m_Duration);
		}
		
		
	}
	else if(strcmp(_parsedData->m_CallType, "SMS_MO") == 0)
	{
		subData->m_isSMS = 1;
		if (isCross == 1)
		{
			opData->m_isSMS = 1;
		}
		
	}
	else if(strcmp(_parsedData->m_CallType, "SMS_MT") == 0)
	{
		subData->m_isSMS = 1;
		subData->m_isIncomming = 1;
		if (isCross == 1)
		{
			opData->m_isSMS = 1;
			opData->m_isIncomming = 1;
		}
		
		
	}
	else if(strcmp(_parsedData->m_CallType, "GPRS") == 0)
	{
		subData->m_isGPRS = 1;
		subData->m_Upload = (strcmp(_parsedData->m_Upload, " ") != 0 ? atoi(_parsedData->m_Upload) : 0);
		subData->m_Download = (strcmp(_parsedData->m_Download, " ") != 0 ? atoi(_parsedData->m_Download) : 0);
	}

	printf("\nEntry:\nMSISDN: %s\nisCall: %d\nisSMS: %d\nisGPRS: %d\nisIncomming: %d\nisCrossOp: %d\nduration: %d\nuploaded: %d\ndownloaded: %d\n", subData->m_MSISDN, subData->m_isCall, subData->m_isSMS, subData->m_isGPRS, subData->m_isIncomming, subData->m_isCrossOp, subData->m_Duration, subData->m_Upload, subData->m_Download);

	processedData->m_SubData = subData;
	processedData->m_OperData = opData;
	*_processedData = processedData;
	return;
}
static void FillProcessedDataStructs (Parser* _parser, char**_prossData, char* _parsedString)
{
	char* string;
	char* tmpStr;
	char** dataInit;
	DataAndContext* dataAndContext;
	ProcessedData* processedData;
	dataInit = _prossData;	
	string = (char*) malloc(STRING_LEN * sizeof(char));
	tmpStr = strtok (_parsedString,"|");
	strcpy(string, tmpStr);
	*_prossData = string;
	++_prossData;
	string = (char*) malloc(STRING_LEN * sizeof(char));
	while((tmpStr = strtok (NULL,"|\n")) != NULL)
	{
		strcpy(string, tmpStr);
		*_prossData = string;
		++_prossData;
		printf (" string : %s\n", string);
		string = (char*) malloc(STRING_LEN * sizeof(char));
	}
	
	ProcessData((ParsedData*)dataInit, &processedData);	
	dataAndContext = (DataAndContext*) malloc(sizeof(DataAndContext));
	dataAndContext->m_data = processedData;
	dataAndContext->m_context = PROCESSED_DATA;
/*	dataAndContext->m_opORSub = SUB_DATA;*/

	SafeQueueInsert(_parser->m_queues->m_readerQueue, dataAndContext);

	free(_parsedString);
	free(string);
	return;
}

static void ParserFunction (Parser* _parser)
{
	char* parsedString;
	ParsedData* parsData; 
	while (_parser->m_isAlive)
	{
/*		printf ("now entered to parser function \n");*/
		SafeQueueRemove (_parser->m_queues->m_parserQueue, (void**)&parsedString);
		parsData = (ParsedData*)malloc(sizeof(ParsedData));
		FillProcessedDataStructs ( _parser, (char**)parsData, parsedString);
	}	
}

Parser* ParserCreate (Queues* _queues, size_t _threadsNum)
{
	Parser* parser;
	parser = (Parser*)malloc(sizeof(Parser));
	if (NULL == parser)
	{
		return NULL;
	}
	parser->m_threads =(pthread_t*) malloc(_threadsNum*sizeof(pthread_t));
	if (NULL == parser->m_threads)
	{
		free (parser);
		return NULL;
	}
	parser->m_queues = _queues;
	parser->m_isAlive = 1;
	parser->m_numOfThreads = _threadsNum;
	return parser;
}

void DestroyParser (Parser* _parser)
{
	int index;
	void* buff;
	for (index = 0; index < _parser->m_numOfThreads; ++index)
	{
		pthread_join(_parser->m_threads[index], &buff);
	}
	free (_parser);
}

ParserStatus ParserRun (Parser* _parser)
{
	int index = 0;
/*	printf ("in parser run function \n");*/
	for (index = 0; index <_parser->m_numOfThreads; index ++)
	{
		pthread_create (&_parser->m_threads[index], NULL, (void*(*)(void*))ParserFunction, _parser);
	}
	return PARSER_OK;
}
