/* renumber transactions */
#ifndef WIN32
#define __NAM__ "rnumtran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "rnumtran.exe"
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

static APPlib *modlst=NULL;

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
					ERRORDIALOG("System Error",tmp,NULL,FALSE);
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
static void okfunction(char *module)
{
	renumbertrans(module);
}
static void okfunctioncb(RDArsrc *mainrsrc)
{
	int m;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	okfunction(modlst->libs[m]);
}
static void doexit(RDArsrc *mainrsrc)
{
	free_rsrc(mainrsrc);
	if(modlst!=NULL) freeapplib(modlst);
	ShutdownSecurity();
	EXITHELP();
	CLSLOG();
}
static int getmodulelist(char *module)
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
		addAPPlib(modlst,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlst);
	x=FINDAPPLIBELEMENT(modlst,module);
	if(x==(-1))
	{
		x=0;
	}
	return(x);
/*
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",0,modlst))
	{
		updatersrc(mainrsrc,"MODULE LIST");
	}
*/
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;
	char *module=NULL,*type=NULL;

	if(InitializeSubsystems(argc,argv,"DATABASE","RENUMBER TRANSACTIONS")) 
	{
		return;
	}
	if(argc<3)
	{
		if(argc>1)
		{
			module=stralloc(argv[1]);
		}
		mainrsrc=RDArsrcNEW("DATABASE","RENUMBER TRANSACTIONS");
		x=getmodulelist(module);
		addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,NULL,modlst->numlibs,&modlst->libs,NULL);
		if(module!=NULL) Rfree(module);
		addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunctioncb,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
		addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
	} else {
		type=stralloc(argv[2]);
		if(!RDAstrcmp(type,"ALL"))
		{
			module=stralloc(argv[1]);
			okfunction(module);
		}
		if(type!=NULL) Rfree(type);
		if(module!=NULL) Rfree(module);
	}
}
