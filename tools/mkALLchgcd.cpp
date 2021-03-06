/* mkALLchgcd.c - Generate Change File C Code for speed of development */
#ifndef WIN32
#define __NAM__ "mkALLchgcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkALLchgcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkchg.hpp>
#include <mkmsc.hpp>
#include <packlib.hpp>
/*ADDLIB chglib */
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


APPlib *liblst=NULL,*addsavl=NULL,*dir_libs=NULL;
static void getaddlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

void makeAllChangecode(CHGstruct *CHGDEF,FILE *fp,char *dirname,char *libname,char *chgname)
{
	FILE *s_fp;
	char *temp1=NULL;

	if(fp!=NULL)
	{
		makechgcode(CHGDEF,fp,dirname,libname,chgname);

		/* Pack change screen if one is used */
		if(CHGDEF->range_screen)
		{
			temp1=Rmalloc(RDAstrlen(CHGDEF->name)+14+1);
			sprintf(temp1,"%s RANGE SCREEN",CHGDEF->name);
			/* Please note "rda" will be overridden by "site" if a screen in "site" is available */
			pack_report_screen(fp,"rda",libname,temp1,1,1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

		/* Make default change menu and pack it */
		makeDefaultChangeMenu(libname,CHGDEF);
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(40+RDAstrlen(libname)+RDAstrlen(CHGDEF->name)+1);
		sprintf(temp1,"%s %s",libname,CHGDEF->name);
		pack_menu(fp,"rda",libname,temp1);
		if(temp1!=NULL) Rfree(temp1);

		s_fp=fopen("packedscreens.list","a");
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);
		s_fp=fopen("scndefs.list","a");
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);
		s_fp=fopen("packedlists.list","a");
		fprintf(s_fp,"\n"); /* print a blank line to separate things in the list file */
		fclose(s_fp);
	}
}
static void quit_printChangeFilecode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printChangeFilecode Quitting Print RDA Screen Code");
        }
        free_rsrc(prsrc);
}
static void printChangeFilecode(RDArsrc *prsrc,CHGstruct *CHGDEF)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*chgname=NULL;
	int l=0,s=0;

	if(CHGDEF!=NULL)
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
			if(FINDRSCGETINT(mrsrc,"CHANGES AVAILABLE",&s))
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
				chgname=stralloc(addsavl->libs[s]);
				makeAllChangecode(CHGDEF,fp,dirname,libname,chgname);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(CHGDEF!=NULL) Rfree(CHGDEF);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintChangeFilecode popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
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
	if(chgname!=NULL) chgname=NULL;
}
static void GetDevicePrintChangeFilecode(CHGstruct *CHGDEF)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printChangeFilecode,CHGDEF);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printChangeFilecode,NULL);
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
static CHGstruct *okChangeFile(char *dirname,char *libname,char *chgname)
{
	CHGstruct *CHGDEF=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(CHGDEF!=NULL) FreeCHGstruct(CHGDEF);
		CHGDEF=CHGstructNEW(libname,chgname);
		dirx=Rmalloc(RDAstrlen(dirname)+2);
#ifndef WIN32
		sprintf(dirx,"%s/",dirname);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\",dirname);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.CHG",dirx,libname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.CHG",dirx,libname);
#endif
		if(dirx!=NULL) Rfree(dirx);
		if(getCHGstruct(libx,CHGDEF))
		{
			prterr("ERROR: Cannot Read Change File Definition.  Error in permissions or Change File [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,chgname);
			if(CHGDEF!=NULL) FreeCHGstruct(CHGDEF);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(CHGDEF);
		}
	} else { 
		return(NULL);
	}
}
void okChangeFilecb(RDArsrc *mainrsrc)
{
	CHGstruct *CHGDEF=NULL;
	int l=0,s=0;
	char *libname=NULL,*chgname=NULL,*dirname=NULL;

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
	if(FINDRSCGETINT(mrsrc,"CHANGES AVAILABLE",&s))
	{
		return;
	} else {
		chgname=stralloc(addsavl->libs[s]);
	}
	CHGDEF=okChangeFile(dirname,libname,chgname);
	if(CHGDEF!=NULL)
	{
		GetDevicePrintChangeFilecode(CHGDEF);
	}
	if(dirname!=NULL) Rfree(dirname);
	if(libname!=NULL) Rfree(libname);
	if(chgname!=NULL) Rfree(chgname);
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
	sprintf(libx,"%s/%s.CHG",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.CHG",directory,tmpx);
#endif
	if(tmpx!=NULL) Rfree(tmpx);
	if(FINDRSCGETINT(mainrsrc,"CHANGES AVAILABLE",&selected)) return;
	if(!RDAstrcmp(addsavl->libs[selected],"No Change Definitions"))
	{
		temp=NULL;
	} else {
		temp=getCHGstructdesc(libx,addsavl->libs[selected]);
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
	FINDRSCGETINT(mainrsrc,"CHANGES AVAILABLE",&s);
	freeapplib(addsavl);
	if(!RDAstrcmp(liblst->libs[l],"Directory Contains No Libraries"))
	{
		addsavl=APPlibNEW();
		addAPPlib(addsavl,"No Change Definitions");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(liblst->libs[l])+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.CHG",dirx,liblst->libs[l]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.CHG",dirx,liblst->libs[l]);
#endif
		addsavl=getlibnames(libx,FALSE);
		if(addsavl==NULL)
		{
			addsavl=APPlibNEW();
			addAPPlib(addsavl,"No Change Definitions");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=addsavl->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"CHANGES AVAILABLE",s,addsavl))
	{
		updatersrc(mainrsrc,"CHANGES AVAILABLE");
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
	for(x=0;findfile(dirx,"*.CHG",&tmp,(int)x+1);++x)
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
void quitmtncode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(liblst!=NULL) freeapplib(liblst);
	if(addsavl!=NULL) freeapplib(addsavl);
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
	char *dirname=NULL,*libname=NULL,*chgname=NULL;
	char *defdir=NULL;
	CHGstruct *CHGDEF=NULL;
	int x=0;

	if(argc>2)
	{
		if(argc>3)
		{
			if(InitializeSubsystems(argc,argv,argv[2],"MAKE CHANGE FILE CODE"))
			{
				RDAAPPMAINLOOP();
				return;
			}
		}
		else if(argc==3)
		{
			if(InitializeSubsystems(argc,argv,argv[1],"MAKE CHANGE FILE CODE"))
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
			chgname=stralloc(argv[2]);
		}
		else
		{
			dirname=stralloc(argv[1]);
			libname=stralloc(argv[2]);
			chgname=stralloc(argv[3]);
		}
		CHGDEF=okChangeFile(dirname,libname,chgname);
		if(CHGDEF!=NULL)
		{
			makeAllChangecode(CHGDEF,stdout,dirname,libname,chgname);
		}
		if(CHGDEF!=NULL) FreeCHGstruct(CHGDEF);
		if(dirname!=NULL) Rfree(dirname);
		if(libname!=NULL) Rfree(libname);
		if(chgname!=NULL) Rfree(chgname);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE CHANGE CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE CHANGE CODE");
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
		addlstrsrc(mrsrc,"CHANGES AVAILABLE",&x,TRUE,setvalues,addsavl->numlibs,&addsavl->libs,NULL);
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
		FINDRSCLISTAPPlib(mrsrc,"MODULE LIST",l,liblst);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"CHANGES AVAILABLE",s,addsavl);
		setvalues(mrsrc);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okChangeFilecb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitmtncode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"MODULE LIST");
		APPmakescrn(mrsrc,TRUE,quitmtncode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
