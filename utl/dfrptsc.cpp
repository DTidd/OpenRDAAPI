/* dfrptsrt.c - Define Sort Sequence section of Define Reports */
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
extern char *sequences[];
extern char changedreport;
static RDAsort *holdsort;
static int holdsnum;
static APPlib *srtavl=NULL,*stypes=NULL;
static RDAdefault *srtdefaults=NULL,*msrtdefaults=NULL;
static char changedsorts=FALSE;

static void makespacesort(int pos)
{
	short x,y;
	RDAsort *temp,*temp1,*new_fields;

	++holdsnum;
	new_fields=Rmalloc(holdsnum*sizeof(RDAsort));
	y=0;
	for(x=0,temp=holdsort;x<(holdsnum-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->type=0;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		++y;
	}
	Rfree(holdsort);
	holdsort=Rmalloc(holdsnum*sizeof(RDAsort));
	for(x=0,temp1=holdsort,temp=new_fields;x<holdsnum;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
	}
	Rfree(new_fields);
}
static void quitmsrt(RDArsrc *msrtrsrc,RDArsrc *srtrsrc)
{
	if(msrtrsrc!=NULL)
	{ 
		killwindow(msrtrsrc);
		free_rsrc(msrtrsrc);
	}
	if(stypes!=NULL) freeapplib(stypes);
	if(loadlist!=NULL) freeapplib(loadlist);
	if(msrtdefaults!=NULL) FreeRDAdefault(msrtdefaults);
}
static void getsorts()
{
	int x,len=0;
	RDAsort *s;
	char *tmp=NULL;

	if(srtavl!=NULL) freeapplib(srtavl);
	srtavl=APPlibNEW();
	for(x=0,s=holdsort;x<holdsnum;++x,++s)
	{
		len=RDAstrlen(s->name)+1;
		if(tmp!=NULL) 
		{
			tmp=Rrealloc(tmp,len);
		} else {
			tmp=Rmalloc(len);
		}
		memset(tmp,0,len);
		sprintf(tmp,"%s",s->name);
		addAPPlib(srtavl,tmp);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(srtavl->numlibs<1)
	{
		addAPPlib(srtavl,"No Sorts Defined");
	}
}
static void savemsrt(RDArsrc *msrtrsrc,RDArsrc *srtrsrc)
{
	int value=0,xvals=0;
	RDAsort *s;

	if(FINDRSCGETINT(srtrsrc,"SORT BY",&value)) return;
	if(holdsort==NULL) 
	{
		holdsort=Rmalloc(sizeof(RDAsort));
		holdsnum=1;
	}
	s=holdsort+value;
	readallwidgets(msrtrsrc);
	FINDRSCGETINT(msrtrsrc,"NAME",&xvals);
	s->name=stralloc(loadlist->libs[xvals]);
	FINDRSCGETINT(msrtrsrc,"SORT TYPES",&s->type);
	getsorts();
	if(!FINDRSCLISTAPPlib(srtrsrc,"SORT BY",value,srtavl))
		updatersrc(srtrsrc,"SORT BY");
	if(COMPARE_RSRCS(msrtrsrc,msrtdefaults,2))
	{
		changedsorts=TRUE;
	}
	quitmsrt(msrtrsrc,srtrsrc);
}
static void savemsrtabove(RDArsrc *msrtrsrc,RDArsrc *srtrsrc)
{
	int x;

	if(FINDRSCGETINT(srtrsrc,"SORT BY",&x)) return;
	makespacesort(x);
	savemsrt(msrtrsrc,srtrsrc);
}
static void savemsrtbelow(RDArsrc *msrtrsrc,RDArsrc *srtrsrc)
{
	int x;

	if(FINDRSCGETINT(srtrsrc,"SORT BY",&x)) return;
	makespacesort(++x);
	FINDRSCSETINT(srtrsrc,"SORT BY",x);
	savemsrt(msrtrsrc,srtrsrc);
}
static void sortscreen(RDArsrc *srtrsrc,int num,char *name,int type,
	void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	int x;
	RDArsrc *msrtrsrc=NULL;

	if(stypes!=NULL) freeapplib(stypes);
	stypes=APPlibNEW();
	for(x=0;x<6;++x) addAPPlib(stypes,sequences[x]);
	msrtrsrc=RDArsrcNEW(module,"MAINTAIN SORT BY");
	addstdrsrc(msrtrsrc,"NUMBER",LONGV,8,&num,FALSE);
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
	if(x>=loadlist->numlibs) x=0;
	addlstrsrc(msrtrsrc,"NAME",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addlstrsrc(msrtrsrc,"SORT TYPES",&type,TRUE,NULL,stypes->numlibs,
		&stypes->libs,NULL);
	addrfcbrsrc(msrtrsrc,"SAVE",TRUE,savefunction,srtrsrc);
	addrfcbrsrc(msrtrsrc,"QUIT",TRUE,quitfunctiontest,srtrsrc);
	addbtnrsrc(msrtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(msrtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	msrtdefaults=GetDefaults(msrtrsrc);
	APPmakescrn(msrtrsrc,TRUE,quitmsrt,srtrsrc,FALSE);
}
static void quitmsrttest(RDArsrc *msrtrsrc,RDArsrc *srtrsrc)
{
	readallwidgets(msrtrsrc);
	if(COMPARE_RSRCS(msrtrsrc,msrtdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this SORT FIELD!\nDo you want to Save these changes?",savemsrt,quitmsrt,FALSE,2,msrtrsrc,srtrsrc,NULL);
	} else {
		quitmsrt(msrtrsrc,srtrsrc);
	}
}
static void quitmsrtabovetest(RDArsrc *msrtrsrc,RDArsrc *srtrsrc)
{
	readallwidgets(msrtrsrc);
	if(COMPARE_RSRCS(msrtrsrc,msrtdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this SORT FIELD!\nDo you want to Save these changes?",savemsrtabove,quitmsrt,FALSE,2,msrtrsrc,srtrsrc,NULL);
	} else {
		quitmsrt(msrtrsrc,srtrsrc);
	}
}
static void quitmsrtbelowtest(RDArsrc *msrtrsrc,RDArsrc *srtrsrc)
{
	readallwidgets(msrtrsrc);
	if(COMPARE_RSRCS(msrtrsrc,msrtdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this SORT FIELD!\nDo you want to Save these changes?",savemsrtbelow,quitmsrt,FALSE,2,msrtrsrc,srtrsrc,NULL);
	} else {
		quitmsrt(msrtrsrc,srtrsrc);
	}
}
static void srtaddabove(RDArsrc *srtrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(srtrsrc,"SORT BY",&selected)) return;
	if(srtavl->numlibs==1 && !RDAstrcmp(srtavl->libs[0],"No Sorts Defined")) return;
	sortscreen(srtrsrc,++selected,NULL,0,savemsrtabove,quitmsrtabovetest);
}
static void srtaddbelow(RDArsrc *srtrsrc)
{
	int selected=0;

	if(srtavl->numlibs==1 && !RDAstrcmp(srtavl->libs[0],"No Sorts Defined")) return;
	if(FINDRSCGETINT(srtrsrc,"SORT BY",&selected)) return;
	selected+=2;
	sortscreen(srtrsrc,selected,NULL,0,savemsrtbelow,quitmsrtbelowtest);
}
static void srtdelete(RDArsrc *srtrsrc)
{
	short x,y;
	RDAsort *temp,*temp1,*new_fields;
	int value;

	readallwidgets(srtrsrc);
	if(FINDRSCGETINT(srtrsrc,"SORT BY",&value)) return;
	if(holdsnum<1) return;
	if(holdsnum>1)
	{
		new_fields=Rmalloc((holdsnum-1)*sizeof(RDAsort));
		y=0;
		for(x=0,temp=holdsort;x<holdsnum;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				temp1->type=temp->type;
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
			}
		}
		Rfree(holdsort);
		--holdsnum;
		holdsort=Rmalloc(holdsnum*sizeof(RDAsort));
		for(x=0,temp1=holdsort,temp=new_fields;x<holdsnum;++x,
			++temp,++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->type=temp->type;
		}
		Rfree(new_fields);
	} else if(holdsort!=NULL)
	{
		for(x=0,temp1=holdsort;x<holdsnum;++x,++temp1)
		{
			if(temp1->name!=NULL) Rfree(temp1->name);
		}
		Rfree(holdsort);
		holdsnum=0;
	}
	getsorts();
	if(value>=srtavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(srtrsrc,"SORT BY",value,srtavl))
	updatersrc(srtrsrc,"SORT BY");
	changedsorts=TRUE;
}
static void srtedit(RDArsrc *srtrsrc)
{
	int selected=0;
	RDAsort *s;

	if(FINDRSCGETINT(srtrsrc,"SORT BY",&selected)) return;
	s=holdsort+selected;
	if(s!=NULL)
	{
		sortscreen(srtrsrc,++selected,s->name,s->type,savemsrt,quitmsrttest);
	} else {
		sortscreen(srtrsrc,++selected,NULL,0,savemsrt,quitmsrttest);
	}
}
static void quitsrt(RDArsrc *srtrsrc)
{
	RDAsort *s1;
	int x=0;

	if(srtavl!=NULL) freeapplib(srtavl);
	if(srtrsrc!=NULL)
	{
		killwindow(srtrsrc);
		free_rsrc(srtrsrc);
	}
	if(holdsort!=NULL)
	{
		for(x=0,s1=holdsort;x<holdsnum;++x,++s1)
		{
			if(s1->name!=NULL) Rfree(s1->name);
		}
		Rfree(holdsort);
		holdsnum=0;
	}
	if(changedsorts) changedreport=TRUE;
	if(srtdefaults!=NULL) FreeRDAdefault(srtdefaults);
}
static void quitsrtclr(RDArsrc *srtrsrc)
{
	changedsorts=FALSE;
	quitsrt(srtrsrc);
}
static void savesrt(RDArsrc *srtrsrc)
{
	int x=0;
	RDAsort *s,*s1;

	if(report->sort!=NULL)
	{
		for(x=0,s=report->sort;x<report->numsorts;++x,++s)
		{
			if(s->name!=NULL) Rfree(s->name);
		}
		Rfree(report->sort);
		report->numsorts=0;
	}
	if(holdsort!=NULL)
	{
		report->numsorts=holdsnum;
		report->sort=Rmalloc(holdsnum*sizeof(RDAsort));
		for(x=0,s=report->sort,s1=holdsort;x<holdsnum;++x,++s,++s1)
		{
			if(s1->name!=NULL) 
			{
				s->name=stralloc(s1->name);
				Rfree(s1->name);
			} else s->name=NULL;
			s->type=s1->type;
		}
		Rfree(holdsort);
		holdsnum=0;
	}
	if(COMPARE_RSRCS(srtrsrc,srtdefaults,0) || changedsorts)
	{
		changedsorts=TRUE;
	}
	quitsrt(srtrsrc);
}
static void quitsrttest(RDArsrc *srtrsrc)
{
	readallwidgets(srtrsrc);
	if(COMPARE_RSRCS(srtrsrc,srtdefaults,0) || changedsorts)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the SORT DEFINITIONS!\nDo you want to Save these changes?",savesrt,quitsrtclr,FALSE,srtrsrc);
	} else {
		quitsrt(srtrsrc);
	}
}
void sortdef(RDArsrc *rptrsrc)
{
	int x=0;
	RDAsort *s,*s1;
	RDArsrc *srtrsrc=NULL;

	holdsnum=report->numsorts;
	if(report->sort!=NULL)
	{
		holdsort=Rmalloc(holdsnum*sizeof(RDAsort));
		for(x=0,s=holdsort,s1=report->sort;x<holdsnum;++x,++s,++s1)
		{
			if(s1->name!=NULL)
			{
				s->name=stralloc(s1->name);
			} else s->name=NULL;
			s->type=s1->type;
		}
	} else {
		holdsort=NULL;
		holdsnum=0;
	}
	srtrsrc=RDArsrcNEW(module,"DEFINE SORT");
	getsorts();
	x=0;
	addlstrsrc(srtrsrc,"SORT BY",&x,TRUE,NULL,srtavl->numlibs,
		&srtavl->libs,NULL);
	addbtnrsrc(srtrsrc,"ADD ABOVE",TRUE,srtaddabove,NULL);
	addbtnrsrc(srtrsrc,"ADD BELOW",TRUE,srtaddbelow,NULL);
	addbtnrsrc(srtrsrc,"DELETE",TRUE,srtdelete,NULL);
	addbtnrsrc(srtrsrc,"SELECT",TRUE,srtedit,NULL);
	addrfcbrsrc(srtrsrc,"SAVE",TRUE,savesrt,NULL);
	addrfcbrsrc(srtrsrc,"QUIT",TRUE,quitsrttest,NULL);
	addbtnrsrc(srtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(srtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedsorts=FALSE;
	srtdefaults=GetDefaults(srtrsrc);
	APPmakescrn(srtrsrc,TRUE,quitsrt,NULL,FALSE);
}
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
		len=RDAstrlen(c->name)+3;
		if(tmp!=NULL) 
		{
			tmp=Rrealloc(tmp,len);
		} else {
			tmp=Rmalloc(len);
		}
		memset(tmp,0,len);
		sprintf(tmp,"[%s]",(c->name!=NULL ? c->name:""));
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
	if(x>=loadlist->numlibs) x=0;
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
	addrfcbrsrc(ctlrsrc,"SAVE",TRUE,savectl,NULL);
	addrfcbrsrc(ctlrsrc,"QUIT",TRUE,quitctltest,NULL);
	addbtnrsrc(ctlrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ctlrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedcontrols=FALSE;
	ctldefaults=GetDefaults(ctlrsrc);
	APPmakescrn(ctlrsrc,TRUE,quitctl,NULL,FALSE);
}
