/* dfnrd.c - Define Datafiles */
#ifndef WIN32
#define __NAM__ "massdfnrd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "massdfnrd.exe"
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

char RangeMod=FALSE;
char *FromMod=NULL,*ToMod=NULL;

static void doexit(RDArsrc *r)
{
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(FromMod!=NULL) Rfree(FromMod);
	if(ToMod!=NULL) Rfree(ToMod);
	ShutdownSubsystems();
}
static void okfunction(RDArsrc *r)
{
	int y=0,z=0;
	char *tmp=NULL,*dirx=NULL,*libx=NULL;
	APPlib *modlst=NULL,*filelst=NULL;
	APPlib *args=NULL;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"RANGE MODULE",&RangeMod);
	FINDRSCGETSTRING(r,"FROM MODULE",&FromMod);
	FINDRSCGETSTRING(r,"TO MODULE",&ToMod);
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
					args=APPlibNEW();
					addAPPlib(args,modlst->libs[z]);
					addAPPlib(args,filelst->libs[y]);
					Execute2Program("dfnrd",args);
					if(args!=NULL) freeapplib(args);
				}
			}
			if(libx!=NULL) Rfree(libx);
			if(filelst!=NULL) freeapplib(filelst);
		}
	}
	if(modlst!=NULL) freeapplib(modlst);
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
	if(InitializeSubsystems(argc,argv,"DATABASE","MASS DFNRD")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW("DATABASE","MASS DFNRD");
	addstdrsrc(mainrsrc,"RANGE MODULE",BOOLNS,1,&RangeMod,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",PLAINTEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",PLAINTEXT,15,NULL,TRUE);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
