/* PRTERR - print error messages*/
#include <misc.hpp>
#include <stdarg.h>
#include<errno.h>
#include <time.h>
#include <string.h>

char errspace[4096];

void prtflderr(char *file,int line,char *date)
{
	time_t ltime;
	time(&ltime);
	memset(errspace,0,4096);
	errspace[0]=0;
	strftime(errspace,sizeof(errspace),"%D %T",localtime(&ltime));
	sprintf(&errspace[strlen(errspace)]," File %s Line %d Ver %s ",file,line,
		date);
}
void prtopterr(char *fmt,...)
{
	va_list args;
#ifdef WIN32
	int counter=0;
	char userid[41];
	char temp[101];
	DWORD l=40;
	RDA_STDERR=NULL;

#endif


	if(RDA_STDERR!=NULL)
	{
		va_start(args,fmt);
		vsprintf(&errspace[strlen(errspace)],fmt,args);
		va_end(args);
		fprintf(RDA_STDERR,"%s\n",errspace);
		fflush(RDA_STDERR);

#ifdef WIN32	
	}else{
		GetUserName(userid,&l);
		sprintf(temp,"%s.ERR",userid);

		while(counter<10000)
		{
			RDA_STDERR=fopen(temp,"a+b");
			if(RDA_STDERR!=NULL) 
			{
				va_start(args,fmt);
				vsprintf(&errspace[strlen(errspace)],fmt,args);
				va_end(args);
				fprintf(RDA_STDERR,"%s\r\n",errspace);
				fclose(RDA_STDERR);
				return(0);
			}
			Sleep(1);
			counter++;
		}	
		return(0);
#endif
	}



}
char *errname(void)
{
	static char buffer[512];

	memset(buffer,0,512);
	sprintf(buffer,"[%s(%d)]",strerror(errno),errno);
	return(buffer);
}
