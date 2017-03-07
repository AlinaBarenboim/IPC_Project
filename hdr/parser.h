#ifndef _PARSER_H_
#define _PARSER_H_
#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"
#include "intern.h"

typedef enum ParserStatus {PARSER_OK, PARSER_ERR} ParserStatus;
typedef struct Parser Parser;
/*typedef struct FullData FullData;*/

typedef enum ParserSegments {IMSI,MSISDN,SUB_IMEI,OP_BRAND_NAME, OP_MCC_MNC_TUP,CALL_TYPE,CALL_DATE, CALL_TIME, DURATION, DOWNLOAD, UPLOAD, PARTY_MSISDN, PARTY_OPERATOR} ParserSegments;

Parser* ParserCreate (Queues* _queues, size_t _threadsNum);
ParserStatus ParserRun (Parser* _parser);
void DestroyParser (Parser* _parser);

#endif


