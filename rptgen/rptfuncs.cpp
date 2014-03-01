/* rptfuncs.c - Report Generator Functions */
#include <rptgen.hpp>

#ifdef WIN32
#include <io.h>
#endif

char *REMOVE_LIST[]={"Do Nothing","Remove File","dohttp/URL"};

RDAreport *xRDAreportNEW(char *name,char *title,char *desc,short engine,
	char *server_name,short process_exceptions,short use_print_screen,
	short stop_on_error,short ask_b4exe,short chain_report,
	char *chain_module,char *chain_name,short range_screen,
	short diag_breakable,short removesort,char *remove_name,
	short no_empty_error,short no_wc_diag,short writecount_only,
	short write_all_detail_levels,int line,char *file)
{
	RDAreport *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG RDAreportNEW Creating RDAreport Structure for Report [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAreport));
	if(name!=NULL)
	{
		tmp->name=stralloc(name);
	} else {
		tmp->name=NULL;
	}
	if(!isEMPTY(desc))
	{
		tmp->desc=stralloc(desc);
	} else { 
		tmp->desc=NULL;
	}
	if(!isEMPTY(title))
	{
		tmp->title=stralloc(title);
	} else { 
		tmp->title=NULL;
	}
	tmp->use_print_screen=use_print_screen;
	tmp->process_exceptions=process_exceptions;
	tmp->diag_breakable=diag_breakable;
	tmp->engine_type=engine; /* ctree engine default */
	tmp->stop_on_error=stop_on_error; /* defaults to stop on error */
	if(server_name!=NULL)
	{
		tmp->server_name=stralloc(server_name);
	} else { 
		tmp->server_name=NULL;
	}
	tmp->ask_b4_execute=ask_b4exe;
	tmp->chain_report=chain_report;
	if(chain_report)
	{
		if(chain_module!=NULL)
		{
			tmp->chain_module=stralloc(chain_module);
		} else { 
			tmp->chain_module=NULL;
		}
		if(chain_name!=NULL)
		{
			tmp->chain_name=stralloc(chain_name);
		} else { 
			tmp->chain_name=NULL;
		}
	} else { 
		tmp->chain_name=NULL;
	}
	tmp->range_screen=range_screen;
	tmp->removesort=removesort;
	tmp->reporttype=0;
	tmp->remove_file=FALSE;
	tmp->remove_name=NULL;
	tmp->numvirtuals=0;
	tmp->virflds=NULL;
	tmp->numsearchs=0;
	tmp->search=NULL;
	tmp->select=NULL;
	tmp->numsorts=0;
	tmp->sort=NULL;
	tmp->numcontrols=0;
	tmp->control=NULL;
	tmp->numaccums=0;
	tmp->accum=NULL;
	tmp->display=NULL;
	tmp->numimages=0;
	tmp->image=NULL;
	tmp->type=0;
	tmp->no_empty_error=no_empty_error; 
	tmp->no_wc_diag=no_wc_diag; 
	tmp->writecount_only=writecount_only;
	tmp->write_all_detail_levels=write_all_detail_levels;
	tmp->MTNMSTR=NULL;
	return(tmp);
}
void xFreeRDAreport(RDAreport *r,int line,char *file)
{
	int x,y;
	RDAvirtual *v;
	RDAsearch *s;
	RDAsort *o;
	RDAcontrol *c;
	RDAaccum *a;
	RDAdisplay *d;
	RDAimage *i;
	RDApvar *pvar;
	RDApval *pval;
	RDAconfld *confld;
	RDAaccval *accval;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG FreeRDAreport Freeing RDAreport Structure for Report [%s] at line [%d] program [%s].",(r->name!=NULL ? r->name:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->name!=NULL) Rfree(r->name);
		if(r->title!=NULL) Rfree(r->title);
		if(r->desc!=NULL) Rfree(r->desc);
		if(r->server_name!=NULL) Rfree(r->server_name);
		if(r->chain_module!=NULL) Rfree(r->chain_module);
		if(r->chain_name!=NULL) Rfree(r->chain_name);
		if(r->remove_name!=NULL) Rfree(r->remove_name);
		if(r->virflds!=NULL)
		{
			for(x=0,v=r->virflds;x<r->numvirtuals;++x,++v)
			{
				if(v->name!=NULL) Rfree(v->name);
				if(v->expression!=NULL) Rfree(v->expression);
				if(!r->type)
				{
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
						prterr("Error Field Type [%d] is invalid for Virtual Field [%s].",v->type,v->name);
						break;
				}
				}
			}
			Rfree(r->virflds);
		}
		if(r->search!=NULL)
		{
			for(x=0,s=r->search;x<r->numsearchs;++x,++s)
			{
				if(s->module!=NULL) Rfree(s->module);
				if(s->filename!=NULL) Rfree(s->filename);
				if(s->cmodule!=NULL) Rfree(s->cmodule);
				if(s->cfilename!=NULL) Rfree(s->cfilename);
				if(s->expression!=NULL) Rfree(s->expression);
				if(s->skip_detail!=NULL) Rfree(s->skip_detail);
				if(s->confld!=NULL)
				{
					for(y=0,confld=s->confld;y<s->num;++y,++confld)
					{
						if(confld->fldname!=NULL) Rfree(confld->fldname);
						if(confld->conname!=NULL) Rfree(confld->conname);
					}
					Rfree(s->confld);
				}
				if(s->subkeyname!=NULL) Rfree(s->subkeyname);
				if(s->wc_expression!=NULL) Rfree(s->wc_expression);
			}
			Rfree(r->search);
		}
		if(r->select!=NULL) Rfree(r->select);
		if(r->sort!=NULL)
		{
			for(x=0,o=r->sort;x<r->numsorts;++x,++o)
			{
				if(o->name!=NULL) Rfree(o->name);
			}
			Rfree(r->sort);
		}
		if(r->control!=NULL)
		{
			for(x=0,c=r->control;x<r->numcontrols;++x,++c)
			{
				if(!r->type)
				{
					switch(c->fieldtype)
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
							if(c->value.string_value!=NULL) 
								Rfree(c->value.string_value);
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDECIMALV:
						case SDOUBLEV:
							if(c->value.float_value!=NULL) 
								Rfree(c->value.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							if(c->value.short_value!=NULL) 
								Rfree(c->value.short_value);
							break;
						case SLONGV:
						case LONGV:
						case SCROLLEDLIST:
							if(c->value.integer_value!=NULL)
								Rfree(c->value.integer_value);
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error Field Type [%d] is invalid for Virtual Field [%s].",c->fieldtype,c->name);
							break;
					}
				}
				if(c->name!=NULL) Rfree(c->name);
			}
			Rfree(r->control);
		}
		if(r->accum!=NULL)
		{
			for(x=0,a=r->accum;x<r->numaccums;++x,++a)
			{
				if(a->accvals!=NULL)
				{
					for(y=0,accval=a->accvals;y<a->num;++y,++accval)
					{
						if(accval->breakname!=NULL) Rfree(accval->breakname);
						switch(a->fieldtype)
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
								if(accval->value.string_value!=NULL) 
									Rfree(accval->value.string_value);
								if(accval->prev_value.string_value!=NULL) Rfree(accval->prev_value.string_value);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(accval->value.float_value!=NULL)
									Rfree(accval->value.float_value);
								if(accval->prev_value.float_value!=NULL)
									Rfree(accval->prev_value.float_value);
								break;
							case SHORTV:
							case SSHORTV:
								if(accval->value.short_value!=NULL)
									Rfree(accval->value.short_value);
								if(accval->prev_value.short_value!=NULL)
									Rfree(accval->prev_value.short_value);
								break;
							case SLONGV:
							case LONGV:
							case SCROLLEDLIST:
								if(accval->value.integer_value!=NULL) 
									Rfree(accval->value.integer_value);
								if(accval->prev_value.integer_value!=NULL) 
									Rfree(accval->prev_value.integer_value);
								break;
							case BUTTONS:
								break;
							default:
								prterr("Error Field Type [%d] for Accumulator [%s].",a->fieldtype,a->name);
								break;
						}
					}
					Rfree(a->accvals);
				}
				if(a->name!=NULL) Rfree(a->name);
				if(a->accname!=NULL) Rfree(a->accname);
			}
			Rfree(r->accum);
		}
		if(r->display!=NULL)
		{
			d=r->display;
			if(d->set_lpp!=NULL) Rfree(d->set_lpp);
			if(d->pvars!=NULL)
			{
				for(y=0,pvar=d->pvars;y<d->num;++y,++pvar)
				{
					if(pvar->name!=NULL) Rfree(pvar->name);
					if(pvar->command!=NULL) Rfree(pvar->command);
				}
				Rfree(d->pvars);
			}
			if(d->device_spec!=NULL) Rfree(d->device_spec);
			Rfree(r->display);
		}
		if(r->image!=NULL)
		{
			for(x=0,i=r->image;x<r->numimages;++x,++i)
			{
				if(i->line!=NULL) Rfree(i->line);
				if(i->breakname!=NULL) Rfree(i->breakname);
				if(i->expression!=NULL) Rfree(i->expression);
				if(i->pvals!=NULL)
				{
					for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
					{
						if(pval->name!=NULL) Rfree(pval->name);
						if(pval->fieldname!=NULL) Rfree(pval->fieldname);
					}
					Rfree(i->pvals);
				}
			}
			Rfree(r->image);
		}
		if(r->MTNMSTR!=NULL) FreeMaintainMaster(r->MTNMSTR);
		Rfree(r);
		r=NULL;
	}
}
RDAreport *xGETRDAreport(char *module,char *name,short type,int line,char *file)
{
	int fd;
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL;
	short x,test=0,version=FALSE;
	int y,z;
	RDAreport *rpt=NULL;
	RDAsearch *srch;
	RDAsort *sort;
	RDAvirtual *virt;
	RDAimage *img;
	RDAcontrol *c;
	RDAaccum *acm;
	RDAdisplay *d;
	RDApvar *pvar;
	RDApval *pval;
	RDAconfld *confld;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG Getting Report Binary [%s] [%s.RPT] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(name)+7+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.RPT",CURRENTDIRECTORY,module,fname1);
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.RPT",CURRENTDIRECTORY,module,fname1);
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(fd==(-1))
	{
		prterr("Error Can't open file(%s)%s.  The report file [%s] either doesn't exist or security is denied for this file.",fname,errname(),fname);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	if(access(fname,04))
	{
		prterr("Error in permissions, user can't read report file [%s] at line [%d] program [%s].",fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=BINread(fd);
	if(mainbin==NULL)
	{
		prterr("Error Can't read file(%s) or file doesn't exist or security is denied.",fname);
		if(close(fd)==(-1)) {prterr("Error Can't close file(%s)",fname);}
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1681) && x!=(-1682) && x!=(-1683) && 
		x!=(-1684) && x!=(-1685) && x!=(-1686) && x!=(-1687) && 
		x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && 
		x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1695) && 
		x!=(-1696) && x!=(-1697) && x!=(-1701) && x!=(-1702) && 
		x!=(-1703) && x!=(-1704) && x!=(-1699) && x!=(-1698))
	{
		prterr("Error Invalid version or bad binary in Report File [%s].  Report definition may be corrupted.",fname);
		BINfree(mainbin);
		return(NULL);
	}
	if(x==(-1681)) version=22;
	else if(x==(-1682)) version=21;
	else if(x==(-1683)) version=20;
	else if(x==(-1684)) version=19;
	else if(x==(-1685)) version=18;
	else if(x==(-1686)) version=17;
	else if(x==(-1687)) version=16;
	else if(x==(-1688)) version=15;
	else if(x==(-1689)) version=14;
	else if(x==(-1690)) version=13;
	else if(x==(-1691)) version=12;
	else if(x==(-1692)) version=11;
	else if(x==(-1693)) version=10;
	else if(x==(-1694)) version=9;
	else if(x==(-1695)) version=8;
	else if(x==(-1696)) version=7;
	else if(x==(-1697)) version=6;
	else if(x==(-1702)) version=1;
	else if(x==(-1703)) version=2;
	else if(x==(-1704)) version=3;
	else if(x==(-1699)) version=4;
	else if(x==(-1698)) version=5;
	else version=FALSE;
	rpt=RDAreportNEW(NULL,NULL,NULL,0,NULL,0,0,0,0,0,NULL,NULL,0,0,0);
	rpt->type=type;
	if(version>2)
	{
		test=BINgetshort(mainbin);
		if(test) rpt->name=BINgetstring(mainbin);
			else rpt->name=NULL;
	} else rpt->name=BINgetstring(mainbin);
	if(version)
	{
		test=BINgetshort(mainbin);
		if(test) rpt->title=BINgetstring(mainbin);
			else rpt->title=NULL;
		test=BINgetshort(mainbin);
		if(test) rpt->desc=BINgetstring(mainbin);
			else rpt->desc=NULL;
	}
	rpt->engine_type=BINgetshort(mainbin);
	rpt->stop_on_error=BINgetshort(mainbin);
	if(version>12)
	{
		rpt->no_empty_error=BINgetshort(mainbin);
		rpt->no_wc_diag=BINgetshort(mainbin);
	} else {
		rpt->no_empty_error=FALSE;
		rpt->no_wc_diag=FALSE;
	}
	if(version>14)
	{
		rpt->writecount_only=BINgetshort(mainbin);
		rpt->write_all_detail_levels=BINgetshort(mainbin);
	} else {
		rpt->writecount_only=FALSE;
		rpt->write_all_detail_levels=FALSE;
	}
	if(version>4)
	{
		rpt->diag_breakable=BINgetshort(mainbin);
	} else rpt->diag_breakable=FALSE;
	rpt->process_exceptions=BINgetshort(mainbin);
	test=BINgetshort(mainbin);
	if(test) rpt->server_name=BINgetstring(mainbin);
		else rpt->server_name=NULL;
	if(version>21)
	{
		test=BINgetshort(mainbin);
		if(test) rpt->manual_form=TRUE;
			else rpt->manual_form=FALSE;
		test=BINgetshort(mainbin);
		if(test) rpt->report_completion=TRUE;
			else rpt->report_completion=FALSE;
	} else {
		rpt->manual_form=FALSE;
		rpt->report_completion=FALSE;
	}
	test=0;
	rpt->ask_b4_execute=BINgetshort(mainbin);
	rpt->chain_report=BINgetshort(mainbin);
	if(rpt->chain_report)
	{
		test=BINgetshort(mainbin);
		if(test) rpt->chain_module=BINgetstring(mainbin);
			else rpt->chain_module=NULL;
		test=BINgetshort(mainbin);
		if(test) rpt->chain_name=BINgetstring(mainbin);
			else rpt->chain_name=NULL;
	}
	rpt->range_screen=BINgetshort(mainbin);
	if(version>7)
	{
		rpt->reporttype=BINgetint(mainbin);
	} else rpt->reporttype=0;
	rpt->remove_file=BINgetshort(mainbin);
	if(rpt->remove_file)
	{
		if(version>2)
		{
			test=BINgetshort(mainbin);
			if(test) rpt->remove_name=BINgetstring(mainbin);
				else rpt->remove_name=NULL;
		} else rpt->remove_name=BINgetstring(mainbin);
	}
	rpt->removesort=BINgetshort(mainbin);
	if(version>8)
	{
		rpt->use_print_screen=BINgetshort(mainbin);
	} else rpt->use_print_screen=TRUE;
	rpt->numsearchs=BINgetint(mainbin);
	if(rpt->numsearchs>0)
	{
		rpt->search=Rmalloc(rpt->numsearchs*sizeof(RDAsearch));
		for(y=0,srch=rpt->search;y<rpt->numsearchs;++y,++srch)
		{
			srch->occurrence=(-1);
			srch->key_fastbreak=FALSE;
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) srch->module=BINgetstring(mainbin);
					else srch->module=NULL;
			} else srch->module=BINgetstring(mainbin);
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) srch->filename=BINgetstring(mainbin);
					else srch->filename=NULL;
			} else srch->filename=BINgetstring(mainbin);
			if(version>7)
			{
				test=BINgetshort(mainbin);
				if(test) srch->expression=BINgetstring(mainbin);
					else srch->expression=NULL;
			} else srch->expression=NULL;
			if(version>10)
			{
				test=BINgetshort(mainbin);
				if(test) srch->skip_detail=BINgetstring(mainbin);
					else srch->skip_detail=NULL;
			} else {
				srch->skip_detail=NULL;
			}
			srch->ctype=BINgetint(mainbin);
			if(srch->ctype>0 || version>9)
			{
				if(version>2)
				{
					test=BINgetshort(mainbin);
					if(test) srch->cmodule=BINgetstring(mainbin);
						else srch->cmodule=NULL;
				} else srch->cmodule=BINgetstring(mainbin);
				if(version>2)
				{
					test=BINgetshort(mainbin);
					if(test) srch->cfilename=BINgetstring(mainbin);
						else srch->cfilename=NULL;
				} else srch->cfilename=BINgetstring(mainbin);
				srch->num=BINgetshort(mainbin);
				if(srch->num>0)
				{
					srch->confld=Rmalloc(srch->num*sizeof(RDAconfld));
					for(z=0,confld=srch->confld;z<srch->num;++z,++confld)
					{
						if(version>2)
						{
							test=BINgetshort(mainbin);
							if(test) confld->fldname=BINgetstring(mainbin);
								else confld->fldname=NULL;
						} else confld->fldname=BINgetstring(mainbin);
						if(version>2)
						{
							test=BINgetshort(mainbin);
							if(test) confld->conname=BINgetstring(mainbin);
								else confld->conname=NULL;
						} else confld->conname=BINgetstring(mainbin);
					}
				} else {
					srch->confld=NULL;
				}
			} else {
				srch->cmodule=NULL;
				srch->cfilename=NULL;
				srch->confld=NULL;
			}
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) srch->subkeyname=BINgetstring(mainbin);
					else srch->subkeyname=NULL;
			} else srch->subkeyname=BINgetstring(mainbin);
			if(version>1)
			{
				srch->range=BINgetshort(mainbin);
			} else srch->range=FALSE;
			if(version>5)
			{
				srch->num_writes=BINgetshort(mainbin);
			} else srch->num_writes=1;
			if(version>12)
			{
				srch->wc_eval=BINgetshort(mainbin);
				test=BINgetshort(mainbin);
				if(test) srch->wc_expression=BINgetstring(mainbin);
					else srch->wc_expression=NULL;
			} else {
				srch->wc_eval=FALSE;
				srch->wc_expression=NULL;
			}
			if(version>13)
			{
				srch->wc_skip=BINgetshort(mainbin);
				test=BINgetshort(mainbin);
				if(test) srch->skip_detail_error=BINgetstring(mainbin);
					else srch->skip_detail_error=NULL;
			} else {
				srch->wc_skip=FALSE;
				srch->skip_detail_error=NULL;
			}
		}
	}
	rpt->numvirtuals=BINgetint(mainbin);
	if(rpt->numvirtuals>0)
	{
		rpt->virflds=Rmalloc(rpt->numvirtuals*sizeof(RDAvirtual));
		for(y=0,virt=rpt->virflds;y<rpt->numvirtuals;++y,++virt)
		{
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) virt->name=BINgetstring(mainbin);
					else virt->name=NULL;
			} else virt->name=BINgetstring(mainbin);
			virt->type=BINgetint(mainbin);
			virt->when=BINgetint(mainbin);
			if(version>6)
			{
				virt->len=BINgetint(mainbin);
			} else virt->len=BINgetshort(mainbin);
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) virt->expression=BINgetstring(mainbin);
					else virt->expression=NULL;
			} else virt->expression=BINgetstring(mainbin);
			if(version>20)	
			{
				test=BINgetshort(mainbin);
				if(test) virt->computed=TRUE;
					else virt->computed=FALSE;
			} else virt->computed=FALSE;
			if(version>3)
			{
				test=BINgetshort(mainbin);
				virt->range=test;
			} else virt->range=FALSE;
			if(!rpt->type)
			{
			switch(virt->type)
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
					virt->value.string_value=NULL;
					virt->dlen=0;
					break;
				case BOOLNS:
				case CHARACTERS:
					virt->value.string_value=Rmalloc(1);
					*virt->value.string_value=0;
					virt->dlen=1;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					virt->value.float_value=Rmalloc(sizeof(double));
					*virt->value.float_value=0;
					virt->dlen=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					virt->value.short_value=Rmalloc(sizeof(short));
					*virt->value.short_value=0;
					virt->dlen=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					virt->value.integer_value=Rmalloc(sizeof(int));
					*virt->value.integer_value=0;
					virt->dlen=sizeof(int);
					break;
				default:
					prterr("Error Field Type [%d] is invalid for virtual field [%s].",virt->type,virt->name);
					virt->dlen=0;
					break;
			}
			}
		}
	}
	test=BINgetshort(mainbin);
	if(test)
	{
		rpt->select=BINgetstring(mainbin);
	} else {
		rpt->select=NULL;
	}
	test=0;
	rpt->numsorts=BINgetint(mainbin);
	if(rpt->numsorts>0)
	{
		rpt->sort=Rmalloc(rpt->numsorts*sizeof(RDAsort));
		for(y=0,sort=rpt->sort;y<rpt->numsorts;++y,++sort)
		{
			sort->type=BINgetint(mainbin);
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) sort->name=BINgetstring(mainbin);
					else sort->name=NULL;
			} else sort->name=BINgetstring(mainbin);
		}
	}
	rpt->numcontrols=BINgetint(mainbin);
	if(rpt->numcontrols>0)
	{
		rpt->control=Rmalloc(rpt->numcontrols*sizeof(RDAcontrol));
		for(y=0,c=rpt->control;y<rpt->numcontrols;++y,++c)
		{
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) c->name=BINgetstring(mainbin);
					else c->name=NULL;
			} else c->name=BINgetstring(mainbin);
			switch(c->fieldtype)
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
					c->value.string_value=NULL;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDECIMALV:
				case SDOUBLEV:
					c->value.float_value=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					c->value.short_value=NULL;
					break;
				case SLONGV:
				case LONGV:
				case SCROLLEDLIST:
					c->value.integer_value=NULL;
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Virtual Field [%s].",c->fieldtype,c->name);
					break;
			}
		}
	}
	rpt->numaccums=BINgetint(mainbin);
	if(rpt->numaccums>0)
	{
		rpt->accum=Rmalloc(rpt->numaccums*sizeof(RDAaccum));
		for(y=0,acm=rpt->accum;y<rpt->numaccums;++y,++acm)
		{
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) acm->name=BINgetstring(mainbin);
					else acm->name=NULL;
			} else acm->name=BINgetstring(mainbin);
			acm->atype=BINgetint(mainbin);
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) acm->accname=BINgetstring(mainbin);
					else acm->accname=NULL;
			} else acm->accname=BINgetstring(mainbin);
			acm->num=0;
			acm->accvals=NULL;
		}
	}
	test=BINgetshort(mainbin);
	if(test)
	{
		rpt->display=Rmalloc(sizeof(RDAdisplay));
		d=rpt->display;
		d->page_width=BINgetdouble(mainbin);
		d->page_length=BINgetdouble(mainbin);
		d->date_order=BINgetshort(mainbin);
		d->first_page_hdr=BINgetshort(mainbin);
		d->last_page_ftr=BINgetshort(mainbin);
		d->device=BINgetshort(mainbin);
		d->body_count=BINgetshort(mainbin);
		if(version>17)
		{
			d->count_invisible=BINgetshort(mainbin);
		} else d->count_invisible=TRUE;
		if(version>18)
		{
			test=BINgetshort(mainbin);
			if(test)
			{
				d->doscopy=(char)test;
			} else {
				d->doscopy=FALSE;
			}
			test=BINgetshort(mainbin);
			if(test)
			{ 
				d->device_spec=BINgetstring(mainbin);
			} else {
				d->device_spec=NULL;
			}
		} else {
			d->doscopy=FALSE;
			d->device_spec=NULL;
		}
		if(version>11)
		{
			test=BINgetshort(mainbin);
			if(test)
			{
				d->body_count_type=(char)test;
			} else {
				d->body_count_type=FALSE;
			}
		} else {
			d->body_count_type=FALSE;
		}
		d->test_pages=BINgetshort(mainbin);
		d->starting_page=BINgetshort(mainbin);
		test=BINgetshort(mainbin);
		if(test) d->set_lpp=BINgetstring(mainbin);
			else d->set_lpp=NULL;
		if(version>19)
		{
			test=BINgetshort(mainbin);
			if(test==FALSE) d->set_lpp_eval=FALSE;
				 else d->set_lpp_eval=TRUE;
		} else {
			d->set_lpp_eval=FALSE;
		}
		if(version>15)
		{
			test=BINgetshort(mainbin);
			if(test==FALSE) d->dumprinter=FALSE;
				 else d->dumprinter=TRUE;
			test=BINgetshort(mainbin);
			d->dumprinter_type=test;
		} else {
			d->dumprinter=FALSE;
			d->dumprinter_type=0;
		}
		d->num=BINgetint(mainbin);
		if(d->num>0)
		{
			d->pvars=Rmalloc(d->num*sizeof(RDApvar));
			for(y=0,pvar=d->pvars;y<d->num;++y,++pvar)
			{
				if(version>2)
				{
					test=BINgetshort(mainbin);
					if(test) pvar->name=BINgetstring(mainbin);
						else pvar->name=NULL;
				} else pvar->name=BINgetstring(mainbin);
				if(version>2)
				{
					test=BINgetshort(mainbin);
					if(test) pvar->command=BINgetstring(mainbin);
						else pvar->command=NULL;
				} else pvar->command=BINgetstring(mainbin);
				pvar->chgsize=BINgetshort(mainbin);
				pvar->pitch=BINgetshort(mainbin);
				pvar->points=BINgetshort(mainbin);
			}
		} else d->pvars=NULL;
	} else {
		rpt->display=NULL;
		d=NULL;
	}
	rpt->numimages=BINgetint(mainbin);
	if(rpt->numimages>0)
	{
		rpt->image=Rmalloc(rpt->numimages*sizeof(RDAimage));
		for(y=0,img=rpt->image;y<rpt->numimages;++y,++img)
		{
			img->type=BINgetint(mainbin);
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test)
				{
					img->breakname=BINgetstring(mainbin);
				} else { 
					img->breakname=NULL;
				}
			} else { 
				img->breakname=BINgetstring(mainbin);
			}
			if(version>2)
			{
				test=BINgetshort(mainbin);
				if(test) 
				{
					img->line=BINgetstring(mainbin);
				} else { 
					img->line=NULL;
				}
			} else { 
				img->line=BINgetstring(mainbin);
			}
			img->numpvals=BINgetint(mainbin);
			if(img->numpvals>0)
			{
				img->pvals=Rmalloc(img->numpvals*sizeof(RDApval));
				for(z=0,pval=img->pvals;z<img->numpvals;++z,++pval)
				{
					pval->symbol_type=BINgetshort(mainbin);
					if(version>2)
					{
						test=BINgetshort(mainbin);
						if(test)
						{
							pval->name=BINgetstring(mainbin);
						} else { 
							pval->name=NULL;
						}
					} else { 
						pval->name=BINgetstring(mainbin);
					}
					pval->type=BINgetint(mainbin);
					if(d->device>2 && d->device!=5) 
					{
						if(version>2)
						{
							test=BINgetshort(mainbin);
							if(test)
							{
								pval->fieldname=BINgetstring(mainbin);
							} else { 
								pval->fieldname=NULL;
							}
						} else { 
							pval->fieldname=BINgetstring(mainbin);
						}
					}
				}
			} else img->pvals=NULL;
			img->test=BINgetshort(mainbin);
			if(img->test) 
			{
				img->expression=BINgetstring(mainbin);
			} else {
				img->expression=NULL;
			}
		}
	} else { 
		rpt->image=NULL;
	}
	if(version>16)
	{
		test=BINgetshort(mainbin);
		if(test)
		{
			rpt->MTNMSTR=ReportMaintainMasterNew(module,name,rpt);
			getMaintainMasterADV(fname,rpt->MTNMSTR,&mainbin);
		} else { 
			rpt->MTNMSTR=NULL;
		}
	} else {
		rpt->MTNMSTR=NULL;
	}
	BINfree(mainbin);
	if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
	if(fname!=NULL) Rfree(fname);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field) 
	{
		prterr("DIAG GETRDAreport Successfully Returning Report Binary [%s] [%s.RPT] to line [%d] program [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(rpt);
}
short xWRTRDAreport(char *module,RDAreport *rpt,int line,char *file)
{
	int fd,y,z;
	BIN *bin;
	char *fname=NULL,*fname1=NULL;
	RDAsearch *srch;
	RDAsort *sort;
	RDAvirtual *virt;
	RDAimage *img;
	RDAcontrol *c;
	RDAaccum *acm;
	RDAdisplay *d=NULL;
	RDApvar *pvar;
	RDApval *pval;
	RDAconfld *confld;
/*
	MaintainMaster *MTNMSTR;
*/
	short test=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG WRTRDAreport Writing report defintion binary [%s] [%s] at line [%d] file [%s].",(module!=NULL ? module:""),(rpt->name!=NULL ? rpt->name:""),line,file);
/* SJS took out because of using prtidea for the RDA_PFILE instead of 
   STDOUT.
		SEERDAreport(rpt,FALSE);
*/
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(rpt->name)+7+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(rpt->name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.RPT",CURRENTDIRECTORY,module,fname1);
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.RPT",CURRENTDIRECTORY,module,fname1);
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(fd!=(-1))
	{
		if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
		if(access(fname,02))
		{
			prterr("Error in permissions, user can't write report file [%s] at line [%d] program [%s].",fname,line,file);
			if(fname!=NULL) Rfree(fname);
			return(-1);
		}
		unlink(fname);
	}
	bin=BINnew();
	BINaddshort(bin,-1681);	/*type identifier*/
	if(!isEMPTY(rpt->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,rpt->name);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(rpt->title))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,rpt->title);
	} else {
		BINaddshort(bin,FALSE);
	}
	if(!isEMPTY(rpt->desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,rpt->desc);
	} else {
		BINaddshort(bin,FALSE);
	}
	BINaddshort(bin,rpt->engine_type);
	BINaddshort(bin,rpt->stop_on_error);
	BINaddshort(bin,rpt->no_empty_error);
	BINaddshort(bin,rpt->no_wc_diag);
	BINaddshort(bin,rpt->writecount_only);
	BINaddshort(bin,rpt->write_all_detail_levels);
	BINaddshort(bin,rpt->diag_breakable);
	BINaddshort(bin,rpt->process_exceptions);
	if(!isEMPTY(rpt->server_name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,rpt->server_name);
	} else {
		BINaddshort(bin,FALSE);
	}
	BINaddshort(bin,(rpt->manual_form ? TRUE:FALSE));
	BINaddshort(bin,(rpt->report_completion ? TRUE:FALSE));
	BINaddshort(bin,rpt->ask_b4_execute);
	BINaddshort(bin,rpt->chain_report);
	if(rpt->chain_report)
	{
		if(!isEMPTY(rpt->chain_module))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,rpt->chain_module);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(rpt->chain_name))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,rpt->chain_name);
		} else BINaddshort(bin,FALSE);
	}
	BINaddshort(bin,rpt->range_screen);
	BINaddint(bin,rpt->reporttype);
	BINaddshort(bin,rpt->remove_file);
	if(rpt->remove_file)
	{
		if(!isEMPTY(rpt->remove_name))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,rpt->remove_name);
		} else BINaddshort(bin,FALSE);
	}
	BINaddshort(bin,rpt->removesort);
	BINaddshort(bin,rpt->use_print_screen);
	BINaddint(bin,rpt->numsearchs);
	if(rpt->numsearchs>0)
	{
		for(y=0,srch=rpt->search;y<rpt->numsearchs;++y,++srch)
		{
			if(!isEMPTY(srch->module))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->module);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(srch->filename))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->filename);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(srch->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->expression);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(srch->skip_detail))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->skip_detail);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,srch->ctype);
			if(!isEMPTY(srch->cmodule))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->cmodule);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(srch->cfilename))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->cfilename);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,srch->num);
			if(srch->num>0)
			{
				for(z=0,confld=srch->confld;z<srch->num;++z,++confld)
				{
					if(!isEMPTY(confld->fldname))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,confld->fldname);
					} else BINaddshort(bin,FALSE);
					if(!isEMPTY(confld->conname))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,confld->conname);
					} else BINaddshort(bin,FALSE);
				}
			}
			if(!isEMPTY(srch->subkeyname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->subkeyname);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,srch->range);
			BINaddshort(bin,srch->num_writes);
			BINaddshort(bin,srch->wc_eval);
			if(!isEMPTY(srch->wc_expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->wc_expression);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,srch->wc_skip);
			if(!isEMPTY(srch->skip_detail_error))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,srch->skip_detail_error);
			} else BINaddshort(bin,FALSE);
		}
	}
	BINaddint(bin,rpt->numvirtuals);
	if(rpt->numvirtuals>0)
	{
		for(y=0,virt=rpt->virflds;y<rpt->numvirtuals;++y,++virt)
		{
			if(!isEMPTY(virt->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,virt->name);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,virt->type);
			BINaddint(bin,virt->when);
			BINaddint(bin,virt->len);
			if(!isEMPTY(virt->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,virt->expression);
			} else BINaddshort(bin,FALSE);
			test=virt->computed;
			BINaddshort(bin,test);
			test=virt->range;
			BINaddshort(bin,test);
		}
	}
	if(!isEMPTY(rpt->select))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,rpt->select);
	} else {
		BINaddshort(bin,FALSE);
	}
	BINaddint(bin,rpt->numsorts);
	if(rpt->numsorts>0)
	{
		for(y=0,sort=rpt->sort;y<rpt->numsorts;++y,++sort)
		{
			BINaddint(bin,sort->type);
			if(!isEMPTY(sort->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sort->name);
			} else BINaddshort(bin,FALSE);
		}
	}
	BINaddint(bin,rpt->numcontrols);
	if(rpt->numcontrols>0)
	{
		for(y=0,c=rpt->control;y<rpt->numcontrols;++y,++c)
		{
			if(!isEMPTY(c->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,c->name);
			} else BINaddshort(bin,FALSE);
		}
	}
	BINaddint(bin,rpt->numaccums);
	if(rpt->numaccums>0)
	{
		for(y=0,acm=rpt->accum;y<rpt->numaccums;++y,++acm)
		{
			if(!isEMPTY(acm->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,acm->name);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,acm->atype);
			if(!isEMPTY(acm->accname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,acm->accname);
			} else BINaddshort(bin,FALSE);
		}
	}
	if(rpt->display!=NULL)
	{
		d=rpt->display;
		BINaddshort(bin,TRUE);
		BINadddouble(bin,d->page_width);
		BINadddouble(bin,d->page_length);
		BINaddshort(bin,d->date_order);
		BINaddshort(bin,d->first_page_hdr);
		BINaddshort(bin,d->last_page_ftr);
		BINaddshort(bin,d->device);
		BINaddshort(bin,d->body_count);
		BINaddshort(bin,(d->count_invisible ? TRUE:FALSE));
		BINaddshort(bin,(d->doscopy? TRUE:FALSE));
		if(!isEMPTY(d->device_spec))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,d->device_spec);
		} else {
			BINaddshort(bin,FALSE);
		}
		BINaddshort(bin,(short)d->body_count_type);
		BINaddshort(bin,d->test_pages);
		BINaddshort(bin,d->starting_page);
		if(!isEMPTY(d->set_lpp))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,d->set_lpp);
		} else {
			BINaddshort(bin,FALSE);
		}
		if(d->set_lpp_eval==FALSE)
		{
			BINaddshort(bin,FALSE);
		} else {
			BINaddshort(bin,TRUE);
		}
		if(d->dumprinter==FALSE)
		{
			BINaddshort(bin,FALSE);
		} else {
			BINaddshort(bin,TRUE);
		}
		BINaddshort(bin,d->dumprinter_type);
		if(d->pvars==NULL) d->num=0;
		BINaddint(bin,d->num);
		if(d->num>0)
		{
			for(y=0,pvar=d->pvars;y<d->num;++y,++pvar)
			{
				if(!isEMPTY(pvar->name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,pvar->name);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(pvar->command))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,pvar->command);
				} else BINaddshort(bin,FALSE);
				BINaddshort(bin,pvar->chgsize);
				BINaddshort(bin,pvar->pitch);
			 	BINaddshort(bin,pvar->points);
			}
		}
	} else {
		BINaddshort(bin,FALSE);
	}
	BINaddint(bin,rpt->numimages);
	if(rpt->numimages>0)
	{
		for(y=0,img=rpt->image;y<rpt->numimages;++y,++img)
		{
			BINaddint(bin,img->type);
			if(!isEMPTY(img->breakname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,img->breakname);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(img->line))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,img->line);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,img->numpvals);
			for(z=0,pval=img->pvals;z<img->numpvals;++z,++pval)
			{
				BINaddshort(bin,pval->symbol_type);
				if(!isEMPTY(pval->name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,pval->name);
				} else BINaddshort(bin,FALSE);
				BINaddint(bin,pval->type);
				if(d!=NULL && d->device>2 && d->device!=5) 
				{
					if(!isEMPTY(pval->fieldname))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,pval->fieldname);
					} else BINaddshort(bin,FALSE);
				}
			}
			if(!isEMPTY(img->expression))
			{
				img->test=TRUE;
				BINaddshort(bin,img->test);
				BINaddstring(bin,img->expression);
			} else {
				img->test=FALSE;
				BINaddshort(bin,img->test);
			}
		}
	}
	if(rpt->MTNMSTR!=NULL && rpt->process_exceptions==TRUE)
	{
		BINaddshort(bin,TRUE);

		if(rpt->MTNMSTR->name!=NULL) Rfree(rpt->MTNMSTR->name);
		rpt->MTNMSTR->name=Rmalloc(RDAstrlen(rpt->name)+5);
		sprintf(rpt->MTNMSTR->name,"MTN %s",(rpt->name!=NULL?rpt->name:""));
		if(rpt->MTNMSTR->browse_screen!=NULL) Rfree(rpt->MTNMSTR->browse_screen);
		rpt->MTNMSTR->browse_screen=Rmalloc(RDAstrlen(rpt->name)+8);
		sprintf(rpt->MTNMSTR->browse_screen,"%s BROWSE",(rpt->name!=NULL?rpt->name:""));
		if(rpt->MTNMSTR->define_list!=NULL) Rfree(rpt->MTNMSTR->define_list);
		rpt->MTNMSTR->define_list=Rmalloc(RDAstrlen(rpt->name)+13);
		sprintf(rpt->MTNMSTR->define_list,"%s DEFINE LIST",(rpt->name!=NULL?rpt->name:""));
		if(rpt->MTNMSTR->search_browse!=NULL) Rfree(rpt->MTNMSTR->search_browse);
		rpt->MTNMSTR->search_browse=Rmalloc(RDAstrlen(rpt->name)+15);
		sprintf(rpt->MTNMSTR->search_browse,"%s SEARCH BROWSE",(rpt->name!=NULL?rpt->name:""));
		if(rpt->MTNMSTR->maintain_screen!=NULL) Rfree(rpt->MTNMSTR->maintain_screen);
		rpt->MTNMSTR->maintain_screen=Rmalloc(RDAstrlen(rpt->name)+17);
		sprintf(rpt->MTNMSTR->maintain_screen,"%s MAINTAIN SCREEN",(rpt->name!=NULL?rpt->name:""));
		writeMaintainMasterADV(fname,rpt->MTNMSTR,&bin);
	} else {
		BINaddshort(bin,FALSE);
	}
