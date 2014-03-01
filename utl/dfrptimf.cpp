/* dfrptimf.c - Define Output to File Section of Define Reports */
#include <cstdio>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <fin.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

extern RDAreport *report;
extern char *module;
extern void makefieldvallistrpt(APPlib **,short,short,short,short);
extern char changedreport;
static RDAimage *holdimage=NULL;
static APPlib *imgavl=NULL,*imgpval=NULL;
static APPlib *brkavl=NULL,*fldnames=NULL,*varnames=NULL;
static APPlib *symboltypes=NULL;
static int holdnum=0,holdpnum=0;
static RDApval *holdpval=NULL;
static RDAdefault *imgdefaults=NULL,*mimgdefaults=NULL,*editdefaults=NULL;
static void changeSYMBOLtype(RDArsrc *);
static void copyline(RDArsrc *);
static char changedimages=FALSE;
APPlib *makefieldvallist1(RDArsrc *,short,short,short,short);

APPlib *makefieldvallist1(RDArsrc *mimgrsrc,short dofilefields,short doglobals,short virtuals,short accums)
{
	APPlib *tmp=NULL;
	int x,y,z,j;
	RDAvirtual *v;
	RDAsearch *s;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDAaccum *a;
	RDAdisplay *d;
	RDAimage *i=NULL;
	char *modx=NULL,*filx=NULL,*temp1,*temp=NULL;

	tmp=APPlibNEW();
	if(report->virflds!=NULL && virtuals)
	{
		for(x=0,v=report->virflds;x<report->numvirtuals;++x,++v)
		{
			if(v->name!=NULL)
			{
				if(temp!=NULL)
				{
					temp=Rrealloc(temp,RDAstrlen(v->name)+3);
				} else {
					temp=Rmalloc(RDAstrlen(v->name)+3);
				}
				sprintf(temp,"[%s]",v->name);
				addAPPlibNoDuplicates(tmp,temp);
			}
		}
	}
	if(accums)
	{
		addAPPlibNoDuplicates(tmp,"[RECORD COUNTER]");
	}
	if(report->accum!=NULL && accums)
	{
		for(x=0,a=report->accum;x<report->numaccums;++x,++a)
		{
			if(a->name!=NULL)
			{
				if(temp!=NULL)
				{
					temp=Rrealloc(temp,RDAstrlen(a->name)+3);
				} else {
					temp=Rmalloc(RDAstrlen(a->name)+3);
				}
				sprintf(temp,"[%s]",a->name);
				addAPPlibNoDuplicates(tmp,temp);
			}
		}
	}
	if(doglobals)
	{
		d=report->display;
		if(d!=NULL && d->device>2) addAPPlib(tmp,"NEXT TRANSACTION NO");
		addAPPlibNoDuplicates(tmp,"[PAGE NO]");
		addAPPlibNoDuplicates(tmp,"[BREAK LEVEL]");
		addAPPlibNoDuplicates(tmp,"[PREVIOUS BREAK LEVEL]");
		addAPPlibNoDuplicates(tmp,"[PREVIOUS BREAK LEVEL 2]");
		addAPPlibNoDuplicates(tmp,"[BODY COUNT]");
		loadglobals(tmp);
	}
	if(dofilefields)
	{
		for(x=0,s=report->search;x<report->numsearchs;++x,++s)
		{
			if(!GETBIN(s->module,s->filename,&engine,&nofields,&fields,
				&nokeys,&keys)) 
			{
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(s->module)+RDAstrlen(s->filename)+7);
						} else {
							temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(s->module)+RDAstrlen(s->filename)+7);
						}
						sprintf(temp,"[%s][%s][%s]",s->module,s->filename,f->name);
						addAPPlibNoDuplicates(tmp,temp);
						if(f->name!=NULL) Rfree(f->name);
					}
					Rfree(fields);
				}
				if(keys!=NULL)
				{
					for(y=0,keyx=keys;y<nokeys;++y,++keyx)
					{
						if(keyx->name!=NULL) Rfree(keyx->name);
						if(keyx->part!=NULL)
						{
							for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
							Rfree(keyx->part);
						}
					}
					Rfree(keys);
				}
			}
		}
	}
	if(holdimage!=NULL)
	{
		for(j=0,i=holdimage;j<holdnum;++j,++i)
		{
			if(!strncmp(i->line,"OPEN",4))
			{
				temp1=i->line+5;
				while(iswhite(*temp1)) ++temp1;
				modx=stripmodulename(temp1);
				filx=stripfilename(temp1);
				if(!isEMPTY(modx) && !isEMPTY(filx))
				{
					if(!GETBIN(modx,filx,&engine,&nofields,&fields,&nokeys,&keys)) 
					{
						if(fields!=NULL)
						{
							for(y=0,f=fields;y<nofields;++y,++f)
							{
								if(temp!=NULL)
								{
									temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
								} else {
									temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
								}
								sprintf(temp,"[%s][%s][%s]",modx,filx,f->name);
								addAPPlibNoDuplicates(tmp,temp);
								if(f->name!=NULL) Rfree(f->name);
							}
							Rfree(fields);
						}
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
									Rfree(keyx->part);
								}
							}
							Rfree(keys);
						}
					}
				} else {
					prterr("Error Module [%s] File [%s] names in error cannot display fields.",(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	SORTAPPlib(tmp);
	return(tmp);
}
void loadvalue1(RDArsrc *r,RDArsrc *mimgrsrc)
{
	int x;
	LoadValue *load;

	load=Rmalloc(sizeof(LoadValue));
	ldvalrsrc=RDArsrcNEW(module,"LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,"EXPRESSION");
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist1(mimgrsrc,TRUE,TRUE,TRUE,TRUE);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	} else {
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=loadlist->numlibs) x=0;
	}
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void getimgvalues()
{
	int x,len=0;
	RDApval *p;
	char *tmp=NULL;

	if(imgpval!=NULL) freeapplib(imgpval);
	imgpval=APPlibNEW();
	if(holdpval!=NULL)
	{
		for(x=0,p=holdpval;x<holdpnum;++x,++p)
		{
			len=(5+RDAstrlen(p->name)+RDAstrlen(p->fieldname)+16+1);
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"[%5d] Copy [%s] to [%s]",
				(x+1),
				(p->name!=NULL ? p->name:""),
				(p->fieldname!=NULL ? p->fieldname:""));
			addAPPlib(imgpval,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(imgpval->numlibs<1)
	{
		addAPPlib(imgpval,"Currently there are no image values defined file this report.");
	}
}
static void makespaceimgpval(int pos)
{
	short x,y;
	RDApval *temp,*temp1,*new_fields;

	++holdpnum;
	new_fields=Rmalloc(holdpnum*sizeof(RDApval));
	y=0;
	for(x=0,temp=holdpval;x<(holdpnum-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->symbol_type=0;
			temp1->name=NULL;
			temp1->type=0;
			temp1->fieldname=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->symbol_type=temp->symbol_type;
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
		++y;
	}
	Rfree(holdpval);
	holdpval=Rmalloc(holdpnum*sizeof(RDApval));
	for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->symbol_type=temp->symbol_type;
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
	}
	Rfree(new_fields);
}
static void quitline(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	if(editrsrc!=NULL)
	{
		killwindow(editrsrc);
		free_rsrc(editrsrc);
	}
	if(varnames!=NULL) freeapplib(varnames);
	if(fldnames!=NULL) freeapplib(fldnames);
	if(symboltypes!=NULL) freeapplib(symboltypes);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
}
static void shuffleimages()
{
/*
	int x,y;
	RDApval *pval1,*pval2;
	RDAimage *i1,*i2;

	for(x=1,i1=holdimage;x<a->numlibs;++x,++i1) 
	{
		for(y=(a->numlibs-1);y>=x;--y)
		{
			if(RDAstrcmp(a->libs[y],a->libs[y-1])<0)
			{
				hold=a->libs[y];
				a->libs[y]=a->libs[y-1];
				a->libs[y-1]=hold;
			}
		}
	} 
*/
}
static void saveline(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int value=0;
	int x=0;
	RDApval *p;
	char *linex=NULL;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&value)) return;
	if(holdpval==NULL) 
	{
		holdpval=Rmalloc(sizeof(RDApval));
		holdpnum=1;
	}
	p=holdpval+value;
	if(p==NULL) 
	{
		p=Rmalloc(sizeof(RDApval));
	}
	readallwidgets(editrsrc);
	x=0;
	FINDRSCGETINT(editrsrc,"SYMBOL TYPES",&x);
	p->symbol_type=(short)x;
	x=0;
	if(!p->symbol_type)
	{
		FINDRSCGETINT(editrsrc,"VARIABLE NAMES",&x);
		if(p->name!=NULL && RDAstrlen(p->name)>0) Rfree(p->name);
		p->name=stralloc(varnames->libs[x]);
	} else {
		if(p->name!=NULL && RDAstrlen(p->name)>0) Rfree(p->name);
		FINDRSCGETSTRING(editrsrc,"EXPRESSION",&p->name);
	}
	FINDRSCGETINT(editrsrc,"FIELD NAMES",&x);
	if(p->fieldname!=NULL) Rfree(p->fieldname);
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&linex)) return;
	if(!RDAstrcmp(linex,"POST FIELD") || !strncmp(linex,"PUTENV",6) || !strncmp(linex,"HTTP",4))
	{
		FINDRSCGETSTRING(editrsrc,"FIELD NAME",&p->fieldname);
	} else {
		p->fieldname=stralloc(fldnames->libs[x]);
	}
	if(linex!=NULL) Rfree(linex);
	shuffleimages();
	getimgvalues();
	if(!FINDRSCLISTAPPlib(mimgrsrc,"IMAGE VALUES",value,imgpval))
		updatersrc(mimgrsrc,"IMAGE VALUES");
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedimages=TRUE;
	}
	quitline(editrsrc,mimgrsrc);
}
static void savelineabove(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&x)) return;
	makespaceimgpval(x);
	saveline(editrsrc,mimgrsrc);
}
static void savelinebelow(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x;

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&x)) return;
	makespaceimgpval(++x);
	FINDRSCSETINT(mimgrsrc,"IMAGE VALUES",x);
	saveline(editrsrc,mimgrsrc);
}
static void quitlinetest(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE VARIABLE!\nDo you want to Save these changes?",saveline,quitline,FALSE,2,editrsrc,mimgrsrc,NULL);
	} else {
		quitline(editrsrc,mimgrsrc);
	}
}
static void quitlineabovetest(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE VARIABLE!\nDo you want to Save these changes?",savelineabove,quitline,FALSE,2,editrsrc,mimgrsrc,NULL);
	} else {
		quitline(editrsrc,mimgrsrc);
	}
}
static void quitlinebelowtest(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE VARIABLE!\nDo you want to Save these changes?",savelinebelow,quitline,FALSE,2,editrsrc,mimgrsrc,NULL);
	} else {
		quitline(editrsrc,mimgrsrc);
	}
}
static void chgFIELDS(RDArsrc *editrsrc,RDArsrc *mimgrsrc)
{
	int x=0;
	char *linex=NULL;

	FINDRSCGETINT(editrsrc,"FIELD NAMES",&x);
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&linex)) return;
	if(!RDAstrcmp(linex,"POST FIELD") || !strncmp(linex,"HTTP",4))
	{
		FINDRSCSETEDITABLE(editrsrc,"FIELD NAME",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"FIELD NAME",TRUE);
		FINDRSCSETEDITABLE(editrsrc,"VARIABLE NAMES",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",FALSE);
		updatersrc(editrsrc,"FIELD NAME");
		updatersrc(editrsrc,"VARIABLE NAMES");
	} else if(!RDAstrcmp(linex,"PUTENV"))
	{
		FINDRSCSETEDITABLE(editrsrc,"FIELD NAME",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"FIELD NAME",TRUE);
		FINDRSCSETEDITABLE(editrsrc,"VARIABLE NAMES",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",TRUE);
		updatersrc(editrsrc,"FIELD NAME");
		updatersrc(editrsrc,"VARIABLE NAMES");
	} else {
		FINDRSCSETEDITABLE(editrsrc,"FIELD NAME",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"FIELD NAME",FALSE);
		FINDRSCSETEDITABLE(editrsrc,"VARIABLE NAMES",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",TRUE);
		updatersrc(editrsrc,"FIELD NAME");
		updatersrc(editrsrc,"VARIABLE NAMES");
	}	
	if(linex!=NULL) Rfree(linex);
}
static char *evaltypes[2]={"Copy Variable to Field","Evaluate Expression"};
static void linescreen(RDArsrc *mimgrsrc,int num,char *varname,char *fldname,
	int type,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x,y,z;
	char *modx=NULL,*filx=NULL,*temp1,*linex=NULL,*temp=NULL,*expression=NULL;
	char *fieldname=NULL,*fldname1=NULL;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	short engine=0,nofields=0,nokeys=0;
	RDArsrc *editrsrc=NULL;

	readwidget(mimgrsrc,"LINE");
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&linex)) return;
	if(symboltypes!=NULL) freeapplib(symboltypes);
	symboltypes=APPlibNEW();
	addAPPlib(symboltypes,evaltypes[0]);
	addAPPlib(symboltypes,evaltypes[1]);
	if(fldnames!=NULL) freeapplib(fldnames);
	fldnames=APPlibNEW();
	fldname1=stralloc(fldname);
	if(!strncmp(&linex[0],"WRITE_GENERIC_COUNTER",21) || 
		!strncmp(&linex[0],"write_generic_counter",21)) /* write generic Counter */
	{
		addAPPlib(fldnames,"FILENAME");
		addAPPlib(fldnames,"GENERIC COUNTER");
	} else if(!strncmp(&linex[0],"RMDIR",5) ||
		!strncmp(&linex[0],"rmdir",5))
	{
		addAPPlib(fldnames,"DIRECTORY NAME");
	} else if(!strncmp(&linex[0],"MKDIR",5) ||
		!strncmp(&linex[0],"mkdir",5))
	{
		addAPPlib(fldnames,"DIRECTORY NAME");
	} else if(!strncmp(&linex[0],"REMDEL",6) ||
		!strncmp(&linex[0],"remdel",6))
	{
		addAPPlib(fldnames,"MODULE");
		addAPPlib(fldnames,"FILENAME");
	} else if(!strncmp(&linex[0],"UNLINK",6) ||
		!strncmp(&linex[0],"unlink",6))
	{
		addAPPlib(fldnames,"FILENAME");
	} else if(!strncmp(&linex[0],"DOREPORT",8) ||
		!strncmp(&linex[0],"doreport",8))
	{
		addAPPlib(fldnames,"MODULE NAME");
		addAPPlib(fldnames,"REPORT NAME");
	} else if(!strncmp(&linex[0],"WWWDOADD",8) ||
		!strncmp(&linex[0],"wwwdoadd",8))
	{
		addAPPlib(fldnames,"MODULE NAME");
		addAPPlib(fldnames,"POWERADD NAME");
	} else if(!strncmp(&linex[0],"COPYFILE",8) ||
		!strncmp(&linex[0],"copyfile",8))
	{
		addAPPlib(fldnames,"FROM FILENAME");
		addAPPlib(fldnames,"TO FILENAME");
	} else if(!strncmp(&linex[0],"DISPLAYRELATIVEFILE",19) ||
		!strncmp(&linex[0],"displayrelativefile",19))
	{
		addAPPlib(fldnames,"FILENAME");
	} else if(!strncmp(&linex[0],"DISPLAYFILE",11) ||
		!strncmp(&linex[0],"displayfile",11))
	{
		addAPPlib(fldnames,"FILENAME");
	} else if(!strncmp(&linex[0],"MOVEFILE",8) ||
		!strncmp(&linex[0],"movefile",8))
	{
		addAPPlib(fldnames,"FROM FILENAME");
		addAPPlib(fldnames,"TO FILENAME");
	} else if(!strncmp(&linex[0],"SENDATTACH-3",12) ||
		!strncmp(&linex[0],"sendattach-3",12))
	{
		addAPPlib(fldnames,"NAME OF SENDER");
		addAPPlib(fldnames,"SENDER EMAIL ADDRESS");
		addAPPlib(fldnames,"SUBJECT");
		addAPPlib(fldnames,"BODY");
		addAPPlib(fldnames,"RECIPIENT EMAIL ADDRESS");
		addAPPlib(fldnames,"FILENAMES TO BE ATTACHED");
		addAPPlib(fldnames,"EMAIL ATTACHMENTS TO BE NAMED");
	} else if(!strncmp(&linex[0],"SENDATTACH-1",12) ||
		!strncmp(&linex[0],"sendattach-1",12))
	{
		addAPPlib(fldnames,"NAME OF SENDER");
		addAPPlib(fldnames,"SENDER EMAIL ADDRESS");
		addAPPlib(fldnames,"SUBJECT");
		addAPPlib(fldnames,"BODY");
		addAPPlib(fldnames,"RECIPIENT EMAIL ADDRESS");
		addAPPlib(fldnames,"FILENAME TO BE ATTACHED");
		addAPPlib(fldnames,"EMAIL ATTACHMENT TO BE NAMED");
	} else if(!strncmp(&linex[0],"EMAILUSERINFO",13) ||
		!strncmp(&linex[0],"emailuserinfo",13))
	{
		addAPPlib(fldnames,"USER IDENTIFICATION");
		addAPPlib(fldnames,"EMAIL ADDRESS");
		addAPPlib(fldnames,"FROM USER NAME");
		addAPPlib(fldnames,"FROM USER EMAIL");
	} else if(!strncmp(&linex[0],"SENDMAIL",8) ||
		!strncmp(&linex[0],"sendmail",8))
	{
		addAPPlib(fldnames,"NAME OF SENDER");
		addAPPlib(fldnames,"SENDER EMAIL ADDRESS");
		addAPPlib(fldnames,"SUBJECT");
		addAPPlib(fldnames,"BODY");
		addAPPlib(fldnames,"RECIPIENT EMAIL ADDRESS");
	} else if(!strncmp(&linex[0],"SEND EMAIL",10) ||
		!strncmp(&linex[0],"send email",10))
	{
		addAPPlib(fldnames,"NAME OF SENDER");
		addAPPlib(fldnames,"SENDER EMAIL ADDRESS");
		addAPPlib(fldnames,"SUBJECT");
		addAPPlib(fldnames,"BODY");
		addAPPlib(fldnames,"TO RECIPIENTS");
		addAPPlib(fldnames,"CC RECIPIENTS");
		addAPPlib(fldnames,"BCC RECIPIENTS");
		addAPPlib(fldnames,"IGNORE ERRORS");
		addAPPlib(fldnames,"ATTACHMENT FILENAME 0");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 0");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 0");
		addAPPlib(fldnames,"ATTACHMENT DATA 0");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 1");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 1");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 1");
		addAPPlib(fldnames,"ATTACHMENT DATA 1");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 2");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 2");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 2");
		addAPPlib(fldnames,"ATTACHMENT DATA 2");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 3");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 3");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 3");
		addAPPlib(fldnames,"ATTACHMENT DATA 3");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 4");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 4");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 4");
		addAPPlib(fldnames,"ATTACHMENT DATA 4");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 5");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 5");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 5");
		addAPPlib(fldnames,"ATTACHMENT DATA 5");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 6");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 6");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 6");
		addAPPlib(fldnames,"ATTACHMENT DATA 6");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 7");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 7");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 7");
		addAPPlib(fldnames,"ATTACHMENT DATA 7");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 8");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 8");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 8");
		addAPPlib(fldnames,"ATTACHMENT DATA 8");

		addAPPlib(fldnames,"ATTACHMENT FILENAME 9");
		addAPPlib(fldnames,"ATTACHMENT DISPLAY NAME 9");
		addAPPlib(fldnames,"ATTACHMENT MIMETYPE 9");
		addAPPlib(fldnames,"ATTACHMENT DATA 9");

	} else if(!strncmp(&linex[0],"WRITE BLOB",10) ||
		!strncmp(&linex[0],"write blob",10))
	{
		addAPPlib(fldnames,"DATABASE");
		addAPPlib(fldnames,"TABLE");
		addAPPlib(fldnames,"FIELD LIST");
		addAPPlib(fldnames,"KEY VALUES");
		addAPPlib(fldnames,"DECODING METHOD");
		addAPPlib(fldnames,"FILENAME");
	} else if(!strncmp(&linex[0],"HTTP POST",9) ||
		!strncmp(&linex[0],"http post",9))
	{
		addAPPlib(fldnames,"URL");
		addAPPlib(fldnames,"PROXY");
		addAPPlib(fldnames,"PROXY PORT");
		addAPPlib(fldnames,"POST FIELD");
		if(RDAstrcmp(fldname,"URL") && RDAstrcmp(fldname,"PROXY") 
			&& RDAstrcmp(fldname,"PROXY PORT"))
		{
			fieldname=stralloc(fldname);
			if(fldname1!=NULL) Rfree(fldname1);
			fldname1=stralloc("POST FIELD");
		}
	} else if(!strncmp(&linex[0],"PUTENV",6) ||
		!strncmp(&linex[0],"RDA_PutEnv",6))
	{
		fieldname=stralloc(fldname);
		if(fldname1!=NULL) Rfree(fldname1);
		fldname1=stralloc("POST FIELD");
	} else if(!strncmp(&linex[0],"EXPORT",6) ||
		!strncmp(&linex[0],"export",6))
	{
		fieldname=stralloc(fldname);
		if(fldname1!=NULL) Rfree(fldname1);
		fldname1=stralloc("POST FIELD");
	} else if(!strncmp(&linex[0],"FTPPUT",6) ||
		!strncmp(&linex[0],"ftpput",6))
	{
		addAPPlib(fldnames,"URL");
		addAPPlib(fldnames,"USER ID");
		addAPPlib(fldnames,"PASSWORD");
		addAPPlib(fldnames,"PROXY");
		addAPPlib(fldnames,"PROXY PORT");
		addAPPlib(fldnames,"FILENAME");
	} else if(!strncmp(&linex[0],"FTPGET",6) ||
		!strncmp(&linex[0],"ftpget",6))
	{
		addAPPlib(fldnames,"URL");
		addAPPlib(fldnames,"USER ID");
		addAPPlib(fldnames,"PASSWORD");
		addAPPlib(fldnames,"PROXY");
		addAPPlib(fldnames,"PROXY PORT");
		addAPPlib(fldnames,"FILENAME");
	} else if(!strncmp(&linex[0],"WRITE",5) || 
		!strncmp(&linex[0],"write",5)) /* write data file */
	{
		temp1=linex+5;
		while(iswhite(*temp1)) ++temp1;
		modx=stripmodulename(temp1);
		filx=stripfilename(temp1);
		if(modx!=NULL && RDAstrlen(modx)>0 && filx!=NULL && RDAstrlen(filx)>0)
		{
			if(!GETBIN(modx,filx,&engine,&nofields,&fields,&nokeys,&keys)) 
			{
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
						} else {
							temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
						}
						sprintf(temp,"[%s][%s][%s]",modx,filx,f->name);
						addAPPlib(fldnames,temp);
						if(f->name!=NULL) Rfree(f->name);
					}
					Rfree(fields);
				}
				if(keys!=NULL)
				{
					for(y=0,keyx=keys;y<nokeys;++y,++keyx)
					{
						if(keyx->name!=NULL) Rfree(keyx->name);
						if(keyx->part!=NULL)
						{
							for(z=0,part=keyx->part;z<keyx->numparts;++z,
								++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
							Rfree(keyx->part);
						}
					}
					Rfree(keys);
				}
			}
		} else {
			prterr("Error Module [%s] File [%s] names in error cannot display fields.",(modx!=NULL ? modx:"NULL"),(filx!=NULL ? filx:"NULL"));
		}
	}
	if(fldnames->numlibs<1)
	{
		addAPPlib(fldnames,"No Fields Available");
	} else SORTAPPlib(fldnames);
	if(linex!=NULL) Rfree(linex);
	editrsrc=RDArsrcNEW(module,"EDIT FILE VARIABLES");
	addstdrsrc(editrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addstdrsrc(editrsrc,"FIELD NAME",VARIABLETEXT,0,fieldname,FALSE);
	x=0;
	if(varnames!=NULL) freeapplib(varnames);
	varnames=APPlibNEW();
	makefieldvallistrpt(&varnames,TRUE,TRUE,TRUE,TRUE);
	if(varnames->numlibs<1)
	{
		addAPPlib(varnames,"No Field / Values Available");
	}
	x=0;
	addlstrsrc(editrsrc,"SYMBOL TYPES",&type,TRUE,changeSYMBOLtype,
		symboltypes->numlibs,&symboltypes->libs,NULL);
	if(!type)
	{
		for(x=0;x<varnames->numlibs;++x)
		{
			if(!RDAstrcmp(varnames->libs[x],varname)) break;
		}
		if(x>=varnames->numlibs)
		{
			for(x=0;x<varnames->numlibs;++x)
			{
				if(strncmp(varnames->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
			}
			if(x>=varnames->numlibs) x=0;
		}
	} else {
		for(x=0;x<varnames->numlibs;++x)
		{
			if(strncmp(varnames->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=varnames->numlibs) x=0;
		expression=stralloc(varname);
	}
	addlstrsrc(editrsrc,"VARIABLE NAMES",&x,TRUE,NULL,varnames->numlibs,
		&varnames->libs,NULL);
	for(x=0;x<fldnames->numlibs;++x)
	{
		if(!RDAstrcmp(fldnames->libs[x],fldname1)) break;
	}
	if(fldname1!=NULL) Rfree(fldname1);
	if(x>=fldnames->numlibs) x=0;
	addlstrsrc(editrsrc,"FIELD NAMES",&x,TRUE,chgFIELDS,fldnames->numlibs,
		&fldnames->libs,mimgrsrc);
	addsctrsrc(editrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),
		TRUE);
	addbtnrsrc(editrsrc,"LOAD VALUE",TRUE,loadvalue1,mimgrsrc);
	addbtnrsrc(editrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(editrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	if(!type)
	{
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",FALSE);
	} else {
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",FALSE);
	}
	chgFIELDS(editrsrc,mimgrsrc);
	addrfcbrsrc(editrsrc,"SAVE",TRUE,savefunc,mimgrsrc);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quitfunctest,mimgrsrc);
	addbtnrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(expression!=NULL) Rfree(expression);
	editdefaults=GetDefaults(editrsrc);
	if(fieldname!=NULL) Rfree(fieldname);
	APPmakescrn(editrsrc,TRUE,quitline,mimgrsrc,FALSE);
}
static void changeSYMBOLtype(RDArsrc *editrsrc)
{
	int stype=0,x=0,c=0;

	FINDRSCGETINT(editrsrc,"SYMBOL TYPES",&stype);
	if(stype==0)
	{
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",TRUE);
		FINDRSCGETINT(editrsrc,"VARIABLE NAMES",&c);
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",FALSE);
		if(varnames->libs!=NULL)
		{
			for(x=0;x<varnames->numlibs;++x)
			{
				if(strncmp(varnames->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
			}
			if(x>=varnames->numlibs) x=0;
			if(c>x && c<varnames->numlibs) 
			{
				FINDRSCSETINT(editrsrc,"VARIABLE NAMES",c);
			} else {
				FINDRSCSETINT(editrsrc,"VARIABLE NAMES",x);
			}
			updatersrc(editrsrc,"VARIABLE NAMES");
		}
	} else if(stype==1)
	{
		FINDRSCSETSENSITIVE(editrsrc,"VARIABLE NAMES",FALSE);
		FINDRSCSETSENSITIVE(editrsrc,"EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(editrsrc,"LOAD CONTROL",TRUE);
	}
}
static void lineaddabove(RDArsrc *mimgrsrc)
{
	int selected=0;
	char *test=NULL;

	if(imgpval->numlibs==1 && !RDAstrcmp(imgpval->libs[0],"Currently there are no image values defined file this report.")) return;
	readwidget(mimgrsrc,"LINE");
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&test)) return;
	if(strncmp(&test[0],"WRITE",5) && 
		strncmp(&test[0],"UNLINK",6) &&
		strncmp(&test[0],"REMDEL",6) &&
		strncmp(&test[0],"MKDIR",5) &&
		strncmp(&test[0],"RMDIR",5) &&
		strncmp(&test[0],"COPYFILE",8) &&
		strncmp(&test[0],"DOREPORT",8) &&
		strncmp(&test[0],"WWWDOADD",8) &&
		strncmp(&test[0],"MOVEFILE",8) &&
		strncmp(&test[0],"DISPLAYFILE",11) &&
		strncmp(&test[0],"DISPLAYRELATIVEFILE",19) &&
		strncmp(&test[0],"SENDMAIL",8) &&
		strncmp(&test[0],"EMAILUSERINFO",13) && 
		strncmp(&test[0],"SENDATTACH-1",12) &&
		strncmp(&test[0],"SENDATTACH-3",12) &&
		strncmp(&test[0],"SEND EMAIL",10) &&
		strncmp(&test[0],"WRITE BLOB",10) &&
		strncmp(&test[0],"HTTP POST",9) &&
		strncmp(&test[0],"FTPPUT",6) &&
		strncmp(&test[0],"FTPGET",6) &&
		strncmp(&test[0],"PUTENV",6) &&
		strncmp(&test[0],"WRITE_GENERIC_COUNTER",21)) 
	{
		if(test!=NULL) Rfree(test);
		return;
	}
	if(test!=NULL) Rfree(test);
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
	linescreen(mimgrsrc,++selected,NULL,NULL,0,savelineabove,quitlineabovetest);
}
static void copyline(RDArsrc *mimgrsrc)
{
	int selected=0,x=0,copyfrom=0;
	RDApval *p;
	char *test=NULL;

	readwidget(mimgrsrc,"LINE");
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&test)) return;
	if(strncmp(&test[0],"WRITE",5) && 
		strncmp(&test[0],"UNLINK",6) &&
		strncmp(&test[0],"REMDEL",6) &&
		strncmp(&test[0],"RMDIR",5) &&
		strncmp(&test[0],"MKDIR",5) &&
		strncmp(&test[0],"COPYFILE",8) &&
		strncmp(&test[0],"DOERPORT",8) &&
		strncmp(&test[0],"WWWDOADD",8) &&
		strncmp(&test[0],"MOVEFILE",8) &&
		strncmp(&test[0],"DISPLAYFILE",11) &&
		strncmp(&test[0],"DISPLAYRELATIVEFILE",19) &&
		strncmp(&test[0],"SENDMAIL",8) &&
		strncmp(&test[0],"EMAILUSERINFO",13) && 
		strncmp(&test[0],"SENDATTACH-1",12) &&
		strncmp(&test[0],"SENDATTACH-3",12) &&
		strncmp(&test[0],"SEND EMAIL",10) &&
		strncmp(&test[0],"WRITE BLOB",10) &&
		strncmp(&test[0],"HTTP POST",9) &&
		strncmp(&test[0],"FTPPUT",6) &&
		strncmp(&test[0],"PUTENV",6) &&
		strncmp(&test[0],"FTPGET",6) &&
		strncmp(&test[0],"WRITE_GENERIC_COUNTER",21)) 
	{
		if(test!=NULL) Rfree(test);
		return;
	}
	if(test!=NULL) Rfree(test);
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
        copyfrom=selected;
        selected+=2;
	p=holdpval+copyfrom;
	if(p!=NULL)
	{
		x=(int)p->symbol_type;
		linescreen(mimgrsrc,selected,p->name,p->fieldname,x,savelinebelow,quitlinebelowtest);
	} else {
		linescreen(mimgrsrc,selected,NULL,NULL,0,savelinebelow,quitlinebelowtest);
	}
}
static void lineaddbelow(RDArsrc *mimgrsrc)
{
	int selected=0;
	char *test=NULL;

	if(imgpval->numlibs==1 && !RDAstrcmp(imgpval->libs[0],"Currently there are no image values defined file this report.")) return;
	readwidget(mimgrsrc,"LINE");
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&test)) return;
	if(strncmp(&test[0],"WRITE",5) && 
		strncmp(&test[0],"UNLINK",6) &&
		strncmp(&test[0],"REMDEL",6) &&
		strncmp(&test[0],"RMDIR",5) &&
		strncmp(&test[0],"MKDIR",5) &&
		strncmp(&test[0],"COPYFILE",8) &&
		strncmp(&test[0],"DOREPORT",8) &&
		strncmp(&test[0],"WWWDOADD",8) &&
		strncmp(&test[0],"MOVEFILE",8) &&
		strncmp(&test[0],"DISPLAYFILE",11) &&
		strncmp(&test[0],"DISPLAYRELATIVEFILE",19) &&
		strncmp(&test[0],"SENDMAIL",8) &&
		strncmp(&test[0],"EMAILUSERINFO",13) && 
		strncmp(&test[0],"SENDATTACH-1",12) &&
		strncmp(&test[0],"SENDATTACH-3",12) &&
		strncmp(&test[0],"SEND EMAIL",10) &&
		strncmp(&test[0],"WRITE BLOB",10) &&
		strncmp(&test[0],"HTTP POST",9) &&
		strncmp(&test[0],"FTPPUT",6) &&
		strncmp(&test[0],"FTPGET",6) &&
		strncmp(&test[0],"PUTENV",6) &&
		strncmp(&test[0],"WRITE_GENERIC_COUNTER",21)) 
	{
		if(test!=NULL) Rfree(test);
		return;
	}
	if(test!=NULL) Rfree(test);

	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
	selected+=2;
	linescreen(mimgrsrc,selected,NULL,NULL,0,savelinebelow,quitlinebelowtest);
}
static void linedelete(RDArsrc *mimgrsrc)
{
	short x,y;
	RDApval *temp,*temp1,*new_fields;
	int value;

	readallwidgets(mimgrsrc);
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&value)) return;
	if(holdpnum<1) return;
	if(holdpnum>1)
	{
		new_fields=Rmalloc((holdpnum-1)*sizeof(RDApval));
		y=0;
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				temp1->symbol_type=temp->symbol_type;
				temp1->type=temp->type;
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				if(temp->fieldname!=NULL)
				{
					temp1->fieldname=stralloc(temp->fieldname);
					Rfree(temp->fieldname);
				} else temp1->fieldname=NULL;
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
				if(temp->fieldname!=NULL) Rfree(temp->fieldname);
			}
		}
		Rfree(holdpval);
		--holdpnum;
		holdpval=Rmalloc(holdpnum*sizeof(RDApval));
		for(x=0,temp1=holdpval,temp=new_fields;x<holdpnum;++x,
			++temp,++temp1)
		{
			temp1->symbol_type=temp->symbol_type;
			temp1->type=temp->type;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			if(temp->fieldname!=NULL)
			{
				temp1->fieldname=stralloc(temp->fieldname);
				Rfree(temp->fieldname);
			} else temp1->fieldname=NULL;
		}
		Rfree(new_fields);
	} else {
		for(x=0,temp=holdpval;x<holdpnum;++x,++temp)
		{
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->fieldname!=NULL) Rfree(temp->fieldname);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	getimgvalues();
	if(value>=imgpval->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mimgrsrc,"IMAGE VALUES",value,imgpval))
		updatersrc(mimgrsrc,"IMAGE VALUES");
	changedimages=TRUE;
}
static void lineedit(RDArsrc *mimgrsrc)
{
	int selected=0,x=0;
	RDApval *p;
	char *test=NULL;

	readwidget(mimgrsrc,"LINE");
	if(FINDRSCGETSTRING(mimgrsrc,"LINE",&test)) return;
	if(strncmp(&test[0],"WRITE",5) && 
		strncmp(&test[0],"UNLINK",6) &&
		strncmp(&test[0],"REMDEL",6) &&
		strncmp(&test[0],"RMDIR",5) &&
		strncmp(&test[0],"MKDIR",5) &&
		strncmp(&test[0],"COPYFILE",8) &&
		strncmp(&test[0],"DOREPORT",8) &&
		strncmp(&test[0],"WWWDOADD",8) &&
		strncmp(&test[0],"MOVEFILE",8) &&
		strncmp(&test[0],"DISPLAYFILE",11) &&
		strncmp(&test[0],"DISPLAYRELATIVEFILE",19) &&
		strncmp(&test[0],"SENDMAIL",8) &&
		strncmp(&test[0],"EMAILUSERINFO",13) && 
		strncmp(&test[0],"SENDATTACH-1",12) &&
		strncmp(&test[0],"SENDATTACH-3",12) &&
		strncmp(&test[0],"SEND EMAIL",10) &&
		strncmp(&test[0],"WRITE BLOB",10) &&
		strncmp(&test[0],"HTTP POST",9) &&
		strncmp(&test[0],"FTPPUT",6) &&
		strncmp(&test[0],"FTPGET",6) &&
		strncmp(&test[0],"PUTENV",6) &&
		strncmp(&test[0],"WRITE_GENERIC_COUNTER",21)) 
	{
		if(test!=NULL) Rfree(test);
		return;
	}
	if(test!=NULL) Rfree(test);
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&selected)) return;
	p=holdpval+selected;
	if(p!=NULL)
	{
		x=(int)p->symbol_type;
		linescreen(mimgrsrc,++selected,p->name,p->fieldname,x,saveline,quitlinetest);
	} else {
		linescreen(mimgrsrc,++selected,NULL,NULL,0,saveline,quitlinetest);
	}
}
static void makespaceimage(int pos)
{
	short x,y;
	int z=0;
	RDAimage *temp,*temp1,*new_fields;
	RDApval *a,*b;

	++holdnum;
	new_fields=Rmalloc(holdnum*sizeof(RDAimage));
	y=0;
	for(x=0,temp=holdimage;x<(holdnum-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->type=0;
			temp1->breakname=NULL;
			temp1->line=NULL;
			temp1->test=FALSE;
			temp1->numpvals=0;
			temp1->pvals=NULL;
			temp1->expression=NULL;
			++y;
			temp1=new_fields+y;
		}
		temp1->type=temp->type;
		if(temp->breakname!=NULL)
		{
			temp1->breakname=stralloc(temp->breakname);
			Rfree(temp->breakname);
		} else temp1->breakname=NULL;
		if(temp->line!=NULL)
		{
			temp1->line=stralloc(temp->line);
			Rfree(temp->line);
		} else temp1->line=NULL;
		temp1->test=temp->test;
		temp1->numpvals=temp->numpvals;
		if(temp->pvals!=NULL)
		{
			temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
			for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
			{
				a->symbol_type=b->symbol_type;
				if(b->name!=NULL)
				{
					a->name=stralloc(b->name);
					Rfree(b->name);
				} else a->name=NULL;
				a->type=b->type;
				if(b->fieldname!=NULL)
				{
					a->fieldname=stralloc(b->fieldname);
					Rfree(b->fieldname);
				} else a->fieldname=NULL;
			}
			Rfree(temp->pvals);
		}
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		++y;
	}
	Rfree(holdimage);
	holdimage=Rmalloc(holdnum*sizeof(RDAimage));
	for(x=0,temp1=holdimage,temp=new_fields;x<holdnum;++x,++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->breakname!=NULL)
		{
			temp1->breakname=stralloc(temp->breakname);
			Rfree(temp->breakname);
		} else temp1->breakname=NULL;
		if(temp->line!=NULL)
		{
			temp1->line=stralloc(temp->line);
			Rfree(temp->line);
		} else temp1->line=NULL;
		temp1->test=temp->test;
		temp1->numpvals=temp->numpvals;
		if(temp->pvals!=NULL)
		{
			temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
			for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
			{
				a->symbol_type=b->symbol_type;
				if(b->name!=NULL)
				{
					a->name=stralloc(b->name);
					Rfree(b->name);
				} else a->name=NULL;
				a->type=b->type;
				if(b->fieldname!=NULL)
				{
					a->fieldname=stralloc(b->fieldname);
					Rfree(b->fieldname);
				} else a->fieldname=NULL;
			}
			Rfree(temp->pvals);
		}
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
	}
	Rfree(new_fields);
}
static void getimages()
{
	int x,len=0;
	RDAimage *i;
	char *tmp=NULL;

	if(imgavl!=NULL) freeapplib(imgavl);
	imgavl=APPlibNEW();
	if(holdimage!=NULL)
	{
		for(x=0,i=holdimage;x<holdnum;++x,++i)
		{
			len=(5+RDAstrlen(i->line)+RDAstrlen(i->breakname)+(i->expression!=NULL?3:2)+9+1);
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			sprintf(tmp,"[%5d] [%s] [%s] %s",
				(x+1),
				(i->line!=NULL ? i->line:""),
				(i->breakname!=NULL ? i->breakname:""),
				(i->expression!=NULL ? "Yes":"No"));
			addAPPlib(imgavl,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(imgavl->numlibs<1)
	{
		addAPPlib(imgavl,"Currently there are no image lines defined file this report.");
	}
}
static void quitmimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x=0;
	RDApval *pval;

	if(mimgrsrc!=NULL)
	{
		killwindow(mimgrsrc);
		free_rsrc(mimgrsrc);
	}
	if(imgpval!=NULL) freeapplib(imgpval);
	if(holdpval!=NULL)
	{
		for(x=0,pval=holdpval;x<holdpnum;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(holdpval);
		holdpnum=0;	
	}
	if(mimgdefaults!=NULL) FreeRDAdefault(mimgdefaults);
}
static void savemimg(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int value=0,x=0;
	RDAimage *s;
	RDApval *pval,*pval1;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdimage==NULL) 
	{
		holdimage=Rmalloc(sizeof(RDAimage));
		holdnum=1;
	}
	s=holdimage+value;
	readallwidgets(mimgrsrc);
	FINDRSCGETINT(mimgrsrc,"BREAK NAMES",&x);
	if(s->breakname!=NULL) Rfree(s->breakname);
	s->breakname=stralloc(brkavl->libs[x]);
	FINDRSCGETSTRING(mimgrsrc,"LINE",&s->line);
	FINDRSCGETSTRING(mimgrsrc,"EXPRESSION",&s->expression);
	if(s->pvals!=NULL)
	{
		for(x=0,pval=s->pvals;x<s->numpvals;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(s->pvals);
		s->numpvals=0;	
	}
	if(holdpval!=NULL)
	{
		s->numpvals=holdpnum;
		s->pvals=Rmalloc(holdpnum*sizeof(RDApval));
		for(x=0,pval1=holdpval,pval=s->pvals;x<holdpnum;++x,++pval1,++pval)
		{
			pval->symbol_type=pval1->symbol_type;
			pval->type=pval1->type;
			if(pval1->name!=NULL)
			{
				pval->name=stralloc(pval1->name);
				Rfree(pval1->name);
			} else pval->name=NULL;
			if(pval1->fieldname!=NULL)
			{
				pval->fieldname=stralloc(pval1->fieldname);
				Rfree(pval1->fieldname);
			} else pval->fieldname=NULL;
		}
		Rfree(holdpval);
		holdpnum=0;
	}
	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",value,imgavl))
		updatersrc(imgrsrc,"IMAGES");
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		changedimages=TRUE;
	}
	quitmimg(mimgrsrc,imgrsrc);
}
static void savemimgabove(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&x)) return;
	makespaceimage(x);
	savemimg(mimgrsrc,imgrsrc);
}
void lineverify(RDArsrc *mimgrsrc)
{
}
static void savemimgbelow(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	int x;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&x)) return;
	makespaceimage(++x);
	FINDRSCSETINT(imgrsrc,"IMAGES",x);
	savemimg(mimgrsrc,imgrsrc);
}
static void quitmimgtest(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	readallwidgets(mimgrsrc);
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE!\nDo you want to Save these changes?",savemimg,quitmimg,FALSE,2,mimgrsrc,imgrsrc,NULL);
	} else {
		quitmimg(mimgrsrc,imgrsrc);
	}
}
static void quitmimgabovetest(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	readallwidgets(mimgrsrc);
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE!\nDo you want to Save these changes?",savemimgabove,quitmimg,FALSE,2,mimgrsrc,imgrsrc,NULL);
	} else {
		quitmimg(mimgrsrc,imgrsrc);
	}
}
static void quitmimgbelowtest(RDArsrc *mimgrsrc,RDArsrc *imgrsrc)
{
	readallwidgets(mimgrsrc);
	if(COMPARE_RSRCS(mimgrsrc,mimgdefaults,2) || changedimages)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this IMAGE LINE!\nDo you want to Save these changes?",savemimgbelow,quitmimg,FALSE,2,mimgrsrc,imgrsrc,NULL);
	} else {
		quitmimg(mimgrsrc,imgrsrc);
	}
}

