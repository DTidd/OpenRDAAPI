/* mkvirfnc.c - Makes Virtual Fields functions*/
/*lint -library */
#include <mkvir.hpp>
#include <mkmsc.hpp>

/* DCT to review and delete PARTIALLIST */
/*#define PARTIALLIST*/
static void CreateFINSBCvirtual(short fileno,char *modx,char *name)
{
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char *temp=NULL;
	
	memset(stemp,0,101);
	sprintf(stemp,"B/S %s",name);
	d=DFvirtualNEW(modx,stemp);
	if(d!=NULL)
	{
		d->incvir=Rmalloc(sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc(modx);
		i->file=NULL;
		i->keyname=NULL;
		d->vtype=1;
		d->num=1;
		d->type=PLAINTEXT;
		d->length=15;
		d->when=0;
		d->avail2rsrc=FALSE;
		temp=GetSBC(fileno,name,TRUE);
		if(temp!=NULL)
		{
			d->expression=Rmalloc(RDAstrlen(temp)+20);
			sprintf(d->expression,"\"%s\"",(temp!=NULL ? temp:""));
		}
		if(temp!=NULL) Rfree(temp);
		writeDFvirtual(modx,d);
		FreeDFvirtual(d);
	}
}
static void CreateFINSBCvirtuals(char *modx)
{
	short fileno=(-1);

	fileno=OPNNRD("FINMGT","FINSBC");
	if(fileno!=(-1))
	{
		CreateFINSBCvirtual(fileno,modx,"CASH");
		CreateFINSBCvirtual(fileno,modx,"PRE-PAID ITEMS");
		CreateFINSBCvirtual(fileno,modx,"REVENUE");
		CreateFINSBCvirtual(fileno,modx,"EXPENDITURE");
		CreateFINSBCvirtual(fileno,modx,"ACCOUNTS PAYABLE");
		CreateFINSBCvirtual(fileno,modx,"ACCOUNTS RECEIVABLE");
		CreateFINSBCvirtual(fileno,modx,"APPROPRIATION");
		CreateFINSBCvirtual(fileno,modx,"DEPOSITS PAYABLE");
		CreateFINSBCvirtual(fileno,modx,"DISCOUNTS ALLOWED");
		CreateFINSBCvirtual(fileno,modx,"ENCUMBRANCE");
		CreateFINSBCvirtual(fileno,modx,"ESTIMATED REVENUE");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-UNRESERVED");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-RESERVED");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-RESERVED FOR ENCUMBRANCES");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-UNRESERVED, DESIGNATED");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-BUDGETARY CONTROL");
		CreateFINSBCvirtual(fileno,modx,"BUDGETARY FD BAL-RESERVED ENCUMBRANCES");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-NONSPENDABLE");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-ASSIGNED");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-UNASSIGNED");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-COMMITTED");
		CreateFINSBCvirtual(fileno,modx,"FUND BALANCE-RESTRICTED");
		CreateFINSBCvirtual(fileno,modx,"SALARIES / BENEFITS PAYABLE");
		CreateFINSBCvirtual(fileno,modx,"DEFERRED REVENUE");
		CreateFINSBCvirtual(fileno,modx,"DUE TO");
		CreateFINSBCvirtual(fileno,modx,"DUE FROM");
		CreateFINSBCvirtual(fileno,modx,"INVENTORY");
		CreateFINSBCvirtual(fileno,modx,"FIXED ASSETS");
		CreateFINSBCvirtual(fileno,modx,"INVENTORY FIXED ASSETS");
		CreateFINSBCvirtual(fileno,modx,"DEFERRED REVENUE");
		CreateFINSBCvirtual(fileno,modx,"TRANSFER CONTROLS");
		CLSNRD(fileno);
	}
}
int xGetFieldLength(char *modname,char *filename,char *fieldname,int line,char *file)
{
	short engine=0,nofields=0,nokeys=0;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	NRDfield *fields=NULL,*fldx=NULL;
	int y=0,z=0;
	int return_value=(-1);
	

	if(!GETBIN(modname,filename,&engine,&nofields,&fields,&nokeys,&keys))
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);                                      
					}
				}
				Rfree(keyx->part);
			}
		}
		Rfree(keys);
		if(fields!=NULL)
		{
			for(y=0,fldx=fields;y<nofields;++y,++fldx)
			{
				if(fldx->name!=NULL)
				{
					if(!RDAstrcmp(fldx->name,fieldname))
					{
						if(fldx->len>0)
						{
							return_value=fldx->len;
						} else {
							return_value=0;
						}
					}
					Rfree(fldx->name);
				}
			}
			Rfree(fields);
		}
	}
	return(return_value);
}
char *xReturnBEGBALvirtual(short type,char *acctname,char *defname,short *longest_length,int line,char *file)
{
	short q=0,z,x;
	short if_count=0,exp_count=0;
	short part_length=0;
	short total_part_length=0;
	RDAaccpart *part=NULL;
	RDAacct *acc=NULL;
	char temp[15001],*tmpexp=NULL;

	if(BEGINNING_BALANCE_ACCOUNT!=NULL)
	{
		if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
		{
			if(tmpexp!=NULL) Rfree(tmpexp);
			exp_count=0;
			if_count=0;
			*longest_length=0;
			for(z=0,acc=BEGINNING_BALANCE_ACCOUNT->codetype;
				z<BEGINNING_BALANCE_ACCOUNT->num;++z,++acc)
			{
				part_length=0;
				total_part_length=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							if((x+1)==acc->num)
							{
								total_part_length+=part_length;
							} else {
								total_part_length+=part_length+1;
							}
						}
					}
					if(total_part_length>*longest_length) *longest_length=total_part_length;
				}
				memset(temp,0,15001);
				if(type==0)
				{
					sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=%s[1,%d]",
						(defname!=NULL?defname:""),
						z,
						(acctname!=NULL?acctname:""),
						(total_part_length));
				} else {
					sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=%s[%d,1,%d]",
						(defname!=NULL?defname:""),
						z,
						(acctname!=NULL?acctname:""),
						type,(total_part_length));
				}
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			return(tmpexp);
		} else {
			*longest_length=0;
			return(NULL);
		}
	} else {
		*longest_length=0;
		return(NULL);
	}
}
void xCreateBEGBALvirtual(char *modx,char *name,short type,char *acctname,char *defname,int line,char *file)
{
	short length=0;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char *expression=NULL;

	expression=ReturnBEGBALvirtual(type,acctname,defname,&length);
	if(expression!=NULL)
	{
		d=DFvirtualNEW(modx,name);
		d->incvir=Rmalloc(sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc(modx);
		i->file=NULL;
		d->vtype=1;
		d->num=1;
		d->type=PLAINTEXT;
		d->length=length;
		d->when=0;
		if(expression!=NULL)
		{
			d->expression=stralloc(expression);
			Rfree(expression);
		} else d->expression=NULL;
		if(expression!=NULL) Rfree(expression);
		memset(stemp,0,101);
		d->desc=Rmalloc(235);
		sprintf(d->desc,"Beginning Balance Code. You must define a virtual fields named DEFINITION TYPE and ACCOUNT CODE that evaluates to the Account that you want to get the fund from in order for this to evaluate properly.");
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
	}
}
static short FindCommonFront(short deftype)
{
	short z=0;
	RDAaccpart *part=NULL,*Epart=NULL;
	RDAacct *acc=NULL,*Eacc=NULL;

	if(BALANCE_SHEET_ACCOUNT==NULL) return(0);
	if(EXPENDITURE_ACCOUNT==NULL) return(0);
	acc=BALANCE_SHEET_ACCOUNT->codetype+deftype;
	Eacc=EXPENDITURE_ACCOUNT->codetype+deftype;
	for(z=0,part=acc->acct;z<(acc->num-1);++z,++part)
	{
		Epart=Eacc->acct+z;
		if(RDAstrcmp(part->name,Epart->name)) break;
	}
	return(z);
}
char *xReturnFUNDvirtual(short type,char *acctname,char *defname,short *longest_length,int line,char *file)
{
	short q=0,z,x;
	short if_count=0,exp_count=0;
	short part_length=0,COMMON=0,start=0,flen=0;
	short total_part_length=0;
	RDAaccpart *part=NULL,*Epart=NULL,*Rpart=NULL;
	RDAacct *acc=NULL,*Eacc=NULL,*Racc=NULL;
	char temp[15001],*tmpexp=NULL;

	if(XPERT_SETUP->AccountingMethod==0)
	{
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				if(tmpexp!=NULL) Rfree(tmpexp);
				exp_count=0;
				if_count=0;
				*longest_length=0;
				for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
				{
					part_length=0;
					total_part_length=0;
					if(acc->acct!=NULL)
					{
						part_length=0;
						total_part_length=0;
						for(x=0,part=acc->acct;x<(acc->num-1);++x,++part)
						{
							if(part!=NULL)
							{
								if(RDAstrlen(part->name)>part->length)
								{
									part_length=RDAstrlen(part->name);
								} else {
									part_length=part->length;
								}
								total_part_length+=part_length+1;
							}
						}
						if(total_part_length>*longest_length) *longest_length=total_part_length;
					}
					memset(temp,0,15001);
					if(type==0)
					{
						sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=%s[1,%d]",
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							total_part_length);
					} else {
						sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=%s[%d,1,%d]",
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							type,total_part_length);
					}
					if_count+=1;
					if(tmpexp!=NULL)
					{
						tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
						exp_count+=(RDAstrlen(temp)+6);
					} else {
						tmpexp=stralloc(temp);
						exp_count+=RDAstrlen(temp);
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				return(tmpexp);
			} else {
				*longest_length=0;
				return(NULL);
			}
		} else {
			*longest_length=0;
			return(NULL);
		}
	} else if(XPERT_SETUP->AccountingMethod==1)
	{
/* XPERT_SETUP->AccountingMethod==1 */
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				if(tmpexp!=NULL) Rfree(tmpexp);
				exp_count=0;
				if_count=0;
				*longest_length=0;
				for(z=0,Eacc=EXPENDITURE_ACCOUNT->codetype,Racc=REVENUE_ACCOUNT->codetype,acc=BALANCE_SHEET_ACCOUNT->codetype;
					z<BALANCE_SHEET_ACCOUNT->num;++z,++acc,++Eacc,++Racc)
				{
					part_length=0;
					total_part_length=0;
					if(acc->acct!=NULL)
					{
						for(x=0,part=acc->acct;x<(acc->num-1);++x,++part)
						{
							Epart=Eacc->acct+x;
							Rpart=Racc->acct+x;
							if(RDAstrcmp(part->name,Epart->name)) break;
							if(RDAstrcmp(part->name,Rpart->name)) break;
							if(part!=NULL)
							{
								if(RDAstrlen(part->name)>part->length)
								{
									part_length=RDAstrlen(part->name);
								} else {
									part_length=part->length;
								}
								total_part_length+=part_length+1;
							}
						}
						if(total_part_length>*longest_length) *longest_length=total_part_length;
					}
					memset(temp,0,15001);
					if(type==0)
					{
						if(!RDAstrcmp(XPERT_SETUP->state,"AL") && XPERT_SETUP->STUDFIN==TRUE)
						{
						sprintf(temp,"IF(%s=\"\") THEN RETURN_VALUE=\"\" ELSE \nIF(%s=%d) THEN RETURN_VALUE=\"0000-\"+%s[6,%d]",
							(acctname!=NULL?acctname:""),
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							(total_part_length-5));
						} else {
						sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=%s[1,%d]",
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							(total_part_length));
						}
					} else {
						if(!RDAstrcmp(XPERT_SETUP->state,"AL") && XPERT_SETUP->STUDFIN==TRUE)
						{
						sprintf(temp,"IF(%s=\"\") THEN RETURN_VALUE=\"\" ELSE \nIF(%s=%d) THEN RETURN_VALUE=\"0000-\"+%s[%d,6,%d]",
							(acctname!=NULL?acctname:""),
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							type,(total_part_length-5));
						} else {
						sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=%s[%d,1,%d]",
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							type,(total_part_length));
						}
					}
					if_count+=1;
					if(tmpexp!=NULL)
					{
						tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
						exp_count+=(RDAstrlen(temp)+6);
					} else {
						tmpexp=stralloc(temp);
						exp_count+=RDAstrlen(temp);
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(!RDAstrcmp(XPERT_SETUP->state,"AL") && XPERT_SETUP->STUDFIN==TRUE)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				return(tmpexp);
			} else {
				*longest_length=0;
				return(NULL);
			}
		} else {
			*longest_length=0;
			return(NULL);
		}
	} else { /* XPERT_SETUP->AccountingMethod==2 */
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				if(tmpexp!=NULL) Rfree(tmpexp);
				exp_count=0;
				if_count=0;
				*longest_length=0;
				for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
				{
					part_length=0;
					total_part_length=0;
					COMMON=FindCommonFront(z);
					if(acc->acct!=NULL)
					{
						part_length=0;
						total_part_length=0;
						for(x=0,part=acc->acct;x<(acc->num-1);++x,++part)
						{
							if(x==COMMON) break;
							if(part!=NULL)
							{
								if(RDAstrlen(part->name)>part->length)
								{
									part_length=RDAstrlen(part->name);
								} else {
									part_length=part->length;
								}
								total_part_length+=part_length+1;
							}
						}
						start=0;
						memset(stemp,0,101);
						flen=total_part_length;
						for(x=COMMON,part=acc->acct+(COMMON);x<(acc->num-1);++x,++part)
						{
							if(part!=NULL)
							{
								if(RDAstrlen(part->name)>part->length)
								{
									part_length=RDAstrlen(part->name);
									memset(&stemp[start],'0',part->length);
									start+=part->length;
									if(part_length>part->length)
									{
										memset(&stemp[start],' ',(part_length-part->length));
										start+=part_length-part->length;
									}
								} else {
									part_length=part->length;
									memset(&stemp[start],'0',part->length);
									start+=part->length;
								}
								memset(&stemp[start],'-',1);
								start+=1;
								total_part_length+=part_length+1;
							}
						}
						if(total_part_length>*longest_length) *longest_length=total_part_length;
					}
					memset(temp,0,15001);
					if(type==0)
					{
						sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=(%s[1,%d]+\"%s\")",
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							flen,stemp);
					} else {
						sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=(%s[%d,1,%d]+\"%s\")",
							(defname!=NULL?defname:""),
							z,
							(acctname!=NULL?acctname:""),
							type,flen,stemp);
					}
					if_count+=1;
					if(tmpexp!=NULL)
					{
						tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
						exp_count+=(RDAstrlen(temp)+6);
					} else {
						tmpexp=stralloc(temp);
						exp_count+=RDAstrlen(temp);
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				return(tmpexp);
			} else {
				*longest_length=0;
				return(NULL);
			}
		} else {
			*longest_length=0;
			return(NULL);
		}
	}
}
void xCreateFUNDvirtual(char *modx,char *name,short type,char *acctname,char *defname,int line,char *file)
{
	short length=0;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char *expression=NULL;

	expression=ReturnFUNDvirtual(type,acctname,defname,&length);
	if(expression!=NULL)
	{
		d=DFvirtualNEW(modx,name);
		d->incvir=Rmalloc(sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc(modx);
		i->file=NULL;
		d->vtype=1;
		d->num=1;
		d->type=PLAINTEXT;
		d->length=length;
		d->when=0;
		if(expression!=NULL)
		{
			d->expression=stralloc(expression);
			Rfree(expression);
		} else d->expression=NULL;
		if(expression!=NULL) Rfree(expression);
		memset(stemp,0,101);
		d->desc=Rmalloc(235);
		sprintf(d->desc,"Account Code Fund Portion. You must define a virtual fields named DEFINITION TYPE and ACCOUNT CODE that evaluates to the Account that you want to get the fund from in order for this to evaluate properly.");
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
	}
}
char *xReturnCLEARINGCASHSUFFIXvirtual(short type,char *acctname,char *defname,short *longest_length,int line,char *file)
{
	short q=0,z,x,w=0;
	short if_count=0,exp_count=0;
	short part_length=0,start_pos=0;
	char zeros[101];
	short total_part_length=0;
	int temp_count=0;
	RDAaccpart *part=NULL;
	RDAacct *acc=NULL;
	char temp[15001],*tmpexp=NULL;

	if(XPERT_SETUP->AccountingMethod==0 || XPERT_SETUP->AccountingMethod==2)
	{
		*longest_length=0;
		return(NULL);
	} else { /* XPERT_SETUP->AccountingMethod==1 */
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			if(tmpexp!=NULL) Rfree(tmpexp);
			exp_count=0;
			if_count=0;
			*longest_length=0;
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				part_length=0;
				total_part_length=0;
				memset(temp,0,15001);
				sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=",
					(defname!=NULL?defname:""),z);
				temp_count=RDAstrlen(temp);
				if_count+=1;
				if(acc->acct!=NULL)
				{
					for(w=0,part=acc->acct;w<(acc->num);++w,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							if(!RDAstrcmp(part->name,"B/S")) break;
						}
					}
					start_pos=total_part_length+1;
					total_part_length=0;
					part_length=0;
					if((w+1)>=acc->num)
					{
						sprintf(&temp[temp_count],"\"\"");
						temp_count+=2;
					}
					for(x=w+1,part=acc->acct+w+1;x<(acc->num);++x,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							memset(zeros,0,101);
							memset(zeros,'0',part_length);
							zeros[part->length]=0;
							if((x+1)==acc->num)
							{
							sprintf(&temp[temp_count],"+\"%*s\"",
								part_length,zeros);
							} else if(x>(w+1))
							{
							sprintf(&temp[temp_count],"+\"%*s-\"",
								part_length,zeros);
							} else {
							sprintf(&temp[temp_count],"\"-%*s-\"",
								part_length,zeros);
							}
							temp_count=RDAstrlen(temp);
							start_pos+=part_length+1;
						}
					}
					if(total_part_length>*longest_length) *longest_length=total_part_length;
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			return(tmpexp);
		} else {
			*longest_length=0;
			return(NULL);
		}
	} else {
		*longest_length=0;
		return(NULL);
	}
	}
}
void xCreateCLEARINGCASHSUFFIXvirtual(char *modx,char *name,short type,char *acctname,char *defname,int line,char *file)
{
	short length=0;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char *expression=NULL;

	expression=ReturnCLEARINGCASHSUFFIXvirtual(type,acctname,defname,&length);
	d=DFvirtualNEW(modx,name);
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=1;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=length;
	d->when=0;
	if(expression!=NULL)
	{
		d->expression=stralloc(expression);
		Rfree(expression);
	} else d->expression=NULL;
	if(expression!=NULL) Rfree(expression);
	memset(stemp,0,101);
	d->desc=Rmalloc(335);
	sprintf(d->desc,"Clearing Cash Suffix Portion. You must define a virtual fields named DEFINITION TYPE and ACCOUNT CODE that evaluates to the Account that you want to get the clearing cash suffix in order for this to evaluate properly.");
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
}
char *xReturnFUNDSUFFIXvirtual(short type,char *acctname,char *defname,short *longest_length,int line,char *file)
{
	short q=0,z,x,w=0;
	short if_count=0,exp_count=0;
	short part_length=0,start_pos=0;
	char zeros[101];
	short total_part_length=0;
	int temp_count=0;
	RDAaccpart *part=NULL;
	RDAacct *acc=NULL;
	char temp[15001],*tmpexp=NULL;

	if(XPERT_SETUP->AccountingMethod==0 || XPERT_SETUP->AccountingMethod==2)
	{
		*longest_length=0;
		return(NULL);
	} else if(!RDAstrcmp(XPERT_SETUP->state,"AL"))
	{
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			if(tmpexp!=NULL) Rfree(tmpexp);
			exp_count=0;
			if_count=0;
			*longest_length=0;
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				part_length=0;
				total_part_length=0;
				memset(temp,0,15001);
				if(XPERT_SETUP->STUDFIN==FALSE)
				{
				if(type==0)
				{
					sprintf(temp,"IF(%s=\"\") THEN RETURN_VALUE=\"\" ELSE IF((%s=%d) AND (%s[20,4]!=\"6701\") AND (%s[20,4]!=\"7101\") AND (%s[20,4]!=\"7501\")) THEN RETURN_VALUE=",
						(acctname!=NULL ? acctname:""),
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""));
				} else {
					sprintf(temp,"IF(%s=\"\") THEN RETURN_VALUE=\"\" ELSE IF((%s=%d) AND (%s[%d,20,4]!=\"6701\") AND (%s[%d,20,4]!=\"7101\") AND (%s[%d,20,4]!=\"7501\")) THEN RETURN_VALUE=",
						(acctname!=NULL ? acctname:""),
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type);
				}
				} else {
				if(type==0)
				{
					sprintf(temp,"IF(%s=\"\") THEN RETURN_VALUE=\"\" ELSE IF((%s=%d) AND (%s[25,4]!=\"6701\") AND (%s[25,4]!=\"7101\") AND (%s[25,4]!=\"7501\")) THEN RETURN_VALUE=",
						(acctname!=NULL ? acctname:""),
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""));
				} else {
					sprintf(temp,"IF(%s=\"\") THEN RETURN_VALUE=\"\" ELSE IF((%s=%d) AND (%s[%d,25,4]!=\"6701\") AND (%s[%d,25,4]!=\"7101\") AND (%s[%d,25,4]!=\"7501\")) THEN RETURN_VALUE=",
						(acctname!=NULL ? acctname:""),
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type);
				}
				}
				temp_count=RDAstrlen(temp);
				if_count+=2;
				if(acc->acct!=NULL)
				{
					for(w=0,part=acc->acct;w<(acc->num);++w,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							if(!RDAstrcmp(part->name,"B/S")) break;
						}
					}
					start_pos=total_part_length+1;
					total_part_length=0;
					part_length=0;
					for(x=w+1,part=acc->acct+w+1;x<(acc->num);++x,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							memset(zeros,0,101);
							memset(zeros,'0',part_length);
							zeros[part->length]=0;
							if(part->force_zero || part->default_zero)
							{
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+\"%*s\"",
									part_length,zeros);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+\"%*s-\"",
									part_length,zeros);
								} else {
								sprintf(&temp[temp_count],"\"-%*s-\"",
									part_length,zeros);
								}
							} else if(type==0)
							{
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length+1);
								} else {
								sprintf(&temp[temp_count],"\"-\"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length+1);
								}
							} else {
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length+1);
								} else {
								sprintf(&temp[temp_count],"\"-\"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length+1);
								}
							}
							temp_count=RDAstrlen(temp);
							start_pos+=part_length+1;
						}
					}
					if(total_part_length>*longest_length) *longest_length=total_part_length;
				}
				if(XPERT_SETUP->STUDFIN==FALSE)
				{
				if(type==0)
				{
					sprintf(&temp[temp_count]," ELSE \nIF((%s=%d) AND ((%s[20,4]=\"6701\") OR (%s[20,4]=\"7101\") OR (%s[20,4]=\"7501\"))) THEN RETURN_VALUE=",
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""));
				} else {
					sprintf(&temp[temp_count]," ELSE \nIF((%s=%d) AND ((%s[%d,20,4]=\"6701\") OR (%s[%d,20,4]=\"7101\") OR (%s[%d,20,4]=\"7501\"))) THEN RETURN_VALUE=",
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type);
				}
				} else {
				if(type==0)
				{
					sprintf(&temp[temp_count]," ELSE \nIF((%s=%d) AND ((%s[25,4]=\"6701\") OR (%s[25,4]=\"7101\") OR (%s[25,4]=\"7501\"))) THEN RETURN_VALUE=",
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""),(acctname!=NULL ? acctname:""));
				} else {
					sprintf(&temp[temp_count]," ELSE \nIF((%s=%d) AND ((%s[%d,25,4]=\"6701\") OR (%s[%d,25,4]=\"7101\") OR (%s[%d,25,4]=\"7501\"))) THEN RETURN_VALUE=",
						(defname!=NULL?defname:""),z,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type,(acctname!=NULL ? acctname:""),type);
				}
				}
				temp_count=RDAstrlen(temp);
				total_part_length=0;
				if_count+=1;
				if(acc->acct!=NULL)
				{
					for(w=0,part=acc->acct;w<(acc->num);++w,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							if(!RDAstrcmp(part->name,"B/S")) break;
						}
					}
					start_pos=total_part_length+1;
					total_part_length=0;
					part_length=0;
					for(x=w+1,part=acc->acct+w+1;x<(acc->num);++x,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							memset(zeros,0,101);
							memset(zeros,'0',part_length);
							zeros[part->length]=0;
							if(((part->force_zero || part->default_zero)) && RDAstrcmp(part->name,"CCTR"))
							{
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+\"%*s\"",
									part_length,zeros);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+\"%*s-\"",
									part_length,zeros);
								} else {
								sprintf(&temp[temp_count],"\"-%*s-\"",
									part_length,zeros);
								}
							} else if(type==0)
							{
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length+1);
								} else {
								sprintf(&temp[temp_count],"\"-\"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length+1);
								}
							} else {
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length+1);
								} else {
								sprintf(&temp[temp_count],"\"-\"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length+1);
								}
							}
							temp_count=RDAstrlen(temp);
							start_pos+=part_length+1;
						}
					}
					if(total_part_length>*longest_length) *longest_length=total_part_length;
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count]," \nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count]," \nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			return(tmpexp);
		} else {
			*longest_length=0;
			return(NULL);
		}
	} else {
		*longest_length=0;
		return(NULL);
	}
			
	} else { /* XPERT_SETUP->AccountingMethod==1 */
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			if(tmpexp!=NULL) Rfree(tmpexp);
			exp_count=0;
			if_count=0;
			*longest_length=0;
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				part_length=0;
				total_part_length=0;
				memset(temp,0,15001);
				sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=",
					(defname!=NULL?defname:""),z);
				temp_count=RDAstrlen(temp);
				if_count+=1;
				if(acc->acct!=NULL)
				{
					for(w=0,part=acc->acct;w<(acc->num);++w,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							if(!RDAstrcmp(part->name,"B/S")) break;
						}
					}
					start_pos=total_part_length+1;
					total_part_length=0;
					part_length=0;
					if((w+1)>=acc->num) 
					{
						sprintf(&temp[temp_count],"\"\"");
						temp_count+=2;
					}
					for(x=w+1,part=acc->acct+w+1;x<(acc->num);++x,++part)
					{
						if(part!=NULL)
						{
							if(RDAstrlen(part->name)>part->length)
							{
								part_length=RDAstrlen(part->name);
							} else {
								part_length=part->length;
							}
							total_part_length+=part_length+1;
							memset(zeros,0,101);
							memset(zeros,'0',part_length);
							zeros[part->length]=0;
							if(part->force_zero || part->default_zero)
							{
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+\"%*s\"",
									part_length,zeros);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+\"%*s-\"",
									part_length,zeros);
								} else {
								sprintf(&temp[temp_count],"\"-%*s-\"",
									part_length,zeros);
								}
							} else if(type==0)
							{
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length+1);
								} else {
								sprintf(&temp[temp_count],"\"-\"+%s[%d,%d]",
									(acctname!=NULL?acctname:""),
									start_pos,part_length+1);
								}
							} else {
								if((x+1)==acc->num)
								{
								sprintf(&temp[temp_count],"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length);
								} else if(x>(w+1))
								{
								sprintf(&temp[temp_count],"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length+1);
								} else {
								sprintf(&temp[temp_count],"\"-\"+%s[%d,%d,%d]",
									(acctname!=NULL?acctname:""),
									type,start_pos,part_length+1);
								}
							}
							temp_count=RDAstrlen(temp);
							start_pos+=part_length+1;
						}
					}
					if(total_part_length>*longest_length) *longest_length=total_part_length;
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count]," \nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count]," \nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			return(tmpexp);
		} else {
			*longest_length=0;
			return(NULL);
		}
	} else {
		*longest_length=0;
		return(NULL);
	}
	}
}
void xCreateFUNDSUFFIXvirtual(char *modx,char *name,short type,char *acctname,char *defname,int line,char *file)
{
	short length=0;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char *expression=NULL;

	expression=ReturnFUNDSUFFIXvirtual(type,acctname,defname,&length);
	d=DFvirtualNEW(modx,name);
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=1;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=length;
	d->when=0;
	if(expression!=NULL)
	{
		d->expression=stralloc(expression);
		Rfree(expression);
	} else d->expression=NULL;
	if(expression!=NULL) Rfree(expression);
	memset(stemp,0,101);
	d->desc=Rmalloc(335);
	sprintf(d->desc,"Account Code Fund Suffix Portion. You must define a virtual fields named DEFINITION TYPE and ACCOUNT CODE that evaluates to the Account that you want to get the fund suffix from in order for this to evaluate properly.");
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
}
char *xReturnACCMINUS1virtual(short type,char *acctname,char *defname,short *longest_length,int line,char *file)
{
	short q=0,z,y;
	short if_count=0,exp_count=0;
	RDAaccpart *p=NULL;
/*
	RDAaccpart *part=NULL;
*/
	RDAacct *acc=NULL;
	int acc_count=0,accminus1_longest=0;
	char temp[15001],*tmpexp=NULL;
	char prev_defused=FALSE;

	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
			tmpexp=stralloc(temp);
			exp_count+=RDAstrlen(temp);

			if_count=0;
			prev_defused=FALSE;
			if(EXPENDITURE_ACCOUNT->num>0)
			{
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
/*
						part=acc->acct+((acc->num-1));
*/
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}


						memset(temp,0,15001);
						sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=[ACCOUNT CODE][1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						if((acc_count-1)>accminus1_longest) accminus1_longest=(acc_count-1);
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
					if(y==(EXPENDITURE_ACCOUNT->num-1))
					{
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
					}
				}
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			if_count=0;
			memset(temp,0,15001);
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}

			prev_defused=FALSE;
			if(REVENUE_ACCOUNT->num>0)
			{
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
/*
						part=acc->acct+((acc->num-1));
*/
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=[ACCOUNT CODE][1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						if((acc_count-1)>accminus1_longest) accminus1_longest=(acc_count-1);

						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
					if(y==(REVENUE_ACCOUNT->num-1))
					{
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
					}
				}
			}
		}
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			if_count=0;
			memset(temp,0,15001);
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}


			prev_defused=FALSE;
			if(BALANCE_SHEET_ACCOUNT->num>0)
			{
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
/*
						part=acc->acct+((acc->num-1));
*/
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=[ACCOUNT CODE][1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						if((acc_count-1)>accminus1_longest) accminus1_longest=(acc_count-1);

						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
					if(y==(BALANCE_SHEET_ACCOUNT->num-1))
					{
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
					}
				}
			}
		}
	}
	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=\"\"");
	tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
	sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);
	return(tmpexp);

}
void xCreateACCMINUS1virtual(char *modx,char *name,short type,char *acctname,char *defname,int line,char *file)
{
	short length=0;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char *expression=NULL;

	expression=ReturnACCMINUS1virtual(type,acctname,defname,&length);
	if(!isEMPTY(expression))
	{
		d=DFvirtualNEW(modx,name);
		d->incvir=Rmalloc(sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc(modx);
		i->file=NULL;
		d->vtype=1;
		d->num=1;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		if(expression!=NULL)
		{
			d->expression=stralloc(expression);
			Rfree(expression);
		} else d->expression=NULL;
		if(expression!=NULL) Rfree(expression);
		memset(stemp,0,101);
		d->desc=Rmalloc(201);
		sprintf(d->desc,"Account Code Minus Last Part. You must define virtual fields named DEFINITION TYPE and ACCOUNT CODE that evaluates to the Account that you want to get the from in order for this to evaluate properly.");
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
	}
}
static char *padstr(int length,char padchar)
{
	char *return_string=NULL;
	int x=0;

	if(length>0)
	{
		return_string=Rmalloc(length+1);
		memset(return_string,0,length+1);
		for(x=0;x<length;++x)
		{
			return_string[x]=padchar;
		}
	} else {
		return_string=stralloc("");
	}
	return(return_string);
}
void xCreateCOAs(char *modx,int line,char *file)
{
	short q=0,z,x,y;
	short if_count=0;
	short if_count_2=0;
	short exp_count=0,desc_count=0,acc_count=0,no_parts=0;
	short struct_count=0,label_count=0,partlabel_count=0;
	short label2_count=0,label3_count=0,label4_count=0;
/*
	short label_count5=0;
*/
	short longest_length=0,exp_no_parts=0,rev_no_parts=0,bal_no_parts=0;
	short accminus1_longest=0;
	RDAaccpart *part=NULL,*p=NULL;
	RDAacct *acc=NULL;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char temp[15001],*tmpexp=NULL,*tmpdesc=NULL;
	char *exp_label=NULL,*tmpstruct=NULL,*tmplabel=NULL,*tmppartlabel=NULL;
	char *tmplabel2=NULL,*tmplabel3=NULL,*tmplabel4=NULL,*c=NULL;
/*
	char *tmplabel5=NULL;
*/
	int field_length=0;
	char prev_defused=FALSE;
	char prev_defused_2=FALSE;
	char *vname=NULL;
	short endlength=0;
	char *pad_string=NULL;


	CreateFINSBCvirtuals(modx);
	d=DFvirtualNEW(modx,"EXECUTED BY");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	field_length=GetFieldLength("SECURITY","USERS","USER IDENTIFICATION");
	if(field_length<1)
	{
		d->type=VARIABLETEXT;
		d->length=0;
	} else {
		d->type=PLAINTEXT;
		d->length=field_length+15;
	}
	d->when=1;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Pre-Titled Executing User's Identification.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"CURRENT FISCAL MONTH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	d->type=SHORTV;
	d->length=2;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([SYSTEM DATE][1,2]=\"01\") THEN RETURN_VALUE=(1) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"02\") THEN RETURN_VALUE=(2) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"03\") THEN RETURN_VALUE=(3) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"04\") THEN RETURN_VALUE=(4) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"05\") THEN RETURN_VALUE=(5) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"06\") THEN RETURN_VALUE=(6) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"07\") THEN RETURN_VALUE=(7) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"08\") THEN RETURN_VALUE=(8) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"09\") THEN RETURN_VALUE=(9) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"10\") THEN RETURN_VALUE=(10) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"11\") THEN RETURN_VALUE=(11) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"12\") THEN RETURN_VALUE=(12) ",
		"\nFI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Current Fiscal Month with Pre-Year=0,January=1, February=2,... calculated from the [SYSTEM DATE] virtual field.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"CURRENT CALENDAR MONTH");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	d->type=SHORTV;
	d->length=2;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s %s %s %s %s",
		"IF ([SYSTEM DATE][1,2]=\"01\") THEN RETURN_VALUE=(0) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"02\") THEN RETURN_VALUE=(1) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"03\") THEN RETURN_VALUE=(2) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"04\") THEN RETURN_VALUE=(3) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"05\") THEN RETURN_VALUE=(4) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"06\") THEN RETURN_VALUE=(5) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"07\") THEN RETURN_VALUE=(6) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"08\") THEN RETURN_VALUE=(7) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"09\") THEN RETURN_VALUE=(8) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"10\") THEN RETURN_VALUE=(9) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"11\") THEN RETURN_VALUE=(10) ",
		"\nELSE IF ([SYSTEM DATE][1,2]=\"12\") THEN RETURN_VALUE=(11) ",
		"\nFI FI FI FI FI FI FI FI FI FI FI FI ");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Current Calendar Month with January=0, February=1,... calculated from the [SYSTEM DATE] virtual field .");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	d=DFvirtualNEW(modx,"ORGANIZATIONAL ADDRESS 1");
	d->incvir=Rmalloc(sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc(modx);
	i->file=NULL;
	d->vtype=0;
	d->num=1;
	d->type=PLAINTEXT;
	longest_length=(RDAstrlen(XPERT_SETUP->addr1)+RDAstrlen(XPERT_SETUP->addr2)+RDAstrlen(XPERT_SETUP->city)+RDAstrlen(XPERT_SETUP->state)+RDAstrlen(XPERT_SETUP->zip)+4);
	d->length=longest_length;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"%s %s %s %s %s %s %s %s %s%s %s %s %s %s",
	"IF(([ORGANIZATION ADDRESS #1]=\"\") AND",
	"([ORGANIZATION ADDRESS #2]=\"\") AND ",
	"([ORGANIZATION CITY]=\"\") AND ",
	"([ORGANIZATION STATE]=\"\") AND ",
	"([ORGANIZATION ZIP]=\"\"))",
	"THEN RETURN_VALUE=\"\"",
	"ELSE IF(([ORGANIZATION ADDRESS #1]=\"\") AND",
	"([ORGANIZATION ADDRESS #2]=\"\"))",
	"THEN RETURN_VALUE=([ORGANIZATION CITY]+\", \"+",
	"[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP])",
	"ELSE IF([ORGANIZATION ADDRESS #1]=\"\")",
	"THEN RETURN_VALUE=[ORGANIZATION ADDRESS #2]",
	"ELSE RETURN_VALUE=[ORGANIZATION ADDRESS #1]",
	"FI FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field formats the Organization's Address line 1.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
        d=DFvirtualNEW(modx,"ORGANIZATIONAL ADDRESS 2");
        d->incvir=Rmalloc(sizeof(DFincvir));
        i=d->incvir;
        i->module=stralloc(modx);
        i->file=NULL;
        d->vtype=0;
        d->num=1;
        d->type=PLAINTEXT;
	longest_length=(RDAstrlen(XPERT_SETUP->addr1)+RDAstrlen(XPERT_SETUP->addr2)+RDAstrlen(XPERT_SETUP->city)+RDAstrlen(XPERT_SETUP->state)+RDAstrlen(XPERT_SETUP->zip)+4);
        d->length=longest_length;
        d->when=0;
        d->avail2rsrc=TRUE;
        memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"IF(([ORGANIZATION ADDRESS #1]=\"\") AND ([ORGANIZATION ADDRESS #2]=\"\")) THEN RETURN_VALUE=\"\" ELS",
		"E IF((([ORGANIZATION ADDRESS #1]=\"\") OR ([ORGANIZATION ADDRESS #2]=\"\")) AND ([ORGANIZATION CITY]",
		"=\"\") AND ([ORGANIZATION STATE]=\"\") AND ([ORGANIZATION ZIP]=\"\")) THEN RETURN_VALUE=(\"\") ELSE ",
		"IF(([ORGANIZATION ADDRESS #2]=\"\") OR ([ORGANIZATION ADDRESS #1]=\"\")) THEN RETURN_VALUE=([ORGANIZ",
		"ATION CITY]+\", \"+[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP]) ELSE RETURN_VALUE=[ORGANIZATION A",
		"DDRESS #2] FI FI FI");
        d->expression=stralloc(temp);
        memset(temp,0,15000);
        sprintf(temp,"This virtual field formats the Organizations's Address line 2.");
        d->desc=stralloc(temp);
        memset(temp,0,15000);
        writeDFvirtual(modx,d);
        if(d!=NULL) FreeDFvirtual(d);
        d=DFvirtualNEW(modx,"ORGANIZATIONAL ADDRESS 3");
        d->incvir=Rmalloc(sizeof(DFincvir));
        i=d->incvir;
        i->module=stralloc(modx);
        i->file=NULL;
        d->vtype=0;
        d->num=1;
        d->type=PLAINTEXT;
	longest_length=(RDAstrlen(XPERT_SETUP->addr1)+RDAstrlen(XPERT_SETUP->addr2)+RDAstrlen(XPERT_SETUP->city)+RDAstrlen(XPERT_SETUP->state)+RDAstrlen(XPERT_SETUP->zip)+4);
        d->length=longest_length;
        d->when=0;
        d->avail2rsrc=TRUE;
        memset(temp,0,15000);
        sprintf(temp,"%s %s %s %s %s %s %s%s %s",
                "IF(([ORGANIZATION ADDRESS #1]=\"\")",
                "OR ([ORGANIZATION ADDRESS #2]=\"\")",
                "OR (([ORGANIZATION CITY]=\"\") AND",
                "([ORGANIZATION STATE]=\"\") AND",
                "([ORGANIZATION ZIP]=\"\")))",
                "THEN RETURN_VALUE=\"\"",
                "ELSE RETURN_VALUE=([ORGANIZATION CITY]+\", \"+",
                "[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP])",
                "FI");
        d->expression=stralloc(temp);
        memset(temp,0,15000);
        sprintf(temp,"This virtual field formats the Organizations's Address line 3.");
        d->desc=stralloc(temp);
        memset(temp,0,15000);
        writeDFvirtual(modx,d);
        if(d!=NULL) FreeDFvirtual(d);

	d=DFvirtualNEW(modx,"ACCOUNT TYPE");
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=SHORTV;
	d->length=1;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"IF([FINMGT][FINEACC][ACCOUNT CODE]) THEN RETURN_VALUE=0 ELSE IF([FINMGT][FINRACC][ACCOUNT CODE]) THEN RETURN_VALUE=1 ELSE RETURN_VALUE=2 FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Account Type.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	CreateFUNDvirtual(modx,"ACCOUNT FUND",0,"[ACCOUNT CODE]","[DEFINITION TYPE]");
	CreateFUNDSUFFIXvirtual(modx,"ACCOUNT FUND SUFFIX",0,"[ACCOUNT CODE]","[DEFINITION TYPE]");
	CreateCLEARINGCASHSUFFIXvirtual(modx,"CLEARING CASH SUFFIX",0,"[ACCOUNT CODE]","[DEFINITION TYPE]");
	CreateBEGBALvirtual(modx,"BEGINNING BALANCE CODE",0,"[ACCOUNT CODE]","[DEFINITION TYPE]");
	CreateACCMINUS1virtual(modx,"ACCOUNT CODE MINUS 1 PART VIR",0,"[ACCOUNT CODE]","[DEFINITION TYPE]");
#ifndef PARTIALLIST
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			if(XPERT_SETUP->software_type!=2)
			{
			for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
				y<EXPENDITURE_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+23);
						sprintf(vname,"EXPENDITURE CODE %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINEACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+44);
						sprintf(d->desc,"%s Part of the %s Expenditure Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+31);
						sprintf(vname,"EXPENDITURE DESCRIPTION %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINEACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+65);
						sprintf(d->desc,"Description For %s Part of the %s Expenditure Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);



			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(35);
			sprintf(vname,"EXPENDITURE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			if(tmpexp!=NULL) Rfree(tmpexp);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);




			vname=Rmalloc(20);
			sprintf(vname,"EXPENDITURE LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						if(acc->num>x)
						{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=0;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(p->length>=RDAstrlen(p->name))
							{
								acc_count+=p->length+1;
							} else {
								acc_count+=RDAstrlen(p->name)+1;
							}
						}
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count+1,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
							desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(35);
				sprintf(vname,"EXPENDITURE CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINEACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				d->avail2rsrc=TRUE;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Expenditure Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(45);
				sprintf(vname,"EXPENDITURE DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINEACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Expenditure Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}


			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				for(c=exp_label;*c;++c) if((*c=='N') || (*c=='A') || (*c=='X')) *c=' ';
				memset(temp,0,15001);
				sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(45);
			sprintf(vname,"BLANK EXPENDITURE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc(modx);
			i->file=NULL;
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			if(tmpexp!=NULL) Rfree(tmpexp);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents a Blank Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);

			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				for(c=exp_label;*c;++c) if((*c=='N') || (*c=='A') || (*c=='X')) *c='0';
				memset(temp,0,15001);
				sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(45);
			sprintf(vname,"ZEROED EXPENDITURE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc(modx);
			i->file=NULL;
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			if(tmpexp!=NULL) Rfree(tmpexp);
			d->desc=Rmalloc(140);
			sprintf(d->desc,"This virtual field represents a Zeroed Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);

		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			if(XPERT_SETUP->software_type!=2)
			{
			for(y=0,acc=REVENUE_ACCOUNT->codetype;
				y<REVENUE_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+19);
						sprintf(vname,"REVENUE CODE %s %s",acc->name,
							part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINRACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+44);
						sprintf(d->desc,"%s Part of the %s Revenue Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+26);
						sprintf(vname,"REVENUE DESCRIPTION %s %s",
							acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINRACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+55);
						sprintf(d->desc,"Description For %s Part of the %s Revenue Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=REVENUE_ACCOUNT->codetype;
				z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(30);
			sprintf(vname,"REVENUE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Revenue Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			vname=Rmalloc(15);
			sprintf(vname,"REVENUE LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Revenue Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						if(acc->num>x)
						{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=0;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(p->length>=RDAstrlen(p->name))
							{
								acc_count+=p->length+1;
							} else {
								acc_count+=RDAstrlen(p->name)+1;
							}
						}
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count+1,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(31);
				sprintf(vname,"REVENUE CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINRACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				d->avail2rsrc=TRUE;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Revenue Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(41);
				sprintf(vname,"REVENUE DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINRACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Revenue Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			if(XPERT_SETUP->software_type!=2)
			{
			for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+
							RDAstrlen(acc->name)+25);
						sprintf(vname,"BALANCE SHEET CODE %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINBACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+60);
						sprintf(d->desc,"%s Part of the %s Balance Sheet Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+32);
						sprintf(vname,"BALANCE SHEET DESCRIPTION %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINBACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+58);
						sprintf(d->desc,"Description For %s Part of the %s Balance Sheet Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}	
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(35);
			sprintf(vname,"BALANCE SHEET ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			if(tmpexp!=NULL) Rfree(tmpexp);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Balance Sheet Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			vname=Rmalloc(20);
			sprintf(vname,"BALANCE SHEET LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Balance Sheet Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						if(acc->num>x)
						{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}	
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(40);
				sprintf(vname,"BALANCE SHEET CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINBACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				d->avail2rsrc=TRUE;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Balance Sheet Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);

				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(50);
				sprintf(vname,"BALANCE SHEET DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINBACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Balance Sheet Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
#endif /* End PARTIALLIST */
	no_parts=0;
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_no_parts=acc->num;
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=REVENUE_ACCOUNT->codetype;
				z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				rev_no_parts=acc->num;
			}
		}
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				bal_no_parts=acc->num;
			}
		}
	}
	for(x=0;x<no_parts;++x)
	{
		exp_count=0;
		desc_count=0;
		struct_count=0;
		label_count=0;
		partlabel_count=0;
		if_count=0;
		if(tmpexp!=NULL) Rfree(tmpexp);
		if(tmpdesc!=NULL) Rfree(tmpdesc); 
		if(tmpstruct!=NULL) Rfree(tmpstruct); 
		if(tmplabel!=NULL) Rfree(tmplabel); 
		if(tmppartlabel!=NULL) Rfree(tmppartlabel); 
		longest_length=0;
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
				tmpexp=stralloc(temp);
				tmpdesc=stralloc(temp);
				if(x==0)
				{
				tmplabel=stralloc(temp);
				tmpstruct=stralloc(temp);
				}
				tmppartlabel=stralloc(temp);
				exp_count+=RDAstrlen(temp);
				desc_count+=RDAstrlen(temp);
				if(x==0)
				{
				label_count+=RDAstrlen(temp);
				struct_count+=RDAstrlen(temp);
				}
				partlabel_count+=RDAstrlen(temp);
				if_count=0;
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<exp_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<exp_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<exp_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<exp_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<exp_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(x==0)
				{
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				if(tmpstruct!=NULL)
				{
					tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
					sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
					struct_count+=RDAstrlen(temp)+5;
				} else {
					tmpstruct=stralloc(temp);
					struct_count+=RDAstrlen(temp);
				}
				}
				if(tmppartlabel!=NULL)
				{
					tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+6);
					sprintf(&tmppartlabel[partlabel_count],"ELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+5;
				} else {
					tmppartlabel=stralloc(temp);
					partlabel_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<rev_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<rev_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<rev_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<rev_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<rev_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=2) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(x==0)
				{
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				if(tmpstruct!=NULL)
				{
					tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
					sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
					struct_count+=RDAstrlen(temp)+5;
				} else {
					tmpstruct=stralloc(temp);
					struct_count+=RDAstrlen(temp);
				}
				}
				if(tmppartlabel!=NULL)
				{
					tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+6);
					sprintf(&tmppartlabel[partlabel_count],"ELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+5;
				} else {
					tmppartlabel=stralloc(temp);
					partlabel_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						acc_count=1;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<bal_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<bal_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<bal_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<bal_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<bal_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(tmpexp!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
			sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpdesc!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+16);
			sprintf(&tmpdesc[desc_count],"ELSE %s FI FI FI\n",temp);
		}
		if(x==0)
		{
		if(tmplabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
			sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpstruct!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+16);
			sprintf(&tmpstruct[struct_count],"ELSE %s FI FI FI\n",temp);
		}
		}
		if(tmppartlabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+16);
			sprintf(&tmppartlabel[partlabel_count],"ELSE %s FI FI FI\n",temp);
		}
		memset(temp,0,15001);
		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d CODE",x);
		d=DFvirtualNEW(modx,stemp);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=longest_length;
		d->when=0;
		d->avail2rsrc=TRUE;
		if(tmpexp!=NULL)
		{
			d->expression=stralloc(tmpexp);
			Rfree(tmpexp);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code Dimension %d representing all account type and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d DESCRIPTION",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=80;
		d->when=0;
		if(tmpdesc!=NULL)
		{
			d->expression=stralloc(tmpdesc);
			Rfree(tmpdesc);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Description for Code Dimension %d representing all accounts and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=SHORTV;
		d->length=2;
		d->when=0;
		sprintf(stemp,"%d",x);
		d->expression=stralloc(stemp);
		memset(stemp,0,101);
		memset(temp,0,15001);
		sprintf(temp,"Represents Dimension number %d for use in the Chart of Accounts.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
		memset(stemp,0,101);
		if(x==0)
		{
			memset(stemp,0,101);
			sprintf(stemp,"ACCOUNT CODE LABEL");
			d=DFvirtualNEW(modx,stemp);
			memset(stemp,0,101);
			d->incvir=Rmalloc(4*sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINCOA");
			i->keyname=NULL;
			d->vtype=1;
			d->num=4;
			d->type=PLAINTEXT;
			d->length=40;
			d->when=0;
			d->avail2rsrc=TRUE;
			if(tmplabel!=NULL)
			{
				d->expression=stralloc(tmplabel);
				Rfree(tmplabel);
			} else d->expression=NULL;
			memset(temp,0,15001);
			sprintf(temp,"Account Label for each Account Type.");
			d->desc=stralloc(temp);
			memset(temp,0,15001);
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			memset(stemp,0,101);
			sprintf(stemp,"ACCOUNT CODE STRUCTURE");
			d=DFvirtualNEW(modx,stemp);
			memset(stemp,0,101);
			d->incvir=Rmalloc(4*sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINEACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINBACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINCOA");
			i->keyname=NULL;
			d->vtype=1;
			d->num=4;
			d->type=PLAINTEXT;
			d->length=40;
			d->when=0;
			d->avail2rsrc=FALSE;
			if(tmpstruct!=NULL)
			{
				d->expression=stralloc(tmpstruct);
				Rfree(tmpstruct);
			} else d->expression=NULL;
			memset(temp,0,15001);
			sprintf(temp,"Account Code Structure for each Account Type.");
			d->desc=stralloc(temp);
			memset(temp,0,15001);
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
		}
		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d LABEL",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		if(tmppartlabel!=NULL)
		{
			d->expression=stralloc(tmppartlabel);
			Rfree(tmppartlabel);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Account Part Name for Dimension %d or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

	}

/* NEW DIMENSION #X CODE THRU Y virtual fields */
	for(endlength=0;endlength<10;++endlength)
	{
	for(x=0;x<no_parts;++x)
	{
		exp_count=0;
		desc_count=0;
		label_count=0;
		partlabel_count=0;
		if_count=0;
		if(tmpexp!=NULL) Rfree(tmpexp);
		if(tmpdesc!=NULL) Rfree(tmpdesc); 
		if(tmplabel!=NULL) Rfree(tmplabel); 
		longest_length=0;
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
				tmpexp=stralloc(temp);
				tmpdesc=stralloc(temp);
				tmplabel=stralloc(temp);
				exp_count+=RDAstrlen(temp);
				desc_count+=RDAstrlen(temp);
				label_count+=RDAstrlen(temp);
				if_count=0;
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,part->length,x+1);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]+\"%s\"+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""),(x+1));
							} else {
								sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(x+1));
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,(x+1),acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<exp_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<exp_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(tmplabel!=NULL && x<exp_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,part->length,x+1);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]+\"%s\"+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""),(x+1));
							} else {
								sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(x+1));
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,(x+1),acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<rev_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<rev_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(tmplabel!=NULL && x<rev_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=2) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						acc_count=1;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,part->length,x+1);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]+\"%s\"+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""),(x+1));
							} else {
								sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,x+1);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,(x+1),acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<bal_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<bal_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(tmplabel!=NULL && x<bal_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(tmpexp!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
			sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpdesc!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+16);
			sprintf(&tmpdesc[desc_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmplabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
			sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);
		}

		memset(temp,0,15001);
		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d CODE/DESC THRU %d",x,endlength);
		d=DFvirtualNEW(modx,stemp);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=80+longest_length;
		d->when=0;
		d->avail2rsrc=FALSE;
		if(tmpexp!=NULL)
		{
			d->expression=stralloc(tmpexp);
			Rfree(tmpexp);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code+Description for Dimension #%d 1 through %d representing all account type and definitions, or the empty string if not appropriate.",x,endlength);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d DESC/CODE THRU %d",x,endlength);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(4*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=4;
		d->type=PLAINTEXT;
		d->length=80+longest_length;
		d->when=0;
		if(tmpdesc!=NULL)
		{
			d->expression=stralloc(tmpdesc);
			Rfree(tmpdesc);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Description+Code for Dimension #%d 1 through %d representing all accounts and definitions, or the empty string if not appropriate.",x,endlength);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"DIMENSION #%d CODE THRU %d",x,endlength);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINBACC");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=longest_length;
		d->when=0;
		if(tmplabel!=NULL)
		{
			d->expression=stralloc(tmplabel);
			Rfree(tmplabel);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code for Dimension #%d 1 through %d representing all accounts and definitions, or the empty string if not appropriate.",x,endlength);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);


	}
	}
	if(tmpexp!=NULL) Rfree(tmpexp);
	if(tmpdesc!=NULL) Rfree(tmpdesc);
	if(tmplabel!=NULL) Rfree(tmplabel);

/* END NEW DIMENSION #X CODE THRU Y virtual fields */

/* BEGIN NEW DIMENSION LAST virtual fields */
	x=(no_parts-1);
	exp_count=0;
	struct_count=0;
	label_count=0;
	label2_count=0;
	label3_count=0;
	if_count=0;
	longest_length=0;
	if(tmpexp!=NULL) Rfree(tmpexp);
	if(tmpstruct!=NULL) Rfree(tmpstruct);
	if(tmplabel!=NULL) Rfree(tmplabel);
	if(tmplabel2!=NULL) Rfree(tmplabel2);
	if(tmplabel3!=NULL) Rfree(tmplabel3);
	if(tmplabel4!=NULL) Rfree(tmplabel4);
/*
	if(tmplabel5!=NULL) Rfree(tmplabel5);
*/
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
			tmpexp=stralloc(temp);
			exp_count+=RDAstrlen(temp);

			tmpstruct=stralloc(temp);
			struct_count+=RDAstrlen(temp);

			tmplabel=stralloc(temp);
			label_count+=RDAstrlen(temp);

			tmplabel2=stralloc(temp);
			label2_count+=RDAstrlen(temp);

			tmplabel3=stralloc(temp);
			label3_count+=RDAstrlen(temp);

			tmplabel4=stralloc(temp);
			label4_count+=RDAstrlen(temp);

/*
			tmplabel5=stralloc(temp);
			label5_count+=RDAstrlen(temp);
*/


			if_count=0;
			prev_defused=FALSE;
			if(EXPENDITURE_ACCOUNT->num>0)
			{
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
						part=acc->acct+((acc->num-1));
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}


						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[DIMENSION #%d DESCRIPTION]\n",y,(acc->num-1));
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,(acc->num-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if(part->length>longest_length) longest_length=part->length;
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,acc_count);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel2[label2_count],"\tELSE %s",temp);
							label2_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel2[label2_count],"\t%s",temp);
							label2_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,part->length);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel3[label3_count],"\tELSE %s",temp);
							label3_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel3[label3_count],"\t%s",temp);
							label3_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel4[label4_count],"\tELSE %s",temp);
							label4_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel4[label4_count],"\t%s",temp);
							label4_count+=RDAstrlen(temp)+1;
						}

/*
						memset(temp,0,15001);
						sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=[ACCOUNT CODE][1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel5=Rrealloc(tmplabel5,
								label5_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel5[label5_count],"\tELSE %s",temp);
							label5_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel5=Rrealloc(tmplabel5,
								label5_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel5[label5_count],"\t%s",temp);
							label5_count+=RDAstrlen(temp)+1;
						}
*/

						if((acc_count-1)>accminus1_longest) accminus1_longest=(acc_count-1);

						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
					if(y==(EXPENDITURE_ACCOUNT->num-1))
					{
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=0");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpstruct!=NULL)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel!=NULL)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel2!=NULL)
						{
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel2[label2_count],"\t%s",temp);
							label2_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel3!=NULL)
						{
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel3[label3_count],"\t%s",temp);
							label3_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel4!=NULL)
						{
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel4[label4_count],"\t%s",temp);
							label4_count+=RDAstrlen(temp)+1;
						}
		
/*
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel5!=NULL)
						{
							tmplabel5=Rrealloc(tmplabel5,
								label5_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel5[label5_count],"\t%s",temp);
							label5_count+=RDAstrlen(temp)+1;
						}
*/
					}
				}
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			if_count=0;
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}

			if(tmpstruct!=NULL)
			{
				tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
				sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
				struct_count+=RDAstrlen(temp)+5;
			} else {
				tmpstruct=stralloc(temp);
				struct_count+=RDAstrlen(temp);
			}

			if(tmplabel!=NULL)
			{
				tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel[label_count],"ELSE %s",temp);
				label_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel=stralloc(temp);
				label_count+=RDAstrlen(temp);
			}

			if(tmplabel2!=NULL)
			{
				tmplabel2=Rrealloc(tmplabel2,label2_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel2[label2_count],"ELSE %s",temp);
				label2_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel2=stralloc(temp);
				label2_count+=RDAstrlen(temp);
			}

			if(tmplabel3!=NULL)
			{
				tmplabel3=Rrealloc(tmplabel3,label3_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel3[label3_count],"ELSE %s",temp);
				label3_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel3=stralloc(temp);
				label3_count+=RDAstrlen(temp);
			}

			if(tmplabel4!=NULL)
			{
				tmplabel4=Rrealloc(tmplabel4,label4_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel4[label4_count],"ELSE %s",temp);
				label4_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel4=stralloc(temp);
				label4_count+=RDAstrlen(temp);
			}

/*
			if(tmplabel5!=NULL)
			{
				tmplabel5=Rrealloc(tmplabel5,label5_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel5[label5_count],"ELSE %s",temp);
				label5_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel5=stralloc(temp);
				label5_count+=RDAstrlen(temp);
			}
*/

			prev_defused=FALSE;
			if(REVENUE_ACCOUNT->num>0)
			{
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
						part=acc->acct+((acc->num-1));
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[DIMENSION #%d DESCRIPTION]\n",y,(acc->num-1));
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,(acc->num-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if(part->length>longest_length) longest_length=part->length;
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,acc_count);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel2[label2_count],"\tELSE %s",temp);
							label2_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel2[label2_count],"\t%s",temp);
							label2_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,part->length);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel3[label3_count],"\tELSE %s",temp);
							label3_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel3[label3_count],"\t%s",temp);
							label3_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel4[label4_count],"\tELSE %s",temp);
							label4_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel4[label4_count],"\t%s",temp);
							label4_count+=RDAstrlen(temp)+1;
						}

/*
						memset(temp,0,15001);
						sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN RETURN_VALUE=[ACCOUNT CODE][1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel5=Rrealloc(tmplabel5,
								label5_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel5[label5_count],"\tELSE %s",temp);
							label5_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel5=Rrealloc(tmplabel5,
								label5_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel5[label5_count],"\t%s",temp);
							label5_count+=RDAstrlen(temp)+1;
						}
*/

						if((acc_count-1)>accminus1_longest) accminus1_longest=(acc_count-1);

						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
					if(y==(REVENUE_ACCOUNT->num-1))
					{
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
		
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=0");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpstruct!=NULL)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
		
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel!=NULL)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=0");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel2!=NULL)
						{
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel2[label2_count],"\t%s",temp);
							label2_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=0");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel3!=NULL)
						{
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel3[label3_count],"\t%s",temp);
							label3_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel4!=NULL)
						{
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel4[label4_count],"\t%s",temp);
							label4_count+=RDAstrlen(temp)+1;
						}

/*
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel5!=NULL)
						{
							tmplabel5=Rrealloc(tmplabel5,
								label5_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel5[label5_count],"\t%s",temp);
							label5_count+=RDAstrlen(temp)+1;
						}
*/
					}
				}
			}
		}
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			if_count=0;
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=2) THEN\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}

			if(tmpstruct!=NULL)
			{
				tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
				sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
				struct_count+=RDAstrlen(temp)+5;
			} else {
				tmpstruct=stralloc(temp);
				struct_count+=RDAstrlen(temp);
			}

			if(tmplabel!=NULL)
			{
				tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel[label_count],"ELSE %s",temp);
				label_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel=stralloc(temp);
				label_count+=RDAstrlen(temp);
			}

			if(tmplabel2!=NULL)
			{
				tmplabel2=Rrealloc(tmplabel2,label2_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel2[label2_count],"ELSE %s",temp);
				label2_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel2=stralloc(temp);
				label2_count+=RDAstrlen(temp);
			}

			if(tmplabel3!=NULL)
			{
				tmplabel3=Rrealloc(tmplabel3,label3_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel3[label3_count],"ELSE %s",temp);
				label3_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel3=stralloc(temp);
				label3_count+=RDAstrlen(temp);
			}

			if(tmplabel4!=NULL)
			{
				tmplabel4=Rrealloc(tmplabel4,label4_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel4[label4_count],"ELSE %s",temp);
				label4_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel4=stralloc(temp);
				label4_count+=RDAstrlen(temp);
			}

			prev_defused=FALSE;
			if(BALANCE_SHEET_ACCOUNT->num>0)
			{
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
						part=acc->acct+((acc->num-1));
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[DIMENSION #%d DESCRIPTION]\n",y,(acc->num-1));
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,(acc->num-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}


						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if(part->length>longest_length) longest_length=part->length;
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,acc_count);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel2[label2_count],"\tELSE %s",temp);
							label2_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel2[label2_count],"\t%s",temp);
							label2_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=%d\n",y,part->length);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel3[label3_count],"\tELSE %s",temp);
							label3_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel3[label3_count],"\t%s",temp);
							label3_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([FINMGT][FINBACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINBACC][ACCOUNT CODE][1,1,%d]\n",y,(acc_count-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel4[label4_count],"\tELSE %s",temp);
							label4_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel4[label4_count],"\t%s",temp);
							label4_count+=RDAstrlen(temp)+1;
						}

						if((acc_count-1)>accminus1_longest) accminus1_longest=(acc_count-1);




						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
					if(y==(BALANCE_SHEET_ACCOUNT->num-1))
					{
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=0");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmpstruct!=NULL)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel!=NULL)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=0");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel2!=NULL)
						{
							tmplabel2=Rrealloc(tmplabel2,
								label2_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel2[label2_count],"\t%s",temp);
							label2_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=0");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel3!=NULL)
						{
							tmplabel3=Rrealloc(tmplabel3,
								label3_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel3[label3_count],"\t%s",temp);
							label3_count+=RDAstrlen(temp)+1;
						}
		
						memset(temp,0,15001);
						sprintf(temp,"ELSE RETURN_VALUE=\"\"");
						for(q=0;q<(if_count);++q)
						{
							strcat(temp," FI");
						}
						strcat(temp,"\n");
						if(tmplabel4!=NULL)
						{
							tmplabel4=Rrealloc(tmplabel4,
								label4_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel4[label4_count],"\t%s",temp);
							label4_count+=RDAstrlen(temp)+1;
						}
		
					}
				}
			}
		}
	}
	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=\"\"");
	tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
	sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);

	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=0");
	tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+16);
	sprintf(&tmpstruct[struct_count],"ELSE %s FI FI FI\n",temp);

	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=\"\"");
	tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
	sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);

	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=0");
	tmplabel2=Rrealloc(tmplabel2,label2_count+RDAstrlen(temp)+16);
	sprintf(&tmplabel2[label2_count],"ELSE %s FI FI FI\n",temp);

	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=0");
	tmplabel3=Rrealloc(tmplabel3,label3_count+RDAstrlen(temp)+16);
	sprintf(&tmplabel3[label3_count],"ELSE %s FI FI FI\n",temp);

	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=\"\"");
	tmplabel4=Rrealloc(tmplabel4,label4_count+RDAstrlen(temp)+16);
	sprintf(&tmplabel4[label4_count],"ELSE %s FI FI FI\n",temp);



	memset(temp,0,15001);
	memset(stemp,0,101);
	sprintf(stemp,"DIMENSION LAST DESCRIPTION");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(4*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINCOA");
	i->keyname=NULL;
	d->vtype=1;
	d->num=4;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmpexp!=NULL)
	{
		d->expression=stralloc(tmpexp);
		Rfree(tmpexp);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Description of the last dimension part from the Account Structure representing all account types and definitions, or the empty string if not appropriate.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);


	memset(stemp,0,101);
	sprintf(stemp,"DIMENSION LAST");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=SHORTV;
	d->length=2;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmpstruct!=NULL)
	{
		d->expression=stralloc(tmpstruct);
		Rfree(tmpstruct);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Represents Dimension number of the last dimension from the Account Structure representing all account types and definitions.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);


	memset(stemp,0,101);
	sprintf(stemp,"DIMENSION LAST CODE");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=PLAINTEXT;
	d->length=longest_length;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmplabel!=NULL)
	{
		d->expression=stralloc(tmplabel);
		Rfree(tmplabel);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Represents Dimension Code of the last dimension from the Account Structure representing all account types and definitions.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	memset(stemp,0,101);
	sprintf(stemp,"DIMENSION LAST CODE OFFSET");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=SHORTV;
	d->length=3;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmplabel2!=NULL)
	{
		d->expression=stralloc(tmplabel2);
		Rfree(tmplabel2);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Represents Dimension Code Offset of the last dimension from the Account Structure representing all account types and definitions.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	memset(stemp,0,101);
	sprintf(stemp,"DIMENSION LAST CODE LENGTH");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=SHORTV;
	d->length=3;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmplabel3!=NULL)
	{
		d->expression=stralloc(tmplabel3);
		Rfree(tmplabel3);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Represents Dimension Code Length of the last dimension from the Account Structure representing all account types and definitions.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	memset(stemp,0,101);
	sprintf(stemp,"ACCOUNT CODE MINUS 1 PART");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINBACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=PLAINTEXT;
	if(accminus1_longest==0) d->length=40;
		else d->length=accminus1_longest;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmplabel4!=NULL)
	{
		d->expression=stralloc(tmplabel4);
		Rfree(tmplabel4);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Represents Account Code minus last account part from the Account Structure representing all account types and definitions.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);




	if(tmpexp!=NULL) Rfree(tmpexp);
	if(tmpstruct!=NULL) Rfree(tmpstruct);
	if(tmplabel!=NULL) Rfree(tmplabel);
	if(tmplabel2!=NULL) Rfree(tmplabel2);
	if(tmplabel3!=NULL) Rfree(tmplabel3);
	if(tmplabel4!=NULL) Rfree(tmplabel4);
