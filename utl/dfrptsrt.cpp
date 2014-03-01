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
		len=RDAstrlen(s->name)+5+5;
		if(tmp!=NULL) 
		{
			tmp=Rrealloc(tmp,len);
		} else {
			tmp=Rmalloc(len);
		}
		memset(tmp,0,len);
		sprintf(tmp,"[%5d] %s",(x+1),(s->name!=NULL?s->name:""));
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
	char dev_license=FALSE,*tmp=NULL,*temp1=NULL,*devlicense=NULL;

	tmp=RDA_GetEnv("DEV_LICENSE");
	if(tmp!=NULL)
	{
		temp1=strtok(tmp," ");
		devlicense=stralloc(temp1);
		if(CheckDevLicense(devlicense))
		{
			dev_license=TRUE;
		} else {
			dev_license=FALSE;
		}
	} else {
			dev_license=FALSE;
	}
	if(devlicense!=NULL) Rfree(devlicense);
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
	if(x>=loadlist->numlibs)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=loadlist->numlibs) x=0;
	}
	addlstrsrc(msrtrsrc,"NAME",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addlstrsrc(msrtrsrc,"SORT TYPES",&type,dev_license,NULL,stypes->numlibs,
		&stypes->libs,NULL);
	FINDRSCSETSENSITIVE(msrtrsrc,"SORT TYPES",dev_license);
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
static void copysrt(RDArsrc *srtrsrc)
{
	int selected=0;
	RDAsort *s;

	if(srtavl->numlibs==1 && !RDAstrcmp(srtavl->libs[0],"No Sorts Defined")) return;
	if(FINDRSCGETINT(srtrsrc,"SORT BY",&selected)) return;
	s=holdsort+selected;
	selected+=2;
	if(s!=NULL)
	{
		sortscreen(srtrsrc,++selected,s->name,s->type,savemsrtbelow,quitmsrtbelowtest);
	} else {
		sortscreen(srtrsrc,++selected,NULL,0,savemsrtbelow,quitmsrtbelowtest);
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
void RPTsortdef(RDArsrc *rptrsrc)
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
	addbtnrsrc(srtrsrc,"COPY",TRUE,copysrt,NULL);
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
