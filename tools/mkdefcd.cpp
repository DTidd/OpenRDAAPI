/* mkdefcd.c - Generate Screen Default C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkdefcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkdefcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkdef.hpp>
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


APPlib *scr_libs=NULL,*scr_defs=NULL,*dir_libs=NULL;
static void getscreenlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void quit_printRDAdefaultcode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDAdefaultcode Quitting Print RDA Screen Default Code");
        }
        free_rsrc(prsrc);
}
static void printRDAdefaultcode(RDArsrc *prsrc,RDAdefault *def)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*scrnname=NULL;
	int l=0,s=0;

	if(def!=NULL)
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
			if(FINDRSCGETINT(mrsrc,"SCREEN DEFAULT LIST",&s))
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
				makedefcode(def,fp,dirname,libname,scrnname);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(def!=NULL) FreeRDAdefault(def);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDAdefaultcode popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
				if(dirname!=NULL) dirname=NULL;
				if(outdevice!=NULL) Rfree(outdevice);
			}
		} else {
			ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
	} else {
		ERRORDIALOG("SCREEN DEFAULT ERROR!","The attempt to retrieve the screen default definition failed.",NULL,FALSE);
		prterr("SCREEN DEFAULT RETRIVAL ERROR: The attempt to retrieve the screen default definition failed.",NULL,FALSE);
	}
	if(dirname!=NULL) dirname=NULL;
	if(libname!=NULL) libname=NULL;
	if(scrnname!=NULL) scrnname=NULL;
}
static void GetDevicePrintRDAdefaultcode(RDAdefault *def)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAdefaultcode,def);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAdefaultcode,NULL);
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
void okdefcb(RDArsrc *mainrsrc)
{
	RDAdefault *def=NULL;
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
		libname=stralloc(scr_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"SCREEN DEFAULT LIST",&s))
	{
		return;
	} else {
		scrnname=stralloc(scr_defs->libs[s]);
	}
	def=okdef(dirname,libname,scrnname);
	if(def!=NULL)
	{
		GetDevicePrintRDAdefaultcode(def);
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
	FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIST",&s);
	freeapplib(scr_defs);
	if(!RDAstrcmp(scr_libs->libs[l],"Directory Contains No Libraries"))
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Screen Definitions");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+6);
		sprintf(libx,"%s/%s.DEF",dirx,scr_libs->libs[l]);
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
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN DEFAULT LIST",s,scr_defs))
	{
		updatersrc(mainrsrc,"SCREEN DEFAULT LIST");
	}
}
static void getscreenlist(RDArsrc *mainrsrc)
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
	if(scr_libs!=NULL) freeapplib(scr_libs);
	scr_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(dirname)+2);
	sprintf(dirx,"%s/",dirname);
	for(x=0;findfile(dirx,"*.DEF",&tmp,(int)x+1);++x)
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
	getscreenlist(mainrsrc);
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
	getscreenlist(mainrsrc);
}
void quitdefaultcode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(scr_libs!=NULL) freeapplib(scr_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	ShutdownSubsystems();
	exit(0);
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
	RDAdefault *def=NULL;

	if(argc>2)
	{
		if(argc>3)
		{
			if(InitializeSubsystems(argc,argv,argv[2],"MAKE SCREEN DEFAULT CODE"))
			{
				RDAAPPMAINLOOP();
				return;
			}
		}
		else if(argc==3)
		{
			if(InitializeSubsystems(argc,argv,argv[1],"MAKE SCREEN DEFAULT CODE"))
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
		def=okdef(dirname,libname,scrnname);
		if(def!=NULL)
		{
			makedefcode(def,stdout,dirname,libname,scrnname);
		}
		if(def!=NULL) FreeRDAdefault(def);
		if(dirname!=NULL) Rfree(dirname);
		if(libname!=NULL) Rfree(libname);
		if(scrnname!=NULL) Rfree(scrnname);
		ShutdownSubsystems();
		exit(0);
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE SCREEN DEFAULT CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE SCREEN DEFAULT CODE");
		scr_libs=APPlibNEW();
		scr_defs=APPlibNEW();
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
		addlstrsrc(mrsrc,"LIBRARY LIST",&l,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
		addlstrsrc(mrsrc,"SCREEN DEFAULT LIST",0,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
		getdir(mrsrc);
		if(argc==2)
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
		FINDRSCLISTAPPlib(mrsrc,"SCREEN DEFAULT LIST",s,scr_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okdefcb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitdefaultcode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"LIBRARY LIST");
		APPmakescrn(mrsrc,TRUE,quitdefaultcode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
