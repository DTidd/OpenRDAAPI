/* dfvir.c - Define Virtual Fields */
#ifndef WIN32
#define __NAM__ "dfvir.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfvir.exe"
#endif
#include <app.hpp>





#include <rptgen.hpp>
#include <ldval.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="RPTGEN";
RDArsrc *mainrsrc=NULL,*virtrsrc=NULL,*incrsrc=NULL;
APPlib *modsavl=NULL,*viravl=NULL,*inclst=NULL;
APPlib *modsavl2=NULL;
APPlib *wtypes=NULL,*filesavl=NULL,*ftypes=NULL,*vtypes=NULL;
DFvirtual *DFV=NULL;
EZfield *holdfields=NULL;
int numfields=0;
static int getvirtmodules(RDArsrc *,char *);
static void getvirtmodulescb(RDArsrc *);
static int dovirtfldlist(RDArsrc *,char *,char *);
static void dovirtfldlistcb(RDArsrc *);
static int dofilelist(RDArsrc *,char *,char *);
static void dofilelistcb(RDArsrc *);
static void savevirtual(RDArsrc *,RDArsrc *);
static void quitvirtualtest(RDArsrc *,RDArsrc *);
static void setvaluesx(RDArsrc *,char *,char *),setvaluesxcb(RDArsrc *);
static void quit_include(RDArsrc *,RDArsrc *),quit_includetest(RDArsrc *,RDArsrc *);
static void save_include(RDArsrc *,RDArsrc *);
static void quitvirtual(RDArsrc *,RDArsrc *);
static RDAdefault *incdefaults=NULL,*virtdefaults=NULL;
static void print_virtualscb(RDArsrc *);
static void print_virtuals(RDArsrc *,RDArsrc *);
static char changedvalues=FALSE;

