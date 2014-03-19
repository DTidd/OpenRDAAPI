/****************************************************************************
	  		   FIELDTEST PROGRAM
		 		fldtest.c 

*****************************************************************************/
#include <mix.hpp>
#ifndef WIN32
#define __NAM__ "fldtest.lnx"
#endif
#ifdef WIN32
#define __NAM__ "fldtest.exe"
#endif
#include <app.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

// define _TEST_MYISAM_

static void quit_record(RDArsrc *);
static void read_widgets(RDArsrc *);
static void update_widgets(RDArsrc *);
static char *module="UTILITIES";
static char sensitive=FALSE;

static void changesensitive(RDArsrc *mainrsrc)
{
	FINDRSCSETSENSITIVE(mainrsrc,"READ WIDGETS",(sensitive ? FALSE:TRUE));
	FINDRSCSETSENSITIVE(mainrsrc,"UPDATE WIDGETS",(sensitive ? FALSE:TRUE));
	if(sensitive)
	{
		sensitive=FALSE;
	} else {
		sensitive=TRUE;
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int length=20;
	char boolnvalue=FALSE;
	short sv=25;
#ifdef _TEST_MYISAM_
	short filenum=(-1),venmsr=(-1),venjrnl=(-1),ef=0;
	char *journalid=NULL;
	char *vendorid=NULL;
#endif /* _TEST_MYISAM_ */
	char *temp=NULL,*mydate=NULL;

	if(InitializeSubsystems(argc,argv,"UTILITIES","FIELD TEST"))
	{
		ShutdownSubsystems();
		return(1);
	}
#ifdef _TEST_MYISAM_
// Adding an open to VENPMT -> VENJRNL and connecting it to the VENMSR loop in field test should cause a failure... that's basically what's happening in the browse.  Let me know if you want me to pound that code frag.... may do it anyway depending on the day.


	filenum=OPNNRDsec("FINMGT","FINEACC",TRUE,TRUE,FALSE);
	if(filenum!=(-1))
	{
		prterr("SIZNRD [%d]\n",SIZNRD(filenum));
		ef=FRSNRD(filenum,1);
		while(!ef)
		{
			//SEENRDRECORD(filenum);TRACE;
			ef=NXTNRD(filenum,1);
		}
		CLSNRD(filenum);
		filenum=(-1);
	}

	venmsr=OPNNRDsec("VENPMT","VENMSR",TRUE,TRUE,FALSE);
	if(venmsr!=(-1))
	{
		prterr("SIZNRD [%d]\n",SIZNRD(venmsr));
		ef=FRSNRD(venmsr,1);
		
		venjrnl=OPNNRDsec("VENPMT","VENJRNL",TRUE,TRUE,FALSE);
		while(!ef)
		{
			//SEENRDRECORD(venmsr);TRACE;
			FINDFLDGETSTRING(venmsr,"VENDOR IDENTIFICATION",&vendorid);
			FINDFLDGETSTRING(venmsr,"JOURNAL IDENTIFICATION",&journalid);
			prterr("Vendor ID [%s]    Journal ID [%s]",vendorid,journalid);

			if(venjrnl!=(-1))
			{
				prterr("Vendor ID [%s]    Journal ID [%s]",vendorid,journalid);
				FINDFLDSETSTRING(venjrnl,"JOURNAL IDENTIFICATION",journalid);
				if(!EQLNRD(venjrnl,1))
				{
					SEENRDRECORD(venjrnl);
				}
			}else{
				prterr("Error Opening Security File For Journal File.\n");
			}
			ef=NXTNRD(venmsr,1);
		}
		CLSNRD(venjrnl);
		venjrnl=(-1);
		CLSNRD(venmsr);
		venmsr=(-1);
	}else{
		prterr("ERROR:  Could not open Vendor Master File.");
	}




#endif /* _TEST_MYISAM_ */
	mainrsrc=RDArsrcNEW(module,"FIELD TEST SCREEN");
	addstdrsrc(mainrsrc,"PLAINTEXT",PLAINTEXT,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"VARIABLETEXT",VARIABLETEXT,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"SCROLLEDTEXT",SCROLLEDTEXT,length*100,NULL,TRUE);
	addstdrsrc(mainrsrc,"PHONE",PHONE,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"SOCSECNUM",SOCSECNUM,length,NULL,TRUE);
	mydate=GETCURRENTDATE10();
	fprintf(RDA_STDERR,"mydate [%s] ",(mydate!=NULL ? mydate:""));TRACE;
	addstdrsrc(mainrsrc,"DATE",DATES,length,mydate,TRUE);
	addstdrsrc(mainrsrc,"TIME",TIMES,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"ZIP",ZIPCODE,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"EXPENDITURE",EXPENDITURE,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"REVENUE",REVENUE,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"BALANCE SHEET",BALANCESHEET,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"BEGINNING BALANCE",BEGINNINGBALANCE,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"CUSTOM TYPE",CUSTOMTYPE,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"DOLLARS",DOLLARS,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"DOLLARS NO CENTS",DOLLARS_NOCENTS,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"SHORTV",SHORTV,length,&sv,TRUE);
	addstdrsrc(mainrsrc,"SSHORTV",SSHORTV,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"LONGV",LONGV,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"SLONGV",SLONGV,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"DOUBLEV",DOUBLEV,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"SDOUBLEV",SDOUBLEV,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"DECIMALV",DECIMALV,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"SDECIMALV",SDECIMALV,length,NULL,TRUE);
	addstdrsrc(mainrsrc,"BOOLEAN",BOOLNS,1,&boolnvalue,TRUE);
	addbtnrsrc(mainrsrc,"SENSITIVE",TRUE,changesensitive,NULL);
	addrfcbrsrcnh(mainrsrc,"READ WIDGETS",TRUE,read_widgets,mainrsrc);
	addrfcbrsrcnh(mainrsrc,"UPDATE WIDGETS",TRUE,update_widgets,mainrsrc);
	addrfexrsrcnh(mainrsrc,"QUIT",TRUE,quit_record,NULL);
	addrfcbrsrcnh(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	FINDRSCSETSENSITIVE(mainrsrc,"READ WIDGETS",FALSE);
	FINDRSCSETSENSITIVE(mainrsrc,"UPDATE WIDGETS",FALSE);
	FINDRSCSETSTRING(mainrsrc,"DATE",mydate);
	APPmakescrn(mainrsrc,FALSE,quit_record,NULL,TRUE);
#ifdef _TEST_MYISAM_
/*
	filenum=OPNNRDsec("VENPMT","VENMSR",TRUE,TRUE,FALSE);
	if(filenum!=(-1))
	{
		prterr("SIZNRD [%d]\n",SIZNRD(filenum));
		ef=FRSNRD(filenum,1);
		while(!ef)
		{
			SEENRDRECORD(filenum);TRACE;
			ef=NXTNRD(filenum,1);
		}
		CLSNRD(filenum);
		filenum=(-1);
	}
*/

	venmsr=OPNNRDsec("VENPMT","VENMSR",TRUE,TRUE,FALSE);
	if(venmsr!=(-1))
	{
		prterr("SIZNRD [%d]\n",SIZNRD(venmsr));
		ef=FRSNRD(venmsr,1);
		
		venjrnl=OPNNRDsec("VENPMT","VENJRNL",TRUE,TRUE,FALSE);
		while(!ef)
		{
			//SEENRDRECORD(venmsr);TRACE;
			FINDFLDGETSTRING(venmsr,"VENDOR IDENTIFICATION",&vendorid);
			FINDFLDGETSTRING(venmsr,"JOURNAL IDENTIFICATION",&journalid);
			prterr("Vendor ID [%s]    Journal ID [%s]",vendorid,journalid);

			if(venjrnl!=(-1))
			{
				prterr("Vendor ID [%s]    Journal ID [%s]",vendorid,journalid);
				FINDFLDSETSTRING(venjrnl,"JOURNAL IDENTIFICATION",journalid);
				if(!EQLNRD(venjrnl,1))
				{
					SEENRDRECORD(venjrnl);
				}
			}else{
				prterr("Error Opening Security File For Journal File.\n");
			}
			ef=NXTNRD(venmsr,1);
		}
		CLSNRD(venjrnl);
		venjrnl=(-1);
		CLSNRD(venmsr);
		venmsr=(-1);
	}else{
		prterr("ERROR:  Could not open Vendor Master File.");
	}
#endif /* _TEST_MYISAM_ */
#ifdef _TEST_MYISAM_
/*
	filenum=OPNNRDsec("FINMGT","FINEYR",TRUE,TRUE,FALSE);
	if(filenum!=(-1))
	{
		prterr("SIZNRD [%d]\n",SIZNRD(filenum));
		ef=FRSNRD(filenum,1);
		while(!ef)
		{
			SEENRDRECORD(filenum);TRACE;
			ef=NXTNRD(filenum,1);
		}
		CLSNRD(filenum);
		filenum=(-1);

	}
*/

	WT_RDA_SetEnv("RDA_N1","RDA_VALUE2",__LINE__,__FILE__);
	temp=WT_RDA_GetEnv("RDA_N1",__LINE__,__FILE__);
	printf("DIAG:  value[%s]\n",temp);
	venmsr=OPNNRDsec("VENPMT","VENMSR",TRUE,TRUE,FALSE);
	if(venmsr!=(-1))
	{
		prterr("SIZNRD [%d]\n",SIZNRD(venmsr));
		ef=FRSNRD(venmsr,1);
		
		venjrnl=OPNNRDsec("VENPMT","VENJRNL",TRUE,TRUE,FALSE);
		while(!ef)
		{
			//SEENRDRECORD(venmsr);TRACE;
			FINDFLDGETSTRING(venmsr,"VENDOR IDENTIFICATION",&vendorid);
			FINDFLDGETSTRING(venmsr,"JOURNAL IDENTIFICATION",&journalid);
			prterr("Vendor ID [%s]    Journal ID [%s]",vendorid,journalid);

			if(venjrnl!=(-1))
			{
				prterr("Vendor ID [%s]    Journal ID [%s]",vendorid,journalid);
				FINDFLDSETSTRING(venjrnl,"JOURNAL IDENTIFICATION",journalid);
				if(!EQLNRD(venjrnl,1))
				{
					SEENRDRECORD(venjrnl);
				}
			}else{
				prterr("Error Opening Security File For Journal File.\n");
			}
			ef=NXTNRD(venmsr,1);
		}
		CLSNRD(venjrnl);
		venjrnl=(-1);
		CLSNRD(venmsr);
		venmsr=(-1);
	}else{
		prterr("ERROR:  Could not open Vendor Master File.");
	}
#endif /* _TEST_MYISAM_ */
}
static void read_widgets(RDArsrc *mainrsrc)
{
	char *value=NULL;
	double doublevalue=0.0;
	int intvalue=0;
	short shortvalue=0;
	char charvalue=0;

	prterr("Reading all widgets!:\n");
	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"PLAINTEXT",&value);
	prterr("\tPLAIN TEXT=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"VARIABLETEXT",&value);
	prterr("\tVARIABLE TEXT=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"PHONE",&value);
	prterr("\tPHONE=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"SOCSECNUM",&value);
	prterr("\tSSN=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"ZIP",&value);
	prterr("\tZIP=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"CUSTOM TYPE",&value);
	prterr("\tCUSTOM TYPE=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"EXPENDITURE",&value);
	prterr("\tEXPENDITURE=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"REVENUE",&value);
	prterr("\tREVENUE=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"BALANCE SHEET",&value);
	prterr("\tBALANCE SHEET=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"BEGINNING BALANCE",&value);
	prterr("\tBEGINNING BALANCE=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"DATE",&value);
	prterr("\tDATE=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"TIME",&value);
	prterr("\tTIME=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSTRING(mainrsrc,"SCROLLEDTEXT",&value);
	prterr("\tSCROLLEDTEXT=[%s]\n",(value!=NULL?value:""));
	if(value!=NULL) Rfree(value);
	FINDRSCGETSHORT(mainrsrc,"SHORTV",&shortvalue);
	prterr("\tSHORTV=[%d]\n",shortvalue);
	FINDRSCGETSHORT(mainrsrc,"SSHORTV",&shortvalue);
	prterr("\tSSHORTV=[%d]\n",shortvalue);
	FINDRSCGETINT(mainrsrc,"LONGV",&intvalue);
	prterr("\tLONGV=[%d]\n",intvalue);
	FINDRSCGETINT(mainrsrc,"SLONGV",&intvalue);
	prterr("\tSLONGV=[%d]\n",intvalue);
	FINDRSCGETDOUBLE(mainrsrc,"DOLLARS",&doublevalue);
	prterr("\tDOLLARS=[$%.02f]\n",(doublevalue/100));
	FINDRSCGETDOUBLE(mainrsrc,"DOLLARS NO CENTS",&doublevalue);
	prterr("\tDOLLARS NO CENTS=[$%f]\n",doublevalue);
	FINDRSCGETDOUBLE(mainrsrc,"DOUBLEV",&doublevalue);
	prterr("\tDOUBLEV=[%f]\n",doublevalue);
	FINDRSCGETDOUBLE(mainrsrc,"SDOUBLEV",&doublevalue);
	prterr("\tSDOUBLEV=[%f]\n",doublevalue);
	FINDRSCGETDOUBLE(mainrsrc,"DECIMALV",&doublevalue);
	prterr("\tDECIMALV=[%f]\n",doublevalue);
	FINDRSCGETDOUBLE(mainrsrc,"SDECIMALV",&doublevalue);
	prterr("\tSDECIMALV=[%f]\n",doublevalue);
	FINDRSCGETCHAR(mainrsrc,"BOOLEAN",&charvalue);
	prterr("\tBOOLEAN=[%s]\n",(charvalue==TRUE?"TRUE":"FALSE"));
}
static void update_widgets(RDArsrc *mainrsrc)
{
	prterr("Updateing all widgets!\n");
	updateallrsrc(mainrsrc);
}
static void quit_record(RDArsrc *mainrsrc)
{
	TRACE;
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	TRACE;
	ShutdownSubsystems();
	TRACE;
}
