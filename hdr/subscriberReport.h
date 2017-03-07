#ifndef _SUB_REPORT_GENERATOR_H_
#define _SUB_REPORT_GENERATOR_H_

#include "reportGenerator.h"
#include <stddef.h>

typedef enum subRepGenStatus {SUB_REPORT_GENERATOR_OK, SUB_REPORT_GENERATOR_ERR} subRepGenStatus;
typedef struct SubReportGenerator SubReportGenerator;

SubReportGenerator* SubReportGeneratorCreate (Queues* _queue, size_t _threadsNum);
subRepGenStatus SubReportGeneratorRun (SubReportGenerator* _subReportGenerator);
void DestroySubReportGenerator (SubReportGenerator* _subReportGenerator);


#endif
