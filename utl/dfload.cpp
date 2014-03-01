/* dflist.c - Define Load Values */
#ifndef WIN32
#define __NAM__ "dfload.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfload.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <ldval.hpp>
#include <rptgen.hpp>
#include<doheader.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="GUI";
RDArsrc *mainrsrc=NULL,*scrlstrsrc=NULL,*incrsrc=NULL;
APPlib *liblst=NULL,*modlst=NULL,*RLVavl=NULL,*inclst=NULL,*special_funcs=NULL;
APPlib *file_defs=NULL;
RDALoadValue *RLV=NULL;
int numfields=0;
EZfield *holdfields=NULL;
static void getRLVmodulescb(RDArsrc *);
static int getRLVmodules(RDArsrc *,char *);
static void setvaluesx(RDArsrc *,char *,char *);
static void setvaluesxcb(RDArsrc *);
static void doRLVfldlistcb(RDArsrc *);
static int doRLVfldlist(RDArsrc *,char *,char *);
static APPlib *getmodulelist(RDArsrc *,char *,APPlib *);
static APPlib *dofilelist(RDArsrc *,char *,APPlib *,char *,APPlib *);
static void incdofilelistcb(RDArsrc *);
static void saveload(RDArsrc *,RDArsrc *);
static void quitloadtest(RDArsrc *,RDArsrc *);
static void quit_include(RDArsrc *,RDArsrc *);
static void quit_includetest(RDArsrc *,RDArsrc *);
static void save_include(RDArsrc *,RDArsrc *);
static void quitload(RDArsrc *,RDArsrc *);
static RDAdefault *incdefaults=NULL,*listdefaults=NULL,*elementdefaults=NULL;
static void print_RLVscb(RDArsrc *);
static void print_RLVs(RDArsrc *,RDArsrc *);
static char changedvalues=FALSE;
static char changedelements=FALSE;

