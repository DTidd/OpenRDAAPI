/* mktablecd.c - Generate RDA Table C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mktablecd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mktablecd.exe"
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
static void getscreenlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void maketablecode(RDATABLE *scrn,FILE *fp,char *libname,char *scrnname)
{
	int q=0;
	char *temp=NULL;
	TABLEVALUE *v=NULL;

	if(fp!=NULL)
	{	
		temp=fixname(scrn->name);
#ifdef PAIN_IN_
		fprintf(fp,"/* %s's - Make (%s) List */\n",scrn->module,scrn->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		fprintf(fp,"\n");
#endif /* PAIN_IN_ */
		fprintf(fp,"static void TBL_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDATABLE *scrn=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");

		fprintf(fp,"\n");

		fprintf(fp,"\tscrn=RDATABLENEW(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"\tif(scrn!=NULL)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tscrn->title1=stralloc(\"%s\");\n",(scrn->title1!=NULL ? scrn->title1:""));
		fprintf(fp,"\t\tscrn->title2=stralloc(\"%s\");\n",(scrn->title2!=NULL ? scrn->title2:""));
		fprintf(fp,"\t\tscrn->desc=stralloc(\"%s\");\n",(scrn->desc!=NULL ? scrn->desc:""));
		fprintf(fp,"\t\tscrn->desc=stralloc(\"%s\");\n",(scrn->desc!=NULL ? scrn->desc:""));
		fprintf(fp,"\t\tscrn->inclusive=%d;\n",(scrn->inclusive ? TRUE:FALSE));
		if(scrn->values!=NULL)
		{
			for(q=0,v=scrn->values;q<scrn->num;++q,++v)
			{
				fprintf(fp,"\t\taddRDATableValue(scrn,%f,%f,%f);\n",v->amount,v->base,v->rate);
			}
		}

		fprintf(fp,"\n");
		fprintf(fp,"\t\tif(ADVwriteRDATablebin(\"%s\",scrn))\n",(libname!=NULL ? libname:""));
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+110+1);\n",RDAstrlen(scrn->name),RDAstrlen(module));
		fprintf(fp,"\t\t\tsprintf(temp1,\"TABLE WRITE ERROR: Module [%s] Table [%s], Can Not Save Table!\");\n",(module!=NULL?module:""),(scrn->name!=NULL?scrn->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(scrn!=NULL) FreeRDATABLE(scrn);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");
	}
}
static void quit_printRDAscrncode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDAscrncode Quitting Print RDA Screen Code");
        }
        free_rsrc(prsrc);
}
static void printRDAscrncode(RDArsrc *prsrc,RDATABLE *scrn)
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
			if(FINDRSCGETINT(mrsrc,"TABLES",&s))
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
				maketablecode(scrn,fp,libname,scrnname);
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
static void GetDevicePrintRDAscrncode(RDATABLE *scrn)
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
static RDATABLE *oktable(char *libname,char *module,char *scrnname)
{
	RDATABLE *scrn=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(scrn!=NULL) FreeRDATABLE(scrn); 
		scrn=RDATABLENEW(module,scrnname);
		if(getRDATablebin(libname,scrn))
		{
			prterr("ERROR: Cannot Read RDATABLE Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,scrnname);
			if(scrn!=NULL) FreeRDATABLE(scrn);
			return(NULL);
		} else {
			return(scrn);
		}
	} else { 
		return(NULL);
	}
}
void oktablecb(RDArsrc *mainrsrc)
{
	RDATABLE *scrn=NULL;
	int l=0,s=0;
	char *libname=NULL,*scrnname=NULL;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
	{
		return;
	} else {
		libname=stralloc(scr_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"TABLES",&s))
	{
		return;
	} else {
		scrnname=stralloc(scr_defs->libs[s]);
	}
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+6);
	sprintf(libx,"%s/%s.TBL",dirx,scr_libs->libs[l]);
	scrn=oktable(libx,libname,scrnname);
	if(scrn!=NULL)
	{
		GetDevicePrintRDAscrncode(scrn);
	}
	if(libname!=NULL) Rfree(libname);
	if(scrnname!=NULL) Rfree(scrnname);
	if(libx!=NULL) Rfree(libx);
	if(dirx!=NULL) Rfree(dirx);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"TABLES",&s);
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
		sprintf(libx,"%s/%s.TBL",dirx,scr_libs->libs[l]);
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
	if(!FINDRSCLISTAPPlib(mainrsrc,"TABLES",s,scr_defs))
	{
		updatersrc(mainrsrc,"TABLES");
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
	for(x=0;findfile(dirx,"*.TBL",&tmp,(int)x+1);++x)
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
	exit(0);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int l=0,s=0;
	char *libname=NULL,*scrnname=NULL;
	RDATABLE *scrn=NULL;
	char *libx=NULL;
	char *dirx=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE TABLE CODE"))
		{
			RDAAPPMAINLOOP();
			return(-1);
		}
		libname=stralloc(argv[1]);
		scrnname=stralloc(argv[2]);
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
		sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.TBL",dirx,libname);
		scrn=oktable(libx,libname,scrnname);
		if(scrn!=NULL)
		{
			maketablecode(scrn,stdout,libname,scrnname);
		}
		if(scrn!=NULL) Rfree(scrn);
		if(libname!=NULL) Rfree(libname);
		if(scrnname!=NULL) Rfree(scrnname);
		if(dirx!=NULL) Rfree(dirx);
		if(libx!=NULL) Rfree(libx);
		ShutdownSubsystems();
		exit(0);
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE TABLE CODE"))
		{
			ShutdownSubsystems();
			exit(1);
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE TABLE CODE");
		scr_libs=APPlibNEW();
		scr_defs=APPlibNEW();
		addlstrsrc(mrsrc,"LIBRARY LIST",&l,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
		addlstrsrc(mrsrc,"TABLES",&s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
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
		if(argc>2)
		{
			for(s=0;s<scr_defs->numlibs;++s)
			{
				if(!RDAstrcmp(argv[2],scr_defs->libs[s])) break;
			}
			if(s>=scr_defs->numlibs)
			{
				s=0;
			}
		}
		FINDRSCLISTAPPlib(mrsrc,"TABLES",s,scr_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,oktablecb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitscrncode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"LIBRARY LIST");
		APPmakescrn(mrsrc,TRUE,quitscrncode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
