/* cron or at transaction log maintenance */
#ifndef WIN32
#define __NAM__ "crontran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "crontran.exe"
#endif
#include <app.hpp>
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
static APPlib *modlst=NULL;

#define INCOMPLETE_TRANS 0
#define LOG_ONLY_TRANS	 1
#define COMPLETE_TRANS   2 

#define CronTranInitializeSubsystems(a,b,c,d)	xCronTranInitializeSubsystems(a,b,c,d,__LINE__,__FILE__)
short xCronTranInitializeSubsystems(int argc,char **argv,char *module,char *process,int line,char *file)
{
	short x=0;
	short return_value=0;

	initrdadiag();
	if(diagmix || diagmix_field)
	{
		prterr("DIAG InitializeSubsystems Initializing Subsystems for Module [%s] Process [%s] at line [%d] program [%s].",module,process,line,file);
	}
	USERLOGIN=stralloc("root");
	SETCWD();
	XPERT_SETUP=XPERTstpNEW();
	if((getXPERTbin(XPERT_SETUP))==(-1))
	{
		prterr("ERROR: Error Invalid Version or Bad Binary for [XPERT SETUP] from Module: [%s] Process: [%s] at line [%d] program [%s].",module,process,line,file);
		return_value=(-1);
	} else {
		if(diagmix || diagapps)
		{
			prterr("DIAG XPERT.STP version=[%d], Module: [%s] Process: [%s] at line [%d] program [%s].",x,module,process,line,file);
		}
	}
#ifdef USE_MYSQL
	INITNRD_MYSQL_GSVS();
#endif /* USE_MYSQL */
	if(INITNRD()) 
	{
		prterr("ERROR: Cannot Initialize the Database Subsystem at line [%d] program [%s].",line,file);
		return_value=(-1);
	} else if(InitializeSecurity()) 
	{
		prterr("Error Cannot Initialize the Security Subsystem at line [%d] program [%s].",line,file);
		return_value=(-1);
	} else if(!PROCsecurity(module,process)) 
	{
		prterr("ERROR: Security Access to Module [%s] Process [%s] DENIED! at line [%d] program [%s].",module,process,line,file);
		return_value=(-1);
	} else {
		INITTRANS();
		OPNLOG(module,process);
	}
	return(return_value);
}
static TempTran *TempTranNEW(void)
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
static void FreeTempTran(TempTran *transact)
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

static void renumbertrans(char *module)
{
	int x,retval=0;
	char *tmp=NULL,*tmpfile=NULL,*temp=NULL;
	char *dirx=NULL;
	APPlib *trnfiles=NULL;
	char command[1048];
#ifdef WIN32
	char *error_message=NULL;
#endif 
	FILE *fp;

	trnfiles=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,module);
