#ifndef WIN32
#define __NAM__ "rdaout.lnx"
#endif
#ifdef WIN32
#define __NAM__ "rdaout.exe"
#endif
#include <app.hpp>
#include <cstdlib>
#if (!defined(UNIXWARE21) && !defined(UNIXWARE7))
#include <sys/stat.h>
#include <fcntl.h>
#else 
#include <sys/stat.h>
#endif


#include <rptgen.hpp>
#include <powadd.hpp>
#include <mix.hpp>
#include <gui.hpp>
/*ADDLIB add */
/*ADDLIB rpt */
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

static char standalone=FALSE,SKIP_WARNINGS=FALSE;
short archwork=(-1),archsub=(-1);
static char *module="UTILITIES",which=FALSE;
static char *message=NULL,*header=NULL;
static APPlib *modlst=NULL,*print_size=NULL,*namestyle=NULL;
static short a_fileno=(-1);
MakeBrowseList *mbl_wrkgp=NULL,*mbl_subject=NULL;

static void quit(RDArsrc *),quitcheck(RDArsrc *),save(RDArsrc *);
static void savetext(RDArsrc *,RDArsrc *);
static short read_file(RDArsrc *),read_pipe(RDArsrc *);
static void read_filetest(RDArsrc *);
static void read_filefront(RDArsrc *);
static void print(RDArsrc *),printtext(RDArsrc *,RDArsrc *); 
static void quitrsrc(RDArsrc *);
static void search_for_string(RDArsrc *);
static void archive(RDArsrc *mainrsrc);
typedef struct stat Statb;
void save(RDArsrc *);

static char EditText()
{
	short fileno=(-1);
	char editable=FALSE;

	fileno=OPNNRD("SECURITY","USERS");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(fileno,1))
		{
			FINDFLDGETCHAR(fileno,"EDIT TEXT BROWSER",&editable);
		}	
		CLSNRD(fileno);
	}
	return(editable);
}
static char ExecuteArchive()
{
	char e=FALSE;

	ZERNRD(MODSEC_FILENO);
	FINDFLDSETSTRING(MODSEC_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN);
	FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME","ARCHIVE");
	if(!EQLNRD(MODSEC_FILENO,1)) FINDFLDGETCHAR(MODSEC_FILENO,"EXECUTE FUNCTIONS",&e);
	return(e);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc, char **argv)
