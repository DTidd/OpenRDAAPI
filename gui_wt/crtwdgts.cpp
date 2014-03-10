#include<gui.hpp>
#include<guip.hpp>

#define __NEED_WDIALOG_LAYOUT__
#define FLAT_DOCK_BUTTON

int (*CustomTableFunction)(...);
char INSIDE_PASTE=FALSE;

// define __USE_ANIMATIONS__

short getLeadingSpaces(char *s)
{
	short x=0;
	char *temp=NULL;

	if(s==NULL) return(0);
	if(isEMPTY(s)) return(0);
	for(x=0,temp=s;*temp;++x,++temp) if(*temp!=' ') break;
	return(x);
}

void FrameWidget(Wt::WWidget *w,int frame_style)
{
	Wt::WWebWidget *WW=NULL;
	Wt::WCssDecorationStyle cDS;
	Wt::WBorder wBorder;
	int y=0;

	if(frame_style<0) return;
	WW=(Wt::WWebWidget *)w;
	cDS=WW->decorationStyle();
	for(y=0;y<4;++y)
	{
		switch(y)
		{
			default:
			case 0:
				wBorder=cDS.border(Wt::Top);
				break;
			case 1:
				wBorder=cDS.border(Wt::Bottom);
				break;
			case 2:
				wBorder=cDS.border(Wt::Left);
				break;
			case 3:
				wBorder=cDS.border(Wt::Right);
				break;
		}
		switch(frame_style)
		{
			case 0: 
				wBorder.setStyle(WBorder::Inset);
				wBorder.setWidth(WBorder::Medium);
			case 1:
				wBorder.setStyle(WBorder::Outset);
				wBorder.setWidth(WBorder::Medium);
			case 2:
				wBorder.setStyle(WBorder::Inset);
				wBorder.setWidth(WBorder::Thick);
			case 3:
				wBorder.setStyle(WBorder::Outset);
				wBorder.setWidth(WBorder::Thick);
				break;
		}
		switch(y)
		{
			default:
			case 0:
				cDS.setBorder(wBorder,Wt::Top);
				break;
			case 1:
				cDS.setBorder(wBorder,Wt::Bottom);
				break;
			case 2:
				wBorder=cDS.border(Wt::Left);
				break;
			case 3:
				cDS.setBorder(wBorder,Wt::Right);
				break;
		}
		WW->setDecorationStyle(cDS);
	}
}

void RDArmemKeyPressed(RDArmem *mem,Wt::WKeyEvent &e)
{
	if(e.key()==Key_F1)
	{
		if(mem->helpfunction!=NULL)
		{
			mem->helpfunction(mem->parent,mem);
		}
	}
}
Wt::WWidget *myHorizontalLine(Wt::WContainerWidget *C,short which)
{
	Wt::WContainerWidget *c1=NULL;
	Wt::WHBoxLayout *lBox=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WText *hold=NULL;
	Wt::WWebWidget *WW=NULL;
	Wt::WCssDecorationStyle cDS;
	Wt::WBorder wBorder;

	c1=new Wt::WContainerWidget(C);
/*
	c1->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
	c1->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
*/
	lBox=new Wt::WHBoxLayout();
	daL=(Wt::WLayout *)lBox;
	daL->setContentsMargins(0,0,0,0);
	c1->setLayout(lBox);
	hold =(Wt::WWidget *) new Wt::WText(" ");
	lBox->setStretchFactor((Wt::WWidget *)hold,200);
	WW=(Wt::WWebWidget *)c1;
	cDS=WW->decorationStyle();
	wBorder=cDS.border(Wt::Bottom);
	wBorder.setColor(WColor("Blue"));
	switch(which)
	{
		default:
		case 0:
			wBorder.setStyle(WBorder::Solid);
			wBorder.setWidth(WBorder::Medium);
			break;
		case 1:
			wBorder.setStyle(WBorder::Double);
			wBorder.setWidth(WBorder::Thick);
			break;
		case 2:
			wBorder.setStyle(WBorder::Dashed);
			wBorder.setWidth(WBorder::Medium);
			break;
		case 3:
			wBorder.setStyle(WBorder::Dashed);
			wBorder.setWidth(WBorder::Thick);
			break;
	}
	cDS.setBorder(wBorder,Wt::Bottom);
	return((Wt::WWidget *)c1);
}
Wt::WWidget *myVerticalLine(Wt::WContainerWidget *C,short which)
{
	Wt::WContainerWidget *c1=NULL;
	Wt::WVBoxLayout *lBox=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WText *hold=NULL;
	Wt::WWebWidget *WW=NULL;
	Wt::WCssDecorationStyle cDS;
	Wt::WBorder wBorder;

	c1=new Wt::WContainerWidget(C);
/*
	c1->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
	c1->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
*/
	lBox=new Wt::WVBoxLayout();
	daL=(Wt::WLayout *)lBox;
	daL->setContentsMargins(0,0,0,0);
	c1->setLayout(lBox);
	hold =(Wt::WWidget *) new Wt::WText(" ");
	lBox->setStretchFactor((Wt::WWidget *)hold,200);
	WW=(Wt::WWebWidget *)c1;
	cDS=WW->decorationStyle();
	wBorder=cDS.border(Wt::Left);
	wBorder.setColor(WColor("Blue"));
	switch(which)
	{
		default:
		case 0:
			wBorder.setStyle(WBorder::Solid);
			wBorder.setWidth(WBorder::Medium);
			break;
		case 1:
			wBorder.setStyle(WBorder::Double);
			wBorder.setWidth(WBorder::Thick);
			break;
		case 2:
			wBorder.setStyle(WBorder::Dashed);
			wBorder.setWidth(WBorder::Medium);
			break;
		case 3:
			wBorder.setStyle(WBorder::Dashed);
			wBorder.setWidth(WBorder::Thick);
			break;
	}
	cDS.setBorder(wBorder,Wt::Left);
	return((Wt::WWidget *)c1);
}
/*---------------------------------------------------------------------------
	crtheadercontainer - to create a header in a crttablecontainer
---------------------------------------------------------------------------*/
void xcrtheadercontainer(Wt::WTable *parent,RDAscrn *scn,RDArsrc *rsrc,int *widgetcount,Wt::WTableRow **pop,int rtype,int line,char *file)
{
	RDAwdgt *wdgt=NULL;
	RDArmem *member=NULL;
	char *dashes=NULL;
	int section=0;
	Wt::WTableRow *row=NULL;
	Wt::WTableCell *tc=NULL;
	Wt::WText *t=NULL;
	char *mssc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crtheadercontainer Creating Custom Header for Screen [%s] [%s] Starting with WWidget [%d] at line [%d] program [%s].",rsrc->module,rsrc->screen,*widgetcount,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	parent->setHeaderCount(1,Horizontal);
	*pop=parent->rowAt(0);
	row=*pop;
/* fix this loop right here */
	section=0;
	if(rtype!=0 && rtype!=2)
	{
		tc=row->elementAt(section);
		t=new Wt::WText("",PlainText);
		mssc=ModuleScreenStyleClass(rsrc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s TableHeaderLabel",mssc);
		t->addStyleClass(GUIstemp);
		tc->addWidget(t);
		++section;
	}
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+*widgetcount;
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG Creating WWidget [%d] Type [%s] Name [%s] Label [%s] [%s]  Rows [%d] Cols [%d] Rtype [%d]",
				*widgetcount,wdgttypes[wdgt->type],(wdgt->resource_name!=NULL ? 
				wdgt->resource_name:""),(wdgt->label!=NULL ? wdgt->label:""),(wdgt->XHTML_Label!=NULL ? wdgt->XHTML_Label:""),
				wdgt->rows,wdgt->cols,wdgt->rtype); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(wdgt->resource_num>=0) 
		{
			if(wdgt->resource_num<rsrc->numrscs)
			{
				member=rsrc->rscs+wdgt->resource_num;
			} else member=NULL;
			switch(wdgt->type)
			{
				case 5: /* LABELS */
/* These may need to be push buttons to get the move capability
   need to review drag and drop to decide best method */
					tc=row->elementAt(section);
					if(isEMPTY(wdgt->XHTML_Label)) 
					{
						t=new Wt::WText(wdgt->label,PlainText);
						mssc=ModuleScreenStyleClass(rsrc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s TableHeaderLabel",mssc);
						t->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
						if(diagcss)
						{
							if(cssNames!=NULL)
							{
								fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),t->styleClass().toUTF8().c_str());
							}
						}
#endif /* USE_RDA_DIAGNOSTICS */
						if(mssc!=NULL) Rfree(mssc);
					} else {
						t=new Wt::WText(wdgt->XHTML_Label,XHTMLText);
					}
					tc->addWidget(t);
					++section;
					break;
				case 31: /* End Header */
					return;
				default:
					break;
			}
		}
	}
}
/*---------------------------------------------------------------------------
	ContainsWtHeader - determines if Table Contains Horizontal Header
---------------------------------------------------------------------------*/
int xContainsWtHeader(RDAscrn *scn,int curnum,int line,char *file)
{
	int x;
	RDAwdgt *wdgt;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG ContainsWtHeader on Screen [%s] [%s] Starting with Widget [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 30: /* Header */
				return(TRUE);
			case 10: /* frame */
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginnging balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle button */
			case 14: /* optional screens */
			case 11: /* seperator */
			case 5: /* label */
			case 22: /* progress_bar */
			case 23: /* new tab bar */
				break;
			case 1: /* new line */
				break;
			case 2: /* end line */
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
			case 20: /* new tab container */
			case 24: /* new popup menu    */
			case 26: /* new toolbar    */
			case 28: /* new table   */
				break;
			case 4: /* end box */
			case 13: /* end scrolled window */
			case 21: /* end tab container */
			case 25: /* end popup menu    */
			case 27: /* end toolbar    */
				break;
			case 29: /* end table  */
				return(FALSE);
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%d] [%d] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}		
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG ContainsWtHeader For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,FALSE,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(FALSE);
}
static char *ValueWithinString(char *s,int x,short *q)
{
	char *temp=NULL,*temp2=NULL,*s1=NULL;
	int y=0,count=0,pos=0;
	
	if(isEMPTY(s))
	{
		*q=TRUE;
		return(NULL);
	}
	s1=stralloc(s);
	for(temp=s1,temp2=s1;*temp;++temp)
	{
		++count;
		if(*temp=='\t' || *temp=='\n') 
		{
			++y;
			if(y>x) break;
			pos=count;
		}
	}
	temp2=s1+pos;
	if(temp2==NULL) 
	{
		*q=TRUE;
		return(NULL);
	}
	*temp=0;
	temp=stralloc(temp2);
	if(s1!=NULL) Rfree(s1);
	return(temp);
}
#define __USE_SCROLLAREA_ON_TABLE__
/*---------------------------------------------------------------------------
	crttablecontainer - to create a table container
---------------------------------------------------------------------------*/
int xcrttablecontainer(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsrc,int *widgetcount,Wt::WWidget **pop,int rtype,int line,char *file)
{
	Wt::WContainerWidget *VB=NULL;
	Wt::WHBoxLayout *vb=NULL;
#ifdef __USE_SCROLLAREA_ON_TABLE__
	Wt::WScrollArea *SA=NULL;
	Wt::WContainerWidget *VB1=NULL;
	Wt::WVBoxLayout *vb1=NULL;
#endif /* __USE_SCROLLAREA_ON_TABLE__ */
	Wt::WLayout *daL=NULL;
	Wt::WTable *myTable;
	Wt::WTableColumn *TC=NULL;
	Wt::WWidget *hold=NULL;
	Wt::WText *myText=NULL;
	int frame_style=(-1);
	Wt::WTableCell *TE=NULL;
	Wt::WTableRow *HH=NULL;
	Wt::WString *temp_xstr=NULL;
	Wt::WLength wl,sw;
	Wt::WImage *myImage=NULL;
	Wt::WLink *WK=NULL;
	RDAwdgt *wdgt=NULL;
	RDArmem *member=NULL;
	char *dashes=NULL,last=FALSE,use_header=FALSE,*rname=NULL;
	int h=0,boxmaxr=0,boxmaxc=0,row=0,col=0,row_span=0,col_span=0;
	int w=0,main_width=0,header_width=0,vheader_width=0;
	int x=0,rows=0,rowsx=0;
	Wt::WLength spc;
	char *mssc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crttablecontainer Creating Table for Screen [%s] [%s] Starting with WWidget [%d] at line [%d] program [%s].",rsrc->module,rsrc->screen,*widgetcount,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	VB=new Wt::WContainerWidget((Wt::WContainerWidget *)parent);
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s TableContainer",mssc);
	if(mssc!=NULL) Rfree(mssc);
	VB->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n","",VB->styleClass().toUTF8().c_str());
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	vb=new Wt::WHBoxLayout();
	VB->setLayout(vb);
	vb->setSpacing(0);
	daL=(Wt::WLayout *)vb;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	VB->setPadding(spc,All);
	*pop=(Wt::WWidget *)VB;
#ifdef __USE_SCROLLAREA_ON_TABLE__
	SA=new Wt::WScrollArea((Wt::WContainerWidget *)VB);
	vb->addWidget(SA);
	VB1=new Wt::WContainerWidget();
	SA->setWidget((Wt::WWidget *)VB1);
	vb->setStretchFactor(SA,800);
	vb1=new Wt::WVBoxLayout();
	VB1->setLayout(vb1);
	myTable=new Wt::WTable();
	myTable->addStyleClass("polished");
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s Table",mssc);
	if(mssc!=NULL) Rfree(mssc);
	myTable->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myTable->styleClass().toUTF8().c_str());
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	vb1->addWidget(myTable);
	vb1->setStretchFactor(myTable,750);
	vb1->setSpacing(0);
	daL=(Wt::WLayout *)vb1;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	VB1->setPadding(spc,All);
