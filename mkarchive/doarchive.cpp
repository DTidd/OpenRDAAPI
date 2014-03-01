/* doarchive.c - Execute Archive in Viewer */
/*lint -library */
#ifndef WIN32
#define __NAM__ "doarchive.lnx"
#endif
#ifdef WIN32
#define __NAM__ "doarchive.exe"
#endif
#include <app.hpp>


#include <rptgen.hpp>
#include <powadd.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static APPlib *aprogs=NULL;
static short archprog=(-1);

static void doexit(RDArsrc *r)
{
	if(archprog!=(-1)) CLSNRD(archprog);
	if(aprogs!=NULL) freeapplib(aprogs);
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	ShutdownSubsystems();
	exit(0);
}
static void okfunction(RDArsrc *r)
{
	char *exec_name=NULL,*viewfile=NULL;
	char *progid=NULL,*temp=NULL;
	int s=0;
	APPlib *eargs=NULL;

	readwidget(r,"FILE TO VIEW");
	FINDRSCGETSTRING(r,"FILE TO VIEW",&viewfile);
	FINDRSCGETINT(r,"PROGRAM LIST",&s);
	eargs=APPlibNEW();
	progid=stralloc(aprogs->libs[s]);
	if(RDAstrlen(progid)>15) progid[15]=0;
	unpad(progid);
	ZERNRD(archprog);
	FINDFLDSETSTRING(archprog,"PROGRAM ID",progid);
	EQLNRD(archprog,1);
	FINDFLDGETSTRING(archprog,"EXECUTABLE NAME",&exec_name);
	if(isEMPTY(exec_name))
	{
		prterr("Error:  No Executable Name recorded for this program.");
	} else {
		if((!strncmp(progid,"dohttp",6)))
		{
			temp=Rmalloc(RDAstrlen(viewfile)+RDAstrlen(CURRENTDIRECTORY)+25);
#ifndef WIN32
			sprintf(temp,"file:/%s/%s",(viewfile!=NULL ? viewfile:""));
#endif
#ifdef WIN32
			sprintf(temp,"file://%s/%s",(viewfile!=NULL ? viewfile:""));
#endif
			addAPPlib(eargs,temp);
			if(temp!=NULL) Rfree(temp);
		} else addAPPlib(eargs,viewfile);
		ADVExecuteProgramB(exec_name,eargs,NULL,TRUE);
	}
	if(exec_name!=NULL) Rfree(exec_name);
	if(viewfile!=NULL) Rfree(viewfile);
	if(eargs!=NULL) freeapplib(eargs);
	if(progid!=NULL) Rfree(progid);
	doexit(r);
}
static void OpenWith(char *viewfile)
{
	RDArsrc *mainrsrc=NULL;
	short ef=0;
	char delflag=FALSE,*progid=NULL;
	char *temp=NULL,*name=NULL;
	int s=0;

	if(archprog==(-1)) archprog=OPNNRD("ARCHIVE","ARCHPROG");
	if(aprogs!=NULL) freeapplib(aprogs);
	aprogs=APPlibNEW();
	if(archprog!=(-1))
	{
		ef=BEGNRD(archprog);
		while(!ef)
		{
			FINDFLDGETCHAR(archprog,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(archprog,"PROGRAM ID",&progid);
				FINDFLDGETSTRING(archprog,"PROGRAM NAME",&name);
				temp=Rmalloc(RDAstrlen(progid)+RDAstrlen(name)+50);
				sprintf(temp,"%-15s : %s",(progid!=NULL ? progid:""),(name!=NULL ? name:""));
				addAPPlib(aprogs,temp);
				if(temp!=NULL) Rfree(temp);
			}
			ef=SEQNRD(archprog);
		}
	}
	mainrsrc=RDArsrcNEW("ARCHIVE","OPEN WITH");	
	addstdrsrc(mainrsrc,"FILE TO VIEW",VARIABLETEXT,0,viewfile,FALSE);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	if(aprogs->numlibs<1)
	{
		addAPPlib(aprogs,"No Programs Available");
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
	} else SORTAPPlib(aprogs);
	addlstrsrc(mainrsrc,"PROGRAM LIST",&s,TRUE,NULL,aprogs->numlibs,&aprogs->libs,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	mainrsrc->scn=RDAscrnNEW("ARCHIVE","OPEN WITH");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","File to View:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"FILE TO VIEW","File to View:","","",0,60,0,"","FALSE","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,5,"","   Open With:","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,7,"PROGRAM LIST","Program List","","",8,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"SELECT","","","",0,0,25,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"QUIT","","","",0,0,14,"","","","");
	ADVaddwdgt(mainrsrc->scn,0,"HELP","","","",0,0,11,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
	ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
	RDAAPPMAINLOOP();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char delflag=FALSE,*prog_name=NULL,*prog_id=NULL; 
	char *temp=NULL,*temp1=NULL,*prog_ext=NULL;
	char *prog_type=NULL;
	short fileno=(-1),x=0;
	APPlib *args=NULL;

	if(InitializeSubsystems(argc,argv,"ARCHIVE","DOARCHIVE"))
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc<2)
	{
		prterr("Error:  You must pass a filename to DOARCHIVE.");
		ShutdownSubsystems();
		exit(0);
	}
	if(argc<3)
	{
		temp1=stralloc(argv[1]);
		for(x=RDAstrlen(temp1)-1;x>0;--x)
		{
			temp=temp1+x;
			if(*temp=='.') break;
		}
		if((x>0) && (RDAstrlen(temp1)>x))
		{
			prog_ext=Rmalloc(RDAstrlen(temp1)-x+1);
			sprintf(prog_ext,"%s",&temp1[x+1]);
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(argc>2) prog_type=argv[2];
		else prog_type=prog_ext;
	fileno=OPNNRD("ARCHIVE","ARCHTYPE");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE",prog_type);
		if(EQLNRD(fileno,1)) 
		{
			KEYNRD(fileno,1);
		} else {
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(fileno,"PROGRAM ID",&prog_id);
			}
		}
		if(isEMPTY(prog_id))
		{
			prog_type=prog_ext;
			ZERNRD(fileno);
			FINDFLDSETSTRING(fileno,"TYPE",prog_type);
			if(EQLNRD(fileno,1)) 
			{
				KEYNRD(fileno,1);
			} else {
				FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
				if(!delflag)
				{
					FINDFLDGETSTRING(fileno,"PROGRAM ID",&prog_id);
				}
			}
		}
		CLSNRD(fileno);
	}
	fileno=OPNNRD("ARCHIVE","ARCHPROG");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"PROGRAM ID",prog_id);
		if(EQLNRD(fileno,1))
		{
			KEYNRD(fileno,1);
		} else {
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(fileno,"EXECUTABLE NAME",&prog_name);
			}
		}
	}
	if(isEMPTY(prog_name))
	{
		OpenWith(argv[1]);
	} else {
		args=APPlibNEW();
		if(RDAstrstr(prog_name,"gnuplot"))
		{
			addAPPlib(args,"-persist");
		}
		for(x=1;x<argc;++x)
		{
			if((!strncmp(prog_name,"dohttp",6)) && (x==1))
			{
				temp=Rmalloc(RDAstrlen(argv[x])+RDAstrlen(CURRENTDIRECTORY)+25);
#ifndef WIN32
				sprintf(temp,"file:/%s/%s",CURRENTDIRECTORY,argv[x]);
#endif
#ifdef WIN32
				sprintf(temp,"file://%s/%s",CURRENTDIRECTORY,argv[x]);
#endif
				addAPPlib(args,temp);
				if(temp!=NULL) Rfree(temp);
			} else if(x!=2) addAPPlib(args,argv[x]);
		}
		ADVExecuteProgramB(prog_name,args,NULL,TRUE);
		if(args!=NULL) freeapplib(args);
		if(prog_name!=NULL) Rfree(prog_name);
		if(prog_ext!=NULL) Rfree(prog_ext);
		ShutdownSubsystems();
		exit(0);
	}
}
