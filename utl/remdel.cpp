/* remdel.c - Remove Deleted Records */
#ifndef WIN32
#define __NAM__ "remdel.lnx"
#endif
#ifdef WIN32
#define __NAM__ "remdel.exe"
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
static void okfunction(RDArsrc *,char *,char *);
static void okfunctioncb(RDArsrc *);

static void okfunction(RDArsrc *mainrsrc,char *modulename,char *filename)
{
	short fileno=(-1),ef;
	char *errordesc=NULL;
	char deleteflag=FALSE;

	fileno=OPNNRDsec(modulename,filename,TRUE,TRUE,FALSE);
	if(fileno!=(-1))
	{
/* Tried this for performace but index pointer get screwed up in INFORMIX 
		ef=BEGNRDsec(fileno);
*/
		ef=FRSNRDsec(fileno,1);
		while(!ef)
		{
			if(FINDFLDGETCHAR(fileno,"DELETEFLAG",&deleteflag)) break;
			if(deleteflag) DELNRD(fileno);
/* Tried this for performace but index pointer get screwed up in INFORMIX 
			ef=SEQNRDsec(fileno);
*/
			ef=NXTNRDsec(fileno,1);
		}
		CLSNRD(fileno);
	} else {
		errordesc=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+89);
		sprintf(errordesc,"Security Access Denied to the Module [%s] File [%s]. No Action will be done to the data file.",modulename,filename);
		if(mainrsrc==NULL)
		{
			prterr(errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		} else {
			ERRORDIALOG("SECURITY ACCESS DENIED",errordesc,NULL,FALSE);
			if(errordesc!=NULL) Rfree(errordesc);
		}
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
static void okfunctioncb(RDArsrc *mainrsrc)
{
	int s,m=0;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&s)) return;
	okfunction(mainrsrc,modlst->libs[m],file_defs->libs[s]);
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
	int x=0,l=0;
	RDArsrc *mainrsrc=NULL;

	if(argc>3)
	{
		if(InitializeSubsystems(argc,argv,module,"REMOVE DELETES"))
		{
			return;
		}
		if(!RDAstrcmp(argv[3],"ALL")) 
		{
			okfunction(mainrsrc,argv[1],argv[2]);
			ShutdownSubsystems();
			std::exit;
		}
	}

/* Not automatically removing deletes with key REMOVE ALL DELETES, just setting module and file lists */
	if(InitializeSubsystems(argc,argv,module,"REMOVE DELETES")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"REMOVE DELETED RECORDS");
	modlst=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,domodulelist,modlst->numlibs,
		&modlst->libs,NULL);
	getmodulelist(mainrsrc);
	if(argc>1)
	{
		for(l=0;l<modlst->numlibs;++l)
		{
			if(!RDAstrcmp(argv[1],modlst->libs[l])) break;
		}
		if(l>=modlst->numlibs)
		{
			l=0;
		}
	}
	FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",l,modlst);


	file_defs=APPlibNEW();
	addlstrsrc(mainrsrc,"FILE LIST",&x,TRUE,NULL,file_defs->numlibs,
		&file_defs->libs,NULL);
	domodulelist(mainrsrc);
	if(argc>2)
	{
		for(x=0;x<file_defs->numlibs;++x)
		{
			if(!RDAstrcmp(argv[2],file_defs->libs[x])) break;
		}
		if(x>=file_defs->numlibs)
		{
			x=0;
		}
	}
	FINDRSCLISTAPPlib(mainrsrc,"FILE LIST",x,file_defs);

	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunctioncb,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
