#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileWatcher.h"
#include "dataWatcher.h"
#include <stddef.h>
#include "../../advance_c/SafeQueue/safeQ.h"
#include "../../advance_c/Queue/queue.h"
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))
#define MAX_FILE_LEN 1024
#define MAX_FILE_PATH_LENGTH 256
#define MAX_FILE_MASK_LENGTH 16
#include <dirent.h>
#include <pthread.h> 
#include <errno.h>

static int isPause = 0;


struct FileWatcher
{
	pthread_t* m_threads;
	pthread_t* m_waitForPauseThread;
	pthread_cond_t m_cv;
	size_t m_numOfThreads;
	Queues* m_queues;
	int m_isAlive;
	pthread_mutex_t m_mutex;
	char m_filePath[MAX_FILE_PATH_LENGTH];
	char m_fileMask[MAX_FILE_MASK_LENGTH];
	DIR* m_dirp;

};

static void FileWatcherForpauseFunction (FileWatcher* _fileWatcher)
{
	Command* command;
	while (_fileWatcher->m_isAlive)
	{
		SafeQueueRemove(_fileWatcher->m_queues->m_fileWatcherWithDesQueue, (void**) &command);
		if (command->m_typeOfCommand == PAUSE)
		{	
			printf ("File Watcher got pause command \n");
			pthread_mutex_lock(&_fileWatcher->m_mutex);
			while  (isPause == 1)
			{
				pthread_cond_wait(&_fileWatcher->m_cv, &_fileWatcher->m_mutex);
			}
			printf ("File Watcher woke up \n");
			pthread_cond_broadcast(&_fileWatcher->m_cv);
			pthread_mutex_unlock(&_fileWatcher->m_mutex);
		}
	}
}

void FileWatcherWakeUp ()
{

	isPause = 0;
	
}
void FileWatcherPause ()
{

	isPause = 1;
	
}
static void FileWatcherFunction (FileWatcher* _fileWatcher, char* _fileName)
{
	int length;
	int i = 0;
        int fd;
	int wd;
	char buffer[BUF_LEN];
	DataAndContext* dataAndContext;
	FILE* fp;
	char fileName[MAX_FILE_LEN];
	struct inotify_event* event;
  	fd = inotify_init();
  	if ( fd < 0 )
  	{
   	 	  perror( "inotify_init" );
        }
  	printf ("fd: %d \n", fd);
  	wd = inotify_add_watch( fd, _fileWatcher->m_filePath, IN_CREATE |  IN_MOVED_TO);
  	if ( wd < 0 )
        {
   		 perror( "inotify_init" );
	}
/*  	printf ("wd: %d \n", wd);*/
/*  	printf ("%s\n",_fileWatcher->m_filePath );*/
  	while (_fileWatcher->m_isAlive)
  	{
  		if(isPause != 1)
  		{
/*  		{	pthread_mutex_lock(&_fileWatcher->m_mutex);*/
/*			while  (isPause == 1)*/
/*			{*/
/*				pthread_cond_wait(&_fileWatcher->m_cv, &_fileWatcher->m_mutex);*/
/*			}*/
/*			pthread_cond_broadcast(&_fileWatcher->m_cv);*/

			
	  		printf ("in file watcher function - not pause\n");
		  	length = read( fd, buffer, BUF_LEN ); 
		  	while ( i < length ) 
		  	{
		  		event = ( struct inotify_event * ) &buffer[ i ]; 
		  		if ( event->len /* && event->mask & IN_CREATE*/ )
		  		{
		  			dataAndContext = (DataAndContext*)malloc(sizeof(DataAndContext));
					dataAndContext->m_context = ROW_DATA;
		  			strcpy (fileName, _fileWatcher->m_filePath);
		  			strcat (fileName, "/");
		  			strcat (fileName, event->name);
	/*	  			printf(" File Name: %s\n", fileName);*/
		  			Context res = dataAndContext->m_context;
	/*	  			printf ("context: %d \n", res);	*/
					dataAndContext->m_data = fopen(fileName, "r");
					SafeQueueInsert(_fileWatcher->m_queues->m_dataWatcherQueue, dataAndContext);

		  		}
		  		i += EVENT_SIZE + event->len;
			   }
/*		 	pthread_mutex_unlock(&_fileWatcher->m_mutex);*/
		}
	  i = 0;

  	 } 
  	 inotify_rm_watch( fd, wd );
	 close( fd );	
}
	
fWStatus FileWatcherRun (FileWatcher* _fileWatcher)
{
	int index;
	for(index = 0; index < _fileWatcher->m_numOfThreads; ++index)
	{
		pthread_create(&_fileWatcher->m_waitForPauseThread[index], NULL, (void*(*)(void*))FileWatcherForpauseFunction, _fileWatcher);
		sleep(2);
		pthread_create(&_fileWatcher->m_threads[index], NULL, (void*(*)(void*))FileWatcherFunction, _fileWatcher);

	}
/*	sleep(5);*/
	return FILE_WATCHER_OK;	
}

void FileWatcherDestroy(FileWatcher* _fileWatcher)
{
	void* buff;
	int index;
	_fileWatcher->m_isAlive = 0;
	pthread_cond_destroy(&_fileWatcher->m_cv);
	for (index = 0; index < _fileWatcher->m_numOfThreads; ++index)
	{
		pthread_join (_fileWatcher->m_threads[index], &buff);
		pthread_join (_fileWatcher->m_waitForPauseThread[index], &buff);
	}
	closedir(_fileWatcher->m_dirp);
	free(_fileWatcher);
}

FileWatcher* FileWatcherCreate(Queues* _queue, size_t _threadsNum)
{
	FileWatcher* fileWatcher;
	fileWatcher = (FileWatcher*) malloc(sizeof(FileWatcher));

	if(NULL == fileWatcher)
	{
		return NULL;
	}
	
	strcpy(fileWatcher->m_filePath, "./Input");
	strcpy(fileWatcher->m_fileMask, "*.cdr");	
	fileWatcher->m_numOfThreads = _threadsNum;	
	
	fileWatcher->m_threads = (pthread_t*) malloc(_threadsNum * sizeof(pthread_t));
	if(NULL == fileWatcher->m_threads)
	{
		free (fileWatcher);
		return NULL;
	}
	fileWatcher->m_waitForPauseThread = (pthread_t*) malloc(_threadsNum * sizeof(pthread_t));
	if(NULL == fileWatcher->m_waitForPauseThread)
	{
		free (fileWatcher->m_threads);
		free (fileWatcher);
		return NULL;
	}
	fileWatcher->m_isAlive = 1;
	fileWatcher->m_queues = _queue;
	pthread_mutex_init(&fileWatcher->m_mutex, NULL);/*TODO check if ok!*/
	pthread_cond_init(&fileWatcher->m_cv,NULL);
	fileWatcher->m_dirp = opendir(fileWatcher->m_filePath);	
	return fileWatcher;
}
	

