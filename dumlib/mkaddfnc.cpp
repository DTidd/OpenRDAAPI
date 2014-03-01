/* addfunc2.c - Power Add Make Functions */
#include<mkadd.hpp>

int xaddPowerAddButtonField(MaintainButton *buttons,char *rname,char *ename,int line,char *file)
{
	ButtonField *bf=NULL;

	if(buttons!=NULL)
	{
		if(buttons->bf!=NULL)
		{
			buttons->bf=Rrealloc(buttons->bf,(buttons->num+1)*sizeof(ButtonField));
		} else {
			buttons->bf=Rmalloc(sizeof(ButtonField));
			buttons->num=0;
		}
		bf=buttons->bf+buttons->num;
		if(!isEMPTY(rname))
		{
			bf->rname=stralloc(rname);
		} else bf->rname=NULL;
		if(!isEMPTY(ename))
		{
			bf->ename=stralloc(ename);
		} else bf->ename=NULL;
		return(++buttons->num);
	} else {
		return(-1);
	}
}
int xaddPowerAddButton(PowerAdd *POWERADD,char *progname,char *listname,char *buttonname,
	char *modulename,char *filename,char *keyname,char *expression,
	short type,char evalargs,APPlib *args,int numflds,ButtonField *bf,int line,char *file)
{
	MaintainButton *button=NULL;
	ButtonField *bf1=NULL,*bf2=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG xaddPowerAddButton(%s,progname=%s,listname=%s,buttonname=%s,modulename=%s,filename=%s,keyname=%s,expression=%s,type=%d,evalargs=%d,%s,numflds(bf)=%d,%s,line %d,file %s.",
			(POWERADD!=NULL?"NOT NULL POWERADD":"NULL POWERADD"),
			(progname!=NULL?progname:""),
			(listname!=NULL?listname:""),
			(buttonname!=NULL?buttonname:""),
			(modulename!=NULL?modulename:""),
			(filename!=NULL?filename:""),
			(keyname!=NULL?keyname:""),
			(expression!=NULL?expression:""),
			type,evalargs,
			(args!=NULL?"NOT NULL args":"NULL args"),
			numflds,(bf!=NULL?"NOT NULL bf":"NULL bf"),
			line,(file!=NULL?file:""));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		if(POWERADD->buttons!=NULL)

		{
			POWERADD->buttons=Rrealloc(POWERADD->buttons,(POWERADD->numbuttons+1)*sizeof(MaintainButton));
		} else {
			POWERADD->buttons=Rmalloc(sizeof(MaintainButton));
			POWERADD->numbuttons=0;
		}
		button=POWERADD->buttons+POWERADD->numbuttons;
		if(!isEMPTY(progname))
		{
			button->progname=stralloc(progname);
		} else button->progname=NULL;
		if(!isEMPTY(listname))
		{
			button->listname=stralloc(listname);
		} else button->listname=NULL;
		if(!isEMPTY(buttonname))
		{
			button->buttonname=stralloc(buttonname);
		} else button->buttonname=NULL;
		if(!isEMPTY(modulename))
		{
			button->modulename=stralloc(modulename);
		} else button->modulename=NULL;
		if(!isEMPTY(filename))
		{
			button->filename=stralloc(filename);
		} else button->filename=NULL;
		if(!isEMPTY(keyname))
		{
			button->keyname=stralloc(keyname);
		} else button->keyname=NULL;
		if(!isEMPTY(expression))
		{
			button->expression=stralloc(expression);
		} else button->expression=NULL;
		button->type=type;
		button->evalargs=evalargs;
		if(args!=NULL)
		{
			button->args=APPlibNEW();
			for(x=0;x<args->numlibs;++x)
			{
				addAPPlib(button->args,args->libs[x]);
			}
		} else button->args=NULL;
		if(bf!=NULL && numflds>0)
		{
			button->bf=Rmalloc(numflds*sizeof(ButtonField));
			for(x=0,bf1=bf,bf2=button->bf;x<numflds;++x,++bf1,++bf2)
			{
				if(!isEMPTY(bf1->rname))
				{
					bf2->rname=stralloc(bf1->rname);
				} else bf2->rname=NULL;
				if(!isEMPTY(bf1->ename))
				{
					bf2->ename=stralloc(bf1->ename);
				} else bf2->ename=NULL;
			}
			button->num=numflds;
		} else {
			button->bf=NULL;
			button->num=0;
		}
		return(++POWERADD->numbuttons);
	} else {
		return(-1);
	}
}




