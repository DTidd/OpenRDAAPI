/* UTILITIES's - Make (MTN MODULE) Maintain Master */
#include <cstdio>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mknrd.hpp>
#include <mkdef.hpp>

extern char *module;
extern APPlib *errorlist;
char SWB_BP04_PAYROLL=FALSE;

void AddModuleRecord(short fileno,short Group,short Rank,char *m,char *desc)
{
	ZERNRD(fileno);
	FINDFLDSETSHORT(fileno,"GROUP",Group);
	FINDFLDSETSHORT(fileno,"RANK",Rank);
	FINDFLDSETSTRING(fileno,"MODULE NAME",m);
	FINDFLDSETSTRING(fileno,"DESCRIPTION",desc);
	WRTNRD(fileno,0,NULL);
}
void DB_MODULE()
{
	DBsort *sort=NULL;
	short x=(-1),ef=0,g=0;
	char same=FALSE;
	char *temp1=NULL;
	RDATData *prev=NULL;

	char *libx=NULL,c=FALSE;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("BUDPREP")+11);

#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"BUDPREP");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"BUDPREP");
#endif
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("BUDPREP","SWB_BP04_PAYROLL");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_BP04_PAYROLL=*gsv->value.string_value;
		}
	}
	if(XPERT_SETUP->software_type<1)
	{
		SWB_BP04_PAYROLL=TRUE;
	} 
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	sort=DBsortNEW("UTILITIES","MODULE",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"GROUP",7,2);
		addDBfield(sort,"RANK",7,4);
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"DESCRIPTION",1,40);
		addDBfield(sort,"CUSTOM",10,1);
		addDBkey(sort,"MODULE KEY 1");
		addDBkeypart(sort,1,"GROUP");
		addDBkeypart(sort,1,"RANK");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkey(sort,"MODULE KEY 2");
		addDBkeypart(sort,2,"MODULE NAME");
		addDBkeypart(sort,2,"GROUP");
		addDBkeypart(sort,2,"RANK");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+9+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [UTILITIES] Screen [MODULE], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
	x=OPNNRD("UTILITIES","MODULE");
	if(x!=(-1))
	{
		ZERNRD(x);
		ef=FRSNRD(x,1);
		while(!ef)
		{
			FINDFLDGETSHORT(x,"GROUP",&g);
			if(g<6) 
			{
				FINDFLDGETCHAR(x,"CUSTOM",&c);
				if(c==FALSE)
				{
					prev=RDATDataNEW(x);
					FINDFLDSETCHAR(x,"DELETEFLAG",TRUE);
					WRTTRANS(x,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
					DELNRD(x);
				}
			}
			ef=NXTNRD(x,1);
		}
		AddModuleRecord(x,0,10,"FINMGT","Financial Management");
		AddModuleRecord(x,0,20,"BNKREC","Bank Reconciliation");
		AddModuleRecord(x,0,30,"FIXASS","Fixed Assets");
		AddModuleRecord(x,1,10,"BUDPREP","Budget Preparation");
		if(SWB_BP04_PAYROLL)
		{
		AddModuleRecord(x,1,20,"BUDPAY","Budget Preparation Payroll");
		}
		AddModuleRecord(x,2,10,"VENPMT","Vendor Payments");
		AddModuleRecord(x,2,20,"1099","1099s");
		AddModuleRecord(x,2,30,"PURORD","Purchase Orders");
		AddModuleRecord(x,2,40,"CATALOGUE","Catalogue");
		AddModuleRecord(x,2,50,"REQMGT","Requisitions");
		AddModuleRecord(x,2,60,"INVCTL","Inventory Control");
		AddModuleRecord(x,3,10,"PRSNNL","Personnel");
		AddModuleRecord(x,3,20,"PAYROLL","Payroll");
		AddModuleRecord(x,3,30,"VW","Payroll Vendor Withholding");
		AddModuleRecord(x,3,40,"W2","Payroll W2 Processing");
		AddModuleRecord(x,3,50,"POSTRK","Position Tracking");
		AddModuleRecord(x,3,60,"LVEMGT","Leave Management");
		AddModuleRecord(x,3,70,"EMPABS","Employee Absentee");
		AddModuleRecord(x,3,80,"SUBMGT","Substitute Management");
		AddModuleRecord(x,3,90,"CPI","GA CPI");
		AddModuleRecord(x,3,100,"APPMGT","Applicant Management");
		AddModuleRecord(x,4,10,"CSHDWR","Cash Drawer");
		AddModuleRecord(x,4,31,"MISCBCHG","Misc Billing Charges");
		AddModuleRecord(x,4,32,"MISCBREC","Misc Billing Receipts");
		AddModuleRecord(x,4,40,"BLDPRMT","Building Permits");
		AddModuleRecord(x,4,50,"OCCTAX","Occupational Tax/Bus. License");
		AddModuleRecord(x,4,61,"DMVLIVE","Motor Vehicle Live");
		AddModuleRecord(x,4,62,"DMVWORK","Motor Vehicle Work");
		AddModuleRecord(x,4,65,"PROPCHG","Personal Property Charges");
		AddModuleRecord(x,4,66,"PROPREC","Personal Property Receipts");
		AddModuleRecord(x,4,66,"PROPSTM","Personal Property Statements");
		AddModuleRecord(x,4,71,"RLSTCHG","Real Estate Charges");
		AddModuleRecord(x,4,72,"RLSTREC","Real Estate Receipts");
		AddModuleRecord(x,4,73,"RLSTSTM","Real Estate Statements");
		AddModuleRecord(x,4,77,"VAIRMSGET","VA IRMS");
		AddModuleRecord(x,4,81,"UTLBLLCUS","Utility Billing Customers");
		AddModuleRecord(x,4,82,"UTLBLLMTR","Utility Billing Meters");
		AddModuleRecord(x,4,83,"UTLBLLCHG","Utility Billing Charges");
		AddModuleRecord(x,4,84,"UTLBLLREC","Utility Billing Receipts");
		/*
		AddModuleRecord(x,4,85,"UTLBLLPDUE","Utility Billing Past Due");
		*/
		AddModuleRecord(x,4,86,"UTLBLLWKO","Utility Billing Work Order");
		AddModuleRecord(x,5,10,"ARCHIVE","F.A.S.T. / Archive");
		AddModuleRecord(x,5,20,"DATABASE","Database / Transaction Processing");
		AddModuleRecord(x,5,30,"OLHELP","On-Line Help / Services");
		AddModuleRecord(x,5,40,"RPTGEN","Report Generation / Devices");
		AddModuleRecord(x,5,50,"SECURITY","Security");
		AddModuleRecord(x,5,60,"UTILITIES","Utilities / Tools");
		AddModuleRecord(x,5,70,"VERSION","Version Control");
		CLSNRD(x);
	}
}
static void MTN_MTN_MODULE()
{
	char *defdir=NULL;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN MODULE",temp1,1,"MODULE","MODULE BROWSE","MODULE DEFINE LIST","MODULE SEARCH BROWSE","MODULE MAINTAIN SCREEN",0,1,0,0,temp2,temp3,temp5,temp4,temp6,FALSE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
		SetRDAsearchOccurrence(MTNMSTR);


		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MTN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,module);
