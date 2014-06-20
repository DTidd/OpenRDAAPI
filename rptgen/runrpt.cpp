#include <rptgen.hpp>
#ifdef SCO50
extern int toupper(int);
extern int tolower(int);
#endif
#ifdef SCO50P
extern int toupper(int);
extern int tolower(int);
#endif

short INVERSEsht(short s)
{
	return((-1)*s);
}
int INVERSEint(int s)
{
	return((-1)*s);
}
double INVERSEdbl(double s)
{
	return((-1)*s);
}
#define CONNECTFILENUMBER(a,b)	xCONNECTFILENUMBER(a,b,__LINE__,__FILE__)
short xCONNECTFILENUMBER(RUNfile *runfile,char *filename,int line,char *file)
{
	RUNfile *a;
	int x;
	short t=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG CONNECTFILENUMBER Finding File Number for File [%s] off Runfile [%s] [%s] at line [%d] program [%s].",filename,runfile->module,runfile->filename,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(runfile->child!=NULL)
	{
		for(x=0,a=runfile->child;x<runfile->num;++x,++a)
		{
			if(!RDAstrcmp(a->filename,filename))
			{
				return(a->fileno);
			}
			if(a->child!=NULL)
			{
				t=CONNECTFILENUMBER(a,filename);
				if(t!=(-1))
				{
					return(t);
				}
			}
		}
	}
	return(-1);
}
RUNfile *GimmyRUNfile(RUNfile *r,char *modx,char *filx,short occ,short *count)
{
	RUNfile *f=NULL,*f1=NULL;
	int x=0;

	if(r->child!=NULL)
	{
		for(x=0,f=r->child;x<r->num;++x,++f)
		{
			if(!RDAstrcmp(f->module,modx) && 
				!RDAstrcmp(f->filename,filx) 
				&& f->fileno!=(-1)) 
			{
				++(*count);
				if(*count==occ) 
				{
					return(f);
				} else {
					if(f->child!=NULL)
					{
						f1=GimmyRUNfile(f,modx,filx,occ,count);
						if(f1!=NULL) return(f1);
					}
				}
			} else {
				if(f->child!=NULL)
				{
					f1=GimmyRUNfile(f,modx,filx,occ,count);
					if(f1!=NULL) 
					{
						return(f1);
					}
				}
			}
		}
	}	
	return(NULL);
}
short xREPORTFILENUMBER(RDArunrpt *rrpt,char *module,char *filename,short occurrance,short type,int line,char *file)
{
	short fileno=(-1),count=0;
	int x=0;
	ReportFile *r=NULL;
	RUNfile *f=NULL;
	RUNfile *prim=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG REPORTFILENUMBER Returning Filenumber of Report file for Module [%s] Filename [%s] Occurrance [%d] at line [%d] program [%s].",module,filename,occurrance,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!type)
	{
		if(rrpt->primary_file_no==0) rrpt->primary_file_no=1;
		prim=rrpt->prims+(rrpt->primary_file_no-1);
		if(prim!=NULL)
		{
			if(!RDAstrcmp(module,prim->module) && !RDAstrcmp(filename,prim->filename))
			{
				++count;
				if(count==occurrance)
				{
					fileno=prim->fileno;
					return(fileno);
				}
			}
/* Problems here with children of children of children etc */
			if(prim->child!=NULL)
			{
				f=GimmyRUNfile(prim,module,filename,occurrance,&count);
				if(f!=NULL)
				{
					fileno=f->fileno;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagrptgen)
					{
						prterr("DIAG REPORTFILENUMBER Returning Filenumber [%d] for Module [%s] File [%s] Occurrance [%d].",fileno,MODULENAME(fileno),FILENAME(fileno),occurrance);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					return(fileno);
				}
			}
			prterr("Error REPORTFILENUMBER returning(-1) File Not Found for Module [%s] File [%s] Occurrance [%d] to line [%d] program [%s].",module,filename,occurrance,line,file);
			return(-1);
		} else {
			prterr("Error REPORTFILENUMBER returning(-1) File Not Found for Module [%s] File [%s] Occurrance [%d] to line [%d] program [%s].",module,filename,line,occurrance,file);
			return(-1);
		}
	} else if(rrpt->ofiles!=NULL)
	{
		for(x=0,r=rrpt->ofiles;x<rrpt->numofiles;++x,++r)
		{
			if(!RDAstrcmp(r->module,module) && !RDAstrcmp(r->filename,filename) 
				&& r->fileno!=(-1)) break;
		}
		if(x<rrpt->numofiles)
		{
			fileno=r->fileno;
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen)
			{
				prterr("DIAG REPORTFILENUMBER Returning Filenumber [%d] for Module [%s] File [%s] Occurrance [%d].",fileno,MODULENAME(fileno),FILENAME(fileno),occurrance);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(fileno);
		} else {
			prterr("Error REPORTFILENUMBER returning(-1) for Module [%s] File [%s] Occurrance [%d] Not Found to Line [%d] Program [%s].",module,filename,occurrance,line,file);
		}
	}
	return(fileno);
}
char *INVERSEstr(char *s)
{
	char *tmp=NULL;
	char *hold;

	tmp=Rmalloc(RDAstrlen(s)+1);
	hold=tmp;
	for(;*s;++s,++hold)
	{
		*hold=255-(*s);
	}
	*hold=0;
	return(tmp);
}
char *stripfourthname(char *rname)
{
	char *tmp=NULL;
	short x,l,s,start=0;

	for(x=0,l=0,s=0;x<RDAstrlen(rname);++x) 
	{
		if(rname[x]=='[')
 		{
			++s;
			start=x+1;
		}
		if(s==4 && rname[x]==']') break;
		if(s==4) ++l;
	}
	if(l>0)
	{
		tmp=Rmalloc(l);
		memcpy(tmp,&rname[start],l-1);
		tmp[l-1]=0;
	} else tmp=NULL;
	return(tmp);
}
short addReportFile(RDArunrpt *rrpt,char *module,char *file,short type,short range,short force_new)
{
	int y;
	ReportFile *tmp,*hold;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG addReportFile Module [%s] File [%s] type [%d] range [%s] force_new [%s].",module,file,type,(range ? "Yes":"No"),(force_new ? "Yes":"No"));
		SHOWrunfiles(rrpt);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(module!=NULL && file!=NULL)
	{
		if(!type)
		{
			if(rrpt->files!=NULL)
			{
				rrpt->files=Rrealloc(rrpt->files,(rrpt->numfiles+1)*
					sizeof(ReportFile));
			} else {
				rrpt->files=Rmalloc(sizeof(ReportFile));
			}
			tmp=rrpt->files+rrpt->numfiles;
			tmp->module=stralloc(module);
			tmp->filename=stralloc(file);
			tmp->range=range;
			hold=NULL;
			if(!force_new)
			{
				for(y=0,hold=rrpt->files;y<rrpt->numfiles;++y,++hold)
				{
		 			if(!RDAstrcmp(hold->module,tmp->module) &&
						!RDAstrcmp(hold->filename,tmp->filename)) break;
				}
			} else {
				y=0;
			} 
			if(y<rrpt->numfiles && !force_new && hold!=NULL)
			{
				tmp->fileno=hold->fileno;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen)
				{
					prterr("DIAG addReportFile Module [%s] File [%s] type [%d] Filenumber [%d]",(module!=NULL?module:""),(file!=NULL?file:""),type,tmp->fileno);
					SHOWrunfiles(rrpt);TRACE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			} else {
				tmp->fileno=OPNNRDsec(tmp->module,tmp->filename,TRUE,FALSE,TRUE);
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen)
				{
					prterr("DIAG addReportFile Module [%s] File [%s] type [%d] Filenumber [%d]",(module!=NULL?module:""),(file!=NULL?file:""),type,tmp->fileno);
					SHOWrunfiles(rrpt);TRACE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			}
			++rrpt->numfiles;
			return(tmp->fileno);
		} else {
			if(rrpt->ofiles!=NULL)
			{
				rrpt->ofiles=Rrealloc(rrpt->ofiles,(rrpt->numofiles+1)*sizeof(ReportFile));
			} else {
				rrpt->ofiles=Rmalloc(sizeof(ReportFile));
			}
			tmp=rrpt->ofiles+rrpt->numofiles;
			tmp->module=stralloc(module);
			tmp->filename=stralloc(file);
			hold=NULL;
			for(y=0,hold=rrpt->ofiles;y<rrpt->numofiles;++y,++hold)
			{
				if(!RDAstrcmp(tmp->module,hold->module) &&
					!RDAstrcmp(tmp->filename,hold->filename)) break;
			}
			if(y<rrpt->numofiles && hold!=NULL)
			{
				tmp->fileno=hold->fileno;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen)
				{
					prterr("DIAG addReportFile Module [%s] File [%s] type [%d] Filenumber [%d]",(module!=NULL?module:""),(file!=NULL?file:""),type,tmp->fileno);
					SHOWrunfiles(rrpt);TRACE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			} else {
				tmp->fileno=OPNNRDsec(tmp->module,tmp->filename,TRUE,TRUE,FALSE);
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen)
				{
					prterr("DIAG addReportFile Module [%s] File [%s] type [%d] Filenumber [%d]",(module!=NULL?module:""),(file!=NULL?file:""),type,tmp->fileno);
					SHOWrunfiles(rrpt);TRACE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			}
			++rrpt->numofiles;
			return(tmp->fileno);
		}
	} else { 
		return(-1);
	}
}
short OPNsubfiles(RUNfile *runfile,RDArunrpt *rrpt)
{
	int y,z;
	RUNfile *sub,*sub1;
	short force_new=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG OPNsubfiles for Module [%s] File [%s].",runfile->module,runfile->filename);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(runfile->num>0 && runfile->child!=NULL)
	{
		for(y=0,sub=runfile->child;y<runfile->num;++y,++sub)
		{
			force_new=FALSE;
			if(!RDAstrcmp(runfile->module,sub->module) &&
				!RDAstrcmp(runfile->filename,sub->filename))
			{
				force_new=TRUE;
			} else {
				for(z=0,sub1=runfile->child;z<y;++z,++sub1)
				{
					if(!RDAstrcmp(sub->module,sub1->module) && !RDAstrcmp(sub->filename,sub1->filename))
					{
						force_new=TRUE;
						break;
					}
				}
			}
			sub->fileno=addReportFile(rrpt,sub->module,sub->filename,FALSE,sub->range,force_new);
			sub->keyno=KEYNUMBERBYNAME(sub->module,sub->filename,sub->keyname);
			if(sub->fileno==(-1) || sub->keyno==(-1)) 
			{
				prterr("Error cannot open Sub-File [%s] [%s] using Keyname [%s] joined to File [%s] [%s].  Report aborted.",sub->module,sub->filename,sub->keyname,runfile->module,runfile->filename);
				return(-1);
			}
			if(OPNsubfiles(sub,rrpt)==(-1)) return(-1);
		}
	}
	return(0);
}
short OPNReportfiles(RDArunrpt *rrpt)
{
	int x;
	RUNfile *prim;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG OPNReportFiles.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rrpt->numprims<=0 || rrpt->prims==NULL) return(-1);
	for(x=0,prim=rrpt->prims;x<rrpt->numprims;++x,++prim)
	{
		prim->fileno=addReportFile(rrpt,prim->module,prim->filename,FALSE,prim->range,FALSE);
		prim->keyno=KEYNUMBERBYNAME(prim->module,prim->filename,prim->keyname);
		if(prim->fileno==(-1) || prim->keyno==(-1)) 
		{
			prterr("Error cannot open Primary File [%s] [%s] using Keyname [%s].  Report Aborted.",prim->module,prim->filename,prim->keyname);
			return(-1);
		}
		if(OPNsubfiles(prim,rrpt)==(-1)) return(-1);
	}
	return(0);
}
void xCLSReportfiles(RDArunrpt *rrpt,short type,int line,char *file)
{
	int x,y;
	ReportFile *tmp,*hold;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG CLSReportFIles of type [%d] at line [%d] program [%s].",type,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!type)
	{
		if(rrpt->files!=NULL)
		{
			for(x=0,tmp=rrpt->files;x<rrpt->numfiles;++x,++tmp)
			{
				if(tmp->fileno!=(-1)) 
				{
					for(y=(x+1),hold=(rrpt->files+x+1);y<rrpt->numfiles;++y,++hold)
					{
						if(!RDAstrcmp(hold->module,tmp->module) &&
							!RDAstrcmp(hold->filename,tmp->filename) && tmp->fileno==hold->fileno)
						{
							hold->fileno=(-1);
						}
					}
					CLSNRD(tmp->fileno);
				}
				tmp->fileno=(-1);
				if(tmp->module!=NULL) Rfree(tmp->module);
				if(tmp->filename!=NULL) Rfree(tmp->filename);
			}
			Rfree(rrpt->files);
			rrpt->files=NULL;
			rrpt->numfiles=0;
		}
	} else {
		if(rrpt->ofiles!=NULL)
		{
			for(x=0,tmp=rrpt->ofiles;x<rrpt->numofiles;++x,++tmp)
			{
				if(tmp->fileno!=(-1)) 
				{
					for(y=0,hold=rrpt->ofiles;y<rrpt->numofiles;++y,++hold)
					{
						if(!RDAstrcmp(hold->module,tmp->module) &&
							!RDAstrcmp(hold->filename,tmp->filename) && hold->fileno==tmp->fileno)
						{
							hold->fileno=(-1);
						}
					}
					CLSNRD(tmp->fileno);
					tmp->fileno=(-1);
				}
				if(tmp->module!=NULL) Rfree(tmp->module);
				if(tmp->filename!=NULL) Rfree(tmp->filename);
			}
			if(rrpt->ofiles!=NULL) Rfree(rrpt->ofiles);
			rrpt->ofiles=NULL;
			rrpt->numofiles=0;
		}
	}
}
short fieldexists(int total,NRDfield *fields,char *name)
{
	int x;
	NRDfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG Checking if [%s] already fieldexists.",name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	for(x=0,f=fields;x<total;++x,++f)
	{
		if(!RDAstrcmp(f->name,name)) return(TRUE);
	}
	return(FALSE);
}
short addfield(RDArunrpt *rrpt,char *name,short type,short len)
{
	NRDfield *f1;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG addfield name [%s] Type [%d] [%s] Length [%d].",
			(name!=NULL ? name:"NULL"),type,fieldtypedesc[type],len);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rrpt->fields!=NULL)
	{
		rrpt->fields=Rrealloc(rrpt->fields,(rrpt->nofields+1)*
			sizeof(NRDfield));
	} else {
		rrpt->fields=Rmalloc(sizeof(NRDfield));
	}
	f1=rrpt->fields+rrpt->nofields;	
	f1->name=stralloc(name);
	f1->type=type;
	f1->len=len;
	++rrpt->nofields;
	return(rrpt->nofields);
}
short addkeyelements(RDArunrpt *rrpt,char *name)
{
	NRDkey *f1;
	NRDpart *part;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG addkeyelements Add Key Element [%s].",name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rrpt->keys==NULL)
	{
		rrpt->nokeys=1;
		rrpt->keys=Rmalloc(sizeof(NRDkey));
		f1=rrpt->keys;
		f1->name=stralloc("USER DEFINED KEY");
		f1->numparts=0;
		f1->part=NULL;
	}
	f1=rrpt->keys;
	if(f1->part!=NULL)
	{
		f1->part=Rrealloc(f1->part,(f1->numparts+1)*sizeof(NRDpart));
	} else {
		f1->part=Rmalloc(sizeof(NRDpart));
	}
	part=f1->part+(f1->numparts);	
	part->name=stralloc(name);
	++f1->numparts;
	return(f1->numparts);
}
short xaddsortfield(RDArunrpt *rrpt,RDAreport *rpt,char *name,
	char *subname,int type,int line,char *file)
{
	short fileno;
	int y;
	RDAvirtual *v;
	NRDfield *f;
	char *modname=NULL,*fldname=NULL,*filename=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG addsortfield for [%s] subname [%s] type [%d] at line [%d] program [%s]",name,(subname!=NULL ? subname:""),type,line,file);
	}	
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!fieldexists(rrpt->nofields,rrpt->fields,name) || subname!=NULL)
	{
		modname=stripmodulename((char *)name);
		filename=stripfilename((char *)name);
		if(filename!=NULL && RDAstrcmp(filename,"LOWER") && 
			RDAstrcmp(filename,"UPPER") && RDAstrcmp(filename,"DESCENDING") && 
			RDAstrcmp(filename,"DESCEND LOWER") && 
			RDAstrcmp(filename,"DESCEND UPPER")) 
		{
			fileno=REPORTFILENUMBER(rrpt,modname,filename,1,FALSE);
			if(fileno!=(-1))
			{
				fldname=stripfieldname((char *)name);
				f=FLDNRDsec(fileno,fldname,TRUE,FALSE,FALSE);
				if(f!=NULL)
				{
					switch(type)
					{
						default:
						case ASCENDING:
							addfield(rrpt,name,f->type,f->len);
							break;
						case ASCENDLOWER:
						case ASCENDUPPER:
						case DESCENDING:
						case DESCENDLOWER:
						case DESCENDUPPER:
							addfield(rrpt,subname,f->type,f->len);
							break;
					}
					if(modname!=NULL) Rfree(modname);
					if(filename!=NULL) Rfree(filename);
					if(fldname!=NULL) Rfree(fldname);
					return(TRUE);
				} else {
					prterr("Error Field [%s] couldn't be found in Module [%s] File [%s] fileno [%d] from line [%d] program [%s].",fldname,modname,filename,fileno,line,file);
					if(modname!=NULL) Rfree(modname);
					if(filename!=NULL) Rfree(filename);
					if(fldname!=NULL) Rfree(fldname);
					return(FALSE);
				}
			} else {
				prterr("Error Module [%s] File [%s] couldn't be found, must not be opened at line [%d] program [%s].",modname,filename,line,file);
				if(modname!=NULL) Rfree(modname);
				if(filename!=NULL) Rfree(filename);
				return(FALSE);
			}
		} else { /* virtual field */
			for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
			{
				if(!RDAstrcmp(modname,v->name)) break;
			}
			if(y<rpt->numvirtuals)
			{
				switch(type)
				{
					default:
					case ASCENDING:
						addfield(rrpt,name,v->type,v->len);
						break;
					case ASCENDLOWER:
					case ASCENDUPPER:
					case DESCENDING:
					case DESCENDLOWER:
					case DESCENDUPPER:
						addfield(rrpt,subname,v->type,v->len);
						break;
				}
				if(modname!=NULL) Rfree(modname);
				return(TRUE);
			} else {
				prterr("Error Virtual Field [%s] not found at line [%d] program [%s].",(modname!=NULL ? modname:"NULL"),line,file);
				if(modname!=NULL) Rfree(modname);
				return(FALSE);
			}
		}
	} return(TRUE);
}

