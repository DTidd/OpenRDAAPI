/* finarec.c - Financial Management Add Receipts */
#define CLEARWDGT
#ifndef WIN32
#define __NAM__ "dquery1.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dquery1.exe"
#endif
#include <app.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <guip.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <fin.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char SWB_FM25_AFFECT_DATE=FALSE;
static short Style=0;
static double TOTAL_DEBITS=0.0,TOTAL_CREDITS=0.0;
static char *AccountCode=NULL,*AFFECTDate=NULL;
static short filenum=(-1),yearnum=(-1),expnum=(-1),revnum=(-1),balnum=(-1);
static short byrnum=(-1),eyrnum=(-1),ryrnum=(-1),venfyr=(-1),puracc=(-1);
static short finjen=(-1),finrec=(-1),venvch=(-1),purtrn=(-1),purmst=(-1);
static short payjtrn=(-1),paydtrn=(-1),payddsv=(-1),paymst=(-1);
static short invadj=(-1),invisst=(-1),bnkchg=(-1),bnkrtn=(-1);
static short purlin=(-1),payddms=(-1),payjpms=(-1);
short prptrn=(-1),prppay=(-1),restran=(-1),restpay=(-1),utbtrn=(-1);
short msctrn=(-1),mscpay=(-1),occrec=(-1),prmttran=(-1),prmtpaid=(-1);
static RDAfinmgt *finsetup=NULL;
static APPlib *ACCTtypes=NULL,*DEFtypes=NULL;
static APPlib *yearavl=NULL,*acctavl=NULL;
static APPlib *journalavl=NULL,*pmonth=NULL;
static char *module="FINMGT",ADCFP=FALSE;
static int FiscalYear=(-1),NumberRows=100,CurrentRow=0,dm=0;
static int NumberColumns=14,CurrentPage=1,TotalPages=1;
static int AccountType=(-1),DefinitionType=(-1),NumRecs=0;
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
static RDArsrc *MAINRSRC=NULL;
static DBsort *dbsrt=NULL;
static RDAvirtual *csortsequence=NULL;
qt_QTable *MainTable=NULL;
void ClearTableResources(void);

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
*/

