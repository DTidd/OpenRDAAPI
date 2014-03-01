/* restsstm.c - Set Statement Numbers */
#ifndef WIN32
#define __NAM__ "restsstm.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restsstm.exe"
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

char *module="RLSTMGT",*vch_expression=NULL;
short filenum=0,body_count=1;
short rstdst=(-1),rststm=(-1);
int start_check=1;
RDAvirtual *csortsequence=NULL;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,filenum);
	AddFileFields(list,rstdst);
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
	if(filenum!=(-1)) CLSNRD(filenum);
	if(rstdst!=(-1)) CLSNRD(rstdst);
	if(rststm!=(-1)) CLSNRD(rststm);
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
	key=KEYNUM(filenum,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(filenum,part->name);
			if(field!=NULL) 
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
				addDBkeypart(dbsrt,keyno,part->name);
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
	key=KEYNUM(filenum,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			COPYFIELD(filenum,dbsrt->fileno,part->name);
		}
	}
	WRTNRD(dbsrt->fileno,0,NULL);
}
int voucher_count(RDArsrc *mainrsrc,short fileno,char *realestateid,char *ownerid)
{
	int count=0;
	short ef;
	char *ownid=NULL,*realid=NULL;

	ef=ADVGTENRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(fileno,"OWNER IDENTIFICATION",&ownid);
		FINDFLDGETSTRING(fileno,"REAL ESTATE IDENTIFICATION",&realid);
		unpad(ownid);unpad(realid);
		if(!RDAstrcmp(ownid,ownerid) && !RDAstrcmp(realid,realestateid)) ++count;
			else break;
		ef=ADVNXTNRDsec(fileno,1,SCRNvirtualSubData,mainrsrc);
	}
	if(ownid!=NULL) Rfree(ownid);
	if(realid!=NULL) Rfree(realid);
	return(count);
}
void setcheck(RDArsrc *mainrsrc)
{
	short ef=0,increment_diagnostic=FALSE,sortfile2=(-1);
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	char *sortname=NULL;
	char *ownerid=NULL,*last_ownerid=NULL;
	char *realid=NULL,*last_realid=NULL;
	RDArsrc *tmprsrc=NULL;
	int check_number=0,ven_count=0,x=0,numfields=0;
	NRDpart *part;
	NRDkey *key=NULL;
	NRDfield *field=NULL,*fields=NULL;
	RDAvirtual *v;
	char deleteflag=FALSE;
	RDATData *prev=NULL;
	char record_added=FALSE;
	char *errmsg=NULL;
	char flag=FALSE;
	int old_number=0;
	

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSSION",&vch_expression);
	FINDRSCGETSHORT(mainrsrc,"BODY COUNT",&body_count);
	FINDRSCGETINT(mainrsrc,"STARTING STATEMENT NUMBER",&start_check);
	sortname=unique_name();
	sortfile=DBsortNEW(module,sortname,NRDLocalCtreeEngine);
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
	ReadRangeScreen(filenum,rs);
	ReadRangeScreen(rstdst,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{			
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(filenum,"DIAGNOSTIC SCREEN",module,
		"Selecting Real Estate Transactions",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
		}
	}
	ef=ADVBEGNRDsec(filenum,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		if(!testfilerangersrc(filenum,rs))
		{
			COPYFIELD(filenum,rstdst,"DISTRICT IDENTIFICATION");
			if(ADVEQLNRDsec(rstdst,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(rstdst,1);
			FINDFLDGETCHAR(rstdst,"DELETEFLAG",&deleteflag);
			if(!deleteflag) 
			{
			if(!testfilerangersrc(rstdst,rs))
			{
			for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
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
							csortsequence->computed=FALSE;
							computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
						}
						addsortrecord(sortfile);
						increment_diagnostic=TRUE;
						record_added=TRUE;
					}
				} else {
					if(csortsequence!=NULL)
					{
						csortsequence->computed=FALSE;
						computevirtual(csortsequence,SCRNvirtualSubData,mainrsrc);
					}
					addsortrecord(sortfile);
					increment_diagnostic=TRUE;
					record_added=TRUE;
				}
			} else {
/* INSERT APPLICATION DIAGNOSTICS (diagapp) here */
			}
			} else {
/* INSERT APPLICATION DIAGNOSTICS (diagapp) here */
			}
			} else {
/* INSERT APPLICATION DIAGNOSTICS (diagapp) here */
			}
		}
		}
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,increment_diagnostic);
		ef=ADVSEQNRDsec(filenum,SCRNvirtualSubData,mainrsrc);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(SIZNRD(sortfile->fileno) || record_added==TRUE)
	{
		sortfile2=OPNNRD(sortfile->module,sortfile->filename);
		if(sortfile2!=(-1))
		{
			key=KEYNUM(filenum,1);
			if(key!=NULL)
			{
				tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",
					module,"Setting Statement Numbers",NULL);
				if(tmprsrc!=NULL)
					if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
				check_number=start_check;
				ef=FRSNRD(sortfile->fileno,1);
				FINDFLDGETSTRING(sortfile->fileno,"OWNER IDENTIFICATION",&ownerid);
				FINDFLDGETSTRING(sortfile->fileno,"REAL ESTATE IDENTIFICATION",&realid);
				last_ownerid=stralloc(ownerid);
				last_realid=stralloc(realid);
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
				ven_count=voucher_count(mainrsrc,sortfile2,realid,ownerid);
				check_number+=((ven_count+body_count-1)/body_count)-1;
				
				while(!ef)
				{
					FINDFLDGETSTRING(sortfile->fileno,"OWNER IDENTIFICATION",&ownerid);
					FINDFLDGETSTRING(sortfile->fileno,"REAL ESTATE IDENTIFICATION",&realid);
					if(RDAstrcmp(last_ownerid,ownerid) ||
						RDAstrcmp(last_realid,realid))
					{
						if(last_ownerid!=NULL) Rfree(last_ownerid);
						if(last_realid!=NULL) Rfree(last_realid);
						last_ownerid=stralloc(ownerid);
						last_realid=stralloc(realid);
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
						ven_count=voucher_count(mainrsrc,sortfile2,realid,ownerid);
						check_number+=((ven_count+body_count-1)/body_count);
					}
					for(x=0,part=key->part;x<key->numparts;++x,++part)
						COPYFIELD(sortfile->fileno,filenum,part->name);
					LOCNRDFILE(filenum);
					if(!ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(filenum);
						FINDFLDGETINT(filenum,"STATEMENT NUMBER",&old_number);
						FINDFLDSETINT(filenum,"STATEMENT NUMBER",check_number);
						ADVWRTTRANSsec(filenum,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
						ZERNRD(rststm);
						COPYFIELD(filenum,rststm,"TAX YEAR");
						FINDFLDSETINT(rststm,"STATEMENT NUMBER",old_number);
						LOCNRDFILE(rststm);
						if(!ADVEQLNRDsec(rststm,1,SCRNvirtualSubData,mainrsrc))
						{
							prev=RDATDataNEW(rststm);
							flag=TRUE;
							FINDFLDSETCHAR(rststm,"DELETEFLAG",flag);
							ADVWRTTRANSsec(rststm,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
							if(prev!=NULL) FreeRDATData(prev);
						}
						UNLNRDFILE(rststm);

						ZERNRD(rststm);
						COPYFIELD(filenum,rststm,"TAX YEAR");
						COPYFIELD(filenum,rststm,"STATEMENT NUMBER");
						COPYFIELD(filenum,rststm,"REAL ESTATE IDENTIFICATION");
						COPYFIELD(filenum,rststm,"OWNER IDENTIFICATION");
						LOCNRDFILE(rststm);
						if(ADVEQLNRDsec(rststm,1,SCRNvirtualSubData,mainrsrc))
						{
							COPYFIELD(filenum,rststm,"REAL ESTATE IDENTIFICATION");
							COPYFIELD(filenum,rststm,"OWNER IDENTIFICATION");
							flag=FALSE;
							FINDFLDSETCHAR(rststm,"DELETEFLAG",flag);
							ADVWRTTRANSsec(rststm,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
						} else {
							prev=RDATDataNEW(rststm);
							FINDFLDGETCHAR(rststm,"DELETEFLAG",&flag);
							if(flag==TRUE)
							{
								COPYFIELD(filenum,rststm,"REAL ESTATE IDENTIFICATION");
								COPYFIELD(filenum,rststm,"OWNER IDENTIFICATION");
								flag=FALSE;
								FINDFLDSETCHAR(rststm,"DELETEFLAG",flag);
								ADVWRTTRANSsec(rststm,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
								if(prev!=NULL) FreeRDATData(prev);
							}
						}
						UNLNRDFILE(rststm);
					}
					UNLNRDFILE(filenum);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
					ef=NXTNRD(sortfile->fileno,1);
				}
				if(tmprsrc!=NULL)
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
					tmprsrc=NULL;
				}
			}
			if(sortfile2!=(-1)) CLSNRD(sortfile2);
		} else {
			prterr("Error couldn't open sortfile [%s] [%s].",sortfile->module,sortfile->filename);
		}
	} else {
		errmsg=Rmalloc(112+1);
		sprintf(errmsg,"No records processed in Set Real Estate Statement Numbers using the given selection criteria.  Please try again.");
		ERRORDIALOG("SET REAL ESTATE STATEMENT NUMBERS ERROR!",errmsg,NULL,FALSE); 
		prterr(errmsg);
		if(errmsg!=NULL) Rfree(errmsg);
	}
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

	if(InitializeSubsystems(argc,argv,module,"SET STATEMENT NUMBERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRD(module,"RESTMSTR",TRUE,TRUE))==(-1)) return;
	if((rstdst=APPOPNNRD(module,"RESTDIST",TRUE,FALSE))==(-1)) return;
	if((rststm=APPOPNNRD(module,"RLSTSTM",TRUE,TRUE))==(-1)) return;
	mainrsrc=RDArsrcNEW(module,"SET STATEMENT NUMBERS");
	if(filenum!=(-1)) file2rangersrc(filenum,mainrsrc);
	if(rstdst!=(-1)) file2rangersrc(rstdst,mainrsrc);
	addDFincvir(mainrsrc,module,"RESTMSTR",NULL,filenum);
	addDFincvir(mainrsrc,module,"RESTDIST",NULL,rstdst);
	addDFincvir(mainrsrc,module,"RLSTSTM",NULL,rststm);
	addstdrsrc(mainrsrc,"STARTING STATEMENT NUMBER",LONGV,8,&start_check,TRUE);
	addstdrsrc(mainrsrc,"BODY COUNT",SHORTV,3,&body_count,TRUE);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
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
