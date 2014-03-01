/* runadd.c - Run Power Add */
/*COMPMEM 36864 */
/*lint -library */
#include <powadd.hpp>
#include <rptgen.hpp>
#include <mix.hpp>
#include <gui.hpp>
#include <guip.hpp>

#define REPLICATE	50
static void doexit(PowerAdd *);
static void save_check(RDArsrc *,PowerAdd *,short);
static void save_record_now(RDArsrc *,PowerAdd *);
static void save_all_now(RDArsrc *,PowerAdd *);
static void save_all_now_test(RDArsrc *,PowerAdd *);
static void save_record(RDArsrc *,PowerAdd *,short);
static void quit_record(RDArsrc *,PowerAdd *);
static void quit_test(RDArsrc *,PowerAdd *);
static void readscreen(RDArsrc *,PowerAdd *);
static void resetdefs(RDArsrc *,PowerAdd *);
static void SETFILES(RDArsrc *,AddEditWdgt *);
static void PerformWrites(RDArsrc *,PowerAdd *);
static void AfterWrites(RDArsrc *,PowerAdd *,short);
static void SETNEWIDANDFILES(RDArsrc *,AddEditWdgt *);
static void INCREFNUM(RDArsrc *,AddEditWdgt *);
void setpadddeftype(RDArmem *,PowerAdd *);
void setpaddacctype(RDArmem *,PowerAdd *);
void doadd(int,void (*)(...),RDArsrc *,void *); 
static short check_row(RDArsrc *,PowerAdd *,short,short);
static short check_row_quick(RDArsrc *,PowerAdd *,short,short);
static short UsingCurrentRow(RDArsrc *,PowerAdd *);
static void deletethisrow(RDArmem *,PowerAdd *);

