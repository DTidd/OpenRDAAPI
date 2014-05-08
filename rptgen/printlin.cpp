/* printline.c - Print Line Function */
#include<rptgen.hpp>

int symbol_count=0;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

void print_font(RDArunrpt *rrpt,RDAreport *rpt,char *font,char **holdvarx)
{
	RDAdisplay *d;
	RDApvar *pvar;
	int x,y,len=0;
	char *temp=NULL,*hold=NULL,*holdx=NULL;
	int s=0;
	char temp1[512];
	FILE *fpx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG print_font Substitute Font Command for Font Name [%s].",font);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	d=rpt->display;
	for(x=0,pvar=d->pvars;x<d->num;++x,++pvar)
	{
		if(!RDAstrcmp(pvar->name,font)) break;
	}
	if(x<d->num)
	{
		if(pvar->chgsize)
		{
			if(RDAstrcmp(rrpt->curfont,font))
			{
				if(rrpt->curfont!=NULL) Rfree(rrpt->curfont);
				rrpt->curfont=stralloc(font);
				rrpt->curpoints=pvar->points;
				rrpt->curpitch=pvar->pitch;
			}
			if(pvar->points>rrpt->points || !rrpt->startline)
			{
				rrpt->points=pvar->points;
				rrpt->pitch=pvar->pitch;
				if(rrpt->longfont!=NULL) Rfree(rrpt->longfont);
				rrpt->longfont=stralloc(pvar->name);
				rrpt->startline=TRUE;
			}
		}
		if(pvar->type==0)
		{
			if(RDAstrlen(*holdvarx)<RDAstrlen(pvar->command) || *holdvarx==NULL)
			{
				*holdvarx=Rrealloc(*holdvarx,RDAstrlen(pvar->command)+1);
				memset(*holdvarx,0,RDAstrlen(pvar->command)+1);
			}
			holdx=*holdvarx;
			hold=stralloc(pvar->command);
			memset(hold,0,RDAstrlen(hold)+1);
			temp=pvar->command;
			while(*temp)
			{
				if(*temp=='\\')
				{
					y=0;
					++temp;
					while(isdigit(*temp))
					{
						hold[y]=*temp;
						++temp;++y;
					}
					hold[y]=0;
					if(!isEMPTY(hold))
					{
						x=Ratoi(hold);
					} else x=0;
					if(x>0 && x<255) *holdx=x;
				} else {
					*holdx=*temp;
					++temp;
				}
				++holdx;
			}
			if(hold!=NULL) Rfree(hold);
		} else if(pvar->type==1) /* file substitution */
		{
			if(!isEMPTY(pvar->command))
			{
				hold=PRT_Text_File(pvar->command);
				if(!isEMPTY(hold))
				{
					*holdvarx=Rrealloc(*holdvarx,RDAstrlen(hold)+1);
					memset(*holdvarx,0,RDAstrlen(hold)+1);
					memcpy(*holdvarx,hold,RDAstrlen(hold)+1);
				} else {
					sprintf(*holdvarx,"");
				}
				if(hold!=NULL) Rfree(hold);
			} else {
				sprintf(*holdvarx,"");
			}
		} else if(pvar->type==2) /* binary file substitution */
		{
			if(!RDAstrcmp(pvar->name,"LOAD VENPMT SIGNATURE"))
			{
				memset(temp1,0,512);
				sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",(VENPMT_PASSWORD!=NULL ? VENPMT_PASSWORD:""),(VENPMT_SIGNATURE_FILE!=NULL ? VENPMT_SIGNATURE_FILE:""));
				fpx=popen(temp1,"r");
				if(fpx!=NULL)
				{
					while(true)
					{
						s=getc(fpx);
						if(s==EOF) break;
						if(rpt->display->device==0 || rpt->display->device==5)
						{
							RDA_fprintf(rrpt->pfile,"%c",s);
						} else {
							fprintf(rrpt->fp,"%c",s);
						}
					}
				}
				sprintf(*holdvarx,"");
			} else if(!RDAstrcmp(pvar->name,"LOAD PAYROLL SIGNATURE"))
			{
				memset(temp1,0,512);
				sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",(PAYROLL_PASSWORD!=NULL ? PAYROLL_PASSWORD:""),(PAYROLL_SIGNATURE_FILE!=NULL ? PAYROLL_SIGNATURE_FILE:""));
				fpx=popen(temp1,"r");
				if(fpx!=NULL)
				{
					while(true)
					{
						s=getc(fpx);
						if(s==EOF) break;
						if(rpt->display->device==0 || rpt->display->device==5)
						{
							RDA_fprintf(rrpt->pfile,"%c",s);
						} else {
							fprintf(rrpt->fp,"%c",s);
						}
					}
				}
				sprintf(*holdvarx,"");
			} else if(!RDAstrcmp(pvar->name,"LOAD PO SIGNATURE"))
			{
				memset(temp1,0,512);
				sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",(PURORD_PASSWORD!=NULL ? PURORD_PASSWORD:""),(PURORD_SIGNATURE_FILE!=NULL ? PURORD_SIGNATURE_FILE:""));
				fpx=popen(temp1,"r");
				if(fpx!=NULL)
				{
					while(true)
					{
						s=getc(fpx);
						if(s==EOF) break;
						if(rpt->display->device==0 || rpt->display->device==5)
						{
							RDA_fprintf(rrpt->pfile,"%c",s);
						} else {
							fprintf(rrpt->fp,"%c",s);
						}
					}
				}
				sprintf(*holdvarx,"");
			} else {
				hold=PRT_Binary_File(pvar->command,&len);
				if(rpt->display->device==0 || rpt->display->device==5)
				{
					for(y=0,temp=hold;y<len;++y,++temp)
					{
						RDA_fprintf(rrpt->pfile,"%c",*temp);
					}
#ifndef WIN32
					if(rrpt->pfile!=NULL) fflush(rrpt->pfile->pfile);
#endif
				} else {
					for(y=0,temp=hold;y<len;++y,++temp)
					{
						fprintf(rrpt->fp,"%c",*temp);
					}
#ifndef WIN32
					fflush(rrpt->fp);
#endif
				}
				sprintf(*holdvarx,"");
				if(hold!=NULL) Rfree(hold);
			}
		} else {
			prterr("Error Unknown RDApvar Type....");
			sprintf(*holdvarx," ");
		}
	} else if(!RDAstrcmp(font,"FORMFEED"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\f");
	} else if(!RDAstrcmp(font,"LINEFEED"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\n");
	} else if(!RDAstrcmp(font,"CARRIAGE RETURN"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\r");
	} else if(!RDAstrcmp(font,"TAB"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\t");
	} else if(!isEMPTY(font))
	{
		if(RDAstrlen(*holdvarx)<(RDAstrlen(font)+1) || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,RDAstrlen(font)+1);
		}
		memset(*holdvarx,0,RDAstrlen(font)+1);
		sprintf(*holdvarx,"%s",font);
	} else {
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx," ");
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG print_font Font Name [%s] returns command [%s].",font,*holdvarx);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
void FreeRDAprint(RDAprint *p)
{
	if(p!=NULL) Rfree(p);
}
RDAprint *RDAprintNEW(short type,char *string)
{
	RDAprint *tmp=NULL;
	char *temp;
	short hold=0,count=0,hold1=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG RDAprintNEW [%d] [%s] image string [%s].",type,fieldtypedesc[type],string);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAprint));
	tmp->fieldtype=type;
	tmp->length=RDAstrlen(string);
	tmp->type=(-1);
	tmp->numdecimals=0;
	temp=string;
	switch(type)
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
			for(;*temp;++temp)
			{
				++count;
				switch(*temp)
				{
					case 'L':
						if(tmp->type==(-1)) tmp->type=0;
						break;
					case 'R':
						if(tmp->type==(-1)) tmp->type=1;
						break;
					case 'C':
						if(tmp->type==(-1)) tmp->type=2;
						break;
					case 'W':
						if(tmp->type==(-1)) tmp->type=3;
						break;
					case 'V':
						if(tmp->type==(-1)) tmp->type=4;
						break;
					case 'X':
						if(tmp->type==(-1)) tmp->type=5;
						break;
					default:
						break;
				}
			}
			if(tmp->type==(-1)) tmp->type=4;
			break;
		case SSHORTV:
		case SLONGV:
		case SDOUBLEV:
		case SHORTV:
		case SCROLLEDLIST:
		case DOUBLEV:
		case LONGV:
			for(;*temp;++temp)
			{
				++count;
				switch(*temp)
				{
					case 'L':
						if(tmp->type==(-1)) tmp->type=0;
						break;
					case 'C':
						if(tmp->type==(-1)) tmp->type=2;
						break;
					case '0':
						if(tmp->type==(-1)) tmp->type=3;
						break;
					case '9':
						if(tmp->type==(-1)) tmp->type=4;
						break;
					case '*':
						if(tmp->type==(-1)) tmp->type=5;
						break;
					case 'R':
						if(tmp->type==(-1)) tmp->type=1;
						break;
					case 'W':
					case 'V':
						if(tmp->type==(-1)) tmp->type=6;
						break;
					default:
						break;
				}
			}
			if(tmp->type==(-1)) tmp->type=1;
			break;
		case DECIMALV:
		case SDECIMALV:
			for(;*temp;++temp)
			{
				++count;
				switch(*temp)
				{
					case '.':
						tmp->numdecimals=tmp->length-count;
						break;
					case 'L':
					case 'W':
					case 'V':
						if(tmp->type==(-1)) tmp->type=0;
						break;
					case 'C':
						if(tmp->type==(-1)) tmp->type=2;
						break;
					case '0':
						if(tmp->type==(-1)) tmp->type=3;
						break;
					case '9':
						if(tmp->type==(-1)) tmp->type=4;
						break;
					case '*':
						if(tmp->type==(-1)) tmp->type=5;
						break;
					case 'R':
						if(tmp->type==(-1)) tmp->type=1;
						break;
					default:
						break;
				}
			}
			if(tmp->type==(-1)) tmp->type=1;
			tmp->type+=hold;
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
			for(;*temp;++temp)
			{
				++count;
				switch(*temp)
				{
					case '(':
					case ')':
						hold1=8;
						break;
					case ',':
						hold=4;
						break;
					case '.':
						tmp->numdecimals=tmp->length-count;
						break;
					case 'L':
					case 'W':
					case 'V':
					case 'C':
					case 'R':
						if(tmp->type==(-1)) tmp->type=0;
						break;
					case '0':
						if(tmp->type==(-1)) tmp->type=1;
						break;
					case '9':
						if(tmp->type==(-1)) tmp->type=2;
						break;
					case '*':
						if(tmp->type==(-1)) tmp->type=3;
						break;
					default:
						break;
				}
			}
			if(tmp->type==(-1)) tmp->type=0;
			tmp->type+=hold+hold1;
			break;
		case BOOLNS:
			for(;*temp;++temp)
			{
				++count;
				switch(*temp)
				{
					case '1':
					case '0':
						if(tmp->type==(-1)) tmp->type=0;
						break;
					case 'Y':
					case 'N':
						if(tmp->type==(-1)) tmp->type=1;
						break;
					case 'T':
					case 'F':
						if(tmp->type==(-1)) tmp->type=2;
						break;
					default:
						break;
				}
			}
			if(tmp->type==(-1)) tmp->type=1;
			break;
		case CHARACTERS:
			for(;*temp;++temp)
			{
				++count;
				switch(*temp)
				{
					case 'R':
						if(tmp->type==(-1)) tmp->type=1;
						break;
					case 'C':
						if(tmp->type==(-1)) tmp->type=2;
						break;
					case 'W':
					case 'V':
						if(tmp->type==(-1)) tmp->type=3;
						break;
					case 'L':
						if(tmp->type==(-1)) tmp->type=0;
						break;
					default:
						break;
				}
			}
			if(tmp->type==(-1)) tmp->type=3;
			break;
		default:
			prterr("Error field type [%d].",type);
			break;
	}
	return(tmp);
}
short PrintLineSubData(char **tmp,char *name,HoldReport *h)
{
	char *modulename=NULL;
	NRDfield *field;
	char *dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";
	char strtmp[1024],*holdstring=NULL,*temp;
	RDAaccum *a;
	RDAaccval *accval;
	int y,tmplen=0;
	RDAvirtual *v;
	RDAreport *rpt;
	RDArunrpt *rrpt;
	RDAprint *prtx=NULL;
	double holdvalue=0;
	short return_value=0,len=0;
	RDAfinmgt *fmgt=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG PrintLineSubData Substitute Function for the Report Generator's Print Line, substitute for [%s] from [%s].",*tmp,name);
	}	
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rpt=h->rpt;
	rrpt=h->rrpt;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen_field)
	{
		prterr("DIAG PrintLineSubData Looking for [%s] in sort fields.",name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	field=FLDNRD(rrpt->sortno,name);
	if(field!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_field)
		{
			prterr("DIAG PrintLineSubData Found [%s] in sort fields.",name);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		prtx=RDAprintNEW(field->type,*tmp);
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
			case OPTIONALFIELDS:
				len=RDAstrlen(field->data.string_value);
				if(field->data.string_value!=NULL && len>0)
				{
					holdstring=Rmalloc(len+1);
					memcpy(holdstring,field->data.string_value,len+1);
				} else {
					holdstring=NULL;
				}
				break;
			case SHORTV:
			case SSHORTV:
				holdvalue=*field->data.short_value;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				holdvalue=*field->data.integer_value;
				break;
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				holdvalue=*field->data.float_value;
				break;
			case BOOLNS:
			case CHARACTERS:
				holdstring=Rmalloc(1);
				*holdstring=*field->data.string_value;
				break;
			default:
				prterr("Error field type [%d] is invalid for field [%s].",field->type,field->name); 
				break;
		}
	} else if(!RDAstrcmp(name,"[ORGANIZATION NAME]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(XPERT_SETUP->company_name!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->company_name);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[ORGANIZATION ADDRESS #1]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(XPERT_SETUP->addr1!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->addr1);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[ORGANIZATION ADDRESS #2]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(XPERT_SETUP->addr2!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->addr2);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"STARTING FISCAL MONTH"))
	{
		fmgt=RDAfinmgtNEW();
		getfinmgtbin(fmgt);
		prtx=RDAprintNEW(SHORTV,*tmp);
		holdvalue=fmgt->month;
		free_finmgt(fmgt);
		fmgt=NULL;
	} else if(!RDAstrcmp(name,"[ORGANIZATION CITY]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(XPERT_SETUP->city!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->city);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[ORGANIZATION STATE]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(XPERT_SETUP->state!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->state);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[ORGANIZATION ZIP]"))
	{
		prtx=RDAprintNEW(ZIPCODE,*tmp);
		if(XPERT_SETUP->zip!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->zip);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[ORGANIZATION PHONE]"))
	{
		prtx=RDAprintNEW(PHONE,*tmp);
		if(XPERT_SETUP->phone!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->phone);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[ORGANIZATION FAX]"))
	{
		prtx=RDAprintNEW(PHONE,*tmp);
		if(XPERT_SETUP->phone!=NULL)
		{
			holdstring=stralloc(XPERT_SETUP->fax);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[BREAK LEVEL]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(rrpt->cur_brk_name!=NULL)
		{
			holdstring=stralloc(rrpt->cur_brk_name);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[PREVIOUS BREAK LEVEL 2]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(!isEMPTY(rrpt->prev_brk_name2))
		{
			holdstring=stralloc(rrpt->prev_brk_name2);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[PREVIOUS BREAK LEVEL]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		if(!isEMPTY(rrpt->prev_brk_name))
		{
			holdstring=stralloc(rrpt->prev_brk_name);
		} else holdstring=NULL;
	} else if(!RDAstrcmp(name,"[PAGE NO]"))
	{
		prtx=RDAprintNEW(LONGV,*tmp);
		holdvalue=rrpt->pageno;
	} else if(!RDAstrcmp(name,"[BODY COUNT]"))
	{
		prtx=RDAprintNEW(LONGV,*tmp);
		holdvalue=rrpt->body_count;
	} else if(!RDAstrcmp(name,"[SYSTEM DATE]"))
	{
		prtx=RDAprintNEW(DATES,*tmp);
		holdstring=GETCURRENTDATE();
	} else if(!RDAstrcmp(name,"[SYSTEM DATE-YYYY]"))
	{
		prtx=RDAprintNEW(DATES,*tmp);
		holdstring=GETCURRENTDATE10();
	} else if(!RDAstrcmp(name,"[SYSTEM TIME]"))
	{
		prtx=RDAprintNEW(TIMES,*tmp);
		holdstring=GETCURRENTTIME();
	} else if(!RDAstrcmp(name,"[CURRENT PROCESS ID]"))
	{
		prtx=RDAprintNEW(LONGV,*tmp);
		memset(stemp,0,101);
		sprintf(stemp,"%d",RGETPID());
		holdstring=stralloc(stemp);
	} else if(!RDAstrcmp(name,"[CURRENT DIRECTORY]"))
	{
		prtx=RDAprintNEW(TIMES,*tmp);
#ifdef WIN32
		holdstring=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+2);
		sprintf(holdstring,"%s\\",CURRENTDIRECTORY);
#endif
#ifndef WIN32
		holdstring=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+2);
		sprintf(holdstring,"%s/",CURRENTDIRECTORY);
#endif
/*
		holdstring=stralloc(CURRENTDIRECTORY);
*/
	} else if(!RDAstrcmp(name,"[SYSTEM TYPE]"))
	{
		prtx=RDAprintNEW(TIMES,*tmp);
#if defined(WIN32)
		holdstring=stralloc("WIN32");
#elif defined(UNIXWARE7)
		holdstring=stralloc("UNIXWARE7");
#elif defined(UNIXWARE21)
		holdstring=stralloc("UNIXWARE21");
#elif defined(SCO50P)
		holdstring=stralloc("SCO5OP");
#elif defined(SCO50)
		holdstring=stralloc("SCO5O");
#elif defined(SC0386)
		holdstring=stralloc("SCO386");
#elif defined(RS6000)
		holdstring=stralloc("RS6000");
#elif defined(LINUX)
		holdstring=stralloc("LINUX");
#else
		holdstring=stralloc("UNKNOWN");
#endif
	} else if(!RDAstrcmp(name,"[USERS DEPARTMENT]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		holdstring=Users_Department();
	} else if(!RDAstrcmp(name,"[ABLE TO APPROVE]"))
	{
		prtx=RDAprintNEW(SHORTV,*tmp);
		memset(stemp,0,101);
		sprintf(stemp,"%d",Users_Able_to_Approve());
		holdstring=stralloc(stemp);
	} else if(!RDAstrcmp(name,"[APPROVAL LIMIT]"))
	{
		prtx=RDAprintNEW(DOLLARS,*tmp);
		memset(stemp,0,101);
		sprintf(stemp,"%d",Users_Approval_Limit());
		holdstring=stralloc(stemp);
	} else if(!RDAstrcmp(name,"[ACCOUNT MANAGER]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		holdstring=RDA_ReturnAccountManager();
	} else if(!RDAstrcmp(name,"[ACCOUNT GRANT IDENTIFICATION]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		holdstring=RDA_ReturnAccountGrantIdentification();
	} else if(!RDAstrcmp(name,"[ACCOUNT REFERENCE TYPE]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		holdstring=RDA_ReturnAccountReferenceType();
	} else if(!RDAstrcmp(name,"[LOGIN IDENTIFICATION]"))
	{
		prtx=RDAprintNEW(VARIABLETEXT,*tmp);
		holdstring=stralloc(USERLOGIN);
	} else {
	/* accumulators */
		modulename=stripmodulename(name);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_field)
		{
			prterr("DIAG PrintLineSubData Looking for [%s] in accumulators.",modulename);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
		{
			if(!RDAstrcmp(modulename,a->name)) break;
		}
		if(y<rpt->numaccums)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen_field)
			{
				prterr("DIAG PrintLineSubData Found [%s] in accumulators.",modulename);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			accval=a->accvals+rpt->currbrk;
			switch(a->fieldtype)
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
					switch(a->atype)
					{
						case 2: /* FIRST OCCURRENCE */
						case 3: /* LAST OCCURRENCE */
						case 6: /* PREVIOUS OCCURRENCE */
							prtx=RDAprintNEW(a->fieldtype,*tmp);
							if(accval->prev_value.string_value!=NULL && RDAstrlen(accval->prev_value.string_value)>0) 
							{
								holdstring=stralloc(accval->prev_value.string_value);
							} else holdstring=NULL;
							break;
						default:
						case 0: /* AVERAGE */
						case 4: /* MAXIMUM */
						case 5: /* MINIMUM */
						case 7: /* SUMMATION */
							prtx=RDAprintNEW(a->fieldtype,*tmp);
							if(accval->value.string_value!=NULL && RDAstrlen(accval->value.string_value)>0) 
							{
								holdstring=stralloc(accval->value.string_value);
							} else holdstring=NULL;
							break;
						case 1: /* FREQUENCY */
							prtx=RDAprintNEW(DOUBLEV,*tmp);
							holdvalue=*accval->value.float_value;
							break;
					}
					break;
				case CHARACTERS:
				case BOOLNS:
					holdstring=Rmalloc(1);
					switch(a->atype)
					{
						case 2: /* FIRST OCCURRENCE */
						case 3: /* LAST OCCURRENCE */
						case 6: /* PREVIOUS OCCURRENCE */
							prtx=RDAprintNEW(a->fieldtype,*tmp);
							holdvalue=*accval->prev_value.float_value;
							if(holdvalue!=0.0) *holdstring=TRUE; 
							break;
						default:
						case 0: /* AVERAGE */
						case 4: /* MAXIMUM */
						case 5: /* MINIMUM */
						case 7: /* SUMMATION */
							prtx=RDAprintNEW(a->fieldtype,*tmp);
							holdvalue=*accval->value.float_value;
							if(holdvalue!=0.0) *holdstring=TRUE; 
							break;
						case 1: /* FREQUENCY */
							prtx=RDAprintNEW(DOUBLEV,*tmp);
							holdvalue=*accval->value.float_value;
							if(holdvalue!=0.0) *holdstring=TRUE; 
							break;
					}
					break;
				case SSHORTV:
				case SLONGV:
				case SDOUBLEV:
				case SDECIMALV:
				case SHORTV:
				case SCROLLEDLIST:
				case LONGV:
				case DOUBLEV:
				case DECIMALV:
					switch(a->atype)
					{
						case 2: /* FIRST OCCURRENCE */
						case 3: /* LAST OCCURRENCE */
						case 6: /* PREVIOUS OCCURRENCE */
							prtx=RDAprintNEW(a->fieldtype,*tmp);
							holdvalue=*accval->prev_value.float_value;
							break;
						default:
						case 0: /* AVERAGE */
						case 4: /* MAXIMUM */
						case 5: /* MINIMUM */
						case 7: /* SUMMATION */
							prtx=RDAprintNEW(a->fieldtype,*tmp);
							holdvalue=*accval->value.float_value;
							break;
						case 1: /* FREQUENCY */
							prtx=RDAprintNEW(DOUBLEV,*tmp);
							holdvalue=*accval->value.float_value;
							break;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
					switch(a->atype)
					{
						case 2: /* FIRST OCCURRENCE */
						case 3: /* LAST OCCURRENCE */
						case 6: /* PREVIOUS OCCURRENCE */
							prtx=RDAprintNEW(DOLLARS,*tmp);
							holdvalue=*accval->prev_value.float_value;
							break;
						default:
						case 0: /* AVERAGE */
						case 4: /* MAXIMUM */
						case 5: /* MINIMUM */
						case 7: /* SUMMATION */
							prtx=RDAprintNEW(DOLLARS,*tmp);
							holdvalue=*accval->value.float_value;
							break;
						case 1: /* FREQUENCY */
							prtx=RDAprintNEW(DOUBLEV,*tmp);
							holdvalue=*accval->value.float_value;
							break;
					}
					break;
				default:
					prterr("Error field type [%d] is invalid for Accumulator [%s].",a->fieldtype,a->name); 
					break;
			}
/*
			if(modulename!=NULL) Rfree(modulename);
			modulename=NULL;
*/
		} else {
		/* virtual field */
/*
			modulename=stripmodulename(name);
*/
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen_field)
			{
				prterr("DIAG PrintLineSubData Looking for [%s] in Virtual fields.",modulename);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
			{
				if(!RDAstrcmp(modulename,v->name) && v->when) break;
			}
			tempvalue=NULL;
			if(y<rpt->numvirtuals)
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagrptgen_field)
				{
					prterr("DIAG PrintLineSubData Found [%s] in Virtual fields.",modulename);
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				prtx=RDAprintNEW(v->type,*tmp);
				switch(v->type)
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
						if(!v->computed && v->when) 
						{
							v->computed=TRUE;
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSTRING(rrpt->rsrsrc,v->name,
									&v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(!isEMPTY(tempvalue))
								{
									if(v->value.string_value!=NULL) Rfree(v->value.string_value);
									v->value.string_value=stralloc(tempvalue);
									v->dlen=RDAstrlen(v->value.string_value)+1;
								} else if(v->value.string_value!=NULL)
								{
									memset(v->value.string_value,0,v->dlen);
								} else {
									v->value.string_value=NULL;
									v->dlen=0;
								}
							}
							if(v->expression!=NULL)
							{
								temp=PP_EVALUATEstr(v->expression,ReportGenSubData1,h);
								if(v->value.string_value!=NULL) 
									Rfree(v->value.string_value);
								v->value.string_value=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
							}
							if(v->value.string_value!=NULL)
							{
								v->dlen=RDAstrlen(v->value.string_value)+1;
								if(v->dlen>v->len && v->len>0)
								{
									v->value.string_value[v->len]=0;
								}
							} else v->dlen=0;
						}
						if(v->value.string_value!=NULL && v->dlen>0)
						{
							holdstring=stralloc(v->value.string_value);
						} else holdstring=NULL;
						break;
					case SHORTV:
					case SSHORTV:
						if(!v->computed && v->when) 
						{
							v->computed=TRUE;
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSHORT(rrpt->rsrsrc,v->name,
									v->value.short_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.short_value=Rmalloc(sizeof(short));
									v->dlen=sizeof(short);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.short_value=Ratoi(tempvalue);
								} else {
									*v->value.short_value=0;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData1,h);
							}
						}
						holdvalue=*v->value.short_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(!v->computed && v->when) 
						{
							v->computed=TRUE;
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETINT(rrpt->rsrsrc,v->name,
									v->value.integer_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.integer_value=Rmalloc(sizeof(int));
									v->dlen=sizeof(int);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.integer_value=Ratoi(tempvalue);
								} else {
									*v->value.integer_value=0;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData1,h);
							}
						}
						holdvalue=*v->value.integer_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case DECIMALV:
					case SDOUBLEV:
					case SDECIMALV:
						if(!v->computed && v->when) 
						{
							v->computed=TRUE;
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,
									v->value.float_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.float_value=Rmalloc(sizeof(double));
									v->dlen=sizeof(double);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.float_value=atof(tempvalue);
								} else {
									*v->value.float_value=0;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
								if(v->type==DOLLARS || v->type==DOUBLEV ||
									v->type==SDOUBLEV || v->type==DOLLARS_NOCENTS)
								{
									*v->value.float_value=round(*v->value.float_value);
								}
							}
						}
						holdvalue=*v->value.float_value;
						break;
					case BOOLNS:
						if(!v->computed && v->when) 
						{
							v->computed=TRUE;
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,
									v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								if(!isEMPTY(tempvalue))
								{
									if(Ratoi(tempvalue))
									{
										*v->value.string_value=TRUE;
									} else {
										*v->value.string_value=FALSE;
									}
								} else {
									*v->value.string_value=FALSE;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
							}
						}
						holdstring=Rmalloc(1);
						*holdstring=*v->value.string_value;
						break;
					case CHARACTERS:
						if(!v->computed && v->when) 
						{
							v->computed=TRUE;
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,
									v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								
								if(!isEMPTY(tempvalue))
								{
									*v->value.string_value=*tempvalue;
								} else {
									*v->value.string_value=*tempchar;
								}
							}
							if(v->expression!=NULL)
							{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
							}
						}
						holdstring=Rmalloc(1);
						*holdstring=*v->value.string_value;
						break;
					default:
						prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
						break;
				}
			} else {
				prterr("Error field/value [%s] not found.",name);
			}
		}
	}
	if(modulename!=NULL) Rfree(modulename);
	modulename=NULL;
	switch(prtx->fieldtype)
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
			switch(prtx->type)
			{
				default:
				case 0: /* left */
					if(RDAstrlen(*tmp)<RDAstrlen(holdstring) && holdstring!=NULL)
					{
						strncpy(*tmp,holdstring,RDAstrlen(*tmp));
					} else {
						sprintf(*tmp,"%-*s",RDAstrlen(*tmp),(holdstring!=NULL ? holdstring:""));
					}
					break;
				case 1: /* right */
					if(RDAstrlen(*tmp)<RDAstrlen(holdstring) && holdstring!=NULL)
					{
						strncpy(*tmp,holdstring,RDAstrlen(*tmp));
					} else {
						sprintf(*tmp,"%*s",RDAstrlen(*tmp),(holdstring!=NULL ? holdstring:""));
					}
					break;
				case 2: /* centered */
					memset(*tmp,' ',RDAstrlen(*tmp));
					if(RDAstrlen(*tmp)>(RDAstrlen(holdstring)+1))
					{
						if(holdstring!=NULL)
						{
							sprintf(*tmp,"%*s%s%*s",(prtx->length-RDAstrlen(holdstring))/2,"",(holdstring!=NULL ? holdstring:""),(prtx->length-RDAstrlen(holdstring)+1)/2,"");
						}
					} else if(RDAstrlen(*tmp)>=RDAstrlen(holdstring))
					{
						if(holdstring!=NULL)
						{
							sprintf(*tmp,"%s%*s",(holdstring!=NULL ? holdstring:""),prtx->length-RDAstrlen(holdstring),"");
						}
					} else if(holdstring!=NULL)
					{
						strncpy(*tmp,holdstring,prtx->length);
					}
					break;
				case 3: /* wordwrap */
					if(holdstring!=NULL)
					{
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc(holdstring);
						return_value=1;
					} else {
						memset(*tmp,' ',RDAstrlen(*tmp));
					}
					break;
				case 4: /* variable */
					if(holdstring!=NULL)
					{
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc(holdstring);
					} else {
						memset(*tmp,' ',RDAstrlen(*tmp));
					}
					break;
				case 5: /* hex */
					if(holdstring!=NULL)
					{
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=Rmalloc(RDAstrlen(holdstring)+1);
						sprintf(*tmp,"%*x",RDAstrlen(holdstring),holdstring);
						*tmp=stralloc(holdstring);
					} else {
						memset(*tmp,' ',RDAstrlen(*tmp));
					}
					break;
			}
			break;
		case SSHORTV:
		case SLONGV:
		case SDOUBLEV:
		case SHORTV:
		case SCROLLEDLIST:
		case LONGV:
		case DOUBLEV:
			sprintf(stemp,"%.0f",holdvalue);
			switch(prtx->type)
			{
				case 6: /* Wordwrap or Variable */
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc(stemp);
					break;
				case 0: /* left */
					if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
					{
						strncpy(*tmp,stemp,RDAstrlen(*tmp));
					} else {
						sprintf(*tmp,"%-*s",RDAstrlen(*tmp),stemp);
					}
					break;
				default:
				case 1: /* right */
					if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
					{
						strncpy(*tmp,stemp,RDAstrlen(*tmp));
					} else {
						sprintf(*tmp,"%*s",RDAstrlen(*tmp),stemp);
					}
					break;
				case 2: /* center */
					if(RDAstrlen(*tmp)>(RDAstrlen(stemp)+1))
					{
						memset(*tmp,' ',RDAstrlen(*tmp));
						sprintf(*tmp,"%*s%s",(RDAstrlen(*tmp)-
							RDAstrlen(stemp))/2,"",stemp);
					} else if(RDAstrlen(*tmp)>=RDAstrlen(stemp))
					{
						memset(*tmp,' ',RDAstrlen(*tmp));
						sprintf(*tmp,"%s%*s",stemp,
							RDAstrlen(*tmp)-RDAstrlen(stemp),"");
					} else {
						strncpy(*tmp,stemp,RDAstrlen(*tmp));
					}
					break;
				case 3: /* 0 filled */
					sprintf(*tmp,"%0*.0f",RDAstrlen(*tmp),holdvalue);
					break;
				case 4: /* 9 filled */
					sprintf(*tmp,"%0*.0f",RDAstrlen(*tmp),holdvalue);
					temp=*tmp;
					for(;*temp;++temp)
					{
						if(*temp!='0') break;
						*temp='9';
					}
					break;
				case 5: /* * filled */
					sprintf(*tmp,"%0*.0f",RDAstrlen(*tmp),holdvalue);
					temp=*tmp;
					for(;*temp;++temp)
					{
						if(*temp!='0') break;
						*temp='*';
					}
					break;
			}
			memset(stemp,0,101);
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
			if(prtx->fieldtype==DOLLARS_NOCENTS) holdvalue*=100;
			if(prtx->numdecimals)
			{
				switch(prtx->type)
				{
					default:
					case 0: /* L,W,V */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncomma(holdvalue,prtx->length,' ');
						break;
					case 1: /* 0 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncomma(holdvalue,prtx->length,'0');
						break;
					case 2: /* 9 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncomma(holdvalue,prtx->length,'9');
						break;
					case 3: /* * */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncomma(holdvalue,prtx->length,'*');
						break;
					case 4: /* commas */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtpad(holdvalue,prtx->length,' ');
						break;
					case 5: /* commas 0 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtpad(holdvalue,prtx->length,'0');
						break;
					case 6: /* commas 9 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtpad(holdvalue,prtx->length,'9');
						break;
					case 7: /* commas * */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtpad(holdvalue,prtx->length,'*');
						break;
					case 8:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncomma(holdvalue,prtx->length,' ');
						break;
					case 9:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncomma(holdvalue,prtx->length,'0');
						break;
					case 10:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncomma(holdvalue,prtx->length,'9');
						break;
					case 11:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncomma(holdvalue,prtx->length,'*');
						break;
					case 12:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbk(holdvalue,prtx->length,' ');
						break;
					case 13:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbk(holdvalue,prtx->length,'0');
						break;
					case 14:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbk(holdvalue,prtx->length,'9');
						break;
					case 15:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbk(holdvalue,prtx->length,'*');
						break;
				}
			} else {
				switch(prtx->type)
				{
					default:
					case 0: /* L,W,V */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncnc(holdvalue,prtx->length,' ');
						break;
					case 1: /* 0 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncnc(holdvalue,prtx->length,'0');
						break;
					case 2: /* 9 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncnc(holdvalue,prtx->length,'9');
						break;
					case 3: /* * */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncnc(holdvalue,prtx->length,'*');
						break;
					case 4: /* commas */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncents(holdvalue,prtx->length,' ');
						break;
					case 5: /* commas 0 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncents(holdvalue,prtx->length,'0');
						break;
					case 6: /* commas 9 */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncents(holdvalue,prtx->length,'9');
						break;
					case 7: /* commas * */
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtncents(holdvalue,prtx->length,'*');
						break;
					case 8:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncnc(holdvalue,prtx->length,' ');
						break;
					case 9:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncnc(holdvalue,prtx->length,'0');
						break;
					case 10:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncnc(holdvalue,prtx->length,'9');
						break;
					case 11:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbkncnc(holdvalue,prtx->length,'*');
						break;
					case 12:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbknc(holdvalue,prtx->length,' ');
						break;
					case 13:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbknc(holdvalue,prtx->length,'0');
						break;
					case 14:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbknc(holdvalue,prtx->length,'9');
						break;
					case 15:
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=famtnbknc(holdvalue,prtx->length,'*');
						break;
				}
			}
			break;
		case DECIMALV:
		case SDECIMALV:
			tmplen=RDAstrlen(*tmp);
			sprintf(strtmp,"%*.*f",tmplen,prtx->numdecimals,holdvalue);
			strtmp[tmplen]=0;
			switch(prtx->type)
			{
				default:
				case 0: /* left */
					if(tmplen<(RDAstrlen(strtmp)))
					{
						strncpy(*tmp,strtmp,tmplen);
					} else {
						sprintf(*tmp,"%-*s",tmplen,strtmp);
					}
					break;
				case 1: /* right */
					if(tmplen<(RDAstrlen(strtmp)))
					{
						strncpy(*tmp,strtmp,tmplen);
					} else {
						sprintf(*tmp,"%*s",tmplen,strtmp);
					}
					break;
				case 2: /* center */
					if(tmplen>(RDAstrlen(strtmp)+1))
					{
						memset(*tmp,' ',tmplen);
						sprintf(*tmp,"%*s%s",
							(tmplen-RDAstrlen(strtmp))/2,"",strtmp);
					} else if(tmplen>=RDAstrlen(strtmp))
					{
						memset(*tmp,' ',tmplen);
						sprintf(*tmp,"%s%*s",strtmp,
							tmplen-RDAstrlen(strtmp),"");
					} else {
						strncpy(*tmp,strtmp,tmplen);
					}
					break;
				case 3: /* 0 filled */
					sprintf(strtmp,"%0*.*f",tmplen,prtx->numdecimals,holdvalue);
/* SJS FOUND ERROR 06/15/2000 : 19:43, missing this section of code */
					if(tmplen<(RDAstrlen(strtmp)))
					{
						strncpy(*tmp,strtmp,tmplen);
					} else {
						sprintf(*tmp,"%*s",tmplen,strtmp);
					}
/* SJS FOUND ERROR missing that section of code */
					break;
				case 4: /* 9 filled */
					sprintf(strtmp,"%0*.*f",tmplen,prtx->numdecimals,holdvalue);
					temp=*tmp;
					for(;*temp;++temp)
					{
						if(*temp!='0') break;
						*temp='9';
					}
					break;
				case 5: /* * filled */
					sprintf(strtmp,"%0*.*f",tmplen,prtx->numdecimals,holdvalue);
					temp=*tmp;
					for(;*temp;++temp)
					{
						if(*temp!='0') break;
						*temp='*';
					}
					break;
			}
			memset(stemp,0,101);
			break;
		case CHARACTERS:
			switch(prtx->type)
			{
				default:
				case 0: /* left */
					sprintf(*tmp,"%c%*s",*holdstring,prtx->length-1,"");
					break;
				case 1: /* right */
					sprintf(*tmp,"%*s%c",prtx->length-1,"",*holdstring);
					break;
				case 2: /* centered */
					break;
				case 3: /* wordwrap & variable */
					if(*tmp!=NULL)
					{
						*tmp=Rrealloc(*tmp,2);
					} else {
						*tmp=Rmalloc(2);
					}
					sprintf(*tmp,"%c",*holdstring);
					break;
			}
			break;
		case BOOLNS:
			switch(prtx->type)
			{
				default:
				case 0: /* 1 and 0 */
					sprintf(*tmp,"%d%*s",*holdstring,prtx->length-1,"");
					break;
				case 1: /* Y and N */
					if(RDAstrlen(*tmp)>3)
					{
						sprintf(*tmp,"%s%*s",(*holdstring ? "Yes":"No "),
							prtx->length-3,"");
					} else if(RDAstrlen(*tmp)==3)
					{
						strcpy(*tmp,(*holdstring ? "Yes":"No "));
					} else {
						strncpy(*tmp,(*holdstring ? "Yes":"No "),RDAstrlen(*tmp));
					}
					break;
				case 2: /* T and F */
					if(RDAstrlen(*tmp)>5)
					{
						sprintf(*tmp,"%s%*s",(*holdstring ? "True ":"False"),
							prtx->length-5,"");
					} else if(RDAstrlen(*tmp)==5)
					{
						strcpy(*tmp,(*holdstring ? "True ":"False"));
					} else {
						strncpy(*tmp,(*holdstring ? "True ":"False"),
							RDAstrlen(*tmp));
					}
					break;
			}
			break;
		default:
			prterr("Error field type [%d].",prtx->fieldtype);
			break;
	}
	if(prtx!=NULL) FreeRDAprint(prtx);
	if(holdstring!=NULL) Rfree(holdstring);
	
/* Bug of freeing (char *) returned from RDA_GetEnv() call */
/*
	if(value!=NULL) Rfree(value);
*/
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG PrintLineSubData Returning [%s] to be printed.",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(return_value);
}
short getlinecomponent(char **prog,char **holdvar,RDArunrpt *rrpt,RDAreport *rpt,RDAimage *i,short test_page)
{
	char *temp;
	RDApval *pvals;
	HoldReport *h=NULL;
	short return_value=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG getlinecomponent Get line component for image breakname [%s] line [%s].",i->breakname,i->line);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	temp=*holdvar;
	if(**prog=='\0' || *prog==NULL)
	{
		*temp='\0';
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen || diagrptgen_field)
		{
			prterr("DIAG getlinecomponent Returning holdvar [%s] return_value [%d].","NULL",return_value);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(return_value);
	}
	if(**prog=='@')
	{
		*temp=**prog;
		++temp;++(*prog);
		pvals=i->pvals+symbol_count;
		if(pvals->symbol_type==DEVICECONTROL)
		{
			*temp=0;
			print_font(rrpt,rpt,pvals->name,holdvar);
			if(!rrpt->startline) rrpt->startline=TRUE;
		} else {
			while(!is_in(**prog," @") && **prog) 
			{
				*temp=**prog;
				++temp;++(*prog);
			}
			*temp=0;
			if(!test_page)
			{
				h=HoldReportNew(rrpt,rpt);
				return_value=PrintLineSubData(holdvar,pvals->name,h);
				if(h!=NULL) Rfree(h);
				h=NULL;
			}
			if(!rrpt->startline) rrpt->startline=TRUE;
		}
		++symbol_count;
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen || diagrptgen_field)
		{
			prterr("DIAG getlinecomponent Returning holdvar [%s] return_value [%d].",(*holdvar!=NULL ? *holdvar:""),return_value);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(return_value);
	} else {
		*temp=**prog;
		++temp;++(*prog);
		while(**prog!='@' && **prog) 
		{
			*temp=**prog;
			++temp;++(*prog);
		}
		if(!rrpt->startline) rrpt->startline=TRUE;
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen || diagrptgen_field)
		{
			prterr("DIAG getlinecomponent Returning holdvar [%s] return_value [%d].",*holdvar,return_value);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(return_value);
	}
}
short increment_points(RDAimage *image)
{
	int x;
	RDApval *pval;

	for(x=0,pval=image->pvals;x<image->numpvals;++x,++pval)
	{
		if(pval->symbol_type && (!RDAstrcmp(pval->name,"[FORMFEED]")  ||
			!RDAstrcmp(pval->name,"[LINEFEED]") ||
			!RDAstrcmp(pval->name,"[CARRIAGE RETURN]"))) return(TRUE);
	}
	return(FALSE);
}
short xinterpretline(RDArunrpt *rrpt,RDAreport *rpt,RDAimage *image,short test_page,short currbrk,int line,char *file)
{
	char *tmp=NULL,*holdvar=NULL,*prog=NULL;
	int count=0;
	int length=0,length1=0;
	short wordwrap=TRUE,did_formfeed=FALSE,printit=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG Interpret Image Line [%s] for printed output at line [%d] program [%s].",image->line,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rrpt->startline=FALSE;
	symbol_count=0;
	prog=image->line;
	length=RDAstrlen(image->line);
	if(image->line!=NULL && length>0)
	{
		holdvar=Rmalloc(length+1);
		while(wordwrap)
		{
			while(TRUE)
			{
				wordwrap=getlinecomponent(&prog,&holdvar,rrpt,rpt,image,test_page);
				if(!*holdvar) break;
				if(!wordwrap)
				{
					length1=RDAstrlen(holdvar);
					if(tmp!=NULL)
					{
						tmp=Rrealloc(tmp,count+length1+1);
					} else {
						tmp=Rmalloc(length1+1);
					}
					sprintf(&tmp[count],"%s",(holdvar!=NULL ? holdvar:""));
					if(!RDAstrcmp(holdvar,"\f"))
					{
						did_formfeed=TRUE;
					}
					count+=length1;
					length1=0;
					if(holdvar!=NULL)
					{
						holdvar=Rrealloc(holdvar,length+1);
						memset(holdvar,0,length+1);
					} else {
						holdvar=Rmalloc(length+1);
					}
				} else {
					prterr("supposed to wordwrap.");TRACE;
				}
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diagrptgen_field)
			{
				prterr("DIAG xinterpretline Interpretted Image Line [%s].",tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			testform(rrpt,rpt,FALSE,test_page);
			rpt->currbrk=currbrk;
			if((rrpt->range_page && rrpt->pageno>=rrpt->from_page
				&& rrpt->pageno<=rrpt->to_page) ||
				!rrpt->range_page)
			{
				if(rpt->display->device==0 || rpt->display->device==5)
				{
					RDA_fprintf(rrpt->pfile,"%s",(tmp!=NULL ? tmp:""));
#ifndef WIN32
					if(rrpt->pfile!=NULL) fflush(rrpt->pfile->pfile);
#endif
					if(rrpt->archive==TRUE && rrpt->fp_pdf!=NULL)
					{
						fprintf(rrpt->fp_pdf,"%s",(tmp!=NULL ? tmp:""));
						fflush(rrpt->fp_pdf);
					}
				} else if((rpt->display->device==1 || rpt->display->device==2) && rrpt->archive==TRUE && rrpt->fp_pdf!=NULL)
				{
					fprintf(rrpt->fp,"%s",tmp);
					fflush(rrpt->fp);
					fprintf(rrpt->fp_pdf,"%s",(tmp!=NULL ? tmp:""));
					fflush(rrpt->fp_pdf);
				} else {
					fprintf(rrpt->fp,"%s",tmp);
					fflush(rrpt->fp);
				}
				printit=TRUE;
/* Increment Line Body Count Here */
				if(rpt->display->body_count_type==TRUE && 
					(!RDAstrcmp(image->breakname,"DETAIL LINES")))
				{
					++rrpt->body_count;
				}
			}
			if(did_formfeed)
			{

				if(RDAstrcmp(image->breakname,"PAGE FOOTER") &&
					RDAstrcmp(image->breakname,"REPORT FOOTER"))
				{
					rrpt->forced=TRUE;
					if(rrpt->pageno)
					{
						++rrpt->pageno;
						rrpt->numlines=PageFooterSize(rrpt,rpt);
					} else {
						if(rpt->display->starting_page!=0) rrpt->pageno=rpt->display->starting_page;
							else rrpt->pageno=1;
						print_section(rrpt,rpt,"REPORT HEADER",test_page);
						rrpt->numlines=PageFooterSize(rrpt,rpt);
					}
					print_section(rrpt,rpt,"PAGE HEADER",test_page);
					rrpt->forced=FALSE;
/* NEW BETWEEN HERE 07/31/2007 */
				} else if(!RDAstrcmp(image->breakname,"REPORT FOOTER"))
				{
					++rrpt->pageno;
					rrpt->numlines=0;
/* NEW BETWEEN HERE 07/31/2007 */
				}
				did_formfeed=FALSE;
			}
			if(tmp!=NULL) Rfree(tmp);
			tmp=NULL;
		}
		if(holdvar!=NULL) Rfree(holdvar);
		holdvar=NULL;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(tmp!=NULL)
	{
		if(diagrptgen || diagrptgen_field)
		{
			prterr("DIAG xinterpretline Interpretted Image Line [%s].",tmp);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(printit);
}
void process_dataline(RDArunrpt *rrpt,RDAreport *rpt,RDAimage *image,
	short update)
{
	char *modx=NULL,*filx=NULL,*temp,*fieldname=NULL;
	char *ret_string=NULL;
	APPlib *args=NULL;
	int count=0,ret_int=0;
	ReportFile *fx;
	int x,y;
	NRDfield *field=NULL,*field2=NULL,*field3=NULL,*field4=NULL,*field5=NULL,*field6=NULL;
	RDApval *pval;
	char *trnfield=NULL,xtemp[1024];
	HoldReport *h=NULL;
	RDATData *prev=NULL;

	MAIL_VARS *email_vars=NULL;
	email_vars=Rmalloc(sizeof(MAIL_VARS));
	email_vars->from_name=NULL;
	email_vars->from_addr=NULL;
	email_vars->subject=NULL;
	email_vars->body=NULL;
	email_vars->tolist=NULL;
	email_vars->cclist=NULL;
	email_vars->bcclist=NULL;
	email_vars->srcdomain=NULL;
	email_vars->ignerr=0;
	for(x=0;x<10;x++)
	{
		email_vars->fname[x]=NULL;
		email_vars->mtype[x]=NULL;
		email_vars->dname[x]=NULL;
		email_vars->fdata[x]=NULL;
	}

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG process_dataline Process Image Line [%s] for writing to database.",image->line);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!strncmp(&image->line[0],"OPEN",4) || 
		!strncmp(&image->line[0],"open",4)) /* open data file */
	{
		temp=image->line+4;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		filx=stripfilename(temp);
		if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
		{
			if(addReportFile(rrpt,modx,filx,TRUE,0,FALSE)==(-1))
			{
				prterr("Error Module [%s] File [%s] couldn't be opened.",
					modx,filx);
			}
		} else {
			prterr("Error Module [%s] File [%s] names in error.",(modx!=NULL ?
				modx:"NULL"),(filx!=NULL ? filx:"NULL"));
		}
		if(modx!=NULL) Rfree(modx);
		if(filx!=NULL) Rfree(filx);
	} else if(!strncmp(&image->line[0],"CLOSE",5) ||
		!strncmp(&image->line[0],"close",5)) /* close data file */
	{
		temp=image->line+5;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		filx=stripfilename(temp);
		if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
		{
			if(rrpt->ofiles!=NULL)
			{
				for(x=0,fx=rrpt->ofiles;x<rrpt->numofiles;++x,++fx)
				{
					if(fx->fileno!=(-1) && !RDAstrcmp(fx->module,modx)
						&& !RDAstrcmp(fx->filename,filx)) 
					{
						CLSNRD(fx->fileno);
						fx->fileno=(-1);
						break;
					}
				}
			}
		} else {
			prterr("Error Module [%s] File [%s] names in error cannot CLOSE.",
				(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
		}
		if(modx!=NULL) Rfree(modx);
		if(filx!=NULL) Rfree(filx);
	} else if(!strncmp(&image->line[0],"WWWDOADD",8) || 
		!strncmp(&image->line[0],"wwwdoadd",8))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"MODULE NAME"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"POWERADD NAME"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			args=APPlibNEW();
			addAPPlib(args,field->data.string_value);
			addAPPlib(args,field2->data.string_value);
			ADVExecute2Program("wwwdoadd",args,NULL);
			if(args!=NULL) freeapplib(args);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"DOREPORT",8) || 
		!strncmp(&image->line[0],"doreport",8))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"MODULE NAME"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"REPORT NAME"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			args=APPlibNEW();
			addAPPlib(args,field->data.string_value);
			addAPPlib(args,field2->data.string_value);
			ADVExecute2Program("doreport",args,NULL);
			if(args!=NULL) freeapplib(args);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"PUTENV",6) || 
		!strncmp(&image->line[0],"RDA_PutEnv",6))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!isEMPTY(pval->fieldname))
				{
					field=Rmalloc(sizeof(NRDfield));
					field->name=stralloc(pval->fieldname);
					field->data.string_value=NULL;
					field->dlen=0;
					field->type=VARIABLETEXT;
					field->len=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						temp=stralloc(pval->name);
						field->data.string_value=PP_EVALUATEstr(temp,ReportGenSubData1,h);
						if(temp!=NULL) Rfree(temp);
					}
#ifdef WIN32
					temp=Rmalloc(RDAstrlen(pval->fieldname)+RDAstrlen(field->data.string_value)+2);
					sprintf(temp,"%s=%s",(pval->fieldname!=NULL ? pval->fieldname:""),(field->data.string_value!=NULL ? field->data.string_value:""));
					RDA_PutEnv(temp);
					if(temp!=NULL) Rfree(temp);
#endif /* WIN32 */
#ifndef WIN32
					RDA_SetEnv(pval->fieldname,field->data.string_value);
#endif /* not WIN32 */
					if(field!=NULL)
					{
						if(field->data.string_value!=NULL) Rfree(field->data.string_value);
						if(field->name!=NULL) Rfree(field->name);
						Rfree(field);
					}
				}
			}
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"MKDIR",5) || 
		!strncmp(&image->line[0],"mkdir",5))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"DIRECTORY NAME"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						ret_string=RDAmkdir(field->data.string_value,MODE_RWXRWX___,&ret_int);
						if(!isEMPTY(ret_string))
						{
							if(ret_int!=EEXIST)
							{
								prterr("Error:  Cannot create directory [%s] because of [%s].",field->data.string_value,ret_string);
							} 
						}
						if(ret_string!=NULL) Rfree(ret_string);
						ret_string=RDAchmod(field->data.string_value,MODE_RWXRWX___,&ret_int);
						if(ret_string!=NULL) Rfree(ret_string);
					}
				}
				FreeNRDfieldvalue(field);
			}
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"RMDIR",5) || 
		!strncmp(&image->line[0],"rmdir",5))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"DIRECTORY NAME"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						rmdir(field->data.string_value);
					}
				}
				FreeNRDfieldvalue(field);
			}
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"UNLINK",6) || 
		!strncmp(&image->line[0],"unlink",6))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						memset(xtemp,0,1024);
						sprintf(xtemp,"%s/%s",CURRENTDIRECTORY,field->data.string_value);
						unlink(xtemp);
					}
				}
				FreeNRDfieldvalue(field);
			}
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"remdel",6) || 
		!strncmp(&image->line[0],"REMDEL",6))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"MODULE"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			args=APPlibNEW();
			addAPPlib(args,field->data.string_value);
			addAPPlib(args,field2->data.string_value);
			addAPPlib(args,"ALL");
			ADVExecute2Program("remdel",args,NULL);
			if(args!=NULL) freeapplib(args);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"COPYFILE",8) || 
		!strncmp(&image->line[0],"copyfile",8))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"FROM FILENAME"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"TO FILENAME"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			RDACopyFile(field->data.string_value,field2->data.string_value);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"MOVEFILE",8) || 
		!strncmp(&image->line[0],"movefile",8))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"FROM FILENAME"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"TO FILENAME"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			RDAMoveFile(field->data.string_value,field2->data.string_value);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"DISPLAYRELATIVEFILE",19) || 
		!strncmp(&image->line[0],"displayrelativefile",19))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			DisplayRelativeFile(field->data.string_value);
			FreeNRDfieldvalue(field);
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"DISPLAYFILE",11) || 
		!strncmp(&image->line[0],"displayfile",11))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			DisplayFile(field->data.string_value);
			FreeNRDfieldvalue(field);
		}
		if(h!=NULL) Rfree(h);
