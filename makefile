#CDR

CFLAGS = -g -c #-ansi -pedantic -Wall -Werror -g -c
CC = gcc

OBJS = initializer.o ui2.o  despatcher.o rt.o intern.o fileWatcher.o dataWatcher.o    dataManager.o  dataReader.o parser.o aggregator.o reportGenerator.o subscriberReport.o subscriberAggregator.o operatorAggregator.o operatorReport.o

.PHONY: clean  
#subscriberAggregator.o 
#UI2

ui2 : ui2.o ui2_test.o
	$(CC) ui2_test.o ui2.o intern.o -o ui_test2 -L../../advance_c  -lpthread -lds
	
ui2_test.o : ui2_test.c ui2.h despatcher.h initializer.h intern.h 
	$(CC) $(CFLAGS) ui2_test.c 
	
ui2.o : ui2.c ui2.h ../../advance_c/ipcQueue/queue.h ../../advance_c/HashMap/HashMap.h intern.h
	$(CC) $(CFLAGS) ui2.c 


cdr : watchDog.o cdr.o
	$(CC) watchDog.o cdr.o intern.o -o cdr -L../../advance_c  -lds -lpthread

cdr.o : cdr.c watchDog.h
	$(CC) $(CFLAGS) cdr.c

watchdog.o : watchDog.c watchDog.h ../../advance_c/ipcQueue/queue.h intern.c intern.h 
	$(CC) $(CFLAGS) watchdog.c 
	
# RT	
	
rt : $(OBJS)
	$(CC) $(OBJS) -o rt -L../../advance_c  -lds -lpthread

rt.o : rt.c despatcher.h initializer.h 
	$(CC) $(CFLAGS) rt.c

despatcher.o : despatcher.c despatcher.h ../../advance_c/ipcQueue/queue.h  intern.c intern.h
	$(CC) $(CFLAGS) despatcher.c 

initializer.o : initializer.c initializer.h ../../advance_c/SafeQueue/safeQ.h intern.h
	$(CC) $(CFLAGS) initializer.c 
	
aggregator.o : aggregator.c aggregator.h ../../advance_c/SafeQueue/safeQ.h intern.h
	$(CC) $(CFLAGS) aggregator.c 	
	
subscriberAggregator.o : subscriberAggregator.c subscriberAggregator.h ../../advance_c/SafeQueue/safeQ.h intern.h  ../../advance_c/HashMap/HashMap.h
	$(CC) $(CFLAGS) subscriberAggregator.c 
	
operatorAggregator.o : operatorAggregator.c operatorAggregator.h ../../advance_c/SafeQueue/safeQ.h intern.h ../../advance_c/HashMap/HashMap.h
	$(CC) $(CFLAGS) operatorAggregator.c 
	
dataManager.o : dataManager.c dataManager.h intern.h
	$(CC) $(CFLAGS) dataManager.c 
	
parser.o : parser.c parser.h intern.h ../../advance_c/SafeQueue/safeQ.h dataReader.h dataManager.h
	$(CC) $(CFLAGS) parser.c 
	
dataReader.o : dataReader.c dataReader.h ../../advance_c/SafeQueue/safeQ.h intern.h
	$(CC) $(CFLAGS) dataReader.c 

fileWatcher.o : fileWatcher.c fileWatcher.h ../../advance_c/SafeQueue/safeQ.h intern.h 
	$(CC) $(CFLAGS) fileWatcher.c 
	
dataWatcher.o : dataWatcher.c dataWatcher.h ../../advance_c/SafeQueue/safeQ.h
	$(CC) $(CFLAGS) dataWatcher.c 
	
intern.o : intern.c intern.h 
	$(CC) $(CFLAGS) intern.c 
	
reportGenerator.o : reportGenerator.c reportGenerator.h dataManager.h ../../advance_c/SafeQueue/safeQ.h intern.h ../../advance_c/HashMap/HashMap.h
	$(CC) $(CFLAGS) reportGenerator.c 

subscriberReport.o : subscriberReport.c subscriberReport.h dataManager.h subscriberAggregator.h ../../advance_c/SafeQueue/safeQ.h intern.h ../../advance_c/HashMap/HashMap.h
	$(CC) $(CFLAGS) subscriberReport.c 

operatorReport.o : operatorReport.c operatorReport.h dataManager.h subscriberAggregator.h ../../advance_c/SafeQueue/safeQ.h intern.h ../../advance_c/HashMap/HashMap.h
	$(CC) $(CFLAGS) operatorReport.c 


clean:
	rm -f *.o
	
rebuild: clean 
