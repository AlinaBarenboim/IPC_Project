#ifndef _AGGREGATOR_H_
#define _AGGREGATOR_H_
#include "intern.h"
#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"


typedef enum AggStatus {AGGREGATOR_OK, AGGREGATOR_ERR} AggStatus;
typedef struct Aggregator Aggregator;

Aggregator* AggregatorCreate (Queues* _queues, size_t _threadsNum);
AggStatus AggregatorRun (Aggregator* _aggregator);
void AggregatorDestroy(Aggregator* _aggregator);

#endif
