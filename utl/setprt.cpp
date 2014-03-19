/* setprt.c - Setup Output Devices */
#ifndef WIN32
#define __NAM__ "setprt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "setprt.exe"
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

static char *module="RPTGEN";
static APPlib *out_devs=NULL,*printers=NULL;
static void getprinters(RDArsrc *);

static APPlib *GetDevNames()
{
	char *libx=NULL;
	APPlib *out_devs1=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	out_devs1=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	return(out_devs1);
}

void getpdeventries(RDArsrc *mainrsrc)
{
	if(out_devs!=NULL) freeapplib(out_devs);
	out_devs=GetDevNames();
	if(out_devs==NULL)
	{
		out_devs=APPlibNEW();
		addAPPlib(out_devs,"Contains No Output Devices");
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"OUTPUT DEVICE LIST",0,out_devs))
		updatersrc(mainrsrc,"OUTPUT DEVICE LIST");
}
static void quitpdev(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	getprinters(mainrsrc);
	if(out_devs!=NULL) freeapplib(out_devs);
}
void savepdev(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x=0;
	char *name=NULL,asp=FALSE;
	RDApdev *pdev=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"DEVICE NAME",&name);
	FINDRSCGETINT(mtnrsrc,"OUTPUT DEVICE LIST",&x);
	pdev=RDApdevNEW(out_devs->libs[x]);
	if(getpdevbin(pdev))
	{
		ERRORDIALOG("Cannot Read Output Device","Error in permissions, the user cannot read the output device binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		if(pdev!=NULL) free_pdev(pdev);
		if(name!=NULL) Rfree(name);
		return;
	}
	if(pdev->name!=NULL) Rfree(pdev->name);
	FINDRSCGETCHAR(mtnrsrc,"ASP PRINTER",&asp);
	if(asp)
	{
		pdev->name=Rmalloc(RDAstrlen(name)+17);
		sprintf(pdev->name,"asp_printer.lnx %s",(name!=NULL ? name:""));
	} else pdev->name=stralloc(name);
	pdev->active=TRUE;
	FINDRSCGETCHAR(mtnrsrc,"FORMFEED",&pdev->formfeed);
	FINDRSCGETCHAR(mtnrsrc,"58 LINE SHEET FEEDER",&pdev->line_58);
	writepdevbin(pdev);
	if(pdev!=NULL) free_pdev(pdev);
	if(name!=NULL) Rfree(name);
	quitpdev(mtnrsrc,mainrsrc); 
} 
void setprt(RDArsrc *mainrsrc,short *which)
{
	int x=0,s=0;
	RDArsrc *mtnrsrc=NULL;
	char *temp=NULL;
	char ff=FALSE,asp=FALSE;

	if(*which==0)
	{
		FINDRSCGETINT(mainrsrc,"PRINTERS",&s);
		if(printers->numlibs>s)
		{
			temp=printers->libs[s];
		}
	} 
	mtnrsrc=RDArsrcNEW(module,"EDIT SETUP DEVICE");
	out_devs=APPlibNEW();
	if(temp!=NULL)
	{
		if(!strncmp(temp,"asp_printer.lnx",15)) 
		{
			temp+=15;
			asp=TRUE;
		}
	}
	addstdrsrc(mtnrsrc,"DEVICE NAME",VARIABLETEXT,0,temp,(*which==0 ? FALSE:TRUE));
	addlstrsrc(mtnrsrc,"OUTPUT DEVICE LIST",&x,TRUE,NULL,out_devs->numlibs,
		&out_devs->libs,NULL);
	getpdeventries(mtnrsrc);
	addstdrsrc(mtnrsrc,"FORMFEED",BOOLNS,1,&ff,TRUE);
	addstdrsrc(mtnrsrc,"58 LINE SHEET FEEDER",BOOLNS,1,&ff,TRUE);
	addstdrsrc(mtnrsrc,"ASP PRINTER",BOOLNS,1,&asp,TRUE);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,savepdev,mainrsrc);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quitpdev,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mtnrsrc,TRUE,quitpdev,mainrsrc,FALSE);
}
void quitprt(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL)
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	if(printers!=NULL) freeapplib(printers);
	ShutdownSubsystems();
}
static void getprinters(RDArsrc *mainrsrc)
{
#ifdef RISC6000
	FILE *p=NULL;
	int x=0;
	char *temp=NULL;
#endif
#ifdef LINUX
	char *temp1=NULL,*temp2=NULL;
	FILE *p=NULL;
	int x=0;
	char *temp=NULL;
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	char *temp1=NULL,*temp2=NULL;
	FILE *p=NULL;
	int x=0;
	char *temp=NULL;
#endif
#ifdef WIN32
	int x=0;
	DWORD rsize=0,asize=0,count=0,er=0;
	LPPRINTER_INFO_5 en_data=NULL,en1=NULL;
#endif 
	char *libx=NULL;
	APPlib *tmplist=NULL;
	RDApdev *pdev=NULL;



	if(printers!=NULL) freeapplib(printers);
	printers=APPlibNEW();

#ifdef RISC6000
	addAPPlibNoDuplicates(printers,"rdaout.rsc");
	sprintf(stemp,"lpstat -p");
	p=popen(stemp,"r");
	if(p!=NULL)
	{
		temp=Rmalloc(1024+1);
		x=0;
		while(fgets(temp,1024,p))
		{
			if(x>2)
			{
				temp[7]=0;
				memset(stemp,0,101);
				sprintf(stemp,"qprt -d p -P %s",temp);
				addAPPlibNoDuplicates(printers,stemp);
			}
			++x;
		}
		pclose(p);
	}
#endif
#ifdef UNIXWARE21
	addAPPlibNoDuplicates(printers,"rdaout.unw");
#endif
#ifdef UNIXWARE7
	addAPPlibNoDuplicates(printers,"rdaout.unw");
#endif
#ifdef WIN32
	addAPPlibNoDuplicates(printers,"rdaout.exe");
	EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,NULL,0,&rsize,&count);
	if(rsize>0)
	{
		asize=rsize;	
		en_data=Rmalloc(asize+1);
		er=EnumPrinters(PRINTER_ENUM_LOCAL,NULL,5,en_data,asize,&rsize,&count);
		if(er)
		{
			for(er=0,en1=en_data;er<count;++er,++en1)
			{
				if(!isEMPTY(en1->pPrinterName))
				{
					addAPPlibNoDuplicates(printers,en1->pPrinterName);
				}
			}		
		}
	}
