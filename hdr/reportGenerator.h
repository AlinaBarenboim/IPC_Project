#ifndef _REPORT_GENERATOR_H_
#define _REPORT_GENERATOR_H_
#include "intern.h"
#include "aggregator.h"
#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"

typedef enum repGenStatus {REPORT_GENERATOR_OK, REPORT_GENERATOR_ERR} repGenStatus;
typedef struct ReportGenerator ReportGenerator;

ReportGenerator* ReportGeneratorCreate (Queues* _queue, size_t _threadsNum);
repGenStatus ReportGeneratorRun (ReportGenerator* _reportGen);
void DestroyReportGenerator (ReportGenerator* _reportGen);

#endif
