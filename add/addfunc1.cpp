/* addfuncs1.c - Power Add Functions */
#include<powadd.hpp>

void xCopyEditWdgt2EditWdgt(AddEditWdgt *EWfrom,AddEditWdgt *EWto,int line,char *file)
{

	int y=0;
	PowerAdd *POWERADD=NULL;
	short xvalue=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG CopyEditWdgt2EditWdgt From [%s] To [%s] at line [%d] program [%s].",EWfrom->name,EWto->name,line,file);
		ShowPowerAddEditWdgt(EWfrom);
		ShowPowerAddEditWdgt(EWto);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	POWERADD=(PowerAdd *)EWfrom->parent;
	switch(EWfrom->type)
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
			if(EWfrom->data.string_value!=NULL && EWfrom->dlen>0)
			{
				QUICKALLOC(EWto->data.string_value,EWto->dlen,EWfrom->dlen);
				memcpy(EWto->data.string_value,EWfrom->data.string_value,EWfrom->dlen);
				if(EWto->dlen>EWto->len && EWto->len>0)
					EWto->data.string_value[EWto->len]=0;
			} else if(EWto->dlen>0)
			{
				memset(EWto->data.string_value,0,EWto->dlen);
			} else {
				EWto->data.string_value=NULL;
				EWto->dlen=0;
			}
			break;
		case BOOLNS:
			*EWto->data.string_value=*EWfrom->data.string_value;
			break;
		case CHARACTERS:
			*EWto->data.string_value=*EWfrom->data.string_value;
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
			*EWto->data.float_value=*EWfrom->data.float_value;
			xvalue=(short)*EWfrom->data.float_value;
			break;
		case SHORTV:
		case SSHORTV:
			*EWto->data.short_value=*EWfrom->data.short_value;
			xvalue=*EWfrom->data.short_value;
			break;
		case LONGV:
		case SLONGV:
		case SCROLLEDLIST:
			*EWto->data.integer_value=*EWfrom->data.integer_value;
			xvalue=(short)*EWfrom->data.integer_value;
			break;
		default:
		case BUTTONS:
			break;
	}
	if(EWfrom->setaccttype==TRUE)
	{
		if(EWfrom->acctrsc!=NULL)
		{
			for(y=0;y<EWfrom->acctrsc->numlibs;++y)
			{
				memset(stemp,0,101);
				sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EWfrom->acctrsc->libs[y]);
				FINDRSCSETACCTTYPE(POWERADD->addrsrc,stemp,xvalue);
			}
		}
	}
	if(EWfrom->setdeftype==TRUE)
	{
		if(EWfrom->defrsc!=NULL)
		{
			for(y=0;y<EWfrom->defrsc->numlibs;++y)
			{
				memset(stemp,0,101);
				sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EWfrom->defrsc->libs[y]);
				FINDRSCSETDEFTYPE(POWERADD->addrsrc,stemp,xvalue);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG After CopyEditWdgt2EditWdgt From [%s] To [%s] at line [%d] program [%s].",EWfrom->name,EWto->name,line,file);
		ShowPowerAddEditWdgt(EWfrom);
		ShowPowerAddEditWdgt(EWto);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
void xShowPowerAddEditWdgt(AddEditWdgt *EditWdgt,int line,char *file)
{
	prterr("ShowPowerAddEditWdgt at line [%d] program [%s].\n",line,file);
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
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case ZIPCODE:
			prterr("EditWdgt [%s] [%d] [%s] ",EditWdgt->name,EditWdgt->type,(EditWdgt->data.string_value!=NULL ? EditWdgt->data.string_value:""));TRACE;
			break;
		case BOOLNS:
			prterr("EditWdgt [%s] [%d] [%d] ",EditWdgt->name,EditWdgt->type,*EditWdgt->data.string_value);TRACE;
			break;
		case CHARACTERS:
			prterr("EditWdgt [%s] [%d] [%c] ",EditWdgt->name,EditWdgt->type,*EditWdgt->data.string_value);TRACE;
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
			prterr("EditWdgt [%s] [%d] [%f] ",EditWdgt->name,EditWdgt->type,*EditWdgt->data.float_value);TRACE;
			break;
		case SHORTV:
		case SSHORTV:
			prterr("EditWdgt [%s] [%d] [%d] ",EditWdgt->name,EditWdgt->type,*EditWdgt->data.short_value);TRACE;
			break;
		case LONGV:
		case SLONGV:
		case SCROLLEDLIST:
			prterr("EditWdgt [%s] [%d] [%d] ",EditWdgt->name,EditWdgt->type,*EditWdgt->data.integer_value);TRACE;
			break;
		default:
		case BUTTONS:
			break;
	}
}
void xShowPowerAdd(PowerAdd *P,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	int x=0;
	
	prterr("ShowPowerAdd at line [%d] program [%s].\n",line,file);
	if(P!=NULL)
	{
		for(x=0,EditWdgt=P->EditWdgts;x<P->numwdgts;++x,++EditWdgt)
		{
			ShowPowerAddEditWdgt(EditWdgt);
#ifdef XXXX
			if(EditWdgt->id_mgt)
			{
				prterr("\tid_mgt [%s] mgt_expression [%s] ","True",(EditWdgt->mgt_expression!=NULL ? EditWdgt->mgt_expression:""));TRACE;
				prterr("\t\tmodule [%s] filename [%s] fileno [%d] ",(EditWdgt->module!=NULL ? EditWdgt->module:""),(EditWdgt->filename!=NULL ? EditWdgt->filename:""),EditWdgt->fileno);
			}
#endif /* XXXX */
		}	
	} else {
		prterr("ShowPowerAdd PowerAdd P is NULL.");TRACE;
	}
}
void xGET_POWERADD_INCVARS(RDArsrc *mtnrsrc,PowerAdd *POWERADD,int line,char *file)
{
	RDAaddsub *addsub=NULL;
	AddWrite *Write=NULL;
	int x;

	if(POWERADD!=NULL)
	{
		if(POWERADD->subord!=NULL)
		{
			for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
			{
				addDFincvir(mtnrsrc,addsub->module,addsub->filename,NULL,addsub->fileno);
   			}
		}
		if(POWERADD->save_type>0 && POWERADD->save_type<4)
		{
			if(POWERADD->Writes!=NULL)
			{
				for(x=0,Write=POWERADD->Writes;x<POWERADD->numWrites;++x,++Write)
				{
					if(Write->closefile==TRUE)
					{
						addDFincvir(mtnrsrc,Write->module,Write->filename,NULL,Write->fileno);
					}
				}
			}
		} 
	}
}
void xSET_POWERADD_FILE2RSRCS(RDArsrc *mtnrsrc,PowerAdd *POWERADD,int line,char *file)
{
	RDAaddsub *addsub=NULL;
	int x;

	if(POWERADD!=NULL)
	{
		if(POWERADD->subord!=NULL)
		{
			for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
			{
				if(addsub->skip_widget==FALSE)
				{
				file2rsrc(addsub->fileno,mtnrsrc,FALSE);
				}
			}
		}
	}
}
char xSAVE_POWERADD_CHECK(RDArsrc *mrsrc,PowerAdd *POWERADD,void (*SubFunc)(...),void *args,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG SaveCheck at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(SetPowerAddSupportingFiles(mrsrc,POWERADD,SubFunc,args,TRUE,TRUE));
}
void xGET_POWERADD_SUPPORTING(RDArsrc *mtnrsrc,PowerAdd *POWERADD,void (*SubFunc)(...),void *args,short update_rsrcs,short display_diags,int line,char *file)
{
	SetPowerAddSupportingFiles(mtnrsrc,POWERADD,SubFunc,args,update_rsrcs,display_diags);
}
void xSET_POWERADD_MGT(RDArsrc *mtnrsrc,PowerAdd *POWERADD,char *value,void (*SubFunc)(...),void *args,int line,char *file)
{
	readallwidgets(mtnrsrc);
	GetAddEditWdgts(mtnrsrc,POWERADD);
	GET_POWERADD_SUPPORTING(mtnrsrc,POWERADD,SubFunc,args,TRUE,FALSE);
	ADVDefaultAddEditWdgts(mtnrsrc,POWERADD,2,value,SubFunc,args);
	SetAddEditWdgts(mtnrsrc,POWERADD);
	xcomputeandupdateallSCRNvirtuals(mtnrsrc,SubFunc,args,FALSE,FALSE,NULL,
		POWERADD,__LINE__,__FILE__);
	updateallrsrc(mtnrsrc);
	SetAllEditableAndSensitive(mtnrsrc);
}
char run_changevalue_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE,r=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			if(button->type==5)
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
					r=TRUE;
				}
			}
		}
	}
	return(r);
}
void xSET_POWERADD(RDArsrc *mtnrsrc,PowerAdd *POWERADD,void (*SubFunc)(...),void *args,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG SET_POWERADD at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	readallwidgets(mtnrsrc);
	GetAddEditWdgts(mtnrsrc,POWERADD);
	GET_POWERADD_SUPPORTING(mtnrsrc,POWERADD,SubFunc,args,TRUE,FALSE);
	DefaultAddEditWdgts(mtnrsrc,POWERADD,2,SubFunc,args);
	SetAddEditWdgts(mtnrsrc,POWERADD);
	xcomputeandupdateallSCRNvirtuals(mtnrsrc,SubFunc,args,FALSE,FALSE,NULL,POWERADD,__LINE__,__FILE__);
	updateallrsrc(mtnrsrc);
	SetAllEditableAndSensitive(mtnrsrc);
	run_changevalue_buttons(mtnrsrc,POWERADD);
}
void xSET_POWERADD_KEYFUNC(RDArsrc *mtnrsrc,PowerAdd *POWERADD,
	void (*SubFunc)(...),void *args,int line,char *file)
{
	readallwidgets(mtnrsrc);
	GetAddEditWdgts(mtnrsrc,POWERADD);
	GET_POWERADD_SUPPORTING(mtnrsrc,POWERADD,SubFunc,args,TRUE,TRUE);
	DefaultAddEditWdgts(mtnrsrc,POWERADD,2,SubFunc,args);
	SetAddEditWdgts(mtnrsrc,POWERADD);
	updateallrsrc(mtnrsrc);
	POWERADDADVupdateSCRNvirtuals(mtnrsrc,SubFunc,args,POWERADD);
}
short xReturnPowerAddSupportingFileno(PowerAdd *POWERADD,char *mod,char *fil,short occurrence,int line,char *file)
{
	short count=0;
	int x=0;
	RDAaddsub *addsub=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG ReturnPowerAddSupportingFileno for Module [%s] File [%s] Occurrence [%d] at line [%d] program [%s].",mod,fil,occurrence,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		if(POWERADD->subord!=NULL)
		{
			for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
			{
				if(!RDAstrcmp(addsub->module,mod) && !RDAstrcmp(addsub->filename,fil))
				{
					++count;
					if(count==occurrence) return(addsub->fileno);
				}
			}
		}
		return(-1);
	}
	return(-1);
}
short xSetPowerAddSupportingWritable(PowerAdd *POWERADD,char *mod,char *fil,short occurrence,
	short writable,char *editable,int line,char *file)
{
	short count=0;
	int x=0;
	RDAaddsub *addsub=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG SetPowerAddSupportingWritable for Module [%s] File [%s] Occurance [%d] Writable [%s] at line [%d] program [%s].",mod,fil,occurrence,(writable==TRUE?"TRUE":"FALSE"),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		if(POWERADD->subord!=NULL)
		{
			for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
			{
				if(!RDAstrcmp(addsub->module,mod) && !RDAstrcmp(addsub->filename,fil))
				{
					++count;
					if(count==occurrence)
					{
						addsub->writable=writable;
						return(0);
					}
				}
			}
		}
		*editable=FALSE;
		prterr("Error Cannot find the File [%s][%s] with Occurence [%4d] in the Power Add Definition.",mod,fil,occurrence);
		return(-1);
	}
	*editable=FALSE;
	prterr("Error Power Add Definition not available.");
	return(-1);
}
short xAPPReturnPowerAddSupportingFileno(PowerAdd *POWERADD,char *mod,char *fil,
	short occurrence,int line,char *file)
{
	short fileno=(-1);
	char *desc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG APPREturnPowerAddSupportingFileno at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fileno=ReturnPowerAddSupportingFileno(POWERADD,mod,fil,occurrence);
	if(fileno==(-1))
	{
		desc=Rmalloc(86+RDAstrlen(mod)+RDAstrlen(fil)+5);
		sprintf(desc,"Cannot find the File [%s][%s] of Occurrence [%4d] in the Power Add Definition.\n",mod,fil,occurrence);
		ERRORDIALOG("CANNOT FIND FILE!",desc,ShutdownOnError,TRUE);
		if(desc!=NULL) Rfree(desc);
		if(!POWERADD->APPmainLoop)
		{
			RDAAPPMAINLOOP();
		}
	}
	return(fileno);
}
void xAllocateEditWdgts(PowerAdd *POWERADD,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG AllocateEditWdgts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
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
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					EditWdgt->data.string_value=NULL;
					EditWdgt->dlen=0;
					break;
				case BOOLNS:
				case CHARACTERS:
					EditWdgt->data.string_value=Rmalloc(1);
					*EditWdgt->data.string_value=0;
					EditWdgt->dlen=1;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					EditWdgt->data.float_value=Rmalloc(sizeof(double));
					*EditWdgt->data.float_value=0;
					EditWdgt->dlen=0;
					break;
				case SHORTV:
				case SSHORTV:
					EditWdgt->data.short_value=Rmalloc(sizeof(short));
					*EditWdgt->data.short_value=0;
					EditWdgt->dlen=0;
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					EditWdgt->data.integer_value=Rmalloc(sizeof(int));
					*EditWdgt->data.integer_value=0;
					EditWdgt->dlen=0;
					break;
				default:
				case BUTTONS:
					break;
			}
		}
	}
}
void xGetAddEditWdgts(RDArsrc *rsrc,PowerAdd *POWERADD,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	int x,len=0;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG GetAddEditWdgts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL && POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			if(EditWdgt->etype!=4)
			{
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
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					FINDRSCGETSTRING(rsrc,EditWdgt->name,&temp);
					if(!isEMPTY(temp))
					{
						len=RDAstrlen(temp)+1;
						QUICKALLOC(EditWdgt->data.string_value,EditWdgt->dlen,len);
						memset(EditWdgt->data.string_value,0,EditWdgt->dlen);
						memcpy(EditWdgt->data.string_value,temp,len);
						if(temp!=NULL) Rfree(temp);
					} else if(EditWdgt->dlen>0)
					{
						memset(EditWdgt->data.string_value,0,EditWdgt->dlen);
					}
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDRSCGETCHAR(rsrc,EditWdgt->name,EditWdgt->data.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					FINDRSCGETDOUBLE(rsrc,EditWdgt->name,EditWdgt->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDRSCGETSHORT(rsrc,EditWdgt->name,EditWdgt->data.short_value);
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					FINDRSCGETINT(rsrc,EditWdgt->name,EditWdgt->data.integer_value);
					break;
				default:
				case BUTTONS:
					break;
			}
			}
		}
	}
}
void xFreeEditWdgts(PowerAdd *POWERADD,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG FreeEditWdgts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		if(POWERADD->EditWdgts!=NULL)
		{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			
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
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
				case BOOLNS:
				case CHARACTERS:
					if(EditWdgt->data.string_value!=NULL)
						Rfree(EditWdgt->data.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(EditWdgt->data.float_value!=NULL)
						Rfree(EditWdgt->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					if(EditWdgt->data.short_value!=NULL)
						Rfree(EditWdgt->data.short_value);
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					if(EditWdgt->data.integer_value!=NULL)
						Rfree(EditWdgt->data.integer_value);
					break;
				default:
				case BUTTONS:
					break;
			}
		}
		}
	}
}
static void READEW(AddEditWdgt *EditWdgt)
{
	char *dashes=NULL,*temp=NULL;

	if(!isEMPTY(EditWdgt->name))
	{
		dashes=adddashes(EditWdgt->name);
		temp=RDA_GetEnv(dashes);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("DIAG ReadingEnvironment EditWdgt->name [%s] [%s] ",EditWdgt->name,(temp!=NULL ? temp:""));
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
	}
#ifndef WIN32
	RDA_SetEnv(dashes,"");
#endif /* Not WIN32 */
#ifdef WIN32
	temp=Rmalloc(RDAstrlen(dashes)+2);
	sprintf(temp,"%s=",(dashes!=NULL ? dashes:""));
	RDA_PutEnv(temp);
	if(temp!=NULL) Rfree(temp);
#endif /* WIN32 */
	if(dashes!=NULL) Rfree(dashes);
}
void xComputeAddEditWdgts(RDArsrc *rsrc,PowerAdd *POWERADD,short timing,char *value,void (*SubFunc)(...),void *args,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	char *temp=NULL,*temp1=NULL;
	int x;
	short xvalue=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG ComputeAddEditWdgts(): module=[%s] screen=[%s], timing=[%d], value=[%s]  at line [%d] program [%s].",
			(rsrc->module!=NULL?rsrc->module:""),
			(rsrc->screen!=NULL?rsrc->screen:""),timing,
			(value!=NULL?value:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL && POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagadd)
			{
				prterr("DIAG DefaultAddEditWdgt [%s] ",(EditWdgt->name!=NULL ? EditWdgt->name:""));
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if((EditWdgt->etype==timing) ||
				(((timing==0) || (timing==1)) && 
				EditWdgt->etype==3) ||
				((timing!=4) && ((EditWdgt->etype==2) || (EditWdgt->etype==5))))
			{
				if(EditWdgt->etype==5)
				{
					READEW(EditWdgt);
				}
				if((!isEMPTY(EditWdgt->expression)) ||
					(EditWdgt->id_mgt==TRUE))
				{
					EditWdgt->wrote_id=FALSE;
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(!isEMPTY(EditWdgt->expression))
							{
								temp1=PP_EVALUATEstr(EditWdgt->expression,SubFunc,args);
							}
							if(EditWdgt->id_mgt==TRUE)
							{
								if(value!=NULL) 
								{
									temp=GETASSIGNNEXTNEWID(POWERADD,EditWdgt,value,TRUE,SubFunc,args);
								} else {
									temp=GETASSIGNNEXTNEWID(POWERADD,EditWdgt,temp1,FALSE,SubFunc,args);
								}
							} else {
								if(temp!=NULL) Rfree(temp);
								temp=stralloc(temp1);
							}

							if(EditWdgt->data.string_value!=NULL) Rfree(EditWdgt->data.string_value);
							EditWdgt->dlen=0;
							if(temp!=NULL)
							{
								EditWdgt->data.string_value=stralloc(temp);
								Rfree(temp);
							}
							if(temp1!=NULL) Rfree(temp1);
							if(EditWdgt->data.string_value!=NULL)
							{
								EditWdgt->dlen=RDAstrlen(EditWdgt->data.string_value)+1;
							}
							if(EditWdgt->dlen>EditWdgt->len &&
								EditWdgt->len>0)
							{
								EditWdgt->data.string_value[EditWdgt->len]=0;
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							*EditWdgt->data.string_value=PP_EVALUATEbol(EditWdgt->expression,SubFunc,args);
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							*EditWdgt->data.float_value=PP_EVALUATEdbl(EditWdgt->expression,SubFunc,args);
							xvalue=(short)*EditWdgt->data.float_value;
							break;
						case SHORTV:
						case SSHORTV:
							*EditWdgt->data.short_value=PP_EVALUATEsht(EditWdgt->expression,SubFunc,args);
							xvalue=*EditWdgt->data.short_value;
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							*EditWdgt->data.integer_value=PP_EVALUATEint(EditWdgt->expression,SubFunc,args);
							xvalue=(short)*EditWdgt->data.integer_value;
							break;
						default:
						case BUTTONS:
							break;
					}
				}
			}
		}
	}
}
void xDefaultAddEditWdgts(RDArsrc *rsrc,PowerAdd *POWERADD,short timing,char *value,void (*SubFunc)(...),void *args,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	char *temp=NULL,*temp1=NULL;
	int x,y;
	short xvalue=0;
	AddEditWdgt *EW2=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG DefaultAddEditWdgts(): module=[%s] screen=[%s], timing=[%d], value=[%s]  at line [%d] program [%s].",
			(rsrc->module!=NULL?rsrc->module:""),
			(rsrc->screen!=NULL?rsrc->screen:""),timing,
			(value!=NULL?value:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL && POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagadd)
			{
				prterr("DIAG DefaultAddEditWdgt [%s] ",(EditWdgt->name!=NULL ? EditWdgt->name:""));
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if((EditWdgt->etype==timing) ||
				(((timing==0) || (timing==1)) && 
				EditWdgt->etype==3) ||
				((timing!=4) && ((EditWdgt->etype==2) || (EditWdgt->etype==5))))
			{
				if(EditWdgt->etype==5)
				{
					READEW(EditWdgt);
				}
				if((!isEMPTY(EditWdgt->expression)) ||
					(EditWdgt->id_mgt==TRUE))
				{
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(!isEMPTY(EditWdgt->expression))
							{
								temp1=PP_EVALUATEstr(EditWdgt->expression,SubFunc,args);
							}
							if(EditWdgt->id_mgt==TRUE)
							{
								if(value!=NULL) 
								{
									temp=GETASSIGNNEXTNEWID(POWERADD,EditWdgt,value,TRUE,SubFunc,args);
								} else {
									temp=GETASSIGNNEXTNEWID(POWERADD,EditWdgt,temp1,FALSE,SubFunc,args);
								}
							} else {
								if(temp!=NULL) Rfree(temp);
								temp=stralloc(temp1);
							}

							if(EditWdgt->data.string_value!=NULL) Rfree(EditWdgt->data.string_value);
							EditWdgt->dlen=0;
							if(temp!=NULL)
							{
								EditWdgt->data.string_value=stralloc(temp);
								Rfree(temp);
							}
							if(temp1!=NULL) Rfree(temp1);
							if(EditWdgt->data.string_value!=NULL)
							{
								EditWdgt->dlen=RDAstrlen(EditWdgt->data.string_value)+1;
							}
							if(EditWdgt->dlen>EditWdgt->len &&
								EditWdgt->len>0)
							{
								EditWdgt->data.string_value[EditWdgt->len]=0;
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							*EditWdgt->data.string_value=PP_EVALUATEbol(EditWdgt->expression,SubFunc,args);
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							*EditWdgt->data.float_value=PP_EVALUATEdbl(EditWdgt->expression,SubFunc,args);
							xvalue=(short)*EditWdgt->data.float_value;
							break;
						case SHORTV:
						case SSHORTV:
							*EditWdgt->data.short_value=PP_EVALUATEsht(EditWdgt->expression,SubFunc,args);
							xvalue=*EditWdgt->data.short_value;
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							*EditWdgt->data.integer_value=PP_EVALUATEint(EditWdgt->expression,SubFunc,args);
							xvalue=(short)*EditWdgt->data.integer_value;
							break;
						default:
						case BUTTONS:
							break;
					}
					if(EditWdgt->setaccttype==TRUE)
					{
						if(EditWdgt->acctrsc!=NULL)
						{
							for(y=0;y<EditWdgt->acctrsc->numlibs;++y)
							{
								FINDRSCSETACCTTYPE(rsrc,EditWdgt->acctrsc->libs[y],xvalue);
							}
						}
					}
					if(EditWdgt->setdeftype==TRUE)
					{
						if(EditWdgt->defrsc!=NULL)
						{
							for(y=0;y<EditWdgt->defrsc->numlibs;++y)
							{
								FINDRSCSETDEFTYPE(rsrc,EditWdgt->defrsc->libs[y],xvalue);
							}
						}
					}
				}
				if(POWERADD->AddType==1 && ((EditWdgt->loadvirtual==2) || (EditWdgt->loadvirtual==3)))
				{
					memset(stemp,0,101);
					sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EditWdgt->name);
					EW2=FINDEDITWDGT(POWERADD,stemp);		
					if(EW2!=NULL)
					{
						CopyEditWdgt2EditWdgt(EditWdgt,EW2);
					}
				}
			}
		}
	}
}
void xSetAddEditWdgts(RDArsrc *rsrc,PowerAdd *POWERADD,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL,*EW2=NULL;
	RDArmem *mem=NULL;
	int x,y,z=0;
	short value=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG SetAddEditWdgts at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL && POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagadd)
			{
				prterr("DIAG SetAddEditWdgts for [%s] ",(EditWdgt->name!=NULL ? EditWdgt->name:""));
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
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
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					FINDRSCSETSTRING(rsrc,EditWdgt->name,EditWdgt->data.string_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDRSCSETCHAR(rsrc,EditWdgt->name,*EditWdgt->data.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					FINDRSCSETDOUBLE(rsrc,EditWdgt->name,*EditWdgt->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDRSCSETSHORT(rsrc,EditWdgt->name,*EditWdgt->data.short_value);
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					FINDRSCSETINT(rsrc,EditWdgt->name,*EditWdgt->data.integer_value);
					break;
				default:
				case BUTTONS:
					break;
			}
			if(EditWdgt->setaccttype==TRUE || EditWdgt->setdeftype==TRUE)
			{
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
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						break;
					case BOOLNS:
					case CHARACTERS:
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						value=(short)*EditWdgt->data.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						value=*EditWdgt->data.short_value;
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						value=(short)*EditWdgt->data.integer_value;
						break;
					default:
					case BUTTONS:
						break;
				}
			}
			if(EditWdgt->setaccttype==TRUE)
			{
				if(EditWdgt->acctrsc!=NULL)
				{
					for(y=0;y<EditWdgt->acctrsc->numlibs;++y)
					{
						FINDRSCSETACCTTYPE(rsrc,EditWdgt->acctrsc->libs[y],value);
					}
				}
			}
			if(EditWdgt->setdeftype==TRUE)
			{
				if(EditWdgt->defrsc!=NULL)
				{
					for(y=0;y<EditWdgt->defrsc->numlibs;++y)
					{
						FINDRSCSETDEFTYPE(rsrc,EditWdgt->defrsc->libs[y],value);
					}
				}
			}
			if(POWERADD->AddType==1 && (EditWdgt->loadvirtual==2 || EditWdgt->loadvirtual==3))
			{
				memset(stemp,0,101);
				sprintf(stemp,"%04d:%s",POWERADD->table_row+1,EditWdgt->name);
				if(POWERADD->TableWidget==NULL)
				{
					z=FINDRSC(rsrc,stemp);
					if(z>(-1))
					{
						mem=rsrc->rscs+z;
						POWERADD->TableWidget=(void *)mem->Table;
					}
				}
				EW2=FINDEDITWDGT(POWERADD,stemp);		
				if(EW2!=NULL)
				{
					CopyEditWdgt2EditWdgt(EditWdgt,EW2);
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
							FINDRSCSETSTRING(rsrc,EW2->name,EW2->data.string_value);
							break;
						case BOOLNS:
						case CHARACTERS:
							FINDRSCSETCHAR(rsrc,EW2->name,*EW2->data.string_value);
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							FINDRSCSETDOUBLE(rsrc,EW2->name,*EW2->data.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							FINDRSCSETSHORT(rsrc,EW2->name,*EW2->data.short_value);
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							FINDRSCSETINT(rsrc,EW2->name,*EW2->data.integer_value);
							break;
						default:
						case BUTTONS:
							break;
					}
				}
			}
		}
	}
}
short GetSupportingFileno(PowerAdd *POWERADD,char *modname,char *filename)
{
	int x; 
	RDAaddsub *addsub=NULL;

	for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
	{
		if((!RDAstrcmp(modname,addsub->module)) && (!RDAstrcmp(filename,addsub->filename)))
		{
			return(addsub->fileno);
		}
	}
	return(-1);
}
void SetPrevMgtId(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,void (*SubFunc)(...),void *args)
{
	int x; 
	RDAeditfld *confld;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing SetPrevMgtId(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	if(EditWdgt->confld!=NULL)
	{
		ZERNRD(EditWdgt->fileno);
		for(x=0,confld=EditWdgt->confld;x<EditWdgt->num;++x,++confld)
		{
			if(confld->type!=(-1))
			{
				if(!isEMPTY(confld->fldname))
				{
					switch(confld->type)
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
							FINDFLDSETSTRING(EditWdgt->fileno,confld->fldname,confld->data.string_value);
							break;
						case BOOLNS:
						case CHARACTERS:
							FINDFLDSETCHAR(EditWdgt->fileno,confld->fldname,*confld->data.string_value);
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							FINDFLDSETDOUBLE(EditWdgt->fileno,confld->fldname,*confld->data.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							FINDFLDSETSHORT(EditWdgt->fileno,confld->fldname,*confld->data.short_value);
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							FINDFLDSETINT(EditWdgt->fileno,confld->fldname,*confld->data.integer_value);
							break;
						default:
						case BUTTONS:
							break;
					}
				} else {
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting SetPrevMgtId(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
short CompareMgtIdKeys(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,void (*SubFunc)(...),void *args)
{
	NRDfield *field=NULL;
	RDAeditfld *confld=NULL;
	int x;
	char break_out=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing CompareMgtIdKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	if(EditWdgt->confld!=NULL)
	{
		for(x=0,confld=EditWdgt->confld;x<EditWdgt->num;++x,++confld)
		{	
			field=FLDNRDsec(EditWdgt->fileno,confld->fldname,TRUE,FALSE,FALSE);
			if(!isEMPTY(confld->conname))
			{
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(!RDAstrcmp(field->data.string_value,confld->data.string_value))
							{
								break_out=FALSE;
							} else {
#ifdef USE_RDA_DIAGNOSTICS
								if(diagadd)
								{
								prterr("CompareMgtIdKeys() EditWdgt->name=[%s] confld->fldname=[%s] : field->name=[%s] (!RDAstrcmp(confld->data=[%s],field->data=[%s]))==FALSE)",
									EditWdgt->name,confld->fldname,
									field->name,confld->data.string_value,
									field->data.string_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break_out=TRUE;
							}
							break;
						case BOOLNS:
							if(*field->data.string_value==*confld->data.string_value)
							{
								break_out=FALSE;
							} else {
#ifdef USE_RDA_DIAGNOSTICS
								if(diagadd)
								{
								prterr("CompareMgtIdKeys() EditWdgt->name=[%s] confld->fldname=[%s] : field->name=[%s] (!RDAstrcmp(confld->data=[%s],field->data=[%s]))==FALSE)",
									EditWdgt->name,confld->fldname,field->name,
									(*confld->data.string_value==TRUE?"TRUE":"FALSE"),
									(*field->data.string_value==TRUE?"TRUE":"FALSE"));
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break_out=TRUE;
							}
							break;
						case CHARACTERS:
                                                        if(*field->data.string_value==*confld->data.string_value)
                                                        {
                                                                break_out=FALSE;
                                                        } else {
#ifdef USE_RDA_DIAGNOSTICS
								if(diagadd)
								{
								prterr("CompareMgtIdKeys() EditWdgt->name=[%s] confld->fldname=[%s] : field->name=[%s] (!RDAstrcmp(confld->data=[%d],field->data=[%d]))==FALSE)",
									EditWdgt->name,confld->fldname,field->name,
									*confld->data.string_value,
									*field->data.string_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
                                                                break_out=TRUE;
                                                        }
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(*field->data.float_value==*confld->data.float_value)
							{
								break_out=FALSE;
							} else {
#ifdef USE_RDA_DIAGNOSTICS
								if(diagadd)
								{
								prterr("CompareMgtIdKeys() EditWdgt->name=[%s] confld->fldname=[%s] : field->name=[%s] (!RDAstrcmp(confld->data=[%f],field->data=[%f]))==FALSE)",
									EditWdgt->name,confld->fldname,field->name,
									*confld->data.float_value,
									*field->data.float_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break_out=TRUE;
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(*field->data.short_value==*confld->data.short_value)
							{
								break_out=FALSE;
							} else {
#ifdef USE_RDA_DIAGNOSTICS
								if(diagadd)
								{
								prterr("CompareMgtIdKeys() EditWdgt->name=[%s] confld->fldname=[%s] : field->name=[%s] (!RDAstrcmp(confld->data=[%d],field->data=[%d]))==FALSE)",
									EditWdgt->name,confld->fldname,field->name,
									*confld->data.short_value,
									*field->data.short_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break_out=TRUE;
							}
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							if(*field->data.integer_value==*confld->data.integer_value)
							{
								break_out=FALSE;
							} else {
#ifdef USE_RDA_DIAGNOSTICS
								if(diagadd)
								{
								prterr("CompareMgtIdKeys() EditWdgt->name=[%s] confld->fldname=[%s] : field->name=[%s] (!RDAstrcmp(confld->data=[%d],field->data=[%d]))==FALSE)",
									EditWdgt->name,confld->fldname,field->name,
									*confld->data.integer_value,
									*field->data.integer_value);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								break_out=TRUE;
							}
							break;
						default:
						case BUTTONS:
							break;
					} 
					if(break_out)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagadd)
						{
							prterr("DIAG Exiting CompareMgtIdKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])==FALSE",
								EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
								(EditWdgt->name!=NULL?EditWdgt->name:""));
							SEENRDRECORD(EditWdgt->fileno);TRACE;
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						return(FALSE);
					}
				}
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diagadd)
				{
					prterr("DIAG Exiting CompareMgtIdKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])==%s",
						EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
						(EditWdgt->name!=NULL?EditWdgt->name:""),(break_out!=TRUE?"TRUE":"FALSE"));
					SEENRDRECORD(EditWdgt->fileno);TRACE;
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				return(!break_out);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting CompareMgtIdKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])==TRUE",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(TRUE);
}
void StoreMgtIdPreKeys(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,void (*SubFunc)(...),void *args)
{
	NRDfield *field=NULL;
	RDAeditfld *confld=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing StoreMgtPreIdPreKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	if(EditWdgt->confld!=NULL)
	{
		for(x=0,confld=EditWdgt->confld;x<EditWdgt->num;++x,++confld)
		{
			field=FLDNRDsec(EditWdgt->fileno,confld->fldname,TRUE,FALSE,FALSE);
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
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=NULL;
							confld->dlen=0;
						}
						if(!isEMPTY(field->data.string_value))
						{
							QUICKALLOC(confld->data.string_value,confld->dlen,field->dlen+1);
							memcpy(confld->data.string_value,field->data.string_value,field->dlen);
							if(confld->dlen>confld->len && confld->len>0)
							{
								confld->data.string_value[confld->len]=0;
							}
						} else if(confld->dlen>0)
						{
							memset(confld->data.string_value,0,confld->dlen);
						} else {
							confld->data.string_value=NULL;
							confld->dlen=0;
						}

						break;
					case BOOLNS:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=Rmalloc(1);
							*confld->data.string_value=0;
							confld->dlen=1;
						}
						*confld->data.string_value=*field->data.string_value;
						break;
					case CHARACTERS:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=Rmalloc(1);
							*confld->data.string_value=0;
							confld->dlen=1;
						}
						*confld->data.string_value=*field->data.string_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.float_value=Rmalloc(sizeof(double));
							*confld->data.float_value=0;
							confld->dlen=0;
						}
						*confld->data.float_value=*field->data.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.short_value=Rmalloc(sizeof(short));
							*confld->data.short_value=0;
							confld->dlen=0;
						}
						*confld->data.short_value=*field->data.short_value;
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.integer_value=Rmalloc(sizeof(int));
							*confld->data.integer_value=0;
							confld->dlen=0;
						}
						*confld->data.integer_value=*field->data.integer_value;
						break;
					default:
					case BUTTONS:
						break;
				}
			} else {
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
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=NULL;
							confld->dlen=0;
						} else {
							if(confld->dlen>0)
							{
								memset(confld->data.string_value,0,confld->dlen);
							} else {
								confld->data.string_value=NULL;
								confld->dlen=0;
							}
						}
						break;
					case BOOLNS:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=Rmalloc(1);
							*confld->data.string_value=0;
							confld->dlen=1;
						} else {
							*confld->data.string_value=0;
						}
						break;
					case CHARACTERS:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=Rmalloc(1);
							*confld->data.string_value=0;
							confld->dlen=1;
						} else {
							*confld->data.string_value=0;
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.float_value=Rmalloc(sizeof(double));
							*confld->data.float_value=0;
							confld->dlen=0;
						} else {
							*confld->data.float_value=0;
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.short_value=Rmalloc(sizeof(short));
							*confld->data.short_value=0;
							confld->dlen=0;
						} else {
							*confld->data.short_value=0;
						}
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.integer_value=Rmalloc(sizeof(int));
							*confld->data.integer_value=0;
							confld->dlen=0;
						} else {
							*confld->data.integer_value=0;
						}
						break;
					default:
					case BUTTONS:
						break;
				}
			}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting StoreMgtIdPreKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
void StoreMgtIdKeys(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,void (*SubFunc)(...),void *args)
{
	NRDfield *field=NULL;
	RDAeditfld *confld=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing StoreMgtIdKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	
	if(EditWdgt->confld!=NULL)
	{
		for(x=0,confld=EditWdgt->confld;x<EditWdgt->num;++x,++confld)
		{
			field=FLDNRDsec(EditWdgt->fileno,confld->fldname,TRUE,FALSE,FALSE);
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
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=NULL;
							confld->dlen=0;
						}
						if(!isEMPTY(field->data.string_value))
						{
							QUICKALLOC(confld->data.string_value,confld->dlen,field->dlen+1);
							memcpy(confld->data.string_value,field->data.string_value,field->dlen);
							if(confld->dlen>confld->len && confld->len>0)
							{
								confld->data.string_value[confld->len]=0;
							}
						} else if(confld->dlen>0)
						{
							memset(confld->data.string_value,0,confld->dlen);
						} else {
							confld->data.string_value=NULL;
							confld->dlen=0;
						}

						break;
					case BOOLNS:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=Rmalloc(1);
							*confld->data.string_value=0;
							confld->dlen=1;
						}
						*confld->data.string_value=*field->data.string_value;
						break;
					case CHARACTERS:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.string_value=Rmalloc(1);
							*confld->data.string_value=0;
							confld->dlen=1;
						}
						*confld->data.string_value=*field->data.string_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.float_value=Rmalloc(sizeof(double));
							*confld->data.float_value=0;
							confld->dlen=0;
						}
						*confld->data.float_value=*field->data.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.short_value=Rmalloc(sizeof(short));
							*confld->data.short_value=0;
							confld->dlen=0;
						}
						*confld->data.short_value=*field->data.short_value;
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						if(confld->type==(-1))
						{
							confld->type=field->type;
							confld->len=field->len;
							confld->data.integer_value=Rmalloc(sizeof(int));
							*confld->data.integer_value=0;
							confld->dlen=0;
						}
						*confld->data.integer_value=*field->data.integer_value;
						break;
					default:
					case BUTTONS:
						break;
				}
			}
		}
	}	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting StoreMgtIdKeys(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
void SetNewMgtIdKeys(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,void (*SubFunc)(...),void *args)
{
	NRDfield *field=NULL,*field1=NULL;
	RDAeditfld *confld=NULL;
	int x=0,y=0;
	AddEditWdgt *EditWdgt1=NULL;
	char *modname=NULL,*filename=NULL,*fieldname=NULL;
	short fileno=(-1);
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing SetNewMgtIdKeys(EditWdgt->fileno=[%d], id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	if(EditWdgt->confld!=NULL)
	{
		ZERNRD(EditWdgt->fileno);
		for(x=0,confld=EditWdgt->confld;x<EditWdgt->num;++x,++confld)
		{
			field=FLDNRDsec(EditWdgt->fileno,confld->fldname,TRUE,FALSE,FALSE);
			if(!isEMPTY(confld->conname))
			{
				if(field!=NULL)
				{
				for(y=0,EditWdgt1=POWERADD->EditWdgts;y<POWERADD->numwdgts;++y,++EditWdgt1)
				{
					temp=Rmalloc(RDAstrlen(EditWdgt1->name)+3);
					sprintf(temp,"[%s]",EditWdgt1->name);
					if(!RDAstrcmp(temp,confld->conname))
					{
						break;
					}
					if(temp!=NULL) Rfree(temp);
				}
				if(y<POWERADD->numwdgts)
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
							if(EditWdgt1->type==SCROLLEDTEXT || EditWdgt1->type==VARIABLETEXT ||
								EditWdgt1->type==DATES || EditWdgt1->type==TIMES ||
								EditWdgt1->type==SOCSECNUM || EditWdgt1->type==PLAINTEXT ||
								EditWdgt1->type==PHONE || EditWdgt1->type==CUSTOMTYPE ||
								EditWdgt1->type==ZIPCODE || EditWdgt1->type==EXPENDITURE ||
								EditWdgt1->type==REVENUE || EditWdgt1->type==BALANCESHEET ||
								EditWdgt1->type==BEGINNINGBALANCE || EditWdgt1->type==OPTIONALFIELDS)
							{
								if(!isEMPTY(EditWdgt1->data.string_value))
								{
									QUICKALLOC(field->data.string_value,field->dlen,EditWdgt1->dlen+1);
									memcpy(field->data.string_value,EditWdgt1->data.string_value,EditWdgt1->dlen);
									if(field->dlen>field->len && field->len>0)
									{
										field->data.string_value[field->len]=0;

									}

								} else if(field->dlen>0)
								{
									memset(field->data.string_value,0,field->dlen);
								} else {
									field->data.string_value=NULL;
									field->dlen=0;
								}

							} else {
								prterr("Error:  Incompatable String Field Types between Managed EditWdgt Connedt Field [%s][%d] and EditWdgt [%s][%d]",field->name,field->type,EditWdgt1->name,EditWdgt1->type);
							}
							break;
						case BOOLNS:
							if(EditWdgt1->type==BOOLNS)
							{
								*field->data.string_value=*EditWdgt1->data.string_value;

							} else {
								prterr("Error:  Incompatable Boolean Field Types between Managed EditWdgt Connedt Field [%s][%d] and EditWdgt [%s][%d]",field->name,field->type,EditWdgt1->name,EditWdgt1->type);
							}
							break;
						case CHARACTERS:
							if(EditWdgt1->type==CHARACTERS)
							{
								*field->data.string_value=*EditWdgt1->data.string_value;
							} else {
								prterr("Error:  Incompatable Character Field Types between Managed EditWdgt Connedt Field [%s][%d] and EditWdgt [%s][%d]",field->name,field->type,EditWdgt1->name,EditWdgt1->type);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(EditWdgt1->type==DOLLARS || EditWdgt1->type==DOLLARS_NOCENTS ||
								EditWdgt1->type==DECIMALV || EditWdgt1->type==DOUBLEV ||
								EditWdgt1->type==SDOUBLEV || EditWdgt1->type==SDECIMALV)
							{
								*field->data.float_value=*EditWdgt1->data.float_value;
							} else {
								prterr("Error:  Incompatable Float Field Types between Managed EditWdgt Connect Field [%s][%d] and EditWdgt [%s][%d]",field->name,field->type,EditWdgt1->name,EditWdgt1->type);
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(EditWdgt1->type==SHORTV || EditWdgt1->type==SSHORTV)
							{
								*field->data.short_value=*EditWdgt1->data.short_value;
							} else {
								prterr("Error:  Incompatable Short Integer Field Types between Managed EditWdgt Connect Field [%s][%d] and EditWdgt [%s][%d]",field->name,field->type,EditWdgt1->name,EditWdgt1->type);
							}
							break;
						case LONGV:
						case SLONGV:
						case SCROLLEDLIST:
							if(EditWdgt1->type==SCROLLEDLIST || EditWdgt1->type==LONGV ||
								EditWdgt1->type==SLONGV)
							{
								*field->data.integer_value=*EditWdgt1->data.integer_value;
							} else if(EditWdgt1->type==SHORTV || EditWdgt1->type==SSHORTV) 
							{
								*field->data.integer_value=(int)*EditWdgt1->data.short_value;
							} else {
								prterr("Error:  Incompatable Integer Field Types between Managed EditWdgt Connect Field [%s][%d] and EditWdgt [%s][%d]",field->name,field->type,EditWdgt1->name,EditWdgt1->type);
							}
							break;
						default:
						case BUTTONS:
							break;
					}
				} else {
					modname=stripmodulename(confld->conname);
					filename=stripfilename(confld->conname);
					fieldname=stripfieldname(confld->conname);
					if(!isEMPTY(fieldname))
					{
						fileno=GetSupportingFileno(POWERADD,modname,filename);
						if(fileno!=(-1))
						{
							field1=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
							if(field1!=NULL)
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
										if(field1->type==SCROLLEDTEXT || field1->type==VARIABLETEXT ||
											field1->type==DATES || field1->type==TIMES ||
											field1->type==SOCSECNUM || field1->type==PLAINTEXT ||
											field1->type==PHONE || field1->type==CUSTOMTYPE ||
											field1->type==ZIPCODE || field1->type==EXPENDITURE ||
											field1->type==REVENUE || field1->type==BALANCESHEET ||
											field1->type==BEGINNINGBALANCE || field1->type==OPTIONALFIELDS)
										{
											if(!isEMPTY(field1->data.string_value))
											{
												QUICKALLOC(field->data.string_value,field->dlen,field1->dlen+1);
												memcpy(field->data.string_value,field1->data.string_value,field1->dlen);
												if(field->dlen>field->len && field->len>0)
												{
													field->data.string_value[field->len]=0;
												}

											} else if(field->dlen>0)
											{
												memset(field->data.string_value,0,field->dlen);
											} else {
												field->data.string_value=NULL;
												field->dlen=0;
											}

										} else {
											prterr("Error:  Incompatable String Field Types between Managed EditWdgt Connect Field [%s][%d] and Supporting File Field [%s][%d]",field->name,field->type,field1->name,field1->type);
										}
										break;
									case BOOLNS:
										if(field1->type==BOOLNS)
										{
											*field->data.string_value=*field1->data.string_value;
										} else {
											prterr("Error:  Incompatable Boolean Field Types between Managed EditWdgt Connect Field [%s][%d] and Supporting File Field [%s][%d]",field->name,field->type,field1->name,field1->type);
										}
										break;
									case CHARACTERS:
										if(field1->type==CHARACTERS)
										{
											*field->data.string_value=*field1->data.string_value;
										} else {
											prterr("Error:  Incompatable Character Field Types between Managed EditWdgt Connect Field [%s][%d] and Supporting File Field [%s][%d]",field->name,field->type,field1->name,field1->type);
										}
										break;
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case SDECIMALV:
										if(field1->type==DOLLARS || field1->type==DOLLARS_NOCENTS ||
											field1->type==DECIMALV || field1->type==DOUBLEV ||
											field1->type==SDOUBLEV || field1->type==SDECIMALV)
										{
											*field->data.float_value=*field1->data.float_value;
										} else {
											prterr("Error:  Incompatable Float Field Types between Managed EditWdgt Connect Field [%s][%d] and Supporting File Field [%s][%d]",field->name,field->type,field1->name,field1->type);
										}
										break;
									case SHORTV:
									case SSHORTV:
										if(field1->type==SHORTV || field1->type==SSHORTV)
										{
											*field->data.short_value=*field1->data.short_value;
										} else {
											prterr("Error:  Incompatable Short Integer Field Types between Managed EditWdgt Connect Field [%s][%d] and Supporting File Field [%s][%d]",field->name,field->type,field1->name,field1->type);
										}
										break;
									case LONGV:
									case SLONGV:
									case SCROLLEDLIST:
										if(field1->type==SCROLLEDLIST || field1->type==LONGV ||
											field1->type==SLONGV)
										{
											*field->data.integer_value=*field1->data.integer_value;
										} else if(field1->type==SHORTV || field1->type==SSHORTV) 
										{
											*field->data.integer_value=(int)*field1->data.short_value;
										} else {
											prterr("Error:  Incompatable Integer Field Types between Managed EditWdgt Connect Field [%s][%d] and Supporting File Field [%s][%d]",field->name,field->type,EditWdgt1->name,EditWdgt1->type);
										}
										break;
									default:
									case BUTTONS:
										break;
								}
							}
						}
					}
					if(modname!=NULL) Rfree(modname);
					if(filename!=NULL) Rfree(filename);
					if(fieldname!=NULL) Rfree(fieldname);
				}
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting SetNewMgtIdKeys(EditWdgt->fileno=[%d], id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
char *GETASSIGNNEXTNEWID(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,char *prevalue,short user_value,void (*SubFunc)(...),void *args)
{
	char deleteflag=FALSE,assigned=FALSE,domgtid=FALSE;
	char *tmp=NULL,*assignedfld=NULL;
	RDATData *prev=NULL;
	short ef=0;
	short compared=TRUE;
	short keyno=(-1);
	double counter=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing GETASSIGNNEXTNEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s], prevalue=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""),
			(prevalue!=NULL?prevalue:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(EditWdgt->override_expression))
	{
		tmp=PP_EVALUATEstr(EditWdgt->override_expression,SubFunc,args);
	}
	if(!isEMPTY(tmp))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("DIAG Exiting GETASSIGNNEXTNEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s], prevalue=[%s])==[%s]  override expression evaluation returned.",
				EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
				(EditWdgt->name!=NULL?EditWdgt->name:""),
				(prevalue!=NULL?prevalue:""),
				(tmp!=NULL?tmp:""));
			SEENRDRECORD(EditWdgt->fileno);TRACE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(tmp);
	}
	
/* dct 05-10-2013 4pm */
	UndoASSIGNID(POWERADD,EditWdgt,SubFunc,args);

	if(!isEMPTY(EditWdgt->mgt_expression))
	{
		domgtid=PP_EVALUATEbol(EditWdgt->mgt_expression,SubFunc,args);
	} else {
		domgtid=TRUE;
	}
	if(domgtid==TRUE)
	{
		if(!isEMPTY(EditWdgt->subkeyname))
		{
			keyno=KEYNUMBER(EditWdgt->fileno,EditWdgt->subkeyname);
		} else {
			keyno=1;
		}
		SetNewMgtIdKeys(POWERADD,EditWdgt,SubFunc,args);
		LOCNRDFILE(EditWdgt->fileno);
		FINDFLDSETSTRING(EditWdgt->fileno,EditWdgt->id,prevalue);
		if(SubFunc!=NULL)
		{
			ef=ADVGTENRDsec(EditWdgt->fileno,keyno,SubFunc,args);
		} else {
			ef=GTENRDsec(EditWdgt->fileno,keyno);
		}
		StoreMgtIdPreKeys(POWERADD,EditWdgt,SubFunc,args);
		counter=0.0;
		while(!ef)
		{
			if(counter==0 || fmod(counter,100.0)==0.0)
			{
				compared=CompareMgtIdKeys(POWERADD,EditWdgt,SubFunc,args);
				if(compared==FALSE)
				{
					break;
				}
				if(tmp!=NULL) Rfree(tmp);
				tmp=NULL;
				FINDFLDGETSTRING(EditWdgt->fileno,EditWdgt->id,&tmp);
				if(!isEMPTY(prevalue))
				{
					if(strncmp(tmp,prevalue,RDAstrlen(prevalue)))
					{
						break;
					}
				}
			}
			FINDFLDGETCHAR(EditWdgt->fileno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				if(!isEMPTY(EditWdgt->assignname))
				{
					assignedfld=stralloc(EditWdgt->assignname);
				} else {
					assignedfld=stralloc("ASSIGNED");
				}
				FINDFLDGETCHAR(EditWdgt->fileno,assignedfld,&assigned);
				if(!assigned)
				{
					if(counter!=0.0 && fmod(counter,100.0)!=0.0)
					{
						compared=CompareMgtIdKeys(POWERADD,EditWdgt,SubFunc,args);
						if(compared==FALSE)
						{
							break;
						}
						FINDFLDGETSTRING(EditWdgt->fileno,EditWdgt->id,&tmp);
						if(!isEMPTY(prevalue))
						{
							if(strncmp(tmp,prevalue,RDAstrlen(prevalue)))
							{
								break;
							}
						}
					}
					prev=RDATDataNEW(EditWdgt->fileno);
					assigned=TRUE;
					FINDFLDSETCHAR(EditWdgt->fileno,assignedfld,assigned);
					if(SubFunc!=NULL)
					{
						ADVWRTTRANSsec(EditWdgt->fileno,0,NULL,prev,SubFunc,args);
					} else {
						WRTTRANSsec(EditWdgt->fileno,0,NULL,prev);
					}
					if(prev!=NULL) FreeRDATData(prev);
					UNLNRDFILE(EditWdgt->fileno);
					StoreMgtIdKeys(POWERADD,EditWdgt,SubFunc,args);
#ifdef USE_RDA_DIAGNOSTICS
					if(diagadd)
					{
						prterr("DIAG Exiting GETASSIGNNEXTNEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s], prevalue=[%s])==[%s]",
							EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
							(EditWdgt->name!=NULL?EditWdgt->name:""),
							(prevalue!=NULL?prevalue:""),
							(tmp!=NULL?tmp:""));
						SEENRDRECORD(EditWdgt->fileno);TRACE;
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					return(tmp);
				}
			}
			if(SubFunc!=NULL)
			{
				ef=ADVNXTNRDsec(EditWdgt->fileno,keyno,SubFunc,args);
			} else {
				ef=NXTNRDsec(EditWdgt->fileno,keyno);
			}
		}
		if(tmp!=NULL) Rfree(tmp);
		if(assignedfld!=NULL) Rfree(assignedfld);
		UNLNRDFILE(EditWdgt->fileno);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("DIAG Exiting GETASSIGNNEXTNEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s], prevalue=[%s])==NULL",
				EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
				(EditWdgt->name!=NULL?EditWdgt->name:""),
				(prevalue!=NULL?prevalue:""));
			SEENRDRECORD(EditWdgt->fileno);TRACE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(NULL);
	} else {
		FINDFLDGETSTRING(EditWdgt->fileno,EditWdgt->id,&tmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("DIAG Exiting GETASSIGNNEXTNEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s], prevalue=[%s])=NULL",
				EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
				(EditWdgt->name!=NULL?EditWdgt->name:""),
				(prevalue!=NULL?prevalue:""));
			SEENRDRECORD(EditWdgt->fileno);TRACE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(NULL);
	}
}
void xASSIGNNEWID(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,char *prevalue,void (*SubFunc)(...),
	void *args,int line,char *file)
{
	RDATData *prev=NULL;
	short ef=FALSE;
	char *assignedfld=NULL;
	short keyno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing  ASSIGNNEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s], prevalue=[%s]) at line [%d] program [%s]",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""),
			(prevalue!=NULL?prevalue:""),line,file);
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	ZERNRD(EditWdgt->fileno);
	FINDFLDSETSTRING(EditWdgt->fileno,EditWdgt->id,prevalue);
	LOCNRDFILE(EditWdgt->fileno);
	if(!isEMPTY(EditWdgt->subkeyname))
	{
		keyno=KEYNUMBER(EditWdgt->fileno,EditWdgt->subkeyname);

	} else {
		keyno=1;
	}
	if(SubFunc!=NULL)
	{
		ef=ADVEQLNRDsec(EditWdgt->fileno,keyno,SubFunc,args);
	} else {
		ef=EQLNRDsec(EditWdgt->fileno,keyno);
	}
	if(!ef)
	{
		prev=RDATDataNEW(EditWdgt->fileno);
		if(!isEMPTY(EditWdgt->assignname))
		{
			assignedfld=stralloc(EditWdgt->assignname);
		} else {
			assignedfld=stralloc("ASSIGNED");
		}
		FINDFLDSETCHAR(EditWdgt->fileno,assignedfld,TRUE);
		if(SubFunc!=NULL)
		{
			ADVWRTTRANSsec(EditWdgt->fileno,0,NULL,prev,SubFunc,args);
		} else {
			WRTTRANSsec(EditWdgt->fileno,0,NULL,prev);
		}
		if(prev!=NULL) FreeRDATData(prev);
	}
	if(assignedfld!=NULL) Rfree(assignedfld);
	UNLNRDFILE(EditWdgt->fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting ASSIGNNEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s], prevalue=[%s]) at line [%d] program [%s]",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""),
			(prevalue!=NULL?prevalue:""),line,file);
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
void UndoASSIGNID(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,void (*SubFunc)(...),void *args)
{
	char deleteflag=FALSE,assigned=FALSE;
	char *assignedfld=NULL;
	char *tmp=NULL;
	RDATData *prev=NULL;
	short ef=FALSE;
	short keyno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing UndoASSIGNID(EditWdgt->fileno=[%d], id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	SetPrevMgtId(POWERADD,EditWdgt,SubFunc,args);
	if(!isEMPTY(tmp))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("DIAG Exiting UndoASSIGNID(EditWdgt->fileno=[%d], id=[%s], EditWdgt->name=[%s]) didn't Undo Assignment because override expression=[%s]",
				EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
				(EditWdgt->name!=NULL?EditWdgt->name:""),
				(tmp!=NULL?tmp:""));
			SEENRDRECORD(EditWdgt->fileno);TRACE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(tmp!=NULL) Rfree(tmp);
		return;
	}
/*
	SetPrevMgtId(POWERADD,EditWdgt,SubFunc,args);
*/
/*
	FINDFLDSETSTRING(EditWdgt->fileno,EditWdgt->id,prevalue);
*/
	LOCNRDFILE(EditWdgt->fileno);
	if(!isEMPTY(EditWdgt->subkeyname))
	{
		keyno=KEYNUMBER(EditWdgt->fileno,EditWdgt->subkeyname);
	} else {
		keyno=1;
	}
	if(SubFunc!=NULL)
	{
		ef=ADVEQLNRDsec(EditWdgt->fileno,keyno,SubFunc,args);
	} else {
		ef=EQLNRDsec(EditWdgt->fileno,keyno);
	}
	if(!ef)
	{
		FINDFLDGETCHAR(EditWdgt->fileno,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			if(!isEMPTY(EditWdgt->assignname))
			{
				assignedfld=stralloc(EditWdgt->assignname);
			} else {
				assignedfld=stralloc("ASSIGNED");
			}
			FINDFLDGETCHAR(EditWdgt->fileno,assignedfld,&assigned);
			if(assigned)
			{
				prev=RDATDataNEW(EditWdgt->fileno);
				FINDFLDSETCHAR(EditWdgt->fileno,assignedfld,FALSE);
				if(SubFunc!=NULL)
				{
					ADVWRTTRANSsec(EditWdgt->fileno,0,NULL,prev,SubFunc,args);
				} else {
					WRTTRANSsec(EditWdgt->fileno,0,NULL,prev);
				}
				if(prev!=NULL) FreeRDATData(prev);
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diagadd)
				{
					prterr("DIAG UndoASSIGNID(): Attempt to Undo Assigment of Field [%s] from Managed Edit Widget Field [%s] failed because ASSIGNED value of FALSE.",
						(EditWdgt->id!=NULL?EditWdgt->id:""),
						(EditWdgt->name!=NULL?EditWdgt->name:""));
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			}
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagadd)
			{
				prterr("DIAG UndoASSIGNID(): Attempt to Undo Assigment of Field [%s] from Managed Edit Widget Field [%s] failed because DELETEFLAG value of TRUE.",
					(EditWdgt->id!=NULL?EditWdgt->id:""),
					(EditWdgt->name!=NULL?EditWdgt->name:""));
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		}
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("Error: Attempt to Remove Assigment of Field [%s] from Managed Edit Widget Field [%s] failed.",
				(EditWdgt->id!=NULL?EditWdgt->id:""),
				(EditWdgt->name!=NULL?EditWdgt->name:""));
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	}
	if(assignedfld!=NULL) Rfree(assignedfld);
	UNLNRDFILE(EditWdgt->fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting UndoASSIGNID(EditWdgt->fileno=[%d], id=[%s], EditWdgt->name=[%s])",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""));
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
void xREMOVENEWID(PowerAdd *POWERADD,AddEditWdgt *EditWdgt,void (*SubFunc)(...),void *args,int line,char *file)
{
	RDATData *prev=NULL;
	char deleteflag=TRUE;
	short ef=FALSE;
	short keyno=(-1);
	char *assignedfld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Initializing REMOVENEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s]) at line [%d] program [%s]",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""),line,file);
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	SetPrevMgtId(POWERADD,EditWdgt,SubFunc,args);
	LOCNRDFILE(EditWdgt->fileno);
	if(!isEMPTY(EditWdgt->subkeyname))
	{
		keyno=KEYNUMBER(EditWdgt->fileno,EditWdgt->subkeyname);
	} else {
		keyno=1;
	}
	if(SubFunc!=NULL)
	{
		ef=ADVEQLNRDsec(EditWdgt->fileno,keyno,SubFunc,args);
	} else {
		ef=EQLNRDsec(EditWdgt->fileno,keyno);
	}
	if(!ef)
	{
		if(!isEMPTY(EditWdgt->assignname))
		{
			assignedfld=stralloc(EditWdgt->assignname);
		} else {
			assignedfld=stralloc("ASSIGNED");
		}
		FINDFLDGETCHAR(EditWdgt->fileno,assignedfld,&deleteflag);
		if(deleteflag) 
		{
			deleteflag=TRUE;
			prev=RDATDataNEW(EditWdgt->fileno);
			FINDFLDSETCHAR(EditWdgt->fileno,"DELETEFLAG",deleteflag);
			if(SubFunc!=NULL)
			{
				ADVWRTTRANSsec(EditWdgt->fileno,0,NULL,prev,SubFunc,args);
			} else {
				WRTTRANSsec(EditWdgt->fileno,0,NULL,prev);
			}
			if(prev!=NULL) FreeRDATData(prev);
		}
		if(assignedfld!=NULL) Rfree(assignedfld);
	} else {
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
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				if(!isEMPTY(EditWdgt->data.string_value))
				{
					prterr("Error: REMOVENEWID(): Attempt to Remove Assigment of Field [%s] from Managed Edit Widget Field [%s] failed.",(EditWdgt->id!=NULL?EditWdgt->id:""),(EditWdgt->name!=NULL?EditWdgt->name:""));
				}
				break;
			case BOOLNS:
			case CHARACTERS:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
			case SHORTV:
			case SSHORTV:
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
			default:
			case BUTTONS:
				prterr("Error: REMOVENEWID(): Attempt to Remove Assigment of Field [%s] from Managed Edit Widget Field [%s] failed.",
					(EditWdgt->id!=NULL?EditWdgt->id:""),
					(EditWdgt->name!=NULL?EditWdgt->name:""));
				break;
		}
	}
	UNLNRDFILE(EditWdgt->fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG Exiting REMOVENEWID(EditWdgt->fileno=[%d], EditWdgt->id=[%s], EditWdgt->name=[%s]) Removed Id at line [%d] program [%s]",
			EditWdgt->fileno,(EditWdgt->id!=NULL?EditWdgt->id:""),
			(EditWdgt->name!=NULL?EditWdgt->name:""),line,file);
		SEENRDRECORD(EditWdgt->fileno);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
