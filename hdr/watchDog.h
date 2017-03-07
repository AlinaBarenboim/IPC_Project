#ifndef _WATCH_DOG_H_
#define _WATCH_DOG_H_

#include "despatcher.h"
#include "intern.h"
#include "../../advance_c/ipcQueue/queue.h"
typedef enum WatchDogStatus {WATCH_DOG_OK, WATCH_DOG_ERR} WatchDogStatus;
typedef struct WatchDog WatchDog ;


WatchDog* WatchDogCreate ();
void DestroyWatchDog (WatchDog* _watchDog);
WatchDogStatus WatchDogRun (WatchDog* _watchDog);





#endif
