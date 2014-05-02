/* dfie.c - Define Import Export */
#ifndef WIN32
#define __NAM__ "dfie.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfie.exe"
#endif
#include <app.hpp>



#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <impexp.hpp>
#include <ldval.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void setvaluesx(RDArsrc *),setselectfile(RDArsrc *),makefldlist1(void);
static void quitfld(RDArsrc *,RDArsrc *),quitfldtest(RDArsrc *,RDArsrc *);
static void savefld(RDArsrc *,RDArsrc *);
static void quitimp(RDArsrc *,RDArsrc *),quitimptest(RDArsrc *,RDArsrc *);
static void saveimp(RDArsrc *,RDArsrc *);
static void free_holdfields(NRDfield *,int);
static APPlib *modlstx=NULL,*defsavl=NULL,*file_defs=NULL,*fieldlst=NULL;
static APPlib *flddefs=NULL,*ftypes=NULL,*ietypes=NULL,*rtypes=NULL;
static APPlib *qtypes=NULL;
static char *module="DATABASE";
static IEfile *IMPDEF=NULL;
static int numfields=0;
static NRDfield *holdfields=NULL;
static RDAdefault *flddefaults=NULL,*mtndefaults=NULL;
static void printdefinition(RDArsrc *,IEfile *),printdefinitioncb(RDArsrc *,IEfile *);
static void quit_print_definition(RDArsrc *);
static char changedvalues=FALSE;
static short truevalue=TRUE,falsevalue=FALSE;

static APPlib *makefieldvallist(RDArsrc *r)
{
	APPlib *tmp=NULL;
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y,selmod=0,selfil=0,modlen=0,fillen=0;
	char *temp=NULL;

	tmp=APPlibNEW();
	if(FINDRSCGETINT(r,"MODULE LIST",&selmod)) return(tmp);
	if(FINDRSCGETINT(r,"FILE LIST",&selfil)) return(tmp);
	if(!GETBIN(modlstx->libs[selmod],file_defs->libs[selfil],&engine,&nofields,&fields,
		&nokeys,&keys))
	{
		modlen=RDAstrlen(modlstx->libs[selmod]);
		fillen=RDAstrlen(file_defs->libs[selfil]);
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part
						;z<keyx->numparts;++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
		}
		Rfree(keys);
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL)
				{
					temp=Rmalloc(modlen+fillen+RDAstrlen(f->name)+7);
					sprintf(temp,"[%s][%s][%s]",modlstx->libs[selmod],file_defs->libs[selfil],f->name);
					addAPPlib(tmp,temp);
					if(temp!=NULL) Rfree(temp);
					Rfree(f->name);
				}
			}
			Rfree(fields);
		}
	}
	return(tmp);
}
static APPlib *makefieldvallist2(RDArsrc *r)
{
	APPlib *tmp=NULL;
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y,selmod=0,selfil=0,modlen=0,fillen=0;
	char *temp=NULL;

	tmp=APPlibNEW();
	if(FINDRSCGETINT(r,"MODULE LIST",&selmod)) return(tmp);
	if(FINDRSCGETINT(r,"FILE LIST",&selfil)) return(tmp);
	if(!GETBIN(modlstx->libs[selmod],file_defs->libs[selfil],&engine,&nofields,&fields,
		&nokeys,&keys))
	{
		modlen=RDAstrlen(modlstx->libs[selmod]);
		fillen=RDAstrlen(file_defs->libs[selfil]);
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part
						;z<keyx->numparts;++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
		}
		Rfree(keys);
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL)
				{
					temp=Rmalloc(modlen+fillen+RDAstrlen(f->name)+7);
					sprintf(temp,"[%s][%s][%s]",modlstx->libs[selmod],file_defs->libs[selfil],f->name);
					addAPPlib(tmp,temp);
					if(temp!=NULL) Rfree(temp);
					Rfree(f->name);
				}
			}
			Rfree(fields);
		}
	}
	addAPPlib(tmp,"[NEXT TRANSACTION NO]");
	addAPPlib(tmp,"[RECORD COUNTER]");
	loadglobals(tmp);
	SORTAPPlib(tmp);
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist(load->val);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	if(loadlist->numlibs>1)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP",13)) break;
		}
	}
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void loadvalue1(RDArsrc *r,RDArsrc *mtnrsrc)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,"EXPRESSION");
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist(mtnrsrc);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void loadvalue2(RDArsrc *r,RDArsrc *mtnrsrc)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,"VALUE EXPRESSION");
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist2(mtnrsrc);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void quit_print_definition(RDArsrc *prsrc)
{
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void printdefinitioncb(RDArsrc *parent,IEfile *IEdef)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printdefinition,IEdef);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_definition,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_definition(prsrc);
	}
}
void printdefinition(RDArsrc *prsrc,IEfile *IEdef)
{
	IEfield *field;
        RDA_PFILE *fp=NULL;
        int x=0,y=0,z=0,pages=0;
	int p=0;
        char *outdevice=NULL,*message=NULL,print_style=TRUE;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
			RDA_fprintf(fp,"%*sXPERT IMPORT/EXPORT DEFINITION                Page: %*d\n",24,"",4,++pages);
			RDA_fprintf(fp,"%*sModule [%s]  Name [%s]  File [%s]\n\n",
				((80-(26+RDAstrlen(IEdef->module)+
				RDAstrlen(IEdef->name)+RDAstrlen(IEdef->file)))/2),""
				,IEdef->module,IEdef->name,IEdef->file);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!","The output attemp to the DEVICE failed when trying to open pipe.",NULL,NULL,TRUE,NULL);
			prterr("Output to Device [%s] call failed at line [%d] program [%s].",outdevice,__LINE__,__FILE__);
			if(message!=NULL) Rfree(message);
			return;
		}
		RDA_fprintf(fp,"  Direction: [%s]\n",
			(IEdef->which==0?"Import To Datafile":"Export From Datafile"));
		RDA_fprintf(fp,"  Description:\n");
		if(RDAstrlen(IEdef->desc)<75)
		{
			RDA_fprintf(fp,"    %s\n",IEdef->desc);
		} else {
			for(x=0;x<RDAstrlen(IEdef->desc);)
			{
				if((RDAstrlen(IEdef->desc)-x)<75)
				{
					z=(RDAstrlen(IEdef->desc)-x);
				} else { 
					z=75;
				}
				for(y=0;y<z;++y)
				{
					stemp[y]=IEdef->desc[x];
					++x;
				}
				stemp[y]=0;
				RDA_fprintf(fp,"    %s\n",stemp);
			}
		}
		RDA_fprintf(fp,"  Import File Name: [%s]  Type: [%s]\n",
			IEdef->ifile,
			(IEdef->type==0?"Flat":"Field Record Delimited"));
		if(IEdef->type==1)
		{
			RDA_fprintf(fp,"  Field Delimiter: [%s]   Record Delimiter: [%s]\n",
				(IEdef->fdel!=NULL?IEdef->fdel:""),
				(IEdef->rdel!=NULL?IEdef->rdel:""));
		}
		RDA_fprintf(fp,"  Use Range Screen: [%s]   Ask Before Execute: [%s]\n",
			(IEdef->range==TRUE?"TRUE":"FALSE"),
			(IEdef->ask_b4_execute==TRUE?"TRUE":"FALSE"));
		RDA_fprintf(fp,"  Carriage Return: [%s]  Line Feed: [%s]\n",
			(IEdef->carriage_return==TRUE?"TRUE":"FALSE"),
			(IEdef->line_feed==TRUE?"TRUE":"FALSE"));
		RDA_fprintf(fp,"  Skip Duplicates: [%s]  Force Quotes: [%s]\n",
			(IEdef->skip_dups==TRUE?"TRUE":"FALSE"),
			(IEdef->force_quotes==TRUE?"TRUE":"FALSE"));
                if(!isEMPTY(IEdef->expression))
		{
		RDA_fprintf(fp,"  Expression:\n");
		if(RDAstrlen(IEdef->expression)<75)
		{
			RDA_fprintf(fp,"    %s\n",IEdef->expression);
		} else {
			for(x=0;x<RDAstrlen(IEdef->expression);)
			{
				if((RDAstrlen(IEdef->expression)-x)<75)
				{
					z=(RDAstrlen(IEdef->expression)-x);
				} else { 
					z=75;
				}
				for(y=0;y<z;++y)
				{
					stemp[y]=IEdef->expression[x];
					++x;
				}
				stemp[y]=0;
				RDA_fprintf(fp,"    %s\n",stemp);
			}
		}
		}
		RDA_fprintf(fp,"Fields :\n");
		if(IEdef->fields!=NULL)
		{
			if(IEdef->num>0)
			{
				for(x=0,field=IEdef->fields;x<IEdef->num;++x,++field)
				{
					RDA_fprintf(fp,"%3d  Name: [%s] Type: [%s] Start: [%d] Length: [%d] Use Quotes: [%d]\n",x+1,
						(field->name!=NULL?field->name:""),
						standardfieldtypes[field->type],field->start,field->length,field->use_quotes);
					if(!isEMPTY(field->expression))
					{
					RDA_fprintf(fp,"      Expression:\n");
					if(RDAstrlen(field->expression)<55)
					{
						RDA_fprintf(fp,"      %s\n",field->expression);
					} else {
						for(p=0;p<RDAstrlen(field->expression);)
						{
							if((RDAstrlen(field->expression)-p)<55)
							{
								z=(RDAstrlen(field->expression)-p);
							} else { 
								z=55;
							}
							for(y=0;y<z;++y)
							{
								stemp[y]=field->expression[p];
								++p;
							}
							stemp[y]=0;
							RDA_fprintf(fp,"      %s\n",stemp);
						}
					}
					}
					if(!isEMPTY(field->value_expression))
					{
					RDA_fprintf(fp,"      Value Expression:\n");
					if(RDAstrlen(field->value_expression)<55)
					{
						RDA_fprintf(fp,"      %s\n",field->value_expression);
					} else {
						for(p=0;p<RDAstrlen(field->value_expression);)
						{
							if((RDAstrlen(field->value_expression)-p)<55)
							{
								z=(RDAstrlen(field->value_expression)-p);
							} else { 
								z=55;
							}
							for(y=0;y<z;++y)
							{
								stemp[y]=field->value_expression[p];
								++p;
							}
							stemp[y]=0;
							RDA_fprintf(fp,"      %s\n",stemp);
						}
					}
					}
				}
			} else {
				RDA_fprintf(fp,"No Fields Defined.");
			}
		}
		RDA_pclose(fp);
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}
void dofilelist(RDArsrc *mtnrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL,*temp=NULL;

	if(FINDRSCGETINT(mtnrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mtnrsrc,"FILE LIST",&selectedf);
	if(file_defs!=NULL) freeapplib(file_defs);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+10);
	temp=adddashes(modlstx->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,temp);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,temp);