/*
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
*/
/*
static APPlib *makefieldvallist(RDArsrc *scrlstrsrc)
{
	APPlib *tmp=NULL,*listflist=NULL;
	char *holdmodule=NULL,*name=NULL;
	char *tmpx=NULL,addvf=FALSE,*libx=NULL,deleteflag=FALSE,active=FALSE;
	int x,y,z;
	EZfield *f=NULL;
	DFincvir *i=NULL,*a=NULL;
	RDALoadValue *d=NULL;
	short gotbin=0,fileno=0,ef=0;

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
	if(RLV->incvir!=NULL)
	{
		for(x=0,i=RLV->incvir;x<RLV->numincvirs;++x,++i)
		{
			addholdvalues(tmp,i->module,i->file);
		}
		FINDRSCGETSTRING(scrlstrsrc,"MODULE",&holdmodule);
		FINDRSCGETSTRING(scrlstrsrc,"NAME",&name);
		if(listflist!=NULL) freeapplib(listflist);
		tmpx=adddashes(holdmodule);
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(tmpx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,tmpx);
#endif
		if(tmpx!=NULL) Rfree(tmpx);
		listflist=getlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(listflist!=NULL)
		{
			for(y=0;y<listflist->numlibs;++y)
			{
				if(RDAstrcmp(listflist->libs[y],name))
				{
					d=RDALoadValueNEW(holdmodule,listflist->libs[y]);
					gotbin=getRDALoadValuebin(libx,d);
					if(gotbin!=(-1))
					{
						addvf=TRUE;
						if(d->incvir!=NULL)
						{
							for(z=0,a=d->incvir;z<d->numincvirs;++z,++a)
							{
								if(RLV->incvir!=NULL)
								{
								for(x=0,i=RLV->incvir;x<RLV->numincvirs;++x,++i)
								{
									if(!RDAstrcmp(i->module,a->module) && 
										!RDAstrcmp(i->file,a->file)) break;
								}
								if(x>=RLV->numincvirs) 
								{
									addvf=FALSE;
									break;
								}
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
						FreeRDALoadValue(d);
					}
				}
			}
		}
	}
	if(listflist!=NULL) freeapplib(listflist);
	if(!RDAstrcmp(holdmodule,"PAYROLL") && RLV!=NULL)
	{
		if(RLV->incvir!=NULL)
		{
			for(x=0,i=RLV->incvir;x<RLV->numincvirs;++x,++i)
			{
				if(!RDAstrcmp(i->module,holdmodule) && 
					!RDAstrcmp(i->file,"PAYDDMS")) break;
			}
			if(x<RLV->numincvirs)
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
	}
	if(holdmodule!=NULL) Rfree(holdmodule);
	if(name!=NULL) Rfree(name);
	if(libx!=NULL) Rfree(libx);
	loadglobals(tmp);
	if(RLV->mlist!=NULL)
	{
		for(x=0;x<RLV->mload->numlibs;++x)
		{
			addAPPlibNoDuplicates(tmp,RLV->mload->libs[x]);
		}
	}
	SORTAPPlib(tmp);
	return(tmp);
}
*/
/*
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x=0;
	RDALoadValue *load=NULL;

	ldvalrsrc=RDArsrcNEW(module,"LOAD VALUE SCREEN");
	load=RDALoadValueNEW(ldvalrsrc,fieldname);
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
*/
static void makeincludelist()
{
	int l=0;
	short x;
	DFincvir *fields=NULL;
	char *s=NULL;

	if(inclst!=NULL) freeapplib(inclst);
	inclst=APPlibNEW();
	if(RLV->incvir!=NULL)
	{
		for(x=0,fields=RLV->incvir;x<RLV->numincvirs;++x,++fields)
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
		addAPPlib(inclst,"No Include Definitions");
	}
}
static void makespaceinclist(int pos)
{
	short x,y;
	DFincvir *temp=NULL,*temp1=NULL,*new_fields=NULL;

	++RLV->numincvirs;
	new_fields=Rmalloc(RLV->numincvirs*sizeof(DFincvir));
	y=0;
	for(x=0,temp=RLV->incvir;x<(RLV->numincvirs-1);++x,++temp)
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
	Rfree(RLV->incvir);
	RLV->incvir=Rmalloc(RLV->numincvirs*sizeof(DFincvir));
	for(x=0,temp1=RLV->incvir,temp=new_fields;x<RLV->numincvirs;++x,
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
static APPlib *dofilelist(RDArsrc *r,char *modlstname,APPlib *modlst,char *filelstname,APPlib *filelst)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(r,modlstname,&selected)) return(filelst);
	FINDRSCGETINT(r,filelstname,&selectedf);
	if(filelst!=NULL) freeapplib(filelst);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
	filelst=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(filelst==NULL)
	{
		filelst=APPlibNEW();
		addAPPlib(filelst,"No File Definitions");
	}
	if(selectedf>=filelst->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(r,filelstname,selectedf,filelst))
	{
		updatersrc(r,filelstname);
	}
	return(filelst);
}
static void incdofilelistcb(RDArsrc *r)
{
	file_defs=dofilelist(r,"MODULE LIST",modlst,"FILE LIST",file_defs);
}
static void quit_include(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	if(file_defs!=NULL) freeapplib(file_defs);
	if(modlst!=NULL) freeapplib(modlst);
	if(incrsrc!=NULL) 
	{
		killwindow(incrsrc);
		free_rsrc(incrsrc);
	}
	if(incdefaults!=NULL) FreeRDAdefault(incdefaults);
}
static void quit_includetest(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_include,quit_include,FALSE,2,incrsrc,scrlstrsrc,NULL);
	} else {
		quit_include(incrsrc,scrlstrsrc);
	}
}
static void save_include(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	int value=0,selected=0,selectedf=0;
	DFincvir *i=NULL;

	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&value)) return;
	i=RLV->incvir+value;
	readallwidgets(incrsrc);
	FINDRSCGETINT(incrsrc,"MODULE LIST",&selected);
	i->module=stralloc(modlst->libs[selected]);
	FINDRSCGETINT(incrsrc,"FILE LIST",&selectedf);
	i->file=stralloc(file_defs->libs[selectedf]);
	makeincludelist();
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"ASSOCIATED FILES",value,inclst))
		updatersrc(scrlstrsrc,"ASSOCIATED FILES");
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		changedvalues=TRUE;
	}
	quit_include(incrsrc,scrlstrsrc);
}
static void save_include_above(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	int x;

	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&x)) return;
	makespaceinclist(x);
	save_include(incrsrc,scrlstrsrc);
}
static void save_include_below(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	int x;

	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&x)) return;
	makespaceinclist(++x);
	FINDRSCSETINT(scrlstrsrc,"ASSOCIATED FILES",x);
	updatersrc(scrlstrsrc,"ASSOCIATED FILES");
	save_include(incrsrc,scrlstrsrc);
}
static void quit_include_abovetest(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_include_above,quit_include,FALSE,2,incrsrc,scrlstrsrc,NULL);
	} else {
		quit_include(incrsrc,scrlstrsrc);
	}
}
static void quit_include_belowtest(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_include_below,quit_include,FALSE,2,incrsrc,scrlstrsrc,NULL);
	} else {
		quit_include(incrsrc,scrlstrsrc);
	}
}
static void includescreen(RDArsrc *scrlstrsrc,char *modx,char *filx,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0;
	RDArsrc *incrsrc=NULL;
	char *modz=NULL;

	incrsrc=RDArsrcNEW(module,"MAINTAIN LOAD VALUE ASSOCIATED FILES");
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	addlstrsrc(incrsrc,"MODULE LIST",&x,TRUE,incdofilelistcb,
		modlst->numlibs,&modlst->libs,NULL);
	if(file_defs!=NULL) freeapplib(file_defs);
	file_defs=APPlibNEW();
	addlstrsrc(incrsrc,"FILE LIST",&x,TRUE,NULL,
		file_defs->numlibs,&file_defs->libs,NULL);
	modlst=getmodulelist(incrsrc,"MODULE LIST",modlst);
	if(isEMPTY(modx))
	{
		readwidget(scrlstrsrc,"MODULE");
		FINDRSCGETSTRING(scrlstrsrc,"MODULE",&modz);
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(modz,modlst->libs[x])) break;
		}
		if(modz!=NULL) Rfree(modz);
	} else {
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(modx,modlst->libs[x])) break;
		}
	}
	if(x>=modlst->numlibs) x=0;
	FINDRSCSETINT(incrsrc,"MODULE LIST",x);
	file_defs=dofilelist(incrsrc,"MODULE LIST",modlst,"FILE LIST",file_defs);
	for(x=0;x<file_defs->numlibs;++x)
	{
		if(!RDAstrcmp(filx,file_defs->libs[x])) break;
	}
	if(x>=file_defs->numlibs) x=0;
	FINDRSCSETINT(incrsrc,"FILE LIST",x);
	addrfcbrsrc(incrsrc,"SAVE",TRUE,savefunc,scrlstrsrc);
	addrfcbrsrc(incrsrc,"QUIT",TRUE,quitfunctest,scrlstrsrc);
	addbtnrsrc(incrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(incrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	incdefaults=GetDefaults(incrsrc);
	APPmakescrn(incrsrc,TRUE,quit_include,scrlstrsrc,FALSE);
}
static void add_include_above(RDArsrc *scrlstrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&selected)) return;
	if(inclst->numlibs==1 && !RDAstrcmp(inclst->libs[0],"No Include Definitions")) return;
	includescreen(scrlstrsrc,NULL,NULL,save_include_above,quit_include_abovetest);
}
static void add_include_below(RDArsrc *scrlstrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&selected)) return;
	if(inclst->numlibs==1 && !RDAstrcmp(inclst->libs[0],"No Include Definitions")) return;
	includescreen(scrlstrsrc,NULL,NULL,save_include_below,quit_include_belowtest);
}
static void edit_include(RDArsrc *scrlstrsrc)
{
	int x;
	DFincvir *i=NULL;

	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&x)) return;
	if(RLV->incvir==NULL)
	{
		RLV->incvir=Rmalloc(sizeof(DFincvir));
		i=RLV->incvir;
		i->module=NULL;
		i->file=NULL;
		RLV->numincvirs=1;
		x=0;
	}
	i=RLV->incvir+x;
	includescreen(scrlstrsrc,i->module,i->file,save_include,quit_includetest);
}
static void delete_include(RDArsrc *scrlstrsrc)
{
	short x,y;
	DFincvir *temp=NULL,*temp1=NULL,*new_fields=NULL;
	int value=0;

	readallwidgets(scrlstrsrc);
	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&value)) return;
	if((RLV->numincvirs<2) && !RDAstrcmp(inclst->libs[0],"No Include Definitions")) return;
	new_fields=Rmalloc((RLV->numincvirs-1)*sizeof(DFincvir));
	y=0;
	for(x=0,temp=RLV->incvir;x<RLV->numincvirs;++x,++temp)
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
	Rfree(RLV->incvir);
	--RLV->numincvirs;
	RLV->incvir=Rmalloc(RLV->numincvirs*sizeof(DFincvir));
	for(x=0,temp1=RLV->incvir,temp=new_fields;x<RLV->numincvirs;++x,
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
	makeincludelist();
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"ASSOCIATED FILES",value,inclst))
		updatersrc(scrlstrsrc,"ASSOCIATED FILES");
	changedvalues=TRUE;
}
static void quit_element(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	if(elementdefaults!=NULL) FreeRDAdefault(elementdefaults);
	if(elementrsrc!=NULL)
	{
		killwindow(elementrsrc);
		free_rsrc(elementrsrc);
	}
	if(changedelements) changedvalues=TRUE;
}
static void quit_elementtest(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	readallwidgets(elementrsrc);
	if(COMPARE_RSRCS(elementrsrc,elementdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the List Elements.\nDo you want to Save these changes?",save_include,quit_element,FALSE,2,elementrsrc,scrlstrsrc,NULL);
	} else {
		quit_element(elementrsrc,scrlstrsrc);
	}
}
static void quit_elementcls(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	changedelements=FALSE;
	quit_element(elementrsrc,scrlstrsrc);
}
static void save_element(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(elementrsrc);
	if(FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&selected)) return;
	FINDRSCGETSTRING(elementrsrc,"ELEMENT",&temp);
	if(temp!=NULL)
	{
		if(RLV->mlist==NULL)
		{
			RLV->mlist=APPlibNEW();
			addAPPlib(RLV->mlist,temp);
		} else {
			a=RLV->mlist;
			RLV->mlist=APPlibNEW();
			for(x=0;x<a->numlibs;++x)
			{
				if(x==selected) addAPPlib(RLV->mlist,temp);
				else addAPPlib(RLV->mlist,a->libs[x]);
			}
			if(a!=NULL) freeapplib(a);
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(selected>=RLV->mlist->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RLV->mlist))
	updatersrc(scrlstrsrc,"LIST ELEMENTS");
	if(COMPARE_RSRCS(elementrsrc,elementdefaults,2))
	{
		changedelements=TRUE;
	}
	quit_element(elementrsrc,scrlstrsrc);
}
static void save_element_above(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(elementrsrc);
	if(FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&selected)) return;
	FINDRSCGETSTRING(elementrsrc,"ELEMENT",&temp);
	if(temp!=NULL)
	{
		if(RLV->mlist==NULL)
		{
			RLV->mlist=APPlibNEW();
			addAPPlib(RLV->mlist,temp);
		} else {
			a=RLV->mlist;
			RLV->mlist=APPlibNEW();
			for(x=0;x<a->numlibs;++x)
			{
				if(x==selected) addAPPlib(RLV->mlist,temp);
				addAPPlib(RLV->mlist,a->libs[x]);
			}
			if(a!=NULL) freeapplib(a);
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(selected>=RLV->mlist->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RLV->mlist))
	updatersrc(scrlstrsrc,"LIST ELEMENTS");
	if(COMPARE_RSRCS(elementrsrc,elementdefaults,2))
	{
		changedelements=TRUE;
	}
	quit_element(elementrsrc,scrlstrsrc);
}
void save_element_below(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(elementrsrc);
	if(FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&selected)) return;
	FINDRSCGETSTRING(elementrsrc,"ELEMENT",&temp);
	if(temp!=NULL)
	{
		if(RLV->mlist==NULL)
		{
			RLV->mlist=APPlibNEW();
			addAPPlib(RLV->mlist,temp);
		} else {
			a=RLV->mlist;
			RLV->mlist=APPlibNEW();
			for(x=0;x<a->numlibs;++x)
			{
				addAPPlib(RLV->mlist,a->libs[x]);
				if(x==selected) addAPPlib(RLV->mlist,temp);
			}
			if(a!=NULL) freeapplib(a);
		}
		if(temp!=NULL) Rfree(temp);
	}
	selected+=1;
	if(selected>=RLV->mlist->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RLV->mlist))
	updatersrc(scrlstrsrc,"LIST ELEMENTS");
	if(COMPARE_RSRCS(elementrsrc,elementdefaults,2))
	{
		changedelements=TRUE;
	}
	quit_element(elementrsrc,scrlstrsrc);
}
static void quit_element_abovetest(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	readallwidgets(elementrsrc);
	if(COMPARE_RSRCS(elementrsrc,elementdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_element_above,quit_elementcls,FALSE,2,elementrsrc,scrlstrsrc,NULL);
	} else {
		quit_element(elementrsrc,scrlstrsrc);
	}

}
static void quit_element_belowtest(RDArsrc *elementrsrc,RDArsrc *scrlstrsrc)
{
	readallwidgets(elementrsrc);
	if(COMPARE_RSRCS(elementrsrc,elementdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_element_below,quit_elementcls,FALSE,2,elementrsrc,scrlstrsrc,NULL);
	} else {
		quit_element(elementrsrc,scrlstrsrc);
	}
}
static void elementscreen(RDArsrc *scrlstrsrc,char *element,
	void (*savefunc)(...),void (*quitfunc)(...))
{
	RDArsrc *elementrsrc=NULL;

	changedelements=FALSE;
	elementrsrc=RDArsrcNEW(module,"MAINTAIN LOAD VALUE ELEMENTS");
	addstdrsrc(elementrsrc,"ELEMENT",VARIABLETEXT,0,(element!=NULL?element:""),TRUE);
	addrfcbrsrc(elementrsrc,"SAVE",TRUE,savefunc,scrlstrsrc);
	addrfcbrsrc(elementrsrc,"QUIT",TRUE,quitfunc,scrlstrsrc);
	addbtnrsrc(elementrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(elementrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	elementdefaults=GetDefaults(elementrsrc);
	APPmakescrn(elementrsrc,TRUE,quit_element,scrlstrsrc,FALSE);
}
static void add_element_above(RDArsrc *scrlistrsrc)
{
        int x;

        if(FINDRSCGETINT(scrlistrsrc,"LIST ELEMENTS",&x)) return;
        if(RLV->mlist->numlibs==1 && !RDAstrcmp(RLV->mlist->libs[0],"No Elements Defined")) return;
	elementscreen(scrlistrsrc,NULL,save_element_above,quit_element_abovetest);
}
static void add_element_below(RDArsrc *scrlstrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&selected)) return;
	if(RLV->mlist->numlibs==1 && !RDAstrcmp(RLV->mlist->libs[0],"No Elements Defined")) return;
	elementscreen(scrlstrsrc,NULL,save_element_below,quit_element_belowtest);
}
static void edit_element(RDArsrc *scrlstrsrc)
{
	int x;

	if(FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&x)) return;
	if(RLV->mlist!=NULL)
	{
		if(!RDAstrcmp(RLV->mlist->libs[x],"No Elements Defined"))
		{
			elementscreen(scrlstrsrc,NULL,save_element,quit_elementtest);
		} else {
			elementscreen(scrlstrsrc,RLV->mlist->libs[x],save_element,quit_elementtest);
		}
	} else {
		RLV->mlist=APPlibNEW();
		addAPPlib(RLV->mlist,"No Elements Defined");
		elementscreen(scrlstrsrc,NULL,save_element,quit_elementtest);
	}

}
static void delete_element(RDArsrc *scrlstrsrc)
{
	int selected=0,x=0;
	APPlib *a=NULL;

	FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&selected);
	if(RLV->mlist->numlibs>0 && RDAstrcmp(RLV->mlist->libs[0],"No Elements Defined"))
	{
		a=RLV->mlist;
		RLV->mlist=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x!=selected)
			{
				addAPPlib(RLV->mlist,a->libs[x]);
			}
		}
		freeapplib(a);
	} else {
		freeapplib(RLV->mlist);
		RLV->mlist=APPlibNEW();
	}
	if(RLV->mlist->numlibs<1)
	{
		addAPPlib(RLV->mlist,"No Elements Defined");
	}
	if(selected>=RLV->mlist->numlibs) selected=0;
	FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RLV->mlist);
	updatersrc(scrlstrsrc,"LIST ELEMENTS");
	changedelements=TRUE;
}
static void quitload(RDArsrc *scrlstrsrc,RDArsrc *mainrsrc)
{
	changedvalues=FALSE;
	if(scrlstrsrc!=NULL)
	{
		killwindow(scrlstrsrc);
		free_rsrc(scrlstrsrc);
	}
	if(listdefaults!=NULL) FreeRDAdefault(listdefaults);
}
static void quitloadtest(RDArsrc *scrlstrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(scrlstrsrc);
	if(COMPARE_RSRCS(scrlstrsrc,listdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the LOAD VALUE DEFINITION.\nDo you want to Save these changes?",saveload,quitload,FALSE,2,scrlstrsrc,mainrsrc,NULL);
	} else {
		quitload(scrlstrsrc,mainrsrc);
	}
}
static void saveload(RDArsrc *scrlstrsrc,RDArsrc *mainrsrc)
{
	char *libx=NULL;
	char *modulx=NULL;

	readallwidgets(scrlstrsrc);
	FINDRSCGETSTRING(scrlstrsrc,"MODULE",&RLV->module);
	FINDRSCGETSTRING(scrlstrsrc,"NAME",&RLV->name);
	if(RLV->module!=NULL && RLV->name!=NULL)
	{
		FINDRSCGETSTRING(scrlstrsrc,"DESCRIPTION",&RLV->desc);
		FINDRSCGETINT(scrlstrsrc,"LOAD TYPES",&RLV->type);
		FINDRSCGETSTRING(scrlstrsrc,"CONNECT NAME",&RLV->conname);
		if(RLV->mlist!=NULL)
		{
			if(RLV->mlist->numlibs<0 || !RDAstrcmp(RLV->mlist->libs[0],"No Elements Defined"))
			{
				freeapplib(RLV->mlist);
			} else if(RLV->mlist->numlibs<1)
			{
				freeapplib(RLV->mlist);	
			}
		}
		modulx=adddashes(RLV->module);	
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,modulx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,modulx);
#endif
		if(writeRDALoadValuebin(libx,RLV))
		{
			ERRORDIALOG("Cannot Write Load Value","Error attempting to write the Load Value definition.  Check the permissions on the Load Value Binary Library, and retry.  Call your installer.",NULL,FALSE);
		}
		if(libx!=NULL) Rfree(libx);
		FreeRDALoadValue(RLV);
	}
	doRLVfldlistcb(mainrsrc);
	getRLVmodulescb(mainrsrc);
	quitload(scrlstrsrc,mainrsrc);
}
void printRLV(RDArsrc *parent,RDALoadValue *s)
{
        SEERDALoadValue(s,NULL,doheaders);
}
static void loadvaluescreen(RDArsrc *mainrsrc,char* modulx,char *name,
	int type,int numincvirs,DFincvir *incvir,char *conname,char *desc,
	APPlib *mlist)
{
	int x=0;
	RDArsrc *scrlstrsrc=NULL;

	scrlstrsrc=RDArsrcNEW(module,"MAINTAIN LOAD VALUE");
	if(inclst!=NULL) freeapplib(inclst);
	inclst=APPlibNEW();
	addlstrsrc(scrlstrsrc,"ASSOCIATED FILES",&x,TRUE,NULL,inclst->numlibs,&inclst->libs,NULL);
	addbtnrsrc(scrlstrsrc,"ADD ABOVE",TRUE,add_include_above,NULL);
	addbtnrsrc(scrlstrsrc,"ADD BELOW",TRUE,add_include_below,NULL);
	addbtnrsrc(scrlstrsrc,"DELETE",TRUE,delete_include,NULL);
	addbtnrsrc(scrlstrsrc,"SELECT",TRUE,edit_include,NULL);
	makeincludelist();
	FINDRSCLISTAPPlib(scrlstrsrc,"ASSOCIATED FILES",x,inclst);
	addstdrsrc(scrlstrsrc,"NAME",VARIABLETEXT,0,(name!=NULL?name:""),TRUE);
	addstdrsrc(scrlstrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL?modulx:""),TRUE);
	addstdrsrc(scrlstrsrc,"CONNECT NAME",VARIABLETEXT,0,(conname!=NULL?conname:""),TRUE);
	addsctrsrc(scrlstrsrc,"DESCRIPTION",0,(desc!=NULL?desc:""),TRUE);
	if(mlist==NULL)
	{
		mlist=APPlibNEW();
		addAPPlib(mlist,"No Elements Defined");
	}
	addlstrsrc(scrlstrsrc,"LIST ELEMENTS",&mlist,TRUE,NULL,mlist->numlibs,&mlist->libs,NULL);
	addbtnrsrc(scrlstrsrc,"ADD ELEMENT ABOVE",TRUE,add_element_above,NULL);
	addbtnrsrc(scrlstrsrc,"ADD ELEMENT BELOW",TRUE,add_element_below,NULL);
	addbtnrsrc(scrlstrsrc,"DELETE ELEMENT",TRUE,delete_element,NULL);
	addbtnrsrc(scrlstrsrc,"SELECT ELEMENT",TRUE,edit_element,NULL);

	addrfcbrsrc(scrlstrsrc,"SAVE",TRUE,saveload,mainrsrc);
	addrfcbrsrc(scrlstrsrc,"QUIT",TRUE,quitloadtest,mainrsrc);
	addbtnrsrc(scrlstrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(scrlstrsrc,"PRINT DEFINITION",TRUE,printRLV,RLV);
	addbtnrsrc(scrlstrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedvalues=FALSE;
	APPmakescrn(scrlstrsrc,TRUE,quitload,mainrsrc,FALSE);
	listdefaults=GetDefaults(scrlstrsrc);
}
static void okRLV(RDArsrc *mainrsrc)
{
	int selected=0,selectedl=0;
	char *modulx=NULL,*libx=NULL;
	short gotten=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LOADS AVAILABLE",&selectedl)) return;
	if(RLV!=NULL) FreeRDALoadValue(RLV); 
	if(RDAstrcmp(RLVavl->libs[selectedl],"No Load Values")) 
	{
		modulx=adddashes(liblst->libs[selected]);	
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,modulx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,modulx);
#endif
		RLV=RDALoadValueNEW(liblst->libs[selected],RLVavl->libs[selectedl]);
		gotten=getRDALoadValuebin(libx,RLV);
		if(libx!=NULL) Rfree(libx);
		if(modulx!=NULL) Rfree(modulx);
		if(gotten==(-1))
		{
			ERRORDIALOG("Cannot Read Load Value","Error in permissions, the user cannot read the Load Value binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			return;
		}
	}
	if(RLV==NULL)
	{
		RLV=RDALoadValueNEW(liblst->libs[selected],NULL);
	}
	loadvaluescreen(mainrsrc,RLV->module,RLV->name,
		RLV->type,RLV->numincvirs,RLV->incvir,
		RLV->conname,RLV->desc,RLV->mlist);
}
static void newRLV(RDArsrc *mainrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(RLV!=NULL) FreeRDALoadValue(RLV);
	RLV=RDALoadValueNEW(liblst->libs[selected],NULL);
	loadvaluescreen(mainrsrc,RLV->module,RLV->name,
		RLV->type,RLV->numincvirs,RLV->incvir,
		RLV->conname,RLV->desc,RLV->mlist);
}
static void quitdfload(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(liblst!=NULL) freeapplib(liblst);
	if(modlst!=NULL) freeapplib(modlst);
	if(RLVavl!=NULL) freeapplib(RLVavl);
	if(incdefaults!=NULL) FreeRDAdefault(incdefaults);
	if(listdefaults!=NULL) FreeRDAdefault(listdefaults);
	if(elementdefaults!=NULL) FreeRDAdefault(elementdefaults);
	ShutdownSubsystems();
}
static void setvaluesx(RDArsrc *mainrsrc,char *modname,char *loadvaluename)
{
	char *temp=NULL,*temp2=NULL,*temp3=NULL;
	char *libx=NULL;

	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,modname);
#endif
		if(loadvaluename!=NULL)
		{
			if(!RDAstrcmp(loadvaluename,"No Load Values"))
			{
				temp=NULL;
			} else {
				temp=getRDALoadValuedesc(libx,loadvaluename);
			}
		} else {
			temp3=stralloc(RLVavl->libs[0]);
			if(!RDAstrcmp(temp3,"No Load Values"))
			{
				temp=NULL;
			} else {
				temp=getRDALoadValuedesc(libx,temp3);
			}
		}
	} else {
		temp2=stralloc(liblst->libs[0]);
		temp3=stralloc(RLVavl->libs[0]);
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp2)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,temp2);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,temp2);
#endif
		if(!RDAstrcmp(temp3,"No Load Values"))
		{
			temp=NULL;
		} else {
			temp=getRDALoadValuedesc(libx,temp3);
		}
	}
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		{
			updatersrc(mainrsrc,"DESCRIPTION");
		}
	}
	if(libx!=NULL) Rfree(libx);
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
}
static void setvaluesxcb(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"LOADS AVAILABLE",&selected)) return;
	setvaluesx(mainrsrc,liblst->libs[selectedm],RLVavl->libs[selected]);
}
static int doRLVfldlist(RDArsrc *mainrsrc,char *modname,char *loadvaluename)
{
	int x=0,selectedf=0;
	char *libx=NULL;

	if(RLVavl!=NULL) freeapplib(RLVavl);
	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,modname);
