#ifndef _INITIALIZER_H_
#define _INITIALIZER_H_
#include "reportGenerator.h"
#include "dataManager.h"
#include "dataReader.h"
#include "dataWatcher.h"
#include "parser.h"
#include "fileWatcher.h"
#include "aggregator.h"
#include "subscriberAggregator.h"
#include "operatorAggregator.h"
#include "serializer.h"
#include "subscriberReport.h"
#include "operatorReport.h"
#include "despatcher.h"
#include "intern.h"
#include <stddef.h>


typedef enum initStatus {INITIALIZER_OK, INITIALIZER_ERR} initStatus;
typedef struct Initializer Initializer;
initStatus InitializeSystem ();



#endif