static void addholdvalues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	EZfield *ezf=NULL;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvalues Module [%s] File [%s] ",modulex,filex);
	}
	if(!GETBIN(modulex,filex,&engine,&nofields,&fields,
		&nokeys,&keys)) 
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
			Rfree(keys);
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(holdfields!=NULL)
					{
						holdfields=Rrealloc(holdfields,(numfields+1)*
							sizeof(EZfield));
					} else {
						holdfields=Rmalloc(sizeof(EZfield));
						numfields=0;
					}
					ezf=holdfields+numfields;
					if(f->name!=NULL)
					{
						ezf->name=stralloc(f->name);
					} else ezf->name=NULL;	
					ezf->type=f->type;
					ezf->len=f->len;
					++numfields;
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+
							RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					} else 	{
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modulex,
						filex,f->name);
					ezf->wholename=stralloc(temp);
					addAPPlib(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
static APPlib *makefieldvallist(RDArsrc *virtrsrc)
{
	APPlib *tmp=NULL,*virflist=NULL;
	char *holdmodule=NULL,*name=NULL;
	char *tmpx=NULL,addvf=FALSE,*libx=NULL,deleteflag=FALSE,active=FALSE;
	int x,y,z;
	EZfield *f=NULL;
	DFincvir *i=NULL,*a=NULL;
	DFvirtual *d=NULL;
	short fileno=0,ef=0;

	if(holdfields!=NULL)
	{
		for(x=0,f=holdfields;x<numfields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
			if(f->wholename!=NULL) Rfree(f->wholename);
		}
		Rfree(holdfields);
		numfields=0;
	}
	tmp=APPlibNEW();
	if(DFV->incvir!=NULL)
	{
		for(x=0,i=DFV->incvir;x<DFV->num;++x,++i)
		{
			addholdvalues(tmp,i->module,i->file);
		}
		FINDRSCGETSTRING(virtrsrc,"MODULE",&holdmodule);
		FINDRSCGETSTRING(virtrsrc,"NAME",&name);
		if(virflist!=NULL) freeapplib(virflist);
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(holdmodule)+10);
		tmpx=adddashes(holdmodule);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
		if(tmpx!=NULL) Rfree(tmpx);
		virflist=getlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(virflist!=NULL)
		{
			for(y=0;y<virflist->numlibs;++y)
			{
				if(RDAstrcmp(virflist->libs[y],name))
				{
					d=getDFvirtual(holdmodule,virflist->libs[y]);
					if(d!=NULL)
					{
						addvf=TRUE;
						if(d->incvir!=NULL)
						{
							for(z=0,a=d->incvir;z<d->num;++z,++a)
							{
								for(x=0,i=DFV->incvir;x<DFV->num;++x,++i)
								{
									if(!RDAstrcmp(i->module,a->module) && 
										!RDAstrcmp(i->file,a->file)) break;
								}
								if(x>=DFV->num) 
								{
									addvf=FALSE;
									break;
								}
							}
						}
						if(addvf)
						{
							libx=Rmalloc(RDAstrlen(d->name)+3);
							sprintf(libx,"[%s]",d->name);
							addAPPlibNoDuplicates(tmp,libx);
							if(libx!=NULL) Rfree(libx);
						}
						FreeDFvirtual(d);
					}
				}
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(!RDAstrcmp(holdmodule,"PAYROLL") && DFV!=NULL)
	{
		for(x=0,i=DFV->incvir;x<DFV->num;++x,++i)
		{
			if(!RDAstrcmp(i->module,holdmodule) && 
				!RDAstrcmp(i->file,"PAYDDMS")) break;
		}
		if(x<DFV->num)
		{
			fileno=OPNNRDsec(holdmodule,"PAYDDMS",TRUE,FALSE,FALSE);
			if(fileno!=(-1))
			{
				ef=FRSNRDsec(fileno,1);
				while(!ef)
				{
					FINDFLDGETCHAR(fileno,"DELETEFLAG",
						&deleteflag);
					FINDFLDGETCHAR(fileno,"ACTIVE",&active);
					if(!deleteflag && active)
					{
						FINDFLDGETSTRING(fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",&tmpx);
						libx=Rmalloc(RDAstrlen(tmpx)+16);
						sprintf(libx,"[%s][CHECK GROSS]",tmpx);
						addAPPlibNoDuplicates(tmp,libx);
						if(libx!=NULL) Rfree(libx);
						libx=Rmalloc(RDAstrlen(tmpx)+11);
						sprintf(libx,"[%s][AMOUNT]",tmpx);
						addAPPlibNoDuplicates(tmp,libx);
						if(libx!=NULL) Rfree(libx);
						libx=Rmalloc(RDAstrlen(tmpx)+9);
						sprintf(libx,"[%s][BASE]",tmpx);
						addAPPlibNoDuplicates(tmp,libx);
						if(libx!=NULL) Rfree(libx);
						libx=Rmalloc(RDAstrlen(tmpx)+22);
						sprintf(libx,"[%s][COMPUTATION LEVEL]",tmpx);
						addAPPlibNoDuplicates(tmp,libx);
						if(libx!=NULL) Rfree(libx);
						libx=Rmalloc(RDAstrlen(tmpx)+16);
						sprintf(libx,"[%s][ANNUAL BASE]",tmpx);
						addAPPlibNoDuplicates(tmp,libx);
						if(libx!=NULL) Rfree(libx);
						libx=Rmalloc(RDAstrlen(tmpx)+16);
						sprintf(libx,"[%s][AFFECT BASE]",tmpx);
						addAPPlibNoDuplicates(tmp,libx);
						if(libx!=NULL) Rfree(libx);
						libx=Rmalloc(RDAstrlen(tmpx)+23);
						sprintf(libx,"[%s][ANNUAL AFFECT BASE]",tmpx);
						addAPPlibNoDuplicates(tmp,libx);
						if(libx!=NULL) Rfree(libx);
					}
					ef=NXTNRDsec(fileno,1);
				}
				CLSNRD(fileno);
				if(tmpx!=NULL) Rfree(tmpx);
			}
		}
	}
	if(holdmodule!=NULL) Rfree(holdmodule);
	if(name!=NULL) Rfree(name);
	if(libx!=NULL) Rfree(libx);
	addAPPlibNoDuplicates(tmp,"[PAGE NO]");
	addAPPlibNoDuplicates(tmp,"[BODY COUNT]");
	addAPPlibNoDuplicates(tmp,"[BREAK LEVEL]");
	addAPPlibNoDuplicates(tmp,"[PRIMARY FILE NUMBER]");
	addAPPlibNoDuplicates(tmp,"[WRITE COUNT]");
	loadglobals(tmp);
	SORTAPPlib(tmp);
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x=0;
	LoadValue *load=NULL;

	ldvalrsrc=RDArsrcNEW(module,"LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist(r);
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
	}
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void makeincludelist()
{
	int l=0;
	short x;
	DFincvir *fields=NULL;
	char *s=NULL;

	if(inclst!=NULL) freeapplib(inclst);
	inclst=APPlibNEW();
	if(DFV->incvir!=NULL)
	{
		for(x=0,fields=DFV->incvir;x<DFV->num;++x,++fields)
		{
			l=(fields->module!=NULL ? RDAstrlen(fields->module):0)+
				(fields->file!=NULL ? RDAstrlen(fields->file):0)+24;
			s=Rmalloc(l);
			sprintf(s,"Module: [%s] Filename: [%s]",(fields->module!=NULL ? fields->module:""),(fields->file!=NULL ? fields->file:""));
			addAPPlib(inclst,s);
			Rfree(s);
		}
	}
	if(inclst->numlibs<1)
	{
		addAPPlib(inclst,"No Includes");
	}
}
static void makespaceinclist(int pos)
{
	short x,y;
	DFincvir *temp=NULL,*temp1=NULL,*new_fields=NULL;

	++DFV->num;
	new_fields=Rmalloc(DFV->num*sizeof(DFincvir));
	y=0;
	for(x=0,temp=DFV->incvir;x<(DFV->num-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->module=NULL;
			temp1->file=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->file!=NULL)
		{
			temp1->file=stralloc(temp->file);
			Rfree(temp->file);
		} else temp1->file=NULL;
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		++y;
	}
	Rfree(DFV->incvir);
	DFV->incvir=Rmalloc(DFV->num*sizeof(DFincvir));
	for(x=0,temp1=DFV->incvir,temp=new_fields;x<DFV->num;++x,
		++temp,++temp1)
	{
		if(temp->file!=NULL)
		{
			temp1->file=stralloc(temp->file);
			Rfree(temp->file);
		} else temp1->file=NULL;
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
	}
	Rfree(new_fields);
}
static void quit_include(RDArsrc *incrsrc,RDArsrc *virtrsrc)
{
	if(filesavl!=NULL) freeapplib(filesavl);
	if(modsavl2!=NULL) freeapplib(modsavl2);
	if(incrsrc!=NULL) 
	{
		killwindow(incrsrc);
		free_rsrc(incrsrc);
	}
	if(incdefaults!=NULL) FreeRDAdefault(incdefaults);
}
static void quit_includetest(RDArsrc *incrsrc,RDArsrc *virtrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_include,quit_include,FALSE,2,incrsrc,virtrsrc,NULL);
	} else {
		quit_include(incrsrc,virtrsrc);
	}
}
static void save_include(RDArsrc *incrsrc,RDArsrc *virtrsrc)
{
	int value=0,selected=0,selectedf=0;
	DFincvir *i=NULL;

	if(FINDRSCGETINT(virtrsrc,"ASSOCIATED FILES",&value)) return;
	i=DFV->incvir+value;
	readallwidgets(incrsrc);
	FINDRSCGETINT(incrsrc,"MODULE LIST",&selected);
	i->module=stralloc(modsavl2->libs[selected]);
	FINDRSCGETINT(incrsrc,"FILE LIST",&selectedf);
	i->file=stralloc(filesavl->libs[selectedf]);
	makeincludelist();
	if(!FINDRSCLISTAPPlib(virtrsrc,"ASSOCIATED FILES",value,inclst))
		updatersrc(virtrsrc,"ASSOCIATED FILES");
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		changedvalues=TRUE;
	}
	quit_include(incrsrc,virtrsrc);
}
static void save_include_above(RDArsrc *incrsrc,RDArsrc *virtrsrc)
{
	int x;

	if(FINDRSCGETINT(virtrsrc,"ASSOCIATED FILES",&x)) return;
	makespaceinclist(x);
	save_include(incrsrc,virtrsrc);
}
static void save_include_below(RDArsrc *incrsrc,RDArsrc *virtrsrc)
{
	int x;

	if(FINDRSCGETINT(virtrsrc,"ASSOCIATED FILES",&x)) return;
	makespaceinclist(++x);
	FINDRSCSETINT(virtrsrc,"ASSOCIATED FILES",x);
	updatersrc(virtrsrc,"ASSOCIATED FILES");
	save_include(incrsrc,virtrsrc);
}
static void quit_include_abovetest(RDArsrc *incrsrc,RDArsrc *virtrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_include_above,quit_include,FALSE,2,incrsrc,virtrsrc,NULL);
	} else {
		quit_include(incrsrc,virtrsrc);
	}
}
static void quit_include_belowtest(RDArsrc *incrsrc,RDArsrc *virtrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_include_below,quit_include,FALSE,2,incrsrc,virtrsrc,NULL);
	} else {
		quit_include(incrsrc,virtrsrc);
	}
}
static int dofilelist(RDArsrc *incrsrc,char *modname,char *filename)
{
	int x=0,selectedf=0;
	char *libx=NULL;

	if(filesavl!=NULL) freeapplib(filesavl); 
	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modname);