#endif
	} else {
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(liblst->libs[0])+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,liblst->libs[0]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,liblst->libs[0]);
#endif
	}
	RLVavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(RLVavl==NULL)
	{
		RLVavl=APPlibNEW();
		addAPPlib(RLVavl,"No Load Values");
	}
	if(loadvaluename!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(RLVavl,loadvaluename))==(-1))
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
		if(!FINDRSCLISTAPPlib(mainrsrc,"LOADS AVAILABLE",selectedf,RLVavl))
		{
			updatersrc(mainrsrc,"LOADS AVAILABLE");
		}
	}
	return(selectedf);
}
static void doRLVfldlistcb(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *temp=NULL,*temp2=NULL;
	int x=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LOADS AVAILABLE",&selectedf)) return;
	temp=stralloc(liblst->libs[selected]);
	temp2=stralloc(RLVavl->libs[selectedf]);
	x=doRLVfldlist(mainrsrc,temp,temp2);
	setvaluesx(mainrsrc,temp,RLVavl->libs[x]);
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
}
static int getRLVmodules(RDArsrc *mainrsrc,char *modname)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;

	if(liblst!=NULL) freeapplib(liblst);
	liblst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.LDV",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(liblst,tmp);
	}
	if(liblst->numlibs<1)
	{
		addAPPlib(liblst,"No Load Value Libraries");
		selectedm=0;
	} else {
		SORTAPPlib(liblst);
		if(selectedm>=liblst->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(liblst,modname))==(-1))
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
		if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selectedm,liblst))
		{
			updatersrc(mainrsrc,"MODULE LIST");
		}
	}
	return(selectedm);
}
static void getRLVmodulescb(RDArsrc *mainrsrc)
{
	int selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	temp=stralloc(liblst->libs[selectedm]);
	getRLVmodules(mainrsrc,temp);
	doRLVfldlistcb(mainrsrc);
	if(temp!=NULL) Rfree(temp);
}
static APPlib *getmodulelist(RDArsrc *r,char *modlstname,APPlib *modlst)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	
	if(FINDRSCGETINT(r,modlstname,&selectedm)) return(modlst);
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
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
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"No Database Libraries");
		selectedm=0;
	} else {
		SORTAPPlib(modlst);
		if(selectedm>=modlst->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(!FINDRSCLISTAPPlib(r,modlstname,selectedm,modlst))
	{
		updatersrc(r,modlstname);
	}
	return(modlst);
}
static void deleteRLVlist(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulx=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"LOADS AVAILABLE",&s)) return;
	modulx=adddashes(liblst->libs[m]);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulx)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,modulx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,modulx);
