/* dfhelp.c - Define Help */
#ifdef WIN32
#include<io.h>
#endif
#ifndef WIN32
#define __NAM__ "dfhelp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfhelp.exe"
#endif
#include <app.hpp>



#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
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
struct RDASCRNrhlps
{
	char *name;
	char *desc;
	char *label;
	char onscreen;
	char available;
};
typedef struct RDASCRNrhlps RDASCRNrhlp;
struct RDASCRNhelps 
{
	char *name;
	char *desc;
	int numrschs;
	RDASCRNrhlp *rhlp;
	RDAscrn *screen;
	HoldRDArsrc *hold;
};
typedef struct RDASCRNhelps RDASCRNhelp;

static RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL,*editrsrc=NULL;
static char *module="OLHELP";
static APPlib *hlp_bins=NULL,*hlp_defs=NULL,*dir_libs=NULL;
static APPlib *help=NULL,*RSRClist=NULL;
static RDASCRNhelp *hlpdata=NULL;
static void makespacehelp(int),deletedfhelp(RDArsrc *);
static void quitdfhelp(RDArsrc *),okdfhelp(RDArsrc *,RDArsrc *);
static void makemtnscreen(RDArsrc *,char *,char *,char *);
static void newdfhelp(RDArsrc *,RDArsrc *),makehelplist(void);
static void gethelpentries(RDArsrc *),getdirectoryhelp(RDArsrc *);
static void savehelp(RDArsrc *),quithelp(RDArsrc *);
static void edit_resource(RDArsrc *),delete_rsc(RDArsrc *);
static void add_rsc_above(RDArsrc *),add_rsc_below(RDArsrc *);
static void quitresource(RDArsrc *),quitresourcetest(RDArsrc *);
static void quitresourceabovetest(RDArsrc *),quitresourcebelowtest(RDArsrc *);
static void saveresource(RDArsrc *),save_resource_below(RDArsrc *);
static void save_resource_above(RDArsrc *);
static void helpscreen(RDArsrc *,char *,char *,char *,char,char,void (*)(...),void (*)(...));
static void deletetest(RDArsrc *);
static RDAdefault *mtndefaults=NULL,*editdefaults=NULL; 
static char changedvalues=FALSE;
static void changeRSRClist(RDArsrc *);
static short FINDHoldRDArsrc(HoldRDArsrc *,char *);

#define RDASCRNhelpNEW()	xRDASCRNhelpNEW(__LINE__,__FILE__)