#else
	myTable=new Wt::WTable();
	myTable->addStyleClass("polished");
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s Table",mssc);
	if(mssc!=NULL) Rfree(mssc);
	myTable->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myTable->styleClass().toUTF8().c_str());
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	vb1->addWidget(myTable);
#endif /* __USE_SCROLLAREA_ON_TABLE__ */
	myTable->setHeaderCount(1);
/* Determine Rows & Columns */
	boxmaxr=boxmaxrows(scn,*widgetcount);
	if(boxmaxr<1) boxmaxr=1;
	rows=boxmaxr;
#ifdef __USE_SCROLLAREA_ON_TABLE__
	if(boxmaxr<=20)
	{
		SA->setHorizontalScrollBarPolicy(Wt::WScrollArea::ScrollBarAsNeeded);
		SA->setVerticalScrollBarPolicy(Wt::WScrollArea::ScrollBarAsNeeded);
	} else {
		SA->setHorizontalScrollBarPolicy(Wt::WScrollArea::ScrollBarAsNeeded);
		SA->setVerticalScrollBarPolicy(Wt::WScrollArea::ScrollBarAlwaysOn);
		sw=WLength(400,Wt::WLength::Pixel);
		SA->setMinimumSize(sw,Wt::WLength::Auto);
		rsrc->has_large_table=TRUE;		
	}
#endif /* __USE_SCROLLAREA_ON_TABLE__ */
	boxmaxc=boxmaxcols(scn,*widgetcount);
	if(ContainsWtHeader(scn,*widgetcount))
	{
/*
		boxmaxr-=1;
*/
		row=1;
		use_header=TRUE;
	}
/*
	if(rtype==1 || rtype==3 || rtype==5) boxmaxc-=1;
*/

