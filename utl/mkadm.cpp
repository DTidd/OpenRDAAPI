/*********************************************************************
                        MAKE ADMINISTRATION ITEMS
                           mkadm.c
**********************************************************************/
#ifndef WIN32
#define __NAM__ "mkadm.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkadm.exe"
#endif
#include <app.hpp>
#include <doheader.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>
#include <curver.hpp>
/*CPP_OPART mkadm1 */
/*CPP_OPART mkadm2 */
/*CPP_OPART mkadm3 */
/*CPP_OPART mkadm4 */

/*ADDLIB cver */
/*ADDLIB mklib */
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX && LINUX2_2 */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX || LINUX2_2 || UBUNTU_OS */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

struct RPTSTRUCTs
{
	short type;
        char *name;
        char *desc;
        void (*func)(...);
}; 
typedef struct RPTSTRUCTs RPTSTRUCT;

struct MAKE_STRUCTs
{
	int num;
	RPTSTRUCT *RptStruct;
}; 
typedef struct MAKE_STRUCTs MAKESTRUCT;

MAKESTRUCT *MakeStructNEW(void);
MAKESTRUCT *MakeStructFREE(MAKESTRUCT *);
int addRPTSTRUCT(MAKESTRUCT *,short,char *,char *,void (*)(...));


char *module="UTILITIES";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
char Conversion=FALSE;
static void make_screens(int,char **,MAKESTRUCT *);
static void mk_diagscrn(void);
static void MAKE_ADMIN_SCREEN(void);
static void checkheader(FILE *,int *,int *,char *);
static void printheader(FILE *,int *,int *,char *,char *);

MAKESTRUCT *MakeStructNEW()
{
	MAKESTRUCT *MakeStruct=NULL;

	MakeStruct=Rmalloc(sizeof(MAKESTRUCT));
	MakeStruct->RptStruct=NULL;
	MakeStruct->num=0;
	return(MakeStruct);
}

MAKESTRUCT *MakeStructFREE(MAKESTRUCT *MakeStruct)
{
	int x=0;
	RPTSTRUCT *RptStruct=NULL;

	if(MakeStruct!=NULL)
	{
		if(MakeStruct->RptStruct!=NULL)
		{
			for(x=0;x<MakeStruct->num;++x)
			{
				RptStruct=MakeStruct->RptStruct+x;
				if(RptStruct!=NULL)
				{
					if(RptStruct->name!=NULL) Rfree(RptStruct->name);
					if(RptStruct->desc!=NULL) Rfree(RptStruct->desc);
				}
			}
			Rfree(MakeStruct->RptStruct);
		}
		Rfree(MakeStruct);
	}
	return(MakeStruct);
}

int addRPTSTRUCT(MAKESTRUCT *MakeStruct,short type,char *name,char *desc,void (*func)(...))
{
	RPTSTRUCT *RptStruct=NULL;

	if(MakeStruct!=NULL)
	{
		if(MakeStruct->RptStruct!=NULL)
		{
			MakeStruct->RptStruct=Rrealloc(MakeStruct->RptStruct,(MakeStruct->num+1)*sizeof(RPTSTRUCT));
		} else {
			MakeStruct->RptStruct=Rmalloc(sizeof(RPTSTRUCT));
			MakeStruct->num=0;
		}
		RptStruct=MakeStruct->RptStruct+MakeStruct->num;
		RptStruct->type=type;
		RptStruct->name=stralloc(name);
		RptStruct->desc=stralloc(desc);
		RptStruct->func=func;
		return(++MakeStruct->num);
	} else { 
		return(-1);
	}
}
/* mkadm1.c */
extern void SENDMYVNC_SCRIPT(void);
/* mkadm2.c */
extern void LPS_SCRIPT(void);
/* mkadm3.c */
extern void KILLWHO_SCRIPT(void);
/* mkadm4.c */
extern void TEST_SCRIPT(void);
#if defined(LINUX2_2) || defined(UBUNTU_OS)
extern void EditVNCFile(void);
#endif /* ifdef LINUX2_2 || UBUNTU_OS */