void schfldcb(RDArsrc *mainrsrc)
{
	char *valuex=NULL;
	int x,current_value=0;

	readwidget(mainrsrc,"SEARCH FIELD");
	FINDRSCGETSTRING(mainrsrc,"SEARCH FIELD",&valuex);
	FINDRSCGETINT(mainrsrc,"RESOURCE LIST",&current_value);
	if(!isEMPTY(valuex))
	{
/* if you are at the end, start looking at the top */
		if((current_value+1)>=help->numlibs) current_value=(-1);
/* starting from current location, find first RESOURCE LIST element, containing the target string */
		for(x=(current_value+1);x<help->numlibs;++x)
		{
			if(RDAstrstr(help->libs[x],valuex)!=NULL)
			{
/* target found in RESOURCE LIST element x, updating RESOURCE LIST accordingly  */
				if(!FINDRSCSETINT(mainrsrc,"RESOURCE LIST",x))
				{
					updatersrc(mainrsrc,"RESOURCE LIST");	
					Rfree(valuex);
					return;
				}
			} 
		}
/* target not found, setting RESOURCE LIST to last element in list */
		x=help->numlibs-1;
		if(!FINDRSCSETINT(mainrsrc,"RESOURCE LIST",x))
			updatersrc(mainrsrc,"RESOURCE LIST");
		Rfree(valuex);
	} else {
/* when target is NULL, set RESOURCE LIST to the 1st element */
		x=0;
		if(!FINDRSCSETINT(mainrsrc,"RESOURCE LIST",x))
			updatersrc(mainrsrc,"RESOURCE LIST");
	}
}
RDASCRNhelp *xRDASCRNhelpNEW(int line,char *file)
{
	RDASCRNhelp *tmp=NULL;

	if(diagolh)
	{
		prterr("DIAG RDAhelpNEW Creating new RDAhelp structure at line [%d] program [%s].",line,file);
	}
	tmp=Rmalloc(sizeof(RDASCRNhelp));
	tmp->name=NULL;
	tmp->desc=NULL;
	tmp->rhlp=NULL;
	tmp->numrschs=0;
	tmp->screen=NULL;
	tmp->hold=NULL;
	return(tmp);
}
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
#define FreeHoldRDArsrc(a)	{xFreeHoldRDArsrc(a,__LINE__,__FILE__);a=NULL;}
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
#define FreeSCRNhelp(a)	{xFreeSCRNhelp(a,__LINE__,__FILE__);a=NULL;}
void xFreeSCRNhelp(RDASCRNhelp *h,int line,char *file)
{
	int x;
	RDASCRNrhlp *r;

	if(diagolh)
	{
		prterr("DIAG free_help Freeing RDAhelp structure for Screen [%s] at line [%d] program [%s].",h->name,line,file);
	}
	if(h!=NULL)
	{
		if(h->name!=NULL) Rfree(h->name);
		if(h->desc!=NULL) Rfree(h->desc);
		if(h->rhlp!=NULL)
		{
			for(x=0,r=h->rhlp;x<h->numrschs;++x,++r)
			{
				if(r->name!=NULL) Rfree(r->name);
				if(r->label!=NULL) Rfree(r->label);
				if(r->desc!=NULL) Rfree(r->desc);
			}
			Rfree(h->rhlp);
		}
		if(h->screen!=NULL) free_scrn(h->screen);
		if(h->hold!=NULL) FreeHoldRDArsrc(h->hold);
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
		prterr("Error in permissions, user cannot read the resource binary library [%s].",fname);
		return(NULL);
	}
	mainbin=getlibbin(fname,name,TRUE);
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
	if(total>0)
	{
		tmp->rscs=Rmalloc(total*sizeof(HoldRDArmem));
		tmp->num=total;
	}
	for(x=0;x<total;++x)
	{
		mem=tmp->rscs+x;
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
	}
	if(mainbin!=NULL) BINfree(mainbin);
	if(fname!=NULL) Rfree(fname);
	return(tmp);
}
static void changersrcname(RDArsrc *edit_rsrc)
{
	char *temp=NULL,a=FALSE,*label=NULL;
	int x=0,y=0;
	RDAwdgt *w;
	RDASCRNrhlp *f;

	readwidget(edit_rsrc,"RESOURCE NAME");
	FINDRSCGETSTRING(edit_rsrc,"RESOURCE NAME",&temp);
	if(!isEMPTY(temp))
	{
		for(x=0;x<RSRClist->numlibs;++x)
		{
			if(!RDAstrcmp(RSRClist->libs[x],temp)) break;
		}
		if(x<RSRClist->numlibs)
		{
			FINDRSCSETINT(edit_rsrc,"RESOURCES AVAILABLE",x);
		} else {
			x=RSRClist->numlibs;
			addAPPlib(RSRClist,temp);
			FINDRSCLISTAPPlib(edit_rsrc,"RESOURCES AVAILABLE",x,RSRClist);
		}
		for(y=0,f=hlpdata->rhlp;y<hlpdata->numrschs;++y,++f)
		{
			if(!RDAstrcmp(f->name,RSRClist->libs[x])) break;
		}
		if(y<hlpdata->numrschs)
		{
			FINDRSCSETSTRING(edit_rsrc,"RESOURCE LABEL",f->label);
			FINDRSCSETCHAR(edit_rsrc,"ON SCREEN",f->onscreen);
			FINDRSCSETCHAR(edit_rsrc,"AVAILABLE",f->available);
			if(f->desc!=NULL)
			{
				FINDRSCSETSTRING(edit_rsrc,"RESOURCE DESCRIPTION",f->desc);
				updatersrc(edit_rsrc,"RESOURCE DESCRIPTION");
			}
		} else {
			if(((y=FINDWDGT(hlpdata->screen,RSRClist->libs[x]))!=(-1)))
			{
				w=hlpdata->screen->wdgts+y;
				if(!isEMPTY(w->label)) label=stralloc(w->label);
				else label=NULL;
				a=TRUE;
			} else {
				label=NULL;
				a=FALSE;
			}
			FINDRSCSETSTRING(edit_rsrc,"RESOURCE LABEL",label);
			if(label!=NULL) Rfree(label);
			FINDRSCSETCHAR(edit_rsrc,"ON SCREEN",a);
			if(FINDHoldRDArsrc(hlpdata->hold,RSRClist->libs[x])!=(-1)) a=TRUE;
				else a=FALSE;
			FINDRSCSETCHAR(edit_rsrc,"AVAILABLE",a);
		}
		updatersrc(edit_rsrc,"RESOURCE LABEL");
		updatersrc(edit_rsrc,"ON SCREEN");
		updatersrc(edit_rsrc,"AVAILABLE");
	} else {
		FINDRSCLISTAPPlib(edit_rsrc,"RESOURCES AVAILABLE",x,RSRClist);
		FINDRSCGETINT(edit_rsrc,"RESOURCES AVAILABLE",&x);
		if(RDAstrcmp(RSRClist->libs[x],"Not Available"))
		{
			FINDRSCSETSTRING(edit_rsrc,"RESOURCE NAME",RSRClist->libs[x]);
			updatersrc(edit_rsrc,"RESOURCE NAME");
		}
	}
	if(temp!=NULL) Rfree(temp);
	updatersrc(edit_rsrc,"RESOURCES AVAILABLE");
}
static short FINDHoldRDArsrc(HoldRDArsrc *h,char *name)
{
	HoldRDArmem *m;
	int x;

	if(h!=NULL)
	{
		for(x=0,m=h->rscs;x<h->num;++x,++m)
		{
			if(!RDAstrcmp(name,m->name)) return(x);
		}
	}
	return(-1);
}
static void changersrclist(RDArsrc *edit_rsrc)
{
	int x=0,y=0;
	char a=FALSE,*label=NULL;
	RDAwdgt *w=NULL;
	RDASCRNrhlp *f;

	FINDRSCGETINT(edit_rsrc,"RESOURCES AVAILABLE",&x);
	if(RDAstrcmp(RSRClist->libs[x],"Not Available"))
	{
		FINDRSCSETSTRING(edit_rsrc,"RESOURCE NAME",RSRClist->libs[x]);
		for(y=0,f=hlpdata->rhlp;y<hlpdata->numrschs;++y,++f)
		{
			if(!RDAstrcmp(f->name,RSRClist->libs[x])) break;
		}
		if(y<hlpdata->numrschs)
		{
			FINDRSCSETSTRING(edit_rsrc,"RESOURCE LABEL",f->label);
			FINDRSCSETCHAR(edit_rsrc,"ON SCREEN",f->onscreen);
			FINDRSCSETCHAR(edit_rsrc,"AVAILABLE",f->available);
			if(f->desc!=NULL)
			{
				FINDRSCSETSTRING(edit_rsrc,"RESOURCE DESCRIPTION",f->desc);
				updatersrc(edit_rsrc,"RESOURCE DESCRIPTION");
			}
		} else {
			if(((y=FINDWDGT(hlpdata->screen,RSRClist->libs[x]))!=(-1)))
			{
				w=hlpdata->screen->wdgts+y;
				if(!isEMPTY(w->label)) label=stralloc(w->label);
				else label=NULL;
				a=TRUE;
			} else {
				label=NULL;
				a=FALSE;
			}
			FINDRSCSETSTRING(edit_rsrc,"RESOURCE LABEL",label);
			FINDRSCSETCHAR(edit_rsrc,"ON SCREEN",a);
			if(FINDHoldRDArsrc(hlpdata->hold,RSRClist->libs[x])!=(-1)) a=TRUE;
				else a=FALSE;
			FINDRSCSETCHAR(edit_rsrc,"AVAILABLE",a);
			if(label!=NULL) Rfree(label);
		}
		updatersrc(edit_rsrc,"RESOURCE NAME");
		updatersrc(edit_rsrc,"RESOURCE LABEL");
		updatersrc(edit_rsrc,"ON SCREEN");
		updatersrc(edit_rsrc,"AVAILABLE");
	}
}
void makespacehelp(int pos)
{
	short x,y;
	RDASCRNrhlp *temp,*temp1,*new_helps;

	++hlpdata->numrschs;
	new_helps=Rmalloc(hlpdata->numrschs*sizeof(RDASCRNrhlp));
	y=0;
	for(x=0,temp=hlpdata->rhlp;x<(hlpdata->numrschs-1);++x,++temp)
	{
		temp1=new_helps+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->desc=NULL;
			temp1->label=NULL;
			temp1->onscreen=FALSE;
			temp1->available=FALSE;
			++y;
			temp1=new_helps+y;
		}
		if(!isEMPTY(temp->name))
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(!isEMPTY(temp->desc))
		{
			temp1->desc=stralloc(temp->desc);
			Rfree(temp->desc);
		} else temp1->desc=NULL;
		if(!isEMPTY(temp->label))
		{
			temp1->label=stralloc(temp->label);
			Rfree(temp->label);
		} else temp1->label=NULL;
		temp1->onscreen=temp->onscreen;
		temp1->available=temp->available;
		++y;
	}
	Rfree(hlpdata->rhlp);
	hlpdata->rhlp=Rmalloc(hlpdata->numrschs*sizeof(RDASCRNrhlp));
	for(x=0,temp1=hlpdata->rhlp,temp=new_helps;x<hlpdata->numrschs;++x,
		++temp,++temp1)
	{
		if(!isEMPTY(temp->name))
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(!isEMPTY(temp->desc))
		{
			temp1->desc=stralloc(temp->desc);
			Rfree(temp->desc);
		} else temp1->desc=NULL;
		if(!isEMPTY(temp->label))
		{
			temp1->label=stralloc(temp->label);
			Rfree(temp->label);
		} else temp1->label=NULL;
		temp1->onscreen=temp->onscreen;
		temp1->available=temp->available;
	}
	Rfree(new_helps);
}
static void deletedfhelp(RDArsrc *parent)
{
	int l,s;
	char *libx=NULL,*directory=NULL;

	readwidget(mainrsrc,"HELP LIBRARIES");
	if(FINDRSCGETINT(mainrsrc,"HELP LIBRARIES",&l)) return;
	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"HELP LIST",&s)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(hlp_bins->libs[l])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.HLP",directory,hlp_bins->libs[l]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.HLP",directory,hlp_bins->libs[l]);
