#include "intern.h"
#include "ui2.h"
#include <stdio.h>
#include "despatcher.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "../../advance_c/ipcQueue/queue.h"
#include <stdlib.h>
#include <unistd.h>
#include "unistd.h"
#include "watchDog.h"
#define PING 1
#define PROJ_ID "B"
#define FILE "file2"
#define ALIVE 1
#include <string.h>

struct UI
{
	IpcQueue* m_queue;
	pthread_t m_despatcherThread;
	int m_isAlive;
	int m_shmId;	
};


static void SendCommand (Command _command, UI* _ui)
{
	int msg;
	msg = *(((int*)&_command));
	IpcQueueInsert (_ui->m_queue, msg);	
}

static void GetCommand (UI* _userInterface)
{
	int msg;
	char name [128];
	Command command;
	printf ("Please insert your command: \n 1 : One Subscriber Report \n 2 : Many Subscribers Report \n 3 : One Operator Report \n 4 : Many Operators Report \n 5 : Pause \n 6 : Resume \n 7 : ShutDown \n" );
	scanf ("%d", &msg);
	if (msg == REP_ONE_SUB)
	{
		printf ("Please enter  subscriber name : \n"   );
		scanf ("%s", name);
		command.m_typeOfCommand = msg;
		command.m_Name = atoi(name);
		printf ("IN USER INTERFACE: name : %d \n", command.m_Name);
	} 
	else if (msg == REP_OPER)
	{
		printf ("Please enter operator  name : \n 1. Cellcom \n 2. Orange \n 3. Pelephone \n 4. Partner \n 5. Vodafone \n");
		scanf ("%s", name);
		command.m_typeOfCommand = msg;
		command.m_Name = atoi(name);
		printf ("IN USER INTERFACE: name : %d \n", command.m_Name);
		
/*		if  (command.m_Name == 1)*/
/*			{*/
/*				command.m_Name = CELLCOM;*/
/*			}*/

/*		if (command.m_Name == 2)*/
/*			{*/
/*				command.m_Name = ORANGE;*/
/*			}*/
/*		if (command.m_Name == 3)*/
/*			{*/
/*				command.m_Name = PELEPHONE;*/
/*			}*/
/*		if (command.m_Name == 4)*/
/*			{*/
/*				command.m_Name =  PARTNER;*/
/*			}*/

/*		else*/
/*			{*/
/*				command.m_Name = VODAFONE;*/
/*			}	*/
/*			*/
	}
/*	else if (msg == REP_MANY_SUBS ||  msg == REP_MANY_OPERS)*/
/*	{*/
/*		printf ("Report will be created soon \n");*/
/*		command.m_typeOfCommand = msg;*/
/*		command.m_Name = 0;*/
/*	}*/
	else
	{
		command.m_typeOfCommand = msg;
		command.m_Name = 0;
	}
	printf (" command: %d \n", command.m_typeOfCommand);
	printf (" name: %d \n", command.m_Name);	
	SendCommand (command, _userInterface);
	IpcQueuePrint(_userInterface->m_queue);
}

static void UserInterfaceFunction(UI* _userInterface)
{
	while(_userInterface->m_isAlive)
	{
		printf ("User will get new command  now \n");
		GetCommand (_userInterface);
	}
}


UI* UserInterfaceCreate (const char* _configFile)
{
	UI* userInterface;
	IpcQueue* queue;
	char* projId;
	char* fileName;
	userInterface = (UI*) malloc(sizeof(UI));
	
	if(NULL == userInterface)
	{
		return NULL;
	}
	projId = PROJ_ID;
	fileName = FILE;
	userInterface->m_shmId = GetShmId(projId, fileName);
	queue = GetSharedIPCQueue(userInterface->m_shmId);
	
	if(NULL == queue)
	{
		free(userInterface);
		return NULL;
	}
	userInterface->m_queue = queue;
	userInterface->m_isAlive = ALIVE;	
	return userInterface;
}

UserIntStatus UserRun (UI* _userInterface)
{
	void* buff;
	pthread_create(&_userInterface->m_despatcherThread, NULL, (void*(*)(void*))UserInterfaceFunction, _userInterface);
	pthread_join(_userInterface->m_despatcherThread, &buff);
	return USER_INTERFACE_OK;	
}

