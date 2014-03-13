#include<mix.hpp>
#include<fin.hpp>
#include<pay.hpp>
#include<appmgt.hpp>
#include<venpmt.hpp>
#include<purord.hpp>
#include<ldval.hpp>
/*lint -e537 */
#include<limits.h>
/*lint +e537 */


int xaddSCRNvirtual(RDArsrc *r,char *name,int type,unsigned len,
	char *expression,short when,short no_duplicates,int line,char *file)
{
	RDAvirtual *v;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG addSCRNvirtual Add Virtual Field [%s] to Screen [%s] [%s]'s virtual field list at line [%d] program [%s].",name,r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(no_duplicates)
	{
		if(r->virflds!=NULL)
		{
			for(x=0,v=r->virflds;x<r->num;++x,++v)
			{
				if(!RDAstrcmp(name,v->name)) 
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diagvirtual)
					{
						prterr("DIAG Virtual [%s] already exists for RDArsrc Structure [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return(x);
				}
			}
		}
	}
	if(r->virflds!=NULL)
	{
		r->virflds=Rrealloc(r->virflds,(r->num+1)*
			sizeof(RDAvirtual));
	} else {
		r->virflds=Rmalloc(sizeof(RDAvirtual));
		r->num=0;
	}
	v=r->virflds+r->num;
	v->when=when;
	if(name!=NULL) v->name=stralloc(name);
		else v->name=NULL;
	v->type=type;
	v->len=len;
	if(expression!=NULL)
	{
		v->expression=PP_translate(expression);
	} else v->expression=NULL;
	v->computed=FALSE;
	v->range=FALSE;
	switch(v->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			v->value.string_value=NULL;
			v->dlen=0;
			break;
		case BOOLNS:
		case CHARACTERS:
			v->value.string_value=Rmalloc(1);
			*v->value.string_value=0;
			v->dlen=1;
			break;
		case DECIMALV:
		case SDECIMALV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DOUBLEV:
		case SDOUBLEV:
			v->value.float_value=Rmalloc(sizeof(double));
			*v->value.float_value=0;
			v->dlen=sizeof(double);
			break;
		case SHORTV:
		case SSHORTV:
			v->value.short_value=Rmalloc(sizeof(short));
			*v->value.short_value=0;
			v->dlen=sizeof(short);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			v->value.integer_value=Rmalloc(sizeof(int));
			*v->value.integer_value=0;
			v->dlen=sizeof(int);
			break;
		default:
			prterr("Error field type [%d] not found for virtual field [%s] at line [%d] program [%s].",v->type,v->name,line,file);
			v->dlen=0;
			break;
	}
	return(++r->num);
}
int xaddSCRNScrolledList(RDArsrc *r,char *module,char *name,int vtype,
	int numincvirs,DFincvir *incvir,int type,char *select_formula,
	char *special_function,short num,RDAconfld *initfld,char *ffield,
	short occurrence,DFincvir *ffinfo,int contype,char *conmod,
	char *confil,char *confld,short con_occurrence,APPlib *elements,
	char *format_formula,char *unformat_formula,char *reformat_formula,
	char *desc,int timing,
	int line,char *file)
{
	int x,y=0;
	RDAScrolledList *list=NULL;
	DFincvir *temp=NULL,*temp1=NULL;
	RDAconfld *tempinitfld=NULL,*tempinitfld1=NULL;
	char *xtemp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diaglist)
	{
		prterr("DIAG addSCRNScrolledList Add Scrolled List to Screen [%s] [%s]'s at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r->lists!=NULL)
	{
		for(x=0,list=r->lists;x<r->numlists;++x,++list)
		{
			if(!RDAstrcmp(name,list->name)) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagvirtual)
				{
					prterr("DIAG Virtual [%s] already exists for RDArsrc Structure [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(x);
			}
		}
	}
	if(r->lists!=NULL)
	{
		r->lists=Rrealloc(r->lists,(r->numlists+1)*
			sizeof(RDAScrolledList));
	} else {
		r->lists=Rmalloc(sizeof(RDAScrolledList));
		r->numlists=0;
	}
	list=r->lists+r->numlists;
	if(!isEMPTY(module)) list->module=stralloc(module);
		else list->module=NULL;
	if(!isEMPTY(name)) list->name=stralloc(name);
		else list->name=NULL;
	list->vtype=vtype;
	list->dlist=NULL;
	list->numincvirs=numincvirs;
	if(numincvirs>0)
	{
	if(incvir!=NULL)
	{
		y=0;
		list->incvir=Rmalloc(numincvirs*sizeof(DFincvir));
		for(x=0,temp=incvir;x<numincvirs;++x,++temp)
		{
			temp1=list->incvir+y;
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(incvir->module);
			} else temp1->module=NULL;
			if(temp->file!=NULL)
			{
				temp1->file=stralloc(temp->file);
			} else temp1->file=NULL;
			temp1->keyname=NULL;
			++y;
		}
	} else {
		list->numincvirs=0;
		list->incvir=NULL;
	}
	} else {
		list->numincvirs=0;
		list->incvir=NULL;
	}
	list->type=type;
	list->num=num;
	if(num>0)
	{
		if(initfld!=NULL)
		{
			y=0;
			list->initfld=Rmalloc(num*sizeof(RDAconfld));
			for(x=0,tempinitfld=initfld;x<num;++x,++tempinitfld)
			{
				tempinitfld1=list->initfld+y;
				if(tempinitfld->fldname!=NULL)
				{
					tempinitfld1->fldname=stralloc(tempinitfld->fldname);
				} else tempinitfld1->fldname=NULL;
				if(tempinitfld->conname!=NULL)
				{
					tempinitfld1->conname=stralloc(tempinitfld->conname);
				} else tempinitfld1->conname=NULL;
				++y;
			}
		} else {
			list->num=0;
			list->initfld=NULL;
		}
	} else {
		list->initfld=NULL;
	}
	list->ffield=ffield;
	if(!isEMPTY(select_formula)) 
	{
		list->select_formula=PP_translate(select_formula);
	} else list->select_formula=NULL;
	if(!isEMPTY(special_function)) 
	{
		list->special_function=stralloc(special_function);
	} else list->special_function=NULL;
	list->occurrence=occurrence;
	list->dlist=NULL;
	if(ffinfo!=NULL)
	{
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		if(!isEMPTY(ffinfo->module)) 
		{
			list->ffinfo->module=stralloc(ffinfo->module);
		} else list->ffinfo->module=NULL;
		if(!isEMPTY(ffinfo->file))
		{
			list->ffinfo->file=stralloc(ffinfo->file);
		} else list->ffinfo->file=NULL;
		if(!isEMPTY(ffinfo->keyname))
		{
			list->ffinfo->keyname=stralloc(ffinfo->keyname);
		} else list->ffinfo->keyname=NULL;
		list->ffinfo->fileno=ffinfo->fileno;
	} else list->ffinfo=NULL;
	list->contype=contype;
	if(!isEMPTY(conmod)) list->conmod=stralloc(conmod);
		else list->conmod=NULL;
	if(!isEMPTY(confil)) list->confil=stralloc(confil);
		else list->confil=NULL;
	if(!isEMPTY(confld)) list->confld=stralloc(confld);
		else list->confld=NULL;
	list->con_occurrence=con_occurrence;
	if(elements!=NULL)
	{
		list->list=APPlibNEW();
		if(list->type==3 && elements->libs!=NULL)
		{
			for(x=0;x<elements->numlibs;++x) 
			{
				xtemp=PP_translate(elements->libs[x]);
				addAPPlib(list->list,xtemp);
				if(xtemp!=NULL) Rfree(xtemp);
			}
		} else {
			for(x=0;x<elements->numlibs;++x) 
				addAPPlib(list->list,elements->libs[x]);
		}
	} else list->list=APPlibNEW();
	if(!isEMPTY(format_formula)) 
	{
		list->format_formula=PP_translate(format_formula);
	} else list->format_formula=NULL;
	if(!isEMPTY(unformat_formula))
	{
		list->unformat_formula=PP_translate(unformat_formula);
	} else list->unformat_formula=NULL;
	if(!isEMPTY(reformat_formula))
	{
		list->reformat_formula=PP_translate(reformat_formula);
	} else list->reformat_formula=NULL;
	if(!isEMPTY(desc))
	{
		list->desc=stralloc(desc);
	} else list->desc=NULL;
	list->timing=timing;
	return(++r->numlists);
}
/* FREE VIRTUAL FIELD RESOURCES */
void FreeRDAvirtual(RDAvirtual *v)
{
	if(v!=NULL)
	{
		if(v->name!=NULL) Rfree(v->name);
		if(v->expression!=NULL) Rfree(v->expression);
		switch(v->type)
		{
			case SCROLLEDTEXT:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case VARIABLETEXT:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case BOOLNS:
			case CHARACTERS:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(v->value.string_value!=NULL) 
					Rfree(v->value.string_value);
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDECIMALV:
			case SDOUBLEV:
				if(v->value.float_value!=NULL) 
					Rfree(v->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				if(v->value.short_value!=NULL) 
					Rfree(v->value.short_value);
				break;
			case SLONGV:
			case LONGV:
			case SCROLLEDLIST:
				if(v->value.integer_value!=NULL)
					Rfree(v->value.integer_value);
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error field type [%d] invalid for virtual field [%s] at line [%d] program [%s].",v->type,v->name,__LINE__,__FILE__);
				break;
		}
		if(v!=NULL) Rfree(v);
	}
}
void RDALoadValueCB(RDArsrc *r,RDALoadValue *load)
{
	RDArsrc *ldvalrsrc=NULL;
	LoadValue *LOAD=NULL;
	APPlib *filenames=NULL;
	int x=0,y,z;
	char *libx=NULL,addvf=TRUE,*tmpx=NULL;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	RDArsrc *tmprsrc=NULL;
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	char *temp=NULL;


	if(load->dlist==NULL)
	{
		load->dlist=APPlibNEW();


/* LOAD INCLUDE FILE FIELDS INTO APPLIB */
		for(x=0,i=load->incvir;x<load->numincvirs;++x,++i)
		{
			if(!GETBIN(i->module,i->file,&engine,&nofields,&fields,&nokeys,&keys)) 
			{
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(i->module)+RDAstrlen(i->file)+7);
						} else {
							temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(i->module)+RDAstrlen(i->file)+7);
						}
						sprintf(temp,"[%s][%s][%s]",i->module,i->file,f->name);
						addAPPlibNoDuplicates(load->dlist,temp);
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
							for(z=0,part=keyx->part;z<keyx->numparts;
								++z,++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
						}
						Rfree(keyx->part);
					}
					Rfree(keys);
				}
			}
		}
		if(temp!=NULL) Rfree(temp);
	
/* LOAD QUALIFIED VIRTUAL FIELDS INTO APPLIB */
	
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(load->module)+10);
		tmpx=adddashes(load->module);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(tmpx!=NULL) Rfree(tmpx);
		libx=NULL;
		if(filenames!=NULL)
		{
#ifdef RS6000
			tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","GUI","Collecting Virtual Fields",NULL,filenames->numlibs);
#else
			tmprsrc=NULL;
			tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","GUI","Collecting Virtual Fields",NULL,filenames->numlibs);
#endif
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
			}
			for(y=0;y<filenames->numlibs;++y)
			{
				d=getDFvirtual(load->module,filenames->libs[y]);
				if(d!=NULL)
				{
					addvf=TRUE;
					if(d->incvir!=NULL)
					{
						if(!RDAstrcmp(d->incvir->module,load->module) &&  d->incvir->file==NULL) 
						{
							addvf=TRUE;
						} else {
							if(!d->vtype)
							{
								for(z=0,a=d->incvir;z<d->num;++z,++a)
								{
									for(x=0,i=load->incvir;x<load->numincvirs;++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module) && 
											!RDAstrcmp(i->file,a->file)) break;
									}
									if(x>=load->numincvirs) 
									{
										addvf=FALSE;
										break;
									}
									if(!addvf) break;
								}
							} else {
								for(z=0,a=d->incvir;z<d->num;++z,++a)
								{
									for(x=0,i=load->incvir;x<load->numincvirs;++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module) && 
											!RDAstrcmp(i->file,a->file)) break;
									}
									if(x<load->numincvirs) break;
								}
								if(z<d->num) addvf=TRUE;
									else addvf=FALSE;
							}
						}
					}
					if(addvf)
					{
						libx=Rmalloc(RDAstrlen(d->name)+3);
						sprintf(libx,"[%s]",d->name);
						if(addAPPlibNoDuplicates(load->dlist,libx)!=(-1))
						{
							if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
						} else {
							if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
						}
						if(libx!=NULL) Rfree(libx);
					} else {
						if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
					}
					FreeDFvirtual(d);
					d=NULL;
				}
			}
			freeapplib(filenames);
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}

/* LOAD GLOBAL FIELDS INTO APPLIB */
		loadglobals(load->dlist);
/* LOAD MANUAL ELEMENTS INTO APPLIB */
		if(load->mlist!=NULL)
		{
			for(x=0;x<load->mlist->numlibs;++x)
			{
				addAPPlibNoDuplicates(load->dlist,load->mlist->libs[x]);
			}
		}
	}


	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	LOAD=LoadValueNew(ldvalrsrc,r,load->conname);
	if(load->dlist->numlibs<1)
	{
		addAPPlib(load->dlist,"Load List Unavailable");
	} else SORTAPPlib(load->dlist);
	loadlist=CopyAPPlib(load->dlist);
	if(load->dlist->libs!=NULL)
	{
		for(x=0;x<load->dlist->numlibs;++x)
		{
			if(strncmp(load->dlist->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=load->dlist->numlibs) x=0;
	} else x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,load->dlist->numlibs,&load->dlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,LOAD);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,LOAD);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,LOAD,FALSE);
}
void RDALoadOperatorCB(RDArsrc *r,RDALoadValue *load)
{
	loadoperand(r,load->conname);
}
void RDALoadGrouperCB(RDArsrc *r,RDALoadValue *load)
{
	loadgrouper(r,load->conname);
}
void RDALoadControlCB(RDArsrc *r,RDALoadValue *load)
{
	loadcontrol(r,load->conname);
}
int xaddSCRNLoadValues(RDArsrc *r,char *module,char *name,
	int numincvirs,DFincvir *incvir,
	char *conname,char *desc,APPlib *elements,
	int line,char *file)
{
	int x,y=0;
	RDALoadValue *load=NULL;
	DFincvir *temp=NULL,*temp1=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diaglist)
	{
		prterr("DIAG addSCRNLoadValues Add Load Value resources to Screen [%s] [%s]'s at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r->loads!=NULL)
	{
		for(x=0,load=r->loads;x<r->numloads;++x,++load)
		{
			if(!RDAstrcmp(name,load->name)) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagvirtual)
				{
					prterr("DIAG Load Value [%s] already exists for RDArsrc Structure [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(x);
			}
		}
	}
	if(r->loads!=NULL)
	{
		r->loads=Rrealloc(r->loads,(r->numloads+1)*
			sizeof(RDALoadValue));
	} else {
		r->loads=Rmalloc(sizeof(RDALoadValue));
		r->numloads=0;
	}
	load=r->loads+r->numloads;
	if(!isEMPTY(module)) load->module=stralloc(module);
		else load->module=NULL;
	if(!isEMPTY(name)) load->name=stralloc(name);
		else load->name=NULL;
	load->dlist=NULL;
	load->numincvirs=numincvirs;
	if(numincvirs>0)
	{
	if(incvir!=NULL)
	{
		y=0;
		load->incvir=Rmalloc(numincvirs*sizeof(DFincvir));
		for(x=0,temp=incvir;x<numincvirs;++x,++temp)
		{
			temp1=load->incvir+y;
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
			} else temp1->module=NULL;
			if(temp->file!=NULL)
			{
				temp1->file=stralloc(temp->file);
			} else temp1->file=NULL;
			temp1->keyname=NULL;
			++y;
		}
	} else {
		load->numincvirs=0;
		load->incvir=NULL;
	}
	} else {
		load->numincvirs=0;
		load->incvir=NULL;
	}
	load->dlist=NULL;
	if(!isEMPTY(conname)) load->conname=stralloc(conname);
		else load->conname=NULL;
	if(!isEMPTY(desc))
	{
		load->desc=stralloc(desc);
	} else load->desc=NULL;
	if(elements!=NULL)
	{
		load->mlist=APPlibNEW();
		for(x=0;x<elements->numlibs;++x) 
		{
			addAPPlib(load->mlist,elements->libs[x]);
		}
	} else load->mlist=APPlibNEW();
	return(++r->numloads);
}
void xGET_SCREEN_VIRTUALS(RDArsrc *r,short type,short force,
	short includelists,short includeloads,short include_virtuals,
	int line,char *file)
{
	char *libx=NULL,*tmpx=NULL,addvf=FALSE,temp[501];
	char *tx=NULL,*jones=NULL;
	APPlib *filenames=NULL,*ScrolledLists=NULL;
	int q=0,x,y,z,g,vc=0,lc=0;
	DFvirtual *d=NULL;
	DFincvir *i,*a;
	RDAwdgt *wdgt;
	short INCLUDE_LISTS=FALSE;
	RDAvirtual *v,*v1;
	RDAScrolledList *list=NULL,*LIST=NULL;
	APPlib *RLVs=NULL;
	RDALoadValue *load=NULL;
	char *temp_loadname=NULL;
	char *libloadx=NULL;
	int mi=0;

	INCLUDE_LISTS=includelists;
	if(type==2) INCLUDE_LISTS=TRUE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG GET_SCREEN_VIRTUALS Creating Virtual Fields used on Custom Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!force)
	{
		if(r->scn==NULL)
		{
			r->scn=RDAscrnNEW(r->module,r->screen);
			if(!isEMPTY(r->screen))
			{
				if(findscrnbin(r->scn)) 
				{
					prterr("Error couldn't read Screen Binary [%s] [%s] from Binary Library [%s.SCN]",r->module,(r->screen!=NULL ? r->screen:""),r->module);
					free_scrn(r->scn);
					r->scn=NULL;
					return;
				} 
			}
		}
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(r->module)+10);
	tmpx=adddashes(r->module);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
	if(include_virtuals)
	{
		filenames=getunsortedlibnames(libx,FALSE);
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
	tmpx=NULL;
	libx=NULL;
	if(INCLUDE_LISTS)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(r->module)+10);
		tmpx=adddashes(r->module);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,tmpx);
#endif
		ScrolledLists=getunsortedlibnames(libx,FALSE);
		if(ScrolledLists==NULL) ScrolledLists=APPlibNEW();
		if(tmpx!=NULL) Rfree(tmpx);
	}
	if(includeloads)
	{
		libloadx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(r->module)+10);
		tmpx=adddashes(r->module);
#ifndef WIN32
		sprintf(libloadx,"%s/rda/%s.LDV",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libloadx,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,tmpx);

