/* remtrans.c - Remove Transaction Files */
#ifndef WIN32
#define __NAM__ "remtrans.lnx"
#endif
#ifdef WIN32
#define __NAM__ "remtrans.exe"
#endif
#include <app.hpp>


#include <fcntl.h>

#include <mix.hpp>
#ifdef UNIXWARE7
#include <cerrno>
#endif
#ifdef UNIXWARE21
#include <cerrno>
#endif
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define INCOMPLETE_TRANS 0
#define LOG_ONLY_TRANS	 1
#define COMPLETE_TRANS   2 

static APPlib *modlst=NULL;
struct TempTrans 
{
	char *user;
	char *process;
	char *datetime;
	char *endtime;
	long trnname;
};
typedef struct TempTrans TempTran;
static TempTran *tran=NULL;
static void delrange(RDArsrc *);
static short removetranfilescb(RDArsrc *);
static int getdirectorytrans(char *);
static TempTran *TempTranNEW(void);
static void FreeTempTran(TempTran *);
static short testrange(char,int,int,char,char *,char *,char,char *,char *,char,char *,char *,char,char *,char *,char,char *,char *,char,char *,char *,TempTran *);
static short removetranfiles(RDArsrc *,char *,char,char,char,char,int,int,char,char *,char *,char,char *,char *,char,char *,char *,char,char *,char *,char,char *,char *,char,char *,char *);

static int getdirectorytrans(char *module)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlst!=NULL) freeapplib(modlst); 
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6); 
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Entries");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(modlst);
	x=FINDAPPLIBELEMENT(modlst,module);
	if(x==(-1))
	{
		x=0;
	}
	return(x);
