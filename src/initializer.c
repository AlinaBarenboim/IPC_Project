#include <stdio.h>
#include "initializer.h"
#include "../../advance_c/SafeQueue/safeQ.h"
#define QUEUE_INITIAL_LENGTH 100
#define CONFIG_FILE_NAME "config.text"

initStatus InitializeSystem ()
{
	Dispatcher* dispatcher;
	FileWatcher* fileWatcher;
	DataWatcher* dataWatcher;
	DataManager* dataManager;
	DataReader* reader;
	Parser* parser;
	Aggregator* aggregator;
	SubAggregator* subaggregator;
	OperAggregator* opaggregator;
	ReportGenerator* repgen;
	SubReportGenerator* subrepgen;

	
	Queues RTQueues;
	
	RTQueues.m_dataWatcherQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_dataManagerQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_readerQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_parserQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_aggregatorQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_subAggrQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_opAggrQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_subRepQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_repGenQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_despatcherQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_aggWithDespQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_subAggWithAggForRepReqQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_dataWatcherWithDesQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_fileWatcherWithDesQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	RTQueues.m_opAggWithAggForRepReqQueue = SafeQueueCreate(QUEUE_INITIAL_LENGTH);
	
	dispatcher = DispatcherCreate(CONFIG_FILE_NAME,&RTQueues );
	fileWatcher = FileWatcherCreate(&RTQueues, 1 );
	dataWatcher = DataWatcherCreate(&RTQueues, 1);
	dataManager = DataManagerCreate(&RTQueues, 1);
	reader = DataReaderCreate (&RTQueues, 1);
	parser = ParserCreate(&RTQueues, 1);
	aggregator = AggregatorCreate(&RTQueues, 1);
	subaggregator = SubAggregatorCreate(&RTQueues, 1);
	opaggregator = OprAggregatorCreate(&RTQueues, 1);
	repgen = ReportGeneratorCreate (&RTQueues, 1);
	subrepgen = SubReportGeneratorCreate (&RTQueues, 1);

	DispatcherRun(dispatcher);
	FileWatcherRun(fileWatcher);
	DataWatcherRun(dataWatcher);
	DataManagerRun(dataManager);
	DataReaderRun(reader);
	ParserRun(parser);
	AggregatorRun(aggregator);
	SubAggregatorRun(subaggregator);
	ReportGeneratorRun (repgen);
	SubReportGeneratorRun (subrepgen);
	OprAggregatorRun(opaggregator);


	DispatcherDestroy(dispatcher);
/*	DataWatcherDestroy(dataWatcher);*/
	
	return INITIALIZER_OK;
}

	

