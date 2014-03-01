/* finarec.c - Financial Management Add Receipts */
#define CLEARWDGT
#ifndef WIN32
#define __NAM__ "dquery.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dquery.exe"
#endif
#include <app.hpp>
#include <cstdlib>



#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char filename[101],*SOURCE_MODULE=NULL,*SOURCE_PROCESS=NULL;
static char *SOURCE_USER=NULL,*DISTRIBUTED_DATE=NULL,*DISTRIBUTED_TIME=NULL;
static char SWB_FM25_AFFECT_DATE=FALSE,PayrollActivity=FALSE;
static char *REFERENCE_ID=NULL;
static short Style=0,Summed=0,DID_STUFF=FALSE,payvdst_option=0;
static double TOTAL_DEBITS=0.0,TOTAL_CREDITS=0.0;
static char *AccountCode=NULL,*AFFECTDate=NULL,*FromAFFECTDate=NULL;
static short filenum=(-1),yearnum=(-1),expnum=(-1),revnum=(-1),balnum=(-1),venmst=(-1);
static short byrnum=(-1),eyrnum=(-1),ryrnum=(-1),venfyr=(-1),puracc=(-1),fixtran=(-1);
static short finjen=(-1),finrec=(-1),venvch=(-1),purtrn=(-1),purmst=(-1),fineact=(-1);
static short payjtrn=(-1),paydtrn=(-1),payddsv=(-1),paymst=(-1),perdmg=(-1),finbact=(-1);
static short invadj=(-1),invisst=(-1),bnkchg=(-1),bnkrtn=(-1),utbsvc=(-1),invacc=(-1);
static short prptrn=(-1),prppay=(-1),restran=(-1),restpay=(-1),utbtrn=(-1),utbcds=(-1);
static short misctran=(-1),miscpaid=(-1),occrec=(-1),prmttran=(-1),prmtpaid=(-1);
static short purlin=(-1),payddms=(-1),payjpms=(-1),prmtmstr=(-1),restmst=(-1),prpmst=(-1),miscmstr=(-1);
static short AFFECT_DATE_STYLE=0;
static RDAfinmgt *finsetup=NULL;
static APPlib *ACCTtypes=NULL,*DEFtypes=NULL;
static APPlib *yearavl=NULL,*acctavl=NULL;
static APPlib *pmonth=NULL;
static char *module="FINMGT",ADCFP=FALSE,SELECT_DISTRIBUTED=TRUE,SELECT_UNDISTRIBUTED=FALSE;
static int FiscalYear=(-1),NumberRows=100,CurrentRow=0,dm=(-1);
static int NumberColumns=17,CurrentPage=1,TotalPages=1;
static int AccountType=(-1),DefaultAccountType=(-1),DefinitionType=(-1),NumRecs=0;
static int GrantYear=(-1),SECONDS_DIFFERENTIAL=90;
static int DefaultDefinitionType=(-1);
static void changeACCTtype(RDArsrc *),changeDEFtype(RDArsrc *);
static void changeCODElist(RDArsrc *);
static void ACCTtypecb(RDArsrc *),DEFtypecb(RDArsrc *);
static void getacctdesc(RDArsrc *);
static void changeyear(RDArsrc *),setyear(RDArsrc *);
static int getyears(RDArsrc *);
static void set_defaults(RDArsrc *);
static void quit_record(RDArsrc *);
static void acctsearch(RDArsrc *);
static void setfiles(RDArsrc *);
static void getsupportingrecords(RDArsrc *,short);
static double Dtime=0.0;
static RDArsrc *MAINRSRC=NULL;
static DBsort *dbsrt=NULL;
static RDAvirtual *csortsequence=NULL;
void ClearTableResources(void);
static RDAvirtual *EmployeeName=NULL;
static char *SourceList[]={"Journal Entries","Receipts (A/R)","Vendor Vouchers (A/P)","PO Transactions","Bank Charges","Bank Return Checks","V/W Vouchers","Job Transactions","Deduction Transactions","Permit Transactions","Permit Payments","Real Estate Transactions","Real Estate Payments","Property Transactions","Property Payments","Misc Billing Transactions","Misc Billing Payments","Utility Billing Deposits","Utility Billing Receipts","Utility Billing Charges","OccTax Receipts","Fixed Assets","Inventory Issues","Inventory Adjustments","Expenditure Activity","Balance Sheet Activity"};

/* Items to Keep Track of */
/* 
	Identification		plaintext	30
	Calendar Year		int 		4
	Transaction Number	int		8
	Account Number		int 		4
	Reference Id		plaintext 	40
	Control			plaintext	40 
	Debit Amount		dollars		20	
	Credit Amount		dollars		20
	Description		plaintext	40
	Affect Date		Date		10
	Source User		plaintext	15
	Entry Date		Date		10
	Distributed 		Boolean		1
	source			Short		2
	Bank Identification	plaintext	15
	Check/EFT		int 		9
*/
static void getsecdiff(RDArsrc *r)
{
	readwidget(r,"SECONDS DIFFERENTIAL");
	FINDRSCGETINT(r,"SECONDS DIFFERENTIAL",&SECONDS_DIFFERENTIAL);
	DID_STUFF=FALSE;
	getacctdesc(r);
}
char TimeComparison(char *tx)
{
	short hrs=0,min=0,sec=0;
	double t=0.0,r=0.0;

	if(isEMPTY(tx)) return(FALSE);
	if(RDAstrlen(tx)<8) return(FALSE);
	if(isEMPTY(DISTRIBUTED_TIME)) return(FALSE);
	hrs=val(&tx[0],2);
	min=val(&tx[3],2);
	sec=val(&tx[6],2);
	t=(hrs*3600)+(min*60)+sec;
	if(Dtime==0.0)
	{
		hrs=val(&DISTRIBUTED_TIME[0],2);
		min=val(&DISTRIBUTED_TIME[3],2);
		sec=val(&DISTRIBUTED_TIME[6],2);
		Dtime=(hrs*3600)+(min*60)+sec;
	}
	if(diagapps)
	{
		prterr("DIAG TimeComparison Dtime [%s] [%8.0f] t [%s] [%8.0f] ",DISTRIBUTED_TIME,Dtime,tx,t);
	}
	r=Dtime-t;
	if(r<=0 && ((r*(-1))<SECONDS_DIFFERENTIAL)) return(TRUE);
	if(r>=0 && (r<SECONDS_DIFFERENTIAL)) return(TRUE);
/*
	if(((Dtime-t)<=SECONDS_DIFFERENTIAL) && (Dtime>=t)) return(TRUE);
	else if(((t-Dtime)<=SECONDS_DIFFERENTIAL) && (t>=Dtime)) return(TRUE);
*/
	return(FALSE);
}
void dQueryCustomTableFunction(Wt::WWidget *parent,Wt::WWidget **pop)
{
	Wt::WTable *myTable=NULL;
	Wt::WContainerWidget *VB=NULL;
	Wt::WScrollArea *SA=NULL;
	Wt::WContainerWidget *VB1=NULL;
	Wt::WVBoxLayout *vb1=NULL;
	Wt::WHBoxLayout *hb=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WTableColumn *TC=NULL;
	Wt::WTableCell *TE=NULL;
	Wt::WTableRow *HH=NULL;
	Wt::WLength spc,wl,sw;
	Wt::WString *tmpstr=NULL;
	Wt::WText *myText=NULL;
	char DQUERYstemp[101];
	int crow=0,r=0,section=0;
	RDArmem *mem=NULL;
	char *mssc=NULL;

	if(diaggui || diaggui_field)
	{
		prterr("DIAG dQueryCustomTableFunction.");
	}
	TRACE;
	VB=new Wt::WContainerWidget();
	*pop=(Wt::WWidget *)VB;
	hb=new Wt::WHBoxLayout();
	hb->setSpacing(0);
	VB->setLayout(hb);
	daL=(Wt::WLayout *)hb;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	VB->setPadding(spc,All);
	mssc=ModuleScreenStyleClass(MAINRSRC);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s TableContainer",mssc);
	if(mssc!=NULL) Rfree(mssc);
	VB->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
/*
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n","",VB->styleClass.toUTF8().c_str());
		}
	}
*/
#endif /* USE_RDA_DIAGNOSTICS */
	SA=new Wt::WScrollArea((Wt::WContainerWidget *)VB);
	hb->addWidget(SA);
	VB1=new Wt::WContainerWidget();
	SA->setWidget((Wt::WWidget *)VB1);	
	hb->setStretchFactor(SA,800);
	vb1=new Wt::WVBoxLayout();
	VB1->setLayout(vb1);
	SA->setHorizontalScrollBarPolicy(Wt::WScrollArea::ScrollBarAsNeeded);
	SA->setVerticalScrollBarPolicy(Wt::WScrollArea::ScrollBarAlwaysOn);
	sw=Wt::WLength(400,Wt::WLength::Pixel);
	SA->setMinimumSize(sw,Wt::WLength::Auto);
	MAINRSRC->has_large_table=TRUE;
	myTable=new Wt::WTable();
	myTable->addStyleClass("polished");
	mssc=ModuleScreenStyleClass(MAINRSRC);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s Table",mssc);
	if(mssc!=NULL) Rfree(mssc);
	myTable->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
/*
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myTable->styleClass.toUTF8().c_str());
		}
	}
*/
#endif /* USE_RDA_DIAGNOSTICS */
	vb1->addWidget(myTable);
	vb1->setStretchFactor(myTable,750);
	vb1->setSpacing(0);
	daL=(Wt::WLayout *)vb1;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	VB1->setPadding(spc,All);
/* Determine Rows & Columns */
	myTable->setHeaderCount(1,Horizontal);
	section=0;
	if(SWB_FM25_AFFECT_DATE==TRUE)
	{
		myText=new Wt::WText("Affect Date");
	} else {
		myText=new Wt::WText("Date");
	}
	TE=myTable->elementAt(0,section);
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Control");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Debit Amount");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Credit Amount");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Reference");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Description");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Identification");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Bank ID");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Grant Year");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Entry Date");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Calendar Year");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Source User");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Trans#");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Acc#");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Source Description");
	TE->addWidget(myText);
	++section;
	TE=myTable->elementAt(0,section);
	myText=new Wt::WText("Source");
	TE->addWidget(myText);
	++section;
	TRACE;
	for(crow=1;crow<(NumberRows+1);++crow)
	{
		section=0;
		TRACE;
		TE=myTable->elementAt(crow,section);
		TRACE;
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:AFFECT DATE",crow);
		TRACE;
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			TRACE;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,10,0,(Wt::WTable *)*pop,crow-1,section,0,0);
			TRACE;
		}
		TRACE;
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:CONTROL",crow);
		TRACE;
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			TRACE;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,15,0,(Wt::WTable *)*pop,crow-1,section,0,0);
			TRACE;
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:DEBIT AMOUNT",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,20,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:CREDIT AMOUNT",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,20,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:REFERENCE",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,15,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:DESCRIPTION",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,40,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:IDENTIFICATION",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,15,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section; 
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:BANK IDENTIFICATION",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,15,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:GRANT YEAR",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,10,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:ENTRY DATE",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,10,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:DISTRIBUTED",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,"Distributed",NULL,0,0,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:CALENDAR YEAR",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,13,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:SOURCE USER",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,15,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:TRANSACTION NUMBER",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,8,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:ACCOUNT NUMBER",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,4,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:SOURCE DESCRIPTION",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,25,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
		TE=myTable->elementAt(crow,section);
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%08d:SOURCE",crow);
		r=FINDRSC(MAINRSRC,DQUERYstemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((Wt::WWidget *)*pop,mem,DQUERYstemp,NULL,0,2,0,(Wt::WTable *)*pop,crow-1,section,0,0);
		}
		++section;
	}
}
void DisplayData()
{
	short ef=0,src=0;
	int count=0,cy=0,trans=0,acctnum=0,display_count=0,gy=0;
	char *adate=NULL,*ref=NULL,*c=NULL,*desc=NULL,*su=NULL,*id=NULL;
	char *edate=NULL,d=FALSE,delflag=FALSE,*bankid=NULL;
	double deb=0.0,cred=0.0;
	char DQUERYstemp[101];

	TotalPages=RDAroundup((NumRecs/NumberRows));
	if(TotalPages==0) TotalPages=1;
	if((TotalPages*NumberRows)<NumRecs) ++TotalPages;
	ef=FRSNRD(dbsrt->fileno,1);
	while(!ef)
	{
		FINDFLDGETCHAR(dbsrt->fileno,"DELETEFLAG",&delflag);
		if(!delflag)
		{
		++count;
		if(count>(CurrentPage*NumberRows)) break;
		else if(count<((CurrentPage-1)*NumberRows))
		{
		} else {
			++display_count;
			FINDFLDGETSTRING(dbsrt->fileno,"AFFECT DATE",&adate);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:AFFECT DATE",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,adate);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETDOUBLE(dbsrt->fileno,"DEBIT AMOUNT",&deb);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:DEBIT AMOUNT",display_count);
			FINDRSCSETDOUBLE(MAINRSRC,DQUERYstemp,deb);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETDOUBLE(dbsrt->fileno,"CREDIT AMOUNT",&cred);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:CREDIT AMOUNT",display_count);
			FINDRSCSETDOUBLE(MAINRSRC,DQUERYstemp,cred);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETSTRING(dbsrt->fileno,"REFERENCE",&ref);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:REFERENCE",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,ref);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETSTRING(dbsrt->fileno,"DESCRIPTION",&desc);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:DESCRIPTION",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,desc);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETSTRING(dbsrt->fileno,"IDENTIFICATION",&id);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:IDENTIFICATION",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,id);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETSTRING(dbsrt->fileno,"BANK IDENTIFICATION",&bankid);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:BANK IDENTIFICATION",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,bankid);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETSTRING(dbsrt->fileno,"CONTROL",&c);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:CONTROL",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,c);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			memset(DQUERYstemp,0,101);
			FINDFLDGETINT(dbsrt->fileno,"GRANT YEAR",&gy);
			sprintf(DQUERYstemp,"%08d:GRANT YEAR",display_count);
			FINDRSCSETINT(MAINRSRC,DQUERYstemp,gy);
			if(AccountType!=2)
			{
				FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			}
			FINDFLDGETSTRING(dbsrt->fileno,"ENTRY DATE",&edate);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:ENTRY DATE",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,edate);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETCHAR(dbsrt->fileno,"DISTRIBUTED",&d);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:DISTRIBUTED",display_count);
			FINDRSCSETCHAR(MAINRSRC,DQUERYstemp,d);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			memset(DQUERYstemp,0,101);
			FINDFLDGETINT(dbsrt->fileno,"CALENDAR YEAR",&cy);
			sprintf(DQUERYstemp,"%08d:CALENDAR YEAR",display_count);
			FINDRSCSETINT(MAINRSRC,DQUERYstemp,cy);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETSTRING(dbsrt->fileno,"SOURCE USER",&su);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:SOURCE USER",display_count);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,su);
			FINDFLDGETINT(dbsrt->fileno,"TRANSACTION NUMBER",&trans);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:TRANSACTION NUMBER",display_count);
			FINDRSCSETINT(MAINRSRC,DQUERYstemp,trans);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETINT(dbsrt->fileno,"ACCOUNT NUMBER",&acctnum);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:ACCOUNT NUMBER",display_count);
			FINDRSCSETINT(MAINRSRC,DQUERYstemp,acctnum);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			FINDFLDGETSHORT(dbsrt->fileno,"SOURCE",&src);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:SOURCE DESCRIPTION",display_count);
			FINDRSCSETSTRING(MAINRSRC,DQUERYstemp,SourceList[src]);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:SOURCE",display_count);
			FINDRSCSETSHORT(MAINRSRC,DQUERYstemp,src);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,TRUE);
		}
		}
		ef=NXTNRD(dbsrt->fileno,1);
	}
	if(adate!=NULL) Rfree(adate);
	if(edate!=NULL) Rfree(edate);
	if(bankid!=NULL) Rfree(bankid);
	if(desc!=NULL) Rfree(desc);
	if(ref!=NULL) Rfree(ref);
	if(c!=NULL) Rfree(c);
	if(id!=NULL) Rfree(id);
	if(ref!=NULL) Rfree(ref);
	if(su!=NULL) Rfree(su);
}
void PreviousPage(RDArsrc *r)
{
	readwidget(r,"CURRENT PAGE");
	FINDRSCGETINT(r,"CURRENT PAGE",&CurrentPage);
	--CurrentPage;
	if(CurrentPage<1) CurrentPage=1;
	ClearTableResources();
	DisplayData();
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updateallrsrc(MAINRSRC);
}
void NextPage(RDArsrc *r)
{
	readwidget(r,"CURRENT PAGE");
	FINDRSCGETINT(r,"CURRENT PAGE",&CurrentPage);
	++CurrentPage;
	if(CurrentPage>TotalPages) CurrentPage=TotalPages;
	ClearTableResources();
	DisplayData();
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updateallrsrc(MAINRSRC);
}
void changeCurrentPage(RDArsrc *r)
{
	readwidget(r,"CURRENT PAGE");
	FINDRSCGETINT(r,"CURRENT PAGE",&CurrentPage);
	ClearTableResources();
	DisplayData();
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updateallrsrc(MAINRSRC);
}

