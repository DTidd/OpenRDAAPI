/* restruct.c - Restructure Data File */
#ifndef WIN32
#define __NAM__ "restruct.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restruct.exe"
#endif
#include <app.hpp>





#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *module="DATABASE";
static APPlib *modlst=NULL,*file_defs=NULL;

static void okfunction(RDArsrc *mainrsrc)
{
	int s,m=0;
	short fileno=(-1);
	char *errordesc=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&s)) return;
	fileno=OPNNRDsec(modlst->libs[m],file_defs->libs[s],TRUE,TRUE,FALSE);
	if(fileno!=(-1))
	{
		fileno=RESNRD(fileno);
		if(fileno!=(-1)) 
		{
			CLSNRD(fileno);
		} else {
			prterr("Error File wasn't rebuilt successfully.");
		}
	} else {
		errordesc=Rmalloc(RDAstrlen(modlst->libs[m])+RDAstrlen(file_defs->libs[s])+89);
		sprintf(errordesc,"Security Access Denied to the Module [%s] File [%s]. No Action will be done to the data file.",modlst->libs[m],file_defs->libs[s]);
		ERRORDIALOG("SECURITY ACCESS DENIED",errordesc,NULL,FALSE);
		if(errordesc!=NULL) Rfree(errordesc);
	}
}
static void doexit(RDArsrc *mainrsrc)
{
	free_rsrc(mainrsrc);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(modlst!=NULL) freeapplib(modlst);
	ShutdownSecurity();
	EXITHELP();
	CLSLOG();
}
void getmodulelist(RDArsrc *mainrsrc)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlst);
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",0,modlst))
	{
		updatersrc(mainrsrc,"MODULE LIST");
	}
}
void domodulelist(RDArsrc *mainrsrc)
{
	int selected=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(file_defs!=NULL) freeapplib(file_defs);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
	file_defs=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(file_defs==NULL)
	{
		file_defs=APPlibNEW();
		addAPPlib(file_defs,"Contains No File Definitions");
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"FILE LIST",0,file_defs))
		updatersrc(mainrsrc,"FILE LIST");
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"RESTRUCTURE DATFILE")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"RESTRUCTURE DATA FILES");
	modlst=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,domodulelist,modlst->numlibs,
		&modlst->libs,NULL);
	getmodulelist(mainrsrc);
	file_defs=APPlibNEW();
	addlstrsrc(mainrsrc,"FILE LIST",&x,TRUE,NULL,file_defs->numlibs,
		&file_defs->libs,NULL);
	domodulelist(mainrsrc);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
