/*
	This program will be called with the following syntax to 
	execute properly.

	rdabackup.EXT (DAILY|MONTHLY) (REMOTE_CLIENT_DIR_NAME)
	rdabackup.exe DAILY CRAIG_VA

*/
#ifndef WIN32
#define __NAM__ "mtnmst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mtnmst.exe"
#endif
#include <app.hpp>
#include <cstdio>
#include <cerrno>
#include <ctime>


#include <xpert.hpp>

#include <mix.hpp>
#include <misc.hpp>

#if (!defined(UNIXWARE21) && !defined(UNIXWARE7))
#include <sys/stat.h>
#include <fcntl.h>
#else 
#include <sys/stat.h>
#endif



#ifndef WIN32
extern char **environ;
#endif
/* Added for quick test without changing libs */


/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
#ifdef WIN32
#define TRACE	prterr("Line %d Program %s\r\n",__LINE__,__FILE__);flushall(RDA_STDERR)
#endif

typedef struct stat Statb;


int ExitError(int argc,char **argv,char *temp,char *zipfilename,char *logfilename, char *backuptype,char *clientdir,char *cname,XPERTstp *XPERT_SETUP)
{
	AddSupportRequest("DATABASE","RDABACKUP","Incorrect number of arguments provided when attempting to execute.");
	prterr("\n Incorrect Arguments.\n",argc);
	prterr("\n You Must Enter The Backup Type And Client Dir.\n",argc);
	prterr("\n Example: \"rdabackup.EXT DAILY BACKUP_DIR\"\n\n",argc);


	if(temp!=NULL) Rfree(temp);
	if(zipfilename!=NULL) Rfree(zipfilename);
	if(logfilename!=NULL) Rfree(logfilename);
	if(backuptype!=NULL) Rfree(backuptype);
	if(clientdir!=NULL) Rfree(clientdir);
	if(cname!=NULL) Rfree(cname);
	if(XPERT_SETUP!=NULL) free_xpert(XPERT_SETUP);
	exit(-1);
}


#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	FILE *MyReadPipe;
	char buffer[513],*temp=NULL,*zipfilename=NULL,*logfilename=NULL,*backuptype=NULL,*clientdir=NULL,*zippass=NULL;
	char *cname=NULL,*WebID=NULL;
	XPERTstp *XPERT_SETUP=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	struct tm *now;
	time_t tim;
	APPlib *args=NULL,*mye=NULL;
	Statb statb;
	int file_length=0;
	short x=0;
	char username[40],password[200],temp1[40];
	
	initrdadiag();
	tim=time(NULL);
	now=localtime(&tim);
	SETUSERLOGIN();
	SETCWD();
	XPERT_SETUP=XPERTstpNEW();
	getXPERTbin(XPERT_SETUP);
	XPERT_SETUP->TRANSACTIONS=FALSE;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","WEB ID");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			WebID=stralloc(gsv->value.string_value);
		} else {
			WebID=adddashes(XPERT_SETUP->company_name);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);

	memset(username,0,40);
	memset(password,0,200);
	memset(temp1,0,40);


	if(!strncmp(WebID,"MBGUI",5)) {
		strncpy(temp1,WebID+7,3);
		sprintf(username,"%d",((atoi(temp1))+48800));
#ifdef WIN32
		sprintf(password,"%s%s%s","jou9varu3elo1cheev3vutieyooquiesaiShohy7iethub0echoyah4moa8ooJ1Ohpeghu",WebID,"shaish3lohb4mahWaal");
#endif
	}else if(!strncmp(WebID,"OPENRDA",7)) {
		strncpy(temp1,WebID+9,3);
		sprintf(username,"%d",((atoi(temp1))+48000));
#ifdef WIN32
		memset(temp1,0,40);
		strncpy(temp1,WebID+8,4);

		sprintf(password,"%sXPERT-%s%s","jou9varu3elo1cheev3vutieyooquiesaiShohy7iethub0echoyah4moa8ooJ1Ohpeghu",temp1,"shaish3lohb4mahWaal");
#endif
	} else {
		strncpy(temp1,WebID+7,3);
		sprintf(username,"%d",((atoi(temp1))+48000));
#ifdef WIN32
		sprintf(password,"%s%s%s","jou9varu3elo1cheev3vutieyooquiesaiShohy7iethub0echoyah4moa8ooJ1Ohpeghu",WebID,"shaish3lohb4mahWaal");
#endif
	}


	if(diagapps)
	{
		prterr("argc [%d]\n",argc);
		prterr("argv1 [%s] argv2 [%s]\n",argv[1],argv[2]);
	}
	cname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(WebID)+22);
	if((RDAstrlen(WebID))==10) {
		zippass=Rmalloc(RDAstrlen(WebID)+45);
		sprintf(zippass,"%s-pyNWcD7B6RlAax1TOVcjVdDvBCWILGMsS0mHoUYR",WebID);
	}else if((RDAstrlen(WebID))==12) {
		zippass=Rmalloc(RDAstrlen(WebID)+45);
		sprintf(zippass,"%s-pyNWcD7B6RlAax1TOVcjVdDvBCWILGMsS0mHoUYR",WebID);
	} else {
		zippass=Rmalloc(25);
		sprintf(zippass,"XPERT-0000-pyNWcD7B6RlAax1TOVcjVdDvBCWILGMsS0mHoUYR");
	}
		
