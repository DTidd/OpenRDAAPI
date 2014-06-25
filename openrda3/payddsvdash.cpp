/* Vendor Withholding Dashboards */
#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

int VendorWithholdingValuetoVisual=0;
short VendorWithholdingShowTable=FALSE;
Wt::WContainerWidget *UnProcessedVendorWithholdingDashBoard=NULL;
Wt::WStandardItemModel *VendorWithholdingsUnProcessed=NULL;
#define SEE_VendorWithholding()	xSEE_VendorWithholding(__LINE__,__FILE__)
void xSEE_VendorWithholding(int,char *);
Wt::WContainerWidget *UnProcessedVendorWithholding(void);

struct VendorWithholdings {
	char *status;
	int count;
	double amt;
};
typedef struct VendorWithholdings VendorWithholding;

VendorWithholding *myVendorWithholding=NULL;
int numVendorWithholding=0;

void FreeVendorWithholding()
{
	VendorWithholding *e=NULL;
	int x=0;

	if(myVendorWithholding!=NULL)
	{
		for(x=0,e=myVendorWithholding;x<numVendorWithholding;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myVendorWithholding);
	}
	myVendorWithholding=NULL;
	numVendorWithholding=0;
}
void xSEE_VendorWithholding(int line,char *file)
{
	int x=0;
	VendorWithholding *e=NULL;

	fprintf(RDA_STDERR,"Showing VendorWithholding num [%d] at line [%d] program [%s]\n.",numVendorWithholding,line,file);
	if(myVendorWithholding!=NULL)
	{
		for(x=0,e=myVendorWithholding;x<numVendorWithholding;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s] Amount [%.02f]\n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addVendorWithholding(char *s,double revx)
{
	VendorWithholding *e=NULL;
	int x=0;
	
	if(myVendorWithholding!=NULL)
	{
		for(x=0,e=myVendorWithholding;x<numVendorWithholding;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numVendorWithholding)
		{
			myVendorWithholding=Rrealloc(myVendorWithholding,((numVendorWithholding+1)*sizeof(VendorWithholding)));
			e=myVendorWithholding+numVendorWithholding;
			++numVendorWithholding;
			e->status=stralloc(s);
			e->count=0;
			e->amt=0.0;
		}
	} else {
		myVendorWithholding=Rmalloc(sizeof(VendorWithholding));
		e=myVendorWithholding;
		numVendorWithholding=1;
		e->status=stralloc(s);
		e->count=0;
		e->amt=0.0;
	}
	++e->count;
	e->amt+=revx;
}
short ReadVendorWithholding()
{
	int fileno=(-1),count=0,checknum=0,eftnum=0;
	short ef=FALSE,payddms=(-1);
	char delflag=FALSE,distributed=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char verified=FALSE,bnkrec=FALSE,cleared=FALSE,printed=FALSE;
	char approved=FALSE,manualc=FALSE,ddraft=FALSE,eft=FALSE;
	char voidck=FALSE;
	double amt=0.0;

	fileno=OPNNRD("PAYROLL","PAYDDSV");
	if(fileno!=(-1))
	{
		payddms=OPNNRD("PAYROLL","PAYDDMS");
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"DISTRIBUTED",&distributed);
			FINDFLDGETCHAR(fileno,"BNKREC UPDATED",&bnkrec);
			if(!delflag && (!distributed || !bnkrec))
			{
				ZERNRD(payddms);
				COPYFIELD(fileno,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
				if(EQLNRD(payddms,1)) KEYNRD(payddms,1);
				FINDFLDGETCHAR(fileno,"SOURCE USER VERIFIED",&verified);
				FINDFLDGETCHAR(fileno,"DIRECT DRAFT",&ddraft);
				FINDFLDGETCHAR(fileno,"MANUAL VOUCHER",&manualc);
				FINDFLDGETCHAR(fileno,"ELECTRONIC TRANSFER",&eft);
				FINDFLDGETCHAR(fileno,"VOID CHECK",&voidck);
				FINDFLDGETINT(fileno,"CHECK NUMBER",&checknum);
				FINDFLDGETINT(fileno,"ELECTRONIC TRANSFER NUMBER",&eftnum);
				FINDFLDGETCHAR(fileno,"PRINTED",&printed);
				if(!verified) srcmod="Awaiting Source User Verification";
				else if(!voidck && !ddraft && !eft && !printed && !cleared && checknum==0) srcmod="Vouchers waiting on check numbers.";
				else if(!voidck && !ddraft && eft && !printed && !cleared && eftnum==0) srcmod="Vouchers waiting on EFT numbers.";
				else if(!voidck && !ddraft && !eft && !printed && !cleared && checknum!=0) srcmod="Checks waiting to be printed.";
				else if(!voidck && !ddraft && !eft && !printed && !cleared && eftnum!=0) srcmod="EFT(s) waiting to be printed.";
				else if(!distributed && !bnkrec) srcmod="Vouchers waiting for BnkRec Updated";
				else if(!distributed) srcmod="Vouchers waiting for Distribution";
				++count;
				FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
				addVendorWithholding(srcmod,amt);
				DID_STUFF=TRUE;
			}
			ef=NXTNRD(fileno,1);
		}
		if(payddms!=(-1)) CLSNRD(payddms);
		CLSNRD(fileno);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		return(-1);
	}
}
void VendorWithholdingChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=UnProcessedVendorWithholdingDashBoard->parent();
	VendorWithholdingValuetoVisual=CB->currentIndex();
	VendorWithholdingShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=UnProcessedVendorWithholding();
	parent->addWidget(w);
}
Wt::WContainerWidget *UnProcessedVendorWithholding()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	VendorWithholding *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	double amt=0.0;
	int x=0,count=0;
	long y=0;
	char *temp=NULL,VPstemp[1024];

	myVendorWithholding=NULL;
	numVendorWithholding=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnProcessedVendorWithholdingDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(UnProcessedVendorWithholdingDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unprocessed Vendor Withholding Vouchers"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadVendorWithholding();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Vendor Withholding Vouchers."),UnProcessedVendorWithholdingDashBoard);
		return(UnProcessedVendorWithholdingDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unprocessed Vendor Withholding Vouchers."),UnProcessedVendorWithholdingDashBoard);
		return(UnProcessedVendorWithholdingDashBoard);
	}
	if(VendorWithholdingsUnProcessed!=NULL) VendorWithholdingsUnProcessed->~WStandardItemModel();
	VendorWithholdingsUnProcessed=new Wt::WStandardItemModel();
	VendorWithholdingsUnProcessed->insertColumns(VendorWithholdingsUnProcessed->columnCount(),3);