/*
	if(tmplabel5!=NULL) Rfree(tmplabel5);
*/
/* NEW DIMENSION LABEL virtual field */
	exp_count=0;
	acc_count=1;
	if_count=0;
	if(tmpexp!=NULL) Rfree(tmpexp);
	longest_length=0;
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
			tmpexp=stralloc(temp);
			exp_count+=RDAstrlen(temp);
			if_count=0;
			if_count_2=0;
			memset(temp,0,15001);
			prev_defused=FALSE;
			prev_defused_2=FALSE;
			for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
				y<EXPENDITURE_ACCOUNT->num;++y,++acc)
			{
				if(acc->acct!=NULL)
				{
					part=acc->acct+x;
					if(part->length>longest_length) longest_length=part->length;
					memset(temp,0,15001);
					sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN\n",y);
					if_count+=1;
					if(y>0 && prev_defused==TRUE)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+9);
						sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+8;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+4);
						sprintf(&tmpexp[exp_count],"   %s",temp);
						exp_count+=RDAstrlen(temp)+3;
						prev_defused=TRUE;
					}
					prev_defused_2=FALSE;
					if_count_2=0;
					for(z=0,p=acc->acct;z<acc->num;++z,++p) 
					{
						if(RDAstrlen(p->name)>p->length)
						{
							if(acc_count<RDAstrlen(p->name)) acc_count=RDAstrlen(p->name); 
						} else {
							if(acc_count<p->length) acc_count=p->length; 
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([DIMENSION]=%d) THEN RETURN_VALUE=\"%s\"\n",z,p->name);
						++if_count_2;
						if(z>0 && prev_defused_2==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+12);
							sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
							exp_count+=RDAstrlen(temp)+11;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"      %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						}
						prev_defused_2=TRUE;
					}
					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count_2);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpexp!=NULL)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+12);
						sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+11;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"      %s",temp);
						exp_count+=RDAstrlen(temp)+6;
					}
				} else {
					prev_defused=FALSE;
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			strcat(temp,"\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+9);
				sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+8;
			} else {
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+4);
				sprintf(&tmpexp[exp_count],"   %s",temp);
				exp_count+=RDAstrlen(temp)+3;
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			memset(temp,0,15001);
			if_count=0;
			if_count_2=0;
			prev_defused=FALSE;
			prev_defused_2=FALSE;
			for(y=0,acc=REVENUE_ACCOUNT->codetype;
				y<REVENUE_ACCOUNT->num;++y,++acc)
			{
				if(acc->acct!=NULL)
				{
					part=acc->acct+x;
					if(part->length>longest_length) longest_length=part->length;
					memset(temp,0,15001);
					memset(temp,0,15001);
					sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN\n",y);
					if_count+=1;
					if(y>0 && prev_defused==TRUE)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+9);
						sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+8;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+4);
						sprintf(&tmpexp[exp_count],"   %s",temp);
						exp_count+=RDAstrlen(temp)+3;
						prev_defused=TRUE;
					}
					prev_defused_2=FALSE;
					if_count_2=0;
					for(z=0,p=acc->acct;z<acc->num;++z,++p) 
					{
						if(RDAstrlen(p->name)>p->length)
						{
							if(acc_count<RDAstrlen(p->name)) acc_count=RDAstrlen(p->name); 
						} else {
							if(acc_count<p->length) acc_count=p->length; 
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([DIMENSION]=%d) THEN RETURN_VALUE=\"%s\"\n",z,p->name);
						++if_count_2;
						if(z>0 && prev_defused_2==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+12);
							sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
							exp_count+=RDAstrlen(temp)+11;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"      %s",temp);
							exp_count+=RDAstrlen(temp)+6;
							prev_defused_2=TRUE;
						}
					}
					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count_2);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpexp!=NULL)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+12);
						sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+11;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"      %s",temp);
						exp_count+=RDAstrlen(temp)+6;
						prev_defused=FALSE;
					}
				} else {
					prev_defused=FALSE;
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			strcat(temp,"\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+9);
				sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+8;
			} else {
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+4);
				sprintf(&tmpexp[exp_count],"   %s",temp);
				exp_count+=RDAstrlen(temp)+3;
			}
		}
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=2) THEN\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if_count=0;
			if_count_2=0;
			memset(temp,0,15001);
			prev_defused=FALSE;
			prev_defused_2=FALSE;
			for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
				y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
			{
				if(acc->acct!=NULL)
				{
					part=acc->acct+x;
					if(part->length>longest_length) longest_length=part->length;
					memset(temp,0,15001);
					memset(temp,0,15001);
					sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN\n",y);
					if_count+=1;
					if(y>0 && prev_defused==TRUE)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+9);
						sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+8;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+4);
						sprintf(&tmpexp[exp_count],"   %s",temp);
						exp_count+=RDAstrlen(temp)+3;
					}
					prev_defused=TRUE;
					prev_defused_2=FALSE;
					for(z=0,p=acc->acct;z<acc->num;++z,++p) 
					{
						if(RDAstrlen(p->name)>p->length)
						{
							if(acc_count<RDAstrlen(p->name)) acc_count=RDAstrlen(p->name); 
						} else {
							if(acc_count<p->length) acc_count=p->length; 
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([DIMENSION]=%d) THEN RETURN_VALUE=\"%s\"\n",z,p->name);
						if_count_2+=1;
						if(z>0 && prev_defused_2==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+12);
							sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
							exp_count+=RDAstrlen(temp)+11;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"      %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						}
						prev_defused_2=TRUE;
					}
					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count_2);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpexp!=NULL)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+12);
						sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+11;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"      %s",temp);
						exp_count+=RDAstrlen(temp)+6;
					}
				} else {
					prev_defused=FALSE;
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			strcat(temp,"\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+9);
				sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+8;
			} else {
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+4);
				sprintf(&tmpexp[exp_count],"   %s",temp);
				exp_count+=RDAstrlen(temp)+3;
			}
		}
	}
	if(BEGINNING_BALANCE_ACCOUNT!=NULL)
	{
		if(BEGINNING_BALANCE_ACCOUNT->codetype!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=3) THEN\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if_count=0;
			if_count_2=0;
			memset(temp,0,15001);
			prev_defused=FALSE;
			prev_defused_2=FALSE;
			for(y=0,acc=BEGINNING_BALANCE_ACCOUNT->codetype;
				y<BEGINNING_BALANCE_ACCOUNT->num;++y,++acc)
			{
				if(acc->acct!=NULL)
				{
					memset(temp,0,15001);
					sprintf(temp,"IF([DEFINITION TYPE]=%d) THEN\n",y);
					if_count+=1;
					if(y>0 && prev_defused==TRUE)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+9);
						sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+8;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+4);
						sprintf(&tmpexp[exp_count],"   %s",temp);
						exp_count+=RDAstrlen(temp)+3;
					}
					prev_defused=TRUE;
					prev_defused_2=FALSE;
					if_count_2=0;
					for(z=0,p=acc->acct;z<acc->num;++z,++p) 
					{
						if(RDAstrlen(p->name)>p->length)
						{
							if(acc_count<RDAstrlen(p->name)) acc_count=RDAstrlen(p->name); 
						} else {
							if(acc_count<p->length) acc_count=p->length; 
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([DIMENSION]=%d) THEN RETURN_VALUE=\"%s\"\n",z,p->name);
						++if_count_2;
						if(z>0 && prev_defused_2==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+12);
							sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
							exp_count+=RDAstrlen(temp)+11;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"      %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						}
						prev_defused_2=TRUE;
					}
					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count_2);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpexp!=NULL)
					{
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+12);
						sprintf(&tmpexp[exp_count],"      ELSE %s",temp);
						exp_count+=RDAstrlen(temp)+11;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"      %s",temp);
						exp_count+=RDAstrlen(temp)+6;
					}
				} else {
					prev_defused=FALSE;
				}
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			strcat(temp,"\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+9);
				sprintf(&tmpexp[exp_count],"   ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+8;
			} else {
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+4);
				sprintf(&tmpexp[exp_count],"   %s",temp);
				exp_count+=RDAstrlen(temp)+3;
			}
		}
	}
	if(tmpexp!=NULL)
	{
		memset(temp,0,15001);
		sprintf(temp,"RETURN_VALUE=\"\"");
		tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+18);
		sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI FI",temp);
	}

	memset(temp,0,15001);
	memset(stemp,0,101);
	sprintf(stemp,"DIMENSION LABEL");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(1*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINCOA");
	i->keyname=NULL;
	d->vtype=1;
	d->num=1;
	d->type=PLAINTEXT;
	d->length=acc_count;
	d->when=0;
	d->avail2rsrc=FALSE;
	if(tmpexp!=NULL)
	{
		d->expression=stralloc(tmpexp);
		Rfree(tmpexp);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Dimension Label for all account types and definition types.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	if(tmpexp!=NULL) Rfree(tmpexp);

/* END NEW DIMENSION LABEL */
}
void xCreateBUDPREPCOAs(char *modx,int line,char *file)
{
	short q=0,z,x,y;
	short if_count=0,exp_count=0,desc_count=0,acc_count=0,no_parts=0;
	short struct_count=0,label_count=0,partlabel_count=0;
	short longest_length=0,exp_no_parts=0,rev_no_parts=0;
/*
	short bal_no_parts=0;
	int field_length=0;
*/
	RDAaccpart *part=NULL,*p=NULL;
	RDAacct *acc=NULL;
	DFvirtual *d=NULL;
	DFincvir *i=NULL;
	char temp[15001],*tmpexp=NULL,*tmpdesc=NULL;
	char *exp_label=NULL,*tmpstruct=NULL,*tmplabel=NULL,*tmppartlabel=NULL;
	char prev_defused=FALSE;
	char *vname=NULL;
	short endlength=0;
	char *pad_string=NULL;

	d=DFvirtualNEW(modx,"BUDPREP ACCOUNT TYPE");
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDRACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=2;
	d->type=SHORTV;
	d->length=1;
	d->when=0;
	d->avail2rsrc=TRUE;
	memset(temp,0,15000);
	sprintf(temp,"IF([BUDPREP][BUDEACC][ACCOUNT CODE]) THEN RETURN_VALUE=0 ELSE IF([BUDPREP][BUDRACC][ACCOUNT CODE]) THEN RETURN_VALUE=1 ELSE RETURN_VALUE=2 FI FI");
	d->expression=stralloc(temp);
	memset(temp,0,15000);
	sprintf(temp,"This virtual field represents the Budget Prep. Account Type.");
	d->desc=stralloc(temp);
	memset(temp,0,15000);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);
	CreateFUNDvirtual(modx,"ACCOUNT FUND",0,"[ACCOUNT CODE]","[DEFINITION TYPE]");
	CreateFUNDSUFFIXvirtual(modx,"ACCOUNT FUND SUFFIX",0,"[ACCOUNT CODE]","[DEFINITION TYPE]");
#ifndef PARTIALLIST
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			if(XPERT_SETUP->software_type!=2)
			{
			for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
				y<EXPENDITURE_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+40);
						sprintf(vname,"BUDGET EXPENDITURE CODE %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("BUDPREP");
						i->file=stralloc("BUDEACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+44);
						sprintf(d->desc,"%s Part of the %s Expenditure Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+40);
						sprintf(vname,"BUDGET EXPENDITURE DESCRIPTION %s %s",acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("BUDPREP");
						i->file=stralloc("BUDEACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+65);
						sprintf(d->desc,"Description For %s Part of the %s Expenditure Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(45);
			sprintf(vname,"BUDGET EXPENDITURE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("BUDPREP");
			i->file=stralloc("BUDEACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			if(tmpexp!=NULL) Rfree(tmpexp);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			vname=Rmalloc(25);
			sprintf(vname,"BUDGET EXPENDITURE LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("BUDPREP");
			i->file=stralloc("BUDEACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Expenditure Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>=no_parts)
					{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=0;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(p->length>=RDAstrlen(p->name))
							{
								acc_count+=p->length+1;
							} else {
								acc_count+=RDAstrlen(p->name)+1;
							}
						}
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count+1,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
							desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(45);
				sprintf(vname,"BUDGET EXPENDITURE CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("BUDPREP");
				i->file=stralloc("BUDEACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				d->avail2rsrc=TRUE;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Expenditure Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(55);
				sprintf(vname,"BUDPREP EXPENDITURE DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("BUDPREP");
				i->file=stralloc("BUDEACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Expenditure Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			if(XPERT_SETUP->software_type!=2)
			{
			for(y=0,acc=REVENUE_ACCOUNT->codetype;
				y<REVENUE_ACCOUNT->num;++y,++acc)
			{
				acc_count=0;
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+27);
						sprintf(vname,"BUDPREP REVENUE CODE %s %s",acc->name,
							part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("BUDPREP");
						i->file=stralloc("BUDRACC");
						d->vtype=0;
						d->num=1;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]",acc_count,RDAstrlen(part->definition));
						acc_count+=part->length+1;
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+44);
						sprintf(d->desc,"%s Part of the %s Revenue Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
						vname=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+35);
						sprintf(vname,"BUDPREP REVENUE DESCRIPTION %s %s",
							acc->name,part->name);
						d=DFvirtualNEW(modx,vname);
						if(vname!=NULL) Rfree(vname);
						d->incvir=Rmalloc(2*sizeof(DFincvir));
						i=d->incvir;
						i->module=stralloc("BUDPREP");
						i->file=stralloc("BUDRACC");
						++i;
						i->module=stralloc("FINMGT");
						i->file=stralloc("FINCOA");
						d->vtype=1;
						d->num=2;
						d->type=PLAINTEXT;
						d->length=part->length;
						d->when=0;
						memset(stemp,0,101);
						sprintf(stemp,"[FINMGT][FINCOA][DESCRIPTION][%d,1,40]",x+1);
						d->expression=stralloc(stemp);
						memset(stemp,0,101);
						d->desc=Rmalloc(RDAstrlen(part->name)+RDAstrlen(acc->name)+55);
						sprintf(d->desc,"Description For %s Part of the %s Revenue Account Codes",part->name,acc->name);
						writeDFvirtual(modx,d);
						if(d!=NULL) FreeDFvirtual(d);
					}
				}
			}
			}
			if(tmpexp!=NULL) Rfree(tmpexp);
			if(tmpdesc!=NULL) Rfree(tmpdesc);
			exp_count=0;
			desc_count=0;
			if_count=0;
			longest_length=0;
			no_parts=0;
			for(z=0,acc=REVENUE_ACCOUNT->codetype;
				z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_label=fullacctdef(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if_count+=1;
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+6);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
				exp_label=fullacctlabel(acc);
				memset(temp,0,15001);
				sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=\"%s\"",z,exp_label);
				if(RDAstrlen(exp_label)>longest_length) longest_length=RDAstrlen(exp_label);
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+6);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(exp_label!=NULL) Rfree(exp_label);
			}
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			for(q=0;q<(if_count);++q)
			{
				strcat(temp," FI");
			}
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,
					exp_count+RDAstrlen(temp)+7);
				sprintf(&tmpexp[exp_count],"\nELSE %s",temp);
				exp_count+=(RDAstrlen(temp)+6);
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}
			if(tmpdesc!=NULL)
			{
				tmpdesc=Rrealloc(tmpdesc,
					desc_count+RDAstrlen(temp)+7);
				sprintf(&tmpdesc[desc_count],"\nELSE %s",temp);
				desc_count+=(RDAstrlen(temp)+6);
			} else {
				tmpdesc=stralloc(temp);
				desc_count+=RDAstrlen(temp);
			}
			vname=Rmalloc(39);
			sprintf(vname,"BUDPREP REVENUE ACCOUNT STRUCTURE");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("BUDPREP");
			i->file=stralloc("BUDRACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpexp!=NULL)
			{
				d->expression=stralloc(tmpexp);
				Rfree(tmpexp);
			} else d->expression=NULL;
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Structure of the entire Revenue Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			vname=Rmalloc(15);
			sprintf(vname,"REVENUE LABEL");
			d=DFvirtualNEW(modx,vname);
			if(vname!=NULL) Rfree(vname);
			d->incvir=Rmalloc(sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINRACC");
			d->vtype=0;
			d->num=1;
			d->type=PLAINTEXT;
			d->length=longest_length;
			d->avail2rsrc=TRUE;
			d->when=0;
			if(tmpdesc!=NULL)
			{
				d->expression=stralloc(tmpdesc);
				Rfree(tmpdesc);
			} else d->expression=NULL;
			d->desc=Rmalloc(130);
			sprintf(d->desc,"This virtual field represents the Label of the entire Revenue Account Code related to the Definition Type");
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			for(x=0;x<no_parts;++x)
			{
				exp_count=0;
				desc_count=0;
				if_count=0;
				if(tmpexp!=NULL) Rfree(tmpexp);
				if(tmpdesc!=NULL) Rfree(tmpdesc);
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL)
					{
						part=acc->acct+x;
						memset(temp,0,15001);
						acc_count=0;
						for(z=0,p=acc->acct;z<x;++z,++p) 
						{
							if(p->length>=RDAstrlen(p->name))
							{
								acc_count+=p->length+1;
							} else {
								acc_count+=RDAstrlen(p->name)+1;
							}
						}
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d] ",y,acc_count+1,part->length);
						if_count+=1;
						if(tmpexp!=NULL)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+6);
							sprintf(&tmpexp[exp_count],"ELSE %s",temp);
							exp_count+=(RDAstrlen(temp)+5);
						} else {
							tmpexp=stralloc(temp);
							exp_count+=RDAstrlen(temp);
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,40] ",y,x+1);
						if(tmpdesc!=NULL)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+6);
							sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
							desc_count+=(RDAstrlen(temp)+5);
						} else {
							tmpdesc=stralloc(temp);
							desc_count+=RDAstrlen(temp);
						}
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=(RDAstrlen(temp)+5);
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=(RDAstrlen(temp)+5);
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				vname=Rmalloc(40);
				sprintf(vname,"BUDPREP REVENUE CODE DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINRACC");
				d->vtype=0;
				d->num=1;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				d->avail2rsrc=TRUE;
				if(tmpexp!=NULL)
				{
					d->expression=stralloc(tmpexp);
					Rfree(tmpexp);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Revenue Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
				vname=Rmalloc(51);
				sprintf(vname,"BUDPREP REVENUE DESCRIPTION DIMENSION #%d",x);
				d=DFvirtualNEW(modx,vname);
				if(vname!=NULL) Rfree(vname);
				d->incvir=Rmalloc(2*sizeof(DFincvir));
				i=d->incvir;
				i->module=stralloc("BUDPREP");
				i->file=stralloc("BUDRACC");
				++i;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				d->vtype=1;
				d->num=2;
				d->type=PLAINTEXT;
				d->length=part->length;
				d->when=0;
				if(tmpdesc!=NULL)
				{
					d->expression=stralloc(tmpdesc);
					Rfree(tmpdesc);
				} else d->expression=NULL;
				memset(temp,0,15001);
				sprintf(temp,"Description for Revenue Code Dimension %d representing all definitions, or the empty string if not appropriate.",x);
				d->desc=stralloc(temp);
				memset(temp,0,15001);
				writeDFvirtual(modx,d);
				if(d!=NULL) FreeDFvirtual(d);
			}
		}
	}
#endif /* End PARTIALLIST */
	no_parts=0;
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
				z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				exp_no_parts=acc->num;
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			for(z=0,acc=REVENUE_ACCOUNT->codetype;
				z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				if(acc->num>no_parts) no_parts=acc->num;
				rev_no_parts=acc->num;
			}
		}
	}
	for(x=0;x<no_parts;++x)
	{
		exp_count=0;
		desc_count=0;
		struct_count=0;
		label_count=0;
		partlabel_count=0;
		if_count=0;
		if(tmpexp!=NULL) Rfree(tmpexp);
		if(tmpdesc!=NULL) Rfree(tmpdesc); 
		if(tmpstruct!=NULL) Rfree(tmpstruct); 
		if(tmplabel!=NULL) Rfree(tmplabel); 
		if(tmppartlabel!=NULL) Rfree(tmppartlabel); 
		longest_length=0;
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
				tmpexp=stralloc(temp);
				tmpdesc=stralloc(temp);
				if(x==0)
				{
				tmplabel=stralloc(temp);
				tmpstruct=stralloc(temp);
				}
				tmppartlabel=stralloc(temp);
				exp_count+=RDAstrlen(temp);
				desc_count+=RDAstrlen(temp);
				if(x==0)
				{
				label_count+=RDAstrlen(temp);
				struct_count+=RDAstrlen(temp);
				}
				partlabel_count+=RDAstrlen(temp);
				if_count=0;
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						if(exp_label!=NULL) Rfree(exp_label);
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<exp_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<exp_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<exp_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<exp_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<exp_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				if_count=0;
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(x==0)
				{
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				if(tmpstruct!=NULL)
				{
					tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
					sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
					struct_count+=RDAstrlen(temp)+5;
				} else {
					tmpstruct=stralloc(temp);
					struct_count+=RDAstrlen(temp);
				}
				}
				if(tmppartlabel!=NULL)
				{
					tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+6);
					sprintf(&tmppartlabel[partlabel_count],"ELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+5;
				} else {
					tmppartlabel=stralloc(temp);
					partlabel_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,x+1);
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						if(x==0)
						{
						exp_label=fullacctlabel(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						exp_label=fullacctdef(acc);
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,exp_label);
						if(exp_label!=NULL) Rfree(exp_label);
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}
						}
						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=\"%s\"\n",y,p->name);
						if(y>0 && prev_defused==TRUE)
						{
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+7);
							sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
							partlabel_count+=RDAstrlen(temp)+6;
						} else {
							tmppartlabel=Rrealloc(tmppartlabel,
								partlabel_count+RDAstrlen(temp)+2);
							sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
							partlabel_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<rev_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<rev_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(x==0)
				{
				if(tmplabel!=NULL && x<rev_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
				if(tmpstruct!=NULL && x<rev_no_parts)
				{
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
					sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
					struct_count+=RDAstrlen(temp)+6;
				} else {
					tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+2);
					sprintf(&tmpstruct[struct_count],"\t%s",temp);
					struct_count+=RDAstrlen(temp)+1;
				}
				}
				if(tmppartlabel!=NULL && x<rev_no_parts)
				{
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+7);
					sprintf(&tmppartlabel[partlabel_count],"\tELSE %s",temp);
					partlabel_count+=RDAstrlen(temp)+6;
				} else {
					tmppartlabel=Rrealloc(tmppartlabel,
						partlabel_count+RDAstrlen(temp)+2);
					sprintf(&tmppartlabel[partlabel_count],"\t%s",temp);
					partlabel_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(tmpexp!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
			sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpdesc!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+16);
			sprintf(&tmpdesc[desc_count],"ELSE %s FI FI FI\n",temp);
		}
		if(x==0)
		{
		if(tmplabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
			sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpstruct!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+16);
			sprintf(&tmpstruct[struct_count],"ELSE %s FI FI FI\n",temp);
		}
		}
		if(tmppartlabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmppartlabel=Rrealloc(tmppartlabel,partlabel_count+RDAstrlen(temp)+16);
			sprintf(&tmppartlabel[partlabel_count],"ELSE %s FI FI FI\n",temp);
		}
		memset(temp,0,15001);
		memset(stemp,0,101);
		sprintf(stemp,"BUDPREP DIMENSION #%d CODE",x);
		d=DFvirtualNEW(modx,stemp);
		d->incvir=Rmalloc(2*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDRACC");
		i->keyname=NULL;
		d->vtype=1;
		d->num=2;
		d->type=PLAINTEXT;
		d->length=longest_length;
		d->when=0;
		d->avail2rsrc=TRUE;
		if(tmpexp!=NULL)
		{
			d->expression=stralloc(tmpexp);
			Rfree(tmpexp);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code Dimension %d representing all account type and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"BUDPREP DIMENSION #%d DESCRIPTION",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=80;
		d->when=0;
		if(tmpdesc!=NULL)
		{
			d->expression=stralloc(tmpdesc);
			Rfree(tmpdesc);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Description for Code Dimension %d representing all accounts and definitions, or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"BUDPREP DIMENSION #%d",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=SHORTV;
		d->length=2;
		d->when=0;
		sprintf(stemp,"%d",x);
		d->expression=stralloc(stemp);
		memset(stemp,0,101);
		memset(temp,0,15001);
		sprintf(temp,"Represents Dimension number %d for use in the Chart of Accounts.",x);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
		memset(stemp,0,101);
		if(x==0)
		{
			memset(stemp,0,101);
			sprintf(stemp,"BUDPREP ACCOUNT CODE LABEL");
			d=DFvirtualNEW(modx,stemp);
			memset(stemp,0,101);
			d->incvir=Rmalloc(3*sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("BUDPREP");
			i->file=stralloc("BUDEACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("BUDPREP");
			i->file=stralloc("BUDRACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINCOA");
			i->keyname=NULL;
			d->vtype=1;
			d->num=3;
			d->type=PLAINTEXT;
			d->length=40;
			d->when=0;
			d->avail2rsrc=TRUE;
			if(tmplabel!=NULL)
			{
				d->expression=stralloc(tmplabel);
				Rfree(tmplabel);
			} else d->expression=NULL;
			memset(temp,0,15001);
			sprintf(temp,"Account Label for each Account Type.");
			d->desc=stralloc(temp);
			memset(temp,0,15001);
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
			memset(stemp,0,101);
			sprintf(stemp,"BUDPREP ACCOUNT CODE STRUCTURE");
			d=DFvirtualNEW(modx,stemp);
			memset(stemp,0,101);
			d->incvir=Rmalloc(3*sizeof(DFincvir));
			i=d->incvir;
			i->module=stralloc("BUDPREP");
			i->file=stralloc("BUDEACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("BUDPREP");
			i->file=stralloc("BUDRACC");
			i->keyname=NULL;
			++i;
			i->module=stralloc("FINMGT");
			i->file=stralloc("FINCOA");
			i->keyname=NULL;
			d->vtype=1;
			d->num=3;
			d->type=PLAINTEXT;
			d->length=40;
			d->when=0;
			d->avail2rsrc=FALSE;
			if(tmpstruct!=NULL)
			{
				d->expression=stralloc(tmpstruct);
				Rfree(tmpstruct);
			} else d->expression=NULL;
			memset(temp,0,15001);
			sprintf(temp,"Account Code Structure for each Account Type.");
			d->desc=stralloc(temp);
			memset(temp,0,15001);
			writeDFvirtual(modx,d);
			if(d!=NULL) FreeDFvirtual(d);
		}
		memset(stemp,0,101);
		sprintf(stemp,"BUDPREP DIMENSION #%d LABEL",x);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=40;
		d->when=0;
		if(tmppartlabel!=NULL)
		{
			d->expression=stralloc(tmppartlabel);
			Rfree(tmppartlabel);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Account Part Name for Dimension %d or the empty string if not appropriate.",x);
		d->desc=stralloc(temp);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);
	}

/* NEW DIMENSION #X CODE THRU Y virtual fields */
	for(endlength=0;endlength<10;++endlength)
	{
	for(x=0;x<no_parts;++x)
	{
		exp_count=0;
		desc_count=0;
		label_count=0;
		partlabel_count=0;
		if_count=0;
		if(tmpexp!=NULL) Rfree(tmpexp);
		if(tmpdesc!=NULL) Rfree(tmpdesc); 
		if(tmplabel!=NULL) Rfree(tmplabel); 
		longest_length=0;
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
				tmpexp=stralloc(temp);
				tmpdesc=stralloc(temp);
				tmplabel=stralloc(temp);
				exp_count+=RDAstrlen(temp);
				desc_count+=RDAstrlen(temp);
				label_count+=RDAstrlen(temp);
				if_count=0;
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,part->length,x+1);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]+\"%s\"+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""),(x+1));
							} else {
								sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(x+1));
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,(x+1),acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<exp_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<exp_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(tmplabel!=NULL && x<exp_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				memset(temp,0,15001);
				sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
				if(tmpexp!=NULL)
				{
					tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
					sprintf(&tmpexp[exp_count],"ELSE %s",temp);
					exp_count+=RDAstrlen(temp)+5;
				} else {
					tmpexp=stralloc(temp);
					exp_count+=RDAstrlen(temp);
				}
				if(tmpdesc!=NULL)
				{
					tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+6);
					sprintf(&tmpdesc[desc_count],"ELSE %s",temp);
					desc_count+=RDAstrlen(temp)+5;
				} else {
					tmpdesc=stralloc(temp);
					desc_count+=RDAstrlen(temp);
				}
				if(tmplabel!=NULL)
				{
					tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
					sprintf(&tmplabel[label_count],"ELSE %s",temp);
					label_count+=RDAstrlen(temp)+5;
				} else {
					tmplabel=stralloc(temp);
					label_count+=RDAstrlen(temp);
				}
				memset(temp,0,15001);
				prev_defused=FALSE;
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>x)
					{
						part=acc->acct+x;
						if(part->length>longest_length) longest_length=part->length;
						memset(temp,0,15001);
						acc_count=1;
						for(z=0,p=acc->acct;z<x;
							++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,part->length,x+1);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]+\"%s\"+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""),(x+1));
							} else {
								sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]+\" \"+[FINMGT][FINCOA][DESCRIPTION][%d,1,80]\n",y,acc_count,endlength,(x+1));
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,(x+1),acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][%d,1,80]+\" \"+[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]\n",y,(x+1),acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+7);
							sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
							desc_count+=RDAstrlen(temp)+6;
						} else {
							tmpdesc=Rrealloc(tmpdesc,
								desc_count+RDAstrlen(temp)+2);
							sprintf(&tmpdesc[desc_count],"\t%s",temp);
							desc_count+=RDAstrlen(temp)+1;
						}
						memset(temp,0,15001);
						if(endlength>=part->length || endlength==0)
						{
							sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						} else {
							pad_string=padstr((part->length-endlength),'0');
							if(!isEMPTY(pad_string))
							{
								sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]+\"%s\"\n",y,acc_count,endlength,(pad_string!=NULL?pad_string:""));
							} else {
								sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,endlength);
							}
							if(pad_string!=NULL) Rfree(pad_string);
						}
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}
						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}
				}
				memset(temp,0,15001);
				sprintf(temp,"RETURN_VALUE=\"\"");
				for(q=0;q<(if_count);++q)
				{
					strcat(temp," FI");
				}
				strcat(temp,"\n");
				if(tmpexp!=NULL && x<rev_no_parts)
				{
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
					sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
					exp_count+=RDAstrlen(temp)+6;
				} else {
					tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+2);
					sprintf(&tmpexp[exp_count],"\t%s",temp);
					exp_count+=RDAstrlen(temp)+1;
				}
				if(tmpdesc!=NULL && x<rev_no_parts)
				{
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+7);
					sprintf(&tmpdesc[desc_count],"\tELSE %s",temp);
					desc_count+=RDAstrlen(temp)+6;
				} else {
					tmpdesc=Rrealloc(tmpdesc,
						desc_count+RDAstrlen(temp)+2);
					sprintf(&tmpdesc[desc_count],"\t%s",temp);
					desc_count+=RDAstrlen(temp)+1;
				}
				if(tmplabel!=NULL && x<rev_no_parts)
				{
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
					sprintf(&tmplabel[label_count],"\tELSE %s",temp);
					label_count+=RDAstrlen(temp)+6;
				} else {
					tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+2);
					sprintf(&tmplabel[label_count],"\t%s",temp);
					label_count+=RDAstrlen(temp)+1;
				}
			}
		}
		if(tmpexp!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
			sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmpdesc!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmpdesc=Rrealloc(tmpdesc,desc_count+RDAstrlen(temp)+16);
			sprintf(&tmpdesc[desc_count],"ELSE %s FI FI FI\n",temp);
		}
		if(tmplabel!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"RETURN_VALUE=\"\"");
			tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
			sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);
		}

		memset(temp,0,15001);
		memset(stemp,0,101);
		sprintf(stemp,"BUDGET DIMENSION #%d CODE/DESC THRU %d",x,endlength);
		d=DFvirtualNEW(modx,stemp);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=80+longest_length;
		d->when=0;
		d->avail2rsrc=FALSE;
		if(tmpexp!=NULL)
		{
			d->expression=stralloc(tmpexp);
			Rfree(tmpexp);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code+Description for Dimension #%d 1 through %d representing all account type and definitions, or the empty string if not appropriate.",x,endlength);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"BUDPREP DIMENSION #%d DESC/CODE THRU %d",x,endlength);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(3*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDRACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("FINMGT");
		i->file=stralloc("FINCOA");
		i->keyname=NULL;
		d->vtype=1;
		d->num=3;
		d->type=PLAINTEXT;
		d->length=80+longest_length;
		d->when=0;
		if(tmpdesc!=NULL)
		{
			d->expression=stralloc(tmpdesc);
			Rfree(tmpdesc);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Description+Code for Dimension #%d 1 through %d representing all accounts and definitions, or the empty string if not appropriate.",x,endlength);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);

		memset(stemp,0,101);
		sprintf(stemp,"BUDPREP DIMENSION #%d CODE THRU %d",x,endlength);
		d=DFvirtualNEW(modx,stemp);
		memset(stemp,0,101);
		d->incvir=Rmalloc(2*sizeof(DFincvir));
		i=d->incvir;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDEACC");
		i->keyname=NULL;
		++i;
		i->module=stralloc("BUDPREP");
		i->file=stralloc("BUDRACC");
		i->keyname=NULL;
		d->vtype=1;
		d->num=2;
		d->type=PLAINTEXT;
		d->length=longest_length;
		d->when=0;
		if(tmplabel!=NULL)
		{
			d->expression=stralloc(tmplabel);
			Rfree(tmplabel);
		} else d->expression=NULL;
		memset(temp,0,15001);
		sprintf(temp,"Code for Dimension #%d 1 through %d representing all accounts and definitions, or the empty string if not appropriate.",x,endlength);
		d->desc=stralloc(temp);
		memset(temp,0,15001);
		writeDFvirtual(modx,d);
		if(d!=NULL) FreeDFvirtual(d);


	}
	}
	if(tmpexp!=NULL) Rfree(tmpexp);
	if(tmpdesc!=NULL) Rfree(tmpdesc);
	if(tmplabel!=NULL) Rfree(tmplabel);

