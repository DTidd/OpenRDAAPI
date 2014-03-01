/* venschk.c - Set Check Numbers */
#ifndef WIN32
#define __NAM__ "venschk_old.lnx"
#endif
#ifdef WIN32
#define __NAM__ "venschk_old.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <nrd.h>
#include <mix.h>
#include <ldval.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

APPlib *BANKids=NULL;
char *module="VENPMT",*vch_expression=NULL,*bank_id=NULL;
char record_check_number=FALSE;
short venyr=(-1),vennum=(-1),invnum=(-1),vchnum=(-1),body_count=10;
short eaccnum=(-1),raccnum=(-1),baccnum=(-1),banknum=(-1);
int start_check=1;
char *check_date=NULL,DID_STUFF=FALSE;
RDAvirtual *csortsequence=NULL;
static void getBANKids(RDArsrc *),changeBANKids(RDArsrc *);

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
	addAPPlibNoDuplicates(list,"[SYSTEM DATE-YYYY]");
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
	if(DID_STUFF)
	{
		args=APPlibNEW();
		addAPPlib(args,"VENPMT");
		addAPPlib(args,"VENCKNEGATIVE");
		ADVExecuteProgram("doreport",args,NULL);
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
	d=getDFvirtual("VENPMT","CHECK SORT SEQUENCE");
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
				}
			}
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
	key=KEYNUM(vchnum,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(vchnum,dbsrt->fileno,part->name);
		}
	}
	COPYFIELD(invnum,dbsrt->fileno,"ADDRESS IDENTIFICATION");
	WRTNRD(dbsrt->fileno,0,NULL);
}
int voucher_count(RDArsrc *mainrsrc,short fileno,char *vendorid,char *addid)
{
	int count=0;
	short ef;
	char *venid=NULL,*addrid=NULL;

	ef=ADVGTENRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(fileno,"VENDOR IDENTIFICATION",&venid);
		FINDFLDGETSTRING(fileno,"ADDRESS IDENTIFICATION",&addrid);
		if(!RDAstrcmp(venid,vendorid) && !RDAstrcmp(addid,addrid)) ++count;
			else break;
		ef=ADVNXTNRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	}
	if(venid!=NULL) Rfree(venid);
	if(addrid!=NULL) Rfree(addrid);
	return(count);
}
void setcheck(RDArsrc *mainrsrc)
{
	short accttype=0,ef=0,increment_diagnostic=FALSE,sortfile2=(-1);
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*vendorid=NULL,*last_vendorid=NULL;
	char *bankid1=NULL,*addrid=NULL,*last_addrid=NULL;
	char *temp1=NULL;
	RDArsrc *tmprsrc=NULL;
	int selbankid=0,ven_count=0,x=0,numfields=0;
	int check_number=0,vindx=0,virt_chk_no=0;
	NRDpart *part;
	NRDkey *key=NULL;
	NRDfield *field=NULL,*fields=NULL,*f=NULL;
	RDAvirtual *v;
	char manual_voucher=FALSE,deleteflag=FALSE,cleared=FALSE;
	char verified=FALSE,electronic=FALSE,*libx=NULL;
	RDATData *prev=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&vch_expression);
	FINDRSCGETSHORT(mainrsrc,"BODY COUNT",&body_count);
	FINDRSCGETSTRING(mainrsrc,"CHECK DATE",&check_date);
	FINDRSCGETINT(mainrsrc,"STARTING CHECK NUMBER",&start_check);
	if(banknum!=(-1))
	{
		if(BANKids->numlibs>0)
		{
			FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&selbankid);
			if(RDAstrcmp(BANKids->libs[selbankid],"No Banks Defined."))
			{
				bank_id=stralloc(BANKids->libs[selbankid]);
			}
		} 
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
	if(mainrsrc->virtual!=NULL)
	{
		for(x=0,v=mainrsrc->virtual;x<mainrsrc->num;++x,++v)
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
		FINDFLDGETCHAR(vchnum,"MANUAL VOUCHER",&manual_voucher);
		FINDFLDGETCHAR(vchnum,"ELECTRONIC TRANSFER",&electronic);
		FINDFLDGETCHAR(vchnum,"DELETEFLAG",&deleteflag);
		if(verified && !cleared && !manual_voucher && !electronic && !deleteflag)
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
					COPYFIELD(vchnum,baccnum,"ACCOUNT CODE");
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
										for(x=0,v=mainrsrc->virtual;
											x<mainrsrc->num;++x,++v)
										{
											if(ADVtestvirtualrangersrc(v,rs,(void (*)())SCRNvirtualSubData,mainrsrc)) 
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
				last_vendorid=stralloc(vendorid);
				last_addrid=stralloc(addrid);
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
				ven_count=voucher_count(mainrsrc,sortfile2,vendorid,addrid);
				check_number+=((ven_count+body_count-1)/body_count)-1;
				vindx=0;
				while(!ef)
				{
					FINDFLDGETSTRING(sortfile->fileno,
						"VENDOR IDENTIFICATION",&vendorid);
					FINDFLDGETSTRING(sortfile->fileno,
						"ADDRESS IDENTIFICATION",&addrid);
					if(RDAstrcmp(last_vendorid,vendorid) || 
						RDAstrcmp(last_addrid,addrid))
					{
						if(last_vendorid!=NULL) Rfree(last_vendorid);
						if(last_addrid!=NULL) Rfree(last_addrid);
						last_vendorid=stralloc(vendorid);
						last_addrid=stralloc(addrid);
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
						ven_count=voucher_count(mainrsrc,sortfile2,vendorid,addrid);
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
						FINDFLDSETINT(vchnum,"CHECK NUMBER",check_number);
						FINDFLDSETINT(vchnum,"VIRTUAL CHECK NUMBER",virt_chk_no);
						FINDFLDSETSTRING(vchnum,"CHECK DATE",check_date);
/* ZERO ELECTRONIC TRANSFER NUMBER */
						f=FLDNRD(vchnum,"ELETRONIC TRANSFER DATE");
						if(f!=NULL)
						{
							/* has electronic transfer fields */
							FINDFLDSETCHAR(vchnum,"ELECTRONIC TRANSFER",FALSE);
							FINDFLDSETINT(vchnum,"ELECTRONIC TRANSFER NUMBER",0);
							FINDFLDSETINT(vchnum,"VIRTUAL ELECTRONIC TRANSFER NUMBER",0);
							FINDFLDSETSTRING(vchnum,"ELECTRONIC TRANSFER DATE","");
						}
						ADVWRTTRANSsec(vchnum,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						DID_STUFF=TRUE;
						if(prev!=NULL) FreeRDATData(prev);
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
				libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+15);
#ifdef WIN32
				sprintf(libx,"%s\\%s\\",CURRENTDIRECTORY,"VENPMT");
#endif
#ifndef WIN32
				sprintf(libx,"%s/%s/",CURRENTDIRECTORY,"VENPMT");
#endif
				sprintf(stemp,"%s.CHK",(bank_id!=NULL ? bank_id:""));
				temp1=adddashes(stemp);
				getnexttran(libx,temp1,check_number);
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				}
			}
			if(sortfile2!=(-1)) CLSNRD(sortfile2);
		} else {
			prterr("Error couldn't open sortfile [%s] [%s].",sortfile->module,sortfile->filename);
		}
	} else {
		prterr("Error no Vendor Vouchers Selected.");
	}
	if(last_vendorid!=NULL) Rfree(last_vendorid);
	if(last_addrid!=NULL) Rfree(last_addrid);
	if(addrid!=NULL) Rfree(addrid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(bankid1!=NULL) Rfree(bankid1);
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
}
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
		sprintf(libx,"%s\\%s\\",CURRENTDIRECTORY,"VENPMT");
