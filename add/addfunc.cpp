/* addfunc.c - Power Add Functions and Structures */
#include<rptgen.hpp>
#include<powadd.hpp>

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

void PowerAddBrowseFreeFunction(MakeBrowseList *);
PowerAdd *xPowerAddNew(char *module,char *name,int line,char *file)
{
	PowerAdd *POWERADD=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG PowerAddNew at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	POWERADD=Rmalloc(sizeof(PowerAdd));
	if(module!=NULL) POWERADD->module=stralloc(module);
		else POWERADD->module=NULL;
	if(name!=NULL) POWERADD->name=stralloc(name);
		else POWERADD->name=NULL;
	POWERADD->define_list=NULL;
	POWERADD->addscreen=NULL;
	POWERADD->subnum=0;
	POWERADD->subord=NULL;
	POWERADD->args=NULL;
	POWERADD->reportname=NULL;
	POWERADD->desc=NULL;
	POWERADD->numbuttons=0;
	POWERADD->buttons=NULL;
	POWERADD->save_expression=NULL;
	POWERADD->save_warning=NULL;
	POWERADD->save_error_desc=NULL;
	POWERADD->save_warning_desc=NULL;
	POWERADD->quit_error=NULL;
	POWERADD->quit_warning=NULL;
	POWERADD->quit_error_desc=NULL;
	POWERADD->quit_warning_desc=NULL;
	POWERADD->numWrites=0;
	POWERADD->Writes=NULL;
	POWERADD->numwdgts=0;
	POWERADD->EditWdgts=NULL;
	POWERADD->SubFunc=NULL;
	POWERADD->SubFuncArgs=NULL;
	POWERADD->addrsrc=NULL;
	POWERADD->PREVIOUSADD=NULL;
	POWERADD->ADD_DEFINE_LIST=NULL;
	POWERADD->WRITE_COUNTER=0;
	POWERADD->save_type=0;
	POWERADD->APPmainLoop=FALSE;
	POWERADD->AddType=0;
	POWERADD->TableWidget=NULL;
	POWERADD->table_row=0;
	POWERADD->table_col=0;
	memset(POWERADD->UsingRow,0,101);
	return(POWERADD);
}
void xFreePowerAdd(PowerAdd *POWERADD,int line,char *file)
{
	RDAaddsub *addsub;
	int x,y,z;
	RDAconfld *confld=NULL;
	RDAeditfld *editfld=NULL;
	MaintainButton *button;
	ButtonField *bf;
	AddWriteField *WriteField=NULL;
	AddWrite *Write=NULL;
	AddEditWdgt *EditWdgt=NULL;
/*
	Browse_Exit *BE=NULL;
*/

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG FreePowerAdd at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		if(POWERADD->module!=NULL) Rfree(POWERADD->module);
		if(POWERADD->name!=NULL) Rfree(POWERADD->name);
		if(POWERADD->desc!=NULL) Rfree(POWERADD->desc);
		if(POWERADD->define_list!=NULL) Rfree(POWERADD->define_list);
		if(POWERADD->addscreen!=NULL) Rfree(POWERADD->addscreen);
		if(POWERADD->reportname!=NULL) Rfree(POWERADD->reportname);
		if(POWERADD->save_expression!=NULL) Rfree(POWERADD->save_expression);
		if(POWERADD->save_warning!=NULL) Rfree(POWERADD->save_warning);
		if(POWERADD->save_error_desc!=NULL) Rfree(POWERADD->save_error_desc);
		if(POWERADD->save_warning_desc!=NULL) Rfree(POWERADD->save_warning_desc);
		if(POWERADD->quit_error!=NULL) Rfree(POWERADD->quit_error);
		if(POWERADD->quit_warning!=NULL) Rfree(POWERADD->quit_warning);
		if(POWERADD->quit_error_desc!=NULL) Rfree(POWERADD->quit_error_desc);
		if(POWERADD->quit_warning_desc!=NULL) Rfree(POWERADD->quit_warning_desc);
		if(POWERADD->subord!=NULL)
		{
			for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
			{
				if(addsub->module!=NULL) Rfree(addsub->module);
				if(addsub->filename!=NULL) Rfree(addsub->filename);
				if(addsub->subkeyname!=NULL) Rfree(addsub->subkeyname);
				if(addsub->cmodule!=NULL) Rfree(addsub->cmodule);
				if(addsub->cfilename!=NULL) Rfree(addsub->cfilename);	
				if(addsub->expression!=NULL) Rfree(addsub->expression);
				if(addsub->browse_screen!=NULL) Rfree(addsub->browse_screen);
				if(addsub->button_name!=NULL) Rfree(addsub->button_name);
				if(addsub->define_list!=NULL) Rfree(addsub->define_list);
				if(addsub->search_browse!=NULL) Rfree(addsub->search_browse);
				if(addsub->confld!=NULL)
				{
					for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
					{
						if(confld->fldname!=NULL) Rfree(confld->fldname);
						if(confld->conname!=NULL) Rfree(confld->conname);
					}
					Rfree(addsub->confld);
				}
				if(addsub->BL!=NULL)
				{
					PowerAddBrowseFreeFunction(addsub->BL);
					addsub->BL=NULL;
/* trying this */
/*
					BE=RETURN_BROWSE_EXIT(addsub->BL);
					if(BE!=NULL)
					{
						FreeBrowseExit(BE);
					}
*/
				}
			}
			Rfree(POWERADD->subord);
		}
		if(POWERADD->Writes!=NULL)
		{
			for(x=0,Write=POWERADD->Writes;x<POWERADD->numWrites;++x,++Write)
			{
				if(Write->module!=NULL) Rfree(Write->module);
				if(Write->filename!=NULL) Rfree(Write->filename);
				if(Write->expression!=NULL) Rfree(Write->expression);
				if(Write->fields!=NULL)
				{
					for(y=0,WriteField=Write->fields;
						y<Write->numflds;
						++y,++WriteField)
					{
						if(WriteField->to_name!=NULL)
							Rfree(WriteField->to_name);
						if(WriteField->from_name!=NULL)
							Rfree(WriteField->from_name);	
					}
					Rfree(Write->fields);
				}
			}
			Rfree(POWERADD->Writes);
		}
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
				++x,++EditWdgt)
			{
				if(EditWdgt->name!=NULL) Rfree(EditWdgt->name);
				if(EditWdgt->expression!=NULL) Rfree(EditWdgt->expression);
				if(EditWdgt->module!=NULL) Rfree(EditWdgt->module);
				if(EditWdgt->filename!=NULL) Rfree(EditWdgt->filename);
				if(EditWdgt->subkeyname!=NULL) Rfree(EditWdgt->subkeyname);
				if(EditWdgt->mgt_expression!=NULL) Rfree(EditWdgt->mgt_expression);
				if(EditWdgt->assignname!=NULL) Rfree(EditWdgt->assignname);
				if(EditWdgt->id!=NULL) Rfree(EditWdgt->id);
				if(EditWdgt->override_expression!=NULL) Rfree(EditWdgt->override_expression);
				if(EditWdgt->acctrsc!=NULL) freeapplib(EditWdgt->acctrsc);
				if(EditWdgt->defrsc!=NULL) freeapplib(EditWdgt->defrsc);
				if(EditWdgt->confld!=NULL && EditWdgt->num>0)
				{
					for(y=0,editfld=EditWdgt->confld;y<EditWdgt->num;++y,++confld)
					{
						if(editfld->fldname!=NULL) Rfree(editfld->fldname);
						if(editfld->conname!=NULL) Rfree(editfld->conname);
						if(editfld->type!=(-1))
						{
							switch(editfld->type)
							{
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
									if(editfld->data.string_value!=NULL) Rfree(editfld->data.string_value);
									break;
								case CHARACTERS:
									if(editfld->data.string_value!=NULL)
										Rfree(editfld->data.string_value);
									break;
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case SDECIMALV:
									if(editfld->data.float_value!=NULL) 
										Rfree(editfld->data.float_value); 
									break;
								case SHORTV:
								case SSHORTV:
									if(editfld->data.short_value!=NULL) 
										Rfree(editfld->data.short_value); 
									break;
								case LONGV:
								case SLONGV:
								case SCROLLEDLIST:
									if(editfld->data.integer_value!=NULL)
										Rfree(editfld->data.integer_value);
									break;
								default:
								case BUTTONS:
									break;

							}
							editfld->dlen=0;
							editfld->len=0;
							editfld->type=(-1);
						}
					}
				
				}
			}
			Rfree(POWERADD->EditWdgts);
		}
		if(POWERADD->buttons!=NULL)
		{
			for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
			{
				if(button->progname!=NULL) Rfree(button->progname);
				if(button->modulename!=NULL) Rfree(button->modulename);
				if(button->filename!=NULL) Rfree(button->filename);
				if(button->keyname!=NULL) Rfree(button->keyname);
				if(button->listname!=NULL) Rfree(button->listname);
				if(button->buttonname!=NULL) Rfree(button->buttonname);
				if(button->expression!=NULL) Rfree(button->expression);
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
			Rfree(POWERADD->buttons);
		}
		Rfree(POWERADD);
	}
}
static void SetRDAsupportingPOWERADDOccurrences(PowerAdd *POWERADD)
{
	int x,y;
	short Roccurrence=0;
	RDAaddsub *addsub,*addsub1;

	if(POWERADD!=NULL)
	{
	if(POWERADD->subord!=NULL)
	{
		for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub) 
			addsub->occurrence=(-1);
		for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
		{
			if(addsub->occurrence==(-1))
			{
				addsub->occurrence=1;
				Roccurrence=1;
				for(y=(x+1);y<POWERADD->subnum;++y)
				{
					addsub1=POWERADD->subord+y;
					if(addsub1->occurrence==(-1) && !RDAstrcmp(addsub->module,addsub1->module) && !RDAstrcmp(addsub->filename,addsub1->filename))
					{
						++Roccurrence;
						addsub1->occurrence=Roccurrence;
					}
				}
			}
		}
	}
	}
}
short xgetPowerAdd(char *libname,PowerAdd *POWERADD,int line,char *file)
{
	short version=FALSE,x,test=FALSE;
	BIN *bin;
	RDAaddsub *addsub;
	int y,z,w,tot=0;
	RDAconfld *confld=NULL;
	RDAeditfld *aconfld=NULL;
	MaintainButton *button;
	ButtonField *bf;
	char *temp=NULL;
	AddWriteField *WriteField=NULL;
	AddWrite *Write=NULL;
	AddEditWdgt *EditWdgt=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG getPowerAdd Attempting to Read Power Add Binary [%s] from Power Add Binary Library [%s] at line [%d] program [%s].",POWERADD->name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,POWERADD->name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("Error Attempting to Read Power Add Binary [%s] from Power Add Library [%s] Failed at line [%d] program [%s].",POWERADD->name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1685) && x!=(-1686) && x!=(-1687) && x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary for Power Add [%s] in Power Add Binary Library [%s] at line [%d] program [%s].",POWERADD->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1685)) version=16;
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
	if(POWERADD->module!=NULL) Rfree(POWERADD->module);
	if(test) POWERADD->module=BINgetstring(bin);
		else POWERADD->module=NULL;
	test=BINgetshort(bin);
	if(POWERADD->name!=NULL) Rfree(POWERADD->name);
	if(test) POWERADD->name=BINgetstring(bin);
		else POWERADD->name=NULL;
	test=BINgetshort(bin);
	if(test) POWERADD->desc=BINgetstring(bin);
		else POWERADD->desc=NULL;
	test=BINgetshort(bin);
	if(test) POWERADD->define_list=BINgetstring(bin);
		else POWERADD->define_list=NULL;
	test=BINgetshort(bin);
	if(test) POWERADD->addscreen=BINgetstring(bin);
		else POWERADD->addscreen=NULL;
	test=BINgetshort(bin);
	if(test) POWERADD->reportname=BINgetstring(bin);
		else POWERADD->reportname=NULL;
	if(version>11)
	{
		POWERADD->save_type=BINgetint(bin);
	} else POWERADD->save_type=0;
	test=BINgetshort(bin);
	if(test) POWERADD->save_expression=BINgetstring(bin);
		else POWERADD->save_expression=NULL;
	test=BINgetshort(bin);
	if(test) POWERADD->save_error_desc=BINgetstring(bin);
		else POWERADD->save_error_desc=NULL;
	test=BINgetshort(bin);
	if(test) POWERADD->save_warning=BINgetstring(bin);
		else POWERADD->save_warning=NULL;
	test=BINgetshort(bin);
	if(test) POWERADD->save_warning_desc=BINgetstring(bin);
		else POWERADD->save_warning_desc=NULL;
	if(version>3)
	{
		test=BINgetshort(bin);
		if(test) POWERADD->quit_error=BINgetstring(bin);
			else POWERADD->quit_error=NULL;
		test=BINgetshort(bin);
		if(test) POWERADD->quit_error_desc=BINgetstring(bin);
			else POWERADD->quit_error_desc=NULL;
		test=BINgetshort(bin);
		if(test) POWERADD->quit_warning=BINgetstring(bin);
			else POWERADD->quit_warning=NULL;
		test=BINgetshort(bin);
		if(test) POWERADD->quit_warning_desc=BINgetstring(bin);
			else POWERADD->quit_warning_desc=NULL;
	} else {
		POWERADD->quit_error=NULL;
		POWERADD->quit_error_desc=NULL;
		POWERADD->quit_warning=NULL;
		POWERADD->quit_warning_desc=NULL;
	}
	POWERADD->numbuttons=BINgetint(bin);
	if(POWERADD->numbuttons>0)
	{
		POWERADD->buttons=Rmalloc(POWERADD->numbuttons*sizeof(MaintainButton));
		if(version>1)
		{
			for(z=0,button=POWERADD->buttons;z<POWERADD->numbuttons;++z,++button)
			{
				button->parent=POWERADD;
				button->func=NULL;
				test=BINgetshort(bin);
				if(test) button->progname=BINgetstring(bin);
					else button->progname=NULL;
				test=BINgetshort(bin);
				if(test) button->modulename=BINgetstring(bin);
					else button->modulename=NULL;
				test=BINgetshort(bin);
				if(test) button->filename=BINgetstring(bin);
					else button->filename=NULL;
				test=BINgetshort(bin);
				if(test) button->keyname=BINgetstring(bin);
					else button->keyname=NULL;
				test=BINgetshort(bin);
				if(test) button->listname=BINgetstring(bin);
					else button->listname=NULL;
				test=BINgetshort(bin);
				if(test) button->buttonname=BINgetstring(bin);
					else button->buttonname=NULL;
				if(version>7) 
				{	
					test=BINgetshort(bin);
					if(test) button->evalargs=TRUE;
						else button->evalargs=FALSE;
					if(version>10)
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
				button->args=APPlibNEW();
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
		} else {
			for(z=0,button=POWERADD->buttons;z<POWERADD->numbuttons;++z,++button)
			{
				button->parent=POWERADD;
				button->func=NULL;
				test=BINgetshort(bin);
				if(test) button->progname=BINgetstring(bin);
					else button->progname=NULL;
				test=BINgetshort(bin);
				if(test) button->modulename=BINgetstring(bin);
					else button->modulename=NULL;
				test=BINgetshort(bin);
				if(test) button->filename=BINgetstring(bin);
					else button->filename=NULL;
				button->evalargs=FALSE;
				button->expression=NULL;
				button->type=0;
			}
			test=BINgetshort(bin);
			if(test) button->keyname=BINgetstring(bin);
				else button->keyname=NULL;
			test=BINgetshort(bin);
			if(test) button->listname=BINgetstring(bin);
				else button->listname=NULL;
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
			button->args=APPlibNEW();
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
	} else {
		POWERADD->buttons=NULL;
		POWERADD->numbuttons=0;
	}
	POWERADD->subnum=BINgetint(bin);
	if(POWERADD->subnum>0)
	{
		POWERADD->subord=Rmalloc(POWERADD->subnum*sizeof(RDAaddsub));
		for(y=0,addsub=POWERADD->subord;y<POWERADD->subnum;++y,++addsub)
		{
			addsub->skip_widget=FALSE;
			test=BINgetshort(bin);
			addsub->fileno=(-1);
			addsub->keyno=(-1);
			if(test) addsub->module=BINgetstring(bin);
				else addsub->module=NULL;
			test=BINgetshort(bin);
			if(test) addsub->filename=BINgetstring(bin);
				else addsub->filename=NULL;
			if(version>6)
			{
				test=BINgetshort(bin);
				if(test)
				{
					addsub->subkeyname=BINgetstring(bin);
				} else {
					addsub->subkeyname=NULL;
				}
			} else {
				addsub->subkeyname=NULL;
			}
			if(version>12)
			{
				test=BINgetshort(bin);
				if(test) addsub->expression=BINgetstring(bin);
					else addsub->expression=NULL;
			} else addsub->expression=NULL;
			addsub->dtype=BINgetint(bin);
			addsub->cmodule=NULL;
			addsub->cfilename=NULL;
			if(version>4)
			{
				test=BINgetshort(bin);
				if(test) addsub->browse=TRUE;
					else addsub->browse=FALSE;
				if(version>5)
				{
					test=BINgetshort(bin);
					if(test) addsub->button_name=BINgetstring(bin);
						else addsub->button_name=NULL;
				} else {
					addsub->button_name=NULL;
				}
				test=BINgetshort(bin);
				if(test) addsub->browse_screen=BINgetstring(bin);
					else addsub->browse_screen=NULL;
				test=BINgetshort(bin);
				if(test) addsub->define_list=BINgetstring(bin);
					else addsub->define_list=NULL;
				test=BINgetshort(bin);
				if(test) addsub->search_browse=BINgetstring(bin);
					else addsub->search_browse=NULL;
				if(version>13)
				{
					test=BINgetshort(bin);
					if(version<15)
					{
						if(test) addsub->smart_browse=TRUE;
							else addsub->smart_browse=FALSE;
					} else {
						addsub->smart_browse=test;
					}
				} else {
					addsub->smart_browse=FALSE;
				}
			} else {
				addsub->browse=FALSE;
				addsub->button_name=NULL;
				addsub->browse_screen=NULL;
				addsub->define_list=NULL;
				addsub->search_browse=NULL;
				addsub->smart_browse=FALSE;
			}
			addsub->num=BINgetshort(bin);
			if(addsub->num>0)
			{
				addsub->confld=Rmalloc(addsub->num*sizeof(RDAconfld));
				for(z=0,confld=addsub->confld;z<addsub->num;++z,++confld)
				{
					test=BINgetshort(bin);
					if(test) confld->fldname=BINgetstring(bin);
						else confld->fldname=NULL;
					test=BINgetshort(bin);
					if(test) confld->conname=BINgetstring(bin);
						else confld->conname=NULL;
					if(version>4)
					{
						test=BINgetshort(bin);
						if(test) confld->inherit=TRUE;
							else confld->inherit=FALSE;
					} else {
						confld->inherit=FALSE;
					}
				}
			} else addsub->confld=NULL;
			addsub->BL=NULL;
			addsub->parent=POWERADD;
			addsub->parent_rsrc=NULL;
/*
			addsub->parent_rsrc=POWERADD->addrsrc;
*/
		}
	} else { 
		POWERADD->subord=NULL;
		POWERADD->subnum=0;
	}
	POWERADD->numWrites=BINgetint(bin);
	if(POWERADD->numWrites>0)
	{
		POWERADD->Writes=Rmalloc(POWERADD->numWrites*sizeof(AddWrite));
		for(y=0,Write=POWERADD->Writes;y<POWERADD->numWrites;++y,++Write)
		{
			Write->fileno=(-1);
			test=BINgetshort(bin);
			if(test) Write->module=BINgetstring(bin);
				else Write->module=NULL;
			test=BINgetshort(bin);
			if(test) Write->filename=BINgetstring(bin);
				else Write->filename=NULL;
			test=BINgetshort(bin);	
			if(test) Write->expression=BINgetstring(bin);
				else Write->expression=NULL;
			Write->numflds=BINgetint(bin);
			if(Write->numflds>0)
			{
				Write->fields=Rmalloc(Write->numflds*sizeof(AddWriteField));
				for(z=0,WriteField=Write->fields;z<Write->numflds;++z,++WriteField)
				{
					test=BINgetshort(bin);
					if(test) WriteField->to_name=BINgetstring(bin);
						else WriteField->to_name=NULL;
					WriteField->copy_type=BINgetshort(bin);
					test=BINgetshort(bin);
					if(test) WriteField->from_name=BINgetstring(bin);
						else WriteField->from_name=NULL;
				}
			} else Write->fields=NULL;
		}
	} else POWERADD->Writes=NULL;
	POWERADD->numwdgts=BINgetint(bin);
	if(POWERADD->numwdgts>0)
	{
		POWERADD->EditWdgts=Rmalloc(POWERADD->numwdgts*sizeof(AddEditWdgt));
		for(y=0,EditWdgt=POWERADD->EditWdgts;y<POWERADD->numwdgts;++y,++EditWdgt)
		{
			test=BINgetshort(bin);
			if(test) EditWdgt->name=BINgetstring(bin);
				else EditWdgt->name=NULL;
			EditWdgt->type=BINgetshort(bin);
			EditWdgt->len=BINgetint(bin);
			if(version>0)
			{
				EditWdgt->etype=BINgetint(bin);
			} else EditWdgt->etype=0;
			test=BINgetshort(bin);
			if(test) EditWdgt->expression=BINgetstring(bin);
				else EditWdgt->expression=NULL;
			if(version>15)
			{
				test=BINgetshort(bin);
				EditWdgt->loadvirtual=test;
			} else EditWdgt->loadvirtual=FALSE;
			EditWdgt->wrote_id=FALSE;
			if(version>2)
			{
				EditWdgt->id_mgt=BINgetshort(bin);
				if(EditWdgt->id_mgt)
				{
					test=BINgetshort(bin);
					if(test) EditWdgt->module=BINgetstring(bin);
						else EditWdgt->module=NULL;
					test=BINgetshort(bin);
					if(test) EditWdgt->filename=BINgetstring(bin);
						else EditWdgt->filename=NULL;
					if(version>8)
					{
						test=BINgetshort(bin);
						if(test) EditWdgt->subkeyname=BINgetstring(bin);
							else EditWdgt->subkeyname=NULL;
						EditWdgt->num=BINgetint(bin);
						if(EditWdgt->num>0)
						{
							EditWdgt->confld=Rmalloc(EditWdgt->num*sizeof(RDAeditfld));
							for(z=0,aconfld=EditWdgt->confld;z<EditWdgt->num;++z,++aconfld)
							{
								test=BINgetshort(bin);
								if(test) aconfld->fldname=BINgetstring(bin);
									else aconfld->fldname=NULL;
								test=BINgetshort(bin);
								if(test) aconfld->conname=BINgetstring(bin);
									else aconfld->conname=NULL;
								aconfld->type=(-1);
								aconfld->len=0;
								aconfld->dlen=0;
							}
						} else { 
							EditWdgt->confld=NULL;
							EditWdgt->num=0;
						}
						test=BINgetshort(bin);
						if(test) EditWdgt->mgt_expression=BINgetstring(bin);
							else EditWdgt->mgt_expression=NULL;
						test=BINgetshort(bin);
						if(test) EditWdgt->assignname=BINgetstring(bin);
							else EditWdgt->assignname=NULL;
						test=BINgetshort(bin);
						if(test) EditWdgt->id=BINgetstring(bin);
							else EditWdgt->id=NULL;
						if(version>9)
						{
							test=BINgetshort(bin);
							if(test) EditWdgt->override_expression=BINgetstring(bin);
								else EditWdgt->override_expression=NULL;
						} else {
							EditWdgt->override_expression=NULL;
						}
					} else {
						EditWdgt->subkeyname=NULL;
						EditWdgt->num=0;
						EditWdgt->confld=NULL;
						EditWdgt->mgt_expression=NULL;
						EditWdgt->assignname=NULL;
						if(!isEMPTY(EditWdgt->name))
						{
							EditWdgt->id=stralloc(EditWdgt->name);
						} else {
							EditWdgt->id=NULL;
						}
						EditWdgt->override_expression=NULL;
						
					}
				} else {
					EditWdgt->module=NULL;
					EditWdgt->filename=NULL;
					EditWdgt->subkeyname=NULL;
					EditWdgt->num=0;
					EditWdgt->confld=NULL;
					EditWdgt->mgt_expression=NULL;
					EditWdgt->assignname=NULL;
					if(!isEMPTY(EditWdgt->name))
					{
						EditWdgt->id=stralloc(EditWdgt->name);
					} else {
						EditWdgt->id=NULL;
					}
					EditWdgt->override_expression=NULL;
				}
				EditWdgt->fileno=(-1);
/* PLACE HERE */
				if(version>11)
				{
					test=BINgetshort(bin);
					if(test==TRUE) EditWdgt->setaccttype=TRUE;
						else EditWdgt->setaccttype=FALSE;
					if(EditWdgt->setaccttype==TRUE)
					{
						tot=BINgetint(bin);
						if(tot>0)
						{
							EditWdgt->acctrsc=APPlibNEW();
							for(w=0;w<tot;++w)
							{
								test=BINgetshort(bin);
								if(test) temp=BINgetstring(bin); else temp=NULL;
								addAPPlib(EditWdgt->acctrsc,temp);
								if(temp!=NULL) Rfree(temp);
							}
						}  else EditWdgt->acctrsc=NULL;
					}  else EditWdgt->acctrsc=NULL;
					test=BINgetshort(bin);
					if(test==TRUE) EditWdgt->setdeftype=TRUE;
						else EditWdgt->setdeftype=FALSE;
					if(EditWdgt->setdeftype==TRUE)
					{
						tot=BINgetint(bin);
						if(tot>0)
						{
							EditWdgt->defrsc=APPlibNEW();
							for(w=0;w<tot;++w)
							{
								test=BINgetshort(bin);
								if(test) temp=BINgetstring(bin); else temp=NULL;
								addAPPlib(EditWdgt->defrsc,temp);
								if(temp!=NULL) Rfree(temp);
							}
						} else EditWdgt->defrsc=NULL;
					} else EditWdgt->defrsc=NULL;
				} else {
					EditWdgt->setaccttype=FALSE;
					EditWdgt->acctrsc=NULL;
					EditWdgt->setdeftype=FALSE;
					EditWdgt->defrsc=NULL;
				}
			} else {
				EditWdgt->id_mgt=FALSE;
				EditWdgt->wrote_id=FALSE;
				EditWdgt->module=NULL;
				EditWdgt->filename=NULL;
				EditWdgt->subkeyname=NULL;
				EditWdgt->num=0;
				EditWdgt->confld=NULL;
				EditWdgt->mgt_expression=NULL;
				EditWdgt->assignname=NULL;
				EditWdgt->loadvirtual=FALSE;
				if(!isEMPTY(EditWdgt->name))
				{
					EditWdgt->id=stralloc(EditWdgt->name);
				} else {
					EditWdgt->id=NULL;
				}
				EditWdgt->override_expression=NULL;
				EditWdgt->fileno=(-1);
				EditWdgt->setaccttype=FALSE;
				EditWdgt->acctrsc=NULL;
				EditWdgt->setdeftype=FALSE;
				EditWdgt->defrsc=NULL;
			}
			EditWdgt->parent=POWERADD;
		}
	} else POWERADD->EditWdgts=NULL;
	SetRDAsupportingPOWERADDOccurrences(POWERADD);
	BINfree(bin);
	return(0);
}
short xwritePowerAdd(char *libname,PowerAdd *POWERADD,int line,
	char *file)
{
	BIN *bin;
	RDAaddsub *addsub;
	int y,z;
	RDAconfld *confld=NULL;
	RDAeditfld *aconfld=NULL;
	MaintainButton *button;
	ButtonField *bf;
	AddWriteField *WriteField=NULL;
	AddWrite *Write=NULL;
	AddEditWdgt *EditWdgt=NULL;
	char *temp=NULL;
	short smart_browse=0,ldv=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG writePowerAdd Writing PowerAdd [%s] into Power Add Binary Library [%s] at line [%d] program [%s].",(POWERADD->name!=NULL ? POWERADD->name:""),libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,(-1685));
	if(POWERADD->module!=NULL) 
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->module);
	} else BINaddshort(bin,FALSE);
	if(POWERADD->name!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->name);
	} else BINaddshort(bin,FALSE);
	if(POWERADD->desc!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->desc);
	} else BINaddshort(bin,FALSE);
	if(POWERADD->define_list!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->define_list);
	} else BINaddshort(bin,FALSE);
	if(POWERADD->addscreen!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->addscreen);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->reportname))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->reportname);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,POWERADD->save_type);
	if(!isEMPTY(POWERADD->save_expression))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->save_expression);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->save_error_desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->save_error_desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->save_warning))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->save_warning);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->save_warning_desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->save_warning_desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->quit_error))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->quit_error);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->quit_error_desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->quit_error_desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->quit_warning))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->quit_warning);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(POWERADD->quit_warning_desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,POWERADD->quit_warning_desc);
	} else BINaddshort(bin,FALSE);
	if(POWERADD->buttons!=NULL)
	{
		BINaddint(bin,POWERADD->numbuttons);
		for(y=0,button=POWERADD->buttons;y<POWERADD->numbuttons;++y,++button)
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
			if(button->evalargs==FALSE)
			{
				BINaddshort(bin,FALSE);
			} else {
				BINaddshort(bin,TRUE);
			}
			if(!isEMPTY(button->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,button->expression);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,button->type);
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
			} else BINaddint(bin,0);
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
	if(POWERADD->subord!=NULL)
	{
		BINaddint(bin,POWERADD->subnum);
		for(y=0,addsub=POWERADD->subord;y<POWERADD->subnum;++y,++addsub)
		{
			if(addsub->module!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->module);
			} else BINaddshort(bin,FALSE);
			if(addsub->filename!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->filename);
			} else {
				BINaddshort(bin,FALSE);
			}
			if(addsub->subkeyname!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->subkeyname);
			} else {
				BINaddshort(bin,FALSE);
			}
			if(!isEMPTY(addsub->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->expression);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,addsub->dtype);
			BINaddshort(bin,(addsub->browse==TRUE?TRUE:FALSE));
			if(!isEMPTY(addsub->button_name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->button_name);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(addsub->browse_screen))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->browse_screen);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(addsub->define_list))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->define_list);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(addsub->search_browse))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,addsub->search_browse);
			} else BINaddshort(bin,FALSE);
			smart_browse=addsub->smart_browse;
			BINaddshort(bin,smart_browse);
			BINaddshort(bin,addsub->num);
			if(addsub->num>0)
			{
				for(z=0,confld=addsub->confld;z<addsub->num;++z,++confld)
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
					if(confld->inherit) BINaddshort(bin,TRUE);
						else BINaddshort(bin,FALSE);
				}
			}
		}
	} else {
		BINaddint(bin,0);
	}
	BINaddint(bin,POWERADD->numWrites);
	if(POWERADD->numWrites>0)
	{
		for(y=0,Write=POWERADD->Writes;y<POWERADD->numWrites;++y,++Write)
		{
			if(!isEMPTY(Write->module))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,Write->module);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(Write->filename))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,Write->filename);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(Write->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,Write->expression);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,Write->numflds);
			if(Write->numflds>0)
			{
				for(z=0,WriteField=Write->fields;z<Write->numflds;++z,++WriteField)
				{
					if(!isEMPTY(WriteField->to_name))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,WriteField->to_name);
					} else BINaddshort(bin,FALSE);
					BINaddshort(bin,WriteField->copy_type);
					if(!isEMPTY(WriteField->from_name))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,WriteField->from_name);
					} else BINaddshort(bin,FALSE);
				}
			}
		}
	}
	BINaddint(bin,POWERADD->numwdgts);
	if(POWERADD->numwdgts>0)
	{
		for(y=0,EditWdgt=POWERADD->EditWdgts;y<POWERADD->numwdgts;++y,++EditWdgt)
		{
			if(!isEMPTY(EditWdgt->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,EditWdgt->name);
			} else BINaddshort(bin,FALSE);
			BINaddshort(bin,EditWdgt->type);
			BINaddint(bin,EditWdgt->len);
			BINaddint(bin,EditWdgt->etype);
			if(!isEMPTY(EditWdgt->expression))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,EditWdgt->expression);
			} else BINaddshort(bin,FALSE);
			ldv=EditWdgt->loadvirtual;
			BINaddshort(bin,ldv);
			BINaddshort(bin,EditWdgt->id_mgt);
			if(EditWdgt->id_mgt)
			{
				if(!isEMPTY(EditWdgt->module))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,EditWdgt->module);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(EditWdgt->filename))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,EditWdgt->filename);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(EditWdgt->subkeyname))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,EditWdgt->subkeyname);
				} else BINaddshort(bin,FALSE);
				BINaddint(bin,EditWdgt->num);
				if(EditWdgt->num>0)
				{
					for(z=0,aconfld=EditWdgt->confld;z<EditWdgt->num;++z,++aconfld)
					{
						if(aconfld->fldname!=NULL)
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,aconfld->fldname);
						} else {
							BINaddshort(bin,FALSE);
						}
						if(aconfld->conname!=NULL) 
						{
							BINaddshort(bin,TRUE);
							BINaddstring(bin,aconfld->conname);
						} else {
							BINaddshort(bin,FALSE);
						}
					}
				}
				if(!isEMPTY(EditWdgt->mgt_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,EditWdgt->mgt_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(EditWdgt->assignname))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,EditWdgt->assignname);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(EditWdgt->id))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,EditWdgt->id);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(EditWdgt->override_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,EditWdgt->override_expression);
				} else BINaddshort(bin,FALSE);
			}
			BINaddshort(bin,(EditWdgt->setaccttype==TRUE ? TRUE:FALSE));
			if(EditWdgt->setaccttype)
			{
				if(EditWdgt->acctrsc!=NULL)
				{
					if(EditWdgt->acctrsc->numlibs>0)
					{
						BINaddint(bin,EditWdgt->acctrsc->numlibs);
						for(z=0;z<EditWdgt->acctrsc->numlibs;++z)
						{
							temp=EditWdgt->acctrsc->libs[z];
							if(!isEMPTY(temp))
							{
								BINaddshort(bin,TRUE);
								BINaddstring(bin,temp);
							} else BINaddshort(bin,FALSE);
						}
					} else BINaddint(bin,0);
				} else BINaddint(bin,0);
			}
			BINaddshort(bin,(EditWdgt->setdeftype==TRUE ? TRUE:FALSE));
			if(EditWdgt->setdeftype)
			{
				if(EditWdgt->defrsc!=NULL)
				{
					if(EditWdgt->defrsc->numlibs>0)
					{
						BINaddint(bin,EditWdgt->defrsc->numlibs);
						for(z=0;z<EditWdgt->defrsc->numlibs;++z)
						{
							temp=EditWdgt->defrsc->libs[z];
							if(!isEMPTY(temp))
							{
								BINaddshort(bin,TRUE);
								BINaddstring(bin,temp);
							} else BINaddshort(bin,FALSE);
						}
					} else BINaddint(bin,0);
				} else BINaddint(bin,0);
			}
		}
	}
	if(writelibbin(libname,bin,POWERADD->name))
	{
		prterr("Error Can't write PowerAdd binary [%s] into Power Add library [%s] at line [%d] program [%s].",
			(POWERADD->name!=NULL?POWERADD->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
char *xgetPowerAdddesc(char *libname,char *name,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG getPowerAdddesc Attempting to Read Power Add description  for [%s] from Power Add Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagadd)
		{
			prterr("Error Attempting to Read Power Add Binary [%s] from Power Add Library [%s] Failed at line [%d] program [%s].",name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(NULL);
	}
	x=BINgetshort(bin);
	if(x!=(-1685) && x!=(-1686) && x!=(-1687) && x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary for Power Add [%s] in Power Add Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
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
short xfindPowerAdd(PowerAdd *POWERADD,int line,char *file)
{
	short ef=(-1);
	char *libname=NULL,*dashes=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd) 
	{ 
		prterr("DIAG findMaintainMAster [%s] [%s] at line [%d] program [%s].",POWERADD->module,POWERADD->name,line,file); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	dashes=adddashes(POWERADD->module);
	libname=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
	sprintf(libname,"%s/%s/%s.ADD",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s\\%s.ADD",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
	if(dashes!=NULL) Rfree(dashes);
	if(!getPowerAdd(libname,POWERADD)) ef=FALSE;
	if(ef && (!isEMPTY(RDA_GROUP)))
	{
		dashes=adddashes(POWERADD->module);
		libname=Rmalloc(RDAstrlen(RDA_GROUP)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
		sprintf(libname,"%s/%s/%s.ADD",CURRENTDIRECTORY,RDA_GROUP,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\%s\\%s.ADD",CURRENTDIRECTORY,RDA_GROUP,dashes);
#endif
		if(dashes!=NULL) Rfree(dashes);
		if(!getPowerAdd(libname,POWERADD)) ef=FALSE;
	}
	if(ef)
	{
		if(libname!=NULL) 
		{
			libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(POWERADD->module)+11);
		} else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(POWERADD->module)+11);
		}
#ifndef WIN32
		sprintf(libname,"%s/site/%s.ADD",CURRENTDIRECTORY,POWERADD->module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\site\\%s.ADD",CURRENTDIRECTORY,POWERADD->module);
#endif
		if(!getPowerAdd(libname,POWERADD)) ef=FALSE;
	}
	if(ef)
	{
		if(libname!=NULL) 
		{
			libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(POWERADD->module)+10);
		} else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
					RDAstrlen(POWERADD->module)+10);
		}
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.ADD",CURRENTDIRECTORY,POWERADD->module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.ADD",CURRENTDIRECTORY,POWERADD->module);
#endif
		if(!getPowerAdd(libname,POWERADD)) ef=FALSE;
	}
	if(ef)
	{
		prterr("Error Can't Find PowerAdd [%s] [%s] at line [%d] program [%s].",POWERADD->module,POWERADD->name,line,file);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		if(ef)
		{
			prterr("DIAG findPowerAdd Looking for [%s][%s] at line [%d] program [%s] has NOT been found in Power Add Library [%s].",POWERADD->module,POWERADD->name,line,file,libname);
		} else {
			prterr("DIAG findPowerAdd Looking for [%s][%s] at line [%d] program [%s] has been found in Power Add Library [%s].",POWERADD->module,POWERADD->name,line,file,libname);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(libname!=NULL) Rfree(libname);
	return(ef);
}
AddEditWdgt *xFINDEDITWDGT(PowerAdd *POWERADD,char *name,int line,char *file)
{
	AddEditWdgt *EditWdgt=NULL;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG FINDEDITWDGT [%s] at line [%d] program [%s] ",(name!=NULL ? name:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
			{
				if(!RDAstrcmp(EditWdgt->name,name)) break;
			}
			if(x<POWERADD->numwdgts) return(EditWdgt);
				else EditWdgt=NULL;
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(EditWdgt!=NULL)
	{
		if(diagadd)
		{
			prterr("DIAG FINDEDITWDGT Found [%s] at line [%d] program [%s] ",(name!=NULL ? name:""),line,file);
		}
	} else {
		if(diagadd)
		{
			prterr("DIAG FINDEDITWDGT Not Available [%s] at line [%d] program [%s] ",(name!=NULL ? name:""),line,file);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(EditWdgt);
}
void PowerAddSupInitFunctionLPM(RDAaddsub *addsub)
{
	PowerAdd *POWERADD=NULL;
	RDAconfld *confld=NULL;
	int y,last_inherit=(-1);
	char *temp=NULL;
	NRDfield *field=NULL;
	RDArsrc *rsrc=NULL;

	if(addsub!=NULL)
	{
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->parent_rsrc!=NULL)
		{
			rsrc=addsub->parent_rsrc;
		} else {
			rsrc=POWERADD->addrsrc;
		}
		if(addsub->confld!=NULL)
		{
		for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
		{
			if(confld->inherit==TRUE)
			{
				last_inherit=y;
			} else break;
		}
		for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
		{
			if((confld->inherit==TRUE) || (y==(last_inherit+1)))
			{
				if(!isEMPTY(confld->fldname))
				{
					field=FLDNRD(addsub->fileno,confld->fldname);
				} else field=NULL;
				if(field!=NULL)
				{
					if(!isEMPTY(confld->conname))
					{
						switch(field->type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								if(field->data.string_value!=NULL) Rfree(field->data.string_value);
								field->dlen=0;
								field->data.string_value=PP_EVALUATEstr(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
								if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
								break;
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								if(field->data.string_value!=NULL) Rfree(field->data.string_value);
								field->dlen=0;
								field->data.string_value=PP_EVALUATEstr(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
								if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
								break;
							case BOOLNS:
							case CHARACTERS:
								if(confld->inherit==TRUE) *field->data.string_value=PP_EVALUATEbol(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(confld->inherit==TRUE) *field->data.float_value=PP_EVALUATEdbl(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							case SHORTV:
							case SSHORTV:
								if(confld->inherit==TRUE) *field->data.short_value=PP_EVALUATEsht(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								if(confld->inherit==TRUE) *field->data.integer_value=PP_EVALUATEint(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							default:
							case BUTTONS:
								break;
						}
					} else {
						temp=Rmalloc(RDAstrlen(addsub->filename)+RDAstrlen(confld->fldname)+5);
						sprintf(temp,"[%s][%s]",addsub->filename,confld->fldname);
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
								FINDRSCGETSTRING(rsrc,temp,&field->data.string_value);
								break;
							case BOOLNS:
							case CHARACTERS:
								if(confld->inherit==TRUE) FINDRSCGETCHAR(rsrc,temp,field->data.string_value);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(confld->inherit==TRUE) FINDRSCGETDOUBLE(rsrc,temp,field->data.float_value);
								break;
							case SHORTV:
							case SSHORTV:
								if(confld->inherit==TRUE) FINDRSCGETSHORT(rsrc,temp,field->data.short_value);
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								if(confld->inherit==TRUE) FINDRSCGETINT(rsrc,temp,field->data.integer_value);
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
		}
		}
	}
}
void PowerAddSupInitFunction(RDAaddsub *addsub)
{
	PowerAdd *POWERADD=NULL;
	RDAconfld *confld=NULL;
	int y;
	char *temp=NULL;
	NRDfield *field=NULL;
	RDArsrc *rsrc=NULL;

	if(addsub!=NULL)
	{
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->parent_rsrc!=NULL)
		{
			rsrc=addsub->parent_rsrc;
		} else {
			rsrc=POWERADD->addrsrc;
		}
		if(addsub->confld!=NULL)
		{
		for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
		{
			if(confld->inherit==TRUE)
			{
				if(!isEMPTY(confld->fldname))
				{
					field=FLDNRD(addsub->fileno,confld->fldname);
				} else field=NULL;
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
								field->data.string_value=PP_EVALUATEstr(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
								if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
								break;
							case BOOLNS:
							case CHARACTERS:
								*field->data.string_value=PP_EVALUATEbol(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								*field->data.float_value=PP_EVALUATEdbl(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							case SHORTV:
							case SSHORTV:
								*field->data.short_value=PP_EVALUATEsht(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								*field->data.integer_value=PP_EVALUATEint(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								break;
							default:
							case BUTTONS:
								break;
						}
					} else {
						temp=Rmalloc(RDAstrlen(addsub->filename)+RDAstrlen(confld->fldname)+5);
						sprintf(temp,"[%s][%s]",addsub->filename,confld->fldname);
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
								FINDRSCGETSTRING(rsrc,temp,&field->data.string_value);
								break;
							case BOOLNS:
							case CHARACTERS:
								FINDRSCGETCHAR(rsrc,temp,field->data.string_value);
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								FINDRSCGETDOUBLE(rsrc,temp,field->data.float_value);
								break;
							case SHORTV:
							case SSHORTV:
								FINDRSCGETSHORT(rsrc,temp,field->data.short_value);
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								FINDRSCGETINT(rsrc,temp,field->data.integer_value);
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
		}
		}
	}
}
void PowerAddBrowseInitFunctionLPM(MakeBrowseList *BL)
{
	RDAaddsub *addsub=NULL;

	if(BL!=NULL)
	{
		addsub=(RDAaddsub *)BL->funcargs;
		PowerAddSupInitFunctionLPM(addsub);
	}
}
void PowerAddBrowseInitFunction(MakeBrowseList *BL)
{
	RDAaddsub *addsub=NULL;

	if(BL!=NULL)
	{
		addsub=(RDAaddsub *)BL->funcargs;
		PowerAddSupInitFunction(addsub);
	}
}
short PowerAddBrowseSelectFunction(MakeBrowseList *BL)
{
	RDAaddsub *addsub=NULL;
	PowerAdd *POWERADD=NULL;
	short return_value=TRUE;

	addsub=(RDAaddsub *)BL->funcargs;
	if(addsub!=NULL)
	{
		POWERADD=(PowerAdd *)addsub->parent;
		if(POWERADD!=NULL)
		{
			if(!isEMPTY(addsub->expression))
			{
				if(PP_EVALUATEbol(addsub->expression,POWERADD->SubFunc,POWERADD->SubFuncArgs))
				{
					return_value=TRUE;
				} else { 
					return_value=FALSE;
				}
			}
		}
	}
	return(return_value);
}
short PowerAddBrowseBreakFunctionLPM(MakeBrowseList *BL)
{
	RDAaddsub *addsub=NULL;
	PowerAdd *POWERADD=NULL;
	RDAconfld *confld=NULL;
	int y,ivalue=0,last_inherit=(-1),x=0;
	short return_value=TRUE,shvalue=0;
	char *temp=NULL,cvalue=FALSE,*svalue=NULL;
	char *temp3=NULL;
#ifdef USE_MYISAM
	char *temp2=NULL;
#endif
	double fvalue=0.0;
	NRDfield *field=NULL;
	RDArsrc *rsrc=NULL;

	addsub=(RDAaddsub *)BL->funcargs;
	if(addsub!=NULL)
	{
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->parent_rsrc!=NULL)
		{
			rsrc=addsub->parent_rsrc;
		} else {
			rsrc=POWERADD->addrsrc;
		}
		last_inherit=(-1);
		for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
		{
			if(confld->inherit==TRUE)
			{
				last_inherit=y;
			}
			if(confld->inherit==FALSE) break;
		}
		for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
		{
			if(return_value==FALSE) break;
			if((confld->inherit==TRUE) || (y==(last_inherit+1)))
			{
				field=FLDNRD(addsub->fileno,confld->fldname);
				if(field!=NULL)
				{
					if(!isEMPTY(confld->conname))
					{
						switch(field->type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								svalue=PP_EVALUATEstr(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(!isEMPTY(svalue))
								{
								x=RDAstrlen(svalue);
								for(;x>0;--x)
								{
									temp3=svalue+(x-1);
									if(*temp3==' ') *temp3=0;
								}
								if(isEMPTY(svalue)) 
								{
									if(svalue!=NULL) Rfree(svalue);
								}
								}
								if(confld->inherit==TRUE) 
								{
									if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
								} else {
#ifndef USE_MYISAM
									if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
									if(RDA_CollateType==FALSE)
									{
										stoupper(svalue);
										temp2=stralloc(field->data.string_value);
										stoupper(temp2);
										if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
										if(temp2!=NULL) Rfree(temp2);
									} else {
										if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
									}
#endif
								}
								if(svalue!=NULL) Rfree(svalue);
								break;
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								svalue=PP_EVALUATEstr(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(confld->inherit==TRUE) 
								{
									if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
								} else {
#ifndef USE_MYISAM
									if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
									if(RDA_CollateType==FALSE)
									{
										stoupper(svalue);
										temp2=stralloc(field->data.string_value);
										stoupper(temp2);
										if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
										if(temp2!=NULL) Rfree(temp2);
									} else {
										if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
									}
#endif
								}
								if(svalue!=NULL) Rfree(svalue);
								break;
							case BOOLNS:
							case CHARACTERS:
								if(confld->inherit==TRUE)
								{
								cvalue=PP_EVALUATEbol(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.string_value!=cvalue) return_value=FALSE;
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(confld->inherit==TRUE)
								{
								fvalue=PP_EVALUATEdbl(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.float_value!=fvalue) return_value=FALSE;
								}
								break;
							case SHORTV:
							case SSHORTV:
								if(confld->inherit==TRUE)
								{
								shvalue=PP_EVALUATEsht(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.short_value!=shvalue) return_value=FALSE;
								}
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								if(confld->inherit==TRUE)
								{
								ivalue=PP_EVALUATEint(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.integer_value!=ivalue) return_value=FALSE;
								}
								break;
							default:
							case BUTTONS:
								break;
						}
					} else {
						temp=Rmalloc(RDAstrlen(addsub->filename)+RDAstrlen(confld->fldname)+5);
						sprintf(temp,"[%s][%s]",addsub->filename,confld->fldname);
						switch(field->type)
						{
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
								FINDRSCGETSTRING(rsrc,temp,&svalue);
								if(!isEMPTY(svalue))
								{
								x=RDAstrlen(svalue);
								for(;x>0;--x)
								{
									temp3=svalue+(x-1);
									if(*temp3==' ') *temp3=0;
								}
								if(isEMPTY(svalue)) 
								{
									if(svalue!=NULL) Rfree(svalue);
								}
								}
								if(confld->inherit==TRUE) 
								{
									if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
								} else {
#ifndef USE_MYISAM
									if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
									if(RDA_CollateType==FALSE)
									{
										stoupper(svalue);
										temp2=stralloc(field->data.string_value);
										stoupper(temp2);
										if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
										if(temp2!=NULL) Rfree(temp2);
									} else {
										if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
									}
#endif
								}
								if(svalue!=NULL) Rfree(svalue);
								break;
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case OPTIONALFIELDS:
							case ZIPCODE:
								FINDRSCGETSTRING(rsrc,temp,&svalue);
								if(confld->inherit==TRUE) 
								{
									if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
								} else {
#ifndef USE_MYISAM
									if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
#else
									if(RDA_CollateType==FALSE)
									{
										stoupper(svalue);
										temp2=stralloc(field->data.string_value);
										stoupper(temp2);
										if(strncmp(temp2,svalue,RDAstrlen(svalue))) return_value=FALSE;
										if(temp2!=NULL) Rfree(temp2);
									} else {
										if(strncmp(field->data.string_value,svalue,RDAstrlen(svalue))) return_value=FALSE;
									}
#endif
								}
								if(svalue!=NULL) Rfree(svalue);
								break;
							case BOOLNS:
							case CHARACTERS:
								if(confld->inherit==TRUE)
								{
								FINDRSCGETCHAR(rsrc,temp,&cvalue);
								if(*field->data.string_value!=cvalue) return_value=FALSE;
								}
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								if(confld->inherit==TRUE)
								{
								FINDRSCGETDOUBLE(rsrc,temp,&fvalue);
								if(*field->data.float_value!=fvalue) return_value=FALSE;
								}
								break;
							case SHORTV:
							case SSHORTV:
								if(confld->inherit==TRUE)
								{
								FINDRSCGETSHORT(rsrc,temp,&shvalue);
								if(*field->data.short_value!=shvalue) return_value=FALSE;
								}
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								if(confld->inherit==TRUE)
								{
								FINDRSCGETINT(rsrc,temp,&ivalue);
								if(*field->data.integer_value!=ivalue) return_value=FALSE;
								}
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
		}
	}
	return(return_value);
}
short PowerAddBrowseBreakFunction(MakeBrowseList *BL)
{
	RDAaddsub *addsub=NULL;
	PowerAdd *POWERADD=NULL;
	RDAconfld *confld=NULL;
	int y,ivalue=0;
	short return_value=TRUE,shvalue=0;
	char *temp=NULL,cvalue=FALSE,*svalue=NULL;
	double fvalue=0.0;
	NRDfield *field=NULL;
	RDArsrc *rsrc=NULL;

	addsub=(RDAaddsub *)BL->funcargs;
	if(addsub!=NULL)
	{
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->parent_rsrc!=NULL)
		{
			rsrc=addsub->parent_rsrc;
		} else {
			rsrc=POWERADD->addrsrc;
		}
		for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
		{
			if(return_value==FALSE) break;
			if(confld->inherit==TRUE)
			{
				field=FLDNRD(addsub->fileno,confld->fldname);
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
								svalue=PP_EVALUATEstr(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
								if(svalue!=NULL) Rfree(svalue);
								break;
							case BOOLNS:
							case CHARACTERS:
								cvalue=PP_EVALUATEbol(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.string_value!=cvalue) return_value=FALSE;
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								fvalue=PP_EVALUATEdbl(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.float_value!=fvalue) return_value=FALSE;
								break;
							case SHORTV:
							case SSHORTV:
								shvalue=PP_EVALUATEsht(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.short_value!=shvalue) return_value=FALSE;
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								ivalue=PP_EVALUATEint(confld->conname,POWERADD->SubFunc,POWERADD->SubFuncArgs);
								if(*field->data.integer_value!=ivalue) return_value=FALSE;
								break;
							default:
							case BUTTONS:
								break;
						}
					} else {
						temp=Rmalloc(RDAstrlen(addsub->filename)+RDAstrlen(confld->fldname)+5);
						sprintf(temp,"[%s][%s]",addsub->filename,confld->fldname);
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
								FINDRSCGETSTRING(rsrc,temp,&svalue);
								if(RDAstrcmp(svalue,field->data.string_value)) return_value=FALSE;
								if(svalue!=NULL) Rfree(svalue);
								break;
							case BOOLNS:
							case CHARACTERS:
								FINDRSCGETCHAR(rsrc,temp,&cvalue);
								if(*field->data.string_value!=cvalue) return_value=FALSE;
								break;
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case SDECIMALV:
								FINDRSCGETDOUBLE(rsrc,temp,&fvalue);
								if(*field->data.float_value!=fvalue) return_value=FALSE;
								break;
							case SHORTV:
							case SSHORTV:
								FINDRSCGETSHORT(rsrc,temp,&shvalue);
								if(*field->data.short_value!=shvalue) return_value=FALSE;
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								FINDRSCGETINT(rsrc,temp,&ivalue);
								if(*field->data.integer_value!=ivalue) return_value=FALSE;
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
		}
	}
	return(return_value);
}
void PowerAddBrowseQuitFunction2(MakeBrowseList *BL)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDAaddsub *addsub=NULL;
	Browse_Exit *BE=NULL;

	if(BL!=NULL)
	{
		drsrc=BL->definelist;
		srsrc=BL->searchrsrc;
		brsrc=BL->mainrsrc;
		addsub=(RDAaddsub *)BL->funcargs;
		if(addsub!=NULL)
		{
			addsub->BL=NULL;
			if(addsub->parent_rsrc!=NULL) addsub->parent_rsrc=NULL;
		}
		if(drsrc!=NULL)
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
			drsrc=NULL;
		}
		if(srsrc!=NULL)
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
			srsrc=NULL;
		}
		BE=RETURN_BROWSE_EXIT(BL);
		if(BE!=NULL)
		{
			FreeBrowseExit(BE);
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			free_rsrc(brsrc);
			brsrc=NULL;
		}
	}
}
void PowerAddBrowseQuitFunction(MakeBrowseList *BL)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	RDAaddsub *addsub=NULL;
	RDAconfld *c=NULL;
	Browse_Exit *BE=NULL;
	char kill_browse=FALSE;
	int x;

	if(BL!=NULL)
	{
		drsrc=BL->definelist;
		srsrc=BL->searchrsrc;
		brsrc=BL->mainrsrc;
		addsub=(RDAaddsub *)BL->funcargs;
		if(addsub!=NULL)
		{
			if(addsub->confld!=NULL)
			{
				for(x=0,c=addsub->confld;x<addsub->num;++x,++c)
				{
					if(c->inherit==TRUE)
					{
						kill_browse=TRUE;
					}
				}
			} else {
				kill_browse=TRUE;
			}
			if(kill_browse==TRUE && FALSE)
			{
				addsub->BL=NULL;
			}
			if(addsub->parent_rsrc!=NULL) addsub->parent_rsrc=NULL;
		} else {
			kill_browse=TRUE;
		}
		if(drsrc!=NULL)
		{
			killwindow(drsrc);
			if(kill_browse==TRUE && FALSE)
			{
				free_rsrc(drsrc);
				drsrc=NULL;
			}
		}
		if(srsrc!=NULL)
		{
			killwindow(srsrc);
			if(kill_browse==TRUE && FALSE)
			{
				free_rsrc(srsrc);
				srsrc=NULL;
			}
		}
		if(kill_browse==TRUE && FALSE)
		{
			BE=RETURN_BROWSE_EXIT(BL);
			if(BE!=NULL)
			{
				FreeBrowseExit(BE);
			}
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			if(kill_browse==TRUE && FALSE)
			{
				free_rsrc(brsrc);
				brsrc=NULL;
			}
		}
	}
}
static void SetPowerAddNotInherited(RDAconfld *confld,PowerAdd *POWERADD,NRDfield *field)
{
	AddEditWdgt *EditWdgt=NULL;
	char *temp=NULL;

	temp=stripmodulename(confld->conname);
	if(temp!=NULL)
	{
	EditWdgt=FINDEDITWDGT(POWERADD,temp);
	if(EditWdgt!=NULL)
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
				if(EditWdgt->data.string_value!=NULL) Rfree(EditWdgt->data.string_value);
				EditWdgt->dlen=0;
				if(!isEMPTY(field->data.string_value))
				{
					EditWdgt->data.string_value=stralloc(field->data.string_value);
					EditWdgt->dlen=RDAstrlen(EditWdgt->data.string_value)+1;
				} else {
					EditWdgt->data.string_value=NULL;
				}
				if(EditWdgt->dlen>EditWdgt->len && EditWdgt->len>0)
				{
					EditWdgt->data.string_value[EditWdgt->len]=0;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
				*EditWdgt->data.string_value=*field->data.string_value;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*EditWdgt->data.float_value=*field->data.float_value;
				break;
			case SHORTV:
			case SSHORTV:
				*EditWdgt->data.short_value=*field->data.short_value;
				break;
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				*EditWdgt->data.integer_value=*field->data.integer_value;
				break;
			default:
			case BUTTONS:
				break;
		}		
	}	
	Rfree(temp);
	}
}
short PowerAddBrowseTestInherited(MakeBrowseList *BL,RDArmem *member)
{
	RDAaddsub *addsub=NULL;
	PowerAdd *POWERADD=NULL;
	RDAconfld *c=NULL;
	short return_value=FALSE;
	int x=0;
	short keyno=0;
	short fileno=(-1);
	short len=0;
	void *comptarget=NULL;
	void *origtarget=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd==TRUE)
	{
		prterr("DIAG PowerAddBrowseTestInherited().");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(BL!=NULL)
	{
		addsub=(RDAaddsub *)BL->funcargs;
		if(addsub!=NULL)
		{
			addsub->parent_rsrc=(RDArsrc *)member->parent;
			POWERADD=(PowerAdd *)addsub->parent;
			if(addsub->confld!=NULL)
			{
				for(x=0,c=addsub->confld;x<addsub->num;++x,++c)
				{
					if(c->inherit==TRUE)
					{
						return_value=TRUE;
					}
				}
			} else {
				return_value=FALSE;
			}
			if(return_value==FALSE)
			{
				return(FALSE);
			}
			if(addsub->keyno!=(-1) && addsub->keyno>0)
			{
				keyno=addsub->keyno;
			} else {
				keyno=1;
			}
			fileno=addsub->fileno;
			origtarget=GETNRDKEY(fileno,keyno,&len);
			if(BL->list!=NULL)
			{
				if(BL->list->key!=NULL && BL->list->num>0)
				{
					comptarget=BL->list->key[0];
					if(ADVEQLNRDKEYsec(fileno,keyno,comptarget,POWERADD->SubFunc,POWERADD->SubFuncArgs))
					{
						KEYNRD(fileno,keyno);
						return_value=FALSE;	
					} else {

						return_value=PowerAddBrowseBreakFunction(BL);
					}
				} else {
					return_value=FALSE;	
				}
			} else {
				return_value=FALSE;	
			}
			EQLNRDKEY(fileno,keyno,origtarget); /* put pointer back before comparing */
			if(origtarget!=NULL) Rfree(origtarget);
			return(!return_value);
		
		} 
		return(FALSE);
	}
	return(FALSE);
}
char PowerAddSubOKFunction(RDAaddsub *addsub,void *target)
{
	RDAconfld *c=NULL;
	PowerAdd *POWERADD=NULL;
	RDAconfld*confld=NULL;
	int x,y;
	NRDfield *field=NULL;
	short keyno=1;
	char kill_browse=FALSE;
#ifdef RADDOG_TEST
	char *temp=NULL;
#endif 
	RDArsrc *rsrc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd==TRUE)
	{
		prterr("DIAG PowerAddSubOKFunction().");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	if(addsub!=NULL)
	{
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->parent_rsrc!=NULL)
		{
			rsrc=addsub->parent_rsrc;
		} else {
			rsrc=POWERADD->addrsrc;
		}
		if(addsub->confld!=NULL)
		{
			for(x=0,c=addsub->confld;x<addsub->num;++x,++c)
			{
				if(c->inherit==TRUE) kill_browse=TRUE;
			}
		} else {
			kill_browse=TRUE;
		}
		if(addsub->keyno!=(-1) && addsub->keyno>0)
		{ 
			keyno=addsub->keyno;
		} else {
			keyno=1;
		}
/*
		if(!ADVEQLNRDKEYsec(addsub->fileno,keyno,target,POWERADD->SubFunc,POWERADD->SubFuncArgs)) 
*/
		if(!EQLNRDKEY(addsub->fileno,keyno,target))
		{
			for(y=0,confld=addsub->confld;y<addsub->num;++y,++confld)
			{
				if(confld->inherit==FALSE)
				{
					field=FLDNRD(addsub->fileno,
						confld->fldname);
					if(field!=NULL)
					{
						SetPowerAddNotInherited(confld,POWERADD,field);
					}
				}
			}
			IncVirRsrcstoFile(rsrc);
			GET_POWERADD_SUPPORTING(rsrc,POWERADD,POWERADD->SubFunc,POWERADD->SubFuncArgs,TRUE,FALSE);
			DefaultAddEditWdgts(rsrc,POWERADD,2,POWERADD->SubFunc,POWERADD->SubFuncArgs);
			SetAddEditWdgts(rsrc,POWERADD);
			xcomputeandupdateallSCRNvirtuals(rsrc,POWERADD->SubFunc,POWERADD->SubFuncArgs,FALSE,FALSE,NULL,POWERADD,__LINE__,__FILE__);
#ifdef RADDOG_TEST
			if(!isEMPTY(rsrc->input_focus))
			{
				temp=stralloc(rsrc->input_focus);
			}
#endif /* RADDOG_TEST */
			updateallrsrc(rsrc);
#ifdef RADDOG_TEST
			if(!isEMPTY(temp))
			{
				FINDRSCSETINPUTFOCUS(rsrc,temp);
				if(temp!=NULL) Rfree(temp);
			} 
#endif /* RADDOG_TEST */
			SetAllEditableAndSensitive(rsrc);
			run_changevalue_buttons(rsrc,POWERADD);
		}
	} else {
		kill_browse=TRUE;
	}
	return(kill_browse);
}
void PowerAddBrowseFreeFunction(MakeBrowseList *BL)
{
	RDAaddsub *addsub=NULL;
	Browse_Exit *BE=NULL;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd==TRUE)
	{
		prterr("DIAG PowerAddBrowseFreeFunction().");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(BL!=NULL)
	{
		addsub=(RDAaddsub *)BL->funcargs;
		if(addsub!=NULL)
		{
			drsrc=BL->definelist;
			srsrc=BL->searchrsrc;
			brsrc=BL->mainrsrc;
			if(drsrc!=NULL)
			{
				free_rsrc(drsrc);
				drsrc=NULL;
			}
			if(srsrc!=NULL)
			{
				free_rsrc(srsrc);
				srsrc=NULL;
			}
			BE=RETURN_BROWSE_EXIT(BL);
			if(BE!=NULL)
			{
				FreeBrowseExit(BE);
			}
			if(brsrc!=NULL)
			{
				free_rsrc(brsrc);
				brsrc=NULL;
			}
		}
	}
}
void PowerAddBrowseOKFunction(MakeBrowseList *BL,void *target)
{
	RDAaddsub *addsub=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd==TRUE)
	{
		prterr("DIAG PowerAddBrowseOKFunction().");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(BL!=NULL)
	{
		addsub=(RDAaddsub *)BL->funcargs;
		if(addsub!=NULL)
		{
/*
			kill_browse=PowerAddSubOKFunction(addsub,target);
*/
			PowerAddSubOKFunction(addsub,target);
			if(BL->definelist!=NULL)
			{
				killwindow(BL->definelist);
			}
			if(BL->searchrsrc!=NULL)
			{
				killwindow(BL->searchrsrc);
			}
			if(BL->mainrsrc!=NULL)
			{
				killwindow(BL->mainrsrc);
			}
		}
	}
}
void PowerAddSubNextFunction(RDArmem *member,RDAaddsub *addsub)
{
	PowerAdd *POWERADD=NULL;
	short keyno=1;
	short fileno=(-1);
	void *target=NULL;
	short len=0;

	if(addsub!=NULL)
	{
		addsub->parent_rsrc=(RDArsrc *)member->parent;
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->keyno!=(-1) && addsub->keyno>0)
		{
			keyno=addsub->keyno;
		} else {
			keyno=1;
		}
		fileno=addsub->fileno;
		PowerAddSupInitFunction(addsub);
		if(ADVNXTNRDsec(fileno,keyno,POWERADD->SubFunc,POWERADD->SubFuncArgs))
		{
			KEYNRD(fileno,keyno);
		} else {
			target=GETNRDKEY(fileno,keyno,&len);
			PowerAddSubOKFunction(addsub,target);
			if(target!=NULL) Rfree(target);
		}
	}
}
void PowerAddSubPrevFunction(RDArmem *member,RDAaddsub *addsub)
{
	PowerAdd *POWERADD=NULL;
	short keyno=1;
	short fileno=(-1);
	void *target=NULL;
	short len=0;

	if(addsub!=NULL)
	{
		addsub->parent_rsrc=(RDArsrc *)member->parent;
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->keyno!=(-1) && addsub->keyno>0)
		{
			keyno=addsub->keyno;
		} else {
			keyno=1;
		}
		fileno=addsub->fileno;
		PowerAddSupInitFunction(addsub);
		if(ADVPRVNRDsec(fileno,keyno,POWERADD->SubFunc,POWERADD->SubFuncArgs))
		{
			KEYNRD(fileno,keyno);
		} else {
			target=GETNRDKEY(fileno,keyno,&len);
			PowerAddSubOKFunction(addsub,target);
			if(target!=NULL) Rfree(target);
		}
	}
}
void PowerAddBrowseFunction(RDArmem *member,RDAaddsub *addsub)
{
	PowerAdd *POWERADD=NULL;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;
	short keyno=1;
	short kill_browse=TRUE;

	if(addsub!=NULL)
	{
		addsub->parent_rsrc=(RDArsrc *)member->parent;
		POWERADD=(PowerAdd *)addsub->parent;
		if(addsub->BL!=NULL)
		{ 
			if(addsub->smart_browse==TRUE)
			{
				kill_browse=PowerAddBrowseTestInherited(addsub->BL,member);
				if(kill_browse==FALSE)
				{
					if(APPmakescrn(addsub->BL->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
					{
						PowerAddBrowseQuitFunction2(addsub->BL);
					}
					return;
				} else {
					PowerAddBrowseFreeFunction(addsub->BL);
					addsub->BL=NULL;
				}
			} else {
				PowerAddBrowseFreeFunction(addsub->BL);
				addsub->BL=NULL;
			}
		}
		if(!isEMPTY(addsub->browse_screen) && !isEMPTY(addsub->define_list))
		{
			brsrc=RDArsrcNEW(POWERADD->module,addsub->browse_screen);
			GET_SCREEN_VIRTUALS(brsrc,0);
			SCRNvirtual2rsrc(brsrc);
			
			drsrc=RDArsrcNEW(POWERADD->module,addsub->define_list);
			addDFincvir(drsrc,addsub->module,addsub->filename,NULL,addsub->fileno);
			GET_SCREEN_VIRTUALS(drsrc,1);
			srsrc=RDArsrcNEW(POWERADD->module,addsub->search_browse);
			addDFincvir(srsrc,addsub->module,addsub->filename,NULL,addsub->fileno);
			GET_SCREEN_VIRTUALS(srsrc,2);
			SCRNvirtual2rangersrc(srsrc);
			brsrc->scn=RDAscrnNEW(brsrc->module,brsrc->screen);
			findscrnbin(brsrc->scn);
			MakeBrowseListTotals(brsrc,addsub->fileno,drsrc);
			if(addsub->keyno!=(-1) && addsub->keyno>0)
			{
				keyno=addsub->keyno;
			} else { 
				keyno=1;
			}
			switch(addsub->smart_browse)
			{
				default:
				case 0:
				case 1:
					addsub->BL=xbrowse(addsub->fileno,keyno,brsrc,srsrc,drsrc,PowerAddBrowseInitFunction,PowerAddBrowseBreakFunction,PowerAddBrowseSelectFunction,PowerAddBrowseQuitFunction,PowerAddBrowseOKFunction,NULL,addsub,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
					break;
				case 2:
					addsub->BL=xbrowse(addsub->fileno,keyno,brsrc,srsrc,drsrc,PowerAddBrowseInitFunction,PowerAddBrowseBreakFunction,PowerAddBrowseSelectFunction,PowerAddBrowseQuitFunction,PowerAddBrowseOKFunction,NULL,addsub,TRUE,NULL,0,0,0,0,NULL,0,2,__LINE__,__FILE__);
					break;
				case 3:
					addsub->BL=xbrowse(addsub->fileno,keyno,brsrc,srsrc,drsrc,PowerAddBrowseInitFunctionLPM,PowerAddBrowseBreakFunctionLPM,PowerAddBrowseSelectFunction,PowerAddBrowseQuitFunction,PowerAddBrowseOKFunction,NULL,addsub,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
					break;
			}	
		}
	}
}
void xMakePowerAddButtons(RDArsrc *mainrsrc,PowerAdd *POWERADD,
	void (*quitfunc)(...),int line,char *file)
{
	int x,y;
	MaintainButton *button=NULL;
	char *bname=NULL;
	RDAaddsub *addsub=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG MakePowerAddButtons at line [%d] program [%s].",
			line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		if(POWERADD->buttons!=NULL)
		{
			for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
			{
				if(!RDAstrcmp(button->progname,"RUNREPORT") || !RDAstrcmp(button->progname,"runreport"))
				{
					button->func=quitfunc;
				} else  if(!RDAstrcmp(button->progname,"RUNPOWERADD") || !RDAstrcmp(button->progname,"runpoweradd"))
				{
					button->func=quitfunc;
				} else  if(!RDAstrcmp(button->progname,"MULTIPOWERADD") || !RDAstrcmp(button->progname,"multipoweradd"))
				{
					button->func=quitfunc;
				} else  if(!RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"runmaintain"))
				{
					button->func=quitfunc;
				} else if(!RDAstrcmp(button->progname,"RMDIR") || !RDAstrcmp(button->progname,"rmdir"))
				{
					button->func=quitfunc;
				} else if(!RDAstrcmp(button->progname,"MKDIR") || !RDAstrcmp(button->progname,"mkdir"))
				{
					button->func=quitfunc;
				} else if(!RDAstrcmp(button->progname,"COPYFILE") || !RDAstrcmp(button->progname,"copyfile"))
				{
				} else if(!RDAstrcmp(button->progname,"MOVEFILE") || !RDAstrcmp(button->progname,"movefile"))
				{
				} else if(!RDAstrcmp(button->progname,"DISPLAYRELATIVEFILE") || !RDAstrcmp(button->progname,"displayrelativefile"))
				{
				} else if(!RDAstrcmp(button->progname,"DISPLAYFILE") || !RDAstrcmp(button->progname,"displayfile"))
				{
				} else if(!RDAstrcmp(button->progname,"UNLINK") || !RDAstrcmp(button->progname,"unlink"))
				{
					button->func=quitfunc;
				} else if(!RDAstrcmp(button->progname,"SETCOOKIE") || !RDAstrcmp(button->progname,"setcookie"))
				{
					button->func=quitfunc;
				} else if(!RDAstrcmp(button->progname,"PUTENV") || !RDAstrcmp(button->progname,"putenv"))
				{
					button->func=quitfunc;
				} else if(!RDAstrcmp(button->progname,"DOHTTP") || !RDAstrcmp(button->progname,"dohttp"))
				{
					button->func=quitfunc;
				}
				addbtnrsrc(mainrsrc,button->buttonname,TRUE,
					PowerAddButtonFunction,button);
			}
		}
		if(POWERADD->subord!=NULL)
		{
			for(y=0,addsub=POWERADD->subord;y<POWERADD->subnum;++y,++addsub)
			{
				if(addsub->browse==TRUE)
				{
					if(!isEMPTY(addsub->browse_screen) && !isEMPTY(addsub->define_list))
					{
						if(!isEMPTY(addsub->button_name))
						{
							bname=stralloc(addsub->button_name);
						} else {
							bname=Rmalloc(strlen(addsub->module)+strlen(addsub->filename)+25);
							sprintf(bname,"BROWSE %s %s %d",(addsub->module!=NULL?addsub->module:""),(addsub->filename!=NULL?addsub->filename:""),(y+1));
						}
						RMEMaddbtnrsrc(mainrsrc,bname,TRUE,PowerAddBrowseFunction,addsub);
						if(bname!=NULL) Rfree(bname);
					}
					if(!isEMPTY(addsub->button_name))
					{
						bname=Rmalloc(strlen(addsub->button_name)+6);
						sprintf(bname,"NEXT %s",addsub->button_name);
					} else {
						bname=Rmalloc(strlen(addsub->module)+strlen(addsub->filename)+25);
						sprintf(bname,"NEXT %s %s %d",(addsub->module!=NULL?addsub->module:""),(addsub->filename!=NULL?addsub->filename:""),(y+1));
					}
					RMEMaddbtnrsrc(mainrsrc,bname,TRUE,PowerAddSubNextFunction,addsub);
					if(bname!=NULL) Rfree(bname);
					if(!isEMPTY(addsub->button_name))
					{
						bname=Rmalloc(strlen(addsub->button_name)+11);
						sprintf(bname,"PREVIOUS %s",addsub->button_name);
					} else {
						bname=Rmalloc(strlen(addsub->module)+strlen(addsub->filename)+30);
						sprintf(bname,"PREVIOUS %s %s %d",(addsub->module!=NULL?addsub->module:""),(addsub->filename!=NULL?addsub->filename:""),(y+1));
					}
					RMEMaddbtnrsrc(mainrsrc,bname,TRUE,PowerAddSubPrevFunction,addsub);
					if(bname!=NULL) Rfree(bname);
				}
			}
		}
	}
}
APPlib *xPowerAdd2ClearPassKey(MaintainButton *button,
	int line,char *file)
{
	APPlib *tmp=NULL;
	char *temp=NULL;
	short x=0;
	ButtonField *btnfld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG PowerAdd2ClearPassKey at line [%d] program [%s].",line,file);
	}	
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=APPlibNEW();
	temp=Rmalloc(9+RDAstrlen(button->keyname));
	sprintf(temp,"KEYNAME=%s",button->keyname);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	if(button->bf!=NULL)
	{
		for(x=0,btnfld=button->bf;x<button->num;++x,++btnfld)
		{
			temp=Rmalloc(RDAstrlen(btnfld->ename)+3);
			sprintf(temp,"%s=",btnfld->ename);
			addAPPlib(tmp,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG PowerAdd2ClearPassKey at line [%d] program [%s].",
			line,file);
		SEEAPPlib(tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(tmp);
}
APPlib *xPowerAdd2PassKey(MaintainButton *button,PowerAdd *POWERADD,
	int line,char *file)
{
	APPlib *tmp=NULL;
	char *temp3=NULL,*temp2=NULL,*temp1=NULL,*temp=NULL;
	int x=0,y=0,z=0,l=0;
	ButtonField *btnfld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG PowerAdd2PassKey at line [%d] program [%s].",line,file);
	}	
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(button!=NULL)
	{
		tmp=APPlibNEW();
		temp=Rmalloc(RDAstrlen(button->keyname)+9);
		sprintf(temp,"KEYNAME=%s",(button->keyname!=NULL ? button->keyname:""));
		addAPPlib(tmp,temp);
		if(temp!=NULL) Rfree(temp);
		for(x=0,btnfld=button->bf;x<button->num;++x,++btnfld)
		{
			temp1=stralloc(btnfld->rname);
			if(POWERADD->SubFunc!=NULL)
			{
/*lint -e746*/
				POWERADD->SubFunc(&temp1,POWERADD->SubFuncArgs);
/*lint +e746*/
			}
			z=RDAstrlen(temp1);
			for(l=0,temp=temp1;*temp;++temp)
			{
				if(l==0)
				{
					if((*temp!='"') && (*temp!='('))
					{
						++l;
					}
				} else if(l==(z-1))
				{
					if(*temp!='"')
					{
						++l;
					}
				} else {
					if(*temp!='"')
					{
						++l;
					} else {
						l+=2;
					}
				}
			}
			temp3=Rmalloc(l+1);
			temp2=temp3;
			for(y=0,temp=temp1;*temp;++temp,++y)
			{
				if(y==0)
				{
					if((*temp!='"') && (*temp!='('))
					{
						*temp2=*temp;
						++temp2;
					}
				} else if(y==(z-1))
				{
					if(*temp!='"')
					{
						*temp2=*temp;
						++temp2;
					}
				} else {
					if(*temp!='"')
					{
						*temp2=*temp;
						++temp2;
					} else {
						*temp2='\\';
						++temp2;
						*temp2=*temp;
						++temp2;
					}
				}
			}
			temp=Rmalloc(RDAstrlen(btnfld->ename)+RDAstrlen(temp3)+2);
			sprintf(temp,"%s=%s",(btnfld->ename!=NULL?btnfld->ename:""),
				(temp3!=NULL?temp3:""));
			addAPPlib(tmp,temp);
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			if(temp3!=NULL) Rfree(temp3);
		}
		if(tmp->numlibs<1) freeapplib(tmp);
	}
	return(tmp);
}
void PowerAddPutEnv(MaintainButton *button,PowerAdd *POWERADD,short which)
{
	ButtonField *btnfld=NULL,*VarName=NULL,*VarValue=NULL;
	char *temp3=NULL,*temp2=NULL,*temp1=NULL,*temp=NULL;
	char *name=NULL,*value=NULL;
	int x=0,y=0,z=0,l=0;

	if(which) RDA_NOGUI=TRUE;
	if(button!=NULL)
	{
		if(button->bf!=NULL)
		{
		for(x=0,btnfld=button->bf;x<button->num;++x,++btnfld)
		{
			if((which==FALSE) && (!RDAstrcmp(btnfld->rname,"[COOKIE NAME]"))) VarName=btnfld;
			if((which==FALSE) && (!RDAstrcmp(btnfld->rname,"[COOKIE VALUE]"))) VarValue=btnfld;
			if((which==TRUE) && (!RDAstrcmp(btnfld->rname,"[VARIABLE NAME]"))) VarName=btnfld;
			if((which==TRUE) && (!RDAstrcmp(btnfld->rname,"[VARIABLE VALUE]"))) VarValue=btnfld;
		}
		if(VarName!=NULL && VarValue!=NULL)
		{
		temp1=stralloc(VarName->rname);
		if(POWERADD->SubFunc!=NULL)
		{
/*lint -e746*/
			POWERADD->SubFunc(&temp1,POWERADD->SubFuncArgs);
/*lint +e746*/
		}
		z=RDAstrlen(temp1);
		for(l=0,temp=temp1;*temp;++temp)
		{
			if(l==0)
			{
				if((*temp!='"') && (*temp!='('))
				{
					++l;
				}
			} else if(l==(z-1))
			{
				if(*temp!='"')
				{
					++l;
				}
			} else {
				if(*temp!='"')
				{
					++l;
				} else {
					l+=2;
				}
			}
		}
		temp3=Rmalloc(l+1);
		temp2=temp3;
		for(y=0,temp=temp1;*temp;++temp,++y)
		{
			if(y==0)
			{
				if((*temp!='"') && (*temp!='('))
				{
					*temp2=*temp;
					++temp2;
				}
			} else if(y==(z-1))
			{
				if(*temp!='"')
				{
					*temp2=*temp;
					++temp2;
				}
			} else {
				if(*temp!='"')
				{
					*temp2=*temp;
					++temp2;
				} else {
					*temp2='\\';
					++temp2;
					*temp2=*temp;
					++temp2;
				}
			}
		}
		name=stralloc(temp3);
		if(temp1!=NULL) Rfree(temp1);
		if(temp3!=NULL) Rfree(temp3);
		y=0;l=0;
		temp1=stralloc(VarValue->rname);
		if(POWERADD->SubFunc!=NULL)
		{
/*lint -e746*/
			POWERADD->SubFunc(&temp1,POWERADD->SubFuncArgs);
/*lint +e746*/
		}
		z=RDAstrlen(temp1);
		for(l=0,temp=temp1;*temp;++temp)
		{
			if(l==0)
			{
				if((*temp!='"') && (*temp!='('))
				{
					++l;
				}
			} else if(l==(z-1))
			{
				if(*temp!='"')
				{
					++l;
				}
			} else {
				if(*temp!='"')
				{
					++l;
				} else {
					l+=2;
				}
			}
		}
		temp3=Rmalloc(l+1);
		temp2=temp3;
		for(y=0,temp=temp1;*temp;++temp,++y)
		{
			if(y==0)
			{
				if((*temp!='"') && (*temp!='('))
				{
					*temp2=*temp;
					++temp2;
				}
			} else if(y==(z-1))
			{
				if(*temp!='"')
				{
					*temp2=*temp;
					++temp2;
				}
			} else {
				if(*temp!='"')
				{
					*temp2=*temp;
					++temp2;
				} else {
					*temp2='\\';
					++temp2;
					*temp2=*temp;
					++temp2;
				}
			}
		}
		value=stralloc(temp3);
		if(temp1!=NULL) Rfree(temp1);
		if(temp3!=NULL) Rfree(temp3);
		} else {
		}
		} else {
		}
	}
	Skip_OpenRDA_Cookies=TRUE;
	if(!which)
	{
		WT_RDA_SetEnv(name,value,__LINE__,__FILE__);
	} else {
		setenv(name,value,TRUE);
	}
	Skip_OpenRDA_Cookies=FALSE;
	if(which) RDA_NOGUI=FALSE;
	if(name!=NULL) Rfree(name);
	if(value!=NULL) Rfree(value);
}
void PowerAddSetCookie(MaintainButton *button,PowerAdd *POWERADD)
{
	PowerAddPutEnv(button,POWERADD,FALSE);
}
void PowerAddPutEnv(MaintainButton *button,PowerAdd *POWERADD)
{
	PowerAddPutEnv(button,POWERADD,TRUE);
}
void PowerAddButtonFunction(RDArsrc *mainrsrc,MaintainButton *button)
{
	APPlib *envpx=NULL,*envpx1=NULL,*newargs=NULL;
	int x,ret_int=0;
	char blank_envpx1=FALSE;
	char *value=NULL,*ret_string=NULL;
	char *temp=NULL;
	PowerAdd *POWERADD=NULL;

	if(button!=NULL)
	{
	POWERADD=(PowerAdd *)button->parent;
	if(POWERADD!=NULL)
	{
		envpx=PowerAdd2PassKey(button,POWERADD);
		envpx1=PowerAdd2ClearPassKey(button);
		newargs=APPlibNEW();
		for(x=0;x<button->args->numlibs;++x)
		{
			addAPPlib(newargs,button->args->libs[x]);
		}
		if(button->evalargs==TRUE)
		{
			for(x=0;x<newargs->numlibs;++x)
			{
				if(!isEMPTY(newargs->libs[x]))
				{
					value=EVALUATEstr(newargs->libs[x],POWERADD->SubFunc,POWERADD->SubFuncArgs);
					if(value==NULL) value=stralloc("");
					if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
					newargs->libs[x]=stralloc(value);
					if(value!=NULL) Rfree(value);
				} else {
					if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
					newargs->libs[x]=stralloc("");
				}
			}
		}
		if(!RDAstrcmp(button->progname,"RUNREPORT") || !RDAstrcmp(button->progname,"runreport"))
		{

/* This removes environment before it can be read when there is a range screen 

			blank_envpx1=TRUE;
*/
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			RUNREPORTADV2(newargs->libs[0],newargs->libs[1],NULL,NULL,
				TRUE,2,NULL,button->func,mainrsrc,(void *)POWERADD,(POWERADD->level+1));
		} else if(!RDAstrcmp(button->progname,"RUNPOWERADD") || !RDAstrcmp(button->progname,"runpoweradd"))
		{
			blank_envpx1=TRUE;
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(POWERADD->level+1),button->func,mainrsrc,(void *)POWERADD,TRUE);
		} else if(!RDAstrcmp(button->progname,"MULTIPOWERADD") || !RDAstrcmp(button->progname,"multipoweradd"))
		{
			blank_envpx1=TRUE;
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			MULTIPOWERADDADV(newargs->libs[0],newargs->libs[1],(POWERADD->level+1),button->func,mainrsrc,(void *)POWERADD,TRUE);
		} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"runmaintain"))
		{
			blank_envpx1=TRUE;
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(POWERADD->level+1),button->func,mainrsrc,(void *)POWERADD,TRUE);
		} else if(!RDAstrcmp(button->progname,"RMDIR") || !RDAstrcmp(button->progname,"rmdir"))
		{
			blank_envpx1=TRUE;
			if(newargs!=NULL)
			{
				for(x=0;x<newargs->numlibs;++x)
				{
					if(!isEMPTY(newargs->libs[x])) rmdir(newargs->libs[x]);
				}
			}
		} else if(!RDAstrcmp(button->progname,"MKDIR") || !RDAstrcmp(button->progname,"mkdir"))
		{
			blank_envpx1=TRUE;
			if(newargs!=NULL)
			{
				for(x=0;x<newargs->numlibs;++x)
				{
					if(!isEMPTY(newargs->libs[x])) 
					{
						ret_string=RDAmkdir(newargs->libs[x],MODE_RWXRWX___,&ret_int);
						if(!isEMPTY(ret_string))
						{
							if(ret_int!=EEXIST)
							{
								prterr("Error:  Cannot create directory [%s] because of [%s].",newargs->libs[x],ret_string);
							} 
						}
						if(ret_string!=NULL) Rfree(ret_string);
						ret_string=RDAchmod(newargs->libs[x],MODE_RWXRWX___,&ret_int);
						if(ret_string!=NULL) Rfree(ret_string);
					}
				}
			}
		} else if(!RDAstrcmp(button->progname,"SETCOOKIE") || !RDAstrcmp(button->progname,"setcookie"))
		{
			PowerAddSetCookie(button,POWERADD);
		} else if(!RDAstrcmp(button->progname,"PUTENV") || !RDAstrcmp(button->progname,"putenv"))
		{
			PowerAddPutEnv(button,POWERADD);
		} else if(!RDAstrcmp(button->progname,"UNLINK") || !RDAstrcmp(button->progname,"unlink"))
		{
			blank_envpx1=TRUE;
			if(newargs!=NULL)
			{
				for(x=0;x<newargs->numlibs;++x)
				{
					if(!isEMPTY(newargs->libs[x])) unlink(newargs->libs[x]);
				}
			}
		} else if(!RDAstrcmp(button->progname,"COPYFILE") || !RDAstrcmp(button->progname,"copyfile"))
		{
			blank_envpx1=TRUE;
			if(newargs->numlibs>1)
			{
				RDACopyFile(newargs->libs[0],newargs->libs[1]);
			}
		} else if(!RDAstrcmp(button->progname,"MOVEFILE") || !RDAstrcmp(button->progname,"movefile"))
		{
			blank_envpx1=TRUE;
			if(newargs->numlibs>1)
			{
				RDAMoveFile(newargs->libs[0],newargs->libs[1]);
			}
		} else if(!RDAstrcmp(button->progname,"DISPLAYRELATIVEFILE") || !RDAstrcmp(button->progname,"displayrelativefile"))
		{
			blank_envpx1=TRUE;
			if(newargs->numlibs>0)
			{
				DisplayRelativeFile(newargs->libs[0]);
			}
		} else if(!RDAstrcmp(button->progname,"DISPLAYFILE") || !RDAstrcmp(button->progname,"displayfile") || !RDAstrcmp(button->progname,"doarchive"))
		{
			blank_envpx1=TRUE;
			if(newargs->numlibs>0)
			{
				DisplayFile(newargs->libs[0]);
			}
		} else if(!RDAstrcmp(button->progname,"DOHTTP") || !RDAstrcmp(button->progname,"dohttp")) 
		{
			blank_envpx1=TRUE;
			if(newargs->numlibs>0)
			{
				GUI_OpenURL(newargs->libs[0]);
			}
		} else {
			if(envpx!=NULL)
			{
				for(x=0;x<envpx->numlibs;++x)
				{
					RDA_PutEnv(envpx->libs[x]);
				}
			}
			if(button->type==0)
			{
				ADVExecuteProgram(button->progname,newargs,envpx);
			} else {
				ADVExecute2Program(button->progname,newargs,envpx);
			}
		}
		if(envpx1!=NULL)
		{
			if(blank_envpx1==TRUE)
			{
				for(x=0;x<envpx1->numlibs;++x)
				{
					temp=RDAstrstr(envpx1->libs[x],"=");
					if(temp!=NULL)
					{
						*temp=0;
					}
					RDA_UnSetEnv(envpx1->libs[x]);
				}
			}
			freeapplib(envpx1);
		}
		if(newargs!=NULL) freeapplib(newargs);
	} else {
		prterr("Error in PowerAddButtonFunction.");
	}
	}
}
short xOPEN_POWERADD_FILES(PowerAdd *POWERADD,int line,char *file)
{
	RDAaddsub *addsub;
	AddEditWdgt *EditWdgt=NULL;
	AddWrite *Write=NULL,*pWrite=NULL;
	int x,y,z;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG OPEN_POWERADD_FILES at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
		{
			if((addsub->fileno=APPOPNNRD(addsub->module,addsub->filename,TRUE,TRUE))==(-1)) return(FALSE);
			if(!isEMPTY(addsub->subkeyname)) addsub->keyno=KEYNUMBER(addsub->fileno,addsub->subkeyname);
			if((addsub->keyno==(-1)) || (addsub->keyno<1)) addsub->keyno=1;
		}
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
			{
				if(EditWdgt->id_mgt==TRUE)
				{
					if((EditWdgt->fileno=APPOPNNRD(EditWdgt->module,EditWdgt->filename,TRUE,TRUE))==(-1)) return(FALSE);
				}
			}
		}
		if(POWERADD->Writes!=NULL)
		{
			for(y=0,Write=POWERADD->Writes;y<POWERADD->numWrites;++y,++Write)
			{
				Write->fileno=(-1);
			}
		}
		if(POWERADD->Writes!=NULL && (POWERADD->save_type>0 && POWERADD->save_type<4))
		{
			for(y=0,Write=POWERADD->Writes;y<POWERADD->numWrites;++y,++Write)
			{
				Write->fileno=(-1);
				Write->closefile=FALSE;
				if(POWERADD->subord!=NULL)
				{
					for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
					{
						if(!RDAstrcmp(addsub->module,Write->module) && !RDAstrcmp(addsub->filename,Write->filename)) break;
					}
					if(x<POWERADD->subnum)
					{
						Write->closefile=FALSE;
						Write->zerofile=FALSE;
						Write->fileno=addsub->fileno;
					}
				}
				if(Write->fileno==(-1))
				{
					for(z=0,pWrite=POWERADD->Writes;z<y;++z,++pWrite)
					{
						if(!RDAstrcmp(pWrite->module,Write->module) && !RDAstrcmp(pWrite->filename,Write->filename)) break;
					}
					if(z<y)
					{	
						Write->closefile=FALSE;
						Write->zerofile=TRUE;
						Write->fileno=pWrite->fileno;
					}
				}
				if(Write->fileno==(-1))
				{
					Write->closefile=TRUE;
					Write->zerofile=TRUE;
					if((Write->fileno=APPOPNNRD(Write->module,Write->filename,TRUE,TRUE))==(-1)) return(FALSE);
				}
			}
		}
		return(TRUE);
	} else {
		prterr("Error PowerAdd is NULL at line [%d] program [%s].",
			line,file);
		return(FALSE);
	}
}
void xCLOSE_POWERADD_FILES(PowerAdd *POWERADD,int line,char *file)
{
	RDAaddsub *addsub;
	AddEditWdgt *EditWdgt=NULL;
	AddWrite *Write=NULL;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagadd)
	{
		prterr("DIAG CLOSE_POWERADD_FILES at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(POWERADD!=NULL)
	{
		for(x=0,addsub=POWERADD->subord;x<POWERADD->subnum;++x,++addsub)
		{
			if(addsub->fileno!=(-1)) CLSNRD(addsub->fileno);
		}
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
			{
				if(EditWdgt->id_mgt==TRUE)
				{
					if(EditWdgt->fileno!=(-1)) CLSNRD(EditWdgt->fileno);
				}
			}
		}
		if(POWERADD->save_type>0 && POWERADD->save_type<4)
		{
			if(POWERADD->Writes!=NULL)
			{
				for(x=0,Write=POWERADD->Writes;x<POWERADD->numWrites;++x,++Write)
				{
					if((Write->closefile==TRUE) && (Write->fileno!=(-1))) CLSNRD(Write->fileno);
				}
			}
		}
	}
}