#endif
		RLVs=getunsortedlibnames(libloadx,FALSE);
		if(RLVs==NULL) RLVs=APPlibNEW();
		if(tmpx!=NULL) Rfree(tmpx);
	}
	if(filenames==NULL) filenames=APPlibNEW();
	if(!force)
	{
		if(r->scn->wdgts!=NULL)
		{
			for(g=0,wdgt=r->scn->wdgts;g<r->scn->numwdgts;++g,++wdgt)
			{
				if((!wdgt->type || wdgt->type==8 || wdgt->type==9)
					&& wdgt->resource_name!=NULL)
				{
/* GETTING VIRTUAL FIELDS USED ON SCREEN */
					for(y=0;y<filenames->numlibs;++y)
					{
						if(!type) /* regular screen */
						{
							if(!RDAstrcmp(wdgt->resource_name,
								filenames->libs[y])) break;
						} else if(type==1) /* Define List */
						{
							memset(temp,0,501);
							sprintf(temp,"%s LENGTH",filenames->libs[y]);
							if(!RDAstrcmp(wdgt->resource_name,temp)) break;
							memset(temp,0,501);
							sprintf(temp,"%s POSITION",filenames->libs[y]);
							if(!RDAstrcmp(wdgt->resource_name,temp)) break; 
						} else if(type==2) /* Range Screen */
						{
							if(wdgt->type==9)
							{
								memset(temp,0,501);
								sprintf(temp,"SELECT [%s] TRUE",
									filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break;
								memset(temp,0,501);
								sprintf(temp,"SELECT [%s] FALSE",
									filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break; 
							} else {
								memset(temp,0,501);
								sprintf(temp,"FROM [%s]",filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break;
								memset(temp,0,501);
								sprintf(temp,"TO [%s]",filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break; 
							}
						}
					}
					if(y<filenames->numlibs)
					{
						mi=FINDRSC(r,filenames->libs[y]);
						if(mi==(-1))
						{
						d=getDFvirtual(r->module,filenames->libs[y]);
						if(d!=NULL)
						{
							addvf=d->avail2rsrc;
							if(d->incvir!=NULL && d->avail2rsrc &&
								!d->vtype)
							{
								for(z=0,a=d->incvir;z<d->num;++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x>=r->numvir) 
									{
										addvf=FALSE;
										break;
									}
								}
							} else if(d->incvir!=NULL && d->avail2rsrc &&
								d->vtype)
							{
								for(z=0,a=d->incvir;z<d->num;++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x<r->numvir)
									{
										addvf=TRUE;
										break;
									}
								}
								if(z>=d->num) addvf=FALSE;
							}
							if(addvf)
							{
								addSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when);
							}
							FreeDFvirtual(d);
							d=NULL;
						}
						}
					}
				} else if(((wdgt->type==7) && wdgt->resource_name!=NULL)
					&& INCLUDE_LISTS)
				{
/* GETTING SCROLLED LISTS USED ON SCREEN */
					for(y=0;y<ScrolledLists->numlibs;++y)
					{
						if(!RDAstrcmp(wdgt->resource_name,
							ScrolledLists->libs[y])) break;
					}
					if(y<ScrolledLists->numlibs)
					{
						list=RDAScrolledListNew(r->module,
							wdgt->resource_name);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addvf=TRUE;
							if(list->incvir!=NULL && !list->vtype)
							{
								for(z=0,a=list->incvir;z<list->numincvirs;
									++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x>=r->numvir) 
									{
										addvf=FALSE;
										break;
									}
								}
							} else if(list->incvir!=NULL && list->vtype)
							{
								for(z=0,a=list->incvir;z<list->numincvirs;++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x<r->numvir)
									{
										addvf=TRUE;
										break;
									}
								}
								if(z>=list->numincvirs) addvf=FALSE;
							}
							if(addvf)
							{
								addSCRNScrolledList(r,list->module,
									list->name,list->vtype,
									list->numincvirs,list->incvir,
									list->type,list->select_formula,
									list->special_function,list->num,list->initfld,
									list->ffield,list->occurrence,list->ffinfo,list->contype,
									list->conmod,list->confil,
									list->confld,list->con_occurrence,list->list,
									list->format_formula,
									list->unformat_formula,
									list->reformat_formula,
									list->desc,list->timing);
							}
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
				} else if((wdgt->type==6) && (!isEMPTY(wdgt->resource_name))
					&& includeloads)
				{
					if((RDAstrlen(wdgt->resource_name)>11) && (RDAstrstr(wdgt->resource_name,"LOAD VALUE"))!=NULL)
					{
					temp_loadname=stralloc(wdgt->resource_name);
					temp_loadname[RDAstrlen(temp_loadname)-11]=0;
					for(y=0;y<RLVs->numlibs;++y)
					{
						if(!RDAstrcmp(temp_loadname,
							RLVs->libs[y])) break;
					}
					if(y<RLVs->numlibs)
					{
						load=RDALoadValueNEW(r->module,
							temp_loadname);
						if(getRDALoadValuebin(libloadx,load)!=(-1))
						{
							addvf=TRUE;
							if(addvf)
							{
								addSCRNLoadValuesADV(r,load->module,
									load->name,load->numincvirs,load->incvir,
									load->conname,
									load->desc,load->mlist);
							}
						}
						if(load!=NULL) FreeRDALoadValue(load);
					}
					if(temp_loadname!=NULL) Rfree(temp_loadname);
					}
				}
			}
		}
	} else { /* FORCED */
		if(INCLUDE_LISTS)
		{
			if(r->scn==NULL && !isEMPTY(r->module) && !isEMPTY(r->screen))
			{
				r->scn=RDAscrnNEW(r->module,r->screen);
				if(findscrnbin(r->scn))
				{ 
					free_scrn(r->scn);
					r->scn=NULL;
				}
			} 
			if(r->scn!=NULL)
			{
			
/* GETTING ALL SCROLLED LISTS AVAILABLE */
			if(r->scn->wdgts!=NULL)
			{
				for(g=0,wdgt=r->scn->wdgts;g<r->scn->numwdgts;++g,++wdgt)
				{
					if(((wdgt->type==7) && wdgt->resource_name!=NULL)
						&& INCLUDE_LISTS)
					{
/* 	GETTING SCROLLED LISTS USED ON SCREEN */
						for(y=0;y<ScrolledLists->numlibs;++y)
						{
							if(!RDAstrcmp(wdgt->resource_name,
								ScrolledLists->libs[y])) break;
						}
						if(y<ScrolledLists->numlibs)
						{
							list=RDAScrolledListNew(r->module,
								wdgt->resource_name);
							if(getScrolledListbin(libx,list)!=(-1))
							{
								addvf=TRUE;
								if(list->incvir!=NULL && !list->vtype)
								{
									for(z=0,a=list->incvir;z<list->numincvirs;
										++z,++a)
									{
										for(x=0,i=r->incvir;x<r->numvir;
											++x,++i)
										{
											if(!RDAstrcmp(i->module,a->module)
												&& !RDAstrcmp(i->file,a->file)) 
												break;
										}
										if(x>=r->numvir) 
										{
											addvf=FALSE;
											break;
										}
									}
								} else if(list->incvir!=NULL && list->vtype)
								{
									for(z=0,a=list->incvir;z<list->numincvirs;++z,++a)
									{
										for(x=0,i=r->incvir;x<r->numvir;
											++x,++i)
										{
											if(!RDAstrcmp(i->module,a->module)
												&& !RDAstrcmp(i->file,a->file)) 
												break;
										}
										if(x<r->numvir)
										{
											addvf=TRUE;
											break;
										}
									}
									if(z>=list->numincvirs) addvf=FALSE;
								}
								if(addvf)
								{
									addSCRNScrolledList(r,list->module,
										list->name,list->vtype,
										list->numincvirs,list->incvir,
										list->type,list->select_formula,
										list->special_function,list->num,list->initfld,
										list->ffield,list->occurrence,list->ffinfo,list->contype,
										list->conmod,list->confil,
										list->confld,list->con_occurrence,list->list,
										list->format_formula,
										list->unformat_formula,
										list->reformat_formula,
										list->desc,list->timing);
								}
							}
							if(list!=NULL) FreeRDAScrolledList(list);
						}
					}
				}
			}
			}
		}
/* GETTING ALL VIRTUAL FIELDS AVAILABLE */
		for(y=0;y<filenames->numlibs;++y)
		{
			mi=FINDRSC(r,filenames->libs[y]);
			if(mi==(-1))
			{
			d=getDFvirtual(r->module,filenames->libs[y]);
			if(d!=NULL)
			{
				addvf=d->avail2rsrc;
				if(d->incvir!=NULL && d->avail2rsrc && !d->vtype)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=r->incvir;x<r->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=r->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				} else if(d->incvir!=NULL && d->avail2rsrc &&
					d->vtype)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=r->incvir;x<r->numvir;
							++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module)
								&& !RDAstrcmp(i->file,a->file)) 
								break;
						}
						if(x<r->numvir)
						{
							addvf=TRUE;
							break;
						}
					}
					if(z>=d->num) addvf=FALSE;
				}
				if(addvf)
				{
					addSCRNvirtual(r,d->name,d->type,d->length,d->expression,d->when);
				}
				FreeDFvirtual(d);
				d=NULL;
			}
			}
		}
	} 
/* GETTING VIRTUAL FIELDS USED IN OTHER VIRTUAL FIELDS */
	if(!force)
	{
	vc=0;
	while(TRUE)
	{
		if(vc>=r->num) break;
		for(x=0;x<filenames->numlibs;++x)
		{
			v=r->virflds+vc;
			if(v->expression!=NULL && RDAstrcmp(v->name,filenames->libs[x]))
			{
				jones=Rmalloc(RDAstrlen(filenames->libs[x])+3);
				sprintf(jones,"[%s]",filenames->libs[x]);
				tx=RDAstrstr(v->expression,jones); 
				if(!isEMPTY(tx))
				{
					for(y=0,v1=r->virflds;y<r->num;++y,++v1)
					{
						if(!RDAstrcmp(v1->name,
							filenames->libs[x])) break;
					}	
					if(y>=r->num)
					{
						mi=FINDRSC(r,filenames->libs[x]);
						if(mi==(-1))
						{
						d=getDFvirtual(r->module,filenames->libs[x]);
						if(d!=NULL)
						{
							ADVaddSCRNvirtual(r,d->name,d->type,d->length,
								d->expression,d->when,FALSE);
							FreeDFvirtual(d);
							d=NULL;
						}
						}
					}
				}
				if(jones!=NULL) Rfree(jones);
			}
		}
		if(INCLUDE_LISTS)
		{
			for(y=0;y<ScrolledLists->numlibs;++y)
			{
				v=r->virflds+vc;
				if(v->expression!=NULL)
				{
					jones=Rmalloc(RDAstrlen(ScrolledLists->libs[y])+3);
					sprintf(jones,"[%s]",ScrolledLists->libs[y]);
					tx=RDAstrstr(v->expression,jones); 
					if(!isEMPTY(tx))
					{
						list=RDAScrolledListNew(r->module,
							ScrolledLists->libs[y]);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addSCRNScrolledList(r,list->module,
								list->name,list->vtype,list->numincvirs,list->incvir,
								list->type,list->select_formula,
								list->special_function,list->num,list->initfld,
								list->ffield,list->occurrence,list->ffinfo,
								list->contype,list->conmod,
								list->confil,list->confld,list->con_occurrence,list->list,
								list->format_formula,
								list->unformat_formula,
								list->reformat_formula,
								list->desc,list->timing);
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
					if(jones!=NULL) Rfree(jones);
				}
			}
			++vc;
		}
	}
	if(INCLUDE_LISTS)
	{
		lc=0;
		while(TRUE)
		{
			if(lc>=r->numlists) break;
			for(x=0;x<filenames->numlibs;++x)
			{
				LIST=r->lists+lc;
				if(!isEMPTY(LIST->select_formula))
				{
					jones=Rmalloc(RDAstrlen(filenames->libs[x])+3);
					sprintf(jones,"[%s]",filenames->libs[x]);
					tx=RDAstrstr(LIST->select_formula,jones); 
					if(!isEMPTY(tx))
					{
						for(y=0,v1=r->virflds;y<r->num;++y,++v1)
						{
							if(!RDAstrcmp(v1->name,
								filenames->libs[x])) break;
						}	
						if(y>=r->num)
						{
							mi=FINDRSC(r,filenames->libs[x]);
							if(mi==(-1))
							{
							d=getDFvirtual(r->module,filenames->libs[x]);
							if(d!=NULL)
							{
								ADVaddSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when,FALSE);
								FreeDFvirtual(d);
								d=NULL;
							}
							}
						}
					}
					if(jones!=NULL) Rfree(jones);
				}
/* for computed list */
				if(LIST->type==3 && LIST->list!=NULL)
				{
					if(LIST->list->libs!=NULL && LIST->list->numlibs>0)
					{
						for(q=0;q<LIST->list->numlibs;++q)
						{
							jones=Rmalloc(RDAstrlen(filenames->libs[x])+3);
							sprintf(jones,"[%s]",filenames->libs[x]);
							tx=RDAstrstr(LIST->list->libs[q],jones); 
							if(!isEMPTY(tx))
							{
								for(y=0,v1=r->virflds;y<r->num;++y,++v1)
								{
									if(!RDAstrcmp(v1->name,
										filenames->libs[x])) break;
								}	
								if(y>=r->num)
								{
									mi=FINDRSC(r,filenames->libs[x]);
									if(mi==(-1))
									{
									d=getDFvirtual(r->module,filenames->libs[x]);
									if(d!=NULL)
									{
										ADVaddSCRNvirtual(r,d->name,d->type,d->length,
											d->expression,d->when,FALSE);
										FreeDFvirtual(d);
										d=NULL;
									}
									}
								}
							}
							if(jones!=NULL) Rfree(jones);
						}
					}
				}
				if(!isEMPTY(LIST->format_formula))
				{
					jones=Rmalloc(RDAstrlen(filenames->libs[x])+3);
					sprintf(jones,"[%s]",filenames->libs[x]);
					tx=RDAstrstr(LIST->format_formula,jones); 
					if(!isEMPTY(tx))
					{
						for(y=0,v1=r->virflds;y<r->num;++y,++v1)
						{
							if(!RDAstrcmp(v1->name,
								filenames->libs[x])) break;
						}	
						if(y>=r->num)
						{
							mi=FINDRSC(r,filenames->libs[x]);
							if(mi==(-1))
							{
							d=getDFvirtual(r->module,filenames->libs[x]);
							if(d!=NULL)
							{
								ADVaddSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when,FALSE);
								FreeDFvirtual(d);
								d=NULL;
							}
							}
						}
					}
					if(jones!=NULL) Rfree(jones);
				}
			}
			for(y=0;y<ScrolledLists->numlibs;++y)
			{
				LIST=r->lists+lc;
				if(!isEMPTY(LIST->select_formula) && 
					RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{
					jones=Rmalloc(RDAstrlen(ScrolledLists->libs[y])+3);
					sprintf(jones,"[%s]",ScrolledLists->libs[y]);
					tx=RDAstrstr(LIST->select_formula,jones); 
					if(!isEMPTY(tx))
					{
						list=RDAScrolledListNew(r->module,
							ScrolledLists->libs[y]);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addSCRNScrolledList(r,list->module,
								list->name,list->vtype,list->numincvirs,list->incvir,
								list->type,list->select_formula,
								list->special_function,
								list->num,list->initfld,list->ffield,
								list->occurrence,list->ffinfo,
								list->contype,list->conmod,
								list->confil,list->confld,list->con_occurrence,
								list->list,
																		list->format_formula,
								list->unformat_formula,
								list->reformat_formula,
								list->desc,list->timing);
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
					if(jones!=NULL) Rfree(jones);
				}
				if(!isEMPTY(LIST->format_formula) &&
					RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{
					jones=Rmalloc(RDAstrlen(ScrolledLists->libs[y])+3);
					sprintf(jones,"[%s]",ScrolledLists->libs[y]);
					tx=RDAstrstr(LIST->select_formula,jones); 
					if(!isEMPTY(tx))
					{
						list=RDAScrolledListNew(r->module,
							ScrolledLists->libs[y]);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addSCRNScrolledList(r,list->module,
								list->name,list->vtype,list->numincvirs,list->incvir,
								list->type,list->select_formula,
								list->special_function,
								list->num,list->initfld,list->ffield,
								list->occurrence,list->ffinfo,
								list->contype,list->conmod,
								list->confil,list->confld,list->con_occurrence,list->list,
								list->format_formula,
								list->unformat_formula,
								list->reformat_formula,
								list->desc,list->timing);
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
					if(jones!=NULL) Rfree(jones);
				}
			}
			++lc;
		}
	}
	}
	freeapplib(filenames);
	filenames=NULL;
	if(INCLUDE_LISTS)
	{
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
				if(list->incvir!=NULL)
				{
				for(y=0,i=list->incvir;y<list->numincvirs;++y,++i)
				{
					for(z=0,a=r->incvir;z<r->numvir;++z,++a)
					{
						if(!RDAstrcmp(i->module,a->module)
							&& !RDAstrcmp(i->file,a->file)) break;
					}
					if(z<r->numvir)
					{
						i->fileno=a->fileno;
					}
				}
				if(!list->type)
				{
					if(list->ffinfo!=NULL)
					{
						for(z=0,a=r->incvir;z<r->numvir;++z,++a)
						{
							if(!RDAstrcmp(list->ffinfo->module,a->module)
								&& !RDAstrcmp(list->ffinfo->file,a->file)) break;
						}
						if(z<r->numvir)
						{
							list->ffinfo->fileno=a->fileno;
						}
					}
				}
				}
			}
		}
	}
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(libloadx!=NULL) Rfree(libloadx);
	if(RLVs!=NULL) freeapplib(RLVs);
	if(ScrolledLists!=NULL) freeapplib(ScrolledLists);
	if(filenames!=NULL) freeapplib(filenames);
}
APPlib *FindModulesAvailable()
{
	int x,y=0;
	char *tmp=NULL,*dirx=NULL;
	APPlib *retval=NULL;

	retval=APPlibNEW();
	for(y=0;y<2;++y)
	{
	switch(y)
	{
		default:
		case 0:
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
			sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
			sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
			break;
		case 1:
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
			sprintf(dirx,"%s/site/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
			sprintf(dirx,"%s\\site\\",CURRENTDIRECTORY);
#endif
			break;
		case 2:
			dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+6);
#ifndef WIN32
			sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,(USERLOGIN!=NULL ? USERLOGIN:""));
#endif
#ifdef WIN32
			sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,(USERLOGIN!=NULL ? USERLOGIN:""));
#endif
			break;
	}
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlibNoDuplicates(retval,tmp);
	}
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlibNoDuplicates(retval,tmp);
	}
	if(dirx!=NULL) Rfree(dirx);
	}
	if(XPERT_SETUP->FINMGT==TRUE) addAPPlibNoDuplicates(retval,"FINMGT");
	if(XPERT_SETUP->VENPMT==TRUE) addAPPlibNoDuplicates(retval,"VENPMT");
	if(XPERT_SETUP->VENPMT==TRUE) addAPPlibNoDuplicates(retval,"VENSAFE");
	if(XPERT_SETUP->PURORD==TRUE) addAPPlibNoDuplicates(retval,"PURORD");
	if(XPERT_SETUP->BNKREC==TRUE) addAPPlibNoDuplicates(retval,"BNKREC");
	if(XPERT_SETUP->FIXASS==TRUE) addAPPlibNoDuplicates(retval,"FIXASS");
	if(XPERT_SETUP->PRSNNL==TRUE) addAPPlibNoDuplicates(retval,"PRSNNL");
	if(XPERT_SETUP->PAYROLL==TRUE) addAPPlibNoDuplicates(retval,"PAYROLL");
	if(XPERT_SETUP->POSTRK==TRUE) addAPPlibNoDuplicates(retval,"POSTRK");
	if(XPERT_SETUP->BFTMGT==TRUE) addAPPlibNoDuplicates(retval,"BFTMGT");
	if(XPERT_SETUP->APPMGT==TRUE) addAPPlibNoDuplicates(retval,"APPMGT");
	if(XPERT_SETUP->EMPABS==TRUE) addAPPlibNoDuplicates(retval,"EMPABS");
	if(XPERT_SETUP->LVEMGT==TRUE) addAPPlibNoDuplicates(retval,"LVEMGT");
	if(XPERT_SETUP->INVCTL==TRUE) addAPPlibNoDuplicates(retval,"INVCTL");
	if(XPERT_SETUP->SUBMGT==TRUE) addAPPlibNoDuplicates(retval,"SUBMGT");
	if(XPERT_SETUP->LVSIMP==TRUE) addAPPlibNoDuplicates(retval,"LVSIMP");
	if(XPERT_SETUP->INVCTL==TRUE) addAPPlibNoDuplicates(retval,"INVCTL");
	if(XPERT_SETUP->RLVEMGT==TRUE) addAPPlibNoDuplicates(retval,"RLVEMGT");
	if(XPERT_SETUP->REQMGT==TRUE) addAPPlibNoDuplicates(retval,"REQMGT");
	if(XPERT_SETUP->RLSTMGT==TRUE) addAPPlibNoDuplicates(retval,"RLSTMGT");
	if(XPERT_SETUP->CSHDWR==TRUE) addAPPlibNoDuplicates(retval,"CSHDWR");
	if(XPERT_SETUP->CSHDWR==TRUE) addAPPlibNoDuplicates(retval,"OWNERS");
	if(XPERT_SETUP->DMVREG==TRUE) addAPPlibNoDuplicates(retval,"DMVREG");
	if(XPERT_SETUP->PROPERTY==TRUE) addAPPlibNoDuplicates(retval,"PROPERTY");
	if((XPERT_SETUP->PROPERTY==TRUE) && (!RDAstrcmp(XPERT_SETUP->state,"VA"))) addAPPlibNoDuplicates(retval,"VAIRMS");
	if((XPERT_SETUP->RLSTMGT==TRUE) && (!RDAstrcmp(XPERT_SETUP->state,"VA"))) addAPPlibNoDuplicates(retval,"VAIRMS");
	if(XPERT_SETUP->MISCBILL==TRUE) addAPPlibNoDuplicates(retval,"MISCBILL");
	if(XPERT_SETUP->CATALOGUE==TRUE) addAPPlibNoDuplicates(retval,"CATALOGUE");
	if(XPERT_SETUP->BLDPRMT==TRUE) addAPPlibNoDuplicates(retval,"BLDPRMT");
	if(XPERT_SETUP->ARCHIVE==TRUE) addAPPlibNoDuplicates(retval,"ARCHIVE");
	if(XPERT_SETUP->BUDPREP==TRUE) addAPPlibNoDuplicates(retval,"BUDPREP");
	if(XPERT_SETUP->UTLBLL==TRUE) addAPPlibNoDuplicates(retval,"UTLBLL");
	if(XPERT_SETUP->OCCTAX==TRUE) addAPPlibNoDuplicates(retval,"OCCTAX");
	if(XPERT_SETUP->FOODSVC==TRUE) addAPPlibNoDuplicates(retval,"FOODSVC");
	if(XPERT_SETUP->OPENSOURCE==TRUE)
	{
		addAPPlibNoDuplicates(retval,"CUSTOMERS");
		addAPPlibNoDuplicates(retval,"HELPDESK");
		addAPPlibNoDuplicates(retval,"KNOWLEDGE");
		addAPPlibNoDuplicates(retval,"MBGUI");
		addAPPlibNoDuplicates(retval,"MYCLASS");
		addAPPlibNoDuplicates(retval,"PAYABLE");
		addAPPlibNoDuplicates(retval,"PRICING");
		addAPPlibNoDuplicates(retval,"PRODUCTS");
		addAPPlibNoDuplicates(retval,"RECEIVABLE");
		addAPPlibNoDuplicates(retval,"TIMEMGT");
		addAPPlibNoDuplicates(retval,"XACT");
		addAPPlibNoDuplicates(retval,"XPERT");
		addAPPlibNoDuplicates(retval,"VIOLATION");
	}
	addAPPlibNoDuplicates(retval,"CONVERSION");
	addAPPlibNoDuplicates(retval,"DATABASE");
	addAPPlibNoDuplicates(retval,"GUI");
	addAPPlibNoDuplicates(retval,"OLHELP");
	addAPPlibNoDuplicates(retval,"RPTGEN");
	addAPPlibNoDuplicates(retval,"SECURITY");
	addAPPlibNoDuplicates(retval,"TRANS");
	addAPPlibNoDuplicates(retval,"TOOLS");
	addAPPlibNoDuplicates(retval,"UTILITIES");
	
	SORTAPPlib(retval);
	if(tmp!=NULL) Rfree(tmp);
	return(retval);	
}
APPlib *FindTablesAvailable(char *modexp,void (*SubFunc)(...),void *args)
{
	char *modx=NULL,*libx=NULL;
	APPlib *file_defs=NULL;

	modx=PP_EVALUATEstr(modexp,SubFunc,args);
	if(!isEMPTY(modx))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.TBL",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,modx);
#endif
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs==NULL)
		{
			file_defs=APPlibNEW();
			addAPPlib(file_defs,"Contains No Matrix Definitions");
		} else {
			SORTAPPlib(file_defs);
		}
	}
	return(file_defs);
}
APPlib *FindMatrixAvailable(char *modexp,void (*SubFunc)(...),void *args)
{
	char *modx=NULL,*libx=NULL;
	APPlib *file_defs=NULL;

	modx=PP_EVALUATEstr(modexp,SubFunc,args);
	if(!isEMPTY(modx))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.MTX",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,modx);
#endif
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs==NULL)
		{
			file_defs=APPlibNEW();
			addAPPlib(file_defs,"Contains No Matrix Definitions");
		} else {
			SORTAPPlib(file_defs);
		}
	}
	return(file_defs);
}
APPlib *FindFilesAvailable(char *modexp,void (*SubFunc)(...),void *args)
{
	char *modx=NULL,*libx=NULL;
	APPlib *file_defs=NULL;

	modx=PP_EVALUATEstr(modexp,SubFunc,args);
	if(!isEMPTY(modx))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modx);
