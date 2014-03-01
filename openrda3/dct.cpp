
Wt::WContainerWidget *OpenPObyCategoryDisplayDashBoard(void);
int OpenPObyCategoryValuetoVisual=0;
short OpenPObyCategoryShowTable=FALSE;
Wt::WContainerWidget *OpenPObyCategoryDashBoard=NULL;
Wt::WStandardItemModel *OpenPObyCategorysModel=NULL;
char APPROVE_PO=FALSE;
#define SEE_OpenPObyCategory()	xSEE_OpenPObyCategory(__LINE__,__FILE__)
void xSEE_OpenPObyCategory(int,char *);

struct OpenPObyCategorys {
	char *category_id;
	int count;
	double amt;
};
typedef struct OpenPObyCategorys OpenPObyCategory;

OpenPObyCategory *myOpenPObyCategory=NULL;
int numOpenPObyCategory=0;

void FreeOpenPObyCategory()
{
	OpenPObyCategory *e=NULL;
	int x=0;

	if(myOpenPObyCategory!=NULL)
	{
		for(x=0,e=myOpenPObyCategory;x<numOpenPObyCategory;++x,++e)
		{
			if(e->category_id!=NULL) Rfree(e->category_id);
		}
		Rfree(myOpenPObyCategory);
	}
	myOpenPObyCategory=NULL;
	numOpenPObyCategory=0;
}
void xSEE_OpenPObyCategory(int line,char *file)
{
	int x=0;
	OpenPObyCategory *e=NULL;

	fprintf(RDA_STDERR,"Showing OpenPObyCategory num [%d] at line [%d] program [%s]\n.",numOpenPObyCategory,line,file);
	if(myOpenPObyCategory!=NULL)
	{
		for(x=0,e=myOpenPObyCategory;x<numOpenPObyCategory;++x,++e)
		{
			fprintf(RDA_STDERR,"x [%d] Vendor Identification [%s] Outstanding Amount [%.02f]\n",x,(e->category_id!=NULL ? e->category_id:""),e->amt/100);
		}
	}
}
void addOpenPObyCategory(char *s,double revx)
{
	OpenPObyCategory *e=NULL;
	int x=0;
	
	if(myOpenPObyCategory!=NULL)
	{
		for(x=0,e=myOpenPObyCategory;x<numOpenPObyCategory;++x,++e)
		{
			if(!RDAstrcmp(s,e->category_id)) break;
		}
		if(x>=numOpenPObyCategory)
		{
			myOpenPObyCategory=Rrealloc(myOpenPObyCategory,((numOpenPObyCategory+1)*sizeof(OpenPObyCategory)));
			e=myOpenPObyCategory+numOpenPObyCategory;
			++numOpenPObyCategory;
			e->category_id=stralloc(s);
			e->count=0;
			e->amt=0.0;
		}
	} else {
		myOpenPObyCategory=Rmalloc(sizeof(OpenPObyCategory));
		e=myOpenPObyCategory;
		numOpenPObyCategory=1;
		e->category_id=stralloc(s);
		e->count=0;
		e->amt=0.0;
	}
	++e->count;
	e->amt+=revx;
}
short ReadOpenPObyCategory()
{
	int fileno=(-1),count=0,purmst=(-1);
	short ef=FALSE,approve=0;
	char delflag=FALSE,*srcmod=NULL,DID_STUFF=FALSE;
	char closed=FALSE;
	double amt=0.0;

	fileno=OPNNRD("PURORD","PURLIN");
	purmst=OPNNRD("PURORD","PURMST");
	if(fileno!=(-1) && purmst!=(-1))
	{
		count=0;
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				ZERNRD(purmst);
				COPYFIELD(fileno,purmst,"FISCAL YEAR");
				COPYFIELD(fileno,purmst,"PO IDENTIFICATION");
				if(!EQLNRD(purmst,1))
				{
					FINDFLDGETSHORT(purmst,"APPROVE",&approve);
					FINDFLDGETCHAR(purmst,"DELETEFLAG",&delflag);
					FINDFLDGETCHAR(purmst,"CLOSED",&closed);
					if(!delflag && !closed && (APPROVE_PO==FALSE || approve==1))
					{
						FINDFLDGETSTRING(fileno,"CATEGORY IDENTIFICATION",&srcmod);
						++count;
						FINDFLDGETDOUBLE(fileno,"OUTSTANDING AMOUNT",&amt);
						addOpenPObyCategory(srcmod,amt);
						DID_STUFF=TRUE;
					}
				}
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
		CLSNRD(purmst);
		return((DID_STUFF==TRUE ? 0:(-2)));
	} else {
		if(fileno!=(-1)) CLSNRD(fileno);
		if(purmst!=(-1)) CLSNRD(purmst);
		return(-1);
	}
}
void OpenPObyCategoryChangeVisualValue(Wt::WComboBox *CB)
{
	Wt::WContainerWidget *w=NULL;
	Wt::WContainerWidget *parent=NULL;

	parent=OpenPObyCategoryDashBoard->parent();
	OpenPObyCategoryValuetoVisual=CB->currentIndex();
	OpenPObyCategoryShowTable=TRUE;
	if(parent->count()>0) parent->clear();
	w=OpenPObyCategoryDisplayDashBoard();
	parent->addWidget(w);
}
Wt::WContainerWidget *OpenPObyCategoryDisplayDashBoard()
{
	Wt::WContainerWidget *w=NULL,*l=NULL;
	Wt::WHBoxLayout *lbox=NULL;
	Wt::WTableView *table=NULL;
	Wt::Chart::WPieChart *xchart=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WText *text=NULL;
	Wt::WItemDelegate *myID=NULL;
	OpenPObyCategory *e=NULL;
	int x=0;
	long y=0;
	char *libx=NULL,*category_idname=NULL;
	RDAGenericSetup *gsv=NULL;
	short catmst=(-1);

	myOpenPObyCategory=NULL;
	numOpenPObyCategory=0;
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
	OpenPObyCategoryDashBoard=new Wt::WContainerWidget();	
	l=new Wt::WContainerWidget(OpenPObyCategoryDashBoard);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);
	text=new Wt::WText(Wt::WString("Open Purchase Orders by Vendor"));
	text->addStyleClass("OpenRDA h3");
	lbox->addWidget(text,0,Wt::AlignCenter);
	x=ReadOpenPObyCategory();
	if(x==(-1))
	{
		text=new Wt::WText(Wt::WString("Error:  Unable to read Purchase Orders."),OpenPObyCategoryDashBoard);
		return(OpenPObyCategoryDashBoard);
	} else if(x==(-2))
	{
		text=new Wt::WText(Wt::WString("No Open Purchase Orders."),OpenPObyCategoryDashBoard);
		return(OpenPObyCategoryDashBoard);
	}
	if(OpenPObyCategorysModel!=NULL) OpenPObyCategorysModel->~WStandardItemModel();
	OpenPObyCategorysModel=new Wt::WStandardItemModel();
	OpenPObyCategorysModel->insertColumns(OpenPObyCategorysModel->columnCount(),3);
