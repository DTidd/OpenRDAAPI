/* prpstt.c - MBGUI Set Statement Numbers */
#ifndef WIN32
#define __NAM__ "prpstt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "prpstt.exe"
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

char *module="PROPERTY",*vch_expression=NULL;
char one_statement=FALSE;
short prpdist=(-1),prpmst=(-1),body_count=10;
short prpcls=(-1),prpstm=(-1);
int start_check=1,tax_year=0;
RDAvirtual *csortsequence=NULL;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	loadglobals(list);
	AddFileFields(list,prpmst);
	AddFileFields(list,prpdist);
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
	if(prpdist!=(-1)) CLSNRD(prpdist);
	if(prpmst!=(-1)) CLSNRD(prpmst);
	if(prpcls!=(-1)) CLSNRD(prpcls);
	if(prpstm!=(-1)) CLSNRD(prpstm);
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
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("PROPERTY","STATEMENT SORT SEQUENCE");
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
	key=KEYNUM(prpmst,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(prpmst,part->name);
			if(field!=NULL) 
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
				addDBkeypart(dbsrt,keyno,field->name);
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
	key=KEYNUM(prpmst,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(prpmst,dbsrt->fileno,part->name);
		}
	}
	WRTNRD(dbsrt->fileno,0,NULL);
}
int voucher_count(RDArsrc *mainrsrc,short fileno,char *vendorid)
{
	int count=0;
	short ef;
	char *venid=NULL;

	ef=ADVGTENRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(fileno,"OWNER IDENTIFICATION",&venid);
		if(!RDAstrcmp(venid,vendorid))
		{
			++count;
		} else { 
			break;
		}
		ef=ADVNXTNRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	}
	if(venid!=NULL) Rfree(venid);
	return(count);
}
void setcheck(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE,sortfile2=(-1);
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL,*vendorid=NULL,*last_vendorid=NULL;
	RDArsrc *tmprsrc=NULL;
	int ven_count=0,x=0,numfields=0;
	int statement_number=0,vindx=0,old_number=0;
	NRDpart *part;
	NRDkey *key=NULL;
	NRDfield *field=NULL,*fields=NULL;
	RDAvirtual *v;
	char deleteflag=FALSE;
	int tax_year1=0;
	char dobackend=FALSE;
	short keyno=(-1),key_range=FALSE;
	RDATData *prev=NULL;

	readallwidgets(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&vch_expression);
	FINDRSCGETSHORT(mainrsrc,"BODY COUNT",&body_count);
	FINDRSCGETINT(mainrsrc,"SELECTED TAX YEAR",&tax_year);
	FINDRSCGETINT(mainrsrc,"STARTING STATEMENT NUMBER",&start_check);
	sortname=unique_name();
	sortfile=DBsortNEW("PROPERTY",sortname,NRDRdaEngine);
#ifndef USE_MYISAM
	sortfile->engine=NRDLocalCtreeEngine;
#endif
#ifdef USE_MYISAM
	sortfile->engine=NRDMyIsamEngine;
#endif
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(prpmst,rs);
	ReadRangeScreen(prpdist,rs);
	ReadRangeScreen(prpcls,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(prpmst,"DIAGNOSTIC SCREEN",module,
		"Selecting Property Masters",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	}
	ZERNRD(prpmst);
	keyno=KEYNUMBER(prpmst,"PRPMST KEY");
	if(keyno!=(-1))
	{
		key_range=setfilekeyFROMrangersrc(prpmst,keyno,rs);
		if(key_range==TRUE)
		{
			ef=ADVGTENRDsec(prpmst,keyno,SCRNvirtualSubData,mainrsrc);
		} else {
			ef=ADVBEGNRDsec(prpmst,SCRNvirtualSubData,mainrsrc);
		}
	} else {
		key_range=FALSE;
		ef=ADVBEGNRDsec(prpmst,SCRNvirtualSubData,mainrsrc);
	}
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(prpmst,"DELETEFLAG",&deleteflag);
		FINDFLDGETINT(prpmst,"TAX YEAR",&tax_year1);
		if(deleteflag==FALSE && tax_year==tax_year1)
		{
			COPYFIELD(prpmst,prpcls,"CLASS IDENTIFICATION");
			if(!ADVEQLNRDsec(prpcls,1,SCRNvirtualSubData,mainrsrc))
			{
			COPYFIELD(prpmst,prpdist,"DISTRICT IDENTIFICATION");
			if(!ADVEQLNRDsec(prpdist,1,SCRNvirtualSubData,mainrsrc))
			{
				FINDFLDGETCHAR(prpdist,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					if(!testfilerangersrc(prpmst,rs) && 
						!testfilerangersrc(prpcls,rs) &&
						!testfilerangersrc(prpdist,rs))
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
								if(EVALUATEbol(vch_expression,SCRNvirtualSubData,mainrsrc))
								{
									if(csortsequence!=NULL)
									{
										computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
									}
									addsortrecord(sortfile);
									dobackend=TRUE;
									increment_diagnostic=TRUE;
								}
							} else {
								if(csortsequence!=NULL)
								{
									computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
								}
								addsortrecord(sortfile);
								dobackend=TRUE;
								increment_diagnostic=TRUE;
							}
						}	
					}
				}
			}
			}
		}	
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		if(key_range==TRUE && keyno!=(-1))
		{
			ef=ADVNXTNRDsec(prpmst,keyno,SCRNvirtualSubData,mainrsrc);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(prpmst,keyno,rs);
			}
		} else {	
			ef=ADVSEQNRDsec(prpmst,SCRNvirtualSubData,mainrsrc);
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
		sortfile2=OPNNRD(sortfile->module,sortfile->filename);
		if(sortfile2!=(-1))
		{
			key=KEYNUM(prpmst,1);
			if(key!=NULL)
			{
				tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
					module,"Setting Statement Numbers",NULL);
				if(tmprsrc!=NULL)
					if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
				statement_number=start_check;
				ef=FRSNRD(sortfile->fileno,1);
				FINDFLDGETSTRING(sortfile->fileno,"OWNER IDENTIFICATION",&vendorid);
				last_vendorid=stralloc(vendorid);
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
				ven_count=voucher_count(mainrsrc,sortfile2,vendorid);
				statement_number+=((ven_count+body_count-1)/body_count)-1;
				vindx=0;
				while(!ef)
				{
					FINDFLDGETSTRING(sortfile->fileno,"OWNER IDENTIFICATION",&vendorid);
					if(RDAstrcmp(last_vendorid,vendorid))
					{
						if(last_vendorid!=NULL) Rfree(last_vendorid);
						last_vendorid=stralloc(vendorid);
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
						ven_count=voucher_count(mainrsrc,sortfile2,vendorid);
						statement_number+=((ven_count+body_count-1)/body_count);
						vindx=1;
					} else {
						if(vindx!=0)
						{
							if(vindx%body_count==0)
							{
								vindx=1;
							} else {
								++vindx;
							}
						} else {
							++vindx;
						}
					}
					for(x=0,part=key->part;x<key->numparts;++x,++part)
						COPYFIELD(sortfile->fileno,prpmst,part->name);
					LOCNRDFILE(prpmst);
					if(!ADVEQLNRDsec(prpmst,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(prpmst);
						FINDFLDSETINT(prpmst,"STATEMENT NUMBER",statement_number);
						ADVWRTTRANSsec(prpmst,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
					}
					/*
					UNLNRDFILE(prpmst);
					LOCNRDFILE(prpstm);
					ZERNRD(prpstm);
					COPYFIELD(sortfile->fileno,prpstm,"OWNER IDENTIFICATION");
					if(!one_statement)
					{
						COPYFIELD(sortfile->fileno,prpstm,"DISTRICT IDENTIFICATION");
						COPYFIELD(sortfile->fileno,prpstm,"CLASS IDENTIFICATION");
					}
					COPYFIELD(sortfile->fileno,prpstm,"TAX YEAR");
					FINDFLDSETINT(prpstm,"STATEMENT NUMBER",statement_number);
					FINDFLDSETCHAR(prpstm,"DELETEFLAG",FALSE);	
					ADVWRTTRANSsec(prpstm,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
					UNLNRDFILE(prpstm);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
					*/
					if(!ADVEQLNRDsec(prpmst,1,SCRNvirtualSubData,mainrsrc))
					{
						old_number=0;
						FINDFLDGETINT(prpmst,"STATEMENT NUMBER",&old_number);
						if(old_number!=0)
						{
							LOCNRDFILE(prpstm);
							ZERNRD(prpstm);
							COPYFIELD(prpmst,prpstm,"TAX YEAR");
							FINDFLDSETINT(prpstm,"STATEMENT NUMBER",old_number);
							COPYFIELD(prpmst,prpstm,"CLASS IDENTIFICATION");
							COPYFIELD(prpmst,prpstm,"DISTRICT IDENTIFICATION");
							if(!ADVEQLNRDsec(prpstm,1,SCRNvirtualSubData,mainrsrc))
							{
								prev=RDATDataNEW(prpstm);
								FINDFLDSETCHAR(prpstm,"DELETEFLAG",TRUE);
								ADVWRTTRANSsec(prpstm,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
								if(prev!=NULL) FreeRDATData(prev);
							}
							UNLNRDFILE(prpstm);
				 		}
						prev=RDATDataNEW(prpmst);
						FINDFLDSETINT(prpmst,"STATEMENT NUMBER",statement_number);
						ADVWRTTRANSsec(prpmst,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(prpmst);
					LOCNRDFILE(prpstm);
					ZERNRD(prpstm);
					COPYFIELD(sortfile->fileno,prpstm,"TAX YEAR");
					FINDFLDSETINT(prpstm,"STATEMENT NUMBER",statement_number);
					COPYFIELD(sortfile->fileno,prpstm,"CLASS IDENTIFICATION");
					COPYFIELD(sortfile->fileno,prpstm,"DISTRICT IDENTIFICATION");
					if(!ADVEQLNRDsec(prpstm,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(prpstm);
						COPYFIELD(sortfile->fileno,prpstm,"OWNER IDENTIFICATION");
						FINDFLDSETCHAR(prpstm,"DELETEFLAG",FALSE);
						ADVWRTTRANSsec(prpstm,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
					} else {
						ZERNRD(prpstm);
						COPYFIELD(sortfile->fileno,prpstm,"OWNER IDENTIFICATION");
						COPYFIELD(sortfile->fileno,prpstm,"DISTRICT IDENTIFICATION");
						COPYFIELD(sortfile->fileno,prpstm,"CLASS IDENTIFICATION");
						COPYFIELD(sortfile->fileno,prpstm,"TAX YEAR");
						FINDFLDSETINT(prpstm,"STATEMENT NUMBER",statement_number);
						FINDFLDSETCHAR(prpstm,"DELETEFLAG",FALSE);	
						ADVWRTTRANSsec(prpstm,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
						
					}
					UNLNRDFILE(prpstm);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
					ef=NXTNRD(sortfile->fileno,1);
				}
				if(tmprsrc!=NULL)
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
			}
			if(sortfile2!=(-1)) CLSNRD(sortfile2);
		} else {
			prterr("Error couldn't open sortfile [%s] [%s].",sortfile->module,sortfile->filename);
		}
	} else {
		prterr("Error no Property Masters Selected.");
	}
	if(last_vendorid!=NULL) Rfree(last_vendorid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	if(InitializeSubsystems(argc,argv,module,"SET STATEMENT NUMBERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew(module,"ONE STATEMENT PER OWNER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			one_statement=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			one_statement=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if((prpmst=APPOPNNRD(module,"PRPMST",TRUE,TRUE))==(-1)) return;
	if((prpdist=APPOPNNRD(module,"PRPDIST",TRUE,FALSE))==(-1)) return;
	if((prpcls=APPOPNNRD(module,"PRPCLS",TRUE,FALSE))==(-1)) return;
	if((prpstm=APPOPNNRD(module,"PRPSTM",TRUE,TRUE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"SET STATEMENT NUMBERS");
	addDFincvir(mainrsrc,module,"PRPMST",NULL,prpmst);
	addDFincvir(mainrsrc,module,"PRPDIST",NULL,prpdist);
	addDFincvir(mainrsrc,module,"PRPCLS",NULL,prpcls);
	addDFincvir(mainrsrc,module,"PRPSTM",NULL,prpstm);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	if(prpmst!=(-1)) file2rangersrc(prpmst,mainrsrc);
	if(prpdist!=(-1)) file2rangersrc(prpdist,mainrsrc);
	if(prpcls!=(-1)) file2rangersrc(prpcls,mainrsrc);
	addstdrsrc(mainrsrc,"STARTING STATEMENT NUMBER",LONGV,8,&start_check,TRUE);
	addstdrsrc(mainrsrc,"SELECTED TAX YEAR",LONGV,4,&tax_year,TRUE);
	addstdrsrc(mainrsrc,"BODY COUNT",SHORTV,3,&body_count,TRUE);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,setcheck,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
