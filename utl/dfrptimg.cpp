/* dfrptimg.c - Define Image section of Define Reports */
#include <cstdio>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

extern RDAreport *report;
extern char *module;
extern void loadvalue(RDArsrc *,char *);
extern void makefieldvallistrpt(APPlib **,short,short,short,short);
extern char changedreport;
static RDAimage *holdimage=NULL;
static APPlib *imgavl=NULL,*imgpval=NULL,*brkavl=NULL;
static APPlib *symboltypes=NULL,*varnames=NULL;
static int holdnum=0,holdpnum=0;
static RDApval *holdpval=NULL;
static char *symboltypedesc[2]={"FIELD / VIRTUAL / ACCUM","DEVICE CONTROL VARIABLE"};
static RDAdefault *imgdefaults=NULL,*mimgdefaults=NULL,*editdefaults=NULL;
static char changedimages=FALSE;
static short verifyline(RDArsrc *);
static void copyline(RDArsrc *);

static APPlib *makedevicelist()
{
	APPlib *tmp=NULL;
	RDAdisplay *d;
	RDApvar *pvar;
	int x;

	tmp=APPlibNEW();
	addAPPlib(tmp,"CARRIAGE RETURN");
	addAPPlib(tmp,"FORMFEED");
	addAPPlib(tmp,"LINEFEED");
	addAPPlib(tmp,"TAB");
	addAPPlib(tmp,"HALF LINE FEED");
	addAPPlib(tmp,"NEG Q LINE FEED");
	addAPPlib(tmp,"POS Q LINE FEED");
	if(report->display->dumprinter==FALSE)
	{
	addAPPlib(tmp,"NORMAL TEXT");
	addAPPlib(tmp,"COMPRESSED TEXT");
	addAPPlib(tmp,"LANDSCAPE");
	addAPPlib(tmp,"PORTRAIT");
	addAPPlib(tmp,"DUPLEX SHORT");
	addAPPlib(tmp,"DUPLEX LONG");
	addAPPlib(tmp,"SIMPLEX");
	addAPPlib(tmp,"POP CURSOR");
	addAPPlib(tmp,"PUSH CURSOR");
	addAPPlib(tmp,"LOAD VENPMT SIGNATURE");
	addAPPlib(tmp,"VENPMT SIGNATURE");
	addAPPlib(tmp,"LOAD PAYROLL SIGNATURE");
	addAPPlib(tmp,"PAYROLL SIGNATURE");
	addAPPlib(tmp,"RESET FONT");
	addAPPlib(tmp,"LOAD_SOFTFONT_40");
	addAPPlib(tmp,"LOAD_SOFTFONT_44");
	addAPPlib(tmp,"LOAD_SOFTFONT_45");
	addAPPlib(tmp,"LOAD_SOFTFONT_46");
	addAPPlib(tmp,"LOAD_SOFTFONT_47");
	addAPPlib(tmp,"LOAD_SOFTFONT_48");
	addAPPlib(tmp,"LOAD_SOFTFONT_49");
	addAPPlib(tmp,"USE_SOFTFONT_40");
	addAPPlib(tmp,"USE_SOFTFONT_44");
	addAPPlib(tmp,"USE_SOFTFONT_45");
	addAPPlib(tmp,"USE_SOFTFONT_46");
	addAPPlib(tmp,"USE_SOFTFONT_47");
	addAPPlib(tmp,"USE_SOFTFONT_48");
	addAPPlib(tmp,"USE_SOFTFONT_49");
	addAPPlib(tmp,"DELETE_SOFTFONT_40");
	addAPPlib(tmp,"DELETE_SOFTFONT_44");
	addAPPlib(tmp,"DELETE_SOFTFONT_45");
	addAPPlib(tmp,"DELETE_SOFTFONT_46");
	addAPPlib(tmp,"DELETE_SOFTFONT_47");
	addAPPlib(tmp,"DELETE_SOFTFONT_48");
	addAPPlib(tmp,"DELETE_SOFTFONT_49");
	addAPPlib(tmp,"DELETE_SOFTFONT_200");
	addAPPlib(tmp,"DELETE_SOFTFONT_201");
	}
	addAPPlib(tmp,"<");
	addAPPlib(tmp,">");
	addAPPlib(tmp,"$");
	addAPPlib(tmp,".");
	addAPPlib(tmp,",");
	addAPPlib(tmp,"\"");
	addAPPlib(tmp,"/");
	addAPPlib(tmp,"\\");
	addAPPlib(tmp,"@");
	addAPPlib(tmp,"%");
	addAPPlib(tmp,"#");
	addAPPlib(tmp,"(");
	addAPPlib(tmp,")");
	addAPPlib(tmp,"[");
	addAPPlib(tmp,"]");
	addAPPlib(tmp,"{");
	addAPPlib(tmp,"}");
	addAPPlib(tmp,"|");
	d=report->display;
	if(d!=NULL)
	{
		if(d->dumprinter==FALSE)
		{
			for(x=0,pvar=d->pvars;x<d->num;++x,++pvar)
			{
				addAPPlib(tmp,pvar->name);
			}
		}
	}
	return(tmp);
}

