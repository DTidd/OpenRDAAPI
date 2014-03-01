/* docrontab.c - Program to Backup Xpert Package */
/*lint -library */
#ifndef WIN32
#define __NAM__ "docrontab.lnx"
#endif
#ifdef WIN32
#define __NAM__ "docrontab.exe"
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

char *module="UTILITIES";
APPlib *CRON=NULL,*hours=NULL,*days_mo=NULL,*days_wk=NULL,*mths=NULL;
APPlib *cpacks=NULL;

static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	ShutdownSubsystems();
	if(CRON!=NULL) freeapplib(CRON);
	std::exit;
}
static void getcurrentcrons()
{
	FILE *fp=NULL;
	char buffer[512];
	int x=0,y=0;

	if(CRON!=NULL) freeapplib(CRON);
	CRON=APPlibNEW();
	/*
	  This needs to read the /etc/passwd file and check
	  for the rdacron user and report an error using
	  The ErrorDialog Function before trying to continue.
	*/
	sprintf(stemp,"sudo -u rdacron crontab -l",USERLOGIN);
	fp=popen(stemp,"r");
	if(fp!=NULL)
	{
		while(fgets(buffer,sizeof(buffer),fp))
		{
			if(!isEMPTY(buffer) && buffer[0]!='#')
			{
				x=RDAstrlen(buffer);
				for(y=0;y<x;++y)
				{
					if(buffer[y]=='\n' || buffer[y]=='\r')
					{
						if(y+1<x)
						if((buffer[y+1]=='\n' || buffer[y+1]=='\r') && buffer[y]!=buffer[y+1])
						{
							buffer[y]=0;
							++y;
						}
						buffer[y]=0;
						++y;
						break;
					}
				}
				addAPPlib(CRON,buffer);
			}
			memset(buffer,0,sizeof(buffer));
		}
		pclose(fp);
	}
	if(CRON->numlibs<1) addAPPlib(CRON,"No Crontab's Defined");
}
static void quitadd(RDArsrc *r)
{
	if(hours!=NULL) freeapplib(hours);
	if(days_mo!=NULL) freeapplib(days_mo);
	if(days_wk!=NULL) freeapplib(days_wk);
	if(mths!=NULL) freeapplib(mths);
	if(cpacks!=NULL) freeapplib(cpacks);
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);		
	}
}
void WriteCronJobs(RDArsrc *mainrsrc,char *new_one)
{
	char temp[512],tempx[1024];
	int selected=0,x=0;
	FILE *fp=NULL;

	FINDRSCGETINT(mainrsrc,"CRONTABS",&selected);
	memset(tempx,0,1024);
	sprintf(tempx,"%s/%s.cron",CURRENTDIRECTORY,USERLOGIN);
	unlink(tempx);
	sprintf(temp,"sudo -u rdacron crontab %s",tempx);
	fp=fopen(tempx,"w");
	if(fp!=NULL)
	{
		for(x=0;x<CRON->numlibs;++x)
		{
			if(RDAstrcmp(CRON->libs[x],"No Crontab's Defined")) 
				fprintf(fp,"%s\n",CRON->libs[x]);
		}
		if(!isEMPTY(new_one))
		{
			fprintf(fp,"%s\n",(new_one!=NULL ? new_one:""));
		}
		fclose(fp);
		system_command(temp);
		unlink(tempx);
	}
	getcurrentcrons();
	if(selected>=CRON->numlibs) selected=0;
	FINDRSCLISTAPPlib(mainrsrc,"CRONTABS",selected,CRON);
	updatersrc(mainrsrc,"CRONTABS");
}
static void savecron(RDArsrc *r,RDArsrc *mainrsrc)
{
	char daymo[3],mon[3],daywk[3],*c=NULL;
	char tempx[1024];
	short m=0;
	int h=0,d=0,mo=0,wd=0,p=0;
	
	readallwidgets(r);
	FINDRSCGETSHORT(r,"MINUTE",&m);
	FINDRSCGETINT(r,"HOUR OF DAY",&h);
	FINDRSCGETINT(r,"DAY OF MONTH",&d);
	FINDRSCGETINT(r,"MONTH OF YEAR",&mo);
	FINDRSCGETINT(r,"DAY OF WEEK",&wd);
	FINDRSCGETINT(r,"PACKAGES",&p);
	FINDRSCGETSTRING(r,"COMMAND",&c);
	if(d==0) sprintf(daymo,"*");
		else sprintf(daymo,"%d",d);
	if(mo==0) sprintf(mon,"*");
		else sprintf(mon,"%d",mo);
	if(wd==7) sprintf(daywk,"*");
		else sprintf(daywk,"%d",wd);
	switch(p)
	{
		case 0: /* crontran */
			sprintf(tempx,"%d %d %s %s %s cd %s ; /rda/xpgms/crontran.lnx >/dev/null 2>&1",m,h,daymo,mon,daywk,CURRENTDIRECTORY);
			break;
		case 1: /* rdacroncd */
			sprintf(tempx,"%d %d %s %s %s %s/rdacroncd.lnx >/dev/null 2>&1",m,h,daymo,mon,daywk,CURRENTDIRECTORY);
			break;
		case 2: /* rdacrontape */
			sprintf(tempx,"%d %d %s %s %s %s/rdacrontape.lnx >/dev/null 2>&1",m,h,daymo,mon,daywk,CURRENTDIRECTORY);
			break;
		case 3: /* custom command */
			sprintf(tempx,"%d %d %s %s %s cd %s ; %s/scripts/setRDAenv %s",m,h,daymo,mon,daywk,CURRENTDIRECTORY,CURRENTDIRECTORY,c);
			prterr("%s",tempx);
			break;
	}	
	if(c!=NULL) Rfree(c);
	WriteCronJobs(mainrsrc,tempx);
	quitadd(r);
}
static void checkminute(RDArsrc *r)
{
	short m=0;

	readwidget(r,"MINUTE");
	FINDRSCGETSHORT(r,"MINUTE",&m);
	if(m>60)
	{
		m=0;
		FINDRSCSETSHORT(r,"MINUTE",m);
		updatersrc(r,"MINUTE");
	}
}
static void chgpcks(RDArsrc *r)
{
	int c=0;

	FINDRSCGETINT(r,"PACKAGES",&c);
	if(c==3)
	{
		FINDRSCSETEDITABLE(r,"COMMAND",TRUE);
		FINDRSCSETSENSITIVE(r,"COMMAND",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"COMMAND",FALSE);
		FINDRSCSETSENSITIVE(r,"COMMAND",FALSE);
	}
	updatersrc(r,"COMMAND");
}
static void addcron(RDArsrc *r)
{
	RDArsrc *editrsrc=NULL;
	short m=1;
	int x=7;

	editrsrc=RDArsrcNEW(module,"ADD CRONTAB");
	addstdrsrc(editrsrc,"COMMAND",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(editrsrc,"MINUTE",SHORTV,2,&m,TRUE);
	FINDRSCSETFUNC(editrsrc,"MINUTE",checkminute,NULL);
	if(cpacks!=NULL) freeapplib(cpacks);
	cpacks=APPlibNEW();
	addAPPlib(cpacks,"TransProc Maintenance");
	addAPPlib(cpacks,"Backup to CDROM");
	addAPPlib(cpacks,"Backup to Tape");
	addAPPlib(cpacks,"Custom Command");
	addlstrsrc(editrsrc,"PACKAGES",0,TRUE,chgpcks,cpacks->numlibs,&cpacks->libs,NULL);
	if(hours!=NULL) freeapplib(hours);
	hours=APPlibNEW();
	addAPPlib(hours,"00:00 Hour");
	addAPPlib(hours,"01:00 Hour");
	addAPPlib(hours,"02:00 Hour");
	addAPPlib(hours,"03:00 Hour");
	addAPPlib(hours,"04:00 Hour");
	addAPPlib(hours,"05:00 Hour");
	addAPPlib(hours,"06:00 Hour");
	addAPPlib(hours,"07:00 Hour");
	addAPPlib(hours,"08:00 Hour");
	addAPPlib(hours,"09:00 Hour");
	addAPPlib(hours,"10:00 Hour");
	addAPPlib(hours,"11:00 Hour");
	addAPPlib(hours,"12:00 Hour");
	addAPPlib(hours,"13:00 Hour");
	addAPPlib(hours,"14:00 Hour");
	addAPPlib(hours,"15:00 Hour");
	addAPPlib(hours,"16:00 Hour");
	addAPPlib(hours,"17:00 Hour");
	addAPPlib(hours,"18:00 Hour");
	addAPPlib(hours,"19:00 Hour");
	addAPPlib(hours,"20:00 Hour");
	addAPPlib(hours,"21:00 Hour");
	addAPPlib(hours,"22:00 Hour");
	addAPPlib(hours,"23:00 Hour");
	addlstrsrc(editrsrc,"HOUR OF DAY",0,TRUE,NULL,hours->numlibs,&hours->libs,NULL);
	if(days_mo!=NULL) freeapplib(days_mo);
	days_mo=APPlibNEW();
	addAPPlib(days_mo,"All Days");
	addAPPlib(days_mo,"Day 1");
	addAPPlib(days_mo,"Day 2");
	addAPPlib(days_mo,"Day 3");
	addAPPlib(days_mo,"Day 4");
	addAPPlib(days_mo,"Day 5");
	addAPPlib(days_mo,"Day 6");
	addAPPlib(days_mo,"Day 7");
	addAPPlib(days_mo,"Day 8");
	addAPPlib(days_mo,"Day 9");
	addAPPlib(days_mo,"Day 10");
	addAPPlib(days_mo,"Day 11");
	addAPPlib(days_mo,"Day 12");
	addAPPlib(days_mo,"Day 13");
	addAPPlib(days_mo,"Day 14");
	addAPPlib(days_mo,"Day 15");
	addAPPlib(days_mo,"Day 16");
	addAPPlib(days_mo,"Day 17");
	addAPPlib(days_mo,"Day 18");
	addAPPlib(days_mo,"Day 19");
	addAPPlib(days_mo,"Day 20");
	addAPPlib(days_mo,"Day 21");
	addAPPlib(days_mo,"Day 22");
	addAPPlib(days_mo,"Day 23");
	addAPPlib(days_mo,"Day 24");
	addAPPlib(days_mo,"Day 25");
	addAPPlib(days_mo,"Day 26");
	addAPPlib(days_mo,"Day 27");
	addAPPlib(days_mo,"Day 28");
	addAPPlib(days_mo,"Day 29");
	addAPPlib(days_mo,"Day 30");
	addAPPlib(days_mo,"Day 31");
	addlstrsrc(editrsrc,"DAY OF MONTH",0,TRUE,NULL,days_mo->numlibs,&days_mo->libs,NULL);
	if(mths!=NULL) freeapplib(mths);
	mths=APPlibNEW();
	addAPPlib(mths,"All Months");
	addAPPlib(mths,"January");
	addAPPlib(mths,"February");
	addAPPlib(mths,"March");
	addAPPlib(mths,"April");
	addAPPlib(mths,"May");
	addAPPlib(mths,"June");
	addAPPlib(mths,"July");
	addAPPlib(mths,"August");
	addAPPlib(mths,"September");
	addAPPlib(mths,"October");
	addAPPlib(mths,"November");
	addAPPlib(mths,"December");
	addlstrsrc(editrsrc,"MONTH OF YEAR",0,TRUE,NULL,mths->numlibs,&mths->libs,NULL);
	if(days_wk!=NULL) freeapplib(days_wk);
	days_wk=APPlibNEW();
	addAPPlib(days_wk,"Sunday");
	addAPPlib(days_wk,"Monday");
	addAPPlib(days_wk,"Tuesday");
	addAPPlib(days_wk,"Wednesday");
	addAPPlib(days_wk,"Thursday");
	addAPPlib(days_wk,"Friday");
	addAPPlib(days_wk,"Saturday");
	addAPPlib(days_wk,"All Days");
	x=7;
	addlstrsrc(editrsrc,"DAY OF WEEK",&x,TRUE,NULL,days_wk->numlibs,&days_wk->libs,NULL);
	GET_SCREEN_VIRTUALS(editrsrc,0);
	addrfcbrsrc(editrsrc,"SAVE",TRUE,savecron,r);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quitadd,NULL);
	addrfcbrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	chgpcks(editrsrc);
	computeandupdateallSCRNvirtuals(editrsrc,FALSE);
	APPmakescrn(editrsrc,TRUE,quitadd,NULL,TRUE);
}
static void deletecron(RDArsrc *r)
{
	int x=0,selected=0;
	APPlib *a=NULL;

	FINDRSCGETINT(r,"CRONTABS",&selected);
#ifndef XXXX
	system_command("crontab -r");
#endif /* Not XXXX */
#ifdef XXXX
	a=APPlibNEW();
	addAPPlib(a,"-r");
	Execute2Program("crontab",a);
	if(a!=NULL) freeapplib(a);
#endif /* XXXX */
	if(CRON->numlibs>1)
	{
		a=APPlibNEW();
		for(x=0;x<CRON->numlibs;++x)
		{
			if(x!=(selected))
			{
				addAPPlib(a,CRON->libs[x]);
			}
		}
		if(CRON!=NULL) freeapplib(CRON);
		CRON=APPlibNEW();
		for(x=0;x<a->numlibs;++x) addAPPlib(CRON,a->libs[x]);
		freeapplib(a);
	} else {
		if(CRON!=NULL) freeapplib(CRON);
		CRON=APPlibNEW();
	}
	WriteCronJobs(r,NULL);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"MANAGE CRONTAB")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"MANAGE CRONTAB");
	getcurrentcrons();
	addlstrsrc(mainrsrc,"CRONTABS",0,TRUE,NULL,CRON->numlibs,&CRON->libs,NULL);
	addrfcbrsrc(mainrsrc,"ADD",TRUE,addcron,NULL);
	addrfcbrsrc(mainrsrc,"DELETE",TRUE,deletecron,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
