#ifndef _FILE_WATCHER_H_
#define _FILE_WATCHER_H_
#include "dataWatcher.h"
#include <stddef.h>

typedef enum fWStatus {FILE_WATCHER_OK, FILE_WATCHER_ERR} fWStatus;
typedef struct FileWatcher FileWatcher;

FileWatcher* FileWatcherCreate (Queues* queue, size_t _threadsNum);
fWStatus FileWatcherRun (FileWatcher* _fileWatcher);
void DestroyFileWatcher (FileWatcher* _fileWatcher);
void FileWatcherPause ();
void FileWatcherWakeUp ();
#endif
