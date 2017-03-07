#ifndef _DATA_READER_H_
#define _DATA_READER_H_
#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"
#include "intern.h"

typedef enum dRStatus {DATA_READER_OK, DATA_READER_ERR} dRStatus;
typedef struct DataReader DataReader;

DataReader* DataReaderCreate (Queues* _queues, size_t _threadsNum);
dRStatus DataReaderRun (DataReader* _dataReader);
void DestroyDataReader (DataReader* _dataReader);

#endif