// headers 
	VendorWithholdingsUnProcessed->setHeaderData(0,Wt::WString("Status"));
	VendorWithholdingsUnProcessed->setHeaderData(1,Wt::WString("Number of Records"));
	VendorWithholdingsUnProcessed->setHeaderData(2,Wt::WString("Amount"));
	VendorWithholdingsUnProcessed->insertRows(VendorWithholdingsUnProcessed->rowCount(),numVendorWithholding);
	count=0;
	amt=0.0;
	for(x=0,e=myVendorWithholding;x<numVendorWithholding;++x,++e)
	{
		VendorWithholdingsUnProcessed->setData(x,0,Wt::WString(e->status));
		VendorWithholdingsUnProcessed->setData(x,1,e->count);
		count+=e->count;
		VendorWithholdingsUnProcessed->setData(x,2,e->amt/100);
		amt+=e->amt;
	}
	VendorWithholdingsUnProcessed->sort((VendorWithholdingValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	temp=famt(amt,20);
	sprintf(VPstemp,"Records: %8d Total: %s",count,temp);
	l=new Wt::WContainerWidget(UnProcessedVendorWithholdingDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(UnProcessedVendorWithholdingDashBoard);
	xchart->setModel(VendorWithholdingsUnProcessed);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((VendorWithholdingValuetoVisual+1));
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnProcessedVendorWithholdingDashBoard);
	TablePanel->setTitle(Wt::WString("Unprocessed Vendor Withholding Voucher Status Table"));
	TablePanel->collapsed().connect(std::bind([=]() { VendorWithholdingShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { VendorWithholdingShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(VendorWithholdingShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(VendorWithholdingsUnProcessed);
	table->setColumnWidth(0,350);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(1,myID);
	table->setHeaderAlignment(0,AlignCenter);
	table->setHeaderAlignment(1,AlignRight);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	l=new Wt::WContainerWidget(w);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of Records"));
	CB->addItem(Wt::WString("Amount"));
	CB->setCurrentIndex(VendorWithholdingValuetoVisual);
	CB->activated().connect(boost::bind(&VendorWithholdingChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeVendorWithholding();
	return(UnProcessedVendorWithholdingDashBoard);
}




int AccumulatedWithholdingValuetoVisual=0;
short AccumulatedWithholdingShowTable=FALSE;
Wt::WContainerWidget *UnProcessedAccumulatedWithholdingDashBoard=NULL;
Wt::WStandardItemModel *AccumulatedWithholdingsUnProcessed=NULL;
#define SEE_AccumulatedWithholding()	xSEE_AccumulatedWithholding(__LINE__,__FILE__)
void xSEE_AccumulatedWithholding(int,char *);
Wt::WContainerWidget *UnProcessedAccumulatedWithholding(void);

struct AccumulatedWithholdings {
	char *status;
	double amt;
};
typedef struct AccumulatedWithholdings AccumulatedWithholding;

AccumulatedWithholding *myAccumulatedWithholding=NULL;
int numAccumulatedWithholding=0;

void FreeAccumulatedWithholding()
{
	AccumulatedWithholding *e=NULL;
	int x=0;

	if(myAccumulatedWithholding!=NULL)
	{
		for(x=0,e=myAccumulatedWithholding;x<numAccumulatedWithholding;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myAccumulatedWithholding);
	}
	myAccumulatedWithholding=NULL;
	numAccumulatedWithholding=0;
}
void xSEE_AccumulatedWithholding(int line,char *file)
{
	int x=0;
	AccumulatedWithholding *e=NULL;

	fprintf(RDA_STDERR,"Showing AccumulatedWithholding num [%d] at line [%d] program [%s]\n.",numAccumulatedWithholding,line,file);
	if(myAccumulatedWithholding!=NULL)
	{
		for(x=0,e=myAccumulatedWithholding;x<numAccumulatedWithholding;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s] Amount [%.02f]\n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addAccumulatedWithholding(char *s,double revx)
{
	AccumulatedWithholding *e=NULL;
	int x=0;
	
	if(myAccumulatedWithholding!=NULL)
	{
		for(x=0,e=myAccumulatedWithholding;x<numAccumulatedWithholding;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numAccumulatedWithholding)
		{
			myAccumulatedWithholding=Rrealloc(myAccumulatedWithholding,((numAccumulatedWithholding+1)*sizeof(AccumulatedWithholding)));
			e=myAccumulatedWithholding+numAccumulatedWithholding;
			++numAccumulatedWithholding;
			e->status=stralloc(s);
			e->amt=0.0;
		}
	} else {
		myAccumulatedWithholding=Rmalloc(sizeof(AccumulatedWithholding));
		e=myAccumulatedWithholding;
		numAccumulatedWithholding=1;
		e->status=stralloc(s);
		e->amt=0.0;
	}
	e->amt+=revx;
}
short ReadAccumulatedWithholding()
{
	int fileno=(-1),checknum=0,eftnum=0;
	short ef=FALSE,payddms=(-1);
	char delflag=FALSE,distributed=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char verified=FALSE,bnkrec=FALSE,cleared=FALSE,printed=FALSE;
	char approved=FALSE,manualc=FALSE,ddraft=FALSE,eft=FALSE;
	char voidck=FALSE;
	double amt=0.0;

	fileno=OPNNRD("PAYROLL","PAYDDAC");
	if(fileno!=(-1))
	{
		payddms=OPNNRD("PAYROLL","PAYDDMS");
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
			if(!delflag && amt!=0.0)
			{
				ZERNRD(payddms);
				COPYFIELD(fileno,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
				if(EQLNRD(payddms,1)) KEYNRD(payddms,1);
				FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&srcmod);
				addAccumulatedWithholding(srcmod,amt);
				DID_STUFF=TRUE;
			}
			ef=NXTNRD(fileno,1);
		}
		if(payddms!=(-1)) CLSNRD(payddms);
		CLSNRD(fileno);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		return(-1);
	}
}
Wt::WContainerWidget *UnProcessedAccumulatedWithholding()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	AccumulatedWithholding *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	double amt=0.0;
	int x=0,count=0;
	long y=0;
	char *temp=NULL,VPstemp[1024];

	myAccumulatedWithholding=NULL;
	numAccumulatedWithholding=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnProcessedAccumulatedWithholdingDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(UnProcessedAccumulatedWithholdingDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unprocessed Accumulated Withholdings"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadAccumulatedWithholding();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Accumulated Withholdings."),UnProcessedAccumulatedWithholdingDashBoard);
		return(UnProcessedAccumulatedWithholdingDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Accumulated Withholdings."),UnProcessedAccumulatedWithholdingDashBoard);
		return(UnProcessedAccumulatedWithholdingDashBoard);
	}
	if(AccumulatedWithholdingsUnProcessed!=NULL) AccumulatedWithholdingsUnProcessed->~WStandardItemModel();
	AccumulatedWithholdingsUnProcessed=new Wt::WStandardItemModel();
	AccumulatedWithholdingsUnProcessed->insertColumns(AccumulatedWithholdingsUnProcessed->columnCount(),2);
// headers 
	AccumulatedWithholdingsUnProcessed->setHeaderData(0,Wt::WString("Deduction Type"));
	AccumulatedWithholdingsUnProcessed->setHeaderData(1,Wt::WString("Amount"));
	AccumulatedWithholdingsUnProcessed->insertRows(AccumulatedWithholdingsUnProcessed->rowCount(),numAccumulatedWithholding);
	amt=0.0;
	for(x=0,e=myAccumulatedWithholding;x<numAccumulatedWithholding;++x,++e)
	{
		AccumulatedWithholdingsUnProcessed->setData(x,0,Wt::WString(e->status));
		AccumulatedWithholdingsUnProcessed->setData(x,1,e->amt/100);
		amt+=e->amt;
	}
	AccumulatedWithholdingsUnProcessed->sort((AccumulatedWithholdingValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	temp=famt(amt,20);
	sprintf(VPstemp,"Records: %8d Total: %s",count,temp);
	l=new Wt::WContainerWidget(UnProcessedAccumulatedWithholdingDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(UnProcessedAccumulatedWithholdingDashBoard);
	xchart->setModel(AccumulatedWithholdingsUnProcessed);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((AccumulatedWithholdingValuetoVisual+1));
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnProcessedAccumulatedWithholdingDashBoard);
	TablePanel->setTitle(Wt::WString("Accumulated Withholdings Table by Deduction Type"));
	TablePanel->collapsed().connect(std::bind([=]() { AccumulatedWithholdingShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { AccumulatedWithholdingShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(AccumulatedWithholdingShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(AccumulatedWithholdingsUnProcessed);
	table->setColumnWidth(0,350);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(1,myID);
	table->setHeaderAlignment(0,AlignCenter);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	FreeAccumulatedWithholding();
	return(UnProcessedAccumulatedWithholdingDashBoard);
}




int AccumulatedWithholdingbyVenValuetoVisual=0;
short AccumulatedWithholdingbyVenShowTable=FALSE;
Wt::WContainerWidget *UnProcessedAccumulatedWithholdingbyVenDashBoard=NULL;
Wt::WStandardItemModel *AccumulatedWithholdingbyVensUnProcessed=NULL;
#define SEE_AccumulatedWithholdingbyVen()	xSEE_AccumulatedWithholdingbyVen(__LINE__,__FILE__)
void xSEE_AccumulatedWithholdingbyVen(int,char *);
Wt::WContainerWidget *UnProcessedAccumulatedWithholdingbyVen(void);

struct AccumulatedWithholdingbyVens {
	char *status;
	double amt;
};
typedef struct AccumulatedWithholdingbyVens AccumulatedWithholdingbyVen;

AccumulatedWithholdingbyVen *myAccumulatedWithholdingbyVen=NULL;
int numAccumulatedWithholdingbyVen=0;

void FreeAccumulatedWithholdingbyVen()
{
	AccumulatedWithholdingbyVen *e=NULL;
	int x=0;

	if(myAccumulatedWithholdingbyVen!=NULL)
	{
		for(x=0,e=myAccumulatedWithholdingbyVen;x<numAccumulatedWithholdingbyVen;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myAccumulatedWithholdingbyVen);
	}
	myAccumulatedWithholdingbyVen=NULL;
	numAccumulatedWithholdingbyVen=0;
}
void xSEE_AccumulatedWithholdingbyVen(int line,char *file)
{
	int x=0;
	AccumulatedWithholdingbyVen *e=NULL;

	fprintf(RDA_STDERR,"Showing AccumulatedWithholdingbyVen num [%d] at line [%d] program [%s]\n.",numAccumulatedWithholdingbyVen,line,file);
	if(myAccumulatedWithholdingbyVen!=NULL)
	{
		for(x=0,e=myAccumulatedWithholdingbyVen;x<numAccumulatedWithholdingbyVen;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s] Amount [%.02f]\n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addAccumulatedWithholdingbyVen(char *s,double revx)
{
	AccumulatedWithholdingbyVen *e=NULL;
	int x=0;
	
	if(myAccumulatedWithholdingbyVen!=NULL)
	{
		for(x=0,e=myAccumulatedWithholdingbyVen;x<numAccumulatedWithholdingbyVen;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numAccumulatedWithholdingbyVen)
		{
			myAccumulatedWithholdingbyVen=Rrealloc(myAccumulatedWithholdingbyVen,((numAccumulatedWithholdingbyVen+1)*sizeof(AccumulatedWithholdingbyVen)));
			e=myAccumulatedWithholdingbyVen+numAccumulatedWithholdingbyVen;
			++numAccumulatedWithholdingbyVen;
			e->status=stralloc(s);
			e->amt=0.0;
		}
	} else {
		myAccumulatedWithholdingbyVen=Rmalloc(sizeof(AccumulatedWithholdingbyVen));
		e=myAccumulatedWithholdingbyVen;
		numAccumulatedWithholdingbyVen=1;
		e->status=stralloc(s);
		e->amt=0.0;
	}
	e->amt+=revx;
}
short ReadAccumulatedWithholdingbyVen()
{
	int fileno=(-1),checknum=0,eftnum=0;
	short ef=FALSE,payddms=(-1),venmsr=(-1);
	char delflag=FALSE,distributed=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char verified=FALSE,bnkrec=FALSE,cleared=FALSE,printed=FALSE;
	char approved=FALSE,manualc=FALSE,ddraft=FALSE,eft=FALSE;
	char voidck=FALSE;
	double amt=0.0;

	fileno=OPNNRD("PAYROLL","PAYDDAC");
	if(fileno!=(-1))
	{
		payddms=OPNNRD("PAYROLL","PAYDDMS");
		venmsr=OPNNRD("VENPMT","VENMSR");
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
			if(!delflag && amt!=0.0)
			{
				ZERNRD(payddms);
				COPYFIELD(fileno,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
				if(EQLNRD(payddms,1)) KEYNRD(payddms,1);
				ZERNRD(venmsr);
				COPYFIELD(payddms,venmsr,"VENDOR IDENTIFICATION");
				if(EQLNRD(venmsr,1)) KEYNRD(venmsr,1);
				FINDFLDGETSTRING(venmsr,"VENDOR NAME",&srcmod);
				addAccumulatedWithholdingbyVen(srcmod,amt);
				DID_STUFF=TRUE;
			}
			ef=NXTNRD(fileno,1);
		}
		if(payddms!=(-1)) CLSNRD(payddms);
		if(venmsr!=(-1)) CLSNRD(venmsr);
		CLSNRD(fileno);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		return(-1);
	}
}
Wt::WContainerWidget *UnProcessedAccumulatedWithholdingbyVen()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	AccumulatedWithholdingbyVen *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	double amt=0.0;
	int x=0,count=0;
	long y=0;
	char *temp=NULL,VPstemp[1024];

	myAccumulatedWithholdingbyVen=NULL;
	numAccumulatedWithholdingbyVen=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnProcessedAccumulatedWithholdingbyVenDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(UnProcessedAccumulatedWithholdingbyVenDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unprocessed Accumulated Withholdings"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadAccumulatedWithholdingbyVen();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Accumulated Withholdings."),UnProcessedAccumulatedWithholdingbyVenDashBoard);
		return(UnProcessedAccumulatedWithholdingbyVenDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Accumulated Withholdings."),UnProcessedAccumulatedWithholdingbyVenDashBoard);
		return(UnProcessedAccumulatedWithholdingbyVenDashBoard);
	}
	if(AccumulatedWithholdingbyVensUnProcessed!=NULL) AccumulatedWithholdingbyVensUnProcessed->~WStandardItemModel();
	AccumulatedWithholdingbyVensUnProcessed=new Wt::WStandardItemModel();
	AccumulatedWithholdingbyVensUnProcessed->insertColumns(AccumulatedWithholdingbyVensUnProcessed->columnCount(),2);
// headers 
	AccumulatedWithholdingbyVensUnProcessed->setHeaderData(0,Wt::WString("Deduction Type"));
	AccumulatedWithholdingbyVensUnProcessed->setHeaderData(1,Wt::WString("Amount"));
	AccumulatedWithholdingbyVensUnProcessed->insertRows(AccumulatedWithholdingbyVensUnProcessed->rowCount(),numAccumulatedWithholdingbyVen);
	amt=0.0;
	for(x=0,e=myAccumulatedWithholdingbyVen;x<numAccumulatedWithholdingbyVen;++x,++e)
	{
		AccumulatedWithholdingbyVensUnProcessed->setData(x,0,Wt::WString(e->status));
		AccumulatedWithholdingbyVensUnProcessed->setData(x,1,e->amt/100);
		amt+=e->amt;
	}
	AccumulatedWithholdingbyVensUnProcessed->sort((AccumulatedWithholdingbyVenValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	temp=famt(amt,20);
	sprintf(VPstemp,"Records: %8d Total: %s",count,temp);
	l=new Wt::WContainerWidget(UnProcessedAccumulatedWithholdingbyVenDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(UnProcessedAccumulatedWithholdingbyVenDashBoard);
	xchart->setModel(AccumulatedWithholdingbyVensUnProcessed);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((AccumulatedWithholdingbyVenValuetoVisual+1));
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnProcessedAccumulatedWithholdingbyVenDashBoard);
	TablePanel->setTitle(Wt::WString("Accumulated Withholdings Table by Vendor"));
	TablePanel->collapsed().connect(std::bind([=]() { AccumulatedWithholdingbyVenShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { AccumulatedWithholdingbyVenShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(AccumulatedWithholdingbyVenShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(AccumulatedWithholdingbyVensUnProcessed);
	table->setColumnWidth(0,350);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(1,myID);
	table->setHeaderAlignment(0,AlignCenter);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	FreeAccumulatedWithholdingbyVen();
	return(UnProcessedAccumulatedWithholdingbyVenDashBoard);
}