#ifndef WIN32
	fd=open(fname,O_WRONLY | O_CREAT | O_EXCL,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_WRONLY | O_CREAT | O_EXCL | O_BINARY,0666);
#endif
	if(fd==(-1))
	{
		prterr("Error Can't open file(%s)%s.",fname,errname());
		BINfree(bin);
		return(-1);
	}
	BINwrite(fd,bin);
	BINfree(bin);
	if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
	if(fname!=NULL) Rfree(fname);
	return(0);
}
static void SEERDAreport_header(RDA_PFILE *fp,int *lines,int *pages,
			char *header,/* char *module,*/
			char *file,short doheader,unsigned line_length)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	if(((*lines+1)>57) || (*pages==0 && *lines==1)) 
	{
	if(doheader==TRUE)
	{
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	if(*pages>0) RDA_fprintf(fp,"\f\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",header,tail_ws,"",10,++*pages);
	*lines+=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(9+RDAstrlen(file)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(9+RDAstrlen(file)))/2)-16)));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(file)+16+9+lead_ws+tail_ws)>(line_length))
	{
		RDA_fprintf(fp,"%*s Report %s %*sDate: %s\n",
			(int)(round(((double)(line_length-(8+RDAstrlen(file)))/2))),"",
			file,
			(int)(Rint((((double)(line_length-(8+RDAstrlen(file)))/2)-16))),"",
			date);
		*lines+=1;
	} else {
		RDA_fprintf(fp,"%*s Report %s %*sDate: %s\n",
			lead_ws,"",file,tail_ws,"",date);
		*lines+=1;
	}
