/* dfrptvir.c - Virtual Field Section of Define Reports */
#include <cstdio>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

static APPlib *methods=NULL,*ftypes=NULL,*viravl=NULL,*cmpwhen=NULL;
static APPlib *ezviravl=NULL,*mod_list=NULL;
extern RDArsrc *mainrsrc;
extern APPlib *modlstx;
extern RDAreport *report;
extern char *module;
extern void loadvalue(RDArsrc *,char *);
extern char changedreport;
static void loadvirtual(RDArsrc *);
static void makemodlist(void);
static void makeezvirlisttest(RDArsrc *);
static void makeezvirlist(RDArsrc *);
static void quitlvir(RDArsrc *),changevdesc(RDArsrc *);
static RDAvirtual *holdvir=NULL;
static int numvir=0,virmodpointer=0;
static RDAdefault *virdefaults=NULL,*vscndefaults=NULL;
static char changedvirtuals=FALSE;
static void copyvirtual(RDArsrc *);
static RDArsrc *definevirlst=NULL;
static void makespacevirtualfield(int);
static void mk_define_virtual_list(RDArsrc *);
static void makevirtuallist(void);
static void mkvirlst(RDAvirtual *,int);
static void definevirlstfunc(RDArsrc *,RDArsrc *);
static void savevirlst(RDArsrc *,RDArsrc *);
static void savevirlstcb(RDArsrc *,RDArsrc *);

static void definevirlstfunc(RDArsrc *parent,RDArsrc *definevirlst)
{
	APPmakescrn(definevirlst,TRUE,NULL,NULL,FALSE);
}
static void savevirlstcb(RDArsrc *definevirlst,RDArsrc *parent)
{
	savevirlst(definevirlst,parent);
	if(IsScreenDisplayed(definevirlst)) NULL_RSCS(definevirlst);
}
static void savevirlst(RDArsrc *definevirlst,RDArsrc *parent)
{
	int x=0;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v=NULL;

	if(IsScreenDisplayed(definevirlst)) readallwidgets(definevirlst);
	if(viravl!=NULL) freeapplib(viravl);
	viravl=APPlibNEW();
#ifdef DIAGNOSTIC_SCREENS
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,
		"Reading User Virtual Fields Defaults",NULL,numvir);
#else
	tmprsrc=NULL;