// This is the beginning of email functions
	} else if(!strncmp(&image->line[0],"SENDATTACH-1",12) || 
		!strncmp(&image->line[0],"sendattach-1",12))
	{
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"NAME OF SENDER"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_name=stralloc(field->data.string_value);
					} else email_vars->from_name=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SENDER EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_addr=stralloc(field->data.string_value);
					} else email_vars->from_addr=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SUBJECT"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->subject=stralloc(field->data.string_value);
					} else email_vars->subject=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BODY"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->body=stralloc(field->data.string_value);
					} else email_vars->body=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"RECIPIENT EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->tolist=stralloc(field->data.string_value);
					} else email_vars->tolist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"FILENAME TO BE ATTACHED"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[0]=stralloc(field->data.string_value);
					} else email_vars->fname[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"EMAIL ATTACHMENT TO BE NAMED"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[0]=stralloc(field->data.string_value);
					} else email_vars->dname[0]=NULL;
				}
			}
			if(field!=NULL)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				Rfree(field);
			}

			if(!isEMPTY(email_vars->from_name) && !isEMPTY(email_vars->from_addr) && 
				!isEMPTY(email_vars->subject) &&!isEMPTY(email_vars->body) && 
				!isEMPTY(email_vars->tolist) && !isEMPTY(email_vars->fname[0]) && !isEMPTY(email_vars->dname[0]))
			{
				if(email_vars->srcdomain==NULL)
				{
					temp=GetWebIDGSV();
					sprintf(stemp,"%s.openrda.net",temp);
					email_vars->srcdomain=stralloc(stemp);
				}
				RunVMimeSendmail(email_vars);
			} else {
				prterr("Error: sendattach-1 Requires 1st 6 parameters established.");
				prterr("\t\t               NAME OF SENDER: [%s] ",(email_vars->from_name!=NULL ? email_vars->from_name:""));
				prterr("\t\t         SENDER EMAIL ADDRESS: [%s] ",(email_vars->from_addr!=NULL ? email_vars->from_addr:""));
				prterr("\t\t                      SUBJECT: [%s] ",(email_vars->subject!=NULL ? email_vars->subject:""));
				prterr("\t\t                         BODY: [%s] ",(email_vars->body!=NULL ? email_vars->body:""));
				prterr("\t\t      RECIPIENT EMAIL ADDRESS: [%s] ",(email_vars->tolist!=NULL ? email_vars->tolist:""));
				prterr("\t\t       FILENAME TO BE ATTACHED [%s] ",(email_vars->fname[0]!=NULL ? email_vars->fname[0]:""));
				prterr("\t\t EMAIL ATTACHMENT TO BE NAMED: [%s] ",(email_vars->dname[0]!=NULL ? email_vars->dname[0]:""));
			}
			if(temp!=NULL) Rfree(temp);
			if(email_vars!=NULL) Rfree(email_vars);
		}
	} else if(!strncmp(&image->line[0],"EMAILUSERINFO",13) || 
		!strncmp(&image->line[0],"emailuserinfo",13))
	{
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;
			field3=Rmalloc(sizeof(NRDfield));
			field3->data.string_value=NULL;
			field3->dlen=0;
			field3->type=VARIABLETEXT;
			field3->len=0;
			field4=Rmalloc(sizeof(NRDfield));
			field4->data.string_value=NULL;
			field4->dlen=0;
			field4->type=VARIABLETEXT;
			field4->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"USER IDENTIFICATION"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"EMAIL ADDRESS"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"FROM USER NAME"))
				{
					if(field3->data.string_value!=NULL) Rfree(field3->data.string_value);
					field3->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field3,pval->name,rpt,rrpt);
					} else {
						field3->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"FROM USER EMAIL"))
				{
					if(field4->data.string_value!=NULL) Rfree(field4->data.string_value);
					field4->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field4,pval->name,rpt,rrpt);
					} else {
						field3->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			args=APPlibNEW();
			memset(stemp,0,101);
			sprintf(stemp,"XPERT_USER=%s",(field->data.string_value!=NULL ? field->data.string_value:""));
			addAPPlib(args,stemp);
			memset(stemp,0,101);
			sprintf(stemp,"VMIME_TO=%s",(field2->data.string_value!=NULL ? field2->data.string_value:""));
			addAPPlib(args,stemp);
			memset(stemp,0,101);
			sprintf(stemp,"VMIME_FROM_NAME=%s",(field3->data.string_value!=NULL ? field3->data.string_value:""));
			addAPPlib(args,stemp);
			memset(stemp,0,101);
			sprintf(stemp,"VMIME_FROM_ADDR=%s",(field4->data.string_value!=NULL ? field4->data.string_value:""));
			addAPPlib(args,stemp);
			ADVExecute2Program("emailuserinfo",NULL,args);
			if(args!=NULL) freeapplib(args);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
			FreeNRDfieldvalue(field3);
			FreeNRDfieldvalue(field4);
		}
		if(h!=NULL) Rfree(h);
	} else if(!strncmp(&image->line[0],"SENDMAIL",8) || 
		!strncmp(&image->line[0],"sendmail",8))
	{
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"NAME OF SENDER"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_name=stralloc(field->data.string_value);
					} else email_vars->from_name=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SENDER EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_addr=stralloc(field->data.string_value);
					} else email_vars->from_addr=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SUBJECT"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->subject=stralloc(field->data.string_value);
					} else email_vars->subject=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BODY"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->body=stralloc(field->data.string_value);
					} else email_vars->body=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"RECIPIENT EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->tolist=stralloc(field->data.string_value);
					} else email_vars->tolist=NULL;
				}
			}
			if(field!=NULL)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				Rfree(field);
			}
			if(!isEMPTY(email_vars->from_name) && !isEMPTY(email_vars->from_addr) && !isEMPTY(email_vars->subject) &&
				!isEMPTY(email_vars->body) && !isEMPTY(email_vars->tolist))
			{
				if(email_vars->srcdomain==NULL)
				{
					temp=GetWebIDGSV();
					sprintf(stemp,"%s.openrda.net",temp);
					email_vars->srcdomain=stralloc(stemp);
				}
				RunVMimeSendmail(email_vars);
			} else {
				prterr("Error: SENDMAIL Requires all 5 parameters established.");
			}
			if(temp!=NULL) Rfree(temp);
			if(email_vars!=NULL) Rfree(email_vars);
		}
	} else if(!strncmp(&image->line[0],"SEND EMAIL",10) || 
		!strncmp(&image->line[0],"send email",10))
	{
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;
			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				field->dlen=0;
				if(!pval->symbol_type)
				{
					SetFieldValue1(field,pval->name,rpt,rrpt);
				} else {
					field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
				}
				if(!RDAstrcmp(pval->fieldname,"NAME OF SENDER"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_name=stralloc(field->data.string_value);
					} else email_vars->from_name=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SENDER EMAIL ADDRESS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->from_addr=stralloc(field->data.string_value);
					} else email_vars->from_addr=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"SUBJECT"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->subject=stralloc(field->data.string_value);
					} else email_vars->subject=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BODY"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->body=stralloc(field->data.string_value);
					} else email_vars->body=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"TO RECIPIENTS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->tolist=stralloc(field->data.string_value);
					} else email_vars->tolist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"CC RECIPIENTS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->cclist=stralloc(field->data.string_value);
					} else email_vars->cclist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"BCC RECIPIENTS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->bcclist=stralloc(field->data.string_value);
					} else email_vars->bcclist=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"IGNORE ERRORS"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->ignerr=atoi(field->data.string_value);
					} else email_vars->ignerr=0;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[0]=stralloc(field->data.string_value);
					} else email_vars->fname[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[1]=stralloc(field->data.string_value);
					} else email_vars->fname[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[2]=stralloc(field->data.string_value);
					} else email_vars->fname[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[3]=stralloc(field->data.string_value);
					} else email_vars->fname[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[4]=stralloc(field->data.string_value);
					} else email_vars->fname[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[5]=stralloc(field->data.string_value);
					} else email_vars->fname[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[6]=stralloc(field->data.string_value);
					} else email_vars->fname[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[7]=stralloc(field->data.string_value);
					} else email_vars->fname[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[8]=stralloc(field->data.string_value);
					} else email_vars->fname[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT FILENAME 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fname[9]=stralloc(field->data.string_value);
					} else email_vars->fname[9]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[0]=stralloc(field->data.string_value);
					} else email_vars->fdata[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[1]=stralloc(field->data.string_value);
					} else email_vars->fdata[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[2]=stralloc(field->data.string_value);
					} else email_vars->fdata[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[3]=stralloc(field->data.string_value);
					} else email_vars->fdata[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[4]=stralloc(field->data.string_value);
					} else email_vars->fdata[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[5]=stralloc(field->data.string_value);
					} else email_vars->fdata[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[6]=stralloc(field->data.string_value);
					} else email_vars->fdata[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[7]=stralloc(field->data.string_value);
					} else email_vars->fdata[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[8]=stralloc(field->data.string_value);
					} else email_vars->fdata[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DATA 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->fdata[9]=stralloc(field->data.string_value);
					} else email_vars->fdata[9]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[0]=stralloc(field->data.string_value);
					} else email_vars->dname[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[1]=stralloc(field->data.string_value);
					} else email_vars->dname[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[2]=stralloc(field->data.string_value);
					} else email_vars->dname[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[3]=stralloc(field->data.string_value);
					} else email_vars->dname[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[4]=stralloc(field->data.string_value);
					} else email_vars->dname[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[5]=stralloc(field->data.string_value);
					} else email_vars->dname[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[6]=stralloc(field->data.string_value);
					} else email_vars->dname[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[7]=stralloc(field->data.string_value);
					} else email_vars->dname[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[8]=stralloc(field->data.string_value);
					} else email_vars->dname[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT DISPLAY NAME 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->dname[9]=stralloc(field->data.string_value);
					} else email_vars->dname[9]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 0"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[0]=stralloc(field->data.string_value);
					} else email_vars->mtype[0]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 1"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[1]=stralloc(field->data.string_value);
					} else email_vars->mtype[1]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 2"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[2]=stralloc(field->data.string_value);
					} else email_vars->mtype[2]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 3"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[3]=stralloc(field->data.string_value);
					} else email_vars->mtype[3]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 4"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[4]=stralloc(field->data.string_value);
					} else email_vars->mtype[4]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 5"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[5]=stralloc(field->data.string_value);
					} else email_vars->mtype[5]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 6"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[6]=stralloc(field->data.string_value);
					} else email_vars->mtype[6]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 7"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[7]=stralloc(field->data.string_value);
					} else email_vars->mtype[7]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 8"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[8]=stralloc(field->data.string_value);
					} else email_vars->mtype[8]=NULL;
				} else if(!RDAstrcmp(pval->fieldname,"ATTACHMENT MIMETYPE 9"))
				{
					if(!isEMPTY(field->data.string_value))
					{
						email_vars->mtype[9]=stralloc(field->data.string_value);
					} else email_vars->mtype[9]=NULL;
				}
			}
			if(field!=NULL)
			{
				if(field->data.string_value!=NULL) Rfree(field->data.string_value);
				Rfree(field);
			}
			if(!isEMPTY(email_vars->from_name) && !isEMPTY(email_vars->from_addr) && !isEMPTY(email_vars->subject) &&
				!isEMPTY(email_vars->body) && ((!isEMPTY(email_vars->tolist)) || (!isEMPTY(email_vars->cclist)) || (!isEMPTY(email_vars->bcclist))))
			{
				if(email_vars->srcdomain==NULL)
				{
					temp=GetWebIDGSV();
					sprintf(stemp,"%s.openrda.net",temp);
					email_vars->srcdomain=stralloc(stemp);
				}
				RunVMimeSendmail(email_vars);
			} else {
				prterr("Error: SEND EMAIL Requires From User Info, Valid Recipients, a Subject and a Body to be set at a minimum.");
			}
			if(temp!=NULL) Rfree(temp);
			if(email_vars!=NULL) Rfree(email_vars);
		}