/*
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULES AVAILABLE",0,modlst))
	{
		updatersrc(mainrsrc,"MODULES AVAILABLE");
	}
*/
}
static void delrange(RDArsrc *mainrsrc)
{
	char *e=NULL;
	short x=0;

	readallwidgets(mainrsrc);
 	x=removetranfilescb(mainrsrc);
 	if(x==0)
	{
		e=Rmalloc(175);
		sprintf(e,"There where no files with the specified range criteria given.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO FILES IN RANGE FOUND!",e,NULL,NULL,TRUE,NULL);
		prterr("WARNING: No Files In Specified Range: %s.",e);
		if(e!=NULL) Rfree(e);
	} else {
	}
}
static short testrange(char tranrg,int tranto,int tranfr,
		char processrg,char *processto,char *processfr,
		char userrg,char *userto,char *userfr,
		char startdaterg,char *sdateto,char *sdatefr,
		char starttimerg,char *stimeto,char *stimefr,
		char enddaterg,char *edateto,char *edatefr,
		char endtimerg,char *etimeto,char *etimefr,
		TempTran *rgtran)
{
	short return_val=TRUE;
	char *dated=NULL,*datet=NULL;
	char *datedadj=NULL,*datefradj=NULL,*datetoadj=NULL;
	
	if(tranrg)
	{
		if((rgtran->trnname>=tranfr)&&(rgtran->trnname<=tranto))
		{
			return_val=TRUE;
		} else {
			return(FALSE);
		}
	}
	if(processrg)
	{
		if(RDAstrcmp(rgtran->process,processfr)>=0&&RDAstrcmp(rgtran->process,
			processto)<=0)
		{
			return_val=TRUE;
		} else {
			return(FALSE);
		}
	}
	if(userrg)
	{
		if(RDAstrcmp(rgtran->user,userfr)>=0&&RDAstrcmp(rgtran->user,userto)<=0)
		{
			return_val=TRUE;
		} else { 
			return(FALSE);
		}
	}
	if(starttimerg||startdaterg)
	{
		dated=strtok(rgtran->datetime," ");
		datet=strtok(NULL," ");
		if(starttimerg)
		{
			if(RDAstrcmp(datet,stimefr)>=0&&RDAstrcmp(datet,stimeto)<=0)
			{
				return_val=TRUE;
			} else { 
				return(FALSE);
			}
		}
		if(startdaterg)
		{
			if(!isEMPTY(dated) && !isEMPTY(sdatefr) && !isEMPTY(sdateto))
			{
				if(RDAstrlen(dated)==10)
				{
					datedadj=Rmalloc(9);
					sprintf(datedadj,"%c%c%c%c%c%c%c%c",dated[6],dated[7],dated[8],dated[9],dated[0],dated[1],dated[3],dated[4]);

				} else if(RDAstrlen(dated)==8)
				{
					datedadj=Rmalloc(9);
					sprintf(datedadj,"%c%c%c%c%c%c%c%c",'1','9',dated[6],dated[7],dated[0],dated[1],dated[3],dated[4]);
				}

				if(RDAstrlen(sdatefr)==10)
				{
					datefradj=Rmalloc(9);
					sprintf(datefradj,"%c%c%c%c%c%c%c%c",sdatefr[6],sdatefr[7],sdatefr[8],sdatefr[9],sdatefr[0],sdatefr[1],sdatefr[3],sdatefr[4]);
				} else if(RDAstrlen(sdatefr)==8)
				{
					datefradj=Rmalloc(9);
					sprintf(datefradj,"%c%c%c%c%c%c%c%c",'1','9',sdatefr[6],sdatefr[7],sdatefr[0],sdatefr[1],sdatefr[3],sdatefr[4]);
				}

				if(RDAstrlen(sdateto)==10)
				{
					datetoadj=Rmalloc(9);
					sprintf(datetoadj,"%c%c%c%c%c%c%c%c",sdateto[6],sdateto[7],sdateto[8],sdateto[9],sdateto[0],sdateto[1],sdateto[3],sdateto[4]);
				} else if(RDAstrlen(sdateto)==8)
				{
					datetoadj=Rmalloc(9);
					sprintf(datetoadj,"%c%c%c%c%c%c%c%c",'1','9',sdateto[6],sdateto[7],sdateto[0],sdateto[1],sdateto[3],sdateto[4]);
				}
			}
			if(RDAstrcmp(datedadj,datefradj)>=0&&RDAstrcmp(datedadj,datetoadj)<=0)
			{	
				return_val=TRUE;
			} else { 
				return(FALSE);
			}
			if(datedadj!=NULL) Rfree(datedadj);
			if(datefradj!=NULL) Rfree(datefradj);
			if(datetoadj!=NULL) Rfree(datetoadj);
/*
			if(RDAstrcmp(dated,sdatefr)>=0&&RDAstrcmp(dated,sdateto)<=0)
			{
				return_val=TRUE;
			} else {
				return(FALSE);
			}
*/
		}
	}
	if(endtimerg||enddaterg)
	{
		dated=strtok(rgtran->endtime," ");
		datet=strtok(NULL," ");
		if(endtimerg)
		{
			if(RDAstrcmp(datet,etimefr)>=0&&RDAstrcmp(datet,etimeto)<=0)
			{
				return_val=TRUE;
			} else {
				return(FALSE);
			}
		}
		if(enddaterg)
		{
			if(!isEMPTY(dated) && !isEMPTY(edatefr) && !isEMPTY(edateto))
			{
				if(RDAstrlen(dated)==10)
				{
					datedadj=Rmalloc(9);
					sprintf(datedadj,"%c%c%c%c%c%c%c%c",dated[6],dated[7],dated[8],dated[9],dated[0],dated[1],dated[3],dated[4]);

				} else if(RDAstrlen(dated)==8)
				{
					datedadj=Rmalloc(9);
					sprintf(datedadj,"%c%c%c%c%c%c%c%c",'1','9',dated[6],dated[7],dated[0],dated[1],dated[3],dated[4]);
				}

				if(RDAstrlen(edatefr)==10)
				{
					datefradj=Rmalloc(9);
					sprintf(datefradj,"%c%c%c%c%c%c%c%c",edatefr[6],edatefr[7],edatefr[8],edatefr[9],edatefr[0],edatefr[1],edatefr[3],edatefr[4]);
				} else if(RDAstrlen(edatefr)==8)
				{
					datefradj=Rmalloc(9);
					sprintf(datefradj,"%c%c%c%c%c%c%c%c",'1','9',edatefr[6],edatefr[7],edatefr[0],edatefr[1],edatefr[3],edatefr[4]);
				}

				if(RDAstrlen(edateto)==10)
				{
					datetoadj=Rmalloc(9);
					sprintf(datetoadj,"%c%c%c%c%c%c%c%c",edateto[6],edateto[7],edateto[8],edateto[9],edateto[0],edateto[1],edateto[3],edateto[4]);
				} else if(RDAstrlen(edateto)==8)
				{
					datetoadj=Rmalloc(9);
					sprintf(datetoadj,"%c%c%c%c%c%c%c%c",'1','9',edateto[6],edateto[7],edateto[0],edateto[1],edateto[3],edateto[4]);
				}
			}
			if(RDAstrcmp(datedadj,datefradj)>=0&&RDAstrcmp(datedadj,datetoadj)<=0)
			{	
				return_val=TRUE;
			} else { 
				return(FALSE);
			}
			if(datedadj!=NULL) Rfree(datedadj);
			if(datefradj!=NULL) Rfree(datefradj);
			if(datetoadj!=NULL) Rfree(datetoadj);
/*
			if(RDAstrcmp(dated,edatefr)>=0&&RDAstrcmp(dated,edateto)<=0)
			{
				return_val=TRUE;
			} else {
				return(FALSE);
			}
*/
		}
	}
	return(return_val);
}
/*
static short testrangecb(RDArsrc *mainrsrc,TempTran *rgtran)
{
	char tranrg=FALSE,userrg=FALSE,processrg=FALSE;
	char startdaterg=FALSE, starttimerg=FALSE;
	char enddaterg=FALSE, endtimerg=FALSE;
	int tranto=0,tranfr=0;
	char *processto=NULL,*processfr=NULL;
	char *userto=NULL,*userfr=NULL;
	char *sdateto=NULL,*sdatefr=NULL,*stimeto=NULL,*stimefr=NULL;
	char *edateto=NULL,*edatefr=NULL,*etimeto=NULL,*etimefr=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON TRANS",&tranrg);
	if(tranrg)
	{
		FINDRSCGETINT(mainrsrc,"RANGE TRANS FROM",&tranfr);
		FINDRSCGETINT(mainrsrc,"RANGE TRANS TO",&tranto);
	}
	FINDRSCGETCHAR(mainrsrc,"RANGE ON PROCESS",&processrg);
	if(processrg)
	{
		FINDRSCGETSTRING(mainrsrc,"RANGE PROCESS FROM",&processfr);
		FINDRSCGETSTRING(mainrsrc,"RANGE PROCESS TO",&processto);
	}
	FINDRSCGETCHAR(mainrsrc,"RANGE ON USER",&userrg);
	if(userrg)
	{
		FINDRSCGETSTRING(mainrsrc,"RANGE USER FROM",&userfr); 
		FINDRSCGETSTRING(mainrsrc,"RANGE USER TO",&userto);
	}
	FINDRSCGETCHAR(mainrsrc,"RANGE ON STARTTIME",&starttimerg);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON STARTDATE",&startdaterg);
	if(startdaterg||starttimerg)
	{
		if(starttimerg)
		{
			FINDRSCGETSTRING(mainrsrc,"RANGE STARTTIME FROM",&stimefr);
			FINDRSCGETSTRING(mainrsrc,"RANGE STARTTIME TO",&stimeto);
		}
		if(startdaterg)
		{
			FINDRSCGETSTRING(mainrsrc,"RANGE STARTDATE FROM",&sdatefr);
			FINDRSCGETSTRING(mainrsrc,"RANGE STARTDATE TO",&sdateto);
		}
	}
	FINDRSCGETCHAR(mainrsrc,"RANGE ON ENDTIME",&endtimerg);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON ENDDATE",&enddaterg);
	if(enddaterg||endtimerg)
	{
		if(endtimerg)
		{
			FINDRSCGETSTRING(mainrsrc,"RANGE ENDTIME FROM",&etimefr);
			FINDRSCGETSTRING(mainrsrc,"RANGE ENDTIME TO",&etimeto);
		}
		if(enddaterg)
		{
			FINDRSCGETSTRING(mainrsrc,"RANGE ENDDATE FROM",&edatefr);
			FINDRSCGETSTRING(mainrsrc,"RANGE ENDDATE TO",&edateto);
		}
	}
	testrange(tranrg,tranto,tranfr,processrg,processto,processfr,
		userrg,userto,userfr,startdaterg,sdateto,sdatefr,
		starttimerg,stimeto,stimefr,enddaterg,edateto,edatefr,
		endtimerg,etimeto,etimefr,rgtran);
	if(userto!=NULL) Rfree(userto);
	if(userfr!=NULL) Rfree(userfr);
	if(processfr!=NULL) Rfree(processfr);
	if(processto!=NULL) Rfree(processto);
	if(sdateto!=NULL) Rfree(sdateto);
	if(sdatefr!=NULL) Rfree(sdatefr);
	if(stimeto!=NULL) Rfree(stimeto);
	if(stimefr!=NULL) Rfree(stimefr);
	if(edateto!=NULL) Rfree(edateto);
	if(edatefr!=NULL) Rfree(edatefr);
	if(etimeto!=NULL) Rfree(etimeto);
	if(etimefr!=NULL) Rfree(etimefr);
}
*/
static short removetranfiles(RDArsrc *mainrsrc,char *module,
		char logonlys,char incomptrans,char comptrans,
		char tranrg,int tranto,int tranfr,
		char processrg,char *processto,char *processfr,
		char userrg,char *userto,char *userfr,
		char startdaterg,char *sdateto,char *sdatefr,
		char starttimerg,char *stimeto,char *stimefr,
		char enddaterg,char *edateto,char *edatefr,
		char endtimerg,char *etimeto,char *etimefr)
{
	FILE *fp=NULL;
	char *tmp=NULL,*tmp2=NULL,*filename=NULL,*test=NULL;
	char *tname=NULL, *errordesc=NULL,*errorname=NULL,*e=NULL,temp[512];
	short exit=FALSE,filesfound=0,deltran=FALSE,removed=FALSE;
	short type=INCOMPLETE_TRANS;
	long pos=0,curpos=0;
	int x,numtrans=0;
	char *dirx=NULL;
	RDArsrc *tmprsrc=NULL;
#ifdef _POSIX_SOURCE
/*
	long offset=-34;
*/
#ifdef WIN32
	long offset2=-35;
	long offset=-37;
#endif
#ifndef WIN32
	long offset2=-34;
	long offset=-36;
#endif
#else
/*
	long offset=-31;
*/
#ifdef WIN32
	long offset2=-35;
	long offset=-37;
#endif
#ifndef WIN32
	long offset2=-34;
	long offset=-36;
#endif
#endif

	if(!logonlys&&!incomptrans&&!comptrans)
	{
		e=Rmalloc(RDAstrlen(module)+175);
		sprintf(e,"You must select a type or group of types of transactions that you wish to remove, e.g. Log Entries Only, Incomplete Transactions or Complete Transactions from Module [%s]",module);
		if(mainrsrc!=NULL)
		{
			ERRORDIALOG("NO TRANSACTION ENTRY TYPE!",e,NULL,FALSE);
		}
		prterr("ERROR: NO TRANSACTION ENTRY TYPE!, %s",e);
		if(e!=NULL) Rfree(e);
		return(2);
	}
	if(module!=NULL) 
	{
 		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+3);
#ifndef WIN32
	 	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	 	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,module);
