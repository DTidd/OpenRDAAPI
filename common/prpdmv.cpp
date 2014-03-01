/* prpdmv.c - MBGUI Submit Batches to DMV */
#ifndef WIN32
#define __NAM__ "prpdmv.lnx"
#endif
#ifdef WIN32
#define __NAM__ "prpdmv.exe"
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

char *module="PROPERTY",*det_expression=NULL,*jurs_id=NULL,*juldate=NULL;
char *refund_formula=NULL,*start_date=NULL,*end_date=NULL;
char *credit_formula=NULL;
char credit_run=FALSE,test_run=FALSE,incstate=TRUE;
short prpmst=(-1),prpown=(-1),prpdet=(-1),start_seq=1;
int taxyear=1998,BATCH_COUNT=20000;
RDAvirtual *csortsequence=NULL;

static APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	loadglobals(list);
	AddFileFields(list,prpdet);
	AddFileFields(list,prpmst);
	AddFileFields(list,prpown);
	SORTAPPlib(list);
	return(list);
}
static void loadvalue(RDArsrc *r,char *fieldname)
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
static void quitfunc(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	if(prpown!=(-1)) CLSNRD(prpown);
	if(prpmst!=(-1)) CLSNRD(prpmst);
	if(prpdet!=(-1)) CLSNRD(prpdet);
	if(det_expression!=NULL) Rfree(det_expression);
	if(start_date!=NULL) Rfree(start_date);
	if(end_date!=NULL) Rfree(end_date);
	if(refund_formula!=NULL) Rfree(refund_formula);
	if(credit_formula!=NULL) Rfree(credit_formula);
	ShutdownSubsystems();
}
static void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("PROPERTY","DMV SORT SEQUENCE");
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
		} else csortsequence->expression=NULL;
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
	key=KEYNUM(prpdet,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(prpdet,part->name);
			if(field!=NULL) 
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
				addDBkeypart(dbsrt,keyno,field->name);
			}
		}
	}
	addDBfield(dbsrt,"VALUE",DOLLARS_NOCENTS,20);
	addDBfield(dbsrt,"TAX AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"TAX PAID",DOLLARS,20);
	addDBfield(dbsrt,"ABATEMENT",DOLLARS,20);
	addDBfield(dbsrt,"CREDIT AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"QUALIFIED",BOOLNS,1);
	addDBfield(dbsrt,"SUBMITTED REFUND",DOLLARS,20);
	addDBfield(dbsrt,"MAKE",PLAINTEXT,15);
	addDBfield(dbsrt,"MODEL",PLAINTEXT,15);
	addDBfield(dbsrt,"MODEL YEAR",LONGV,4);
	addDBfield(dbsrt,"REFERENCE ID",PLAINTEXT,15);
	addDBfield(dbsrt,"LEASED",BOOLNS,1);
	addDBfield(dbsrt,"SSN",15,11);
	addDBfield(dbsrt,"LAST NAME",1,25);
	addDBfield(dbsrt,"FIRST NAME",1,25);
	addDBfield(dbsrt,"MIDDLE INITIAL",1,15);
	addDBfield(dbsrt,"LINEAGE",1,10);
	addDBfield(dbsrt,"ADDRESS 1",1,40);
	addDBfield(dbsrt,"ADDRESS 2",1,40);
	addDBfield(dbsrt,"CITY",1,25);
	addDBfield(dbsrt,"STATE",1,2);
	addDBfield(dbsrt,"ZIP",5,10);
	addDBfield(dbsrt,"SSN 2",15,11);
	addDBfield(dbsrt,"SUBMITTED",BOOLNS,1);
	addDBfield(dbsrt,"ABATEMENT SUBMITTED",BOOLNS,1);
}
static void addsortrecord(DBsort *dbsrt,RDArsrc *mainrsrc)
{
	char deleteflag=FALSE,leased=FALSE;
	NRDkey *key=NULL;
	NRDpart *part;
	int x;
	double refnd=0.0,cred=0.0;

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
	key=KEYNUM(prpdet,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(prpdet,dbsrt->fileno,part->name);
		}
	}
	if(!isEMPTY(refund_formula))
	{
		refnd=EVALUATEdbl(refund_formula,SCRNvirtualSubData,mainrsrc);
	}
	FINDFLDSETDOUBLE(dbsrt->fileno,"SUBMITTED REFUND",refnd);
	if(credit_run)
	{
		if(!isEMPTY(credit_formula))
		{
			cred=EVALUATEdbl(credit_formula,SCRNvirtualSubData,mainrsrc);
		}
	} 
	FINDFLDSETDOUBLE(dbsrt->fileno,"CREDIT AMOUNT",cred);
	COPYFIELD(prpdet,dbsrt->fileno,"TAX AMOUNT");
	COPYFIELD(prpdet,dbsrt->fileno,"TAX PAID");
	COPYFIELD(prpdet,dbsrt->fileno,"ABATEMENT");
	COPYFIELD(prpdet,dbsrt->fileno,"VALUE");
	COPYFIELD(prpdet,dbsrt->fileno,"QUALIFIED");
	COPYFIELD(prpdet,dbsrt->fileno,"MAKE");
	COPYFIELD(prpdet,dbsrt->fileno,"MODEL");
	COPYFIELD(prpdet,dbsrt->fileno,"MODEL YEAR");
	COPYFIELD(prpdet,dbsrt->fileno,"REFERENCE ID");
	COPYFIELD(prpdet,dbsrt->fileno,"SUBMITTED");
	COPYFIELD(prpdet,dbsrt->fileno,"ABATEMENT SUBMITTED");

	FINDFLDGETCHAR(prpdet,"LEASED",&leased);
	FINDFLDSETCHAR(dbsrt->fileno,"LEASED",leased);
	if(leased)
	{
		FIELDCOPY(prpdet,"LEASED SSN",dbsrt->fileno,"SSN");
		FIELDCOPY(prpdet,"LEASED LAST NAME",dbsrt->fileno,"LAST NAME");
		FIELDCOPY(prpdet,"LEASED FIRST NAME",dbsrt->fileno,"FIRST NAME");
		FIELDCOPY(prpdet,"LEASED MIDDLE INITIAL",dbsrt->fileno,"MIDDLE INITIAL");
		FIELDCOPY(prpdet,"LEASED LINEAGE",dbsrt->fileno,"LINEAGE");
		FIELDCOPY(prpdet,"LEASED ADDRESS 1",dbsrt->fileno,"ADDRESS 1");
		FIELDCOPY(prpdet,"LEASED ADDRESS 2",dbsrt->fileno,"ADDRESS 2");
		FIELDCOPY(prpdet,"LEASED CITY",dbsrt->fileno,"CITY");
		FIELDCOPY(prpdet,"LEASED STATE",dbsrt->fileno,"STATE");
		FIELDCOPY(prpdet,"LEASED ZIP",dbsrt->fileno,"ZIP");
	} else {
		FIELDCOPY(prpmst,"SOCIAL SECURITY NUMBER",dbsrt->fileno,"SSN");
		COPYFIELD(prpmst,dbsrt->fileno,"LAST NAME");
		COPYFIELD(prpmst,dbsrt->fileno,"FIRST NAME");
		COPYFIELD(prpmst,dbsrt->fileno,"MIDDLE INITIAL");
		COPYFIELD(prpmst,dbsrt->fileno,"LINEAGE");
		COPYFIELD(prpmst,dbsrt->fileno,"ADDRESS 1");
		COPYFIELD(prpmst,dbsrt->fileno,"ADDRESS 2");
		COPYFIELD(prpmst,dbsrt->fileno,"CITY");
		COPYFIELD(prpmst,dbsrt->fileno,"STATE");
		COPYFIELD(prpmst,dbsrt->fileno,"ZIP");
	}
	FIELDCOPY(prpmst,"SOCIAL SECURITY NUMBER 2",dbsrt->fileno,"SSN 2");		
	WRTNRD(dbsrt->fileno,0,NULL);
}
static void GetHeaderInfo(short fileno,short fileno2,int *count,double *total,
	int *acount,double *atotal)
{
	NRDkey *key=NULL;
	NRDpart *part;
	short ef=0,x;
	double amt=0.0,amt1=0.0;

	*count=0;
	*total=0.0;
	*acount=0;
	*atotal=0.0;
	ZERNRD(fileno2);
	if(csortsequence!=NULL)
	{
		COPYFIELD(fileno,fileno2,csortsequence->name);
	}
	key=KEYNUM(fileno2,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(fileno,fileno2,part->name);
		}
	}
	ef=GTENRD(fileno2,1);
	while(!ef)
	{
		++*count;
		FINDFLDGETDOUBLE(fileno2,"SUBMITTED REFUND",&amt);
		*total+=amt;
		FINDFLDGETDOUBLE(fileno2,"CREDIT AMOUNT",&amt1);
		if(amt1!=0.0)
		{
			*atotal+=amt1;
			++*acount;
		}
		if(*count==BATCH_COUNT) break;
		ef=NXTNRD(fileno2,1);
	}
}
static void UpdatePRPDET(short fileno,RDArsrc *mainrsrc)
{
	NRDkey *key=NULL;
	NRDpart *part;
	RDATData *prev=NULL;
	short x;
	char blns=TRUE,*temp=NULL;
	char prevblns=FALSE,asub1=FALSE;
	double abatement=0.0;

	ZERNRD(prpdet);
	key=KEYNUM(fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(fileno,prpdet,part->name);
		}
	}
	FINDFLDGETCHAR(fileno,"SUBMITTED",&prevblns);
	FINDFLDGETCHAR(fileno,"ABATEMENT SUBMITTED",&asub1);
	FINDFLDGETDOUBLE(fileno,"ABATEMENT",&abatement);
	if(prevblns==FALSE || asub1==FALSE)
	{
		if(!ADVEQLNRDsec(prpdet,1,SCRNvirtualSubData,mainrsrc))
		{
			prev=RDATDataNEW(prpdet);
			if(!credit_run)
			{
				FINDFLDSETCHAR(prpdet,"SUBMITTED",blns);
				FINDFLDSETSTRING(prpdet,"SUBMITTED BY",USERLOGIN);
				temp=GETCURRENTDATE10();
				FINDFLDSETSTRING(prpdet,"SUBMITTED DATE",temp);
				if(temp!=NULL) Rfree(temp);
				temp=GETCURRENTTIME();
				FINDFLDSETSTRING(prpdet,"SUBMITTED TIME",temp);
				if(temp!=NULL) Rfree(temp);
				COPYFIELD(fileno,prpdet,"SUBMITTED REFUND");
			} else {	
				FINDFLDSETCHAR(prpdet,"ABATEMENT SUBMITTED",blns);
				FINDFLDSETSTRING(prpdet,"ABATEMENT SUBMITTED BY",USERLOGIN);
				temp=GETCURRENTDATE10();
				FINDFLDSETSTRING(prpdet,"ABATEMENT SUBMITTED DATE",temp);
				if(temp!=NULL) Rfree(temp);
				temp=GETCURRENTTIME();
				FINDFLDSETSTRING(prpdet,"ABATEMENT SUBMITTED TIME",temp);
				if(temp!=NULL) Rfree(temp);
			}
			ADVWRTTRANSsec(prpdet,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
			if(prev!=NULL) FreeRDATData(prev);
		} else {
			prterr("Error:  How did you get here?");
		}
	}
}
static void setcheck(RDArsrc *mainrsrc)
{
	int x=0,batch_count=0,tyear=0,abatch_count=0;
	short ef=0,increment_diagnostic=FALSE,keyno=0,key_range=FALSE;
	short sortfile2=(-1);
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*xend_date=NULL,*xstart_date=NULL;
	RDArsrc *tmprsrc=NULL;
	RDAvirtual *v=NULL;
	char deleteflag=FALSE,asub=FALSE;
/*
	char verified=FALSE;
	int x=0,batch_count=0,tyear=0,cred_count=0;
	double state_paid=0.0,cred_total=0.0;
*/
	char first_one=FALSE;
	char dobackend=FALSE;
	double taxpaid=0.0,taxamt=0.0,batch_total=0.0,rfund=0.0,value=0.0;
	double state_paid=0.0,abatement=0.0,abatch_total=0.0;
	double credamt=0.0;
	char *fname=NULL,*temp=NULL,*temp1=NULL,*ssn1=NULL;
	FILE *fp=NULL;
	char *lastname=NULL,*firstname=NULL,*addr1=NULL,*addr2=NULL,*city=NULL;
	char *state=NULL,*zip=NULL,*vin=NULL,mi=0;

	readallwidgets(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&det_expression);
	FINDRSCGETSTRING(mainrsrc,"REFUND FORMULA",&refund_formula);
	FINDRSCGETSTRING(mainrsrc,"CREDIT FORMULA",&credit_formula);
	FINDRSCGETINT(mainrsrc,"BATCH COUNT",&BATCH_COUNT);
	FINDRSCGETCHAR(mainrsrc,"TEST RUN",&test_run);
	FINDRSCGETCHAR(mainrsrc,"CREDIT RUN",&credit_run);
	FINDRSCGETCHAR(mainrsrc,"INCLUDE STATE",&incstate);
	FINDRSCGETINT(mainrsrc,"TAX YEAR",&taxyear);
	FINDRSCGETSTRING(mainrsrc,"JURISDICTION INDICATOR",&jurs_id);
	FINDRSCGETSTRING(mainrsrc,"JULIAN DATE",&juldate);
	FINDRSCGETSTRING(mainrsrc,"TAX START DATE",&start_date);
	FINDRSCGETSTRING(mainrsrc,"TAX END DATE",&end_date);
	FINDRSCGETSHORT(mainrsrc,"STARTING SEQUENCE",&start_seq);
	xstart_date=Rmalloc(11);
	if(!isEMPTY(start_date))
	{
		sprintf(xstart_date,"%.2s%.4s",&start_date[0],&start_date[6]);
	} else memset(xstart_date,0,7);
	xend_date=Rmalloc(7);
	if(!isEMPTY(end_date))
	{
		sprintf(xend_date,"%.2s%.4s",&end_date[0],&end_date[6]);
	} else  memset(xend_date,0,7);

	sortname=unique_name();
	sortfile=DBsortNEW("PROPERTY",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(prpdet,rs);
	ReadRangeScreen(prpmst,rs);
	ReadRangeScreen(prpown,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(prpdet,"DIAGNOSTIC SCREEN",module,
		"Selecting Vendor Vouchers",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	}
	ZERNRD(prpdet);
	keyno=KEYNUMBER(prpdet,"PRPDET KEY 3");
	if(keyno!=(-1))
	{
		key_range=setfilekeyFROMrangersrc(prpdet,keyno,rs);
		if(key_range==TRUE)
		{
			ef=ADVGTENRDsec(prpdet,keyno,SCRNvirtualSubData,mainrsrc);
		} else {
			ef=ADVBEGNRDsec(prpdet,SCRNvirtualSubData,mainrsrc);
		}
	} else {
		key_range=FALSE;
		ef=ADVBEGNRDsec(prpdet,SCRNvirtualSubData,mainrsrc);
	}
	while(!ef)
	{
		increment_diagnostic=FALSE;
		if(diagapps)
		{
			prterr("DIAG APPLICATION: Showing Current Record ....");TRACE;
			SEENRDRECORD(prpdet);TRACE;
		}
		FINDFLDGETCHAR(prpdet,"DELETEFLAG",&deleteflag);
		if(deleteflag==FALSE)
		{
			FINDFLDGETDOUBLE(prpdet,"TAX AMOUNT",&taxamt);
			FINDFLDGETDOUBLE(prpdet,"TAX PAID",&taxpaid);
			FINDFLDGETDOUBLE(prpdet,"STATE TO PAY",&state_paid);
			FINDFLDGETDOUBLE(prpdet,"ABATEMENT",&abatement);
			FINDFLDGETCHAR(prpdet,"ABATEMENT SUBMITTED",&asub);
/*
			FINDFLDGETCHAR(prpdet,"SUBMITTED",&verified);
*/
			FINDFLDGETINT(prpdet,"TAX YEAR",&tyear);
/*
			if(verified==FALSE && (((incstate==FALSE) && (taxpaid==taxamt)) || ((incstate==TRUE) && ((taxpaid+state_paid)==taxamt))) && taxamt!=0.0 && taxyear==tyear)
*/
			if(taxyear==tyear)
			{
			if(taxamt!=0.0)
			{
			if(((credit_run==FALSE) && (((incstate==FALSE) && (taxpaid==taxamt)) || ((incstate==TRUE) && ((taxpaid+state_paid)==taxamt)))) || ((credit_run==TRUE) && (asub==FALSE) && (abatement!=0)))
			{
				COPYFIELD(prpdet,prpown,"OWNER IDENTIFICATION");
				if(!ADVEQLNRDsec(prpown,1,SCRNvirtualSubData,mainrsrc))
				{
					FINDFLDGETCHAR(prpown,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
						COPYFIELD(prpdet,prpmst,"OWNER IDENTIFICATION");
						COPYFIELD(prpdet,prpmst,"TAX YEAR");
						COPYFIELD(prpdet,prpmst,"CLASS IDENTIFICATION");
						COPYFIELD(prpdet,prpmst,"DISTRICT IDENTIFICATION");
						if(!ADVEQLNRDsec(prpmst,1,SCRNvirtualSubData,mainrsrc))
						{
							FINDFLDGETCHAR(prpmst,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
								if(!testfilerangersrc(prpdet,rs) && 
									!testfilerangersrc(prpown,rs) &&
									!testfilerangersrc(prpmst,rs))
								{
									for(x=0,v=mainrsrc->virflds;
										x<mainrsrc->num;++x,++v)
									{
											if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mainrsrc)) 
											{
												break;
											}
									}
									if(x>=mainrsrc->num)
									{
										if(det_expression!=NULL)
										{
											if(EVALUATEbol(det_expression,SCRNvirtualSubData,mainrsrc))
											{
												if(csortsequence!=NULL)
												{
													computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
												}
												addsortrecord(sortfile,mainrsrc);
												dobackend=TRUE;
												increment_diagnostic=TRUE;
											}
										} else {
											if(csortsequence!=NULL)
											{
												computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
											}
											addsortrecord(sortfile,mainrsrc);
											dobackend=TRUE;
											increment_diagnostic=TRUE;
										}
									}	
								}
							} else {
								if(diagapps)
								{
									prterr("DIAG Deleted PRPMST..");TRACE;
								}
							}
						} else {
							if(diagapps)
							{
								prterr("DIAG No Matching PRPMST.");TRACE;
							}
						}
					} else {
						if(diagapps)
						{
							prterr("DIAG Deleted PRPOWN..");TRACE;
						}
					}
				} else {
					if(diagapps)
					{
						prterr("DIAG No Matching PRPOWN.");TRACE;
					}
				}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: PRPDMV ((incstate:[%s]==FALSE)  && taxpaid:[%10.02f]==taxamt:[%10.02f]))  && ((taxpaid:[%10.02f]+state_paid:[%10.02f])==taxamt:[%10.02f]))....",(incstate==TRUE?"TRUE":"FALSE"),(taxpaid/100),(taxamt/100),(taxpaid/100),(state_paid/100),(taxamt/100));TRACE;
				}
			}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: PRPDMV calcualted Tax Amount=0.....");TRACE;
				}
			}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: PRPDMV Tax Year not equal to Selected Tax Year ....");TRACE;
				}
			}
		} else {
			if(diagapps)
			{
				prterr("DIAG Deleted PRPDET.");TRACE
			}
		}	
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		if(key_range==TRUE && keyno!=(-1))
		{
			ef=ADVNXTNRDsec(prpdet,keyno,SCRNvirtualSubData,mainrsrc);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(prpdet,keyno,rs);
			}
		} else {	
			ef=ADVSEQNRDsec(prpdet,SCRNvirtualSubData,mainrsrc);
		}
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(dobackend==TRUE)
	{
		ZERNRD(sortfile->fileno);
		first_one=TRUE;
		sortfile2=OPNNRD(sortfile->module,sortfile->filename);
		if(sortfile2!=(-1))
		{
			while(TRUE)
			{
				GetHeaderInfo(sortfile->fileno,sortfile2,&batch_count,&batch_total,&abatch_count,&abatch_total);	
				if(batch_count)
				{
					fname=Rmalloc(RDAstrlen(jurs_id)+RDAstrlen(juldate)+12);
#ifndef WIN32
					sprintf(fname,"T%s%s.%03d",(jurs_id!=NULL ? jurs_id:""),
						(juldate!=NULL ? juldate:""),start_seq);		
#endif
#ifdef WIN32
					sprintf(fname,"T%s%s.%03d",(jurs_id!=NULL ? jurs_id:""),
						(juldate!=NULL ? juldate:""),start_seq);		
#endif
					fp=fopen(fname,"w+");
					if(fp!=NULL)
					{
/* header */
						temp1=Rmalloc(9);
						sprintf(temp1,"%.2s%.2s%.4s",&CURRENT_DATE10[0],&CURRENT_DATE10[3],
							&CURRENT_DATE10[6]);
						fprintf(fp,"H@%d@%s@%d@%8s@%d@%.02f@%d@%.02f\r\n",
							start_seq,jurs_id,taxyear,temp1,batch_count,batch_total/100,abatch_count,abatch_total/100);
							
						for(x=0;x<batch_count;++x)
						{
							if(first_one==TRUE)
							{
								first_one=FALSE;
								ef=FRSNRD(sortfile->fileno,1);
							} else ef=NXTNRD(sortfile->fileno,1);
							if(!ef)
							{
								FINDFLDGETSTRING(sortfile->fileno,"LAST NAME",&lastname);
								FINDFLDGETSTRING(sortfile->fileno,"FIRST NAME",&firstname);
								FINDFLDGETSTRING(sortfile->fileno,"MIDDLE INITIAL",&temp);
								if(!isEMPTY(temp)) mi=temp[0];
									else mi=' ';
								FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 1",&addr1);
								FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 2",&addr2);
								FINDFLDGETSTRING(sortfile->fileno,"CITY",&city);
								FINDFLDGETSTRING(sortfile->fileno,"STATE",&state);
								FINDFLDGETSTRING(sortfile->fileno,"ZIP",&temp);
								if(RDAstrstr(temp,"-"))
								{
									zip=Rmalloc(11);
									sprintf(zip,"%.5s%.4s",&temp[0],&temp[6]);
								} else {
									zip=stralloc(temp);
								}
								if(temp!=NULL) Rfree(temp);
								FINDFLDGETDOUBLE(sortfile->fileno,"SUBMITTED REFUND",&rfund);
								FINDFLDGETDOUBLE(sortfile->fileno,"VALUE",&value);
								FINDFLDGETDOUBLE(sortfile->fileno,"TAX AMOUNT",&taxamt);
								FINDFLDGETDOUBLE(sortfile->fileno,"TAX PAID",&taxpaid);
								FINDFLDGETDOUBLE(sortfile->fileno,"ABATEMENT",&abatement);
								FINDFLDGETDOUBLE(sortfile->fileno,"CREDIT AMOUNT",&credamt);
								FINDFLDGETSTRING(sortfile->fileno,"PROPERTY IDENTIFICATION",&vin);
								if(RDAstrlen(vin)>18) vin[18]=0;
								FINDFLDGETSTRING(sortfile->fileno,"SSN",&temp);
								ssn1=Rmalloc(10);
								if(!isEMPTY(temp))
								{
									sprintf(ssn1,"%.3s%.2s%.4s",&temp[0],&temp[4],&temp[7]);
								} else memset(ssn1,0,10);
								if(temp!=NULL) Rfree(temp);
/* detail */
								if(RDAstrlen(addr1)>32) addr1[32]=0;
								if(RDAstrlen(addr2)>32) addr2[32]=0;
								if(credit_run==FALSE)
								{
								fprintf(fp,"D@%d@%s@%s@%s@%c@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%.0f@%.02f@%.02f@%s@%s@%s@%s@%s\r\n",
									x,(ssn1!=NULL ? ssn1:""),(lastname!=NULL ? lastname:""),
									(firstname!=NULL ? firstname:""),mi,"","","","",
									(addr1!=NULL ? addr1:""),(addr2!=NULL ? addr2:""),(city!=NULL ? city:""),
									(state!=NULL ? state:""),(zip!=NULL ? zip:""),(vin!=NULL ? vin:""),
									value,taxamt/100,rfund/100,(xstart_date!=NULL ? xstart_date:""),(xend_date!=NULL ? xend_date:""),jurs_id,temp1,"");
								} else {
								fprintf(fp,"D@%d@%s@%s@%s@%c@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%.0f@%.02f@%.02f@%s@%s@%s@%s@%s\r\n",
									x,(ssn1!=NULL ? ssn1:""),(lastname!=NULL ? lastname:""),
									(firstname!=NULL ? firstname:""),mi,"","","","",
									(addr1!=NULL ? addr1:""),(addr2!=NULL ? addr2:""),(city!=NULL ? city:""),
									(state!=NULL ? state:""),(zip!=NULL ? zip:""),(vin!=NULL ? vin:""),
									value,abatement/100,credamt/100,(xstart_date!=NULL ? xstart_date:""),(xend_date!=NULL ? xend_date:""),jurs_id,temp1,"C");
								}
								if(!test_run) UpdatePRPDET(sortfile->fileno,mainrsrc);
								if(lastname!=NULL) Rfree(lastname);
								if(firstname!=NULL) Rfree(firstname);
								if(addr1!=NULL) Rfree(addr1);
								if(addr2!=NULL) Rfree(addr2);
								if(city!=NULL) Rfree(city);
								if(state!=NULL) Rfree(state);
								if(zip!=NULL) Rfree(zip);
								if(vin!=NULL) Rfree(vin);
								if(ssn1!=NULL) Rfree(ssn1);
							}
						}
						fclose(fp);
					}
					if(fname!=NULL) Rfree(fname);
					if(temp1!=NULL) Rfree(temp1);
					++start_seq;
				}
				if(batch_count<BATCH_COUNT) break;
			}
			CLSNRD(sortfile2);
		} else {
			prterr("Error couldn't open second occurrance of sort file.");
		}
	} else {
		ERRORDIALOG("NO DETAILS SELECTED!","Error no PRPDET Detail DMV Records Selected.",NULL,FALSE);
		prterr("Error no PRPDET Detail DMV Records Selected.");
	}
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
	if(xend_date!=NULL) Rfree(xend_date);
	if(xstart_date!=NULL) Rfree(xstart_date);
}
static void SetCredit(RDArsrc *r)
{
	char c=FALSE;

	readwidget(r,"CREDIT RUN");
	FINDRSCGETCHAR(r,"CREDIT RUN",&c);
	if(c==TRUE)
	{
		FINDRSCSETEDITABLE(r,"CREDIT FORMULA",TRUE);
		FINDRSCSETSENSITIVE(r,"CREDIT FORMULA",TRUE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD VALUE",TRUE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD GROUPER",TRUE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD CONTROL",TRUE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD CONTROL",TRUE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD OPERATOR",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"CREDIT FORMULA",FALSE);
		FINDRSCSETSENSITIVE(r,"CREDIT FORMULA",FALSE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD VALUE",FALSE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD GROUPER",FALSE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD CONTROL",FALSE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD CONTROL",FALSE);
		FINDRSCSETEDITABLE(r,"CREDIT LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(r,"CREDIT LOAD OPERATOR",FALSE);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"SUBMIT TO DMV")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((prpdet=APPOPNNRD(module,"PRPDET",TRUE,TRUE))==(-1)) return;
	if((prpown=APPOPNNRD(module,"PRPOWN",TRUE,FALSE))==(-1)) return;
	if((prpmst=APPOPNNRD(module,"PRPMST",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"SUBMIT TO DMV");
	addDFincvir(mainrsrc,module,"PRPOWN",NULL,prpown);
	addDFincvir(mainrsrc,module,"PRPDET",NULL,prpdet);
	addDFincvir(mainrsrc,module,"PRPMST",NULL,prpmst);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	if(prpown!=(-1)) file2rangersrc(prpown,mainrsrc);
	if(prpdet!=(-1)) file2rangersrc(prpdet,mainrsrc);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"REFUND FORMULA",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"CREDIT FORMULA",0,NULL,TRUE);
	addstdrsrc(mainrsrc,"JURISDICTION INDICATOR",PLAINTEXT,4,&jurs_id,TRUE);
	addstdrsrc(mainrsrc,"TEST RUN",BOOLNS,1,&test_run,TRUE);
	addstdrsrc(mainrsrc,"CREDIT RUN",BOOLNS,1,&credit_run,TRUE);
	FINDRSCSETFUNC(mainrsrc,"CREDIT RUN",SetCredit,NULL);
	addstdrsrc(mainrsrc,"INCLUDE STATE",BOOLNS,1,&incstate,TRUE);
	addstdrsrc(mainrsrc,"JULIAN DATE",PLAINTEXT,3,&juldate,TRUE);
	addstdrsrc(mainrsrc,"STARTING SEQUENCE",SHORTV,3,&start_seq,TRUE);
	addstdrsrc(mainrsrc,"BATCH COUNT",LONGV,6,&BATCH_COUNT,TRUE);
	addstdrsrc(mainrsrc,"TAX YEAR",LONGV,4,&taxyear,TRUE);
	addstdrsrc(mainrsrc,"TAX START DATE",DATES,10,&start_date,TRUE);
	addstdrsrc(mainrsrc,"TAX END DATE",DATES,10,&end_date,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"REFUND LOAD VALUE",TRUE,loadvalue,"REFUND FORMULA");
	addbtnrsrc(mainrsrc,"REFUND LOAD OPERATOR",TRUE,loadoperand,"REFUND FORMULA");
	addbtnrsrc(mainrsrc,"REFUND LOAD GROUPER",TRUE,loadgrouper,"REFUND FORMULA");
	addbtnrsrc(mainrsrc,"REFUND LOAD CONTROL",TRUE,loadcontrol,"REFUND FORMULA");
	addbtnrsrc(mainrsrc,"CREDIT LOAD VALUE",TRUE,loadvalue,"CREDIT FORMULA");
	addbtnrsrc(mainrsrc,"CREDIT LOAD OPERATOR",TRUE,loadoperand,"CREDIT FORMULA");
	addbtnrsrc(mainrsrc,"CREDIT LOAD GROUPER",TRUE,loadgrouper,"CREDIT FORMULA");
	addbtnrsrc(mainrsrc,"CREDIT LOAD CONTROL",TRUE,loadcontrol,"CREDIT FORMULA");
	SetCredit(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,setcheck,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
