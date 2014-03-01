/* itronout.c - ITRON Create Handheld File */
#ifndef WIN32
#define __NAM__ "itronout.lnx"
#endif
#ifdef WIN32
#define __NAM__ "itronout.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
/*CPP_OPART itronout1 */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTLBLL",*ITRON_FILENAME=NULL,*itron_expression=NULL;
char *CycleDate=NULL,*CycleDate1=NULL;
short utbcus=(-1),utbsvc=(-1),utbmtr=(-1),utbsds=(-1),utbrte=(-1);
DBsort *sortfile=NULL;
extern void CreateItronFile();
short UseRadio=FALSE,UseWand=FALSE,UseProbe=FALSE,NumberKeys=1;
RDArsrc *MAINrsrc=NULL;

APPlib *makefieldvallist()
{
	APPlib *list=NULL;

	list=APPlibNEW();
	AddFileFields(list,utbcus);
	AddFileFields(list,utbsvc);
	AddFileFields(list,utbmtr);
	AddFileFields(list,utbsds);
	addAPPlibNoDuplicates(list,"[BODY COUNT]");
	addAPPlibNoDuplicates(list,"[BREAK LEVEL]");
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
	if(mainrsrc!=NULL) 
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	mainrsrc=NULL;
	if(utbcus!=(-1)) CLSNRD(utbcus);
	if(utbsvc!=(-1)) CLSNRD(utbsvc);
	if(utbmtr!=(-1)) CLSNRD(utbmtr);
	if(utbsds!=(-1)) CLSNRD(utbsds);
	if(utbrte!=(-1)) CLSNRD(utbrte);
	if(itron_expression!=NULL) Rfree(itron_expression);
	if(CycleDate!=NULL) Rfree(CycleDate);
	if(CycleDate1!=NULL) Rfree(CycleDate1);
	ShutdownSubsystems();
	exit(0);
}
void makeDBsortfile(DBsort *dbsrt)
{
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE KEY 1");
	addDBfield(dbsrt,"ROUTE NUMBER",8,4);
	addDBfield(dbsrt,"SEQUENCE NUMBER",9,15);
	addDBfield(dbsrt,"CUSTOMER IDENTIFICATION",PLAINTEXT,15);
	addDBfield(dbsrt,"SERVICE NUMBER",8,4);
	addDBfield(dbsrt,"CUSTOMER NAME",1,40);
	addDBfield(dbsrt,"SERVICE IDENTIFICATION",1,15);
	addDBfield(dbsrt,"READER TYPE",7,2);
	addDBfield(dbsrt,"METER TYPE",7,2);
	addDBfield(dbsrt,"METER DESCRIPTION",1,40);
	addDBfield(dbsrt,"DIALS",7,2);
	addDBfield(dbsrt,"COLLECTION TYPE",7,2);
	addDBfield(dbsrt,"ACTIVE",BOOLNS,1);
	addDBfield(dbsrt,"READING ID",1,4);
	addDBfield(dbsrt,"PROMPT CODE",7,2);
	addDBfield(dbsrt,"READ DIRECTION",7,2);
	addDBfield(dbsrt,"COMPARE CODE",7,2);
	addDBfield(dbsrt,"VALIDATION CODE",7,2);
	addDBfield(dbsrt,"DECIMAL POSITIONS",7,2);
	addDBfield(dbsrt,"VISUAL READ",10,1);
	addDBfield(dbsrt,"HIGH2 PERCENTAGE",SLONGV,6);
	addDBfield(dbsrt,"LOW2 PERCENTAGE",SLONGV,6);
	addDBfield(dbsrt,"WAND PROGRAM",1,4);
	addDBfield(dbsrt,"LOCATION",1,15);
	addDBfield(dbsrt,"SERIAL NUMBER",1,25);
	addDBfield(dbsrt,"PREVIOUS READING",20,15);
	addDBfield(dbsrt,"STANDARD DEVIATION",20,15);
	addDBfield(dbsrt,"AVERAGE READING",20,15);
	addDBfield(dbsrt,"NAME OF RESIDENT",1,40);
	addDBfield(dbsrt,"ADDRESS 1",1,40);
	addDBfield(dbsrt,"ADDRESS 2",1,40);
	addDBfield(dbsrt,"CITY",1,25);
	addDBfield(dbsrt,"STATE",1,2);
	addDBfield(dbsrt,"ZIP",5,10);
	addDBfield(dbsrt,"PREVIOUS DEMAND",20,12);
	addDBfield(dbsrt,"METER STATUS",1,15);
	addDBfield(dbsrt,"METER ACTION",1,15);
	addDBfield(dbsrt,"METER SIZE",1,15);
	addDBfield(dbsrt,"READER ALERT",1,15);
	addDBfield(dbsrt,"RF FREQUENCY",DECIMALV,14);
	addDBfield(dbsrt,"RF TONE",SHORTV,4);
	addDBfield(dbsrt,"PROBE ID",1,15);
	addDBfield(dbsrt,"DEVICE ID",1,20);
	addDBfield(dbsrt,"METER CONFIG ID",1,8);
	addDBfield(dbsrt,"DEVICE PASSWORD",1,20);
	addDBfield(dbsrt,"TAMPER STATUS",1,4);
	addDBfield(dbsrt,"NO TAMPER CHECK",10,1);
	addDBfield(dbsrt,"CONCENTRATOR INDICATOR",BOOLNS,1);
	addDBfield(dbsrt,"METER TIME CODE",TIMES,8);
	addDBfield(dbsrt,"DAISY CHAINED",BOOLNS,1);
	addDBfield(dbsrt,"REGISTER READ INDICATOR",BOOLNS,1);
	addDBfield(dbsrt,"RESET DEMAND INDICATOR",BOOLNS,1);
	addDBfield(dbsrt,"DST INDICATOR",BOOLNS,1);
	addDBfield(dbsrt,"STATUS CHECK INDICATOR",BOOLNS,1);
	addDBfield(dbsrt,"ENCODER",BOOLNS,1);
	addDBfield(dbsrt,"ENCODER TYPE",PLAINTEXT,32);
	addDBfield(dbsrt,"COMMUNICATION ADDRESS",PLAINTEXT,2);
	addDBfield(dbsrt,"TIME ZONE ADJUST",SLONGV,5);
	addDBfield(dbsrt,"NODE NUMBER",SHORTV,2);
	addDBfield(dbsrt,"POSITIVE DIAL CREEP",SLONGV,5);
	addDBfield(dbsrt,"NEGATIVE DIAL CREEP",SLONGV,5);
	addDBfield(dbsrt,"BILLING TYPE",7,2);
	addDBfield(dbsrt,"READ TIME",SHORTV,3);
	addDBfield(dbsrt,"GEOGRAPHIC AREA",SHORTV,2);
	addDBfield(dbsrt,"MULTIPLIER",8,10);

	addDBkeypart(dbsrt,keyno,"ROUTE NUMBER");
	addDBkeypart(dbsrt,keyno,"SEQUENCE NUMBER");
	addDBkeypart(dbsrt,keyno,"CUSTOMER IDENTIFICATION");
	addDBkeypart(dbsrt,keyno,"SERVICE NUMBER");
}
void addsortrecord(DBsort *dbsrt)
{
	short ColType=0;

	ZERNRD(dbsrt->fileno);
/* utbsvc */
	COPYFIELD(utbsvc,dbsrt->fileno,"ROUTE NUMBER");
	COPYFIELD(utbsvc,dbsrt->fileno,"SEQUENCE NUMBER");
	COPYFIELD(utbsvc,dbsrt->fileno,"CUSTOMER IDENTIFICATION");
	COPYFIELD(utbsvc,dbsrt->fileno,"SERVICE NUMBER");
	COPYFIELD(utbsvc,dbsrt->fileno,"NAME OF RESIDENT");
	COPYFIELD(utbsvc,dbsrt->fileno,"ADDRESS 1");
	COPYFIELD(utbsvc,dbsrt->fileno,"ADDRESS 2");
	COPYFIELD(utbsvc,dbsrt->fileno,"ACTIVE");
	COPYFIELD(utbsvc,dbsrt->fileno,"CITY");
	COPYFIELD(utbsvc,dbsrt->fileno,"STATE");
	COPYFIELD(utbsvc,dbsrt->fileno,"ZIP");
	COPYFIELD(utbsvc,dbsrt->fileno,"SERVICE IDENTIFICATION");
	COPYFIELD(utbsvc,dbsrt->fileno,"PREVIOUS READING");
	COPYFIELD(utbsvc,dbsrt->fileno,"STANDARD DEVIATION");
	COPYFIELD(utbsvc,dbsrt->fileno,"AVERAGE READING");
	COPYFIELD(utbsvc,dbsrt->fileno,"METER TYPE");
	COPYFIELD(utbsvc,dbsrt->fileno,"METER SIZE");
	COPYFIELD(utbsvc,dbsrt->fileno,"SERIAL NUMBER");
	COPYFIELD(utbsvc,dbsrt->fileno,"LOCATION");
	COPYFIELD(utbsvc,dbsrt->fileno,"PREVIOUS DEMAND");
	COPYFIELD(utbsvc,dbsrt->fileno,"METER STATUS");
	COPYFIELD(utbsvc,dbsrt->fileno,"METER ACTION");
	COPYFIELD(utbsvc,dbsrt->fileno,"READER ALERT");
	COPYFIELD(utbsvc,dbsrt->fileno,"RF FREQUENCY");
	COPYFIELD(utbsvc,dbsrt->fileno,"RF TONE");
	COPYFIELD(utbsvc,dbsrt->fileno,"PROBE ID");
	COPYFIELD(utbsvc,dbsrt->fileno,"DEVICE ID");
	COPYFIELD(utbsvc,dbsrt->fileno,"METER CONFIG ID");
	COPYFIELD(utbsvc,dbsrt->fileno,"DEVICE PASSWORD");
	COPYFIELD(utbsvc,dbsrt->fileno,"TAMPER STATUS");
	COPYFIELD(utbsvc,dbsrt->fileno,"NO TAMPER CHECK");
	COPYFIELD(utbsvc,dbsrt->fileno,"CONCENTRATOR INDICATOR");
	COPYFIELD(utbsvc,dbsrt->fileno,"METER TIME CODE");
	COPYFIELD(utbsvc,dbsrt->fileno,"DAISY CHAINED");
	COPYFIELD(utbsvc,dbsrt->fileno,"DST INDICATOR");
	COPYFIELD(utbsvc,dbsrt->fileno,"REGISTER READ INDICATOR");
	COPYFIELD(utbsvc,dbsrt->fileno,"RESET DEMAND INDICATOR");
	COPYFIELD(utbsvc,dbsrt->fileno,"DST UPDATE INDICATOR");
	COPYFIELD(utbsvc,dbsrt->fileno,"STATUS CHECK INDICATOR");
	COPYFIELD(utbsvc,dbsrt->fileno,"ENCODER");
	COPYFIELD(utbsvc,dbsrt->fileno,"ENCODER TYPE");
	COPYFIELD(utbsvc,dbsrt->fileno,"COMMUNICATION ADDRESS");
	COPYFIELD(utbsvc,dbsrt->fileno,"TIME ZONE ADJUST");
	COPYFIELD(utbsvc,dbsrt->fileno,"NODE NUMBER");
	COPYFIELD(utbsvc,dbsrt->fileno,"POSITIVE DIAL CREEP");
	COPYFIELD(utbsvc,dbsrt->fileno,"NEGATIVE DIAL CREEP");
	COPYFIELD(utbsvc,dbsrt->fileno,"READ TIME");
	COPYFIELD(utbsvc,dbsrt->fileno,"GEOGRAPHIC AREA");
/* utbcus */
	COPYFIELD(utbcus,dbsrt->fileno,"CUSTOMER NAME");
/* utbsds */
	COPYFIELD(utbsds,dbsrt->fileno,"READER TYPE");
	COPYFIELD(utbsds,dbsrt->fileno,"BILLING TYPE");
/* utbmtr */
	FINDFLDGETSHORT(utbmtr,"COLLECTION TYPE",&ColType);
	COPYFIELD(utbmtr,dbsrt->fileno,"READING ID");
	COPYFIELD(utbmtr,dbsrt->fileno,"PROMPT CODE");
	COPYFIELD(utbmtr,dbsrt->fileno,"READ DIRECTION");
	COPYFIELD(utbmtr,dbsrt->fileno,"COMPARE CODE");
	COPYFIELD(utbmtr,dbsrt->fileno,"VALIDATION CODE");
	COPYFIELD(utbmtr,dbsrt->fileno,"DECIMAL POSITIONS");
	COPYFIELD(utbmtr,dbsrt->fileno,"VISUAL READ");
	COPYFIELD(utbmtr,dbsrt->fileno,"HIGH2 PERCENTAGE");
	COPYFIELD(utbmtr,dbsrt->fileno,"LOW2 PERCENTAGE");
	COPYFIELD(utbmtr,dbsrt->fileno,"WAND PROGRAM");
	COPYFIELD(utbmtr,dbsrt->fileno,"MULTIPLIER");


	if(ColType==1) UseProbe=TRUE;
	else if(ColType==2) UseWand=TRUE;
	else if(ColType==3) UseRadio=TRUE;
	COPYFIELD(utbmtr,dbsrt->fileno,"COLLECTION TYPE");
	COPYFIELD(utbmtr,dbsrt->fileno,"DIALS");
	FIELDCOPY(utbmtr,"DESCRIPTION",dbsrt->fileno,"METER DESCRIPTION");
	WRTNRD(dbsrt->fileno,0,NULL);
}