/* fix this loop right here */
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+*widgetcount;
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG xcrttablecontainer Creating WWidget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
				*widgetcount,wdgttypes[wdgt->type],(wdgt->resource_name!=NULL ? 
				wdgt->resource_name:""),(wdgt->label!=NULL ? wdgt->label:""),
				wdgt->rows,wdgt->cols,wdgt->rtype); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(wdgt->resource_num>=0) 
		{
			if(wdgt->resource_num<rsrc->numrscs)
			{
				member=rsrc->rscs+wdgt->resource_num;
			} else member=NULL;
			TC=myTable->columnAt(col);
			TE=myTable->elementAt(row,col);
			mssc=ModuleScreenStyleClass(rsrc);
			memset(GUIstemp,0,1024);
			sprintf(GUIstemp,"OpenRDA %s TableElement",mssc);
			if(mssc!=NULL) Rfree(mssc);
			TE->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
			if(diagcss)
			{
				if(cssNames!=NULL)
				{
					fprintf(cssNames,"\"%s\",\"%s\"\r\n","",TE->styleClass().toUTF8().c_str());
				}
			}
#endif /* USE_RDA_DIAGNOSTICS */
			switch(wdgt->type)
			{
				case 8: /* scrolled text */
				case 7: /* scrolled list */
					if(member!=NULL)
					{
						member->cols=wdgt->cols;
						if(member->editable_expression!=NULL) Rfree(member->editable_expression);
						if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
						if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
						if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
						if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
						if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
						makefield((Wt::WWidget *)TE,member,wdgt->label,wdgt->XHTML_Label,wdgt->pixmap,wdgt->rows,wdgt->cols,wdgt->rtype,myTable,row,col,row_span,col_span);
						if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) hold=(Wt::WWidget *)member->w->parent();
							else hold=member->w;
						if(wdgt->type==7 && wdgt->rows<2) last=FALSE;
						else if(wdgt->type==8 && wdgt->rows==1) last=FALSE;
						else last=TRUE;
						if(wdgt->type==7 && wdgt->rows==1)
						{
							w=17;
							wl=Wt::WLength(w);
							sw=TC->width();
							if(wl.value()<sw.value())
							{
								TC->setWidth(sw);
							} else if(wl.value()>sw.value())
							{
								TC->setWidth(wl);
							} 
						}
						TE->addWidget(hold);
					}
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					if(member->rows>1) rows+=(wdgt->rows-1);
					++col;
					break;
				case 15: /* custom input fields */
				case 16: /* expenditure fields */
				case 17: /* revenue fields */
				case 18: /* balance sheet fields */
				case 19: /* beginning balance fields */
				case 0: /* standard resource fields */
				case 6: /* buttons */
				case 9: /* toggle buttons */
				case 14: /* optional screen button */
				case 22: /* Progress Bar */
					if(member!=NULL)
					{
						member->cols=wdgt->cols;
						if(member->editable_expression!=NULL) Rfree(member->editable_expression);
						if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
						if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
						if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
						if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
						if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
						makefield((Wt::WWidget *)TE,member,wdgt->label,wdgt->XHTML_Label,wdgt->pixmap,wdgt->rows,wdgt->cols,wdgt->rtype,myTable,row,col,row_span,col_span);
						if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) hold=(Wt::WWidget *)member->w->parent();
							else hold=member->w;
						TE->addWidget(hold);
						if(wdgt->type==6)
						{
							if(RDAstrstr(member->rscrname,":DELETE"))
							{
								w=16;
								wl = Wt::WLength(w);
								sw=TC->width();
								if(wl.value()<sw.value())
								{
									TC->setWidth(sw);
								} else if(wl.value()>sw.value())
								{
									TC->setWidth(wl);
								}
							}
						} else if(wdgt->type==9)
						{
						} else {
							w=member->cols+2;
							wl = Wt::WLength(w);
							sw=TC->width();
							if(wl.value()<sw.value())
							{
								TC->setWidth(sw);
							} else if(wl.value()>sw.value())
							{
								TC->setWidth(wl);
							}
						}
					}
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					last=FALSE;
					++col;
					break;
				case 1: /* start newline */
					break;
				case 2: /* endline */
					++row;
					col=0;
					break;
				case 23: /* New Tab Bar */
					rowsx=crttab((Wt::WWidget *)TE,scn,rsrc,widgetcount,&hold);
					TE->addWidget(hold);
					last=TRUE;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					if(rowsx>1) rows+=(rowsx-1);
					++col;
					break;
				case 24: /* New Popup Menu */
					crtpopup((Wt::WWidget *)TE,scn,rsrc,widgetcount,&hold,FALSE);
					TE->addWidget(hold);
					last=FALSE;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					++col;
					break;
				case 25: /* End Popup Menu */
					break;
				case 26: /* New Toolbar Menu */
					rowsx=crttoolbarcontainer((Wt::WWidget *)TE,scn,rsrc,widgetcount,&hold);
					TE->addWidget(hold);
					last=TRUE;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					if(rowsx>1) rows+=(rowsx-1);
					++col;
					break;
				case 27: /* End ToolBar Menu */
					break;
				case 30: /* New Header */
					crtheadercontainer((Wt::WWidget *)myTable,scn,rsrc,widgetcount,(Wt::WTableRow **)&hold,rtype);
					HH=(WTableRow *)hold;
					break;
				case 31: /* End Header */
					break;
				case 28: /* New Table Container */
					rowsx=crttablecontainer((Wt::WWidget *)TE,scn,rsrc,widgetcount,&hold,wdgt->rtype);
					if(rowsx>1) rows+=(rowsx-1);
					TE->addWidget(hold);
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					++col;
					break;
				case 29: /* End Table Container */
					if(HH!=NULL)
					{
						for(x=0;x<boxmaxc;++x)
						{
							TC=myTable->columnAt(x);
							sw=TC->width();
							header_width+=sw.value();
						}
					} else {
						header_width=0;
					}
					hold=(Wt::WWidget *)myTable;
					sw=hold->width();
					main_width=sw.value();
					if(main_width>header_width)
					{
						w=main_width+vheader_width;
					} else {
						w=header_width+vheader_width;
					}
					sw=hold->height();
					wl=Wt::WLength(w);
					hold->setMinimumSize(wl,sw);
					return(rows);
				case 20: /* New Tabbed Container */
				case 21: /* End Tabbed Container */
					prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
					hold=NULL;
					break;
				case 3: /* start box */
					rowsx=crtbox((Wt::WWidget *)TE,scn,rsrc,widgetcount,&hold);
					TE->addWidget(hold);
					last=TRUE;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					if(rowsx>1) rows+=(rowsx-1);
					++col;
					break;
				case 4: /* end box */
					prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
					hold=NULL;
					break;
				case 5: /* LABELS */
					temp_xstr = new WString(wdgt->label,UTF8);
					if((rtype!=1 && rtype!=3) || col>0)
					{
						dashes=adddashes(wdgt->label);
						if(wdgt->rtype==0)
						{
							if(isEMPTY(wdgt->XHTML_Label)) 
							{
								myText = new Wt::WText(wdgt->label);
								mssc=ModuleScreenStyleClass(rsrc);
								memset(GUIstemp,0,1024);
								sprintf(GUIstemp,"OpenRDA %s Label",mssc);
								if(mssc!=NULL) Rfree(mssc);
								myText->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
								if(diagcss)
								{
									if(cssNames!=NULL)
									{
										fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),myText->styleClass().toUTF8().c_str());
									}
								}
#endif /* USE_RDA_DIAGNOSTICS */
								h=getLeadingSpaces(wdgt->label);
								hold = (Wt::WWidget *)myText;
								if(h>0)
								{
									spc=Wt::WLength(h,Wt::WLength::FontEm);
									myText->setPadding(spc,Left);
								}
							} else {
								myText = new Wt::WText(wdgt->XHTML_Label,XHTMLText);
							}
						} else if(wdgt->rtype==1)
						{
							WK = new WLink(wdgt->pixmap);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TE);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myImage->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myImage->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						} else if(wdgt->rtype==2)
						{
							rname=Rmalloc(512);
#ifndef RDA_64BITS
							sprintf(rname,"resources/OpenRDA/rda.png");
#else
							sprintf(rname,"resources/OpenRDA/rda64.png");
#endif
							WK = new WLink(rname);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TE);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							if(rname!=NULL) Rfree(rname);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myImage->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myImage->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						} else if(wdgt->rtype==3)
						{
							rname=Rmalloc(512);
							sprintf(rname,"resources/OpenRDA/tuxcloud.png");
							WK = new WLink(rname);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TE);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							if(rname!=NULL) Rfree(rname);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myImage->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myImage->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						} else if(wdgt->rtype==4)
						{
							myText = new Wt::WText(wdgt->XHTML_Label,XHTMLText);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Label",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myText->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),myText->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						}
						TE->addWidget(hold);
						last=FALSE;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
					} else {
						if(wdgt->rtype==0)
						{
							myText = new Wt::WText(wdgt->label);
							h=getLeadingSpaces(wdgt->label);
							hold = (Wt::WWidget *)myText;
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Label",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myText->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),myText->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
							if(h>0)
							{
								spc=Wt::WLength(h,Wt::WLength::FontEm);
								myText->setPadding(spc,Left);
							}
						} else if(wdgt->rtype==1)
						{
							WK = new WLink(wdgt->pixmap);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TE);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myImage->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myImage->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						} else if(wdgt->rtype==2)
						{
							rname=Rmalloc(512);
#ifndef RDA_64BITS
							sprintf(rname,"resources/OpenRDA/rda.png");
#else
							sprintf(rname,"resources/OpenRDA/rda64.png");
#endif
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TE);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							if(rname!=NULL) Rfree(rname);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myImage->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myImage->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						} else if(wdgt->rtype==3)
						{
							rname=Rmalloc(512);
								sprintf(rname,"resources/OpenRDA/tuxcloud.png");
							WK = new WLink(rname);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TE);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							if(rname!=NULL) Rfree(rname);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myImage->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n","",myImage->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						} else if(wdgt->rtype==4)
						{
							myText = new Wt::WText(wdgt->XHTML_Label,XHTMLText);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Label",mssc);
							if(mssc!=NULL) Rfree(mssc);
							myText->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),myText->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
						}
						TE->addWidget(hold);
						wl=hold->width();
						if(wl.value()>vheader_width) vheader_width=wl.value();
					}
					temp_xstr->~WString();
					++col;
					if(dashes!=NULL) Rfree(dashes);
					break;
				case 10: /* FRAME */
					frame_style=wdgt->rtype;
					last=FALSE;
					break;
				case 11: /* Separator */
					switch(wdgt->rtype)
					{
						default:
						case 0: /* horizontal Single */
							hold=myHorizontalLine((Wt::WContainerWidget *)TE,1);
							break;
						case 1:
							hold=myHorizontalLine((Wt::WContainerWidget *)TE,2);
							break;
						case 2:
							hold=myHorizontalLine((Wt::WContainerWidget *)TE,3);
							break;
						case 3:
							hold=myHorizontalLine((Wt::WContainerWidget *)TE,4);
							break;
						case 4:
							hold=myVerticalLine((Wt::WContainerWidget *)TE,1);
							break;
						case 5:
							hold=myVerticalLine((Wt::WContainerWidget *)TE,2);
							break;
						case 6:
							hold=myVerticalLine((Wt::WContainerWidget *)TE,3);
							break;
						case 7:
							hold=myVerticalLine((Wt::WContainerWidget *)TE,4);
							break;
					}
					TE->addWidget(hold);
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					++col;
					break;
				case 12: /* New ScrolledWindow */
					rowsx=crtscrollwindow((Wt::WWidget *)TE,scn,rsrc,widgetcount,&hold);
					TE->addWidget((Wt::WWidget *)hold);
					last=TRUE;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold,frame_style);
						frame_style=(-1);
					}
					if(rowsx>1) rows+=(rowsx-1);
					++col;
					break;
				case 13: /* End ScrolledWindow */
					prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
					hold=NULL;
					break;
				default:
					prterr("Error WWidget *Type [%d] is invalid for WWidget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
					hold=NULL;
					break;
			}
		}
	}
}
/*---------------------------------------------------------------------------
	crtline - function to create lines inside a window
---------------------------------------------------------------------------*/
int xcrtline(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsrc,
	int *widgetcount,Wt::WWidget **line,int linex,char *file)
{
	Wt::WWidget *hold=NULL;
	Wt::WText *myText=NULL;
	Wt::WString *temp_xstr=NULL;
	Wt::WImage *MyP=NULL;
	Wt::WLink *WK=NULL;
	int frame_style=(-1);
	Wt::WImage *myImage=NULL;
	Wt::WHBoxLayout *lBox=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WContainerWidget *myLine=NULL,*myLabel=NULL;
	RDAwdgt *wdgt=NULL;
	int num,rows=0,rowsx=0;
	RDArmem *member=NULL;
	char *tmpstr=NULL,*dashes=NULL,last=FALSE,useit=FALSE,*rname=NULL;
	int h=0,w=0,z=0;
	Wt::WLength spc;
	char *mssc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crtline Creating Custom Line for Screen [%s] [%s] Starting with WWidget [%d] at line [%d] program [%s].",rsrc->module,rsrc->screen,*widgetcount,linex,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	num=numperline(scn,*widgetcount);
	if(num<0) return(0);
	if(num>0)
	{
		if(!USER_INTERFACE)
		{
			myLine = new Wt::WContainerWidget((Wt::WContainerWidget *)parent);
			mssc=ModuleScreenStyleClass(rsrc);
			memset(GUIstemp,0,1024);
			sprintf(GUIstemp,"OpenRDA %s LineContainer",mssc);
			if(mssc!=NULL) Rfree(mssc);
			myLine->addStyleClass(GUIstemp);
			*line=(WWidget *)myLine;
			spc=Wt::WLength(0,Wt::WLength::Pixel);
			myLine->setPadding(spc,All);
			lBox=new Wt::WHBoxLayout();
			daL=(Wt::WLayout *)lBox;
			daL->setContentsMargins(0,0,0,0);
			lBox->setSpacing(2);
			myLine->setLayout(lBox);
		}
	}
/* fix this loop right here */
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+*widgetcount;
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG Creating WWidget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
				*widgetcount,wdgttypes[wdgt->type],(wdgt->resource_name!=NULL ? 
				wdgt->resource_name:""),(wdgt->label!=NULL ? wdgt->label:""),
				wdgt->rows,wdgt->cols,wdgt->rtype); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(wdgt->resource_num>=0) 
		{
			if(wdgt->resource_num<rsrc->numrscs)
			{
				member=rsrc->rscs+wdgt->resource_num;
			} else member=NULL;
			switch(wdgt->type)
			{
				case 8: /* scrolled text */
				case 7: /* scrolled list */
					if(num)
					{
						if(member!=NULL)
						{
							member->cols=wdgt->cols;
							if(member->editable_expression!=NULL) Rfree(member->editable_expression);
							if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
							if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
							if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
							if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
							if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
							makefield((Wt::WWidget *)*line,member,wdgt->label,wdgt->XHTML_Label,wdgt->pixmap,wdgt->rows,wdgt->cols,wdgt->rtype,NULL,0,0,0,0);
							if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) hold=(Wt::WWidget *)member->w->parent();
								else hold=member->w;
							lBox->addWidget((Wt::WWidget *)hold);
							if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) lBox->setStretchFactor(hold,200);
							else if(wdgt->type==8) lBox->setStretchFactor(hold,500);
							else if((wdgt->type==7 && wdgt->rows>1))
							{
								lBox->setStretchFactor((Wt::WWidget *)hold,100);
							}
							if(wdgt->type==7 && wdgt->rows>1) last=TRUE;
							else if(wdgt->type==8) last=TRUE;
							else last=FALSE;
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
							if(member->rows>rows) rows=member->rows;
						}
					}
					break;
				case 15: /* custom input fields */
				case 16: /* expenditure fields */
				case 17: /* revenue fields */
				case 18: /* balance sheet fields */
				case 19: /* beginning balance fields */
				case 0: /* standard resource fields */
				case 6: /* buttons */
				case 9: /* toggle buttons */
				case 14: /* optional screen button */
				case 22: /* Progress Bar */
					if(num)
					{
						useit=TRUE;
						if(PUSHBUTTON_STYLE==2)
						{
							if(!RDAstrcmp(member->rscrname,"SELECT"))
							{
								if(FINDRSC(rsrc,"BROWSE LIST")!=(-1)) useit=FALSE;
							}
						}
						if(member!=NULL && useit==TRUE)
						{
							member->cols=wdgt->cols;
							if(member->editable_expression!=NULL) Rfree(member->editable_expression);
							if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
							if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
							if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
							if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
							if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
							makefield((Wt::WWidget *)*line,member,wdgt->label,wdgt->XHTML_Label,wdgt->pixmap,wdgt->rows,wdgt->cols,wdgt->rtype,NULL,0,0,0,0);
							if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) hold=(Wt::WWidget *)member->w->parent();
								else hold=member->w;
							if(wdgt->type==9)
							{
								lBox->addWidget((Wt::WWidget *)hold,0,Wt::AlignMiddle);
							} else {
								lBox->addWidget((Wt::WWidget *)hold);
							}
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
							if(rows==0) rows=1;
							last=FALSE;
/*
							if(wdgt->type!=6 && wdgt->type!=9 && wdgt->type!=14) last=FALSE;
								else last=TRUE;
*/
						}
					}
					break;
				case 1: /* start newline */
					if(num)
					{
						rowsx=crtline((Wt::WWidget *)*line,scn,rsrc,widgetcount,&hold);
						if(rowsx>0)
						{
						lBox->addWidget((Wt::WWidget *)hold);
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
						}
					}
					break;
				case 2: /* endline */
					if(!USER_INTERFACE)
					{
						if(num)
						{
							if(RDAstrstr(rsrc->screen,"DOCK WINDOW"))
							{
								if(hold!=NULL)
								{
								if(!last)
								{
									hold =(Wt::WWidget *) new Wt::WText("<pre> </pre>",Wt::XHTMLText);
									mssc=ModuleScreenStyleClass(rsrc);
									memset(GUIstemp,0,1024);
									sprintf(GUIstemp,"OpenRDA %s LineStretch",mssc);
									if(mssc!=NULL) Rfree(mssc);
									hold->addStyleClass(GUIstemp);
									lBox->setStretchFactor((Wt::WWidget *)hold,100);
								}
								}
							} else if(hold!=NULL)
							{
								if(!last) lBox->addStretch(500);
							}
							hold=NULL;
						}
					}
					return(rows);
				case 23: /* New Tab Bar */
					if(num)
					{
						rowsx=crttab((Wt::WWidget *)*line,scn,rsrc,widgetcount,&hold);
						lBox->addWidget((Wt::WWidget *)hold);
						last=TRUE;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 24: /* New Popup Menu */
					if(num)
					{
						crtpopup((Wt::WWidget *)*line,scn,rsrc,widgetcount,&hold,FALSE);
						lBox->addWidget((Wt::WWidget *)hold);
						last=FALSE;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
					}
					break;
				case 25: /* End Popup Menu */
					break;
				case 26: /* New Toolbar Menu */
					if(num)
					{
						rowsx=crttoolbarcontainer((Wt::WWidget *)*line,scn,rsrc,widgetcount,&hold);
						lBox->addWidget((Wt::WWidget *)hold);
						last=TRUE;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 27: /* End ToolBar Menu */
					break;
				case 30: /* New Header */
				case 31: /* End Header */
					break;
				case 28: /* New Table Container */
					if(num)
					{
						rowsx=crttablecontainer((Wt::WWidget *)*line,scn,rsrc,widgetcount,&hold,wdgt->rtype);
						lBox->addWidget((Wt::WWidget *)hold);
						last=TRUE;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 29: /* End Table Container */
					break;
				case 20: /* New Tabbed Container */
				case 21: /* End Tabbed Container */
					if(num)
					{
						prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
						hold=NULL;
					}
					break;
				case 3: /* start box */
					if(num)
					{
						rowsx=crtbox((Wt::WWidget *)*line,scn,rsrc,widgetcount,&hold);
						if(rowsx>0)
						{
							lBox->addWidget((Wt::WWidget *)hold,500);
							last=TRUE;
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
							if(rowsx>rows) rows=rowsx;
						}
					}
					break;
				case 4: /* end box */
					if(num)
					{
						prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
						hold=NULL;
					}
					break;
				case 5: /* LABELS */
					if(num)
					{
						if(wdgt->label!=NULL)
						{
							tmpstr=Rmalloc(RDAstrlen(wdgt->label)+12);
							sprintf(tmpstr,"<pre>%s</pre>",(wdgt->label!=NULL ? wdgt->label:""));
						} else {
							tmpstr=stralloc(" ");
						}
						temp_xstr = new WString(tmpstr,UTF8);
						dashes=adddashes(wdgt->label);
						if(wdgt->rtype==0)
						{
							myText = new Wt::WText(*temp_xstr,Wt::XHTMLText);
							hold=(Wt::WWidget *)myText;
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Label",mssc);
							if(mssc!=NULL) Rfree(mssc);
							hold->addStyleClass(GUIstemp);
						} else if(wdgt->rtype==1)
						{
							WK = new WLink(wdgt->pixmap);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)myLine);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							hold->addStyleClass(GUIstemp);
						} else if(wdgt->rtype==2)
						{
							rname=Rmalloc(512);
#ifndef RDA_64BITS
							sprintf(rname,"resources/OpenRDA/rda.png");
#else
							sprintf(rname,"resources/OpenRDA/rda64.png");
#endif
							WK = new WLink(rname);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)myLine);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							if(rname!=NULL) Rfree(rname);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							hold->addStyleClass(GUIstemp);
						} else if(wdgt->rtype==3)
						{
							rname=Rmalloc(512);
							sprintf(rname,"resources/OpenRDA/tuxcloud.png");
							WK = new WLink(rname);
							myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)myLine);
							hold = (WWidget *)myImage;
							myImage->setAlternateText(*temp_xstr);
							if(rname!=NULL) Rfree(rname);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Image",mssc);
							if(mssc!=NULL) Rfree(mssc);
							hold->addStyleClass(GUIstemp);
						} else if(wdgt->rtype==4)
						{
							hold = new Wt::WText(wdgt->XHTML_Label,Wt::XHTMLText);
							mssc=ModuleScreenStyleClass(rsrc);
							memset(GUIstemp,0,1024);
							sprintf(GUIstemp,"OpenRDA %s Label",mssc);
							if(mssc!=NULL) Rfree(mssc);
							hold->addStyleClass(GUIstemp);
						}
