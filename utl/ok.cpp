#ifndef WIN32
#define __NAM__ "ok.lnx"
#endif
#ifdef WIN32
#define __NAM__ "ok.exe"
#endif
#include <app.hpp>
#include <cstdlib>
#if (!defined(UNIXWARE21) && !defined(UNIXWARE7))
#include <sys/stat.h>
#include <fcntl.h>
#else 
#include <sys/stat.h>
#endif


#include <gui.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *module="RPTGEN";

static void quit(RDArsrc *mainrsrc)
{
	killwindow(mainrsrc);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	ShutdownSubsystems();
	std::exit;
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc, char **argv)
#endif
{
	char *filename=NULL,*text=NULL;
	RDArsrc *mainrsrc=NULL;

	if (InitializeSubsystems(argc, argv, module, "OK ACKNOWLEDGEMENT"))
	{ 
		return;
	}

	if(!RDAstrcmp(argv[1],"ERRORDIALOG"))
	{
		ERRORDIALOG(argv[2],argv[3],NULL,TRUE);
		prterr("Function [%s]  Message [%s] [%s]",argv[1],argv[2],argv[3]);
	}
	if(argc>1)
	{
		filename=argv[1];
	}
	if(argc>2)
	{
		text=argv[2];
	}
	mainrsrc=RDArsrcNEW(module,"OK ACKNOWLEDGEMENT");
	mainrsrc->scn=RDAscrnNEW("RPTGEN","OK ACKNOWLEDGEMENT");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,5,"","Process:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,5,"",filename,"","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,5,"",text,"","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,6,"QUIT","OK","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	addsctrsrc(mainrsrc,"TEXT",0,text,TRUE);
	addstdrsrc(mainrsrc,"PROCESS NAME",VARIABLETEXT,0,filename,TRUE);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quit,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	APPmakescrn(mainrsrc,TRUE,quit,NULL,TRUE);
}