static void CopyRow2Primary(PowerAdd *POWERADD)
{
	int x=0;
	AddEditWdgt *EW=NULL,*EW2=NULL;

	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(!RDAstrcmp(EW->name,"CURRENT ROW"))
		{
			*EW->data.integer_value=POWERADD->table_row+1;
			FINDRSCSETINT(POWERADD->addrsrc,EW->name,*EW->data.integer_value);
		}
		if(EW->loadvirtual==2 || EW->loadvirtual==3)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EW->name);
			EW2=FINDEDITWDGT(POWERADD,stemp);		
			if(EW2!=NULL)
			{
				switch(EW2->type)
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
						if(EW2->data.string_value!=NULL && EW2->dlen>0)
						{
							QUICKALLOC(EW->data.string_value,EW->dlen,EW2->dlen);
							memcpy(EW->data.string_value,EW2->data.string_value,EW2->dlen);
							if(EW->dlen>EW->len && EW->len>0)
								EW->data.string_value[EW->len]=0;
						} else if(EW->dlen>0)
						{
							memset(EW->data.string_value,0,EW->dlen);
						} else {
							EW->data.string_value=NULL;
							EW->dlen=0;
						}
						FINDRSCSETSTRING(POWERADD->addrsrc,EW->name,EW->data.string_value);
						break;
					case BOOLNS:
						*EW->data.string_value=*EW2->data.string_value;
						FINDRSCSETCHAR(POWERADD->addrsrc,EW->name,*EW->data.string_value);
						break;
					case CHARACTERS:
						*EW->data.string_value=*EW2->data.string_value;
						FINDRSCSETCHAR(POWERADD->addrsrc,EW->name,*EW->data.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*EW->data.float_value=*EW2->data.float_value;
						FINDRSCSETDOUBLE(POWERADD->addrsrc,EW->name,*EW->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						*EW->data.short_value=*EW2->data.short_value;
						FINDRSCSETSHORT(POWERADD->addrsrc,EW->name,*EW->data.short_value);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						*EW->data.integer_value=*EW2->data.integer_value;
						FINDRSCSETINT(POWERADD->addrsrc,EW->name,*EW->data.integer_value);
						break;
					default:
					case BUTTONS:
						break;
				}
			}
		}
	}
}
static void SetReplicate2EditWgt(PowerAdd *POWERADD)
{
	int x=0,y=0,z=0;
	AddEditWdgt *EW=NULL,*EWR=NULL;
	RDArmem *mem=NULL,*Rmem=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG SetReplicate2EditWdgt on Row [%d] ",POWERADD->table_row+1);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(EW->loadvirtual==2 || EW->loadvirtual==3)
		{
/* Copy Replicated Widdgets to Store */
			memset(stemp,0,101);
			sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EW->name);
			z=FINDRSC(POWERADD->addrsrc,stemp);
			if(z>(-1)) Rmem=POWERADD->addrsrc->rscs+z;
			EWR=FINDEDITWDGT(POWERADD,stemp);
			if(EWR!=NULL)
			{
				memset(stemp,0,101);
				sprintf(stemp,"STORE:%s",EW->name);
				y=FINDRSC(POWERADD->addrsrc,stemp);
				if(y>(-1)) mem=POWERADD->addrsrc->rscs+y;
				switch(Rmem->field_type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case ZIPCODE:
						break;
					case CUSTOMTYPE:
						MEMBERSETACCTTYPE(mem,CUSTOM_ACCT);
						break;
					case EXPENDITURE:
						MEMBERSETACCTTYPE(mem,EXP_ACCT);
						break;
					case REVENUE:
						MEMBERSETACCTTYPE(mem,REV_ACCT);
						break;
					case BALANCESHEET:
						MEMBERSETACCTTYPE(mem,BAL_ACCT);
						break;
					case BEGINNINGBALANCE:
						MEMBERSETACCTTYPE(mem,BEG_ACCT);
						break;
					case SHORTV:
					case SSHORTV:
						break;
					case SLONGV:
					case SCROLLEDLIST:
					case LONGV:
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						break;
					case BOOLNS:
						break;
					case CHARACTERS:
						break;
					default:
						break;
				}
				COPYRSRC2RSRC(POWERADD->addrsrc,EWR->name,stemp);
			}
		}
	}
	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(EW->loadvirtual==2 || EW->loadvirtual==3)
		{
/* Copy STORE Widgets to Primary */
			memset(stemp,0,101);
			sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EW->name);
			EWR=FINDEDITWDGT(POWERADD,stemp);
			if(EWR!=NULL)
			{
				memset(stemp,0,101);
				sprintf(stemp,"STORE:%s",EW->name);
				COPYRSRC2RSRC(POWERADD->addrsrc,stemp,EW->name);
				y=FINDRSC(POWERADD->addrsrc,EW->name);
				if(y>(-1)) 
				{
					mem=POWERADD->addrsrc->rscs+y;
					updatemember(mem);
					ExecuteRDArmemFunction(mem);
				}
			}
		}
	}
	updateallrsrc(POWERADD->addrsrc);
	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(EW->loadvirtual==2 || EW->loadvirtual==3)
		{
/* Copy STORE Widgets to Primary */
			memset(stemp,0,101);
			sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EW->name);
			EWR=FINDEDITWDGT(POWERADD,stemp);
			if(EWR!=NULL)
			{
				memset(stemp,0,101);
				sprintf(stemp,"STORE:%s",EW->name);
				y=FINDRSC(POWERADD->addrsrc,stemp);
				if(y>(-1))
				{
					mem=POWERADD->addrsrc->rscs+y;
					switch(mem->field_type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case BOOLNS:
						case CHARACTERS:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(!isEMPTY(mem->value.string_value))
							{
								memset(mem->value.string_value,0,mem->dlen);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							*mem->value.float_value=0.0;
							break;
						case SHORTV:
						case SSHORTV:
							*mem->value.short_value=0;
							break;
						case PROGRESS_BAR:
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							*mem->value.integer_value=0;
							break;
						case BUTTONS:
							break;
						default:
							prterr("Error field type [%s] invalid for Resource [%s] on Screen [%s] [%s].",mem->field_type,mem->rscrname,POWERADD->addrsrc->module,POWERADD->addrsrc->screen);
							break;
					}
				}
			}
		}
	}
}
static void currentrow(RDArmem *m,PowerAdd *POWERADD)
{
	RDArsrc *r=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG currentrow beginning.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	r=(RDArsrc *)m->parent;
	check_row(r,POWERADD,TRUE,FALSE);
	readmember(m);
	FINDRSCGETINT(r,"CURRENT ROW",&x);
	if(x<1 || x>REPLICATE) 
	{
		x=1;
		FINDRSCSETINT(r,"CURRENT ROW",x);
		updatersrc(r,"CURRENT ROW");
	}
	POWERADD->table_row=x-1;
	SetReplicate2EditWgt(POWERADD);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG currentrow ending.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
static void ReplicateEditWdgts(PowerAdd *POWERADD)
{
	short x,y,z,w,last=0;
	int NumReplicate=0;
	AddEditWdgt *temp,*temp1,*new_fields,*prev=NULL;
	RDAeditfld *confld1,*confld2;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG ReplicateEditWdgts.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	for(x=0,temp=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++temp)
	{
		if(temp->loadvirtual==2 || temp->loadvirtual==3) NumReplicate+=REPLICATE;
	}
	POWERADD->numwdgts=POWERADD->numwdgts+NumReplicate;
	new_fields=Rmalloc(POWERADD->numwdgts*sizeof(AddEditWdgt));
	y=0;
	for(x=0,temp=POWERADD->EditWdgts;x<(POWERADD->numwdgts-NumReplicate);++x,++temp)
	{
		temp1=new_fields+y;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->len=temp->len;
		temp1->etype=temp->etype;
		temp1->id_mgt=temp->id_mgt;
		temp1->loadvirtual=temp->loadvirtual;
		temp1->setaccttype=temp->setaccttype;
		temp1->setdeftype=temp->setdeftype;
		if(temp->acctrsc!=NULL)
		{
			temp1->acctrsc=APPlibNEW();
			for(z=0;z<temp->acctrsc->numlibs;++z)
			{
				addAPPlib(temp1->acctrsc,temp->acctrsc->libs[z]);
			}
			freeapplib(temp->acctrsc);
		} else temp1->acctrsc=NULL;
		if(temp->defrsc!=NULL)
		{
			temp1->defrsc=APPlibNEW();
			for(z=0;z<temp->defrsc->numlibs;++z)
			{
				addAPPlib(temp1->defrsc,temp->defrsc->libs[z]);
			}
			freeapplib(temp->defrsc);
		} else temp1->defrsc=NULL;
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;	
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		if(temp->confld!=NULL && temp->num>0)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAeditfld));
			for(z=0,confld1=temp->confld,confld2=temp1->confld;
				z<temp->num;++z,++confld1,++confld2)
			{
				if(confld1->fldname!=NULL)
				{
					confld2->fldname=stralloc(confld1->fldname);
				} else confld2->fldname=NULL;
				if(confld1->conname!=NULL)
				{
					confld2->conname=stralloc(confld1->conname);
				} else confld2->conname=NULL;
			}
		}
		temp1->num=temp->num;
		if(temp->mgt_expression!=NULL)
		{
			temp1->mgt_expression=stralloc(temp->mgt_expression);
			Rfree(temp->mgt_expression);
		} else temp1->mgt_expression=NULL;
		if(temp->assignname!=NULL)
		{
			temp1->assignname=stralloc(temp->assignname);
			Rfree(temp->assignname);
		} else temp1->assignname=NULL;
		if(temp->id!=NULL)
		{
			temp1->id=stralloc(temp->id);
			Rfree(temp->id);
		} else temp1->id=NULL;
		if(temp->override_expression!=NULL)
		{
			temp1->override_expression=stralloc(temp->override_expression);
			Rfree(temp->override_expression);
		} else temp1->override_expression=NULL;
		temp1->fileno=temp->fileno;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->loadvirtual=temp->loadvirtual;
		temp1->parent=POWERADD;
		last=y;
		++y;
		if(temp1->loadvirtual==2 || temp1->loadvirtual==3)
		{
			for(w=1;w<(REPLICATE+1);++w)
			{
				prev=new_fields+last;
				temp1=new_fields+y;
				memset(stemp,0,101);
				sprintf(stemp,"%04d:%s",w,prev->name);
				temp1->name=stralloc(stemp);
				temp1->type=prev->type;
				temp1->len=prev->len;
				temp1->etype=FALSE;
				temp1->id_mgt=prev->id_mgt;
				temp1->loadvirtual=4;
				temp1->setaccttype=FALSE;
				temp1->setdeftype=FALSE;
				temp1->acctrsc=NULL;
				temp1->defrsc=NULL;
			 	temp1->module=NULL;	
				temp1->filename=NULL;
				temp1->subkeyname=stralloc(prev->subkeyname);
				temp1->confld=NULL;
				temp1->num=0;
				temp1->mgt_expression=stralloc(prev->mgt_expression);
				temp1->assignname=stralloc(prev->assignname);
				temp1->id=stralloc(prev->id);
				temp1->override_expression=stralloc(prev->override_expression);
				temp1->fileno=prev->fileno;
				temp1->expression=NULL;
				temp1->parent=POWERADD;
				++y;
			}
		}
	}
	Rfree(POWERADD->EditWdgts);
	POWERADD->EditWdgts=Rmalloc(POWERADD->numwdgts*sizeof(AddEditWdgt));
	for(x=0,temp1=POWERADD->EditWdgts,temp=new_fields;x<POWERADD->numwdgts;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->len=temp->len;
		temp1->loadvirtual=temp->loadvirtual;
		temp1->etype=temp->etype;
		temp1->id_mgt=temp->id_mgt;
		temp1->setaccttype=temp->setaccttype;
		temp1->setdeftype=temp->setdeftype;
		if(temp->acctrsc!=NULL)
		{
			temp1->acctrsc=APPlibNEW();
			for(z=0;z<temp->acctrsc->numlibs;++z)
			{
				addAPPlib(temp1->acctrsc,temp->acctrsc->libs[z]);
			}
			freeapplib(temp->acctrsc);
		} else temp1->acctrsc=NULL;
		if(temp->defrsc!=NULL)
		{
			temp1->defrsc=APPlibNEW();
			for(z=0;z<temp->defrsc->numlibs;++z)
			{
				addAPPlib(temp1->defrsc,temp->defrsc->libs[z]);
			}
			freeapplib(temp->defrsc);
		} else temp1->defrsc=NULL;
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		if(temp->confld!=NULL && temp->num>0)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAeditfld));
			for(z=0,confld1=temp->confld,confld2=temp1->confld;
				z<temp->num;++z,++confld1,++confld2)
			{
				if(confld1->fldname!=NULL)
				{
					confld2->fldname=stralloc(confld1->fldname);
				} else confld2->fldname=NULL;
				if(confld1->conname!=NULL)
				{
					confld2->conname=stralloc(confld1->conname);
				} else confld2->conname=NULL;
			}
			temp1->num=temp->num;
		} else {
			temp1->num=0;
		}
		if(temp->mgt_expression!=NULL)
		{
			temp1->mgt_expression=stralloc(temp->mgt_expression);
			Rfree(temp->mgt_expression);
		} else temp1->mgt_expression=NULL;
		if(temp->assignname!=NULL)
		{
			temp1->assignname=stralloc(temp->assignname);
			Rfree(temp->assignname);
		} else temp1->assignname=NULL;
		if(temp->id!=NULL)
		{
			temp1->id=stralloc(temp->id);
			Rfree(temp->id);
		} else temp1->id=NULL;
		if(temp->override_expression!=NULL)
		{
			temp1->override_expression=stralloc(temp->override_expression);
			Rfree(temp->override_expression);
		} else temp1->override_expression=NULL;
		temp1->fileno=temp->fileno;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->loadvirtual=temp->loadvirtual;
		temp1->parent=POWERADD;
	}
	Rfree(new_fields);
}
static void Set_Skip_Widgets(PowerAdd *p)
{
	RDAaddsub *addsub=NULL,*addsub1=NULL;
	int x,y;
	short count=0;

	if(p->subord!=NULL)
	{
		for(x=0,addsub=p->subord;x<p->subnum;++x,++addsub)
		{
			count=0;
			for(y=0,addsub1=p->subord;y<p->subnum;++y,++addsub1)
			{
				if(!RDAstrcmp(addsub->module,addsub1->module) && 
					!RDAstrcmp(addsub->filename,addsub1->filename))
				{
					 ++count;
					if(count>1) addsub1->skip_widget=TRUE;
				}
			}
		}
	}
}
void PP_translate_POWERADD(PowerAdd *p)
{
	AddEditWdgt *AE=NULL;
	int x,y;
	char *temp=NULL;
	AddWrite *AW=NULL;
	AddWriteField *AWF=NULL;
	MaintainButton *BTN=NULL;
	RDAaddsub *addsub=NULL;

	if(p!=NULL)
	{
		if(p->subord!=NULL)
		{
			for(x=0,addsub=p->subord;x<p->subnum;++x,++addsub)
			{
				if(!isEMPTY(addsub->expression))
				{
					temp=PP_translate(addsub->expression);
					Rfree(addsub->expression);
					if(temp!=NULL)
					{
						addsub->expression=stralloc(temp);
						Rfree(temp);
					}
				}
			}
		}
		if(p->EditWdgts!=NULL)
		{
			for(x=0,AE=p->EditWdgts;x<p->numwdgts;++x,++AE)
			{
				if(!isEMPTY(AE->expression))
				{
					temp=PP_translate(AE->expression);
					Rfree(AE->expression);
					if(temp!=NULL)
					{
						AE->expression=stralloc(temp);
						Rfree(temp);
					}
				}
				if(!isEMPTY(AE->mgt_expression))
				{
					temp=PP_translate(AE->mgt_expression);
					Rfree(AE->mgt_expression);
					if(temp!=NULL)
					{
						AE->mgt_expression=stralloc(temp);
						Rfree(temp);
					}
				}
				if(!isEMPTY(AE->override_expression))
				{
					temp=PP_translate(AE->override_expression);
					Rfree(AE->override_expression);
					if(temp!=NULL)
					{
						AE->override_expression=stralloc(temp);
						Rfree(temp);
					}
				}
			}
		}
		if(!isEMPTY(p->save_error))
		{
			temp=PP_translate(p->save_error);
			Rfree(p->save_error);
			if(temp!=NULL)
			{
				p->save_error=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_error_desc))
		{
			temp=PP_translate(p->save_error_desc);
			Rfree(p->save_error_desc);
			if(temp!=NULL)
			{
				p->save_error_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_warning))
		{
			temp=PP_translate(p->save_warning);
			Rfree(p->save_warning);
			if(temp!=NULL)
			{
				p->save_warning=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_warning_desc))
		{
			temp=PP_translate(p->save_warning_desc);
			Rfree(p->save_warning_desc);
			if(temp!=NULL)
			{
				p->save_warning_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_error))
		{
			temp=PP_translate(p->quit_error);
			Rfree(p->quit_error);
			if(temp!=NULL)
			{
				p->quit_error=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_expression))
		{
			temp=PP_translate(p->save_expression);
			Rfree(p->save_expression);
			if(temp!=NULL)
			{
				p->save_expression=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_error_desc))
		{
			temp=PP_translate(p->quit_error_desc);
			Rfree(p->quit_error_desc);
			if(temp!=NULL)
			{
				p->quit_error_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_warning))
		{
			temp=PP_translate(p->quit_warning);
			Rfree(p->quit_warning);
			if(temp!=NULL)
			{
				p->quit_warning=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_warning_desc))
		{
			temp=PP_translate(p->quit_warning_desc);
			Rfree(p->quit_warning_desc);
			if(temp!=NULL)
			{
				p->quit_warning_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(p->Writes!=NULL)
		{
			for(x=0,AW=p->Writes;x<p->numWrites;++x,++AW)
			{
				if(!isEMPTY(AW->expression))
				{
					temp=PP_translate(AW->expression);
					Rfree(AW->expression);
					if(temp!=NULL) 
					{
						AW->expression=stralloc(temp);
						Rfree(temp);
					}	
					
				}
				if(AW->fields!=NULL)
				{
					for(y=0,AWF=AW->fields;y<AW->numflds;++y,++AWF)
					{
						if(AWF->copy_type==1)
						{
						if(!isEMPTY(AWF->from_name))
						{
							temp=PP_translate(AWF->from_name);
							Rfree(AWF->from_name);
							if(temp!=NULL)
							{
								AWF->from_name=stralloc(temp);
								Rfree(temp);
							}
						}
						}
					}
				}
			}
		}
		if(p->buttons!=NULL)
		{
			for(x=0,BTN=p->buttons;x<p->numbuttons;++x,++BTN)
			{
				if(!isEMPTY(BTN->expression))
				{
					temp=PP_translate(BTN->expression);
					Rfree(BTN->expression);
					if(temp!=NULL) 
					{
						BTN->expression=stralloc(temp);
						Rfree(temp);
					}	
					
				}
			}
		}
	}
}
static void doexit(PowerAdd *POWERADD)
{
	int level=0; 

	if(POWERADD!=NULL)
	{
		level=POWERADD->level;
		CLOSE_POWERADD_FILES(POWERADD);
		if(POWERADD!=NULL) FreePowerAdd(POWERADD);
		if(level==0)
		{
		 	ShutdownSubsystems();
		}
	}
}
void POWERADDSubData(char **tmp,PowerAdd *POWERADD)
{
	RDArsrc *r=NULL;
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL,PAstemp[101];
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
	PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	r=POWERADD->addrsrc;
	}
#ifdef USE_RDA_DIAGNOSTICS
/*
struct ADDPassableStructs
{
        RDArsrc *r;
        void *a;
        void (*quitfunc)(...);
        RDArsrc *fromrsrc;
        void *fromarg;
        PowerAdd *POWERADD;
        void *arg;
typedef struct ADDPassableStructs ADDPassableStruct;
}; */
	if(diagvirtual)
	{
		prterr("DIAG POWERADDSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
/* BREAK_HERE fields */
	memset(PAstemp,0,101);
	filename=stripfilename(*tmp);
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
				} else occurrence=1;
				gn=strtok(NULL,",]");
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
						sprintf(PAstemp,"(%d)",*field->data.short_value);
					} else {
						sprintf(PAstemp,"%d",*field->data.short_value);
					}
					str_length=RDAstrlen(PAstemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,PAstemp,str_length+1);
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					if(*field->data.integer_value<0)
					{
						sprintf(PAstemp,"(%d)",*field->data.integer_value);
					} else {
						sprintf(PAstemp,"%d",*field->data.integer_value);
					}
					str_length=RDAstrlen(PAstemp);
					if(RDAstrlen(*tmp)<RDAstrlen(PAstemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,PAstemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(*field->data.float_value<0)
					{
						sprintf(PAstemp,"(%f)",*field->data.float_value);
					} else {
						sprintf(PAstemp,"%f",*field->data.float_value);
					}
					str_length=RDAstrlen(PAstemp);
					if(RDAstrlen(*tmp)<RDAstrlen(PAstemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,PAstemp,str_length+1);
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
					break;
			}
		} else {
			prterr("Error Field [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
	} else if(!AddEditWdgtSubData(tmp,POWERADD))
	{
	} else if(SCRNvirtualVIRTUALSubData(tmp,r,modulename)==0)
	{
	} else if(GLOBALSubData(tmp,modulename)==0)
	{
	} else if(ScrolledListSubData(tmp,r)==0)
	{
	}
/* OK */
	Rfree(modulename);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG POWERADDSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
short AddEditWdgtSubData(char **tmp,PowerAdd *POWERADD)
{
	AddEditWdgt *EditWdgt=NULL;
	char *xname=NULL,*temp=NULL,*gn;
	int length=0,x,str_length=0,start=1;

	if(POWERADD!=NULL)
	{
		xname=stripmodulename(*tmp);
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			if(!RDAstrcmp(EditWdgt->name,xname)) break;
		}
		if(x<POWERADD->numwdgts)
		{
			if(RDAstrlen(*tmp)>(RDAstrlen(xname)+2))
			{
				temp=*tmp+(RDAstrlen(xname)+3);
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
			switch(EditWdgt->type)
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
					temp=stralloc(EditWdgt->data.string_value);
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
					if(*EditWdgt->data.short_value<0)
					{
						sprintf(stemp,"(%d)",*EditWdgt->data.short_value);
					} else {
						sprintf(stemp,"%d",*EditWdgt->data.short_value);
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
					if(*EditWdgt->data.integer_value<0)
					{
						sprintf(stemp,"(%d)",*EditWdgt->data.integer_value);
					} else {
						sprintf(stemp,"%d",*EditWdgt->data.integer_value);
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
					if(*EditWdgt->data.float_value<0)
					{
						sprintf(stemp,"(%f)",*EditWdgt->data.float_value);
					} else {
						sprintf(stemp,"%f",*EditWdgt->data.float_value);
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
					sprintf(*tmp,"%d",(*EditWdgt->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*EditWdgt->data.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s].",EditWdgt->type,EditWdgt->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					return(-1);
			}
			if(xname!=NULL) Rfree(xname);
			return(0);
		}
		if(xname!=NULL) Rfree(xname);
	} else {
		prterr("No POWERADD Structure.");
	}
	return(-1);
}
short DOADD_GUIEVAL(char *string,RDArsrc *r,PowerAdd *POWERADD)
{
	if(!isEMPTY(string))
	{
		return(EVALUATEbol(string,POWERADDSubData,(void *)POWERADD));
	} else {
		return(TRUE);
	}
}
char *DOADD_GUIEVALSTR(char *string,RDArsrc *r,void *SubFuncArgs,PowerAdd *POWERADD)
{
	char *tmp=NULL;

	if(!isEMPTY(string))
	{
		tmp=PP_EVALUATEstr(string,POWERADDSubData,(void *)POWERADD);
	}
	return(tmp);
}

void READEditWdgts(PowerAdd *POWERADD)
{
	char *temp=NULL,*dashes=NULL,*rname=NULL;
	int x=0;
	AddEditWdgt *EditWdgt=NULL;

	if(POWERADD!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			if(!isEMPTY(EditWdgt->name))
			{
				dashes=adddashes(EditWdgt->name);
				temp=RDA_GetEnv(dashes);
				if(dashes!=NULL) Rfree(dashes);
#ifdef USE_RDA_DIAGNOSTICS
				if(diagadd)
				{
					prterr("DIAG ReadingEnvironment EditWidget [%s] [%s] ",EditWdgt->name,(temp!=NULL ? temp:""));
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			}
			if(!isEMPTY(temp))
			{
				switch(EditWdgt->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case EXPENDITURE:
					case BALANCESHEET:
					case REVENUE:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(EditWdgt->data.string_value!=NULL) Rfree(EditWdgt->data.string_value);
						EditWdgt->data.string_value=stralloc(temp);
						EditWdgt->dlen=RDAstrlen(EditWdgt->data.string_value)+1;
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS_NOCENTS:
					case DOLLARS:
					case DOUBLEV:
					case SDOUBLEV:
						if(!isEMPTY(temp))
						{
						*EditWdgt->data.float_value=atof(temp);
						} else *EditWdgt->data.float_value=0;
						break;
					case SHORTV:
					case SSHORTV:
						if(!isEMPTY(temp))
						{
						*EditWdgt->data.short_value=Ratoi(temp);
						} else *EditWdgt->data.short_value=0;
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						if(!isEMPTY(temp))
						{
						*EditWdgt->data.integer_value=Ratoi(temp);
						} else *EditWdgt->data.integer_value=0;
						break;
					case BOOLNS:
						if(Ratoi(temp)>0) *EditWdgt->data.string_value=TRUE;
							else *EditWdgt->data.string_value=FALSE;
						break;
					case CHARACTERS:
						*EditWdgt->data.string_value=*temp;
						break;
					case BUTTONS:
						prterr("Huh?....");
						break;
					default:
						prterr("Error Invalid Field Type [%d].",EditWdgt->type);
						break;
				}
				EditWdgt->etype=(-1);
				if(POWERADD->AddType==1 && ((EditWdgt->loadvirtual==2) || (EditWdgt->loadvirtual==3)))
				{
					rname=Rmalloc(RDAstrlen(EditWdgt->name)+6);
					sprintf(rname,"%04d:%s",POWERADD->table_row+1,EditWdgt->name);
					COPYRSRC2RSRC(POWERADD->addrsrc,EditWdgt->name,rname);
					if(rname!=NULL) Rfree(rname);
				}
			}
		}
	}
}
void setpadddeftype(RDArmem *member,PowerAdd *POWERADD)
{
	RDArmem *mem=NULL;
	short x;
	ADDPassableStruct *PSTRUCT=NULL;
	RDArsrc *rsrc=NULL;
	
	rsrc=(RDArsrc *)member->parent;
	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
		if(PSTRUCT!=NULL)
		{
			if(member!=NULL && rsrc!=NULL)
			{	
				readwidget(rsrc,member->rscrname);
				if(rsrc->rscs!=NULL)
				{
					for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
					{
						if(mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE)
						{
							switch(mem->field_type)
							{
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
									mem->items=*member->value.short_value;
									if(mem->dlen>0)
									{
										memset(mem->value.string_value,0,mem->dlen);
									} else {
										mem->value.string_value=NULL;
										mem->dlen=0;
									}
									updatemember(mem);
									break;
								default:
									prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
									break;
							}
						}
					}
				}
			}
			SET_POWERADD(rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		} else {
			prterr("Error PSTRUCT (ADDPassableStruct) is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error POWERADD is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}

}
void setpaddacctype(RDArmem *member,PowerAdd *POWERADD)
{
	RDArmem *mem=NULL;
	short x;
	ADDPassableStruct *PSTRUCT=NULL;
	RDArsrc *rsrc=NULL;

	rsrc=(RDArsrc *)member->parent;
	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
		if(PSTRUCT!=NULL)
		{
			if(rsrc!=NULL && member!=NULL)
			{
				if(!isEMPTY(member->rscrname))
				{
					readwidget(rsrc,member->rscrname);
					if(rsrc->rscs!=NULL)
					{
						for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
						{
							if(!isEMPTY(mem->rscrname))
							{
								if((mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE) && (RDAstrstr(mem->rscrname,"ACCOUNT CODE")!=NULL))
								{
									switch(mem->field_type)
									{
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
											switch(*member->value.short_value)
											{
												case EXP_ACCT:
													mem->field_type=EXPENDITURE;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												case REV_ACCT:
													mem->field_type=REVENUE;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												case BAL_ACCT:
													mem->field_type=BALANCESHEET;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												case BEG_ACCT:
													mem->field_type=BEGINNINGBALANCE;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												default:
													prterr("Error Resource [%s] has the wrong acctype [%d].",member->rscrname,*member->value.short_value);
													return;
											}
											break;
										default:
											prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
											break;
									}
								}
							}
						}
					}
				}
			}
			SET_POWERADD(rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		} else {
			prterr("Error PSTRUCT (ADDPassableStruct) is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error POWERADD is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void add_definelist(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	if(IsScreenDisplayed(POWERADD->ADD_DEFINE_LIST)) return;
	POWERADDcomputeallSCRNvirtuals(POWERADD->ADD_DEFINE_LIST,POWERADD,POWERADDSubData,(void *)POWERADD);
	if(ADVmakescrn(POWERADD->ADD_DEFINE_LIST,TRUE))
	{
		prterr("Error cannot create screen [%s][%s].",(POWERADD->ADD_DEFINE_LIST->module!=NULL?POWERADD->ADD_DEFINE_LIST->module:""),(POWERADD->ADD_DEFINE_LIST->screen!=NULL?POWERADD->ADD_DEFINE_LIST->screen:""));
	}
}
static void makedflist(PowerAdd *POWERADD)
{
	RDArmem *len_member=NULL,*pos_member=NULL;
	int x,cur=1,length=0,lcount=0;
	char *s=NULL,*hold=NULL,*temp=NULL;
	AddEditWdgt *EditWdgt=NULL;
	char strtemp[1024];
	
	
	if(POWERADD!=NULL)
	{
	if(POWERADD->PREVIOUSADD!=NULL)
	{
		if(POWERADD->PREVIOUSADD->numlibs==1 && !RDAstrcmp(POWERADD->PREVIOUSADD->libs[0],
			"Nothing Added this Session."))
		{
			freeapplib(POWERADD->PREVIOUSADD);
			POWERADD->PREVIOUSADD=APPlibNEW();
		}
	} else POWERADD->PREVIOUSADD=APPlibNEW();
	if(POWERADD->ADD_DEFINE_LIST!=NULL)
	{
		for(x=0,length=0;x<(POWERADD->ADD_DEFINE_LIST->numrscs-5);x+=2)
		{
			pos_member=POWERADD->ADD_DEFINE_LIST->rscs+x;
			len_member=POWERADD->ADD_DEFINE_LIST->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				length+=(*len_member->value.short_value+1);
			}		
		}	
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(POWERADD->ADD_DEFINE_LIST->numrscs-5))
	{
		for(x=0;x<(POWERADD->ADD_DEFINE_LIST->numrscs-5);x+=2)
		{
			pos_member=POWERADD->ADD_DEFINE_LIST->rscs+x;
			len_member=POWERADD->ADD_DEFINE_LIST->rscs+(x+1);
			if(*pos_member->value.short_value>0)
			{
			if(*pos_member->value.short_value==cur &&
				*len_member->value.short_value>0)
			{
				s=stralloc(pos_member->rscrname);
				s[RDAstrlen(s)-9]=0;
				EditWdgt=FINDEDITWDGT(POWERADD,s);		
				if(EditWdgt!=NULL)
				{
					switch(EditWdgt->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case EXPENDITURE:
						case BALANCESHEET:
						case REVENUE:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(*len_member->value.short_value<RDAstrlen(EditWdgt->data.string_value))
							{
								sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(EditWdgt->data.string_value!=NULL ? EditWdgt->data.string_value:""));
							} else {
								sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(EditWdgt->data.string_value!=NULL ? EditWdgt->data.string_value:""));
							}
							break;
						case DECIMALV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*f",*len_member->value.short_value,*EditWdgt->data.float_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;

						case SDECIMALV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.float_value>=0)
							{
								sprintf(strtemp,"%*f ",*len_member->value.short_value,*EditWdgt->data.float_value);
								strtemp[*len_member->value.short_value]=0;
							} else {
								sprintf(strtemp,"%*f- ",*len_member->value.short_value,(*EditWdgt->data.float_value)*(-1));
								strtemp[*len_member->value.short_value+1]=0;
							}
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SHORTV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*d",*len_member->value.short_value,*EditWdgt->data.short_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SSHORTV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.short_value>=0)
							{ 
								sprintf(strtemp,"%*d ",*len_member->value.short_value,*EditWdgt->data.short_value);
								strtemp[*len_member->value.short_value]=0;
							} else {
								sprintf(strtemp,"%*d-",*len_member->value.short_value,(*EditWdgt->data.short_value)*(-1));
								strtemp[*len_member->value.short_value+1]=0;
							}
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case LONGV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*d",*len_member->value.short_value,*EditWdgt->data.integer_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SLONGV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.integer_value>=0)
							{
								sprintf(strtemp,"%*d ",*len_member->value.short_value,*EditWdgt->data.integer_value);
								strtemp[*len_member->value.short_value]=0;
							} else {
								sprintf(strtemp,"%*d-",*len_member->value.short_value,(*EditWdgt->data.integer_value)*(-1));
								strtemp[*len_member->value.short_value+1]=0;
							}
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case DOLLARS_NOCENTS:
							temp=famtncents(*EditWdgt->data.float_value*100,*len_member->value.short_value,' ');
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
							if(temp!=NULL) Rfree(temp);
							break;
						case DOLLARS:
							temp=famt(*EditWdgt->data.float_value,*len_member->value.short_value);
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
							if(temp!=NULL) Rfree(temp);
							break;
						case DOUBLEV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*.0f",*len_member->value.short_value,*EditWdgt->data.float_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SDOUBLEV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.float_value>=0)
							{ 
								sprintf(strtemp,"%*.0f ",*len_member->value.short_value,*EditWdgt->data.float_value);
								strtemp[*len_member->value.short_value]=0;
							} else {
								sprintf(strtemp,"%*.0f-",*len_member->value.short_value,(*EditWdgt->data.float_value)*(-1));
								strtemp[*len_member->value.short_value+1]=0;
							}
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case BOOLNS:
							sprintf(&hold[lcount],"%c ",(*EditWdgt->data.string_value  ? 'Y':'N'));
							break;
						case CHARACTERS:
							sprintf(&hold[lcount],"%c ",*EditWdgt->data.string_value);
							break;
						case BUTTONS:
						case SCROLLEDLIST:
							prterr("Huh?....");
							break;
						default:
							prterr("Error Invalid Field Type [%d].",EditWdgt->type);
							break;
					}
					lcount+=(*len_member->value.short_value+1);
				} else {
					prterr("Error AddEditWdgt [%s] not found.",s);
				}
				if(s!=NULL) Rfree(s);
			}
			}
		}
		++cur;
	}
	addAPPlib(POWERADD->PREVIOUSADD,hold);
	if(hold!=NULL) Rfree(hold);
	}
}
static void savelistcb(RDArsrc *define_list,RDArsrc *add_rsrc)
{
	if(IsScreenDisplayed(define_list)) NULL_RSCS(define_list);
}
static void INCREFNUM(RDArsrc *r,AddEditWdgt *e)
{
	PowerAdd *POWERADD=NULL;
	AddEditWdgt *AE=NULL;
	RDArmem *member=NULL;
	int x=0;

	if(e!=NULL)
	{
		member=r->rscs+FINDRSC(r,e->name);
		readmember(member);	
#ifdef __GTK_H__
		member->app_update=TRUE;
#endif /* __GTK_H__ */
		POWERADD=(PowerAdd *)e->parent;
		if(POWERADD!=NULL && *member->value.string_value==TRUE)
		{
			for(x=0,AE=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++AE)
			{
				if(AE->id_mgt==TRUE && AE->wrote_id==TRUE)
				{
					REMOVENEWID(POWERADD,AE,POWERADDSubData,(void *)POWERADD);
					AE->wrote_id=FALSE;
				} else if(AE->id_mgt==TRUE)
				{
					UndoASSIGNID(POWERADD,AE,POWERADDSubData,POWERADD);
					AE->wrote_id=FALSE;
				}
			}
			SET_POWERADD(r,POWERADD,POWERADDSubData,(void *)POWERADD);
		}
		*member->value.string_value=FALSE;
		updatemember(member);
#ifdef __GTK_H__
		member->app_update=FALSE;
#endif /* __GTK_H__ */
	}
}
void xRUNPOWERADD(char *module,char *poweradd,int level,
	void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromarg,
	short APPmainLoop,int line,char *file)
{
	char *warnmessage=NULL;
	RDArsrc *add_rsrc=NULL;
	AddEditWdgt *EditWdgt=NULL;
	char *temp=NULL;
	int x=0;
	short count=0,flen=0;
	PowerAdd *POWERADD=NULL;
	ADDPassableStruct *PSTRUCT=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG RUNPOWERADD(%s,%s) called at line [%d] program [%s].",module,poweradd,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	POWERADD=PowerAddNew(module,poweradd);
	POWERADD->APPmainLoop=APPmainLoop;
	POWERADD->AddType=0;
	PSTRUCT=Rmalloc(sizeof(ADDPassableStruct));
	PSTRUCT->r=add_rsrc;
	PSTRUCT->quitfunc=quitfunc;
	PSTRUCT->fromrsrc=fromrsrc;
	PSTRUCT->fromarg=fromarg;

	if(findPowerAdd(POWERADD))
	{
		warnmessage=Rmalloc(300+RDAstrlen(module)+RDAstrlen(poweradd));
		sprintf(warnmessage,"Error Power Add Definition not found, this process cannot be executed without it's definition [%s] in the [%s.ADD] library.",poweradd,module);
		ERRORDIALOG("MAINTAIN POWERADD NOT FOUND.",warnmessage,NULL,(level==0?TRUE:FALSE)); 
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(POWERADD);
		return;
	} else {
		if(!OPEN_POWERADD_FILES(POWERADD))
		{
			doexit(POWERADD);
			prterr("Error cannot open files for this power add..");
			return;
		}
		Set_Skip_Widgets(POWERADD);

		AllocateEditWdgts(POWERADD);
		PP_translate_POWERADD(POWERADD);

		add_rsrc=RDArsrcNEW(POWERADD->module,POWERADD->addscreen);
		POWERADD->ADD_DEFINE_LIST=RDArsrcNEW(POWERADD->module,POWERADD->define_list);
		PSTRUCT->r=add_rsrc;
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;

		POWERADD->SubFunc=POWERADDSubData;
		POWERADD->SubFuncArgs=POWERADD;
		POWERADD->addrsrc=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;
		POWERADD->level=level;

		POWERADD->PREVIOUSADD=APPlibNEW();
		addAPPlib(POWERADD->PREVIOUSADD,"Nothing Added this Session.");
		addlstrsrc(add_rsrc,"PREVIOUS ADD'S",&x,TRUE,NULL,POWERADD->PREVIOUSADD->numlibs,
			&POWERADD->PREVIOUSADD->libs,NULL);
		GET_POWERADD_INCVARS(add_rsrc,POWERADD);
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
				++x,++EditWdgt)
			{
				if(!RDAstrcmp(EditWdgt->name,"INCREMENT REFERENCE NUMBER"))
				{
					++count;
					temp=Rmalloc(RDAstrlen(EditWdgt->name)+10);
					sprintf(temp,"%s POSITION",EditWdgt->name);
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&count,TRUE);
					sprintf(temp,"%s LENGTH",EditWdgt->name);
					flen=EditWdgt->len;
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&flen,TRUE);
					if(temp!=NULL) Rfree(temp);
					makefieldrsrc(add_rsrc,EditWdgt->name,EditWdgt->type,
						EditWdgt->len,TRUE);
					FINDRSCSETFUNC(add_rsrc,EditWdgt->name,INCREFNUM,EditWdgt);
				} else if(!isEMPTY(EditWdgt->name))
				{
					++count;
					temp=Rmalloc(RDAstrlen(EditWdgt->name)+10);
					sprintf(temp,"%s POSITION",EditWdgt->name);
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&count,TRUE);
					sprintf(temp,"%s LENGTH",EditWdgt->name);
					flen=EditWdgt->len;
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&flen,TRUE);
					if(temp!=NULL) Rfree(temp);
					makefieldrsrc(add_rsrc,EditWdgt->name,EditWdgt->type,
						EditWdgt->len,TRUE);
					if(EditWdgt->id_mgt==TRUE)
					{
						FINDRSCSETFUNC(add_rsrc,EditWdgt->name,SETNEWIDANDFILES,EditWdgt);
					} else {
						FINDRSCSETFUNC(add_rsrc,EditWdgt->name,SETFILES,EditWdgt);
					}
					if(RDAstrcmp(EditWdgt->name,"IMAGE"))
					{
						switch(EditWdgt->type)
						{
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								FINDRSCSETCALLBACKTYPE(add_rsrc,EditWdgt->name,TRUE);
								break;
							default:
								break;
						}
					}
				}
			}
		}
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"DEFAULTS",TRUE,SaveDefaultsUsedOnly,NULL);
		addrfkwrsrc(POWERADD->ADD_DEFINE_LIST,"SELECT",TRUE,savelistcb,add_rsrc);
		addrfkwrsrc(POWERADD->ADD_DEFINE_LIST,"QUIT",TRUE,NULL_RSCS,NULL);
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		GET_POWERADD_SCREEN_VIRTUALS(add_rsrc,0,FALSE,TRUE,POWERADD);
		SET_POWERADD_FILE2RSRCS(add_rsrc,POWERADD);

		ADVSCRNvirtual2rsrc(add_rsrc,POWERADDSubData,(void *)POWERADD);

		PSTRUCT->r=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;

		DefaultScreens(add_rsrc);
		GetAddEditWdgts(add_rsrc,POWERADD);
		ReadRDAScrolledLists(add_rsrc);
		READEditWdgts(POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		DefaultScreens(POWERADD->ADD_DEFINE_LIST);
		GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,FALSE,FALSE);
		DefaultAddEditWdgts(add_rsrc,POWERADD,0,POWERADDSubData,(void *)POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		addbtnrsrc(add_rsrc,"SAVE",TRUE,save_record_now,POWERADD);
		addbtnrsrc(add_rsrc,"DEFAULTS",TRUE,SaveDefaults,POWERADD);
		addbtnrsrc(add_rsrc,"RESET DEFAULTS",TRUE,resetdefs,POWERADD);
		addbtnrsrc(add_rsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
		addbtnrsrc(add_rsrc,"DEFINE LIST",TRUE,add_definelist,POWERADD);
		addrfcbrsrc(add_rsrc,"QUIT",TRUE,quit_test,POWERADD);
		addrfcbrsrc(add_rsrc,"HELP",TRUE,screenhelp,POWERADD);
		addrfcbrsrc(add_rsrc,"PRINT RESOURCES",TRUE,printrsrcs,POWERADD);
		MakePowerAddButtonsADV(add_rsrc,POWERADD,updateallPowerAdd);
		POWERADDcomputeallSCRNvirtuals(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;
		PSTRUCT->r=add_rsrc;
		
		POWERADD->passable=(void *)PSTRUCT;
		SetAddEditWdgts(add_rsrc,POWERADD);
		xmakescrn(add_rsrc,TRUE,DOADD_GUIEVAL,(void *)POWERADD,addoswidgets,DOADD_GUIEVALSTR,(void *)POWERADD,POWERADDSubData,POWERADD,NULL,__LINE__,__FILE__);
		SetAddEditWdgts(add_rsrc,POWERADD);
		if(!APPmainLoop)
		{
			APPmainLoop=TRUE;
			POWERADD->APPmainLoop=TRUE;
			RDAAPPMAINLOOP();
		}
	}
}
static void resetdefs(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	AddEditWdgt *EditWdgt=NULL;
	int x=0;
	char *name=NULL,*rname=NULL;

	if(POWERADD!=NULL)
	{
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
		{
			if(EditWdgt!=NULL)
			{
				if(EditWdgt->id_mgt==TRUE)
				{
					UndoASSIGNID(POWERADD,EditWdgt,POWERADDSubData,POWERADD);
				}
			}
		}
	}
	}
	EditWdgt=NULL;
	ADVDefaultResetScreen(add_rsrc,TRUE,&name);
	if(POWERADD!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			if(EditWdgt->etype==(-1))
			{
				switch(EditWdgt->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case EXPENDITURE:
					case BALANCESHEET:
					case REVENUE:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						FINDRSCSETSTRING(add_rsrc,EditWdgt->name,EditWdgt->data.string_value);
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS_NOCENTS:
					case DOLLARS:
					case DOUBLEV:
					case SDOUBLEV:
						FINDRSCSETDOUBLE(add_rsrc,EditWdgt->name,*EditWdgt->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						FINDRSCSETSHORT(add_rsrc,EditWdgt->name,*EditWdgt->data.short_value);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						FINDRSCSETINT(add_rsrc,EditWdgt->name,*EditWdgt->data.integer_value);
						break;
					case BOOLNS:
					case CHARACTERS:
						FINDRSCSETCHAR(add_rsrc,EditWdgt->name,*EditWdgt->data.string_value);
						break;
					case BUTTONS:
						prterr("Huh?....");
						break;
					default:
						prterr("Error Invalid Field Type [%d].",EditWdgt->type);
						break;
				}
				if(POWERADD->AddType==1 && ((EditWdgt->loadvirtual==2) || (EditWdgt->loadvirtual==3)))
				{
					rname=Rmalloc(RDAstrlen(EditWdgt->name)+6);
					sprintf(rname,"%04d:%s",POWERADD->table_row+1,EditWdgt->name);
					COPYRSRC2RSRC(add_rsrc,EditWdgt->name,rname);
					if(rname!=NULL) Rfree(rname);
				}
			}
		}
	}
	updateallrsrc(add_rsrc);
	GetAddEditWdgts(add_rsrc,POWERADD);
	DefaultAddEditWdgts(add_rsrc,POWERADD,1,POWERADDSubData,(void *)POWERADD);
	SetAddEditWdgts(add_rsrc,POWERADD);
	updateallrsrc(add_rsrc);
	SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
	updateallrsrc(add_rsrc);
	if(!isEMPTY(name)) FINDRSCSETINPUTFOCUS(add_rsrc,name);

	if(name!=NULL) Rfree(name);
}
void SETNEWIDANDFILES(RDArsrc *add_rsrc,AddEditWdgt *EditWdgt)
{	
	char *value=NULL,*rname=NULL;
	PowerAdd *POWERADD=NULL;

	if(EditWdgt!=NULL)
	{
	POWERADD=(PowerAdd *)EditWdgt->parent;
	if(POWERADD!=NULL)
	{
	readwidget(add_rsrc,EditWdgt->name);
	switch(EditWdgt->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case PLAINTEXT:
		case PHONE:
		case CUSTOMTYPE:
		case ZIPCODE:
		case SOCSECNUM:
		case DATES:
		case TIMES:
		case EXPENDITURE:
		case BALANCESHEET:
		case REVENUE:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			FINDRSCGETSTRING(add_rsrc,EditWdgt->name,&value);
			break;
		case DECIMALV:
		case SDECIMALV:
		case DOLLARS_NOCENTS:
		case DOLLARS:
		case DOUBLEV:
		case SDOUBLEV:
		case SHORTV:
		case SSHORTV:
		case LONGV:
		case SLONGV:
		case SCROLLEDLIST:
		case BOOLNS:
		case CHARACTERS:
		case BUTTONS:
		default:
			prterr("Error: SETNEWIDANDFILES(): Managed Edit Widget must be a String Type Field, name [%s]",
				(EditWdgt->name!=NULL?EditWdgt->name:""));
			break;
	}
	if(POWERADD->AddType==1 && ((EditWdgt->loadvirtual==2) || (EditWdgt->loadvirtual==3)))
	{
		rname=Rmalloc(RDAstrlen(EditWdgt->name)+6);
		sprintf(rname,"%04d:%s",POWERADD->table_row+1,EditWdgt->name);
		COPYRSRC2RSRC(add_rsrc,EditWdgt->name,rname);
		if(rname!=NULL) Rfree(rname);
	}
	SET_POWERADD_MGT(add_rsrc,POWERADD,value,POWERADDSubData,(void *)POWERADD);
	}
	if(value!=NULL) Rfree(value);
	}
}
void SETFILES(RDArsrc *add_rsrc,AddEditWdgt *EditWdgt)
{
	PowerAdd *POWERADD=NULL;
	RDArmem *member=NULL;

	if(EditWdgt!=NULL)
	{
		member=add_rsrc->rscs+FINDRSC(add_rsrc,EditWdgt->name);
#ifdef __GTK_H__
		member->app_update=TRUE;
#endif /* __GTK_H__ */
		POWERADD=(PowerAdd *)EditWdgt->parent;
		if(POWERADD!=NULL)
		{
			SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		}
#ifdef __GTK_H__
		member->app_update=FALSE;
#endif /* __GTK_H__ */
	}
}
static void readscreen(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	readallwidgets(add_rsrc);
	ReadRDAScrolledLists(add_rsrc);
	GetAddEditWdgts(add_rsrc,POWERADD);
}
void updateallPowerAdd(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	if(POWERADD!=NULL)
	{
		if(add_rsrc!=NULL)
		{
			readallwidgets(add_rsrc);
       			GetAddEditWdgts(add_rsrc,POWERADD);
			GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,TRUE,FALSE);
			DefaultAddEditWdgts(add_rsrc,POWERADD,2,POWERADDSubData,(void *)POWERADD);
			SetAddEditWdgts(add_rsrc,POWERADD);
			xcomputeandupdateallSCRNvirtuals(add_rsrc,POWERADDSubData,(void *)POWERADD,FALSE,FALSE,NULL,POWERADD,__LINE__,__FILE__);
			updateallrsrc(add_rsrc);
			SetAllEditableAndSensitive(add_rsrc);
		}
	}
}
static void run_prequit_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			if(button->type==3)
			{
				do_btn=FALSE;
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
	/*
					if(do_btn==FALSE) return;
	*/
				} else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=updateallPowerAdd;
					PowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void run_presave_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			if(button->type==1)
			{
				do_btn=FALSE;
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
				}  else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=updateallPowerAdd;
					PowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void run_postsave_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			do_btn=FALSE;
			if(button->type==2)
			{
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
				} else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=updateallPowerAdd;
					PowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void save_record_warn(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	if(POWERADD!=NULL)
	{
		readscreen(add_rsrc,POWERADD);
		run_presave_buttons(add_rsrc,POWERADD,TRUE);
		save_record(add_rsrc,POWERADD,TRUE);
		run_postsave_buttons(add_rsrc,POWERADD,TRUE);
		if((POWERADD->save_type==0 || POWERADD->save_type>3))
		{
/* Added to recompute widgets when using Power Save types */
			SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		}
		if(POWERADD->save_type==2 || POWERADD->save_type==4)
		{
			resetdefs(add_rsrc,POWERADD);
		}
	}
}
static short UsingCurrentRow(RDArsrc *r,PowerAdd *POWERADD)
{
	int x=0,num_used=0,num_dols=0,dols_used=0,z=0;
	AddEditWdgt *EW=NULL,*EWR=NULL;
	short using_replicate=FALSE,retval=FALSE;
	RDArmem *mem=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG UsingCurrentRow [%d] ",POWERADD->table_row+1);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(EW->loadvirtual==2)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EW->name);
			EWR=FINDEDITWDGT(POWERADD,stemp);
			z=FINDRSC(POWERADD->addrsrc,stemp);
			if(z>(-1)) mem=POWERADD->addrsrc->rscs+z;
				else mem=NULL;
			if(mem!=NULL)
			{
			if(EWR!=NULL && mem->w!=NULL && EW->loadvirtual<3)
			{
				using_replicate=TRUE;
				switch(EWR->type)
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
						if(!isEMPTY(EWR->data.string_value)) ++num_used;
						break;
					case BOOLNS:
						break;
					case CHARACTERS:
						if(*EWR->data.string_value!=0) ++num_used;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
						++num_dols;
						if(*EWR->data.float_value!=0.0)
						{
							++num_used;
							++dols_used;
						}
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(*EWR->data.float_value!=0.0) ++num_used;
						break;
					case SHORTV:
					case SSHORTV:
					/*
						if(*EWR->data.short_value!=0) ++num_used;
					*/
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						if(*EWR->data.integer_value!=0) ++num_used;
						break;
					default:
					case BUTTONS:
						break;
				}
			}
			}
		}
	}
	if(!using_replicate && POWERADD->table_row==0) retval=TRUE;
	else if(using_replicate && num_used) retval=TRUE;
	else retval=FALSE;
	POWERADD->UsingRow[POWERADD->table_row]=retval;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG UsingCurrentRow [%d] returning [%s].",POWERADD->table_row+1,(retval ? "TRUE":"FALSE"));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(retval);
}
static void save_all_now_test2(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	char *desc=NULL;
	int x=0,currow=0;
	readscreen(add_rsrc,POWERADD);
	currow=POWERADD->table_row+1;
	for(x=0;x<REPLICATE;++x)
	{
		POWERADD->table_row=x;
		FINDRSCSETINT(add_rsrc,"CURRENT ROW",(x+1));
		updatersrc(add_rsrc,"CURRENT ROW");
		if(UsingCurrentRow(add_rsrc,POWERADD))
		{
			if(currow==(-1)) currow=(x+1);
/*
			SetReplicate2EditWgt(POWERADD);
*/
			CopyRow2Primary(POWERADD);
			if(check_row_quick(add_rsrc,POWERADD,FALSE,FALSE)==(-1)) 
			{
				if(POWERADD->TableWidget!=NULL)
				{
				}
				FINDRSCSETINT(add_rsrc,"CURRENT ROW",currow);
				updatersrc(add_rsrc,"CURRENT ROW");
				return;
			}
		}
	}
	FINDRSCSETINT(add_rsrc,"CURRENT ROW",currow);
	updatersrc(add_rsrc,"CURRENT ROW");
/*
	SetReplicate2EditWgt(POWERADD);
*/
	if(INSIDE_PASTE)
	{
		INSIDE_PASTE=FALSE;
		return;
	}
/* DCT's latest test
	SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
	if(!SAVE_POWERADD_CHECK(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD))
	{
		return;
	}
*/
	if(!isEMPTY(POWERADD->save_expression))
	{
		if(PP_EVALUATEbol(POWERADD->save_expression,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->save_error_desc,POWERADDSubData,POWERADD);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(POWERADD->save_warning))
	{
		if(PP_EVALUATEbol(POWERADD->save_warning,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->save_warning_desc,
				POWERADDSubData,(void *)POWERADD);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,save_all_now,NULL,FALSE,2,add_rsrc,(void *)POWERADD,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_all_now(add_rsrc,POWERADD);
}
static void save_all_now_test(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	char *desc=NULL;
	int z=0,x=0,currow=0;
	RDArmem *mem=NULL;
	readscreen(add_rsrc,POWERADD);
	currow=POWERADD->table_row+1;
	if(INSIDE_PASTE==FALSE)
	{
		if(UsingCurrentRow(add_rsrc,POWERADD))
		{
			if(check_row_quick(add_rsrc,POWERADD,TRUE,TRUE)) 
			{
				currow=(-1);
				return;
			}
		}
		for(x=0;x<REPLICATE;++x)
		{
			POWERADD->table_row=x;
			FINDRSCSETINT(add_rsrc,"CURRENT ROW",(x+1));
			updatersrc(add_rsrc,"CURRENT ROW");
			if(UsingCurrentRow(add_rsrc,POWERADD))
			{
				if(currow==(-1)) currow=(x+1);
				CopyRow2Primary(POWERADD);
				if(check_row_quick(add_rsrc,POWERADD,FALSE,FALSE)==(-1)) 
				{
					if(POWERADD->TableWidget!=NULL)
					{
					}
					FINDRSCSETINT(add_rsrc,"CURRENT ROW",currow);
					updatersrc(add_rsrc,"CURRENT ROW");
					return;
				}
			}
		}
		FINDRSCSETINT(add_rsrc,"CURRENT ROW",currow);
		updatersrc(add_rsrc,"CURRENT ROW");
	} else {
		if(!UsingCurrentRow(add_rsrc,POWERADD))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%04d:DELETE",POWERADD->table_row+1);
			z=FINDRSC(POWERADD->addrsrc,stemp);
			if(z>(-1)) 
			{
				mem=POWERADD->addrsrc->rscs+z;
				deletethisrow(mem,POWERADD);
			}
			currow=(-1);
		}
		for(x=0;x<REPLICATE;++x)
		{
			POWERADD->table_row=x;
			FINDRSCSETINT(add_rsrc,"CURRENT ROW",(x+1));
			updatersrc(add_rsrc,"CURRENT ROW");
			if(UsingCurrentRow(add_rsrc,POWERADD))
			{
				if(currow==(-1)) currow=(x+1);

				SetReplicate2EditWgt(POWERADD);
				if(check_row(add_rsrc,POWERADD,TRUE,FALSE))
				{
					if(POWERADD->TableWidget!=NULL)
					{
					}
					FINDRSCSETINT(add_rsrc,"CURRENT ROW",currow);
					updatersrc(add_rsrc,"CURRENT ROW");
					return;
				}
			}
		}
		FINDRSCSETINT(add_rsrc,"CURRENT ROW",currow);
		updatersrc(add_rsrc,"CURRENT ROW");
		INSIDE_PASTE=FALSE;
		return;
	}
	if(!isEMPTY(POWERADD->save_expression))
	{
		if(PP_EVALUATEbol(POWERADD->save_expression,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->save_error_desc,POWERADDSubData,POWERADD);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(POWERADD->save_warning))
	{
		if(PP_EVALUATEbol(POWERADD->save_warning,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->save_warning_desc,
				POWERADDSubData,(void *)POWERADD);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,save_all_now,NULL,FALSE,2,add_rsrc,(void *)POWERADD,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_all_now(add_rsrc,POWERADD);
}
static void save_all_now(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	int x=0,y=0,z=0,counters=0;
	AddEditWdgt *EW=NULL,*EWR=NULL;
	RDArmem *mem=NULL;
#ifdef USE_DIAGNOSTIC_DURING_SAVE
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;
#endif /* USE_DIAGNOSTIC_DURING_SAVE */

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG save_all_now beginning.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	for(x=0;x<REPLICATE;++x) 
	{
		if(POWERADD->UsingRow[x]==TRUE) ++counters;
	}
#ifdef USE_DIAGNOSTIC_DURING_SAVE
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",POWERADD->module,"SAVING RECORDS",stop_diagnostic,counters);
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
#endif /* USE_DIAGNOSTIC_DURING_SAVE */
/* All Pass.. repeat loop and save */
	for(x=0;x<REPLICATE;++x)
	{
		POWERADD->table_row=x;
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("DIAG save_all_now saving UsingCurrentRow [%d].",POWERADD->table_row+1);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(POWERADD->UsingRow[x]==TRUE)
		{
/*
			SetReplicate2EditWgt(POWERADD);
*/
			CopyRow2Primary(POWERADD);
			run_presave_buttons(add_rsrc,POWERADD,TRUE);
			PerformWrites(add_rsrc,POWERADD);
			run_postsave_buttons(add_rsrc,POWERADD,TRUE);
			makedflist(POWERADD);
			FINDRSCLISTAPPlib(add_rsrc,"PREVIOUS ADD'S",(POWERADD->PREVIOUSADD->numlibs-1),
				POWERADD->PREVIOUSADD);
			updatersrc(add_rsrc,"PREVIOUS ADD'S");
			++POWERADD->WRITE_COUNTER;
		}
#ifdef USE_DIAGNOSTIC_DURING_SAVE
		if(!abort_diagnostic)
		{
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		}
#endif /* USE_DIAGNOSTIC_DURING_SAVE */
	}
	resetdefs(add_rsrc,POWERADD);
	readscreen(add_rsrc,POWERADD);
	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(EW->loadvirtual==2 || EW->loadvirtual==3)
		{
			for(z=0;z<(REPLICATE+1);++z)
			{
				memset(stemp,0,101);
				sprintf(stemp,"%04d:%s",(z==0 ? 1:z),EW->name);
				EWR=FINDEDITWDGT(POWERADD,stemp);
				if(EWR!=NULL)
				{
					if(z==0)
					{
						memset(stemp,0,101);
						sprintf(stemp,"STORE:%s",EW->name);
					} else {
						memset(stemp,0,101);
						sprintf(stemp,"%04d:%s",z,EW->name);
					}
					y=FINDRSC(POWERADD->addrsrc,stemp);
					if(y>(-1))
					{
						mem=POWERADD->addrsrc->rscs+y;
						readmember(mem);
/*
						switch(mem->field_type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case BOOLNS:
							case CHARACTERS:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								if(!isEMPTY(mem->value.string_value))
								{
									memset(mem->value.string_value,0,mem->dlen);
								}
								if(!isEMPTY(EWR->data.string_value))
								{
									memset(EWR->data.string_value,0,EWR->dlen);
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								*mem->value.float_value=0.0;
								*EWR->data.float_value=0.0;
								break;
							case SHORTV:
							case SSHORTV:
								*mem->value.short_value=0;
								*EWR->data.short_value=0;
								break;
							case PROGRESS_BAR:
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								*mem->value.integer_value=0;
								*EWR->data.integer_value=0;
								break;
							case BUTTONS:
								break;
							default:
								prterr("Error field type [%s] invalid for Resource [%s] on Screen [%s] [%s].",mem->field_type,mem->rscrname,POWERADD->addrsrc->module,POWERADD->addrsrc->screen);
								break;
						}
*/
						if(z!=0) updatemember(mem);
					}
				}
			}
		}
	}
#ifdef USE_DIAGNOSTIC_DURING_SAVE
	if(tmprsrc!=NULL)
	{
		if(!abort_diagnostic)
		{
			killwindow(tmprsrc);
		}
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
#endif /* USE_DIAGNOSTIC_DURING_SAVE */
	memset(POWERADD->UsingRow,0,101);
	POWERADD->table_row=0;
	FINDRSCSETINT(POWERADD->addrsrc,"CURRENT ROW",(POWERADD->table_row+1));
	updateallrsrc(POWERADD->addrsrc);
	AfterWrites(add_rsrc,POWERADD,FALSE);
}
static void save_record_now(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	readscreen(add_rsrc,POWERADD);
	save_check(add_rsrc,POWERADD,TRUE);
}
static short check_row_quick(RDArsrc *add_rsrc,PowerAdd *POWERADD,short warn,short save)
{
	AddEditWdgt *EWR=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG check_row [%d].",POWERADD->table_row+1);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
/*
		SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
*/
		GetAddEditWdgts(add_rsrc,POWERADD);
		GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,FALSE,FALSE);
		ComputeAddEditWdgts(add_rsrc,POWERADD,2,NULL,POWERADDSubData,(void *)POWERADD);
		if(!SetPowerAddSupportingFiles(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,FALSE,TRUE)) 
		{
			return(-1);
		}
/*
		if(!SAVE_POWERADD_CHECK(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD))
		{
			return(-1);
		}
*/
		EWR=FINDEDITWDGT(POWERADD,"SAVE ERROR");
		if(EWR!=NULL)
		{
			if(!isEMPTY(EWR->data.string_value))
			{
				memset(stemp,0,101);
				sprintf(stemp,"SAVE ERROR on ROW %d",POWERADD->table_row+1);
				ERRORDIALOG(stemp,EWR->data.string_value,NULL,FALSE);
				return(-1);
			}
		}
		if(warn || INSIDE_PASTE)
		{
			EWR=FINDEDITWDGT(POWERADD,"SAVE WARNING");
			if(EWR!=NULL)
			{
				if(!isEMPTY(EWR->data.string_value))
				{
					memset(stemp,0,101);
					sprintf(stemp,"SAVE WARNING on ROW %d",POWERADD->table_row+1);
					ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",stemp,EWR->data.string_value,(save ? save_all_now_test2:NULL),NULL,FALSE,2,add_rsrc,(void *)POWERADD,NULL);
					return(-2);
				}
			}
		}
	}
	return(0);
}
static short check_row(RDArsrc *add_rsrc,PowerAdd *POWERADD,short warn,short save)
{
	AddEditWdgt *EWR=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG check_row [%d].",POWERADD->table_row+1);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
	if(UsingCurrentRow(add_rsrc,POWERADD))
	{
/* 1-9-2007 DCT */
		SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		if(!SAVE_POWERADD_CHECK(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD))
		{
			return(-1);
		}
		EWR=FINDEDITWDGT(POWERADD,"SAVE ERROR");
		if(EWR!=NULL)
		{
			if(!isEMPTY(EWR->data.string_value))
			{
				memset(stemp,0,101);
				sprintf(stemp,"SAVE ERROR on ROW %d",POWERADD->table_row+1);
				ERRORDIALOG(stemp,EWR->data.string_value,NULL,FALSE);
				return(-1);
			}
		}
		if(warn || INSIDE_PASTE)
		{
			EWR=FINDEDITWDGT(POWERADD,"SAVE WARNING");
			if(EWR!=NULL)
			{
				if(!isEMPTY(EWR->data.string_value))
				{
					memset(stemp,0,101);
					sprintf(stemp,"SAVE WARNING on ROW %d",POWERADD->table_row+1);
					ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",stemp,EWR->data.string_value,(save ? save_all_now_test2:NULL),NULL,FALSE,2,add_rsrc,(void *)POWERADD,NULL);
					return(-2);
				}
			}
		}
	}
	}
	return(0);
}
static void save_check(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	char *desc=NULL;

	if(POWERADD!=NULL)
	{
/* 1-9-2007 DCT */
		SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		if(!SAVE_POWERADD_CHECK(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD))
		{
			return;
		}
		if(!isEMPTY(POWERADD->save_expression))
		{
			if(PP_EVALUATEbol(POWERADD->save_expression,POWERADDSubData,(void *)POWERADD))
			{
				desc=PP_EVALUATEstr(POWERADD->save_error_desc,POWERADDSubData,POWERADD);
				ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
				if(desc!=NULL) Rfree(desc);
				return;
			}
		}
		if(!isEMPTY(POWERADD->save_warning))
		{
			if(PP_EVALUATEbol(POWERADD->save_warning,POWERADDSubData,(void *)POWERADD))
			{
				desc=PP_EVALUATEstr(POWERADD->save_warning_desc,
					POWERADDSubData,(void *)POWERADD);
				ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
					desc,save_record_warn,NULL,FALSE,2,add_rsrc,(void *)POWERADD,NULL);
				if(desc!=NULL) Rfree(desc);
				return;
			}
		}
		run_presave_buttons(add_rsrc,POWERADD,update);
		save_record(add_rsrc,POWERADD,update);
		run_postsave_buttons(add_rsrc,POWERADD,update);
		if((POWERADD->save_type==0 || POWERADD->save_type>3))
		{
/* Added to recompute widgets when using Power Save types */
			SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		}
		if(POWERADD->save_type==2 || POWERADD->save_type==4)
		{
			resetdefs(add_rsrc,POWERADD);
		} else if(POWERADD->save_type==3 || POWERADD->save_type==5)
		{
			quit_test(add_rsrc,POWERADD);
		}
	}
}
static void PerformWrites(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	AddWrite *Write=NULL,*pWrite=NULL;
	AddWriteField *WriteField=NULL;
	AddEditWdgt *EditWdgt=NULL;
	NRDfield *field=NULL;
	RDAaddsub *subord=NULL;
	int a,x,b;
	char dowrite=FALSE,*trnfld=NULL,*fieldname=NULL;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL,*holdrsrc=NULL;
	void *holdarg=NULL;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
		holdrsrc=PSTRUCT->r;
	}
	holdarg=PSTRUCT->arg;
	if(POWERADD->Writes!=NULL)
	{
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
			{
				if(EditWdgt->id_mgt==TRUE)
				{
					EditWdgt->wrote_id=TRUE;
				}
			}
		}
		if(POWERADD->save_type==0 || POWERADD->save_type>3)
		{
			tmprsrc=RDArsrcNEW(POWERADD->module,NULL);
			GET_POWERADD_INCVARS(tmprsrc,POWERADD);
			GET_POWERADD_SCREEN_VIRTUALS(tmprsrc,0,TRUE,FALSE,POWERADD);
		} else {
			tmprsrc=add_rsrc;
		}
		holdrsrc=PSTRUCT->r;
		holdarg=PSTRUCT->arg;
		PSTRUCT->r=tmprsrc;
		PSTRUCT->arg=POWERADD;
		for(a=0,Write=POWERADD->Writes;a<POWERADD->numWrites;
			++a,++Write)
		{
			if(isEMPTY(Write->expression)) dowrite=TRUE;
				else dowrite=PP_EVALUATEbol(Write->expression,POWERADDSubData,(void *)POWERADD);
			if(dowrite==TRUE)
			{
			if(POWERADD->save_type==0 || POWERADD->save_type>3)
			{
			Write->closefile=FALSE;
			if(POWERADD->subord!=NULL)
			{
				for(x=0,subord=POWERADD->subord;x<POWERADD->subnum;++x,++subord)
				{
					if(!RDAstrcmp(subord->module,Write->module) && !RDAstrcmp(subord->filename,Write->filename)) break;
				}
				if(x<POWERADD->subnum)
				{
					Write->fileno=subord->fileno;
					Write->closefile=FALSE;
					Write->zerofile=FALSE;
				}
			}
			if(Write->fileno==(-1))
			{
				for(b=0,pWrite=POWERADD->Writes;b<a;++b,++pWrite)
				{
					if(!RDAstrcmp(pWrite->module,Write->module) && !RDAstrcmp(pWrite->filename,Write->filename)) break;
				}
				if(a<b)
				{
					Write->fileno=pWrite->fileno;
					Write->closefile=FALSE;
					Write->zerofile=TRUE;
				}
			}
			if(Write->fileno==(-1))
			{
				Write->fileno=OPNNRDsec(Write->module,Write->filename,TRUE,TRUE,FALSE);
				addDFincvir(tmprsrc,Write->module,Write->filename,NULL,Write->fileno);
				Write->closefile=TRUE;
				Write->zerofile=TRUE;
			}
			}
			if(Write->fileno!=(-1))
			{
				if(Write->zerofile==TRUE) ZERNRD(Write->fileno);
				if(Write->fields!=NULL)
				{
					for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
					{
						fieldname=stripfieldname(WriteField->to_name);
						if(!isEMPTY(fieldname))
						{
							field=FLDNRD(Write->fileno,fieldname);
						}
						if(!WriteField->copy_type)
						{
							if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
							{
								trnfld=stralloc(fieldname);
								if(fieldname!=NULL) Rfree(fieldname);
							} else {
								if(fieldname!=NULL) Rfree(fieldname);
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
										case CHARACTERS:
										case OPTIONALFIELDS:
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								}
							}
						} else {
							if(fieldname!=NULL) Rfree(fieldname);
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
									case CHARACTERS:
									case OPTIONALFIELDS:
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										if(field->data.string_value!=NULL)
										{
											field->dlen=RDAstrlen(field->data.string_value)+1;
											if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
										} else field->dlen=0;
										break;
									case BOOLNS:
										*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case SLONGV:
									case SCROLLEDLIST:
									case LONGV:
										*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
										*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
										{
											*field->data.float_value=round(*field->data.float_value);
										}
										break;
									default:
										prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
								}
							}
						}
					}
					LOCNRDFILE(Write->fileno);
					if(trnfld!=NULL)
					{
						ADVWRTTRANSsec(Write->fileno,1,trnfld,NULL,POWERADDSubData,(void *)POWERADD);	
						Rfree(trnfld);
						trnfld=NULL;
					} else if(ADVEQLNRDsec(Write->fileno,1,POWERADDSubData,(void *)POWERADD))
					{
						ADVWRTTRANSsec(Write->fileno,0,NULL,NULL,POWERADDSubData,(void *)POWERADD);	
					} else {
						prev=RDATDataNEW(Write->fileno);
						
						for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
						{
							fieldname=stripfieldname(WriteField->to_name);
							field=FLDNRD(Write->fileno,fieldname);
							if(!WriteField->copy_type)
							{
								if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
								{
									trnfld=stralloc(fieldname);
									if(fieldname!=NULL) Rfree(fieldname);
								} else {
									if(fieldname!=NULL) Rfree(fieldname);
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
											case CHARACTERS:
											case OPTIONALFIELDS:
												if(field->data.string_value!=NULL) Rfree(field->data.string_value);
												field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												if(field->data.string_value!=NULL)
												{
													field->dlen=RDAstrlen(field->data.string_value)+1;
													if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
												} else field->dlen=0;
												break;
											case BOOLNS:
												*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case SHORTV:
											case SSHORTV:
												*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case SLONGV:
											case SCROLLEDLIST:
											case LONGV:
												*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case DOUBLEV:
											case SDOUBLEV:
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case SDECIMALV:
												*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
												{
													*field->data.float_value=round(*field->data.float_value);
												}
												break;
											default:
												prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
										}
									} else {
									}
								}
							} else {		
								if(fieldname!=NULL) Rfree(fieldname);
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
										case CHARACTERS:
										case OPTIONALFIELDS:
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								} else {
								}
							}
						}
						ADVWRTTRANSsec(Write->fileno,0,NULL,prev,POWERADDSubData,(void *)POWERADD);	
						if(prev!=NULL) FreeRDATData(prev);
					}	
				} else {
					prterr("Error AddWriteField's are missing...");
				}
			} else {
				prterr("Error AddWrite File not found [%s][%s].",(Write->module!=NULL ? Write->module:""),(Write->filename!=NULL ? Write->filename:""));
			}	
			}
		}
	}
	DefaultAddEditWdgts(add_rsrc,POWERADD,4,POWERADDSubData,(void *)POWERADD);
}
static void AfterWrites(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	AddEditWdgt *AE;
	AddWrite *Write=NULL;
	RDArsrc *tmprsrc=NULL,*holdrsrc=NULL;
	void *holdarg=NULL;
	RDArmem *mem=NULL;
	ADDPassableStruct *PSTRUCT=NULL;
	int x=0;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	updateallrsrc(add_rsrc);
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,AE=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++AE)
		{
			if(POWERADD->AddType==0)
			{
				if(AE->id_mgt==TRUE)
				{
					REMOVENEWID(POWERADD,AE,POWERADDSubData,(void *)POWERADD);
				}
			}
		}
		if(POWERADD->save_type==0 || POWERADD->save_type>3)
		{
			if(POWERADD->Writes!=NULL)
			{
				for(x=0,Write=POWERADD->Writes;x<POWERADD->numWrites;++x,
					++Write)
				{
					if(Write->fileno!=(-1) && Write->closefile==TRUE) 
						CLSNRD(Write->fileno);
				}
			}
		}
	}
	PSTRUCT->r=holdrsrc;
	PSTRUCT->arg=holdarg;
	if(POWERADD->save_type==0 || POWERADD->save_type>3)
	{
		if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	}
	POWERADDADVupdateSCRNvirtuals(add_rsrc,POWERADDSubData,(void *)POWERADD,POWERADD);
	if(update)
	{
		makedflist(POWERADD);
		FINDRSCLISTAPPlib(add_rsrc,"PREVIOUS ADD'S",(POWERADD->PREVIOUSADD->numlibs-1),
			POWERADD->PREVIOUSADD);
		updatersrc(add_rsrc,"PREVIOUS ADD'S");
		++POWERADD->WRITE_COUNTER;
	}
	x=FINDRSC(add_rsrc,"SAVE");
	if(x!=(-1))
	{
		mem=add_rsrc->rscs+x;
		if(!isEMPTY(mem->transversal_expression))
		{
			ComputeNewTransversalADV(add_rsrc,add_rsrc->EvalStr,add_rsrc->EvalStrArgs,add_rsrc->SubFunc,add_rsrc->SubFuncArgs,mem->transversal_expression);
		}
	}
}
static void save_record(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update_list)
{
	PerformWrites(add_rsrc,POWERADD);
	AfterWrites(add_rsrc,POWERADD,TRUE);
}
static void quit_record_warn(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	run_prequit_buttons(add_rsrc,POWERADD,FALSE);
	quit_record(add_rsrc,POWERADD);
}
static void quit_test(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	char *desc=NULL;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
	PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	if(!isEMPTY(POWERADD->quit_error))
	{
		if(PP_EVALUATEbol(POWERADD->quit_error,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->quit_error_desc,POWERADDSubData,(void *)POWERADD);
			ERRORDIALOG("QUIT ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(POWERADD->quit_warning))
	{
		if(PP_EVALUATEbol(POWERADD->quit_warning,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->quit_warning_desc,POWERADDSubData,(void *)POWERADD);
			ADVWARNINGDIALOG("QUIT WARNING DIALOG SCREEN","QUIT WARNING",
				desc,quit_record_warn,NULL,FALSE,2,add_rsrc,POWERADD,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	run_prequit_buttons(add_rsrc,POWERADD,FALSE);
	if(POWERADD->level!=0)
	{
/*lint -e746 */
		if(PSTRUCT->quitfunc!=NULL) PSTRUCT->quitfunc(PSTRUCT->fromrsrc,PSTRUCT->fromarg);
/*lint +e746 */
	}
	quit_record(add_rsrc,POWERADD);
	}
}
static void quit_record(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	APPlib *args=NULL;
	AddEditWdgt *EditWdgt=NULL;
	int x=0;
	int level=0;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	level=POWERADD->level;
	if(POWERADD!=NULL)
	{
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
		{
			if(POWERADD->AddType==0)
			{
				if(EditWdgt->id_mgt==TRUE)
				{
					UndoASSIGNID(POWERADD,EditWdgt,POWERADDSubData,(void *)POWERADD);
				}
			} else {

				if(EditWdgt->id_mgt==TRUE && EditWdgt->wrote_id==FALSE)
				{
					UndoASSIGNID(POWERADD,EditWdgt,POWERADDSubData,(void *)POWERADD);
				} else if(EditWdgt->id_mgt==TRUE && EditWdgt->wrote_id==TRUE)
				{
					REMOVENEWID(POWERADD,EditWdgt,POWERADDSubData,(void *)POWERADD);
				}
			}
		}
	}
	}
	if(add_rsrc!=NULL)
	{
		killwindow(add_rsrc);
		free_rsrc(add_rsrc); 
	}
	if(PSTRUCT!=NULL) Rfree(PSTRUCT);

	if(POWERADD!=NULL)
	{
		if(POWERADD->PREVIOUSADD!=NULL) freeapplib(POWERADD->PREVIOUSADD);
		if(POWERADD->ADD_DEFINE_LIST!=NULL) free_rsrc(POWERADD->ADD_DEFINE_LIST);
		CLOSE_POWERADD_FILES(POWERADD);
		if((POWERADD->WRITE_COUNTER>0) && !isEMPTY(POWERADD->reportname))
		{
			args=APPlibNEW();
			addAPPlib(args,POWERADD->module);
			addAPPlib(args,POWERADD->reportname);
			ExecuteProgram("doreport",args);
			if(args!=NULL) freeapplib(args);
		}
		FreeEditWdgts(POWERADD);
		FreePowerAdd(POWERADD);
	}
	if(level==0)
	{
		ShutdownSubsystems();
	}
}
void xGET_POWERADD_SCREEN_VIRTUALS(RDArsrc *r,short type,short force,
	short includelists,PowerAdd *POWERADD,int line,char *file)
{
	char *libx=NULL,*tmpx=NULL,addvf=FALSE,temp[501];
	char *tx=NULL;
	APPlib *filenames=NULL,*ScrolledLists=NULL;
	int x,y,z,g,vc=0,lc=0;
	DFvirtual *d=NULL;
	DFincvir *i,*a;
	RDAwdgt *wdgt;
	int count=0,got_it=0;
	int mi=(-1);
/*
	RDArmem *mem=NULL;
*/
	RDAvirtual *v,*v1;
	RDAScrolledList *list=NULL,*LIST=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG GET_SCREEN_VIRTUALS Creating Virtual Fields used on Custom Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!force)
	{
		if(r->scn==NULL && !isEMPTY(r->screen) && RDA_NOGUI==FALSE)
		{
			r->scn=RDAscrnNEW(r->module,r->screen);
			if(findscrnbin(r->scn)) 
			{
				prterr("Error couldn't read Screen Binary [%s] [%s] from Binary Library [%s.SCN]",r->module,(r->screen!=NULL ? r->screen:""),r->module);
				free_scrn(r->scn);
				r->scn=NULL;
				return;
			} 
		} else {
			r->scn=NULL;
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
	filenames=getunsortedlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
	tmpx=NULL;
	libx=NULL;
	if(includelists && (force || (!force && r->scn!=NULL)))
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
	} else {
		if(ScrolledLists==NULL) ScrolledLists=APPlibNEW();
	}
	if(filenames==NULL) filenames=APPlibNEW();
	if(!force && r->scn!=NULL)
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
							if(wdgt->type==8)
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
								++count;
							}
							FreeDFvirtual(d);
							d=NULL;
						}
						}
					}
				} else if(((wdgt->type==7) && wdgt->resource_name!=NULL)
					&& !type && includelists)
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
								++count;
							}
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
				}
			}
		}
	} else { /* FORCED */
		if(includelists)
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
						&& includelists)
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
									++count;
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
					addSCRNvirtual(r,d->name,d->type,d->length,
						d->expression,d->when);
					++count;
				}
				FreeDFvirtual(d);
				d=NULL;
			}
			}
		}
	} 
