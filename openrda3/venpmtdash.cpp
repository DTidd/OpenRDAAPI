/* Vendor Payment Dashboards */
#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

int VoucherStatusValuetoVisual=0;
short VoucherStatusShowTable=FALSE;
Wt::WContainerWidget *UnProcessedVoucherStatusDashBoard=NULL;
Wt::WStandardItemModel *VoucherStatussUnProcessed=NULL;
char VP21_VOUCHER_APPROVAL=FALSE;
#define SEE_VoucherStatus()	xSEE_VoucherStatus(__LINE__,__FILE__)
void xSEE_VoucherStatus(int,char *);
Wt::WContainerWidget *UnProcessedVoucherStatus(void);

struct VoucherStatuss {
	char *status;
	int count;
	double amt;
};
typedef struct VoucherStatuss VoucherStatus;

VoucherStatus *myVoucherStatus=NULL;
int numVoucherStatus=0;

void FreeVoucherStatus()
{
	VoucherStatus *e=NULL;
	int x=0;

	if(myVoucherStatus!=NULL)
	{
		for(x=0,e=myVoucherStatus;x<numVoucherStatus;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myVoucherStatus);
	}
	myVoucherStatus=NULL;
	numVoucherStatus=0;
}
void xSEE_VoucherStatus(int line,char *file)
{
	int x=0;
	VoucherStatus *e=NULL;

	fprintf(RDA_STDERR,"Showing VoucherStatus num [%d] at line [%d] program [%s]\n.",numVoucherStatus,line,file);
	if(myVoucherStatus!=NULL)
	{
		for(x=0,e=myVoucherStatus;x<numVoucherStatus;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s] Amount [%.02f]\n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addVoucherStatus(char *s,double revx)
{
	VoucherStatus *e=NULL;
	int x=0;
	
	if(myVoucherStatus!=NULL)
	{
		for(x=0,e=myVoucherStatus;x<numVoucherStatus;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numVoucherStatus)
		{
			myVoucherStatus=Rrealloc(myVoucherStatus,((numVoucherStatus+1)*sizeof(VoucherStatus)));
			e=myVoucherStatus+numVoucherStatus;
			++numVoucherStatus;
			e->status=stralloc(s);
			e->count=0;
			e->amt=0.0;
		}
	} else {
		myVoucherStatus=Rmalloc(sizeof(VoucherStatus));
		e=myVoucherStatus;
		numVoucherStatus=1;
		e->status=stralloc(s);
		e->count=0;
		e->amt=0.0;
	}
	++e->count;
	e->amt+=revx;
}
short ReadVoucherStatus()
{
	int fileno=(-1),count=0,checknum=0,eftnum=0;
	short ef=FALSE;
	char delflag=FALSE,distributed=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char verified=FALSE,bnkrec=FALSE,cleared=FALSE,printed=FALSE;
	char approved=FALSE,manualc=FALSE,ddraft=FALSE,eft=FALSE;
	char voidck=FALSE;
	double amt=0.0;

	fileno=OPNNRD("VENPMT","VENVCH");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"DISTRIBUTED",&distributed);
			FINDFLDGETCHAR(fileno,"CLEARED",&cleared);
			if(!delflag && (!distributed || !cleared))
			{
				FINDFLDGETCHAR(fileno,"SOURCE USER VERIFIED",&verified);
				FINDFLDGETCHAR(fileno,"DIRECT DRAFT",&ddraft);
				FINDFLDGETCHAR(fileno,"MANUAL VOUCHER",&manualc);
				FINDFLDGETCHAR(fileno,"ELECTRONIC TRANSFER",&eft);
				FINDFLDGETCHAR(fileno,"VOID CHECK",&voidck);
				if(VP21_VOUCHER_APPROVAL)
				{
					FINDFLDGETCHAR(fileno,"APPROVED",&approved);
				}
				FINDFLDGETINT(fileno,"CHECK NUMBER",&checknum);
				FINDFLDGETINT(fileno,"ELECTRONIC TRANSFER NUMBER",&eftnum);
				FINDFLDGETCHAR(fileno,"PRINTED",&printed);
				FINDFLDGETCHAR(fileno,"BNKREC UPDATED",&bnkrec);
				if(!verified) srcmod="Added awaiting Source User Verification";
				else if(VP21_VOUCHER_APPROVAL && !voidck && !approved) srcmod="Verified waiting Approval";
				else if(!voidck && !ddraft && !eft && !printed && !cleared && checknum==0) srcmod="Vouchers waiting on check numbers.";
				else if(!voidck && !ddraft && eft && !printed && !cleared && eftnum==0) srcmod="Vouchers waiting on EFT numbers.";
				else if(!voidck && !ddraft && !eft && !printed && !cleared && checknum!=0) srcmod="Checks waiting to be printed.";
				else if(!voidck && !ddraft && !eft && !printed && !cleared && eftnum!=0) srcmod="EFT(s) waiting to be printed.";
				else if(!distributed && !bnkrec && !cleared) srcmod="Vouchers waiting for BnkRec Updated";
				else if(!distributed) srcmod="Vouchers waiting for Distribution";
				else if(!cleared) srcmod="Vouchers waiting to be Cleared";
				++count;
				FINDFLDGETSTRING(fileno,"SOURCE MODULE",&srcmod);
				FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
				addVoucherStatus(srcmod,amt);
				DID_STUFF=TRUE;
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		return(-1);
	}
}
void VoucherStatusChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=UnProcessedVoucherStatusDashBoard->parent();
	VoucherStatusValuetoVisual=CB->currentIndex();
	VoucherStatusShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=UnProcessedVoucherStatus();
	parent->addWidget(w);
}
Wt::WContainerWidget *UnProcessedVoucherStatus()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	VoucherStatus *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	double amt=0.0;
	int x=0,count=0;
	long y=0;
	char *libx=NULL,*temp=NULL,VPstemp[1024];
	RDAGenericSetup *gsv=NULL;

	myVoucherStatus=NULL;
	numVoucherStatus=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libx,"%s/rda/VENPMT.GSV",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\VENPMT.GSV",CURRENTDIRECTORY);
