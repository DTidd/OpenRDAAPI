/* dfscn.c - Define Screens */
#ifndef WIN32
#define __NAM__ "dfscn.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfscn.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include <cstdlib>
#include <io.h>
#endif /* WIN32 */

#include <mix.hpp>
#include <ldval.hpp>
#include <doheader.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct HoldRDArmems
{
	char *name;
	short field_type;
	union rda_field_types value;
	APPlib *a;
};
typedef struct HoldRDArmems HoldRDArmem;
struct HoldRDArsrcs
{
	char *module;
	char *name;
	int num;
	HoldRDArmem *rscs;
};
typedef struct HoldRDArsrcs HoldRDArsrc;

static void doliblist(RDArsrc *),getscreenlist(RDArsrc *);
static void close_preview_screen(RDArsrc *);
static void show_preview_screen(RDArsrc *);
static void save_screen(RDArsrc *,RDArsrc *);
static void quit_widget(RDArsrc *,RDArsrc *),quit_widgettest(RDArsrc *,RDArsrc *);
static void save_widget(RDArsrc *,RDArsrc *);
static void printscrndef(RDA_PFILE *,RDAscrn *,int *,int *);
static void printscrndefcb(RDArsrc *,RDArsrc *);
static void printmodule(RDArsrc *,RDArsrc *);
static void printdefinition(RDArsrc *,void (*)(...));
static void quit_print_definition(RDArsrc *);
static void changewidgetlist(RDArsrc *);
static void changersctype(RDArsrc *);
static void printheader(RDA_PFILE *,int *,int *,char *,char *);
static void checkheader(RDA_PFILE *,int *,int *,char *);
RDArsrc *mainrsrc=NULL;
static APPlib *RSRClist=NULL;
HoldRDArsrc *HoldRsrc=NULL;
static char Making_A_Menu=FALSE;

struct dfwdgts 
{
	int type;
	char *name;
	char *label;
	char *pixmap;
	char *XHTML_Label;
	short rows;
	short cols;
	int rtype;
	char *expression;
	char *editable_expression;
	char *sensitive_expression;
	char *transversal_expression;
};
typedef struct dfwdgts dfwdgt;
struct dfscrns
{
	short num;
	dfwdgt *wdgts;
};
typedef struct dfscrns dfscn;

static dfscn *dfscreen;
static APPlib *scr_libs,*rsrclist=NULL;
static APPlib *scr_defs;
static APPlib *dir_libs;
static char **wlist,**wdgtlist;
static RDAscrn *screen=NULL;
static RDAdefault *editdefaults=NULL,*rsrcdefaults=NULL;
static char changedvalues=FALSE;