#endif
	if(temp!=NULL) Rfree(temp);
	file_defs=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(file_defs==NULL)
	{
		file_defs=APPlibNEW();
		addAPPlib(file_defs,"Contains No File Definitions");
	}
	if(selectedf>=file_defs->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"FILE LIST",selectedf,file_defs))
		updatersrc(mtnrsrc,"FILE LIST");
/*
	setselectfile(mtnrsrc);
*/
}
void getdefinitionlist1(RDArsrc *mainrsrc)
{
	int x,selected=0;
	char *tmp=NULL,*dirx=NULL;

	FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected);
	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=APPlibNEW();
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
		addAPPlib(modlstx,tmp);
	}
	if(modlstx->numlibs<1)
	{
		addAPPlib(modlstx,"Contains No Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlstx);
	if(selected>=modlstx->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selected,modlstx))
	{
		updatersrc(mainrsrc,"MODULE LIST");
	}
}
void getimpentries(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL,*temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selectedf);
	if(defsavl!=NULL) freeapplib(defsavl);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+10);
	temp=adddashes(modlstx->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.IMP",CURRENTDIRECTORY,temp);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.IMP",CURRENTDIRECTORY,temp);
#endif
	if(temp!=NULL) Rfree(temp);
	defsavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(defsavl==NULL)
	{
		defsavl=APPlibNEW();
		addAPPlib(defsavl,"Contains No Definitions");
	}
	if(selectedf>=defsavl->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",selectedf,defsavl))
		updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
	setvaluesx(mainrsrc);
}
static void makefldlist1(void)
{
	int l=0;
	short x;
	IEfield *field;
	char *s;

	if(flddefs!=NULL) freeapplib(flddefs);
	flddefs=APPlibNEW();
	if(IMPDEF->fields!=NULL)
	{
		for(x=0,field=IMPDEF->fields;x<IMPDEF->num;++x,++field)
		{
			if(!IMPDEF->type)
			{
				l=RDAstrlen(standardfieldtypes[field->type])+RDAstrlen(field->name)+(field->use_quotes ? 3:2)+5+5+120;
				s=Rmalloc(l);
				sprintf(s,"%3d Name: [%s] Type: [%s] Start: [%d] Length: [%d] Quotes: [%d] Expression: [%s] Value Expression: [%s]",x+1,
					(field->name!=NULL ? field->name:""),
					standardfieldtypes[field->type],field->start,field->length,
					field->use_quotes,
					(field->expression!=NULL ? "True":"False"),
					(field->value_expression!=NULL ? "True":"False"));
			} else {
				l=RDAstrlen(standardfieldtypes[field->type])+RDAstrlen(field->name)+(field->use_quotes ? 3:2)+(field->expression!=NULL ? 3:2)+5+5+70;
				s=Rmalloc(l);
				sprintf(s,"%3d Name: [%s] Type: [%s] Quotes: [%d] Expression: [%s] Value Expression: [%s]",x+1,
					(field->name!=NULL ? field->name:""),
					standardfieldtypes[field->type],
					field->use_quotes,
					(field->expression!=NULL ? "Yes":"No"),
					(field->value_expression!=NULL ? "Yes":"No"));
			}
			addAPPlib(flddefs,s);
			Rfree(s);
		}
	}
	if(flddefs->numlibs<1)
	{
		addAPPlib(flddefs,"No Fields Defined");
	}
}
void makespacefld1(int pos)
{
	short x,y;
	IEfield *temp,*temp1,*new_fields;

	++IMPDEF->num;
	new_fields=Rmalloc(IMPDEF->num*sizeof(IEfield));
	y=0;
	for(x=0,temp=IMPDEF->fields;x<(IMPDEF->num-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->type=0;
			temp1->start=0;
			temp1->length=0;
			temp1->expression=NULL;
			temp1->use_quotes=FALSE;
			temp1->value_expression=NULL;
			++y;
			temp1=new_fields+y;
		}
		temp1->type=temp->type;
		temp1->use_quotes=temp->use_quotes;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->value_expression!=NULL)
		{
			temp1->value_expression=stralloc(temp->value_expression);
			Rfree(temp->value_expression);
		} else temp1->value_expression=NULL;
		temp1->start=temp->start;
		temp1->length=temp->length;
		++y;
	}
	Rfree(IMPDEF->fields);
	IMPDEF->fields=Rmalloc(IMPDEF->num*sizeof(IEfield));
	for(x=0,temp1=IMPDEF->fields,temp=new_fields;x<IMPDEF->num;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		temp1->use_quotes=temp->use_quotes;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->value_expression!=NULL)
		{
			temp1->value_expression=stralloc(temp->expression);
			Rfree(temp->value_expression);
		} else temp1->value_expression=NULL;
		temp1->start=temp->start;
		temp1->length=temp->length;
	}
	Rfree(new_fields);
}
static void quitfld(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	if(fldrsrc!=NULL)
	{
		killwindow(fldrsrc);
		free_rsrc(fldrsrc);
	}
	if(flddefaults!=NULL) FreeRDAdefault(flddefaults);
}
static void quitfldcls(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	changedvalues=FALSE;
	quitfld(fldrsrc,mtnrsrc);
}
static void quitfldtest(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(fldrsrc);
	if(COMPARE_RSRCS(fldrsrc,flddefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this CONVERSION FIELD DEFINITION.\nDo you want to Save these changes?",savefld,quitfldcls,FALSE,2,fldrsrc,mtnrsrc,NULL);
	} else {
		quitfld(fldrsrc,mtnrsrc);
	}
}
static void savefld(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	int value=0,x=0;
	IEfield *field;

	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&value)) return;
	field=IMPDEF->fields+value;
	readallwidgets(fldrsrc);
	FINDRSCGETINT(fldrsrc,"FIELD LIST",&x);
	field->name=stralloc(fieldlst->libs[x]);
	FINDRSCGETSHORT(fldrsrc,"START",&field->start);
	FINDRSCGETINT(fldrsrc,"LENGTH",&x);
	field->length=(unsigned)x;
	FINDRSCGETINT(fldrsrc,"FIELD TYPES",&x);
	field->type=x;
	FINDRSCGETINT(fldrsrc,"USE QUOTES",&x);
	field->use_quotes=x;
	FINDRSCGETSTRING(fldrsrc,"EXPRESSION",&field->expression);
	FINDRSCGETSTRING(fldrsrc,"VALUE EXPRESSION",&field->value_expression);
	makefldlist1();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"FIELD DEFINITIONS",value,flddefs))
		updatersrc(mtnrsrc,"FIELD DEFINITIONS");
	if(COMPARE_RSRCS(fldrsrc,flddefaults,2))
	{
		changedvalues=TRUE;
	}
	quitfld(fldrsrc,mtnrsrc);
}
void save_above(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	int x=0;

	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&x)) return;
	makespacefld1(x);
	savefld(fldrsrc,mtnrsrc);
}
void save_below(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	int x=0;

	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&x)) return;
	makespacefld1(++x);
	FINDRSCSETINT(mtnrsrc,"FIELD DEFINITIONS",x);
	updatersrc(mtnrsrc,"FIELD DEFINITIONS");
	savefld(fldrsrc,mtnrsrc);
}
static void quitfldabovetest(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(fldrsrc);
	if(COMPARE_RSRCS(fldrsrc,flddefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this CONVERSION FIELD DEFINITION.\nDo you want to Save these changes?",save_above,quitfldcls,FALSE,2,fldrsrc,mtnrsrc,NULL);
	} else {
		quitfld(fldrsrc,mtnrsrc);
	}
}
static void quitfldbelowtest(RDArsrc *fldrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(fldrsrc);
	if(COMPARE_RSRCS(fldrsrc,flddefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this CONVERSION FIELD DEFINITION.\nDo you want to Save these changes?",save_below,quitfldcls,FALSE,2,fldrsrc,mtnrsrc,NULL);
	} else {
		quitfld(fldrsrc,mtnrsrc);
	}
}
void estfieldvalues1(RDArsrc *fldrsrc,short *listcallback)
{
	int length=0,selectedf=0,x=0,type=0;
	NRDfield *f=NULL;	

	if(FINDRSCGETINT(fldrsrc,"FIELD LIST",&selectedf)) return;
	FINDRSCGETINT(fldrsrc,"LENGTH",&length);
	for(x=0,f=holdfields;x<numfields;++x,++f)
	{
		if(!RDAstrcmp(f->name,fieldlst->libs[selectedf])) break;
	}
	if(x<numfields)
	{
		type=(int)f->type;
		FINDRSCSETINT(fldrsrc,"FIELD TYPES",type);
		updatersrc(fldrsrc,"FIELD TYPES");
		if(*listcallback || (length==0 && f->len!=0))
		{
			FINDRSCSETINT(fldrsrc,"LENGTH",f->len);
			updatersrc(fldrsrc,"LENGTH");
		}
	}
}
void fielddefinition1(RDArsrc *mtnrsrc,char *name,short type,short start,short length,void (*savefunc)(...),void (*quitfunctest)(...),char *expression,char use_quotes,char *value_expression)
{
	int typex,dowhich=0,uquotes=0;
	RDArsrc *fldrsrc=NULL;
	char force_quotes=TRUE;

	FINDRSCGETINT(mtnrsrc,"TYPES",&dowhich);
	FINDRSCGETCHAR(mtnrsrc,"FORCE QUOTES",&force_quotes);
	fldrsrc=RDArsrcNEW(module,"MAINTAIN IMPORT EXPORT FIELD");
	addstdrsrc(fldrsrc,"START",SHORTV,5,&start,TRUE);
	addstdrsrc(fldrsrc,"LENGTH",SHORTV,4,&length,TRUE);
	uquotes=use_quotes;
	addlstrsrc(fldrsrc,"USE QUOTES",&uquotes,TRUE,NULL,qtypes->numlibs,
		&qtypes->libs,NULL);
	if(!dowhich)
	{
		FINDRSCSETSENSITIVE(fldrsrc,"START",TRUE);
		FINDRSCSETSENSITIVE(fldrsrc,"LENGTH",TRUE);
	} else {
		FINDRSCSETSENSITIVE(fldrsrc,"START",FALSE);
		FINDRSCSETSENSITIVE(fldrsrc,"LENGTH",FALSE);
	}
	typex=type;
	addlstrsrc(fldrsrc,"FIELD TYPES",&typex,FALSE,NULL,ftypes->numlibs,
		&ftypes->libs,NULL);
	for(typex=0;typex<fieldlst->numlibs;++typex)
	{
		if(!RDAstrcmp(fieldlst->libs[typex],name)) break;
	}
	if(fieldlst->numlibs<=typex) typex=0;
	addlstrsrc(fldrsrc,"FIELD LIST",&typex,TRUE,estfieldvalues1,
		fieldlst->numlibs,&fieldlst->libs,&truevalue);
	addsctrsrc(fldrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),TRUE);
	addbtnrsrc(fldrsrc,"LOAD VALUE",TRUE,loadvalue1,mtnrsrc);
	addbtnrsrc(fldrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(fldrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(fldrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addsctrsrc(fldrsrc,"VALUE EXPRESSION",0,(value_expression!=NULL ? value_expression:""),TRUE);
	addbtnrsrc(fldrsrc,"LOAD VALUE VALUE EXPRESSION",TRUE,loadvalue2,mtnrsrc);
	addbtnrsrc(fldrsrc,"LOAD OPERATOR VALUE EXPRESSION",TRUE,loadoperand,"VALUE EXPRESSION");
	addbtnrsrc(fldrsrc,"LOAD GROUPER VALUE EXPRESSION",TRUE,loadgrouper,"VALUE EXPRESSION");
	addbtnrsrc(fldrsrc,"LOAD CONTROL VALUE EXPRESSION",TRUE,loadcontrol,"VALUE EXPRESSION");
	addrfcbrsrc(fldrsrc,"SAVE",TRUE,savefunc,mtnrsrc);
	addrfcbrsrc(fldrsrc,"QUIT",TRUE,quitfunctest,mtnrsrc);
	addbtnrsrc(fldrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(fldrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	estfieldvalues1(fldrsrc,&falsevalue);
	flddefaults=GetDefaults(fldrsrc);
	APPmakescrn(fldrsrc,TRUE,quitfld,mtnrsrc,FALSE);
}
static void add_above(RDArsrc *mtnrsrc)
{
	int selected=0;
	short start;
	IEfield *field;
	char force_quotes=FALSE;

	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&selected)) return;
	readwidget(mtnrsrc,"FORCE QUOTES");
	FINDRSCGETCHAR(mtnrsrc,"FORCE QUOTES",&force_quotes);
	if(flddefs->numlibs==1 && !RDAstrcmp(flddefs->libs[0],"No Fields Defined")) return;
	if(IMPDEF->fields!=NULL && IMPDEF->num>selected && selected)
	{
		field=IMPDEF->fields+selected-1;
		start=field->start+field->length;
	} else start=1;
	fielddefinition1(mtnrsrc,NULL,0,start,0,save_above,quitfldabovetest,NULL,force_quotes,NULL);
}
static void add_below(RDArsrc *mtnrsrc)
{
	int selected=0;
	short start;
	IEfield *field;
	char force_quotes=FALSE;

	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&selected)) return;
	readwidget(mtnrsrc,"FORCE QUOTES");
	FINDRSCGETCHAR(mtnrsrc,"FORCE QUOTES",&force_quotes);
	if(flddefs->numlibs==1 && !RDAstrcmp(flddefs->libs[0],"No Fields Defined")) return;
	if(IMPDEF->fields!=NULL && IMPDEF->num>selected && selected)
	{
		field=IMPDEF->fields+selected;
		start=field->start+field->length;
	} else start=1;
	fielddefinition1(mtnrsrc,NULL,0,start,0,save_below,quitfldbelowtest,NULL,force_quotes,NULL);
}
static void editfld(RDArsrc *mtnrsrc)
{
	int x=0;
	IEfield *f;
	char force_quotes=FALSE;

	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&x)) return;
	readwidget(mtnrsrc,"FORCE QUOTES");
	FINDRSCGETCHAR(mtnrsrc,"FORCE QUOTES",&force_quotes);
	if(IMPDEF->fields==NULL)
	{
		IMPDEF->fields=Rmalloc(sizeof(IEfield));
		f=IMPDEF->fields;
		f->start=1;
		f->length=0;
		f->type=0;
		f->name=NULL;
		f->use_quotes=force_quotes;
		f->expression=NULL;
		f->value_expression=NULL;
		IMPDEF->fields=f;
		IMPDEF->num=1;
	}
	f=IMPDEF->fields+x;
	fielddefinition1(mtnrsrc,f->name,f->type,f->start,f->length,savefld,quitfldtest,f->expression,f->use_quotes,f->value_expression);
}
static void deletefld1(RDArsrc *mtnrsrc)
{
	short x,y;
	IEfield *temp,*temp1,*new_fields;
	int value;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&value)) return;
	if(IMPDEF->num==1) return;
	new_fields=Rmalloc((IMPDEF->num-1)*sizeof(IEfield));
	y=0;
	for(x=0,temp=IMPDEF->fields;x<IMPDEF->num;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->use_quotes=temp->use_quotes;
			temp1->start=temp->start;
			temp1->length=temp->length;
			temp1->type=temp->type;
			if(temp->expression!=NULL)
			{ 
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			if(temp->value_expression!=NULL)
			{ 
				temp1->value_expression=stralloc(temp->value_expression);
				Rfree(temp->value_expression);
			} else temp1->value_expression=NULL;
			++y;
		} else {
			if(temp->name!=NULL) Rfree(temp->name);
		}
	}
	Rfree(IMPDEF->fields);
	--IMPDEF->num;
	IMPDEF->fields=Rmalloc(IMPDEF->num*sizeof(IEfield));
	for(x=0,temp1=IMPDEF->fields,temp=new_fields;x<IMPDEF->num;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->value_expression!=NULL)
		{
			temp1->value_expression=stralloc(temp->value_expression);
			Rfree(temp->value_expression);
		} else temp1->value_expression=NULL;
		temp1->start=temp->start;
		temp1->length=temp->length;
		temp1->type=temp->type;
		temp1->use_quotes=temp->use_quotes;
	}
	Rfree(new_fields);
	makefldlist1();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"FIELD DEFINITIONS",value,flddefs))
		updatersrc(mtnrsrc,"FIELD DEFINITIONS");
	changedvalues=TRUE;
}
static void setselectfile(RDArsrc *mtnrsrc)
{
	NRDfield *f,*fields=NULL,*ezf;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y,selectedm,selectedf;

	if(FINDRSCGETINT(mtnrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mtnrsrc,"FILE LIST",&selectedf)) return;
	if(fieldlst!=NULL) freeapplib(fieldlst);
	fieldlst=APPlibNEW();
	if(holdfields!=NULL) { free_holdfields(holdfields,numfields); holdfields=NULL; } 
	if(!GETBIN(modlstx->libs[selectedm],file_defs->libs[selectedf],&engine,&nofields,&fields,
		&nokeys,&keys)) 
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
			Rfree(keys);
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(holdfields!=NULL)
					{
						holdfields=Rrealloc(holdfields,(numfields+1)*
							sizeof(NRDfield));
					} else {
						holdfields=Rmalloc(sizeof(NRDfield));
						numfields=0;
					}
					ezf=holdfields+numfields;
					if(f->name!=NULL)
					{
						ezf->name=stralloc(f->name);
						addAPPlib(fieldlst,f->name);
						Rfree(f->name);
					} else ezf->name=NULL;	
					ezf->type=f->type;
					ezf->len=f->len;
					++numfields;
				}
				Rfree(fields);
			}
		}
	}
	if(fieldlst->numlibs<1)
	{
		addAPPlib(fieldlst,"No Fields Available");
	}
	SORTAPPlib(fieldlst);
}
static void free_holdfields(NRDfield *holdfields,int numfields)
{
	int y=0;
	NRDfield *ezf=NULL;

	if(holdfields!=NULL)
	{
		for(y=0,ezf=holdfields;y<numfields;++y,++ezf)
		{
			if(ezf!=NULL)
			{
				if(ezf->name!=NULL)
				{
					Rfree(ezf->name);
				}
			}
		}
		Rfree(holdfields);
		numfields=0;
	}
}
static void quitimp(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	if(fieldlst!=NULL) freeapplib(fieldlst);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(ftypes!=NULL) freeapplib(ftypes);
	if(qtypes!=NULL) freeapplib(qtypes);
	if(ietypes!=NULL) freeapplib(ietypes);
	if(rtypes!=NULL) freeapplib(rtypes);
	if(mtndefaults!=NULL) FreeRDAdefault(mtndefaults);
	if(IMPDEF!=NULL) free_import(IMPDEF);
	if(holdfields!=NULL) { free_holdfields(holdfields,numfields); holdfields=NULL; } 
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void quitimptest(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMPORT/EXPORT DEFINITION.\nDo you want to Save these changes?",saveimp,quitimp,FALSE,2,mtnrsrc,mainrsrc,NULL);
	} else {
		quitimp(mtnrsrc,mainrsrc);
	}
}
#define MAXIMUM_SCREEN_LINES	15