#ifdef USE_RDA_DIAGNOSTICS
						if(diagcss)
						{
							if(cssNames!=NULL)
							{
								fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),hold->styleClass().toUTF8().c_str());
							}
						}
#endif /* USE_RDA_DIAGNOSTICS */
						lBox->addWidget((Wt::WWidget *)hold);
						last=FALSE;
						if(rows==0) rows=1;
						if(dashes!=NULL) Rfree(dashes);
						temp_xstr->~WString();
						if(tmpstr!=NULL) Rfree(tmpstr);
						tmpstr=NULL;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
					}
					break;
				case 10: /* FRAME */
					if(num)
					{
						frame_style=wdgt->rtype;
						last=TRUE;
					}
					break;
				case 11: /* Separator */
					if(num)
					{
						switch(wdgt->rtype)
						{
							default:
							case 0: /* horizontal Single */
								hold=myHorizontalLine((Wt::WContainerWidget *)myLine,1);
								break;
							case 1:
								hold=myHorizontalLine((Wt::WContainerWidget *)myLine,2);
								break;
							case 2:
								hold=myHorizontalLine((Wt::WContainerWidget *)myLine,3);
								break;
							case 3:
								hold=myHorizontalLine((Wt::WContainerWidget *)myLine,4);
								break;
							case 4:
								hold=myVerticalLine((Wt::WContainerWidget *)myLine,1);
								break;
							case 5:
								hold=myVerticalLine((Wt::WContainerWidget *)myLine,2);
								break;
							case 6:
								hold=myVerticalLine((Wt::WContainerWidget *)myLine,3);
								break;
							case 7:
								hold=myVerticalLine((Wt::WContainerWidget *)myLine,4);
								break;
						}	
						lBox->addWidget((Wt::WWidget *)hold);
						last=TRUE;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
					}
					break;
				case 12: /* New ScrolledWindow */
					if(num)
					{
						rowsx=crtscrollwindow((WWidget *)*line,scn,rsrc,widgetcount,&hold);
						lBox->addWidget((Wt::WWidget *)hold);
						last=TRUE;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 13: /* End ScrolledWindow */
					if(num)
					{
						prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
						hold=NULL;
					}
					break;
				default:
					prterr("Error WWidget *Type [%d] is invalid for WWidget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
					hold=NULL;
					break;
			}
		}
	}
}
static int BoxWidgetWidth(RDAscrn *scn,RDArsrc *rsrc,int widgetcount)
{
	RDAwdgt *wdgt=NULL;
	RDArmem *member=NULL;
	int longest=0,w=0;

	while((widgetcount+1)<scn->numwdgts)
	{
		++(widgetcount);
		wdgt=scn->wdgts+widgetcount;
		if(wdgt->resource_num>=0) 
		{	
			if(wdgt->resource_num<rsrc->numrscs)
			{
				member=rsrc->rscs+wdgt->resource_num;
			} else member=NULL;
			switch(wdgt->type)
			{
				default:
				case 3: /* new box */
				case 7: /* scrolled list */
				case 23: /* new tab bar */
					w=0;
					break;
				case 5:
					w=RDAstrlen(wdgt->label);
					break;
				case 8: /* scrolled text */
					w=wdgt->cols;
					break;
				case 6: /* buttons */
				case 14:/* optional screens */
					if(wdgt->rtype==0)
					{
						w=RDAstrlen(wdgt->label)+2;
					}
					break;
				case 9: /* toggle buttons */
					w=RDAstrlen(wdgt->label)+4;
					break;
				case 22: /* progress_bar */
					break;
				case 15: /* custom input fields */
				case 16: /* expenditure fields */
				case 17: /* revenue fields */
				case 18: /* balance sheet fields */
				case 19: /* beginning balance fields */
				case 0: /* fields */
					if(wdgt->cols!=0)
					{
						w=wdgt->cols;
					} else w=member->field_length;
					break;
				case 1: /* new line */
				case 2: /* end line */
					break;
				case 4: /* end box */
					return(longest);
			}
			if(w>longest) longest=w;
		}
	}
	return(longest);
}
int xcrttoolbarcontainer(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsrc,int *widgetcount,Wt::WWidget **pop,int line,char *file)
{
	Wt::WPopupMenu *popMenu=NULL;
	Wt::WPushButton *pB=NULL,*B=NULL;
	RDAwdgt *wdgt=NULL;
	RDArmem *member=NULL;
	Wt::WString *c=NULL;
	char *dashes=NULL;
	WWidget *hold=NULL;
	int x=0,count=0;
	char *mssc=NULL;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crttoolbarcontainer Creating ToolBar Container Screen [%s] [%s] starting with WWidget [%d] at line [%d] program [%s].",scn->module,scn->name,*widgetcount,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+(*widgetcount);
	if(rsrc->window_toolbar==NULL)
	{
		rsrc->window_toolbar=new Wt::WToolBar((Wt::WWidget *)parent);
		mssc=ModuleScreenStyleClass(rsrc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s ToolBar",mssc);
		if(mssc!=NULL) Rfree(mssc);
		rsrc->window_toolbar->addStyleClass(GUIstemp);
	}
	*pop=(Wt::WWidget *)rsrc->window_toolbar;
	B=new Wt::WPushButton();
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s ToolBar PushButton",mssc);
	if(mssc!=NULL) Rfree(mssc);
	B->addStyleClass(GUIstemp);
	popMenu = new Wt::WPopupMenu();
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s ToolBar PushButton PopupMenu",mssc);
	if(mssc!=NULL) Rfree(mssc);
	popMenu->addStyleClass(GUIstemp);
	popMenu->setAutoHide(TRUE,1500);
	c=new WString(wdgt->label);
	B->setText(*c);
	c->~WString();
	B->setMenu(popMenu);
	B->setDefault(FALSE);
	rsrc->window_toolbar->addButton(B);
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+(*widgetcount);
		if(wdgt->resource_num>=0) 
		{
			if(wdgt->resource_num<rsrc->numrscs)
			{
				member=rsrc->rscs+wdgt->resource_num;
				member->parent=rsrc;
				member->toolbar_w=rsrc->window_toolbar;
			} else member=NULL;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Creating WWidget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
					*widgetcount,wdgttypes[wdgt->type],
					(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
					(wdgt->label!=NULL ? wdgt->label:""),
					wdgt->rows,wdgt->cols,wdgt->rtype); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(wdgt->type==6) /* buttons */
			{
				++count;
				if(member->editable_expression!=NULL) Rfree(member->editable_expression);
				if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
				if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
				if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
				if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
				if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
				dashes=adddashes(wdgt->label);
				if(member->label==NULL && !isEMPTY(wdgt->label)) member->label=stralloc(wdgt->label);
				else if(member->label==NULL) member->label=stralloc(member->rscrname);
				popMenu->addItem(member->label)->triggered().connect(boost::bind(&xExecuteRDArmemFunction,member,__LINE__,__FILE__));
				member->popup_id=count-1;
				member->menuitem=popMenu->items().at(member->popup_id);
				member->w=(Wt::WWidget *)member->menuitem;
				member->popup_w=(Wt::WPopupMenu *)popMenu;
				member->field_type=BUTTONS;
			} else if(wdgt->type==24) /* Popup */
			{
				x=crtpopup((Wt::WPopupMenu *)popMenu,scn,rsrc,widgetcount,&hold,TRUE);
				if(x>0) 
				{
					++count;
					popMenu->addMenu(wdgt->label,(Wt::WPopupMenu *)hold);
				}
			} else if(wdgt->type==27) /* End Toolbar Menu */
			{
				break;
			} else {
				prterr("Error WWidget *Type [%d] is invalid for WWidget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
			}
		}
	}
	return(1);
}
int xcrtpopup(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsrc,int *widgetcount,Wt::WWidget **pop,short parent_type,int line,char *file)
{
	Wt::WPopupMenu *pB=NULL,*ppB=NULL;
	Wt::WPushButton *B=NULL;
	Wt::WWidget *hold=NULL;
	Wt::WString *c=NULL;
	Wt::WMenuItem *Item=NULL;
	RDAwdgt *wdgt=NULL;
	RDArmem *member=NULL;
	char *dashes=NULL;
	int count=0,x=0;
	char *mssc=NULL;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crtpopup Creating Popup Menu Container Screen [%s] [%s] starting with WWidget [%d] at line [%d] program [%s].",scn->module,scn->name,*widgetcount,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+(*widgetcount);
	if(parent_type==TRUE)
	{
		pB = new Wt::WPopupMenu();
		mssc=ModuleScreenStyleClass(rsrc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s PushButton PopupMenu",mssc);
		if(mssc!=NULL) Rfree(mssc);
		pB->addStyleClass(GUIstemp);
		pB->setAutoHide(TRUE,1500);
		*pop=(Wt::WWidget *)pB;
		ppB=((Wt::WPopupMenu *)parent);		
		ppB->addMenu(wdgt->label,(Wt::WPopupMenu *)pB);
	} else {
		c = new WString(wdgt->label);
		B = new Wt::WPushButton(*c,(Wt::WContainerWidget *)parent);
		*pop=(Wt::WWidget *)B;
		B->setDefault(FALSE);
		B->setTextFormat(XHTMLText); /* or PlainText */
		mssc=ModuleScreenStyleClass(rsrc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s PushButton",mssc);
		if(mssc!=NULL) Rfree(mssc);
		B->addStyleClass(GUIstemp);
		c->~WString();
		pB = new Wt::WPopupMenu();
		mssc=ModuleScreenStyleClass(rsrc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s PushButton PopupMenu",mssc);
		if(mssc!=NULL) Rfree(mssc);
		pB->addStyleClass(GUIstemp);
		pB->setAutoHide(TRUE,1500);
		B->setMenu(pB);
		B->setDefault(FALSE);
#ifdef FLAT_DOCK_BUTTON
		if(RDAstrstr(rsrc->screen,"DOCK WINDOW"))
		{
			B->setCheckable(TRUE);
		} else B->setCheckable(FALSE);
#endif /* FLAT_DOCK_BUTTON */
	}
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+(*widgetcount);
		if(wdgt->resource_num>=0) 
		{
			if(wdgt->resource_num<rsrc->numrscs)
			{
				member=rsrc->rscs+wdgt->resource_num;
			} else member=NULL;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Creating WWidget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
					*widgetcount,wdgttypes[wdgt->type],
					(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
					(wdgt->label!=NULL ? wdgt->label:""),
					wdgt->rows,wdgt->cols,wdgt->rtype); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(wdgt->type==6) /* buttons */
			{
				++count;
				if(member->editable_expression!=NULL) Rfree(member->editable_expression);
				if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
				if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
				if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
				if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
				if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
				if(member->label==NULL && !isEMPTY(wdgt->label)) member->label=stralloc(wdgt->label);
				else if(member->label==NULL) member->label=stralloc(member->rscrname);
				pB->addItem(member->label)->triggered().connect(boost::bind(&xExecuteRDArmemFunction,member,__LINE__,__FILE__));
				member->popup_id=count-1;
				member->menuitem=pB->items().at(member->popup_id);
				member->w=(Wt::WWidget *)member->menuitem;
				member->popup_w=(Wt::WPopupMenu *)pB;
				member->field_type=BUTTONS;
			} else if(wdgt->type==24)
			{
				x=crtpopup((Wt::WPopupMenu *)pB,scn,rsrc,widgetcount,&hold,TRUE);
				if(x>0) 
				{
					++count;
					pB->addMenu(wdgt->label,(Wt::WPopupMenu *)hold);
				}
			} else if(wdgt->type==25) /* End Popup Menu */
			{
				return(count);
			} else {
				prterr("Error WWidget *Type [%d] is invalid for WWidget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
			}
		}
	}
}
int xcrtbox(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsrc,int *widgetcount,Wt::WWidget **box,int line,char *file)
{
	Wt::WWidget *hold=NULL;
	RDAwdgt *wdgt=NULL;
	RDArmem *member=NULL;
	char *tmpstr=NULL,*dashes=NULL,useit=FALSE,*rname=NULL;
	WImage *MyP=NULL;
	Wt::WContainerWidget *hBox=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WGridLayout *myGrid=NULL;
	int frame_style=(-1);
	Wt::WImage *myImage=NULL;
	Wt::WLink *WK=NULL;
	Wt::WString *temp_xstr=NULL;
	int fractbase=0,boxmaxr=0,boxmaxc=0,wcount=0;
	int x=0,row=0,col=0;
	int Widest=0;
	Wt::WHBoxLayout *dframe=NULL;
	Wt::WLength wl,spc,pd;
	int h=0,rows=0,rowsx=0;
	Wt::WText *myText=NULL;
	char *mssc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crtbox Creating Custom Box/Scrolled Window for Screen [%s] [%s] Starting with WWidget [%d] at line [%d] program [%s].",scn->module,scn->name,*widgetcount,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	boxmaxr=boxmaxrows(scn,*widgetcount);
	if(boxmaxr<1) boxmaxr=1;
	boxmaxc=boxmaxcols(scn,*widgetcount);
	fractbase=boxmaxr*boxmaxc;
	if(fractbase<1) 
	{
		box=NULL;
		return(0);
	}
	rows=1;
	*box=(Wt::WContainerWidget *)new WContainerWidget((Wt::WContainerWidget *)parent);
	hBox=(Wt::WContainerWidget *)*box;
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s BoxContainer",mssc);
	if(mssc!=NULL) Rfree(mssc);
	hBox->addStyleClass(GUIstemp);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	hBox->setPadding(spc,All);
	myGrid=new Wt::WGridLayout();
	daL=(Wt::WLayout *)myGrid;
	daL->setContentsMargins(0,0,0,0);
	hBox->setLayout(myGrid);
	Widest=BoxWidgetWidth(scn,rsrc,*widgetcount);
	myGrid->setHorizontalSpacing(0);
	myGrid->setVerticalSpacing(0);
	for(x=0;x<boxmaxr;++x) 
	{
		myGrid->setRowStretch(x,0);
	}
	for(x=0;x<boxmaxc;++x) myGrid->setColumnStretch(x,100);
	
	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+*widgetcount;
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG Creating WWidget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
				*widgetcount,wdgttypes[wdgt->type],(wdgt->resource_name!=NULL ? 
				wdgt->resource_name:""),(wdgt->label!=NULL ? wdgt->label:""),
				wdgt->rows,wdgt->cols,wdgt->rtype); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(wdgt->resource_num>=0) 
		{
			if(wdgt->resource_num<rsrc->numrscs)
			{
				member=rsrc->rscs+wdgt->resource_num;
			} else member=NULL;
			useit=TRUE;
			if(PUSHBUTTON_STYLE==2 && member!=NULL)
			{
				if(!RDAstrcmp(member->rscrname,"SELECT"))
				{
					if(FINDRSC(rsrc,"BROWSE LIST")!=(-1)) useit=FALSE;
				}
			}
			if(useit)
			{
			switch(wdgt->type)
			{
				case 8: /* scrolled text */
				case 7: /* scrolled list */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						if(member!=NULL)
						{
							member->cols=wdgt->cols;
							if(member->editable_expression!=NULL) Rfree(member->editable_expression);
							if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
							if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
							if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
							if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
							if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
							makefield((Wt::WWidget *)hBox,member,wdgt->label,wdgt->XHTML_Label,wdgt->pixmap,wdgt->rows,wdgt->cols,wdgt->rtype,NULL,0,0,0,0);
							if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) hold=(Wt::WWidget *)member->w->parent();
								else hold=member->w;
							myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							++col;
							if(wdgt->rows>rows) rows=wdgt->rows;
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
						}
					}
					break;
				case 15: /* custom input fields */
				case 16: /* expenditure fields */
				case 17: /* revenue fields */
				case 18: /* balance sheet fields */
				case 19: /* beginning balance fields */
				case 0: /* fields */
				case 22: /* progress_bar */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{

						if(member!=NULL)
						{
							member->cols=wdgt->cols;
							if(member->editable_expression!=NULL) Rfree(member->editable_expression);
							if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
							if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
							if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
							if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
							if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
							makefield((Wt::WWidget *)hBox,member,wdgt->label,wdgt->XHTML_Label,wdgt->pixmap,wdgt->rows,wdgt->cols,wdgt->rtype,NULL,0,0,0,0);
							if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) hold=(Wt::WWidget *)member->w->parent();
								else hold=member->w;
							myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							++col;
							hold=member->w;
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
						}
					}
					break;
				case 6: /* buttons */
				case 9: /* toggle buttons */
				case 14:/* optional screens */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						if(member!=NULL)
						{
							member->cols=wdgt->cols;
							if(member->editable_expression!=NULL) Rfree(member->editable_expression);
							if(!isEMPTY(wdgt->editable_expression)) member->editable_expression=stralloc(wdgt->editable_expression);
							if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
							if(!isEMPTY(wdgt->sensitive_expression)) member->sensitive_expression=stralloc(wdgt->sensitive_expression);
							if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
							if(!isEMPTY(wdgt->transversal_expression)) member->transversal_expression=stralloc(wdgt->transversal_expression);
							makefield((Wt::WWidget *)hBox,member,wdgt->label,wdgt->XHTML_Label,wdgt->pixmap,wdgt->rows,wdgt->cols,wdgt->rtype,NULL,0,0,0,0);
							if(!RDAstrcmp(member->rscrname,"BROWSE LIST")) hold=(Wt::WWidget *)member->w->parent();
								else hold=member->w;
							if(wdgt->type==9)
							{
								myGrid->addWidget((Wt::WWidget *)hold,row,col,AlignMiddle|AlignJustify);
							} else {
								myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							}
							++col;
							if(member->field_type!=BOOLNS && Widest!=0)
							{
								wl = Wt::WLength(Widest);
								member->w->setMinimumSize(wl,member->w->minimumHeight());
							}
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
						}
					}
					break;
				case 1: /* start newline */
					hold=NULL;
					break;
				case 2: /* endline */
					if(fractbase)
					{
						hold=NULL;
						++row;
						col=0;
					}
					break;
				case 24: /* New Popup Menu */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						crtpopup((Wt::WWidget *)hBox,scn,rsrc,widgetcount,&hold,FALSE);
						myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
						++col;
						if(Widest!=0)
						{
							wl = Wt::WLength(Widest);
							hold->setMinimumSize(wl,hold->minimumHeight());
						}
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
					}
					break;
				case 25: /* End Popup Menu */
					break;
				case 26: /* New Toolbar Menu */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						rowsx=crttoolbarcontainer((Wt::WWidget *)hBox,scn,rsrc,widgetcount,&hold);
						myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
						++col;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 27: /* End Toolbar Container */
					break;
				case 30: /* New Header */
				case 31: /* End Header */
					break;
				case 28: /* New Table */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						rowsx=crttablecontainer((Wt::WWidget *)hBox,scn,rsrc,widgetcount,&hold,wdgt->rtype);
						myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
						++col;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 29: /* End Table Container */
					break;
				case 23: /* new tab bar */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						rowsx=crttab((Wt::WWidget *)hBox,scn,rsrc,widgetcount,&hold);
						myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
						++col;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 20: /* new tabbed container */
				case 21: /* end tabbed container */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
						hold=NULL;
					}
					break;
				case 3: /* start box */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						rowsx=crtbox((Wt::WWidget *)hBox,scn,rsrc,widgetcount,&hold);
						if(rowsx>0)
						{
							myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							++col;
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
							if(rowsx>rows) rows=rowsx;
						}
					}
					break;
				case 4: /* end box */
					return(rows*boxmaxr);
				case 5: /* LABELS */
					if(!USER_INTERFACE)
					{
						if(wdgt->label!=NULL) tmpstr=stralloc(wdgt->label);
							else tmpstr=stralloc(" ");
						if(fractbase)
						{
							dashes=adddashes(wdgt->label);
							temp_xstr = new WString(tmpstr);
							if(wdgt->rtype==0)
							{
								myText = new Wt::WText(wdgt->label);
								h=getLeadingSpaces(wdgt->label);
								hold = (Wt::WWidget *)myText;
								mssc=ModuleScreenStyleClass(rsrc);
								memset(GUIstemp,0,1024);
								sprintf(GUIstemp,"OpenRDA %s Label",mssc);
								if(mssc!=NULL) Rfree(mssc);
								hold->addStyleClass(GUIstemp);
								if(h>0)
								{
									spc=Wt::WLength(h,Wt::WLength::FontEm);
									myText->setPadding(spc,Left);
								}
								myGrid->addWidget((Wt::WWidget *)hold,row,col,AlignMiddle|AlignJustify);
							} else if(wdgt->rtype==1)
							{
								WK = new WLink((const char *)wdgt->pixmap);
								myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)hBox);
								hold = (WWidget *)myImage;
								myImage->setAlternateText(*temp_xstr);
								mssc=ModuleScreenStyleClass(rsrc);
								memset(GUIstemp,0,1024);
								sprintf(GUIstemp,"OpenRDA %s Image",mssc);
								if(mssc!=NULL) Rfree(mssc);
								hold->addStyleClass(GUIstemp);
								myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							} else if(wdgt->rtype==2)
							{
								rname=Rmalloc(512);
#ifndef RDA_64BITS
								sprintf(rname,"resources/OpenRDA/rda.png");
#else
								sprintf(rname,"resources/OpenRDA/rda64.png");
#endif
								WK = new WLink(rname);
								myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)hBox);
								hold = (WWidget *)myImage;
								myImage->setAlternateText(*temp_xstr);
								if(rname!=NULL) Rfree(rname);
								mssc=ModuleScreenStyleClass(rsrc);
								memset(GUIstemp,0,1024);
								sprintf(GUIstemp,"OpenRDA %s Image",mssc);
								if(mssc!=NULL) Rfree(mssc);
								hold->addStyleClass(GUIstemp);
								myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							} else if(wdgt->rtype==3)
							{
								rname=Rmalloc(512);
								sprintf(rname,"resources/OpenRDA/tuxcloud.png");
								WK = new WLink(rname);
								myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)hBox);
								hold = (WWidget *)myImage;
								myImage->setAlternateText(*temp_xstr);
								if(rname!=NULL) Rfree(rname);
								mssc=ModuleScreenStyleClass(rsrc);
								memset(GUIstemp,0,1024);
								sprintf(GUIstemp,"OpenRDA %s Image",mssc);
								if(mssc!=NULL) Rfree(mssc);
								hold->addStyleClass(GUIstemp);
								myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							} else if(wdgt->rtype==4)
							{
								myText = new Wt::WText(wdgt->XHTML_Label,XHTMLText);
								hold = (Wt::WWidget *)myText;
								mssc=ModuleScreenStyleClass(rsrc);
								memset(GUIstemp,0,1024);
								sprintf(GUIstemp,"OpenRDA %s Label",mssc);
								if(mssc!=NULL) Rfree(mssc);
								hold->addStyleClass(GUIstemp);
								myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							}
