/* Fixed Assets Dashboards */
#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

Wt::WContainerWidget *FixedAssetsByCategoryView(void);
int FixedAssetsByCategoryValuetoVisual=1;
short FixedAssetsByCategoryShowTable=FALSE;
Wt::WContainerWidget *FixedAssetsByCategoryDashBoard=NULL;
Wt::WStandardItemModel *FixedAssetsByCategorysUnProcessed=NULL;
#define SEE_FixedAssetsByCategory()	xSEE_FixedAssetsByCategory(__LINE__,__FILE__)
void xSEE_FixedAssetsByCategory(int,char *);

struct FixedAssetsByCategorys {
	char *status;
	double qty;
	double amt;
};
typedef struct FixedAssetsByCategorys FixedAssetsByCategory;

FixedAssetsByCategory *myFixedAssetsByCategory=NULL;
int numFixedAssetsByCategory=0;

void FreeFixedAssetsByCategory()
{
	FixedAssetsByCategory *e=NULL;
	int x=0;

	if(myFixedAssetsByCategory!=NULL)
	{
		for(x=0,e=myFixedAssetsByCategory;x<numFixedAssetsByCategory;++x,++e)
		{
			if(e->status!=NULL) Rfree(e->status);
		}
		Rfree(myFixedAssetsByCategory);
	}
	myFixedAssetsByCategory=NULL;
	numFixedAssetsByCategory=0;
}
void xSEE_FixedAssetsByCategory(int line,char *file)
{
	int x=0;
	FixedAssetsByCategory *e=NULL;

	fprintf(RDA_STDERR,"Showing FixedAssetsByCategory num [%d] at line [%d] program [%s]\n.",numFixedAssetsByCategory,line,file);
	if(myFixedAssetsByCategory!=NULL)
	{
		for(x=0,e=myFixedAssetsByCategory;x<numFixedAssetsByCategory;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s] Quantity [%f] Amount [%.02f]\n",x,(e->status!=NULL ? e->status:""),e->qty,e->amt/100);
		}
	}
}
void addFixedAssetsByCategory(char *s,double qty,double revx)
{
	FixedAssetsByCategory *e=NULL;
	int x=0;
	
	if(myFixedAssetsByCategory!=NULL)
	{
		for(x=0,e=myFixedAssetsByCategory;x<numFixedAssetsByCategory;++x,++e)
		{
			if(!RDAstrcmp(s,e->status)) break;
		}
		if(x>=numFixedAssetsByCategory)
		{
			myFixedAssetsByCategory=Rrealloc(myFixedAssetsByCategory,((numFixedAssetsByCategory+1)*sizeof(FixedAssetsByCategory)));
			e=myFixedAssetsByCategory+numFixedAssetsByCategory;
			++numFixedAssetsByCategory;
			e->status=stralloc(s);
			e->qty=0.0;
			e->amt=0.0;
		}
	} else {
		myFixedAssetsByCategory=Rmalloc(sizeof(FixedAssetsByCategory));
		e=myFixedAssetsByCategory;
		numFixedAssetsByCategory=1;
		e->status=stralloc(s);
		e->qty=0.0;
		e->amt=0.0;
	}
	e->qty+=qty;
	e->amt+=revx;
}
short ReadFixedAssetsByCategory()
{
	int count=0,checknum=0,eftnum=0;
	short ef=FALSE,fileno=(-1),fixcat=(-1);
	char delflag=FALSE,*cg=NULL,*srcmod=NULL,DID_STUFF=FALSE;
	char *temp=NULL;
	double amt=0.0,qty=0.0;

	fileno=OPNNRD("FIXASS","FIXMST");
	fixcat=OPNNRD("FIXASS","FIXCAT");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(fileno,"CATEGORY IDENTIFICATION",&srcmod);
				if(fixcat!=(-1))
				{
					ZERNRD(fixcat);
					FINDFLDSETSTRING(fixcat,"CATEGORY IDENTIFICATION",srcmod);
					if(EQLNRD(fixcat,1)) KEYNRD(fixcat,1);
					FINDFLDGETSTRING(fixcat,"DESCRIPTION",&temp);
					if(isEMPTY(temp))
					{
						if(temp!=NULL) Rfree(temp);
						temp=stralloc(srcmod);
					}
				} else temp=stralloc(srcmod);
				++count;
				FINDFLDGETDOUBLE(fileno,"TOTAL QUANTITY",&qty);
				FINDFLDGETDOUBLE(fileno,"TOTAL CURRENT VALUE",&amt);
				addFixedAssetsByCategory(temp,qty,amt);
				if(temp!=NULL) Rfree(temp);
				DID_STUFF=TRUE;
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
		if(fixcat!=(-1)) CLSNRD(fixcat);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		if(fixcat!=(-1)) CLSNRD(fixcat);
		return(-1);
	}
}
void FixedAssetsByCategoryChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=FixedAssetsByCategoryDashBoard->parent();
	FixedAssetsByCategoryValuetoVisual=CB->currentIndex();
	FixedAssetsByCategoryShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=FixedAssetsByCategoryView();
	parent->addWidget(w);
}
Wt::WContainerWidget *FixedAssetsByCategoryView()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	FixedAssetsByCategory *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	double amt=0.0,count=0.0;;
	int x=0;
	long y=0;
	char *temp=NULL,VPstemp[1024];

	myFixedAssetsByCategory=NULL;
	numFixedAssetsByCategory=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	FixedAssetsByCategoryDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(FixedAssetsByCategoryDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Fixed Assets Summaries"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadFixedAssetsByCategory();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Fixed Asset Masters."),FixedAssetsByCategoryDashBoard);
		return(FixedAssetsByCategoryDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Fixed Asset Summaries."),FixedAssetsByCategoryDashBoard);
		return(FixedAssetsByCategoryDashBoard);
	}
	if(FixedAssetsByCategorysUnProcessed!=NULL) FixedAssetsByCategorysUnProcessed->~WStandardItemModel();
	FixedAssetsByCategorysUnProcessed=new Wt::WStandardItemModel();
	FixedAssetsByCategorysUnProcessed->insertColumns(FixedAssetsByCategorysUnProcessed->columnCount(),3);
