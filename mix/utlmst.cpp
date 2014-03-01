/* utlmst.c - Utility Master Structures */
#include<mix.hpp>

RDAScrolledList *xRDAScrolledListNew(char *module,char *name,int line,
	char *file)
{
	RDAScrolledList *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDAScrolledListNew for Module [%s] ScrolledList [%s] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAScrolledList));
	if(!isEMPTY(module)) tmp->module=stralloc(module);
		else tmp->module=NULL;
	if(!isEMPTY(name)) tmp->name=stralloc(name);
		else tmp->name=NULL;
	tmp->vtype=0;
	tmp->numincvirs=0;
	tmp->incvir=NULL;
	tmp->num=0;
	tmp->initfld=NULL;
	tmp->type=0;
	tmp->select_formula=NULL;
	tmp->special_function=NULL;
	tmp->ffield=NULL;
	tmp->ffinfo=NULL;

/* SJS added for core in dflist.EXT */
	tmp->ffinfo=Rmalloc(sizeof(DFincvir));
	tmp->ffinfo->module=NULL;
	tmp->ffinfo->file=NULL;
	tmp->ffinfo->keyname=NULL;
	tmp->ffinfo->fileno=(-1);
	
	tmp->occurrence=0;
	tmp->contype=0;
	tmp->conmod=NULL;
	tmp->confil=NULL;
	tmp->confld=NULL;
	tmp->con_occurrence=0;
	tmp->list=NULL;
	tmp->dlist=NULL;
	tmp->format_formula=NULL;
	tmp->unformat_formula=NULL;
	tmp->desc=NULL;
	tmp->timing=0;
	tmp->reformat_formula=NULL;
	tmp->SubFunc=NULL;
	tmp->args=NULL;
	return(tmp);
}
short xgetScrolledListbin(char *libname,RDAScrolledList *list,
	int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	int y,z;
	char *temp=NULL;
	RDAconfld *c=NULL;
	DFincvir *i=NULL;
	short version=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getScrolledListbin Attempting to Read ScrolledList Binary [%s] from ScrolledList Library [%s] at line [%d] program [%s].",list->name,libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,list->name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("Error Attempting to Read ScrolledList Binary [%s] from ScrolledList Library [%s] Failed at line [%d] program [%s].",list->name,libname,line,file); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1700) && x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for ScrolledList [%s] in ScrolledList Binary Library [%s] at line [%d] program [%s].",list->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1700)) version=2;
	else if(x==(-1701)) version=1;
	else version=0;
	test=BINgetshort(bin);
	if(test) list->module=BINgetstring(bin);
		else list->module=NULL;
	test=BINgetshort(bin);
	if(test) list->name=BINgetstring(bin);
		else list->name=NULL;
	test=BINgetshort(bin);
	if(test) list->desc=BINgetstring(bin);
		else list->desc=NULL;
	list->vtype=BINgetint(bin);
	list->numincvirs=BINgetint(bin);
	if(list->numincvirs>0)
	{
		list->incvir=Rmalloc(list->numincvirs*sizeof(DFincvir));
		for(z=0,i=list->incvir;z<list->numincvirs;++z,++i)
		{
			test=BINgetshort(bin);
			if(test) i->module=BINgetstring(bin);
				else i->module=NULL;
			test=BINgetshort(bin);
			if(test) i->file=BINgetstring(bin);
				else i->file=NULL;
			test=BINgetshort(bin);
			if(test) i->keyname=BINgetstring(bin);
				else i->keyname=NULL;
		}
	} else {
		BINaddint(bin,0);
	}
	list->type=BINgetint(bin);
	test=BINgetshort(bin);
	if(test) list->select_formula=BINgetstring(bin);	
		else list->select_formula=NULL;
	test=BINgetshort(bin);
	if(test) list->special_function=BINgetstring(bin);	
		else list->special_function=NULL;
	test=BINgetshort(bin);
	if(test) list->format_formula=BINgetstring(bin);
		else list->format_formula=NULL;
	test=BINgetshort(bin);
	if(test) list->unformat_formula=BINgetstring(bin);
		else list->unformat_formula=NULL;
	y=BINgetint(bin);
	if(y>0)
	{
		list->list=APPlibNEW();
		for(z=0;z<y;++z)
		{
			test=BINgetshort(bin);
			if(test) temp=BINgetstring(bin);
				else temp=NULL;
			addAPPlib(list->list,temp);
			if(temp!=NULL) Rfree(temp);
		}
	} else list->list=NULL;
	list->dlist=NULL;
	list->contype=BINgetint(bin);
	test=BINgetshort(bin);
	if(test) list->conmod=BINgetstring(bin);
		else list->conmod=NULL;
	test=BINgetshort(bin);
	if(test) list->confil=BINgetstring(bin);
		else list->confil=NULL;
	test=BINgetshort(bin);
	if(test) list->confld=BINgetstring(bin);
		else list->confld=NULL;
	list->con_occurrence=BINgetshort(bin);
	list->num=BINgetint(bin);
	if(list->num>0)
	{
		list->initfld=Rmalloc(list->num*sizeof(RDAconfld));
		for(z=0,c=list->initfld;z<list->num;++z,++c)
		{
			test=BINgetshort(bin);
			if(test) c->fldname=BINgetstring(bin);
				else c->fldname=NULL;
			test=BINgetshort(bin);
			if(test) c->conname=BINgetstring(bin);
				else c->conname=NULL;
		}
	} else list->initfld=NULL;
	test=BINgetshort(bin);
	if(test) list->ffield=BINgetstring(bin);
		else list->ffield=NULL;
	list->ffinfo=Rmalloc(sizeof(DFincvir));
	test=BINgetshort(bin);
	if(test) list->ffinfo->module=BINgetstring(bin);
		else list->ffinfo->module=NULL;
	test=BINgetshort(bin);
	if(test) list->ffinfo->file=BINgetstring(bin);
		else list->ffinfo->file=NULL;
	test=BINgetshort(bin);
	if(test) list->ffinfo->keyname=BINgetstring(bin);
		else list->ffinfo->keyname=NULL;
	list->ffinfo->fileno=(-1);
	list->occurrence=BINgetshort(bin);
	list->timing=BINgetint(bin);
	if(version>1)
	{
		test=BINgetshort(bin);
		if(test) list->reformat_formula=BINgetstring(bin);
			else list->reformat_formula=NULL;
	}
	BINfree(bin);
	return(0);
}
short xwriteScrolledListbin(char *libname,RDAScrolledList *list,int line,
	char *file)
{
	BIN *bin;
	int x;
	RDAconfld *c;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG writeScrolledListbin Writing ScrolledList [%s] into ScrolledList Binary Library [%s] at line [%d] program [%s].",(list->name!=NULL ? list->name:""),libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1700);	/*type identifier*/
	if(list->module!=NULL) 
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->module);
	} else BINaddshort(bin,FALSE);
	if(list->name!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->name);
	} else BINaddshort(bin,FALSE);
	if(list->desc!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->desc);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,list->vtype);
	if(list->incvir!=NULL && list->numincvirs>0)
	{
		BINaddint(bin,list->numincvirs);
		for(x=0,i=list->incvir;x<list->numincvirs;++x,++i)
		{
			if(!isEMPTY(i->module))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->module);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(i->file))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->file);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(i->keyname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->keyname);
			} else BINaddshort(bin,FALSE);
		}
	} else {
		BINaddint(bin,0);
	}
	BINaddint(bin,list->type);
	if(!isEMPTY(list->select_formula))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->select_formula);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(list->special_function))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->special_function);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(list->format_formula))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->format_formula);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(list->unformat_formula))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->unformat_formula);
	} else BINaddshort(bin,FALSE);
	if(list->list!=NULL)
	{
		BINaddint(bin,list->list->numlibs);
		if(list->list->numlibs>0)
		{
			for(x=0;x<list->list->numlibs;++x)
			{
				if(!isEMPTY(list->list->libs[x]))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,list->list->libs[x]);
				} else BINaddshort(bin,FALSE);
			}
		}
	} else BINaddint(bin,0);
	BINaddint(bin,list->contype);
	if(!isEMPTY(list->conmod))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->conmod);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(list->confil))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->confil);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(list->confld))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->confld);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,list->con_occurrence);
	if(list->initfld!=NULL && list->num>0)
	{
		BINaddint(bin,list->num);
		for(x=0,c=list->initfld;x<list->num;++x,++c)
		{
			if(!isEMPTY(c->fldname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,c->fldname);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(c->conname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,c->conname);
			} else BINaddshort(bin,FALSE);
		}
	} else BINaddint(bin,0);
	if(!isEMPTY(list->ffield))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->ffield);
	} else BINaddshort(bin,FALSE);
	if(list->ffinfo!=NULL)
	{
		if(!isEMPTY(list->ffinfo->module))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,list->ffinfo->module);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(list->ffinfo->file))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,list->ffinfo->file);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(list->ffinfo->keyname))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,list->ffinfo->keyname);
		} else BINaddshort(bin,FALSE);
	} else {
		BINaddshort(bin,FALSE);
		BINaddshort(bin,FALSE);
		BINaddshort(bin,FALSE);
	}
	BINaddshort(bin,list->occurrence);
	BINaddint(bin,list->timing);
	if(!isEMPTY(list->reformat_formula))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,list->reformat_formula);
	} else BINaddshort(bin,FALSE);
	if(writelibbin(libname,bin,list->name))
	{
		prterr("Error Can't write ScrolledList binary [%s] into ScrolledList library [%s] at line [%d] program [%s].",(list->name!=NULL ? list->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
char *xgetScrolledListdesc(char *libname,char *name,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getScrolledListdesc Attempting to Read ScrolledList description  for [%s] from ScrolledList Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("Error Attempting to Read ScrolledList Binary [%s] from ScrolledList Library [%s] Failed at line [%d] program [%s].",name,libname,line,file); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(NULL);
	}
	x=BINgetshort(bin);
	if(x!=(-1700) && x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for ScrolledList [%s] in ScrolledList Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
		BINfree(bin);
		return(NULL);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
	} else temp=NULL;
	BINfree(bin);
	return(temp);
}
MaintainMaster *xMaintainMasterNew(char *module,char *name,int line,char *file)
{
	MaintainMaster *MTNMASTER=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG MaintainMasterNew at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	MTNMASTER=Rmalloc(sizeof(MaintainMaster));
	if(module!=NULL)
	{
		MTNMASTER->module=stralloc(module);
	} else { 
		MTNMASTER->module=NULL;
	}
	if(name!=NULL)
	{
		MTNMASTER->name=stralloc(name);
	} else { 
		MTNMASTER->name=NULL;
	}
	MTNMASTER->style=0;
	MTNMASTER->mainfile=NULL;
	MTNMASTER->browse_screen=NULL;
	MTNMASTER->define_list=NULL;
	MTNMASTER->search_browse=NULL;
	MTNMASTER->maintain_screen=NULL;
	MTNMASTER->file_type=0;
	MTNMASTER->start_with=0;
	MTNMASTER->start_where=0;
	MTNMASTER->num=0;
	MTNMASTER->s=NULL;
	MTNMASTER->subnum=0;
	MTNMASTER->subord=NULL;
	MTNMASTER->passkey=NULL;
	MTNMASTER->args=NULL;
	MTNMASTER->desc=NULL;
	MTNMASTER->numlists=0;
	MTNMASTER->lists=NULL;
	MTNMASTER->numbuttons=0;
	MTNMASTER->buttons=NULL;
	MTNMASTER->runfunc=NULL;
	MTNMASTER->save_expression=NULL;
	MTNMASTER->save_warning=NULL;
	MTNMASTER->save_error_desc=NULL;
	MTNMASTER->save_warning_desc=NULL;
	MTNMASTER->UsePowerWrite=FALSE;
	MTNMASTER->numPrevWrites=0;
	MTNMASTER->PrevWrites=NULL;
	MTNMASTER->numNewWrites=0;
	MTNMASTER->NewWrites=NULL;
	MTNMASTER->Use_Audit_Trail=FALSE;
	MTNMASTER->Audit_Trail=NULL;
	MTNMASTER->numActions=0;
	MTNMASTER->Actions=NULL;
	MTNMASTER->passparent=NULL;
	MTNMASTER->level=0;
	MTNMASTER->APPmainLoop=FALSE;
	return(MTNMASTER);
}
void xFreeMaintainMaster(MaintainMaster *MTNMASTER,int line,char *file)
{
	RDAsearch *sup;
	RDAsubord *sub;
	MaintainList *l;
	int x,y,z;
	RDAconfld *confld;
	MaintainButton *button;
	ButtonField *bf;
	PowerWrite *W=NULL;
	PowerWriteField *F=NULL;
	MTNAction *action=NULL;
	Actionstruct *actstruct=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG FreeMaintainMaster at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(MTNMASTER!=NULL)
	{
		if(MTNMASTER->module!=NULL) Rfree(MTNMASTER->module);
		if(MTNMASTER->mainfile!=NULL) Rfree(MTNMASTER->mainfile);
		if(MTNMASTER->name!=NULL) Rfree(MTNMASTER->name);
		if(MTNMASTER->desc!=NULL) Rfree(MTNMASTER->desc);
		if(MTNMASTER->browse_screen!=NULL) Rfree(MTNMASTER->browse_screen);
		if(MTNMASTER->define_list!=NULL) Rfree(MTNMASTER->define_list);
		if(MTNMASTER->search_browse!=NULL) Rfree(MTNMASTER->search_browse);
		if(MTNMASTER->maintain_screen!=NULL) Rfree(MTNMASTER->maintain_screen);
		if(MTNMASTER->passkey!=NULL) FreePassKey(MTNMASTER->passkey);
		if(MTNMASTER->runfunc!=NULL) freeapplib(MTNMASTER->runfunc);
		if(MTNMASTER->save_expression!=NULL) Rfree(MTNMASTER->save_expression);
		if(MTNMASTER->save_warning!=NULL) Rfree(MTNMASTER->save_warning);
		if(MTNMASTER->save_error_desc!=NULL) Rfree(MTNMASTER->save_error_desc);
		if(MTNMASTER->save_warning_desc!=NULL) Rfree(MTNMASTER->save_warning_desc);
		if(MTNMASTER->Audit_Trail!=NULL) Rfree(MTNMASTER->Audit_Trail);
		if(MTNMASTER->Actions!=NULL)
		{
			for(x=0,action=MTNMASTER->Actions;x<MTNMASTER->numActions;++x,++action)
			{
				if(action->name!=NULL) Rfree(action->name);
				if(action->a!=NULL)
				{
					for(y=0,actstruct=action->a;y<action->num;++y,++actstruct)
					{
						if(actstruct->to_name!=NULL) Rfree(actstruct->to_name);
						if(actstruct->expression!=NULL) 
							Rfree(actstruct->expression);
					}
					Rfree(action->a);
				}
			}
			Rfree(MTNMASTER->Actions);
			MTNMASTER->numActions=0;
		}
		if(MTNMASTER->PrevWrites!=NULL)
		{
			for(x=0,W=MTNMASTER->PrevWrites;x<MTNMASTER->numPrevWrites;++x,++W)
			{
				if(W->module!=NULL) Rfree(W->module);
				if(W->filename!=NULL) Rfree(W->filename);
				if(W->expression!=NULL) Rfree(W->expression);
				if(W->fields!=NULL)
				{
					for(y=0,F=W->fields;y<W->numflds;++y,++F)
					{
						if(F->to_name!=NULL) Rfree(F->to_name);
						if(F->from_name!=NULL) Rfree(F->from_name);
					}
					Rfree(W->fields);
				}
			}
			Rfree(MTNMASTER->PrevWrites);
		}
		if(MTNMASTER->NewWrites!=NULL)
		{
			for(x=0,W=MTNMASTER->NewWrites;x<MTNMASTER->numNewWrites;++x,++W)
			{
				if(W->module!=NULL) Rfree(W->module);
				if(W->filename!=NULL) Rfree(W->filename);
				if(W->expression!=NULL) Rfree(W->expression);
				if(W->fields!=NULL)
				{
					for(y=0,F=W->fields;y<W->numflds;++y,++F)
					{
						if(F->to_name!=NULL) Rfree(F->to_name);
						if(F->from_name!=NULL) Rfree(F->from_name);
					}
					Rfree(W->fields);
				}
			}
			Rfree(MTNMASTER->NewWrites);
		}
		if(MTNMASTER->s!=NULL)
		{
			for(x=0,sup=MTNMASTER->s;x<MTNMASTER->num;++x,++sup)
			{
				if(sup->module!=NULL) Rfree(sup->module);
				if(sup->filename!=NULL) Rfree(sup->filename);
				if(sup->cmodule!=NULL) Rfree(sup->cmodule);
				if(sup->cfilename!=NULL) Rfree(sup->cfilename);
				if(sup->expression!=NULL) Rfree(sup->expression);
				if(sup->confld!=NULL)
				{
					for(y=0,confld=sup->confld;y<sup->num;++y,++confld)
					{
						if(confld->fldname!=NULL) Rfree(confld->fldname);
						if(confld->conname!=NULL) Rfree(confld->conname);
					}
					Rfree(sup->confld);
				}
				if(sup->subkeyname!=NULL) Rfree(sup->subkeyname);
			}
			Rfree(MTNMASTER->s);
		}
		if(MTNMASTER->subord!=NULL)
		{
			for(x=0,sub=MTNMASTER->subord;x<MTNMASTER->subnum;++x,++sub)
			{
				if(sub->module!=NULL) Rfree(sub->module);
				if(sub->filename!=NULL) Rfree(sub->filename);
				if(sub->cmodule!=NULL) Rfree(sub->cmodule);
				if(sub->cfilename!=NULL) Rfree(sub->cfilename);
				if(sub->expression!=NULL) Rfree(sub->expression);
				if(sub->Writes!=NULL)
				{
					for(z=0,W=sub->Writes;z<sub->numWrites;++z,++W)
					{
						if(W->module!=NULL) Rfree(W->module);
						if(W->filename!=NULL) Rfree(W->filename);
						if(W->expression!=NULL) Rfree(W->expression);
						if(W->fields!=NULL)
						{
							for(y=0,F=W->fields;y<W->numflds;++y,++F)
							{
								if(F->to_name!=NULL) Rfree(F->to_name);
								if(F->from_name!=NULL) Rfree(F->from_name);
							}
							Rfree(W->fields);
						}
					}
					Rfree(sub->Writes);
				}
				if(sub->confld!=NULL)
				{
					for(y=0,confld=sub->confld;y<sub->num;++y,++confld)
					{
						if(confld->fldname!=NULL) Rfree(confld->fldname);
						if(confld->conname!=NULL) Rfree(confld->conname);
					}
					Rfree(sub->confld);
				}
				if(sub->subkeyname!=NULL) Rfree(sub->subkeyname);
			}
			Rfree(MTNMASTER->subord);
		}
		if(MTNMASTER->lists!=NULL)
		{
			for(x=0,l=MTNMASTER->lists;x<MTNMASTER->numlists;++x,++l)
			{
				if(l->module!=NULL) Rfree(l->module);
				if(l->name!=NULL) Rfree(l->name);
			}
			Rfree(MTNMASTER->lists);
		}
		if(MTNMASTER->buttons!=NULL)
		{
			for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
			{
				if(button->progname!=NULL) Rfree(button->progname);
				if(button->modulename!=NULL) Rfree(button->modulename);
				if(button->filename!=NULL) Rfree(button->filename);
				if(button->keyname!=NULL) Rfree(button->keyname);
				if(button->listname!=NULL) Rfree(button->listname);
				if(button->buttonname!=NULL) Rfree(button->buttonname);
				if(button->args!=NULL) freeapplib(button->args);
				if(button->bf!=NULL)
				{
					for(z=0,bf=button->bf;z<button->num;++z,++bf)
					{
						if(bf->rname!=NULL) Rfree(bf->rname);
						if(bf->ename!=NULL) Rfree(bf->ename);
					}
					Rfree(button->bf);
				}
			}
			Rfree(MTNMASTER->buttons);
		}
		Rfree(MTNMASTER);
	}
}
short xgetMaintainMaster(char *libname,MaintainMaster *MTNMASTER,BIN **outbin,int line,char *file)
{
	short x=0,test=FALSE,version=FALSE;
	BIN *bin=NULL;
	RDAsearch *sup=NULL;
	RDAsubord *sub=NULL;
	int v,y,z,tot=0;
	MaintainList *l=NULL;
	RDAconfld *confld=NULL;
	MaintainButton *button;
	ButtonField *bf=NULL;
	PowerWrite *W=NULL;
	PowerWriteField *F=NULL;
	char *temp=NULL;
	MTNAction *action=NULL;
	Actionstruct *actstruct=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getMaintainMAster Attempting to Read Maintain Master Binary [%s] from Maintain Master Library [%s] at line [%d] program [%s].",MTNMASTER->name,libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(outbin==NULL)
	{
		bin=getlibbin(libname,MTNMASTER->name,FALSE);
		if(bin==NULL) 
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagmix || diagmix_field)
			{
				prterr("Error Attempting to Read Maintain Master Binary [%s] from Maintain Master Library [%s] Failed at line [%d] program [%s].",MTNMASTER->name,libname,line,file); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	} else {
		bin=*outbin;
	}
	x=BINgetshort(bin);
	if(x!=(-1683) && x!=(-1684) && x!=(-1685) && x!=(-1686) && x!=(-1687) && x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary for Maintain Master [%s] in Maintain Master Binary Library [%s] at line [%d] program [%s].",MTNMASTER->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1683)) version=18;
	else if(x==(-1684)) version=17;
	else if(x==(-1685)) version=16;
	else if(x==(-1686)) version=15;
	else if(x==(-1687)) version=14;
	else if(x==(-1688)) version=13;
	else if(x==(-1689)) version=12;
	else if(x==(-1690)) version=11;
	else if(x==(-1691)) version=10;
	else if(x==(-1692)) version=9;
	else if(x==(-1693)) version=8;
	else if(x==(-1694)) version=7;
	else if(x==(-1695)) version=6;
	else if(x==(-1696)) version=5;
	else if(x==(-1697)) version=4;
	else if(x==(-1698)) version=3;
	else if(x==(-1699)) version=2;
	else if(x==(-1700)) version=TRUE;
	else version=FALSE;
	test=BINgetshort(bin);
	if(MTNMASTER->module!=NULL) Rfree(MTNMASTER->module);
	if(test) 
	{
		MTNMASTER->module=BINgetstring(bin);
	} else { 
		MTNMASTER->module=NULL;
	}
	test=BINgetshort(bin);
	if(MTNMASTER->name!=NULL) Rfree(MTNMASTER->name);
	if(test)
	{
		MTNMASTER->name=BINgetstring(bin);
	} else { 
		MTNMASTER->name=NULL;
	}
	test=BINgetshort(bin);
	if(test)
	{
		MTNMASTER->desc=BINgetstring(bin);
	} else { 
		MTNMASTER->desc=NULL;
	}
	MTNMASTER->start_with=BINgetint(bin);
	MTNMASTER->file_type=BINgetint(bin);
	MTNMASTER->style=BINgetint(bin);
	test=BINgetshort(bin);
	if(test)
	{
		MTNMASTER->mainfile=BINgetstring(bin);
	} else { 
		MTNMASTER->mainfile=NULL;
	}
	test=BINgetshort(bin);
	if(test)
	{
		MTNMASTER->browse_screen=BINgetstring(bin);
	} else { 
		MTNMASTER->browse_screen=NULL;
	}
	test=BINgetshort(bin);
	if(test)
	{
		MTNMASTER->define_list=BINgetstring(bin);
	} else { 
		MTNMASTER->define_list=NULL;
	}
	test=BINgetshort(bin);
	if(test)
	{
		MTNMASTER->search_browse=BINgetstring(bin);
	} else { 
		MTNMASTER->search_browse=NULL;
	}
	test=BINgetshort(bin);
	if(test)
	{
		MTNMASTER->maintain_screen=BINgetstring(bin);
	} else { 
		MTNMASTER->maintain_screen=NULL;
	}
	MTNMASTER->save_type=BINgetint(bin);
	if(version>14)
	{
		MTNMASTER->start_where=BINgetint(bin);
	} else { 
		MTNMASTER->start_where=0;
	}
	test=BINgetshort(bin);
	if(test)
	{
		MTNMASTER->save_expression=BINgetstring(bin);
	} else { 
		MTNMASTER->save_expression=NULL;
	}
	if(version>4)
	{
		test=BINgetshort(bin);
		if(test)
		{
			MTNMASTER->save_error_desc=BINgetstring(bin);
		} else { 
			MTNMASTER->save_error_desc=NULL;
		}
		test=BINgetshort(bin);
		if(test)
		{
			MTNMASTER->save_warning=BINgetstring(bin);
		} else { 
			MTNMASTER->save_warning=NULL;
		}
		test=BINgetshort(bin);
		if(test)
		{
			MTNMASTER->save_warning_desc=BINgetstring(bin);
		} else { 
			MTNMASTER->save_warning_desc=NULL;
		}
	} else {
		MTNMASTER->save_error_desc=NULL;
		MTNMASTER->save_warning=NULL;
		MTNMASTER->save_warning_desc=NULL;
	}
	if(version>10)
	{
		test=BINgetshort(bin);
		if(test)
		{ 
			MTNMASTER->Use_Audit_Trail=TRUE;
		} else { 
			MTNMASTER->Use_Audit_Trail=FALSE;
		}
		test=BINgetshort(bin);
		if(test)
		{ 
			MTNMASTER->Audit_Trail=BINgetstring(bin);
		} else { 
			MTNMASTER->Audit_Trail=NULL;
		}
	} else {
		MTNMASTER->Use_Audit_Trail=FALSE;
		MTNMASTER->Audit_Trail=NULL;
	}
	MTNMASTER->num=BINgetint(bin);
	if(MTNMASTER->num>0)
	{
		MTNMASTER->s=Rmalloc(MTNMASTER->num*sizeof(RDAsearch));
		for(y=0,sup=MTNMASTER->s;y<MTNMASTER->num;++y,++sup)
		{
			sup->skip_widget=FALSE;
			sup->fileno=(-1);
			sup->keyno=(-1);
			test=BINgetshort(bin);
			if(test)
			{ 
				sup->module=BINgetstring(bin);
			} else { 
				sup->module=NULL;
			}
			test=BINgetshort(bin);
			if(test)
			{ 
				sup->filename=BINgetstring(bin);
			} else { 
				sup->filename=NULL;
			}
			if(version>9)
			{
				test=BINgetshort(bin);
				if(test)
				{
					sup->expression=BINgetstring(bin);
				} else { 
					sup->expression=NULL;
				}
			} else { 
				sup->expression=NULL;
			}
			sup->ctype=BINgetint(bin);
			if(version>6)
			{
				test=BINgetshort(bin);
				if(test)
				{ 
					sup->cmodule=BINgetstring(bin);
				} else {
					sup->cmodule=NULL;
				}
				test=BINgetshort(bin);
				if(test)
				{ 
					sup->cfilename=BINgetstring(bin);
				} else { 
					sup->cfilename=NULL;
				}
				sup->num=BINgetshort(bin);
				if(sup->num>0)
				{
					sup->confld=Rmalloc(sup->num*sizeof(RDAconfld));
					for(z=0,confld=sup->confld;z<sup->num;++z,++confld)
					{
						test=BINgetshort(bin);
						if(test)
						{ 
							confld->fldname=BINgetstring(bin);
						} else { 
							confld->fldname=NULL;
						}
						test=BINgetshort(bin);
						if(test)
						{ 
							confld->conname=BINgetstring(bin);
						} else { 
							confld->conname=NULL;
						}
						if(version>13)
						{
							test=BINgetshort(bin);
							if(test)
							{ 
								confld->inherit=TRUE;
							} else { 
								confld->inherit=FALSE;
							}
						} else {
							confld->inherit=FALSE;
						}
					}
				} else {
					sup->confld=NULL;
				}
			} else if(sup->ctype>0)
			{
				test=BINgetshort(bin);
				if(test)
				{ 
					sup->cmodule=BINgetstring(bin);
				} else { 
					sup->cmodule=NULL;
				}
				test=BINgetshort(bin);
				if(test) sup->cfilename=BINgetstring(bin);
					else sup->cfilename=NULL;
				sup->num=BINgetshort(bin);
				if(sup->num>0)
				{
					sup->confld=Rmalloc(sup->num*sizeof(RDAconfld));
					for(z=0,confld=sup->confld;z<sup->num;++z,++confld)
					{
						test=BINgetshort(bin);
						if(test) confld->fldname=BINgetstring(bin);
							else confld->fldname=NULL;
						test=BINgetshort(bin);
						if(test) confld->conname=BINgetstring(bin);
							else confld->conname=NULL;
					}
				} else {
					sup->confld=NULL;
				}
			} else {
				sup->cmodule=NULL;
				sup->cfilename=NULL;
				sup->confld=NULL;
			}
			test=BINgetshort(bin);
			if(test) sup->subkeyname=BINgetstring(bin);
				else sup->subkeyname=NULL;
			if(version>7) sup->occurrence=BINgetshort(bin);
				else sup->occurrence=1;
			if(version>13)
			{
				test=BINgetshort(bin);
				if(test) sup->browse=TRUE;
					else sup->browse=FALSE;
				test=BINgetshort(bin);
				if(test) sup->button_name=BINgetstring(bin);
					else sup->button_name=NULL;
				test=BINgetshort(bin);
				if(test) sup->browse_screen=BINgetstring(bin);
					else sup->browse_screen=NULL;
				test=BINgetshort(bin);
				if(test) sup->define_list=BINgetstring(bin);
					else sup->define_list=NULL;
				test=BINgetshort(bin);
				if(test) sup->search_browse=BINgetstring(bin);
					else sup->search_browse=NULL;
				if(version>15)
				{
					test=BINgetshort(bin);
					if(version<17)
					{
						if(test) sup->smart_browse=TRUE;
							else sup->smart_browse=FALSE;
					} else sup->smart_browse=test;
				} else {
					sup->smart_browse=FALSE;
				}
			} else {
				sup->browse=FALSE;
				sup->button_name=NULL;
				sup->browse_screen=NULL;
				sup->define_list=NULL;
				sup->search_browse=NULL;
				sup->smart_browse=FALSE;
			}
			sup->range=FALSE;
			sup->num_writes=1;
			sup->writable=FALSE;
			sup->BL=NULL;
			sup->parent=MTNMASTER;
			sup->parent_rsrc=NULL;
		}
	} else { 
		MTNMASTER->s=NULL;
	}
	MTNMASTER->numlists=BINgetint(bin);
	if(MTNMASTER->numlists>0)
	{
		MTNMASTER->lists=Rmalloc(MTNMASTER->numlists*sizeof(MaintainList));
		for(y=0,l=MTNMASTER->lists;y<MTNMASTER->numlists;++y,++l)
		{
			test=BINgetshort(bin);
			if(test) l->module=BINgetstring(bin);
				else l->module=NULL;
			test=BINgetshort(bin);
			if(test) l->name=BINgetstring(bin);
				else l->name=NULL;
		}
	} else { 
		MTNMASTER->lists=NULL;
	}
	MTNMASTER->runfunc=APPlibNEW();
	addAPPlib(MTNMASTER->runfunc,"Edit Current Record");
	if(version)
	{
		MTNMASTER->numbuttons=BINgetint(bin);
		if(MTNMASTER->numbuttons>0)
		{
			MTNMASTER->buttons=Rmalloc(MTNMASTER->numbuttons*sizeof(MaintainButton));
			for(z=0,button=MTNMASTER->buttons;z<MTNMASTER->numbuttons;++z,++button)
			{
				button->parent=MTNMASTER;
				button->func=NULL;
				test=BINgetshort(bin);
				if(test) button->progname=BINgetstring(bin);
					else button->progname=NULL;
				if(version>2)
				{
					test=BINgetshort(bin);
					if(test) button->modulename=BINgetstring(bin);
						else button->modulename=NULL;
					test=BINgetshort(bin);
					if(test) button->filename=BINgetstring(bin);
						else button->filename=NULL;
				} else {
					button->modulename=NULL;
					button->filename=NULL;
				}
				if(version>1)
				{
					test=BINgetshort(bin);
					if(test) button->keyname=BINgetstring(bin);
						else button->keyname=NULL;
				} else button->keyname=NULL;
				test=BINgetshort(bin);
				if(test) button->listname=BINgetstring(bin);
					else button->listname=NULL;
				addAPPlib(MTNMASTER->runfunc,button->listname);
				test=BINgetshort(bin);
				if(test) button->buttonname=BINgetstring(bin);
					else button->buttonname=NULL;
				button->num=BINgetint(bin);	
				if(button->num>0)
				{
					button->bf=Rmalloc(button->num*sizeof(ButtonField));
					for(y=0,bf=button->bf;y<button->num;++y,++bf)
					{
						test=BINgetshort(bin);
						if(test) bf->rname=BINgetstring(bin);
							else bf->rname=NULL;
						test=BINgetshort(bin);
						if(test) bf->ename=BINgetstring(bin);
							else bf->ename=NULL;
					}
				} else {
					button->bf=NULL;
					button->num=0;
				}
				if(version>11)
				{
					test=BINgetshort(bin);
					if(test)
					{
						button->evalargs=TRUE;
					} else {
						button->evalargs=FALSE;
					}
					if(version>12)
					{
						test=BINgetshort(bin);
						if(test) button->expression=BINgetstring(bin);
							else button->expression=NULL;
						button->type=BINgetshort(bin);
					} else {
						button->expression=NULL;
						button->type=0;
					}
				} else {
					button->evalargs=FALSE;
					button->expression=NULL;
					button->type=0;
				}
				button->args=APPlibNEW();
				if(version>3)
				{
					tot=BINgetint(bin);
					if(tot>0)
					{
						for(y=0;y<tot;++y)
						{
							test=BINgetshort(bin);
							if(test) 
							{
								temp=BINgetstring(bin);
								addAPPlib(button->args,temp);
								if(temp!=NULL) Rfree(temp);
							}
						}
					}
				}
			}
		} else {
			MTNMASTER->buttons=NULL;
			MTNMASTER->numbuttons=0;
		}
	} else {
		MTNMASTER->buttons=NULL;
		MTNMASTER->numbuttons=0;
	}
	if(version>10)
	{
		test=BINgetshort(bin);
		if(test) MTNMASTER->UsePowerWrite=TRUE;
			else MTNMASTER->UsePowerWrite=FALSE;
		MTNMASTER->numPrevWrites=0;
		MTNMASTER->PrevWrites=NULL;
		MTNMASTER->numNewWrites=0;
		MTNMASTER->NewWrites=NULL;
		if(MTNMASTER->UsePowerWrite==TRUE)
		{
			MTNMASTER->numPrevWrites=BINgetint(bin);
			if(MTNMASTER->numPrevWrites>0)
			{
				MTNMASTER->PrevWrites=Rmalloc(MTNMASTER->numPrevWrites*sizeof(PowerWrite));
				for(z=0,W=MTNMASTER->PrevWrites;z<MTNMASTER->numPrevWrites;++z,++W)
				{
					W->fileno=(-1);
					W->closefile=FALSE;
					test=BINgetshort(bin);
					if(test) W->module=BINgetstring(bin);
						else W->module=NULL;
					test=BINgetshort(bin);
					if(test) W->filename=BINgetstring(bin);
						else W->filename=NULL;
					test=BINgetshort(bin);
					if(test) W->expression=BINgetstring(bin);
						else W->expression=NULL;
					W->numflds=BINgetint(bin);
					W->fields=NULL;
					if(W->numflds>0)
					{
						W->fields=Rmalloc(W->numflds*sizeof(PowerWriteField));
						for(v=0,F=W->fields;v<W->numflds;++v,++F)
						{
							test=BINgetshort(bin);
							if(test) F->to_name=BINgetstring(bin);
								else F->to_name=NULL;
							F->copy_type=BINgetshort(bin);
							test=BINgetshort(bin);
							if(test) F->from_name=BINgetstring(bin);
								else F->from_name=NULL;
						}
					} else {
						W->numflds=0;
						W->fields=NULL;
					}
				}
			} else {
				MTNMASTER->PrevWrites=NULL;
				MTNMASTER->numPrevWrites=0;
			}
			MTNMASTER->numNewWrites=BINgetint(bin);
			if(MTNMASTER->numNewWrites>0)
			{
				MTNMASTER->NewWrites=Rmalloc(MTNMASTER->numNewWrites*sizeof(PowerWrite));
				for(z=0,W=MTNMASTER->NewWrites;z<MTNMASTER->numNewWrites;++z,++W)
				{
					W->fileno=(-1);
					W->closefile=FALSE;
					test=BINgetshort(bin);
					if(test) W->module=BINgetstring(bin);
						else W->module=NULL;
					test=BINgetshort(bin);
					if(test) W->filename=BINgetstring(bin);
						else W->filename=NULL;
					test=BINgetshort(bin);
					if(test) W->expression=BINgetstring(bin);
						else W->expression=NULL;
					W->numflds=BINgetint(bin);
					if(W->numflds>0)
					{
						W->fields=Rmalloc(W->numflds*sizeof(PowerWriteField));
						for(v=0,F=W->fields;v<W->numflds;++v,++F)
						{
							test=BINgetshort(bin);
							if(test) F->to_name=BINgetstring(bin);
								else F->to_name=NULL;
							F->copy_type=BINgetshort(bin);
							test=BINgetshort(bin);
							if(test) F->from_name=BINgetstring(bin);
								else F->from_name=NULL;
						}
					} else {
						W->numflds=0;
						W->fields=NULL;
					}
				}
			} else {
				MTNMASTER->NewWrites=NULL;
				MTNMASTER->numNewWrites=0;
			}
		} else {
			MTNMASTER->numPrevWrites=0;
			MTNMASTER->PrevWrites=NULL;
			MTNMASTER->numNewWrites=0;
			MTNMASTER->NewWrites=NULL;
		}
	} else {
		MTNMASTER->UsePowerWrite=FALSE;
		MTNMASTER->numPrevWrites=0;
		MTNMASTER->PrevWrites=NULL;
		MTNMASTER->numNewWrites=0;
		MTNMASTER->NewWrites=NULL;
	}
	if(version>5)
	{
		MTNMASTER->subnum=BINgetint(bin);
		if(MTNMASTER->subnum>0)
		{
			MTNMASTER->subord=Rmalloc(MTNMASTER->subnum*sizeof(RDAsubord));
			for(y=0,sub=MTNMASTER->subord;y<MTNMASTER->subnum;++y,++sub)
			{
				test=BINgetshort(bin);
				sub->fileno=(-1);
				sub->keyno=(-1);
				if(test) sub->module=BINgetstring(bin);
					else sub->module=NULL;
				test=BINgetshort(bin);
				if(test) sub->filename=BINgetstring(bin);
					else sub->filename=NULL;
				if(version>9)
				{
					test=BINgetshort(bin);
					if(test) sub->expression=BINgetstring(bin);
						else sub->expression=NULL;
				} else sub->expression=NULL;
				sub->dtype=BINgetint(bin);
				test=BINgetshort(bin);
				if(test) sub->cmodule=BINgetstring(bin);
					else sub->cmodule=NULL;
				test=BINgetshort(bin);
				if(test) sub->cfilename=BINgetstring(bin);
					else sub->cfilename=NULL;
				sub->num=BINgetshort(bin);
				if(sub->num>0)
				{
					sub->confld=Rmalloc(sub->num*sizeof(RDAconfld));
					for(z=0,confld=sub->confld;z<sub->num;++z,++confld)
					{
						test=BINgetshort(bin);
						if(test) confld->fldname=BINgetstring(bin);
							else confld->fldname=NULL;
						test=BINgetshort(bin);
						if(test) confld->conname=BINgetstring(bin);
							else confld->conname=NULL;
					}
				} else sub->confld=NULL;
				test=BINgetshort(bin);
				if(test) sub->subkeyname=BINgetstring(bin);
					else sub->subkeyname=NULL;
				if(version>10)
				{
					test=BINgetshort(bin);
					if(test) sub->UsePowerWrite=TRUE;
						else sub->UsePowerWrite=FALSE;
					if(sub->UsePowerWrite==TRUE)
					{
						sub->numWrites=BINgetint(bin);
						if(sub->numWrites>0)
						{
							sub->Writes=Rmalloc(sub->numWrites*sizeof(PowerWrite));
							for(z=0,W=sub->Writes;z<sub->numWrites;++z,++W)
							{
								W->fileno=(-1);
								W->closefile=FALSE;
								test=BINgetshort(bin);
								if(test) W->module=BINgetstring(bin);
									else W->module=NULL;
								test=BINgetshort(bin);
								if(test) W->filename=BINgetstring(bin);
									else W->filename=NULL;
								test=BINgetshort(bin);
								if(test) W->expression=BINgetstring(bin);
									else W->expression=NULL;
								W->numflds=BINgetint(bin);
								if(W->numflds>0)
								{
									W->fields=Rmalloc(W->numflds*sizeof(PowerWriteField));
									for(v=0,F=W->fields;v<W->numflds;++v,++F)
									{
										test=BINgetshort(bin);
										if(test) F->to_name=BINgetstring(bin);
											else F->to_name=NULL;
										F->copy_type=BINgetshort(bin);
										test=BINgetshort(bin);
										if(test) 
										{
											F->from_name=BINgetstring(bin);
										} else F->from_name=NULL;
									}
								} else {
									W->numflds=0;
									W->fields=NULL;
								}
							}
						} else {
							sub->numWrites=0;
							sub->Writes=NULL;
						}
					} else {
						sub->numWrites=0;
						sub->Writes=NULL;
					}
				} else {
					sub->UsePowerWrite=FALSE;
					sub->numWrites=0;
					sub->Writes=NULL;
				}
			}
		}
	} else MTNMASTER->subord=NULL;
	if(version>8 && MTNMASTER->style==1) /* style==1 - Utility Master */
	{
		MTNMASTER->inheritrsrcs=BINgetshort(bin);
	} else MTNMASTER->inheritrsrcs=FALSE;
	if(version>14)
	{
		MTNMASTER->numActions=BINgetint(bin);
		if(MTNMASTER->numActions>0)
		{
			MTNMASTER->Actions=Rmalloc(MTNMASTER->numActions*sizeof(MTNAction));
			for(z=0,action=MTNMASTER->Actions;z<MTNMASTER->numActions;++z,++action)
			{
				test=BINgetshort(bin);
				if(test) action->name=BINgetstring(bin);
					else action->name=NULL;
				action->num=BINgetint(bin);
				if(action->num>0)
				{
					action->a=Rmalloc(action->num*sizeof(Actionstruct));
					for(y=0,actstruct=action->a;y<action->num;++y,++actstruct)
					{
						test=BINgetshort(bin);
						if(test) actstruct->to_name=BINgetstring(bin);
							else actstruct->to_name=NULL;
						test=BINgetshort(bin);
						if(test) actstruct->expression=BINgetstring(bin);
							else actstruct->expression=NULL;
						actstruct->typelist=BINgetint(bin);
					}
				} else action->a=NULL;
			}
		} else MTNMASTER->Actions=NULL;
	} else {
		MTNMASTER->Actions=NULL;
		MTNMASTER->numActions=0;
	}
	MTNMASTER->mtnrsrc=NULL;
	MTNMASTER->passparent=NULL;
	if(outbin==NULL)
	{
		BINfree(bin);
	}
	return(0);
}
short xwriteMaintainMaster(char *libname,MaintainMaster *MTNMASTER,BIN **outbin,int line,char *file)
{
	BIN *bin=NULL;
	RDAsearch *sup=NULL;
	RDAsubord *sub=NULL;
	int v,y,z;
	MaintainList *l=NULL;
	RDAconfld *confld=NULL;
	MaintainButton *button=NULL;
	ButtonField *bf=NULL;
	PowerWriteField *F=NULL;
	PowerWrite *W=NULL;
	MTNAction *action=NULL;
	Actionstruct *actstruct=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG writeMaintainMaster Writing MaintainMaster [%s] into Maintain Master Binary Library [%s] at line [%d] program [%s].",(MTNMASTER->name!=NULL ? MTNMASTER->name:""),libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(outbin==NULL)
	{
		bin=BINnew();
	} else {
		bin=*outbin;
	}
	BINaddshort(bin,-1683);	/*type identifier*/
	if(MTNMASTER->module!=NULL) 
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->module);
	} else BINaddshort(bin,FALSE);
	if(MTNMASTER->name!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->name);
	} else BINaddshort(bin,FALSE);
	if(MTNMASTER->desc!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->desc);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,MTNMASTER->start_with);
	BINaddint(bin,MTNMASTER->file_type);
	BINaddint(bin,MTNMASTER->style);
	if(MTNMASTER->mainfile!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->mainfile);
	} else BINaddshort(bin,FALSE);
	if(MTNMASTER->browse_screen!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->browse_screen);
	} else BINaddshort(bin,FALSE);
	if(MTNMASTER->define_list!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->define_list);
	} else BINaddshort(bin,FALSE);
	if(MTNMASTER->search_browse!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->search_browse);
	} else BINaddshort(bin,FALSE);
	if(MTNMASTER->maintain_screen!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->maintain_screen);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,MTNMASTER->save_type);
	BINaddint(bin,MTNMASTER->start_where);
	if(!isEMPTY(MTNMASTER->save_expression))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->save_expression);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(MTNMASTER->save_error_desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->save_error_desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(MTNMASTER->save_warning))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->save_warning);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(MTNMASTER->save_warning_desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->save_warning_desc);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,(MTNMASTER->Use_Audit_Trail==TRUE ? TRUE:FALSE));
	if(!isEMPTY(MTNMASTER->Audit_Trail))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,MTNMASTER->Audit_Trail);
	} else BINaddshort(bin,FALSE);
	if(MTNMASTER->s!=NULL)
	{
		BINaddint(bin,MTNMASTER->num);
		for(y=0,sup=MTNMASTER->s;y<MTNMASTER->num;++y,++sup)
		{
			if(sup->module!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->module);
			} else BINaddshort(bin,FALSE);
			if(sup->filename!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->filename);
			} else BINaddshort(bin,FALSE);
			if(sup->expression!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->expression);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,sup->ctype);
			if(sup->cmodule!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->cmodule);
			} else BINaddshort(bin,FALSE);
			if(sup->cfilename!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->cfilename);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,sup->num);
			if(sup->num>0)
			{
				for(z=0,confld=sup->confld;z<sup->num;++z,++confld)
				{
					if(confld->fldname!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,confld->fldname);
					} else BINaddshort(bin,FALSE);
					if(confld->conname!=NULL) 
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,confld->conname);
					} else BINaddshort(bin,FALSE);
					if(confld->inherit==TRUE)
					{
						BINaddshort(bin,TRUE);
					} else BINaddshort(bin,FALSE);
				}
			}
			if(sup->subkeyname!=NULL && RDAstrlen(sup->subkeyname)>0)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->subkeyname);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,sup->occurrence);
			if(sup->browse==TRUE)
			{
				BINaddshort(bin,TRUE);
			} else {
				BINaddshort(bin,FALSE);
			}
			if(sup->button_name!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->button_name);
			} else BINaddshort(bin,FALSE);
			if(sup->browse_screen!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->browse_screen);
			} else BINaddshort(bin,FALSE);
			if(sup->define_list!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->define_list);
			} else BINaddshort(bin,FALSE);
			if(sup->search_browse!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sup->search_browse);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,sup->smart_browse);
		}
	} else BINaddint(bin,0);
	if(MTNMASTER->numlists>0)
	{
		BINaddint(bin,MTNMASTER->numlists);
		for(y=0,l=MTNMASTER->lists;y<MTNMASTER->numlists;++y,++l)
		{
			if(l->module!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,l->module);
			} else BINaddshort(bin,FALSE);
			if(l->name!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,l->name);
			} else BINaddshort(bin,FALSE);
		}
	} else BINaddint(bin,0);
	if(MTNMASTER->buttons!=NULL)
	{
		BINaddint(bin,MTNMASTER->numbuttons);
		for(y=0,button=MTNMASTER->buttons;y<MTNMASTER->numbuttons;++y,++button)
		{
			if(!isEMPTY(button->progname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->progname);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(button->modulename))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->modulename);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(button->filename))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->filename);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(button->keyname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->keyname);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(button->listname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->listname);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(button->buttonname))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->buttonname);
			} else BINaddshort(bin,FALSE);
			if(button->bf!=NULL)
			{
				BINaddint(bin,button->num);
				for(z=0,bf=button->bf;z<button->num;++z,++bf)
				{
					if(!isEMPTY(bf->rname))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,bf->rname);
					} else BINaddshort(bin,FALSE);
					if(!isEMPTY(bf->ename))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,bf->ename);
					} else BINaddshort(bin,FALSE);
				}
			} else {
				BINaddint(bin,0);
			}
			if(button->evalargs==TRUE)
			{
				BINaddshort(bin,TRUE);
			} else {
				BINaddshort(bin,FALSE);
			}
			if(!isEMPTY(button->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->expression);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,button->type);
			if(button->args!=NULL)
			{
				BINaddint(bin,button->args->numlibs);
				for(z=0;z<button->args->numlibs;++z)
				{
					if(!isEMPTY(button->args->libs[z]))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,button->args->libs[z]);
					} else BINaddshort(bin,FALSE);
				}
			} else BINaddint(bin,0);
		}
	} else BINaddint(bin,0);
	BINaddshort(bin,(MTNMASTER->UsePowerWrite==TRUE ? TRUE:FALSE));
	if(MTNMASTER->UsePowerWrite==TRUE)
	{
		BINaddint(bin,MTNMASTER->numPrevWrites);
		if(MTNMASTER->numPrevWrites>0)
		{
			for(y=0,W=MTNMASTER->PrevWrites;y<MTNMASTER->numPrevWrites;++y,++W)
			{
				if(!isEMPTY(W->module))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,W->module);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(W->filename))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,W->filename);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(W->expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,W->expression);
				} else BINaddshort(bin,FALSE);
				if(W->fields!=NULL && W->numflds>0)
				{
					BINaddint(bin,W->numflds);
					for(z=0,F=W->fields;z<W->numflds;++z,++F)
					{
						if(!isEMPTY(F->to_name))
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,F->to_name);
						} else BINaddshort(bin,FALSE);
						BINaddshort(bin,F->copy_type);
						if(!isEMPTY(F->from_name))
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,F->from_name);
						} else BINaddshort(bin,FALSE);
					}
				} else {
					BINaddint(bin,0);
				}
			}
		}
		BINaddint(bin,MTNMASTER->numNewWrites);
		if(MTNMASTER->numNewWrites>0)
		{
			for(y=0,W=MTNMASTER->NewWrites;y<MTNMASTER->numNewWrites;++y,++W)
			{
				if(!isEMPTY(W->module))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,W->module);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(W->filename))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,W->filename);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(W->expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,W->expression);
				} else BINaddshort(bin,FALSE);
				if(W->fields!=NULL && W->numflds>0)
				{
					BINaddint(bin,W->numflds);
					for(z=0,F=W->fields;z<W->numflds;++z,++F)
					{
						if(!isEMPTY(F->to_name))
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,F->to_name);
						} else BINaddshort(bin,FALSE);
						BINaddshort(bin,F->copy_type);
						if(!isEMPTY(F->from_name))
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,F->from_name);
						} else BINaddshort(bin,FALSE);
					}
				} else {
					BINaddint(bin,0);
				}
			}
		}
	}
	if(MTNMASTER->subord!=NULL)
	{
		BINaddint(bin,MTNMASTER->subnum);
		for(y=0,sub=MTNMASTER->subord;y<MTNMASTER->subnum;++y,++sub)
		{
			if(sub->module!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sub->module);
			} else BINaddshort(bin,FALSE);
			if(sub->filename!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sub->filename);
			} else {
				BINaddshort(bin,FALSE);
			}
			if(sub->expression!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sub->expression);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,sub->dtype);
			if(!isEMPTY(sub->cmodule))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sub->cmodule);
			} else {
				BINaddshort(bin,FALSE);
			}
			if(!isEMPTY(sub->cfilename))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sub->cfilename);
			} else {
				BINaddshort(bin,FALSE);
			}
			BINaddshort(bin,sub->num);
			if(sub->num>0)
			{
				for(z=0,confld=sub->confld;z<sub->num;++z,++confld)
				{
					if(confld->fldname!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,confld->fldname);
					} else {
						BINaddshort(bin,FALSE);
					}
					if(confld->conname!=NULL) 
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,confld->conname);
					} else {
						BINaddshort(bin,FALSE);
					}
				}
			}
			if(sub->subkeyname!=NULL && RDAstrlen(sub->subkeyname)>0)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,sub->subkeyname);
			} else {
				BINaddshort(bin,FALSE);
			}
			BINaddshort(bin,(sub->UsePowerWrite==TRUE ? TRUE:FALSE));
			if(sub->UsePowerWrite==TRUE)
			{
				BINaddint(bin,sub->numWrites);
				if(sub->numWrites>0)
				{
					for(v=0,W=sub->Writes;v<sub->numWrites;++v,++W)
					{
						if(!isEMPTY(W->module))
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,W->module);
						} else BINaddshort(bin,FALSE);
						if(!isEMPTY(W->filename))
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,W->filename);
						} else BINaddshort(bin,FALSE);
						if(!isEMPTY(W->expression))
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,W->expression);
						} else BINaddshort(bin,FALSE);
						if(W->fields!=NULL && W->numflds>0)
						{
							BINaddint(bin,W->numflds);
							for(z=0,F=W->fields;z<W->numflds;++z,++F)
							{
								if(!isEMPTY(F->to_name))
								{
									BINaddshort(bin,TRUE);
									BINaddstring(bin,F->to_name);
								} else BINaddshort(bin,FALSE);
								BINaddshort(bin,F->copy_type);
								if(!isEMPTY(F->from_name))
								{
									BINaddshort(bin,TRUE);
									BINaddstring(bin,F->from_name);
								} else BINaddshort(bin,FALSE);
							}
						} else {
							BINaddint(bin,0);
						}
					}
				}
			}
		}
	} else {
		BINaddint(bin,0);
	}
	if(MTNMASTER->style==1) /* style==1 - Utility Master */
	{
		BINaddshort(bin,MTNMASTER->inheritrsrcs);
	}
	if(MTNMASTER->Actions!=NULL && MTNMASTER->numActions>0)
	{
		BINaddint(bin,MTNMASTER->numActions);
		for(y=0,action=MTNMASTER->Actions;y<MTNMASTER->numActions;++y,++action)
		{
			if(!isEMPTY(action->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,action->name);
			} else BINaddshort(bin,FALSE);
			if(action->a!=NULL && action->num>0)
			{
				BINaddint(bin,action->num);
				for(z=0,actstruct=action->a;z<action->num;++z,++actstruct)
				{
					if(!isEMPTY(actstruct->to_name))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,actstruct->to_name);
					} else BINaddshort(bin,FALSE);
					if(!isEMPTY(actstruct->expression))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,actstruct->expression);
					} else BINaddshort(bin,FALSE);
					BINaddint(bin,actstruct->typelist);
				}
			} else BINaddint(bin,0);
		}
	} else BINaddint(bin,0);
	if(outbin==NULL)
	{
		if(writelibbin(libname,bin,MTNMASTER->name))
		{
			prterr("Error Can't write MaintainMaster binary [%s] into Maintain Master library [%s] at line [%d] program [%s].",(MTNMASTER->name!=NULL ? MTNMASTER->name:""),libname,line,file);
			return(-1);
		}
	}
	return(0);
}
char *xgetMaintainMasterdesc(char *libname,char *name,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getMaintainMAsterdesc Attempting to Read Maintain Master description  for [%s] from Maintain Master Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagmix || diagmix_field)
		{
			prterr("Error Attempting to Read Maintain Master Binary [%s] from Maintain Master Library [%s] Failed at line [%d] program [%s].",name,libname,line,file); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(NULL);
	}
	x=BINgetshort(bin);
	if(x!=(-1683) && x!=(-1684) && x!=(-1685) && x!=(-1686) && x!=(-1687) && x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary for Maintain Master [%s] in Maintain Master Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
		BINfree(bin);
		return(NULL);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
	} else temp=NULL;
	BINfree(bin);
	return(temp);
}
short xfindMaintainMaster(MaintainMaster *MTNMASTER,int line,char *file)
{
	short ef=(-1);
	char *libname=NULL,*dashes=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field) 
	{ 
		prterr("DIAG findMaintainMAster [%s] [%s] at line [%d] program [%s].",MTNMASTER->module,MTNMASTER->name,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	dashes=adddashes(MTNMASTER->module);
	libname=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
	sprintf(libname,"%s/%s/%s.MTN",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s\\%s.MTN",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
	if(dashes!=NULL) Rfree(dashes);
	if(!getMaintainMaster(libname,MTNMASTER)) ef=FALSE;
	if(ef && (!isEMPTY(RDA_GROUP)))
	{
		dashes=adddashes(MTNMASTER->module);
		libname=Rmalloc(RDAstrlen(RDA_GROUP)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
		sprintf(libname,"%s/%s/%s.MTN",CURRENTDIRECTORY,RDA_GROUP,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\%s\\%s.MTN",CURRENTDIRECTORY,RDA_GROUP,dashes);
#endif
		if(dashes!=NULL) Rfree(dashes);
		if(!getMaintainMaster(libname,MTNMASTER)) ef=FALSE;
	}
	if(ef)
	{
		if(libname!=NULL) 
		{
			libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(MTNMASTER->module)+11);
		} else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(MTNMASTER->module)+11);
		}
#ifndef WIN32
		sprintf(libname,"%s/site/%s.MTN",CURRENTDIRECTORY,MTNMASTER->module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\site\\%s.MTN",CURRENTDIRECTORY,MTNMASTER->module);
#endif
		if(!getMaintainMaster(libname,MTNMASTER)) ef=FALSE;
	}
	if(ef)
	{
		if(libname!=NULL) 
		{
			libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(MTNMASTER->module)+10);
		} else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
					RDAstrlen(MTNMASTER->module)+10);
		}
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.MTN",CURRENTDIRECTORY,MTNMASTER->module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,MTNMASTER->module);
#endif
		if(!getMaintainMaster(libname,MTNMASTER)) ef=FALSE;
	}
	if(ef)
	{
		prterr("Error Can't Find MaintainMaster [%s] [%s] at line [%d] program [%s].",MTNMASTER->module,MTNMASTER->name,line,file);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		if(ef)
		{
			prterr("DIAG findMaintainMaster Looking for [%s][%s] at line [%d] program [%s] has NOT been found in Maintain Master Library [%s].",MTNMASTER->module,MTNMASTER->name,line,file,libname);
		} else {
			prterr("DIAG findMaintainMaster Looking for [%s][%s] at line [%d] program [%s] has been found in Maintain Master Library [%s].",MTNMASTER->module,MTNMASTER->name,line,file,libname);
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(libname!=NULL) Rfree(libname);
	return(ef);
}
void xMakeRunFunction(RDArsrc *mainrsrc,MaintainMaster *MTNMASTER,int line,
	char *file)
{
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MakeRunFunction at line [%d] program [%s].",
			line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(MTNMASTER!=NULL)
	{
		if(MTNMASTER->runfunc!=NULL)
		{
			if(MTNMASTER->runfunc->numlibs>0)
			{
				addlstrsrc(mainrsrc,"RUN FUNCTION",&x,TRUE,NULL,
					MTNMASTER->runfunc->numlibs,&MTNMASTER->runfunc->libs,NULL);
			}
		}
	}
}
void xMakeMasterButtons(RDArsrc *mainrsrc,MaintainMaster *MTNMASTER,
	int line,char *file)
{
	int x;
	MaintainButton *button;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MakeMaintainMaster at line [%d] program [%s].",
			line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(MTNMASTER!=NULL)
	{
		if(MTNMASTER->buttons!=NULL)
		{
			for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
			{
				addbtnrsrc(mainrsrc,button->buttonname,TRUE,
					MasterButtonFunction,button);
			}
		}
	}
}
APPlib *xMaintainButton2PassKey(MaintainButton *button,short fileno,
	int line,char *file)
{
	APPlib *tmp=NULL;
	char *temp=NULL,*mod=NULL,*fil=NULL;
	short x=0,y=0;
	NRDfield *field;
	ButtonField *btnfld=NULL;
	MaintainMaster *MTNMSTR=NULL;
	RDAvirtual *v=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
	RDArsrc *rsrc=NULL;
	MakeBrowseList *blist=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MaintainButton2PassKey for Program [%s] Using File [%d] [%s] [%s] at line [%d] program [%s].",button->progname,fileno,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=APPlibNEW();
	mod=MODULENAME(fileno);
	fil=FILENAME(fileno);
	temp=Rmalloc(13+RDAstrlen(mod));
	sprintf(temp,"FROM_MODULE=%s",mod);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(15+RDAstrlen(fil));
	sprintf(temp,"FROM_FILENAME=%s",fil);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(9+RDAstrlen(button->keyname));
	sprintf(temp,"KEYNAME=%s",button->keyname);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	if(button->bf!=NULL)
	{
		for(x=0,btnfld=button->bf;x<button->num;++x,++btnfld)
		{
			if(!isEMPTY(btnfld->rname))
			{
				field=FLDNRD(fileno,btnfld->rname);
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
					case ZIPCODE:
					case PHONE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case CUSTOMTYPE:
						temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(field->data.string_value));
						sprintf(temp,"%s=%s",btnfld->ename,(field->data.string_value!=NULL ? field->data.string_value:""));
						addAPPlib(tmp,temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case CHARACTERS:
					case BOOLNS:
						sprintf(stemp,"%d",(*field->data.string_value ? 
							TRUE:FALSE));
						temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
						sprintf(temp,"%s=%s",btnfld->ename,stemp);
						addAPPlib(tmp,temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						sprintf(stemp,"%f",*field->data.float_value);
						temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
						sprintf(temp,"%s=%s",btnfld->ename,stemp);
						addAPPlib(tmp,temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
					case SSHORTV:
						sprintf(stemp,"%d",*field->data.short_value);
						temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
						sprintf(temp,"%s=%s",btnfld->ename,stemp);
						addAPPlib(tmp,temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						sprintf(stemp,"%d",*field->data.integer_value);
						temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
						sprintf(temp,"%s=%s",btnfld->ename,stemp);
						addAPPlib(tmp,temp);
						if(temp!=NULL) Rfree(temp);
						break;
					default:
						prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
						break;
				}
			} else if(button->parent!=NULL)
			{

				PSTRUCT=(MTNPassableStruct *)button->parent;
				if(PSTRUCT!=NULL)
				{
				MTNMSTR=(MaintainMaster *)PSTRUCT->MTNMASTER;
				if(MTNMSTR!=NULL)
				{
					if(MTNMSTR->mtnrsrc!=NULL) 
					{
						rsrc=MTNMSTR->mtnrsrc;
					} else {
						blist=MTNMSTR->mbl_utlmstr;
						rsrc=blist->definelist;
					}
					if(rsrc!=NULL)
					{
						if(rsrc->virflds!=NULL)
						{
							for(y=0,v=rsrc->virflds;y<rsrc->num;++y,++v)
							{
								if(!RDAstrcmp(btnfld->rname,v->name)) break;
							}
							if(y<rsrc->num)
							{
								if(!v->computed)
								{
									computevirtual(v,MTNMSTR->SubFunc,MTNMSTR->SubFuncArgs);
								}
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
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case CUSTOMTYPE:
										temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(v->value.string_value));
										sprintf(temp,"%s=%s",btnfld->ename,(v->value.string_value!=NULL ? v->value.string_value:""));
										addAPPlib(tmp,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case CHARACTERS:
									case BOOLNS:
										sprintf(stemp,"%d",(*v->value.string_value ? 
											TRUE:FALSE));
										temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
										sprintf(temp,"%s=%s",btnfld->ename,stemp);
										addAPPlib(tmp,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case DECIMALV:
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case SDECIMALV:
										sprintf(stemp,"%f",*v->value.float_value);
										temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
										sprintf(temp,"%s=%s",btnfld->ename,stemp);
										addAPPlib(tmp,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case SHORTV:
									case SSHORTV:
										sprintf(stemp,"%d",*v->value.short_value);
										temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
										sprintf(temp,"%s=%s",btnfld->ename,stemp);
										addAPPlib(tmp,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										sprintf(stemp,"%d",*v->value.integer_value);
										temp=Rmalloc(RDAstrlen(btnfld->ename)+2+RDAstrlen(stemp));
										sprintf(temp,"%s=%s",btnfld->ename,stemp);
										addAPPlib(tmp,temp);
										if(temp!=NULL) Rfree(temp);
										break;
									default:
										prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",v->type,v->name,line,file);
										break;
								}
							}
						}
					}
				}
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MaintainButton2PassKey at line [%d] program [%s].",
			line,file);
		SEEAPPlib(tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(tmp);
}
void xExecuteListFunc(int listnum,void *targetkey,MaintainMaster *MTNMASTER,
	int line,char *file)
{
	APPlib *envpx=NULL;
	MaintainButton *button;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG ExecuteListFunc at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(MTNMASTER!=NULL)
	{
		if(listnum<MTNMASTER->numbuttons && listnum>=0)
		{
			button=MTNMASTER->buttons+listnum;
			if(EQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,
				targetkey)) 
			{
				ZERNRD(MTNMASTER->passkey->fileno);
#ifdef USE_RDA_DIAGNOSTICS
				if(diagmix || diagmix_field)
				{
					prterr("Error in ExecuteListFunc targetkey not found at line [%d] program [%s].",line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
			}
			envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
			ADVExecuteProgram(button->progname,button->args,envpx);
			if(envpx!=NULL) freeapplib(envpx);
		} else {
			prterr("Error in ExecuteListFunc Button Number [%d] not defined in Maintain Master [%s][%s] at line [%d] program [%s].",
				listnum,MTNMASTER->module,MTNMASTER->name,line,file);
		}
	} else {
		prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",line,file);
	}
}
void MasterButtonFunction(RDArsrc *mainrsrc,MaintainButton *button)
{
	MaintainMaster *MTNMASTER=NULL;
	APPlib *envpx=NULL;


	if(button!=NULL)
	{
	MTNMASTER=(MaintainMaster *)button->parent;
	if(MTNMASTER!=NULL)
	{
		xrsrc2filerecord(MTNMASTER->passkey->fileno,mainrsrc,MTNMASTER->file_type,__LINE__,__FILE__);
		envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
		ADVExecuteProgram(button->progname,button->args,envpx);
		if(envpx!=NULL) freeapplib(envpx);
	} else {
		prterr("Error in MasterButtonFunction.");
	}
	}
}
short xOPEN_MASTER_FILES(MaintainMaster *MTNMASTER,int line,char *file)
{
	RDAsearch *sup;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG OPEN_MASTER_FILES at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(MTNMASTER!=NULL)
	{
		for(x=0,sup=MTNMASTER->s;x<MTNMASTER->num;++x,++sup)
		{
			if((sup->fileno=APPOPNNRD(sup->module,sup->filename,TRUE,(sup->writable==TRUE?TRUE:FALSE)))==(-1)) return(FALSE);
			sup->keyno=KEYNUMBER(sup->fileno,sup->subkeyname);
			if(sup->keyno<1) sup->keyno=1;
		}
		return(TRUE);
	} else {
		prterr("Error MaintainMaster is NULL at line [%d] program [%s].",line,file);
		return(FALSE);
	}
}
void xCLOSE_MASTER_FILES(MaintainMaster *MTNMASTER,int line,char *file)
{
	RDAsearch *sup;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG CLOSE_MASTER_FILES at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(MTNMASTER!=NULL)
	{
		if(MTNMASTER->passkey!=NULL)
		{
			if(MTNMASTER->passkey->fileno!=(-1)) CLSNRD(MTNMASTER->passkey->fileno);
		}
		for(x=0,sup=MTNMASTER->s;x<MTNMASTER->num;++x,++sup)
		{
			if(sup->fileno!=(-1)) CLSNRD(sup->fileno);
		}
	}
}
static void SEERDAScrolledList_header(RDA_PFILE *fp,RDAScrolledList *SL,char print_head,int *pages,int *numpagelines)
{
	char *date=NULL,*timex=NULL;

	if(print_head)
	{
		if(*pages==1 && *numpagelines==1)
		{
			date=GETCURRENTDATE();
			timex=GETCURRENTTIME();
			RDA_fprintf(fp,"%*sXPERT SCROLLED LIST DEFINITION          Page: %*d\r\n",
				25,"",8,*pages);
			RDA_fprintf(fp,"%*sModule [%s]  Name [%s]%*sDate: %s\r\n",((80-(19+RDAstrlen(SL->module)+RDAstrlen(SL->name)))/2),"",SL->module,SL->name,(((80-(18+RDAstrlen(SL->module)+RDAstrlen(SL->name)))/2)-14),"",date);
			RDA_fprintf(fp,"%*s Executed By: %s%*sTime: %s\r\n",
				((80-(14+RDAstrlen(USERLOGIN)))/2),"",
				USERLOGIN,
				(((80-(14+RDAstrlen(USERLOGIN)))/2)-14),"",
				timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
			*numpagelines=*numpagelines+3;
		}
	}
	RDA_fprintf(fp,"\r\n"); *numpagelines=*numpagelines+1;
	if(print_head)
	{
		if(*numpagelines>=60)
		{
			RDA_fprintf(fp,"\f"); *numpagelines=1; *pages=*pages+1;
			RDA_fprintf(fp,"%*sXPERT SCROLLED LIST DEFINITION          Page: %*d\r\n",
				25,"",8,*pages);
			RDA_fprintf(fp,"%*sModule [%s]  Name [%s]\r\n",
				((79-(18+RDAstrlen(SL->module)+
				RDAstrlen(SL->name)))/2),"",
				SL->module,SL->name);
			RDA_fprintf(fp,"%*s Executed By: %s\r\n\r\n",
				((80-(14+RDAstrlen(USERLOGIN)))/2),"",USERLOGIN);
				*numpagelines=*numpagelines+5;
		}
	}
}
static void fixSLout(RDA_PFILE *fp,char *data,RDAScrolledList *SL,short print_head,int *pages,int *numpagelines,unsigned line_length,char *initstring)
{
	unsigned y=0,x=0,length=0,initoffset=0;
	char *temp=NULL,*temp1=NULL,*temp2=NULL;

	if(!isEMPTY(data))
	{
		initoffset=RDAstrlen(initstring);
		temp=strchr(data,'\n');
		temp1=strchr(data,'\r');
		if(temp==NULL && temp1==NULL)
		{
			length=RDAstrlen(data);
		} else if(temp==NULL)
		{
			length=temp1-data;
		} else if(temp1==NULL)
		{
			length=temp-data;
		} else if(temp<temp1)
		{
			length=temp-data;
		} else {
			length=temp1-data;
		}
		for(x=0,y=0;x<length;++x,++y)
		{
			if(data[x]=='\t')
			{
				if(y+9>(line_length-initoffset))
				{
					break;
				}  
				y+=7;
			}
			if(y+2>(line_length-initoffset))
			{
				break;
			}
		}
		y=x;
		temp2=(data+x+1);
		if(temp2[0]!=0)
		{
			if(temp2[0]!=' ')
			{
				for(y=x;y>0;--y)
				{
					if(data[y]==' ') break;
				}
			}
			if(y<1) y=x;
		}
		temp2=Rmalloc(initoffset+1);
		sprintf(temp2,"%*s",initoffset,"");
		if(y<length)
		{
			RDA_fprintf(fp,"%s%.*s\r\n",initstring,y+1,data);
			fixSLout(fp,data+y+1,SL,print_head,pages,numpagelines,line_length,temp2);
		} else {
			RDA_fprintf(fp,"%s%.*s",initstring,length,data);
			if(temp!=NULL || temp1!=NULL)
			{
				fixSLout(fp,data+length+1,SL,print_head,pages,numpagelines,line_length,temp2);
			}
		}
		if(temp2!=NULL) Rfree(temp2);
	}
}
void xSEERDAScrolledList_detail(RDAScrolledList *SL,RDA_PFILE *fp,char detail,int *pages,int *numpagelines,int line,char *file)
{
	DFincvir *i=NULL;
	unsigned line_length=79;
	char *temp_head=NULL,*temp=NULL;
	int y;

	if(fp!=NULL)
	{
	if(SL!=NULL)
	{
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		temp_head=stralloc("Description: [");
		temp=Rmalloc(3+(SL->desc==NULL?3:RDAstrlen(SL->desc)));
		sprintf(temp,"%s]",(SL->desc==NULL?"":SL->desc));
		if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
		{
			fixSLout(fp,temp,SL,detail,pages,numpagelines,line_length,temp_head);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		} else {
			RDA_fprintf(fp,"%s%s",temp_head,temp);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		}
		if(temp_head!=NULL) Rfree(temp_head);
		if(temp!=NULL) Rfree(temp);
		if(SL->type==0)
		{
			temp=stralloc("From File");
		} else if(SL->type==1)
		{
			temp=stralloc("Parsed List");
		} else if(SL->type==2)
		{
			temp=stralloc("Special");
		} else {
			temp=stralloc("*** BAD VALUE! ***");
		}
		RDA_fprintf(fp,"List Type: [%d]=[%s]",SL->type,temp);
		if(temp!=NULL) Rfree(temp);
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		if(SL->list!=NULL)
		{
			if(SL->list->numlibs>0)
			{
				for(y=0;y<SL->list->numlibs;++y)
				{
					RDA_fprintf(fp,"   List Element [%d]=[%s]",
						y,
						(SL->list->libs[y]!=NULL?SL->list->libs[y]:""));
					SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
				}
			}
		}
		if(SL->ffinfo!=NULL)
		{
			if(SL->ffinfo->module!=NULL &&
			 	SL->ffinfo->file!=NULL &&
				SL->ffinfo->keyname!=NULL)
			{
				RDA_fprintf(fp,"   From Module [%s] From File [%s] From Keyname [%s] Fileno [%d]",
					(SL->ffinfo->module!=NULL?SL->ffinfo->module:""),
					(SL->ffinfo->file!=NULL?SL->ffinfo->file:""),
					(SL->ffinfo->keyname!=NULL?SL->ffinfo->keyname:""),
					SL->ffinfo->fileno);
				SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
			}
		}
		RDA_fprintf(fp,"From Field: [%s]",(SL->ffield==NULL?"":SL->ffield));
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		if(SL->contype==0)
		{
			temp=stralloc("Normal Field");
		} else if(SL->contype==1)
		{
			temp=stralloc("Special Field");
		} else if(SL->contype==2)
		{
			temp=stralloc("Virtual Field");
		} else if(SL->contype==3)
		{
			temp=stralloc("Range Screen Field");
		} else {
			temp=stralloc("*** BAD VALUE! ***");
		}
		RDA_fprintf(fp,"Connection Type: [%d]=[%s]  Connection Module [%s]",
			SL->contype,temp,SL->conmod);
		if(temp!=NULL) Rfree(temp);
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		RDA_fprintf(fp,"   Connection File: [%s]  Connection Field: [%s]",
			SL->confil,SL->confld);
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		if(SL->vtype==0)
		{
			temp=stralloc("When ALL Files Present");
		} else if(SL->vtype==1)
		{
			temp=stralloc("When One of Many Present");
		} else {
			temp=stralloc("*** BAD VALUE! ***");
		}
		RDA_fprintf(fp,"Condition Type: [%d]=[%s]",SL->vtype,temp);
		if(temp!=NULL) Rfree(temp);
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		if(SL->incvir!=NULL)
		{
			for(y=0,i=SL->incvir;y<SL->numincvirs;++y,++i)
			{
				RDA_fprintf(fp,"   Module [%s] File [%s] Keyname [%s] Fileno [%d]",
					(i->module!=NULL?i->module:""),
					(i->file!=NULL?i->file:""),
					(i->keyname!=NULL?i->keyname:""),
					i->fileno);
				SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
			}
		}
		if(SL->timing==0)
		{
			temp=stralloc("Initial Only");
		} else if(SL->timing==1)
		{
			temp=stralloc("Initial & Update Virtuals");
		} else if(SL->timing==2)
		{
			temp=stralloc("Update Virtuals Only");
		} else {
			temp=stralloc("*** BAD VALUE! ***");
		}
		RDA_fprintf(fp,"Timing: [%d]=[%s]",SL->timing,temp);
		if(temp!=NULL) Rfree(temp);
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		temp_head=stralloc("Select Formula: [");
		temp=Rmalloc(3+(SL->select_formula==NULL?3:RDAstrlen(SL->select_formula)));
		sprintf(temp,"%s]",(SL->select_formula==NULL?"":SL->select_formula));
		if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
		{
			fixSLout(fp,temp,SL,detail,pages,numpagelines,line_length,temp_head);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		} else {
			RDA_fprintf(fp,"%s%s",temp_head,temp);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		}
		if(temp_head!=NULL) Rfree(temp_head);
		if(temp!=NULL) Rfree(temp);
		temp_head=stralloc("Format Formula: [");
		temp=Rmalloc(3+(SL->format_formula==NULL?3:RDAstrlen(SL->format_formula)));
		sprintf(temp,"%s]",(SL->format_formula==NULL?"":SL->format_formula));
		if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
		{
			fixSLout(fp,temp,SL,detail,pages,numpagelines,line_length,temp_head);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		} else {
			RDA_fprintf(fp,"%s%s",temp_head,temp);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		}
		if(temp_head!=NULL) Rfree(temp_head);
		if(temp!=NULL) Rfree(temp);
		temp_head=stralloc("Unformat Formula: [");
		temp=Rmalloc(3+(SL->unformat_formula==NULL?4:RDAstrlen(SL->unformat_formula)));
		sprintf(temp,"%s]",(SL->unformat_formula==NULL?"":SL->unformat_formula));
		if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
		{
			fixSLout(fp,temp,SL,detail,pages,numpagelines,line_length,temp_head);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		} else {
			RDA_fprintf(fp,"%s%s",temp_head,temp);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		}
		if(temp_head!=NULL) Rfree(temp_head);
		if(temp!=NULL) Rfree(temp);
		temp_head=stralloc("Reformat Formula: [");
		temp=Rmalloc(3+(SL->reformat_formula==NULL?4:RDAstrlen(SL->reformat_formula)));
		sprintf(temp,"%s]",(SL->reformat_formula==NULL?"":SL->reformat_formula));
		if((RDAstrlen(temp)+RDAstrlen(temp_head))>=line_length)
		{
			fixSLout(fp,temp,SL,detail,pages,numpagelines,line_length,temp_head);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		} else {
			RDA_fprintf(fp,"%s%s",temp_head,temp);
			SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
		}
		if(temp_head!=NULL) Rfree(temp_head);
		if(temp!=NULL) Rfree(temp);
		RDA_fprintf(fp,"Special Function: [%s]",(SL->special_function!=NULL?SL->special_function:""));
		SEERDAScrolledList_header(fp,SL,detail,pages,numpagelines);
	} else {
		prterr("ERROR: Null Scrolled Lists.");
	}
	} else {
		prterr("ERROR: No file pointer.");
	}
}
static void printRDAScrolledList(RDArsrc *prsrc,RDAScrolledList *SL)
{
	int pages=1,numpagelines=1;
	char *outdevice=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;

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
			SEERDAScrolledList_detail(SL,fp,TRUE,&pages,&numpagelines);
			RDA_pclose(fp);
		} else {
			ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
                        prterr("Error PrintRDAScrolledList RDA_popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
                        if(outdevice!=NULL) Rfree(outdevice);
                }
        } else {
		ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
        }
}
static void quit_printRDAScrolledList(RDArsrc *prsrc)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG quit_printRDAScrolledList Qruitting Print RDA Scrolled List");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	free_rsrc(prsrc);
}
static void GetDevicePrintRDAScrolledList(RDAScrolledList *SL)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAScrolledList,SL);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAScrolledList,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);

	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
void xSEERDAScrolledList(RDAScrolledList *SL,RDA_PFILE *fp,
	char detail,int line,char *file)
{
	int pages=1,numpagelines=1;

	if(SL==NULL)
	{
		prterr("Error SEERDAScrolledList has no Scrolled List Definition, i.e. the structure is NULL prior to line [%d] program [%s].",line,file);
		return;
	}
	if(detail)
	{
		GetDevicePrintRDAScrolledList(SL);
	} else {
		prterr("DIAG SEERDAScrolledList Show Structure for RDAScrolledList [%s] at line [%d] program [%s].",SL->name,line,file);
		SEERDAScrolledList_detail(SL,fp,FALSE,&pages,&numpagelines);
	}
}
void xScrolledListDFincvir(RDAScrolledList *l,char *mod,char *fil,char *key,
	short fileno,int line,char *file)
{
	DFincvir *i=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG ScrolledListDFincvir at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(l!=NULL)
	{
		if(l->incvir!=NULL)
		{
			l->incvir=Rrealloc(l->incvir,(l->numincvirs+1)*sizeof(DFincvir));
		} else {
			l->incvir=Rmalloc(sizeof(DFincvir));
			l->numincvirs=0;
		}
		i=l->incvir+l->numincvirs;
		i->module=stralloc(mod);
		i->file=stralloc(fil);
		i->keyname=stralloc(key);
		i->fileno=fileno;
		i->v=NULL;
		++l->numincvirs;
	} else {
		prterr("Error RDAScrolledList is NULL at line [%d] program [%s].",line,file);
	}
}
void xDFvirtualDFincvir(DFvirtual *l,char *mod,char *fil,char *key,
	short fileno,int line,char *file)
{
	DFincvir *i=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DFvirtualDFincvir at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(l!=NULL)
	{
		if(l->incvir!=NULL)
		{
			l->incvir=Rrealloc(l->incvir,(l->num+1)*sizeof(DFincvir));
		} else {
			l->incvir=Rmalloc(sizeof(DFincvir));
			l->num=0;
		}
		i=l->incvir+l->num;
		i->module=stralloc(mod);
		i->file=stralloc(fil);
		i->keyname=stralloc(key);
		i->fileno=fileno;
		i->v=NULL;
		++l->num;
	} else {
		prterr("Error DFvirtual is NULL at line [%d] program [%s].",line,file);
	}
}
void SetRDAsearchOccurrence(MaintainMaster *MTNMASTER)
{
	int x,y;
	short occurrence;
	RDAsearch *s,*s1;

	if(MTNMASTER->s)
	{
		for(x=0,s=MTNMASTER->s;x<MTNMASTER->num;++x,++s) s->occurrence=(-1);
		for(x=0,s=MTNMASTER->s;x<MTNMASTER->num;++x,++s)
		{
			if(s->occurrence==(-1))
			{
				s->occurrence=1;
				occurrence=1;
				for(y=(x+1);y<MTNMASTER->num;++y)
				{
					s1=MTNMASTER->s+y;
					if(s1->occurrence==(-1))
					{
						if(!RDAstrcmp(s->module,s1->module) &&
							!RDAstrcmp(s->filename,s1->filename))
						{
							++occurrence;
							s1->occurrence=occurrence;
						}
					}
				}
			}
		}
	}
}
APPlib *xMaintainButton2ClearPassKey(MaintainButton *button,short fileno,
	int line,char *file)
{
	APPlib *tmp=NULL;
	char *temp=NULL,*mod=NULL,*fil=NULL;
	short x=0;
	NRDfield *field;
	ButtonField *btnfld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MaintainButton2ClearPassKey for Program [%s] Using File [%d] [%s] [%s] at line [%d] program [%s].",button->progname,fileno,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=APPlibNEW();
	mod=MODULENAME(fileno);
	fil=FILENAME(fileno);
	temp=Rmalloc(13+RDAstrlen(mod));
	sprintf(temp,"FROM_MODULE=%s",mod);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(15+RDAstrlen(fil));
	sprintf(temp,"FROM_FILENAME=%s",fil);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(9+RDAstrlen(button->keyname));
	sprintf(temp,"KEYNAME=%s",button->keyname);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	if(button->bf!=NULL)
	{
		for(x=0,btnfld=button->bf;x<button->num;++x,++btnfld)
		{
			if(!isEMPTY(btnfld->rname))
			{
				field=FLDNRD(fileno,btnfld->rname);
			} else field=NULL;
			if(field!=NULL) 
			{
				temp=Rmalloc(RDAstrlen(btnfld->ename)+3);
				sprintf(temp,"%s=",btnfld->ename);
				addAPPlib(tmp,temp);
				if(temp!=NULL) Rfree(temp);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG MaintainButton2ClearPassKey at line [%d] program [%s].",
			line,file);
		SEEAPPlib(tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(tmp);
}
