/* qn2it */
/*lint -library */
#ifndef WIN32
#define __NAM__ "openrda.lnx"
#endif
#ifdef WIN32
#define __NAM__ "openrda.exe"
#endif
#include <app.hpp>


#include <powadd.hpp>
#include <rptgen.hpp>
#include <mix.hpp>
#include <olh.hpp>
#include <sec.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <misc.hpp>
#ifndef WIN32
#include <signal.h>
#endif 
/*CPP_OPART qn2it1 */
/*CPP_OPART cyclemenu */
/*CPP_OPART finmgtdash */
/*CPP_OPART finscatter */
/*CPP_OPART venpmtdash */
/*CPP_OPART prsnnldash */
/*CPP_OPART bnkrecdash */
/*CPP_OPART fixassdash */
/*CPP_OPART purorddash */
/*CPP_OPART embedfeed */
/*ADDLIB curl */
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


short DOMENU_STYLE=0,OPENRDA_STYLE=0;

APPlib *ENV_Start=NULL;
Wt::WContainerWidget *MainWindow=NULL,*MainWindowCenter=NULL;
Wt::WContainerWidget *MainWindowRight=NULL;
Wt::WContainerWidget *MainWindowStatus=NULL,*MainWindowDock=NULL;
Wt::WBorderLayout *MainBorderLayout=NULL;
Wt::WPushButton *Reports_P=NULL,*Query_P=NULL;
Wt::WPushButton *Maintenance_P=NULL,*Setup_P=NULL,*Other_P=NULL;
Wt::WPopupMenu *FPOP=NULL,*Edit=NULL,*QueryPop=NULL,*Reports=NULL;
Wt::WPopupMenu *Maintenance=NULL,*Setup=NULL,*Other=NULL,*Modules=NULL;
Wt::WPopupMenu *Window=NULL,*Finance_Pop=NULL;
Wt::WStackedWidget *StatusWindowStack=NULL;
Wt::WPushButton *Finance_P=NULL;
extern Wt::WPopupMenu *ModuleDDL(short);
short MODULE_FILENO=(-1),MENUITEM_FILENO=(-1),OPENRDA_DATLOG=(-1);
char *CURRENT_MODULE=NULL,*defmodule=NULL,*LAST_MODULE=NULL;
char DISPLAY_FAST=FALSE,NO_NEWS=FALSE;
int LAST_EXE=(-1),ARCHIVE_STYLE=0;
RDArsrc *DockRSRC=NULL;
static void cybersupport(Wt::WWidget *),rda_website(Wt::WWidget *),rda_blogs(Wt::WWidget *);
static void rda_feedback(Wt::WWidget *);
static void rda_wikis(Wt::WWidget *);
static void rda_newsletters(Wt::WWidget *w);
static void uimetric_all(Wt::WWidget *);
static void uimetric_sumall(Wt::WWidget *);
static void uimetric_me(Wt::WWidget *);
static void MyUserProfile();
#ifndef WIN32
static void upload_a_file(Wt::WWidget *);
#endif /* WIN32 */
static void systemprintmgt(Wt::WWidget *);
extern void CreateDockedWindow(void);
extern void ChooseModule(char *),VIEWTRAN(int,char **),ExecuteOption(int);
extern void CreateWorkFlow(char *);
char **ModuleGroupList=NULL,IGNORE_RESTRICTED=FALSE;
short NumberModuleGroups=6;
FILE *myMENUITEMS=NULL;
APPlib *ProcessList=NULL;
APPlib *SysAdminModules=NULL,*SysAdminNames=NULL;
APPlib *FinanceModules=NULL,*FNames=NULL,*PNames=NULL,*HNames=NULL,*RNames=NULL;
APPlib *ProcurementModules=NULL,*HRModules=NULL,*RevenueModules=NULL;
static void sign_out(void),go_home(void);

static char OpenRDA_POSTRKSWB()
{
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	char SWB_PT02_POSITION_MASTERS=FALSE;
	char SWB_PT03_POSITION_ACCOUNTS=FALSE;
	char SWB_PT04_GROSS_AND_RATES=FALSE;
	char SWB_PT05_VACANCY_MANAGEMENT=FALSE;
	char SWB_PT07_RATE_ACCOUNT_IMAGES=FALSE;
	char SWB_PT08_SUPPLEMENTAL_CODES=FALSE;
	char SWB_PT12_EEO_REPORTING=FALSE;
	char SWB_PT15_SET_CHANGE_GROSS_ID=FALSE;
	char SWB_PT16_SET_CHANGE_FLSA_FLAGS=FALSE;
	char SWB_PT18_CHANGE_POSITION_IDS=FALSE;
	char SWB_PT19_CHANGE_LOCATION_IDS=FALSE;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("POSTRK")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"POSTRK");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"POSTRK");
#endif
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT02_POSITION_MASTERS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT02_POSITION_MASTERS=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT03_POSITION_ACCOUNTS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT03_POSITION_ACCOUNTS=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT04_GROSS_AND_RATES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT04_GROSS_AND_RATES=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT05_VACANCY_MANAGEMENT");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT05_VACANCY_MANAGEMENT=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT07_RATE_ACCOUNT_IMAGES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT07_RATE_ACCOUNT_IMAGES=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT08_SUPPLEMENTAL_CODES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT08_SUPPLEMENTAL_CODES=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT12_EEO_REPORTING");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT12_EEO_REPORTING=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT15_SET_CHANGE_GROSS_ID");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT15_SET_CHANGE_GROSS_ID=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT16_SET_CHANGE_FLSA_FLAGS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT16_SET_CHANGE_FLSA_FLAGS=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT18_CHANGE_POSITION_IDS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT18_CHANGE_POSITION_IDS=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("POSTRK","SWB_PT19_CHANGE_LOCATION_IDS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_PT19_CHANGE_LOCATION_IDS=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(SWB_PT02_POSITION_MASTERS || SWB_PT03_POSITION_ACCOUNTS || SWB_PT04_GROSS_AND_RATES || SWB_PT05_VACANCY_MANAGEMENT ||
		SWB_PT07_RATE_ACCOUNT_IMAGES || SWB_PT08_SUPPLEMENTAL_CODES || 
		SWB_PT12_EEO_REPORTING || SWB_PT15_SET_CHANGE_GROSS_ID || SWB_PT16_SET_CHANGE_FLSA_FLAGS ||
		SWB_PT19_CHANGE_LOCATION_IDS) return(TRUE);
	return(FALSE);
}

