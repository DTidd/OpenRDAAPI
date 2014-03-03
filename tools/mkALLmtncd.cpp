/* mkALLmtncd.c - Generate MaintainMaster C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkALLmtncd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkALLmtncd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkmtn.hpp>
#include <packlib.hpp>
/*ADDLIB tool */
/*ADDLIB mklib */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


APPlib *liblst=NULL,*mtnsavl=NULL,*dir_libs=NULL;
static void getMaintainMasterlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void quit_printMaintainMastercode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printMaintainMastercode Quitting Print RDA Screen Code");
        }
        free_rsrc(prsrc);
}
static void printMaintainMastercode(RDArsrc *prsrc,MaintainMaster *MTNMASTER)
{
	FILE *fp=NULL;
	char *outdevice=NULL;
	char *dirname=NULL,*libname=NULL,*mtnname=NULL;
	int l=0,s=0;

	if(MTNMASTER!=NULL)
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
			if(FINDRSCGETINT(mrsrc,"MAINTAIN MTNMASTERS AVAILABLE",&s))
			{
				return;
			}
			libname=stralloc(liblst->libs[l]);
			mtnname=stralloc(mtnsavl->libs[s]);
#ifndef WIN32
			fp=popen(outdevice,"w");
#endif
#ifdef WIN32
			fp=fopen(outdevice,"a+");
#endif
			makeAllMaintainMastercode(MTNMASTER,fp,dirname,libname,mtnname,1);
			fflush(fp); /* make sure the output is clear */
			if(fp!=NULL)
			{
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
			}
			if(outdevice!=NULL) Rfree(outdevice);
			if(MTNMASTER!=NULL) Rfree(MTNMASTER);
		} else {
			ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
	} else {
		ERRORDIALOG("SCREEN ERROR!","The attempt to retrieve the screen definition failed.",NULL,FALSE);
		prterr("SCREEN RETRIVAL ERROR: The attempt to retrieve the screen definition failed.",NULL,FALSE);
	}
	if(dirname!=NULL) dirname=NULL;
	if(libname!=NULL) libname=NULL;
	if(mtnname!=NULL) mtnname=NULL;
}
static void GetDevicePrintMaintainMastercode(MaintainMaster *MTNMASTER)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printMaintainMastercode,MTNMASTER);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printMaintainMastercode,NULL);
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
static MaintainMaster *okMaintainMaster(char *dirname,char *libname,char *mtnname)
{
	MaintainMaster *MTNMASTER=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER); 
		MTNMASTER=MaintainMasterNew(libname,mtnname);
		dirx=Rmalloc(RDAstrlen(dirname)+2);
