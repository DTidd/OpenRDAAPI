/* mkgsvcd.c - Generate Setup Variable C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkgsvcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkgsvcd.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <mkmsc.hpp>
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


APPlib *scr_libs=NULL,*scr_defs=NULL;
static void getscreenlist(RDArsrc *);
char *module=NULL;
RDArsrc *mrsrc=NULL;

static void makegsvcode(RDAGenericSetup *gsv,FILE *fp,char *libname,char *gsvname)
{
	char *temp=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"/* %s's - Make (%s) List */\n",gsv->module,gsv->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mix.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		temp=fixname(gsv->name);
		fprintf(fp,"static void GSV_%s(void);\n",temp);
		fprintf(fp,"\n");
		fprintf(fp,"static void GSV_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDAGenericSetup *gsv=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL,*libx=NULL;\n");

		fprintf(fp,"\n");
		fprintf(fp,"\tlibx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);\n");
		fprintf(fp,"#ifndef WIN32\n");
		fprintf(fp,"\tsprintf(libx,\"%%s/rda/%%s.GSV\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"#ifdef WIN32\n");
		fprintf(fp,"\tsprintf(libx,\"%%s\\\\rda\\\\%%s.GSV\",CURRENTDIRECTORY,module);\n");
		fprintf(fp,"#endif\n");
		fprintf(fp,"\n");

		fprintf(fp,"\tgsv=RDAGenericSetupNew(\"%s\",\"%s\");\n",(libname!=NULL ? libname:""),(gsvname!=NULL ? gsvname:""));
		fprintf(fp,"\tif(gsv!=NULL)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tif(getRDAGenericSetupbin(libx,gsv))\n");
		fprintf(fp,"\t\t{\n");
		fprintf(fp,"\t\t\tgsv->type=%d;\n",gsv->type);
		fprintf(fp,"\t\t\tgsv->length=%d;\n",gsv->length);
		fprintf(fp,"\t\t\tgsv->desc=stralloc(\"%s\");\n",(gsv->desc!=NULL ? gsv->desc:""));
		fprintf(fp,"\t\t\tgsv->label=stralloc(\"%s\");\n",(gsv->label!=NULL ? gsv->label:""));
		switch(gsv->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				fprintf(fp,"\t\t\tgsv->value.string_value=stralloc(\"%s\");\n",(gsv->value.string_value!=NULL ? gsv->value.string_value:""));
				break;
			case SHORTV:
			case SSHORTV:
				fprintf(fp,"\t\t\tgsv->value.short_value=Rmalloc(sizeof(short));\n");
				fprintf(fp,"\t\t\t*gsv->value.short_value=%d;\n",*gsv->value.short_value);
				break;
			case LONGV:
			case SLONGV:
				fprintf(fp,"\t\t\tgsv->value.integer_value=Rmalloc(sizeof(int));\n");
				fprintf(fp,"\t\t\t*gsv->value.integer_value=%d;\n",*gsv->value.integer_value);
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				fprintf(fp,"\t\t\tgsv->value.float_value=Rmalloc(sizeof(double));\n");
				fprintf(fp,"\t\t\t*gsv->value.float_value=%f;\n",*gsv->value.float_value);
				break;
			case BOOLNS:
				fprintf(fp,"\t\t\tgsv->value.string_value=Rmalloc(1);\n");
				fprintf(fp,"\t\t\t*gsv->value.string_value=%d;\n",(*gsv->value.string_value ? TRUE:FALSE));
				break;
			case CHARACTERS:
				fprintf(fp,"\t\t\tgsv->value.string_value=Rmalloc(1);\n");
				fprintf(fp,"\t\t\t*gsv->value.string_value=%c;\n",(*gsv->value.string_value));
				break;
			default:
				prterr("Error Unrecognized Field type [%d] for resource [%s].",gsv->type,gsv->name); 
				break;
		}
		fprintf(fp,"\t\n");
		fprintf(fp,"\t\t\tif(writeRDAGenericSetupbin(libx,gsv))\n");
		fprintf(fp,"\t\t\t{\n");

		fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\t\tif(libx!=NULL) Rfree(libx);\n");
		fprintf(fp,"\t\t\t\ttemp1=Rmalloc(%d+%d+110+1);\n",RDAstrlen(gsv->name),RDAstrlen(libname));
		fprintf(fp,"\t\t\t\tsprintf(temp1,\"GENERIC SETUP VARIABLE WRITE ERROR: Module [%s] GSV [%s], Can Not Save Generic Setup Variable!\");\n",(libname!=NULL?libname:""),(gsv->name!=NULL?gsv->name:""));
		fprintf(fp,"\t\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t\t{\n");
		fprintf(fp,"\t\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t\t}\n");
		fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"\tif(libx!=NULL) Rfree(libx);\n");
		fprintf(fp,"\tif(gsv!=NULL) FreeRDAGenericSetup(gsv);\n");
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
static void printRDAscrncode(RDArsrc *prsrc,RDAGenericSetup *scrn)
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
			if(FINDRSCGETINT(mrsrc,"GENERIC SETUP VARIABLES",&s))
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
				makegsvcode(scrn,fp,libname,scrnname);
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
static void GetDevicePrintRDAscrncode(RDAGenericSetup *scrn)
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
static RDAGenericSetup *okgsv(char *libname,char *scrnname)
{
	RDAGenericSetup *scrn=NULL;
	char *libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(scrn!=NULL) FreeRDAGenericSetup(scrn); 
		scrn=RDAGenericSetupNew(libname,scrnname);
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(libname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,(libname!=NULL ? libname:""));
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,(libname!=NULL ? libname:""));
#endif
		if(getRDAGenericSetupbin(libx,scrn))
		{
			prterr("ERROR: Cannot Read RDAGenericSetup Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,scrnname);
			if(scrn!=NULL) FreeRDAGenericSetup(scrn);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(scrn);
		}
	} else { 
		return(NULL);
	}
}
void okgsvcb(RDArsrc *mainrsrc)
{
	RDAGenericSetup *scrn=NULL;
	int l=0,s=0;
	char *libname=NULL,*scrnname=NULL;

	if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
	{
		return;
	} else {
		libname=stralloc(scr_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"GENERIC SETUP VARIABLES",&s))
	{
		return;
	} else {
		scrnname=stralloc(scr_defs->libs[s]);
	}
	scrn=okgsv(libname,scrnname);
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
	FINDRSCGETINT(mainrsrc,"GENERIC SETUP VARIABLES",&s);
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
		sprintf(libx,"%s/%s.GSV",dirx,scr_libs->libs[l]);
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
	if(!FINDRSCLISTAPPlib(mainrsrc,"GENERIC SETUP VARIABLES",s,scr_defs))
	{
		updatersrc(mainrsrc,"GENERIC SETUP VARIABLES");
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
	for(x=0;findfile(dirx,"*.GSV",&tmp,(int)x+1);++x)
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
	RDAGenericSetup *scrn=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE GENERIC SETUP VARIABLE CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		libname=stralloc(argv[1]);
		scrnname=stralloc(argv[2]);
		scrn=okgsv(libname,scrnname);
		if(scrn!=NULL)
		{
			makegsvcode(scrn,stdout,libname,scrnname);
		}
		if(scrn!=NULL) Rfree(scrn);
		if(libname!=NULL) Rfree(libname);
		if(scrnname!=NULL) Rfree(scrnname);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE GENERIC SETUP VARIABLE CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE GENERIC SETUP VARIABLE CODE");
		scr_libs=APPlibNEW();
		scr_defs=APPlibNEW();
		addlstrsrc(mrsrc,"LIBRARY LIST",&l,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
		addlstrsrc(mrsrc,"GENERIC SETUP VARIABLES",&s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
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
		FINDRSCLISTAPPlib(mrsrc,"GENERIC SETUP VARIABLES",s,scr_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okgsvcb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitscrncode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"LIBRARY LIST");
		APPmakescrn(mrsrc,TRUE,quitscrncode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
