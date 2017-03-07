#ifndef _OPER_AGGREGATOR_H_
#define _OPER_AGGREGATOR_H_
#include "aggregator.h"
#include <stddef.h>

typedef enum oprAggStatus {OPER_AGGREGATOR_OK, OPER_AGGREGATOR_ERR} oprAggStatus;
typedef struct OperAggregator OperAggregator;

typedef struct OpAggData
{
	char* m_BrandName;
	int m_IMSI;
	int m_InCallsDuration;
	int m_OutCallsDuration;
	int m_SMS_in;
	int m_SMS_out;	
}OpAggData;

//• Operator	MCC/MNC		
//• Operator	Brand	name	
//• Incoming	voice	call	durations	
//• Outgoing	voice	call	durations	
//• Incoming	SMS	messages		
//• Outgoing	SMS	Messages	

OperAggregator* OprAggregatorCreate (Queues* _queues, size_t _threadsNum);
oprAggStatus OprAggregatorRun (OperAggregator* _operAgg);
void DestroyOperAggregator (OperAggregator* _operAgg);

#endif