// headers 
	OpenPObyCategorysModel->setHeaderData(0,Wt::WString("Vendor"));
	OpenPObyCategorysModel->setHeaderData(1,Wt::WString("Number of PO(s)"));
	OpenPObyCategorysModel->setHeaderData(2,Wt::WString("Outstanding Amount"));
	OpenPObyCategorysModel->insertRows(OpenPObyCategorysModel->rowCount(),numOpenPObyCategory);
	catmst=OPNNRD("CATALOGUE","CATMST");
	for(x=0,e=myOpenPObyCategory;x<numOpenPObyCategory;++x,++e)
	{
		if(name!=NULL) Rfree(name);
		if(catmst!=(-1))
		{
			ZERNRD(catmst);
			FINDFLDSETSTRING(catmst,"CATEGORY IDENTIFICATION",e->category_id);
			if(EQLNRD(catmst,1)) KEYNRD(catmst,1);
			FINDFLDGETSTRING(catmst,"NAME",&name);
		}	
		if(!isEMPTY(name))
		{
			OpenPObyCategorysModel->setData(x,0,Wt::WString(name));
		} else {
			OpenPObyCategorysModel->setData(x,0,Wt::WString(e->category_id));
		}
		OpenPObyCategorysModel->setData(x,1,e->count);
		OpenPObyCategorysModel->setData(x,2,e->amt/100);
	}
	OpenPObyCategorysModel->sort((OpenPObyCategoryValuetoVisual+1),Wt::SortOrder::DescendingOrder);
	if(catmst!=(-1)) CLSNRD(catmst);
	table=new Wt::WTableView(OpenPObyCategoryDashBoard);
	table->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	table->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	table->setSortingEnabled(true);
	table->setColumnResizeEnabled(true);
	table->setModel(OpenPObyCategorysModel);
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
	TablePanel=new Wt::WPanel(OpenPObyCategoryDashBoard);
	TablePanel->setTitle(Wt::WString("Open Purchase Orders by Vendor"));
	TablePanel->collapsed().connect(std::bind([=]() { OpenPObyCategoryShowTable=FALSE; }));
	TablePanel->expanded().connect(std::bind([=]() { OpenPObyCategoryShowTable=TRUE; }));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
	if(OpenPObyCategoryShowTable==FALSE) TablePanel->setCollapsed(TRUE);
		else TablePanel->setCollapsed(FALSE);
	w=new Wt::WContainerWidget();
	TablePanel->setCentralWidget(w);
	l=new Wt::WContainerWidget(w);
	lbox=new Wt::WHBoxLayout();
	l->setLayout(lbox);	
	text=new Wt::WText(Wt::WString("Visualized Column:"));
	lbox->addWidget(text,0,Wt::AlignLeft);
	CB=new Wt::WComboBox();
	CB->addItem(Wt::WString("Number of PO(s)"));
	CB->addItem(Wt::WString("Outstanding Amount"));
	CB->setCurrentIndex(OpenPObyCategoryValuetoVisual);
	CB->activated().connect(boost::bind(&OpenPObyCategoryChangeVisualValue,CB));
	lbox->addWidget(CB,200,Wt::AlignLeft);
	xchart=new Wt::Chart::WPieChart(w);
	xchart->setModel(OpenPObyCategorysModel);
	xchart->setLabelsColumn(0);
	xchart->setDataColumn((OpenPObyCategoryValuetoVisual+1));
	xchart->setDisplayLabels(Wt::Chart::LabelOption::Outside | Wt::Chart::LabelOption::TextLabel | Wt::Chart::LabelOption::TextPercentage);
	xchart->setPerspectiveEnabled(true,0.2);
	xchart->setExplode(0,0.3);
	xchart->resize(960,360);
	xchart->setMargin(2,Wt::Side::Top | Wt::Side::Bottom);
	xchart->setMargin(Wt::WLength::Auto,Wt::Side::Left | Wt::Side::Right);
	FreeOpenPObyCategory();
	return(OpenPObyCategoryDashBoard);
}