APPlib *GetModuleGroups()
{
	short ef=0,g=0,domenu_style=0;
	char delflag=FALSE,execfunc=FALSE,*modname=NULL;
	APPlib *grpavl=NULL;

	grpavl=APPlibNEW();
	ZERNRD(MODULE_FILENO);
	ef=FRSNRD(MODULE_FILENO,1);
	while(!ef)
	{
		FINDFLDGETCHAR(MODULE_FILENO,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			ZERNRD(MODSEC_FILENO);
			FINDFLDSETSTRING(MODSEC_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN);
			FINDFLDGETSTRING(MODULE_FILENO,"MODULE NAME",&modname);
			if(!RDAstrcmp(modname,"VW"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","PAYROLL");
			} else if(!RDAstrcmp(modname,"RLSTCHG") || !RDAstrcmp(modname,"RLSTREC") || !RDAstrcmp(modname,"RLSTSTM"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","RLSTMGT");
			} else if(!RDAstrcmp(modname,"PROPCHG") || !RDAstrcmp(modname,"PROPREC") || !RDAstrcmp(modname,"PROPSTM"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","PROPERTY");
			} else if(!RDAstrcmp(modname,"MISCBCHG") || !RDAstrcmp(modname,"MISCBREC"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","MISCBILL");
			} else if(!RDAstrcmp(modname,"UTLBLLCUS") || !RDAstrcmp(modname,"UTLBLLMTR") || !RDAstrcmp(modname,"UTLBLLCHG") || !RDAstrcmp(modname,"UTLBLLREC") || !RDAstrcmp(modname,"UTLBLLWKO"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","UTLBLL");
			} else if(!RDAstrcmp(modname,"1099"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","VENPMT");
			} else if(!RDAstrcmp(modname,"W2"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","PAYROLL");
			} else if(!RDAstrcmp(modname,"BUDPAY"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","BUDPREP");
			} else if((!RDAstrcmp(modname,"TOOLS") || !RDAstrcmp(modname,"VERSION")))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","UTILITIES");
			} else if(RDAstrstr(modname,"DMV"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","DMVREG");
			} else if(!RDAstrcmp(modname,"VAIRMSGET"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","VAIRMS");
			} else COPYFIELD(MODULE_FILENO,MODSEC_FILENO,"MODULE NAME");
			if(!RDAstrcmp(modname,"PAYROLL") || !RDAstrcmp(modname,"VW") || !RDAstrcmp(modname,"W2")) delflag=XPERT_SETUP->PAYROLL;
			else if(!RDAstrcmp(modname,"VENPMT") || !RDAstrcmp(modname,"1099")) delflag=XPERT_SETUP->VENPMT;
			else if(!RDAstrcmp(modname,"FINMGT")) delflag=XPERT_SETUP->FINMGT;
			else if(!RDAstrcmp(modname,"BNKREC")) delflag=XPERT_SETUP->BNKREC;
			else if(!RDAstrcmp(modname,"INVCTL")) delflag=XPERT_SETUP->INVCTL;
			else if(!RDAstrcmp(modname,"FIXASS")) delflag=XPERT_SETUP->FIXASS;
			else if(!RDAstrcmp(modname,"PURORD")) delflag=XPERT_SETUP->PURORD;
			else if(!RDAstrcmp(modname,"CATALOGUE")) delflag=XPERT_SETUP->CATALOGUE;
			else if(RDAstrstr(modname,"BUD")) delflag=XPERT_SETUP->BUDPREP;
			else if(!RDAstrcmp(modname,"PRSNNL")) delflag=XPERT_SETUP->PRSNNL;
			else if(!RDAstrcmp(modname,"POSTRK")) 
			{
				if(XPERT_SETUP->software_type==0) 
				{
					delflag=XPERT_SETUP->POSTRK;
				} else {
					if(XPERT_SETUP->POSTRK==FALSE) delflag=FALSE;
					else delflag=OpenRDA_POSTRKSWB();
				}
			} else if(!RDAstrcmp(modname,"LVEMGT")) delflag=XPERT_SETUP->LVEMGT;
			else if(!RDAstrcmp(modname,"EMPABS")) delflag=XPERT_SETUP->EMPABS;
			else if(!RDAstrcmp(modname,"SUBMGT")) delflag=XPERT_SETUP->SUBMGT;
			else if(!RDAstrcmp(modname,"APPMGT")) delflag=XPERT_SETUP->APPMGT;
			else if(RDAstrstr(modname,"RLST")) delflag=XPERT_SETUP->RLSTMGT;
			else if(RDAstrstr(modname,"PROP")) delflag=XPERT_SETUP->PROPERTY;
			else if(RDAstrstr(modname,"IRMS")) delflag=XPERT_SETUP->PROPERTY;
			else if(RDAstrstr(modname,"UTLBLL")) delflag=XPERT_SETUP->UTLBLL;
			else if(!RDAstrcmp(modname,"OCCTAX")) delflag=XPERT_SETUP->OCCTAX;
			else if(RDAstrstr(modname,"MISCB")) delflag=XPERT_SETUP->MISCBILL;
			else if(!RDAstrcmp(modname,"BLDPRMT")) delflag=XPERT_SETUP->BLDPRMT;
			else if(!RDAstrcmp(modname,"CSHDWR")) delflag=XPERT_SETUP->CSHDWR;
			else if(RDAstrstr(modname,"DMV")) delflag=XPERT_SETUP->DMVREG;
			if(modname!=NULL) Rfree(modname);
			if(!EQLNRD(MODSEC_FILENO,1))
			{
				FINDFLDGETCHAR(MODULE_FILENO,"DELETEFLAG",&delflag);
				FINDFLDGETCHAR(MODSEC_FILENO,"EXECUTE FUNCTIONS",&execfunc);
				if(execfunc==TRUE && !delflag)
				{
					FINDFLDGETSHORT(MODULE_FILENO,"GROUP",&g);
					domenu_style=Users_DomenuStyle();
					if(delflag==TRUE || (g==5 && domenu_style==0) || g!=5)
					{
					addAPPlibNoDuplicates(grpavl,ModuleGroupList[g]);
					}
				}
			}
		}
		ef=NXTNRD(MODULE_FILENO,1);
	}
	return(grpavl);
}
APPlib *GetGroupModules(char *gname,APPlib *Names)
{
	short ef=0,g=0,group=0;
	char delflag=FALSE,*temp=NULL,execfunc=FALSE,*temp1=NULL;
	APPlib *modavl=NULL;

	for(ef=0;ef<NumberModuleGroups;++ef)
	{
		if(!RDAstrcmp(gname,ModuleGroupList[ef])) break;
	}
	if(group<NumberModuleGroups) group=ef;
	modavl=APPlibNEW();
	ZERNRD(MODULE_FILENO);
	FINDFLDSETSHORT(MODULE_FILENO,"GROUP",group);
	ef=GTENRD(MODULE_FILENO,1);
	while(!ef)
	{
		FINDFLDGETSHORT(MODULE_FILENO,"GROUP",&g);
		if(g!=group) break;
		FINDFLDGETCHAR(MODULE_FILENO,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			ZERNRD(MODSEC_FILENO);
			FINDFLDSETSTRING(MODSEC_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN);
			FINDFLDGETSTRING(MODULE_FILENO,"MODULE NAME",&temp);
			if(!RDAstrcmp(temp,"VW"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","PAYROLL");
			} else if(!RDAstrcmp(temp,"1099"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","VENPMT");
			} else if(!RDAstrcmp(temp,"W2"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","PAYROLL");
			} else if(!RDAstrcmp(temp,"BUDPAY"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","BUDPREP");
			} else if(!RDAstrcmp(temp,"RLSTCHG") || !RDAstrcmp(temp,"RLSTREC") || !RDAstrcmp(temp,"RLSTSTM"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","RLSTMGT");
			} else if(!RDAstrcmp(temp,"PROPCHG") || !RDAstrcmp(temp,"PROPREC") || !RDAstrcmp(temp,"PROPSTM"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","PROPERTY");
			} else if(!RDAstrcmp(temp,"MISCBCHG") || !RDAstrcmp(temp,"MISCBREC"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","MISCBILL");
			} else if(!RDAstrcmp(temp,"VAIRMSGET"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","VAIRMS");
			} else if(RDAstrstr(temp,"DMV"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","DMVREG");
			} else if(!RDAstrcmp(temp,"UTLBLLCUS") || !RDAstrcmp(temp,"UTLBLLMTR") || !RDAstrcmp(temp,"UTLBLLCHG") || !RDAstrcmp(temp,"UTLBLLREC") || !RDAstrcmp(temp,"UTLBLLWKO"))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","UTLBLL");
			} else if((!RDAstrcmp(temp,"TOOLS")))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","UTILITIES");
			} else if((!RDAstrcmp(temp,"VERSION")))
			{
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","UTILITIES");
			} else {
				FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME",temp);
			}
			delflag=TRUE;
			if(!RDAstrcmp(temp,"PAYROLL") || !RDAstrcmp(temp,"VW") || !RDAstrcmp(temp,"W2")) delflag=XPERT_SETUP->PAYROLL;
			else if(!RDAstrcmp(temp,"VENPMT") || !RDAstrcmp(temp,"1099")) delflag=XPERT_SETUP->VENPMT;
			else if(!RDAstrcmp(temp,"BNKREC")) delflag=XPERT_SETUP->BNKREC;
			else if(!RDAstrcmp(temp,"INVCTL")) delflag=XPERT_SETUP->INVCTL;
			else if(!RDAstrcmp(temp,"FINMGT")) delflag=XPERT_SETUP->FINMGT;
			else if(!RDAstrcmp(temp,"FIXASS")) delflag=XPERT_SETUP->FIXASS;
			else if(!RDAstrcmp(temp,"PURORD")) delflag=XPERT_SETUP->PURORD;
			else if(!RDAstrcmp(temp,"CATALOGUE")) delflag=XPERT_SETUP->CATALOGUE;
			else if(!RDAstrcmp(temp,"BUDPREP")) delflag=XPERT_SETUP->BUDPREP;
			else if(!RDAstrcmp(temp,"BUDPAY")) delflag=XPERT_SETUP->BUDPREP; 
			else if(!RDAstrcmp(temp,"PRSNNL")) delflag=XPERT_SETUP->PRSNNL;
			else if(!RDAstrcmp(temp,"POSTRK")) 
			{
				if(XPERT_SETUP->software_type==0) 
				{
					delflag=XPERT_SETUP->POSTRK;
				} else {
					if(XPERT_SETUP->POSTRK==FALSE) delflag=FALSE;
					else delflag=OpenRDA_POSTRKSWB();
				}
			} else if(!RDAstrcmp(temp,"LVEMGT")) delflag=XPERT_SETUP->LVEMGT;
			else if(!RDAstrcmp(temp,"EMPABS")) delflag=XPERT_SETUP->EMPABS;
			else if(!RDAstrcmp(temp,"SUBMGT")) delflag=XPERT_SETUP->SUBMGT;
			else if(!RDAstrcmp(temp,"APPMGT")) delflag=XPERT_SETUP->APPMGT;
			else if(RDAstrstr(temp,"RLST")) delflag=XPERT_SETUP->RLSTMGT;
			else if(RDAstrstr(temp,"PROP")) delflag=XPERT_SETUP->PROPERTY;
			else if(RDAstrstr(temp,"IRMS")) delflag=XPERT_SETUP->PROPERTY;
			else if(RDAstrstr(temp,"UTLBLL")) delflag=XPERT_SETUP->UTLBLL;
			else if(!RDAstrcmp(temp,"OCCTAX")) delflag=XPERT_SETUP->OCCTAX;
			else if(RDAstrstr(temp,"MISCB")) delflag=XPERT_SETUP->MISCBILL;
			else if(!RDAstrcmp(temp,"BLDPRMT")) delflag=XPERT_SETUP->BLDPRMT;
			else if(!RDAstrcmp(temp,"CSHDWR")) delflag=XPERT_SETUP->CSHDWR;
			else if(RDAstrstr(temp,"DMV")) delflag=XPERT_SETUP->DMVREG;
			if(!EQLNRD(MODSEC_FILENO,1))
			{
				FINDFLDGETCHAR(MODSEC_FILENO,"EXECUTE FUNCTIONS",&execfunc);
				if(execfunc==TRUE && (delflag==TRUE || group==5))
				{
					if(isEMPTY(defmodule)) 
					{
						if(defmodule!=NULL) Rfree(defmodule);
						defmodule=stralloc(temp);
					}
					addAPPlibNoDuplicates(modavl,temp);
					if(Names!=NULL)
					{
						FINDFLDGETSTRING(MODULE_FILENO,"DESCRIPTION",&temp1);
						addAPPlibNoDuplicates(Names,(temp1!=NULL ? temp1:temp));
					}
				}
			}
		}
		ef=NXTNRD(MODULE_FILENO,1);
	}
	if(temp!=NULL) Rfree(temp);
	return(modavl);
}
#ifndef WIN32
void quitrunning(RDArsrc *r)
{
	int x=0;
	char makeexit=FALSE;
	if(!RDAstrcmp(ProcessList->libs[x],"No Running Processes")) 
	{
		makeexit=TRUE;
	}
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(ProcessList!=NULL) freeapplib(ProcessList);
	ProcessList=NULL;
	if(makeexit) exit(0);
}
static void setdevlicense(RDArsrc *r)
{
	short allow_add=FALSE;
	char *devlicense=NULL;

	readwidget(r,"DEV_LICENSE");
	FINDRSCGETSTRING(r,"DEV_LICENSE",&devlicense);
	if(CheckDevLicense(devlicense))
	{
		FINDRSCSETSENSITIVE(r,"KILL PROCESS",TRUE);
		FINDRSCSETEDITABLE(r,"KILL PROCESS",TRUE);
	} else {
		FINDRSCSETSENSITIVE(r,"KILL PROCESS",FALSE);
		FINDRSCSETEDITABLE(r,"KILL PROCESS",FALSE);
	}
	if(devlicense!=NULL) Rfree(devlicense);
}
static void killrunning(RDArsrc *r)
{
	int x=0,y=0,start=0,end=0,p=0;
	char *temp=NULL;
	APPlib *list=NULL;

	FINDRSCGETINT(r,"RUNNING PROCESSES",&x);
	if(!RDAstrcmp(ProcessList->libs[x],"No Running Processes")) 
	{
		return;
	}
	for(y=0;y<RDAstrlen(ProcessList->libs[x]);++y)
	{
		temp=ProcessList->libs[x]+y;
		if(isdigit(*temp) && start==0)
		{
			start=y;
		} else if(start!=0 && !isdigit(*temp))
		{
			end=y;
		}
		if(end!=0) break;
	}
	temp=stralloc(&ProcessList->libs[x][start]);
	temp[end-start]=0;
	p=atoi(temp);
	if(temp!=NULL) Rfree(temp);
	kill(p,SIGTERM);
	list=APPlibNEW();
	for(y=0;y<ProcessList->numlibs;++y)
	{
		if(y!=x) addAPPlib(list,ProcessList->libs[y]);
	}
	if(list->numlibs==0)
	{
		addAPPlib(list,"No Running Processes");
	}
	freeapplib(ProcessList);
	ProcessList=APPlibNEW();
	for(y=0;y<list->numlibs;++y)
	{
		addAPPlib(ProcessList,list->libs[y]);
	}
	if(list!=NULL) freeapplib(list);
	x=0;
	FINDRSCSETLIST(r,"RUNNING PROCESSES",x,ProcessList->numlibs,&ProcessList->libs);
	updatersrc(r,"RUNNING PROCESSES");
	if(!RDAstrcmp(ProcessList->libs[x],"No Running Processes")) 
	{
		FINDRSCSETEDITABLE(r,"KILL PROCESS",FALSE);
		FINDRSCSETSENSITIVE(r,"KILL PROCESS",FALSE);
		updatersrc(r,"KILL PROCESS");
	}
}
#endif 
static void setcookie(Wt::WWidget *P)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"GUI");
	addAPPlib(args,"SET COOKIE");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
static void go_home()
{
	if(defmodule!=NULL) Rfree(defmodule);
	defmodule=stralloc("HOME");
	ChooseModule(defmodule);
}
static void sign_out()
{
	short ef=0;
	char delflag=FALSE,*userid=NULL;
	RDATData *prev=NULL;
	RDA_UnSetEnv("rdalogin");
	CLSNRD(OPENRDA_DATLOG);
	if(SEC_TOKEN_FILENO!=(-1))
	{
		ZERNRD(SEC_TOKEN_FILENO);
		ef=FRSNRD(SEC_TOKEN_FILENO,1);
		while(!ef)
		{
			FINDFLDGETCHAR(SEC_TOKEN_FILENO,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(SEC_TOKEN_FILENO,"USER IDENTIFICATION",&userid);
				if(!RDAstrcmp(userid,USERLOGIN))
				{
					prev=RDATDataNEW(SEC_TOKEN_FILENO);
					FINDFLDSETCHAR(SEC_TOKEN_FILENO,"DELETEFLAG",TRUE);
					WRTTRANS(SEC_TOKEN_FILENO,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
					DELNRD(SEC_TOKEN_FILENO);
				}
			}
			ef=NXTNRD(SEC_TOKEN_FILENO,1);
		}
	}
	ShutdownSubsystems();
}
static char *getopenselfserve()
{
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	char *r=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("OPENSS")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"OPENSS");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"OPENSS");
#endif
	gsv=RDAGenericSetupNew("PRSNNL","OPENSS SITE URL");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			r=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	return(r);
}
static void FAST_Cabinet(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"MTN ARCHIVES.BROWSE");
	WT_RDA_SetEnv("WORK_GROUP",CURRENT_MODULE,__LINE__,__FILE__);
	ExecuteProgram("mtnmst",args);
	if(args!=NULL) freeapplib(args);
}
static void FAST_Folder(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"MTN ARCHWORK");
	ExecuteProgram("mtnmst",args);
	if(args!=NULL) freeapplib(args);
}
static void FAST_QueryCabinet(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"MTN ARCHIVES.SEARCH");
	WT_RDA_SetEnv("WORK_GROUP",CURRENT_MODULE,__LINE__,__FILE__);
	ExecuteProgram("mtnmst",args);
	if(args!=NULL) freeapplib(args);
}
static void FAST_AddWorkGroup(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"ADD WORK GROUP");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
static void FAST_Build(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"ADD ARCHIVE");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
static void FAST_Scans(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"MANAGE SCANS");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
static void FAST_Email(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"EMAILARCHIVE");
	ExecuteProgram("doreport",args);
	if(args!=NULL) freeapplib(args);
}
static void FAST_CopyArchive(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	addAPPlib(args,"COPYARCHIVE");
	ExecuteProgram("doreport",args);
	if(args!=NULL) freeapplib(args);
}
#ifndef WIN32
static void systemprintmgt(Wt::WWidget *t)
{
	GUI_OpenURL("http://127.0.0.1:631/printers");
}
#endif
static void MyUserProfile()
{
	APPlib *args=NULL,*envpx=NULL;

	args=APPlibNEW();
	envpx=APPlibNEW();
	memset(stemp,0,101);
	sprintf(stemp,"ADD_USER_IDENTIFICATION=%s",USERLOGIN);
	addAPPlib(envpx,stemp);
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"USER PROFILE");
	ADVExecuteProgram("doadd",args,envpx);
	if(args!=NULL) freeapplib(args);
	if(envpx!=NULL) freeapplib(envpx);
}
#ifndef WIN32
static void upload_a_file(Wt::WWidget *P)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"UTILITIES");
	addAPPlib(args,"UPLOAD FILE");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
#endif
static void cybersupport(Wt::WWidget *P)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"CYBERSUPPORT");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
static void rda_website(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"http://www.openrda.net");
	DOHTTP(args->numlibs,args->libs);
	if(args!=NULL) freeapplib(args);
}
static void rda_blogs(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"http:///openrda.com/blog");
	DOHTTP(args->numlibs,args->libs);
	if(args!=NULL) freeapplib(args);
}
static void rda_feedback(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"ADD FEEDBACK");
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
static void msmtp_logdelete(Wt::WWidget *w)
{
	FILE *fp=NULL;
	char *temp1=NULL,*temp2=NULL;
	char stempx[512];

	memset(stempx,0,512);
	sprintf(stempx,"%s/%s/msmtp.log",CURRENTDIRECTORY,USERLOGIN);
	unlink(stempx);
#ifndef WIN32
	fp=fopen(stempx,"a+b");
#else
	fp=fopen(stempx,"abc");
#endif
	if(fp!=NULL)
	{
		temp1=GETCURRENTDATE10();
		temp2=GETCURRENTTIME();	
		fprintf(fp,"Log Truncated on [%s] at [%s].\r\n\r\n",(temp1!=NULL ? temp1:""),(temp2!=NULL ? temp2:""));
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		fflush(fp);
		close(fp);
	}
}
static void msmtp_log(Wt::WWidget *w)
{
	APPlib *args=NULL;
	char stempx[512];

	args=APPlibNEW();
	addAPPlib(args,"-pr4");
	addAPPlib(args,"-ps2");
	addAPPlib(args,"-i");
	memset(stempx,0,512);
	sprintf(stemp,"%s/%s/msmtp.log",CURRENTDIRECTORY,USERLOGIN);
	addAPPlib(args,stemp);
	ADVExecuteProgram("viewpdf",args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void rda_newsletters(Wt::WWidget *w)
{
	ExecuteProgram("news",NULL);
}
static void rda_wikis(Wt::WWidget *w)
{
	char *page=NULL;

	OpenRDAWiki(page);
	if(page!=NULL) Rfree(page);
}
static void uimetric_me(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"openrda3");
	addAPPlib(args,USERLOGIN);
	ExecuteProgram("uimetric",args);
	if(args!=NULL) freeapplib(args);
}
static void uimetric_sumall(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"openrda3");
	addAPPlib(args,"SUM_ALL_USERS");
	ExecuteProgram("uimetric",args);
	if(args!=NULL) freeapplib(args);
}
static void uimetric_all(Wt::WWidget *w)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"openrda3");
	ExecuteProgram("uimetric",args);
	if(args!=NULL) freeapplib(args);
}
static void eDocumentStack()
{
	StatusWindowStack->setCurrentIndex(1);
}
static void MaintenanceStack()
{
}
static void SetupStack()
{
}
static void OtherStack()
{
}
void CreateAccordionBar()
{
#ifdef XXXXXX
	Wt::WStackedWidget *stack=NULL;
	Wt::WContainerWidget *C=NULL;
	Wt::WMenu *menu=NULL;

	StatusWindowStack = new Wt::WStackedWidget(MainWindowStatus);
	C = new Wt::WContainerWidget();
	stack = new Wt::WStackedWidget(C);
	menu=new Wt::WMenu(stack,Wt::Vertical,C);
	menu->addItem("E-Documents",eDocumentStack);
	menu->addItem("Maintenance",MaintenanceStack);
#endif /* XXXXXX */
}
void CreateStatusBar()
{
	Wt::WVBoxLayout *MWSL=NULL;
	Wt::WGridLayout *StatusBarL=NULL;
	Wt::WContainerWidget *StatusBar=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WLength spc;
	Wt::WLink *WK=NULL;
	Wt::WMenuItem *MItem=NULL;
	Wt::WPushButton *P=NULL;
	Wt::WPopupMenu *pop=NULL,*pop1=NULL;
	Wt::WString *c=NULL;
	char *user="openrda",*passw="cW9jd3ExRXhWS1U5";
	char *redirect=NULL,*temp=NULL,*temp1=NULL;
	char *url=NULL;
	int len=0;
	base64_encodestate enstate;
	int count=0,y=0;

	if(!RDAstrcmp(CURRENT_MODULE,"HOME"))
	{
		if(MainWindowStatus!=NULL) MainWindowStatus->clear();
	} else if(MainWindowStatus->count()>0) 
	{
		if(Maintenance!=NULL) Maintenance->~WMenu();
		Maintenance=ModuleDDL(2);
		if(Maintenance==NULL)
		{
			Maintenance_P->setEnabled(FALSE);
		} else {
			Maintenance_P->setMenu(Maintenance);
			Maintenance_P->setEnabled(TRUE);
		}
		if(Setup!=NULL) Setup->~WMenu();
		Setup=ModuleDDL(3);
		if(Setup==NULL)
		{
			Setup_P->setEnabled(FALSE);
		} else {
			Setup_P->setMenu(Setup);
			Setup_P->setEnabled(TRUE);
		}
		if(Other!=NULL) Other->~WMenu();
		Other=ModuleDDL(4);
		if(Other==NULL)
		{
			Other_P->setEnabled(FALSE);
		} else {
			Other_P->setMenu(Other);
			Other_P->setEnabled(TRUE);
		}
	} else {
		MWSL=new Wt::WVBoxLayout();
		MainWindowStatus->setLayout(MWSL);
		MainWindowStatus->addStyleClass("OpenRDA ResourceBar");
		MWSL->setSpacing(0);
		daL=(Wt::WLayout *)MWSL;
		daL->setContentsMargins(0,0,0,0);
		spc=Wt::WLength(0,Wt::WLength::Pixel);
		MainWindowStatus->setPadding(spc,All);
		StatusBar=new Wt::WContainerWidget();

		MWSL->addWidget((Wt::WWidget *)StatusBar);
		StatusBarL=new Wt::WGridLayout();
		StatusBar->setLayout(StatusBarL);	
		daL=(Wt::WLayout *)StatusBarL;
		daL->setContentsMargins(0,0,0,0);
		spc=Wt::WLength(0,Wt::WLength::Pixel);
		StatusBar->setPadding(spc,All);
		StatusBarL->setHorizontalSpacing(0);
		StatusBarL->setVerticalSpacing(0);
		StatusBarL->setRowStretch(0,0);

/* E-Documents */
		P=new Wt::WPushButton();
		P->addStyleClass("OpenRDA ResourceBar E-Documents");
		StatusBarL->addWidget(P,0,count,0);
		++count;
		P->setDefault(FALSE);
		c=new Wt::WString("E-Documents");
		P->setText(*c);
		c->~WString();
		pop=new Wt::WPopupMenu();
		pop->addStyleClass("OpenRDA ResourceBar E-Documents WPopupMenu");
		P->setMenu(pop);
		P->setEnabled(TRUE);
		pop->addItem("View E-Documents(s)")->triggered().connect(boost::bind(&FAST_Cabinet,P));
		pop->addItem("Query E-Documents(s)")->triggered().connect(boost::bind(&FAST_QueryCabinet,P));
		if(ARCHIVE_STYLE>1)
		{
			pop->addItem("Capture Scan(s)")->triggered().connect(boost::bind(&FAST_Scans,P));
			pop->addItem("Store E-Document(s)")->triggered().connect(boost::bind(&FAST_Build,P));
		}
		if(DOMENU_STYLE==0)
		{
			pop->addItem("Copy E-Document(s)")->triggered().connect(boost::bind(&FAST_CopyArchive,P));
		}
		pop->addItem("Email E-Document(s)")->triggered().connect(boost::bind(&FAST_Email,P));
		if(DOMENU_STYLE==0)
		{
			pop->addItem("Available Folders")->triggered().connect(boost::bind(&FAST_Folder,P));
			pop->addItem("Add a New Folder")->triggered().connect(boost::bind(&FAST_AddWorkGroup,P));
		}

/* Maintenance */
		Maintenance_P=new Wt::WPushButton();
		Maintenance_P->addStyleClass("OpenRDA ResourceBar Maintenance");
		StatusBarL->addWidget(Maintenance_P,0,count,0);
		++count;
		Maintenance_P->setDefault(FALSE);
		c=new Wt::WString("Maintenance");
		Maintenance_P->setText(*c);
		c->~WString();
		Maintenance=ModuleDDL(2);
		if(Maintenance==NULL)
		{
			Maintenance_P->setEnabled(FALSE);
		} else {
			Maintenance->addStyleClass("OpenRDA ResourceBar Maintenance WPopupMenu");
			Maintenance_P->setMenu(Maintenance);
			Maintenance_P->setEnabled(TRUE);
		}

/* Setup */
		Setup_P=new Wt::WPushButton();
		Setup_P->addStyleClass("OpenRDA ResourceBar Setup");
		StatusBarL->addWidget(Setup_P,0,count,0);
		++count;
		Setup_P->setDefault(FALSE);
		c=new Wt::WString("Setup");
		Setup_P->setText(*c);
		c->~WString();
		Setup=ModuleDDL(3);
		if(Setup==NULL)
		{
			Setup_P->setEnabled(FALSE);
		} else {
			Setup->addStyleClass("OpenRDA ResourceBar Setup WPopupMenu");
			Setup_P->setMenu(Setup);
			Setup_P->setEnabled(TRUE);
		}

/* Other */
		Other_P=new Wt::WPushButton();
		Other_P->addStyleClass("OpenRDA ResourceBar Other");
		StatusBarL->addWidget(Other_P,0,count,0);
		++count;
		Other_P->setDefault(FALSE);
		c=new Wt::WString("Other");
		Other_P->setText(*c);
		c->~WString();
		Other=ModuleDDL(4);
		if(Other==NULL)
		{
			Other_P->setEnabled(FALSE);
		} else {
			Other->addStyleClass("OpenRDA ResourceBar Other WPopupMenu");
			Other_P->setMenu(Other);
			Other_P->setEnabled(TRUE);
		}


/* HELP */
		P=new Wt::WPushButton();
		P->addStyleClass("OpenRDA ResourceBar RDAHelp");
		StatusBarL->addWidget(P,0,count,0);
		++count;
		P->setDefault(FALSE);
		c=new Wt::WString("Help");
		P->setText(*c);
		c->~WString();
		pop=new Wt::WPopupMenu();
		P->addStyleClass("OpenRDA ResourceBar RDAHelp WPopupMenu");
		P->setMenu(pop);
		P->setEnabled(TRUE);

#ifdef XXXX
/* Diagrams */
		Diagrams=qt_new_QPopupMenu((qt_QWidget *)w,"Diagrams");
		SetupWindowColorPalette((qt_QWidget *)Diagrams,"MAIN MENU");
		c=qt_new_QString5("Diagrams");
		qt_QMenuData_insertItem6((qt_QMenuData *)MenuData,c,(qt_QPopupMenu *)Diagrams,-1,-1);
		DiagramData=qt_QPopupMenu_QMenuData((qt_QPopupMenu *)Diagrams);
		qt_del_QString(c);
		SubMenu=qt_new_QPopupMenu((qt_QWidget *)Diagrams,"OpenRDA");
		SubData=qt_QPopupMenu_QMenuData((qt_QPopupMenu *)SubMenu);
		grpavl=GetModuleGroups();
		if(grpavl!=NULL)
		{
			for(y=0;y<grpavl->numlibs;++y)
			{
				SubMenu1=qt_new_QPopupMenu((qt_QWidget *)Diagrams,grpavl->libs[y]);
				SubData1=qt_QPopupMenu_QMenuData((qt_QPopupMenu *)SubMenu1);
				namavl=APPlibNEW();
				modavl=GetGroupModules(grpavl->libs[y],namavl);
				for(z=0;z<modavl->numlibs;++z)
				{
					c1=qt_new_QString5(namavl->libs[z]);
					mySlot=(qt_Slot *)qt_new_UserDataSlot(CreateWorkFlow,modavl->libs[z]);
					a=qt_QMenuData_insertItem4((qt_QMenuData *)SubData1,c1,-1,-1);
					qt_QMenuData_connectItem((qt_QMenuData *)SubData1,a,(qt_QObject *)mySlot,"1UserDataSlot()");
					qt_del_QString(c1);
					if(!RDAstrcmp(modavl->libs[z],"PURORD"))
					{
						c1=qt_new_QString5("Purchase Order Approval");
						mySlot=(qt_Slot *)qt_new_UserDataSlot(CreateWorkFlow,"PO APPROVAL");
						a=qt_QMenuData_insertItem4((qt_QMenuData *)SubData1,c1,-1,-1);
						qt_QMenuData_connectItem((qt_QMenuData *)SubData1,a,(qt_QObject *)mySlot,"1UserDataSlot()");
						qt_del_QString(c1);
					}
				}
				c=qt_new_QString5(grpavl->libs[y]);
				qt_QMenuData_insertItem6((qt_QMenuData *)DiagramData,c,(qt_QPopupMenu *)SubMenu1,-1,-1);
				qt_del_QString(c);
				if(namavl!=NULL) freeapplib(namavl);
			}
			freeapplib(grpavl);
		}
#endif /* XXXX */
		WK=new Wt::WLink("resources/OpenRDA/NavigationalTerminology.pdf");
		MItem=pop->addItem("Navigation Terminology");
		MItem->setLink(*WK);
		MItem->setLinkTarget(TargetNewWindow);
		WK=new Wt::WLink("http://www.openrda.net/OpenRDALandingPage.php");
		MItem=pop->addItem("OpenRDA News");
		MItem->setLink(*WK);
		MItem->setLinkTarget(TargetNewWindow);

		temp=stralloc("Category:Modules");
		base64_init_encodestate(&enstate);
		len=RDAstrlen(temp);
		redirect=Rmalloc((len+1)*2);
		temp1=Rmalloc((len+1)*2);
		len=base64_encode_block(temp,len,temp1,&enstate);
		sprintf(redirect,"%s%s",redirect,temp1);
		memset(temp1,0,RDAstrlen(temp)+1);
		len=base64_encode_blockend(temp1,&enstate);
		sprintf(redirect,"%s%s",redirect,temp1);
		url=Rmalloc(256);
		sprintf(url,"http://wiki.openrda.com/index.php?user=%s&passw=%s&redirect=%s",user,passw,(redirect!=NULL ? redirect:""));
		WK=new Wt::WLink(url);
		if(temp!=NULL) Rfree(temp);
		if(temp1!=NULL) Rfree(temp1);
		if(redirect!=NULL) Rfree(redirect);
		MItem=pop->addItem("OpenRDA Answers / Documentation");
		MItem->setLink(*WK);
		MItem->setLinkTarget(TargetNewWindow);
		WK=new Wt::WLink("http://openrda.net/rda-announcements");
		MItem=pop->addItem("Product Announcements");
		MItem->setLink(*WK);
		MItem->setLinkTarget(TargetNewWindow);
		WK=new Wt::WLink("http://openrda.net/videos");
		MItem=pop->addItem("Videos");
		MItem->setLink(*WK);
		MItem->setLinkTarget(TargetNewWindow);

		temp=Users_Login();
		if(PROCsecurity("OLHELP","CYBERSUPPORT") && !isEMPTY(temp))
		{
			pop->addItem("CyberSupport")->triggered().connect(boost::bind(&cybersupport,P));
		}
		if(temp!=NULL) Rfree(temp);

#ifndef WIN32
		if(UsersWorkstationValidated())
		{
			pop->addItem("Upload a File")->triggered().connect(boost::bind(&upload_a_file,P));
		}
#endif
		WK=new Wt::WLink("http://openrda.com");
		MItem=pop->addItem("Visit Our Web Site");
		MItem->setLink(*WK);
		MItem->setLinkTarget(TargetNewWindow);
		WK=new Wt::WLink("http://openrda.com/blog");
		MItem=pop->addItem("Visit Our Blogs");
		MItem->setLink(*WK);
		MItem->setLinkTarget(TargetNewWindow);
		pop->addItem("Submit Suggestion")->triggered().connect(boost::bind(&rda_feedback,P));

		pop1=new Wt::WPopupMenu();
		pop1->addStyleClass("OpenRDA ResourceBar RDAHelp WPopupMenu EmailLog");
		pop1->addItem("View")->triggered().connect(boost::bind(&msmtp_log,P));
		pop1->addItem("Truncate")->triggered().connect(boost::bind(&msmtp_logdelete,P));
		pop->addMenu("Email Log (msmtp.log)",pop1);

		pop->addItem("My User Interface Metrics")->triggered().connect(boost::bind(&uimetric_me,P));
		pop->addItem("Site Interface Metrics")->triggered().connect(boost::bind(&uimetric_all,P));
		pop->addItem("Summary of Interface Metrics")->triggered().connect(boost::bind(&uimetric_sumall,P));
		pop->addItem("Set a Cookie")->triggered().connect(boost::bind(&setcookie,P));


/* System Administration */
		if(SysAdminModules!=NULL)
		{
			if(SysAdminModules->numlibs>0)
			{
				P=new Wt::WPushButton();
				P->addStyleClass("OpenRDA ResourceBar SystemAdministration");
				StatusBarL->addWidget(P,0,count,0);
				++count;
				P->setDefault(FALSE);
				c=new Wt::WString("System Administration");
				P->setText(*c);
				c->~WString();
				pop=new Wt::WPopupMenu();
				pop->addStyleClass("OpenRDA ResourceBar SystemAdministration WPopupMenu");
				P->setMenu(pop);
				P->setEnabled(TRUE);
				for(y=0;y<SysAdminModules->numlibs;++y)
				{
					pop->addItem(SysAdminNames->libs[y])->triggered().connect(boost::bind(&ChooseModule,SysAdminModules->libs[y]));
				}
			}
		}


		if(XPERT_SETUP->HRM)
		{
/* Open Self Service */
			P=new Wt::WPushButton();
			P->addStyleClass("OpenRDA ResourceBar OpenSelfServe");
			StatusBarL->addWidget(P,0,count,0);
			++count;
			P->setDefault(FALSE);
			c=new Wt::WString("Open Self Serve");
			P->setText(*c);
			c->~WString();
			temp=getopenselfserve();
			WK=new Wt::WLink(temp);
			if(temp!=NULL) Rfree(temp);
			P->setLink(*WK);
			P->setLinkTarget(TargetNewWindow);
		}
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	Wt::WContainerWidget *rC=NULL,*West=NULL;
	Wt::WContainerWidget *ModuleNavigation=NULL;
	Wt::WPushButton *P=NULL;
	Wt::WContainerWidget *v1=NULL,*v2=NULL,*h=NULL;
	Wt::WVBoxLayout *vb1=NULL,*vb2=NULL;
	Wt::WHBoxLayout *hb=NULL,*mwh=NULL,*hb2=NULL;
	Wt::WGridLayout *g=NULL;
	Wt::WString *c=NULL;
	Wt::WText *text=NULL;
	Wt::WAnchor *myA=NULL;
	Wt::WImage *OpenRDAImage=NULL;
	Wt::WPopupMenu *pop=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WMenuItem *MenuI=NULL;
	std::string *s1=NULL;
	char *temp=NULL;
	short x=0,height=0,y=0,z=0;
	char *libx=NULL,*opt1=NULL,*opt2=NULL,*opt3=NULL,*opt4=NULL;
	RDAGenericSetup *gsv=NULL;
	APPlib *atmp1=NULL,*atmp2=NULL;
	Wt:WLength spc;
	int count=0;

	if(InitializeSubsystems(argc,argv,"UTILITIES","MAIN MENU")) 
	{
        	return(1);
	}
	temp=RDA_GetEnv("NO_NEWS");
	if(!isEMPTY(temp)) NO_NEWS=TRUE;
		else NO_NEWS=FALSE;
	memset(stemp,0,101);
	sprintf(stemp,"TRUE");
	IGNORE_RESTRICTED=UsersIgnoreRestrictedProcessConstraints();
	memset(stemp,0,101);
	sprintf(stemp,"%s/%s/openrda3.csv",CURRENTDIRECTORY,USERLOGIN);
#ifndef WIN32
	myMENUITEMS=fopen(stemp,"a+b");
#else
	myMENUITEMS=fopen(stemp,"abc");
#endif
	dotrans=TRUE;
#ifdef __NOT_USING_HOME__
	CURRENT_MODULE=stralloc("FINMGT");
#else
	CURRENT_MODULE=stralloc("HOME");
#endif /* __NOT_USING_HOME__ */
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("ARCHIVE")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
	gsv=RDAGenericSetupNew("ARCHIVE","ARCHIVE STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			ARCHIVE_STYLE=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
	gsv=RDAGenericSetupNew("UTILITIES","SWB_S1_FAST");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			DISPLAY_FAST=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","OPENRDA STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			OPENRDA_STYLE=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 1");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			opt1=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 2");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			opt2=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 3");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			opt3=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","OPTIONAL MODULE GROUP 4");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			opt4=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	DOMENU_STYLE=Users_DomenuStyle();
	NumberModuleGroups=6;
	if(!isEMPTY(opt1)) ++NumberModuleGroups;
	if(!isEMPTY(opt2)) ++NumberModuleGroups;
	if(!isEMPTY(opt3)) ++NumberModuleGroups;
	if(!isEMPTY(opt4)) ++NumberModuleGroups;
	ModuleGroupList=Rmalloc(NumberModuleGroups*sizeof(char *));
	for(x=0;x<NumberModuleGroups;++x)
	{
		if(x==0) ModuleGroupList[0]=stralloc("Finance");
		if(x==1) ModuleGroupList[1]=stralloc("Budget");
		if(x==2) ModuleGroupList[2]=stralloc("Procurement");
		if(x==3) ModuleGroupList[3]=stralloc("HR");
		if(x==4) ModuleGroupList[4]=stralloc("Revenue");
		if(x==5) ModuleGroupList[5]=stralloc("System Administration");
		if(x==6) ModuleGroupList[6]=stralloc(opt1);
		if(x==7) ModuleGroupList[7]=stralloc(opt2);
		if(x==8) ModuleGroupList[8]=stralloc(opt3);
		if(x==9) ModuleGroupList[9]=stralloc(opt4);
	}
	if(opt1!=NULL) Rfree(opt1);
	if(opt2!=NULL) Rfree(opt2);
	if(opt3!=NULL) Rfree(opt3);
	if(opt4!=NULL) Rfree(opt4);
	MODULE_FILENO=OPNNRD("UTILITIES","MODULE");
	MENUITEM_FILENO=OPNNRD("UTILITIES","MENUITEM");
	OPENRDA_DATLOG=OPNNRD("DATABASE","DATLOG");
	RDAMAINWIDGET->setTitle("OpenRDA 4.0");
	rC=RDAMAINWIDGET->root();
	rC->addStyleClass("OpenRDA MainWindow");
	MainBorderLayout=new Wt::WBorderLayout();
	rC->setLayout(MainBorderLayout);
	MainBorderLayout->setSpacing(0);
	daL=(Wt::WLayout *)MainBorderLayout;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	rC->setPadding(spc,All);
	MainWindow=new Wt::WContainerWidget();
	MainWindow->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
	MainWindow->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
	MainWindow->addStyleClass("OpenRDA MenuBar");
	MainBorderLayout->addWidget((Wt::WWidget *)MainWindow,Wt::WBorderLayout::Position::North);
	if(OPENRDA_STYLE==1)
	{
		West=new Wt::WContainerWidget();
		West->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
		West->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
		MainBorderLayout->addWidget((Wt::WWidget *)West,Wt::WBorderLayout::Position::West);
		MainWindowStatus=new Wt::WContainerWidget(West);
		MainWindowStatus->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
		MainWindowStatus->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
		MainWindowStatus->addStyleClass("OpenRDA ResourceBar");
		MainWindowDock=new Wt::WContainerWidget(West);
		MainWindowDock->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
		MainWindowDock->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
		MainWindowDock->addStyleClass("OpenRDA TaskBar");
	} else {
		MainWindowDock=new Wt::WContainerWidget();
		MainWindowDock->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
		MainWindowDock->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
		MainWindowDock->addStyleClass("OpenRDA TaskBar");
		MainBorderLayout->addWidget((Wt::WWidget *)MainWindowDock,Wt::WBorderLayout::Position::West);
		MainWindowStatus=new Wt::WContainerWidget();
		MainWindowStatus->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
		MainWindowStatus->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
		MainWindowStatus->addStyleClass("OpenRDA ResourceBar");
		MainBorderLayout->addWidget((Wt::WWidget *)MainWindowStatus,Wt::WBorderLayout::Position::South);
	}
	MainWindowCenter=new Wt::WContainerWidget();
	MainWindowCenter->addStyleClass("OpenRDA Workspace");
	MainWindowCenter->setOverflow(Wt::WContainerWidget::OverflowAuto,Vertical);
	MainWindowCenter->setOverflow(Wt::WContainerWidget::OverflowAuto,Horizontal);
	MainBorderLayout->addWidget((Wt::WWidget *)MainWindowCenter,Wt::WBorderLayout::Position::Center);
	MainWindowRight=new Wt::WContainerWidget();
	MainBorderLayout->addWidget((Wt::WWidget *)MainWindowRight,Wt::WBorderLayout::Position::East);
	MainWindowRight->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
	MainWindowRight->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
	MainWindowRight->addStyleClass("OpenRDA RightMenu");


/* Create Main Navigational Window */
	mwh=new Wt::WHBoxLayout();
	MainWindow->setLayout(mwh);
	mwh->setSpacing(0);
	daL=(Wt::WLayout *)mwh;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	MainWindow->setPadding(spc,All);


	v1=new Wt::WContainerWidget();
	v1->setContentAlignment(Wt::AlignLeft);
	mwh->addWidget(v1,500);
	vb1=new Wt::WVBoxLayout();
	v1->setLayout(vb1);
	vb1->setSpacing(0);
	daL=(Wt::WLayout *)vb1;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	v1->setPadding(spc,All);

	if(OPENRDA_STYLE==0)
	{
		OpenRDAImage=new Wt::WImage();
		OpenRDAImage->setImageLink("resources/OpenRDA/rdalogo.png");
		myA=new Wt::WAnchor();
		myA->setLink("http://www.openrda.com");
		myA->addWidget(OpenRDAImage);
		mwh->addWidget(myA);
	}

	h=new Wt::WContainerWidget();
	h->setContentAlignment(Wt::AlignLeft);
	vb1->addWidget(h);
	hb=new Wt::WHBoxLayout();
	h->setLayout(hb);
	hb->setSpacing(0);
	daL=(Wt::WLayout *)hb;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	h->setPadding(spc,All);

	temp=Rmalloc(RDAstrlen(XPERT_SETUP->company_name)+10);
	sprintf(temp,"%s",(XPERT_SETUP->company_name!=NULL ? XPERT_SETUP->company_name:""));
	text=new Wt::WText(temp);
	text->addStyleClass("OpenRDA h1");
	if(temp!=NULL) Rfree(temp);
	hb->addWidget(text,500);

	P=new Wt::WPushButton((Wt::WContainerWidget *)v2);
	P->setToolTip("Home",PlainText);
	P->addStyleClass("OpenRDA Home");
	hb->addWidget(P);
	P->setDefault(FALSE);
	P->clicked().connect(boost::bind(&go_home));

	P=new Wt::WPushButton((Wt::WContainerWidget *)v2);
	P->setToolTip("My Settings",PlainText);
	P->addStyleClass("OpenRDA MySettings");
	hb->addWidget(P);
	P->setDefault(FALSE);
	P->clicked().connect(boost::bind(&MyUserProfile));

	P=new Wt::WPushButton((Wt::WContainerWidget *)v2);
	P->setToolTip("Sign Out",PlainText);
	P->addStyleClass("OpenRDA SignOut");
	hb->addWidget(P);
	P->setDefault(FALSE);
	P->clicked().connect(boost::bind(&sign_out));
	

/* Module Navigation Bar */
	ModuleNavigation=new Wt::WContainerWidget();
	ModuleNavigation->addStyleClass("OpenRDA ModuleNavigation");
	vb1->addWidget(ModuleNavigation);
	g=new Wt::WGridLayout();
	ModuleNavigation->setLayout(g);
	g->setRowStretch(0,0);
	g->setHorizontalSpacing(0);
	g->setVerticalSpacing(0);
	count=0;

	SysAdminNames=APPlibNEW();
	SysAdminModules=GetGroupModules("System Administration",SysAdminNames);
/* Finance */
	FNames=APPlibNEW();
	FinanceModules=GetGroupModules("Finance",FNames);
	atmp1=APPlibNEW();
	atmp2=GetGroupModules("Budget",atmp1);
	if(atmp2!=NULL)
	{
		if(atmp2->numlibs>0)
		{
			for(y=0;y<atmp2->numlibs;++y)
			{
				addAPPlib(FNames,atmp1->libs[y]);
				addAPPlib(FinanceModules,atmp2->libs[y]);
			}
		}
	}
	if(atmp1!=NULL) freeapplib(atmp1);
	if(atmp2!=NULL) freeapplib(atmp2);
	if(FinanceModules!=NULL)
	{
		if(FinanceModules->numlibs>0)
		{
			c=new Wt::WString("Finance");
			Finance_P=new Wt::WPushButton(*c,(Wt::WContainerWidget *)ModuleNavigation);
			c->~WString();
			Finance_P->addStyleClass("OpenRDA ModuleNavigation Finance");
			g->addWidget(Finance_P,0,count,0);
			g->setRowStretch(count,100);
			++count;
			Finance_P->setDefault(FALSE);
			Finance_Pop=new Wt::WPopupMenu();
			Finance_Pop->addStyleClass("OpenRDA ModuleNavigation Finance WPopupMenu");
			Finance_P->setMenu(Finance_Pop);

			for(y=0;y<FinanceModules->numlibs;++y)
			{
				MenuI=Finance_Pop->addItem(FNames->libs[y]);
				MenuI->triggered().connect(boost::bind(&ChooseModule,FinanceModules->libs[y]));
				MenuI->addStyleClass("OpenRDA ModuleNavigation Finance WPopupMenu");
			}
		}
	}
/* Procurement */
	PNames=APPlibNEW();
	ProcurementModules=GetGroupModules("Procurement",PNames);
	if(ProcurementModules!=NULL)
	{
		if(ProcurementModules->numlibs>0)
		{
			P=new Wt::WPushButton();
			P->addStyleClass("OpenRDA ModuleNavigation Procurement");
			g->addWidget(P,0,count,0);
			++count;
			P->setDefault(FALSE);
			c=new Wt::WString("Procurement");
			P->setText(*c);
			c->~WString();
			pop=new Wt::WPopupMenu();
			pop->addStyleClass("OpenRDA ModuleNavigation Procurement WPopupMenu");
			P->setMenu(pop);
			for(y=0;y<ProcurementModules->numlibs;++y)
			{
				MenuI=pop->addItem(PNames->libs[y]);
				MenuI->triggered().connect(boost::bind(&ChooseModule,ProcurementModules->libs[y]));
				MenuI->addStyleClass("OpenRDA ModuleNavigation Procurement WPopupMenu");
			}
		}
	}
/* HR */
	HNames=APPlibNEW();
	HRModules=GetGroupModules("HR",HNames);
	if(HRModules!=NULL)
	{
		if(HRModules->numlibs>0)
		{
			P=new Wt::WPushButton();
			P->addStyleClass("OpenRDA ModuleNavigation HR");
			g->addWidget(P,0,count,0);
			++count;
			P->setDefault(FALSE);
			c=new Wt::WString("HR");
			P->setText(*c);
			c->~WString();
			pop=new Wt::WPopupMenu();
			pop->addStyleClass("OpenRDA ModuleNavigation HR WPopupMenu");
			P->setMenu(pop);
			for(y=0;y<HRModules->numlibs;++y)
			{
				MenuI=pop->addItem(HNames->libs[y]);
				MenuI->triggered().connect(boost::bind(&ChooseModule,HRModules->libs[y]));
				MenuI->addStyleClass("OpenRDA ModuleNavigation HR WPopupMenu");
			}
		}
	}
/* Revenue */
	RNames=APPlibNEW();
	RevenueModules=GetGroupModules("Revenue",RNames);
	if(RevenueModules!=NULL)
	{
		if(RevenueModules->numlibs>0)
		{
			x=FINDAPPLIBELEMENT(RevenueModules,"RLSTCHG");
			if(x==(-1)) x=FINDAPPLIBELEMENT(RevenueModules,"PROPCHG");
			if(x==(-1)) x=FINDAPPLIBELEMENT(RevenueModules,"DMVWORK");
			if(x==(-1)) x=FINDAPPLIBELEMENT(RevenueModules,"DMVLIVE");
			if(x==(-1)) x=FINDAPPLIBELEMENT(RevenueModules,"VAIRMSGET");
			if(x>(-1))
			{
				P=new Wt::WPushButton();
				P->addStyleClass("OpenRDA ModuleNavigation Property");
				g->addWidget(P,0,count,0);
				++count;
				P->setDefault(FALSE);
				c=new Wt::WString("Property");
				P->setText(*c);
				c->~WString();
				pop=new Wt::WPopupMenu();
				pop->addStyleClass("OpenRDA ModuleNavigation Property WPopupMenu");
				P->setMenu(pop);

				for(y=0;y<RevenueModules->numlibs;++y)
				{
					if(RDAstrstr(RevenueModules->libs[y],"PROP") || RDAstrstr(RevenueModules->libs[y],"RLST") || RDAstrstr(RevenueModules->libs[y],"DMV") || RDAstrstr(RevenueModules->libs[y],"IRMS"))
					{
						MenuI=pop->addItem(RNames->libs[y]);
						MenuI->triggered().connect(boost::bind(&ChooseModule,RevenueModules->libs[y]));
						MenuI->addStyleClass("OpenRDA ModuleNavigation Property WPopupMenu");
					}
				}
			}
			x=FINDAPPLIBELEMENT(RevenueModules,"BLDPRMT");
			if(x==(-1)) x=FINDAPPLIBELEMENT(RevenueModules,"MISCBCHG");
			if(x==(-1)) x=FINDAPPLIBELEMENT(RevenueModules,"MISCBREC");
			if(x==(-1)) x=FINDAPPLIBELEMENT(RevenueModules,"OCCTAX");
			if(x>(-1))
			{
				P=new Wt::WPushButton();
				P->addStyleClass("OpenRDA ModuleNavigation PermitsLicensesMisc");
				g->addWidget(P,0,count,0);
				++count;
				P->setDefault(FALSE);
				c=new Wt::WString("Permits/Licenses/Misc");
				P->setText(*c);
				c->~WString();
				pop=new Wt::WPopupMenu();
				pop->addStyleClass("OpenRDA ModuleNavigation PermitsLicensesMisc WPopupMenu");
				P->setMenu(pop);

				for(y=0;y<RevenueModules->numlibs;++y)
				{
					if(RDAstrstr(RevenueModules->libs[y],"BLDPRMT") || RDAstrstr(RevenueModules->libs[y],"MISC") || RDAstrstr(RevenueModules->libs[y],"OCCTAX"))
					{
						MenuI=pop->addItem(RNames->libs[y]);
						MenuI->triggered().connect(boost::bind(&ChooseModule,RevenueModules->libs[y]));
						MenuI->addStyleClass("OpenRDA ModuleNavigation PermitsLicensesMisc WPopupMenu");
					}
				}
			}
			x=FINDAPPLIBELEMENT(RevenueModules,"UTLBLLCHG");
			if(x>(-1))
			{
				P=new Wt::WPushButton();
				P->addStyleClass("OpenRDA ModuleNavigation Enterprise");
				g->addWidget(P,0,count,0);
				++count;
				P->setDefault(FALSE);
				c=new Wt::WString("Enterprise");
				P->setText(*c);
				c->~WString();
				pop=new Wt::WPopupMenu();
				pop->addStyleClass("OpenRDA ModuleNavigation Enterprise WPopupMenu");
				P->setMenu(pop);

				for(y=0;y<RevenueModules->numlibs;++y)
				{
					if(RDAstrstr(RevenueModules->libs[y],"UTLBLL"))
					{
						MenuI=pop->addItem(RNames->libs[y]);
						MenuI->triggered().connect(boost::bind(&ChooseModule,RevenueModules->libs[y]));
						MenuI->addStyleClass("OpenRDA ModuleNavigation Enterprise WPopupMenu");
					}
				}
			}
			x=FINDAPPLIBELEMENT(RevenueModules,"CSHDWR");
			if(x>(-1))
			{
				P=new Wt::WPushButton();
				P->addStyleClass("OpenRDA ModuleNavigation Collections");
				g->addWidget(P,0,count,0);
				++count;
				P->setDefault(FALSE);
				c=new Wt::WString("Collections");
				P->setText(*c);
				c->~WString();
				P->clicked().connect(boost::bind(&ChooseModule,RevenueModules->libs[x]));
			}
		}
	}

/* End of Main Navigational Window */
#ifdef __NOT_USING_HOME__

	FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
	if(EQLNRD(SEC_USERS_FILENO,1)) KEYNRD(SEC_USERS_FILENO,1);
	FINDFLDGETSTRING(SEC_USERS_FILENO,"MODULE NAME",&defmodule);
	if(isEMPTY(defmodule))
	{
		if(defmodule!=NULL) Rfree(defmodule);
		defmodule=stralloc(CURRENT_MODULE);
	}
	ChooseModule(defmodule);
#else 
	defmodule=stralloc("HOME");
	ChooseModule(defmodule);
#endif /* __NOT_USING_HOME__ */
	rC->show();
}
