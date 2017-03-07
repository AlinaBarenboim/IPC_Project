#include "watchDog.h"
#define FILE "config.text"

WatchDogStatus main()
{
	 WatchDog* watchDog;
	 watchDog = WatchDogCreate(FILE);
	 WatchDogRun (watchDog);
	 return 0;
}
