/* Bank Reconciliation Dashboards */
#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

/* By Identification */

Wt::WContainerWidget *BankBalanceByIDView(void);
int BankBalanceByIDValuetoVisual=0;
short BankBalanceByIDShowTable=FALSE;
Wt::WContainerWidget *BankBalanceByIDDashBoard=NULL;
Wt::WStandardItemModel *BankBalanceByIDsModel=NULL;
#define SEE_BankBalanceByID()	xSEE_BankBalanceByID(__LINE__,__FILE__)
void xSEE_BankBalanceByID(int,char *);

struct BankBalanceByIDs {
	char *status;
	double amt;
};
typedef struct BankBalanceByIDs BankBalanceByID;

BankBalanceByID *myBankBalanceByID=NULL;
int numBankBalanceByID=0;

void FreeBankBalanceByID()
{
	BankBalanceByID *e=NULL;
	int x=0;

	if(myBankBalanceByID!=NULL)
	{
		for(x=0,e=myBankBalanceByID;x<numBankBalanceByID;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myBankBalanceByID);
	}
	myBankBalanceByID=NULL;
	numBankBalanceByID=0;
}
void xSEE_BankBalanceByID(int line,char *file)
{
	int x=0;
	BankBalanceByID *e=NULL;

	fprintf(RDA_STDERR,"Showing BankBalanceByID num [%d] at line [%d] program [%s]\n.",numBankBalanceByID,line,file);
	if(myBankBalanceByID!=NULL)
	{
		for(x=0,e=myBankBalanceByID;x<numBankBalanceByID;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Bank [%s] Bank Balance [%.02f] \n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addBankBalanceByID(char *s,double revx)
{
	BankBalanceByID *e=NULL;
	int x=0;
	
	if(myBankBalanceByID!=NULL)
	{
		for(x=0,e=myBankBalanceByID;x<numBankBalanceByID;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numBankBalanceByID)
		{
			myBankBalanceByID=Rrealloc(myBankBalanceByID,((numBankBalanceByID+1)*sizeof(BankBalanceByID)));
			e=myBankBalanceByID+numBankBalanceByID;
			++numBankBalanceByID;
			e->status=stralloc(s);
			e->amt=0.0;
		}
	} else {
		myBankBalanceByID=Rmalloc(sizeof(BankBalanceByID));
		e=myBankBalanceByID;
		numBankBalanceByID=1;
		e->status=stralloc(s);
		e->amt=0.0;
	}
	e->amt+=revx;
}
short ReadBankBalanceByID()
{
	int fileno=(-1),count=0,checknum=0,eftnum=0;
	short ef=FALSE;
	char delflag=FALSE,active1=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char checking=FALSE;
	double amt=0.0,uamt=0.0;

	fileno=OPNNRD("BNKREC","BNKACC");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"ACTIVE",&active1);
			if(!delflag && active1)
			{
				FINDFLDGETSTRING(fileno,"BANK NAME",&srcmod);
				FINDFLDGETDOUBLE(fileno,"BALANCE",&amt);
				FINDFLDGETDOUBLE(fileno,"UNPROCESSED BALANCE",&uamt);
				++count;
				addBankBalanceByID(srcmod,amt+uamt);
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
void BankBalanceByIDChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=BankBalanceByIDDashBoard->parent();
	BankBalanceByIDValuetoVisual=CB->currentIndex();
	BankBalanceByIDShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=BankBalanceByIDView();
	parent->addWidget(w);
}
Wt::WContainerWidget *BankBalanceByIDView()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	BankBalanceByID *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	double amt=0.0;
	int x=0,count=0;
	long y=0;
	char *temp=NULL,VPstemp[1024];

	myBankBalanceByID=NULL;
	numBankBalanceByID=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	BankBalanceByIDDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(BankBalanceByIDDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Bank Account Balances by Identification"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadBankBalanceByID();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Bank Accounts."),BankBalanceByIDDashBoard);
		return(BankBalanceByIDDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Bank Accounts."),BankBalanceByIDDashBoard);
		return(BankBalanceByIDDashBoard);
	}
	if(BankBalanceByIDsModel!=NULL) BankBalanceByIDsModel->~WStandardItemModel();
	BankBalanceByIDsModel=new Wt::WStandardItemModel();
	BankBalanceByIDsModel->insertColumns(BankBalanceByIDsModel->columnCount(),2);
// headers 
	BankBalanceByIDsModel->setHeaderData(0,Wt::WString("Bank"));
	BankBalanceByIDsModel->setHeaderData(1,Wt::WString("Bank Balance"));
	BankBalanceByIDsModel->insertRows(BankBalanceByIDsModel->rowCount(),numBankBalanceByID);
	count=0;
	amt=0.0;
	for(x=0,e=myBankBalanceByID;x<numBankBalanceByID;++x,++e)
	{
		BankBalanceByIDsModel->setData(x,0,Wt::WString(e->status));
		BankBalanceByIDsModel->setData(x,1,e->amt/100);
		amt+=e->amt;
	}
	BankBalanceByIDsModel->sort((BankBalanceByIDValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	temp=famt(amt,20);
	sprintf(VPstemp,"Bank Balance: %s",temp);
	l=new Wt::WContainerWidget(BankBalanceByIDDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(BankBalanceByIDDashBoard);
	xchart->setModel(BankBalanceByIDsModel);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((BankBalanceByIDValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(BankBalanceByIDDashBoard);
	TablePanel->setTitle(Wt::WString("Bank Balance Table"));
	TablePanel->collapsed().connect(std::bind([=]() { BankBalanceByIDShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { BankBalanceByIDShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(BankBalanceByIDShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(BankBalanceByIDsModel);
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
	FreeBankBalanceByID();
	return(BankBalanceByIDDashBoard);
}
/* By Cash Code */

Wt::WContainerWidget *BankBalanceByCashCodeView(void);
int BankBalanceByCashCodeValuetoVisual=0;
short BankBalanceByCashCodeShowTable=FALSE;
Wt::WContainerWidget *BankBalanceByCashCodeDashBoard=NULL;
Wt::WStandardItemModel *BankBalanceByCashCodesModel=NULL;
#define SEE_BankBalanceByCashCode()	xSEE_BankBalanceByCashCode(__LINE__,__FILE__)
void xSEE_BankBalanceByCashCode(int,char *);

struct BankBalanceByCashCodes {
	char *status;
	double amt;
};
typedef struct BankBalanceByCashCodes BankBalanceByCashCode;

BankBalanceByCashCode *myBankBalanceByCashCode=NULL;
int numBankBalanceByCashCode=0;

void FreeBankBalanceByCashCode()
{
	BankBalanceByCashCode *e=NULL;
	int x=0;

	if(myBankBalanceByCashCode!=NULL)
	{
		for(x=0,e=myBankBalanceByCashCode;x<numBankBalanceByCashCode;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myBankBalanceByCashCode);
	}
	myBankBalanceByCashCode=NULL;
	numBankBalanceByCashCode=0;
}
void xSEE_BankBalanceByCashCode(int line,char *file)
{
	int x=0;
	BankBalanceByCashCode *e=NULL;

	fprintf(RDA_STDERR,"Showing BankBalanceByCashCode num [%d] at line [%d] program [%s]\n.",numBankBalanceByCashCode,line,file);
	if(myBankBalanceByCashCode!=NULL)
	{
		for(x=0,e=myBankBalanceByCashCode;x<numBankBalanceByCashCode;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Bank [%s] Bank Balance [%.02f] \n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addBankBalanceByCashCode(char *s,double revx)
{
	BankBalanceByCashCode *e=NULL;
	int x=0;
	
	if(myBankBalanceByCashCode!=NULL)
	{
		for(x=0,e=myBankBalanceByCashCode;x<numBankBalanceByCashCode;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numBankBalanceByCashCode)
		{
			myBankBalanceByCashCode=Rrealloc(myBankBalanceByCashCode,((numBankBalanceByCashCode+1)*sizeof(BankBalanceByCashCode)));
			e=myBankBalanceByCashCode+numBankBalanceByCashCode;
			++numBankBalanceByCashCode;
			e->status=stralloc(s);
			e->amt=0.0;
		}
	} else {
		myBankBalanceByCashCode=Rmalloc(sizeof(BankBalanceByCashCode));
		e=myBankBalanceByCashCode;
		numBankBalanceByCashCode=1;
		e->status=stralloc(s);
		e->amt=0.0;
	}
	e->amt+=revx;
}
short ReadBankBalanceByCashCode()
{
	int fileno=(-1),count=0,checknum=0,eftnum=0;
	short ef=FALSE;
	char delflag=FALSE,active1=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char checking=FALSE;
	double amt=0.0,uamt=0.0;

	fileno=OPNNRD("BNKREC","BNKACC");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"ACTIVE",&active1);
			if(!delflag && active1)
			{
				FINDFLDGETSTRING(fileno,"DEBIT CODE",&srcmod);
				FINDFLDGETDOUBLE(fileno,"BALANCE",&amt);
				FINDFLDGETDOUBLE(fileno,"UNPROCESSED BALANCE",&uamt);
				++count;
				addBankBalanceByCashCode(srcmod,amt+uamt);
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
void BankBalanceByCashCodeChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=BankBalanceByCashCodeDashBoard->parent();
	BankBalanceByCashCodeValuetoVisual=CB->currentIndex();
	BankBalanceByCashCodeShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=BankBalanceByCashCodeView();
	parent->addWidget(w);
}
Wt::WContainerWidget *BankBalanceByCashCodeView()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	BankBalanceByCashCode *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	double amt=0.0;
	int x=0,count=0;
	long y=0;
	char *temp=NULL,VPstemp[1024];

	myBankBalanceByCashCode=NULL;
	numBankBalanceByCashCode=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	BankBalanceByCashCodeDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(BankBalanceByCashCodeDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Bank Account Balances by Identification"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadBankBalanceByCashCode();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Bank Accounts."),BankBalanceByCashCodeDashBoard);
		return(BankBalanceByCashCodeDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Bank Accounts."),BankBalanceByCashCodeDashBoard);
		return(BankBalanceByCashCodeDashBoard);
	}
	if(BankBalanceByCashCodesModel!=NULL) BankBalanceByCashCodesModel->~WStandardItemModel();
	BankBalanceByCashCodesModel=new Wt::WStandardItemModel();
	BankBalanceByCashCodesModel->insertColumns(BankBalanceByCashCodesModel->columnCount(),2);
// headers 
	BankBalanceByCashCodesModel->setHeaderData(0,Wt::WString("Bank"));
	BankBalanceByCashCodesModel->setHeaderData(1,Wt::WString("Bank Balance"));
	BankBalanceByCashCodesModel->insertRows(BankBalanceByCashCodesModel->rowCount(),numBankBalanceByCashCode);
	count=0;
	amt=0.0;
	for(x=0,e=myBankBalanceByCashCode;x<numBankBalanceByCashCode;++x,++e)
	{
		BankBalanceByCashCodesModel->setData(x,0,Wt::WString(e->status));
		BankBalanceByCashCodesModel->setData(x,1,e->amt/100);
		amt+=e->amt;
	}
	BankBalanceByCashCodesModel->sort((BankBalanceByCashCodeValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	temp=famt(amt,20);
	sprintf(VPstemp,"Bank Balance: %s",temp);
	l=new Wt::WContainerWidget(BankBalanceByCashCodeDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(BankBalanceByCashCodeDashBoard);
	xchart->setModel(BankBalanceByCashCodesModel);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((BankBalanceByCashCodeValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(BankBalanceByCashCodeDashBoard);
	TablePanel->setTitle(Wt::WString("Bank Balance Table"));
	TablePanel->collapsed().connect(std::bind([=]() { BankBalanceByCashCodeShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { BankBalanceByCashCodeShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(BankBalanceByCashCodeShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(BankBalanceByCashCodesModel);
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
	FreeBankBalanceByCashCode();
	return(BankBalanceByCashCodeDashBoard);
}