#endif
		filename=Rmalloc(RDAstrlen(dirx)+12);
		sprintf(filename,"%s/NXTTRN.TRN",dirx);
		fp=fopen(filename,"r");
		if(fp!=NULL)
		{
#ifdef UNIXWARE21
			fgets(temp,sizeof(temp)-1,fp);
			tmp2=strtok(temp,"\r\n");
			numtrans=Ratoi(tmp2);
#else
			while(fgets(temp,sizeof(temp)-1,fp))
			{
				tmp2=strtok(temp,"\r\n");
				numtrans=Ratoi(tmp2);
				break;
			}
#endif
			fclose(fp);
		} else {
			for(x=0;findfile(dirx,"*.TRN",&tmp2,(int)x+1);++x)
			{
				if((RDAstrlen(tmp2)-4)>0)
				{
					tmp2[RDAstrlen(tmp2)-4]=0;
					if(numtrans<Ratoi(tmp2)) numtrans=Ratoi(tmp2);
				}
			}
			if(tmp2!=NULL) Rfree(tmp2);
		}
		if(filename!=NULL) Rfree(filename);
		e=Rmalloc(RDAstrlen(module)+35);
		sprintf(e,"Removing Transactions from Module %s",module);
		if(mainrsrc!=NULL)
		{
			tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","GUI",e,NULL,numtrans);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE))
				{
					ForceWindowUpdate(tmprsrc);
				}
			}
		}
		if(e!=NULL) Rfree(e);
		e=NULL;
	 	for(x=0;findfile(dirx,"*.TRN",&tmp,(int)x+1);++x) 
	 	{
			removed=FALSE;
			if(RDAstrcmp(tmp,"NXTTRN.TRN"))
			{
				filename=Rmalloc(RDAstrlen(dirx)+RDAstrlen(tmp)+1);
				sprintf(filename,"%s%s",dirx,tmp);
				fp=fopen(filename,"r+");
				if(fp!=NULL)
				{
					if(tname!=NULL) Rfree(tname);
					tname=stralloc(tmp);
					tname[RDAstrlen(tname)-4]=0;
					tran->trnname=Ratoi(tname);
					type=INCOMPLETE_TRANS;
					tran->datetime=READFIELD(fp,&exit);
					tran->process=READFIELD(fp,&exit);
					tran->user=READFIELD(fp,&exit);
					pos=ftell(fp);
					if(!fseek(fp,offset2,SEEK_END) && !exit)
					{
						curpos=ftell(fp);
						if(curpos>=pos)
						{
							test=READFIELD(fp,&exit);
							if(!exit && !RDAstrcmp(test,"END TRANSACTION"))
							{
								tran->endtime=READFIELD(fp,&exit);
								if(curpos>(pos))
								{
									type=COMPLETE_TRANS;
								} else {
									type=LOG_ONLY_TRANS;
								}	
							} else if(!fseek(fp,offset,SEEK_END) && !exit)
							{
								curpos=ftell(fp);
								if(curpos>=pos)
								{
									test=READFIELD(fp,&exit);
									if(!exit && !RDAstrcmp(test,"END TRANSACTION"))
									{
										tran->endtime=READFIELD(fp,&exit);
										if(curpos>(pos))
										{
											type=COMPLETE_TRANS;
										} else {
											type=LOG_ONLY_TRANS;
										}
									}
								}
							}
						}
					}
					fclose(fp);
					deltran=FALSE;
					if((type==LOG_ONLY_TRANS&&logonlys)) deltran=TRUE;
					if((type==COMPLETE_TRANS&&comptrans)) deltran=TRUE;
					if((type==INCOMPLETE_TRANS&&incomptrans)) deltran=TRUE;
					if(deltran==TRUE)
					{
/*
					if(testrangecb(mainrsrc,tran)&&deltran) 
					{
*/
					if(testrange(tranrg,tranto,tranfr,
						processrg,processto,processfr,
						userrg,userto,userfr,
						startdaterg,sdateto,sdatefr,
						starttimerg,stimeto,stimefr,
						enddaterg,edateto,edatefr,
						endtimerg,etimeto,etimefr,
						tran))
					{
						removed=TRUE;
						filesfound=1;
				 		if(unlink(filename)==(-1)) 
				 		{
							if(errno!=ENOENT) 
							{ 
								if(errorname!=NULL) Rfree(errorname);
								if(errordesc!=NULL) Rfree(errordesc);
								errorname=stralloc(errname());
								errordesc=Rmalloc(RDAstrlen(filename)+RDAstrlen(errorname)+53);
								sprintf(errordesc,"Can't remove transaction file [%s] Error %d: %s.",filename,errno,errorname);
								if(mainrsrc!=NULL)
								{
									ERRORDIALOG("CAN'T REMOVE FILE!",errordesc,NULL,FALSE);
								}
								prterr("%s",errordesc); 
								if(errorname!=NULL) Rfree(errorname);
								if(errordesc!=NULL) Rfree(errordesc);
							}
						} else {
							x--;
						}
					}
					}
				}
				if(test!=NULL) Rfree(test);
				if(filename!=NULL) Rfree(filename);
				if(mainrsrc!=NULL)
				{
					if(tmprsrc!=NULL)
					{
						update_diagnostic(tmprsrc,(removed?TRUE:FALSE));
					}
				}
			}
		}
		if(dirx!=NULL) Rfree(dirx);
		if(tmp!=NULL) Rfree(tmp);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
			tmprsrc=NULL;
		}
	}
	return(filesfound);
}
static short removetranfilescb(RDArsrc *mainrsrc)
{
	char logonlys=TRUE,incomptrans=FALSE,comptrans=FALSE;
	int modselection=0;
	char tranrg=FALSE,userrg=FALSE,processrg=FALSE;
	char startdaterg=FALSE,starttimerg=FALSE;
	char enddaterg=FALSE,endtimerg=FALSE;
	int tranto=0,tranfr=0;
	char *processto=NULL,*processfr=NULL;
	char *userto=NULL,*userfr=NULL;
	char *sdateto=NULL,*sdatefr=NULL,*stimeto=NULL,*stimefr=NULL;
	char *edateto=NULL,*edatefr=NULL,*etimeto=NULL,*etimefr=NULL;
	short return_value=FALSE;
	char *e=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"LOG ONLY ENTRIES",&logonlys);
	FINDRSCGETCHAR(mainrsrc,"INCOMPLETE ENTRIES",&incomptrans);
	FINDRSCGETCHAR(mainrsrc,"COMPLETE ENTRIES",&comptrans);
	if(!FINDRSCGETINT(mainrsrc,"MODULES AVAILABLE",&modselection))
	{
		FINDRSCGETCHAR(mainrsrc,"RANGE ON TRANS",&tranrg);
		if(tranrg)
		{
			FINDRSCGETINT(mainrsrc,"RANGE TRANS FROM",&tranfr);
			FINDRSCGETINT(mainrsrc,"RANGE TRANS TO",&tranto);
		}
		FINDRSCGETCHAR(mainrsrc,"RANGE ON PROCESS",&processrg);
		if(processrg)
		{
			FINDRSCGETSTRING(mainrsrc,"RANGE PROCESS FROM",&processfr);
			FINDRSCGETSTRING(mainrsrc,"RANGE PROCESS TO",&processto);
		}
		FINDRSCGETCHAR(mainrsrc,"RANGE ON USER",&userrg);
		if(userrg)
		{
			FINDRSCGETSTRING(mainrsrc,"RANGE USER FROM",&userfr); 
			FINDRSCGETSTRING(mainrsrc,"RANGE USER TO",&userto);
		}
		FINDRSCGETCHAR(mainrsrc,"RANGE ON STARTTIME",&starttimerg);
		FINDRSCGETCHAR(mainrsrc,"RANGE ON STARTDATE",&startdaterg);
		if(startdaterg||starttimerg)
		{
			if(starttimerg)
			{
				FINDRSCGETSTRING(mainrsrc,"RANGE STARTTIME FROM",&stimefr);
				FINDRSCGETSTRING(mainrsrc,"RANGE STARTTIME TO",&stimeto);
			}
			if(startdaterg)
			{
				FINDRSCGETSTRING(mainrsrc,"RANGE STARTDATE FROM",&sdatefr);
				FINDRSCGETSTRING(mainrsrc,"RANGE STARTDATE TO",&sdateto);
				if(RDAstrlen(sdatefr)<10)
				{
					e=Rmalloc(350);
					sprintf(e,"The FROM START DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
					if(mainrsrc!=NULL)
					{
						ERRORDIALOG("BAD FROM START DATE RANGE FORMAT!",e,NULL,FALSE);
					}
					prterr("ERROR: BAD FROM START DATE RANGE FORMAT!, %s",e);
					if(e!=NULL) Rfree(e);
					return(2);
				}
				if(RDAstrlen(sdateto)<10)
				{
					e=Rmalloc(350);
					sprintf(e,"The TO START DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
					if(mainrsrc!=NULL)
					{
						ERRORDIALOG("BAD TO START DATE RANGE FORMAT!",e,NULL,FALSE);
					}
					prterr("ERROR: BAD TO START DATE RANGE FORMAT!, %s",e);
					if(e!=NULL) Rfree(e);
					return(2);
				}
			}
		}
		FINDRSCGETCHAR(mainrsrc,"RANGE ON ENDTIME",&endtimerg);
		FINDRSCGETCHAR(mainrsrc,"RANGE ON ENDDATE",&enddaterg);
		if(enddaterg||endtimerg)
		{
			if(endtimerg)
			{
				FINDRSCGETSTRING(mainrsrc,"RANGE ENDTIME FROM",&etimefr);
				FINDRSCGETSTRING(mainrsrc,"RANGE ENDTIME TO",&etimeto);
			}
			if(enddaterg)
			{
				FINDRSCGETSTRING(mainrsrc,"RANGE ENDDATE FROM",&edatefr);
				FINDRSCGETSTRING(mainrsrc,"RANGE ENDDATE TO",&edateto);
				if(RDAstrlen(edatefr)<10)
				{
					e=Rmalloc(350);
					sprintf(e,"The FROM END DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
					if(mainrsrc!=NULL)
					{
						ERRORDIALOG("BAD FROM END DATE RANGE FORMAT!",e,NULL,FALSE);
					}
					prterr("ERROR: BAD FROM END DATE RANGE FORMAT!, %s",e);
					if(e!=NULL) Rfree(e);
					return(2);
				}
				if(RDAstrlen(edateto)<10)
				{
					e=Rmalloc(350);
					sprintf(e,"The TO END DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
					if(mainrsrc!=NULL)
					{
						ERRORDIALOG("BAD TO END DATE RANGE FORMAT!",e,NULL,FALSE);
					}
					prterr("ERROR: BAD TO END DATE RANGE FORMAT!, %s",e);
					if(e!=NULL) Rfree(e);
					return(2);
				}
			}
		}
		return_value=removetranfiles(mainrsrc,
			modlst->libs[modselection],
			logonlys,incomptrans,comptrans,
			tranrg,tranto,tranfr,
			processrg,processto,processfr,
			userrg,userto,userfr,
			startdaterg,sdateto,sdatefr,
			starttimerg,stimeto,stimefr,
			enddaterg,edateto,edatefr,
			endtimerg,etimeto,etimefr);
		if(userto!=NULL) Rfree(userto);
		if(userfr!=NULL) Rfree(userfr);
		if(processfr!=NULL) Rfree(processfr);
		if(processto!=NULL) Rfree(processto);
		if(sdateto!=NULL) Rfree(sdateto);
		if(sdatefr!=NULL) Rfree(sdatefr);
		if(stimeto!=NULL) Rfree(stimeto);
		if(stimefr!=NULL) Rfree(stimefr);
		if(edateto!=NULL) Rfree(edateto);
		if(edatefr!=NULL) Rfree(edatefr);
		if(etimeto!=NULL) Rfree(etimeto);
		if(etimefr!=NULL) Rfree(etimefr);
		return(return_value);
	} else {
		e=Rmalloc(100);
		sprintf(e,"Cannot find the Module List on the screen to select the module value.");
		if(mainrsrc!=NULL)
		{
			ERRORDIALOG("MISSING MODULE VALUE!",e,NULL,FALSE);
		}
		prterr("ERROR: MISSING MODULE VALUE!, %s",e);
		if(e!=NULL) Rfree(e);
		return(3);
	}
}
TempTran *TempTranNEW(void)
{
	TempTran *tmp=NULL;

	tmp=Rmalloc(sizeof(TempTran));
	tmp->user=NULL;
	tmp->process=NULL;
	tmp->datetime=NULL;
	tmp->endtime=NULL;
	tmp->trnname=0;
	return(tmp);
}
void FreeTempTran(TempTran *transact)
{
	if(transact!=NULL)
	{
		if(transact->user!=NULL) Rfree(transact->user);
		if(transact->process!=NULL) Rfree(transact->process);
		if(transact->datetime!=NULL) Rfree(transact->datetime);
		if(transact->endtime!=NULL) Rfree(transact->endtime);
	}
	Rfree(transact);
}
static void quitremtran(RDArsrc *mainrsrc)
{
	if(tran!=NULL) FreeTempTran(tran);
	if(modlst!=NULL) freeapplib(modlst);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	ShutdownSubsystems();
}
static void print_help(void)
{
	prterr("Usage:\n");
	prterr("           Remove Transactions Command Summary\n");
	prterr("           ------ ------------ ------- -------\n");
	prterr("  Windows     : remtrans.EXT [module]\n");
	prterr("  Command-line: remtrans.EXT module -c command ...\n");
	prterr("    command(s)=\n");
	prterr("              -t l(&|)i(&|)c\n");
	prterr("                    (l=logonly,i=incomplete,c=complete)\n");
	prterr("              -n from_tranno to_tranno \n");
	prterr("                    (trasaction number range)\n");
	prterr("              -p from_process to_process\n");
	prterr("                    (process name range)\n");
	prterr("              -u from_user to_user\n");
	prterr("                    (user name range)\n");
	prterr("              -sd from_startdate to_startdate\n");
	prterr("                    (start date range mm/dd/YYYY)\n");
	prterr("              -st from_starttime to_starttime\n");
	prterr("                    (start time range hh:mm:ss)\n");
	prterr("              -ed from_enddate to_enddate\n");
	prterr("                    (end date range mm/dd/YYYY)\n");
	prterr("              -et from_endtime to_endtime\n");
	prterr("                    (end time range hh:mm:ss)\n");
	prterr("    examples=\n");
	prterr("     *  remtrans.rsc VENPMT -c -t l -u doe doe \n");
	prterr("      (This command would remove all Log Only Type Transactions\n");
	prterr("       made by the user doe in the Vendor Payment module (VENPMT).\n");
	prterr("     *  remtrans.rsc PAYROLL -c -t lic -u doe spot -sd 01/01/1997 01/31/1997\n");
	prterr("      (This command would remove all Log Only, Incomplete, and\n");
	prterr("       Complete Type Transactions made between the start dates\n");
	prterr("       of Jan. 01, 1997 and Jan. 31, 1997 and made by the user(s)\n");
	prterr("       between doe and spot (inclusive) in the Payroll module (PAYROLL).\n");
	prterr("       All combinations can be used.)\n");
}
void setuseredit(RDArsrc *mainrsrc)
{
	char value=FALSE;

	readwidget(mainrsrc,"RANGE ON USER");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON USER",&value);
	if(value==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"RANGE USER FROM",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE USER FROM",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE USER TO",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE USER TO",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE USER FROM",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE USER FROM",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE USER TO",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE USER TO",FALSE);
	}
}
void settransedit(RDArsrc *mainrsrc)
{
	char value=FALSE;

	readwidget(mainrsrc,"RANGE ON TRANS");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON TRANS",&value);
	if(value==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"RANGE TRANS FROM",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE TRANS FROM",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE TRANS TO",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE TRANS TO",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE TRANS FROM",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE TRANS FROM",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE TRANS TO",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE TRANS TO",FALSE);
	}
}
void setprocedit(RDArsrc *mainrsrc)
{
	char value=FALSE;

	readwidget(mainrsrc,"RANGE ON PROCESS");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON PROCESS",&value);
	if(value==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"RANGE PROCESS FROM",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE PROCESS FROM",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE PROCESS TO",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE PROCESS TO",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE PROCESS FROM",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE PROCESS FROM",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE PROCESS TO",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE PROCESS TO",FALSE);
	}
}
void setstimeedit(RDArsrc *mainrsrc)
{
	char value=FALSE;

	readwidget(mainrsrc,"RANGE ON STARTTIME");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON STARTTIME",&value);
	if(value==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTTIME FROM",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTTIME FROM",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTTIME TO",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTTIME TO",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTTIME FROM",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTTIME FROM",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTTIME TO",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTTIME TO",FALSE);

	}
}
void setsdateedit(RDArsrc *mainrsrc)
{
	char value=FALSE;

	readwidget(mainrsrc,"RANGE ON STARTDATE");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON STARTDATE",&value);
	if(value==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTDATE FROM",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTDATE FROM",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTDATE TO",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTDATE TO",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTDATE FROM",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTDATE FROM",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE STARTDATE TO",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE STARTDATE TO",FALSE);

	}
}
void setetimeedit(RDArsrc *mainrsrc)
{
	char value=FALSE;

	readwidget(mainrsrc,"RANGE ON ENDTIME");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON ENDTIME",&value);
	if(value==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDTIME FROM",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDTIME FROM",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDTIME TO",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDTIME TO",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDTIME FROM",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDTIME FROM",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDTIME TO",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDTIME TO",FALSE);

	}
}
void setedateedit(RDArsrc *mainrsrc)
{
	char value=FALSE;

	readwidget(mainrsrc,"RANGE ON ENDDATE");
	FINDRSCGETCHAR(mainrsrc,"RANGE ON ENDDATE",&value);
	if(value==TRUE)
	{
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDDATE FROM",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDDATE FROM",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDDATE TO",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDDATE TO",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDDATE FROM",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDDATE FROM",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE ENDDATE TO",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE ENDDATE TO",FALSE);

	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0,a=0;
	RDArsrc *mainrsrc=NULL;
	char *module=NULL;
	char valid_args=FALSE;
	char logonlys=TRUE,incomptrans=FALSE,comptrans=FALSE;
	char tranrg=FALSE,userrg=FALSE,processrg=FALSE;
	char startdaterg=FALSE,starttimerg=FALSE;
	char enddaterg=FALSE,endtimerg=FALSE;
	int tranto=0,tranfr=0;
	char *processto=NULL,*processfr=NULL;
	char *userto=NULL,*userfr=NULL;
	char *sdateto=NULL,*sdatefr=NULL;
	char *stimeto=NULL,*stimefr=NULL;
	char *edateto=NULL,*edatefr=NULL;
	char *etimeto=NULL,*etimefr=NULL;
	char *e=NULL;

	if(InitializeSubsystems(argc,argv,"DATABASE","REMOVE TRANS FILES")) 
	{
		return;
	}
	if(argc<3)
	{
		if(argc>1)
		{
			module=stralloc(argv[1]);
		}
		tran=TempTranNEW();
		mainrsrc=RDArsrcNEW("DATABASE","REMOVE TRANS FILES");
		x=getdirectorytrans(module);
		addlstrsrc(mainrsrc,"MODULES AVAILABLE",&x,TRUE,NULL,modlst->numlibs,&modlst->libs,NULL);
	
		addstdrsrc(mainrsrc,"LOG ONLY ENTRIES",BOOLNS,1,&logonlys,TRUE);
		addstdrsrc(mainrsrc,"INCOMPLETE ENTRIES",BOOLNS,1,&incomptrans,TRUE);
		addstdrsrc(mainrsrc,"COMPLETE ENTRIES",BOOLNS,1,&comptrans,TRUE);
		addstdrsrc(mainrsrc,"RANGE ON USER",BOOLNS,1,&userrg,TRUE);
		addstdrsrc(mainrsrc,"RANGE ON TRANS",BOOLNS,1,&userrg,TRUE);
		addstdrsrc(mainrsrc,"RANGE ON PROCESS",BOOLNS,1,&userrg,TRUE);
		addstdrsrc(mainrsrc,"RANGE ON STARTDATE",BOOLNS,1,&startdaterg,TRUE);
		addstdrsrc(mainrsrc,"RANGE ON STARTTIME",BOOLNS,1,&starttimerg,TRUE);
		addstdrsrc(mainrsrc,"RANGE ON ENDDATE",BOOLNS,1,&enddaterg,TRUE);
		addstdrsrc(mainrsrc,"RANGE ON ENDTIME",BOOLNS,1,&endtimerg,TRUE);
		FINDRSCSETFUNC(mainrsrc,"RANGE ON USER",setuseredit,NULL);
		FINDRSCSETFUNC(mainrsrc,"RANGE ON TRANS",settransedit,NULL);
		FINDRSCSETFUNC(mainrsrc,"RANGE ON PROCESS",setprocedit,NULL);
		FINDRSCSETFUNC(mainrsrc,"RANGE ON STARTDATE",setsdateedit,NULL);
		FINDRSCSETFUNC(mainrsrc,"RANGE ON STARTTIME",setstimeedit,NULL);
		FINDRSCSETFUNC(mainrsrc,"RANGE ON ENDDATE",setedateedit,NULL);
		FINDRSCSETFUNC(mainrsrc,"RANGE ON ENDTIME",setetimeedit,NULL);
	
		addstdrsrc(mainrsrc,"RANGE USER FROM",VARIABLETEXT,0,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE USER TO",VARIABLETEXT,0,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE TRANS FROM",LONGV,15,0,TRUE);
		addstdrsrc(mainrsrc,"RANGE TRANS TO",LONGV,15,0,TRUE);
		addstdrsrc(mainrsrc,"RANGE PROCESS FROM",VARIABLETEXT,0,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE PROCESS TO",VARIABLETEXT,0,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE STARTTIME FROM",TIMES,8,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE STARTTIME TO",TIMES,8,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE STARTDATE FROM",DATES,10,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE STARTDATE TO",DATES,10,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE ENDTIME FROM",TIMES,8,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE ENDTIME TO",TIMES,8,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE ENDDATE FROM",DATES,10,NULL,TRUE);
		addstdrsrc(mainrsrc,"RANGE ENDDATE TO",DATES,10,NULL,TRUE);
		addbtnrsrc(mainrsrc,"DELETE",TRUE,delrange,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitremtran,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,NULL,NULL,TRUE);
		setuseredit(mainrsrc);
		settransedit(mainrsrc);
		setprocedit(mainrsrc);
		setsdateedit(mainrsrc);
		setstimeedit(mainrsrc);
		setedateedit(mainrsrc);
		setetimeedit(mainrsrc);
		exit(1);
	} else if(argc>3 && !RDAstrcmp(argv[2],"-c")) 
	{ 
		tran=TempTranNEW();
		valid_args=TRUE;
		module=stralloc(argv[1]);
		for(a=3;a<argc;)
		{
			if(!RDAstrcmp(argv[a],"-t"))
			{ 
				if(argc>(a+1))
				{
					if(!RDAstrcmp(argv[a+1],"lic"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"icl"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"cli"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"cil"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"ilc"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"lci"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"cl"))
					{
						logonlys=TRUE;
						incomptrans=FALSE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"lc"))
					{
						logonlys=TRUE;
						incomptrans=FALSE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"li"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=FALSE;
					} else if(!RDAstrcmp(argv[a+1],"il"))
					{
						logonlys=TRUE;
						incomptrans=TRUE;
						comptrans=FALSE;
					} else if(!RDAstrcmp(argv[a+1],"ic"))
					{
						logonlys=FALSE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"ci"))
					{
						logonlys=FALSE;
						incomptrans=TRUE;
						comptrans=TRUE;
					} else if(!RDAstrcmp(argv[a+1],"l"))
					{
						logonlys=TRUE;
						incomptrans=FALSE;
						comptrans=FALSE;
					} else if(!RDAstrcmp(argv[a+1],"i"))
					{
						logonlys=FALSE;
						incomptrans=TRUE;
						comptrans=FALSE;
					} else if(!RDAstrcmp(argv[a+1],"c"))
					{
						logonlys=FALSE;
						incomptrans=FALSE;
						comptrans=TRUE;
					} else {
						valid_args=FALSE;
						break;
					}
					a+=2;
				} else {
					valid_args=FALSE;
					break;
				}
			} else if(!RDAstrcmp(argv[a],"-n"))
			{ 
				if(argc>(a+2))
				{
					tranfr=Ratoi(argv[a+1]);
					tranto=Ratoi(argv[a+2]);
					tranrg=TRUE;
					a+=3;
				} else {
					valid_args=FALSE;
					break;
				}
			} else if(!RDAstrcmp(argv[a],"-p"))
			{
				if(argc>(a+2))
				{
					processfr=stralloc(argv[a+1]);
					processto=stralloc(argv[a+2]);
					processrg=TRUE;
					a+=3;
				} else {
					valid_args=FALSE;
					break;
				}
			} else if(!RDAstrcmp(argv[a],"-u"))
			{
				if(argc>(a+2))
				{
					userfr=stralloc(argv[a+1]);
					userto=stralloc(argv[a+2]);
					userrg=TRUE;
					a+=3;
				} else {
					valid_args=FALSE;
					break;
				}
			} else if(!RDAstrcmp(argv[a],"-sd"))
			{
				if(argc>(a+2))
				{
					sdatefr=stralloc(argv[a+1]);
					if(RDAstrlen(sdatefr)<10)
					{
						e=Rmalloc(350);
						sprintf(e,"The FROM START DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
						prterr("ERROR: BAD FROM START DATE RANGE FORMAT!, %s",e);
						if(e!=NULL) Rfree(e);
						valid_args=FALSE;
						break;
					}
					sdateto=stralloc(argv[a+2]);
					if(RDAstrlen(sdateto)<10)
					{
						e=Rmalloc(350);
						sprintf(e,"The TO START DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
						prterr("ERROR: BAD TO START DATE RANGE FORMAT!, %s",e);
						if(e!=NULL) Rfree(e);
						valid_args=FALSE;
						break;
					}
					startdaterg=TRUE;
					a+=3;
				} else {
					valid_args=FALSE;
					break;
				}
			} else if(!RDAstrcmp(argv[a],"-st"))
			{
				if(argc>(a+2))
				{
					stimefr=stralloc(argv[a+1]);
					stimeto=stralloc(argv[a+2]);
					starttimerg=TRUE;
					a+=3;
				} else {
					valid_args=FALSE;
					break;
				}
			} else if(!RDAstrcmp(argv[a],"-ed"))
			{
				if(argc>(a+2))
				{
					edatefr=stralloc(argv[a+1]);
					if(RDAstrlen(edatefr)<10)
					{
						e=Rmalloc(350);
						sprintf(e,"The FROM END DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
						prterr("ERROR: BAD FROM END DATE RANGE FORMAT!, %s",e);
						if(e!=NULL) Rfree(e);
						valid_args=FALSE;
						break;
					}
					edateto=stralloc(argv[a+2]);
					if(RDAstrlen(edateto)<10)
					{
						e=Rmalloc(350);
						sprintf(e,"The TO END DATE value that you have selected isn't a valid date value, it must be in the full format of mm/dd/YYYY (where mm=two digit month, dd=two digit date, YYYY=four digit year - e.g. 10/31/1999) to be an acceptable range value.  Please change the format and re-run it again.");
						prterr("ERROR: BAD TO END DATE RANGE FORMAT!, %s",e);
						if(e!=NULL) Rfree(e);
						valid_args=FALSE;
						break;
					}
					enddaterg=TRUE;
					a+=3;
				} else {
					valid_args=FALSE;
					break;
				}
			} else if(!RDAstrcmp(argv[a],"-et"))
			{
				if(argc>(a+2))
				{
					etimefr=stralloc(argv[a+1]);
					etimeto=stralloc(argv[a+2]);
					endtimerg=TRUE;
					a+=3;
				} else {
					valid_args=FALSE;
					break;
				}
			} else {
				valid_args=FALSE;
				break;
			}
		}
		if(valid_args==TRUE)
		{
			removetranfiles(NULL,module,
				logonlys,incomptrans,comptrans,
				tranrg,tranto,tranfr,
				processrg,processto,processfr,
				userrg,userto,userfr,
				startdaterg,sdateto,sdatefr,
				starttimerg,stimeto,stimefr,
				enddaterg,edateto,edatefr,
				endtimerg,etimeto,etimefr);
		} else {
			print_help();
		}
		if(userto!=NULL) Rfree(userto);
		if(userfr!=NULL) Rfree(userfr);
		if(processfr!=NULL) Rfree(processfr);
		if(processto!=NULL) Rfree(processto);
		if(sdateto!=NULL) Rfree(sdateto);
		if(sdatefr!=NULL) Rfree(sdatefr);
		if(stimeto!=NULL) Rfree(stimeto);
		if(stimefr!=NULL) Rfree(stimefr);
		if(edateto!=NULL) Rfree(edateto);
		if(edatefr!=NULL) Rfree(edatefr);
		if(etimeto!=NULL) Rfree(etimeto);
		if(etimefr!=NULL) Rfree(etimefr);
	} else {
		print_help();
	}
}
