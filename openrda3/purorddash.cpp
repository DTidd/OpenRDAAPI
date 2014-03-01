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

Wt::WContainerWidget *OpenPObyVendorDisplayDashBoard(void);
int OpenPObyVendorValuetoVisual=0;
short OpenPObyVendorShowTable=FALSE;
Wt::WContainerWidget *OpenPObyVendorDashBoard=NULL;
Wt::WStandardItemModel *OpenPObyVendorsModel=NULL;
char APPROVE_PO=FALSE;
#define SEE_OpenPObyVendor()	xSEE_OpenPObyVendor(__LINE__,__FILE__)
void xSEE_OpenPObyVendor(int,char *);

struct OpenPObyVendors {
	char *vendor;
	int count;
	double amt;
};
typedef struct OpenPObyVendors OpenPObyVendor;

OpenPObyVendor *myOpenPObyVendor=NULL;
int numOpenPObyVendor=0;

void FreeOpenPObyVendor()
{
	OpenPObyVendor *e=NULL;
	int x=0;

	if(myOpenPObyVendor!=NULL)
	{
		for(x=0,e=myOpenPObyVendor;x<numOpenPObyVendor;++x,++e)
		{
			if(e->vendor!=NULL) Rfree(e->vendor);
		}
		Rfree(myOpenPObyVendor);
	}
	myOpenPObyVendor=NULL;
	numOpenPObyVendor=0;
}
void xSEE_OpenPObyVendor(int line,char *file)
{
	int x=0;
	OpenPObyVendor *e=NULL;

	fprintf(RDA_STDERR,"Showing OpenPObyVendor num [%d] at line [%d] program [%s]\n.",numOpenPObyVendor,line,file);
	if(myOpenPObyVendor!=NULL)
	{
		for(x=0,e=myOpenPObyVendor;x<numOpenPObyVendor;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Vendor Identification [%s] Outstanding Amount [%.02f]\n",x,(e->vendor!=NULL ? e->vendor:""),e->amt/100);
		}
	}
}
void addOpenPObyVendor(char *s,double revx)
{
	OpenPObyVendor *e=NULL;
	int x=0;
	
	if(myOpenPObyVendor!=NULL)
	{
		for(x=0,e=myOpenPObyVendor;x<numOpenPObyVendor;++x,++e)
		{
			if(!RDAstrcmp(s,e->vendor)) break;
		}
		if(x>=numOpenPObyVendor)
		{
			myOpenPObyVendor=Rrealloc(myOpenPObyVendor,((numOpenPObyVendor+1)*sizeof(OpenPObyVendor)));
			e=myOpenPObyVendor+numOpenPObyVendor;
			++numOpenPObyVendor;
			e->vendor=stralloc(s);
			e->count=0;
			e->amt=0.0;
		}
	} else {
		myOpenPObyVendor=Rmalloc(sizeof(OpenPObyVendor));
		e=myOpenPObyVendor;
		numOpenPObyVendor=1;
		e->vendor=stralloc(s);
		e->count=0;
		e->amt=0.0;
	}
	++e->count;
	e->amt+=revx;
}
short ReadOpenPObyVendor()
{
	int fileno=(-1),count=0;
	short ef=FALSE,approve=0;
	char delflag=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char closed=FALSE;
	double amt=0.0;

	fileno=OPNNRD("PURORD","PURMST");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETSHORT(fileno,"APPROVE",&approve);
			FINDFLDGETCHAR(fileno,"CLOSED",&closed);
			FINDFLDGETDOUBLE(fileno,"OUTSTANDING AMOUNT",&amt);
			if(!delflag && !closed && (APPROVE_PO==FALSE || approve==1) && amt>0.0)
			{
				FINDFLDGETSTRING(fileno,"VENDOR IDENTIFICATION",&srcmod);
				++count;
				addOpenPObyVendor(srcmod,amt);
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
void OpenPObyVendorChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=OpenPObyVendorDashBoard->parent();
	OpenPObyVendorValuetoVisual=CB->currentIndex();
	OpenPObyVendorShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=OpenPObyVendorDisplayDashBoard();
	parent->addWidget(w);
}
Wt::WContainerWidget *OpenPObyVendorDisplayDashBoard()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WLineEdit *hold=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	Wt::WItemDelegate *myID=NULL;
	OpenPObyVendor *e=NULL;
	Wt::WLength spc;
	double outamt=0.0;
	int x=0,count=0;
	long y=0;
	char *libx=NULL,*vendorname=NULL,POstemp[1024],*temp=NULL;
	RDAGenericSetup *gsv=NULL;
	short vennum=(-1);

	myOpenPObyVendor=NULL;
	numOpenPObyVendor=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libx,"%s/rda/PURORD.GSV",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PURORD.GSV",CURRENTDIRECTORY);
#endif

	gsv=RDAGenericSetupNew("PURORD","APPROVE PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			APPROVE_PO=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	OpenPObyVendorDashBoard=new Wt::WContainerWidget();	
	OpenPObyVendorDashBoard->setPadding(spc,All);
	l=new Wt::WContainerWidget(OpenPObyVendorDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Open Purchase Orders by Vendor"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadOpenPObyVendor();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Purchase Orders."),OpenPObyVendorDashBoard);
		return(OpenPObyVendorDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Open Purchase Orders."),OpenPObyVendorDashBoard);
		return(OpenPObyVendorDashBoard);
	}
	if(OpenPObyVendorsModel!=NULL) OpenPObyVendorsModel->~WStandardItemModel();
	OpenPObyVendorsModel=new Wt::WStandardItemModel();
	OpenPObyVendorsModel->insertColumns(OpenPObyVendorsModel->columnCount(),3);
// headers 
	OpenPObyVendorsModel->setHeaderData(0,Wt::WString("Vendor"));
	OpenPObyVendorsModel->setHeaderData(1,Wt::WString("Number of PO(s)"));
	OpenPObyVendorsModel->setHeaderData(2,Wt::WString("Outstanding Amount"));
	OpenPObyVendorsModel->insertRows(OpenPObyVendorsModel->rowCount(),numOpenPObyVendor);
	vennum=OPNNRD("VENPMT","VENMSR");
	count=0;
	outamt=0.0;
	for(x=0,e=myOpenPObyVendor;x<numOpenPObyVendor;++x,++e)
	{
		if(vendorname!=NULL) Rfree(vendorname);
		if(vennum!=(-1))
		{
			ZERNRD(vennum);
			FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",e->vendor);
			if(EQLNRD(vennum,1)) KEYNRD(vennum,1);
			FINDFLDGETSTRING(vennum,"VENDOR NAME",&vendorname);
		}	
		if(!isEMPTY(vendorname))
		{
			OpenPObyVendorsModel->setData(x,0,Wt::WString(vendorname));
		} else {
			OpenPObyVendorsModel->setData(x,0,Wt::WString(e->vendor));
		}
		OpenPObyVendorsModel->setData(x,1,e->count);
		OpenPObyVendorsModel->setData(x,2,e->amt/100);
		count+=e->count;
		outamt+=e->amt;
	}
	OpenPObyVendorsModel->sort((OpenPObyVendorValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	if(vennum!=(-1)) CLSNRD(vennum);
	l=new Wt::WContainerWidget(OpenPObyVendorDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	memset(POstemp,0,1024);
	temp=famt(outamt,20);
	sprintf(POstemp,"Open PO(s): %8d %s",count,temp);
	if(temp!=NULL) Rfree(temp);
	text=new Wt::WText(Wt::WString(POstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	
	table=new Wt::WTableView(OpenPObyVendorDashBoard);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(OpenPObyVendorsModel);
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
	TablePanel=new Wt::WPanel(OpenPObyVendorDashBoard);
	TablePanel->setTitle(Wt::WString("Open Purchase Orders by Vendor"));
	TablePanel->collapsed().connect(std::bind([=]() { OpenPObyVendorShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { OpenPObyVendorShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(OpenPObyVendorShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	w->setPadding(spc,All);
	TablePanel->setCentralWidget(w);
	l=new Wt::WContainerWidget(w);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of PO(s)"));
	CB->addItem(Wt::WString("Outstanding Amount"));
	CB->setCurrentIndex(OpenPObyVendorValuetoVisual);
	CB->activated().connect(boost::bind(&OpenPObyVendorChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	xchart=new Wt::Chart::WPieChart(w);
	xchart->setModel(OpenPObyVendorsModel);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((OpenPObyVendorValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	FreeOpenPObyVendor();
	return(OpenPObyVendorDashBoard);
}
