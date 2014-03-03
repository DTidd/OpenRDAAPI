/* mkprtcd.c - Generate Printer Definition C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkprtcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkprtcd.exe"
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


APPlib *scr_defs=NULL;
char *module=NULL;
RDArsrc *mrsrc=NULL;

static short chkforsym(char *name)
{
	int s=0,x=0;

	if(!isEMPTY(name))
	{
		s=RDAstrlen(name);
		for(x=0;x<s;++x)
		{
			if(name[x]==016) return(-1);
		}
	}
	return(0);
}
static char *fixcmdx(char *name)
{
	int str_length=0,x;
	char *new_name=NULL,*tmp_name=NULL;

	if(name!=NULL)
	{
		str_length=RDAstrlen(name);
		tmp_name=stralloc(name);
		for(x=0;x<str_length;++x) 
		{
			if(tmp_name[x]=='\\') tmp_name[x]=016;
		}
		while(chkforsym(tmp_name))
		{
			for(x=0;x<str_length;++x)
			{
				if(tmp_name[x]==016)
				{
					new_name=Rmalloc(str_length+4);
					memset(new_name,0,str_length+3);
					if(x>0)
					{
						sprintf(new_name,"%.*s\\\\%s",x,&tmp_name[0],&tmp_name[x+1]);
					} else {
						sprintf(new_name,"\\\\%s",&tmp_name[x+1]);
					}
					if(tmp_name!=NULL) Rfree(tmp_name);
					tmp_name=stralloc(new_name);
					str_length=RDAstrlen(tmp_name);
					if(new_name!=NULL) Rfree(new_name);
					break;
				}
			}
		}
		if(tmp_name!=NULL) Rfree(tmp_name);
	}
	return(new_name);
}
static void makePRTDEFcode(RDApdev *PRTDEF,FILE *fp,char *libname,char *name)
{
	int x=0;
	RDApvar *field=NULL;
	char *temp=NULL,*cmd=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"/* Printer Definition (%s)  */\n",PRTDEF->name);
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"#include <rptgen.hpp>\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		temp=fixname(PRTDEF->name);
		fprintf(fp,"\n");
		fprintf(fp,"static void PRT_%s()\n",temp);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"{\n");
		fprintf(fp,"\tRDApdev *PRTDEF=NULL;\n");
		fprintf(fp,"\tchar *temp1=NULL;\n");
		fprintf(fp,"\tchar *temp2=NULL;\n");
		fprintf(fp,"\n");

		fprintf(fp,"\tPRTDEF=RDApdevNEW(\"%s\");\n",
			(name!=NULL ? name:""));
		fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");
		fprintf(fp,"\tif(PRTDEF!=NULL)\n");
		fprintf(fp,"\t{\n");
		if(PRTDEF->pvar!=NULL)
		{
			for(x=0,field=PRTDEF->pvar;x<PRTDEF->num;++x,++field)
			{
				cmd=fixcmdx(field->command);
				fprintf(fp,"\t\taddRDApvar(PRTDEF,\"%s\",\"%s\",%d,%d,%d);\n",
					(field->name!=NULL ? field->name:""),
					(cmd!=NULL ? cmd:""),(field->chgsize ? 1:0),
					field->pitch,field->points);
				if(cmd!=NULL) Rfree(cmd);
			}
		}
		fprintf(fp,"\t\tif(writepdevbin(PRTDEF))\n");
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+110+1);\n",RDAstrlen(PRTDEF->name));
		fprintf(fp,"\t\t\tsprintf(temp1,\"PRINTER DEFINITION WRITE ERROR: Definition [%s], Can Not Save Printer Definition!\");\n",(PRTDEF->name!=NULL?PRTDEF->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\tif(temp2!=NULL) Rfree(temp2);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tif(PRTDEF!=NULL) free_pdev(PRTDEF);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");
	}
}
static void quit_printRDApdevcode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDApdevcode Quitting Print Import/Export Code");
        }
        free_rsrc(prsrc);
}
static void printRDApdevcode(RDArsrc *prsrc,RDApdev *PRTDEF)
{
	char *outdevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*name=NULL;
	int s=0;

	if(PRTDEF!=NULL)
	{
		readwidget(prsrc,"DEVICE");
		FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
		if(outdevice!=NULL)
		{
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
				name=stralloc(scr_defs->libs[s]);
				makePRTDEFcode(PRTDEF,fp,libname,name);
#ifndef WIN32
				pclose(fp);
#endif
#ifdef WIN32
				fclose(fp);
#endif
				if(PRTDEF!=NULL) free_pdev(PRTDEF);
			} else {
				ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDApdev popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
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
static void GetDevicePrintRDApdevcode(RDApdev *PRTDEF)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDApdevcode,PRTDEF);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDApdevcode,NULL);
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
static RDApdev *okprt(char *name)
{
	RDApdev *PRTDEF=NULL;

	if(RDAstrcmp(name,"Contains No Definitions"))
	{
		PRTDEF=RDApdevNEW(name);
		if(getpdevbin(PRTDEF))
		{
			prterr("ERROR: Cannot Read Printer Definition.  Error in permissions or definition [%s] doesn't exist.  Check the permissions, and retry.",name);
			if(PRTDEF!=NULL) free_pdev(PRTDEF);
			return(NULL);
		} else {
			return(PRTDEF);
		}
	} else { 
		return(NULL);
	}
}
void okprtcb(RDArsrc *mainrsrc)
{
	RDApdev *PRTDEF=NULL;
	int s=0;
	char *name=NULL;

	if(FINDRSCGETINT(mrsrc,"DEFINITIONS AVAILABLE",&s))
	{
		return;
	} else {
		name=stralloc(scr_defs->libs[s]);
	}
	PRTDEF=okprt(name);
	if(PRTDEF!=NULL)
	{
		GetDevicePrintRDApdevcode(PRTDEF);
	}
	if(name!=NULL) Rfree(name);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirx=NULL;

	FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&s);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+18);
