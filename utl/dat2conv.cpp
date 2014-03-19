/* dat2conv.c - Database(s) to CONVERSION */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dat2conv.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dat2conv.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include<mknrd.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="DATABASE",rng_mod=FALSE,*mod_from=NULL,*mod_to=NULL;
char rng_file=FALSE,*file_from=NULL,*file_to=NULL;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

static void quitfunc(RDArsrc *r)
{
#ifdef USE_RDACOPYFILE
	char *temp1=NULL,*temp2=NULL;

	temp1=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+50);
	temp2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+50);
#ifdef WIN32
	sprintf(temp1,"%s\\rda\\ACCT.ACC",CURRENTDIRECTORY);
	sprintf(temp2,"%s\\CONVERSION\\ACCT.ACC",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp1,"%s/rda/ACCT.ACC",CURRENTDIRECTORY);
	sprintf(temp2,"%s/CONVERSION/ACCT.ACC",CURRENTDIRECTORY);
#endif
	RDACopyFile(temp1,temp2);
#endif /* USE_RDACOPYFILE */
	if(r!=NULL) free_rsrc(r);
	if(file_from!=NULL) Rfree(file_from);
	if(file_to!=NULL) Rfree(file_to);
	if(mod_from!=NULL) Rfree(mod_from);
	if(mod_to!=NULL) Rfree(mod_to);
	ShutdownSubsystems();
}
static void dat2conv(char *modx,char *filx)
{
	char delflag=FALSE;
	short ef=(-1),from_fileno=(-1),tfileno=(-1);
	DBsort *sort=NULL;
	short x=(-1),y=(-1),nofields=0,nokeys=0,z=0;
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *k=NULL,*keys=NULL;
	NRDpart *p=NULL;
	RDArsrc *tmprsrc=NULL;
	
	if(isEMPTY(modx) || isEMPTY(filx))
	{
		prterr("Error:  Either the module or filname is empty.");
		return;	
	}
	from_fileno=OPNNRD(modx,filx);
	if(from_fileno==(-1))
	{
		prterr("Error:  Module [%s] File [%s] can't be openned.",(modx!=NULL ? modx:""),(filx!=NULL ? filx:""));
		return;
	}
	sort=DBsortNEW("CONVERSION",filx,0);
	if(sort!=NULL)
	{
		nofields=NUMFLDS(from_fileno);
		fields=FLDPOINTER(from_fileno);
		nokeys=NUMKEYS(from_fileno);
		keys=KEYPOINTER(from_fileno);
		if(fields!=NULL)
		{
			for(x=0,f=fields;x<nofields;++x,++f)
			{
				addDBfield(sort,f->name,f->type,f->len);
			}
		}	
		if(keys!=NULL)
		{
			for(x=0,k=keys;x<nokeys;++x,++k)
			{
				addDBkey(sort,k->name);
				for(y=0,p=k->part;y<k->numparts;++y,++p)
				{
					addDBkeypart(sort,(x+1),p->name);
				}
			}
		}
		CLSNRD(from_fileno);
		z=OPNDBsort(sort);
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
		tfileno=OPNNRD("CONVERSION",filx);
		if(tfileno!=(-1))
		{
			sprintf(stemp,"Copying %s %s to CONVERSION",modx,filx);
			tmprsrc=diagscrn(from_fileno,"DIAGNOSTIC SCREEN","DATABASE",stemp,NULL);
			if(tmprsrc!=NULL)
			{
				if(!ADVmakescrn(tmprsrc,TRUE))
				{
					ForceWindowUpdate(tmprsrc);
				} else {
					prterr("Error creating diagnostic screen.");	
				}
			}
			from_fileno=OPNNRD(modx,filx);
			if(from_fileno!=(-1))
			{
				nofields=NUMFLDS(from_fileno);
				fields=FLDPOINTER(from_fileno);
				ef=BEGNRD(from_fileno);
				while(!ef)
				{
					nofields=NUMFLDS(from_fileno);
					fields=FLDPOINTER(from_fileno);
					FINDFLDGETCHAR(from_fileno,"DELETEFLAG",&delflag);
					if(!delflag)
					{
						for(z=0,f=fields;z<nofields;++z,++f)
						{
							COPYFIELD(from_fileno,tfileno,f->name);
						}
						WRTNRD(tfileno,0,NULL);
					}
					delflag=FALSE;
					ef=SEQNRD(from_fileno);	
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				}
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			CLSNRD(tfileno);
		} else {
			prterr("Error:  Unable to Open Database File [CONVERSION][%s].",filx);
		}
	}
	CLSNRD(from_fileno);
}
static void dat_to_conv(RDArsrc *r)
{
	int x=0,y=0;
	char *tmp=NULL,*dirx=NULL,*libx=NULL;
	APPlib *modlst=NULL,*filedefs=NULL;
	int ret_int=0;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"RANGE MODULE",&rng_mod);
	FINDRSCGETSTRING(r,"FROM MODULE",&mod_from);
	FINDRSCGETSTRING(r,"TO MODULE",&mod_to);
	FINDRSCGETCHAR(r,"RANGE FILE",&rng_file);
	FINDRSCGETSTRING(r,"FROM FILE",&file_from);
	FINDRSCGETSTRING(r,"TO FILE",&file_to);
	if(r!=NULL)
	{
		killwindow(r);
	}
	RDAmkdir("CONVERSION",00770,&ret_int);
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(rng_mod)
		{
			if(RDAstrcmp(mod_from,tmp)<=0 && 
				RDAstrcmp(tmp,mod_to)<=0)
			{
				if(RDAstrcmp(tmp,"RPTGEN") && RDAstrcmp(tmp,"OLHELP") && RDAstrcmp(tmp,"MBGUICONV") && RDAstrcmp(tmp,"CONVERSION"))
				{
					addAPPlib(modlst,tmp);
				}
			}
		} else if(RDAstrcmp(tmp,"RPTGEN") && RDAstrcmp(tmp,"OLHELP") && RDAstrcmp(tmp,"MBGUICONV") && RDAstrcmp(tmp,"CONVERSION"))
		{
			addAPPlib(modlst,tmp);
		}
	}
	if(modlst->numlibs>0)
	{
		SORTAPPlib(modlst);
		for(x=0;x<modlst->numlibs;++x)
		{	
			if(filedefs!=NULL) freeapplib(filedefs);
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[x])+10);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
			filedefs=getlibnames(libx,TRUE);
			if(libx!=NULL) Rfree(libx);
			if(filedefs!=NULL)
			{
				SORTAPPlib(filedefs);
				for(y=0;y<filedefs->numlibs;++y)
				{
					if(rng_file)
					{
						if(RDAstrcmp(file_from,filedefs->libs[y])<=0 && RDAstrcmp(filedefs->libs[y],file_to)<=0)
						{
							dat2conv(modlst->libs[x],filedefs->libs[y]);
						}
					} else {
						dat2conv(modlst->libs[x],filedefs->libs[y]);
					}
				}
				freeapplib(filedefs);
			}
		}		
	}
	if(modlst!=NULL) freeapplib(modlst);
	quitfunc(r);
}
static void setrngmod(RDArsrc *r)
{
	readwidget(r,"RANGE MODULE");
	FINDRSCGETCHAR(r,"RANGE MODULE",&rng_mod);
	if(rng_mod==TRUE)
	{
		FINDRSCSETEDITABLE(r,"FROM MODULE",TRUE);
		FINDRSCSETSENSITIVE(r,"FROM MODULE",TRUE);
		FINDRSCSETEDITABLE(r,"TO MODULE",TRUE);
		FINDRSCSETSENSITIVE(r,"TO MODULE",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"FROM MODULE",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM MODULE",FALSE);
		FINDRSCSETEDITABLE(r,"TO MODULE",FALSE);
		FINDRSCSETSENSITIVE(r,"TO MODULE",FALSE);
	}
}
static void setrngfile(RDArsrc *r)
{
	readwidget(r,"RANGE FILE");
	FINDRSCGETCHAR(r,"RANGE FILE",&rng_file);
	if(rng_file==TRUE)
	{
		FINDRSCSETEDITABLE(r,"FROM FILE",TRUE);
		FINDRSCSETSENSITIVE(r,"FROM FILE",TRUE);
		FINDRSCSETEDITABLE(r,"TO FILE",TRUE);
		FINDRSCSETSENSITIVE(r,"TO FILE",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"FROM FILE",FALSE);
		FINDRSCSETSENSITIVE(r,"FROM FILE",FALSE);
		FINDRSCSETEDITABLE(r,"TO FILE",FALSE);
		FINDRSCSETSENSITIVE(r,"TO FILE",FALSE);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"DATABASE TO CONVERSION")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"DATABASE TO CONVERSION");
	addstdrsrc(mainrsrc,"RANGE MODULE",BOOLNS,1,&rng_mod,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",PLAINTEXT,15,mod_from,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",PLAINTEXT,15,mod_to,TRUE);
	addstdrsrc(mainrsrc,"RANGE FILE",BOOLNS,1,&rng_file,TRUE);
	addstdrsrc(mainrsrc,"FROM FILE",PLAINTEXT,15,file_from,TRUE);
	addstdrsrc(mainrsrc,"TO FILE",PLAINTEXT,15,file_to,TRUE);
	FINDRSCSETFUNC(mainrsrc,"RANGE MODULE",setrngmod,NULL);
	FINDRSCSETFUNC(mainrsrc,"RANGE FILE",setrngfile,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,dat_to_conv,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(mainrsrc);
	setrngmod(mainrsrc);
	setrngfile(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
