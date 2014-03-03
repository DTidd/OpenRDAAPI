/* mkALLimpcd.c - Generate Import/Export C Code for speed of development */
#ifndef WIN32
#define __NAM__ "mkALLimpcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkALLimpcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkimp.hpp>
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
static void getscreenlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

/*
static char *fixdataname(char *name,char frtype,char totype)
{
	int str_length=0,x;
	char *new_name=NULL;

	if(!isEMPTY(name))
	{
		str_length=RDAstrlen(name);
		new_name=Rmalloc(str_length+1);
		memset(new_name,0,str_length+1);
		for(x=0;x<str_length;++x)
		{
			if(name[x]==frtype)
			{
				new_name[x]=totype;
			} else {
				new_name[x]=name[x];
			}
		}
	}
	return(new_name);
}
*/
static void makeALLIMPEXPcode(IEfile *IMPEXP,FILE *fp,char *libname,char *name)
{
	FILE *s_fp;
	char *temp1=NULL;

	if(fp!=NULL)
	{
		makeIMPEXPcode(IMPEXP,fp,libname,name);

		/* Pack change screen if one is used */
		if(IMPEXP->range)
		{
			temp1=Rmalloc(RDAstrlen(IMPEXP->name)+28+1);
			sprintf(temp1,"%s IMPORT/EXPORT RANGE SCREEN",IMPEXP->name);
			/* Please note "rda" will be overridden by "site" if a screen in "site" is available */
			pack_report_screen(fp,"rda",libname,temp1,1,1,0);
			if(temp1!=NULL) Rfree(temp1);
		}

		/* Make default change menu and pack it */
		makeDefaultIMPEXPMenu(libname,IMPEXP);
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(40+RDAstrlen(libname)+RDAstrlen(IMPEXP->name)+1);
		sprintf(temp1,"%s %s",libname,IMPEXP->name);
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
static void quit_printIEfilecode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printIEfilecode Quitting Print Import/Export Code");
        }
        free_rsrc(prsrc);
}
static void printIEfilecode(RDArsrc *prsrc,IEfile *IMPEXP)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*name=NULL;
	int l=0,s=0;

	if(IMPEXP!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
			if(FINDRSCGETINT(mrsrc,"MODULE LIST",&l))
			{
				return;
			}
			if(FINDRSCGETINT(mrsrc,"DEFINITIONS AVAILABLE",&s))
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
				name=stralloc(scr_defs->libs[s]);
				makeALLIMPEXPcode(IMPEXP,fp,libname,name);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(IMPEXP!=NULL) free_import(IMPEXP);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintIEfile popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
				if(dirname!=NULL) dirname=NULL;
				if(outdevice!=NULL) Rfree(outdevice);
			}
		} else {
			ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
	} else {
		ERRORDIALOG("SCREEN ERROR!","The attempt to retrieve the import/export definition failed.",NULL,FALSE);
		prterr("SCREEN RETRIVAL ERROR: The attempt to retrieve the import/export definition failed.",NULL,FALSE);
	}
	if(libname!=NULL) libname=NULL;
	if(name!=NULL) name=NULL;
}
static void GetDevicePrintIEfilecode(IEfile *IMPEXP)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printIEfilecode,IMPEXP);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printIEfilecode,NULL);
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
static IEfile *okie(char *libname,char *name)
{
	IEfile *IMPEXP=NULL;

	if(RDAstrcmp(libname,"Contains No Libraries"))
	{
		IMPEXP=IMPORTstpNEW(name);
		if(getIMPORTbin(libname,IMPEXP))
		{
			prterr("ERROR: Cannot Read Import/Export Definition.  Error in permissions or definition [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,name);
			if(IMPEXP!=NULL) free_import(IMPEXP);
			return(NULL);
		} else {
			return(IMPEXP);
		}
	} else { 
		return(NULL);
	}
}
void okiecb(RDArsrc *mainrsrc)
{
	IEfile *IMPEXP=NULL;
	int l=0,s=0;
	char *libname=NULL,*name=NULL;

	if(FINDRSCGETINT(mrsrc,"MODULE LIST",&l))
	{
		return;
	} else {
		libname=stralloc(scr_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"DEFINITIONS AVAILABLE",&s))
	{
		return;
	} else {
		name=stralloc(scr_defs->libs[s]);
	}
	IMPEXP=okie(libname,name);
	if(IMPEXP!=NULL)
	{
		GetDevicePrintIEfilecode(IMPEXP);
	}
	if(libname!=NULL) Rfree(libname);
	if(name!=NULL) Rfree(name);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&s);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	freeapplib(scr_defs);
	if(!RDAstrcmp(scr_libs->libs[l],"Contains No Libraries"))
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Definitions");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+6);
		sprintf(libx,"%s/%s.IMP",dirx,scr_libs->libs[l]);
		scr_defs=getlibnames(libx,FALSE);
		if(scr_defs==NULL)
		{
			scr_defs=APPlibNEW();
			addAPPlib(scr_defs,"Contains No Definitions");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=scr_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",s,scr_defs))
	{
		updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
	}
}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int l=0,x=0;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&l))
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
	for(x=0;findfile(dirx,"*.IMP",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(scr_libs,tmp);
	}
	if(scr_libs->numlibs<1)
	{
		addAPPlib(scr_libs,"Contains No Libraries");
	} else {
		SORTAPPlib(scr_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(l>=scr_libs->numlibs) l=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",l,scr_libs))
	{
		updatersrc(mainrsrc,"MODULE LIST");
	}
	doliblist(mainrsrc);
}
void quitIEfilecode(RDArsrc *r)
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
	char *libname=NULL,*name=NULL;
	IEfile *IMPEXP=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE IMPORT EXPORT CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		libname=stralloc(argv[1]);
		name=stralloc(argv[2]);
		IMPEXP=okie(libname,name);
		if(IMPEXP!=NULL)
		{
			makeALLIMPEXPcode(IMPEXP,stdout,libname,name);
			free_import(IMPEXP);
		}
		if(libname!=NULL) Rfree(libname);
		if(name!=NULL) Rfree(name);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE IMPORT EXPORT CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE IMPORT EXPORT CODE");
		scr_libs=APPlibNEW();
		scr_defs=APPlibNEW();
		addlstrsrc(mrsrc,"MODULE LIST",&l,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
		addlstrsrc(mrsrc,"DEFINITIONS AVAILABLE",&s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
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
		FINDRSCLISTAPPlib(mrsrc,"MODULE LIST",l,scr_libs);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"DEFINITIONS AVAILABLE",s,scr_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okiecb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitIEfilecode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"MODULE LIST");
		APPmakescrn(mrsrc,TRUE,quitIEfilecode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
