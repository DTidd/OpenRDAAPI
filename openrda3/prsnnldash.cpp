/* Personnel Demographics Dashboards */
#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

/* By Personnel Class */

Wt::WContainerWidget *CurrentEmployeeClass(void);
int EmployeeClassValuetoVisual=0;
short EmployeeClassShowTable=FALSE;
Wt::WContainerWidget *CurrentEmployeeClassDashBoard=NULL;
Wt::WStandardItemModel *EmployeeClasssCurrent=NULL;
#define SEE_EmployeeClass()	xSEE_EmployeeClass(__LINE__,__FILE__)
void xSEE_EmployeeClass(int,char *);

struct EmployeeClasss {
	char *classid;
	int count;
};
typedef struct EmployeeClasss EmployeeClass;

EmployeeClass *myEmployeeClass=NULL;
int numEmployeeClass=0;

void FreeEmployeeClass()
{
	EmployeeClass *e=NULL;
	int x=0;

	if(myEmployeeClass!=NULL)
	{
		for(x=0,e=myEmployeeClass;x<numEmployeeClass;++x,++e)
		{
			if(e->classid!=NULL) Rfree(e->classid);
		}
		Rfree(myEmployeeClass);
	}
	myEmployeeClass=NULL;
	numEmployeeClass=0;
}
void xSEE_EmployeeClass(int line,char *file)
{
	int x=0;
	EmployeeClass *e=NULL;

	fprintf(RDA_STDERR,"Showing EmployeeClass num [%d] at line [%d] program [%s]\n.",numEmployeeClass,line,file);
	if(myEmployeeClass!=NULL)
	{
		for(x=0,e=myEmployeeClass;x<numEmployeeClass;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s]\n",x,(e->classid!=NULL ? e->classid:""));
		}
	}
}
void addEmployeeClass(char *s)
{
	EmployeeClass *e=NULL;
	int x=0;
	
	if(myEmployeeClass!=NULL)
	{
		for(x=0,e=myEmployeeClass;x<numEmployeeClass;++x,++e)
		{
			if(!RDAstrcmp(s,e->classid)) break;
		}
		if(x>=numEmployeeClass)
		{
			myEmployeeClass=Rrealloc(myEmployeeClass,((numEmployeeClass+1)*sizeof(EmployeeClass)));
			e=myEmployeeClass+numEmployeeClass;
			++numEmployeeClass;
			e->classid=stralloc(s);
			e->count=0;
		}
	} else {
		myEmployeeClass=Rmalloc(sizeof(EmployeeClass));
		e=myEmployeeClass;
		numEmployeeClass=1;
		e->classid=stralloc(s);
		e->count=0;
	}
	++e->count;
}
short ReadEmployeeClass()
{
	int count=0;
	short ef=FALSE,fileno=(-1),poscls=(-1);
	char delflag=FALSE,curemp=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char *temp=NULL;

	poscls=OPNNRD("POSTRK","POSCLS");
	fileno=OPNNRD("PRSNNL","PERDMG");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"CURRENT EMPLOYEE",&curemp);
			if(!delflag && curemp)
			{
				++count;
				FINDFLDGETSTRING(fileno,"PERSONNEL CLASS",&srcmod);
				if(poscls!=(-1))
				{
					ZERNRD(poscls);
					FINDFLDSETSTRING(poscls,"CLASS IDENTIFICATION",srcmod);
					if(EQLNRD(poscls,1)) KEYNRD(poscls,1);
					FINDFLDGETSTRING(poscls,"DESCRIPTION",&temp);
					if(isEMPTY(temp)) 
					{
						if(temp!=NULL) Rfree(temp);
						temp=stralloc(srcmod);
					}
				} else temp=stralloc(srcmod);
				addEmployeeClass(temp);
				if(temp!=NULL) Rfree(temp);
				DID_STUFF=TRUE;
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
		if(poscls!=(-1)) CLSNRD(poscls);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		if(poscls!=(-1)) CLSNRD(poscls);
		return(-1);
	}
}
void EmployeeClassChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=CurrentEmployeeClassDashBoard->parent();
	EmployeeClassValuetoVisual=CB->currentIndex();
	EmployeeClassShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=CurrentEmployeeClass();
	parent->addWidget(w);
}
Wt::WContainerWidget *CurrentEmployeeClass()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	EmployeeClass *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	int x=0,count=0;
	long y=0;
	char *libx=NULL,*temp=NULL,VPstemp[1024];
	RDAGenericSetup *gsv=NULL;

	myEmployeeClass=NULL;
	numEmployeeClass=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	CurrentEmployeeClassDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(CurrentEmployeeClassDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Current Employees by Classification"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadEmployeeClass();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Personnel Demographics."),CurrentEmployeeClassDashBoard);
		return(CurrentEmployeeClassDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Current Employees."),CurrentEmployeeClassDashBoard);
		return(CurrentEmployeeClassDashBoard);
	}
	if(EmployeeClasssCurrent!=NULL) EmployeeClasssCurrent->~WStandardItemModel();
	EmployeeClasssCurrent=new Wt::WStandardItemModel();
	EmployeeClasssCurrent->insertColumns(EmployeeClasssCurrent->columnCount(),2);