#endif
	} else {
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl2->libs[0])+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modsavl2->libs[0]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modsavl2->libs[0]);
#endif
	}
	filesavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(filesavl==NULL)
	{
		filesavl=APPlibNEW();
		addAPPlib(filesavl,"No Database Files");
	}
	if(filename!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(filesavl,filename))==(-1))
		{
			selectedf=0;
		} else {
			selectedf=x;
		}
	} else {
		selectedf=0;
	}
	if(incrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(incrsrc,"FILE LIST",selectedf,filesavl))
		{
			updatersrc(incrsrc,"FILE LIST");
		}
	}
	return(selectedf);
}
static void dofilelistcb(RDArsrc *incrsrc)
{
	int selected=0,selectedf=0;
	char *temp=NULL,*temp2=NULL;

	if(FINDRSCGETINT(incrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(incrsrc,"FILE LIST",&selectedf)) return; 
	temp=stralloc(modsavl2->libs[selected]);
	temp2=stralloc(filesavl->libs[selectedf]);
	dofilelist(incrsrc,temp,temp2);
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
}
static int getfilemodules2(RDArsrc *incrsrc,char *modname)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	
	if(modsavl2!=NULL) freeapplib(modsavl2); 
	modsavl2=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modsavl2,tmp);
	}
	if(modsavl2->numlibs<1)
	{
		addAPPlib(modsavl2,"No Database Files");
		selectedm=0;
	} else {
		SORTAPPlib(modsavl2);
		if(selectedm>=modsavl2->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(modsavl2,modname))==(-1))
		{
			selectedm=0;
		} else {
			selectedm=x;
		}
	} else {
		selectedm=0;
	}
	if(incrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(incrsrc,"MODULE LIST",selectedm,modsavl2))
		{
			updatersrc(incrsrc,"MODULE LIST");
		}
	}
	return(selectedm);
}
static void includescreen(RDArsrc *virtrsrc,char *modx,char *filx,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0,y=0;
	RDArsrc *incrsrc=NULL;

	incrsrc=RDArsrcNEW(module,"MAINTAIN EZ VIRTUAL FIELD ASSOCIATED FILES");
	x=getfilemodules2(virtrsrc,modx);
	y=dofilelist(virtrsrc,modx,filx);
	addlstrsrc(incrsrc,"MODULE LIST",&x,TRUE,dofilelistcb,
		modsavl2->numlibs,&modsavl2->libs,NULL);
	addlstrsrc(incrsrc,"FILE LIST",&y,TRUE,NULL,
		filesavl->numlibs,&filesavl->libs,NULL);
	addrfcbrsrc(incrsrc,"SAVE",TRUE,savefunc,virtrsrc);
	addrfcbrsrc(incrsrc,"QUIT",TRUE,quitfunctest,virtrsrc);
	addbtnrsrc(incrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(incrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	incdefaults=GetDefaults(incrsrc);
	APPmakescrn(incrsrc,TRUE,quit_include,virtrsrc,FALSE);
}
static void add_include_above(RDArsrc *virtrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(virtrsrc,"ASSOCIATED FILES",&selected)) return;
	readwidget(virtrsrc,"MODULE");
	if(FINDRSCGETSTRING(virtrsrc,"MODULE",&modx)) return;
	if(inclst->numlibs==1 && !RDAstrcmp(inclst->libs[0],"No Includes")) return;
	includescreen(virtrsrc,modx,NULL,save_include_above,quit_include_abovetest);
	if(modx!=NULL) Rfree(modx);
}
static void add_include_below(RDArsrc *virtrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(virtrsrc,"ASSOCIATED FILES",&selected)) return;
	readwidget(virtrsrc,"MODULE");
	if(FINDRSCGETSTRING(virtrsrc,"MODULE",&modx)) return;
	if(inclst->numlibs==1 && !RDAstrcmp(inclst->libs[0],"No Includes")) return;
	includescreen(virtrsrc,modx,NULL,save_include_below,quit_include_belowtest);
	if(modx!=NULL) Rfree(modx);
}
static void edit_include(RDArsrc *virtrsrc)
{
	int x;
	DFincvir *i=NULL;
	char *modx=NULL;

	if(FINDRSCGETINT(virtrsrc,"ASSOCIATED FILES",&x)) return;
	if(DFV->incvir==NULL)
	{
		DFV->incvir=Rmalloc(sizeof(DFincvir));
		i=DFV->incvir;
		i->module=NULL;
		i->file=NULL;
		DFV->num=1;
	}
	i=DFV->incvir+x;
	if(i->module==NULL)
	{
		readwidget(virtrsrc,"MODULE");
		if(FINDRSCGETSTRING(virtrsrc,"MODULE",&modx)) return;
		i->module=stralloc(modx);
		if(modx!=NULL) Rfree(modx);
	}
	includescreen(virtrsrc,i->module,i->file,save_include,quit_includetest);
}
static void delete_include(RDArsrc *virtrsrc)
{
	short x,y;
	DFincvir *temp=NULL,*temp1=NULL,*new_fields=NULL;
	int value=0;

	readallwidgets(virtrsrc);
	if(FINDRSCGETINT(virtrsrc,"ASSOCIATED FILES",&value)) return;
/*
	if(DFV->num<2) return;
*/
	if(inclst->numlibs==1 && !RDAstrcmp(inclst->libs[0],"No Includes")) return;
	if(DFV->num>1)
	{
		new_fields=Rmalloc((DFV->num-1)*sizeof(DFincvir));
		y=0;
		for(x=0,temp=DFV->incvir;x<DFV->num;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->module!=NULL)
				{
					temp1->module=stralloc(temp->module);
					Rfree(temp->module);
				} else temp1->module=NULL;
				if(temp->file!=NULL)
				{
					temp1->file=stralloc(temp->file);
					Rfree(temp->file);
				} else temp1->file=NULL;
				++y;
			} else {
				if(temp->module!=NULL) Rfree(temp->module);
					if(temp->file!=NULL) Rfree(temp->file);
			}
		}
		Rfree(DFV->incvir);
		--DFV->num;
		DFV->incvir=Rmalloc(DFV->num*sizeof(DFincvir));
		for(x=0,temp1=DFV->incvir,temp=new_fields;x<DFV->num;++x,
			++temp,++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
				Rfree(temp->module);
			} else temp1->module=NULL;
			if(temp->file!=NULL)
			{
				temp1->file=stralloc(temp->file);
				Rfree(temp->file);
			} else temp1->file=NULL;
		}
		Rfree(new_fields);
	} else {
		if(DFV->num==1)
		{
			temp=DFV->incvir;
			if(temp->module!=NULL)
			{
				Rfree(temp->module);
			}
			if(temp->file!=NULL)
			{
				Rfree(temp->file);
			}
			Rfree(DFV->incvir);
			--DFV->num;
		}
	} 
	makeincludelist();
	if(value>=inclst->numlibs) value=0; 
	if(!FINDRSCLISTAPPlib(virtrsrc,"ASSOCIATED FILES",value,inclst))
		updatersrc(virtrsrc,"ASSOCIATED FILES");
	changedvalues=TRUE;
}
static void quitvirtual(RDArsrc *virtrsrc,RDArsrc *mainrsrc)
{
	if(vtypes!=NULL) freeapplib(vtypes);
	if(wtypes!=NULL) freeapplib(wtypes);
	if(inclst!=NULL) freeapplib(inclst);
	if(ftypes!=NULL) freeapplib(ftypes);
	changedvalues=FALSE;
	if(virtrsrc!=NULL)
	{
		killwindow(virtrsrc);
		free_rsrc(virtrsrc);
	}
	if(virtdefaults!=NULL) FreeRDAdefault(virtdefaults);
}
static void quitvirtualtest(RDArsrc *virtrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(virtrsrc);
	if(COMPARE_RSRCS(virtrsrc,virtdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the VIRTUAL FIELD DEFINITION.\nDo you want to Save these changes?",savevirtual,quitvirtual,FALSE,2,virtrsrc,mainrsrc,NULL);
	} else {
		quitvirtual(virtrsrc,mainrsrc);
	}
}
static void savevirtual(RDArsrc *virtrsrc,RDArsrc *mainrsrc)
{
	char *modx=NULL,avlrsrc=FALSE;
	int ftypex=0;
	
	readallwidgets(virtrsrc);
	FINDRSCGETSTRING(virtrsrc,"MODULE",&modx);
	FINDRSCGETSTRING(virtrsrc,"NAME",&DFV->name);
	if(modx!=NULL && DFV->name!=NULL)
	{
		FINDRSCGETSTRING(virtrsrc,"DESCRIPTION",&DFV->desc);
		FINDRSCGETINT(virtrsrc,"VIRTUAL TYPES",&DFV->vtype);
		FINDRSCGETSHORT(virtrsrc,"LENGTH",&DFV->length);
		FINDRSCGETINT(virtrsrc,"COMPUTE TIMES",&ftypex);
		DFV->when=(short)ftypex;
		FINDRSCGETSTRING(virtrsrc,"EXPRESSION",&DFV->expression);
		FINDRSCGETCHAR(virtrsrc,"AVAILABLE TO SCREENS",&avlrsrc);
		DFV->avail2rsrc=(short)avlrsrc;
		ftypex=0;
		FINDRSCGETINT(virtrsrc,"FIELD TYPES",&ftypex);
		DFV->type=(short)ftypex;
		if(writeDFvirtual(modx,DFV))
		{
			ERRORDIALOG("Cannot Write Virtual Field","Error attempting to write the virtual field.  Check the permissions on the Virtual Field Binary Library, and retry.  Call your installer.",NULL,FALSE);
		}
		FreeDFvirtual(DFV);
	}
	if(modx!=NULL) Rfree(modx);
	dovirtfldlistcb(mainrsrc);
	/*getvirtmodules();*/
	getvirtmodulescb(mainrsrc);
	quitvirtual(virtrsrc,mainrsrc);
}
static void virtualscreen(RDArsrc *mainrsrc,int vtype,char *modulx,char *name,
	short type,short length,short when,char *expression,char *desc,
	short availrsrc)
{
	int typex,x=0;
	char avail2rsrc=FALSE;
	RDArsrc *virtrsrc=NULL;

	virtrsrc=RDArsrcNEW(module,"MAINTAIN EZ VIRTUAL FIELD");
	if(vtypes!=NULL) freeapplib(vtypes);
	vtypes=APPlibNEW();
	addAPPlib(vtypes,"When ALL Files Present");
	addAPPlib(vtypes,"When One of Many Present");
	if(inclst!=NULL) freeapplib(inclst);
	inclst=APPlibNEW();
	addlstrsrc(virtrsrc,"ASSOCIATED FILES",&x,TRUE,NULL,inclst->numlibs,
		&inclst->libs,NULL);
	addbtnrsrc(virtrsrc,"ADD ABOVE",TRUE,add_include_above,NULL);
	addbtnrsrc(virtrsrc,"ADD BELOW",TRUE,add_include_below,NULL);
	addbtnrsrc(virtrsrc,"DELETE",TRUE,delete_include,NULL);
	addbtnrsrc(virtrsrc,"SELECT",TRUE,edit_include,NULL);
	makeincludelist();
	FINDRSCLISTAPPlib(virtrsrc,"ASSOCIATED FILES",x,inclst);
	addlstrsrc(virtrsrc,"VIRTUAL TYPES",&vtype,TRUE,NULL,
		vtypes->numlibs,&vtypes->libs,NULL);
	addstdrsrc(virtrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),
		TRUE);
	addstdrsrc(virtrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),
		TRUE);
	addsctrsrc(virtrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	addstdrsrc(virtrsrc,"LENGTH",SHORTV,3,&length,TRUE);
	avail2rsrc=availrsrc;
	addstdrsrc(virtrsrc,"AVAILABLE TO SCREENS",BOOLNS,1,&avail2rsrc,TRUE);
	typex=type;
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x) addAPPlib(ftypes,standardfieldtypes[x]);
	addlstrsrc(virtrsrc,"FIELD TYPES",&typex,TRUE,NULL,
		ftypes->numlibs,&ftypes->libs,NULL);
	if(wtypes!=NULL) freeapplib(wtypes);
	wtypes=APPlibNEW();
	addAPPlib(wtypes,"Select Time");
	addAPPlib(wtypes,"Output Time");
	typex=when;
	addlstrsrc(virtrsrc,"COMPUTE TIMES",&typex,TRUE,NULL,
		wtypes->numlibs,&wtypes->libs,NULL);
	addsctrsrc(virtrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),
		TRUE);
	addbtnrsrc(virtrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(virtrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(virtrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(virtrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addrfcbrsrc(virtrsrc,"SAVE",TRUE,savevirtual,mainrsrc);
	addrfcbrsrc(virtrsrc,"QUIT",TRUE,quitvirtualtest,mainrsrc);
	addbtnrsrc(virtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(virtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	virtdefaults=GetDefaults(virtrsrc);
	APPmakescrn(virtrsrc,TRUE,quitvirtual,mainrsrc,FALSE);
}
static void okvirtual(RDArsrc *mainrsrc)
{
	int selected=0,selectedv=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"VIRTUALS AVAILABLE",&selectedv)) return;
	if(DFV!=NULL) { FreeDFvirtual(DFV); DFV=NULL; }
	if(RDAstrcmp(viravl->libs[selectedv],"No Virtuals")) 
	{
		DFV=getDFvirtual(modsavl->libs[selected],viravl->libs[selectedv]);
		if(DFV==NULL)
		{
			ERRORDIALOG("Cannot Read Virtual Field","Error in permissions, the user cannot read the virtual field binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			return;
		}
	}
	if(DFV==NULL)
	{
		DFV=DFvirtualNEW(modsavl->libs[selected],NULL);
	}
	virtualscreen(mainrsrc,DFV->vtype,DFV->module,DFV->name,
		DFV->type,DFV->length,DFV->when,DFV->expression,DFV->desc,
		DFV->avail2rsrc);
}
static void newvirtual(RDArsrc *mainrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(DFV!=NULL) { FreeDFvirtual(DFV); DFV=NULL; }
	DFV=DFvirtualNEW(modsavl->libs[selected],NULL);
	virtualscreen(mainrsrc,DFV->vtype,DFV->module,DFV->name,DFV->type,
		DFV->length,DFV->when,DFV->expression,DFV->desc,
		DFV->avail2rsrc);
}
static void quitdfvir(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(modsavl!=NULL) freeapplib(modsavl);
	if(viravl!=NULL) freeapplib(viravl);
	if(incdefaults!=NULL) FreeRDAdefault(incdefaults);
	if(virtdefaults!=NULL) FreeRDAdefault(virtdefaults);
	ShutdownSubsystems();
}
static int getvirtmodules(RDArsrc *mainrsrc,char *modname)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	
	if(modsavl!=NULL) freeapplib(modsavl); 
	modsavl=APPlibNEW();
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
		addAPPlib(modsavl,tmp);
	}
	if(modsavl->numlibs<1)
	{
		addAPPlib(modsavl,"No Database Files");
		selectedm=0;
	} else {
		SORTAPPlib(modsavl);
		if(selectedm>=modsavl->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(modsavl,modname))==(-1))
		{
			selectedm=0;
		} else {
			selectedm=x;
		}
	} else {
		selectedm=0;
	}
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selectedm,modsavl))
		{
			updatersrc(mainrsrc,"MODULE LIST");
		}
	}
	return(selectedm);
}
static void getvirtmodulescb(RDArsrc *mainrsrc)
{
	int selectedm=0;
	char *temp=NULL;
	
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	temp=stralloc(modsavl->libs[selectedm]);
	getvirtmodules(mainrsrc,temp);
	dovirtfldlistcb(mainrsrc);
	if(temp!=NULL) Rfree(temp);
}
static void deletevir(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulename=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"VIRTUALS AVAILABLE",&s)) return;
	modulename=adddashes(modsavl->libs[m]);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,modulename);
