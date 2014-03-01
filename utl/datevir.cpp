/* datevir.c - Program to Display Virtuals with date implementations */
/*lint -library */
#ifndef WIN32
#define __NAM__ "datevir.lnx"
#endif
#ifdef WIN32
#define __NAM__ "datevir.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="DATABASE";
APPlib *DateFields=NULL;
APPlib *DateVirtuals=NULL;

static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(DateFields!=NULL) freeapplib(DateFields);
	if(DateVirtuals!=NULL) freeapplib(DateVirtuals);
	ShutdownSubsystems();
}
static short ContainsADateField(char *expression)
{
	int x=0;
	
	if(isEMPTY(expression)) return(FALSE);
	if(RDAstrstr(expression,"[SYSTEM DATE]")!=NULL) return(TRUE);
	if(RDAstrstr(expression,"[SYSTEM DATE-YYYY]")!=NULL) return(TRUE);
	if(DateFields!=NULL)
	{
		for(x=0;x<DateFields->numlibs;++x)
		{
			if(RDAstrstr(expression,DateFields->libs[x])!=NULL) return(TRUE);
		}
	}
	return(FALSE);
}
static short RDA_RecursiveVirTest(char *RDA_modulename,APPlib *file_def)
{
	int y=0,v=0;
	DFvirtual *d=NULL;
	char *temp=NULL;

	for(y=0;y<file_def->numlibs;++y)
	{
		d=getDFvirtual(RDA_modulename,file_def->libs[y]);
		if(d!=NULL)
		{
			if(ContainsADateField(d->expression))
			{
				temp=Rmalloc(RDAstrlen(d->name)+3);
				sprintf(temp,"[%s]",(d->name!=NULL?d->name:""));
				v=FINDAPPLIBELEMENT(DateFields,temp);
				if(v==(-1))
				{
					addAPPlib(DateFields,temp);
					y=0;
				}
				if(temp!=NULL) Rfree(temp);
/*
				temp=Rmalloc(RDAstrlen(d->name)+RDAstrlen(modlst->libs[x])+5);
				sprintf(temp,"[%s][%s]",modlst->libs[x],(d->name!=NULL ? d->name:""));
				addAPPlib(DateVirtuals,temp);
				if(temp!=NULL) Rfree(temp);
*/
			}
			if(d!=NULL) FreeDFvirtual(d);
		}
	}
	return(1);
}
static void datevir(RDArsrc *mainrsrc)
{
	RDA_PFILE *fp1=NULL;
	char *outputdevice=NULL;
	char *prtmessage=NULL;
	char *dirx=NULL,*tmp=NULL,*libx=NULL,*temp=NULL;
	char rng_mod=FALSE,*mod_from=NULL,*mod_to=NULL;
	int x=0,y=0;
	APPlib *modlst=NULL,*file_def=NULL;
	short fileno=(-1),nofields=0,z=0;
	NRDfield *fields=NULL,*field=NULL;
	DFvirtual *d=NULL;
	char RDA_DoRecursive=TRUE;

	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON MODULE",&rng_mod);
	FINDRSCGETSTRING(mainrsrc,"FROM MODULE",&mod_from);
	FINDRSCGETSTRING(mainrsrc,"TO MODULE",&mod_to);
	FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&outputdevice);
	FINDRSCGETCHAR(mainrsrc,"DO RECURSIVE",&RDA_DoRecursive);
	if(DateFields!=NULL) freeapplib(DateFields);
	DateFields=APPlibNEW();
	if(DateVirtuals!=NULL) freeapplib(DateVirtuals);
	DateVirtuals=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	modlst=APPlibNEW();
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(RDAstrcmp(tmp,"RPTGEN"))
		{
			addAPPlib(modlst,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modlst->numlibs<1)
	{
		if(modlst!=NULL) freeapplib(modlst);
		prterr("Error No Databases for any module to search with....");
		return;
	}
	for(x=0;x<modlst->numlibs;++x)
	{
		if(file_def!=NULL) freeapplib(file_def); 
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
		file_def=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_def!=NULL)
		{
			for(y=0;y<file_def->numlibs;++y)
			{
				fileno=OPNNRD(modlst->libs[x],file_def->libs[y]);
				if(fileno!=(-1))
				{
					fields=FLDPOINTER(fileno);
					nofields=NUMFLDS(fileno);
					if(fields!=NULL)
					{
						for(z=0,field=fields;z<nofields;++z,++field)
						{
							if(field->type==DATES)
							{
							temp=Rmalloc(RDAstrlen(modlst->libs[x])+RDAstrlen(file_def->libs[y])+RDAstrlen(field->name)+7);
							sprintf(temp,"[%s][%s][%s]",modlst->libs[x],file_def->libs[y],field->name);
							addAPPlib(DateFields,temp);
							if(temp!=NULL) Rfree(temp);
							}
						}
					}
					CLSNRD(fileno);
				} else {
					prterr("Error:  Search will not include fields from database [%s][%s], it was unable to be opened.",modlst->libs[x],file_def->libs[y]);
				}
			}
			if(file_def!=NULL) freeapplib(file_def);
		}
	}
	if(modlst!=NULL) freeapplib(modlst);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	modlst=APPlibNEW();
	for(x=0;findfile(dirx,"*.VIR",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(rng_mod==TRUE)
		{
			if((RDAstrcmp(tmp,mod_from)>=0) &&
				(RDAstrcmp(tmp,mod_to)<=0))
			{
				addAPPlib(modlst,tmp);
			}
		} else { 
			addAPPlib(modlst,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modlst->numlibs<1)
	{
		freeapplib(modlst);
		prterr("Error No Virtual Binary Libaries for any module....");
		return;
	}
	for(x=0;x<modlst->numlibs;++x)
	{
		if(file_def!=NULL) freeapplib(file_def); 
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,modlst->libs[x]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,modlst->libs[x]);
#endif
		file_def=getlibnames(libx,TRUE);
		if(libx!=NULL) Rfree(libx);
		if(file_def!=NULL)
		{
			for(y=0;y<file_def->numlibs;++y)
			{
				d=getDFvirtual(modlst->libs[x],file_def->libs[y]);
				if(d!=NULL)
				{
					if(d->type==DATES)
					{
						temp=Rmalloc(RDAstrlen(d->name)+3);
						sprintf(temp,"[%s]",(d->name!=NULL ?
							d->name:""));
						addAPPlib(DateFields,temp);
						if(temp!=NULL) Rfree(temp);
						temp=Rmalloc(RDAstrlen(d->name)+RDAstrlen(modlst->libs[x])+5);
						sprintf(temp,"[%s][%s]",modlst->libs[x],(d->name!=NULL ? d->name:""));
						addAPPlib(DateVirtuals,temp);
						if(temp!=NULL) Rfree(temp);
					}
					FreeDFvirtual(d);
				}
			}
			if(RDA_DoRecursive==TRUE)
			{
				RDA_RecursiveVirTest(modlst->libs[x],file_def);
			} 
			for(y=0;y<file_def->numlibs;++y)
			{
				d=getDFvirtual(modlst->libs[x],file_def->libs[y]);
				if(d!=NULL)
				{
					if(ContainsADateField(d->expression))
					{
						temp=Rmalloc(RDAstrlen(d->name)+RDAstrlen(modlst->libs[x])+5);
						sprintf(temp,"[%s][%s]",modlst->libs[x],(d->name!=NULL ? d->name:""));
						addAPPlib(DateVirtuals,temp);
						if(temp!=NULL) Rfree(temp);
					}
				}
				if(d!=NULL) FreeDFvirtual(d);
			}
			freeapplib(file_def);
		}

	}
	if(modlst!=NULL) freeapplib(modlst);
	if(DateVirtuals!=NULL)
	{
		if((fp1=RDA_popen(outputdevice,"w"))!=NULL)
		{
			RDA_fprintf(fp1,"VIRTUALS CONTAINING DATES\r\n");
			RDA_fprintf(fp1,"\r\n");
			for(y=0;y<DateVirtuals->numlibs;++y)
			{
				RDA_fprintf(fp1,"%s\r\n",DateVirtuals->libs[y]);
			}
			RDA_pclose(fp1);
		} else {
			prtmessage=Rmalloc(RDAstrlen(outputdevice)+67);
			sprintf(prtmessage,"The output device [%s] does not exist or cannot otherwise be opened!",outputdevice);
			ERRORDIALOG("Output Device Failure",prtmessage,NULL,FALSE);
			if(prtmessage!=NULL) Rfree(prtmessage);
			return;
		}
	} else {
		prterr("Error:  No Date Virtuals exists which need verification...");
	}
	quitfunc(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char b=FALSE;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"FIND DATE VIRTUAL")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"FIND DATE VIRTUAL");
	addstdrsrc(mainrsrc,"RANGE ON MODULE",BOOLNS,1,&b,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,NULL,TRUE);
	b=TRUE;
	addstdrsrc(mainrsrc,"DO RECURSIVE",BOOLNS,1,&b,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"SELECT",TRUE,datevir,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