// headers 
	FixedAssetsByCategorysUnProcessed->setHeaderData(0,Wt::WString("Category"));
	FixedAssetsByCategorysUnProcessed->setHeaderData(1,Wt::WString("Quantity"));
	FixedAssetsByCategorysUnProcessed->setHeaderData(2,Wt::WString("Amount"));
	FixedAssetsByCategorysUnProcessed->insertRows(FixedAssetsByCategorysUnProcessed->rowCount(),numFixedAssetsByCategory);
	count=0;
	amt=0.0;
	for(x=0,e=myFixedAssetsByCategory;x<numFixedAssetsByCategory;++x,++e)
	{
		FixedAssetsByCategorysUnProcessed->setData(x,0,Wt::WString(e->status));
		FixedAssetsByCategorysUnProcessed->setData(x,1,e->qty);
		count+=e->qty;
		FixedAssetsByCategorysUnProcessed->setData(x,2,e->amt/100);
		amt+=e->amt;
	}
	FixedAssetsByCategorysUnProcessed->sort((FixedAssetsByCategoryValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	temp=famt(amt,20);
	sprintf(VPstemp,"Quantity: %.04f Total: %s",count,temp);
	l=new Wt::WContainerWidget(FixedAssetsByCategoryDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(FixedAssetsByCategoryDashBoard);
	xchart->setModel(FixedAssetsByCategorysUnProcessed);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((FixedAssetsByCategoryValuetoVisual+1));
	xchart->setAvoidLabelRendering(4.99);
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(FixedAssetsByCategoryDashBoard);
	TablePanel->setTitle(Wt::WString("Fixed Asset Summary Table"));
	TablePanel->collapsed().connect(std::bind([=]() { FixedAssetsByCategoryShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { FixedAssetsByCategoryShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(FixedAssetsByCategoryShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(FixedAssetsByCategorysUnProcessed);
	table->setColumnWidth(0,350);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%f");
	table->setItemDelegateForColumn(1,myID);
	table->setColumnAlignment(2,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%.02f");
	table->setItemDelegateForColumn(2,myID);
	table->setHeaderAlignment(0,AlignCenter);
	table->setHeaderAlignment(1,AlignRight);
	table->setHeaderAlignment(2,AlignRight);
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
	CB->addItem(Wt::WString("Quantity"));
	CB->addItem(Wt::WString("Amount"));
	CB->setCurrentIndex(FixedAssetsByCategoryValuetoVisual);
	CB->activated().connect(boost::bind(&FixedAssetsByCategoryChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	FreeFixedAssetsByCategory();
	return(FixedAssetsByCategoryDashBoard);
}
