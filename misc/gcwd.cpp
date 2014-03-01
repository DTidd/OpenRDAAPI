#include <misc.hpp>
char *CURRENTDIRECTORY=NULL;
char *DOCUMENTROOT=NULL;
char *CURRENT_TIME=NULL;
char *CURRENT_DATE=NULL;
char *CURRENT_DATE10=NULL;
char *(*UI_GetAppRoot)(void);
char *(*UI_GetDocRoot)(void);

void SETCWD() 
{
#ifndef WT_FASTCGI
	char *dirx=NULL;
#else
	char *temp=NULL;
#endif
	time_t ltime;
	time(&ltime);
	if(CURRENTDIRECTORY!=NULL) Rfree(CURRENTDIRECTORY);
#ifdef WT_FASTCGI
	temp=UI_GetAppRoot();
	if(temp!=NULL) 
	{
		CURRENTDIRECTORY=stralloc(temp);	
	} else {
		CURRENTDIRECTORY=NULL;
	}
	if(DOCUMENTROOT!=NULL) Rfree(DOCUMENTROOT);
	DOCUMENTROOT=stralloc(UI_GetDocRoot());
#else
	dirx=getcwd((char *)NULL,128);
	CURRENTDIRECTORY=stralloc(dirx);
	if(dirx!=NULL) free(dirx);
	DOCUMENTROOT=stralloc(CURRENTDIRECTORY);
#endif
	CURRENT_DATE=(char *)Rmalloc(9);
	CURRENT_DATE10=(char *)Rmalloc(11);
	CURRENT_TIME=(char *)Rmalloc(9);
	strftime(CURRENT_DATE,9,"%D",localtime(&ltime));
	strftime(CURRENT_DATE10,11,"%m/%d/%Y",localtime(&ltime));
	strftime(CURRENT_TIME,9,"%T",localtime(&ltime));
}
int RCHGCWD(char *dirn)
{
	if(!chdir(dirn))
	{
		if(CURRENTDIRECTORY!=NULL) Rfree(CURRENTDIRECTORY);
		CURRENTDIRECTORY=stralloc(dirn);
		return(TRUE);
	}
	return(FALSE);
}
void FREECWD()
{
	if(CURRENTDIRECTORY!=NULL) Rfree(CURRENTDIRECTORY);
	if(CURRENT_DATE!=NULL) Rfree(CURRENT_DATE);
	if(CURRENT_DATE10!=NULL) Rfree(CURRENT_DATE10);
	if(CURRENT_TIME!=NULL) Rfree(CURRENT_TIME);
	CURRENT_DATE10=NULL;CURRENT_DATE=NULL;CURRENT_TIME=NULL;
}
char *GETCURRENTDATE()
{
	char *tmp=NULL;
	time_t ltime;

	time(&ltime);
	tmp=(char *)Rmalloc(9);
	strftime(tmp,9,"%D",localtime(&ltime));
	return(tmp);
}
char *GETCURRENTDATE10()
{
	char *tmp=NULL;
	time_t ltime;

	time(&ltime);
	tmp=(char *)Rmalloc(11);
	strftime(tmp,11,"%m/%d/%Y",localtime(&ltime));
	return(tmp);
}
char *GETCURRENTTIME()
{
	char *tmp=NULL;
	time_t ltime;

	time(&ltime);
	tmp=(char *)Rmalloc(9);
	strftime(tmp,9,"%T",localtime(&ltime));
	return(tmp);
}
