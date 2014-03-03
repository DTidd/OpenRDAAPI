/* makelistcode.c - Generate List C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mklistcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mklistcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkmsc.hpp>
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
static void getlistlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void quit_printRDAlistcode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDAlistcode Quitting Print RDA List Code");
        }
        free_rsrc(prsrc);
}
static void printRDAlistcode(RDArsrc *prsrc,RDAScrolledList *list)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*listname=NULL;
	int l=0,s=0;

	if(list!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
			if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"SCROLLEDLISTS",&s))
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
				listname=stralloc(scr_defs->libs[s]);
				makelistcode(list,fp,libname,listname);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(list!=NULL) Rfree(list);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDAlist popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
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
	if(listname!=NULL) listname=NULL;
}
static void GetDevicePrintRDAlistcode(RDAScrolledList *list)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAlistcode,list);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAlistcode,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKElist FAILED","The Make Screen function failed for the custom screen PACKAGE BINARY. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
void oklistcb(RDArsrc *mainrsrc)
{
	RDAScrolledList *list=NULL;
	int l=0,s=0;
	char *libname=NULL,*listname=NULL;

	if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
	{
		return;
	} else {
		libname=stralloc(scr_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"SCROLLEDLISTS",&s))
	{
		return;
	} else {
		listname=stralloc(scr_defs->libs[s]);
	}
	list=oklist(libname,listname);
	if(list!=NULL)
	{
		GetDevicePrintRDAlistcode(list);
	}
	if(libname!=NULL) Rfree(libname);
	if(listname!=NULL) Rfree(listname);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"SCROLLEDLISTS",&s);
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
		sprintf(libx,"%s/%s.LST",dirx,scr_libs->libs[l]);
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
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCROLLEDLISTS",s,scr_defs))
	{
		updatersrc(mainrsrc,"SCROLLEDLISTS");
	}
}
static void getlistlist(RDArsrc *mainrsrc)
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
	for(x=0;findfile(dirx,"*.LST",&tmp,(int)x+1);++x)
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
void quitlistcode(RDArsrc *r)
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
	char *libname=NULL,*listname=NULL;
	RDAScrolledList *list=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE LIST CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		libname=stralloc(argv[1]);
		listname=stralloc(argv[2]);
		list=oklist(libname,listname);
		if(list!=NULL)
		{
			makelistcode(list,stdout,libname,listname);
		}
		if(list!=NULL) Rfree(list);
		if(libname!=NULL) Rfree(libname);
		if(listname!=NULL) Rfree(listname);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE LIST CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE LIST CODE");
		scr_libs=APPlibNEW();
		scr_defs=APPlibNEW();
		addlstrsrc(mrsrc,"LIBRARY LIST",&l,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
		addlstrsrc(mrsrc,"SCROLLEDLISTS",&s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
		getlistlist(mrsrc);
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
		FINDRSCLISTAPPlib(mrsrc,"SCROLLEDLISTS",s,scr_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,oklistcb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitlistcode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"LIBRARY LIST");
		APPmakescrn(mrsrc,TRUE,quitlistcode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
