#include<mix.hpp>

void quitoptscrn(RDArsrc *r,RDArsrc *parent)
{
	int selected=0,x=0,y=0;
	RDArmem *mem=NULL;
	RDAScrolledList *list=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG quitoptscrn Quit Optional Screen [%s] [%s] ",r->module,r->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(parent!=NULL)
	{
		for(x=0,mem=r->rscs;x<r->numrscs;++x,++mem)
		{
			if(mem->field_type==SCROLLEDLIST)
			{
				FINDRSCGETINT(r,mem->rscrname,&selected);
				if((y=FINDSCROLLLIST(parent,mem->rscrname))==(-1))
				{
#ifdef XXX /* Done by DCT 7-10-96 */
					FINDRSCSETLIST(parent,mem->rscrname,
						selected,mem->items,mem->list);
#endif
				} else {
/* 12-3-97 DCT: There are going to be problems here with the Computed List */
					list=parent->lists+y;
					FINDRSCLISTAPPlib(parent,list->name,
						selected,list->list);
					updatersrc(parent,mem->rscrname);
				}
			}
		}
	}
	if(r!=NULL) free_rsrc(r);
	r=NULL;
}
void saveoptscrn(RDArsrc *r,RDArsrc *parent)
{
	int x,y=0;
	RDArmem *mem=NULL,*mem1=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG saveoptscrn Saving Optional Screen [%s] [%s] back to it's parent [%s] [%s] ",r->module,r->screen,parent->module,parent->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	readallwidgets(r);
	ReadRDAScrolledLists(r);
	for(x=0,mem=r->rscs;x<r->numrscs;++x,++mem)
	{
		y=FINDRSC(parent,mem->rscrname);
		if(y!=(-1))
		{
			mem1=parent->rscs+y;
			if((mem->field_type!=BUTTONS) ||
				(RDAstrcmp(mem->rscrname,"SAVE") 
					&& RDAstrcmp(mem->rscrname,"QUIT")
					&& RDAstrcmp(mem->rscrname,"HELP"))) 
			{
				mem1->type=mem->type;
				mem1->field_type=mem->field_type;
				mem1->field_length=mem->field_length;
				mem1->editable=mem->editable;
				mem1->function=mem->function;
				mem1->function_type=mem->function_type;
				mem1->items=mem->items;
				mem1->rtype=mem->rtype;
				mem1->arglist=mem->arglist;
				mem1->helpfunction=mem->helpfunction;
				mem1->sensitive=mem->sensitive;
				mem1->list=mem->list;
				mem1->cols=mem->cols;
				mem1->callbacktype=mem->callbacktype;
				mem1->user_editable=mem->user_editable;
				mem1->user_sensitive=mem->user_sensitive;
				mem1->table_row=mem->table_row;
				mem1->table_row_span=mem->table_row_span;
				mem1->table_col=mem->table_col;
				mem1->table_col_span=mem->table_col_span;
				mem1->table_function=mem->table_function;
				mem1->table_arglist=mem->table_arglist;
				mem1->table_function_type=mem->table_function_type;
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
						if(!isEMPTY(mem->value.string_value))
						{
							QUICKALLOC(mem1->value.string_value,mem1->dlen,mem->dlen);
							memcpy(mem1->value.string_value,mem->value.string_value,mem->dlen);
						} else if(mem1->dlen>0)
						{
							memset(mem1->value.string_value,0,mem1->dlen);
						} else {
							mem1->value.string_value=NULL;
							mem1->dlen=0;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						*mem1->value.string_value=*mem->value.string_value;
						mem1->dlen=1;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case SDECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
						*mem1->value.float_value=*mem->value.float_value;
						mem1->dlen=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						*mem1->value.short_value=*mem->value.short_value;
						mem1->dlen=sizeof(short);
						break;
					case SCROLLEDLIST:
						mem1->list=mem->list;
						*mem1->value.integer_value=*mem->value.integer_value;
						mem1->dlen=sizeof(int);
						break;
					case LONGV:
					case SLONGV:
						*mem1->value.integer_value=*mem->value.integer_value;
						mem1->dlen=sizeof(int);
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error field type [%d] incorrect for resource [%s] Screen [%s] [%s].",mem->field_type,mem->rscrname,r->module,r->screen);
						break;
				}
			}
		}
	}
	ADVcomputeandupdateallSCRNvirtuals(parent,parent->SubFunc,parent->SubFuncArgs,FALSE);
	updateallrsrc(parent);
	for(x=0,mem=parent->rscs;x<parent->numrscs;++x,++mem)
	{
		if(mem->sensitive==FALSE)
		{
			MEMBERSETSENSITIVE(mem,FALSE);	
		} else {
			if(mem->user_sensitive==FALSE)
			{
				ADVMEMBERSETSENSITIVE(mem,FALSE,1);	
			} else {
				ADVMEMBERSETSENSITIVE(mem,TRUE,1);	
			}
		}
		if(mem->editable==FALSE)
		{
			MEMBERSETEDITABLE(mem,FALSE);
		} else {
			if(mem->user_editable==FALSE)
			{
				ADVMEMBERSETEDITABLE(mem,FALSE,1);
			} else {
				ADVMEMBERSETEDITABLE(mem,TRUE,1);
			}
		}
	}
	quitoptscrn(r,parent);
}
void OPTIONAL_SCREEN_FUNC(RDArsrc *parent,RDArmem *member)
{
	RDArsrc *tmprsrc=NULL;
	int x,lf=0;
	RDArmem *mem,*mem1;
	DFincvir *i1,*i2;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG OPTIONAL_SCREEN_FUNC Creating Optional Screen [%s] [%s] from parent Screen [%s] [%s].",parent->module,member->rscrname,parent->module,parent->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(member==NULL)
	{
		prterr("ERROR Optional Screen button member doesn't exist.");
		return;
	} else if(member->rscrname==NULL)
	{
		prterr("ERROR Optional Screen has no name.  Optional Screen widget name doesn't not exist.");
		return;
	}
	readallwidgets(parent);
	tmprsrc=RDArsrcNEW(parent->module,member->rscrname);
	tmprsrc->SubFunc=parent->SubFunc;
	tmprsrc->SubFuncArgs=parent->SubFuncArgs;
	tmprsrc->EvalStr=parent->EvalStr;
	tmprsrc->EvalStrArgs=parent->EvalStrArgs;
	tmprsrc->EvalFunc=parent->EvalFunc;
	tmprsrc->EvalFuncArgs=parent->EvalFuncArgs;
	if(parent->incvir!=NULL)
	{
		tmprsrc->incvir=Rmalloc(parent->numvir*sizeof(DFincvir));
		for(x=0,i1=parent->incvir,i2=tmprsrc->incvir;x<parent->numvir;++x,
			++i1,++i2)
		{
			if(i1->module!=NULL) i2->module=stralloc(i1->module);
				else i2->module=NULL;
			if(i1->file!=NULL) i2->file=stralloc(i1->file);
				else i2->file=NULL;
			i2->fileno=i1->fileno;
			if(i1->keyname!=NULL) i2->keyname=stralloc(i1->keyname);
				else i2->keyname=NULL;
			i2->v=NULL;
		}
		tmprsrc->numvir=parent->numvir;
	}
	tmprsrc->virflds=NULL;
	tmprsrc->num=0;
	tmprsrc->numlists=0;
	tmprsrc->lists=NULL;
	GET_SCREEN_VIRTUALS(tmprsrc,0);
	for(x=0,mem=parent->rscs;x<parent->numrscs;++x,++mem)
	{
		if(mem->field_type==SCROLLEDLIST)
		{
			lf=FINDSCROLLLIST(parent,mem->rscrname);
		} else lf=FINDVIRTUAL(parent,mem->rscrname);
		if((lf==(-1)) && (mem->field_type!=BUTTONS || 
			(mem->field_type==BUTTONS  
			&& RDAstrcmp(mem->rscrname,"SAVE") 
			&& RDAstrcmp(mem->rscrname,"QUIT")
			&& RDAstrcmp(mem->rscrname,"HELP")))) 
		{
			if(tmprsrc->rscs!=NULL)
			{
				tmprsrc->rscs=Rrealloc(tmprsrc->rscs,
					(tmprsrc->numrscs+1)*sizeof(RDArmem));
			} else {
				tmprsrc->rscs=Rmalloc(sizeof(RDArmem));
				tmprsrc->numrscs=0;
			}
			mem1=tmprsrc->rscs+tmprsrc->numrscs;
			mem1->type=mem->type;
			if(mem->rscrname!=NULL) mem1->rscrname=stralloc(mem->rscrname);
				else mem1->rscrname=NULL;
			mem1->editable_expression=NULL;
			mem1->sensitive_expression=NULL;
			mem1->transversal_expression=NULL;
			mem1->required_expression=NULL;
			mem1->definition=NULL;
			mem1->label=NULL;
			mem1->XHTML_Label=NULL;
			mem1->user_editable=mem->user_editable;
			mem1->user_sensitive=mem->user_sensitive;
			mem1->field_type=mem->field_type;
			mem1->field_length=mem->field_length;
			mem1->editable=mem->editable;
			mem1->w=NULL;
			mem1->popup_w=NULL;
			mem1->toolbar_w=NULL;
			mem1->function=mem->function;
			mem1->function_type=mem->function_type;
			mem1->items=mem->items;
			mem1->rtype=mem->rtype;
			mem1->list=mem->list;
			mem1->arglist=mem->arglist;
			mem1->parent=tmprsrc;
			mem1->helpfunction=mem->helpfunction;
			mem1->sensitive=mem->sensitive;
			mem1->app_update=mem->app_update;
			mem1->cols=mem->cols;
			mem1->callbacktype=mem->callbacktype;
			mem1->Table=NULL;
			mem1->wSIM=NULL;
			mem1->tCell=NULL;
			mem1->popup_id=(-1);
			mem1->menuitem=NULL;
			mem1->table_row=0;
			mem1->table_row_span=0;
			mem1->table_col=0;
			mem1->table_col_span=0;
			mem1->table_function=mem->table_function;
			mem1->pfd=NULL;
			mem1->validobject=NULL;
			mem1->table_arglist=mem->table_arglist;
			mem1->table_function_type=mem->table_function_type;
			switch(mem->field_type)
			{
				case SCROLLEDLIST:
					mem1->list=mem->list;
					mem1->value.integer_value=Rmalloc(sizeof(int));
					mem1->dlen=sizeof(int);
					if(mem->value.integer_value!=NULL)
					{
						*mem1->value.integer_value=*mem->value.integer_value;
					} else *mem1->value.integer_value=0;
					break;
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
					if(!isEMPTY(mem->value.string_value))
					{
						mem1->value.string_value=stralloc(mem->value.string_value);
						mem1->dlen=RDAstrlen(mem1->value.string_value)+1;
					} else {
						mem1->value.string_value=NULL;
						mem1->dlen=0;
					}
					break;
				case DECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					mem1->dlen=sizeof(double);
					mem1->value.float_value=Rmalloc(sizeof(double));
					if(mem->value.float_value!=NULL)
					{
						*mem1->value.float_value=*mem->value.float_value;
					} else *mem1->value.float_value=0;
					break;
				case SHORTV:
				case SSHORTV:
					mem1->dlen=sizeof(short);
					mem1->value.short_value=Rmalloc(sizeof(short));
					if(mem->value.short_value!=NULL)
					{
						*mem1->value.short_value=*mem->value.short_value;
					} else *mem1->value.short_value=0;
					break;
				case LONGV:
				case SLONGV:
					mem1->dlen=sizeof(int);
					mem1->value.integer_value=Rmalloc(sizeof(int));
					if(mem->value.integer_value!=NULL)
					{
						*mem1->value.integer_value=*mem->value.integer_value;
					} else *mem1->value.integer_value=0;
					break;
				case CHARACTERS:
					mem1->dlen=1;
					mem1->value.string_value=Rmalloc(1);	
					*mem1->value.string_value=*mem->value.string_value;
					break;
				case BOOLNS:
					mem1->dlen=1;
					mem1->value.string_value=Rmalloc(1);	
					if(*mem->value.string_value) *mem1->value.string_value=TRUE;
						else *mem1->value.string_value=FALSE;
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error field type [%d] incorrect for resource [%s] on Screen [%s] [%s].",mem->field_type,mem->rscrname,parent->module,parent->screen);
					break;
			}
			++tmprsrc->numrscs;
		}
	}
/*
	if(tmprsrc->incvir!=NULL)
	{
*/
		SCRNvirtual2rsrc(tmprsrc);
		SetVirFieldFuncs(tmprsrc);
/*
	}
*/
	computeandupdateallSCRNvirtuals(tmprsrc,FALSE);
	addrfkwrsrc(tmprsrc,"SAVE",TRUE,saveoptscrn,parent);
	addrfkwrsrc(tmprsrc,"QUIT",TRUE,quitoptscrn,parent);
	addbtnrsrc(tmprsrc,"HELP",TRUE,screenhelp,NULL);
/*
	if(ADVmakescrn(tmprsrc,TRUE))
*/
	if(xmakescrn(tmprsrc,TRUE,tmprsrc->EvalFunc,tmprsrc->EvalFuncArgs,addoswidgets,tmprsrc->EvalStr,tmprsrc->EvalStrArgs,tmprsrc->SubFunc,tmprsrc->SubFuncArgs,parent,__LINE__,__FILE__))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		prterr("Can't make screen %s.",tmprsrc->screen);
		quitoptscrn(tmprsrc,parent);
	}
}
void addoswidgets(RDArsrc *r)
{
	RDArmem *member;
	RDAwdgt *w;
	int x,y;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diagmix)
	{
		prterr("DIAG addoswidgets Creating Screen Resources for Optional Screen Buttons.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r==NULL)
	{
		prterr("Error Resource Structure passed to addoswidgets is NULL.");
		return;
	}
	for(x=0,w=r->scn->wdgts;x<r->scn->numwdgts;++x,++w)
	{
		if(w->type==14)
			addbtnrsrc(r,w->resource_name,TRUE,OPTIONAL_SCREEN_FUNC,NULL);
	}
	for(x=0,w=r->scn->wdgts;x<r->scn->numwdgts;++x,++w)
	{
		if(w->type==14)
		{
			y=FINDRSC(r,w->resource_name);
			if(y!=(-1))
			{
				member=r->rscs+y;
				member->arglist=member;
			}
		}
	}
}