// This is the end of email functions
	} else if(!strncmp(&image->line[0],"WRITE BLOB",10) || 
		!strncmp(&image->line[0],"write blob",10))
	{
#ifdef USE_MYSQL
		h=HoldReportNew(rrpt,rpt);
		if(image->pvals!=NULL)
		{
			field=Rmalloc(sizeof(NRDfield));
			field->data.string_value=NULL;
			field->dlen=0;
			field->type=VARIABLETEXT;
			field->len=0;

			field2=Rmalloc(sizeof(NRDfield));
			field2->data.string_value=NULL;
			field2->dlen=0;
			field2->type=VARIABLETEXT;
			field2->len=0;

			field3=Rmalloc(sizeof(NRDfield));
			field3->data.string_value=NULL;
			field3->dlen=0;
			field3->type=VARIABLETEXT;
			field3->len=0;

			field4=Rmalloc(sizeof(NRDfield));
			field4->data.string_value=NULL;
			field4->dlen=0;
			field4->type=VARIABLETEXT;
			field4->len=0;

			field5=Rmalloc(sizeof(NRDfield));
			field5->data.string_value=NULL;
			field5->dlen=0;
			field5->type=VARIABLETEXT;
			field5->len=0;

			field6=Rmalloc(sizeof(NRDfield));
			field6->data.string_value=NULL;
			field6->dlen=0;
			field6->type=VARIABLETEXT;
			field6->len=0;

			for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
			{
				if(!RDAstrcmp(pval->fieldname,"DATABASE"))
				{
					if(field->data.string_value!=NULL) Rfree(field->data.string_value);
					field->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field,pval->name,rpt,rrpt);
					} else {
						field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"TABLE"))
				{
					if(field2->data.string_value!=NULL) Rfree(field2->data.string_value);
					field2->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field2,pval->name,rpt,rrpt);
					} else {
						field2->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"FIELD LIST"))
				{
					if(field3->data.string_value!=NULL) Rfree(field3->data.string_value);
					field3->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field3,pval->name,rpt,rrpt);
					} else {
						field3->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"KEY VALUES"))
				{
					if(field4->data.string_value!=NULL) Rfree(field4->data.string_value);
					field4->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field4,pval->name,rpt,rrpt);
					} else {
						field4->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"DECODING METHOD"))
				{
					if(field5->data.string_value!=NULL) Rfree(field5->data.string_value);
					field5->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field5,pval->name,rpt,rrpt);
					} else {
						field5->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
				if(!RDAstrcmp(pval->fieldname,"FILENAME"))
				{
					if(field6->data.string_value!=NULL) Rfree(field6->data.string_value);
					field6->dlen=0;
					if(!pval->symbol_type)
					{
						SetFieldValue1(field6,pval->name,rpt,rrpt);
					} else {
						field6->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
					}
				}
			}
			download_blob_data(field->data.string_value,field2->data.string_value,field3->data.string_value,field4->data.string_value,field5->data.string_value,field6->data.string_value);
			FreeNRDfieldvalue(field);
			FreeNRDfieldvalue(field2);
			FreeNRDfieldvalue(field3);
			FreeNRDfieldvalue(field4);
			FreeNRDfieldvalue(field5);
			FreeNRDfieldvalue(field6);
		}
		if(h!=NULL) Rfree(h);