#endif
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs==NULL)
		{
			file_defs=APPlibNEW();
			addAPPlib(file_defs,"Contains No File Definitions");
		} else {
			SORTAPPlib(file_defs);
		}
	}
	return(file_defs);
}
APPlib *GetFieldsAvailable(char *modexp,void (*SubFunc)(...),void *args)
{
	char *srx=NULL,*modx=NULL,*filx=NULL;
	APPlib *file_defs=NULL;
	short fileno=(-1),x=0;
	NRDfield *field=NULL,*fields=NULL;
	short numflds=0;

	file_defs=APPlibNEW();
	srx=PP_EVALUATEstr(modexp,SubFunc,args);
	if(!isEMPTY(srx))
	{
		modx=stripmodulename(srx);
		filx=stripfilename(srx);
		if(!isEMPTY(modx) && !isEMPTY(filx))
		{
			fileno=OPNNRD(modx,filx);
			if(fileno!=(-1))
			{
				fields=FLDPOINTER(fileno);
				numflds=NUMFLDS(fileno);
				if(fields!=NULL)
				{
					for(x=0,field=fields;x<numflds;++x,++field)
					{
						addAPPlib(file_defs,field->name);
					}
					SORTAPPlib(file_defs);
				}
				CLSNRD(fileno);
			}
		}
	}
	if(file_defs->numlibs<1)
	{
		addAPPlib(file_defs,"No Fields Available");
	}
	return(file_defs);
}
APPlib *GetProcessesAvailable(char *modexp,void (*SubFunc)(...),void *args)
{
	char *modx=NULL,*libx=NULL,*tmp=NULL,*temp=NULL,*tc=NULL;
	APPlib *file_defs=NULL,*tmplist=NULL;
	int x=0,y=0,tc_count=0;
	RDAmenu *menu=NULL;

	modx=PP_EVALUATEstr(modexp,SubFunc,args);
	tmplist=APPlibNEW();
	if(!isEMPTY(modx))
	{
		if(!RDAstrcmp(modx,"RPTGEN"))
		{
			addAPPlibNoDuplicates(tmplist,"OK ACKNOWLEDGEMENT");
			addAPPlibNoDuplicates(tmplist,"SETUP OUTPUT DEVICES");
			addAPPlibNoDuplicates(tmplist,"DEFINE REPORTS");
			addAPPlibNoDuplicates(tmplist,"DEFINE OUTPUT DEVICES");
			addAPPlibNoDuplicates(tmplist,"EZ DEFINE REPORTS");
			addAPPlibNoDuplicates(tmplist,"DEFINE EZ VFS");
		} else if(!RDAstrcmp(modx,"UTILITIES"))
		{
			addAPPlibNoDuplicates(tmplist,"DOHTTP");
			addAPPlibNoDuplicates(tmplist,"EDIT MATRIX");
			addAPPlibNoDuplicates(tmplist,"TEST EMAIL");
			addAPPlibNoDuplicates(tmplist,"CHART DATABASE");
			addAPPlibNoDuplicates(tmplist,"DEFINE IMP EXP");
			addAPPlibNoDuplicates(tmplist,"DEFINE CHANGE");
			addAPPlibNoDuplicates(tmplist,"DEFINE CHANGE FIELDS");
			addAPPlibNoDuplicates(tmplist,"DEFINE GENERIC SETUP VARIABLES");
			addAPPlibNoDuplicates(tmplist,"MANAGE CRONTAB");
			addAPPlibNoDuplicates(tmplist,"FTP PROGRAMS");
			addAPPlibNoDuplicates(tmplist,"SSH PROGRAMS");
			addAPPlibNoDuplicates(tmplist,"DOFTPZIP");
			addAPPlibNoDuplicates(tmplist,"GET LASER FORMS");
			addAPPlibNoDuplicates(tmplist,"LASER FORM TEST");
			addAPPlibNoDuplicates(tmplist,"PLAINPAPER TEST");
			addAPPlibNoDuplicates(tmplist,"DISPLAY PROGRAM UPDATE STATUS");
			addAPPlibNoDuplicates(tmplist,"TEXT BROWSER");
			addAPPlibNoDuplicates(tmplist,"REMOTELP");
			addAPPlibNoDuplicates(tmplist,"WORKSTATION VALIDATION");
			addAPPlibNoDuplicates(tmplist,"VIEW LICENSES");
			addAPPlibNoDuplicates(tmplist,"XPERT SETUP");
		} else if(!RDAstrcmp(modx,"DATABASE"))
		{
			addAPPlibNoDuplicates(tmplist,"CHECK FILE");
			addAPPlibNoDuplicates(tmplist,"CHANGE ENGINE");
			addAPPlibNoDuplicates(tmplist,"CREATE IMPORT/EXPORT");
			addAPPlibNoDuplicates(tmplist,"MASS DELETE FILES");
			addAPPlibNoDuplicates(tmplist,"MASSDIFF");
			addAPPlibNoDuplicates(tmplist,"MASS IMPORT/EXPORT");
			addAPPlibNoDuplicates(tmplist,"REMOVE DELETED RECORDS IN MASS");
			addAPPlibNoDuplicates(tmplist,"BACKUP DATA");
			addAPPlibNoDuplicates(tmplist,"REBUILD DATFILE");
			addAPPlibNoDuplicates(tmplist,"REMOVE DELETES");
			addAPPlibNoDuplicates(tmplist,"REMOVE TRANS FILES");
			addAPPlibNoDuplicates(tmplist,"RESTRUCTURE DATFILE");
			addAPPlibNoDuplicates(tmplist,"UNDO DELETES");
			addAPPlibNoDuplicates(tmplist,"RENUMBER TRANSACTIONS");
			addAPPlibNoDuplicates(tmplist,"VIEW TRANSACTIONS");
		} else if(!RDAstrcmp(modx,"ARCHIVE"))
		{
			addAPPlibNoDuplicates(tmplist,"AUTO ARCHIVE");
			addAPPlibNoDuplicates(tmplist,"SFTPLIST");
		} else if(!RDAstrcmp(modx,"GUI"))
		{
			addAPPlibNoDuplicates(tmplist,"ADD MENU OPTION");
			addAPPlibNoDuplicates(tmplist,"DEFINE SCREENS");
			addAPPlibNoDuplicates(tmplist,"DEFINE SCREEN DEFAULTS");
			addAPPlibNoDuplicates(tmplist,"DEFINE SCROLLED LISTS");
			addAPPlibNoDuplicates(tmplist,"DEFINE LOAD VALUES");
			addAPPlibNoDuplicates(tmplist,"DEFINE MENU");
		} else if(!RDAstrcmp(modx,"OLHELP"))
		{
			addAPPlibNoDuplicates(tmplist,"RDA NEWS");
			addAPPlibNoDuplicates(tmplist,"DEFINE HELP");
		} else if(!RDAstrcmp(modx,"SECURITY"))
		{
			addAPPlibNoDuplicates(tmplist,"CREATE PROCSEC");
			addAPPlibNoDuplicates(tmplist,"CREATE FILESEC");
			addAPPlibNoDuplicates(tmplist,"CREATE FIELDSEC");
		}
		for(y=0;y<3;++y)
		{
		switch(y)
		{	
			case 0:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
				sprintf(libx,"%s/rda/%s.MTN",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,modx);
#endif
				break;
			case 1:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+20);
#ifndef WIN32
				sprintf(libx,"%s/site/%s.MTN",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\site\\%s.MTN",CURRENTDIRECTORY,modx);
#endif
				break;
			case 2:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+RDAstrlen(USERLOGIN)+10);
#ifndef WIN32
				sprintf(libx,"%s/%s/%s.MTN",CURRENTDIRECTORY,USERLOGIN,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\%s\\%s.MTN",CURRENTDIRECTORY,USERLOGIN,modx);
#endif
				break;
		}
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs!=NULL)
		{
			for(x=0;x<file_defs->numlibs;++x)
			{
				addAPPlibNoDuplicates(tmplist,file_defs->libs[x]);
			}
			freeapplib(file_defs);
		}
		switch(y)
		{	
			case 0:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
				sprintf(libx,"%s/rda/%s.MNU",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,modx);
#endif
				break;
			case 1:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+20);
#ifndef WIN32
				sprintf(libx,"%s/site/%s.MNU",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\site\\%s.MNU",CURRENTDIRECTORY,modx);
#endif
				break;
			case 2:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+RDAstrlen(USERLOGIN)+20);
#ifndef WIN32
				sprintf(libx,"%s/%s/%s.MNU",CURRENTDIRECTORY,USERLOGIN,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\%s\\%s.MNU",CURRENTDIRECTORY,USERLOGIN,modx);
#endif
				break;
		}
		file_defs=getlibnames(libx,TRUE);
		if(file_defs!=NULL)
		{
			for(x=0;x<file_defs->numlibs;++x)
			{
				if(menu!=NULL) { free_menu(menu);menu=NULL; }
				menu=RDAmenuNEW(file_defs->libs[x],NULL);
				if(!getmenubin(libx,menu))
				{
					if(!strncmp(menu->command,"domenu",5) || !strncmp(menu->command,"DOMENU",5))
					{
						tc_count=0;
						tc=menu->command;
						for(;*tc;++tc) if(*tc=='\"') ++tc_count;
						if(tc_count>2)
						{
							tmp=strchr(menu->command,'\"');
							++tmp;
							--tc_count;

							while(tc_count>1)
							{
								tmp=strchr(tmp,'\"');
								++tmp;
								--tc_count;
							}
						} else {
							tmp=strchr(menu->command,'\"');
						}
						if(tmp!=NULL)
						{
							++tmp;
							if(*tmp)
							{
							temp=stralloc(tmp);
						
							temp[RDAstrlen(tmp)-1]=0;
							addAPPlibNoDuplicates(tmplist,temp);
							if(temp!=NULL) Rfree(temp);
							}
						} else if(!RDAstrcmp(menu->command,"domenu.EXT"))
						{
							addAPPlibNoDuplicates(tmplist,"MAIN MENU");
						}
					} else if(!strncmp(menu->command,"rdaout",6) || 
						!strncmp(menu->command,"dfmatrix",8) ||
						!strncmp(menu->command,"dfrpt",5) ||
						!strncmp(menu->command,"dfvir",5) ||
						!strncmp(menu->command,"ezrpt",5) ||
						!strncmp(menu->command,"laser",5) ||
						!strncmp(menu->command,"doreport",8) ||
						!strncmp(menu->command,"setprt",6) ||
						!strncmp(menu->command,"dfie",4) ||
						!strncmp(menu->command,"remdel",6) ||
						!strncmp(menu->command,"undel",5) ||
						!strncmp(menu->command,"rbdfil",6) ||
						!strncmp(menu->command,"restruct",8) ||
						!strncmp(menu->command,"dfmtn",5) ||
						!strncmp(menu->command,"dfadd",5) ||
						!strncmp(menu->command,"dfnrd",5) ||
						!strncmp(menu->command,"dfscn",5) ||
						!strncmp(menu->command,"dfgsv",5) ||
						!strncmp(menu->command,"dfwww",5) ||
						!strncmp(menu->command,"dfmenu",6) ||
						!strncmp(menu->command,"dfacct",6) ||
						!strncmp(menu->command,"dfload",6) ||
						!strncmp(menu->command,"chgengine",9) ||
						!strncmp(menu->command,"dfscndef",8) ||
						!strncmp(menu->command,"rdadiag",7) ||
						!strncmp(menu->command,"dfprt",5) ||
						!strncmp(menu->command,"dfhelp",6) ||
						!strncmp(menu->command,"rnumtran",8) ||
						!strncmp(menu->command,"dat2conv",8) ||
						!strncmp(menu->command,"dobackup",8) ||
						!strncmp(menu->command,"dorestore",9) ||
						!strncmp(menu->command,"doftpzip",8) ||
						!strncmp(menu->command,"doftpget",8) ||
						!strncmp(menu->command,"convscrn",8) ||
						!strncmp(menu->command,"convmenu",8) ||
						!strncmp(menu->command,"viewtran",8) ||
						!strncmp(menu->command,"undotran",8) ||
						!strncmp(menu->command,"xpertstp",8) ||
						!strncmp(menu->command,"pay.hppeck",8) ||
						!strncmp(menu->command,"pay.hppeck1",9) ||
						!strncmp(menu->command,"pay.hppeck2",9) ||
						!strncmp(menu->command,"pay.hppecka",9) ||
						!strncmp(menu->command,"dirdep",6) ||
						!strncmp(menu->command,"dirdep1",7) ||
						!strncmp(menu->command,"dirdep2",7) ||
						!strncmp(menu->command,"dirdepa",7) ||
						!strncmp(menu->command,"payschk",7) ||
						!strncmp(menu->command,"paysddn",7) ||
						!strncmp(menu->command,"payvschk",8) ||
						!strncmp(menu->command,"payvsetr",8) ||
						!strncmp(menu->command,"venschk",7) ||
						!strncmp(menu->command,"paybc",5) ||
						!strncmp(menu->command,"paysel",6) ||
						!strncmp(menu->command,"paynet",6) ||
						!strncmp(menu->command,"paymts",6) ||
						!strncmp(menu->command,"payunmts",8) ||
						!strncmp(menu->command,"budsel",6) ||
						!strncmp(menu->command,"budnet",6) ||
						!strncmp(menu->command,"budmts",6) ||
						!strncmp(menu->command,"budunmts",8) ||
						!strncmp(menu->command,"dftable",7) ||
						!strncmp(menu->command,"doarchive",9) || 
						!strncmp(menu->command,"dohttp",6))
					{
						addAPPlibNoDuplicates(tmplist,menu->name);
					}
				}
				tmp=NULL;
				if(menu!=NULL) free_menu(menu);
			}
			freeapplib(file_defs);
		}
		if(libx!=NULL) Rfree(libx);
		switch(y)
		{	
			case 0:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
				sprintf(libx,"%s/rda/%s.CHG",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\rda\\%s.CHG",CURRENTDIRECTORY,modx);
#endif
				break;
			case 1:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
				sprintf(libx,"%s/site/%s.CHG",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\site\\%s.CHG",CURRENTDIRECTORY,modx);
#endif
				break;
			case 2:
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+RDAstrlen(USERLOGIN)+10);
#ifndef WIN32
				sprintf(libx,"%s/%s/%s.CHG",CURRENTDIRECTORY,USERLOGIN,modx);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\%s\\%s.CHG",CURRENTDIRECTORY,USERLOGIN,modx);
#endif
				break;
		}
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs!=NULL)
		{
			for(x=0;x<file_defs->numlibs;++x)
			{
				addAPPlibNoDuplicates(tmplist,file_defs->libs[x]);
			}
			freeapplib(file_defs);
		}
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.ADD",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.ADD",CURRENTDIRECTORY,modx);
#endif
		switch(y)
		{	
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
		}
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs!=NULL)
		{
			for(x=0;x<file_defs->numlibs;++x)
			{
				addAPPlibNoDuplicates(tmplist,file_defs->libs[x]);
			}
			freeapplib(file_defs);
		}
		}
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.IMP",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.IMP",CURRENTDIRECTORY,modx);
#endif
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs!=NULL)
		{
			for(x=0;x<file_defs->numlibs;++x)
			{
				addAPPlibNoDuplicates(tmplist,file_defs->libs[x]);
			}
			freeapplib(file_defs);
		}
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.MTX",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.MTX",CURRENTDIRECTORY,modx);
#endif
		file_defs=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_defs!=NULL)
		{
			for(x=0;x<file_defs->numlibs;++x)
			{
				addAPPlibNoDuplicates(tmplist,file_defs->libs[x]);
			}
			freeapplib(file_defs);
		}
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+4);
#ifndef WIN32
		sprintf(libx,"%s/%s/",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s\\",CURRENTDIRECTORY,modx);
#endif
		tmp=NULL;
		for(x=0;findfile(libx,"*.RPT",&tmp,(int)x+1);++x) 
		{
			tmp[RDAstrlen(tmp)-4]=0;
			temp=stripdashes(tmp);
			addAPPlibNoDuplicates(tmplist,temp);
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
		}
		if(tmp!=NULL) Rfree(tmp);
		if(libx!=NULL) Rfree(libx);
		if(temp!=NULL) Rfree(temp);
		SORTAPPlib(tmplist);
	}
	return(tmplist);
}
#define BACKUP_A_DIRECTORY
APPlib *RemoteDirectoryContentsAvailable(char *modexp,void (*SubFunc)(...),void *args,short style)
{
#ifdef __USE_SFTPLIST__
#ifndef WIN32
	char *work_ip=NULL,*logname=NULL,*pass_word=NULL,*docfolder=NULL;
	char buffer[4096],temp[1024];
	int y=0;
	APPlib *tmplist=NULL;
	FILE *fp=NULL;
	
	tmplist=APPlibNEW();
	work_ip=WorkStationIPAddr();
	logname=UsersWorkstationLogin();
	pass_word=UsersWorkstationPassword();
	docfolder=UsersWorkstationDocuments();
	memset(temp,0,1024);
	sprintf(temp,"%s/sftplist.lnx \"%s\" \"%s\" \"%s\" \"%s\"",(DOCUMENTROOT!=NULL ? DOCUMENTROOT:""),(work_ip!=NULL ? work_ip:""),(docfolder!=NULL ? docfolder:""),(logname!=NULL ? logname:""),(pass_word!=NULL ? pass_word:""));
	fp=popen(temp,"r");
	if(fp!=NULL)
	{
		while(fgets(buffer,sizeof(buffer),fp)!=NULL)
		{
			for(y=0;y<RDAstrlen(buffer);++y)
			{
				if(buffer[y]=='\n' || buffer[y]=='\r') buffer[y]=0;
			}
			addAPPlib(tmplist,buffer);	
		}
		pclose(fp);
	}
	return(tmplist);
#else
	APPlib *tmplist=NULL;

	tmplist=APPlibNEW();
	return(tmplist);
#endif
#else
	APPlib *tmplist=NULL;

	tmplist=APPlibNEW();
	return(tmplist);
#endif
}
APPlib *DirectoryContentsAvailable(char *modexp,void (*SubFunc)(...),void *args,short style)
{
	APPlib *tmplist=NULL,*tmplist2=NULL;
	int x=0,y=0;
	char *dirx=NULL,*tmp=NULL,*srx=NULL,*pattern=NULL;
	char *dirx1=NULL,*tmp2=NULL;
#ifdef BACKUP_A_DIRECTORY
	char *cd=NULL,*cd1=NULL;
#endif /* BACKUP_A_DIRECTORY */

	tmplist=APPlibNEW();
	srx=PP_EVALUATEstr(modexp,SubFunc,args);
	if(isEMPTY(srx)) return(tmplist);
	dirx=Rmalloc(RDAstrlen(srx)+6);
#ifndef WIN32
	sprintf(dirx,"%s/",(srx!=NULL ? srx:""));
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",(srx!=NULL ? srx:""));
#endif
	switch(style)
	{
		default:
		case 0:
			pattern=stralloc("*");
			for(x=0;findfile(dirx,pattern,&tmp,(int)x+1);++x) 
			{
				addAPPlib(tmplist,tmp);
			}
			break;
		case 1: /* PDF's */
			pattern=stralloc("*.PDF");
			for(x=0;findfile(dirx,pattern,&tmp,(int)x+1);++x) 
			{
				addAPPlib(tmplist,tmp);
			}
			break;
		case 2: /* TEXT */
			pattern=stralloc("*.TXT");
			for(x=0;findfile(dirx,pattern,&tmp,(int)x+1);++x) 
			{
				addAPPlib(tmplist,tmp);
			}
			pattern=stralloc("*.TEXT");
			for(x=0;findfile(dirx,pattern,&tmp,(int)x+1);++x) 
			{
				addAPPlib(tmplist,tmp);
			}
			break;
		case 3: /* RECURSIVELY */
			pattern=stralloc("*");
			for(x=0;findfile(dirx,pattern,&tmp,(int)x+1);++x) 
			{
				addAPPlib(tmplist,tmp);
				if(!RDAstrcmp(tmp,".") || !RDAstrcmp(tmp,".."))
				{
				} else {
					dirx1=Rmalloc(RDAstrlen(dirx)+RDAstrlen(tmp)+10);
#ifndef WIN32
					sprintf(dirx1,"\"%s%s\"",dirx,tmp);
#endif
#ifdef WIN32
					sprintf(dirx1,"\"%s%s\"",dirx,tmp);
#endif
					tmplist2=DirectoryContentsAvailable(dirx1,SubFunc,args,3);
					
					if(tmplist2!=NULL)
					{
						for(y=0;y<tmplist2->numlibs;++y)
						{
							if(RDAstrcmp(tmplist2->libs[y],".") && RDAstrcmp(tmplist2->libs[y],".."))
							{
								tmp2=Rmalloc(RDAstrlen(tmplist2->libs[y])+RDAstrlen(tmp)+5);
#ifndef WIN32
								sprintf(tmp2,"%s/%s",tmp,tmplist2->libs[y]);
#endif 
#ifdef WIN32
								sprintf(tmp2,"%s\\%s",tmp,tmplist2->libs[y]);
#endif
								addAPPlib(tmplist,tmp2);
								if(tmp2!=NULL) Rfree(tmp2);
							}
						}
						freeapplib(tmplist2);
					}
					if(dirx1!=NULL) Rfree(dirx1);
				}
			}
			break;
		case 4: /* DIRECTORIES ONLY */
			pattern=stralloc("*");
			for(x=0;(findfile(dirx,pattern,&tmp,(int)x+1));++x) 
			{
				if(!RDAstrcmp(tmp,"."))
				{
					addAPPlib(tmplist,srx);
				} else if(!RDAstrcmp(tmp,".."))
				{
#ifdef BACKUP_A_DIRECTORY 
					tmp2=Rmalloc(RDAstrlen(srx)+5);
#ifndef WIN32
					sprintf(tmp2,"%s/..",srx);
#endif 
#ifdef WIN32
					sprintf(tmp2,"%s\\..",srx);
#endif
					
					cd=stralloc(CURRENTDIRECTORY);
					RCHGCWD(tmp2);
					SETCWD();
					cd1=stralloc(CURRENTDIRECTORY);
					addAPPlib(tmplist,cd1);
					RCHGCWD(cd);
					if(cd!=NULL) Rfree(cd);
					if(cd1!=NULL) Rfree(cd1);
					if(tmp2!=NULL) Rfree(tmp2);
#endif /* BACKUP_A_DIRECTORY */
				} else {
					dirx1=Rmalloc(RDAstrlen(dirx)+RDAstrlen(tmp)+10);
#ifndef WIN32
					sprintf(dirx1,"\"%s%s\"",dirx,tmp);
#endif
#ifdef WIN32
					sprintf(dirx1,"\"%s%s\"",dirx,tmp);
#endif
					tmplist2=DirectoryContentsAvailable(dirx1,SubFunc,args,4);
					
					if(tmplist2!=NULL && tmplist2->numlibs>0)
					{
						tmp2=Rmalloc(RDAstrlen(dirx)+RDAstrlen(tmp)+10);
#ifndef WIN32
						sprintf(tmp2,"%s%s",dirx,tmp);
#endif
#ifdef WIN32
						sprintf(tmp2,"%s%s",dirx,tmp);
#endif
						addAPPlib(tmplist,tmp2);
						if(tmp2!=NULL) Rfree(tmp2);
					}
					if(tmplist2!=NULL) freeapplib(tmplist2);
					if(dirx1!=NULL) Rfree(dirx1);
				}
			}
			break;
		case 5: /* NO DIRECTORIES */
			pattern=stralloc("*");
			for(x=0;findfile(dirx,pattern,&tmp,(int)x+1);++x) 
			{
				if(!RDAstrcmp(tmp,".") || !RDAstrcmp(tmp,".."))
				{
				} else {
					dirx1=Rmalloc(RDAstrlen(dirx)+RDAstrlen(tmp)+10);
#ifndef WIN32
					sprintf(dirx1,"\"%s%s\"",dirx,tmp);
#endif
#ifdef WIN32
					sprintf(dirx1,"\"%s%s\"",dirx,tmp);
#endif
					tmplist2=DirectoryContentsAvailable(dirx1,SubFunc,args,0);
					
					if(tmplist2!=NULL)
					{
						if(tmplist2->numlibs<1)
						{
							addAPPlib(tmplist,tmp);
						}
						if(tmplist2!=NULL) freeapplib(tmplist2);
					} else {
						addAPPlib(tmplist,tmp);
					}
					if(dirx1!=NULL) Rfree(dirx1);
				}
			}
			break;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(srx!=NULL) Rfree(srx);
	if(pattern!=NULL) Rfree(pattern);
	SORTAPPlib(tmplist);
	return(tmplist);
}
void xCreateRDAScrolledList(RDArsrc *r,RDAScrolledList *list,void (*SubFunc)(...),
	void *args,int line,char *file)
{
	short keyno=(-1);
	char *temp=NULL,*libx=NULL;
	APPlib *tmplist=NULL;
	short tempsht=0;
	short ef=0,len=0;
	void *targetkey=NULL;
        int x,y;
        RDAacct *acct=NULL;
	RDArmem *mem=NULL;
	RDApdev *pdev=NULL;
	short s=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglist)
	{
		prterr("DIAG xCreateRDAScrolledList for Screen [%s] [%s] List [%s] line [%d] program [%s]",
			(r->module==NULL?"":r->module),
			(r->screen==NULL?"":r->screen),
			(list->name==NULL?"":list->name),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(list->dlist!=NULL)
	{
		freeapplib(list->dlist);
	}
	switch(list->type)
	{
		default:
		case 0: /* list connecting FROM a file's field */
			list->dlist=APPlibNEW();
			if(list->ffinfo->fileno!=(-1))
			{
				keyno=KEYNUMBER(list->ffinfo->fileno,
					list->ffinfo->keyname);
				if(targetkey==NULL && list->contype!=3)
				{
					targetkey=GETNRDKEY(list->ffinfo->fileno,
						keyno,&len);
				}
				ZERNRD(list->ffinfo->fileno);
/* Establish Initialization Here */

/* Loop thru file */
				ef=ADVGTENRDsec(list->ffinfo->fileno,keyno,SubFunc,args);
				if(targetkey==NULL && !ef && list->contype==3)
				{
					targetkey=GETNRDKEY(list->ffinfo->fileno,
						keyno,&len);
				}
				while(!ef)
				{
/* Break on change from Initialization Here */
					if(!isEMPTY(list->select_formula))
					{
						if(PP_EVALUATEbol(list->select_formula,SubFunc,args))
						{
							if(!isEMPTY(list->format_formula))
							{
								temp=PP_EVALUATEstr(list->format_formula,SubFunc,args);
								addAPPlib(list->dlist,temp);
								if(temp!=NULL) { Rfree(temp); temp=NULL; }
							}
						}
					} else {
						if(!isEMPTY(list->format_formula))
						{
							temp=PP_EVALUATEstr(list->format_formula,SubFunc,args);
							addAPPlib(list->dlist,temp);
							if(temp!=NULL) { Rfree(temp); temp=NULL; }
						}
					}
					ef=ADVNXTNRDsec(list->ffinfo->fileno,keyno,SubFunc,args);
				}
			}
			EQLNRDKEY(list->ffinfo->fileno,keyno,targetkey);
			if(targetkey!=NULL) Rfree(targetkey);
			break;
		case 1: /* List from User Defined Arrary of Strings */
			if(list->list==NULL) 
			{
				list->dlist=APPlibNEW();
				prterr("WARNING: User Defined RDAScrolledList [%s] without any defined elements.",list->name);
			} else {
				list->dlist=CopyAPPlib(list->list);
			}
			break; 
		case 3:
			if(list->list!=NULL) 
			{
				list->dlist=APPlibNEW();
				for(y=0;y<list->list->numlibs;++y)
				{
					temp=PP_EVALUATEstr(list->list->libs[y],SubFunc,args);
					if(!isEMPTY(temp))
					{
						addAPPlib(list->dlist,temp);
					}
					if(temp!=NULL) Rfree(temp);
				}
			}
			if(list->dlist==NULL) 
			{
				list->dlist=APPlibNEW();
				prterr("WARNING: Special Type RDAScrolledList [%s] without any defined elements.",list->name);
			}
			break;
		case 2: /* Special Type List */
			list->dlist=APPlibNEW();
			if(!RDAstrcmp(list->special_function,"DEFINITION TYPES"))
			{
				tempsht=PP_EVALUATEsht(list->select_formula,SubFunc,args);
				switch(tempsht)
				{
					case EXP_ACCT : 
						if(EXPENDITURE_ACCOUNT->codetype!=NULL)
						{
							for(x=0,acct=EXPENDITURE_ACCOUNT->codetype;
								x<EXPENDITURE_ACCOUNT->num;++x,++acct)
							{
								addAPPlib(list->dlist,acct->name);
							}
						} 
						break;
					case REV_ACCT : 
						if(REVENUE_ACCOUNT->codetype!=NULL)
						{
							for(x=0,acct=REVENUE_ACCOUNT->codetype;
								x<REVENUE_ACCOUNT->num;++x,++acct)
							{
								addAPPlib(list->dlist,acct->name);
							}
						}
						break;
					case BAL_ACCT : 
						if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
						{
							for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
								x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
							{
								addAPPlib(list->dlist,acct->name);
							}
						}
						break;
					case BEG_ACCT : 
						if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
						{
							for(x=0,acct=BEGINNING_BALANCE_ACCOUNT->codetype;
								x<BEGINNING_BALANCE_ACCOUNT->num;++x,++acct)
							{
								addAPPlib(list->dlist,acct->name);
							}
						}
						break;
					case CUSTOM_ACCT:
						if(CUSTOM_INPUTS_ACCOUNT->codetype!=NULL)
						{
							for(x=0,acct=CUSTOM_INPUTS_ACCOUNT->codetype;
								x<CUSTOM_INPUTS_ACCOUNT->num;++x,++acct)
							{
								addAPPlib(list->dlist,acct->name);
							}
						}
						break;
					default : 
                        			prterr("WARNING: Invalid Account Type [%d], can't make list.",tempsht);

						break;
				}
			} else  if(!RDAstrcmp(list->special_function,"OUTPUT DEVICES"))
			{
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
				sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
				sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
				tmplist=getlibnames(libx,FALSE);
				if(tmplist!=NULL)
				{
					for(x=0;x<tmplist->numlibs;++x)
					{
						pdev=RDApdevNEW(tmplist->libs[x]);
						if(getpdevbin(pdev))
						{
						} else if(pdev->active==TRUE)
						{
							addAPPlib(list->dlist,tmplist->libs[x]);
						}
						if(pdev!=NULL) free_pdev(pdev);
					}
					freeapplib(tmplist);
				}
				if(libx!=NULL) Rfree(libx);
				if(XPERT_SETUP->ARCHIVE==TRUE)
				{
					if(!RDAstrcmp(list->conmod,"SECURITY") && !RDAstrcmp(list->confil,"USERS") && !RDAstrcmp(list->confld,"DEFAULT PRINTER"))
					{
																addAPPlib(list->dlist,"AUTOARCHIVE");
					}
				}
			} else if(!isEMPTY(list->special_function) && RDAstrcmp(list->special_function,"No Special Function"))
			{
				if(!RDAstrcmp(list->special_function,"MODULES AVAILABLE"))
				{
					tmplist=FindModulesAvailable();
				} else if(!RDAstrcmp(list->special_function,"FILES AVAILABLE"))
				{
					tmplist=FindFilesAvailable(list->select_formula,SubFunc,args);
				} else if(!RDAstrcmp(list->special_function,"FIELDS AVAILABLE"))
				{
					tmplist=GetFieldsAvailable(list->select_formula,SubFunc,args);
				} else  if(!RDAstrcmp(list->special_function,"PROCESSES AVAILABLE"))
				{
					tmplist=GetProcessesAvailable(list->select_formula,SubFunc,args);
				} else if(!RDAstrcmp(list->special_function,"MATRIXES AVAILABLE"))
				{
					tmplist=FindMatrixAvailable(list->select_formula,SubFunc,args);
				} else if(!RDAstrcmp(list->special_function,"TABLES AVAILABLE"))
				{
					tmplist=FindTablesAvailable(list->select_formula,SubFunc,args);
				} else if(!RDAstrcmp(list->special_function,"DIRECTORY CONTENTS"))
				{
					tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,0);
				} else if(!RDAstrcmp(list->special_function,"DIRECTORY RECURSIVELY"))
				{
					tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,3);
				} else if(!RDAstrcmp(list->special_function,"DIRECTORIES ONLY"))
				{
					tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,4);
				} else if(!RDAstrcmp(list->special_function,"NO DIRECTORIES"))
				{
					tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,5);
				} else if(!RDAstrcmp(list->special_function,"PDF(S) IN DIRECTORY"))
				{
					tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,1);
				} else if(!RDAstrcmp(list->special_function,"TXT(S) IN DIRECTORY"))
				{
					tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,2);
				} else if(!RDAstrcmp(list->special_function,"WORKSTATION DIRECTORY CONTENTS (SFTP)"))
				{
					s=FINDRSC(r,list->name);
					if(s>(-1))
					{
						mem=r->rscs+s;
						if((isEMPTY(mem->editable_expression) && isEMPTY(mem->sensitive_expression)) || (mem->editable==TRUE && mem->sensitive==TRUE))
						{
							tmplist=RemoteDirectoryContentsAvailable(list->select_formula,SubFunc,args,0);
						} else {
							tmplist=APPlibNEW();
						}
					} else { 
						tmplist=APPlibNEW();
					}
				}
				if(tmplist!=NULL)
				{
					for(x=0;x<tmplist->numlibs;++x)
					{
						addAPPlib(list->dlist,tmplist->libs[x]);
					}
					freeapplib(tmplist);
				}
			}
			if(list->dlist==NULL) 
			{
				list->dlist=APPlibNEW();
				prterr("WARNING: Special Type RDAScrolledList [%s] without any defined elements.",list->name);
			}
			break;
	}
	if(list->dlist->numlibs<1)
	{
		temp=Rmalloc(RDAstrlen(list->name)+19);
		sprintf(temp,"No %s List Available",list->name);
		addAPPlib(list->dlist,temp);
		if(temp!=NULL) Rfree(temp);
	}
}
void xADVSetRDAScrolledList(RDArsrc *r,RDAScrolledList *list,short runfunc,int line,char *file)
{
	int selected=0,y=0,lf=0;
	char *temp=NULL;
	RDArmem *mem=NULL,*lmem=NULL;
	void *value=NULL;
	short fileno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG ADVSetRDAScrolledList at line [%d] program [%s].",
			line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	FINDRSCGETINT(r,list->name,&selected);
	y=FINDRSC(r,list->name);
	if(y!=(-1)) lmem=r->rscs+y;
	switch(list->contype)
	{
		default:
		case 0: /* Connect Normal Field */
		case 1: /* Connect Special Field */
		case 2: /* Connect Virtual Field */
		case 3: /* Connect Report Generator Range Screen */
			if(!list->contype)
			{
				temp=Rmalloc(RDAstrlen(list->confil)+
					RDAstrlen(list->confld)+7);
				sprintf(temp,"[%s][%s]",list->confil,list->confld);
			} else {
				temp=stralloc(list->conmod);
			}
			y=FINDRSC(r,temp);
			if(temp!=NULL) Rfree(temp);
			if(y!=(-1))
			{
				mem=r->rscs+y;
				if(lmem->Table!=NULL)
				{
					mem->Table=lmem->Table;
					mem->table_row=lmem->table_row;
					mem->table_col=lmem->table_col;
				}
				value=list->dlist->libs[selected];
				switch(mem->field_type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case SOCSECNUM:
					case TIMES:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(!isEMPTY(list->unformat_formula))
						{
							value=PP_EVALUATEstr(list->unformat_formula,
								list->SubFunc,
								list->args);
						} 
						if(value!=NULL) lf=RDAstrlen(value);
							else lf=0;
						if(value!=NULL && lf>0)
						{
							QUICKALLOC(mem->value.string_value,mem->dlen,lf+1);
							memcpy(mem->value.string_value,value,lf+1);
						} else if(mem->dlen>0)
						{
							memset(mem->value.string_value,0,mem->dlen);
						} else {
							mem->value.string_value=NULL;
							mem->dlen=0;
						}
						if(mem->dlen>mem->field_length && mem->field_length>0) mem->value.string_value[mem->field_length]=0;
						if(!isEMPTY(list->unformat_formula))
						{
							if(value!=NULL) Rfree(value);
						}
						if(fileno!=(-1))
							FINDFLDSETSTRING(fileno,list->confld,mem->value.string_value);
						break;
					case BOOLNS:
					case CHARACTERS:
						if(!isEMPTY(list->unformat_formula))
						{
							*mem->value.string_value=
								PP_EVALUATEbol(list->unformat_formula,
									list->SubFunc,list->args);
						} else *mem->value.string_value=selected;
						if(fileno!=(-1))
							FINDFLDSETCHAR(fileno,list->confld,*mem->value.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case SDECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
						if(!isEMPTY(list->unformat_formula))
						{
							*mem->value.float_value=PP_EVALUATEdbl(list->unformat_formula,list->SubFunc,list->args);
						} else *mem->value.float_value=selected;
						if(fileno!=(-1))
							FINDFLDSETDOUBLE(fileno,list->confld,*mem->value.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(!isEMPTY(list->unformat_formula))
						{
							*mem->value.short_value=PP_EVALUATEsht(list->unformat_formula,list->SubFunc,list->args);
						} else *mem->value.short_value=selected;
						if(fileno!=(-1))
							FINDFLDSETSHORT(fileno,list->confld,*mem->value.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(!isEMPTY(list->unformat_formula))
						{
							*mem->value.integer_value=PP_EVALUATEint(list->unformat_formula,list->SubFunc,list->args);
						} else *mem->value.integer_value=selected;
						if(fileno!=(-1))
							FINDFLDSETINT(fileno,list->confld,*mem->value.integer_value);
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Resource [%s].",mem->rscrname);
						break;
				}
				if(IsWidgetDisplayed(mem)) updatemember(mem);
				if(runfunc && list->contype!=3)
				{
					if(mem->function!=NULL)
					{
						ExecuteRDArmemFunction(mem);
					} else {
						ComputeSCRNcallback(r,NULL);
					}
				} else if(list->contype==3)
				{
					if(mem->function!=NULL)
					{
						ExecuteRDArmemFunction(mem);
					} else {
						readallwidgets(r);
/*
						ADVupdateSCRNvirtuals(r,r->SubFunc,r->SubFuncArgs);
*/
						SensitiveADVcomputeandupdateallSCRNvirtuals(r,r->SubFunc,r->SubFuncArgs,TRUE,TRUE);
					}
				}
			}
			break;
	}
}
void SetRDAScrolledList(RDArsrc *r,RDAScrolledList *list)
{
	ADVSetRDAScrolledList(r,list,TRUE);
}
void xSCRNvirtual2rsrc(RDArsrc *r,void (*SubFunc)(...),void *args,int line,char *file)
{
	int x,y=(-1),start=0;
	short s=FALSE;
	char *temp=NULL,*libx=NULL;
	APPlib *tmplist=NULL;
	RDAvirtual *v;
	RDAScrolledList *list=NULL;
	RDALoadValue *load=NULL;
	short edit_rsrc=TRUE,filenum=(-1);
        int z,p=(-1);
	RDArmem *mem1=NULL,*mem=NULL;
        RDAacct *acct=NULL;
	short tempsht=0;
	RDApdev *pdev=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG SCRNvirtual2rsrc Creates Resources for Virtual Fields on Screen [%s] [%s] from virtual field list at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->virflds!=NULL)
		{
			for(x=0,v=r->virflds;x<r->num;++x,++v)
			{
				makefieldrsrc(r,v->name,v->type,v->len,FALSE);
			}
		}
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
				if(list->timing<2)
				{
					CreateRDAScrolledList(r,list,SubFunc,args);
				} else {
					if(list->dlist==NULL)
					{
						list->dlist=APPlibNEW();
					} 
				}
				if(SubFunc!=NULL)
				{
					list->SubFunc=SubFunc;
					list->args=args;
				} else {
					list->SubFunc=SCRNvirtualSubData;
					list->args=r;
				}
				y=FINDRSC(r,list->name);
				switch(list->type)
				{
					default:
					case 0: /* list connecting FROM a file's field */
						if(!list->contype) /* connecting TO a file field */
						{
							filenum=SCRNFILENUMBER(r,list->conmod,list->confil,1);
							if(FIELDscrn(filenum,list->confld,&edit_rsrc))
							{
								if(y==(-1))
								{
									temp=Rmalloc(RDAstrlen(list->confil)+RDAstrlen(list->confld)+7);
									sprintf(temp,"[%s][%s]",(list->confil!=NULL?list->confil:""),(list->confld!=NULL?list->confld:""));
									p=FINDRSC(r,temp);
									if(p!=(-1))
									{
										if(edit_rsrc==TRUE)
										{
											mem1=r->rscs+p;
											if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
											{
												addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											} else {
												addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											}
										} else {
											addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										}
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);
									}
									if(temp!=NULL) Rfree(temp);
								} else {
									FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
									updatersrc(r,list->name);
								}
							}
						} else {
							if(y==(-1))
							{
								temp=Rmalloc(RDAstrlen(list->conmod)+3);
								sprintf(temp,"%s",(list->conmod!=NULL?list->conmod:""));
								p=FINDRSC(r,temp);
								if(p!=(-1))
								{
									mem1=r->rscs+p;
									if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
									{
										addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									}
								} else {
									addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);
								}
								if(temp!=NULL) Rfree(temp);
							} else {
								FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
								updatersrc(r,list->name);
							}
						}
						break;
					case 2: /* connected FROM Special */
						if(y==(-1))
						{
							addlstrsrc(r,list->name,&start,(edit_rsrc ? TRUE:FALSE),
								SetRDAScrolledList,list->dlist->numlibs,
								&list->dlist->libs,list);
						} else {
							if(list->dlist!=NULL)
							{
								freeapplib(list->dlist);
							}
							list->dlist=APPlibNEW();
							if(!RDAstrcmp(list->special_function,"DEFINITION TYPES"))
							{
								tempsht=PP_EVALUATEsht(list->select_formula,SubFunc,args);
								switch(tempsht)
								{
									case EXP_ACCT : 
										if(EXPENDITURE_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=EXPENDITURE_ACCOUNT->codetype;
												z<EXPENDITURE_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										} 
										break;
									case REV_ACCT : 
										if(REVENUE_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=REVENUE_ACCOUNT->codetype;
												z<REVENUE_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									case BAL_ACCT : 
										if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
												z<BALANCE_SHEET_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									case BEG_ACCT : 
										if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=BEGINNING_BALANCE_ACCOUNT->codetype;
												z<BEGINNING_BALANCE_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									case CUSTOM_ACCT:
										if(CUSTOM_INPUTS_ACCOUNT->codetype!=NULL)
										{
											for(x=0,acct=CUSTOM_INPUTS_ACCOUNT->codetype;
												x<CUSTOM_INPUTS_ACCOUNT->num;++x,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									default : 
                        							prterr("WARNING: Invalid Account Type [%d], can't make list.",tempsht);
										break;
								}
							} else if(!RDAstrcmp(list->special_function,"OUTPUT DEVICES"))
							{
								libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
								sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
								sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
								tmplist=getlibnames(libx,FALSE);
								if(tmplist!=NULL)
								{
									for(x=0;x<tmplist->numlibs;++x)
									{
										pdev=RDApdevNEW(tmplist->libs[x]);
										if(getpdevbin(pdev))
										{
										} else if(pdev->active==TRUE)
										{
											addAPPlib(list->dlist,tmplist->libs[x]);
										}
										if(pdev!=NULL) free_pdev(pdev);
									}
									freeapplib(tmplist);
								}
								if(libx!=NULL) Rfree(libx);
								if(XPERT_SETUP->ARCHIVE==TRUE)
								{
									if(!RDAstrcmp(list->conmod,"SECURITY") && !RDAstrcmp(list->confil,"USERS") && !RDAstrcmp(list->confld,"DEFAULT PRINTER"))
									{
																				addAPPlib(list->dlist,"AUTOARCHIVE");
									}
								}
							} else if(!isEMPTY(list->special_function) && RDAstrcmp(list->special_function,"No Special Function"))
							{
								if(!RDAstrcmp(list->special_function,"MODULES AVAILABLE"))
								{
									tmplist=FindModulesAvailable();
								} else if(!RDAstrcmp(list->special_function,"FILES AVAILABLE"))
								{
									tmplist=FindFilesAvailable(list->select_formula,SubFunc,args);
								} else if(!RDAstrcmp(list->special_function,"FIELDS AVAILABLE"))
								{
									tmplist=GetFieldsAvailable(list->select_formula,SubFunc,args);
								} else if(!RDAstrcmp(list->special_function,"PROCESSES AVAILABLE"))
								{
									tmplist=GetProcessesAvailable(list->select_formula,SubFunc,args);
								} else if(!RDAstrcmp(list->special_function,"MATRIXES AVAILABLE"))
								{
									tmplist=FindMatrixAvailable(list->select_formula,SubFunc,args);
								} else if(!RDAstrcmp(list->special_function,"TABLES AVAILABLE"))
								{
									tmplist=FindTablesAvailable(list->select_formula,SubFunc,args);
								} else if(!RDAstrcmp(list->special_function,"DIRECTORY CONTENTS"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,0);
								} else if(!RDAstrcmp(list->special_function,"DIRECTORY RECURSIVELY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,3);
								} else if(!RDAstrcmp(list->special_function,"DIRECTORIES ONLY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,4);
								} else if(!RDAstrcmp(list->special_function,"NO DIRECTORIES"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,5);
								} else if(!RDAstrcmp(list->special_function,"PDF(S) IN DIRECTORY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,1);
								} else if(!RDAstrcmp(list->special_function,"TXT(S) IN DIRECTORY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,SubFunc,args,2);
								} else if(!RDAstrcmp(list->special_function,"WORKSTATION DIRECTORY CONTENTS (SFTP)"))
								{
									s=FINDRSC(r,list->name);
									if(s>(-1))
									{
										mem=r->rscs+s;
										if((isEMPTY(mem->editable_expression) && isEMPTY(mem->sensitive_expression)) || (mem->w!=NULL && mem->editable==TRUE && mem->sensitive==TRUE))
										{
											tmplist=RemoteDirectoryContentsAvailable(list->select_formula,SubFunc,args,0);
										} else tmplist=APPlibNEW();
									} else tmplist=APPlibNEW();
								}
								if(tmplist!=NULL)
								{
									for(x=0;x<tmplist->numlibs;++x)
									{
										addAPPlib(list->dlist,tmplist->libs[x]);
									}
									freeapplib(tmplist);
								}
							}
							if(list->dlist==NULL) 
							{
								list->dlist=APPlibNEW();
								prterr("WARNING: Special Type RDAScrolledList [%s] without any defined elements.",list->name);
								temp=Rmalloc(RDAstrlen(list->name)+19);
								sprintf(temp,"No %s List Available",list->name);
								addAPPlib(list->dlist,temp);
								if(temp!=NULL) Rfree(temp);
							} else if(list->dlist->numlibs<1)
							{
								temp=Rmalloc(RDAstrlen(list->name)+19);
								sprintf(temp,"No %s List Available",list->name);
								addAPPlib(list->dlist,temp);
								if(temp!=NULL) Rfree(temp);
							}
							FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
							updatersrc(r,list->name);
						}
						break;
					case 1: /* connect FROM Parsed List */
					case 3: /* unused */
						if(!list->contype) /* connecting TO a file field */
						{
							filenum=SCRNFILENUMBER(r,list->conmod,list->confil,1);
							if(FIELDscrn(filenum,list->confld,&edit_rsrc))
							{
								if(y==(-1))
								{
									temp=Rmalloc(RDAstrlen(list->confil)+RDAstrlen(list->confld)+7);
									sprintf(temp,"[%s][%s]",(list->confil!=NULL?list->confil:""),(list->confld!=NULL?list->confld:""));
									p=FINDRSC(r,temp);
									if(p!=(-1))
									{
										if(edit_rsrc==TRUE)
										{
											mem1=r->rscs+p;
											if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
											{
												addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											} else {
												addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											}
										} else {
											addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										}
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);
									}
								} else {
									FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
									updatersrc(r,list->name);
								}
								if(temp!=NULL) Rfree(temp);
							} /* security failed */
						} else {
							if(y==(-1))
							{
								temp=Rmalloc(RDAstrlen(list->conmod)+3);
								sprintf(temp,"%s",(list->conmod!=NULL?list->conmod:""));
								p=FINDRSC(r,temp);
								if(p!=(-1))
								{
									mem1=r->rscs+p;
									if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
									{
										addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									}
								} else {
									addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);

								}
								if(temp!=NULL) Rfree(temp);
							} else {
								FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
								updatersrc(r,list->name);
							}
						}
#ifdef RADDOG
						DefaultComputeSCRNcallback(r);
#endif
						break;
				}
			}
		}
/* SJS HERE */
		if(r->loads!=NULL)
		{
			for(x=0,load=r->loads;x<r->numloads;++x,++load)
			{
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+12);
				sprintf(temp,"%s LOAD VALUE",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadValueCB,load);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+15);
				sprintf(temp,"%s LOAD OPERATOR",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadOperatorCB,load);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+14);
				sprintf(temp,"%s LOAD GROUPER",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadGrouperCB,load);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+14);
				sprintf(temp,"%s LOAD CONTROL",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadControlCB,load);
				if(temp!=NULL) Rfree(temp);
			}
		}
	}
}
void xCLEAR_SCREEN_VIRTUAL(RDArsrc *s,int line,char *file)
{
	int x;
	RDAvirtual *v;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG CLEAR_SCREEN_VIRTUAL Setting Computed Flag on Screen Resource [%s] [%s] virtual's at line [%d] program [%s].",s->module,s->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(s!=NULL)
	{
		if(s->virflds!=NULL)
		{
			for(x=0,v=s->virflds;x<s->num;++x,++v) v->computed=FALSE;
		}
	}
}
void xSHOWvirtual(RDAvirtual *v,int line,char *file)
{
	prterr("SHOW Virtual Field [%s] at line [%d] program [%s] has ",v->name,line,file); 
	switch(v->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			prterr(" Value [%s]\n",(v->value.string_value!=NULL ? v->value.string_value:"NULL"));
			break;
		case BOOLNS:
			prterr(" Value [%d] [%s]\n",*v->value.string_value,(*v->value.string_value ? "Yes":"No"));
			break;
		case CHARACTERS:
			prterr(" Value [%c]\n",*v->value.string_value);
			break;
		case DECIMALV:
		case DOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDOUBLEV:
		case SDECIMALV:
			prterr(" Value [%f]\n",*v->value.float_value);
			break;
		case SHORTV:
		case SSHORTV:
			prterr(" Value [%d]\n",*v->value.short_value);
			break;
		case LONGV:
		case SLONGV:
			prterr(" Value [%d]\n",*v->value.integer_value);
			break;
		default:
			prterr("Error Field Type [%d] is invalid for virtual field [%s] at line [%d] program [%s].",v->type,v->name,line,file);
			break;
	}
}
void xcomputevirtual(RDAvirtual *v,void (*ReportSubFunc)(...),void *vargs,
	int line,char *file)
{
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG computevirtual Computing Virtual Field [%s] at line [%d] program [%s].",v->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	v->computed=TRUE;
	switch(v->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case PHONE:
		case CUSTOMTYPE:
		case ZIPCODE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			if(ComputeVirtualType==1) temp=PP_EVALUATEstr(v->expression,ReportSubFunc,vargs);
			else temp=EVALUATEstr(v->expression,ReportSubFunc,vargs);
			if(v->value.string_value!=NULL) 
				Rfree(v->value.string_value);
			v->value.string_value=stralloc(temp);
			if(temp!=NULL) Rfree(temp);
			if(v->value.string_value!=NULL)
			{
				v->dlen=RDAstrlen(v->value.string_value)+1;
				if(v->dlen>v->len && v->len>0)
				{
					v->value.string_value[v->len]=0;
				}
			} else v->dlen=0;
			break;
		case BOOLNS:
		case CHARACTERS:
			if(ComputeVirtualType==1) *v->value.string_value=PP_EVALUATEbol(v->expression,ReportSubFunc,vargs);
			else *v->value.string_value=EVALUATEbol(v->expression,ReportSubFunc,vargs);
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DOUBLEV:
		case SDOUBLEV:
			if(ComputeVirtualType==1) *v->value.float_value=PP_EVALUATEdbl(v->expression,ReportSubFunc,vargs);
			else *v->value.float_value=EVALUATEdbl(v->expression,ReportSubFunc,vargs);
			*v->value.float_value=round(*v->value.float_value);
			break;
		case DECIMALV:
		case SDECIMALV:
			if(ComputeVirtualType==1) *v->value.float_value=PP_EVALUATEdbl(v->expression,ReportSubFunc,vargs);
			else *v->value.float_value=EVALUATEdbl(v->expression,ReportSubFunc,vargs);
			break;
		case SHORTV:
		case SSHORTV:
			if(ComputeVirtualType==1) *v->value.short_value=PP_EVALUATEsht(v->expression,ReportSubFunc,vargs);
			else *v->value.short_value=EVALUATEsht(v->expression,ReportSubFunc,vargs);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(ComputeVirtualType==1) *v->value.integer_value=PP_EVALUATEint(v->expression,ReportSubFunc,vargs);
			else *v->value.integer_value=EVALUATEint(v->expression,ReportSubFunc,vargs);
			break;
		default:
			prterr("Error Field Type [%d] is invalid for virtual field [%s] at line [%d] program [%s].",v->type,v->name,line,file);
			break;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field) { SHOWvirtual(v); }
#endif /* USE_RDA_DIAGNOSTICS */
}
short xCOMPUTE_SCREEN_VIRTUAL(RDArsrc *r,char *name,
	void (*ReportSubFunc)(...),void *vargs,int line,char *file)
{
	RDAvirtual *v;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG Compute Screen Virtual Field [%s] for Screen [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r==NULL) 
	{
		prterr("Error RDArsrc Pointer is NULL in COMPUTE_SCREEN_VIRTUAL at line [%d] program [%s].",line,file);
		return(-1);
	}
	if(r->virflds!=NULL)
	{
		for(x=0,v=r->virflds;x<r->num;++x,++v)
		{
			if(!RDAstrcmp(v->name,name)) break;
		}
		if(x>=r->num) return(-1);
		computevirtual(v,ReportSubFunc,vargs);
		return(x);
	} else return(-1);
}
short SCRNFILENUMBER(RDArsrc *r,char *modx,char *filx,short occurrence)
{
	short fileno=(-1),count=0;
	int x;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG SCRNFILENUMBER Looking up file number for Module [%s] File [%s] occurrence [%d] used on Screen [%s] [%s].",modx,filx,occurrence,r->module,r->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,i=r->incvir;x<r->numvir;++x,++i)
	{
		if(!RDAstrcmp(i->module,modx) && !RDAstrcmp(i->file,filx)) 
		{
			++count;
			if(count==occurrence) break;
		}
	}
	if(x<r->numvir)
	{
		fileno=i->fileno;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG SCRNFILENUMBER returning Fileno [%d] for File [%s][%s].",fileno,(fileno!=(-1)?MODULENAME(fileno):""),(fileno!=(-1)?FILENAME(fileno):""));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(fileno);
}
short ScrolledListSubData(char **tmp,RDArsrc *r)
{
	int y=0,selected=0;
	RDAScrolledList *list=NULL;
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn=NULL,*modulename=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diageval)
	{
		prterr("DIAG ScrolledListSubData Expression Evaluator for RDAScrolledLists on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->lists!=NULL)
		{
			modulename=stripmodulename(*tmp);
			for(y=0,list=r->lists;y<r->numlists;++y,++list)
			{
				if(!RDAstrcmp(modulename,list->name)) break;
			}
			if(y<r->numlists)
			{
				if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
				{
					temp=*tmp+RDAstrlen(modulename)+3;
					gn=strtok(temp,",]");
					if(gn!=NULL)
					{
						if(!isEMPTY(gn))
						{
						start=Ratoi(gn);
						} else start=1;
						gn=strtok(NULL,",]");
						if(!isEMPTY(gn))
						{
						length=Ratoi(gn);
						} else length=0;
					}
					if(start<1) start=1;
				}
				FINDRSCGETINT(r,list->name,&selected);
				if(selected>=list->dlist->numlibs) selected=0;
				temp=stralloc(list->dlist->libs[selected]);
				str_length=(temp!=NULL ? RDAstrlen(temp):0);
				if(length==0) length=str_length;
				if(RDAstrlen(*tmp)<(length+3))
				{
					*tmp=Rrealloc(*tmp,length+3);
				}
				if(!isEMPTY(temp) && str_length>length)
				{
					gn=temp+(start-1);
					if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"%s\"",&temp[start-1]);
				} else if(start<=str_length && !isEMPTY(temp))
				{
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"%s\"",&temp[start-1]);
				} else {
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"\"");
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				if(modulename!=NULL) Rfree(modulename);
				return(0);
			}
		}
	}
	if(modulename!=NULL) Rfree(modulename);
	return(-1);
}
short SCRNvirtualVIRTUALSubData(char **tmp,RDArsrc *r,char *modulename)
{
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y=0;
	RDAvirtual *v=NULL;
	void (*SubFunc)(...);
	void *SubFuncArgs=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diageval)
	{
		prterr("DIAG SCRNvirtualVIRTUALSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r->SubFunc!=NULL)
	{
		SubFunc=r->SubFunc;
		SubFuncArgs=r->SubFuncArgs;
	} else {
		SubFunc=SCRNvirtualSubData;
		SubFuncArgs=r;
	}
/* BREAK_HERE virtual fields */
	if(r->virflds!=NULL && r->num>0)
	{
		for(y=0,v=r->virflds;y<r->num;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<r->num)
		{
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						temp=PP_EVALUATEstr(v->expression,SubFunc,SubFuncArgs);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						if(v->value.string_value!=NULL)
						{
							v->dlen=RDAstrlen(v->value.string_value)+1;
							if(v->dlen>v->len && v->len>0)
							{
								v->value.string_value[v->len]=0;
							}
						}
					}
					if(RDAstrlen(*tmp)>(RDAstrlen(v->name)+2))
					{
						temp=*tmp+(RDAstrlen(v->name)+3);
						gn=strtok(temp,",]");
						if(gn!=NULL)
						{
							if(!isEMPTY(gn))
							{
							start=Ratoi(gn);
							} else start=1;
							gn=strtok(NULL,"]");
							if(!isEMPTY(gn))
							{
							length=Ratoi(gn);
							} else length=0;
						}
						if(start<1) start=1;
					}
					if(length==0) length=v->dlen;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if((RDAstrlen(*tmp)>(RDAstrlen(v->name)+2)) && v->value.string_value!=NULL)
					{
						temp=stralloc(v->value.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(start-1+length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=PP_EVALUATEsht(v->expression,SubFunc,SubFuncArgs);
					}
					if(*v->value.short_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.short_value);
					} else {
						sprintf(stemp,"%d",*v->value.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.integer_value=PP_EVALUATEint(v->expression,SubFunc,SubFuncArgs);
					}
					if(*v->value.integer_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.integer_value);
					} else {
						sprintf(stemp,"%d",*v->value.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.float_value=PP_EVALUATEdbl(v->expression,SubFunc,SubFuncArgs);
						if(v->type==DOLLARS || v->type==DOLLARS_NOCENTS ||
							v->type==DOUBLEV || v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=PP_EVALUATEbol(v->expression,SubFunc,SubFuncArgs);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=PP_EVALUATEbol(v->expression,SubFunc,SubFuncArgs);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					return(1);
			}
		} else {
			return(1);
		}
	} else {
		return(1);
	}
	return(0);
}
short SCRNvirtualFIELDSubData(char **tmp,RDArsrc *r,char *modulename)
{
	char *filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diageval)
	{
		prterr("DIAG SCRNvirtualFIELDSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
/* BREAK_HERE file fields */
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	if(filename!=NULL)
	{
		fieldname=stripfieldname(*tmp);
		if(fieldname!=NULL)
		{
			if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+6))
			{
				temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
				gn=strtok(temp,",]");
				if(gn!=NULL)
				{
					if(!isEMPTY(gn))
					{
						occurrence=Ratoi(gn);
					} else { 
						occurrence=1;
					}
					gn=strtok(NULL,",]");
					if(!isEMPTY(gn))
					{
						start=Ratoi(gn);
					} else {
						start=1;
					}
					gn=strtok(NULL,",]");
					if(!isEMPTY(gn))
					{
						length=Ratoi(gn);
					} else { 
						length=0;
					}
				}
				if(start<1)
				{
					start=1;
				}
			}
			if(DO_SECURITY_FILENO!=(-1))
			{
				if(!RDAstrcmp(modulename,MODULENAME(DO_SECURITY_FILENO)) &&
					!RDAstrcmp(filename,FILENAME(DO_SECURITY_FILENO)))
				{
					fileno=DO_SECURITY_FILENO;
				} else fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
			} else fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
			if(fileno!=(-1))
			{
				field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
			} else {
				field=NULL;
			}
			if(field!=NULL)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
						temp=stralloc(field->data.string_value);
						str_length=(temp!=NULL?RDAstrlen(temp):0);
						if(length<1) length=str_length;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if(!isEMPTY(temp) && str_length>length)
						{
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(length))
								temp[start-1+length]=0;
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else if(start<=str_length && !isEMPTY(temp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else {
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"\"");
						}
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						break;
					case SHORTV:
					case SSHORTV:
						if(*field->data.short_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.short_value);
						} else {
							sprintf(stemp,"%d",*field->data.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SLONGV:
					case SCROLLEDLIST:
					case LONGV:
						if(*field->data.integer_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.integer_value);
						} else {
							sprintf(stemp,"%d",*field->data.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%c",*field->data.string_value);
						break;
					default:
						prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						if(fieldname!=NULL) Rfree(fieldname);
						if(filename!=NULL) Rfree(filename);
						return(1);
				}
			} else {
				if(fieldname!=NULL) Rfree(fieldname);
				if(filename!=NULL) Rfree(filename);
				return(1);
			}
		} else {
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
	} else {
		return(1);
	}
	return(0);
}
short SCRNExecuteSubData(short (*func)(...),char **tmp,RDArsrc *r,char *modulename)
{
	short retval=FALSE;
	
/*lint -e746*/
	if(r!=NULL)
	{
		retval=func(tmp,r,modulename);
	} else {
		retval=func(tmp,modulename);
	}
/*lint +e746*/
	if(retval<1)
	{
		if(retval==(-1))
		{
			prterr("Error Fatal Error encountered substitution aborted.....");
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen || diageval)
		{
			prterr("DIAG SCRNExecuteSubData Returning [%s].",*tmp);
		}
#endif /* USE_RDA_DIAGNOSTICS */
	}
	return(retval);
}
void SCRNvirtualSubData(char **tmp,RDArsrc *r)
{
	char *modulename=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diageval)
	{
		prterr("DIAG SCRNvirtualSubData Substituting for [%s].",*tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
		if(SCRNExecuteSubData(SCRNvirtualFIELDSubData,tmp,r,modulename)==0)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagvirtual|| diageval)
			{
				prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(SCRNExecuteSubData(SCRNvirtualVIRTUALSubData,tmp,r,modulename)==0)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagvirtual|| diageval)
			{
				prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(ScrolledListSubData(tmp,r)==0)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagvirtual|| diageval)
			{
				prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(GLOBALSubData(tmp,modulename)==0)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagvirtual|| diageval)
			{
				prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			prterr("Error SCRNvirtualSubData [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
#ifdef USE_RDA_DIAGNOSTICS
			if(diagvirtual || diageval)
			{
					prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
	} else {
		prterr("Error SCRNvirtualSubData [%s] not found.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=stralloc("\"\"");
#ifdef USE_RDA_DIAGNOSTICS
		if(diagvirtual || diageval)
		{
			prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(modulename!=NULL) Rfree(modulename);
	}
}
void xupdateSCRNvirtuals(RDArsrc *r,void (*func)(...),void *args,MaintainMaster *MTNMASTER,PowerAdd *POWERADD,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG xupdateSCRNvirtuals Computing and Updateing Screen Virtuals for Screen [%s] [%s] at line [%d] program [%s].",(r->module==NULL?"":r->module),(r->screen==NULL?"":r->screen),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(POWERADD==NULL)
	{	
		if(func!=NULL)
		{
			MSTADVcomputeandupdateallSCRNvirtuals(r,func,args,TRUE,MTNMASTER);
		} else {
			MSTcomputeandupdateallSCRNvirtuals(r,TRUE,MTNMASTER);
		}
	} else {
		if(func!=NULL)
		{
			POWERADDADVcomputeandupdateallSCRNvirtuals(r,func,args,TRUE,POWERADD);
		} else {
			POWERADDcomputeandupdateallSCRNvirtuals(r,TRUE,SCRNvirtualSubData,r,POWERADD);
		}
	}
}
void xIncVirRsrcstoFile(RDArsrc *r,int line,char *file)
{
	DFincvir *i=NULL;
	int x;

	if(r!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG xIncVirRsrcstoFile Setting incvir file resources to file records for Screen [%s] [%s] line [%d] program [%s]",
			(r->module!=NULL?r->module:""),
			(r->screen!=NULL?r->screen:""),line,file);
	} 
#endif /* USE_RDA_DIAGNOSTICS */
	if(r->incvir!=NULL)
	{
		for(x=0,i=r->incvir;x<r->numvir;++x,++i)
		{
			rsrc2filerecord(i->fileno,r);
		}
	}
	} else {
		prterr("Error RDArsrc is NULL in IncVirRsrcstoFile at line [%d] program [%s].",line,file);
	}
}
void POWERADDComputeSCRNcallback(RDArsrc *r,PowerAdd *POWERADD)
{
	readallwidgets(r);
	IncVirRsrcstoFile(r);
	POWERADDupdateSCRNvirtuals(r,POWERADD);
}
void ComputeSCRNcallback(RDArsrc *r,MaintainMaster *MTNMASTER)
{
	readallwidgets(r);
	IncVirRsrcstoFile(r);
	MSTupdateSCRNvirtuals(r,MTNMASTER);
}
void DefaultComputeSCRNcallback(RDArsrc *r)
{
	ComputeSCRNcallback(r,NULL);
}
void xSetListValue(short fileno,RDArsrc *r,RDAScrolledList *list,short updatevirtuals,void (*SubFunc)(...),void *args,int line,char *file)
{
	int x,y,selected=0;
	char *string=NULL;
	NRDfield *field=NULL;
	double holdit=0.0;
	RDArmem *mem=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglist)
	{
		prterr("DIAG xSetListValue for Screen [%s] [%s] List [%s] Updatevirtuals [%s] line [%d] program [%s]",
			(r->module==NULL?"":r->module),
			(r->screen==NULL?"":r->screen),
			(list->name==NULL?"":list->name),
			(updatevirtuals==TRUE?"TRUE":"FALSE"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(list->contype==3)
	{
/* RADDOG */
		x=FINDRSC(r,list->conmod);
		if(x>(-1))
		{
			if(!isEMPTY(list->reformat_formula))
			{
				string=PP_EVALUATEstr(list->reformat_formula,list->SubFunc,list->args);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaglist)
				{
					prterr("DIAG xSetListValue PP_EVALUATEstr on reformat formula returned=[%s]",string);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				y=0;
				if(list->dlist!=NULL)
				{
					for(y=0;y<list->dlist->numlibs;++y)
					{
						if(!RDAstrcmp(list->dlist->libs[y],string)) 
						break;
					} 
					if(y>=list->dlist->numlibs)
					{
						y=0;
					}
				}
				FINDRSCSETINT(r,list->name,y);
				if(string!=NULL) Rfree(string);
/* UPDATERSRC */
/*
			} else if(!list->contype)
			{
				fileno=SCRNFILENUMBER(r,list->conmod,
					list->confil,1);
				if(fileno!=(-1))
				{
					field=FLDNRD(fileno,list->confld);
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
							y=0;
							if(list->dlist!=NULL)
							{
								for(y=0;y<list->dlist->numlibs;++y)
								{
									if(!RDAstrcmp(list->dlist->libs[y],field->data.string_value)) break;
								} 
								if(y>=list->dlist->numlibs)
								{
									y=0;
								}
							}
							FINDRSCSETINT(r,list->name,y);
							break;
						case CHARACTERS:
							selected=*field->data.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case BOOLNS:
							selected=*field->data.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							holdit=*field->data.float_value;
							while(holdit>(INT_MAX)) 
								holdit-=(INT_MAX);
							selected=(int)holdit;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SHORTV: 
						case SSHORTV:
							selected=*field->data.short_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							selected=*field->data.integer_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						default:
							prterr("Error cannot recognize field type [%d] for field [%s].",field->type,field->name);
							break;
					}
				}
*/
			} else {
				x=FINDRSC(r,list->conmod);
				if(x>(-1))
				{
					mem=r->rscs+x;
					switch(mem->field_type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
							y=0;
							if(list->dlist!=NULL)
							{
								for(y=0;y<list->dlist->numlibs;++y)
								{
									if(!RDAstrcmp(list->dlist->libs[y],mem->value.string_value)) break;
								} 
								if(y>=list->dlist->numlibs)
								{
									y=0;
								}
							}
							FINDRSCSETINT(r,list->name,y);
							break;
						case CHARACTERS:
							selected=*mem->value.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case BOOLNS:
							selected=*mem->value.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							holdit=*mem->value.float_value;
							while(holdit>(INT_MAX)) 
								holdit-=(INT_MAX);
							selected=(int)holdit;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SHORTV: 
						case SSHORTV:
							selected=*mem->value.short_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							selected=*mem->value.integer_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						default:
							prterr("Error cannot recognize field type [%d] for RDArmem [%s].",mem->field_type,mem->rscrname);
							break;
					}
				}
			}
		}
		if(updatevirtuals) updatersrc(r,list->name);
/* RADDOG */
	} else {
	switch(list->type)
	{
		default:
		case 0: /* list connecting FROM a file's field */
			if(!isEMPTY(list->format_formula))
			{
				string=PP_EVALUATEstr(list->format_formula,SubFunc,args);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaglist)
				{
					prterr("DIAG xSetListValue PP_EVALUATEstr returned=[%s]",string);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				for(y=0;y<list->dlist->numlibs;++y)
				{
					if(!RDAstrcmp(list->dlist->libs[y],string)) break;
				} 
				if(y>=list->dlist->numlibs)
				{
					y=0;
				}
				FINDRSCSETINT(r,list->name,y);
				if(string!=NULL) Rfree(string);
/* UPDATERSRC */
				if(updatevirtuals) updatersrc(r,list->name);
			} else {
				prterr("Error NULL format formula in [%s].",list->name);
			}
			break;
		case 3: /* Computed List */
		case 1: /* list connecting to a parsed list */
		case 2: /*  Special Lists */
			if(!isEMPTY(list->format_formula))
			{
				string=PP_EVALUATEstr(list->format_formula,SubFunc,args);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaglist)
				{
					prterr("DIAG xSetListValue PP_EVALUATEstr on format formula returned=[%s]",string);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				y=0;
				if(list->dlist!=NULL)
				{
					for(y=0;y<list->dlist->numlibs;++y)
					{
						if(!RDAstrcmp(list->dlist->libs[y],string)) 
						break;
					} 
					if(y>=list->dlist->numlibs)
					{
						y=0;
					}
				}
				FINDRSCSETINT(r,list->name,y);
				if(string!=NULL) Rfree(string);
/* UPDATERSRC */
			} else if(!list->contype)
			{
				fileno=SCRNFILENUMBER(r,list->conmod,
					list->confil,1);
				if(fileno!=(-1))
				{
					field=FLDNRD(fileno,list->confld);
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
							y=0;
							if(list->dlist!=NULL)
							{
								for(y=0;y<list->dlist->numlibs;++y)
								{
									if(!RDAstrcmp(list->dlist->libs[y],field->data.string_value)) break;
								} 
								if(y>=list->dlist->numlibs)
								{
									y=0;
								}
							}
							FINDRSCSETINT(r,list->name,y);
							break;
						case CHARACTERS:
							selected=*field->data.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case BOOLNS:
							selected=*field->data.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							holdit=*field->data.float_value;
							while(holdit>(INT_MAX)) 
								holdit-=(INT_MAX);
							selected=(int)holdit;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SHORTV: 
						case SSHORTV:
							selected=*field->data.short_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							selected=*field->data.integer_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						default:
							prterr("Error cannot recognize field type [%d] for field [%s].",field->type,field->name);
							break;
					}
				}
			} else if(list->contype>0)
			{
				x=FINDRSC(r,list->conmod);
				if(x>(-1))
				{
					mem=r->rscs+x;
					switch(mem->field_type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
							y=0;
							if(list->dlist!=NULL)
							{
								for(y=0;y<list->dlist->numlibs;++y)
								{
									if(!RDAstrcmp(list->dlist->libs[y],mem->value.string_value)) break;
								} 
								if(y>=list->dlist->numlibs)
								{
									y=0;
								}
							}
							FINDRSCSETINT(r,list->name,y);
							break;
						case CHARACTERS:
							selected=*mem->value.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case BOOLNS:
							selected=*mem->value.string_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							holdit=*mem->value.float_value;
							while(holdit>(INT_MAX)) 
								holdit-=(INT_MAX);
							selected=(int)holdit;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SHORTV: 
						case SSHORTV:
							selected=*mem->value.short_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							selected=*mem->value.integer_value;
							if(selected>=list->dlist->numlibs)
								selected=0;
							FINDRSCSETINT(r,list->name,selected);
							break;
						default:
							prterr("Error cannot recognize field type [%d] for RDArmem [%s].",mem->field_type,mem->rscrname);
							break;
					}
				}
			}
			if(updatevirtuals) updatersrc(r,list->name);
			break;
	}
	}
}
void xcomputeandupdateallSCRNvirtuals(RDArsrc *r,void (*SubFunc)(...),void *args,
	short updatevirtuals,short do_sensitive,MaintainMaster *MTNMASTER,
	PowerAdd *POWERADD,int line,char *file)
{
	RDArmem *member;
	RDAvirtual *v;
	int x,y,selected=0;
	RDAScrolledList *list=NULL;
	short fileno=(-1),edit_rsrc=TRUE;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		if(!updatevirtuals)
		{
			prterr("DIAG computeandupdateallSCRNvirtuals Computing-Only Screen Virtuals for Screen [%s] [%s] at line [%d] program [%s].",(r->module==NULL?"":r->module),(r->screen==NULL?"":r->screen),line,file);
		} else {
			prterr("DIAG computeandupdateallSCRNvirtuals Computing and Updating Screen Virtuals for Screen [%s] [%s] at line [%d] program [%s].",(r->module==NULL?"":r->module),(r->screen==NULL?"":r->screen),line,file);
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->SetFunc!=NULL)
		{
/*lint -e746*/
			r->SetFunc(r->SetFuncArgs);
/*lint +e746*/
		}
		CLEAR_SCREEN_VIRTUAL(r);
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
/* Re-create list when necessary */
				edit_rsrc=TRUE;
				if(list->timing)
				{
					if(list->timing<2)
					{
						FINDRSCGETINT(r,list->name,&selected);
					} else {
						selected=0;
					}
					CreateRDAScrolledList(r,list,SubFunc,args);
					if(list->timing==2)
					{
						y=FINDRSC(r,list->name);
						if(y==(-1))
						{
							if(!list->contype)
							{
								fileno=SCRNFILENUMBER(r,list->conmod,list->confil,1);
								if(FIELDscrn(fileno,list->confld,&edit_rsrc))
								{
									addlstrsrc(r,list->name,&selected,
										(edit_rsrc ? TRUE:FALSE),
										SetRDAScrolledList,list->dlist->numlibs,
										&list->dlist->libs,list);
								}
							} else {
								addlstrsrc(r,list->name,&selected,
									(edit_rsrc ? TRUE:FALSE),
									SetRDAScrolledList,list->dlist->numlibs,
									&list->dlist->libs,list);
							}
						} else {
							member=r->rscs+y;
							FINDRSCLISTAPPlib(r,list->name,selected,list->dlist);
							updatemember(member);
						}
					} else {
						FINDRSCLISTAPPlib(r,list->name,selected,list->dlist);
/* timing with a missing updatersrc */
						updatersrc(r,list->name);
					}
				}
/* Set Supporting Files from Maintain Master Definition */
/* consider moving this down in the master's as well */
/* Set List Values */
				SetListValue(fileno,r,list,updatevirtuals,SubFunc,args);
			}
		}
		if(MTNMASTER!=NULL)
		{
			SetSupportingFiles(r,MTNMASTER,SubFunc,args,(updatevirtuals ? TRUE:FALSE),FALSE);
		}
		if(POWERADD!=NULL)
		{
			SetPowerAddSupportingFiles(r,POWERADD,SubFunc,args,(updatevirtuals ? TRUE:FALSE),FALSE);
		}
		if(r->virflds!=NULL)
		{
			for(x=0,v=r->virflds;x<r->num;++x,++v)
			{
				y=FINDRSC(r,v->name);
				if(y!=(-1))
				{
					member=r->rscs+y;
					if((IsWidgetDisplayed(member) 
						&& IsScreenDisplayed(r))
						|| !IsScreenDisplayed(r))
					{
						computevirtual(v,SubFunc,args);
						switch(member->field_type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case SOCSECNUM:
							case TIMES:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(v->value.string_value!=NULL)
								{
									QUICKALLOC(member->value.string_value,member->dlen,v->dlen);
									memcpy(member->value.string_value,v->value.string_value,v->dlen);
									if(member->dlen>member->field_length && member->field_length>0)
									{
										member->value.string_value[member->field_length]=0;
									}
								} else if(member->dlen>0)
								{
									memset(member->value.string_value,0,member->dlen);
								} else {
									member->value.string_value=NULL;
									member->dlen=0;
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								*member->value.string_value=*v->value.string_value;
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case SDECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
								*member->value.float_value=*v->value.float_value;
								break;
							case SHORTV:
							case SSHORTV:
								*member->value.short_value=*v->value.short_value;
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								*member->value.integer_value=
									*v->value.integer_value;
								break;
							default:
								prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name);
								break;
						}
						if(updatevirtuals) updatemember(member);
					}
				}
			}
		}
	} else {
		prterr("Error NULL resource structure passed to computeandupdateallSCRNvirtuals.");
	}
	if(do_sensitive)
	{
		SetAllEditableAndSensitive(r);
	}
}
void xSetVirFieldFuncs(RDArsrc *rsrc,void (*func)(...),void *args,short function_type,MaintainMaster *MTNMASTER,PowerAdd *POWERADD,int line,char *file)
{
	int x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG SetVirFieldFuncs Assigning member->function Callbacks for updating Screen Virtual Fields for Screen [%s] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL)
	{
		for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
		{
			if(member->function==NULL)
			{
				if(func!=NULL)
				{
					member->function=func;
					member->arglist=args;
					member->function_type=function_type;
				} else {
					if(MTNMASTER!=NULL)
					{
						member->function=(void (*)(...))ComputeSCRNcallback;
					/* Originally passed whole member value but
					   Realloc from xaddrsrc() is called after
					   SetVirFieldFuncs() from SCRNvitual2rsrc() 
					   so field addresses being reset*/
						member->arglist=MTNMASTER;
					} else if(POWERADD!=NULL)
					{
						member->function=(void (*)(...))POWERADDComputeSCRNcallback;
					/* Originally passed whole member value but
					   Realloc from xaddrsrc() is called after
					   SetVirFieldFuncs() from SCRNvitual2rsrc() 
					   so field addresses being reset*/
						member->arglist=POWERADD;
					} else {
						member->function=(void (*)(...))DefaultComputeSCRNcallback;
						member->arglist=MTNMASTER;
					}
					member->function_type=0;
				}
			}
		}
	} else {
	}
}
char *GUIEvalStr(char *string,void (*SubFunc)(...),void *args)
{
	char *tmp=NULL;

	if(!isEMPTY(string) && SubFunc!=NULL)
	{
		tmp=PP_EVALUATEstr(string,SubFunc,args);
	}
	return(tmp);
}
short GUIEVAL(char *string,RDArsrc *r)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diageval || diageval_field)
	{
		prterr("DIAG GUIEVAL Expression Evaluator used for evaluating expressions on Custom Screen [%s] [%s].",r->module,r->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(string))
	{
		return(EVALUATEbol(string,SCRNvirtualSubData,r));
	} else return(TRUE);
}
short xSCRNvirtual2rangersrc(RDArsrc *r,void (*SubFunc)(...),void *SubFuncArgs,int line,char *file)
{
	int x,count=0;
	RDAvirtual *v;
	int y=(-1),start=0;
	char *temp=NULL,*libx=NULL;
	APPlib *tmplist=NULL;
	RDAScrolledList *list=NULL;
	RDALoadValue *load=NULL;
	short edit_rsrc=TRUE,filenum=(-1),s=FALSE;
        int z,p=(-1);
	RDArmem *mem1=NULL,*mem=NULL;
        RDAacct *acct=NULL;
	short tempsht=0;
	RDApdev *pdev=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG SCRNvirtual2rangersrc Creating Range Resources for virtual fields on Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->virflds!=NULL)
		{
			for(x=0,v=r->virflds;x<r->num;++x,++v)
			{
				virtual2rangersrc(v,r);
				++count;
			}
		}
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
				if(list->timing<2)
				{
					CreateRDAScrolledList(r,list,SubFunc,SubFuncArgs);
				} else {
					if(list->dlist==NULL)
					{
						list->dlist=APPlibNEW();
					} 
				}
				if(SubFunc!=NULL)
				{
					list->SubFunc=SubFunc;
					list->args=SubFuncArgs;
				} else {
					list->SubFunc=SCRNvirtualSubData;
					list->args=r;
				}
				y=FINDRSC(r,list->name);
				switch(list->type)
				{
					default:
					case 0: /* list connecting FROM a file's field */
						if(!list->contype) /* connecting TO a file field */
						{
							filenum=SCRNFILENUMBER(r,list->conmod,list->confil,1);
							if(FIELDscrn(filenum,list->confld,&edit_rsrc))
							{
								if(y==(-1))
								{
									temp=Rmalloc(RDAstrlen(list->confil)+RDAstrlen(list->confld)+7);
									sprintf(temp,"[%s][%s]",(list->confil!=NULL?list->confil:""),(list->confld!=NULL?list->confld:""));
									p=FINDRSC(r,temp);
									if(p!=(-1))
									{
										if(edit_rsrc==TRUE)
										{
											mem1=r->rscs+p;
											if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
											{
												addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											} else {
												addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											}
										} else {
											addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										}
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);
									}
									if(temp!=NULL) Rfree(temp);
								} else {
									FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
									updatersrc(r,list->name);
								}
							}
						} else {
							if(y==(-1))
							{
								temp=Rmalloc(RDAstrlen(list->conmod)+3);
								sprintf(temp,"%s",(list->conmod!=NULL?list->conmod:""));
								p=FINDRSC(r,temp);
								if(p!=(-1))
								{
									mem1=r->rscs+p;
									if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
									{
										addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									}
								} else {
									addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);
								}
								if(temp!=NULL) Rfree(temp);
							} else {
								FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
								updatersrc(r,list->name);
							}
						}
						break;
					case 2: /* connected FROM Special */
						if(y==(-1))
						{
							addlstrsrc(r,list->name,&start,(edit_rsrc ? TRUE:FALSE),
								SetRDAScrolledList,list->dlist->numlibs,
								&list->dlist->libs,list);
						} else {
							if(list->dlist!=NULL)
							{
								freeapplib(list->dlist);
							}
							list->dlist=APPlibNEW();
							if(!RDAstrcmp(list->special_function,"DEFINITION TYPES"))
							{
								tempsht=PP_EVALUATEsht(list->select_formula,list->SubFunc,list->args);
								switch(tempsht)
								{
									case EXP_ACCT : 
										if(EXPENDITURE_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=EXPENDITURE_ACCOUNT->codetype;
												z<EXPENDITURE_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										} 
										break;
									case REV_ACCT : 
										if(REVENUE_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=REVENUE_ACCOUNT->codetype;
												z<REVENUE_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									case BAL_ACCT : 
										if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
												z<BALANCE_SHEET_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									case BEG_ACCT : 
										if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
										{
											for(z=0,acct=BEGINNING_BALANCE_ACCOUNT->codetype;
												z<BEGINNING_BALANCE_ACCOUNT->num;++z,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									case CUSTOM_ACCT:
										if(CUSTOM_INPUTS_ACCOUNT->codetype!=NULL)
										{
											for(x=0,acct=CUSTOM_INPUTS_ACCOUNT->codetype;
												x<CUSTOM_INPUTS_ACCOUNT->num;++x,++acct)
											{
												addAPPlib(list->dlist,acct->name);
											}
										}
										break;
									default : 
                        							prterr("WARNING: Invalid Account Type [%d], can't make list.",tempsht);
										break;
								}
							} else if(!RDAstrcmp(list->special_function,"OUTPUT DEVICES"))
							{
								libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
								sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
								sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
								tmplist=getlibnames(libx,FALSE);
								if(tmplist!=NULL)
								{
									for(x=0;x<tmplist->numlibs;++x)
									{
										pdev=RDApdevNEW(tmplist->libs[x]);
										if(getpdevbin(pdev))
										{
										} else if(pdev->active==TRUE)
										{
											addAPPlib(list->dlist,tmplist->libs[x]);
										}
										if(pdev!=NULL) free_pdev(pdev);
									}
									freeapplib(tmplist);
								}
								if(libx!=NULL) Rfree(libx);
								if(XPERT_SETUP->ARCHIVE==TRUE)
								{
									if(!RDAstrcmp(list->conmod,"SECURITY") && !RDAstrcmp(list->confil,"USERS") && !RDAstrcmp(list->confld,"DEFAULT PRINTER"))
									{
																				addAPPlib(list->dlist,"AUTOARCHIVE");
									}
								}
							} else if(!isEMPTY(list->special_function) && RDAstrcmp(list->special_function,"No Special Function"))
							{
								if(!RDAstrcmp(list->special_function,"MODULES AVAILABLE"))
								{
									tmplist=FindModulesAvailable();
								} else if(!RDAstrcmp(list->special_function,"FILES AVAILABLE"))
								{
									tmplist=FindFilesAvailable(list->select_formula,list->SubFunc,list->args);
								} else if(!RDAstrcmp(list->special_function,"FIELDS AVAILABLE"))
								{
									tmplist=GetFieldsAvailable(list->select_formula,list->SubFunc,list->args);
								} else if(!RDAstrcmp(list->special_function,"PROCESSES AVAILABLE"))
								{
									tmplist=GetProcessesAvailable(list->select_formula,list->SubFunc,list->args);
								} else if(!RDAstrcmp(list->special_function,"MATRIXES AVAILABLE"))
								{
									tmplist=FindMatrixAvailable(list->select_formula,list->SubFunc,list->args);
								} else if(!RDAstrcmp(list->special_function,"TABLES AVAILABLE"))
								{
									tmplist=FindTablesAvailable(list->select_formula,list->SubFunc,list->args);
								} else if(!RDAstrcmp(list->special_function,"DIRECTORY CONTENTS"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,list->SubFunc,list->args,0);
								} else if(!RDAstrcmp(list->special_function,"DIRECTORY RECURSIVELY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,list->SubFunc,list->args,3);
								} else if(!RDAstrcmp(list->special_function,"DIRECTORIES ONLY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,list->SubFunc,list->args,4);
								} else if(!RDAstrcmp(list->special_function,"NO DIRECTORIES"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,list->SubFunc,list->args,5);
								} else if(!RDAstrcmp(list->special_function,"PDF(S) IN DIRECTORY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,list->SubFunc,list->args,1);
								} else if(!RDAstrcmp(list->special_function,"TXT(S) IN DIRECTORY"))
								{
									tmplist=DirectoryContentsAvailable(list->select_formula,list->SubFunc,list->args,2);
								} else if(!RDAstrcmp(list->special_function,"WORKSTATION DIRECTORY CONTENTS (SFTP)"))
								{
									s=FINDRSC(r,list->name);
									if(s>(-1))
									{
										mem=r->rscs+s;
										if((isEMPTY(mem->editable_expression) && isEMPTY(mem->sensitive_expression)) || (mem->w!=NULL && mem->editable==TRUE && mem->sensitive==TRUE))
										{
											tmplist=RemoteDirectoryContentsAvailable(list->select_formula,list->SubFunc,list->args,0);
										} else tmplist=APPlibNEW();
									} else tmplist=APPlibNEW();
								}
								if(tmplist!=NULL)
								{
									for(x=0;x<tmplist->numlibs;++x)
									{
										addAPPlib(list->dlist,tmplist->libs[x]);
									}
									freeapplib(tmplist);
								}
							}
							if(list->dlist==NULL) 
							{
								list->dlist=APPlibNEW();
								prterr("WARNING: Special Type RDAScrolledList [%s] without any defined elements.",list->name);
								temp=Rmalloc(RDAstrlen(list->name)+19);
								sprintf(temp,"No %s List Available",list->name);
								addAPPlib(list->dlist,temp);
								if(temp!=NULL) Rfree(temp);
							} else if(list->dlist->numlibs<1)
							{
								temp=Rmalloc(RDAstrlen(list->name)+19);
								sprintf(temp,"No %s List Available",list->name);
								addAPPlib(list->dlist,temp);
								if(temp!=NULL) Rfree(temp);
							}
							FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
							updatersrc(r,list->name);
						}
						break;
					case 1: /* connect FROM Parsed List */
					case 3: /* unused */
						if(!list->contype) /* connecting TO a file field */
						{
							filenum=SCRNFILENUMBER(r,list->conmod,list->confil,1);
							if(FIELDscrn(filenum,list->confld,&edit_rsrc))
							{
								if(y==(-1))
								{
									temp=Rmalloc(RDAstrlen(list->confil)+RDAstrlen(list->confld)+7);
									sprintf(temp,"[%s][%s]",(list->confil!=NULL?list->confil:""),(list->confld!=NULL?list->confld:""));
									p=FINDRSC(r,temp);
									if(p!=(-1))
									{
										if(edit_rsrc==TRUE)
										{
											mem1=r->rscs+p;
											if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
											{
												addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											} else {
												addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
											}
										} else {
											addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										}
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
										prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);
									}
								} else {
									FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
									updatersrc(r,list->name);
								}
								if(temp!=NULL) Rfree(temp);
							} /* security failed */
						} else {
							if(y==(-1))
							{
								temp=Rmalloc(RDAstrlen(list->conmod)+3);
								sprintf(temp,"%s",(list->conmod!=NULL?list->conmod:""));
								p=FINDRSC(r,temp);
								if(p!=(-1))
								{
									mem1=r->rscs+p;
									if(mem1->editable==TRUE && mem1->user_editable==TRUE && mem1->user_sensitive==TRUE)
									{
										addlstrsrc(r,list->name,&start,TRUE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									} else {
										addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
									}
								} else {
									addlstrsrc(r,list->name,&start,FALSE,SetRDAScrolledList,list->dlist->numlibs,&list->dlist->libs,list);
#ifdef XXXX
									prterr("ERROR: Can't find Connect Resource [%s] for RDA Scrolled List [%s]",temp,list->name);
#endif /* XXXX */

								}
								if(temp!=NULL) Rfree(temp);
							} else {
								FINDRSCLISTAPPlib(r,list->name,start,list->dlist);
								updatersrc(r,list->name);
							}
						}
#ifdef RADDOG
						DefaultComputeSCRNcallback(r);
#endif
						break;
				}
			}
		}
/* SJS HERE */
		if(r->loads!=NULL)
		{
			for(x=0,load=r->loads;x<r->numloads;++x,++load)
			{
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+12);
				sprintf(temp,"%s LOAD VALUE",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadValueCB,load);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+15);
				sprintf(temp,"%s LOAD OPERATOR",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadOperatorCB,load);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+14);
				sprintf(temp,"%s LOAD GROUPER",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadGrouperCB,load);
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+14);
				sprintf(temp,"%s LOAD CONTROL",load->name);
				addrfcbrsrc(r,temp,TRUE,RDALoadControlCB,load);
				if(temp!=NULL) Rfree(temp);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG SCRNvirtual2rangersrc Screen [%s] [%s] is able to range on [%d] virtual fields at line [%d] program [%s].",r->module,r->screen,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(count);
}
void xReadRDAScrolledLists(RDArsrc *r,int line,char *file)
{
	int selected=0,y=0,lf=0,x=0;
	char *temp=NULL;
	RDArmem *mem=NULL;
	char *value=NULL;
	RDAScrolledList *list=NULL;
	short fileno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglist)
	{
		prterr("DIAG ReadRDAScrolledLists Setting Scrolled List [%s] [%s]'s at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
				FINDRSCGETINT(r,list->name,&selected);
				switch(list->contype)
				{
					default:
					case 0: /* Connect Normal Field */
					case 1: /* Connect Special Field */
					case 3: /* Connect Report Generator */
						if(!list->contype)
						{
							temp=Rmalloc(RDAstrlen(list->confil)+RDAstrlen(list->confld)+5);
							sprintf(temp,"[%s][%s]",
								list->confil,list->confld);
						} else {
							temp=stralloc(list->conmod);
						}
						y=FINDRSC(r,temp);
						if(temp!=NULL) Rfree(temp);
						if(y>(-1))
						{
							mem=r->rscs+y;
							value=list->dlist->libs[selected];
							fileno=SCRNFILENUMBER(r,list->conmod,
								list->confil,1);
							switch(mem->field_type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case SOCSECNUM:
								case TIMES:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case ZIPCODE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
									if(!isEMPTY(list->unformat_formula))
									{
										value=PP_EVALUATEstr(list->unformat_formula,list->SubFunc,list->args);
									}
									if(value!=NULL) lf=RDAstrlen(value);
										else lf=0;
									if(value!=NULL && lf>0)
									{
										QUICKALLOC(mem->value.string_value,mem->dlen,lf+1);
										memcpy(mem->value.string_value,value,lf+1);
									} else if(mem->dlen>0)
									{
										memset(mem->value.string_value,0,mem->dlen);
									} else {
										mem->value.string_value=NULL;
										mem->dlen=0;
									}
									if(mem->dlen>mem->field_length && mem->field_length>0) mem->value.string_value[mem->field_length]=0;
									if(!isEMPTY(list->unformat_formula))
									{
										if(value!=NULL) Rfree(value);
									}
									if(fileno!=(-1))
									{
										if(!list->contype)
										{
											FINDFLDSETSTRING(fileno,list->confld,mem->value.string_value);
										}
									}
									break;
								case BOOLNS:
								case CHARACTERS:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.string_value=PP_EVALUATEbol(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.string_value=(char)selected;
									if(fileno!=(-1))
									{
										if(!list->contype)
										{
											FINDFLDSETCHAR(fileno,list->confld,*mem->value.string_value);
										}
									}
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case SDECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.float_value=PP_EVALUATEdbl(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.float_value=(double)selected;
									if(fileno!=(-1))
									{
										if(!list->contype)
										{
											FINDFLDSETDOUBLE(fileno,list->confld,*mem->value.float_value);
										}
									}
									break;
								case SHORTV:
								case SSHORTV:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.short_value=PP_EVALUATEsht(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.short_value=(short)selected;
									if(fileno!=(-1))
									{
										if(!list->contype)
										{
											FINDFLDSETSHORT(fileno,list->confld,*mem->value.short_value);
										}
									}
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.integer_value=PP_EVALUATEint(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.integer_value=selected;
									if(fileno!=(-1))
									{	
										if(!list->contype)
										{
											FINDFLDSETINT(fileno,list->confld,*mem->value.integer_value);
										}
									}
									break;
								default:
									prterr("Error Field Type [%d] is invalid for Resource [%s].",mem->rscrname);
									break;
							}
#ifdef IF_WE_NEED_TOO
							ExecuteRDArmemFunction(mem);
#endif /* IF_WE_NEED_TO */
						}
						break;
					case 2: /* Connect Virtual Field */
						y=FINDRSC(r,list->conmod);
						if(y>(-1))
						{
							mem=r->rscs+y;
							value=list->dlist->libs[selected];
							switch(mem->field_type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case SOCSECNUM:
								case TIMES:
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case ZIPCODE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
									if(!isEMPTY(list->unformat_formula))
									{
										value=PP_EVALUATEstr(list->unformat_formula,list->SubFunc,list->args);
									}
									if(value!=NULL) lf=RDAstrlen(value);
										else lf=0;
									if(value!=NULL && lf>0)
									{
										QUICKALLOC(mem->value.string_value,mem->dlen,lf+1);
										memcpy(mem->value.string_value,value,lf+1);
									} else if(mem->dlen>0)
									{
										memset(mem->value.string_value,0,mem->dlen);
									} else {
										mem->value.string_value=NULL;
										mem->dlen=0;
									}
									if(mem->dlen>mem->field_length && mem->field_length>0) mem->value.string_value[mem->field_length]=0;
									if(!isEMPTY(list->unformat_formula))
									{
										if(value!=NULL) Rfree(value);
									}
									break;
								case BOOLNS:
								case CHARACTERS:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.string_value=PP_EVALUATEbol(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.string_value=(char)selected;
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case SDECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.float_value=PP_EVALUATEdbl(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.float_value=(double)selected;
									break;
								case SHORTV:
								case SSHORTV:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.short_value=PP_EVALUATEsht(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.short_value=(short)selected;
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									if(!isEMPTY(list->unformat_formula))
									{
										*mem->value.integer_value=PP_EVALUATEint(list->unformat_formula,list->SubFunc,list->args);
									} else *mem->value.integer_value=selected;
									break;
								default:
									prterr("Error Field Type [%d] is invalid for Resource [%s].",mem->rscrname);
									break;
							}
#ifdef IF_WE_NEED_TOO
							ExecuteRDArmemFunction(mem);
#endif /* IF_WE_NEED_TOO */
						}
						break;
				}
			}
		}
	}
}
void xReadRDAScrolledListsKeysOnly(RDArsrc *r,short filenum,
	int line,char *file)
{
	int selected=0,y=0,lf=0,x=0;
	char *temp=NULL,*mod=NULL,*fild=NULL;
	RDArmem *mem=NULL;
	char *value=NULL;
	RDAScrolledList *list=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	short fileno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglist)
	{
		prterr("DIAG ReadRDAScrolledListsKeysOnly Setting Scrolled List [%s] [%s]'s at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		mod=MODULENAME(filenum);
		fild=FILENAME(filenum);
		key=KEYNUM(filenum,1);
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
				if(!list->contype && !RDAstrcmp(list->conmod,mod) &&
					!RDAstrcmp(list->confil,fild))
				{
					for(y=0,part=key->part;y<key->numparts;++y,++part)
					{
						if(!RDAstrcmp(part->name,list->confld)) break;
					}
					if(y<key->numparts)
					{
						FINDRSCGETINT(r,list->name,&selected);
						switch(list->contype)
						{
							default:
							case 0: /* Connect Normal Field */
							case 1: /* Connect Special Field */
								if(!list->contype)
								{
									temp=Rmalloc(RDAstrlen(list->confil)+
										RDAstrlen(list->confld)+5);
									sprintf(temp,"[%s][%s]",
										list->confil,list->confld);
								} else {
									temp=stralloc(list->conmod);
								}
								y=FINDRSC(r,temp);
								if(temp!=NULL) Rfree(temp);
								if(y>(-1))
								{
									mem=r->rscs+y;
									value=list->dlist->libs[selected];
									fileno=SCRNFILENUMBER(r,list->conmod,
										list->confil,1);
									switch(mem->field_type)
									{
										case SCROLLEDTEXT:
										case VARIABLETEXT:
										case DATES:
										case SOCSECNUM:
										case TIMES:
										case PLAINTEXT:
										case PHONE:
										case CUSTOMTYPE:
										case ZIPCODE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:
											if(!isEMPTY(list->unformat_formula))
											{
												value=PP_EVALUATEstr(list->unformat_formula,list->SubFunc,list->args);
											} 
											if(value!=NULL) lf=RDAstrlen(value);
												else lf=0;
											if(value!=NULL && lf>0)
											{
												QUICKALLOC(mem->value.string_value,mem->dlen,lf+1);
												memcpy(mem->value.string_value,value,lf+1);
											} else if(mem->dlen>0)
											{
												memset(mem->value.string_value,0,mem->dlen);
											} else {
												mem->value.string_value=NULL;
												mem->dlen=0;
											}
											if(mem->dlen>mem->field_length && mem->field_length>0) mem->value.string_value[mem->field_length]=0;
											if(!isEMPTY(list->unformat_formula))
											{
												if(value!=NULL) Rfree(value);
											}
											if(fileno!=(-1))
											{
												if(!list->contype)
												{
													FINDFLDSETSTRING(fileno,list->confld,mem->value.string_value);
												} 
											}
											break;
										case BOOLNS:
										case CHARACTERS:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.string_value=PP_EVALUATEbol(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.string_value=(char)selected;
											if(fileno!=(-1))
											{
												if(!list->contype)
												{
													FINDFLDSETCHAR(fileno,list->confld,*mem->value.string_value);
												}
											}
											break;
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.float_value=PP_EVALUATEdbl(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.float_value=(double)selected;
											if(fileno!=(-1))
											{
												if(!list->contype)
												{
													FINDFLDSETDOUBLE(fileno,list->confld,*mem->value.float_value);
												}
											}
											break;
										case SHORTV:
										case SSHORTV:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.short_value=PP_EVALUATEsht(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.short_value=(short)selected;
											if(fileno!=(-1))
											{
												if(!list->contype)
												{
													FINDFLDSETSHORT(fileno,list->confld,*mem->value.short_value);
												}
											}
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.integer_value=PP_EVALUATEint(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.integer_value=(int)selected;
											if(fileno!=(-1))
											{
												if(!list->contype)
												{
													FINDFLDSETINT(fileno,list->confld,*mem->value.integer_value);
												}
											}
											break;
										default:
											prterr("Error Field Type [%d] is invalid for Resource [%s].",mem->rscrname);
											break;
									}
#ifdef IF_WE_NEED_TOO
									ExecuteRDArmemFunction(mem);
#endif /* IF_WE_NEED_TO */
								}
								break;
							case 2: /* Connect Virtual Field */
								y=FINDRSC(r,list->conmod);
								if(y>(-1))
								{
									mem=r->rscs+y;
									value=list->dlist->libs[selected];
									switch(mem->field_type)
									{
										case SCROLLEDTEXT:
										case VARIABLETEXT:
										case DATES:
										case SOCSECNUM:
										case TIMES:
										case PLAINTEXT:
										case PHONE:
										case CUSTOMTYPE:
										case ZIPCODE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case OPTIONALFIELDS:
											if(!isEMPTY(list->unformat_formula))
											{
												value=PP_EVALUATEstr(list->unformat_formula,list->SubFunc,list->args);
											} 
											if(value!=NULL) lf=RDAstrlen(value);
												else lf=0;
											if(value!=NULL && lf>0)
											{
												QUICKALLOC(mem->value.string_value,mem->dlen,lf+1);
												memcpy(mem->value.string_value,value,lf+1);
											} else if(mem->dlen>0)
											{
												memset(mem->value.string_value,0,mem->dlen);
											} else {
												mem->value.string_value=NULL;
												mem->dlen=0;
											}
											if(mem->dlen>mem->field_length && mem->field_length>0) mem->value.string_value[mem->field_length]=0;
											if(!isEMPTY(list->unformat_formula))
											{
												if(value!=NULL) Rfree(value);
											}
											break;
										case BOOLNS:
										case CHARACTERS:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.string_value=PP_EVALUATEbol(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.string_value=(char)selected;
											break;
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.float_value=PP_EVALUATEdbl(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.float_value=(double)selected;
											break;
										case SHORTV:
										case SSHORTV:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.short_value=PP_EVALUATEsht(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.short_value=(short)selected;
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(!isEMPTY(list->unformat_formula))
											{
												*mem->value.integer_value=PP_EVALUATEint(list->unformat_formula,list->SubFunc,list->args);
											} else *mem->value.integer_value=(int)selected;
											break;
										default:
											prterr("Error Field Type [%d] is invalid for Resource [%s].",mem->rscrname);
											break;
									}
#ifdef IF_WE_NEED_TOO
									ExecuteRDArmemFunction(mem);
#endif /* IF_WE_NEED_TO */
								}
								break;
						}
					}
				}
			}
		}
	}
}
void xReadRDAScrolledListsNoPassKeys(RDArsrc *r,PassKey *passkey,
	int line,char *file)
{
	int selected=0,y=0,lf=0,x=0;
	char *temp=NULL,*mod=NULL,*fild=NULL;
	RDArmem *mem=NULL;
	char *value=NULL;
	RDAScrolledList *list=NULL;
	NRDfield *field;
	short fileno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglist)
	{
		prterr("DIAG ReadRDAScrolledListsNoPassKeys Setting Scrolled List [%s] [%s]'s at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		mod=MODULENAME(passkey->fileno);
		fild=FILENAME(passkey->fileno);
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
				y=(-1);
				if(passkey->fields!=NULL)
				{		
					if(!list->contype && !RDAstrcmp(list->conmod,mod) 
						&& !RDAstrcmp(list->confil,fild))
					{
						for(y=0,field=passkey->fields;y<passkey->numflds;++y,++field)
						{
							if(!RDAstrcmp(field->name,list->confld)) break;
						}
					} else if((list->contype==1) || (list->contype==3))
					{
						for(y=0,field=passkey->fields;y<passkey->numflds;++y,++field)
						{
							if(!RDAstrcmp(field->name,list->confld)) break;
						}
					}
				} 
				if(y>=passkey->numflds || y==(-1))
				{
					FINDRSCGETINT(r,list->name,&selected);
					switch(list->contype)
					{
						default:
						case 0: /* Connect Normal Field */
						case 1: /* Connect Special Field */
						case 3: /* Connect Report Generator */
							if(!list->contype)
							{
								temp=Rmalloc(RDAstrlen(list->confil)+
									RDAstrlen(list->confld)+5);
								sprintf(temp,"[%s][%s]",
									list->confil,list->confld);
							} else {
								temp=stralloc(list->conmod);
							}
							y=FINDRSC(r,temp);
							if(temp!=NULL) Rfree(temp);
							if(y>(-1))
							{
								mem=r->rscs+y;
								value=list->dlist->libs[selected];
								fileno=SCRNFILENUMBER(r,list->conmod,
									list->confil,1);
								switch(mem->field_type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case SOCSECNUM:
									case TIMES:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case ZIPCODE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
										if(!isEMPTY(list->unformat_formula))
										{
											value=PP_EVALUATEstr(list->unformat_formula,list->SubFunc,list->args);
										} 
										if(value!=NULL) lf=RDAstrlen(value);
											else lf=0;
										if(value!=NULL && lf>0)
										{
											QUICKALLOC(mem->value.string_value,mem->dlen,lf+1);
											memcpy(mem->value.string_value,value,lf+1);
										} else if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										if(mem->dlen>mem->field_length && mem->field_length>0) mem->value.string_value[mem->field_length]=0;
										if(fileno!=(-1))
										{
											if(!list->contype)
											{
												FINDFLDSETSTRING(fileno,list->confld,mem->value.string_value);
											}
										}
										if(!isEMPTY(list->unformat_formula))
										{
											if(value!=NULL) Rfree(value);
										}
										break;
									case BOOLNS:
									case CHARACTERS:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.string_value=PP_EVALUATEbol(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.string_value=selected;
										if(fileno!=(-1))
										{
											if(!list->contype)
											{
												FINDFLDSETCHAR(fileno,list->confld,*mem->value.string_value);
											}
										}
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.float_value=PP_EVALUATEdbl(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.float_value=selected;
										if(fileno!=(-1))
										{
											if(!list->contype)
											{
												FINDFLDSETDOUBLE(fileno,list->confld,*mem->value.float_value);
											}
										} 
										break;
									case SHORTV:
									case SSHORTV:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.short_value=PP_EVALUATEsht(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.short_value=selected;
										if(fileno!=(-1))
										{
											if(!list->contype)
											{
												FINDFLDSETSHORT(fileno,list->confld,*mem->value.short_value);
											}
										}
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.integer_value=PP_EVALUATEint(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.integer_value=selected;
										if(fileno!=(-1))
										{
											if(!list->contype)
											{
												FINDFLDSETINT(fileno,list->confld,*mem->value.integer_value);
											}
										}
										break;
									default:
										prterr("Error Field Type [%d] is invalid for Resource [%s].",mem->rscrname);
										break;
								}
#ifdef IF_WE_NEED_TOO
								ExecuteRDArmemFunction(mem);
#endif /* IF_WE_NEED_TO */
							}
							break;
						case 2: /* Connect Virtual Field */
							y=FINDRSC(r,list->conmod);
							if(y>(-1))
							{
								mem=r->rscs+y;
								value=list->dlist->libs[selected];
								switch(mem->field_type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case SOCSECNUM:
									case TIMES:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case ZIPCODE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
										if(!isEMPTY(list->unformat_formula))
										{
											value=PP_EVALUATEstr(list->unformat_formula,list->SubFunc,list->args);
										} 
										if(value!=NULL) lf=RDAstrlen(value);
											else lf=0;
										if(value!=NULL && lf>0)
										{
											QUICKALLOC(mem->value.string_value,mem->dlen,lf+1);
											memcpy(mem->value.string_value,value,lf+1);
										} else if(mem->dlen>0)
										{
											memset(mem->value.string_value,0,mem->dlen);
										} else {
											mem->value.string_value=NULL;
											mem->dlen=0;
										}
										if(mem->dlen>mem->field_length && mem->field_length>0) mem->value.string_value[mem->field_length]=0;
										if(fileno!=(-1))
											FINDFLDSETSTRING(fileno,list->confld,mem->value.string_value);
										if(!isEMPTY(list->unformat_formula))
										{
											if(value!=NULL) Rfree(value);
										}
										break;
									case BOOLNS:
									case CHARACTERS:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.string_value=PP_EVALUATEbol(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.string_value=selected;
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.float_value=PP_EVALUATEdbl(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.float_value=selected;
										break;
									case SHORTV:
									case SSHORTV:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.short_value=PP_EVALUATEsht(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.short_value=selected;
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										if(!isEMPTY(list->unformat_formula))
										{
											*mem->value.integer_value=PP_EVALUATEint(list->unformat_formula,list->SubFunc,list->args);
										} else *mem->value.integer_value=selected;
										break;
									default:
										prterr("Error Field Type [%d] is invalid for Resource [%s].",mem->rscrname);
										break;
								}
#ifdef IF_WE_NEED_TOO
								ExecuteRDArmemFunction(mem);
#endif /* IF_WE_NEED_TO */
							}
							break;
					}
				}
			}
		}
	}
}
void xGETVirtualsFromExpression(RDArsrc *r,char *exprssn,int line,char *file)
{
	char *libx=NULL,*tmpx=NULL,*tx=NULL;
	APPlib *filenames=NULL;
	int x,y,vc=0,start=(-1);
	DFvirtual *d=NULL;
	RDAvirtual *v,*v1;
	char *jones=NULL;
	int mi=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG GETVirtualsFromExpression on Custom Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(exprssn))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(r->module)+10);
		tmpx=adddashes(r->module);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(tmpx!=NULL) Rfree(tmpx);
		tmpx=NULL;
		libx=NULL;
		if(filenames==NULL) filenames=APPlibNEW();
		for(x=0;x<filenames->numlibs;++x)
		{
/*
			tx=RDAstrstr(exprssn,filenames->libs[x]); 
*/
			jones=Rmalloc(RDAstrlen(filenames->libs[x])+3);
			sprintf(jones,"[%s]",filenames->libs[x]);
			tx=RDAstrstr(exprssn,jones); 
			if(!isEMPTY(tx))
			{
				for(y=0,v1=r->virflds;y<r->num;++y,++v1)
				{
					if(!RDAstrcmp(v1->name,
						filenames->libs[x])) break;
				}	
				if(y>=r->num)
				{
					mi=FINDRSC(r,filenames->libs[x]);
					if(mi==(-1))
					{
					d=getDFvirtual(r->module,filenames->libs[x]);
					if(d!=NULL)
					{
						vc=ADVaddSCRNvirtual(r,d->name,d->type,d->length,d->expression,d->when,FALSE);
						if(start<0) start=vc;
						FreeDFvirtual(d);
						d=NULL;
					}
					}
				}
			}
			if(jones!=NULL) Rfree(jones);
		}
		if(start>(-1))
		{
			vc=start;
			while(TRUE)
			{
				if(vc>r->num) break;
				for(x=0;x<filenames->numlibs;++x)
				{
					v=r->virflds+(vc-1);
					if(v->expression!=NULL && 
						RDAstrcmp(v->name,filenames->libs[x]))
					{
						tx=RDAstrstr(v->expression,filenames->libs[x]); 
						if(!isEMPTY(tx))
						{
							for(y=0,v1=r->virflds;y<r->num;++y,++v1)
							{
								if(!RDAstrcmp(v1->name,
									filenames->libs[x])) break;
							}	
							if(y>=r->num)
							{
								mi=FINDRSC(r,filenames->libs[x]);
								if(mi==(-1))
								{
								d=getDFvirtual(r->module,filenames->libs[x]);
								if(d!=NULL)
								{
									ADVaddSCRNvirtual(r,d->name,d->type,d->length,d->expression,d->when,FALSE);
									FreeDFvirtual(d);
									d=NULL;
								}
								}
							}
						}
					}
				}
				++vc;
			}
		}
		if(filenames!=NULL) freeapplib(filenames);
	}
}
void xGET_RANGESCREEN_EXPRESSION_VIRTUALS(RDArsrc *r,int line,char *file)
{
	char *exprssn=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG GET_SCREEN_VIRTUALS Creating Virtual Fields used on Custom Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	FINDRSCGETSTRING(r,"EXPRESSION",&exprssn);
	GETVirtualsFromExpression(r,exprssn);
	if(exprssn!=NULL) Rfree(exprssn);
}
void xSetRDAScrolledListEditable(RDArsrc *r,short fileno,short keyno,
	short editable,short include_keys,int line,char *file)
{
	int x,y;
	RDAScrolledList *list=NULL;
	NRDkey *key;
	NRDpart *part;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG SetRDAScrolledListEditable for Screen [%s][%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(!include_keys)
		{
			key=KEYNUM(fileno,keyno);
			if(key!=NULL)
			{
				for(x=0,list=r->lists;x<r->numlists;++x,++list)
				{
					if(list->contype==0)
					{
						for(y=0,part=key->part;y<key->numparts;++y,++part)
						{
							if(!RDAstrcmp(part->name,list->confld)) break;
						}
					} else if(((list->contype==1) || (list->contype==3)) && (list->type==0))
					{
						for(y=0,part=key->part;y<key->numparts;++y,++part)
						{
							if(!RDAstrcmp(part->name,list->confld)) break;
						}
					} else y=INT_MAX;
					if(y>=key->numparts)
					{
						FINDRSCSETEDITABLE(r,list->name,
							(editable ? TRUE:FALSE));
					} else {
						FINDRSCSETEDITABLE(r,list->name,TRUE);
					}
				}
			} else {
				for(x=0,list=r->lists;x<r->numlists;++x,++list)
				{
					FINDRSCSETEDITABLE(r,list->name,(editable ? TRUE:FALSE));
				}
			}
		} else {
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{	
				FINDRSCSETEDITABLE(r,list->name,(editable ? TRUE:FALSE));
			}
		}
	}
}
char xSetPowerAddSupportingFiles(RDArsrc *mrsrc,PowerAdd *POWERADD,void (*SubFunc)(...),void *args,short update_rsrcs,short display_diags,int line,char *file)
{
	int x,y;
	short fileno=(-1);
	RDAaddsub *s;
	NRDkey *key;
	NRDpart *part;
	RDAconfld *confld;
	NRDfield *field=NULL;
	short keyno=1,getfile=FALSE;
	char *temp=NULL,*temp1=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG xSetPowerAddSupportingFiles Update Resources [%s] Display Diagnostics [%s] at line [%d] program [%s].",
			(update_rsrcs==TRUE?"TRUE":"FALSE"),
			(display_diags==TRUE?"TRUE":"FALSE"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
	if(POWERADD->subord!=NULL)
	{
	for(x=0,s=POWERADD->subord;x<POWERADD->subnum;++x,++s)
	{
		fileno=SCRNFILENUMBER(mrsrc,s->module,s->filename,s->occurrence);
		if(fileno!=(-1))
		{
			keyno=1;
			key=KEYNUM(fileno,keyno);
			if(s->confld!=NULL && key!=NULL)
			{
				if(key->numparts==s->num && key->part!=NULL)
				{
					for(y=0,confld=s->confld,part=key->part;(y<key->numparts && !getfile);
						++y,++confld,++part)
					{
						field=FLDNRD(fileno,confld->fldname);
						if(field!=NULL)
						{
							if(!isEMPTY(confld->conname))
							{
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->dlen=0;
										field->data.string_value=PP_EVALUATEstr(confld->conname,SubFunc,args);
										if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
										if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
										break;
									case BOOLNS:
									case CHARACTERS:
										*field->data.string_value=PP_EVALUATEbol(confld->conname,SubFunc,args);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										*field->data.float_value=PP_EVALUATEdbl(confld->conname,SubFunc,args);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=PP_EVALUATEsht(confld->conname,SubFunc,args);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										*field->data.integer_value=PP_EVALUATEint(confld->conname,SubFunc,args);
										break;
									default:
									case BUTTONS:
										break;
								}
							} else {
								temp=Rmalloc(RDAstrlen(s->filename)+RDAstrlen(confld->fldname)+5);
								sprintf(temp,"[%s][%s]",s->filename,confld->fldname);
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case ZIPCODE:
										FINDRSCGETSTRING(mrsrc,temp,&field->data.string_value);
										break;
									case BOOLNS:
									case CHARACTERS:
										FINDRSCGETCHAR(mrsrc,temp,field->data.string_value);
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										FINDRSCGETDOUBLE(mrsrc,temp,field->data.float_value);
										break;
									case SHORTV:
									case SSHORTV:
										FINDRSCGETSHORT(mrsrc,temp,field->data.short_value);
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										FINDRSCGETINT(mrsrc,temp,field->data.integer_value);
										break;
									default:
									case BUTTONS:
										break;
								}
								
								if(temp!=NULL) Rfree(temp);
							}	
						} else {
							prterr("Error field not found for confld->fldname [%s] ",(confld->fldname!=NULL ? confld->fldname:""));
						}
					}
					switch(s->dtype)
					{
						default:
						case 0: /* Equal To */
						case 1:
							if(ADVEQLNRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==0)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(307+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,"Sub#:%d  There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",x,(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									memset(stemp,0,101);
									if(POWERADD->AddType>0)
									{
										sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
									} else {
										sprintf(stemp,"INVALID KEY VALUES!");
									}
									WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==0)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(342+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"Sub#: %d  The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",x,
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										memset(stemp,0,101);
										if(POWERADD->AddType>0)
										{
											sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
										} else {
											sprintf(stemp,"INVALID KEY VALUES!");
										}
										WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
						case 2: /* Greater Than or Equal To */ 
						case 3:
							if(ADVGTENRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==2)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(307+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,"Sub#: %d  There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",x,(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									memset(stemp,0,101);
									if(POWERADD->AddType>0)
									{
										sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
									} else {
										sprintf(stemp,"INVALID KEY VALUES!");
									}
									WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==2)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(342+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"Sub#: %d  The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",x,
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										memset(stemp,0,101);
										if(POWERADD->AddType>0)
										{
											sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
										} else {
											sprintf(stemp,"INVALID KEY VALUES!");
										}
										WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
										if(temp1!=NULL) Rfree(temp1);
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										return(FALSE);
									}
									}
								}
							}
							break;
						case 4: /* Greater Than */
						case 5:
							if(ADVNXTNRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								if(display_diags && s->dtype==4)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,
										"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									memset(stemp,0,101);
									if(POWERADD->AddType>0)
									{
										sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
									} else {
										sprintf(stemp,"INVALID KEY VALUES!");
									}
									WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==4)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										memset(stemp,0,101);
										if(POWERADD->AddType>0)
										{
											sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
										} else {
											sprintf(stemp,"INVALID KEY VALUES!");
										}
										WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
						case 6: /* Less Than or Equal */
						case 7:
							if(ADVLTENRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==6)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									memset(stemp,0,101);
									if(POWERADD->AddType>0)
									{
										sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
									} else {
										sprintf(stemp,"INVALID KEY VALUES!");
									}
									WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==6)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);

										memset(stemp,0,101);
										if(POWERADD->AddType>0)
										{
											sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
										} else {
											sprintf(stemp,"INVALID KEY VALUES!");
										}
										WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
						case 8: /* Less Than */
						case 9:
							if(ADVPRVNRDsec(fileno,1,SubFunc,args))
							{
								KEYNRD(fileno,keyno);
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==8)
								{
									for(y=0,part=key->part;(y<key->numparts);++y,++part)
									{
										if(y+1<key->numparts)
										{
											temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
										} else {
											temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
											sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
										}
										temp=stralloccat(temp,temp1);
										if(temp1!=NULL) Rfree(temp1);	
									}
									temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
									sprintf(temp1,
										"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),
										(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
									if(temp!=NULL) Rfree(temp);
									memset(stemp,0,101);
									if(POWERADD->AddType>0)
									{
										sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
									} else {
										sprintf(stemp,"INVALID KEY VALUES!");
									}
									WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
									if(temp1!=NULL) Rfree(temp1);
									return(FALSE);
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc); 
								}
								if(display_diags && s->dtype==8)
								{
									if(!isEMPTY(s->expression))
									{
									if(!PP_EVALUATEbol(s->expression,SubFunc,args))
									{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,
											"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
											(temp==NULL?"":temp),
											(s->module==NULL?"":s->module),
											(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);
										memset(stemp,0,101);
										if(POWERADD->AddType>0)
										{
											sprintf(stemp,"INVALID KEY VALUES on Row %d!",POWERADD->table_row+1);
										} else {
											sprintf(stemp,"INVALID KEY VALUES!");
										}
										WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
										if(temp1!=NULL) Rfree(temp1);
										if(s->skip_widget==FALSE)
										{
										if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc);
										}
										return(FALSE);
									}
									}
								}
							}
							break;
					}
				}
			}
		} else {
			if(display_diags && (s->dtype==0 || s->dtype==2 || s->dtype==4 || s->dtype==6 || s->dtype==8))
			{
			temp1=Rmalloc(300+RDAstrlen(s->module)+RDAstrlen(s->filename));
			sprintf(temp1,"Cannot find the Supporting file of [%s][%s].  Please have this corrected in the Power Add definition so that records can be saved to this file.",(s->module!=NULL?s->module:""),(s->filename!=NULL?s->filename:""));
			memset(stemp,0,101);
			if(POWERADD->AddType>0)
			{
				sprintf(stemp,"CANNOT FIND SUPPORTING FILE on Row %d!",POWERADD->table_row+1);
			} else {
				sprintf(stemp,"CANNOT FIND SUPPORTING FILE!");
			}
			WARNINGDIALOG("WARNING DIALOG SCREEN",stemp,(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
			if(temp1!=NULL) Rfree(temp1);
			return(FALSE);
			}
		}
		if(fileno!=(-1))
		{
			if(update_rsrcs) updatefilerecord2rsrc(fileno,mrsrc);
			else filerecord2rsrc(fileno,mrsrc); 
		}
	}
	}
	}
	return(TRUE);
}
