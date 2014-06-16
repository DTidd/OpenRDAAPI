#include<mix.hpp>

RangeScreen *RangeScreenNew(RDArsrc *rsrc)
{
	RangeScreen *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RangeScreenNew Allocating Range Screen Structure for Range Screen [%s] [%s].",rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RangeScreen));
	tmp->module=stralloc(rsrc->module);
	tmp->screen=stralloc(rsrc->screen);
	tmp->rsrc=rsrc;
	tmp->num=0;
	tmp->frs=NULL;
	return(tmp);
}
void xFreeRangeScreen(RangeScreen *r,int line,char *file)
{
	int x;
	FileRangeStruct *frs;

	if(r==NULL) return;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG FreeRangeScreen Freeing Range Screen Structure for Screen [%s] [%s] at line [%d] program [%s].",r->rsrc->module,r->rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->module!=NULL) Rfree(r->module);
		if(r->screen!=NULL) Rfree(r->screen);
		if(r->frs!=NULL)
		{
			for(x=0,frs=r->frs;x<r->num;++x,++frs)
			{
				if(frs->filename!=NULL) Rfree(frs->filename);
				if(frs->fieldname!=NULL) Rfree(frs->fieldname);
				switch(frs->field_type)
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
					case BOOLNS:
					case CHARACTERS:
						if(frs->from.string_value!=NULL) 
							Rfree(frs->from.string_value);
						if(frs->to.string_value!=NULL)
							Rfree(frs->to.string_value);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						if(frs->from.float_value!=NULL) 
							Rfree(frs->from.float_value);
						if(frs->to.float_value!=NULL)
							Rfree(frs->to.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(frs->from.short_value!=NULL) 
							Rfree(frs->from.short_value);
						if(frs->to.short_value!=NULL)
							Rfree(frs->to.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(frs->from.integer_value!=NULL) 
							Rfree(frs->from.integer_value);
						if(frs->to.integer_value!=NULL)
							Rfree(frs->to.integer_value);
						break;
					default:
						prterr("Error field type [%d] is invalid for file [%s] field [%s].",frs->field_type,(frs->filename!=NULL ? frs->filename:""),(frs->fieldname!=NULL ? frs->fieldname:""));
						break;
				}
			}
			Rfree(r->frs);
		}
		Rfree(r);
		r=NULL;
	}
}
void setrangeto2fromcb(RDArmem *member)
{
	char *temp=NULL,*temp1=NULL;

	readmember(member);
	temp=Rmalloc(RDAstrlen(member->rscrname));
	temp1=member->rscrname+5;
	sprintf(temp,"TO %s",temp1);
	switch(member->field_type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case PLAINTEXT:
		case PHONE:
		case SOCSECNUM:
		case CUSTOMTYPE:
		case ZIPCODE:
		case EXPENDITURE:
		case REVENUE:
		case BEGINNINGBALANCE:
		case BALANCESHEET:
		case OPTIONALFIELDS:
			FINDRSCSETSTRING(member->parent,temp,member->value.string_value);
			break;
		case BOOLNS:
		case CHARACTERS:
			FINDRSCSETCHAR(member->parent,temp,*member->value.string_value);
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case SDECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
			FINDRSCSETDOUBLE(member->parent,temp,*member->value.float_value);
			break;
		case SHORTV:
		case SSHORTV:
			FINDRSCSETSHORT(member->parent,temp,*member->value.short_value);
			break;
		case PROGRESS_BAR:
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			FINDRSCSETINT(member->parent,temp,*member->value.integer_value);
			break;
		default:
			break;
	}
	updatersrc(member->parent,temp);
	FINDRSCSETINPUTFOCUS((RDArsrc *)member->parent,temp);
	if(temp!=NULL) Rfree(temp);
}
void xReadRangeScreen(short fileno,RangeScreen *rs,short type,
	int line,char *file)
{
	NRDfield *field=NULL;
	short z,numflds=0;
	int x;
	char *filename=NULL,*fieldname=NULL;
	FileRangeStruct *frs=NULL;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG ReadRangeScreen Type [%d] for file [%d] [%s] Screen [%s] [%s] at line [%d] program [%s].",type,fileno,FILENAME(fileno),rs->module,rs->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	if((numflds=NUMFLDS(fileno))==(-1))
	{
		prterr("Error cannot get fields for file [%d] [%s].",fileno,FILENAME(fileno));
		return;
	}
	filename=FILENAME(fileno);
	for(z=0;z<numflds;++z)
	{
		field=FLDNUM(fileno,z);
		if(field!=NULL)
		{
			frs=NULL;
			if(rs->frs!=NULL)
			{
				rs->frs=Rrealloc(rs->frs,(rs->num+1)*
					sizeof(FileRangeStruct));
			} else {
				rs->frs=Rmalloc(sizeof(FileRangeStruct));
				rs->num=0;
			}
			frs=rs->frs+rs->num;
			frs->visible=FALSE;
			frs->filename=stralloc(filename);
			frs->fieldname=stralloc(field->name);
			frs->range=FALSE;
			++rs->num;
			frs->field_type=field->type;
			frs->field_len=field->len;
			if(field->type!=BOOLNS)
			{
				if(!type)
				{
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+14+RDAstrlen(FILENAME(fileno)));
					sprintf(fieldname,"RANGE ON [%s][%s]",FILENAME(fileno),field->name);
				} else {
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+12);
					sprintf(fieldname,"RANGE ON %s",field->name);
				}
				x=FINDRSC(rs->rsrc,fieldname);
				if(x!=(-1)) 
				{
					member=rs->rsrc->rscs+x;
					frs->range=*member->value.string_value;
					if(member->w!=NULL) frs->visible=TRUE;
				} else frs->range=FALSE;
				if(fieldname!=NULL) Rfree(fieldname);
			}
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case PLAINTEXT:
				case SOCSECNUM:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case DATES:
				case TIMES:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					frs->from.string_value=NULL;
					frs->to.string_value=NULL;
					if(frs->range)
					{
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+
								10+RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"FROM [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
							sprintf(fieldname,"FROM %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							if(!isEMPTY(member->value.string_value))
							{
								frs->from.string_value=
									stralloc(member->value.string_value);
							}
							if(member->w!=NULL) frs->visible=TRUE;
						}
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+
								8+RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"TO [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
							sprintf(fieldname,"TO %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							if(!isEMPTY(member->value.string_value))
							{
								frs->to.string_value=
									stralloc(member->value.string_value);
							}
							if(member->w!=NULL) frs->visible=TRUE;
						}
					}
					break;
				case LONGV:
				case SLONGV:
					frs->from.integer_value=Rmalloc(sizeof(int));
					*frs->from.integer_value=0;
					frs->to.integer_value=Rmalloc(sizeof(int));
					*frs->to.integer_value=0;
					if(frs->range)
					{
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+
								10+RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"FROM [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
							sprintf(fieldname,"FROM %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->from.integer_value=*member->value.integer_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+
								8+RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"TO [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
							sprintf(fieldname,"TO %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->to.integer_value=
								*member->value.integer_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
					}
					break;
				case SHORTV:
				case SSHORTV:
					frs->from.short_value=Rmalloc(sizeof(short));
					*frs->from.short_value=0;
					frs->to.short_value=Rmalloc(sizeof(short));
					*frs->to.short_value=0;
					if(frs->range)
					{
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+10+
								RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"FROM [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
							sprintf(fieldname,"FROM %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->from.short_value=
								*member->value.short_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8+
								RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"TO [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
							sprintf(fieldname,"TO %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->to.short_value=
								*member->value.short_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
					}
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
					frs->from.float_value=Rmalloc(sizeof(double));
					*frs->from.float_value=0;
					frs->to.float_value=Rmalloc(sizeof(double));
					*frs->to.float_value=0;
					if(frs->range)
					{
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+10+RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"FROM [%s][%s]",FILENAME(fileno),field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
							sprintf(fieldname,"FROM %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->from.float_value=
								*member->value.float_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8+
								RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"TO [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
							sprintf(fieldname,"TO %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->to.float_value=
								*member->value.float_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
					}
					break;
				case BOOLNS:
					frs->from.string_value=Rmalloc(1);
					*frs->from.string_value=TRUE;
					frs->to.string_value=Rmalloc(1);
					*frs->to.string_value=TRUE;
					frs->visible=FALSE;
					if(!type)
					{
						fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+17+RDAstrlen(FILENAME(fileno)));
						sprintf(fieldname,"SELECT [%s][%s] TRUE",FILENAME(fileno),field->name);
					} else {
						fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+15);
						sprintf(fieldname,"SELECT %s TRUE",field->name);
					}
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->from.string_value=
							*member->value.string_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
					if(!type)
					{
						fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+18
							+RDAstrlen(FILENAME(fileno)));
						sprintf(fieldname,"SELECT [%s][%s] FALSE",
							FILENAME(fileno),field->name);
					} else {
						fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+16);
						sprintf(fieldname,"SELECT %s FALSE",field->name);
					}
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->to.string_value=
							*member->value.string_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
					break;
				case CHARACTERS:
					frs->from.string_value=Rmalloc(1);
					*frs->from.string_value=0;
					frs->to.string_value=Rmalloc(1);
					*frs->to.string_value=0;
					if(frs->range)
					{
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+
								10+RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"FROM [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
							sprintf(fieldname,"FROM %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->from.string_value=
								*member->value.string_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
						if(!type)
						{
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8+
								RDAstrlen(FILENAME(fileno)));
							sprintf(fieldname,"TO [%s][%s]",FILENAME(fileno),
								field->name);
						} else {
							fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
							sprintf(fieldname,"TO %s",field->name);
						}
						x=FINDRSC(rs->rsrc,fieldname);
						if(x!=(-1))
						{
							member=rs->rsrc->rscs+x;
							*frs->to.string_value=
								*member->value.string_value;
							if(member->w!=NULL) frs->visible=TRUE;
						}
					}
					break;
				case SCROLLEDLIST:
				case BUTTONS:
					prterr("Error Cannot Range on this field type.");
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Field [%s].",field->type,field->name);
					break;
			}
			if(fieldname!=NULL) Rfree(fieldname);
		}
	}
}
/*-----------------------------------------------------------------------------
	file2rangersrc	function to create range resources for each field in
			 the file 
-----------------------------------------------------------------------------*/
short xfile2rangersrc(short fileno,RDArsrc *rsrc,short type,int line,char *file)
{
	NRDfield *field=NULL;
	short x,numflds=0,editable=FALSE;
	char *name=NULL,svalue=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG file2rangersrc Creating Range Resources for File [%d] [%s] on Screen [%s] [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	if((numflds=NUMFLDS(fileno))==(-1))
	{
		prterr("Error cannot get fields for file [%d] [%s].",fileno,
			FILENAME(fileno));
		return(-1);
	}
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(fileno,x);
		if(field!=NULL)
		{
			if(FIELDscrn(fileno,field->name,&editable))
			{
				if(field->type!=BOOLNS)
				{
					if(!type)
					{
						name=Rmalloc(RDAstrlen(field->name)+14+RDAstrlen(FILENAME(fileno)));
						sprintf(name,"RANGE ON [%s][%s]",FILENAME(fileno),field->name);
					} else {
						name=Rmalloc(RDAstrlen(field->name)+12);
						sprintf(name,"RANGE ON %s",field->name);
					}
					if(!alreadyrsrc(name,rsrc))
					{
						makefieldrsrc(rsrc,name,BOOLNS,1,TRUE);
						ADVFINDRSCSETFUNC(rsrc,name,setrangesensitivecb,NULL,TRUE);
					}
					Rfree(name);
					if(!type)
					{
						name=Rmalloc(RDAstrlen(field->name)+10+RDAstrlen(FILENAME(fileno)));
						sprintf(name,"FROM [%s][%s]",FILENAME(fileno),field->name);
					} else {
						name=Rmalloc(RDAstrlen(field->name)+8);
						sprintf(name,"FROM %s",field->name);
					}
					if(!alreadyrsrc(name,rsrc))
					{
						makefieldrsrc(rsrc,name,field->type,
							field->len,TRUE);
						ADVFINDRSCSETFUNC(rsrc,name,setrangeto2fromcb,NULL,TRUE);
					}
					Rfree(name);
					if(!type)
					{
						name=Rmalloc(RDAstrlen(field->name)+8+RDAstrlen(FILENAME(fileno)));
						sprintf(name,"TO [%s][%s]",FILENAME(fileno),field->name);
					} else {
						name=Rmalloc(RDAstrlen(field->name)+6);
						sprintf(name,"TO %s",field->name);
					}
					if(!alreadyrsrc(name,rsrc))
					{
						makefieldrsrc(rsrc,name,field->type,
							field->len,TRUE);
					}
					Rfree(name);
				} else {
					if(!type)
					{
						name=Rmalloc(RDAstrlen(field->name)+17+RDAstrlen(FILENAME(fileno)));
						sprintf(name,"SELECT [%s][%s] TRUE",FILENAME(fileno),field->name);
					} else {
						name=Rmalloc(RDAstrlen(field->name)+15);
						sprintf(name,"SELECT %s TRUE",field->name);
					}
					if(!alreadyrsrc(name,rsrc))
					{
						makefieldrsrc(rsrc,name,field->type,
					 		field->len,TRUE);
						svalue=TRUE;
						FINDRSCSETCHAR(rsrc,name,svalue);
					}
					Rfree(name);
					if(!type)
					{
						name=Rmalloc(RDAstrlen(field->name)+18+RDAstrlen(FILENAME(fileno)));
						sprintf(name,"SELECT [%s][%s] FALSE",FILENAME(fileno),field->name);
					} else {
						name=Rmalloc(RDAstrlen(field->name)+16);
						sprintf(name,"SELECT %s FALSE",field->name);
					}
					if(!alreadyrsrc(name,rsrc))
					{
						makefieldrsrc(rsrc,name,field->type,
							field->len,TRUE);
						FINDRSCSETCHAR(rsrc,name,svalue);
					}	
					Rfree(name);
				}
			}
		}
	}
	return(0);
}
/*-----------------------------------------------------------------------------
	testfilerangersrc	Automatically tests ranges for a specified files 
						range resources.
-----------------------------------------------------------------------------*/
short xtestfilerangersrc(short fileno,RangeScreen *rs,int line,char *file)
{
	NRDfield *field=NULL;
	short x,numflds=0,y;
	FileRangeStruct *frs=NULL;
	short return_value=FALSE;
	char *filename=NULL;
	char *tempdateadj=NULL;
	char *tempdatetoadj=NULL,*tempdatefradj=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Testing Module [%s] File [%d] [%s] for range restrictions Using Range Screen [%s] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),(rs!=NULL?rs->module:"NULL"),(rs!=NULL?rs->screen:"NULL"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rs!=NULL)
	{
	if(!TESTFILENO(fileno,line,file)) return(-1);
	if((numflds=NUMFLDS(fileno))==(-1))
	{
		prterr("Error cannot get fields for file %d [%s].",fileno,
			FILENAME(fileno));
		return(-1);
	}	
	if(rs->frs!=NULL)
	{
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(fileno,x);
		if(field!=NULL)
		{
			filename=FILENAME(fileno);
			for(y=0,frs=rs->frs;y<rs->num;++y,++frs)
			{
				if(!RDAstrcmp(frs->filename,filename) &&
					!RDAstrcmp(frs->fieldname,field->name)) 
				{
					break;
				}
			}
			if(y<rs->num)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case CHARACTERS:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case TIMES:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(frs->range)
						{
							unpad(field->data.string_value);
							if(RDAstrcmp(field->data.string_value,frs->from.string_value)<0)
							{
								return_value=TRUE;
							}
							if(RDAstrcmp(field->data.string_value,frs->to.string_value)>0)
							{
								return_value=TRUE;
							}
						}
						break;
					case DATES:
						if(frs->range)
						{
							unpad(field->data.string_value);
							if(!isEMPTY(field->data.string_value))
							{
								if(RDAstrlen(field->data.string_value)==10)
								{
									tempdateadj=Rmalloc(9);
									sprintf(tempdateadj,"%c%c%c%c%c%c%c%c",field->data.string_value[6],field->data.string_value[7],field->data.string_value[8],field->data.string_value[9],field->data.string_value[0],field->data.string_value[1],field->data.string_value[3],field->data.string_value[4]);
								} else if(RDAstrlen(field->data.string_value)==8)
								{
									tempdateadj=Rmalloc(9);
									sprintf(tempdateadj,"%c%c%c%c%c%c%c%c",'1','9',field->data.string_value[6],field->data.string_value[7],field->data.string_value[0],field->data.string_value[1],field->data.string_value[3],field->data.string_value[4]);

								} 
							}
							if(!isEMPTY(frs->from.string_value))
							{
								if(RDAstrlen(frs->from.string_value)==10)
								{
									tempdatefradj=Rmalloc(9);
									sprintf(tempdatefradj,"%c%c%c%c%c%c%c%c",frs->from.string_value[6],frs->from.string_value[7],frs->from.string_value[8],frs->from.string_value[9],frs->from.string_value[0],frs->from.string_value[1],frs->from.string_value[3],frs->from.string_value[4]);
								} else if(RDAstrlen(frs->to.string_value)==8)
								{
									tempdatefradj=Rmalloc(9);
									sprintf(tempdatefradj,"%c%c%c%c%c%c%c%c",'1','9',frs->from.string_value[6],frs->from.string_value[7],frs->from.string_value[0],frs->from.string_value[1],frs->from.string_value[3],frs->from.string_value[4]);
								}
							}
							if(!isEMPTY(frs->to.string_value))
							{
								if(RDAstrlen(frs->to.string_value)==10)
								{
									tempdatetoadj=Rmalloc(9);
									sprintf(tempdatetoadj,"%c%c%c%c%c%c%c%c",frs->to.string_value[6],frs->to.string_value[7],frs->to.string_value[8],frs->to.string_value[9],frs->to.string_value[0],frs->to.string_value[1],frs->to.string_value[3],frs->to.string_value[4]);
								} else if(RDAstrlen(frs->to.string_value)==8)
								{
									tempdatetoadj=Rmalloc(9);
									sprintf(tempdatetoadj,"%c%c%c%c%c%c%c%c",'1','9',frs->to.string_value[6],frs->to.string_value[7],frs->to.string_value[0],frs->to.string_value[1],frs->to.string_value[3],frs->to.string_value[4]);
								}
							}
/*
							if(RDAstrcmp(tempdateadj,tempdatefradj)>=0&&RDAstrcmp(tempdateadj,tempdatetoadj)<=0)
							{
								return_val=TRUE;
							} else {
								return(FALSE);
							}	
*/
							if(RDAstrcmp(tempdateadj,tempdatefradj)<0)
							{
								return_value=TRUE;
							}
							if(RDAstrcmp(tempdateadj,tempdatetoadj)>0)
							{
								return_value=TRUE;
							}
							if(tempdateadj!=NULL) Rfree(tempdateadj);
							if(tempdatefradj!=NULL) Rfree(tempdatefradj);
							if(tempdatetoadj!=NULL) Rfree(tempdatetoadj);

/* originally this
							if(RDAstrcmp(field->data.string_value,frs->from.string_value)<0)
							{
								return_value=TRUE;
							}
							if(RDAstrcmp(field->data.string_value,frs->to.string_value)>0)
							{
								return_value=TRUE;
							}
*/
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(frs->range)
						{
							if(*field->data.integer_value<*frs->from.integer_value || *field->data.integer_value>*frs->to.integer_value) return_value=TRUE;
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(frs->range)
						{
							if(*field->data.short_value<*frs->from.short_value || *field->data.short_value>*frs->to.short_value) return_value=TRUE;
						}
						break;
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case SDECIMALV:
						if(frs->range)
						{
							if(*field->data.float_value<*frs->from.float_value || *field->data.float_value>*frs->to.float_value) return_value=TRUE;
						}
						break;
					case BUTTONS:
						break;
					case BOOLNS:
						if((*field->data.string_value && !*frs->from.string_value)) return_value=TRUE;
						if((!*field->data.string_value && !*frs->to.string_value)) return_value=TRUE;		
						break;
					default:
						prterr("Error field type [%d] is invalid for field [%s].",field->type,field->name);
						break;
				}
			}
			if(return_value) break;
		}
	}
	} else {
		prterr("Error No Range information available in Screen [%s] [%s].",rs->module,rs->screen);
	}
	} else {
		prterr("Error No Range Screen Defined for Testing Module [%s] File [%s] fileno [%d].",MODULENAME(fileno),FILENAME(fileno),fileno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG testfilerangersrc returning [%d] ",return_value);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return return_value;
}
char *xgetfilerangedesc(short fileno,RangeScreen *rs,int line,char *file)
{
	NRDfield *field=NULL;
	short x,numflds=0;
	FileRangeStruct *frs=NULL;
	int y;
	short count=0;
	char *filename=NULL,*desc=NULL,*to_str=NULL,*from_str=NULL;
	char stemp1[101];

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getfilerangedesc Get File Range Description for file [%d] [%s] off Screen [%s] [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),rs->module,rs->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(NULL);
	if((numflds=NUMFLDS(fileno))==(-1))
	{
		prterr("Error cannot get fields for file %d [%s].",fileno,
			FILENAME(fileno));
		return(NULL);
	}	
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(fileno,x);
		if(field!=NULL)
		{
			filename=FILENAME(fileno);
			for(y=0,frs=rs->frs;y<rs->num;++y,++frs)
			{
				if(!RDAstrcmp(frs->filename,filename) &&
					!RDAstrcmp(frs->fieldname,field->name)) break;
			}
			if(y<rs->num)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case CHARACTERS:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case DATES:
					case TIMES:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(frs->range)
						{
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,RDAstrlen(desc)+(frs->to.string_value!=NULL ? RDAstrlen(frs->to.string_value):0)+(frs->from.string_value!=NULL ? RDAstrlen(frs->from.string_value):0)+RDAstrlen(field->name)+28);
							} else desc=Rmalloc(RDAstrlen(desc)+(frs->to.string_value!=NULL ? RDAstrlen(frs->to.string_value):0)+(frs->from.string_value!=NULL ? RDAstrlen(frs->from.string_value):0)+RDAstrlen(field->name)+28);
							unpad(frs->from.string_value);
							unpad(frs->to.string_value);
							sprintf(&desc[count],"Range on [%s] from [%s] to [%s]. ",field->name,(frs->from.string_value!=NULL ? frs->from.string_value:""),(frs->to.string_value!=NULL ? frs->to.string_value:""));
							count+=(frs->to.string_value!=NULL ? RDAstrlen(frs->to.string_value):0)+(frs->from.string_value!=NULL ? RDAstrlen(frs->from.string_value):0)+RDAstrlen(field->name)+27;
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(frs->range)
						{
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,RDAstrlen(desc)+((field->len)*2)+RDAstrlen(field->name)+28);
							} else desc=Rmalloc(((field->len)*2)+RDAstrlen(field->name)+28);
							sprintf(&desc[count],"Range on [%s] from [%*d] to [%*d]. ",field->name,field->len,*frs->from.integer_value,field->len,*frs->to.integer_value);
							count+=((field->len)*2)+RDAstrlen(field->name)+27;
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(frs->range)
						{
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,RDAstrlen(desc)+((field->len)*2)+RDAstrlen(field->name)+28);
							} else desc=Rmalloc(((field->len)*2)+RDAstrlen(field->name)+28);
							sprintf(&desc[count],"Range on [%s] from [%*d] to [%*d]. ",field->name,field->len,*frs->from.short_value,field->len,*frs->to.short_value);
							count+=((field->len)*2)+RDAstrlen(field->name)+27;
						}
						break;
					case DOLLARS:
						if(frs->range)
						{
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,RDAstrlen(desc)+((field->len)*2)+RDAstrlen(field->name)+28);
							} else desc=Rmalloc(((field->len)*2)+RDAstrlen(field->name)+28);
							to_str=famt(*frs->to.float_value,field->len);
							from_str=famt(*frs->from.float_value,field->len);
							sprintf(&desc[count],"Range on [%s] from [%s] to [%s]. ",field->name,(from_str!=NULL?from_str:""),(to_str!=NULL?to_str:""));
							if(to_str!=NULL) Rfree(to_str);
							if(from_str!=NULL) Rfree(from_str);
							count+=((field->len)*2)+RDAstrlen(field->name)+27;
						}
						break;
					case DOLLARS_NOCENTS:
						if(frs->range)
						{
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,RDAstrlen(desc)+((field->len)*2)+RDAstrlen(field->name)+28);
							} else desc=Rmalloc(((field->len)*2)+RDAstrlen(field->name)+28);
							to_str=famtncents(*frs->to.float_value*100,field->len,' ');
							from_str=famtncents(*frs->from.float_value*100,field->len,' ');
							sprintf(&desc[count],"Range on [%s] from [%s] to [%s]. ",field->name,(from_str!=NULL?from_str:""),(to_str!=NULL?to_str:""));
							if(to_str!=NULL) Rfree(to_str);
							if(from_str!=NULL) Rfree(from_str);
							count+=((field->len)*2)+RDAstrlen(field->name)+27;
						}
						break;
					case DOUBLEV:
					case SDOUBLEV:
						if(frs->range)
						{
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,RDAstrlen(desc)+((field->len)*2)+RDAstrlen(field->name)+28);
							} else desc=Rmalloc(((field->len)*2)+RDAstrlen(field->name)+28);
							sprintf(&desc[count],"Range on [%s] from [%*.0f] to [%*.0f]. ",field->name,field->len,*frs->from.float_value,field->len,*frs->to.float_value);
							count+=((field->len)*2)+RDAstrlen(field->name)+27;
						}
						break;
					case DECIMALV:
					case SDECIMALV:
						if(frs->range)
						{
							memset(stemp,0,101);
							memset(stemp1,0,101);
							sprintf(stemp,"%*f",field->len,*frs->from.float_value);
							sprintf(stemp1,"%*f",field->len,*frs->to.float_value);
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,RDAstrlen(desc)+RDAstrlen(stemp)+RDAstrlen(stemp1)+RDAstrlen(field->name)+28);
							} else desc=Rmalloc(RDAstrlen(stemp)+RDAstrlen(stemp1)+RDAstrlen(field->name)+28);
							sprintf(&desc[count],"Range on [%s] from [%s] to [%s]. ",field->name,stemp,stemp1);
							count+=((field->len)*2)+RDAstrlen(field->name)+27;
						}
						break;
					case BUTTONS:
						break;
					case BOOLNS:
						if(!*frs->from.string_value || !*frs->to.string_value)
						{
							if(desc!=NULL)
							{
								desc=Rrealloc(desc,(RDAstrlen(desc)+RDAstrlen(field->name)+35));
							} else desc=Rmalloc((RDAstrlen(field->name)+35));
							sprintf(&desc[count],"Select [%s] TRUE [%3s] FALSE [%3s]. ",(field->name!=NULL ? field->name:""),(*frs->from.string_value ? "Yes":"No"),(*frs->to.string_value ? "Yes":"No"));
							count+=RDAstrlen(field->name)+34;
						}
						break;
					default:
						prterr("Error field type [%d] is invalid for field [%s].",field->type,field->name);
						break;
				}
			}
		}
	}
	if(desc==NULL) desc=stralloc("SELECT ALL");
	return desc;
}
void setrangesensitivecb(RDArmem *member)
{
	char *temp=NULL;

	readmember(member);
	setrangesensitive(member);
	temp=Rmalloc(10+RDAstrlen(member->rscrname));
	sprintf(temp,"FROM%s",&member->rscrname[8]);
	FINDRSCSETINPUTFOCUS((RDArsrc *)member->parent,temp);
	if(temp!=NULL) Rfree(temp);
}
void SetRangersrcsensitive(RDArsrc *parent)
{
	short x=0;
	RDArmem *member=NULL;

	if(parent!=NULL)
	{
		for(x=0,member=parent->rscs;x<parent->numrscs;++x,++member)
		{
			if(!strncmp(member->rscrname,"RANGE ON",8))
			{
				setrangesensitive(member);
			}
		}
		SetAllEditableAndSensitive(parent);
	}
}
void setrangesensitive(RDArmem *member)
{
	RDArsrc *parent=NULL;
	char value=FALSE;
	char *fromname=NULL,*toname=NULL;
	RDArmem *frommember=NULL,*tomember=NULL;
	short fromrsrcno=0,torsrcno=0;

	parent=member->parent;
/*
put here for a timing issue, sometimes the *member->value.string_value 
isn't set when it is polled for its value in this callback function for the
RANGE ON toggle button.
tried, this to no avail:
	FINDRSCGETCHAR(parent,member->rscrname,&value);
*/
	value=(char)*member->value.string_value;
	fromname=Rmalloc(RDAstrlen(&member->rscrname[9])+6);
	sprintf(fromname,"FROM %s",&member->rscrname[9]);
	toname=Rmalloc(RDAstrlen(&member->rscrname[9])+5);
	sprintf(toname,"TO %s",&member->rscrname[9]);
	fromrsrcno=FINDRSC(member->parent,fromname);
	torsrcno=FINDRSC(member->parent,toname);
	frommember=parent->rscs+fromrsrcno;
	tomember=parent->rscs+torsrcno;
	if(value==TRUE)
	{
		MEMBERSETEDITABLE(frommember,TRUE);
		MEMBERSETEDITABLE(tomember,TRUE);
		MEMBERSETSENSITIVE(frommember,TRUE);
		MEMBERSETSENSITIVE(tomember,TRUE);
	} else {
		MEMBERSETSENSITIVE(frommember,FALSE);
		MEMBERSETSENSITIVE(tomember,FALSE);
		MEMBERSETEDITABLE(frommember,FALSE);
		MEMBERSETEDITABLE(tomember,FALSE);
	}
	if(fromname!=NULL) Rfree(fromname);
	if(toname!=NULL) Rfree(toname);
	SetAllListEditableAndSensitive(parent);
}
short xfile2range_editableandsensitive(short fileno,RDArsrc *rsrc,short type,short editable,short sensitive,int line,char *file)
{
	NRDfield *field=NULL;
	short x,numflds=0,e=0,s=0;
	char bol=FALSE,*name=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG file2range_editableandsensitive for File [%d] [%s] on Screen [%s] [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	if((numflds=NUMFLDS(fileno))==(-1))
	{
		prterr("Error cannot get fields for file [%d] [%s].",fileno,
			FILENAME(fileno));
		return(-1);
	}
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(fileno,x);
		if(field!=NULL)
		{
			if(field->type!=BOOLNS)
			{
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+14+RDAstrlen(FILENAME(fileno)));
					sprintf(name,"RANGE ON [%s][%s]",FILENAME(fileno),field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+12);
					sprintf(name,"RANGE ON %s",field->name);
				}
				FINDRSCSETEDITABLE(rsrc,name,editable);
				FINDRSCSETSENSITIVE(rsrc,name,sensitive);
				if(editable) FINDRSCGETCHAR(rsrc,name,&bol);
					else bol=FALSE;
				Rfree(name);
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+10+RDAstrlen(FILENAME(fileno)));
					sprintf(name,"FROM [%s][%s]",FILENAME(fileno),field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+8);
					sprintf(name,"FROM %s",field->name);
				}
				if(bol && editable) e=editable;
					else e=FALSE;
				if(bol && sensitive) s=sensitive;
					else s=FALSE;
				FINDRSCSETEDITABLE(rsrc,name,e);
				FINDRSCSETSENSITIVE(rsrc,name,s);
				Rfree(name);
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+8+RDAstrlen(FILENAME(fileno)));
					sprintf(name,"TO [%s][%s]",FILENAME(fileno),field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+6);
					sprintf(name,"TO %s",field->name);
				}
				if(bol && editable) e=editable;
					else e=FALSE;
				if(bol && sensitive) s=sensitive;
					else s=FALSE;
				FINDRSCSETEDITABLE(rsrc,name,e);
				FINDRSCSETSENSITIVE(rsrc,name,s);
				Rfree(name);
			} else {
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+17+RDAstrlen(FILENAME(fileno)));
					sprintf(name,"SELECT [%s][%s] TRUE",FILENAME(fileno),field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+15);
					sprintf(name,"SELECT %s TRUE",field->name);
				}
				FINDRSCSETEDITABLE(rsrc,name,editable);
				FINDRSCSETSENSITIVE(rsrc,name,sensitive);
				Rfree(name);
				if(!type)
				{
					name=Rmalloc(RDAstrlen(field->name)+18+RDAstrlen(FILENAME(fileno)));
					sprintf(name,"SELECT [%s][%s] FALSE",FILENAME(fileno),field->name);
				} else {
					name=Rmalloc(RDAstrlen(field->name)+16);
					sprintf(name,"SELECT %s FALSE",field->name);
				}
				FINDRSCSETEDITABLE(rsrc,name,editable);
				FINDRSCSETSENSITIVE(rsrc,name,sensitive);
				Rfree(name);
			}
		}
	}
	SetAllListEditableAndSensitive(rsrc);
	return(0);
}