#endif
	if(deletelibbin(libx,hlp_defs->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Help","Error cannot delete help.  Check the user's permissions on the Help Binary Library.  Call your installer.",NULL,FALSE);
	} else {
	freeapplib(hlp_defs);
	hlp_defs=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	if(hlp_defs==NULL)
	{
		hlp_defs=APPlibNEW();
		addAPPlib(hlp_defs,"Contains No Help Definitions");
	}
	if(s>=hlp_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"HELP LIST",s,hlp_defs))
		updatersrc(mainrsrc,"HELP LIST");
	}
}
static void deletetest(RDArsrc *parent)
{
	int s;

	readwidget(mainrsrc,"HELP LIST");
	if(FINDRSCGETINT(mainrsrc,"HELP LIST",&s)) return;
	if(!RDAstrcmp(hlp_defs->libs[s],"Contains No Help Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE THIS HELP?","Are you sure that you want to DELETE this HELP DEFINITION?",deletedfhelp,NULL,FALSE,parent);
}
void quitdfhelp(RDArsrc *parent)
{
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(editrsrc!=NULL) free_rsrc(editrsrc);
	if(help!=NULL) freeapplib(help);
	if(hlp_bins!=NULL) freeapplib(hlp_bins);
	if(hlp_defs!=NULL) freeapplib(hlp_defs);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(RSRClist!=NULL) freeapplib(RSRClist);
	ShutdownSubsystems();
}
void makemtnscreen(RDArsrc *parent,char *libname,char *sname,char *desc)
{
	makehelplist();
	FINDRSCSETSTRING(mtnrsrc,"HELP LIBRARY",(libname!=NULL ? libname:""));
	FINDRSCSETSTRING(mtnrsrc,"SCREEN NAME",(sname!=NULL ? sname:""));
	FINDRSCSETSTRING(mtnrsrc,"SCREEN DESCRIPTION",(desc!=NULL ? desc:""));
	FINDRSCLISTAPPlib(mtnrsrc,"RESOURCE LIST",0,help);
	changedvalues=FALSE;
	mtndefaults=GetDefaults(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quithelp,NULL,FALSE);
	changeRSRClist(mtnrsrc);
}
static void okdfhelp(RDArsrc *parent,RDArsrc *editrsrc)
{
	int s=0,h=0,x,y;
	char *dirx=NULL,*dira=NULL,*deff=NULL,*libx=NULL;
	RDAhelp *tmpdata=NULL;
	RDArhlp *f;
	RDASCRNrhlp *g;
	RDAwdgt *w;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx);
	FINDRSCGETINT(mainrsrc,"HELP LIBRARIES",&s); 
	FINDRSCGETINT(mainrsrc,"HELP LIST",&h);
	dira=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[s])+6);