#ifndef WIN32
	sprintf(cname,"%s%s",WebID,CURRENTDIRECTORY);
	for(libx=cname;*libx;++libx) if(*libx=='/') *libx='-';
#else 
	sprintf(cname,"%s%s",WebID,&CURRENTDIRECTORY[2]);
	for(libx=cname;*libx;++libx) if(*libx=='\\') *libx='-';
#endif
	if(argc==3)
	{
		/*backuptype=Rmalloc(10);*/
		if(!RDAstrcmp(argv[1],""))
		{
			printf("ARGV1=EMPTY STRING\n");
			ExitError(argc, argv, temp, zipfilename, logfilename, backuptype, clientdir, cname, XPERT_SETUP);
		} else if(argv[1]==NULL)
		{
			printf("ARGV1=NULL\n");
			ExitError(argc, argv, temp, zipfilename, logfilename, backuptype, clientdir, cname, XPERT_SETUP);
		} else if(!RDAstrcmp(argv[1],"DAILY"))
		{
			printf("ARGV1=DAILY\n");
			backuptype=stralloc("DAILY");
		} else if(!RDAstrcmp(argv[1],"MONTHLY"))
		{
			printf("ARGV1=MONTHLY\n");
			backuptype=stralloc("MONTHLY");
			prterr("backuptype [%s]\n",backuptype);
		} else {
			backuptype=stralloc("DAILY");
			prterr("backuptype [%s]\n",backuptype);
		}
		
		if(!RDAstrcmp(argv[2],""))
		{
			printf("ARGV2=EMPTY STRING\n");
			ExitError(argc, argv, temp, zipfilename, logfilename, backuptype, clientdir, cname, XPERT_SETUP);
		} else if(argv[2]==NULL)
		{
			printf("ARGV2=NULL\n");
			ExitError(argc, argv, temp, zipfilename, logfilename, backuptype, clientdir, cname, XPERT_SETUP);
		} else {
			clientdir=stralloc(argv[2]);
			if(diagapps)
			{
				prterr("clientdir [%s]\n",clientdir);
			}
			
		}
	} else {
		if(diagapps)
		{
			prterr("argc [%d]\n",argc);
		}

		ExitError(argc, argv, temp, zipfilename, logfilename, backuptype, clientdir, cname, XPERT_SETUP);
	}

/*  WIN32 function for retrieving to to variable
	strftime(tmp,11,"%m/%d/%Y",localtime(&ltime));
*/

#ifdef WIN32

	zipfilename=Rmalloc((RDAstrlen(cname))+(RDAstrlen(backuptype))+128);
	sprintf(zipfilename,"C:\\rda\\backups\\%s-%s-%d-%d-%d.7z",cname,backuptype,(now->tm_year+1900),(now->tm_mon+1),(now->tm_mday));
	logfilename=Rmalloc((RDAstrlen(cname))+(RDAstrlen(backuptype))+128);
	sprintf(logfilename,"C:\\rda\\backups\\%s-%s-%d-%d-%d.log",cname,backuptype,(now->tm_year+1900),(now->tm_mon+1),(now->tm_mday));
