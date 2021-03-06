/* vensetr.c - Set Electronic Transfer Numbers */
#ifndef WIN32
#define __NAM__ "vensetr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "vensetr.exe"
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

char *module="VENPMT",*vch_expression=NULL,*bank_id=NULL;
short venyr=(-1),vennum=(-1),invnum=(-1),vchnum=(-1),body_count=10;
short eaccnum=(-1),raccnum=(-1),baccnum=(-1),banknum=(-1);
int start_check=1;
char record_check_number=FALSE;
static char *check_date=NULL,DID_STUFF=FALSE,use_approval=FALSE;
RDAvirtual *csortsequence=NULL;
char use_affect_date=FALSE;
short affect_date_style=0;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,vchnum);
	AddFileFields(list,invnum);
	AddFileFields(list,venyr);
	AddFileFields(list,vennum);
	addAPPlibNoDuplicates(list,"[PAGE NO]");
	addAPPlibNoDuplicates(list,"[BODY COUNT]");
	addAPPlibNoDuplicates(list,"[BREAK LEVEL]");
	addAPPlibNoDuplicates(list,"[PRIMARY FILE NUMBER]");
	addAPPlibNoDuplicates(list,"[WRITE COUNT]");
	addAPPlibNoDuplicates(list,"[SYSTEM DATE]");
	addAPPlibNoDuplicates(list,"[SYSTEM TIME]");
	addAPPlibNoDuplicates(list,"[LOGIN IDENTIFICATION]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION NAME]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ADDRESS #1]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ADDRESS #2]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION CITY]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION STATE]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ZIP]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION PHONE]");
	addAPPlibNoDuplicates(list,"[STARTING FISCAL MONTH]");
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
	if(eaccnum!=(-1)) CLSNRD(eaccnum);
	if(raccnum!=(-1)) CLSNRD(raccnum);
	if(baccnum!=(-1)) CLSNRD(baccnum);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(venyr!=(-1)) CLSNRD(venyr);
	if(invnum!=(-1)) CLSNRD(invnum);
	if(vchnum!=(-1)) CLSNRD(vchnum);
	if(banknum!=(-1)) CLSNRD(banknum);
	if(check_date!=NULL) Rfree(check_date);
	if(vch_expression!=NULL) Rfree(vch_expression);
	if(bank_id!=NULL) Rfree(bank_id);
	if(DID_STUFF)
	{
		args=APPlibNEW();
		addAPPlib(args,"VENPMT");
		addAPPlib(args,"VENEFTNEGATIVE");
		Execute2Program("doreport",args);
		if(args!=NULL) freeapplib(args);
		args=APPlibNEW();
		addAPPlib(args,"VENPMT");
		addAPPlib(args,"VENEFTREGISTER");
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
	short keyno=(-1);
	char *temp=NULL;

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("VENPMT","ELECTRONIC TRANSFER SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=stralloc(d->expression);
		} else csortsequence->expression=NULL;
		csortsequence->computed=FALSE;
		csortsequence->range=FALSE;
		csortsequence->dlen=0;
		if(!isEMPTY(csortsequence->expression))
		{
			temp=PP_translate(csortsequence->expression);
			Rfree(csortsequence->expression);	
			if(temp!=NULL)
			{
				csortsequence->expression=stralloc(temp);
				Rfree(temp);
			}
		}		
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
	key=KEYNUM(vchnum,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(vchnum,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
			addDBkeypart(dbsrt,keyno,part->name);
			if(!x)
			{
				field=FLDNRD(invnum,"ADDRESS IDENTIFICATION");
				if(field!=NULL)
				{
					addDBfield(dbsrt,"ADDRESS IDENTIFICATION",field->type,field->len);
					addDBkeypart(dbsrt,keyno,"ADDRESS IDENTIFICATION");
					addDBfield(dbsrt,"YYYY-MM-DD DUE DATE",1,10);
					addDBkeypart(dbsrt,keyno,"YYYY-MM-DD DUE DATE");
					addDBfield(dbsrt,"BREAK IDENTIFICATION",1,15);
					addDBkeypart(dbsrt,keyno,"BREAK IDENTIFICATION");
				}
			}
		}
	}
	addDBfield(dbsrt,"SEPARATE CHECK",BOOLNS,1);
	addDBfield(dbsrt,"DIRECT DRAFT",BOOLNS,1);
	addDBfield(dbsrt,"DUE DATE",16,10);
}
void addsortrecord(DBsort *dbsrt)
{
	char deleteflag=FALSE,sep=FALSE,*temp=NULL,ddraft=FALSE;
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
	key=KEYNUM(vchnum,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(vchnum,dbsrt->fileno,part->name);
		}
	}
	COPYFIELD(invnum,dbsrt->fileno,"ADDRESS IDENTIFICATION");
	COPYFIELD(invnum,dbsrt->fileno,"DUE DATE");
	COPYFIELD(vennum,dbsrt->fileno,"DIRECT DRAFT");
	FINDFLDGETCHAR(invnum,"SEPARATE CHECK",&sep);
	if(sep==TRUE)
	{
		FINDFLDGETCHAR(vennum,"DIRECT DRAFT",&ddraft);
		if(ddraft)
		{
			FINDFLDGETSTRING(invnum,"DUE DATE",&temp);
			memset(stemp,0,101);
			sprintf(stemp,"%.4s-%.2s-%.2s",&temp[7],&temp[0],&temp[3]);
			FINDFLDSETSTRING(dbsrt->fileno,"YYYY-MM-DD DUE DATE",stemp);
			if(temp!=NULL) Rfree(temp);
		}
		FIELDCOPY(invnum,"INVOICE IDENTIFICATION",dbsrt->fileno,"BREAK IDENTIFICATION");
	} else {
		FINDFLDSETSTRING(dbsrt->fileno,"BREAK IDENTIFICATION","");
	}
	COPYFIELD(invnum,dbsrt->fileno,"SEPARATE CHECK");
	WRTNRD(dbsrt->fileno,0,NULL);
}
int voucher_count(RDArsrc *mainrsrc,short fileno,char *vendorid,char *addid,char *breakid)
{
	int count=0;
	short ef;
	char *venid=NULL,*addrid=NULL,*bid=NULL;

	ef=ADVGTENRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(fileno,"VENDOR IDENTIFICATION",&venid);
		FINDFLDGETSTRING(fileno,"ADDRESS IDENTIFICATION",&addrid);
		FINDFLDGETSTRING(fileno,"BREAK IDENTIFICATION",&bid);
		if(!RDAstrcmp(venid,vendorid) && !RDAstrcmp(addid,addrid) && !RDAstrcmp(bid,breakid)) ++count;
			else break;
		ef=ADVNXTNRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	}
	if(venid!=NULL) Rfree(venid);
	if(addrid!=NULL) Rfree(addrid);
	if(bid!=NULL) Rfree(bid);
	return(count);
}
void setcheck(RDArsrc *mainrsrc)
{
	short accttype=0,ef=0,increment_diagnostic=FALSE,sortfile2=(-1);
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*vendorid=NULL,*last_vendorid=NULL;
	char *bankid1=NULL,*addrid=NULL,*last_addrid=NULL;
	char *breakid=NULL,*last_breakid=NULL;
	RDArsrc *tmprsrc=NULL;
	/* int selbankid=0; */
	int ven_count=0,x=0,numfields=0;
	int check_number=0,vindx=0,virt_chk_no=0;
	NRDpart *part;
	NRDkey *key=NULL;
	NRDfield *field=NULL,*fields=NULL;
	RDAvirtual *v;
	char manual_voucher=FALSE,deleteflag=FALSE,cleared=FALSE;
	char verified=FALSE,electronic=FALSE/*,*libx=NULL,*temp1=NULL*/;
	char void_check=FALSE;
	short approved=FALSE;
	char bnkupd=FALSE,directdraft=FALSE,distributed=FALSE;
	RDATData *prev=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&vch_expression);
	FINDRSCGETSHORT(mainrsrc,"BODY COUNT",&body_count);
	if(banknum!=(-1))
	{
	    /*
		if(BANKids->numlibs>0)
		{
			FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&selbankid);
			if(RDAstrcmp(BANKids->libs[selbankid],"No Banks Defined."))
			{
				bank_id=stralloc(BANKids->libs[selbankid]);
			}
		} 
		*/
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	}
	sortname=unique_name();
	sortfile=DBsortNEW("VENPMT",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(vchnum,rs);
	ReadRangeScreen(invnum,rs);
	ReadRangeScreen(vennum,rs);
	ReadRangeScreen(venyr,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(vchnum,"DIAGNOSTIC SCREEN",module,
		"Selecting Vendor Vouchers",NULL);
	if(tmprsrc!=NULL)
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	ef=ADVBEGNRDsec(vchnum,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(vchnum,"SOURCE USER VERIFIED",&verified);
		FINDFLDGETCHAR(vchnum,"CLEARED",&cleared);
		FINDFLDGETCHAR(vchnum,"BNKREC UPDATED",&bnkupd);
		FINDFLDGETCHAR(vchnum,"MANUAL VOUCHER",&manual_voucher);
		FINDFLDGETCHAR(vchnum,"ELECTRONIC TRANSFER",&electronic);
		FINDFLDGETCHAR(vchnum,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(vchnum,"DISTRIBUTED",&distributed);
		FINDFLDGETCHAR(vchnum,"VOID CHECK",&void_check);
		FINDFLDGETSHORT(vchnum,"APPROVED",&approved);
		if(verified && !cleared && !manual_voucher && electronic && !deleteflag && !bnkupd && !void_check && ((use_approval==FALSE) || (approved==1)) && ((distributed==FALSE || use_affect_date==FALSE || affect_date_style==0)))
		{
		if(banknum!=(-1))
		{
			if(bank_id!=NULL && RDAstrlen(bank_id)>0)
			{
				FINDFLDGETSTRING(vchnum,"BANK IDENTIFICATION",&bankid1);
			}
		}
		if(!RDAstrcmp(bank_id,bankid1))
		{
			FINDFLDGETSHORT(vchnum,"ACCOUNT TYPE",&accttype);
			switch(accttype)
			{
				case 0: /* Expenditures */
					COPYFIELD(vchnum,eaccnum,"DEFINITION TYPE");
					COPYFIELD(vchnum,eaccnum,"ACCOUNT CODE");
					if(ADVEQLNRDsec(eaccnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(eaccnum,1);
					ZERNRD(raccnum);
					ZERNRD(baccnum);
					break;
				case 1: /* Revenue */
					COPYFIELD(vchnum,raccnum,"DEFINITION TYPE");
					COPYFIELD(vchnum,raccnum,"ACCOUNT CODE");
					if(ADVEQLNRDsec(raccnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(raccnum,1);
					ZERNRD(eaccnum);
					ZERNRD(baccnum);
					break;
				case 2: /* Balance Sheets */
					COPYFIELD(vchnum,baccnum,"DEFINITION TYPE");
					FIELDCOPY(vchnum,"DEBIT CODE",baccnum,"ACCOUNT CODE");
					/* COPYFIELD(vchnum,baccnum,"ACCOUNT CODE"); */
					if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(baccnum,1);
					ZERNRD(eaccnum);
					ZERNRD(raccnum);
					break;
				default:
					ZERNRD(eaccnum);
					ZERNRD(baccnum);
					ZERNRD(raccnum);
					break;
			}
			COPYFIELD(vchnum,vennum,"VENDOR IDENTIFICATION");
			if(!ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(vennum,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					COPYFIELD(vchnum,venyr,"VENDOR IDENTIFICATION");
					COPYFIELD(vchnum,venyr,"CALENDAR YEAR");
					if(!ADVEQLNRDsec(venyr,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDGETCHAR(venyr,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							COPYFIELD(vchnum,invnum,"VENDOR IDENTIFICATION");
							COPYFIELD(vchnum,invnum,"CALENDAR YEAR");
							COPYFIELD(vchnum,invnum,
								"INVOICE IDENTIFICATION");
							if(!ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mainrsrc))
							{
								FINDFLDGETCHAR(invnum,"DELETEFLAG",
									&deleteflag);
								if(!deleteflag)
								{
									if(!testfilerangersrc(vchnum,rs) && 
										!testfilerangersrc(invnum,rs) &&
										!testfilerangersrc(vennum,rs) && 
										!testfilerangersrc(venyr,rs)) 
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
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		if(bankid1!=NULL) { Rfree(bankid1); bankid1=NULL; }
		ef=ADVSEQNRDsec(vchnum,SCRNvirtualSubData,mainrsrc);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(SIZNRD(sortfile->fileno))
	{
		sortfile2=OPNNRD(sortfile->module,sortfile->filename);
		if(sortfile2!=(-1))
		{
			key=KEYNUM(vchnum,1);
			if(key!=NULL)
			{
				tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
					module,"Setting Check Numbers",NULL);
				if(tmprsrc!=NULL)
					if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
				check_number=start_check;
				virt_chk_no=start_check;
				ef=FRSNRD(sortfile->fileno,1);
				FINDFLDGETSTRING(sortfile->fileno,
					"VENDOR IDENTIFICATION",&vendorid);
				FINDFLDGETSTRING(sortfile->fileno,
					"ADDRESS IDENTIFICATION",&addrid);
				FINDFLDGETSTRING(sortfile->fileno,"BREAK IDENTIFICATION",&breakid);
				last_vendorid=stralloc(vendorid);
				last_addrid=stralloc(addrid);
				last_breakid=stralloc(breakid);
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
				ven_count=voucher_count(mainrsrc,sortfile2,vendorid,addrid,breakid);
				check_number+=((ven_count+body_count-1)/body_count)-1;
				vindx=0;
				LOCNRDFILE(banknum);
				while(!ef)
				{
					FINDFLDGETSTRING(sortfile->fileno,
						"VENDOR IDENTIFICATION",&vendorid);
					FINDFLDGETSTRING(sortfile->fileno,
						"ADDRESS IDENTIFICATION",&addrid);
					FINDFLDGETSTRING(sortfile->fileno,"BREAK IDENTIFICATION",&breakid);
					if(RDAstrcmp(last_vendorid,vendorid) || 
						RDAstrcmp(last_addrid,addrid) || RDAstrcmp(last_breakid,breakid))
					{
						if(last_vendorid!=NULL) Rfree(last_vendorid);
						if(last_addrid!=NULL) Rfree(last_addrid);
						if(last_breakid!=NULL) Rfree(last_breakid);
						last_vendorid=stralloc(vendorid);
						last_addrid=stralloc(addrid);
						last_breakid=stralloc(breakid);
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
						ven_count=voucher_count(mainrsrc,sortfile2,vendorid,addrid,breakid);
						check_number+=((ven_count+body_count-1)/body_count);
						++virt_chk_no;
						vindx=1;
					} else {
						if(vindx!=0)
						{
							if(vindx%body_count==0)
							{
								++virt_chk_no;
								vindx=1;
							} else {
								++vindx;
							}
						} else {
							++vindx;
						}
					}
					for(x=0,part=key->part;x<key->numparts;++x,++part)
						COPYFIELD(sortfile->fileno,vchnum,part->name);
					LOCNRDFILE(vchnum);
					if(!ADVEQLNRDsec(vchnum,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(vchnum);
						FINDFLDSETCHAR(vchnum,"ELECTRONIC TRANSFER",TRUE);
						FINDFLDSETINT(vchnum,"ELECTRONIC TRANSFER NUMBER",check_number);
						FINDFLDSETINT(vchnum,"VIRTUAL ELECTRONIC TRANSFER NUMBER",virt_chk_no);
						FINDFLDGETCHAR(sortfile->fileno,"DIRECT DRAFT",&directdraft);
						if(directdraft==FALSE)
						{
							FINDFLDSETSTRING(vchnum,"ELECTRONIC TRANSFER DATE",check_date);
						} else {
							FIELDCOPY(sortfile->fileno,"DUE DATE",vchnum,"ELECTRONIC TRANSFER DATE");
						}
						FINDFLDSETSTRING(vchnum,"CHECK/ETR ASSIGNED DATE",CURRENT_DATE10);
						FINDFLDSETSTRING(vchnum,"CHECK/ETR ASSIGNED TIME",CURRENT_TIME);
/* ZERO OUT CHECK NUMBER */
						FINDFLDSETINT(vchnum,"CHECK NUMBER",0);
						FINDFLDSETINT(vchnum,"VIRTUAL CHECK NUMBER",0);
						FINDFLDSETSTRING(vchnum,"CHECK DATE","");
						ADVWRTTRANSsec(vchnum,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
						DID_STUFF=TRUE;
					}
					UNLNRDFILE(vchnum);
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
						FINDFLDSETINT(banknum,"LAST ELECTRONIC TRANSFER NUMBER",check_number);
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
					sprintf(stemp,"%s.EFT",(bank_id!=NULL ? bank_id:""));
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
		ERRORDIALOG("No Vendor Vouchers Selected","Error: No Vendor Vouchers were selected for this Bank Identification.",NULL,FALSE);
	}
	if(last_vendorid!=NULL) Rfree(last_vendorid);
	if(last_addrid!=NULL) Rfree(last_addrid);
	if(last_breakid!=NULL) Rfree(last_breakid);
	if(addrid!=NULL) Rfree(addrid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(bankid1!=NULL) Rfree(bankid1);
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
	int orig_check=1;
	RDATData *prev=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"ELECTRONIC TRANSFER DATE",&check_date);
	FINDRSCGETINT(mainrsrc,"STARTING ELECTRONIC TRANSFER NUMBER",&start_check);
	if(record_check_number==TRUE)
	{
		if(!ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc))
		{
			prev=RDATDataNEW(banknum);
			FINDFLDGETINT(banknum,"LAST ELECTRONIC TRANSFER NUMBER",&orig_check);
			if(prev!=NULL) FreeRDATData(prev);
		}
		if((orig_check+1)!=start_check)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","LAST ELECTRONIC TRANSFER NUMBER CHANGED","During this process the BNKACC Last Electronic Transfer Number has changed. Please exit this process and re-access it so that the starting electronic transfer number defaults correctly.",NULL,NULL,TRUE,NULL);
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
			ADVWARNINGDIALOG("WARNING DIALOG SCREEN","DD DATE'S YEAR","Warning: The year of the electronic transfer date does not agree with the year of the system date. Are you sure you wish to continue?",setcheck,NULL,FALSE,1,mainrsrc,NULL,NULL);
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
/*
static void getBANKids(RDArsrc *mainrsrc)
{

	if(banknum!=(-1))
	{
		if(BANKids!=NULL) { freeapplib(BANKids); BANKids=NULL; }
		BANKids=APPlibNEW();
		ZERNRD(banknum);
		BANKids=ADVMakeNRDlistAllSTRING(banknum,"BNKREC",
					"Creating Bank Identification List",1,
					"BANK IDENTIFICATION",
					SCRNvirtualSubData,mainrsrc);
		if(BANKids->numlibs<1) addAPPlib(BANKids,"No Banks Defined.");
		if(ADVEQLNRDsec(banknum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(banknum,1);
		filerecord2rsrc(banknum,mainrsrc);
	}
}
void changeBANKids(RDArsrc *mainrsrc)
{
	char *libx=NULL;
	char *temp1=NULL;
	int start_check=0,x=0;

	if(record_check_number==TRUE)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+15);
#ifdef WIN32
		sprintf(libx,"%s\\%s\\",CURRENTDIRECTORY,"BNKREC");
#endif
#ifndef WIN32
		sprintf(libx,"%s/%s/",CURRENTDIRECTORY,"BNKREC");
#endif
		FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&x);
		sprintf(stemp,"%s.EFT",BANKids->libs[x]);
		temp1=adddashes(stemp);
		start_check=getnexttran(libx,temp1,(-1));
		++start_check;
		FINDRSCSETINT(mainrsrc,"STARTING ELECTRONIC TRANSFER NUMBER",start_check);
		updatersrc(mainrsrc,"STARTING ELECTRONIC TRANSFER NUMBER");
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
	}
}
*/
static void bankidcb(RDArsrc *mainrsrc)
{
	char delflag=FALSE;
	/*char *libx=NULL;
	char *temp1=NULL;*/
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
		sprintf(stemp,"%s.EFT",(bank_id!=NULL ? bank_id:""));
		temp1=adddashes(stemp);
		start_check=getnexttran(libx,temp1,(-1));
*/
		FINDRSCGETINT(mainrsrc,"[BNKACC][LAST ELECTRONIC TRANSFER NUMBER]",&start_check);
		++start_check;
		FINDRSCSETINT(mainrsrc,"STARTING ELECTRONIC TRANSFER NUMBER",start_check);
		updatersrc(mainrsrc,"STARTING ELECTRONIC TRANSFER NUMBER");
/*
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
*/
	}
	updateSCRNvirtuals(mainrsrc);
}
static void etrdate(RDArsrc *mainrsrc,NRDfield *field)
{
	readwidget(mainrsrc,"ELECTRONIC TRANSFER DATE");
	if(check_date!=NULL) Rfree(check_date);
	FINDRSCGETSTRING(mainrsrc,"ELECTRONIC TRANSFER DATE",&check_date);
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
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	/* int which_type=0; */
	NRDfield *field=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	DFvirtual *df=NULL;

	if(InitializeSubsystems(argc,argv,module,"SET ELECTRONIC TRANSFER NUMBERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((eaccnum=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((raccnum=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
	if((baccnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((invnum=APPOPNNRD(module,"VENINV",TRUE,FALSE))==(-1)) return;
	if((venyr=APPOPNNRD(module,"VENYR",TRUE,FALSE))==(-1)) return;
	if((vchnum=APPOPNNRD(module,"VENVCH",TRUE,TRUE))==(-1)) return;
	if((vennum=APPOPNNRD(module,"VENMSR",TRUE,FALSE))==(-1)) return;
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
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("VENPMT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif

	gsv=RDAGenericSetupNew("VENPMT","AFFECT DATE STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			affect_date_style=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("VENPMT","RECORD EFT NUMBER");
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
	gsv=RDAGenericSetupNew("VENPMT","SWB_VP21_VOUCHER_APPROVAL");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_approval=*gsv->value.string_value;
		} else {
			use_approval=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("VENPMT","CLEARING BANK IDENTIFICATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			bank_id=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("VENPMT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
	gsv=RDAGenericSetupNew("FINMGT","SWB_FM25_AFFECT_DATE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_affect_date=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	mainrsrc=RDArsrcNEW(module,"SET ELECTRONIC TRANSFER NUMBERS");
	if(vennum!=(-1)) file2rangersrc(vennum,mainrsrc);
	if(venyr!=(-1)) file2rangersrc(venyr,mainrsrc);
	if(invnum!=(-1)) file2rangersrc(invnum,mainrsrc);
	if(vchnum!=(-1)) file2rangersrc(vchnum,mainrsrc);
	if(eaccnum!=(-1)) file2rangersrc(eaccnum,mainrsrc);
	if(raccnum!=(-1)) file2rangersrc(raccnum,mainrsrc);
	if(baccnum!=(-1)) file2rangersrc(baccnum,mainrsrc);
	if(banknum!=(-1)) file2rsrc(banknum,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	FINDRSCSETFUNC(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",bankidcb,NULL);
	addDFincvir(mainrsrc,module,"VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,module,"VENYR",NULL,venyr);
	addDFincvir(mainrsrc,module,"VENINV",NULL,invnum);
	addDFincvir(mainrsrc,module,"VENVCH",NULL,vchnum);
	addDFincvir(mainrsrc,"FINMGT","FINEACC",NULL,eaccnum);
	addDFincvir(mainrsrc,"FINMGT","FINRACC",NULL,raccnum);
	addDFincvir(mainrsrc,"FINMGT","FINBACC",NULL,baccnum);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,banknum);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	addstdrsrc(mainrsrc,"STARTING ELECTRONIC TRANSFER NUMBER",LONGV,8,&start_check,TRUE);
	/*
	if(BANKids!=NULL) { freeapplib(BANKids); BANKids=NULL; }
	BANKids=APPlibNEW();
	if(banknum==(-1))
	{
		addAPPlib(BANKids,"No BNKREC Module.");
	} else {
		addAPPlib(BANKids,"No Banks Defined.");
	}
	getBANKids(mainrsrc);
	addlstrsrc(mainrsrc,"BANK IDENTIFICATIONS",&which_type,TRUE,changeBANKids,BANKids->numlibs,&BANKids->libs,NULL);
	*/
	addstdrsrc(mainrsrc,"BODY COUNT",SHORTV,3,&body_count,TRUE);
	field=FLDNRD(vchnum,"ELECTRONIC TRANSFER DATE");
	if(field!=NULL)
	{
		if(field->len==8)
		{
			addstdrsrc(mainrsrc,"ELECTRONIC TRANSFER DATE",DATES,8,CURRENT_DATE,TRUE);
		} else addstdrsrc(mainrsrc,"ELECTRONIC TRANSFER DATE",DATES,10,CURRENT_DATE10,TRUE);
	} else addstdrsrc(mainrsrc,"ELECTRONIC TRANSFER DATE",DATES,10,CURRENT_DATE10,TRUE);
	FINDRSCSETFUNC(mainrsrc,"ELECTRONIC TRANSFER DATE",etrdate,field);
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
	/*
	for(which_type=0;which_type<BANKids->numlibs;++which_type)
	{
		if(!RDAstrcmp(BANKids->libs[which_type],bank_id)) break;
	}
	if(which_type>=BANKids->numlibs) which_type=0;
	FINDRSCSETINT(mainrsrc,"BANK IDENTIFICATIONS",which_type);
	*/
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,check_warnings,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	df=getDFvirtual("VENPMT","ACCOUNT TYPE");
	if(df!=NULL)
	{
		ADVaddSCRNvirtual(mainrsrc,df->name,df->type,df->length,df->expression,df->when,TRUE);
		FreeDFvirtual(df);
	}
	df=getDFvirtual("VENPMT","DIMENSION #0 CODE");
	if(df!=NULL)
	{
		ADVaddSCRNvirtual(mainrsrc,df->name,df->type,df->length,df->expression,df->when,TRUE);
		FreeDFvirtual(df);
	}
	computeallSCRNvirtuals(mainrsrc);
	/* changeBANKids(mainrsrc); */
	etrdate(mainrsrc,field);
	bankidcb(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