#ifndef WIN32
	sprintf(dirx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	if(scr_defs!=NULL) freeapplib(scr_defs);
	scr_defs=getlibnames(dirx,FALSE);
	if(scr_defs==NULL)
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Definitions");
		s=0;
	}
	if(dirx!=NULL) Rfree(dirx);
	if(s>=scr_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",s,scr_defs))
	{
		updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
	}
}
void quitRDApdevcode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int s=0;
	char *libname=NULL,*name=NULL;
	RDApdev *PRTDEF=NULL;
	char *temp=NULL;

	if(argc>1)
	{
		if(InitializeSubsystems(argc,argv,"TOOLS","MAKE PRINTER DEFINITION CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		name=stralloc(argv[1]);
		PRTDEF=okprt(name);
		if(PRTDEF!=NULL)
		{
			makePRTDEFcode(PRTDEF,stdout,libname,name);
			free_pdev(PRTDEF);
		}
		if(libname!=NULL) Rfree(libname);
		if(name!=NULL) Rfree(name);
		ShutdownSubsystems();
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE PRINTER DEFINITION CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		mrsrc=RDArsrcNEW("TOOLS","MAKE PRINTER DEFINITION CODE");
		scr_defs=APPlibNEW();
		addlstrsrc(mrsrc,"DEFINITIONS AVAILABLE",&s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
		doliblist(mrsrc);
		FINDRSCLISTAPPlib(mrsrc,"DEFINITIONS AVAILABLE",s,scr_defs);
		addrfcbrsrc(mrsrc,"SELECT",TRUE,okprtcb,NULL);
		addrfexrsrc(mrsrc,"QUIT",TRUE,quitRDApdevcode,NULL);
		addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mrsrc,"DEFINITIONS AVAILABLE");
		APPmakescrn(mrsrc,TRUE,quitRDApdevcode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
