/* chartdat.c - Chart Database Columns */
/*lint -library */
#ifndef WIN32
#define __NAM__ "chartdat.lnx"
#endif
#ifdef WIN32
#define __NAM__ "chartdat.exe"
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


static qt_QPopupMenu *FPOP=NULL,*STYLEPOP=NULL;
static qt_QMainWindow *MainWindow=NULL;
static qt_QCanvas *Canvas=NULL;
static qt_QCanvasView *CanvasView=NULL;
static int windowWidth=810,windowHeight=580;
enum chartTypes {PIE,VERTICAL_BAR,HORIZONTAL_BAR};
static char UseDollars=FALSE;
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

static short addRDAelement(RDAchart *c,double value,char *label)
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
			e->valuePattern=0;
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
		e->valuePattern=0;
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
static void addExtraFiles(RDArsrc *tmprsrc,short fileno)
{
	APPlib *files=NULL;
	char *libx=NULL;
	int x=0,y=0;
	short fileno1=(-1);
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	NRDfield *field=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(MODULENAME(fileno))+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,MODULENAME(fileno));
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,MODULENAME(fileno));
#endif
	files=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(files!=NULL)
	{
		for(x=0;x<files->numlibs;++x)
		{
			if(RDAstrcmp(FILENAME(fileno),files->libs[x]))
			{
				fileno1=OPNNRD(MODULENAME(fileno),files->libs[x]);
				if(fileno1!=(-1))
				{
					key=KEYNUM(fileno1,1);
					if(key!=NULL)
					{
						for(y=0,part=key->part;(y<key->numparts);++y,++part)
						{
							field=FLDNRD(fileno,part->name);
							if(field==NULL) break;
						}
						if(y<key->numparts)
						{
							CLSNRD(fileno1);
						} else {
							addDFincvir(tmprsrc,MODULENAME(fileno),files->libs[x],NULL,fileno1);
						}
					} else {
						CLSNRD(fileno1);
					}
				}
			}
		}
		freeapplib(files);
	}
}
static void CloseFiles(RDArsrc *tmprsrc)
{
	DFincvir *i=NULL;
	int x=0;

	if(tmprsrc!=NULL)
	{
		if(tmprsrc->incvir!=NULL)
		{
			for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
			{
				CLSNRD(i->fileno);
			}
		}
	}
}
static NRDfield *FindThisField(RDArsrc *tmprsrc,char *name)
{
	DFincvir *i=NULL;
	NRDfield *field=NULL;
/*
	RDAvirtual *virtual=NULL;
*/
	int x=0;

	if(tmprsrc!=NULL)
	{
		if(tmprsrc->incvir!=NULL)
		{
			for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
			{
				field=FLDNRD(i->fileno,name);
				if(field!=NULL) return(field);
			}
		}
/*
		if(tmprsrc->virtual!=NULL)
		{
			for(x=0,virtual=tmprsrc->virtual;x<tmprsrc->num;++x,++virtual)
			{
				if(!RDAstrcmp(virtual->name,name))
				{
					COMPUTE_SCREEN_VIRTUAL(tmprsrc,name,SCRNVirtualSubData,tmprsrc);

				}
			}
		}
*/
	}
	return(NULL);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short fileno=(-1),ef=(-1),DID_STUFF=FALSE;
	double v=0.0;
	NRDfield *labelField=NULL,*valueField=NULL;
	char delflag=FALSE,*temp=NULL;
	char myTemp[512],*selexp=NULL,skip=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(argc<5) std::exit;
	if(InitializeSubsystems(argc,argv,"UTILITIES","CHART DATABASE"))
	{
		RDAAPPMAINLOOP();
		return;
	}
	chart.total=0;
	if(argc>5) chart.name=stralloc(argv[6]);
		else chart.name=NULL;
	chart.description=NULL;
	chart.num=0;
	if(RDAstrstr(argv[1],"PIE"))
	{
		chart.type=PIE;
	} else if(RDAstrstr(argv[1],"VERTICAL"))
	{
		chart.type=VERTICAL_BAR;
	} else if(RDAstrstr(argv[1],"HORIZONTAL"))
	{
		chart.type=HORIZONTAL_BAR;
	} else chart.type=VERTICAL_BAR;
	chart.elements=NULL;
	fileno=OPNNRD(argv[2],argv[3]);
	if(fileno==(-1)) 
	{
		memset(myTemp,0,512);
		sprintf(myTemp,"Error cannot open file [%s] [%s] for reading.",argv[2],argv[3]);
		ERRORDIALOG("ERROR FILE OPEN",myTemp,NULL,TRUE);
		ShutdownSubsystems();
		return;
	} else {
		tmprsrc=RDArsrcNEW("UTILITIES","CHART DATABASE");
		addDFincvir(tmprsrc,argv[2],argv[3],NULL,fileno);
		addExtraFiles(tmprsrc,fileno);
		GET_SCREEN_VIRTUALS(tmprsrc,0);
		if(argc>6)
		{
			selexp=stralloc(argv[7]);
		} else selexp=NULL;
		labelField=FindThisField(tmprsrc,argv[4]);
		valueField=FindThisField(tmprsrc,argv[5]);
		if(labelField==NULL)
		{
			memset(myTemp,0,512);
			sprintf(myTemp,"Error label field [%s]  not available in [%s] [%s].",argv[4],argv[2],argv[3]);
			ERRORDIALOG("ERROR LABEL FIELD UNAVAILABLE",myTemp,NULL,TRUE);
			CloseFiles(tmprsrc);
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			ShutdownSubsystems();
			return;
		}
		if(valueField==NULL)
		{
			memset(myTemp,0,512);
			sprintf(myTemp,"Error value field [%s]  not available in [%s] [%s].",argv[5],argv[2],argv[3]);
			ERRORDIALOG("ERROR VALUE FIELD UNAVAILABLE",myTemp,NULL,TRUE);
			CloseFiles(tmprsrc);
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
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
					sprintf(myTemp,"Error value field [%s]  not appropriate type to accumulate in [%s] [%s].",argv[5],argv[2],argv[3]);
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
				SetRDArsrcFiles(fileno,tmprsrc);
				skip=FALSE;
				if(!isEMPTY(selexp))
				{
					skip=!EVALUATEbol(selexp,SCRNvirtualSubData,tmprsrc);
				}
				if(!skip)
				{
					labelField=FindThisField(tmprsrc,argv[4]);
					valueField=FindThisField(tmprsrc,argv[5]);
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
							if(!RDAstrcmp(argv[5],"DELETEFLAG")) v=!*valueField->data.string_value;
							else v=*valueField->data.string_value;
							break;
						case DOLLARS:
							v=*valueField->data.float_value;
							UseDollars=TRUE;
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
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
			}
			CLEAR_SCREEN_VIRTUAL(tmprsrc);
			ef=NXTNRD(fileno,1);
		}
		CloseFiles(tmprsrc);
	}
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
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
	double width=0.0,tw=0.0,th=0.0;
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
	char temp[512],*amt=NULL;

	label=qt_new_QString5(RDAFontNAME);
	f=qt_new_QFont1(label,7,60,0);
	qt_del_QString(label);
	width=qt_QCanvas_width((qt_QCanvas *)Canvas);
	height=qt_QCanvas_height((qt_QCanvas *)Canvas);
	if(width>height) size=height;
		else size=width;
	size=size/2;
	x=width/2;
	y=height/2;
	angle=0;
	for(i=0,e=chart->elements;i<chart->num;++i,++e)
	{
		if(e->valid)
		{

			extent=e->scales;
			arc=qt_new_QCanvasEllipse2(size,size,angle,extent,(qt_QCanvas *)Canvas);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)arc,TRUE);
			qt_QCanvasItem_setX((qt_QCanvasItem *)arc,x);
			qt_QCanvasItem_setY((qt_QCanvasItem *)arc,y);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)arc,0);
			brush=qt_new_QBrush2(e->valueColor,e->valuePattern);
			qt_QCanvasPolygonalItem_setBrush((qt_QCanvasPolygonalItem *)arc,brush);
			qt_QCanvasItem_show((qt_QCanvasItem *)arc);
			qt_del_QBrush(brush);
			angle+=extent;
			memset(temp,0,512);
			if(UseDollars)
			{
				amt=famt(e->value,15);
				sprintf(temp,"%s %s %2.1f%c",(e->label!=NULL ? e->label:""),amt,(e->value/total)*100,'%');
				if(amt!=NULL) Rfree(amt);
			} else {
				sprintf(temp,"%s %.02f %2.1f%c",(e->label!=NULL ? e->label:""),e->value,(e->value/total)*100,'%');
			}
			label=qt_new_QString5(temp);
			/* label=qt_new_QString5("."); */
			text=qt_new_QCanvasText2((qt_QString *)label,(qt_QFont *)f,(qt_QCanvas *)Canvas);
			qt_QCanvasItem_setCanvas((qt_QCanvasItem *)text,(qt_QCanvas *)Canvas);
			rect=qt_QCanvasPolygonalItem_boundingRect((qt_QCanvasPolygonalItem *)arc);
			prox=(qt_QRect_width((qt_QRect *)rect)/2)+qt_QRect_x((qt_QRect *)rect);
			proy=(qt_QRect_height((qt_QRect *)rect)/2)+qt_QRect_y((qt_QRect *)rect);
			rect=qt_QCanvasText_boundingRect((qt_QCanvasText *)text);
			prox-=(qt_QRect_width((qt_QRect *)rect)/2);
			proy-=(qt_QRect_height((qt_QRect *)rect)/2);
			if(diagapps)
			{
				prterr("prox [%d] proy [%d] width [%d] height [%d] label [%s]\n",prox,proy,width,height,temp);
			}
			prox/=width;
			proy/=height;
			e->prox=prox;
			e->proy=proy;
			if(diagapps)
			{
				prterr("prox [%d] proy [%d] width [%d] height [%d]\n",prox,proy,width,height);
			}
			qt_QCanvasText_setColor((qt_QCanvasText *)text,(qt_QColor*)e->labelColor);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)text,TRUE);
			qt_QCanvasItem_setX((qt_QCanvasItem *)text,e->prox*width);
			qt_QCanvasItem_setY((qt_QCanvasItem *)text,e->proy*height);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)text,1);
			qt_QCanvasItem_show((qt_QCanvasItem *)text);
			qt_del_QString(label);
		}
	}
	qt_del_QFont(f);
	if(!isEMPTY(chart->name))
	{
		label=qt_new_QString5(RDAFontNAME);
		f=qt_new_QFont1(label,12,60,0);
		qt_del_QString(label);
		label=qt_new_QString5(chart->name);
		text=qt_new_QCanvasText2((qt_QString *)label,(qt_QFont *)f,(qt_QCanvas *)Canvas);
		tw=0;
		th=qt_QRect_height((qt_QRect *)rect)/2;
		tw=qt_QRect_width((qt_QRect *)rect)/2;
		qt_QCanvasItem_setVisible((qt_QCanvasItem *)text,TRUE);

		qt_QCanvasItem_setX((qt_QCanvasItem *)text,tw);
		qt_QCanvasItem_setY((qt_QCanvasItem *)text,th);
		qt_QCanvasItem_setZ((qt_QCanvasItem *)text,2);
		qt_QCanvasItem_show((qt_QCanvasItem *)text);
		qt_del_QString(label);
		qt_del_QFont(f);
	}
}
void DrawVerticalBarChart(qt_QCanvas *Canvas,RDAchart *chart,double total,int count)
{
	double width=0.0,tw=0.0,th=0.0;
	double height=0.0,z=0;
	int prowidth=0;
	int x=0,i=0,y=0,extent=0;
	RDAelement *e=NULL;
	qt_QRect *rect=NULL;
	qt_QCanvasRectangle *crect=NULL;
	qt_QPen *pen=NULL;
	qt_QBrush *brush=NULL;
	qt_QString *label=NULL;
	qt_QCanvasText *text=NULL;
	qt_QFont *f=NULL;
	char temp[512],*amt=NULL;

	label=qt_new_QString5(RDAFontNAME);
	f=qt_new_QFont1(label,7,60,0);
	qt_del_QString(label);
	width=qt_QCanvas_width((qt_QCanvas *)Canvas);
	height=qt_QCanvas_height((qt_QCanvas *)Canvas);
	prowidth=width/count;
	pen=qt_new_QPen1(QT_NoPen);
	x=0;
	for(i=0,e=chart->elements;i<chart->num;++i,++e)
	{
		if(e->valid)
		{
			extent=e->scales;
			y=height-extent;
			crect=qt_new_QCanvasRectangle2(x,y,prowidth,extent,(qt_QCanvas *)Canvas);
			brush=qt_new_QBrush2(e->valueColor,e->valuePattern);
			qt_QCanvasPolygonalItem_setPen((qt_QCanvasPolygonalItem *)crect,(qt_QPen *)pen);
			qt_QCanvasPolygonalItem_setBrush((qt_QCanvasPolygonalItem *)crect,brush);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)crect,TRUE);
			qt_QCanvasItem_setX((qt_QCanvasItem *)crect,x);
			qt_QCanvasItem_setY((qt_QCanvasItem *)crect,y);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)crect,0);
			qt_QCanvasItem_setVisible((qt_QCanvasItem *)crect,TRUE);
			qt_QCanvasItem_show((qt_QCanvasItem *)crect);
			qt_del_QBrush(brush);
			memset(temp,0,512);
			if(UseDollars)
			{
				amt=famt(e->value,15);
				sprintf(temp,"%s\n%s\n%2.1f%c",(e->label!=NULL ? e->label:""),amt,(e->value/total)*100,'%');
				if(amt!=NULL) Rfree(amt);
			} else {
				sprintf(temp,"%s\n%.02f\n%2.1f%c",(e->label!=NULL ? e->label:""),e->value,(e->value/total)*100,'%');
			}
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
	qt_del_QFont(f);
	if(!isEMPTY(chart->name))
	{
		label=qt_new_QString5(RDAFontNAME);
		f=qt_new_QFont1(label,12,60,0);
		qt_del_QString(label);
		label=qt_new_QString5(chart->name);
		text=qt_new_QCanvasText2((qt_QString *)label,(qt_QFont *)f,(qt_QCanvas *)Canvas);
		rect=qt_QCanvasText_boundingRect((qt_QCanvasText *)text);
		tw=0;
		th=qt_QRect_height((qt_QRect *)rect)/2;
		qt_QCanvasItem_setVisible((qt_QCanvasItem *)text,TRUE);

		qt_QCanvasItem_setX((qt_QCanvasItem *)text,tw);
		qt_QCanvasItem_setY((qt_QCanvasItem *)text,th);
		qt_QCanvasItem_setZ((qt_QCanvasItem *)text,2);
		qt_QCanvasItem_show((qt_QCanvasItem *)text);
		qt_del_QString(label);
		qt_del_QFont(f);
	}
}
void DrawHorizontalBarChart(qt_QCanvas *Canvas,RDAchart *chart,double total,int count)
{
	double width=0.0,th=0.0,tw=0.0;
	double height=0.0;
	int proheight=0;
	int i=0,y=0,extent=0;
	RDAelement *e=NULL;
	qt_QCanvasRectangle *crect=NULL;
	qt_QRect *rect=NULL;
	qt_QPen *pen=NULL;
	qt_QBrush *brush=NULL;
	qt_QString *label=NULL;
	qt_QCanvasText *text=NULL;
	qt_QFont *f=NULL;
	char temp[512],*amt=NULL;

	label=qt_new_QString5(RDAFontNAME);
	f=qt_new_QFont1(label,7,60,0);
	qt_del_QString(label);
	width=qt_QCanvas_width((qt_QCanvas *)Canvas);
	height=qt_QCanvas_height((qt_QCanvas *)Canvas);
	proheight=height/count;
	pen=qt_new_QPen();
	qt_QPen_setStyle((qt_QPen *)pen,QT_NoPen);
	for(i=0,e=chart->elements;i<chart->num;++i,++e)
	{
		if(e->valid)
		{
			extent=e->scales;
			crect=qt_new_QCanvasRectangle2(0,y,extent,proheight,(qt_QCanvas *)Canvas);
			brush=qt_new_QBrush2(e->valueColor,e->valuePattern);
			qt_QCanvasPolygonalItem_setBrush((qt_QCanvasPolygonalItem *)crect,brush);
			qt_QCanvasPolygonalItem_setPen((qt_QCanvasPolygonalItem *)crect,(qt_QPen *)pen);
			qt_QCanvasItem_setZ((qt_QCanvasItem *)crect,0);
			qt_QCanvasItem_show((qt_QCanvasItem *)crect);
			qt_del_QBrush(brush);
			memset(temp,0,512);
			if(UseDollars)
			{
				amt=famt(e->value,15);
				sprintf(temp,"%s\n%s\n%2.1f%c",(e->label!=NULL ? e->label:""),amt,(e->value/total)*100,'%');
				if(amt!=NULL) Rfree(amt);
			} else {
				sprintf(temp,"%s\n%.02f\n%2.1f%c",(e->label!=NULL ? e->label:""),e->value,(e->value/total)*100,'%');
			}
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
	qt_del_QFont(f);
	if(!isEMPTY(chart->name))
	{
		label=qt_new_QString5(RDAFontNAME);
		f=qt_new_QFont1(label,12,60,0);
		qt_del_QString(label);
		label=qt_new_QString5(chart->name);
		text=qt_new_QCanvasText2((qt_QString *)label,(qt_QFont *)f,(qt_QCanvas *)Canvas);
		rect=qt_QCanvasText_boundingRect((qt_QCanvasText *)text);
		tw=width-(qt_QRect_width((qt_QRect *)rect));
		th=qt_QRect_height((qt_QRect *)rect)/2;
		qt_QCanvasItem_setVisible((qt_QCanvasItem *)text,TRUE);

		qt_QCanvasItem_setX((qt_QCanvasItem *)text,tw);
		qt_QCanvasItem_setY((qt_QCanvasItem *)text,th);
		qt_QCanvasItem_setZ((qt_QCanvasItem *)text,2);
		qt_QCanvasItem_show((qt_QCanvasItem *)text);
		qt_del_QString(label);
		qt_del_QFont(f);
	}
}
void DrawElements(RDAchart *chart)
{
	int x=0;
	int scaleFactor=0;
	double biggest=0.0;
	int count=0;
	double total=0.0;
	RDAelement *e=NULL;

	if(chart==NULL) return;
	if(chart->elements==NULL) return;

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
			}
		}
	}
	switch(chart->type)
	{
		default:
		case PIE:
			DrawPieChart((qt_QCanvas *)Canvas,chart,total,count);
			break;
		case VERTICAL_BAR:
			DrawVerticalBarChart((qt_QCanvas *)Canvas,chart,total,count);
			break;
		case HORIZONTAL_BAR:
			DrawHorizontalBarChart((qt_QCanvas *)Canvas,chart,total,count);
			break;
	}
	qt_QCanvas_update((qt_QCanvas *)Canvas);
}
static void changetoPieChart(RDAchart *chart)
{
	qt_del_QCanvas((qt_QCanvas *)Canvas);
	Canvas=qt_new_QCanvas((qt_QObject *)MainWindow,"CHART CANVAS");
	qt_QCanvas_resize((qt_QCanvas *)Canvas,windowWidth-6,windowHeight-30);
	qt_QCanvasView_setCanvas((qt_QCanvasView *)CanvasView,(qt_QCanvas *)Canvas);
	chart->type=PIE;
	DrawElements(chart);
}
static void changetoVerticalChart(RDAchart *chart)
{
	qt_del_QCanvas((qt_QCanvas *)Canvas);
	Canvas=qt_new_QCanvas((qt_QObject *)MainWindow,"CHART CANVAS");
	qt_QCanvas_resize((qt_QCanvas *)Canvas,windowWidth-6,windowHeight-30);
	qt_QCanvasView_setCanvas((qt_QCanvasView *)CanvasView,(qt_QCanvas *)Canvas);
	chart->type=VERTICAL_BAR;
	DrawElements(chart);
}
static void changetoHorizontalChart(RDAchart *chart)
{
	qt_del_QCanvas((qt_QCanvas *)Canvas);
	Canvas=qt_new_QCanvas((qt_QObject *)MainWindow,"CHART CANVAS");
	qt_QCanvas_resize((qt_QCanvas *)Canvas,windowWidth-6,windowHeight-30);
	qt_QCanvasView_setCanvas((qt_QCanvasView *)CanvasView,(qt_QCanvas *)Canvas);
	chart->type=HORIZONTAL_BAR;
	DrawElements(chart);
}
static void PrintChart(qt_QWidget *CanvasView)
{
	qt_QPrinter *p=NULL;
	qt_QPainter *painter=NULL;
	qt_QRect *rect=NULL;

	p=qt_new_QPrinter(QPRINTER_ScreenResolution);
	if(qt_QPrinter_setup((qt_QPrinter *)p,(qt_QWidget *)MainWindow))
	{
		painter=qt_new_QPainter((qt_QPrinter *)p,FALSE);
		qt_QPainter_begin((qt_QPainter *)painter,(qt_QPaintDevice *)p,FALSE);
		rect=qt_QCanvas_rect((qt_QCanvas *)Canvas);
		qt_QCanvas_drawArea((qt_QCanvas *)Canvas,(qt_QRect *)rect,(qt_QPainter *)painter,FALSE);
		qt_QPainter_end((qt_QPainter *)painter);
		if(painter!=NULL) qt_del_QPainter(painter);
	}
	if(p!=NULL) qt_del_QPrinter(p);
}
static void SaveChart(qt_QWidget *CanvasView)
{
	qt_QString *filename=NULL,*d=NULL,*f=NULL,*c=NULL,*sf=NULL;
	qt_QPixmap *pixmap=NULL;

	d=qt_new_QString5(CURRENTDIRECTORY);
	f=qt_new_QString5("Images (*.png)");
	c=qt_new_QString5("Save Chart to Image");
	sf=qt_new_QString5("png");
	filename=qt_QFileDialog_getSaveFileName(d,f,(qt_QWidget *)MainWindow,"Save Chart Dialog",c,sf,TRUE);

	if(filename!=NULL)
	{
		pixmap=qt_QPixmap_grabWidget((qt_QWidget *)CanvasView,0,0,-1,-1);
		qt_QPixmap_save((qt_QPixmap *)pixmap,(qt_QString *)filename,"PNG",-1);
		qt_del_QPixmap((qt_QPixmap *)pixmap);
		qt_del_QString(filename);
		if(d!=NULL) qt_del_QString(d);
		if(f!=NULL) qt_del_QString(f);
		if(c!=NULL) qt_del_QString(c);
		if(sf!=NULL) qt_del_QString(sf);
	}
}
void DisplayChart(RDAchart *chart)
{
	RDAelement *e=NULL;
	double f1=0.0,f2=0.0,fr=0.0;
	int x=0,patnum=0,r=0,g=0,b=0,y=0;
	qt_QMenuBar *MenuBar=NULL;
	qt_QMenuData *MenuData=NULL,*BarData=NULL;
	qt_QKeySequence *HAKS=NULL;
	qt_QString *c=NULL;
	qt_Slot *mySlot=NULL;


	if(chart!=NULL)
	{
		y=0;
		for(x=0,e=chart->elements;x<chart->num;++x,++e)
		{
			if(e->value<=0.0) e->valid=FALSE;
			if(e->valid)
			{
			switch(y)
			{
				case 0: /* red */
					e->valueColor=qt_new_QColor5("red");
					break;
				case 1: /* yellow */
					e->valueColor=qt_new_QColor5("yellow");
					break;
				case 2: /* green */
					e->valueColor=qt_new_QColor5("green");
					break;
				case 3: /* cyan */
					e->valueColor=qt_new_QColor5("cyan");
					break;
				case 4: /* blue */
					e->valueColor=qt_new_QColor5("blue");
					break;
				case 5: /* magenta */
					e->valueColor=qt_new_QColor5("grey");
					break;
				case 6: /* brown */
					e->valueColor=qt_new_QColor5("brown");
					break;
				case 7: /* purple */
					e->valueColor=qt_new_QColor5("purple");
					break;
				case 8: /* orange */
					e->valueColor=qt_new_QColor5("orange");
					break;
				case 9: /* darkBlue */
					e->valueColor=qt_new_QColor5("darkBlue");
					break;
				case 10: /* darkRed */
					e->valueColor=qt_new_QColor5("darkRed");
					break;
				case 11: /* darkGreen */
					e->valueColor=qt_new_QColor5("darkGreen");
					break;
				default:
					f1=y*360/chart->num;
					f2=((y*256)%105)+151;
					fr=((y*17)%105)+151;
					r=f1;
					g=f2;
					b=fr;
					e->valueColor=qt_new_QColor2(r,g,b,QCOLOR_Hsv);
					break;
			}
			e->labelColor=qt_new_QColor5("black");
			f1=y;
			f2=12;
			fr=fmod(f1,f2);
			if(fr==0) patnum+=1;
			e->valuePattern=patnum;
			++y;
			}
		}
	}
	MainWindow=qt_new_QMainWindow((qt_QWidget *)NULL,"CHART DATABASE",QT_WType_TopLevel|QT_WStyle_Customize|QT_WStyle_NormalBorder|QT_WStyle_Title|QT_WStyle_MinMax);
	qt_QWidget_resize((qt_QWidget *)MainWindow,windowWidth,windowHeight);
	MenuBar=qt_QMainWindow_menuBar((qt_QMainWindow *)MainWindow);
	SetupWindowColorPalette((qt_QWidget *)MenuBar,"QN2IT MAIN WINDOW");
	BarData=qt_QMenuBar_QMenuData((qt_QMenuBar *)MenuBar);
	FPOP=qt_new_QPopupMenu((qt_QWidget *)MainWindow,"File");
	SetupWindowColorPalette((qt_QWidget *)FPOP,"CHART DATABASE");
	c=qt_new_QString5("File");
	qt_QMenuData_insertItem6((qt_QMenuData *)BarData,c,(qt_QPopupMenu *)FPOP,-1,-1);
	STYLEPOP=qt_new_QPopupMenu((qt_QWidget *)MainWindow,"File");
	SetupWindowColorPalette((qt_QWidget *)STYLEPOP,"CHART DATABASE");
	c=qt_new_QString5("Style");
	qt_QMenuData_insertItem6((qt_QMenuData *)BarData,c,(qt_QPopupMenu *)STYLEPOP,-1,-1);
	Canvas=qt_new_QCanvas((qt_QObject *)MainWindow,"CHART CANVAS");
	qt_QCanvas_resize((qt_QCanvas *)Canvas,windowWidth-6,windowHeight-30);
	CanvasView=qt_new_QCanvasView1((qt_QCanvas *)Canvas,(qt_QWidget *)MainWindow,"CHART VIEW",0);
	qt_QMainWindow_setCentralWidget((qt_QMainWindow *)MainWindow,(qt_QWidget *)CanvasView);
	DrawElements(chart);
	qt_QWidget_show((qt_QWidget *)CanvasView);

	MenuData=qt_QPopupMenu_QMenuData((qt_QPopupMenu *)FPOP);
	c=qt_new_QString5("Print");
	HAKS=qt_new_QKeySequence2(QT_CTRL+QT_Key_P);
	mySlot=(qt_Slot *)qt_new_UserDataSlot(PrintChart,CanvasView);
	qt_QMenuData_insertItem((qt_QMenuData *)MenuData,c,(qt_QObject *)mySlot,"1UserDataSlot()",HAKS,-1,-1);
	qt_del_QKeySequence(HAKS);
	qt_del_QString(c);
	c=qt_new_QString5("Save");
	HAKS=qt_new_QKeySequence2(QT_CTRL+QT_Key_S);
	mySlot=(qt_Slot *)qt_new_UserDataSlot(SaveChart,CanvasView);
	qt_QMenuData_insertItem((qt_QMenuData *)MenuData,c,(qt_QObject *)mySlot,"1UserDataSlot()",HAKS,-1,-1);
	qt_del_QKeySequence(HAKS);
	qt_del_QString(c);
	c=qt_new_QString5("Quit");
	HAKS=qt_new_QKeySequence2(QT_CTRL+QT_Key_Q);
	qt_QMenuData_insertItem((qt_QMenuData *)MenuData,c,(qt_QObject *)RDAMAINWIDGET,"1quit()",HAKS,-1,-1);
	qt_del_QKeySequence(HAKS);
	qt_del_QString(c);
	MenuData=qt_QPopupMenu_QMenuData((qt_QPopupMenu *)STYLEPOP);
	c=qt_new_QString5("Pie Chart");
	HAKS=qt_new_QKeySequence2(QT_CTRL+QT_Key_P);
	mySlot=(qt_Slot *)qt_new_UserDataSlot(changetoPieChart,chart);
	qt_QMenuData_insertItem((qt_QMenuData *)MenuData,c,(qt_QObject *)mySlot,"1UserDataSlot()",HAKS,-1,-1);
	qt_del_QKeySequence(HAKS);
	qt_del_QString(c);
	c=qt_new_QString5("Vertical Bar Chart");
	HAKS=qt_new_QKeySequence2(QT_CTRL+QT_Key_V);
	mySlot=(qt_Slot *)qt_new_UserDataSlot(changetoVerticalChart,chart);
	qt_QMenuData_insertItem((qt_QMenuData *)MenuData,c,(qt_QObject *)mySlot,"1UserDataSlot()",HAKS,-1,-1);
	qt_del_QKeySequence(HAKS);
	qt_del_QString(c);
	c=qt_new_QString5("Horizontal Bar Chart");
	HAKS=qt_new_QKeySequence2(QT_CTRL+QT_Key_H);
	mySlot=(qt_Slot *)qt_new_UserDataSlot(changetoHorizontalChart,chart);
	qt_QMenuData_insertItem((qt_QMenuData *)MenuData,c,(qt_QObject *)mySlot,"1UserDataSlot()",HAKS,-1,-1);
	qt_del_QKeySequence(HAKS);

	qt_QObject_connect1((qt_QObject *)RDAMAINWIDGET,"2lastWindowClosed()",(qt_QObject *)RDAMAINWIDGET,"1quit()");
	qt_QMainWindow_show((qt_QMainWindow *)MainWindow);
	RDAAPPMAINLOOP();
}