#ifdef USE_RDA_DIAGNOSTICS
							if(diagcss)
							{
								if(cssNames!=NULL)
								{
									fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),hold->styleClass().toUTF8().c_str());
								}
							}
#endif /* USE_RDA_DIAGNOSTICS */
							if(Widest!=0)
							{
								wl = Wt::WLength(Widest);
								hold->setMinimumSize(wl,hold->minimumHeight());
							}
							temp_xstr->~WString();
							++col;
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
							if(dashes!=NULL) Rfree(dashes);
						}
					}
					if(tmpstr!=NULL) Rfree(tmpstr);
					tmpstr=NULL;
					break;
				case 10: /* FRAME */
					if(!USER_INTERFACE)
					{
						if(fractbase)
						{
							frame_style=wdgt->rtype;
						}
					}
					break;
				case 11: /* Separator */
					if(!USER_INTERFACE)
					{
						if(fractbase)
						{
							switch(wdgt->rtype)
							{
								default:
								case 0: /* horizontal Single */
									hold=myHorizontalLine((Wt::WContainerWidget *)hBox,1);
									break;
								case 1:
									hold=myHorizontalLine((Wt::WContainerWidget *)hBox,2);
									break;
								case 2:
									hold=myHorizontalLine((Wt::WContainerWidget *)hBox,3);
									break;
								case 3:
									hold=myHorizontalLine((Wt::WContainerWidget *)hBox,4);
									break;
								case 4:
									hold=myVerticalLine((Wt::WContainerWidget *)hBox,1);
									break;
								case 5:
									hold=myVerticalLine((Wt::WContainerWidget *)hBox,2);
									break;
								case 6:
									hold=myVerticalLine((Wt::WContainerWidget *)hBox,3);
									break;
								case 7:
									hold=myVerticalLine((Wt::WContainerWidget *)hBox,4);
									break;
							}
							myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
							++col;
							if(frame_style!=(-1))
							{
								FrameWidget((Wt::WWidget *)hold,frame_style);
								frame_style=(-1);
							}
						}
					}
					break;
				case 12: /* New ScrolledWindow */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						rowsx=crtscrollwindow((Wt::WWidget *)hBox,scn,rsrc,widgetcount,&hold);
						myGrid->addWidget((Wt::WWidget *)hold,row,col,0);
						++col;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)hold,frame_style);
							frame_style=(-1);
						}
						if(rowsx>rows) rows=rowsx;
					}
					break;
				case 13: /* End ScrolledWindow */
					if(USER_INTERFACE || (!USER_INTERFACE 
						&& fractbase))
					{
						prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",*widgetcount,wdgt->type,wdgttypes[wdgt->type],rsrc->module,rsrc->screen);
						hold=NULL;
					}
					break;
				default:
					prterr("Error WWidget *Type [%d] is invalid for WWidget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
					hold=NULL;
					break;
			}
			}
		}
	}
}
int xcrttab(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsrc,int *widgetcount,WWidget **tabw,int line,char *file)
{
	Wt::WTabWidget *Tab=NULL;
	Wt::WText *myText=NULL;
	Wt::WContainerWidget *TabD=NULL;
	Wt::WWidget *line_widget=NULL,*hold_widget=NULL;
	char *mssc=NULL;

#ifdef __USE_ANIMATIONS__
	Wt::WStackedWidget *S=NULL;
	Wt::WAnimation fade(Wt::WAnimation::Fade,Wt::WAnimation::EaseInOut,250);
#endif /* __USE_ANIMATIONS__ */
#ifdef __NEED_WDIALOG_LAYOUT__
	Wt::WVBoxLayout *vb=NULL;
	Wt::WLayout *daL=NULL;
#endif /* __NEED_WDIALOG_LAYOUT__ */

	RDAwdgt *wdgt=NULL,*test=NULL;
	int frame_style=(-1);
	Wt::WString *c=NULL;
	Wt::WImage *myImage=NULL;
	Wt::WLink *WK=NULL;
	Wt::WString *temp_xstr=NULL;
	Wt::WWidget *WW=NULL;
	char *tmpstr=NULL,*dashes=NULL,*rname=NULL;
	int h=0,count=0,rows=0,rowsx=0,rowsr=0,which=0;
	Wt::WLength spc;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crttab Creating Tabbed Containers Screen [%s] [%s] starting with WWidget [%d] at line [%d] program [%s].",scn->module,scn->name,*widgetcount,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	rsrc->has_tabbar=TRUE;
	wdgt=scn->wdgts+(*widgetcount);
	Tab=new Wt::WTabWidget((Wt::WContainerWidget *)parent);
	*tabw=(Wt::WWidget *)Tab;
	WW=(Wt::WWidget *)Tab;
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s TabBar",mssc);
	if(mssc!=NULL) Rfree(mssc);
	WW->addStyleClass(GUIstemp);

#ifdef __USE_ANIMATIONS__
	S=Tab->contentsStack();
	S->setTransitionAnimation(fade);
#endif /* __USE_ANIMATIONS__ */


	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+(*widgetcount);
		if(wdgt->resource_num!=(-1))
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Creating WWidget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
					*widgetcount,wdgttypes[wdgt->type],
					(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
					(wdgt->label!=NULL ? wdgt->label:""),
					wdgt->rows,wdgt->cols,wdgt->rtype); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(wdgt->type==20) /* New Tab Widget */
			{
				rows=0;
				TabD=new Wt::WContainerWidget();
				mssc=ModuleScreenStyleClass(rsrc);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s TabBar TabContainer",mssc);
				if(mssc!=NULL) Rfree(mssc);
				TabD->addStyleClass(GUIstemp);
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsrc->primary!=NULL)
				{
					TabD->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
					TabD->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
				}
#endif /* __NEED_WDIALOG_LAYOUT__ */
				spc=Wt::WLength(0,Wt::WLength::Pixel);
				TabD->setPadding(spc,All);
				c = new Wt::WString(wdgt->label);
				Tab->addTab((Wt::WWidget *)TabD,*c,Wt::WTabWidget::LoadPolicy::PreLoading);
				c->~WString();
				Tab->setTabEnabled(count,TRUE);
				Tab->setTabCloseable(count,FALSE);
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsrc->primary!=NULL)
				{
					vb=new Wt::WVBoxLayout();
					vb->setSpacing(0);
					daL=(Wt::WLayout *)vb;
					daL->setContentsMargins(0,0,0,0);
					TabD->setLayout(vb);
				}
