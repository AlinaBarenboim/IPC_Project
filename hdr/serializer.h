#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "aggregator.h"
#include <stddef.h>

typedef enum serializerStatus {SERIALIZER_OK, SERIALIZER_ERR} serializerStatus;
typedef struct Serializer Serializer;


Serializer* SerializerCreate (SafeQueue* _queue, size_t _threadsNum);
serializerStatus SerializerRun (Serializer* _serializer);
void DestroySerializer (Serializer* _serializer);

#endif
