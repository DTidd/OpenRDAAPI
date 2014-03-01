/* payvschk.c - Set Vendor Withholding Check Numbers */
#ifndef WIN32
#define __NAM__ "payvschk.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payvschk.exe"
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
short vennum=(-1),venadr=(-1),payddsv=(-1),payddms=(-1),body_count=10;
short baccnum=(-1),banknum=(-1);
int start_check=1;
char record_check_number=FALSE;
static char *check_date=NULL,DID_STUFF=FALSE;
char *bank_id=NULL;
RDAvirtual *csortsequence=NULL;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,payddsv);
	AddFileFields(list,payddms);
	AddFileFields(list,vennum);
	AddFileFields(list,venadr);
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
	APPlib *args=NULL;

	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	if(baccnum!=(-1)) CLSNRD(baccnum);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(venadr!=(-1)) CLSNRD(venadr);
	if(payddsv!=(-1)) CLSNRD(payddsv);
	if(payddms!=(-1)) CLSNRD(payddms);
	if(banknum!=(-1)) CLSNRD(banknum);
	if(check_date!=NULL) Rfree(check_date);
	if(vch_expression!=NULL) Rfree(vch_expression);
	if(DID_STUFF)
	{
		args=APPlibNEW();
		addAPPlib(args,"PAYROLL");
		addAPPlib(args,"CHKBNKCHECKNUMVW");
		Execute2Program("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	ShutdownSubsystems();
}
void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short keyno=(-1),keynum=0;

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"VENDOR WITHHOLDING CHECK SORT SEQUENCE");
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
		addDBkeypart(dbsrt,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	addDBfilefield(dbsrt,vennum,"VENDOR IDENTIFICATION");
	addDBkeypart(dbsrt,keyno,"VENDOR IDENTIFICATION");
	addDBfilefield(dbsrt,venadr,"ADDRESS IDENTIFICATION");
	addDBkeypart(dbsrt,keyno,"ADDRESS IDENTIFICATION");
	addDBfield(dbsrt,"BREAK IDENTIFICATION",1,30);
	addDBkeypart(dbsrt,keyno,"BREAK IDENTIFICATION");
	keynum=KEYNUMBER(payddsv,"PAYDDSV KEY");
	if(keynum<1) keynum=1;
	key=KEYNUM(payddsv,keynum);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(payddsv,part->name);
			if(field!=NULL) 
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
				addDBkeypart(dbsrt,keyno,part->name);
			}
		}
	}
	addDBfield(dbsrt,"DEFAULT CHECK DETAIL",BOOLNS,1);
	addDBfield(dbsrt,"SEPARATE CHECK",BOOLNS,1);
}
void addsortrecord(DBsort *dbsrt)
{
	char deleteflag=FALSE,separate_c=FALSE;
	NRDkey *key=NULL;
	NRDpart *part;
	int x;
	short keyno=0;

	ZERNRD(dbsrt->fileno);
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
	keyno=KEYNUMBER(payddsv,"PAYDDSV KEY");
	if(keyno<1) keyno=1;
	key=KEYNUM(payddsv,keyno);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(payddsv,dbsrt->fileno,part->name);
		}
	}
	COPYFIELD(payddsv,dbsrt->fileno,"VENDOR IDENTIFICATION");
	COPYFIELD(payddsv,dbsrt->fileno,"ADDRESS IDENTIFICATION");
	FINDFLDGETCHAR(payddsv,"SEPARATE CHECK",&separate_c);
	if(separate_c==TRUE)
	{
		FIELDCOPY(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",dbsrt->fileno,"BREAK IDENTIFICATION");
	}
	COPYFIELD(vennum,dbsrt->fileno,"DEFAULT CHECK DETAIL");
	COPYFIELD(payddsv,dbsrt->fileno,"SEPARATE CHECK");
	WRTNRD(dbsrt->fileno,0,NULL);
}
int voucher_count(RDArsrc *mainrsrc,short fileno,char *vendorid,char *venaddrid,char *breakid)
{
	int count=0;
	short ef;
	char *venid=NULL,*addrid=NULL,*bid=NULL;

/*
	ZERNRD(fileno);
*/
	FINDFLDSETSTRING(fileno,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(fileno,"ADDRESS IDENTIFICATION",venaddrid);
	FINDFLDSETSTRING(fileno,"BREAK IDENTIFICATION",breakid);
	ef=ADVGTENRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(fileno,"VENDOR IDENTIFICATION",&venid);
		FINDFLDGETSTRING(fileno,"ADDRESS IDENTIFICATION",&addrid);
		FINDFLDGETSTRING(fileno,"BREAK IDENTIFICATION",&bid);
		if(!RDAstrcmp(venid,vendorid) && !RDAstrcmp(addrid,venaddrid) && !RDAstrcmp(breakid,bid))
		{
			++count;
		} else {
			break;
		}
		ef=ADVNXTNRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	}
	if(venid!=NULL) Rfree(venid);
	if(addrid!=NULL) Rfree(addrid);
	if(bid!=NULL) Rfree(bid);
	return(count);
}
void setcheck(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE,sortfile2=(-1);
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*vendorid=NULL,*last_vendorid=NULL;
	char *venaddrid=NULL,*last_venaddrid=NULL,*break_id=NULL,*last_breakid=NULL;
	char *bankid1=NULL,*dedid=NULL,*e=NULL;
	char default_check_detail=FALSE,sep=FALSE;
	int fyear=0,transno=0;
	RDArsrc *tmprsrc=NULL;
	int ven_count=0,x=0,numfields=0;
	int check_number=0,vindx=0,virt_chk_no=0;
	NRDpart *part;
	NRDkey *key=NULL;
	NRDfield *field=NULL,*fields=NULL;
	RDAvirtual *v;
	char flag=FALSE/*,*libx=NULL,*temp1=NULL*/;
	RDATData *prev_file=NULL;
	RDATData *prev=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&vch_expression);
	FINDRSCGETSHORT(mainrsrc,"BODY COUNT",&body_count);
	if(banknum!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	}
	sortname=unique_name();
	sortfile=DBsortNEW("VENPMT",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(payddsv,rs);
	ReadRangeScreen(payddms,rs);
	ReadRangeScreen(vennum,rs);
	ReadRangeScreen(venadr,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(payddsv,"DIAGNOSTIC SCREEN",module,
		"Selecting Vendor Vouchers",NULL);
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
	ef=ADVBEGNRDsec(payddsv,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(payddsv,"SOURCE USER VERIFIED",&flag);
		if(flag)
		{
		if(banknum!=(-1))
		{
			if(!isEMPTY(bank_id))
			{
				FINDFLDGETSTRING(payddsv,"BANK IDENTIFICATION",&bankid1);
			}
		}
		if(!RDAstrcmp(bank_id,bankid1))
		{
		FINDFLDGETCHAR(payddsv,"DELETEFLAG",&flag);
		if(!flag)
		{
		FINDFLDGETCHAR(payddsv,"BNKREC UPDATED",&flag);
		if(banknum==(-1) || (!flag && banknum!=(-1)))
		{
		FINDFLDGETCHAR(payddsv,"ELECTRONIC TRANSFER",&flag);
		if(!flag)
		{
		FINDFLDGETCHAR(payddsv,"MANUAL VOUCHER",&flag);
		if(!flag)
		{
		FINDFLDGETCHAR(payddsv,"DISTRIBUTED",&flag);
		if(!flag)
		{
		if(!testfilerangersrc(payddsv,rs))
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
			if(!ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,mainrsrc))
			{
			FINDFLDGETCHAR(payddms,"PRINT CHECK",&flag);
			if(flag)
			{
			if(!testfilerangersrc(payddms,rs)) 
			{
			COPYFIELD(payddsv,baccnum,"DEFINITION TYPE");
			COPYFIELD(payddsv,baccnum,"ACCOUNT CODE");
			if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(baccnum,1);
			FINDFLDGETSTRING(payddsv,"VENDOR IDENTIFICATION",&vendorid);
			FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid);
			if(!ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc))
			{
			FINDFLDGETCHAR(vennum,"DELETEFLAG",&flag);
			if(!flag)
			{
			FINDFLDGETCHAR(vennum,"ACTIVE",&flag);
			if(flag)
			{
			if(!testfilerangersrc(vennum,rs)) 
			{
				FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
				FINDFLDGETSTRING(payddsv,"ADDRESS IDENTIFICATION",&venaddrid);
				FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",venaddrid);
				if(!ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mainrsrc))
				{
				FINDFLDGETCHAR(venadr,"DELETEFLAG",&flag);
				if(!flag)
				{
				if(!testfilerangersrc(venadr,rs)) 
				{
					for(x=0,v=mainrsrc->virflds;
						x<mainrsrc->num;++x,++v)
					{
						if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mainrsrc)) break;
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
					} else { 
					if(diagapps)
					{
						prterr("Skipped by Ranging on Virtual Field [%s].",v->name);
					}
					}
				} else {
				if(diagapps)
				{
					prterr("Skipped the [VENPMT][VENADR] (Vendor Address) data file record Vendor Identification [%s] and Address Identification [%s] because of Ranging on Field Values.",
					(vendorid==NULL?"":vendorid),
					(venaddrid==NULL?"":venaddrid));
				}
				}
				} else {
				if(diagapps)
				{
						prterr("Skipped the [VENPMT][VENADR] (Vendor Address) data file record Vendor Identification [%s] and Address Identification [%s] because of DELETEFLAG field value of TRUE.",(vendorid==NULL?"":vendorid),(venaddrid==NULL?"":venaddrid));
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped because the [VENPMT][VENADR] (Vendor Address) data file record for Vendor Identification [%s] and Address Identification [%s]  doesn't exist.",(vendorid==NULL?"":vendorid),(venaddrid==NULL?"":venaddrid));
				}
				}
			} else {
			if(diagapps)
			{
				prterr("Skipped the [VENPMT][VENMSR] (Vendor Master) data file record Vendor Identification [%s] because of Ranging on Field Values.",
				(vendorid==NULL?"":vendorid));
			}
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped the [VENPMT][VENMSR] (Vendor Master) data file record Vendor Identification [%s] because of ACTIVE field value of FALSE.",(vendorid==NULL?"":vendorid));
			}	
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped the [VENPMT][VENMSR] (Vendor Master) data file record Vendor Identification [%s] because of DELETEFLAG field value of TRUE.",(vendorid==NULL?"":vendorid));
			}	
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped because the [VENPMT][VENMSR] (Vendor Master) data file record for Vendor Identification [%s] doesn't exist.",(vendorid==NULL?"":vendorid));
			}
			}

			} else {
			if(diagapps)
			{
				prterr("Skipped because the [PAYROLL][PAYDDMS] (Payroll Deduction Description Master) data file record Deduction Description Identification [%s] because of Ranging on Field Values.",
				(dedid==NULL?"":dedid));
			}	
			}
			} else {
			if(diagapps)
			{
			prterr("Skipped the [PAYROLL][PAYDDMS] (Payroll Deduction Desciption Master) data file record Deduction Description Identification [%s] because of PRINT CHECK field value of FALSE.",(dedid==NULL?"":dedid));
			}	
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped because the [PAYROLL][PAYDDMS] (Payroll Deduction Description Master) data file record for Deduction Description Identification [%s] doesn't exist.",(dedid==NULL?"":dedid));
			}
			}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduciont Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of Ranging on Field Values.",
				(dedid==NULL?"":dedid),fyear,transno);
		}
		}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduction Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of DISTRIBUTION field value of TRUE.",
				(dedid==NULL?"":dedid),fyear,transno);
		}
		}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduction Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of MANUAL VOUCHER field value of TRUE.",
				(dedid==NULL?"":dedid),fyear,transno);
		}
		}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduction Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of ELECTRONIC TRANSFER field value of TRUE.",
				(dedid==NULL?"":dedid),fyear,transno);
		}
		}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduction Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of BNKREC UPDATED field value of TRUE.",
				(dedid==NULL?"":dedid),fyear,transno);
		}
		}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduciont Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of DELETEFLAG field value of TRUE.",
				(dedid==NULL?"":dedid),fyear,transno);
		}
		}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduciont Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of BANK IDENTIFICATION [%s] not equaling selectded Bank Identification of [%s].",(dedid==NULL?"":dedid),fyear,transno,(bankid1==NULL?"":bankid1),(bank_id==NULL?"":bank_id));
		}
		}
		} else {
		if(diagapps)
		{
			FINDFLDGETSTRING(payddsv,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			FINDFLDGETINT(payddsv,"FISCAL YEAR",&fyear);
			FINDFLDGETINT(payddsv,"TRANSACTION NUMBER",&transno);
			prterr("Skipped the [PAYROLL][PAYDDSV] (Deduciont Description Voucher) data file record Deduction Description Identification [%s], Fiscal Year [%d], and Transaction Number [%d] because of SOURCE USER VERIFIED field value of FALSE.",(dedid==NULL?"":dedid),fyear,transno);
		}
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		if(dedid!=NULL) Rfree(dedid);
		ef=ADVSEQNRDsec(payddsv,SCRNvirtualSubData,mainrsrc);
	}
	if(bankid1!=NULL) Rfree(bankid1)
	if(dedid!=NULL) Rfree(dedid);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(SIZNRD(sortfile->fileno))
	{
		sortfile2=OPNNRD(sortfile->module,sortfile->filename);
		if(sortfile2!=(-1))
		{
			key=KEYNUM(payddsv,1);
			if(key!=NULL)
			{
				tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
					module,"Setting Check Numbers",NULL);
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
				virt_chk_no=start_check;
				ef=FRSNRD(sortfile->fileno,1);
				FINDFLDGETSTRING(sortfile->fileno,
					"VENDOR IDENTIFICATION",&vendorid);
				FINDFLDGETSTRING(sortfile->fileno,
					"ADDRESS IDENTIFICATION",&venaddrid);
				FINDFLDGETSTRING(sortfile->fileno,"BREAK IDENTIFICATION",&break_id);
				last_vendorid=stralloc(vendorid);
				last_venaddrid=stralloc(venaddrid);
				last_breakid=stralloc(break_id);
				ZERNRD(sortfile2);
				fields=FLDPOINTER(sortfile->fileno);
				numfields=NUMFLDS(sortfile->fileno);
				if(fields!=NULL)
				{
					for(x=0,field=fields;x<numfields;++x,++field)
					{
						COPYFIELD(sortfile->fileno,sortfile2,field->name);
					}
				}
				ven_count=voucher_count(mainrsrc,sortfile2,vendorid,venaddrid,break_id);
				FINDFLDGETCHAR(sortfile->fileno,"DEFAULT CHECK DETAIL",&default_check_detail);
				if(default_check_detail==TRUE)
				{
					check_number+=((ven_count+body_count-1)/body_count)-1;
				}
				vindx=0;
				LOCNRDFILE(banknum);
				while(!ef)
				{
					FINDFLDGETSTRING(sortfile->fileno,
						"VENDOR IDENTIFICATION",&vendorid);
					FINDFLDGETSTRING(sortfile->fileno,
						"ADDRESS IDENTIFICATION",&venaddrid);
					FINDFLDGETSTRING(sortfile->fileno,"BREAK IDENTIFICATION",&break_id);
					if(RDAstrcmp(last_vendorid,vendorid) || RDAstrcmp(last_venaddrid,venaddrid) || RDAstrcmp(last_breakid,break_id))
					{
						if(last_vendorid!=NULL) Rfree(last_vendorid);
						if(last_venaddrid!=NULL) Rfree(last_venaddrid);
						if(last_breakid!=NULL) Rfree(last_breakid);
						last_vendorid=stralloc(vendorid);
						last_venaddrid=stralloc(venaddrid);
						last_breakid=stralloc(break_id);
						ZERNRD(sortfile2);
						fields=FLDPOINTER(sortfile->fileno);
						numfields=NUMFLDS(sortfile->fileno);
						if(fields!=NULL)
						{
							for(x=0,field=fields;x<numfields;++x,++field)
							{
								COPYFIELD(sortfile->fileno,sortfile2,
									field->name);
							}
						}
						ven_count=voucher_count(mainrsrc,sortfile2,vendorid,venaddrid,break_id);
						FINDFLDGETCHAR(sortfile->fileno,"DEFAULT CHECK DETAIL",&default_check_detail);
						FINDFLDGETCHAR(sortfile->fileno,"SEPARATE CHECK",&sep);
						if(default_check_detail==TRUE || sep==TRUE)
						{
							if(default_check_detail==FALSE) check_number+=1;
							else check_number+=((ven_count+body_count-1)/body_count);
						} else {
							check_number++;
						}
						++virt_chk_no;
						vindx=1;
					} else {
						if(vindx!=0)
						{
							FINDFLDGETCHAR(sortfile->fileno,"DEFAULT CHECK DETAIL",&default_check_detail);
							FINDFLDGETCHAR(sortfile->fileno,"SEPARATE CHECK",&sep);
							if(vindx%body_count==0 && (default_check_detail==TRUE || sep==TRUE))
							{
								if(default_check_detail==TRUE) ++virt_chk_no;
								vindx=1;
							} else {
								++vindx;
							}
						} else {
							++vindx;
						}
					}
					for(x=0,part=key->part;x<key->numparts;++x,++part)
						COPYFIELD(sortfile->fileno,payddsv,part->name);
					COPYFIELD(sortfile->fileno,payddsv,"VENDOR IDENTIFICATION");
					COPYFIELD(sortfile->fileno,payddsv,"ADDRESS IDENTIFICATION");
					if(!ADVEQLNRDsec(payddsv,1,SCRNvirtualSubData,mainrsrc))
					{
						LOCNRD(payddsv);
						prev_file=RDATDataNEW(payddsv);
						FINDFLDSETINT(payddsv,"CHECK NUMBER",
							check_number);
						FINDFLDSETINT(payddsv,"VIRTUAL CHECK NUMBER",
							virt_chk_no);
						FINDFLDSETSTRING(payddsv,"CHECK DATE",
							check_date);
						ADVWRTTRANSsec(payddsv,0,NULL,prev_file,SCRNvirtualSubData,mainrsrc);
						DID_STUFF=TRUE;
						UNLNRD(payddsv);
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
				if(record_check_number==TRUE)
				{
					if(!ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(banknum);
						FINDFLDSETINT(banknum,"LAST CHECK NUMBER",check_number);
						ADVWRTTRANSsec(banknum,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
					}
/*					libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+15);
#ifdef WIN32
					sprintf(libx,"%s\\%s\\",CURRENTDIRECTORY,"BNKREC");
#endif
#ifndef WIN32
					sprintf(libx,"%s/%s/",CURRENTDIRECTORY,"BNKREC");
#endif
					sprintf(stemp,"%s.CHK",(bank_id!=NULL ? bank_id:""));
					temp1=adddashes(stemp);
					getnexttran(libx,temp1,check_number);
					if(temp1!=NULL) Rfree(temp1);
					if(libx!=NULL) Rfree(libx);
*/

				}
			}
			UNLNRDFILE(banknum);
			if(sortfile2!=(-1)) CLSNRD(sortfile2);
		} else {
			prterr("Error couldn't open sortfile [%s] [%s].",sortfile->module,sortfile->filename);
		}
	} else {
		prterr("Error no Vendor Vouchers Selected.");
	}
	if(vendorid!=NULL) Rfree(vendorid);
	if(last_vendorid!=NULL) Rfree(last_vendorid);
	if(venaddrid!=NULL) Rfree(venaddrid);
	if(last_venaddrid!=NULL) Rfree(last_venaddrid);
	if(break_id!=NULL) Rfree(break_id);
	if(last_breakid!=NULL) Rfree(last_breakid);
	if(dedid!=NULL) Rfree(dedid);
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
}
void check_warnings(RDArsrc *mainrsrc)
{
	char *temp1=NULL,*temp2=NULL;
	int check_year=0,cal_year=0;
	RDATData *prev=NULL;
	int orig_check=1;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"STARTING CHECK NUMBER",&start_check);
	if(record_check_number==TRUE)
	{
		if(!ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc))
		{
			prev=RDATDataNEW(banknum);
			FINDFLDGETINT(banknum,"LAST CHECK NUMBER",&orig_check);
			if(prev!=NULL) FreeRDATData(prev);
		}
		if((orig_check+1)!=start_check)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","LAST CHECK NUMBER CHANGED","During this process the BNKACC Last Check Number has changed. Please exit this process and re-access it so that the starting check number defaults correctly.",NULL,NULL,TRUE,NULL);
			return;
		}
	}
	if(!isEMPTY(check_date))
	{
		temp1=stralloc(check_date);
		check_year=atoi(temp1+6);
		temp2=GETCURRENTDATE10();
		cal_year=atoi(temp2+6);
		if(check_year!=cal_year)
		{
			ADVWARNINGDIALOG("WARNING DIALOG SCREEN","CHECK DATE'S YEAR","Warning: The year of the check date does not agree with the year of the system date. Are you sure you wish to continue?",setcheck,NULL,FALSE,1,mainrsrc,NULL,NULL);
			return;
		} else {
			setcheck(mainrsrc);
		}
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
	} else {
		setcheck(mainrsrc);
	}
}
static void bankidcb(RDArsrc *mainrsrc)
{
	char delflag=FALSE;
/*	char *libx=NULL;
	char *temp1=NULL;
*/
	int start_check=0;

	readwidget(mainrsrc,"[BNKACC][BANK IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",bank_id);
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
	if(record_check_number==TRUE)
	{
/*		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+15);
#ifdef WIN32
		sprintf(libx,"%s\\%s\\",CURRENTDIRECTORY,"BNKREC");
#endif
#ifndef WIN32
		sprintf(libx,"%s/%s/",CURRENTDIRECTORY,"BNKREC");
#endif
		sprintf(stemp,"%s.CHK",(bank_id!=NULL ? bank_id:""));
		temp1=adddashes(stemp);
		start_check=getnexttran(libx,temp1,(-1));
*/
		FINDRSCGETINT(mainrsrc,"[BNKACC][LAST CHECK NUMBER]",&start_check);
		++start_check;
		FINDRSCSETINT(mainrsrc,"STARTING CHECK NUMBER",start_check);
		updatersrc(mainrsrc,"STARTING CHECK NUMBER");

/*		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
*/
	}
	updateSCRNvirtuals(mainrsrc);
}
static void chkdate(RDArsrc *mainrsrc,NRDfield *field)
{
	readwidget(mainrsrc,"CHECK DATE");
	if(check_date!=NULL) Rfree(check_date);
	FINDRSCGETSTRING(mainrsrc,"CHECK DATE",&check_date);
	if(field!=NULL)
	{
		if(field->len==8)
		{
			if((RDAstrlen(check_date))<8)
			{
				FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
				FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
			} else {
				if(((atoi(check_date))<1) || ((atoi(check_date))>12) || ((atoi(check_date+3))<1) || ((atoi(check_date+3))>31))
				{
					FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
					FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
				} else {
					FINDRSCSETSENSITIVE(mainrsrc,"SELECT",TRUE);
					FINDRSCSETEDITABLE(mainrsrc,"SELECT",TRUE);
				}
			}
		} else {
			if((RDAstrlen(check_date))<10)
			{
				FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
				FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
			} else {
				if(((atoi(check_date))<1) || ((atoi(check_date))>12) || ((atoi(check_date+3))<1) || ((atoi(check_date+3))>31))
				{
					FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
					FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
				} else {
					FINDRSCSETSENSITIVE(mainrsrc,"SELECT",TRUE);
					FINDRSCSETEDITABLE(mainrsrc,"SELECT",TRUE);
				}
			}
		}
	} else if((RDAstrlen(check_date))<10)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
	} else {
		if(((atoi(check_date))<1) || ((atoi(check_date))>12) || ((atoi(check_date+3))<1) || ((atoi(check_date+3))>31))
		{
			FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
			FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
		} else {
			FINDRSCSETSENSITIVE(mainrsrc,"SELECT",TRUE);
			FINDRSCSETEDITABLE(mainrsrc,"SELECT",TRUE);
		}
	}
	updatersrc(mainrsrc,"SELECT");
}
static void payvschkResetGroupDefaults(RDArsrc *r)
{
	ResetGroupDefaults(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	NRDfield *field=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"SET VENDOR WITHHOLDING CHECK NUMBERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((baccnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((payddsv=APPOPNNRD(module,"PAYDDSV",TRUE,TRUE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDDMS",TRUE,TRUE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((venadr=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
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
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif

	gsv=RDAGenericSetupNew("PAYROLL","RECORD CHECK NUMBER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			record_check_number=*gsv->value.string_value;
		} else {
			record_check_number=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PAYROLL","V/W BANK IDENTIFICATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			bank_id=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	mainrsrc=RDArsrcNEW(module,"SET VENDOR WITHHOLDING CHECK NUMBERS");
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(mainrsrc,module,"PAYDDSV",NULL,payddsv);
	addDFincvir(mainrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(mainrsrc,"FINMGT","FINBACC",NULL,baccnum);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,banknum);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	if(vennum!=(-1)) file2rangersrc(vennum,mainrsrc);
	if(venadr!=(-1)) file2rangersrc(venadr,mainrsrc);
	if(payddsv!=(-1)) file2rangersrc(payddsv,mainrsrc);
	if(payddms!=(-1)) file2rangersrc(payddms,mainrsrc);
	if(baccnum!=(-1)) file2rangersrc(baccnum,mainrsrc);
	if(banknum!=(-1)) file2rsrc(banknum,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	FINDRSCSETFUNC(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",bankidcb,NULL);
	addstdrsrc(mainrsrc,"STARTING CHECK NUMBER",LONGV,8,&start_check,TRUE);
	addstdrsrc(mainrsrc,"BODY COUNT",SHORTV,3,&body_count,TRUE);
	field=FLDNRD(payddsv,"CHECK DATE");
	if(field!=NULL)
	{
		if(field->len==8) 
		{
			addstdrsrc(mainrsrc,"CHECK DATE",DATES,8,CURRENT_DATE,TRUE);
		} else addstdrsrc(mainrsrc,"CHECK DATE",DATES,10,CURRENT_DATE10,TRUE);
	} else addstdrsrc(mainrsrc,"CHECK DATE",DATES,8,CURRENT_DATE,TRUE);
	FINDRSCSETFUNC(mainrsrc,"CHECK DATE",chkdate,field);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	if(bank_id!=NULL)
	{
		FINDRSCSETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",bank_id);
		updatersrc(mainrsrc,"[BNKACC][BANK IDENTIFICATION]");
	}
	if(banknum!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
		FINDFLDSETSTRING(banknum,"BANK IDENTIFICATION",bank_id);
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(banknum,1);
		filerecord2rsrc(banknum,mainrsrc);
	}
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,payvschkResetGroupDefaults,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,check_warnings,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	chkdate(mainrsrc,field);
	bankidcb(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
