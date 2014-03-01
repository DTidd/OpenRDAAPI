/* addfunc2.c - Power Add Make Functions */
#include<mkadd.hpp>
#include<mkmsc.hpp>

int xaddMaintainMasterButtonField(MaintainButton *buttons,char *rname,char *ename,int line,char *file)
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
int xaddMaintainMasterButton(MaintainMaster *MTNMSTR,char *progname,
	char *listname,char *buttonname,char *modulename,char *filename,
	char *keyname,char *expression,short type,char evalargs,APPlib *args,
	int numflds,ButtonField *bf,int line,char *file)
{
	MaintainButton *button=NULL;
	ButtonField *bf1=NULL,*bf2=NULL;
	int x=0;

	if(MTNMSTR!=NULL)
	{
		if(MTNMSTR->buttons!=NULL)
		{
			MTNMSTR->buttons=Rrealloc(MTNMSTR->buttons,(MTNMSTR->numbuttons+1)*sizeof(MaintainButton));
		} else {
			MTNMSTR->buttons=Rmalloc(sizeof(MaintainButton));
			MTNMSTR->numbuttons=0;
		}
		button=MTNMSTR->buttons+MTNMSTR->numbuttons;
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
		return(++MTNMSTR->numbuttons);
	} else {
		return(-1);
	}
}
int xaddActionStruct(MTNAction *A,char *to_name,char *expression,int typelist,
	int line,char *file)
{
	Actionstruct *a=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addActionstruct at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(A->a!=NULL)
	{
		A->a=Rrealloc(A->a,(A->num+1)*sizeof(Actionstruct));
	} else {
		A->a=Rmalloc(sizeof(Actionstruct));
		A->num=0;
	}
	a=A->a+A->num;
	a->typelist=typelist;
	a->to_name=stralloc(to_name);
	a->expression=stralloc(expression);
	return(++A->num);
}
int xaddMaintainMasterAction(MaintainMaster *MTNMSTR,char *name,int line,char *file)
{
	MTNAction *a=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addMaintainMasterAction at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(MTNMSTR->Actions!=NULL)
	{
		MTNMSTR->Actions=Rrealloc(MTNMSTR->Actions,(MTNMSTR->numActions+1)*sizeof(MTNAction));
	} else {
		MTNMSTR->Actions=Rmalloc(sizeof(MTNAction));
		MTNMSTR->numActions=0;
	}
	a=MTNMSTR->Actions+MTNMSTR->numActions;
	a->name=stralloc(name);
	a->a=NULL;
	a->num=0;
	return(++MTNMSTR->numActions);
}
int xaddSubordinateWrite(RDAsubord *sub,char *module,char *filename,
	char *expression,int line,char *file)
{
	PowerWrite *W=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addSubordinateWrite at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(sub->Writes!=NULL)
	{
		sub->Writes=Rrealloc(sub->Writes,
			(sub->numWrites+1)*sizeof(PowerWrite));
	} else {
		sub->Writes=Rmalloc(sizeof(PowerWrite));
		sub->numWrites=0;
	}
	W=sub->Writes+sub->numWrites;
	W->module=stralloc(module);
	W->filename=stralloc(filename);
	W->expression=stralloc(expression);
	W->numflds=0;
	W->fields=NULL;
	W->closefile=FALSE;
	return(++sub->numWrites);
}
short xaddSubordinateRDAconfld(RDAsubord *sub,char *fldname,char *conname,
	char inherit,int line,char *file)
{
	RDAconfld *c=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addSubordinateRDAconfld at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(sub->confld!=NULL)
	{
		sub->confld=Rrealloc(sub->confld,(sub->num+1)*sizeof(RDAconfld));
	} else {
		sub->confld=Rmalloc(sizeof(RDAconfld));
		sub->num=0;
	}
	c=sub->confld+sub->num;
	c->fldname=stralloc(fldname);
	c->conname=stralloc(conname);
	c->inherit=inherit;
	return(++sub->num);
}
int xaddMaintainMasterSubordinate(MaintainMaster *MTNMSTR,char *module,
	char *filename,int dtype,char *cmodule,char *cfilename,
	char *subkeyname,char *expression,char UsePowerWrite,
	int line,char *file)
{
	RDAsubord *sub=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addMaintainMasterSubordinate at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(MTNMSTR->subord!=NULL)
	{
		MTNMSTR->subord=Rrealloc(MTNMSTR->subord,(MTNMSTR->subnum+1)*sizeof(RDAsubord));
	} else {
		MTNMSTR->subord=Rmalloc(sizeof(RDAsubord));
		MTNMSTR->subnum=0;
	}
	sub=MTNMSTR->subord+MTNMSTR->subnum;
	sub->module=stralloc(module);
	sub->filename=stralloc(filename);
	sub->dtype=dtype;
	sub->cmodule=stralloc(cmodule);
	sub->cfilename=stralloc(cfilename);
	sub->subkeyname=stralloc(subkeyname);
	sub->expression=stralloc(expression);
	sub->UsePowerWrite=UsePowerWrite;
	sub->numWrites=0;
	sub->Writes=NULL;
	sub->fileno=0;
	sub->keyno=0;
	sub->occurrence=0;
	sub->writable=0;
	sub->num=0;
	sub->confld=NULL;
	return(++MTNMSTR->subnum);
}

