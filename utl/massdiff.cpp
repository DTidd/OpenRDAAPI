/* dfnrd.c - Define Datafiles */
#ifndef WIN32
#define __NAM__ "massdiff.lnx"
#endif
#ifdef WIN32
#define __NAM__ "massdiff.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include<cstdlib>
#include<io.h>
#endif
#include<mknrd.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char RangeMod=FALSE,*suffix1=NULL;
char *FromMod=NULL,*ToMod=NULL,*suffix2=NULL;

static void doexit(RDArsrc *r)
{
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(FromMod!=NULL) Rfree(FromMod);
	if(ToMod!=NULL) Rfree(ToMod);
	if(suffix1!=NULL) Rfree(suffix1);
	if(suffix2!=NULL) Rfree(suffix2);
	ShutdownSubsystems();
	std::exit;
}
static void okfunction(RDArsrc *r)
{
	int y=0,z=0,pid=0;
	char *tmp=NULL,*dirx=NULL,*libx=NULL;
	APPlib *modlst=NULL,*filelst=NULL;
	APPlib *args=NULL;
	char temp[512],stemp1[101];

	pid=RGETPID();
	readallwidgets(r);
	FINDRSCGETCHAR(r,"RANGE MODULE",&RangeMod);
	FINDRSCGETSTRING(r,"FROM MODULE",&FromMod);
	FINDRSCGETSTRING(r,"TO MODULE",&ToMod);
	FINDRSCGETSTRING(r,"SUFFIX 1",&suffix1);
	FINDRSCGETSTRING(r,"SUFFIX 2",&suffix2);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	modlst=APPlibNEW();
	for(z=0;findfile(dirx,"*.FIL",&tmp,(int)z+1);++z) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(RDAstrcmp(tmp,"RPTGEN") && RDAstrcmp(tmp,"CONVERSION") && RDAstrcmp(tmp,"MBGUICONV")) 
		{
			if(!RangeMod || ((RDAstrcmp(FromMod,tmp)<=0) && RDAstrcmp(tmp,ToMod)<=0))
			{
				addAPPlib(modlst,tmp);
			}
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(modlst->numlibs<1)
	{
		prterr("Error:  No modules found (*.FIL's).");
	} else {
		SORTAPPlib(modlst);
		unlink("myisam.diff");
		for(z=0;z<modlst->numlibs;++z)
		{
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[z])+10);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[z]);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[z]);
#endif
			filelst=getlibnames(libx,TRUE);
			SORTAPPlib(filelst);
			if(libx!=NULL) Rfree(libx);
			if(filelst->numlibs>0)
			{
				for(y=0;y<filelst->numlibs;++y)
				{
					memset(stemp,0,101);
#ifdef WIN32
					sprintf(stemp,"data\\%s.%s.%s.csv",modlst->libs[z],filelst->libs[y],suffix1);
#endif
#ifndef WIN32
					sprintf(stemp,"data/%s.%s.%s.csv",modlst->libs[z],filelst->libs[y],suffix1);
#endif
					memset(stemp1,0,101);
#ifdef WIN32
					sprintf(stemp1,"data\\%s.%s.%s.csv",modlst->libs[z],filelst->libs[y],suffix2);
#endif
#ifndef WIN32
					sprintf(stemp1,"data/%s.%s.%s.csv",modlst->libs[z],filelst->libs[y],suffix2);
#endif
					memset(temp,0,512);
					sprintf(temp,"cat %s | sort > %d.csv",stemp,pid);
					system_command(temp);
					memset(temp,0,512);
					sprintf(temp,"cat %s | sort > %d.1.csv",stemp1,pid);
					system_command(temp);
					
					memset(temp,0,512);
					sprintf(temp,"echo \"diff %s %s\" >>myisam.diff",stemp,stemp1);
					system_command(temp);
					memset(temp,0,512);
					sprintf(temp,"diff \"%d.csv\" \"%d.1.csv\" >>myisam.diff 2>&1",pid,pid);
					system_command(temp);
					memset(temp,0,512);
					sprintf(temp,"echo \" \" >>myisam.diff");
					system_command(temp);
				}
			}
			if(libx!=NULL) Rfree(libx);
			if(filelst!=NULL) freeapplib(filelst);
		}
	}
	if(modlst!=NULL) freeapplib(modlst);
	if(!access("myisam.diff",02))
	{
		args=APPlibNEW();
		addAPPlib(args,"-s");
		addAPPlib(args,"myisam.diff");
		ExecuteProgram("rdaout",args);
		if(args!=NULL) freeapplib(args);
	}
	doexit(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	RDA_SOFTWARE_TYPE=RDA_ALL_APP;
	if(InitializeSubsystems(argc,argv,"DATABASE","MASSDIFF")) 
	{
		return;
	}
	suffix1=stralloc("orig");
	suffix2=stralloc("myisam");
	mainrsrc=RDArsrcNEW("DATABASE","MASS EXPORT DIFFERENCES");
	addstdrsrc(mainrsrc,"SUFFIX 1",PLAINTEXT,15,suffix1,TRUE);
	addstdrsrc(mainrsrc,"SUFFIX 2",PLAINTEXT,15,suffix2,TRUE);
	addstdrsrc(mainrsrc,"RANGE MODULE",BOOLNS,1,&RangeMod,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",PLAINTEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",PLAINTEXT,15,NULL,TRUE);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