#endif
#if (defined(SCO50) || defined(SCO50P))
#endif
#ifdef LINUX
	addAPPlibNoDuplicates(printers,"rdaout.lnx");
	addAPPlibNoDuplicates(printers,"rdareportcd.lnx");
	addAPPlibNoDuplicates(printers,"sendattach-2.lnx");
	addAPPlibNoDuplicates(printers,"outputemail.lnx");
	sprintf(stemp,"lpstat -v | cut -f3 -d' '");
	p=popen(stemp,"r");
	if(p!=NULL)
	{
		temp=Rmalloc(1024+1);
		temp2=Rmalloc(1024+1);
		x=0;
		while(fgets(temp,1024,p))
		{
			temp1=temp;
			memset(temp2,0,1025);
			for(;*temp1;++temp1) 
			{
				if((*temp1==':'))
				{
					*temp1=0;
					memset(stemp,0,101);
					sprintf(stemp,"lpr -h -P %s",temp2);
					addAPPlibNoDuplicates(printers,stemp);
					memset(temp2,0,1025);
					break;
				}else{
					temp2[x]=*temp1;
					++x;
				}
			}
			x=0;
		}
		pclose(p);
		if(temp2!=NULL) Rfree(temp2);
	}	
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	addAPPlibNoDuplicates(printers,"rdaout.lnx");
	addAPPlibNoDuplicates(printers,"rdareportcd.lnx");
	addAPPlibNoDuplicates(printers,"outputemail.lnx");
	sprintf(stemp,"lpstat -v | cut -f3 -d' '");
	p=popen(stemp,"r");
	if(p!=NULL)
	{
		temp=Rmalloc(1024+1);
		temp2=Rmalloc(1024+1);
		x=0;
		while(fgets(temp,1024,p))
		{
			temp1=temp;
			memset(temp2,0,1025);
			for(;*temp1;++temp1) 
			{
				if((*temp1==':'))
				{
					*temp1=0;
					memset(stemp,0,101);
					sprintf(stemp,"lpr -h -P %s",temp2);
					addAPPlibNoDuplicates(printers,stemp);
					memset(temp2,0,1025);
					break;
				}else {
					temp2[x]=*temp1;
					++x;
				}
			}
			x=0;
		}
		pclose(p);
		if(temp2!=NULL) Rfree(temp2);
	}	
#endif
#ifdef SUNX86
	addAPPlibNoDuplicates(printers,"rdaout.sun");
#endif
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	tmplist=getlibnames(libx,FALSE);
	if(tmplist!=NULL)
	{
		for(x=0;x<tmplist->numlibs;++x)
		{
			pdev=RDApdevNEW(tmplist->libs[x]);
			if(getpdevbin(pdev))
			{
			} else if(pdev->active==TRUE)
			{
				addAPPlibNoDuplicates(printers,tmplist->libs[x]);
			}
			if(pdev!=NULL) free_pdev(pdev);
		}
		freeapplib(tmplist);
	}
	if(libx!=NULL) Rfree(libx);
	if(!FINDRSCLISTAPPlib(mainrsrc,"PRINTERS",0,printers))
		updatersrc(mainrsrc,"PRINTERS");
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;
	short a=1,s=0;

	if(InitializeSubsystems(argc,argv,module,"SETUP OUTPUT DEVICES")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"SETUP OUTPUT DEVICES");
	printers=APPlibNEW();
	addlstrsrc(mainrsrc,"PRINTERS",&x,TRUE,NULL,printers->numlibs,
		&printers->libs,NULL);
	getprinters(mainrsrc);
	addbtnrsrc(mainrsrc,"ADD",TRUE,setprt,&a);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,setprt,&s);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitprt,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitprt,NULL,TRUE);
}