void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	short keyno=(-1);
	char *temp=NULL;
	char DQUERYstemp[101];

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("FINMGT","DQUERY SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=stralloc(d->expression);
		} else csortsequence->expression=NULL;
		csortsequence->computed=FALSE;
		csortsequence->range=FALSE;
		csortsequence->dlen=0;
		if(!isEMPTY(csortsequence->expression))
		{
			temp=PP_translate(csortsequence->expression);
			Rfree(csortsequence->expression);	
			if(temp!=NULL)
			{
				csortsequence->expression=stralloc(temp);
				Rfree(temp);
			}
		}		
		switch(csortsequence->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				csortsequence->value.string_value=NULL;
				csortsequence->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				csortsequence->value.string_value=Rmalloc(1);
				*csortsequence->value.string_value=0;
				csortsequence->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				csortsequence->value.float_value=Rmalloc(sizeof(double));
				*csortsequence->value.float_value=0;
				csortsequence->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				csortsequence->value.short_value=Rmalloc(sizeof(short));
				*csortsequence->value.short_value=0;
				csortsequence->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				csortsequence->value.integer_value=Rmalloc(sizeof(int));
				*csortsequence->value.integer_value=0;
				csortsequence->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
		addDBfield(dbsrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsrt,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	addDBfield(dbsrt,"DATE AFFECTED",PLAINTEXT,10);
	addDBkeypart(dbsrt,keyno,"DATE AFFECTED");
	addDBfield(dbsrt,"CONTROL",PLAINTEXT,40);
	addDBkeypart(dbsrt,keyno,"CONTROL");
	if(Summed==0)
	{
		addDBfield(dbsrt,"NUMBER",LONGV,8);
		addDBkeypart(dbsrt,keyno,"NUMBER");
	}
	addDBfield(dbsrt,"IDENTIFICATION",PLAINTEXT,15);
	addDBfield(dbsrt,"TRANSACTION NUMBER",LONGV,8);
	addDBfield(dbsrt,"ACCOUNT NUMBER",LONGV,4);
	addDBfield(dbsrt,"CALENDAR YEAR",LONGV,4);
	addDBfield(dbsrt,"GRANT YEAR",LONGV,4);
	addDBfield(dbsrt,"REFERENCE",PLAINTEXT,40);
	addDBfield(dbsrt,"DESCRIPTION",PLAINTEXT,40);
	addDBfield(dbsrt,"SOURCE USER",PLAINTEXT,15);
	addDBfield(dbsrt,"AFFECT DATE",DATES,10);
	addDBfield(dbsrt,"ENTRY DATE",DATES,10);
	addDBfield(dbsrt,"DEBIT AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"CREDIT AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"DISTRIBUTED",BOOLNS,1);
	addDBfield(dbsrt,"SOURCE",SHORTV,2);
	addDBfield(dbsrt,"BANK IDENTIFICATION",PLAINTEXT,15);
}
void addsortrecord(DBsort *dbsrt,short src,char *AffectDate,char *id,
	char *refid,int transnum,int accnum,int calyear,int grantyr,
	char *control,char *desc,char *su,
	char *edate,double debit,double credit,char d,char *bankid)
{
	char deleteflag=FALSE;
	double amt=0.0;
	char DQUERYstemp[101];

	if(debit==0.0 && credit==0.0) return;
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		switch(csortsequence->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				FINDFLDSETSTRING(dbsrt->fileno,csortsequence->name,
					csortsequence->value.string_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				FINDFLDSETCHAR(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				FINDFLDSETDOUBLE(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				FINDFLDSETSHORT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				FINDFLDSETINT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.integer_value);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
	}
	memset(DQUERYstemp,0,101);
	sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&AffectDate[6],&AffectDate[0],&AffectDate[3]);
	FINDFLDSETSTRING(dbsrt->fileno,"DATE AFFECTED",DQUERYstemp);
	FINDFLDSETSTRING(dbsrt->fileno,"CONTROL",control);
	if(Summed>0)
	{
		if(EQLNRD(dbsrt->fileno,1))
		{
			KEYNRD(dbsrt->fileno,1);
			++NumRecs;
		}
	}
	FINDFLDSETSTRING(dbsrt->fileno,"IDENTIFICATION",id);
	FINDFLDSETSTRING(dbsrt->fileno,"AFFECT DATE",AffectDate);
	FINDFLDSETSHORT(dbsrt->fileno,"SOURCE",src);
	FINDFLDSETSTRING(dbsrt->fileno,"REFERENCE",refid);
	FINDFLDSETSTRING(dbsrt->fileno,"DESCRIPTION",desc);
	FINDFLDSETSTRING(dbsrt->fileno,"SOURCE USER",su);
	FINDFLDSETSTRING(dbsrt->fileno,"ENTRY DATE",edate);
	if(Summed==0)
	{
		FINDFLDSETDOUBLE(dbsrt->fileno,"DEBIT AMOUNT",debit);
		FINDFLDSETDOUBLE(dbsrt->fileno,"CREDIT AMOUNT",credit);
	} else {
		FINDFLDGETDOUBLE(dbsrt->fileno,"DEBIT AMOUNT",&amt);
		amt+=debit;
		FINDFLDSETDOUBLE(dbsrt->fileno,"DEBIT AMOUNT",amt);
		FINDFLDGETDOUBLE(dbsrt->fileno,"CREDIT AMOUNT",&amt);
		amt+=credit;
		FINDFLDSETDOUBLE(dbsrt->fileno,"CREDIT AMOUNT",amt);
	}
	FINDFLDSETINT(dbsrt->fileno,"TRANSACTION NUMBER",transnum);
	FINDFLDSETINT(dbsrt->fileno,"ACCOUNT NUMBER",accnum);
	FINDFLDSETINT(dbsrt->fileno,"CALENDAR YEAR",calyear);
	FINDFLDSETINT(dbsrt->fileno,"GRANT YEAR",grantyr);
	FINDFLDSETCHAR(dbsrt->fileno,"DISTRIBUTED",d);
	FINDFLDSETSTRING(dbsrt->fileno,"BANK IDENTIFICATION",bankid);
	TOTAL_DEBITS+=debit;
	TOTAL_CREDITS+=credit;
	if(Summed==0)
	{
		WRTNRD(dbsrt->fileno,TRUE,"NUMBER");
		++NumRecs;
	} else {
		WRTNRD(dbsrt->fileno,0,NULL);
	}
}
void GatherData()
{
	int fy=0,PM=0,trans=0,accnum=0,cy=0,checkno=0,gy=0;
	char delflag=FALSE,read_record=FALSE;
	char *venid1=NULL,*venid2=NULL,*payid1=NULL,*payid2=NULL,*id=NULL;
	short ef=0,ef2=0,pm=0,acctype=0,deftype=0,type=0,jetype=0;
	char *acctcode=NULL,*debcode=NULL,*credcode=NULL;
	char *duefrom=NULL,*clearcash=NULL,*payable=NULL;
	char *adate=NULL,*ref=NULL,*c=NULL,*desc=NULL,*su=NULL;
	char *edate=NULL,d=FALSE,*date_affected=NULL;
	char *from_date_affected=NULL,*d_date=NULL,*d_time=NULL;
	char use_deferred_revenue=FALSE,realize_revenue=FALSE;
	char use_clearing_fund=FALSE,*bankid=NULL,*source_mod=NULL,*source_proc=NULL;
	char *deferred_code=NULL,*receivable_code=NULL,*clearing_cash=NULL,*clearing_dueto=NULL;
	double deb=0.0,cred=0.0,amt=0.0;
	char *e=NULL,voidchk=FALSE,*refid=NULL;
	RDArsrc *tmprsrc=NULL;
	char DQUERYstemp[101];

	NumRecs=0;
	if(isEMPTY(AccountCode)) return;

	FINDRSCSETSENSITIVE(MAINRSRC,"TOTAL DEBITS",TRUE);
	FINDRSCSETSENSITIVE(MAINRSRC,"TOTAL CREDITS",TRUE);
	FINDRSCSETSENSITIVE(MAINRSRC,"BALANCE",TRUE);
	FINDRSCSETSENSITIVE(MAINRSRC,"CSV",TRUE);
	if(!isEMPTY(AFFECTDate))
	{
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&AFFECTDate[6],&AFFECTDate[0],&AFFECTDate[3]);
		date_affected=stralloc(DQUERYstemp);
	} 
	if(!isEMPTY(FromAFFECTDate))
	{
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&FromAFFECTDate[6],&FromAFFECTDate[0],&FromAFFECTDate[3]);
		from_date_affected=stralloc(DQUERYstemp);
	}
	FINDRSCGETINT(MAINRSRC,"PROCESSING MONTHS",&PM);




	if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"FINMGT") && !RDAstrcmp(SOURCE_PROCESS,"JEDIST")))
	{
		e=Rmalloc(RDAstrlen(MODULENAME(finjen))+RDAstrlen(FILENAME(finjen))+65);
		sprintf(e,"Gathering from Module %s File %s",MODULENAME(finjen),FILENAME(finjen));
		tmprsrc=diagscrn(finjen,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
		if(e!=NULL) Rfree(e);
		e=NULL;
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
				sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
				prterr("Error %s.",e);
				if(e!=NULL) Rfree(e);
			} else {
				e=Rmalloc(RDAstrlen(MODULENAME(finjen))+RDAstrlen(FILENAME(finjen))+15);
				sprintf(e,"%s %s QUERYING",MODULENAME(finjen),FILENAME(finjen));
				SETTITLE(tmprsrc,e);
				if(e!=NULL) Rfree(e);
				ForceWindowUpdate(tmprsrc);
			}
		}
		ZERNRD(finjen);
		FINDFLDSETINT(finjen,"FISCAL YEAR",FiscalYear);
		ef=ADVGTENRDsec(finjen,2,SCRNvirtualSubData,MAINRSRC);
		while(!ef)
		{
			ForceWindowUpdate(tmprsrc);
			FINDFLDGETINT(finjen,"FISCAL YEAR",&fy);
			if(fy!=FiscalYear) break;
			read_record=FALSE;
			FINDFLDGETCHAR(finjen,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSHORT(finjen,"PROCESSING MONTH",&pm);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(finjen,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(finjen,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(finjen,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETSTRING(finjen,"JOURNAL IDENTIFICATION",&refid);
				}
				if((pm==PM || Style==1) && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					FINDFLDGETSHORT(finjen,"ACCOUNT TYPE",&acctype);
					FINDFLDGETSHORT(finjen,"DEFINITION TYPE",&deftype);
					if(SWB_FM25_AFFECT_DATE==TRUE)
					{
						FINDFLDGETSTRING(finjen,"AFFECT DATE",&adate);
					} else {
						FINDFLDGETSTRING(finjen,"DISTRIBUTED DATE",&adate);
					}
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
					if(DefinitionType==deftype && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
					{
						read_record=FALSE;
						FINDFLDGETINT(finjen,"GRANT YEAR",&gy);
						if(AccountType==2 || (gy==GrantYear || GrantYear==(-1)))
						{
							if((AccountType==2) || (acctype==AccountType)) 
							{

								FINDFLDGETSHORT(finjen,"JOURNAL ENTRY TYPE",&jetype);
								if(AccountType<2 && jetype==0) 
								{
									FINDFLDGETSTRING(finjen,"ACCOUNT CODE",&acctcode);
									if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
								}
								if(AccountType==2)
								{
									FINDFLDGETSTRING(finjen,"DEBIT CODE",&debcode);
									FINDFLDGETSTRING(finjen,"CREDIT CODE",&credcode);
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
								}
							}
							FINDFLDGETCHAR(finjen,"DISTRIBUTED",&d);
						}
						if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
						{
							FINDFLDGETSTRING(finjen,"JOURNAL IDENTIFICATION",&id);
							FINDFLDGETSTRING(finjen,"ENTRY DATE",&edate);
							FINDFLDGETSTRING(finjen,"DESCRIPTION",&desc);
							FINDFLDGETSTRING(finjen,"CONTROL",&c);
							FINDFLDGETSTRING(finjen,"REFERENCE NUMBER",&ref);
							FINDFLDGETINT(finjen,"TRANSACTION NUMBER",&trans);
							FINDFLDGETSTRING(finjen,"SOURCE USER",&su);
							FINDFLDGETDOUBLE(finjen,"AMOUNT",&amt);
							cy=atoi(&edate[6]);
							deb=0.0;cred=0.0;
							if(AccountType==0 && amt>=0.0) 
							{
								deb=amt;
								addsortrecord(dbsrt,0,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							} else if(AccountType==0) 
							{
								cred=amt*(-1);
								addsortrecord(dbsrt,0,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(AccountType==1 && amt>=0.0) 
							{
								cred=amt;
								addsortrecord(dbsrt,0,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							} else if(AccountType==1) 
							{
								deb=amt*(-1);
								addsortrecord(dbsrt,0,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
							{
								deb=amt;
								gy=0;
								addsortrecord(dbsrt,0,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
							{
								cred=amt;
								gy=0;
								addsortrecord(dbsrt,0,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(bankid!=NULL) Rfree(bankid);
						}
					}
				}
			}
			if(!abort_diagnostic)
			{
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
			}
			ef=ADVNXTNRDsec(finjen,2,SCRNvirtualSubData,MAINRSRC);
		}
		ZERNRD(finjen);
		if(tmprsrc!=NULL) 
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"FINMGT") && !RDAstrcmp(SOURCE_PROCESS,"RECDIST")))
	{
		e=Rmalloc(RDAstrlen(MODULENAME(finrec))+RDAstrlen(FILENAME(finrec))+65);
		sprintf(e,"Gathering from Module %s File %s",MODULENAME(finrec),FILENAME(finrec));
		tmprsrc=diagscrn(finrec,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
		if(e!=NULL) Rfree(e);
		e=NULL;
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
				sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
				prterr("Error %s.",e);
				if(e!=NULL) Rfree(e);
			} else {
				e=Rmalloc(RDAstrlen(MODULENAME(finrec))+RDAstrlen(FILENAME(finrec))+15);
				sprintf(e,"%s %s QUERYING",MODULENAME(finrec),FILENAME(finrec));
				SETTITLE(tmprsrc,e);
				if(e!=NULL) Rfree(e);
				ForceWindowUpdate(tmprsrc);
			}
		}
		ZERNRD(finrec);
		FINDFLDSETINT(finrec,"FISCAL YEAR",FiscalYear);
		ef=ADVGTENRDsec(finrec,2,SCRNvirtualSubData,MAINRSRC);
		while(!ef)
		{
			ForceWindowUpdate(tmprsrc);
			FINDFLDGETINT(finrec,"FISCAL YEAR",&fy);
			if(fy!=FiscalYear) break;
			FINDFLDGETCHAR(finrec,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSHORT(finrec,"PROCESSING MONTH",&pm);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(finrec,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(finrec,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(finrec,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETSTRING(finrec,"JOURNAL IDENTIFICATION",&refid);
				}
				if((pm==PM || Style==1) && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					if(diagapps)
					{
					fprintf(RDA_STDERR,"REFERENCE_ID [%s] refid [%s] ",(REFERENCE_ID!=NULL ? REFERENCE_ID:""),(refid!=NULL ? refid:""));TRACE;
					}
					FINDFLDGETSHORT(finrec,"ACCOUNT TYPE",&acctype);
					FINDFLDGETSHORT(finrec,"DEFINITION TYPE",&deftype);
					FINDFLDGETSTRING(finrec,"DEPOSIT DATE",&adate);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
					if(DefinitionType==deftype && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
					{
						read_record=FALSE;
						FINDFLDGETINT(finrec,"GRANT YEAR",&gy);
						if(AccountType==2 || (gy==GrantYear || GrantYear==(-1)))
						{
							if((AccountType==2) || (acctype==AccountType)) 
							{
								if(AccountType<2) 
								{
									FINDFLDGETSTRING(finrec,"ACCOUNT CODE",&acctcode);
									if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
								}
								if(AccountType==2)
								{
									FINDFLDGETSTRING(finrec,"DEBIT CODE",&debcode);
									FINDFLDGETSTRING(finrec,"CREDIT CODE",&credcode);
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
								}
							}
							FINDFLDGETCHAR(finrec,"DISTRIBUTED",&d);
						}
						if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
						{
							FINDFLDGETSTRING(finrec,"JOURNAL IDENTIFICATION",&id);
							FINDFLDGETSTRING(finrec,"ENTRY DATE",&edate);
							FINDFLDGETSTRING(finrec,"DESCRIPTION",&desc);
							FINDFLDGETSTRING(finrec,"CONTROL",&c);
							FINDFLDGETSTRING(finrec,"REFERENCE NUMBER",&ref);
							FINDFLDGETINT(finrec,"TRANSACTION NUMBER",&trans);
							FINDFLDGETSTRING(finrec,"SOURCE USER",&su);
							FINDFLDGETDOUBLE(finrec,"AMOUNT",&amt);
							cy=atoi(&edate[6]);
							deb=0.0;cred=0.0;
							FINDFLDGETSTRING(finrec,"BANK IDENTIFICATION",&bankid);
							if(AccountType==0 && amt>=0.0) 
							{
								cred=amt;
								addsortrecord(dbsrt,1,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							} else if(AccountType==0)
							{
								deb=amt*(-1);
								addsortrecord(dbsrt,1,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(AccountType==1 && amt>=0.0) 
							{
								cred=amt;
								addsortrecord(dbsrt,1,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							} else if(AccountType==1) 
							{
								deb=amt*(-1);
								addsortrecord(dbsrt,1,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
							{
								deb=amt;
								gy=0;
								addsortrecord(dbsrt,1,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
							{
								cred=amt;
								gy=0;
								addsortrecord(dbsrt,1,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
						}
					}
				}
			}
			if(!abort_diagnostic)
			{
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
			}
			ef=ADVNXTNRDsec(finrec,2,SCRNvirtualSubData,MAINRSRC);
		}
		ZERNRD(finrec);
		if(tmprsrc!=NULL) 
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	if(XPERT_SETUP->VENPMT)
	{
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"VENPMT") && !RDAstrcmp(SOURCE_PROCESS,"VCHDIST")))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(venfyr))+RDAstrlen(FILENAME(venfyr))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(venfyr),FILENAME(venfyr));
			tmprsrc=diagscrn(venfyr,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(venfyr))+RDAstrlen(FILENAME(venfyr))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(venfyr),FILENAME(venfyr));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(venfyr);
			FINDFLDSETINT(venfyr,"FISCAL YEAR",FiscalYear);
			ef=ADVGTENRDsec(venfyr,2,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(venfyr,"FISCAL YEAR",&fy);
				if(fy!=FiscalYear) break;
				FINDFLDGETCHAR(venfyr,"DELETEFLAG",&delflag);
				if(!delflag)
				{
					FINDFLDGETSTRING(venfyr,"VENDOR IDENTIFICATION",&venid1);
					ZERNRD(venvch);
					FINDFLDSETSTRING(venvch,"VENDOR IDENTIFICATION",venid1);
					ef2=ADVGTENRDsec(venvch,1,SCRNvirtualSubData,MAINRSRC);
					while(!ef2)
					{
						FINDFLDGETSTRING(venvch,"VENDOR IDENTIFICATION",&venid2);
						if(RDAstrcmp(venid1,venid2)) break;
						FINDFLDGETCHAR(venvch,"DELETEFLAG",&delflag);
						if(!delflag)
						{
							if(DISTRIBUTED_DATE!=NULL)
							{
								FINDFLDGETSTRING(venvch,"DISTRIBUTED DATE",&d_date);
								FINDFLDGETSTRING(venvch,"DISTRIBUTED TIME",&d_time);
								FINDFLDGETSTRING(venvch,"DISTRIBUTED BY",&su);
							}
							if((isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
							{
							FINDFLDGETSHORT(venvch,"FISCAL PROCESSING MONTH",&pm);
							FINDFLDGETINT(venvch,"FISCAL YEAR",&fy);
							FINDFLDGETCHAR(venvch,"ELECTRONIC TRANSFER",&d);
							FINDFLDGETCHAR(venvch,"VOID CHECK",&voidchk);
							FINDFLDGETSTRING(venvch,"DESCRIPTION",&desc);
							if(voidchk && !strncmp(desc,"VOID VOUCHER TRANSACTION",24))
							{
								FINDFLDGETSTRING(venvch,"VOID CHECK DATE",&adate);
							} else if(AFFECT_DATE_STYLE==0)
							{
								FINDFLDGETSTRING(venvch,"ENTRY DATE",&adate);
							} else if(d)
							{
								FINDFLDGETSTRING(venvch,"ELECTRONIC TRANSFER DATE",&adate);
							} else FINDFLDGETSTRING(venvch,"CHECK DATE",&adate);
							memset(DQUERYstemp,0,101);
							sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
							if((pm==PM || Style==1) && fy==FiscalYear && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
							{
								FINDFLDGETSHORT(venvch,"ACCOUNT TYPE",&acctype);
								FINDFLDGETSHORT(venvch,"DEFINITION TYPE",&deftype);
								FINDFLDGETINT(venvch,"GRANT YEAR",&gy);
								if(AccountType==2 || (gy==GrantYear || GrantYear==(-1)))
								{
								if(DefinitionType==deftype)
								{
									read_record=FALSE;
									if(acctype==AccountType && AccountType!=2)
									{
										FINDFLDGETSTRING(venvch,"ACCOUNT CODE",&acctcode);
										if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
									} else if(AccountType==2)
									{
										FINDFLDGETSTRING(venvch,"DEBIT CODE",&debcode);
										FINDFLDGETSTRING(venvch,"PAYABLE CODE",&payable);
										if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(payable,AccountCode)) read_record=TRUE;
									}
									FINDFLDGETCHAR(venvch,"DISTRIBUTED",&d);
									if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
									{
										ZERNRD(venmst);
										COPYFIELD(venvch,venmst,"VENDOR IDENTIFICATION");
										if(ADVEQLNRDsec(venmst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(venmst,1);
										FINDFLDGETSTRING(venvch,"SOURCE USER",&su);
										FINDFLDGETSTRING(venvch,"VENDOR IDENTIFICATION",&id);
										FINDFLDGETSTRING(venvch,"ENTRY DATE",&edate);
										FINDFLDGETSTRING(venvch,"DESCRIPTION",&desc);
										FINDFLDGETSTRING(venvch,"BANK IDENTIFICATION",&bankid);
										FINDFLDGETINT(venvch,"CHECK NUMBER",&checkno);
										if(c!=NULL) Rfree(c);
										memset(DQUERYstemp,0,101);
										if(checkno!=0)
										{
											sprintf(DQUERYstemp,"%d",checkno);
										} else {
											FINDFLDGETINT(venvch,"ELECTRONIC TRANSFER NUMBER",&checkno);
											sprintf(DQUERYstemp,"%d",checkno);
										}
										c=stralloc(DQUERYstemp);	
										FINDFLDGETSTRING(venvch,"INVOICE IDENTIFICATION",&ref);
										FINDFLDGETINT(venvch,"TRANSACTION NUMBER",&trans);
										FINDFLDGETDOUBLE(venvch,"AMOUNT",&amt);
										FINDFLDGETINT(venvch,"CALENDAR YEAR",&cy);
										deb=0.0;cred=0.0;
										if(AccountType==0 && amt>=0.0) 
										{
											deb=amt;
											addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										} else if(AccountType==0)
										{
											cred=amt*(-1);
											addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==1 && amt>=0.0)
										{
											deb=amt;
											addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										} else if(AccountType==1)
										{
											cred=amt*(-1);
											addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										} else if(AccountType==1 && amt>=0 && voidchk==TRUE)
										{
											deb=amt*(-1);
											addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										} else if(AccountType==1 && voidchk==TRUE)
										{
											cred=amt*(-1);
											addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2)
										{
											gy=0;
											if(!RDAstrcmp(debcode,AccountCode) && debcode!=NULL) 
											{
												deb=amt; 
												addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
											}
											if(!RDAstrcmp(payable,AccountCode) && payable!=NULL) 
											{
												cred=amt;
												addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
											}
										}
									}
								}
								}
							}
							}
						}
						ef2=ADVNXTNRDsec(venvch,1,SCRNvirtualSubData,MAINRSRC);
					}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				}
				ef=ADVNXTNRDsec(venfyr,2,SCRNvirtualSubData,MAINRSRC);
			}
			if(venid1!=NULL) Rfree(venid1);
			if(venid2!=NULL) Rfree(venid2);
			ZERNRD(venfyr);
			ZERNRD(venvch);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"VENPMT") && !RDAstrcmp(SOURCE_PROCESS,"VENCLCK")))
		{
			gy=0;
			if(AccountType==2)
			{
				e=Rmalloc(RDAstrlen(MODULENAME(venvch))+RDAstrlen(FILENAME(venvch))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(venvch),FILENAME(venvch));
				tmprsrc=diagscrn(venvch,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(venvch))+RDAstrlen(FILENAME(venvch))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(venvch),FILENAME(venvch));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				ZERNRD(venvch);
				ef=ADVGTENRDsec(venvch,1,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					read_record=FALSE;
					FINDFLDGETCHAR(venvch,"DELETEFLAG",&delflag);
					if(!delflag)
					{
						if(DISTRIBUTED_DATE!=NULL)
						{
							FINDFLDGETSTRING(venvch,"CLEARED DATE",&d_date);
							FINDFLDGETSTRING(venvch,"CLEARED TIME",&d_time);
							FINDFLDGETSTRING(venvch,"CLEARED BY",&su);
						}
						if(!isEMPTY(REFERENCE_ID))
						{
							FINDFLDGETSTRING(venvch,"JOURNAL IDENTIFICATION",&refid);
						}
						if((isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
						{
							FINDFLDGETSHORT(venvch,"CLEAR PROCESSING MONTH",&pm);
							FINDFLDGETINT(venvch,"CLEAR FISCAL YEAR",&fy);
							FINDFLDGETCHAR(venvch,"VOID CHECK",&voidchk);
							FINDFLDGETCHAR(venvch,"ELECTRONIC TRANSFER",&d);
							if(voidchk && !strncmp(desc,"VOID VOUCHER TRANSACTION",24))
							{
								FINDFLDGETSTRING(venvch,"VOID CHECK DATE",&adate);
							} else if(d)
							{
								FINDFLDGETSTRING(venvch,"ELECTRONIC TRANSFER DATE",&adate);
							} else FINDFLDGETSTRING(venvch,"CHECK DATE",&adate);
							memset(DQUERYstemp,0,101);
							sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
							if(((pm+1)==PM || Style==1) && fy==FiscalYear && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
							{
								FINDFLDGETSHORT(venvch,"ACCOUNT TYPE",&acctype);
								FINDFLDGETSHORT(venvch,"DEFINITION TYPE",&deftype);
								if(DefinitionType==deftype)
								{
									read_record=FALSE;
									FINDFLDGETSTRING(venvch,"PAYABLE CODE",&payable);
									FINDFLDGETSTRING(venvch,"CREDIT CODE",&credcode);
									FINDFLDGETSTRING(venvch,"CLEARING DUE FROM",&duefrom);
									FINDFLDGETSTRING(venvch,"CLEARING CASH",&clearcash);
									if(!RDAstrcmp(credcode,AccountCode) || !RDAstrcmp(payable,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(clearcash,AccountCode)) read_record=TRUE;
									FINDFLDGETCHAR(venvch,"CLEARED",&d);
									if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
									{
										ZERNRD(venmst);
										COPYFIELD(venvch,venmst,"VENDOR IDENTIFICATION");
										if(ADVEQLNRDsec(venmst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(venmst,1);
										FINDFLDGETSTRING(venvch,"VENDOR IDENTIFICATION",&id);
										FINDFLDGETSTRING(venvch,"ENTRY DATE",&edate);
										FINDFLDGETSTRING(venvch,"DESCRIPTION",&desc);
										FINDFLDGETSTRING(venvch,"SOURCE USER",&su);
										FINDFLDGETSTRING(venvch,"BANK IDENTIFICATION",&bankid);
										FINDFLDGETINT(venvch,"CHECK NUMBER",&checkno);
										if(c!=NULL) Rfree(c);
										memset(DQUERYstemp,0,101);
										if(checkno!=0)
										{
											sprintf(DQUERYstemp,"%d",checkno);
										} else {
											FINDFLDGETINT(venvch,"ELECTRONIC TRANSFER NUMBER",&checkno);
											sprintf(DQUERYstemp,"%d",checkno);
										}
										c=stralloc(DQUERYstemp);	
										FINDFLDGETSTRING(venvch,"INVOICE IDENTIFICATION",&ref);
										FINDFLDGETINT(venvch,"TRANSACTION NUMBER",&trans);
										FINDFLDGETDOUBLE(venvch,"AMOUNT",&amt);
										FINDFLDGETINT(venvch,"CALENDAR YEAR",&cy);
										deb=0.0;cred=0.0;
										FINDFLDGETCHAR(venvch,"CLEARED",&d);
										if(AccountType==2)
										{
											gy=0;
											if(!RDAstrcmp(duefrom,AccountCode) && duefrom!=NULL) 
											{
												deb=amt;
												addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
											}
											if(!RDAstrcmp(payable,AccountCode) && payable!=NULL) 
											{
												deb=amt;
												addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
											}
											if(!RDAstrcmp(credcode,AccountCode) && credcode!=NULL) 
											{
												cred=amt;
												addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
											}
											if(!RDAstrcmp(clearcash,AccountCode) && clearcash!=NULL) 
											{
												cred=amt;
												addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
											}
										}
									}
								}
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(venvch,2,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(venvch);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
			}
		}
	}


	if(XPERT_SETUP->PURORD)
	{
#ifndef __NO_ENCUMBRANCES__
	if(AccountType==2 && (GrantYear==0 || GrantYear==(-1)))
	{
#endif
	gy=0;
	if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"PURORD") && !RDAstrcmp(SOURCE_PROCESS,"PODIST")))
	{
	e=Rmalloc(RDAstrlen(MODULENAME(purtrn))+RDAstrlen(FILENAME(purtrn))+65);
	sprintf(e,"Gathering from Module %s File %s",MODULENAME(purtrn),FILENAME(purtrn));
	tmprsrc=diagscrn(purtrn,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
	if(e!=NULL) Rfree(e);
	e=NULL;
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(MODULENAME(purtrn))+RDAstrlen(FILENAME(purtrn))+15);
			sprintf(e,"%s %s QUERYING",MODULENAME(purtrn),FILENAME(purtrn));
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
	ZERNRD(purtrn);
	FINDFLDSETINT(purtrn,"FISCAL YEAR",FiscalYear);
	ef=ADVGTENRDsec(purtrn,1,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		ForceWindowUpdate(tmprsrc);
		read_record=FALSE;
		FINDFLDGETINT(purtrn,"FISCAL YEAR",&fy);
		if(fy!=FiscalYear) break;
		FINDFLDGETCHAR(purtrn,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSHORT(purtrn,"PROCESSING MONTH",&pm);
			if(DISTRIBUTED_DATE!=NULL)
			{
				FINDFLDGETSTRING(purtrn,"DISTRIBUTED DATE",&d_date);
				FINDFLDGETSTRING(purtrn,"DISTRIBUTED TIME",&d_time);
				FINDFLDGETSTRING(purtrn,"DISTRIBUTED BY",&su);
			}
			if((pm==PM || Style==1) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
			{
				ZERNRD(puracc);
				COPYFIELD(purtrn,puracc,"FISCAL YEAR");
				COPYFIELD(purtrn,puracc,"PO IDENTIFICATION");
				COPYFIELD(purtrn,puracc,"LINE NUMBER");
				COPYFIELD(purtrn,puracc,"ACCOUNT NUMBER");
				if(ADVEQLNRDsec(puracc,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(puracc,1);
				FINDFLDGETSHORT(puracc,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(puracc,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(purtrn,"ENTRY DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
				{
					read_record=FALSE;
#ifdef __NO_ENCUMBRANCES__
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(puracc,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							FINDFLDGETSTRING(puracc,"DEBIT CODE",&debcode);
							FINDFLDGETSTRING(puracc,"CREDIT CODE",&credcode);
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
#else 
					FINDFLDGETSTRING(puracc,"DEBIT CODE",&debcode);
					FINDFLDGETSTRING(puracc,"CREDIT CODE",&credcode);
					if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
#endif /* __NO_ENCUMBRANCES__ */
					FINDFLDGETCHAR(purtrn,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						ZERNRD(purmst);
						COPYFIELD(purtrn,purmst,"FISCAL YEAR");
						COPYFIELD(purtrn,purmst,"PO IDENTIFICATION");
						if(ADVEQLNRDsec(purmst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(purmst,1);
						ZERNRD(purlin);
						COPYFIELD(purtrn,purlin,"FISCAL YEAR");
						COPYFIELD(purtrn,purlin,"PO IDENTIFICATION");
						COPYFIELD(purtrn,purlin,"LINE NUMBER");
						if(ADVEQLNRDsec(purlin,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(purmst,1);
						FINDFLDGETSTRING(purtrn,"SITE IDENTIFICATION",&id);
						FINDFLDGETSTRING(purtrn,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(purlin,"ITEM IDENTIFICATION",&desc);
						FINDFLDGETINT(purtrn,"ACCOUNT NUMBER",&accnum);
						FINDFLDGETSTRING(purtrn,"SOURCE USER",&su);
						FINDFLDGETSTRING(purmst,"VENDOR IDENTIFICATION",&c);
						FINDFLDGETSTRING(purtrn,"PO IDENTIFICATION",&ref);
						FINDFLDGETINT(purtrn,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(purtrn,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,3,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,3,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0)
						{
							cred=amt;
							addsortrecord(dbsrt,3,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,3,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							gy=0;
							deb=amt;
							addsortrecord(dbsrt,3,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							gy=0;
							cred=amt;
							addsortrecord(dbsrt,3,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(bankid!=NULL) Rfree(bankid);
					}
				}
			}
		}
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
		}
		ef=ADVNXTNRDsec(purtrn,1,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(puracc);
	ZERNRD(purmst);
	ZERNRD(purlin);
	ZERNRD(purtrn);
	}
#ifndef __NO_ENCUMBRANCES__
	}
#endif
	if(tmprsrc!=NULL) 
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	}

	if(XPERT_SETUP->BNKREC)
	{
	if(((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"BNKREC") && !RDAstrcmp(SOURCE_PROCESS,"CHGDIST"))) && (GrantYear==0 || GrantYear==(-1)))
	{
	gy=0;
	e=Rmalloc(RDAstrlen(MODULENAME(bnkchg))+RDAstrlen(FILENAME(bnkchg))+65);
	sprintf(e,"Gathering from Module %s File %s",MODULENAME(bnkchg),FILENAME(bnkchg));
	tmprsrc=diagscrn(bnkchg,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
	if(e!=NULL) Rfree(e);
	e=NULL;
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(MODULENAME(bnkchg))+RDAstrlen(FILENAME(bnkchg))+15);
			sprintf(e,"%s %s QUERYING",MODULENAME(bnkchg),FILENAME(bnkchg));
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
	ZERNRD(bnkchg);
	ef=ADVGTENRDsec(bnkchg,1,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		ForceWindowUpdate(tmprsrc);
		read_record=FALSE;
		FINDFLDGETCHAR(bnkchg,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(bnkchg,"FISCAL YEAR",&fy);
			FINDFLDGETSHORT(bnkchg,"PROCESSING MONTH",&pm);
			if(DISTRIBUTED_DATE!=NULL)
			{
				FINDFLDGETSTRING(bnkchg,"DISTRIBUTED DATE",&d_date);
				FINDFLDGETSTRING(bnkchg,"DISTRIBUTED TIME",&d_time);
				FINDFLDGETSTRING(bnkchg,"DISTRIBUTED BY",&su);
			}
			if(!isEMPTY(REFERENCE_ID))
			{
				FINDFLDGETSTRING(bnkchg,"BANK IDENTIFICATION",&refid);
			}
			if((pm==PM || Style==1) && fy==FiscalYear && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
			{
				FINDFLDGETSHORT(bnkchg,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(bnkchg,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(bnkchg,"CHARGE DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
				{
					read_record=FALSE;
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(bnkchg,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							FINDFLDGETSTRING(bnkchg,"DEBIT CODE",&debcode);
							FINDFLDGETSTRING(bnkchg,"CREDIT CODE",&credcode);
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(bnkchg,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						FINDFLDGETSTRING(bnkchg,"BANK IDENTIFICATION",&id);
						FINDFLDGETSTRING(bnkchg,"BANK IDENTIFICATION",&bankid);
						FINDFLDGETSTRING(bnkchg,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(bnkchg,"SOURCE USER",&su);
						FINDFLDGETSTRING(bnkchg,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(bnkchg,"CONTROL",&c);
						FINDFLDGETSTRING(bnkchg,"REFERENCE",&ref);
						FINDFLDGETINT(bnkchg,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(bnkchg,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							gy=0;
							deb=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							gy=0;
							cred=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
			}
		}
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
		}
		ef=ADVNXTNRDsec(bnkchg,2,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(bnkchg);
	if(tmprsrc!=NULL) 
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	}


	if(((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"BNKREC") && !RDAstrcmp(SOURCE_PROCESS,"RTNDIST"))) && (GrantYear==0 || GrantYear==(-1)))
	{
	gy=0;
	e=Rmalloc(RDAstrlen(MODULENAME(bnkrtn))+RDAstrlen(FILENAME(bnkrtn))+65);
	sprintf(e,"Gathering from Module %s File %s",MODULENAME(bnkrtn),FILENAME(bnkrtn));
	tmprsrc=diagscrn(bnkrtn,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
	if(e!=NULL) Rfree(e);
	e=NULL;
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		} else {
			e=Rmalloc(RDAstrlen(MODULENAME(bnkrtn))+RDAstrlen(FILENAME(bnkrtn))+15);
			sprintf(e,"%s %s QUERYING",MODULENAME(bnkrtn),FILENAME(bnkrtn));
			SETTITLE(tmprsrc,e);
			if(e!=NULL) Rfree(e);
			ForceWindowUpdate(tmprsrc);
		}
	}
	ZERNRD(bnkrtn);
	ef=ADVGTENRDsec(bnkrtn,1,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		ForceWindowUpdate(tmprsrc);
		read_record=FALSE;
		FINDFLDGETCHAR(bnkrtn,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(bnkrtn,"FISCAL YEAR",&fy);
			FINDFLDGETSHORT(bnkrtn,"PROCESSING MONTH",&pm);
			if(DISTRIBUTED_DATE!=NULL)
			{
				FINDFLDGETSTRING(bnkrtn,"DISTRIBUTED DATE",&d_date);
				FINDFLDGETSTRING(bnkrtn,"DISTRIBUTED TIME",&d_time);
				FINDFLDGETSTRING(bnkrtn,"DISTRIBUTED BY",&su);
			}
			if((pm==PM || Style==1) && fy==FiscalYear && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
			{
				FINDFLDGETSHORT(bnkrtn,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(bnkrtn,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(bnkrtn,"CLEARED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
				{
					read_record=FALSE;
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(bnkrtn,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							FINDFLDGETSTRING(bnkrtn,"DEBIT CODE",&debcode);
							FINDFLDGETSTRING(bnkrtn,"CREDIT CODE",&credcode);
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(bnkrtn,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						FINDFLDGETSTRING(bnkrtn,"BANK IDENTIFICATION",&id);
						FINDFLDGETSTRING(bnkrtn,"BANK IDENTIFICATION",&bankid);
						FINDFLDGETSTRING(bnkrtn,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(bnkrtn,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(bnkrtn,"SOURCE USER",&su);
						FINDFLDGETSTRING(bnkrtn,"CHECK NUMBER",&c);
						FINDFLDGETSTRING(bnkrtn,"REFERENCE",&ref);
						FINDFLDGETINT(bnkrtn,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(bnkrtn,"AMOUNT",&amt);
						amt*=(-1);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							gy=0;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							gy=0;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
			}
			FINDFLDGETINT(bnkrtn,"REDEPOSIT FISCAL YEAR",&fy);
			FINDFLDGETSHORT(bnkrtn,"REDEPOSIT PROCESSING MONTH",&pm);
			if(DISTRIBUTED_DATE!=NULL)
			{
				FINDFLDGETSTRING(bnkrtn,"REDEPOSIT DISTRIBUTED DATE",&d_date);
				FINDFLDGETSTRING(bnkrtn,"REDEPOSIT DISTRIBUTED TIME",&d_time);
				FINDFLDGETSTRING(bnkrtn,"REDEPOSIT DISTRIBUTED BY",&su);
			}
			if((pm==PM || Style==1) && fy==FiscalYear && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
			{
				FINDFLDGETSHORT(bnkrtn,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(bnkrtn,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(bnkrtn,"REDEPOSIT CLEARED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
				{
					read_record=FALSE;
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(bnkrtn,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							FINDFLDGETSTRING(bnkrtn,"DEBIT CODE",&debcode);
							FINDFLDGETSTRING(bnkrtn,"CREDIT CODE",&credcode);
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(bnkrtn,"REDEPOSIT DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						FINDFLDGETSTRING(bnkrtn,"BANK IDENTIFICATION",&id);
						FINDFLDGETSTRING(bnkrtn,"BANK IDENTIFICATION",&bankid);
						FINDFLDGETSTRING(bnkrtn,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(bnkrtn,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(bnkrtn,"SOURCE USER",&su);
						FINDFLDGETSTRING(bnkrtn,"CHECK NUMBER",&c);
						FINDFLDGETSTRING(bnkrtn,"REFERENCE",&ref);
						FINDFLDGETINT(bnkrtn,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(bnkrtn,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode))
						{
							deb=amt;
							gy=0;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							gy=0;
							addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
			}
		}
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
		}
		ef=ADVNXTNRDsec(bnkrtn,2,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(bnkrtn);
	if(tmprsrc!=NULL) 
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	}

	}

	if(XPERT_SETUP->PAYROLL)
	{
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"PAYROLL") && !RDAstrcmp(SOURCE_PROCESS,"PAYVDST")))
		{
			if(AccountType==2 && (GrantYear==0 || GrantYear==(-1)))
			{
				gy=0;
				e=Rmalloc(RDAstrlen(MODULENAME(payddsv))+RDAstrlen(FILENAME(payddsv))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(payddsv),FILENAME(payddsv));
				tmprsrc=diagscrn(payddsv,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(payddsv))+RDAstrlen(FILENAME(payddsv))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(payddsv),FILENAME(payddsv));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				ZERNRD(payddsv);
				FINDFLDSETINT(payddsv,"FISCAL YEAR",FiscalYear);
				ef=ADVGTENRDsec(payddsv,2,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					FINDFLDGETINT(payddsv,"FISCAL YEAR",&fy);
					if(fy!=FiscalYear) break;
					read_record=FALSE;
					FINDFLDGETCHAR(payddsv,"DELETEFLAG",&delflag);
					FINDFLDGETSHORT(payddsv,"FISCAL PROCESSING MONTH",&pm);
					FINDFLDGETCHAR(payddsv,"VOID TRANSACTION",&voidchk);
					if(voidchk)
					{
						FINDFLDGETSTRING(payddsv,"ENTRY DATE",&adate);
					} else {
						FINDFLDGETSTRING(payddsv,"CHECK DATE",&adate);
						if(isEMPTY(adate))
						{
							FINDFLDGETSTRING(payddsv,"ELECTRONIC TRANSFER DATE",&adate);
						}
					}
					ZERNRD(payddms);
					COPYFIELD(payddsv,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
					if(ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payddms,1);
					FINDFLDGETSHORT(payddsv,"DEFINITION TYPE",&deftype);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
					if(DISTRIBUTED_DATE!=NULL)
					{
						FINDFLDGETSTRING(payddsv,"DISTRIBUTED DATE",&d_date);
						FINDFLDGETSTRING(payddsv,"DISTRIBUTED TIME",&d_time);
						FINDFLDGETSTRING(payddsv,"DISTRIBUTED BY",&su);
					}
					if(!isEMPTY(REFERENCE_ID))
					{
						if(payvdst_option==1)
						{
							FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&refid);
						} else if(payvdst_option==2)
						{
							FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&refid);
						}
					}
					if((pm==PM || Style==1) && fy==FiscalYear && !delflag && DefinitionType==deftype  && (isEMPTY(REFERENCE_ID) || (!RDAstrcmp(REFERENCE_ID,refid))) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
					{
						FINDFLDGETSTRING(payddsv,"DEBIT CODE",&debcode);
						FINDFLDGETSTRING(payddsv,"CREDIT CODE",&credcode);
						read_record=FALSE;
						if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						FINDFLDGETCHAR(payddsv,"DISTRIBUTED",&d);
						if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
						{
							FINDFLDGETSTRING(payddsv,"VENDOR IDENTIFICATION",&id);
							FINDFLDGETSTRING(payddsv,"ENTRY DATE",&edate);
							FINDFLDGETSTRING(payddms,"SHORT DESCRIPTION",&desc);
							FINDFLDGETINT(payddsv,"CHECK NUMBER",&checkno);
							memset(DQUERYstemp,0,101);
							if(checkno!=0)
							{
								sprintf(DQUERYstemp,"%d",checkno);
							} else {
								FINDFLDGETINT(payddsv,"ELECTRONIC TRANSFER NUMBER",&checkno);
								sprintf(DQUERYstemp,"%d",checkno);
							}
							c=stralloc(DQUERYstemp);	
							FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&ref);
							FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&trans);
							FINDFLDGETSTRING(payddsv,"SOURCE USER",&su);
							FINDFLDGETDOUBLE(payddsv,"AMOUNT",&amt);
							cy=atoi(&edate[6]);
							deb=0.0;cred=0.0;
							FINDFLDGETSTRING(payddsv,"BANK IDENTIFICATION",&bankid);
							if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
							{
								deb=amt;
								gy=0;
								addsortrecord(dbsrt,6,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
							if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
							{
								cred=amt;
								gy=0;
								addsortrecord(dbsrt,6,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(payddsv,2,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(payddsv);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
			}
		}
		if(PayrollActivity==FALSE)
		{
			if(AccountType==2 && GrantYear<=0)
			{
				gy=0;
				if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"PAYROLL") && !RDAstrcmp(SOURCE_PROCESS,"PAYCLCK")))
				{
					e=Rmalloc(RDAstrlen(MODULENAME(payjtrn))+RDAstrlen(FILENAME(payjtrn))+65);
					sprintf(e,"Gathering from Module %s File %s",MODULENAME(payjtrn),FILENAME(payjtrn));
					tmprsrc=diagscrn(payjtrn,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
					if(e!=NULL) Rfree(e);
					e=NULL;
					if(tmprsrc!=NULL)
					{
						if(ADVmakescrn(tmprsrc,TRUE))
						{
							e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
							sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
							WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
							prterr("Error %s.",e);
							if(e!=NULL) Rfree(e);
						} else {
							e=Rmalloc(RDAstrlen(MODULENAME(payjtrn))+RDAstrlen(FILENAME(payjtrn))+15);
							sprintf(e,"%s %s QUERYING",MODULENAME(payjtrn),FILENAME(payjtrn));
							SETTITLE(tmprsrc,e);
							if(e!=NULL) Rfree(e);
							ForceWindowUpdate(tmprsrc);
						}
					}
					ZERNRD(payjtrn);
					if(!isEMPTY(REFERENCE_ID))
					{
						FINDFLDSETSTRING(payjtrn,"PAY IDENTIFICATION",REFERENCE_ID);
					}
					ef=ADVGTENRDsec(payjtrn,2,SCRNvirtualSubData,MAINRSRC);
					while(!ef)
					{
						ForceWindowUpdate(tmprsrc);
						if(!isEMPTY(REFERENCE_ID))
						{
							FINDFLDGETSTRING(payjtrn,"PAY IDENTIFICATION",&refid);
							if(RDAstrcmp(REFERENCE_ID,refid)) break;
						}
						FINDFLDGETINT(payjtrn,"CLEAR FISCAL YEAR",&fy);
						FINDFLDGETSHORT(payjtrn,"CLEAR PROCESSING MONTH",&pm);
						FINDFLDGETCHAR(payjtrn,"DELETEFLAG",&delflag);
						FINDFLDGETSTRING(payjtrn,"SOURCE USER",&su);
						FINDFLDGETSHORT(payjtrn,"DEFINITION TYPE",&deftype);
						if(DISTRIBUTED_DATE!=NULL)
						{
							FINDFLDGETSTRING(payjtrn,"CLEARED DATE",&d_date);
							FINDFLDGETSTRING(payjtrn,"CLEARED TIME",&d_time);
							FINDFLDGETSTRING(payjtrn,"CLEARED BY",&su);
						}
						if((pm==PM || Style==1) && fy==FiscalYear && !delflag && DefinitionType==deftype && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
						{
							ZERNRD(paymst);
							COPYFIELD(payjtrn,paymst,"PAY IDENTIFICATION");
							if(ADVEQLNRDsec(paymst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(paymst,1);
							ZERNRD(payjpms);
							COPYFIELD(payjtrn,payjpms,"PERSONNEL IDENTIFICATION");
							COPYFIELD(payjtrn,payjpms,"PAY IDENTIFICATION");
							COPYFIELD(payjtrn,payjpms,"JOB NUMBER");
							if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
							FINDFLDGETCHAR(paymst,"VOID PAY MASTER",&voidchk);
							if(voidchk)
							{
								FINDFLDGETSTRING(paymst,"PAY DATE",&adate);
							} else {
								FINDFLDGETSTRING(payjpms,"CHECK DATE",&adate);
								if(isEMPTY(adate))
								{
									FINDFLDGETSTRING(payjpms,"DIRECT DEPOSIT DATE",&adate);
								}
							}
							memset(DQUERYstemp,0,101);
							sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
							if(!delflag && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
							{
								read_record=FALSE;
								if(AccountType==2)
								{
									FINDFLDGETSTRING(payjtrn,"PAYABLE CODE",&payable);
									FINDFLDGETSTRING(payjtrn,"CREDIT CODE",&credcode);
									FINDFLDGETSTRING(payjtrn,"DUE FROM",&duefrom);
									FINDFLDGETSTRING(payjtrn,"CASH CODE",&clearcash);
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(payable,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(clearcash,AccountCode)) read_record=TRUE;
								}
								FINDFLDGETCHAR(payjtrn,"CLEARED",&d);
								if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
								{
									ZERNRD(perdmg);
									COPYFIELD(payjtrn,perdmg,"PERSONNEL IDENTIFICATION");
									if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(perdmg,1);
									ZERNRD(payjpms);
									COPYFIELD(payjtrn,payjpms,"PERSONNEL IDENTIFICATION");
									COPYFIELD(payjtrn,payjpms,"PAY IDENTIFICATION");
									COPYFIELD(payjtrn,payjpms,"JOB NUMBER");
									if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
									if(EmployeeName!=NULL)
									{
										EmployeeName->computed=FALSE;
										computevirtual(EmployeeName,SCRNvirtualSubData,MAINRSRC);
										desc=stralloc(EmployeeName->value.string_value);
									}
									FINDFLDGETSTRING(payjtrn,"PERSONNEL IDENTIFICATION",&id);
									FINDFLDGETSTRING(payjtrn,"ENTRY DATE",&edate);
									FINDFLDGETSTRING(payjtrn,"SOURCE USER",&su);
									FINDFLDGETINT(payjpms,"CHECK NUMBER",&checkno);
									memset(DQUERYstemp,0,101);
									if(checkno!=0)
									{
										sprintf(DQUERYstemp,"%d",checkno);
									} else {
										FINDFLDGETINT(payjpms,"DIRECT DEPOSIT NUMBER",&checkno);
										sprintf(DQUERYstemp,"%d",checkno);
									}
									c=stralloc(DQUERYstemp);	
									FINDFLDGETSTRING(payjtrn,"PAY IDENTIFICATION",&ref);
									FINDFLDGETINT(payjtrn,"JOB NUMBER",&trans);
									FINDFLDGETINT(payjtrn,"ACCOUNT NUMBER",&accnum);
									FINDFLDGETDOUBLE(payjtrn,"AMOUNT",&amt);
									FINDFLDGETINT(paymst,"CALENDAR YEAR",&cy);
									deb=0.0;cred=0.0;
									FINDFLDGETSTRING(payjpms,"BANK IDENTIFICATION",&bankid);
									if(AccountType==2 && !RDAstrcmp(debcode,AccountCode))
									{
										deb=amt;
										addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									}
									if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode)) 
									{
										deb=amt;
										addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									}
									if(AccountType==2 && !RDAstrcmp(payable,AccountCode))
									{
										cred=amt;
										addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									}
									if(AccountType==2 && !RDAstrcmp(clearcash,AccountCode)) 
									{
										cred=amt;
										addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									}
								}
							}
						}
						if(!abort_diagnostic)
						{
							if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
						}
						ef=ADVNXTNRDsec(payjtrn,2,SCRNvirtualSubData,MAINRSRC);
					}
					if(tmprsrc!=NULL) 
					{
						killwindow(tmprsrc);
						free_rsrc(tmprsrc);
					}
					ZERNRD(payjtrn);
				}
			}
			if(AccountType!=1)
			{
				if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"PAYROLL") && !RDAstrcmp(SOURCE_PROCESS,"PAYDIST")))
				{
					e=Rmalloc(RDAstrlen(MODULENAME(paymst))+RDAstrlen(FILENAME(paymst))+65);
					sprintf(e,"Gathering from Module %s File %s",MODULENAME(paymst),FILENAME(paymst));
					tmprsrc=diagscrn(paymst,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
					if(e!=NULL) Rfree(e);
					e=NULL;
					if(tmprsrc!=NULL)
					{
						if(ADVmakescrn(tmprsrc,TRUE))
						{
							e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
							sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
							WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
							prterr("Error %s.",e);
							if(e!=NULL) Rfree(e);
						} else {
							e=Rmalloc(RDAstrlen(MODULENAME(paymst))+RDAstrlen(FILENAME(paymst))+15);
							sprintf(e,"%s %s QUERYING",MODULENAME(paymst),FILENAME(paymst));
							SETTITLE(tmprsrc,e);
							if(e!=NULL) Rfree(e);
							ForceWindowUpdate(tmprsrc);
						}
					}
					ZERNRD(paymst);
					if(!isEMPTY(REFERENCE_ID))
					{
						FINDFLDSETSTRING(paymst,"PAY IDENTIFICATION",REFERENCE_ID);
					}
					ef=ADVGTENRDsec(paymst,1,SCRNvirtualSubData,MAINRSRC);
					while(!ef)
					{
						ForceWindowUpdate(tmprsrc);
						if(!isEMPTY(REFERENCE_ID))
						{
							FINDFLDGETSTRING(paymst,"PAY IDENTIFICATION",&refid);
							if(RDAstrcmp(REFERENCE_ID,refid)) break;
						}
						FINDFLDGETINT(paymst,"FISCAL YEAR",&fy);
						FINDFLDGETSHORT(paymst,"FISCAL MONTH",&pm);
						FINDFLDGETCHAR(paymst,"DELETEFLAG",&delflag);
						FINDFLDGETCHAR(paymst,"VOID PAY MASTER",&voidchk);
						FINDFLDGETINT(paymst,"GRANT YEAR",&gy);
						if(fy==FiscalYear && (pm==PM || Style==1) && !delflag && (gy==GrantYear || GrantYear==(-1)))
						{
							FINDFLDGETSTRING(paymst,"PAY IDENTIFICATION",&payid1);
							ZERNRD(payjtrn);
							FINDFLDSETSTRING(payjtrn,"PAY IDENTIFICATION",payid1);
							ef2=ADVGTENRDsec(payjtrn,2,SCRNvirtualSubData,MAINRSRC);
							while(!ef2)
							{
								FINDFLDGETSTRING(payjtrn,"PAY IDENTIFICATION",&payid2);
								if(RDAstrcmp(payid1,payid2)) break;
								FINDFLDGETCHAR(payjtrn,"DELETEFLAG",&delflag);
								ZERNRD(payjpms);
								COPYFIELD(payjtrn,payjpms,"PERSONNEL IDENTIFICATION");
								COPYFIELD(payjtrn,payjpms,"PAY IDENTIFICATION");
								COPYFIELD(payjtrn,payjpms,"JOB NUMBER");
								if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
								if(voidchk)
								{
									FINDFLDGETSTRING(paymst,"PAY DATE",&adate);
								} else {
									FINDFLDGETSTRING(payjpms,"CHECK DATE",&adate);
									if(isEMPTY(adate))
									{
										FINDFLDGETSTRING(payjpms,"DIRECT DEPOSIT DATE",&adate);
									}
								}
								memset(DQUERYstemp,0,101);
								sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
								FINDFLDGETSHORT(payjtrn,"DEFINITION TYPE",&deftype);
								if(DISTRIBUTED_DATE!=NULL)
								{
									FINDFLDGETSTRING(payjtrn,"DISTRIBUTED DATE",&d_date);
									FINDFLDGETSTRING(payjtrn,"DISTRIBUTED TIME",&d_time);
									FINDFLDGETSTRING(payjtrn,"DISTRIBUTED BY",&su);
								}
								if(!delflag && DefinitionType==deftype  && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
								{
									read_record=FALSE;
									if((AccountType==2) || (AccountType==0)) 
									{
										if(AccountType<2) 
										{
											FINDFLDGETSTRING(payjtrn,"ACCOUNT CODE",&acctcode);
											if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
										}
/* Note:  Deal with Clearing Checks */
										if(AccountType==2)
										{
											FINDFLDGETSTRING(payjtrn,"DEBIT CODE",&debcode);
											FINDFLDGETSTRING(payjtrn,"PAYABLE CODE",&payable);
											if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(payable,AccountCode)) read_record=TRUE;
										}
									}
									FINDFLDGETCHAR(payjtrn,"DISTRIBUTED",&d);
									if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
									{
										ZERNRD(perdmg);
										COPYFIELD(payjtrn,perdmg,"PERSONNEL IDENTIFICATION");
										if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(perdmg,1);
										ZERNRD(payjpms);
										COPYFIELD(payjtrn,payjpms,"PERSONNEL IDENTIFICATION");
										COPYFIELD(payjtrn,payjpms,"PAY IDENTIFICATION");
										COPYFIELD(payjtrn,payjpms,"JOB NUMBER");
										if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
										if(EmployeeName!=NULL)
										{
											EmployeeName->computed=FALSE;
											computevirtual(EmployeeName,SCRNvirtualSubData,MAINRSRC);
											desc=stralloc(EmployeeName->value.string_value);
										}
										FINDFLDGETSTRING(payjtrn,"PERSONNEL IDENTIFICATION",&id);
										FINDFLDGETSTRING(payjtrn,"ENTRY DATE",&edate);
										FINDFLDGETINT(payjpms,"CHECK NUMBER",&checkno);
										memset(DQUERYstemp,0,101);
										if(checkno!=0)
										{
											sprintf(DQUERYstemp,"%d",checkno);
										} else {
											FINDFLDGETINT(payjpms,"DIRECT DEPOSIT NUMBER",&checkno);
											sprintf(DQUERYstemp,"%d",checkno);
										}
										c=stralloc(DQUERYstemp);	
										FINDFLDGETSTRING(payjtrn,"PAY IDENTIFICATION",&ref);
										FINDFLDGETINT(payjtrn,"JOB NUMBER",&trans);
										FINDFLDGETINT(payjtrn,"ACCOUNT NUMBER",&accnum);
										FINDFLDGETSTRING(payjtrn,"SOURCE USER",&su);
										FINDFLDGETDOUBLE(payjtrn,"AMOUNT",&amt);
										FINDFLDGETINT(paymst,"CALENDAR YEAR",&cy);
										deb=0.0;cred=0.0;
										FINDFLDGETSTRING(payjpms,"BANK IDENTIFICATION",&bankid);
										if(AccountType==0 && amt>=0.0) 
										{
											deb=amt;
											addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										} else if(AccountType==0) 
										{
											cred=amt*(-1);
											addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
										{
											deb=amt;
											gy=0;
											addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(payable,AccountCode)) 
										{
											cred=amt;
											gy=0;
											addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
									}
								}
								ef2=ADVNXTNRDsec(payjtrn,2,SCRNvirtualSubData,MAINRSRC);
							}
							ZERNRD(paydtrn);
							FINDFLDSETSTRING(paydtrn,"PAY IDENTIFICATION",payid1);
							ef2=ADVGTENRDsec(paydtrn,2,SCRNvirtualSubData,MAINRSRC);
							while(!ef2)
							{
								FINDFLDGETSTRING(paydtrn,"PAY IDENTIFICATION",&payid2);
								if(RDAstrcmp(payid1,payid2)) break;
								FINDFLDGETCHAR(paydtrn,"DELETEFLAG",&delflag);
								ZERNRD(payjpms);
								COPYFIELD(paydtrn,payjpms,"PERSONNEL IDENTIFICATION");
								COPYFIELD(paydtrn,payjpms,"PAY IDENTIFICATION");
								COPYFIELD(paydtrn,payjpms,"JOB NUMBER");
								if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
								if(voidchk)
								{
									FINDFLDGETSTRING(paymst,"PAY DATE",&adate);
								} else {
									FINDFLDGETSTRING(payjpms,"CHECK DATE",&adate);
									if(isEMPTY(adate))
									{
										FINDFLDGETSTRING(payjpms,"DIRECT DEPOSIT DATE",&adate);
									}
								}
								memset(DQUERYstemp,0,101);
								sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
								FINDFLDGETSHORT(paydtrn,"DEFINITION TYPE",&deftype);
								if(DISTRIBUTED_DATE!=NULL)
								{
									FINDFLDGETSTRING(paydtrn,"DISTRIBUTED DATE",&d_date);
									FINDFLDGETSTRING(paydtrn,"DISTRIBUTED TIME",&d_time);
									FINDFLDGETSTRING(paydtrn,"DISTRIBUTED BY",&su);
								}
								if(!delflag && DefinitionType==deftype  && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
								{
									read_record=FALSE;
									if((AccountType==2) || (AccountType==0)) 
									{
										if(AccountType<2) 
										{
											FINDFLDGETSTRING(paydtrn,"ACCOUNT CODE",&acctcode);
											if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
										}
/* Note:  Deal with Clearing Checks */
										if(AccountType==2)
										{
											FINDFLDGETSTRING(paydtrn,"DEBIT CODE",&debcode);
											FINDFLDGETSTRING(paydtrn,"CREDIT CODE",&credcode);
											FINDFLDGETSTRING(paydtrn,"LIABILITY CODE",&payable);
											FINDFLDGETSTRING(paydtrn,"DUE FROM",&duefrom);
											FINDFLDGETSTRING(paydtrn,"CASH CODE",&clearcash);
											if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode) || !RDAstrcmp(payable,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(clearcash,AccountCode)) read_record=TRUE;
										}
									}
									FINDFLDGETCHAR(paydtrn,"DISTRIBUTED",&d);
									if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
									{
										ZERNRD(perdmg);
										COPYFIELD(paydtrn,perdmg,"PERSONNEL IDENTIFICATION");
										if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(perdmg,1);
										ZERNRD(payjpms);
										COPYFIELD(paydtrn,payjpms,"PERSONNEL IDENTIFICATION");
										COPYFIELD(paydtrn,payjpms,"PAY IDENTIFICATION");
										COPYFIELD(paydtrn,payjpms,"JOB NUMBER");
										if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
										if(EmployeeName!=NULL)
										{
											EmployeeName->computed=FALSE;
											computevirtual(EmployeeName,SCRNvirtualSubData,MAINRSRC);
											desc=stralloc(EmployeeName->value.string_value);
										}
										FINDFLDGETSTRING(paydtrn,"PERSONNEL IDENTIFICATION",&id);
										FINDFLDGETSTRING(paydtrn,"ENTRY DATE",&edate);
										FINDFLDGETINT(payjpms,"CHECK NUMBER",&checkno);
										memset(DQUERYstemp,0,101);
										if(checkno!=0)
										{
											sprintf(DQUERYstemp,"%d",checkno);
										} else {
											FINDFLDGETINT(payjpms,"DIRECT DEPOSIT NUMBER",&checkno);
											sprintf(DQUERYstemp,"%d",checkno);
										}
										c=stralloc(DQUERYstemp);	
										FINDFLDGETSTRING(paydtrn,"PAY IDENTIFICATION",&ref);
										FINDFLDGETINT(paydtrn,"JOB NUMBER",&trans);
										FINDFLDGETSTRING(paydtrn,"SOURCE USER",&su);
										FINDFLDGETDOUBLE(paydtrn,"AMOUNT",&amt);
										FINDFLDGETINT(paydtrn,"ACCOUNT NUMBER",&accnum);
										FINDFLDGETINT(paymst,"CALENDAR YEAR",&cy);
										deb=0.0;cred=0.0;
										if(AccountType==0 && amt>=0.0) 
										{
											deb=amt;
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										} else if(AccountType==0) 
										{
											cred=amt*(-1);
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(debcode,AccountCode))
										{
											gy=0;
											deb=amt;
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode))
										{
											gy=0;
											deb=amt;
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(payable,AccountCode)) 
										{
											gy=0;
											deb=amt;
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(credcode,AccountCode))
										{
											gy=0;
											cred=amt;
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(payable,AccountCode))
										{
											gy=0;
											cred=amt;
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(AccountType==2 && !RDAstrcmp(clearcash,AccountCode)) 
										{
											gy=0;
											cred=amt;
											addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
										}
										if(bankid!=NULL) Rfree(bankid);
									}
								}
								ef2=ADVNXTNRDsec(paydtrn,2,SCRNvirtualSubData,MAINRSRC);
							}
						}
						if(!abort_diagnostic)
						{
							if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
						}
						ef=ADVNXTNRDsec(paymst,1,SCRNvirtualSubData,MAINRSRC);
					}
					ZERNRD(paymst);
					ZERNRD(payjtrn);
					ZERNRD(paydtrn);
					if(tmprsrc!=NULL) 
					{
						killwindow(tmprsrc);
						free_rsrc(tmprsrc);
					}
				}
			}
		} else if(PayrollActivity==TRUE)
		{
			if(AccountType==2 && SELECT_DISTRIBUTED==TRUE && (GrantYear==0 || GrantYear==(-1)))
			{
				gy=0;
				e=Rmalloc(RDAstrlen(MODULENAME(finbact))+RDAstrlen(FILENAME(finbact))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(finbact),FILENAME(finbact));
				tmprsrc=diagscrn(finbact,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(finbact))+RDAstrlen(FILENAME(finbact))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(finbact),FILENAME(finbact));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				if(ref!=NULL) Rfree(ref);
				if(c!=NULL) Rfree(c);
				ZERNRD(finbact);
				FINDFLDSETSHORT(finbact,"DEFINITION TYPE",DefinitionType);
				FINDFLDSETINT(finbact,"FISCAL YEAR",FiscalYear);
				ef=ADVGTENRDsec(finbact,2,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					FINDFLDGETINT(finbact,"FISCAL YEAR",&fy);
					FINDFLDGETSHORT(finbact,"DEFINITION TYPE",&deftype);
					if(fy!=FiscalYear || DefinitionType!=deftype) break;
					read_record=FALSE;
					FINDFLDGETSTRING(finbact,"SOURCE MODULE",&source_mod);
					FINDFLDGETSTRING(finbact,"SOURCE PROCESS",&source_proc);
					FINDFLDGETCHAR(finbact,"DELETEFLAG",&delflag);
					if(!isEMPTY(REFERENCE_ID))
					{
						FINDFLDGETSTRING(finbact,"PAY IDENTIFICATION",&refid);
					}
					if(!delflag && !RDAstrcmp(source_mod,"PAYROLL") && RDAstrcmp(source_proc,"PAYVDST") && (isEMPTY(REFERENCE_ID) || (!RDAstrcmp(REFERENCE_ID,refid))))
					{
						read_record=TRUE;
						FINDFLDGETSHORT(finbact,"PROCESSING MONTH",&pm);
						FINDFLDGETSTRING(finbact,"ACCOUNT CODE",&acctcode);
						if((pm==PM || Style==1) && !RDAstrcmp(acctcode,AccountCode))
						{
							acctype=2;
							if(SWB_FM25_AFFECT_DATE==TRUE)
							{
								FINDFLDGETSTRING(finbact,"AFFECT DATE",&adate);
							} else {
								FINDFLDGETSTRING(finbact,"ENTRY DATE",&adate);
							}
							memset(DQUERYstemp,0,101);
							sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
							if((isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
							{
								FINDFLDGETSHORT(finbact,"TYPE",&type);
								id=stralloc(source_proc);
								FINDFLDGETSTRING(finbact,"ENTRY DATE",&edate);
								FINDFLDGETSTRING(finbact,"COMMENTS",&desc);
								FINDFLDGETINT(finbact,"TRANSACTION NUMBER",&trans);
								d=TRUE;
								FINDFLDGETSTRING(finbact,"SOURCE USER",&su);
								FINDFLDGETDOUBLE(finbact,"AMOUNT",&amt);
								cy=atoi(&edate[6]);
								deb=0.0;cred=0.0;
								if(type==0) deb=amt;
									else cred=amt;
								addsortrecord(dbsrt,25,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(finbact,2,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(finbact);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
				if(source_mod!=NULL) Rfree(source_mod);
				if(source_proc!=NULL) Rfree(source_proc);
			} else if(AccountType==0 && SELECT_DISTRIBUTED==TRUE)
			{
				e=Rmalloc(RDAstrlen(MODULENAME(fineact))+RDAstrlen(FILENAME(fineact))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(fineact),FILENAME(fineact));
				tmprsrc=diagscrn(fineact,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(fineact))+RDAstrlen(FILENAME(fineact))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(fineact),FILENAME(fineact));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				if(ref!=NULL) Rfree(ref);
				if(c!=NULL) Rfree(c);
				ZERNRD(fineact);
				FINDFLDSETSHORT(fineact,"DEFINITION TYPE",DefinitionType);
				FINDFLDSETINT(fineact,"FISCAL YEAR",FiscalYear);
				ef=ADVGTENRDsec(fineact,2,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					FINDFLDGETINT(fineact,"FISCAL YEAR",&fy);
					FINDFLDGETSHORT(fineact,"DEFINITION TYPE",&deftype);
					if(fy!=FiscalYear || DefinitionType!=deftype) break;
					read_record=FALSE;
					FINDFLDGETSTRING(fineact,"SOURCE MODULE",&source_mod);
					FINDFLDGETSTRING(fineact,"SOURCE PROCESS",&source_proc);
					FINDFLDGETCHAR(fineact,"DELETEFLAG",&delflag);
					FINDFLDGETINT(fineact,"GRANT YEAR",&gy);
					if(!isEMPTY(REFERENCE_ID))
					{
						FINDFLDGETSTRING(fineact,"PAY IDENTIFICATION",&refid);
					}
					if(!delflag && ((gy==GrantYear || GrantYear==(-1))) && !RDAstrcmp(source_mod,"PAYROLL") && RDAstrcmp(source_proc,"PAYVDST") && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)))
					{
						read_record=TRUE;
						FINDFLDGETSHORT(fineact,"PROCESSING MONTH",&pm);
						FINDFLDGETSTRING(fineact,"ACCOUNT CODE",&acctcode);
						if((pm==PM || Style==1) && !RDAstrcmp(acctcode,AccountCode))
						{
							acctype=0;
							if(SWB_FM25_AFFECT_DATE==TRUE)
							{
								FINDFLDGETSTRING(fineact,"AFFECT DATE",&adate);
							} else {
								FINDFLDGETSTRING(fineact,"ENTRY DATE",&adate);
							}
							memset(DQUERYstemp,0,101);
							sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
							if((isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
							{
								FINDFLDGETSTRING(fineact,"ACCOUNT CODE",&acctcode);
								FINDFLDGETSHORT(fineact,"TYPE",&type);
								id=stralloc(source_proc);
								FINDFLDGETSTRING(fineact,"ENTRY DATE",&edate);
								FINDFLDGETSTRING(fineact,"COMMENTS",&desc);
								FINDFLDGETINT(fineact,"TRANSACTION NUMBER",&trans);
								d=TRUE;
								FINDFLDGETSTRING(fineact,"SOURCE USER",&su);
								FINDFLDGETDOUBLE(fineact,"AMOUNT",&amt);
								cy=atoi(&edate[6]);
								deb=0.0;cred=0.0;
								if(type==0) deb=amt;
									else cred=amt;
								addsortrecord(dbsrt,24,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(fineact,2,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(fineact);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
				if(source_mod!=NULL) Rfree(source_mod);
				if(source_proc!=NULL) Rfree(source_proc);
			}
		} 
	}
	if(XPERT_SETUP->BLDPRMT && AccountType!=0 && GrantYear<=0)
	{
		gy=0;
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"BLDPRMT") && !RDAstrcmp(SOURCE_PROCESS,"RLZREV")))
		{
/* transactions */
		if(prmttran!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(prmttran))+RDAstrlen(FILENAME(prmttran))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(prmttran),FILENAME(prmttran));
			tmprsrc=diagscrn(prmttran,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(prmttran))+RDAstrlen(FILENAME(prmttran))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(prmttran),FILENAME(prmttran));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(prmttran);
			ef=ADVFRSNRDsec(prmttran,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(prmttran,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(prmttran,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(prmttran,"DELETEFLAG",&delflag);
				FINDFLDGETSTRING(prmttran,"SOURCE USER",&su);
				FINDFLDGETSHORT(prmttran,"DEFINITION TYPE",&deftype);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(prmttran,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(prmttran,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(prmttran,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETSTRING(prmttran,"PERMIT TYPE",&refid);
				}
				if((pm==PM || Style==1) && fy==FiscalYear && !delflag && DefinitionType==deftype && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
				FINDFLDGETSHORT(prmttran,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(prmttran,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(prmttran,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0))))) 
				{
					read_record=FALSE;
					FINDFLDGETCHAR(prmttran,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(prmttran,"REALIZE REVENUE",&realize_revenue);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2 && realize_revenue==TRUE) 
						{
							FINDFLDGETSTRING(prmttran,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_deferred_revenue==TRUE)
							{
								FINDFLDGETSTRING(prmttran,"RECEIVABLE CODE",&debcode);
								FINDFLDGETSTRING(prmttran,"DEFERRED REVENUE",&credcode);
							} else if(realize_revenue==TRUE)
							{
								FINDFLDGETSTRING(prmttran,"CASH CODE",&debcode);
								FINDFLDGETSTRING(prmttran,"REVENUE CODE",&credcode);
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(prmttran,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						ZERNRD(prmtmstr);
						COPYFIELD(prmttran,prmtmstr,"PERMIT IDENTIFICATION");
						if(ADVEQLNRDsec(prmtmstr,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(prmtmstr,1);
						FINDFLDGETSTRING(prmttran,"PERMIT IDENTIFICATION",&id);
						FINDFLDGETSTRING(prmttran,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(prmttran,"SOURCE USER",&su);
						FINDFLDGETSTRING(prmttran,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(prmtmstr,"PERMIT TYPE",&c);
						FINDFLDGETSTRING(prmttran,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(prmttran,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(prmttran,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,9,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,9,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,9,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,9,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							gy=0;
							deb=amt;
							addsortrecord(dbsrt,9,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							gy=0;
							cred=amt;
							addsortrecord(dbsrt,9,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(bankid!=NULL) Rfree(bankid);
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(prmttran,1,SCRNvirtualSubData,MAINRSRC);
			}
			ZERNRD(prmttran);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
/* payments */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"BLDPRMT") && !RDAstrcmp(SOURCE_PROCESS,"PRMTDIST")))
		{
		if(prmtpaid!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(prmtpaid))+RDAstrlen(FILENAME(prmtpaid))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(prmtpaid),FILENAME(prmtpaid));
			tmprsrc=diagscrn(prmtpaid,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(prmtpaid))+RDAstrlen(FILENAME(prmtpaid))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(prmtpaid),FILENAME(prmtpaid));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(prmtpaid);
			ef=ADVFRSNRDsec(prmtpaid,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(prmtpaid,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(prmtpaid,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(prmtpaid,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(prmtpaid,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(prmtpaid,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(prmtpaid,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(prmtpaid,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(prmtpaid,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(prmtpaid,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETSTRING(prmtpaid,"PERMIT TYPE",&refid);
				}
				if(DefinitionType==deftype && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(prmtpaid,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(prmtpaid,"REALIZE REVENUE",&realize_revenue);
					FINDFLDGETCHAR(prmtpaid,"USE CLEARING FUND",&use_clearing_fund);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(prmtpaid,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(prmtpaid,"DUE FROM CODE",&duefrom);
							} else {
								FINDFLDGETSTRING(prmtpaid,"DEBIT CODE",&debcode);
							}
							FINDFLDGETSTRING(prmtpaid,"CREDIT CODE",&credcode);
							FINDFLDGETSTRING(prmtpaid,"DEFERRED REVENUE",&deferred_code);
							FINDFLDGETSTRING(prmtpaid,"RECEIVABLE CODE",&receivable_code);
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(prmtpaid,"CLEARING CASH CODE",&clearing_cash);
								FINDFLDGETSTRING(prmtpaid,"CLEARING DUE TO CODE",&clearing_dueto);
								FINDFLDGETSTRING(prmtpaid,"DEBIT CODE",&debcode);
								FINDFLDGETSTRING(prmtpaid,"DUE FROM CODE",&duefrom);
							} else {
								clearing_cash=NULL;
								clearing_dueto=NULL;
								debcode=NULL;
								duefrom=NULL;
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)
								|| !RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(deferred_code,AccountCode) ||
								!RDAstrcmp(receivable_code,AccountCode) ||
								!RDAstrcmp(clearing_cash,AccountCode) ||
								!RDAstrcmp(clearing_dueto,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(prmtpaid,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						ZERNRD(prmtmstr);
						COPYFIELD(prmtpaid,prmtmstr,"PERMIT IDENTIFICATION");
						if(ADVEQLNRDsec(prmtmstr,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(prmtmstr,1);
						FINDFLDGETSTRING(prmtpaid,"PERMIT IDENTIFICATION",&id);
						FINDFLDGETSTRING(prmtpaid,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(prmtpaid,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(prmtpaid,"SOURCE USER",&su);
						FINDFLDGETSTRING(prmtpaid,"PERMIT TYPE",&c);
						FINDFLDGETSTRING(prmtpaid,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(prmtpaid,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(prmtpaid,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						FINDFLDGETSTRING(prmtpaid,"BANK IDENTIFICATION",&bankid);
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode))
						{
							gy=0;
							deb=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							cred=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							gy=0;
							deb=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							gy=0;
							cred=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(deferred_code,AccountCode)) 
						{
							gy=0;
							deb=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(receivable_code,AccountCode)) 
						{
							gy=0;
							cred=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}		
						if(AccountType==2 && !RDAstrcmp(clearing_cash,AccountCode)) 
						{
							gy=0;
							deb=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_dueto,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,10,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(prmtpaid,1,SCRNvirtualSubData,MAINRSRC);
			}
			ZERNRD(prmtpaid);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
	}
	if(XPERT_SETUP->OCCTAX && AccountType!=0 && GrantYear<=0)
	{
		gy=0;
/* receipts */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"OCCTAX") && !RDAstrcmp(SOURCE_PROCESS,"OCCRECTDIST")))
		{
		if(occrec!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(occrec))+RDAstrlen(FILENAME(occrec))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(occrec),FILENAME(occrec));
			tmprsrc=diagscrn(occrec,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(occrec))+RDAstrlen(FILENAME(occrec))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(occrec),FILENAME(occrec));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(occrec);
			ef=ADVFRSNRDsec(occrec,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(occrec,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(occrec,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(occrec,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(occrec,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(occrec,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(occrec,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(occrec,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(occrec,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(occrec,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETINT(occrec,"CHARGE YEAR",&cy);
					if(refid!=NULL) Rfree(refid);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%04d",cy);
					refid=stralloc(DQUERYstemp);
				}
				if(DefinitionType==deftype  && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(occrec,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(occrec,"REALIZE REVENUE",&realize_revenue);
					FINDFLDGETCHAR(occrec,"USE CLEARING FUND",&use_clearing_fund);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(occrec,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(occrec,"DUE FROM CODE",&duefrom);
							} else {
								FINDFLDGETSTRING(occrec,"DEBIT CODE",&debcode);
							}
							FINDFLDGETSTRING(occrec,"CREDIT CODE",&credcode);
							FINDFLDGETSTRING(occrec,"DEFERRED REVENUE CODE",&deferred_code);
							FINDFLDGETSTRING(occrec,"RECEIVABLE CODE",&receivable_code);
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(occrec,"CLEARING CASH CODE",&clearing_cash);
								FINDFLDGETSTRING(occrec,"CLEARING DUE TO CODE",&clearing_dueto);
								FINDFLDGETSTRING(occrec,"DEBIT CODE",&debcode);
								FINDFLDGETSTRING(occrec,"CLEARING DUE FROM CODE",&duefrom);
							} else {
								clearing_cash=NULL;
								clearing_dueto=NULL;
								debcode=NULL;
								duefrom=NULL;
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)
								|| !RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(deferred_code,AccountCode) ||
								!RDAstrcmp(receivable_code,AccountCode) ||
								!RDAstrcmp(clearing_cash,AccountCode) ||
								!RDAstrcmp(clearing_dueto,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(occrec,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						FINDFLDGETSTRING(occrec,"BANK IDENTIFICATION",&bankid);
						FINDFLDGETSTRING(occrec,"BUSINESS IDENTIFICATION",&id);
						FINDFLDGETSTRING(occrec,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(occrec,"DESCRIPTION",&desc);
						FINDFLDGETINT(occrec,"CHARGE YEAR",&cy);
						if(c!=NULL) Rfree(c);
						c=Rmalloc(5);
						sprintf(c,"%04d",cy);
						FINDFLDGETSTRING(occrec,"DEPOSIT IDENTIFICATION",&ref);
						FINDFLDGETSTRING(occrec,"SOURCE USER",&su);
						FINDFLDGETINT(occrec,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(occrec,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode))
						{
							deb=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							cred=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} 
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(deferred_code,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(receivable_code,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_cash,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_dueto,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,20,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(occrec,1,SCRNvirtualSubData,MAINRSRC);
			}

			ZERNRD(occrec);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
	}
	if(XPERT_SETUP->RLSTMGT && AccountType!=0 && GrantYear<=0)
	{
		gy=0;
/* transactions */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"RLSTMGT") && !RDAstrcmp(SOURCE_PROCESS,"RLZREV")))
		{
		if(restran!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(restran))+RDAstrlen(FILENAME(restran))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(restran),FILENAME(restran));
			tmprsrc=diagscrn(restran,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(restran))+RDAstrlen(FILENAME(restran))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(restran),FILENAME(restran));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(restran);
			ef=ADVFRSNRDsec(restran,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(restran,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(restran,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(restran,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(restran,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(restran,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(restran,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(restran,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(restran,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(restran,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETINT(restran,"TAX YEAR",&cy);
					if(refid!=NULL) Rfree(refid);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%04d",cy);
					refid=stralloc(DQUERYstemp);
				}
				if(DefinitionType==deftype  && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(restran,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(restran,"REALIZE REVENUE",&realize_revenue);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2 && realize_revenue==TRUE) 
						{
							FINDFLDGETSTRING(restran,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_deferred_revenue==TRUE)
							{
								FINDFLDGETSTRING(restran,"RECEIVABLE CODE",&debcode);
								FINDFLDGETSTRING(restran,"DEFERRED REVENUE",&credcode);
							} else if(realize_revenue==TRUE)
							{
								FINDFLDGETSTRING(restran,"CASH CODE",&debcode);
								FINDFLDGETSTRING(restran,"REVENUE CODE",&credcode);
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(restran,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						ZERNRD(restmst);
						COPYFIELD(restran,restmst,"OWNER IDENTIFICATION");
						COPYFIELD(restran,restmst,"REAL ESTATE IDENTIFICATION");
						COPYFIELD(restran,restmst,"TAX YEAR");
						if(ADVEQLNRDsec(restmst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(restmst,1);
						FINDFLDGETSTRING(restran,"REAL ESTATE IDENTIFICATION",&id);
						FINDFLDGETSTRING(restran,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(restran,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(restran,"SOURCE USER",&su);
						FINDFLDGETSTRING(restran,"CLASS IDENTIFICATION",&c);
						FINDFLDGETSTRING(restran,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(restran,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(restran,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,11,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,11,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,11,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,11,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,11,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,11,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(bankid!=NULL) Rfree(bankid);
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(restran,1,SCRNvirtualSubData,MAINRSRC);
			}
			ZERNRD(restran);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
/* payments */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"RLSTMGT") && !RDAstrcmp(SOURCE_PROCESS,"RLSTDIST")))
		{
		if(restpay!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(restpay))+RDAstrlen(FILENAME(restpay))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(restpay),FILENAME(restpay));
			tmprsrc=diagscrn(restpay,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(restpay))+RDAstrlen(FILENAME(restpay))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(restpay),FILENAME(restpay));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(restpay);
			ef=ADVFRSNRDsec(restpay,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(restpay,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(restpay,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(restpay,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(restpay,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(restpay,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(restpay,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(restpay,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(restpay,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(restpay,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETINT(restpay,"TAX YEAR",&cy);
					if(refid!=NULL) Rfree(refid);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%04d",cy);
					refid=stralloc(DQUERYstemp);
				}
				if(DefinitionType==deftype  && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(restpay,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(restpay,"USE CLEARING FUND",&use_clearing_fund);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(restpay,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(restpay,"DUE FROM CODE",&duefrom);
							} else {
								FINDFLDGETSTRING(restpay,"DEBIT CODE",&debcode);
							}
							FINDFLDGETSTRING(restpay,"CREDIT CODE",&credcode);
							FINDFLDGETSTRING(restpay,"DEFERRED REVENUE",&deferred_code);
							FINDFLDGETSTRING(restpay,"RECEIVABLE CODE",&receivable_code);
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(restpay,"CLEARING CASH CODE",&clearing_cash);
								FINDFLDGETSTRING(restpay,"CLEARING DUE TO CODE",&clearing_dueto);
								FINDFLDGETSTRING(restpay,"DEBIT CODE",&debcode);
								FINDFLDGETSTRING(restpay,"DUE FROM CODE",&duefrom);
							} else {
								clearing_cash=NULL;
								clearing_dueto=NULL;
								debcode=NULL;
								duefrom=NULL;
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)
								|| !RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(deferred_code,AccountCode) ||
								!RDAstrcmp(receivable_code,AccountCode) ||
								!RDAstrcmp(clearing_cash,AccountCode) ||
								!RDAstrcmp(clearing_dueto,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(restpay,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						ZERNRD(restmst);
						COPYFIELD(restpay,restmst,"OWNER IDENTIFICATION");
						COPYFIELD(restpay,restmst,"REAL ESTATE IDENTIFICATION");
						COPYFIELD(restpay,restmst,"TAX YEAR");
						if(ADVEQLNRDsec(restmst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(restmst,1);
						FINDFLDGETSTRING(restpay,"REAL ESTATE IDENTIFICATION",&id);
						FINDFLDGETSTRING(restpay,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(restpay,"SOURCE USER",&su);
						FINDFLDGETSTRING(restpay,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(restmst,"CLASS IDENTIFICATION",&c);
						FINDFLDGETSTRING(restpay,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(restpay,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(restpay,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						FINDFLDGETSTRING(restpay,"BANK IDENTIFICATION",&bankid);
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode))
						{
							deb=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							cred=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(deferred_code,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(receivable_code,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_cash,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_dueto,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,12,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(restpay,1,SCRNvirtualSubData,MAINRSRC);
			}

			ZERNRD(restpay);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
	}
	if(XPERT_SETUP->PROPERTY && AccountType!=0 && GrantYear<=0)
	{
		gy=0;
/* transactions */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"PROPERTY") && !RDAstrcmp(SOURCE_PROCESS,"RLZREV")))
		{
		if(prptrn!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(prptrn))+RDAstrlen(FILENAME(prptrn))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(prptrn),FILENAME(prptrn));
			tmprsrc=diagscrn(prptrn,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(prptrn))+RDAstrlen(FILENAME(prptrn))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(prptrn),FILENAME(prptrn));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(prptrn);

			ef=ADVFRSNRDsec(prptrn,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(prptrn,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(prptrn,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(prptrn,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(prptrn,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(prptrn,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(prptrn,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(prptrn,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(prptrn,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(prptrn,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETINT(prptrn,"TAX YEAR",&cy);
					if(refid!=NULL) Rfree(refid);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%04d",cy);
					refid=stralloc(DQUERYstemp);
				}
				if(DefinitionType==deftype && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(prptrn,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(prptrn,"REALIZE REVENUE",&realize_revenue);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2 && realize_revenue==TRUE) 
						{
							FINDFLDGETSTRING(prptrn,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_deferred_revenue==TRUE)
							{
								FINDFLDGETSTRING(prptrn,"RECEIVABLE CODE",&debcode);
								FINDFLDGETSTRING(prptrn,"DEFERRED REVENUE",&credcode);
							} else if(realize_revenue==TRUE)
							{
								FINDFLDGETSTRING(prptrn,"CASH CODE",&debcode);
								FINDFLDGETSTRING(prptrn,"REVENUE CODE",&credcode);
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(prptrn,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						ZERNRD(prpmst);
						COPYFIELD(prptrn,prpmst,"OWNER IDENTIFICATION");
						COPYFIELD(prptrn,prpmst,"REAL ESTATE IDENTIFICATION");
						COPYFIELD(prptrn,prpmst,"TAX YEAR");
						if(ADVEQLNRDsec(prpmst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(prpmst,1);
						FINDFLDGETSTRING(prptrn,"OWNER IDENTIFICATION",&id);
						FINDFLDGETSTRING(prptrn,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(prptrn,"SOURCE USER",&su);
						FINDFLDGETSTRING(prptrn,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(prptrn,"CLASS IDENTIFICATION",&c);
						FINDFLDGETSTRING(prptrn,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(prptrn,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(prptrn,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,13,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,13,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,13,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,13,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,13,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,13,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(bankid!=NULL) Rfree(bankid);
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(prptrn,1,SCRNvirtualSubData,MAINRSRC);
			}
			ZERNRD(prptrn);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
/* payments */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"PROPERTY") && !RDAstrcmp(SOURCE_PROCESS,"PRPDIST")))
		{
		if(prppay!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(prppay))+RDAstrlen(FILENAME(prppay))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(prppay),FILENAME(prppay));
			tmprsrc=diagscrn(prppay,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(prppay))+RDAstrlen(FILENAME(prppay))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(prppay),FILENAME(prppay));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(prppay);
			ef=ADVFRSNRDsec(prppay,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(prppay,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(prppay,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(prppay,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(prppay,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(prppay,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(prppay,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(prppay,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(prppay,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(prppay,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					FINDFLDGETINT(prppay,"TAX YEAR",&cy);
					if(refid!=NULL) Rfree(refid);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%04d",cy);
					refid=stralloc(DQUERYstemp);
				}
				if(DefinitionType==deftype && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(prppay,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(prppay,"USE CLEARING FUND",&use_clearing_fund);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(prppay,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(prppay,"DUE FROM CODE",&duefrom);
							} else {
								FINDFLDGETSTRING(prppay,"DEBIT CODE",&debcode);
							}
							FINDFLDGETSTRING(prppay,"CREDIT CODE",&credcode);
							FINDFLDGETSTRING(prppay,"DEFERRED REVENUE",&deferred_code);
							FINDFLDGETSTRING(prppay,"RECEIVABLE CODE",&receivable_code);
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(prppay,"CLEARING CASH CODE",&clearing_cash);
								FINDFLDGETSTRING(prppay,"CLEARING DUE TO CODE",&clearing_dueto);
								FINDFLDGETSTRING(prppay,"DEBIT CODE",&debcode);
								FINDFLDGETSTRING(prppay,"DUE FROM CODE",&duefrom);
							} else {
								clearing_cash=NULL;
								clearing_dueto=NULL;
								debcode=NULL;
								duefrom=NULL;
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)
								|| !RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(deferred_code,AccountCode) ||
								!RDAstrcmp(receivable_code,AccountCode) ||
								!RDAstrcmp(clearing_cash,AccountCode) ||
								!RDAstrcmp(clearing_dueto,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(prppay,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						ZERNRD(prpmst);
						COPYFIELD(prppay,prpmst,"OWNER IDENTIFICATION");
						COPYFIELD(prppay,prpmst,"REAL ESTATE IDENTIFICATION");
						COPYFIELD(prppay,prpmst,"TAX YEAR");
						if(ADVEQLNRDsec(prpmst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(prpmst,1);
						FINDFLDGETSTRING(prppay,"OWNER IDENTIFICATION",&id);
						FINDFLDGETSTRING(prppay,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(prppay,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(prppay,"SOURCE USER",&su);
						FINDFLDGETSTRING(prppay,"CLASS IDENTIFICATION",&c);
						FINDFLDGETSTRING(prppay,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(prppay,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(prppay,"AMOUNT",&amt);
						FINDFLDGETSTRING(prppay,"BANK IDENTIFICATION",&bankid);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode))
						{
							deb=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							cred=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} 
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(deferred_code,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(receivable_code,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_cash,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_dueto,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,14,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(prppay,1,SCRNvirtualSubData,MAINRSRC);
			}

			ZERNRD(prppay);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
	}
	if(XPERT_SETUP->MISCBILL && AccountType!=0 && GrantYear<=0)
	{
		gy=0;
/* transactions */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"MISCBILL") && !RDAstrcmp(SOURCE_PROCESS,"RLZREV")))
		{
		if(misctran!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(misctran))+RDAstrlen(FILENAME(misctran))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(misctran),FILENAME(misctran));
			tmprsrc=diagscrn(misctran,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(misctran))+RDAstrlen(FILENAME(misctran))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(misctran),FILENAME(misctran));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(misctran);

			ef=ADVFRSNRDsec(misctran,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(misctran,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(misctran,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(misctran,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(misctran,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(misctran,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(misctran,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(misctran,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(misctran,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(misctran,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					if(refid!=NULL) Rfree(refid);
					FINDFLDGETINT(misctran,"TAX YEAR",&cy);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%04d",cy);
					refid=stralloc(DQUERYstemp);
				}
				if(DefinitionType==deftype  && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(misctran,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(misctran,"REALIZE REVENUE",&realize_revenue);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2 && realize_revenue==TRUE) 
						{
							FINDFLDGETSTRING(misctran,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_deferred_revenue==TRUE)
							{
								FINDFLDGETSTRING(misctran,"RECEIVABLE CODE",&debcode);
								FINDFLDGETSTRING(misctran,"DEFERRED REVENUE",&credcode);
							} else if(realize_revenue==TRUE)
							{
								FINDFLDGETSTRING(misctran,"CASH CODE",&debcode);
								FINDFLDGETSTRING(misctran,"REVENUE CODE",&credcode);
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(misctran,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						FINDFLDGETSTRING(misctran,"OWNER IDENTIFICATION",&id);
						FINDFLDGETSTRING(misctran,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(misctran,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(misctran,"CLASS IDENTIFICATION",&c);
						FINDFLDGETSTRING(misctran,"SOURCE USER",&su);
						FINDFLDGETSTRING(misctran,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(misctran,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(misctran,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,15,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,15,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,15,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{
							deb=amt*(-1);
							addsortrecord(dbsrt,15,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,15,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,15,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(bankid!=NULL) Rfree(bankid);
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(misctran,1,SCRNvirtualSubData,MAINRSRC);
			}
			ZERNRD(misctran);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
/* payments */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"MISCBILL") && !RDAstrcmp(SOURCE_PROCESS,"MISCDIST")))
		{
		if(miscpaid!=(-1))
		{
			e=Rmalloc(RDAstrlen(MODULENAME(miscpaid))+RDAstrlen(FILENAME(miscpaid))+65);
			sprintf(e,"Gathering from Module %s File %s",MODULENAME(miscpaid),FILENAME(miscpaid));
			tmprsrc=diagscrn(miscpaid,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
			if(e!=NULL) Rfree(e);
			e=NULL;
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				} else {
					e=Rmalloc(RDAstrlen(MODULENAME(miscpaid))+RDAstrlen(FILENAME(miscpaid))+15);
					sprintf(e,"%s %s QUERYING",MODULENAME(miscpaid),FILENAME(miscpaid));
					SETTITLE(tmprsrc,e);
					if(e!=NULL) Rfree(e);
					ForceWindowUpdate(tmprsrc);
				}
			}
			ZERNRD(miscpaid);
			ef=ADVFRSNRDsec(miscpaid,1,SCRNvirtualSubData,MAINRSRC);
			while(!ef)
			{
				ForceWindowUpdate(tmprsrc);
				FINDFLDGETINT(miscpaid,"FISCAL YEAR",&fy);
				FINDFLDGETSHORT(miscpaid,"FISCAL MONTH",&pm);
				FINDFLDGETCHAR(miscpaid,"DELETEFLAG",&delflag);
				if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
				{
				FINDFLDGETSHORT(miscpaid,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(miscpaid,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(miscpaid,"DISTRIBUTED DATE",&adate);
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DISTRIBUTED_DATE!=NULL)
				{
					FINDFLDGETSTRING(miscpaid,"DISTRIBUTED DATE",&d_date);
					FINDFLDGETSTRING(miscpaid,"DISTRIBUTED TIME",&d_time);
					FINDFLDGETSTRING(miscpaid,"DISTRIBUTED BY",&su);
				}
				if(!isEMPTY(REFERENCE_ID))
				{
					if(refid!=NULL) Rfree(refid);
					FINDFLDGETINT(miscpaid,"TAX YEAR",&cy);
					memset(DQUERYstemp,0,101);
					sprintf(DQUERYstemp,"%04d",cy);
					refid=stralloc(DQUERYstemp);
				}
				if(DefinitionType==deftype  && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
				{
					read_record=FALSE;
					FINDFLDGETCHAR(miscpaid,"USE DEFERRED REVENUE",&use_deferred_revenue);
					FINDFLDGETCHAR(miscpaid,"USE CLEARING FUND",&use_clearing_fund);
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
						{
							FINDFLDGETSTRING(miscpaid,"ACCOUNT CODE",&acctcode);
							if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
						}
						if(AccountType==2)
						{
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(miscpaid,"DUE FROM CODE",&duefrom);
							} else {
								FINDFLDGETSTRING(miscpaid,"DEBIT CODE",&debcode);
							}
							FINDFLDGETSTRING(miscpaid,"CREDIT CODE",&credcode);
							FINDFLDGETSTRING(miscpaid,"DEFERRED REVENUE",&deferred_code);
							FINDFLDGETSTRING(miscpaid,"RECEIVABLE CODE",&receivable_code);
							if(use_clearing_fund==TRUE)
							{
								FINDFLDGETSTRING(miscpaid,"CLEARING CASH CODE",&clearing_cash);
								FINDFLDGETSTRING(miscpaid,"CLEARING DUE TO CODE",&clearing_dueto);
								FINDFLDGETSTRING(miscpaid,"DEBIT CODE",&debcode);
								FINDFLDGETSTRING(miscpaid,"DUE FROM CODE",&duefrom);
							} else {
								clearing_cash=NULL;
								clearing_dueto=NULL;
								debcode=NULL;
								duefrom=NULL;
							}
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)
								|| !RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(duefrom,AccountCode) || 
								!RDAstrcmp(deferred_code,AccountCode) ||
								!RDAstrcmp(receivable_code,AccountCode) ||
								!RDAstrcmp(clearing_cash,AccountCode) ||
								!RDAstrcmp(clearing_dueto,AccountCode)) read_record=TRUE;
						}
					}
					FINDFLDGETCHAR(miscpaid,"DISTRIBUTED",&d);
					if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
					{
						FINDFLDGETSTRING(miscpaid,"OWNER IDENTIFICATION",&id);
						FINDFLDGETSTRING(miscpaid,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(miscpaid,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(miscpaid,"SOURCE USER",&su);
						FINDFLDGETSTRING(miscpaid,"CLASS IDENTIFICATION",&c);
						FINDFLDGETSTRING(miscpaid,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(miscpaid,"TRANSACTION NUMBER",&trans);
						FINDFLDGETDOUBLE(miscpaid,"AMOUNT",&amt);
						FINDFLDGETSTRING(miscpaid,"BANK IDENTIFICATION",&bankid);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) 
						{
							deb=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==0) 
						{
							cred=amt*(-1);
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==1 && amt>=0.0) 
						{
							cred=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						} else if(AccountType==1) 
						{	
							deb=amt*(-1);
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode))
						{
							deb=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
							cred=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(deferred_code,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(receivable_code,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_cash,AccountCode)) 
						{
							deb=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
						if(AccountType==2 && !RDAstrcmp(clearing_dueto,AccountCode)) 
						{
							cred=amt;
							addsortrecord(dbsrt,16,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
						}
					}
				}
				}
				if(!abort_diagnostic)
				{
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(miscpaid,1,SCRNvirtualSubData,MAINRSRC);
			}
			ZERNRD(miscpaid);
			if(tmprsrc!=NULL) 
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		}
	}
	if(XPERT_SETUP->UTLBLL && AccountType!=0 && GrantYear<=0)
	{
		gy=0;
/* Deposits */
/* Receipts */
/* Transactions */
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"UTLBLL") && !RDAstrcmp(SOURCE_PROCESS,"UTBDIST")))
		{
			if(utbtrn!=(-1))
			{
				e=Rmalloc(RDAstrlen(MODULENAME(utbtrn))+RDAstrlen(FILENAME(utbtrn))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(utbtrn),FILENAME(utbtrn));
				tmprsrc=diagscrn(utbtrn,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(utbtrn))+RDAstrlen(FILENAME(utbtrn))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(utbtrn),FILENAME(utbtrn));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				ZERNRD(utbtrn);
				ef=ADVFRSNRDsec(utbtrn,1,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					FINDFLDGETINT(utbtrn,"FISCAL YEAR",&fy);
					FINDFLDGETSHORT(utbtrn,"FISCAL MONTH",&pm);
					FINDFLDGETCHAR(utbtrn,"DELETEFLAG",&delflag);
					if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
					{
						FINDFLDGETSHORT(utbtrn,"ACCOUNT TYPE",&acctype);
						FINDFLDGETSHORT(utbtrn,"DEFINITION TYPE",&deftype);
						FINDFLDGETSTRING(utbtrn,"DISTRIBUTED DATE",&adate);
						memset(DQUERYstemp,0,101);
						sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
						if(DISTRIBUTED_DATE!=NULL)
						{
							FINDFLDGETSTRING(utbtrn,"DISTRIBUTED DATE",&d_date);
							FINDFLDGETSTRING(utbtrn,"DISTRIBUTED TIME",&d_time);
							FINDFLDGETSTRING(utbtrn,"DISTRIBUTED BY",&su);
						}
						if(DefinitionType==deftype  && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
						{
							read_record=FALSE;
							FINDFLDGETCHAR(utbtrn,"USE CLEARING FUND",&use_clearing_fund);
							if((AccountType==2) || (acctype==AccountType)) 
							{
								if(AccountType<2) 
								{
									FINDFLDGETSTRING(utbtrn,"ACCOUNT CODE",&acctcode);
									if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
								}
								if(AccountType==2)
								{
									FINDFLDGETSTRING(utbtrn,"DEBIT CODE",&debcode);
									FINDFLDGETSTRING(utbtrn,"CREDIT CODE",&credcode);
									if(use_clearing_fund==TRUE)
									{
										FINDFLDGETSTRING(utbtrn,"CLEARING CASH CODE",&clearing_cash);
										FINDFLDGETSTRING(utbtrn,"CLEARING DUE TO CODE",&clearing_dueto);
										FINDFLDGETSTRING(miscpaid,"DUE FROM CODE",&duefrom);
									} else {
										clearing_cash=NULL;
										clearing_dueto=NULL;
									}
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)
										|| !RDAstrcmp(duefrom,AccountCode) || 
										!RDAstrcmp(clearing_cash,AccountCode) ||
										!RDAstrcmp(clearing_dueto,AccountCode)) read_record=TRUE;
								}
							}
							FINDFLDGETCHAR(utbtrn,"DISTRIBUTED",&d);
							if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
							{
								ZERNRD(utbsvc);
								COPYFIELD(utbtrn,utbsvc,"CUSTOMER IDENTIFICATION");
								COPYFIELD(utbtrn,utbsvc,"SERVICE NUMBER");
								if(ADVEQLNRDsec(utbsvc,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(utbsvc,1);
								ZERNRD(utbcds);
								COPYFIELD(utbsvc,utbcds,"SERVICE IDENTIFICATION");
								COPYFIELD(utbtrn,utbcds,"CHARGE NUMBER");
								if(ADVEQLNRDsec(utbcds,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(utbcds,1);
								FINDFLDGETSTRING(utbtrn,"CUSTOMER IDENTIFICATION",&id);
								FINDFLDGETINT(utbtrn,"CHARGE NUMBER",&cy);
								if(c!=NULL) Rfree(c);
								c=Rmalloc(5);
								sprintf(c,"%04d",cy);
								FINDFLDGETSTRING(utbtrn,"ENTRY DATE",&edate);
								FINDFLDGETSTRING(utbcds,"SHORT DESCRIPTION",&desc);
								FINDFLDGETSTRING(utbsvc,"SERVICE IDENTIFICATION",&ref);
								FINDFLDGETSTRING(utbtrn,"SOURCE USER",&su);
								FINDFLDGETINT(utbtrn,"TRANSACTION NUMBER",&trans);
								FINDFLDGETDOUBLE(utbtrn,"AMOUNT",&amt);
								FINDFLDGETSHORT(utbtrn,"TYPE",&type);
								cy=atoi(&edate[6]);
								deb=0.0;cred=0.0;
								if(AccountType==0 && amt>=0.0) 
								{
									deb=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								} else if(AccountType==0) 
								{
									cred=amt*(-1);
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								}
								if(AccountType==1 && amt>=0.0) 
								{
									cred=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								} else if(AccountType==1) 
								{
									deb=amt*(-1);
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								}
								if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode))
								{
									deb=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
									cred=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								} 
								if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
								{
									deb=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								}
								if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
								{
									cred=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								}
								if(AccountType==2 && !RDAstrcmp(clearing_cash,AccountCode)) 
								{
									deb=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								}
								if(AccountType==2 && !RDAstrcmp(clearing_dueto,AccountCode)) 
								{	
									cred=amt;
									if(trans==0) /* Deposits */
									{
										FINDFLDGETSTRING(utbtrn,"DEPOSIT ID",&c);
										addsortrecord(dbsrt,17,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else if(type==0)
									{
										FINDFLDGETSTRING(utbtrn,"RECEIPT IDENTIFICATION",&c);
										addsortrecord(dbsrt,18,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
									} else {
										addsortrecord(dbsrt,19,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,NULL);
									}
								}
								if(bankid!=NULL) Rfree(bankid);
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(utbtrn,1,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(utbtrn);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
			}
		}
	}
	if(XPERT_SETUP->INVCTL && GrantYear<=0)
	{
		gy=0;
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"INVCTL") && !RDAstrcmp(SOURCE_PROCESS,"ISSDIST")))
		{
			if(invisst!=(-1) && invacc!=(-1))
			{
				e=Rmalloc(RDAstrlen(MODULENAME(invacc))+RDAstrlen(FILENAME(invacc))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(invacc),FILENAME(invacc));
				tmprsrc=diagscrn(invacc,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(invacc))+RDAstrlen(FILENAME(invacc))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(invacc),FILENAME(invacc));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				ZERNRD(invacc);
				if(desc!=NULL) Rfree(desc);
				ef=ADVFRSNRDsec(invacc,1,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					ZERNRD(invisst);
					COPYFIELD(invacc,invisst,"ISSUE IDENTIFICATION");
					COPYFIELD(invacc,invisst,"TRANSACTION NUMBER");
					if(ADVEQLNRDsec(invisst,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(invisst,1);
					FINDFLDGETINT(invisst,"FISCAL YEAR",&fy);
					FINDFLDGETSHORT(invisst,"FISCAL MONTH",&pm);
					FINDFLDGETCHAR(invacc,"DELETEFLAG",&delflag);
					if(fy==FiscalYear && (((pm+1)==PM) || Style==1) && !delflag)
					{
						FINDFLDGETSHORT(invacc,"ACCOUNT TYPE",&acctype);
						FINDFLDGETSHORT(invacc,"DEFINITION TYPE",&deftype);
						FINDFLDGETSTRING(invisst,"ENTRY DATE",&adate);
						memset(DQUERYstemp,0,101);
						sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
						if(DISTRIBUTED_DATE!=NULL)
						{
							FINDFLDGETSTRING(invisst,"DISTRIBUTED DATE",&d_date);
							FINDFLDGETSTRING(invisst,"DISTRIBUTED TIME",&d_time);
							FINDFLDGETSTRING(invisst,"DISTRIBUTED BY",&su);
						}
						if(!isEMPTY(REFERENCE_ID))
						{
							FINDFLDGETSTRING(invisst,"ISSUE IDENTIFICATION",&refid);
						}
						if(DefinitionType==deftype && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
						{
							read_record=FALSE;
							if((AccountType==2) || (acctype==AccountType)) 
							{
								if(AccountType<2) 
								{
									FINDFLDGETSTRING(invacc,"ACCOUNT CODE",&acctcode);
									if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
								}
								if(AccountType==2)
								{
									FINDFLDGETSTRING(invacc,"DEBIT CODE",&debcode);
									FINDFLDGETSTRING(invacc,"CREDIT CODE",&credcode);
									FINDFLDGETSTRING(invacc,"DUE FROM",&duefrom);
									FINDFLDGETSTRING(invacc,"INVENTORY CODE",&payable);
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(payable,AccountCode)) read_record=TRUE;
								}
							}
							FINDFLDGETCHAR(invisst,"DISTRIBUTED",&d);
							if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
							{
								FINDFLDGETSTRING(invisst,"ITEM IDENTIFICATION",&id);
								FINDFLDGETSTRING(invisst,"ENTRY DATE",&edate);
								FINDFLDGETSTRING(invisst,"DESCRIPTION",&desc);
								FINDFLDGETSTRING(invisst,"LOCATION IDENTIFICATION",&c);
								FINDFLDGETSTRING(invisst,"SOURCE USER",&su);
								FINDFLDGETSTRING(invisst,"ISSUE IDENTIFICATION",&ref);
								FINDFLDGETINT(invacc,"TRANSACTION NUMBER",&trans);
								FINDFLDGETDOUBLE(invacc,"AMOUNT",&amt);
								cy=atoi(&edate[6]);
								deb=0.0;cred=0.0;
								if(AccountType==0 && amt>=0.0) 
								{
									deb=amt;
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								} else if(AccountType==0) 
								{
									cred=amt*(-1);
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==1 && amt>=0.0) 
								{
									cred=amt;
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								} else if(AccountType==1) 
								{
									deb=amt*(-1);
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
								{
									deb=amt;
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
								{
									cred=amt;
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(duefrom,AccountCode)) 
								{
									deb=amt;
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(payable,AccountCode)) 
								{
									cred=amt;
									addsortrecord(dbsrt,22,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(bankid!=NULL) Rfree(bankid);
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(invacc,1,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(invacc);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
			}
		}
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"INVCTL") && !RDAstrcmp(SOURCE_PROCESS,"ADJDIST")))
		{
			if(invadj!=(-1))
			{
				e=Rmalloc(RDAstrlen(MODULENAME(invadj))+RDAstrlen(FILENAME(invadj))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(invadj),FILENAME(invadj));
				tmprsrc=diagscrn(invadj,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(invadj))+RDAstrlen(FILENAME(invadj))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(invadj),FILENAME(invadj));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				ZERNRD(invadj);
				if(desc!=NULL) Rfree(desc);
				ef=ADVFRSNRDsec(invadj,1,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					FINDFLDGETINT(invadj,"FISCAL YEAR",&fy);
					FINDFLDGETSHORT(invadj,"FISCAL MONTH",&pm);
					FINDFLDGETCHAR(invadj,"DELETEFLAG",&delflag);
					if(fy==FiscalYear && (((pm+1)==PM) || Style==1) && !delflag)
					{
						FINDFLDGETSHORT(invadj,"ACCOUNT TYPE",&acctype);
						FINDFLDGETSHORT(invadj,"DEFINITION TYPE",&deftype);
						FINDFLDGETSTRING(invadj,"ENTRY DATE",&adate);
						memset(DQUERYstemp,0,101);
						sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
						if(DISTRIBUTED_DATE!=NULL)
						{
							FINDFLDGETSTRING(invadj,"DISTRIBUTED DATE",&d_date);
							FINDFLDGETSTRING(invadj,"DISTRIBUTED TIME",&d_time);
							FINDFLDGETSTRING(invadj,"DISTRIBUTED BY",&su);
						}
						if(DefinitionType==deftype  && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
						{
							read_record=FALSE;
							if((AccountType==2) || (acctype==AccountType)) 
							{
								if(AccountType<2) 
								{
									FINDFLDGETSTRING(invadj,"ACCOUNT CODE",&acctcode);
									if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
								}
								if(AccountType==2)
								{
									FINDFLDGETSTRING(invadj,"DEBIT CODE",&debcode);
									FINDFLDGETSTRING(invadj,"CREDIT CODE",&credcode);
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
								}
							}
							FINDFLDGETCHAR(invadj,"DISTRIBUTED",&d);
							if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
							{
								FINDFLDGETSTRING(invadj,"ITEM IDENTIFICATION",&id);
								FINDFLDGETSTRING(invadj,"ENTRY DATE",&edate);
								FINDFLDGETSTRING(invadj,"SOURCE USER",&su);
								FINDFLDGETSTRING(invadj,"DESCRIPTION",&desc);
								FINDFLDGETSTRING(invadj,"LOCATION IDENTIFICATION",&c);
								FINDFLDGETSTRING(invadj,"CATEGORY IDENTIFICATION",&ref);
								FINDFLDGETINT(invadj,"TRANSACTION NUMBER",&trans);
								FINDFLDGETDOUBLE(invadj,"VALUE",&amt);
								cy=atoi(&edate[6]);
								deb=0.0;cred=0.0;
								if(AccountType==0 && amt>=0.0) 
								{
									deb=amt;
									addsortrecord(dbsrt,23,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								} else if(AccountType==0) 
								{
									cred=amt*(-1);
									addsortrecord(dbsrt,23,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==1 && amt>=0.0) 
								{
									cred=amt;
									addsortrecord(dbsrt,23,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								} else if(AccountType==1) 
								{
									deb=amt*(-1);
									addsortrecord(dbsrt,23,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
								{
									deb=amt;
									addsortrecord(dbsrt,23,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
								{
									cred=amt;
									addsortrecord(dbsrt,23,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(bankid!=NULL) Rfree(bankid);
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(invadj,1,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(invadj);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
			}
		}
	}
	if(XPERT_SETUP->FIXASS && GrantYear<=0)
	{
		gy=0;
		if((isEMPTY(SOURCE_MODULE) && isEMPTY(SOURCE_PROCESS)) || (!RDAstrcmp(SOURCE_MODULE,"FIXASS") && !RDAstrcmp(SOURCE_PROCESS,"FIXDIST")))
		{
			if(fixtran!=(-1))
			{
				e=Rmalloc(RDAstrlen(MODULENAME(fixtran))+RDAstrlen(FILENAME(fixtran))+65);
				sprintf(e,"Gathering from Module %s File %s",MODULENAME(fixtran),FILENAME(fixtran));
				tmprsrc=diagscrn(fixtran,"DIAGNOSTIC SCREEN","FINMGT",e,stop_diagnostic);
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(ADVmakescrn(tmprsrc,TRUE))
					{
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
						WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
						if(e!=NULL) Rfree(e);
					} else {
						e=Rmalloc(RDAstrlen(MODULENAME(fixtran))+RDAstrlen(FILENAME(fixtran))+15);
						sprintf(e,"%s %s QUERYING",MODULENAME(fixtran),FILENAME(fixtran));
						SETTITLE(tmprsrc,e);
						if(e!=NULL) Rfree(e);
						ForceWindowUpdate(tmprsrc);
					}
				}
				ZERNRD(fixtran);
				ef=ADVFRSNRDsec(fixtran,1,SCRNvirtualSubData,MAINRSRC);
				while(!ef)
				{
					ForceWindowUpdate(tmprsrc);
					FINDFLDGETINT(fixtran,"FISCAL YEAR",&fy);
					FINDFLDGETSHORT(fixtran,"FISCAL MONTH",&pm);
					FINDFLDGETCHAR(fixtran,"DELETEFLAG",&delflag);
					if(fy==FiscalYear && (pm==PM || Style==1) && !delflag)
					{
						FINDFLDGETSHORT(fixtran,"ACCOUNT TYPE",&acctype);
						FINDFLDGETSHORT(fixtran,"DEFINITION TYPE",&deftype);
						FINDFLDGETSTRING(fixtran,"DISTRIBUTED DATE",&adate);
						memset(DQUERYstemp,0,101);
						sprintf(DQUERYstemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
						if(DISTRIBUTED_DATE!=NULL)
						{
							FINDFLDGETSTRING(fixtran,"DISTRIBUTED DATE",&d_date);
							FINDFLDGETSTRING(fixtran,"DISTRIBUTED TIME",&d_time);
							FINDFLDGETSTRING(fixtran,"DISTRIBUTED BY",&su);
						}
						if(!isEMPTY(REFERENCE_ID))
						{
							if(refid!=NULL) Rfree(refid);
							FINDFLDGETSHORT(fixtran,"TRANSACTION TYPE",&type);
							memset(DQUERYstemp,0,101);
							sprintf(DQUERYstemp,"%d",type);
							refid=stralloc(DQUERYstemp);
						}
						if(DefinitionType==deftype && (isEMPTY(REFERENCE_ID) || !RDAstrcmp(REFERENCE_ID,refid)) && (isEMPTY(adate) || (((RDAstrcmp(DQUERYstemp,date_affected))<=0) && (isEMPTY(from_date_affected) || ((RDAstrcmp(from_date_affected,DQUERYstemp))<=0)))) && (isEMPTY(SOURCE_USER) || !RDAstrcmp(SOURCE_USER,su)) && (isEMPTY(DISTRIBUTED_DATE) || !RDAstrcmp(DISTRIBUTED_DATE,d_date)) && (isEMPTY(DISTRIBUTED_TIME) || TimeComparison(d_time)))
						{
							read_record=FALSE;
							if((AccountType==2) || (acctype==AccountType)) 
							{
								if(AccountType<2) 
								{
									FINDFLDGETSTRING(fixtran,"ACCOUNT CODE",&acctcode);
									if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
								}
								if(AccountType==2)
								{
									FINDFLDGETSTRING(fixtran,"DEBIT CODE",&debcode);
									FINDFLDGETSTRING(fixtran,"CREDIT CODE",&credcode);
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
								}
							}
							FINDFLDGETCHAR(fixtran,"DISTRIBUTED",&d);
							if(read_record==TRUE && ((SELECT_DISTRIBUTED==TRUE && d==TRUE) || (SELECT_UNDISTRIBUTED==TRUE && d==FALSE)))
							{
								FINDFLDGETSTRING(fixtran,"ITEM",&id);
								FINDFLDGETSTRING(fixtran,"ENTRY DATE",&edate);
								FINDFLDGETSTRING(fixtran,"DESCRIPTION",&desc);
								FINDFLDGETSTRING(fixtran,"SOURCE USER",&su);
								FINDFLDGETSTRING(fixtran,"SERIAL NUMBER",&c);
								FINDFLDGETSTRING(fixtran,"SERIAL NUMBER",&ref);
								FINDFLDGETINT(fixtran,"TRANSACTION NUMBER",&trans);
								FINDFLDGETDOUBLE(fixtran,"AMOUNT",&amt);
								cy=atoi(&edate[6]);
								deb=0.0;cred=0.0;
								if(AccountType==0 && amt>=0.0) 
								{
									deb=amt;
									addsortrecord(dbsrt,21,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								} else if(AccountType==0) 
								{
									cred=amt*(-1);
									addsortrecord(dbsrt,21,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==1 && amt>=0.0) 
								{
									cred=amt;
									addsortrecord(dbsrt,21,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								} else if(AccountType==1) 
								{
									deb=amt*(-1);
									addsortrecord(dbsrt,21,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) 
								{
									deb=amt;
									addsortrecord(dbsrt,21,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								if(AccountType==2 && !RDAstrcmp(credcode,AccountCode)) 
								{
									cred=amt;
									addsortrecord(dbsrt,21,adate,id,ref,trans,accnum,cy,gy,c,desc,su,edate,deb,cred,d,bankid);
								}
								FINDFLDGETSTRING(fixtran,"BANK IDENTIFICATION",&bankid);
							}
						}
					}
					if(!abort_diagnostic)
					{
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(read_record ? TRUE:FALSE));
					}
					ef=ADVNXTNRDsec(fixtran,1,SCRNvirtualSubData,MAINRSRC);
				}
				ZERNRD(fixtran);
				if(tmprsrc!=NULL) 
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
			}
		}
	}
	if(adate!=NULL) Rfree(adate);
	if(edate!=NULL) Rfree(edate);
	if(id!=NULL) Rfree(id);
	if(c!=NULL) Rfree(c);
	if(desc!=NULL) Rfree(desc);
	if(su!=NULL) Rfree(su);
	if(ref!=NULL) Rfree(ref);
	if(date_affected!=NULL) Rfree(date_affected);
	if(bankid!=NULL) Rfree(bankid);
	if(debcode!=NULL) Rfree(debcode);
	if(credcode!=NULL) Rfree(credcode);
	if(payable!=NULL) Rfree(payable);
	if(duefrom!=NULL) Rfree(duefrom);
	if(clearing_cash!=NULL) Rfree(clearing_cash);
	if(clearing_dueto!=NULL) Rfree(clearing_dueto);
	if(receivable_code!=NULL) Rfree(receivable_code);
	if(deferred_code!=NULL) Rfree(deferred_code);
	if(refid!=NULL) Rfree(refid);
	DisplayData();
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL DEBITS",TOTAL_DEBITS);
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL CREDITS",TOTAL_CREDITS);
	if(AccountType!=1)
	{
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_DEBITS-TOTAL_CREDITS));
	} else {
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_CREDITS-TOTAL_DEBITS));
	}
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updateallrsrc(MAINRSRC);
}
void ClearDataBase()
{
	short ef=0;

	CurrentPage=1;TotalPages=1;
	ZERNRD(dbsrt->fileno);
	ef=FRSNRD(dbsrt->fileno,1);
	while(!ef)
	{
		FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",TRUE);
		WRTNRD(dbsrt->fileno,0,NULL);
		DELNRD(dbsrt->fileno);
		ef=NXTNRD(dbsrt->fileno,1);
	}
	ZERNRD(dbsrt->fileno);
}
void ClearTableResources()
{
	short nofields=0,x=0;
	int y=0,r=0;
	NRDfield *fields=NULL,*fldx;
	RDArmem *mem=NULL;
	char DQUERYstemp[101];

	if(!IsScreenDisplayed(MAINRSRC)) return;
	nofields=NUMFLDS(dbsrt->fileno);
	fields=FLDPOINTER(dbsrt->fileno);
	for(y=1;y<(NumberRows+1);++y)
	{
		if(fields!=NULL)
		{
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:SOURCE DESCRIPTION",y);
			FINDRSCSETSENSITIVE(MAINRSRC,DQUERYstemp,FALSE);
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%08d:%s",y,fldx->name);
				r=FINDRSC(MAINRSRC,DQUERYstemp);
				if(r>(-1))
				{
					mem=MAINRSRC->rscs+r;
					switch(mem->field_type)
					{
						case SCROLLEDLIST:
							*mem->value.integer_value=0;
							break;
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(mem->value.string_value!=NULL) Rfree(mem->value.string_value);
							mem->dlen=0;
							break;
						case BOOLNS:
						case CHARACTERS:
							*mem->value.string_value=0;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							*mem->value.float_value=0.0;
							break;
						case SHORTV:
						case SSHORTV:
							*mem->value.short_value=0;
							break;
						case PROGRESS_BAR:
						case LONGV:
						case SLONGV:
							*mem->value.integer_value=0;
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error field type [%s] invalid for Resource [%s] on Screen [%s] [%s].",mem->field_type,mem->rscrname,MAINRSRC->module,MAINRSRC->screen);
							break;
					}
					MEMBERSETSENSITIVE(mem,FALSE);
					updatemember(mem);
				}
			}
		}
	}
}
static void set_defaults(RDArsrc *parent)
{
	DefaultResetScreen(parent,TRUE);
	readwidget(parent,"SELECT DISTRIBUTED");
	readwidget(parent,"SELECT UNDISTRIBUTED");
	FINDRSCGETCHAR(parent,"SELECT DISTRIBUTED",&SELECT_DISTRIBUTED);
	FINDRSCGETCHAR(parent,"SELECT UNDISTRIBUTED",&SELECT_UNDISTRIBUTED);
	changeACCTtype(parent);
	changeyear(parent);
	changeDEFtype(parent);
	getacctdesc(parent);
	GatherData();
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void acctsearch(RDArsrc *parent)
{
	char *searchwidget=NULL,*listwidget=NULL,*selectedc=NULL;
	APPlib *holdapplib=NULL;
	int x=0;

	searchwidget=stralloc("ACCOUNT CODE SEARCH");
	listwidget=stralloc("ACCOUNT CODES");
	holdapplib=acctavl;
	if(searchwidget==NULL) return;
	readwidget(parent,searchwidget);
	if(selectedc!=NULL) Rfree(selectedc);
	FINDRSCGETSTRING(parent,searchwidget,&selectedc);
	if(selectedc!=NULL)
	{
		if(RDAstrlen(selectedc)!=0)
		{
			for(x=0;x<holdapplib->numlibs;++x)
			{
				if(strncmp(selectedc,holdapplib->libs[x],RDAstrlen(selectedc))<=0)
				{
					break;
				} 
			}
			if(x>=holdapplib->numlibs) x=(holdapplib->numlibs-1);
		}
	}
	FINDRSCSETINT(parent,listwidget,x);
	updatersrc(parent,listwidget);
	getacctdesc(parent);
	if(selectedc!=NULL) Rfree(selectedc);
	if(searchwidget!=NULL) Rfree(searchwidget);
	if(listwidget!=NULL) Rfree(listwidget);
}
static void getacctdesc(RDArsrc *parent)
{
	int selacct=0,accttype=0,deftype=0;
	short fileaccttype=0,filedeftype=0;
	char *descript=NULL;

	if(DID_STUFF==TRUE)
	{
		if(SOURCE_USER!=NULL) Rfree(SOURCE_USER);
		if(SOURCE_MODULE!=NULL) Rfree(SOURCE_MODULE);
		if(SOURCE_PROCESS!=NULL) Rfree(SOURCE_PROCESS);
		if(DISTRIBUTED_DATE!=NULL) Rfree(DISTRIBUTED_DATE);
		if(DISTRIBUTED_TIME!=NULL) Rfree(DISTRIBUTED_TIME);
		if(REFERENCE_ID!=NULL) Rfree(REFERENCE_ID);
	}
	readwidget(parent,"SELECT DISTRIBUTED");
	readwidget(parent,"SELECT UNDISTRIBUTED");
	FINDRSCGETCHAR(parent,"SELECT DISTRIBUTED",&SELECT_DISTRIBUTED);
	FINDRSCGETCHAR(parent,"SELECT UNDISTRIBUTED",&SELECT_UNDISTRIBUTED);
	FINDRSCGETINT(parent,"ACCOUNT CODES",&selacct);
	if(AccountCode!=NULL) Rfree(AccountCode);
	TOTAL_DEBITS=0.0;
	TOTAL_CREDITS=0.0;
	CurrentPage=1;
	TotalPages=1;
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL DEBITS",TOTAL_DEBITS);
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL CREDITS",TOTAL_CREDITS);
	if(AccountType!=1)
	{
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_DEBITS-TOTAL_CREDITS));
	} else {
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_CREDITS-TOTAL_DEBITS));
	}
	FINDRSCSETSENSITIVE(MAINRSRC,"TOTAL DEBITS",FALSE);
	FINDRSCSETSENSITIVE(MAINRSRC,"TOTAL CREDITS",FALSE);
	FINDRSCSETSENSITIVE(MAINRSRC,"BALANCE",FALSE);
	FINDRSCSETSENSITIVE(MAINRSRC,"CSV",FALSE);
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updatersrc(MAINRSRC,"TOTAL DEBITS");
	updatersrc(MAINRSRC,"TOTAL CREDITS");
	updatersrc(MAINRSRC,"BALANCE");
	updatersrc(MAINRSRC,"CURRENT PAGE");
	updatersrc(MAINRSRC,"TOTAL PAGES");
	if(selacct<acctavl->numlibs)
	{
               	if(RDAstrcmp(acctavl->libs[selacct],"No Accounts Available"))
		{
			descript=stralloc(acctavl->libs[selacct]);
			FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",acctavl->libs[selacct]);
			AccountCode=stralloc(acctavl->libs[selacct]);
		} else {
			FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
		}
	} else {
		FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	}
	updatersrc(parent,"ACCOUNT CODE SEARCH");
	FINDRSCGETINT(parent,"ACCOUNT TYPES",&accttype);
	fileaccttype=(short)accttype;
	FINDFLDSETSHORT(filenum,"ACCOUNT TYPE",fileaccttype);
	FINDRSCGETINT(parent,"DEFINITION TYPES",&deftype);
	filedeftype=(short)deftype;
	switch(accttype)
	{
		case EXP_ACCT:
			ZERNRD(expnum);
			FINDFLDSETSHORT(expnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(expnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(expnum,1,SCRNvirtualSubData,parent)) KEYNRD(expnum,1);
			if(descript!=NULL) Rfree(descript);
			FINDFLDGETSTRING(expnum,"DESCRIPTION",&descript);
			break;
		case REV_ACCT:
			ZERNRD(revnum);
			FINDFLDSETSHORT(revnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(revnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(revnum,1,SCRNvirtualSubData,parent)) KEYNRD(revnum,1);
			if(descript!=NULL) Rfree(descript);
			FINDFLDGETSTRING(revnum,"DESCRIPTION",&descript);
			break;
		case BAL_ACCT:
			ZERNRD(balnum);
			FINDFLDSETSHORT(balnum,"DEFINITION TYPE",filedeftype);
			FINDFLDSETSTRING(balnum,"ACCOUNT CODE",descript);
			if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,parent)) KEYNRD(balnum,1);
			if(descript!=NULL) Rfree(descript);
			FINDFLDGETSTRING(balnum,"DESCRIPTION",&descript);
			break;
		default:
			prterr("Invalid Account Type [%d]",accttype);
			break;
	}
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",descript);
	updatersrc(parent,"ACCOUNT DESCRIPTION");
	if(descript!=NULL) Rfree(descript);
	updateSCRNvirtuals(parent);
	ClearDataBase();
	ClearTableResources();
}
static void quit_record_free(RDArsrc *parent)
{
	if(parent!=NULL) 
	{
		killwindow(parent);
		free_rsrc(parent);
	}
	if(dbsrt!=NULL) 
	{
		CLSDBsort(dbsrt);
		FreeDBsort(dbsrt);
	}
	unlink(filename);
	if(fineact!=(-1)) CLSNRD(fineact);
	if(finbact!=(-1)) CLSNRD(finbact);
	if(expnum!=(-1)) CLSNRD(expnum);
	if(revnum!=(-1)) CLSNRD(revnum);
	if(balnum!=(-1)) CLSNRD(balnum);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(yearnum!=(-1)) CLSNRD(yearnum);
	if(eyrnum!=(-1)) CLSNRD(eyrnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(finjen!=(-1)) CLSNRD(finjen);
	if(finrec!=(-1)) CLSNRD(finrec);
	if(venvch!=(-1)) CLSNRD(venvch);
	if(venmst!=(-1)) CLSNRD(venmst);
	if(venfyr!=(-1)) CLSNRD(venfyr);
	if(purtrn!=(-1)) CLSNRD(purtrn);
	if(purmst!=(-1)) CLSNRD(purmst);
	if(puracc!=(-1)) CLSNRD(puracc);
	if(purlin!=(-1)) CLSNRD(purlin);
	if(payjtrn!=(-1)) CLSNRD(payjtrn);
	if(paydtrn!=(-1)) CLSNRD(paydtrn);
	if(payddsv!=(-1)) CLSNRD(payddsv);
	if(payddms!=(-1)) CLSNRD(payddms);
	if(payjpms!=(-1)) CLSNRD(payjpms);
	if(paymst!=(-1)) CLSNRD(paymst);
	if(invadj!=(-1)) CLSNRD(invadj);
	if(invisst!=(-1)) CLSNRD(invisst);
	if(invacc!=(-1)) CLSNRD(invacc);
	if(bnkchg!=(-1)) CLSNRD(bnkchg);
	if(bnkrtn!=(-1)) CLSNRD(bnkrtn);
	if(prptrn!=(-1)) CLSNRD(prptrn);
	if(prppay!=(-1)) CLSNRD(prppay);
	if(restran!=(-1)) CLSNRD(restran);
	if(restpay!=(-1)) CLSNRD(restpay);
	if(utbtrn!=(-1)) CLSNRD(utbtrn);
	if(utbsvc!=(-1)) CLSNRD(utbsvc);
	if(utbcds!=(-1)) CLSNRD(utbcds);
	if(misctran!=(-1)) CLSNRD(misctran);
	if(miscpaid!=(-1)) CLSNRD(miscpaid);
	if(occrec!=(-1)) CLSNRD(occrec);
	if(prmttran!=(-1)) CLSNRD(prmttran);
	if(prmtpaid!=(-1)) CLSNRD(prmtpaid);
	if(prmtmstr!=(-1)) CLSNRD(prmtmstr);
	if(restmst!=(-1)) CLSNRD(restmst);
	if(prpmst!=(-1)) CLSNRD(prpmst);
	if(miscmstr!=(-1)) CLSNRD(miscmstr);
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(fixtran!=(-1)) CLSNRD(fixtran);
	if(ACCTtypes!=NULL) freeapplib(ACCTtypes);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(yearavl!=NULL) freeapplib(yearavl);
	if(acctavl!=NULL) freeapplib(acctavl);
	if(AccountCode!=NULL) Rfree(AccountCode);
	if(pmonth!=NULL) freeapplib(pmonth);
	FreeRDAvirtual(EmployeeName);
	if(finsetup!=NULL) free_finmgt(finsetup);
}
static void quit_record(RDArsrc *parent)
{
	quit_record_free(parent);
	ShutdownSubsystems();
	exit(0);
}
void setyear(RDArsrc *parent)
{
	int yearid=0,yearno=0;

	ZERNRD(yearnum);
	if(FINDRSCGETINT(parent,"YEARS AVAILABLE",&yearno)) return;
	yearid=Ratoi(yearavl->libs[yearno]);
	FINDFLDSETINT(filenum,"FISCAL YEAR",yearid);
	FINDFLDSETINT(yearnum,"FISCAL YEAR",yearid);
	if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,parent)) KEYNRD(yearnum,1);
	updatefilerecord2rsrc(yearnum,parent);
}
void changeyear(RDArsrc *parent)
{
	int selecteda=0,selectedyear=0,yearid=0;

	FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(parent,"YEARS AVAILABLE",&selectedyear);
	if(selectedyear>=yearavl->numlibs) selectedyear=0;
	yearid=Ratoi(yearavl->libs[selectedyear]);
	if(yearid!=FiscalYear || selecteda!=AccountType)
	{
		FiscalYear=(-1);
	  	AccountType=(-1);
	  	DefinitionType=(-1);
		setyear(parent);
		changeDEFtype(parent);
		updateSCRNvirtuals(parent);
	}
}
static void changeCODElist(RDArsrc *parent)
{
	int selecteda=0,selectedd=0,selyear=0,yearid=0;
	short ef=0,accttype=0,deftype=0,deftype1=0,tempyrfile=0,tempaccfile=0;
	short keyn=0,keynumber=0;
	char *account_code=NULL,active=FALSE,deleteflag=FALSE;
	char *baltype=NULL,*e=NULL,*actyp=NULL,*speedkeyname=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
	if(FINDRSCGETINT(parent,"YEARS AVAILABLE",&selyear)) return;
	yearid=Ratoi(yearavl->libs[selyear]);
	selyear=yearid;
	accttype=(short)selecteda;
	deftype=(short)selectedd;
	FINDRSCSETACCTTYPE(parent,"ACCOUNT CODE SEARCH",accttype);
	FINDRSCSETDEFTYPE(parent,"ACCOUNT CODE SEARCH",deftype);
	if(yearid!=FiscalYear || selecteda!=AccountType
  		|| selectedd!=DefinitionType)
	{
		acctavl=APPlibNEW();
               	switch(selecteda)
                	{
			case EXP_ACCT:
				tempyrfile=eyrnum;
				speedkeyname=stralloc("RDA FINEYR KEY");
				tempaccfile=expnum;
				actyp=stralloc("Expenditure");
				break;
			case REV_ACCT:
				tempyrfile=ryrnum;
				speedkeyname=stralloc("RDA FINRYR KEY");
				tempaccfile=revnum;
				actyp=stralloc("Revenue");
				break;
			case BAL_ACCT:
				tempyrfile=byrnum;
				speedkeyname=stralloc("RDA FINBYR KEY");
				tempaccfile=balnum;
				actyp=stralloc("Balance Sheet");
				break;
			default:
				prterr("Invalid Account Type [%d]",selecteda);
				break;
		}
		ZERNRD(tempyrfile);
		FINDFLDSETSHORT(tempyrfile,"DEFINITION TYPE",deftype);
		FINDFLDSETINT(tempyrfile,"FISCAL YEAR",selyear);
		if((keyn=KEYNUMBER(tempyrfile,"RDA FINEYR KEY"))!=(-1))
		{
			keynumber=keyn;
		} else {
			keynumber=1;
		}
		ef=ADVGTENRDsec(tempyrfile,keynumber,SCRNvirtualSubData,parent);
		e=Rmalloc(RDAstrlen(actyp)+34);
		sprintf(e,"Updating %s Accounts Available List",actyp);
		tmprsrc=diagscrn(tempyrfile,"DIAGNOSTIC SCREEN",module,e,NULL);
		if(e!=NULL) Rfree(e);
		e=NULL;
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		while(!ef)
		{
			listupdated=FALSE;
			FINDFLDGETSHORT(tempyrfile,"DEFINITION TYPE",&deftype1);
			if(deftype1!=deftype) break;
			FINDFLDGETINT(tempyrfile,"FISCAL YEAR",&yearid);
			if(account_code!=NULL) { Rfree(account_code); account_code=NULL; }
			FINDFLDGETSTRING(tempyrfile,"ACCOUNT CODE",&account_code);
			if(keyn!=(-1))
			{
				if(yearid!=selyear) break;
			} 
			if(yearid==selyear)
			{
				ZERNRD(tempaccfile);
				FINDFLDSETSHORT(tempaccfile,"DEFINITION TYPE",deftype);
				FINDFLDSETSTRING(tempaccfile,"ACCOUNT CODE",account_code);
				if(!ADVEQLNRDsec(tempaccfile,1,SCRNvirtualSubData,parent))
				{
					FINDFLDGETCHAR(tempaccfile,"DELETEFLAG",&deleteflag);
					FINDFLDGETCHAR(tempaccfile,"ACTIVE",&active);
					if(!deleteflag)
					{
						FINDFLDGETCHAR(tempyrfile,"DELETEFLAG",&deleteflag);
						FINDFLDGETCHAR(tempyrfile,"ACTIVE",&active);
						if(account_code!=NULL && !deleteflag)
						{
							addAPPlib(acctavl,account_code);
							listupdated=TRUE;
						}
					}
				}
			} 
			if(account_code!=NULL) Rfree(account_code); 
			account_code=NULL; 
			ef=ADVNXTNRDsec(tempyrfile,keynumber,SCRNvirtualSubData,parent);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,listupdated);
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	FiscalYear=selyear;
	AccountType=selecteda;
	DefinitionType=selectedd;
	if(account_code!=NULL) Rfree(account_code);
	if(baltype!=NULL) Rfree(baltype);
	if(actyp!=NULL) Rfree(actyp);
	if(speedkeyname!=NULL) Rfree(speedkeyname);;
}
static void changeDEFtype(RDArsrc *parent)
{
	int sela=0,x=0;
	int selecteddef=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&selecteddef);
	if(selecteddef!=DefinitionType)
	{
		FiscalYear=(-1);
		FINDRSCGETINT(parent,"ACCOUNT CODES",&sela);
		changeCODElist(parent);
		if(acctavl->numlibs<1)
		{
			addAPPlib(acctavl,"No Accounts Available");
		}
		for(x=0;x<acctavl->numlibs;++x) 
		{
			if(!RDAstrcmp(AccountCode,acctavl->libs[x])) 
			{
				sela=x;
				break;
			}
		}
		if(sela>=acctavl->numlibs) sela=0;
		if(!FINDRSCLISTAPPlib(parent,"ACCOUNT CODES",sela,acctavl))
		{
			updatersrc(parent,"ACCOUNT CODES");
		}
		getacctdesc(parent);
	}
}
static void DEFtypecb(RDArsrc *parent)
{
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	changeDEFtype(parent);
}
static void ACCTtypecb(RDArsrc *parent)
{
	int sdtype=0;

	FINDRSCGETINT(parent,"DEFINITION TYPES",&sdtype);
	FINDRSCSETSTRING(parent,"ACCOUNT CODE SEARCH",NULL);
	FINDRSCSETSTRING(parent,"ACCOUNT DESCRIPTION",NULL);
	changeACCTtype(parent);
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",sdtype,DEFtypes))
	{
		updatersrc(parent,"DEFINITION TYPES");
	}
	changeDEFtype(parent);
}
static void changeACCTtype(RDArsrc *parent)
{
	int selectedd=0,acctype=0;
	int x;
	RDAacct *acct;

	if(FINDRSCGETINT(parent,"ACCOUNT TYPES",&acctype)) return;
	if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
	if(AccountType==(-1) && DefaultAccountType!=(-1)) acctype=DefaultAccountType;
	if(acctype==2)
	{
		GrantYear=(-1);
		FINDRSCSETINT(parent,"GRANT YEAR",GrantYear);
		FINDRSCSETSENSITIVE(parent,"GRANT YEAR",FALSE);
		FINDRSCSETEDITABLE(parent,"GRANT YEAR",FALSE);
	} else {
		FINDRSCSETSENSITIVE(parent,"GRANT YEAR",TRUE);
		FINDRSCSETEDITABLE(parent,"GRANT YEAR",TRUE);
	}
	if(acctype!=AccountType || selectedd!=DefinitionType)
	{
		if(DefinitionType==(-1) && DefaultDefinitionType!=(-1)) selectedd=DefaultDefinitionType;
		DefinitionType=(-1);
/*
		FiscalYear=(-1);
*/
		if(DEFtypes!=NULL) freeapplib(DEFtypes);
		DEFtypes=APPlibNEW();
		switch(acctype)
		{
			case 0: /* Expenditures */
				if(EXPENDITURE_ACCOUNT->codetype!=NULL)
				{
					for(x=0,acct=EXPENDITURE_ACCOUNT->codetype;
						x<EXPENDITURE_ACCOUNT->num;++x,++acct)
					{
						addAPPlib(DEFtypes,acct->name);
					}
				}
				break;
			case 1: /* Revenues */
				if(REVENUE_ACCOUNT->codetype!=NULL)
				{
					for(x=0,acct=REVENUE_ACCOUNT->codetype;
						x<REVENUE_ACCOUNT->num;++x,++acct)
					{
						addAPPlib(DEFtypes,acct->name);
					}
				}
				break;
			case 2: /* Balance Sheets */
				if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
				{
					for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
						x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
					{
						addAPPlib(DEFtypes,acct->name);
					}
				}
				break;
			default:
				prterr("Invalid Account Type [%d]",acctype);
				break;
		}
		if(DEFtypes->numlibs<1)
		{
			addAPPlib(DEFtypes,"Not Available");
		}
		if(selectedd>=DEFtypes->numlibs) selectedd=0;
		if(!FINDRSCLISTAPPlib(parent,"DEFINITION TYPES",selectedd,DEFtypes))
		{
			updatersrc(parent,"DEFINITION TYPES");
		}
	}
}
void changeFromAFFECTdate(RDArsrc *r)
{
	readwidget(MAINRSRC,"FROM AFFECT DATE");
	FINDRSCGETSTRING(MAINRSRC,"FROM AFFECT DATE",&FromAFFECTDate);
	changeyear(MAINRSRC);
	changeDEFtype(MAINRSRC);
	getacctdesc(MAINRSRC);
	TOTAL_DEBITS=0.0;
	TOTAL_CREDITS=0.0;
	CurrentPage=1;
	TotalPages=1;
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL DEBITS",TOTAL_DEBITS);
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL CREDITS",TOTAL_CREDITS);
	if(AccountType!=1)
	{
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_DEBITS-TOTAL_CREDITS));
	} else {
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_CREDITS-TOTAL_DEBITS));
	}
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updatersrc(MAINRSRC,"TOTAL DEBITS");
	updatersrc(MAINRSRC,"TOTAL CREDITS");
	updatersrc(MAINRSRC,"BALANCE");
	updatersrc(MAINRSRC,"CURRENT PAGE");
	updatersrc(MAINRSRC,"TOTAL PAGES");
	ClearTableResources();
	updateallrsrc(MAINRSRC);
	ClearDataBase();
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updateallrsrc(MAINRSRC);
}
void changeAFFECTdate(RDArsrc *r)
{
	int PM=0;
	int x=0;
	char DQUERYstemp[101];

	readwidget(MAINRSRC,"AFFECT DATE");
	FINDRSCGETSTRING(MAINRSRC,"AFFECT DATE",&AFFECTDate);
	if(ADCFP==TRUE)
	{
		memset(DQUERYstemp,0,101);
		sprintf(DQUERYstemp,"%.2s",&AFFECTDate[0]);
		PM=atoi(DQUERYstemp);
		FiscalYear=atoi(&AFFECTDate[6]);
		if(PM>finsetup->month) ++FiscalYear;
		for(x=0;x<yearavl->numlibs;++x)
		{
			if(FiscalYear==atoi(yearavl->libs[x])) break;
		}
		if(x>=yearavl->numlibs) x=0;
		FINDRSCSETINT(MAINRSRC,"PROCESSING MONTHS",PM);
		FINDRSCSETINT(MAINRSRC,"YEARS AVAILABLE",x);
		updatersrc(MAINRSRC,"PROCESSING MONTHS");
		updatersrc(MAINRSRC,"YEARS AVAILABLE");
	}
	changeyear(MAINRSRC);
	changeDEFtype(MAINRSRC);
	getacctdesc(MAINRSRC);
	TOTAL_DEBITS=0.0;
	TOTAL_CREDITS=0.0;
	CurrentPage=1;
	TotalPages=1;
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL DEBITS",TOTAL_DEBITS);
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL CREDITS",TOTAL_CREDITS);
	if(AccountType!=1)
	{
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_DEBITS-TOTAL_CREDITS));
	} else {
		FINDRSCSETDOUBLE(MAINRSRC,"BALANCE",(TOTAL_CREDITS-TOTAL_DEBITS));
	}
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updatersrc(MAINRSRC,"TOTAL DEBITS");
	updatersrc(MAINRSRC,"TOTAL CREDITS");
	updatersrc(MAINRSRC,"BALANCE");
	updatersrc(MAINRSRC,"CURRENT PAGE");
	updatersrc(MAINRSRC,"TOTAL PAGES");
	ClearTableResources();
	updateallrsrc(MAINRSRC);
	ClearDataBase();
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updateallrsrc(MAINRSRC);
}
static int getyears(RDArsrc *parent)
{
	short ef,x=0;
	int selyear=0,year=0;
	char delflag=FALSE;
	char DQUERYstemp[101];

	if(yearavl!=NULL) freeapplib(yearavl);
	yearavl=APPlibNEW();
	ZERNRD(yearnum);
	ef=ADVFRSNRDsec(yearnum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETCHAR(yearnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(yearnum,"FISCAL YEAR",&year);
			sprintf(DQUERYstemp,"%-4d",year);
			addAPPlib(yearavl,DQUERYstemp);
			if(year==FiscalYear) 
			{
				if(dm==(-1))
				{
					FINDFLDGETSHORT(yearnum,"PROCESSING MONTH",&x);
					dm=x;
				}
				selyear=((yearavl->numlibs)-1);
			}
		}
		ef=ADVNXTNRDsec(yearnum,1,SCRNvirtualSubData,parent);
	}
	return(selyear);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
	if(yearnum!=(-1))
	{
		COPYFIELD(filenum,yearnum,"FISCAL YEAR");
		if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,parent)) KEYNRD(yearnum,1);
		if(update) updatefilerecord2rsrc(yearnum,parent);
		else filerecord2rsrc(yearnum,parent);
	}
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void csv_dump(RDArsrc *parent)
{
	FILE *fp=NULL;
	APPlib *args=NULL;
	int pid=0,PM=0;
	short ef=0,src=0;
	int cy=0,trans=0,acctnum=0,gy=0;
	char *adate=NULL,*ref=NULL,*c=NULL,*desc=NULL,*su=NULL,*id=NULL;
	char *edate=NULL,d=FALSE,delflag=FALSE,*bankid=NULL;
	double deb=0.0,cred=0.0;

	unlink(filename);
	readwidget(MAINRSRC,"PROCESSING MONTHS");
	FINDRSCGETINT(MAINRSRC,"PROCESSING MONTHS",&PM);
	pid=RGETPID();
	memset(filename,0,101);
	sprintf(filename,"%s-%4d-%02d-%d.csv",AccountCode,FiscalYear,PM,pid);
	fp=fopen(filename,"w+");
#ifndef WIN32
	fp=fopen(filename,"w+b");
#else
	fp=fopen(filename,"wbc");
#endif
	if(fp!=NULL)
	{
		fprintf(fp,"\"%s\",\"CONTROL\",\"DEBIT AMOUNT\",\"CREDIT AMOUNT\",\"REFERENCE\",\"DESCRIPTION\",\"IDENTIFICATION\",\"BANK IDENTIFICATION\",\"GRANT YEAR\",\"ENTRY DATE\",\"DISTRIBUTED\",\"CALENDAR YEAR\",\"SOURCE USER\",\"TRANS#\",\"ACC#\",\"SOURCE DESCRIPTION\",\"SOURCE NUMBER\"\r\n",(SWB_FM25_AFFECT_DATE==TRUE ? "AFFECT DATE":"DATE"));
			
		ef=FRSNRD(dbsrt->fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(dbsrt->fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(dbsrt->fileno,"AFFECT DATE",&adate);
				FINDFLDGETSTRING(dbsrt->fileno,"ENTRY DATE",&edate);
				FINDFLDGETSTRING(dbsrt->fileno,"CONTROL",&c);
				FINDFLDGETDOUBLE(dbsrt->fileno,"DEBIT AMOUNT",&deb);
				FINDFLDGETDOUBLE(dbsrt->fileno,"CREDIT AMOUNT",&cred);
				FINDFLDGETSTRING(dbsrt->fileno,"DESCRIPTION",&desc);
				FINDFLDGETSTRING(dbsrt->fileno,"REFERENCE",&ref);
				FINDFLDGETSTRING(dbsrt->fileno,"IDENTIFICATION",&id);
				FINDFLDGETSTRING(dbsrt->fileno,"BANK IDENTIFICATION",&bankid);
				FINDFLDGETCHAR(dbsrt->fileno,"DISTRIBUTED",&d);
				FINDFLDGETSTRING(dbsrt->fileno,"SOURCE USER",&su);
				FINDFLDGETSHORT(dbsrt->fileno,"SOURCE",&src);
				FINDFLDGETINT(dbsrt->fileno,"TRANSACTION NUMBER",&trans);
				FINDFLDGETINT(dbsrt->fileno,"ACCOUNT NUMBER",&acctnum);
				FINDFLDGETINT(dbsrt->fileno,"CALENDAR YEAR",&cy);
				FINDFLDGETINT(dbsrt->fileno,"GRANT YEAR",&gy);
				fprintf(fp,"\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%4d\",\"%s\",\"%s\",\"%4d\",\"%s\",\"%8d\",\"%4d\",\"%s\",\"%2d\"\r\n",
					(adate!=NULL ? adate:""),
					(c!=NULL ? c:""),
					famt(deb,15),famt(cred,15),
					(ref!=NULL ? ref:""),
					(desc!=NULL ? desc:""),
					(id!=NULL ? id:""),
					(bankid!=NULL ? bankid:""),gy,
					(edate!=NULL ? edate:""),
					(d==TRUE ? "True":"False"),cy,
					(su!=NULL ? su:""),
					trans,acctnum,SourceList[src],src);
			}
			ef=NXTNRD(dbsrt->fileno,1);
		}
		fclose(fp);
	}
	if(adate!=NULL) Rfree(adate);
	if(edate!=NULL) Rfree(edate);
	if(ref!=NULL) Rfree(ref);
	if(c!=NULL) Rfree(c);
	if(desc!=NULL) Rfree(desc);
	if(su!=NULL) Rfree(su);
	if(id!=NULL) Rfree(id);
	if(bankid!=NULL) Rfree(bankid);
	args=APPlibNEW();
	addAPPlib(args,filename);
	addAPPlib(args,"csv");
	Execute2Program("doarchive",args);
	if(args!=NULL) freeapplib(args);
}
static void GetData()
{
	ClearDataBase();
	ClearTableResources();
	GatherData();
	DisplayData();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	int start=0,presentyear=0,y=0;
	int selecteda=0,selectedd=0;
	short deftype=0,accttype=0;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL;
	char *name=NULL,*filename=NULL,*sortname=NULL,*temp=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	DFvirtual *d=NULL;
	char DQUERYstemp[101];
	char *AFFECT_DATE=NULL;

	if(InitializeSubsystems(argc,argv,module,"DETAIL QUERY")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	diaggui=TRUE;TRACE;
	PayrollActivity=FALSE;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
	gsv=RDAGenericSetupNew("FINMGT","SWB_FM25_AFFECT_DATE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SWB_FM25_AFFECT_DATE=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	ADCFP=FALSE;
	gsv=RDAGenericSetupNew("FINMGT","SET FISCAL PERIOD USING AFFECT DATE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			ADCFP=*gsv->value.string_value;
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
	gsv=RDAGenericSetupNew("VENPMT","AFFECT DATE STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			AFFECT_DATE_STYLE=*gsv->value.short_value;
		}
	}
	if(diagapps)
	{
		fprintf(RDA_STDERR,"AFFECT_DATE_STYLE [%d] ",AFFECT_DATE_STYLE);TRACE;
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif

	gsv=RDAGenericSetupNew("PAYROLL","V/W DISTRIBUTION OPTIONS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			payvdst_option=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	temp=RDA_GetEnv("ACCOUNT_CODE");
	if(!isEMPTY(temp)) AccountCode=stralloc(temp);
	temp=RDA_GetEnv("FISCAL_YEAR");
	if(!isEMPTY(temp)) FiscalYear=atoi(temp);
	if(FiscalYear!=(-1))
	{
		temp=RDA_GetEnv("FISCAL_MONTH");
		if(!isEMPTY(temp))  dm=atoi(temp);
	}
	temp=RDA_GetEnv("GRANT_YEAR");
	if(!isEMPTY(temp)) GrantYear=atoi(temp);
		else GrantYear=(-1);
	temp=RDA_GetEnv("SOURCE_USER");
	if(!isEMPTY(temp)) SOURCE_USER=stralloc(temp);
	temp=RDA_GetEnv("SOURCE_MODULE");
	if(!isEMPTY(temp)) SOURCE_MODULE=stralloc(temp);
	temp=RDA_GetEnv("SOURCE_PROCESS");
	if(!isEMPTY(temp)) SOURCE_PROCESS=stralloc(temp);
	temp=RDA_GetEnv("DISTRIBUTED_DATE");
	if(!isEMPTY(temp)) DISTRIBUTED_DATE=stralloc(temp);
	temp=RDA_GetEnv("DISTRIBUTED_TIME");
	if(!isEMPTY(temp)) DISTRIBUTED_TIME=stralloc(temp);
	temp=RDA_GetEnv("AFFECT_DATE");
	if(!isEMPTY(temp)) AFFECT_DATE=stralloc(temp);
	temp=RDA_GetEnv("REFERENCE_IDENTIFICATION");
	if(!isEMPTY(temp)) REFERENCE_ID=stralloc(temp);
	TRACE;
	if(diagapps)
	{
		prterr("DIAG     ACCOUNT CODE [%s] ",(AccountCode!=NULL ? AccountCode:""));
		prterr("DIAG      FISCAL YEAR [%d] ",FiscalYear);
		prterr("DIAG     FISCAL MONTH [%d] ",dm);
		prterr("DIAG      SOURCE_USER [%s] ",(SOURCE_USER!=NULL ? SOURCE_USER:""));
		prterr("DIAG    SOURCE_MODULE [%s] ",(SOURCE_MODULE!=NULL ? SOURCE_MODULE:""));
		prterr("DIAG   SOURCE_PROCESS [%s] ",(SOURCE_PROCESS!=NULL ? SOURCE_PROCESS:""));
		prterr("DIAG DISTRIBUTED DATE [%s] ",(DISTRIBUTED_DATE!=NULL ? DISTRIBUTED_DATE:""));
		prterr("DIAG DISTRIBUTED TIME [%s] ",(DISTRIBUTED_TIME!=NULL ? DISTRIBUTED_TIME:""));
		prterr("DIAG      AFFECT DATE [%s] ",(AFFECT_DATE!=NULL ? AFFECT_DATE:""));
		prterr("DIAG     REFERENCE ID [%s] ",(REFERENCE_ID!=NULL ? REFERENCE_ID:""));
		prterr("DIAG       GRANT YEAR [%d] ",GrantYear);
	}
	if(FiscalYear==(-1))
	{
		gsv=RDAGenericSetupNew("FINMGT","CURRENT FISCAL YEAR");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				FiscalYear=*gsv->value.short_value;
			}
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(argc>1) 
	{
		DefaultAccountType=atoi(argv[1]);
	}
	if(argc>2)
	{
		DefaultDefinitionType=atoi(argv[2]);
	}
	if(argc>3)
	{
		if(!RDAstrcmp(argv[3],"MULTIPLE_PERIODS")) Style=1;
		else if(!RDAstrcmp(argv[3],"SUM_CONTROL")) Summed=1;
		else if(!RDAstrcmp(argv[3],"ACTIVITY")) PayrollActivity=TRUE;
	}
	if(argc>4)
	{
		if(!RDAstrcmp(argv[4],"MULTIPLE_PERIODS")) Style=1;
		else if(!RDAstrcmp(argv[4],"SUM_CONTROL")) Summed=1;
		else if(!RDAstrcmp(argv[4],"ACTIVITY")) PayrollActivity=TRUE;
	}
	if(argc>5)
	{
		if(!RDAstrcmp(argv[5],"MULTIPLE_PERIODS")) Style=1;
		if(!RDAstrcmp(argv[5],"SUM_CONTROL")) Summed=1;
		else if(!RDAstrcmp(argv[5],"ACTIVITY")) PayrollActivity=TRUE;
	}
	if(argc>6)
	{
		if(!RDAstrcmp(argv[6],"MULTIPLE_PERIODS")) Style=1;
		if(!RDAstrcmp(argv[6],"SUM_CONTROL")) Summed=1;
		else if(!RDAstrcmp(argv[6],"ACTIVITY")) PayrollActivity=TRUE;
	}
	if(argc>7)
	{
		if(!RDAstrcmp(argv[7],"MULTIPLE_PERIODS")) Style=1;
		if(!RDAstrcmp(argv[7],"SUM_CONTROL")) Summed=1;
		else if(!RDAstrcmp(argv[7],"ACTIVITY")) PayrollActivity=TRUE;
	}
	TRACE;
	if(DefaultAccountType==2) GrantYear=(-1);
        finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,quit_record,MAINRSRC)==(-1)) return;
	if((fineact=APPOPNNRD(module,"FINEACT",TRUE,FALSE))==(-1)) return;
	if((finbact=APPOPNNRD(module,"FINBACT",TRUE,FALSE))==(-1)) return;
	if((filenum=APPOPNNRD(module,"FINALIAS",TRUE,FALSE))==(-1)) return;
	if((byrnum=APPOPNNRD(module,"FINBYR",TRUE,FALSE))==(-1)) return;
	if((eyrnum=APPOPNNRD(module,"FINEYR",TRUE,FALSE))==(-1)) return;
	if((ryrnum=APPOPNNRD(module,"FINRYR",TRUE,FALSE))==(-1)) return;
	if((yearnum=APPOPNNRD(module,"FINYEAR",TRUE,FALSE))==(-1)) return;
	if((expnum=APPOPNNRD(module,"FINEACC",TRUE,FALSE))==(-1)) return;
	if((revnum=APPOPNNRD(module,"FINRACC",TRUE,FALSE))==(-1)) return;
	if((balnum=APPOPNNRD(module,"FINBACC",TRUE,FALSE))==(-1)) return;
	if((finjen=APPOPNNRD(module,"FINJEN",TRUE,FALSE))==(-1)) return;
	if((finrec=APPOPNNRD(module,"FINREC",TRUE,FALSE))==(-1)) return;
	if(XPERT_SETUP->VENPMT)
	{
	if((venvch=APPOPNNRD("VENPMT","VENVCH",TRUE,FALSE))==(-1)) return;
	if((venfyr=APPOPNNRD("VENPMT","VENFYR",TRUE,FALSE))==(-1)) return;
	if((venmst=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->PURORD)
	{
	if((purtrn=APPOPNNRD("PURORD","PURTRN",TRUE,FALSE))==(-1)) return;
	if((purmst=APPOPNNRD("PURORD","PURMST",TRUE,FALSE))==(-1)) return;
	if((puracc=APPOPNNRD("PURORD","PURACC",TRUE,FALSE))==(-1)) return;
	if((purlin=APPOPNNRD("PURORD","PURLIN",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->PAYROLL)
	{
	if((payjtrn=APPOPNNRD("PAYROLL","PAYJTRN",TRUE,FALSE))==(-1)) return;
	if((paydtrn=APPOPNNRD("PAYROLL","PAYDTRN",TRUE,FALSE))==(-1)) return;
	if((payddsv=APPOPNNRD("PAYROLL","PAYDDSV",TRUE,FALSE))==(-1)) return;
	if((payddms=APPOPNNRD("PAYROLL","PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((payjpms=APPOPNNRD("PAYROLL","PAYJPMS",TRUE,FALSE))==(-1)) return;
	if((paymst=APPOPNNRD("PAYROLL","PAYMSTR",TRUE,FALSE))==(-1)) return;
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->INVCTL)
	{
		if((invadj=APPOPNNRD("INVCTL","INVADJ",TRUE,FALSE))==(-1)) return;
		if((invisst=APPOPNNRD("INVCTL","INVISST",TRUE,FALSE))==(-1)) return;
		if((invacc=APPOPNNRD("INVCTL","INVACC",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->BNKREC)
	{
	if((bnkchg=APPOPNNRD("BNKREC","BNKCHG",TRUE,FALSE))==(-1)) return;
	if((bnkrtn=APPOPNNRD("BNKREC","BNKRTN",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->PROPERTY)
	{
		if((prptrn=APPOPNNRD("PROPERTY","PRPTRN",TRUE,FALSE))==(-1)) return;
		if((prppay=APPOPNNRD("PROPERTY","PRPPAY",TRUE,FALSE))==(-1)) return;
		if((prpmst=APPOPNNRD("PROPERTY","PRPMST",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->RLSTMGT)
	{
		if((restran=APPOPNNRD("RLSTMGT","RESTRAN",TRUE,FALSE))==(-1)) return;
		if((restpay=APPOPNNRD("RLSTMGT","RESTPAID",TRUE,FALSE))==(-1)) return;
		if((restmst=APPOPNNRD("RLSTMGT","RESTMSTR",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->MISCBILL)
	{
		if((misctran=APPOPNNRD("MISCBILL","MISCTRAN",TRUE,FALSE))==(-1)) return;
		if((miscpaid=APPOPNNRD("MISCBILL","MISCPAY",TRUE,FALSE))==(-1)) return;
		if((miscmstr=APPOPNNRD("MISCBILL","MISCMSTR",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->OCCTAX)
	{
		if((occrec=APPOPNNRD("OCCTAX","OCCREC",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->FIXASS)
	{
		if((fixtran=APPOPNNRD("FIXASS","FIXTRAN",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->BLDPRMT)
	{
		if((prmttran=APPOPNNRD("BLDPRMT","PRMTTRAN",TRUE,FALSE))==(-1)) return;
		if((prmtpaid=APPOPNNRD("BLDPRMT","PRMTPAID",TRUE,FALSE))==(-1)) return;
		if((prmtmstr=APPOPNNRD("BLDPRMT","PRMTMSTR",TRUE,FALSE))==(-1)) return;
	}	
	if(XPERT_SETUP->UTLBLL)
	{
		if((utbtrn=APPOPNNRD("UTLBLL","UTBTRN",TRUE,FALSE))==(-1)) return;
		if((utbsvc=APPOPNNRD("UTLBLL","UTBSVC",TRUE,FALSE))==(-1)) return;
		if((utbcds=APPOPNNRD("UTLBLL","UTBCDS",TRUE,FALSE))==(-1)) return;
	}
	TRACE;
	CurrentRow=0;
	filename=FILENAME(filenum);
	nokeys=NUMKEYS(filenum);
	keys=KEYPOINTER(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	sortname=unique_name();
	dbsrt=DBsortNEW("FINMGT",sortname,NRDMyIsamEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsrt);
	dbsrt->fileno=OPNDBsort(dbsrt);
	MAINRSRC=RDArsrcNEW(module,"DETAIL QUERY");
	addDFincvir(MAINRSRC,module,"FINBACC",NULL,balnum);
	addDFincvir(MAINRSRC,module,"FINRACC",NULL,revnum);
	addDFincvir(MAINRSRC,module,"FINEACC",NULL,expnum);
	addDFincvir(MAINRSRC,module,"FINRYR",NULL,ryrnum);
	addDFincvir(MAINRSRC,module,"FINBYR",NULL,byrnum);
	addDFincvir(MAINRSRC,module,"FINEYR",NULL,eyrnum);
	addDFincvir(MAINRSRC,module,"FINYEAR",NULL,yearnum);
	addDFincvir(MAINRSRC,module,"FINALIAS",NULL,filenum);
	addDFincvir(MAINRSRC,module,"FINJEN",NULL,finjen);
	addDFincvir(MAINRSRC,module,"FINREC",NULL,finrec);
	addDFincvir(MAINRSRC,module,"FINBACT",NULL,finbact);
	addDFincvir(MAINRSRC,module,"FINEACT",NULL,fineact);
	addDFincvir(MAINRSRC,"VENPMT","VENVCH",NULL,venvch);
	addDFincvir(MAINRSRC,"VENPMT","VENMSR",NULL,venmst);
	addDFincvir(MAINRSRC,"VENPMT","VENFYR",NULL,venfyr);
	addDFincvir(MAINRSRC,"PURORD","PURTRN",NULL,purtrn);
	addDFincvir(MAINRSRC,"PURORD","PURACC",NULL,puracc);
	addDFincvir(MAINRSRC,"PURORD","PURLIN",NULL,purlin);
	addDFincvir(MAINRSRC,"PURORD","PURMST",NULL,purmst);
	addDFincvir(MAINRSRC,"PAYROLL","PAYMSTR",NULL,paymst);
	addDFincvir(MAINRSRC,"PAYROLL","PAYJPMS",NULL,payjpms);
	addDFincvir(MAINRSRC,"PAYROLL","PAYJTRN",NULL,payjtrn);
	addDFincvir(MAINRSRC,"PAYROLL","PAYDTRN",NULL,paydtrn);
	addDFincvir(MAINRSRC,"PAYROLL","PAYDDSV",NULL,payddsv);
	addDFincvir(MAINRSRC,"BNKREC","BNKCHG",NULL,bnkchg);
	addDFincvir(MAINRSRC,"BNKREC","BNKRTN",NULL,bnkrtn);
	addDFincvir(MAINRSRC,"INVCTL","INVADJ",NULL,invadj);
	addDFincvir(MAINRSRC,"INVCTL","INVACC",NULL,invacc);
	addDFincvir(MAINRSRC,"INVCTL","INVISST",NULL,invisst);
	addDFincvir(MAINRSRC,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(MAINRSRC,"UTLBLL","UTBSVC",NULL,utbsvc);
	addDFincvir(MAINRSRC,"UTLBLL","UTBTRN",NULL,utbtrn);
	addDFincvir(MAINRSRC,"MISCBILL","MISCTRAN",NULL,misctran);
	addDFincvir(MAINRSRC,"MISCBILL","MISCPAY",NULL,miscpaid);
	addDFincvir(MAINRSRC,"MISCBILL","MISCMSTR",NULL,miscmstr);
	addDFincvir(MAINRSRC,"PROPERTY","PRPTRN",NULL,prptrn);
	addDFincvir(MAINRSRC,"PROPERTY","PRPPAY",NULL,prppay);
	addDFincvir(MAINRSRC,"PROPERTY","PRPMST",NULL,prpmst);
	addDFincvir(MAINRSRC,"RLSTMGT","RESTRAN",NULL,restran);
	addDFincvir(MAINRSRC,"RLSTMGT","RESTPAID",NULL,restpay);
	addDFincvir(MAINRSRC,"RLSTMGT","RESTMSTR",NULL,restmst);
	addDFincvir(MAINRSRC,"BLDPRMT","PRMTMSTR",NULL,prmtmstr);
	addDFincvir(MAINRSRC,"BLDPRMT","PRMTTRAN",NULL,prmttran);
	addDFincvir(MAINRSRC,"BLDPRMT","PRMTPAID",NULL,prmtpaid);

	d=getDFvirtual("PAYROLL","NAME FIRST MIDDLE LAST LINEAGE");
	if(d!=NULL)
	{
		EmployeeName=Rmalloc(sizeof(RDAvirtual));
		EmployeeName->name=stralloc(d->name);
		EmployeeName->when=0;
		EmployeeName->len=d->length;
		EmployeeName->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeName->expression=PP_translate(d->expression);
		} else {
			EmployeeName->expression=NULL;
		}
		EmployeeName->computed=FALSE;
		EmployeeName->range=FALSE;
		EmployeeName->dlen=0;
		switch(EmployeeName->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				EmployeeName->value.string_value=NULL;
				EmployeeName->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeName->value.string_value=Rmalloc(1);
				*EmployeeName->value.string_value=0;
				EmployeeName->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeName->value.float_value=Rmalloc(sizeof(double));
				*EmployeeName->value.float_value=0;
				EmployeeName->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeName->value.short_value=Rmalloc(sizeof(short));
				*EmployeeName->value.short_value=0;
				EmployeeName->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeName->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeName->value.integer_value=0;
				EmployeeName->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeName->type,EmployeeName->name);
				break;
		}
		FreeDFvirtual(d);
	}
	TRACE;
	GET_SCREEN_VIRTUALS(MAINRSRC,0);
	presentyear=getyears(MAINRSRC);
	if(yearavl->numlibs<1)
	{
		ERRORDIALOG("FINYEAR Empty File","The FINYEAR file is an empty file.  There must exist at least 1 year on file to process journal entries.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		quit_record(MAINRSRC);
		return;
	}
	addlstrsrc(MAINRSRC,"YEARS AVAILABLE",&presentyear,TRUE,changeyear,yearavl->numlibs,&yearavl->libs,NULL);
	pmonth=APPlibNEW();
	addAPPlib(pmonth,"PRE-YEAR");
	addAPPlib(pmonth,"JANUARY");
	addAPPlib(pmonth,"FEBRUARY");
	addAPPlib(pmonth,"MARCH");
	addAPPlib(pmonth,"APRIL");
	addAPPlib(pmonth,"MAY");
	addAPPlib(pmonth,"JUNE");
	addAPPlib(pmonth,"JULY");
	addAPPlib(pmonth,"AUGUST");
	addAPPlib(pmonth,"SEPTEMBER");
	addAPPlib(pmonth,"OCTOBER");
	addAPPlib(pmonth,"NOVEMBER");
	addAPPlib(pmonth,"DECEMBER");
	addAPPlib(pmonth,"POST YEAR");
	start=dm;
	addlstrsrc(MAINRSRC,"PROCESSING MONTHS",&start,TRUE,getacctdesc,pmonth->numlibs,&pmonth->libs,NULL);
	if(Style==1)
	{
		FINDRSCSETSENSITIVE(MAINRSRC,"PROCESSING MONTHS",FALSE);
	}
	if(isEMPTY(AFFECT_DATE))
	{
		temp=GETCURRENTDATE10();
		AFFECTDate=stralloc(temp);
	} else {
		AFFECTDate=stralloc(AFFECT_DATE);
		FromAFFECTDate=stralloc(AFFECT_DATE);
	}
	addstdrsrc(MAINRSRC,"FROM AFFECT DATE",DATES,10,FromAFFECTDate,TRUE);
	FINDRSCSETFUNC(MAINRSRC,"FROM AFFECT DATE",changeFromAFFECTdate,NULL);
	addstdrsrc(MAINRSRC,"AFFECT DATE",DATES,10,AFFECTDate,TRUE);
	FINDRSCSETFUNC(MAINRSRC,"AFFECT DATE",changeAFFECTdate,NULL);
	addstdrsrc(MAINRSRC,"TOTAL DEBITS",DOLLARS,20,NULL,FALSE);
	addstdrsrc(MAINRSRC,"TOTAL CREDITS",DOLLARS,20,NULL,FALSE);
	addstdrsrc(MAINRSRC,"GRANT YEAR",LONGV,4,&GrantYear,TRUE);
	addstdrsrc(MAINRSRC,"SECONDS DIFFERENTIAL",LONGV,4,&SECONDS_DIFFERENTIAL,TRUE);
	addstdrsrc(MAINRSRC,"BALANCE",DOLLARS,20,NULL,FALSE);
	addstdrsrc(MAINRSRC,"CURRENT PAGE",LONGV,8,&CurrentPage,TRUE);
	addstdrsrc(MAINRSRC,"SELECT DISTRIBUTED",BOOLNS,1,&SELECT_DISTRIBUTED,TRUE);
	addstdrsrc(MAINRSRC,"SELECT UNDISTRIBUTED",BOOLNS,1,&SELECT_UNDISTRIBUTED,TRUE);
	FINDRSCSETFUNC(MAINRSRC,"SECONDS DIFFERENTIAL",getsecdiff,NULL);
	FINDRSCSETFUNC(MAINRSRC,"SELECT DISTRIBUTED",getacctdesc,NULL);
	FINDRSCSETFUNC(MAINRSRC,"SELECT UNDISTRIBUTED",getacctdesc,NULL);
	FINDRSCSETFUNC(MAINRSRC,"CURRENT PAGE",changeCurrentPage,NULL);
	addstdrsrc(MAINRSRC,"TOTAL PAGES",LONGV,8,&TotalPages,FALSE);
	TRACE;
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				if(DefaultAccountType!=(-1)) start=DefaultAccountType;
				else start=0;
				ACCTtypes=APPlibNEW();
				addAPPlib(ACCTtypes,accttypes[0]);
				addAPPlib(ACCTtypes,accttypes[1]);
				addAPPlib(ACCTtypes,accttypes[2]);
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addlstrsrc(MAINRSRC,"ACCOUNT TYPES",
						&start,TRUE,ACCTtypecb,
						ACCTtypes->numlibs,
						&ACCTtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(MAINRSRC,
							"ACCOUNT TYPES",
							(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					DEFtypes=APPlibNEW();
					start=0;
					addlstrsrc(MAINRSRC,"DEFINITION TYPES",
						&start,TRUE,DEFtypecb,
						DEFtypes->numlibs,
						&DEFtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(MAINRSRC,
							"DEFINITION TYPES",
							(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"ACCOUNT CODE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					acctavl=APPlibNEW();
					start=0;
					addlstrsrc(MAINRSRC,"ACCOUNT CODES",
						&start,TRUE,
						getacctdesc,acctavl->numlibs,
						&acctavl->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(MAINRSRC,"ACCOUNT CODES",FALSE);
					addstdrsrc(MAINRSRC,"ACCOUNT DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
					addstdrsrc(MAINRSRC,"ACCOUNT CODE SEARCH",EXPENDITURE,40,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(MAINRSRC,"ACCOUNT CODE SEARCH",acctsearch,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(MAINRSRC,filenum,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				}
				sprintf(name,"[%s][%s]",filename,fldx->name);
				FINDRSCSETFUNC(MAINRSRC,name,setfiles,NULL);
			}
		}
	}
	nofields=NUMFLDS(dbsrt->fileno);
	fields=FLDPOINTER(dbsrt->fileno);
	for(y=1;y<(NumberRows+1);++y)
	{
		if(fields!=NULL)
		{
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				memset(DQUERYstemp,0,101);
				sprintf(DQUERYstemp,"%08d:%s",y,fldx->name);
				if(!alreadyrsrc(DQUERYstemp,MAINRSRC))
				{
					makefieldrsrc(MAINRSRC,DQUERYstemp,fldx->type,
						fldx->len,FALSE);
				}
			}
			memset(DQUERYstemp,0,101);
			sprintf(DQUERYstemp,"%08d:SOURCE DESCRIPTION",y);
			if(!alreadyrsrc(DQUERYstemp,MAINRSRC))
			{
				makefieldrsrc(MAINRSRC,DQUERYstemp,PLAINTEXT,25,FALSE);
			}
		}
	}
	TRACE;
	getsupportingrecords(MAINRSRC,FALSE);
	SetVirFieldFuncs(MAINRSRC);
	SCRNvirtual2rsrc(MAINRSRC);
	addbtnrsrc(MAINRSRC,"CSV",TRUE,csv_dump,NULL);
	addbtnrsrc(MAINRSRC,"PREVIOUS PAGE",TRUE,PreviousPage,NULL);
	addbtnrsrc(MAINRSRC,"NEXT PAGE",TRUE,NextPage,NULL);
	addbtnrsrc(MAINRSRC,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(MAINRSRC,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addbtnrsrc(MAINRSRC,"SELECT",TRUE,GetData,NULL);
	addbtnrsrc(MAINRSRC,"QUIT",TRUE,quit_record,NULL);
	addrfcbrsrc(MAINRSRC,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(MAINRSRC,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(MAINRSRC);
	TRACE;
	CustomTableFunction=dQueryCustomTableFunction;
	TRACE;
	APPmakescrn(MAINRSRC,TRUE,quit_record_free,NULL,TRUE);
	TRACE;
	FINDRSCGETINT(MAINRSRC,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(MAINRSRC,"DEFINITION TYPES",&selectedd);
	accttype=(short)selecteda;
	deftype=(short)selectedd;
	FINDRSCSETACCTTYPE(MAINRSRC,"ACCOUNT CODE SEARCH",accttype);
	FINDRSCSETDEFTYPE(MAINRSRC,"ACCOUNT CODE SEARCH",deftype);
	DID_STUFF=TRUE;
	TRACE;
	RDAAPPMAINLOOP();
	TRACE;
}
