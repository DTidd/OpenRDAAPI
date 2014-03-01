/* purapv.c - Add Purchase Order Vouchers */
#ifndef WIN32
#define __NAM__ "purapv.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purapv.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <fin.hpp>
#include <venpmt.hpp>
#include <purord.hpp>
/*CPP_OPART puravci */
/*CPP_OPART puravcim */
/*CPP_OPART purapv1 */
/*CPP_OPART purapv2 */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char SKIP_OVER_BUDGET_WARNINGS=FALSE;
extern char skip_invoice_date,set_f;
extern char *duedate;
short msrnum=(-1),sitenum=(-1),vennum=(-1),venadrnum=(-1),infnum=(-1);
short linenum=(-1),accnum=(-1),trannum=(-1),invnum=(-1),vchnum=(-1);
short finyrnum=(-1),eyrnum=(-1),ryrnum=(-1),byrnum=(-1);
short yearnum=(-1),fyearnum=(-1),percis=(-1),perctrn=(-1);
short bnkacc=(-1),expnum=(-1),revnum=(-1),balnum=(-1),alias_num=(-1);
RDAfinmgt *finsetup=NULL;
RDApurord *purord=NULL;
RDAvenpmt *venpmt=NULL;
char *reportname=NULL;
int PO_COUNT=0;
short POV_DESCRIPTION=0,prtapo_type=0;
char *poid=NULL,*vendorid=NULL,*invid=NULL,*checkdate=NULL,manual_voucher=FALSE,prepaid=FALSE,pay_1099=FALSE,check_detail=FALSE, addlist=TRUE;
char use_grant_management=FALSE;
int yearid=0,checkno=0,venyrid=0,purordfyear=0,venpmtfyear=0;
short pmonth=0,fmonth=0,venpmtfmonth=0,purordfmonth=0;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static short selectfunc(MakeBrowseList *);
extern char distributable;
extern void purinvm(short,void *,int,char **);
char *module="PURORD";
MakeBrowseList *mbl_addpo=NULL;
char approvepo=FALSE;
short approval_type=0;

static short selectfunc(MakeBrowseList *b)
{
	char flag=FALSE;
	short a=0,atype=0;

	FINDFLDGETCHAR(msrnum,"CLOSED",&flag);
	if(flag) return(FALSE);
	FINDFLDGETCHAR(msrnum,"DELETEFLAG",&flag);
	if(flag) return(FALSE);
	if(approvepo==TRUE)
	{
		FINDFLDGETSHORT(msrnum,"APPROVE",&a);
		if(approval_type==1)
		{
			FINDFLDGETSHORT(msrnum,"APPROVE TYPE",&atype);
			if(atype!=0 && a!=1) return(FALSE);
		} else if(a!=1) return(FALSE);
	}
	return(TRUE);
}
static void doexit(MakeBrowseList *b)
{
	APPlib *args=NULL;

	if(PO_COUNT>0)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(b->definelist!=NULL) 
	{
		killwindow(b->definelist);
		free_rsrc(b->definelist);
	}
	if(b->searchrsrc!=NULL) 
	{
		killwindow(b->searchrsrc);
		free_rsrc(b->searchrsrc);
	}
	if(b->mainrsrc!=NULL)
	{
		killwindow(b->mainrsrc);
		free_rsrc(b->mainrsrc);
	}
	if(reportname!=NULL) Rfree(reportname);
	if(yearnum!=(-1)) CLSNRD(yearnum);
	if(venpmt->use_fiscal_summary==TRUE)
	{
		if(fyearnum!=(-1)) CLSNRD(fyearnum);
	}
	if(accnum!=(-1)) CLSNRD(accnum);
	if(alias_num!=(-1)) CLSNRD(alias_num);
	if(bnkacc!=(-1)) CLSNRD(bnkacc);
	if(sitenum!=(-1)) CLSNRD(sitenum);
	if(trannum!=(-1)) CLSNRD(trannum);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(linenum!=(-1)) CLSNRD(linenum);
	if(venadrnum!=(-1)) CLSNRD(venadrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(expnum!=(-1)) CLSNRD(expnum);
	if(invnum!=(-1)) CLSNRD(invnum);
	if(infnum!=(-1)) CLSNRD(infnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(revnum!=(-1)) CLSNRD(revnum);
	if(balnum!=(-1)) CLSNRD(balnum);
	if(eyrnum!=(-1)) CLSNRD(eyrnum);
	if(finyrnum!=(-1)) CLSNRD(finyrnum);
	if(percis!=(-1)) CLSNRD(percis);
	if(perctrn!=(-1)) CLSNRD(perctrn);
	if(vchnum!=(-1)) CLSNRD(vchnum);
	if(venpmt!=NULL) free_venpmt(venpmt);
	if(finsetup!=NULL) free_finmgt(finsetup);
	if(purord!=NULL) free_purord(purord);
	ShutdownSubsystems();
	exit(0);
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	purinvm(1,targetkey,0,NULL);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	int currentcalyear=0;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"ADD PURCHASE ORDER VOUCHERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	SKIP_DOMENU_TRANSVERSAL=FALSE;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PURORD")+11);
#ifndef WIN32
        sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PURORD");
#endif
#ifdef WIN32
        sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PURORD");
#endif
        gsv=RDAGenericSetupNew("PURORD","PURAPV ADDLISTS");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        addlist=*gsv->value.string_value;
                } else {
                        addlist=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("PURORD","SKIP INVOICE DATE");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        skip_invoice_date=*gsv->value.string_value;
                } else {
                        skip_invoice_date=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PURORD","PRTAPO TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			prtapo_type=*gsv->value.short_value;
		} else {
			prtapo_type=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("PURORD","PO VOUCHER DESCRIPTION");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        POV_DESCRIPTION=*gsv->value.short_value;
                } else {
                        POV_DESCRIPTION=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("PURORD","CURRENT PO FISCAL MONTH");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        purordfmonth=*gsv->value.short_value;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("PURORD","CURRENT PO FISCAL YEAR");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        purordfyear=*gsv->value.integer_value;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PURORD","APPROVE PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			approvepo=*gsv->value.string_value;
		} else {
			approvepo=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PURORD","APPROVAL TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			approval_type=*gsv->value.short_value;
		} else {
			approval_type=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("VENPMT")+11);
