/* perdmgimp.c - Import Personnel Info into DB (PERDMG) */

#ifndef WIN32
#define __NAM__ "perdmgimp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "perdmgimp.exe"
#endif
#include <app.h>
#include<rdaglob.h>
#include<mix.h>
#include<impexp.h>
#include<mkimp.h>
#define DO_UNLINK
/*ADDLIB mklib */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIb trans */
/*ADDLIB sec */
/*ADDLIB gui */
/*ADDLIB trans */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *module="PRSNNL";
static short perdmgids=(-1),perdmg=(-1),filenum=(-1),perstat=(-1),poslmst=(-1);
static char *filename=NULL;
//char managepid=FALSE;
char UsingManagedID=FALSE;
RDAvirtual *csortsequence=NULL;
DBsort *sort=NULL;

static void CHECK_MANAGEDID()
{
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;


		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PRSNNL")+11);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("PRSNNL","MANAGE PERSONNEL IDENTIFICATIONS");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				UsingManagedID=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);
		/*prterr("Manage Personnel ID gsv = %d.",UsingManagedID);*/


}
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(filename!=NULL) Rfree(filename);
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(perdmgids!=(-1)) CLSNRD(perdmgids);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(perstat!=(-1)) CLSNRD(perstat);
	if(poslmst!=(-1)) CLSNRD(poslmst);
	ShutdownSubsystems();
	exit(0);
}
char *GetNextPID(short perdmgids,char *perid,void (*SubFunc)(),void *args)
{
	char deleteflag=FALSE,assigned=FALSE;
	char *tmp=NULL;
	RDATData *prev=NULL;
	short ef=0;

	if(perdmgids!=(-1)) 
	{
	ZERNRD(perdmgids);
	LOCNRDFILE(perdmgids);
	ef=ADVGTENRDsec(perdmgids,1,SubFunc,args);
	while(!ef)
	{
		FINDFLDGETCHAR(perdmgids,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(perdmgids,"ASSIGNED",&assigned);
		FINDFLDGETSTRING(perdmgids,"PERSONNEL IDENTIFICATION",&tmp);
		if(!deleteflag && !assigned)
		{
			prev=RDATDataNEW(perdmgids);
			assigned=TRUE;
			FINDFLDSETCHAR(perdmgids,"ASSIGNED",assigned);
			FINDFLDSETCHAR(perdmgids,"DELETEFLAG",assigned);
			if(SubFunc!=NULL) ADVWRTTRANSsec(perdmgids,0,NULL,prev,SubFunc,args);
				else WRTTRANSsec(perdmgids,0,NULL,prev);
			if(prev!=NULL) FreeRDATData(prev);
			UNLNRDFILE(perdmgids);
			return(tmp);
		}
	ef=ADVNXTNRDsec(perdmgids,1,SubFunc,args);
	}
	if(tmp!=NULL) Rfree(tmp);
	UNLNRDFILE(perdmgids);
	return(NULL);
	}
}

static void importperdmg(RDArsrc *r)
{
	FILE *fn=NULL;
	RangeScreen *rs=NULL;
	DBsort *sortfile=NULL;
	RDATData *prev=NULL;
	char *FIELD_DELIMITER=NULL,*fieldname=NULL;
	long N = 1000;
	char *fieldlist[N];
	char *RECORD_DELIMITER=NULL;
	short exitstatus=FALSE;
	int index=0;
	long len=0;
	char *ptr=NULL;
	//char *sortname=NULL;
	char *importLine=NULL;
	char *perid=NULL,*fname=NULL,*lname=NULL,*mname=NULL,*aname=NULL;
	char *add1=NULL,*city=NULL,*state=NULL,*zip=NULL;
	char *padd=NULL,*pcity=NULL,*pstate=NULL,*pzip=NULL,*cphone=NULL,*aphone=NULL,*gender=NULL,*ethnic=NULL;
	char *empdate=NULL,*wphone=NULL,*hdate=NULL,*email=NULL,*dbirth=NULL;
	char *ssn=NULL,*ssn1=NULL,*ssn2=NULL,*ssn3=NULL;
	char *phone1=NULL,*phone2=NULL,*phone3=NULL;

	/*
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;


		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PRSNNL")+11);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("PRSNNL","MANAGE PERSONNEL IDENTIFICATIONS");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				managepid=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);
		printf("In Main => Manage Personnel ID gsv = %d\n",managepid);
		*/
		
	if(diagapps)
	{
		prterr("Made it into the importperdmg function.\n");
	}
	readallwidgets(r);
	rs=RangeScreenNew(r);

	FINDRSCGETSTRING(r,"FILENAME",&filename);
	if(filename==NULL)
	{
		prterr("Error: No FILENAME specified.  This program will now close.");
		quitfunc(r);
	}
	fn=fopen(filename,"r");
	if(diagapps)
	{
		prterr("Made it past the file opening.\n");
	}
	if(fn==NULL) 
	{
		prterr("Error unable to open file [%s].",filename);
		return;
	}
	CHECK_MANAGEDID();
	if(UsingManagedID==0) 
	{
		prterr("Manage Personnel Identication in PRSNNL must be turned on!!!");
		return;
	}
	
	filenum=OPNNRD("PRSNNL","PERDMG");
	perdmgids=OPNNRD("PRSNNL","PERDMGIDS");
	perstat=OPNNRD("PRSNNL","PERSTAT");
	poslmst=OPNNRD("POSTRK","POSLMST");
	if(filenum==(-1))
	{
		prterr("Error: Couldn't open database PRSNNL->PERDMG.");
		return;
	} else if(fn!=NULL)
	{
		perid=Rmalloc(16);
		ssn=Rmalloc(12);
		ssn1=Rmalloc(4);
		ssn2=Rmalloc(4);
		ssn3=Rmalloc(4);
		fname=Rmalloc(26);
		mname=Rmalloc(26);
		aname=Rmalloc(26);
		lname=Rmalloc(26);
		add1=Rmalloc(41);
		city=Rmalloc(26);
		state=Rmalloc(26);
		zip=Rmalloc(11);
		padd=Rmalloc(41);
		pcity=Rmalloc(26);
		pstate=Rmalloc(26);
		pzip=Rmalloc(11);
		cphone=Rmalloc(13);
		aphone=Rmalloc(13);
		phone1=Rmalloc(13);
		phone2=Rmalloc(13);
		phone3=Rmalloc(13);
		gender=Rmalloc(26);
		ethnic=Rmalloc(26);
		empdate=Rmalloc(11);
		wphone=Rmalloc(13);
		hdate=Rmalloc(11);
		email=Rmalloc(41);
		dbirth=Rmalloc(11);
		FIELD_DELIMITER=stralloc("\t");
		RECORD_DELIMITER=stralloc("\n");
		while(true) 
		{
			importLine=readDelimitedline(fn,RECORD_DELIMITER,&len);
			if(importLine==NULL) break;
			unpad(importLine);
			if(!isEMPTY(importLine))
			{
				ptr=importLine;
				index = 0;
				exitstatus=FALSE;
				while(true)
				{
					fieldname=READIEfield(&ptr,FIELD_DELIMITER,TRUE,&exitstatus);
					stoupper(fieldname);
					fieldlist[index] = stralloc(fieldname);
					/*printf("fieldlist[%d] = %s\n",index,fieldlist[index]);*/
					if(fieldname!=NULL) Rfree(fieldname);
					if(exitstatus) break;
					index++;
				}
					ZERNRD(filenum);
					if(fieldlist[0]!=NULL)
					{
					memset(fname,0,26);
					memcpy(fname,fieldlist[0],25);
					unpad(fname);
					FINDFLDSETSTRING(filenum,"FIRST NAME",fname);
					}
					if(fieldlist[1]!=NULL)
					{
					memset(lname,0,26);
					memcpy(lname,fieldlist[1],25);
					unpad(lname);
					FINDFLDSETSTRING(filenum,"LAST NAME",lname);
					}
					if(fieldlist[2]!=NULL)
					{
					memset(mname,0,26);
					memcpy(mname,fieldlist[2],25);
					unpad(mname);
					FINDFLDSETSTRING(filenum,"MIDDLE NAME",mname);
					}
					if(fieldlist[3]!=NULL)
					{
					memset(aname,0,26);
					memcpy(aname,fieldlist[3],25);
					unpad(aname);
					FINDFLDSETSTRING(filenum,"ALTERNATE NAME",aname);
					}
					if(fieldlist[8]!=NULL)
					{
					memset(padd,0,41);
					memcpy(padd,fieldlist[8],40);
					unpad(padd);
					FINDFLDSETSTRING(filenum,"ADDRESS #1",padd);
					FINDFLDSETSTRING(filenum,"MAILING ADDRESS #1",padd);
					}
					if(fieldlist[9]!=NULL)
					{
					memset(pcity,0,26);
					memcpy(pcity,fieldlist[9],25);
					unpad(pcity);
					FINDFLDSETSTRING(filenum,"CITY",pcity);
					FINDFLDSETSTRING(filenum,"MAILING CITY",pcity);
					}
					if(fieldlist[10]!=NULL)
					{
					memset(pstate,0,26);
					memcpy(pstate,fieldlist[10],2);
					unpad(pstate);
					if(!strncmp(pstate,"VI",2))
					{
					sprintf(pstate,"%s","VA");
					}
					FINDFLDSETSTRING(filenum,"STATE",pstate);
					FINDFLDSETSTRING(filenum,"MAILING STATE",pstate);
					}
					if(fieldlist[11]!=NULL)
					{
					memset(pzip,0,11);
					memcpy(pzip,fieldlist[11],10);
					unpad(pzip);
					FINDFLDSETSTRING(filenum,"ZIP CODE",pzip);
					FINDFLDSETSTRING(filenum,"MAILING ZIP CODE",pzip);
					}
					if(fieldlist[12]!=NULL)
					{
					memset(cphone,0,13);
					memcpy(cphone,fieldlist[12],12);
					unpad(cphone);
					if(RDAstrlen(cphone)==10)
						{
						strncpy(phone1,cphone,3);
						strncpy(phone2,cphone+3,3);
						strncpy(phone3,cphone+6,4);
						sprintf(cphone,"%s-%s-%s",phone1,phone2,phone3);
						} 
					FINDFLDSETSTRING(filenum,"HOME PHONE",cphone);
					}
					if(fieldlist[13]!=NULL)
					{
					memset(aphone,0,13);
					memcpy(aphone,fieldlist[13],12);
					unpad(aphone);
					if(RDAstrlen(aphone)==10)
						{
						strncpy(phone1,aphone,3);
						strncpy(phone2,aphone+3,3);
						strncpy(phone3,aphone+6,4);
						sprintf(aphone,"%s-%s-%s",phone1,phone2,phone3);
						}
					FINDFLDSETSTRING(filenum,"CELL PHONE",aphone);
					}
					if(fieldlist[14]!=NULL)
					{
					memset(gender,0,26);
					memcpy(gender,fieldlist[14],15);
					unpad(gender);
						if(!strncmp(gender,"M",1))
						{
						FINDFLDSETINT(filenum,"SEX",1);
						} else {
						FINDFLDSETINT(filenum,"SEX",0);
						}
					}
					if(fieldlist[16]!=NULL)
					{
					memset(empdate,0,11);
					memcpy(empdate,fieldlist[16],10);
					unpad(empdate);
					FINDFLDSETSTRING(filenum,"ADJUSTED DOE",empdate);
					}
					if(fieldlist[18]!=NULL)
					{
					memset(hdate,0,11);
					memcpy(hdate,fieldlist[18],10);
					unpad(hdate);
					FINDFLDSETSTRING(filenum,"HIRE DATE",hdate);
					}
					if(fieldlist[19]!=NULL)
					{
					memset(email,0,41);
					memcpy(email,fieldlist[19],41);
					unpad(email);
					FINDFLDSETSTRING(filenum,"EMAIL ADDRESS",email);
					}
					if(fieldlist[20]!=NULL)
					{
					memset(ssn,0,12);
					memcpy(ssn,fieldlist[20],12);
					unpad(ssn);
					/*printf("RDAstrlen = %d\t",RDAstrlen(ssn));*/
					if(RDAstrlen(ssn)==9)
						{
						strncpy(ssn1,ssn,3);
						strncpy(ssn2,ssn+3,2);
						strncpy(ssn3,ssn+5,4);
						sprintf(ssn,"%s-%s-%s",ssn1,ssn2,ssn3);
						} 
					FINDFLDSETSTRING(filenum,"SOCIAL SECURITY NUMBER",ssn);
					}
					if(fieldlist[21]!=NULL)
					{
					memset(dbirth,0,11);
					memcpy(dbirth,fieldlist[21],12);
					unpad(dbirth);
					FINDFLDSETSTRING(filenum,"BIRTHDATE",(RDAstrlen(dbirth)>1 ? dbirth:""));
					}
					/*Do not create records for the CSV file column title*/
					if(strncmp(fname,"FIRST NAME",10))
					{
					perid=GetNextPID(perdmgids,NULL,SCRNvirtualSubData,r);
					FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid);
					FINDFLDSETSTRING(filenum,"STATE FILING STATUS","UNIFIED");
					FINDFLDSETSTRING(filenum,"PERSONNEL CLASS","N/A");
					FINDFLDSETCHAR(filenum,"CURRENT EMPLOYEE",TRUE);
					FINDFLDSETCHAR(filenum,"DELETEFLAG",FALSE);
					//prterr("Per ID: %s Name: %s %s %s SSN: %s",perid,fname,mname,lname,ssn);
					if(!isEMPTY(perid)) 
						{
						ADVWRTTRANSsec(filenum,0,NULL,prev,SCRNvirtualSubData,r);
						if(prev!=NULL) FreeRDATData(prev);
						}
					}

			}
			if(importLine!=NULL) Rfree(importLine);
		}
	}

	fclose(fn);
	if(perid!=NULL) Rfree(perid);
	if(fname!=NULL) Rfree(fname);
	if(lname!=NULL) Rfree(lname);
	if(mname!=NULL) Rfree(mname);
	if(aname!=NULL) Rfree(aname);
	if(aname!=NULL) Rfree(aname);
	if(add1!=NULL) Rfree(add1);
	if(city!=NULL) Rfree(city);
	if(state!=NULL) Rfree(state);
	if(zip!=NULL) Rfree(zip);
	if(padd!=NULL) Rfree(padd);
	if(pcity!=NULL) Rfree(pcity);
	if(pstate!=NULL) Rfree(pstate);
	if(pzip!=NULL) Rfree(pzip);
	if(cphone!=NULL) Rfree(cphone);
	if(aphone!=NULL) Rfree(aphone);
	if(phone1!=NULL) Rfree(phone1);
	if(phone2!=NULL) Rfree(phone2);
	if(phone3!=NULL) Rfree(phone3);
	if(gender!=NULL) Rfree(gender);
	if(ethnic!=NULL) Rfree(ethnic);
	if(empdate!=NULL) Rfree(empdate);
	if(wphone!=NULL) Rfree(wphone);
	if(hdate!=NULL) Rfree(hdate);
	if(email!=NULL) Rfree(email);
	if(ssn!=NULL) Rfree(ssn);
	if(dbirth!=NULL) Rfree(dbirth);
	if(rs!=NULL) FreeRangeScreen(rs);
	if(sortfile!=NULL) 
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
	}
	quitfunc(r);
}
void main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	//char *libx=NULL;
	//RDAGenericSetup *gsv=NULL;

	/*
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;


		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PRSNNL")+11);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("PRSNNL","MANAGE PERSONNEL IDENTIFICATIONS");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				managepid=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);
		prterr("In Main => Manage Personnel ID gsv = %d.",managepid);
		printf("In Main => Manage Personnel ID gsv = %d\n",managepid);
		CHECK_MANAGEDID();
		*/


	if(InitializeSubsystems(argc,argv,module,"PERDMG IMPORT DATA")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((perdmg=APPOPNNRD(module,"PERDMG",TRUE,TRUE))==(-1)) return;

	mainrsrc=RDArsrcNEW(module,"PERDMG IMPORT DATA");
	addstdrsrc(mainrsrc,"FILENAME",PLAINTEXT,80,&filename,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,importperdmg,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
