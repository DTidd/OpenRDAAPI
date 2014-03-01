/* purapv.c - Add Purchase Order Vouchers */
#ifndef WIN32
#define __NAM__ "purapvgusd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purapvgusd.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <fin.hpp>
#include <venpmt.hpp>
#include <purord.hpp>
/*CPP_OPART puravcigusd */
/*CPP_OPART puravcimgusd */
/*CPP_OPART purapv1gusd */
/*CPP_OPART purapv2gusd */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern char skip_invoice_date;
short msrnum=(-1),sitenum=(-1),vennum=(-1),venadrnum=(-1);
short linenum=(-1),accnum=(-1),trannum=(-1),invnum=(-1),vchnum=(-1);
short finyrnum=(-1),eyrnum=(-1),ryrnum=(-1),byrnum=(-1);
short yearnum=(-1),fyearnum=(-1);
short bnkacc=(-1),expnum=(-1),revnum=(-1),balnum=(-1);
RDAfinmgt *finsetup=NULL;
RDApurord *purord=NULL;
RDAvenpmt *venpmt=NULL;
char *reportname=NULL;
int PO_COUNT=0;
char *poid=NULL,*vendorid=NULL,*invid=NULL,*checkdate=NULL,manual_voucher=FALSE,prepaid=FALSE,pay_1099=FALSE,check_detail=FALSE, addlist=TRUE;
int yearid=0,checkno=0,venyrid=0;
short pmonth=0,fmonth=0;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static short selectfunc(MakeBrowseList *);
extern char distributable;
extern void purinvm(short,void *,int,char **);
char *module="PURORD";
MakeBrowseList *mbl_addpo=NULL;

static short selectfunc(MakeBrowseList *b)
{
	char flag=FALSE;

	FINDFLDGETCHAR(msrnum,"CLOSED",&flag);
	if(flag) return(FALSE);
	FINDFLDGETCHAR(msrnum,"DELETEFLAG",&flag);
	if(flag) return(FALSE);
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
	if(bnkacc!=(-1)) CLSNRD(bnkacc);
	if(sitenum!=(-1)) CLSNRD(sitenum);
	if(trannum!=(-1)) CLSNRD(trannum);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(linenum!=(-1)) CLSNRD(linenum);
	if(venadrnum!=(-1)) CLSNRD(venadrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(expnum!=(-1)) CLSNRD(expnum);
	if(invnum!=(-1)) CLSNRD(invnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(revnum!=(-1)) CLSNRD(revnum);
	if(balnum!=(-1)) CLSNRD(balnum);
	if(eyrnum!=(-1)) CLSNRD(eyrnum);
	if(finyrnum!=(-1)) CLSNRD(finyrnum);
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
static int getcurrentcalyear(void)
{
	int currentcalyear=0;

	currentcalyear=GETCURRENTCALENDARYEAR();
	return(currentcalyear);
}
static void setpmonths(RDArsrc *mainrsrc)
{
	int x=0;
	short y=0;

	readwidget(mainrsrc,"PURAPV DEFAULT PROCESSING MONTHS");
	FINDRSCGETINT(mainrsrc,"PURAPV DEFAULT PROCESSING MONTHS",&x);
	y=(short)x;
	FINDRSCSETSHORT(mainrsrc,"PROCESSING MONTH",y);
	FINDRSCSETINT(mainrsrc,"PURAPV DEFAULT FISCAL PROCESSING MONTHS",(x+1));
	y=(short)x;
	FINDRSCSETSHORT(mainrsrc,"FISCAL PROCESSING MONTH",(y+1));
	updatersrc(mainrsrc,"PURAPV DEFAULT FISCAL PROCESSING MONTHS");
	updatersrc(mainrsrc,"PROCESSING MONTH");
	updatersrc(mainrsrc,"FISCAL PROCESSING MONTH");
}
static void setpmonth(RDArsrc *mainrsrc)
{
	short x=0;
	int y=0;

	readwidget(mainrsrc,"PROCESSING MONTH");
	FINDRSCGETSHORT(mainrsrc,"PROCESSING MONTH",&x);
	y=(int)x;
	FINDRSCGETINT(mainrsrc,"PURAPV DEFAULT PROCESSING MONTHS",&y);
	FINDRSCSETSHORT(mainrsrc,"FISCAL PROCESSING MONTH",(x+1));
	y=(int)x;
	FINDRSCSETINT(mainrsrc,"PURAPV DEFAULT FISCAL PROCESSING MONTHS",(y+1));
	updatersrc(mainrsrc,"PROCESSING MONTH");
	updatersrc(mainrsrc,"FISCAL PROCESSING MONTH");
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	int currentcalyear=0;
	int x=0;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	if(InitializeSubsystems(argc,argv,module,"ADD PURCHASE ORDER VOUCHERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
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
	if(venpmt->use_fiscal_summary==TRUE)
	{
		if((fyearnum=APPOPNNRD("VENPMT","VENFYR",TRUE,FALSE))==(-1)) return;
	}
	if((venadrnum=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((invnum=APPOPNNRD("VENPMT","VENINV",TRUE,TRUE))==(-1)) return;
	if((vchnum=APPOPNNRD("VENPMT","VENVCH",TRUE,TRUE))==(-1)) return;
	if((expnum=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
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
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	currentcalyear=getcurrentcalyear();
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&currentcalyear,TRUE);
	addstdrsrc(mainrsrc,"PROCESSING MONTH",SHORTV,2,&pmonth,TRUE);
	addstdrsrc(mainrsrc,"FISCAL PROCESSING MONTH",SHORTV,2,&fmonth,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	mbl_addpo=ADVSbrowse(msrnum,1,mainrsrc,searchrsrc,definelist,NULL,
		NULL,selectfunc,doexit,okfunc,NULL,NULL,TRUE,NULL,0,0,1,0);
	if(mbl_addpo!=NULL)
	{
		PO_COUNT=0;
	}
	x=FINDRSC(mainrsrc,"PROCESSING MONTH");
	if(x!=(-1))
	{
		FINDRSCSETFUNC(mainrsrc,"PROCESSING MONTH",setpmonth,NULL);
	}
	x=FINDRSC(mainrsrc,"PURAPV DEFAULT PROCESSING MONTHS");
	if(x!=(-1))
	{
		FINDRSCSETFUNC(mainrsrc,"PURAPV DEFAULT PROCESSING MONTHS",setpmonths,NULL);
	}
	setpmonth(mainrsrc);
	setpmonths(mainrsrc);
	RDAAPPMAINLOOP();
}
