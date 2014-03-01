/*LASER - Program to Test Laser Forms*/
#ifndef WIN32
#define __NAM__ "laser.lnx"
#endif
#ifdef WIN32
#define __NAM__ "laser.exe"
#endif
#include <app.hpp>
#include <mkmsc.hpp>
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTILITIES";

static void SCN_LASER_FORM_TEST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","LASER FORM TEST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"FALSE",NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Output Device:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD DEVICE NAME","Load Device Name","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEVICE","Device","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"USE FORMFEED","Formfeed @ Page End","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

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
			temp1=Rmalloc(15+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [LASER FORM TEST], Can Not Save Maintain Master!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void quitx(RDArsrc *r)
{
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
	std::exit;
}
static void selectx(RDArsrc *r)
{
	char *devicex=NULL,*prtmessage=NULL,use_formfeed=FALSE;
	RDA_PFILE *fp1=NULL;
	int page=1,x=0;

	readwidget(r,"USE FORMFEED");
	readwidget(r,"DEVICE");
	FINDRSCGETSTRING(r,"DEVICE",&devicex);
	FINDRSCGETCHAR(r,"USE FORMFEED",&use_formfeed);
	if(!isEMPTY(devicex))
	{
		if((fp1=RDA_popen(devicex,"w"))!=NULL)
		{
			SelectPrintTypeByName(devicex,"PCL JOB HEADER",fp1);
			SelectPrintType(devicex,FALSE,fp1);
			for(page=1;page<3;++page)
			{
				for(x=0;x<60;++x)
				{
					if(x==1)
					{
						RDA_fprintf(fp1,"BEGINNING LINE %2d PAGE %d XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX LINE %2d END\r",(x+1),page,(x+1));
						SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
						SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
						RDA_fprintf(fp1,"\r");
					} else {
						RDA_fprintf(fp1,"BEGINNING LINE %2d PAGE %d XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX LINE %2d END\r\n",(x+1),page,(x+1));
					}
				}
				if(use_formfeed)
				{
					RDA_fprintf(fp1,"\f");
				}
			}
			SelectPrintTypeByName(devicex,"PCL JOB FOOTER",fp1);
			RDA_pclose(fp1);
			return;
		}
	}
	prtmessage=Rmalloc(RDAstrlen(devicex)+67);
	sprintf(prtmessage,"The output device [%s] does not exist or cannot otherwise be opened!",devicex);
	ERRORDIALOG("Output Device Failure",prtmessage,NULL,FALSE);
	if(prtmessage!=NULL) Rfree(prtmessage);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *devicex=NULL,use_formfeed=FALSE;

	if(InitializeSubsystems(argc,argv,"UTILITIES","LASER FORM TEST")) 
	{
		return(1);
	}
	SCN_LASER_FORM_TEST();
	mainrsrc=RDArsrcNEW("UTILITIES","LASER FORM TEST");
	addbtnrsrc(mainrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(mainrsrc,"USE FORMFEED",BOOLNS,1,&use_formfeed,TRUE);
	devicex=DefaultPrinter();
	addstdrsrc(mainrsrc,"DEVICE",VARIABLETEXT,0,devicex,TRUE);
	if(devicex!=NULL) Rfree(devicex);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectx,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitx,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitx,NULL,TRUE);
	return(0);
}