// headers 
	EmployeeClasssCurrent->setHeaderData(0,Wt::WString("Classification"));
	EmployeeClasssCurrent->setHeaderData(1,Wt::WString("Employees"));
	EmployeeClasssCurrent->insertRows(EmployeeClasssCurrent->rowCount(),numEmployeeClass);
	count=0;
	for(x=0,e=myEmployeeClass;x<numEmployeeClass;++x,++e)
	{
		EmployeeClasssCurrent->setData(x,0,Wt::WString(e->classid));
		EmployeeClasssCurrent->setData(x,1,e->count);
		count+=e->count;
	}
	EmployeeClasssCurrent->sort((EmployeeClassValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	sprintf(VPstemp,"Current Employees: %8d",count);
	l=new Wt::WContainerWidget(CurrentEmployeeClassDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(CurrentEmployeeClassDashBoard);
	xchart->setModel(EmployeeClasssCurrent);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((EmployeeClassValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(CurrentEmployeeClassDashBoard);
	TablePanel->setTitle(Wt::WString("Current Employees by Class"));
	TablePanel->collapsed().connect(std::bind([=]() { EmployeeClassShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { EmployeeClassShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(EmployeeClassShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(EmployeeClasssCurrent);
	table->setColumnWidth(0,350);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%d");
	table->setItemDelegateForColumn(1,myID);
	table->setColumnAlignment(1,AlignRight);
	table->setHeaderAlignment(0,AlignCenter);
	table->setHeaderAlignment(1,AlignRight);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	FreeEmployeeClass();
	return(CurrentEmployeeClassDashBoard);
}

/* By Location */

Wt::WContainerWidget *CurrentEmployeeLocation(void);
int EmployeeLocationValuetoVisual=0;
short EmployeeLocationShowTable=FALSE;
Wt::WContainerWidget *CurrentEmployeeLocationDashBoard=NULL;
Wt::WStandardItemModel *EmployeeLocationsCurrent=NULL;
#define SEE_EmployeeLocation()	xSEE_EmployeeLocation(__LINE__,__FILE__)
void xSEE_EmployeeLocation(int,char *);

struct EmployeeLocations {
	char *location_id;
	int count;
};
typedef struct EmployeeLocations EmployeeLocation;

EmployeeLocation *myEmployeeLocation=NULL;
int numEmployeeLocation=0;

void FreeEmployeeLocation()
{
	EmployeeLocation *e=NULL;
	int x=0;

	if(myEmployeeLocation!=NULL)
	{
		for(x=0,e=myEmployeeLocation;x<numEmployeeLocation;++x,++e)
		{
			if(e->location_id!=NULL) Rfree(e->location_id);
		}
		Rfree(myEmployeeLocation);
	}
	myEmployeeLocation=NULL;
	numEmployeeLocation=0;
}
void xSEE_EmployeeLocation(int line,char *file)
{
	int x=0;
	EmployeeLocation *e=NULL;

	fprintf(RDA_STDERR,"Showing EmployeeLocation num [%d] at line [%d] program [%s]\n.",numEmployeeLocation,line,file);
	if(myEmployeeLocation!=NULL)
	{
		for(x=0,e=myEmployeeLocation;x<numEmployeeLocation;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Status [%s]\n",x,(e->location_id!=NULL ? e->location_id:""));
		}
	}
}
void addEmployeeLocation(char *s)
{
	EmployeeLocation *e=NULL;
	int x=0;
	
	if(myEmployeeLocation!=NULL)
	{
		for(x=0,e=myEmployeeLocation;x<numEmployeeLocation;++x,++e)
		{
			if(!RDAstrcmp(s,e->location_id)) break;
		}
		if(x>=numEmployeeLocation)
		{
			myEmployeeLocation=Rrealloc(myEmployeeLocation,((numEmployeeLocation+1)*sizeof(EmployeeLocation)));
			e=myEmployeeLocation+numEmployeeLocation;
			++numEmployeeLocation;
			e->location_id=stralloc(s);
			e->count=0;
		}
	} else {
		myEmployeeLocation=Rmalloc(sizeof(EmployeeLocation));
		e=myEmployeeLocation;
		numEmployeeLocation=1;
		e->location_id=stralloc(s);
		e->count=0;
	}
	++e->count;
}
short ReadEmployeeLocation()
{
	int count=0;
	short ef=FALSE,fileno=(-1),poslmst=(-1);
	char delflag=FALSE,curemp=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char *temp=NULL;

	poslmst=OPNNRD("POSTRK","POSLMST");
	fileno=OPNNRD("PRSNNL","PERDMG");
	if(fileno!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"CURRENT EMPLOYEE",&curemp);
			if(!delflag && curemp)
			{
				++count;
				FINDFLDGETSTRING(fileno,"LOCATION IDENTIFICATION",&srcmod);
				if(poslmst!=(-1))
				{
					ZERNRD(poslmst);
					FINDFLDSETSTRING(poslmst,"LOCATION IDENTIFICATION",srcmod);
					if(EQLNRD(poslmst,1)) KEYNRD(poslmst,1);
					FINDFLDGETSTRING(poslmst,"DESCRIPTION",&temp);
					if(isEMPTY(temp)) 
					{
						if(temp!=NULL) Rfree(temp);
						temp=stralloc(srcmod);
					}
				} else temp=stralloc(srcmod);
				addEmployeeLocation(temp);
				if(temp!=NULL) Rfree(temp);
				DID_STUFF=TRUE;
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
		if(poslmst!=(-1)) CLSNRD(poslmst);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		if(poslmst!=(-1)) CLSNRD(poslmst);
		return(-1);
	}
}
void EmployeeLocationChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=CurrentEmployeeLocationDashBoard->parent();
	EmployeeLocationValuetoVisual=CB->currentIndex();
	EmployeeLocationShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=CurrentEmployeeLocation();
	parent->addWidget(w);
}
Wt::WContainerWidget *CurrentEmployeeLocation()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	EmployeeLocation *e=NULL;
	Wt::WLength spc;
	Wt::WItemDelegate *myID=NULL;
	int x=0,count=0;
	long y=0;
	char *libx=NULL,*temp=NULL,VPstemp[1024];
	RDAGenericSetup *gsv=NULL;

	myEmployeeLocation=NULL;
	numEmployeeLocation=0;
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	CurrentEmployeeLocationDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(CurrentEmployeeLocationDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Current Employees by Location"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadEmployeeLocation();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Personnel Demographics."),CurrentEmployeeLocationDashBoard);
		return(CurrentEmployeeLocationDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Current Employees."),CurrentEmployeeLocationDashBoard);
		return(CurrentEmployeeLocationDashBoard);
	}
	if(EmployeeLocationsCurrent!=NULL) EmployeeLocationsCurrent->~WStandardItemModel();
	EmployeeLocationsCurrent=new Wt::WStandardItemModel();
	EmployeeLocationsCurrent->insertColumns(EmployeeLocationsCurrent->columnCount(),2);
// headers 
	EmployeeLocationsCurrent->setHeaderData(0,Wt::WString("Locationification"));
	EmployeeLocationsCurrent->setHeaderData(1,Wt::WString("Employees"));
	EmployeeLocationsCurrent->insertRows(EmployeeLocationsCurrent->rowCount(),numEmployeeLocation);
	count=0;
	for(x=0,e=myEmployeeLocation;x<numEmployeeLocation;++x,++e)
	{
		EmployeeLocationsCurrent->setData(x,0,Wt::WString(e->location_id));
		EmployeeLocationsCurrent->setData(x,1,e->count);
		count+=e->count;
	}
	EmployeeLocationsCurrent->sort((EmployeeLocationValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	memset(VPstemp,0,1024);
	sprintf(VPstemp,"Current Employees: %8d",count);
	l=new Wt::WContainerWidget(CurrentEmployeeLocationDashBoard);
	l->setPadding(spc,All);
	lbox=new Wt::WHBoxLayout();
	lbox->setContentsMargins(0,0,0,0);
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString(VPstemp));
	text->addStyleClass("OpenRDA");
	lbox->addWidget(text,0,Wt::AlignCenter);
	if(temp!=NULL) Rfree(temp);
	xchart=new Wt::Chart::WPieChart(CurrentEmployeeLocationDashBoard);
	xchart->setModel(EmployeeLocationsCurrent);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((EmployeeLocationValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	TablePanel=new Wt::WPanel(CurrentEmployeeLocationDashBoard);
	TablePanel->setTitle(Wt::WString("Current Employees by Location"));
	TablePanel->collapsed().connect(std::bind([=]() { EmployeeLocationShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { EmployeeLocationShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(EmployeeLocationShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	table=new Wt::WTableView(w);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(EmployeeLocationsCurrent);
	table->setColumnWidth(0,350);
	table->setRowHeight(22);
	table->setColumnAlignment(0,AlignCenter);
	table->setColumnAlignment(1,AlignRight);
	myID=new Wt::WItemDelegate();
	myID->setTextFormat("%d");
	table->setItemDelegateForColumn(1,myID);
	table->setColumnAlignment(1,AlignRight);
	table->setHeaderAlignment(0,AlignCenter);
	table->setHeaderAlignment(1,AlignRight);
	if(Wt::WApplication::instance()->environment().ajax()) 
	{
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}
	FreeEmployeeLocation();
	return(CurrentEmployeeLocationDashBoard);
}
