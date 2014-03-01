/* restspnm.c - Set Deed Book Page Numbers */
#ifndef WIN32
#define __NAM__ "restspn.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restspn.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <mix.h>
#include <ldval.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="RLSTMGT",*vch_expression=NULL;
short rstown=0,rstyr=0,rstmst=0,rstoyr=0,body_count=10;
RDAvirtual *csortsequence=NULL;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,rstown);
	AddFileFields(list,rstoyr);
	AddFileFields(list,rstyr);
	AddFileFields(list,rstmst);
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
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	if(rstmst!=(-1)) CLSNRD(rstmst);
	if(rstyr!=(-1)) CLSNRD(rstyr);
	if(rstown!=(-1)) CLSNRD(rstown);
	if(rstoyr!=(-1)) CLSNRD(rstoyr);
	if(vch_expression!=NULL) Rfree(vch_expression);
	ShutdownSubsystems();
}
void makesortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"STATEMENT SORT SEQUENCE");
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
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				csortsequence->value.float_value=Rmalloc(sizeof(double));
				*csortsequence->value.float_value=0;
				csortsequence->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				csortsequence->value.short_value=Rmalloc(sizeof(short));
				csortsequence->value.short_value=0;
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
	key=KEYNUM(rstoyr,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(rstoyr,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
			addDBkeypart(dbsrt,keyno,part->name);
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
			case DOLLARS_NOCENTS:
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
	key=KEYNUM(rstoyr,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(rstoyr,dbsrt->fileno,part->name);
		}
	}
	WRTNRD(dbsrt->fileno,0,NULL);
}
int voucher_count(RDArsrc *mainrsrc,short fileno,char *realestateid,char *ownerid)
{
	int count=0;
	short ef;
	char *realid=NULL,*ownid=NULL;

	ef=ADVGTENRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(fileno,"REAL ESTATE IDENTIFICATION",&realid);
		FINDFLDGETSTRING(fileno,"OWNER IDENTIFICATION",&ownid);
		unpad(realid);unpad(ownid);
		if(!RDAstrcmp(realid,realestateid) &&
			!RDAstrcmp(ownid,ownerid)) ++count;
			else break;
		ef=ADVNXTNRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	}
	if(realid!=NULL) Rfree(realid);
	if(ownid!=NULL) Rfree(ownid);
	return(count);
}
void setcheck(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE,sortfile2=(-1);
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*realestateid=NULL,*last_realestateid=NULL;
	char *ownerid=NULL,*last_ownerid=NULL;
	RDArsrc *tmprsrc=NULL;
	int check_number=0,ven_count=0,x=0,numfields=0;
	NRDpart *part;
	NRDkey *key=NULL;
	NRDfield *field=NULL,*fields=NULL;
	RDAvirtual *v;
	char deleteflag=FALSE;
	RDATData *prev=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSSION",&vch_expression);
	FINDRSCGETSHORT(mainrsrc,"BODY COUNT",&body_count);
	sortname=unique_name();
	sortfile=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makesortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(rstoyr,rs);
	ReadRangeScreen(rstown,rs);
	ReadRangeScreen(rstmst,rs);
	tmprsrc=diagscrn(rstoyr,"DIAGNOSTIC SCREEN",module,
		"Selecting Real Estate Owner Years",NULL);
	if(tmprsrc!=NULL)
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	ef=ADVBEGNRDsec(rstoyr,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(rstoyr,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			COPYFIELD(rstoyr,rstmst,"REAL ESTATE IDENTIFICATION");
			if(!ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(rstmst,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					COPYFIELD(rstoyr,rstown,"REAL ESTATE IDENTIFICATION");
					COPYFIELD(rstoyr,rstown,"OWNER IDENTIFICATION");
					if(!ADVEQLNRDsec(rstown,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDGETCHAR(rstown,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							COPYFIELD(rstoyr,rstyr,
								"REAL ESTATE IDENTIFICATION");
							COPYFIELD(rstoyr,rstyr,"FISCAL YEAR");
							if(!ADVEQLNRDsec(rstyr,1,SCRNvirtualSubData,mainrsrc))
							{
								FINDFLDGETCHAR(rstyr,"DELETEFLAG",&deleteflag);
								if(!deleteflag)
								{
									if(!testfilerangersrc(rstoyr,rs) && 
										!testfilerangersrc(rstown,rs) &&
										!testfilerangersrc(rstmst,rs)) 
									{
										for(x=0,v=mainrsrc->virtual;
											x<mainrsrc->num;++x,++v)
										{
											if(!v->computed) computevirtual(v,SCRNvirtualSubData,mainrsrc);
											if(testvirtualrangersrc(v,rs)) break;
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
		if(tmprsrc!=NULL) 
			if(tmprsrc->swidget!=NULL)  
				update_diagnostic(tmprsrc,increment_diagnostic);
		ef=ADVSEQNRDsec(rstoyr,SCRNvirtualSubData,mainrsrc);
	}
	if(tmprsrc!=NULL)
	{
		if(tmprsrc->swidget!=NULL) killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(SIZNRD(sortfile->fileno))
	{
		sortfile2=OPNNRD(sortfile->module,sortfile->filename);
		if(sortfile2!=(-1))
		{
			key=KEYNUM(rstoyr,1);
			if(key!=NULL)
			{
				tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
					module,"Setting Check Numbers",NULL);
				if(tmprsrc!=NULL)
					if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
				check_number=1;
				ef=FRSNRD(sortfile->fileno,1);
				FINDFLDGETSTRING(sortfile->fileno,
					"REAL ESTATE IDENTIFICATION",&realestateid);
				FINDFLDGETSTRING(sortfile->fileno,
					"OWNER IDENTIFICATION",&ownerid);
				last_realestateid=stralloc(realestateid);
				last_ownerid=stralloc(ownerid);
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
				ven_count=voucher_count(mainrsrc,sortfile2,realestateid,ownerid);
				check_number+=((ven_count+body_count-1)/body_count)-1;
				while(!ef)
				{
					FINDFLDGETSTRING(sortfile->fileno,
						"REAL ESTATE IDENTIFICATION",&realestateid);
					FINDFLDGETSTRING(sortfile->fileno,
						"OWNER IDENTIFICATION",&ownerid);
					if(RDAstrcmp(last_realestateid,realestateid) ||
						RDAstrcmp(last_ownerid,ownerid))
					{
						if(last_realestateid!=NULL) Rfree(last_realestateid);
						if(last_ownerid!=NULL) Rfree(last_ownerid);
						last_realestateid=stralloc(realestateid);
						last_ownerid=stralloc(ownerid);
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
						ven_count=voucher_count(mainrsrc,sortfile2,realestateid,ownerid);
						check_number+=((ven_count+body_count-1)/body_count);
					}
					for(x=0,part=key->part;x<key->numparts;++x,++part)
						COPYFIELD(sortfile->fileno,rstoyr,part->name);
					LOCNRDFILE(rstoyr);
					if(!ADVEQLNRDsec(rstoyr,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(rstoyr);
						FINDFLDSETINT(rstoyr,"PAGE NO",
							check_number);
						ADVWRTTRANSsec(rstoyr,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(rstoyr);
					if(tmprsrc!=NULL) if(tmprsrc->swidget!=NULL)  
						update_diagnostic(tmprsrc,TRUE);
					ef=NXTNRD(sortfile->fileno,1);
				}
				if(tmprsrc!=NULL)
				{
					if(tmprsrc->swidget!=NULL) killwindow(tmprsrc);
					free_rsrc(tmprsrc);
					tmprsrc=NULL;
				}
			}
			if(sortfile2!=(-1)) CLSNRD(sortfile2);
		} else {
			prterr("Error couldn't open sortfile [%s] [%s].",sortfile->module,sortfile->filename);
		}
	} else {
		prterr("Error no Real Estate Transactions Selected.");
	}
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
}
void main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"SET DEED BOOK PAGE NUMBERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((rstown=APPOPNNRD(module,"RESTOWN",TRUE,FALSE))==(-1)) return;
	if((rstoyr=APPOPNNRD(module,"RESTOYR",TRUE,TRUE))==(-1)) return;
	if((rstyr=APPOPNNRD(module,"RESTYEAR",TRUE,FALSE))==(-1)) return;
	if((rstmst=APPOPNNRD(module,"RESTMST",TRUE,FALSE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"SET DEED BOOK PAGE NUMBERS");
	if(rstmst!=(-1)) file2rangersrc(rstmst,mainrsrc);
	if(rstyr!=(-1)) file2rangersrc(rstyr,mainrsrc);
	if(rstown!=(-1)) file2rangersrc(rstown,mainrsrc);
	if(rstoyr!=(-1)) file2rangersrc(rstoyr,mainrsrc);
	addDFincvir(mainrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(mainrsrc,module,"RESTOWN",NULL,rstown);
	addDFincvir(mainrsrc,module,"RESTYEAR",NULL,rstyr);
	addDFincvir(mainrsrc,module,"RESTOYR",NULL,rstoyr);
	addstdrsrc(mainrsrc,"BODY COUNT",SHORTV,3,&body_count,TRUE);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,setcheck,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
