#ifndef _SUB_AGGREGATOR_H_
#define _SUB_AGGREGATOR_H_
#include "aggregator.h"
#include <stddef.h>


typedef enum subAggStatus {SUB_AGGREGATOR_OK, SUB_AGGREGATOR_ERR} subAggStatus;
typedef struct SubAggregator SubAggregator;

typedef struct SubAggData
{
	char* m_MSISDN;
	int m_OutCallsWithMobOper;
	int m_InCallsWithMobOper;
	int m_OutCallsOutOper;
	int m_InCallsOutOper;
	int m_SMS_sentWithOper;
	int m_SMS_receivedWithOper;
	int m_SMS_sentOutOper;
	int m_SMS_receivedOutOper;
	int m_MB_Download;
	int m_MB_Upload;	
}SubAggData;

SubAggregator* SubAggregatorCreate (Queues* _queues, size_t _threadsNum);
subAggStatus SubAggregatorRun (SubAggregator* _subAgg);
void DestroySubAggregator (SubAggregator* _subAgg);

#endif

/*
• Outgoing	voice	calls	duration	to	a	subscriber	within	the	mobile	operator	
• Incoming	voice	calls	duration	to	a	subscriber	within	the	mobile	operator	
• Outgoing	voice	calls	duration	to	a	subscriber	outside	the	mobile	operator	
• Incoming	voice	calls	duration	to	a	subscriber	outside	the	mobile	operator	
• SMS	messages	sent	within	the	mobile	operator	
• SMS	messages	received	within	the	mobile	operator	
• SMS	messages	sent	outside	the	mobile	operator	
• SMS	messages	received	outside	the	mobile	operator	
• MB	downloaded	
• MB	uploaded
*/