#endif
{
	short return_value=TRUE;
	char *filename=NULL,*text=NULL,e=FALSE;
	RDArsrc *mainrsrc=NULL;
	char *temp=NULL;

	if (InitializeSubsystems(argc, argv, module, "TEXT BROWSER"))
	{ 
		return;
	}
	if(argc>1)
	{
		if(RDAstrcmp(argv[1],"-s")) 
		{
			filename=argv[1];
		} else { 
			standalone=TRUE;
			if(!isEMPTY(argv[2])) filename=argv[2];
				else SKIP_WARNINGS=TRUE;
		}
	}
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
		a_fileno=OPNNRDsec("ARCHIVE","ARCHIVES",TRUE,TRUE,FALSE);
	} else a_fileno=(-1);
	if(standalone)
	{
		mainrsrc=RDArsrcNEW(module,"TEXT EDITOR");
		e=TRUE;
	} else {
		mainrsrc=RDArsrcNEW(module,"TEXT BROWSER");
		e=EditText();
	}
	addsctrsrc(mainrsrc,"TEXT",0,text,e);
	addstdrsrc(mainrsrc,"FILE NAME",VARIABLETEXT,0,filename,TRUE);
	addstdrsrc(mainrsrc,"SEARCH STRING",VARIABLETEXT,0,NULL,TRUE);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitcheck,NULL);
	addbtnrsrc(mainrsrc,"PRINT",TRUE,print,NULL);
	if(XPERT_SETUP->ARCHIVE==TRUE && a_fileno!=(-1) && ExecuteArchive())
	{
		addbtnrsrc(mainrsrc,"ARCHIVE",TRUE,archive,NULL);
		temp=RDA_GetEnv("AUTO_ARCHIVE");
		if(!isEMPTY(temp))
		{
			if(!RDAstrcmp(temp,"TRUE"))
			{
				FINDRSCSETSENSITIVE(mainrsrc,"ARCHIVE",FALSE);
			}
		}
	}
	if(standalone)
	{
		addbtnrsrc(mainrsrc,"SAVE",TRUE,save,NULL);
	}
	FINDRSCSETFUNC(mainrsrc,"FILE NAME",read_filetest,NULL);
	FINDRSCSETFUNC(mainrsrc,"SEARCH STRING",search_for_string,NULL);
	if(!APPmakescrn(mainrsrc,TRUE,quit,NULL,TRUE))
	{
		if(standalone)
		{
			FINDRSCSETINPUTFOCUS(mainrsrc,"TEXT");
			return_value=TRUE;
		}
		if(filename!=NULL)
		{ 
			return_value=read_file(mainrsrc);
		} else if(!standalone)
		{
			return_value=read_pipe(mainrsrc);
		} 
	}
	if(return_value==FALSE)
	{
		ERRORDIALOG(header,message,NULL,FALSE);
	}
}
static void savefile(RDArsrc *prtrsrc,RDArsrc *mainrsrc)
{
	save(mainrsrc);
}
void print(RDArsrc *mainrsrc)
{
	char *devicex=NULL;
	char *temp=NULL;
	RDArsrc *prtrsrc=NULL;
	short rep_copies=1;
	int x=0;

	devicex=DefaultPrinter();
	if(standalone)
	{
	prtrsrc=RDArsrcNEW(module,"TEXT EDITOR PRINT SCREEN");
	} else {
	prtrsrc=RDArsrcNEW(module,"TEXT PRINT SCREEN");
	}
	addbtnrsrc(prtrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prtrsrc,"DEVICE",VARIABLETEXT,0,devicex,TRUE);
	if(print_size!=NULL) freeapplib(print_size);
	print_size=APPlibNEW();
	addAPPlib(print_size,"NORMAL TEXT (DUPLEX)");
	addAPPlib(print_size,"COMPRESSED TEXT (DUPLEX)");
	addAPPlib(print_size,"LANDSCAPE NORMAL TEXT (DUPLEX)");
	addAPPlib(print_size,"LANDSCAPE COMPRESSED TEXT (DUPLEX)");
	addAPPlib(print_size,"NORMAL TEXT");
	addAPPlib(print_size,"COMPRESSED TEXT");
	addAPPlib(print_size,"LANDSCAPE NORMAL TEXT");
	addAPPlib(print_size,"LANDSCAPE COMPRESSED TEXT");
	temp=RDA_GetEnv("PRINT_TYPE");
	if(!isEMPTY(temp))
	{
		x=atoi(temp);
	} else x=0;
	addlstrsrc(prtrsrc,"PRINT STYLE",&x,TRUE,NULL,print_size->numlibs,
		&print_size->libs,NULL);
	addstdrsrc(prtrsrc,"REPORT COPIES",SHORTV,4,&rep_copies,TRUE);
	if(devicex!=NULL) Rfree(devicex);
	DefaultScreens(prtrsrc);
	FINDRSCSETINT(prtrsrc,"PRINT STYLE",x);
	addrfkwrsrc(prtrsrc,"PRINT",TRUE,printtext,mainrsrc);
	if(!standalone)
	{
		addbtnrsrc(prtrsrc,"SAVE",TRUE,savefile,mainrsrc);
	}
	addbtnrsrc(prtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfkwrsrc(prtrsrc,"QUIT",TRUE,quitrsrc,NULL);
	addbtnrsrc(prtrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	APPmakescrn(prtrsrc,TRUE,quitrsrc,NULL,FALSE);
}
void printtext(RDArsrc *prtrsrc,RDArsrc *mainrsrc)
{
	RDA_PFILE *fp1=NULL;
	char *outputdevice=NULL;
	int print_size=FALSE;
	char *prtmessage=NULL;
	RDArmem *member=NULL;
	int x=0,y=0;
	short rep_copies=1;

	if((x=FINDRSC(mainrsrc,"TEXT"))==(-1))
	{
		return;
	} else {
		member=mainrsrc->rscs+x;
	}
	readwidget(mainrsrc,"TEXT");
	readwidget(prtrsrc,"DEVICE");
	readwidget(prtrsrc,"REPORT COPIES");
	if(FINDRSCGETSTRING(prtrsrc,"DEVICE",&outputdevice)) return;
	FINDRSCGETINT(prtrsrc,"PRINT STYLE",&print_size);
	FINDRSCGETSHORT(prtrsrc,"REPORT COPIES",&rep_copies);
	if(outputdevice!=NULL&&(member->value.string_value!=NULL && RDAstrlen(member->value.string_value)>0))
	{
		for(y=0;y<rep_copies;++y)
		{
		if((fp1=RDA_popen(outputdevice,"w"))!=NULL)
		{
			SelectPrintTypeByName(outputdevice,"PCL JOB HEADER",fp1);
			switch(print_size)
			{
				case 0: /* Normal */
					SelectPrintTypeByName(outputdevice,"DUPLEX LONG",fp1);
					SelectPrintTypeByName(outputdevice,"PORTRAIT",fp1);
					SelectPrintType(outputdevice,0,fp1);
					break;
				case 1: /* Compressed */
					SelectPrintTypeByName(outputdevice,"DUPLEX LONG",fp1);
					SelectPrintTypeByName(outputdevice,"PORTRAIT",fp1);
					SelectPrintType(outputdevice,1,fp1);
					break;
				case 2: /* Landscape Normal */
					SelectPrintTypeByName(outputdevice,"DUPLEX SHORT",fp1);
					SelectPrintTypeByName(outputdevice,"LANDSCAPE",fp1);
					SelectPrintType(outputdevice,0,fp1);
					break;
				case 3: /* Landscape Compressed */
					SelectPrintTypeByName(outputdevice,"DUPLEX SHORT",fp1);
					SelectPrintTypeByName(outputdevice,"LANDSCAPE",fp1);
					SelectPrintType(outputdevice,1,fp1);
					break;
				case 4: /* Normal */
					SelectPrintTypeByName(outputdevice,"SIMPLEX",fp1);
					SelectPrintType(outputdevice,0,fp1);
					break;
				case 5: /* Compressed */
					SelectPrintTypeByName(outputdevice,"SIMPLEX",fp1);
					SelectPrintTypeByName(outputdevice,"PORTRAIT",fp1);
					SelectPrintType(outputdevice,1,fp1);
					break;
				case 6: /* Landscape Normal */
					SelectPrintTypeByName(outputdevice,"SIMPLEX",fp1);
					SelectPrintTypeByName(outputdevice,"LANDSCAPE",fp1);
					SelectPrintType(outputdevice,0,fp1);
					break;
				case 7: /* Landscape Compressed */
					SelectPrintTypeByName(outputdevice,"SIMPLEX",fp1);
					SelectPrintTypeByName(outputdevice,"LANDSCAPE",fp1);
					SelectPrintType(outputdevice,1,fp1);
					break;
			}
/* We need to review making this faster.... it really slow's down the printing
   to and thru the queue to the printer... look at readline and let's do
   something similar... dealing with hunks instead of characters */
#ifdef WIN32
			which=TRUE;
#endif
#ifdef USING_QT
			which=TRUE;
#endif
			if(which)
			{
				for(x=0;x<RDAstrlen(member->value.string_value);++x)
				{
					if(member->value.string_value[x]=='\n')
					{
						RDA_fprintf(fp1,"\r\n");
					} else {
						RDA_fprintf(fp1,"%c",member->value.string_value[x]);
					}
				}
			} else {
				RDA_fprintf(fp1,"%s\r\n",(member->value.string_value!=NULL ? member->value.string_value:""));
			}
			SelectPrintTypeByName(outputdevice,"PCL JOB FOOTER",fp1);
			RDA_pclose(fp1);
		} else {
			prtmessage=Rmalloc(RDAstrlen(outputdevice)+67);
			sprintf(prtmessage,"The output device [%s] does not exist or cannot otherwise be opened!",outputdevice);
			ERRORDIALOG("Output Device Failure",prtmessage,NULL,FALSE);
			if(prtmessage!=NULL) Rfree(prtmessage);
			quitrsrc(prtrsrc);
			return;
		}
		}
		if(outputdevice!=NULL) Rfree(outputdevice);
		if(prtmessage!=NULL) Rfree(prtmessage);
	} else {
		if(outputdevice==NULL)
			ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
	quitrsrc(prtrsrc);
}
static void archivetext(RDArsrc *prtrsrc,RDArsrc *mainrsrc)
{
	RDArmem *member=NULL;
	int x=0,p=0,pageW=0,pageH=0;
	char *aname=NULL,*desc=NULL,temp[512],*fname=NULL;
	char *temp1=NULL,*c=NULL,*t=NULL,*wg=NULL,*wgdir=NULL;
	short pitch=0,points=0,s=1;
	short mcolumns=0,mrows=0;
	double pwidth=0.0,pheight=0.0;
	RDATData *prev=NULL;
	FILE *fp1=NULL;
	RDArsrc *tmprsrc=NULL;
	int aclass=1;
	char *subject=NULL;
	int pdfpagetype=1;
	int value=0;
	int ret_int=0;


	readwidget(mainrsrc,"TEXT");
	readallwidgets(prtrsrc);
	if((x=FINDRSC(mainrsrc,"TEXT"))==(-1))
	{
		return;
	} else {
		member=mainrsrc->rscs+x;
	}
	FINDRSCGETSTRING(prtrsrc,"WORK GROUP",&wg);
	FINDRSCGETSTRING(prtrsrc,"SUBJECT",&subject);
	FINDRSCGETINT(prtrsrc,"PRINT SIZE",&p);
	FINDRSCGETSTRING(prtrsrc,"ARCHIVE NAME",&aname);
	if(isEMPTY(aname)) return;
	FINDRSCGETSTRING(prtrsrc,"DESCRIPTION",&desc);
	FINDRSCGETSHORT(prtrsrc,"POINTS",&points);
	FINDRSCGETSHORT(prtrsrc,"PITCH",&pitch);
	FINDRSCGETDOUBLE(prtrsrc,"PAGE WIDTH",&pwidth);
	FINDRSCGETDOUBLE(prtrsrc,"PAGE HEIGHT",&pheight);
	if(FINDRSCGETINT(prtrsrc,"PRINT SIZE",&value)) return;

	temp1=adddashes(aname);
	ZERNRD(archwork);
	FINDFLDSETSTRING(archwork,"WORK GROUP",wg);
	if(EQLNRD(archwork,1)) KEYNRD(archwork,1);
	FINDFLDGETSTRING(archwork,"DIRECTORY",&wgdir);
	stoupper(temp1);
	fname=Rmalloc(RDAstrlen(temp1)+5);
	sprintf(fname,"%s.pdf",temp1);	
	if(temp1!=NULL) Rfree(temp1);



	 /* -pt1  --  8x11 PORTRAIT */
	if((pheight<12.0)&&(pwidth<9.0)) pdfpagetype=1; 

	/* -pt2  --  8x11 LANDSCAPE */
	else if((pheight<9.0)&&(pwidth<12.0)) pdfpagetype=2; 

	/* -pt3  --  8x14 PORTRAIT */
	else if((pheight<15.0)&&(pheight>11.0)&&(pwidth<9.0)&&(pwidth>7.0)) pdfpagetype=3; 

	/* -pt4  --  8x14 LANDSCAPE */
	else if((pheight<9.0)&&(pheight>7.0)&&(pwidth<15.0)&&(pwidth>11.0)) pdfpagetype=4; 

	/* -pt5  --  11x14 PORTRAIT */
	else if((pheight<15.0)&&(pheight>11.0)&&(pwidth>8)&&(pwidth<12)) pdfpagetype=5; 

	/* -pt6  --  11x14 LANDSCAPE */
	else if((pheight<12.0)&&(pheight>8.0)&&(pwidth<15)&&(pwidth>11)) pdfpagetype=6; 

	/* -pt7  --  11x17 PORTRAIT */
	else if((pheight>14.0)&&(pwidth>10)&&(pheight>pwidth)) pdfpagetype=7; 

	/* -pt8  --  11x17 LANDSCAPE */
   	else if((pheight>10.0)&&(pwidth>14.0)) pdfpagetype=8; 
	else {
		prterr("Should Not Reach Here.  Review Page Size Ranges.   Page Length [%d]   Page Width [%d].\n",pheight,pwidth);
		pdfpagetype=1; 
	}
	


	if((p>1 && p<4) || (p>5))
	{
		if(pheight>=9.5 && pheight<=11.0) pheight=11.0;
		else if(pheight>11.0 && pheight<=14.0) pheight=14.0;
		if(pwidth>=7 && pwidth<=8.5) pwidth=8.5;
		else if(pwidth>8.5 && pwidth<=11.0) pwidth=11.0;
		else if(pwidth>11.0 && pwidth<=14.0) pwidth=14.0;
	} else {
		if(pwidth>=9.5 && pwidth<=11.0) pwidth=11.0;
		else if(pwidth>11.0 && pwidth<=14.0) pwidth=14.0;
		if(pheight>=7 && pheight<=8.5) pheight=8.5;
		else if(pheight>8.5 && pheight<=11.0) pheight=11.0;
		else if(pheight>11.0 && pheight<=14.0) pheight=14.0;
	}
	pageW=(pwidth)*72;
	pageH=(pheight)*72;
	mrows=pageH/points;
	mcolumns=pwidth*pitch;
	memset(stemp,0,101);
	temp1=Rmalloc(38);
	sprintf(temp1,"Archiving Report");
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,temp1,NULL,RDAstrlen(member->value.string_value));
	if(temp1!=NULL) Rfree(temp1);
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			ForceWindowUpdate(tmprsrc);
		}
	}

#ifndef WIN32
	sprintf(stemp,"ARCHIVE/.%s",(wg!=NULL ? wg:""));
#endif
#ifdef WIN32
	sprintf(stemp,"ARCHIVE\\.%s",(wg!=NULL ? wg:""));
#endif
	RDAmkdir(stemp,00770,&ret_int);

#ifdef WIN32
	memset(temp,0,512);

#ifndef WIN32
#endif
#ifdef WIN32
	switch(value)
	{
		default:
		case 0: /* Normal */
		case 4: /* Normal */
		case 2: /* Landscape Normal */
		case 6: /* Landscape Normal */
			sprintf(temp,"viewpdf.exe --dont-view --dont-delete -pt%d -ps1 -o .\\ARCHIVE\\.%s\\%s",pdfpagetype,wgdir,fname);
			break;
		case 1: /* Compressed */
		case 5: /* Compressed */
		case 3: /* Landscape Compressed */
		case 7: /* Landscape Compressed */
			sprintf(temp,"viewpdf.exe --dont-view --dont-delete -pt%d -ps2 -o .\\ARCHIVE\\.%s\\%s",pdfpagetype,wgdir,fname);
			break;
	}
#endif

	if((fp1=popen(temp,"w"))!=NULL)
	{
#ifdef FIRST_WAY
		for(x=0;x<RDAstrlen(member->value.string_value);++x)
		{
			if(member->value.string_value[x]=='\n')
			{
				fprintf(fp1,"\n");
			} else {
				fprintf(fp1,"%c",member->value.string_value[x]);
			}
			if(tmprsrc!=NULL) 
			{
				update_diagnostic(tmprsrc,TRUE);
			}
		}
#endif /* FIRST_WAY */
		fprintf(fp1,"%s",member->value.string_value);
		pclose(fp1);
	}
#endif 
#ifndef WIN32
	memset(temp,0,512);
	switch(value)
	{
		default:
		case 0: /* Normal */
		case 4: /* Normal */
		case 2: /* Landscape Normal */
		case 6: /* Landscape Normal */
			sprintf(temp,"viewpdf.lnx --dont-view --dont-delete -pt%d -ps1 -o ./ARCHIVE/.%s/%s",pdfpagetype,wgdir,fname);
			break;
		case 1: /* Compressed */
		case 5: /* Compressed */
		case 3: /* Landscape Compressed */
		case 7: /* Landscape Compressed */
			sprintf(temp,"viewpdf.lnx --dont-view --dont-delete -pt%d -ps2 -o ./ARCHIVE/.%s/%s",pdfpagetype,wgdir,fname);
			break;
	}
	if((fp1=popen(temp,"w"))!=NULL)
	{
#ifdef FIRST_WAY
		if(which)
		{
			for(x=0;x<RDAstrlen(member->value.string_value);++x)
			{
				if(member->value.string_value[x]=='\n')
				{
					fprintf(fp1,"\n");
				} else {
					fprintf(fp1,"%c",member->value.string_value[x]);
				}
				if(tmprsrc!=NULL) 
				{
					update_diagnostic(tmprsrc,TRUE);
				}
			}
		} else {
			fprintf(fp1,"%s\n",(member->value.string_value!=NULL ? member->value.string_value:""));
			if(tmprsrc!=NULL) 
			{
				update_diagnostic(tmprsrc,TRUE);
			}
		}
#endif /* FIRST_WAY */
		fprintf(fp1,"%s\n",(member->value.string_value!=NULL ? member->value.string_value:""));
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,TRUE);
		}
		pclose(fp1);
	}
