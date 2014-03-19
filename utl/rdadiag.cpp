/* rdadiag.c - rdadiag.c sets up Xpert's Diagnostics */
#ifndef WIN32
#define __NAM__ "rdadiag.lnx"
#endif
#ifdef WIN32
#define __NAM__ "rdadiag.exe"
#endif
#include <app.hpp>
#include <sys/stat.h>
#ifdef WIN32
#include<cstdlib>
#include<process.h>
#endif /* WIN32 */

#include<mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIb trans */
/*ADDLIB sec */
/*ADDLIB gui */
/*ADDLIB trans */
/*ADDLIB nrd */
/*ADDLIB misc */

static void quitREADME(RDArsrc *r)
{
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
}
static void XpertEvaluateReadMe(RDArsrc *r)
{
	int count=0;
	FILE *fp=NULL;
	char *temp=NULL,xtemp[513];
	RDArsrc *tmprsrc=NULL;

#ifndef WIN32
	fp=fopen("./rda/Xpert.Evaluate.ReadMe","rb");
#endif
#ifdef WIN32
	fp=fopen(".\\rda\\Xpert.Evaluate.ReadMe","rb");
#endif
	if(fp!=NULL)
	{
		while(fgets(xtemp,512,fp))
		{
			if(temp==NULL)
			{
				temp=Rmalloc(RDAstrlen(xtemp)+1);
				sprintf(&temp[count],"%s",(xtemp!=NULL ? xtemp:""));
				count=RDAstrlen(xtemp);
			} else {
				temp=Rrealloc(temp,count+RDAstrlen(xtemp)+1);
				sprintf(&temp[count],"%s",(xtemp!=NULL ? xtemp:""));
				count+=RDAstrlen(xtemp);
			}
		}
		fclose(fp);
	}
	tmprsrc=RDArsrcNEW("UTILITIES","READ ME");
	addsctrsrc(tmprsrc,"README",0,temp,FALSE);
	addbtnrsrc(tmprsrc,"QUIT",TRUE,quitREADME,NULL);
	tmprsrc->scn=RDAscrnNEW("UTILITIES","READ ME");
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,8,"README","Read Me",NULL,NULL,25,70,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,6,"QUIT","Cancel",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	APPmakescrn(tmprsrc,TRUE,quitREADME,NULL,TRUE);
	if(temp!=NULL) Rfree(temp);
}
static void XpertGuiReadMe(RDArsrc *r)
{
	int count=0;
	FILE *fp=NULL;
	char *temp=NULL,xtemp[513];
	RDArsrc *tmprsrc=NULL;

#ifndef WIN32
	fp=fopen("./rda/Xpert.Gui.ReadMe","rb");
#endif
#ifdef WIN32
	fp=fopen(".\\rda\\Xpert.Gui.ReadMe","rb");
#endif
	if(fp!=NULL)
	{
		while(fgets(xtemp,512,fp))
		{
			if(temp==NULL)
			{
				temp=Rmalloc(RDAstrlen(xtemp)+1);
				sprintf(&temp[count],"%s",(xtemp!=NULL ? xtemp:""));
				count=RDAstrlen(xtemp);
			} else {
				temp=Rrealloc(temp,count+RDAstrlen(xtemp)+1);
				sprintf(&temp[count],"%s",(xtemp!=NULL ? xtemp:""));
				count+=RDAstrlen(xtemp);
			}
		}
		fclose(fp);
	}
	tmprsrc=RDArsrcNEW("UTILITIES","READ ME");
	addsctrsrc(tmprsrc,"README",0,temp,FALSE);
	addbtnrsrc(tmprsrc,"QUIT",TRUE,quitREADME,NULL);
	tmprsrc->scn=RDAscrnNEW("UTILITIES","READ ME");
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,8,"README","Read Me",NULL,NULL,25,70,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,6,"QUIT","Cancel",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	APPmakescrn(tmprsrc,TRUE,quitREADME,NULL,TRUE);
	if(temp!=NULL) Rfree(temp);
}
static void XpertReadMe(RDArsrc *r)
{
	int count=0;
	FILE *fp=NULL;
	char *temp=NULL,xtemp[513];
	RDArsrc *tmprsrc=NULL;

#ifndef WIN32
	fp=fopen("./rda/Xpert.Read.Me","rb");
#endif
#ifdef WIN32
	fp=fopen(".\\rda\\Xpert.Read.Me","rb");
#endif
	if(fp!=NULL)
	{
		memset(xtemp,0,513);
		while(fgets(xtemp,512,fp))
		{
			if(temp==NULL)
			{
				temp=Rmalloc(RDAstrlen(xtemp)+1);
				sprintf(&temp[count],"%s",(xtemp!=NULL ? xtemp:""));
				count=RDAstrlen(xtemp);
			} else {
				temp=Rrealloc(temp,count+RDAstrlen(xtemp)+1);
				sprintf(&temp[count],"%s",(xtemp!=NULL ? xtemp:""));
				count+=RDAstrlen(xtemp);
			}
			memset(xtemp,0,513);
		}
		fclose(fp);
	}
	tmprsrc=RDArsrcNEW("UTILITIES","READ ME");
	addsctrsrc(tmprsrc,"README",0,temp,FALSE);
	addbtnrsrc(tmprsrc,"QUIT",TRUE,quitREADME,NULL);
	tmprsrc->scn=RDAscrnNEW("UTILITIES","READ ME");
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,8,"README","Read Me",NULL,NULL,25,70,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,6,"QUIT","Cancel",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(tmprsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	APPmakescrn(tmprsrc,TRUE,quitREADME,NULL,TRUE);
	if(temp!=NULL) Rfree(temp);
}
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
	ShutdownSubsystems();
}
void LaunchStrCat(char **s,char *x)
{
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(x)+2);
	sprintf(temp,"%s ",x);
	*s=stralloccat(*s,temp);
}
static void LaunchDiagnostics(RDArsrc *r)
{
	char *temp=NULL,b=FALSE,*es=NULL;
	char *fprog=NULL;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"ALLOC",&b);
	if(b==TRUE) LaunchStrCat(&temp,"ALLOC");
	FINDRSCGETCHAR(r,"ALLOCX",&b);
	if(b==TRUE) LaunchStrCat(&temp,"ALLOCX");
	FINDRSCGETCHAR(r,"BADALLOC",&b);
	if(b==TRUE) LaunchStrCat(&temp,"BADALLOC");
	FINDRSCGETCHAR(r,"GUI",&b);
	if(b==TRUE) LaunchStrCat(&temp,"GUI");
	FINDRSCGETCHAR(r,"NRD",&b);
	if(b==TRUE) LaunchStrCat(&temp,"NRD");
	FINDRSCGETCHAR(r,"MIX",&b);
	if(b==TRUE) LaunchStrCat(&temp,"MIX");
	FINDRSCGETCHAR(r,"BIN",&b);
	if(b==TRUE) LaunchStrCat(&temp,"BIN");
	FINDRSCGETCHAR(r,"BINLIB",&b);
	if(b==TRUE) LaunchStrCat(&temp,"BINLIB");
	FINDRSCGETCHAR(r,"PROC",&b);
	if(b==TRUE) LaunchStrCat(&temp,"PROC");
	FINDRSCGETCHAR(r,"SEC",&b);
	if(b==TRUE) LaunchStrCat(&temp,"SEC");
	FINDRSCGETCHAR(r,"OLH",&b);
	if(b==TRUE) LaunchStrCat(&temp,"OLH");
	FINDRSCGETCHAR(r,"TRANS",&b);
	if(b==TRUE) LaunchStrCat(&temp,"TRANS");
	FINDRSCGETCHAR(r,"RPTGEN",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN");
	FINDRSCGETCHAR(r,"EVAL",&b);
	if(b==TRUE) LaunchStrCat(&temp,"EVAL");
	FINDRSCGETCHAR(r,"VF",&b);
	if(b==TRUE) LaunchStrCat(&temp,"VF");
	FINDRSCGETCHAR(r,"RF",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RF");
	FINDRSCGETCHAR(r,"RDADTA",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RDADTA");
	FINDRSCGETCHAR(r,"MISC",&b);
	if(b==TRUE) LaunchStrCat(&temp,"MISC");
	FINDRSCGETCHAR(r,"IMPORT",&b);
	if(b==TRUE) LaunchStrCat(&temp,"IMPORT");
	FINDRSCGETCHAR(r,"VIRTUAL",&b);
	if(b==TRUE) LaunchStrCat(&temp,"VIRTUAL");
	FINDRSCGETCHAR(r,"EXEPROG",&b);
	if(b==TRUE) LaunchStrCat(&temp,"EXEPROG");
	FINDRSCGETCHAR(r,"RPTGEN_SLTEVAL",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_SLTEVAL");
	FINDRSCGETCHAR(r,"RPTGEN_SLTALL",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_SLTALL");
	FINDRSCGETCHAR(r,"RPTGEN_SELECT",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_SELECT");
	FINDRSCGETCHAR(r,"RPTGEN_SLTREC",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_SLTREC");
	FINDRSCGETCHAR(r,"RPTGEN_OUTALL",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_OUTALL");
	FINDRSCGETCHAR(r,"RPTGEN_OUTEXP",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_OUTEXP");
	FINDRSCGETCHAR(r,"RPTGEN_OUTPUT",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_OUTPUT");
	FINDRSCGETCHAR(r,"RPTGEN_OUTLINE",&b);
	if(b==TRUE) LaunchStrCat(&temp,"RPTGEN_OUTLINE");
	FINDRSCGETCHAR(r,"FIELD_RPTGEN",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_RPTGEN");
	FINDRSCGETCHAR(r,"FIELD_GUI",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_GUI");
	FINDRSCGETCHAR(r,"FIELD_NRD",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_NRD");
	FINDRSCGETCHAR(r,"FIELD_EVAL",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_EVAL");
	FINDRSCGETCHAR(r,"FIELD_SEC",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_SEC");
	FINDRSCGETCHAR(r,"FIELD_VIRTUAL",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_VIRTUAL");
	FINDRSCGETCHAR(r,"FIELD_IMPORT",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_IMPORT");
	FINDRSCGETCHAR(r,"FIELD_OLH",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_OLH");
	FINDRSCGETCHAR(r,"FIELD_MIX",&b);
	if(b==TRUE) LaunchStrCat(&temp,"FIELD_MIX");
	FINDRSCGETCHAR(r,"YWIN",&b);
	if(b==TRUE) LaunchStrCat(&temp,"YWIN");
	FINDRSCGETCHAR(r,"APPLICATION",&b);
	if(b==TRUE) LaunchStrCat(&temp,"APPLICATION");
	FINDRSCGETCHAR(r,"TABLE",&b);
	if(b==TRUE) LaunchStrCat(&temp,"TABLE");
	FINDRSCGETCHAR(r,"MATRIX",&b);
	if(b==TRUE) LaunchStrCat(&temp,"MATRIX");
	FINDRSCGETCHAR(r,"LIST",&b);
	if(b==TRUE) LaunchStrCat(&temp,"LIST");
	FINDRSCGETCHAR(r,"INFORMIX_ISAM",&b);
	if(b==TRUE) LaunchStrCat(&temp,"INFORMIX_ISAM");
	FINDRSCGETCHAR(r,"POWERADD",&b);
	if(b==TRUE) LaunchStrCat(&temp,"POWERADD");

	if(!isEMPTY(temp))
	{
		es=Rmalloc(RDAstrlen(temp)+10);
		sprintf(es,"RDADIAG=%s",temp);
		RDA_PutEnv(es);
	}
	if(temp!=NULL) Rfree(temp);
	FINDRSCGETSTRING(r,"COMMAND",&temp);
	fprog=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp)+2);
	sprintf(fprog,"%s/%s",CURRENTDIRECTORY,(temp!=NULL ? temp:""));
	GUI_OpenWindow(temp);
	if(temp!=NULL) Rfree(temp);
	if(fprog!=NULL) Rfree(fprog);
}	
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char bv=FALSE;

	initrdadiag();
	SETCWD();
	INITGUI(argc,argv,CURRENTDIRECTORY);
	mainrsrc=RDArsrcNEW("UTILITIES","XPERT DIAGNOSTICS");
	addsctrsrc(mainrsrc,"COMMAND",0,"",TRUE);
	addstdrsrc(mainrsrc,"ALLOC",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"ALLOCX",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"BADALLOC",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"GUI",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"NRD",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"MIX",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"BIN",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"BINLIB",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"PROC",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"SEC",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"OLH",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"TRANS",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"EVAL",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"VF",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RF",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RDADTA",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"MISC",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"IMPORT",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"VIRTUAL",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"EXEPROG",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_SLTEVAL",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_SLTALL",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_SELECT",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_SLTREC",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_OUTALL",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_OUTEXP",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_OUTPUT",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"RPTGEN_OUTLINE",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_RPTGEN",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_GUI",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_NRD",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_EVAL",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_SEC",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_VIRTUAL",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_IMPORT",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_OLH",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"FIELD_MIX",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"YWIN",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"APPLICATION",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"TABLE",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"MATRIX",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"LIST",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"INFORMIX_ISAM",BOOLNS,1,&bv,TRUE);
	addstdrsrc(mainrsrc,"POWERADD",BOOLNS,1,&bv,TRUE);
	addbtnrsrc(mainrsrc,"XPERT.READ.ME",TRUE,XpertReadMe,NULL);
	addbtnrsrc(mainrsrc,"XPERT.GUI.README",TRUE,XpertGuiReadMe,NULL);
	addbtnrsrc(mainrsrc,"XPERT.EVALUATE.README",TRUE,XpertEvaluateReadMe,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,LaunchDiagnostics,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	mainrsrc->scn=RDAscrnNEW("UTILITIES","XPERT DIAGNOSTICS");
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,5,NULL,"URL:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,8,"COMMAND","Command",NULL,NULL,5,65,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,5,NULL,"Field / User Level Diagnostics",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"APPLICATION","Application",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_RPTGEN","Field RptGen",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_EVAL","Field Eval",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_MIX","Field Mix",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_SEC","Field Sec",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_GUI","Field Gui",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_NRD","Field NRD",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_OLH","Field OLH",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_IMPORT","Field Import",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"FIELD_VIRTUAL","Field Virtual",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,5,NULL,"RPTGEN Specific Diagnostics",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_SELECT","RptGen Select",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_SLTALL","RptGen SltAll",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_SLTEVAL","RptGen SltEval",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_SLTREC","RptGen SltRec",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_OUTPUT","RptGen OutPut",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_OUTALL","RptGen OutAll",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_OUTEXP","RptGen OutExp",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN_OUTLINE","RptGen OutLin",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,5,NULL,"Developer's Level Diagnostics",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RPTGEN","RptGen",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"EVAL","Eval",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"POWERADD","PowerAdd",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"SEC","Sec",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"GUI","Gui",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"NRD","Nrd",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"MIX","Mix",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"OLH","Olh",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"BIN","Bin",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"BINLIB","Binlib",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"TRANS","Trans",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RDADTA","RDADTA",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"VF","VFILE",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"RF","RFILE",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"TABLE","Table",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"MATRIX","Matrix",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"IMPORT","Import",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"VIRTUAL","Virtual",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"EXEPROG","Exeprog",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"YWIN","Ywindows",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"ALLOC","Alloc",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"ALLOCX","AllocX",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"BADALLOC","BadAlloc",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"MISC","Misc",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,9,"PROC","Proc",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,6,"XPERT.READ.ME","Xpert.Read.Me",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,6,"XPERT.GUI.README","Xpert.GUI.ReadMe",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,6,"XPERT.EVALUATE.README","Xpert.Evaluate.ReadMe",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,6,"SELECT","Select",NULL,NULL,0,0,25,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(mainrsrc->scn,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	APPmakescrn(mainrsrc,FALSE,quitfunc,NULL,TRUE);
}