#endif /* __NEED_WDIALOG_LAYOUT__ */
			} else if(wdgt->type==21)
			{
				line_widget =(Wt::WWidget *) new Wt::WText(" ");
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsrc->primary==NULL)
				{
					TabD->addWidget(line_widget);
				} else {
					vb->addWidget((Wt::WWidget *)line_widget,900,Wt::AlignCenter);
				}
#else
				TabD->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
				if(rows>rowsr) 
				{
					rowsr=rows;
					which=count;
				}
				test=scn->wdgts+(*widgetcount+1);
				if(test->type!=20) 
				{
					if(rsrc->has_large_table==TRUE)
					{
						Tab->setCurrentIndex(which);
					}
					return(rowsr);
				} else {
					++count;
				}
			} else if(wdgt->type==1) /* LINE */
			{
				if(numperline(scn,*widgetcount))
				{
					rowsx=crtline((Wt::WWidget *)TabD,scn,rsrc,widgetcount,&line_widget);
					if(rowsx>0)
					{
#ifdef __NEED_WDIALOG_LAYOUT__
						if(rsrc->primary==NULL)
						{
							TabD->addWidget(line_widget);
						} else {
							vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
						}
#else
						TabD->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
						hold_widget=line_widget;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)line_widget,frame_style);
							frame_style=(-1);
						}
						rows+=rowsx;
					}
				}
			} else if(wdgt->type==5) /* LABEL */
			{
				if(wdgt->label!=NULL)
				{
					tmpstr=stralloc(wdgt->label);
				} else {
					tmpstr=stralloc(" ");
				}
				dashes=adddashes(wdgt->label);
				temp_xstr = new WString(wdgt->label,UTF8);
				if(wdgt->rtype==0)
				{
					myText = new Wt::WText(wdgt->label);
					h=getLeadingSpaces(wdgt->label);
					line_widget = (Wt::WWidget *)myText;
					if(h>0)
					{
						spc=Wt::WLength(h,Wt::WLength::FontEm);
						myText->setPadding(spc,Left);
					}
					mssc=ModuleScreenStyleClass(rsrc);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s Label",mssc);
					if(mssc!=NULL) Rfree(mssc);
					line_widget->addStyleClass(GUIstemp);
				} else if(wdgt->rtype==1)
				{
					WK = new WLink(wdgt->pixmap);
					myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TabD);
					line_widget = (WWidget *)myImage;
					myImage->setAlternateText(*temp_xstr);
					mssc=ModuleScreenStyleClass(rsrc);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s Image",mssc);
					if(mssc!=NULL) Rfree(mssc);
					line_widget->addStyleClass(GUIstemp);
				} else if(wdgt->rtype==2)
				{
					rname=Rmalloc(512);
#ifndef RDA_64BITS
					sprintf(rname,"resources/OpenRDA/rda.png");
#else
					sprintf(rname,"resources/OpenRDA/rda64.png");
#endif
					WK = new WLink(rname);
					myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TabD);
					line_widget = (WWidget *)myImage;
					myImage->setAlternateText(*temp_xstr);
					if(rname!=NULL) Rfree(rname);
					mssc=ModuleScreenStyleClass(rsrc);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s Image",mssc);
					if(mssc!=NULL) Rfree(mssc);
					line_widget->addStyleClass(GUIstemp);
				} else if(wdgt->rtype==3)
				{
					rname=Rmalloc(512);
					sprintf(rname,"resources/OpenRDA/tuxcloud.png");
					WK = new WLink(rname);
					myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)TabD);
					line_widget = (WWidget *)myImage;
					myImage->setAlternateText(*temp_xstr);
					if(rname!=NULL) Rfree(rname);
					mssc=ModuleScreenStyleClass(rsrc);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s Image",mssc);
					if(mssc!=NULL) Rfree(mssc);
					line_widget->addStyleClass(GUIstemp);
				} else if(wdgt->rtype==4)
				{
					myText = new Wt::WText(wdgt->XHTML_Label);
					line_widget = (Wt::WWidget *)myText;
					mssc=ModuleScreenStyleClass(rsrc);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s Label",mssc);
					if(mssc!=NULL) Rfree(mssc);
					line_widget->addStyleClass(GUIstemp);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagcss)
				{
					if(cssNames!=NULL)
					{
						fprintf(cssNames,"\"%s\",\"%s\"\r\n","",line_widget->styleClass().toUTF8().c_str());
					}
				}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsrc->primary==NULL)
				{
					TabD->addWidget(line_widget);
				} else {
					vb->addWidget((Wt::WWidget *)line_widget,0,Wt::AlignLeft | Wt::AlignJustify);
				}
