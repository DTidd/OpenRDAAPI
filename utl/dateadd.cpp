/* dateadd.c - Program to Display Adds with date implementations */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dateadd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dateadd.exe"
#endif
#include <app.hpp>
#include <powadd.hpp>
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTILITIES";
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
static void POWERADD_ContainsDate(PowerAdd *p,char *lib)
{
	AddEditWdgt *AE=NULL;
	int x,y;
	char *temp=NULL;
	AddWrite *AW=NULL;
	AddWriteField *AWF=NULL;
	MaintainButton *BTN=NULL;
	RDAaddsub *addsub=NULL;

	if(p!=NULL)
	{
		if(p->subord!=NULL)
		{
			for(x=0,addsub=p->subord;x<p->subnum;++x,++addsub)
			{
				if(!isEMPTY(addsub->expression))
				{
					if(ContainsADateField(addsub->expression))
					{
						temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(addsub->module)+RDAstrlen(addsub->filename)+200);
						sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Supporting File [%s][%s] has dates in it's expression.",lib,p->module,p->name,(addsub->module!=NULL ? addsub->module:""),(addsub->filename!=NULL ? addsub->filename:""));
						addAPPlib(DateVirtuals,temp);
						if(temp!=NULL) Rfree(temp);
					}
				}
			}
		}
		if(p->EditWdgts!=NULL)
		{
			for(x=0,AE=p->EditWdgts;x<p->numwdgts;++x,++AE)
			{
				if(AE->type==DATES)
				{
					temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(AE->name)+200);
					sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Add Edit Widget [%s] is a DATE type field.",lib,p->module,p->name,(AE->name!=NULL ? AE->name:""));
					addAPPlib(DateVirtuals,temp);
					if(temp!=NULL) Rfree(temp);
				}
				if(!isEMPTY(AE->expression))
				{
					if(ContainsADateField(AE->expression))
					{
						temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(AE->name)+200);
						sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Add Edit Widget [%s] has dates in it's expression.",lib,p->module,p->name,(AE->name!=NULL ? AE->name:""));
						addAPPlib(DateVirtuals,temp);
						if(temp!=NULL) Rfree(temp);
					}
				}
				if(!isEMPTY(AE->mgt_expression))
				{
					if(ContainsADateField(AE->mgt_expression))
					{
						temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(AE->name)+200);
						sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Add Edit Widget [%s] has dates in it's Manage ID expression.",lib,p->module,p->name,(AE->name!=NULL ? AE->name:""));
						addAPPlib(DateVirtuals,temp);
						if(temp!=NULL) Rfree(temp);
					}
				}
				if(!isEMPTY(AE->override_expression))
				{
					if(ContainsADateField(AE->override_expression))
					{
						temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(AE->name)+200);
						sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Add Edit Widget [%s] has dates in it's Override expression.",lib,p->module,p->name,(AE->name!=NULL ? AE->name:""));
						addAPPlib(DateVirtuals,temp);
						if(temp!=NULL) Rfree(temp);
					}
				}
			}
		}
		if(!isEMPTY(p->save_error))
		{
			if(ContainsADateField(p->save_error))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Save Error expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_error_desc))
		{
			if(ContainsADateField(p->save_error_desc))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Save Error Description expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_warning))
		{
			if(ContainsADateField(p->save_warning))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Save Warning expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_warning_desc))
		{
			if(ContainsADateField(p->save_warning_desc))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Save Warning Description expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_error))
		{
			if(ContainsADateField(p->quit_error))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Quit Error Expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_expression))
		{
			if(ContainsADateField(p->save_expression))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Save Expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_error_desc))
		{
			if(ContainsADateField(p->quit_error_desc))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Quit Error Description expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_warning))
		{
			if(ContainsADateField(p->quit_warning))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Quit Warning expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_warning_desc))
		{
			if(ContainsADateField(p->quit_warning_desc))
			{
			temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+200);
			sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] has dates in it's Quit Warning Description expression.",lib,p->module,p->name);
			addAPPlib(DateVirtuals,temp);
			if(temp!=NULL) Rfree(temp);
			}
		}
		if(p->Writes!=NULL)
		{
			for(x=0,AW=p->Writes;x<p->numWrites;++x,++AW)
			{
				if(!isEMPTY(AW->expression))
				{
					if(ContainsADateField(AW->expression))
					{
					temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(AW->module)+RDAstrlen(AW->filename)+200);
					sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Power Write [%s][%s] has dates in it's expression.",lib,p->module,p->name,(AW->module!=NULL ? AW->module:""),(AW->filename!=NULL ? AW->filename:""));
					addAPPlib(DateVirtuals,temp);
					if(temp!=NULL) Rfree(temp);
					}
				}
				if(AW->fields!=NULL)
				{
					for(y=0,AWF=AW->fields;y<AW->numflds;++y,++AWF)
					{
#ifdef XXXX
						if(AWF->copy_type==1)
						{
#endif /* XXXX */
						if(!isEMPTY(AWF->from_name))
						{
							if(ContainsADateField(AWF->from_name))
							{
							temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(AW->module)+RDAstrlen(AW->filename)+RDAstrlen(AWF->from_name)+200);
							sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Power Write [%s][%s] FROM Field [%s] has dates in it's expression.",lib,p->module,p->name,(AW->module!=NULL ? AW->module:""),(AW->filename!=NULL ? AW->filename:""),(AWF->from_name!=NULL ? AWF->from_name:""));
							addAPPlib(DateVirtuals,temp);
							if(temp!=NULL) Rfree(temp);
							}
						}
#ifdef XXXX
						}
#endif /* XXXX */
						if(!isEMPTY(AWF->to_name))
						{
							if(ContainsADateField(AWF->to_name))
							{
							temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(AW->module)+RDAstrlen(AW->filename)+RDAstrlen(AWF->to_name)+200);
							sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Power Write [%s][%s] to Field [%s] has dates in it's expression.",lib,p->module,p->name,(AW->module!=NULL ? AW->module:""),(AW->filename!=NULL ? AW->filename:""),(AWF->to_name!=NULL ? AWF->to_name:""));
							addAPPlib(DateVirtuals,temp);
							if(temp!=NULL) Rfree(temp);
							}
						}

					}
				}
			}
		}
		if(p->buttons!=NULL)
		{
			for(x=0,BTN=p->buttons;x<p->numbuttons;++x,++BTN)
			{
				if(!isEMPTY(BTN->expression))
				{
					if(ContainsADateField(BTN->expression))
					{
					temp=Rmalloc(RDAstrlen(p->module)+RDAstrlen(p->name)+RDAstrlen(lib)+RDAstrlen(BTN->buttonname)+200);
					sprintf(temp,"Library [%s] Module [%s] PowerAdd [%s] Button Expression [%s] has dates in it's expression.",lib,p->module,p->name,(BTN->buttonname!=NULL ? BTN->buttonname:""));
					addAPPlib(DateVirtuals,temp);
					if(temp!=NULL) Rfree(temp);
					}
				}
			}
		}
	}
}
static APPlib *makeLiblist()
{
	short fileno=(-1),ef=FALSE;
	APPlib *libavl=NULL;
	char *temp=NULL,deleteflag=FALSE,*user=NULL;

	libavl=APPlibNEW();
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+12);
#ifndef WIN32
	sprintf(temp,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(temp,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(libavl,temp);
#ifndef WIN32
	sprintf(temp,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(temp,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(libavl,temp);
	if(temp!=NULL) Rfree(temp);
	fileno=OPNNRD("SECURITY","USERS");
	if(fileno!=(-1))
	{
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(fileno,"USER IDENTIFICATION",&user);
				temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(user)+10);
#ifndef WIN32
				sprintf(temp,"%s/%s",CURRENTDIRECTORY,user);
#endif
#ifdef WIN32
				sprintf(temp,"%s\\%s",CURRENTDIRECTORY,user);
#endif
				addAPPlib(libavl,temp);
				if(temp!=NULL) Rfree(temp);
				
			}
			ef=NXTNRD(fileno,1);
		}
		if(user!=NULL) Rfree(user);
		CLSNRD(fileno);
	}
	return(libavl);
}
static void dateadd(RDArsrc *mainrsrc)
{
	RDA_PFILE *fp1=NULL;
	char *outputdevice=NULL;
	char *prtmessage=NULL;
	char *dirx=NULL,*tmp=NULL,*libx=NULL,*temp=NULL;
	char rng_mod=FALSE,*mod_from=NULL,*mod_to=NULL;
	int x=0,y=0;
	APPlib *modlst=NULL,*file_def=NULL,*liblist=NULL;
	short fileno=(-1),nofields=0,z=0;
	NRDfield *fields=NULL,*field=NULL;
	DFvirtual *d=NULL;
	PowerAdd *POWERADD=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON MODULE",&rng_mod);
	FINDRSCGETSTRING(mainrsrc,"FROM MODULE",&mod_from);
	FINDRSCGETSTRING(mainrsrc,"TO MODULE",&mod_to);
	FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&outputdevice);
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
				}
				FreeDFvirtual(d);
				}
			}
			freeapplib(file_def);
		}

	}
	if(modlst!=NULL) freeapplib(modlst);
	liblist=makeLiblist();
	if(liblist!=NULL)
	{
		if(liblist->libs!=NULL)
		{
			for(z=0;z<liblist->numlibs;++z)
			{
				dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
/*
#ifndef WIN32
				sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
				sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
				for(x=0;findfile(dirx,"*.ADD",&tmp,(int)x+1);++x) 
*/
				modlst=APPlibNEW();
				for(x=0;findfile(liblist->libs[z],"*.ADD",&tmp,(int)x+1);++x) 
				{
					tmp[RDAstrlen(tmp)-4]=0;
					if(rng_mod==TRUE)
					{
						if((RDAstrcmp(tmp,mod_from)>=0) &&
							(RDAstrcmp(tmp,mod_to)<=0))
						{
							addAPPlib(modlst,tmp);
						} else {
						}
					} else { 
						addAPPlib(modlst,tmp);
					}
				}
				if(tmp!=NULL) Rfree(tmp);
				if(dirx!=NULL) Rfree(dirx);
				if(modlst->libs!=NULL)
				{
					for(x=0;x<modlst->numlibs;++x)
					{
						if(file_def!=NULL) freeapplib(file_def); 
						libx=Rmalloc(RDAstrlen(liblist->libs[z])+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
						sprintf(libx,"%s/%s.ADD",liblist->libs[z],modlst->libs[x]);
#endif
#ifdef WIN32
						sprintf(libx,"%s\\%s.ADD",liblist->libs[z],modlst->libs[x]);
#endif
						file_def=getlibnames(libx,TRUE);
						if(file_def!=NULL)
						{
							for(y=0;y<file_def->numlibs;++y)
							{
								POWERADD=PowerAddNew(modlst->libs[x],file_def->libs[y]);
								if(!getPowerAdd(libx,POWERADD))
								{
									POWERADD_ContainsDate(POWERADD,libx);		
								} else {
									prterr("Error:  Couldn't read power add from binary library.");
								}
								if(POWERADD!=NULL) FreePowerAdd(POWERADD);
							}
							freeapplib(file_def);
						}
						if(libx!=NULL) Rfree(libx);
					}
				}
				if(modlst!=NULL) freeapplib(modlst);
			}
		}
	}
	if(DateVirtuals!=NULL)
	{
		if((fp1=RDA_popen(outputdevice,"w"))!=NULL)
		{
			RDA_fprintf(fp1,"POWERADD'S CONTAINING DATES\r\n");
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

	if(InitializeSubsystems(argc,argv,module,"FIND DATE POWERADD")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"FIND DATE POWERADD");
	addstdrsrc(mainrsrc,"RANGE ON MODULE",BOOLNS,1,&b,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,NULL,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"SELECT",TRUE,dateadd,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