#endif
		if(writeMaintainMaster(defdir,MTNMSTR))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+9+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [UTILITIES] Maintain Master [MTN MODULE], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		} else {
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		}
	}
}
static void MNU_UTILITIES_MTN_MODULE()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("UTILITIES MTN MODULE","mtnmst.EXT UTILITIES \"MTN MODULE\"");
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
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
static void DEF_MODULE_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("UTILITIES","MODULE DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"MODULE NAME POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"MODULE NAME LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"DESCRIPTION POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"DESCRIPTION LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"GROUP POSITION",TRUE,7,1);
		addDefaultFieldSHORT(def,"GROUP LENGTH",TRUE,7,2);
		addDefaultFieldSHORT(def,"RANK POSITION",TRUE,7,2);
		addDefaultFieldSHORT(def,"RANK LENGTH",TRUE,7,4);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+9+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [UTILITIES] Screen [MODULE DEFINE LIST], Can Not Save Screen Defaults!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(def!=NULL) FreeRDAdefault(def);
	}
}
static void SCN_MODULE_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MODULE BROWSE");
	if(scrn!=NULL)
	{
		temp1=Rmalloc(23+1);
		sprintf(temp1,"%s",
			"[EXTENSIONS SETUP]=TRUE");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PRINT BROWSE","Print Browse","","",0,0,7,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search Field:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Position","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST POSITION","Position","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Count","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG TOTAL","Count","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,16,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(13+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MODULE BROWSE], Can Not Save Maintain Master!");
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
static void SCN_MODULE_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MODULE DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Rank:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESCRIPTION POSITION","Description Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DESCRIPTION LENGTH","Description Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"GROUP POSITION","Group Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"GROUP LENGTH","Group Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE NAME POSITION","Module Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE NAME LENGTH","Module Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RANK POSITION","Rank Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RANK LENGTH","Rank Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(18+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MODULE DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_MODULE_MAINTAIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MODULE MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"[MODULE][DELETEFLAG]","Delete this record","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"MODULE GROUPS","Module Group","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Rank within Group:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MODULE][RANK]","Module Rank within Group","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS MODULE KEY 1","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MODULE][MODULE NAME]","Module Name:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT MODULE KEY 1","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"[MODULE][CUSTOM]","Custom","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MODULE][DESCRIPTION]","Description:","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BROWSE","Browse","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET DEFAULTS","Reset","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,24,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(22+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MODULE MAINTAIN SCREEN], Can Not Save Maintain Master!");
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
static void SCN_MODULE_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MODULE SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MODULE][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MODULE][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][DESCRIPTION]","Range on Description","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][DESCRIPTION]","From Description","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][DESCRIPTION]","To Description","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][GROUP]","Range on Group","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][GROUP]","From Group","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][GROUP]","To Group","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MODULE][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MODULE][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MODULE][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Query:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,14,"MODULE SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(20+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MODULE SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_MODULE_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MODULE SEARCH BROWSE EXPRESSION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"EXPRESSION","Expression","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD VALUE","Load Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD GROUPER","Load Grouper","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD OPERATOR","Load Operator","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD CONTROL","Load Control","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(31+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MODULE SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void LST_MODULE_GROUPS()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("UTILITIES","MODULE GROUPS");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=3;
		ScrolledListDFincvir(list,"UTILITIES","MODULE","",0);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=0;
		list->conmod=stralloc("UTILITIES");
		list->confil=stralloc("MODULE");
		list->confld=stralloc("GROUP");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"\"Finance\"");
		addAPPlib(list->list,"\"Budget\"");
		addAPPlib(list->list,"\"Procurement\"");
		addAPPlib(list->list,"\"HR\"");
		addAPPlib(list->list,"\"Revenue\"");
		addAPPlib(list->list,"\"System Administration\"");
		addAPPlib(list->list,"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][OPTIONAL MODULE GROUP 1]\"))");
		addAPPlib(list->list,"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][OPTIONAL MODULE GROUP 2]\"))");
		addAPPlib(list->list,"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][OPTIONAL MODULE GROUP 3]\"))");
		addAPPlib(list->list,"(GENERIC_SETUP_VARIABLE(\"[UTILITIES][OPTIONAL MODULE GROUP 4]\"))");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(13+9+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [UTILITIES] RDAScrolledList [MODULE GROUPS], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
void MTNMODULE()
{
	MTN_MTN_MODULE();
	MNU_UTILITIES_MTN_MODULE();
	LST_MODULE_GROUPS();
	DEF_MODULE_DEFINE_LIST();
	SCN_MODULE_BROWSE();
	SCN_MODULE_DEFINE_LIST();
	SCN_MODULE_MAINTAIN_SCREEN();
	SCN_MODULE_SEARCH_BROWSE();
	SCN_MODULE_SEARCH_BROWSE_EXPRESSION();
}