#ifndef WIN32
	sprintf(dira,"%s/%s.HLP",dirx,hlp_bins->libs[s]);
#endif
#ifdef WIN32
	sprintf(dira,"%s\\%s.HLP",dirx,hlp_bins->libs[s]);
#endif
	hlpdata=RDASCRNhelpNEW();
	tmpdata=RDAhelpNEW();
	if(RDAstrcmp(hlp_defs->libs[h],"Contains No Help Definitions"))
	{
		deff=stralloc(hlp_defs->libs[h]);
		gethelpbin(dira,deff,tmpdata,FALSE);
	}
	if(tmpdata!=NULL)
	{
		if(!isEMPTY(tmpdata->name)) hlpdata->name=stralloc(tmpdata->name);
			else hlpdata->name=NULL;
		if(!isEMPTY(tmpdata->desc)) hlpdata->desc=stralloc(tmpdata->desc);
			else hlpdata->desc=NULL;
		hlpdata->screen=RDAscrnNEW(hlp_bins->libs[s],hlpdata->name);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[s])+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.SCN",dirx,hlp_bins->libs[s]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.SCN",dirx,hlp_bins->libs[s]);
#endif
		getscrnbin(libx,hlpdata->screen,FALSE);
		if(libx!=NULL) Rfree(libx);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[s])+2);
#ifndef WIN32
		sprintf(libx,"%s/%s",dirx,hlp_bins->libs[s]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s",dirx,hlp_bins->libs[s]);
#endif
		hlpdata->hold=ReadRsrcList(libx,hlpdata->name);
		if(libx!=NULL) Rfree(libx);
		if(tmpdata->rhlp!=NULL)
		{
			hlpdata->rhlp=Rmalloc(tmpdata->numrschs*sizeof(RDASCRNrhlp));
			for(x=0,f=tmpdata->rhlp,g=hlpdata->rhlp;x<tmpdata->numrschs;
				++x,++f,++g)
			{
				if(f->name!=NULL) g->name=stralloc(f->name);
					else g->name=NULL;
				if(f->desc!=NULL) g->desc=stralloc(f->desc);
					else g->desc=NULL;
				if(!isEMPTY(f->label)) g->label=stralloc(f->label);
					else g->label=NULL;
				if(((y=FINDWDGT(hlpdata->screen,f->name))!=(-1)))
				{
					if(isEMPTY(g->label))
					{
						w=hlpdata->screen->wdgts+y;
						if(!isEMPTY(w->label)) 
						{
							g->label=stralloc(w->label);
						} else if(!isEMPTY(w->resource_name)) 
						{
							g->label=stralloc(w->resource_name);
						}
					}
					g->onscreen=TRUE;
				} else {
					g->onscreen=FALSE;
				}
				if(FINDHoldRDArsrc(hlpdata->hold,f->name)!=(-1))
				{
					g->available=TRUE;
				} else g->available=FALSE;
				++hlpdata->numrschs;
			}
		}
		free_help(tmpdata);
	}
	dira[RDAstrlen(dira)-4]=0;
	makemtnscreen(parent,dira,deff,hlpdata->desc);
	if(deff!=NULL) Rfree(deff);
	if(dirx!=NULL) Rfree(dirx);
	if(dira!=NULL) Rfree(dira);
}
static void newdfhelp(RDArsrc *parent,RDArsrc *editrsrc)
{
	int s=0;
	char *dirx=NULL,*dira=NULL,*deflib=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx);
	FINDRSCGETINT(mainrsrc,"HELP LIBRARIES",&s); 
	if(RDAstrcmp(hlp_bins->libs[s],"Directory Contains No Libraries"))
	{
		deflib=stralloc(hlp_bins->libs[s]);
	} else deflib=stralloc(" ");
	dira=Rmalloc(RDAstrlen(dirx)+RDAstrlen(deflib)+2);