/* 
	lead_ws=(int)(RDAroundup((double)(line_length-(17+RDAstrlen(module)+RDAstrlen(file)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(17+RDAstrlen(module)+RDAstrlen(file)))/2)-14)));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(module)+RDAstrlen(file)+16+17+lead_ws+tail_ws)>(line_length))
	{
		RDA_fprintf(fp,"%*s%s Module %*sDate: %s\n",
			(int)(round(((double)(line_length-(8+RDAstrlen(module)))/2))),"",
			module,
			(int)(Rint((((double)(line_length-(8+RDAstrlen(module)))/2)-16))),"",
			date);
		RDA_fprintf(fp,"%*s Report %s %*s\n",
			(int)(round(((double)(line_length-(8+RDAstrlen(file)))/2))),"",
			file,
			(int)(Rint((((double)(line_length-(8+RDAstrlen(file)))/2)-14))),"");
		*lines+=2;
	} else {
		RDA_fprintf(fp,"%*s %s Module %s Report %*sDate: %s\n",
			lead_ws,"",module,file,tail_ws,"",date);
		*lines+=1;
	}
*/
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
		*lines+=1;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		*lines+=1;
	}
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	}
	}
	RDA_fprintf(fp,"\n");
	*lines+=1;
}
/*
static void SEERDAreport_header(RDA_PFILE *fp,RDAreport *r,short print_head,int *pages,int *numpagelines)
{
	char *date=NULL,*timex=NULL; 

	if(print_head)
	{
		if(*pages==1 && *numpagelines==1)
		{
			date=GETCURRENTDATE10();
			timex=GETCURRENTTIME();
			RDA_fprintf(fp,"%*sXPERT REPORT DEFINITION              Page: %*d\n",28,"",8,*pages);
			RDA_fprintf(fp,"%*sName [%s]%*sDate: %s\n",
				((80-(8+RDAstrlen(r->name)))/2),"",
				r->name,r->title,
				(((80-(8+RDAstrlen(r->name)))/2)-16),"",
				date);
			RDA_fprintf(fp,"%*sTitle [%s]%*s\n",
				((80-(9+RDAstrlen(r->title)))/2),"",
				r->title,
				(((80-(9+RDAstrlen(r->name)))/2)-14),"");
			RDA_fprintf(fp,"%*s Executed By: %s%*sTime:   %s\n",
				((80-(14+RDAstrlen(USERLOGIN)))/2),"",
				USERLOGIN,
				(((80-(14+RDAstrlen(USERLOGIN)))/2)-16),"",
				timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
			*numpagelines=*numpagelines+4;
		}
	}
	RDA_fprintf(fp,"\n"); *numpagelines=*numpagelines+1;
	if(print_head)
	{
		if(*numpagelines>=60)
		{
			RDA_fprintf(fp,"\f"); *numpagelines=1; *pages=*pages+1;
			RDA_fprintf(fp,"%*sXPERT REPORT DEFINITION                   Page: %*d\n",28,"",4,*pages);
			RDA_fprintf(fp,"%*sName [%s]  Title [%s]\n",
				((80-(18+RDAstrlen(r->name)+
				RDAstrlen(r->title)))/2),"",r->name,
				r->title);
			RDA_fprintf(fp,"%*s Executed By: %s\n\n",
				((80-(14+RDAstrlen(USERLOGIN)))/2),"",USERLOGIN);
			*numpagelines=*numpagelines+5;
		}
	}
}
*/
static void fixrptout(RDA_PFILE *fp,char *data,RDAreport *r,short print_head,int *pages,int *numpagelines,unsigned line_length)
{
	unsigned y=0,x=0,length=0;
	char *temp=NULL,*temp1=NULL;
	char *header_value="XPERT REPORT DEFINITION";

	if(!isEMPTY(data))
	{
		temp=strchr(data,'\n');
		temp1=strchr(data,'\r');
		if(temp==NULL && temp1==NULL) length=RDAstrlen(data);
		else if(temp==NULL) length=temp1-data;
		else if(temp1==NULL) length=temp-data;
		else if(temp<temp1) length=temp-data;
		else length=temp1-data;
		for(x=0,y=0;x<length;++x,++y)
		{
			if(data[x]=='\t')
			{
				if(y+9>line_length) break;
				y+=7;
			}
			if(y+2>line_length) break;
		}
		if(x<length)
		{
			RDA_fprintf(fp,"%.*s",x+1,data);
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,print_head,line_length);
			fixrptout(fp,data+x+1,r,print_head,pages,numpagelines,line_length);
		} else {
			RDA_fprintf(fp,"%.*s",length,data);
			if(temp!=NULL || temp1!=NULL) fixrptout(fp,data+length+1,r,print_head,pages,numpagelines,line_length);
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,print_head,line_length);
		}
	} 
}
void SEERDAreport_detail(RDAreport *r,RDA_PFILE *fp,short rdaout,int *pages,int *numpagelines)
{
	int x,y,z,q,numlines=0;
	RDAsearch *s=NULL;
	RDAvirtual *v=NULL;
	RDAsort *sort=NULL;
	RDAimage *i=NULL;
	RDApval *pval=NULL;
	RDApvar *pvar=NULL;
	RDAcontrol *c=NULL;
	RDAaccum *acc=NULL;
	RDAaccval *accval=NULL;
	RDAdisplay *d=NULL;
	RDAconfld *confld=NULL;
	char *temp=NULL;
	unsigned line_length=79;
	char *header_value="XPERT REPORT DEFINITION";

	if(fp!=NULL)
	{
		RDA_fprintf(fp,"Engine Type: [%d] Stop On Error: [%s] No Empty Error: [%s] WriteCount Only [%s]",
			r->engine_type,
			(r->stop_on_error ? "Yes":"No"),
			(r->no_empty_error? "Yes":"No"),
			(r->no_wc_diag? "Yes":"No"));
/*
		SEERDAreport_header(fp,r,rdaout,pages,numpagelines);
*/
		SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
		RDA_fprintf(fp,"  All Levels [%s] No Write Count Total: [%s] Process Exceptions: [%s]",
			(r->write_all_detail_levels ? "Yes":"No"),
			(r->writecount_only ? "Yes":"No"),
			(r->process_exceptions ? "Yes":"No"));
		SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
		RDA_fprintf(fp,"  Ask Before Execute: [%s] Change Report: [%s] Module: [%s] Name: [%s]",(r->ask_b4_execute ? "Yes":"No"),(r->chain_report ? "Yes":"No"),(r->chain_module!=NULL ? r->chain_module:""),(r->chain_name!=NULL ? r->chain_name:""));
		SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
		RDA_fprintf(fp,"Range Screen: [%s] Diagnostic Level: [%d]",(r->range_screen ? "Yes":"No"),r->diag_breakable);
		SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
		RDA_fprintf(fp,"At Finish: [%s] Filename: [%s] Remove Sortfile: [%s]",REMOVE_LIST[r->remove_file],(r->remove_name!=NULL ? r->remove_name:""),(r->removesort ? "Yes":"No"));
		SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
		if(r->search!=NULL)
		{
			RDA_fprintf(fp,"Search Definitions:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			for(x=0,s=r->search;x<r->numsearchs;++x,++s)
			{
				RDA_fprintf(fp,"  Module: [%s]  Filename: [%s]  Number Writes: [%d]  Connect Type: [%d]",s->module,s->filename,s->num_writes,s->ctype);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				if(s->cmodule!=NULL)
				{
					RDA_fprintf(fp,"    C-Module: [%s]  C-Filename: [%s]",s->cmodule,s->cfilename);
					SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					RDA_fprintf(fp,"    SubKeyName:  [%s]",s->subkeyname);
					SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				}
				if(s->confld!=NULL)
				{
					for(y=0,confld=s->confld;y<s->num;++y,++confld)
					{
						RDA_fprintf(fp,"      Connecting Field: [%d]  Name: [%s]",y,(confld->fldname!=NULL?confld->fldname:""));
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						RDA_fprintf(fp,"      Connect Name: [%s]",(confld->conname!=NULL?confld->conname:""));
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					}
				}
				if(s->expression!=NULL)
				{
					temp=Rmalloc(19+RDAstrlen(s->expression));
					sprintf(temp,"    Expression: [%s]",s->expression);
					fixrptout(fp,temp,r,rdaout,pages,numpagelines,line_length);
					if(temp!=NULL) Rfree(temp);
				}
				if(s->skip_detail!=NULL)
				{
					temp=Rmalloc(20+RDAstrlen(s->skip_detail));
					sprintf(temp,"    Skip Detail: [%s]",s->skip_detail);
					fixrptout(fp,temp,r,rdaout,pages,numpagelines,line_length);
					if(temp!=NULL) Rfree(temp);
				}
				if(s->wc_expression!=NULL && s->wc_eval==TRUE)
				{
					temp=Rmalloc(20+RDAstrlen(s->wc_expression));
					sprintf(temp,"    Write Count Expression: [%s]",s->wc_expression);
					fixrptout(fp,temp,r,rdaout,pages,numpagelines,line_length);
					if(temp!=NULL) Rfree(temp);
				}
			}
		}
		if(r->virflds!=NULL)
		{
			RDA_fprintf(fp,"Virtual Field Definitions:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			for(x=0,v=r->virflds;x<r->numvirtuals;++x,++v)
			{
				RDA_fprintf(fp,"  Name: [%s]  Type: [%d]  When: [%d]  Length: [%d]",v->name,v->type,v->when,v->len);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				if(rdaout)
				{
					if(!isEMPTY(v->expression))
					{
						numlines=(int)(RDAstrlen(v->expression)/64);
						if((RDAstrlen(v->expression)%64)!=0) numlines++;
						for(z=0;z<numlines;++z)
						{
							if(temp==NULL)
							{
								temp=Rmalloc(66);
							} else {
								for(y=0;y<66;++y)
								{
									temp[y]=0;
								}
							}
							for(y=0;y<64;++y)
							{
								temp[y]=v->expression[z*64+y];
								if(temp[y]=='\n') temp[y]=' ';
								else if(temp[y]=='\t') temp[y]=' ';
								else if(temp[y]=='\v') temp[y]=' ';
								else if(temp[y]=='\b') temp[y]=' ';
								else if(temp[y]=='\r') temp[y]=' ';
								else if(temp[y]=='\a') temp[y]=' ';
								else if(temp[y]=='\f') temp[y]=' ';
								if(v->expression[z*64+y+1]=='\0')
								{
									temp[y+1]=0;
									break;
								}
							}
							if(z==0)
							{
								RDA_fprintf(fp,"    Expression: %s",temp);
							} else {
								RDA_fprintf(fp,"                %s",temp);
							}
							SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						}
						if(temp!=NULL) Rfree(temp);
					} else {
						RDA_fprintf(fp,"    Expression:");
					}
				} else {
					if(!isEMPTY(v->expression))
					{
						RDA_fprintf(fp,"    Expression:");
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						RDA_fprintf(fp,"%s",v->expression);
					} else {
						RDA_fprintf(fp,"    Expression:");
					}
				}
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			}
		}
		if(r->select!=NULL)
		{
			RDA_fprintf(fp,"Select Record Criteria:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			if(rdaout)
			{
				if(!isEMPTY(r->select))
				{
					numlines=(int)(RDAstrlen(r->select)/76);
					if((RDAstrlen(r->select)%76)!=0) numlines++;
					for(z=0;z<numlines;++z)
					{
						if(temp==NULL)
						{
							temp=Rmalloc(78);
						} else {
							for(y=0;y<78;++y)
							{
								temp[y]=0;
							}
						}
						for(y=0;y<76;++y)
						{
							temp[y]=r->select[z*76+y];
							if(temp[y]=='\n') temp[y]=' ';
							else if(temp[y]=='\t') temp[y]=' ';
							else if(temp[y]=='\v') temp[y]=' ';
							else if(temp[y]=='\b') temp[y]=' ';
							else if(temp[y]=='\r') temp[y]=' ';
							else if(temp[y]=='\a') temp[y]=' ';
							else if(temp[y]=='\f') temp[y]=' ';
							if(r->select[z*76+y+1]=='\0')
							{
								temp[y+1]=0;
								break;
							}
						}
						RDA_fprintf(fp,"  %s",temp);
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					}
					if(temp!=NULL) Rfree(temp);
				}
			} else {
				if(!isEMPTY(r->select))
				{
					RDA_fprintf(fp,"%s",r->select);
				}
			}
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
		}
		if(r->sort!=NULL)
		{
			RDA_fprintf(fp,"Sort Definition:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			for(x=0,sort=r->sort;x<r->numsorts;++x,++sort)
			{
				RDA_fprintf(fp,"  Sort Name: [%s] Type: [%d]",
					sort->name,sort->type);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			}
		}
		if(r->control!=NULL)
		{
			RDA_fprintf(fp,"Control Break Definitions:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			for(x=0,c=r->control;x<r->numcontrols;++x,++c)
			{
				RDA_fprintf(fp,"  Control Break Name: [%s]",c->name);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			}
		}
		if(r->accum!=NULL)
		{
			RDA_fprintf(fp,"Accumulator Definitions:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			for(x=0,acc=r->accum;x<r->numaccums;++x,++acc)
			{
				RDA_fprintf(fp,"  Accumulator Name: [%s] Acc Type: [%d]",acc->name,acc->atype);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				RDA_fprintf(fp,"    Field Name to Accumulate: [%s]",acc->accname);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				RDA_fprintf(fp,"      Field Type: [%d]  Len: [%d]",acc->fieldtype,acc->len);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				if(acc->accvals!=NULL)
				{
					for(y=0,accval=acc->accvals;y<acc->num;++y,++accval)
					{
						RDA_fprintf(fp,"        BreakName: [%s]",accval->breakname);
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					}
				}
			}
		}
		if(r->display!=NULL)
		{
			d=r->display;
			RDA_fprintf(fp,"Display Definitions:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			RDA_fprintf(fp,"  Page Width: [%f]  Length: [%f] Body Count Type: [%d][%s] Body Countx: [%d]",d->page_width,d->page_length,d->body_count_type,(d->body_count_type==FALSE?"FALSE":"TRUE"),d->body_count);
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			RDA_fprintf(fp,"  Test Pages: [%d]  Starting Page: [%d]",d->test_pages,d->starting_page);
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			RDA_fprintf(fp,"  Device Type:[%d] Name:[%s]",d->device,
				(d->set_lpp!=NULL ? d->set_lpp:""));
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			if(d->pvars!=NULL)
			{
				for(x=0,pvar=d->pvars;x<d->num;++x,++pvar)
				{
					RDA_fprintf(fp,"    Name: [%s]  Command: [%s]  Change Size: [%s]  ",pvar->name,pvar->command,(pvar->chgsize ? "Yes":"No"));
					SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					if(pvar->chgsize)
					{
						RDA_fprintf(fp,"      Pitch: [%d]  Points: [%d]",
							pvar->pitch,pvar->points);
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					}
				}
			}
		}
		if(r->image!=NULL)
		{
			RDA_fprintf(fp,"Report Images:");
			SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
			for(x=0,i=r->image;x<r->numimages;++x,++i)
			{
				RDA_fprintf(fp,"  Image Line: [%d]  BreakName: [%s]",x,i->breakname);
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				if(rdaout)
				{
					numlines=(int)(RDAstrlen(i->line)/68);
					if((RDAstrlen(i->line)%68)!=0) numlines++;
					for(z=0;z<numlines;++z)
					{
						if(temp!=NULL) Rfree(temp);
						temp=Rmalloc(69);
						temp[68]='\0';
						for(y=0;y<68;++y)
						{
							temp[y]=i->line[z*68+y];
							if(temp[y]=='\n') temp[y]=' ';
							if(i->line[z*68+y+1]=='\0') break;
						}
						if(z==0)
						{
							RDA_fprintf(fp,"    Line: [%s]",temp);
							SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						} else {
							RDA_fprintf(fp,"          [%s]",temp);
							SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						}
						if(temp!=NULL) Rfree(temp);
					}
				} else {
					RDA_fprintf(fp,"    Line:");
					SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					RDA_fprintf(fp,"%s",i->line);
					SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
					if(!isEMPTY(i->expression))
					{
						RDA_fprintf(fp,"    Expression:");
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						RDA_fprintf(fp,"%s",i->expression);
					} else {
						RDA_fprintf(fp,"    Expression:");
					}
				}
				SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
				if(i->pvals!=NULL)
				{
					for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
					{
						RDA_fprintf(fp,"    RDApval: [%d]  Symbol: [%d]  ",y,pval->symbol_type);
						SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						if(pval->name!=NULL)
						{
							if(rdaout)
							{
								numlines=(int)(RDAstrlen(pval->name)/66);
								if((RDAstrlen(pval->name)%66)!=0) numlines++;
								for(z=0;z<numlines;++z)
								{
									if(temp!=NULL) Rfree(temp);
									temp=Rmalloc(67);
									temp[66]='\0';
									for(q=0;q<66;++q)
									{
										temp[q]=pval->name[z*66+q];
										if(temp[q]=='\n') temp[q]=' ';
										if(pval->name[z*66+q+1]=='\0') break;
									}
									if(z==0) RDA_fprintf(fp,"      Name: [%s]",temp);
									else     RDA_fprintf(fp,"            [%s]",temp);
									if(temp!=NULL) Rfree(temp);
								}
							} else {
								RDA_fprintf(fp,"      Name:  %s",pval->name);
							}
							SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						}
						if(pval->fieldname!=NULL)
						{
							if(rdaout)
							{
								numlines=(int)(RDAstrlen(pval->fieldname)/58);
								if((RDAstrlen(pval->fieldname)%58)!=0) numlines++;
								for(z=0;z<numlines;++z)
								{
									if(temp!=NULL) Rfree(temp);
									temp=Rmalloc(59);
									temp[58]='\0';
									for(q=0;q<58;++q)
									{
										temp[q]=pval->fieldname[z*58+q];
										if(temp[q]=='\n') temp[q]=' ';
										if(pval->fieldname[z*58+q+1]=='\0') break;
									}
									if(z==0) RDA_fprintf(fp,"        Field Name: [%s]",temp);
									else     RDA_fprintf(fp,"                    [%s]",temp);
									if(temp!=NULL) Rfree(temp);
								}
							} else {
								RDA_fprintf(fp,"        Field Name:   %s",pval->fieldname);
							}
							SEERDAreport_header(fp,numpagelines,pages,header_value,r->name,rdaout,line_length);
						}
					}
				}
			}
		}
	}
}
static void printRDAreport(RDArsrc *prsrc,RDAreport *r)
{
	int x,y,pages=0,numlines=0,numpagelines=1;
	char *outdevice=NULL,*temp=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;
	char *header_value="XPERT REPORT DEFINITION";
	unsigned line_length=79;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
                if(fp!=NULL)
                {
			SelectPrintType(outdevice,print_style,fp);
			if(outdevice!=NULL) Rfree(outdevice);
			SEERDAreport_header(fp,&numpagelines,&pages,header_value,r->name,TRUE,line_length);
			RDA_fprintf(fp,"Title: %s",r->title);
			SEERDAreport_header(fp,&numpagelines,&pages,header_value,r->name,TRUE,line_length);
			if(RDAstrlen(r->desc)<=line_length)
			{
				RDA_fprintf(fp,"Description:");
				SEERDAreport_header(fp,&numpagelines,&pages,header_value,r->name,TRUE,line_length);
				RDA_fprintf(fp,"%*s%s"
					,((line_length-(RDAstrlen(r->desc)))/2),""
					,r->desc);
				SEERDAreport_header(fp,&numpagelines,&pages,header_value,r->name,TRUE,line_length);
			} else {
				RDA_fprintf(fp,"Description:");
				SEERDAreport_header(fp,&numpagelines,&pages,header_value,r->name,TRUE,line_length);
				numlines=(int)(RDAstrlen(r->desc)/(int)(line_length-2));
				if((RDAstrlen(r->desc)%(int)(line_length-2))!=0) numlines++;
				for(x=0;x<numlines;++x)
				{
					temp=Rmalloc(line_length-1);
					temp[line_length-1]='\0';
					for(y=0;y<(line_length-1);++y)
					{
						temp[y]=r->desc[x*(line_length-1)+y];
						if(temp[y]=='\n') temp[y]=' ';
						if(r->desc[x*(line_length-1)+y+1]=='\0') break;
					}
					RDA_fprintf(fp,"  %s",temp);
					SEERDAreport_header(fp,&numpagelines,&pages,header_value,r->name,TRUE,line_length);
					if(temp!=NULL) Rfree(temp);
				}
			}
			SEERDAreport_detail(r,fp,TRUE,&pages,&numpagelines);
			RDA_pclose(fp);
		} else {
			ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
			prterr("Error PrintRDAreport popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
			if(outdevice!=NULL) Rfree(outdevice);
		}
	} else {
		ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
	}
}
static void quit_printRDAreport(RDArsrc *prsrc)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG quit_printRDAreport Quitting Print RDA Report"); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	free_rsrc(prsrc);
}	
static void GetDevicePrintRDAreport(RDAreport *r)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW("RPTGEN","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAreport,r);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAreport,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
void xSEERDAreport(RDAreport *r,short rdaout,int line,char *file)
{
/*
	int pages=1,numpagelines=1;
*/

	if(r==NULL)
	{
		prterr("Error SEERDAreport has no Report Definition, i.e. the RDAreport * is NULL prior to line [%d] program [%s].",line,file);
		return;
	}
	if(rdaout)
	{
		GetDevicePrintRDAreport(r);
	} else {
		prterr("DIAG SEERDAreport Show Structure for Report [%s] at line [%d] program [%s].",r->name,line,file);
/*
		SEERDAreport_detail(r,RDA_STDERR,FALSE,&pages,&numpagelines);
*/
	}
}
char *getreporttitle(char *module,char *name)
{
	int fd;
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL,*hold=NULL;
	short x,test=0,version=FALSE;
	char *test1=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG getreporttitle Getting Report Title for Module [%s] Report [%s].",module,name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(name)+7+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.RPT",CURRENTDIRECTORY,module,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.RPT",CURRENTDIRECTORY,module,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		prterr("Error in permissions, user can't read report file [%s].",fname);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd==(-1))
	{
		prterr("Error Can't open file(%s)%s.",fname,errname());
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=BINread(fd);
	if(mainbin==NULL)
	{
		prterr("Error Can't read file(%s).",fname);
		if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1681) && x!=(-1682) && x!=(-1683) && 
		x!=(-1684) && x!=(-1685) && x!=(-1686) && x!=(-1687) && 
		x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && 
		x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1695) && 
		x!=(-1696) && x!=(-1697) && x!=(-1701) && x!=(-1702) && 
		x!=(-1703) && x!=(-1704) && x!=(-1699) && x!=(-1698))
	{
		prterr("Error Invalid version or bad binary in Module [%s] Report [%s].  Report definition may be corrupted.",module,name);
		BINfree(mainbin);
		return(NULL);
	}
	if(x==(-1681)) version=22;
	else if(x==(-1682)) version=21;
	else if(x==(-1683)) version=20;
	else if(x==(-1684)) version=19;
	else if(x==(-1685)) version=18;
	else if(x==(-1686)) version=17;
	else if(x==(-1687)) version=16;
	else if(x==(-1688)) version=15;
	else if(x==(-1689)) version=14;
	else if(x==(-1690)) version=13;
	else if(x==(-1691)) version=12;
	else if(x==(-1692)) version=11;
	else if(x==(-1693)) version=10;
	else if(x==(-1694)) version=9;
	else if(x==(-1695)) version=8;
	else if(x==(-1696)) version=7;
	else if(x==(-1697)) version=6;
	else if(x==(-1702)) version=1;
	else if(x==(-1703)) version=2;
	else if(x==(-1704)) version=3;
	else if(x==(-1699)) version=4;
	else if(x==(-1698)) version=5;
	else version=FALSE;
	if(version>2)
	{
		test=BINgetshort(mainbin);
		if(test) test1=BINgetstring(mainbin);
			else test1=NULL;
	} else test1=BINgetstring(mainbin);
	if(test1!=NULL) Rfree(test1);
	test1=NULL;
	if(version)
	{
		test=BINgetshort(mainbin);
		if(test) hold=BINgetstring(mainbin);
			else hold=NULL;
	}
	BINfree(mainbin);
	if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
	if(fname!=NULL) Rfree(fname);
	return(hold);
}
char *getreportdesc(char *module,char *name)
{
	int fd;
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL,*hold=NULL;
	short x,test=0,version=FALSE;
	char *test1=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG getreportdesc Getting Report Description for Module [%s] Report [%s].",module,name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+RDAstrlen(name)+7+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(name);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.RPT",CURRENTDIRECTORY,module,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.RPT",CURRENTDIRECTORY,module,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		prterr("Error in permissions, user can't read report file [%s]].",fname);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd==(-1))
	{
		prterr("Error Can't open file(%s)%s.",fname,errname());
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=BINread(fd);
	if(mainbin==NULL)
	{
		prterr("Error Can't read file(%s).",fname);
		if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1681) && x!=(-1682) && x!=(-1683) && 
		x!=(-1684) && x!=(-1685) && x!=(-1686) && x!=(-1687) && 
		x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && 
		x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1695) && 
		x!=(-1696) && x!=(-1697) && x!=(-1701) && x!=(-1702) && 
		x!=(-1703) && x!=(-1704) && x!=(-1699) && x!=(-1698))
	{
		prterr("Error Invalid version or bad binary for Module [%s] Report [%s].  Report definition may be corrupted.",module,name);
		BINfree(mainbin);
		return(NULL);
	}
	if(x==(-1681)) version=22;
	else if(x==(-1682)) version=21;
	else if(x==(-1683)) version=20;
	else if(x==(-1684)) version=19;
	else if(x==(-1685)) version=18;
	else if(x==(-1686)) version=17;
	else if(x==(-1687)) version=16;
	else if(x==(-1688)) version=15;
	else if(x==(-1689)) version=14;
	else if(x==(-1690)) version=13;
	else if(x==(-1691)) version=12;
	else if(x==(-1692)) version=11;
	else if(x==(-1693)) version=10;
	else if(x==(-1694)) version=9;
	else if(x==(-1695)) version=8;
	else if(x==(-1696)) version=7;
	else if(x==(-1697)) version=6;
	else if(x==(-1702)) version=1;
	else if(x==(-1703)) version=2;
	else if(x==(-1704)) version=3;
	else if(x==(-1699)) version=4;
	else if(x==(-1698)) version=5;
	else version=FALSE;
	if(version>2)
	{
		test=BINgetshort(mainbin);
		if(test) test1=BINgetstring(mainbin);
			else test1=NULL;
	} else test1=BINgetstring(mainbin);
	if(test1!=NULL) Rfree(test1);
	test1=NULL;
	if(version)
	{
		test=BINgetshort(mainbin);
		if(test) test1=BINgetstring(mainbin);
			else test1=NULL;
		if(test1!=NULL) Rfree(test1);
		test1=NULL;
		test=BINgetshort(mainbin);
		if(test) hold=BINgetstring(mainbin);
			else hold=NULL;
	}
	BINfree(mainbin);
	if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
	if(fname!=NULL) Rfree(fname);
	return(hold);
}
MaintainMaster *xReportMaintainMasterNew(char *module,char *name,RDAreport *report,int line,char *file)
{
	char *temp=NULL;
	MaintainMaster *MTNMSTR=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG ReportMaintainMasterNew() Setting Report Maintain Master for Module [%s] Report [%s] at line [%d] file [%d].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	temp=Rmalloc(RDAstrlen(name)+5);
	sprintf(temp,"MTN %s",(name!=NULL?name:""));
	MTNMSTR=MaintainMasterNew(module,temp);
	if(temp!=NULL) Rfree(temp);

	MTNMSTR->desc=stralloc(report->desc);
	MTNMSTR->style=2;
	MTNMSTR->mainfile=stralloc("SORTFILE");
	MTNMSTR->browse_screen=Rmalloc(RDAstrlen(name)+8);
	sprintf(MTNMSTR->browse_screen,"%s BROWSE",(name!=NULL?name:""));
	MTNMSTR->define_list=Rmalloc(RDAstrlen(name)+13);
	sprintf(MTNMSTR->define_list,"%s DEFINE LIST",(name!=NULL?name:""));
	MTNMSTR->search_browse=Rmalloc(RDAstrlen(name)+15);
	sprintf(MTNMSTR->search_browse,"%s SEARCH BROWSE",(name!=NULL?name:""));
	MTNMSTR->maintain_screen=Rmalloc(RDAstrlen(name)+17);
	sprintf(MTNMSTR->maintain_screen,"%s MAINTAIN SCREEN",(name!=NULL?name:""));
	MTNMSTR->file_type=1;
	MTNMSTR->start_with=1;
	MTNMSTR->start_where=0;
	MTNMSTR->passparent=NULL;
	return(MTNMSTR);
}
