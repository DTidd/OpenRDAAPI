/* dochg1.c */
#include<mix.hpp>
#include<dfchg.hpp>

extern CHGstruct *CHGSTRUCT;
extern void doexit(void);
#ifdef RADDOG
extern void dochg_usekey(RDArsrc *,RangeScreen *,CHGfile *);
#endif
extern void dochg_nokey(RDArsrc *,RangeScreen *,CHGfile *);
void dochg(void);
static void dochg_range(void),process_change(RDArsrc *);
char xSetCHGstructSupportingFiles(CHGfile *,void (*)(...),void *,int,char *);

static void quit_change(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	doexit();
	std::exit;
}
static void set_simplefrom(RDArsrc *rsrc)
{
	char s=FALSE;
	int x,y;
	RDAsearch *s1=NULL;
	CHGfile *chgfile=NULL;

	readwidget(rsrc,"SIMPLE FROM");
	FINDRSCGETCHAR(rsrc,"SIMPLE FROM",&s);
	if(s)
	{
		if(CHGSTRUCT->chgfiles!=NULL)
		{
			for(x=0,chgfile=CHGSTRUCT->chgfiles;x<CHGSTRUCT->numfiles;
				++x,++chgfile)
			{
				if(chgfile->supfil!=NULL)
				{
					for(y=0,s1=chgfile->supfil;y<chgfile->num;
						++y,++s1)
					{
						if(s1->range==TRUE)
						{
							file2range_editableandsensitive(s1->fileno,rsrc,FALSE,FALSE);
						}
					}
				}
			}
		}
		FINDRSCSETSENSITIVE(rsrc,"FROM",TRUE);
		FINDRSCSETEDITABLE(rsrc,"FROM",TRUE);
	} else {
		if(CHGSTRUCT->chgfiles!=NULL)
		{
			for(x=0,chgfile=CHGSTRUCT->chgfiles;x<CHGSTRUCT->numfiles;
				++x,++chgfile)
			{
				if(chgfile->supfil!=NULL)
				{
					for(y=0,s1=chgfile->supfil;y<chgfile->num;
						++y,++s1)
					{
						if(s1->range==TRUE)
						{
							file2range_editableandsensitive(s1->fileno,rsrc,TRUE,TRUE);
						}
					}
				}
			}
		}
		FINDRSCSETEDITABLE(rsrc,"FROM",FALSE);
		FINDRSCSETSENSITIVE(rsrc,"FROM",FALSE);
	}
}
static void set_simpleto(RDArsrc *rsrc)
{
	char s=FALSE;

	readwidget(rsrc,"SIMPLE TO");
	FINDRSCGETCHAR(rsrc,"SIMPLE TO",&s);
	if(s)
	{
		FINDRSCSETEDITABLE(rsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(rsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(rsrc,"TO",TRUE);
		FINDRSCSETEDITABLE(rsrc,"TO",TRUE);
	} else {
		FINDRSCSETEDITABLE(rsrc,"TO",FALSE);
		FINDRSCSETSENSITIVE(rsrc,"TO",FALSE);
		FINDRSCSETSENSITIVE(rsrc,"EXPRESSION",TRUE);
		FINDRSCSETEDITABLE(rsrc,"EXPRESSION",TRUE);
	}
	SetAllEditableAndSensitive(rsrc);
}
static void dochg_range()
{
	CHGfile *chgfile=NULL;
	RDAsearch *s=NULL;
	int x,y;

	CHGSTRUCT->rsrc=RDArsrcNEW(CHGSTRUCT->module,CHGSTRUCT->range_screen);
	addstdrsrc(CHGSTRUCT->rsrc,"SIMPLE FROM",BOOLNS,1,&CHGSTRUCT->simple_from,TRUE);
	FINDRSCSETFUNC(CHGSTRUCT->rsrc,"SIMPLE FROM",set_simplefrom,NULL);
	addstdrsrc(CHGSTRUCT->rsrc,"SIMPLE TO",BOOLNS,1,&CHGSTRUCT->simple_to,TRUE);
	FINDRSCSETFUNC(CHGSTRUCT->rsrc,"SIMPLE TO",set_simpleto,NULL);
	addsctrsrc(CHGSTRUCT->rsrc,"EXPRESSION",0,(CHGSTRUCT->expression!=NULL ? CHGSTRUCT->expression:""),TRUE);
	switch(CHGSTRUCT->type)
	{
		case SCROLLEDTEXT:
			addsctrsrc(CHGSTRUCT->rsrc,"FROM",0,CHGSTRUCT->from.string_value,TRUE);
			addsctrsrc(CHGSTRUCT->rsrc,"TO",0,CHGSTRUCT->to.string_value,TRUE);
			break;	
		case VARIABLETEXT:
		case PLAINTEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			addstdrsrc(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->from.string_value,TRUE);
			addstdrsrc(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->to.string_value,TRUE);
			break;	
		case BOOLNS:
		case CHARACTERS:
			addstdrsrc(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->from.string_value,TRUE);
			addstdrsrc(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->to.string_value,TRUE);
			break;
		case DECIMALV:
		case SDECIMALV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DOUBLEV:
		case SDOUBLEV:
			addstdrsrc(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->from.float_value,TRUE);
			addstdrsrc(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->to.float_value,TRUE);
			break;
		case SHORTV:
		case SSHORTV:
			addstdrsrc(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->from.short_value,TRUE);
			addstdrsrc(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->to.short_value,TRUE);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			addstdrsrc(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->from.integer_value,TRUE);
			addstdrsrc(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->type,CHGSTRUCT->len,CHGSTRUCT->to.integer_value,TRUE);
			break;
		default:
			prterr("Error field type [%d] is inccorect.",CHGSTRUCT->type);
			break;
	}
	if(CHGSTRUCT->chgfiles!=NULL)
	{
		for(x=0,chgfile=CHGSTRUCT->chgfiles;x<CHGSTRUCT->numfiles;
			++x,++chgfile)
		{
			addDFincvir(CHGSTRUCT->rsrc,chgfile->module,chgfile->filename,NULL,chgfile->fileno);
			if(chgfile->supfil!=NULL)
			{
				for(y=0,s=chgfile->supfil;y<chgfile->num;++y,++s)
				{
					addDFincvir(CHGSTRUCT->rsrc,s->module,s->filename,NULL,s->fileno);
					if(s->range==TRUE)
					{
						file2rangersrc(s->fileno,CHGSTRUCT->rsrc);
/*
						setfile2rsrccallbacks(s->fileno,CHGSTRUCT->rsrc,0);
*/
					}
				}
			}	
		}
	}
	GET_SCREEN_VIRTUALS(CHGSTRUCT->rsrc,2);
	SCRNvirtual2rangersrc(CHGSTRUCT->rsrc);
	DefaultScreens(CHGSTRUCT->rsrc);
	addrfkwrsrc(CHGSTRUCT->rsrc,"SELECT",TRUE,process_change,NULL);
	addrfexrsrc(CHGSTRUCT->rsrc,"QUIT",TRUE,quit_change,NULL);
	addbtnrsrc(CHGSTRUCT->rsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(CHGSTRUCT->rsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(CHGSTRUCT->rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_simplefrom(CHGSTRUCT->rsrc);
	set_simpleto(CHGSTRUCT->rsrc);
	APPmakescrn(CHGSTRUCT->rsrc,TRUE,quit_change,NULL,TRUE);
}
static void set_keyparts(CHGstruct *c)
{
	int x,y,z;
	CHGfile *cf=NULL;
	CHGfield *f=NULL;
	NRDkey *key;
	NRDpart *part;

	if(c!=NULL)
	{
		if(c->chgfiles!=NULL)
		{
			for(x=0,cf=c->chgfiles;x<c->numfiles;++x,++cf)
			{
				key=KEYNUM(cf->fileno,cf->keyno);
				if(key!=NULL)
				{
					if(cf->chgflds!=NULL)
					{
						for(y=0,f=cf->chgflds;y<cf->numflds;++y,++f)
						{
							for(z=0,part=key->part;z<key->numparts;++z,++part)
							{
								if(!RDAstrcmp(part->name,f->fieldname)) break;
							}
							if(z<key->numparts) f->keypart=TRUE;
								else f->keypart=FALSE;
						}	
					}
				}
			}
		}
	}
}
#ifdef RADDOG
static short DetermineLoopType(CHGfile *chgfile)
{
	CHGfield *f;
	int x,y;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	short do_fast=0;

	if(chgfile!=NULL)
	{
		if(chgfile->numflds>1) return(1);
		key=KEYNUM(chgfile->fileno,chgfile->keyno);
		if(key!=NULL)
		{
			if(chgfile->chgflds!=NULL && key->part!=NULL)
			{
				for(x=0,part=key->part;x<key->numparts;++x,++part)
				{
					for(y=0,f=chgfile->chgflds;y<chgfile->numflds;++y,++f)
					{
						if(!RDAstrcmp(f->fieldname,part->name)) break;
					}
					if(y<chgfile->numflds)
					{
						++do_fast;
					} else break;
				}
				if((do_fast==x) && (do_fast!=0)) return(0);
			}
			return(1);
		}
	}
	return(-1);
}
#endif
static void process_change(RDArsrc *range)
{
	RangeScreen *rs=NULL;
	CHGfile *chgfile=NULL;
	RDAsearch *s=NULL;
	int x,y;
	char *temp=NULL;
	RDArsrc *tmprsrc=NULL;
#ifdef RADDOG
	short loop_type=(-1);
#endif
	
	if(range!=NULL)
	{
		readallwidgets(range);
	}
	if((CHGSTRUCT->range==TRUE) && range!=NULL)
	{
		FINDRSCGETSTRING(CHGSTRUCT->rsrc,"EXPRESSION",&CHGSTRUCT->expression);
		if(!isEMPTY(CHGSTRUCT->expression))
		{
			temp=PP_translate(CHGSTRUCT->expression);
			if(!isEMPTY(temp))
			{
				if(CHGSTRUCT->expression!=NULL) 
					Rfree(CHGSTRUCT->expression);
				CHGSTRUCT->expression=stralloc(temp);
			}
			if(temp!=NULL) Rfree(temp);
		}		
		FINDRSCGETCHAR(CHGSTRUCT->rsrc,"SIMPLE FROM",&CHGSTRUCT->simple_from);
		FINDRSCGETCHAR(CHGSTRUCT->rsrc,"SIMPLE TO",&CHGSTRUCT->simple_to);
		if(CHGSTRUCT->simple_from==FALSE)
		{
			rs=RangeScreenNew(CHGSTRUCT->rsrc);
			for(x=0,chgfile=CHGSTRUCT->chgfiles;x<CHGSTRUCT->numfiles;++x,++chgfile)
			{
				if(chgfile->supfil!=NULL)
				{
					for(y=0,s=chgfile->supfil;y<chgfile->num;
						++y,++s)
					{
						ReadRangeScreen(s->fileno,rs);	
					}
				}
			}
		}
		if(CHGSTRUCT->simple_from || CHGSTRUCT->simple_to)
		{
			switch(CHGSTRUCT->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case PLAINTEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(CHGSTRUCT->simple_from)
					{
						FINDRSCGETSTRING(CHGSTRUCT->rsrc,"FROM",&CHGSTRUCT->from.string_value);
						CHGSTRUCT->from_dlen=RDAstrlen(CHGSTRUCT->from.string_value)+1;
					}
					if(CHGSTRUCT->simple_to)
					{
						FINDRSCGETSTRING(CHGSTRUCT->rsrc,"TO",&CHGSTRUCT->to.string_value);
						CHGSTRUCT->to_dlen=RDAstrlen(CHGSTRUCT->to.string_value)+1;
					}	
					break;	
				case BOOLNS:
				case CHARACTERS:
					if(CHGSTRUCT->simple_from)
					{
						FINDRSCGETCHAR(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->from.string_value);
					}
					if(CHGSTRUCT->simple_to)
					{
						FINDRSCGETCHAR(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->to.string_value);
					}
					break;
				case DECIMALV:
				case SDECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case SDOUBLEV:
					if(CHGSTRUCT->simple_from)
					{
						FINDRSCGETDOUBLE(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->from.float_value);
					}
					if(CHGSTRUCT->simple_to)
					{
						FINDRSCGETDOUBLE(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->to.float_value);
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(CHGSTRUCT->simple_from)
					{
						FINDRSCGETSHORT(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->from.short_value);
					}
					if(CHGSTRUCT->simple_to)
					{
						FINDRSCGETSHORT(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->to.short_value);
					}
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(CHGSTRUCT->simple_from)
					{
						FINDRSCGETINT(CHGSTRUCT->rsrc,"FROM",CHGSTRUCT->from.integer_value);
					}
					if(CHGSTRUCT->simple_to)
					{
						FINDRSCGETINT(CHGSTRUCT->rsrc,"TO",CHGSTRUCT->to.integer_value);
					}
					break;
				default:
					prterr("Error field type [%d] is inccorect.",CHGSTRUCT->type);
					break;
			}
		}
	}
	set_keyparts(CHGSTRUCT);
	if(CHGSTRUCT->chgfiles!=NULL)
	{
		for(x=0,chgfile=CHGSTRUCT->chgfiles;x<CHGSTRUCT->numfiles;
			++x,++chgfile)
		{
			temp=Rmalloc(RDAstrlen(chgfile->module)+
				RDAstrlen(chgfile->filename)+
				RDAstrlen(CHGSTRUCT->module)+
				RDAstrlen(CHGSTRUCT->name)+14);
			sprintf(temp,"%s %s Changing %s %s",
				(CHGSTRUCT->module!=NULL ? CHGSTRUCT->module:""),
				(CHGSTRUCT->name!=NULL ? CHGSTRUCT->name:""),
				(chgfile->module!=NULL ? chgfile->module:""),
				(chgfile->filename!=NULL ? chgfile->filename:""));
			tmprsrc=diagscrn(chgfile->fileno,"DIAGNOSTIC SCREEN",
				CHGSTRUCT->module,temp,NULL);	
			CHGSTRUCT->SubFunc=SCRNvirtualSubData;
			CHGSTRUCT->SubFuncArgs=tmprsrc;
			if(temp!=NULL) Rfree(temp);
			if(tmprsrc!=NULL)
			{
				addDFincvir(tmprsrc,chgfile->module,chgfile->filename,NULL,chgfile->fileno);
				if(chgfile->supfil!=NULL)
				{
					for(y=0,s=chgfile->supfil;y<chgfile->num;++y,++s)
					{
						addDFincvir(tmprsrc,s->module,s->filename,NULL,s->fileno);
					}
				}
			}
#ifdef RADDOG
			loop_type=DetermineLoopType(chgfile);
#endif
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			if(tmprsrc!=NULL)
			{
				if(ADVmakescrn(tmprsrc,TRUE))
				{
				} else {
					ForceWindowUpdate(tmprsrc);
				}
			}
#ifndef RADDOG
			dochg_nokey(tmprsrc,rs,chgfile);
#else
			switch(loop_type)
			{
				case 0:
					dochg_usekey(tmprsrc,rs,chgfile);
					break;
				case 1:
					dochg_nokey(tmprsrc,rs,chgfile);
					break;
				default:
				case (-1):
					prterr("Error Skipping CHGfile [%s][%s] because of indeterminate loop type...",(chgfile->module!=NULL ? chgfile->module:""),(chgfile->filename!=NULL ? chgfile->filename:""));
					break;
			}
#endif
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
				tmprsrc=NULL;
			}
		}
	}
	quit_change(CHGSTRUCT->rsrc);
}
void dochg()
{
	if(diagapps)
	{
		prterr("DIAG dochg Starting Change [%s] [%s].",CHGSTRUCT->module,CHGSTRUCT->name);
	}
	if(CHGSTRUCT->range==TRUE)
	{
		if(!isEMPTY(CHGSTRUCT->range_screen))
		{
			dochg_range();
		} else {
			ERRORDIALOG("INVALID SCREEN NAME","The Range Screen Name was empty.... Please define a Range Screen in the Define Change and retry.",doexit,TRUE);
		}
	} else {
		process_change(NULL);
	}
}
char xSetCHGstructSupportingFiles(CHGfile *chgfile,void (*SubFunc)(...),void *args,int line,char *file)
{
	int x,y;
	RDAsearch *s;
	RDAconfld *confld;
	NRDfield *field=NULL;
	char *temp=NULL;

	if(diagapps)
	{
		prterr("DIAG xSetCHGstructSupportingFiles at line [%d] program [%s].",line,file);
	}
	if(chgfile!=NULL)
	{
		if(chgfile->supfil!=NULL)
		{
			for(x=0,s=chgfile->supfil;x<chgfile->num;++x,++s)
			{
				if(s->fileno!=(-1))
				{
					if(s->confld!=NULL)
					{
						for(y=0,confld=s->confld;y<s->num;
							++y,++confld)
						{
							field=FLDNRD(s->fileno,confld->fldname);
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
											field->data.string_value=PP_EVALUATEstr(confld->conname,SubFunc,args);
											if(field->data.string_value!=NULL) field->dlen=RDAstrlen(field->data.string_value)+1;
											if(field->dlen>field->len && field->len>0) field->data.string_value[field->len]=0;
											break;
										case BOOLNS:
										case CHARACTERS:
											*field->data.string_value=PP_EVALUATEbol(confld->conname,SubFunc,args);
											break;
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(confld->conname,SubFunc,args);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(confld->conname,SubFunc,args);
											break;
										case LONGV:
										case SLONGV:
										case SCROLLEDLIST:
											*field->data.integer_value=PP_EVALUATEint(confld->conname,SubFunc,args);
											break;
										default:
										case BUTTONS:
											break;
									}
									if(temp!=NULL) Rfree(temp);
								} else {
									prterr("Error no conname....");
								}	
							} else {
								prterr("Error field not found for confld->fldname [%s] ",(confld->fldname!=NULL ? confld->fldname:""));
							}
						}
						if(ADVEQLNRDsec(s->fileno,s->keyno,SubFunc,args))
						{
							KEYNRD(s->fileno,s->keyno);
							return(0);
						}
					}
				}
			}
		}
	}
	return(TRUE);
}
