/* dfrptscn.c - Functions to make default screens */
#include<rptgen.hpp>

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
	char *xmn=NULL,*xfdn=NULL,*xfil=NULL;
	char *ret=NULL;
	char last=FALSE;
	int x=0,total_len=0;

	if(!isEMPTY(name))
	{
		xmn=stripmodulename(name);
		xfil=stripfilename(name);
		xfdn=stripfieldname(name);
		if(!isEMPTY(xmn) && isEMPTY(xfdn) && (RDAstrlen(xmn)+2)==RDAstrlen(name))
		{
			ret=stralloc(xmn);
		} else if(!isEMPTY(xmn) && isEMPTY(xfdn))
		{
			ret=Rmalloc(RDAstrlen(name)+1);
			sprintf(ret,"%s%s",xmn,&name[(RDAstrlen(xmn)+2)]);
		} else if(!isEMPTY(xfdn))
		{
			total_len=(RDAstrlen(xmn)+RDAstrlen(xfil)+RDAstrlen(xfdn)+6);
			if(total_len==RDAstrlen(name))
			{
				ret=stralloc(xfdn);
			} else {
				ret=Rmalloc(RDAstrlen(name)+1);
				sprintf(ret,"%s%s",xfdn,&name[total_len]);
			}
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
	int custom_type=0;
	APPlib *reg_fields=NULL,*bol_fields=NULL;

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
	if(nofields>8)
	{
	}
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
	if(bol_fields->numlibs || reg_fields->numlibs)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,23,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	SORTAPPlib(bol_fields);
	SORTAPPlib(reg_fields);
	if(bol_fields->numlibs>0)
	{
		addwdgt(SCREEN,20,NULL,"Select",NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(reg_fields->numlibs>8)
		{
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,791,0,NULL,NULL,NULL);
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
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,791,0,NULL,NULL,NULL);
		}
		if(XPERT_SETUP->software_type<2)
		{
			if(bol_fields->numlibs==0)
			{
				addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
				addwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL);
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
			addwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
		}
		if(reg_fields->numlibs>8)
		{
			addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
		addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if(bol_fields->numlibs || reg_fields->numlibs)
	{
		ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	}
	if(fileno!=(-1)) CLSNRD(fileno);
	if(bol_fields!=NULL) freeapplib(bol_fields);
	if(reg_fields!=NULL) freeapplib(reg_fields);
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
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
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
}
void makeBrowseScreen(MaintainMaster *MTN,RDAsearch *s)
{
	char *temp1=NULL,*temp2=NULL,*temp3=NULL,*temp4=NULL,*lib=NULL;
	RDAscrn *SCREEN=NULL;

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
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,0,NULL,NULL,NULL);
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
	reg_fields=APPlibNEW();
	for(x=0,field=fields;x<nofields;++x,++field)
	{
		addAPPlib(reg_fields,field->name);
	}
	SORTAPPlib(reg_fields);
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
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,0,NULL,NULL,NULL);
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
}
void makeRangeScreen(char *module,RDAreport *r)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscreename=NULL;
	char *rscrlabel=NULL;
	int x=0,y=0;
	NRDfield *field=NULL,*fields=NULL;
	int nofields=0;
	short fileno=(-1),DID_STUFF=FALSE,DID_RANGE=FALSE;
	int custom_type=0;
	RDAsearch *s=NULL;
	RDAvirtual *v=NULL;

	temp=Rmalloc(RDAstrlen(r->name)+14);
	sprintf(temp,"%s RANGE SCREEN",r->name);
	SCREEN=RDAscrnNEW(module,temp);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,23,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,20,NULL,"Report Description",NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(!isEMPTY(r->title))
	{
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	temp=stralloc(r->title);
	stoupper(temp);
	addwdgt(SCREEN,5,NULL,temp,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(temp!=NULL) Rfree(temp);
	}
	if(!isEMPTY(r->desc))
	{
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,r->desc,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if((!isEMPTY(r->title)) || (!isEMPTY(r->desc)))
	{
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,11,NULL,NULL,NULL,NULL,0,0,1,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if(temp!=NULL) Rfree(temp);
	addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(r->virflds!=NULL)
	{
		DID_STUFF=FALSE;
		for(y=0,v=r->virflds;y<r->numvirtuals;++y,++v)
		{
			if(v->range==2 || v->range==4)
			{
				if(!DID_STUFF)
				{
					addwdgt(SCREEN,20,NULL,"Resources",NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				temp=stralloc(v->name);
				rscrlabel=ReturnScreenLabel(v->name);
				temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
				if(v->type!=BOOLNS)
				{
					sprintf(temp1,"%s:",rscrlabel);
					addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(!RDAstrstr(temp,"FILENAME"))
					{
						addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,v->len,0,NULL,NULL,NULL);
					} else {
						addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,v->len,4,NULL,NULL,NULL);
					}
				} else {
					sprintf(temp1,"%s",rscrlabel);
					addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				if(temp!=NULL) Rfree(temp);
				if(temp1!=NULL) Rfree(temp1);
				if(rscrlabel!=NULL) Rfree(rscrlabel);
				addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(!DID_STUFF)
				{
					addwdgt(SCREEN,21,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
				DID_STUFF=TRUE;
			}
		}
	}
	if(r->virflds!=NULL)
	{
		for(y=0,v=r->virflds;y<r->numvirtuals;++y,++v)
		{
			if(v->range==1 || v->range==4)
			{
				if(DID_RANGE==FALSE)
				{
					addwdgt(SCREEN,20,NULL,"Range",NULL,NULL,0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,791,0,NULL,NULL,NULL);
					addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
					addwdgt(SCREEN,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
					addwdgt(SCREEN,5,"","FROM","","",0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
					addwdgt(SCREEN,5,"","TO","","",0,0,0,NULL,NULL,NULL);
					addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
				}
				DID_RANGE=TRUE;
				if(v->type!=SCROLLEDTEXT)
				{
					rscreename=stralloc(v->name);
					rscrlabel=ReturnScreenLabel(v->name);
					if(!isEMPTY(rscreename))
					{
						if(v->type!=BOOLNS)
						{
							if((v->len>20) && (XPERT_SETUP->software_type==2))
							{
								addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								temp=Rmalloc(RDAstrlen(rscreename)+14);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+14);
								sprintf(temp,"RANGE ON [%s]",rscreename);
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
								temp=Rmalloc(RDAstrlen(rscreename)+9);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+9);
								sprintf(temp,"FROM [%s]",rscreename);
								sprintf(temp1,"From %s",rscrlabel);
								if(v->type!=CUSTOMTYPE)
								{
									addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,v->len,0,NULL,NULL,NULL);
								} else {
									custom_type=GuessCustomFieldType(module,v->name);
									addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,v->len,custom_type,NULL,NULL,NULL);
								}
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								temp=Rmalloc(RDAstrlen(rscreename)+7);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+7);
								sprintf(temp,"TO [%s]",rscreename);
								sprintf(temp1,"To %s",rscrlabel);
								if(v->type!=CUSTOMTYPE)
								{
									addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,v->len,0,NULL,NULL,NULL);
								} else {
									custom_type=GuessCustomFieldType(module,v->name);
									addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,v->len,custom_type,NULL,NULL,NULL);
								}
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							} else {
								addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								temp=Rmalloc(RDAstrlen(rscreename)+14);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+14);
								sprintf(temp,"RANGE ON [%s]",rscreename);
								sprintf(temp1,"Range on %s",rscrlabel);
								addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								if(XPERT_SETUP->software_type==2)
								{
									addwdgt(SCREEN,5,NULL,"From:",NULL,NULL,0,0,0,NULL,NULL,NULL);
								} else {
									if(v->len>34) v->len=34;
								}
								temp=Rmalloc(RDAstrlen(rscreename)+9);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+9);
								sprintf(temp,"FROM [%s]",rscreename);
								sprintf(temp1,"From %s",rscrlabel);
								if(v->type!=CUSTOMTYPE)
								{
									addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,v->len,0,NULL,NULL,NULL);
								} else {
									custom_type=GuessCustomFieldType(module,v->name);
									addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,v->len,custom_type,NULL,NULL,NULL);
								}
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								if(XPERT_SETUP->software_type==2)
								{
									addwdgt(SCREEN,5,NULL,"To:",NULL,NULL,0,0,0,NULL,NULL,NULL);
								}
								temp=Rmalloc(RDAstrlen(rscreename)+7);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+7);
								sprintf(temp,"TO [%s]",rscreename);
								sprintf(temp1,"To %s",rscrlabel);
								if(v->type!=CUSTOMTYPE)
								{
									addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,v->len,0,NULL,NULL,NULL);
								} else {
									custom_type=GuessCustomFieldType(module,v->name);
									addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,v->len,custom_type,NULL,NULL,NULL);
								}
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);

								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							}
						}
					}
					if(rscreename!=NULL) Rfree(rscreename);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				}
			} 
		}
	}
	if(r->search!=NULL)
	{
		for(y=0,s=r->search;y<r->numsearchs;++y,++s)
		{
			if(s->range==TRUE)
			{
			if(DID_RANGE==FALSE)
			{
			addwdgt(SCREEN,20,NULL,"Range",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,791,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
			addwdgt(SCREEN,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
			addwdgt(SCREEN,5,"","FROM","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
			addwdgt(SCREEN,5,"","TO","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
			}
			DID_RANGE=TRUE;
			fileno=OPNNRD(s->module,s->filename);
			if(fileno==(-1))
			{
				prterr("Error:  Unable to open file [%s][%s].",s->module,s->filename);
				return;
			}
			nofields=NUMFLDS(fileno);
			fields=FLDPOINTER(fileno);
			for(x=0,field=fields;x<nofields;++x,++field)
			{
				if(field->type!=SCROLLEDTEXT && RDAstrcmp(field->name,"DELETEFLAG"))
				{
					rscreename=ReturnScreenName(s->filename,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(!isEMPTY(rscreename))
					{
						if(field->type!=BOOLNS)
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
					}
					if(rscreename!=NULL) Rfree(rscreename);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				}
			}
			if(fileno!=(-1)) CLSNRD(fileno);
			}
		}
	}
	if(DID_RANGE==TRUE)
	{
		addwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,21,"","","","",0,0,0,NULL,NULL,NULL);
	}
	if(r->virflds!=NULL)
	{
		addwdgt(SCREEN,20,NULL,"Select",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,791,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL);
		for(y=0,v=r->virflds;y<r->numvirtuals;++y,++v)
		{
			if(v->range==1 || v->range==4)
			{
				if(v->type!=SCROLLEDTEXT)
				{
					rscreename=stralloc(v->name);
					rscrlabel=ReturnScreenLabel(v->name);
					if(!isEMPTY(rscreename))
					{
						if(v->type==BOOLNS)
						{
							addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							temp=Rmalloc(RDAstrlen(rscreename)+18);
							temp1=Rmalloc(RDAstrlen(rscrlabel)+24);
							sprintf(temp,"SELECT [%s] FALSE",rscreename);
							sprintf(temp1,"Select when %s False",rscrlabel);
							addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
							sprintf(temp,"SELECT [%s] TRUE",rscreename);
							sprintf(temp1,"Select when %s True",rscrlabel);
							addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
							if(temp!=NULL) Rfree(temp);
							if(temp1!=NULL) Rfree(temp1);
							addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						}
					}
					if(rscreename!=NULL) Rfree(rscreename);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				}
			} 
		}
	}
	if(r->search!=NULL)
	{
		if((r->virflds==NULL))
		{
			addwdgt(SCREEN,20,NULL,"Range",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,791,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL);
		}
		for(y=0,s=r->search;y<r->numsearchs;++y,++s)
		{
			if(s->range==TRUE)
			{
			fileno=OPNNRD(s->module,s->filename);
			if(fileno==(-1))
			{
				prterr("Error:  Unable to open file [%s][%s].",s->module,s->filename);
				return;
			}
			nofields=NUMFLDS(fileno);
			fields=FLDPOINTER(fileno);
			for(x=0,field=fields;x<nofields;++x,++field)
			{
				if(field->type!=SCROLLEDTEXT && RDAstrcmp(field->name,"DELETEFLAG"))
				{
					rscreename=ReturnScreenName(s->filename,field);
					rscrlabel=ReturnScreenLabel(field->name);
					if(!isEMPTY(rscreename))
					{
						if(field->type==BOOLNS)
						{
							addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							temp=Rmalloc(RDAstrlen(rscreename)+18);
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
					}
					if(rscreename!=NULL) Rfree(rscreename);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				}
			}
			if(fileno!=(-1)) CLSNRD(fileno);
			}
		}
	}
	if(((r->virflds!=NULL) || (r->search!=NULL)))
	{
		addwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,21,"","","","",0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"RUN REPORT","Select",NULL,NULL,0,0,25,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
}
void makeRPTBrowseScreen(char *module,RDAreport *r)
{
	char *temp1=NULL,*temp2=NULL,*temp3=NULL,*temp4=NULL,*lib=NULL;
	RDAscrn *SCREEN=NULL;

	temp1=Rmalloc(RDAstrlen(r->name)+8);
	sprintf(temp1,"%s BROWSE",r->name);
	SCREEN=RDAscrnNEW(module,temp1);
	if(temp1!=NULL) Rfree(temp1);
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
	ADVaddwdgt(SCREEN,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,30,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","Search Field:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,0,"SEARCH FIELD","Search Field","","",0,40,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,36,NULL,NULL,NULL);
	if(r->display->device<3)
	{
		addwdgt(SCREEN,6,"CONTINUE","Continue",NULL,NULL,0,0,7,NULL,NULL,NULL);
	} else {
		addwdgt(SCREEN,6,"SAVE","Save/Finished",NULL,NULL,0,0,54,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
}
static APPlib *RDAReportFields(RDAreport *r)
{
	APPlib *tmp=NULL;
	RDAsort *sort=NULL;
	RDAaccum *accum=NULL;
	RDAimage *image=NULL;
	RDApval *pval=NULL;
	int x=0,y=0;

	if(r==NULL) return(NULL);
	tmp=APPlibNEW();
	if(r->sort!=NULL)
	{
		for(x=0,sort=r->sort;x<r->numsorts;++x,++sort)
		{
			addAPPlibNoDuplicates(tmp,sort->name);
		}
	}
	if(r->accum!=NULL)
	{
		for(x=0,accum=r->accum;x<r->numaccums;++x,++accum)
		{
			addAPPlibNoDuplicates(tmp,accum->accname);
		}
	}
	if(r->image!=NULL)
	{
		for(x=0,image=r->image;x<r->numimages;++x,++image)
		{
			if(!RDAstrcmp(image->breakname,"DETAIL LINES"))
			{
				if(image->pvals!=NULL)
				{
				for(y=0,pval=image->pvals;y<image->numpvals;++y,++pval)
				{
					if(pval->symbol_type==0 && 
						!isEMPTY(pval->name))
					{
						if(RDAstrcmp(pval->name,"[SYSTEM DATE]") && 
							RDAstrcmp(pval->name,"[SYSTEM DATE-YYYY]") && 
							RDAstrcmp(pval->name,"[LOGIN IDENTIFICATION]") &&
							RDAstrcmp(pval->name,"[NEXT TRANSACTION NO]") &&
							RDAstrcmp(pval->name,"[SYSTEM TIME]"))
						{
							addAPPlibNoDuplicates(tmp,pval->name);
						}
					}
				}
				}
			}
		}
	}
	if(tmp->numlibs<1) 
	{
		freeapplib(tmp);
		return(NULL);
	}
	SORTAPPlib(tmp);
	return(tmp);
}
void makeRPTSearchBrowse(char *module,RDAreport *r)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL;

	
	temp1=Rmalloc(RDAstrlen(r->name)+15);
	sprintf(temp1,"%s SEARCH BROWSE",r->name);
	SCREEN=RDAscrnNEW(module,temp1);
	if(temp1!=NULL) Rfree(temp1);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
		addwdgt(SCREEN,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
		addwdgt(SCREEN,5,"","FROM","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,10,"","","","",0,0,2,NULL,NULL,NULL);
		addwdgt(SCREEN,5,"","TO","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
	}
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,5,"","","","",0,0,0,NULL,NULL,NULL);
	}
	ADVaddwdgt(SCREEN,9,"SELECT DELETEFLAG FALSE","Select Non-Skipped","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,9,"SELECT DELETEFLAG TRUE","Select Skipped","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(SCREEN,5,"","Query:","","",0,0,0,NULL,NULL,NULL,NULL);
	if(XPERT_SETUP->software_type<2)
	{
		ADVaddwdgt(SCREEN,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(SCREEN,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	} else {
		ADVaddwdgt(SCREEN,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
	}
	ADVaddwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"SELECT","Select",NULL,NULL,0,0,12,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,14,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,11,NULL,NULL,NULL);
	addwdgt(SCREEN,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,"(([SOFTWARE TYPE]=0) OR ((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))))",NULL,NULL);
	addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,17,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

	lib=Rmalloc(RDAstrlen(module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
}
void makeRPTDefineList(char *module,RDAreport *r)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscrlabel=NULL;
	int x=0;
	APPlib *field=NULL;

	field=RDAReportFields(r);
	if(field==NULL) return;
	SORTAPPlib(field);

	temp1=Rmalloc(RDAstrlen(r->name)+15);
	sprintf(temp1,"%s DEFINE LIST",r->name);
	SCREEN=RDAscrnNEW(module,temp1);
	if(temp1!=NULL) Rfree(temp1);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,"Enter a position and length for each of the following:",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(field->numlibs>8)
	{
		addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,200,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	for(x=0;x<field->numlibs;++x)
	{
		temp1=stripmodulename(field->libs[x]);
		if((RDAstrlen(temp1)+2)<RDAstrlen(field->libs[x]))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=stripfieldname(field->libs[x]);
		}
		rscrlabel=ReturnScreenLabel(temp1);
		if(temp1!=NULL) Rfree(temp1);
/* The tests on the rscrlabel here and below were added 03/31/2003 by ddc. */
/* Reason: For define list screens with the NEXT TRANSACTION NO field */
/* available, the label (in the left column or first box) for the NEXT */
/* TRANSACTION NO fields would be (null).  This would give errors when the */
/* screen is displayed by doreport. */
		if(rscrlabel!=NULL)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
			sprintf(temp1,"%s:",rscrlabel);
			addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(temp1!=NULL) Rfree(temp1);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	for(x=0;x<field->numlibs;++x)
	{
		temp1=stripmodulename(field->libs[x]);
		if((RDAstrlen(temp1)+2)<RDAstrlen(field->libs[x]))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=stripfieldname(field->libs[x]);
		}
		rscrlabel=ReturnScreenLabel(temp1);
		if(temp1!=NULL) Rfree(temp1);
		if(rscrlabel!=NULL)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			temp=Rmalloc(RDAstrlen(field->libs[x])+10);
			sprintf(temp,"%s POSITION",field->libs[x]);
			temp1=ReturnScreenLabel(temp);
			addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			temp=Rmalloc(RDAstrlen(field->libs[x])+10);
			sprintf(temp,"%s LENGTH",field->libs[x]);
			temp1=ReturnScreenLabel(temp);
			addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,3,0,NULL,NULL,NULL);
			if(temp!=NULL) Rfree(temp);
			if(temp1!=NULL) Rfree(temp1);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(rscrlabel!=NULL) Rfree(rscrlabel);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(XPERT_SETUP->software_type==2)
	{
		addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(field->numlibs>8)
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

	lib=Rmalloc(RDAstrlen(module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(field!=NULL) freeapplib(field);
	if(SCREEN!=NULL) free_scrn(SCREEN);
}
void makeRPTMaintainScreen(char *module,RDAreport *r)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	char *rscrlabel=NULL;
	int x=0;
	APPlib *field=NULL;

	
	field=RDAReportFields(r);
	temp1=Rmalloc(RDAstrlen(r->name)+35);
	sprintf(temp1,"%s MAINTAIN SCREEN",r->name);
	SCREEN=RDAscrnNEW(module,temp1);
	if(temp1!=NULL) Rfree(temp1);
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,9,"DELETEFLAG","Skip this record",NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,791,0,NULL,NULL,NULL);
	for(x=0;x<field->numlibs;++x)
	{
		temp1=stripmodulename(field->libs[x]);
		if((RDAstrlen(temp1)+2)<RDAstrlen(field->libs[x]))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=stripfieldname(field->libs[x]);
		}
		rscrlabel=ReturnScreenLabel(temp1);
		if(temp1!=NULL) Rfree(temp1);

		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
		sprintf(temp1,"%s:",rscrlabel);
		addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(temp1!=NULL) Rfree(temp1);
		temp=Rmalloc(RDAstrlen(field->libs[x])+10);
		sprintf(temp,"%s",field->libs[x]);
		temp1=ReturnScreenLabel(temp);
		addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,15,0,NULL,NULL,NULL);
		if(temp!=NULL) Rfree(temp);
		if(temp1!=NULL) Rfree(temp1);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if(field!=NULL) freeapplib(field);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);

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

	lib=Rmalloc(RDAstrlen(module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
}
void makeDefaultMenu(char *module,RDAreport *MTN)
{
	RDAmenu *menu=NULL;
	char *defdir=NULL,temp1[512],temp2[512];

	memset(temp1,0,512);
	memset(temp2,0,512);
	if(MTN==NULL) return;
	if(isEMPTY(module)) return;
	sprintf(temp1,"%s %s",module,MTN->name);
	sprintf(temp2,"doreport.EXT %s \"%s\"",module,MTN->name);
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