#endif
	if(deletelibbin(libx,RLVavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Load Value Definition","Error attempting to delete Load Value definition.  Check the permissions on the Load Value Binary Library, and retry.  Call your installer.",NULL,FALSE);
	} else {
		getRLVmodulescb(mainrsrc);
	}
	if(libx!=NULL) Rfree(libx);
	if(modulx!=NULL) Rfree(modulx);
}
static void deleteRLVtest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(RLVavl->libs[s],"No Load Values")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE LOAD VALUE!","Are you sure you want to delete this Load Value definition?",deleteRLVlist,NULL,FALSE,mainrsrc);
}
static void print_RLVs(RDArsrc *prsrc,RDArsrc *mainrsrc)
{
	RDA_PFILE *fp=NULL;
	int pages=1,numpagelines=1;
	int x=0,selected=0;
	char *tmpx=NULL,*libx=NULL,*odevice=NULL,print_style=TRUE;
	RDALoadValue *d=NULL;
	APPlib *loadvaluenames=NULL;
	short gotbin=0;

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
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(liblst->libs[selected])+10);
	tmpx=adddashes(liblst->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.LDV",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,tmpx);
#endif
	loadvaluenames=getlibnames(libx,FALSE);
	if(loadvaluenames!=NULL)
	{
		fp=RDA_popen(odevice,"w");
		if(fp==NULL)
		{
			prterr("Error Couldn't open device.");
			if(loadvaluenames!=NULL) freeapplib(loadvaluenames);
			if(tmpx!=NULL) Rfree(tmpx);
			if(libx!=NULL) Rfree(libx);
			if(odevice!=NULL) Rfree(odevice);
			return;
		} 
		SelectPrintType(odevice,print_style,fp);
		if(odevice!=NULL) Rfree(odevice);
		for(x=0;x<loadvaluenames->numlibs;++x)
		{
			d=RDALoadValueNEW(liblst->libs[selected],loadvaluenames->libs[x]);
			gotbin=getRDALoadValuebin(libx,d);
			if(gotbin!=(-1))
			{
        			SEERDALoadValue_detail(d,fp,doheaders,&pages,&numpagelines);
				RDA_fprintf(fp,"\f\r\n");pages=1;numpagelines=1;
			}
			FreeRDALoadValue(d);
			d=NULL;
		}
		RDA_pclose(fp);
		if(loadvaluenames!=NULL) freeapplib(loadvaluenames);
	} else {
		prterr("Error No Load Values Available.");
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
}
static void quit_print_RLVs(RDArsrc *prsrc)
{
	free_rsrc(prsrc);
}
static void print_RLVscb(RDArsrc *mainrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,print_RLVs,mainrsrc);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_RLVs,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_RLVs(prsrc);
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
	char *modname=NULL,*loadvaluename=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE LOAD VALUES")) 
	{
		return;
	}
	if(argc>2)
	{
		modname=stralloc(argv[1]);
		loadvaluename=stralloc(argv[2]);
	} else if(argc>1)
	{
		modname=stralloc(argv[1]);
	}
	x=getRLVmodules(mainrsrc,modname);
	y=doRLVfldlist(mainrsrc,modname,loadvaluename);

	DoHeaders();
	mainrsrc=RDArsrcNEW(module,"DEFINE LOAD VALUES");
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,doRLVfldlistcb,liblst->numlibs,&liblst->libs,NULL);
	addlstrsrc(mainrsrc,"LOADS AVAILABLE",&y,TRUE,setvaluesxcb,RLVavl->numlibs,&RLVavl->libs,NULL);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newRLV,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okRLV,NULL);
	addbtnrsrc(mainrsrc,"PRINT MODULE",TRUE,print_RLVscb,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfload,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deleteRLVtest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setvaluesx(mainrsrc,modname,loadvaluename);
	APPmakescrn(mainrsrc,TRUE,quitdfload,NULL,TRUE);
}