/*
APPlib *makesortfieldvallist(APPlib *field_list,RDArunrpt *rrpt,RDAreport *rpt)
{
	int x=0,y=0,z=0,u=0,t=0;
	RDAaccum *a=NULL;
	RDAimage *i=NULL;
	RDApval *pval=NULL;
	RDAsort *s=NULL;
	NRDfield *f=NULL;
	char *temp=NULL,*fname=NULL;
	RDAvirtual *v=NULL;

	if(diagrptgen)
	{
		prterr("DIAG makesortfilefieldlist.");
	}
	if(rrpt->fields!=NULL)
	{
		for(x=0,f=rrpt->fields;x<rrpt->nofields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
		}
		Rfree(rrpt->fields);
		rrpt->nofields=0;
	}
	if(APPlib==NULL)
	{
		field_list=APPlibNEW();
	}
	addAPPlib(field_list,"DELETEFLAG");
	addAPPlib(field_list,"[RECORD COUNTER]");
	for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
	{
		for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
		{
			temp=stripmodulename(a->accname);
			if(!RDAstrcmp(v->name,temp)) break;
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		if((y<rpt->numvirtuals && !v->when) || y>=rpt->numvirtuals)
		{
			addAPPlib(field_list,a->accname);
		}
	}
	if(rpt->image!=NULL)
	{
		for(x=0,i=rpt->image;x<rpt->numimages;++x,++i)
		{
			if(i->pvals!=NULL)
			{
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->symbol_type==FIELDTYPE && 
						RDAstrcmp(pval->name,"[PAGE NO]")
						&& RDAstrcmp(pval->name,"[BREAK LEVEL]")
						&& RDAstrcmp(pval->name,"[BODY COUNT]")	
						&& RDAstrcmp(pval->name,"NEXT TRANSACTION NO")
						&& RDAstrcmp(pval->name,"[PRIMARY FILE NUMBER]")
						&& RDAstrcmp(pval->name,"[WRITE COUNT]")
						&& RDAstrcmp(pval->name,"[SYSTEM DATE]")
						&& RDAstrcmp(pval->name,"[SYSTEM DATE-YYYY]")
						&& RDAstrcmp(pval->name,"[LOGIN IDENTIFICATION]")
						&& RDAstrcmp(pval->name,"[ACCOUNT MANAGER]")
						&& RDAstrcmp(pval->name,"[USERS DEPARTMENT]")
						&& RDAstrcmp(pval->name,"[ABLE TO APPROVE]")
						&& RDAstrcmp(pval->name,"[APPROVAL LIMIT]")
						&& RDAstrcmp(pval->name,"[ACCOUNT GRANT IDENTIFICATION]")
						&& RDAstrcmp(pval->name,"[ACCOUNT REFERENCE TYPE]")
						&& RDAstrcmp(pval->name,"[SYSTEM TIME]")
						&& RDAstrcmp(pval->name,"[SYSTEM TYPE]")
						&& RDAstrcmp(pval->name,"[CURRENT PROCESS ID]")
						&& RDAstrcmp(pval->name,"[CURRENT DIRECTORY]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION NAME]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ADDRESS #1]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ADDRESS #2]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION CITY]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION STATE]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ZIP]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION FAX]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION PHONE]"))
					{
						for(u=0,a=rpt->accum;u<rpt->numaccums;++u,++a)
						{
							temp=stripmodulename(pval->name);
							if(!RDAstrcmp(a->name,temp)) break;
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} 
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						for(t=0,v=rpt->virflds;t<rpt->numvirtuals;++t,++v)
						{
							temp=stripmodulename(pval->name);
							if(!RDAstrcmp(v->name,temp))
							{
								if(!v->when) t=rpt->numvirtuals+1;	
								break;
							}
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						}
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						if(u>=rpt->numaccums && t>=rpt->numvirtuals)
						{
							addAPPlib(field_list,pval->name);
						}
					}
				}
			}
		}
	}
	if(rpt->sort!=NULL)
	{
		for(x=0,s=rpt->sort;x<rpt->numsorts;++x,++s)
		{
			switch(s->type)
			{
				default:
				case ASCENDING:
					addAPPlib(field_list,s->name);
					break;
				case ASCENDLOWER:
					addAPPlib(field_list,s->name);
					break;
				case ASCENDUPPER:
					addAPPlib(field_list,s->name);
					break;
				case DESCENDING:
					addAPPlib(field_list,s->name);
					break;
				case DESCENDLOWER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[DESCEND LOWER]",s->name);
					addAPPlib(field_list,s->name);
					if(temp!=NULL) Rfree(temp);
					break;
				case DESCENDUPPER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[DESCEND UPPER]",s->name);
					addAPPlib(field_list,s->name);
					if(temp!=NULL) Rfree(temp);
					break;
			}
		}
	}
}
*/
static short CHECKENGINE(short engine)
{
	switch(engine)
	{
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine: return(TRUE);
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: return(TRUE);
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(TRUE);
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			return(TRUE);
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine: 
		case NRDLocalCtreeEngine: 
			return(TRUE);
#endif
		default: return(FALSE);
	}
}
short makesortfile(RDArunrpt *rrpt,RDAreport *rpt)
{
	int x=0,y=0,z=0,u=0,t=0;
	RDAaccum *a=NULL;
	RDAimage *i=NULL;
	RDApval *pval=NULL;
	RDAsort *s=NULL;
	NRDfield *f=NULL;
	char *temp=NULL,*fname=NULL;
	NRDfield *field=NULL,*OFIELDS=NULL,*field1=NULL;
	NRDpart *part=NULL;
	NRDkey *key=NULL;
	RDAvirtual *v=NULL;
	short fileno=(-1),offcount=(-1);
	int offset=0;
	APPlib *nonkeyfld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG makesortfile.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rrpt->fields!=NULL)
	{
		for(x=0,f=rrpt->fields;x<rrpt->nofields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
		}
		Rfree(rrpt->fields);
		rrpt->nofields=0;
	}
	rrpt->nofields=2;
	rrpt->fields=Rmalloc(2*sizeof(NRDfield));	
	f=rrpt->fields+0;
	f->name=stralloc("DELETEFLAG");
	f->type=BOOLNS;
	f->len=1;
	f=rrpt->fields+1;
	f->name=stralloc("[RECORD COUNTER]");
	f->type=DOUBLEV;
	f->len=9;
	for(x=0,a=rpt->accum;x<rpt->numaccums;++x,++a)
	{
		for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
		{
			temp=stripmodulename(a->accname);
			if(!RDAstrcmp(v->name,temp)) break;
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		if((y<rpt->numvirtuals && !v->when) || y>=rpt->numvirtuals)
		{
			if(!addsortfield(rrpt,rpt,a->accname,NULL,ASCENDING))
			{
				prterr("Error Accumulator [%s] will not be computed, it is defined incorrectly [%s].",a->name,a->accname);
			}
		}
	}
	if(rpt->image!=NULL)
	{
		for(x=0,i=rpt->image;x<rpt->numimages;++x,++i)
		{
			if(i->pvals!=NULL)
			{
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->symbol_type==FIELDTYPE && 
						RDAstrcmp(pval->name,"[PAGE NO]")
						&& RDAstrcmp(pval->name,"[BREAK LEVEL]")
						&& RDAstrcmp(pval->name,"[BODY COUNT]")	
						&& RDAstrcmp(pval->name,"NEXT TRANSACTION NO")
						&& RDAstrcmp(pval->name,"[PRIMARY FILE NUMBER]")
						&& RDAstrcmp(pval->name,"[WRITE COUNT]")
						&& RDAstrcmp(pval->name,"[SYSTEM DATE]")
						&& RDAstrcmp(pval->name,"[SYSTEM DATE-YYYY]")
						&& RDAstrcmp(pval->name,"[LOGIN IDENTIFICATION]")
						&& RDAstrcmp(pval->name,"[ACCOUNT MANAGER]")
						&& RDAstrcmp(pval->name,"[USERS DEPARTMENT]")
						&& RDAstrcmp(pval->name,"[ABLE TO APPROVE]")
						&& RDAstrcmp(pval->name,"[APPROVAL LIMIT]")
						&& RDAstrcmp(pval->name,"[ACCOUNT GRANT IDENTIFICATION]")
						&& RDAstrcmp(pval->name,"[ACCOUNT REFERENCE TYPE]")
						&& RDAstrcmp(pval->name,"[SYSTEM TIME]")
						&& RDAstrcmp(pval->name,"[SYSTEM TYPE]")
						&& RDAstrcmp(pval->name,"[CURRENT DIRECTORY]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION NAME]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ADDRESS #1]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ADDRESS #2]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION CITY]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION STATE]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ZIP]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION FAX]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION PHONE]"))
					{
						for(u=0,a=rpt->accum;u<rpt->numaccums;++u,++a)
						{
							temp=stripmodulename(pval->name);
							if(!RDAstrcmp(a->name,temp)) break;
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} 
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						for(t=0,v=rpt->virflds;t<rpt->numvirtuals;++t,++v)
						{
							temp=stripmodulename(pval->name);
							if(!RDAstrcmp(v->name,temp))
							{
								if(!v->when) t=rpt->numvirtuals+1;	
								break;
							}
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						}
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						if(u>=rpt->numaccums && t>=rpt->numvirtuals)
						{
							if(!addsortfield(rrpt,rpt,pval->name,NULL,ASCENDING))
							{
								prterr("Error Field [%s] not found.",pval->name);
							}
						}
					}
				}
			}
		}
	}
	if(rpt->sort!=NULL)
	{
		for(x=0,s=rpt->sort;x<rpt->numsorts;++x,++s)
		{
			switch(s->type)
			{
				default:
				case ASCENDING:
					if(!addsortfield(rrpt,rpt,s->name,NULL,ASCENDING))
					{
						prterr("Error Field [%s] not found.",s->name);
					} else {
						addkeyelements(rrpt,s->name);
					}
					break;
				case ASCENDLOWER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[LOWER]",s->name);
					if(!addsortfield(rrpt,rpt,s->name,temp,ASCENDLOWER))
					{
						prterr("Error Field [%s] not found.",s->name);
					} else {
						addkeyelements(rrpt,temp);
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case ASCENDUPPER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[UPPER]",s->name);
					if(!addsortfield(rrpt,rpt,s->name,temp,ASCENDUPPER))
					{
						prterr("Error Field [%s] not found.",s->name);
					} else {
						addkeyelements(rrpt,temp);
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case DESCENDING:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[DESCENDING]",s->name);
					if(!addsortfield(rrpt,rpt,s->name,temp,DESCENDING))
					{
						prterr("Error Field [%s] not found.",s->name);
					} else {
						addkeyelements(rrpt,temp);
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case DESCENDLOWER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[DESCEND LOWER]",s->name);
					if(!addsortfield(rrpt,rpt,s->name,temp,DESCENDLOWER))
					{
						prterr("Error Field [%s] not found.",s->name);
					} else {
						addkeyelements(rrpt,temp);
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case DESCENDUPPER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[DESCEND UPPER]",s->name);
					if(!addsortfield(rrpt,rpt,s->name,temp,DESCENDUPPER))
					{
						prterr("Error Field [%s] not found.",s->name);
					} else {
						addkeyelements(rrpt,temp);
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
			}
		}
	}
	addkeyelements(rrpt,"[RECORD COUNTER]");
	if(rrpt->sortfile==NULL)
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d-%d",RGETPID(),rrpt->level);
		rrpt->sortfile=stralloc(stemp);
	}
	if(!CHECKENGINE(rpt->engine_type))
	{
#ifdef USE_MYISAM
		rpt->engine_type=NRDMyIsamEngine;
#else
#ifdef USE_CTREE
		rpt->engine_type=NRDLocalCtreeEngine;
#endif
#endif
	}
	OFIELDS=Rmalloc(rrpt->nofields*sizeof(NRDfield));
	y=0;
	if((rpt->engine_type==NRDMyIsamEngine) || (rpt->engine_type==NRDMySQLXpertEngine) || (rpt->engine_type==NRDMySQLEngine) || (rpt->engine_type==NRDMySQLAutoEngine))
	{
		nonkeyfld=APPlibNEW();
		OFIELDS=Rmalloc(rrpt->nofields*sizeof(NRDfield));
		y=0;
		key=rrpt->keys+0;
		for(x=0,field=rrpt->fields;x<rrpt->nofields;++x,++field)
		{
			if(IsKeyPart(key,field->name))
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			} else {
				addAPPlib(nonkeyfld,field->name);
			}
		}
		SORTAPPlib(nonkeyfld);
		for(z=0;z<nonkeyfld->numlibs;++z)
		{
			for(x=0,field=rrpt->fields;x<rrpt->nofields;++x,++field)
			{
				if(!RDAstrcmp(field->name,nonkeyfld->libs[z]) && field->type!=VARIABLETEXT && field->type!=SCROLLEDTEXT && field->len>0)
				{
					field1=OFIELDS+y;
					field1->name=stralloc(field->name);
					field1->type=field->type;
					field1->len=field->len;
					++y;
					break;
				}
			}
		}
		for(z=0;z<nonkeyfld->numlibs;++z)
		{
			for(x=0,field=rrpt->fields;x<rrpt->nofields;++x,++field)
			{
				if(!RDAstrcmp(field->name,nonkeyfld->libs[z]) && (field->type==VARIABLETEXT || field->type==SCROLLEDTEXT || field->len<1))
				{
					field1=OFIELDS+y;
					field1->name=stralloc(field->name);
					field1->type=field->type;
					field1->len=field->len;
					++y;
					break;
				}
			}
		}
		if(nonkeyfld!=NULL) freeapplib(nonkeyfld);

	} else {
	for(x=0,field=rrpt->fields;x<rrpt->nofields;++x,++field)
	{
		if(field->type!=VARIABLETEXT && field->type!=SCROLLEDTEXT
		 	&& field->len>0)
		{
			field1=OFIELDS+y;
			field1->name=stralloc(field->name);
			field1->type=field->type;
			field1->len=field->len;
			++y;
		}
	}
	for(x=0,field=rrpt->fields;x<rrpt->nofields;++x,++field)
	{
		if(field->type==VARIABLETEXT || field->type==SCROLLEDTEXT
		 	|| field->len==0)
		{
			field1=OFIELDS+y;
			field1->name=stralloc(field->name);
			field1->type=field->type;
			field1->len=field->len;
			++y;
		}
	}
	}
	for(x=0,field=rrpt->fields;x<rrpt->nofields;++x,++field)
	{
		if(field->name!=NULL) Rfree(field->name);
	}
	Rfree(rrpt->fields);
	rrpt->fields=Rmalloc(rrpt->nofields*sizeof(NRDfield));
	for(x=0,field1=OFIELDS,field=rrpt->fields;x<rrpt->nofields;
		++x,++field,++field1)
	{
		field->name=stralloc(field1->name);
		if(field1->name!=NULL) Rfree(field1->name);
		field->type=field1->type;
		field->len=field1->len;
	}
	Rfree(OFIELDS);
	for(x=0,key=rrpt->keys;x<rrpt->nokeys;++x,++key)
	{
		for(z=0,part=key->part;z<key->numparts;++z,++part)
		{
			offset=0;
			offcount=(-1);
			for(y=0,field=rrpt->fields;y<rrpt->nofields;++y,
				++field)
			{
				if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
				{
					++offcount;
				}
				if(!RDAstrcmp(field->name,part->name))
				{
					if(rpt->engine_type==NRDMyIsamEngine) 
					{
						part->offset=offset;
					} else if(rpt->engine_type==NRDCtreeEngine || rpt->engine_type==NRDLocalCtreeEngine)
					{
						if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
						{
							part->offset=offcount;
						} else part->offset=offset;
					} else part->offset=offset;
					break;
				}
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
						if(rpt->engine_type==NRDMyIsamEngine)
						{
							offset+=field->len+4;
						} else if(rpt->engine_type==NRDInformixIsamEngine)
						{
							offset+=field->len;
						} else offset+=field->len+1;
						break;
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(rpt->engine_type==NRDMyIsamEngine && field->len==0) offset+=4;
							else offset+=field->len;
						break;
					case DECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						offset+=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						offset+=sizeof(short);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						offset+=sizeof(int);
						break;
					case BOOLNS:
					case CHARACTERS:
						offset+=1;
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error field type [%d] is invalid for field [%s].",field->type,field->name);
						break;
				}
			}
		}
	}
	fname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("RPTGEN")+
		RDAstrlen(rrpt->sortfile)+10);
	if(rpt->engine_type!=NRDMyIsamEngine)
	{
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.dat",CURRENTDIRECTORY,"RPTGEN",(rrpt->sortfile!=NULL ? rrpt->sortfile:""));
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.dat",CURRENTDIRECTORY,"RPTGEN",(rrpt->sortfile!=NULL ? rrpt->sortfile:""));
#endif
	} else {
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.MYD",CURRENTDIRECTORY,"RPTGEN",(rrpt->sortfile!=NULL ? rrpt->sortfile:""));
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.MYD",CURRENTDIRECTORY,"RPTGEN",(rrpt->sortfile!=NULL ? rrpt->sortfile:""));
#endif
	}
	if(!access(fname,02))
	{
		DELETENRD("RPTGEN",rrpt->sortfile,rpt->engine_type,rpt->server_name);
	}
	if(fname!=NULL) Rfree(fname);
	fileno=CRTNRD("RPTGEN",rpt->engine_type,rrpt->sortfile,rrpt->nofields,
		rrpt->fields,rrpt->nokeys,rrpt->keys,rpt->server_name);
	if(rrpt->fields!=NULL)
	{
		for(x=0,f=rrpt->fields;x<rrpt->nofields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
		}
		Rfree(rrpt->fields);
		rrpt->nofields=0;
	}
	if(rrpt->keys!=NULL)
	{
		for(x=0,key=rrpt->keys;x<rrpt->nokeys;++x,++key)
		{
			if(key->name!=NULL) Rfree(key->name);
			if(key->part!=NULL)
			{
				for(z=0,part=key->part;z<key->numparts;++z,++part)
				{
					if(part->name!=NULL) Rfree(part->name);
				}
				Rfree(key->part);
			}
		}
		Rfree(rrpt->keys);
		rrpt->nokeys=0;
	}
	return(fileno);
}
short compute_virtual_field(RDArunrpt *rrpt,RDAreport *rpt,char *name,void (*ReportSubFunc)(...),void *vargs)
{
	RDAvirtual *v;
	int x;
	char *temp=NULL;
	char *dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG Compute Virtual Field [%s].",name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rpt->virflds==NULL) 
	{
		prterr("Error Virtual Field's Not Found in RDAreport Structure.");
		return(-1);
	}
	for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
	{
		if(!RDAstrcmp(v->name,name))
		{
			if((!v->when && (v->range<2 || (v->range==2 && v->expression!=NULL && RDAstrlen(v->expression)))) || v->when)
			{
			v->computed=TRUE;
			tempvalue=NULL;
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
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
					if(v->range==2 && !v->when)
					{
						if(rrpt->rsrsrc!=NULL)
						{
						FINDRSCGETSTRING(rrpt->rsrsrc,v->name,&v->value.string_value);
						}
					} else if(v->range==3 && !v->when)
					{
						dashname=adddashes(v->name);
						tempvalue=RDA_GetEnv(dashname);

						if(dashname!=NULL) Rfree(dashname);
						if(!isEMPTY(tempvalue))
						{
							if(v->value.string_value!=NULL) Rfree(v->value.string_value);
							v->value.string_value=stralloc(tempvalue);
							v->dlen=RDAstrlen(v->value.string_value)+1;
						} else if(v->value.string_value!=NULL)
						{
							memset(v->value.string_value,0,v->dlen);
						} else { 
							v->value.string_value=NULL;
							v->dlen=0;
						}
					}
					if(v->expression!=NULL)
					{
						temp=PP_EVALUATEstr(v->expression,ReportGenSubData,vargs);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
					}
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
					if(v->range==2 && !v->when)
					{
						if(rrpt->rsrsrc!=NULL)
						{
						FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
						}
					} else if(v->range==3 && !v->when)
					{
						dashname=adddashes(v->name);
						tempvalue=RDA_GetEnv(dashname);
						if(dashname!=NULL) Rfree(dashname);
						if(v->dlen==0)
						{
							v->value.string_value=Rmalloc(1);
							v->dlen=1;
						}
						if(!isEMPTY(tempvalue))
						{
							if(Ratoi(tempvalue))
							{ 
								*v->value.string_value=TRUE;
							} else { 
								*v->value.string_value=FALSE;
							}
						} else {
							*v->value.string_value=FALSE;
						}
					}
					if(v->expression!=NULL)
					{
						*v->value.string_value=PP_EVALUATEbol(v->expression,ReportSubFunc,vargs);
					}
					break;
				case CHARACTERS:
					if(v->range==2 && !v->when)
					{
						if(rrpt->rsrsrc!=NULL)
						{
						FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
						}
					} else if(v->range==3 && !v->when)
					{
						dashname=adddashes(v->name);
						tempvalue=RDA_GetEnv(dashname);
						if(dashname!=NULL) Rfree(dashname);
						if(v->dlen==0)
						{
							v->value.string_value=Rmalloc(1);
							v->dlen=1;
						}
						if(!isEMPTY(tempvalue))
						{
							*v->value.string_value=*tempvalue;
						} else {
							*v->value.string_value=*tempchar;
						}
					}
					if(v->expression!=NULL)
					{
						*v->value.string_value=PP_EVALUATEbol(v->expression,ReportSubFunc,vargs);
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(v->range==2 && !v->when)
					{
						if(rrpt->rsrsrc!=NULL)
						{
						FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,v->value.float_value);
						}
					} else if(v->range==3 && !v->when)
					{
						dashname=adddashes(v->name);
						tempvalue=RDA_GetEnv(dashname);
						if(dashname!=NULL) Rfree(dashname);
						if(v->dlen==0)
						{
							v->value.float_value=Rmalloc(sizeof(double));
							v->dlen=sizeof(double);
						}
						if(!isEMPTY(tempvalue))
						{
							*v->value.float_value=atof(tempvalue);
						} else {
							*v->value.float_value=0;
						}
					}
					if(v->expression!=NULL)
					{
						*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportSubFunc,vargs);
						if(v->type==DOLLARS || v->type==DOLLARS_NOCENTS ||
							v->type==DOUBLEV ||
							v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(v->range==2 && !v->when)
					{
						if(rrpt->rsrsrc!=NULL)
						{
						FINDRSCGETSHORT(rrpt->rsrsrc,v->name,v->value.short_value);
						}
					} else if(v->range==3 && !v->when)
					{
						dashname=adddashes(v->name);
						tempvalue=RDA_GetEnv(dashname);
						if(dashname!=NULL) Rfree(dashname);
						if(v->dlen==0)
						{
							v->value.short_value=Rmalloc(sizeof(short));
							v->dlen=sizeof(short);
						}
						if(!isEMPTY(tempvalue))
						{
							*v->value.short_value=Ratoi(tempvalue);
						} else {
							*v->value.short_value=0;
						}
					}
					if(v->expression!=NULL)
					{
						*v->value.short_value=PP_EVALUATEsht(v->expression,ReportSubFunc,vargs);
					}
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(v->range==2 && !v->when)
					{
						if(rrpt->rsrsrc!=NULL)
						{
						FINDRSCGETINT(rrpt->rsrsrc,v->name,v->value.integer_value);
						}
					} else if(v->range==3 && !v->when)
					{
						dashname=adddashes(v->name);
						tempvalue=RDA_GetEnv(dashname);
						if(dashname!=NULL) Rfree(dashname);
						if(v->dlen==0)
						{
							v->value.integer_value=Rmalloc(sizeof(int));
							v->dlen=sizeof(int);
						}
						if(!isEMPTY(tempvalue))
						{
							*v->value.integer_value=Ratoi(tempvalue);
						} else {
							*v->value.integer_value=0;
						}
					}
					if(v->expression!=NULL)
					{
					*v->value.integer_value=PP_EVALUATEint(v->expression,ReportSubFunc,vargs);
					}
					break;
				default:
					prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name);
					break;
			}
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diagrptgen_field) { SHOWvirtual(v); }
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(TRUE);
		}
	}
	return(FALSE);
}
void compute_remaining_virtuals(RDArunrpt *rrpt,RDAreport *rpt)
{
	RDAvirtual *v;
	int x;
	HoldReport *h=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG Compute Remaining Virtual Fields.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rpt->virflds!=NULL)
	{
		h=HoldReportNew(rrpt,rpt);
		for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
		{
			if(!v->computed && !v->when)
			{
				compute_virtual_field(rrpt,rpt,v->name,(void (*)(...))ReportGenSubData,h);
			}
		}
		if(h!=NULL) Rfree(h);
	}
}
void SetFieldValue(NRDfield *fld,RDAreport *rpt,RDArunrpt *rrpt)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL,*special=NULL;
	NRDfield *field;
	short fileno=(-1);
	char *temp=NULL,*dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";
	int y;
	RDAvirtual *v;
	HoldReport *h=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen) 
	{ 
		prterr("DIAG SetFieldValue for Field [%s].",fld->name);
		SEENRDFIELD(fld); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(fld->name,"DELETEFLAG")) return;
	modulename=stripmodulename(fld->name);
	if(modulename!=NULL)
	{
	filename=stripfilename(fld->name);
	if(filename==NULL && !RDAstrcmp(modulename,"DELETEFLAG")) 
	{
		Rfree(modulename);
		return;
	}
	if(filename!=NULL && RDAstrcmp(filename,"LOWER") && RDAstrcmp(filename,"UPPER")
		&& RDAstrcmp(filename,"DESCENDING") && RDAstrcmp(filename,"DESCENDING LOWER")
		&& RDAstrcmp(filename,"DESCENDING UPPER"))
	{
		fieldname=stripfieldname(fld->name);
		special=stripfourthname(fld->name);
		fileno=REPORTFILENUMBER(rrpt,modulename,filename,1,FALSE);
		if(fileno!=(-1))
		{
			field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
		} else field=NULL;
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
				case OPTIONALFIELDS:
					if(special==NULL || RDAstrlen(special)<1)
					{
						if(field->data.string_value!=NULL)
						{
							QUICKALLOC(fld->data.string_value,fld->dlen,field->dlen);
							memcpy(fld->data.string_value,field->data.string_value,field->dlen);
							if(fld->dlen>fld->len && fld->len>0)
								fld->data.string_value[fld->len]=0;
						} else if(fld->dlen>0)
						{ 
							memset(fld->data.string_value,0,fld->dlen);
						} else {
							fld->data.string_value=NULL;
							fld->dlen=0;
						}
					} else if(!RDAstrcmp(special,"LOWER"))
					{
						if(field->data.string_value!=NULL)
						{
							QUICKALLOC(fld->data.string_value,fld->dlen,field->dlen);
							memcpy(fld->data.string_value,field->data.string_value,field->dlen);
							if(fld->dlen>fld->len && fld->len>0)
								fld->data.string_value[fld->len]=0;
							stolower(fld->data.string_value);
						} else if(fld->dlen>0)
						{ 
							memset(fld->data.string_value,0,fld->dlen);
						} else {
							fld->data.string_value=NULL;
							fld->dlen=0;
						}
					} else if(!RDAstrcmp(special,"UPPER"))
					{
						if(field->data.string_value!=NULL)
						{
							QUICKALLOC(fld->data.string_value,fld->dlen,field->dlen);
							memcpy(fld->data.string_value,
								field->data.string_value,field->dlen);
							if(fld->dlen>fld->len && fld->len>0)
								fld->data.string_value[fld->len]=0;
							stoupper(fld->data.string_value);
						} else if(fld->dlen>0)
						{ 
							memset(fld->data.string_value,0,fld->dlen);
						} else {
							fld->data.string_value=NULL;
							fld->dlen=0;
						}
					} else if(!RDAstrcmp(special,"DESCENDING"))
					{
						if(fld->data.string_value!=NULL) 
							Rfree(fld->data.string_value);
						fld->data.string_value=
							INVERSEstr(field->data.string_value);
						fld->dlen=RDAstrlen(fld->data.string_value)+1;
					} else if(!RDAstrcmp(special,"DESCENDING LOWER"))
					{
						if(fld->data.string_value!=NULL) 
							Rfree(fld->data.string_value);
						temp=stralloc(field->data.string_value);
						stolower(temp);
						fld->data.string_value=INVERSEstr(temp);
						fld->dlen=RDAstrlen(fld->data.string_value)+1;
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else if(!RDAstrcmp(special,"DESCENDING UPPER"))
					{
						if(fld->data.string_value!=NULL) 
							Rfree(fld->data.string_value);
						temp=stralloc(field->data.string_value);
						stoupper(temp);
						fld->data.string_value=INVERSEstr(temp);
						fld->dlen=RDAstrlen(fld->data.string_value)+1;
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(special==NULL || RDAstrlen(special)<1 ||
						strncmp(special,"DESCENDING",10))
					{
						*fld->data.short_value=*field->data.short_value;
					} else {
						*fld->data.short_value=INVERSEsht(*field->data.short_value);
					}
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(special==NULL || RDAstrlen(special)<1 ||
						strncmp(special,"DESCENDING",10))
					{
						*fld->data.integer_value=*field->data.integer_value;
					} else {
						*fld->data.integer_value=
							INVERSEint(*field->data.integer_value);
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(special==NULL || RDAstrlen(special)<1 ||
						strncmp(special,"DESCENDING",10))
					{
						*fld->data.float_value=*field->data.float_value;
					} else {
						*fld->data.float_value=
							INVERSEdbl(*field->data.float_value);
					}
					break;
				case BOOLNS:
					if(special==NULL || RDAstrlen(special)<1 ||
						strncmp(special,"DESCENDING",10))
					{
						*fld->data.string_value=*field->data.string_value;
					} else {
						if(*field->data.string_value) 
						{
							*fld->data.string_value=TRUE;
						} else *fld->data.string_value=FALSE;
					}
					break;
				case CHARACTERS:
					if(special==NULL || RDAstrlen(special)<1)
					{
						*fld->data.string_value=*field->data.string_value;
					} else if(!RDAstrcmp(special,"LOWER"))
					{
						*fld->data.string_value=tolower(*field->data.string_value);
					} else if(!RDAstrcmp(special,"UPPER"))
					{
						*fld->data.string_value=toupper(*field->data.string_value);
					} else if(!RDAstrcmp(special,"DESCENDING"))
					{
						*fld->data.string_value=
							255-(*field->data.string_value);
					} else if(!RDAstrcmp(special,"DESCENDING LOWER"))
					{
						temp=Rmalloc(1);
						*temp=*field->data.string_value;
						tolower(*temp);
						*fld->data.string_value=255-(*temp);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else if(!RDAstrcmp(special,"DESCENDING UPPER"))
					{
						temp=Rmalloc(1);
						*temp=*field->data.string_value;
						toupper(*temp);
						*fld->data.string_value=255-(*temp);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					}
					break;
				default:
					prterr("Error field type [%d] is invalid for field [%s].",
						field->type,field->name); 
					break;
			}
		}
		if(fieldname!=NULL) Rfree(fieldname);
		fieldname=NULL;
		if(filename!=NULL) Rfree(filename);
		filename=NULL;
		if(special!=NULL) Rfree(special);
		special=NULL;
	} else { /* virtual field */
		if(!RDAstrcmp(modulename,"RECORD COUNTER"))
		{
			*fld->data.float_value=(++rrpt->record_count);
		} else {
			for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
			{
				if(!RDAstrcmp(modulename,v->name)) break;
			}
			if(y<rpt->numvirtuals)
			{
				if(!v->computed)
				{
					h=HoldReportNew(rrpt,rpt);
				}
				tempvalue=NULL;
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
							if(v->range==2 && !v->when)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSTRING(rrpt->rsrsrc,v->name,&v->value.string_value);
								}
							} else if(v->range==3 && !v->when)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(!isEMPTY(tempvalue))
								{
									if(v->value.string_value!=NULL) Rfree(v->value.string_value);
									v->value.string_value=stralloc(tempvalue);
									v->dlen=RDAstrlen(v->value.string_value)+1;
								} else if(v->value.string_value!=NULL)
								{
									memset(v->value.string_value,0,v->dlen);
								} else { 
									v->value.string_value=NULL;
									v->dlen=0;
								}
							}
							if(v->expression!=NULL)
							{
								temp=PP_EVALUATEstr(v->expression,ReportGenSubData,h);
								if(v->value.string_value!=NULL) 
									Rfree(v->value.string_value);
								v->value.string_value=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
							}
							if(v->value.string_value!=NULL)
							{
								v->dlen=RDAstrlen(v->value.string_value)+1;
								if(v->dlen>v->len && v->len>0) 
								{
									v->value.string_value[v->len]=0;
								}
							} else v->dlen=0;
						}
						if(filename==NULL || RDAstrlen(filename)<1)
						{
							if(v->value.string_value!=NULL)
							{
								QUICKALLOC(fld->data.string_value,fld->dlen,v->dlen);
								memcpy(fld->data.string_value,v->value.string_value,v->dlen);
								if(fld->dlen>fld->len && fld->len>0)
									fld->data.string_value[fld->len]=0;
							} else if(fld->dlen>0)
							{ 
								memset(fld->data.string_value,0,fld->dlen);
							} else {
								fld->data.string_value=NULL;
								fld->dlen=0;
							}
						} else if(!RDAstrcmp(filename,"LOWER"))
						{
							if(v->value.string_value!=NULL)
							{
								QUICKALLOC(fld->data.string_value,fld->dlen,v->dlen);
								memcpy(fld->data.string_value,v->value.string_value,v->dlen);
								if(fld->dlen>fld->len && fld->len>0)
									fld->data.string_value[fld->len]=0;
								stolower(fld->data.string_value);
							} else if(fld->dlen>0)
							{ 
								memset(fld->data.string_value,0,fld->dlen);
							} else {
								fld->data.string_value=NULL;
								fld->dlen=0;
							}
						} else if(!RDAstrcmp(filename,"UPPER"))
						{
							if(v->value.string_value!=NULL)
							{
								QUICKALLOC(fld->data.string_value,fld->dlen,v->dlen);
								memcpy(fld->data.string_value,v->value.string_value,v->dlen);
								if(fld->dlen>fld->len && fld->len>0)
									fld->data.string_value[fld->len]=0;
								stoupper(fld->data.string_value);
							} else if(fld->dlen>0)
							{ 
								memset(fld->data.string_value,0,fld->dlen);
							} else {
								fld->data.string_value=NULL;
								fld->dlen=0;
							}
						} else if(!RDAstrcmp(filename,"DESCENDING"))
						{
							if(fld->data.string_value!=NULL) 
								Rfree(fld->data.string_value);
							fld->data.string_value=
								INVERSEstr(v->value.string_value);
						} else if(!RDAstrcmp(filename,"DESCENDING LOWER"))
						{
							if(fld->data.string_value!=NULL) 
								Rfree(fld->data.string_value);
							temp=stralloc(v->value.string_value);
							stolower(temp);
							fld->data.string_value=INVERSEstr(temp);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} else if(!RDAstrcmp(filename,"DESCENDING UPPER"))
						{
							if(fld->data.string_value!=NULL) 
								Rfree(fld->data.string_value);
							temp=stralloc(v->value.string_value);
							stoupper(temp);
							fld->data.string_value=INVERSEstr(temp);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(!v->computed)
						{
							v->computed=TRUE;
							if(v->range==2 && !v->when)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSHORT(rrpt->rsrsrc,v->name,v->value.short_value);
								}
							} else if(v->range==3 && !v->when)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.short_value=Rmalloc(sizeof(short));
									v->dlen=sizeof(short);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.short_value=Ratoi(tempvalue);
								} else {
									*v->value.short_value=0;
								}
							}
							if(v->expression!=NULL)
							{
							*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData,h);
							}
						}
						if(filename==NULL || RDAstrlen(filename)<1 ||
							strncmp(filename,"DESCENDING",10))
						{
							*fld->data.short_value=*v->value.short_value;
						} else {
							*fld->data.short_value=
								INVERSEsht(*v->value.short_value);
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(!v->computed)
						{
							v->computed=TRUE;
							if(v->range==2 && !v->when)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETINT(rrpt->rsrsrc,v->name,v->value.integer_value);
								}
							} else if(v->range==3 && !v->when)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.integer_value=Rmalloc(sizeof(int));
									v->dlen=sizeof(int);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.integer_value=Ratoi(tempvalue);
								} else {
									*v->value.integer_value=0;
								}
							}
							if(v->expression!=NULL)
							{
							*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData,h);
							}
						}
						if(filename==NULL || RDAstrlen(filename)<1 ||
							strncmp(filename,"DESCENDING",10))
						{
							*fld->data.integer_value=*v->value.integer_value;
						} else {
							*fld->data.integer_value=
								INVERSEint(*v->value.integer_value);
						}
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
							if(v->range==2 && !v->when)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,v->value.float_value);
								}
							} else if(v->range==3 && !v->when)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.float_value=Rmalloc(sizeof(double));
									v->dlen=sizeof(double);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.float_value=atof(tempvalue);
								} else {
									*v->value.float_value=0;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData,h);
								if(v->type==DOLLARS || v->type==DOLLARS_NOCENTS ||
									v->type==DOUBLEV ||
									v->type==SDOUBLEV)
								{
									*v->value.float_value=round(*v->value.float_value);
								}
							}
						}
						if(filename==NULL || RDAstrlen(filename)<1 ||
							strncmp(filename,"DESCENDING",10))
						{
							*fld->data.float_value=*v->value.float_value;
						} else {
							*fld->data.float_value=
								INVERSEdbl(*v->value.float_value);
						}
						break;
					case BOOLNS:
						if(!v->computed)
						{
							v->computed=TRUE;
							if(v->range==2 && !v->when)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
								}
							} else if(v->range==3 && !v->when)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								if(!isEMPTY(tempvalue))
								{
									if(Ratoi(tempvalue))
									{
										*v->value.string_value=TRUE;
									} else {
										*v->value.string_value=FALSE;
									}
								} else {
									*v->value.string_value=FALSE;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
							}
						}
						if(filename!=NULL || RDAstrlen(filename)<1 || 
							strncmp(filename,"DESCENDING",10))
						{
							*fld->data.string_value=*v->value.string_value;
						} else {
							if(*v->value.string_value)
							{
								*fld->data.string_value=FALSE;
							} else *fld->data.string_value=TRUE;
						}
						break;
					case CHARACTERS:
						if(!v->computed)
						{
							v->computed=TRUE;
							if(v->range==2 && !v->when)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
								}
							} else if(v->range==3 && !v->when)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.string_value=*tempvalue;
								} else {
									*v->value.string_value=*tempchar;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
							}
						}
						if(filename==NULL || RDAstrlen(filename)<1)
						{
							*fld->data.string_value=*v->value.string_value;
						} else if(!RDAstrcmp(filename,"LOWER"))
						{
							*fld->data.string_value=
								tolower(*v->value.string_value);
						} else if(!RDAstrcmp(filename,"UPPER"))
						{
							*fld->data.string_value=
								toupper(*v->value.string_value);
						} else if(!RDAstrcmp(filename,"DESCENDING"))
						{
							*fld->data.string_value=
								255-(*v->value.string_value);
						} else if(!RDAstrcmp(filename,"DESCEND LOWER"))
						{
							temp=Rmalloc(1);
							*temp=*v->value.string_value;
							tolower(*temp);
							*fld->data.string_value=255-(*temp);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} else if(!RDAstrcmp(filename,"DESCEND UPPER"))
						{
							temp=Rmalloc(1);
							*temp=*v->value.string_value;
							toupper(*temp);
							*fld->data.string_value=255-(*temp);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						}
						break;
					default:
						prterr("Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
						break;
				}
				if(h!=NULL) Rfree(h);
			} else {
				prterr("Error Field [%s] not found.",fld->name);
			}
		}
	}
	if(filename!=NULL) Rfree(filename);
	Rfree(modulename);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG SetFieldValue Returning Field [%s].",fld->name);
		SEENRDFIELD(fld); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
