#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileWatcher.h"
#include "dataWatcher.h"
#include <stddef.h>
#include "/home/alina/alina/advance_c/SafeQueue/safeQ.h"
#include "/home/alina/alina/advance_c/Queue/queue.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>


int main()
{
	FileWatcherRun (/*FileWatcher* _fileWatcher*/);
}
