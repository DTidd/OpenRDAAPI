/* dfrptacc.c - Define Accumulators section of Define Reports */
#include <cstdio>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>

extern APPlib *loadlist;
extern RDAreport *report;
extern char *module;
extern void makefieldvallistrpt(APPlib **,short,short,short,short);
extern char *accumtypes[];
extern char changedreport;
static APPlib *atypes=NULL,*accavl=NULL;
static RDAaccum *holdaccm=NULL;
static int holdaccn=0;
static RDAdefault *accdefaults=NULL,*maccdefaults=NULL;
static char changedaccumulators=FALSE;
static void copyaccumulator(RDArsrc *);

static void makespaceaccum(int pos)
{
	short x,y;
	RDAaccum *temp,*temp1,*new_fields;

	++holdaccn;
	new_fields=Rmalloc(holdaccn*sizeof(RDAaccum));
	y=0;
	for(x=0,temp=holdaccm;x<(holdaccn-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->atype=0;
			temp1->accname=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->atype=temp->atype;
		if(temp->accname!=NULL)
		{
			temp1->accname=stralloc(temp->accname);
			Rfree(temp->accname);
		} else temp1->accname=NULL;
		++y;
	}
	Rfree(holdaccm);
	holdaccm=Rmalloc(holdaccn*sizeof(RDAaccum));
	for(x=0,temp1=holdaccm,temp=new_fields;x<holdaccn;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->atype=temp->atype;
		if(temp->accname!=NULL)
		{
			temp1->accname=stralloc(temp->accname);
			Rfree(temp->accname);
		} else temp1->accname=NULL;
	}
	Rfree(new_fields);
}
static void quitmacc(RDArsrc *maccrsrc,RDArsrc *accrsrc)
{
	if(maccrsrc!=NULL)
	{
		killwindow(maccrsrc);
		free_rsrc(maccrsrc);
	}
	if(atypes!=NULL) freeapplib(atypes);
	if(loadlist!=NULL) freeapplib(loadlist);
	if(maccdefaults!=NULL) FreeRDAdefault(maccdefaults);
}
static void getaccums()
{
	int x,len=0;
	RDAaccum *a;
	char *tmp=NULL;

	if(accavl!=NULL) freeapplib(accavl);
	accavl=APPlibNEW();
	for(x=0,a=holdaccm;x<holdaccn;++x,++a)
	{
		len=RDAstrlen(a->name)+RDAstrlen(a->accname)+RDAstrlen(accumtypes[a->atype])+5+7;
		if(tmp!=NULL) 
		{
			tmp=Rrealloc(tmp,len);
		} else {
			tmp=Rmalloc(len);
		}
		memset(tmp,0,len);
		sprintf(tmp,"[%5d] %s %s %s",(x+1),(a->name!=NULL ? a->name:""),accumtypes[a->atype],(a->accname!=NULL ? a->accname:""));
		addAPPlib(accavl,tmp);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(accavl->numlibs<1)
	{
		addAPPlib(accavl,"No Accumulators Defined");
	}
}
static void savemacc(RDArsrc *maccrsrc,RDArsrc *accrsrc)
{
	int value=0,fieldvalue=0;
	RDAaccum *a;

	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&value)) return;
	if(holdaccm==NULL) 
	{
		holdaccm=Rmalloc(sizeof(RDAaccum));
		holdaccn=1;
	}
	a=holdaccm+value;
	readallwidgets(maccrsrc);
	FINDRSCGETSTRING(maccrsrc,"NAME",&a->name);
	FINDRSCGETINT(maccrsrc,"ACCUM TYPES",&a->atype);
	FINDRSCGETINT(maccrsrc,"FIELD / VALUES",&fieldvalue);
	a->accname=stralloc(loadlist->libs[fieldvalue]);
	getaccums();
	if(!FINDRSCLISTAPPlib(accrsrc,"ACCUMULATORS",value,accavl))
		updatersrc(accrsrc,"ACCUMULATORS");
        if(COMPARE_RSRCS(maccrsrc,maccdefaults,2))
	{
		changedaccumulators=TRUE;
	}
	quitmacc(maccrsrc,accrsrc);
}
static void savemaccabove(RDArsrc *maccrsrc,RDArsrc *accrsrc)
{
	int x;

	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&x)) return;
	makespaceaccum(x);
	savemacc(maccrsrc,accrsrc);
}
static void savemaccbelow(RDArsrc *maccrsrc,RDArsrc *accrsrc)
{
	int x;

	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&x)) return;
	makespaceaccum(++x);
	FINDRSCSETINT(accrsrc,"ACCUMULATORS",x);
	savemacc(maccrsrc,accrsrc);
}
static void quitmacctest(RDArsrc *maccrsrc,RDArsrc *accrsrc)
{
        readallwidgets(maccrsrc);
        if(COMPARE_RSRCS(maccrsrc,maccdefaults,2))
        {
                ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this ACCUMULATOR!\nDo you want to Save these changes?",savemacc,quitmacc,FALSE,2,maccrsrc,accrsrc,NULL);
        } else {
                quitmacc(maccrsrc,accrsrc);
        }
}
static void quitmaccabovetest(RDArsrc *maccrsrc,RDArsrc *accrsrc)
{
        readallwidgets(maccrsrc);
        if(COMPARE_RSRCS(maccrsrc,maccdefaults,2))
        {
                ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this ACCUMULATOR!\nDo you want to Save these changes?",savemaccabove,quitmacc,FALSE,2,maccrsrc,accrsrc,NULL);
	} else {
                quitmacc(maccrsrc,accrsrc);
        }
}
static void quitmaccbelowtest(RDArsrc *maccrsrc,RDArsrc *accrsrc)
{
        readallwidgets(maccrsrc);
        if(COMPARE_RSRCS(maccrsrc,maccdefaults,2))
        {
                ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this ACCUMULATOR!\nDo you want to Save these changes?",savemaccbelow,quitmacc,FALSE,2,maccrsrc,accrsrc,NULL);
        } else {
                quitmacc(maccrsrc,accrsrc);
        }
}
static void accumscreen(RDArsrc *accrsrc,int num,char *name,int atypex,
	char *accname,void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	int x;
	RDArsrc *maccrsrc=NULL;

	if(atypes!=NULL) freeapplib(atypes);
	atypes=APPlibNEW();
	for(x=0;x<8;++x) addAPPlib(atypes,accumtypes[x]);
	maccrsrc=RDArsrcNEW(module,"MAINTAIN ACCUMULATORS");
	addstdrsrc(maccrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addstdrsrc(maccrsrc,"NAME",VARIABLETEXT,0,name,TRUE);
	addlstrsrc(maccrsrc,"ACCUM TYPES",&atypex,TRUE,NULL,atypes->numlibs,
		&atypes->libs,NULL);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=APPlibNEW();
	makefieldvallistrpt(&loadlist,TRUE,TRUE,TRUE,FALSE);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"No Fields or Virtual Fields Available");
	}
	for(x=0;x<loadlist->numlibs;++x)
	{
		if(!RDAstrcmp(loadlist->libs[x],accname)) break;
	}
	if(x>=loadlist->numlibs)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=loadlist->numlibs) x=0;
	}
	addlstrsrc(maccrsrc,"FIELD / VALUES",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfcbrsrc(maccrsrc,"SAVE",TRUE,savefunction,accrsrc);
	addrfcbrsrc(maccrsrc,"QUIT",TRUE,quitfunctiontest,accrsrc);
	addbtnrsrc(maccrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(maccrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	maccdefaults=GetDefaults(maccrsrc);
	APPmakescrn(maccrsrc,TRUE,quitmacc,accrsrc,FALSE);
}
static void accaddabove(RDArsrc *accrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&selected)) return;
	if(accavl->numlibs==1 && !RDAstrcmp(accavl->libs[0],"No Accumulators Defined")) return;
	accumscreen(accrsrc,++selected,NULL,0,NULL,savemaccabove,quitmaccabovetest);
}
static void accaddbelow(RDArsrc *accrsrc)
{
	int selected=0;

	if(accavl->numlibs==1 && !RDAstrcmp(accavl->libs[0],"No Accumulators Defined")) return;
	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&selected)) return;
	selected+=2;
	accumscreen(accrsrc,selected,NULL,0,NULL,savemaccbelow,quitmaccbelowtest);
}
static void copyaccumulator(RDArsrc *accrsrc)
{
	int selected=0,copyfrom=0;
	RDAaccum *a;

	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&selected)) return;
	copyfrom=selected;
	selected+=2;
	a=holdaccm+copyfrom;
	if(a!=NULL)
	{
		accumscreen(accrsrc,selected,a->name,a->atype,a->accname,savemaccbelow,quitmaccbelowtest);
	} else {
		accumscreen(accrsrc,selected,NULL,0,NULL,savemaccbelow,quitmaccbelowtest);
	}
}
static void accdelete(RDArsrc *accrsrc)
{
	short x,y;
	RDAaccum *temp,*temp1,*new_fields;
	int value;

	readallwidgets(accrsrc);
	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&value)) return;
	if(holdaccn<1) return;
	if(holdaccn>1)
	{
		new_fields=Rmalloc((holdaccn-1)*sizeof(RDAaccum));
		y=0;
		for(x=0,temp=holdaccm;x<holdaccn;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				temp1->atype=temp->atype;
				if(temp->accname!=NULL)
				{
					temp1->accname=stralloc(temp->accname);
					Rfree(temp->accname);
				} else temp1->accname=NULL;
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
				if(temp->accname!=NULL) Rfree(temp->accname);
			}
		}
		Rfree(holdaccm);
		--holdaccn;
		holdaccm=Rmalloc(holdaccn*sizeof(RDAaccum));
		for(x=0,temp1=holdaccm,temp=new_fields;x<holdaccn;++x,
			++temp,++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->atype=temp->atype;
			if(temp->accname!=NULL)
			{
				temp1->accname=stralloc(temp->accname);
				Rfree(temp->accname);
			} else temp1->accname=NULL;
		}
		Rfree(new_fields);
	} else {
		for(x=0,temp1=holdaccm;x<holdaccn;++x,++temp1)
		{
			if(temp1->name!=NULL) Rfree(temp1->name);
			if(temp1->accname!=NULL) Rfree(temp1->accname);
		}
		Rfree(holdaccm);
		holdaccn=0;
	}
	getaccums();
	if(value>=accavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(accrsrc,"ACCUMULATORS",value,accavl))
		updatersrc(accrsrc,"ACCUMULATORS");
	changedaccumulators=TRUE;
}
static void accedit(RDArsrc *accrsrc)
{
	int selected=0;
	RDAaccum *a;

	if(FINDRSCGETINT(accrsrc,"ACCUMULATORS",&selected)) return;
	a=holdaccm+selected;
	if(a!=NULL)
	{
		accumscreen(accrsrc,++selected,a->name,a->atype,a->accname,savemacc,quitmacctest);
	} else {
		accumscreen(accrsrc,++selected,NULL,0,NULL,savemacc,quitmacctest);
	}
}
static void quitacc(RDArsrc *accrsrc)
{
	int x=0;
	RDAaccum *a2;

	if(accrsrc!=NULL)
	{
		killwindow(accrsrc);
		free_rsrc(accrsrc);
	}
	if(accavl!=NULL) freeapplib(accavl);
	if(holdaccm!=NULL)
	{
		for(x=0,a2=holdaccm;x<holdaccn;++x,++a2)
		{
			if(a2->name!=NULL) Rfree(a2->name);
			if(a2->accname!=NULL) Rfree(a2->accname);
		}
		Rfree(holdaccm);
		holdaccn=0;
	}
	if(changedaccumulators) changedreport=TRUE;
	if(accdefaults!=NULL) FreeRDAdefault(accdefaults);
}
static void quitaccclr(RDArsrc *accrsrc)
{
	changedaccumulators=FALSE;
	quitacc(accrsrc);
}
static void saveacc(RDArsrc *accrsrc)
{
	int x=0;
	RDAaccum *a1,*a2;

	readallwidgets(accrsrc);
	if(report->accum!=NULL)
	{
		for(x=0,a2=report->accum;x<report->numaccums;++x,++a2)
		{
			if(a2->name!=NULL) Rfree(a2->name);
			if(a2->accname!=NULL) Rfree(a2->accname);
		}
		Rfree(report->accum);
		report->numaccums=0;
	}
	if(holdaccm!=NULL)
	{
		report->numaccums=holdaccn;
		report->accum=Rmalloc(holdaccn*sizeof(RDAaccum));
		for(x=0,a2=holdaccm,a1=report->accum;x<holdaccn;++x,++a2,++a1)
		{
			if(a2->name!=NULL)
			{
				a1->name=stralloc(a2->name);
				Rfree(a2->name);
			} else a1->name=NULL;
			a1->atype=a2->atype;
			if(a2->accname!=NULL)
			{
				a1->accname=stralloc(a2->accname);
				Rfree(a2->accname);
			} else a1->accname=NULL;
		}
		Rfree(holdaccm);
		holdaccn=0;
	}
	if(COMPARE_RSRCS(accrsrc,accdefaults,0) || changedaccumulators)
	{
		changedaccumulators=TRUE;
	}
	quitacc(accrsrc);
}
static void quitacctest(RDArsrc *accrsrc)
{
	readallwidgets(accrsrc);
	if(COMPARE_RSRCS(accrsrc,accdefaults,0) || changedaccumulators)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the ACCUMULATORS!\nDo you want to Save these changes?",saveacc,quitaccclr,FALSE,accrsrc);
	} else {
		quitacc(accrsrc);
	}
}
void RPTaccum(RDArsrc *rptrsrc)
{
	int x=0;
	RDAaccum *a1,*a2;
	RDArsrc *accrsrc=NULL;

	if(report->accum!=NULL)
	{
		holdaccm=Rmalloc(report->numaccums*sizeof(RDAaccum));
		holdaccn=report->numaccums;
		for(x=0,a1=holdaccm,a2=report->accum;x<holdaccn;++x,++a1,++a2)
		{
			if(a2->name!=NULL)
			{
				a1->name=stralloc(a2->name);
			} else a1->name=NULL;
			a1->atype=a2->atype;
			if(a2->accname!=NULL)
			{
				a1->accname=stralloc(a2->accname);
			} else a1->accname=NULL;
		}
	}
	accrsrc=RDArsrcNEW(module,"DEFINE ACCUMULATORS");
	getaccums();
	x=0;
	addlstrsrc(accrsrc,"ACCUMULATORS",&x,TRUE,NULL,accavl->numlibs,
		&accavl->libs,NULL);
	addbtnrsrc(accrsrc,"ADD ABOVE",TRUE,accaddabove,NULL);
	addbtnrsrc(accrsrc,"ADD BELOW",TRUE,accaddbelow,NULL);
	addbtnrsrc(accrsrc,"COPY ACCUMULATOR",TRUE,copyaccumulator,NULL);
	addbtnrsrc(accrsrc,"DELETE",TRUE,accdelete,NULL);
	addbtnrsrc(accrsrc,"SELECT",TRUE,accedit,NULL);
	addrfcbrsrc(accrsrc,"SAVE",TRUE,saveacc,NULL);
	addrfcbrsrc(accrsrc,"QUIT",TRUE,quitacctest,NULL);
	addbtnrsrc(accrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(accrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedaccumulators=FALSE;
	accdefaults=GetDefaults(accrsrc);
	APPmakescrn(accrsrc,TRUE,quitacc,NULL,FALSE);
}