#endif /* USE_MYSQL */

	} else if(!strncmp(&image->line[0],"HTTP POST",9) || 
		!strncmp(&image->line[0],"http post",9))
	{
		prterr("Error:  wwwdoadd or wwwdorpt should be the selecting executable.");
	} else if(!strncmp(&image->line[0],"HTTP POST-GET",13) || 
		!strncmp(&image->line[0],"http post-get",13))
	{
		prterr("Error:  wwwdoadd or wwwdorpt should be the selecting executable.");
	} else if(!strncmp(&image->line[0],"FTPPUT",6) || 
		!strncmp(&image->line[0],"ftpput",6))
	{
		prterr("Error:  wwwdoadd or wwwdorpt should be the selecting executable.");
	} else if(!strncmp(&image->line[0],"FTPGET",6) || 
		!strncmp(&image->line[0],"ftpget",6))
	{
		prterr("Error:  wwwdoadd or wwwdorpt should be the selecting executable.");
	} else if(!strncmp(&image->line[0],"WRITE_GENERIC_COUNTER",21) ||
		!strncmp(&image->line[0],"write_generic_counter",21))
	{
		temp=image->line+5;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		if(!isEMPTY(modx))
		{
			count=1;
			temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+15);
#ifndef WIN32
			sprintf(temp,"%s/%s/",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
			sprintf(temp,"%s\\%s\\",CURRENTDIRECTORY,modx);
#endif
			if(image->pvals!=NULL)
			{
				pval=image->pvals;
				for(y=0,pval=image->pvals;y<image->numpvals;
					++y,++pval)
				{
					if(!RDAstrcmp(pval->fieldname,"FILENAME"))
					{
						field=Rmalloc(sizeof(NRDfield));
						field->data.string_value=NULL;
						field->dlen=0;
						field->type=VARIABLETEXT;
						field->len=0;
						if(!pval->symbol_type)
						{
							SetFieldValue1(field,pval->name,rpt,rrpt);
						} else {
							field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
						}
						if(!isEMPTY(field->data.string_value))
						{
							filx=stralloc(field->data.string_value);
						} else filx=NULL;
						if(field!=NULL)
						{
							if(field->data.string_value!=NULL) Rfree(field->data.string_value);
							Rfree(field);
						}
					} else if(!RDAstrcmp(pval->fieldname,"GENERIC COUNTER"))
					{
						field=Rmalloc(sizeof(NRDfield));
						field->data.integer_value=Rmalloc(sizeof(int));
						*field->data.integer_value=1;
						field->dlen=sizeof(int);
						field->type=LONGV;
						field->len=8;
						if(!pval->symbol_type)
						{
							SetFieldValue1(field,pval->name,rpt,rrpt);
						} else {
							*field->data.integer_value=PP_EVALUATEint(pval->name,ReportGenSubData1,h);
						}
						count=*field->data.integer_value;
						if(field!=NULL)
						{
							if(field->data.integer_value!=NULL) Rfree(field->data.integer_value);
							Rfree(field);
						}
					}
				}
			}
			if(!isEMPTY(filx))
			{
				getnexttran(temp,filx,count);
			} else {
				prterr("Error: Filename not found for WRITE_GENERIC_COUNTER.");
			}
			if(filx!=NULL) Rfree(filx);
			if(temp!=NULL) Rfree(temp);
			
		}
		if(modx!=NULL) Rfree(modx);
	} else if(!strncmp(&image->line[0],"WRITE",5) || 
		!strncmp(&image->line[0],"write",5)) /* write to data file */
	{
		h=HoldReportNew(rrpt,rpt);
		temp=image->line+5;
		while(iswhite(*temp)) ++temp;
		modx=stripmodulename(temp);
		filx=stripfilename(temp);
		if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
		{
			for(x=0,fx=rrpt->ofiles;x<rrpt->numofiles;++x,++fx)
			{
				if(fx->fileno!=(-1) && !RDAstrcmp(fx->module,modx)
					&& !RDAstrcmp(fx->filename,filx)) break;
			}
			if(x<rrpt->numofiles)
			{
				ZERNRD(fx->fileno);
				if(image->pvals!=NULL)
				{
					for(y=0,pval=image->pvals;y<image->numpvals;
						++y,++pval)
					{
						fieldname=stripfieldname(pval->fieldname);
						field=FLDNRD(fx->fileno,fieldname);
						if(!pval->symbol_type)
						{
							if(!RDAstrcmp(pval->name,"NEXT TRANSACTION NO"))
							{
								trnfield=stralloc(fieldname);
							} else {
								if(field!=NULL)
								{
									SetFieldValue1(field,pval->name,rpt,rrpt);
								} else {
									prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
								}
							}
						} else {
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
										if(field->data.string_value!=NULL)
											Rfree(field->data.string_value);
										field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
										if(field->data.string_value!=NULL)
										{
											field->dlen=RDAstrlen(field->data.string_value)+1;
											if(RDAstrlen(field->data.string_value)>field->len && field->len>0)  field->data.string_value[field->len]=0;
										} else field->dlen=0;
										break;
									case BOOLNS:
										*field->data.string_value=PP_EVALUATEbol(pval->name,ReportGenSubData1,h);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=PP_EVALUATEsht(pval->name,ReportGenSubData1,h);
										break;
									case SLONGV:
									case SCROLLEDLIST:
									case LONGV:
										*field->data.integer_value=PP_EVALUATEint(pval->name,ReportGenSubData1,h);
										break;
									case SDOUBLEV:
									case DOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
										*field->data.float_value=PP_EVALUATEdbl(pval->name,ReportGenSubData1,h);
										if(field->type==DOLLARS || 
											field->type==DOLLARS_NOCENTS ||
											field->type==DOUBLEV ||
											field->type==SDOUBLEV)
										{
											*field->data.float_value=round(*field->data.float_value);
										}
										break;
									default:
										prterr("Error unrecognized field type [%d] for field [%s] in Module [%s] File [%d] [%s].",field->type,field->name,MODULENAME(fx->fileno),fx->fileno,FILENAME(fx->fileno));
										break;
								}
							} else {
								prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
							}
						}
						if(fieldname!=NULL) Rfree(fieldname);
						fieldname=NULL;
					}
					LOCNRDFILE(fx->fileno);
					if(trnfield!=NULL)
					{
						ADVWRTTRANSsec(fx->fileno,1,
							trnfield,NULL,ReportGenSubData1,h); 
						Rfree(trnfield);
						trnfield=NULL;
					} else if(!update)
					{
						ADVWRTTRANSsec(fx->fileno,0,NULL,NULL,
							ReportGenSubData1,h);
					} else if(ADVEQLNRDsec(fx->fileno,1,ReportGenSubData1,h))
					{
						ADVWRTTRANSsec(fx->fileno,0,NULL,NULL,
							ReportGenSubData1,h);
					} else {
						prev=RDATDataNEW(fx->fileno);
						for(y=0,pval=image->pvals;y<image->numpvals;++y,++pval)
						{
							fieldname=stripfieldname(pval->fieldname);
							field=FLDNRD(fx->fileno,fieldname);
							if(!pval->symbol_type)
							{
								if(field!=NULL)
								{
									SetFieldValue1(field,pval->name,rpt,rrpt);
								} else {
									prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
								}
							} else {
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
											if(field->data.string_value!=NULL)
												Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(pval->name,ReportGenSubData1,h);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0)  field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(pval->name,ReportGenSubData1,h);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(pval->name,ReportGenSubData1,h);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(pval->name,ReportGenSubData1,h);
											break;
										case SDOUBLEV:
										case DOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(pval->name,ReportGenSubData1,h);
											if(field->type==DOLLARS || 
												field->type==DOLLARS_NOCENTS ||
												field->type==DOUBLEV ||
												field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s] in Module [%s] File [%d] [%s].",field->type,field->name,MODULENAME(fx->fileno),fx->fileno,FILENAME(fx->fileno));
											break;
									}
								} else {
									prterr("Error field [%s] not found in File [%d] [%s] [%s].",fieldname,fx->fileno,MODULENAME(fx->fileno),FILENAME(fx->fileno));
								}
							}
							if(fieldname!=NULL) Rfree(fieldname);
							fieldname=NULL;
						}
						ADVWRTTRANSsec(fx->fileno,0,NULL,prev,
							ReportGenSubData1,h);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(fx->fileno);
				}
			} else {
				prterr("Error Module [%s] File [%s] not found in open file list.",(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
			}
		} else {
			prterr("Error Module [%s] File [%s] names in error cannot WRITE.",(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
		}
		if(modx!=NULL) Rfree(modx);
		if(filx!=NULL) Rfree(filx);
		if(h!=NULL) Rfree(h);
	} else {
		prterr("Error in syntax of Image Line Definition [%s].",image->line);
	}
}
short xprint_line(RDArunrpt *rrpt,RDAreport *rpt,RDAimage *image,
	short test_page,short currbrk,int line,char *file)
{
	RDAdisplay *d;
	HoldReport *h=NULL;
	short retval=0;
	char bolvalue=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG print_line Print Line for Image Breakname [%s] Line [%s] current breaklevel [%d] at line [%d] program [%s].",image->breakname,image->line,currbrk,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	d=rpt->display;
	if(image->expression!=NULL && RDAstrlen(image->expression)>0)
	{
		h=HoldReportNew(rrpt,rpt);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_outline)
		{
			prterr("DIAG Report Generator and Expression Parser Diagnostics Enabled.");
			diageval=TRUE;diagrptgen=TRUE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		bolvalue=PP_EVALUATEbol(image->expression,ReportGenSubData1,h);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen_outline)
		{
			prterr("DIAG Report Generator and Expression Parser Diagnostics Disabled.");
			diageval=FALSE;diagrptgen=FALSE;
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(bolvalue)
		{
			switch(d->device)
			{
				default:
				case 0: /* printer */
				case 1: /* ascii file */
				case 2: /* update ascii file */
				case 5: /* popen */
					retval=interpretline(rrpt,rpt,image,test_page,currbrk);
					break;
				case 3: /* datafile */
/*lint -e746*/
					rrpt->ProcDataLine(rrpt,rpt,image,FALSE);
/*lint +e746*/
					break;
				case 4: /* update datafile */
/*lint -e746*/
					rrpt->ProcDataLine(rrpt,rpt,image,TRUE);
/*lint +e746*/
					break;
			}
		}
		if(h!=NULL) Rfree(h);
	} else {
		switch(d->device)
		{
			default:
			case 0: /* printer */
			case 1: /* ascii file */
			case 2: /* update ascii file */
			case 5: /* popen */
				retval=interpretline(rrpt,rpt,image,test_page,currbrk);
				break;
			case 3: /* datafile */
/*lint -e746*/
				rrpt->ProcDataLine(rrpt,rpt,image,FALSE);
/*lint +e746*/
				break;
			case 4: /* update datafile */
/*lint -e746*/
				rrpt->ProcDataLine(rrpt,rpt,image,TRUE);
/*lint +e746*/
				break;
		}
	}
	return(retval);
}