#ifndef WIN32
        sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif
#ifdef WIN32
        sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER CALENDAR YEAR");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        currentcalyear=*gsv->value.integer_value;
                } else {
                        currentcalyear=GETCURRENTCALENDARYEAR();
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER CALENDAR MONTH");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        pmonth=*gsv->value.short_value;
                } else {
			CURRENT_DATE10=GETCURRENTDATE10();
			memset(stemp,0,101);
			memcpy(stemp,CURRENT_DATE10,10);
			stemp[2]=0;
			pmonth=(atoi(stemp)-1);
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER FISCAL YEAR");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        venpmtfyear=*gsv->value.integer_value;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER FISCAL MONTH");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        venpmtfmonth=*gsv->value.short_value;
			set_f=TRUE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	fmonth=venpmtfmonth;
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER DUE DATE");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
			duedate=stralloc(gsv->value.string_value);
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
        sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
        sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
        gsv=RDAGenericSetupNew("FINMGT","SKIP OVER BUDGET WARNINGS");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        SKIP_OVER_BUDGET_WARNINGS=*gsv->value.string_value;
                } else {
                        SKIP_OVER_BUDGET_WARNINGS=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("FINMGT","SWB_FM11_GRANT_MANAGEMENT");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        use_grant_management=*gsv->value.string_value;
                } else {
                        use_grant_management=FALSE;
                }
		if(XPERT_SETUP->software_type==0) use_grant_management=TRUE;
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("PVCAUDIT");
	venpmt=RDAvenpmtNEW();
	if(getvenpmtbin(venpmt)==(-1)) return;
	if((msrnum=APPOPNNRD(module,"PURMST",TRUE,TRUE))==(-1)) return;
	if((sitenum=APPOPNNRD(module,"PURSIT",TRUE,FALSE))==(-1)) return;
	if((linenum=APPOPNNRD(module,"PURLIN",TRUE,FALSE))==(-1)) return;
	if((accnum=APPOPNNRD(module,"PURACC",TRUE,TRUE))==(-1)) return;
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if((trannum=APPOPNNRD(module,"PURTRN",TRUE,TRUE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((yearnum=APPOPNNRD("VENPMT","VENYR",TRUE,FALSE))==(-1)) return;
	if(XPERT_SETUP->CIS==TRUE)
	{
		if((percis=APPOPNNRD("PRSNNL","PERCIS",TRUE,TRUE))==(-1)) return;
		if((perctrn=APPOPNNRD("PRSNNL","PERCTRN",TRUE,TRUE))==(-1)) return;
	} else {
		percis=(-1);
		perctrn=(-1);
	}
	if(venpmt->use_fiscal_summary==TRUE)
	{
		if((fyearnum=APPOPNNRD("VENPMT","VENFYR",TRUE,FALSE))==(-1)) return;
	}
	if((venadrnum=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((invnum=APPOPNNRD("VENPMT","VENINV",TRUE,TRUE))==(-1)) return;
	if((infnum=APPOPNNRD("VENPMT","VENINF",TRUE,TRUE))==(-1)) return;
	if((vchnum=APPOPNNRD("VENPMT","VENVCH",TRUE,TRUE))==(-1)) return;
	if((expnum=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((alias_num=APPOPNNRD("FINMGT","FINALIAS",TRUE,FALSE))==(-1)) return;
	if((revnum=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
	if((balnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((finyrnum=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if((eyrnum=APPOPNNRDEDITABLE("FINMGT","FINEYR",&distributable))==(-1)) 
		return;
	if((ryrnum=APPOPNNRDEDITABLE("FINMGT","FINRYR",&distributable))==(-1)) 
		return;
	if((byrnum=APPOPNNRDEDITABLE("FINMGT","FINBYR",&distributable))==(-1)) 
		return;
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
	purord=RDApurordNEW();
	getpurordbin(purord);
	mainrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHERS BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHERS SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHERS DEFINE LIST");
	addDFincvir(searchrsrc,module,"PURMST",NULL,msrnum); 
	addDFincvir(definelist,module,"PURMST",NULL,msrnum); 
	addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum); 
	addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum); 
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	CURRENT_DATE10=GETCURRENTDATE10();
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&currentcalyear,TRUE);
	addstdrsrc(mainrsrc,"PROCESSING MONTH",SHORTV,2,&pmonth,TRUE);
	DefaultScreens(mainrsrc);
	FINDRSCSETINT(mainrsrc,"CALENDAR YEAR",currentcalyear);
	FINDRSCSETSHORT(mainrsrc,"PROCESSING MONTH",pmonth);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	mbl_addpo=ADVSbrowse(msrnum,1,mainrsrc,searchrsrc,definelist,NULL,
		NULL,selectfunc,doexit,okfunc,NULL,NULL,TRUE,NULL,0,0,1,0);
	if(mbl_addpo!=NULL)
	{
		PO_COUNT=0;
	}
	RDAAPPMAINLOOP();
}
