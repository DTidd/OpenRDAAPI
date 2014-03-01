/* mkaddcd.c - Generate Power Add C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkaddcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkaddcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkadd.hpp>
#include <mkmsc.hpp>
#include <packlib.hpp>
/*ADDLIB tool */
/*ADDLIB mklib */
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


APPlib *liblst=NULL,*addsavl=NULL,*dir_libs=NULL;
static void getaddlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void quit_printPowerAddcode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printPowerAddcode Quitting Print RDA Screen Code");
        }
        free_rsrc(prsrc);
}
static void printPowerAddcode(RDArsrc *prsrc,PowerAdd *add)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*addname=NULL;
	int l=0,s=0;

	if(add!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
			if(FINDRSCGETSTRING(mrsrc,"DIRECTORY",&dirname))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"MODULE LIST",&l))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"POWER ADDS AVAILABLE",&s))
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
				libname=stralloc(liblst->libs[l]);
				addname=stralloc(addsavl->libs[s]);
				makeaddcode(add,fp,dirname,libname,addname);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(add!=NULL) Rfree(add);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintPowerAdd popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
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
	if(dirname!=NULL) dirname=NULL;
	if(libname!=NULL) libname=NULL;
	if(addname!=NULL) addname=NULL;
}
static void GetDevicePrintPowerAddcode(PowerAdd *add)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printPowerAddcode,add);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printPowerAddcode,NULL);
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
static PowerAdd *okPowerAdd(char *dirname,char *libname,char *addname)
{
	PowerAdd *add=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(add!=NULL) FreePowerAdd(add); 
		add=PowerAddNew(libname,addname);
		dirx=Rmalloc(RDAstrlen(dirname)+2);
#ifndef WIN32
		sprintf(dirx,"%s/",dirname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\",dirname);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.ADD",dirx,libname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.ADD",dirx,libname);
#endif
		if(dirx!=NULL) Rfree(dirx);
		if(getPowerAdd(libx,add))
		{
			prterr("ERROR: Cannot Read Power Add Definition.  Error in permissions or Power Add [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,addname);
			if(add!=NULL) FreePowerAdd(add);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(add);
		}
	} else { 
		return(NULL);
	}
}
void okPowerAddcb(RDArsrc *mainrsrc)
{
	PowerAdd *add=NULL;
	int l=0,s=0;
	char *libname=NULL,*addname=NULL,*dirname=NULL;

	if(FINDRSCGETSTRING(mrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mrsrc,"MODULE LIST",&l))
	{
		return;
	} else {
		libname=stralloc(liblst->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"POWER ADDS AVAILABLE",&s))
	{
		return;
	} else {
		addname=stralloc(addsavl->libs[s]);
	}
	add=okPowerAdd(dirname,libname,addname);
	if(add!=NULL)
	{
		GetDevicePrintPowerAddcode(add);
	}
	if(dirname!=NULL) Rfree(dirname);
	if(libname!=NULL) Rfree(libname);
	if(addname!=NULL) Rfree(addname);
}
static void setvalues(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *tmpx=NULL,*temp=NULL,*libx=NULL,*directory=NULL;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(liblst->libs[selectedm])+6);
	tmpx=adddashes(liblst->libs[selectedm]);
#ifndef WIN32
	sprintf(libx,"%s/%s.ADD",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.ADD",directory,tmpx);
#endif
	if(tmpx!=NULL) Rfree(tmpx);
	if(FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(addsavl->libs[selected],"No Power Adds"))
	{
		temp=NULL;
	} else {
		temp=getPowerAdddesc(libx,addsavl->libs[selected]);
	}
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
	{
		updatersrc(mainrsrc,"DESCRIPTION");
	}
	if(temp!=NULL) Rfree(temp);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&s);
	freeapplib(addsavl);
	if(!RDAstrcmp(liblst->libs[l],"Directory Contains No Libraries"))
	{
		addsavl=APPlibNEW();
		addAPPlib(addsavl,"No Power Adds");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(liblst->libs[l])+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.ADD",dirx,liblst->libs[l]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.ADD",dirx,liblst->libs[l]);
#endif
		addsavl=getlibnames(libx,FALSE);
		if(addsavl==NULL)
		{
			addsavl=APPlibNEW();
			addAPPlib(addsavl,"No Power Adds");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=addsavl->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"POWER ADDS AVAILABLE",s,addsavl))
	{
		updatersrc(mainrsrc,"POWER ADDS AVAILABLE");
	}
	setvalues(mainrsrc);
}
static void getaddlist(RDArsrc *mainrsrc)
{
	int l=0,x=0;
	char *dirname=NULL;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&l))
	{
		l=0;
	}
	if(liblst!=NULL) freeapplib(liblst);
	liblst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(dirname)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",dirname);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",dirname);