#ifndef WIN32
	sprintf(dira,"%s/%s",dirx,deflib);
#endif
#ifdef WIN32
	sprintf(dira,"%s\\%s",dirx,deflib);
#endif
	hlpdata=RDASCRNhelpNEW();
	hlpdata->rhlp=Rmalloc(sizeof(RDASCRNrhlp));
	makemtnscreen(parent,dira,NULL,NULL);
	if(deflib!=NULL) Rfree(deflib);
	if(dirx!=NULL) Rfree(dirx);
	if(dira!=NULL) Rfree(dira);
}
static void makehelplist(void)
{
	short x;
	char *s;
	RDASCRNrhlp *h;

	if(help!=NULL)
	{
		freeapplib(help);
		help=NULL;
	}
	help=APPlibNEW();
	for(x=0,h=hlpdata->rhlp;x<hlpdata->numrschs;++x,++h)
	{
		s=Rmalloc((RDAstrlen(h->name)+RDAstrlen(h->label)+53));
		sprintf(s,"Resource: [%s] Label: [%s] On Screen: %-3s Available: %-3s",
			(h->name!=NULL ? h->name:""),(h->label!=NULL ? h->label:""),
			(h->onscreen ? "Yes":"No"),(h->available ? "Yes":"No"));
		addAPPlib(help,s);
		Rfree(s);
	}
	if(help->numlibs<1)
	{
		addAPPlib(help,"Resource: [] Label: [] On Screen: No  Available: No");
		++hlpdata->numrschs;
		if(hlpdata->rhlp==NULL) hlpdata->rhlp=Rmalloc(sizeof(RDASCRNrhlp));
			else hlpdata->rhlp=Rrealloc(hlpdata->rhlp,sizeof(RDASCRNrhlp));
		h=hlpdata->rhlp;
		h->name=NULL;
		h->label=NULL;
		h->desc=NULL;	
		h->onscreen=FALSE;
		h->available=FALSE;
	}
}
static void helpscreen(RDArsrc *parent,char *name,char *desc,char *label,
	char onscreen,char avail,void (*save_function)(...),
	void (*quit_functiontest)(...))
{
	char *hlib=NULL;
	char *sname=NULL;
	int x=0;

	readwidget(mtnrsrc,"HELP LIBRARY");
	readwidget(mtnrsrc,"SCREEN NAME");
	if(!FINDRSCGETSTRING(mtnrsrc,"HELP LIBRARY",&hlib))
		FINDRSCSETSTRING(editrsrc,"HELP LIBRARY",hlib);
	if(hlib!=NULL) Rfree(hlib);
	if(!FINDRSCGETSTRING(mtnrsrc,"SCREEN NAME",&sname))
		FINDRSCSETSTRING(editrsrc,"SCREEN NAME",sname);
	if(sname!=NULL) Rfree(sname);
	FINDRSCLISTAPPlib(editrsrc,"RESOURCES AVAILABLE",x,RSRClist);
	updatersrc(editrsrc,"RESOURCES AVAILABLE");
	FINDRSCSETSTRING(editrsrc,"RESOURCE NAME",(name!=NULL ? name:""));
	FINDRSCSETSTRING(editrsrc,"RESOURCE DESCRIPTION",(desc!=NULL ? desc:""));
	FINDRSCSETSTRING(editrsrc,"RESOURCE LABEL",(label!=NULL ? label:""));
	FINDRSCSETCHAR(editrsrc,"ON SCREEN",onscreen);
	FINDRSCSETCHAR(editrsrc,"AVAILABLE",avail);
	FINDRSCSETFUNC(editrsrc,"SAVE",save_function,NULL);
	FINDRSCSETFUNC(editrsrc,"QUIT",quit_functiontest,NULL);
	APPmakescrn(editrsrc,TRUE,NULL,NULL,FALSE);
	changersrcname(editrsrc);
	editdefaults=GetDefaults(editrsrc);
}
static void quitresource(RDArsrc *editrsrc)
{
	if(editrsrc!=NULL) killwindow(editrsrc);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
	NULL_RSCS(editrsrc);
}
static void quitresourcetest(RDArsrc *editrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,1))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Resource Help Definition.\nExiting now without saving will loose these changes.",saveresource,quitresource,FALSE,editrsrc);
	} else {
		quitresource(editrsrc);
	}
}
static void quitresourceabovetest(RDArsrc *editrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,1))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Resource Help Definition.\nExiting now without saving will loose these changes.",save_resource_above,quitresource,FALSE,editrsrc);
	} else {
		quitresource(editrsrc);
	}
}
static void quitresourcebelowtest(RDArsrc *editrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,1))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Resource Help Definition.\nExiting now without saving will loose these changes.",save_resource_below,quitresource,FALSE,editrsrc);
	} else {
		quitresource(editrsrc);
	}
}
static void saveresource(RDArsrc *editrsrc)
{
	int value;
	RDASCRNrhlp *fds;

	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&value)) return;
	fds=hlpdata->rhlp+value;
	if(fds==NULL) prterr("fds=NULL\n");
	readallwidgets(editrsrc);
	FINDRSCGETSTRING(editrsrc,"RESOURCE NAME",&fds->name);
	FINDRSCGETSTRING(editrsrc,"RESOURCE DESCRIPTION",&fds->desc);
	FINDRSCGETSTRING(editrsrc,"RESOURCE LABEL",&fds->label);
	FINDRSCGETCHAR(editrsrc,"ON SCREEN",&fds->onscreen);
	FINDRSCGETCHAR(editrsrc,"AVAILABLE",&fds->available);
	makehelplist();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"RESOURCE LIST",value,help))
	updatersrc(mtnrsrc,"RESOURCE LIST");
 	if(COMPARE_RSRCS(editrsrc,editdefaults,1))
 	{
		changedvalues=TRUE;
	}
	quitresource(editrsrc);
}
static void save_resource_above(RDArsrc *parent)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&x)) return;
	makespacehelp(x);
	saveresource(parent);
}
static void save_resource_below(RDArsrc *parent)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&x)) return;
	makespacehelp(++x);
	if(!FINDRSCSETINT(mtnrsrc,"RESOURCE LIST",x))
		updatersrc(mtnrsrc,"RESOURCE LIST");
	saveresource(parent);
}
static void add_rsc_above(RDArsrc *parent)
{
	helpscreen(parent,NULL,NULL,NULL,FALSE,FALSE,save_resource_above,
		quitresourceabovetest);
}
static void add_rsc_below(RDArsrc *parent)
{
	helpscreen(parent,NULL,NULL,NULL,FALSE,FALSE,save_resource_below,
		quitresourcebelowtest);
}
static void delete_rsc(RDArsrc *parent)
{
	short x,y;
	RDASCRNrhlp *temp,*temp1,*new_helps;
	int value;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&value)) return;
	if(hlpdata->numrschs==1) return;
	new_helps=Rmalloc((hlpdata->numrschs-1)*sizeof(RDASCRNrhlp));
	y=0;
	for(x=0,temp=hlpdata->rhlp;x<hlpdata->numrschs;++x,++temp)
	{
		temp1=new_helps+y;
		if(x!=value)
		{
			if(!isEMPTY(temp->name))
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			if(!isEMPTY(temp->desc))
			{
				temp1->desc=stralloc(temp->desc);
				Rfree(temp->desc);
			} else temp1->desc=NULL;
			if(!isEMPTY(temp->label))
			{
				temp1->label=stralloc(temp->label);
				Rfree(temp->label);
			} else temp1->label=NULL;
			temp1->onscreen=temp->onscreen;
			temp1->available=temp->available;
			++y;
		} else {
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->desc!=NULL) Rfree(temp->desc);
			if(temp->label!=NULL) Rfree(temp->label);
		}
	}
	Rfree(hlpdata->rhlp);
	--hlpdata->numrschs;
	hlpdata->rhlp=Rmalloc(hlpdata->numrschs*sizeof(RDASCRNrhlp));
	for(x=0,temp1=hlpdata->rhlp,temp=new_helps;x<hlpdata->numrschs;++x,
		++temp,++temp1)
	{
		if(!isEMPTY(temp->name))
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(!isEMPTY(temp->desc))
		{
			temp1->desc=stralloc(temp->desc);
			Rfree(temp->desc);
		} else temp1->desc=NULL;
		if(!isEMPTY(temp->label))
		{
			temp1->label=stralloc(temp->label);
			Rfree(temp->label);
		} else temp1->label=NULL;
		temp1->onscreen=temp->onscreen;
		temp1->available=temp->available;
	}
	Rfree(new_helps);
	makehelplist();
	if(value>=hlpdata->numrschs) value=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"RESOURCE LIST",value,help))
		updatersrc(mtnrsrc,"RESOURCE LIST");
	changedvalues=TRUE;
}
static void edit_resource(RDArsrc *parent)
{
	int x;
	RDASCRNrhlp *f;

	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&x)) return;
	f=hlpdata->rhlp+x;
	helpscreen(parent,f->name,f->desc,f->label,f->onscreen,f->available,saveresource,quitresourcetest);
}
static void quithelp(RDArsrc *mtnrsrc)
{
	if(hlpdata!=NULL) FreeSCRNhelp(hlpdata);
	if(help!=NULL) freeapplib(help);
	if(mtnrsrc!=NULL) killwindow(mtnrsrc);
	if(mtndefaults!=NULL) FreeRDAdefault(mtndefaults);
	NULL_RSCS(mtnrsrc);
}
static void quithelptest(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,0) || changedvalues)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this Help Definition.\nExiting now without saving will loose these changes.",savehelp,quithelp,FALSE,mtnrsrc);
	} else {
		quithelp(mtnrsrc);
	}
}
static void savehelp(RDArsrc *mtnrsrc)
{
	char *libname=NULL,*libx=NULL;
	RDAhelp *tmpdata=NULL;
	RDArhlp *h;
	RDASCRNrhlp *g;
	int x;

	readallwidgets(mtnrsrc);
	tmpdata=RDAhelpNEW();
	FINDRSCGETSTRING(mtnrsrc,"SCREEN DESCRIPTION",&tmpdata->desc);
	if(FINDRSCGETSTRING(mtnrsrc,"SCREEN NAME",&tmpdata->name)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"HELP LIBRARY",&libname))
	{
		prterr("Error Couldn't write screen because the library name resource was not found.");
		return;
	}
	libx=Rmalloc(RDAstrlen(libname)+5);
	sprintf(libx,"%s.HLP",libname);
	if(hlpdata->rhlp!=NULL)
	{
		tmpdata->rhlp=Rmalloc(hlpdata->numrschs*sizeof(RDArhlp));
		for(x=0,h=tmpdata->rhlp,g=hlpdata->rhlp;x<hlpdata->numrschs;
			++x,++h,++g)
		{
			if(!isEMPTY(g->name)) h->name=stralloc(g->name);
				else h->name=NULL;
			if(!isEMPTY(g->desc)) h->desc=stralloc(g->desc);
				else h->desc=NULL;
			if(!isEMPTY(g->label)) h->label=stralloc(g->label);
			else h->label=NULL;
			++tmpdata->numrschs;
		}
	}
	writehelpbin(libx,tmpdata);
	if(tmpdata!=NULL) free_help(tmpdata);
	gethelpentries(mtnrsrc);
	if(libname!=NULL) Rfree(libname);
	if(libx!=NULL) Rfree(libx);
	quithelp(mtnrsrc);
}
static void getdirectoryhelp(RDArsrc *parent)
{
	int x,selectedl=0;
	short y=0;
	char *tmp=NULL,*directory=NULL,*dirx=NULL;
	RDArmem *member;

	if((y=FINDRSC(mainrsrc,"DIRECTORY"))!=(-1)) 
	{
		member=mainrsrc->rscs+y;
		if(IsWidgetDisplayed(member))
		{
			FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory);
		} else {
			if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&x)) return;
			directory=stralloc(dir_libs->libs[x]);
		}
	}
	FINDRSCGETINT(mainrsrc,"HELP LIBRARIES",&selectedl);
	if(hlp_bins!=NULL) freeapplib(hlp_bins);
	hlp_bins=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(directory)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",directory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",directory);
