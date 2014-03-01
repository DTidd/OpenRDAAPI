/* dfnrd.c - Define Datafiles */
#ifndef WIN32
#define __NAM__ "crtimpexp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "crtimpexp.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include<cstdlib>
#include<io.h>
#endif
#include <mix.hpp>
#include <impexp.hpp>
#include<mknrd.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char RangeMod=FALSE,Which=FALSE,*suffix=NULL;
char *FromMod=NULL,*ToMod=NULL;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */


static void doexit(RDArsrc *r)
{
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(FromMod!=NULL) Rfree(FromMod);
	if(ToMod!=NULL) Rfree(ToMod);
	if(suffix!=NULL) Rfree(suffix);
	ShutdownSubsystems();
	std::exit;
}
static void CreateIE(char *mod,char *fil)
{
	int y=0,start_offset=0,add_offset=0,lastfieldlength=0;
	int lastfieldstart=0;
	APPlib *fieldlst=NULL;
	IEfile *IMPDEF=NULL;
	IEfield *fldx=NULL;
	NRDfield *field=NULL,*fields=NULL;
	short fileno=(-1),numflds=0;

	memset(stemp,0,101);
	sprintf(stemp,"%s-%s",fil,(Which==FALSE ? "EXPORT":"IMPORT"));
	IMPDEF=IMPORTstpNEW(stemp);
	IMPDEF->num=0;
	IMPDEF->ifile=Rmalloc(RDAstrlen(mod)+RDAstrlen(suffix)+RDAstrlen(stemp)+40);
#ifndef WIN32
	sprintf(IMPDEF->ifile,"data/%s.%s.%s.csv",mod,fil,suffix);
#endif
#ifdef WIN32
	sprintf(IMPDEF->ifile,"data\\%s.%s.%s.csv",mod,fil,suffix);
#endif
	IMPDEF->module=stralloc(mod);
	IMPDEF->file=stralloc(fil);
	IMPDEF->force_quotes=TRUE;
	IMPDEF->carriage_return=FALSE;
	IMPDEF->line_feed=FALSE;
	if(Which==FALSE) IMPDEF->which=1;
		else IMPDEF->which=0;
	IMPDEF->type=1;
	IMPDEF->skip_dups=FALSE;
	IMPDEF->range=FALSE;
	IMPDEF->ask_b4_execute=FALSE;
	IMPDEF->expression=NULL;
	IMPDEF->rs=NULL;
	IMPDEF->fileno=(-1);
	IMPDEF->MainLoop=FALSE;
	IMPDEF->fdel=Rmalloc(2);
	IMPDEF->fdel[0]='\t';
	IMPDEF->fdel[1]=0;
	IMPDEF->rdel=Rmalloc(3);
	IMPDEF->rdel[0]='\r';
	IMPDEF->rdel[1]='\n';
	IMPDEF->rdel[2]=0;

	fileno=OPNNRD(mod,fil);
	if(fileno!=(-1))
	{
		fields=FLDPOINTER(fileno);
		numflds=NUMFLDS(fileno);
		fieldlst=APPlibNEW();
		for(y=0,field=fields;y<numflds;++y,++field)
		{
			addAPPlibNoDuplicates(fieldlst,field->name);
		}
		SORTAPPlib(fieldlst);
		IMPDEF->fields=Rmalloc(numflds*sizeof(IEfield));
		IMPDEF->num=numflds;
		fldx=IMPDEF->fields;
		for(y=0;y<fieldlst->numlibs;++y)
		{
			field=FLDNRD(fileno,fieldlst->libs[y]);
			fldx->name=stralloc(field->name);
			fldx->type=field->type;
			fldx->expression=NULL;
			fldx->use_quotes=TRUE;
			fldx->value_expression=NULL;
			if(y==0)
			{
				if(start_offset<2) start_offset=1;
				fldx->start=start_offset;
			} else {
				fldx->start=lastfieldstart+lastfieldlength+add_offset;
			}
			fldx->length=field->len;
			lastfieldstart=fldx->start;
			lastfieldlength=fldx->length;
			++fldx;
		}
		if(writeIMPORTbin(mod,IMPDEF))
		{
			ERRORDIALOG("Cannot Write Import/Export","Error the user cannot write the import/export definition to the library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		}
		CLSNRD(fileno);
	}
	if(IMPDEF!=NULL) free_import(IMPDEF);
}
static void okfunction(RDArsrc *r)
{
	int y=0,z=0;
	char *tmp=NULL,*dirx=NULL,*libx=NULL;
	APPlib *modlst=NULL,*filelst=NULL;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"WHICH",&Which);
	FINDRSCGETCHAR(r,"RANGE MODULE",&RangeMod);
	FINDRSCGETSTRING(r,"FROM MODULE",&FromMod);
	FINDRSCGETSTRING(r,"TO MODULE",&ToMod);
	FINDRSCGETSTRING(r,"SUFFIX",&suffix);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	modlst=APPlibNEW();
	for(z=0;findfile(dirx,"*.FIL",&tmp,(int)z+1);++z) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(RDAstrcmp(tmp,"RPTGEN") && RDAstrcmp(tmp,"CONVERSION") && RDAstrcmp(tmp,"MBGUICONV")) 
		{
			if(!RangeMod || ((RDAstrcmp(FromMod,tmp)<=0) && RDAstrcmp(tmp,ToMod)<=0))
			{
				addAPPlib(modlst,tmp);
			}
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(modlst->numlibs<1)
	{
		prterr("Error:  No modules found (*.FIL's).");
	} else {
		SORTAPPlib(modlst);
		for(z=0;z<modlst->numlibs;++z)
		{
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[z])+10);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[z]);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[z]);
#endif
			filelst=getlibnames(libx,TRUE);
			SORTAPPlib(filelst);
			if(libx!=NULL) Rfree(libx);
			if(filelst->numlibs>0)
			{
				for(y=0;y<filelst->numlibs;++y)
				{
					CreateIE(modlst->libs[z],filelst->libs[y]);
				}
			}
			if(libx!=NULL) Rfree(libx);
			if(filelst!=NULL) freeapplib(filelst);
		}
	}
	if(modlst!=NULL) freeapplib(modlst);
	doexit(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int ret_int=0;

	RDA_SOFTWARE_TYPE=RDA_ALL_APP;
	if(InitializeSubsystems(argc,argv,"DATABASE","CREATE IMPORT/EXPORT")) 
	{
		return;
	}
	RDAmkdir("data",00770,&ret_int);
#ifdef USE_MYISAM
	suffix=stralloc("myisam");
#endif
#ifndef USE_MYISAM
	suffix=stralloc("orig");
#endif
	mainrsrc=RDArsrcNEW("DATABASE","CREATE IMPORT EXPORT");
	addstdrsrc(mainrsrc,"SUFFIX",PLAINTEXT,15,suffix,TRUE);
	addstdrsrc(mainrsrc,"WHICH",BOOLNS,1,&Which,TRUE);
	addstdrsrc(mainrsrc,"RANGE MODULE",BOOLNS,1,&RangeMod,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",PLAINTEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",PLAINTEXT,15,NULL,TRUE);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
