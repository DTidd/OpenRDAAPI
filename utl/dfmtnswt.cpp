/* dfmtnswt.c - Define Maintain Master Subordinate Writes */
#include <cstdio>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <fin.hpp>
#include <ldval.hpp>

extern RDArsrc *mimgrsrc;
extern void addholdvalues(APPlib *,char *,char *);
extern char *module,changedvalues;
extern APPlib *subwriteavl;
extern APPlib *makefieldvallistwrtmtn(PowerWrite *,int,int,short,char *,char *);
static void loadvalueswt(RDArsrc *,char *);
static APPlib *wrtfldavl=NULL;
static APPlib *fldnames=NULL,*varnames=NULL;
APPlib *holdapplib=NULL;
char got_applib=FALSE;
static APPlib *symboltypes=NULL;
static int holdpnum=0;
static PowerWriteField *holdpval=NULL;
static RDAdefault *mimgdefaults=NULL,*editdefaults=NULL;
static void changeSYMBOLtype(RDArsrc *);
static void copywritefield(RDArsrc *);
static char changedimages=FALSE;
void getSubWrites(void);
extern PowerWrite *tempSubWrites;
extern int tmpnumSubWrites;
static RDArsrc *tempimgrsrc;
extern APPlib *modlstxmtn,*filelstmtn;