#endif
	for(x=0;findfile(dirx,"*.HLP",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(hlp_bins,tmp);
	}
	if(hlp_bins->numlibs<1)
	{
		addAPPlib(hlp_bins,"Directory Contains No Libraries");
	}
	if(directory!=NULL) Rfree(directory);
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(hlp_bins);
	if(selectedl>=hlp_bins->numlibs) selectedl=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"HELP LIBRARIES",selectedl,hlp_bins))
		updatersrc(mainrsrc,"HELP LIBRARIES");
}
void gethelpentries(RDArsrc *parent)
{
	int selected,selectedh=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"HELP LIBRARIES",&selected)) return;
	FINDRSCGETINT(mainrsrc,"HELP LIST",&selectedh);
	if(hlp_defs!=NULL) freeapplib(hlp_defs);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(hlp_bins->libs[selected])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.HLP",dirx,hlp_bins->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.HLP",dirx,hlp_bins->libs[selected]);
#endif
	hlp_defs=getlibnames(libx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(hlp_defs==NULL)
	{
		hlp_defs=APPlibNEW();
		addAPPlib(hlp_defs,"Contains No Help Definitions");
	}
	if(selectedh>=hlp_defs->numlibs) selectedh=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"HELP LIST",selectedh,hlp_defs))
		updatersrc(mainrsrc,"HELP LIST");
}
static void setdirlist(RDArsrc *parent)
{
	int x=0;
	short y=0;
	char *directory=NULL;
	char inlist=FALSE;
	RDArmem *member;

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
	if((y=FINDRSC(mainrsrc,"DIRECTORY LIST"))!=(-1)) 
	{
		member=mainrsrc->rscs+y;
		if(IsWidgetDisplayed(member))
		{
			FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
			updatersrc(mainrsrc,"DIRECTORY LIST");
			getdirectoryhelp(parent);
			gethelpentries(parent);
		}
	}
	if(directory!=NULL) Rfree(directory);
}
static void getdir(RDArsrc *parent)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(!FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr))
	{
		if(RDAstrcmp(dirstr,dir_libs->libs[s]))
		{
			FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s]);
			updatersrc(mainrsrc,"DIRECTORY");
		}
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getdirectoryhelp(parent);
	gethelpentries(parent);
}
APPlib *MakeRsrcList(HoldRDArsrc *h)
{
	int x;
	APPlib *tmp=NULL;
	HoldRDArmem *r;

	tmp=APPlibNEW();
	if(h!=NULL)
	{
		if(h->rscs!=NULL)
		{
			for(x=0,r=h->rscs;x<h->num;++x,++r)
			{
				if(!isEMPTY(r->name)) addAPPlib(tmp,r->name);
			}
		}
	}
	if(tmp->numlibs<1) addAPPlib(tmp,"Not Available");
	return(tmp);
}
static void changeRSRClist(RDArsrc *mtnrsrc)
{
	char *libname=NULL,*name=NULL,*libx=NULL;
	char *modx=NULL;
	int value=0;

	readwidget(mtnrsrc,"HELP LIBRARY");
	FINDRSCGETSTRING(mtnrsrc,"HELP LIBRARY",&libname);
	readwidget(mtnrsrc,"SCREEN NAME");
	FINDRSCGETSTRING(mtnrsrc,"SCREEN NAME",&name);
	FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&value);
	if(hlpdata->screen!=NULL) free_scrn(hlpdata->screen);
