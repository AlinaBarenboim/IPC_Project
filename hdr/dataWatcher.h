#ifndef _DATA_WATCHER_H_
#define _DATA_WATCHER_H_
#include "dataManager.h"
#include <stddef.h>

typedef enum dWStatus {DATA_WATCHER_OK, DATA_WATCHER_ERR} dWStatus;
typedef struct DataWatcher DataWatcher;

DataWatcher* DataWatcherCreate (Queues* _queues, size_t _threadsNum);
dWStatus DataWatcherRun (DataWatcher* _dataWatcher);
void DestroyDataWatcher (DataWatcher* _dataWatcher);

#endif