#define HoldRDArsrcNEW(a,b)	xHoldRDArsrcNEW(a,b,__LINE__,__FILE__)
HoldRDArsrc *xHoldRDArsrcNEW(char *module,char *name,int line,char *file)
{
	HoldRDArsrc *tmp=NULL;

	if(diaggui)
	{
		prterr("DIAG HoldRDArsrc for Module [%s] Screen [%s] at line [%d] program [%s].",module,name,line,file);
	}
	tmp=Rmalloc(sizeof(HoldRDArsrc));
	if(!isEMPTY(module)) tmp->module=stralloc(module);
		else tmp->module=NULL;
	if(!isEMPTY(name)) tmp->name=stralloc(name);
		else tmp->name=NULL;
	tmp->num=0;
	tmp->rscs=NULL;
	return(tmp);
}
#define FreeHoldRDArsrc(a)	xFreeHoldRDArsrc(a,__LINE__,__FILE__)
void xFreeHoldRDArsrc(HoldRDArsrc *h,int line,char *file)
{
	int x;
	HoldRDArmem *m;

	if(diaggui)
	{
		prterr("DIAG FreeHoldRDArsrc at line [%d] program [%s].",line,file);
	}
	if(h!=NULL)
	{
		if(h->module!=NULL) Rfree(h->module);
		if(h->name!=NULL) Rfree(h->name);
		if(h->rscs!=NULL)
		{
			for(x=0,m=h->rscs;x<h->num;++x,++m)
			{
				if(m->name!=NULL) Rfree(m->name);
				if(m->a!=NULL) freeapplib(m->a);
				switch(m->field_type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case PLAINTEXT:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case BOOLNS:
					case CHARACTERS:
						if(m->value.string_value!=NULL)
							Rfree(m->value.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case SDECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
						if(m->value.float_value!=NULL)
							Rfree(m->value.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(m->value.short_value!=NULL)
							Rfree(m->value.short_value);
						break;
#ifdef USING_QT
					case PROGRESS_BAR:
#endif
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(m->value.integer_value!=NULL)
							Rfree(m->value.integer_value);
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s].",m->field_type,m->name,h->module,h->name);
						break;
				}
			}
			Rfree(h->rscs);
		}
		Rfree(h);
	}
}
HoldRDArsrc *ReadRsrcList(char *module,char *name)
{
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL,*temp=NULL;
	short x,test=0;
	int y,z,total=0;
	HoldRDArsrc *tmp=NULL;
	HoldRDArmem *mem=NULL;

	if(isEMPTY(module)) return(NULL);
	fname=Rmalloc(RDAstrlen(module)+5);
	fname1=adddashes(module);
	sprintf(fname,"%s.RSC",module);
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		/* JAB:  Disabling this error because it isn't necessary and is annoying, I think. */
		/*prterr("Error in permissions, user cannot read the resource binary library [%s].",fname);*/
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=getlibbin(fname,name,TRUE);
	if(fname!=NULL) Rfree(fname);
	if(mainbin==NULL)
	{
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1700))
	{
		prterr("Error Invalid version or bad binary in Library [%s.RSC].",module);
		BINfree(mainbin);
		return(NULL);
	}
	tmp=HoldRDArsrcNEW(module,name);
	test=BINgetshort(mainbin);
	if(test)
	{
		temp=BINgetstring(mainbin);
		if(temp!=NULL) Rfree(temp);
	} 
	total=BINgetint(mainbin);
	for(x=0;x<total;++x)
	{
		if(tmp->rscs!=NULL)
		{
			tmp->rscs=Rrealloc(tmp->rscs,(tmp->num+1)*sizeof(HoldRDArmem));
		} else {
			tmp->rscs=Rmalloc(sizeof(HoldRDArmem));
		}
		mem=tmp->rscs+tmp->num;
		test=BINgetshort(mainbin);
		if(test) mem->name=BINgetstring(mainbin);
			else mem->name=NULL;
		mem->field_type=BINgetshort(mainbin);
		mem->a=NULL;
		switch(mem->field_type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case PLAINTEXT:
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				test=BINgetshort(mainbin);
				if(test) mem->value.string_value=BINgetstring(mainbin);
					else mem->value.string_value=NULL;
				break;
			case BOOLNS:
				mem->value.string_value=Rmalloc(1);
				*mem->value.string_value=BINgetshort(mainbin);
				break;
			case CHARACTERS:
				mem->value.string_value=Rmalloc(1);
				*mem->value.string_value=BINgetshort(mainbin);
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
				mem->value.float_value=Rmalloc(sizeof(double));
				*mem->value.float_value=BINgetdouble(mainbin);
				break;
			case SHORTV:
			case SSHORTV:
				mem->value.short_value=Rmalloc(sizeof(short));
				*mem->value.short_value=BINgetshort(mainbin);
				break;
			case SCROLLEDLIST:
				mem->value.integer_value=Rmalloc(sizeof(int));
				*mem->value.integer_value=BINgetint(mainbin);
				z=BINgetint(mainbin);
				for(y=0;y<z;++y)
				{
					test=BINgetshort(mainbin);
					if(test)
					{
						temp=BINgetstring(mainbin);
						if(temp!=NULL) Rfree(temp);
					} 
				}
				break;
#ifdef USING_QT
			case PROGRESS_BAR:
#endif
			case LONGV:
			case SLONGV:
				mem->value.integer_value=Rmalloc(sizeof(int));
				*mem->value.integer_value=BINgetint(mainbin);
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s].",mem->field_type,mem->name,module,name);
				break;
		}
		++tmp->num;
	}
	if(mainbin!=NULL) BINfree(mainbin);
	return(tmp);
}
static void changelibname(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	char *libname=NULL,*name=NULL;

	if(HoldRsrc!=NULL) FreeHoldRDArsrc(HoldRsrc);
	readwidget(rsrc,"LIBRARY NAME");
	FINDRSCGETSTRING(rsrc,"LIBRARY NAME",&libname);
	readwidget(rsrc,"SCREEN NAME");
	FINDRSCGETSTRING(rsrc,"SCREEN NAME",&name);
	HoldRsrc=ReadRsrcList(libname,name);
	if(libname!=NULL) Rfree(libname);
	if(name!=NULL) Rfree(name);
	changewidgetlist(edit_rsrc);
}
void printscrndefcb(RDArsrc *prsrc,RDArsrc *rsrc)
{
	char *outdevice=NULL,*message=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;
	int lines=0,pages=0;

	readwidget(prsrc,"DEVICE");
        FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
			printscrndef(fp,screen,&lines,&pages);
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}
void printscrndef(RDA_PFILE *fp,RDAscrn *s,int *lines,int *pages)
{
	int x;
	RDAwdgt *w=NULL;
	char *temp=NULL;
/*
	char *date=NULL,*timex=NULL;
*/

	if(fp!=NULL)
	{
/*
		if(doheaders)
		{
			date=GETCURRENTDATE10();
			timex=GETCURRENTTIME();
			RDA_fprintf(fp,"%*sXPERT SCREEN RESOURCES AVAILABLE         Page: %*d\r\n",21,"",10,++pages);
			RDA_fprintf(fp,"%*s%s Module %s Screen%*sDate: %s\r\n",
				((80-(16+RDAstrlen(s->module)+RDAstrlen(s->name)))/2),"",
				s->module,s->name,
				(((80-(16+RDAstrlen(s->module)+RDAstrlen(s->name)))/2)-16),"",
				date);
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime:   %s\r\n\r\n",
				((80-(15+RDAstrlen(USERLOGIN)))/2),"",
				USERLOGIN,
				(((80-(15+RDAstrlen(USERLOGIN)))/2)-16),"",
				timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
		} else {
			RDA_fprintf(fp,"Module [%s] Screen [%s]\r\n",
				s->module,s->name);
		}
*/
		temp=stralloc("XPERT SCREEN DEFINITION");
		printheader(fp,lines,pages,temp,s->module);
		if(temp!=NULL) Rfree(temp);

		RDA_fprintf(fp,"Name: [%s]\r\n",(s->name!=NULL ? s->name:""));
		++*lines;
		checkheader(fp,lines,pages,s->module);

		for(x=0,w=s->wdgts;x<s->numwdgts;++x,++w)
		{
			switch(w->type)
			{
				case 15: /* custom input field */
				case 16: /* Expenditure field*/
				case 17: /* Revenue field*/
				case 18: /* Balance Sheet field*/
				case 19: /* Beginning Balance field*/
					RDA_fprintf(fp,"[%5d] [%s] Name: [%s] Label: [%s] Cols: [%d]\r\n",
						(x+1),wdgttypes[w->type],
						(w->resource_name!=NULL?w->resource_name:""),
						(w->label!=NULL?w->label:""),
						w->cols);
					++*lines;
					checkheader(fp,lines,pages,s->module);
					break;
				case 22: /* progress bar */
				case 0: /* standard resource */
					RDA_fprintf(fp,"[%5d] [%s] Name: [%s] Label: [%s] Cols: [%d]\r\n",
						(x+1),
						wdgttypes[w->type],
						(w->resource_name!=NULL?w->resource_name:""),
						(w->label!=NULL?w->label:""),
						w->cols);
					++*lines;
					checkheader(fp,lines,pages,s->module);
					break;
				case 1: /* new line */
				case 2: /* end line */
				case 3: /* new box */
				case 4: /* end box */
				case 10: /* frame */
				case 11: /* seperator */
				case 12: /* New Scrolled Window */
				case 13: /* End Scrolled Window */
				case 21: /* End Tab Container */
				case 25: /* End Popup Menu */
				case 27: /* End Toolbar Menu */
				case 29: /* End Table */
				case 31: /* End Header */
				case 30: /* New Header */
					RDA_fprintf(fp,"[%5d] [%s]\r\n",
						(x+1),wdgttypes[w->type]);
					++*lines;
					checkheader(fp,lines,pages,s->module);
					break;
				case 20: /* new tab container */
				case 24: /* New Popup Menu */
				case 26: /* New Toolbar Menu */
				case 23: /* tab bar */
				case 5: /* label */
				case 28: /* New Table */
					RDA_fprintf(fp,"[%5d] [%s] Label: [%s] Pixmap: [%s] XHTML label: [%s]\r\n",
						(x+1),wdgttypes[w->type],
						(w->label!=NULL?w->label:""),
						(w->pixmap!=NULL?w->pixmap:""),
						(w->XHTML_Label!=NULL?w->XHTML_Label:""));
					++*lines;
					checkheader(fp,lines,pages,s->module);
					break;
				case 14: /* Optional Screen */
				case 6: /* button */
				case 9: /* toggle button */
					RDA_fprintf(fp,"[%5d] [%s] Name: [%s] Label: [%s] Pixmap: [%s] XHTML label: [%s]\r\n",
						(x+1),wdgttypes[w->type],
						(w->resource_name!=NULL?w->resource_name:""),
						(w->label!=NULL?w->label:""),
						(w->pixmap!=NULL?w->pixmap:""),
						(w->XHTML_Label!=NULL?w->XHTML_Label:""));
					++*lines;
					checkheader(fp,lines,pages,s->module);
					break;
				case 7: /* scrolled list */
					RDA_fprintf(fp,"[%5d] [%s] Name: [%s] Label: [%s] Rows: [%d]\r\n",
						(x+1),wdgttypes[w->type],
						(w->resource_name!=NULL?w->resource_name:""),
						(w->label!=NULL ? w->label:""),w->rows);
					++*lines;
					checkheader(fp,lines,pages,s->module);
					break;
				case 8: /* scrolled text */
					RDA_fprintf(fp,"[%5d] [%s] Name: [%s] Label: [%s] Rows: [%d] Cols: [%d]\r\n",
						(x+1),wdgttypes[w->type],
						(w->resource_name!=NULL?w->resource_name:""),
						(w->label!=NULL ? w->label:""),w->rows,w->cols);
					++*lines;
					checkheader(fp,lines,pages,s->module);
					break;
				default:
					prterr("Error unrecognized widget type.");
					break;
			}
			if(!isEMPTY(w->expression) ||
				!isEMPTY(w->editable_expression)|| 
				!isEMPTY(w->sensitive_expression) ||
				!isEMPTY(w->transversal_expression)) 
			{
				RDA_fprintf(fp,"        Expressions: \r\n");
				++*lines;
				checkheader(fp,lines,pages,s->module);
				if(!isEMPTY(w->expression))
				{
					RDA_fprintf(fp,"          Visible : [%s]\r\n",
						w->expression);
					++*lines;
					checkheader(fp,lines,pages,s->module);
				}
				if(!isEMPTY(w->editable_expression))
				{
					RDA_fprintf(fp,"          Editable: [%s]\r\n",
						w->editable_expression);
					++*lines;
					checkheader(fp,lines,pages,s->module);
				}
				if(!isEMPTY(w->sensitive_expression))
				{
					RDA_fprintf(fp,"          Sensitive: [%s]\r\n",
						w->sensitive_expression);
					++*lines;
					checkheader(fp,lines,pages,s->module);
				}
				if(!isEMPTY(w->transversal_expression))
				{
					RDA_fprintf(fp,"          Transversal: [%s]\r\n",
						w->transversal_expression);
					++*lines;
					checkheader(fp,lines,pages,s->module);
				}
			}
		}
		RDA_fprintf(fp,"END SCREEN DEFINITION [%s]\r\n",
			(s->name!=NULL?s->name:""));
		++*lines;
	}
}
static void checkheader(RDA_PFILE *fp,int *lines,int *pages,char *modname)
{
	char *temp=NULL;

	if(doheaders)
	{
		if(*lines>57)
		{
			RDA_fprintf(fp,"\f");
			temp=stralloc("XPERT SCREEN DEFINITION");
			printheader(fp,lines,pages,temp,modname);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *header,char *modname)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\r\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",header,tail_ws,"",10,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(10+RDAstrlen(modname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(10+RDAstrlen(modname)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	RDA_fprintf(fp,"%*s Module: %s %*sDate: %s\r\n",
	lead_ws,"",(modname!=NULL?modname:""),tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\r\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	RDA_fprintf(fp,"\r\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void quit_print_definition(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print_definition Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void printdefinition(RDArsrc *mtn,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(diaggui)
	{
		prterr("DIAG printdefinition Displaying the Print Screen in preparation to print screen definition.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,mtn);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_definition,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_definition(prsrc);
	}
}
static void printmodule(RDArsrc *prsrc,RDArsrc *mainrsrc)
{
	int x=0;
	int l=0;
	RDAscrn *screen=NULL;
	char *dirx=NULL,*libx=NULL;
	char *outdevice=NULL,*message=NULL;
	RDA_PFILE *fp=NULL;
	int lines=0,pages=0;

        if(!RDAstrcmp(scr_defs->libs[0],"Contains No Screen Definitions")) return;
	readwidget(mainrsrc,"LIBRARY LIST");
	FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l);
        if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	readwidget(prsrc,"DEVICE");
        FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			for(x=0;x<scr_defs->numlibs;++x)
			{
				screen=RDAscrnNEW(scr_libs->libs[l],scr_defs->libs[x]);
        			libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+6);
#ifndef WIN32
        			sprintf(libx,"%s/%s.SCN",dirx,scr_libs->libs[l]);
#endif
#ifdef WIN32
        			sprintf(libx,"%s\\%s.SCN",dirx,scr_libs->libs[l]);
#endif
        			if(!getscrnbin(libx,screen,FALSE))
				{
					printscrndef(fp,screen,&lines,&pages);
					RDA_fprintf(fp,"\f");
				}
				if(screen!=NULL) free_scrn(screen);
       				if(libx!=NULL) Rfree(libx);
			}
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
       	if(dirx!=NULL) Rfree(dirx);
}
static dfscn *NEWdfscn()
{
	dfscn *tmp;

	tmp=Rmalloc(sizeof(dfscn));
	tmp->num=0;
	tmp->wdgts=NULL;
	return(tmp);
}
static void FREEdfscn(dfscn *tmp)
{
	short x;
	dfwdgt *wdgt;

	if(tmp!=NULL)
	{
		if(tmp->wdgts!=NULL)
		{
			for(x=0,wdgt=tmp->wdgts;x<tmp->num;++x,++wdgt)
			{
				if(wdgt->name!=NULL) Rfree(wdgt->name);
				if(wdgt->label!=NULL) Rfree(wdgt->label);	
				if(wdgt->pixmap!=NULL) Rfree(wdgt->pixmap);	
				if(wdgt->XHTML_Label!=NULL) Rfree(wdgt->XHTML_Label);	
				if(wdgt->expression!=NULL) Rfree(wdgt->expression);
				if(wdgt->editable_expression!=NULL) Rfree(wdgt->editable_expression);
				if(wdgt->sensitive_expression!=NULL) Rfree(wdgt->sensitive_expression);
				if(wdgt->transversal_expression!=NULL) Rfree(wdgt->transversal_expression);
			}
			Rfree(tmp->wdgts);
		}
		if(wlist!=NULL)
		{
			for(x=0;x<tmp->num;++x) if(wlist[x]!=NULL) Rfree(wlist[x]);
			Rfree(wlist);
			wlist=NULL;
		}
		Rfree(tmp);
		tmp=NULL;
	}
}
static void changenamefield(RDArsrc *edit_rsrc)
{
	char *temp=NULL;
	int x=0;

	readwidget(edit_rsrc,"WIDGET NAME");
	FINDRSCGETSTRING(edit_rsrc,"WIDGET NAME",&temp);
	if(!isEMPTY(temp))
	{
		for(x=0;x<RSRClist->numlibs;++x)
		{
			if(!RDAstrcmp(RSRClist->libs[x],temp)) break;
		}
		if(x<RSRClist->numlibs)
		{
			FINDRSCLISTAPPlib(edit_rsrc,"WIDGET NAMES",x,RSRClist);
		} else {
			x=RSRClist->numlibs;
			addAPPlib(RSRClist,temp);
			FINDRSCLISTAPPlib(edit_rsrc,"WIDGET NAMES",x,RSRClist);
		}
	} else {
		FINDRSCLISTAPPlib(edit_rsrc,"WIDGET NAMES",x,RSRClist);
		FINDRSCGETINT(edit_rsrc,"WIDGET NAMES",&x);
		if(RDAstrcmp(RSRClist->libs[x],"Not Available"))
		{
			FINDRSCSETSTRING(edit_rsrc,"WIDGET NAME",RSRClist->libs[x]);
			updatersrc(edit_rsrc,"WIDGET NAME");
		}
	}
	if(temp!=NULL) Rfree(temp);
	updatersrc(edit_rsrc,"WIDGET NAMES");
}
static void changenamelist(RDArsrc *edit_rsrc)
{
	int x;

	FINDRSCGETINT(edit_rsrc,"WIDGET NAMES",&x);
	if(RDAstrcmp(RSRClist->libs[x],"Not Available"))
	{
		FINDRSCSETSTRING(edit_rsrc,"WIDGET NAME",RSRClist->libs[x]);
	} else {
		FINDRSCSETSTRING(edit_rsrc,"WIDGET NAME",NULL);
	}
	updatersrc(edit_rsrc,"WIDGET NAME");
}
static void changewidgetlist(RDArsrc *edit_rsrc)
{
	int selected=0,selectedr=0,x;
	HoldRDArmem *mem;
	RDAacct *acc=NULL;

	if(FINDRSCGETINT(edit_rsrc,"WIDGET TYPES",&selected)) return;
	if(FINDRSCGETINT(edit_rsrc,"RESOURCE TYPES",&selectedr)) return;
	if(RSRClist!=NULL) freeapplib(RSRClist);
	RSRClist=APPlibNEW();
	FINDRSCSETSENSITIVE(edit_rsrc,"DFLIST",FALSE);
	FINDRSCSETSENSITIVE(edit_rsrc,"DFMENU",FALSE);
	FINDRSCSETSENSITIVE(edit_rsrc,"DFLOAD",FALSE);
	FINDRSCSETSENSITIVE(edit_rsrc,"DFVIR",FALSE);
	switch(selected)
	{
		default:
			break;
		case 7:
			FINDRSCSETSENSITIVE(edit_rsrc,"DFLIST",TRUE);
			break;
		case 6:
		case 14:
		case 15:
			if(Making_A_Menu==TRUE)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"DFMENU",TRUE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"DFLOAD",TRUE);
			}
			break;
		case 0:
			FINDRSCSETSENSITIVE(edit_rsrc,"DFVIR",TRUE);
			break;
	}	
	if(HoldRsrc!=NULL)
	{
		for(x=0,mem=HoldRsrc->rscs;x<HoldRsrc->num;++x,++mem)
		{
			if(selected==7 && mem->field_type==SCROLLEDLIST)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==8 && mem->field_type==SCROLLEDTEXT)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==9 && mem->field_type==BOOLNS)
			{
				addAPPlib(RSRClist,mem->name);
			} else if((selected==6 || selected==14) &&
				mem->field_type==BUTTONS)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==15 && mem->field_type==CUSTOMTYPE)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==16 && mem->field_type==EXPENDITURE)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==17 && mem->field_type==REVENUE)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==18 && mem->field_type==BALANCESHEET)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==19 && mem->field_type==BEGINNINGBALANCE)
			{
				addAPPlib(RSRClist,mem->name);
			} else if(selected==0 && (mem->field_type!=SCROLLEDLIST
				&& mem->field_type!=SCROLLEDTEXT &&
				mem->field_type!=BOOLNS &&
				mem->field_type!=BUTTONS &&
				mem->field_type!=CUSTOMTYPE))
			{
				addAPPlib(RSRClist,mem->name);
			}
		}
	}
	if(RSRClist->numlibs<1) addAPPlib(RSRClist,"Not Available");
	changenamefield(edit_rsrc);
	if(rsrclist!=NULL) freeapplib(rsrclist);
	rsrclist=APPlibNEW();
	switch(selected)
	{
		case 15: /* custom input fields */
			if(CUSTOM_INPUTS_ACCOUNT!=NULL)
			{
				if(CUSTOM_INPUTS_ACCOUNT->codetype!=NULL)
				{
					for(x=0,acc=CUSTOM_INPUTS_ACCOUNT->codetype;x<CUSTOM_INPUTS_ACCOUNT->num;++x,++acc)
					{
						addAPPlib(rsrclist,acc->name);
					}
				}
			}
			if(rsrclist->numlibs<1)
			{
				addAPPlib(rsrclist,"Not Applicable");
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",
					FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",
					TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 16: /* Expenditure fields */
			if(EXPENDITURE_ACCOUNT!=NULL)
			{
				if(EXPENDITURE_ACCOUNT->codetype!=NULL)
				{
					addAPPlib(rsrclist,"Dynamic Expenditure");
					for(x=0,acc=EXPENDITURE_ACCOUNT->codetype;x<EXPENDITURE_ACCOUNT->num;++x,++acc)
					{
						addAPPlib(rsrclist,acc->name);
					}
				}
			}
			if(rsrclist->numlibs<1)
			{
				addAPPlib(rsrclist,"Accounts Not Setup");
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 17: /* Revenue fields */
			if(REVENUE_ACCOUNT!=NULL)
			{
				if(REVENUE_ACCOUNT->codetype!=NULL)
				{
					addAPPlib(rsrclist,"Dynamic Revenue");
					for(x=0,acc=REVENUE_ACCOUNT->codetype;x<REVENUE_ACCOUNT->num;++x,++acc)
					{
						addAPPlib(rsrclist,acc->name);
					}
				}
			}
			if(rsrclist->numlibs<1)
			{
				addAPPlib(rsrclist,"Accounts Not Setup");
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 18: /* Balance Sheet fields */
			if(BALANCE_SHEET_ACCOUNT!=NULL)
			{
				if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
				{
					addAPPlib(rsrclist,"Dynamic Balance Sheet");
					for(x=0,acc=BALANCE_SHEET_ACCOUNT->codetype;x<BALANCE_SHEET_ACCOUNT->num;++x,++acc)
					{
						addAPPlib(rsrclist,acc->name);
					}
				}
			}
			if(rsrclist->numlibs<1)
			{
				addAPPlib(rsrclist,"Accounts Not Setup");
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 19: /* Beginning Balance fields */
			if(BEGINNING_BALANCE_ACCOUNT!=NULL)
			{
				if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
				{
					addAPPlib(rsrclist,"Dynamic Balance Sheet");
					for(x=0,acc=BEGINNING_BALANCE_ACCOUNT->codetype;x<BEGINNING_BALANCE_ACCOUNT->num;++x,++acc)
					{
						addAPPlib(rsrclist,acc->name);
					}
				}
			}
			if(rsrclist->numlibs<1)
			{
				addAPPlib(rsrclist,"Accounts Not Setup");
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 0: /* standard resource */
			addAPPlib(rsrclist,"None");
			addAPPlib(rsrclist,"Calendar");
			addAPPlib(rsrclist,"Pixmap");
			addAPPlib(rsrclist,"Password");
			addAPPlib(rsrclist,"Filename (csv,tsv,txt)");
			addAPPlib(rsrclist,"Filename (any)");
			addAPPlib(rsrclist,"Directory");
			addAPPlib(rsrclist,"Filename (Export/Create)");
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			break;
		case 12: /* start scrolled window */
			addAPPlib(rsrclist,"Not Applicable");
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",FALSE);
			break;
		case 28: /* Start Table */
			addAPPlib(rsrclist,"Number Rows in Header (Auto Stretch)");
			addAPPlib(rsrclist,"First Label Vertical Header (Auto Stretch)");
			addAPPlib(rsrclist,"Number Rows in Header");
			addAPPlib(rsrclist,"First Label Vertical Header");
#ifdef USE_SORT_COLUMNS
			addAPPlib(rsrclist,"Number Rows in Header Column Sort");
			addAPPlib(rsrclist,"First Label Vertical Header Column Sort");
#endif
			addAPPlib(rsrclist,"First Label Vertical Header w/Pie Chart");
			addAPPlib(rsrclist,"First Label Vertical Header w/Category Chart");
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 30: /* Start Header */
			addAPPlib(rsrclist,"Not Applicable");
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 1: /* new line */
		case 2: /* end line */
		case 3: /* new box */
		case 4: /* end box */
		case 13: /* end scrolled window */
		case 21: /* end tab container */
		case 25: /* End Popup Menu */
		case 27: /* End Toolbar Menu */
		case 29: /* End Table */
		case 31: /* End Header */
			addAPPlib(rsrclist,"Not Applicable");
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",FALSE);
			break;
		case 20: /* New Tab Container */
		case 5: /* label */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			if(selectedr==0)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			addAPPlib(rsrclist,"Text Label");
			addAPPlib(rsrclist,"Pixmap Label");
			addAPPlib(rsrclist,"RDA Logo");
			addAPPlib(rsrclist,"TuxCloud");
			addAPPlib(rsrclist,"XHTML");
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",FALSE);
			break;
		case 24: /* New Popup Menu */
		case 26: /* New Toolbar Menu */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			if(selectedr==0)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			addAPPlib(rsrclist,"Not Applicable");
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 23: /* Tab Bar */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			if(selectedr==0)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			addAPPlib(rsrclist,"Rounded    -> Top");
			addAPPlib(rsrclist,"Triangular -> Top");
			addAPPlib(rsrclist,"Rounded    -> Bottom");
			addAPPlib(rsrclist,"Triangular -> Bottom");
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 14: /* OPTIONAL SCREEN */
		case 6: /* button */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			if(selectedr==5)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			addAPPlib(rsrclist,"Push Button");
			addAPPlib(rsrclist,"Arrow Button Right");
			addAPPlib(rsrclist,"Arrow Button Left");
			addAPPlib(rsrclist,"Arrow Button Up");
			addAPPlib(rsrclist,"Arrow Button Down");
			addAPPlib(rsrclist,"Pixmap");
			addAPPlib(rsrclist,"Find");
			addAPPlib(rsrclist,"Printer");
			addAPPlib(rsrclist,"Redo");
			addAPPlib(rsrclist,"Undo");
			addAPPlib(rsrclist,"FileFind");
			addAPPlib(rsrclist,"Help");
			addAPPlib(rsrclist,"Search");
			addAPPlib(rsrclist,"Edit");
			addAPPlib(rsrclist,"Exit");
			addAPPlib(rsrclist,"Define List");
			addAPPlib(rsrclist,"Add / Create");
			addAPPlib(rsrclist,"Save Defaults");
			addAPPlib(rsrclist,"Reset Defaults");
			addAPPlib(rsrclist,"Cabinet");
			addAPPlib(rsrclist,"Mail Send");
			addAPPlib(rsrclist,"Scanner");
			addAPPlib(rsrclist,"Wizard");
			addAPPlib(rsrclist,"Save");
			addAPPlib(rsrclist,"Fast Save");
			addAPPlib(rsrclist,"Exec");
			addAPPlib(rsrclist,"Html");
			addAPPlib(rsrclist,"Pdf");
			addAPPlib(rsrclist,"Zip");
			addAPPlib(rsrclist,"Display");
			addAPPlib(rsrclist,"Archive");
			addAPPlib(rsrclist,"Folder");
			addAPPlib(rsrclist,"WWW");
			addAPPlib(rsrclist,"Delete/Trash");
			addAPPlib(rsrclist,"Configure");
			addAPPlib(rsrclist,"Look-n-Feel");
			addAPPlib(rsrclist,"View");
			addAPPlib(rsrclist,"View-Detailed");
			addAPPlib(rsrclist,"Main Menu");
			addAPPlib(rsrclist,"Open Source");
			addAPPlib(rsrclist,"GoogleMaps");
			addAPPlib(rsrclist,"Top");
			addAPPlib(rsrclist,"Bottom");
			addAPPlib(rsrclist,"Announcement");
			addAPPlib(rsrclist,"Chat");
			addAPPlib(rsrclist,"Download");
			addAPPlib(rsrclist,"Upload");
			addAPPlib(rsrclist,"News");
			addAPPlib(rsrclist,"Documentation");
			addAPPlib(rsrclist,"Copy");
			addAPPlib(rsrclist,"Video");
			addAPPlib(rsrclist,"Ksnapshot");
			addAPPlib(rsrclist,"Konqueror");
			addAPPlib(rsrclist,"Cybersupport");
			addAPPlib(rsrclist,"Save Update");
			addAPPlib(rsrclist,"Print Error");
			addAPPlib(rsrclist,"Print Warning");
			addAPPlib(rsrclist,"Add Above");
			addAPPlib(rsrclist,"Add Below");
			addAPPlib(rsrclist,"...");
			addAPPlib(rsrclist,"tuxCloud");
			addAPPlib(rsrclist,"FY...");
			addAPPlib(rsrclist,"CSV");
			addAPPlib(rsrclist,"Paperclip");

			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 7: /* scrolled list */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			addAPPlib(rsrclist,"Not Applicable");
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 22: /* progress bar */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",FALSE);
			addAPPlib(rsrclist,"Not Applicable");
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 9: /* toggle button */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			if(selectedr==0)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			}
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			addAPPlib(rsrclist,"Text Label");
			addAPPlib(rsrclist,"Pixmap Label");
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 8: /* scrolled text */
			addAPPlib(rsrclist,"WordWrap & No Horizontal SB");
			addAPPlib(rsrclist,"Horizontal SB & No WordWrap");
			addAPPlib(rsrclist,"Rich XHTML Text");
#ifdef USING_QT
#ifdef XXXX
			addAPPlib(rsrclist,"HTTP URL Richtext");
			addAPPlib(rsrclist,"FTP  URL Richtext");
#endif /* XXXX */
#endif /* USING_QT */
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",TRUE);
			break;
		case 10: /* frame */
			addAPPlib(rsrclist,"Shadow In");
			addAPPlib(rsrclist,"Shadow Out");
			addAPPlib(rsrclist,"Shadow Etched In");
			addAPPlib(rsrclist,"Shadow Etched Out");
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",FALSE);
			break;
		case 11: /* seperator */
			addAPPlib(rsrclist,"Horizontal Single Line");
			addAPPlib(rsrclist,"Horizontal Double Line");
			addAPPlib(rsrclist,"Horizontal Single Dashed");
			addAPPlib(rsrclist,"Horizontal Double Dashed");
			addAPPlib(rsrclist,"Vertical Single Line");
			addAPPlib(rsrclist,"Vertical Double Line");
			addAPPlib(rsrclist,"Vertical Single Dashed");
			addAPPlib(rsrclist,"Vertical Double Dashed");
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAME",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET NAMES",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"XHTML LABEL",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER ROWS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"NUMBER COLUMNS",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"RESOURCE TYPES",TRUE);
			FINDRSCSETSENSITIVE(edit_rsrc,"EDITABLE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"SENSITIVE EXPRESSION",FALSE);
			FINDRSCSETSENSITIVE(edit_rsrc,"TRANSVERSAL EXPRESSION",FALSE);
			break;
		default:
			prterr("Error unrecognized widget type.");
			break;
	}
	if(selectedr>=rsrclist->numlibs) selectedr=0;
	if(!FINDRSCLISTAPPlib(edit_rsrc,"RESOURCE TYPES",selectedr,rsrclist))
	{
		updatersrc(edit_rsrc,"RESOURCE TYPES");
	}
}
static void changersctype(RDArsrc *edit_rsrc)
{
	int selected=0,selectedr=0;

	if(FINDRSCGETINT(edit_rsrc,"WIDGET TYPES",&selected)) return;
	if(FINDRSCGETINT(edit_rsrc,"RESOURCE TYPES",&selectedr)) return;
	switch(selected)
	{
		case 15: /* custom input fields */
		case 16: /* expenditure fields */
		case 17: /* revenue fields */
		case 18: /* balance sheet fields */
		case 19: /* beginning balance fields */
		case 0: /* standard resource */
		case 12: /* start scrolled window */
		case 1: /* new line */
		case 2: /* end line */
		case 3: /* new box */
		case 4: /* end box */
		case 13: /* end scrolled window */
		case 7: /* scrolled list */
		case 8: /* scrolled text */
		case 10: /* frame */
		case 11: /* seperator */
		case 20: /* new tab container */
		case 21: /* end tab container */
		case 22: /* progress bar */
		case 23: /* tab bar */
		case 24: /* New Popup Menu */
		case 25: /* End Popup Menu */
		case 26: /* New Toolbar */
		case 27: /* End Toolbar */
		case 28: /* New Table */
		case 29: /* End Table */
		case 30: /* New Header */
		case 31: /* End Header */
			break;
		case 5: /* label */
			if(selectedr==0)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			}
			break;
		case 14: /* OPTIONAL SCREEN */
		case 6: /* button */
			if(selectedr==5)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			}
			break;
		case 9: /* toggle button */
			if(selectedr==0)
			{
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",FALSE);
			} else {
				FINDRSCSETSENSITIVE(edit_rsrc,"WIDGET PIXMAP",TRUE);
			}
			break;
		default:
			prterr("Error unrecognized widget type.");
			break;
	}
}
static void maketestlist()
{
	short x;
	int y=0;

	y=NE(wdgttypes);
	wdgtlist=Rmalloc(y*sizeof(char *));
	for(x=0;x<y;++x) wdgtlist[x]=stralloc(wdgttypes[x]);
}
static void freetestlist()
{
	short x;
	int y=0;

	if(wdgtlist!=NULL)
	{
		y=NE(wdgttypes);
		for(x=0;x<y;++x) Rfree(wdgtlist[x]);
		Rfree(wdgtlist);
		wdgtlist=NULL;
	}
}
static void makewlist(char ***wlist)
{
	short x,length=0;
	dfwdgt *w;

	if(*wlist!=NULL)
	{
		*wlist=Rrealloc(*wlist,dfscreen->num*sizeof(char *));
	} else *wlist=Rmalloc(dfscreen->num*sizeof(char *));
	*(wlist[0]+(dfscreen->num-1))=NULL;
	for(x=0,w=dfscreen->wdgts;x<dfscreen->num;++x,++w)
	{
		switch(w->type) /* obtain length */
		{
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* standard resource */
			case 22: /* progress bar */
				length=RDAstrlen(wdgttypes[w->type])+RDAstrlen(w->name)+RDAstrlen(w->label)+56+8;
				if(*(wlist[0]+x)!=NULL)
				{
					*(wlist[0]+x)=Rrealloc(*(wlist[0]+x),length);
				} else *(wlist[0]+x)=Rmalloc(length);
				sprintf(*(wlist[0]+x),"[%5d] [%s] Name: [%s] Label: [%s] Cols: [%d] Expression [%s]",
					(x+1),wdgttypes[w->type],(w->name!=NULL ?
					w->name:""),(w->label!=NULL ? w->label:""),
					w->cols,
					((!isEMPTY(w->expression)||!isEMPTY(w->editable_expression)||!isEMPTY(w->sensitive_expression)||!isEMPTY(w->transversal_expression))?"True":"False"));
				break;
			case 1: /* new line */
			case 2: /* end line */
			case 3: /* new box */
			case 4: /* end box */
			case 10: /* frame */
			case 11: /* seperator */
			case 12: /* New Scrolled Window */
			case 13: /* End Scrolled Window */
			case 21: /* end tab container */
			case 25: /* End Popup Menu */
			case 27: /* End Toolbar */
			case 29: /* End Table */
			case 31: /* End Header */
				length=RDAstrlen(wdgttypes[w->type])+24+8;
				if(*(wlist[0]+x)!=NULL)
				{
					*(wlist[0]+x)=Rrealloc(*(wlist[0]+x),
						length);
				} else *(wlist[0]+x)=Rmalloc(length);
				sprintf(*(wlist[0]+x),"[%5d] [%s] Expression [%s]",
					(x+1),wdgttypes[w->type],
					((!isEMPTY(w->expression)||!isEMPTY(w->editable_expression)||!isEMPTY(w->sensitive_expression)||!isEMPTY(w->transversal_expression))?"True":"False"));
				break;
			case 5: /* label */
			case 20: /* new tab container */
			case 23: /* tab bar */
			case 24: /* New Popup Menu */
			case 26: /* New Toolbar Menu */
			case 28: /* New Table */
			case 30: /* New Header */
				length=RDAstrlen(wdgttypes[w->type])+RDAstrlen(w->label)+34+8;
				if(*(wlist[0]+x)!=NULL)
				{
					*(wlist[0]+x)=Rrealloc(*(wlist[0]+x),length);
				} else *(wlist[0]+x)=Rmalloc(length);
				sprintf(*(wlist[0]+x),"[%5d] [%s] Label: [%s] Expression [%s]",
					(x+1),wdgttypes[w->type],
					(w->label!=NULL?w->label:""),
					((!isEMPTY(w->expression)||!isEMPTY(w->editable_expression)||!isEMPTY(w->sensitive_expression)||!isEMPTY(w->transversal_expression))?"True":"False"));
				break;
			case 14: /* Optional Screen */
			case 6: /* button */
			case 9: /* toggle button */
				length=RDAstrlen(wdgttypes[w->type])+RDAstrlen(w->label)+RDAstrlen(w->name)+44+8;
				if(*(wlist[0]+x)!=NULL) 
				{
					*(wlist[0]+x)=Rrealloc(*(wlist[0]+x),length);
				} else *(wlist[0]+x)=Rmalloc(length);
				sprintf(*(wlist[0]+x),
					"[%5d] [%s] Name: [%s] Label: [%s] Expression [%s]",
					(x+1),wdgttypes[w->type],
					(w->name!=NULL?w->name:""),
					(w->label!=NULL?w->label:""),
					((!isEMPTY(w->expression)||!isEMPTY(w->editable_expression)||!isEMPTY(w->sensitive_expression)||!isEMPTY(w->transversal_expression))?"True":"False"));
				break;
			case 7: /* scrolled list */
				length=RDAstrlen(wdgttypes[w->type])+RDAstrlen(w->name)+RDAstrlen(w->label)+57+8;
				if(*(wlist[0]+x)!=NULL) 
				{
					*(wlist[0]+x)=Rrealloc(*(wlist[0]+x),length);
				} else *(wlist[0]+x)=Rmalloc(length);
				sprintf(*(wlist[0]+x),"[%5d] [%s] Name: [%s] Label: [%s] Rows: [%d] Expression [%s]",
					(x+1),wdgttypes[w->type],
					(w->name!=NULL?w->name:""),
					(w->label!=NULL?w->label:""),w->rows,
					((!isEMPTY(w->expression)||!isEMPTY(w->editable_expression)||!isEMPTY(w->sensitive_expression)||!isEMPTY(w->transversal_expression))?"True":"False"));
				break;
			case 8: /* scrolled text */
				length=RDAstrlen(wdgttypes[w->type])+70+RDAstrlen(w->name)+RDAstrlen(w->label)+8;
				if(*(wlist[0]+x)!=NULL) 
				{
					*(wlist[0]+x)=Rrealloc(*(wlist[0]+x),length);
				} else *(wlist[0]+x)=Rmalloc(length);
				sprintf(*(wlist[0]+x),"[%5d] [%s] Name: [%s] Label: [%s] Rows: [%d] Cols: [%d] Expression [%s]",
					(x+1),wdgttypes[w->type],
					(w->name!=NULL?w->name:""),
					(w->label!=NULL?w->label:""),
					w->rows,w->cols,
					((!isEMPTY(w->expression)||!isEMPTY(w->editable_expression)||!isEMPTY(w->sensitive_expression)||!isEMPTY(w->transversal_expression))?"True":"False"));
				break;
			default:
				prterr("Error unrecognized widget type [%d].",w->type);
				break;
		}
	}
}
static void doexit(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	freetestlist();
	if(scr_libs!=NULL) freeapplib(scr_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	if(rsrclist!=NULL) freeapplib(rsrclist);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
	if(rsrcdefaults!=NULL) FreeRDAdefault(rsrcdefaults);
	if(screen!=NULL) { free_scrn(screen); screen=NULL; }
	if(HoldRsrc!=NULL) FreeHoldRDArsrc(HoldRsrc);
	ShutdownSubsystems();
	std::exit;
}
static void doexit1(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{	
	if(RSRClist!=NULL) freeapplib(RSRClist);
	if(rsrc!=NULL)
	{
		killwindow(rsrc);
		free_rsrc(rsrc); 
	}
	if(edit_rsrc!=NULL)
	{
		killwindow(edit_rsrc);
		free_rsrc(edit_rsrc); 
	}
	if(dfscreen!=NULL) FREEdfscn(dfscreen);
	dfscreen=NULL;
	changedvalues=FALSE;
	if(rsrcdefaults!=NULL) FreeRDAdefault(rsrcdefaults);
}
static void doexit1test(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	readallwidgets(rsrc);
	if(COMPARE_RSRCS(rsrc,rsrcdefaults,0) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this SCREEN DEFINITION.\nDo you want to Save these changes?",save_screen,doexit1,FALSE,2,rsrc,edit_rsrc,NULL);
	} else {
		doexit1(rsrc,edit_rsrc);
	}
}
static void makespace(int pos)
{
	short x,y;
	dfwdgt *temp,*new_wdgt,*temp1;

	++dfscreen->num;
	new_wdgt=Rmalloc(dfscreen->num*sizeof(dfwdgt));
	y=0;
	for(x=0,temp=dfscreen->wdgts;x<(dfscreen->num-1);++x,++temp)
	{
		temp1=new_wdgt+y;
		if(x==pos)
		{
			temp1->type=0;
			temp1->name=NULL;
			temp1->label=NULL;
			temp1->pixmap=NULL;
			temp1->XHTML_Label=NULL;
			temp1->rows=0;
			temp1->cols=0;
			temp1->rtype=0;
			temp1->expression=NULL;
			temp1->editable_expression=NULL;
			temp1->sensitive_expression=NULL;
			temp1->transversal_expression=NULL;
			++y;
			temp1=new_wdgt+y;
		}
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->label!=NULL) 
		{
			temp1->label=stralloc(temp->label);
			Rfree(temp->label);
		} else temp1->label=NULL;
		if(temp->pixmap!=NULL) 
		{
			temp1->pixmap=stralloc(temp->pixmap);
			Rfree(temp->pixmap);
		} else temp1->pixmap=NULL;
		if(temp->XHTML_Label!=NULL) 
		{
			temp1->XHTML_Label=stralloc(temp->XHTML_Label);
			Rfree(temp->XHTML_Label);
		} else temp1->XHTML_Label=NULL;
		temp1->rows=temp->rows;
		temp1->cols=temp->cols;
		temp1->rtype=temp->rtype;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->editable_expression!=NULL)
		{
			temp1->editable_expression=stralloc(temp->editable_expression);
			Rfree(temp->editable_expression);
		} else temp1->editable_expression=NULL;
		if(temp->sensitive_expression!=NULL)
		{
			temp1->sensitive_expression=stralloc(temp->sensitive_expression);
			Rfree(temp->sensitive_expression);
		} else temp1->sensitive_expression=NULL;
		if(temp->transversal_expression!=NULL)
		{
			temp1->transversal_expression=stralloc(temp->transversal_expression);
			Rfree(temp->transversal_expression);
		} else temp1->transversal_expression=NULL;
		++y;
	}
	Rfree(dfscreen->wdgts);
	dfscreen->wdgts=Rmalloc(dfscreen->num*sizeof(dfwdgt));
	for(x=0,temp1=dfscreen->wdgts,temp=new_wdgt;x<dfscreen->num;++x,++temp,
		++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
			temp->name=NULL;
		} else temp1->name=NULL;
		if(temp->label!=NULL)
		{
			temp1->label=stralloc(temp->label);
			Rfree(temp->label);
			temp->label=NULL;
		} else temp1->label=NULL;
		if(temp->pixmap!=NULL)
		{
			temp1->pixmap=stralloc(temp->pixmap);
			Rfree(temp->pixmap);
			temp->pixmap=NULL;
		} else temp1->pixmap=NULL;
		if(temp->XHTML_Label!=NULL)
		{
			temp1->XHTML_Label=stralloc(temp->XHTML_Label);
			Rfree(temp->XHTML_Label);
			temp->XHTML_Label=NULL;
		} else temp1->XHTML_Label=NULL;
		temp1->rows=temp->rows;
		temp1->cols=temp->cols;
		temp1->rtype=temp->rtype;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->editable_expression!=NULL)
		{
			temp1->editable_expression=stralloc(temp->editable_expression);
			Rfree(temp->editable_expression);
		} else temp1->editable_expression=NULL;
		if(temp->sensitive_expression!=NULL)
		{
			temp1->sensitive_expression=stralloc(temp->sensitive_expression);
			Rfree(temp->sensitive_expression);
		} else temp1->sensitive_expression=NULL;
		if(temp->transversal_expression!=NULL)
		{
			temp1->transversal_expression=stralloc(temp->transversal_expression);
			Rfree(temp->transversal_expression);
		} else temp1->transversal_expression=NULL;
	}
	Rfree(new_wdgt);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int selected,selecteds=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"SCREEN LIST",&selecteds);
	freeapplib(scr_defs);
	if(!RDAstrcmp(scr_libs->libs[selected],"Directory Contains No Libraries"))
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Screen Definitions");
		selecteds=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[selected])+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.SCN",dirx,scr_libs->libs[selected]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.SCN",dirx,scr_libs->libs[selected]);