static void getimgvalues()
{
	int x,len=0;
	RDApval *p;
	char *tmp=NULL;

	if(imgpval!=NULL) freeapplib(imgpval);
	imgpval=APPlibNEW();
	if(holdpval!=NULL)
	{
		for(x=0,p=holdpval;x<holdpnum;++x,++p)
		{
			len=(5+RDAstrlen(p->name)+RDAstrlen(symboltypedesc[p->symbol_type])+8+1);
			if(tmp!=NULL)
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"[%5d] [%s] [%s]",
				(x+1),
				symboltypedesc[p->symbol_type],
				(p->name!=NULL ? p->name:""));
			addAPPlib(imgpval,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(imgpval->numlibs<1)
	{
		addAPPlib(imgpval,"Currently there are no image values defined file this report.");
	}
}
static void makespaceimgpval(int pos)
{
	short x,y;
	RDApval *temp,*temp1,*new_fields;

	++holdpnum;
	new_fields=Rmalloc(holdpnum*sizeof(RDApval));
	y=0;
	for(x=0,temp=holdpval;x<(holdpnum-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->symbol_type=0;
			temp1->name=NULL;
			temp1->type=0;
			temp1->fieldname=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->symbol_type=temp->symbol_type;
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
		++y;
	}
	Rfree(holdpval);
	holdpval=Rmalloc(holdpnum*sizeof(RDApval));
	for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->symbol_type=temp->symbol_type;
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
	}
	Rfree(new_fields);
}
static void quitline(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	if(editrsrc!=NULL)
	{
		killwindow(editrsrc);
		free_rsrc(editrsrc);
	}
	if(symboltypes!=NULL) freeapplib(symboltypes);
	if(varnames!=NULL) freeapplib(varnames);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
}
static void switchsymboltypes(RDArsrc *editrsrc)
{
	int x=0;
	int selectedf=0;

	readwidget(editrsrc,"SYMBOL TYPES");
	readwidget(editrsrc,"VARIABLE NAMES");
	if(FINDRSCGETINT(editrsrc,"SYMBOL TYPES",&x)) return;
	if(FINDRSCGETINT(editrsrc,"VARIABLE NAMES",&selectedf)) return;
	if(varnames!=NULL) freeapplib(varnames);
	if(!x)
	{
		varnames=APPlibNEW();
		makefieldvallistrpt(&varnames,TRUE,TRUE,TRUE,TRUE);
		if(varnames->numlibs<1)
		{
			addAPPlib(varnames,"No Fields or Virtual Fields Available");
		}
	} else {
		varnames=makedevicelist();
		if(varnames->numlibs<1)
		{
			addAPPlib(varnames,"No Device Control Strings Available");
		}
	}
	if(varnames->numlibs>=selectedf) selectedf=0;
	if(!x)
	{
		for(selectedf=0;selectedf<varnames->numlibs;++selectedf)
		{
			if(strncmp(varnames->libs[selectedf],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(selectedf>=varnames->numlibs) selectedf=0;
	}
	if(!FINDRSCLISTAPPlib(editrsrc,"VARIABLE NAMES",selectedf,varnames))
		updatersrc(editrsrc,"VARIABLE NAMES");
}
static void shuffleimages()
{
/*
	int x,y;
	RDApval *pval1,*pval2;
	RDAimage *i1,*i2;

	for(x=1,i1=holdimage;x<a->numlibs;++x,++i1)
	{
		for(y=(a->numlibs-1);y>=x;--y)
		{
			if(RDAstrcmp(a->libs[y],a->libs[y-1])<0)
			{
				hold=a->libs[y];
				a->libs[y]=a->libs[y-1];
				a->libs[y-1]=hold;
			}
		}
	}
*/
}
static void saveline(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int value=0;
	int x=0;
	RDApval *p;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&value)) return;
	if(holdpval==NULL)
	{
		holdpval=Rmalloc(sizeof(RDApval));
		holdpnum=1;
	}
	p=holdpval+value;
	if(p==NULL)
	{
		p=Rmalloc(sizeof(RDApval));
	}
	readallwidgets(editrsrc);
	FINDRSCGETINT(editrsrc,"SYMBOL TYPES",&x);
	p->symbol_type=x;
	x=0;
	FINDRSCGETINT(editrsrc,"VARIABLE NAMES",&x);
	if(p->name!=NULL && RDAstrlen(p->name)>0) Rfree(p->name);
	p->name=stralloc(varnames->libs[x]);
	p->fieldname=NULL;
	p->type=0;
	shuffleimages();
	getimgvalues();
	if(!FINDRSCLISTAPPlib(mimgrsrc,"IMAGE VALUES",value,imgpval))
		updatersrc(mimgrsrc,"IMAGE VALUES");
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedimages=TRUE;
	}
	quitline(editrsrc,mimgrsrc);
}
static void savelineabove(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&x)) return;
	makespaceimgpval(x);
	saveline(editrsrc,mimgrsrc);
}
static void savelinebelow(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&x)) return;
	makespaceimgpval(++x);
	FINDRSCSETINT(mimgrsrc,"IMAGE VALUES",x);
	saveline(editrsrc,mimgrsrc);
}
static void quitlinetest(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE VARIABLE!\nDo you want to Save these changes?",saveline,quitline,FALSE,2,editrsrc,mimgrsrc,NULL);
	} else {
		quitline(editrsrc,mimgrsrc);
	}
}
static void quitlineabovetest(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE VARIABLE!\nDo you want to Save these changes?",savelineabove,quitline,FALSE,2,editrsrc,mimgrsrc,NULL);
	} else {
		quitline(editrsrc,mimgrsrc);
	}
}
static void quitlinebelowtest(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE VARIABLE!\nDo you want to Save these changes?",savelinebelow,quitline,FALSE,2,editrsrc,mimgrsrc,NULL);
	} else {
		quitline(editrsrc,mimgrsrc);
	}
}
static void linescreen(RDArsrc *mimgrsrc,int num,int type,char *varname,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0;
	RDArsrc *editrsrc=NULL;

	if(symboltypes!=NULL) freeapplib(symboltypes);
	symboltypes=APPlibNEW();
	addAPPlib(symboltypes,symboltypedesc[0]);
	addAPPlib(symboltypes,symboltypedesc[1]);
	editrsrc=RDArsrcNEW(module,"EDIT LINE VARIABLES");
	addstdrsrc(editrsrc,"NUMBER",LONGV,8,&num,FALSE);
	x=0;
	addlstrsrc(editrsrc,"SYMBOL TYPES",&type,TRUE,switchsymboltypes,
		symboltypes->numlibs,&symboltypes->libs,NULL);
	if(varnames!=NULL) { freeapplib(varnames); varnames=NULL; }
	varnames=APPlibNEW();
	addlstrsrc(editrsrc,"VARIABLE NAMES",&x,TRUE,NULL,varnames->numlibs,
		&varnames->libs,NULL);
	switchsymboltypes(editrsrc);
	for(x=0;x<varnames->numlibs;++x)
	{
		if(!RDAstrcmp(varnames->libs[x],varname)) break;
	}
	if(x>=varnames->numlibs)
	{
		for(x=0;x<varnames->numlibs;++x)
		{
			if(strncmp(varnames->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=varnames->numlibs) x=0;
	}
	FINDRSCSETINT(editrsrc,"VARIABLE NAMES",x);
	addrfcbrsrc(editrsrc,"SAVE",TRUE,savefunc,mimgrsrc);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quitfunctest,mimgrsrc);
	addbtnrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	editdefaults=GetDefaults(editrsrc);
	APPmakescrn(editrsrc,TRUE,quitline,mimgrsrc,FALSE);
}
static void lineaddabove(RDArsrc *mimgrsrc)
{
	int selected=0;

	if(imgpval->numlibs==1 && !RDAstrcmp(imgpval->libs[0],"Currently there are no image values defined file this report.")) return;
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
	linescreen(mimgrsrc,++selected,0,NULL,savelineabove,quitlineabovetest);
}
static void lineaddbelow(RDArsrc *mimgrsrc)
{
	int selected=0;

	if(imgpval->numlibs==1 && !RDAstrcmp(imgpval->libs[0],"Currently there are no image values defined file this report.")) return;
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
	selected+=2;
	linescreen(mimgrsrc,selected,0,NULL,savelinebelow,quitlinebelowtest);
}
static void linedelete(RDArsrc *mimgrsrc)
{
	short x,y;
	RDApval *temp,*temp1,*new_fields;
	int value;

	readallwidgets(mimgrsrc);
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&value)) return;
	if(holdpnum<1) return;
	if(holdpnum>1)
	{
		new_fields=Rmalloc((holdpnum-1)*sizeof(RDApval));
		y=0;
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				temp1->symbol_type=temp->symbol_type;
				temp1->type=temp->type;
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				if(temp->fieldname!=NULL)
				{
					temp1->fieldname=stralloc(temp->fieldname);
					Rfree(temp->fieldname);
				} else temp1->fieldname=NULL;
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
				if(temp->fieldname!=NULL) Rfree(temp->fieldname);
			}
		}
		Rfree(holdpval);
		holdpval=NULL;
		--holdpnum;
		holdpval=Rmalloc(holdpnum*sizeof(RDApval));
		for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
			++temp,++temp1)
		{
			temp1->symbol_type=temp->symbol_type;
			temp1->type=temp->type;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			if(temp->fieldname!=NULL)
			{
				temp1->fieldname=stralloc(temp->fieldname);
				Rfree(temp->fieldname);
			} else temp1->fieldname=NULL;
		}
		Rfree(new_fields);
	} else if(holdpval!=NULL)
	{
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->fieldname!=NULL) Rfree(temp->fieldname);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	getimgvalues();
	if(value>=imgpval->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mimgrsrc,"IMAGE VALUES",value,imgpval))
		updatersrc(mimgrsrc,"IMAGE VALUES");
	changedimages=TRUE;
}
static void lineedit(RDArsrc *mimgrsrc)
{
	int selected=0;
	int x=0;
	RDApval *p;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
	p=holdpval+selected;
	if(p!=NULL)
	{
		x=p->symbol_type;
		linescreen(mimgrsrc,++selected,x,p->name,saveline,quitlinetest);
	} else {
		linescreen(mimgrsrc,++selected,0,NULL,saveline,quitlinetest);
	}
}
static short verifyline(RDArsrc *mimgrsrc)
{
	char *line=NULL,*hold,*warnmessage=NULL;
	int x,count=0;

	readwidget(mimgrsrc,"LINE");
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&line)) return(FALSE);
	for(x=0,hold=line;x<RDAstrlen(line);++x,++hold)
	{
		if(*hold=='@') ++count;
	}
	if(line!=NULL) Rfree(line);
	if(count!=holdpnum)
	{
		warnmessage=Rmalloc(300);
		sprintf(warnmessage,"The number of line variables [%d] is not equal to the number of specifications [%d] on this line.  Please correct.",holdpnum,count);
		WARNINGDIALOG("WARNING DIALOG SCREEN","Line Variable Error!",warnmessage,NULL,NULL,FALSE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return(FALSE);
	}
	return(TRUE);
}
static void lineverify(RDArsrc *mimgrsrc)
{
	verifyline(mimgrsrc);
}
static void makespaceimage(int pos)
{
	short x,y;
	int z=0;
	RDAimage *temp,*temp1,*new_fields;
	RDApval *a,*b;

	++holdnum;
	new_fields=Rmalloc(holdnum*sizeof(RDAimage));
	y=0;
	for(x=0,temp=holdimage;x<(holdnum-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->type=0;
			temp1->breakname=NULL;
			temp1->line=NULL;
			temp1->test=FALSE;
			temp1->numpvals=0;
			temp1->pvals=NULL;
			temp1->expression=NULL;
			++y;
			temp1=new_fields+y;
		}
		temp1->type=temp->type;
		if(temp->breakname!=NULL)
		{
			temp1->breakname=stralloc(temp->breakname);
			Rfree(temp->breakname);
		} else temp1->breakname=NULL;
		if(temp->line!=NULL)
		{
			temp1->line=stralloc(temp->line);
			Rfree(temp->line);
		} else temp1->line=NULL;
		temp1->test=temp->test;
		temp1->numpvals=temp->numpvals;
		if(temp->pvals!=NULL)
		{
			temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
			for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
			{
				a->symbol_type=b->symbol_type;
				if(b->name!=NULL)
				{
					a->name=stralloc(b->name);
					Rfree(b->name);
				} else a->name=NULL;
				a->type=b->type;
				if(b->fieldname!=NULL)
				{
					a->fieldname=stralloc(b->fieldname);
					Rfree(b->fieldname);
				} else a->fieldname=NULL;
			}
			Rfree(temp->pvals);
		}
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		++y;
	}
	Rfree(holdimage);
	holdimage=Rmalloc(holdnum*sizeof(RDAimage));
	for(x=0,temp1=holdimage,temp=new_fields;x<holdnum;++x,++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->breakname!=NULL)
		{
			temp1->breakname=stralloc(temp->breakname);
			Rfree(temp->breakname);
		} else temp1->breakname=NULL;
		if(temp->line!=NULL)
		{
			temp1->line=stralloc(temp->line);
			Rfree(temp->line);
		} else temp1->line=NULL;
		temp1->test=temp->test;
		temp1->numpvals=temp->numpvals;
		if(temp->pvals!=NULL)
		{
			temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
			for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
			{
				a->symbol_type=b->symbol_type;
				if(b->name!=NULL)
				{
					a->name=stralloc(b->name);
					Rfree(b->name);
				} else a->name=NULL;
				a->type=b->type;
				if(b->fieldname!=NULL)
				{
					a->fieldname=stralloc(b->fieldname);
					Rfree(b->fieldname);
				} else a->fieldname=NULL;
			}
			Rfree(temp->pvals);
		}
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
	}
	Rfree(new_fields);
}
static void getimages()
{
	int x,len=0;
	RDAimage *i;
	char *tmp=NULL;

	if(imgavl!=NULL) freeapplib(imgavl);
	imgavl=APPlibNEW();
	if(holdimage!=NULL)
	{
		for(x=0,i=holdimage;x<holdnum;++x,++i)
		{
			len=(5+RDAstrlen(i->line)+RDAstrlen(i->breakname)+(i->expression!=NULL?3:2)+29+1);
			if(tmp!=NULL)
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"[%5d] [%s] [%s] %s %d",
				(x+1),
				(i->line!=NULL ? i->line:""),
				(i->breakname!=NULL ? i->breakname:""),
				(i->expression!=NULL ? "Yes":"No"),
				(i->line!=NULL ? (RDAstrlen(i->line)):0));
			addAPPlib(imgavl,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(imgavl->numlibs<1)
	{
		addAPPlib(imgavl,"Currently there are no image lines defined file this report.");
	}
}
static void quitmimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x=0;
	RDApval *pval;

	if(mimgrsrc!=NULL)
	{
		killwindow(mimgrsrc);
		free_rsrc(mimgrsrc);
	}
	if(imgpval!=NULL) freeapplib(imgpval);
	if(holdpval!=NULL)
	{
		for(x=0,pval=holdpval;x<holdpnum;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	if(mimgdefaults!=NULL) FreeRDAdefault(mimgdefaults);
}
static void savemimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int value=0,x=0;
	RDAimage *s;
	RDApval *pval,*pval1;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdimage==NULL)
	{
		holdimage=Rmalloc(sizeof(RDAimage));
		holdnum=1;
	}
	s=holdimage+value;
	readallwidgets(mimgrsrc);
	FINDRSCGETINT(mimgrsrc,"BREAK NAMES",&x);
	if(s->breakname!=NULL) Rfree(s->breakname);
	s->breakname=stralloc(brkavl->libs[x]);
	FINDRSCGETSTRING(mimgrsrc,"LINE",&s->line);
	FINDRSCGETSTRING(mimgrsrc,"EXPRESSION",&s->expression);
	if(s->pvals!=NULL)
	{
		for(x=0,pval=s->pvals;x<s->numpvals;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(s->pvals);
		s->numpvals=0;	
	}
	if(holdpval!=NULL)
	{
		s->numpvals=holdpnum;
		s->pvals=Rmalloc(holdpnum*sizeof(RDApval));
		for(x=0,pval1=holdpval,pval=s->pvals;x<holdpnum;++x,++pval1,++pval)
		{
			pval->symbol_type=pval1->symbol_type;
			pval->type=pval1->type;
			if(pval1->name!=NULL)
			{
				pval->name=stralloc(pval1->name);
				Rfree(pval1->name);
			} else pval->name=NULL;
			if(pval1->fieldname!=NULL)
			{
				pval->fieldname=stralloc(pval1->fieldname);
				Rfree(pval1->fieldname);
			} else pval->fieldname=NULL;
		}
		Rfree(holdpval);
		holdpnum=0;
	}
	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",value,imgavl))
		updatersrc(imgrsrc,"IMAGES");
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		changedimages=TRUE;
	}
	quitmimg(mimgrsrc,imgrsrc);
}
static void savemimgedit(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	if(!verifyline(mimgrsrc)) return;
	savemimg(mimgrsrc,imgrsrc);
}
static void savemimgabove(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(!verifyline(mimgrsrc)) return;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&x)) return;
	makespaceimage(x);
	savemimg(mimgrsrc,imgrsrc);
}
static void savemimgbelow(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(!verifyline(mimgrsrc)) return;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&x)) return;
	makespaceimage(++x);
	FINDRSCSETINT(imgrsrc,"IMAGES",x);
	savemimg(mimgrsrc,imgrsrc);
}
static void quitmimgtest(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	readallwidgets(mimgrsrc);
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE!\nDo you want to Save these changes?",savemimgedit,quitmimg,FALSE,2,mimgrsrc,imgrsrc,NULL);
	} else {
		quitmimg(mimgrsrc,imgrsrc);
	}
}
static void quitmimgabovetest(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	readallwidgets(mimgrsrc);
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE!\nDo you want to Save these changes?",savemimgabove,quitmimg,FALSE,2,mimgrsrc,imgrsrc,NULL);
	} else {
		quitmimg(mimgrsrc,imgrsrc);
	}
}
static void quitmimgbelowtest(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	readallwidgets(mimgrsrc);
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE!\nDo you want to Save these changes?",savemimgbelow,quitmimg,FALSE,2,mimgrsrc,imgrsrc,NULL);
	} else {
		quitmimg(mimgrsrc,imgrsrc);
	}
}
static void copyline(RDArsrc *mimgrsrc)
{
	int selected=0,x=0,copyfrom=0;
	RDApval *p;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
        copyfrom=selected;
        selected+=2;
	p=holdpval+copyfrom;
	if(p!=NULL)
	{
		x=(int)p->symbol_type;
		linescreen(mimgrsrc,selected,x,p->name,savelinebelow,quitlinebelowtest);
	} else {
		linescreen(mimgrsrc,selected,0,NULL,savelinebelow,quitlinebelowtest);
	}
}
static void setcp(RDArsrc *r)
{
	int l=0;

	readwidget(r,"LINE");
	l=FINDRSCGETCURSORPOSITION(r,"LINE");
	FINDRSCSETINT(r,"CURSOR POSITION",l);
	updatersrc(r,"CURSOR POSITION");
}
static char *bstringx[2]={"HEADER","FOOTER"};
static void imagescreen(RDArsrc *imgrsrc,int num,char *breakname,char *line,
	char *expression,int numpvals,RDApval *pvals,
	void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	char *temp=NULL;
	int x,y;
	RDApval *pv,*pv1;
	RDAcontrol *c;
	RDArsrc *mimgrsrc=NULL;

	if(brkavl!=NULL) freeapplib(brkavl);
	brkavl=APPlibNEW();
	addAPPlib(brkavl,"REPORT HEADER");
	addAPPlib(brkavl,"PAGE HEADER");
	for(y=0;y<2;++y)
	{
		if(y==1) addAPPlib(brkavl,"DETAIL LINES");
		for(x=0,c=report->control;x<report->numcontrols;++x,++c)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(c->name)+8);
			} else {
				temp=Rmalloc(RDAstrlen(c->name)+8);
			}
			sprintf(temp,"%s %s",c->name,bstringx[y]);
			addAPPlib(brkavl,temp);
		}
	}
	if(temp!=NULL) Rfree(temp);
	addAPPlib(brkavl,"PAGE FOOTER");
	addAPPlib(brkavl,"REPORT FOOTER");
	for(x=0;x<brkavl->numlibs;++x)
	{
		if(!RDAstrcmp(brkavl->libs[x],breakname)) break;
	}
	if(x>=brkavl->numlibs) x=0;
	mimgrsrc=RDArsrcNEW(module,"MAINTAIN FORM IMAGES");
	addstdrsrc(mimgrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addlstrsrc(mimgrsrc,"BREAK NAMES",&x,TRUE,NULL,brkavl->numlibs,
		&brkavl->libs,NULL);
	addstdrsrc(mimgrsrc,"CURSOR POSITION",LONGV,5,&x,FALSE);
	addstdrsrc(mimgrsrc,"LINE",VARIABLETEXT,0,(line!=NULL ? line:""),TRUE);
	FINDRSCSETFUNC(mimgrsrc,"LINE",setcp,NULL);
	addsctrsrc(mimgrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),TRUE);
	if(holdpval!=NULL)
	{
		for(x=0,pv=holdpval;x<holdpnum;++x,++pv)
		{
			if(pv->name!=NULL) Rfree(pv->name);
			if(pv->fieldname!=NULL) Rfree(pv->fieldname);
		}
		Rfree(holdpval);
		holdpval=NULL;
		holdpnum=0;
	}
	holdpnum=numpvals;
	if(pvals!=NULL)
	{
		holdpval=Rmalloc(holdpnum*sizeof(RDApval));
		for(x=0,pv=holdpval,pv1=pvals;x<holdpnum;++x,++pv,++pv1)
		{
			pv->symbol_type=pv1->symbol_type;
			if(pv1->name!=NULL)
			{
				pv->name=stralloc(pv1->name);
			} else pv->name=NULL;
			pv->type=pv1->type;
			if(pv1->fieldname!=NULL)
			{
				pv->fieldname=stralloc(pv1->fieldname);
			} else pv->fieldname=NULL;
		}
	} else holdpval=NULL;
	getimgvalues();
	x=0;
	addlstrsrc(mimgrsrc,"IMAGE VALUES",&x,TRUE,NULL,imgpval->numlibs,
		&imgpval->libs,NULL);
	addbtnrsrc(mimgrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"ADD ABOVE",TRUE,lineaddabove,NULL);
	addbtnrsrc(mimgrsrc,"ADD BELOW",TRUE,lineaddbelow,NULL);
	addbtnrsrc(mimgrsrc,"COPY",TRUE,copyline,NULL);
	addbtnrsrc(mimgrsrc,"DELETE",TRUE,linedelete,NULL);
	addbtnrsrc(mimgrsrc,"SELECT",TRUE,lineedit,NULL);
	addbtnrsrc(mimgrsrc,"VERIFY",TRUE,lineverify,NULL);
	addrfcbrsrc(mimgrsrc,"SAVE",TRUE,savefunction,imgrsrc);
	addrfcbrsrc(mimgrsrc,"QUIT",TRUE,quitfunctiontest,imgrsrc);
	addbtnrsrc(mimgrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mimgrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mimgdefaults=GetDefaults(mimgrsrc);
	APPmakescrn(mimgrsrc,TRUE,quitmimg,imgrsrc,FALSE);
	setcp(mimgrsrc);
}
static void imgaddabove(RDArsrc *imgrsrc)
{
	int selected=0;

	if(imgavl->numlibs==1 && !RDAstrcmp(imgavl->libs[0],"Currently there are no image lines defined file this report.")) return;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	imagescreen(imgrsrc,++selected,NULL,NULL,NULL,0,NULL,savemimgabove,quitmimgabovetest);
}
static void imgaddbelow(RDArsrc *imgrsrc)
{
	int selected=0;

	if(imgavl->numlibs==1 && !RDAstrcmp(imgavl->libs[0],"Currently there are no image lines defined file this report.")) return;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	selected+=2;
	imagescreen(imgrsrc,selected,NULL,NULL,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
}
static void imgdelete(RDArsrc *imgrsrc)
{
	short x,y;
	int z;
	RDAimage *temp,*temp1,*new_fields;
	int value;
	RDApval *a,*b;

	readallwidgets(imgrsrc);
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdnum<1) return;
	if(holdnum>1)
	{
		new_fields=Rmalloc((holdnum-1)*sizeof(RDAimage));
		y=0;
		for(x=0,temp=holdimage;x<holdnum;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				temp1->type=temp->type;
				if(temp->breakname!=NULL)
				{
					temp1->breakname=stralloc(temp->breakname);
					Rfree(temp->breakname);
				} else temp1->breakname=NULL;
				if(temp->line!=NULL)
				{
					temp1->line=stralloc(temp->line);
					Rfree(temp->line);
				} else temp1->line=NULL;
				temp1->test=temp->test;
				temp1->numpvals=temp->numpvals;
				if(temp->pvals!=NULL)
				{
					temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
					temp1->numpvals=temp->numpvals;
					for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
					{
						a->symbol_type=b->symbol_type;
						if(b->name!=NULL)
						{
							a->name=stralloc(b->name);
							Rfree(b->name);
						} else a->name=NULL;
						a->type=b->type;
						if(b->fieldname!=NULL)
						{
							a->fieldname=stralloc(b->fieldname);
							Rfree(b->fieldname);
						} else a->fieldname=NULL;
					}
					Rfree(temp->pvals);
				}
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				++y;
			} else {
				if(temp->breakname!=NULL) Rfree(temp->breakname);
				if(temp->line!=NULL) Rfree(temp->line);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->pvals!=NULL)
				{
					for(z=0,a=temp->pvals;z<temp->numpvals;++z,++a)
					{
						if(a->fieldname!=NULL) Rfree(a->fieldname);
						if(a->name!=NULL) Rfree(a->name);
					}
					Rfree(temp->pvals);
				}
			}
		}
		Rfree(holdimage);
		--holdnum;
		holdimage=Rmalloc(holdnum*sizeof(RDAimage));
		for(x=0,temp1=holdimage,temp=new_fields;x<holdnum;++x,
			++temp,++temp1)
		{
			temp1->type=temp->type;
			if(temp->breakname!=NULL)
			{
				temp1->breakname=stralloc(temp->breakname);
				Rfree(temp->breakname);
			} else temp1->breakname=NULL;
			if(temp->line!=NULL)
			{
				temp1->line=stralloc(temp->line);
				Rfree(temp->line);
			} else temp1->line=NULL;
			temp1->test=temp->test;
			temp1->numpvals=temp->numpvals;
			if(temp->pvals!=NULL)
			{
				temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
				temp1->numpvals=temp->numpvals;
				for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
				{
					a->symbol_type=b->symbol_type;
					if(b->name!=NULL)
					{
						a->name=stralloc(b->name);
						Rfree(b->name);
					} else a->name=NULL;
					a->type=b->type;
					if(b->fieldname!=NULL)
					{
						a->fieldname=stralloc(b->fieldname);
						Rfree(b->fieldname);
					} else a->fieldname=NULL;
				}
				Rfree(temp->pvals);
			}
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
		}
		Rfree(new_fields);
	} else if(holdimage!=NULL)
	{
		for(x=0,temp=holdimage;x<holdnum;++x,++temp)
		{
			if(temp->breakname!=NULL) Rfree(temp->breakname);
			if(temp->line!=NULL) Rfree(temp->line);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->pvals!=NULL)
			{
				for(y=0,a=temp->pvals;y<temp->numpvals;++y,++a)
				{
					if(a->fieldname!=NULL) Rfree(a->fieldname);
					if(a->name!=NULL) Rfree(a->name);
				}
				Rfree(temp->pvals);
			}
		}	
		Rfree(holdimage);
		holdnum=0;
	}
	getimages();
	if(value>=imgavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",value,imgavl))
		updatersrc(imgrsrc,"IMAGES");
	changedimages=TRUE;
}
static void copyimage(RDArsrc *imgrsrc)
{
	int selected=0;
	RDAimage *i;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	selected+=2;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,selected,i->breakname,i->line,
			i->expression,i->numpvals,i->pvals,savemimgbelow,quitmimgbelowtest);
	} else {
		imagescreen(imgrsrc,selected,NULL,NULL,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
	}
}
static void imgmoveup(RDArsrc *imgrsrc)
{
	int selected=0,z=0;
	RDAimage *i;
	RDAimage *i2;
	RDAimage itemp;
	RDApval *a,*b;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	if((i == NULL) || (selected <= 0)) return;
	selected--;
	i2=holdimage+selected;

	itemp.type=i2->type;
	if(i2->breakname!=NULL)
	{
		itemp.breakname=stralloc(i2->breakname);
		Rfree(i2->breakname);
	} else itemp.breakname=NULL;
	if(i2->line!=NULL)
	{
		itemp.line=stralloc(i2->line);
		Rfree(i2->line);
	} else itemp.line=NULL;
	itemp.test=i2->test;
	itemp.numpvals=i2->numpvals;
	if(i2->expression!=NULL)
	{
		itemp.expression=stralloc(i2->expression);
		Rfree(i2->expression);
	} else itemp.expression=NULL;
	if(i2->pvals!=NULL)
	{
		itemp.pvals=Rmalloc(i2->numpvals*sizeof(RDApval));
		for(z=0,a=itemp.pvals,b=i2->pvals;z<i2->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i2->pvals);
	} else itemp.pvals = NULL;

	i2->type=i->type;
	if(i->breakname!=NULL)
	{
		i2->breakname=stralloc(i->breakname);
		Rfree(i->breakname);
	} else i2->breakname=NULL;
	if(i->line!=NULL)
	{
		i2->line=stralloc(i->line);
		Rfree(i->line);
	} else i2->line=NULL;
	i2->test=i->test;
	i2->numpvals=i->numpvals;
	if(i->expression!=NULL)
	{
		i2->expression=stralloc(i->expression);
		Rfree(i->expression);
	} else i2->expression=NULL;
	if(i->pvals!=NULL)
	{
		i2->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
		for(z=0,a=i2->pvals,b=i->pvals;z<i->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i->pvals);
	} else i2->pvals = NULL;

	i->type=itemp.type;
	if(itemp.breakname!=NULL)
	{
		i->breakname=stralloc(itemp.breakname);
		Rfree(itemp.breakname);
	} else i->breakname=NULL;
	if(itemp.line!=NULL)
	{
		i->line=stralloc(itemp.line);
		Rfree(itemp.line);
	} else i->line=NULL;
	i->test=itemp.test;
	i->numpvals=itemp.numpvals;
	if(itemp.expression!=NULL)
	{
		i->expression=stralloc(itemp.expression);
		Rfree(itemp.expression);
	} else i->expression=NULL;
	if(itemp.pvals!=NULL)
	{
		i->pvals=Rmalloc(itemp.numpvals*sizeof(RDApval));
		for(z=0,a=i->pvals,b=itemp.pvals;z<itemp.numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(itemp.pvals);
	} else i->pvals = NULL;

	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",selected,imgavl))
	{
		updatersrc(imgrsrc,"IMAGES");
	}
	changedimages=TRUE;
}
static void imgmovedown(RDArsrc *imgrsrc)
{
	int selected=0,z=0;
	RDAimage *i;
	RDAimage *i2;
	RDAimage itemp;
	RDApval *a,*b;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	if((i == NULL) || (selected >= (imgavl->numlibs-1))) return;
	selected++;
	i2=holdimage+selected;

	itemp.type=i2->type;
	if(i2->breakname!=NULL)
	{
		itemp.breakname=stralloc(i2->breakname);
		Rfree(i2->breakname);
	} else itemp.breakname=NULL;
	if(i2->line!=NULL)
	{
		itemp.line=stralloc(i2->line);
		Rfree(i2->line);
	} else itemp.line=NULL;
	itemp.test=i2->test;
	itemp.numpvals=i2->numpvals;
	if(i2->expression!=NULL)
	{
		itemp.expression=stralloc(i2->expression);
		Rfree(i2->expression);
	} else itemp.expression=NULL;
	if(i2->pvals!=NULL)
	{
		itemp.pvals=Rmalloc(i2->numpvals*sizeof(RDApval));
		for(z=0,a=itemp.pvals,b=i2->pvals;z<i2->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i2->pvals);
	} else itemp.pvals = NULL;

	i2->type=i->type;
	if(i->breakname!=NULL)
	{
		i2->breakname=stralloc(i->breakname);
		Rfree(i->breakname);
	} else i2->breakname=NULL;
	if(i->line!=NULL)
	{
		i2->line=stralloc(i->line);
		Rfree(i->line);
	} else i2->line=NULL;
	i2->test=i->test;
	i2->numpvals=i->numpvals;
	if(i->expression!=NULL)
	{
		i2->expression=stralloc(i->expression);
		Rfree(i->expression);
	} else i2->expression=NULL;
	if(i->pvals!=NULL)
	{
		i2->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
		for(z=0,a=i2->pvals,b=i->pvals;z<i->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i->pvals);
	} else i2->pvals = NULL;

	i->type=itemp.type;
	if(itemp.breakname!=NULL)
	{
		i->breakname=stralloc(itemp.breakname);
		Rfree(itemp.breakname);
	} else i->breakname=NULL;
	if(itemp.line!=NULL)
	{
		i->line=stralloc(itemp.line);
		Rfree(itemp.line);
	} else i->line=NULL;
	i->test=itemp.test;
	i->numpvals=itemp.numpvals;
	if(itemp.expression!=NULL)
	{
		i->expression=stralloc(itemp.expression);
		Rfree(itemp.expression);
	} else i->expression=NULL;
	if(itemp.pvals!=NULL)
	{
		i->pvals=Rmalloc(itemp.numpvals*sizeof(RDApval));
		for(z=0,a=i->pvals,b=itemp.pvals;z<itemp.numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(itemp.pvals);
	} else i->pvals = NULL;

	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",selected,imgavl))
	{
		updatersrc(imgrsrc,"IMAGES");
	}
	changedimages=TRUE;
}
static void imgedit(RDArsrc *imgrsrc)
{
	int selected=0;
	RDAimage *i;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,++selected,i->breakname,i->line,
			i->expression,i->numpvals,i->pvals,savemimgedit,quitmimgtest);
	} else {
		imagescreen(imgrsrc,++selected,NULL,NULL,NULL,0,NULL,savemimgedit,quitmimgtest);
	}
}
static void quitimg(RDArsrc *imgrsrc)
{
	int x,y;
	RDAimage *i;
	RDApval *l;

	if(imgrsrc!=NULL)
	{
		killwindow(imgrsrc);
		free_rsrc(imgrsrc);
	}
	if(imgavl!=NULL) freeapplib(imgavl);
	if(holdimage!=NULL)
	{
		for(x=0,i=holdimage;x<holdnum;++x,++i)
		{
			if(i->breakname!=NULL) Rfree(i->breakname);
			if(i->line!=NULL) Rfree(i->line);
			if(i->expression!=NULL) Rfree(i->expression);
			if(i->pvals!=NULL)
			{
				for(y=0,l=i->pvals;y<i->numpvals;++y,++l)
				{
					if(l->fieldname!=NULL) Rfree(l->fieldname);
					if(l->name!=NULL) Rfree(l->name);
				}
				Rfree(i->pvals);
			}
		}	
		Rfree(holdimage);
		holdnum=0;
	}
	if(brkavl!=NULL) freeapplib(brkavl);
	if(changedimages) changedreport=TRUE;
	if(imgdefaults!=NULL) FreeRDAdefault(imgdefaults);
}
static void quitimgclr(RDArsrc *imgrsrc)
{
	changedimages=FALSE;
	quitimg(imgrsrc);
}
static void saveimg(RDArsrc *imgrsrc)
{
	int x,y;
	RDAimage *i,*j;
	RDApval *l,*m;

	readallwidgets(imgrsrc);
	if(report->image!=NULL)
	{
		for(x=0,i=report->image;x<report->numimages;++x,++i)
		{
			if(i->breakname!=NULL) Rfree(i->breakname);
			if(i->line!=NULL) Rfree(i->line);
			if(i->expression!=NULL) Rfree(i->expression);
			if(i->pvals!=NULL)
			{
				for(y=0,l=i->pvals;y<i->numpvals;++y,++l)
				{
					if(l->fieldname!=NULL) Rfree(l->fieldname);
					if(l->name!=NULL) Rfree(l->name);
				}
				Rfree(i->pvals);
			}
		}	
		Rfree(report->image);
		report->numimages=0;
	}
	if(holdimage!=NULL)
	{
		report->image=Rmalloc(holdnum*sizeof(RDAimage));
		report->numimages=holdnum;
		for(x=0,i=holdimage,j=report->image;x<holdnum;++x,++i,++j)
		{
			if(i->breakname!=NULL)
			{
				j->breakname=stralloc(i->breakname);
				Rfree(i->breakname);
			} else j->breakname=NULL;
			if(i->line!=NULL)
			{
				j->line=stralloc(i->line);
				Rfree(i->line);
			} else j->line=NULL;
			if(i->expression!=NULL)
			{
				j->expression=stralloc(i->expression);
				Rfree(i->expression);
			} else j->expression=NULL;
			if(i->pvals!=NULL)
			{
				j->numpvals=i->numpvals;
				j->pvals=Rmalloc(j->numpvals*sizeof(RDApval));
				for(y=0,l=i->pvals,m=j->pvals;y<i->numpvals;++y,++l,++m)
				{
					if(l->name!=NULL)
					{
						m->name=stralloc(l->name);
						Rfree(l->name);
					} else m->name=NULL;
					if(l->fieldname!=NULL)
					{
						m->fieldname=stralloc(l->fieldname);
						Rfree(l->fieldname);
					} else m->fieldname=NULL;
					m->symbol_type=l->symbol_type;
					m->type=l->type;
				}
				Rfree(i->pvals);
			} else {
				j->numpvals=0;
				j->pvals=NULL;
			}
		}
		Rfree(holdimage);
		holdnum=0;
	}
	if(COMPARE_RSRCS(imgrsrc,imgdefaults,0) || changedimages)
	{
		changedimages=TRUE;
	}
	quitimg(imgrsrc);
}
static void savelinefromimage(RDArsrc *imgrsrc)
{
	int value=0,len;
	RDAimage *s;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdimage==NULL)
	{
		holdimage=Rmalloc(sizeof(RDAimage));
		holdnum=1;
	}
	s=holdimage+value;
	if(s==NULL) return;
	readallwidgets(imgrsrc);
	holdpnum=s->numpvals;
	if(!verifyline(imgrsrc)) return;
	FINDRSCGETSTRING(imgrsrc,"LINE",&s->line);
	len=FINDRSCGETCURSORPOSITION(imgrsrc,"LINE");
	FINDRSCSETINT(imgrsrc,"CURSOR POSITION",len);
	updatersrc(imgrsrc,"CURSOR POSITION");
	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",value,imgavl))
	{
		updatersrc(imgrsrc,"IMAGES");
	}
	changedimages=TRUE;
}
static void updateimageline(RDArsrc *imgrsrc)
{
	int value=0;
	RDAimage *s;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdimage==NULL)
	{
		holdimage=Rmalloc(sizeof(RDAimage));
		holdnum=1;
	}
	s=holdimage+value;
	readallwidgets(imgrsrc);
	FINDRSCSETSTRING(imgrsrc,"LINE",s->line);
	updatersrc(imgrsrc,"LINE");
}
static void quitimgtest(RDArsrc *imgrsrc)
{
	readallwidgets(imgrsrc);
	/* Restore the line to blank so clicking an image line doesn't
	 * register as changing the image.
	 */
	FINDRSCSETSTRING(imgrsrc,"LINE","");
	if(COMPARE_RSRCS(imgrsrc,imgdefaults,0) || changedimages)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the IMAGE LINES!\nDo you want to Save these changes?",saveimg,quitimgclr,FALSE,imgrsrc);
	} else {
		quitimg(imgrsrc);
	}
}
void RPTimage1(RDArsrc *rptrsrc)
{
	int x,y;
	RDAimage *i,*j;
	RDApval *l,*m;
	RDArsrc *imgrsrc=NULL;

	if(report->image!=NULL)
	{
		holdnum=report->numimages;
		holdimage=Rmalloc(report->numimages*sizeof(RDAimage));
		for(x=0,j=holdimage,i=report->image;x<report->numimages;++x,++i,++j)
		{
			j->type=i->type;
			j->breakname=stralloc(i->breakname);
			j->line=stralloc(i->line);
			j->numpvals=i->numpvals;
			j->test=i->test;
			j->expression=stralloc(i->expression);
			if(i->pvals!=NULL)
			{
				j->pvals=Rmalloc(j->numpvals*sizeof(RDApval));
				for(y=0,m=j->pvals,l=i->pvals;y<i->numpvals;++y,++m,++l)
				{
					m->symbol_type=l->symbol_type;
					m->name=stralloc(l->name);
					m->type=l->type;
					m->fieldname=stralloc(l->fieldname);
				}
			}
		}
	}
	getimages();
	x=0;
	imgrsrc=RDArsrcNEW(module,"DEFINE IMAGE");
	x=0;
	addstdrsrc(imgrsrc,"CURSOR POSITION",LONGV,5,&x,FALSE);
	/* addition by JAB */
	addstdrsrc(imgrsrc,"LINE",VARIABLETEXT,0,"",TRUE);
	FINDRSCSETFUNC(imgrsrc,"LINE",savelinefromimage,imgrsrc);
	addbtnrsrc(imgrsrc,"SAVE LINE",TRUE,savelinefromimage,NULL);
	/* end addition by JAB */
	addbtnrsrc(imgrsrc,"MOVE UP",TRUE,imgmoveup,NULL);
	addbtnrsrc(imgrsrc,"MOVE DOWN",TRUE,imgmovedown,NULL);
	addlstrsrc(imgrsrc,"IMAGES",&x,TRUE,updateimageline,imgavl->numlibs,
		&imgavl->libs,NULL);
	addbtnrsrc(imgrsrc,"ADD ABOVE",TRUE,imgaddabove,NULL);
	addbtnrsrc(imgrsrc,"ADD BELOW",TRUE,imgaddbelow,NULL);
	addbtnrsrc(imgrsrc,"DELETE",TRUE,imgdelete,NULL);
	addbtnrsrc(imgrsrc,"SELECT",TRUE,imgedit,NULL);
	addbtnrsrc(imgrsrc,"COPY IMAGE",TRUE,copyimage,NULL);
	addrfcbrsrc(imgrsrc,"SAVE",TRUE,saveimg,NULL);
	addrfcbrsrc(imgrsrc,"QUIT",TRUE,quitimgtest,NULL);
	addbtnrsrc(imgrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(imgrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	updateimageline(imgrsrc);
	changedimages=FALSE;
	imgdefaults=GetDefaults(imgrsrc);
	APPmakescrn(imgrsrc,TRUE,quitimg,NULL,FALSE);
}