#endif
#ifndef WIN32
		sprintf(libx,"%s/%s/",CURRENTDIRECTORY,"VENPMT");
#endif
		FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&x);
		sprintf(stemp,"%s.CHK",BANKids->libs[x]);
		temp1=adddashes(stemp);
		start_check=getnexttran(libx,temp1,(-1));
		++start_check;
		FINDRSCSETINT(mainrsrc,"STARTING CHECK NUMBER",start_check);
		updatersrc(mainrsrc,"STARTING CHECK NUMBER");
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
	}
}
void main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	int which_type=0,x=0;
	NRDfield *field=NULL;
	char *libx=NULL,*temp1=NULL;
	RDAGenericSetup *gsv=NULL;

	if(InitializeSubsystems(argc,argv,module,"SET CHECK NUMBERS")) 
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

	gsv=RDAGenericSetupNew("VENPMT","RECORD CHECK NUMBER");
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
	if(libx!=NULL) Rfree(libx);
	mainrsrc=RDArsrcNEW(module,"SET CHECK NUMBERS");
	if(vennum!=(-1)) file2rangersrc(vennum,mainrsrc);
	if(venyr!=(-1)) file2rangersrc(venyr,mainrsrc);
	if(invnum!=(-1)) file2rangersrc(invnum,mainrsrc);
	if(vchnum!=(-1)) file2rangersrc(vchnum,mainrsrc);
	if(eaccnum!=(-1)) file2rangersrc(eaccnum,mainrsrc);
	if(raccnum!=(-1)) file2rangersrc(raccnum,mainrsrc);
	if(baccnum!=(-1)) file2rangersrc(baccnum,mainrsrc);
	addDFincvir(mainrsrc,module,"VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,module,"VENYR",NULL,venyr);
	addDFincvir(mainrsrc,module,"VENINV",NULL,invnum);
	addDFincvir(mainrsrc,module,"VENVCH",NULL,vchnum);
	addDFincvir(mainrsrc,"FINMGT","FINEACC",NULL,eaccnum);
	addDFincvir(mainrsrc,"FINMGT","FINRACC",NULL,raccnum);
	addDFincvir(mainrsrc,"FINMGT","FINBACC",NULL,baccnum);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,banknum);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	addstdrsrc(mainrsrc,"STARTING CHECK NUMBER",LONGV,8,&start_check,TRUE);
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
	addstdrsrc(mainrsrc,"BODY COUNT",SHORTV,3,&body_count,TRUE);
	field=FLDNRD(vchnum,"CHECK DATE");
	if(field!=NULL)
	{
		if(field->len==8)
		{
			addstdrsrc(mainrsrc,"CHECK DATE",DATES,8,CURRENT_DATE,TRUE);
		} else addstdrsrc(mainrsrc,"CHECK DATE",DATES,10,CURRENT_DATE10,TRUE);
	} else addstdrsrc(mainrsrc,"CHECK DATE",DATES,8,CURRENT_DATE,TRUE);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	if(record_check_number==TRUE)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+15);
#ifdef WIN32
		sprintf(libx,"%s\\%s\\",CURRENTDIRECTORY,"VENPMT");
#endif
#ifndef WIN32
		sprintf(libx,"%s/%s/",CURRENTDIRECTORY,"VENPMT");
#endif
		FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&x);
		sprintf(stemp,"%s.CHK",BANKids->libs[x]);
		temp1=adddashes(stemp);
		start_check=getnexttran(libx,temp1,(-1));
		if(temp1!=NULL) Rfree(temp1);
		++start_check;
		if(libx!=NULL) Rfree(libx);
		FINDRSCSETINT(mainrsrc,"STARTING CHECK NUMBER",start_check);
	}
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
