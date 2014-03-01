/* itronin.c */
#ifndef WIN32
#define __NAM__ "itronin.lnx"
#endif
#ifdef WIN32
#define __NAM__ "itronin.exe"
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

char *module="UTLBLL",*ITRON_FILENAME=NULL;
short utbcus=(-1),utbsvc=(-1),utbmtr=(-1),utbrte=(-1);
RDArsrc *MAINrsrc=NULL;

void quitfunc(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	mainrsrc=NULL;
	if(utbcus!=(-1)) CLSNRD(utbcus);
	if(utbsvc!=(-1)) CLSNRD(utbsvc);
	if(utbmtr!=(-1)) CLSNRD(utbmtr);
	if(utbrte!=(-1)) CLSNRD(utbrte);
	ShutdownSubsystems();
	exit(0);
}
double FixReading(long Reading,short dec_pos)
{
	int x=0,y=1;
	double d=0.0;

	for(x=0;x<dec_pos;++x) { y*=10;	}
	d=Reading/y;
	return(d);	
}
void ReadItron(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL;
	FILE *fp=NULL;
	char temp[129],custid[16],read_method=0,read_code=0;
	char new_service=FALSE,tamper[5],readdate[9],read_date[11];
	char tim[9],reseq=FALSE;
	RDATData *prev=NULL;
	long reading=0;
	int counters=0,serviceno=0,time_code=0,route=0;
	int total_time=0;
	short dec_pos=0,last_type=(-1),hrs=0,mins=0;
	double new_reading=0.0,seq1=0,new_sequence=0,sequence=0;

	readwidget(mainrsrc,"FILENAME");
	FINDRSCGETSTRING(mainrsrc,"FILENAME",&ITRON_FILENAME);
	counters=CountLines(ITRON_FILENAME);
	fp=fopen(ITRON_FILENAME,"r+b");
	if(fp==NULL)
	{
		prterr("Error Attempting to open and read file [%s] for import.",(ITRON_FILENAME!=NULL ? ITRON_FILENAME:""));
		return;
	}
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,
		"Importing Meter Readings",stop_diagnostic,counters);
	ZERNRD(utbsvc);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
		}
	}
	while(fgets(temp,128,fp))
	{
		if(abort_diagnostic) break;
		if(!strncmp(temp,"RHD",3))
		{
			if(temp[51]=='Y') reseq=TRUE;
				else reseq=FALSE;
		} else if(!strncmp(temp,"CUS",3))
		{
			new_service=FALSE;
			memcpy(custid,&temp[14],15);
			sprintf(stemp,"%-4s",&temp[30]);
			serviceno=atoi(stemp);
			if(temp[98]=='Y') new_service=TRUE;
				else new_service=FALSE;
			last_type=0;
		} else if(!strncmp(temp,"MTR",3))
		{
			sprintf(stemp,"%-8s",&temp[61]);
			sequence=atoi(stemp);
			sprintf(stemp,"%-8s",&temp[69]);
			new_sequence=atoi(stemp);
			sprintf(stemp,"%-3s",&temp[95]);
			time_code=atoi(stemp);
			total_time+=time_code;
			last_type=1;
		} else if(!strncmp(temp,"RTR",3))
		{
			sprintf(stemp,"%-6s",&temp[5]);
			route=atoi(stemp);
			ZERNRD(utbrte);
			FINDFLDSETINT(utbrte,"ROUTE NUMBER",route);
			if(!ADVEQLNRDsec(utbrte,1,SCRNvirtualSubData,mainrsrc))
			{
				prev=RDATDataNEW(utbrte);
			} else {
				if(prev!=NULL) FreeRDATData(prev);
				KEYNRD(utbrte,1);
			}
			memset(stemp,0,101);
			hrs=total_time/60;
			mins=(total_time-(hrs*60));
			sprintf(stemp,"%02d:%02d:00",hrs,mins);
			FINDFLDSETSTRING(utbrte,"TIME ALLOWED",stemp);
			ADVWRTTRANSsec(utbrte,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
			if(prev!=NULL) FreeRDATData(prev);
			memset(stemp,0,101);
		} else if(!strncmp(temp,"RDG",3))
		{
			read_method=temp[25];
			read_code=temp[61];
			memset(stemp,0,101);
			strncpy(stemp,&temp[33],10);
			reading=atol(stemp);
			sprintf(stemp,"%-2s",&temp[91]);
			dec_pos=atoi(stemp);
			new_reading=FixReading(reading,dec_pos);
			memcpy(readdate,&temp[47],8);
			sprintf(read_date,"%.2s/%.2s/%.4s",&readdate[0],&readdate[2],&readdate[4]);
			sprintf(tim,"%.2s:%.2s:%.2s",&temp[55],&temp[57],&temp[59]);
			if(read_code!='S' && read_code!=0)
			{
				if(read_method!='R' && read_method!='W')
				{
					if(last_type<2) /* Normal Reading */
					{
/* WRITE UTBSVC NORMAL */
						ZERNRD(utbsvc);
						FINDFLDSETSTRING(utbsvc,"CUSTOMER IDENTIFICATION",custid);
						FINDFLDSETINT(utbsvc,"SERVICE NUMBER",serviceno);
						if(!ADVEQLNRDsec(utbsvc,1,SCRNvirtualSubData,mainrsrc))
						{
							prev=RDATDataNEW(utbsvc);
							FINDFLDGETDOUBLE(utbsvc,"SEQUENCE NUMBER",&seq1);
							FINDFLDSETDOUBLE(utbsvc,"CURRENT READING",new_reading);
							FINDFLDSETSTRING(utbsvc,"LAST READ DATE",read_date);
#ifdef XXXX
							if(seq1!=new_sequence && reseq==TRUE)
							{
								FINDFLDSETCHAR(utbsvc,"RESEQUENCE",TRUE);
								FINDFLDSETDOUBLE(utbsvc,"NEW SEQUENCE NUMBER",new_sequence);
							}
#endif /* XXXX */
							ADVWRTTRANSsec(utbsvc,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
							if(prev!=NULL) FreeRDATData(prev);
						} else if(new_service==TRUE)
						{
							if(prev!=NULL) FreeRDATData(prev);
							KEYNRD(utbsvc,1);
							FINDFLDSETDOUBLE(utbsvc,"CURRENT READING",new_reading);
							FINDFLDSETSTRING(utbsvc,"LAST READ DATE",read_date);
#ifdef XXXX
							if(reseq==TRUE) FINDFLDSETDOUBLE(utbsvc,"SEQUENCE NUMBER",new_sequence);
#endif /* XXXX */
							ADVWRTTRANSsec(utbsvc,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
						}
					} else { /* Demand Reading */
/* WRITE UTBSVC DEMAND */
						ZERNRD(utbsvc);
						FINDFLDSETSTRING(utbsvc,"CUSTOMER IDENTIFICATION",custid);
						FINDFLDSETINT(utbsvc,"SERVICE NUMBER",serviceno);
						if(!ADVEQLNRDsec(utbsvc,1,SCRNvirtualSubData,mainrsrc))
						{
							prev=RDATDataNEW(utbsvc);
							FINDFLDSETDOUBLE(utbsvc,"DEMAND READING",new_reading);
							ADVWRTTRANSsec(utbsvc,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
							if(prev!=NULL) FreeRDATData(prev);
						} else if(new_service==TRUE)
						{
							if(prev!=NULL) FreeRDATData(prev);
							KEYNRD(utbsvc,1);
							FINDFLDSETDOUBLE(utbsvc,"DEMAND READING",new_reading);
							FINDFLDSETSTRING(utbsvc,"LAST READ DATE",read_date);
#ifdef XXXX
							if(reseq==TRUE) FINDFLDSETDOUBLE(utbsvc,"SEQUENCE NUMBER",new_sequence);
#endif /* XXXX */
							ADVWRTTRANSsec(utbsvc,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
						}
					}
				}
			}
			last_type=2;
		} else if(!strncmp(temp,"WRR",3))
		{
			if(read_code!='S' && read_code!=0)
			{
/* WRITE UTBSVC WAND */
				sprintf(tamper,"%.4s",&temp[54]);
				sprintf(stemp,"%-10s",&temp[72]);
				reading=atoi(stemp);
				new_reading=FixReading(reading,dec_pos);
				memcpy(readdate,&temp[58],8);
				sprintf(read_date,"%.2s/%.2s/%.4s",&readdate[0],&readdate[2],&readdate[4]);
				sprintf(tim,"%.2s:%.2s:%.2s",&temp[66],&temp[68],&temp[70]);
				ZERNRD(utbsvc);
				FINDFLDSETSTRING(utbsvc,"CUSTOMER IDENTIFICATION",custid);
				FINDFLDSETINT(utbsvc,"SERVICE NUMBER",serviceno);
				if(!ADVEQLNRDsec(utbsvc,1,SCRNvirtualSubData,mainrsrc))
				{
					prev=RDATDataNEW(utbsvc);
					FINDFLDGETDOUBLE(utbsvc,"SEQUENCE NUMBER",&seq1);
					FINDFLDSETDOUBLE(utbsvc,"CURRENT READING",new_reading);
					FINDFLDSETSTRING(utbsvc,"LAST READ DATE",read_date);
#ifdef XXXX
					if(seq1!=new_sequence && reseq==TRUE)
					{
						FINDFLDSETCHAR(utbsvc,"RESEQUENCE",TRUE);
						FINDFLDSETDOUBLE(utbsvc,"NEW SEQUENCE NUMBER",new_sequence);
					}
#endif /* XXXX */
					ADVWRTTRANSsec(utbsvc,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
					if(prev!=NULL) FreeRDATData(prev);
				} else if(new_service==TRUE)
				{
					if(prev!=NULL) FreeRDATData(prev);
					KEYNRD(utbsvc,1);
					FINDFLDSETDOUBLE(utbsvc,"CURRENT READING",new_reading);
					FINDFLDSETSTRING(utbsvc,"LAST READ DATE",read_date);
#ifdef XXXX
					if(reseq==TRUE) FINDFLDSETDOUBLE(utbsvc,"SEQUENCE NUMBER",new_sequence);
#endif /* XXXX */
					ADVWRTTRANSsec(utbsvc,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
				}
			}
		} else if(!strncmp(temp,"RFF",3))
		{
			if(read_code!='S' && read_code!=0)
			{
/* WRITE UTBSVC RADIO */
				sprintf(tamper,"%.2s",&temp[56]);
				sprintf(stemp,"%-10s",&temp[72]);
				reading=atoi(stemp);
				new_reading=FixReading(reading,dec_pos);
				memcpy(readdate,&temp[58],8);
				sprintf(read_date,"%.2s/%.2s/%.4s",&readdate[0],&readdate[2],&readdate[4]);
				sprintf(tim,"%.2s:%.2s:%.2s",&temp[66],&temp[68],&temp[70]);
				ZERNRD(utbsvc);
				FINDFLDSETSTRING(utbsvc,"CUSTOMER IDENTIFICATION",custid);
				FINDFLDSETINT(utbsvc,"SERVICE NUMBER",serviceno);
				if(!ADVEQLNRDsec(utbsvc,1,SCRNvirtualSubData,mainrsrc))
				{
					prev=RDATDataNEW(utbsvc);
					FINDFLDGETDOUBLE(utbsvc,"SEQUENCE NUMBER",&seq1);
					FINDFLDSETDOUBLE(utbsvc,"CURRENT READING",new_reading);
					FINDFLDSETSTRING(utbsvc,"LAST READ DATE",read_date);
#ifdef XXXX
					if(seq1!=new_sequence && reseq==TRUE)
					{
						FINDFLDSETCHAR(utbsvc,"RESEQUENCE",TRUE);
						FINDFLDSETDOUBLE(utbsvc,"NEW SEQUENCE NUMBER",new_sequence);
					}
#endif /* XXXX */
					ADVWRTTRANSsec(utbsvc,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
					if(prev!=NULL) FreeRDATData(prev);
				} else if(new_service==TRUE)
				{
					if(prev!=NULL) FreeRDATData(prev);
					KEYNRD(utbsvc,1);
					FINDFLDSETDOUBLE(utbsvc,"CURRENT READING",new_reading);
					FINDFLDSETSTRING(utbsvc,"LAST READ DATE",read_date);
#ifdef XXXX
					if(reseq==TRUE) FINDFLDSETDOUBLE(utbsvc,"SEQUENCE NUMBER",new_sequence);
#endif /* XXXX */
					ADVWRTTRANSsec(utbsvc,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
				}
			}
		} else { 
			/* Not used for import */
		}
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	quitfunc(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"ITRON IMPORT FILE")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((utbcus=APPOPNNRD(module,"UTBCUS",TRUE,FALSE))==(-1)) return;
	if((utbsvc=APPOPNNRD(module,"UTBSVC",TRUE,TRUE))==(-1)) return;
	if((utbmtr=APPOPNNRD(module,"UTBMTR",TRUE,FALSE))==(-1)) return;
	if((utbrte=APPOPNNRD(module,"UTBRTE",TRUE,TRUE))==(-1)) return;
	MAINrsrc=RDArsrcNEW(module,"ITRON IMPORT FILE");
	addDFincvir(MAINrsrc,module,"UTBCUS",NULL,utbcus);
	addDFincvir(MAINrsrc,module,"UTBSVC",NULL,utbsvc);
	addDFincvir(MAINrsrc,module,"UTBMTR",NULL,utbmtr);
	addDFincvir(MAINrsrc,module,"UTBRTE",NULL,utbrte);
	ITRON_FILENAME=stralloc("UPLOAD.DAT");
	addstdrsrc(MAINrsrc,"FILENAME",VARIABLETEXT,0,ITRON_FILENAME,TRUE);
	DefaultScreens(MAINrsrc);
	addbtnrsrc(MAINrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(MAINrsrc,"SELECT",TRUE,ReadItron,NULL);
	addrfexrsrc(MAINrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(MAINrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(MAINrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(MAINrsrc);
	APPmakescrn(MAINrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
