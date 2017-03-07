#ifndef _DESPATCHER_H_
#define _DESPATCHER_H_
#include "dataWatcher.h"
#include "serializer.h"
#include "reportGenerator.h"


typedef enum DispStatus {DESPATCHER_OK, DESPATCHER_ERR} DispStatus;
typedef struct Dispatcher Dispatcher;
typedef enum OperNames {CELLCOM = 1, ORANGE, PELEPHONE,VODAFONE, PARTNER}OperNames;


DispStatus DispatcherRun(Dispatcher* _dispatcher);
void DispatcherDestroy(Dispatcher* _dispatcher);
Dispatcher* DispatcherCreate(const char* _configFile, Queues* _queue);

#endif