#ifndef WIN32
		sprintf(dirx,"%s/",dirname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\",dirname);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.MTN",dirx,libname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.MTN",dirx,libname);
#endif
		if(dirx!=NULL) Rfree(dirx);
		if(getMaintainMaster(libx,MTNMASTER))
		{
			prterr("ERROR: Cannot Read Maintain Master Definition.  Error in permissions or Maintain Master [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,mtnname);
			if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(MTNMASTER);
		}
	} else { 
		return(NULL);
	}
}
void okMaintainMastercb(RDArsrc *mainrsrc)
{
	MaintainMaster *MTNMASTER=NULL;
	int l=0,s=0;
	char *libname=NULL,*mtnname=NULL,*dirname=NULL;

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
	if(FINDRSCGETINT(mrsrc,"MAINTAIN MTNMASTERS AVAILABLE",&s))
	{
		return;
	} else {
		mtnname=stralloc(mtnsavl->libs[s]);
	}
	MTNMASTER=okMaintainMaster(dirname,libname,mtnname);
	if(MTNMASTER!=NULL)
	{
		GetDevicePrintMaintainMastercode(MTNMASTER);
	}
	if(dirname!=NULL) Rfree(dirname);
	if(libname!=NULL) Rfree(libname);
	if(mtnname!=NULL) Rfree(mtnname);
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
	sprintf(libx,"%s/%s.MTN",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.MTN",directory,tmpx);
#endif
	if(tmpx!=NULL) Rfree(tmpx);
	if(FINDRSCGETINT(mainrsrc,"MAINTAIN MTNMASTERS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(mtnsavl->libs[selected],"No Maintain Masters"))
	{
		temp=NULL;
	} else {
		temp=getMaintainMasterdesc(libx,mtnsavl->libs[selected]);
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
	FINDRSCGETINT(mainrsrc,"MAINTAIN MTNMASTERS AVAILABLE",&s);
	freeapplib(mtnsavl);
	if(!RDAstrcmp(liblst->libs[l],"Directory Contains No Libraries"))
	{
		mtnsavl=APPlibNEW();
		addAPPlib(mtnsavl,"No Maintain Masters");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(liblst->libs[l])+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.MTN",dirx,liblst->libs[l]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.MTN",dirx,liblst->libs[l]);
#endif
		mtnsavl=getlibnames(libx,FALSE);
		if(mtnsavl==NULL)
		{
			mtnsavl=APPlibNEW();
			addAPPlib(mtnsavl,"No Maintain Masters");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=mtnsavl->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MAINTAIN MTNMASTERS AVAILABLE",s,mtnsavl))
	{
		updatersrc(mainrsrc,"MAINTAIN MTNMASTERS AVAILABLE");
	}
	setvalues(mainrsrc);
}
static void getMaintainMasterlist(RDArsrc *mainrsrc)
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
	for(x=0;findfile(dirx,"*.MTN",&tmp,(int)x+1);++x)
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
	getMaintainMasterlist(mainrsrc);
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
	getMaintainMasterlist(mainrsrc);
}
void quitMaintainMastercode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(liblst!=NULL) freeapplib(liblst);
	if(mtnsavl!=NULL) freeapplib(mtnsavl);
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
	char *dirname=NULL,*libname=NULL,*mtnname=NULL;
	char *defdir=NULL;
	MaintainMaster *MTNMASTER=NULL;
	int x=0;

	if(argc>2)
	{
		if(argc>3)
		{
			if(InitializeSubsystems(argc,argv,argv[2],"MAKE MAINTAIN MTNMASTER CODE"))
			{
				RDAAPPMAINLOOP();
				return;
			}
		}
		else if(argc==3)
		{
			if(InitializeSubsystems(argc,argv,argv[1],"MAKE MAINTAIN MTNMASTER CODE"))
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
			mtnname=stralloc(argv[2]);
		}
		else
		{
			dirname=stralloc(argv[1]);
			libname=stralloc(argv[2]);
			mtnname=stralloc(argv[3]);
		}
		MTNMASTER=okMaintainMaster(dirname,libname,mtnname);
		if(MTNMASTER!=NULL)
		{
			makeAllMaintainMastercode(MTNMASTER,stdout,dirname,libname,mtnname,1);
		}
		if(MTNMASTER!=NULL) Rfree(MTNMASTER);
		if(dirname!=NULL) Rfree(dirname);
		if(libname!=NULL) Rfree(libname);
		if(mtnname!=NULL) Rfree(mtnname);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE MAINTAIN MTNMASTER CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE MAINTAIN MTNMASTER CODE");
		liblst=APPlibNEW();
		mtnsavl=APPlibNEW();
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
		addlstrsrc(mrsrc,"MAINTAIN MTNMASTERS AVAILABLE",&x,TRUE,setvalues,mtnsavl->numlibs,&mtnsavl->libs,NULL);
		addsctrsrc(mrsrc,"DESCRIPTION",0,NULL,FALSE);
		getdir(mrsrc);
		if(argc==2)
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
		else
		{
			FINDRSCLISTAPPlib(mrsrc,"MODULE LIST",l,liblst);
			doliblist(mrsrc);
		}
		getMaintainMasterlist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"MODULE LIST",l,liblst);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"MAINTAIN MTNMASTERS AVAILABLE",s,mtnsavl);
		setvalues(mrsrc);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okMaintainMastercb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitMaintainMastercode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"MODULE LIST");
		APPmakescrn(mrsrc,TRUE,quitMaintainMastercode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