/* END NEW DIMENSION #X CODE THRU Y virtual fields */

	x=(no_parts-1);
	exp_count=0;
	struct_count=0;
	label_count=0;
	if_count=0;
	longest_length=0;
	if(tmpexp!=NULL) Rfree(tmpexp);
	if(tmpstruct!=NULL) Rfree(tmpstruct);
	if(tmplabel!=NULL) Rfree(tmplabel);
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=0) THEN\n");
			tmpexp=stralloc(temp);
			exp_count+=RDAstrlen(temp);

			tmpstruct=stralloc(temp);
			struct_count+=RDAstrlen(temp);

			tmplabel=stralloc(temp);
			label_count+=RDAstrlen(temp);

			if_count=0;
			prev_defused=FALSE;
			if(EXPENDITURE_ACCOUNT->num>0)
			{
				for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
					y<EXPENDITURE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
						part=acc->acct+((acc->num-1));
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}


						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[DIMENSION #%d DESCRIPTION]\n",y,(acc->num-1));
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=%d\n",y,(acc->num-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDEACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDEACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if(part->length>longest_length) longest_length=part->length;
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}

						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}

					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpexp!=NULL && y<(EXPENDITURE_ACCOUNT->num))
					{
						tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
						exp_count+=RDAstrlen(temp)+6;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+2);
						sprintf(&tmpexp[exp_count],"\t%s",temp);
						exp_count+=RDAstrlen(temp)+1;
					}

					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=0");
					for(q=0;q<(if_count);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpstruct!=NULL && y<(EXPENDITURE_ACCOUNT->num))
					{
						tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
						sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
						struct_count+=RDAstrlen(temp)+6;
					} else {
						tmpstruct=Rrealloc(tmpstruct,
							struct_count+RDAstrlen(temp)+2);
						sprintf(&tmpstruct[struct_count],"\t%s",temp);
						struct_count+=RDAstrlen(temp)+1;
					}

					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmplabel!=NULL && y<(EXPENDITURE_ACCOUNT->num))
					{
						tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
						sprintf(&tmplabel[label_count],"\tELSE %s",temp);
						label_count+=RDAstrlen(temp)+6;
					} else {
						tmplabel=Rrealloc(tmplabel,
							label_count+RDAstrlen(temp)+2);
						sprintf(&tmplabel[label_count],"\t%s",temp);
						label_count+=RDAstrlen(temp)+1;
					}
				}
			}
		}
	}
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			if_count=0;
			memset(temp,0,15001);
			sprintf(temp,"IF([ACCOUNT TYPE]=1) THEN\n");
			if(tmpexp!=NULL)
			{
				tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+6);
				sprintf(&tmpexp[exp_count],"ELSE %s",temp);
				exp_count+=RDAstrlen(temp)+5;
			} else {
				tmpexp=stralloc(temp);
				exp_count+=RDAstrlen(temp);
			}

			if(tmpstruct!=NULL)
			{
				tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+6);
				sprintf(&tmpstruct[struct_count],"ELSE %s",temp);
				struct_count+=RDAstrlen(temp)+5;
			} else {
				tmpstruct=stralloc(temp);
				struct_count+=RDAstrlen(temp);
			}

			if(tmplabel!=NULL)
			{
				tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+6);
				sprintf(&tmplabel[label_count],"ELSE %s",temp);
				label_count+=RDAstrlen(temp)+5;
			} else {
				tmplabel=stralloc(temp);
				label_count+=RDAstrlen(temp);
			}

			prev_defused=FALSE;
			if(REVENUE_ACCOUNT->num>0)
			{
				for(y=0,acc=REVENUE_ACCOUNT->codetype;
					y<REVENUE_ACCOUNT->num;++y,++acc)
				{
					if(acc->acct!=NULL && acc->num>0)
					{
						part=acc->acct+((acc->num-1));
						acc_count=1;
						for(z=0,p=acc->acct;z<(acc->num-1);++z,++p) 
						{
							if(RDAstrlen(p->name)>p->length)
							{
								acc_count+=RDAstrlen(p->name)+1;
							} else {
								acc_count+=p->length+1;
							}
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[DIMENSION #%d DESCRIPTION]\n",y,(acc->num-1));
						if_count+=1;
						if(y>0 && prev_defused==TRUE)
						{
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+7);
							sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
							exp_count+=RDAstrlen(temp)+6;
						} else {
							tmpexp=Rrealloc(tmpexp,
								exp_count+RDAstrlen(temp)+2);
							sprintf(&tmpexp[exp_count],"\t%s",temp);
							exp_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=%d\n",y,(acc->num-1));
						if(y>0 && prev_defused==TRUE)
						{
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+7);
							sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
							struct_count+=RDAstrlen(temp)+6;
						} else {
							tmpstruct=Rrealloc(tmpstruct,
								struct_count+RDAstrlen(temp)+2);
							sprintf(&tmpstruct[struct_count],"\t%s",temp);
							struct_count+=RDAstrlen(temp)+1;
						}

						memset(temp,0,15001);
						sprintf(temp,"IF([BUDPREP][BUDRACC][DEFINITION TYPE]=%d) THEN\n\t\tRETURN_VALUE=[BUDPREP][BUDRACC][ACCOUNT CODE][1,%d,%d]\n",y,acc_count,part->length);
						if(part->length>longest_length) longest_length=part->length;
						if(y>0 && prev_defused==TRUE)
						{
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+7);
							sprintf(&tmplabel[label_count],"\tELSE %s",temp);
							label_count+=RDAstrlen(temp)+6;
						} else {
							tmplabel=Rrealloc(tmplabel,
								label_count+RDAstrlen(temp)+2);
							sprintf(&tmplabel[label_count],"\t%s",temp);
							label_count+=RDAstrlen(temp)+1;
						}

						prev_defused=TRUE;
					} else {
						prev_defused=FALSE;
					}

					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpexp!=NULL && y<(REVENUE_ACCOUNT->num))
					{
						tmpexp=Rrealloc(tmpexp,
						exp_count+RDAstrlen(temp)+7);
						sprintf(&tmpexp[exp_count],"\tELSE %s",temp);
						exp_count+=RDAstrlen(temp)+6;
					} else {
						tmpexp=Rrealloc(tmpexp,
							exp_count+RDAstrlen(temp)+2);
						sprintf(&tmpexp[exp_count],"\t%s",temp);
						exp_count+=RDAstrlen(temp)+1;
					}


					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=0");
					for(q=0;q<(if_count);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmpstruct!=NULL && y<(REVENUE_ACCOUNT->num))
					{
						tmpstruct=Rrealloc(tmpstruct,
						struct_count+RDAstrlen(temp)+7);
						sprintf(&tmpstruct[struct_count],"\tELSE %s",temp);
						struct_count+=RDAstrlen(temp)+6;
					} else {
						tmpstruct=Rrealloc(tmpstruct,
							struct_count+RDAstrlen(temp)+2);
						sprintf(&tmpstruct[struct_count],"\t%s",temp);
						struct_count+=RDAstrlen(temp)+1;
					}


					memset(temp,0,15001);
					sprintf(temp,"RETURN_VALUE=\"\"");
					for(q=0;q<(if_count);++q)
					{
						strcat(temp," FI");
					}
					strcat(temp,"\n");
					if(tmplabel!=NULL && y<(REVENUE_ACCOUNT->num))
					{
						tmplabel=Rrealloc(tmplabel,
						label_count+RDAstrlen(temp)+7);
						sprintf(&tmplabel[label_count],"\tELSE %s",temp);
						label_count+=RDAstrlen(temp)+6;
					} else {
						tmplabel=Rrealloc(tmplabel,
							label_count+RDAstrlen(temp)+2);
						sprintf(&tmplabel[label_count],"\t%s",temp);
						label_count+=RDAstrlen(temp)+1;
					}
				}
			}
		}
	}
	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=\"\"");
	tmpexp=Rrealloc(tmpexp,exp_count+RDAstrlen(temp)+16);
	sprintf(&tmpexp[exp_count],"ELSE %s FI FI FI\n",temp);

	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=0");
	tmpstruct=Rrealloc(tmpstruct,struct_count+RDAstrlen(temp)+16);
	sprintf(&tmpstruct[struct_count],"ELSE %s FI FI FI\n",temp);

	memset(temp,0,15001);
	sprintf(temp,"RETURN_VALUE=\"\"");
	tmplabel=Rrealloc(tmplabel,label_count+RDAstrlen(temp)+16);
	sprintf(&tmplabel[label_count],"ELSE %s FI FI FI\n",temp);


	memset(temp,0,15001);
	memset(stemp,0,101);
	sprintf(stemp,"BUDPREP DIMENSION LAST DESCRIPTION");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(3*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDRACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("FINMGT");
	i->file=stralloc("FINCOA");
	i->keyname=NULL;
	d->vtype=1;
	d->num=3;
	d->type=PLAINTEXT;
	d->length=80;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmpexp!=NULL)
	{
		d->expression=stralloc(tmpexp);
		Rfree(tmpexp);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Description of the last dimension part from the Account Structure representing all account types and definitions, or the empty string if not appropriate.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);


	memset(stemp,0,101);
	sprintf(stemp,"BUDPREP DIMENSION LAST");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDRACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=2;
	d->type=SHORTV;
	d->length=2;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmpstruct!=NULL)
	{
		d->expression=stralloc(tmpstruct);
		Rfree(tmpstruct);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Represents Dimension number of the last dimension from the Account Structure representing all account types and definitions.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);


	memset(stemp,0,101);
	sprintf(stemp,"BUDPREP DIMENSION LAST CODE");
	d=DFvirtualNEW(modx,stemp);
	d->incvir=Rmalloc(2*sizeof(DFincvir));
	i=d->incvir;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDEACC");
	i->keyname=NULL;
	++i;
	i->module=stralloc("BUDPREP");
	i->file=stralloc("BUDRACC");
	i->keyname=NULL;
	d->vtype=1;
	d->num=2;
	d->type=PLAINTEXT;
	d->length=longest_length;
	d->when=0;
	d->avail2rsrc=TRUE;
	if(tmplabel!=NULL)
	{
		d->expression=stralloc(tmplabel);
		Rfree(tmplabel);
	} else d->expression=NULL;
	memset(temp,0,15001);
	sprintf(temp,"Represents Dimension Code of the last dimension from the Account Structure representing all account types and definitions.");
	d->desc=stralloc(temp);
	memset(temp,0,15001);
	writeDFvirtual(modx,d);
	if(d!=NULL) FreeDFvirtual(d);

	if(tmpexp!=NULL) Rfree(tmpexp);
	if(tmpstruct!=NULL) Rfree(tmpstruct);

}
