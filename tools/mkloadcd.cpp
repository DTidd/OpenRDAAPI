/* mkloadcd.c - Generate RDALoadValue C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkloadcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkloadcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <packlib.hpp>
/*ADDLIB tool */
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


APPlib *scr_libs=NULL,*scr_defs=NULL;
static void getscreenlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void quit_printRDAscrncode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDAscrncode Quitting Print RDA Screen Code");
        }
        free_rsrc(prsrc);
}
static void printRDAscrncode(RDArsrc *prsrc,RDALoadValue *scrn)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*scrnname=NULL;
	int l=0,s=0;

	if(scrn!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
			if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"LOAD VALUES",&s))
			{
				return;
			}
#ifndef WIN32
			fp=popen(outdevice,"w");
#endif
#ifdef WIN32
			fp=fopen(outdevice,"a+");
#endif
                	if(fp!=NULL)
                	{
				if(outdevice!=NULL) Rfree(outdevice);
				libname=stralloc(scr_libs->libs[l]);
				scrnname=stralloc(scr_defs->libs[s]);
				makeloadcode(scrn,fp,libname,scrnname);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(scrn!=NULL) Rfree(scrn);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDAscrn popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
				if(dirname!=NULL) dirname=NULL;
				if(outdevice!=NULL) Rfree(outdevice);
			}
		} else {
			ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
	} else {
		ERRORDIALOG("SCREEN ERROR!","The attempt to retrieve the screen definition failed.",NULL,FALSE);
		prterr("SCREEN RETRIVAL ERROR: The attempt to retrieve the screen definition failed.",NULL,FALSE);
	}
	if(libname!=NULL) libname=NULL;
	if(scrnname!=NULL) scrnname=NULL;
}
static void GetDevicePrintRDAscrncode(RDALoadValue *scrn)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAscrncode,scrn);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAscrncode,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PACKAGE BINARY. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
void okloadcb(RDArsrc *mainrsrc)
{
	RDALoadValue *scrn=NULL;
	int l=0,s=0;
	char *libname=NULL,*scrnname=NULL;

	if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
	{
		return;
	} else {
		libname=stralloc(scr_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"LOAD VALUES",&s))
	{
		return;
	} else {
		scrnname=stralloc(scr_defs->libs[s]);
	}
	scrn=okload(libname,scrnname);
	if(scrn!=NULL)
	{
		GetDevicePrintRDAscrncode(scrn);
	}
	if(libname!=NULL) Rfree(libname);
	if(scrnname!=NULL) Rfree(scrnname);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"LOAD VALUES",&s);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	freeapplib(scr_defs);
	if(!RDAstrcmp(scr_libs->libs[l],"Directory Contains No Libraries"))
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Screen Definitions");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+6);
		sprintf(libx,"%s/%s.LDV",dirx,scr_libs->libs[l]);
		scr_defs=getlibnames(libx,FALSE);
		if(scr_defs==NULL)
		{
			scr_defs=APPlibNEW();
			addAPPlib(scr_defs,"Contains No Screen Definitions");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=scr_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LOAD VALUES",s,scr_defs))
	{
		updatersrc(mainrsrc,"LOAD VALUES");
	}
}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int l=0,x=0;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l))
	{
		l=0;
	}
	if(scr_libs!=NULL) freeapplib(scr_libs);
	scr_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.LDV",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(scr_libs,tmp);
	}
	if(scr_libs->numlibs<1)
	{
		addAPPlib(scr_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(scr_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(l>=scr_libs->numlibs) l=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LIBRARY LIST",l,scr_libs))
	{
		updatersrc(mainrsrc,"LIBRARY LIST");
	}
	doliblist(mainrsrc);
}
void quitscrncode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(scr_libs!=NULL) freeapplib(scr_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int l=0,s=0;
	char *libname=NULL,*scrnname=NULL;
	RDALoadValue *scrn=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE LOAD VALUE CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		libname=stralloc(argv[1]);
		scrnname=stralloc(argv[2]);
		scrn=okload(libname,scrnname);
		if(scrn!=NULL)
		{
			makeloadcode(scrn,stdout,libname,scrnname);
		}
		if(scrn!=NULL) Rfree(scrn);
		if(libname!=NULL) Rfree(libname);
		if(scrnname!=NULL) Rfree(scrnname);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE LOAD VALUE CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE LOAD VALUE CODE");
		scr_libs=APPlibNEW();
		scr_defs=APPlibNEW();
		addlstrsrc(mrsrc,"LIBRARY LIST",&l,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
		addlstrsrc(mrsrc,"LOAD VALUES",&s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
		getscreenlist(mrsrc);
		if(argc>1)
		{
			for(l=0;l<scr_libs->numlibs;++l)
			{
				if(!RDAstrcmp(argv[1],scr_libs->libs[l])) break;
			}
			if(l>=scr_libs->numlibs)
			{
				l=0;
			}
		}
		FINDRSCLISTAPPlib(mrsrc,"LIBRARY LIST",l,scr_libs);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"LOAD VALUES",s,scr_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okloadcb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitscrncode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"LIBRARY LIST");
		APPmakescrn(mrsrc,TRUE,quitscrncode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