short writesortrecord(RDArunrpt *rrpt,RDAreport *rpt,RUNfile *prim)
{
	int x,return_value=FALSE;
	NRDfield *f;
	short numflds=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG writesortrecord.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	ZERNRD(rrpt->sortno);
	numflds=NUMFLDS(rrpt->sortno);
	for(x=0;x<numflds;++x) 
	{
		f=FLDNUM(rrpt->sortno,x);
		if(f!=NULL) 
		{
			SetFieldValue(f,rpt,rrpt);
		} else {
			prterr("NRDFIELD f is NULL.");TRACE;
		}
	}
	return_value=WRTNRD(rrpt->sortno,0,NULL);
	prim->wrote_first=TRUE;
	return(!return_value);
}
short evaluate_records(RDArunrpt *rrpt,RDAreport *rpt,RUNfile *prim)
{
	short f;
	HoldReport *h=NULL;
	RDAvirtual *v;
	int x;
	char *temp=NULL,test_select=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG evaluate records.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	h=HoldReportNew(rrpt,rpt);
	if(rpt->virflds!=NULL)
	{
		for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
		{
			if(!RDAstrcmp(v->name,"ERROR DESCRIPTION")) break;
		}
		if(x<rpt->numvirtuals)
		{
			if(!v->when && !v->computed) 
			{
				v->computed=TRUE;
				temp=PP_EVALUATEstr(v->expression,ReportGenSubData,h);
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
			}
			if(!v->when && v->value.string_value!=NULL && v->dlen>0)
			{
				addAPPlib(rrpt->errorlist,v->value.string_value);
			}
		}
	}
	if(rpt->select!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_sltrec)
		{
			prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Enabled.");
			diageval=TRUE;
			diagrptgen=TRUE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		test_select=PP_EVALUATEbol(rpt->select,ReportGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_sltrec)
		{
			prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
			diageval=FALSE;
			diagrptgen=FALSE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(test_select)
		{
			if(rpt->range_screen)
			{
				if(rrpt->rsrsrc!=NULL)
				{
					if(rpt->virflds!=NULL)
					{
						for(f=0,v=rpt->virflds;f<rpt->numvirtuals;++f,++v)
						{
							if(!v->when && v->range==1)
							{
								if(ADVtestvirtualrangersrc(v,rrpt->RScreen,(void (*)(...))ReportGenSubData,h))
								{
									if(h!=NULL) Rfree(h);
#ifdef USE_RDA_DIAGNOSTICS
									if(diagrptgen || diagrptgen_field)
									{
										prterr("DIAG ADVtestvirtualrangersrc rejected record.");
									}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
									return(FALSE);
								}
							}
						}
					}
				}
			}
			if(h!=NULL) Rfree(h);
			return(writesortrecord(rrpt,rpt,prim));
		} else {
			if(h!=NULL) Rfree(h);
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diagrptgen_field)
			{
				prterr("DIAG Select Records Rejected record.");
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(FALSE);
		}
	} else {
		if(rpt->range_screen)
		{
			if(rrpt->rsrsrc!=NULL)
			{
				if(rpt->virflds!=NULL)
				{
					for(f=0,v=rpt->virflds;f<rpt->numvirtuals;++f,++v)
					{
						if(!v->when && v->range==1)
						{
							if(ADVtestvirtualrangersrc(v,rrpt->RScreen,(void (*)(...))ReportGenSubData,h))
							{
								if(h!=NULL) Rfree(h);
#ifdef USE_RDA_DIAGNOSTICS
								if(diagrptgen || diagrptgen_field)
								{
									prterr("DIAG ADVtestvirtualrangersrc rejected record.");
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								return(FALSE);
							}
						}
					}
				}
			}
		}
		if(h!=NULL) Rfree(h);
		return(writesortrecord(rrpt,rpt,prim));
	}
}
short process_records(RDArunrpt *rrpt,RDAreport *rpt,RUNfile *prim,HoldReport *h)
{
	short retx=0,supdetail=FALSE;
	RDAvirtual *v;
	int x,COMPUTE_WRITECOUNT=0,f=0;
	RUNfile *t=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG Process records.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	prim->wrote_first=FALSE;
	if(prim->child!=NULL)
	{
		for(x=0,t=prim->child;x<prim->num;++x,++t)
		{
			if(t->type==16) ++supdetail;
		}
	}
	if(!isEMPTY(prim->wc_expression) && prim->wc_eval==TRUE)
	{
		COMPUTE_WRITECOUNT=PP_EVALUATEint(prim->wc_expression,ReportGenSubData,h);
		if(COMPUTE_WRITECOUNT<1)
		{
			COMPUTE_WRITECOUNT=1;
		}
	} else { 
		if(prim->num_writes<1) 
		{
			COMPUTE_WRITECOUNT=1;
		} else {
			COMPUTE_WRITECOUNT=prim->num_writes;
		}
	}
	for(rrpt->write_count=1;rrpt->write_count<(COMPUTE_WRITECOUNT+1);
		++rrpt->write_count)
	{
		if(abort_diagnostic) return(-1);
		for(f=0,v=rpt->virflds;f<rpt->numvirtuals;++f,++v)
		{
			v->computed=FALSE;
		}
		if((rrpt->write_count==2) && prim->wrote_first==FALSE &&
			prim->wc_skip==TRUE) 
		{
			break;
		}
		retx=READsubfiles(prim,rrpt,rpt);
		if(supdetail==0 || rpt->write_all_detail_levels==TRUE)
		{
			if(!retx)
			{
				if(evaluate_records(rrpt,rpt,prim))
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diagrptgen)
					{
						prterr("DIAG evaluate_records returned TRUE");
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(rpt->no_wc_diag==FALSE)
					{
					if(rrpt->diagrsrc!=NULL) 
					{
						if(!abort_diagnostic)
						{
							update_diagnostic(rrpt->diagrsrc,TRUE);
						}
					}
					}
				} else {
#ifdef USE_RDA_DIAGNOSTICS
					if(diagrptgen)
					{
						prterr("DIAG evaluate_records returned FALSE");
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(rpt->no_wc_diag==FALSE)
					{
					if(rrpt->diagrsrc!=NULL) 
					{
						update_diagnostic(rrpt->diagrsrc,FALSE);
					}
					}
				}
			} else if(retx==(-1)) /* errors encountered */
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen || diagrptgen_field)
				{
					prterr("DIAG Error found evaluating the following record displayed in the SEENRDRECORD diagnostic.");
					SEENRDRECORD(prim->fileno);
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				++rrpt->errors_encountered;
				if(rpt->no_wc_diag==FALSE)
				{
				if(rrpt->diagrsrc!=NULL) 
				{
					update_diagnostic(rrpt->diagrsrc,FALSE);
				}
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(rpt->stop_on_error)
				{
					if(diagrptgen || diagrptgen_field)
					{
						prterr("DIAG Report Aborting Upon Detection of the First Error.");
						return(-1);
					}
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			} else if(retx==1) /* skipping it */
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen || diagrptgen_field)
				{
					prterr("DIAG Skipped the following record displayed in the SEENRDRECORD diagnostic because Sub-Files couldn't be read.");
					SEENRDRECORD(prim->fileno);
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				if(rpt->no_wc_diag==FALSE)
				{
				if(rrpt->diagrsrc!=NULL) 
				{
					update_diagnostic(rrpt->diagrsrc,FALSE);
				}
				}
			}
			for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
			{
				v->computed=FALSE;
			}
		} else {
			if(rrpt->diagrsrc!=NULL) 
			{
				if(!abort_diagnostic)
				{
					update_diagnostic(rrpt->diagrsrc,(retx==FALSE ? TRUE:FALSE));
				}
			}
		}
	}
	if(supdetail==0 || rpt->write_all_detail_levels==TRUE)
	{
		if(rpt->no_wc_diag==FALSE)
		{
			for(++rrpt->write_count;rrpt->write_count<(COMPUTE_WRITECOUNT+1);
				++rrpt->write_count)
			{
				if(rrpt->diagrsrc!=NULL) 
				{
					update_diagnostic(rrpt->diagrsrc,FALSE);
				}
			}
		} else {
			if(rrpt->diagrsrc!=NULL) 
			{
				update_diagnostic(rrpt->diagrsrc,(prim->wrote_first ? TRUE:FALSE));
			}
		}
	}
	return(0);
}
#define SetRUNfileKeyParts(a,b,c,d,e,f)	xSetRUNfileKeyParts(a,b,c,d,e,f,__LINE__,__FILE__)
void xSetRUNfileKeyParts(RDAsearch *s,RUNfile *sub,RUNfile *runfile,RDArunrpt *rrpt,RDAreport *rpt,HoldReport *h,int line,char *file)
{
	RDAconfld *confld;
	RDAvirtual *v;
	NRDfield *fld,*f;
	int x=0,z=0;
	char *temp=NULL,*filename=NULL,*fieldname=NULL,*fred=NULL;
	char *dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";
	short tmpfileno=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG SetRUNfileKeyParts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(s==NULL) return;
	if(s->confld==NULL) return;
	ZERNRD(sub->fileno);
	for(z=0,confld=s->confld;z<s->num;++z,++confld)
	{
		if(isEMPTY(confld->conname) || isEMPTY(confld->fldname)) 
		{
			return;
		}
		fld=FLDNRDsec(sub->fileno,confld->fldname,TRUE,FALSE,FALSE);
		if(fld!=NULL)
		{
			filename=stripmodulename(confld->conname);
			fieldname=stripfilename(confld->conname);
			if(fieldname!=NULL && RDAstrlen(fieldname)>0)
			{
				f=NULL;
				if(!RDAstrcmp(filename,runfile->filename))
				{
					f=FLDNRDsec(runfile->fileno,fieldname,
						TRUE,FALSE,FALSE);
				} else {
					tmpfileno=CONNECTFILENUMBER(runfile,filename);
					if(tmpfileno!=(-1))
					{
						f=FLDNRDsec(tmpfileno,fieldname,TRUE,FALSE,
							FALSE);
					}
				}
				if(f!=NULL)
				{
					switch(fld->type)
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
							if(!isEMPTY(f->data.string_value))
							{
								QUICKALLOC(fld->data.string_value,fld->dlen,f->dlen);
								memcpy(fld->data.string_value,f->data.string_value,f->dlen);
								if(fld->dlen>fld->len && 
									fld->len>0)
									fld->data.string_value[fld->len]=0;
							} else if(fld->dlen>0)
							{ 
								memset(fld->data.string_value,0,fld->dlen);
							} else {
								fld->data.string_value=NULL;
								fld->dlen=0;
							}
							break;
						case SHORTV:
						case SSHORTV:
							*fld->data.short_value=
								*f->data.short_value;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							*fld->data.integer_value=
								*f->data.integer_value;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							*fld->data.float_value=
								*f->data.float_value;
							break;
						case BOOLNS:
						case CHARACTERS:
							*fld->data.string_value=
								*f->data.string_value;
							break;
						default:
							prterr("Error field type [%d] is invalid for field [%s].",f->type,f->name); 
							break;
					}
				} else {
					prterr("Error Field [%s] in file [%d] [%s] [%s] not found.",fieldname,runfile->fileno,MODULENAME(runfile->fileno),FILENAME(runfile->fileno));
				}
			} else {
				fred=stripmodulename(confld->conname);
				for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
				{
					if(!RDAstrcmp(fred,v->name)) break;
				}
				if(x<rpt->numvirtuals)
				{	
					tempvalue=NULL;
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
							if((fld->type!=SCROLLEDTEXT)&&(fld->type!=VARIABLETEXT)&&(fld->type!=DATES)&&
								(fld->type!=TIMES)&&(fld->type!=SOCSECNUM)&&(fld->type!=PLAINTEXT)&&
								(fld->type!=PHONE)&&(fld->type!=CUSTOMTYPE)&&(fld->type!=ZIPCODE)&&
								(fld->type!=EXPENDITURE)&&(fld->type!=REVENUE)&&
								(fld->type!=BALANCESHEET)&&(fld->type!=BEGINNINGBALANCE)&&
								(fld->type!=OPTIONALFIELDS))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the e Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed)
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETSTRING(rrpt->rsrsrc,v->name,&v->value.string_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(!isEMPTY(tempvalue))
									{
										if(v->value.string_value!=NULL) Rfree(v->value.string_value);
										v->value.string_value=stralloc(tempvalue);
										v->dlen=RDAstrlen(v->value.string_value)+1;
									} else if(v->value.string_value!=NULL)
									{
										memset(v->value.string_value,0,v->dlen);
									} else { 
										v->value.string_value=NULL;
										v->dlen=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
								temp=PP_EVALUATEstr(v->expression,ReportGenSubData,h);
								if(v->value.string_value!=NULL) 
									Rfree(v->value.string_value);
								v->value.string_value=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
								}
								if(!isEMPTY(v->value.string_value))
								{
									v->dlen=RDAstrlen(v->value.string_value)+1;
									if(v->dlen>v->len && 
										v->len>0) 
									{
										v->value.string_value[v->len]=0;
									}
								} else v->dlen=0;
							}
							if(!isEMPTY(v->value.string_value))
							{
								QUICKALLOC(fld->data.string_value,fld->dlen,v->dlen);
								memcpy(fld->data.string_value,v->value.string_value,v->dlen);
								if(fld->dlen>fld->len && fld->len>0)
									fld->data.string_value[fld->len]=0;
							} else if(fld->dlen>0)
							{ 
								memset(fld->data.string_value,0,fld->dlen);
							} else {
								fld->data.string_value=NULL;
								fld->dlen=0;
							}
							break;
						case SHORTV:
						case SSHORTV:
							if((fld->type!=SHORTV)&&(fld->type!=SHORTV))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETSHORT(rrpt->rsrsrc,v->name,v->value.short_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.short_value=Rmalloc(sizeof(short));
										v->dlen=sizeof(short);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.short_value=Ratoi(tempvalue);
									} else {
										*v->value.short_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData,h);
								}
							}
							*fld->data.short_value=
								*v->value.short_value;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if((fld->type!=SCROLLEDLIST)&&(fld->type!=LONGV)&&(fld->type!=SLONGV))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETINT(rrpt->rsrsrc,v->name,v->value.integer_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.integer_value=Rmalloc(sizeof(int));
										v->dlen=sizeof(int);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.integer_value=Ratoi(tempvalue);
									} else {
										*v->value.integer_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
								*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData,h);
								}
							}
							*fld->data.integer_value=
								*v->value.integer_value;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if((fld->type!=DOLLARS)&&(fld->type!=DOLLARS_NOCENTS)&&(fld->type!=DECIMALV)&&
							(fld->type!=DOUBLEV)&&(fld->type!=SDOUBLEV)&&(fld->type!=SDECIMALV))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,v->value.float_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.float_value=Rmalloc(sizeof(double));
										v->dlen=sizeof(double);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.float_value=atof(tempvalue);
									} else {
										*v->value.float_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
								*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData,h);
								}
								if(v->type==DOLLARS || 
									v->type==DOLLARS_NOCENTS ||
									v->type==DOUBLEV ||
									v->type==SDOUBLEV)
								{
									*v->value.float_value=round(*v->value.float_value);
								}
							}
							*fld->data.float_value=
								*v->value.float_value;
							break;
						case BOOLNS:
							if(fld->type!=BOOLNS)
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										if(Ratoi(tempvalue))
										{
											*v->value.string_value=TRUE;
										} else {
											*v->value.string_value=FALSE;
										}
									} else {
										*v->value.string_value=FALSE;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
								}
							}
							*fld->data.string_value=
								*v->value.string_value;
							break;
						case CHARACTERS:
							if((fld->type!=BOOLNS)&&(fld->type!=CHARACTERS))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.string_value=*tempvalue;
									} else {
										*v->value.string_value=*tempchar;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
								}
							}
							*fld->data.string_value=
								*v->value.string_value;
							break;
						default:
							prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
							break;
					}
				} else {
					prterr("Error Field [%s] not found and cannot be established.",confld->conname);
				}
				if(fred!=NULL) Rfree(fred);
				fred=NULL;
			}
			if(filename!=NULL) Rfree(filename);
			filename=NULL;
			if(fieldname!=NULL) Rfree(fieldname);
			fieldname=NULL;
		}
	}

}
#define TestRUNfileKeyParts(a,b,c,d,e,f)	xTestRUNfileKeyParts(a,b,c,d,e,f,__LINE__,__FILE__)
short xTestRUNfileKeyParts(RDAsearch *s,RUNfile *sub,RUNfile *runfile,RDArunrpt *rrpt,RDAreport *rpt,HoldReport *h,int line,char *file)
{
	RDAconfld *confld;
	RDAvirtual *v;
	NRDfield *fld,*f;
	int x=0,z=0;
	char *temp=NULL,*filename=NULL,*fieldname=NULL,*fred=NULL;
	char *dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";
	short tmpfileno=0,retval=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG TestUNfileKeyParts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(s==NULL)
	{
		prterr("Error no RDAsearch structure....");
	}
	for(z=0,confld=s->confld;z<s->num;++z,++confld)
	{
		if(isEMPTY(confld->conname) || isEMPTY(confld->fldname)) return(retval);
		fld=FLDNRDsec(sub->fileno,confld->fldname,TRUE,FALSE,FALSE);
		if(fld!=NULL)
		{
			filename=stripmodulename(confld->conname);
			fieldname=stripfilename(confld->conname);
			if(fieldname!=NULL && RDAstrlen(fieldname)>0)
			{
				f=NULL;
				if(!RDAstrcmp(filename,runfile->filename))
				{
					f=FLDNRDsec(runfile->fileno,fieldname,
						TRUE,FALSE,FALSE);
				} else {
					tmpfileno=CONNECTFILENUMBER(runfile,filename);
					if(tmpfileno!=(-1))
					{
						f=FLDNRDsec(tmpfileno,fieldname,TRUE,FALSE,
							FALSE);
					}
				}
				if(f!=NULL)
				{
					switch(fld->type)
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
							retval=RDAstrcmp(f->data.string_value,fld->data.string_value);
							if(s->key_fastbreak)
							{
								if(retval>(-1)) retval=0;
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.short_value!=*f->data.short_value) retval=(-1);
							} else {
								if(*fld->data.short_value<*f->data.short_value) retval=(-1);
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.integer_value!=*f->data.integer_value) retval=(-1);
							} else {
								if(*fld->data.integer_value<*f->data.integer_value) retval=(-1);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.float_value!=*f->data.float_value) retval=(-1);
							} else {
								if(*fld->data.float_value<*f->data.float_value) retval=(-1);
							}
							break;
						case BOOLNS:
							if(*fld->data.string_value!=*v->value.string_value) retval=(-1);
							break;
						case CHARACTERS:
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.string_value!=*f->data.string_value) retval=(-1);
							} else {
								if(*fld->data.string_value<*f->data.string_value) retval=(-1);
							}
							break;
						default:
							prterr("Error field type [%d] is invalid for field [%s].",f->type,f->name); 
							break;
					}
				} else {
					prterr("Error Field [%s] in file [%d] [%s] [%s] not found.",fieldname,runfile->fileno,MODULENAME(runfile->fileno),FILENAME(runfile->fileno));
				}
			} else {
				fred=stripmodulename(confld->conname);
				for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
				{
					if(!RDAstrcmp(fred,v->name)) break;
				}
				if(x<rpt->numvirtuals)
				{
					tempvalue=NULL;
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
							if((fld->type!=SCROLLEDTEXT)&&(fld->type!=VARIABLETEXT)&&(fld->type!=DATES)&&
								(fld->type!=TIMES)&&(fld->type!=SOCSECNUM)&&(fld->type!=PLAINTEXT)&&
								(fld->type!=PHONE)&&(fld->type!=CUSTOMTYPE)&&(fld->type!=ZIPCODE)&&
								(fld->type!=EXPENDITURE)&&(fld->type!=REVENUE)&&
								(fld->type!=BALANCESHEET)&&(fld->type!=BEGINNINGBALANCE)&&
								(fld->type!=OPTIONALFIELDS))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the e Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed)
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETSTRING(rrpt->rsrsrc,v->name,&v->value.string_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(!isEMPTY(tempvalue))
									{
										if(v->value.string_value!=NULL) Rfree(v->value.string_value);
										v->value.string_value=stralloc(tempvalue);
										v->dlen=RDAstrlen(v->value.string_value)+1;
									} else if(v->value.string_value!=NULL)
									{
										memset(v->value.string_value,0,v->dlen);
									} else { 
										v->value.string_value=NULL;
										v->dlen=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
								temp=PP_EVALUATEstr(v->expression,ReportGenSubData,h);
								if(v->value.string_value!=NULL) 
									Rfree(v->value.string_value);
								v->value.string_value=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
								}
								if(!isEMPTY(v->value.string_value))
								{
									v->dlen=RDAstrlen(v->value.string_value)+1;
									if(v->dlen>v->len && 
										v->len>0) 
									{
										v->value.string_value[v->len]=0;
									}
								} else v->dlen=0;
							}
							retval=RDAstrcmp(v->value.string_value,fld->data.string_value);
							if(s->key_fastbreak)
							{
								if(retval>(-1)) retval=0;
							}
							break;
						case SHORTV:
						case SSHORTV:
							if((fld->type!=SHORTV)&&(fld->type!=SHORTV))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETSHORT(rrpt->rsrsrc,v->name,v->value.short_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.short_value=Rmalloc(sizeof(short));
										v->dlen=sizeof(short);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.short_value=Ratoi(tempvalue);
									} else {
										*v->value.short_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
									*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData,h);
								}
							}
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.short_value!=*v->value.short_value) retval=(-1);;
							} else {
								if(*fld->data.short_value<*v->value.short_value) retval=(-1);;
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if((fld->type!=SCROLLEDLIST)&&(fld->type!=LONGV)&&(fld->type!=SLONGV))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETINT(rrpt->rsrsrc,v->name,v->value.integer_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.integer_value=Rmalloc(sizeof(int));
										v->dlen=sizeof(int);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.integer_value=Ratoi(tempvalue);
									} else {
										*v->value.integer_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
								*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData,h);
								}
							}
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.integer_value!=*v->value.integer_value) retval=(-1);
							} else {
								if(*fld->data.integer_value<*v->value.integer_value) retval=(-1);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if((fld->type!=DOLLARS)&&(fld->type!=DOLLARS_NOCENTS)&&(fld->type!=DECIMALV)&&
							(fld->type!=DOUBLEV)&&(fld->type!=SDOUBLEV)&&(fld->type!=SDECIMALV))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,v->value.float_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.float_value=Rmalloc(sizeof(double));
										v->dlen=sizeof(double);
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.float_value=atof(tempvalue);
									} else {
										*v->value.float_value=0;
									}
								}
								if(!isEMPTY(v->expression))
								{
								*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData,h);
								}
								if(v->type==DOLLARS || 
									v->type==DOLLARS_NOCENTS ||
									v->type==DOUBLEV ||
									v->type==SDOUBLEV)
								{
									*v->value.float_value=round(*v->value.float_value);
								}
							}
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.float_value!=*v->value.float_value) retval=(-1);;
							} else {
								if(*fld->data.float_value<*v->value.float_value) retval=(-1);;
							}
							break;
						case BOOLNS:
							if((fld->type!=BOOLNS)&&(fld->type!=CHARACTERS))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										if(Ratoi(tempvalue))
										{
											*v->value.string_value=TRUE;
										} else {
											*v->value.string_value=FALSE;
										}
									} else {
										*v->value.string_value=FALSE;
									}
								}
								if(!isEMPTY(v->expression))
								{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
								}
							}
							if(*fld->data.string_value!=*v->value.string_value) retval=(-1);
							break;
						case CHARACTERS:
							if((fld->type!=BOOLNS)&&(fld->type!=CHARACTERS))
							{
								prterr("Error Field [%s] is not a valid Field Type to copy over to the File Field [%s].",(v->name==NULL?"":v->name),(confld->conname==NULL?"":confld->conname));
							}
							if(!v->computed) 
							{
								v->computed=TRUE;
								if(v->range==2 && !v->when)
								{
									if(rrpt->rsrsrc!=NULL)
									{
										FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
									}
								} else if(v->range==3 && !v->when)
								{
									dashname=adddashes(v->name);
									tempvalue=RDA_GetEnv(dashname);
									if(dashname!=NULL) Rfree(dashname);
									if(v->dlen==0)
									{
										v->value.string_value=Rmalloc(1);
										v->dlen=1;
									}
									if(!isEMPTY(tempvalue))
									{
										*v->value.string_value=*tempvalue;
									} else {
										*v->value.string_value=*tempchar;
									}
								}
								if(!isEMPTY(v->expression))
								{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
								}
							}
							if(s->key_fastbreak==FALSE)
							{
								if(*fld->data.string_value!=*v->value.string_value) retval=(-1);
							} else {
								if(*fld->data.string_value<*v->value.string_value) retval=(-1);
							}
							break;
						default:
							prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
							break;
					}
				} else {
					prterr("Error Field [%s] not found and cannot be established.",confld->conname);
				}
				if(fred!=NULL) Rfree(fred);
				fred=NULL;
			}
			if(filename!=NULL) Rfree(filename);
			filename=NULL;
			if(fieldname!=NULL) Rfree(fieldname);
			fieldname=NULL;
		}
	}
	return(retval);
}
static short ProcessSupportingDetail(RUNfile *runfile,HoldReport *h)
{
	int x=0,occ_count=0,f=0;
	RDAvirtual *v=NULL;
	short testrangeval=FALSE,testexpval=FALSE,key_fastbreak=FALSE;
	short pef=0,retx=0,supdetail=FALSE;
	RDAsearch *s=NULL;
	RDAreport *rpt=NULL;
	RDArunrpt *rrpt=NULL;
	RUNfile *r=NULL;
	int COMPUTE_WRITECOUNT=0;

	if(runfile->child!=NULL)
	{
		for(x=0,r=runfile->child;x<runfile->num;++x,++r)
		{
			if(r->type==16) ++supdetail;
		}
	}
	if(h!=NULL)
	{
		rpt=h->rpt;
		rrpt=h->rrpt;
	} else {
		prterr("Error NO HoldReport Structure!!!!!");
		return(-1);
	}
	++rrpt->detail_count;
	rrpt->cur_runfile=runfile;
	occ_count=0;
	for(f=0,s=rpt->search;f<rpt->numsearchs;++f,++s)
	{
		if(s->ctype==0) ++occ_count;
		if(occ_count==rrpt->primary_file_no
			&& !RDAstrcmp(runfile->module,s->module)
			&& !RDAstrcmp(runfile->filename,s->filename) && s->ctype==16)

		{ 
			break;
		}
	} 
	if(f>=rpt->numsearchs)
	{
		prterr("Error RDAsearch not found... How did you get here?");
		s=NULL;
	}
	pef=ADVGTENRDsec(runfile->fileno,runfile->keyno,ReportGenSubData,h);
	if(!pef)
	{
		pef=TestRUNfileKeyParts(s,runfile,runfile->parent,h->rrpt,h->rpt,h);
	}
	while(!pef)
	{
		if(rrpt->rsrsrc!=NULL && runfile->range)
		{
			testrangeval=testfilerangersrc(runfile->fileno,rrpt->RScreen);
		} else testrangeval=FALSE;
		if(!isEMPTY(runfile->wc_expression) && runfile->wc_eval==TRUE)
		{
			COMPUTE_WRITECOUNT=PP_EVALUATEint(runfile->wc_expression,ReportGenSubData,h);
			if(COMPUTE_WRITECOUNT<1)
			{
				COMPUTE_WRITECOUNT=1;
			}
		} else { 
			if(runfile->num_writes<1) 
			{
				COMPUTE_WRITECOUNT=1;
			} else {
				COMPUTE_WRITECOUNT=runfile->num_writes;
			}
		}
		key_fastbreak=FALSE;
		for(runfile->write_count=1;runfile->write_count<(COMPUTE_WRITECOUNT+1);++runfile->write_count)
		{
			for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
			{
				v->computed=FALSE;
			}
			if((runfile->write_count==2) && 
				runfile->wrote_first==FALSE &&
	 			runfile->wc_skip==TRUE) 
			{
				break;
			}
			if(!isEMPTY(runfile->expression))
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen_sltrec)
				{
					prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Enabled.");
					diageval=TRUE;
					diagrptgen=TRUE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				if(s->key_fastbreak==FALSE)
				{
					testexpval=PP_EVALUATEbol(runfile->expression,ReportGenSubData,h);
				} else {
					testexpval=PP_EVALUATEsht(runfile->expression,ReportGenSubData,h);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen_sltrec)
				{
					prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
					diageval=FALSE;
					diagrptgen=FALSE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			} else { 
				testexpval=TRUE;
			}
			key_fastbreak=FALSE;
			if(s->key_fastbreak==TRUE && testexpval==(-1)) key_fastbreak=TRUE;
			if(!testrangeval && testexpval>0)
			{
				retx=READsubfiles(runfile,h->rrpt,h->rpt);
				if(supdetail==0 || rpt->write_all_detail_levels==TRUE)
				{
					if(!retx)
					{
						if(evaluate_records(rrpt,rpt,runfile))
						{
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen)
							{
								prterr("DIAG evaluate_records returned TRUE");
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(rpt->no_wc_diag==FALSE)
							{
							if(rrpt->diagrsrc!=NULL) 
							{
								if(!abort_diagnostic)
								{
									update_diagnostic(rrpt->diagrsrc,TRUE);
								}
							}
							}
						} else {
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen)
							{
								prterr("DIAG evaluate_records returned FALSE");
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(rpt->no_wc_diag==FALSE)
							{
							if(rrpt->diagrsrc!=NULL) 
							{
								update_diagnostic(rrpt->diagrsrc,FALSE);
							}
							}
						}
					} else if(retx==(-1)) /* errors encountered */
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen || diagrptgen_field)
						{
							prterr("DIAG Error found evaluating the following record displayed in the SEENRDRECORD diagnostic.");
							SEENRDRECORD(runfile->fileno);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						++rrpt->errors_encountered;
						if(rpt->no_wc_diag==FALSE)
						{
						if(rrpt->diagrsrc!=NULL) 
						{
							update_diagnostic(rrpt->diagrsrc,FALSE);
						}
						}
						if(rpt->stop_on_error)
						{
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("DIAG Report Aborting Upon Detection of the First Error.");
							}	
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							--rrpt->detail_count;
							return(-1);
						}
					} else if(retx==1) /* skipping it */
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen || diagrptgen_field)
						{
							prterr("DIAG Skipped the following record displayed in the SEENRDRECORD diagnostic because Sub-Files couldn't be read.");
							SEENRDRECORD(runfile->fileno);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(rpt->no_wc_diag==FALSE)
						{
						if(rrpt->diagrsrc!=NULL) 
						{
							update_diagnostic(rrpt->diagrsrc,FALSE);
						}
						}
					}
					for(x=0,v=rpt->virflds;x<rpt->numvirtuals;++x,++v)
					{
						v->computed=FALSE;
					}
				} 
			}	
		}
		if(key_fastbreak==TRUE) break;
		pef=ADVNXTNRDsec(runfile->fileno,runfile->keyno,ReportGenSubData,h);
		if(!pef)
		{
			pef=TestRUNfileKeyParts(s,runfile,runfile->parent,h->rrpt,h->rpt,h);
		}
	}
	--rrpt->detail_count;
	ZERNRD(runfile->fileno);
	return(0);
}
short READsubfiles(RUNfile *runfile,RDArunrpt *rrpt,RDAreport *rpt)
{
	int y,x;
	RUNfile *sub,*sub1;
	short retx=0,count=0,occurrence=1,occur_count=0;
	RDAsearch *s;
	char *errordesc=NULL,*temp=NULL;
	char *mname,*fname,*kname=NULL;
	HoldReport *h=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG READsubfiles.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	h=HoldReportNew(rrpt,rpt);
	if(runfile->num>0 && runfile->child!=NULL)
	{
		for(y=0,sub=runfile->child;y<runfile->num;++y,++sub)
		{
			occurrence=1;
			for(x=0,sub1=runfile->child;x<y;++x,++sub1)
			{
				if(!RDAstrcmp(sub->filename,sub1->filename) &&
					!RDAstrcmp(sub->module,sub1->module)) 
				{
					occurrence+=1;
				}
			}
			count=0;
			occur_count=0;
			for(x=0,s=rpt->search;x<rpt->numsearchs;++x,++s)
			{
				if(s->ctype==0) ++count;
				if(count==rrpt->primary_file_no)
				{
					if(!RDAstrcmp(sub->filename,s->filename) &&
						!RDAstrcmp(sub->module,s->module) &&
						!RDAstrcmp(runfile->filename,
						s->cfilename) && 
						!RDAstrcmp(runfile->module,
						s->cmodule))
					{
						++occur_count;
					}
					if(occur_count==occurrence)
					{
						break;
					}
				}
			}
			if(x<rpt->numsearchs)
			{
				SetRUNfileKeyParts(s,sub,runfile,rrpt,rpt,h);
			}
			switch(sub->type)
			{
				case 17: /* Not EQUAL To */
					if(!ADVEQLNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
					{
						ZERNRD(sub->fileno);
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen || diagrptgen_field)
						{
							prterr("Error couldn't Connect to File [%s] [%s], skipping detail entry.",MODULENAME(sub->fileno),FILENAME(sub->fileno));
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(h!=NULL) Rfree(h);
						h=NULL;
						return(1);
					}
					break;
				case 16: /* Supporting Details */
					ProcessSupportingDetail(sub,h);
					ZERNRD(sub->fileno);
					if(h!=NULL) Rfree(h);
					h=NULL;
					return(0);
				default:
					prterr("Error Invalid Supporting file Connect Type.");
					break;
				case 1: /* Joined To */
				case 4: /* Greater Than and Equal To */
				case 7: /* Less Than and Equal To */
				case 10: /* Greater Than */
				case 13: /* Less Than */
					if(sub->type==1)
					{
						if(ADVEQLNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h)) 
							ZERNRD(sub->fileno);
					} else if(sub->type==4)
					{
						if(ADVGTENRDsec(sub->fileno,sub->keyno,ReportGenSubData,h)) 
							ZERNRD(sub->fileno);
					} else if(sub->type==7)
					{
						if(ADVLTENRDsec(sub->fileno,sub->keyno,ReportGenSubData,h)) 
							ZERNRD(sub->fileno);
					} else if(sub->type==10)
					{
						if(ADVNXTNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h)) 
							ZERNRD(sub->fileno);
					} else if(sub->type==13)
					{
						if(ADVPRVNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h)) 
							ZERNRD(sub->fileno);
					}
					if(rrpt->rsrsrc!=NULL)
					{
					if(sub->range)
					{
						if(testfilerangersrc(sub->fileno,rrpt->RScreen)) 
						{
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("DIAG testfilerangersrc rejected record.");
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(TRUE);
						}
					}
					}
					if(sub->expression!=NULL)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Enabled.");
							diageval=TRUE;
							diagrptgen=TRUE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(!PP_EVALUATEbol(sub->expression,ReportGenSubData,h))
						{
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("DIAG Supporting File Expression rejected record.");
							}
							if(diagrptgen_sltrec)
							{
								prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
								diageval=FALSE;
								diagrptgen=FALSE;
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(TRUE);
						}
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
							diageval=FALSE;
							diagrptgen=FALSE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					}
					retx=READsubfiles(sub,rrpt,rpt);
					if(retx) 
					{
						if(h!=NULL) Rfree(h);
						h=NULL;
						return(0);
					}
					break;
				case 2: /* Joined To Skip */
				case 5: /* Greater Than and Equal To or Skip */
				case 8: /* Less Than and Equal To or Skip */
				case 11: /* Greater Than or Skip */
				case 14: /* Less Than or Skip */
					if(sub->type==2)
					{
						if(ADVEQLNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							ZERNRD(sub->fileno);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("Error couldn't Connect to File [%s] [%s], skipping detail entry.",MODULENAME(sub->fileno),FILENAME(sub->fileno));
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(1);
						}
					} else if(sub->type==5)
					{
						if(ADVGTENRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							ZERNRD(sub->fileno);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("Error couldn't Connect to File [%s] [%s], skipping detail entry.",MODULENAME(sub->fileno),FILENAME(sub->fileno));
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(1);
						}
					} else if(sub->type==8)
					{
						if(ADVLTENRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							ZERNRD(sub->fileno);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("Error couldn't Connect to File [%s] [%s], skipping detail entry.",MODULENAME(sub->fileno),FILENAME(sub->fileno));
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(1);
						}
					} else if(sub->type==11)
					{
						if(ADVNXTNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							ZERNRD(sub->fileno);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("Error couldn't Connect to File [%s] [%s], skipping detail entry.",MODULENAME(sub->fileno),FILENAME(sub->fileno));
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(1);
						}
					} else if(sub->type==14)
					{
						if(ADVPRVNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							ZERNRD(sub->fileno);
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("Error couldn't Connect to File [%s] [%s], skipping detail entry.",MODULENAME(sub->fileno),FILENAME(sub->fileno));
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(1);
						}
					}
					if(rrpt->rsrsrc!=NULL)
					{
						if(sub->range)
						{
							if(testfilerangersrc(sub->fileno,rrpt->RScreen)) 
							{
#ifdef USE_RDA_DIAGNOSTICS
								if(diagrptgen || diagrptgen_field)
								{
									prterr("DIAG testfilerangersrc rejected record.");
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								if(h!=NULL) Rfree(h);
								h=NULL;
								return(TRUE);
							}
						}
					}
					if(sub->expression!=NULL)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Enabled.");
							diageval=TRUE;
							diagrptgen=TRUE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(!PP_EVALUATEbol(sub->expression,ReportGenSubData,h))
						{
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("DIAG Supporting File Expression rejected record.");
							}
							if(diagrptgen_sltrec)
							{
								prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
								diageval=FALSE;
								diagrptgen=FALSE;
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(TRUE);
						}
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
							diageval=FALSE;
							diagrptgen=FALSE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					}
					retx=READsubfiles(sub,rrpt,rpt);
					if(retx) 
					{
						if(h!=NULL) Rfree(h);
						h=NULL;
						return(retx);
					}
					break;
				case 3: /* Joined to Error */
				case 6: /* Greater Than and Equal To or Error */
				case 9: /* Less Than and Equal To or Error */
				case 12: /* Greater Than or Error */
				case 15: /* Less Than or Error */
					if(sub->type==3)
					{
						if(ADVEQLNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							mname=MODULENAME(sub->fileno);
							fname=FILENAME(sub->fileno);
							kname=KEYNAME(sub->fileno,sub->keyno);
							temp=getkeyinfo(sub->fileno,sub->keyno);
							errordesc=Rmalloc(RDAstrlen(mname)+RDAstrlen(fname)+RDAstrlen(kname)+RDAstrlen(temp)+59);
							if(errordesc!=NULL)
							{
								sprintf(errordesc,"Error Encountered Looking for [%s] in File [%s][%s] using Key [%s]",temp,mname,fname,kname);
								addAPPlib(rrpt->errorlist,errordesc);
								prterr(errordesc);
								Rfree(errordesc);
							}
							if(temp!=NULL) Rfree(temp);
							ZERNRD(sub->fileno);
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(-1);
						}
					} else if(sub->type==6)
					{
						if(ADVGTENRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							mname=MODULENAME(sub->fileno);
							fname=FILENAME(sub->fileno);
							kname=KEYNAME(sub->fileno,sub->keyno);
							temp=getkeyinfo(sub->fileno,sub->keyno);
							errordesc=Rmalloc(RDAstrlen(mname)+RDAstrlen(fname)+RDAstrlen(kname)+RDAstrlen(temp)+59);
							if(errordesc!=NULL)
							{
								sprintf(errordesc,"Error Encountered Looking for [%s] in File [%s][%s] using Key [%s]",temp,mname,fname,kname);
								addAPPlib(rrpt->errorlist,errordesc);
								prterr(errordesc);
								Rfree(errordesc);
							}
							if(temp!=NULL) Rfree(temp);
							ZERNRD(sub->fileno);
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(-1);
						}
					} else if(sub->type==9)
					{
						if(ADVLTENRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							mname=MODULENAME(sub->fileno);
							fname=FILENAME(sub->fileno);
							kname=KEYNAME(sub->fileno,sub->keyno);
							temp=getkeyinfo(sub->fileno,sub->keyno);
							errordesc=Rmalloc(RDAstrlen(mname)+RDAstrlen(fname)+RDAstrlen(kname)+RDAstrlen(temp)+59);
							if(errordesc!=NULL)
							{
								sprintf(errordesc,"Error Encountered Looking for [%s] in File [%s][%s] using Key [%s]",temp,mname,fname,kname);
								addAPPlib(rrpt->errorlist,errordesc);
								prterr(errordesc);
								Rfree(errordesc);
							}
							if(temp!=NULL) Rfree(temp);
							ZERNRD(sub->fileno);
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(-1);
						}
					} else if(sub->type==12)
					{
						if(ADVNXTNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							mname=MODULENAME(sub->fileno);
							fname=FILENAME(sub->fileno);
							kname=KEYNAME(sub->fileno,sub->keyno);
							temp=getkeyinfo(sub->fileno,sub->keyno);
							errordesc=Rmalloc(RDAstrlen(mname)+RDAstrlen(fname)+RDAstrlen(kname)+RDAstrlen(temp)+59);
							if(errordesc!=NULL)
							{
								sprintf(errordesc,"Error Encountered Looking for [%s] in File [%s][%s] using Key [%s]",temp,mname,fname,kname);
								addAPPlib(rrpt->errorlist,errordesc);
								prterr(errordesc);
								Rfree(errordesc);
							}
							if(temp!=NULL) Rfree(temp);
							ZERNRD(sub->fileno);
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(-1);
						}
					} else if(sub->type==15)
					{
						if(ADVPRVNRDsec(sub->fileno,sub->keyno,ReportGenSubData,h))
						{
							mname=MODULENAME(sub->fileno);
							fname=FILENAME(sub->fileno);
							kname=KEYNAME(sub->fileno,sub->keyno);
							temp=getkeyinfo(sub->fileno,sub->keyno);
							errordesc=Rmalloc(RDAstrlen(mname)+RDAstrlen(fname)+RDAstrlen(kname)+RDAstrlen(temp)+59);
							if(errordesc!=NULL)
							{
								sprintf(errordesc,"Error Encountered Looking for [%s] in File [%s][%s] using Key [%s]",temp,mname,fname,kname);
								addAPPlib(rrpt->errorlist,errordesc);
								prterr(errordesc);
								Rfree(errordesc);
							}
							if(temp!=NULL) Rfree(temp);
							ZERNRD(sub->fileno);
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(-1);
						}
					}
					if(rrpt->rsrsrc!=NULL)
					{
						if(sub->range)
						{
							if(testfilerangersrc(sub->fileno,rrpt->RScreen)) 
							{
#ifdef USE_RDA_DIAGNOSTICS
								if(diagrptgen || diagrptgen_field)
								{
									prterr("DIAG testfilerangersrc rejected record.");
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								if(h!=NULL) Rfree(h);
								h=NULL;
								return(TRUE);
							}
						}
					}
					if(sub->expression!=NULL)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Enabled.");
							diageval=TRUE;
							diagrptgen=TRUE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(!PP_EVALUATEbol(sub->expression,
							ReportGenSubData,h))
						{
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen || diagrptgen_field)
							{
								prterr("DIAG Supporting File Expression rejected record.");
							}
							if(diagrptgen_sltrec)
							{
								prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
								diageval=FALSE;
								diagrptgen=FALSE;
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(TRUE);
						}
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
							diageval=FALSE;
							diagrptgen=FALSE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					}
					retx=READsubfiles(sub,rrpt,rpt);
					if(retx) 
					{
						if(h!=NULL) Rfree(h);
						h=NULL;
						return(retx);
					}
					break;
			}
		}
	}
	if(h!=NULL) Rfree(h);
	h=NULL;
	return(0);
}
short UseRDAsearchs(RDAsearch *s)
{
	RDAconfld *c;
	int x;

	if(s->num<1 || s->confld==NULL) return(FALSE);
	for(x=0,c=s->confld;x<s->num;++x,++c)
	{
		if(!isEMPTY(c->fldname) && !isEMPTY(c->conname)) return(TRUE);
	}
	return(FALSE);
}
static short writeonlyREADRecords(RDArunrpt *rrpt,RDAreport *rpt,int line,char *file)
{
	int x,total_records=0,occ_count=0;
	RUNfile *prim;
	RDAvirtual *v;
	char *temp=NULL,skip_detail=FALSE;
	short f;
	ReportFile *r;
	HoldReport *h=NULL;
	RDAsearch *s=NULL;
	int doprocexcepts=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG writeonlyREADRecords at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rrpt->numprims<=0 || rrpt->prims==NULL) 
	{
		prterr("Error Report must have at least 1 primary file.");
		return(-1);
	}
	if(rrpt->rsrsrc!=NULL)
	{
		if(IsScreenDisplayed(rrpt->rsrsrc)) 
		{
			readallwidgets(rrpt->rsrsrc);
			killwindow(rrpt->rsrsrc);
			--rrpt->screen_count;
			NULL_RSCS(rrpt->rsrsrc);
		}
		if((rpt->display->device>=0 && rpt->display->device<=2) || rpt->display->device==5)
		{
			FINDRSCGETCHAR(rrpt->rsrsrc,"RANGE ON PAGE NUMBER",&rrpt->range_page);
			FINDRSCGETINT(rrpt->rsrsrc,"FROM PAGE NUMBER",&rrpt->from_page);
			FINDRSCGETINT(rrpt->rsrsrc,"TO PAGE NUMBER",&rrpt->to_page);
		}
		if(rpt->process_exceptions==TRUE)
		{
			FINDRSCGETINT(rrpt->rsrsrc,"PROCESS EXCEPTIONS TYPES",&doprocexcepts);
			if(doprocexcepts==0) rrpt->do_process_exceptions=0;
			else if(doprocexcepts==1) rrpt->do_process_exceptions=1;
			else rrpt->do_process_exceptions=0;
		}
		rrpt->RScreen=RangeScreenNew(rrpt->rsrsrc);
		for(f=0,r=rrpt->files;f<rrpt->numfiles;++f,++r)
		{
			if(r->range)
			{
				ReadRangeScreen(r->fileno,rrpt->RScreen);
			}
		}
		if(rpt->virflds!=NULL)
		{
			for(f=0,v=rpt->virflds;f<rpt->numvirtuals;++f,++v)
			{
				if(!v->when && v->range==1)
				{
					ReadVirtualRangeStruct(v,rrpt->RScreen);
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen_sltall)
	{
		prterr("DIAG Report Generator and Expression Parsing Diagnostics Enabled.");
		diagrptgen=TRUE;
		diageval=TRUE;
	} else if(diagrptgen_select)
	{
		prterr("DIAG Report Generator Diagnostics Enabled.");
		diagrptgen=TRUE;
	} else if(diagrptgen_slteval)
	{
		prterr("DIAG Expression Parsing Diagnostics Enabled.");
		diageval=TRUE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	h=HoldReportNew(rrpt,rpt);
	rrpt->primary_file_no=0;
	abort_diagnostic=FALSE;
	for(x=0,prim=rrpt->prims;x<rrpt->numprims;++x,++prim)
	{
		rrpt->cur_runfile=prim;
		++rrpt->detail_count;
		if(abort_diagnostic) 
		{
			if(h!=NULL) Rfree(h);
			h=NULL;
			--rrpt->detail_count;
			return(-1);
		}
		rrpt->primary_file_no=(x+1);
		if(!isEMPTY(prim->skip_detail))
		{
			skip_detail=PP_EVALUATEbol(prim->skip_detail,ReportGenSubData,h);
		} else skip_detail=FALSE;
		if(!skip_detail)
		{
			occ_count=0;
			for(f=0,s=rpt->search;f<rpt->numsearchs;++f,++s)
			{
				if(s->ctype==0) ++occ_count;
				if(occ_count==(x+1)
					&& !RDAstrcmp(prim->module,s->module)
					&& !RDAstrcmp(prim->filename,s->filename))
				{ 
					break;
				}
			} 
			if(f>=rpt->numsearchs)
			{
				prterr("Error RDAsearch not found... How did you get here?");
				s=NULL;
			}
	
			ZERNRD(prim->fileno);
			rrpt->diagrsrc=NULL;
#ifdef XXXX
#ifdef WIN32
#ifdef __GTK_H__
			if(rpt->range_screen)
			{
#endif
#endif 
			if(rpt->diag_breakable<2)
			{
/*
				if(!prim->wc_eval)
				{
					total_records=((int)SIZNRD(prim->fileno))*prim->num_writes;
				} else {
					total_records=PP_EVALUATEint(prim->wc_expression,ReportGenSubData,h);
				}
*/
				total_records=PP_EVALUATEint(prim->wc_expression,ReportGenSubData,h);
				temp=Rmalloc(RDAstrlen(prim->module)+RDAstrlen(prim->filename)+RDAstrlen(rpt->name)+36);
				sprintf(temp,"Selecting Detail From %s %s in %s Report.",prim->module,prim->filename,rpt->name);
				rrpt->diagrsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN",temp,(rpt->diag_breakable ? stop_diagnostic:NULL),total_records);
				if(temp!=NULL) Rfree(temp);
			} else rrpt->diagrsrc=NULL;
#ifdef WIN32
#ifdef __GTK_H__
			} else rrpt->diagrsrc=NULL;
#endif
#endif
#endif /* XXXX */
			if(rrpt->diagrsrc!=NULL)
			{
				if(ADVmakescrn(rrpt->diagrsrc,TRUE))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that the screen is available in the module.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
					prterr("Error Cannot Create Diagnostic Screen.");
				} else {
					temp=Rmalloc(RDAstrlen(rrpt->module)+RDAstrlen(rpt->name)+16);
					sprintf(temp,"%s %s REPORT FRONT",rrpt->module,rpt->name);
					SETTITLE(rrpt->diagrsrc,temp);
					if(temp!=NULL) Rfree(temp);
					ForceWindowUpdate(rrpt->diagrsrc);
				}
			}
			if(process_records(rrpt,rpt,prim,h)==(-1)) 
			{
#ifdef XXXX
				if(rrpt->diagrsrc!=NULL)
				{
					if(!abort_diagnostic)
						killwindow(rrpt->diagrsrc);
					free_rsrc(rrpt->diagrsrc);
					rrpt->diagrsrc=NULL;
				}
#endif /* XXXX */
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen_sltall)
				{
					prterr("DIAG Report Generator and Expression Parsing Diagnostics Disabled.");
					diagrptgen=FALSE;
					diageval=FALSE;
				} else if(diagrptgen_select)
				{
					prterr("DIAG Report Generator Diagnostics Disabled.");
					diagrptgen=TRUE;
				} else if(diagrptgen_slteval)
				{
					prterr("DIAG Expression Parsing Diagnostics Disabled.");
					diageval=FALSE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				if(h!=NULL) Rfree(h);
				h=NULL;
				--rrpt->detail_count;
				return(-1);
			}
#ifdef XXXX
			if(rrpt->diagrsrc!=NULL)
			{
				if(!abort_diagnostic) killwindow(rrpt->diagrsrc);
				free_rsrc(rrpt->diagrsrc);
				rrpt->diagrsrc=NULL;
			}
#endif /* XXXX */
		}
		--rrpt->detail_count;
	}
	if(h!=NULL) Rfree(h);
	h=NULL;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen_sltall)
	{
		prterr("DIAG Report Generator and Expression Parsing Diagnostics Disabled.");
		diagrptgen=FALSE;
		diageval=FALSE;
	} if(diagrptgen_select)
	{
		prterr("DIAG Report Generator Diagnostics Disabled.");
		diagrptgen=FALSE;
	} else if(diagrptgen_slteval)
	{
		prterr("DIAG Expression Parsing Diagnostics Disabled.");
		diageval=FALSE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(0);
}
static short regularREADRecords(RDArunrpt *rrpt,RDAreport *rpt,int line,char *file)
{
	int x,y,total_records=0,occ_count=0;
	RUNfile *prim;
	short pef=0;
	RDAvirtual *v;
	char *temp=NULL,skip_detail=FALSE;
	short f,testrangeval=FALSE,testexpval=TRUE,key_range=FALSE;
	short doRDAsearchs=FALSE;
	ReportFile *r;
	HoldReport *h=NULL;
	RDAsearch *s=NULL;
	int doprocexcepts=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG READRecords at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rrpt->numprims<=0 || rrpt->prims==NULL) 
	{
		prterr("Error Report must have at least 1 primary file.");
		return(-1);
	}
	if(rrpt->rsrsrc!=NULL)
	{
		if(IsScreenDisplayed(rrpt->rsrsrc)) 
		{
			readallwidgets(rrpt->rsrsrc);
			killwindow(rrpt->rsrsrc);
			--rrpt->screen_count;
			NULL_RSCS(rrpt->rsrsrc);
		}
		if((rpt->display->device>=0 && rpt->display->device<=2) || rpt->display->device==5)
		{
			FINDRSCGETCHAR(rrpt->rsrsrc,"RANGE ON PAGE NUMBER",&rrpt->range_page);
			FINDRSCGETINT(rrpt->rsrsrc,"FROM PAGE NUMBER",&rrpt->from_page);
			FINDRSCGETINT(rrpt->rsrsrc,"TO PAGE NUMBER",&rrpt->to_page);
		}
		if(rpt->process_exceptions==TRUE)
		{
			FINDRSCGETINT(rrpt->rsrsrc,"PROCESS EXCEPTIONS TYPES",&doprocexcepts);
			if(doprocexcepts==0) rrpt->do_process_exceptions=0;
			else if(doprocexcepts==1) rrpt->do_process_exceptions=1;
			else rrpt->do_process_exceptions=0;
		}
		rrpt->RScreen=RangeScreenNew(rrpt->rsrsrc);
		for(f=0,r=rrpt->files;f<rrpt->numfiles;++f,++r)
		{
			if(r->range)
			{
				ReadRangeScreen(r->fileno,rrpt->RScreen);
			}
		}
		if(rpt->virflds!=NULL)
		{
			for(f=0,v=rpt->virflds;f<rpt->numvirtuals;++f,++v)
			{
				if(!v->when && v->range==1)
				{
					ReadVirtualRangeStruct(v,rrpt->RScreen);
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen_sltall)
	{
		prterr("DIAG Report Generator and Expression Parsing Diagnostics Enabled.");
		diagrptgen=TRUE;
		diageval=TRUE;
	} else if(diagrptgen_select)
	{
		prterr("DIAG Report Generator Diagnostics Enabled.");
		diagrptgen=TRUE;
	} else if(diagrptgen_slteval)
	{
		prterr("DIAG Expression Parsing Diagnostics Enabled.");
		diageval=TRUE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	h=HoldReportNew(rrpt,rpt);
	rrpt->primary_file_no=0;
	abort_diagnostic=FALSE;
	for(x=0,prim=rrpt->prims;x<rrpt->numprims;++x,++prim)
	{
		rrpt->cur_runfile=prim;
		++rrpt->detail_count;
		if(abort_diagnostic) 
		{
			if(h!=NULL) Rfree(h);
			h=NULL;
			return(-1);
		}
		rrpt->primary_file_no=(x+1);
		if(!isEMPTY(prim->skip_detail))
		{
			skip_detail=PP_EVALUATEbol(prim->skip_detail,ReportGenSubData,h);
		} else skip_detail=FALSE;
		if(!skip_detail)
		{
			occ_count=0;
			for(f=0,s=rpt->search;f<rpt->numsearchs;++f,++s)
			{
				if(s->ctype==0) ++occ_count;
				if(occ_count==(x+1)
					&& !RDAstrcmp(prim->module,s->module)
					&& !RDAstrcmp(prim->filename,s->filename))
				{ 
					break;
				}
			} 
			if(f>=rpt->numsearchs)
			{
				prterr("Error RDAsearch not found... How did you get here?");
				s=NULL;
			}
			ZERNRD(prim->fileno);
#ifdef XXXX
#ifdef WIN32
#ifdef __GTK_H__
			if(rpt->range_screen)
			{
#endif
#endif
			if(rpt->diag_breakable<2)
			{
				if(rpt->no_wc_diag==FALSE)
				{
					if(prim->wc_eval)
					{
						total_records=((int)SIZNRD(prim->fileno))*(PP_EVALUATEint(prim->wc_expression,ReportGenSubData,h));
					} else if(prim->num_writes>0)
					{
						total_records=((int)SIZNRD(prim->fileno))*prim->num_writes;
					} else { 
						total_records=((int)SIZNRD(prim->fileno));
					}
				} else {
					total_records=((int)SIZNRD(prim->fileno));
				}
				temp=Rmalloc(RDAstrlen(prim->module)+RDAstrlen(prim->filename)+RDAstrlen(rpt->name)+36);
				sprintf(temp,"Selecting Detail From %s %s in %s Report.",prim->module,prim->filename,rpt->name);
				rrpt->diagrsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN",temp,(rpt->diag_breakable ? stop_diagnostic:NULL),total_records);
				if(temp!=NULL) Rfree(temp);
			} else rrpt->diagrsrc=NULL;
#ifdef WIN32
#ifdef __GTK_H__
			} else rrpt->diagrsrc=NULL;
#endif
#endif
#endif /* XXXX */
			if(rrpt->diagrsrc!=NULL)
			{
				if(ADVmakescrn(rrpt->diagrsrc,TRUE))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that the screen is available in the module.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
					prterr("Error Cannot Create Diagnostic Screen.");
				} else {
					temp=Rmalloc(RDAstrlen(rrpt->module)+RDAstrlen(rpt->name)+16);
					sprintf(temp,"%s %s REPORT FRONT",rrpt->module,rpt->name);
					SETTITLE(rrpt->diagrsrc,temp);
					if(temp!=NULL) Rfree(temp);
					ForceWindowUpdate(rrpt->diagrsrc);
				}
			}
			if(s!=NULL)
			{
				doRDAsearchs=UseRDAsearchs(s);
				if(doRDAsearchs)
				{
					SetRUNfileKeyParts(s,prim,prim,rrpt,rpt,h);
					key_range=FALSE;
					if(rrpt->rsrsrc!=NULL && prim->range)
					{
						key_range=setfilekeyFROMrangersrc(prim->fileno,prim->keyno,rrpt->RScreen);
					}
					pef=ADVGTENRDsec(prim->fileno,prim->keyno,ReportGenSubData,h);
				} else {
					if(rrpt->rsrsrc!=NULL && prim->range)
					{
						key_range=setfilekeyFROMrangersrc(prim->fileno,prim->keyno,rrpt->RScreen);
						if(key_range)
						{
							pef=ADVGTENRDsec(prim->fileno,prim->keyno,ReportGenSubData,h);
						} else {
							pef=ADVBEGNRDsec(prim->fileno,ReportGenSubData,h);
						}
					} else  {
						pef=ADVBEGNRDsec(prim->fileno,ReportGenSubData,h);
					}
				}
			} else {
				if(rrpt->rsrsrc!=NULL && prim->range)
				{
					key_range=setfilekeyFROMrangersrc(prim->fileno,prim->keyno,rrpt->RScreen);
					if(key_range)
					{
						pef=ADVGTENRDsec(prim->fileno,prim->keyno,ReportGenSubData,h);
					} else {
						pef=ADVBEGNRDsec(prim->fileno,ReportGenSubData,h);
					}
				} else {
					pef=ADVBEGNRDsec(prim->fileno,ReportGenSubData,h);
				}
			}
			while(!pef)
			{
				if(abort_diagnostic) break;
				for(f=0,v=rpt->virflds;f<rpt->numvirtuals;++f,++v)
				{
					v->computed=FALSE;
				}
				++rrpt->total_count;
				if(rrpt->rsrsrc!=NULL && prim->range)
				{
					testrangeval=testfilerangersrc(prim->fileno,rrpt->RScreen);
					if(!isEMPTY(prim->expression))
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Enabled.");
							diageval=TRUE;
							diagrptgen=TRUE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(s->key_fastbreak==FALSE)
						{
							testexpval=PP_EVALUATEbol(prim->expression,ReportGenSubData,h);
						} else {
							testexpval=PP_EVALUATEsht(prim->expression,ReportGenSubData,h);
						}
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
							diageval=FALSE;
							diagrptgen=FALSE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					} else { 
						testexpval=TRUE;
					}
					if(s->key_fastbreak==TRUE && testexpval==(-1)) break;
					if(!testrangeval && testexpval>0) 
					{
						if(process_records(rrpt,rpt,prim,h)==(-1)) 
						{
#ifdef XXXX
							if(rrpt->diagrsrc!=NULL)
							{
								if(!abort_diagnostic)
									killwindow(rrpt->diagrsrc);
								free_rsrc(rrpt->diagrsrc);
								rrpt->diagrsrc=NULL;
							}
#endif /* XXXX */
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen_sltall)
							{
								prterr("DIAG Report Generator and Expression Parsing Diagnostics Disabled.");
								diagrptgen=FALSE;
								diageval=FALSE;
							} else if(diagrptgen_select)
							{
								prterr("DIAG Report Generator Diagnostics Disabled.");
								diagrptgen=TRUE;
							} else if(diagrptgen_slteval)
							{
								prterr("DIAG Expression Parsing Diagnostics Disabled.");
								diageval=FALSE;
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(-1);
						}
					} else {
						if(rrpt->diagrsrc!=NULL) 
						{
							if(IsScreenDisplayed(rrpt->diagrsrc)) 
							{
								for(y=0;y<prim->num_writes;++y) 
								{
									if(abort_diagnostic) break;
									update_diagnostic(rrpt->diagrsrc,FALSE);
								}
							}
						}
					}
				} else {
					if(!isEMPTY(prim->expression))
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Enabled.");
							diageval=TRUE;
							diagrptgen=TRUE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						testexpval=PP_EVALUATEbol(prim->expression,
							ReportGenSubData,h);
#ifdef USE_RDA_DIAGNOSTICS
						if(diagrptgen_sltrec)
						{
							prterr("DIAG Select Records Report Generator and Expression Parsing Diagnostics Disabled.");
							diageval=FALSE;
							diagrptgen=FALSE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					} else { 
						testexpval=TRUE;
					}
					if(testexpval)
					{
						if(process_records(rrpt,rpt,prim,h)==(-1)) 
						{
#ifdef XXXX
							if(rrpt->diagrsrc!=NULL)
							{
								if(!abort_diagnostic) if(IsScreenDisplayed(rrpt->diagrsrc)) killwindow(rrpt->diagrsrc);
								free_rsrc(rrpt->diagrsrc);
								rrpt->diagrsrc=NULL;
							}
#endif /* XXXX */
#ifdef USE_RDA_DIAGNOSTICS
							if(diagrptgen_sltall)
							{
								prterr("DIAG Report Generator and Expression Parsing Diagnostics Disabled.");
								diagrptgen=FALSE;
								diageval=FALSE;
							} else if(diagrptgen_select)
							{
								prterr("DIAG Report Generator Diagnostics Disabled.");
								diagrptgen=TRUE;
							} else if(diagrptgen_slteval)
							{
								prterr("DIAG Expression Parsing Diagnostics Disabled.");
								diageval=FALSE;
							}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
							if(h!=NULL) Rfree(h);
							h=NULL;
							return(-1);
						}
					} else {
						if(rrpt->diagrsrc!=NULL) 
						{
							for(y=0;y<prim->num_writes;++y) 
							{
								if(abort_diagnostic) break;
								update_diagnostic(rrpt->diagrsrc,FALSE);
							}
						}
					}
				}
				if(s!=NULL)
				{
					if(doRDAsearchs)
					{
						pef=ADVNXTNRDsec(prim->fileno,prim->keyno,ReportGenSubData,h);
						if(!pef)
						{
							pef=TestRUNfileKeyParts(s,prim,prim,rrpt,rpt,h);
	
							if(key_range)
							{
								pef=testfilekeyTOrangersrc(prim->fileno,prim->keyno,rrpt->RScreen);
							}
						}
					} else {
						if(key_range)
						{
							pef=ADVNXTNRDsec(prim->fileno,prim->keyno,ReportGenSubData,h);
							if(!pef)
							{
								pef=testfilekeyTOrangersrc(prim->fileno,prim->keyno,rrpt->RScreen);
							}
						} else {
							pef=ADVSEQNRDsec(prim->fileno,ReportGenSubData,h);
						}
					}
				} else {
					if(key_range)
					{
						pef=ADVNXTNRDsec(prim->fileno,prim->keyno,ReportGenSubData,h);
						if(!pef)
						{
							pef=testfilekeyTOrangersrc(prim->fileno,prim->keyno,rrpt->RScreen);
						}
					} else {
						pef=ADVSEQNRDsec(prim->fileno,ReportGenSubData,h);
					}
				}
			}
#ifdef XXXX
			if(rrpt->diagrsrc!=NULL)
			{
				if(!abort_diagnostic) killwindow(rrpt->diagrsrc);
				free_rsrc(rrpt->diagrsrc);
				rrpt->diagrsrc=NULL;
			}
#endif /* XXXX */
		}
		--rrpt->detail_count;
	}
	if(h!=NULL) Rfree(h);
	h=NULL;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen_sltall)
	{
		prterr("DIAG Report Generator and Expression Parsing Diagnostics Disabled.");
		diagrptgen=FALSE;
		diageval=FALSE;
	} if(diagrptgen_select)
	{
		prterr("DIAG Report Generator Diagnostics Disabled.");
		diagrptgen=FALSE;
	} else if(diagrptgen_slteval)
	{
		prterr("DIAG Expression Parsing Diagnostics Disabled.");
		diageval=FALSE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(0);
}
short xREADRecords(RDArunrpt *rrpt,RDAreport *rpt,int line,char *file)
{
	int f=0;
	RDAvirtual *v=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG READRecords called from line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rpt->virflds!=NULL)
	{
		for(f=0,v=rpt->virflds;f<rpt->numvirtuals;++f,++v)
		{
			v->computed=FALSE;
		}
	}
	if(rpt->writecount_only==TRUE)
	{
		return(writeonlyREADRecords(rrpt,rpt,line,file));
	} else {
		return(regularREADRecords(rrpt,rpt,line,file));
	}
}
RDArunrpt *xRDArunrptNEW(char *module,char *sortserver,char *sortfile,
	short APPmainLoop,short exitstatus,void (*quitfunc)(...),
	RDArsrc *quitfuncrsrc,void *quitfuncArgs,int level,
	int line,char *file)
{
	RDArunrpt *tmp=NULL;
	time_t ltime;
	time(&ltime);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG RDArunrptNEW Allocating Memory fo RDArunrpt Structure at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDArunrpt));
	if(module!=NULL)
	{
		tmp->module=stralloc(module);
	} else tmp->module=NULL;
	tmp->exit_status=exitstatus;
	if(sortserver!=NULL)
	{
		tmp->sortserver=stralloc(sortserver);
	} else tmp->sortserver=NULL;
	tmp->RScreen=NULL;
	tmp->no_pe_accums=TRUE;
	if(sortfile!=NULL)
	{
		tmp->sortfile=stralloc(sortfile);
	} else tmp->sortfile=NULL;
	tmp->sortno=(-1);
	tmp->APPmainLoop=APPmainLoop;
	tmp->curfont=NULL;
	tmp->fatal_errors=0;
	tmp->errors_encountered=0;
	tmp->numfiles=0;
	tmp->files=NULL;
	tmp->numprims=0;
	tmp->archive=FALSE;
	tmp->fp_pdf=NULL;
	tmp->prims=NULL;
	tmp->cur_brk_name=NULL;
	tmp->prev_brk_name=NULL;
	tmp->prev_brk_name2=NULL;
	tmp->nofields=0;
	tmp->fnames=NULL;
	tmp->fields=NULL;
	tmp->body_count=0;
	tmp->nokeys=0;
	tmp->keys=NULL;
	tmp->errorlist=APPlibNEW();
	tmp->pageno=0;
	tmp->numlines=0;
	tmp->page_count=0;
	tmp->horizontal_count=0;
	tmp->numhorizontals=0;
	tmp->curfont=NULL;
	tmp->pitch=0;
	tmp->points=0;
	tmp->curpoints=0;
	tmp->curpitch=0;
	tmp->longfont=NULL;
	tmp->ofiles=NULL;
	tmp->numofiles=0;
	tmp->record_count=0;
	tmp->forced=FALSE;
	tmp->exception=NULL;
	tmp->rsrsrc=NULL;
	tmp->definelist=NULL;
	tmp->searchrsrc=NULL;
	tmp->mainrsrc=NULL;
	tmp->print_range_criteria=FALSE;
	tmp->range_desc=NULL;
	tmp->alignrsrc=NULL;
	tmp->askb4exec=NULL;
	tmp->diagrsrc=NULL;
	tmp->prtrsrc=NULL;
	tmp->primary_file_no=0;
	tmp->total_count=0;
	tmp->screen_count=0;
	tmp->from_page=0;
	tmp->to_page=0;
	tmp->range_page=FALSE;
	tmp->quitfunc=quitfunc;
	tmp->quitfuncrsrc=quitfuncrsrc;
	tmp->quitfuncArgs=quitfuncArgs;
	tmp->do_process_exceptions=0;
	tmp->dumprttypes=NULL;
	tmp->procexcept_types=NULL;
	tmp->level=level;
	tmp->TableWidget=NULL;
	tmp->table_row=0;
	tmp->lastwidget=NULL;
	memset(tmp->UsingRow,0,101);
	tmp->page=0;
	tmp->total_pages=0;
	return(tmp);
}
void xFREEsubfiles(RUNfile *runfile,int line,char *file)
{
	int y;
	RUNfile *sub;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG FREEsubfiles at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(runfile!=NULL)
	{
		if(runfile->module!=NULL) Rfree(runfile->module);
		if(runfile->filename!=NULL) Rfree(runfile->filename);
		if(runfile->keyname!=NULL) Rfree(runfile->keyname);
		if(runfile->skip_detail!=NULL) Rfree(runfile->skip_detail);
		if(runfile->wc_expression!=NULL) Rfree(runfile->wc_expression);
		if(runfile->expression!=NULL) Rfree(runfile->expression);
		if(runfile->num>0 && runfile->child!=NULL)
		{
			for(y=0,sub=runfile->child;y<runfile->num;++y,++sub)
			{
				FREEsubfiles(sub);
			}
			Rfree(runfile->child);
			runfile->child=NULL;
		}
		runfile=NULL;
	}
}
void xFREEReportfiles(RDArunrpt *rrpt,int line,char *file)
{
	int x;
	RUNfile *prim;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG FREEReportfiles at line [%d] program [%s].",
			line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rrpt->numprims<=0 || rrpt->prims==NULL) return;
	for(x=0,prim=rrpt->prims;x<rrpt->numprims;++x,++prim)
	{
		FREEsubfiles(prim);
	}
	Rfree(rrpt->prims);
	rrpt->numprims=0;
	rrpt->prims=NULL;
}
void xFreeRDArunrpt(RDArunrpt *tmp,int line,char *file)
{
	int x;
	ReportFile *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG FreeRDArunrpt Freeing Allocated Memory for an RDArunrpt Structure at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(tmp!=NULL)
	{
		if(tmp->errorlist!=NULL) freeapplib(tmp->errorlist);
		if(tmp->cur_brk_name!=NULL) Rfree(tmp->cur_brk_name);
		if(tmp->prev_brk_name!=NULL) Rfree(tmp->prev_brk_name);
		if(tmp->prev_brk_name2!=NULL) Rfree(tmp->prev_brk_name2);
		if(tmp->module!=NULL) Rfree(tmp->module);
		if(tmp->exception!=NULL) free_rsrc(tmp->exception);
		if(tmp->askb4exec!=NULL) free_rsrc(tmp->askb4exec);
		if(tmp->mainrsrc!=NULL) free_rsrc(tmp->mainrsrc);
		if(tmp->definelist!=NULL) free_rsrc(tmp->definelist);
		if(tmp->searchrsrc!=NULL) free_rsrc(tmp->searchrsrc);
		if(tmp->alignrsrc!=NULL) free_rsrc(tmp->alignrsrc);
		if(tmp->rsrsrc!=NULL) free_rsrc(tmp->rsrsrc);
		if(tmp->diagrsrc!=NULL) free_rsrc(tmp->diagrsrc);
		if(tmp->RScreen!=NULL) FreeRangeScreen(tmp->RScreen);
		if(tmp->prtrsrc!=NULL) free_rsrc(tmp->prtrsrc);
		if(tmp->curfont!=NULL) Rfree(tmp->curfont);
		if(tmp->sortserver!=NULL) Rfree(tmp->sortserver);
		if(tmp->sortfile!=NULL) Rfree(tmp->sortfile);
		if(tmp->curfont!=NULL) Rfree(tmp->curfont);
		if(tmp->longfont!=NULL) Rfree(tmp->longfont);
		if(tmp->dumprttypes!=NULL) freeapplib(tmp->dumprttypes);
		if(tmp->range_desc!=NULL) Rfree(tmp->range_desc);
		if(tmp->files!=NULL)
		{
			for(x=0,f=tmp->files;x<tmp->numfiles;++x,++f)
			{
				if(f->module!=NULL) Rfree(f->module);
				if(f->filename!=NULL) Rfree(f->filename);
			}
			Rfree(tmp->files);
		}
		if(tmp->ofiles!=NULL)
		{
			for(x=0,f=tmp->ofiles;x<tmp->numofiles;++x,++f)
			{
				if(f->module!=NULL) Rfree(f->module);
				if(f->filename!=NULL) Rfree(f->filename);
			}
			Rfree(tmp->ofiles);
		}
		if(tmp->procexcept_types!=NULL) freeapplib(tmp->procexcept_types);
		FREEReportfiles(tmp);
		Rfree(tmp);
	}
}
short addsubfile(RDAsearch *s,RUNfile *runfile,int type)
{
	RUNfile *p,*c;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG addsubfile Module [%s] Filename [%s] Keyname [%s] Connect Type [%d] Connect Module [%s] Filename [%s].",s->module,s->filename,s->subkeyname,s->ctype,(s->cmodule!=NULL ? s->cmodule:""),(s->cfilename!=NULL ? s->cfilename:""));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(s->cmodule,runfile->module) &&
		!RDAstrcmp(s->cfilename,runfile->filename))
	{
		if(runfile->child!=NULL)
		{
			runfile->child=Rrealloc(runfile->child,
				(runfile->num+1)*sizeof(RUNfile));
		} else {
			runfile->child=Rmalloc(sizeof(RUNfile));
			runfile->num=0;
		}
		c=(RUNfile *)runfile->child+runfile->num;
		c->type=type;
		c->module=stralloc(s->module);
		c->filename=stralloc(s->filename);
		c->parent=runfile;
		c->keyname=stralloc(s->subkeyname);
		c->fileno=0;
		c->num_writes=s->num_writes;
		c->keyno=0;
		c->range=s->range;
		c->num=0;
		if(!isEMPTY(s->expression))
		{
			c->expression=stralloc(s->expression);
		} else c->expression=NULL;
		c->wc_eval=s->wc_eval;
		if(!isEMPTY(s->skip_detail))
		{
			c->skip_detail=stralloc(s->skip_detail);
		} else c->skip_detail=NULL;
		c->wc_skip=s->wc_skip;
		if(!isEMPTY(s->wc_expression))
		{
			c->wc_expression=stralloc(s->wc_expression);
		} else c->wc_expression=NULL;
		c->child=NULL;
		++runfile->num;
		return(0);
	}
	if(runfile->num>0 && runfile->child!=NULL)
	{
		for(x=0,p=runfile->child;x<runfile->num;++x,++p)
		{
			if(!RDAstrcmp(s->cmodule,p->module) &&
				!RDAstrcmp(s->cfilename,p->filename))
			{
				if(!addsubfile(s,p,type)) return(0);
			}
		}
	}
	return(-1);
}
short definefiles(RDAreport *rpt,RDArunrpt *runrpt)
{
/* Function needs to be fixed to handle supporting file w/multiple 
   supporting details... needs to dynamically result it's parent RUNfile,
   won't always be a detail type down here or a child or runrpt->prims's 
   children. */
	RUNfile *rf,*c=NULL,*last_subdetail=NULL;
	int x,count_detail=0,y=0;
	RDAsearch *s;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG definefiles.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	runrpt->numprims=0;
	runrpt->prims=NULL;
	for(x=0,s=rpt->search;x<rpt->numsearchs;++x,++s)
	{
		switch(s->ctype)
		{
			default:
			case 0: /* detail */
				if(runrpt->prims!=NULL)
				{
					runrpt->prims=Rrealloc(runrpt->prims,(runrpt->numprims+1)*sizeof(RUNfile));
				} else {
					runrpt->prims=Rmalloc(sizeof(RUNfile));
					runrpt->numprims=0;
				}
				rf=runrpt->prims+runrpt->numprims;
				++runrpt->numprims;
				rf->type=0;
				rf->module=stralloc(s->module);
				rf->filename=stralloc(s->filename);
				rf->keyname=stralloc(s->subkeyname);
				rf->fileno=0;
				rf->num_writes=s->num_writes;
				rf->keyno=0;
				rf->range=s->range;
				rf->num=0;
				rf->child=NULL;
				rf->parent=NULL;
				if(!isEMPTY(s->expression))
				{
					rf->expression=stralloc(s->expression);
				} else rf->expression=NULL;
				if(!isEMPTY(s->skip_detail))
				{
					rf->skip_detail=stralloc(s->skip_detail);
				} else rf->skip_detail=NULL;
				rf->wc_eval=s->wc_eval;
				if(!isEMPTY(s->wc_expression))
				{
					rf->wc_expression=stralloc(s->wc_expression);
				} else rf->wc_expression=NULL;
				rf->wc_skip=s->wc_skip;
				++count_detail;
				break;
			case 17: /* Not EQUAL to */
			case 1: /* Joined To */
			case 2: /* Equal To or Skip */
			case 3: /* Equal To or Error */
			case 4: /* Greater Than and Equal To */
			case 5: /* Greater Than and Equal To or Skip */
			case 6: /* Greater Than and Equal To or Error */
			case 7: /* Less Than and Equal To */
			case 8: /* Less Than and Equal To or Skip */
			case 9: /* Less Than and Equal To or Error */
			case 10: /* Greater Than */
			case 11: /* Greater Than or Skip */
			case 12: /* Greater Than or Error */
			case 13: /* Less Than */
			case 14: /* Less Than or Skip */
			case 15: /* Less Than or Error */
				rf=runrpt->prims+(count_detail-1); /* this line needs to be a function so that it could get the RUNfile that's the supporting detail */
				if(RDAstrcmp(rf->module,s->cmodule) ||
					RDAstrcmp(rf->filename,s->cfilename))
				{
					rf=last_subdetail;
				}
				if(addsubfile(s,rf,s->ctype))
				{
					prterr("Error file hierarchy Violation connecting File [%s] [%s].",s->module,s->filename);
				}
				break;
			case 16: /* Supporting Detail */
				rf=runrpt->prims+(count_detail-1);
				if(addsubfile(s,rf,s->ctype))
				{
					prterr("Error file hierarchy Violation connecting File [%s] [%s].",s->module,s->filename);
				} else {
					last_subdetail=(((RUNfile *)rf->child)+rf->num-1);
				}
				break;
		}
	}
	for(x=0,rf=runrpt->prims;x<runrpt->numprims;++x,++rf)
	{
		if(rf->child!=NULL)
		{
			for(y=0,c=rf->child;y<rf->num;++y,++c)
			{
				c->parent=rf;
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		SHOWrunfiles(runrpt);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(0);
}
short SHOWsubfile(int num,RUNfile *runfile,int count)
{
	RUNfile *p=NULL;
/*
	RUNfile *PARENT=NULL;
*/
	int x,child_count=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG SHOWsubfile.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	child_count=count+1;
	for(x=0,p=runfile;x<num;++x,++p)
	{
/*
		PARENT=p->parent;
*/
		memset(stemp,0,101);
		memset(stemp,'\t',count);
/* SJS this didn't work still cores tring to print out the PARENT->module and PARENT->filename */
/*
		if(PARENT!=NULL)
		{
			prterr("%s Sub-File [%d] Joined Type [%d] File [%s][%s] Keyname [%s] Connect [%s][%s] Fileno [%d] Keyno [%d].\n",stemp,x+1,p->type,p->module,p->filename,p->keyname,PARENT->module,PARENT->filename,p->fileno,p->keyno); 
		} else {
*/
			prterr("%s Sub-File [%d] Joined Type [%d] File [%s][%s] Keyname [%s] Fileno [%d] Keyno [%d].\n",stemp,x+1,p->type,p->module,p->filename,p->keyname,p->fileno,p->keyno); 
/*
		}
*/
		if(p->child!=NULL)
		{
			if(!SHOWsubfile(p->num,p->child,child_count)) return(0);
			prterr("\n");
		}
	}
	prterr("-------------------------------------------------------------------------\n");
	return(-1);
}
short SHOWrunfiles(RDArunrpt *runrpt)
{
	RUNfile *rf;
	int x,count=1;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG SHOWrunfiles.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	for(x=0,rf=runrpt->prims;x<runrpt->numprims;++x,++rf)
	{
		prterr("DIAG Primary [%d] File Module [%s] File [%s] Keyname [%s] Fileno [%d] Keyno [%d].",x,rf->module,rf->filename,rf->keyname,rf->fileno,rf->keyno); 
		if(rf->child!=NULL)
		{
			SHOWsubfile(rf->num,rf->child,count);
		}
	}
	prterr("*************************************************************************\n");
	return(0);
}
