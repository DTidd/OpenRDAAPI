/* UTILITIES's - Make (XPERT SETUP) Screen */
#include <cstdio>
#include <mix.hpp>
#include <rptgen.hpp>
#include <mkmsc.hpp>
/*COMPMEM 40960 */

extern char *module;
extern APPlib *errorlist;
static void XPERT_SETUP_MENU(void);
static void SCN_XPERT_SETUP(void);
void DIAGNOSTIC_SCREEN(void);
void MAKE_XPERT_SETUP(void);
void RUN_XPERT_SETUP(void);

void MAKE_XPERT_SETUP()
{
	XPERT_SETUP_MENU();
	SCN_XPERT_SETUP();
	RUN_XPERT_SETUP();
}
void RUN_XPERT_SETUP()
{
	ADVExecute2Program("xpertstp",NULL,NULL);
}
void DIAGNOSTIC_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("UTILITIES","DIAGNOSTIC SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"PROCESS NAME","Process Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"STATUS","Status","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,22,"PROGRESS BAR","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Selected","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Total Read","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Total All","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED","Selected","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"TOTAL READ","Total Read","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"TOTAL ALL","Total All","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Start Time","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Current","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Elapsed","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"START TIME","Start Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"CURRENT TIME","Current Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"ELAPSED TIME","Elapsed Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Increment in:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"INCREMENT","Increment","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","seconds minimums","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SUSPEND","Suspend","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RESUME","Resume","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->module)+RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [%s] Screen [%s], Can Not Save Screen!",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void XPERT_SETUP_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	menu=RDAmenuNEW("XPERT SETUP","xpertstp.EXT");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
/*
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(module)+RDAstrlen(menu->name)+80+1);
			sprintf(temp1,"MENU WRITE ERROR: Module [%s] Screen [%s], Can Not Save Menu!",(module!=NULL?module:""),(menu->name!=NULL?menu->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
static void SCN_XPERT_SETUP()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","XPERT SETUP");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Organization:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Development License:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEV_LICENSE","Development License","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Organization Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ORGANIZATION NAME","Organization Name","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          Address 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADDRESS #1","Address #1","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          Address 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ADDRESS #2","Address #2","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","               City:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CITY","City","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," State:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"STATE","State","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Zip:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ZIP","Zip","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","       Phone Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PHONE","Phone","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Fax Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FAX","Fax","","",0,12,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","      Software Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"SOFTWARE TYPES","Software Types","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Organization Type:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ORGANIZATION TYPES","Organization Types","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Accounting Method:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ACCOUNTING METHODS","Accounting Methods","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Program Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PROGRAM DIRECTORY","Program Directory","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     Data Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DATA DIRECTORY","Data Directory","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","    Number of Users:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NO USERS","Number of Users","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"USE END DATE","Expires","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,""," Expiration Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"END DATE","End Date","","",0,10,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","       Software Key:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SOFTWARE KEY","Software Key","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Module Setup","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ARCHIVE","F.A.S.T","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"APPMGT","AppMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ASP","Asp Model","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"BARCODING","Barcoding","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"BFTMGT","BftMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"BLDPRMT","BldPrmt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"BNKREC","BnkRec","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"BUDPREP","BudPrep","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CATALOGUE","Catalogue","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CIS","CIS","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CITWEB","CitWeb","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CREDITCARDS","Credit Cards","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"CSHDWR","CshDwr","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DMVREG","DMV","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"EMPABS","EmpAbs","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"EXTENSIONS","Extensions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"FINMGT","FinMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"FIXASS","FixAss","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"FOODSVC","FoodSvc","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"IIDMGT","IIDMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"INVCTL","InvCtl","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"LVEMGT","LveMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"LVSIMP","Leave Simple","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"MISCBILL","MiscBill","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"OCCTAX","OccTax","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"OPEN SOURCE","Open Source","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"PAYROLL","Payroll","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"POSTRK","PosTrk","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"PROPERTY","Property","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"PRSNNL","Personnel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"PURORD","PurOrd","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"REQMGT","ReqMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ROLLACCT","Rollup Accounting","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RLSTMGT","RlstMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RLVEMGT","Remote LveMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RREQMGT","Remote REQMGT","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SECURITYEXTENSIONS","Security Ext","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SHOW NEW IN DEFINE LIST","Show New Defines","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SIMPLE MENU","Simple Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"STUDFIN","StudFin","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SUBMGT","SubMgt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"TAXCLT","TaxClt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"TOOLS","Tools","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"TRANSACTIONS","Transactions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"UTILITIES","Utilities","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"UTLBLL","Utility Billing","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"VEHMTN","VehMtn","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"VENPMT","VenPmt","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"WRKORD","WorkOrd","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"VIOLATION","Violations","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"HRM","ESS","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);



		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(11+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [XPERT SETUP], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void addMenuItem(short fileno,char *md,char *itemnumber1,char *arg1,
char *arg2,char *arg3,char *arg4,char *arg5,char *dflag,char *dd_list1,
char *en1,char *en2,char *en3,char *en4,char *en5,char *name,char * parent1,
char *program,char *rank1,char *style1,char *ev1,char *ev2,char *ev3,
char *ev4,char *ev5,char *vis)
{
	char delflag=FALSE;
	int itemnumber=atoi(itemnumber1);
	short dd_list=atoi(dd_list1);
	int parent=atoi(parent1);
	int rank=atoi(rank1);
	short style=atoi(style1);

	if(!RDAstrcmp(dflag,"Y")) delflag=TRUE;
	ZERNRD(fileno);
	FINDFLDSETCHAR(fileno,"DELETEFLAG",delflag);
	FINDFLDSETSTRING(fileno,"MODULE NAME",md);
	FINDFLDSETINT(fileno,"ITEM NUMBER",itemnumber);
	FINDFLDSETSTRING(fileno,"ARGUMENT 1",arg1);
	FINDFLDSETSTRING(fileno,"ARGUMENT 2",arg2);
	FINDFLDSETSTRING(fileno,"ARGUMENT 3",arg3);
	FINDFLDSETSTRING(fileno,"ARGUMENT 4",arg4);
	FINDFLDSETSTRING(fileno,"ARGUMENT 5",arg5);
	FINDFLDSETSHORT(fileno,"DROP DOWN LIST",dd_list);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 1",en1);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 2",en2);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 3",en3);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 4",en4);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 5",en5);
	FINDFLDSETSTRING(fileno,"NAME",name);
	FINDFLDSETINT(fileno,"PARENT",parent);
	FINDFLDSETSTRING(fileno,"PROGRAM",program);
	FINDFLDSETSHORT(fileno,"RANK",rank);
	FINDFLDSETSHORT(fileno,"STYLE",style);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 1",ev1);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 2",ev2);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 3",ev3);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 4",ev4);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 5",ev5);
	FINDFLDSETSTRING(fileno,"VISIBLE EXPRESSION",vis);
	WRTNRD(fileno,0,NULL);
}
char *swb_bp07_purge="((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) OR ([SOFTWARE TYPE]=0)";
char *swb_lv07_purge="((GENERIC_SETUP_VARIABLE(\"[LVEMGT][SWB_LV07_PURGE]\"))) OR ([SOFTWARE TYPE]=0)";

void CREATE_MENUITEM()
{
	short fileno=(-1),ef=FALSE;
	char custom=FALSE,*mod=NULL,*mod1=NULL;

	fileno=OPNNRD("UTILITIES","MENUITEM");
	if(fileno!=(-1))
	{
		mod=stralloc("UTILITIES");
		FINDFLDSETSTRING(fileno,"MODULE NAME",mod);
		ef=GTENRDsec(fileno,1);
		while(!ef)
		{
			FINDFLDGETSTRING(fileno,"MODULE NAME",&mod1);
			if(RDAstrcmp(mod,mod1)) break;
			FINDFLDGETCHAR(fileno,"CUSTOM",&custom);
			if(!custom) DELNRD(fileno);
			ef=NXTNRDsec(fileno,1);
		}
		if(mod!=NULL) Rfree(mod);
		if(mod1!=NULL) Rfree(mod1);
		mod=stralloc("DATABASE");
		FINDFLDSETSTRING(fileno,"MODULE NAME",mod);
		ef=GTENRDsec(fileno,1);
		while(!ef)
		{
			FINDFLDGETSTRING(fileno,"MODULE NAME",&mod1);
			if(RDAstrcmp(mod,mod1)) break;
			FINDFLDGETCHAR(fileno,"CUSTOM",&custom);
			if(!custom) DELNRD(fileno);
			ef=NXTNRDsec(fileno,1);
		}
		if(mod!=NULL) Rfree(mod);
		if(mod1!=NULL) Rfree(mod1);
		mod=stralloc("OLHELP");
		FINDFLDSETSTRING(fileno,"MODULE NAME",mod);
		ef=GTENRDsec(fileno,1);
		while(!ef)
		{
			FINDFLDGETSTRING(fileno,"MODULE NAME",&mod1);
			if(RDAstrcmp(mod,mod1)) break;
			FINDFLDGETCHAR(fileno,"CUSTOM",&custom);
			if(!custom) DELNRD(fileno);
			ef=NXTNRDsec(fileno,1);
		}
		if(mod!=NULL) Rfree(mod);
		if(mod1!=NULL) Rfree(mod1);
		mod=stralloc("RPTGEN");
		FINDFLDSETSTRING(fileno,"MODULE NAME",mod);
		ef=GTENRDsec(fileno,1);
		while(!ef)
		{
			FINDFLDGETSTRING(fileno,"MODULE NAME",&mod1);
			if(RDAstrcmp(mod,mod1)) break;
			FINDFLDGETCHAR(fileno,"CUSTOM",&custom);
			if(!custom) DELNRD(fileno);
			ef=NXTNRDsec(fileno,1);
		}
		if(mod!=NULL) Rfree(mod);
		if(mod1!=NULL) Rfree(mod1);
		mod=stralloc("SECURITY");
		FINDFLDSETSTRING(fileno,"MODULE NAME",mod);
		ef=GTENRDsec(fileno,1);
		while(!ef)
		{
			FINDFLDGETSTRING(fileno,"MODULE NAME",&mod1);
			if(RDAstrcmp(mod,mod1)) break;
			FINDFLDGETCHAR(fileno,"CUSTOM",&custom);
			if(!custom) DELNRD(fileno);
			ef=NXTNRDsec(fileno,1);
		}
		if(mod!=NULL) Rfree(mod);
		if(mod1!=NULL) Rfree(mod1);
		addMenuItem(fileno,"DATABASE","00001000","","","","","","N","05","","","","","","Backup Data","00000000","DOBACKUP","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001050","","","","","","N","05","","","","","","Display Tape Contents","00000000","DISPLAYTAPE","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001100","","","","","","N","05","","","","","","Restore Data","00000000","DORESTORE","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001200","","","","","","N","05","","","","","","Zip and FTP Data","00000000","DOFTPZIP","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001300","","","","","","N","05","","","","","","Remove Deleted Records","00000000","REMDEL","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001400","","","","","","N","05","","","","","","Rebuild Data Files","00000000","RBDFIL","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001500","","","","","","N","05","","","","","","Restructure Data Files","00000000","RESTRUCT","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001600","","","","","","N","05","","","","","","Define Import/Export","00000000","DFIE","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00001700","","","","","","N","05","","","","","","Do Import/Export","00000000","DOIE","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002000","DATABASE","MTN DATLOG","","","","N","05","","","","","","Query Database Logs","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002010","DATABASE","MTN DATLOG","","","","N","00","","","","","","Query Database Logs","00000000","RUNMAINTAIN","0000","10","","","","","","");
		addMenuItem(fileno,"DATABASE","00002020","DATABASE","MTN DATTRN.SEARCH","","","","N","00","","","","","","Query Database Log Transactions","00000000","RUNMAINTAIN","0000","20","","","","","","");
		addMenuItem(fileno,"DATABASE","00002050","DATABASE","LOGSTAT","","","","N","05","","","","","","Transaction Log Statistics","00000000","RUNREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002100","DATABASE","PURGE-DATLOG","","","","N","05","","","","","","Purge Logs + Transactions","00000000","RUNREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001000","OLHELP","MTN SCNNOT","","","","N","05","","","","","","Screen Notes","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001010","OLHELP","MTN RSCNOT","","","","N","05","","","","","","Resource Notes","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001020","OLHELP","MTN USCNNOT","","","","N","05","","","","","","User Screen Notes","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001030","OLHELP","MTN URSCNOT","","","","N","05","","","","","","User Resource Notes","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001100","OLHELP","ADD FEEDBACK","","","","N","05","","","","","","Add Feedback","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001150","OLHELP","CYBERSUPPORT","","","","N","05","","","","","","Add Support Request","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001200","OLHELP","MYOPENCALLS","","","","N","05","","","","","","My Open Support Requests","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001210","OLHELP","OUROPENCALLS","","","","N","05","","","","","","Our Open Support Requests","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001300","OLHELP","SETPASSWORD","","","","N","05","","","","","","Set Cyber Support Password","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"OLHELP","00001400","OLHELP","SENDMYVNC","","","","N","05","","","","","","Send My VNC Session","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001000","","","","","","N","05","","","","","","Run Reports","00000000","DOREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001100","","","","","","N","05","","","","","","Define EZ Reports","00000000","EZRPT","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001200","","","","","","N","05","","","","","","Define EZ Virtual Fields","00000000","DFVIR","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001300","","","","","","N","05","","","","","","Define Reports","00000000","DFRPT","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001400","","","","","","N","05","","","","","","Define Output Devices","00000000","DFPRT","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001450","","","","","","N","05","","","","","","Set Up Output Devices","00000000","SETPRT","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001500","","","","","","N","05","","","","","","Laser Print Test","00000000","LASER","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001600","TEST ACTIVE PRINTERS","","","","","N","05","","","","","","Test Active Printers","00000000","MKRPT","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001610","","","","","","N","05","","","","","","Get Laser Forms","00000000","GETFORMS","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001650","UTILITIES","ADD USER REPORT SUBMENU","","","","N","05","","","","","","Add User Report Submenu","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001655","UTILITIES","MTN USERREPORTMENU","","","","N","05","","","","","","User Report Submenu Maintain","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"RPTGEN","00001700","","","","","","N","05","","","","","","Check Email","00000000","CHECKEMAIL","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001000","SECURITY","ADD USERS","","","","N","05","","","","","","Add User Profiles","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001100","SECURITY","COPYSECURITY","","","","N","05","","","","","","Copy Security Profile","00000000","RUNREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001200","SECURITY","ADDMODSEC","","","","N","05","","","","","","Add Module Security","00000000","RUNREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001300","SECURITY","MTN USERS","","","","N","05","","","","","","User Profiles","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001400","SECURITY","MTN MODSEC","","","","N","05","","","","","","Module Security","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001500","SECURITY","MTN DEPARTMENT","","","","N","05","","","","","","Departments","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001550","SECURITY","EMAILUSERSINFO","","","","N","05","","","","","","Email Workstation Info","00000000","RUNREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00001600","SECURITY","EMAILUSERS","","","","N","05","","","","","","Email OpenRDA Users","00000000","RUNREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00002000","SECURITY","MTN USERS.SEARCH","","","","N","00","","","","","","Query User Profiles","00000000","RUNMAINTAIN","0010","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00002010","SECURITY","MTN TOKENS.SEARCH","","","","N","00","","","","","","Query Tokens","00000000","RUNMAINTAIN","0015","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00002100","SECURITY","MTN DEPARTMENT.SEARCH","","","","N","00","","","","","","Query Departments","00000000","RUNMAINTAIN","0020","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00002200","SECURITY","MTN MODSEC.SEARCH","","","","N","00","","","","","","Query Module Access","00000000","RUNMAINTAIN","0030","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00002300","SECURITY","MTN PROCSEC.SEARCH","","","","N","00","","","","","","Query Process Security","00000000","RUNMAINTAIN","0040","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00002400","SECURITY","MTN FILESEC.SEARCH","","","","N","00","","","","","","Query File/Record Security","00000000","RUNMAINTAIN","0050","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00002500","SECURITY","MTN FIELD.SEARCH","","","","N","00","","","","","","Query Field Security","00000000","RUNMAINTAIN","0060","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00003000","SECURITY","USERLIST","","","","N","01","","","","","","Security Users List","00000000","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00003100","SECURITY","USERSLDIF","","","","N","01","","","","","","Export Users to Netscape Address Book (LDIF)","00000000","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00003200","SECURITY","MODACCESS","","","","N","01","","","","","","Module Security Access","00000000","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00003800","SECURITY","SETWTPASSWD","","","","N","02","","","","","","Set WT Password","00000000","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00004000","SECURITY","SETBROWSER","","","","N","02","","","","","","Set Browser Type","00000000","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00004100","SECURITY","SETPROCSEC","","","","N","02","","","","","","Set Process Security","00000000","RUNREPORT","0040","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00004110","SECURITY","CHGRECORDSEC","","","","N","02","","","","","","Change Record Security","00000000","RUNREPORT","0045","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00004200","","","","","","N","02","","","","","","Purge","00000000","RUNPOWERADD","0050","01","","","","","","");
		addMenuItem(fileno,"SECURITY","00004300","SECURITY","PURGEMODSEC","","","","N","02","","","","","","Purge Module Security","00004200","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00004400","SECURITY","PURGEPROCSEC","","","","N","02","","","","","","Purge Process Security","00004200","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00004500","SECURITY","PURGEFILESEC","","","","N","02","","","","","","Purge File/Record Security","00004200","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00004600","SECURITY","PURGEFLDSEC","","","","N","02","","","","","","Purge Field Security","00004200","RUNREPORT","0040","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00005000","SECURITY","COPYPOSLMST","","","","N","03","","","","","","Copy Position Locations to Departments","00000000","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006000","","","","","","N","04","","","","","","Export","00000000","RUNPOWERADD","0010","01","","","","","","");
		addMenuItem(fileno,"SECURITY","00006100","SECURITY","USERS OUT","","","","N","04","","","","","","User Profiles","00006000","DOIE","0010","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006200","SECURITY","MODSEC OUT","","","","N","04","","","","","","Module Access","00006000","DOIE","0020","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006300","SECURITY","PROCSEC OUT","","","","N","04","","","","","","Process Security","00006000","DOIE","0030","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006400","SECURITY","FILESEC OUT","","","","N","04","","","","","","File/Record Security","00006000","DOIE","0040","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006500","SECURITY","FLDSEC OUT","","","","N","04","","","","","","Field Security","00006000","DOIE","0050","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006600","Export","","","","","N","04","","","","","","Import","00000000","RUNPOWERADD","0010","01","","","","","","");
		addMenuItem(fileno,"SECURITY","00006700","SECURITY","USERS IN","","","","N","04","","","","","","User Profiles","00006600","DOIE","0010","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006800","SECURITY","MODSEC IN","","","","N","04","","","","","","Module Access","00006600","DOIE","0020","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00006900","SECURITY","PROCSEC IN","","","","N","04","","","","","","Process Security","00006600","DOIE","0030","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00007000","SECURITY","FILESEC IN","","","","N","04","","","","","","File/Record Security","00006600","DOIE","0040","00","","","","","","");
		addMenuItem(fileno,"SECURITY","00007100","SECURITY","FLDSEC IN","","","","N","04","","","","","","Field Security","00006600","DOIE","0050","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001000","http://wiki.openrda.com/wiki/index.php/Category:PROGRAM_UPDATES","","","","","N","05","","","","","","Download Instructions","00000000","DOHTTP","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001105","","","","","","N","05","","","","","","Program Update Status","00000000","PROGUPD","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001100","","","","","","N","05","","","","","","Download Programs","00000000","DOFTPGET","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001200","","","","","","N","05","","","","","","Update Security","00000000","MKSEC","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001300","","","","","","N","05","","","","","","Update Utilities","00000000","MKUTL","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001400","","","","","","N","05","","","","","","Update Database","00000000","MKDAT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001410","","","","","","N","05","","","","","","Update Report Generator","00000000","MKRPT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001500","","","","","","N","05","","","","","","Update On-Line Help","00000000","MKOLH","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001600","","","","","","N","05","","","","","","Update F.A.S.T.","00000000","MKARCHIVE","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00001700","","","","","","N","05","","","","","","Update GUI","00000000","MKGUI","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002000","","","","","","N","05","","","","","","Update Financial Management","00000000","MKFINMGT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002010","","","","","","N","05","","","","","","Update Bank Reconciliation","00000000","MKBNKREC","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002020","","","","","","N","05","","","","","","Update Vendor Payments","00000000","MKVENPMT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002030","","","","","","N","05","","","","","","Update 1099s","00000000","MK1099","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002040","","","","","","N","05","","","","","","Update Purchase Orders","00000000","MKPURORD","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002045","","","","","","N","05","","","","","","Update OPENSS","00000000","MKOPENSS","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002055","","","","","","N","05","","","","","","Update Requisition Management","00000000","MKREQMGT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002056","","","","","","N","05","","","","","","Update Inventory Control","00000000","MKINVCTL","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002050","","","","","","N","05","","","","","","Update Personnel","00000000","MKPRSNNL","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002060","","","","","","N","05","","","","","","Update Position Tracking","00000000","MKPOSTRK","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002070","","","","","","N","05","","","","","","Update Payroll","00000000","MKPAYROLL","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002080","","","","","","N","05","","","","","","Update W2 / MMREF","00000000","MKW2","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002090","","","","","","N","05","","","","","","Update Benefits Management","00000000","MKBFTMGT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002100","","","","","","N","05","","","","","","Update Leave Management","00000000","MKLVEMGT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002110","","","","","","N","05","","","","","","Update Employee Absentee","00000000","MKEMPABS","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002120","","","","","","N","05","","","","","","Update Substitute Management","00000000","MKSUBMGT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002130","","","","","","N","05","","","","","","Update Budget Preparation","00000000","MKBUDPREP","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002140","","","","","","N","05","","","","","","Update Occupational Taxes / Business License","00000000","MKOCCTAX","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002150","","","","","","N","05","","","","","","Update Misc Billing","00000000","MKMISCBILL","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002160","","","","","","N","05","","","","","","Update Real Estate Taxes","00000000","MKRLSTMGT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002170","","","","","","N","05","","","","","","Update Personal Property Taxes","00000000","MKPROPERTY","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002180","","","","","","N","05","","","","","","Update Building Permits","00000000","MKBLDPRMT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002190","","","","","","N","05","","","","","","Update Fixed Assets","00000000","MKFIXASS","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002200","","","","","","N","05","","","","","","Update Applicant Management","00000000","MKAPPMGT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002210","","","","","","N","05","","","","","","Update Catalogue","00000000","MKCATALOGUE","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002220","","","","","","N","05","","","","","","Update DMV Registration","00000000","MKDMV","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002230","","","","","","N","05","","","","","","Update Utility Billing","00000000","MKUTLBLL","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00002240","","","","","","N","05","","","","","","Update Centralized Collections","00000000","MKCSHDWR","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005000","","","","","","N","05","","","","","","Define Files","00000000","DFNRD","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005005","UTILITIES","MTN MODULE","","","","N","05","","","","","","Modules","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005010","","","","","","N","05","","","","","","Define Screens","00000000","DFSCN","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005020","","","","","","N","05","","","","","","Define Help","00000000","DFHELP","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005030","","","","","","N","05","","","","","","Define EZ-Virtual Fields","00000000","DFVIR","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005040","","","","","","N","05","","","","","","Define EZ Reports","00000000","EZRPT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005050","","","","","","N","05","","","","","","Define Accounts","00000000","DFACCT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005060","","","","","","N","05","","","","","","Define Import/Export","00000000","DFIE","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005070","","","","","","N","05","","","","","","Define Tables","00000000","DFTABLE","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005080","","","","","","N","05","","","","","","Define Scrolled Lists","00000000","DFLIST","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005090","","","","","","N","05","","","","","","Define Screen Defaults","00000000","DFSCNDEF","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005100","","","","","","N","05","","","","","","Define Matrix","00000000","DFMATRIX","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005110","","","","","","N","05","","","","","","Define Maintain Master","00000000","DFMTN","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005120","","","","","","N","05","","","","","","Define Power Add","00000000","DFADD","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005130","","","","","","N","05","","","","","","Define Reports","00000000","DFRPT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005135","UTILITIES","MTN MENUITEM","","","","N","05","","","","","","Menu Items","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005140","","","","","","N","05","","","","","","Define Load Buttons","00000000","DFLOAD","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005150","","","","","","N","05","","","","","","Define Generic Setup Variables","00000000","DFGSV","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005160","","","","","","N","05","","","","","","Define Change Files","00000000","DFCHG","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005170","","","","","","N","05","","","","","","Define Output Devices","00000000","DFPRT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005175","","","","","","N","05","","","","","","Set Up Output Devices","00000000","SETPRT","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005180","","","","","","N","05","","","","","","Define WWW Interface","00000000","DFWWW","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005190","","","","","","N","05","","","","","","OpenRDA Setup","00000000","XPERTSTP","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005600","UTILITIES","MVFINCOA","","","","N","02","","","","","","Move COA Components","00000000","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005610","","","","","","N","02","","","","","","Delete Sort Files","00000000","MASSDEL","0020","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005620","","","","","","N","02","","","","","","Remove Deleted Records in Mass","00000000","MASSREMDEL","0030","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005630","","","","","","N","02","","","","","","Check Files","00000000","CHECKFILE","0040","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005640","","","","","","N","02","","","","","","Create Import/Export in Mass","00000000","CRTIMPEXP","0050","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005650","","","","","","N","02","","","","","","Execute Import/Export in Mass","00000000","MASSIE","0060","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005660","","","","","","N","02","","","","","","Change Database Engine","00000000","CHGENGINE","0070","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005670","","","","","","N","02","","","","","","Display Export Differences","00000000","MASSDIFF","0080","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005800","UTILITIES","MTN MODULE","","","","N","00","","","","","","Modules","00000000","RUNMAINTAIN","0010","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005810","UTILITIES","MTN MENUITEM.SEARCH","","","","N","00","","","","","","Menu Items","00000000","RUNMAINTAIN","0020","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005820","UTILITIES","MTN MSG-GATEWAY","","","","N","00","","","","","","Message Gateways","00000000","RUNMAINTAIN","0030","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00006000","","","","","","N","05","","","","","","OpenRDA Diagnostics","00000000","RDADIAG","0000","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00008000","UTILITIES","SYSTEM WIDE BUNDLES","","","","N","04","","","","","","System Wide Bundles","00000000","DOGSV","0010","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00008100","UTILITIES","","","","","N","03","","","","","","Generic Setup Variables","00000000","DOGSV","0010","00","","","","","","");


		addMenuItem(fileno,"DATABASE","00010000","UTILITIES","","","","","N","02","","","","","","Purge","00000000","DOGSV","0200","01","","","","","","([DISPLAY PURGE FUNCTIONS]=TRUE)");
		if(XPERT_SETUP->FINMGT==TRUE)
		{
		addMenuItem(fileno,"DATABASE","00010100","UTILITIES","","","","","N","02","","","","","","Financial Management","00010000","DOGSV","0010","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00535500","","","","","","N","02","","","","","","Account Masters","00010100","RUNREPORT","0010","01","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00536000","FINMGT","PURGE-FINEACC","","","","N","02","","","","","","Expenditure(FINEACC)","00535500","RUNREPORT","0010","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00536500","FINMGT","PURGE-FINRACC","","","","N","02","","","","","","Revenue(FINRACC)","00535500","RUNREPORT","0020","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00537000","FINMGT","PURGE-FINBACC","","","","N","02","","","","","","G/L (FINBACC)","00535500","RUNREPORT","0030","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00537500","","","","","","N","02","","","","","","Account Activity","00010100","RUNPOWERADD","0030","01","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00538000","FINMGT","PURGE-FINEACT","","","","N","02","","","","","","Expenditure(FINEACT)","00537500","RUNREPORT","0010","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00538500","FINMGT","PURGE-FINRACT","","","","N","02","","","","","","Revenue(FINRACT)","00537500","RUNREPORT","0020","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00539000","FINMGT","PURGE-FINBACT","","","","N","02","","","","","","G/L (FINBACT)","00537500","RUNREPORT","0030","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00539500","","","","","","N","02","","","","","","Account Year Masters","00010100","RUNPOWERADD","0020","01","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00540000","FINMGT","PURGE-FINEYR","","","","N","02","","","","","","Expenditure(FINEYR)","00539500","RUNREPORT","0010","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00540500","FINMGT","PURGE-FINRYR","","","","N","02","","","","","","Revenue(FINRYR)","00539500","RUNREPORT","0020","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00541100","FINMGT","PURGE-FINBYR","","","","N","02","","","","","","G/L (FINBYR)","00539500","RUNREPORT","0030","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00541600","FINMGT","JEPURGE","","","","N","02","","","","","","Journal Entries(FINJEN)","00010100","RUNREPORT","0030","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00542000","FINMGT","RECPURGE","","","","N","02","","","","","","Receipts(FINREC)","00010100","RUNREPORT","0050","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[FINMGT][SWB_FM17_PURGE]\")))");
		}



		if(XPERT_SETUP->BNKREC==TRUE)
		{
		addMenuItem(fileno,"DATABASE","00010200","UTILITIES","","","","","N","02","","","","","","Bank Reconciliation","00010000","DOGSV","0020","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00006552","BNKREC","CHKPURGE","","","","N","02","","","","","","Checks","00010200","RUNREPORT","0005","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[BNKREC][SWB_BR04_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006554","BNKREC","DEPPURGE","","","","N","02","","","","","","Deposits","00010200","RUNREPORT","0010","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[BNKREC][SWB_BR04_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006556","BNKREC","ETRPURGE","","","","N","02","","","","","","Electronic Transfers","00010200","RUNREPORT","0015","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[BNKREC][SWB_BR04_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006558","BNKREC","CHGPURGE","","","","N","02","","","","","","Service Charges","00010200","RUNREPORT","0020","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[BNKREC][SWB_BR04_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006560","BNKREC","RTNPURGE","","","","N","02","","","","","","Returned Checks","00010200","RUNREPORT","0025","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[BNKREC][SWB_BR04_PURGE]\")))");
		}


#ifdef __ONCE_CREATED__
		if(XPERT_SETUP->FIXASS)
		{
		addMenuItem(fileno,"DATABASE","00010300","UTILITIES","","","","","N","02","","","","","","Fixed Assets","00010000","DOGSV","0030","01","","","","","","");
		}
#endif /* __ONCE_CREATED__ */



		if(XPERT_SETUP->BUDPREP)
		{
		addMenuItem(fileno,"DATABASE","00010400","UTILITIES","","","","","N","02","","","","","","Budget Preparation","00010000","DOGSV","0040","01","","","","","","");

		addMenuItem(fileno,"DATABASE","00670200","BUDPREP","PURGEBUDEXP","","","","N","02","","","","","","Expenditure Budgets","00010400","RUNREPORT","0012","00","","","","","",swb_bp07_purge);
		addMenuItem(fileno,"DATABASE","00670400","BUDPREP","PURGEBUDREV","","","","N","02","","","","","","Revenue Budgets","00010400","RUNREPORT","0010","00","","","","","",swb_bp07_purge);
		addMenuItem(fileno,"DATABASE","00670600","BUDPREP","PURGEBUDEACC","","","","N","02","","","","","","Expenditure Accounts","00010400","RUNREPORT","0015","00","","","","","",swb_bp07_purge);
		addMenuItem(fileno,"DATABASE","00670800","BUDPREP","PURGEBUDRACC","","","","N","02","","","","","","Revenue Accounts","00010400","RUNREPORT","0020","00","","","","","",swb_bp07_purge);
		addMenuItem(fileno,"DATABASE","00671000","BUDPREP","PURGEBUDEACT","","","","N","02","","","","","","Expenditure Activity","00010400","RUNREPORT","0025","00","","","","","",swb_bp07_purge);
		addMenuItem(fileno,"DATABASE","00671200","BUDPREP","PURGEBUDRACT","","","","N","02","","","","","","Revenue Activity","00010400","RUNREPORT","0030","00","","","","","",swb_bp07_purge);
		addMenuItem(fileno,"DATABASE","00671300","BUDPREP","MTN BUDPMSTR PURGE","","","","N","02","","","","","","Budget Pay Masters","00010400","RUNMAINTAIN","0033","00","","","","","","([SOFTWARE TYPE]=0) OR (((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP04_PAYROLL]\"))))");
		addMenuItem(fileno,"DATABASE","00671400","BUDPREP","PURGEBUDJMST","","","","N","02","","","","","","Job/Vacancy Masters","00010400","RUNREPORT","0035","00","","","","","","([SOFTWARE TYPE]=0) OR (((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP04_PAYROLL]\"))))");
		addMenuItem(fileno,"DATABASE","00671600","BUDPREP","PURGEBUDJAMS","","","","N","02","","","","","","Job Accounts","00010400","RUNREPORT","0040","00","","","","","","([SOFTWARE TYPE]=0) OR (((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP04_PAYROLL]\"))))");
		addMenuItem(fileno,"DATABASE","00671800","BUDPREP","PRGBUDACC","","","","N","02","","","","","","Position Accounts","00010400","RUNREPORT","0045","00","","","","","","([SOFTWARE TYPE]=0) OR (((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP04_PAYROLL]\"))))");
		addMenuItem(fileno,"DATABASE","00672000","BUDPREP","PURGEBUDDEDM","","","","N","02","","","","","","Employee Deduction Masters","00010400","RUNREPORT","0050","00","","","","","","([SOFTWARE TYPE]=0) OR (((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP04_PAYROLL]\"))))");
		addMenuItem(fileno,"DATABASE","00672200","BUDPREP","PRGEBUDGRS","","","","N","02","","","","","","Gross IDs","00010400","RUNREPORT","0055","00","","","","","","([SOFTWARE TYPE]=0) OR (((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP04_PAYROLL]\"))))");
		addMenuItem(fileno,"DATABASE","00672400","BUDPREP","PRGEBUDGRAT","","","","N","02","","","","","","Gross Rate Masters","00010400","RUNREPORT","0060","00","","","","","","([SOFTWARE TYPE]=0) OR (((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP04_PAYROLL]\"))))");
		addMenuItem(fileno,"DATABASE","00672600","BUDPREP","PURGEBUDCAP","","","","N","02","","","","","","Capital Outlays","00010400","RUNREPORT","0065","00","","","","","","(((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP07_PURGE]\"))) AND ((GENERIC_SETUP_VARIABLE(\"[BUDPREP][SWB_BP03_CAPITAL_OUTLAYS]\")))) OR ([SOFTWARE TYPE]=0)");
		}


		if(XPERT_SETUP->VENPMT)
		{
		addMenuItem(fileno,"DATABASE","00010500","UTILITIES","","","","","N","02","","","","","","Vendor Payments","00010000","DOGSV","0050","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00006137","VENPMT","VCHPURGE","","","","N","02","","","","","","Vendor Vouchers","00010500","RUNREPORT","0005","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006140","VENPMT","VENINVPURGE","","","","N","02","","","","","","Vendor Invoices","00010500","RUNREPORT","0010","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006142","VENPMT","SETCYDATA","","","","N","02","","","","","","Re-Establish Calendar Year Data File","00010500","RUNREPORT","0015","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006144","VENPMT","SETFYDATA","","","","N","02","","","","","","Re-Establish Fiscal Year Data File","00010500","RUNREPORT","0020","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006131","VENPMT","NOVENMSRCALYR","","","","N","02","","","","","","View / Purge Vendors with Calendar Years","00010500","RUNREPORT","0023","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006132","VENPMT","NOVENVCH","","","","N","02","","","","","","Vendors without Vouchers","00010500","RUNREPORT","0025","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006138","VENPMT","PURGEVENMSR","","","","N","02","","","","","","Vendor Masters","00010500","RUNREPORT","0030","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006141","VENPMT","PURGEVENDOC","","","","N","02","","","","","","Vendor Documents","00010500","RUNREPORT","0033","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006146","VENPMT","VENFYRPURGE","","","","N","02","","","","","","Vendor Fiscal Years","00010500","RUNREPORT","0035","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006148","VENPMT","VENCYRPURGE","","","","N","02","","","","","","Vendor Calendar Years","00010500","RUNREPORT","0040","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006134","VENPMT","VENMSRPURGE","","","","N","02","","","","","","Vendor Masters Based on Activity","00010500","RUNREPORT","0045","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP09_ENHANCED_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00006150","VENPMT","PRGRVCH","","","","N","02","","","","","","Recurring Vouchers","00010500","RUNREPORT","0050","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[VENPMT][SWB_VP03_RECURRING_VOUCHERS]\")))");
		}


		if(XPERT_SETUP->PURORD)
		{
		addMenuItem(fileno,"DATABASE","00010600","UTILITIES","","","","","N","02","","","","","","Purchase Orders","00010000","DOGSV","0060","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00638200","PURORD","PURGEPURMST","","","","N","02","","","","","","PO Masters","00010600","RUNREPORT","0005","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PURORD][SWB_PO07_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00638300","PURORD","PURGEPURLIN","","","","N","02","","","","","","PO Lines","00010600","RUNREPORT","0007","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PURORD][SWB_PO07_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00638400","PURORD","PURGEPODETAIL","","","","N","02","","","","","","PO Lines/Accounts/Transactions","00010600","RUNREPORT","0010","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PURORD][SWB_PO07_PURGE]\")))");
		}



		if(XPERT_SETUP->CATALOGUE)
		{
		addMenuItem(fileno,"DATABASE","00010700","UTILITIES","","","","","N","02","","","","","","Catalogue","00010000","DOGSV","0070","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00008340","CATALOGUE","PRGITEM","","","","N","02","","","","","","Purge Catalogue Items","00010700","RUNREPORT","0099","00","","","","","","");
		}



		if(XPERT_SETUP->PRSNNL)
		{
		addMenuItem(fileno,"DATABASE","00010800","UTILITIES","","","","","N","02","","","","","","Personnel","00010000","DOGSV","0080","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00008430","PRSNNL","PURGEPERDMG","","","","N","02","","","","","","Personnel Demographics","00010800","RUNREPORT","0005","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00008435","PRSNNL","PURGEPERDOC","","","","N","02","","","","","","Personnel Documents","00010800","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00008437","PRSNNL","PURGEDEPENDENTS","","","","N","02","","","","","","Dependents","00010800","RUNREPORT","0015","00","","","","","","");
		}



		if(XPERT_SETUP->PAYROLL)
		{
		addMenuItem(fileno,"DATABASE","00010900","UTILITIES","","","","","N","02","","","","","","Payroll","00010000","DOGSV","0090","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00812000","PAYROLL","PURGE-PAYDTRN","","","","N","02","","","","","","Deduction Transactions","00010900","RUNREPORT","0002","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00812200","PAYROLL","PURGE-PAYJTRN","","","","N","02","","","","","","Job Transactions","00010900","RUNREPORT","0004","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00812400","PAYROLL","PURGE-PAYJDPM","","","","N","02","","","","","","Job Deduction Pay Masters","00010900","RUNREPORT","0006","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00812600","PAYROLL","PURGE-PAYDPMS","","","","N","02","","","","","","Deduction Pay Masters","00010900","RUNREPORT","0008","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00812800","PAYROLL","PURGEPAYID","","","","N","02","","","","","","Pay Masters (All Files)","00010900","RUNREPORT","0010","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00813000","PAYROLL","PURGE-PAYDDSV","","","","N","02","","","","","","Deduction Vouchers","00010900","RUNREPORT","0012","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00813100","PAYROLL","NOVWHISTORY","","","","N","02","","","","","","Deduction Descriptions w/o VW History","00010900","RUNREPORT","0014","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		addMenuItem(fileno,"DATABASE","00813110","PAYROLL","NOEMPLOYEEDEDS","","","","N","02","","","","","","Deduction Descriptions w/o Employee Deduction Masters","00010900","RUNREPORT","0016","00","","","","","","([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[PAYROLL][SWB_PR14_PURGE]\")))");
		}



		if(XPERT_SETUP->LVEMGT)
		{
		addMenuItem(fileno,"DATABASE","00011000","UTILITIES","","","","","N","02","","","","","","Leave Management","00010000","DOGSV","0100","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00086920","LVEMGT","PRGLVETRAN","","","","N","02","","","","","","Purge Leave Transactions","00011000","RUNREPORT","0002","00","","","","","",swb_lv07_purge);
		}


		if(XPERT_SETUP->EMPABS)
		{
		addMenuItem(fileno,"DATABASE","00011100","UTILITIES","","","","","N","02","","","","","","Employee Absentee","00010000","DOGSV","0110","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00854105","EMPABS","PRGEMPABS","","","","N","02","","","","","","Purge Absences","00011100","RUNREPORT","0006","00","","","","","","");
		}


		if(XPERT_SETUP->SUBMGT)
		{
		addMenuItem(fileno,"DATABASE","00011200","UTILITIES","","","","","N","02","","","","","","Substitute Management","00010000","DOGSV","0120","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00844600","SUBMGT","PRGSUBWKMAS","","","","N","02","","","","","","Substitute Work and Payment Records","00011200","RUNREPORT","0002","00","","","","","","");
		}


		if(XPERT_SETUP->APPMGT)
		{
		addMenuItem(fileno,"DATABASE","00011300","UTILITIES","","","","","N","02","","","","","","Applicant Management","00010000","DOGSV","0130","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00848900","APPMGT","PRGAPPCIDS","","","","N","02","","","","","","Applicant IDs","00011300","RUNREPORT","0002","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00848910","APPMGT","PURGEAPPCNT","","","","N","02","","","","","","Applicants","00011300","RUNREPORT","0002","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00848920","APPMGT","PURGEAPPCAT","","","","N","02","","","","","","Applications","00011300","RUNREPORT","0002","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00848925","APPMGT","PURGEAPPDOC","","","","N","02","","","","","","Documents","00011300","RUNREPORT","0002","00","","","","","","");
		}


		if(XPERT_SETUP->PROPERTY)
		{
		addMenuItem(fileno,"DATABASE","00011400","UTILITIES","","","","","N","02","","","","","","Personal Property","00010000","DOGSV","0140","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00012815","PROPERTY","PURGEPRPIDS","","","","N","02","","","","","","Supplement Identifications","00011400","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00012900","PROPERTY","PURGETRNPAY","","","","N","02","","","","","","Transactions and Payments","00011400","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00012810","PROPERTY","PURGEPAY","","","","N","02","","","","","","Payments","00011400","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00013110","PROPERTY","PURGEPRPDET","","","","N","02","","","","","","Details","00011400","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00013000","PROPERTY","PRGPRPSTM","","","","N","02","","","","","","Statements","00011400","RUNREPORT","0040","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00013100","PROPERTY","PURGEPRPMST","","","","N","02","","","","","","Property Masters","00011400","RUNREPORT","0050","00","","","","","","");
		}


		if(XPERT_SETUP->RLSTMGT)
		{
		addMenuItem(fileno,"DATABASE","00011500","UTILITIES","","","","","N","02","","","","","","Real Estate","00010000","DOGSV","0150","01","","","","","","");
		addMenuItem(fileno,"DATABASE","00002105","RLSTMGT","PURGERESTIDS","","","","N","03","","","","","","Supplement Identifications","00011500","RUNREPORT","0001","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002200","RLSTMGT","PURGERESTMIL","","","","N","02","","","","","","Millages","00011500","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002300","RLSTMGT","PURGEPAID","","","","N","02","","","","","","Payments","00011500","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002400","RLSTMGT","PURGETRANS","","","","N","02","","","","","","Transactions","00011500","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002500","RLSTMGT","PURGETRANPAY","","","","N","02","","","","","","Transactions and Payments","00011500","RUNREPORT","0040","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002600","RLSTMGT","PURGETRANSAM","","","","N","02","","","","","","Transam","00011500","RUNREPORT","0050","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002700","RLSTMGT","PURGERESTMSTR","","","","N","02","","","","","","Masters","00011500","RUNREPORT","0060","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002800","RLSTMGT","PRGRLSTSTM","","","","N","02","","","","","","Statement Master","00011500","RUNREPORT","0070","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00002900","RLSTMGT","PURGEMEXP","","","","N","02","","","","","","Owner Exemption Masters","00011500","RUNREPORT","0090","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00003000","RLSTMGT","PURGEMMEXP","","","","N","02","","","","","","Millage Exemption Masters","00011500","RUNREPORT","0100","00","","","","","","");
		addMenuItem(fileno,"DATABASE","00003100","RLSTMGT","PURGERESTNEW","","","","N","02","","","","","","Assessments","00011500","RUNREPORT","0110","00","","","","","","");
		}


		if(XPERT_SETUP->MISCBILL)
		{
		addMenuItem(fileno,"DATABASE","00011600","UTILITIES","","","","","N","02","","","","","","Misc Billing","00010000","DOGSV","0160","01","","","","","","");
		addMenuItem(fileno,"DATABASE","02002900","MISCBILL","PURGEPAY","","","","N","02","","","","","","Payments","00011600","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02003000","MISCBILL","PURGETRANS","","","","N","02","","","","","","Transactions","00011600","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02003100","MISCBILL","PURGEMASTERS","","","","N","02","","","","","","Misc Billing Masters","00011600","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02003110","MISCBILL","PURGEMISCDOC","","","","N","02","","","","","","Misc Billing Documents","00011600","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02003150","MISCBILL","PRGMISCIDS","","","","N","02","","","","","","Misc ID Masters","00011600","RUNREPORT","0040","00","","","","","","");
		}


		if(XPERT_SETUP->BLDPRMT)
		{
		addMenuItem(fileno,"DATABASE","00011700","UTILITIES","","","","","N","02","","","","","","Building Permits","00010000","DOGSV","0170","01","","","","","","");
		addMenuItem(fileno,"DATABASE","03001800","BLDPRMT","PRGPRMTMSTR","","","","N","02","","","","","","Permit Masters","00011700","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"DATABASE","03001900","BLDPRMT","PRGPRMTTRAN","","","","N","02","","","","","","Permit Transactions","00011700","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"DATABASE","03002000","BLDPRMT","PRGPRMTPAID","","","","N","02","","","","","","Permit Payments","00011700","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"DATABASE","03002100","BLDPRMT","PRGPRMTINSP","","","","N","02","","","","","","Permit Inspection Masters","00011700","RUNREPORT","0040","00","","","","","","");
		addMenuItem(fileno,"DATABASE","03002200","BLDPRMT","PRGPRMTINSD","","","","N","02","","","","","","Permit Inspection Date Masters","00011700","RUNREPORT","0050","00","","","","","","");
		addMenuItem(fileno,"DATABASE","03002300","BLDPRMT","PRGPRMTCIDS","","","","N","02","","","","","","Identifications","00011700","RUNREPORT","0060","00","","","","","","");
		}


		if(XPERT_SETUP->OCCTAX)
		{
		addMenuItem(fileno,"DATABASE","00011800","UTILITIES","","","","","N","02","","","","","","Occ. Tax/Bus. License","00010000","DOGSV","0180","01","","","","","","");
		addMenuItem(fileno,"DATABASE","04018700","OCCTAX","PURGERECEIPTS","","","","N","02","","","","","","Receipts","00011800","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"DATABASE","04018800","OCCTAX","PURGECHARGES","","","","N","02","","","","","","Charges","00011800","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"DATABASE","04018900","OCCTAX","PURGEBUSMSTR","","","","N","02","","","","","","Businesses","00011800","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"DATABASE","04018910","OCCTAX","PURGEOCCDOC","","","","N","02","","","","","","Documents","00011800","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"DATABASE","04019000","OCCTAX","PRGGPLICENSE","","","","N","02","","","","","","Group Licenses Numbers","00011800","RUNREPORT","0040","00","","","","","","");
		}


		if(XPERT_SETUP->UTLBLL)
		{
		addMenuItem(fileno,"DATABASE","00011900","UTILITIES","","","","","N","02","","","","","","Utility Billing","00010000","DOGSV","0190","01","","","","","","");
		addMenuItem(fileno,"DATABASE","02061700","UTLBLL","PRGUTBUSG","","","","N","02","","","","","","Usage","00011900","RUNREPORT","0040","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02061800","UTLBLL","PURGE TRANSACT","","","","N","02","","","","","","Transactions","00011900","RUNREPORT","0050","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02061900","UTLBLL","CREATEPURGECHG","","","","N","02","","","","","","Create Purged Transactions (9999)","00011900","RUNREPORT","0060","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02062000","UTLBLL","PURGE CHARGES","","","","N","02","","","","","","Charges","00011900","RUNREPORT","0070","00","","","","","","");
		addMenuItem(fileno,"DATABASE","02062100","UTLBLL","PURGESVCS","","","","N","02","","","","","","Services","00011900","RUNREPORT","0080","00","","","","","","");
		}


		if(XPERT_SETUP->CSHDWR)
		{
		addMenuItem(fileno,"DATABASE","00012000","UTILITIES","","","","","N","02","","","","","","Collections","00010000","DOGSV","0200","01","","","","","","");
		addMenuItem(fileno,"DATABASE","05015300","CSHDWR","PRGCSHTRN","","","","N","02","","","","","","Transactions","00012000","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"DATABASE","05015400","CSHDWR","PRGCSHTRNDET","","","","N","02","","","","","","Transactions with Detail","00012000","RUNREPORT","0030","00","","","","","","");
		}





		addMenuItem(fileno,"UTILITIES","00005700","","","","","","N","02","","","","","","Purge","00000000","MASSDIFF","0090","01","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005710","UTILITIES","PURGE-MODULE","","","","N","02","","","","","","Purge Modules","00005700","RUNREPORT","5010","00","","","","","","");
		addMenuItem(fileno,"UTILITIES","00005720","UTILITIES","PURGE-MENUITEM","","","","N","02","","","","","","Purge Menu Items","00005700","RUNREPORT","5020","00","","","","","","");






		CLSNRD(fileno);
	}
}