void dQueryCustomTableFunction(qt_QWidget *parent,qt_QWidget **pop)
{
	qt_QKeySequence *HAKS=NULL;
	qt_QAccel *KA=NULL;
	qt_QSizePolicy *mySizePolicy=NULL;
	qt_Slot *CL=NULL;
	qt_QHeader *H=NULL;
	qt_QString *tmpstr=NULL;
	int crow=0,section=0,main_width=0,r=0;
	RDArmem *mem=NULL;

	if(diaggui || diaggui_field)
	{
		prterr("DIAG dQueryCustomTableFunction.");
	}
	*pop=(qt_QWidget *)qt_new_QTable((qt_QWidget *)parent,"dQueryTable");
	qt_QTable_setColumnMovingEnabled((qt_QTable *)*pop,TRUE);
	qt_QTable_setRowMovingEnabled((qt_QTable *)*pop,FALSE);
	qt_QTable_setSelectionMode((qt_QTable *)*pop,QTABLE_Multi);
	mySizePolicy=qt_new_QSizePolicy1(QSIZEPOLICY_Minimum,QSIZEPOLICY_Minimum,FALSE);
	qt_QSizePolicy_setHorStretch((qt_QSizePolicy *)mySizePolicy,100);
	qt_QSizePolicy_setVerStretch((qt_QSizePolicy *)mySizePolicy,100);
	qt_QWidget_setSizePolicy((qt_QWidget *)*pop,mySizePolicy);
	qt_del_QSizePolicy(mySizePolicy);
	qt_QTable_setShowGrid((qt_QTable *)*pop,TRUE);
	SetupWindowColorPalette(*pop,MAINRSRC->screen);
	HAKS=qt_new_QKeySequence2(QT_ALT+QT_Key_V);
	KA=qt_new_QAccel1((qt_QWidget *)*pop,(qt_QObject *)MAINRSRC->swidget,"Paste Accelerator");
	CL=qt_new_UserDataSlot(TablePasted,*pop);
	qt_QAccel_insertItem(KA,HAKS,98);
	qt_QAccel_connectItem(KA,98,(qt_QObject *)CL,"1UserDataSlot()");
	qt_QAccel_setEnabled(KA,TRUE);
	qt_del_QKeySequence(HAKS);
	HAKS=qt_new_QKeySequence2(QT_ALT+QT_Key_C);
	KA=qt_new_QAccel1((qt_QWidget *)*pop,(qt_QObject *)MAINRSRC->swidget,"Copy Accelerator");
	CL=qt_new_UserDataSlot(TableCopied,*pop);
	qt_QAccel_insertItem(KA,HAKS,98);
	qt_QAccel_connectItem(KA,98,(qt_QObject *)CL,"1UserDataSlot()");
	qt_QAccel_setEnabled(KA,TRUE);
	qt_del_QKeySequence(HAKS);
/* Determine Rows & Columns */
	qt_QTable_setNumRows((qt_QTable *)*pop,NumberRows);
	qt_QTable_setNumCols((qt_QTable *)*pop,NumberColumns);
	H=qt_QTable_horizontalHeader((qt_QTable *)*pop);
	qt_QHeader_setResizeEnabled((qt_QHeader *)H,TRUE,-1);
	qt_QHeader_setClickEnabled((qt_QHeader *)H,TRUE,-1);
	qt_QHeader_setOrientation((qt_QHeader *)H,QT_Horizontal);
	SetupWindowColorPalette((qt_QWidget *)H,MAINRSRC->screen);
	section=0;
	if(SWB_FM25_AFFECT_DATE==TRUE)
	{
		tmpstr=qt_new_QString5("Affect Date");
	} else {
		tmpstr=qt_new_QString5("Date");
	}
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Control");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Debit Amount");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Credit Amount");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Reference");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Description");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Identification");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Entry Date");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Dist/Clear");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Calendar Year");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Source User");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Trans#");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Acc#");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	tmpstr=qt_new_QString5("Source");
	qt_QHeader_setLabel((qt_QHeader *)H,section,tmpstr,-1);
	qt_del_QString(tmpstr);
	++section;
	for(crow=1;crow<(NumberRows+1);++crow)
	{
		section=0;
		memset(stemp,0,101);
		sprintf(stemp,"%08d:AFFECT DATE",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,10,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:CONTROL",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,15,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:DEBIT AMOUNT",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,20,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:CREDIT AMOUNT",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,20,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:REFERENCE",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,15,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:DESCRIPTION",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,40,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:IDENTIFICATION",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,15,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:ENTRY DATE",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,10,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:DISTRIBUTED",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem," ",NULL,0,0,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:CALENDAR YEAR",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,4,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;

		memset(stemp,0,101);
		sprintf(stemp,"%08d:SOURCE USER",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,15,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;


		memset(stemp,0,101);
		sprintf(stemp,"%08d:TRANSACTION NUMBER",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,8,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;


		memset(stemp,0,101);
		sprintf(stemp,"%08d:ACCOUNT NUMBER",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,4,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;


		memset(stemp,0,101);
		sprintf(stemp,"%08d:SOURCE",crow);
		r=FINDRSC(MAINRSRC,stemp);
		if(r>(-1))
		{
			mem=MAINRSRC->rscs+r;
			makefield((qt_QWidget *)*pop,mem,stemp,NULL,0,2,0,(qt_QTable *)*pop,crow-1,section,0,0);
		}
		++section;
	}
	main_width=qt_QScrollView_contentsWidth((qt_QScrollView *)*pop);
	qt_QHeader_adjustHeaderSize((qt_QHeader *)H);
	MainTable=(qt_QTable *)*pop;
}
void DisplayData()
{
	short ef=0,src=0;
	int count=0,cy=0,trans=0,acctnum=0,display_count=0;
	char *adate=NULL,*ref=NULL,*c=NULL,*desc=NULL,*su=NULL,*id=NULL;
	char *edate=NULL,d=FALSE,delflag=FALSE;
	double deb=0.0,cred=0.0;

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
			memset(stemp,0,101);
			sprintf(stemp,"%08d:AFFECT DATE",display_count);
			FINDRSCSETSTRING(MAINRSRC,stemp,adate);
			FINDFLDGETDOUBLE(dbsrt->fileno,"DEBIT AMOUNT",&deb);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:DEBIT AMOUNT",display_count);
			FINDRSCSETDOUBLE(MAINRSRC,stemp,deb);
			FINDFLDGETDOUBLE(dbsrt->fileno,"CREDIT AMOUNT",&cred);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:CREDIT AMOUNT",display_count);
			FINDRSCSETDOUBLE(MAINRSRC,stemp,cred);
			FINDFLDGETSTRING(dbsrt->fileno,"REFERENCE",&ref);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:REFERENCE",display_count);
			FINDRSCSETSTRING(MAINRSRC,stemp,ref);
			FINDFLDGETSTRING(dbsrt->fileno,"DESCRIPTION",&desc);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:DESCRIPTION",display_count);
			FINDRSCSETSTRING(MAINRSRC,stemp,desc);
			FINDFLDGETSTRING(dbsrt->fileno,"IDENTIFICATION",&id);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:IDENTIFICATION",display_count);
			FINDRSCSETSTRING(MAINRSRC,stemp,id);
			FINDFLDGETSTRING(dbsrt->fileno,"CONTROL",&c);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:CONTROL",display_count);
			FINDRSCSETSTRING(MAINRSRC,stemp,c);
			FINDFLDGETSTRING(dbsrt->fileno,"ENTRY DATE",&edate);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:ENTRY DATE",display_count);
			FINDRSCSETSTRING(MAINRSRC,stemp,edate);
			FINDFLDGETCHAR(dbsrt->fileno,"DISTRIBUTED",&d);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:DISTRIBUTED",display_count);
			FINDRSCSETCHAR(MAINRSRC,stemp,d);
			FINDFLDGETINT(dbsrt->fileno,"CALENDAR YEAR",&cy);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:CALENDAR YEAR",display_count);
			FINDRSCSETINT(MAINRSRC,stemp,cy);
			FINDFLDGETSTRING(dbsrt->fileno,"SOURCE USER",&su);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:SOURCE USER",display_count);
			FINDRSCSETSTRING(MAINRSRC,stemp,su);
			FINDFLDGETINT(dbsrt->fileno,"TRANSACTION NUMBER",&trans);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:TRANSACTION NUMBER",display_count);
			FINDRSCSETINT(MAINRSRC,stemp,trans);
			FINDFLDGETINT(dbsrt->fileno,"ACCOUNT NUMBER",&acctnum);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:ACCOUNT NUMBER",display_count);
			FINDRSCSETINT(MAINRSRC,stemp,acctnum);
			FINDFLDGETSHORT(dbsrt->fileno,"SOURCE",&src);
			memset(stemp,0,101);
			sprintf(stemp,"%08d:SOURCE",display_count);
			FINDRSCSETSHORT(MAINRSRC,stemp,src);
		}
		}
		ef=NXTNRD(dbsrt->fileno,1);
	}
	if(adate!=NULL) Rfree(adate);
	if(edate!=NULL) Rfree(edate);
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
	if(Style==0)
	{
		addDBfield(dbsrt,"NUMBER",LONGV,8);
		addDBkeypart(dbsrt,keyno,"NUMBER");
	}
	addDBfield(dbsrt,"IDENTIFICATION",PLAINTEXT,15);
	addDBfield(dbsrt,"TRANSACTION NUMBER",LONGV,8);
	addDBfield(dbsrt,"ACCOUNT NUMBER",LONGV,4);
	addDBfield(dbsrt,"CALENDAR YEAR",LONGV,4);
	addDBfield(dbsrt,"REFERENCE",PLAINTEXT,40);
	addDBfield(dbsrt,"DESCRIPTION",PLAINTEXT,40);
	addDBfield(dbsrt,"SOURCE USER",PLAINTEXT,15);
	addDBfield(dbsrt,"AFFECT DATE",DATES,10);
	addDBfield(dbsrt,"ENTRY DATE",DATES,10);
	addDBfield(dbsrt,"DEBIT AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"CREDIT AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"DISTRIBUTED",BOOLNS,1);
	addDBfield(dbsrt,"SOURCE",SHORTV,2);
}
void addsortrecord(DBsort *dbsrt,short src,char *AffectDate,char *id,
	char *refid,int transnum,int accnum,int calyear,
	char *control,char *desc,char *su,
	char *edate,double debit,double credit,char d)
{
	char deleteflag=FALSE;
	double amt=0.0;

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
	memset(stemp,0,101);
	sprintf(stemp,"%.4s-%.2s-%.2s",&AffectDate[6],&AffectDate[0],&AffectDate[3]);
	FINDFLDSETSTRING(dbsrt->fileno,"DATE AFFECTED",stemp);
	FINDFLDSETSTRING(dbsrt->fileno,"CONTROL",control);
	if(Style==1)
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
	if(Style==0)
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
	FINDFLDSETCHAR(dbsrt->fileno,"DISTRIBUTED",d);
	TOTAL_DEBITS+=debit;
	TOTAL_CREDITS+=credit;
	if(Style==0)
	{
		WRTNRD(dbsrt->fileno,TRUE,"NUMBER");
		++NumRecs;
	} else {
		WRTNRD(dbsrt->fileno,0,NULL);
	}
}
void GatherData()
{
	int fy=0,PM=0,trans=0,accnum=0,cy=0,checknum=0;
	char delflag=FALSE,read_record=FALSE;
	char *venid1=NULL,*venid2=NULL,*payid1=NULL,*payid2=NULL,*id=NULL;
	short ef=0,ef2=0,pm=0,acctype=0,deftype=0;
	char *acctcode=NULL,*debcode=NULL,*credcode=NULL;
	char *duefrom=NULL,*clearcash=NULL,*payable=NULL;
	char *adate=NULL,*ref=NULL,*c=NULL,*desc=NULL,*su=NULL;
	char *edate=NULL,d=FALSE,*date_affected=NULL;
	double deb=0.0,cred=0.0,amt=0.0;

	NumRecs=0;
	if(isEMPTY(AccountCode)) return;
	memset(stemp,0,101);
	sprintf(stemp,"%.4s-%.2s-%.2s",&AFFECTDate[6],&AFFECTDate[0],&AFFECTDate[3]);
	date_affected=stralloc(stemp);
	FINDRSCGETINT(MAINRSRC,"PROCESSING MONTHS",&PM);

	ZERNRD(finjen);
	FINDFLDSETINT(finjen,"FISCAL YEAR",FiscalYear);
	ef=ADVGTENRDsec(finjen,2,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		FINDFLDGETINT(finjen,"FISCAL YEAR",&fy);
		if(fy!=FiscalYear) break;
		read_record=FALSE;
		FINDFLDGETCHAR(finjen,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSHORT(finjen,"PROCESSING MONTH",&pm);
			if(pm==PM)
			{
				FINDFLDGETSHORT(finjen,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(finjen,"DEFINITION TYPE",&deftype);
				if(SWB_FM25_AFFECT_DATE==TRUE)
				{
					FINDFLDGETSTRING(finjen,"AFFECT DATE",&adate);
				} else {
					FINDFLDGETSTRING(finjen,"DISTRIBUTED DATE",&adate);
				}
				memset(stemp,0,101);
				sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && ((RDAstrcmp(stemp,date_affected))<=0)) 
				{
					read_record=FALSE;
					if((AccountType==2) || (acctype==AccountType)) 
					{
						if(AccountType<2) 
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
					if(read_record==TRUE)
					{
						FINDFLDGETSTRING(finjen,"JOURNAL IDENTIFICATION",&id);
						FINDFLDGETSTRING(finjen,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(finjen,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(finjen,"SOURCE USER",&su);
						FINDFLDGETSTRING(finjen,"CONTROL",&c);
						FINDFLDGETSTRING(finjen,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(finjen,"TRANSACTION NUMBER",&trans);
						FINDFLDGETCHAR(finjen,"DISTRIBUTED",&d);
						FINDFLDGETDOUBLE(finjen,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) deb=amt;
						else if(AccountType==0) cred=amt;
						else if(AccountType==1 && amt>=0.0) cred=amt;
						else if(AccountType==1) deb=amt;
						else if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) deb=amt;
						else if(AccountType==2) cred=amt;
						addsortrecord(dbsrt,0,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
					}
				}
			}
		}
		ef=ADVNXTNRDsec(finjen,2,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(finjen);
	ZERNRD(finrec);
	FINDFLDSETINT(finrec,"FISCAL YEAR",FiscalYear);
	ef=ADVGTENRDsec(finrec,2,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		FINDFLDGETINT(finrec,"FISCAL YEAR",&fy);
		if(fy!=FiscalYear) break;
		FINDFLDGETCHAR(finrec,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSHORT(finrec,"PROCESSING MONTH",&pm);
			if(pm==PM)
			{
				FINDFLDGETSHORT(finrec,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(finrec,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(finrec,"DEPOSIT DATE",&adate);
				memset(stemp,0,101);
				sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && ((RDAstrcmp(stemp,date_affected))<=0)) 
				{
					read_record=FALSE;
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
					if(read_record==TRUE)
					{
						FINDFLDGETSTRING(finrec,"JOURNAL IDENTIFICATION",&id);
						FINDFLDGETSTRING(finrec,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(finrec,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(finrec,"SOURCE USER",&su);
						FINDFLDGETSTRING(finrec,"CONTROL",&c);
						FINDFLDGETSTRING(finrec,"REFERENCE IDENTIFICATION",&ref);
						FINDFLDGETINT(finrec,"TRANSACTION NUMBER",&trans);
						FINDFLDGETCHAR(finrec,"DISTRIBUTED",&d);
						FINDFLDGETDOUBLE(finrec,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) cred=amt;
						else if(AccountType==0) deb=amt;
						else if(AccountType==1 && amt>=0.0) cred=amt;
						else if(AccountType==1) deb=amt;
						else if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) deb=amt;
						else if(AccountType==2) cred=amt;
						addsortrecord(dbsrt,1,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
					}
				}
			}
		}
		ef=ADVNXTNRDsec(finrec,2,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(finrec);
	ZERNRD(venfyr);
	FINDFLDSETINT(venfyr,"FISCAL YEAR",FiscalYear);
	ef=ADVGTENRDsec(venfyr,2,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
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
					FINDFLDGETSHORT(venvch,"PROCESSING MONTH",&pm);
					FINDFLDGETINT(venvch,"FISCAL YEAR",&fy);
					FINDFLDGETCHAR(venvch,"ELECTRONIC TRANSFER",&d);
					if(d)
					{
						FINDFLDGETSTRING(venvch,"ELECTRONIC TRANSFER DATE",&adate);
					} else FINDFLDGETSTRING(venvch,"CHECK DATE",&adate);
					memset(stemp,0,101);
					sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
					if(pm==PM && fy==FiscalYear && ((RDAstrcmp(stemp,date_affected))<=0))
					{
						FINDFLDGETSHORT(venvch,"ACCOUNT TYPE",&acctype);
						FINDFLDGETSHORT(venvch,"DEFINITION TYPE",&deftype);
						if(DefinitionType==deftype)
						{
							read_record=FALSE;
							if((AccountType==2) || (acctype==AccountType)) 
							{
								if(AccountType<2) 
								{
									FINDFLDGETSTRING(venvch,"ACCOUNT CODE",&acctcode);
									if(!RDAstrcmp(acctcode,AccountCode)) read_record=TRUE;
								}
/* Note:  Deal with Clearing Checks */
								if(AccountType==2)
								{
									FINDFLDGETSTRING(venvch,"DEBIT CODE",&debcode);
									FINDFLDGETSTRING(venvch,"CREDIT CODE",&credcode);
									FINDFLDGETSTRING(venvch,"PAYABLE CODE",&payable);
									FINDFLDGETSTRING(venvch,"CLEARING DUE FROM",&duefrom);
									FINDFLDGETSTRING(venvch,"CLEARING CASH",&clearcash);
									if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode) || !RDAstrcmp(payable,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(clearcash,AccountCode)) read_record=TRUE;
								}
							}
							if(read_record==TRUE)
							{
								FINDFLDGETSTRING(venvch,"VENDOR IDENTIFICATION",&id);
								FINDFLDGETSTRING(venvch,"ENTRY DATE",&edate);
								FINDFLDGETSTRING(venvch,"DESCRIPTION",&desc);
								FINDFLDGETSTRING(venvch,"SOURCE USER",&su);
								FINDFLDGETINT(venvch,"CHECK NUMBER",&checknum);
								if(c!=NULL) Rfree(c);
								memset(stemp,0,101);
								if(checknum!=0)
								{
									sprintf(stemp,"%d",checknum);
								} else {
									FINDFLDGETINT(venvch,"ELECTRONIC TRANSFER NUMBER",&checknum);
									sprintf(stemp,"%d",checknum);
								}
								c=stralloc(stemp);	
								FINDFLDGETSTRING(venvch,"INVOICE IDENTIFICATION",&ref);
								FINDFLDGETINT(venvch,"TRANSACTION NUMBER",&trans);
								if(!RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(clearcash,AccountCode))
								{
									FINDFLDGETCHAR(venvch,"CLEARED",&d);
								} else FINDFLDGETCHAR(venvch,"DISTRIBUTED",&d);
								FINDFLDGETDOUBLE(venvch,"AMOUNT",&amt);
								FINDFLDGETINT(venvch,"CALENDAR YEAR",&cy);
								deb=0.0;cred=0.0;
								if(AccountType==0 && amt>=0.0) deb=amt;
								else if(AccountType==0) cred=amt;
								else if(AccountType==1 && amt>=0.0) cred=amt;
								else if(AccountType==1) deb=amt;
								else if(AccountType==2)
								{
									if(!RDAstrcmp(debcode,AccountCode) && debcode!=NULL) deb=amt; 
									else if(!RDAstrcmp(duefrom,AccountCode) && duefrom!=NULL) deb=amt;
									else if(!RDAstrcmp(payable,AccountCode) && payable!=NULL)
									{
										deb=amt;
										cred=amt;
									} else if(!RDAstrcmp(credcode,AccountCode) && credcode!=NULL) cred=amt;
									else if(!RDAstrcmp(clearcash,AccountCode) && clearcash!=NULL) cred=amt;
								}
								addsortrecord(dbsrt,2,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
							}
						}
					}
				}
				ef2=ADVNXTNRDsec(venvch,1,SCRNvirtualSubData,MAINRSRC);
			}
		}
		ef=ADVNXTNRDsec(venfyr,2,SCRNvirtualSubData,MAINRSRC);
	}
	if(venid1!=NULL) Rfree(venid1);
	if(venid2!=NULL) Rfree(venid2);
	ZERNRD(venfyr);
	ZERNRD(venvch);

#ifndef __NO_ENCUMBRANCES__
	if(AccountType==2)
	{
#endif
	ZERNRD(purtrn);
	FINDFLDSETINT(purtrn,"FISCAL YEAR",FiscalYear);
	ef=ADVGTENRDsec(purtrn,1,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		read_record=FALSE;
		FINDFLDGETINT(purtrn,"FISCAL YEAR",&fy);
		if(fy!=FiscalYear) break;
		FINDFLDGETCHAR(purtrn,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSHORT(purtrn,"PROCESSING MONTH",&pm);
			if(pm==PM)
			{
				ZERNRD(puracc);
				COPYFIELD(purtrn,puracc,"FISCAL YEAR");
				COPYFIELD(purtrn,puracc,"PO IDENTIFICATION");
				COPYFIELD(purtrn,puracc,"LINE NUMBER");
				COPYFIELD(purtrn,puracc,"ACCOUNT NUMBER");
				if(ADVEQLNRDsec(puracc,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(puracc,1);
				FINDFLDGETSHORT(puracc,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(puracc,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(purtrn,"DISTRIBUTED DATE",&adate);
				memset(stemp,0,101);
				sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && ((RDAstrcmp(stemp,date_affected))<=0)) 
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
					if(read_record==TRUE)
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
						FINDFLDGETCHAR(purtrn,"DISTRIBUTED",&d);
						FINDFLDGETDOUBLE(purtrn,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) deb=amt;
						else if(AccountType==0) cred=amt;
						else if(AccountType==1 && amt>=0.0) cred=amt;
						else if(AccountType==1) deb=amt;
						else if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) deb=amt;
						else if(AccountType==2) cred=amt;
						addsortrecord(dbsrt,3,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
					}
				}
			}
		}
		ef=ADVNXTNRDsec(purtrn,1,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(puracc);
	ZERNRD(purmst);
	ZERNRD(purlin);
	ZERNRD(purtrn);
#ifndef __NO_ENCUMBRANCES__
	}
#endif

	ZERNRD(bnkchg);
	ef=ADVGTENRDsec(bnkchg,1,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		read_record=FALSE;
		FINDFLDGETCHAR(bnkchg,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETINT(bnkchg,"FISCAL YEAR",&fy);
			FINDFLDGETSHORT(bnkchg,"PROCESSING MONTH",&pm);
			if(pm==PM && fy==FiscalYear)
			{
				FINDFLDGETSHORT(bnkchg,"ACCOUNT TYPE",&acctype);
				FINDFLDGETSHORT(bnkchg,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(bnkchg,"CHARGE DATE",&adate);
				memset(stemp,0,101);
				sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(DefinitionType==deftype && ((RDAstrcmp(stemp,date_affected))<=0)) 
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
					if(read_record==TRUE)
					{
						FINDFLDGETSTRING(bnkchg,"BANK IDENTIFICATION",&id);
						FINDFLDGETSTRING(bnkchg,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(bnkchg,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(bnkchg,"SOURCE USER",&su);
						FINDFLDGETSTRING(bnkchg,"CONTROL",&c);
						FINDFLDGETSTRING(bnkchg,"REFERENCE",&ref);
						FINDFLDGETINT(bnkchg,"TRANSACTION NUMBER",&trans);
						FINDFLDGETCHAR(bnkchg,"DISTRIBUTED",&d);
						FINDFLDGETDOUBLE(bnkchg,"AMOUNT",&amt);
						cy=atoi(&edate[6]);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) deb=amt;
						else if(AccountType==0) cred=amt;
						else if(AccountType==1 && amt>=0.0) cred=amt;
						else if(AccountType==1) deb=amt;
						else if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) deb=amt;
						else if(AccountType==2) cred=amt;
						addsortrecord(dbsrt,4,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
					}
				}
			}
		}
		ef=ADVNXTNRDsec(bnkchg,2,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(bnkchg);

	ZERNRD(payddsv);
	if(AccountType==2)
	{
		FINDFLDSETINT(payddsv,"FISCAL YEAR",FiscalYear);
		ef=ADVGTENRDsec(payddsv,2,SCRNvirtualSubData,MAINRSRC);
		while(!ef)
		{
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fy);
			if(fy!=FiscalYear) break;
			read_record=FALSE;
			FINDFLDGETCHAR(payddsv,"DELETEFLAG",&delflag);
			FINDFLDGETSHORT(payddsv,"FISCAL PROCESSING MONTH",&pm);
			FINDFLDGETSTRING(payddsv,"CHARGE DATE",&adate);
			FINDFLDGETSHORT(payddsv,"DEFINITION TYPE",&deftype);
			memset(stemp,0,101);
			sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
			if(!delflag && pm==PM && DefinitionType==deftype && ((RDAstrcmp(stemp,date_affected))<=0)) 
			{
				FINDFLDGETSTRING(payddsv,"DEBIT CODE",&debcode);
				FINDFLDGETSTRING(payddsv,"CREDIT CODE",&credcode);
				read_record=FALSE;
				if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode)) read_record=TRUE;
				if(read_record)
				{
					ZERNRD(payddms);
					COPYFIELD(payddsv,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
					if(ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payddms,1);
					FINDFLDGETSTRING(payddsv,"VENDOR IDENTIFICATION",&id);
					FINDFLDGETSTRING(payddsv,"ENTRY DATE",&edate);
					FINDFLDGETSTRING(payddms,"SHORT DESCRIPTION",&desc);
					FINDFLDGETSTRING(payddsv,"SOURCE USER",&su);
					FINDFLDGETINT(payddsv,"CHECK NUMBER",&checknum);
					memset(stemp,0,101);
					if(checknum!=0)
					{
						sprintf(stemp,"%d",checknum);
					} else {
						FINDFLDGETINT(payddsv,"ELECTRONIC TRANSFER NUMBER",&checknum);
						sprintf(stemp,"%d",checknum);
					}
					c=stralloc(stemp);	
					FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&ref);
					FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&trans);
					FINDFLDGETCHAR(payddsv,"DISTRIBUTED",&d);
					FINDFLDGETDOUBLE(payddsv,"AMOUNT",&amt);
					cy=atoi(&edate[6]);
					deb=0.0;cred=0.0;
					if(AccountType==2 && !RDAstrcmp(debcode,AccountCode)) deb=amt;
					else if(AccountType==2) cred=amt;
					addsortrecord(dbsrt,6,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
				}
			}
			ef=ADVNXTNRDsec(payddsv,2,SCRNvirtualSubData,MAINRSRC);
		}
		ZERNRD(payddsv);
	}
	ZERNRD(paymst);
	ef=ADVGTENRDsec(paymst,1,SCRNvirtualSubData,MAINRSRC);
	while(!ef)
	{
		FINDFLDGETINT(paymst,"FISCAL YEAR",&fy);
		FINDFLDGETSHORT(paymst,"FISCAL MONTH",&pm);
		FINDFLDGETCHAR(paymst,"DELETEFLAG",&delflag);
		if(fy==FiscalYear && pm==PM && !delflag)
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
				FINDFLDGETSTRING(payjpms,"CHECK DATE",&adate);
				if(isEMPTY(adate))
				{
					FINDFLDGETSTRING(payjpms,"DIRECT DEPOSIT DATE",&adate);
				}
				memset(stemp,0,101);
				sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(!delflag && ((RDAstrcmp(stemp,date_affected))<=0)) 
				{
					read_record=FALSE;
					if((AccountType==2) || (acctype==AccountType)) 
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
							FINDFLDGETSTRING(payjtrn,"CREDIT CODE",&credcode);
							FINDFLDGETSTRING(payjtrn,"PAYABLE CODE",&payable);
							FINDFLDGETSTRING(payjtrn,"DUE FROM",&duefrom);
							FINDFLDGETSTRING(payjtrn,"CASH CODE",&clearcash);
							if(!RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(credcode,AccountCode) || !RDAstrcmp(payable,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(clearcash,AccountCode)) read_record=TRUE;
						}
					}
					if(read_record==TRUE)
					{
						ZERNRD(payjpms);
						COPYFIELD(payjtrn,payjpms,"PERSONNEL IDENTIFICATION");
						COPYFIELD(payjtrn,payjpms,"PAY IDENTIFICATION");
						COPYFIELD(payjtrn,payjpms,"JOB NUMBER");
						if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
						FINDFLDGETSTRING(payjtrn,"PERSONNEL IDENTIFICATION",&id);
						FINDFLDGETSTRING(payjtrn,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(paymst,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(payjtrn,"SOURCE USER",&su);
						FINDFLDGETINT(payjpms,"CHECK NUMBER",&checknum);
						memset(stemp,0,101);
						if(checknum!=0)
						{
							sprintf(stemp,"%d",checknum);
						} else {
							FINDFLDGETINT(payjpms,"ELECTRONIC TRANSFER NUMBER",&checknum);
							sprintf(stemp,"%d",checknum);
						}
						c=stralloc(stemp);	
						FINDFLDGETSTRING(payjtrn,"PAY IDENTIFICATION",&ref);
						FINDFLDGETINT(payjtrn,"JOB NUMBER",&trans);
						if(!RDAstrcmp(credcode,AccountCode))
						{
							FINDFLDGETCHAR(payjtrn,"CLEARED",&d);
						} else FINDFLDGETCHAR(payjtrn,"DISTRIBUTED",&d);
						FINDFLDGETDOUBLE(payjtrn,"AMOUNT",&amt);
						FINDFLDGETINT(paymst,"CALENDAR YEAR",&cy);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) deb=amt;
						else if(AccountType==0) cred=amt;
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(payable,AccountCode)) deb=amt;
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode) || !RDAstrcmp(payable,AccountCode) || !RDAstrcmp(clearcash,AccountCode)) cred=amt;
						addsortrecord(dbsrt,7,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
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
				FINDFLDGETSTRING(payjpms,"CHECK DATE",&adate);
				if(isEMPTY(adate))
				{
					FINDFLDGETSTRING(payjpms,"DIRECT DEPOSIT DATE",&adate);
				}
				memset(stemp,0,101);
				sprintf(stemp,"%.4s-%.2s-%.2s",&adate[6],&adate[0],&adate[3]);
				if(!delflag && ((RDAstrcmp(stemp,date_affected))<=0)) 
				{
					read_record=FALSE;
					if((AccountType==2) || (acctype==AccountType)) 
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
					if(read_record==TRUE)
					{
						ZERNRD(payjpms);
						COPYFIELD(paydtrn,payjpms,"PERSONNEL IDENTIFICATION");
						COPYFIELD(paydtrn,payjpms,"PAY IDENTIFICATION");
						COPYFIELD(paydtrn,payjpms,"JOB NUMBER");
						if(ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,MAINRSRC)) KEYNRD(payjpms,1);
						FINDFLDGETSTRING(paydtrn,"PERSONNEL IDENTIFICATION",&id);
						FINDFLDGETSTRING(paydtrn,"ENTRY DATE",&edate);
						FINDFLDGETSTRING(paymst,"DESCRIPTION",&desc);
						FINDFLDGETSTRING(paydtrn,"SOURCE USER",&su);
						FINDFLDGETINT(payjpms,"CHECK NUMBER",&checknum);
						memset(stemp,0,101);
						if(checknum!=0)
						{
							sprintf(stemp,"%d",checknum);
						} else {
							FINDFLDGETINT(payjpms,"ELECTRONIC TRANSFER NUMBER",&checknum);
							sprintf(stemp,"%d",checknum);
						}
						c=stralloc(stemp);	
						FINDFLDGETSTRING(paydtrn,"PAY IDENTIFICATION",&ref);
						FINDFLDGETINT(paydtrn,"JOB NUMBER",&trans);
						FINDFLDGETCHAR(paydtrn,"DISTRIBUTED",&d);
						FINDFLDGETDOUBLE(paydtrn,"AMOUNT",&amt);
						FINDFLDGETINT(paymst,"CALENDAR YEAR",&cy);
						deb=0.0;cred=0.0;
						if(AccountType==0 && amt>=0.0) deb=amt;
						else if(AccountType==0) cred=amt;
						if(AccountType==2 && !RDAstrcmp(debcode,AccountCode) || !RDAstrcmp(duefrom,AccountCode) || !RDAstrcmp(payable,AccountCode)) deb=amt;
						if(AccountType==2 && !RDAstrcmp(credcode,AccountCode) || !RDAstrcmp(payable,AccountCode) || !RDAstrcmp(clearcash,AccountCode)) cred=amt;
						addsortrecord(dbsrt,8,adate,id,ref,trans,accnum,cy,c,desc,su,edate,deb,cred,d);
					}
				}
				ef2=ADVNXTNRDsec(paydtrn,2,SCRNvirtualSubData,MAINRSRC);
			}
		}
		ef=ADVNXTNRDsec(paymst,1,SCRNvirtualSubData,MAINRSRC);
	}
	ZERNRD(paymst);
	ZERNRD(payjtrn);
	ZERNRD(paydtrn);
	if(adate!=NULL) Rfree(adate);
	if(edate!=NULL) Rfree(edate);
	if(id!=NULL) Rfree(id);
	if(c!=NULL) Rfree(c);
	if(desc!=NULL) Rfree(desc);
	if(su!=NULL) Rfree(su);
	if(ref!=NULL) Rfree(ref);
	if(date_affected!=NULL) Rfree(date_affected);
	DisplayData();
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL DEBITS",TOTAL_DEBITS);
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL CREDITS",TOTAL_CREDITS);
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

	if(!IsScreenDisplayed(MAINRSRC)) return;
	nofields=NUMFLDS(dbsrt->fileno);
	fields=FLDPOINTER(dbsrt->fileno);
	for(y=1;y<(NumberRows+1);++y)
	{
		if(fields!=NULL)
		{
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				memset(stemp,0,101);
				sprintf(stemp,"%08d:%s",y,fldx->name);
				r=FINDRSC(MAINRSRC,stemp);
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
					updatemember(mem);
				}
			}
		}
	}
}
static void set_defaults(RDArsrc *parent)
{
	DefaultResetScreen(parent,TRUE);
	changeACCTtype(parent);
	changeyear(parent);
	changeDEFtype(parent);
	getacctdesc(parent);
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

	FINDRSCGETINT(parent,"ACCOUNT CODES",&selacct);
	if(AccountCode!=NULL) Rfree(AccountCode);
	TOTAL_DEBITS=0.0;
	TOTAL_CREDITS=0.0;
	CurrentPage=1;
	TotalPages=1;
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL DEBITS",TOTAL_DEBITS);
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL CREDITS",TOTAL_CREDITS);
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updatersrc(MAINRSRC,"TOTAL DEBITS");
	updatersrc(MAINRSRC,"TOTAL CREDITS");
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
	GatherData();
}
static void quit_record_free(RDArsrc *parent)
{
	if(parent!=NULL) free_rsrc(parent);
	if(dbsrt!=NULL) 
	{
		CLSDBsort(dbsrt);
		FreeDBsort(dbsrt);
	}
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
	if(bnkchg!=(-1)) CLSNRD(bnkchg);
	if(bnkrtn!=(-1)) CLSNRD(bnkrtn);
	if(prptrn!=(-1)) CLSNRD(prptrn);
	if(prppay!=(-1)) CLSNRD(prppay);
	if(restran!=(-1)) CLSNRD(restran);
	if(restpay!=(-1)) CLSNRD(restpay);
	if(utbtrn!=(-1)) CLSNRD(utbtrn);
	if(msctrn!=(-1)) CLSNRD(msctrn);
	if(mscpay!=(-1)) CLSNRD(mscpay);
	if(occrec!=(-1)) CLSNRD(occrec);
	if(prmttran!=(-1)) CLSNRD(prmttran);
	if(prmtpaid!=(-1)) CLSNRD(prmtpaid);
	if(ACCTtypes!=NULL) freeapplib(ACCTtypes);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(yearavl!=NULL) freeapplib(yearavl);
	if(acctavl!=NULL) freeapplib(acctavl);
	if(journalavl!=NULL) freeapplib(journalavl);
	if(AccountCode!=NULL) Rfree(AccountCode);
	if(pmonth!=NULL) freeapplib(pmonth);
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
					if(!deleteflag && active) 
					{
						FINDFLDGETCHAR(tempyrfile,"DELETEFLAG",&deleteflag);
						FINDFLDGETCHAR(tempyrfile,"ACTIVE",&active);
						if(account_code!=NULL && !deleteflag && active) 
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
	if(acctype!=AccountType)
	{
		DefinitionType=(-1);
		FiscalYear=(-1);
		if(FINDRSCGETINT(parent,"DEFINITION TYPES",&selectedd)) return;
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
void changeAFFECTdate(RDArsrc *r)
{
	int PM=0;
	int x=0;

	readwidget(MAINRSRC,"AFFECT DATE");
	FINDRSCGETSTRING(MAINRSRC,"AFFECT DATE",&AFFECTDate);
	memset(stemp,0,101);
	sprintf(stemp,"%.2s",&AFFECTDate[0]);
	PM=atoi(stemp);
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
	changeyear(MAINRSRC);
	changeDEFtype(MAINRSRC);
	getacctdesc(MAINRSRC);
	TOTAL_DEBITS=0.0;
	TOTAL_CREDITS=0.0;
	CurrentPage=1;
	TotalPages=1;
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL DEBITS",TOTAL_DEBITS);
	FINDRSCSETDOUBLE(MAINRSRC,"TOTAL CREDITS",TOTAL_CREDITS);
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updatersrc(MAINRSRC,"TOTAL DEBITS");
	updatersrc(MAINRSRC,"TOTAL CREDITS");
	updatersrc(MAINRSRC,"CURRENT PAGE");
	updatersrc(MAINRSRC,"TOTAL PAGES");
	ClearTableResources();
	updateallrsrc(MAINRSRC);
	ClearDataBase();
	GatherData();
	DisplayData();
	FINDRSCSETINT(MAINRSRC,"CURRENT PAGE",CurrentPage);
	FINDRSCSETINT(MAINRSRC,"TOTAL PAGES",TotalPages);
	updateallrsrc(MAINRSRC);
}
static int getyears(RDArsrc *parent)
{
	short ef,x=0;
	int selyear=0,curryear=0,year=0;
	char delflag=FALSE/*,*yearstr=NULL*/;

	curryear=GETCURRENTFISCALYEAR(finsetup->month);
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
			sprintf(stemp,"%-4d",year);
			addAPPlib(yearavl,stemp);
			if(year==curryear) 
			{
				FINDFLDGETSHORT(yearnum,"PROCESSING MONTH",&x);
				dm=x;
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
void main(int argc,char **argv)
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

	if(InitializeSubsystems(argc,argv,module,"DETAIL QUERY")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
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
	gsv=RDAGenericSetupNew("FINMGT","CURRENT FISCAL YEAR");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			FiscalYear=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(argc>1) 
	{
		if(!RDAstrcmp(argv[1],"SUM_CONTROL")) Style=1;
	}
        finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,quit_record,MAINRSRC)==(-1)) return;
	if((filenum=APPOPNNRD(module,"FINALIAS",TRUE,TRUE))==(-1)) return;
	if((byrnum=APPOPNNRD(module,"FINBYR",TRUE,TRUE))==(-1)) return;
	if((eyrnum=APPOPNNRD(module,"FINEYR",TRUE,TRUE))==(-1)) return;
	if((ryrnum=APPOPNNRD(module,"FINRYR",TRUE,TRUE))==(-1)) return;
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
	}
	if(XPERT_SETUP->INVCTL)
	{
	if((invadj=APPOPNNRD("INVCTL","INVADJ",TRUE,FALSE))==(-1)) return;
	if((invisst=APPOPNNRD("INVCTL","INVISST",TRUE,FALSE))==(-1)) return;
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
	}
	if(XPERT_SETUP->RLSTMGT)
	{
		if((restran=APPOPNNRD("RLSTMGT","RESTRAN",TRUE,FALSE))==(-1)) return;
		if((restpay=APPOPNNRD("RLSTMGT","RESTPAID",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->MISCBILL)
	{
		if((msctrn=APPOPNNRD("MISCBILL","MISCTRAN",TRUE,FALSE))==(-1)) return;
		if((mscpay=APPOPNNRD("MISCBILL","MISCPAY",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->OCCTAX)
	{
		if((occrec=APPOPNNRD("OCCTAX","OCCREC",TRUE,FALSE))==(-1)) return;
	}
	if(XPERT_SETUP->BLDPRMT)
	{
		if((prmttran=APPOPNNRD("BLDPRMT","PRMTTRAN",TRUE,FALSE))==(-1)) return;
		if((prmtpaid=APPOPNNRD("BLDPRMT","PRMTPAID",TRUE,FALSE))==(-1)) return;
	}	
	if(XPERT_SETUP->UTLBLL)
	{
		if((utbtrn=APPOPNNRD("UTLBLL","UTBTRN",TRUE,FALSE))==(-1)) return;
	}
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
	addDFincvir(MAINRSRC,"VENPMT","VENVCH",NULL,venvch);
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
	addDFincvir(MAINRSRC,"INVCTL","INVISST",NULL,invisst);
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
	temp=GETCURRENTDATE10();
	AFFECTDate=stralloc(temp);
	addstdrsrc(MAINRSRC,"AFFECT DATE",DATES,10,AFFECTDate,TRUE);
	FINDRSCSETFUNC(MAINRSRC,"AFFECT DATE",changeAFFECTdate,NULL);
	addstdrsrc(MAINRSRC,"TOTAL DEBITS",DOLLARS,20,NULL,FALSE);
	addstdrsrc(MAINRSRC,"TOTAL CREDITS",DOLLARS,20,NULL,FALSE);
	addstdrsrc(MAINRSRC,"CURRENT PAGE",LONGV,8,&CurrentPage,TRUE);
	FINDRSCSETFUNC(MAINRSRC,"CURRENT PAGE",changeCurrentPage,NULL);
	addstdrsrc(MAINRSRC,"TOTAL PAGES",LONGV,8,&TotalPages,FALSE);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				start=0;
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
				memset(stemp,0,101);
				sprintf(stemp,"%08d:%s",y,fldx->name);
				if(!alreadyrsrc(stemp,MAINRSRC))
				{
					makefieldrsrc(MAINRSRC,stemp,fldx->type,
						fldx->len,FALSE);
				}
			}
		}
	}
	getsupportingrecords(MAINRSRC,FALSE);
	SetVirFieldFuncs(MAINRSRC);
	SCRNvirtual2rsrc(MAINRSRC);
	addbtnrsrc(MAINRSRC,"PREVIOUS PAGE",TRUE,PreviousPage,NULL);
	addbtnrsrc(MAINRSRC,"NEXT PAGE",TRUE,NextPage,NULL);
	addbtnrsrc(MAINRSRC,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(MAINRSRC,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addbtnrsrc(MAINRSRC,"QUIT",TRUE,quit_record,NULL);
	addrfcbrsrc(MAINRSRC,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(MAINRSRC,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(MAINRSRC);
	CustomTableFunction=dQueryCustomTableFunction;
	APPmakescrn(MAINRSRC,TRUE,quit_record_free,NULL,TRUE);
	FINDRSCGETINT(MAINRSRC,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(MAINRSRC,"DEFINITION TYPES",&selectedd);
	accttype=(short)selecteda;
	deftype=(short)selectedd;
	FINDRSCSETACCTTYPE(MAINRSRC,"ACCOUNT CODE SEARCH",accttype);
	FINDRSCSETDEFTYPE(MAINRSRC,"ACCOUNT CODE SEARCH",deftype);
	RDAAPPMAINLOOP();
}