#endif
#ifndef WIN32

	zipfilename=Rmalloc((RDAstrlen(cname))+(RDAstrlen(backuptype))+128);
	sprintf(zipfilename,"/rda/backups/%s-%s-%d-%d-%d.7z",cname,backuptype,(now->tm_year+1900),(now->tm_mon+1),(now->tm_mday));
	logfilename=Rmalloc((RDAstrlen(cname))+(RDAstrlen(backuptype))+128);
	sprintf(logfilename,"/rda/backups/%s-%s-%d-%d-%d.log",cname,backuptype,(now->tm_year+1900),(now->tm_mon+1),(now->tm_mday));

#endif 
	if(diagapps)
	{
		prterr("zipfilename [%s]\n",zipfilename);
		prterr("logfilename [%s]\n",logfilename);
	}
#ifndef WIN32
	unlink("PERMISSIONS.star.gz");
	args=APPlibNEW();
	addAPPlib(args,"-c");
	addAPPlib(args,"-v");
	addAPPlib(args,"-f");
	addAPPlib(args,"PERMISSIONS.star.gz");
	addAPPlib(args,"-z");
	addAPPlib(args,"-meta");
	addAPPlib(args,"./");
	x=ADV3Execute2Program("star",args,NULL,FALSE,TRUE);
	if(x)
	{
		AddSupportRequest("DATABASE","RDABACKUP","Unable to star permissions into PERMISSIONS.star.gz.  Will continue to attempt obtain data.");
		printf("Star Exit Code [%d]\n",x);
	}
	if(args!=NULL) Rfree(args);
#endif /* Not WIN32 */

#ifndef WIN32
	temp=Rmalloc((RDAstrlen(zipfilename))+(RDAstrlen(zippass))+130);
	sprintf(temp,"/rda/xpgms/7za.lnx a -t7z -mmt=on -p%s %s -xr-\\!\\*.exe  -xr-\\!\\*.dll -xr-\\!\\*.bz2 -xr-\\!\\*.gz .",zippass,(zipfilename!=NULL ? zipfilename:""));
#endif /* Not WIN32 */

#ifdef WIN32
	temp=Rmalloc((RDAstrlen(zipfilename))+(RDAstrlen(zippass))+130);
	sprintf(temp,"7za.exe a -t7z -mmt=on -ssw -p%s %s -xr-!*.exe  -xr-!*.dll -xr-!*.bz2 -xr-!*.gz .",zippass,(zipfilename!=NULL ? zipfilename:""));

#endif /* win32 */
	if(diagapps)
	{
		prterr("temp [%s] ",(temp!=NULL ? temp:""));TRACE;
	}
	MyReadPipe=popen(temp,"r");
	if(MyReadPipe!=NULL)
	{
		while(fgets(buffer,sizeof(buffer)-1,MyReadPipe)!=NULL)
		{
		}
		pclose(MyReadPipe);
	} else {
		AddSupportRequest("DATABASE","RDABACKUP","Unable to read pipe used to create 7z Archive.");
		exit(-1);
	}

#ifndef WIN32
	temp=Rmalloc(RDAstrlen(zipfilename)+RDAstrlen(zippass)+(RDAstrlen(logfilename))+230);
	sprintf(temp,"/rda/xpgms/7za.lnx t -p%s \"%s\" > \"%s\"",(zippass!=NULL ? zippass:""),(zipfilename!=NULL ? zipfilename:""),logfilename);
#endif /* Not WIN32 */

#ifdef WIN32
	temp=Rmalloc(RDAstrlen(zipfilename)+RDAstrlen(zippass)+(RDAstrlen(logfilename))+230);
	sprintf(temp,"7za.exe t -p%s %s > %s",(zippass!=NULL ? zippass:""),(zipfilename!=NULL ? zipfilename:""),logfilename);