static char *ReturnScreenName(char *filename,NRDfield *f)
{
	char *tmp=NULL;

	tmp=Rmalloc(RDAstrlen(filename)+RDAstrlen(f->name)+5);
	sprintf(tmp,"[%s][%s]",(filename!=NULL ? filename:""),(f->name!=NULL ? f->name:""));
	return(tmp);
}
static char *ReturnScreenLabel(char *name)
{
	char *xmn=NULL,*xfdn=NULL;
	char *ret=NULL;
	char last=FALSE;
	int x=0;

	if(!isEMPTY(name))
	{
		xmn=stripmodulename(name);
		xfdn=stripfieldname(name);
		if(!isEMPTY(xmn) && isEMPTY(xfdn))
		{
			ret=stralloc(xmn);
		} else if(!isEMPTY(xfdn))
		{
			ret=stralloc(xfdn);
		} else {
			ret=stralloc(name);
		}
		if(xmn!=NULL) Rfree(xmn);
		if(xfdn!=NULL) Rfree(xfdn);
		if(!isEMPTY(ret))
		{
			for(x=0,xmn=ret;x<RDAstrlen(ret);++x,++xmn)
			{
				if(x==0 || last==TRUE)
				{
					*xmn=toupper(*xmn);
				} else *xmn=tolower(*xmn);
				if(*xmn==' ') last=TRUE;
					else last=FALSE;
			}
		}
	}
	return(ret);
}
void makeRangeScreen(IEfile *IMPDEF)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscreename=NULL;
	char *rscrlabel=NULL;
	int x=0;
	NRDfield *field=NULL,*fields=NULL;
	int nofields=0;
	short fileno=(-1);
	APPlib *bol_fields=NULL,*reg_fields=NULL;

	fileno=OPNNRD(IMPDEF->module,IMPDEF->file);
	if(fileno==(-1))
	{
		prterr("Error:  Unable to open file [%s][%s].",IMPDEF->module,IMPDEF->file);
		return;
	}
	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	memset(stemp,0,101);
	sprintf(stemp,"%s IMPORT/EXPORT RANGE SCREEN",IMPDEF->name);
	SCREEN=RDAscrnNEW(IMPDEF->module,stemp);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,"Filename:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,0,"FILENAME","Filename:",NULL,NULL,0,60,(IMPDEF->which==0 ? 4:7),NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(nofields>8)
	{
		if(XPERT_SETUP->software_type<2)
		{
			ADVaddwdgt(SCREEN,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		} else {
			ADVaddwdgt(SCREEN,12,"","","","",300,650,0,NULL,NULL,NULL,NULL);
		}
	}

	bol_fields=APPlibNEW();
	reg_fields=APPlibNEW();
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		if(field->type==BOOLNS)
		{
			addAPPlib(bol_fields,field->name);
		} else if(field->type!=SCROLLEDTEXT)
		{
			addAPPlib(reg_fields,field->name);
		}
	}
	SORTAPPlib(bol_fields);
	SORTAPPlib(reg_fields);
	if(bol_fields->numlibs>0)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		for(x=0;x<bol_fields->numlibs;++x)
		{
			field=FLDNRD(fileno,bol_fields->libs[x]);
			if(field!=NULL)
			{
				rscreename=ReturnScreenName(IMPDEF->file,field);
				rscrlabel=ReturnScreenLabel(field->name);
				if(!isEMPTY(rscreename))
				{
					addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(XPERT_SETUP->software_type<2)
					{
						addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					}
					temp=Rmalloc(RDAstrlen(rscreename)+14);
					temp1=Rmalloc(RDAstrlen(rscrlabel)+20);
					sprintf(temp,"SELECT %s FALSE",rscreename);
					sprintf(temp1,"Select when %s False",rscrlabel);
					addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					sprintf(temp,"SELECT %s TRUE",rscreename);
					sprintf(temp1,"Select when %s True",rscrlabel);
					addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp!=NULL) Rfree(temp);
					if(temp1!=NULL) Rfree(temp1);
					addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				if(rscreename!=NULL) Rfree(rscreename);
				if(rscrlabel!=NULL) Rfree(rscrlabel);
			}
		}
		if(XPERT_SETUP->software_type==2)
		{
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		} else if(reg_fields->numlibs==0) {
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
	if(reg_fields->numlibs>0)
	{
		if(XPERT_SETUP->software_type<2)
		{
			if(bol_fields->numlibs==0)
			{
				addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
			addwdgt(SCREEN,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
			addwdgt(SCREEN,5,"","FROM","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
			addwdgt(SCREEN,5,"","TO","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
		}
		for(x=0;x<reg_fields->numlibs;++x)
		{
			field=FLDNRD(fileno,reg_fields->libs[x]);
			if(field!=NULL)
			{
				rscreename=ReturnScreenName(IMPDEF->file,field);
				rscrlabel=ReturnScreenLabel(field->name);
				if(!isEMPTY(rscreename))
				{
					if((field->len>20) && (XPERT_SETUP->software_type==2))
					{
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp=Rmalloc(RDAstrlen(rscreename)+10);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp,"RANGE ON %s",rscreename);
						sprintf(temp1,"Range on %s",rscrlabel);
						addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,5,NULL,"From:",NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,5,NULL,"To:",NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp=Rmalloc(RDAstrlen(rscreename)+6);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+6);
						sprintf(temp,"FROM %s",rscreename);
						sprintf(temp1,"From %s",rscrlabel);
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp=Rmalloc(RDAstrlen(rscreename)+4);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+4);
						sprintf(temp,"TO %s",rscreename);
						sprintf(temp1,"To %s",rscrlabel);
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					} else {
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp=Rmalloc(RDAstrlen(rscreename)+10);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp,"RANGE ON %s",rscreename);
						sprintf(temp1,"Range on %s",rscrlabel);
						addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						if(XPERT_SETUP->software_type==2)
						{
							addwdgt(SCREEN,5,NULL,"From:",NULL,NULL,0,0,0,NULL,NULL,NULL);
						} else {
							if(field->len>34) field->len=34;
						}
						temp=Rmalloc(RDAstrlen(rscreename)+6);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+6);
						sprintf(temp,"FROM %s",rscreename);
						sprintf(temp1,"From %s",rscrlabel);
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						if(XPERT_SETUP->software_type==2)
						{
							addwdgt(SCREEN,5,NULL,"To:",NULL,NULL,0,0,0,NULL,NULL,NULL);
						}
						temp=Rmalloc(RDAstrlen(rscreename)+4);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+4);
						sprintf(temp,"TO %s",rscreename);
						sprintf(temp1,"To %s",rscrlabel);
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);

						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					}
				}
				if(rscreename!=NULL) Rfree(rscreename);
				if(rscrlabel!=NULL) Rfree(rscrlabel);
			}
		}
		if(XPERT_SETUP->software_type<2)
		{
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
	if(bol_fields!=NULL) freeapplib(bol_fields);
	if(reg_fields!=NULL) freeapplib(reg_fields);
	if(nofields>8)
	{
		ADVaddwdgt(SCREEN,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	}
	if(fileno!=(-1)) CLSNRD(fileno);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,25,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(IMPDEF->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,IMPDEF->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,IMPDEF->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
void makeDefaultMenu(char *module,char *name)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(isEMPTY(module)) return;
	sprintf(temp1,"%s %s",module,name);
	sprintf(temp2,"doie.EXT %s \"%s\"",module,name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
static void saveimp(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int selectedm=0,selectedf=0;
	char range=FALSE,ask=FALSE;
	char defmenu=FALSE,defscreen=FALSE;

	if(FINDRSCGETINT(mtnrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mtnrsrc,"FILE LIST",&selectedf)) return;
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"NAME",&IMPDEF->name);
	if(IMPDEF->name!=NULL)
	{
		IMPDEF->file=stralloc(file_defs->libs[selectedf]);
		IMPDEF->module=stralloc(modlstx->libs[selectedm]);
		FINDRSCGETSTRING(mtnrsrc,"DESCRIPTION",&IMPDEF->desc);
		FINDRSCGETSTRING(mtnrsrc,"EXPRESSION",&IMPDEF->expression);
		FINDRSCGETSTRING(mtnrsrc,"ASCII FILE",&IMPDEF->ifile);
		FINDRSCGETSTRING(mtnrsrc,"FIELD DELIMITER",&IMPDEF->fdel);
		FINDRSCGETSTRING(mtnrsrc,"RECORD DELIMITER",&IMPDEF->rdel);
		FINDRSCGETINT(mtnrsrc,"DIRECTION",&IMPDEF->which);
		FINDRSCGETINT(mtnrsrc,"TYPES",&IMPDEF->type);
		FINDRSCGETCHAR(mtnrsrc,"FORCE QUOTES",&IMPDEF->force_quotes);
		FINDRSCGETCHAR(mtnrsrc,"CARRIAGE RETURN",&IMPDEF->carriage_return);
		FINDRSCGETCHAR(mtnrsrc,"LINE FEED",&IMPDEF->line_feed);
		FINDRSCGETCHAR(mtnrsrc,"SKIP DUPLICATES",&IMPDEF->skip_dups);
		FINDRSCGETCHAR(mtnrsrc,"RANGE SCREEN",&range);
		IMPDEF->range=(short)range;
		FINDRSCGETCHAR(mtnrsrc,"ASK BEFORE EXECUTE",&ask);
		FINDRSCGETCHAR(mtnrsrc,"DEFAULT MENU",&defmenu);
		FINDRSCGETCHAR(mtnrsrc,"DEFAULT RANGE SCREEN",&defscreen);
		if(ask) IMPDEF->ask_b4_execute=TRUE;
		else IMPDEF->ask_b4_execute=FALSE;
		if(defscreen) makeRangeScreen(IMPDEF);
		if(defmenu) makeDefaultMenu(IMPDEF->module,IMPDEF->name);
		if(writeIMPORTbin(modlstx->libs[selectedm],IMPDEF))
		{
			ERRORDIALOG("Cannot Write Import/Export","Error the user cannot write the import/export definition to the library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		}
	}
	getimpentries(mainrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,2))
	{
		changedvalues=TRUE;
	}
	quitimp(mtnrsrc,mainrsrc);
}
static void setdelimiters(RDArsrc *mtnrsrc)
{
	int type=0;

	FINDRSCGETINT(mtnrsrc,"TYPES",&type);
	if(!type)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"FORCE QUOTES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"FORCE QUOTES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"FIELD DELIMITER",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"RECORD DELIMITER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"FIELD DELIMITER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"RECORD DELIMITER",FALSE);
	} else if(type==1)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"FORCE QUOTES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"FIELD DELIMITER",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"RECORD DELIMITER",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"FORCE QUOTES",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"FIELD DELIMITER",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"RECORD DELIMITER",TRUE);
	} else {
		FINDRSCSETEDITABLE(mtnrsrc,"FORCE QUOTES",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"FORCE QUOTES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"FIELD DELIMITER",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"RECORD DELIMITER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"FIELD DELIMITER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"RECORD DELIMITER",FALSE);
	}
}
static void setrangescreen(RDArsrc *mtnrsrc)
{
	char rangescrn=FALSE,*name=NULL,*rsname=NULL;

	readwidget(mtnrsrc,"RANGE SCREEN");
	FINDRSCGETCHAR(mtnrsrc,"RANGE SCREEN",&rangescrn);
	readwidget(mtnrsrc,"NAME");
	FINDRSCGETSTRING(mtnrsrc,"NAME",&name);
	if(rangescrn)
	{
		rsname=Rmalloc(RDAstrlen(name)+29);
		sprintf(rsname,"%s IMPORT/EXPORT RANGE SCREEN",name);
	}
	FINDRSCSETSENSITIVE(mtnrsrc,"RANGE SCREEN NAME",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"RANGE SCREEN NAME",rsname);
	updatersrc(mtnrsrc,"RANGE SCREEN NAME");
	if(rsname!=NULL) 
	{
		Rfree(rsname);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"RANGE SCREEN NAME",FALSE);
	}
}
static void recal_offsets(RDArsrc *mtnrsrc)
{
	int x=0,y=0,selected=0; 
	NRDfield *nrdfield=NULL;
	IEfield *field=NULL,*field2=NULL;
	int start_offset=0,add_offset=0;
	int lastfieldlength=0;
	char *impexpname=NULL,*modname=NULL,*filename=NULL,*warnmessage=NULL;

	if(FINDRSCGETINT(mtnrsrc,"FIELD DEFINITIONS",&selected)) return;
	if(flddefs->numlibs==1 && !RDAstrcmp(flddefs->libs[0],"No Fields Defined")) return;
	readwidget(mtnrsrc,"START OFFSET");
	if(FINDRSCGETINT(mtnrsrc,"START OFFSET",&start_offset)) return;
	readwidget(mtnrsrc,"ADDITIONAL OFFSET AMOUNT");
	if(FINDRSCGETINT(mtnrsrc,"ADDITIONAL OFFSET AMOUNT",&add_offset)) return;
	if(holdfields!=NULL)
	{
	if(IMPDEF!=NULL)
	{
		impexpname=stralloc(IMPDEF->name);
		modname=stralloc(IMPDEF->module);
		filename=stralloc(IMPDEF->file);
		if(IMPDEF->fields!=NULL)
		{
		for(x=selected,field=IMPDEF->fields+selected;x<IMPDEF->num;++x,++field)
		{
			for(y=0,nrdfield=holdfields;y<numfields;++y,++nrdfield)
			{
				if(!RDAstrcmp(nrdfield->name,field->name)) break;
			}
			if(y<numfields)
			{
				if(x==selected)
				{
					if(start_offset<2) start_offset=1;
					field->start=start_offset;
					if(nrdfield->len>0)
					{
						field->length=nrdfield->len;
						lastfieldlength=nrdfield->len;
					} else {
						lastfieldlength=field->length;
					}
				} else {
					field2=IMPDEF->fields+(x-1);
					field->start=field2->start+lastfieldlength+add_offset;
					if(nrdfield->len>0)
					{
						field->length=nrdfield->len;
						lastfieldlength=nrdfield->len;
					} else {
						lastfieldlength=field->length;
					}
				}
			} else {
				warnmessage=Rmalloc(RDAstrlen(field->name)+RDAstrlen(impexpname)+RDAstrlen(modname)+RDAstrlen(filename)+270);
				sprintf(warnmessage,"While recalculating the offsets and lengths for the Import/Export Definition [%s] the field [%s] at Field Definitions line number [%d] was not found in the file defintion [%s][%s].",
					(impexpname==NULL?"":impexpname),
					(field->name==NULL?"":field->name),
					x,(modname==NULL?"":modname),
					(filename==NULL?"":filename));
				WARNINGDIALOG("WARNING DIALOG SCREEN","FIELD NOT IN FILE!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage)
			}
		}
		makefldlist1();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"FIELD DEFINITIONS",selected,flddefs))
			updatersrc(mtnrsrc,"FIELD DEFINITIONS");
		}
	}
	}
}
static void auto_buildflds(RDArsrc *mtnrsrc)
{
	int x=0,y=0,selected=0; 
	NRDfield *nrdfield=NULL;
	IEfield *field=NULL;
	int start_offset=0,add_offset=0;
	int lastfieldstart=0,lastfieldlength=0;
	char force_quotes=FALSE;

	readwidget(mtnrsrc,"START OFFSET");
	if(FINDRSCGETINT(mtnrsrc,"START OFFSET",&start_offset)) return;
	readwidget(mtnrsrc,"ADDITIONAL OFFSET AMOUNT");
	if(FINDRSCGETINT(mtnrsrc,"ADDITIONAL OFFSET AMOUNT",&add_offset)) return;
	readwidget(mtnrsrc,"FORCE QUOTES");
	if(FINDRSCGETCHAR(mtnrsrc,"FORCE QUOTES",&force_quotes)) return;
	if(holdfields!=NULL)
	{
		if(IMPDEF!=NULL)
		{
			if(IMPDEF->fields!=NULL)
			{
				for(x=0,field=IMPDEF->fields;x<IMPDEF->num;++x,++field)
				{
					if(field->name!=NULL) Rfree(field->name);
					if(field->expression!=NULL) Rfree(field->expression);
					if(field->value_expression!=NULL) Rfree(field->value_expression);
				}
				Rfree(IMPDEF->fields);
				IMPDEF->fields=NULL;
			}
		}
		IMPDEF->num=0;
		IMPDEF->fields=Rmalloc(numfields*sizeof(IEfield));
		IMPDEF->num=numfields;
		field=IMPDEF->fields;
		for(y=0,nrdfield=holdfields;y<numfields;++y,++nrdfield)
		{
			field->name=stralloc(nrdfield->name);
			field->type=nrdfield->type;
			field->expression=NULL;
			field->use_quotes=(force_quotes==TRUE?TRUE:FALSE);
			field->value_expression=NULL;
			if(y==0)
			{
				if(start_offset<2) start_offset=1;
				field->start=start_offset;
			} else {
				field->start=lastfieldstart+lastfieldlength+add_offset;
			}
			field->length=nrdfield->len;
			lastfieldstart=field->start;
			lastfieldlength=field->length;
			++field;
		}
		makefldlist1();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"FIELD DEFINITIONS",selected,flddefs))
			updatersrc(mtnrsrc,"FIELD DEFINITIONS");
	}
}
static void setquotes(RDArsrc *mtnrsrc)
{
	int x=0;
	IEfield *f;
	char force_quotes=FALSE;
	
	if(IMPDEF->fields!=NULL)
	{
		readwidget(mtnrsrc,"FORCE QUOTES");
		FINDRSCGETCHAR(mtnrsrc,"FORCE QUOTES",&force_quotes);
		if(force_quotes)
		{
			for(x=0,f=IMPDEF->fields;x<IMPDEF->num;++x,++f)
			{
				f->use_quotes=TRUE;
			}
		}
	}
}
static void dfscn(RDArsrc *r,RDArsrc *mainrsrc)
{
	APPlib *args=NULL;
	char *s=NULL;
	int l=0;
	
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&l)) return;
	args=APPlibNEW();
	addAPPlib(args,modlstx->libs[l]);
	readwidget(r,"RANGE SCREEN NAME");
	FINDRSCGETSTRING(r,"RANGE SCREEN NAME",&s);
	if(!isEMPTY(s)) addAPPlib(args,s);
	if(s!=NULL) Rfree(s);
	ExecuteProgram("dfscn",args);
	if(args!=NULL) freeapplib(args);
}
static void dfmenu(RDArsrc *r,RDArsrc *mainrsrc)
{
	APPlib *args=NULL;
	char *s=NULL;
	int l=0;
	
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&l)) return;
	args=APPlibNEW();
	addAPPlib(args,modlstx->libs[l]);
	readwidget(r,"NAME");
	FINDRSCGETSTRING(r,"NAME",&s);
	if(!isEMPTY(s)) 
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s %s",modlstx->libs[l],s);
		addAPPlib(args,stemp);
	}
	if(s!=NULL) Rfree(s);
	ExecuteProgram("dfmenu",args);
	if(args!=NULL) freeapplib(args);
}
void importdefinition1(RDArsrc *mainrsrc,char *defname,char *modx,char *filex,
	char *ifile,char *desc,char *fdel,char *rdel,int which,int type,
	short range,short ask_b4_exe,char force_quotes,char *expression,
	char cret,char lfeed,char skipdups)
{
	RDArsrc *mtnrsrc=NULL;
	int x;
	char rangescrn=FALSE,askb4exe=FALSE,*rsname=NULL;
	char defit=FALSE;
	int add_offset=0,start_offset=0;
	if(qtypes!=NULL) freeapplib(qtypes);
	qtypes=APPlibNEW();
	addAPPlib(qtypes,"Normal (ASCII)");
	addAPPlib(qtypes,"Quoted Field");
	addAPPlib(qtypes,"Encryped Sign");
	addAPPlib(qtypes,"EBCDIC Protocol");
	addAPPlib(qtypes,"Zoned  Field (0)");
	addAPPlib(qtypes,"Packed Field (0)");
	addAPPlib(qtypes,"Packed Field (1)");
	addAPPlib(qtypes,"Packed Field (2)");
	addAPPlib(qtypes,"Packed Field (3)");
	addAPPlib(qtypes,"Packed Field (4)");
	addAPPlib(qtypes,"Zoned  Field (1)");
	addAPPlib(qtypes,"Zoned  Field (2)");
	addAPPlib(qtypes,"Zoned  Field (3)");
	addAPPlib(qtypes,"Zoned  Field (4)");
	addAPPlib(qtypes,"Quoted with $999.99 or No Zero Fill");
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x)
	{
		addAPPlib(ftypes,standardfieldtypes[x]);
	}
	mtnrsrc=RDArsrcNEW(module,"MAINTAIN IMPORT EXPORT DEFINITION");
	if(flddefs!=NULL) freeapplib(flddefs);
	flddefs=APPlibNEW();
	addstdrsrc(mtnrsrc,"NAME",PLAINTEXT,15,defname,TRUE);
	addstdrsrc(mtnrsrc,"ASCII FILE",VARIABLETEXT,0,ifile,TRUE);
	addstdrsrc(mtnrsrc,"FIELD DELIMITER",VARIABLETEXT,0,fdel,TRUE);
	addstdrsrc(mtnrsrc,"RECORD DELIMITER",VARIABLETEXT,0,rdel,TRUE);
	if(ietypes!=NULL) freeapplib(ietypes);
	ietypes=APPlibNEW();
	addAPPlib(ietypes,"Import to Datafile");
	addAPPlib(ietypes,"Export from Datafile");
	addlstrsrc(mtnrsrc,"DIRECTION",&which,TRUE,NULL,ietypes->numlibs,
		&ietypes->libs,NULL);
	if(rtypes!=NULL) freeapplib(rtypes);
	rtypes=APPlibNEW();
	addAPPlib(rtypes,"Flat");
	addAPPlib(rtypes,"Field Record Delimited");
	addAPPlib(rtypes,"XML");
	addlstrsrc(mtnrsrc,"TYPES",&type,TRUE,setdelimiters,rtypes->numlibs,
		&rtypes->libs,NULL);
	if(range) rangescrn=TRUE;
		else rangescrn=FALSE;
	addstdrsrc(mtnrsrc,"RANGE SCREEN",BOOLNS,1,&rangescrn,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT RANGE SCREEN",BOOLNS,1,&defit,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT MENU",BOOLNS,1,&defit,TRUE);
	if(rangescrn)
	{
		rsname=Rmalloc(RDAstrlen(defname)+29);
		sprintf(rsname,"%s IMPORT/EXPORT RANGE SCREEN",defname);
	}
	addstdrsrc(mtnrsrc,"RANGE SCREEN NAME",VARIABLETEXT,0,(rsname!=NULL ? rsname:""),FALSE);
	if(rsname!=NULL) 
	{
		Rfree(rsname);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"RANGE SCREEN NAME",FALSE);
	}
	FINDRSCSETFUNC(mtnrsrc,"RANGE SCREEN",setrangescreen,NULL);
	if(ask_b4_exe) askb4exe=TRUE;
		else askb4exe=FALSE;
	addstdrsrc(mtnrsrc,"ASK BEFORE EXECUTE",BOOLNS,1,&askb4exe,TRUE);
	addstdrsrc(mtnrsrc,"FORCE QUOTES",BOOLNS,1,&force_quotes,(type ? TRUE:FALSE));
	FINDRSCSETFUNC(mtnrsrc,"FORCE QUOTES",setquotes,NULL);
	addstdrsrc(mtnrsrc,"CARRIAGE RETURN",BOOLNS,1,&cret,TRUE);
	addstdrsrc(mtnrsrc,"LINE FEED",BOOLNS,1,&lfeed,TRUE);
	addstdrsrc(mtnrsrc,"SKIP DUPLICATES",BOOLNS,1,&skipdups,TRUE);
	addsctrsrc(mtnrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:NULL),TRUE);
	for(x=0;x<modlstx->numlibs;++x) 
	{
		if(!RDAstrcmp(modlstx->libs[x],modx)) break;
	}
	if(x>=modlstx->numlibs) x=0;
	addlstrsrc(mtnrsrc,"MODULE LIST",&x,TRUE,dofilelist,
		modlstx->numlibs,&modlstx->libs,NULL);
	if(file_defs!=NULL) freeapplib(file_defs);
	file_defs=APPlibNEW();
	x=0;
	addlstrsrc(mtnrsrc,"FILE LIST",&x,TRUE,setselectfile,file_defs->numlibs,
		&file_defs->libs,NULL);
	dofilelist(mtnrsrc);
	for(x=0;x<file_defs->numlibs;++x)
	{
		if(!RDAstrcmp(file_defs->libs[x],filex)) break;
	}
	if(x>=file_defs->numlibs) x=0;
	FINDRSCLISTAPPlib(mtnrsrc,"FILE LIST",x,file_defs);
	addbtnrsrc(mtnrsrc,"AUTO BUILD FIELDS",TRUE,auto_buildflds,NULL);
	addbtnrsrc(mtnrsrc,"RE-CALCULATE OFFSETS",TRUE,recal_offsets,NULL);
	addstdrsrc(mtnrsrc,"ADDITIONAL OFFSET AMOUNT",LONGV,5,&add_offset,TRUE);
	addstdrsrc(mtnrsrc,"START OFFSET",LONGV,5,&start_offset,TRUE);
	if(flddefs!=NULL) freeapplib(flddefs);
	flddefs=APPlibNEW();
	addlstrsrc(mtnrsrc,"FIELD DEFINITIONS",0,TRUE,NULL,flddefs->numlibs,
		&flddefs->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD ABOVE",TRUE,add_above,NULL);
	addbtnrsrc(mtnrsrc,"ADD BELOW",TRUE,add_below,NULL);
	addbtnrsrc(mtnrsrc,"DELETE",TRUE,deletefld1,NULL);
	addbtnrsrc(mtnrsrc,"SELECT",TRUE,editfld,NULL);
	addsctrsrc(mtnrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mtnrsrc,"DFSCN",TRUE,dfscn,mainrsrc);
	addbtnrsrc(mtnrsrc,"DFMENU",TRUE,dfmenu,mainrsrc);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,saveimp,mainrsrc);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quitimptest,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT DEFINITION",TRUE,printdefinitioncb,IMPDEF);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setselectfile(mtnrsrc);
	setdelimiters(mtnrsrc);
	makefldlist1();
	FINDRSCLISTAPPlib(mtnrsrc,"FIELD DEFINITIONS",0,flddefs);
	changedvalues=FALSE;
	mtndefaults=GetDefaults(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitimp,mainrsrc,FALSE);
}
void setvaluesx(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	if(RDAstrcmp(defsavl->libs[selected],"Contains No Definitions"))
	{
		temp=getIMPORTdesc(modlstx->libs[selectedm],defsavl->libs[selected]);
	} else temp=NULL;
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
}
void selectdef1(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(RDAstrcmp(defsavl->libs[selected],"Contains No Definitions"))
	{
		IMPDEF=IMPORTstpNEW(defsavl->libs[selected]);
	} else IMPDEF=IMPORTstpNEW(NULL);
	if(IMPDEF->name!=NULL)
	{
		if(getIMPORTbin(modlstx->libs[selectedm],IMPDEF))
		{
			ERRORDIALOG("Cannot Read Import/Export","Error in permissions, user cannot read the import/export binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Error Import Definition [%s] not readable.",defsavl->libs[selected]);
			if(IMPDEF!=NULL) free_import(IMPDEF);
			return;
		}
	}
	importdefinition1(mainrsrc,IMPDEF->name,IMPDEF->module,IMPDEF->file,
		IMPDEF->ifile,IMPDEF->desc,IMPDEF->fdel,IMPDEF->rdel,
		IMPDEF->which,IMPDEF->type,IMPDEF->range,IMPDEF->ask_b4_execute,
		IMPDEF->force_quotes,IMPDEF->expression,IMPDEF->carriage_return,
		IMPDEF->line_feed,IMPDEF->skip_dups);
}
static void newdef1(RDArsrc *mainrsrc)
{
	int selectedm=0;

	IMPDEF=IMPORTstpNEW(NULL);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	importdefinition1(mainrsrc,IMPDEF->name,modlstx->libs[selectedm],
		NULL,NULL,NULL,",","\r\n",1,1,FALSE,FALSE,TRUE,NULL,
		FALSE,FALSE,FALSE);
}
static void deletedef1(RDArsrc *mainrsrc)
{
	int s=0,m=0;
	char *libx=NULL,*modulename=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&s)) return;
	modulename=adddashes(modlstx->libs[m]);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.IMP",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.IMP",CURRENTDIRECTORY,modulename);