#endif

	gsv=RDAGenericSetupNew("VENPMT","SWB_VP21_VOUCHER_APPROVAL");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			VP21_VOUCHER_APPROVAL=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	UnProcessedVoucherStatusDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(UnProcessedVoucherStatusDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unprocessed Vouchers"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadVoucherStatus();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Vendor Vouchers."),UnProcessedVoucherStatusDashBoard);
		return(UnProcessedVoucherStatusDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unprocessed Vouchers."),UnProcessedVoucherStatusDashBoard);
		return(UnProcessedVoucherStatusDashBoard);
	}
	if(VoucherStatussUnProcessed!=NULL) VoucherStatussUnProcessed->~WStandardItemModel();
	VoucherStatussUnProcessed=new Wt::WStandardItemModel();
	VoucherStatussUnProcessed->insertColumns(VoucherStatussUnProcessed->columnCount(),3);
// headers 
	VoucherStatussUnProcessed->setHeaderData(0,Wt::WString("Status"));
	VoucherStatussUnProcessed->setHeaderData(1,Wt::WString("Number of Records"));
	VoucherStatussUnProcessed->setHeaderData(2,Wt::WString("Amount"));
	VoucherStatussUnProcessed->insertRows(VoucherStatussUnProcessed->rowCount(),numVoucherStatus);
	count=0;
	amt=0.0;
	for(x=0,e=myVoucherStatus;x<numVoucherStatus;++x,++e)
	{
		VoucherStatussUnProcessed->setData(x,0,Wt::WString(e->status));
		VoucherStatussUnProcessed->setData(x,1,e->count);
		count+=e->count;
		VoucherStatussUnProcessed->setData(x,2,e->amt/100);
		amt+=e->amt;
	}
	VoucherStatussUnProcessed->sort((VoucherStatusValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	temp=famt(amt,20);
	sprintf(VPstemp,"Records: %8d Total: %s",count,temp);
	l=new Wt::WContainerWidget(UnProcessedVoucherStatusDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(UnProcessedVoucherStatusDashBoard);
	xchart->setModel(VoucherStatussUnProcessed);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((VoucherStatusValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->setAvoidLabelRendering(4.99);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnProcessedVoucherStatusDashBoard);
	TablePanel->setTitle(Wt::WString("Unprocessed Voucher Status Table"));
	TablePanel->collapsed().connect(std::bind([=]() { VoucherStatusShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { VoucherStatusShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(VoucherStatusShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(VoucherStatussUnProcessed);
	table->setColumnWidth(0,350);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%d");
	table->setItemDelegateForColumn(1,myID);
	table->setColumnAlignment(2,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(2,myID);
	table->setHeaderAlignment(0,AlignCenter);
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
	CB->setCurrentIndex(VoucherStatusValuetoVisual);
	CB->activated().connect(boost::bind(&VoucherStatusChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeVoucherStatus();
	return(UnProcessedVoucherStatusDashBoard);
}