#ifndef WIN32
	modx=strrchr(libname,'/');
#endif
#ifdef WIN32
	modx=strrchr(libname,'\\');
#endif
	++modx;
	hlpdata->screen=RDAscrnNEW(modx,hlpdata->name);
	libx=Rmalloc(RDAstrlen(libname)+6);
	sprintf(libx,"%s.SCN",libname);
	getscrnbin(libx,hlpdata->screen,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(hlpdata->hold!=NULL) FreeHoldRDArsrc(hlpdata->hold);
	hlpdata->hold=ReadRsrcList(libname,hlpdata->name);
	if(RSRClist!=NULL) freeapplib(RSRClist);
	if(libname!=NULL) Rfree(libname);
	if(name!=NULL) Rfree(name);
	RSRClist=MakeRsrcList(hlpdata->hold);
	if(RSRClist==NULL) RSRClist=APPlibNEW();
	if(RSRClist->numlibs<1) addAPPlib(RSRClist,"Not Available");
	makehelplist();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"RESOURCE LIST",value,help))
	updatersrc(mtnrsrc,"RESOURCE LIST");
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *defdir=NULL,a=FALSE;
	int x=0,w=2,m=0,s=0;

	if(InitializeSubsystems(argc,argv,module,"DEFINE HELP")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"DEFINE HELP");
	mtnrsrc=RDArsrcNEW(module,"MAINTAIN HELP");
	editrsrc=RDArsrcNEW(module,"MAINTAIN RESOURCE HELP");
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
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",w,dir_libs);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[0],TRUE);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist,NULL);
	if(defdir!=NULL) Rfree(defdir);
	hlp_bins=APPlibNEW();
	addlstrsrc(mainrsrc,"HELP LIBRARIES",&m,TRUE,gethelpentries,
		hlp_bins->numlibs,&hlp_bins->libs,NULL);
	getdir(mainrsrc);
	hlp_defs=APPlibNEW();
	addlstrsrc(mainrsrc,"HELP LIST",&s,TRUE,NULL,hlp_defs->numlibs,
		&hlp_defs->libs,NULL);
	gethelpentries(mainrsrc);
	if(argc>1 && hlp_bins!=NULL)
	{
		for(m=0;m<hlp_bins->numlibs;++m)
		{
			if(!RDAstrcmp(hlp_bins->libs[m],argv[1])) break;
		}
		if(m<hlp_bins->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"HELP LIBRARIES",m);
		}
		else
		{
			m=0;
			FINDRSCSETINT(mainrsrc,"HELP LIBRARIES",m);
		}
		getdir(mainrsrc);
	}
	if(argc>2 && hlp_defs!=NULL)
	{
		for(s=0;s<hlp_defs->numlibs;++s)
		{
			if(!RDAstrcmp(hlp_defs->libs[s],argv[2])) break;
		}
		if(s<hlp_defs->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"HELP LIST",s);
		}
		else
		{
			s=0;
			FINDRSCSETINT(mainrsrc,"HELP LIST",s);
		}
	}
	addbtnrsrc(mainrsrc,"ADD",TRUE,newdfhelp,editrsrc);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okdfhelp,editrsrc);
	addbtnrsrc(mainrsrc,"DELETE SCREEN HELP",TRUE,deletetest,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfhelp,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addstdrsrc(mtnrsrc,"HELP LIBRARY",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mtnrsrc,"SCREEN NAME",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mtnrsrc,"SEARCH FIELD",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"SEARCH FIELD",schfldcb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"HELP LIBRARY",changeRSRClist,NULL);
	FINDRSCSETFUNC(mtnrsrc,"SCREEN NAME",changeRSRClist,NULL);
	addsctrsrc(mtnrsrc,"SCREEN DESCRIPTION",0,NULL,TRUE);
	hlpdata=RDASCRNhelpNEW();
	makehelplist();
	addlstrsrc(mtnrsrc,"RESOURCE LIST",0,TRUE,NULL,help->numlibs,
		&help->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD ABOVE",TRUE,add_rsc_above,NULL);
	addbtnrsrc(mtnrsrc,"ADD BELOW",TRUE,add_rsc_below,NULL);
	addbtnrsrc(mtnrsrc,"SELECT",TRUE,edit_resource,NULL);
	addbtnrsrc(mtnrsrc,"DELETE RESOURCE",TRUE,delete_rsc,NULL);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,savehelp,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quithelptest,NULL);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addstdrsrc(editrsrc,"HELP LIBRARY",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(editrsrc,"SCREEN NAME",VARIABLETEXT,0,NULL,FALSE);
	if(RSRClist==NULL) RSRClist=APPlibNEW();
	x=0;
	addlstrsrc(editrsrc,"RESOURCES AVAILABLE",&x,TRUE,changersrclist,RSRClist->numlibs,&RSRClist->libs,NULL);
	addstdrsrc(editrsrc,"RESOURCE NAME",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(editrsrc,"RESOURCE NAME",changersrcname,NULL);
	addstdrsrc(editrsrc,"RESOURCE LABEL",VARIABLETEXT,0,NULL,TRUE);
	addsctrsrc(editrsrc,"RESOURCE DESCRIPTION",0,NULL,TRUE);
	addstdrsrc(editrsrc,"ON SCREEN",BOOLNS,1,&a,FALSE);
	addstdrsrc(editrsrc,"AVAILABLE",BOOLNS,1,&a,FALSE);
	addrfcbrsrc(editrsrc,"SAVE",TRUE,saveresource,NULL);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quitresourcetest,NULL);
	addbtnrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	FINDRSCSETINPUTFOCUS(mainrsrc,"HELP LIBRARIES");
	APPmakescrn(mainrsrc,TRUE,quitdfhelp,NULL,TRUE);
}