#endif
	if(deletelibbin(libx,defsavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Import/Export","Error the user cannot delete the import/export definition.  Check the permissions on the binary library, and retry.  Call your installer.",NULL,FALSE);
	} else getimpentries(mainrsrc);
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
}
static void deletedeftest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(defsavl->libs[s],"Contains No Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE WARNING","Are you sure you want to delete this import/export definition",deletedef1,NULL,FALSE,mainrsrc);
}
static void execdef1(RDArsrc *mainrsrc)
{
	int selectedm=0,selectedd=0;
	APPlib *args=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selectedd)) return;
	if(RDAstrcmp(defsavl->libs[selectedd],"Contains No Definitions"))
	{
		args=APPlibNEW();
		addAPPlib(args,modlstx->libs[selectedm]);
		addAPPlib(args,defsavl->libs[selectedd]);
		ExecuteProgram("doie",args);
		if(args!=NULL) freeapplib(args);
	}
}
static void quitdfiex(RDArsrc *r)
{
	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=NULL;
	if(defsavl!=NULL) freeapplib(defsavl);
	defsavl=NULL;
	if(r!=NULL) free_rsrc(r);
	r=NULL;
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int m=0,s=0;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE IMP EXP")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"DEFINE IMPORT EXPORT");
	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&m,TRUE,getimpentries,
		modlstx->numlibs,&modlstx->libs,NULL);
	getdefinitionlist1(mainrsrc);
	if(defsavl!=NULL) freeapplib(defsavl);
	defsavl=APPlibNEW();
	addlstrsrc(mainrsrc,"DEFINITIONS AVAILABLE",&s,TRUE,setvaluesx,
		defsavl->numlibs,&defsavl->libs,NULL);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	getimpentries(mainrsrc);
	if(argc>1 && modlstx!=NULL)
	{
		for(m=0;m<modlstx->numlibs;++m)
		{
			if(!RDAstrcmp(modlstx->libs[m],argv[1])) break;
		}
		if(m<modlstx->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"MODULE LIST",m);
		}
		else
		{
			m=0;
			FINDRSCSETINT(mainrsrc,"MODULE LIST",m);
		}
		getimpentries(mainrsrc);
	}
	if(argc>2 && defsavl!=NULL)
	{
		for(s=0;s<defsavl->numlibs;++s)
		{
			if(!RDAstrcmp(defsavl->libs[s],argv[2])) break;
		}
		if(s<defsavl->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"DEFINITIONS AVAILABLE",s);
		}
		else
		{
			s=0;
			FINDRSCSETINT(mainrsrc,"DEFINITIONS AVAILABLE",s);
		}
	}
	addbtnrsrc(mainrsrc,"ADD",TRUE,newdef1,NULL);
	addbtnrsrc(mainrsrc,"EXECUTE",TRUE,execdef1,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectdef1,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfiex,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletedeftest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setvaluesx(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitdfiex,NULL,TRUE);
}