#else
				TabD->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
				if(dashes!=NULL) Rfree(dashes);
				temp_xstr->~WString();
				if(tmpstr!=NULL) Rfree(tmpstr);
				tmpstr=NULL;
				if(frame_style!=(-1))
				{
					FrameWidget((Wt::WWidget *)line_widget,frame_style);
					frame_style=(-1);
				}
				++rows;
			} else if(wdgt->type==10) /* FRAME */
			{
				frame_style=wdgt->rtype;
			} else if(wdgt->type==3) /* START BOX */
			{
				rowsx=crtbox((Wt::WWidget *)TabD,scn,rsrc,widgetcount,&line_widget);
				if(rowsx>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
					if(rsrc->primary==NULL)
					{
						TabD->addWidget(line_widget);
					} else {
						vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
					}
#else
					TabD->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)hold_widget,frame_style);
						frame_style=(-1);
					}
					rows+=rowsx;
				}
			} else if(wdgt->type==12)
			{
				rowsx=crtscrollwindow((Wt::WWidget *)TabD,scn,rsrc,widgetcount,&line_widget);
				if(rowsx>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
					if(rsrc->primary==NULL)
					{
						TabD->addWidget(line_widget);
					} else {
						vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
					}
#else
					TabD->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					rows+=rowsx;
				}
			} else if(wdgt->type==28)
			{
				rowsx=crttablecontainer((Wt::WWidget *)TabD,scn,rsrc,widgetcount,&line_widget,wdgt->rtype);
				if(rowsx>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
					if(rsrc->primary==NULL)
					{
						TabD->addWidget(line_widget);
					} else {
						vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
					}
#else
					TabD->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					rows+=rowsx;
				}
			} else {
				prterr("Error WWidget *Type [%d] is invalid for WWidget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
			}
		}
	}
}
int xcrtscrollwindow(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsrc,
	int *widgetcount,Wt::WWidget **swindow,int line,char *file)
{
	Wt::WWidget *line_widget=NULL,*hold_widget=NULL;
	RDAwdgt *wdgt=NULL;
	int line_count=0,w=0,h=0;
	Wt::WScrollArea *SA=NULL;
	int frame_style=(-1);
	Wt::WVBoxLayout *vb=NULL;
	Wt::WHBoxLayout *vb1=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WImage *myImage=NULL;
	Wt::WLink *WK=NULL;
	Wt::WString *temp_xstr=NULL;
	Wt::WContainerWidget *Vb=NULL,*VB=NULL;
	Wt::WLength H,L;
	char *tmpstr=NULL,*dashes=NULL,*rname=NULL;
	int rows=0,rowsx=0;
	Wt::WLength spc,mw,ml;
	Wt::WText *myText=NULL;
	char *mssc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG crtscrollwindow Creating Scrolled Window on Screen [%s] [%s] starting with WWidget [%d] at line [%d] program [%s].",scn->module,scn->name,*widgetcount,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	VB=new Wt::WContainerWidget((Wt::WContainerWidget *)parent);
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s ScrolledWindow ExternalContainer",mssc);
	if(mssc!=NULL) Rfree(mssc);
	VB->addStyleClass(GUIstemp);
/*
	VB->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
	VB->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
*/
	wdgt=scn->wdgts+(*widgetcount);
	if(wdgt->cols!=0 && wdgt->rows!=0)
	{
		L=Wt::WLength(wdgt->cols);
		H=Wt::WLength(wdgt->rows);
		VB->setMaximumSize(L,H);
		VB->setMinimumSize(L,H);
	}
	vb1=new Wt::WHBoxLayout();
	VB->setLayout(vb1);
	*swindow=(Wt::WWidget *)VB;
	vb1->setSpacing(0);
	daL=(Wt::WLayout *)vb1;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	VB->setPadding(spc,All);
	SA=new Wt::WScrollArea((Wt::WContainerWidget *)VB);
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s ScrolledWindow ScrolledArea",mssc);
	if(mssc!=NULL) Rfree(mssc);
	SA->addStyleClass(GUIstemp);
	SA->setHorizontalScrollBarPolicy(Wt::WScrollArea::ScrollBarAsNeeded);
	SA->setVerticalScrollBarPolicy(Wt::WScrollArea::ScrollBarAlwaysOn);
	Vb=new Wt::WContainerWidget();
	mssc=ModuleScreenStyleClass(rsrc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s ScrolledWindow InternalContainer",mssc);
	if(mssc!=NULL) Rfree(mssc);
	Vb->addStyleClass(GUIstemp);
	SA->setWidget((Wt::WWidget *)Vb);
	vb1->addWidget(SA);
	vb=new Wt::WVBoxLayout();
	Vb->setLayout(vb);
	daL=(Wt::WLayout *)vb;
	daL->setContentsMargins(0,0,0,0);

	while((*widgetcount+1)<scn->numwdgts)
	{
		++(*widgetcount);
		wdgt=scn->wdgts+(*widgetcount);
		if(wdgt->resource_num!=(-1))
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Creating WWidget [%d] Type [%s] Name [%s] Label [%s] Rows [%d] Cols [%d] Rtype [%d]",
					*widgetcount,wdgttypes[wdgt->type],
					(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
					(wdgt->label!=NULL ? wdgt->label:""),
					wdgt->rows,wdgt->cols,wdgt->rtype); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(wdgt->type==1) /* LINE */
			{
				if(numperline(scn,*widgetcount))
				{
					rowsx=crtline((Wt::WWidget *)Vb,scn,rsrc,widgetcount,&line_widget);
					if(rowsx>0)
					{
						vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
						++line_count;
						hold_widget=line_widget;
						if(frame_style!=(-1))
						{
							FrameWidget((Wt::WWidget *)line_widget,frame_style);
							frame_style=(-1);
						}
						rows+=rowsx;
					}
				}
			} else if(wdgt->type==5) /* LABEL */
			{
				if(!USER_INTERFACE)
				{
					if(wdgt->label!=NULL)
					{
						tmpstr=stralloc(wdgt->label);
					} else {
						tmpstr=stralloc(" ");
					}
					dashes=adddashes(wdgt->label);
					temp_xstr = new WString(wdgt->label,UTF8);
					if(wdgt->rtype==0)
					{
						myText = new Wt::WText(wdgt->label);
						h=getLeadingSpaces(wdgt->label);
						line_widget = (Wt::WWidget *)myText;
						if(h>0)
						{
							spc=Wt::WLength(h,Wt::WLength::FontEm);
							myText->setPadding(spc,Left);
						}
						mssc=ModuleScreenStyleClass(rsrc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Label",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==1)
					{
						WK = new WLink(wdgt->pixmap);
						myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)Vb);
						line_widget = (WWidget *)myImage;
						myImage->setAlternateText(*temp_xstr);
						mssc=ModuleScreenStyleClass(rsrc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Image",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==2)
					{
						rname=Rmalloc(512);
#ifndef RDA_64BITS
						sprintf(rname,"resources/OpenRDA/rda.png");
#else
						sprintf(rname,"resources/OpenRDA/rda64.png");
#endif
						WK = new WLink(rname);
						myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)Vb);
						line_widget = (WWidget *)myImage;
						myImage->setAlternateText(*temp_xstr);
						if(rname!=NULL) Rfree(rname);
						mssc=ModuleScreenStyleClass(rsrc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Image",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==3)
					{
						rname=Rmalloc(512);
						sprintf(rname,"resources/OpenRDA/tuxcloud.png");
						WK = new WLink(rname);
						myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)Vb);
						line_widget = (WWidget *)myImage;
						myImage->setAlternateText(*temp_xstr);
						if(rname!=NULL) Rfree(rname);
						mssc=ModuleScreenStyleClass(rsrc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Image",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==4)
					{
						myText = new Wt::WText(wdgt->XHTML_Label);
						line_widget = (Wt::WWidget *)myText;
						mssc=ModuleScreenStyleClass(rsrc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Label",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diagcss)
					{
						if(cssNames!=NULL)
						{
							fprintf(cssNames,"\"%s\",\"%s\"\r\n","",line_widget->styleClass().toUTF8().c_str());
						}
					}
#endif /* USE_RDA_DIAGNOSTICS */
					vb->addWidget((Wt::WWidget *)line_widget,0,Wt::AlignLeft | Wt::AlignJustify);
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					if(dashes!=NULL) Rfree(dashes);
					temp_xstr->~WString();
					if(tmpstr!=NULL) Rfree(tmpstr);
					tmpstr=NULL;
					++rows;
				}
			} else if(wdgt->type==10) /* FRAME */
			{
				if(!USER_INTERFACE)
				{
					frame_style=wdgt->rtype;
				}
				++line_count;
			} else if(wdgt->type==28) /* Table Container */
			{
				rowsx=crttablecontainer((Wt::WWidget *)Vb,scn,rsrc,widgetcount,&line_widget,wdgt->rtype);
				if(rowsx>0)
				{
					vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
					++line_count;
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					rows+=rowsx;
				}
			} else if(wdgt->type==23) /* NEW TAB BAR */
			{
				rowsx=crttab((Wt::WWidget *)Vb,scn,rsrc,widgetcount,&line_widget);
				if(rowsx>0)
				{
					vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
					++line_count;
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					rows+=rowsx;
				}
			} else if(wdgt->type==3) /* START BOX */
			{
				rowsx=crtbox((Wt::WWidget *)Vb,scn,rsrc,widgetcount,&line_widget);
				if(rowsx>0)
				{
					vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
					++line_count;
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					rows+=rowsx;
				}
			} else if(wdgt->type==12)
			{
				rowsx=crtscrollwindow((Wt::WWidget *)Vb,scn,rsrc,widgetcount,&line_widget);
				if(rowsx>0)
				{
					vb->addWidget((Wt::WWidget *)line_widget,(rowsx>0 ? rowsx-1:0),Wt::AlignLeft | Wt::AlignJustify);
					++line_count;
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					rows+=rowsx;
				}
			} else if(wdgt->type==13) /* END SCROLLED WINDOW */
			{
				break;
			} else {
				prterr("Error WWidget *Type [%d] is invalid for WWidget [%d] on Screen [%s] [%s]",wdgt->type,*widgetcount,scn->module,scn->name);
			}
		}
	}
	if(!USER_INTERFACE)
	{
/* show scrolled window contents */
	}
}
/*---------------------------------------------------------------------------
	crtwdgts - function to create widgets inside a window
---------------------------------------------------------------------------*/
#ifdef __USE_MAKESCREENHEADER__
#ifdef __NEED_WDIALOG_LAYOUT__
static void MakeScreenHeader(Wt::WContainerWidget *parent,Wt::WVBoxLayout *vb,char *label,RDArsrc *rsc)
#else
static void MakeScreenHeader(Wt::WContainerWidget *parent,char *label)
#endif /* __NEED_WDIALOG_LAYOUT__ */
{
	Wt::WContainerWidget *c=NULL;
	Wt::WText *t=NULL;
	Wt::WHBoxLayout *h=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WLength spc;
	char *mssc=NULL;

	c=new Wt::WContainerWidget();
#ifdef __NEED_WDIALOG_LAYOUT__
	if(rsc->primary==NULL)
	{
		parent->addWidget(c);
		t =(Wt::WWidget *) new Wt::WText(label);
		mssc=ModuleScreenStyleClass(rsc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s h4",mssc);
		if(mssc!=NULL) Rfree(mssc);
		t->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagcss)
		{
			if(cssNames!=NULL)
			{
				fprintf(cssNames,"\"%s\",\"%s\"\r\n",label,t->styleClass().toUTF8().c_str());
			}
		}
#endif /* USE_RDA_DIAGNOSTICS */
		h=new Wt::WHBoxLayout();
		h->setSpacing(0);
		daL=(Wt::WLayout *)h;
		daL->setContentsMargins(0,0,0,0);
		spc=Wt::WLength(0,Wt::WLength::Pixel);
		c->setPadding(spc,All);
		c->setLayout(h);	
		t =(Wt::WWidget *) new Wt::WText(label);
		mssc=ModuleScreenStyleClass(rsc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s h4",mssc);
		if(mssc!=NULL) Rfree(mssc);
		t->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagcss)
		{
			if(cssNames!=NULL)
			{
				fprintf(cssNames,"\"%s\",\"%s\"\r\n",label,t->styleClass().toUTF8().c_str());
			}
		}
#endif /* USE_RDA_DIAGNOSTICS */
		h->addWidget(t,100,Wt::AlignCenter);
	} else {
		c->setOverflow(WContainerWidget::OverflowVisible,Vertical);
		c->setOverflow(WContainerWidget::OverflowVisible,Horizontal);
		vb->addWidget((Wt::WWidget *)c,0,Wt::AlignCenter | Wt::AlignJustify);
		h=new Wt::WHBoxLayout();
		h->setSpacing(0);
		daL=(Wt::WLayout *)h;
		daL->setContentsMargins(0,0,0,0);
		spc=Wt::WLength(0,Wt::WLength::Pixel);
		c->setPadding(spc,All);
		c->setLayout(h);	
		t =(Wt::WWidget *) new Wt::WText(label);
		mssc=ModuleScreenStyleClass(rsc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s h4",mssc);
		if(mssc!=NULL) Rfree(mssc);
		t->addStyleClass(GUIstemp);
		h->addWidget(t,100,Wt::AlignCenter);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagcss)
		{
			if(cssNames!=NULL)
			{
				fprintf(cssNames,"\"%s\",\"%s\"\r\n",label,t->styleClass().toUTF8().c_str());
			}
		}
#endif /* USE_RDA_DIAGNOSTICS */
	}
#else 
	parent->addWidget(c);
	t =(Wt::WWidget *) new Wt::WText(label);
	mssc=ModuleScreenStyleClass(rsc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s h4",mssc);
	if(mssc!=NULL) Rfree(mssc);
	t->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n",label,t->styleClass().toUTF8().c_str());
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	h=new Wt::WHBoxLayout();
	h->setSpacing(0);
	daL=(Wt::WLayout *)h;
	daL->setContentsMargins(0,0,0,0);
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	c->setPadding(spc,All);
	c->setLayout(h);	
	t =(Wt::WWidget *) new Wt::WText(label);
	mssc=ModuleScreenStyleClass(rsc);
	memset(GUIstemp,0,1024);
	sprintf(GUIstemp,"OpenRDA %s h4",mssc);
	if(mssc!=NULL) Rfree(mssc);
	t->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n",label,t->styleClass().toUTF8().c_str());
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	h->addWidget(t,100,Wt::AlignCenter);
#endif /* __NEED_WDIALOG_LAYOUT__ */
}
#endif /* __USE_MAKESCREENHEADER__ */
void crtwdgts(Wt::WWidget *parent,RDAscrn *scn,RDArsrc *rsc,char *label) 
{
	Wt::WWidget *hold_widget=NULL,*line_widget=NULL;
	Wt::WString *temp_xstr=NULL;
	int frame_style=(-1);
	Wt::WContainerWidget *CW=NULL;
	Wt::WImage *myImage=NULL;
	Wt::WLink *WK=NULL;
	int line_count=0,h=0;
	int widgetcount=0,rowz=0;
	RDAwdgt *wdgt=NULL;
	char *tmpstr=NULL,*dashes=NULL,eat_end_table=FALSE,*rname=NULL;
	char *mssc=NULL;
	short last_wdgttype=(-1);
	Wt::WLength spc;
	Wt::WText *myText=NULL;
#ifdef __NEED_WDIALOG_LAYOUT__
	Wt::WVBoxLayout *vb=NULL;
	Wt::WLayout *daL=NULL;
#endif /* __NEED_WDIALOG_LAYOUT__ */

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) { prterr("DIAG Creating Custom Window Widgets for Screen [%s] [%s]",rsc->module,rsc->screen); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(scn!=NULL)
	{
	if(!USER_INTERFACE)
	{
		CW=(Wt::WContainerWidget *)parent;
		mssc=ModuleScreenStyleClass(rsc);
		memset(GUIstemp,0,1024);
		sprintf(GUIstemp,"OpenRDA %s",mssc);
		if(mssc!=NULL) Rfree(mssc);
		CW->addStyleClass(GUIstemp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagcss)
		{
			if(cssNames!=NULL) 
			{
				fprintf(cssNames,"\"%s\",\"%s\"\r\n","",CW->styleClass().toUTF8().c_str());
			}
		}
#endif /* USE_RDA_DIAGNOSTICS */
		spc=Wt::WLength(0,Wt::WLength::Pixel);
		CW->setPadding(spc,All);
#ifdef __NEED_WDIALOG_LAYOUT__
		if(rsc->primary!=NULL)
		{
			CW->setOverflow(Wt::WContainerWidget::OverflowVisible,Vertical);
			CW->setOverflow(Wt::WContainerWidget::OverflowVisible,Horizontal);
			vb=new Wt::WVBoxLayout();
			vb->setSpacing(0);
			daL=(Wt::WLayout *)vb;
			daL->setContentsMargins(0,0,0,0);
			CW->setLayout(vb);
		}	
#endif /* __NEED_WDIALOG_LAYOUT__ */
	}
	if(rsc->primary==NULL && QN2IT_PARENT_TYPE==0)
	{
#ifdef __USE_MAKESCREENHEADER__
#ifdef __NEED_WDIALOG_LAYOUT__
		MakeScreenHeader(CW,vb,label,rsc);
#else
		MakeScreenHeader(CW,label);
#endif /* __NEED_WDIALOG_LAYOUT__ */
#endif /* __USE_MAKESCREENHEADER__ */
	}
	widgetcount=0;
	while(widgetcount<scn->numwdgts)
	{
		wdgt=scn->wdgts+widgetcount;
		if(wdgt->resource_num!=(-1))
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG creating widget [%d] type [%s] name [%s] label [%s] rows [%d] cols [%d] rtype [%d]",
					widgetcount,wdgttypes[wdgt->type],
					(wdgt->resource_name!=NULL ? wdgt->resource_name:""),
					(wdgt->label!=NULL ? wdgt->label:""),
					wdgt->rows,wdgt->cols,wdgt->rtype); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(eat_end_table==TRUE && wdgt->type!=29) eat_end_table=FALSE;
			if(wdgt->type==1) /* NEW LINE */
			{
				if(numperline(scn,widgetcount))
				{
					rowz=crtline((Wt::WWidget *)CW,scn,rsc,&widgetcount,&line_widget);
					if(rowz>0)
					{
					++line_count;
#ifdef __NEED_WDIALOG_LAYOUT__
					if(rsc->primary==NULL)
					{
						CW->addWidget(line_widget);
					} else {
						vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
					}
#else 
					CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
					hold_widget=line_widget;
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					}
				} else {
					prterr("Error No Widgets on line# [%d] of Screen [%s][%s].",widgetcount,rsc->module,rsc->screen);
				}
			} else if(wdgt->type==5) /* LABEL */
			{
				if(!USER_INTERFACE)
				{
					if(wdgt->label!=NULL)
					{
						tmpstr=stralloc(wdgt->label);
					} else {
						tmpstr=stralloc(" ");
					}
					dashes=adddashes(wdgt->label);
					temp_xstr = new WString(wdgt->label,UTF8);
					if(wdgt->rtype==0)
					{
						myText = new Wt::WText(wdgt->label);
						h=getLeadingSpaces(wdgt->label);
						line_widget = (Wt::WWidget *)myText;
						if(h>0)
						{
							spc=Wt::WLength(h,Wt::WLength::FontEm);
							myText->setPadding(spc,Left);
						}
						mssc=ModuleScreenStyleClass(rsc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Label",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==1)
					{
						WK = new WLink(wdgt->pixmap);
						myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)CW);
						line_widget = (WWidget *)myImage;
						myImage->setAlternateText(*temp_xstr);
						mssc=ModuleScreenStyleClass(rsc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Image",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==2)
					{
						rname=Rmalloc(512);
#ifndef RDA_64BITS
						sprintf(rname,"resources/OpenRDA/rda.png");
#else
						sprintf(rname,"resources/OpenRDA/rda64.png");
#endif
						WK = new WLink(rname);
						myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)CW);
						line_widget = (WWidget *)myImage;
						myImage->setAlternateText(*temp_xstr);
						if(rname!=NULL) Rfree(rname);
						mssc=ModuleScreenStyleClass(rsc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Image",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==3)
					{
						rname=Rmalloc(512);
						sprintf(rname,"resources/OpenRDA/tuxcloud.png");
						WK = new WLink(rname);
						myImage  = new  Wt::WImage(*WK,(Wt::WContainerWidget *)CW);
						line_widget = (WWidget *)myImage;
						myImage->setAlternateText(*temp_xstr);
						if(rname!=NULL) Rfree(rname);
						mssc=ModuleScreenStyleClass(rsc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Image",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					} else if(wdgt->rtype==4)
					{
						myText = new Wt::WText(wdgt->XHTML_Label);
						line_widget = (Wt::WWidget *)myText;
						mssc=ModuleScreenStyleClass(rsc);
						memset(GUIstemp,0,1024);
						sprintf(GUIstemp,"OpenRDA %s Label",mssc);
						if(mssc!=NULL) Rfree(mssc);
						line_widget->addStyleClass(GUIstemp);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diagcss)
					{
						if(cssNames!=NULL) 
						{
							fprintf(cssNames,"\"%s\",\"%s\"\r\n",(wdgt->label!=NULL ? wdgt->label:""),line_widget->styleClass().toUTF8().c_str());
						}
					}			
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef __NEED_WDIALOG_LAYOUT__
					if(rsc->primary==NULL)
					{
						CW->addWidget(line_widget);
					} else {
						vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
					}
#else 
					CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
					rowz=1;
					if(dashes!=NULL) Rfree(dashes);
					temp_xstr->~WString();
					if(tmpstr!=NULL) Rfree(tmpstr);
					tmpstr=NULL;
				}
			} else if(wdgt->type==10) /* FRAME */
			{
				if(!USER_INTERFACE)
				{
					frame_style=wdgt->rtype;
				}
				++line_count;
			} else if(wdgt->type==26) /* Tool Bar */
			{
				rowz=crttoolbarcontainer((Wt::WWidget *)CW,scn,rsc,&widgetcount,&line_widget);
				if(last_wdgttype==(-1))
				{
#ifdef __NEED_WDIALOG_LAYOUT__
					if(rsc->primary==NULL)
					{
						CW->addWidget(line_widget);
					} else {
						vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
					}
#else 
					CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
					if(frame_style!=(-1))
					{
						FrameWidget((Wt::WWidget *)line_widget,frame_style);
						frame_style=(-1);
					}
				}
				last_wdgttype=wdgt->type;
				hold_widget=NULL;
				line_widget=NULL;
			} else if((wdgt->type==28) && !RDAstrcmp(wdgt->label,"CUSTOM_TABLE_FUNCTION") && CustomTableFunction!=NULL)
			{
				eat_end_table=TRUE;
				rowz=CustomTableFunction((Wt::WWidget *)CW,&line_widget);
				if(rowz>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsc->primary==NULL)
				{
					CW->addWidget(line_widget);
				} else {
					vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
				}
#else 
				CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
				++line_count;
				hold_widget=line_widget;
				if(frame_style!=(-1))
				{
					FrameWidget((Wt::WWidget *)line_widget,frame_style);
					frame_style=(-1);
				}
				}
			} else if(wdgt->type==29 && eat_end_table==TRUE)
			{
				eat_end_table=FALSE;
			} else if(wdgt->type==28) /* Table Container */
			{
				rowz=crttablecontainer((Wt::WWidget *)CW,scn,rsc,&widgetcount,&line_widget,wdgt->rtype);
				if(rowz>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsc->primary==NULL)
				{
					CW->addWidget(line_widget);
				} else {
					vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
				}
#else 
				CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
				++line_count;
				hold_widget=line_widget;
				if(frame_style!=(-1))
				{
					FrameWidget((Wt::WWidget *)line_widget,frame_style);
					frame_style=(-1);
				}
				}
			} else if(wdgt->type==30) /* Header Container */
			{
			} else if(wdgt->type==31) /* End Header Container */
			{
			} else if(wdgt->type==23) /* NEW TAB BAR */
			{
				rowz=crttab((Wt::WWidget *)CW,scn,rsc,&widgetcount,&line_widget);
				if(rowz>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsc->primary==NULL)
				{
					CW->addWidget(line_widget);
				} else {
					vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
				}
#else 
				CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
				++line_count;
				hold_widget=line_widget;
				if(frame_style!=(-1))
				{
					FrameWidget((Wt::WWidget *)line_widget,frame_style);
					frame_style=(-1);
				}
				}
			} else if(wdgt->type==3) /* NEW BOX */
			{
				rowz=crtbox((Wt::WWidget *)CW,scn,rsc,&widgetcount,&line_widget);
				if(rowz>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsc->primary==NULL)
				{
					CW->addWidget(line_widget);
				} else {
					vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
				}
#else 
				CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
				++line_count;
				hold_widget=line_widget;
				if(frame_style!=(-1))
				{
					FrameWidget((Wt::WWidget *)line_widget,frame_style);
					frame_style=(-1);
				}
				}
			} else if(wdgt->type==12) /* NEW SCROLLED WINDOW */
			{
				rowz=crtscrollwindow((Wt::WWidget *)CW,scn,rsc,&widgetcount,&line_widget);
				if(rowz>0)
				{
#ifdef __NEED_WDIALOG_LAYOUT__
				if(rsc->primary==NULL)
				{
					CW->addWidget(line_widget);
				} else {
					vb->addWidget((Wt::WWidget *)line_widget,(rowz>0 ? rowz-1:0),Wt::AlignLeft | Wt::AlignJustify);
				}
#else 
				CW->addWidget(line_widget);
#endif /* __NEED_WDIALOG_LAYOUT__ */
				++line_count;
				if(frame_style!=(-1))
				{
					FrameWidget((Wt::WWidget *)line_widget,frame_style);
					frame_style=(-1);
				}
				}
			} else { /* ERROR */
				prterr("Error WWidget [%d] Type [%d] [%s] on Screen [%s] [%s] is out of sequence.",widgetcount,wdgt->type,wdgttypes[wdgt->type],rsc->module,rsc->screen);
			}
		}
		if(diaggui)
		{
			prterr("rowz [%d] for WidgetCount [%d] ",rowz,widgetcount);
		}
		++widgetcount;
	}
	}
}
