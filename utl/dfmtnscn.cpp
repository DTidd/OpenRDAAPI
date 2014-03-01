/* dfmtnscn.c - Functions to make default screens */
#include<mix.hpp>
#include<fin.hpp>

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
void makeSearchBrowseExpressionScreen(char *scnmodule,char *expscreen)
{
	char *lib=NULL;
	RDAscrn *SCREEN=NULL;

	SCREEN=RDAscrnNEW(scnmodule,expscreen);
	if(SCREEN==NULL) return;
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"Expression:","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,8,"EXPRESSION","Expression","","",5,80,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"LOAD VALUE","Load Value",NULL,NULL,0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"LOAD GROUPER","Load Grouper",NULL,NULL,0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"LOAD OPERATOR","Load Operator",NULL,NULL,0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"LOAD CONTROL","Load Control",NULL,NULL,0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(scnmodule)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,scnmodule);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,scnmodule);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
void makeSearchBrowseScreen(MaintainMaster *MTN,RDAsearch *s)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscreename=NULL;
	char *rscrlabel=NULL;
	int x=0;
	NRDfield *field=NULL,*fields=NULL;
	int nofields=0;
	short fileno=(-1);
	char *scnmodule=NULL,*expscreen=NULL;
	APPlib *bol_fields=NULL,*reg_fields=NULL;
	short custom_type=0;

	if(s!=NULL)
	{
		fileno=OPNNRD(s->module,s->filename);
	} else {
		fileno=OPNNRD(MTN->module,MTN->mainfile);
	}
	if(fileno==(-1))
	{
		if(s!=NULL)
		{
			prterr("Error:  Unable to open file [%s][%s].",s->module,s->filename);
		} else {
			prterr("Error:  Unable to open file [%s][%s].",MTN->module,MTN->mainfile);
		}
		return;
	}
	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	SCREEN=RDAscrnNEW(MTN->module,(s!=NULL ? s->search_browse:MTN->search_browse));
	bol_fields=APPlibNEW();
	reg_fields=APPlibNEW();
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,23,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
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
		addwdgt(SCREEN,20,NULL,"Select",NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(bol_fields->numlibs>8)
		{
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,350,791,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		for(x=0;x<bol_fields->numlibs;++x)
		{
			field=FLDNRD(fileno,bol_fields->libs[x]);
			if(field!=NULL)
			{
				rscreename=ReturnScreenName((s!=NULL ? s->filename:MTN->mainfile),field);
				rscrlabel=ReturnScreenLabel(field->name);
				if(!isEMPTY(rscreename))
				{
					addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
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
		}
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(bol_fields->numlibs>8)
		{
			addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if(reg_fields->numlibs>0)
	{
		addwdgt(SCREEN,20,NULL,"Range",NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(reg_fields->numlibs>8)
		{
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,350,791,0,NULL,NULL,NULL);
		}
		if(XPERT_SETUP->software_type<2)
		{
/* DDC 04/16/2003 - Why did I do this?  Because GTK does not stretch several */
/* widgets in a box to the edge of the screen when there is nothing */
/* after it on the line (unlike Motif).  Therefore, to get the */
/* SEARCH CRITERIA (etc.) and the booleans to line up like they should */
/* (with the regular range fields), you have to put them in the same box. */
/* But the box still has to be started, even if there are no booleans. */
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
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
				rscreename=ReturnScreenName((s!=NULL ? s->filename:MTN->mainfile),field);
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
							custom_type=GuessCustomFieldType((s!=NULL ? s->module:MTN->module),field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,custom_type,NULL,NULL,NULL);
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
							custom_type=GuessCustomFieldType((s!=NULL ? s->module:MTN->module),field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,custom_type,NULL,NULL,NULL);
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
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,(field->len<25 ? field->len:25),0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType((s!=NULL ? s->module:MTN->module),field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,(field->len<25 ? field->len:25),custom_type,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						if(XPERT_SETUP->software_type==2)
						{
							addwdgt(SCREEN,5,NULL,"To:",NULL,NULL,0,0,0,NULL,NULL,NULL);
						}
						temp=Rmalloc(RDAstrlen(rscreename)+6);
						temp=Rmalloc(RDAstrlen(rscreename)+4);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+4);
						sprintf(temp,"TO %s",rscreename);
						sprintf(temp1,"To %s",rscrlabel);
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType((s!=NULL ? s->module:MTN->module),field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,custom_type,NULL,NULL,NULL);
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
			addwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
		}
		if(reg_fields->numlibs>8)
		{
			addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(bol_fields!=NULL) freeapplib(bol_fields);
	if(reg_fields!=NULL) freeapplib(reg_fields);
	if(fileno!=(-1)) CLSNRD(fileno);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","Query:","","",0,0,0,NULL,NULL,NULL,NULL);
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL);
		ADVaddwdgt(SCREEN,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
		addwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL);
	} else {
		ADVaddwdgt(SCREEN,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
	}
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,12,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);

/* Creates the Search Browse Expression Screen */
	if(s!=NULL)
	{
		scnmodule=stralloc(s->module);
		expscreen=Rmalloc(RDAstrlen(s->search_browse)+12);
		sprintf(expscreen,"%s EXPRESSION",s->search_browse);
	} else {
		scnmodule=stralloc(MTN->module);
		expscreen=Rmalloc(RDAstrlen(MTN->search_browse)+12);
		sprintf(expscreen,"%s EXPRESSION",MTN->search_browse);
	}
	makeSearchBrowseExpressionScreen(scnmodule,expscreen);
	addwdgt(SCREEN,14,expscreen,"Expression",NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(scnmodule!=NULL) Rfree(scnmodule);
	if(expscreen!=NULL) Rfree(expscreen);


	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(MTN->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
void makeBrowseScreen(MaintainMaster *MTN,RDAsearch *s)
{
	char *temp1=NULL,*temp2=NULL,*temp3=NULL,*temp4=NULL,*lib=NULL;
	RDAscrn *SCREEN=NULL;
	int x=0;
	MaintainButton *button=NULL;

	if(s!=NULL)
	{
		SCREEN=RDAscrnNEW(s->module,s->browse_screen);
	} else {
		SCREEN=RDAscrnNEW(MTN->module,MTN->browse_screen);
	}
	if(SCREEN==NULL) return;
	temp1=Rmalloc(50+1);
	sprintf(temp1,"%s",
		"[EXTENSIONS SETUP]=TRUE");
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	ADVaddwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"DEFINE LIST","Define List","","",0,0,15,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"SEARCH BUTTON","Search","","",0,0,12,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"PRINT BROWSE","Print Browse","","",0,0,7,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,20,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","Search Field:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","Position","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,0,"BROWSE LIST POSITION","Position","","",0,8,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","Count","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,0,"DELETEFLAG TOTAL","Count","","",0,8,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	if(s==NULL)
	{
		if((MTN->buttons!=NULL) && MTN->numbuttons>1)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(x=0,button=MTN->buttons;x<MTN->numbuttons;++x,++button)
			{
				if(button->type==0)
				{
					temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
					addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp1!=NULL) Rfree(temp1);
				}
			}
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}

	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(s==NULL)
	{
		addwdgt(SCREEN,6,"ADD","Add",NULL,NULL,0,0,16,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,36,NULL,NULL,NULL);
	if(s==NULL)
	{
		if((MTN->buttons!=NULL) && MTN->numbuttons==1)
		{
			button=MTN->buttons;
			if(button->type==0)
			{
				temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
				addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(temp1!=NULL) Rfree(temp1);
			}
		}
	}
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(MTN->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
void makeDefineListScreen(MaintainMaster *MTN,RDAsearch *s)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscrlabel=NULL;
	int x=0;
	NRDfield *field=NULL,*fields=NULL;
	int nofields=0;
	short fileno=(-1);
	APPlib *reg_fields=NULL;

	if(s!=NULL)
	{
		fileno=OPNNRD(s->module,s->filename);
	} else {
		fileno=OPNNRD(MTN->module,MTN->mainfile);
	}
	if(fileno==(-1))
	{
		if(s!=NULL)
		{
			prterr("Error:  Unable to open file [%s][%s].",s->module,s->filename);
		} else {
			prterr("Error:  Unable to open file [%s][%s].",MTN->module,MTN->mainfile);
		}
		return;
	}
	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(s!=NULL)
	{
		SCREEN=RDAscrnNEW(s->module,s->define_list);
	} else {
		SCREEN=RDAscrnNEW(MTN->module,MTN->define_list);
	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,"Enter a position and length for each of the following:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(nofields>8)
	{
		addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,200,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	reg_fields=APPlibNEW();
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		addAPPlib(reg_fields,field->name);
	}
	SORTAPPlib(reg_fields);
	for(x=0;x<reg_fields->numlibs;++x)
	{
		field=FLDNRD(fileno,reg_fields->libs[x]);
		if(field!=NULL)
		{
			rscrlabel=ReturnScreenLabel(field->name);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
			sprintf(temp1,"%s:",rscrlabel);
			addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(temp1!=NULL) Rfree(temp1);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(rscrlabel!=NULL) Rfree(rscrlabel);
		}
	}
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	for(x=0;x<reg_fields->numlibs;++x)
	{
		field=FLDNRD(fileno,reg_fields->libs[x]);
		if(field!=NULL)
		{
			rscrlabel=ReturnScreenLabel(field->name);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			temp=Rmalloc(RDAstrlen(field->name)+10);
			sprintf(temp,"%s POSITION",field->name);
			temp1=ReturnScreenLabel(temp);
		
			addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			temp=Rmalloc(RDAstrlen(field->name)+10);
			sprintf(temp,"%s LENGTH",field->name);
			temp1=ReturnScreenLabel(temp);
			
			addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);

			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(rscrlabel!=NULL) Rfree(rscrlabel);
		}
	}
	if(fileno!=(-1)) CLSNRD(fileno);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(nofields>8)
	{
		addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,15,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(MTN->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(reg_fields!=NULL) freeapplib(reg_fields);
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
static char IsAKeyPart(short fileno,char *fldname)
{
	int x;
	NRDkey *key=NULL;
	NRDpart *part=NULL;

	if(!isEMPTY(fldname))
	{
		key=KEYPOINTER(fileno);
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if(!RDAstrcmp(part->name,fldname)) return(TRUE);
		}
	}
	return(FALSE);
}
static RDAsearch *SupportingFileonField(MaintainMaster *MTN,char *fldname)
{
	int x=0;
	RDAsearch *s=NULL;
	RDAconfld *confld=NULL;
	char *temp=NULL;

	if(MTN->s!=NULL)
	{
		for(x=0,s=MTN->s;x<MTN->num;++x,++s)
		{
			confld=s->confld+(s->num-1);
			temp=Rmalloc(RDAstrlen(MTN->mainfile)+RDAstrlen(fldname)+6);
			sprintf(temp,"[%s][%s]",(MTN->mainfile!=NULL ? MTN->mainfile:""),(fldname!=NULL ? fldname:""));
			if(!RDAstrcmp(temp,confld->conname) && confld->inherit==FALSE) 
			{
				if(temp!=NULL) Rfree(temp);
				return(s);
			}
			if(temp!=NULL) Rfree(temp);
		}
	}
	return(NULL);
}

void makeMaintainScreen(MaintainMaster *MTN)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL,*temp2=NULL;
	char *rscrlabel=NULL;
	int x=0,y=0,total_len=0,noflds=0,q,z=0;
	NRDfield *field=NULL,*fields=NULL,*fld1=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	short fileno=(-1);
	MaintainButton *button=NULL;
	RDAsearch *supfile=NULL;
	char use_fmonths=FALSE,use_cmonths=FALSE,a_field=FALSE;
	APPlib *four_ways=NULL,*reg_fields=NULL,*fmonths=NULL,*cmonths=NULL;
	APPlib *scr_texts=NULL,*col_1=NULL,*col_2=NULL,*col_3=NULL,*col_4=NULL;
	int count=0;
	short custom_type=0;

	fileno=OPNNRD(MTN->module,MTN->mainfile);
	if(fileno==(-1))
	{
		prterr("Error:  Unable to open file [%s][%s].",MTN->module,MTN->mainfile);
		return;
	}
	SCREEN=RDAscrnNEW(MTN->module,MTN->maintain_screen);

	field=FLDNRD(fileno,"DELETEFLAG");
	if(field!=NULL)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		temp=ReturnScreenName(MTN->mainfile,field);
		addwdgt(SCREEN,9,temp,"Delete this record",NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(temp!=NULL) Rfree(temp);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}

	key=KEYPOINTER(fileno);
	if(key!=NULL)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		total_len=0;
		for(y=0,part=key->part;y<key->numparts;++y,++part)
		{
			if(RDAstrcmp(part->name,"DELETEFLAG"))
			{
				field=FLDNRD(fileno,part->name);
				if(field!=NULL)
				{
					if(total_len>60)
					{
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						total_len=0;
					}	
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					total_len+=RDAstrlen(rscrlabel);
					temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
					sprintf(temp1,"%s:",rscrlabel);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
					addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					supfile=SupportingFileonField(MTN,field->name);
					if(supfile!=NULL)
					{
						if(supfile->browse) 
						{
							rscrlabel=ReturnScreenLabel(supfile->button_name);
							addwdgt(SCREEN,6,supfile->button_name,rscrlabel,NULL,NULL,0,0,1,NULL,NULL,NULL);
							if(rscrlabel!=NULL) Rfree(rscrlabel);
							temp2=Rmalloc(RDAstrlen(supfile->button_name)+12);
							sprintf(temp2,"PREVIOUS %s",supfile->button_name);
							rscrlabel=ReturnScreenLabel(temp2);
							addwdgt(SCREEN,6,temp2,rscrlabel,NULL,NULL,0,0,2,NULL,NULL,NULL);
							if(rscrlabel!=NULL) Rfree(rscrlabel);
							if(temp2!=NULL) Rfree(temp2);	
							total_len+=20;
						}
					}
					if((y+1)==key->numparts)
					{
						temp2=Rmalloc(RDAstrlen(key->name)+10);
						sprintf(temp2,"PREVIOUS %s",key->name);
						addwdgt(SCREEN,6,temp2,"Previous",NULL,NULL,0,0,2,NULL,NULL,NULL);
						if(temp2!=NULL) Rfree(temp2);
						total_len+=10;
					}
					if(field->len<20)
					{
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(MTN->module,field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,custom_type,NULL,NULL,NULL);
						}
					} else if((y+1<key->numparts))
					{
						addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						total_len=0;
					} else {
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(MTN->module,field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,custom_type,NULL,NULL,NULL);
						}
					}
					total_len+=field->len;
					if((y+1)==key->numparts)
					{
						temp2=Rmalloc(RDAstrlen(key->name)+10);
						sprintf(temp2,"NEXT %s",key->name);
						addwdgt(SCREEN,6,temp2,"Next",NULL,NULL,0,0,1,NULL,NULL,NULL);
						if(temp2!=NULL) Rfree(temp2);
						total_len+=10;
					}
					if(supfile!=NULL)
					{
						if(supfile->browse) 
						{
							temp2=Rmalloc(RDAstrlen(supfile->button_name)+6);
							sprintf(temp2,"NEXT %s",supfile->button_name);
							rscrlabel=ReturnScreenLabel(temp2);
							addwdgt(SCREEN,6,temp2,rscrlabel,NULL,NULL,0,0,1,NULL,NULL,NULL);
							if(rscrlabel!=NULL) Rfree(rscrlabel);
							if(temp2!=NULL) Rfree(temp2);	
							total_len+=10;
						}
					}
					if(temp!=NULL) Rfree(temp);
					if(temp1!=NULL) Rfree(temp1);
				}
			}
		}
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	noflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(noflds>12)
	{
		ADVaddwdgt(SCREEN,12,"","","","",300,650,0,NULL,NULL,NULL,NULL);
	}

	four_ways=APPlibNEW();
	reg_fields=APPlibNEW();
	fmonths=APPlibNEW();
	cmonths=APPlibNEW();
	scr_texts=APPlibNEW();
	if(fields!=NULL)
	{
		for(y=0,field=fields;y<noflds;++y,++field)
		{
			if(RDAstrcmp(field->name,"DELETEFLAG") &&
				!IsAKeyPart(fileno,field->name))
			{
				if((!strncmp(field->name,"PRE-YEAR ",9)) || (!strncmp(field->name,"POST YEAR ",10)))
				{
					use_fmonths=TRUE;
					use_cmonths=FALSE;
				}
				if(use_fmonths==FALSE)
				{
					for(q=1;q<13;++q)
					{
						if(!strncmp(FISCALPERIOD[q],field->name,RDAstrlen(FISCALPERIOD[q])))
						{
							use_cmonths=TRUE;
						}
					}
				}
			}			
		}
		for(y=0,field=fields;y<noflds;++y,++field)
		{
			a_field=FALSE;
			if(RDAstrcmp(field->name,"DELETEFLAG") &&
				!IsAKeyPart(fileno,field->name))
			{
				if(field->type==SCROLLEDTEXT)
				{
					addAPPlib(scr_texts,field->name);
					a_field=TRUE;
				} else if(!RDAstrcmp(field->name,"SOURCE USER VERIFIED"))
				{
					addAPPlibNoDuplicates(four_ways,field->name);
					addAPPlibNoDuplicates(four_ways,"SOURCE USER");
					addAPPlibNoDuplicates(four_ways,"ENTRY DATE");
					addAPPlibNoDuplicates(four_ways,"ENTRY TIME");
					a_field=TRUE;
				} else if(!RDAstrcmp(field->name,"SOURCE USER"))
				{
					addAPPlibNoDuplicates(four_ways,"SOURCE USER VERIFIED");
					addAPPlibNoDuplicates(four_ways,field->name);
					addAPPlibNoDuplicates(four_ways,"ENTRY DATE");
					addAPPlibNoDuplicates(four_ways,"ENTRY TIME");
					a_field=TRUE;
				} else if(!RDAstrcmp(field->name,"ENTRY DATE"))
				{
					addAPPlibNoDuplicates(four_ways,"SOURCE USER VERIFIED");
					addAPPlibNoDuplicates(four_ways,"SOURCE USER");
					addAPPlibNoDuplicates(four_ways,field->name);
					addAPPlibNoDuplicates(four_ways,"ENTRY TIME");
					a_field=TRUE;
				} else if(!RDAstrcmp(field->name,"ENTRY TIME"))
				{
					addAPPlibNoDuplicates(four_ways,"SOURCE USER VERIFIED");
					addAPPlibNoDuplicates(four_ways,"SOURCE USER");
					addAPPlibNoDuplicates(four_ways,"ENTRY DATE");
					addAPPlibNoDuplicates(four_ways,field->name);
					a_field=TRUE;
				}
				
				if(use_fmonths==TRUE && a_field==FALSE)
				{
					for(q=0;q<14;++q)
					{
						if(!strncmp(FISCALPERIOD[q],field->name,RDAstrlen(FISCALPERIOD[q])))
						{
							addAPPlib(fmonths,field->name);
							a_field=TRUE;
							break;
						}
					}
				}
				if(use_cmonths==TRUE && a_field==FALSE)
				{
					for(q=1;q<13;++q)
					{
						if(!strncmp(FISCALPERIOD[q],field->name,RDAstrlen(FISCALPERIOD[q])))
						{
							addAPPlib(cmonths,field->name);
							a_field=TRUE;
							break;
						}
					}
				}
				if(a_field==FALSE)
				{
					if(field->type==BOOLNS)
					{
						temp=Rmalloc(RDAstrlen(field->name)+25);
						sprintf(temp,"%s BY",(field->name!=NULL ? field->name:""));
						for(z=0,fld1=fields;z<noflds;++z,++fld1)
						{
							if(!RDAstrcmp(fld1->name,temp))	break;
						}
						if(z<noflds)
						{
							addAPPlib(four_ways,field->name);
							addAPPlib(four_ways,temp);
							sprintf(temp,"%s DATE",(field->name!=NULL ? field->name:""));
							addAPPlib(four_ways,temp);
							sprintf(temp,"%s TIME",(field->name!=NULL ? field->name:""));
							addAPPlib(four_ways,temp);
							a_field=TRUE;
						}
						if(temp!=NULL) Rfree(temp);
					}
				}
				if(a_field==FALSE)
				{
					for(z=0;z<four_ways->numlibs;++z)
					{
						if(!RDAstrcmp(four_ways->libs[z],field->name)) break;
					}
					if(z<four_ways->numlibs)
					{
						a_field=TRUE; 
					} else {
						addAPPlib(reg_fields,field->name);
						a_field=TRUE;
					}
				}
			}
		}
	}

/* DISPLAY REGULAR FIELDS */
	if(reg_fields!=NULL)
	{
		if(reg_fields->numlibs>0)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			total_len=0;
			for(y=0;y<reg_fields->numlibs;++y)
			{
				field=FLDNRD(fileno,reg_fields->libs[y]);
				if(field!=NULL)
				{
					if(total_len>60)
					{
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						total_len=0;
					}	
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					total_len+=RDAstrlen(rscrlabel);
					if(field->type!=BOOLNS && field->type!=SCROLLEDTEXT)
					{
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						supfile=SupportingFileonField(MTN,field->name);
						if(supfile!=NULL)
						{
							if(supfile->browse) 
							{
								rscrlabel=ReturnScreenLabel(supfile->button_name);
								addwdgt(SCREEN,6,supfile->button_name,rscrlabel,NULL,NULL,0,0,1,NULL,NULL,NULL);
								if(rscrlabel!=NULL) Rfree(rscrlabel);
								temp2=Rmalloc(RDAstrlen(supfile->button_name)+12);
								sprintf(temp2,"PREVIOUS %s",supfile->button_name);
								rscrlabel=ReturnScreenLabel(temp2);
								addwdgt(SCREEN,6,temp2,rscrlabel,NULL,NULL,0,0,2,NULL,NULL,NULL);
								if(rscrlabel!=NULL) Rfree(rscrlabel);
								if(temp2!=NULL) Rfree(temp2);	
								total_len+=20;
							}
						}
						if(field->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(MTN->module,field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,custom_type,NULL,NULL,NULL);
						}
						total_len+=field->len;
						if(supfile!=NULL)
						{
							if(supfile->browse) 
							{
								temp2=Rmalloc(RDAstrlen(supfile->button_name)+6);
								sprintf(temp2,"NEXT %s",supfile->button_name);
								rscrlabel=ReturnScreenLabel(temp2);
								addwdgt(SCREEN,6,temp2,rscrlabel,NULL,NULL,0,0,1,NULL,NULL,NULL);
								if(rscrlabel!=NULL) Rfree(rscrlabel);
								if(temp2!=NULL) Rfree(temp2);	
								total_len+=10;
							}
						}
					} else if(field->type==BOOLNS)
					{
						addwdgt(SCREEN,9,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
					}
					if(temp!=NULL) Rfree(temp);
					if(temp1!=NULL) Rfree(temp1);
				}
			}
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}

	if(use_fmonths==TRUE && fmonths!=NULL)
	{
/* DISPLAY FISCAL */
	} else if(use_cmonths==TRUE && cmonths!=NULL)
	{
/* DISPLAY CALENDAR */
	}


/* DISPLAY 4-WAYS */
	if(four_ways!=NULL)
	{
		col_1=APPlibNEW();
		col_2=APPlibNEW();
		col_3=APPlibNEW();
		col_4=APPlibNEW();
		if(four_ways->numlibs>0)
		{
			count=0;
			for(y=0;y<four_ways->numlibs;++y)
			{
				++count;
				switch(count)
				{
					default:
					case 1:
						addAPPlib(col_1,four_ways->libs[y]);
						break;
					case 2:
						addAPPlib(col_2,four_ways->libs[y]);
						break;
					case 3:
						addAPPlib(col_3,four_ways->libs[y]);
						break;
					case 4:
						addAPPlib(col_4,four_ways->libs[y]);
						break;
				}
				if(count==4)
				{
					count=0;
				}
			}
		}
	}
	if(four_ways!=NULL)
	{
		if(four_ways->numlibs>0)
		{
			addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(y=0;y<col_1->numlibs;++y)
			{
				addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				field=FLDNRD(fileno,col_1->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(field->type==BOOLNS)
					{
						addwdgt(SCREEN,9,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					} else {
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,0,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					}
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,4,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(y=0;y<col_1->numlibs;++y)
			{
				addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(col_2->numlibs>=y)
				{
				field=FLDNRD(fileno,col_2->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(field->type==BOOLNS)
					{
					} else {
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp1!=NULL) Rfree(temp1);
					}
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,4,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(y=0;y<col_1->numlibs;++y)
			{
				addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(col_2->numlibs>=y)
				{
				field=FLDNRD(fileno,col_2->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(field->type==BOOLNS)
					{
						addwdgt(SCREEN,9,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					} else {
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,0,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					}
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,4,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(y=0;y<col_1->numlibs;++y)
			{
				addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(col_3->numlibs>=y)
				{
				field=FLDNRD(fileno,col_3->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(field->type==BOOLNS)
					{
					} else {
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp1!=NULL) Rfree(temp1);
					}
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,4,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(y=0;y<col_1->numlibs;++y)
			{
				addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(col_3->numlibs>=y)
				{
				field=FLDNRD(fileno,col_3->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(field->type==BOOLNS)
					{
						addwdgt(SCREEN,9,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					} else {
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,0,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					}
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,4,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(y=0;y<col_1->numlibs;++y)
			{
				addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(col_4->numlibs>=y)
				{
				field=FLDNRD(fileno,col_4->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(field->type==BOOLNS)
					{
					} else {
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp1!=NULL) Rfree(temp1);
					}
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,4,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(y=0;y<col_1->numlibs;++y)
			{
				addwdgt(SCREEN,1,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(col_4->numlibs>=y)
				{
				field=FLDNRD(fileno,col_4->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(field->type==BOOLNS)
					{
						addwdgt(SCREEN,9,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					} else {
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,0,temp,rscrlabel,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					}
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				} else {
					addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,4,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,5,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,"","",NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		if(col_1!=NULL) freeapplib(col_1);
		if(col_2!=NULL) freeapplib(col_2);
		if(col_3!=NULL) freeapplib(col_3);
		if(col_4!=NULL) freeapplib(col_4);
	}

/* DISPLAY SCROLLED TEXTS */
	if(scr_texts!=NULL)
	{
		if(scr_texts->numlibs>0)
		{
			for(y=0;y<scr_texts->numlibs;++y)
			{
				field=FLDNRD(fileno,scr_texts->libs[y]);
				if(field!=NULL)
				{
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
					sprintf(temp1,"%s:",rscrlabel);
					addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp1!=NULL) Rfree(temp1);
					addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,8,temp,temp1,NULL,NULL,5,80,0,NULL,NULL,NULL);
					addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				}
			}
		}
	}
	if(four_ways!=NULL) freeapplib(four_ways);
	if(fmonths!=NULL) freeapplib(fmonths);
	if(cmonths!=NULL) freeapplib(cmonths);
	if(reg_fields!=NULL) freeapplib(reg_fields);
	if(scr_texts!=NULL) freeapplib(scr_texts);

	if(noflds>12)
	{
		addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}

	if(fileno!=(-1)) CLSNRD(fileno);
	if(MTN->buttons!=NULL && MTN->numbuttons>1)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"BROWSE","Browse",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,18,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);


		if((MTN->buttons!=NULL) && MTN->numbuttons>1)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(x=0,button=MTN->buttons;x<MTN->numbuttons;++x,++button)
			{
				temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
				addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(temp1!=NULL) Rfree(temp1);
			}
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}



		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,54,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE NO UPDATE","Fast Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"BROWSE","Browse",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,0,NULL,NULL,NULL);
		if((MTN->buttons!=NULL) && MTN->numbuttons==1)
		{
			button=MTN->buttons;
			temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
			addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(temp1!=NULL) Rfree(temp1);
		}
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE NO UPDATE","Fast Save",NULL,NULL,0,0,24,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}

	lib=Rmalloc(RDAstrlen(MTN->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
void makeDefaultMenu(MaintainMaster *MTN)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(MTN==NULL) return;
	if(isEMPTY(MTN->module)) return;
	sprintf(temp1,"%s %s",MTN->module,MTN->name);
	sprintf(temp2,"mtnmst.EXT %s \"%s\"",MTN->module,MTN->name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(MTN->module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,MTN->module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,MTN->module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
