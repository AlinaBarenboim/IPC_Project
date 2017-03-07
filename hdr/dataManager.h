#ifndef __DATAMANAGER_H__
#define __DATAMANAGER_H__
#include "dataReader.h"
#include "dataManager.h"
#include "../../advance_c/SafeQueue/safeQ.h"
#include <stddef.h>

typedef enum DataManagerStatus {DM_OK, DM_ERR} DataManagerStatus;
typedef struct ProcessedData_t ProcessedData_t;
typedef struct DataManager DataManager;

typedef struct ParsedData
{
	char* m_IMSI;
	char* m_MSISDN;
	char* m_SubscriberIMEI;
	char* m_OperatorBrandName;
	char* m_OperatorMCC_MNCtuple;
	char* m_CallType;
	char* m_CallDate;
	char* m_CallTime;
	char* m_Duration;
	char* m_Download;
	char* m_Upload;
	char* m_PartyMSISDN;
	char* m_PartyOperator;
}ParsedData;


typedef struct SubData
{
	char* m_MSISDN;
	int m_Duration;
	int m_Download;
	int m_Upload;
	int m_isCall;
	int m_isSMS;
	int m_isGPRS;
	int m_isIncomming;
	int m_isCrossOp;
}SubData;

typedef struct OperData
{
/*	char* m_OperatorBrandName;*/
/*	int m_iscomingDuration;*/
/*	int m_OutgoingDuration;*/
/*	int m_incomingSMS;*/
/*	int m_OutgoingSMS;*/
	int m_IMSI;
	char* m_OperatorBrandName;
	int m_isCall;
	int m_isSMS;
	int m_Duration;
	int m_isIncomming;
	
}OperData;

typedef struct ProcessedData
{
	OperData* m_OperData;
	SubData* m_SubData;
}ProcessedData;

/* OPERATOR DATA
• Incoming	voice	call	durations	
• Outgoing	voice	call	durations	
• Incoming	SMS	messages		
• Outgoing	SMS	Messages
*/	
DataManager* DataManagerCreate(Queues* _queues, size_t _threadsNum);

void DataManagerDestroy(DataManager* _dataManager);

DataManagerStatus DataManagerRun(DataManager* _dataManager);

#endif