short xaddSupportingRDAconfld(RDAsearch *sub,char *fldname,char *conname,
	char inherit,int line,char *file)
{
	RDAconfld *c=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addSubordinateRDAconfld at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(sub->confld!=NULL)
	{
		sub->confld=Rrealloc(sub->confld,(sub->num+1)*sizeof(RDAconfld));
	} else {
		sub->confld=Rmalloc(sizeof(RDAconfld));
		sub->num=0;
	}
	c=sub->confld+sub->num;
	c->fldname=stralloc(fldname);
	c->conname=stralloc(conname);
	c->inherit=inherit;
	return(++sub->num);
}
int xaddMaintainMasterSupporting(MaintainMaster *MTNMSTR,char *module,
	char *filename,int ctype,short numconflds,RDAconfld *conflds,
	char *subkeyname,char *expression,char browse,char *button_name,
	char *browse_screen,char *define_list,char *search_browse,
	char smart_browse,char *cmodule,char *cfilename,int line,char *file)
{
	RDAsearch *subord=NULL;
	RDAconfld *confld1=NULL,*confld2=NULL;
	int x=0;

	if(MTNMSTR!=NULL)
	{
		if(MTNMSTR->s!=NULL)
		{
			MTNMSTR->s=Rrealloc(MTNMSTR->s,(MTNMSTR->num+1)*sizeof(RDAsearch));
		} else {
			MTNMSTR->s=Rmalloc(sizeof(RDAsearch));
			MTNMSTR->num=0;
		}
		subord=MTNMSTR->s+MTNMSTR->num;
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
		subord->ctype=ctype;
		subord->range=FALSE;
		subord->num_writes=0;
		subord->fileno=(-1);
		subord->keyno=(-1);
		subord->occurrence=(-1);
		subord->writable=FALSE;
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
		subord->cmodule=stralloc(cmodule);
		subord->cfilename=stralloc(cfilename);
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
		return(++MTNMSTR->num);
	} else {
		return(-1);
	}
}
int xaddMaintainMasterWriteField(PowerWrite *Write,char *to_name,int copy_type,char *from_name,int line,char *file)
{
	PowerWriteField *field=NULL;

	if(Write!=NULL)
	{
		if(Write->fields!=NULL)
		{
			Write->fields=Rrealloc(Write->fields,(Write->numflds+1)*sizeof(PowerWriteField));
		} else {
			Write->fields=Rmalloc(sizeof(PowerWriteField));
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
int xaddMaintainMasterWrite(short which,MaintainMaster *MTNMSTR,char *module,
		char *filename,char *expression,
		int numflds,PowerWriteField *fields,int line,char *file)
{
	PowerWrite *Writes=NULL;
	PowerWriteField *field1=NULL,*field2=NULL;
	int x=0; 

	if(MTNMSTR!=NULL)
	{
		if(which==FALSE)
		{
			if(MTNMSTR->PrevWrites!=NULL)
			{
				MTNMSTR->PrevWrites=Rrealloc(MTNMSTR->PrevWrites,(MTNMSTR->numPrevWrites+1)*sizeof(PowerWrite));
			} else {
				MTNMSTR->PrevWrites=Rmalloc(sizeof(PowerWrite));
				MTNMSTR->numPrevWrites=0;
			}
			Writes=MTNMSTR->PrevWrites+MTNMSTR->numPrevWrites;
		} else {
			if(MTNMSTR->NewWrites!=NULL)
			{
				MTNMSTR->NewWrites=Rrealloc(MTNMSTR->NewWrites,(MTNMSTR->numNewWrites+1)*sizeof(PowerWrite));
			} else {
				MTNMSTR->NewWrites=Rmalloc(sizeof(PowerWrite));
				MTNMSTR->numNewWrites=0;
			}
			Writes=MTNMSTR->NewWrites+MTNMSTR->numNewWrites;
		}
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
			Writes->fields=Rmalloc(numflds*sizeof(PowerWriteField));
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
		if(which==FALSE)
		{
			return(++MTNMSTR->numPrevWrites);
		} else return(++MTNMSTR->numNewWrites);
	} else {
		return(-1);
	}
}

MaintainMaster *xRDAMaintainMasterNEW(char *module,char *name,char *desc,
	int style,char *mainfile,char *browse_screen,char *define_list,
	char *search_browse,char *maintain_screen,int file_type,int start_with,
	int start_where,int save_type,char *save_expression,char *save_error,
	char *save_warning,char *save_error_desc,char *save_warning_desc,
	short inheritrsrcs,char UsePowerWrite,char Use_Audit_Trail,
	char *Audit_Trail,int line,char *file)
{
	MaintainMaster *MTNMSTR=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG RDAMaintainMasterNew at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	MTNMSTR=MaintainMasterNew(module,name);
	if(MTNMSTR!=NULL)
	{
		MTNMSTR->desc=stralloc(desc);
		MTNMSTR->style=style;
		MTNMSTR->mainfile=stralloc(mainfile);
		MTNMSTR->browse_screen=stralloc(browse_screen);
		MTNMSTR->define_list=stralloc(define_list);
		MTNMSTR->search_browse=stralloc(search_browse);
		MTNMSTR->maintain_screen=stralloc(maintain_screen);
		MTNMSTR->file_type=file_type;
		MTNMSTR->start_with=start_with;
		MTNMSTR->start_where=start_where;
		MTNMSTR->save_type=save_type;
		MTNMSTR->save_expression=stralloc(save_expression);
		MTNMSTR->save_error=stralloc(save_error);
		MTNMSTR->save_error_desc=stralloc(save_error_desc);
		MTNMSTR->save_warning=stralloc(save_warning);
		MTNMSTR->save_warning_desc=stralloc(save_warning_desc);
		MTNMSTR->inheritrsrcs=inheritrsrcs;
		MTNMSTR->UsePowerWrite=UsePowerWrite;
		MTNMSTR->Use_Audit_Trail=Use_Audit_Trail;
		MTNMSTR->Audit_Trail=stralloc(Audit_Trail);
		MTNMSTR->num=0;
		MTNMSTR->s=NULL;
		MTNMSTR->subnum=0;
		MTNMSTR->subord=NULL;
		MTNMSTR->passkey=NULL;
		MTNMSTR->args=NULL;
		MTNMSTR->numlists=0;
		MTNMSTR->lists=NULL;
		MTNMSTR->buttons=NULL;
		MTNMSTR->numbuttons=0;
		MTNMSTR->runfunc=NULL;
		MTNMSTR->numPrevWrites=0;
		MTNMSTR->PrevWrites=NULL;
		MTNMSTR->numNewWrites=0;
		MTNMSTR->NewWrites=NULL;
		MTNMSTR->mbl_utlmstr=NULL;
		MTNMSTR->editable=FALSE;
		MTNMSTR->level=0;
		MTNMSTR->WindowCount=0;
		MTNMSTR->SubFunc=NULL;
		MTNMSTR->SubFuncArgs=NULL;
		MTNMSTR->numActions=0;
		MTNMSTR->Actions=NULL;
	}
	return(MTNMSTR);
}
/* mkmtncd.c - Generate MaintainMaster C Code for speed of development*/
void makemtncode(MaintainMaster *MTNMASTER,FILE *fp,char *mtnname)
{
	int x=0,y=0,z=0,q=0;
	int overall_length=0;
	MaintainButton *button=NULL;
	ButtonField *bf=NULL;
	RDAsearch *s=NULL;
	RDAsubord *subord=NULL;
	MTNAction *action=NULL;
	Actionstruct *as=NULL;
	RDAconfld *confld=NULL;
	PowerWrite *wrte=NULL;
	PowerWriteField *wf=NULL;
	APPlib *tempapplib=NULL;
	char *thing1="/*",*thing2="*/";
	short sb=0;

	if(fp!=NULL)
	{	
		overall_length=0;
		tempapplib=break_expression2(MTNMASTER->desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp1,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		

		overall_length=0;
		tempapplib=break_expression2(MTNMASTER->save_expression,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp2=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp2,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		overall_length=0;
		tempapplib=break_expression2(MTNMASTER->save_error,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp3=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp3,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		overall_length=0;
		tempapplib=break_expression2(MTNMASTER->save_error_desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp4=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp4,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		overall_length=0;
		tempapplib=break_expression2(MTNMASTER->save_warning,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp5=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp5,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}

		overall_length=0;
		tempapplib=break_expression2(MTNMASTER->save_warning_desc,&overall_length);
		if(tempapplib!=NULL)
		{
			fprintf(fp,"\ttemp6=Rmalloc(%d+1);\n",overall_length);
			fprintf(fp,"\tsprintf(temp6,\"");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				fprintf(fp,"%%s");
			}
			fprintf(fp,"\",\n");
			for(q=0;q<tempapplib->numlibs;++q)
			{
				if(q!=(tempapplib->numlibs-1))
				{
					fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
				} else {
					fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
				}
			}
			freeapplib(tempapplib);
		}
		if(!isEMPTY(MTNMASTER->browse_screen)) stoupper(MTNMASTER->browse_screen);
		if(!isEMPTY(MTNMASTER->define_list)) stoupper(MTNMASTER->define_list);
		if(!isEMPTY(MTNMASTER->search_browse)) stoupper(MTNMASTER->search_browse);
		if(!isEMPTY(MTNMASTER->maintain_screen)) stoupper(MTNMASTER->maintain_screen);
		fprintf(fp,"\n");
		fprintf(fp,"\tMTNMSTR=RDAMaintainMasterNEW(module,\"%s\",temp1,%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,%d,%d,%d,temp2,temp3,temp5,temp4,temp6,%s,%s,%s,\"%s\");\n",
			mtnname,
			MTNMASTER->style,
			(MTNMASTER->mainfile!=NULL?MTNMASTER->mainfile:""),
			(MTNMASTER->browse_screen!=NULL?MTNMASTER->browse_screen:""),
			(MTNMASTER->define_list!=NULL?MTNMASTER->define_list:""),
			(MTNMASTER->search_browse!=NULL?MTNMASTER->search_browse:""),
			(MTNMASTER->maintain_screen!=NULL?MTNMASTER->maintain_screen:""),
			MTNMASTER->file_type,
			MTNMASTER->start_with,
			MTNMASTER->start_where,
			MTNMASTER->save_type,
			(MTNMASTER->inheritrsrcs==TRUE?"TRUE":"FALSE"),
			(MTNMASTER->UsePowerWrite==TRUE?"TRUE":"FALSE"),
			(MTNMASTER->Use_Audit_Trail==TRUE?"TRUE":"FALSE"),
			(MTNMASTER->Audit_Trail!=NULL ? MTNMASTER->Audit_Trail:""));
		fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\tif(temp2!=NULL) Rfree(temp2);\n");
		fprintf(fp,"\tif(temp3!=NULL) Rfree(temp3);\n");
		fprintf(fp,"\tif(temp4!=NULL) Rfree(temp4);\n");
		fprintf(fp,"\tif(temp5!=NULL) Rfree(temp5);\n");
		fprintf(fp,"\tif(temp6!=NULL) Rfree(temp6);\n");
		fprintf(fp,"\tif(MTNMSTR!=NULL)\n");
		fprintf(fp,"\t{\n");

/* RDAsearch Supporting Files Section */
		if(MTNMASTER->s!=NULL)
		{
			fprintf(fp,"%s SUPPORTING FILES Beginning %s\n\n",thing1,thing2);
			for(x=0,s=MTNMASTER->s;x<MTNMASTER->num;++x,++s)
			{
				fprintf(fp,"%s %s %s %s\n\n",thing1,(s->module!=NULL ? s->module:""),
					(s->filename!=NULL ? s->filename:""),thing2);
				overall_length=0;
				tempapplib=break_expression2(s->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				if(!isEMPTY(s->browse_screen)) stoupper(s->browse_screen);
				if(!isEMPTY(s->define_list)) stoupper(s->define_list);
				if(!isEMPTY(s->search_browse)) stoupper(s->search_browse);

				sb=s->smart_browse;
				fprintf(fp,"\t\tr=addMaintainMasterSupportingADV(MTNMSTR,\"%s\",\"%s\",%d,0,NULL,\"%s\",temp1,%s,\"%s\",\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\");\n",
					(s->module!=NULL ? s->module:""),
					(s->filename!=NULL ? s->filename:""),
					s->ctype,
					(s->subkeyname!=NULL ? s->subkeyname:""),
					(s->browse==TRUE ? "TRUE":"FALSE"),
					(s->button_name!=NULL ? s->button_name:""),
					(s->browse_screen!=NULL ? s->browse_screen:""),
					(s->define_list!=NULL ? s->define_list:""),
					(s->search_browse!=NULL ? s->search_browse:""),
					sb,(s->cmodule!=NULL ? s->cmodule:""),
					(s->cfilename!=NULL ? s->cfilename:""));
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\t\tif(r>0)\n");
				fprintf(fp,"\t\t{\n");
				if(s->confld!=NULL && s->ctype!=2)
				{
					fprintf(fp,"\t\t\tS=MTNMSTR->s+(r-1);\n");
					for(y=0,confld=s->confld;y<s->num;++y,++confld)
					{
						fprintf(fp,"\t\t\taddSupportingRDAconfld(S,\"%s\",\"%s\",%s);\n",(confld->fldname!=NULL ? confld->fldname:""),
							(confld->conname!=NULL ? confld->conname:""),
							(confld->inherit==TRUE ? "TRUE":"FALSE"));
					}
				}
				fprintf(fp,"\t\t}\n");
				fprintf(fp,"\n\n");
			}
			fprintf(fp,"%s SUPPORTING FILES Ending %s\n\n\n",thing1,thing2);
			fprintf(fp,"\n");
			fprintf(fp,"\n");
		}
		fprintf(fp,"\t\tSetRDAsearchOccurrence(MTNMSTR);\n");
		if(MTNMASTER->subord!=NULL)
		{
			fprintf(fp,"\n\n\n%s SUBORDINATE FILES Beginning %s\n",thing1,thing2);
			for(x=0,subord=MTNMASTER->subord;x<MTNMASTER->subnum;++x,++subord)
			{
				fprintf(fp,"%s %s %s %s\n\n",thing1,(subord->module!=NULL ? subord->module:""),
					(subord->filename!=NULL ? subord->filename:""),thing2);
				overall_length=0;
				tempapplib=break_expression2(subord->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

				fprintf(fp,"\t\tr=addMaintainMasterSubordinate(MTNMSTR,\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\",temp1,%s);\n",
					(subord->module!=NULL ? subord->module:""),
					(subord->filename!=NULL ? subord->filename:""),
					subord->dtype,
					(subord->cmodule!=NULL ? subord->cmodule:""),
					(subord->cfilename!=NULL ? subord->cfilename:""),
					(subord->subkeyname!=NULL ? subord->subkeyname:""),
					(subord->UsePowerWrite==TRUE ? "TRUE":"FALSE"));
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\t\tif(r>0)\n");
				fprintf(fp,"\t\t{\n");
				if(subord->confld!=NULL)
				{
					fprintf(fp,"\t\t\tSUBORD=MTNMSTR->subord+(r-1);\n");
					for(y=0,confld=subord->confld;y<subord->num;++y,++confld)
					{
						fprintf(fp,"\t\t\taddSubordinateRDAconfld(SUBORD,\"%s\",\"%s\",%s);\n",(confld->fldname!=NULL ? confld->fldname:""),
							(confld->conname!=NULL ? confld->conname:""),
							(confld->inherit==TRUE ? "TRUE":"FALSE"));
					}
				}
				if(subord->UsePowerWrite==TRUE)
				{
					for(y=0,wrte=subord->Writes;y<subord->numWrites;++y,++wrte)
					{
						overall_length=0;
						tempapplib=break_expression2(wrte->expression,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}

						fprintf(fp,"\t\t\tx=addSubordinateWrite(SUBORD,\"%s\",\"%s\",temp1);\n",
							(wrte->module!=NULL ? wrte->module:""),
							(wrte->filename!=NULL ? wrte->filename:""));
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
						fprintf(fp,"\t\t\tif(x>0)\n");
						fprintf(fp,"\t\t\t{\n");	
						if(wrte->fields!=NULL)
						{
							fprintf(fp,"\t\t\t\tWRTE=SUBORD->Writes+(x-1);\n");
							for(z=0,wf=wrte->fields;z<wrte->numflds;++z,++wf)
							{
								overall_length=0;
								tempapplib=break_expression2(wf->to_name,&overall_length);
								if(tempapplib!=NULL)
								{
									fprintf(fp,"\t\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
									fprintf(fp,"\t\t\t\tsprintf(temp1,\"");
									for(q=0;q<tempapplib->numlibs;++q)
									{
										fprintf(fp,"%%s");
									}
									fprintf(fp,"\",\n");
									for(q=0;q<tempapplib->numlibs;++q)
									{
										if(q!=(tempapplib->numlibs-1))
										{
											fprintf(fp,"\t\t\t\t\t%s,\n",tempapplib->libs[q]);
										} else {
											fprintf(fp,"\t\t\t\t\t%s);\n",tempapplib->libs[q]);
										}
									}
									freeapplib(tempapplib);
								}
								overall_length=0;
								tempapplib=break_expression2(wf->from_name,&overall_length);
								if(tempapplib!=NULL)
								{
									fprintf(fp,"\t\t\t\ttemp2=Rmalloc(%d+1);\n",overall_length);
									fprintf(fp,"\t\t\t\tsprintf(temp2,\"");
									for(q=0;q<tempapplib->numlibs;++q)
									{
										fprintf(fp,"%%s");
									}
									fprintf(fp,"\",\n");
									for(q=0;q<tempapplib->numlibs;++q)
									{
										if(q!=(tempapplib->numlibs-1))
										{
											fprintf(fp,"\t\t\t\t\t%s,\n",tempapplib->libs[q]);
										} else {
											fprintf(fp,"\t\t\t\t\t%s);\n",tempapplib->libs[q]);
										}
									}
									freeapplib(tempapplib);
								}
								fprintf(fp,"\t\t\t\taddMaintainMasterWriteField(WRTE,temp1,%d,temp2);\n",
									wf->copy_type);
								fprintf(fp,"\t\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
								fprintf(fp,"\t\t\t\tif(temp2!=NULL) Rfree(temp2);\n");
							}
							fprintf(fp,"\t\t\t}\n");
						}
					}
				}
				fprintf(fp,"\t\t}\n");
				fprintf(fp,"\n\n");
				fprintf(fp,"\n");
			}
			fprintf(fp,"%s SUBORDINATE FILES Ending %s\n\n\n",thing1,thing2);
			fprintf(fp,"\n");
			fprintf(fp,"\n");
		}
		if(MTNMASTER->PrevWrites!=NULL)
		{
			fprintf(fp,"%s PREVIOUS WRITES Beginning %s\n\n\n",thing1,thing2);
			for(x=0,wrte=MTNMASTER->PrevWrites;x<MTNMASTER->numPrevWrites;++x,++wrte)
			{
				fprintf(fp,"%s %s %s Previous Write %s\n",thing1,(wrte->module!=NULL ? wrte->module:""),(wrte->filename!=NULL ? wrte->filename:""),thing2); 
				overall_length=0;
				tempapplib=break_expression2(wrte->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				fprintf(fp,"\t\tx=addMaintainMasterWrite(FALSE,MTNMSTR,\"%s\",\"%s\",temp1,0,NULL);\n",
					(wrte->module!=NULL?wrte->module:""),
					(wrte->filename!=NULL?wrte->filename:""));
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\t\tif(x>0)\n");
				fprintf(fp,"\t\t{\n");
				if(wrte->fields!=NULL)
				{
					fprintf(fp,"\t\t\tWRTE=MTNMSTR->PrevWrites+(x-1);\n");
					for(z=0,wf=wrte->fields;z<wrte->numflds;++z,++wf)
					{
						overall_length=0;
						tempapplib=break_expression2(wf->to_name,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}
						overall_length=0;
						tempapplib=break_expression2(wf->from_name,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp2=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp2,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}
						fprintf(fp,"\t\t\taddMaintainMasterWriteField(WRTE,temp1,%d,temp2);\n",
							wf->copy_type);
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
						fprintf(fp,"\t\t\tif(temp2!=NULL) Rfree(temp2);\n");
					}
				}
				fprintf(fp,"\t\t}\n\n");
			}
			fprintf(fp,"%s PREVIOUS WRITES Ending %s\n\n\n",thing1,thing2);
			fprintf(fp,"\n");
		}
		if(MTNMASTER->NewWrites!=NULL)
		{
			fprintf(fp,"%s NEW WRITES Beginning %s\n\n\n",thing1,thing2);
			for(x=0,wrte=MTNMASTER->NewWrites;x<MTNMASTER->numNewWrites;++x,++wrte)
			{
				fprintf(fp,"%s %s %s New Write %s\n",thing1,(wrte->module!=NULL ? wrte->module:""),(wrte->filename!=NULL ? wrte->filename:""),thing2); 
				overall_length=0;
				tempapplib=break_expression2(wrte->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				fprintf(fp,"\t\tx=addMaintainMasterWrite(TRUE,MTNMSTR,\"%s\",\"%s\",temp1,0,NULL);\n",
					(wrte->module!=NULL ? wrte->module:""),
					(wrte->filename!=NULL ? wrte->filename:""));
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\t\tif(x>0)\n");
				fprintf(fp,"\t\t{\n");
				if(wrte->fields!=NULL)
				{
					fprintf(fp,"\t\t\tWRTE=MTNMSTR->NewWrites+(x-1);\n");
					for(z=0,wf=wrte->fields;z<wrte->numflds;++z,++wf)
					{
						overall_length=0;
						tempapplib=break_expression2(wf->to_name,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}
						overall_length=0;
						tempapplib=break_expression2(wf->from_name,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp2=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp2,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}
						fprintf(fp,"\t\t\taddMaintainMasterWriteField(WRTE,temp1,%d,temp2);\n",
							wf->copy_type);
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
						fprintf(fp,"\t\t\tif(temp2!=NULL) Rfree(temp2);\n");
					}
				}
				fprintf(fp,"\t\t}\n\n");
			}
			fprintf(fp,"\n");
			fprintf(fp,"%s NEW WRITES Ending %s\n\n\n",thing1,thing2);
		}
		if(MTNMASTER->Actions!=NULL)
		{
			fprintf(fp,"%s ACTIONS Beginning %s\n\n\n",thing1,thing2);
			for(x=0,action=MTNMASTER->Actions;x<MTNMASTER->numActions;++x,++action)
			{
				fprintf(fp,"\t\tr=addMaintainMasterAction(MTNMSTR,\"%s\");\n",(action->name!=NULL ? action->name:""));
				fprintf(fp,"\t\tif(r>0)\n");
				fprintf(fp,"\t\t{\n");
				if(action->a!=NULL)
				{
					fprintf(fp,"\t\t\tACTION=MTNMSTR->Actions+(r-1);\n");
					for(y=0,as=action->a;y<action->num;++y,++as)
					{
						overall_length=0;
						tempapplib=break_expression2(as->expression,&overall_length);
						if(tempapplib!=NULL)
						{
							fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
							fprintf(fp,"\t\t\tsprintf(temp1,\"");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								fprintf(fp,"%%s");
							}
							fprintf(fp,"\",\n");
							for(q=0;q<tempapplib->numlibs;++q)
							{
								if(q!=(tempapplib->numlibs-1))
								{
									fprintf(fp,"\t\t\t\t%s,\n",tempapplib->libs[q]);
								} else {
									fprintf(fp,"\t\t\t\t%s);\n",tempapplib->libs[q]);
								}
							}
							freeapplib(tempapplib);
						}
						fprintf(fp,"\t\t\taddActionStruct(ACTION,\"%s\",temp1,%d);\n",
							(as->to_name!=NULL ? as->to_name:""),
							as->typelist);
						fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
					}
				}
				fprintf(fp,"\t\t}\n\n");
			}
			fprintf(fp,"%s ACTIONS Ending %s\n\n\n",thing1,thing2);
		}
		if(MTNMASTER->buttons!=NULL)
		{
		fprintf(fp,"%s BUTTONS Beginning %s\n\n\n",thing1,thing2);
		for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
		{	
			fprintf(fp,"%s %s %s BUTTON %s\n",thing1,(button->buttonname!=NULL ? button->buttonname:""),
				(button->progname!=NULL ? button->progname:""),thing2);
			if(button!=NULL)
			{
				overall_length=0;
				tempapplib=break_expression2(button->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\t\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\t\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}
				if(button->args!=NULL)
				{
					fprintf(fp,"\t\t\n");
					fprintf(fp,"\t\targs=APPlibNEW();\n");
					for(y=0;y<button->args->numlibs;++y)
					{
						fprintf(fp,"\t\taddAPPlib(args,\"%s\");\n",
							button->args->libs[y]);
					}
				}
				fprintf(fp,"\t\tx=addMaintainMasterButton(MTNMSTR,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",temp1,%d,%d,%s,0,NULL);\n",
					(button->progname!=NULL ? button->progname : ""),
					(button->listname!=NULL ? button->listname: ""),
					(button->buttonname!=NULL ? button->buttonname: ""),
					(button->modulename!=NULL ? button->modulename: ""),
					(button->filename!=NULL ? button->filename: ""),
					(button->keyname!=NULL ? button->keyname: ""),
					button->type,
					button->evalargs,
					(button->args!=NULL ? "args":"NULL"));
				fprintf(fp,"\t\tif(temp1!=NULL) Rfree(temp1);\n");
				if(button->args!=NULL)
				{
					fprintf(fp,"\t\tif(args!=NULL) freeapplib(args);\n");
				}
				if(button->bf!=NULL && button->num>0)
				{
					fprintf(fp,"\t\tif(x!=(-1))\n");
					fprintf(fp,"\t\t{\n");
					fprintf(fp,"\t\t\tbutton=MTNMSTR->buttons+(x-1);\n");
					for(y=0,bf=button->bf;y<button->num;++y,++bf)
					{	
						fprintf(fp,"\t\t\tx=addMaintainMasterButtonField(button,\"%s\",\"%s\");\n",
							(bf->rname!=NULL ? bf->rname: ""),
							(bf->ename!=NULL ? bf->ename: ""));
					}
					fprintf(fp,"\t\t}\n");
				}
			}
			fprintf(fp,"\n\n");
		}
		fprintf(fp,"%s BUTTONS Ending %s\n\n\n",thing1,thing2);
		}
	}

}
