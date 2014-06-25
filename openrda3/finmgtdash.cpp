#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

extern short MODULE_FILENO,MENUITEM_FILENO;
Wt::WContainerWidget *UnPostedExpenditureActivityDashBoard=NULL;
Wt::WStandardItemModel *ExpenditureUnPosted=NULL;
#define SEE_ExpendAct()	xSEE_ExpendAct(__LINE__,__FILE__)
void xSEE_ExpendAct(int,char *);
Wt::WContainerWidget *UnPostedExpenditureActivity(void);

struct ExpendActs {
	char *source;
	int count;
	double app;
	double exp;
	double enc;
};
typedef struct ExpendActs ExpendAct;

ExpendAct *myExpendAct=NULL;
int numExpendAct=0;
int ExpendActValuetoVisual=0;
short ExpendActShowTable=FALSE;

void FreeExpendAct()
{
	ExpendAct *e=NULL;
	int x=0;

	if(myExpendAct!=NULL)
	{
		for(x=0,e=myExpendAct;x<numExpendAct;++x,++e)
		{
			if(e->source!=NULL) Rfree(e->source);
		}
		Rfree(myExpendAct);
	}
	myExpendAct=NULL;
	numExpendAct=0;
}
void xSEE_ExpendAct(int line,char *file)
{
	int x=0;
	ExpendAct *e=NULL;

	fprintf(RDA_STDERR,"Showing ExpendAct num [%d] at line [%d] program [%s]\n.",numExpendAct,line,file);
	if(myExpendAct!=NULL)
	{
		for(x=0,e=myExpendAct;x<numExpendAct;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Source [%s] Appropriations [%.02f] Expenditures [%.02f] Encumbrances [%.02f]\n",x,(e->source!=NULL ? e->source:""),e->app/100,e->exp/100,e->enc/100);
		}
	}
}
void addExpendAct(char *s,double appx,double expx,double encx)
{
	ExpendAct *e=NULL;
	int x=0;
	char *s2use=NULL;
	
	if(isEMPTY(s)) s2use=stralloc("Other");
		else s2use=stralloc(s);
	if(myExpendAct!=NULL)
	{
		for(x=0,e=myExpendAct;x<numExpendAct;++x,++e)
		{
			if(!RDAstrcmp(s2use,e->source)) break;
		}
		if(x>=numExpendAct)
		{
			myExpendAct=Rrealloc(myExpendAct,((numExpendAct+1)*sizeof(ExpendAct)));
			e=myExpendAct+numExpendAct;
			++numExpendAct;
			e->source=stralloc(s2use);
			e->count=0;
			e->app=0.0;
			e->exp=0.0;
			e->enc=0.0;
		}
	} else {
		myExpendAct=Rmalloc(sizeof(ExpendAct));
		e=myExpendAct;
		numExpendAct=1;
		e->source=stralloc(s2use);
		e->count=0;
		e->app=0.0;
		e->exp=0.0;
		e->enc=0.0;
	}
	++e->count;
	e->app+=appx;
	e->exp+=expx;
	e->enc+=encx;
	if(s2use!=NULL) Rfree(s2use);
}
short ReadExpenditureActivity()
{
	int fileno=(-1),count=0;
	short ef=FALSE,type=0;
	char delflag=FALSE,posted=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	double amt=0.0;

	fileno=OPNNRD("FINMGT","FINEACT");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"POSTED",&posted);
			if(!delflag && !posted)
			{
				++count;
				FINDFLDGETSTRING(fileno,"SOURCE MODULE",&srcmod);
				FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
				FINDFLDGETSHORT(fileno,"TYPE",&type);
				switch(type)
				{
					default:
					case 0:
						addExpendAct(srcmod,0,amt,0);
						break;
					case 1:
						addExpendAct(srcmod,amt,0,0);
						break;
					case 2:
						addExpendAct(srcmod,0,0,amt);
						break;
				}
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
void ExpendActChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=UnPostedExpenditureActivityDashBoard->parent();
	ExpendActValuetoVisual=CB->currentIndex();
	ExpendActShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=UnPostedExpenditureActivity();
	parent->addWidget(w);
}
Wt::WContainerWidget *UnPostedExpenditureActivity()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	ExpendAct *e=NULL;
	Wt::WItemDelegate *myID=NULL;
	Wt::WLength spc;
	char *temp=NULL,*temp1=NULL,*temp2=NULL,FMstemp[1024];
	double appamt=0.0,expamt=0.0,encamt=0.0;
	int x=0,count=0;
	long y=0;

	myExpendAct=NULL;
	numExpendAct=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnPostedExpenditureActivityDashBoard=new Wt::WContainerWidget();	
	UnPostedExpenditureActivityDashBoard->setPadding(spc,All);
	l=new Wt::WContainerWidget(UnPostedExpenditureActivityDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unposted Expenditure Activity"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);

	x=ReadExpenditureActivity();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Expenditure Activity."),UnPostedExpenditureActivityDashBoard);
		return(UnPostedExpenditureActivityDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unposted Expenditure Activity to Classify."),UnPostedExpenditureActivityDashBoard);
		return(UnPostedExpenditureActivityDashBoard);
	}
	if(ExpenditureUnPosted!=NULL) ExpenditureUnPosted->~WStandardItemModel();
	ExpenditureUnPosted=new Wt::WStandardItemModel();
	ExpenditureUnPosted->insertColumns(ExpenditureUnPosted->columnCount(),5);
// headers 
	ExpenditureUnPosted->setHeaderData(0,Wt::WString("Source Module"));
	ExpenditureUnPosted->setHeaderData(1,Wt::WString("Number of Records"));
	ExpenditureUnPosted->setHeaderData(2,Wt::WString("Appropriations"));		
	ExpenditureUnPosted->setHeaderData(3,Wt::WString("Expenditures"));
	ExpenditureUnPosted->setHeaderData(4,Wt::WString("Encumbrances"));
	ExpenditureUnPosted->insertRows(ExpenditureUnPosted->rowCount(),numExpendAct);
	count=0;
	appamt=0.0;
	expamt=0.0;
	encamt=0.0;
	for(x=0,e=myExpendAct;x<numExpendAct;++x,++e)
	{
		ZERNRD(MODULE_FILENO);
		FINDFLDSETSTRING(MODULE_FILENO,"MODULE NAME",e->source);
		GTENRD(MODULE_FILENO,2);
		FINDFLDGETSTRING(MODULE_FILENO,"DESCRIPTION",&temp);
		ExpenditureUnPosted->setData(x,0,Wt::WString(temp));
		if(temp!=NULL) Rfree(temp);
		count+=e->count;
		ExpenditureUnPosted->setData(x,1,e->count);
		ExpenditureUnPosted->setData(x,2,e->app/100);
		appamt+=e->app;
		ExpenditureUnPosted->setData(x,3,e->exp/100);
		expamt+=e->exp;
		ExpenditureUnPosted->setData(x,4,e->enc/100);
		encamt+=e->enc;
	}
	memset(FMstemp,0,1024);
	temp=famt(appamt,20);
	temp1=famt(expamt,20);
	temp2=famt(encamt,20);
	sprintf(FMstemp,"Records: %8d Appropriations: %s Expenditures: %s Encumbrances: %s",count,temp,temp1,temp2);
	l=new Wt::WContainerWidget(UnPostedExpenditureActivityDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(FMstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	ExpenditureUnPosted->sort((ExpendActValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	xchart=new Wt::Chart::WPieChart(UnPostedExpenditureActivityDashBoard);
	xchart->setModel(ExpenditureUnPosted);
	xchart->setLabelsColumn(0);
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDataColumn((ExpendActValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnPostedExpenditureActivityDashBoard);
	TablePanel->setTitle(Wt::WString("UnPosted Expenditure Activity Table"));
	TablePanel->collapsed().connect(std::bind([=]() { ExpendActShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { ExpendActShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(ExpendActShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	w->setPadding(spc,All);
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(ExpenditureUnPosted);
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
	table->setColumnAlignment(3,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(3,myID);
	table->setColumnAlignment(4,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(4,myID);
	table->setColumnWidth(0,350);
	table->setHeaderAlignment(0,AlignCenter);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	l=new Wt::WContainerWidget(w);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of Records"));
	CB->addItem(Wt::WString("Appropriations"));
	CB->addItem(Wt::WString("Expenditures"));
	CB->addItem(Wt::WString("Encumbrances"));
	CB->setCurrentIndex(ExpendActValuetoVisual);
	CB->activated().connect(boost::bind(&ExpendActChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeExpendAct();
	return(UnPostedExpenditureActivityDashBoard);
}

Wt::WContainerWidget *UnPostedRevenueActivityDashBoard=NULL;
Wt::WStandardItemModel *RevenueUnPosted=NULL;
#define SEE_RevenueAct()	xSEE_RevenueAct(__LINE__,__FILE__)
void xSEE_RevenueAct(int,char *);
Wt::WContainerWidget *UnPostedRevenueActivity(void);

struct RevenueActs {
	char *source;
	int count;
	double erev;
	double rev;
};
typedef struct RevenueActs RevenueAct;

RevenueAct *myRevenueAct=NULL;
int numRevenueAct=0;
int RevenueActValuetoVisual=0;
short RevenueActShowTable=FALSE;

void FreeRevenueAct()
{
	RevenueAct *e=NULL;
	int x=0;

	if(myRevenueAct!=NULL)
	{
		for(x=0,e=myRevenueAct;x<numRevenueAct;++x,++e)
		{
			if(e->source!=NULL) Rfree(e->source);
		}
		Rfree(myRevenueAct);
	}
	myRevenueAct=NULL;
	numRevenueAct=0;
}
void xSEE_RevenueAct(int line,char *file)
{
	int x=0;
	RevenueAct *e=NULL;

	fprintf(RDA_STDERR,"Showing RevenueAct num [%d] at line [%d] program [%s]\n.",numRevenueAct,line,file);
	if(myRevenueAct!=NULL)
	{
		for(x=0,e=myRevenueAct;x<numRevenueAct;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Source [%s] Estimated Revenue [%.02f] Revenue [%.02f]\n",x,(e->source!=NULL ? e->source:""),e->erev/100,e->rev/100);
		}
	}
}
void addRevenueAct(char *s,double erevx,double revx)
{
	RevenueAct *e=NULL;
	int x=0;
	char *s2use=NULL;
	
	if(isEMPTY(s)) s2use=stralloc("Other");
		else s2use=stralloc(s);
	if(myRevenueAct!=NULL)
	{
		for(x=0,e=myRevenueAct;x<numRevenueAct;++x,++e)
		{
			if(!RDAstrcmp(s2use,e->source)) break;
		}
		if(x>=numRevenueAct)
		{
			myRevenueAct=Rrealloc(myRevenueAct,((numRevenueAct+1)*sizeof(RevenueAct)));
			e=myRevenueAct+numRevenueAct;
			++numRevenueAct;
			e->source=stralloc(s2use);
			e->count=0;
			e->erev=0.0;
			e->rev=0.0;
		}
	} else {
		myRevenueAct=Rmalloc(sizeof(RevenueAct));
		e=myRevenueAct;
		numRevenueAct=1;
		e->source=stralloc(s2use);
		e->count=0;
		e->erev=0.0;
		e->rev=0.0;
	}
	++e->count;
	e->erev+=erevx;
	e->rev+=revx;
	if(s2use!=NULL) Rfree(s2use);
}
short ReadRevenueActivity()
{
	int fileno=(-1),count=0;
	short ef=FALSE,type=0;
	char delflag=FALSE,posted=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	double amt=0.0;

	fileno=OPNNRD("FINMGT","FINRACT");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"POSTED",&posted);
			if(!delflag && !posted)
			{
				++count;
				FINDFLDGETSTRING(fileno,"SOURCE MODULE",&srcmod);
				FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
				FINDFLDGETSHORT(fileno,"TYPE",&type);
				switch(type)
				{
					default:
					case 0:
						addRevenueAct(srcmod,0,amt);
						break;
					case 1:
						addRevenueAct(srcmod,amt,0);
						break;
				}
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
void RevenueActChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=UnPostedRevenueActivityDashBoard->parent();
	RevenueActValuetoVisual=CB->currentIndex();
	RevenueActShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=UnPostedRevenueActivity();
	parent->addWidget(w);
}
Wt::WContainerWidget *UnPostedRevenueActivity()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	RevenueAct *e=NULL;
	Wt::WItemDelegate *myID=NULL;
	Wt::WLength spc;
	double erevamt=0.0,revamt=0.0;
	char *temp=NULL,*temp1=NULL,FMstemp[1024];
	int x=0,count;
	long y=0;

	myRevenueAct=NULL;
	numRevenueAct=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnPostedRevenueActivityDashBoard=new Wt::WContainerWidget();	
	UnPostedRevenueActivityDashBoard->setPadding(spc,All);
	l=new Wt::WContainerWidget(UnPostedRevenueActivityDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unposted Revenue Activity"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadRevenueActivity();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Revenue Activity."),UnPostedRevenueActivityDashBoard);
		return(UnPostedRevenueActivityDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unposted Revenue Activity to Classify."),UnPostedRevenueActivityDashBoard);
		return(UnPostedRevenueActivityDashBoard);
	}
	if(RevenueUnPosted!=NULL) RevenueUnPosted->~WStandardItemModel();
	RevenueUnPosted=new Wt::WStandardItemModel();
	RevenueUnPosted->insertColumns(RevenueUnPosted->columnCount(),4);
// headers 
	RevenueUnPosted->setHeaderData(0,Wt::WString("Source Module"));
	RevenueUnPosted->setHeaderData(1,Wt::WString("Number of Records"));
	RevenueUnPosted->setHeaderData(2,Wt::WString("Estimated Revenue"));		
	RevenueUnPosted->setHeaderData(3,Wt::WString("Revenue"));
	RevenueUnPosted->insertRows(RevenueUnPosted->rowCount(),numRevenueAct);
	count=0;
	erevamt=0.0;
	revamt=0.0;
	for(x=0,e=myRevenueAct;x<numRevenueAct;++x,++e)
	{
		ZERNRD(MODULE_FILENO);
		FINDFLDSETSTRING(MODULE_FILENO,"MODULE NAME",e->source);
		GTENRD(MODULE_FILENO,2);
		FINDFLDGETSTRING(MODULE_FILENO,"DESCRIPTION",&temp);
		RevenueUnPosted->setData(x,0,Wt::WString(temp));
		if(temp!=NULL) Rfree(temp);
		count+=e->count;
		RevenueUnPosted->setData(x,1,e->count);
		erevamt+=e->erev;
		RevenueUnPosted->setData(x,2,e->erev/100);
		revamt+=e->rev;
		RevenueUnPosted->setData(x,3,e->rev/100);
	}
	RevenueUnPosted->sort((RevenueActValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(FMstemp,0,1024);
	temp=famt(erevamt,20);
	temp1=famt(revamt,20);
	sprintf(FMstemp,"Records: %8d Estimated Revenue: %s Revenue: %s",count,temp,temp1);
	l=new Wt::WContainerWidget(UnPostedRevenueActivityDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(FMstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	xchart=new Wt::Chart::WPieChart(UnPostedRevenueActivityDashBoard);
	xchart->setModel(RevenueUnPosted);
	xchart->setLabelsColumn(0);
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDataColumn((RevenueActValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnPostedRevenueActivityDashBoard);
	TablePanel->setTitle(Wt::WString("UnPosted Revenue Activity Table"));
	TablePanel->collapsed().connect(std::bind([=]() { RevenueActShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { RevenueActShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(RevenueActShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	w->setPadding(spc,All);
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(RevenueUnPosted);
	table->setRowHeight(22);
	table->setColumnWidth(0,250);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%d");
	table->setItemDelegateForColumn(1,myID);
	table->setColumnAlignment(2,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(2,myID);
	table->setColumnAlignment(3,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(3,myID);
	table->setHeaderAlignment(0,AlignCenter);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	l=new Wt::WContainerWidget(w);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of Records"));
	CB->addItem(Wt::WString("Estimated Revenue"));
	CB->addItem(Wt::WString("Revenue"));
	CB->setCurrentIndex(RevenueActValuetoVisual);
	CB->activated().connect(boost::bind(&RevenueActChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeRevenueAct();
	return(UnPostedRevenueActivityDashBoard);
}

Wt::WContainerWidget *UnProcessedJournalEntryDashBoard=NULL;
Wt::WStandardItemModel *JournalEntriesUnProcessed=NULL;
#define SEE_JournalEntry()	xSEE_JournalEntry(__LINE__,__FILE__)
void xSEE_JournalEntry(int,char *);
Wt::WContainerWidget *UnProcessedJournalEntry(void);

struct JournalEntrys {
	char *status;
	int count;
	double amt;
};
typedef struct JournalEntrys JournalEntry;

JournalEntry *myJournalEntry=NULL;
int numJournalEntry=0;
int JournalEntryValuetoVisual=0;
short JournalEntryShowTable=FALSE;

void FreeJournalEntry()
{
	JournalEntry *e=NULL;
	int x=0;

	if(myJournalEntry!=NULL)
	{
		for(x=0,e=myJournalEntry;x<numJournalEntry;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myJournalEntry);
	}
	myJournalEntry=NULL;
	numJournalEntry=0;
}
void xSEE_JournalEntry(int line,char *file)
{
	int x=0;
	JournalEntry *e=NULL;

	fprintf(RDA_STDERR,"Showing JournalEntry num [%d] at line [%d] program [%s]\n.",numJournalEntry,line,file);
	if(myJournalEntry!=NULL)
	{
		for(x=0,e=myJournalEntry;x<numJournalEntry;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s] Amount [%.02f]\n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addJournalEntry(char *s,double revx)
{
	JournalEntry *e=NULL;
	int x=0;
	
	if(myJournalEntry!=NULL)
	{
		for(x=0,e=myJournalEntry;x<numJournalEntry;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numJournalEntry)
		{
			myJournalEntry=Rrealloc(myJournalEntry,((numJournalEntry+1)*sizeof(JournalEntry)));
			e=myJournalEntry+numJournalEntry;
			++numJournalEntry;
			e->status=stralloc(s);
			e->count=0;
			e->amt=0.0;
		}
	} else {
		myJournalEntry=Rmalloc(sizeof(JournalEntry));
		e=myJournalEntry;
		numJournalEntry=1;
		e->status=stralloc(s);
		e->count=0;
		e->amt=0.0;
	}
	++e->count;
	e->amt+=revx;
}
short ReadJournalEntryStatus()
{
	int fileno=(-1),count=0;
	short ef=FALSE;
	char delflag=FALSE,distributed=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char verified=FALSE;
	double amt=0.0;

	fileno=OPNNRD("FINMGT","FINJEN");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"DISTRIBUTED",&distributed);
			if(!delflag && !distributed)
			{
				FINDFLDGETCHAR(fileno,"SOURCE USER VERIFIED",&verified);
				if(!verified) srcmod="Added waiting for Verification";
				else if(!distributed) srcmod="Verified waiting for Distribution";
				++count;
				FINDFLDGETSTRING(fileno,"SOURCE MODULE",&srcmod);
				FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
				addJournalEntry(srcmod,amt);
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
void JournalEntryChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=UnProcessedJournalEntryDashBoard->parent();
	JournalEntryValuetoVisual=CB->currentIndex();
	JournalEntryShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=UnProcessedJournalEntry();
	parent->addWidget(w);
}
Wt::WContainerWidget *UnProcessedJournalEntry()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	JournalEntry *e=NULL;
	Wt::WItemDelegate *myID=NULL;
	Wt::WLength spc;
	double amt=0.0;
	char *temp=NULL,FMstemp[1024];
	int x=0,count=0;
	long y=0;

	myJournalEntry=NULL;
	numJournalEntry=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnProcessedJournalEntryDashBoard=new Wt::WContainerWidget();	
	UnProcessedJournalEntryDashBoard->setPadding(spc,All);

	l=new Wt::WContainerWidget(UnProcessedJournalEntryDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unprocessed Journal Entries"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadJournalEntryStatus();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Journal Entries."),UnProcessedJournalEntryDashBoard);
		return(UnProcessedJournalEntryDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unprocessed Journal Entries."),UnProcessedJournalEntryDashBoard);
		return(UnProcessedJournalEntryDashBoard);
	}
	if(JournalEntriesUnProcessed!=NULL) JournalEntriesUnProcessed->~WStandardItemModel();
	JournalEntriesUnProcessed=new Wt::WStandardItemModel();
	JournalEntriesUnProcessed->insertColumns(JournalEntriesUnProcessed->columnCount(),3);
// headers 
	JournalEntriesUnProcessed->setHeaderData(0,Wt::WString("Status"));
	JournalEntriesUnProcessed->setHeaderData(1,Wt::WString("Number of Records"));
	JournalEntriesUnProcessed->setHeaderData(2,Wt::WString("Amount"));
	JournalEntriesUnProcessed->insertRows(JournalEntriesUnProcessed->rowCount(),numJournalEntry);
	count=0;
	amt=0.0;
	for(x=0,e=myJournalEntry;x<numJournalEntry;++x,++e)
	{
		JournalEntriesUnProcessed->setData(x,0,Wt::WString(e->status));
		count+=e->count;
		JournalEntriesUnProcessed->setData(x,1,e->count);
		amt+=e->amt;
		JournalEntriesUnProcessed->setData(x,2,e->amt/100);
	}
	memset(FMstemp,0,1024);
	temp=famt(amt,20);
	sprintf(FMstemp,"Records: %8d Total: %s",count,temp);
	l=new Wt::WContainerWidget(UnProcessedJournalEntryDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(FMstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	JournalEntriesUnProcessed->sort((JournalEntryValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	xchart=new Wt::Chart::WPieChart(UnProcessedJournalEntryDashBoard);
	xchart->setModel(JournalEntriesUnProcessed);
	xchart->setLabelsColumn(0);
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDataColumn((JournalEntryValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnProcessedJournalEntryDashBoard);
	TablePanel->setTitle(Wt::WString("Unprocessed Journal Entry Table"));
	TablePanel->collapsed().connect(std::bind([=]() { JournalEntryShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { JournalEntryShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(JournalEntryShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	w->setPadding(spc,All);
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(JournalEntriesUnProcessed);
	table->setRowHeight(22);
	table->setColumnWidth(1,250);
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
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of Records"));
	CB->addItem(Wt::WString("Amount"));
	CB->setCurrentIndex(JournalEntryValuetoVisual);
	CB->activated().connect(boost::bind(&JournalEntryChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeJournalEntry();
	return(UnProcessedJournalEntryDashBoard);
}
Wt::WContainerWidget *UnProcessedReceiptDashBoard=NULL;
Wt::WStandardItemModel *ReceiptsUnProcessed=NULL;
#define SEE_Receipt()	xSEE_Receipt(__LINE__,__FILE__)
void xSEE_Receipt(int,char *);
Wt::WContainerWidget *UnProcessedReceipt(void);

struct Receipts {
	char *status;
	int count;
	double amt;
};
typedef struct Receipts Receipt;

Receipt *myReceipt=NULL;
int numReceipt=0;
int ReceiptValuetoVisual=0;
short ReceiptShowTable=FALSE;

void FreeReceipt()
{
	Receipt *e=NULL;
	int x=0;

	if(myReceipt!=NULL)
	{
		for(x=0,e=myReceipt;x<numReceipt;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myReceipt);
	}
	myReceipt=NULL;
	numReceipt=0;
}
void xSEE_Receipt(int line,char *file)
{
	int x=0;
	Receipt *e=NULL;

	fprintf(RDA_STDERR,"Showing Receipt num [%d] at line [%d] program [%s]\n.",numReceipt,line,file);
	if(myReceipt!=NULL)
	{
		for(x=0,e=myReceipt;x<numReceipt;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s] Amount [%.02f]\n",x,(e->status!=NULL ? e->status:""),e->amt/100);
		}
	}
}
void addReceipt(char *s,double revx)
{
	Receipt *e=NULL;
	int x=0;
	
	if(myReceipt!=NULL)
	{
		for(x=0,e=myReceipt;x<numReceipt;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numReceipt)
		{
			myReceipt=Rrealloc(myReceipt,((numReceipt+1)*sizeof(Receipt)));
			e=myReceipt+numReceipt;
			++numReceipt;
			e->status=stralloc(s);
			e->count=0;
			e->amt=0.0;
		}
	} else {
		myReceipt=Rmalloc(sizeof(Receipt));
		e=myReceipt;
		numReceipt=1;
		e->status=stralloc(s);
		e->count=0;
		e->amt=0.0;
	}
	++e->count;
	e->amt+=revx;
}
short ReadReceiptStatus()
{
	int fileno=(-1),count=0;
	short ef=FALSE;
	char delflag=FALSE,distributed=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char verified=FALSE,bnkrec=FALSE;
	double amt=0.0;

	fileno=OPNNRD("FINMGT","FINREC");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"DISTRIBUTED",&distributed);
			FINDFLDGETCHAR(fileno,"BANK RECONCILIATION UPDATED",&bnkrec);
			if(!delflag && (!distributed || !bnkrec))
			{
				FINDFLDGETCHAR(fileno,"SOURCE USER VERIFIED",&verified);
				if(!verified) srcmod="Added waiting for Verification";
				else if(!distributed && !bnkrec) srcmod="Verified waiting for BnkRec Updated";
				else if(!distributed && bnkrec) srcmod="Bank Rec Updated waiting for Distribution";
				else if(!bnkrec && distributed) srcmod="Distributed waiting BnkRec Updated";
				++count;
				FINDFLDGETSTRING(fileno,"SOURCE MODULE",&srcmod);
				FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
				addReceipt(srcmod,amt);
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
void ReceiptChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=UnProcessedReceiptDashBoard->parent();
	ReceiptValuetoVisual=CB->currentIndex();
	ReceiptShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=UnProcessedReceipt();
	parent->addWidget(w);
}
Wt::WContainerWidget *UnProcessedReceipt()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	Wt::WItemDelegate *myID=NULL;
	Wt::WLength spc;
	Receipt *e=NULL;
	double amt=0.0;
	char *temp=NULL,FMstemp[1024];
	int x=0,count=0;
	long y=0;

	myReceipt=NULL;
	numReceipt=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnProcessedReceiptDashBoard=new Wt::WContainerWidget();	
	UnProcessedReceiptDashBoard->setPadding(spc,All);
	l=new Wt::WContainerWidget(UnProcessedReceiptDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unprocessed Receipts"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadReceiptStatus();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Receipts."),UnProcessedReceiptDashBoard);
		return(UnProcessedReceiptDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unprocessed Receipts."),UnProcessedReceiptDashBoard);
		return(UnProcessedReceiptDashBoard);
	}
	if(ReceiptsUnProcessed!=NULL) ReceiptsUnProcessed->~WStandardItemModel();
	ReceiptsUnProcessed=new Wt::WStandardItemModel();
	ReceiptsUnProcessed->insertColumns(ReceiptsUnProcessed->columnCount(),3);
// headers 
	ReceiptsUnProcessed->setHeaderData(0,Wt::WString("Status"));
	ReceiptsUnProcessed->setHeaderData(1,Wt::WString("Number of Records"));
	ReceiptsUnProcessed->setHeaderData(2,Wt::WString("Amount"));
	ReceiptsUnProcessed->insertRows(ReceiptsUnProcessed->rowCount(),numReceipt);
	count=0;
	amt=0.0;
	for(x=0,e=myReceipt;x<numReceipt;++x,++e)
	{
		ReceiptsUnProcessed->setData(x,0,Wt::WString(e->status));
		count+=e->count;
		ReceiptsUnProcessed->setData(x,1,e->count);
		amt+=e->amt;
		ReceiptsUnProcessed->setData(x,2,e->amt/100);
	}
	ReceiptsUnProcessed->sort((ReceiptValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(FMstemp,0,1024);
	temp=famt(amt,20);
	sprintf(FMstemp,"Records: %8d Total: %s",count,temp);
	l=new Wt::WContainerWidget(UnProcessedReceiptDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(FMstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(UnProcessedReceiptDashBoard);
	xchart->setModel(ReceiptsUnProcessed);
	xchart->setLabelsColumn(0);
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDataColumn((ReceiptValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnProcessedReceiptDashBoard);
	TablePanel->setTitle(Wt::WString("Unprocessed Receipt Table"));
	TablePanel->collapsed().connect(std::bind([=]() { ReceiptShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { ReceiptShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(ReceiptShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	w->setPadding(spc,All);
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(ReceiptsUnProcessed);
	table->setRowHeight(22);
	table->setColumnWidth(0,350);
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
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of Records"));
	CB->addItem(Wt::WString("Amount"));
	CB->setCurrentIndex(ReceiptValuetoVisual);
	CB->activated().connect(boost::bind(&ReceiptChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeReceipt();
	return(UnProcessedReceiptDashBoard);
}


Wt::WContainerWidget *UnPostedGeneralLedgerActivityDashBoard=NULL;
Wt::WStandardItemModel *GeneralLedgerUnPosted=NULL;
#define SEE_GenLdgrAct()	xSEE_GenLdgrAct(__LINE__,__FILE__)
void xSEE_GenLdgrAct(int,char *);
Wt::WContainerWidget *UnPostedGeneralLedgerActivity(void);

struct GenLdgrActs {
	char *source;
	int count;
	double debits;
	double credits;
	double ending;
};
typedef struct GenLdgrActs GenLdgrAct;

GenLdgrAct *myGenLdgrAct=NULL;
int numGenLdgrAct=0;
int GenLdgrActValuetoVisual=0;
short GenLdgrActShowTable=FALSE;

void FreeGenLdgrAct()
{
	GenLdgrAct *e=NULL;
	int x=0;

	if(myGenLdgrAct!=NULL)
	{
		for(x=0,e=myGenLdgrAct;x<numGenLdgrAct;++x,++e)
		{
			if(e->source!=NULL) Rfree(e->source);
		}
		Rfree(myGenLdgrAct);
	}
	myGenLdgrAct=NULL;
	numGenLdgrAct=0;
}
void xSEE_GenLdgrAct(int line,char *file)
{
	int x=0;
	GenLdgrAct *e=NULL;

	fprintf(RDA_STDERR,"Showing GenLdgrAct num [%d] at line [%d] program [%s]\n.",numGenLdgrAct,line,file);
	if(myGenLdgrAct!=NULL)
	{
		for(x=0,e=myGenLdgrAct;x<numGenLdgrAct;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Source [%s] Debits [%.02f] Credits [%.02f] Ending Balance [%.02f] %s\n",x,(e->source!=NULL ? e->source:""),e->debits/100,e->credits/100,e->ending/100,(e->ending!=0 ? "Out of Balance!":""));
		}
	}
}
void addGenLdgrAct(char *s,double debitsx,double creditsx)
{
	GenLdgrAct *e=NULL;
	int x=0;
	char *s2use=NULL;
	double endingx=debitsx-creditsx;
	
	if(isEMPTY(s)) s2use=stralloc("Other");
		else s2use=stralloc(s);
	if(myGenLdgrAct!=NULL)
	{
		for(x=0,e=myGenLdgrAct;x<numGenLdgrAct;++x,++e)
		{
			if(!RDAstrcmp(s2use,e->source)) break;
		}
		if(x>=numGenLdgrAct)
		{
			myGenLdgrAct=Rrealloc(myGenLdgrAct,((numGenLdgrAct+1)*sizeof(GenLdgrAct)));
			e=myGenLdgrAct+numGenLdgrAct;
			++numGenLdgrAct;
			e->source=stralloc(s2use);
			e->count=0;
			e->debits=0.0;
			e->credits=0.0;
			e->ending=0.0;
		}
	} else {
		myGenLdgrAct=Rmalloc(sizeof(GenLdgrAct));
		e=myGenLdgrAct;
		numGenLdgrAct=1;
		e->source=stralloc(s2use);
		e->count=0;
		e->debits=0.0;
		e->credits=0.0;
		e->ending=0.0;
	}
	++e->count;
	e->debits+=debitsx;
	e->credits+=creditsx;
	e->ending+=endingx;
	if(s2use!=NULL) Rfree(s2use);
}
short ReadGeneralLedgerActivity()
{
	int fileno=(-1),count=0;
	short ef=FALSE,type=0;
	char delflag=FALSE,posted=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	double amt=0.0;

	fileno=OPNNRD("FINMGT","FINBACT");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"POSTED",&posted);
			if(!delflag && !posted)
			{
				++count;
				FINDFLDGETSTRING(fileno,"SOURCE MODULE",&srcmod);
				FINDFLDGETDOUBLE(fileno,"AMOUNT",&amt);
				FINDFLDGETSHORT(fileno,"TYPE",&type);
				switch(type)
				{
					default:
					case 0:
						addGenLdgrAct(srcmod,amt,0);
						break;
					case 1:
						addGenLdgrAct(srcmod,0,amt);
						break;
				}
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
void GenLdgrActChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=UnPostedGeneralLedgerActivityDashBoard->parent();
	GenLdgrActValuetoVisual=CB->currentIndex();
	GenLdgrActShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=UnPostedGeneralLedgerActivity();
	parent->addWidget(w);
}
Wt::WContainerWidget *UnPostedGeneralLedgerActivity()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	GenLdgrAct *e=NULL;
	Wt::WItemDelegate *myID=NULL;
	Wt::WLength spc;
	double debamt=0.0,credamt=0.0,endamt=0.0;
	char *temp=NULL,*temp1=NULL,*temp2=NULL,FMstemp[1024];
	int x=0,count=0;
	long y=0;

	myGenLdgrAct=NULL;
	numGenLdgrAct=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	UnPostedGeneralLedgerActivityDashBoard=new Wt::WContainerWidget();	
	UnPostedGeneralLedgerActivityDashBoard->setPadding(spc,All);
	l=new Wt::WContainerWidget(UnPostedGeneralLedgerActivityDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Unposted General Ledger Activity"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadGeneralLedgerActivity();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Balance Sheet Activity."),UnPostedGeneralLedgerActivityDashBoard);
		return(UnPostedGeneralLedgerActivityDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unposted Balance Sheet Activity to Classify."),UnPostedGeneralLedgerActivityDashBoard);
		return(UnPostedGeneralLedgerActivityDashBoard);
	}
	if(GeneralLedgerUnPosted!=NULL) GeneralLedgerUnPosted->~WStandardItemModel();
	GeneralLedgerUnPosted=new Wt::WStandardItemModel();
	GeneralLedgerUnPosted->insertColumns(GeneralLedgerUnPosted->columnCount(),6);
// headers 
	GeneralLedgerUnPosted->setHeaderData(0,Wt::WString("Source Module"));
	GeneralLedgerUnPosted->setHeaderData(1,Wt::WString("Number of Records"));
	GeneralLedgerUnPosted->setHeaderData(2,Wt::WString("Debits"));		
	GeneralLedgerUnPosted->setHeaderData(3,Wt::WString("Credits"));
	GeneralLedgerUnPosted->setHeaderData(4,Wt::WString("Ending Balance"));
	GeneralLedgerUnPosted->setHeaderData(5,Wt::WString("Remarks"));
	GeneralLedgerUnPosted->insertRows(GeneralLedgerUnPosted->rowCount(),numGenLdgrAct);
	count=0;
	debamt=0.0;
	credamt=0.0;
	endamt=0.0;
	for(x=0,e=myGenLdgrAct;x<numGenLdgrAct;++x,++e)
	{
		ZERNRD(MODULE_FILENO);
		FINDFLDSETSTRING(MODULE_FILENO,"MODULE NAME",e->source);
		GTENRD(MODULE_FILENO,2);
		FINDFLDGETSTRING(MODULE_FILENO,"DESCRIPTION",&temp);
		GeneralLedgerUnPosted->setData(x,0,Wt::WString(temp));
		if(temp!=NULL) Rfree(temp);
		GeneralLedgerUnPosted->setData(x,1,e->count);
		count+=e->count;
		GeneralLedgerUnPosted->setData(x,2,e->debits/100);
		debamt+=e->debits;
		GeneralLedgerUnPosted->setData(x,3,e->credits/100);
		credamt+=e->credits;
		GeneralLedgerUnPosted->setData(x,4,e->ending/100);
		endamt+=e->ending;
		GeneralLedgerUnPosted->setData(x,5,Wt::WString((e->ending!=0 ? "Out of Balance!":"")));
	}
	GeneralLedgerUnPosted->sort((GenLdgrActValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(FMstemp,0,1024);
	temp=famt(debamt,20);
	temp1=famt(credamt,20);
	temp2=famt(endamt,20);
	sprintf(FMstemp,"Records: %8d Debits: %s Credits: %s Ending: %s",count,temp,temp1,temp2);
	l=new Wt::WContainerWidget(UnPostedGeneralLedgerActivityDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(FMstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	xchart=new Wt::Chart::WPieChart(UnPostedGeneralLedgerActivityDashBoard);
	xchart->setModel(GeneralLedgerUnPosted);
	xchart->setLabelsColumn(0);
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDataColumn((GenLdgrActValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(UnPostedGeneralLedgerActivityDashBoard);
	TablePanel->setTitle(Wt::WString("UnPosted General Ledger Activity Table"));
	TablePanel->collapsed().connect(std::bind([=]() { GenLdgrActShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { GenLdgrActShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(GenLdgrActShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	w->setPadding(spc,All);
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(GeneralLedgerUnPosted);
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
	table->setColumnAlignment(3,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(3,myID);
	table->setColumnAlignment(4,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(4,myID);
	table->setColumnWidth(0,350);
	table->setHeaderAlignment(0,AlignCenter);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	l=new Wt::WContainerWidget(w);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of Records"));
	CB->addItem(Wt::WString("Debits"));
	CB->addItem(Wt::WString("Credits"));
	CB->addItem(Wt::WString("Ending Balance"));
	CB->setCurrentIndex(GenLdgrActValuetoVisual);
	CB->activated().connect(boost::bind(&GenLdgrActChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeGenLdgrAct();
	return(UnPostedGeneralLedgerActivityDashBoard);
}