int xaddPowerAddEditWdgtEditFld(AddEditWdgt *EditWdgts,char *fldname,char *conname,int line,char *file)
{	
	RDAeditfld *ef=NULL;

	if(EditWdgts!=NULL)
	{
		if(EditWdgts->confld!=NULL)
		{
			EditWdgts->confld=Rrealloc(EditWdgts->confld,(EditWdgts->num+1)*sizeof(RDAeditfld));
		} else {
			EditWdgts->confld=Rmalloc(sizeof(RDAeditfld));
			EditWdgts->num=0;
		}
		ef=EditWdgts->confld+EditWdgts->num;
		if(!isEMPTY(fldname))
		{
			ef->fldname=stralloc(fldname);
		} else ef->fldname=NULL;
		if(!isEMPTY(conname))
		{
			ef->conname=stralloc(conname);
		} else ef->conname=NULL;
		return(++EditWdgts->num);
	} else {
		return(-1);
	}
}
int xaddPowerAddEditWdgts(PowerAdd *POWERADD,char *name,short type,unsigned len,
	int etype,char *expression,
	char id_mgt,char *module,char *filename,char *subkeyname,
	int numconflds,RDAeditfld *conflds,char *mgt_expression,
	char *assignname,char *id,char *override_expression,
	char setaccttype,APPlib *acctrscs,char setdeftype,APPlib *defrscs,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	RDAeditfld *confld1=NULL,*confld2=NULL;
	int x=0;

	if(POWERADD!=NULL)
	{
		if(POWERADD->EditWdgts!=NULL)
		{
			POWERADD->EditWdgts=Rrealloc(POWERADD->EditWdgts,(POWERADD->numwdgts+1)*sizeof(AddEditWdgt));
		} else {
			POWERADD->EditWdgts=Rmalloc(sizeof(AddEditWdgt));
			POWERADD->numwdgts=0;
		}
		EditWdgt=POWERADD->EditWdgts+POWERADD->numwdgts;
		if(!isEMPTY(name))
		{
			EditWdgt->name=stralloc(name);
		} else EditWdgt->name=NULL;
		EditWdgt->type=type;
		EditWdgt->len=len;
		EditWdgt->etype=etype;
		EditWdgt->loadvirtual=0;
		if(!isEMPTY(expression))
		{
			EditWdgt->expression=stralloc(expression);
		} else EditWdgt->expression=NULL;
		EditWdgt->id_mgt=id_mgt;
		if(id_mgt==TRUE)
		{
			if(!isEMPTY(module))
			{
				EditWdgt->module=stralloc(module);
			} else EditWdgt->module=NULL;
			if(!isEMPTY(filename))
			{
				EditWdgt->filename=stralloc(filename);
			} else EditWdgt->filename=NULL;
			if(!isEMPTY(subkeyname))
			{
				EditWdgt->subkeyname=stralloc(subkeyname);
			} else EditWdgt->subkeyname=NULL;
			if(conflds!=NULL && numconflds>0)
			{		
				EditWdgt->confld=Rmalloc(numconflds*sizeof(RDAeditfld));
				for(x=0,confld1=conflds,confld2=EditWdgt->confld;x<numconflds;++x,++confld1,++confld2)
				{
					if(!isEMPTY(confld1->fldname))
					{
						confld2->fldname=stralloc(confld1->fldname);
					} else confld2->fldname=NULL;
					if(!isEMPTY(confld1->conname))
					{
						confld2->conname=stralloc(confld1->conname);
					} else confld2->conname=NULL;
				}
				EditWdgt->num=numconflds;
			} else {
				EditWdgt->confld=NULL;
				EditWdgt->num=0;
			}
			if(!isEMPTY(mgt_expression))
			{
				EditWdgt->mgt_expression=stralloc(mgt_expression);
			} else EditWdgt->mgt_expression=NULL;
			if(!isEMPTY(assignname))
			{
				EditWdgt->assignname=stralloc(assignname);
			} else EditWdgt->assignname=NULL;
			if(!isEMPTY(id))
			{
				EditWdgt->id=stralloc(id);
			} else EditWdgt->id=NULL;
			if(!isEMPTY(override_expression))
			{
				EditWdgt->override_expression=stralloc(override_expression);
			} else EditWdgt->override_expression=NULL;
		} else {
			EditWdgt->module=NULL;
			EditWdgt->subkeyname=NULL;
			EditWdgt->filename=NULL;
			EditWdgt->override_expression=NULL;
			EditWdgt->id=NULL;
			EditWdgt->assignname=NULL;
			EditWdgt->mgt_expression=NULL;
			EditWdgt->confld=NULL;
			EditWdgt->num=0;
		}
		if(setaccttype==TRUE)
		{
			EditWdgt->setaccttype=TRUE;
			if(acctrscs!=NULL)
			{
				EditWdgt->acctrsc=APPlibNEW();
				for(x=0;x<acctrscs->numlibs;++x)
				{
					addAPPlib(EditWdgt->acctrsc,acctrscs->libs[x]);
				}
			} else {
				EditWdgt->acctrsc=NULL;
				EditWdgt->setaccttype=FALSE;
			}
		} else {
			EditWdgt->acctrsc=NULL;
			EditWdgt->setaccttype=FALSE;
		}
		if(setdeftype==TRUE)
		{
			EditWdgt->setdeftype=TRUE;
			if(defrscs!=NULL)
			{
				EditWdgt->defrsc=APPlibNEW();
				for(x=0;x<defrscs->numlibs;++x)
				{
					addAPPlib(EditWdgt->defrsc,defrscs->libs[x]);
				}
			} else {
				EditWdgt->defrsc=NULL;
				EditWdgt->setdeftype=FALSE;
			}
		} else {
			EditWdgt->defrsc=NULL;
			EditWdgt->setdeftype=FALSE;
		}
		return(++POWERADD->numwdgts);
	} else {
		return(-1);
	}
}
int xaddPowerAddSupportingConfld(RDAaddsub *subord,char *fldname,char *conname,char inherit,int line,char *file)
{
	RDAconfld *confld=NULL;

	if(subord!=NULL)
	{
		if(subord->confld!=NULL && subord->num>0)
		{
			subord->confld=Rrealloc(subord->confld,(subord->num+1)*sizeof(RDAaddsub));
		} else {
			subord->confld=Rmalloc(sizeof(RDAaddsub));
			subord->num=0;
		}
		confld=subord->confld+subord->num;
		if(!isEMPTY(fldname))
		{
			confld->fldname=stralloc(fldname);
		} else confld->fldname=NULL;
		if(!isEMPTY(conname))
		{
			confld->conname=stralloc(conname);
		} else confld->conname=NULL;
		confld->inherit=inherit;
		return(++subord->num);
	} else {
		return(-1);
	}
}
int xaddPowerAddSupporting(PowerAdd *POWERADD,char *module,char *filename,int dtype,
	short numconflds,RDAconfld *conflds,char *subkeyname,char *expression,
	char browse,char *button_name,char *browse_screen,
	char *define_list,char *search_browse,char smart_browse,
	int line,char *file)
{
	RDAaddsub *subord=NULL;
	RDAconfld *confld1=NULL,*confld2=NULL;
	int x=0;

	if(POWERADD!=NULL)
	{
		if(POWERADD->subord!=NULL)
		{
			POWERADD->subord=Rrealloc(POWERADD->subord,(POWERADD->subnum+1)*sizeof(RDAaddsub));
		} else {
			POWERADD->subord=Rmalloc(sizeof(RDAaddsub));
			POWERADD->subnum=0;
		}
		subord=POWERADD->subord+POWERADD->subnum;
		if(!isEMPTY(module))
		{
			subord->module=stralloc(module);
		} else subord->module=NULL;
		if(!isEMPTY(filename))
		{
			subord->filename=stralloc(filename);
		} else subord->filename=NULL;
		if(!isEMPTY(subkeyname))
		{
			subord->subkeyname=stralloc(subkeyname);
		} else subord->subkeyname=NULL;
		subord->parent=NULL;
		subord->parent_rsrc=NULL;
		subord->dtype=dtype;
		subord->cmodule=NULL;
		subord->cfilename=NULL;
		subord->BL=NULL;
		subord->browse=browse;
		if(!isEMPTY(button_name))
		{
			subord->button_name=stralloc(button_name);
		} else subord->button_name=NULL;
		if(!isEMPTY(browse_screen))
		{
			subord->browse_screen=stralloc(browse_screen);
		} else subord->browse_screen=NULL;
		if(!isEMPTY(define_list))
		{
			subord->define_list=stralloc(define_list);
		} else subord->define_list=NULL;
		if(!isEMPTY(search_browse))
		{
			subord->search_browse=stralloc(search_browse);
		} else subord->search_browse=NULL;
		subord->smart_browse=smart_browse;
		if(!isEMPTY(expression))
		{
			subord->expression=stralloc(expression);
		} else subord->expression=NULL;
		if(conflds!=NULL && numconflds>0)
		{		
			subord->confld=Rmalloc(numconflds*sizeof(RDAconfld));
			for(x=0,confld1=conflds,confld2=subord->confld;x<numconflds;++x,++confld1,++confld2)
			{
				if(!isEMPTY(confld1->fldname))
				{
					confld2->fldname=stralloc(confld1->fldname);
				} else confld2->fldname=NULL;
				if(!isEMPTY(confld1->conname))
				{
					confld2->conname=stralloc(confld1->conname);
				} else confld2->conname=NULL;
				confld2->inherit=confld1->inherit;
			}
			subord->num=numconflds;
		} else {
			subord->confld=NULL;
			subord->num=0;
		}
		return(++POWERADD->subnum);
	} else {
		return(-1);
	}
}
int xaddPowerAddWriteField(AddWrite *Write,char *to_name,int copy_type,char *from_name,int line,char *file)
{
	AddWriteField *field=NULL;

	if(Write!=NULL)
	{
		if(Write->fields!=NULL)
		{
			Write->fields=Rrealloc(Write->fields,(Write->numflds+1)*sizeof(AddWriteField));
		} else {
			Write->fields=Rmalloc(sizeof(AddWriteField));
			Write->numflds=0;
		}
		field=Write->fields+Write->numflds;
		if(!isEMPTY(to_name))
		{
			field->to_name=stralloc(to_name);
		} else field->to_name=NULL;
		field->copy_type=copy_type;
		if(!isEMPTY(from_name))
		{
			field->from_name=stralloc(from_name);
		} else field->from_name=NULL;
		return(++Write->numflds);
	} else {
		return(-1);
	}
}
int xaddPowerAddWrite(PowerAdd *POWERADD,char *module,char *filename,char *expression,
		int numflds,AddWriteField *fields,int line,char *file)
{
	AddWrite *Writes=NULL;
	AddWriteField *field1=NULL,*field2=NULL;
	int x=0; 

	if(POWERADD!=NULL)
	{
		if(POWERADD->Writes!=NULL)
		{
			POWERADD->Writes=Rrealloc(POWERADD->Writes,(POWERADD->numWrites+1)*sizeof(AddWrite));
		} else {
			POWERADD->Writes=Rmalloc(sizeof(AddWrite));
			POWERADD->numWrites=0;
		}
		Writes=POWERADD->Writes+POWERADD->numWrites;
		if(!isEMPTY(module))
		{
			Writes->module=stralloc(module);
		} else Writes->module=NULL;
		if(!isEMPTY(filename))
		{
			Writes->filename=stralloc(filename);
		} else Writes->filename=NULL;
		if(!isEMPTY(expression))
		{
			Writes->expression=stralloc(expression);
		} else Writes->expression=NULL;
		if(fields!=NULL && numflds>0)
		{		
			Writes->fields=Rmalloc(numflds*sizeof(AddWriteField));
			for(x=0,field1=fields,field2=Writes->fields;x<numflds;++x,++field1,++field2)
			{
				if(!isEMPTY(field1->to_name))
				{
					field2->to_name=stralloc(field1->to_name);
				} else field2->to_name=NULL;
				field2->copy_type=field1->copy_type;
				if(!isEMPTY(field1->from_name))
				{
					field2->from_name=stralloc(field1->from_name);
				} else field2->from_name=NULL;
			}
			Writes->numflds=numflds;
		} else {
			Writes->fields=NULL;
			Writes->numflds=0;
		}
		return(++POWERADD->numWrites);
	} else {
		return(-1);
	}
}