#endif
		scr_defs=getlibnames(libx,FALSE);
		if(scr_defs==NULL)
		{
			scr_defs=APPlibNEW();
			addAPPlib(scr_defs,"Contains No Screen Definitions");
			selecteds=0;
		} 
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(selecteds>=scr_defs->numlibs) selecteds=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN LIST",selecteds,scr_defs))
		updatersrc(mainrsrc,"SCREEN LIST");
}
static void save_widget(RDArsrc *edit_rsrc,RDArsrc *rsrc)
{
	int l;
	dfwdgt *wdgt;

	readallwidgets(edit_rsrc);
	if(FINDRSCGETINT(rsrc,"WIDGET LIST",&l)) return;
	wdgt=dfscreen->wdgts+l;
	if(FINDRSCGETINT(edit_rsrc,"WIDGET TYPES",&wdgt->type)) return;
	FINDRSCGETSTRING(edit_rsrc,"EXPRESSION",&wdgt->expression);
	if(wdgt->type==0 || wdgt->type==6 || wdgt->type==7 || wdgt->type==8 || 
		wdgt->type==9 || wdgt->type==14 || wdgt->type==15 || 
		wdgt->type==16 || wdgt->type==17 || wdgt->type==18 || 
		wdgt->type==19 || wdgt->type==22 || wdgt->type==20 || wdgt->type==28 || wdgt->type==30)
	{
		FINDRSCGETSTRING(edit_rsrc,"WIDGET NAME",&wdgt->name);
		FINDRSCGETSTRING(edit_rsrc,"EDITABLE EXPRESSION",&wdgt->editable_expression);
		FINDRSCGETSTRING(edit_rsrc,"SENSITIVE EXPRESSION",&wdgt->sensitive_expression);
		FINDRSCGETSTRING(edit_rsrc,"TRANSVERSAL EXPRESSION",&wdgt->transversal_expression);
	} else {
		if(wdgt->name!=NULL) Rfree(wdgt->name);
		wdgt->name=NULL;
		if(wdgt->editable_expression!=NULL) Rfree(wdgt->editable_expression);
		wdgt->editable_expression=NULL;
		if(wdgt->sensitive_expression!=NULL) Rfree(wdgt->sensitive_expression);
		wdgt->sensitive_expression=NULL;
		if(wdgt->transversal_expression!=NULL) Rfree(wdgt->transversal_expression);
		wdgt->transversal_expression=NULL;
	}
	if(wdgt->type==0 || wdgt->type==5 || wdgt->type==6 || wdgt->type==7 || 
		wdgt->type==8 || wdgt->type==9 || wdgt->type==14 || 
		wdgt->type==15 || wdgt->type==16 || wdgt->type==17 || 
		wdgt->type==18 || wdgt->type==19 || wdgt->type==20 || wdgt->type==22 || 
		wdgt->type==23 || wdgt->type==24 || wdgt->type==26 || wdgt->type==28 || wdgt->type==30)
	{
		FINDRSCGETSTRING(edit_rsrc,"WIDGET LABEL",&wdgt->label);
	} else {
		if(wdgt->label!=NULL) Rfree(wdgt->label);
		wdgt->label=NULL;
	}
	if(wdgt->type==9 || wdgt->type==5 || wdgt->type==6 || wdgt->type==14 || wdgt->type==20 || wdgt->type==23)
	{
		FINDRSCGETSTRING(edit_rsrc,"WIDGET PIXMAP",&wdgt->pixmap);
	} else {
		if(wdgt->pixmap!=NULL) Rfree(wdgt->pixmap);
		wdgt->pixmap=NULL;
	}
	if(wdgt->type==9 || wdgt->type==5 || wdgt->type==6 || wdgt->type==14 || wdgt->type==20 || wdgt->type==24 || wdgt->type==26 || wdgt->type==28)
	{
		FINDRSCGETSTRING(edit_rsrc,"XHTML LABEL",&wdgt->XHTML_Label);
	} else {
		if(wdgt->XHTML_Label!=NULL) Rfree(wdgt->XHTML_Label);
		wdgt->XHTML_Label=NULL;
	}
	if(wdgt->type==7 || wdgt->type==8 || wdgt->type==12)
	{
		FINDRSCGETSHORT(edit_rsrc,"NUMBER ROWS",&wdgt->rows);
	} else wdgt->rows=0;
	if(wdgt->type==8 || wdgt->type==0 || wdgt->type==15 || 
		wdgt->type==12 || wdgt->type==16 || wdgt->type==17 || 
		wdgt->type==18 || wdgt->type==19)
	{
		FINDRSCGETSHORT(edit_rsrc,"NUMBER COLUMNS",&wdgt->cols);
	} else wdgt->cols=0;
	if(wdgt->type==6 || wdgt->type==8 || wdgt->type==10 || 
		wdgt->type==11 || wdgt->type==14 || wdgt->type==15 || 
		wdgt->type==9 || wdgt->type==5 || wdgt->type==16 || 
		wdgt->type==17 || wdgt->type==18 || wdgt->type==19 || wdgt->type==0 || wdgt->type==22 || wdgt->type==23 || wdgt->type==28)
	{
		FINDRSCGETINT(edit_rsrc,"RESOURCE TYPES",&wdgt->rtype);
	} else wdgt->rtype=0;
	makewlist(&wlist);
	if(!FINDRSCSETLIST(rsrc,"WIDGET LIST",l,dfscreen->num,&wlist))
	{
		updatersrc(rsrc,"WIDGET LIST");
	}
	if(COMPARE_RSRCS(edit_rsrc,editdefaults,2))
	{
		changedvalues=TRUE;
	}
	quit_widget(edit_rsrc,rsrc);
}
static void quit_widget(RDArsrc *edit_rsrc,RDArsrc *rsrc)
{
	if(edit_rsrc!=NULL)
	{
		killwindow(edit_rsrc);
		NULL_RSCS(edit_rsrc);
	}
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
}
static void quit_widgettest(RDArsrc *edit_rsrc,RDArsrc *rsrc)
{
	readallwidgets(edit_rsrc);
	if(COMPARE_RSRCS(edit_rsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the WIDGET DEFINITION.\nDo you want to Save these changes?",save_widget,quit_widget,FALSE,2,edit_rsrc,rsrc,NULL);
	} else {
		changedvalues=FALSE;
		quit_widget(edit_rsrc,rsrc);
	}
}
static void save_widget_above(RDArsrc *edit_rsrc,RDArsrc *rsrc)
{
	int l;

	if(FINDRSCGETINT(rsrc,"WIDGET LIST",&l)) return;
	makespace(l);
	save_widget(edit_rsrc,rsrc);
}
static void quit_widget_abovetest(RDArsrc *edit_rsrc,RDArsrc *rsrc)
{
	readallwidgets(edit_rsrc);
	if(COMPARE_RSRCS(edit_rsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the WIDGET DEFINITION.\nDo you want to Save these changes?",save_widget_above,quit_widget,FALSE,2,edit_rsrc,rsrc,NULL);
	} else {
		changedvalues=FALSE;
		quit_widget(edit_rsrc,rsrc);
	}
}
static void save_widget_below(RDArsrc *edit_rsrc,RDArsrc *rsrc)
{
	int l;

	if(FINDRSCGETINT(rsrc,"WIDGET LIST",&l)) return;
	makespace(++l);
	FINDRSCSETINT(rsrc,"WIDGET LIST",l);
	updatersrc(rsrc,"WIDGET LIST");
	save_widget(edit_rsrc,rsrc);
}
static void quit_widget_belowtest(RDArsrc *edit_rsrc,RDArsrc *rsrc)
{
	readallwidgets(edit_rsrc);
	if(COMPARE_RSRCS(edit_rsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the WIDGET DEFINITION.\nDo you want to Save these changes?",save_widget_below,quit_widget,FALSE,2,edit_rsrc,rsrc,NULL);
	} else {
		changedvalues=FALSE;
		quit_widget(edit_rsrc,rsrc);
	}
}
static void edit_current(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	dfwdgt *wdgt;
	char *name=NULL,*libname=NULL;
	int l,x;
	
	if(FINDRSCSETFUNC(edit_rsrc,"SAVE",save_widget,rsrc)) return;
	readallwidgets(rsrc);
	if(FINDRSCGETSTRING(rsrc,"LIBRARY NAME",&libname))
	{
		FINDRSCSETSTRING(edit_rsrc,"LIBRARY NAME","");
	} else FINDRSCSETSTRING(edit_rsrc,"LIBRARY NAME",libname);
	if(libname!=NULL) Rfree(libname);
	if(!FINDRSCGETSTRING(rsrc,"SCREEN NAME",&name))
	{
		FINDRSCSETSTRING(edit_rsrc,"SCREEN NAME",name);
	} else FINDRSCSETSTRING(edit_rsrc,"SCREEN NAME","");
	if(name!=NULL) Rfree(name);
	if(!FINDRSCGETINT(rsrc,"WIDGET LIST",&l))
	{
		wdgt=dfscreen->wdgts+l;
		FINDRSCSETINT(edit_rsrc,"WIDGET TYPES",wdgt->type);
		FINDRSCSETSTRING(edit_rsrc,"WIDGET NAME",(wdgt->name!=NULL ?
			wdgt->name:""));
		FINDRSCSETSTRING(edit_rsrc,"WIDGET LABEL",(wdgt->label!=NULL ?
			wdgt->label:""));
		FINDRSCSETSTRING(edit_rsrc,"WIDGET PIXMAP",(wdgt->pixmap!=NULL ?
			wdgt->pixmap:""));
		FINDRSCSETSTRING(edit_rsrc,"XHTML LABEL",(wdgt->XHTML_Label!=NULL ?
			wdgt->XHTML_Label:""));
		FINDRSCSETSHORT(edit_rsrc,"NUMBER ROWS",wdgt->rows);
		FINDRSCSETSHORT(edit_rsrc,"NUMBER COLUMNS",wdgt->cols);
		FINDRSCSETINT(edit_rsrc,"RESOURCE TYPES",wdgt->rtype);
		FINDRSCSETSTRING(edit_rsrc,"EXPRESSION",wdgt->expression);
		FINDRSCSETSTRING(edit_rsrc,"EDITABLE EXPRESSION",wdgt->editable_expression);
		FINDRSCSETSTRING(edit_rsrc,"SENSITIVE EXPRESSION",wdgt->sensitive_expression);
		FINDRSCSETSTRING(edit_rsrc,"TRANSVERSAL EXPRESSION",wdgt->transversal_expression);
	}
	x=0;
	FINDRSCLISTAPPlib(edit_rsrc,"WIDGET NAMES",x,RSRClist);
	changewidgetlist(edit_rsrc);
	APPmakescrn(edit_rsrc,TRUE,NULL,NULL,FALSE);
	editdefaults=GetDefaults(edit_rsrc);
} 
static void copy_widget(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	dfwdgt *wdgt;
	char *name=NULL,*libname=NULL;
	int l,x;
	
	if(FINDRSCSETFUNC(edit_rsrc,"SAVE",save_widget_below,rsrc)) return;
	if(FINDRSCSETFUNC(edit_rsrc,"QUIT",quit_widget_belowtest,rsrc)) return;
	readallwidgets(rsrc);
	if(FINDRSCGETSTRING(rsrc,"LIBRARY NAME",&libname))
	{
		FINDRSCSETSTRING(edit_rsrc,"LIBRARY NAME","");
	} else FINDRSCSETSTRING(edit_rsrc,"LIBRARY NAME",libname);
	if(libname!=NULL) Rfree(libname);
	if(!FINDRSCGETSTRING(rsrc,"SCREEN NAME",&name))
	{
		FINDRSCSETSTRING(edit_rsrc,"SCREEN NAME",name);
	} else FINDRSCSETSTRING(edit_rsrc,"SCREEN NAME","");
	if(name!=NULL) Rfree(name);
	if(!FINDRSCGETINT(rsrc,"WIDGET LIST",&l))
	{
		wdgt=dfscreen->wdgts+l;
		FINDRSCSETINT(edit_rsrc,"WIDGET TYPES",wdgt->type);
		FINDRSCSETSTRING(edit_rsrc,"WIDGET NAME",(wdgt->name!=NULL ?
			wdgt->name:""));
		FINDRSCSETSTRING(edit_rsrc,"WIDGET LABEL",(wdgt->label!=NULL ?
			wdgt->label:""));
		FINDRSCSETSTRING(edit_rsrc,"WIDGET PIXMAP",(wdgt->pixmap!=NULL ?
			wdgt->pixmap:""));
		FINDRSCSETSTRING(edit_rsrc,"XHTML LABEL",(wdgt->XHTML_Label!=NULL ?
			wdgt->XHTML_Label:""));
		FINDRSCSETSHORT(edit_rsrc,"NUMBER ROWS",wdgt->rows);
		FINDRSCSETSHORT(edit_rsrc,"NUMBER COLUMNS",wdgt->cols);
		FINDRSCSETINT(edit_rsrc,"RESOURCE TYPES",wdgt->rtype);
		FINDRSCSETSTRING(edit_rsrc,"EXPRESSION",wdgt->expression);
		FINDRSCSETSTRING(edit_rsrc,"EDITABLE EXPRESSION",wdgt->editable_expression);
		FINDRSCSETSTRING(edit_rsrc,"SENSITIVE EXPRESSION",wdgt->sensitive_expression);
		FINDRSCSETSTRING(edit_rsrc,"TRANSVERSAL EXPRESSION",wdgt->transversal_expression);
	}
	x=0;
	FINDRSCLISTAPPlib(edit_rsrc,"WIDGET NAMES",x,RSRClist);
	changewidgetlist(edit_rsrc);
	APPmakescrn(edit_rsrc,TRUE,NULL,NULL,FALSE);
	editdefaults=GetDefaults(edit_rsrc);
} 
static void add(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	int x=0;
	short l=0;
	char *name=NULL,*libname=NULL;

	readallwidgets(rsrc);
	if(!FINDRSCGETSTRING(rsrc,"LIBRARY NAME",&libname))
	{
		FINDRSCSETSTRING(edit_rsrc,"LIBRARY NAME",libname);
	} else  FINDRSCSETSTRING(edit_rsrc,"LIBRARY NAME","");
	if(libname!=NULL) Rfree(libname);
	if(!FINDRSCGETSTRING(rsrc,"SCREEN NAME",&name))
	{
		FINDRSCSETSTRING(edit_rsrc,"SCREEN NAME",(name!=NULL ? name:""));
	} else FINDRSCSETSTRING(edit_rsrc,"SCREEN NAME","");
	x=0;
	if(name!=NULL) Rfree(name);
	FINDRSCSETINT(edit_rsrc,"WIDGET TYPES",x);
	if(!RDAstrcmp(RSRClist->libs[0],"Not Available"))
	{
		FINDRSCSETSTRING(edit_rsrc,"WIDGET NAME",NULL);
	} else {
		FINDRSCSETSTRING(edit_rsrc,"WIDGET NAME",RSRClist->libs[0]);
	}
	FINDRSCSETSTRING(edit_rsrc,"WIDGET LABEL","");
	FINDRSCSETSTRING(edit_rsrc,"WIDGET PIXMAP","");
	FINDRSCSETSTRING(edit_rsrc,"XHTML LABEL","");
	FINDRSCSETSHORT(edit_rsrc,"NUMBER ROWS",l);
	FINDRSCSETSHORT(edit_rsrc,"NUMBER COLUMNS",l);
	FINDRSCSETINT(edit_rsrc,"RESOURCE TYPES",x);
	FINDRSCSETSTRING(edit_rsrc,"EXPRESSION","");
	FINDRSCSETSTRING(edit_rsrc,"EDITABLE EXPRESSION","");
	FINDRSCSETSTRING(edit_rsrc,"SENSITIVE EXPRESSION","");
	FINDRSCSETSTRING(edit_rsrc,"TRANSVERSAL EXPRESSION","");
	changewidgetlist(edit_rsrc);
	editdefaults=GetDefaults(edit_rsrc);
	APPmakescrn(edit_rsrc,TRUE,NULL,NULL,FALSE);
}
static void add_below(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	if(FINDRSCSETFUNC(edit_rsrc,"SAVE",save_widget_below,rsrc)) return;
	if(FINDRSCSETFUNC(edit_rsrc,"QUIT",quit_widget_belowtest,rsrc)) return;
	add(rsrc,edit_rsrc);
} 
static void add_above(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	if(FINDRSCSETFUNC(edit_rsrc,"SAVE",save_widget_above,rsrc)) return;
	if(FINDRSCSETFUNC(edit_rsrc,"QUIT",quit_widget_abovetest,rsrc)) return;
	add(rsrc,edit_rsrc);
} 
static void scnmove(RDArsrc *rsrc,short direction)
{
	/* A direction of 0 means up, 1 means down */
	int selected=0;
	dfwdgt *temp,*temp1,*temp2;

	if(FINDRSCGETINT(rsrc,"WIDGET LIST",&selected)) return;
	temp=dfscreen->wdgts+selected;
	if(direction)
	{
		if((temp==NULL) || (selected >= (dfscreen->num-1))) return;
		selected++;
	} else {
		if((temp==NULL) || (selected <=0)) return;
		selected--;
	}
	temp2=dfscreen->wdgts+selected;
	temp1=Rmalloc(sizeof(dfwdgt));

	/* STEP 1 */
	temp1->type=temp->type;
	if(temp->name!=NULL)
	{
		temp1->name=stralloc(temp->name);
		Rfree(temp->name);
	} else temp1->name=NULL;
	if(temp->label!=NULL) 
	{
		temp1->label=stralloc(temp->label);
		Rfree(temp->label);
	} else temp1->label=NULL;
	if(temp->pixmap!=NULL) 
	{
		temp1->pixmap=stralloc(temp->pixmap);
		Rfree(temp->pixmap);
	} else temp1->pixmap=NULL;
	if(temp->XHTML_Label!=NULL) 
	{
		temp1->XHTML_Label=stralloc(temp->XHTML_Label);
		Rfree(temp->XHTML_Label);
	} else temp1->XHTML_Label=NULL;
	temp1->rows=temp->rows;
	temp1->cols=temp->cols;
	temp1->rtype=temp->rtype;
	if(temp->expression!=NULL)
	{
		temp1->expression=stralloc(temp->expression);
		Rfree(temp->expression);
	} else temp1->expression=NULL;
	if(temp->editable_expression!=NULL)
	{
		temp1->editable_expression=stralloc(temp->editable_expression);
		Rfree(temp->editable_expression);
	} else temp1->editable_expression=NULL;
	if(temp->sensitive_expression!=NULL)
	{
		temp1->sensitive_expression=stralloc(temp->sensitive_expression);
		Rfree(temp->sensitive_expression);
	} else temp1->sensitive_expression=NULL;
	if(temp->transversal_expression!=NULL)
	{
		temp1->transversal_expression=stralloc(temp->transversal_expression);
		Rfree(temp->transversal_expression);
	} else temp1->transversal_expression=NULL;

	/* STEP 2 */
	temp->type=temp2->type;
	if(temp2->name!=NULL)
	{
		temp->name=stralloc(temp2->name);
		Rfree(temp2->name);
	} else temp->name=NULL;
	if(temp2->label!=NULL)
	{
		temp->label=stralloc(temp2->label);
		Rfree(temp2->label);
	} else temp->label=NULL;
	if(temp2->pixmap!=NULL)
	{
		temp->pixmap=stralloc(temp2->pixmap);
		Rfree(temp2->pixmap);
	} else temp->pixmap=NULL;
	if(temp2->XHTML_Label!=NULL)
	{
		temp->XHTML_Label=stralloc(temp2->XHTML_Label);
		Rfree(temp2->XHTML_Label);
	} else temp->XHTML_Label=NULL;
	temp->rows=temp2->rows;
	temp->cols=temp2->cols;
	temp->rtype=temp2->rtype;
	if(temp2->expression!=NULL)
	{
		temp->expression=stralloc(temp2->expression);
		Rfree(temp2->expression);
	} else temp->expression=NULL;
	if(temp2->editable_expression!=NULL)
	{
		temp->editable_expression=stralloc(temp2->editable_expression);
		Rfree(temp2->editable_expression);
	} else temp->editable_expression=NULL;
	if(temp2->sensitive_expression!=NULL)
	{
		temp->sensitive_expression=stralloc(temp2->sensitive_expression);
		Rfree(temp2->sensitive_expression);
	} else temp->sensitive_expression=NULL;
	if(temp2->transversal_expression!=NULL)
	{
		temp->transversal_expression=stralloc(temp2->transversal_expression);
		Rfree(temp2->transversal_expression);
	} else temp->transversal_expression=NULL;

	/* STEP 3 */
	temp2->type=temp1->type;
	if(temp1->name!=NULL)
	{
		temp2->name=stralloc(temp1->name);
		Rfree(temp1->name);
	} else temp2->name=NULL;
	if(temp1->label!=NULL)
	{
		temp2->label=stralloc(temp1->label);
		Rfree(temp1->label);
	} else temp2->label=NULL;
	if(temp1->pixmap!=NULL)
	{
		temp2->pixmap=stralloc(temp1->pixmap);
		Rfree(temp1->pixmap);
	} else temp2->pixmap=NULL;
	if(temp1->XHTML_Label!=NULL)
	{
		temp2->XHTML_Label=stralloc(temp1->XHTML_Label);
		Rfree(temp1->XHTML_Label);
	} else temp2->XHTML_Label=NULL;
	temp2->rows=temp1->rows;
	temp2->cols=temp1->cols;
	temp2->rtype=temp1->rtype;
	if(temp1->expression!=NULL)
	{
		temp2->expression=stralloc(temp1->expression);
		Rfree(temp1->expression);
	} else temp2->expression=NULL;
	if(temp1->editable_expression!=NULL)
	{
		temp2->editable_expression=stralloc(temp1->editable_expression);
		Rfree(temp1->editable_expression);
	} else temp2->editable_expression=NULL;
	if(temp1->sensitive_expression!=NULL)
	{
		temp2->sensitive_expression=stralloc(temp1->sensitive_expression);
		Rfree(temp1->sensitive_expression);
	} else temp2->sensitive_expression=NULL;
	if(temp1->transversal_expression!=NULL)
	{
		temp2->transversal_expression=stralloc(temp1->transversal_expression);
		Rfree(temp1->transversal_expression);
	} else temp2->transversal_expression=NULL;

	Rfree(temp1);
	makewlist(&wlist);
	if(selected>=dfscreen->num) selected=dfscreen->num-1;
	if(!FINDRSCSETLIST(rsrc,"WIDGET LIST",selected,dfscreen->num,&wlist))
	{
		updatersrc(rsrc,"WIDGET LIST");
	}
	changedvalues=TRUE;
}
static void scnmoveup(RDArsrc *rsrc)
{
	scnmove(rsrc,0);
}
static void scnmovedown(RDArsrc *rsrc)
{
	scnmove(rsrc,1);
}
static void delete_widget(RDArsrc *rsrc)
{
	dfwdgt *temp,*new_wdgt,*temp1;
	int x=0,g,y,l;

	readallwidgets(rsrc);
	if(FINDRSCGETINT(rsrc,"WIDGET LIST",&g)) return;
	if(dfscreen->num==1) return;
	new_wdgt=Rmalloc((dfscreen->num-1)*sizeof(dfwdgt));
	y=0;
	for(x=0,temp=dfscreen->wdgts;x<dfscreen->num;++x,++temp)
	{
		temp1=new_wdgt+y;
		if(x!=g)
		{
			temp1->type=temp->type;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			if(temp->label!=NULL) 
			{
				temp1->label=stralloc(temp->label);
				Rfree(temp->label);
			} else temp1->label=NULL;
			if(temp->pixmap!=NULL) 
			{
				temp1->pixmap=stralloc(temp->pixmap);
				Rfree(temp->pixmap);
			} else temp1->pixmap=NULL;
			if(temp->XHTML_Label!=NULL) 
			{
				temp1->XHTML_Label=stralloc(temp->XHTML_Label);
				Rfree(temp->XHTML_Label);
			} else temp1->XHTML_Label=NULL;
			temp1->rows=temp->rows;
			temp1->cols=temp->cols;
			temp1->rtype=temp->rtype;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			if(temp->editable_expression!=NULL)
			{
				temp1->editable_expression=
					stralloc(temp->editable_expression);
				Rfree(temp->editable_expression);
			} else temp1->editable_expression=NULL;
			if(temp->sensitive_expression!=NULL)
			{
				temp1->sensitive_expression=
					stralloc(temp->sensitive_expression);
				Rfree(temp->sensitive_expression);
			} else temp1->sensitive_expression=NULL;
			if(temp->transversal_expression!=NULL)
			{
				temp1->transversal_expression=
					stralloc(temp->transversal_expression);
				Rfree(temp->transversal_expression);
			} else temp1->transversal_expression=NULL;
			++y;
		} else {
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->label!=NULL) Rfree(temp->label);
			if(temp->pixmap!=NULL) Rfree(temp->pixmap);
			if(temp->XHTML_Label!=NULL) Rfree(temp->XHTML_Label);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->editable_expression!=NULL)
				Rfree(temp->editable_expression);
			if(temp->sensitive_expression!=NULL)
				Rfree(temp->sensitive_expression);
			if(temp->transversal_expression!=NULL)
				Rfree(temp->transversal_expression);
		}
	}
	Rfree(dfscreen->wdgts);
	--dfscreen->num;
	dfscreen->wdgts=Rmalloc(dfscreen->num*sizeof(dfwdgt));
	for(x=0,temp1=dfscreen->wdgts,temp=new_wdgt;x<dfscreen->num;++x,++temp,
		++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->label!=NULL)
		{
			temp1->label=stralloc(temp->label);
			Rfree(temp->label);
		} else temp1->label=NULL;
		if(temp->pixmap!=NULL)
		{
			temp1->pixmap=stralloc(temp->pixmap);
			Rfree(temp->pixmap);
		} else temp1->pixmap=NULL;
		if(temp->XHTML_Label!=NULL)
		{
			temp1->XHTML_Label=stralloc(temp->XHTML_Label);
			Rfree(temp->XHTML_Label);
		} else temp1->XHTML_Label=NULL;
		temp1->rows=temp->rows;
		temp1->cols=temp->cols;
		temp1->rtype=temp->rtype;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			if(temp->expression!=NULL) Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->editable_expression!=NULL)
		{
			temp1->editable_expression=stralloc(temp->editable_expression);
			if(temp->editable_expression!=NULL) 
				Rfree(temp->editable_expression);
		} else temp1->editable_expression=NULL;
		if(temp->sensitive_expression!=NULL)
		{
			temp1->sensitive_expression=stralloc(temp->sensitive_expression);
			if(temp->sensitive_expression!=NULL)
				Rfree(temp->sensitive_expression);
		} else temp1->sensitive_expression=NULL;
		if(temp->transversal_expression!=NULL)
		{
			temp1->transversal_expression=stralloc(temp->transversal_expression);
			Rfree(temp->transversal_expression);
		} else temp1->transversal_expression=NULL;
	}
	Rfree(new_wdgt);
	makewlist(&wlist);
	if(!FINDRSCGETINT(rsrc,"WIDGET LIST",&l))
	{
		if(l>=dfscreen->num) l=dfscreen->num-1;
		FINDRSCSETLIST(rsrc,"WIDGET LIST",l,dfscreen->num,&wlist);
		updatersrc(rsrc,"WIDGET LIST");
	}
	changedvalues=TRUE;
} 
static void save_screen(RDArsrc *rsrc,RDArsrc *edit_rsrc)
{
	dfwdgt *wdgt;
	short x;
	char *sname=NULL,*libname=NULL,*libx=NULL;

	readwidget(rsrc,"LIBRARY NAME");
	readwidget(rsrc,"SCREEN NAME");
	if(FINDRSCGETSTRING(rsrc,"SCREEN NAME",&sname)) return;
	if(screen!=NULL) { free_scrn(screen); screen=NULL; }
	screen=RDAscrnNEW(NULL,sname);
	if(sname!=NULL) Rfree(sname);
	for(x=0,wdgt=dfscreen->wdgts;x<dfscreen->num;++x,++wdgt)
	{
		if((wdgt->type==5 || wdgt->type==9) && wdgt->rtype==1)
		{
			ADVaddwdgt(screen,wdgt->type,wdgt->name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,wdgt->expression,
				wdgt->editable_expression,wdgt->sensitive_expression,
				wdgt->transversal_expression);
		} else if((wdgt->type==6 || wdgt->type==14 )&& wdgt->rtype==5) 
		{
			ADVaddwdgt(screen,wdgt->type,wdgt->name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,wdgt->expression,
				wdgt->editable_expression,wdgt->sensitive_expression,
				wdgt->transversal_expression);
		} else {
			ADVaddwdgt(screen,wdgt->type,wdgt->name,wdgt->label,
				NULL,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,wdgt->expression,
				wdgt->editable_expression,wdgt->sensitive_expression,
				wdgt->transversal_expression);
		}
	}
	if(FINDRSCGETSTRING(rsrc,"LIBRARY NAME",&libname))
	{
		prterr("Error Couldn't write screen because the library name resource was not found.");
		return;
	}
	libx=Rmalloc(RDAstrlen(libname)+5);
	sprintf(libx,"%s.SCN",libname);
	if(writescrnbin(libx,screen))
	{
		ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
	}
	getscreenlist(mainrsrc);
	if(libname!=NULL) Rfree(libname);
	if(libx!=NULL) Rfree(libx);
	if(sname!=NULL) Rfree(sname);
	doexit1(rsrc,edit_rsrc);
}
static void show_preview_screen(RDArsrc *rsrc)
{
	RDArsrc *previewrsrc=NULL;
	dfwdgt *wdgt;
	short x;
	int l;
	char blankboolean=TRUE;
	APPlib *blanklist=NULL;
	char *visible_expression=NULL;

	previewrsrc=RDArsrcNEW("GUI","PREVIEW");
	addbtnrsrc(previewrsrc,"CANCEL",TRUE,close_preview_screen,mainrsrc);
	addstdrsrc(previewrsrc,"BLANK STDRSRC",VARIABLETEXT,80,NULL,TRUE);
	addstdrsrc(previewrsrc,"BLANK CUSTOM TYPES",CUSTOMTYPE,80,NULL,TRUE);
	addbtnrsrc(previewrsrc,"BLANK BUTTON",TRUE,NULL,mainrsrc);
	addbtnrsrc(previewrsrc,"BLANK BUTTON",TRUE,NULL,mainrsrc);
	l=0;
	if(blanklist==NULL) blanklist=APPlibNEW();
	addAPPlib(blanklist,"Testing Item 1...");
	addAPPlib(blanklist,"Testing Item 2...");
	addAPPlib(blanklist,"Testing Item 3...");
	addlstrsrc(previewrsrc,"BLANK SCROLLEDLIST",&l,TRUE,NULL,blanklist->numlibs,&blanklist->libs,NULL);
	addsctrsrc(previewrsrc,"BLANK SCROLLEDTEXT",0,NULL,TRUE);
	addstdrsrc(previewrsrc,"BLANK TOGGLEBUTTON",BOOLNS,1,&blankboolean,TRUE);
	if(previewrsrc->scn!=NULL) { free_scrn(previewrsrc->scn); previewrsrc->scn=NULL; }
	previewrsrc->scn=RDAscrnNEW("GUI","PREVIEW SCREEN");
	for(x=0,wdgt=dfscreen->wdgts;x<dfscreen->num;++x,++wdgt)
	{
		/* Don't use the expression unless it's FALSE or (FALSE). */
		if((RDAstrcmp(wdgt->expression, "FALSE")) && (RDAstrcmp(wdgt->expression, "(FALSE)")))
		{
			visible_expression=NULL;
		}
		else
		{
			visible_expression=wdgt->expression;
		}
		if(wdgt->type==0)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK STDRSRC",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==6)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK BUTTON",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==7)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK SCROLLEDLIST",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==8)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK SCROLLEDTEXT",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==9)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK TOGGLEBUTTON",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==14)
		{
			/* Converts it to a button... */
			ADVaddwdgt(previewrsrc->scn,6,"BLANK BUTTON",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if(wdgt->type==15)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,"BLANK CUSTOM TYPES",wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if((wdgt->type==5 || wdgt->type==9) && wdgt->rtype==1)
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,wdgt->name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		} else if((wdgt->type==6 || wdgt->type==14 )&& wdgt->rtype==5) 
		{
			ADVaddwdgt(previewrsrc->scn,wdgt->type,wdgt->name,wdgt->label,
				wdgt->pixmap,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,
				wdgt->editable_expression,wdgt->sensitive_expression,
				wdgt->transversal_expression);
		} else {
			ADVaddwdgt(previewrsrc->scn,wdgt->type,wdgt->name,wdgt->label,
				NULL,wdgt->XHTML_Label,wdgt->rows,
				wdgt->cols,wdgt->rtype,visible_expression,NULL,NULL,NULL);
		}
	}
	ADVaddwdgt(previewrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	/* Cancel button for closing the preview screen. */
	ADVaddwdgt(previewrsrc->scn,6,"CANCEL","Quit Preview","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(previewrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	APPmakescrn(previewrsrc,TRUE,NULL,NULL,FALSE);
}
static void close_preview_screen(RDArsrc *rsrc)
{
	if(rsrc->scn!=NULL) { free_scrn(rsrc->scn); rsrc->scn=NULL; }
	if(rsrc!=NULL)
	{
		killwindow(rsrc);
		NULL_RSCS(rsrc);
	}
}
static void dfmenu(RDArsrc *r,RDArsrc *m)
{
	APPlib *args=NULL;
	int l=0;
	
	if(FINDRSCGETINT(m,"LIBRARY LIST",&l)) return;
	args=APPlibNEW();
	addAPPlib(args,scr_libs->libs[l]);
	ExecuteProgram("dfmenu",args);
	if(args!=NULL) freeapplib(args);
}
static void dfvir(RDArsrc *r,RDArsrc *m)
{
	APPlib *args=NULL;
	int l=0;
	
	if(FINDRSCGETINT(m,"LIBRARY LIST",&l)) return;
	args=APPlibNEW();
	addAPPlib(args,scr_libs->libs[l]);
	ExecuteProgram("dfvir",args);
	if(args!=NULL) freeapplib(args);
}
static void dflist(RDArsrc *r,RDArsrc *m)
{
	APPlib *args=NULL;
	int l=0;
	
	if(FINDRSCGETINT(m,"LIBRARY LIST",&l)) return;
	args=APPlibNEW();
	addAPPlib(args,scr_libs->libs[l]);
	ExecuteProgram("dflist",args);
	if(args!=NULL) freeapplib(args);
}
static void dfload(RDArsrc *r,RDArsrc *m)
{
	APPlib *args=NULL;
	int l=0;
	
	if(FINDRSCGETINT(m,"LIBRARY LIST",&l)) return;
	args=APPlibNEW();
	addAPPlib(args,scr_libs->libs[l]);
	ExecuteProgram("dfload",args);
	if(args!=NULL) freeapplib(args);
}
static void dfmenu_wdgt(RDArsrc *r,RDArsrc *rsrc)
{
	APPlib *args=NULL;
	char *l=NULL,*w=NULL,*temp=NULL;
	int x=0;

	readwidget(r,"LIBRARY NAME");
	FINDRSCGETSTRING(r,"LIBRARY NAME",&l);
	args=APPlibNEW();
	if(!isEMPTY(l))
	{
		x=RDAstrlen(l)-1;
		for(x=RDAstrlen(l)-1;x>0;--x) 
		{
			temp=l+x;
			if(*temp=='\\' || *temp=='/') 
			{
				temp=l+(x+1);
				break;
			} else temp=NULL;
		}
		if(!isEMPTY(temp))
		{
			addAPPlib(args,temp);
			readwidget(r,"WIDGET NAME");
			FINDRSCGETSTRING(r,"WIDGET NAME",&w);
			if(!isEMPTY(w)) addAPPlib(args,w);
		}
	}
	ExecuteProgram("dfmenu",args);
	if(args!=NULL) freeapplib(args);
	if(l!=NULL) Rfree(l);
	if(w!=NULL) Rfree(w);
}
static void dfvir_wdgt(RDArsrc *r,RDArsrc *rsrc)
{
	APPlib *args=NULL;
	char *l=NULL,*w=NULL,*temp=NULL;
	int x=0;

	readwidget(r,"LIBRARY NAME");
	FINDRSCGETSTRING(r,"LIBRARY NAME",&l);
	args=APPlibNEW();
	if(!isEMPTY(l))
	{
		x=RDAstrlen(l)-1;
		for(x=RDAstrlen(l)-1;x>0;--x) 
		{
			temp=l+x;
			if(*temp=='\\' || *temp=='/') 
			{
				temp=l+(x+1);
				break;
			} else temp=NULL;
		}
		if(!isEMPTY(temp))
		{
			addAPPlib(args,temp);
			readwidget(r,"WIDGET NAME");
			FINDRSCGETSTRING(r,"WIDGET NAME",&w);
			if(!isEMPTY(w)) addAPPlib(args,w);
		}
	}
	ExecuteProgram("dfvir",args);
	if(args!=NULL) freeapplib(args);
	if(l!=NULL) Rfree(l);
	if(w!=NULL) Rfree(w);
}
static void dflist_wdgt(RDArsrc *r,RDArsrc *rsrc)
{
	APPlib *args=NULL;
	char *l=NULL,*w=NULL,*temp=NULL;
	int x=0;

	readwidget(r,"LIBRARY NAME");
	FINDRSCGETSTRING(r,"LIBRARY NAME",&l);
	args=APPlibNEW();
	if(!isEMPTY(l))
	{
		x=RDAstrlen(l)-1;
		for(x=RDAstrlen(l)-1;x>0;--x) 
		{
			temp=l+x;
			if(*temp=='\\' || *temp=='/') 
			{
				temp=l+(x+1);
				break;
			} else temp=NULL;
		}
		if(!isEMPTY(temp))
		{
			addAPPlib(args,temp);
			readwidget(r,"WIDGET NAME");
			FINDRSCGETSTRING(r,"WIDGET NAME",&w);
			if(!isEMPTY(w)) addAPPlib(args,w);
		}
	}
	ExecuteProgram("dflist",args);
	if(args!=NULL) freeapplib(args);
	if(l!=NULL) Rfree(l);
	if(w!=NULL) Rfree(w);
}
static void dfload_wdgt(RDArsrc *r,RDArsrc *rsrc)
{
	APPlib *args=NULL;
	char *l=NULL,*w=NULL,*temp=NULL;
	int x=0;

	readwidget(r,"LIBRARY NAME");
	FINDRSCGETSTRING(r,"LIBRARY NAME",&l);
	args=APPlibNEW();
	if(!isEMPTY(l))
	{
		x=RDAstrlen(l)-1;
		for(x=RDAstrlen(l)-1;x>0;--x) 
		{
			temp=l+x;
			if(*temp=='\\' || *temp=='/') 
			{
				temp=l+(x+1);
				break;
			} else temp=NULL;
		}
		if(!isEMPTY(temp))
		{
			addAPPlib(args,temp);
			readwidget(r,"WIDGET NAME");
			FINDRSCGETSTRING(r,"WIDGET NAME",&w);
			if(!isEMPTY(w)) addAPPlib(args,w);
		}
	}
	ExecuteProgram("dflist",args);
	if(args!=NULL) freeapplib(args);
	if(l!=NULL) Rfree(l);
	if(w!=NULL) Rfree(w);
}
static void setamenu(RDArsrc *r)
{
	readwidget(r,"MAKING A MENU");
	FINDRSCGETCHAR(r,"MAKING A MENU",&Making_A_Menu);
}
static void new_screen(RDArsrc *mainrsrc)
{
	dfwdgt *wp;
	int x,l;
	char *dirx=NULL,*libx=NULL;
	RDArsrc *rsrc=NULL,*edit_rsrc=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	dfscreen=NEWdfscn();
	rsrc=RDArsrcNEW("GUI","MAINTAIN SCREEN");
	edit_rsrc=RDArsrcNEW("GUI","EDIT SCREEN WIDGET");
	dfscreen->num=1;
	if(dfscreen->wdgts!=NULL)
	{
		dfscreen->wdgts=Rrealloc(dfscreen->wdgts,sizeof(dfwdgt));
	} else dfscreen->wdgts=Rmalloc(sizeof(dfwdgt));
	wp=dfscreen->wdgts;
	memset(wp,0,sizeof(dfwdgt));
	wp->type=1;
	wp->name=NULL;
	wp->label=NULL;
	wp->pixmap=NULL;
	wp->expression=NULL;
	wp->sensitive_expression=NULL;
	wp->editable_expression=NULL;
	wp->transversal_expression=NULL;
	makewlist(&wlist);
	x=0;
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+2);
#ifndef WIN32
	sprintf(libx,"%s/%s",dirx,scr_libs->libs[l]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s",dirx,scr_libs->libs[l]);
#endif
	if(dirx!=NULL) Rfree(dirx);
	addstdrsrc(rsrc,"LIBRARY NAME",VARIABLETEXT,80,libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	addstdrsrc(rsrc,"SCREEN NAME",VARIABLETEXT,80,NULL,TRUE);
	FINDRSCSETFUNC(rsrc,"LIBRARY NAME",changelibname,edit_rsrc);
	FINDRSCSETFUNC(rsrc,"SCREEN NAME",changelibname,edit_rsrc);
	addstdrsrc(rsrc,"MAKING A MENU",BOOLNS,1,&Making_A_Menu,TRUE);
	FINDRSCSETFUNC(rsrc,"MAKING A MENU",setamenu,NULL);
	addlstrsrc(rsrc,"WIDGET LIST",&x,TRUE,NULL,dfscreen->num,&wlist,NULL);
	addbtnrsrc(rsrc,"MOVE UP",TRUE,scnmoveup,NULL);
	addbtnrsrc(rsrc,"MOVE DOWN",TRUE,scnmovedown,NULL);
	addrfcbrsrc(rsrc,"PREVIEW",TRUE,show_preview_screen,mainrsrc);
	addrfcbrsrc(rsrc,"ADD ABOVE",TRUE,add_above,edit_rsrc);
	addrfcbrsrc(rsrc,"ADD BELOW",TRUE,add_below,edit_rsrc);
	addrfcbrsrc(rsrc,"DELETE",TRUE,delete_widget,NULL);
	addrfcbrsrc(rsrc,"SELECT",TRUE,edit_current,edit_rsrc);
	addrfcbrsrc(rsrc,"QUIT",TRUE,doexit1test,edit_rsrc);
	addrfcbrsrc(rsrc,"SAVE",TRUE,save_screen,edit_rsrc);
	addrfcbrsrc(rsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(rsrc,"DFLIST",TRUE,dflist,mainrsrc);
	addbtnrsrc(rsrc,"DFVIR",TRUE,dfvir,mainrsrc);
	addbtnrsrc(rsrc,"DFLOAD",TRUE,dfload,mainrsrc);
	addbtnrsrc(rsrc,"DFMENU",TRUE,dfmenu,mainrsrc);
	addstdrsrc(edit_rsrc,"LIBRARY NAME",VARIABLETEXT,80,NULL,FALSE);
	addstdrsrc(edit_rsrc,"SCREEN NAME",VARIABLETEXT,80,NULL,FALSE);
	maketestlist();
	addlstrsrc(edit_rsrc,"WIDGET TYPES",&x,TRUE,changewidgetlist,NE(wdgttypes),&wdgtlist,NULL);
	l=0;
	if(RSRClist==NULL) RSRClist=APPlibNEW();
	addlstrsrc(edit_rsrc,"WIDGET NAMES",&l,TRUE,changenamelist,RSRClist->numlibs,&RSRClist->libs,NULL);
	addstdrsrc(edit_rsrc,"WIDGET NAME",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(edit_rsrc,"WIDGET NAME",changenamefield,NULL);
	addstdrsrc(edit_rsrc,"WIDGET LABEL",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(edit_rsrc,"WIDGET PIXMAP",VARIABLETEXT,0,NULL,TRUE);
	addsctrsrc(edit_rsrc,"XHTML LABEL",0,NULL,TRUE);
	addstdrsrc(edit_rsrc,"NUMBER ROWS",SHORTV,3,0,TRUE);
	addstdrsrc(edit_rsrc,"NUMBER COLUMNS",SHORTV,3,0,TRUE);
	addlstrsrc(edit_rsrc,"RESOURCE TYPES",0,TRUE,changersctype,rsrclist->numlibs,&rsrclist->libs,NULL);
	addsctrsrc(edit_rsrc,"EXPRESSION",0,NULL,TRUE);
	addsctrsrc(edit_rsrc,"EDITABLE EXPRESSION",0,NULL,TRUE);
	addsctrsrc(edit_rsrc,"SENSITIVE EXPRESSION",0,NULL,TRUE);
	addsctrsrc(edit_rsrc,"TRANSVERSAL EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(edit_rsrc,"DFLIST",TRUE,dflist_wdgt,rsrc);
	addbtnrsrc(edit_rsrc,"DFVIR",TRUE,dfvir_wdgt,rsrc);
	addbtnrsrc(edit_rsrc,"DFLOAD",TRUE,dfload_wdgt,rsrc);
	addbtnrsrc(edit_rsrc,"DFMENU",TRUE,dfmenu_wdgt,rsrc);
	addrfcbrsrc(edit_rsrc,"SAVE",TRUE,save_widget,rsrc);
	addrfcbrsrc(edit_rsrc,"QUIT",TRUE,quit_widgettest,rsrc);
	addrfcbrsrc(edit_rsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(edit_rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedvalues=FALSE;
	rsrcdefaults=GetDefaults(rsrc);
	changelibname(rsrc,edit_rsrc);
	APPmakescrn(rsrc,TRUE,NULL,NULL,FALSE);
}
static void okfunction(RDArsrc *mainrsrc)
{
	dfwdgt *wp;
	short x;
	int l;
	int s=(-1);
	RDAwdgt *wdgt;
	char *libx=NULL;
	char *dirx=NULL;
	RDArsrc *rsrc=NULL,*edit_rsrc=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	if(FINDRSCGETINT(mainrsrc,"SCREEN LIST",&s)) return;
	if(!RDAstrcmp(scr_defs->libs[s],"Contains No Screen Definitions")) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	dfscreen=NEWdfscn();
	rsrc=RDArsrcNEW("GUI","MAINTAIN SCREEN");
	edit_rsrc=RDArsrcNEW("GUI","EDIT SCREEN WIDGET");
	if(screen!=NULL) { free_scrn(screen); screen=NULL; }
	screen=RDAscrnNEW(scr_libs->libs[l],scr_defs->libs[s]);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.SCN",dirx,scr_libs->libs[l]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.SCN",dirx,scr_libs->libs[l]);
#endif
	if(dirx!=NULL) Rfree(dirx);
	if(getscrnbin(libx,screen,FALSE))
	{
		/* no information on file */
		dfscreen->num=1;
		if(dfscreen->wdgts!=NULL)
		{
			dfscreen->wdgts=Rrealloc(dfscreen->wdgts,sizeof(dfwdgt));
		} else dfscreen->wdgts=Rmalloc(sizeof(dfwdgt));
		wp=dfscreen->wdgts;
		memset(wp,0,sizeof(dfwdgt));
		wp->type=1;
		wp->name=NULL;
		wp->label=NULL;
		wp->pixmap=NULL;
		wp->expression=NULL;
		wp->sensitive_expression=NULL;
		wp->editable_expression=NULL;
		wp->transversal_expression=NULL;
	} else {
		/* information read from bin editing */
		if(dfscreen->wdgts!=NULL) 
		{
			dfscreen->wdgts=Rrealloc(dfscreen->wdgts,(screen->numwdgts*sizeof(dfwdgt)));
		} else dfscreen->wdgts=Rmalloc(screen->numwdgts*sizeof(dfwdgt));
		dfscreen->num=screen->numwdgts;
		for(x=0,wp=dfscreen->wdgts,wdgt=screen->wdgts;
			x<screen->numwdgts;++x,++wp,++wdgt)
		{
			wp->type=wdgt->type;
			if(wdgt->resource_name!=NULL) wp->name=stralloc(wdgt->resource_name);
			if(wdgt->label!=NULL) wp->label=stralloc(wdgt->label);
			if(wdgt->pixmap!=NULL) wp->pixmap=stralloc(wdgt->pixmap);
			if(wdgt->XHTML_Label!=NULL) wp->XHTML_Label=stralloc(wdgt->XHTML_Label);
			wp->rows=wdgt->rows;
			wp->cols=wdgt->cols;
			wp->rtype=wdgt->rtype;
			wp->expression=stralloc(wdgt->expression);
			if(wdgt->editable_expression!=NULL)
			{
				wp->editable_expression=stralloc(wdgt->editable_expression);
			} else wp->editable_expression=NULL;
			if(wdgt->sensitive_expression!=NULL)
			{
				wp->sensitive_expression=stralloc(wdgt->sensitive_expression);
			} else wp->sensitive_expression=NULL;
			if(wdgt->transversal_expression!=NULL)
			{
				wp->transversal_expression=stralloc(wdgt->transversal_expression);
			} else wp->transversal_expression=NULL;
		}
	}
	makewlist(&wlist);
	libx[RDAstrlen(libx)-4]=0;
	addstdrsrc(rsrc,"LIBRARY NAME",VARIABLETEXT,80,libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	addstdrsrc(rsrc,"SCREEN NAME",VARIABLETEXT,80,scr_defs->libs[s],TRUE);
	FINDRSCSETFUNC(rsrc,"LIBRARY NAME",changelibname,edit_rsrc);
	FINDRSCSETFUNC(rsrc,"SCREEN NAME",changelibname,edit_rsrc);
	l=0;
	addstdrsrc(rsrc,"MAKING A MENU",BOOLNS,1,&Making_A_Menu,TRUE);
	FINDRSCSETFUNC(rsrc,"MAKING A MENU",setamenu,NULL);
	addlstrsrc(rsrc,"WIDGET LIST",&l,TRUE,NULL,dfscreen->num,&wlist,NULL);
	addbtnrsrc(rsrc,"MOVE UP",TRUE,scnmoveup,NULL);
	addbtnrsrc(rsrc,"MOVE DOWN",TRUE,scnmovedown,NULL);
	addrfcbrsrc(rsrc,"PREVIEW",TRUE,show_preview_screen,mainrsrc);
	addrfcbrsrc(rsrc,"ADD ABOVE",TRUE,add_above,edit_rsrc);
	addrfcbrsrc(rsrc,"ADD BELOW",TRUE,add_below,edit_rsrc);
	addrfcbrsrc(rsrc,"COPY",TRUE,copy_widget,edit_rsrc);
	addrfcbrsrc(rsrc,"DELETE",TRUE,delete_widget,NULL);
	addrfcbrsrc(rsrc,"SELECT",TRUE,edit_current,edit_rsrc);
	addrfcbrsrc(rsrc,"QUIT",TRUE,doexit1test,edit_rsrc);
	addrfcbrsrc(rsrc,"SAVE",TRUE,save_screen,edit_rsrc);
	addrfcbrsrc(rsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(rsrc,"DFLIST",TRUE,dflist,mainrsrc);
	addbtnrsrc(rsrc,"DFVIR",TRUE,dfvir,mainrsrc);
	addbtnrsrc(rsrc,"DFLOAD",TRUE,dfload,mainrsrc);
	addbtnrsrc(rsrc,"DFMENU",TRUE,dfmenu,NULL);
/*lint -e611 */
	addrfcbrsrc(rsrc,"PRINT DEFINITION",TRUE,printdefinition,(void *)printscrndefcb);
/*lint +e611 */
	addstdrsrc(edit_rsrc,"LIBRARY NAME",VARIABLETEXT,80,NULL,FALSE);
	addstdrsrc(edit_rsrc,"SCREEN NAME",VARIABLETEXT,80,NULL,FALSE);
	addlstrsrc(edit_rsrc,"WIDGET TYPES",&l,TRUE,changewidgetlist,NE(wdgttypes),&wdgtlist,NULL);
	l=0;
	if(RSRClist==NULL) RSRClist=APPlibNEW();
	addlstrsrc(edit_rsrc,"WIDGET NAMES",&l,TRUE,changenamelist,RSRClist->numlibs,&RSRClist->libs,NULL);
	addstdrsrc(edit_rsrc,"WIDGET NAME",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(edit_rsrc,"WIDGET NAME",changenamefield,NULL);
	addstdrsrc(edit_rsrc,"WIDGET LABEL",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(edit_rsrc,"WIDGET PIXMAP",VARIABLETEXT,0,NULL,TRUE);
	addsctrsrc(edit_rsrc,"XHTML LABEL",0,NULL,TRUE);
	addstdrsrc(edit_rsrc,"NUMBER ROWS",SHORTV,3,0,TRUE);
	addstdrsrc(edit_rsrc,"NUMBER COLUMNS",SHORTV,3,0,TRUE);
	addlstrsrc(edit_rsrc,"RESOURCE TYPES",0,TRUE,changersctype,rsrclist->numlibs,&rsrclist->libs,NULL);
	addsctrsrc(edit_rsrc,"EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(edit_rsrc,"EXPRESSION LOAD VALUE",TRUE,BrowseLoadValue,"EXPRESSION");
	addbtnrsrc(edit_rsrc,"EXPRESSION LOAD OPERAND",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(edit_rsrc,"EXPRESSION LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(edit_rsrc,"EXPRESSION LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addsctrsrc(edit_rsrc,"EDITABLE EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(edit_rsrc,"EDITABLE EXPRESSION LOAD VALUE",TRUE,BrowseLoadValue,"EDITABLE EXPRESSION");
	addbtnrsrc(edit_rsrc,"EDITABLE EXPRESSION LOAD OPERAND",TRUE,loadoperand,"EDITABLE EXPRESSION");
	addbtnrsrc(edit_rsrc,"EDITABLE EXPRESSION LOAD GROUPER",TRUE,loadgrouper,"EDITABLE EXPRESSION");
	addbtnrsrc(edit_rsrc,"EDITABLE EXPRESSION LOAD CONTROL",TRUE,loadcontrol,"EDITABLE EXPRESSION");
	addsctrsrc(edit_rsrc,"SENSITIVE EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(edit_rsrc,"SENSITIVE EXPRESSION LOAD VALUE",TRUE,BrowseLoadValue,"SENSITIVE EXPRESSION");
	addbtnrsrc(edit_rsrc,"SENSITIVE EXPRESSION LOAD OPERAND",TRUE,loadoperand,"SENSITIVE EXPRESSION");
	addbtnrsrc(edit_rsrc,"SENSITIVE EXPRESSION LOAD GROUPER",TRUE,loadgrouper,"SENSITIVE EXPRESSION");
	addbtnrsrc(edit_rsrc,"SENSITIVE EXPRESSION LOAD CONTROL",TRUE,loadcontrol,"SENSITIVE EXPRESSION");
	addsctrsrc(edit_rsrc,"TRANSVERSAL EXPRESSION",0,NULL,TRUE);
	addbtnrsrc(edit_rsrc,"TRANSVERSAL EXPRESSION LOAD VALUE",TRUE,BrowseLoadValue,"TRANSVERSAL EXPRESSION");
	addbtnrsrc(edit_rsrc,"TRANSVERSAL EXPRESSION LOAD OPERAND",TRUE,loadoperand,"TRANSVERSAL EXPRESSION");
	addbtnrsrc(edit_rsrc,"TRANSVERSAL EXPRESSION LOAD GROUPER",TRUE,loadgrouper,"TRANSVERSAL EXPRESSION");
	addbtnrsrc(edit_rsrc,"DFLIST",TRUE,dflist_wdgt,mainrsrc);
	addbtnrsrc(edit_rsrc,"DFVIR",TRUE,dfvir_wdgt,mainrsrc);
	addbtnrsrc(edit_rsrc,"DFLOAD",TRUE,dfload_wdgt,mainrsrc);
	addbtnrsrc(edit_rsrc,"DFMENU",TRUE,dfmenu_wdgt,mainrsrc);
	addbtnrsrc(edit_rsrc,"TRANSVERSAL EXPRESSION LOAD CONTROL",TRUE,loadcontrol,"TRANSVERSAL EXPRESSION");
	addrfcbrsrc(edit_rsrc,"SAVE",TRUE,save_widget,rsrc);
	addrfcbrsrc(edit_rsrc,"QUIT",TRUE,quit_widgettest,rsrc);
	addrfcbrsrc(edit_rsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(edit_rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	rsrcdefaults=GetDefaults(rsrc);
	changelibname(rsrc,edit_rsrc);
	APPmakescrn(rsrc,TRUE,NULL,NULL,FALSE);
}
static void delete_screen(RDArsrc *mainrsrc)
{
	int l,s;
	char *libx=NULL,*directory=NULL;

	if(FINDRSCGETINT(mainrsrc,"SCREEN LIST",&s)) return;
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(scr_libs->libs[l])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.SCN",directory,scr_libs->libs[l]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.SCN",directory,scr_libs->libs[l]);
#endif
	deletelibbin(libx,scr_defs->libs[s]);
	freeapplib(scr_defs);
	scr_defs=getlibnames(libx,FALSE);
	if(scr_defs==NULL)
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Screen Definitions");
	}
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	if(s>=scr_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN LIST",s,scr_defs))
		updatersrc(mainrsrc,"SCREEN LIST");
	changedvalues=TRUE;
}
static void deletescrntest(RDArsrc *mainrsrc)
{
	int s;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"SCREEN LIST",&s)) return;
	if(!RDAstrcmp(scr_defs->libs[s],"Contains No Screen Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE SCREEN?","Are you sure that you want to delete this screen?",delete_screen,NULL,FALSE,mainrsrc);
}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int x,selectedx=0;
	char *tmp=NULL,*Tdirectory=NULL,*dirx=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&selectedx)) selectedx=0;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&Tdirectory)) return;
	if(scr_libs!=NULL) freeapplib(scr_libs);
	scr_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(Tdirectory)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",Tdirectory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",Tdirectory);
#endif
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(scr_libs,tmp);
	}
	if(scr_libs->numlibs<1)
	{
		addAPPlib(scr_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(scr_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(Tdirectory!=NULL) Rfree(Tdirectory);
	if(dirx!=NULL) Rfree(dirx);
	if(selectedx>=scr_libs->numlibs) selectedx=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LIBRARY LIST",selectedx,scr_libs))
		updatersrc(mainrsrc,"LIBRARY LIST");
	doliblist(mainrsrc);
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory)) 
		{
			inlist=TRUE;		
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
		updatersrc(mainrsrc,"DIRECTORY LIST");
	getscreenlist(mainrsrc);
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s])) 
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s])) return;
			updatersrc(mainrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getscreenlist(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *defdir=NULL;
	int w=2,m=0,s=0;

	RDA_SOFTWARE_TYPE=RDA_ALL_APP;
	if(InitializeSubsystems(argc,argv,"GUI","DEFINE SCREENS")) 
	{
		return;
	}
	DoHeaders();
	mainrsrc=RDArsrcNEW("GUI","DEFINE SCREENS");
	scr_libs=APPlibNEW();
	scr_defs=APPlibNEW();
	maketestlist();
	if(rsrclist!=NULL) freeapplib(rsrclist);
	rsrclist=APPlibNEW();
	addAPPlib(rsrclist,"WordWrap & No Horizontal SB");
	addAPPlib(rsrclist,"Horizontal SB & No WordWrap");
	dir_libs=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	if(!isEMPTY(RDA_GROUP))
	{
	w=3;
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(RDA_GROUP)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	}
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) { Rfree(defdir); defdir=NULL; }
	addlstrsrc(mainrsrc,"DIRECTORY LIST",&w,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[w],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",w,dir_libs);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist,NULL);
	addlstrsrc(mainrsrc,"LIBRARY LIST",m,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
	addlstrsrc(mainrsrc,"SCREEN LIST",s,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
	getscreenlist(mainrsrc);
	if(argc>1 && scr_libs!=NULL)
	{
		for(m=0;m<scr_libs->numlibs;++m)
		{
			if(!RDAstrcmp(scr_libs->libs[m],argv[1])) break;
		}
		if(m<scr_libs->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"LIBRARY LIST",m);
		}
		else
		{
			m=0;
			FINDRSCSETINT(mainrsrc,"LIBRARY LIST",m);
		}
		doliblist(mainrsrc);
	}
	if(argc>2 && scr_defs!=NULL)
	{
		for(s=0;s<scr_defs->numlibs;++s)
		{
			if(!RDAstrcmp(scr_defs->libs[s],argv[2])) break;
		}
		if(s<scr_defs->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"SCREEN LIST",s);
		}
		else
		{
			s=0;
			FINDRSCSETINT(mainrsrc,"SCREEN LIST",s);
		}
	}
	addrfcbrsrc(mainrsrc,"ADD",TRUE,new_screen,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfcbrsrc(mainrsrc,"DELETE",TRUE,deletescrntest,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
/*lint -e611 */
	addrfcbrsrc(mainrsrc,"PRINT MODULE",TRUE,printdefinition,(void *)printmodule);
/*lint +e611 */
	FINDRSCSETINPUTFOCUS(mainrsrc,"LIBRARY LIST");
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