#endif
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			ForceWindowUpdate(tmprsrc);
		}
	}
	if(holdvir!=NULL)
	{
		for(x=0,v=holdvir;x<numvir;++x,++v)
		{
			mkvirlst(v,x);
			if(tmprsrc!=NULL)
				update_diagnostic(tmprsrc,TRUE);
		}
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(viravl->numlibs<1)
	{
		addAPPlib(viravl,"No Virtual Fields Defined");
	}
	if(definevirlst!=NULL)
	{
		if(!FINDRSCLISTAPPlib(parent,"VIRTUAL FIELDS",0,viravl))
		{
			updatersrc(parent,"VIRTUAL FIELDS");
		}
	}
}
static void mk_define_virtual_list(RDArsrc *definevirlst)
{
	short l,count;

	count=1;
	l=7;
	addstdrsrc(definevirlst,"NUMBER POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(definevirlst,"NUMBER LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=20;
	addstdrsrc(definevirlst,"NAME POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(definevirlst,"NAME LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=60;
	addstdrsrc(definevirlst,"EXPRESSION POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(definevirlst,"EXPRESSION LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=25;
	addstdrsrc(definevirlst,"TYPE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(definevirlst,"TYPE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=3;
	addstdrsrc(definevirlst,"LENGTH POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(definevirlst,"LENGTH LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=11;
	addstdrsrc(definevirlst,"COMPUTE WHEN POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(definevirlst,"COMPUTE WHEN LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=20;
	addstdrsrc(definevirlst,"MODE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(definevirlst,"MODE LENGTH",SHORTV,3,&l,TRUE);

	addbtnrsrc(definevirlst,"DEFAULTS",TRUE,SaveDefaultsUsedOnly,NULL);
	addrfkwrsrc(definevirlst,"SELECT",TRUE,NULL,NULL);
	addrfkwrsrc(definevirlst,"QUIT",TRUE,NULL_RSCS,NULL);
	addrfcbrsrc(definevirlst,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(definevirlst,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(definevirlst);
}
static void mkvirlst(RDAvirtual *v,int number)
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s=NULL,*hold=NULL;

	if(viravl!=NULL)
	{
		if(viravl->numlibs==1 && !RDAstrcmp(viravl->libs[0],
		"No Virtual Fields Defined"))
		{
			freeapplib(viravl);
			viravl=APPlibNEW();
		}
	} else {
		viravl=APPlibNEW();
	}
	for(x=0,length=0;x<(definevirlst->numrscs-5);x+=2)
	{
		pos_member=definevirlst->rscs+x;
		len_member=definevirlst->rscs+(x+1);
		if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
		{
			length+=(*len_member->value.short_value+1);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(definevirlst->numrscs-5))
	{
		for(x=0;x<(definevirlst->numrscs-5);x+=2)
		{
			pos_member=definevirlst->rscs+x;
			len_member=definevirlst->rscs+(x+1);
			if(*pos_member->value.short_value>0 &&
				*len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					if(!RDAstrcmp("NUMBER",s)) 
					{
						sprintf(&hold[lcount],"[%*d] ",((*len_member->value.short_value)-2),(number+1));
/*
						if(*len_member->value.short_value<RDAstrlen(v->name))
						{
							sprintf(&hold[lcount],"[%*d] ",((*len_member->value.short_value)-2),(number+1));
						} else {
							sprintf(&hold[lcount],"[%-*d] ",((*len_member->value.short_value)-2),(number+1));
						}
*/
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("NAME",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(v->name))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(v->name!=NULL?v->name:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(v->name!=NULL?v->name:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("EXPRESSION",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(v->expression))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(v->expression!=NULL?v->expression:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(v->expression!=NULL?v->expression:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("TYPE",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(standardfieldtypes[v->type]))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(standardfieldtypes[v->type]!=NULL?standardfieldtypes[v->type]:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(standardfieldtypes[v->type]!=NULL?standardfieldtypes[v->type]:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("LENGTH",s)) 
					{
						sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,v->len);
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("COMPUTE WHEN",s)) 
					{
						if(*len_member->value.short_value<11)
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(v->when==0?"Select Time":"Output Time"));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(v->when==0?"Select Time":"Output Time"));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("MODE",s)) 
					{
						memset(stemp,0,101);
						switch(v->range)
						{
							case 0:	sprintf(stemp,"NORMAL COMPUTATION");
								break;
							case 1:	sprintf(stemp,"RANGE ON COMPUTATION");
								break;
							case 2:	sprintf(stemp,"DEFAULT COMPUTATION");
								break;
							case 3:	sprintf(stemp,"ENVIRONMENT COMPUTATION");
								break;
							case 4: sprintf(stemp,"COMBO VIRTUAL");
								break;
							default:
								sprintf(stemp,"INVALID VALUE!!!");
						}
						if(*len_member->value.short_value<RDAstrlen(stemp))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(stemp!=NULL?stemp:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(stemp!=NULL?stemp:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else {
						prterr("Error field [%s] not found.",s);
						if(s!=NULL) Rfree(s);
					}
				}
			}
		}
		++cur;
	}
	addAPPlib(viravl,hold);
	if(hold!=NULL) Rfree(hold);
}
static void makevirtuallist(void)
{
	int x=0;
	RDAvirtual *v=NULL;

	if(viravl!=NULL) freeapplib(viravl);
	viravl=APPlibNEW();
	if(holdvir!=NULL)
	{
		for(x=0,v=holdvir;x<numvir;++x,++v)
		{
			mkvirlst(v,x);
		}
	}
	if(viravl->numlibs<1)
	{
		addAPPlib(viravl,"No Virtual Fields Defined");
	}
}

static void makespacevirtualfield(int pos)
{
	short x,y;
	RDAvirtual *temp,*temp1,*new_fields;

	++numvir;
	new_fields=Rmalloc(numvir*sizeof(RDAvirtual));
	y=0;
	for(x=0,temp=holdvir;x<(numvir-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->type=0;
			temp1->when=0;
			temp1->len=0;
			temp1->expression=NULL;
			temp1->range=FALSE;
			temp1->computed=FALSE;
			++y;
			temp1=new_fields+y;
		}
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->computed=temp->computed;
		temp1->when=temp->when;
		temp1->len=temp->len;
		temp1->range=temp->range;
		++y;
	}
	Rfree(holdvir);
	holdvir=Rmalloc(numvir*sizeof(RDAvirtual));
	for(x=0,temp1=holdvir,temp=new_fields;x<numvir;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->when=temp->when;
		temp1->len=temp->len;
		temp1->range=temp->range;
		temp1->computed=temp->computed;
	}
	Rfree(new_fields);
}
static void quitvscn(RDArsrc *vscnrsrc,RDArsrc *virrsrc)
{
	if(vscnrsrc!=NULL)
	{
		killwindow(vscnrsrc);
		free_rsrc(vscnrsrc);
	}
	if(ftypes!=NULL) freeapplib(ftypes);
	if(cmpwhen!=NULL) freeapplib(cmpwhen);
	if(methods!=NULL) freeapplib(methods);
	if(vscndefaults!=NULL) FreeRDAdefault(vscndefaults);
}
static void makeezvirlisttest(RDArsrc *r)
{
	readwidget(r,"MODULE LIST");
	FINDRSCGETINT(r,"MODULE LIST",&virmodpointer);
	if(ezviravl!=NULL) { freeapplib(ezviravl); ezviravl=NULL; }
	makeezvirlist(r);
}
static void makeezvirlist(RDArsrc *r)
{
	APPlib *virflist=NULL;
    	DFvirtual *d=NULL;
	RDAsearch *tmpsch=NULL;
	DFincvir *a=NULL;
	char *holdmodule=NULL;
	char *tmpx=NULL,addezvf=FALSE,*libx=NULL;
	int x,y,z;
	short addedtolist=FALSE;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	holdmodule=stralloc(mod_list->libs[virmodpointer]);
	if(virflist!=NULL) { freeapplib(virflist); virflist=NULL; }
	if(ezviravl==NULL) 
	{
		if(ezviravl!=NULL) { freeapplib(ezviravl); ezviravl=NULL; }
		ezviravl=APPlibNEW(); 
		if(report->search==NULL) 
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","NO SEARCH FILES SELECTED!","There has been no search files defined for this report.  In order to load an EZ-VIRTUAL FIELD this must be done first.",NULL,NULL,TRUE,NULL);
		} else {
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(holdmodule)+10);
			tmpx=adddashes(holdmodule);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
			virflist=getlibnames(libx,FALSE);
			if(libx!=NULL) { Rfree(libx); libx=NULL; }
			if(virflist!=NULL)
			{
				e=Rmalloc(RDAstrlen(holdmodule)+27);
				sprintf(e,"Collecting Valid %s Virtuals",holdmodule);
#ifdef RS6000
				tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN",e,NULL,virflist->numlibs);
#else
				tmprsrc=NULL;
				tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN",e,NULL,virflist->numlibs);
#endif
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
				}
				for(y=0;y<virflist->numlibs;++y)
				{
					addedtolist=FALSE;
					d=getDFvirtual(holdmodule,virflist->libs[y]);
					if(d!=NULL)
					{
						addezvf=FALSE;
						if(d->incvir!=NULL)
						{
						if(!RDAstrcmp(d->incvir->module,holdmodule) &&
						  d->incvir->file==NULL)
						{
							addezvf=TRUE;
						} else {
							for(z=0,a=d->incvir;z<d->num;++z,++a)
							{
								if(d->vtype==1) /*One of Many Available*/
								{
									for(x=0,tmpsch=report->search;x<report->numsearchs;++x,++tmpsch)
									{
										if(!RDAstrcmp(tmpsch->module,a->module) && 
											!RDAstrcmp(tmpsch->filename,a->file)) break;
									}
									if(x<report->numsearchs)
									{
										addezvf=TRUE;
										break;
									} 
									
								} else if(d->vtype==0) /*All Available*/
								{
									for(x=0,tmpsch=report->search;x<report->numsearchs;++x,++tmpsch)
									{
										if(!RDAstrcmp(tmpsch->module,a->module) &&
											!RDAstrcmp(tmpsch->filename,a->file)) break;
									}
									if(x>=report->numsearchs)
									{
										addezvf=FALSE;
										break;
									} else {
										addezvf=TRUE;
									}
								}
							}
						}
						}
						if(addezvf==TRUE)
						{
							addedtolist=TRUE;
							addAPPlibNoDuplicates(ezviravl,d->name);
						}
					}
					FreeDFvirtual(d);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,
								(addedtolist ? TRUE:FALSE));
				}
			}
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc); 
		}
		if(holdmodule!=NULL) Rfree(holdmodule);
		if(tmpx!=NULL) Rfree(tmpx);
		if(libx!=NULL) Rfree(libx);
		if(ezviravl->numlibs<1)
		{
			addAPPlib(ezviravl,"No EZ-Virtual Fields Defined");
		}
	}
	if(virflist!=NULL) { freeapplib(virflist); virflist=NULL; }
	if(!FINDRSCLISTAPPlib(r,"VIRTUALS AVAILABLE",0,ezviravl))
	{
		updatersrc(r,"VIRTUALS AVAILABLE");
	}
}
static void changevdesc(RDArsrc *r)
{
	char *temp=NULL;
	int selectedm=0,selectedn=0;

	if(FINDRSCGETINT(r,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(r,"VIRTUALS AVAILABLE",&selectedn)) return;
	if(ezviravl!=NULL)
	{
	if(ezviravl->libs!=NULL)
	{
	if(!RDAstrcmp(ezviravl->libs[selectedn],"No EZ-Virtual Fields Defined"))
	{
		temp=NULL;
	} else {
		temp=getvirtualdesc(mod_list->libs[selectedm],
			ezviravl->libs[selectedn]);
	}
	if(!FINDRSCSETSTRING(r,"DESCRIPTION",temp))
		updatersrc(r,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
	}
	}
}
static void quitlvir(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
}
static void oklvir(RDArsrc *r,RDArsrc *vscnrsrc)
{
	DFvirtual *d=NULL;
	int x=0,selectedm=0,selectedv=0;

	if(FINDRSCGETINT(r,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(r,"VIRTUALS AVAILABLE",&selectedv)) return;
	d=getDFvirtual(mod_list->libs[selectedm],ezviravl->libs[selectedv]);
	if(d!=NULL)
	{
		FINDRSCSETSTRING(vscnrsrc,"NAME",d->name);
		x=(int)d->type;
		FINDRSCSETINT(vscnrsrc,"FIELD TYPES",x);
		x=(int)d->when;
		FINDRSCSETINT(vscnrsrc,"COMPUTE WHEN",x);
		FINDRSCSETSHORT(vscnrsrc,"LENGTH",d->length);
		FINDRSCSETSTRING(vscnrsrc,"EXPRESSION",d->expression);
/* changed this to inherit existing mode if virtual field exist, else 
it will get the NORMAL MODE because of setup of Add Above and Add Below 
default to that value 
		x=0;
		FINDRSCSETINT(vscnrsrc,"MODES",x);
*/
		updateallrsrc(vscnrsrc);
		FreeDFvirtual(d); 
	} else {
		WARNINGDIALOG("WARNING DIALOG SCREEN","COULD NOT LOAD EZ-VIRTUAL!","The attemp to load the EZ-VIRTUAL FIELD has failed.  Make sure that it is defined.  If so call your installer.",NULL,NULL,TRUE,NULL);
	}
	quitlvir(r);
}
static void makemodlist()
{
	char *dirx=NULL;
	char *tmp=NULL;
	int x,selectedm=0;

	if(mod_list!=NULL) { freeapplib(mod_list); mod_list=NULL; }
	mod_list=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.VIR",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(mod_list,tmp);
	}
	if(mod_list->numlibs<1)
	{
		addAPPlib(mod_list,"Contains No Database Libraries");
	} else {
		SORTAPPlib(mod_list);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	for(x=0;x<mod_list->numlibs;++x)
	{
		if(!RDAstrcmp(modlstx->libs[selectedm],mod_list->libs[x])) break;
	}
	if(x>=mod_list->numlibs) 
	{
		x=0;
		virmodpointer=0;
	} else {
		virmodpointer=x;
	}
}
static void loadvirtual(RDArsrc *vscnrsrc)
{
	int x,free_ezviravl=FALSE;
	RDArsrc *lvirrsrc=NULL;

	lvirrsrc=RDArsrcNEW(module,"LOAD EZ VIRTUAL FIELD SCREEN");
	if(mod_list==NULL) makemodlist();
	addlstrsrc(lvirrsrc,"MODULE LIST",&virmodpointer,TRUE,makeezvirlisttest,mod_list->numlibs,&mod_list->libs,NULL);
	x=0;
	if(ezviravl==NULL) /* This has to be initialized to deal with addlstrsrc */ 
	{
		ezviravl=APPlibNEW();
		free_ezviravl=TRUE;
	}
	addlstrsrc(lvirrsrc,"VIRTUALS AVAILABLE",&x,TRUE,changevdesc,ezviravl->numlibs,&ezviravl->libs,NULL);
	
/* free_ezviravl is setting up makeezvirlist when it was created just to setup addlstrsrc otherwise it doesn't create the list each time  */
	if(free_ezviravl) { freeapplib(ezviravl);ezviravl=NULL; }
	makeezvirlist(lvirrsrc);
	addsctrsrc(lvirrsrc,"DESCRIPTION",0,NULL,FALSE);
	changevdesc(lvirrsrc);
	addrfkwrsrc(lvirrsrc,"SELECT",TRUE,oklvir,vscnrsrc);
	addrfkwrsrc(lvirrsrc,"QUIT",TRUE,quitlvir,NULL);
	addbtnrsrc(lvirrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(lvirrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(lvirrsrc,TRUE,quitlvir,NULL,FALSE);
}
/*
static void getvirtuals()
{
	int x,len=0;
	RDAvirtual *v;
	char *tmp=NULL;

	if(viravl!=NULL) freeapplib(viravl);
	viravl=APPlibNEW();
	for(x=0,v=holdvir;x<numvir;++x,++v)
	{
		len=RDAstrlen(v->name)+RDAstrlen(v->expression)+3;
		if(tmp!=NULL) 
		{
			tmp=Rrealloc(tmp,len);
		} else {
			tmp=Rmalloc(len);
		}
		memset(tmp,0,len);
		sprintf(tmp,"%s %s",(v->name!=NULL ? v->name:""),(v->expression!=NULL ? v->expression:""));
		addAPPlib(viravl,tmp);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(viravl->numlibs<1)
	{
		addAPPlib(viravl,"No Virtual Fields Defined");
	}
}
*/
static void savevscn(RDArsrc *vscnrsrc,RDArsrc *virrsrc)
{
	int value=0,hold=0;
	RDAvirtual *v;

	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&value)) return;
	if(holdvir==NULL) 
	{
		holdvir=Rmalloc(sizeof(RDAvirtual));
		numvir=1;
	} 
	v=holdvir+value;
	readallwidgets(vscnrsrc);
	FINDRSCGETSTRING(vscnrsrc,"NAME",&v->name);
	FINDRSCGETINT(vscnrsrc,"COMPUTE WHEN",&v->when);
	FINDRSCGETINT(vscnrsrc,"FIELD TYPES",&v->type);
	FINDRSCGETINT(vscnrsrc,"LENGTH",&hold);
	FINDRSCGETCHAR(vscnrsrc,"GATHER VIRTUAL",&v->computed);
	v->len=(unsigned)hold;
	FINDRSCGETSTRING(vscnrsrc,"EXPRESSION",&v->expression);
	FINDRSCGETINT(vscnrsrc,"MODES",&hold);
	v->range=(char)hold;
	makevirtuallist();
/*
	getvirtuals();
*/
	if(!FINDRSCLISTAPPlib(virrsrc,"VIRTUAL FIELDS",value,viravl))
		updatersrc(virrsrc,"VIRTUAL FIELDS");
	if(COMPARE_RSRCS(vscnrsrc,vscndefaults,2))
	{
		changedvirtuals=TRUE;
	}
	quitvscn(vscnrsrc,virrsrc);
}
static void quitvscntest(RDArsrc *vscnrsrc,RDArsrc *virrsrc)
{
	readallwidgets(vscnrsrc);
	if(COMPARE_RSRCS(vscnrsrc,vscndefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this VIRTUAL FIELD!\nDo you want to Save these changes?",savevscn,quitvscn,FALSE,2,vscnrsrc,virrsrc,NULL);
	} else {
		quitvscn(vscnrsrc,virrsrc);
	}
}
static void savevscnabove(RDArsrc *vscnrsrc,RDArsrc *virrsrc)
{
	int x;

	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&x)) return;
	makespacevirtualfield(x);
	savevscn(vscnrsrc,virrsrc);
}
static void savevscnbelow(RDArsrc *vscnrsrc,RDArsrc *virrsrc)
{
	int x;

	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&x)) return;
	makespacevirtualfield(++x);
	FINDRSCSETINT(virrsrc,"VIRTUAL FIELDS",x);
	savevscn(vscnrsrc,virrsrc);
}
static void quitvscnabovetest(RDArsrc *vscnrsrc,RDArsrc *virrsrc)
{
	readallwidgets(vscnrsrc);
	if(COMPARE_RSRCS(vscnrsrc,vscndefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this VIRTUAL FIELD!\nDo you want to Save these changes?",savevscnabove,quitvscn,FALSE,2,vscnrsrc,virrsrc,NULL);
	} else {
		quitvscn(vscnrsrc,virrsrc);
	}
}
static void quitvscnbelowtest(RDArsrc *vscnrsrc,RDArsrc *virrsrc)
{
	readallwidgets(vscnrsrc);
	if(COMPARE_RSRCS(vscnrsrc,vscndefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this VIRTUAL FIELD!\nDo you want to Save these changes?",savevscnbelow,quitvscn,FALSE,2,vscnrsrc,virrsrc,NULL);
	} else {
		quitvscn(vscnrsrc,virrsrc);
	}
}
static void virtualscreen(RDArsrc *virrsrc,int num,char *name,int when,
	int type,int len,char *expression,char range,char computed,
	void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	int x,rng_val=0;
	RDArsrc *vscnrsrc=NULL;

	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x) addAPPlib(ftypes,standardfieldtypes[x]);
	vscnrsrc=RDArsrcNEW(module,"MAINTAIN VIRTUAL FIELD");
	addstdrsrc(vscnrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addstdrsrc(vscnrsrc,"GATHER VIRTUAL",BOOLNS,1,&computed,TRUE);
	addstdrsrc(vscnrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),TRUE);
	if(methods!=NULL) freeapplib(methods);
	methods=APPlibNEW();
	addAPPlib(methods,"NORMAL COMPUTATION");
	addAPPlib(methods,"RANGE ON COMPUTATION");
	addAPPlib(methods,"DEFAULT COMPUTATION");
	addAPPlib(methods,"ENVIRONMENT COMPUTATION");
	addAPPlib(methods,"RANGE / DEFAULT COMBO");
	rng_val=range;
	addlstrsrc(vscnrsrc,"MODES",&rng_val,TRUE,NULL,methods->numlibs,
		&methods->libs,NULL);
	if(cmpwhen!=NULL) freeapplib(cmpwhen);
	cmpwhen=APPlibNEW();
	addAPPlib(cmpwhen,"Select Time");
	addAPPlib(cmpwhen,"Output Time");
	addlstrsrc(vscnrsrc,"COMPUTE WHEN",&when,TRUE,NULL,cmpwhen->numlibs,
		&cmpwhen->libs,NULL);
	addlstrsrc(vscnrsrc,"FIELD TYPES",&type,TRUE,NULL,ftypes->numlibs,
		&ftypes->libs,NULL);
	addstdrsrc(vscnrsrc,"LENGTH",LONGV,5,&len,TRUE);
	addsctrsrc(vscnrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),TRUE);
	addbtnrsrc(vscnrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(vscnrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(vscnrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(vscnrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(vscnrsrc,"LOAD VIRTUAL",TRUE,loadvirtual,NULL);
	addrfcbrsrc(vscnrsrc,"SAVE",TRUE,savefunction,virrsrc);
	addrfcbrsrc(vscnrsrc,"QUIT",TRUE,quitfunctiontest,virrsrc);
	addbtnrsrc(vscnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(vscnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	vscndefaults=GetDefaults(vscnrsrc);
	APPmakescrn(vscnrsrc,TRUE,quitvscn,virrsrc,FALSE);
}
static void viraddabove(RDArsrc *virrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&selected)) return;
	if(viravl->numlibs==1 && !RDAstrcmp(viravl->libs[0],"No Virtual Fields Defined")) return;
	virtualscreen(virrsrc,++selected,NULL,0,0,0,NULL,FALSE,FALSE,savevscnabove,quitvscnabovetest);
}
static void viraddbelow(RDArsrc *virrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&selected)) return;
	if(viravl->numlibs==1 && !RDAstrcmp(viravl->libs[0],"No Virtual Fields Defined")) return;
	selected+=2;
	virtualscreen(virrsrc,selected,NULL,0,0,0,NULL,FALSE,FALSE,savevscnbelow,quitvscnbelowtest);
}
static void copyvirtual(RDArsrc *virrsrc)
{
	int selected=0,copyfrom=0;
	RDAvirtual *v;

	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&selected)) return;
	copyfrom=selected;
	selected+=2;
	v=holdvir+copyfrom;
	if(v!=NULL)
	{
		virtualscreen(virrsrc,selected,v->name,v->when,v->type,v->len,v->expression,v->range,v->computed,savevscnbelow,quitvscnbelowtest);
	} else {
		virtualscreen(virrsrc,selected,NULL,0,0,0,NULL,FALSE,FALSE,savevscnbelow,quitvscnbelowtest);
	}
}
static void virdelete(RDArsrc *virrsrc)
{
	short x,y;
	RDAvirtual *temp,*temp1,*new_fields;
	int value;

	if(numvir<1) return;
	readallwidgets(virrsrc);
	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&value)) return;
	if(numvir>1)
	{
		new_fields=Rmalloc((numvir-1)*sizeof(RDAvirtual));
		y=0;
		for(x=0,temp=holdvir;x<numvir;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				temp1->type=temp->type;
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				temp1->len=temp->len;
				temp1->when=temp->when;
				temp1->range=temp->range;
				temp1->computed=temp->computed;
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
				if(temp->expression!=NULL) Rfree(temp->expression);
			}
		}
		Rfree(holdvir);
		--numvir;
		holdvir=Rmalloc(numvir*sizeof(RDAvirtual));
		for(x=0,temp1=holdvir,temp=new_fields;x<numvir;++x,
			++temp,++temp1)
		{
			temp1->type=temp->type;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->len=temp->len;
			temp1->when=temp->when;
			temp1->range=temp->range;
			temp1->computed=temp->computed;
		}
		Rfree(new_fields);
	} else {
		temp=holdvir;
		if(temp!=NULL)
		{
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->expression!=NULL) Rfree(temp->expression);
			Rfree(holdvir);
			numvir=0;
		}
	}
	makevirtuallist();
/*
	getvirtuals();
*/
	if(value>=viravl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(virrsrc,"VIRTUAL FIELDS",value,viravl))
		updatersrc(virrsrc,"VIRTUAL FIELDS");
	changedvirtuals=TRUE;
}
static void viredit(RDArsrc *virrsrc)
{
	int selected=0;
	RDAvirtual *v;

	if(FINDRSCGETINT(virrsrc,"VIRTUAL FIELDS",&selected)) return;
	v=holdvir+selected;
	if(v!=NULL)
	{
		virtualscreen(virrsrc,++selected,v->name,v->when,v->type,v->len,v->expression,v->range,v->computed,savevscn,quitvscntest);
	} else {
		virtualscreen(virrsrc,++selected,NULL,0,0,0,NULL,FALSE,FALSE,savevscn,quitvscntest);
	}
}
static void quitvir(RDArsrc *virrsrc)
{
	int x;
	RDAvirtual *v1=NULL;

	if(virrsrc!=NULL)
	{
		killwindow(virrsrc);
		free_rsrc(virrsrc);
	}
	if(holdvir!=NULL)
	{
		for(x=0,v1=holdvir;x<numvir;++x,++v1)
		{
			if(v1->name!=NULL) Rfree(v1->name);
			if(v1->expression!=NULL) Rfree(v1->expression);
		}
		Rfree(holdvir);
		numvir=0;
	}
	if(viravl!=NULL) freeapplib(viravl);
	if(ezviravl!=NULL) freeapplib(ezviravl); 
	if(mod_list!=NULL) freeapplib(mod_list);
	if(changedvirtuals) changedreport=TRUE;
	if(virdefaults!=NULL) FreeRDAdefault(virdefaults);
}
static void quitvirclr(RDArsrc *virrsrc)
{
	changedvirtuals=FALSE;
	quitvir(virrsrc);
}
static void savevir(RDArsrc *virrsrc)
{
	RDAvirtual *v,*v1;
	int x;

	readallwidgets(virrsrc);
	if(report->virflds!=NULL)
	{
		for(x=0,v=report->virflds;x<report->numvirtuals;++x,++v)
		{
			if(v->name!=NULL) Rfree(v->name);
			if(v->expression!=NULL) Rfree(v->expression);
		}
		Rfree(report->virflds);
		report->numvirtuals=0;
	}
	if(holdvir!=NULL)
	{
		report->numvirtuals=numvir;
		report->virflds=Rmalloc(numvir*sizeof(RDAvirtual));
		for(x=0,v=report->virflds,v1=holdvir;x<numvir;++x,++v,++v1)
		{
			if(v1->name!=NULL) 
			{
				v->name=stralloc(v1->name);
				Rfree(v1->name);
			} else v->name=NULL;
			v->type=v1->type;
			v->len=v1->len;
			v->when=v1->when;
			if(v1->expression!=NULL)
			{
				v->expression=stralloc(v1->expression);
				Rfree(v1->expression);
			} else v->expression=NULL;
			v->range=v1->range;
			v->computed=v1->computed;
		}
		Rfree(holdvir);
		numvir=0;
	}
	if(COMPARE_RSRCS(virrsrc,virdefaults,0) || changedvirtuals)
	{
		changedvirtuals=TRUE;
	}
	quitvir(virrsrc);
}
static void quitvirtest(RDArsrc *virrsrc)
{
	readallwidgets(virrsrc);
	if(COMPARE_RSRCS(virrsrc,virdefaults,0) || changedvirtuals)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the VIRTUAL FIELDS!\nDo you want to Save these changes?",savevir,quitvirclr,FALSE,virrsrc);
	} else {
		quitvir(virrsrc);
	}
}
void virflds(RDArsrc *rptrsrc)
{
	int x;
	RDAvirtual *v,*v1;
	RDArsrc *virrsrc=NULL;

	numvir=report->numvirtuals;
	if(report->virflds!=NULL)
	{
		holdvir=Rmalloc(numvir*sizeof(RDAvirtual));
		for(x=0,v1=holdvir,v=report->virflds;x<numvir;++x,++v1,++v)
		{
			if(v->name!=NULL)
			{
				v1->name=stralloc(v->name);
			} else v1->name=NULL;
			v1->type=v->type;
			v1->when=v->when;
			v1->len=v->len;
			if(v->expression!=NULL) 
			{
				v1->expression=stralloc(v->expression);
			} else v1->expression=NULL;
			v1->range=v->range;
			v1->computed=v->computed;
		}
	} else {
		holdvir=NULL;
		numvir=0;
	}
	virrsrc=RDArsrcNEW(module,"DEFINE VIRTUAL FIELDS");
	x=0;
/*
	getvirtuals();
*/
	if(ezviravl!=NULL) { freeapplib(ezviravl); ezviravl=NULL; }
	if(mod_list!=NULL) { freeapplib(mod_list); mod_list=NULL; }
	definevirlst=RDArsrcNEW(module,"DEFINE VIRTUAL FIELDS DEFINE LIST");
	mk_define_virtual_list(definevirlst);
	makevirtuallist();
	FINDRSCSETFUNC(definevirlst,"SELECT",savevirlstcb,virrsrc);
	
	addlstrsrc(virrsrc,"VIRTUAL FIELDS",&x,TRUE,NULL,viravl->numlibs,
		&viravl->libs,NULL);
	addbtnrsrc(virrsrc,"DEFINE LIST",TRUE,definevirlstfunc,definevirlst);
	savevirlst(definevirlst,virrsrc);
	addbtnrsrc(virrsrc,"ADD ABOVE",TRUE,viraddabove,NULL);
	addbtnrsrc(virrsrc,"ADD BELOW",TRUE,viraddbelow,NULL);
	addbtnrsrc(virrsrc,"COPY VIRTUAL",TRUE,copyvirtual,NULL);
	addbtnrsrc(virrsrc,"DELETE",TRUE,virdelete,NULL);
	addbtnrsrc(virrsrc,"SELECT",TRUE,viredit,NULL);
	addrfcbrsrc(virrsrc,"SAVE",TRUE,savevir,NULL);
	addrfcbrsrc(virrsrc,"QUIT",TRUE,quitvirtest,NULL);
	addbtnrsrc(virrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(virrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedvirtuals=FALSE;
	virdefaults=GetDefaults(virrsrc);
	APPmakescrn(virrsrc,TRUE,quitvir,NULL,FALSE);
}
