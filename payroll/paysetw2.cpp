/* paysetw2.c - Set W-2 Information */
#ifndef WIN32
#define __NAM__ "paysetw2.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paysetw2.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="PAYROLL",*vch_expression=NULL;
short perdmg=(-1),payjpms=(-1),payjmst=(-1),banknum=(-1),paymstr=(-1);
short posmstr=(-1), poslmst=(-1);
short payppms=(-1),payjdpm=(-1),/*paydpms=(-1),*/payddms=(-1);
short paydtyp=(-1);
static short check_postrk(RDArsrc *,RangeScreen *);
int start_check=1;
char *check_date=NULL;
RDAvirtual *csortsequence=NULL;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,payjpms);
	AddFileFields(list,payjmst);
	AddFileFields(list,perdmg);
	AddFileFields(list,paymstr);
	if(posmstr!=(-1))
	{
		AddFileFields(list,posmstr);
		AddFileFields(list,poslmst);
	}
	addAPPlibNoDuplicates(list,"[PAGE NO]");
	addAPPlibNoDuplicates(list,"[BODY COUNT]");
	addAPPlibNoDuplicates(list,"[BREAK LEVEL]");
	addAPPlibNoDuplicates(list,"[PRIMARY FILE NUMBER]");
	addAPPlibNoDuplicates(list,"[WRITE COUNT]");
	loadglobals(list);
	SORTAPPlib(list);
	return(list);
}
void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist();
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
void quitfunc(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	if(banknum!=(-1)) CLSNRD(banknum);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(posmstr!=(-1)) CLSNRD(posmstr);
	if(poslmst!=(-1)) CLSNRD(poslmst);
	if(payjdpm!=(-1)) CLSNRD(payjdpm);
	if(payjpms!=(-1)) CLSNRD(payjpms);
	if(paymstr!=(-1)) CLSNRD(paymstr);
	if(perdmg!=(-1)) CLSNRD(perdmg);
/*
	if(paydpms!=(-1)) CLSNRD(paydpms);
*/
	if(payppms!=(-1)) CLSNRD(payppms);
	if(paydtyp!=(-1)) CLSNRD(paydtyp);
	if(check_date!=NULL) Rfree(check_date);
	if(vch_expression!=NULL) Rfree(vch_expression);
	ShutdownSubsystems();
}
void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short sortkeyno=(-1),keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	sortkeyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("PAYROLL","DIRECT DEPOSIT SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=PP_translate(d->expression);
		} else { 
			csortsequence->expression=NULL;
		}
		csortsequence->computed=FALSE;
		csortsequence->range=FALSE;
		csortsequence->dlen=0;
		switch(csortsequence->type)
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
				csortsequence->value.string_value=NULL;
				csortsequence->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				csortsequence->value.string_value=Rmalloc(1);
				*csortsequence->value.string_value=0;
				csortsequence->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				csortsequence->value.float_value=Rmalloc(sizeof(double));
				*csortsequence->value.float_value=0;
				csortsequence->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				csortsequence->value.short_value=Rmalloc(sizeof(short));
				*csortsequence->value.short_value=0;
				csortsequence->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				csortsequence->value.integer_value=Rmalloc(sizeof(int));
				*csortsequence->value.integer_value=0;
				csortsequence->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
		addDBfield(dbsrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsrt,sortkeyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	keyno=KEYNUMBER(payjpms,"PAYJPMS KEY");
	if(keyno==(-1))
	{
		 prterr("Error Payroll Set Direct Deposit Numbers: Can't find the PAYJPMS KEY, using Key Number 1");
		 keyno=1;
	}
	key=KEYNUM(payjpms,keyno);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(payjpms,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
			addDBkeypart(dbsrt,sortkeyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt)
{
	char deleteflag=FALSE;
	NRDkey *key=NULL;
	NRDpart *part;
	int x;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		switch(csortsequence->type)
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
				FINDFLDSETSTRING(dbsrt->fileno,csortsequence->name,
					csortsequence->value.string_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				FINDFLDSETCHAR(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				FINDFLDSETDOUBLE(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				FINDFLDSETSHORT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				FINDFLDSETINT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.integer_value);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
	}
	key=KEYNUM(payjpms,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(payjpms,dbsrt->fileno,part->name);
		}
	}
	WRTNRD(dbsrt->fileno,0,NULL);
}
double ComputeDDamount(RDArsrc *mainrsrc,char *perid,char *payid,char *ded_type,RangeScreen *rs)
{
	short ef=FALSE,keyno=0;
	char *lperid=NULL,*lpayid=NULL,deleteflag=FALSE;
	double total_amount=0,amount=0;
	char *dedtype1=NULL;

	keyno=KEYNUMBER(payjdpm,"PAYJDPM KEY #2");
	if(keyno<1) keyno=1;
	ZERNRD(payjdpm);
	FINDFLDSETSTRING(payjdpm,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",perid);
	ef=ADVGTENRDsec(payjdpm,keyno,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETSTRING(payjdpm,"PAY IDENTIFICATION",&lpayid);
		if(RDAstrcmp(payid,lpayid) || RDAstrcmp(perid,lperid))
		{
			break;
		}
		FINDFLDGETCHAR(payjdpm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			COPYFIELD(payjdpm,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(!ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,mainrsrc))
			{
			COPYFIELD(payddms,paydtyp,"DEDUCTION TYPE");
			if(!ADVEQLNRDsec(paydtyp,1,SCRNvirtualSubData,mainrsrc))
			{
			FINDFLDGETSTRING(paydtyp,"DEDUCTION TYPE",&dedtype1);
			if(!RDAstrcmp(dedtype1,ded_type)) 
			{
/*
				if(!testfilerangersrc(paydtyp,rs) &&
*/
				if(!testfilerangersrc(payddms,rs))
				{
					FINDFLDGETDOUBLE(payjdpm,"AMOUNT",&amount);
					total_amount+=amount;
				}
			}
			}
			}
		}
		ef=ADVNXTNRDsec(payjdpm,keyno,SCRNvirtualSubData,mainrsrc);
	}
	return(total_amount);
}
static void setDDnumbers(RDArsrc *mainrsrc,char *ded_type,char *pay_id,char *perid,int jobno,int check_number,char *check_date,RangeScreen *rs)
{
	short ef=FALSE,keyno=0;
	char *lperid=NULL,*lpayid=NULL,deleteflag=FALSE;
	char *dedtype1=NULL;
	RDATData *prev_file=NULL;

	keyno=KEYNUMBER(payjdpm,"PAYJDPM KEY #2");
	if(keyno<1) keyno=1;
	ZERNRD(payjdpm);
	FINDFLDSETSTRING(payjdpm,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",perid);
	ef=ADVGTENRDsec(payjdpm,keyno,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETSTRING(payjdpm,"PAY IDENTIFICATION",&lpayid);
		if(RDAstrcmp(pay_id,lpayid) || RDAstrcmp(perid,lperid)) break;
		FINDFLDGETCHAR(payjdpm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			COPYFIELD(payjdpm,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(!ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,mainrsrc))
			{
			COPYFIELD(payddms,paydtyp,"DEDUCTION TYPE");
			if(!ADVEQLNRDsec(paydtyp,1,SCRNvirtualSubData,mainrsrc))
			{
			FINDFLDGETSTRING(paydtyp,"DEDUCTION TYPE",&dedtype1);
			if(!RDAstrcmp(dedtype1,ded_type)) 
			{
/*
				if(!testfilerangersrc(paydtyp,rs) &&
*/
				if(!testfilerangersrc(payddms,rs))
				{
					LOCNRD(payjdpm);
					prev_file=RDATDataNEW(payjdpm);
					FINDFLDSETINT(payjdpm,"DIRECT DEPOSIT NUMBER",check_number);
					FINDFLDSETSTRING(payjdpm,"DIRECT DEPOSIT DATE",check_date);
					ADVWRTTRANSsec(payjdpm,0,NULL,prev_file,SCRNvirtualSubData,mainrsrc);
					UNLNRD(payjdpm);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				}
			}
			}
			}
		}
		ef=ADVNXTNRDsec(payjdpm,keyno,SCRNvirtualSubData,mainrsrc);
	}
}
static short check_postrk(RDArsrc *mainrsrc,RangeScreen *rs)
{
	char *posid=NULL,*locid=NULL;
	char deleteflag=FALSE;
	short return_value=TRUE;

	if(posmstr!=(-1))
	{
		FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&posid);
		FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",posid);
		if(ADVEQLNRDsec(posmstr,1,SCRNvirtualSubData,mainrsrc))
		{ 
			return_value=FALSE;
		} else {
			FINDFLDGETCHAR(posmstr,"DELETEFLAG",&deleteflag);
			if(deleteflag)
			{
				return_value=FALSE;
			} else {
			
				if(testfilerangersrc(posmstr,rs))
				{
					return_value=FALSE;
				} else {
					FINDFLDGETSTRING(posmstr,"LOCATION IDENTIFICATION",&locid);
					FINDFLDSETSTRING(poslmst,"LOCATION IDENTIFICATION",locid);
					if(ADVEQLNRDsec(poslmst,1,SCRNvirtualSubData,mainrsrc))
					{
						return_value=FALSE;
					} else {
						FINDFLDGETCHAR(poslmst,"DELETEFLAG",&deleteflag);
						if(deleteflag)
						{
							return_value=FALSE;
						} else {
							if(testfilerangersrc(poslmst,rs))
							{
								return_value=FALSE;
							} else {
								return_value=TRUE;
							}
						}
					}
				}
			}
		}
	} else {
		return_value=TRUE;
	}
	if(posid!=NULL) Rfree(posid);
	if(locid!=NULL) Rfree(locid);
	return(return_value);
}
void setcheck(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE,payjpms_keyno=0;
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*perid=NULL,*last_perid=NULL,*e=NULL;
	char *bankid1=NULL,*payid1=NULL,*pay_id=NULL,*bank_id=NULL;
	char *ded_type=NULL;
	RDArsrc *tmprsrc=NULL;
	int check_number=0,x=0,jobno=0;
	NRDpart *part;
	NRDkey *key=NULL;
	RDAvirtual *v;
	char deleteflag=FALSE,benefits_computed=FALSE;
	double amount=0;
	RDATData *prev_file=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&vch_expression);
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&pay_id);
	FINDRSCGETSTRING(mainrsrc,"[PAYDTYP][DEDUCTION TYPE]",&ded_type);
	FINDRSCGETSTRING(mainrsrc,"DIRECT DEPOSIT DATE",&check_date);
	FINDRSCGETINT(mainrsrc,"STARTING DIRECT DEPOSIT NUMBER",&start_check);
	if(banknum!=(-1))
	{			
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	}
	sortname=unique_name();
	sortfile=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(payjpms,rs);
	ReadRangeScreen(payppms,rs);
	ReadRangeScreen(payjmst,rs);
	ReadRangeScreen(perdmg,rs);
	ReadRangeScreen(paydtyp,rs);
	ReadRangeScreen(payddms,rs);
	if(posmstr!=(-1))
	{
		ReadRangeScreen(posmstr,rs);
		ReadRangeScreen(poslmst,rs);
	}
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(payjpms,"DIAGNOSTIC SCREEN",module,
		"Setting Job Pay Masters....",NULL);
	if(tmprsrc!=NULL)
	{
		if(!ADVmakescrn(tmprsrc,TRUE))
		{	
			ForceWindowUpdate(tmprsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	payjpms_keyno=KEYNUMBER(payjpms,"RDA PAYJPMS KEY");
	if(payjpms_keyno<1) payjpms_keyno=1;
	ZERNRD(payjpms);
	FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
	ef=ADVGTENRDsec(payjpms,payjpms_keyno,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjpms,"PAY IDENTIFICATION",&payid1);
		if(RDAstrcmp(payid1,pay_id)) break;
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(payjpms,"DELETEFLAG",&deleteflag);
		FINDFLDGETSTRING(payjpms,"PERSONNEL IDENTIFICATION",&perid);
		if(banknum!=(-1))
		{
			if(!isEMPTY(bank_id))
			{
				FINDFLDGETSTRING(payjpms,"BANK IDENTIFICATION",&bankid1);
			}
		}
		if(!deleteflag && !RDAstrcmp(bank_id,bankid1))
		{
			amount=ComputeDDamount(mainrsrc,perid,payid1,ded_type,rs);
			if(amount!=0.0)
			{
			FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid1);
			if(!ADVEQLNRDsec(payppms,1,SCRNvirtualSubData,mainrsrc))
			{
			FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(payppms,"BENEFITS COMPUTED",&benefits_computed);
			if(!deleteflag && benefits_computed)
			{
			FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
			COPYFIELD(payjpms,payjmst,"JOB NUMBER");
			if(!ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",perid);
					if(!ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDGETCHAR(perdmg,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							if(!testfilerangersrc(payjpms,rs) && 
								!testfilerangersrc(payjmst,rs) &&
								!testfilerangersrc(payppms,rs) &&
								!testfilerangersrc(perdmg,rs)) 
							{
							if((check_postrk(mainrsrc,rs))==TRUE)
							{
								for(x=0,v=mainrsrc->virflds;
									x<mainrsrc->num;++x,++v)
								{
									if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mainrsrc)) 
										break;
								}
								if(x>=mainrsrc->num)
								{
									if(vch_expression!=NULL)
									{
										if(EVALUATEbol(vch_expression,
											SCRNvirtualSubData,
											mainrsrc))
										{
											if(csortsequence!=NULL)
											{
												computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
											}
											addsortrecord(sortfile);
											increment_diagnostic=TRUE;
										}
									} else {
										if(csortsequence!=NULL)
										{
											computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
										}
										addsortrecord(sortfile);
										increment_diagnostic=TRUE;
									}
								}
							}
							}
						}
					}
				}
			}
			}
			}
			}
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		if(bankid1!=NULL) Rfree(bankid1);
		ef=ADVNXTNRDsec(payjpms,payjpms_keyno,SCRNvirtualSubData,mainrsrc);
	}
	if(payid1!=NULL) Rfree(payid1);
	if(bank_id!=NULL) Rfree(bank_id);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(SIZNRD(sortfile->fileno))
	{
		key=KEYNUM(payjpms,1);
		if(key!=NULL)
		{
			tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
				module,"Setting Direct Deposit Numbers...",NULL);
			if(tmprsrc!=NULL)
			{
				if(!ADVmakescrn(tmprsrc,TRUE))
				{	
					ForceWindowUpdate(tmprsrc);
				} else {
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				}
			}
			check_number=start_check;
			ef=FRSNRD(sortfile->fileno,1);
			FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",
				&perid);
			last_perid=stralloc(perid);
			while(!ef)
			{
				FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&perid);
				if(RDAstrcmp(last_perid,perid))
				{
					if(last_perid!=NULL) Rfree(last_perid);
					++check_number;
					last_perid=stralloc(perid);
				}
				FINDFLDGETINT(sortfile->fileno,"JOB NUMBER",&jobno);
				setDDnumbers(mainrsrc,ded_type,pay_id,perid,jobno,check_number,check_date,rs);
				for(x=0,part=key->part;x<key->numparts;++x,++part)
					COPYFIELD(sortfile->fileno,payjpms,part->name);
				if(!ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,mainrsrc))
				{
					LOCNRD(payjpms);
					prev_file=RDATDataNEW(payjpms);
/*
					FINDFLDSETCHAR(payjpms,"CONTAINS DIRECT DEPOSIT",TRUE);
*/
					FINDFLDSETINT(payjpms,"DIRECT DEPOSIT NUMBER",check_number);
					FINDFLDSETSTRING(payjpms,"DIRECT DEPOSIT DATE",check_date);
					ADVWRTTRANSsec(payjpms,0,NULL,prev_file,SCRNvirtualSubData,mainrsrc);
					UNLNRD(payjpms);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				}
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				ef=NXTNRD(sortfile->fileno,1);
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
	} else {
		prterr("Error no Job Deduction Pay Masters selected.");
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(pay_id!=NULL) Rfree(pay_id);
	if(perid!=NULL) Rfree(perid);
	if(ded_type!=NULL) Rfree(ded_type);
	if(last_perid!=NULL) Rfree(last_perid);
	if(bankid1!=NULL) Rfree(bankid1);
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
}
static void bankidcb(RDArsrc *mainrsrc)
{
	char *bank_id=NULL,delflag=FALSE;

	readwidget(mainrsrc,"[BNKACC][BANK IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",bank_id);
	if(bank_id!=NULL) Rfree(bank_id);
	if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(banknum);
		}
	} else {
		FINDFLDGETCHAR(banknum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc))
			{
				ZERNRD(banknum);
			}
		}
	}
	updatefilerecord2rsrc(banknum,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
static void payidcb(RDArsrc *mainrsrc)
{
	char *payid=NULL,delflag=FALSE;

	readwidget(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(payid!=NULL) Rfree(payid);
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc))
		{
		}	
	}
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(paymstr);
		}
	} else {
		FINDFLDGETCHAR(paymstr,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc))
			{
				ZERNRD(paymstr);
			}
		}
	}
	updatefilerecord2rsrc(paymstr,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
static void dedtypcb(RDArsrc *mainrsrc)
{
	char *dedtype=NULL,delflag=FALSE;

	readwidget(mainrsrc,"[PAYDTYP][DEDUCTION TYPE]");
	FINDRSCGETSTRING(mainrsrc,"[PAYDTYP][DEDUCTION TYPE]",&dedtype);
	FINDFLDSETSTRING(paydtyp,"DEDUCTION TYPE",dedtype);
	if(dedtype!=NULL) Rfree(dedtype);
	if(ADVEQLNRDsec(paydtyp,1,SCRNvirtualSubData,mainrsrc))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(paydtyp,1,SCRNvirtualSubData,mainrsrc))
		{
		}	
	}
	if(ADVEQLNRDsec(paydtyp,1,SCRNvirtualSubData,mainrsrc))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(paydtyp,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(paydtyp);
		}
	} else {
		FINDFLDGETCHAR(paydtyp,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(paydtyp,1,SCRNvirtualSubData,mainrsrc))
			{
				ZERNRD(paydtyp);
			}
		}
	}
	updatefilerecord2rsrc(paydtyp,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
int main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	char *bank_id=NULL,*payid=NULL,*dedtype=NULL;

	if(InitializeSubsystems(argc,argv,module,"SETUP W-2 VALUES")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((paydcsm=APPOPNNRD(module,"PAYDCSM",TRUE,FALSE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDEDM",TRUE,FALSE))==(-1)) return;
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payw2typ=APPOPNNRD(module,"PAYW2TYP",TRUE,FALSE))==(-1)) return;
	if((payempr=APPOPNNRD(module,"PAYEMPR",TRUE,FALSE))==(-1)) return;
	if((paydtyp=APPOPNNRD(module,"PAYDTYP",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"SETUP W-2 VALUES");
	addDFincvir(mainrsrc,module,"PAYDCSM",NULL,paydcsm);
	addDFincvir(mainrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(mainrsrc,module,"PAYDEDM",NULL,paydedm);
	addDFincvir(mainrsrc,module,"PAYEMPR",NULL,perdmg);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mainrsrc,module,"PAYDTYP",NULL,paydtyp);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	if(posmstr!=(-1))
	{
		addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmstr);
		addDFincvir(mainrsrc,"POSTRK","POSLMST",NULL,poslmst);
	}
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	if(perdmg!=(-1)) file2rangersrc(perdmg,mainrsrc);
	if(payjmst!=(-1)) file2rangersrc(payjmst,mainrsrc);
	if(payddms!=(-1)) file2rangersrc(payddms,mainrsrc);
	if(paydtyp!=(-1)) file2rangersrc(paydtyp,mainrsrc);
	if(payddms!=(-1)) file2rsrc(payddms,mainrsrc,FALSE);
	if(paydtyp!=(-1)) file2rsrc(paydtyp,mainrsrc,FALSE);
	if(posmstr!=(-1)) file2rsrc(posmstr,mainrsrc,FALSE);
	if(poslmst!=(-1)) file2rsrc(poslmst,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[PAYDTYP][DEDUCTION TYPE]",TRUE);
	FINDRSCSETFUNC(mainrsrc,"[PAYDTYP][DEDUCTION TYPE]",dedtypcb,NULL);
	addstdrsrc(mainrsrc,"STARTING CONTROL NUMBER",LONGV,8,&start_check,TRUE);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	if(ADVEQLNRDsec(paydcsm,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(paydcsm,1);
	filerecord2rsrc(paydcsm,mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,setcheck,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