static void quitmonths(RDArsrc *r)
{
	if(r!=NULL)	
	{
		killwindow(r);
		free_rsrc(r);
	}
}
static void selectmonths(RDArsrc *r,RDArsrc *mimgrsrc)
{
	char *fm_field=NULL,*amt_field=NULL,*month_tail=NULL;
	char *modx=NULL,*filx=NULL,*linex=NULL,*temp1=NULL;
	RDApval *pf=NULL;
	char temp[1024],fiscal=TRUE;
	int x=0,value=0;

	readallwidgets(r);
	FINDRSCGETSTRING(r,"FISCAL MONTH FIELD",&fm_field);
	FINDRSCGETSTRING(r,"AMOUNT FIELD",&amt_field);
	FINDRSCGETSTRING(r,"MONTHLY FIELD TAIL",&month_tail);
	FINDRSCGETCHAR(r,"FISCAL MONTHS",&fiscal);
	readwidget(mimgrsrc,"LINE");
	FINDRSCGETSTRING(mimgrsrc,"LINE",&linex);
	if(!strncmp(&linex[0],"WRITE",5))
	{
		temp1=linex+5;
		while(iswhite(*temp1)) ++temp1;
		modx=stripmodulename(temp1);
		filx=stripfilename(temp1);
	} else return;
	if(FINDRSCGETINT(mimgrsrc,"IMAGE VALUES",&value)) return;
	if(fiscal==TRUE)
	{
		for(x=0;x<14;++x)
		{
			if(holdpval!=NULL)
			{
				holdpval=Rrealloc(holdpval,(holdpnum+1)*sizeof(RDApval));
			} else {
				holdpval=Rmalloc(sizeof(RDApval));
				holdpnum=0;
			}
			pf=holdpval+holdpnum;
			pf->symbol_type=1;
			pf->type=0;
			memset(temp,0,1024);
			sprintf(temp,"[%s][%s][%s %s]",
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x],
				(month_tail!=NULL ? month_tail:""));
			pf->fieldname=stralloc(temp);
			memset(temp,0,1024);
			sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=([%s][%s][%s %s]%s) ELSE RETURN_VALUE=[%s][%s][%s %s] FI",
				(fm_field!=NULL ? fm_field:""),x,(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x],
				(month_tail!=NULL ? month_tail:""),
				(amt_field!=NULL ? amt_field:""),
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x],
				(month_tail!=NULL ? month_tail:""));
			pf->name=stralloc(temp);
			++holdpnum;
		}
	} else {
		for(x=0;x<12;++x)
		{
			if(holdpval!=NULL)
			{
				holdpval=Rrealloc(holdpval,(holdpnum+1)*sizeof(RDApval));
			} else {
				holdpval=Rmalloc(sizeof(RDApval));
				holdpnum=0;
			}
			pf=holdpval+holdpnum;
			pf->symbol_type=1;
			pf->type=0;
			memset(temp,0,1024);
			sprintf(temp,"[%s][%s][%s %s]",
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x+1],
				(month_tail!=NULL ? month_tail:""));
			pf->fieldname=stralloc(temp);
			memset(temp,0,1024);
			sprintf(temp,"IF(%s=%d) THEN RETURN_VALUE=([%s][%s][%s %s]%s) ELSE RETURN_VALUE=[%s][%s][%s %s] FI",
				(fm_field!=NULL ? fm_field:""),x,(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x+1],
				(month_tail!=NULL ? month_tail:""),
				(amt_field!=NULL ? amt_field:""),
				(modx!=NULL ? modx:""),
				(filx!=NULL ? filx:""),FISCALPERIOD[x+1],
				(month_tail!=NULL ? month_tail:""));
			pf->name=stralloc(temp);
			++holdpnum;
		}
	}
	if(modx!=NULL) Rfree(modx);
	if(filx!=NULL) Rfree(filx);
	if(linex!=NULL) Rfree(linex);
	if(fm_field!=NULL) Rfree(fm_field);
	if(amt_field!=NULL) Rfree(amt_field);
	if(month_tail!=NULL) Rfree(month_tail);
	getimgvalues();
	if(!FINDRSCLISTAPPlib(mimgrsrc,"IMAGE VALUES",value,imgpval))
		updatersrc(mimgrsrc,"IMAGE VALUES");
	quitmonths(r);
}
static void monthlyfields(RDArsrc *mimgrsrc)
{
	RDArsrc *monthrsrc=NULL;
	char fmonths=TRUE;

	monthrsrc=RDArsrcNEW(module,"CREATE MONTHLY FIELDS");
	addstdrsrc(monthrsrc,"FISCAL MONTHS",BOOLNS,1,&fmonths,TRUE);
	addstdrsrc(monthrsrc,"FISCAL MONTH FIELD",VARIABLETEXT,0,"",TRUE);
	addstdrsrc(monthrsrc,"AMOUNT FIELD",VARIABLETEXT,0,"",TRUE);
	addstdrsrc(monthrsrc,"MONTHLY FIELD TAIL",VARIABLETEXT,0,"",TRUE);
	addrfcbrsrc(monthrsrc,"SELECT",TRUE,selectmonths,mimgrsrc);
	addrfcbrsrc(monthrsrc,"QUIT",TRUE,quitmonths,NULL);
	addbtnrsrc(monthrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(monthrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(monthrsrc,TRUE,quitmonths,NULL,FALSE);
}



static char *bstringx[2]={"HEADER","FOOTER"};
static void imagescreen(RDArsrc *imgrsrc,int num,char *breakname,char *line,
	char *expression,int numpvals,RDApval *pvals,
	void (*savefunction)(...),void (*quitfunctiontest)(...))
{
	char *temp=NULL;
	int x,y;
	RDApval *pv,*pv1;
	RDAcontrol *c;
	RDArsrc *mimgrsrc=NULL;

	if(brkavl!=NULL) freeapplib(brkavl);
	brkavl=APPlibNEW();
	addAPPlib(brkavl,"REPORT HEADER");
	addAPPlib(brkavl,"PAGE HEADER");
	for(y=0;y<2;++y)
	{
		if(y==1) addAPPlib(brkavl,"DETAIL LINES");
		for(x=0,c=report->control;x<report->numcontrols;++x,++c)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(c->name)+8);
			} else {
				temp=Rmalloc(RDAstrlen(c->name)+8);
			}
			sprintf(temp,"%s %s",c->name,bstringx[y]);
			addAPPlib(brkavl,temp);
		}
	}
	if(temp!=NULL) Rfree(temp);
	addAPPlib(brkavl,"PAGE FOOTER");
	addAPPlib(brkavl,"REPORT FOOTER");
	for(x=0;x<brkavl->numlibs;++x)
	{
		if(!RDAstrcmp(brkavl->libs[x],breakname)) break;
	}
	mimgrsrc=RDArsrcNEW(module,"MAINTAIN FILE IMAGES");
	addstdrsrc(mimgrsrc,"NUMBER",LONGV,8,&num,FALSE);
	for(x=0;x<brkavl->numlibs;++x)
	{
		if(!RDAstrcmp(breakname,brkavl->libs[x])) break;
	}
	if(x>=brkavl->numlibs) x=0;
	addlstrsrc(mimgrsrc,"BREAK NAMES",&x,TRUE,NULL,brkavl->numlibs,
		&brkavl->libs,NULL);
	addstdrsrc(mimgrsrc,"LINE",VARIABLETEXT,0,(line!=NULL ? line:""),TRUE);
	FINDRSCSETFUNC(mimgrsrc,"LINE",lineverify,NULL);
	addsctrsrc(mimgrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),TRUE);
	if(holdpval!=NULL)
	{
		for(x=0,pv=holdpval;x<holdpnum;++x,++pv)
		{
			if(pv->name!=NULL) Rfree(pv->name);
			if(pv->fieldname!=NULL) Rfree(pv->fieldname);
		}
		Rfree(holdpval);
	}
	holdpnum=numpvals;
	if(pvals!=NULL)
	{
		holdpval=Rmalloc(holdpnum*sizeof(RDApval));
		for(x=0,pv=holdpval,pv1=pvals;x<holdpnum;++x,++pv,++pv1)
		{
			pv->symbol_type=pv1->symbol_type;
			if(pv1->name!=NULL)
			{
				pv->name=stralloc(pv1->name);
			} else pv->name=NULL;
			pv->type=pv1->type;
			if(pv1->fieldname!=NULL)
			{
				pv->fieldname=stralloc(pv1->fieldname);
			} else pv->fieldname=NULL;
		}
	}
	getimgvalues();
	x=0;
	addlstrsrc(mimgrsrc,"IMAGE VALUES",&x,TRUE,NULL,imgpval->numlibs,
		&imgpval->libs,NULL);
	addbtnrsrc(mimgrsrc,"LOAD VALUE",TRUE,loadvalue1,mimgrsrc);
	addbtnrsrc(mimgrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mimgrsrc,"ADD ABOVE",TRUE,lineaddabove,NULL);
	addbtnrsrc(mimgrsrc,"ADD BELOW",TRUE,lineaddbelow,NULL);
	addbtnrsrc(mimgrsrc,"DELETE",TRUE,linedelete,NULL);
	addbtnrsrc(mimgrsrc,"SELECT",TRUE,lineedit,NULL);
	addbtnrsrc(mimgrsrc,"COPY LINE",TRUE,copyline,NULL);
	if(XPERT_SETUP->software_type<2)
	{
		addbtnrsrc(mimgrsrc,"MONTHLY FIELDS",TRUE,monthlyfields,NULL);
	}
	addbtnrsrc(mimgrsrc,"VERIFY",TRUE,lineverify,NULL);
	addrfcbrsrc(mimgrsrc,"SAVE",TRUE,savefunction,imgrsrc);
	addrfcbrsrc(mimgrsrc,"QUIT",TRUE,quitfunctiontest,imgrsrc);
	addbtnrsrc(mimgrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mimgrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mimgdefaults=GetDefaults(mimgrsrc);
	APPmakescrn(mimgrsrc,TRUE,quitmimg,imgrsrc,FALSE);
}
static void imgaddabove(RDArsrc *imgrsrc)
{
	int selected=0;

	if(imgavl->numlibs==1 && !RDAstrcmp(imgavl->libs[0],"Currently there are no image lines defined file this report.")) return;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	imagescreen(imgrsrc,++selected,NULL,NULL,NULL,0,NULL,savemimgabove,quitmimgabovetest);
}
static void imgaddbelow(RDArsrc *imgrsrc)
{
	int selected=0;

	if(imgavl->numlibs==1 && !RDAstrcmp(imgavl->libs[0],"Currently there are no image lines defined file this report.")) return;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	selected+=2;
	imagescreen(imgrsrc,selected,NULL,NULL,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
}
static void imgdelete(RDArsrc *imgrsrc)
{
	short x,y;
	int z;
	RDAimage *temp,*temp1,*new_fields;
	int value;
	RDApval *a,*b;

	if(holdnum<1) return;
	readallwidgets(imgrsrc);
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdnum>1)
	{
		new_fields=Rmalloc((holdnum-1)*sizeof(RDAimage));
		y=0;
		for(x=0,temp=holdimage;x<holdnum;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				temp1->type=temp->type;
				if(temp->breakname!=NULL)
				{
					temp1->breakname=stralloc(temp->breakname);
					Rfree(temp->breakname);
				} else temp1->breakname=NULL;
				if(temp->line!=NULL)
				{
					temp1->line=stralloc(temp->line);
					Rfree(temp->line);
				} else temp1->line=NULL;
				temp1->test=temp->test;
				temp1->numpvals=temp->numpvals;
				if(temp->pvals!=NULL)
				{
					temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
					temp1->numpvals=temp->numpvals;
					for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
					{
						a->symbol_type=b->symbol_type;
						if(b->name!=NULL)
						{
							a->name=stralloc(b->name);
							Rfree(b->name);
						} else a->name=NULL;
						a->type=b->type;
						if(b->fieldname!=NULL)
						{
							a->fieldname=stralloc(b->fieldname);
							Rfree(b->fieldname);
						} else a->fieldname=NULL;
					}
					Rfree(temp->pvals);
				}
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				++y;
			} else {
				if(temp->breakname!=NULL) Rfree(temp->breakname);
				if(temp->line!=NULL) Rfree(temp->line);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->pvals!=NULL)
				{
					for(z=0,a=temp->pvals;z<temp->numpvals;++z,++a)
					{
						if(a->fieldname!=NULL) Rfree(a->fieldname);
						if(a->name!=NULL) Rfree(a->name);
					}
					Rfree(temp->pvals);
				}
			}
		}
		Rfree(holdimage);
		--holdnum;
		holdimage=Rmalloc(holdnum*sizeof(RDAimage));
		for(x=0,temp1=holdimage,temp=new_fields;x<holdnum;++x,
			++temp,++temp1)
		{
			temp1->type=temp->type;
			if(temp->breakname!=NULL)
			{
				temp1->breakname=stralloc(temp->breakname);
				Rfree(temp->breakname);
			} else temp1->breakname=NULL;
			if(temp->line!=NULL)
			{
				temp1->line=stralloc(temp->line);
				Rfree(temp->line);
			} else temp1->line=NULL;
			temp1->test=temp->test;
			temp1->numpvals=temp->numpvals;
			if(temp->pvals!=NULL)
			{
				temp1->pvals=Rmalloc(temp->numpvals*sizeof(RDApval));
				temp1->numpvals=temp->numpvals;
				for(z=0,a=temp1->pvals,b=temp->pvals;z<temp->numpvals;++z,++a,++b)
				{
					a->symbol_type=b->symbol_type;
					if(b->name!=NULL)
					{
						a->name=stralloc(b->name);
						Rfree(b->name);
					} else a->name=NULL;
					a->type=b->type;
					if(b->fieldname!=NULL)
					{
						a->fieldname=stralloc(b->fieldname);
						Rfree(b->fieldname);
					} else a->fieldname=NULL;
				}
				Rfree(temp->pvals);
			}
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
		}
		Rfree(new_fields);
	} else if(holdimage!=NULL)
	{
		for(x=0,temp=holdimage;x<holdnum;++x,++temp)
		{
			if(temp->breakname!=NULL) Rfree(temp->breakname);
			if(temp->line!=NULL) Rfree(temp->line);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->pvals!=NULL)
			{
				for(y=0,a=temp->pvals;y<temp->numpvals;++y,++a)
				{
					if(a->fieldname!=NULL) Rfree(a->fieldname);
					if(a->name!=NULL) Rfree(a->name);
				}
				Rfree(temp->pvals);
			}
		}	
		Rfree(holdimage);
		holdnum=0;
	}
	getimages();
	if(value>=imgavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",value,imgavl))
		updatersrc(imgrsrc,"IMAGES");
	changedimages=TRUE;
}
static void imgmoveup(RDArsrc *imgrsrc)
{
	int selected=0,z=0;
	RDAimage *i;
	RDAimage *i2;
	RDAimage itemp;
	RDApval *a,*b;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	if((i == NULL) || (selected <= 0)) return;
	selected--;
	i2=holdimage+selected;

	itemp.type=i2->type;
	if(i2->breakname!=NULL)
	{
		itemp.breakname=stralloc(i2->breakname);
		Rfree(i2->breakname);
	} else itemp.breakname=NULL;
	if(i2->line!=NULL)
	{
		itemp.line=stralloc(i2->line);
		Rfree(i2->line);
	} else itemp.line=NULL;
	itemp.test=i2->test;
	itemp.numpvals=i2->numpvals;
	if(i2->expression!=NULL)
	{
		itemp.expression=stralloc(i2->expression);
		Rfree(i2->expression);
	} else itemp.expression=NULL;
	if(i2->pvals!=NULL)
	{
		itemp.pvals=Rmalloc(i2->numpvals*sizeof(RDApval));
		for(z=0,a=itemp.pvals,b=i2->pvals;z<i2->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i2->pvals);
	} else itemp.pvals = NULL;

	i2->type=i->type;
	if(i->breakname!=NULL)
	{
		i2->breakname=stralloc(i->breakname);
		Rfree(i->breakname);
	} else i2->breakname=NULL;
	if(i->line!=NULL)
	{
		i2->line=stralloc(i->line);
		Rfree(i->line);
	} else i2->line=NULL;
	i2->test=i->test;
	i2->numpvals=i->numpvals;
	if(i->expression!=NULL)
	{
		i2->expression=stralloc(i->expression);
		Rfree(i->expression);
	} else i2->expression=NULL;
	if(i->pvals!=NULL)
	{
		i2->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
		for(z=0,a=i2->pvals,b=i->pvals;z<i->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i->pvals);
	} else i2->pvals = NULL;

	i->type=itemp.type;
	if(itemp.breakname!=NULL)
	{
		i->breakname=stralloc(itemp.breakname);
		Rfree(itemp.breakname);
	} else i->breakname=NULL;
	if(itemp.line!=NULL)
	{
		i->line=stralloc(itemp.line);
		Rfree(itemp.line);
	} else i->line=NULL;
	i->test=itemp.test;
	i->numpvals=itemp.numpvals;
	if(itemp.expression!=NULL)
	{
		i->expression=stralloc(itemp.expression);
		Rfree(itemp.expression);
	} else i->expression=NULL;
	if(itemp.pvals!=NULL)
	{
		i->pvals=Rmalloc(itemp.numpvals*sizeof(RDApval));
		for(z=0,a=i->pvals,b=itemp.pvals;z<itemp.numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(itemp.pvals);
	} else i->pvals = NULL;

	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",selected,imgavl))
	{
		updatersrc(imgrsrc,"IMAGES");
	}
	changedimages=TRUE;
}
static void imgmovedown(RDArsrc *imgrsrc)
{
	int selected=0,z=0;
	RDAimage *i;
	RDAimage *i2;
	RDAimage itemp;
	RDApval *a,*b;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	if((i == NULL) || (selected >= (imgavl->numlibs-1))) return;
	selected++;
	i2=holdimage+selected;

	itemp.type=i2->type;
	if(i2->breakname!=NULL)
	{
		itemp.breakname=stralloc(i2->breakname);
		Rfree(i2->breakname);
	} else itemp.breakname=NULL;
	if(i2->line!=NULL)
	{
		itemp.line=stralloc(i2->line);
		Rfree(i2->line);
	} else itemp.line=NULL;
	itemp.test=i2->test;
	itemp.numpvals=i2->numpvals;
	if(i2->expression!=NULL)
	{
		itemp.expression=stralloc(i2->expression);
		Rfree(i2->expression);
	} else itemp.expression=NULL;
	if(i2->pvals!=NULL)
	{
		itemp.pvals=Rmalloc(i2->numpvals*sizeof(RDApval));
		for(z=0,a=itemp.pvals,b=i2->pvals;z<i2->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i2->pvals);
	} else itemp.pvals = NULL;

	i2->type=i->type;
	if(i->breakname!=NULL)
	{
		i2->breakname=stralloc(i->breakname);
		Rfree(i->breakname);
	} else i2->breakname=NULL;
	if(i->line!=NULL)
	{
		i2->line=stralloc(i->line);
		Rfree(i->line);
	} else i2->line=NULL;
	i2->test=i->test;
	i2->numpvals=i->numpvals;
	if(i->expression!=NULL)
	{
		i2->expression=stralloc(i->expression);
		Rfree(i->expression);
	} else i2->expression=NULL;
	if(i->pvals!=NULL)
	{
		i2->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
		for(z=0,a=i2->pvals,b=i->pvals;z<i->numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(i->pvals);
	} else i2->pvals = NULL;

	i->type=itemp.type;
	if(itemp.breakname!=NULL)
	{
		i->breakname=stralloc(itemp.breakname);
		Rfree(itemp.breakname);
	} else i->breakname=NULL;
	if(itemp.line!=NULL)
	{
		i->line=stralloc(itemp.line);
		Rfree(itemp.line);
	} else i->line=NULL;
	i->test=itemp.test;
	i->numpvals=itemp.numpvals;
	if(itemp.expression!=NULL)
	{
		i->expression=stralloc(itemp.expression);
		Rfree(itemp.expression);
	} else i->expression=NULL;
	if(itemp.pvals!=NULL)
	{
		i->pvals=Rmalloc(itemp.numpvals*sizeof(RDApval));
		for(z=0,a=i->pvals,b=itemp.pvals;z<itemp.numpvals;++z,++a,++b)
		{
			a->symbol_type=b->symbol_type;
			if(b->name!=NULL)
			{
				a->name=stralloc(b->name);
				Rfree(b->name);
			} else a->name=NULL;
			a->type=b->type;
			if(b->fieldname!=NULL)
			{
				a->fieldname=stralloc(b->fieldname);
				Rfree(b->fieldname);
			} else a->fieldname=NULL;
		}
		Rfree(itemp.pvals);
	} else i->pvals = NULL;

	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",selected,imgavl))
	{
		updatersrc(imgrsrc,"IMAGES");
	}
	changedimages=TRUE;
}
static void imgedit(RDArsrc *imgrsrc)
{
	int selected=0;
	RDAimage *i;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,++selected,i->breakname,i->line,
			i->expression,i->numpvals,i->pvals,savemimg,quitmimgtest);
	} else {
		imagescreen(imgrsrc,++selected,NULL,NULL,NULL,0,NULL,savemimg,quitmimgtest);
	}
}
static void copyimage(RDArsrc *imgrsrc)
{
	int selected=0;
	RDAimage *i;

	if(FINDRSCGETINT(imgrsrc,"IMAGES",&selected)) return;
	i=holdimage+selected;
	selected+=2;
	if(i!=NULL)
	{
		imagescreen(imgrsrc,selected,i->breakname,i->line,
			i->expression,i->numpvals,i->pvals,savemimgbelow,quitmimgbelowtest);
	} else {
		imagescreen(imgrsrc,selected,NULL,NULL,NULL,0,NULL,savemimgbelow,quitmimgbelowtest);
	}
}
static void quitimg(RDArsrc *imgrsrc)
{
	int x,y;
	RDAimage *i;
	RDApval *l;

	if(imgrsrc!=NULL)
	{
		killwindow(imgrsrc);
		free_rsrc(imgrsrc);
	}
	if(imgavl!=NULL) freeapplib(imgavl);
	if(holdimage!=NULL)
	{
		for(x=0,i=holdimage;x<holdnum;++x,++i)
		{
			if(i->breakname!=NULL) Rfree(i->breakname);
			if(i->line!=NULL) Rfree(i->line);
			if(i->expression!=NULL) Rfree(i->expression);
			if(i->pvals!=NULL)
			{
				for(y=0,l=i->pvals;y<i->numpvals;++y,++l)
				{
					if(l->fieldname!=NULL) Rfree(l->fieldname);
					if(l->name!=NULL) Rfree(l->name);
				}
				Rfree(i->pvals);
			}
		}	
		Rfree(holdimage);
		holdnum=0;
	}
	if(brkavl!=NULL) freeapplib(brkavl);
	if(changedimages) changedreport=TRUE;
	if(imgdefaults!=NULL) FreeRDAdefault(imgdefaults);
}
static void quitimgclr(RDArsrc *imgrsrc)
{
	changedimages=FALSE;
	quitimg(imgrsrc);
}
static void saveimg(RDArsrc *imgrsrc)
{
	int x,y;
	RDAimage *i,*j;
	RDApval *l,*m;

	readallwidgets(imgrsrc);
	if(report->image!=NULL)
	{
		for(x=0,i=report->image;x<report->numimages;++x,++i)
		{
			if(i->breakname!=NULL) Rfree(i->breakname);
			if(i->line!=NULL) Rfree(i->line);
			if(i->expression!=NULL) Rfree(i->expression);
			if(i->pvals!=NULL)
			{
				for(y=0,l=i->pvals;y<i->numpvals;++y,++l)
				{
					if(l->fieldname!=NULL) Rfree(l->fieldname);
					if(l->name!=NULL) Rfree(l->name);
				}
				Rfree(i->pvals);
			}
		}	
		Rfree(report->image);
		report->numimages=0;
	}
	if(holdimage!=NULL)
	{
		report->image=Rmalloc(holdnum*sizeof(RDAimage));
		report->numimages=holdnum;
		for(x=0,i=holdimage,j=report->image;x<holdnum;++x,++i,++j)
		{
			if(i->breakname!=NULL)
			{
				j->breakname=stralloc(i->breakname);
				Rfree(i->breakname);
			} else j->breakname=NULL;
			if(i->line!=NULL)
			{
				j->line=stralloc(i->line);
				Rfree(i->line);
			} else j->line=NULL;
			if(i->expression!=NULL)
			{
				j->expression=stralloc(i->expression);
				Rfree(i->expression);
			} else j->expression=NULL;
			if(i->pvals!=NULL)
			{
				j->numpvals=i->numpvals;
				j->pvals=Rmalloc(j->numpvals*sizeof(RDApval));
				for(y=0,l=i->pvals,m=j->pvals;y<i->numpvals;++y,++l,++m)
				{
					if(l->name!=NULL)
					{
						m->name=stralloc(l->name);
						Rfree(l->name);
					} else m->name=NULL;
					if(l->fieldname!=NULL)
					{
						m->fieldname=stralloc(l->fieldname);
						Rfree(l->fieldname);
					} else m->fieldname=NULL;
					m->symbol_type=l->symbol_type;
					m->type=l->type;
				}
				Rfree(i->pvals);
			}
		}
		Rfree(holdimage);
		holdnum=0;
	}
	if(COMPARE_RSRCS(imgrsrc,imgdefaults,0) || changedimages)
	{
		changedimages=TRUE;
	}
	quitimg(imgrsrc);
}
static void savelinefromimage(RDArsrc *imgrsrc)
{
	int value=0;
	RDAimage *s;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdimage==NULL) 
	{
		holdimage=Rmalloc(sizeof(RDAimage));
		holdnum=1;
	}
	s=holdimage+value;
	if(s==NULL) return;
	readallwidgets(imgrsrc);
	holdpnum=s->numpvals;
	FINDRSCGETSTRING(imgrsrc,"LINE",&s->line);
	getimages();
	if(!FINDRSCLISTAPPlib(imgrsrc,"IMAGES",value,imgavl))
	{
		updatersrc(imgrsrc,"IMAGES");
	}
	changedimages=TRUE;
}
static void updateimageline(RDArsrc *imgrsrc)
{
	int value=0;
	RDAimage *s;
	if(FINDRSCGETINT(imgrsrc,"IMAGES",&value)) return;
	if(holdimage==NULL) 
	{
		holdimage=Rmalloc(sizeof(RDAimage));
		holdnum=1;
	}
	s=holdimage+value;
	readallwidgets(imgrsrc);
	FINDRSCSETSTRING(imgrsrc,"LINE",s->line);
	updatersrc(imgrsrc,"LINE");
}
static void quitimgtest(RDArsrc *imgrsrc)
{
	readallwidgets(imgrsrc);
	/* Restore the line to blank so clicking an image line doesn't
	 * register as changing the image.
	 */
	FINDRSCSETSTRING(imgrsrc,"LINE","");
	if(COMPARE_RSRCS(imgrsrc,imgdefaults,0) || changedimages)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the IMAGE LINES!\nDo you want to Save these changes?",saveimg,quitimgclr,FALSE,imgrsrc);
	} else {
		quitimg(imgrsrc);
	}
}
void RPTimage2(RDArsrc *rptrsrc)
{
	int x,y;
	RDAimage *i,*j;
	RDApval *l,*m;
	RDArsrc *imgrsrc=NULL;

	if(report->image!=NULL)
	{
		holdnum=report->numimages;
		holdimage=Rmalloc(report->numimages*sizeof(RDAimage));
		for(x=0,j=holdimage,i=report->image;x<report->numimages;++x,++i,++j)
		{
			j->type=i->type;
			j->breakname=stralloc(i->breakname);
			j->line=stralloc(i->line);
			j->numpvals=i->numpvals;
			j->test=i->test;
			j->expression=stralloc(i->expression);
			if(i->pvals!=NULL)
			{
				j->pvals=Rmalloc(j->numpvals*sizeof(RDApval));
				for(y=0,m=j->pvals,l=i->pvals;y<i->numpvals;++y,++m,++l)
				{
					m->symbol_type=l->symbol_type;
					m->name=stralloc(l->name);
					m->type=l->type;
					m->fieldname=stralloc(l->fieldname);
				}
			}
		}
	}
	getimages();
	x=0;
	imgrsrc=RDArsrcNEW(module,"DEFINE IMAGE");
	/* addition by JAB */
	addstdrsrc(imgrsrc,"LINE",VARIABLETEXT,0,"",TRUE);
	FINDRSCSETFUNC(imgrsrc,"LINE",savelinefromimage,imgrsrc);
	addbtnrsrc(imgrsrc,"SAVE LINE",TRUE,savelinefromimage,NULL);
	/* end addition by JAB */
	addbtnrsrc(imgrsrc,"MOVE UP",TRUE,imgmoveup,NULL);
	addbtnrsrc(imgrsrc,"MOVE DOWN",TRUE,imgmovedown,NULL);
	addlstrsrc(imgrsrc,"IMAGES",&x,TRUE,updateimageline,imgavl->numlibs,
		&imgavl->libs,NULL);
	addbtnrsrc(imgrsrc,"ADD ABOVE",TRUE,imgaddabove,NULL);
	addbtnrsrc(imgrsrc,"ADD BELOW",TRUE,imgaddbelow,NULL);
	addbtnrsrc(imgrsrc,"DELETE",TRUE,imgdelete,NULL);
	addbtnrsrc(imgrsrc,"SELECT",TRUE,imgedit,NULL);
	addbtnrsrc(imgrsrc,"COPY IMAGE",TRUE,copyimage,NULL);
	addrfcbrsrc(imgrsrc,"SAVE",TRUE,saveimg,NULL);
	addrfcbrsrc(imgrsrc,"QUIT",TRUE,quitimgtest,NULL);
	addbtnrsrc(imgrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(imgrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedimages=FALSE;
	imgdefaults=GetDefaults(imgrsrc);
	APPmakescrn(imgrsrc,TRUE,quitimg,NULL,FALSE);
}
