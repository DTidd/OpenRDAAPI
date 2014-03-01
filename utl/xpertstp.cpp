/* xpertstp.c - Xpert Setup */
#ifndef WIN32
#define __NAM__ "xpertstp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "xpertstp.exe"
#endif
#include <app.hpp>




#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


static char *module="UTILITIES",*devlicense=NULL;
XPERTstp *Xpert=NULL;
APPlib *software_types=NULL,*orgtypes=NULL,*AcctTypes=NULL;

void quitsetup(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc); 
	if(software_types!=NULL) freeapplib(software_types);
	if(orgtypes!=NULL) freeapplib(orgtypes);
	if(devlicense!=NULL) Rfree(devlicense);
	if(AcctTypes!=NULL) freeapplib(AcctTypes);
	ShutdownSubsystems();
}
void savesetup(RDArsrc *mainrsrc)
{
	int org_type=0,a_type=0;
	char value_char=FALSE;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"DEV_LICENSE",&devlicense);
	FINDRSCGETSTRING(mainrsrc,"ORGANIZATION NAME",&Xpert->company_name);
	FINDRSCGETSTRING(mainrsrc,"ADDRESS #1",&Xpert->addr1);
	FINDRSCGETSTRING(mainrsrc,"ADDRESS #2",&Xpert->addr2);
	FINDRSCGETSTRING(mainrsrc,"CITY",&Xpert->city);
	FINDRSCGETSTRING(mainrsrc,"STATE",&Xpert->state);
	FINDRSCGETSTRING(mainrsrc,"ZIP",&Xpert->zip);
	FINDRSCGETSTRING(mainrsrc,"PHONE",&Xpert->phone);
	FINDRSCGETSTRING(mainrsrc,"FAX",&Xpert->fax);
	FINDRSCGETCHAR(mainrsrc,"USE CENTURY THRESHOLD",&Xpert->use_century_threshold);
	FINDRSCGETSHORT(mainrsrc,"CENTURY THRESHOLD",&Xpert->century_threshold);
	FINDRSCGETINT(mainrsrc,"SOFTWARE TYPES",&Xpert->software_type);

	FINDRSCGETCHAR(mainrsrc,"UTILITIES",&Xpert->UTILITIES);
	FINDRSCGETCHAR(mainrsrc,"FINMGT",&Xpert->FINMGT);
	FINDRSCGETCHAR(mainrsrc,"BNKREC",&Xpert->BNKREC);
	FINDRSCGETCHAR(mainrsrc,"PRSNNL",&Xpert->PRSNNL);
	FINDRSCGETCHAR(mainrsrc,"IIDMGT",&Xpert->IIDMGT);
	FINDRSCGETCHAR(mainrsrc,"APPMGT",&Xpert->APPMGT);
	FINDRSCGETCHAR(mainrsrc,"POSTRK",&Xpert->POSTRK);
	FINDRSCGETCHAR(mainrsrc,"PAYROLL",&Xpert->PAYROLL);
	FINDRSCGETCHAR(mainrsrc,"SUBMGT",&Xpert->SUBMGT);
	FINDRSCGETCHAR(mainrsrc,"LVEMGT",&Xpert->LVEMGT);
	FINDRSCGETCHAR(mainrsrc,"EMPABS",&Xpert->EMPABS);
	FINDRSCGETCHAR(mainrsrc,"BFTMGT",&Xpert->BFTMGT);
	FINDRSCGETCHAR(mainrsrc,"PAYENC",&Xpert->PAYENC);
	FINDRSCGETCHAR(mainrsrc,"VENPMT",&Xpert->VENPMT);
	FINDRSCGETCHAR(mainrsrc,"PURORD",&Xpert->PURORD);
	FINDRSCGETCHAR(mainrsrc,"CATALOGUE",&Xpert->CATALOGUE);
	FINDRSCGETCHAR(mainrsrc,"REQMGT",&Xpert->REQMGT);
	FINDRSCGETCHAR(mainrsrc,"FIXASS",&Xpert->FIXASS);
	FINDRSCGETCHAR(mainrsrc,"INVCTL",&Xpert->INVCTL);
	FINDRSCGETCHAR(mainrsrc,"OCCTAX",&Xpert->OCCTAX);
	FINDRSCGETCHAR(mainrsrc,"BUDPREP",&Xpert->BUDPREP);
	FINDRSCGETCHAR(mainrsrc,"MISCBILL",&Xpert->MISCBILL);
	FINDRSCGETCHAR(mainrsrc,"TAXCLT",&Xpert->TAXCLT);
	FINDRSCGETCHAR(mainrsrc,"RLSTMGT",&Xpert->RLSTMGT);
	FINDRSCGETCHAR(mainrsrc,"PPTMGT",&Xpert->PPTMGT);
	FINDRSCGETCHAR(mainrsrc,"STUDFIN",&Xpert->STUDFIN);
	FINDRSCGETCHAR(mainrsrc,"VEHMTN",&Xpert->VEHMTN);
	FINDRSCGETCHAR(mainrsrc,"WRKORD",&Xpert->WRKORD);
	FINDRSCGETCHAR(mainrsrc,"TOOLS",&Xpert->TOOLS);
	FINDRSCGETCHAR(mainrsrc,"HRM",&Xpert->HRM);
	FINDRSCGETCHAR(mainrsrc,"CIS",&Xpert->CIS);
	FINDRSCGETCHAR(mainrsrc,"ASP",&Xpert->ASP);
	FINDRSCGETCHAR(mainrsrc,"BARCODING",&Xpert->BARCODING);
	FINDRSCGETCHAR(mainrsrc,"CREDITCARDS",&Xpert->CREDITCARDS);
	FINDRSCGETCHAR(mainrsrc,"FOODSVC",&Xpert->FOODSVC);
	FINDRSCGETCHAR(mainrsrc,"ROLLACCT",&Xpert->ROLLACCT);
	FINDRSCGETINT(mainrsrc,"ACCOUNTING METHODS",&a_type);
	Xpert->AccountingMethod=a_type;
	FINDRSCGETCHAR(mainrsrc,"TRANSACTIONS",&Xpert->TRANSACTIONS);
	FINDRSCGETCHAR(mainrsrc,"EXTENSIONS",&Xpert->EXTENSIONS);
	FINDRSCGETCHAR(mainrsrc,"SECURITYEXTENSIONS",&Xpert->SECURITYEXTENSIONS);
	FINDRSCGETCHAR(mainrsrc,"RLVEMGT",&Xpert->RLVEMGT);
	FINDRSCGETCHAR(mainrsrc,"RREQMGT",&Xpert->RREQMGT);
	FINDRSCGETCHAR(mainrsrc,"CSHDWR",&Xpert->CSHDWR);
	FINDRSCGETCHAR(mainrsrc,"BLDPRMT",&Xpert->BLDPRMT);
	FINDRSCGETCHAR(mainrsrc,"ARCHIVE",&Xpert->ARCHIVE);
	FINDRSCGETCHAR(mainrsrc,"CITWEB",&Xpert->CITWEB);
	FINDRSCGETCHAR(mainrsrc,"OPEN SOURCE",&Xpert->OPENSOURCE);
	FINDRSCGETCHAR(mainrsrc,"DMVREG",&Xpert->DMVREG);
	FINDRSCGETCHAR(mainrsrc,"VIOLATION",&Xpert->VIOLATION);
	FINDRSCGETCHAR(mainrsrc,"PROPERTY",&Xpert->PROPERTY);
	FINDRSCGETCHAR(mainrsrc,"UTLBLL",&Xpert->UTLBLL);
	FINDRSCGETCHAR(mainrsrc,"LVSIMP",&Xpert->LVSIMP);
	FINDRSCGETCHAR(mainrsrc,"SIMPLE MENU",&Xpert->SimpleMenu);
	FINDRSCGETINT(mainrsrc,"ORGANIZATION TYPES",&org_type);
	Xpert->OrgType=(short)org_type;

	FINDRSCGETCHAR(mainrsrc,"SHOW NEW IN DEFINE LIST",&value_char);
	Xpert->ShowDefineListNew=(value_char==FALSE?0:1);
	FINDRSCGETINT(mainrsrc,"NO USERS",&Xpert->NoUsers);
	FINDRSCGETCHAR(mainrsrc,"USE END DATE",&value_char);
	Xpert->UseEndDate=(value_char==FALSE?0:1);
	FINDRSCGETSTRING(mainrsrc,"END DATE",&Xpert->EndDate);
	FINDRSCGETSTRING(mainrsrc,"PROGRAM DIRECTORY",&Xpert->ProgDir);
	FINDRSCGETSTRING(mainrsrc,"DATA DIRECTORY",&Xpert->DataDir);
	FINDRSCGETSTRING(mainrsrc,"SOFTWARE KEY",&Xpert->SoftwareKey);

	writeXPERTbin(Xpert);
	quitsetup(mainrsrc);
}
static void setcenthresh(RDArsrc *mainrsrc)
{
	char ct=FALSE;
	
	readwidget(mainrsrc,"USE CENTURY THRESHOLD");
	FINDRSCGETCHAR(mainrsrc,"USE CENTURY THRESHOLD",&ct);
	if(ct==FALSE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"CENTURY THRESHOLD",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"CENTURY THRESHOLD",FALSE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"CENTURY THRESHOLD",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"CENTURY THRESHOLD",TRUE);
	}
}
static APPlib *makesoftwaretypes(void)
{
	APPlib *return_applib=NULL;
	
	return_applib=APPlibNEW();
	addAPPlib(return_applib,"ENTERPRISE");
	addAPPlib(return_applib,"OPENSOURCE");
	addAPPlib(return_applib,"MBGUI");
	return(return_applib);
}
static void setexpiredate(RDArsrc *mainrsrc)
{
	char bv=FALSE;
	
	readwidget(mainrsrc,"USE END DATE");
	FINDRSCGETCHAR(mainrsrc,"USE END DATE",&bv);
	if(bv==FALSE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"END DATE",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"END DATE",FALSE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"END DATE",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"END DATE",TRUE);
	}
}
static void setdevlicense(RDArsrc *r)
{
	short allow_add=FALSE;

	readwidget(r,"DEV_LICENSE");
	FINDRSCGETSTRING(r,"DEV_LICENSE",&devlicense);
	if(CheckDevLicense(devlicense))
	{
		allow_add=TRUE;
	} else {
		allow_add=FALSE;
	}

	FINDRSCSETEDITABLE(r,"UTILITIES",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"FINMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"BNKREC",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"PRSNNL",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"IIDMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"APPMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"POSTRK",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"PAYROLL",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"SUBMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"LVEMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"EMPABS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"BFTMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"PAYENC",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"VENPMT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"PURORD",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"CATALOGUE",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"REQMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"FIXASS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"INVCTL",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"OCCTAX",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"BUDPREP",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"MISCBILL",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"TAXCLT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"RLSTMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"PPTMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"STUDFIN",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"VEHMTN",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"WRKORD",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"TOOLS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"HRM",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"TRANSACTIONS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"EXTENSIONS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"RLVEMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"DMVREG",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"PROPERTY",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"VIOLATION",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"UTLBLL",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"LVSIMP",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"RREQMGT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"CSHDWR",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"BLDPRMT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"CIS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"ASP",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"BARCODING",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"CREDITCARDS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"FOODSVC",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"ROLLACCT",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"ACCOUNTING METHODS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"ARCHIVE",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"CITWEB",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"OPEN SOURCE",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"SIMPLE MENU",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"SOFTWARE TYPE",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"ORGANIZATION TYPE",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"SECURITYEXTENSIONS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"NO USERS",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"USE END DATE",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"END DATE",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"DATA DIRECTORY",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"PROGRAM DIRECTORY",(allow_add==TRUE ? TRUE:FALSE));
	FINDRSCSETEDITABLE(r,"SOFTWARE KEY",(allow_add==TRUE ? TRUE:FALSE));
	updateallrsrc(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *tmp=NULL,*temp1=NULL;
	int org_type=0,a_type=0;
	char value_char=FALSE;
	short allow_add=FALSE;
	short allow_add_license=FALSE;

	if(InitializeSubsystems(argc,argv,module,"XPERT SETUP")) 
	{
		return;
	}
	Xpert=XPERTstpNEW();
	getXPERTbin(Xpert);
	tmp=RDA_GetEnv("DEV_LICENSE");
	if(tmp!=NULL)
	{
		allow_add=FALSE;
		allow_add_license=FALSE;
		temp1=strtok(tmp," ");
		devlicense=stralloc(temp1);
		if(CheckDevLicense(devlicense))
		{
			allow_add=TRUE;
			allow_add_license=TRUE;
		}
	} else {
		allow_add=FALSE;
		allow_add_license=FALSE;
	}
	mainrsrc=RDArsrcNEW(module,"XPERT SETUP");
	addstdrsrc(mainrsrc,"DEV_LICENSE",VARIABLETEXT,0,devlicense,TRUE);
	FINDRSCSETFUNC(mainrsrc,"DEV_LICENSE",setdevlicense,NULL);
	addstdrsrc(mainrsrc,"ORGANIZATION NAME",VARIABLETEXT,0,Xpert->company_name,TRUE);
	addstdrsrc(mainrsrc,"ADDRESS #1",VARIABLETEXT,0,Xpert->addr1,TRUE);
	addstdrsrc(mainrsrc,"ADDRESS #2",VARIABLETEXT,0,Xpert->addr2,TRUE);
	addstdrsrc(mainrsrc,"CITY",VARIABLETEXT,0,Xpert->city,TRUE);
	addstdrsrc(mainrsrc,"STATE",VARIABLETEXT,2,Xpert->state,TRUE);
	addstdrsrc(mainrsrc,"ZIP",ZIPCODE,10,Xpert->zip,TRUE);
	addstdrsrc(mainrsrc,"PHONE",PHONE,12,Xpert->phone,TRUE);
	addstdrsrc(mainrsrc,"FAX",PHONE,12,Xpert->fax,TRUE);
	addstdrsrc(mainrsrc,"USE CENTURY THRESHOLD",BOOLNS,1,&Xpert->use_century_threshold,TRUE);
	FINDRSCSETFUNC(mainrsrc,"USE CENTURY THRESHOLD",setcenthresh,NULL);
	addstdrsrc(mainrsrc,"CENTURY THRESHOLD",SHORTV,2,&Xpert->century_threshold,TRUE);

/* Modules settings for Setup Wizard */
        addstdrsrc(mainrsrc,"UTILITIES",BOOLNS,1,&Xpert->UTILITIES,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"FINMGT",BOOLNS,1,&Xpert->FINMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"BNKREC",BOOLNS,1,&Xpert->BNKREC,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"PRSNNL",BOOLNS,1,&Xpert->PRSNNL,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"IIDMGT",BOOLNS,1,&Xpert->IIDMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"APPMGT",BOOLNS,1,&Xpert->APPMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"POSTRK",BOOLNS,1,&Xpert->POSTRK,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"PAYROLL",BOOLNS,1,&Xpert->PAYROLL,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"SUBMGT",BOOLNS,1,&Xpert->SUBMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"LVEMGT",BOOLNS,1,&Xpert->LVEMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"EMPABS",BOOLNS,1,&Xpert->EMPABS,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"BFTMGT",BOOLNS,1,&Xpert->BFTMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"PAYENC",BOOLNS,1,&Xpert->PAYENC,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"VENPMT",BOOLNS,1,&Xpert->VENPMT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"PURORD",BOOLNS,1,&Xpert->PURORD,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"CATALOGUE",BOOLNS,1,&Xpert->CATALOGUE,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"REQMGT",BOOLNS,1,&Xpert->REQMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"FIXASS",BOOLNS,1,&Xpert->FIXASS,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"INVCTL",BOOLNS,1,&Xpert->INVCTL,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"OCCTAX",BOOLNS,1,&Xpert->OCCTAX,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"BUDPREP",BOOLNS,1,&Xpert->BUDPREP,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"MISCBILL",BOOLNS,1,&Xpert->MISCBILL,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"TAXCLT",BOOLNS,1,&Xpert->TAXCLT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"RLSTMGT",BOOLNS,1,&Xpert->RLSTMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"PPTMGT",BOOLNS,1,&Xpert->PPTMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"STUDFIN",BOOLNS,1,&Xpert->STUDFIN,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"VEHMTN",BOOLNS,1,&Xpert->VEHMTN,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"WRKORD",BOOLNS,1,&Xpert->WRKORD,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"TOOLS",BOOLNS,1,&Xpert->TOOLS,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"HRM",BOOLNS,1,&Xpert->HRM,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"TRANSACTIONS",BOOLNS,1,&Xpert->TRANSACTIONS,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"EXTENSIONS",BOOLNS,1,&Xpert->EXTENSIONS,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"RLVEMGT",BOOLNS,1,&Xpert->RLVEMGT,(allow_add==TRUE ? TRUE:FALSE));
        addstdrsrc(mainrsrc,"DMVREG",BOOLNS,1,&Xpert->DMVREG,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"PROPERTY",BOOLNS,1,&Xpert->PROPERTY,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"VIOLATION",BOOLNS,1,&Xpert->VIOLATION,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"UTLBLL",BOOLNS,1,&Xpert->UTLBLL,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"LVSIMP",BOOLNS,1,&Xpert->LVSIMP,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"RREQMGT",BOOLNS,1,&Xpert->RREQMGT,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"CSHDWR",BOOLNS,1,&Xpert->CSHDWR,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"BLDPRMT",BOOLNS,1,&Xpert->BLDPRMT,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"ARCHIVE",BOOLNS,1,&Xpert->ARCHIVE,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"CITWEB",BOOLNS,1,&Xpert->CITWEB,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"CIS",BOOLNS,1,&Xpert->CIS,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"ASP",BOOLNS,1,&Xpert->ASP,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"BARCODING",BOOLNS,1,&Xpert->BARCODING,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"CREDITCARDS",BOOLNS,1,&Xpert->CREDITCARDS,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"FOODSVC",BOOLNS,1,&Xpert->FOODSVC,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"ROLLACCT",BOOLNS,1,&Xpert->ROLLACCT,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"OPEN SOURCE",BOOLNS,1,&Xpert->OPENSOURCE,(allow_add==TRUE ? TRUE:FALSE));
	addstdrsrc(mainrsrc,"SIMPLE MENU",BOOLNS,1,&Xpert->SimpleMenu,TRUE);
	software_types=makesoftwaretypes();
	addlstrsrc(mainrsrc,"SOFTWARE TYPES",&Xpert->software_type,(allow_add==TRUE ? TRUE:FALSE),NULL,software_types->numlibs,&software_types->libs,NULL);
        addstdrsrc(mainrsrc,"SECURITYEXTENSIONS",BOOLNS,1,&Xpert->SECURITYEXTENSIONS,(allow_add==TRUE ? TRUE:FALSE));
	AcctTypes=APPlibNEW();
	addAPPlib(AcctTypes,"RDA Standard Practice");
	addAPPlib(AcctTypes,"ALL Dimensions All Types");
	addAPPlib(AcctTypes,"RDA Modified Practice");
	a_type=Xpert->AccountingMethod;
	addlstrsrc(mainrsrc,"ACCOUNTING METHODS",&a_type,(allow_add==TRUE ? TRUE:FALSE),NULL,AcctTypes->numlibs,&AcctTypes->libs,NULL);

	orgtypes=APPlibNEW();
	addAPPlib(orgtypes,"School District");
	addAPPlib(orgtypes,"City/Village/Municipality");
	addAPPlib(orgtypes,"County Government");
	addAPPlib(orgtypes,"Other Non-Profit");
	addAPPlib(orgtypes,"No Subsidiary/Profit/Loss");
	org_type=Xpert->OrgType;
	addlstrsrc(mainrsrc,"ORGANIZATION TYPES",&org_type,(allow_add==TRUE ? TRUE:FALSE),NULL,orgtypes->numlibs,&orgtypes->libs,NULL);

	value_char=(char)Xpert->ShowDefineListNew;
	addstdrsrc(mainrsrc,"SHOW NEW IN DEFINE LIST",BOOLNS,1,&value_char,TRUE);;
	addstdrsrc(mainrsrc,"NO USERS",LONGV,10,&Xpert->NoUsers,(allow_add_license==TRUE?TRUE:FALSE));
	value_char=(char)Xpert->UseEndDate;
	addstdrsrc(mainrsrc,"USE END DATE",BOOLNS,1,&value_char,(allow_add_license==TRUE?TRUE:FALSE));
	FINDRSCSETFUNC(mainrsrc,"USE END DATE",setexpiredate,NULL);
	addstdrsrc(mainrsrc,"END DATE",DATES,10,Xpert->EndDate,(allow_add_license==TRUE?TRUE:FALSE));
	addstdrsrc(mainrsrc,"PROGRAM DIRECTORY",VARIABLETEXT,0,Xpert->ProgDir,(allow_add==TRUE?TRUE:FALSE));
	addstdrsrc(mainrsrc,"DATA DIRECTORY",VARIABLETEXT,0,Xpert->DataDir,(allow_add==TRUE?TRUE:FALSE));
	addstdrsrc(mainrsrc,"SOFTWARE KEY",VARIABLETEXT,0,Xpert->SoftwareKey,TRUE);

	addrfexrsrc(mainrsrc,"SAVE",TRUE,savesetup,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitsetup,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setcenthresh(mainrsrc);
	setexpiredate(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitsetup,NULL,TRUE);
}