static void loadvalueswt(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;
	char *mod=NULL,*fn=NULL;
	int selectedm=0,selectedf=0;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	readwidget(mimgrsrc,"MODULE");
	readwidget(mimgrsrc,"FILENAME");
	FINDRSCGETSTRING(mimgrsrc,"MODULE",&mod);
	FINDRSCGETSTRING(mimgrsrc,"FILENAME",&fn);
	if(got_applib==TRUE && holdapplib!=NULL)
	{
		loadlist=CopyAPPlib(holdapplib);
	} else {
		if(holdapplib!=NULL) freeapplib(holdapplib);
		holdapplib=makefieldvallistwrtmtn(tempSubWrites,tmpnumSubWrites,tmpnumSubWrites,TRUE,mod,fn);
		got_applib=TRUE;
		loadlist=CopyAPPlib(holdapplib);
	}
	if(mod!=NULL) Rfree(mod);
	if(fn!=NULL) Rfree(fn);
	FINDRSCGETINT(tempimgrsrc,"MODULE",&selectedm);
	FINDRSCGETINT(tempimgrsrc,"FILENAME",&selectedf);
	if(RDAstrcmp(modlstxmtn->libs[selectedm],"Contains No Database Libraries"))
	{
		mod=stralloc(modlstxmtn->libs[selectedm]);
	}
	if(RDAstrcmp(filelstmtn->libs[selectedf],"Contains No File Definitions"))
	{
		fn=stralloc(filelstmtn->libs[selectedf]);
	}
	addholdvalues(loadlist,mod,fn);
	if(mod!=NULL) Rfree(mod);
	if(fn!=NULL) Rfree(fn);
	SORTAPPlib(loadlist);  /* this is called again to order the add-ons
				it might be better to add an argument to 
				makeloadvaluelistwrt() for not sorting so it 
				can be done here only. */
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	if(loadlist->numlibs>1)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP",13)) break;
		}
		if(x>=loadlist->numlibs) x=0;
	}
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void getimgvalues()
{
	int x,len=0;
	PowerWriteField *p;
	char *tmp=NULL;

	if(wrtfldavl!=NULL) freeapplib(wrtfldavl);
	wrtfldavl=APPlibNEW();
	if(holdpval!=NULL)
	{
		for(x=0,p=holdpval;x<holdpnum;++x,++p)
		{
			len=RDAstrlen(p->to_name)+RDAstrlen(p->from_name)+24;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"%3d Copy [%s] to [%s]",x+1,(p->from_name!=NULL ? p->from_name:""),
				(p->to_name!=NULL ? p->to_name:""));
			addAPPlib(wrtfldavl,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(wrtfldavl->numlibs<1)
	{
		addAPPlib(wrtfldavl,"Currently there are no Add Write Fields Defined.");
	}
}
static void makespacewrtfldavl(int pos)
{
	short x,y;
	PowerWriteField *temp,*temp1,*new_fields;

	++holdpnum;
	new_fields=Rmalloc(holdpnum*sizeof(PowerWriteField));
	y=0;
	for(x=0,temp=holdpval;x<(holdpnum-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->copy_type=0;
			temp1->to_name=NULL;
			temp1->from_name=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->to_name!=NULL)
		{
			temp1->to_name=stralloc(temp->to_name);
			Rfree(temp->to_name);
		} else temp1->to_name=NULL;
		temp1->copy_type=temp->copy_type;
		if(temp->from_name!=NULL)
		{
			temp1->from_name=stralloc(temp->from_name);
			Rfree(temp->from_name);
		} else temp1->from_name=NULL;
		++y;
	}
	Rfree(holdpval);
	holdpval=Rmalloc(holdpnum*sizeof(PowerWriteField));
	for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
		++temp,++temp1)
	{
		if(temp->to_name!=NULL)
		{
			temp1->to_name=stralloc(temp->to_name);
			Rfree(temp->to_name);
		} else temp1->to_name=NULL;
		temp1->copy_type=temp->copy_type;
		if(temp->from_name!=NULL)
		{
			temp1->from_name=stralloc(temp->from_name);
			Rfree(temp->from_name);
		} else temp1->from_name=NULL;
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
	if(varnames!=NULL) freeapplib(varnames);
	if(fldnames!=NULL) freeapplib(fldnames);
	if(symboltypes!=NULL) freeapplib(symboltypes);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
}
static void saveline(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int value=0;
	int x=0;
	PowerWriteField *p;

	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&value)) return;
	if(holdpval==NULL) 
	{
		holdpval=Rmalloc(sizeof(PowerWriteField));
		holdpnum=1;
		value=0;
	}
	p=holdpval+value;
	readallwidgets(editrsrc);
	x=0;
	FINDRSCGETINT(editrsrc,"SYMBOL TYPES",&x);
	p->copy_type=(short)x;
	x=0;
	if(!p->copy_type)
	{
		FINDRSCGETINT(editrsrc,"VARIABLE NAMES",&x);
		if(p->from_name!=NULL && RDAstrlen(p->from_name)>0) Rfree(p->from_name);
		p->from_name=stralloc(varnames->libs[x]);
	} else {
		if(p->from_name!=NULL && RDAstrlen(p->from_name)>0) Rfree(p->from_name);
		FINDRSCGETSTRING(editrsrc,"EXPRESSION",&p->from_name);
	}
	FINDRSCGETINT(editrsrc,"FIELD NAMES",&x);
	if(p->to_name!=NULL) Rfree(p->to_name);
	p->to_name=stralloc(fldnames->libs[x]);
	getimgvalues();
	if(!FINDRSCLISTAPPlib(mimgrsrc,"WRITE FIELDS",value,wrtfldavl))
		updatersrc(mimgrsrc,"WRITE FIELDS");
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedimages=TRUE;
	}
	quitline(editrsrc,mimgrsrc);
}
static void savelineabove(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&x)) return;
	makespacewrtfldavl(x);
	saveline(editrsrc,mimgrsrc);
}
static void savelinebelow(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&x)) return;
	makespacewrtfldavl(++x);
	FINDRSCSETINT(mimgrsrc,"WRITE FIELDS",x);
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
static char *evaltypes[2]={"Copy Variable to Field","Evaluate Expression"};
static void linescreen(RDArsrc *mimgrsrc,int num,char *fldname,char *varname,
	int type,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x,y,z;
	char *modx=NULL,*filx=NULL,*linex=NULL,*temp=NULL,*expression=NULL;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	short engine=0,nofields=0,nokeys=0;
	RDArsrc *editrsrc=NULL;
	int selectedm=0,selectedf=0;

	if(symboltypes!=NULL) freeapplib(symboltypes);
	symboltypes=APPlibNEW();
	addAPPlib(symboltypes,evaltypes[0]);
	addAPPlib(symboltypes,evaltypes[1]);
	if(fldnames!=NULL) freeapplib(fldnames);
	fldnames=APPlibNEW();
	readwidget(mimgrsrc,"MODULE");
	readwidget(mimgrsrc,"FILENAME");
	FINDRSCGETSTRING(mimgrsrc,"MODULE",&modx);
	FINDRSCGETSTRING(mimgrsrc,"FILENAME",&filx);
	if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
	{
		if(!GETBIN(modx,filx,&engine,&nofields,&fields,&nokeys,&keys)) 
		{
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
					} else {
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modx,filx,f->name);
					addAPPlib(fldnames,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;++z,
							++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
						Rfree(keyx->part);
					}
				}
				Rfree(keys);
			}
		}
	} else {
		prterr("Error Module [%s] File [%s] names in error cannot display fields.",(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
	}
	if(fldnames->numlibs<1)
	{
		addAPPlib(fldnames,"No Fields Available");
	} else SORTAPPlib(fldnames);
	if(linex!=NULL) Rfree(linex);
	editrsrc=RDArsrcNEW(module,"MAINTAIN SUBORDINATE POWER WRITE FIELDS");
	addstdrsrc(editrsrc,"NUMBER",LONGV,8,&num,FALSE);
	x=0;
	if(varnames!=NULL) freeapplib(varnames);
	if(got_applib==TRUE && holdapplib!=NULL)
	{
		varnames=CopyAPPlib(holdapplib);
	} else {
		if(holdapplib!=NULL) freeapplib(holdapplib);
		holdapplib=makefieldvallistwrtmtn(tempSubWrites,tmpnumSubWrites,tmpnumSubWrites,TRUE,modx,filx);
		got_applib=TRUE;
		varnames=CopyAPPlib(holdapplib);
	}
	if(modx!=NULL) Rfree(modx);
	if(filx!=NULL) Rfree(filx);
	FINDRSCGETINT(tempimgrsrc,"MODULE",&selectedm);
	FINDRSCGETINT(tempimgrsrc,"FILENAME",&selectedf);
	if(RDAstrcmp(modlstxmtn->libs[selectedm],"Contains No Database Libraries"))
	{
		modx=stralloc(modlstxmtn->libs[selectedm]);
	}
	if(RDAstrcmp(filelstmtn->libs[selectedf],"Contains No File Definitions"))
	{
		filx=stralloc(filelstmtn->libs[selectedf]);
	}
	addholdvalues(varnames,modx,filx);
	if(modx!=NULL) Rfree(modx);
	if(filx!=NULL) Rfree(filx);
	SORTAPPlib(varnames);  /* this is called again to order the add-ons
				it might be better to add an argument to 
				makeloadvaluelistwrt() for not sorting so it 
				can be done here only. */
	if(varnames->numlibs<1)
	{
		addAPPlib(varnames,"No Field / Values Available");
	}
	x=0;
	addlstrsrc(editrsrc,"SYMBOL TYPES",&type,TRUE,changeSYMBOLtype,
		symboltypes->numlibs,&symboltypes->libs,NULL);
	if(!type)
	{
		for(x=0;x<varnames->numlibs;++x)
		{
			if(!RDAstrcmp(varnames->libs[x],varname)) break;
		}
		if(x>=varnames->numlibs) x=0;
	} else {
		expression=stralloc(varname);
	}
	addlstrsrc(editrsrc,"VARIABLE NAMES",&x,TRUE,NULL,varnames->numlibs,
		&varnames->libs,NULL);
	x=0;
	for(x=0;x<fldnames->numlibs;++x)
	{
		if(!RDAstrcmp(fldnames->libs[x],fldname)) break;
	}
	if(x>=fldnames->numlibs) x=0;
	addlstrsrc(editrsrc,"FIELD NAMES",&x,TRUE,NULL,fldnames->numlibs,&fldnames->libs,NULL);
	addsctrsrc(editrsrc,"EXPRESSION",0,(expression!=NULL?expression:""),TRUE);
	addbtnrsrc(editrsrc,"LOAD VALUE",TRUE,loadvalueswt,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	if(!type)
	{
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",FALSE);
	} else {
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",FALSE);
	}
	addrfcbrsrc(editrsrc,"SAVE",TRUE,savefunc,mimgrsrc);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quitfunctest,mimgrsrc);
	addbtnrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(expression!=NULL) Rfree(expression);
	editdefaults=GetDefaults(editrsrc);
	APPmakescrn(editrsrc,TRUE,quitline,mimgrsrc,FALSE);
}
static void changeSYMBOLtype(RDArsrc *editrsrc)
{
	int stype=0;

	FINDRSCGETINT(editrsrc,"SYMBOL TYPES",&stype);
	if(stype==0)
	{
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",FALSE);
	} else if(stype==1)
	{
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",TRUE);
	}
}
static void lineaddabove(RDArsrc *mimgrsrc)
{
	int selected=0;

	if(wrtfldavl->numlibs==1 && !RDAstrcmp(wrtfldavl->libs[0],"Currently there are no Add Write Fields Defined.")) return;
	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&selected)) return;
	linescreen(mimgrsrc,++selected,NULL,NULL,0,savelineabove,quitlineabovetest);
}
static void copywritefield(RDArsrc *mimgrsrc)
{
	int selected=0,x=0,copyfrom=0;
	PowerWriteField *p;

	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&selected)) return;
        copyfrom=selected;
        selected+=2;
	p=holdpval+copyfrom;
	if(p!=NULL)
	{
		x=(int)p->copy_type;
		linescreen(mimgrsrc,selected,p->to_name,p->from_name,x,savelinebelow,quitlinebelowtest);
	} else {
		linescreen(mimgrsrc,selected,NULL,NULL,0,savelinebelow,quitlinebelowtest);
	}
}
static void lineaddbelow(RDArsrc *mimgrsrc)
{
	int selected=0;

	if(wrtfldavl->numlibs==1 && !RDAstrcmp(wrtfldavl->libs[0],"Currently there are no Add Write Fields Defined.")) return;
	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&selected)) return;
	selected+=2;
	linescreen(mimgrsrc,selected,NULL,NULL,0,savelinebelow,quitlinebelowtest);
}
static void linedelete(RDArsrc *mimgrsrc)
{
	short x,y;
	PowerWriteField *temp,*temp1,*new_fields;
	int value;

	readallwidgets(mimgrsrc);
	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&value)) return;
	if(holdpnum<1) return;
	if(holdpnum>1)
	{
		new_fields=Rmalloc((holdpnum-1)*sizeof(PowerWriteField));
		y=0;
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				temp1->copy_type=temp->copy_type;
				if(temp->to_name!=NULL)
				{
					temp1->to_name=stralloc(temp->to_name);
					Rfree(temp->to_name);
				} else temp1->to_name=NULL;
				if(temp->from_name!=NULL)
				{
					temp1->from_name=stralloc(temp->from_name);
					Rfree(temp->from_name);
				} else temp1->from_name=NULL;
				++y;
			} else {
				if(temp->to_name!=NULL) Rfree(temp->to_name);
				if(temp->from_name!=NULL) Rfree(temp->from_name);
			}
		}
		Rfree(holdpval);
		--holdpnum;
		holdpval=Rmalloc(holdpnum*sizeof(PowerWriteField));
		for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
			++temp,++temp1)
		{
			temp1->copy_type=temp->copy_type;
			if(temp->to_name!=NULL)
			{
				temp1->to_name=stralloc(temp->to_name);
				Rfree(temp->to_name);
			} else temp1->to_name=NULL;
			if(temp->from_name!=NULL)
			{
				temp1->from_name=stralloc(temp->from_name);
				Rfree(temp->from_name);
			} else temp1->from_name=NULL;
		}
		Rfree(new_fields);
	} else {
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			if(temp->to_name!=NULL) Rfree(temp->to_name);
			if(temp->from_name!=NULL) Rfree(temp->from_name);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	getimgvalues();
	if(value>=wrtfldavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mimgrsrc,"WRITE FIELDS",value,wrtfldavl))
		updatersrc(mimgrsrc,"WRITE FIELDS");
	changedimages=TRUE;
}
static void lineedit(RDArsrc *mimgrsrc)
{
	int selected=0,x=0;
	PowerWriteField *p=NULL;

	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&selected)) return;
	if(holdpval==NULL)
	{
		holdpval=Rmalloc(sizeof(PowerWriteField));
		p=holdpval;
		p->to_name=NULL;
		p->from_name=NULL;
		p->copy_type=0;
		holdpnum=1;
	}
	p=holdpval+selected;
	if(p!=NULL)
	{
		x=p->copy_type;
		linescreen(mimgrsrc,++selected,p->to_name,p->from_name,x,saveline,quitlinetest);
	} else {
		linescreen(mimgrsrc,++selected,NULL,NULL,0,saveline,quitlinetest);
	}
}
static void makespaceimage(int pos)
{
	short x,y;
	int z=0;
	PowerWrite *temp,*temp1,*new_fields;
	PowerWriteField *a,*b;

	++tmpnumSubWrites;
	new_fields=Rmalloc(tmpnumSubWrites*sizeof(PowerWrite));
	y=0;
	for(x=0,temp=tempSubWrites;x<(tmpnumSubWrites-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->fileno=(-1);
			temp1->module=NULL;
			temp1->filename=NULL;
			temp1->numflds=0;
			temp1->fields=NULL;
			temp1->expression=NULL;
			++y;
			temp1=new_fields+y;
		}
		temp1->fileno=temp->fileno;
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		if(temp->fields!=NULL)
		{
			temp1->numflds=temp->numflds;
			temp1->fields=Rmalloc(temp->numflds*sizeof(PowerWriteField));
			for(z=0,a=temp1->fields,b=temp->fields;z<temp1->numflds;++z,++a,++b)
			{
				a->copy_type=b->copy_type;
				if(b->to_name!=NULL)
				{
					a->to_name=stralloc(b->to_name);
					Rfree(b->to_name);
				} else a->to_name=NULL;
				if(b->from_name!=NULL)
				{
					a->from_name=stralloc(b->from_name);
					Rfree(b->from_name);
				} else a->from_name=NULL;
			}
			
		} else {
			temp1->numflds=0;
			temp1->fields=NULL;
		}
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		++y;
	}
	Rfree(tempSubWrites);
	tempSubWrites=Rmalloc(tmpnumSubWrites*sizeof(PowerWrite));
	for(x=0,temp1=tempSubWrites,temp=new_fields;x<tmpnumSubWrites;++x,++temp,++temp1)
	{
		temp1->fileno=temp->fileno;
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		if(temp->fields!=NULL)
		{
			temp1->numflds=temp->numflds;
			temp1->fields=Rmalloc(temp->numflds*sizeof(PowerWriteField));
			for(z=0,a=temp1->fields,b=temp->fields;z<temp1->numflds;++z,++a,++b)
			{
				if(b->to_name!=NULL)
				{
					a->to_name=stralloc(b->to_name);
					Rfree(b->to_name);
				} else a->to_name=NULL;
				a->copy_type=b->copy_type;
				if(b->from_name!=NULL)
				{
					a->from_name=stralloc(b->from_name);
					Rfree(b->from_name);
				} else a->from_name=NULL;
			}
			
		} else {
			temp1->numflds=0;
			temp1->fields=NULL;
		}
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
	}
	Rfree(new_fields);
}
void getSubWrites(void)
{
	int x,len=0;
	PowerWrite *i;
	char *tmp=NULL;

	if(subwriteavl!=NULL) freeapplib(subwriteavl);
	subwriteavl=APPlibNEW();
	if(tempSubWrites!=NULL)
	{
		for(x=0,i=tempSubWrites;x<tmpnumSubWrites;++x,++i)
		{
			len=RDAstrlen(i->module)+RDAstrlen(i->filename)+RDAstrlen(i->expression)+19;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"%2d [%s] [%s] [%s]",x+1,
				(i->module!=NULL ? i->module:""),
				(i->filename!=NULL ? i->filename:""),
				(i->expression!=NULL ? i->expression:""));
			addAPPlib(subwriteavl,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(subwriteavl->numlibs<1)
	{
		addAPPlib(subwriteavl,"No Add Write Lines Available.");
	}
}
static void quitmimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x=0;
	PowerWriteField *pval;

	if(mimgrsrc!=NULL)
	{
		killwindow(mimgrsrc);
		free_rsrc(mimgrsrc);
	}
	if(wrtfldavl!=NULL) freeapplib(wrtfldavl);
	if(holdpval!=NULL)
	{
		for(x=0,pval=holdpval;x<holdpnum;++x,++pval)
		{
			if(pval->to_name!=NULL) Rfree(pval->to_name);
			if(pval->from_name!=NULL) Rfree(pval->from_name);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	if(mimgdefaults!=NULL) FreeRDAdefault(mimgdefaults);
}
static void savemimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int value=0,x=0;
	PowerWrite *s;
	PowerWriteField *pval,*pval1;

	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&value)) return;
	if(tempSubWrites==NULL) 
	{
		tempSubWrites=Rmalloc(sizeof(PowerWrite));
		tmpnumSubWrites=1;
	}
	s=tempSubWrites+value;
	readallwidgets(mimgrsrc);
	FINDRSCGETSTRING(mimgrsrc,"MODULE",&s->module);
	FINDRSCGETSTRING(mimgrsrc,"FILENAME",&s->filename);
	FINDRSCGETSTRING(mimgrsrc,"EXPRESSION",&s->expression);
	if(s->fields!=NULL)
	{
		for(x=0,pval=s->fields;x<s->numflds;++x,++pval)
		{
			if(pval->to_name!=NULL) Rfree(pval->to_name);
			if(pval->from_name!=NULL) Rfree(pval->from_name);
		}
		Rfree(s->fields);
		s->numflds=0;	
	}
	if(holdpval!=NULL)
	{
		s->numflds=holdpnum;
		s->fields=Rmalloc(holdpnum*sizeof(PowerWriteField));
		for(x=0,pval1=holdpval,pval=s->fields;x<holdpnum;++x,++pval1,++pval)
		{
			pval->copy_type=pval1->copy_type;
			if(pval1->to_name!=NULL)
			{
				pval->to_name=stralloc(pval1->to_name);
				Rfree(pval1->to_name);
			} else pval->to_name=NULL;
			if(pval1->from_name!=NULL)
			{
				pval->from_name=stralloc(pval1->from_name);
				Rfree(pval1->from_name);
			} else pval->from_name=NULL;
		}
		Rfree(holdpval);
		holdpnum=0;
	}
	getSubWrites();
	if(!FINDRSCLISTAPPlib(imgrsrc,"WRITE LIST",value,subwriteavl))
	{
		updatersrc(imgrsrc,"WRITE LIST");
	}
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		changedvalues=TRUE;
	}
	quitmimg(mimgrsrc,imgrsrc);
}
static void savemimgabove(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&x)) return;
	makespaceimage(x);
	savemimg(mimgrsrc,imgrsrc);
}
static void savemimgbelow(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&x)) return;
	makespaceimage(++x);
	FINDRSCSETINT(imgrsrc,"WRITE LIST",x);
	savemimg(mimgrsrc,imgrsrc);
}
static void quitmimgtest(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	readallwidgets(mimgrsrc);
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE!\nDo you want to Save these changes?",savemimg,quitmimg,FALSE,2,mimgrsrc,imgrsrc,NULL);
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
static void quitmonths(RDArsrc *r)
{
	if(r!=NULL)	
	{
		killwindow(r);
		free_rsrc(r);
	}
}
static void selectmonths(RDArsrc *r,RDArsrc *mimgrsrc)
{
	char *fm_field=NULL,*amt_field=NULL,*month_tail=NULL;
	char *modx=NULL,*filx=NULL;
	PowerWriteField *pf=NULL;
	char temp[1024],fiscal=TRUE;
	int x=0,value=0;

	readallwidgets(r);
	FINDRSCGETSTRING(r,"FISCAL MONTH FIELD",&fm_field);
	FINDRSCGETSTRING(r,"AMOUNT FIELD",&amt_field);
	FINDRSCGETSTRING(r,"MONTHLY FIELD TAIL",&month_tail);
	FINDRSCGETCHAR(r,"FISCAL MONTHS",&fiscal);
	readwidget(mimgrsrc,"MODULE");
	readwidget(mimgrsrc,"FILENAME");
	if(FINDRSCGETINT(mimgrsrc,"WRITE FIELDS",&value)) return;
	FINDRSCGETSTRING(mimgrsrc,"MODULE",&modx);
	FINDRSCGETSTRING(mimgrsrc,"FILENAME",&filx);
	if(fiscal==TRUE)
	{
		for(x=0;x<14;++x)
		{
			if(holdpval!=NULL)
			{
				holdpval=Rrealloc(holdpval,(holdpnum+1)*sizeof(PowerWriteField));
			} else {
				holdpval=Rmalloc(sizeof(PowerWriteField));
				holdpnum=0;
			}
			pf=holdpval+holdpnum;
			pf->copy_type=1;
			memset(temp,0,1024);
			sprintf(temp,"[%s][%s][%s %s]",
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x],
				(month_tail!=NULL ? month_tail:""));
			pf->to_name=stralloc(temp);
			memset(temp,0,1024);
			sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=([%s][%s][%s %s]%s) ELSE RETURN_VALUE=[%s][%s][%s %s] FI",
				(fm_field!=NULL ? fm_field:""),x,(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x],
				(month_tail!=NULL ? month_tail:""),
				(amt_field!=NULL ? amt_field:""),
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x],
				(month_tail!=NULL ? month_tail:""));
			pf->from_name=stralloc(temp);
			++holdpnum;
		}
	} else {
		for(x=0;x<12;++x)
		{
			if(holdpval!=NULL)
			{
				holdpval=Rrealloc(holdpval,(holdpnum+1)*sizeof(PowerWriteField));
			} else {
				holdpval=Rmalloc(sizeof(PowerWriteField));
				holdpnum=0;
			}
			pf=holdpval+holdpnum;
			pf->copy_type=1;
			memset(temp,0,1024);
			sprintf(temp,"[%s][%s][%s %s]",
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x+1],
				(month_tail!=NULL ? month_tail:""));
			pf->to_name=stralloc(temp);
			memset(temp,0,1024);
			sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=([%s][%s][%s %s]%s) ELSE RETURN_VALUE=[%s][%s][%s %s] FI",
				(fm_field!=NULL ? fm_field:""),x,(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x+1],
				(month_tail!=NULL ? month_tail:""),
				(amt_field!=NULL ? amt_field:""),
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x+1],
				(month_tail!=NULL ? month_tail:""));
			pf->from_name=stralloc(temp);
			++holdpnum;
		}
	}
	if(modx!=NULL) Rfree(modx);
	if(filx!=NULL) Rfree(filx);
	if(fm_field!=NULL) Rfree(fm_field);
	if(amt_field!=NULL) Rfree(amt_field);
	if(month_tail!=NULL) Rfree(month_tail);
	getimgvalues();
	if(!FINDRSCLISTAPPlib(mimgrsrc,"WRITE FIELDS",value,wrtfldavl))
		updatersrc(mimgrsrc,"WRITE FIELDS");
	quitmonths(r);
}
static void monthlyfields(RDArsrc *mimgrsrc)
{
	RDArsrc *monthrsrc=NULL;
	char fmonths=TRUE;

	monthrsrc=RDArsrcNEW(module,"CREATE MONTHLY FIELDS");
	addstdrsrc(monthrsrc,"FISCAL MONTHS",BOOLNS,1,&fmonths,TRUE);
	addstdrsrc(monthrsrc,"FISCAL MONTH FIELD",VARIABLETEXT,0,"",TRUE);
	addstdrsrc(monthrsrc,"AMOUNT FIELD",VARIABLETEXT,0,"",TRUE);
	addstdrsrc(monthrsrc,"MONTHLY FIELD TAIL",VARIABLETEXT,0,"",TRUE);
	addrfcbrsrc(monthrsrc,"SELECT",TRUE,selectmonths,mimgrsrc);
	addrfcbrsrc(monthrsrc,"QUIT",TRUE,quitmonths,NULL);
	addbtnrsrc(monthrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(monthrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(monthrsrc,TRUE,quitmonths,NULL,FALSE);
}
static void imagescreen(RDArsrc *imgrsrc,int num,char *modulx,char *filename,
	char *expression,int numpvals,PowerWriteField *pvals,
	void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	int x;
	PowerWriteField *pv,*pv1;

	mimgrsrc=RDArsrcNEW(module,"MAINTAIN SUBORDINATE POWER WRITES");
	tempimgrsrc=imgrsrc;
	addstdrsrc(mimgrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addstdrsrc(mimgrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),TRUE);
	addstdrsrc(mimgrsrc,"FILENAME",VARIABLETEXT,0,(filename!=NULL ? filename:""),TRUE);
	addsctrsrc(mimgrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),TRUE);
	got_applib=FALSE;
	if(holdpval!=NULL)
	{
		for(x=0,pv=holdpval;x<holdpnum;++x,++pv)
		{
			if(pv->to_name!=NULL) Rfree(pv->to_name);
			if(pv->from_name!=NULL) Rfree(pv->from_name);
		}
		Rfree(holdpval);
	}
	if(pvals!=NULL)
	{
		holdpnum=numpvals;
		holdpval=Rmalloc(holdpnum*sizeof(PowerWriteField));
		for(x=0,pv=holdpval,pv1=pvals;x<holdpnum;++x,++pv,++pv1)
		{
			pv->copy_type=pv1->copy_type;
			if(pv1->to_name!=NULL)
			{
				pv->to_name=stralloc(pv1->to_name);
			} else pv->to_name=NULL;
			if(pv1->from_name!=NULL)
			{
				pv->from_name=stralloc(pv1->from_name);
			} else pv->from_name=NULL;
		}
	} else {
		holdpnum=0;
		holdpval=NULL;
	}
	getimgvalues();
	x=0;
	addlstrsrc(mimgrsrc,"WRITE FIELDS",&x,TRUE,NULL,wrtfldavl->numlibs,&wrtfldavl->libs,NULL);
	addbtnrsrc(mimgrsrc,"LOAD VALUE",TRUE,loadvalueswt,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"ADD ABOVE",TRUE,lineaddabove,NULL);
	addbtnrsrc(mimgrsrc,"ADD BELOW",TRUE,lineaddbelow,NULL);
	addbtnrsrc(mimgrsrc,"DELETE",TRUE,linedelete,NULL);
	addbtnrsrc(mimgrsrc,"SELECT",TRUE,lineedit,NULL);
	addbtnrsrc(mimgrsrc,"COPY WRITE FIELD",TRUE,copywritefield,NULL);
	if(XPERT_SETUP->software_type<2)
	{
		addbtnrsrc(mimgrsrc,"MONTHLY FIELDS",TRUE,monthlyfields,NULL);
	}
	addrfcbrsrc(mimgrsrc,"SAVE",TRUE,savefunction,imgrsrc);
	addrfcbrsrc(mimgrsrc,"QUIT",TRUE,quitfunctiontest,imgrsrc);
	addbtnrsrc(mimgrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mimgrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mimgdefaults=GetDefaults(mimgrsrc);
	APPmakescrn(mimgrsrc,TRUE,quitmimg,imgrsrc,FALSE);
}
void wrtaddabove(RDArsrc *imgrsrc)
{
	int selected=0;

	got_applib=FALSE;
	if(subwriteavl->numlibs==1 && !RDAstrcmp(subwriteavl->libs[0],"Currently there are no Add Writes defined file this Power Add.")) return;
	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&selected)) return;
	imagescreen(imgrsrc,++selected,NULL,NULL,NULL,0,NULL,savemimgabove,quitmimgabovetest);
}
void wrtaddbelow(RDArsrc *imgrsrc)
{
	int selected=0;

	got_applib=FALSE;
	if(subwriteavl->numlibs==1 && !RDAstrcmp(subwriteavl->libs[0],"Currently there are no Add Writes defined file this Power Add.")) return;
	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&selected)) return;
	selected+=2;
	imagescreen(imgrsrc,selected,NULL,NULL,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
}
void wrtdelete(RDArsrc *imgrsrc)
{
	short x,y;
	int z;
	PowerWrite *temp,*temp1,*new_fields;
	int value;
	PowerWriteField *a,*b;

	if(tmpnumSubWrites<1) return;
	readallwidgets(imgrsrc);
	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&value)) return;
	if(tmpnumSubWrites>1)
	{
		new_fields=Rmalloc((tmpnumSubWrites-1)*sizeof(PowerWrite));
		y=0;
		for(x=0,temp=tempSubWrites;x<tmpnumSubWrites;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->module!=NULL)
				{
					temp1->module=stralloc(temp->module);
					Rfree(temp->module);
				} else temp1->module=NULL;
				if(temp->filename!=NULL)
				{
					temp1->filename=stralloc(temp->filename);
					Rfree(temp->filename);
				} else temp1->filename=NULL;
				temp1->numflds=temp->numflds;
				if(temp->fields!=NULL)
				{
					temp1->fields=Rmalloc(temp->numflds*sizeof(PowerWriteField));
					temp1->numflds=temp->numflds;
					for(z=0,a=temp1->fields,b=temp->fields;z<temp->numflds;++z,++a,++b)
					{
						a->copy_type=b->copy_type;
						if(b->to_name!=NULL)
						{
							a->to_name=stralloc(b->to_name);
							Rfree(b->to_name);
						} else a->to_name=NULL;
						if(b->from_name!=NULL)
						{
							a->from_name=stralloc(b->from_name);
							Rfree(b->from_name);
						} else a->from_name=NULL;
					}
					Rfree(temp->fields);
				}
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				++y;
			} else {
				if(temp->module!=NULL) Rfree(temp->module);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->fields!=NULL)
				{
					for(z=0,a=temp->fields;z<temp->numflds;++z,++a)
					{
						if(a->from_name!=NULL) Rfree(a->from_name);
						if(a->to_name!=NULL) Rfree(a->to_name);
					}
					Rfree(temp->fields);
				}
			}
		}
		Rfree(tempSubWrites);
		--tmpnumSubWrites;
		tempSubWrites=Rmalloc(tmpnumSubWrites*sizeof(PowerWrite));
		for(x=0,temp1=tempSubWrites,temp=new_fields;x<tmpnumSubWrites;++x,
			++temp,++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
				Rfree(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
				Rfree(temp->filename);
			} else temp1->filename=NULL;
			temp1->numflds=temp->numflds;
			if(temp->fields!=NULL)
			{
				temp1->fields=Rmalloc(temp->numflds*sizeof(PowerWriteField));
				temp1->numflds=temp->numflds;
				for(z=0,a=temp1->fields,b=temp->fields;z<temp->numflds;++z,++a,++b)
				{
					a->copy_type=b->copy_type;
					if(b->to_name!=NULL)
					{
						a->to_name=stralloc(b->to_name);
						Rfree(b->to_name);
					} else a->to_name=NULL;
					if(b->from_name!=NULL)
					{
						a->from_name=stralloc(b->from_name);
						Rfree(b->from_name);
					} else a->from_name=NULL;
				}
				Rfree(temp->fields);
			}
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
		}
		Rfree(new_fields);
	} else if(tempSubWrites!=NULL)
	{
		for(x=0,temp=tempSubWrites;x<tmpnumSubWrites;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->fields!=NULL)
			{
				for(y=0,a=temp->fields;y<temp->numflds;++y,++a)
				{
					if(a->from_name!=NULL) Rfree(a->from_name);
					if(a->to_name!=NULL) Rfree(a->to_name);
				}
				Rfree(temp->fields);
			}
		}	
		Rfree(tempSubWrites);
		tmpnumSubWrites=0;
	}
	getSubWrites();
	if(value>=subwriteavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(imgrsrc,"WRITE LIST",value,subwriteavl))
	{
		updatersrc(imgrsrc,"WRITE LIST");
	}
	changedvalues=TRUE;
}
void wrtedit(RDArsrc *imgrsrc)
{
	int selected=0;
	PowerWrite *i;

	got_applib=FALSE;
	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&selected)) return;
	if(tempSubWrites==NULL)
	{
		tempSubWrites=Rmalloc(sizeof(PowerWrite));
		i=tempSubWrites;
		i->module=NULL;	
		i->filename=NULL;
		i->numflds=0;
		i->fields=NULL;
		i->expression=NULL;
		selected=0;
		tmpnumSubWrites=1;
	}
	i=tempSubWrites+selected;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,++selected,i->module,i->filename,
			i->expression,i->numflds,i->fields,savemimg,quitmimgtest);
	} else {
		imagescreen(imgrsrc,++selected,NULL,NULL,NULL,0,NULL,savemimg,quitmimgtest);
	}
}
void wrtcopy(RDArsrc *imgrsrc)
{
	int selected=0;
	PowerWrite *i;

	got_applib=FALSE;
	if(FINDRSCGETINT(imgrsrc,"WRITE LIST",&selected)) return;
	i=tempSubWrites+selected;
	selected+=2;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,selected,i->module,i->filename,
			i->expression,i->numflds,i->fields,savemimgbelow,quitmimgbelowtest);
	} else {
		imagescreen(imgrsrc,selected,NULL,NULL,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
	}
}
void savewrt(RDAsubord *subs)
{
	int x=0,z=0;
	PowerWrite *temp1,*temp;
	PowerWriteField *confld1,*confld2;

	if(subs->Writes!=NULL)
	{
		for(x=0,temp=subs->Writes;x<subs->numWrites;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->fields!=NULL)
			{
				for(z=0,confld1=temp->fields;z<temp->numflds;++z,++confld1)
				{
					if(confld1->to_name!=NULL) Rfree(confld1->to_name);
					if(confld1->from_name!=NULL) Rfree(confld1->from_name);
				}
				Rfree(temp->fields);
			}
		}
		Rfree(subs->Writes);
		subs->Writes=NULL;
		subs->numWrites=0;
	}
	subs->numWrites=0;
	subs->Writes=NULL;
	if(tempSubWrites!=NULL)
	{
		subs->numWrites=tmpnumSubWrites;
		subs->Writes=Rmalloc(tmpnumSubWrites*sizeof(PowerWrite));
		for(x=0,temp=tempSubWrites,temp1=subs->Writes;x<tmpnumSubWrites;++x,++temp,
			++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
				Rfree(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
				Rfree(temp->filename);
			} else temp1->filename=NULL;
			temp1->fileno=temp->fileno;
			temp1->numflds=temp->numflds;
			if(temp->fields!=NULL)
			{
				temp1->fields=Rmalloc(temp->numflds*sizeof(PowerWriteField));
				for(z=0,confld1=temp->fields,confld2=temp1->fields;
					z<temp->numflds;++z,++confld2,++confld1)
				{
					if(confld1->to_name!=NULL)
					{
						confld2->to_name=stralloc(confld1->to_name);
						Rfree(confld1->to_name);
					} else confld2->to_name=NULL;
					confld2->copy_type=confld1->copy_type;
					if(confld1->from_name!=NULL)
					{
						confld2->from_name=stralloc(confld1->from_name);
						Rfree(confld1->from_name);
					} else confld2->from_name=NULL;
				}
				Rfree(temp->fields);
			}
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
		}
		Rfree(tempSubWrites);
		tmpnumSubWrites=0;
	}
}
