/* setupsftp.c - Setup SFTP Services */
#ifndef WIN32
#define __NAM__ "setupsftp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "setupsftp.exe"
#endif
#include <app.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
/*ADDLIB rpt */

static char *module="UTILITIES";
static char *work_login=NULL,*work_passwd=NULL,*work_documents=NULL;
static char *work_ip=NULL;
static short Users=(-1);
static APPlib *Types=NULL;

static void quitprt(RDArsrc *mainrsrc)
{
	if(work_login!=NULL) Rfree(work_login);
	if(work_passwd!=NULL) Rfree(work_passwd)
	if(work_documents!=NULL) Rfree(work_documents);
	if(work_ip!=NULL) Rfree(work_ip);
	if(Types!=NULL) freeapplib(Types);
	if(Users!=(-1)) CLSNRD(Users);
	if(mainrsrc!=NULL)
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	ShutdownSubsystems();
	std::exit;
}
static void workstationvalidate(RDArsrc *mainrsrc)
{
	FILE *fp=NULL;
	APPlib *args=NULL;
	RDATData *prev=NULL;

	memset(stemp,0,101);
	sprintf(stemp,"validate_%s.txt",USERLOGIN);
	fp=fopen(stemp,"w+b");
	if(fp!=NULL)
	{
		fprintf(fp,"VALIDATING %s's WORKSTATION.",USERLOGIN);
		fclose(fp);
	}
	args=APPlibNEW();
	addAPPlib(args,"\"\"");
	addAPPlib(args,stemp);
	addAPPlib(args,work_ip);
	addAPPlib(args,work_documents);
	addAPPlib(args,work_login);
	addAPPlib(args,work_passwd);
	ADVExecute2Program("sftpfile",args,NULL);
	if(args!=NULL) freeapplib(args);
	unlink(stemp);
	args=APPlibNEW();
	addAPPlib(args,"\"\"");
	addAPPlib(args,stemp);
	addAPPlib(args,work_ip);
	addAPPlib(args,work_documents);
	addAPPlib(args,work_login);
	addAPPlib(args,work_passwd);
	addAPPlib(args,"1");
	memset(stemp,0,101);
	sprintf(stemp,"validate_%s.txt",USERLOGIN);
	addAPPlib(args,stemp);
	ADVExecute2Program("sftpfile",args,NULL);
	if(args!=NULL) freeapplib(args);
	memset(stemp,0,101);
	sprintf(stemp,"validate_%s.txt",USERLOGIN);
	fp=fopen(stemp,"r+b");
	if(fp==NULL)
	{
		unlink(stemp);
		ERRORDIALOG("WORKSTATION SETUP FAILED","Based on the information provided and the SFTP Server Server / Sharing Remote Login (Apple's Mac) settings your workstation is unable to be validated.  Please review your workstation and network setup (including firewall rules, etc) and try again.",NULL,FALSE);
		return;
	} else {
		fclose(fp);
	}
	if((access(stemp,R_OK))==0)
	{	
		unlink(stemp);
		ZERNRD(Users);
		FINDFLDSETSTRING(Users,"USER IDENTIFICATION",USERLOGIN);
		if(EQLNRD(Users,1)) KEYNRD(Users,1);
			else prev=RDATDataNEW(Users);
		FINDFLDSETCHAR(Users,"WORKSTATION VALIDATED",TRUE);
		FINDFLDSETSTRING(Users,"WORKSTATION LOGIN",work_login);
		FINDFLDSETSTRING(Users,"WORKSTATION PASSWORD",work_passwd);
		FINDFLDSETSTRING(Users,"WORKSTATION DOCUMENTS",work_documents);
		WRTTRANS(Users,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		args=APPlibNEW();
		addAPPlib(args,"WORKSTATION VALIDATED");
		addAPPlib(args,"Your workstation has been validated for use downloading from F.A.S.T.'s Archived Documents.");
		ExecuteProgram("ok",args);
		if(args!=NULL) freeapplib(args);
	} else {
		unlink(stemp);
		ERRORDIALOG("WORKSTATION SETUP FAILED","Based on the information provided and the SFTP Server Server / Sharing Remote Login (Apple's Mac) settings your workstation is unable to be validated.  Please review your workstation and network setup (including firewall rules, etc) and try again.",NULL,FALSE);
	}
}
static void test_values(RDArsrc *mainrsrc)
{
	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"WORKSTATION IP ADDRESS",&work_ip);
	FINDRSCGETSTRING(mainrsrc,"WORKSTATION LOGIN",&work_login);
	FINDRSCGETSTRING(mainrsrc,"WORKSTATION PASSWORD",&work_passwd);
	FINDRSCGETSTRING(mainrsrc,"WORKSTATION DOCUMENTS",&work_documents);
	if(isEMPTY(work_login) || isEMPTY(work_passwd) || isEMPTY(work_documents) || isEMPTY(work_ip)) 
	{
		ERRORDIALOG("WORKSTATION SETUP ERROR","You must provide the workstation's ip address, login, password, and documents folder in order to validate the workstation for downloading archives.",NULL,FALSE);
	} else {
		workstationvalidate(mainrsrc);
	}
}
static void WorkStationTypeCB(RDArsrc *mainrsrc)
{
	int x=0;

	readwidget(mainrsrc,"WORKSTATION LOGIN");
	FINDRSCGETSTRING(mainrsrc,"WORKSTATION LOGIN",&work_login);
	if(!isEMPTY(work_login))
	{
		FINDRSCGETINT(mainrsrc,"WORKSTATION TYPES",&x);
		memset(stemp,0,101);
		switch(x)
		{
			default:
			case 0:
				sprintf(stemp,"C:/Users/%s/My Documents",(work_login!=NULL ? work_login:""));
				break;
			case 1:
				sprintf(stemp,"/Users/%s/Documents",(work_login!=NULL ? work_login:""));
				break;
			case 2:
				sprintf(stemp,"/home/%s",(work_login!=NULL ? work_login:""));
				break;
		}
		FINDRSCSETSTRING(mainrsrc,"WORKSTATION DOCUMENTS",stemp);
		updatersrc(mainrsrc,"WORKSTATION DOCUMENTS");
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"WORKSTATION VALIDATION")) 
	{
		return;
	}
	Users=OPNNRD("SECURITY","USERS");
	if(Users==(-1))
	{
		ERRORDIALOG("SECURITY USERS UNAVAILABLE","Am unable to open the Security -> Users database.",NULL,TRUE);
		return;
	}
	Types=APPlibNEW();
	addAPPlib(Types,"Microsoft Windows");
	addAPPlib(Types,"Apple's Mac");
	addAPPlib(Types,"Linux / Ubuntu / Unix");
	work_ip=WorkStationIPAddr();
	mainrsrc=RDArsrcNEW(module,"WORKSTATION VALIDATION");
	addstdrsrc(mainrsrc,"WORKSTATION IP ADDRESS",PLAINTEXT,40,work_ip,TRUE);
	addstdrsrc(mainrsrc,"WORKSTATION LOGIN",PLAINTEXT,40,work_login,TRUE);
	FINDRSCSETFUNC(mainrsrc,"WORKSTATION LOGIN",WorkStationTypeCB,NULL);
	addstdrsrc(mainrsrc,"WORKSTATION PASSWORD",PLAINTEXT,25,work_passwd,TRUE);
	x=0;
	addlstrsrc(mainrsrc,"WORKSTATION TYPES",&x,TRUE,WorkStationTypeCB,Types->numlibs,&Types->libs,NULL);
	addstdrsrc(mainrsrc,"WORKSTATION DOCUMENTS",PLAINTEXT,120,work_documents,TRUE);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,test_values,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitprt,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mainrsrc->scn=RDAscrnNEW(module,"WORKSTATION VALIDATION");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,23,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,20,"","Instructions","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","VALIDATE WORKSTATION","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","for","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","SECURE FILE TRANSFER PROTOCOL","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","Windows:  You must have an SFTP Server running on your workstation.","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","Mac, Ubuntu, Linux, and Unix:  should already have an SFTP Server installed.","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","Your firewall and other network appliances must be configured for share files.","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,21,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,20,"","Workstation","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","IP Address:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"WORKSTATION IP ADDRESS","","","",0,40,0,"","","","\"WORKSTATION LOGIN\"");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","     Login:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"WORKSTATION LOGIN","","","",0,40,0,"","","","\"WORKSTATION PASSWORD\"");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","  Password:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"WORKSTATION PASSWORD","","","",0,25,3,"","","","\"WORKSTATION TYPES\"");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","      Type:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,7,"WORKSTATION TYPES","","","",1,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,""," Documents:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"WORKSTATION DOCUMENTS","","","",0,60,0,"","","","\"SELECT\"");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,21,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,6,"SELECT","Validate","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,6,"QUIT","Cancel","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,6,"HELP","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");

	WorkStationTypeCB(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitprt,NULL,TRUE);
}
