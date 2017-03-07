#include <stdio.h>
#define LINE_LENGHT 256
#include <unistd.h>
#define LENGHT 128
#include <string.h>
void CreateLine (char* _cdrLine)
{
	int num;
	char word[32];
	
	sprintf(_cdrLine, "%d|%d|%d|", rand()%100,rand(),rand());
	
	num = rand()%5;
	switch(num)
	{
		case 0: strcat(_cdrLine, "Cellcom");
		break;
		case 1: strcat(_cdrLine, "Orange");
		break;
		case 2:strcat(_cdrLine, "Pelephone");
		break;
		case 3:strcat(_cdrLine, "Golan");
		break;
		case 4:strcat(_cdrLine, "Vodafone");
		break;
		case 5:strcat(_cdrLine, "Partner");
		break;
	}
	
	strcat(_cdrLine, "|");
	sprintf(word, "%d",rand()%100000);
	strcat(_cdrLine,word);
	strcat(_cdrLine,"|");
	
	num = rand()%3;
	switch(num)
	{
		case 0:
			strcat(_cdrLine, "SMS_MO");
			break;
		case 1:
			strcat(_cdrLine, "GPRS");
			break;
		case 2:
			strcat(_cdrLine, "MTC");
			break;
	}		
		
	strcat(_cdrLine, "|");
	strcat(_cdrLine, "01:01:1970");
		strcat(_cdrLine, "|");
				strcat(_cdrLine, "02:00:00");
			strcat(_cdrLine, "|");
			
	sprintf(word, "%d",rand()%100);
		strcat(_cdrLine,word);
			strcat(_cdrLine, "|");
		
			sprintf(word, "%d",rand()%100);
		strcat(_cdrLine,word);
			strcat(_cdrLine, "|");
			
				sprintf(word, "%d",rand()%100);
		strcat(_cdrLine,word);
			strcat(_cdrLine, "|");
			
				sprintf(word, "%d",rand()%100000);
		strcat(_cdrLine,word);
			strcat(_cdrLine, "|");	
			
				num = rand()%5;
	switch(num)
	{
		case 0: strcat(_cdrLine, "Cellcom");
		break;
		case 1: strcat(_cdrLine, "Orange");
		break;
		case 2:strcat(_cdrLine, "Pelephone");
		break;
		case 3:strcat(_cdrLine, "Golan");
		break;
		case 4:strcat(_cdrLine, "Vodafone");
		break;
		case 5:strcat(_cdrLine, "Partner");
		break;
	}
			
}

void GetFileName ( char* _fileName,  char* _path)
{

	static int num = 1;
	char word[32];
	
	sprintf(_fileName, "%s/", _path);
	sprintf (word, "%d", (char)num);
	
	strcat(_fileName,word);
	strcat(_fileName,".cdr");
	++num;
	
}

void CreateCdrFile (size_t _lines, size_t _interval, char* _path)
{
	char fileName [LENGHT];
	FILE* fp;
	char cdrLine [LINE_LENGHT];
	int i = 0;
	int j;
	while (i < 30)
	{
			sleep(2);
			GetFileName (fileName, _path);
			fp = fopen(fileName, "w+");
			for (j = 0; j<_lines;++j)
			{
						CreateLine(cdrLine);
						fprintf(fp,"%s\n", cdrLine);
			}
			
			fclose (fp);
			sleep(_interval);
			i++;
	}

}

int main ()
{
	CreateCdrFile (100, 1, "./Input/");
}