static void checkheader(FILE *fp,int *lines,int *pages,char *modname)
{
	char *temp=NULL;

	if(doheaders)
	{
		if(*lines>57)
		{
			fprintf(fp,"\f");
			temp=Rmalloc(RDAstrlen(modname)+24);
			sprintf(temp,"XPERT MAKE %s ERROR LIST",
				(modname!=NULL?modname:""));
			printheader(fp,lines,pages,temp,modname);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(FILE *fp,int *lines,int *pages,char *header,char *modname)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;
	char *mainheader=NULL;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	fprintf(fp,"\n");
	*lines=1;
	
	if(!isEMPTY(XPERT_SETUP->company_name))
	{
		mainheader=stralloc(XPERT_SETUP->company_name);
	} else {
		mainheader=stralloc("No Organization Name");
	}



	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(mainheader)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(mainheader)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(mainheader)+2+16+lead_ws+tail_ws)>line_length) mainheader[(line_length-16-lead_ws-tail_ws)]=0;
	fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",mainheader,tail_ws,"",10,++*pages);
	++*lines;
	if(mainheader!=NULL) Rfree(mainheader);


	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	fprintf(fp,"%*s %s %*sDate: %s\n",
		lead_ws,"",header,tail_ws,"",date);
	++*lines;



	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		fprintf(fp,"%*s %s %*sTime:   %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
			++*lines;
	} else {
		fprintf(fp,"%*s Executed By: %s %*sTime:   %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}

	fprintf(fp,"\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}

static void printerrorlist(FILE *fp,char *modname,int *lines,int *pages)
{
	int x;
	char *temp=NULL;

	if(fp!=NULL)
	{
		temp=Rmalloc(RDAstrlen(modname)+24);
		sprintf(temp,"XPERT MAKE %s ERROR LIST",(modname!=NULL?modname:""));
		printheader(fp,lines,pages,temp,modname);
		if(temp!=NULL) Rfree(temp);

		for(x=0;x<errorlist->numlibs;++x)
		{
			fprintf(fp,"(%5d) %s\n",(x+1),errorlist->libs[x]);
			++*lines;
			checkheader(fp,lines,pages,modname);
		}
		fprintf(fp,"\f");
	}
}

void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL;
	FILE *fp=NULL;
	int lines=0,pages=0;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
			printerrorlist(fp,module,&lines,&pages);
			pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}

void print_list(RDArsrc *parent,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	if(diaggui)
	{
		prterr("DIAG printerrorlist Displaying the Print Screen in preparation to print Error List.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,parent);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print(prsrc);
	}
}

static APPlib *makescnlist(MAKESTRUCT *MakeStruct)
{
	int x;
	char *tmp=NULL;
	RPTSTRUCT *r=NULL;
	APPlib *a=NULL;

	a=APPlibNEW();
	for(x=0;x<MakeStruct->num;++x)
	{
		r=MakeStruct->RptStruct+x;
		tmp=Rmalloc(RDAstrlen(r->name)+RDAstrlen(r->desc)+6);
		sprintf(tmp,"[%s] [%s]",r->name,r->desc);
		addAPPlib(a,tmp);
		if(tmp!=NULL) Rfree(tmp);
	}
	return(a);
}

static void quitfinrpt(RDArsrc *mainrsrc,MAKESTRUCT *MakeStruct)
{
	if(scnlist!=NULL) freeapplib(scnlist);
	if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
	{
		APPlib2SupportRequest("UTILITIES","mkadm -> Administration Update Package",errorlist);
	}
	if(errorlist!=NULL) freeapplib(errorlist);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(MakeStruct!=NULL) MakeStruct=MakeStructFREE(MakeStruct);
}
static void selectrpt(RDArsrc *mainrsrc,MAKESTRUCT *MakeStruct)
{
	int selected=0;
	RPTSTRUCT *r=NULL;
	char *temp=NULL;
	RDArsrc *tmprsrc=NULL;

	FINDRSCGETINT(mainrsrc,"RESOURCE LIST",&selected);
	r=MakeStruct->RptStruct+selected;
	if(r->func!=NULL)
	{
		temp=Rmalloc(RDAstrlen(r->desc)+18);
		sprintf(temp,"Creating %s",(r->desc!=NULL?r->desc:""));
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,temp,NULL,(MakeStruct->num));
		if(temp!=NULL) Rfree(temp);
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				prterr("Error Cannot Create Diagnostic Screen.");
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
/*lint -e746 */
		r->func();
/*lint +e746 */
		FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
		updatersrc(mainrsrc,"ERROR LIST");
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,TRUE);
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
}
static void selectall(RDArsrc *mainrsrc,MAKESTRUCT *MakeStruct)
{
	int x;
	RPTSTRUCT *r=NULL;
	short do_it=FALSE;
	short did_it=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(mainrsrc!=NULL)
	{
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,"Creating Standards",NULL,MakeStruct->num);
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				prterr("Error Cannot Create Diagnostic Screen.");
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
	}
	for(x=0;x<MakeStruct->num;++x)
	{
		did_it=FALSE;
		r=MakeStruct->RptStruct+x;
		if(r->func!=NULL && 
		((mainrsrc==NULL && r->type==0) || 
		(mainrsrc==NULL && (Conversion==TRUE) && r->type==2) ||
		(mainrsrc!=NULL && r->type==0)))
		{
			do_it=TRUE;
			if(do_it==TRUE)
			{
				prterr("Executing: [%s][%s] from collection defined for %s.",(r->name!=NULL ? r->name:""),(r->desc!=NULL ? r->desc:""),module);
/*lint -e746 */
				r->func();
/*lint +e746 */
				if(mainrsrc!=NULL)
				{
					FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
					updatersrc(mainrsrc,"ERROR LIST");
				}
				did_it=TRUE;
			}
		}
		if(mainrsrc!=NULL)
		{
			if(tmprsrc!=NULL) 
			{
				update_diagnostic(tmprsrc,(did_it ? TRUE:FALSE));
			}
		}
	}
	if(mainrsrc!=NULL)
	{
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	WriteBinaryVersion("UTILITIES",CurrentBinaryVersion);
}
static void make_screens(int argc,char **argv,MAKESTRUCT *MakeStruct)
{
	int x,y;
	RPTSTRUCT *r=NULL;
	char *temp1=NULL,*temp2=NULL;
	short do_it=FALSE;

	if(argc==2 && !RDAstrcmp(argv[1],"ALL"))
	{
		selectall(NULL,MakeStruct);
	} else if(argc==3 && !RDAstrcmp(argv[2],"ALL"))
	{
		if(!RDAstrcmp(argv[1],"-cmb")) 
		{
			Conversion=TRUE;
		}
		selectall(NULL,MakeStruct);
	} else {
		for(x=1;x<argc;++x)
		{
			if(x==1 && !RDAstrcmp(argv[x],"-cmb"))
			{
				Conversion=TRUE;
			} else { 
				for(y=0;y<MakeStruct->num;++y)
				{
					r=MakeStruct->RptStruct+y;
					temp1=stralloc(argv[x]);
					temp2=stralloc(r->name);
					unpad(temp1);
					unpad(temp2);
					if(!RDAstrcmp(temp1,temp2))
					{
						do_it=TRUE;
						if(do_it==TRUE)
						{
/*lint -e746 */
							r->func();
/*lint +e746 */
						}
						break;
					
					}
					if(temp1!=NULL) Rfree(temp1);
					if(temp2!=NULL) Rfree(temp2);
				}
				if(y>=MakeStruct->num)
				{
					prterr("ERROR: [%s] not in collection of defined setup values for [%s].",argv[x],(module!=NULL?module:""));
				}
			}
		}
	}
}
static void MAKE_ADMIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW(module,"MAKE ADMINISTRATION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Binary Version Installed:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"BINARY VERSION INSTALLED","Version Installed","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,5,"","Installing Version:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"CURRENT BINARY VERSION","Current Binary Version","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Resource List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"RESOURCE LIST","Resource List","","",10,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Error List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"ERROR LIST","Error List","","",5,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT ALL","Select All","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PRINT ERROR LIST","Print Errors","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->module)+RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [%s] Screen [%s], Can Not Save Screen!",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void mk_diagscrn()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW(module,"DIAGNOSTIC SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"PROCESS NAME","Process Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"STATUS","Status","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Selected","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Total Read","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Total All","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED","Selected","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"TOTAL READ","Total Read","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"TOTAL ALL","Total All","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Start Time","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Current","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Elapsed","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"START TIME","Start Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"CURRENT TIME","Current Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"ELAPSED TIME","Elapsed Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Increment in:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"INCREMENT","Increment","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","seconds minimums","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SUSPEND","Suspend","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RESUME","Resume","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->module)+RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [%s] Screen [%s], Can Not Save Screen!",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static MAKESTRUCT *MAKE_RPTSTRUCT()
{
	MAKESTRUCT *RDAMakeStruct=NULL;

	RDAMakeStruct=MakeStructNEW();

#ifdef UNIXWARE7
	addRPTSTRUCT(RDAMakeStruct,0,"SYSINFO SCRIPT          ","sysinfo Script        ",SYSINFOSCRIPT);
#endif
#ifdef LINUX
	addRPTSTRUCT(RDAMakeStruct,0,"SENDMYVNC SCRIPT        ","sendmyvnc Script      ",SENDMYVNC_SCRIPT);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	addRPTSTRUCT(RDAMakeStruct,0,"SENDMYVNC SCRIPT        ","sendmyvnc Script      ",SENDMYVNC_SCRIPT);
	addRPTSTRUCT(RDAMakeStruct,0,"LPS SCRIPT              ","lps Script            ",LPS_SCRIPT);
	addRPTSTRUCT(RDAMakeStruct,0,"KILLWHO SCRIPT          ","killwho Script        ",KILLWHO_SCRIPT);
	addRPTSTRUCT(RDAMakeStruct,0,"EDIT VNC FILE SCRIPT    ","Edit VNC File Script  ",EditVNCFile);
#endif

	return(RDAMakeStruct);
}
int main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	MAKESTRUCT *MakeStruct=NULL;
	int x=0,ret_int=0;
	char *ret_string=NULL;

	RDA_SOFTWARE_TYPE=RDA_ALL_APP;
	if(InitializeSubsystems(argc,argv,module,"MAKE ADMINISTRATION")) 
	{
		return;
	}
	if(XPERT_SETUP==NULL)
	{
		prterr("ERROR: Make Financial Management can not load XPERT Setup, exiting abnormally");
		exit(1);
	}
	BinaryVersionInstalled=GetBinaryVersion("ADMIN VERSION");
	CurrentBinaryVersion=1;
	if(XPERT_SETUP->UTILITIES==TRUE)
	{
		ret_string=RDAmkdir("UTILITIES",MODE_RWXRWX___,&ret_int);
		if(ret_string!=NULL)
		{
			if(ret_int!=EEXIST)
			{
				prterr("ERROR: Make Financial Management can not make UTILITIES Directory/Folder and it doesn't already exist!.  System Reason: (%s), exiting abnormally",ret_string);
				Rfree(ret_string);
				exit(1);
			} else {
				prterr("ERROR: Make Financial Management directory [UTILITIES] already exist!.",ret_string);
				Rfree(ret_string);
			}
		}
		ret_string=RDAchmod("UTILITIES",MODE_RWXRWX___,&ret_int);
		if(ret_string!=NULL)
		{
			Rfree(ret_string);
		}
		mk_diagscrn();
		MAKE_ADMIN_SCREEN();
		MakeStruct=MAKE_RPTSTRUCT();

		if(argc>1)
		{
			if(!RDAstrcmp(argv[1],"-cmb"))
			{
				Conversion=TRUE;
				if(argc>2)
				{
					make_screens(argc,argv,MakeStruct);
					ShutdownSubsystems();
					std::exit;
				}
			} else {
				Conversion=FALSE;
				make_screens(argc,argv,MakeStruct);
				ShutdownSubsystems();
				std::exit;
			}
		} 
		mainrsrc=RDArsrcNEW(module,"MAKE UTILITIES");
		scnlist=makescnlist(MakeStruct);
		addstdrsrc(mainrsrc,"BINARY VERSION INSTALLED",LONGV,8,&BinaryVersionInstalled,FALSE);
		addstdrsrc(mainrsrc,"CURRENT BINARY VERSION",LONGV,8,&CurrentBinaryVersion,FALSE);
		addlstrsrc(mainrsrc,"RESOURCE LIST",&x,TRUE,NULL,scnlist->numlibs,&scnlist->libs,NULL);
		errorlist=APPlibNEW();
		addAPPlib(errorlist,"No Errors Detected");
		x=0;
		addlstrsrc(mainrsrc,"ERROR LIST",&x,TRUE,NULL,errorlist->numlibs,&errorlist->libs,NULL);
/*lint -e611 */
			addbtnrsrc(mainrsrc,"PRINT ERROR LIST",TRUE,print_list,(void *)printerrorlistcb);
/*lint +e611 */
		addbtnrsrc(mainrsrc,"SELECT ALL",TRUE,selectall,MakeStruct);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,selectrpt,MakeStruct);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfinrpt,MakeStruct);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitfinrpt,MakeStruct,TRUE);
	} else {
		prterr("ERROR: Client doesn't have [%s] module marked as purchased.",module);
		ShutdownSubsystems();
		exit(1);
	}
}

