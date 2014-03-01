/* dfrptctl.c - Define Control Break Section of Define Reports */
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
extern char changedreport;
static APPlib *ctlavl=NULL;
static RDAcontrol *holdctl=NULL;
static int numctl=0;
static RDAdefault *ctldefaults=NULL,*mctldefaults=NULL;
static char changedcontrols=FALSE;

static void makespacecontrol(int pos)
{
	short x,y;
	RDAcontrol *temp,*temp1,*new_fields;

	++numctl;
	new_fields=Rmalloc(numctl*sizeof(RDAcontrol));
	y=0;
	for(x=0,temp=holdctl;x<(numctl-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		++y;
	}
	if(holdctl) { Rfree(holdctl); holdctl=NULL; }
	holdctl=Rmalloc(numctl*sizeof(RDAcontrol));
	for(x=0,temp1=holdctl,temp=new_fields;x<numctl;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
	}
	if(new_fields) { Rfree(new_fields); new_fields=NULL; }
}
static void quitmctl(RDArsrc *mctlrsrc,RDArsrc *ctlrsrc)
{
	if(mctlrsrc!=NULL)
	{
		killwindow(mctlrsrc);
		free_rsrc(mctlrsrc);
	}
	if(loadlist!=NULL) freeapplib(loadlist);
	if(mctldefaults!=NULL) FreeRDAdefault(mctldefaults);
}
static void getcontrols()
{
	int x,len=0;
	RDAcontrol *c;
	char *tmp=NULL;

	if(ctlavl!=NULL) freeapplib(ctlavl);
	ctlavl=APPlibNEW();
	for(x=0,c=holdctl;x<numctl;++x,++c)
	{
		len=RDAstrlen(c->name)+5+5;
		if(tmp!=NULL) 
		{
			tmp=Rrealloc(tmp,len);
		} else {
			tmp=Rmalloc(len);
		}
		memset(tmp,0,len);
		sprintf(tmp,"[%5d] %s",(x+1),(c->name!=NULL ? c->name:""));
		addAPPlib(ctlavl,tmp);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(ctlavl->numlibs<1)
	{
		addAPPlib(ctlavl,"No Control Breaks Defined");
	}
}
static void savemctl(RDArsrc *mctlrsrc,RDArsrc *ctlrsrc)
{
	int value=0;
	int xval=0;
	RDAcontrol *c;

	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&value)) return;
	if(holdctl==NULL) 
	{
		holdctl=Rmalloc(sizeof(RDAcontrol));
		numctl=1;
	}
	c=holdctl+value;
	readallwidgets(mctlrsrc);
	FINDRSCGETINT(mctlrsrc,"NAME",&xval);
	c->name=stralloc(loadlist->libs[xval]);
	getcontrols();
	if(!FINDRSCLISTAPPlib(ctlrsrc,"CONTROL BREAKS",value,ctlavl))
		updatersrc(ctlrsrc,"CONTROL BREAKS");
	readallwidgets(mctlrsrc);
	if(COMPARE_RSRCS(mctlrsrc,mctldefaults,2))
	{
		changedcontrols=TRUE;
	}
	quitmctl(mctlrsrc,ctlrsrc);
}
static void savemctlabove(RDArsrc *mctlrsrc,RDArsrc *ctlrsrc)
{
	int x;

	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&x)) return;
	makespacecontrol(x);
	savemctl(mctlrsrc,ctlrsrc);
}
static void savemctlbelow(RDArsrc *mctlrsrc,RDArsrc *ctlrsrc)
{
	int x;

	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&x)) return;
	makespacecontrol(++x);
	FINDRSCSETINT(ctlrsrc,"CONTROL BREAKS",x);
	savemctl(mctlrsrc,ctlrsrc);
}
static void controlscreen(RDArsrc *ctlrsrc,int num,char *name,
	void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	RDArsrc *mctlrsrc=NULL;
	int x;

	if(loadlist!=NULL) freeapplib(loadlist); 
	loadlist=APPlibNEW();
	makefieldvallistrpt(&loadlist,TRUE,TRUE,TRUE,FALSE);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"No Fields or Virtual Fields Available");
	}
	for(x=0;x<loadlist->numlibs;++x)
	{
		if(!RDAstrcmp(loadlist->libs[x],name)) break;
	}
	if(x>=loadlist->numlibs)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=loadlist->numlibs) x=0;
	}
	mctlrsrc=RDArsrcNEW(module,"MAINTAIN CONTROL BREAKS");
	addstdrsrc(mctlrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addlstrsrc(mctlrsrc,"NAME",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfcbrsrc(mctlrsrc,"SAVE",TRUE,savefunction,ctlrsrc);
	addrfcbrsrc(mctlrsrc,"QUIT",TRUE,quitfunctiontest,ctlrsrc);
	addbtnrsrc(mctlrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mctlrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mctldefaults=GetDefaults(mctlrsrc);
	APPmakescrn(mctlrsrc,TRUE,quitmctl,ctlrsrc,FALSE);
}
static void quitmctltest(RDArsrc *mctlrsrc,RDArsrc *ctlrsrc)
{
	readallwidgets(mctlrsrc);
	if(COMPARE_RSRCS(mctlrsrc,mctldefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this CONTROL BREAK!\nDo you want to Save these changes?",savemctl,quitmctl,FALSE,2,mctlrsrc,ctlrsrc,NULL);
	} else {
		quitmctl(mctlrsrc,ctlrsrc);
	}
}
static void quitmctlabovetest(RDArsrc *mctlrsrc,RDArsrc *ctlrsrc)
{
	readallwidgets(mctlrsrc);
	if(COMPARE_RSRCS(mctlrsrc,mctldefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this CONTROL BREAK!\nDo you want to Save these changes?",savemctlabove,quitmctl,FALSE,2,mctlrsrc,ctlrsrc,NULL);
	} else {
		quitmctl(mctlrsrc,ctlrsrc);
	}
}
static void quitmctlbelowtest(RDArsrc *mctlrsrc,RDArsrc *ctlrsrc)
{
	readallwidgets(mctlrsrc);
	if(COMPARE_RSRCS(mctlrsrc,mctldefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this CONTROL BREAK!\nDo you want to Save these changes?",savemctlbelow,quitmctl,FALSE,2,mctlrsrc,ctlrsrc,NULL);
	} else {
		quitmctl(mctlrsrc,ctlrsrc);
	}
}
static void ctladdabove(RDArsrc *ctlrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&selected)) return;
	if(ctlavl->numlibs==1 && !RDAstrcmp(ctlavl->libs[0],"No Control Breaks Defined")) return;
	controlscreen(ctlrsrc,++selected,NULL,savemctlabove,quitmctlabovetest);
}
static void ctladdbelow(RDArsrc *ctlrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&selected)) return;
	if(ctlavl->numlibs==1 && !RDAstrcmp(ctlavl->libs[0],"No Control Breaks Defined")) return;
	selected+=2;
	controlscreen(ctlrsrc,selected,NULL,savemctlbelow,quitmctlbelowtest);
}
static void copyctl(RDArsrc *ctlrsrc)
{
	int selected=0;
	RDAcontrol *c;

	if(ctlavl->numlibs==1 && !RDAstrcmp(ctlavl->libs[0],"No Control Breaks Defined")) return;
	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&selected)) return;
	c=holdctl+selected;
	selected+=2;
	if(c!=NULL)
	{
		controlscreen(ctlrsrc,++selected,c->name,savemctlbelow,quitmctlbelowtest);
	} else {
		controlscreen(ctlrsrc,++selected,NULL,savemctlbelow,quitmctlbelowtest);
	}
}
static void ctldelete(RDArsrc *ctlrsrc)
{
	short x,y;
	RDAcontrol *temp,*temp1,*new_fields=NULL;
	int value;

	readallwidgets(ctlrsrc);
	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&value)) return;
	if(numctl<1) return;
	if(numctl>1)
	{
		new_fields=Rmalloc((numctl-1)*sizeof(RDAcontrol));
		y=0;
		for(x=0,temp=holdctl;x<numctl;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
			}
		}
		if(holdctl!=NULL) { Rfree(holdctl); holdctl=NULL; }
		--numctl;
		holdctl=Rmalloc(numctl*sizeof(RDAcontrol));
		for(x=0,temp1=holdctl,temp=new_fields;x<numctl;++x,
			++temp,++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
		}
	} else if(holdctl!=NULL)
	{
		for(x=0,temp1=holdctl;x<numctl;++x,++temp1)
		{
			if(temp1->name!=NULL) Rfree(temp1->name);
		}
		Rfree(holdctl);
		numctl=0;
	}
	if(new_fields!=NULL) Rfree(new_fields);
	getcontrols();
	if(value>=ctlavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(ctlrsrc,"CONTROL BREAKS",value,ctlavl))
		updatersrc(ctlrsrc,"CONTROL BREAKS");
	changedcontrols=TRUE;
}
static void ctledit(RDArsrc *ctlrsrc)
{
	int selected=0;
	RDAcontrol *c;

	if(FINDRSCGETINT(ctlrsrc,"CONTROL BREAKS",&selected)) return;
	c=holdctl+selected;
	if(c!=NULL)
	{
		controlscreen(ctlrsrc,++selected,c->name,savemctl,quitmctltest);
	} else {
		controlscreen(ctlrsrc,++selected,NULL,savemctl,quitmctltest);
	}
}
static void gatherctl(RDArsrc *ctlrsrc)
{
	int x=0,value=0;
	RDAcontrol *c=NULL;
	RDAsort *s=NULL;
	
	if(holdctl!=NULL)
	{
		for(x=0,c=holdctl;x<numctl;++x,++c)
		{
			if(c->name!=NULL) Rfree(c->name);
		}
		Rfree(holdctl);
		numctl=0;
	}
	if(report->sort!=NULL)
	{
		numctl=report->numsorts;
		holdctl=Rmalloc(numctl*sizeof(RDAcontrol));
		for(x=0,s=report->sort,c=holdctl;x<report->numsorts;++x,++s,++c)
		{
			c->name=stralloc(s->name);
		}
	} else {
		holdctl=NULL;
		numctl=0;
	}
	getcontrols();
	if(!FINDRSCLISTAPPlib(ctlrsrc,"CONTROL BREAKS",value,ctlavl))
		updatersrc(ctlrsrc,"CONTROL BREAKS");
}
static void quitctl(RDArsrc *ctlrsrc)
{
	int x=0;
	RDAcontrol *c2;

	if(holdctl!=NULL)
	{
		for(x=0,c2=holdctl;x<numctl;++x,++c2)
		{
			if(c2->name!=NULL) Rfree(c2->name);
		}
		Rfree(holdctl);
		numctl=0;
	}
	if(ctlrsrc!=NULL)
	{
		killwindow(ctlrsrc);
		free_rsrc(ctlrsrc);
	}
	if(ctlavl!=NULL) freeapplib(ctlavl);
	if(changedcontrols) changedreport=TRUE;
	if(ctldefaults!=NULL) FreeRDAdefault(ctldefaults);
}
static void quitctlclr(RDArsrc *ctlrsrc)
{
	changedcontrols=FALSE;
	quitctl(ctlrsrc);
}
static void savectl(RDArsrc *ctlrsrc)
{
	int x=0;
	RDAcontrol *c1,*c2;

	readallwidgets(ctlrsrc);
	if(report->control!=NULL)
	{
		for(x=0,c1=report->control;x<report->numcontrols;++x,++c1)
		{
			if(c1->name!=NULL) Rfree(c1->name);
		}
		Rfree(report->control);
		report->control=NULL;
		report->numcontrols=0;
	}
	if(holdctl!=NULL)
	{
		report->numcontrols=numctl;
		report->control=Rmalloc(numctl*sizeof(RDAcontrol));
		for(x=0,c1=report->control,c2=holdctl;x<numctl;++x,++c1,++c2)
		{
			if(c2->name!=NULL)
			{
				c1->name=stralloc(c2->name);
				Rfree(c2->name);
			} else c1->name=NULL;
		}
		Rfree(holdctl);
		numctl=0;
	}
	if(COMPARE_RSRCS(ctlrsrc,ctldefaults,0) || changedcontrols)
	{
		changedcontrols=TRUE;
	}
	quitctl(ctlrsrc);
}
static void quitctltest(RDArsrc *ctlrsrc)
{
	readallwidgets(ctlrsrc);
	if(COMPARE_RSRCS(ctlrsrc,ctldefaults,0) || changedcontrols)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the CONTROL BREAKS!\nDo you want to Save these changes?",savectl,quitctlclr,FALSE,ctlrsrc);
	} else {
		quitctl(ctlrsrc);
	}
}
void RPTcontrol(RDArsrc *rptrsrc)
{
	int x=0;
	RDAcontrol *c1,*c2;
	RDArsrc *ctlrsrc=NULL;

	if(report->control!=NULL)
	{
		numctl=report->numcontrols;
		holdctl=Rmalloc(report->numcontrols*sizeof(RDAcontrol));
		for(x=0,c1=report->control,c2=holdctl;x<numctl;++x,++c1,++c2)
		{
			if(c1->name!=NULL)
			{
				c2->name=stralloc(c1->name);
			} else c2->name=NULL;
		}
	}
	ctlrsrc=RDArsrcNEW(module,"DEFINE CONTROL BREAKS");
	getcontrols();
	x=0;
	addlstrsrc(ctlrsrc,"CONTROL BREAKS",&x,TRUE,NULL,ctlavl->numlibs,&ctlavl->libs,NULL);
	addbtnrsrc(ctlrsrc,"ADD ABOVE",TRUE,ctladdabove,NULL);
	addbtnrsrc(ctlrsrc,"ADD BELOW",TRUE,ctladdbelow,NULL);
	addbtnrsrc(ctlrsrc,"DELETE",TRUE,ctldelete,NULL);
	addbtnrsrc(ctlrsrc,"SELECT",TRUE,ctledit,NULL);
	addbtnrsrc(ctlrsrc,"COPY",TRUE,copyctl,NULL);
	addbtnrsrc(ctlrsrc,"GATHER SORT",TRUE,gatherctl,NULL);
	addrfcbrsrc(ctlrsrc,"SAVE",TRUE,savectl,NULL);
	addrfcbrsrc(ctlrsrc,"QUIT",TRUE,quitctltest,NULL);
	addbtnrsrc(ctlrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ctlrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedcontrols=FALSE;
	ctldefaults=GetDefaults(ctlrsrc);
	APPmakescrn(ctlrsrc,TRUE,quitctl,NULL,FALSE);
}
