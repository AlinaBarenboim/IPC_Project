#ifndef _INTERN_H_
#define _INTERN_H_
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "../../advance_c/HashMap/HashMap.h"
#include "../../advance_c/ipcQueue/queue.h"
#include "../../advance_c/SafeQueue/safeQ.h"
#define FIELD_SIZE 128

typedef enum Context {ROW_DATA = 1, PROCESSED_DATA, REQUEST_FOR_REPORT, READY_REPORT, DATA_TO_INSERT} Context;
typedef enum Data { OP_DATA, SUB_DATA}Data;
typedef enum RepData { ONE_SUB=1, MANY_SUBS, ONE_OPER, MANY_OPERS }RepData;
typedef enum CommandType {REP_ONE_SUB = 1, REP_MANY_SUBS, REP_OPER, REP_MANY_OPERS, PAUSE, RESUME, SHUT_DOWN}CommandType;


typedef struct Command
{
	unsigned int m_typeOfCommand : 8;
	unsigned int m_Name : 24;		
}Command;

typedef struct DataAndContext
{
	void* m_data;
	Context m_context;
	Data m_opORSub;
	RepData m_kindOfRep;
	int m_name;
}DataAndContext;


typedef struct Queues
{
	SafeQueue* m_dataWatcherQueue;
	SafeQueue* m_dataManagerQueue;
	SafeQueue* m_readerQueue;
	SafeQueue* m_parserQueue;
	SafeQueue* m_aggregatorQueue;
	SafeQueue* m_subAggrQueue;
	SafeQueue* m_opAggrQueue;
	SafeQueue* m_repGenQueue;
	SafeQueue* m_despatcherQueue;
	SafeQueue* m_opRepQueue;
	SafeQueue* m_subRepQueue;
	SafeQueue* m_aggWithDespQueue;
	SafeQueue* m_subAggWithAggForRepReqQueue;
	SafeQueue* m_dataWatcherWithDesQueue;
	SafeQueue* m_fileWatcherWithDesQueue;
	SafeQueue* m_opAggWithAggForRepReqQueue;
		
} Queues;


IpcQueue* GetSharedIPCQueue(int _shmId);
int GetShmId( const char* _projId, const char* _fileName);
/*int GetShmId(HashMap* _shMemoryConfig, const char* _projId, const char* _fileName);*/
/*HashMap* GetSharedMemoryConfig(Config* config, const char* _key);*/

#endif