#endif
	for(x=0;findfile(dirx,"*.TRN",&tmp,(int)x+1);++x) 
	{
		if(RDAstrcmp(tmp,"NXTTRN.TRN"))
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(dirx)+RDAstrlen(tmp)+1);
			} else {
				temp=Rmalloc(RDAstrlen(dirx)+RDAstrlen(tmp)+1);
			}
			sprintf(temp,"%s%s",dirx,tmp);
			addAPPlib(trnfiles,temp);
		}
	} 
	if(temp!=NULL) Rfree(temp);
	temp=NULL;
	if(tmp!=NULL) Rfree(tmp);
	tmp=NULL;
	if(trnfiles->numlibs>0)
	{
		SORTAPPlib(trnfiles);
		tmpfile=Rmalloc(RDAstrlen(dirx)+16);
		for(x=0;x<trnfiles->numlibs;++x)
		{
			sprintf(tmpfile,"%s%010d.TRN",dirx,x+1);
			if(RDAstrcmp(tmpfile,trnfiles->libs[x]))
			{
#ifndef WIN32
				sprintf(command,"mv %s %s",trnfiles->libs[x],tmpfile);
				retval=system(command);
				if(retval)
				{
					temp=errname();
					tmp=Rmalloc(RDAstrlen(temp)+RDAstrlen(trnfiles->libs[x])+RDAstrlen(tmpfile)+80);
					sprintf(tmp,"Error System Move Function Failed with error [%d] [%s] attempting to move File [%s] to new name [%s].",errno,errname(),trnfiles->libs[x],tmpfile);
					prterr("%s.",tmp);
					if(tmp!=NULL) Rfree(tmp);
					break;
				}
#endif /* ifndef WIN32 */
#ifdef WIN32
/*
				sprintf(command,"move %s %s",trnfiles->libs[x],tmpfile);
				retval=system(command);
*/
				retval=MoveFile(trnfiles->libs[x],tmpfile);
				if(retval==0)
				{
					x=GetLastError();
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,x,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR) &error_message,0,NULL);
					temp=Rmalloc(RDAstrlen(trnfiles->libs[x])+RDAstrlen(tmpfile)+80+1);
					sprintf(temp,"MOVE FILE ERROR: Can Not Move File %s To %s because of error [%d] %s!?",trnfiles->libs[x],tmpfile,error_message);
					prterr(temp);
					LocalFree(error_message);
					break;
				}
#endif /* ifdef WIN32 */
			}
		}
		if(tmpfile!=NULL) Rfree(tmpfile);
	}
	tmpfile=Rmalloc(RDAstrlen(dirx)+11);
	sprintf(tmpfile,"%sNXTTRN.TRN",dirx);
	if(unlink(tmpfile)==(-1)) 
	{
		if(errno!=ENOENT)
		{
			prterr("Can't remove file(%s)%s",tmpfile,errname()); 
		}
	}
	fp=fopen(tmpfile,"w");
	if(fp==NULL)
	{
		prterr("Cannot Create file [%s].",tmpfile);
	} else {
		fprintf(fp,"%d\n",trnfiles->numlibs);
		fclose(fp);
	}
	if(dirx!=NULL) Rfree(dirx);
	if(trnfiles!=NULL) freeapplib(trnfiles);
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
		}
	}
	return(return_val);
}
/*
			removetranfiles(modlst->libs[x],
				TRUE,FALSE,FALSE,FALSE,0,0,
				FALSE,NULL,NULL,FALSE,NULL,NULL,
				FALSE,NULL,NULL,
				FALSE,NULL,NULL,FALSE,NULL,NULL,
				FALSE,NULL,NULL);
			datefrom=date60(CURRENT_DATE10);
			if(!isEMPTY(datefrom))
			{
				removetranfiles(modlst->libs[x],
					TRUE,TRUE,TRUE,FALSE,0,0,
					FALSE,NULL,NULL,FALSE,NULL,NULL,
					TRUE,datefrom,"01/01/1900",
					FALSE,NULL,NULL,FALSE,NULL,NULL,
					FALSE,NULL,NULL);
			}
*/
static short removetranfiles(char *module,
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
	short exit=FALSE,filesfound=0,deltran=FALSE;
	short type=INCOMPLETE_TRANS;
	long pos=0,curpos=0;
	int x,numtrans=0;
	char *dirx=NULL;
	RDArsrc *tmprsrc=NULL;
#ifdef _POSIX_SOURCE
#ifdef WIN32
	long offset2=-35;
	long offset=-37;
#endif
#ifndef WIN32
	long offset2=-34;
	long offset=-36;
#endif
#else
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
		if(e!=NULL) Rfree(e);
		e=NULL;
	 	for(x=0;findfile(dirx,"*.TRN",&tmp,(int)x+1);++x) 
	 	{
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
					if(testrange(tranrg,tranto,tranfr,
						processrg,processto,processfr,
						userrg,userto,userfr,
						startdaterg,sdateto,sdatefr,
						starttimerg,stimeto,stimefr,
						enddaterg,edateto,edatefr,
						endtimerg,etimeto,etimefr,
						tran))
					{
					/*
						if(startdaterg)
						{
							prterr("DR Remove File: [%s]\n",filename);
						}else{
							prterr("LOG Remove File: [%s]\n",filename);
						}
					*/
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
static void getdirectorytrans()
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
}
static char *date60(char *dat)
{
	short month=0,year=0,day=0,new_month=0,new_year=0,new_day=0;
	char *new_date=NULL;

	if(isEMPTY(dat)) return(NULL);
	month=val(&dat[0],2);
	day=val(&dat[3],2);
	year=Ratoi(&dat[6]);
	new_day=day;
	if(month==1)
	{
		new_year=year-1;
		new_month=10;
	}else if(month==2)
	{
		new_year=year-1;
		new_month=11;
	}else if(month==3)
	{
		new_year=year-1;
		new_month=12;
	} else {
		new_month=month-2;
		new_year=year;
	}
	new_date=Rmalloc(11);
	/* prterr("\nDATE60 Returns: [%02d/%02d/%04d]\n\n",new_month,new_day,new_year); */
	sprintf(new_date,"%02d/%02d/%04d",new_month,new_day,new_year);
	return(new_date);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	char *datefrom=NULL;

	if(CronTranInitializeSubsystems(argc,argv,"DATABASE","CRON MAINTENANCE TRANSACTIONS")) 
	{
		return;
	}
	getdirectorytrans();
	tran=TempTranNEW();
	if(modlst!=NULL)
	{
		for(x=0;x<modlst->numlibs;++x)
		{
			removetranfiles(modlst->libs[x],
				TRUE,FALSE,FALSE,FALSE,0,0,
				FALSE,NULL,NULL,FALSE,NULL,NULL,
				FALSE,NULL,NULL,
				FALSE,NULL,NULL,FALSE,NULL,NULL,
				FALSE,NULL,NULL);
			datefrom=date60(CURRENT_DATE10);
			if(!isEMPTY(datefrom))
			{
				removetranfiles(modlst->libs[x],
					TRUE,TRUE,TRUE,FALSE,0,0,
					FALSE,NULL,NULL,FALSE,NULL,NULL,
					TRUE,datefrom,"01/01/1900",
					FALSE,NULL,NULL,FALSE,NULL,NULL,
					FALSE,NULL,NULL);
			}
			renumbertrans(modlst->libs[x]);
		}
	}
	if(tran!=NULL) FreeTempTran(tran);
	if(modlst!=NULL) freeapplib(modlst);
	ShutdownSecurity();
	CLSLOG();
	EXITNRD();
	if(CURRENTDIRECTORY!=NULL) Rfree(CURRENTDIRECTORY);
	if(CURRENT_DATE!=NULL) Rfree(CURRENT_DATE);
	if(CURRENT_DATE10!=NULL) Rfree(CURRENT_DATE10);
	if(CURRENT_TIME!=NULL) Rfree(CURRENT_TIME);
	free_xpert(XPERT_SETUP);
#ifdef WIN32
	if(RDA_STDERR!=NULL) fclose(RDA_STDERR);
#endif
	std::exit;
}
