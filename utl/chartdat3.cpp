/* chartdat.c - Chart Database Columns */
/*lint -library */
#ifndef WIN32
#define __NAM__ "chartdat3.lnx"
#endif
#ifdef WIN32
#define __NAM__ "chartdat3.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <guip.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


enum chartTypes {PIE,VERTICAL_BAR,HORIZONTAL_BAR};
typedef enum chartTypes chartType;
struct RDAelements
{
	char valid;
	double value;
	qt_QColor *valueColor;
	int valuePattern;
	char *label;
	qt_QColor *labelColor;
	double prox;
	double proy;
	double scales;
};
typedef struct RDAelements RDAelement;

struct RDAcharts
{
	char *name;
	char *description;
	double total;
	chartType type;
	int num;
	RDAelement *elements;
};
typedef struct RDAcharts RDAchart;
static RDAchart chart;
static void DisplayChart(RDAchart *);

short addRDAelement(RDAchart *c,double value,char *label)
{
	RDAelement *e=NULL;
	int x=0;

	if(c->elements!=NULL)
	{
		for(x=0,e=c->elements;x<c->num;++x,++e)
		{
			if(!RDAstrcmp(e->label,label)) break;
		}
	if(x<c->num)
		{
			e->value+=value;
		} else {
			c->elements=Rrealloc(c->elements,(c->num+1)*sizeof(RDAelement));
			e=c->elements+c->num;
			e->value=value;
			e->label=stralloc(label);
			e->valid=TRUE;
			e->valueColor=NULL;
			e->valuePattern=1;
			e->labelColor=NULL;
			e->prox=-1;
			e->proy=-1;
			e->scales=0.0;
			++c->num;
		}
	} else {
		c->elements=Rmalloc(sizeof(RDAelement));
		e=c->elements;
		e->value=value;
		e->label=stralloc(label);
		e->valid=TRUE;
		e->valueColor=NULL;
		e->valuePattern=1;
		e->labelColor=NULL;
		e->prox=-1;
		e->proy=-1;
		e->scales=0.0;
		c->num=1;
	}
	return(c->num-1);
}
static void SeeChartData(RDAchart *c)
{
	RDAelement *e=NULL;
	int x=0;

	if(c!=NULL)
	{
		prterr("Chart [%s] ",(c->name!=NULL ? c->name:""));TRACE;
		prterr("Number Elements [%d] ",c->num);TRACE;
		for(x=0,e=c->elements;x<c->num;++x,++e)
		{
			prterr("\tElement [%s] Value [%.09f] ",e->label,e->value);TRACE;
		}
	}
}

