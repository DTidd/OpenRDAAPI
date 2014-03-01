#ifndef WIN32
#define __NAM__ "checkemail.lnx"
#endif
#ifdef WIN32
#define __NAM__ "checkemail.exe"
#endif
#include <app.hpp>
#include <cstdlib>
#if (!defined(UNIXWARE21) && !defined(UNIXWARE7))
#include <sys/stat.h>
#include <fcntl.h>
#else 
#include <sys/stat.h>
#endif
#include <mix.hpp>
#include <gui.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define PIPEBLOCKSIZE 512

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

static char *module="UTILITIES";

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short fileno=(-1);
	char *temp=NULL,*home=NULL,*email=NULL;
	APPlib *args=NULL;
	MAIL_VARS *email_vars=NULL;
	int x=0;
	email_vars=malloc(sizeof(MAIL_VARS));
	email_vars->from_name=NULL;
	email_vars->from_addr=NULL;
	email_vars->subject=NULL;
	email_vars->body=NULL;
	email_vars->tolist=NULL;
	email_vars->cclist=NULL;
	email_vars->bcclist=NULL;
	for(x=0;x<10;x++)
	{
		email_vars->fname[x]=NULL;
		email_vars->mtype[x]=NULL;
		email_vars->dname[x]=NULL;
		email_vars->fdata[x]=NULL;
	}


	if (InitializeSubsystems(argc, argv, module, "TEST EMAIL"))
	{ 
		return(-1);
	}
	fileno=OPNNRD("SECURITY","USERS");
	if(fileno==(-1))
	{
		ERRORDIALOG("NO SECURITY PROFILE","Unable to open security profile.",NULL,TRUE);
		prterr("Error:  Unable to open Security Profile.");
		return(-1);
	}
	FINDFLDSETSTRING(fileno,"USER IDENTIFICATION",USERLOGIN);
	EQLNRD(fileno,1);
#ifdef WIN32
	home=RDA_GetEnv("APPDATA");
#else
	home=RDA_GetEnv("HOME");
#endif
	if(isEMPTY(home))
	{
		ERRORDIALOG("NO HOME DIRECTORY","Unable to locate home directory.",NULL,TRUE);
		prterr("Error:  Unable to find home directory.");
		return(-1);
	}
	email=Users_Email();
	if(isEMPTY(email))
	{
		ERRORDIALOG("MISSING EMAIL ADDRESS","User's Email Address is missing from Security Profile.",NULL,TRUE);
		prterr("Error:  User's [%s] Email Address missing from Security Profile.",USERLOGIN);
		if(email!=NULL) Rfree(email);
		return(-1);
	}
	temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen(home)+20);
#ifdef WIN32
	sprintf(temp,"%s\\msmtprc.txt",home);
#else
	sprintf(temp,"%s/.msmtprc",home);

	/* This needs fixed for Windows */
	if(access(temp,00)!=0)
	{
		ERRORDIALOG("MISSING MSMTPRC","User is missing msmtprc file from home directory.",NULL,TRUE);
		prterr("Error:  [%s] file doesn't exist (errno=[%d]).",temp,errno);
		if(email!=NULL) Rfree(email);
		if(temp!=NULL) Rfree(temp);
		return(-1);
	} else if(access(temp,04))
	{
		ERRORDIALOG("ACCESS DENIED TO MSMTPRC","User is unable to read msmtprc file in their home directory.",NULL,TRUE);
		prterr("Error:  [%s] file is unable to be read (errno=[%d]).",temp,errno);
		if(email!=NULL) Rfree(email);
		if(temp!=NULL) Rfree(temp);
		return(-1);
	}
#endif
	if(temp!=NULL) Rfree(temp);
	FINDFLDGETSTRING(fileno,"USER NAME",&temp);
/*
	args=APPlibNEW();
	addAPPlib(args,temp);
	if(temp!=NULL) Rfree(temp);
	addAPPlib(args,email);
	addAPPlib(args,"EMAIL TEST");
	addAPPlib(args,"This is a test email from the Xpert Server, used to diagnose and/or ensure configuration.");
	addAPPlib(args,email);
	if(email!=NULL) Rfree(email);
	Execute2Program("sendmail",args);
	if(args!=NULL) freeapplib(args);
*/
	email_vars->from_addr=Rmalloc((strlen(email))+1);
	sprintf(email_vars->from_addr,"%s",email);
	email_vars->tolist=Rmalloc((strlen(email))+1);
	sprintf(email_vars->tolist,"%s",email);
	email_vars->subject=stralloc("EMAIL TEST");
	email_vars->fname[1]=RDA_GetEnv("VMIME_FNAME1");
	email_vars->dname[1]=RDA_GetEnv("VMIME_DNAME1");

	email_vars->body=stralloc("This is a test email from the Xpert Server, used to diagnose and/or ensure configuration.");

	RunVMimeSendmail(email_vars);

	if(email_vars!=NULL) free(email_vars);
	if(fileno!=(-1)) CLSNRD(fileno);
	ShutdownSubsystems();
	std::exit;
}

