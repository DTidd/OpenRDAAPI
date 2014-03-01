/* payvrsdk.c - Make VRS Diskette */
#ifndef WIN32
#define __NAM__ "payvrsdk.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payvrsdk.exe"
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

char *module="PAYROLL",*select_expression=NULL,*life_expression=NULL;
char *health_expression=NULL,*opt_life_expression=NULL,*employer_code;
short perdmg=(-1),payjpms=(-1),payjmst=(-1),banknum=(-1),paymstr=(-1);
short payjdpm=(-1),paydedm=(-1),payddms=(-1),paydtyp=(-1),paydpms=(-1);
short posmstr=(-1), poslmst=(-1);
short payppms=(-1);
static short check_postrk(RDArsrc *,RangeScreen *);
RDAvirtual *csortsequence=NULL;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,payjdpm);
	AddFileFields(list,payjmst);
	AddFileFields(list,perdmg);
	AddFileFields(list,paymstr);
	AddFileFields(list,payjpms);
	AddFileFields(list,payppms);
	AddFileFields(list,payddms);
	AddFileFields(list,paydtyp);
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
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(posmstr!=(-1)) CLSNRD(posmstr);
	if(poslmst!=(-1)) CLSNRD(poslmst);
	if(payjpms!=(-1)) CLSNRD(payjpms);
	if(payjdpm!=(-1)) CLSNRD(payjdpm);
	if(paymstr!=(-1)) CLSNRD(paymstr);
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(paydpms!=(-1)) CLSNRD(paydpms);
	if(payppms!=(-1)) CLSNRD(payppms);
	if(payddms!=(-1)) CLSNRD(payddms);
	if(paydtyp!=(-1)) CLSNRD(paydtyp);
	if(paydedm!=(-1)) CLSNRD(paydedm);
	if(employer_code!=NULL) Rfree(employer_code);
	if(select_expression!=NULL) Rfree(select_expression);
	if(life_expression!=NULL) Rfree(life_expression);
	if(health_expression!=NULL) Rfree(health_expression);
	if(opt_life_expression!=NULL) Rfree(opt_life_expression);
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
	d=getDFvirtual("PAYROLL","VRS DISKETTE SORT SEQUENCE");
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
	keyno=KEYNUMBER(payjdpm,"PAYJDPM KEY #2");
	if(keyno==(-1))
	{
		 prterr("Error Payroll Make VRS Diskette: Can't find the PAYJDPM KEY #2, using Key Number 1");
		 keyno=1;
	}
	key=KEYNUM(payjdpm,keyno);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(payjdpm,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
			addDBkeypart(dbsrt,sortkeyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt,char *pay_id,char *per_id,int jobno,
		char *employer_code,char *pay_date,char *ssn,
		char *comment_code,double creditable_comp,
		double life_ins_base,double retire_cont_credit,
		short months,short education_months,char *activity_code,
		double health_ins_credit,double health_ins_comp,
		double retire_cont,double group_life_pre,
 		int check_number,int check_date,double opt_life_ins_pre,
		double annual_salary)
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
	FINDFLDSETSTRING(dbsrt->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(dbsrt->fileno,"PERSONNEL IDENTIFICATION",per_id);
	FINDFLDSETSTRING(dbsrt->fileno,"PAY DATE",pay_date);
	FINDFLDSETSTRING(dbsrt->fileno,"SOCIAL SECURITY NUMBER",ssn);
	FINDFLDSETSTRING(dbsrt->fileno,"COMMENT CODE",comment_code);
	FINDFLDSETDOUBLE(dbsrt->fileno,"CREDITABLE COMPENSATION",creditable_comp);
	FINDFLDSETDOUBLE(dbsrt->fileno,"LIFE INSURANCE BASE PAY",life_ins_base);
	FINDFLDSETDOUBLE(dbsrt->fileno,"RETIREMENT CONTRIBUTIONS",retire_cont_credit);
	FINDFLDSETSHORT(dbsrt->fileno,"VRS REPORTED MONTHS",months);
	FINDFLDSETSHORT(dbsrt->fileno,"EDUCATIONAL CONTRACT MONTHS",education_months);
	FINDFLDSETSTRING(dbsrt->fileno,"ACTIVITY CODE",activity_code);
	FINDFLDSETDOUBLE(dbsrt->fileno,"HEALTH INSURANCE CREDIT",health_ins_credit);
	FINDFLDSETDOUBLE(dbsrt->fileno,"HEALTH INSURANCE COMPENSATION",health_ins_credit);
	FINDFLDSETDOUBLE(dbsrt->fileno,"RETIREMENT CONTRIBUTION",retire_cont);
	FINDFLDSETDOUBLE(dbsrt->fileno,"GROUP LIFE PREMIUM",group_life_pre);
	FINDFLDSETINT(dbsrt->fileno,"CHECK NUMBER",check_number);
	FINDFLDSETSTRING(dbsrt->fileno,"CHECK DATE",check_date);
	FINDFLDSETSTRING(dbsrt->fileno,"CHECK DATE",check_date);
	key=KEYNUM(payjdpm,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(payjdpm,dbsrt->fileno,part->name);
		}
	}
	WRTNRD(dbsrt->fileno,0,NULL);
}
/*
double ComputeNet(RDArsrc *mainrsrc,char *perid,char *payid)
{
	short ef=FALSE,keyno=0;
	char *lperid=NULL,*lpayid=NULL,deleteflag=FALSE,reduce_net=TRUE;
	double net=0,amount=0;

	keyno=KEYNUMBER(payjpms2,"RDA PAYJPMS KEY");
	if(keyno<1) keyno=1;
	ZERNRD(payjpms2);
	FINDFLDSETSTRING(payjpms2,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(payjpms2,"PERSONNEL IDENTIFICATION",perid);
	ef=ADVGTENRDsec(payjpms2,keyno,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjpms2,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETSTRING(payjpms2,"PAY IDENTIFICATION",&lpayid);
		if(RDAstrcmp(payid,lpayid) || RDAstrcmp(perid,lperid)) break;
		FINDFLDGETCHAR(payjpms2,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETDOUBLE(payjpms2,"GROSS",&amount);
			net+=amount;
		}
		ef=ADVNXTNRDsec(payjpms2,keyno,SCRNvirtualSubData,mainrsrc);
	}
	keyno=KEYNUMBER(paydpms,"RDA PAYDPMS KEY");
	if(keyno<1) keyno=1;
	ZERNRD(paydpms);
	FINDFLDSETSTRING(paydpms,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(paydpms,"PERSONNEL IDENTIFICATION",perid);
	ef=ADVGTENRDsec(paydpms,keyno,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{	
		FINDFLDGETSTRING(paydpms,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETSTRING(paydpms,"PAY IDENTIFICATION",&lpayid);
		if(RDAstrcmp(payid,lpayid) || RDAstrcmp(perid,lperid)) break;
		FINDFLDGETCHAR(paydpms,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			COPYFIELD(paydpms,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(!ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(payddms,"REDUCE NET",&reduce_net);
				if(reduce_net)
				{
					FINDFLDGETDOUBLE(paydpms,"AMOUNT",&amount);
					net-=amount;
				}
			}
		}
		ef=ADVNXTNRDsec(paydpms,keyno,SCRNvirtualSubData,mainrsrc);
	}
	if(lperid!=NULL) Rfree(lperid);
	if(lpayid!=NULL) Rfree(lpayid);
	return(net);
}
*/
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
void makevrsdiskette(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE,payjpms_keyno=0;
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*perid=NULL,*last_perid=NULL,*e=NULL;
	char *bankid1=NULL,*payid1=NULL,*pay_id=NULL,*bank_id=NULL;
	RDArsrc *tmprsrc=NULL;
	int check_number=0,x=0;
	NRDpart *part;
	NRDkey *key=NULL;
	RDAvirtual *v;
	char deleteflag=FALSE,benefits_computed=FALSE;
	double net=0;
	RDATData *prev_file=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&pay_id);
	FINDRSCGETSTRING(mainrsrc,"EMPLOYER CODE",&employer_code);
	FINDRSCGETSTRING(mainrsrc,"SELECT EXPRESSION",&select_expression);
	FINDRSCGETSTRING(mainrsrc,"LIFE EXPRESSION",&life_expression);
	FINDRSCGETSTRING(mainrsrc,"HEALTH EXPRESSION",&health_expression);
	FINDRSCGETSTRING(mainrsrc,"OPTIONAL LIFE EXPRESSION",&opt_life_expression);
	sortname=unique_name();
	sortfile=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(payjdpm,rs);
	ReadRangeScreen(payjpms,rs);
	ReadRangeScreen(payppms,rs);
	ReadRangeScreen(payjmst,rs);
	ReadRangeScreen(perdmg,rs);
	ReadRangeScreen(paydedm,rs);
	ReadRangeScreen(payddms,rs);
	ReadRangeScreen(paydtyp,rs);
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
	payjdpm_keyno=KEYNUMBER(payjdpms,"RDA PAYJDPM KEY #2");
	if(payjdpm_keyno<1) payjdpm_keyno=1;
	ZERNRD(payjdpm);
	FINDFLDSETSTRING(payjdpm,"PAY IDENTIFICATION",pay_id);
	ef=ADVGTENRDsec(payjdpm,payjdpm_keyno,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjdpm,"PAY IDENTIFICATION",&payid1);
		if(RDAstrcmp(payid1,pay_id)) break;
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(payjdpm,"DELETEFLAG",&deleteflag);
		FINDFLDGETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",&perid);