#endif /* win32 */
	if(diagapps)
	{
		prterr("temp [%s] ",(temp!=NULL ? temp:""));TRACE;
	}
	MyReadPipe=popen(temp,"r");
	if(MyReadPipe!=NULL)
	{
		while(fgets(buffer,sizeof(buffer),MyReadPipe)!=NULL)
		{
		}
		pclose(MyReadPipe);
	} else {
		AddSupportRequest("DATABASE","RDABACKUP","Unable to read pipe used to test the contents 7z Archive.");
		exit(-1);
	}
	if(temp!=NULL) Rfree(temp);


	args=APPlibNEW();
#ifdef WIN32
	addAPPlib(args,"-C");
	addAPPlib(args,"-pw");
	addAPPlib(args,password);
	addAPPlib(args,zipfilename);
	memset(stemp,0,101);
	sprintf(stemp,"%s@usw-s004.rsync.net:%d/%s/",username,((atoi(username))+1000),clientdir);
	addAPPlib(args,stemp);
	x=Execute2Program("C:\\putty\\pscp",args);
	if(x)
	{
		AddSupportRequest("DATABASE","RDABACKUP","Unable to pscp using putty to upload 7z archive.");
		exit(-1);
	}
#else
	chmod(zipfilename,00640);
	addAPPlib(args,"-C");
	addAPPlib(args,zipfilename);
	memset(stemp,0,101);
	sprintf(stemp,"%s@usw-s004.rsync.net:%d/%s/",username,((atoi(username))+1000),clientdir);
	addAPPlib(args,stemp);
	x=ADV3Execute2Program("scp",args,NULL,FALSE,TRUE);
	if(x)
	{
		AddSupportRequest("DATABASE","RDABACKUP","Unable to scp to upload 7z archive.");
		exit(-1);
	}
#endif
	if(temp!=NULL) Rfree(temp);

	file_length=0;
#if !defined(LINUX_SUSE6_1)
	if(stat(zipfilename,&statb)!=(-1))
#endif 
#if defined(LINUX_SUSE6_1)
	if((_xstat(_STAT_VER,zipfilename,&statb)!=(-1))) 
#endif
	{
		file_length=(int)(statb.st_size);
	}
	args=APPlibNEW();
	mye=APPlibNEW();
	addAPPlib(args,"DATABASE");
	addAPPlib(args,"IBSBACKUPSC");
	memset(stemp,0,101);
	sprintf(stemp,"WEB_ID=%s",WebID);
	addAPPlib(mye,stemp);
	memset(stemp,0,101);
#ifdef WIN32
	sprintf(stemp,"FILENAME=%s",&zipfilename[15]);
#else
	sprintf(stemp,"FILENAME=%s",&zipfilename[13]);
#endif
	addAPPlib(mye,stemp);
	memset(stemp,0,101);
	sprintf(stemp,"TYPE=%s",backuptype);
	addAPPlib(mye,stemp);
	memset(stemp,0,101);
	sprintf(stemp,"LOCAL_SIZE=%d",file_length);
	addAPPlib(mye,stemp);
#ifdef WIN32
	x=ADV3Execute2Program("wwwdoadd",args,mye,TRUE,TRUE);
#else
	x=ADV3Execute2Program("/rda/xpgms/wwwdoadd",args,mye,TRUE,TRUE);
#endif
	if(x)
	{
		AddSupportRequest("DATABASE","RDABACKUP","Unable to record the backup into HOS->IBSBACKUPS.");
		exit(-1);
	}
	if(args!=NULL) freeapplib(args);
	if(mye!=NULL) freeapplib(mye);


	if(temp!=NULL) Rfree(temp);
	if(zipfilename!=NULL) Rfree(zipfilename);
	if(logfilename!=NULL) Rfree(logfilename);
	if(backuptype!=NULL) Rfree(backuptype);
	if(clientdir!=NULL) Rfree(clientdir);
	if(cname!=NULL) Rfree(cname);
	if(WebID!=NULL) Rfree(WebID);
	if(XPERT_SETUP!=NULL) free_xpert(XPERT_SETUP);
}
