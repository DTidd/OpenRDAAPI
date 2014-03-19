/*Avery5260.c - Program to print Avery5260 Address Labels */
#ifndef WIN32
#define __NAM__ "avery5260.lnx"
#endif
#ifdef WIN32
#define __NAM__ "avery5260.exe"
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

char *module=NULL,*process=NULL;
char *devicex=NULL,use_formfeed=FALSE;
short FileNumber=(-1);
RDArsrc *mainrsrc=NULL;

static void addExtraFiles(RDArsrc *tmprsrc,short fileno)
{
	APPlib *files=NULL;
	char *libx=NULL;
	int x=0,y=0;
	short fileno1=(-1);
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	NRDfield *field=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(MODULENAME(fileno))+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,MODULENAME(fileno));
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,MODULENAME(fileno));
#endif
	files=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(files!=NULL)
	{
		for(x=0;x<files->numlibs;++x)
		{
			if(RDAstrcmp(FILENAME(fileno),files->libs[x]))
			{
				fileno1=OPNNRD(MODULENAME(fileno),files->libs[x]);
				if(fileno1!=(-1))
				{
					key=KEYNUM(fileno1,1);
					if(key!=NULL)
					{
						for(y=0,part=key->part;(y<key->numparts);++y,++part)
						{
							field=FLDNRD(fileno,part->name);
							if(field==NULL) break;
						}
						if(y<key->numparts)
						{
							CLSNRD(fileno1);
						} else {
							addDFincvir(tmprsrc,MODULENAME(fileno),files->libs[x],NULL,fileno1);
						}
					} else {
						CLSNRD(fileno1);
					}
				}
			}
		}
		freeapplib(files);
	}
}
static void CloseFiles(RDArsrc *tmprsrc)
{
	DFincvir *i=NULL;
	int x=0;

	if(tmprsrc!=NULL)
	{
		if(tmprsrc->incvir!=NULL)
		{
			for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
			{
				CLSNRD(i->fileno);
			}
		}
	}
}
static void quit_avery(RDArsrc *m)
{
	CloseFiles(m);
	CLSNRD(FileNumber);
	if(m!=NULL)
	{
		killwindow(m);
		free_rsrc(m);
	}
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	DFincvir *f;

	if(argc<7) return;
	module=stralloc(argv[1]);
	process=Rmalloc(RDAstrlen(argv[2])+15);
	sprintf(process,"%s LABELS 5260",argv[2]);
	if(InitializeSubsystems(argc,argv,module,process))
	{
		return(1);
	}
	FileNumber=OPNNRD(module,argv[2]);
	mainrsrc=RDArsrcNEW(module,process);
	addDFincvir(mainrsrc,module,argv[2],NULL,FileNumber);
	file2rangersrc(FileNumber,mainrsrc);
	addExtraFiles(mainrsrc,FileNumber);
	if(mainrsrc->incvir!=NULL)
	{
		for(x=0,f=mainrsrc->incvir;x<mainrsrc->numvir;++x,++f)
		{
			if(f->fileno!=(-1) && f->fileno!=FileNumber)
			{
				file2rangersrc(f->fileno,mainrsrc);
			}
		}
	}
	addbtnrsrc(mainrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(mainrsrc,"USE FORMFEED",BOOLNS,1,&use_formfeed,TRUE);
	devicex=DefaultPrinter();
	addstdrsrc(mainrsrc,"DEVICE",VARIABLETEXT,0,devicex,TRUE);
	if(devicex!=NULL) Rfree(devicex);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quit_avery,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,FALSE,quit_avery,NULL,TRUE);
}