/*
		net=ComputeNet(mainrsrc,perid,payid1);
		if(net!=0.0)
		{
		if(!deleteflag && !RDAstrcmp(bank_id,bankid1))
		{
*/
		FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",payid1);
		if(!ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,mainrsrc))
		{
		FINDFLDGETCHAR(payjpms,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
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
							if(!testfilerangersrc(payjdpm,rs) && 
								!testfilerangersrc(payjpms,rs) &&
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
									if(select_expression!=NULL)
									{
										if(EVALUATEbol(select_expression,
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
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		if(bankid1!=NULL) Rfree(bankid1);
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		ef=ADVNXTNRDsec(payjpms,payjpms_keyno,SCRNvirtualSubData,mainrsrc);
	}
	if(payid1!=NULL) Rfree(payid1);
	if(bank_id!=NULL) Rfree(bank_id);
	if(pay_id!=NULL) Rfree(pay_id);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(SIZNRD(sortfile->fileno))
	{
		key=KEYNUM(payjpms,1);
		if(key!=NULL)
		{
			tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
				module,"Setting Check Numbers...",NULL);
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
				for(x=0,part=key->part;x<key->numparts;++x,++part)
					COPYFIELD(sortfile->fileno,payjpms,part->name);
				if(!ADVEQLNRDsec(payjpms,1,SCRNvirtualSubData,mainrsrc))
				{
					LOCNRD(payjpms);
					prev_file=RDATDataNEW(payjpms);
					FINDFLDSETINT(payjpms,"CHECK NUMBER",check_number);
					FINDFLDSETSTRING(payjpms,"CHECK DATE",check_date);
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
		prterr("Error no Job Pay Masters selected.");
	}
	if(perid!=NULL) Rfree(perid);
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
int main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	char *bank_id=NULL,*payid=NULL;

	if(InitializeSubsystems(argc,argv,module,"MAKE VRS DISKETTE")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((payjpms=APPOPNNRD(module,"PAYJPMS",TRUE,TRUE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((payjpms2=APPOPNNRD(module,"PAYJPMS",TRUE,FALSE))==(-1)) return;
	if((paymstr=APPOPNNRD(module,"PAYMSTR",TRUE,FALSE))==(-1)) return;
	if((paydpms=APPOPNNRD(module,"PAYDPMS",TRUE,FALSE))==(-1)) return;
	if((payppms=APPOPNNRD(module,"PAYPPMS",TRUE,FALSE))==(-1)) return;
	if((posmstr=OPNNRDsec("POSTRK","POSMSTR",TRUE,FALSE,FALSE))==(-1))
	{
		if((posmstr=OPNNRD("POSTRK","POSMSTR"))>(-1))
		{
			ERRORDIALOG("Security Access Denied","Security Access denied to the [POSTRK][POSMSTR] file or the file cannot otherwise be opened.\n",NULL,TRUE);
			ShutdownSecurity();
			RDAAPPMAINLOOP();
			return;
		} else {
			CLSNRD(posmstr);
			banknum=(-1);
		}
	}
	if(posmstr!=(-1))
	{
		if((poslmst=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	} else {
		poslmst=(-1);
	}
	if((banknum=OPNNRDsec("BNKREC","BNKACC",TRUE,FALSE,FALSE))==(-1))
	{
		if((banknum=OPNNRD("BNKREC","BNKACC"))>(-1))
		{
			ERRORDIALOG("Security Access Denied","Security Access denied to the [BNKACC] file or the file cannot otherwise be opened.\n",NULL,TRUE);
			ShutdownSecurity();
			RDAAPPMAINLOOP();
			return;
		} else {
			CLSNRD(banknum);
			banknum=(-1);
		}
	}
	mainrsrc=RDArsrcNEW(module,"MAKE VRS DISKETTE");
	addDFincvir(mainrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(mainrsrc,module,"PAYJPMS",NULL,payjpms2);
	addDFincvir(mainrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mainrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(mainrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(mainrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(mainrsrc,module,"PAYDPMS",NULL,paydpms);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	if(posmstr!=(-1))
	{
		addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmstr);
		addDFincvir(mainrsrc,"POSTRK","POSLMST",NULL,poslmst);
	}
	if(banknum!=(-1))
	{
		addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,banknum);
	}
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	if(perdmg!=(-1)) file2rangersrc(perdmg,mainrsrc);
	if(payjmst!=(-1)) file2rangersrc(payjmst,mainrsrc);
	if(payjpms!=(-1)) file2rangersrc(payjpms,mainrsrc);
	if(payppms!=(-1)) file2rangersrc(payppms,mainrsrc);
	if(paymstr!=(-1)) file2rsrc(paymstr,mainrsrc,FALSE);
	if(posmstr!=(-1)) file2rsrc(posmstr,mainrsrc,FALSE);
	if(poslmst!=(-1)) file2rsrc(poslmst,mainrsrc,FALSE);
	if(banknum!=(-1)) file2rsrc(banknum,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",TRUE);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	FINDRSCSETFUNC(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",payidcb,NULL);
	FINDRSCSETFUNC(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",bankidcb,NULL);
	addstdrsrc(mainrsrc,"EMPLOYER CODE",PLAINTEXT,5,NULL,TRUE);
	addsctrsrc(mainrsrc,"SELECT EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"LIFE EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"HEALTH EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"OPT LIFE EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(paymstr,1);
	filerecord2rsrc(paymstr,mainrsrc);
	if(banknum!=(-1))
	{	
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
		FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",bank_id);
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(banknum,1);
		filerecord2rsrc(banknum,mainrsrc);
	}
	if(payid!=NULL) Rfree(payid);
	if(bank_id!=NULL) Rfree(bank_id);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"SELECT EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"SELECT EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"SELECT EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"SELECT EXPRESSION");
	addbtnrsrc(mainrsrc,"LIFE LOAD VALUE",TRUE,loadvalue,"LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"LIFE LOAD OPERATOR",TRUE,loadoperand,"LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"LIFE LOAD GROUPER",TRUE,loadgrouper,"LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"LIFE LOAD CONTROL",TRUE,loadcontrol,"LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"HEALTH LOAD VALUE",TRUE,loadvalue,"HEALTH EXPRESSION");
	addbtnrsrc(mainrsrc,"HEALTH LOAD OPERATOR",TRUE,loadoperand,"HEALTH EXPRESSION");
	addbtnrsrc(mainrsrc,"HEALTH LOAD GROUPER",TRUE,loadgrouper,"HEALTH EXPRESSION");
	addbtnrsrc(mainrsrc,"HEALTH LOAD CONTROL",TRUE,loadcontrol,"HEALTH EXPRESSION");
	addbtnrsrc(mainrsrc,"OPT LIFE LOAD VALUE",TRUE,loadvalue,"OPT LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"OPT LIFE LOAD OPERATOR",TRUE,loadoperand,"OPT LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"OPT LIFE LOAD GROUPER",TRUE,loadgrouper,"OPT LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"OPT LIFE LOAD CONTROL",TRUE,loadcontrol,"OPT LIFE EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,makevrsdiskette,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
