/* dfmtnact.c - Define Maintain Master Actions */
#include <cstdio>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <ldval.hpp>

extern MaintainMaster *MTNMSTR;
extern char *module,changedvalues;
extern APPlib *actstructavl;
extern void loadvaluemtn(RDArsrc *,char *);
static APPlib *actionsavl=NULL;
static APPlib *tonames=NULL,*resourcenames=NULL;
static APPlib *actiontypes=NULL;
static int holdpnum=0;
static Actionstruct *holdpval=NULL;
static RDAdefault *mimgdefaults=NULL,*editdefaults=NULL;
static void changeACTIONtype(RDArsrc *);
static void copyactionstructure(RDArsrc *);
static char changedimages=FALSE;
void getActions(void);
extern MTNAction *tempActions;
extern int tmpnumActions;
extern char *ActionTypeList[5];
extern void addholdvaluesact(APPlib *,char *,char *);

static void getimgvalues()
{
	int x,len=0;
	Actionstruct *p;
	char *tmp=NULL;

	if(actionsavl!=NULL) freeapplib(actionsavl);
	actionsavl=APPlibNEW();
	if(holdpval!=NULL)
	{
		for(x=0,p=holdpval;x<holdpnum;++x,++p)
		{
			len=RDAstrlen(p->to_name)+RDAstrlen(ActionTypeList[p->typelist])+28;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"%2d Acting On [%s] type: [%s]",x+1,(p->to_name!=NULL ? p->to_name:""),
				ActionTypeList[p->typelist]);
			addAPPlib(actionsavl,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(actionsavl->numlibs<1)
	{
		addAPPlib(actionsavl,"Currently this action has no defined actions.");
	}
}
static void makespaceactionsavl(int pos)
{
	short x,y;
	Actionstruct *temp,*temp1,*new_fields;

	++holdpnum;
	new_fields=Rmalloc(holdpnum*sizeof(Actionstruct));
	y=0;
	for(x=0,temp=holdpval;x<(holdpnum-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->to_name=NULL;
			temp1->expression=NULL;
			temp1->typelist=0;
			++y;
			temp1=new_fields+y;
		}
		if(temp->to_name!=NULL)
		{
			temp1->to_name=stralloc(temp->to_name);
			Rfree(temp->to_name);
		} else temp1->to_name=NULL;
		temp1->typelist=temp->typelist;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		++y;
	}
	Rfree(holdpval);
	holdpval=Rmalloc(holdpnum*sizeof(Actionstruct));
	for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
		++temp,++temp1)
	{
		if(temp->to_name!=NULL)
		{
			temp1->to_name=stralloc(temp->to_name);
			Rfree(temp->to_name);
		} else temp1->to_name=NULL;
		temp1->typelist=temp->typelist;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
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
	if(tonames!=NULL) freeapplib(tonames);
	if(actiontypes!=NULL) freeapplib(actiontypes);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
}
static void saveline(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int value=0;
	int x=0;
	Actionstruct *p;

	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&value)) return;
	if(holdpval==NULL) 
	{
		holdpval=Rmalloc(sizeof(Actionstruct));
		holdpnum=1;
		value=0;
	}
	p=holdpval+value;
	readallwidgets(editrsrc);
	x=0;
	FINDRSCGETINT(editrsrc,"ACTION TYPES",&x);
	p->typelist=(short)x;
	x=0;
	if(p->expression!=NULL && RDAstrlen(p->expression)>0) Rfree(p->expression);
	FINDRSCGETSTRING(editrsrc,"EXPRESSION",&p->expression);
	FINDRSCGETINT(editrsrc,"RESOURCE NAMES",&x);
	if(p->to_name!=NULL) Rfree(p->to_name);
	p->to_name=stralloc(tonames->libs[x]);
	getimgvalues();
	if(!FINDRSCLISTAPPlib(mimgrsrc,"ACTIONS",value,actionsavl))
		updatersrc(mimgrsrc,"ACTIONS");
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedimages=TRUE;
	}
	quitline(editrsrc,mimgrsrc);
}
static void savelineabove(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&x)) return;
	makespaceactionsavl(x);
	saveline(editrsrc,mimgrsrc);
}
static void savelinebelow(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&x)) return;
	makespaceactionsavl(++x);
	FINDRSCSETINT(mimgrsrc,"ACTIONS",x);
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
static void linescreen(RDArsrc *mimgrsrc,int num,char *fldname,char *expression,
	int type,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0;
/*
	int y,z;
	char *temp=NULL;
	NRDfield *fields=NULL,*f=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short engine=0,nofields=0,nokeys=0;
*/
	RDArsrc *editrsrc=NULL;

	if(actiontypes!=NULL) freeapplib(actiontypes);
	actiontypes=APPlibNEW();
	for(x=0;x<NE(ActionTypeList);++x)
	{
		addAPPlib(actiontypes,ActionTypeList[x]);
	}
	if(tonames!=NULL) freeapplib(tonames);
	tonames=APPlibNEW();
	if(MTNMSTR->module!=NULL && RDAstrlen(MTNMSTR->module)>0 && MTNMSTR->mainfile!=NULL && RDAstrlen(MTNMSTR->mainfile)>0)
	{
		addholdvaluesact(tonames,MTNMSTR->module,MTNMSTR->mainfile);
/*
		if(!GETBIN(MTNMSTR->module,MTNMSTR->mainfile,&engine,&nofields,&fields,&nokeys,&keys)) 
		{
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(MTNMSTR->mainfile)+7);
					} else {
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(MTNMSTR->module)+RDAstrlen(MTNMSTR->mainfile)+7);
					}
					sprintf(temp,"[%s][%s]",MTNMSTR->mainfile,f->name);
					addAPPlib(tonames,temp);
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
*/
	} else {
		prterr("Error Module [%s] File [%s] names in error cannot display fields.",(MTNMSTR->module!=NULL ? MTNMSTR->module:"NULL"),(MTNMSTR->mainfile!=NULL ? MTNMSTR->mainfile:"NULL"));
	}
	if(tonames->numlibs<1)
	{
		addAPPlib(tonames,"No Fields Available");
	} else SORTAPPlib(tonames);
	editrsrc=RDArsrcNEW(module,"MAINTAIN NEW ACTION");
	addstdrsrc(editrsrc,"NUMBER",LONGV,8,&num,FALSE);
	x=0;
	addlstrsrc(editrsrc,"ACTION TYPES",&type,TRUE,changeACTIONtype,
		actiontypes->numlibs,&actiontypes->libs,NULL);
	x=0;
	for(x=0;x<tonames->numlibs;++x)
	{
		if(!RDAstrcmp(tonames->libs[x],fldname)) break;
	}
	if(x>=tonames->numlibs) x=0;
	addlstrsrc(editrsrc,"RESOURCE NAMES",&x,TRUE,NULL,tonames->numlibs,
		&tonames->libs,NULL);
	addsctrsrc(editrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),
		TRUE);
	addbtnrsrc(editrsrc,"LOAD VALUE",TRUE,loadvaluemtn,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	if(type==3 || type==4)
	{
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",FALSE);
	}
	addrfcbrsrc(editrsrc,"SAVE",TRUE,savefunc,mimgrsrc);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quitfunctest,mimgrsrc);
	addbtnrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	editdefaults=GetDefaults(editrsrc);
	APPmakescrn(editrsrc,TRUE,quitline,mimgrsrc,FALSE);
}
static void changeACTIONtype(RDArsrc *editrsrc)
{
	int stype=0;

	FINDRSCGETINT(editrsrc,"ACTION TYPES",&stype);
	if(stype==3 || stype==4)
	{
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",FALSE);
	} else if(stype==0 || stype==1 || stype==2)
	{
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

	if(actionsavl->numlibs==1 && !RDAstrcmp(actionsavl->libs[0],"Currently there are no Actions Resources Defined.")) return;
	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&selected)) return;
	linescreen(mimgrsrc,++selected,NULL,NULL,0,savelineabove,quitlineabovetest);
}
static void copyactionstructure(RDArsrc *mimgrsrc)
{
	int selected=0,x=0,copyfrom=0;
	Actionstruct *p;

	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&selected)) return;
        copyfrom=selected;
        selected+=2;
	p=holdpval+copyfrom;
	if(p!=NULL)
	{
		x=(int)p->typelist;
		linescreen(mimgrsrc,selected,p->to_name,p->expression,x,savelinebelow,quitlinebelowtest);
	} else {
		linescreen(mimgrsrc,selected,NULL,NULL,0,savelinebelow,quitlinebelowtest);
	}
}
static void lineaddbelow(RDArsrc *mimgrsrc)
{
	int selected=0;

	if(actionsavl->numlibs==1 && !RDAstrcmp(actionsavl->libs[0],"Currently there are no Action Resources Defined.")) return;
	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&selected)) return;
	selected+=2;
	linescreen(mimgrsrc,selected,NULL,NULL,0,savelinebelow,quitlinebelowtest);
}
static void linedelete(RDArsrc *mimgrsrc)
{
	short x,y;
	Actionstruct *temp,*temp1,*new_fields;
	int value;

	readallwidgets(mimgrsrc);
	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&value)) return;
	if(holdpnum<1) return;
	if(holdpnum>1)
	{
		new_fields=Rmalloc((holdpnum-1)*sizeof(Actionstruct));
		y=0;
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				temp1->typelist=temp->typelist;
				if(temp->to_name!=NULL)
				{
					temp1->to_name=stralloc(temp->to_name);
					Rfree(temp->to_name);
				} else temp1->to_name=NULL;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				++y;
			} else {
				if(temp->to_name!=NULL) Rfree(temp->to_name);
				if(temp->expression!=NULL) Rfree(temp->expression);
			}
		}
		Rfree(holdpval);
		--holdpnum;
		holdpval=Rmalloc(holdpnum*sizeof(Actionstruct));
		for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
			++temp,++temp1)
		{
			temp1->typelist=temp->typelist;
			if(temp->to_name!=NULL)
			{
				temp1->to_name=stralloc(temp->to_name);
				Rfree(temp->to_name);
			} else temp1->to_name=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
		}
		Rfree(new_fields);
	} else {
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			if(temp->to_name!=NULL) Rfree(temp->to_name);
			if(temp->expression!=NULL) Rfree(temp->expression);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	getimgvalues();
	if(value>=actionsavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mimgrsrc,"ACTIONS",value,actionsavl))
		updatersrc(mimgrsrc,"ACTIONS");
	changedimages=TRUE;
}
static void lineedit(RDArsrc *mimgrsrc)
{
	int selected=0,x=0;
	Actionstruct *p=NULL;

	if(FINDRSCGETINT(mimgrsrc,"ACTIONS",&selected)) return;
	if(holdpval==NULL)
	{
		holdpval=Rmalloc(sizeof(Actionstruct));
		p=holdpval;
		p->to_name=NULL;
		p->expression=NULL;
		p->typelist=0;
		holdpnum=1;
	}
	p=holdpval+selected;
	if(p!=NULL)
	{
		x=p->typelist;
		linescreen(mimgrsrc,++selected,p->to_name,p->expression,x,saveline,quitlinetest);
	} else {
		linescreen(mimgrsrc,++selected,NULL,NULL,0,saveline,quitlinetest);
	}
}
static void makespaceimage(int pos)
{
	short x,y;
	int z=0;
	MTNAction *temp,*temp1,*new_fields;
	Actionstruct *a,*b;

	++tmpnumActions;
	new_fields=Rmalloc(tmpnumActions*sizeof(MTNAction));
	y=0;
	for(x=0,temp=tempActions;x<(tmpnumActions-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->num=0;
			temp1->a=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->a!=NULL)
		{
			temp1->num=temp->num;
			temp1->a=Rmalloc(temp->num*sizeof(Actionstruct));
			for(z=0,a=temp1->a,b=temp->a;z<temp1->num;++z,++a,++b)
			{
				a->typelist=b->typelist;
				if(b->to_name!=NULL)
				{
					a->to_name=stralloc(b->to_name);
					Rfree(b->to_name);
				} else a->to_name=NULL;
				if(b->expression!=NULL)
				{
					a->expression=stralloc(b->expression);
					Rfree(b->expression);
				} else a->expression=NULL;
			}
			
		} else {
			temp1->num=0;
			temp1->a=NULL;
		}
		++y;
	}
	Rfree(tempActions);
	tempActions=Rmalloc(tmpnumActions*sizeof(MTNAction));
	for(x=0,temp1=tempActions,temp=new_fields;x<tmpnumActions;++x,++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->a!=NULL)
		{
			temp1->num=temp->num;
			temp1->a=Rmalloc(temp->num*sizeof(Actionstruct));
			for(z=0,a=temp1->a,b=temp->a;z<temp1->num;++z,++a,++b)
			{
				if(b->to_name!=NULL)
				{
					a->to_name=stralloc(b->to_name);
					Rfree(b->to_name);
				} else a->to_name=NULL;
				a->typelist=b->typelist;
				if(b->expression!=NULL)
				{
					a->expression=stralloc(b->expression);
					Rfree(b->expression);
				} else a->expression=NULL;
			}
			
		} else {
			temp1->num=0;
			temp1->a=NULL;
		}
	}
	Rfree(new_fields);
}
void getActions(void)
{
	int x,len=0;
	MTNAction *i;
	char *tmp=NULL;

	if(actstructavl!=NULL) freeapplib(actstructavl);
	actstructavl=APPlibNEW();
	if(tempActions!=NULL)
	{
		for(x=0,i=tempActions;x<tmpnumActions;++x,++i)
		{
			len=RDAstrlen(i->name)+13;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"%2d [%s]",x+1,
				(i->name!=NULL ? i->name:""));
			addAPPlib(actstructavl,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(actstructavl->numlibs<1)
	{
		addAPPlib(actstructavl,"No Actions Available.");
	}
}
static void quitmimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x=0;
	Actionstruct *pval;

	if(mimgrsrc!=NULL)
	{
		killwindow(mimgrsrc);
		free_rsrc(mimgrsrc);
	}
	if(actionsavl!=NULL) freeapplib(actionsavl);
	if(resourcenames!=NULL) freeapplib(resourcenames);
	if(holdpval!=NULL)
	{
		for(x=0,pval=holdpval;x<holdpnum;++x,++pval)
		{
			if(pval->to_name!=NULL) Rfree(pval->to_name);
			if(pval->expression!=NULL) Rfree(pval->expression);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	if(mimgdefaults!=NULL) FreeRDAdefault(mimgdefaults);
}
static void savemimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int value=0,x=0;
	MTNAction *s=NULL;
	Actionstruct *pval,*pval1;

	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&value)) return;
	if(tempActions==NULL) 
	{
		tempActions=Rmalloc(sizeof(MTNAction));
		tmpnumActions=1;
	}
	s=tempActions+value;
	readallwidgets(mimgrsrc);
	FINDRSCGETINT(mimgrsrc,"RESOURCE",&x);
	if(s->name!=NULL) Rfree(s->name);
	s->name=stralloc(resourcenames->libs[x]);
	if(s->a!=NULL)
	{
		for(x=0,pval=s->a;x<s->num;++x,++pval)
		{
			if(pval->to_name!=NULL) Rfree(pval->to_name);
			if(pval->expression!=NULL) Rfree(pval->expression);
		}
		Rfree(s->a);
		s->num=0;	
	}
	if(holdpval!=NULL)
	{
		s->num=holdpnum;
		s->a=Rmalloc(holdpnum*sizeof(Actionstruct));
		for(x=0,pval1=holdpval,pval=s->a;x<holdpnum;++x,++pval1,++pval)
		{
			pval->typelist=pval1->typelist;
			if(pval1->to_name!=NULL)
			{
				pval->to_name=stralloc(pval1->to_name);
				Rfree(pval1->to_name);
			} else pval->to_name=NULL;
			if(pval1->expression!=NULL)
			{
				pval->expression=stralloc(pval1->expression);
				Rfree(pval1->expression);
			} else pval->expression=NULL;
		}
		Rfree(holdpval);
		holdpnum=0;
	}
	getActions();
	if(!FINDRSCLISTAPPlib(imgrsrc,"ACTION LIST",value,actstructavl))
		updatersrc(imgrsrc,"ACTION LIST");
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		changedvalues=TRUE;
	}
	quitmimg(mimgrsrc,imgrsrc);
}
static void savemimgabove(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&x)) return;
	makespaceimage(x);
	savemimg(mimgrsrc,imgrsrc);
}
static void savemimgbelow(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&x)) return;
	makespaceimage(++x);
	FINDRSCSETINT(imgrsrc,"ACTION LIST",x);
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
static void imagescreen(RDArsrc *imgrsrc,int num,char *name,
	int numpvals,Actionstruct *pvals,
	void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	int x=0;
	Actionstruct *pv=NULL,*pv1=NULL;
	RDArsrc *mimgrsrc=NULL;
/*
	int y,z;
	NRDfield *fields=NULL,*f=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short engine=0,nofields=0,nokeys=0;
	char *temp=NULL;
*/

	mimgrsrc=RDArsrcNEW(module,"MAINTAIN ACTIONS");
	addstdrsrc(mimgrsrc,"NUMBER",LONGV,8,&num,FALSE);
	if(resourcenames!=NULL) freeapplib(resourcenames);
	resourcenames=APPlibNEW();
	if(MTNMSTR->module!=NULL && RDAstrlen(MTNMSTR->module)>0 && MTNMSTR->mainfile!=NULL && RDAstrlen(MTNMSTR->mainfile)>0)
	{
		addholdvaluesact(resourcenames,MTNMSTR->module,MTNMSTR->mainfile);
/*
		if(!GETBIN(MTNMSTR->module,MTNMSTR->mainfile,&engine,&nofields,&fields,&nokeys,&keys)) 
		{`
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(MTNMSTR->mainfile)+7);
					} else {
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(MTNMSTR->mainfile)+7);
					}
					sprintf(temp,"[%s][%s]",MTNMSTR->mainfile,f->name);
					addAPPlib(resourcenames,temp);
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
*/
	} else {
		prterr("Error Module [%s] File [%s] names in error cannot display fields.",(MTNMSTR->module!=NULL ? MTNMSTR->module:"NULL"),(MTNMSTR->mainfile!=NULL ? MTNMSTR->mainfile:"NULL"));
	}
	if(resourcenames->numlibs<1)
	{
		addAPPlib(resourcenames,"No Fields Available");
	} else SORTAPPlib(resourcenames);
	x=0;
	for(x=0;x<resourcenames->numlibs;++x)
	{
		if(!RDAstrcmp(resourcenames->libs[x],name)) break;
	}
	if(x>=resourcenames->numlibs) x=0;
	addlstrsrc(mimgrsrc,"RESOURCE",&x,TRUE,NULL,resourcenames->numlibs,
		&resourcenames->libs,NULL);
	if(holdpval!=NULL)
	{
		for(x=0,pv=holdpval;x<holdpnum;++x,++pv)
		{
			if(pv->to_name!=NULL) Rfree(pv->to_name);
			if(pv->expression!=NULL) Rfree(pv->expression);
		}
		Rfree(holdpval);
	}
	if(pvals!=NULL)
	{
		holdpnum=numpvals;
		holdpval=Rmalloc(holdpnum*sizeof(Actionstruct));
		for(x=0,pv=holdpval,pv1=pvals;x<holdpnum;++x,++pv,++pv1)
		{
			pv->typelist=pv1->typelist;
			if(pv1->to_name!=NULL)
			{
				pv->to_name=stralloc(pv1->to_name);
			} else pv->to_name=NULL;
			if(pv1->expression!=NULL)
			{
				pv->expression=stralloc(pv1->expression);
			} else pv->expression=NULL;
		}
	} else {
		holdpnum=0;
		holdpval=NULL;
	}
	getimgvalues();
	x=0;
	addlstrsrc(mimgrsrc,"ACTIONS",&x,TRUE,NULL,actionsavl->numlibs,
		&actionsavl->libs,NULL);
	addbtnrsrc(mimgrsrc,"ADD ABOVE",TRUE,lineaddabove,NULL);
	addbtnrsrc(mimgrsrc,"ADD BELOW",TRUE,lineaddbelow,NULL);
	addbtnrsrc(mimgrsrc,"DELETE",TRUE,linedelete,NULL);
	addbtnrsrc(mimgrsrc,"SELECT",TRUE,lineedit,NULL);
	addbtnrsrc(mimgrsrc,"COPY",TRUE,copyactionstructure,NULL);
	addrfcbrsrc(mimgrsrc,"SAVE",TRUE,savefunction,imgrsrc);
	addrfcbrsrc(mimgrsrc,"QUIT",TRUE,quitfunctiontest,imgrsrc);
	addbtnrsrc(mimgrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mimgrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mimgdefaults=GetDefaults(mimgrsrc);
	APPmakescrn(mimgrsrc,TRUE,quitmimg,imgrsrc,FALSE);
}
void actionaddabove(RDArsrc *imgrsrc)
{
	int selected=0;

	if(actstructavl->numlibs==1 && !RDAstrcmp(actstructavl->libs[0],"Currently there are no Actions defined file this Power Add.")) return;
	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&selected)) return;
	imagescreen(imgrsrc,++selected,NULL,0,NULL,savemimgabove,quitmimgabovetest);
}
void actionaddbelow(RDArsrc *imgrsrc)
{
	int selected=0;

	if(actstructavl->numlibs==1 && !RDAstrcmp(actstructavl->libs[0],"Currently there are no Actions defined file this Power Add.")) return;
	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&selected)) return;
	selected+=2;
	imagescreen(imgrsrc,selected,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
}
void actiondelete(RDArsrc *imgrsrc)
{
	short x,y;
	int z;
	MTNAction *temp,*temp1,*new_fields;
	int value;
	Actionstruct *a,*b;

	if(tmpnumActions<1) return;
	readallwidgets(imgrsrc);
	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&value)) return;
	if(tmpnumActions>1)
	{
		new_fields=Rmalloc((tmpnumActions-1)*sizeof(MTNAction));
		y=0;
		for(x=0,temp=tempActions;x<tmpnumActions;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				temp1->num=temp->num;
				temp1->a=NULL;
				if(temp->a!=NULL)
				{
					temp1->a=Rmalloc(temp->num*sizeof(Actionstruct));
					temp1->num=temp->num;
					for(z=0,a=temp1->a,b=temp->a;z<temp->num;++z,++a,++b)
					{
						a->typelist=b->typelist;
						if(b->to_name!=NULL)
						{
							a->to_name=stralloc(b->to_name);
							Rfree(b->to_name);
						} else a->to_name=NULL;
						if(b->expression!=NULL)
						{
							a->expression=stralloc(b->expression);
							Rfree(b->expression);
						} else a->expression=NULL;
					}
					Rfree(temp->a);
				}
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
				if(temp->a!=NULL)
				{
					for(z=0,a=temp->a;z<temp->num;++z,++a)
					{
						if(a->expression!=NULL) Rfree(a->expression);
						if(a->to_name!=NULL) Rfree(a->to_name);
					}
					Rfree(temp->a);
				}
			}
		}
		Rfree(tempActions);
		--tmpnumActions;
		tempActions=Rmalloc(tmpnumActions*sizeof(MTNAction));
		for(x=0,temp1=tempActions,temp=new_fields;x<tmpnumActions;++x,
			++temp,++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->num=temp->num;
			temp1->a=NULL;
			if(temp->a!=NULL)
			{
				temp1->a=Rmalloc(temp->num*sizeof(Actionstruct));
				temp1->num=temp->num;
				for(z=0,a=temp1->a,b=temp->a;z<temp->num;++z,++a,++b)
				{
					a->typelist=b->typelist;
					if(b->to_name!=NULL)
					{
						a->to_name=stralloc(b->to_name);
						Rfree(b->to_name);
					} else a->to_name=NULL;
					if(b->expression!=NULL)
					{
						a->expression=stralloc(b->expression);
						Rfree(b->expression);
					} else a->expression=NULL;
				}
				Rfree(temp->a);
			}
		}
		Rfree(new_fields);
	} else if(tempActions!=NULL)
	{
		for(x=0,temp=tempActions;x<tmpnumActions;++x,++temp)
		{
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->a!=NULL)
			{
				for(y=0,a=temp->a;y<temp->num;++y,++a)
				{
					if(a->expression!=NULL) Rfree(a->expression);
					if(a->to_name!=NULL) Rfree(a->to_name);
				}
				Rfree(temp->a);
			}
		}	
		Rfree(tempActions);
		tmpnumActions=0;
	}
	getActions();
	if(value>=actstructavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(imgrsrc,"ACTION LIST",value,actstructavl))
		updatersrc(imgrsrc,"ACTION LIST");
	changedvalues=TRUE;
}
void actionedit(RDArsrc *imgrsrc)
{
	int selected=0;
	MTNAction *i;

	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&selected)) return;
	if(tempActions==NULL)
	{
		tempActions=Rmalloc(sizeof(MTNAction));
		i=tempActions;
		i->name=NULL;
		i->num=0;
		i->a=NULL;
		selected=0;
		tmpnumActions=1;
	}
	i=tempActions+selected;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,++selected,i->name,i->num,i->a,
			savemimg,quitmimgtest);
	} else {
		imagescreen(imgrsrc,++selected,NULL,0,NULL,savemimg,quitmimgtest);
	}
}
void actioncopy(RDArsrc *imgrsrc)
{
	int selected=0;
	MTNAction *i;

	if(FINDRSCGETINT(imgrsrc,"ACTION LIST",&selected)) return;
	i=tempActions+selected;
	selected+=2;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,selected,i->name,
			i->num,i->a,savemimgbelow,quitmimgbelowtest);
	} else {
		imagescreen(imgrsrc,selected,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
	}
}
void saveaction(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR)
{
	int x=0,z=0;
	MTNAction *temp1,*temp;
	Actionstruct *confld1,*confld2;

	if(MTNMSTR->Actions!=NULL)
	{
		for(x=0,temp=MTNMSTR->Actions;x<MTNMSTR->numActions;++x,++temp)
		{
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->a!=NULL)
			{
				for(z=0,confld1=temp->a;z<temp->num;++z,++confld1)
				{
					if(confld1->to_name!=NULL) Rfree(confld1->to_name);
					if(confld1->expression!=NULL) Rfree(confld1->expression);
				}
				Rfree(temp->a);
			}
		}
		Rfree(MTNMSTR->Actions);
		MTNMSTR->Actions=NULL;
		MTNMSTR->numActions=0;
	}
	if(tempActions!=NULL)
	{
		MTNMSTR->numActions=tmpnumActions;
		MTNMSTR->Actions=Rmalloc(tmpnumActions*sizeof(MTNAction));
		for(x=0,temp=tempActions,temp1=MTNMSTR->Actions;x<tmpnumActions;++x,++temp,
			++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->num=temp->num;
			temp1->a=NULL;
			if(temp->a!=NULL)
			{
				temp1->a=Rmalloc(temp->num*sizeof(Actionstruct));
				for(z=0,confld1=temp->a,confld2=temp1->a;
					z<temp->num;++z,++confld2,++confld1)
				{
					if(confld1->to_name!=NULL)
					{
						confld2->to_name=stralloc(confld1->to_name);
						Rfree(confld1->to_name);
					} else confld2->to_name=NULL;
					confld2->typelist=confld1->typelist;
					if(confld1->expression!=NULL)
					{
						confld2->expression=stralloc(confld1->expression);
						Rfree(confld1->expression);
					} else confld2->expression=NULL;
				}
				Rfree(temp->a);
			}
		}
		Rfree(tempActions);
		tempActions=NULL;
		tmpnumActions=0;
	}
}