#endif
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	ZERNRD(a_fileno);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",wg);
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	if(!EQLNRD(a_fileno,1))
	{
		prev=RDATDataNEW(a_fileno);
	} else KEYNRD(a_fileno,1);
	FINDFLDSETCHAR(a_fileno,"DELETEFLAG",FALSE);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP",wg);
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	FINDFLDSETSTRING(a_fileno,"DESCRIPTION",desc);
	FINDFLDSETSTRING(a_fileno,"SUBJECT",subject);
	FINDFLDSETSHORT(a_fileno,"SOURCE",s);
	FINDFLDSETSTRING(a_fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETSTRING(a_fileno,"FILENAME",fname);
	FINDFLDSETSTRING(a_fileno,"TYPE","pdf");
	FINDFLDSETINT(a_fileno,"CLASS",aclass);
	c=GETCURRENTDATE10();
	FINDFLDSETSTRING(a_fileno,"DATE",c);
	if(c!=NULL) Rfree(c);
	t=GETCURRENTTIME();
	FINDFLDSETSTRING(a_fileno,"TIME",t);
	if(t!=NULL) Rfree(t);
	WRTTRANS(a_fileno,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);	
	if(fname!=NULL) Rfree(fname);
	if(desc!=NULL) Rfree(desc);
	if(aname!=NULL) Rfree(aname);
	if(subject!=NULL) Rfree(subject);
	quitrsrc(prtrsrc);
}
static void changesize(RDArsrc *prtrsrc)
{
	int value=0;
	short pitch=10,points=12;

	if(FINDRSCGETINT(prtrsrc,"PRINT SIZE",&value)) return;
	switch(value)
	{
		default:
		case 0: /* Normal */
		case 4: /* Normal */
			pitch=10;
			points=12;
			break;
		case 1: /* Compressed */
		case 5: /* Compressed */
			pitch=17;
			points=9;
			break;
		case 2: /* Landscape Normal */
		case 6: /* Landscape Normal */
			pitch=10;
			points=12;
			break;
		case 3: /* Landscape Compressed */
		case 7: /* Landscape Compressed */
			pitch=17;
			points=9;
			break;
	}
	FINDRSCSETSHORT(prtrsrc,"POINTS",points);
	FINDRSCSETSHORT(prtrsrc,"PITCH",pitch);
	updatersrc(prtrsrc,"POINTS");
	updatersrc(prtrsrc,"PITCH");
}
static void workgrouptest(RDArsrc *prtrsrc)
{
	char *wg=NULL;

	readwidget(prtrsrc,"WORK GROUP");
	FINDRSCGETSTRING(prtrsrc,"WORK GROUP",&wg);
	if(diagapps)
	{
		prterr("Value of Work Group is %s",wg);
	}
	if((isEMPTY(wg)) || (wg==NULL))
	{
		FINDRSCSETEDITABLE(prtrsrc,"ARCHIVE",FALSE);
		FINDRSCSETSENSITIVE(prtrsrc,"ARCHIVE",FALSE);
	} else {
		FINDRSCSETEDITABLE(prtrsrc,"ARCHIVE",TRUE);
		FINDRSCSETSENSITIVE(prtrsrc,"ARCHIVE",TRUE);
	}
	updatersrc(prtrsrc,"ARCHIVE");
	if(wg!=NULL) Rfree(wg);
}
/* BROWSE WORK GROUP CODE STARTING */
static void doexit_browse_wrkgp(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_wrkgp(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_wrkgp(MakeBrowseList *BL,void *targetkey)
{
	char *grossid=NULL; 

	ZERNRD(archwork);
	if(ADVEQLNRDKEYsec(archwork,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(archwork,1);
	} else {
		if(grossid!=NULL) { Rfree(grossid); grossid=NULL; }
		FINDFLDGETSTRING(archwork,"WORK GROUP",&grossid);
	}
	readallwidgets(BL->funcargs);
	filerecord2rsrc(archwork,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"WORK GROUP",grossid);
	FINDFLDSETSTRING(archwork,"WORK GROUP",grossid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	workgrouptest(BL->funcargs);
	quit_browse_wrkgp(BL);
	if(grossid!=NULL) Rfree(grossid);
}
static short select_browse_wrkgp(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_wrkgp(MakeBrowseList *BL)
{
	ZERNRD(archwork);
}
static short break_browse_wrkgp(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_wrkgpids(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_wrkgp!=NULL)
	{
		mbl_wrkgp->funcargs=(void *)mtnrsrc;
		if(APPmakescrn(mbl_wrkgp->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_wrkgp(mbl_wrkgp);
		}
	} else {
		mainrsrc=RDArsrcNEW("ARCHIVE","SELECT WORK GROUP BROWSE");
		searchrsrc=RDArsrcNEW("ARCHIVE","SELECT WORK GROUP SEARCH BROWSE");
		definelist=RDArsrcNEW("ARCHIVE","SELECT WORK GROUP DEFINE LIST");
		ZERNRD(archwork);
		addDFincvir(searchrsrc,"ARCHIVE","ARCHWORK",NULL,archwork);
		addDFincvir(definelist,"ARCHIVE","ARCHWORK",NULL,archwork);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_wrkgp=xbrowse(archwork,1,mainrsrc,searchrsrc,definelist,init_browse_wrkgp,break_browse_wrkgp,select_browse_wrkgp,quit_browse_wrkgp,okfunc_browse_wrkgp,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE WORK GROUP CODE ENDING */
/* BROWSE SUBJECT STARTING */
static void doexit_browse_subject(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_subject(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_subject(MakeBrowseList *BL,void *targetkey)
{
	char *grossid=NULL; 

	ZERNRD(archsub);
	if(ADVEQLNRDKEYsec(archsub,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(archsub,1);
	} else {
		if(grossid!=NULL) { Rfree(grossid); grossid=NULL; }
		FINDFLDGETSTRING(archsub,"SUBJECT",&grossid);
	}
	readallwidgets(BL->funcargs);
	filerecord2rsrc(archsub,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"SUBJECT",grossid);
	FINDFLDSETSTRING(archsub,"SUBJECT",grossid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	quit_browse_wrkgp(BL);
	if(grossid!=NULL) Rfree(grossid);
}
static short select_browse_subject(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_subject(MakeBrowseList *BL)
{
	ZERNRD(archsub);
}
static short break_browse_subject(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_subject(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_subject!=NULL)
	{
		mbl_subject->funcargs=(void *)mtnrsrc;
		if(APPmakescrn(mbl_subject->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_subject(mbl_subject);
		}
	} else {
		mainrsrc=RDArsrcNEW("ARCHIVE","SELECT SUBJECT BROWSE");
		searchrsrc=RDArsrcNEW("ARCHIVE","SELECT SUBJECT SEARCH BROWSE");
		definelist=RDArsrcNEW("ARCHIVE","SELECT SUBJECT DEFINE LIST");
		ZERNRD(archwork);
		addDFincvir(searchrsrc,"ARCHIVE","ARCHSUB",NULL,archsub);
		addDFincvir(definelist,"ARCHIVE","ARCHSUB",NULL,archsub);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_subject=xbrowse(archsub,1,mainrsrc,searchrsrc,definelist,init_browse_subject,break_browse_subject,select_browse_subject,quit_browse_subject,okfunc_browse_subject,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE WORK GROUP CODE ENDING */
static void addworkgroup()
{
	if(mbl_wrkgp!=NULL)
	{
		doexit_browse_wrkgp(mbl_wrkgp);
		mbl_wrkgp=NULL;
	}
	RUNPOWERADDADV("ARCHIVE","ADD WORK GROUP",1,NULL,NULL,NULL,FALSE);
}
static void changenamestyle(RDArsrc *r)
{
	int ns=0,m=0;
	char *process=NULL,*time=NULL;

	if(FINDRSCGETINT(r,"NAME STYLE",&ns)) return;
	if(ns>0)
	{
		FINDRSCSETSENSITIVE(r,"MODULE LIST",TRUE);
		FINDRSCGETINT(r,"MODULE LIST",&m);
		memset(stemp,0,101);
		switch(ns)
		{
			case 1: /* YYYY-MM-DD.HH-MM.MODULE.PROCESS */
				time=GETCURRENTTIME();
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],modlst->libs[m],process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],modlst->libs[m]);
				}
				if(time!=NULL) Rfree(time);
				break;
			case 2: /* YYYY-MM-DD.HH-MM.MODULE.PROCESS.USER */
				time=GETCURRENTTIME();
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],modlst->libs[m],process,USERLOGIN);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],modlst->libs[m],USERLOGIN);
				}
				if(time!=NULL) Rfree(time);
				break;
			case 3: /* YYYY-MM-DD.HH-MM.MODULE.USER.PROCESS */
				time=GETCURRENTTIME();
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],modlst->libs[m],USERLOGIN,process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],modlst->libs[m],USERLOGIN);
				}
				if(time!=NULL) Rfree(time);
				break;
			case 4: /* YYYY-MM-DD.HH-MM.USER.MODULE.PROCESS */
				time=GETCURRENTTIME();
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN,modlst->libs[m],process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN,modlst->libs[m]);
				}
				if(time!=NULL) Rfree(time);
				break;
			case 5: /* YYYY-MM-DD.HH-MM.USER.PROCESS */
				time=GETCURRENTTIME();
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN,process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%.2s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN);
				}
				if(time!=NULL) Rfree(time);
				break;
			case 6: /* YYYY-MM-DD.MODULE.PROCESS */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],modlst->libs[m],process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],modlst->libs[m]);
				}
				break;
			case 8: /* YYYY-MM-DD.MODULE.USER.PROCESS. */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],modlst->libs[m],USERLOGIN,process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],modlst->libs[m],USERLOGIN);
				}
				break;
			case 9: /* YYYY-MM-DD.USER.MODULE.PROCESS. */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],USERLOGIN,modlst->libs[m],process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],USERLOGIN,modlst->libs[m]);
				}
				break;
			case 10: /* YYYY-MM-DD.USER.PROCESS. */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],USERLOGIN,process);
				} else {
					sprintf(stemp,"%4s-%.2s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],USERLOGIN);
				}
				break;
			case 11: /* YYYY-MM.MODULE.PROCESS. */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],modlst->libs[m],process);
				} else {
					sprintf(stemp,"%4s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],modlst->libs[m]);
				}
				break;
			case 13: /* YYYY-MM.MODULE.USER. */
				sprintf(stemp,"%4s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],modlst->libs[m],USERLOGIN);
				break;
			case 14: /* YYYY-MM.MODULE.USER.PROCESS. */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],modlst->libs[m],USERLOGIN,process);
				} else {
					sprintf(stemp,"%4s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],modlst->libs[m],USERLOGIN);
				}
				break;
			case 15: /* YYYY-MM.USER.MODULE.PROCESS. */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],USERLOGIN,modlst->libs[m],process);
				} else {
					sprintf(stemp,"%4s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],USERLOGIN,modlst->libs[m]);
				}
				break;
			case 16: /* YYYY-MM.USER.PROCESS. */
				process=RDA_GetEnv("CYBER_PROCESS");
				if(!isEMPTY(process))
				{
					sprintf(stemp,"%4s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],USERLOGIN,process);
				} else {
					sprintf(stemp,"%4s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],USERLOGIN);
				}
				break;
			case 17: /* YYYY-MM-DD. */
				sprintf(stemp,"%4s-%.2s-%.2s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3]);
				break;
			case 18: /* YYYY-MM-DD.MODULE. */
				sprintf(stemp,"%4s-%.2s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],modlst->libs[m]);
				break;
			case 19: /* YYYY-MM-DD.MODULE.USER. */
				sprintf(stemp,"%4s-%.2s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],modlst->libs[m],USERLOGIN);
				break;
			case 20: /* YYYY-MM-DD.USER. */
				sprintf(stemp,"%4s-%.2s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],USERLOGIN);
				break;
			case 21: /* YYYY-MM-DD.USER.MODULE. */
				sprintf(stemp,"%4s-%.2s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],USERLOGIN,modlst->libs[m]);
				break;
			case 22: /* YYYY-MM. */
				sprintf(stemp,"%4s-%.2s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0]);
				break;
			case 23: /* YYYY-MM.MODULE. */
				sprintf(stemp,"%4s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],modlst->libs[m]);
				break;
			case 24: /* YYYY-MM.USER. */
				sprintf(stemp,"%4s-%.2s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],USERLOGIN);
				break;
			case 25: /* YYYY-MM.USER.MODULE. */
				sprintf(stemp,"%4s-%.2s.%s.%s.",&CURRENT_DATE10[6],&CURRENT_DATE10[0],USERLOGIN,modlst->libs[m]);
				break;
		}
		FINDRSCSETSTRING(r,"ARCHIVE NAME",stemp);
		updatersrc(r,"ARCHIVE NAME");
	} else {
/*
		FINDRSCSETEDITABLE(r,"MODULE LIST",FALSE);
*/
		FINDRSCSETSENSITIVE(r,"MODULE LIST",FALSE);
	}
}
static void archive(RDArsrc *mainrsrc)
{
	RDArsrc *prtrsrc=NULL;
	short pitch=10,points=12;
	double pwidth=8.5,pheight=11;
	RDArmem *member=NULL;
	int x=0,defaultstyle=0;
	char *tmp=NULL,*dirx=NULL,*cb=NULL,*temp=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("ARCHIVE")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","RDAOUT DEFAULT NAME STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			defaultstyle=*gsv->value.short_value;
		} else {
			defaultstyle=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	readwidget(mainrsrc,"TEXT");
	if((x=FINDRSC(mainrsrc,"TEXT"))==(-1))
	{
		return;
	} else {
		member=mainrsrc->rscs+x;
	}
	if(archwork==(-1)) archwork=OPNNRD("ARCHIVE","ARCHWORK");
	if(archsub==(-1)) archsub=OPNNRD("ARCHIVE","ARCHSUB");
	prtrsrc=RDArsrcNEW(module,"ARCHIVE SCREEN");
	addDFincvir(prtrsrc,"ARCHIVE","ARCHWORK",NULL,archwork);
	addDFincvir(prtrsrc,"ARCHIVE","ARCHSUB",NULL,archsub);
	addstdrsrc(prtrsrc,"ARCHIVE NAME",PLAINTEXT,40,NULL,TRUE);
	addsctrsrc(prtrsrc,"DESCRIPTION",0,NULL,TRUE);
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+10);
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(dirx,"%s/rda",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	SORTAPPlib(modlst);
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	x=0;
	addbtnrsrc(prtrsrc,"ADD WORK GROUP",TRUE,addworkgroup,NULL);
	addbtnrsrc(prtrsrc,"BROWSE WORK GROUP",TRUE,browse_wrkgpids,NULL);
	addstdrsrc(prtrsrc,"WORK GROUP",PLAINTEXT,15,NULL,TRUE);
	FINDRSCSETFUNC(prtrsrc,"WORK GROUP",workgrouptest,NULL);
	addbtnrsrc(prtrsrc,"BROWSE SUBJECT",TRUE,browse_subject,NULL);
	x=0;
	if(namestyle!=NULL) freeapplib(namestyle);
	namestyle=APPlibNEW();
	addAPPlib(namestyle,"Undefined");
	addAPPlib(namestyle,"YYYY-MM-DD.HH-MM.MODULE.PROCESS");
	addAPPlib(namestyle,"YYYY-MM-DD.HH-MM.MODULE.PROCESS.USER");
	addAPPlib(namestyle,"YYYY-MM-DD.HH-MM.MODULE.USER.PROCESS"); 
	addAPPlib(namestyle,"YYYY-MM-DD.HH-MM.USER.MODULE.PROCESS");
	addAPPlib(namestyle,"YYYY-MM-DD.HH-MM.USER.PROCESS");
	addAPPlib(namestyle,"YYYY-MM-DD.MODULE.PROCESS"); 
	addAPPlib(namestyle,"YYYY-MM-DD.MODULE.PROCESS.USER");
	addAPPlib(namestyle,"YYYY-MM-DD.MODULE.USER.PROCESS");
	addAPPlib(namestyle,"YYYY-MM-DD.USER.MODULE.PROCESS");
	addAPPlib(namestyle,"YYYY-MM-DD.USER.PROCESS");
	addAPPlib(namestyle,"YYYY-MM.MODULE.PROCESS"); 
	addAPPlib(namestyle,"YYYY-MM.MODULE.PROCESS.USER"); 
	addAPPlib(namestyle,"YYYY-MM.MODULE.USER"); 
	addAPPlib(namestyle,"YYYY-MM.MODULE.USER.PROCESS"); 
	addAPPlib(namestyle,"YYYY-MM.USER.MODULE.PROCESS"); 
	addAPPlib(namestyle,"YYYY-MM.USER.PROCESS"); 
	addAPPlib(namestyle,"YYYY-MM-DD."); 
	addAPPlib(namestyle,"YYYY-MM-DD.MODULE."); 
	addAPPlib(namestyle,"YYYY-MM-DD.MODULE.USER.");
	addAPPlib(namestyle,"YYYY-MM-DD.USER."); 
	addAPPlib(namestyle,"YYYY-MM-DD.USER.MODULE.");
	addAPPlib(namestyle,"YYYY-MM.");
	addAPPlib(namestyle,"YYYY-MM.MODULE.");
	addAPPlib(namestyle,"YYYY-MM.USER.");
	addAPPlib(namestyle,"YYYY-MM.USER.MODULE.");

	addlstrsrc(prtrsrc,"NAME STYLE",&defaultstyle,TRUE,changenamestyle,namestyle->numlibs,&namestyle->libs,NULL);
	x=0;
	cb=RDA_GetEnv("CYBER_PRODUCT");
	if(!isEMPTY(cb))
	{
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(cb,modlst->libs[x])) break;			
		}
	}
	addlstrsrc(prtrsrc,"MODULE LIST",&x,FALSE,changenamestyle,modlst->numlibs,&modlst->libs,NULL);
	addstdrsrc(prtrsrc,"SUBJECT",PLAINTEXT,15,NULL,TRUE);
	DefaultScreens(prtrsrc);
	if(print_size!=NULL) freeapplib(print_size);
	print_size=APPlibNEW();
	addAPPlib(print_size,"NORMAL TEXT (DUPLEX)");
	addAPPlib(print_size,"COMPRESSED TEXT (DUPLEX)");
	addAPPlib(print_size,"LANDSCAPE NORMAL TEXT (DUPLEX)");
	addAPPlib(print_size,"LANDSCAPE COMPRESSED TEXT (DUPLEX)");
	addAPPlib(print_size,"NORMAL TEXT");
	addAPPlib(print_size,"COMPRESSED TEXT");
	addAPPlib(print_size,"LANDSCAPE NORMAL TEXT");
	addAPPlib(print_size,"LANDSCAPE COMPRESSED TEXT");
	temp=RDA_GetEnv("PRINT_TYPE");
	if(!isEMPTY(temp))
	{
		x=atoi(temp);
	} else x=0;
	temp=RDA_GetEnv("PAGE_LENGTH");
	if(!isEMPTY(temp)) pheight=atof(temp);
	if((pheight>=9.9) && (pheight<=11.0)) pheight=11;
	else if((pheight>11.0) && (pheight<=14)) pheight=14.0;
	temp=RDA_GetEnv("PAGE_WIDTH");
	if(!isEMPTY(temp)) pwidth=atof(temp);
	if((pwidth>=7.5) && (pwidth<=8.5)) pwidth=8.5;
	else if((pwidth>8.5) && (pwidth<11)) pwidth=11;
	else if((pwidth>11) && (pwidth<14)) pwidth=14;
	addlstrsrc(prtrsrc,"PRINT SIZE",&x,TRUE,changesize,print_size->numlibs,
		&print_size->libs,NULL);
	addstdrsrc(prtrsrc,"PITCH",SHORTV,2,&pitch,TRUE);
	addstdrsrc(prtrsrc,"POINTS",SHORTV,2,&points,TRUE);
	addstdrsrc(prtrsrc,"PAGE WIDTH",DECIMALV,5,&pwidth,TRUE);
	addstdrsrc(prtrsrc,"PAGE HEIGHT",DECIMALV,5,&pheight,TRUE);
	file2rsrc(archwork,prtrsrc,FALSE);
	file2rsrc(archsub,prtrsrc,FALSE);
	addrfkwrsrc(prtrsrc,"ARCHIVE",TRUE,archivetext,mainrsrc);
	addbtnrsrc(prtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfkwrsrc(prtrsrc,"QUIT",TRUE,quitrsrc,NULL);
	addbtnrsrc(prtrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	FINDRSCSETSHORT(prtrsrc,"PITCH",pitch);
	FINDRSCSETSHORT(prtrsrc,"POINTS",points);
	FINDRSCSETDOUBLE(prtrsrc,"PAGE WIDTH",pwidth);
	FINDRSCSETDOUBLE(prtrsrc,"PAGE HEIGHT",pheight);
	updatersrc(prtrsrc,"PITCH");
	updatersrc(prtrsrc,"POINTS");
	updatersrc(prtrsrc,"PAGE WIDTH");
	updatersrc(prtrsrc,"PAGE HEIGHT");
	changenamestyle(prtrsrc);
	changesize(prtrsrc);
	FINDRSCSETINPUTFOCUS(prtrsrc,"BROWSE WORK GROUP");
	APPmakescrn(prtrsrc,TRUE,quitrsrc,NULL,FALSE);
	workgrouptest(prtrsrc);
}
void quitrsrc(RDArsrc *rsrc)
{
	if(rsrc!=NULL) { free_rsrc(rsrc); rsrc=NULL; }
	if(print_size!=NULL) freeapplib(print_size);
} 
void save(RDArsrc *mainrsrc)
{
	char *file=NULL;
	RDArsrc *saversrc=NULL;

	readwidget(mainrsrc,"FILE NAME");
	FINDRSCGETSTRING(mainrsrc,"FILE NAME",&file);
	saversrc=RDArsrcNEW(module,"SAVE SCREEN");
	addstdrsrc(saversrc,"DESTINATION",VARIABLETEXT,60,file,TRUE);
	addstdrsrc(saversrc,"ADD CARRIAGE RETURNS",BOOLNS,1,&which,TRUE);
	addrfkwrsrc(saversrc,"SAVE",TRUE,savetext,mainrsrc);
	addbtnrsrc(saversrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(saversrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfkwrsrc(saversrc,"QUIT",TRUE,quitrsrc,NULL);
	APPmakescrn(saversrc,TRUE,quitrsrc,NULL,FALSE);
}
void savetext(RDArsrc *saversrc,RDArsrc *mainrsrc)
{
	FILE *fp=NULL;
	char *destination=NULL,*temp=NULL;
	char *savemessage=NULL;
	RDArmem *member=NULL;
	int x=0;

	readwidget(mainrsrc,"TEXT");
	readwidget(saversrc,"DESTINATION");
	readwidget(saversrc,"ADD CARRIAGE RETURNS");
	FINDRSCGETCHAR(saversrc,"ADD CARRIAGE RETURNS",&which);
	if((x=FINDRSC(mainrsrc,"TEXT"))==(-1))
	{
		return;
	} else {
		member=mainrsrc->rscs+x;
	}
	if(FINDRSCGETSTRING(saversrc,"DESTINATION",&destination)) return;
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(destination)+5);
	sprintf(temp,"%s/%s",CURRENTDIRECTORY,destination);
	if(destination!=NULL) Rfree(destination);
	destination=stralloc(temp);
	if(temp!=NULL) Rfree(temp);
	if((fp=fopen(destination,"w"))!=NULL)
	{
#ifdef WIN32
		which=TRUE;
#endif
		if(which)
		{
			for(x=0;x<RDAstrlen(member->value.string_value);++x)
			{
				if(member->value.string_value[x]=='\n')
				{
					fprintf(fp,"\r\n");
				} else {
					fprintf(fp,"%c",member->value.string_value[x]);
				}
			}
		} else {
			fprintf(fp,"%s",member->value.string_value);
		}
		fclose(fp);
	} else {	
		savemessage=Rmalloc(RDAstrlen(destination)+30);
		sprintf(savemessage,"The file [%s] cannot be opened.",destination);
		ERRORDIALOG("File Access Failure",savemessage,NULL,FALSE);
	}
	if(savemessage!=NULL) Rfree(savemessage);
	if(destination!=NULL) Rfree(destination);
	quitrsrc(saversrc);
}
static void search_for_string(RDArsrc *mainrsrc)
{
	RDArmem *member=NULL;
	short visable=FALSE;
	char *searchstr=NULL,*tmp=NULL;
	int x=0,curposition=0,startpos=0/*,endpos=0 */;

	readwidget(mainrsrc,"SEARCH STRING");
	FINDRSCGETSTRING(mainrsrc,"SEARCH STRING",&searchstr);
	if((x=FINDRSCGETVISABLE(mainrsrc,"TEXT",&visable))==(-1))
	{
		return;
	} else if(!visable)
	{
		return;
	} else {
		member=mainrsrc->rscs+x;
	}
	readwidget(mainrsrc,"TEXT");
	curposition=FINDRSCGETCURSORPOSITION(mainrsrc,"TEXT");
	if(curposition>=RDAstrlen(member->value.string_value))
	{
		curposition=0;
	}
	tmp=RDAstrstr(searchstr,member->value.string_value+(curposition));
	if(tmp!=NULL)
	{
		readwidget(mainrsrc,"TEXT");
		startpos=((int)((curposition+tmp)-RDAstrlen(member->value.string_value)));
/*
		endpos=startpos+RDAstrlen(searchstr);
*/
		FINDRSCSETCURSOR(mainrsrc,"TEXT",startpos);
/*
		FINDRSCHIGHLIGHT(mainrsrc,"TEXT",startpos,endpos);
*/
	}
	if(searchstr!=NULL) Rfree(searchstr);
}
static void read_filefront(RDArsrc *mainrsrc)
{
	short return_value=TRUE;

	return_value=read_file(mainrsrc);
	if(return_value==FALSE)
	{
		ERRORDIALOG(header,message,NULL,FALSE);
	}
}
static void read_filetest(RDArsrc *mainrsrc)
{
	char *text=NULL;
	RDArmem *member=NULL;
	int x=0;
	short visable=FALSE;

	if((x=FINDRSCGETVISABLE(mainrsrc,"TEXT",&visable))==(-1))
	{
		return;
	} else if(!visable)
	{
		return;
	} else {
		member=mainrsrc->rscs+x;
	}
	if(member->value.string_value!=NULL && RDAstrlen(member->value.string_value)>0)
	{
		text=stralloc(member->value.string_value);
		unpad(text);
	}
	readwidget(mainrsrc,"TEXT");
	unpad(member->value.string_value);
	if(RDAstrlen(member->value.string_value)!=RDAstrlen(text))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","CHANGES HAVE BEEN MADE!","Changes have been made to the file.  Are you sure you want to open a new file without saving these changes?",read_filefront,NULL,FALSE,mainrsrc);
	} else if(RDAstrcmp(text,member->value.string_value)!=0)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","CHANGES HAVE BEEN MADE!","Changes have been made to the file.  Are you sure you want to open a new file without saving these changes?",read_filefront,NULL,FALSE,mainrsrc);
	} else {
		read_filefront(mainrsrc);
	}
	if(text!=NULL) Rfree(text);
}
static short read_file(RDArsrc *mainrsrc)
{
	FILE *fp=NULL;
	char *file=NULL,*oldfile=NULL,*temp=NULL;
	RDArmem *member=NULL;
	Statb statb;
	int x=0;
	unsigned n=0;
	int file_length=0;
	short visable=FALSE;

	if(header!=NULL) Rfree(header);
	if(message!=NULL) Rfree(message);
	FINDRSCGETSTRING(mainrsrc,"FILE NAME",&oldfile);
	which=FALSE;
	if((x=FINDRSCGETVISABLE(mainrsrc,"TEXT",&visable))==(-1))
	{
		return(TRUE);
	} else if(!visable)
	{
		return(TRUE);
	} else {
		member=mainrsrc->rscs+x;
	}
	readwidget(mainrsrc,"TEXT");
	readwidget(mainrsrc,"FILE NAME");
	FINDRSCGETSTRING(mainrsrc,"FILE NAME",&file);
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(file)+5);
	sprintf(temp,"%s/%s",CURRENTDIRECTORY,file);
	if(file!=NULL) Rfree(file);
	file=stralloc(temp);
	if(temp!=NULL) Rfree(temp);
	if(file==NULL||(RDAstrlen(file)<1)) 
	{
		FINDRSCSETSTRING(mainrsrc,"FILE NAME",oldfile);
		updatersrc(mainrsrc,"FILE NAME");
		if(oldfile!=NULL) Rfree(oldfile);
		oldfile=Rmalloc(RDAstrlen(file)+16);
		sprintf(oldfile,"TEXT BROWSER (%s)",file);
		SETTITLE(mainrsrc,oldfile);
		if(oldfile!=NULL) Rfree(oldfile);
		if(file!=NULL) Rfree(file);
		return(TRUE);
#if !defined(LINUX_SUSE6_1)
	} else if((stat(file,&statb)==(-1))) 
#endif 
#if defined(LINUX_SUSE6_1)
	} else if((_xstat(_STAT_VER,file,&statb)==(-1))) 
#endif
	{
		header=stralloc("FILE NOT FOUND");
		message=Rmalloc(RDAstrlen(file)+41);
		sprintf(message,"The file [%s] was not found or is damaged!",file); 
		prterr("Error File [%s] not not found or damaged.",file);  
		if(!FINDRSCSETSTRING(mainrsrc,"TEXT",NULL))
			updatersrc(mainrsrc,"TEXT");
		if(!FINDRSCSETSTRING(mainrsrc,"FILE NAME",file))
			updatersrc(mainrsrc,"FILE NAME");
		if(oldfile!=NULL) Rfree(oldfile);
		if(file!=NULL) Rfree(file);
		SETTITLE(mainrsrc,"TEXT BROWSER");
		return(FALSE);
#ifndef WIN32
	} else 
#ifdef _POSIX_SOURCE
	if(!(S_ISREG(statb.st_mode)))
#else
	if((statb.st_mode&S_IFMT)!=S_IFREG)
#endif
	{
		header=stralloc("FILE TYPE ERROR");
		message=Rmalloc(RDAstrlen(file)+40);
		sprintf(message,"The file [%s] is not a correct file type!",file); 
		prterr("Error File [%s] is not a regular readable file!",file);  
		if(!FINDRSCSETSTRING(mainrsrc,"TEXT",NULL))
			updatersrc(mainrsrc,"TEXT");
		if(!FINDRSCSETSTRING(mainrsrc,"FILE NAME",oldfile))
			updatersrc(mainrsrc,"FILE NAME");
		if(oldfile!=NULL) Rfree(oldfile);
		if(file!=NULL) Rfree(file);
		SETTITLE(mainrsrc,"TEXT BROWSER");
		return(FALSE);
#endif /* WIN32 */
	} else if((fp=fopen(file,"r"))==NULL) 
       	{
		header=stralloc("FILEOPEN FAILED");
		message=Rmalloc(RDAstrlen(file)+38);
		sprintf(message,"The fopen function failed on file [%s]!",file);
		prterr("Error Can't fopen file passed as a parameter.");
		if(!FINDRSCSETSTRING(mainrsrc,"TEXT",NULL))
			updatersrc(mainrsrc,"TEXT");
		if(!FINDRSCSETSTRING(mainrsrc,"FILE NAME",oldfile))
			updatersrc(mainrsrc,"FILE NAME");
		if(oldfile!=NULL) Rfree(oldfile);
		if(file!=NULL) Rfree(file);
		SETTITLE(mainrsrc,"TEXT BROWSER");
		return(FALSE);
	} else if(statb.st_size<1)
	{
		header=stralloc("EMPTY FILE");
		message=Rmalloc(RDAstrlen(file)+44);
		sprintf(message,"The file [%s] was found but is an empty file!",file); 
		prterr("Error File [%s] was found but it is empty.",file);  
		if(!FINDRSCSETSTRING(mainrsrc,"TEXT",NULL))
			updatersrc(mainrsrc,"TEXT");
		if(!FINDRSCSETSTRING(mainrsrc,"FILE NAME",file))
			updatersrc(mainrsrc,"FILE NAME");
		if(oldfile!=NULL) Rfree(oldfile);
		if(file!=NULL) Rfree(file);
		fclose(fp);
		SETTITLE(mainrsrc,"TEXT BROWSER");
		return(FALSE);
	} else {
		file_length=(int)(statb.st_size);
		QUICKALLOC(member->value.string_value,member->dlen,file_length+1);
		memset(member->value.string_value,0,member->dlen);
		if(member->value.string_value==NULL)
		{
			header=stralloc("FILE TOO LARGE");
			message=Rmalloc(RDAstrlen(file)+124);
			sprintf(message,"The file [%s] is too large to read because currently free and available memory is less than the size need for allocation!",file);
			prterr("Error malloc() failed to allocate enough space for file data");
			if(!FINDRSCSETSTRING(mainrsrc,"TEXT",NULL))
				updatersrc(mainrsrc,"TEXT");
			if(!FINDRSCSETSTRING(mainrsrc,"FILE NAME",oldfile))
				updatersrc(mainrsrc,"FILE NAME");
			if(oldfile!=NULL) Rfree(oldfile);
			if(file!=NULL) Rfree(file);
			fclose(fp);
			SETTITLE(mainrsrc,"TEXT BROWSER");
			return(FALSE);
		}
		n=fread(member->value.string_value,sizeof(char),file_length,fp);
		if(!n)
		{
			header=stralloc("READ ERROR");
			message=Rmalloc(RDAstrlen(file)+44);
			sprintf(message,"The fread function failed for the file [%s]!",file);
			prterr("fread failed to read characters to end of screen");
			if(!FINDRSCSETSTRING(mainrsrc,"TEXT",NULL))
				updatersrc(mainrsrc,"TEXT");
			if(!FINDRSCSETSTRING(mainrsrc,"FILE NAME",oldfile))
				updatersrc(mainrsrc,"FILE NAME");
			if(oldfile!=NULL) Rfree(oldfile);
			if(file!=NULL) Rfree(file);
			fclose(fp);
			SETTITLE(mainrsrc,"TEXT BROWSER");
			return(FALSE);
		}
		if(!isEMPTY(member->value.string_value) && n<file_length)
		{
			member->value.string_value[n]=0;
		}
		updatersrc(mainrsrc,"TEXT");
		if(!FINDRSCSETSTRING(mainrsrc,"FILE NAME",file))
			updatersrc(mainrsrc,"FILE NAME");
		if(oldfile!=NULL) Rfree(oldfile);
		fclose(fp);
		oldfile=Rmalloc(RDAstrlen(file)+16);
		sprintf(oldfile,"TEXT BROWSER (%s)",file);
		SETTITLE(mainrsrc,oldfile);
		if(oldfile!=NULL) Rfree(oldfile);
		if(file!=NULL) Rfree(file);
		return(TRUE);
	}
}
static short read_pipe(RDArsrc *mainrsrc)
{
	char inbuf[PIPEBLOCKSIZE];
	RDArmem *member=NULL;
	int x=0,lf=0;
	short visable=FALSE;

	which=TRUE;
	if((x=FINDRSCGETVISABLE(mainrsrc,"TEXT",&visable))==(-1))
	{
		return(TRUE);
	} else if(!visable)
	{
		return(TRUE);
	} else {
		member=mainrsrc->rscs+x;
		if(member->value.string_value!=NULL) 
		{
			Rfree(member->value.string_value); 
			member->value.string_value=NULL;
			member->dlen=0;
		}
	}
	if((fgets(inbuf,PIPEBLOCKSIZE,stdin))==NULL)
	{
		updatemember(member);
		return(FALSE);
        } else {
		for(x=0;x<PIPEBLOCKSIZE;++x)
		{
			if(inbuf[x]=='\r')
			{
				inbuf[x]=' ';
			}
		}
		member->value.string_value=stralloc(inbuf);
		member->dlen=(unsigned)RDAstrlen(inbuf)+1;
		while(fgets(inbuf,PIPEBLOCKSIZE,stdin)!=NULL)
		{
			for(x=0;x<PIPEBLOCKSIZE;++x)
			{
				if(inbuf[x]=='\r')
				{
					inbuf[x]=' ';
				}
			}
			lf=(RDAstrlen(member->value.string_value)+RDAstrlen(inbuf)+2);
			QUICKALLOC(member->value.string_value,member->dlen,lf);
			strcat(member->value.string_value,inbuf);
		}
	}
	updatemember(member);
	return(TRUE);
}
static void quit(RDArsrc *mainrsrc)
{
	killwindow(mainrsrc);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(a_fileno!=(-1)) CLSNRD(a_fileno);
	if(archwork!=(-1)) CLSNRD(archwork);
	if(archsub!=(-1)) CLSNRD(archsub);
	ShutdownSubsystems();
}
static void quitcheck(RDArsrc *mainrsrc)
{
	char *text=NULL;
	RDArmem *member=NULL;
	int x=0;
	short visable=FALSE;

	if((x=FINDRSCGETVISABLE(mainrsrc,"TEXT",&visable))==(-1))
	{
		if(text!=NULL) Rfree(text);
		quit(mainrsrc);
		return;
	} else if(!visable)
	{
		if(text!=NULL) Rfree(text);
		quit(mainrsrc);
		return;
	} else {
		member=mainrsrc->rscs+x;
	}
	text=stralloc(member->value.string_value);
	unpad(text);
	readwidget(mainrsrc,"TEXT");
	unpad(member->value.string_value);
	if((RDAstrlen(member->value.string_value)!=RDAstrlen(text)) && SKIP_WARNINGS==FALSE)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES HAVE BEEN MADE!","Changes have been made to the file.  Are you sure you want to exit without saving these changes?",save,quit,FALSE,mainrsrc);
		if(text!=NULL) Rfree(text);
	} else if((RDAstrcmp(text,member->value.string_value)!=0) && SKIP_WARNINGS==FALSE)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES HAVE BEEN MADE!","Changes have been made to the file.  Are you sure you want to exit without saving these changes?",save,quit,FALSE,mainrsrc);
		if(text!=NULL) Rfree(text);
	} else {
		if(text!=NULL) Rfree(text);
		quit(mainrsrc);
	}
}
