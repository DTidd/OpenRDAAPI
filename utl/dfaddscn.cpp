/* dfaddscn.c - Functions to make default screens */
#include<powadd.hpp>

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
void makeDefineListScreen(PowerAdd *POWERADD)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscrlabel=NULL;
	int x=0;
	AddEditWdgt *field=NULL;
	APPlib *reg_fields=NULL;

	SCREEN=RDAscrnNEW(POWERADD->module,POWERADD->define_list);
	if(POWERADD->EditWdgts!=NULL)
	{
		reg_fields=APPlibNEW();
		for(x=0,field=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++field)
		{
			if(strncmp(field->name,"LAST ",5))
			{
/* skip those that start with LAST */
				addAPPlib(reg_fields,field->name);
			}
		}
#ifdef NEW_SCREEN_STYLE
		if(XPERT_SETUP->software_type<2)
		{
		addwdgt(SCREEN,26,NULL,POWERADD->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,15,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,26,NULL,"&Defaults",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
		addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,26,NULL,"&Help",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(SCREEN,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		}
#endif /* NEW_SCREEN_STYLE */				
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,5,NULL,"Enter a position and length for each of the following:",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(reg_fields->numlibs>8)
		{
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,200,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		SORTAPPlib(reg_fields);
		for(x=0;x<reg_fields->numlibs;++x)
		{
			rscrlabel=ReturnScreenLabel(reg_fields->libs[x]);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
			sprintf(temp1,"%s:",rscrlabel);
			addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(temp1!=NULL) Rfree(temp1);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		for(x=0;x<reg_fields->numlibs;++x)
		{
			rscrlabel=ReturnScreenLabel(reg_fields->libs[x]);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			temp=Rmalloc(RDAstrlen(reg_fields->libs[x])+10);
			sprintf(temp,"%s POSITION",reg_fields->libs[x]);
			temp1=ReturnScreenLabel(temp);
			addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			temp=Rmalloc(RDAstrlen(reg_fields->libs[x])+10);
			sprintf(temp,"%s LENGTH",reg_fields->libs[x]);
			temp1=ReturnScreenLabel(temp);
			addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(rscrlabel!=NULL) Rfree(rscrlabel);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(XPERT_SETUP->software_type==2)
		{
			addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(reg_fields->numlibs>8)
		{
			addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,15,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
#ifndef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	}
#else
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
#endif /* NEW_SCREEN_STYLE */				
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(POWERADD->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,POWERADD->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,POWERADD->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(reg_fields!=NULL) freeapplib(reg_fields);
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
void makeSearchBrowseScreen(PowerAdd *MTN,RDAaddsub *s)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscreename=NULL;
	char *rscrlabel=NULL;
	int x=0;
	NRDfield *field=NULL,*fields=NULL;
	int nofields=0;
	short fileno=(-1),custom_type=0;
	APPlib *bol_fields=NULL,*reg_fields=NULL;

	fileno=OPNNRD(s->module,s->filename);
	if(fileno==(-1))
	{
		prterr("Error:  Unable to open file [%s][%s].",s->module,s->filename);
		return;
	}
	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	SCREEN=RDAscrnNEW(MTN->module,s->search_browse);
	if(nofields>8)
	{
		if(XPERT_SETUP->software_type==2)
		{
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
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
#ifdef XXXX
	ADVaddwdgt(SCREEN,26,"",MTN->name,"","",0,0,0,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,12,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,27,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,26,"","&Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,27,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,26,"","&Help","","",0,0,0,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,27,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#endif
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,23,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */
	if(bol_fields->numlibs>0)
	{
#ifdef NEW_SCREEN_STYLE
		if(XPERT_SETUP->software_type<2)
		{
			addwdgt(SCREEN,20,NULL,"Select",NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(nofields>8)
			{
				ADVaddwdgt(SCREEN,12,"","","","",250,801,0,NULL,NULL,NULL,NULL);
			}
		} else {
			if(nofields>8)
			{
				ADVaddwdgt(SCREEN,12,"","","","",250,801,0,NULL,NULL,NULL,NULL);
			}
		}
#else 
		if(nofields>8)
		{
			ADVaddwdgt(SCREEN,12,"","","","",250,801,0,NULL,NULL,NULL,NULL);
		}
#endif /* NEW_SCREEN_STYLE */
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		for(x=0;x<bol_fields->numlibs;++x)
		{
			field=FLDNRD(fileno,bol_fields->libs[x]);
			if(field!=NULL)
			{
				rscreename=ReturnScreenName(s->filename,field);
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
#ifndef NEW_SCREEN_STYLE
		} else if(reg_fields->numlibs==0) {
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
#endif /* NOT NEW_SCREEN_STYLE */
		}
#ifdef NEW_SCREEN_STYLE
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(XPERT_SETUP->software_type<2)
		{
			if(nofields>8)
			{
				addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
#endif /* NEW_SCREEN_STYLE */
	}
	if(reg_fields->numlibs>0)
	{
		if(XPERT_SETUP->software_type<2)
		{
#ifdef NEW_SCREEN_STYLE
			addwdgt(SCREEN,20,NULL,"Range",NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(nofields>8)
			{
				ADVaddwdgt(SCREEN,12,"","","","",250,800,0,NULL,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
#else
			if(bol_fields->numlibs==0)
			{
				addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
#endif /* NEW_SCREEN_STYLE */
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
				rscreename=ReturnScreenName(s->filename,field);
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
							custom_type=GuessCustomFieldType(s->module,field->name);
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
							custom_type=GuessCustomFieldType(s->module,field->name);
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
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(s->module,field->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,field->len,custom_type,NULL,NULL,NULL);
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
							custom_type=GuessCustomFieldType(s->module,field->name);
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
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
#ifdef NEW_SCREEN_STYLE
			if(nofields>8)
			{
				addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
			addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
#endif /* NEW_SCREEN_STYLE */
		}
	}
	if(bol_fields!=NULL) freeapplib(bol_fields);
	if(reg_fields!=NULL) freeapplib(reg_fields);
#ifndef NEW_SCREEN_STYLE
	if(nofields>8)
	{
		ADVaddwdgt(SCREEN,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */
	if(fileno!=(-1)) CLSNRD(fileno);
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,20,NULL,"Query",NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","Query:","","",0,0,0,NULL,NULL,NULL,NULL);
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		ADVaddwdgt(SCREEN,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
		ADVaddwdgt(SCREEN,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
	}
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,"",NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,12,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	}
#else
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
#endif /* NEW_SCREEN_STYLE */
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
void makeBrowseScreen(PowerAdd *MTN,RDAaddsub *s)
{
	char *temp1=NULL,*temp2=NULL,*temp3=NULL,*temp4=NULL,*lib=NULL;
	RDAscrn *SCREEN=NULL;

	SCREEN=RDAscrnNEW(s->module,s->browse_screen);

	if(SCREEN==NULL) return;
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
	ADVaddwdgt(SCREEN,26,"","&Browse","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"DEFINE LIST","Define List","","",0,0,15,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"SEARCH BUTTON","Search","","",0,0,12,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,6,"PRINT BROWSE","Print Browse","","",0,0,7,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,36,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,27,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,26,"","&Help","","",0,0,0,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,27,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	} else {
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
	}
#else 
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
#endif /* NEW_SCREEN_STYLE */
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
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,36,NULL,NULL,NULL);
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
void makeBrowseDefineListScreen(PowerAdd *MTN,RDAaddsub *s)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscrlabel=NULL;
	int x=0;
	NRDfield *field=NULL,*fields=NULL;
	int nofields=0;
	short fileno=(-1);
	APPlib *reg_fields=NULL;

	fileno=OPNNRD(s->module,s->filename);
	if(fileno==(-1))
	{
		prterr("Error:  Unable to open file [%s][%s].",s->module,s->filename);
		return;
	}
	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	SCREEN=RDAscrnNEW(s->module,s->define_list);
	reg_fields=APPlibNEW();
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		addAPPlib(reg_fields,field->name);
	}
	SORTAPPlib(reg_fields);
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
	}
#endif /* NEW_SCREEN_STYLE */				
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
#ifdef XXXX
	addwdgt(SCREEN,26,NULL,s->filename,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,15,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,26,NULL,"&Defaults",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,26,NULL,"&Help",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
#endif /* XXXX */
	}
#endif /* NEW_SCREEN_STYLE */				
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,"Enter a position and length for each of the following:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(reg_fields->numlibs>8)
	{
		addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,200,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	for(x=0;x<reg_fields->numlibs;++x)
	{
		rscrlabel=ReturnScreenLabel(reg_fields->libs[x]);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
		sprintf(temp1,"%s:",rscrlabel);
		addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(temp1!=NULL) Rfree(temp1);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	for(x=0;x<reg_fields->numlibs;++x)
	{
		rscrlabel=ReturnScreenLabel(reg_fields->libs[x]);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		temp=Rmalloc(RDAstrlen(reg_fields->libs[x])+10);
		sprintf(temp,"%s POSITION",reg_fields->libs[x]);
		temp1=ReturnScreenLabel(temp);
		
		addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
		if(temp!=NULL) Rfree(temp);
		if(temp1!=NULL) Rfree(temp1);
		temp=Rmalloc(RDAstrlen(reg_fields->libs[x])+10);
		sprintf(temp,"%s LENGTH",reg_fields->libs[x]);
		temp1=ReturnScreenLabel(temp);
			
		addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
		if(temp!=NULL) Rfree(temp);
		if(temp1!=NULL) Rfree(temp1);

		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(rscrlabel!=NULL) Rfree(rscrlabel);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if(fileno!=(-1)) CLSNRD(fileno);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(XPERT_SETUP->software_type==2)
	{
		addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(reg_fields->numlibs>8)
	{
		addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,15,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */				
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
static RDAaddsub *SupportingFileonField(PowerAdd *MTN,char *fldname)
{
	int x=0;
	RDAaddsub *s=NULL;
	RDAconfld *confld=NULL;
	char *temp=NULL;

	if(MTN->subord!=NULL)
	{
		for(x=0,s=MTN->subord;x<MTN->subnum;++x,++s)
		{
			confld=s->confld+(s->num-1);
			temp=Rmalloc(RDAstrlen(fldname)+6);
			sprintf(temp,"[%s]",(fldname!=NULL ? fldname:""));
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
static char *ReturnAddScreenLabel(char *name)
{
	char *xmn=NULL;
	char *ret=NULL;
	char last=FALSE;
	int x=0;

	if(!isEMPTY(name))
	{
		ret=stralloc(name);
		if(!strncmp("ADD ",name,4))
		{
			if(RDAstrlen(name)>4)
			{
				sprintf(ret,"%s",&name[4]);
			} else Rfree(ret);
		}
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

void makePowerAddScreen(PowerAdd *POWERADD)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp2=NULL;
	char *rscrlabel=NULL;
	int x=0,y=0,total_len=0,total_lines=0;
	MaintainButton *button=NULL;
	RDAaddsub *supfile=NULL;
	APPlib *reg_fields=NULL,*tot_fields=NULL,*scr_texts=NULL;
	AddEditWdgt *AE=NULL;
	short custom_type=0;
#ifdef NEW_SCREEN_STYLE
	int count=0;
	char *temp=NULL;
#endif /* NEW_SCREEN_STYLE */

	SCREEN=RDAscrnNEW(POWERADD->module,POWERADD->addscreen);

#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,26,NULL,POWERADD->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFINE LIST","Define List",NULL,NULL,0,0,15,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(POWERADD->buttons!=NULL && POWERADD->numbuttons>1)
		{
			if((POWERADD->buttons!=NULL) && POWERADD->numbuttons>1)
			{
				count=0;
				for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
				{
					if(button->type==0 && (!RDAstrcmp(button->progname,"doadd") || !RDAstrcmp(button->progname,"DOADD") || !RDAstrcmp(button->progname,"RUNPOWERADD") || !RDAstrcmp(button->progname,"runpoweradd")))
					{
					if(count==0)
						addwdgt(SCREEN,26,NULL,"&Create",NULL,NULL,0,0,0,NULL,NULL,NULL);
				
					temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
					addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp1!=NULL) Rfree(temp1);
					++count;
					}
				}
				if(count) addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				count=0;
				for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
				{
					if(button->type==0 && (!RDAstrcmp(button->progname,"doreport") || !RDAstrcmp(button->progname,"DOREPORT") || !RDAstrcmp(button->progname,"RUNREPORT") || !RDAstrcmp(button->progname,"runreport")))
					{
					if(count==0)
						addwdgt(SCREEN,26,NULL,"&Print-Process",NULL,NULL,0,0,0,NULL,NULL,NULL);
				
					temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
					addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp1!=NULL) Rfree(temp1);
					++count;
					}
				}
				if(count) addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				count=0;
				for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
				{
					if(button->type==0 && (!RDAstrcmp(button->progname,"mtnmst") || !RDAstrcmp(button->progname,"MTNMST") || !RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"runmaintain")))
					{
					if(count==0)
						addwdgt(SCREEN,26,NULL,"&Edit-View",NULL,NULL,0,0,0,NULL,NULL,NULL);
				
					temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
					addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp1!=NULL) Rfree(temp1);
					++count;
					}
				}
				if(count) addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			}
		}



		addwdgt(SCREEN,26,NULL,"&Defaults",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,18,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
		addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,26,NULL,"&Help",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,27,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
		if(POWERADD->save_type!=3 && POWERADD->save_type!=5)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,6,"DEFINE LIST","Define List",NULL,NULL,0,0,15,NULL,NULL,NULL);
			addwdgt(SCREEN,7,"PREVIOUS ADD'S","Previous Add's","","",4,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
#else
	if(POWERADD->save_type!=3 && POWERADD->save_type!=5)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFINE LIST","Define List",NULL,NULL,0,0,15,NULL,NULL,NULL);
		addwdgt(SCREEN,7,"PREVIOUS ADD'S","Previous Add's","","",4,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */				
	reg_fields=APPlibNEW();
	tot_fields=APPlibNEW();
	scr_texts=APPlibNEW();
	if(POWERADD->EditWdgts!=NULL)
	{
		if(POWERADD->numwdgts>0)
		{
			for(x=0,AE=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++AE)
			{
				if(!strncmp(AE->name,"LAST ",5))
				{
/* skip those that start with LAST */
				} else {
					if(AE->etype==4) 
					{
						addAPPlib(tot_fields,AE->name);
					} else if(AE->type==SCROLLEDTEXT)
					{
						addAPPlib(scr_texts,AE->name);
					} else {
						addAPPlib(reg_fields,AE->name);
					}
				}
			}
		}
	}


#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,23,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,20,NULL,POWERADD->name,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */
	addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,650,0,NULL,NULL,NULL);
/* DISPLAY REGULAR FIELDS */
	if(reg_fields!=NULL)
	{
		if(reg_fields->numlibs>0)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			total_len=0;
			for(y=0;y<reg_fields->numlibs;++y)
			{
				AE=FINDEDITWDGT(POWERADD,reg_fields->libs[y]);
				if(AE!=NULL)
				{
					if(total_len>60) /* was 80 */
					{
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						++total_lines;
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						total_len=0;
					}	
					rscrlabel=ReturnAddScreenLabel(AE->name);
					total_len+=RDAstrlen(rscrlabel);
					if(AE->type!=BOOLNS && AE->type!=SCROLLEDTEXT)
					{
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						supfile=SupportingFileonField(POWERADD,AE->name);
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
						if(AE->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,AE->name,temp1,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(POWERADD->module,AE->name);
							addwdgt(SCREEN,0,AE->name,temp1,NULL,NULL,0,AE->len,custom_type,NULL,NULL,NULL);
						}
						total_len+=AE->len;
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
					} else if(AE->type==BOOLNS)
					{
						addwdgt(SCREEN,9,AE->name,rscrlabel,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
					}
					if(temp1!=NULL) Rfree(temp1);
				}
			}
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			++total_lines;
		}
	}
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */
/* DISPLAY SCROLLED TEXT FIELDS */
	if(scr_texts!=NULL)
	{
		if(scr_texts->numlibs>0)
		{
			for(y=0;y<scr_texts->numlibs;++y)
			{
				if(total_lines>20) break;
				AE=FINDEDITWDGT(POWERADD,scr_texts->libs[y]);
				if(AE!=NULL)
				{
					rscrlabel=ReturnAddScreenLabel(AE->name);
#ifdef NEW_SCREEN_STYLE
					if(XPERT_SETUP->software_type<2)
					{
						addwdgt(SCREEN,20,NULL,rscrlabel,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,8,AE->name,rscrlabel,NULL,NULL,5,65,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						total_lines+=5;
					} else {
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp1,"%s:",rscrlabel);
						addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,8,AE->name,temp1,NULL,NULL,5,65,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp1!=NULL) Rfree(temp1);
						if(rscrlabel!=NULL) Rfree(rscrlabel);
						total_lines+=6;
					}
#else 
					addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
					sprintf(temp1,"%s:",rscrlabel);
					addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,8,AE->name,temp1,NULL,NULL,5,65,0,NULL,NULL,NULL);
					addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp1!=NULL) Rfree(temp1);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
					total_lines+=6;
#endif /* NEW_SCREEN_STYLE */
				}
			}
		}
	}
#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type==2)
	{
		addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
		if(POWERADD->save_type!=3 && POWERADD->save_type!=5)
		{
			addwdgt(SCREEN,20,NULL,"Previous",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,650,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,7,"PREVIOUS ADD'S","Previous Add's","","",6,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
/* DISPLAY TOTAL FIELDS */
			if(tot_fields!=NULL)
			{
				if(tot_fields->numlibs>0)
				{
					if(tot_fields->numlibs==3)
					{
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						for(x=0;x<tot_fields->numlibs;++x)
						{
							AE=FINDEDITWDGT(POWERADD,tot_fields->libs[x]);
							if(AE!=NULL)
							{
								rscrlabel=ReturnScreenLabel(AE->name);
								if(AE->type==BOOLNS)
								{
									addwdgt(SCREEN,9,AE->name,rscrlabel,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
								} else {
									temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
									sprintf(temp1,"%s:",rscrlabel);
									addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
									if(temp1!=NULL) Rfree(temp1);
									addwdgt(SCREEN,0,AE->name,rscrlabel,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
								}
								if(rscrlabel!=NULL) Rfree(rscrlabel);
							} else {
								addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							}
						}
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					} else {
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						for(x=0;x<tot_fields->numlibs;x+=2)
						{
							addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							AE=FINDEDITWDGT(POWERADD,tot_fields->libs[x]);
							if(AE!=NULL)
							{
								rscrlabel=ReturnScreenLabel(AE->name);
								if(AE->type==BOOLNS)
								{
									addwdgt(SCREEN,9,AE->name,rscrlabel,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
								} else {
									temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
									sprintf(temp1,"%s:",rscrlabel);
									addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
									if(temp1!=NULL) Rfree(temp1);
									addwdgt(SCREEN,0,AE->name,rscrlabel,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
								}
								if(rscrlabel!=NULL) Rfree(rscrlabel);
							} else {
								addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							}
							if((x+1)<tot_fields->numlibs)
							{
								AE=FINDEDITWDGT(POWERADD,tot_fields->libs[x+1]);
							} else AE=NULL;
							if(AE!=NULL)
							{
								rscrlabel=ReturnScreenLabel(AE->name);
								if(AE->type==BOOLNS)
								{
									addwdgt(SCREEN,9,AE->name,rscrlabel,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
								} else {
									temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
									sprintf(temp1,"%s:",rscrlabel);
									addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
									if(temp1!=NULL) Rfree(temp1);
									addwdgt(SCREEN,0,temp,rscrlabel,NULL,NULL,0,AE->len,0,NULL,NULL,NULL);
								}
								if(rscrlabel!=NULL) Rfree(rscrlabel);
							} else {
								addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							}
							addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						}
						addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					}
				}
			}
			addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
#else
	addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
#endif 
	
	

#ifdef NEW_SCREEN_STYLE
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,18,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
	if(POWERADD->buttons!=NULL && POWERADD->numbuttons>1)
	{
		if((POWERADD->buttons!=NULL) && POWERADD->numbuttons>1)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
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



		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,18,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,18,NULL,NULL,NULL);
		if((POWERADD->buttons!=NULL) && POWERADD->numbuttons==1)
		{
			button=POWERADD->buttons;
			if(button->type==0)
			{
			temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
			addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(temp1!=NULL) Rfree(temp1);
			}
		}
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	}
#else
	if(POWERADD->buttons!=NULL && POWERADD->numbuttons>1)
	{
		if((POWERADD->buttons!=NULL) && POWERADD->numbuttons>1)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
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



		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,18,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,23,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,18,NULL,NULL,NULL);
		if((POWERADD->buttons!=NULL) && POWERADD->numbuttons==1)
		{
			button=POWERADD->buttons;
			if(button->type==0)
			{
			temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
			addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(temp1!=NULL) Rfree(temp1);
			}
		}
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
#endif /* NEW_SCREEN_STYLE */

	lib=Rmalloc(RDAstrlen(POWERADD->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,POWERADD->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,POWERADD->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(reg_fields!=NULL) freeapplib(reg_fields);
	if(tot_fields!=NULL) freeapplib(tot_fields);
	if(scr_texts!=NULL) freeapplib(scr_texts);
	if(SCREEN!=NULL) free_scrn(SCREEN);
	if(lib!=NULL) Rfree(lib);
}
void makeDefaultMenu(PowerAdd *POWERADD)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(POWERADD==NULL) return;
	if(isEMPTY(POWERADD->module)) return;
	sprintf(temp1,"%s %s",POWERADD->module,POWERADD->name);
	sprintf(temp2,"doadd.EXT %s \"%s\"",POWERADD->module,POWERADD->name);
	menu=RDAmenuNEW(temp1,temp2);
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(POWERADD->module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,POWERADD->module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,POWERADD->module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
