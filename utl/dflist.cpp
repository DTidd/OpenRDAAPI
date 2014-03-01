/* dflist.c - Define Scrolled Lists */
#ifndef WIN32
#define __NAM__ "dflist.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dflist.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <ldval.hpp>
#include <rptgen.hpp>
#include <doheader.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="GUI";
RDArsrc *mainrsrc=NULL,*scrlstrsrc=NULL,*incrsrc=NULL;
APPlib *liblst=NULL,*modlst=NULL,*lstavl=NULL,*inclst=NULL,*special_funcs=NULL;
APPlib *ltypes=NULL,*file_defs=NULL,*vtypes=NULL,*contypes=NULL,*timtypes=NULL;
APPlib *fmodlst=NULL,*ffilelst=NULL,*fkeylst=NULL;
APPlib *cmodlst=NULL,*cfilelst=NULL,*cfldlst=NULL;
RDAScrolledList *RSL=NULL;
int numfields=0;
EZfield *holdfields=NULL;
static void getSlistmodulescb(RDArsrc *);
static int getSlistmodules(RDArsrc *,char *);
static void setvaluesx(RDArsrc *,char *,char *);
static void setvaluesxcb(RDArsrc *);
static void doSlistfldlistcb(RDArsrc *);
static int doSlistfldlist(RDArsrc *,char *,char *);
static APPlib *getmodulelist(RDArsrc *,char *,APPlib *);
static APPlib *dofilelist(RDArsrc *,char *,APPlib *,char *,APPlib *);
static APPlib *dokeylist(RDArsrc *,char *,APPlib *,char *,APPlib *,char *,APPlib *);
static APPlib *dofldlist(RDArsrc *,char *,APPlib *,char *,APPlib *,char *,APPlib *);
static void condofilelist(RDArsrc *);
static void condofilelistcb(RDArsrc *);
static void condofldlist(RDArsrc *);
static void incdofilelistcb(RDArsrc *);
static void fromdofilelist(RDArsrc *);
static void fromdofilelistcb(RDArsrc *);
static void fromdokeylist(RDArsrc *);
static void changelisttype(RDArsrc *,DFincvir *);
static void changecontype(RDArsrc *);
static void savelist(RDArsrc *,RDArsrc *);
static void quitlisttest(RDArsrc *,RDArsrc *);
static void quit_element(RDArsrc *,RDArsrc *);
static void quit_elementtest(RDArsrc *,RDArsrc *);
static void save_element(RDArsrc *,RDArsrc *);
static void quit_include(RDArsrc *,RDArsrc *);
static void quit_includetest(RDArsrc *,RDArsrc *);
static void save_include(RDArsrc *,RDArsrc *);
static void quitlist(RDArsrc *,RDArsrc *);
static RDAdefault *incdefaults=NULL,*listdefaults=NULL,*elementdefaults=NULL;
static void print_listscb(RDArsrc *);
static void print_lists(RDArsrc *,RDArsrc *);
static APPlib *getspecial_funcs(APPlib *);
static char changedvalues=FALSE;
static char changedelements=FALSE;
static void addholdvalues(APPlib *,char *,char *);

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
static APPlib *makefieldvallist(RDArsrc *scrlstrsrc)
{
	APPlib *tmp=NULL,*listflist=NULL;
	char *holdmodule=NULL,*name=NULL;
	char *tmpx=NULL,addvf=FALSE,*libx=NULL,deleteflag=FALSE,active=FALSE;
	int x,y,z;
	EZfield *f=NULL;
	DFincvir *i=NULL,*a=NULL;
	RDAScrolledList *d=NULL;
	short gotbin=0,fileno=0,ef=0;
	char *libx2=NULL;

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
	if(RSL->incvir!=NULL)
	{
		for(x=0,i=RSL->incvir;x<RSL->numincvirs;++x,++i)
		{
			addholdvalues(tmp,i->module,i->file);
		}
		FINDRSCGETSTRING(scrlstrsrc,"MODULE",&holdmodule);
		FINDRSCGETSTRING(scrlstrsrc,"NAME",&name);
		if(listflist!=NULL) freeapplib(listflist);
		tmpx=adddashes(holdmodule);
		libx2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(tmpx)+10);
#ifndef WIN32
		sprintf(libx2,"%s/rda/%s.LST",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libx2,"%s\\rda\\%s.LST",CURRENTDIRECTORY,tmpx);
#endif
		listflist=getlibnames(libx2,FALSE);
		if(tmpx!=NULL) Rfree(tmpx);
		if(listflist!=NULL)
		{
			for(y=0;y<listflist->numlibs;++y)
			{
				if(RDAstrcmp(listflist->libs[y],name))
				{
					d=RDAScrolledListNew(holdmodule,listflist->libs[y]);
					gotbin=getScrolledListbin(libx2,d);
					if(gotbin!=(-1))
					{
						addvf=TRUE;
						if(d->incvir!=NULL)
						{
							for(z=0,a=d->incvir;z<d->numincvirs;++z,++a)
							{
								if(RSL->incvir!=NULL)
								{
								for(x=0,i=RSL->incvir;x<RSL->numincvirs;++x,++i)
								{
									if(!RDAstrcmp(i->module,a->module) && 
										!RDAstrcmp(i->file,a->file)) break;
								}
								if(x>=RSL->numincvirs) 
								{
									addvf=FALSE;
									break;
								}
								}
							}
						}
						if(addvf)
						{
							if(libx!=NULL) Rfree(libx);
							libx=Rmalloc(RDAstrlen(d->name)+3);
							sprintf(libx,"[%s]",d->name);
							addAPPlibNoDuplicates(tmp,libx);
							if(libx!=NULL) Rfree(libx);
						}
						FreeRDAScrolledList(d);
					}
				}
			}
		}
	}
	if(libx2!=NULL) Rfree(libx2);
	if(listflist!=NULL) freeapplib(listflist);
	if(!RDAstrcmp(holdmodule,"PAYROLL") && RSL!=NULL)
	{
		if(RSL->incvir!=NULL)
		{
			for(x=0,i=RSL->incvir;x<RSL->numincvirs;++x,++i)
			{
				if(!RDAstrcmp(i->module,holdmodule) && 
					!RDAstrcmp(i->file,"PAYDDMS")) break;
			}
			if(x<RSL->numincvirs)
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
							if(libx!=NULL) Rfree(libx);
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
	if(RSL->incvir!=NULL)
	{
		for(x=0,fields=RSL->incvir;x<RSL->numincvirs;++x,++fields)
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

	++RSL->numincvirs;
	new_fields=Rmalloc(RSL->numincvirs*sizeof(DFincvir));
	y=0;
	for(x=0,temp=RSL->incvir;x<(RSL->numincvirs-1);++x,++temp)
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
	Rfree(RSL->incvir);
	RSL->incvir=Rmalloc(RSL->numincvirs*sizeof(DFincvir));
	for(x=0,temp1=RSL->incvir,temp=new_fields;x<RSL->numincvirs;++x,
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
static APPlib *dokeylist(RDArsrc *r,char *modlstname,APPlib *modlst,char *filelstname,APPlib *filelst,char *keylstname,APPlib *keylst)
{
	int selectedm=0,selectedf=0,selectedk=0;
	short x,y,engine=0,nokeys=0,nofields=0;
	NRDfield *fieldx=NULL,*field;
	NRDkey *keyx=NULL,*key;
	NRDpart *part;

	if(FINDRSCGETINT(r,modlstname,&selectedm)) return(keylst);
	if(FINDRSCGETINT(r,filelstname,&selectedf)) return(keylst);
	FINDRSCGETINT(r,keylstname,&selectedk);
	if(keylst!=NULL) freeapplib(keylst);
	keylst=APPlibNEW();
	if(!GETBIN(modlst->libs[selectedm],filelst->libs[selectedf],&engine,
		&nofields,&fieldx,&nokeys,&keyx))
	{
		if(keyx!=NULL)
		{
			for(x=0,key=keyx;x<nokeys;++x,++key)
			{
				addAPPlib(keylst,key->name);
				if(key->name!=NULL) Rfree(key->name);
				if(key->part!=NULL)
				{
					for(y=0,part=key->part;y<key->numparts;++y,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(key->part);
				}
			}
			Rfree(keyx);
		}
		if(fieldx!=NULL)
		{
			for(x=0,field=fieldx;x<nofields;++x,++field)
			{
				if(field->name!=NULL) Rfree(field->name);
			}
			Rfree(fieldx);
		}
	}
	if(keylst->numlibs<1)
	{
		addAPPlib(keylst,"No Keys Defined");
	}
	if(selectedk>=keylst->numlibs) selectedk=0;
	if(!FINDRSCLISTAPPlib(r,keylstname,selectedk,keylst))
	{
		updatersrc(r,keylstname);
	}
	return(keylst);
}
static APPlib *dofldlist(RDArsrc *r,char *modlstname,APPlib *modlst,char *filelstname,APPlib *filelst,char *fldlstname,APPlib *fldlst)
{
	int selectedm=0,selectedf=0,selectedd=0;
	short x,y,engine=0,nokeys=0,nofields=0;
	NRDfield *fieldx=NULL,*field;
	NRDkey *keyx=NULL,*key;
	NRDpart *part;

	if(FINDRSCGETINT(r,modlstname,&selectedm)) return(fldlst);
	if(FINDRSCGETINT(r,filelstname,&selectedf)) return(fldlst);
	FINDRSCGETINT(r,fldlstname,&selectedd);
	if(fldlst!=NULL) freeapplib(fldlst);
	fldlst=APPlibNEW();
	if(!GETBIN(modlst->libs[selectedm],filelst->libs[selectedf],&engine,
		&nofields,&fieldx,&nokeys,&keyx))
	{
		if(keyx!=NULL)
		{
			for(x=0,key=keyx;x<nokeys;++x,++key)
			{
				if(key->name!=NULL) Rfree(key->name);
				if(key->part!=NULL)
				{
					for(y=0,part=key->part;y<key->numparts;++y,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(key->part);
				}
			}
			Rfree(keyx);
		}
		if(fieldx!=NULL)
		{
			for(x=0,field=fieldx;x<nofields;++x,++field)
			{
				addAPPlib(fldlst,field->name);
				if(field->name!=NULL) Rfree(field->name);
			}
			Rfree(fieldx);
		}
	}
	if(fldlst->numlibs<1)
	{
		addAPPlib(fldlst,"No Keys Defined");
	}
	if(selectedd>=fldlst->numlibs) selectedd=0;
	if(!FINDRSCLISTAPPlib(r,fldlstname,selectedd,fldlst))
	{
		updatersrc(r,fldlstname);
	}
	return(fldlst);
}
static void condofilelistcb(RDArsrc *r)
{
	int selected=0;

	condofilelist(r);
	condofldlist(r);
/*
	cfilelst=dofilelist(r,"CONNECT MODULE LIST",cmodlst,"CONNECT FILE LIST",cfilelst);
	cfldlst=dofldlist(r,"CONNECT MODULE LIST",cmodlst,"CONNECT FILE LIST",cfilelst,"CONNECT FIELD LIST",cfldlst);
*/
	FINDRSCGETINT(r,"CONNECT MODULE LIST",&selected);
	if(RDAstrcmp(cmodlst->libs[selected],"No Database Libraries"))
	{
		FINDRSCSETSTRING(r,"CONNECT MODULE",cmodlst->libs[selected]);
		updatersrc(r,"CONNECT MODULE");
	}
}
static void condofilelist(RDArsrc *r)
{
	cfilelst=dofilelist(r,"CONNECT MODULE LIST",cmodlst,"CONNECT FILE LIST",cfilelst);
}
static void condofldlist(RDArsrc *r)
{
	cfldlst=dofldlist(r,"CONNECT MODULE LIST",cmodlst,"CONNECT FILE LIST",cfilelst,"CONNECT FIELD LIST",cfldlst);
}
static void incdofilelistcb(RDArsrc *r)
{
	file_defs=dofilelist(r,"MODULE LIST",modlst,"FILE LIST",file_defs);
}
static void fromdofilelistcb(RDArsrc *r)
{
	ffilelst=dofilelist(r,"FROM MODULE LIST",fmodlst,"FROM FILE LIST",ffilelst);
	fkeylst=dokeylist(r,"FROM MODULE LIST",fmodlst,"FROM FILE LIST",ffilelst,"FROM KEYNAME LIST",fkeylst);
}
static void fromdofilelist(RDArsrc *r)
{
	ffilelst=dofilelist(r,"FROM MODULE LIST",fmodlst,"FROM FILE LIST",ffilelst);
}
static void fromdokeylist(RDArsrc *r)
{
	fkeylst=dokeylist(r,"FROM MODULE LIST",fmodlst,"FROM FILE LIST",ffilelst,"FROM KEYNAME LIST",fkeylst);
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
		if(RSL->list==NULL)
		{
			RSL->list=APPlibNEW();
			addAPPlib(RSL->list,temp);
		} else {
			a=RSL->list;
			RSL->list=APPlibNEW();
			for(x=0;x<a->numlibs;++x)
			{
				if(x==selected) addAPPlib(RSL->list,temp);
				else addAPPlib(RSL->list,a->libs[x]);
			}
			if(a!=NULL) freeapplib(a);
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(selected>=RSL->list->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RSL->list))
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
		if(RSL->list==NULL)
		{
			RSL->list=APPlibNEW();
			addAPPlib(RSL->list,temp);
		} else {
			a=RSL->list;
			RSL->list=APPlibNEW();
			for(x=0;x<a->numlibs;++x)
			{
				if(x==selected) addAPPlib(RSL->list,temp);
				addAPPlib(RSL->list,a->libs[x]);
			}
			if(a!=NULL) freeapplib(a);
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(selected>=RSL->list->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RSL->list))
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
		if(RSL->list==NULL)
		{
			RSL->list=APPlibNEW();
			addAPPlib(RSL->list,temp);
		} else {
			a=RSL->list;
			RSL->list=APPlibNEW();
			for(x=0;x<a->numlibs;++x)
			{
				addAPPlib(RSL->list,a->libs[x]);
				if(x==selected) addAPPlib(RSL->list,temp);
			}
			if(a!=NULL) freeapplib(a);
		}
		if(temp!=NULL) Rfree(temp);
	}
	selected+=1;
	if(selected>=RSL->list->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RSL->list))
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
	int type=0;

	changedelements=FALSE;
	FINDRSCGETINT(scrlstrsrc,"LIST TYPES",&type);
	if(type!=3)
	{
		elementrsrc=RDArsrcNEW(module,"MAINTAIN SCROLLED LIST ELEMENTS");
		addstdrsrc(elementrsrc,"ELEMENT",VARIABLETEXT,0,(element!=NULL?element:""),TRUE);
	} else {
		elementrsrc=RDArsrcNEW(module,"MAINTAIN COMPUTED LIST ELEMENTS");
		addsctrsrc(elementrsrc,"ELEMENT",0,(element!=NULL?element:""),TRUE);
	}
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
        if(RSL->list->numlibs==1 && !RDAstrcmp(RSL->list->libs[0],"No Elements Defined")) return;
	elementscreen(scrlistrsrc,NULL,save_element_above,quit_element_abovetest);
}
static void add_element_below(RDArsrc *scrlstrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&selected)) return;
	if(RSL->list->numlibs==1 && !RDAstrcmp(RSL->list->libs[0],"No Elements Defined")) return;
	elementscreen(scrlstrsrc,NULL,save_element_below,quit_element_belowtest);
}
static void edit_element(RDArsrc *scrlstrsrc)
{
	int x;

	if(FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&x)) return;
	if(RSL->list!=NULL)
	{
		if(!RDAstrcmp(RSL->list->libs[x],"No Elements Defined"))
		{
			elementscreen(scrlstrsrc,NULL,save_element,quit_elementtest);
		} else {
			elementscreen(scrlstrsrc,RSL->list->libs[x],save_element,quit_elementtest);
		}
	} else {
		RSL->list=APPlibNEW();
		addAPPlib(RSL->list,"No Elements Defined");
		elementscreen(scrlstrsrc,NULL,save_element,quit_elementtest);
	}

}
static void delete_element(RDArsrc *scrlstrsrc)
{
	int selected=0,x=0;
	APPlib *a=NULL;

	FINDRSCGETINT(scrlstrsrc,"LIST ELEMENTS",&selected);
	if(RSL->list->numlibs>0 && RDAstrcmp(RSL->list->libs[0],"No Elements Defined"))
	{
		a=RSL->list;
		RSL->list=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x!=selected)
			{
				addAPPlib(RSL->list,a->libs[x]);
			}
		}
		freeapplib(a);
	} else {
		freeapplib(RSL->list);
		RSL->list=APPlibNEW();
	}
	if(RSL->list->numlibs<1)
	{
		addAPPlib(RSL->list,"No Elements Defined");
	}
	if(selected>=RSL->list->numlibs) selected=0;
	FINDRSCLISTAPPlib(scrlstrsrc,"LIST ELEMENTS",selected,RSL->list);
	updatersrc(scrlstrsrc,"LIST ELEMENTS");
	changedelements=TRUE;
}
static void quit_include(RDArsrc *incrsrc,RDArsrc *scrlstrsrc)
{
	if(incrsrc!=NULL) 
	{
		killwindow(incrsrc);
		free_rsrc(incrsrc);
	}
	if(file_defs!=NULL) freeapplib(file_defs);
	if(modlst!=NULL) freeapplib(modlst);
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
	i=RSL->incvir+value;
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

	incrsrc=RDArsrcNEW(module,"MAINTAIN SCROLLED LIST ASSOCIATED FILES");
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	addlstrsrc(incrsrc,"MODULE LIST",&x,TRUE,incdofilelistcb,
		modlst->numlibs,&modlst->libs,NULL);
	if(file_defs!=NULL) freeapplib(file_defs);
	file_defs=APPlibNEW();
	addlstrsrc(incrsrc,"FILE LIST",&x,TRUE,NULL,
		file_defs->numlibs,&file_defs->libs,NULL);
	modlst=getmodulelist(incrsrc,"MODULE LIST",modlst);
	for(x=0;x<modlst->numlibs;++x)
	{
		if(!RDAstrcmp(modx,modlst->libs[x])) break;
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
	if(RSL->incvir==NULL)
	{
		RSL->incvir=Rmalloc(sizeof(DFincvir));
		i=RSL->incvir;
		i->module=NULL;
		i->file=NULL;
		RSL->numincvirs=1;
		x=0;
	}
	i=RSL->incvir+x;
	includescreen(scrlstrsrc,i->module,i->file,save_include,quit_includetest);
}
static void delete_include(RDArsrc *scrlstrsrc)
{
	short x,y;
	DFincvir *temp=NULL,*temp1=NULL,*new_fields=NULL;
	int value=0;

	readallwidgets(scrlstrsrc);
	if(FINDRSCGETINT(scrlstrsrc,"ASSOCIATED FILES",&value)) return;
/*
	if(RSL->numincvirs<2) return;
*/
	if((RSL->numincvirs<2) && !RDAstrcmp(inclst->libs[0],"No Include Definitions")) return;
	new_fields=Rmalloc((RSL->numincvirs-1)*sizeof(DFincvir));
	y=0;
	for(x=0,temp=RSL->incvir;x<RSL->numincvirs;++x,++temp)
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
	Rfree(RSL->incvir);
	--RSL->numincvirs;
	RSL->incvir=Rmalloc(RSL->numincvirs*sizeof(DFincvir));
	for(x=0,temp1=RSL->incvir,temp=new_fields;x<RSL->numincvirs;++x,
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
static void quitlist(RDArsrc *scrlstrsrc,RDArsrc *mainrsrc)
{
	changedvalues=FALSE;
	if(scrlstrsrc!=NULL)
	{
		killwindow(scrlstrsrc);
		free_rsrc(scrlstrsrc);
	}
	if(vtypes!=NULL) freeapplib(vtypes);
	if(ltypes!=NULL) freeapplib(ltypes);
	if(contypes!=NULL) freeapplib(contypes);
	if(timtypes!=NULL) freeapplib(timtypes);
	if(special_funcs!=NULL) freeapplib(special_funcs);
	if(inclst!=NULL) freeapplib(inclst);
	if(fmodlst!=NULL) freeapplib(fmodlst);
	if(ffilelst!=NULL) freeapplib(ffilelst);
	if(fkeylst!=NULL) freeapplib(fkeylst);
	if(cmodlst!=NULL) freeapplib(cmodlst);
	if(cfilelst!=NULL) freeapplib(cfilelst);
	if(cfldlst!=NULL) freeapplib(cfldlst);
	if(listdefaults!=NULL) FreeRDAdefault(listdefaults);
}
static void quitlisttest(RDArsrc *scrlstrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(scrlstrsrc);
	if(COMPARE_RSRCS(scrlstrsrc,listdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the SCROLLED LIST DEFINITION.\nDo you want to Save these changes?",savelist,quitlist,FALSE,2,scrlstrsrc,mainrsrc,NULL);
	} else {
		quitlist(scrlstrsrc,mainrsrc);
	}
}
static void savelist(RDArsrc *scrlstrsrc,RDArsrc *mainrsrc)
{
	char *libx=NULL;
	char *modulx=NULL,*errmessage=NULL;
	int x=0;
/*
	char *fmodule=NULL,*ffile=NULL,*fkeyname=NULL;
*/
	int spcfunc=0;

	readallwidgets(scrlstrsrc);
	FINDRSCGETSTRING(scrlstrsrc,"MODULE",&RSL->module);
	FINDRSCGETSTRING(scrlstrsrc,"NAME",&RSL->name);
	if(RSL->module!=NULL && RSL->name!=NULL)
	{
		FINDRSCGETSTRING(scrlstrsrc,"DESCRIPTION",&RSL->desc);
		FINDRSCGETINT(scrlstrsrc,"LIST TYPES",&RSL->type);
		FINDRSCGETSTRING(scrlstrsrc,"SELECT FORMULA",&RSL->select_formula);
		FINDRSCGETINT(scrlstrsrc,"SPECIAL FUNCTIONS",&spcfunc);
/* This is not implemented yet */
		RSL->num=0;
		RSL->initfld=NULL;
/* ---------------------------- */
		if(RSL->list!=NULL)
		{
			if(RSL->list->numlibs<0 || !RDAstrcmp(RSL->list->libs[0],"No Elements Defined"))
			{
				freeapplib(RSL->list);
			} else if(RSL->list->numlibs<1)
			{
				freeapplib(RSL->list);	
			}
		}
		if(special_funcs!=NULL)
		{
			if(spcfunc<special_funcs->numlibs)
			{
				if(!RDAstrcmp(special_funcs->libs[spcfunc],"No Special Function"))
				{
					RSL->special_function=NULL;
				} else {
					RSL->special_function=stralloc(special_funcs->libs[spcfunc]);
				}
			} else {
				RSL->special_function=NULL;
			}
		} else {
			RSL->special_function=NULL;
		}
		FINDRSCGETSTRING(scrlstrsrc,"FORMAT FORMULA",&RSL->format_formula);
		FINDRSCGETSTRING(scrlstrsrc,"UNFORMAT FORMULA",&RSL->unformat_formula);
		FINDRSCGETSTRING(scrlstrsrc,"REFORMAT FORMULA",&RSL->reformat_formula);
		FINDRSCGETINT(scrlstrsrc,"CONNECT TYPES",&RSL->contype);
		FINDRSCGETSTRING(scrlstrsrc,"CONNECT MODULE",&RSL->conmod);
		RSL->confil=NULL;
		RSL->confld=NULL;
		if(RSL->contype<1)
		{
			RSL->contype=0;
			FINDRSCGETINT(scrlstrsrc,"CONNECT FILE LIST",&x);
			if(!RDAstrcmp(cfilelst->libs[x],"No File Definitions"))
			{
				errmessage=Rmalloc(RDAstrlen(cfilelst->libs[x])+151);
				sprintf(errmessage,"The Connect File of [%s] that was selected is an Invalid File Definition.  Please select a valid File Definition.",cfilelst->libs[x]);
				ERRORDIALOG("INVALID CONNECT FILE SELECTED!",errmessage,NULL,FALSE);
				if(errmessage!=NULL) Rfree(errmessage);
				return;
			} 
			RSL->confil=stralloc(cfilelst->libs[x]);
			FINDRSCGETINT(scrlstrsrc,"CONNECT FIELD LIST",&x);
			if(!RDAstrcmp(cfldlst->libs[x],"No Fields Defined"))
			{
				errmessage=Rmalloc(RDAstrlen(cfldlst->libs[x])+151);
				sprintf(errmessage,"The Connect Field Name of [%s] that was selected is an Invalid Field Name.  Please select a valid Field Name.",cfldlst->libs[x]);
	
				ERRORDIALOG("INVALID CONNECT FIELD SELECTED!",errmessage,NULL,FALSE);
				if(errmessage!=NULL) Rfree(errmessage);
				return;
			} 
			RSL->confld=stralloc(cfldlst->libs[x]);
		}
		FINDRSCGETINT(scrlstrsrc,"CONNECT FILE LIST",&x);
		FINDRSCGETINT(scrlstrsrc,"CONNECT FIELD LIST",&x);
/*
		FINDRSCGETSTRING(scrlstrsrc,"CONNECT FILE",&RSL->confil);
		FINDRSCGETSTRING(scrlstrsrc,"CONNECT FIELD",&RSL->confld);
*/
		FINDRSCGETSTRING(scrlstrsrc,"CONNECT FILE",&RSL->confil);
		FINDRSCGETSTRING(scrlstrsrc,"CONNECT FIELD",&RSL->confld);
		FINDRSCGETSTRING(scrlstrsrc,"FROM FIELD",&RSL->ffield);
		FINDRSCGETINT(scrlstrsrc,"CONDITIONAL TYPES",&RSL->vtype);
		FINDRSCGETINT(scrlstrsrc,"TIMING TYPES",&RSL->timing);
		if(RSL->ffinfo!=NULL)
		{
			if(RSL->ffinfo->module!=NULL) Rfree(RSL->ffinfo->module);
			if(RSL->ffinfo->file!=NULL) Rfree(RSL->ffinfo->file);
			if(RSL->ffinfo->keyname!=NULL) Rfree(RSL->ffinfo->keyname);
			Rfree(RSL->ffinfo);
		}
		RSL->ffinfo=Rmalloc(sizeof(DFincvir));
		RSL->ffinfo->module=NULL;
		RSL->ffinfo->file=NULL;
		RSL->ffinfo->keyname=NULL;
		RSL->ffinfo->fileno=(-1);
		if(RSL->type<1)
		{
			RSL->type=0;
			FINDRSCGETINT(scrlstrsrc,"FROM MODULE LIST",&x);
			if(!RDAstrcmp(fmodlst->libs[x],"No Database Libraries"))
			{
				errmessage=Rmalloc(RDAstrlen(fmodlst->libs[x])+151);
				sprintf(errmessage,"The From Module of [%s] that was selected is an Invalid Database Library Module Name.  Please select a valid Database Module Name.",fmodlst->libs[x]);
				ERRORDIALOG("INVALID FROM MODULE SELECTED!",errmessage,NULL,FALSE);
				if(errmessage!=NULL) Rfree(errmessage);
				return;
			} 
			RSL->ffinfo->module=stralloc(fmodlst->libs[x]);
			FINDRSCGETINT(scrlstrsrc,"FROM FILE LIST",&x);
			if(!RDAstrcmp(ffilelst->libs[x],"No File Definitions"))
			{
				errmessage=Rmalloc(RDAstrlen(ffilelst->libs[x])+151);
				sprintf(errmessage,"The From File of [%s] that was selected is an Invalid File Definition.  Please select a valid File Definition.",ffilelst->libs[x]);
				ERRORDIALOG("INVALID FROM FILE SELECTED!",errmessage,NULL,FALSE);
				if(errmessage!=NULL) Rfree(errmessage);
				return;
			} 
			RSL->ffinfo->file=stralloc(ffilelst->libs[x]);
			FINDRSCGETINT(scrlstrsrc,"FROM KEYNAME LIST",&x);
			if(!RDAstrcmp(fkeylst->libs[x],"No Keys Defined"))
			{
				errmessage=Rmalloc(RDAstrlen(fkeylst->libs[x])+151);
				sprintf(errmessage,"The From Key Name of [%s] that was selected is an Invalid Key Name.  Please select a valid Key Name.",fkeylst->libs[x]);
	
				ERRORDIALOG("INVALID FROM KEYNAME SELECTED!",errmessage,NULL,FALSE);
				if(errmessage!=NULL) Rfree(errmessage);
				return;
			} 
			RSL->ffinfo->keyname=stralloc(fkeylst->libs[x]);
		}
/*
		FINDRSCGETSTRING(scrlstrsrc,"FROM MODULE",&fmodule);
		FINDRSCGETSTRING(scrlstrsrc,"FROM FILE",&ffile);
		FINDRSCGETSTRING(scrlstrsrc,"FROM KEYNAME",&fkeyname);
		RSL->ffinfo->module=stralloc(fmodule);
		RSL->ffinfo->file=stralloc(ffile);
		RSL->ffinfo->keyname=stralloc(fkeyname);
*/
		modulx=adddashes(RSL->module);	
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modulx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modulx);
#endif
		if(writeScrolledListbin(libx,RSL))
		{
			ERRORDIALOG("Cannot Write Scrolled List","Error attempting to write the scrolled list.  Check the permissions on the Scrolled List Binary Library, and retry.  Call your installer.",NULL,FALSE);
		}
/*
		if(fmodule!=NULL) Rfree(fmodule);
		if(ffile!=NULL) Rfree(ffile);
		if(fkeyname!=NULL) Rfree(fkeyname);
*/
		if(libx!=NULL) Rfree(libx);
		FreeRDAScrolledList(RSL);
	}
	doSlistfldlistcb(mainrsrc);
/*
	setliblist(mainrsrc);
*/
	getSlistmodulescb(mainrsrc);
	quitlist(scrlstrsrc,mainrsrc);
}
static void changecontype(RDArsrc *r)
{
	int selectedc=0,x=0;

	FINDRSCGETINT(r,"CONNECT TYPES",&selectedc);
	if(selectedc<1)
	{
		FINDRSCSETEDITABLE(r,"REFORMAT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(r,"REFORMAT FORMULA",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD REFORMAT VALUE",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD REFORMAT OPERATOR",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD REFORMAT GROUPER",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD REFORMAT CONTROL",FALSE);

		FINDRSCSETEDITABLE(r,"CONNECT MODULE LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"CONNECT MODULE LIST",TRUE);
		FINDRSCSETEDITABLE(r,"CONNECT FILE LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"CONNECT FILE LIST",TRUE);
		FINDRSCSETEDITABLE(r,"CONNECT FIELD LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"CONNECT FIELD LIST",TRUE);
		cmodlst=getmodulelist(r,"CONNECT MODULE LIST",cmodlst);
		for(x=0;x<cmodlst->numlibs;++x)
		{
			if(!RDAstrcmp(RSL->conmod,cmodlst->libs[x])) break;
		}
		if(x>=cmodlst->numlibs) x=0;
		FINDRSCLISTAPPlib(r,"CONNECT MODULE LIST",x,cmodlst);
		condofilelist(r);
		for(x=0;x<cfilelst->numlibs;++x)
		{
			if(!RDAstrcmp(RSL->confil,cfilelst->libs[x])) break;
		}
		if(x>=cfilelst->numlibs) x=0;
		FINDRSCLISTAPPlib(r,"CONNECT FILE LIST",x,cfilelst);
		condofldlist(r);
		for(x=0;x<cfldlst->numlibs;++x)
		{
			if(!RDAstrcmp(RSL->confld,cfldlst->libs[x])) break;
		}
		if(x>=cfldlst->numlibs) x=0;
		FINDRSCLISTAPPlib(r,"CONNECT FIELD LIST",x,cfldlst);
	} else {
		if(selectedc==3)
		{
			FINDRSCSETEDITABLE(r,"REFORMAT FORMULA",TRUE);
			FINDRSCSETSENSITIVE(r,"REFORMAT FORMULA",TRUE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT VALUE",TRUE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT OPERATOR",TRUE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT GROUPER",TRUE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT CONTROL",TRUE);
		} else {
			FINDRSCSETEDITABLE(r,"REFORMAT FORMULA",FALSE);
			FINDRSCSETSENSITIVE(r,"REFORMAT FORMULA",FALSE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT VALUE",FALSE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT OPERATOR",FALSE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT GROUPER",FALSE);
			FINDRSCSETEDITABLE(r,"LOAD REFORMAT CONTROL",FALSE);
		}
		if(cmodlst!=NULL) freeapplib(cmodlst);
		cmodlst=APPlibNEW();
		addAPPlib(cmodlst,"No Database Libraries");
		if(cfilelst!=NULL) freeapplib(cfilelst);
		cfilelst=APPlibNEW();
		addAPPlib(cfilelst,"No File Definitions");
		if(cfldlst!=NULL) freeapplib(cfldlst);
		cfldlst=APPlibNEW();
		addAPPlib(cfldlst,"No Fields Defined");
		FINDRSCLISTAPPlib(r,"CONNECT MODULE LIST",0,cmodlst);
		FINDRSCLISTAPPlib(r,"CONNECT FILE LIST",0,cfilelst);
		FINDRSCLISTAPPlib(r,"CONNECT FIELD LIST",0,cfldlst);
		FINDRSCSETEDITABLE(r,"CONNECT MODULE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"CONNECT MODULE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"CONNECT FILE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"CONNECT FILE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"CONNECT FIELD LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"CONNECT FIELD LIST",FALSE);
	}
	updatersrc(r,"CONNECT MODULE LIST");
	updatersrc(r,"CONNECT FILE LIST");
	updatersrc(r,"CONNECT FIELD LIST");
}
static void changelisttype(RDArsrc *r,DFincvir *ffinfo)
{
	int type=0,x=0;

	FINDRSCGETINT(r,"LIST TYPES",&type);
	if(type<1)
	{
		FINDRSCSETEDITABLE(r,"FROM MODULE LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"FROM MODULE LIST",TRUE);
		FINDRSCSETEDITABLE(r,"FROM FILE LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"FROM FILE LIST",TRUE);
		FINDRSCSETEDITABLE(r,"FROM KEYNAME LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"FROM KEYNAME LIST",TRUE);
		FINDRSCSETEDITABLE(r,"SELECT FORMULA",TRUE);
		FINDRSCSETSENSITIVE(r,"SELECT FORMULA",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT VALUE",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT OPERATOR",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT GROUPER",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT CONTROL",TRUE);
		FINDRSCSETEDITABLE(r,"FORMAT FORMULA",TRUE);
		FINDRSCSETSENSITIVE(r,"FORMAT FORMULA",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT VALUE",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT OPERATOR",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT GROUPER",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT CONTROL",TRUE);
		FINDRSCSETEDITABLE(r,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETEDITABLE(r,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETEDITABLE(r,"DELETE ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(r,"DELETE ELEMENT",FALSE);
		FINDRSCSETEDITABLE(r,"SELECT ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(r,"SELECT ELEMENT",FALSE);
		x=0;
		fmodlst=getmodulelist(r,"FROM MODULE LIST",fmodlst);
		if(ffinfo!=NULL)
		{
			for(x=0;x<fmodlst->numlibs;++x)
			{
				if(!RDAstrcmp(ffinfo->module,fmodlst->libs[x])) break;
			}
			if(x>=fmodlst->numlibs) x=0;
		}
		FINDRSCLISTAPPlib(r,"FROM MODULE LIST",x,fmodlst);
		fromdofilelist(r);
		if(ffinfo!=NULL)
		{
			for(x=0;x<ffilelst->numlibs;++x)
			{
				if(!RDAstrcmp(ffinfo->file,ffilelst->libs[x])) break;
			}
			if(x>=ffilelst->numlibs) x=0;
		}
		FINDRSCLISTAPPlib(r,"FROM FILE LIST",x,ffilelst);
		fromdokeylist(r);
		if(ffinfo!=NULL)
		{
			for(x=0;x<fkeylst->numlibs;++x)
			{
				if(!RDAstrcmp(ffinfo->keyname,fkeylst->libs[x])) break;
			}
		}
		if(x>=fkeylst->numlibs) x=0;
		FINDRSCLISTAPPlib(r,"FROM KEYNAME LIST",x,fkeylst);
	} else if((type==1) || (type==3))
	{
		if(fmodlst!=NULL) freeapplib(fmodlst);
		fmodlst=APPlibNEW();
		addAPPlib(fmodlst,"No Database Libraries");
		if(ffilelst!=NULL) freeapplib(ffilelst);
		ffilelst=APPlibNEW();
		addAPPlib(ffilelst,"No File Definitions");
		if(fkeylst!=NULL) freeapplib(fkeylst);
		fkeylst=APPlibNEW();
		addAPPlib(fkeylst,"No Keys Defined");
		FINDRSCLISTAPPlib(r,"FROM MODULE LIST",0,fmodlst);
		FINDRSCLISTAPPlib(r,"FROM FILE LIST",0,ffilelst);
		FINDRSCLISTAPPlib(r,"FROM KEYNAME LIST",0,fkeylst);
		FINDRSCSETEDITABLE(r,"FROM MODULE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM MODULE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"FROM FILE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM FILE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETEDITABLE(r,"SELECT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(r,"SELECT FORMULA",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT VALUE",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT OPERATOR",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT GROUPER",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT CONTROL",FALSE);
/*
		FINDRSCSETEDITABLE(r,"FORMAT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(r,"FORMAT FORMULA",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT VALUE",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT OPERATOR",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT GROUPER",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT CONTROL",FALSE);
*/
		FINDRSCSETEDITABLE(r,"ADD ELEMENT ABOVE",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD ELEMENT ABOVE",TRUE);
		FINDRSCSETEDITABLE(r,"ADD ELEMENT BELOW",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD ELEMENT BELOW",TRUE);
		FINDRSCSETEDITABLE(r,"DELETE ELEMENT",TRUE);
		FINDRSCSETSENSITIVE(r,"DELETE ELEMENT",TRUE);
		FINDRSCSETEDITABLE(r,"SELECT ELEMENT",TRUE);
		FINDRSCSETSENSITIVE(r,"SELECT ELEMENT",TRUE);
	} else if(type==2)
	{
		if(fmodlst!=NULL) freeapplib(fmodlst);
		fmodlst=APPlibNEW();
		addAPPlib(fmodlst,"No Database Libraries");
		if(ffilelst!=NULL) freeapplib(ffilelst);
		ffilelst=APPlibNEW();
		addAPPlib(ffilelst,"No File Definitions");
		if(fkeylst!=NULL) freeapplib(fkeylst);
		fkeylst=APPlibNEW();
		addAPPlib(fkeylst,"No Keys Defined");
		FINDRSCLISTAPPlib(r,"FROM MODULE LIST",0,fmodlst);
		FINDRSCLISTAPPlib(r,"FROM FILE LIST",0,ffilelst);
		FINDRSCLISTAPPlib(r,"FROM KEYNAME LIST",0,fkeylst);
		FINDRSCSETEDITABLE(r,"FROM MODULE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM MODULE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"FROM FILE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM FILE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETEDITABLE(r,"SELECT FORMULA",TRUE);
		FINDRSCSETSENSITIVE(r,"SELECT FORMULA",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT VALUE",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT OPERATOR",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT GROUPER",TRUE);
		FINDRSCSETEDITABLE(r,"LOAD SELECT CONTROL",TRUE);
/*
		FINDRSCSETEDITABLE(r,"FORMAT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(r,"FORMAT FORMULA",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT VALUE",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT OPERATOR",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT GROUPER",FALSE);
		FINDRSCSETEDITABLE(r,"LOAD FORMAT CONTROL",FALSE);
*/
		FINDRSCSETEDITABLE(r,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETEDITABLE(r,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETEDITABLE(r,"DELETE ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(r,"DELETE ELEMENT",FALSE);
		FINDRSCSETEDITABLE(r,"SELECT ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(r,"SELECT ELEMENT",FALSE);
	}
	updatersrc(r,"FROM MODULE LIST");
	updatersrc(r,"FROM FILE LIST");
	updatersrc(r,"FROM KEYNAME LIST");
}
static APPlib *getspecial_funcs(APPlib *spc_funcs)
{
	if(spc_funcs!=NULL) freeapplib(spc_funcs);
	spc_funcs=APPlibNEW();
	addAPPlib(spc_funcs,"No Special Function");
	addAPPlib(spc_funcs,"DEFINITION TYPES");
	addAPPlib(spc_funcs,"OUTPUT DEVICES");
	addAPPlib(spc_funcs,"MODULES AVAILABLE");
	addAPPlib(spc_funcs,"FILES AVAILABLE");
	addAPPlib(spc_funcs,"FIELDS AVAILABLE");
	addAPPlib(spc_funcs,"PROCESSES AVAILABLE");
	addAPPlib(spc_funcs,"MATRIXES AVAILABLE");
	addAPPlib(spc_funcs,"TABLES AVAILABLE");
	addAPPlib(spc_funcs,"DIRECTORY CONTENTS");
	addAPPlib(spc_funcs,"PDF(S) IN DIRECTORY");
	addAPPlib(spc_funcs,"TXT(S) IN DIRECTORY");
	addAPPlib(spc_funcs,"DIRECTORY RECURSIVELY");
	addAPPlib(spc_funcs,"DIRECTORIES ONLY");
	addAPPlib(spc_funcs,"NO DIRECTORIES");
	addAPPlib(spc_funcs,"WORKSTATION DIRECTORY CONTENTS (SFTP)");
	return(spc_funcs);
}
void printSL(RDArsrc *parent,RDAScrolledList *s)
{
        SEERDAScrolledList(s,NULL,doheaders);
}
static void scrolledlistscreen(RDArsrc *mainrsrc,char* modulx,char *name,
	int vtype,int numincvirs,DFincvir *incvir,int type,
	char *select_formula,char *special_function,
	short numinits,RDAconfld *initfld,char *ffield,short occurrence,
	DFincvir *ffinfo,int contype,char *conmod,char *confil,char *confld,
	short con_occurrence,APPlib *list,char *format_formula,
	char *unformat_formula,char *reformat_formula,char *desc,int timing)
{
	int x=0;
	RDArsrc *scrlstrsrc=NULL;

	scrlstrsrc=RDArsrcNEW(module,"MAINTAIN SCROLLED LIST");
	if(vtypes!=NULL) freeapplib(vtypes);
	vtypes=APPlibNEW();
	addAPPlib(vtypes,"When ALL Files Present");
	addAPPlib(vtypes,"When One of Many Present");
	if(ltypes!=NULL) freeapplib(ltypes);
	ltypes=APPlibNEW();
	addAPPlib(ltypes,"From File");
	addAPPlib(ltypes,"Parsed List");
	addAPPlib(ltypes,"Special");
	addAPPlib(ltypes,"Computed List");
	if(contypes!=NULL) freeapplib(contypes);
	contypes=APPlibNEW();
	addAPPlib(contypes,"Normal File Field");
	addAPPlib(contypes,"Add Edit Widget / Special Field");
	addAPPlib(contypes,"Virtual Fields");
	addAPPlib(contypes,"Range Screen / Search Browse");
	if(timtypes!=NULL) freeapplib(timtypes);
	timtypes=APPlibNEW();
	addAPPlib(timtypes,"Initial Only");
	addAPPlib(timtypes,"Initial & Update Virtuals");
	addAPPlib(timtypes,"Update Virtuals Only");
	special_funcs=getspecial_funcs(special_funcs);
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
	addlstrsrc(scrlstrsrc,"CONDITIONAL TYPES",&vtype,TRUE,NULL,vtypes->numlibs,&vtypes->libs,NULL);
	addlstrsrc(scrlstrsrc,"LIST TYPES",&type,TRUE,changelisttype,ltypes->numlibs,&ltypes->libs,ffinfo);
	addlstrsrc(scrlstrsrc,"CONNECT TYPES",&contype,TRUE,changecontype,contypes->numlibs,&contypes->libs,NULL);
	addlstrsrc(scrlstrsrc,"TIMING TYPES",&timing,TRUE,NULL,timtypes->numlibs,&timtypes->libs,NULL);
	if((x=FINDAPPLIBELEMENT(special_funcs,special_function))==(-1))
	{
		x=0;
	}
	addlstrsrc(scrlstrsrc,"SPECIAL FUNCTIONS",&x,TRUE,NULL,special_funcs->numlibs,&special_funcs->libs,NULL);
	if(list==NULL)
	{
		list=APPlibNEW();
		addAPPlib(list,"No Elements Defined");
	}
	addlstrsrc(scrlstrsrc,"LIST ELEMENTS",&list,TRUE,NULL,list->numlibs,&list->libs,NULL);
	addbtnrsrc(scrlstrsrc,"ADD ELEMENT ABOVE",TRUE,add_element_above,NULL);
	addbtnrsrc(scrlstrsrc,"ADD ELEMENT BELOW",TRUE,add_element_below,NULL);
	addbtnrsrc(scrlstrsrc,"DELETE ELEMENT",TRUE,delete_element,NULL);
	addbtnrsrc(scrlstrsrc,"SELECT ELEMENT",TRUE,edit_element,NULL);
	addsctrsrc(scrlstrsrc,"SELECT FORMULA",0,(select_formula!=NULL?select_formula:""),TRUE);
	addbtnrsrc(scrlstrsrc,"LOAD SELECT VALUE",TRUE,loadvalue,"SELECT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD SELECT OPERATOR",TRUE,loadoperand,"SELECT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD SELECT GROUPER",TRUE,loadgrouper,"SELECT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD SELECT CONTROL",TRUE,loadcontrol,"SELECT FORMULA");
	addsctrsrc(scrlstrsrc,"FORMAT FORMULA",0,(format_formula!=NULL?format_formula:""),TRUE);
	addbtnrsrc(scrlstrsrc,"LOAD FORMAT VALUE",TRUE,loadvalue,"FORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD FORMAT OPERATOR",TRUE,loadoperand,"FORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD FORMAT GROUPER",TRUE,loadgrouper,"FORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD FORMAT CONTROL",TRUE,loadcontrol,"FORMAT FORMULA");
	if(fmodlst!=NULL) freeapplib(fmodlst);
	fmodlst=APPlibNEW();
	addAPPlib(fmodlst,"No Database Libraries");
	addlstrsrc(scrlstrsrc,"FROM MODULE LIST",&x,TRUE,fromdofilelistcb,fmodlst->numlibs,&fmodlst->libs,NULL);
	if(ffilelst!=NULL) freeapplib(ffilelst);
	ffilelst=APPlibNEW();
	addAPPlib(ffilelst,"No File Definitions");
	addlstrsrc(scrlstrsrc,"FROM FILE LIST",&x,TRUE,fromdokeylist,ffilelst->numlibs,&ffilelst->libs,NULL);
	if(fkeylst!=NULL) freeapplib(fkeylst);
	fkeylst=APPlibNEW();
	addAPPlib(fkeylst,"No Keys Defined");
	addlstrsrc(scrlstrsrc,"FROM KEYNAME LIST",&x,TRUE,NULL,fkeylst->numlibs,&fkeylst->libs,NULL);
	if(type<1)
	{
		x=0;
		fmodlst=getmodulelist(scrlstrsrc,"FROM MODULE LIST",fmodlst);
		if(ffinfo!=NULL)
		{
			for(x=0;x<fmodlst->numlibs;++x)
			{
				if(!RDAstrcmp(ffinfo->module,fmodlst->libs[x])) break;
			}
			if(x>=fmodlst->numlibs) x=0;
		}
		FINDRSCSETINT(scrlstrsrc,"FROM MODULE LIST",x);
		fromdofilelist(scrlstrsrc);
		if(ffinfo!=NULL)
		{
			for(x=0;x<ffilelst->numlibs;++x)
			{
				if(!RDAstrcmp(ffinfo->file,ffilelst->libs[x])) break;
			}
			if(x>=ffilelst->numlibs) x=0;
		}
		FINDRSCSETINT(scrlstrsrc,"FROM FILE LIST",x);
		fromdokeylist(scrlstrsrc);
		if(ffinfo!=NULL)
		{
			for(x=0;x<fkeylst->numlibs;++x)
			{
				if(!RDAstrcmp(ffinfo->keyname,fkeylst->libs[x])) break;
			}
			if(x>=fkeylst->numlibs) x=0;
		}
		FINDRSCSETINT(scrlstrsrc,"FROM KEYNAME LIST",x);
		FINDRSCSETEDITABLE(scrlstrsrc,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"DELETE ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"DELETE ELEMENT",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"SELECT ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"SELECT ELEMENT",FALSE);
	} else if(type==1)
	{
		FINDRSCSETEDITABLE(scrlstrsrc,"FROM MODULE LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FROM MODULE LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"FROM FILE LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FROM FILE LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"SELECT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"SELECT FORMULA",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT VALUE",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT OPERATOR",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT GROUPER",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT CONTROL",FALSE);
/*
		FINDRSCSETEDITABLE(scrlstrsrc,"FORMAT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FORMAT FORMULA",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT VALUE",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT OPERATOR",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT GROUPER",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT CONTROL",FALSE);
*/
		FINDRSCSETEDITABLE(scrlstrsrc,"ADD ELEMENT ABOVE",TRUE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"ADD ELEMENT ABOVE",TRUE);
		FINDRSCSETEDITABLE(scrlstrsrc,"ADD ELEMENT BELOW",TRUE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"ADD ELEMENT BELOW",TRUE);
		FINDRSCSETEDITABLE(scrlstrsrc,"DELETE ELEMENT",TRUE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"DELETE ELEMENT",TRUE);
		FINDRSCSETEDITABLE(scrlstrsrc,"SELECT ELEMENT",TRUE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"SELECT ELEMENT",TRUE);
	} else if(type==2)
	{
		FINDRSCSETEDITABLE(scrlstrsrc,"FROM MODULE LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FROM MODULE LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"FROM FILE LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FROM FILE LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FROM KEYNAME LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"SELECT FORMULA",TRUE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"SELECT FORMULA",TRUE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT VALUE",TRUE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT OPERATOR",TRUE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT GROUPER",TRUE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD SELECT CONTROL",TRUE);
/*
		FINDRSCSETEDITABLE(scrlstrsrc,"FORMAT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"FORMAT FORMULA",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT VALUE",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT OPERATOR",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT GROUPER",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"LOAD FORMAT CONTROL",FALSE);
*/
		FINDRSCSETEDITABLE(scrlstrsrc,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"ADD ELEMENT ABOVE",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"ADD ELEMENT BELOW",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"DELETE ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"DELETE ELEMENT",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"SELECT ELEMENT",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"SELECT ELEMENT",FALSE);
	}
/*
		addstdrsrc(scrlstrsrc,"FROM MODULE",VARIABLETEXT,0,(ffinfo->module!=NULL?ffinfo->module:""),TRUE);
		addstdrsrc(scrlstrsrc,"FROM FILE",VARIABLETEXT,0,(ffinfo->file!=NULL?ffinfo->file:""),TRUE);
		addstdrsrc(scrlstrsrc,"FROM KEYNAME",VARIABLETEXT,0,(ffinfo->keyname!=NULL?ffinfo->keyname:""),TRUE);
	} else {
		addstdrsrc(scrlstrsrc,"FROM MODULE",VARIABLETEXT,0,"",TRUE);
		addstdrsrc(scrlstrsrc,"FROM FILE",VARIABLETEXT,0,"",TRUE);
		addstdrsrc(scrlstrsrc,"FROM KEYNAME",VARIABLETEXT,0,"",TRUE);
	}
*/
	addstdrsrc(scrlstrsrc,"FROM FIELD",VARIABLETEXT,0,(ffield!=NULL?ffield:""),TRUE);
	addstdrsrc(scrlstrsrc,"CONNECT MODULE",VARIABLETEXT,0,(conmod!=NULL?conmod:""),TRUE);
	if(cmodlst!=NULL) freeapplib(cmodlst);
	cmodlst=APPlibNEW();
	addAPPlib(cmodlst,"No Database Libraries");
	addlstrsrc(scrlstrsrc,"CONNECT MODULE LIST",&x,TRUE,condofilelistcb,cmodlst->numlibs,&cmodlst->libs,NULL);
	if(cfilelst!=NULL) freeapplib(cfilelst);
	cfilelst=APPlibNEW();
	addAPPlib(cfilelst,"No File Definitions");
	addlstrsrc(scrlstrsrc,"CONNECT FILE LIST",&x,TRUE,condofldlist,cfilelst->numlibs,&cfilelst->libs,NULL);
	if(cfldlst!=NULL) freeapplib(cfldlst);
	cfldlst=APPlibNEW();
	addAPPlib(cfldlst,"No Fields Defined");
	addlstrsrc(scrlstrsrc,"CONNECT FIELD LIST",&x,TRUE,NULL,cfldlst->numlibs,&cfldlst->libs,NULL);
/*
	addstdrsrc(scrlstrsrc,"CONNECT FILE",VARIABLETEXT,0,(confil!=NULL?confil:""),TRUE);
	addstdrsrc(scrlstrsrc,"CONNECT FIELD",VARIABLETEXT,0,(confld!=NULL?confld:""),TRUE);
*/
	addsctrsrc(scrlstrsrc,"DESCRIPTION",0,(desc!=NULL?desc:""),TRUE);

	if(contype<1)
	{
		cmodlst=getmodulelist(scrlstrsrc,"CONNECT MODULE LIST",cmodlst);
		for(x=0;x<cmodlst->numlibs;++x)
		{
			if(!RDAstrcmp(conmod,cmodlst->libs[x])) break;
		}
		if(x>=cmodlst->numlibs) x=0;
		FINDRSCSETINT(scrlstrsrc,"CONNECT MODULE LIST",x);
		condofilelist(scrlstrsrc);
		for(x=0;x<cfilelst->numlibs;++x)
		{
			if(!RDAstrcmp(confil,cfilelst->libs[x])) break;
		}
		if(x>=cfilelst->numlibs) x=0;
		FINDRSCSETINT(scrlstrsrc,"CONNECT FILE LIST",x);
		condofldlist(scrlstrsrc);
		for(x=0;x<cfldlst->numlibs;++x)
		{
			if(!RDAstrcmp(confld,cfldlst->libs[x])) break;
		}
		if(x>=cfldlst->numlibs) x=0;
		FINDRSCSETINT(scrlstrsrc,"CONNECT FIELD LIST",x);
	} else {
		FINDRSCSETEDITABLE(scrlstrsrc,"CONNECT MODULE LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"CONNECT MODULE LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"CONNECT FILE LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"CONNECT FILE LIST",FALSE);
		FINDRSCSETEDITABLE(scrlstrsrc,"CONNECT FIELD LIST",FALSE);
		FINDRSCSETSENSITIVE(scrlstrsrc,"CONNECT FIELD LIST",FALSE);
	}
	addsctrsrc(scrlstrsrc,"UNFORMAT FORMULA",0,(unformat_formula!=NULL?unformat_formula:""),TRUE);
	addbtnrsrc(scrlstrsrc,"LOAD UNFORMAT VALUE",TRUE,loadvalue,"UNFORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD UNFORMAT OPERATOR",TRUE,loadoperand,"UNFORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD UNFORMAT GROUPER",TRUE,loadgrouper,"UNFORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD UNFORMAT CONTROL",TRUE,loadcontrol,"UNFORMAT FORMULA");
	addsctrsrc(scrlstrsrc,"REFORMAT FORMULA",0,(reformat_formula!=NULL?reformat_formula:""),TRUE);
	addbtnrsrc(scrlstrsrc,"LOAD REFORMAT VALUE",TRUE,loadvalue,"REFORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD REFORMAT OPERATOR",TRUE,loadoperand,"REFORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD REFORMAT GROUPER",TRUE,loadgrouper,"REFORMAT FORMULA");
	addbtnrsrc(scrlstrsrc,"LOAD REFORMAT CONTROL",TRUE,loadcontrol,"REFORMAT FORMULA");
	addrfcbrsrc(scrlstrsrc,"SAVE",TRUE,savelist,mainrsrc);
	addrfcbrsrc(scrlstrsrc,"QUIT",TRUE,quitlisttest,mainrsrc);
	addbtnrsrc(scrlstrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(scrlstrsrc,"PRINT DEFINITION",TRUE,printSL,RSL);
	addbtnrsrc(scrlstrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedvalues=FALSE;
	APPmakescrn(scrlstrsrc,TRUE,quitlist,mainrsrc,FALSE);
	listdefaults=GetDefaults(scrlstrsrc);
}
static void oklist(RDArsrc *mainrsrc)
{
	int selected=0,selectedl=0;
	char *modulx=NULL,*libx=NULL;
	short gotten=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LISTS AVAILABLE",&selectedl)) return;
	if(RSL!=NULL) FreeRDAScrolledList(RSL); 
	if(RDAstrcmp(lstavl->libs[selectedl],"No Scrolled Lists")) 
	{
		modulx=adddashes(liblst->libs[selected]);	
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modulx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modulx);
#endif
		RSL=RDAScrolledListNew(liblst->libs[selected],lstavl->libs[selectedl]);
		gotten=getScrolledListbin(libx,RSL);
		if(libx!=NULL) Rfree(libx);
		if(modulx!=NULL) Rfree(modulx);
		if(gotten==(-1))
		{
			ERRORDIALOG("Cannot Read Scrolled List","Error in permissions, the user cannot read the scrolled list binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			return;
		}
	}
	if(RSL==NULL)
	{
		RSL=RDAScrolledListNew(liblst->libs[selected],NULL);
	}
	scrolledlistscreen(mainrsrc,RSL->module,RSL->name,RSL->vtype,
		RSL->numincvirs,RSL->incvir,RSL->type,
		RSL->select_formula,RSL->special_function,
		RSL->num,RSL->initfld,RSL->ffield,RSL->occurrence,RSL->ffinfo,
		RSL->contype,RSL->conmod,RSL->confil,RSL->confld,
		RSL->con_occurrence,RSL->list,RSL->format_formula,
		RSL->unformat_formula,RSL->reformat_formula,
		RSL->desc,RSL->timing);
}
static void newlist(RDArsrc *mainrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(RSL!=NULL) FreeRDAScrolledList(RSL);
	RSL=RDAScrolledListNew(liblst->libs[selected],NULL);
	scrolledlistscreen(mainrsrc,RSL->module,RSL->name,RSL->vtype,
		RSL->numincvirs,RSL->incvir,RSL->type,
		RSL->select_formula,RSL->special_function,
		RSL->num,RSL->initfld,RSL->ffield,RSL->occurrence,RSL->ffinfo,
		RSL->contype,RSL->conmod,RSL->confil,RSL->confld,
		RSL->con_occurrence,RSL->list,RSL->format_formula,
		RSL->unformat_formula,RSL->reformat_formula,
		RSL->desc,RSL->timing);
}
static void quitdflist(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(liblst!=NULL) freeapplib(liblst);
	if(modlst!=NULL) freeapplib(modlst);
	if(lstavl!=NULL) freeapplib(lstavl);
	if(incdefaults!=NULL) FreeRDAdefault(incdefaults);
	if(listdefaults!=NULL) FreeRDAdefault(listdefaults);
	if(elementdefaults!=NULL) FreeRDAdefault(elementdefaults);
	ShutdownSubsystems();
}
/*
static void dolistentries(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"LISTS AVAILABLE",&selectedf);
	if(lstavl!=NULL) freeapplib(lstavl);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(liblst->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,liblst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,liblst->libs[selected]);
#endif
	lstavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(lstavl==NULL)
	{
		lstavl=APPlibNEW();
		addAPPlib(lstavl,"No Scrolled Lists");
	}
	if(selectedf>=lstavl->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LISTS AVAILABLE",selectedf,lstavl))
		updatersrc(mainrsrc,"LISTS AVAILABLE");
	setvaluesx(mainrsrc);
}
static void setliblist(RDArsrc *mainrsrc)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(liblst!=NULL) freeapplib(liblst);
	liblst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.LST",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(liblst,tmp);
	}
	if(liblst->numlibs<1)
	{
		addAPPlib(liblst,"No Scrolled Libraries");
		selectedm=0;
	} else {
		SORTAPPlib(liblst);
		if(selectedm>=liblst->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selectedm,liblst))
	{
		updatersrc(mainrsrc,"MODULE LIST");
	}
	dolistentries(mainrsrc);
}
*/
static void setvaluesx(RDArsrc *mainrsrc,char *modname,char *listname)
{
	char *temp=NULL,*temp2=NULL,*temp3=NULL;
	char *libx=NULL;

	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modname);
#endif
		if(listname!=NULL)
		{
			if(!RDAstrcmp(listname,"No Scrolled Lists"))
			{
				temp=NULL;
			} else {
				temp=getScrolledListdesc(libx,listname);
			}
		} else {
			temp3=stralloc(lstavl->libs[0]);
			if(!RDAstrcmp(temp3,"No Scrolled Lists"))
			{
				temp=NULL;
			} else {
				temp=getScrolledListdesc(libx,temp3);
			}
		}
	} else {
		temp2=stralloc(liblst->libs[0]);
		temp3=stralloc(lstavl->libs[0]);
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp2)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,temp2);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,temp2);
#endif
		if(!RDAstrcmp(temp3,"No Scrolled Lists"))
		{
			temp=NULL;
		} else {
			temp=getScrolledListdesc(libx,temp3);
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
	if(FINDRSCGETINT(mainrsrc,"LISTS AVAILABLE",&selected)) return;
	setvaluesx(mainrsrc,liblst->libs[selectedm],lstavl->libs[selected]);
}
static int doSlistfldlist(RDArsrc *mainrsrc,char *modname,char *listname)
{
	int x=0,selectedf=0;
	char *libx=NULL;

	if(lstavl!=NULL) freeapplib(lstavl);
	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modname);
#endif
	} else {
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(liblst->libs[0])+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,liblst->libs[0]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,liblst->libs[0]);
#endif
	}
	lstavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(lstavl==NULL)
	{
		lstavl=APPlibNEW();
		addAPPlib(lstavl,"No Scrolled Lists");
	}
	if(listname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(lstavl,listname))==(-1))
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
		if(!FINDRSCLISTAPPlib(mainrsrc,"LISTS AVAILABLE",selectedf,lstavl))
		{
			updatersrc(mainrsrc,"LISTS AVAILABLE");
		}
	}
	return(selectedf);
}
static void doSlistfldlistcb(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *temp=NULL,*temp2=NULL;
	int x=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"LISTS AVAILABLE",&selectedf)) return;
	temp=stralloc(liblst->libs[selected]);
	temp2=stralloc(lstavl->libs[selectedf]);
	x=doSlistfldlist(mainrsrc,temp,temp2);
/*
	setvaluesx(mainrsrc,temp,temp2);
*/
	setvaluesx(mainrsrc,temp,lstavl->libs[x]);
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
}
static int getSlistmodules(RDArsrc *mainrsrc,char *modname)
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
	for(x=0;findfile(dirx,"*.LST",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(liblst,tmp);
	}
	if(liblst->numlibs<1)
	{
		addAPPlib(liblst,"No Scrolled List Libraries");
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
static void getSlistmodulescb(RDArsrc *mainrsrc)
{
	int selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	temp=stralloc(liblst->libs[selectedm]);
	getSlistmodules(mainrsrc,temp);
	doSlistfldlistcb(mainrsrc);
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
static void deleteSlist(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulx=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"LISTS AVAILABLE",&s)) return;
	modulx=adddashes(liblst->libs[m]);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulx)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modulx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modulx);
#endif
	if(deletelibbin(libx,lstavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Scrolled List","Error attempting to delete scrolled list.  Check the permissions on the Scrolled List Binary Library, and retry.  Call your installer.",NULL,FALSE);
	} else {
		getSlistmodulescb(mainrsrc);
	}
	if(libx!=NULL) Rfree(libx);
	if(modulx!=NULL) Rfree(modulx);
}
static void deletelisttest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(lstavl->libs[s],"No Scrolled Lists")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE SCROLLED LIST!","Are you sure you want to delete this scrolled list definition?",deleteSlist,NULL,FALSE,mainrsrc);
}
/*
static void setvaluesx(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL,*modulx=NULL,*libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"LISTS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(lstavl->libs[selected],"No Scrolled Lists"))
	{
		temp=NULL;
	} else {
		modulx=adddashes(liblst->libs[selectedm]);	
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,modulx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,modulx);
#endif
		temp=getScrolledListdesc(libx,lstavl->libs[selected]);
		if(libx!=NULL) Rfree(libx);
		if(modulx!=NULL) Rfree(modulx);
	}
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
	{
		updatersrc(mainrsrc,"DESCRIPTION");
	}
	if(temp!=NULL) Rfree(temp);
}
*/
static void print_lists(RDArsrc *prsrc,RDArsrc *mainrsrc)
{
	RDA_PFILE *fp=NULL;
	int pages=1,numpagelines=1;
	int x=0,selected=0;
	char *tmpx=NULL,*libx=NULL,*odevice=NULL,print_style=TRUE;
	RDAScrolledList *d=NULL;
	APPlib *listnames=NULL;
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
	sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,tmpx);
#endif
	listnames=getlibnames(libx,FALSE);
	if(listnames!=NULL)
	{
		fp=RDA_popen(odevice,"w");
		if(fp==NULL)
		{
			prterr("Error Couldn't open device.");
			if(odevice!=NULL) Rfree(odevice);
			if(listnames!=NULL) freeapplib(listnames);
			if(tmpx!=NULL) Rfree(tmpx);
			if(libx!=NULL) Rfree(libx);
			return;
		} 
		SelectPrintType(odevice,print_style,fp);
		if(odevice!=NULL) Rfree(odevice);
		for(x=0;x<listnames->numlibs;++x)
		{
			d=RDAScrolledListNew(liblst->libs[selected],listnames->libs[x]);
			gotbin=getScrolledListbin(libx,d);
			if(gotbin!=(-1))
			{
        			SEERDAScrolledList_detail(d,fp,doheaders,&pages,&numpagelines);
				RDA_fprintf(fp,"\f\r\n");pages=1;numpagelines=1;
			}
			FreeRDAScrolledList(d);
			d=NULL;
		}
		RDA_pclose(fp);
		if(listnames!=NULL) freeapplib(listnames);
	} else {
		prterr("Error No Scrolled Lists Available.");
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
}
static void quit_print_lists(RDArsrc *prsrc)
{
	free_rsrc(prsrc);
}
static void print_listscb(RDArsrc *mainrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,print_lists,mainrsrc);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_lists,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_lists(prsrc);
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
	char *modname=NULL,*listname=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE SCROLLED LISTS")) 
	{
		return;
	}
	if(argc>2)
	{
		modname=stralloc(argv[1]);
		listname=stralloc(argv[2]);
	} else if(argc>1)
	{
		modname=stralloc(argv[1]);
	}
	x=getSlistmodules(mainrsrc,modname);
	y=doSlistfldlist(mainrsrc,modname,listname);

	DoHeaders();
	mainrsrc=RDArsrcNEW(module,"DEFINE SCROLLED LISTS");
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,doSlistfldlistcb,liblst->numlibs,&liblst->libs,NULL);
	addlstrsrc(mainrsrc,"LISTS AVAILABLE",&y,TRUE,setvaluesxcb,lstavl->numlibs,&lstavl->libs,NULL);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newlist,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,oklist,NULL);
	addbtnrsrc(mainrsrc,"PRINT MODULE",TRUE,print_listscb,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdflist,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletelisttest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setvaluesx(mainrsrc,modname,listname);
	APPmakescrn(mainrsrc,TRUE,quitdflist,NULL,TRUE);
}