#endif 
	for(x=0;findfile(dirx,"*.ADD",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(liblst,tmp);
	}
	if(liblst->numlibs<1)
	{
		addAPPlib(liblst,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(liblst);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(l>=liblst->numlibs) l=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",l,liblst))
	{
		updatersrc(mainrsrc,"MODULE LIST");
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
	getaddlist(mainrsrc);
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
	getaddlist(mainrsrc);
}
void quitaddcode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(liblst!=NULL) freeapplib(liblst);
	if(addsavl!=NULL) freeapplib(addsavl);
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
	char *dirname=NULL,*libname=NULL,*addname=NULL;
	char *defdir=NULL,*temp=NULL;
	PowerAdd *add=NULL;
	int x=0;

	if(argc>2)
	{
		if(argc>3)
		{
			if(InitializeSubsystems(argc,argv,argv[2],"MAKE POWER ADD CODE"))
			{
				RDAAPPMAINLOOP();
				return;
			}
		}
		else if(argc==3)
		{
			if(InitializeSubsystems(argc,argv,argv[1],"MAKE POWER ADD CODE"))
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
			addname=stralloc(argv[2]);
		}
		else
		{
			dirname=stralloc(argv[1]);
			libname=stralloc(argv[2]);
			addname=stralloc(argv[3]);
		}
		add=okPowerAdd(dirname,libname,addname);
		if(add!=NULL)
		{
			makeaddcode(add,stdout,dirname,libname,addname);
		}
		if(add!=NULL) Rfree(add);
		if(dirname!=NULL) Rfree(dirname);
		if(libname!=NULL) Rfree(libname);
		if(addname!=NULL) Rfree(addname);
		ShutdownSubsystems();
		exit(0);
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE POWER ADD CODE"))
		{
			ShutdownSubsystems();
			exit(1);
/*
			RDAAPPMAINLOOP();
			return;
*/
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE POWER ADD CODE");
		liblst=APPlibNEW();
		addsavl=APPlibNEW();
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
		addlstrsrc(mrsrc,"MODULE LIST",&l,TRUE,doliblist,liblst->numlibs,&liblst->libs,NULL);
		x=0;
		addlstrsrc(mrsrc,"POWER ADDS AVAILABLE",&x,TRUE,setvalues,addsavl->numlibs,&addsavl->libs,NULL);
		addsctrsrc(mrsrc,"DESCRIPTION",0,NULL,FALSE);
		getdir(mrsrc);
		if(argc>1)
		{
			for(l=0;l<liblst->numlibs;++l)
			{
				if(!RDAstrcmp(argv[1],liblst->libs[l])) break;
			}
			if(l>=liblst->numlibs)
			{
				l=0;
			}
		}
		FINDRSCLISTAPPlib(mrsrc,"MODULE LIST",l,liblst);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"POWER ADDS AVAILABLE",s,addsavl);
		setvalues(mrsrc);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okPowerAddcb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitaddcode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"MODULE LIST");
		APPmakescrn(mrsrc,TRUE,quitaddcode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
