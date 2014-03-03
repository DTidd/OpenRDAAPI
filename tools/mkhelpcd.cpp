/* makehelpcode.c - Generate Help C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkhelpcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkhelpcd.exe"
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


APPlib *help_libs=NULL,*help_defs=NULL,*dir_libs=NULL;
static void gethelplist(RDArsrc *);
RDArsrc *mrsrc=NULL;

static void quit_printRDAhelpcode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDAhelpcode Quitting Print RDA Help Code");
        }
        free_rsrc(prsrc);
}
static void printRDAhelpcode(RDArsrc *prsrc,RDAhelp *help)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*scrnname=NULL;
	int l=0,s=0;

	if(help!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
			if(FINDRSCGETSTRING(mrsrc,"DIRECTORY",&dirname))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"SCREEN LIST",&s))
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
				libname=stralloc(help_libs->libs[l]);
				scrnname=stralloc(help_defs->libs[s]);
				stoupper(scrnname);
				stoupper(help->name);
				makehelpcode(help,fp,dirname,libname,scrnname);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(help!=NULL) free_help(help);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDAhelpcode popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
				if(dirname!=NULL) dirname=NULL;
				if(outdevice!=NULL) Rfree(outdevice);
			}
		} else {
			ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
	} else {
		ERRORDIALOG("SCREEN ERROR!","The attempt to retrieve the help definition failed.",NULL,FALSE);
		prterr("SCREEN RETRIVAL ERROR: The attempt to retrieve the help definition failed.",NULL,FALSE);
	}
	if(dirname!=NULL) dirname=NULL;
	if(libname!=NULL) libname=NULL;
	if(scrnname!=NULL) scrnname=NULL;
}
static void GetDevicePrintRDAhelpcode(RDAhelp *help)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAhelpcode,help);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAhelpcode,NULL);
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
void okhelpcb(RDArsrc *mainrsrc)
{
	RDAhelp *help=NULL;
	int l=0,s=0;
	char *libname=NULL,*scrnname=NULL,*dirname=NULL;

	if(FINDRSCGETSTRING(mrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
	{
		return;
	} else {
		libname=stralloc(help_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"SCREEN LIST",&s))
	{
		return;
	} else {
		scrnname=stralloc(help_defs->libs[s]);
	}
	help=okhelp(dirname,libname,scrnname);
	if(help!=NULL)
	{
		GetDevicePrintRDAhelpcode(help);
	}
	if(dirname!=NULL) Rfree(dirname);
	if(libname!=NULL) Rfree(libname);
	if(scrnname!=NULL) Rfree(scrnname);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"SCREEN LIST",&s);
	freeapplib(help_defs);
	if(!RDAstrcmp(help_libs->libs[l],"Directory Contains No Libraries"))
	{
		help_defs=APPlibNEW();
		addAPPlib(help_defs,"Contains No Screen Definitions");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(help_libs->libs[l])+6);
		sprintf(libx,"%s/%s.HLP",dirx,help_libs->libs[l]);
		help_defs=getlibnames(libx,FALSE);
		if(help_defs==NULL)
		{
			help_defs=APPlibNEW();
			addAPPlib(help_defs,"Contains No Screen Definitions");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=help_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN LIST",s,help_defs))
	{
		updatersrc(mainrsrc,"SCREEN LIST");
	}
}
static void gethelplist(RDArsrc *mainrsrc)
{
	int l=0,x=0;
	char *dirname=NULL;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l))
	{
		l=0;
	}
	if(help_libs!=NULL) freeapplib(help_libs);
	help_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(dirname)+2);
	sprintf(dirx,"%s/",dirname);
	for(x=0;findfile(dirx,"*.HLP",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(help_libs,tmp);
	}
	if(help_libs->numlibs<1)
	{
		addAPPlib(help_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(help_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(l>=help_libs->numlibs) l=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LIBRARY LIST",l,help_libs))
	{
		updatersrc(mainrsrc,"LIBRARY LIST");
	}
	doliblist(mainrsrc);
	if(dirname!=NULL) Rfree(dirname);
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory))
		{
			inlist=TRUE;
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
	updatersrc(mainrsrc,"DIRECTORY LIST");
	gethelplist(mainrsrc);
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s]))
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s])) return;
		updatersrc(mainrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	gethelplist(mainrsrc);
}
void quithelpcode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(help_libs!=NULL) freeapplib(help_libs);
	if(help_defs!=NULL) freeapplib(help_defs);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int d=2,l=0,s=0;
	char *dirname=NULL,*libname=NULL,*scrnname=NULL;
	char *defdir=NULL;
	RDAhelp *help=NULL;

	if(argc>2)
	{
		if(argc>3)
		{
			if(InitializeSubsystems(argc,argv,argv[2],"MAKE HELP CODE"))
			{
				RDAAPPMAINLOOP();
				return;
			}
		}
		else if(argc==3)
		{
			if(InitializeSubsystems(argc,argv,argv[1],"MAKE HELP CODE"))
			{
				RDAAPPMAINLOOP();
				return;
			}
		}
		if(argc==3)
		{
			dirname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
			sprintf(dirname,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
			sprintf(dirname,"%s\\rda",CURRENTDIRECTORY);
#endif
			libname=stralloc(argv[1]);
			scrnname=stralloc(argv[2]);
		}
		else
		{
			dirname=stralloc(argv[1]);
			libname=stralloc(argv[2]);
			scrnname=stralloc(argv[3]);
		}
		help=okhelp(dirname,libname,scrnname);
		if(help!=NULL)
		{
			makehelpcode(help,stdout,dirname,libname,scrnname);
		}
		if(help!=NULL) free_help(help);
		if(dirname!=NULL) Rfree(dirname);
		if(libname!=NULL) Rfree(libname);
		if(scrnname!=NULL) Rfree(scrnname);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE HELP CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE HELP CODE");
		help_libs=APPlibNEW();
		help_defs=APPlibNEW();
		dir_libs=APPlibNEW();
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
		sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
		sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
		sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
		addAPPlib(dir_libs,defdir);
		if(defdir!=NULL) Rfree(defdir); 
		addlstrsrc(mrsrc,"DIRECTORY LIST",&d,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
		addstdrsrc(mrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[0],TRUE);
		if(defdir!=NULL) Rfree(defdir);
		FINDRSCLISTAPPlib(mrsrc,"DIRECTORY LIST",d,dir_libs);
		FINDRSCSETFUNC(mrsrc,"DIRECTORY",setdirlist,NULL);
		addlstrsrc(mrsrc,"LIBRARY LIST",&l,TRUE,doliblist,help_libs->numlibs,&help_libs->libs,NULL);
		addlstrsrc(mrsrc,"SCREEN LIST",&s,TRUE,NULL,help_defs->numlibs,&help_defs->libs,NULL);
		getdir(mrsrc);
		if(argc>1)
		{
			for(l=0;l<help_libs->numlibs;++l)
			{
				if(!RDAstrcmp(argv[1],help_libs->libs[l])) break;
			}
			if(l>=help_libs->numlibs)
			{
				l=0;
			}
		}
		FINDRSCLISTAPPlib(mrsrc,"LIBRARY LIST",l,help_libs);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"SCREEN LIST",s,help_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okhelpcb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quithelpcode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"LIBRARY LIST");
		APPmakescrn(mrsrc,TRUE,quithelpcode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
