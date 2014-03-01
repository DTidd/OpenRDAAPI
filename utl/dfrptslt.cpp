/* dfrptslt.c - Select Records Section of Define Reports */
#include <cstdio>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

extern RDAreport *report;
extern char *module;
extern void loadvalue(RDArsrc *,char *);
extern char changedreport;
static RDAdefault *sltdefaults;
static char changedselects=FALSE;

static void quitslt(RDArsrc *sltrsrc)
{
	if(sltrsrc!=NULL)
	{
		killwindow(sltrsrc);
		free_rsrc(sltrsrc);
	}
	if(changedselects) changedreport=TRUE;
	if(sltdefaults!=NULL) FreeRDAdefault(sltdefaults);
}
static void quitsltclr(RDArsrc *sltrsrc)
{
	changedselects=FALSE;
	quitslt(sltrsrc);
}
static void saveslt(RDArsrc *sltrsrc)
{
	readallwidgets(sltrsrc);
	FINDRSCGETSTRING(sltrsrc,"EXPRESSION",&report->select);
	if(COMPARE_RSRCS(sltrsrc,sltdefaults,2))
	{
		changedselects=TRUE;
	}
	quitslt(sltrsrc);
}
static void quitslttest(RDArsrc *sltrsrc)
{
	readallwidgets(sltrsrc);
	if(COMPARE_RSRCS(sltrsrc,sltdefaults,2))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the SELECT RECORDS DEFINITION!\nDo you want to Save these changes?",saveslt,quitsltclr,FALSE,sltrsrc);
	} else {
		quitslt(sltrsrc);
	}
}
void RPTselectrecords(RDArsrc *rptrsrc)
{
	RDArsrc *sltrsrc=NULL;

	sltrsrc=RDArsrcNEW(module,"DEFINE SELECT RECORDS");
	addsctrsrc(sltrsrc,"EXPRESSION",0,(report->select!=NULL ?
		report->select:""),TRUE);
	addbtnrsrc(sltrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(sltrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(sltrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(sltrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addrfcbrsrc(sltrsrc,"SAVE",TRUE,saveslt,NULL);
	addrfcbrsrc(sltrsrc,"QUIT",TRUE,quitslttest,NULL);
	addbtnrsrc(sltrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(sltrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	sltdefaults=GetDefaults(sltrsrc);
	APPmakescrn(sltrsrc,TRUE,quitslt,NULL,FALSE);
}
