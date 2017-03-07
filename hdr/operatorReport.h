#ifndef _OPR_REPORT_GENERATOR_H_
#define _OPR_REPORT_GENERATOR_H_

#include "reportGenerator.h"
#include <stddef.h>

typedef enum oprRepGenStatus {OPR_REPORT_GENERATOR_OK, OPR_REPORT_GENERATOR_ERR} oprRepGenStatus;
typedef struct OperReport OperReport;

OperReport* OprReportGeneratorCreate (SafeQueue* _queue, size_t _threadsNum);
oprRepGenStatus OprReportGeneratorRun (OperReport* _operRep);
void DestroyOperReportGenerator (OperReport* _operRep);

#endif