#endif
	if(deletelibbin(libx,viravl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Virtual","Error attempting to delete virtual field.  Check the permissions on the Virtual Field Binary Library, and retry.  Call your installer.",NULL,FALSE);
	} else {
		getvirtmodulescb(mainrsrc);
	}
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
}
static void deletevirtest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(viravl->libs[s],"No Virtuals")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE VIRTUAL FIELD!","Are you sure you want to delete this virtual field?",deletevir,NULL,FALSE,mainrsrc);
}
static void setvaluesx(RDArsrc *mainrsrc,char *modname,char *fldname)
{
	char *temp=NULL,*temp2=NULL,*temp3=NULL;

	if(modname!=NULL)
	{
		if(fldname!=NULL)
		{
			if(!RDAstrcmp(fldname,"No Virtuals"))
			{
				temp=NULL;
			} else {
				temp=getvirtualdesc(modname,fldname);
			}
		} else {
			temp3=stralloc(viravl->libs[0]);
			if(!RDAstrcmp(temp3,"No Virtuals"))
			{
				temp=NULL;
			} else {
				temp=getvirtualdesc(modname,temp3);
			}
		}
	} else {
		temp2=stralloc(modsavl->libs[0]);
		temp3=stralloc(viravl->libs[0]);
		if(!RDAstrcmp(temp3,"No Virtuals"))
		{
			temp=NULL;
		} else {
			temp=getvirtualdesc(temp2,temp3);
		}
	}
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		{
			updatersrc(mainrsrc,"DESCRIPTION");
		}
	}
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
}
static void setvaluesxcb(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"VIRTUALS AVAILABLE",&selected)) return;
	setvaluesx(mainrsrc,modsavl->libs[selectedm],viravl->libs[selected]);
}
static int dovirtfldlist(RDArsrc *mainrsrc,char *modname,char *fldname)
{
	int x=0,selectedf=0;
	char *libx=NULL;

	if(viravl!=NULL) freeapplib(viravl); 
	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,modname);