PowerAdd *xRDAPowerAddNEW(char *module,char *name,char *desc,
	char *addscreen,char *define_list,
	char *reportname,int save_type,
	char *save_error_exp,char *save_error_desc,
	char *save_warning_exp,char *save_warning_desc,
	char *quit_error_exp,char *quit_error_desc,
	char *quit_warning_exp,char *quit_warning_desc,
	int numbuttons,MaintainButton *buttons,
	int numsupports,RDAaddsub *supports,
	int numwrites,AddWrite *writes,
	int numwdgts,AddEditWdgt *EditWdgts,int line,char *file)

{
	PowerAdd *POWERADD=NULL;
	MaintainButton *button=NULL;
	RDAaddsub *support=NULL;
	AddWrite *write=NULL;
	AddEditWdgt *EditWdgt=NULL;
	int x=0;

	POWERADD=PowerAddNew(module,name);
	if(POWERADD!=NULL)
	{
		if(!isEMPTY(desc))
		{
			POWERADD->desc=stralloc(desc);
		} else POWERADD->desc=NULL;
		if(!isEMPTY(addscreen))
		{
			POWERADD->addscreen=stralloc(addscreen);
		} else POWERADD->addscreen=NULL;
		if(!isEMPTY(define_list))
		{
			POWERADD->define_list=stralloc(define_list);
		} else POWERADD->define_list=NULL;
		if(!isEMPTY(reportname))
		{
			POWERADD->reportname=stralloc(reportname);
		} else POWERADD->reportname=NULL;
		POWERADD->save_type=save_type;
/*
		if(!isEMPTY(save_error_exp))
		{
			POWERADD->save_error=stralloc(save_error_exp);
		} else POWERADD->save_error=NULL;
*/
		if(!isEMPTY(save_error_exp))
		{
			POWERADD->save_expression=stralloc(save_error_exp);
		} else POWERADD->save_expression=NULL;
		if(!isEMPTY(save_error_desc))
		{
			POWERADD->save_error_desc=stralloc(save_error_desc);
		} else POWERADD->save_error_desc=NULL;
		if(!isEMPTY(save_warning_exp))
		{
			POWERADD->save_warning=stralloc(save_warning_exp);
		} else POWERADD->save_warning=NULL;
		if(!isEMPTY(save_warning_desc))
		{
			POWERADD->save_warning_desc=stralloc(save_warning_desc);
		} else POWERADD->save_warning_desc=NULL;
		if(!isEMPTY(quit_error_exp))
		{
			POWERADD->quit_error=stralloc(quit_error_exp);
		} else POWERADD->quit_error=NULL;
		if(!isEMPTY(quit_error_desc))
		{
			POWERADD->quit_error_desc=stralloc(quit_error_desc);
		} else POWERADD->quit_error_desc=NULL;
		if(!isEMPTY(quit_warning_exp))
		{
			POWERADD->quit_warning=stralloc(quit_warning_exp);
		} else POWERADD->quit_warning=NULL;
		if(!isEMPTY(quit_warning_desc))
		{
			POWERADD->quit_warning_desc=stralloc(quit_warning_desc);
		} else POWERADD->quit_warning_desc=NULL;
		if(buttons!=NULL && numbuttons>0)
		{
			for(x=0,button=buttons;x<numbuttons;++x,++buttons)
			{
				addPowerAddButton(POWERADD,button->progname,button->listname,button->buttonname,
					button->modulename,button->filename,button->keyname,button->expression,
					button->type,button->evalargs,button->args,button->num,button->bf);
			}
/*
			POWERADD->numbuttons=numbuttons;
*/
		} else {
			POWERADD->buttons=NULL;
			POWERADD->numbuttons=0;
		}
		if(supports!=NULL && numsupports>0)
		{
			for(x=0,support=supports;x<numsupports;++x,++support)
			{
				addPowerAddSupportingADV(POWERADD,support->module,support->filename,support->dtype,
					support->num,support->confld,support->subkeyname,support->expression,
					support->browse,support->button_name,support->browse_screen,
					support->define_list,support->search_browse,support->smart_browse);
			}
		} else {
			POWERADD->subord=NULL;
			POWERADD->subnum=0;
		}
		if(writes!=NULL && numwrites>0)
		{
			for(x=0,write=writes;x<numwrites;++x,++write)
			{
				addPowerAddWrite(POWERADD,write->module,write->filename,write->expression,
					write->numflds,write->fields);
			}
		} else {
			POWERADD->Writes=NULL;
			POWERADD->numWrites=0;
		}
		if(EditWdgts!=NULL && numwdgts>0)
		{
			for(x=0,EditWdgt=EditWdgts;x<numwdgts;++x,++EditWdgt)
			{
				addPowerAddEditWdgts(POWERADD,EditWdgt->name,EditWdgt->type,EditWdgt->len,
					EditWdgt->etype,EditWdgt->expression,EditWdgt->id_mgt,EditWdgt->module,
					EditWdgt->filename,EditWdgt->subkeyname,EditWdgt->num,EditWdgt->confld,
					EditWdgt->mgt_expression,EditWdgt->assignname,EditWdgt->id,
					EditWdgt->override_expression,EditWdgt->setaccttype,EditWdgt->acctrsc,
					EditWdgt->setdeftype,EditWdgt->defrsc);
			}
		} else {
			POWERADD->EditWdgts=NULL;
			POWERADD->numwdgts=0;
		}
		return(POWERADD);
	} else {
		return(NULL);
	}
}