void CreateItron(RDArsrc *mainrsrc)
{
	int x=0;
	short ef=0,increment_diagnostic=FALSE;
	RDArsrc *tmprsrc=NULL;
	char *sortname=NULL,record_added=FALSE,deleteflag=FALSE;
	char repository=FALSE;
/*	char disconnect=FALSE; */
	RangeScreen *rs=NULL;
	RDAvirtual *v;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSSION",&itron_expression);
	FINDRSCGETSTRING(mainrsrc,"FILENAME",&ITRON_FILENAME);
	FINDRSCGETSTRING(mainrsrc,"CYCLE DATE",&CycleDate1);
	FINDRSCGETSHORT(mainrsrc,"NUMBER KEYS",&NumberKeys);
	if(CycleDate!=NULL) Rfree(CycleDate);
	CycleDate=Rmalloc(9);
	sprintf(CycleDate,"%.2s%.2s%.4s",&CycleDate1[0],&CycleDate1[3],&CycleDate1[6]);
	sortname=unique_name();
#ifndef USE_MYISAM
	sortfile=DBsortNEW(module,sortname,NRDLocalCtreeEngine);
	sortfile->engine=NRDLocalCtreeEngine;
#endif
#ifdef USE_MYISAM
	sortfile=DBsortNEW(module,sortname,NRDMyIsamEngine);
#endif
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(sortfile);
	sortfile->fileno=OPNDBsort(sortfile);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(utbcus,rs);
	ReadRangeScreen(utbsvc,rs);
	ReadRangeScreen(utbmtr,rs);
	ReadRangeScreen(utbsds,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{			
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(utbsvc,"DIAGNOSTIC SCREEN",module,
		"Selecting Utility Services",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
		}
	}
	ef=ADVBEGNRDsec(utbsvc,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		increment_diagnostic=FALSE;
		FINDFLDGETCHAR(utbsvc,"DELETEFLAG",&deleteflag);
/*		FINDFLDGETCHAR(utbsvc,"DISCONNECTED",&disconnect); */
		FINDFLDGETCHAR(utbsvc,"REPOSITORY SERVICE",&repository);
/*		if(!deleteflag && !repository && !disconnect) */
		if(!deleteflag && !repository)
		{
			if(!testfilerangersrc(utbsvc,rs))
			{
				ZERNRD(utbrte);
				COPYFIELD(utbsvc,utbrte,"ROUTE NUMBER");
				if(ADVEQLNRDsec(utbrte,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(utbrte,1);
				ZERNRD(utbcus);
				COPYFIELD(utbsvc,utbcus,"CUSTOMER IDENTIFICATION");
				if(ADVEQLNRDsec(utbcus,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(utbcus,1);
				FINDFLDGETCHAR(utbcus,"DELETEFLAG",&deleteflag);
				ZERNRD(utbsds);
				COPYFIELD(utbsvc,utbsds,"SERVICE IDENTIFICATION");
				if(ADVEQLNRDsec(utbsds,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(utbsds,1);
				ZERNRD(utbmtr);
				FIELDCOPY(utbsvc,"METER TYPE",utbmtr,"TYPE");
				if(ADVEQLNRDsec(utbmtr,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(utbmtr,1);
				if(!deleteflag) 
				{
					if(!testfilerangersrc(utbcus,rs) && !testfilerangersrc(utbmtr,rs) && !testfilerangersrc(utbsds,rs) && !testfilerangersrc(utbrte,rs))
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
							if(itron_expression!=NULL)
							{
								if(EVALUATEbol(itron_expression,SCRNvirtualSubData,mainrsrc))
								{
									addsortrecord(sortfile);
									increment_diagnostic=TRUE;
									record_added=TRUE;
								}
							} else {
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
		ef=ADVSEQNRDsec(utbsvc,SCRNvirtualSubData,mainrsrc);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(SIZNRD(sortfile->fileno) || record_added==TRUE)
	{
		CreateItronFile();
	}
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
	quitfunc(mainrsrc);
}

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"ITRON CREATE FILE")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((utbcus=APPOPNNRD(module,"UTBCUS",TRUE,FALSE))==(-1)) return;
	if((utbsvc=APPOPNNRD(module,"UTBSVC",TRUE,FALSE))==(-1)) return;
	if((utbmtr=APPOPNNRD(module,"UTBMTR",TRUE,FALSE))==(-1)) return;
	if((utbsds=APPOPNNRD(module,"UTBSDS",TRUE,FALSE))==(-1)) return;
	if((utbrte=APPOPNNRD(module,"UTBRTE",TRUE,FALSE))==(-1)) return;
	MAINrsrc=RDArsrcNEW(module,"ITRON CREATE FILE");
	if(utbcus!=(-1)) file2rangersrc(utbcus,MAINrsrc);
	if(utbsvc!=(-1)) file2rangersrc(utbsvc,MAINrsrc);
	if(utbmtr!=(-1)) file2rangersrc(utbmtr,MAINrsrc);
	if(utbsds!=(-1)) file2rangersrc(utbsds,MAINrsrc);
	if(utbrte!=(-1)) file2rangersrc(utbrte,MAINrsrc);
	addDFincvir(MAINrsrc,module,"UTBCUS",NULL,utbcus);
	addDFincvir(MAINrsrc,module,"UTBSVC",NULL,utbsvc);
	addDFincvir(MAINrsrc,module,"UTBMTR",NULL,utbmtr);
	addDFincvir(MAINrsrc,module,"UTBSDS",NULL,utbsds);
	addDFincvir(MAINrsrc,module,"UTBRTE",NULL,utbrte);
	ITRON_FILENAME=stralloc("DOWNLOAD.DAT");
	addstdrsrc(MAINrsrc,"FILENAME",VARIABLETEXT,0,NULL,TRUE);
	CycleDate1=GETCURRENTDATE10();
	addstdrsrc(MAINrsrc,"CYCLE DATE",DATES,10,CycleDate1,TRUE);
	addstdrsrc(MAINrsrc,"NUMBER KEYS",SHORTV,2,&NumberKeys,TRUE);
	FINDRSCSETSTRING(MAINrsrc,"FILENAME",ITRON_FILENAME);
	addsctrsrc(MAINrsrc,"EXPRESSION",0,NULL,TRUE);
	GET_ALL_SCREEN_VIRTUALS(MAINrsrc,2);
	SCRNvirtual2rangersrc(MAINrsrc);
	DefaultScreens(MAINrsrc);
	SetRangersrcsensitive(MAINrsrc);
	addbtnrsrc(MAINrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(MAINrsrc,"LOAD OPERAND",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(MAINrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(MAINrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(MAINrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(MAINrsrc,"SELECT",TRUE,CreateItron,NULL);
	addrfcbrsrc(MAINrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(MAINrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(MAINrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(MAINrsrc);
	APPmakescrn(MAINrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