/* GETTING VIRTUAL FIELDS USED IN OTHER VIRTUAL FIELDS */
	if(!force && count>0)
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
				got_it=FINDVIRTUAL(r,filenames->libs[x]);
				if(got_it==(-1))
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
								ADVaddSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when,FALSE);
								FreeDFvirtual(d);
								d=NULL;
							}
							}
						}
					}
				}
			}
		}
		if(includelists)
		{
			for(y=0;y<ScrolledLists->numlibs;++y)
			{
				v=r->virflds+vc;
				if(v->expression!=NULL)
				{
					tx=RDAstrstr(v->expression,ScrolledLists->libs[y]); 
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
				}
			}
			++vc;
		}
	}
	if(includelists)
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
					tx=RDAstrstr(LIST->select_formula,filenames->libs[x]); 
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
				}
				if(!isEMPTY(LIST->format_formula))
				{
					tx=RDAstrstr(LIST->format_formula,filenames->libs[x]); 
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
				}
			}
			for(y=0;y<ScrolledLists->numlibs;++y)
			{
				LIST=r->lists+lc;
				if(!isEMPTY(LIST->select_formula) && 
					RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{
					tx=RDAstrstr(LIST->select_formula,
						ScrolledLists->libs[y]); 
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
								list->list,list->format_formula,
								list->unformat_formula,
								list->reformat_formula,
								list->desc,list->timing);
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
				}
/*
				if(!isEMPTY(LIST->format_formula) &&
					RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{	
*/
				if(!isEMPTY(LIST->format_formula))
				{
				if(RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{
					tx=RDAstrstr(LIST->select_formula,ScrolledLists->libs[y]); 
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
				}
				}
			}
			++lc;
		}
	}		
	}
	freeapplib(filenames);
	filenames=NULL;
	if(includelists)
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
		freeapplib(ScrolledLists);
		ScrolledLists=NULL;
		if(libx!=NULL) Rfree(libx);
		if(tmpx!=NULL) Rfree(tmpx);
		tmpx=NULL;
		libx=NULL;
	}
}
static void COPYTOPRIMARYWIDGET(RDArmem *member,AddEditWdgt *wdgt)
{
	RDArmem *m=NULL;
	RDArsrc *parent=NULL;
	int c=0,lc=0,x=0,r=0;
	PowerAdd *P=NULL;
	AddEditWdgt *EW=NULL,*EWR=NULL;

	parent=(RDArsrc *)member->parent;
	P=(PowerAdd *)wdgt->parent;
	c=member->table_row+1;
	if(member->table_row!=P->table_row)
	{
		check_row(parent,P,TRUE,FALSE);
	} 
	FINDRSCSETINT(parent,"CURRENT ROW",c);
	updatersrc(parent,"CURRENT ROW");
	if(member->Table!=NULL)
	{
		P->TableWidget=(void *)member->Table;
		P->table_row=member->table_row;
		P->table_col=member->table_col;
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("DIAG COPYTOPRIMARYWIDGET member->rscrname [%s] row [%d] column [%d] ",member->rscrname,P->table_row+1,P->table_col+1);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	}
	readmember(member);
	r=FINDRSC(member->parent,wdgt->name);
	if(r>(-1))
	{
		m=parent->rscs+r;
		FINDRSCGETINT(parent,"LAST ROW",&lc);
		COPYRSRC2RSRC(parent,member->rscrname,wdgt->name);
		updatersrc(parent,wdgt->name);
		if(c!=lc && P->UsingRow[P->table_row])
		{
			for(x=0,EW=P->EditWdgts;x<P->numwdgts;++x,++EW)
			{
				if(EW->loadvirtual==2 || EW->loadvirtual==3)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%04d:%s",c,EW->name);
					EWR=FINDEDITWDGT(P,stemp);
					CopyEditWdgt2EditWdgt(EWR,EW);
					switch(EW->type)
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
							FINDRSCSETSTRING(parent,EW->name,EW->data.string_value);
							break;
						case BOOLNS:
						case CHARACTERS:
							FINDRSCSETCHAR(parent,EW->name,*EW->data.string_value);
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							FINDRSCSETDOUBLE(parent,EW->name,*EW->data.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							FINDRSCSETSHORT(parent,EW->name,*EW->data.short_value);
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							FINDRSCSETINT(parent,EW->name,*EW->data.integer_value);
							break;
						default:
						case BUTTONS:
							break;
					}
					updatersrc(parent,EW->name);
				}
			}
		} else if(c!=lc && !P->UsingRow[P->table_row])
		{
			for(x=0,EW=P->EditWdgts;x<P->numwdgts;++x,++EW)
			{
				if(!strncmp(EW->name,"LAST JOURNAL",12))
				{
					switch(EW->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
							if(EW->data.string_value!=NULL && EW->dlen>0)
							{
								memset(EW->data.string_value,0,EW->dlen);
								FINDRSCSETSTRING(parent,EW->name,EW->data.string_value);
								updatersrc(parent,EW->name);
							}
							break;
						default:
							break;
					}
				}
			}
			for(x=0,EW=P->EditWdgts;x<P->numwdgts;++x,++EW)
			{
				if(EW->loadvirtual==2 || EW->loadvirtual==3)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%04d:%s",c,EW->name);
					EWR=FINDEDITWDGT(P,stemp);
					switch(EW->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
							if(EW->loadvirtual!=3)
							{
								CopyEditWdgt2EditWdgt(EWR,EW);
								FINDRSCSETSTRING(parent,EW->name,EW->data.string_value);
								updatersrc(parent,EW->name);
							}
							break;
						case DATES:
							if(EW->loadvirtual!=3)
							{
								CopyEditWdgt2EditWdgt(EWR,EW);
								FINDRSCSETSTRING(parent,EW->name,EW->data.string_value);
								updatersrc(parent,EW->name);
							}
							break;
						case TIMES:
						case SOCSECNUM:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
							if(EW->loadvirtual!=3)
							{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETSTRING(parent,EW->name,EW->data.string_value);
							updatersrc(parent,EW->name);
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(EW->loadvirtual!=3)
							{
								CopyEditWdgt2EditWdgt(EWR,EW);
								FINDRSCSETSHORT(parent,EW->name,*EW->data.short_value);
								updatersrc(parent,EW->name);
							}
							break;
						case SLONGV:
						case SCROLLEDLIST:
						case LONGV:
							if(EW->loadvirtual!=3)
							{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETINT(parent,EW->name,*EW->data.integer_value);
							updatersrc(parent,EW->name);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(EW->loadvirtual!=3)
							{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETDOUBLE(parent,EW->name,*EW->data.float_value);
							updatersrc(parent,EW->name);
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							if(EW->loadvirtual!=3)
							{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETCHAR(parent,EW->name,*EW->data.string_value);
							updatersrc(parent,EW->name);
							}
							break;
						default:
							break;
					}
				}
			}
		}
		ExecuteRDArmemFunction(m);
		updateallrsrc(parent);
	}

}
static void deletethisrow(RDArmem *member,PowerAdd *POWERADD)
{
	int x=0,row=0,y=0;
	AddEditWdgt *EW=NULL,*EWR=NULL;
	RDArsrc *r=NULL;
	RDArmem *mem=NULL;

	r=(RDArsrc *)member->parent;
	memset(stemp,0,101);
	sprintf(stemp,"%.4s",&member->rscrname[0]);
	row=atoi(stemp);
	POWERADD->UsingRow[row-1]=0;
	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(EW->loadvirtual==2 || EW->loadvirtual==3)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%04d:%s",row,EW->name);
			EWR=FINDEDITWDGT(POWERADD,stemp);
			if(EWR!=NULL)
			{
				switch(EWR->type)
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
						if(!isEMPTY(EWR->data.string_value))
						{
							memset(EWR->data.string_value,0,EWR->dlen);
						}
						FINDRSCSETSTRING(r,EWR->name,EWR->data.string_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETSTRING(r,EW->name,EW->data.string_value);
						}
						break;
					case BOOLNS:
						*EWR->data.string_value=0;
						FINDRSCSETCHAR(r,EWR->name,*EWR->data.string_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETCHAR(r,EW->name,*EW->data.string_value);
						}
						break;
					case CHARACTERS:
						*EWR->data.string_value=0;
						FINDRSCSETCHAR(r,EWR->name,*EWR->data.string_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETCHAR(r,EW->name,*EW->data.string_value);
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*EWR->data.float_value=0.0;
						FINDRSCSETDOUBLE(r,EWR->name,*EWR->data.float_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETDOUBLE(r,EW->name,*EW->data.float_value);
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(RDAstrstr(EWR->name,"ACCOUNT TYPE") || RDAstrstr(EWR->name,"DEFINITION TYPE"))
						{
/* skip blanking these */
						} else {
						*EWR->data.short_value=0;
						FINDRSCSETSHORT(r,EWR->name,*EWR->data.short_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETSHORT(r,EW->name,*EW->data.short_value);
						}
						}
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						*EWR->data.integer_value=0;
						FINDRSCSETINT(r,EWR->name,*EWR->data.integer_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETINT(r,EW->name,*EW->data.integer_value);
						}
						break;
					default:
					case BUTTONS:
						break;
				}
				updatersrc(r,EWR->name);
				if(row==POWERADD->table_row+1)
				{
					updatersrc(r,EW->name);
					y=FINDRSC(r,EW->name);
					if(y>(-1))
					{
						mem=r->rscs+y;
						ExecuteRDArmemFunction(mem);
					}
				}
			}
		}
	}
}
static void deletecurrentrow(RDArmem *member,PowerAdd *POWERADD)
{
	int x=0,row=0,y=0;
	AddEditWdgt *EW=NULL,*EWR=NULL;
	RDArsrc *r=NULL;
	RDArmem *mem=NULL;

	r=(RDArsrc *)member->parent;
	readwidget(r,"CURRENT ROW");
	FINDRSCGETINT(r,"CURRENT ROW",&row);
	POWERADD->UsingRow[row-1]=0;
	for(x=0,EW=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EW)
	{
		if(EW->loadvirtual==2 || EW->loadvirtual==3)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%04d:%s",row,EW->name);
			EWR=FINDEDITWDGT(POWERADD,stemp);
			if(EWR!=NULL)
			{
				switch(EWR->type)
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
						if(!isEMPTY(EWR->data.string_value))
						{
							memset(EWR->data.string_value,0,EWR->dlen);
						}
						FINDRSCSETSTRING(r,EWR->name,EWR->data.string_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETSTRING(r,EW->name,EW->data.string_value);
						}
						break;
					case BOOLNS:
						*EWR->data.string_value=0;
						FINDRSCSETCHAR(r,EWR->name,*EWR->data.string_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETCHAR(r,EW->name,*EW->data.string_value);
						}
						break;
					case CHARACTERS:
						*EWR->data.string_value=0;
						FINDRSCSETCHAR(r,EWR->name,*EWR->data.string_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETCHAR(r,EW->name,*EW->data.string_value);
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*EWR->data.float_value=0.0;
						FINDRSCSETDOUBLE(r,EWR->name,*EWR->data.float_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETDOUBLE(r,EW->name,*EW->data.float_value);
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(RDAstrstr(EWR->name,"ACCOUNT TYPE") || RDAstrstr(EWR->name,"DEFINITION TYPE"))
						{
/* skip blanking these */
						} else {
						*EWR->data.short_value=0;
						FINDRSCSETSHORT(r,EWR->name,*EWR->data.short_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETSHORT(r,EW->name,*EW->data.short_value);
						}
						}
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						*EWR->data.integer_value=0;
						FINDRSCSETINT(r,EWR->name,*EWR->data.integer_value);
						if(row==POWERADD->table_row+1)
						{
							CopyEditWdgt2EditWdgt(EWR,EW);
							FINDRSCSETINT(r,EW->name,*EW->data.integer_value);
						}
						break;
					default:
					case BUTTONS:
						break;
				}
				updatersrc(r,EWR->name);
				if(row==POWERADD->table_row+1)
				{
					updatersrc(r,EW->name);
					y=FINDRSC(r,EW->name);
					if(y>(-1))
					{
						mem=r->rscs+y;
						ExecuteRDArmemFunction(mem);
					}
				}
			}
		}
	}
}
void xMULTIPOWERADD(char *module,char *poweradd,int level,
	void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromarg,
	short APPmainLoop,int line,char *file)
{
	char *warnmessage=NULL;
	RDArsrc *add_rsrc=NULL;
	AddEditWdgt *EditWdgt=NULL;
	char *temp=NULL,mystemp[101];
	int x=0,y=0;
	short count=0,flen=0;
	PowerAdd *POWERADD=NULL;
	ADDPassableStruct *PSTRUCT=NULL;
	RDArmem *member=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG MULTIPOWERADD(%s,%s) called at line [%d] program [%s].",module,poweradd,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	POWERADD=PowerAddNew(module,poweradd);
	POWERADD->APPmainLoop=APPmainLoop;
	POWERADD->AddType=TRUE;
	PSTRUCT=Rmalloc(sizeof(ADDPassableStruct));
	PSTRUCT->r=add_rsrc;
	PSTRUCT->quitfunc=quitfunc;
	PSTRUCT->fromrsrc=fromrsrc;
	PSTRUCT->fromarg=fromarg;

	if(findPowerAdd(POWERADD))
	{
		warnmessage=Rmalloc(300+RDAstrlen(module)+RDAstrlen(poweradd));
		sprintf(warnmessage,"Error Power Add Definition not found, this process cannot be executed without it's definition [%s] in the [%s.ADD] library.",poweradd,module);
		ERRORDIALOG("MAINTAIN POWERADD NOT FOUND.",warnmessage,NULL,(level==0?TRUE:FALSE)); 
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(POWERADD);
		return;
	} else {
		if(!OPEN_POWERADD_FILES(POWERADD))
		{
			doexit(POWERADD);
			prterr("Error cannot open files for this power add..");
			return;
		}
		ReplicateEditWdgts(POWERADD);
		Set_Skip_Widgets(POWERADD);

		AllocateEditWdgts(POWERADD);
		PP_translate_POWERADD(POWERADD);

		add_rsrc=RDArsrcNEW(POWERADD->module,POWERADD->addscreen);
		POWERADD->ADD_DEFINE_LIST=RDArsrcNEW(POWERADD->module,POWERADD->define_list);
		PSTRUCT->r=add_rsrc;
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;

		POWERADD->SubFunc=POWERADDSubData;
		POWERADD->SubFuncArgs=POWERADD;
		POWERADD->addrsrc=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;
		POWERADD->level=level;
		POWERADD->table_row=0;
		POWERADD->table_col=0;

		POWERADD->PREVIOUSADD=APPlibNEW();
		addAPPlib(POWERADD->PREVIOUSADD,"Nothing Added this Session.");
		addlstrsrc(add_rsrc,"PREVIOUS ADD'S",&x,TRUE,NULL,POWERADD->PREVIOUSADD->numlibs,
			&POWERADD->PREVIOUSADD->libs,NULL);
		GET_POWERADD_INCVARS(add_rsrc,POWERADD);
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
				++x,++EditWdgt)
			{
				if(!RDAstrcmp(EditWdgt->name,"INCREMENT REFERENCE NUMBER"))
				{
					++count;
					temp=Rmalloc(RDAstrlen(EditWdgt->name)+10);
					sprintf(temp,"%s POSITION",EditWdgt->name);
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&count,TRUE);
					sprintf(temp,"%s LENGTH",EditWdgt->name);
					flen=EditWdgt->len;
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&flen,TRUE);
					if(temp!=NULL) Rfree(temp);
					makefieldrsrc(add_rsrc,EditWdgt->name,EditWdgt->type,
						EditWdgt->len,TRUE);
					FINDRSCSETFUNC(add_rsrc,EditWdgt->name,INCREFNUM,EditWdgt);
				} else if(!isEMPTY(EditWdgt->name) && EditWdgt->loadvirtual!=4)
				{
					++count;
					temp=Rmalloc(RDAstrlen(EditWdgt->name)+10);
					sprintf(temp,"%s POSITION",EditWdgt->name);
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&count,TRUE);
					sprintf(temp,"%s LENGTH",EditWdgt->name);
					flen=EditWdgt->len;
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&flen,TRUE);
					if(temp!=NULL) Rfree(temp);

					makefieldrsrc(add_rsrc,EditWdgt->name,EditWdgt->type,
						EditWdgt->len,TRUE);
					if(EditWdgt->id_mgt==TRUE)
					{
						FINDRSCSETFUNC(add_rsrc,EditWdgt->name,SETNEWIDANDFILES,EditWdgt);
					} else {
						FINDRSCSETFUNC(add_rsrc,EditWdgt->name,SETFILES,EditWdgt);
					}
					if(RDAstrcmp(EditWdgt->name,"IMAGE"))
					{
						switch(EditWdgt->type)
						{
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								FINDRSCSETCALLBACKTYPE(add_rsrc,EditWdgt->name,TRUE);
								break;
							default:
								break;
						}
					}
					if(EditWdgt->loadvirtual==2 || EditWdgt->loadvirtual==3)
					{
						for(y=0;y<(REPLICATE+1);++y)
						{
							memset(mystemp,0,101);
							if(y==0)
							{
								sprintf(mystemp,"STORE:%s",EditWdgt->name);
							} else {
								sprintf(mystemp,"%04d:%s",y,EditWdgt->name);
							}
							makefieldrsrc(add_rsrc,mystemp,EditWdgt->type,
								EditWdgt->len,TRUE);
							ADVFINDRSCSETFUNC(add_rsrc,mystemp,COPYTOPRIMARYWIDGET,EditWdgt,TRUE);
						}
					}
				}
			}
		}
		for(y=1;y<(REPLICATE+1);++y)
		{
			memset(stemp,0,101);
			sprintf(stemp,"%04d:DELETE",y);
			addbtnrsrc(add_rsrc,stemp,TRUE,deletethisrow,POWERADD);
			member=(RDArmem *)add_rsrc->rscs+(add_rsrc->numrscs-1);
			member->function_type=1;
		}
		makefieldrsrc(add_rsrc,"CURRENT ROW",LONGV,4,TRUE);
		FINDRSCSETEDITABLE(add_rsrc,"CURRENT ROW",FALSE);
		x=POWERADD->table_row+1;
		FINDRSCSETINT(add_rsrc,"CURRENT ROW",x);
		ADVFINDRSCSETFUNC(add_rsrc,"CURRENT ROW",currentrow,POWERADD,TRUE);
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"DEFAULTS",TRUE,SaveDefaultsUsedOnly,NULL);
		addrfkwrsrc(POWERADD->ADD_DEFINE_LIST,"SELECT",TRUE,savelistcb,add_rsrc);
		addrfkwrsrc(POWERADD->ADD_DEFINE_LIST,"QUIT",TRUE,NULL_RSCS,NULL);
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		GET_POWERADD_SCREEN_VIRTUALS(add_rsrc,0,FALSE,TRUE,POWERADD);
		SET_POWERADD_FILE2RSRCS(add_rsrc,POWERADD);

		ADVSCRNvirtual2rsrc(add_rsrc,POWERADDSubData,(void *)POWERADD);
		addbtnrsrc(add_rsrc,"DELETE CURRENT ROW",TRUE,deletecurrentrow,POWERADD);
		addbtnrsrc(add_rsrc,"SAVE",TRUE,save_all_now_test,POWERADD);
		addbtnrsrc(add_rsrc,"DEFAULTS",TRUE,SaveDefaults,POWERADD);
		addbtnrsrc(add_rsrc,"RESET DEFAULTS",TRUE,resetdefs,POWERADD);
		addbtnrsrc(add_rsrc,"DEFINE LIST",TRUE,add_definelist,POWERADD);
		addrfcbrsrc(add_rsrc,"QUIT",TRUE,quit_test,POWERADD);
		addrfcbrsrc(add_rsrc,"HELP",TRUE,screenhelp,POWERADD);
		addrfcbrsrc(add_rsrc,"PRINT RESOURCES",TRUE,printrsrcs,POWERADD);
		MakePowerAddButtonsADV(add_rsrc,POWERADD,updateallPowerAdd);

		PSTRUCT->r=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;

		DefaultScreens(add_rsrc);
		GetAddEditWdgts(add_rsrc,POWERADD);
		ReadRDAScrolledLists(add_rsrc);
		READEditWdgts(POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		DefaultScreens(POWERADD->ADD_DEFINE_LIST);
		GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,FALSE,FALSE);
		DefaultAddEditWdgts(add_rsrc,POWERADD,0,POWERADDSubData,(void *)POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		POWERADDcomputeallSCRNvirtuals(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;
		PSTRUCT->r=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;
		SetAddEditWdgts(add_rsrc,POWERADD);
		xmakescrn(add_rsrc,TRUE,DOADD_GUIEVAL,(void *)POWERADD,addoswidgets,DOADD_GUIEVALSTR,(void *)POWERADD,POWERADDSubData,POWERADD,NULL,__LINE__,__FILE__);
		SetAddEditWdgts(add_rsrc,POWERADD);
		if(!APPmainLoop)
		{
			APPmainLoop=TRUE;
			POWERADD->APPmainLoop=TRUE;
			RDAAPPMAINLOOP();
		}
	}
}
