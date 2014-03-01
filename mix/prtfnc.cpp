/* prtfnc.c - Print Functions */
#include<mix.hpp>
#ifdef SCO50P
#include<cctype>
#endif
#if (!defined(UNIXWARE21) && !defined(UNIXWARE7))
#include <sys/stat.h>
#include <fcntl.h>
#else
#include <sys/stat.h>
#endif

APPlib *MYout_devs=NULL;
char *VENPMT_SIGNATURE_FILE=NULL,*VENPMT_PASSWORD=NULL;
char *PAYROLL_SIGNATURE_FILE=NULL,*PAYROLL_PASSWORD=NULL;
char *PURORD_SIGNATURE_FILE=NULL,*PURORD_PASSWORD=NULL;


short getpdevbin(RDApdev *pdev)
{
	short x,version=FALSE,test;
	int y;
	BIN *bin;
	char *libname=NULL,*test1=NULL;
	RDApvar *pvar;

	if(diagmix)
	{
		prterr("DIAG getpdevbin Retrieving Output Device Binary [%s].",pdev->name);
	}
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libname,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	bin=xgetlibbin(libname,pdev->name,TRUE,__LINE__,__FILE__);
	if(bin==NULL) 
	{
		if(diagmix)
		{
			prterr("Error Output Device Binary [%s] not found in Output Device Library [PRTDEV.STP]",pdev->name);
		}
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	x=BINgetshort(bin);
	if(x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1699) && x!=(-1700) && x!=(-1701) && x!=(-1702))
	{
		prterr("Error Invalid version or bad binary in Output Device Binary [%s] in Output Device Library [PRTDEV.STP].",pdev->name);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1695)) version=7;
	else if(x==(-1696)) version=6;
	else if(x==(-1697)) version=5;
	else if(x==(-1698)) version=4;
	else if(x==(-1699)) version=3;
	else if(x==(-1700)) version=2;
	else if(x==(-1702)) version=TRUE;
	else version=FALSE;
	if(version)
	{
		test=BINgetshort(bin);
		if(test) 
		{
			test1=BINgetstring(bin);
			if(test1!=NULL) Rfree(test1);
		}
	} else {
		test1=BINgetstring(bin);
		if(test1!=NULL) Rfree(test1);
	}
	if(version>2)
	{
		test=BINgetshort(bin);
		if(test) pdev->formfeed=TRUE;
			else pdev->formfeed=FALSE;
	} else pdev->formfeed=FALSE;
	if(version>3)
	{
		test=BINgetshort(bin);
		if(test) pdev->line_58=TRUE;
			else pdev->line_58=FALSE;
	} else pdev->line_58=FALSE;
	if(version>6)
	{
		test=BINgetshort(bin);
		if(test) pdev->active=TRUE;
			else pdev->active=FALSE;
	} else pdev->active=FALSE;
	if(version>4)
	{
		if(version>5)
		{
			pdev->page_length=BINgetdouble(bin);
		} else pdev->page_length=BINgetshort(bin);
	} else pdev->page_length=0.0;
	pdev->num=BINgetint(bin);
	if(pdev->num>0)
	{
		pdev->pvar=Rmalloc(pdev->num*sizeof(RDApvar));
		for(y=0,pvar=pdev->pvar;y<pdev->num;++y,++pvar)
		{
			if(version)
			{
				test=BINgetshort(bin);
				if(test) pvar->name=BINgetstring(bin);
					else pvar->name=NULL;
			} else pvar->name=BINgetstring(bin);
			if(version>1)
			{
				pvar->type=BINgetint(bin);
			} else pvar->type=0;
			if(version)
			{
				test=BINgetshort(bin);
				if(test) pvar->command=BINgetstring(bin);
					else pvar->command=NULL;
			} else pvar->command=BINgetstring(bin);
			pvar->chgsize=BINgetshort(bin);
			if(pvar->chgsize)
			{
				pvar->pitch=BINgetshort(bin);
				pvar->points=BINgetshort(bin);
			}
		}
	}
	BINfree(bin);
	return(0);
}
short xwritepdevbin(RDApdev *pdev,int line,char *file)
{
	int y;
	BIN *bin;
	char *libname=NULL;
	RDApvar *pvar;

	if(diagmix)
	{
		prterr("DIAG writepdevbin Writing Device binary [%s] in Output Device Library [PRTDEV.STP] at line [%d] file [%s].",(pdev->name!=NULL ? pdev->name:""),line,file);
	}
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libname,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	bin=BINnew();
	BINaddshort(bin,-1695);	/*type identifier*/
	if(!isEMPTY(pdev->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pdev->name);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,(pdev->formfeed ? TRUE:FALSE));
	BINaddshort(bin,(pdev->line_58 ? TRUE:FALSE));
	BINaddshort(bin,(pdev->active ? TRUE:FALSE));
	BINadddouble(bin,pdev->page_length);
	BINaddint(bin,pdev->num);
	for(y=0,pvar=pdev->pvar;y<pdev->num;++y,++pvar)
	{
		if(!isEMPTY(pvar->name))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,pvar->name);
		} else BINaddshort(bin,FALSE);
		BINaddint(bin,pvar->type);
		if(!isEMPTY(pvar->command))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,pvar->command);
		} else BINaddshort(bin,FALSE);
		BINaddshort(bin,pvar->chgsize);
		if(pvar->chgsize)
		{
			BINaddshort(bin,pvar->pitch);
			BINaddshort(bin,pvar->points);
		}
	}
	if(writelibbin(libname,bin,pdev->name))
	{
		prterr("Error Can't write device binary [%s] into device library [%s].",(pdev->name!=NULL ? pdev->name:""),libname);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
RDApdev *xRDApdevNEW(char *name,int line,char *file)
{
	RDApdev *tmp;

	if(diagmix)
	{
		prterr("DIAG RDApdevNEW Creating an Output Device (RDApdev *) structure for [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,file);
	}
	tmp=Rmalloc(sizeof(RDApdev));
	if(!isEMPTY(name))
	{
		tmp->name=stralloc(name);
	} else tmp->name=NULL;
	tmp->num=0;
	tmp->pvar=NULL;
	tmp->formfeed=FALSE;
	tmp->line_58=FALSE;
	tmp->active=FALSE;
	tmp->page_length=10.0;
	return(tmp);
}
void xfree_pdev(RDApdev *pdev,int line,char *file)
{
	int x;
	RDApvar *pvar;

	if(diagmix)
	{
		prterr("DIAG free_pdev Freeing Memory for Output Device (RDApdev *) Structure [%s] at line [%d] program [%s].",pdev->name,line,file);
	}
	if(pdev!=NULL)
	{
		if(pdev->name!=NULL) Rfree(pdev->name);
		if(pdev->pvar!=NULL)
		{
			for(x=0,pvar=pdev->pvar;x<pdev->num;++x,++pvar)
			{
				if(pvar->name!=NULL) Rfree(pvar->name);
				if(pvar->command!=NULL) Rfree(pvar->command);
			}
			Rfree(pdev->pvar);
		}
		Rfree(pdev);
	}
}
static void GetPrintDeviceEntries(RDArsrc *mainrsrc,short which)
{
	char *libx=NULL;
	APPlib *list=NULL;
	RDApdev *tmp=NULL;
	int x=0;


	freeapplib(MYout_devs);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	list=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	MYout_devs=APPlibNEW();
	if(which==1)
	{
		addAPPlib(MYout_devs,"AUTOARCHIVE");
	}
	if(list!=NULL)
	{
		for(x=0;x<list->numlibs;++x)
		{
			tmp=RDApdevNEW(list->libs[x]);
			getpdevbin(tmp);
			if(tmp->active==TRUE)
			{
				addAPPlib(MYout_devs,tmp->name);
			}
			if(tmp!=NULL) free_pdev(tmp);
		}
	}
	if(MYout_devs->numlibs<1)
	{
		addAPPlib(MYout_devs,"Contains No Output Devices");
	}
	if(list!=NULL) freeapplib(list);
	if(!FINDRSCLISTAPPlib(mainrsrc,"OUTPUT DEVICE LIST",0,MYout_devs))
		updatersrc(mainrsrc,"OUTPUT DEVICE LIST");
}
void quitSelectDevice(RDArsrc *r)
{
	if(MYout_devs!=NULL) freeapplib(MYout_devs);
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
}
void SelectFuncDeviceName(RDArsrc *r,RDArsrc *prtrsrc)
{
	int x=0;

	FINDRSCGETINT(r,"OUTPUT DEVICE LIST",&x);
	if(x<=MYout_devs->numlibs)
	{
		FINDRSCSETSTRING(prtrsrc,"DEVICE NAME",MYout_devs->libs[x]);
		updatersrc(prtrsrc,"DEVICE NAME");
	}
	quitSelectDevice(r);
}
void SelectFuncDevice(RDArsrc *r,RDArsrc *prtrsrc)
{
	int x=0;

	FINDRSCGETINT(r,"OUTPUT DEVICE LIST",&x);
	if(x<=MYout_devs->numlibs)
	{
		FINDRSCSETSTRING(prtrsrc,"DEVICE",MYout_devs->libs[x]);
		updatersrc(prtrsrc,"DEVICE");
	}
	quitSelectDevice(r);
}
void SelectFuncOutputDevice(RDArsrc *r,RDArsrc *prtrsrc)
{
	int x=0;

	FINDRSCGETINT(r,"OUTPUT DEVICE LIST",&x);
	if(x<=MYout_devs->numlibs)
	{
		FINDRSCSETSTRING(prtrsrc,"OUTPUT DEVICE",MYout_devs->libs[x]);
		updatersrc(prtrsrc,"OUTPUT DEVICE");
	}
	quitSelectDevice(r);
}
void LoadDeviceWindowAutoArchive(RDArsrc *prtrsrc,void (*selectfunc)(...))
{
	RDArsrc *tmp=NULL;
	int x=0;

	tmp=RDArsrcNEW("RPTGEN","LOAD DEVICE WINDOW");
	if(MYout_devs!=NULL) freeapplib(MYout_devs);
	MYout_devs=APPlibNEW();
	addlstrsrc(tmp,"OUTPUT DEVICE LIST",&x,TRUE,NULL,MYout_devs->numlibs,
		&MYout_devs->libs,NULL);
	GetPrintDeviceEntries(tmp,TRUE);
	addbtnrsrc(tmp,"SELECT",TRUE,selectfunc,prtrsrc);
	addrfcbrsrc(tmp,"QUIT",TRUE,quitSelectDevice,NULL);
	addrfcbrsrc(tmp,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(tmp,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(tmp,TRUE,quitSelectDevice,NULL,TRUE);
}
void LoadDeviceWindow(RDArsrc *prtrsrc,void (*selectfunc)(...))
{
	RDArsrc *tmp=NULL;
	int x=0;

	tmp=RDArsrcNEW("RPTGEN","LOAD DEVICE WINDOW");
	if(MYout_devs!=NULL) freeapplib(MYout_devs);
	MYout_devs=APPlibNEW();
	addlstrsrc(tmp,"OUTPUT DEVICE LIST",&x,TRUE,NULL,MYout_devs->numlibs,
		&MYout_devs->libs,NULL);
	GetPrintDeviceEntries(tmp,FALSE);
	addbtnrsrc(tmp,"SELECT",TRUE,selectfunc,prtrsrc);
	addrfcbrsrc(tmp,"QUIT",TRUE,quitSelectDevice,NULL);
	addrfcbrsrc(tmp,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(tmp,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(tmp,TRUE,quitSelectDevice,NULL,TRUE);
}
char *PRT_Text_File(char *rname)
{
	FILE *fp=NULL;
	char temp[513],*stempx;
	char *rv=NULL;
	int y;
	int count=0;

	if(isEMPTY(rname)) return(NULL);
	stempx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(rname)+5);
#ifndef WIN32
	sprintf(stempx,"%s/%s",CURRENTDIRECTORY,rname);
#endif
#ifdef WIN32
	sprintf(stempx,"%s\\%s",CURRENTDIRECTORY,rname);
#endif
	fp=fopen(stempx,"r");
	if(fp!=NULL)
	{
		while(true)
		{
			y=fread(temp,1,512,fp);
			if(y<0)
			{
				y=0;
				temp[0]=0;
				break;
			}
			temp[y]=0;
			if(y<1) break;
			if(rv!=NULL)
			{
				rv=Rrealloc(rv,count+y+1);
			} else {
				rv=Rmalloc(y+1);
				count=0;
			}
			sprintf(&rv[count],"%s",temp);
			count+=y;
		}
		fclose(fp);
	}
	return(rv);
}
#ifdef WIN32
typedef struct _stat Statb;
#endif
#ifndef WIN32
typedef struct stat Statb;
#endif
void *PRT_Binary_File(char *rname,int *len)
{
	int fd=(-1);
	int length=0;
	Statb statb;
	char *temp=NULL,*stempx=NULL;

	if(isEMPTY(rname)) return(NULL);
	stempx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(rname)+8);
#ifndef WIN32
	sprintf(stempx,"%s/%s",CURRENTDIRECTORY,rname);
#endif
#ifdef WIN32
	sprintf(stempx,"%s\\%s",CURRENTDIRECTORY,rname);
#endif
#ifndef WIN32
#ifndef LINUX_SUSE6_1
	if((stat(stempx,&statb)==(-1)))
#endif 
#ifdef LINUX_SUSE6_1
	if((_xstat(_STAT_VER,stempx,&statb)==(-1)))
#endif
	{
		if(diagmix)
		{
			prterr("Error:  Failed to obtain file statistics [%s].",stempx);
		}
		if(stempx!=NULL) Rfree(stempx);
		return(NULL);
	}
	if(statb.st_size<1)
	{
		if(diagmix)
		{
			prterr("Error:  Failed to obtain file size [%s].",stempx);
		}
		if(stempx!=NULL) Rfree(stempx);
		return(NULL);
	}
	fd=Ropen(stempx);
#endif
#ifdef WIN32
	if((stat(stempx,&statb)==(-1)))
	{
		if(diagmix)
		{
			prterr("Error:  Failed to obtain file statistics [%s].",stempx);
		}
		if(stempx!=NULL) Rfree(stempx);
		return(NULL);
	}
	if(statb.st_size<1)
	{
		if(diagmix)
		{
			prterr("Error:  Failed to obtain file size [%s].",stempx);
		}
		if(stempx!=NULL) Rfree(stempx);
		return(NULL);
	}
	fd=Ropen(stempx);
#endif
	if(fd==(-1)) 
	{
		if(diagmix)
		{
			prterr("Error:  Failed to obtain file handle [%s].",stempx);
			if(stempx!=NULL) Rfree(stempx);
		}
		return(0);
	} else {
			if(stempx!=NULL) Rfree(stempx);
	}
	length=(int)(statb.st_size);
	*len=length;
	temp=Rmalloc(length+1);
	memset(temp,0,length+1);
	if(Rread(fd,temp,length))
	{
		if(diagmix)
		{
			prterr("Error:  Failed to read file.");
		}
		if(temp!=NULL) Rfree(temp);
	}
	Rclose(fd);
	return((void *)temp);
}
static void print_font(RDApdev *pdev,char *font,char **holdvarx)
{
	RDApvar *pvar;
	int x,y,len=0;
	char *temp=NULL,*hold=NULL,*holdx=NULL;

	if(diagmix)
	{
		prterr("DIAG print_font Substitue Font Command for Font Name [%s].",font);
	}
	if(!RDAstrcmp(font,"FORMFEED"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\f");
	} else if(!RDAstrcmp(font,"LINEFEED"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\n");
	} else if(!RDAstrcmp(font,"CARRIAGE RETURN"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\r");
	} else if(!RDAstrcmp(font,"TAB"))
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx,"\t");
	} else if(pdev->pvar==NULL)
	{
		if(RDAstrlen(*holdvarx)<2 || *holdvarx==NULL)
		{
			*holdvarx=Rrealloc(*holdvarx,2);
			memset(*holdvarx,0,2);
		}
		sprintf(*holdvarx," ");
	} else {
		for(x=0,pvar=pdev->pvar;x<pdev->num;++x,++pvar)
		{
			if(!RDAstrcmp(pvar->name,font)) break;
		}
		if(x<pdev->num)
		{
			if(pvar->type==0)
			{
				if(RDAstrlen(*holdvarx)<RDAstrlen(pvar->command) || *holdvarx==NULL)
				{
					*holdvarx=Rrealloc(*holdvarx,RDAstrlen(pvar->command)+1);
					memset(*holdvarx,0,RDAstrlen(pvar->command)+1);
				}
				holdx=*holdvarx;
				hold=stralloc(pvar->command);
				memset(hold,0,RDAstrlen(hold)+1);
				temp=pvar->command;
				while(*temp)
				{
					if(*temp=='\\')
					{
						y=0;
						++temp;
						while(isdigit(*temp))
						{
							hold[y]=*temp;
							++temp;++y;
						}
						hold[y]=0;
						if(!isEMPTY(hold))
						{
							x=Ratoi(hold);
						} else x=0;
						if(x>0 && x<255) *holdx=x;
					} else {
						*holdx=*temp;
						++temp;
					}
					++holdx;
				}
				if(hold!=NULL) Rfree(hold);
			} else if(pvar->type==1) /* file substitution */
			{
				if(!isEMPTY(pvar->command))
				{
				hold=PRT_Text_File(pvar->command);
				if(!isEMPTY(hold))
				{
					*holdvarx=Rrealloc(*holdvarx,RDAstrlen(hold)+1);
					memset(*holdvarx,0,RDAstrlen(hold)+1);
					memcpy(*holdvarx,hold,RDAstrlen(hold)+1);
				} else {
					prterr("Error File not found ....");
					sprintf(*holdvarx," ");
				}
				if(hold!=NULL) Rfree(hold);
				} else {
					sprintf(*holdvarx,"");
				}
			} else if(pvar->type==2) /* binary substitution */
			{
				if(!isEMPTY(pvar->command))
				{
					hold=PRT_Binary_File(pvar->command,&len);
					*holdvarx=Rrealloc(*holdvarx,len+1);
					memset(*holdvarx,0,len+1);
					memcpy(*holdvarx,hold,len);
					if(hold!=NULL) Rfree(hold);
				}
			} else {
				prterr("Error Unknown RDApvar Type....");
				sprintf(*holdvarx," ");
			}
		} else {
			sprintf(*holdvarx," ");
		}
	}
	if(diagmix)
	{
		prterr("DIAG print_font Font Name [%s] returns command [%s].",font,*holdvarx);
	}
}
void xSelectPrintType(char *printer,char normal,RDA_PFILE *o,int line,char *file)
{
	RDApdev *pdev=NULL;
	RDApvar *Pvar=NULL;
	char *HoldString=NULL;
	int u=0;

	if(diagmix)
	{
		prterr("DIAG SelectPrintType for [%s] [%s] at line [%d] program [%s].",(printer!=NULL ? printer:""),(normal ? "Normal":"Compressed"),line,file);
	}
	pdev=RDApdevNEW(printer);
	getpdevbin(pdev);
	if(pdev!=NULL)
	{
	if(normal==0)
	{
		if(pdev->pvar!=NULL)
		{
			for(u=0,Pvar=pdev->pvar;u<pdev->num;++u,++Pvar)
			{
				if(!RDAstrcmp(Pvar->name,"NORMAL TEXT")) break;
			}
			if(u<pdev->num)
			{
				HoldString=Rmalloc(256);
				print_font(pdev,Pvar->name,&HoldString);
				RDA_fprintf(o,"%s",HoldString);
				if(HoldString!=NULL) Rfree(HoldString);
				fflush(o->pfile);
			}
		}
	} else {			
		if(pdev->pvar!=NULL)
		{
			for(u=0,Pvar=pdev->pvar;u<pdev->num;++u,++Pvar)
			{
				if(!RDAstrcmp(Pvar->name,"COMPRESSED TEXT")) break;
			}
			if(u<pdev->num)
			{
				HoldString=Rmalloc(256);
				print_font(pdev,Pvar->name,&HoldString);
				RDA_fprintf(o,"%s",HoldString);
				if(HoldString!=NULL) Rfree(HoldString);
				fflush(o->pfile);
			}
		}
	}
	}
	if(pdev!=NULL) free_pdev(pdev);
}
void xSelectPrintTypeByName(char *printer,char *command_name,RDA_PFILE *o,int line,char *file)
{
	RDApdev *pdev=NULL;
	RDApvar *Pvar=NULL;
	char *HoldString=NULL,*temp=NULL;
	int u=0,len=0;
	int s=0;
	char temp1[512];
	FILE *fpx=NULL;

	if(diagmix)
	{
		prterr("DIAG SelectPrintTypeByName for Printer: [%s]  Command: [%s] at line [%d] program [%s].",
			(printer!=NULL?printer:""),
			(command_name!=NULL?command_name:""),
			line,file);
	}
	pdev=RDApdevNEW(printer);
	getpdevbin(pdev);
	if(pdev!=NULL)
	{
	if(pdev->pvar!=NULL)
	{
		for(u=0,Pvar=pdev->pvar;u<pdev->num;++u,++Pvar)
		{
			if(!RDAstrcmp(Pvar->name,command_name)) break;
		}
		if(u<pdev->num)
		{
			if(!RDAstrcmp(command_name,"LOAD VENPMT SIGNATURE"))
			{
				memset(temp1,0,512);
				sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",(VENPMT_PASSWORD!=NULL ? VENPMT_PASSWORD:""),(VENPMT_SIGNATURE_FILE!=NULL ? VENPMT_SIGNATURE_FILE:""));
				fpx=popen(temp1,"r");
				if(fpx!=NULL)
				{
					while(true)
					{
						s=getc(fpx);
						if(s==EOF) break;
						RDA_fprintf(o,"%c",s);
					}
					pclose(fpx);
				} 
			} else if(!RDAstrcmp(command_name,"LOAD PAYROLL SIGNATURE"))
			{
				memset(temp1,0,512);
				sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",(PAYROLL_PASSWORD!=NULL ? PAYROLL_PASSWORD:""),(PAYROLL_SIGNATURE_FILE!=NULL ? PAYROLL_SIGNATURE_FILE:""));
				fpx=popen(temp1,"r");
				if(fpx!=NULL)
				{
					while(true)
					{
						s=getc(fpx);
						if(s==EOF) break;
						RDA_fprintf(o,"%c",s);
					}
					pclose(fpx);
				} 
			} else if(!RDAstrcmp(command_name,"LOAD PO SIGNATURE"))
			{
				memset(temp1,0,512);
				sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",(PURORD_PASSWORD!=NULL ? PURORD_PASSWORD:""),(PURORD_SIGNATURE_FILE!=NULL ? PURORD_SIGNATURE_FILE:""));
				fpx=popen(temp1,"r");
				if(fpx!=NULL)
				{
					while(true)
					{
						s=getc(fpx);
						if(s==EOF) break;
						RDA_fprintf(o,"%c",s);
					}
					pclose(fpx);
				} 
			} else {
				if(Pvar->type==2)
				{
					HoldString=PRT_Binary_File(Pvar->command,&len);
					for(s=0,temp=HoldString;s<len;++s,++temp)
					{
						RDA_fprintf(o,"%c",*temp);
					}
				} else {
					HoldString=Rmalloc(256);
					print_font(pdev,Pvar->name,&HoldString);
					RDA_fprintf(o,"%s",HoldString);
				}
				if(HoldString!=NULL) Rfree(HoldString);
				fflush(o->pfile);
			}
		}
	}
	}
	if(pdev!=NULL) free_pdev(pdev);
}
