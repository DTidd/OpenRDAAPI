/* makeALLrptcode.c - Generate Report C Code for speed of development */
#ifndef WIN32
#define __NAM__ "mkALLrptcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkALLrptcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkmtn.hpp>
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


APPlib *rptavl=NULL,*modlstx=NULL;
RDArsrc *mainrsrc=NULL;

static void makeAllReportcode(RDAreport *r,FILE *fp,char *module)
{
	RDAreport *chainrpt=NULL;
	FILE *s_fp=NULL;
	char *temp1=NULL;
	short has_chain_report;

	if(fp!=NULL)
	{
		/* Pack report */
		makerptcode(r,fp,module,1);
		/* Pack the chain report if there is one */
		has_chain_report = r->chain_report;
		chainrpt=r;
		while(has_chain_report)
		{
			chainrpt=GETRDAreport(chainrpt->chain_module,chainrpt->chain_name);
			if(chainrpt==NULL)
			{
				ERRORDIALOG("Cannot Read Report","Error in permissions, users doesn't have access to read the chain report definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
				return;
			}
			else
			{
				makerptcode(chainrpt,fp,r->chain_module,0);
				has_chain_report = chainrpt->chain_report;
			}
		}

		/* Pack range screen if one is used */
		if(r->range_screen)
		{
			temp1=Rmalloc(RDAstrlen(r->name)+14+1);
			sprintf(temp1,"%s RANGE SCREEN",r->name);
			/* Please note "rda" will be overridden by "site" if a screen in "site" is available */
			pack_report_screen(fp,"rda",module,temp1,1,1,0);
			if(temp1!=NULL) Rfree(temp1);
		}
		/* Make default menu and pack it */
		makeDefaultReportMenu(module,r);
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(40+RDAstrlen(module)+RDAstrlen(r->name)+1);
		sprintf(temp1,"%s %s",module,r->name);
		pack_menu(fp,"rda",module,temp1);
		if(temp1!=NULL) Rfree(temp1);
		/* Pack the maintain master screens if the report is process exceptions */
		if(r->process_exceptions)
		{
			makeAllMaintainMastercode(r->MTNMSTR,fp,"rda",module,r->MTNMSTR->name,0);
		}

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
static void quit_printRDAreportcode(RDArsrc *prsrc)
{
        if(diagrptgen)
        {
                prterr("DIAG quit_printRDAreportcode Quitting Print RDA Report Code");
        }
        free_rsrc(prsrc);
}
static void printRDAreportcode(RDArsrc *prsrc,RDAreport *r)
{
	int selectedm=0;
	char *outdevice=NULL;
	char *module=NULL;
	FILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
#ifndef WIN32
		fp=popen(outdevice,"w");
#endif
#ifdef WIN32
		fp=fopen(outdevice,"a+");
#endif
                if(fp!=NULL)
                {
			if(outdevice!=NULL) Rfree(outdevice);
			if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
			module=stralloc(modlstx->libs[selectedm]);
			makeAllReportcode(r,fp,module);
#ifndef WIN32
			pclose(fp);
#endif
#ifdef WIN32
			fclose(fp);
#endif
		} else {
			ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
			prterr("Error PrintRDAreport popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
			if(outdevice!=NULL) Rfree(outdevice);
		}
	} else {
		ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
	}
}
static void GetDevicePrintRDAreportcode(RDAreport *r)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAreportcode,r);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAreportcode,NULL);
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
void okreport(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	RDAreport *report=NULL;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(RDAstrcmp(rptavl->libs[selected],"No Definitions Available"))
	{
		report=GETRDAreport(modlstx->libs[selectedm],rptavl->libs[selected]);
		if(report==NULL)
		{
			ERRORDIALOG("Cannot Read Report","Error in permissions, users doesn't have access to read the report definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			return;
		} else {
			GetDevicePrintRDAreportcode(report);
		}
	} else { 
		return;
	}
}
void setvalues(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected))
	{
		return;
	}
	if(!RDAstrcmp(rptavl->libs[selected],"No Report Definitions Available"))
	{
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm))
	{
		return;
	}
	temp=getreporttitle(modlstx->libs[selectedm],rptavl->libs[selected]);
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE",temp))
	{
		updatersrc(mainrsrc,"TITLE");
	}
	if(temp!=NULL) Rfree(temp);
	temp=getreportdesc(modlstx->libs[selectedm],rptavl->libs[selected]);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
	{
		updatersrc(mainrsrc,"DESCRIPTION");
	}
	if(temp!=NULL) Rfree(temp);
}
void getrptentries(RDArsrc *mainrsrc)
{
	int x,selected=0;
	char *tmp=NULL,*dirx=NULL,*temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
	for(x=0;findfile(dirx,"*.RPT",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		temp=stripdashes(tmp);
		addAPPlib(rptavl,temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	}
	if(rptavl->numlibs<1)
	{
		addAPPlib(rptavl,"No Report Definitions Available");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(rptavl);
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"REPORTS AVAILABLE",0,rptavl))
		updatersrc(mainrsrc,"REPORTS AVAILABLE");
	}
	setvalues(mainrsrc);
}
void getmodulelist(RDArsrc *mainrsrc)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlstx,tmp);
	}
	if(modlstx->numlibs<1)
	{
		addAPPlib(modlstx,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlstx);
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",0,modlstx))
		{
			updatersrc(mainrsrc,"MODULE LIST");
		}
	}
}
void quitrptcode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(modlstx!=NULL) freeapplib(modlstx);
	if(rptavl!=NULL) freeapplib(rptavl);
	ShutdownSubsystems();
	exit(0);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	char *module=NULL;
	RDAreport *report=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE REPORT CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		module=stralloc(argv[1]);
		report=GETRDAreport(module,argv[2]);
		if(report==NULL)
		{
			prterr("Error attempting to read the report definition [%s][%s].  Check the permissions and retry.",argv[1],argv[2]);
		} else {
			makeAllReportcode(report,stdout,module);
		}
		if(module!=NULL) Rfree(module);
		ShutdownSubsystems();
		exit(0);
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE REPORT CODE"))
		{
			ShutdownSubsystems();
			exit(1);
/*
			RDAAPPMAINLOOP();
			return;
*/
		}
		getmodulelist(mainrsrc);
		mainrsrc=RDArsrcNEW("TOOLS","MAKE REPORT CODE");
		x=0;
		if(argc>1)
		{
			for(x=0;x<modlstx->numlibs;++x)
			{
				if(!RDAstrcmp(argv[1],modlstx->libs[x])) break;
			}
			if(x>=modlstx->numlibs) x=0;
		}
		addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,getrptentries,
				modlstx->numlibs,&modlstx->libs,NULL);
		x=0;
		if(rptavl!=NULL) freeapplib(rptavl);
		rptavl=APPlibNEW();
		addlstrsrc(mainrsrc,"REPORTS AVAILABLE",&x,TRUE,setvalues,
				rptavl->numlibs,&rptavl->libs,NULL);
		addstdrsrc(mainrsrc,"TITLE",VARIABLETEXT,0,NULL,FALSE);
		addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
		getrptentries(mainrsrc);
		setvalues(mainrsrc);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,okreport,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitrptcode,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mainrsrc,"LIBRARY LIST");
		APPmakescrn(mainrsrc,TRUE,quitrptcode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