#endif
	} else {
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl->libs[0])+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,modsavl->libs[0]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,modsavl->libs[0]);
#endif
	}
	viravl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(viravl==NULL)
	{
		viravl=APPlibNEW();
		addAPPlib(viravl,"No Virtuals");
	}
	if(fldname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(viravl,fldname))==(-1))
		{
			selectedf=0;
		} else {
			selectedf=x;
		}
	} else {
		selectedf=0;
	}
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"VIRTUALS AVAILABLE",selectedf,viravl))
		{
			updatersrc(mainrsrc,"VIRTUALS AVAILABLE");
		}
	}
	return(selectedf);
}
static void dovirtfldlistcb(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *temp=NULL,*temp2=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"VIRTUALS AVAILABLE",&selectedf)) return; 
	temp=stralloc(modsavl->libs[selected]);
	temp2=stralloc(viravl->libs[selectedf]);
	dovirtfldlist(mainrsrc,temp,temp2);
	setvaluesx(mainrsrc,temp,temp2);
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
}
static void print_virtuals(RDArsrc *prsrc,RDArsrc *mainrsrc)
{
	RDA_PFILE *fp=NULL;
	int x=0,y=0,selected=0;
	char *tmpx=NULL,*libx=NULL,*odevice=NULL,print_style=TRUE;
	DFvirtual *d=NULL;
	APPlib *virtnames=NULL;
	DFincvir *i=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&odevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(odevice==NULL)
	{
		ERRORDIALOG("Output Device Failure","There was no device specified.",NULL,FALSE);
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl->libs[selected])+10);
	tmpx=adddashes(modsavl->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
	virtnames=getlibnames(libx,FALSE);
	if(virtnames!=NULL)
	{
		fp=RDA_popen(odevice,"w");
		if(fp==NULL)
		{
			prterr("Error Couldn't open device.");
			if(virtnames!=NULL) freeapplib(virtnames);
			if(tmpx!=NULL) Rfree(tmpx);
			if(libx!=NULL) Rfree(libx);
			if(odevice!=NULL) Rfree(odevice);
			return;
		} 
		SelectPrintType(odevice,print_style,fp);
		if(odevice!=NULL) Rfree(odevice);
		RDA_fprintf(fp,"VIRTUAL FIELDS AVAILABLE\r\n");
		for(x=0;x<virtnames->numlibs;++x)
		{
			d=getDFvirtual(modsavl->libs[selected],virtnames->libs[x]);
			if(d!=NULL)
			{
				RDA_fprintf(fp,"Name: %s Type: [%d]\r\n",d->name,d->vtype);
				for(y=0,i=d->incvir;y<d->num;++y,++i)
				{
					RDA_fprintf(fp,"\tModule [%s] File [%s] Keyname [%s] Fileno [%d]\r\n",(i->module!=NULL ? i->module:""),(i->file!=NULL ? i->file:""),(i->keyname!=NULL ? i->keyname:""),i->fileno);
				}
				RDA_fprintf(fp,"\tWhen [%d] Type [%d] [%s] Length [%d] Available on Screens [%s]\r\n",d->when,d->type,standardfieldtypes[d->type],d->length,(d->avail2rsrc ? "Yes":"No"));
				RDA_fprintf(fp,"\tExpression [%s]\r\n",(d->expression!=NULL ?
					d->expression:""));
				RDA_fprintf(fp,"\tDescription [%s]\r\n\r\n",(d->desc!=NULL ? d->desc:""));
				FreeDFvirtual(d);
				d=NULL;
			}
		}
		RDA_pclose(fp);
		if(virtnames!=NULL) freeapplib(virtnames);
	} else {
		prterr("Error No Virtual Fields Available.");
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
}
static void quit_print_virtuals(RDArsrc *prsrc)
{
	free_rsrc(prsrc);
}
static void print_virtualscb(RDArsrc *mainrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,print_virtuals,mainrsrc);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_virtuals,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_virtuals(prsrc);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0,y=0;
	RDArsrc *mainrsrc=NULL;
	char *modname=NULL,*fldname=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE EZ VFS")) 
	{
		return;
	}
	if(argc>2)
	{ 
		modname=stralloc(argv[1]);
		fldname=stralloc(argv[2]);
	} else if(argc>1) 
	{
		modname=stralloc(argv[1]);
	}
	x=getvirtmodules(mainrsrc,modname);
	y=dovirtfldlist(mainrsrc,modname,fldname);
	mainrsrc=RDArsrcNEW(module,"DEFINE EZ VIRTUAL FIELDS");
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,dovirtfldlistcb,modsavl->numlibs,&modsavl->libs,NULL);
	addlstrsrc(mainrsrc,"VIRTUALS AVAILABLE",&y,TRUE,setvaluesxcb,viravl->numlibs,&viravl->libs,NULL);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newvirtual,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okvirtual,NULL);
	addbtnrsrc(mainrsrc,"PRINT",TRUE,print_virtualscb,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfvir,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletevirtest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setvaluesx(mainrsrc,modname,fldname);
	APPmakescrn(mainrsrc,TRUE,quitdfvir,NULL,TRUE);
}