int main(int argc,char **argv)
{
	short fileno=(-1),ef=(-1),DID_STUFF=FALSE;
	double v=0.0;
	NRDfield *labelField=NULL,*valueField=NULL;
	char delflag=FALSE,*temp=NULL;
	char myTemp[512];

	if(argc<5) std::exit;
	if(InitializeSubsystems(argc,argv,"UTILITIES","CHART DATABASE"))
	{
		RDAAPPMAINLOOP();
		return;
	}
	chart.total=0;
	chart.name=NULL;
	chart.description=NULL;
	chart.num=0;
/*
	chart.type=HORIZONTAL_BAR;
	chart.type=PIE;
*/
	chart.type=VERTICAL_BAR;
	chart.elements=NULL;
	fileno=OPNNRD(argv[1],argv[2]);
	if(fileno==(-1)) 
	{
		memset(myTemp,0,512);
		sprintf(myTemp,"Error cannot open file [%s] [%s] for reading.",argv[1],argv[2]);
		ERRORDIALOG("ERROR FILE OPEN",myTemp,NULL,TRUE);
		ShutdownSubsystems();
		return;
	} else {
		labelField=FLDNRD(fileno,argv[3]);
		valueField=FLDNRD(fileno,argv[4]);
		if(labelField==NULL)
		{
			memset(myTemp,0,512);
			sprintf(myTemp,"Error label field [%s]  not available in [%s] [%s].",argv[3],argv[1],argv[2]);
			ERRORDIALOG("ERROR LABEL FIELD UNAVAILABLE",myTemp,NULL,TRUE);
			CLSNRD(fileno);
			ShutdownSubsystems();
			return;
		}
		if(valueField==NULL)
		{
			memset(myTemp,0,512);
			sprintf(myTemp,"Error value field [%s]  not available in [%s] [%s].",argv[4],argv[1],argv[2]);
			ERRORDIALOG("ERROR VALUE FIELD UNAVAILABLE",myTemp,NULL,TRUE);
			CLSNRD(fileno);
			ShutdownSubsystems();
			return;
		} else {
			switch(valueField->type)
			{
				default:
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CHARACTERS:
					memset(myTemp,0,512);
					sprintf(myTemp,"Error value field [%s]  not appropriate type to accumulate in [%s] [%s].",argv[4],argv[1],argv[2]);
					ERRORDIALOG("ERROR VALUE FIELD INAPPROPRIATE TYPE",myTemp,NULL,TRUE);
					CLSNRD(fileno);
					ShutdownSubsystems();
					return;
				case BOOLNS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
				case SHORTV:
				case SSHORTV:
				case LONGV:
				case SLONGV:
					break;
			}
		}
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				labelField=FLDNRD(fileno,argv[3]);	
				valueField=FLDNRD(fileno,argv[4]);
				switch(valueField->type)
				{
					default:
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case CHARACTERS:
						break;
					case BOOLNS:
						v=*valueField->data.string_value;
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						v=*valueField->data.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						v=*valueField->data.short_value;
						break;
					case LONGV:
					case SLONGV:
						v=*valueField->data.integer_value;
						break;
				}
				switch(labelField->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						addRDAelement(&chart,v,labelField->data.string_value);
						break;
					case BOOLNS:
						if(*labelField->data.string_value==TRUE)
						{
							memset(stemp,0,101);
							sprintf(stemp,"IS %s",labelField->name);
						} else {
							memset(stemp,0,101);
							sprintf(stemp,"NOT %s",labelField->name);
						}
						addRDAelement(&chart,v,stemp);
						break;
					case CHARACTERS:
						memset(stemp,0,101);
						sprintf(stemp,"%c",*labelField->data.string_value);
						addRDAelement(&chart,v,stemp);
						break;
					case DOUBLEV:
					case SDOUBLEV:
						memset(stemp,0,101);
						sprintf(stemp,"%.0f",*labelField->data.float_value);
						addRDAelement(&chart,v,stemp);
						break;
					case DOLLARS:
						temp=famt(*labelField->data.float_value,labelField->len);
						addRDAelement(&chart,v,temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case DOLLARS_NOCENTS:
						temp=famtncents((*labelField->data.float_value*100),labelField->len,' ');
						addRDAelement(&chart,v,temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
					case SDECIMALV:
						memset(stemp,0,101);
						sprintf(stemp,"%.09f",*labelField->data.float_value);
						addRDAelement(&chart,v,stemp);
						break;
					case SHORTV:
					case SSHORTV:
						memset(stemp,0,101);
						sprintf(stemp,"%4d",*labelField->data.short_value);
						addRDAelement(&chart,v,stemp);
						break;
					case LONGV:
					case SLONGV:
						memset(stemp,0,101);
						sprintf(stemp,"%d",*labelField->data.integer_value);
						addRDAelement(&chart,v,stemp);
						break;
					default:
						break;
				}
				++DID_STUFF;
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
	}
	if(!DID_STUFF)
	{
		ERRORDIALOG("ERROR NO DATA TO CHART","Error no data was collected to chart.",NULL,TRUE);
		ShutdownSubsystems();
		return;
	} else if(diagapps)
	{
		SeeChartData(&chart);
	}
	DisplayChart(&chart);
}
void DrawPieChart(qt_QCanvas *Canvas,RDAchart *chart,double total,int count)
{
	double width=0.0;
	double height=0.0;
	int size=0,extent=0;
	int x=0,y=0,angle=0,i=0;
	RDAelement *e=NULL;
	qt_QCanvasEllipse *arc=NULL;
	qt_QBrush *brush=NULL;
	qt_QString *label=NULL;
	qt_QCanvasText *text=NULL;
	qt_QRect *rect=NULL;
	qt_QFont *f=NULL;
	double prox=0.0,proy=0.0;
	char temp[512];

	label=qt_new_QString5(RDAFontNAME);
	f=qt_new_QFont1(label,6,60,0);
	qt_del_QString(label);
	width=qt_QCanvas_width((qt_QCanvas *)Canvas);
	height=qt_QCanvas_height((qt_QCanvas *)Canvas);
	prterr("height [%f] width [%f] size [%d] x [%d] y [%d] ",height,width,size,x,y);TRACE;
	if(width>height) size=height;
		else size=width;
	x=width/2;
	y=height/2;
	prterr("height [%f] width [%f] size [%d] x [%d] y [%d] ",height,width,size,x,y);TRACE;
	angle=0;
	for(i=0,e=chart->elements;i<chart->num;++i,++e)
	{
		if(e->valid)
		{

			extent=e->scales;
			prterr("e->label [%s] e->value [%f] e->scales [%f] extent [%d] angle [%d] ",(e->label!=NULL ? e->label:""),e->value,e->scales,extent,angle);TRACE;
			arc=qt_new_QCanvasEllipse2(size,size,angle,extent,(qt_QCanvas *)Canvas);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)arc,TRUE);
			prterr("e->label [%s] height [%f] width [%f] size [%d] x [%d] y [%d] ",e->label,height,width,size,x,y);TRACE;
			qt_QCanvasItem_setX((qt_QCanvasItem *)arc,x);
			qt_QCanvasItem_setY((qt_QCanvasItem *)arc,y);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)arc,0);
			brush=qt_new_QBrush(e->valueColor,e->valuePattern);
			qt_QCanvasPolygonalItem_setBrush((qt_QCanvasPolygonalItem *)arc,brush);
			qt_QCanvasItem_show((qt_QCanvasItem *)arc);
			qt_del_QBrush(brush);
			angle+=extent;
			memset(temp,0,512);
			sprintf(temp,"%s %.02f %2.1f",(e->label!=NULL ? e->label:""),e->value,(e->value/total)*100);
			label=qt_new_QString5(temp);
			text=qt_new_QCanvasText2((qt_QString *)label,(qt_QFont *)f,(qt_QCanvas *)Canvas);
			qt_QCanvasItem_setCanvas((qt_QCanvasItem *)text,(qt_QCanvas *)Canvas);
			rect=qt_QWidget_rect((qt_QWidget *)arc);
			prox=(qt_QRect_width((qt_QRect *)rect)/2)+qt_QRect_x((qt_QRect *)rect);
			proy=(qt_QRect_height((qt_QRect *)rect)/2)+qt_QRect_y((qt_QRect *)rect);
			prterr("e->label [%s] prox [%f] proy [%f] ",e->label,prox,proy);TRACE;
			rect=qt_QWidget_rect((qt_QWidget *)text);
			prox-=(qt_QRect_width((qt_QRect *)rect)/2);
			proy-=(qt_QRect_height((qt_QRect *)rect)/2);
			prterr("e->label [%s] prox [%f] proy [%f] ",e->label,prox,proy);TRACE;
			e->prox=prox;
			e->proy=proy;
			qt_QCanvasText_setColor((qt_QCanvasText *)text,(qt_QColor*)e->labelColor);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)text,TRUE);
			prterr("e->label [%s] setX [%f] setY [%f] ",e->label,e->prox*width,e->proy*height);TRACE;
			qt_QCanvasItem_setX((qt_QCanvasItem *)text,e->prox*width);
			qt_QCanvasItem_setY((qt_QCanvasItem *)text,e->proy*height);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)text,1);
			qt_QCanvasItem_show((qt_QCanvasItem *)text);
			qt_del_QString(label);
		}
	}
	TRACE;
	qt_del_QFont(f);
}
void DrawVerticalBarChart(qt_QCanvas *Canvas,RDAchart *chart,double total,int count)
{
	double width=0.0;
	double height=0.0,z=0;
	int prowidth=0;
	int x=0,i=0,y=0,extent=0;
	RDAelement *e=NULL;
	qt_QCanvasRectangle *crect=NULL;
	qt_QPen *pen=NULL;
	qt_QBrush *brush=NULL;
	qt_QString *label=NULL;
	qt_QCanvasText *text=NULL;
	qt_QFont *f=NULL;
	char temp[512];

	label=qt_new_QString5(RDAFontNAME);
	f=qt_new_QFont1(label,6,60,0);
	qt_del_QString(label);
	width=qt_QCanvas_width((qt_QCanvas *)Canvas);
	height=qt_QCanvas_height((qt_QCanvas *)Canvas);
	prowidth=width/count;
	prterr("width [%f] height [%f] prowidth [%d] ",width,height,prowidth);TRACE;
	pen=qt_new_QPen();
	qt_QPen_setStyle((qt_QPen *)pen,QT_NoPen);
	x=0;
	for(i=0,e=chart->elements;i<chart->num;++i,++e)
	{
		if(e->valid)
		{
			extent=e->scales;
			y=height-extent;
			crect=qt_new_QCanvasRectangle2(x,y,prowidth,extent,(qt_QCanvas *)Canvas);
			prterr("Rectangle e->label [%s] x [%d] y [%d] width [%d] height [%d] ",e->label,x,y,prowidth,extent);TRACE;
			brush=qt_new_QBrush(e->valueColor,e->valuePattern);
			qt_QCanvasPolygonalItem_setBrush((qt_QCanvasPolygonalItem *)crect,brush);
			qt_QCanvasPolygonalItem_setPen((qt_QCanvasPolygonalItem *)crect,(qt_QPen *)pen);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)crect,TRUE);
			qt_QCanvasItem_setX((qt_QCanvasItem *)crect,x);
			qt_QCanvasItem_setY((qt_QCanvasItem *)crect,y);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)crect,0);
			TRACE;
			qt_QCanvasItem_show((qt_QCanvasItem *)crect);
			TRACE;
	/*
			qt_del_QBrush(brush);
	*/
			memset(temp,0,512);
			sprintf(temp,"%s %.02f %2.1f",(e->label!=NULL ? e->label:""),e->value,(e->value/total)*100);
			label=qt_new_QString5(temp);
			text=qt_new_QCanvasText2((qt_QString *)label,(qt_QFont *)f,(qt_QCanvas *)Canvas);
			qt_QCanvasItem_setCanvas((qt_QCanvasItem *)text,(qt_QCanvas *)Canvas);
			e->prox=x/width;
			e->proy=y/height;
			qt_QCanvasText_setColor((qt_QCanvasText *)text,(qt_QColor*)e->labelColor);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)text,TRUE);
			z=e->prox*width;
			qt_QCanvasItem_setX((qt_QCanvasItem *)text,z);
			z=e->proy*height;
			qt_QCanvasItem_setY((qt_QCanvasItem *)text,z);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)text,1);
			qt_QCanvasItem_show((qt_QCanvasItem *)text);
			qt_del_QString(label);
			x+=prowidth;
		}
	}
	TRACE;
	qt_del_QFont(f);
}
void DrawHorizontalBarChart(qt_QCanvas *Canvas,RDAchart *chart,double total,int count)
{
	double width=0.0;
	double height=0.0;
	int proheight=0;
	int i=0,y=0,extent=0;
	RDAelement *e=NULL;
	qt_QCanvasRectangle *crect=NULL;
	qt_QPen *pen=NULL;
	qt_QBrush *brush=NULL;
	qt_QString *label=NULL;
	qt_QCanvasText *text=NULL;
	qt_QFont *f=NULL;
	char temp[512];

	label=qt_new_QString5(RDAFontNAME);
	f=qt_new_QFont1(label,6,60,0);
	qt_del_QString(label);
	width=qt_QCanvas_width((qt_QCanvas *)Canvas);
	height=qt_QCanvas_height((qt_QCanvas *)Canvas);
	proheight=height/count;
	pen=qt_new_QPen();
	qt_QPen_setStyle((qt_QPen *)pen,QT_NoPen);
	prterr("height [%f] width [%f] proheight [%d] ",height,width,proheight);TRACE;
	for(i=0,e=chart->elements;i<chart->num;++i,++e)
	{
		if(e->valid)
		{
			extent=e->scales;
			crect=qt_new_QCanvasRectangle2(0,y,extent,proheight,(qt_QCanvas *)Canvas);
			brush=qt_new_QBrush(e->valueColor,e->valuePattern);
			qt_QCanvasPolygonalItem_setBrush((qt_QCanvasPolygonalItem *)crect,brush);
			qt_QCanvasPolygonalItem_setPen((qt_QCanvasPolygonalItem *)crect,(qt_QPen *)pen);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)crect,0);
			qt_QCanvasItem_show((qt_QCanvasItem *)crect);
	/*
			qt_del_QBrush(brush);
	*/
			memset(temp,0,512);
			sprintf(temp,"%s %.02f %2.1f",(e->label!=NULL ? e->label:""),e->value,(e->value/total)*100);
			label=qt_new_QString5(temp);
			text=qt_new_QCanvasText2((qt_QString *)label,(qt_QFont *)f,(qt_QCanvas *)Canvas);
			e->prox=0;
			e->proy=y/height;
			qt_QCanvasText_setColor((qt_QCanvasText *)text,(qt_QColor*)e->labelColor);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)text,TRUE);
			qt_QCanvasItem_setX((qt_QCanvasItem *)text,e->prox*width);
			qt_QCanvasItem_setY((qt_QCanvasItem *)text,e->proy*height);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)text,1);
			qt_QCanvasItem_show((qt_QCanvasItem *)text);
			qt_del_QString(label);
			y+=proheight;
		}
	}
	TRACE;
	qt_del_QFont(f);
}
void DrawElements(qt_QMainWindow *MainWindow,qt_QCanvasView *cv,qt_QCanvas *Canvas,RDAchart *chart)
{
	qt_QCanvasItemList *list=NULL;
	qt_QCanvasItem *item=NULL;
	qt_QString *label=NULL;
	int x=0;
	int scaleFactor=0;
	double biggest=0.0;
	int count=0;
	double total=0.0;
	RDAelement *e=NULL;

	if(chart==NULL) return;
	if(chart->elements==NULL) return;

	TRACE;
/*
	list=qt_QCanvas_allItems((qt_QCanvas *)Canvas);
	if(list!=NULL)
	{
		count=qt_QWidgetList_count((qt_QWidgetList *)list);
		for(x=0;x<count;++x) 
		{
			item=(qt_QCanvasItem *)qt_QWidgetList_at((qt_QWidgetList *)list,x);
			qt_QCanvas_removeItem((qt_QCanvas *)Canvas,(qt_QCanvasItem *)item);
		}
	}
*/
	TRACE;
	x=0;
	count=0;
	switch(chart->type)
	{
		default:
		case PIE:
			scaleFactor=5760;
			break;
		case VERTICAL_BAR:
			scaleFactor=qt_QCanvas_height((qt_QCanvas *)Canvas);
			break;
		case HORIZONTAL_BAR:
			scaleFactor=qt_QCanvas_width((qt_QCanvas *)Canvas);
			break;
	}
	prterr("scaleFactor [%d] ",scaleFactor);TRACE;
	for(x=0,e=chart->elements;x<chart->num;++x,++e)
	{
		if(e->value<=0.0) e->valid=FALSE;
		if(e->valid)
		{
			++count;
			total+=e->value;
			if(e->value>biggest) biggest=e->value;
			e->scales=e->value*scaleFactor;
		}
	}
	prterr("count [%d] biggest [%f] scaleFactor [%d] ",count,biggest,scaleFactor);TRACE;
	if(count)
	{
		for(x=0,e=chart->elements;x<chart->num;++x,++e)
		{
			if(e->valid)
			{
				if(chart->type==PIE)
				{
					e->scales=e->value*scaleFactor/total;
				} else {
					e->scales=e->value*scaleFactor/biggest;
				}
				prterr("\te->label [%s] e->value [%f] e->scales [%f] ",(e->label!=NULL ? e->label:""),e->value,e->scales);TRACE;
			}
		}
	}
	switch(chart->type)
	{
		default:
		case PIE:
			TRACE;
			DrawPieChart((qt_QCanvas *)Canvas,chart,total,count);
			TRACE;
			break;
		case VERTICAL_BAR:
			TRACE;
			DrawVerticalBarChart((qt_QCanvas *)Canvas,chart,total,count);
			TRACE;
			break;
		case HORIZONTAL_BAR:
			TRACE;
			DrawHorizontalBarChart((qt_QCanvas *)Canvas,chart,total,count);
			TRACE;
			break;
	}
	TRACE;
	qt_QCanvas_update((qt_QCanvas *)Canvas);
	TRACE;
}
void DisplayChart(RDAchart *chart)
{
	qt_QMainWindow *MainWindow=NULL;
	qt_QCanvas *Canvas=NULL;
	qt_QCanvasView *CanvasView=NULL;
	int windowWidth=810;
	int windowHeight=520;
	RDAelement *e=NULL;
	double f1=0.0,f2=0.0,fr=0.0;
	int x=0,patnum=0,r=0,g=0,b=0;
	qt_QPaintDevice *pd=NULL;
	qt_QPainter *painter=NULL;

	if(chart!=NULL)
	{
		for(x=0,e=chart->elements;x<chart->num;++x,++e)
		{
			switch(x)
			{
				case 0: /* red */
					e->valueColor=qt_new_QColor5("red");
					break;
				case 1: /* cyan */
					e->valueColor=qt_new_QColor5("cyan");
					break;
				case 2: /* blue */
					e->valueColor=qt_new_QColor5("blue");
					break;
				case 3: /* yellow */
					e->valueColor=qt_new_QColor5("yellow");
					break;
				case 4: /* green */
					e->valueColor=qt_new_QColor5("green");
					break;
				case 5: /* magenta */
					e->valueColor=qt_new_QColor5("magenta");
					break;
				case 6: /* darkYellow */
					e->valueColor=qt_new_QColor5("darkYellow");
					break;
				case 7: /* darkRed */
					e->valueColor=qt_new_QColor5("darkRed");
					break;
				case 8: /* darkCyan */
					e->valueColor=qt_new_QColor5("darkCyan");
					break;
				case 9: /* darkGreen */
					e->valueColor=qt_new_QColor5("darkGreen");
					break;
				case 10: /* darkMagenta */
					e->valueColor=qt_new_QColor5("darkMagenta");
					break;
				case 11: /* darkBlue */
					e->valueColor=qt_new_QColor5("darkBlue");
					break;
				default:
					f1=x*360/chart->num;
					f2=((x*256)%105)+151;
					fr=((x*17)%105)+151;
					r=f1;
					g=f2;
					b=fr;
					e->valueColor=qt_new_QColor2(r,g,b,QCOLOR_Hsv);
					break;
			}
			e->labelColor=qt_new_QColor5("black");
			f1=x;
			f2=12;
			fr=fmod(f1,f2);
			if(fr==0) patnum+=1;
			e->valuePattern=patnum;
		}
	}
	MainWindow=qt_new_QMainWindow((qt_QWidget *)NULL,"CHART DATABASE",QT_WType_TopLevel|QT_WStyle_Customize|QT_WStyle_NormalBorder|QT_WStyle_Title|QT_WStyle_MinMax);
	qt_QWidget_resize((qt_QWidget *)MainWindow,windowWidth,windowHeight);
	TRACE;
	Canvas=qt_new_QCanvas((qt_QObject *)MainWindow,"CHART CANVAS");
	prterr("windowWidth [%d] windowHeight [%d] ",windowWidth,windowHeight);TRACE;
	qt_QCanvas_resize((qt_QCanvas *)Canvas,windowWidth-6,windowHeight-30);
	TRACE;
	CanvasView=qt_new_QCanvasView1((qt_QCanvas *)Canvas,(qt_QWidget *)MainWindow,"CHART VIEW",0);
	TRACE;
	qt_QMainWindow_setCentralWidget((qt_QMainWindow *)MainWindow,(qt_QWidget *)CanvasView);
	TRACE;
	pd=qt_QWidget_QPaintDevice((qt_QWidget *)MainWindow);
	TRACE;
	if(pd!=NULL) 
	{
		prterr("got something for pd...");TRACE;
	}
	TRACE;
	painter=qt_new_QPainter();
	TRACE;
	qt_QPainter_begin((qt_QPainter *)painter,(qt_QPaintDevice *)pd,TRUE);
	TRACE;
	DrawElements((qt_QMainWindow *)MainWindow,(qt_QCanvasView *)CanvasView,(qt_QCanvas *)Canvas,chart);
	TRACE;
	qt_QCanvasView_drawContents((qt_QCanvasView *)CanvasView,(qt_QPainter *)painter,0,0,-1,-1);
	TRACE;
	qt_QPainter_end((qt_QPainter *)painter);
	TRACE;
	TRACE;
	qt_QWidget_show((qt_QWidget *)CanvasView);
	TRACE;
	qt_QObject_connect1((qt_QObject *)RDAMAINWIDGET,"2lastWindowClosed()",(qt_QObject *)RDAMAINWIDGET,"1quit()");
	TRACE;
	qt_QMainWindow_show((qt_QMainWindow *)MainWindow);
	TRACE;
	RDAAPPMAINLOOP();
}
