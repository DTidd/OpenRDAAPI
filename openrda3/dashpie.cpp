#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

extern Wt::WContainerWidget *MainWindowCenter;
extern Wt::WGridLayout *MWCL;
Wt::WStandardItemModel *ExpenditureUnPosted=NULL;
#define SEE_ExpendAct()	xSEE_ExpendAct(__LINE__,__FILE__)
void xSEE_ExpendAct(int,char *);

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

	fprintf(RDA_STDERR,"Showing Expendact num [%d] at line [%d] program [%s]\n.",numExpendAct,line,file);
	if(myExpendAct!=NULL)
	{
		for(x=0,e=myExpendAct;x<numExpendAct;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Source [%s] App [%.02f] Exp [%.02f] Enc [%.02f]\n",x,(e->source!=NULL ? e->source:""),e->app/100,e->exp/100,e->enc/100);
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
		fprintf(RDA_STDERR,"count [%d] ",count);TRACE;
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		return(-1);
	}
}
Wt::WContainerWidget *UnPostedExpenditureActivity()
{
	Wt::WContainerWidget *DashBoard=NULL,*w=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WText *text=NULL;
	ExpendAct *e=NULL;
	int x=0;

	myExpendAct=NULL;
	numExpendAct=0;
	DashBoard=new Wt::WContainerWidget();	
	text=new Wt::WText(Wt::WString("Unposted Expenditure Activity"),DashBoard);
	text->addStyleClass("OpenRDA h3");
	DashBoard->addWidget(text);
	x=ReadExpenditureActivity();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Expenditure Activity."),DashBoard);
		return(DashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Unposted Expenditure Activity to Classify."),DashBoard);
		return(DashBoard);
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
	for(x=0,e=myExpendAct;x<numExpendAct;++x,++e)
	{
		fprintf(RDA_STDERR,"x [%d] Source [%s] Count [%d] App [%.02f] Exp [%.02f] Enc [%.02f] ",x,e->source,e->count,e->app/100, e->exp/100,e->enc/100);TRACE;
		ExpenditureUnPosted->setData(x,0,Wt::WString(e->source));
		ExpenditureUnPosted->setData(x,1,e->count);
		ExpenditureUnPosted->setData(x,2,e->app/100);
		ExpenditureUnPosted->setData(x,3,e->exp/100);
		ExpenditureUnPosted->setData(x,4,e->enc/100);
	}
	w=new Wt::WContainerWidget(DashBoard);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(ExpenditureUnPosted);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setHeaderAlignment(0,AlignCenter);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
/*
		table->resize(150+100+14,20+6*22);
*/
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
/*
		table->resize(150+100+14,Wt::WLength::Auto);
*/
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	xchart=new Wt::Chart::WPieChart(DashBoard);
	xchart->setModel(ExpenditureUnPosted);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn(1);
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(800,300);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	SEE_ExpendAct();TRACE;
	FreeExpendAct();
	return(DashBoard);
}
