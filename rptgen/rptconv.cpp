/* rptconv.c - Conversion Program from EZRPT to DFRPT Binary */

#include <rptgen.hpp>


#ifdef WIN32
#include <io.h>
#endif


static void ReplaceBrackets(char *name)
{
	if(!isEMPTY(name))
	{
		for(;*name;++name) if(*name=='[' || *name==']') *name='*';
	}
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
		} else {
			ret=stralloc(xfdn);
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
static char *ReturnScreenName(char *name)
{
	char *xmn=NULL,*xfn=NULL,*xfdn=NULL;
	char *ret=NULL;

	if(!isEMPTY(name))
	{
		xmn=stripmodulename(name);
		xfn=stripfilename(name);
		xfdn=stripfieldname(name);
		if(!isEMPTY(xmn) && isEMPTY(xfn) && isEMPTY(xfdn))
		{
			ret=stralloc(name);
		} else {
			ret=Rmalloc(RDAstrlen(xfn)+RDAstrlen(xfdn)+5);
			sprintf(ret,"[%s][%s]",xfn,xfdn);
		}
		if(xmn!=NULL) Rfree(xmn);
		if(xfn!=NULL) Rfree(xfn);
		if(xfdn!=NULL) Rfree(xfdn);
	}
	return(ret);
}
void makeEZRPTRangeSCreen(char *rptmodule,RDAezrpt *ezrpt)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL;
	RDAezsrt *sortfield=NULL;
	RDAezdtl *fields1;
	char *rscrname=NULL;
	char *rscrlabel=NULL;
	int x=0,y=0;
	short custom_type=0;

	temp=Rmalloc(RDAstrlen(ezrpt->name)+14);
	sprintf(temp,"%s RANGE SCREEN",ezrpt->name);
	SCREEN=RDAscrnNEW(rptmodule,temp);
	if(temp!=NULL) Rfree(temp);
	if(!isEMPTY(ezrpt->title))
	{
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	temp=stralloc(ezrpt->title);
	stoupper(temp);
	addwdgt(SCREEN,5,NULL,temp,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	if(temp!=NULL) Rfree(temp);
	}
	if(!isEMPTY(ezrpt->desc))
	{
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,5,NULL,ezrpt->desc,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if((!isEMPTY(ezrpt->title)) || (!isEMPTY(ezrpt->desc)))
	{
	addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	addwdgt(SCREEN,11,NULL,NULL,NULL,NULL,0,0,1,NULL,NULL,NULL);
	addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}
	if(temp!=NULL) Rfree(temp);
	if(ezrpt->print)
	{
		if(XPERT_SETUP->software_type<2)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,5,NULL,"Override Report Title:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,0,"OPTIONAL REPORT TITLE","Optional Report Title",NULL,NULL,0,40,0,NULL,NULL,NULL);
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		} else {
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,5,NULL,"Override Report Title:",NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,0,"OPTIONAL REPORT TITLE","Optional Report Title",NULL,NULL,0,40,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}
	}
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,350,791,0,NULL,NULL,NULL);
	} else {
		addwdgt(SCREEN,12,NULL,NULL,NULL,NULL,300,650,0,NULL,NULL,NULL);
	}
	if(XPERT_SETUP->software_type<2)
	{
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
	for(x=0,sortfield=ezrpt->sort;x<ezrpt->numsrt;++x,++sortfield)
	{
		if(sortfield->range==FALSE && sortfield->type!=SCROLLEDTEXT)
		{
			rscrname=ReturnScreenName(sortfield->name);
			rscrlabel=ReturnScreenLabel(sortfield->name);
			if(!isEMPTY(rscrname))
			{
				if(sortfield->type!=BOOLNS)
				{
					if((sortfield->length>20) && (XPERT_SETUP->software_type==2))
					{
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp=Rmalloc(RDAstrlen(rscrname)+10);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp,"RANGE ON %s",rscrname);
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
						temp=Rmalloc(RDAstrlen(rscrname)+6);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+6);
						sprintf(temp,"FROM %s",rscrname);
						sprintf(temp1,"From %s",rscrlabel);
						if(sortfield->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,sortfield->length,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(rptmodule,sortfield->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,sortfield->length,custom_type,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp=Rmalloc(RDAstrlen(rscrname)+4);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+4);
						sprintf(temp,"TO %s",rscrname);
						sprintf(temp1,"To %s",rscrlabel);
						if(sortfield->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,sortfield->length,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(rptmodule,sortfield->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,sortfield->length,custom_type,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					} else {
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						temp=Rmalloc(RDAstrlen(rscrname)+10);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
						sprintf(temp,"RANGE ON %s",rscrname);
						sprintf(temp1,"Range on %s",rscrlabel);
						addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						if(XPERT_SETUP->software_type==2)
						{
							addwdgt(SCREEN,5,NULL,"From:",NULL,NULL,0,0,0,NULL,NULL,NULL);
						} else {
							if(sortfield->length>34) sortfield->length=34;
						}
						temp=Rmalloc(RDAstrlen(rscrname)+6);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+6);
						sprintf(temp,"FROM %s",rscrname);
						sprintf(temp1,"From %s",rscrlabel);
						if(sortfield->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,sortfield->length,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(rptmodule,sortfield->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,sortfield->length,custom_type,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);
						if(XPERT_SETUP->software_type==2)
						{
							addwdgt(SCREEN,5,NULL,"To:",NULL,NULL,0,0,0,NULL,NULL,NULL);
						}
						temp=Rmalloc(RDAstrlen(rscrname)+4);
						temp1=Rmalloc(RDAstrlen(rscrlabel)+4);
						sprintf(temp,"TO %s",rscrname);
						sprintf(temp1,"To %s",rscrlabel);
						if(sortfield->type!=CUSTOMTYPE)
						{
							addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,sortfield->length,0,NULL,NULL,NULL);
						} else {
							custom_type=GuessCustomFieldType(rptmodule,sortfield->name);
							addwdgt(SCREEN,15,temp,temp1,NULL,NULL,0,sortfield->length,custom_type,NULL,NULL,NULL);
						}
						if(temp!=NULL) Rfree(temp);
						if(temp1!=NULL) Rfree(temp1);

						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					}
				} else {
					addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(XPERT_SETUP->software_type<2)
					{
						addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
					}
					temp=Rmalloc(RDAstrlen(rscrname)+14);
					temp1=Rmalloc(RDAstrlen(rscrlabel)+20);
					sprintf(temp,"SELECT %s FALSE",rscrname);
					sprintf(temp1,"Select when %s False",rscrlabel);
					addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					sprintf(temp,"SELECT %s TRUE",rscrname);
					sprintf(temp1,"Select when %s True",rscrlabel);
					addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if(temp!=NULL) Rfree(temp);
					if(temp1!=NULL) Rfree(temp1);
					addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
				}
			}
			if(rscrname!=NULL) Rfree(rscrname);
			if(rscrlabel!=NULL) Rfree(rscrlabel);
		}
	}
	if(ezrpt->detail!=NULL)
	{
		for(x=0,fields1=ezrpt->detail;x<ezrpt->numdtl;++x,++fields1)
		{
			if((fields1->fieldtype!=SCROLLEDTEXT) && (fields1->range==TRUE))
			{
			if(fields1->type==0)
			{
				y=0;
				for(y=0,sortfield=ezrpt->sort;y<ezrpt->numsrt;++y,++sortfield)
				{
					if(!RDAstrcmp(sortfield->name,fields1->fieldname)) break;
				}
				if(y>=ezrpt->numsrt || ezrpt->numsrt==0)
				{
					rscrname=ReturnScreenName(fields1->fieldname);
					rscrlabel=ReturnScreenLabel(fields1->fieldname);
					if(!isEMPTY(rscrname))
					{
						if(fields1->fieldtype!=BOOLNS)
						{
							if((fields1->length>20) && (XPERT_SETUP->software_type==2))
							{
								addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								temp=Rmalloc(RDAstrlen(rscrname)+10);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
								sprintf(temp,"RANGE ON %s",rscrname);
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
								temp=Rmalloc(RDAstrlen(rscrname)+6);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+6);
								sprintf(temp,"FROM %s",rscrname);
								sprintf(temp1,"From %s",rscrlabel);
								addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,fields1->length,0,NULL,NULL,NULL);
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								temp=Rmalloc(RDAstrlen(rscrname)+4);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+4);
								sprintf(temp,"TO %s",rscrname);
								sprintf(temp1,"To %s",rscrlabel);
								addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,fields1->length,0,NULL,NULL,NULL);
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							} else {
								addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
								temp=Rmalloc(RDAstrlen(rscrname)+10);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
								sprintf(temp,"RANGE ON %s",rscrname);
								sprintf(temp1,"Range on %s",rscrlabel);
								addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								if(XPERT_SETUP->software_type==2)
								{
									addwdgt(SCREEN,5,NULL,"From:",NULL,NULL,0,0,0,NULL,NULL,NULL);
								} else {
									if(fields1->length>34) fields1->length=34;
								}
								temp=Rmalloc(RDAstrlen(rscrname)+6);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+6);
								sprintf(temp,"FROM %s",rscrname);
								sprintf(temp1,"From %s",rscrlabel);
								addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,fields1->length,0,NULL,NULL,NULL);
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);
								if(XPERT_SETUP->software_type==2)
								{
									addwdgt(SCREEN,5,NULL,"To:",NULL,NULL,0,0,0,NULL,NULL,NULL);
								}
								temp=Rmalloc(RDAstrlen(rscrname)+4);
								temp1=Rmalloc(RDAstrlen(rscrlabel)+4);
								sprintf(temp,"TO %s",rscrname);
								sprintf(temp1,"To %s",rscrlabel);
								addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,fields1->length,0,NULL,NULL,NULL);
								if(temp!=NULL) Rfree(temp);
								if(temp1!=NULL) Rfree(temp1);

								addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							}
						} else {
							addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							if(XPERT_SETUP->software_type<2)
							{
								addwdgt(SCREEN,5,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
							}
							temp=Rmalloc(RDAstrlen(rscrname)+14);
							temp1=Rmalloc(RDAstrlen(rscrlabel)+20);
							sprintf(temp,"SELECT %s FALSE",rscrname);
							sprintf(temp1,"Select when %s False",rscrlabel);
							addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
							sprintf(temp,"SELECT %s TRUE",rscrname);
							sprintf(temp1,"Select when %s True",rscrlabel);
							addwdgt(SCREEN,9,temp,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
							if(temp!=NULL) Rfree(temp);
							if(temp1!=NULL) Rfree(temp1);
							addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						}
					}
					if(rscrname!=NULL) Rfree(rscrname);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				}
			}
			}
		}
	}
	if(XPERT_SETUP->software_type<2)
	{
		addwdgt(SCREEN,4,"","","","",0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,"","","","",0,0,0,NULL,NULL,NULL);
	}
	addwdgt(SCREEN,13,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
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

	lib=Rmalloc(RDAstrlen(rptmodule)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/site/%s.SCN",CURRENTDIRECTORY,rptmodule);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\site\\%s.SCN",CURRENTDIRECTORY,rptmodule);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's site directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
}

short detail_length(RDAezrpt *ez)
{
	short temp,longest=1;
	RDAezdtl *detail;
	RDAezsrt *sort;
	int x,count=0;
	char *modx=NULL,*fldx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG detail_length Computing maximum length of printed line on EZ Report [%s].",ez->name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(ez->sort!=NULL)
	{
		for(x=0,sort=ez->sort;x<ez->numsrt;++x,++sort)
		{
			if((sort->total==1 || sort->total==2) && x<(ez->numsrt-1)) 
			{
				modx=stripmodulename(sort->name);
				fldx=stripfieldname(sort->name);
				if(!ez->account_report)
				{
					if(fldx!=NULL) 
					{
						temp=2*count+RDAstrlen(fldx)+9;
					} else {
						temp=2*count+RDAstrlen(modx)+9;
					}
				} else { /* add chart of accounts descriptions */
					if(fldx!=NULL) 
					{
						temp=2*count+RDAstrlen(fldx)+9;
					} else {
						temp=2*count+RDAstrlen(modx)+9;
					}
				}
				if(modx!=NULL) Rfree(modx);
				modx=NULL;
				if(fldx!=NULL) Rfree(fldx);
				fldx=NULL;
				if(temp>longest) longest=temp;
				++count;
			}
		}
	} 
	if(ez->detail!=NULL)
	{
		for(x=0,detail=ez->detail;x<ez->numdtl;++x,++detail)
		{
			if((detail->start+detail->length)>longest)
				longest=detail->start+detail->length+1;
		}
	}
	return(--longest);
}
int xaddvirtualline(RDAreport *r,char *name,int type,short len,
	char *expression,short when,short range,char lv,int line,char *file)
{
	RDAvirtual *v;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addvirtualline Add Virtual Field [%s] to Report [%s] at line [%d] program [%s].",name,r->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r->virflds!=NULL)
	{
		for(x=0,v=r->virflds;x<r->numvirtuals;++x,++v)
		{
			if(!RDAstrcmp(v->name,name)) break;
		}
		if(x<r->numvirtuals) return(r->numvirtuals);
	}
	if(r->virflds!=NULL)
	{
		r->virflds=Rrealloc(r->virflds,(r->numvirtuals+1)*
			sizeof(RDAvirtual));
	} else {
		r->virflds=Rmalloc(sizeof(RDAvirtual));
	}
	v=r->virflds+r->numvirtuals;
	v->when=when;
	if(name!=NULL) v->name=stralloc(name);
		else v->name=NULL;
	v->type=type;
	v->len=len;
	if(expression!=NULL) v->expression=stralloc(expression);
		else v->expression=NULL;
	v->computed=lv;
	v->range=(char)range;
	switch(v->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case BOOLNS:
		case CHARACTERS:
			v->value.string_value=NULL;
			v->dlen=0;
			break;
		case DECIMALV:
		case SDECIMALV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DOUBLEV:
		case SDOUBLEV:
			v->value.float_value=NULL;
			v->dlen=0;
			break;
		case SHORTV:
		case SSHORTV:
			v->value.short_value=NULL;
			v->dlen=0;
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			v->value.integer_value=NULL;
			v->dlen=0;
			break;
		default:
			prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name);
	}
	return(++r->numvirtuals);
}
int xaddcontrolline(RDAreport *r,char *name,short type,int line,char *file)
{
	RDAcontrol *c;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addcontrolline Add Control Break [%s] to Report [%s] at line [%d] program [%s].",name,r->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r->control!=NULL)
	{
		r->control=Rrealloc(r->control,(r->numcontrols+1)*sizeof(RDAcontrol));
	} else {
		r->control=Rmalloc((r->numcontrols+1)*sizeof(RDAcontrol));
	}
	c=r->control+r->numcontrols;
	if(name!=NULL) c->name=stralloc(name);
		else c->name=NULL;
	c->fieldtype=type;
	switch(c->fieldtype)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case BOOLNS:
		case CHARACTERS:
			c->value.string_value=NULL;
			c->dlen=0;
			break;
		case DECIMALV:
		case SDECIMALV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DOUBLEV:
		case SDOUBLEV:
			c->value.float_value=NULL;
			c->dlen=0;
			break;
		case SHORTV:
		case SSHORTV:
			c->value.short_value=NULL;
			c->dlen=0;
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			c->value.integer_value=NULL;
			c->dlen=0;
			break;
		default:
			prterr("Error field type [%d] is invalid for control break [%s].",c->fieldtype,c->name);
	}
	return(++r->numcontrols);
}
int xaddsortline(RDAreport *r,int type,char *name,int line,char *file)
{
	RDAsort *s;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addsortline Add Sort [%s] to Report [%s] at line [%d] program [%s].",name,r->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r->sort!=NULL)
	{
		r->sort=Rrealloc(r->sort,(r->numsorts+1)*sizeof(RDAsort));
	} else {
		r->sort=Rmalloc(sizeof(RDAsort));
	}
	s=r->sort+r->numsorts;
	if(name!=NULL) s->name=stralloc(name);
		else s->name=NULL;
	s->type=type;
	return(++r->numsorts);
}
int xaddsearchline(RDAreport *r,char *mod,char *file,int ctype,char *cmod,
	char *cfile,short num,RDAconfld *confld,char *subkeyname,short range,
	char *expression,char *skip_detail,short numwrites,
	short wc_eval,char *wc_expression,short wc_skip,
	char *skip_detail_error,int line,char *filex)
{
	RDAsearch *sc;
	RDAconfld *f,*t;
	short x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addsearchline Add Search File [%s] [%s] to Report [%s] at line [%d] program [%s].",mod,file,r->name,line,filex);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r->search!=NULL)
	{
		r->search=Rrealloc(r->search,(r->numsearchs+1)*sizeof(RDAsearch));
	} else {
		r->search=Rmalloc(sizeof(RDAsearch));
	}
	sc=r->search+r->numsearchs;
	if(mod!=NULL) sc->module=stralloc(mod);
		else sc->module=NULL;
	if(file!=NULL) sc->filename=stralloc(file);
		else sc->filename=NULL;
	sc->ctype=ctype;
	if(cmod!=NULL) sc->cmodule=stralloc(cmod);
		else sc->cmodule=NULL;
	if(cfile!=NULL) sc->cfilename=stralloc(cfile);
		else sc->cfilename=NULL;
	if(expression!=NULL) sc->expression=stralloc(expression);
		else sc->expression=NULL;
	if(skip_detail!=NULL) sc->skip_detail=stralloc(skip_detail);
		else sc->skip_detail=NULL;
	sc->wc_eval=wc_eval;
	sc->wc_expression=stralloc(wc_expression);
	sc->wc_skip=wc_skip;
	if(skip_detail_error!=NULL) sc->skip_detail_error=stralloc(skip_detail_error);
		else sc->skip_detail_error=NULL;
	if(confld!=NULL)
	{
		sc->num=num;
		sc->confld=Rmalloc(sc->num*sizeof(RDAconfld));
		for(x=0,f=confld,t=sc->confld;x<num;++x,++f,++t)
		{
			if(f->conname!=NULL) t->conname=stralloc(f->conname);
				else t->conname=NULL;
			if(f->fldname!=NULL) t->fldname=stralloc(f->fldname);
				else t->fldname=NULL;
		}
	} else {
		sc->confld=NULL;
		sc->num=0;
	}
	if(subkeyname!=NULL) sc->subkeyname=stralloc(subkeyname);
		else sc->subkeyname=NULL;
	sc->range=range;
	sc->num_writes=numwrites;
	return(++r->numsearchs);
}
int xaddaccumline(RDAreport *r,char *name,int atype,char *accname,short field_type,int line,char *file)
{
	RDAaccum *a;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addaccumline Add Accumulator [%s] to Report [%s] at line [%d] program [%s].",name,r->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r->accum!=NULL)
	{
		r->accum=Rrealloc(r->accum,(r->numaccums+1)*sizeof(RDAaccum));
	} else {
		r->accum=Rmalloc(sizeof(RDAaccum));
		r->numaccums=0;
	}
	a=r->accum+r->numaccums;
	if(name!=NULL) a->name=stralloc(name);
		else a->name=NULL;
	a->atype=atype;
	a->fieldtype=field_type;
	if(accname!=NULL) a->accname=stralloc(accname);
		else a->accname=NULL;
	a->len=0;
	a->num=0;
	a->accvals=NULL;
	return(++r->numaccums);
}
int xaddimageline(RDAreport *tmp,char *breakname,char *line,short test,char *expression,int numpvals,RDApval *pvals,int linex,char *file)
{
	RDApval *f,*t;
	RDAimage *i;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addimageline Add Image Line to Report [%s] at line [%d] program [%s].",tmp->name,linex,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(tmp->image!=NULL)
	{
		tmp->image=Rrealloc(tmp->image,(tmp->numimages+1)*sizeof(RDAimage));
	} else {
		tmp->image=Rmalloc(sizeof(RDAimage));
		tmp->numimages=0;
	}
	i=tmp->image+tmp->numimages;
	i->type=0;
	if(breakname!=NULL) i->breakname=stralloc(breakname);
		else i->breakname=NULL;
	if(line!=NULL) i->line=stralloc(line);
		else i->line=NULL;
	i->test=test;
	if(expression!=NULL) 
	{
		i->expression=stralloc(expression);
		i->test=TRUE;
	} else {
		i->expression=NULL;
		i->test=FALSE;
	}
	if(pvals!=NULL && numpvals>0)
	{
		i->numpvals=numpvals;
		i->pvals=Rmalloc(numpvals*sizeof(RDApval));
		for(x=0,f=pvals,t=i->pvals;x<numpvals;++x,++f,++t)
		{
			t->symbol_type=f->symbol_type;
			if(f->name!=NULL) t->name=stralloc(f->name);
				else t->name=NULL;
			t->type=f->type;
			if(f->fieldname!=NULL) t->fieldname=stralloc(f->fieldname);
				else t->fieldname=NULL;
		}
	} else {
		i->numpvals=0;
		i->pvals=NULL;
	}
	return(++tmp->numimages);
}
void xestablishconflds(RDAreport *tmp,char *modulename,char *filename,
	char *keyname,char *cmod,char *cfile,EZfield *holdfields,int numezf,
	short range_screen,int line,char *file)
{
	int y,z;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0,holdnum=0;
	RDAconfld *cf,*holdconfld=NULL;
	EZfield *ezf;
	char *filx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG establishconflds Establish connect fields for Module [%s] File [%s] on Report [%s] at line [%d] program [%s].",modulename,filename,tmp->name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!GETBIN(modulename,filename,&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
			fields=NULL;
		}
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->part!=NULL && !RDAstrcmp(keyx->name,keyname))
				{
					holdnum=keyx->numparts;
					holdconfld=Rmalloc(keyx->numparts*sizeof(RDAconfld));
					for(z=0,cf=holdconfld,part=keyx->part;z<keyx->numparts;
						++z,++cf,++part)
					{
						if(part->name!=NULL) 
						{
							cf->fldname=stralloc(part->name);
							Rfree(part->name);
						} else cf->fldname=NULL;
						cf->conname=NULL;
					}
					Rfree(keyx->part);
				}
				if(keyx->name!=NULL) Rfree(keyx->name);
			}
			Rfree(keys);
			keys=NULL;
		}
		if(holdconfld!=NULL)
		{
			if(cmod!=NULL && cfile!=NULL)
			{
				if(!GETBIN(cmod,cfile,&engine,&nofields,&fields,&nokeys,&keys)) 
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
								Rfree(keyx->part);
							}
						}
						Rfree(keys);
					}
					if(fields!=NULL)
					{
						for(y=0,f=fields;y<nofields;++y,++f)
						{
							for(z=0,cf=holdconfld;z<holdnum;++z,++cf)
							{
								if(!RDAstrcmp(cf->fldname,f->name)) break;
							}
							if(z<holdnum)
							{
								cf->conname=Rmalloc(RDAstrlen(cfile)+RDAstrlen(f->name)+5);
								sprintf(cf->conname,"[%s][%s]",cfile,f->name);
							}
							if(f->name!=NULL) Rfree(f->name);
						}
						Rfree(fields);
					}
					if(holdconfld!=NULL)
					{
						for(y=0,cf=holdconfld;y<holdnum;++y,++cf)
						{
							if(cf->conname==NULL ||
								*cf->conname==0)
							{
								for(z=0,ezf=holdfields;z<numezf;++z,++ezf)
								{
									if(!RDAstrcmp(ezf->name,cf->fldname))
									{
										filx=stripfilename(ezf->wholename);
										cf->conname=Rmalloc(RDAstrlen(filx)+RDAstrlen(ezf->name)+5);
										sprintf(cf->conname,"[%s][%s]",filx,ezf->name);
										if(filx!=NULL) Rfree(filx);
										filx=NULL;
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if(cmod!=NULL && cfile!=NULL) 
	{
		addsearchline(tmp,modulename,filename,1,cmod,cfile,holdnum,holdconfld,keyname,range_screen,NULL);
	} else {
		addsearchline(tmp,modulename,filename,0,NULL,NULL,0,NULL,keyname,range_screen,NULL);
	}
	if(holdconfld!=NULL)
	{
		for(y=0,cf=holdconfld;y<holdnum;++y,++cf)
		{
			if(cf->conname!=NULL) Rfree(cf->conname);
			if(cf->fldname!=NULL) Rfree(cf->fldname);
		}
		Rfree(holdconfld);
		holdconfld=NULL;
		holdnum=0;
	}
}
void makepageheader(RDAreport *tmp,RDAezrpt *ez)
{
	short longest=0,count=0;
	RDAimage *i=NULL;
	RDAezdtl *detail;
	RDApval *pval;
	int x,y;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG makepageheader Making Page Header for EZ-Report [%s].",ez->name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	i=Rmalloc(sizeof(RDAimage));
	longest=detail_length(ez);
	if(longest>81) tmp->display->dumprinter_type=TRUE;
	if(ez->print)
	{
	i->breakname=stralloc("PAGE HEADER");
	if(RDAstrlen(XPERT_SETUP->company_name)>RDAstrlen(ez->title))
	{
		if((RDAstrlen(XPERT_SETUP->company_name)+36)>longest) 
			longest=(RDAstrlen(XPERT_SETUP->company_name)+36);
	} else {
		if((RDAstrlen(ez->title)+36)>longest) 
			longest=(RDAstrlen(ez->title)+36);
	}
	if(longest<55) longest=55;
	i->line=Rmalloc(longest+4);
	i->numpvals=4;
	i->pvals=Rmalloc(4*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=0;
	pval->name=stralloc("[ORGANIZATION NAME]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=0;
	pval->name=stralloc("[PAGE NO]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	temp=Rmalloc(longest-29);
	memset(temp,'C',(longest-32));
	sprintf(i->line,"%*s @%*s Page: @RRRRRRR@@",15,"",(longest-32),temp);
	if(temp!=NULL) Rfree(temp);
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	if(i->line!=NULL) Rfree(i->line);
	i->line=Rmalloc(longest+4);
	i->numpvals=4;
	i->pvals=Rmalloc(4*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=0;
	pval->name=stralloc("[REPORT TITLE]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=0;
	pval->name=stralloc("[SYSTEM DATE]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	temp=Rmalloc(longest-29);
	memset(temp,'C',(longest-32));
	sprintf(i->line,"%*s @%*s Date: @LLLLLLL@@",15,"",(longest-32),temp);
	if(temp!=NULL) Rfree(temp);
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	if(i->line!=NULL) Rfree(i->line);
	i->line=Rmalloc(longest+4);
	i->numpvals=4;
	i->pvals=Rmalloc(4*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=0;
	pval->name=stralloc("[EXECUTED BY]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=0;
	pval->name=stralloc("[SYSTEM TIME]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	temp=Rmalloc(longest-29);
	memset(temp,'C',(longest-32));
	sprintf(i->line,"%*s @%*s Time: @LLLLLLL@@",15,"",(longest-32),temp);
	if(temp!=NULL) Rfree(temp);
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	if(i->line!=NULL) Rfree(i->line);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	i->line=Rmalloc(longest+4);
	sprintf(i->line,"@@");
	i->numpvals=2;
	i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,
		i->numpvals,i->pvals);
	if(i->line!=NULL) Rfree(i->line);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	i->numpvals=2;
	i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	if(i->line!=NULL) Rfree(i->line);
	i->line=Rmalloc(longest+4);
	memset(i->line,0,longest+4);
	for(y=0;y<2;++y)
	{
		count=0;
		x=0;
		while(x<ez->numdtl)
		{
			detail=ez->detail+x;
			if(detail->start<count) 
			{
				strcat(i->line,"@@");
				addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
				memset(i->line,0,longest+4);
				count=detail->start;
			} else count=detail->start;
			if(!y)
			{
				if(detail->title1!=NULL) temp=stralloc(detail->title1);
					else temp=NULL;
			} else {
				if(detail->title2!=NULL) temp=stralloc(detail->title2);
					else temp=NULL;
			}
			if(temp!=NULL)
			{
				if(RDAstrlen(temp)>detail->length) 
					temp[detail->length]=0;
			}
/* RIGHT JUSTIFY / LEFT JUSTIFY */
			switch(detail->fieldtype)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case BOOLNS:
				case CHARACTERS:
					sprintf(&i->line[RDAstrlen(i->line)],"%*s%-*s",(count-RDAstrlen(i->line)),"",detail->length,(temp!=NULL ? temp:""));
					break;
				case DOLLARS_NOCENTS:
				case DOLLARS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SHORTV:
				case SSHORTV:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					sprintf(&i->line[RDAstrlen(i->line)],"%*s%*s",(count-RDAstrlen(i->line)),"",detail->length,(temp!=NULL ? temp:""));
					break;
				default:
					prterr("Error field type [%d] is invalid for detail title #1 [%s] title #2 [%s].",detail->type,detail->title1,detail->title2);
					break;
			}
			count+=detail->length;
			++x;
		}
		strcat(i->line,"@@");
		addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
		memset(i->line,0,longest+4);
		count=0;
	}
	if(i->line!=NULL) Rfree(i->line);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	i->line=Rmalloc(longest+4);
	sprintf(i->line,"@@");
	i->numpvals=2;
	i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,
		i->numpvals,i->pvals);
	}
	if(i->line!=NULL) Rfree(i->line);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	if(i->breakname!=NULL) Rfree(i->breakname);
	if(i!=NULL) Rfree(i);
}
void change_name_desc(char **tmp)
{
	char *temp,*tmp1=NULL;
	int x;

	if(*tmp==NULL) return;
	for(x=0;x<(RDAstrlen(*tmp));++x)
	{
		temp=*tmp+x;
		if(!strncmp(temp,"CODE",4))
		{
			temp+=4;
			tmp1=Rmalloc(RDAstrlen("DESCRIPTION")+(temp!=NULL ? 
				RDAstrlen(temp):0)+1+x);
			if(x>0 && x<RDAstrlen(*tmp))
			{
				sprintf(tmp1,"%.*s%s%s",x,*tmp,"DESCRIPTION",
					(temp!=NULL ? temp:""));
			} else if(x>0 && x>=RDAstrlen(*tmp))
			{
				sprintf(tmp1,"%s%s",*tmp,"DESCRIPTION");
			} else if(x==0)
			{
				sprintf(tmp1,"%s%s","DESCRIPTION",(temp!=NULL ? temp:""));
			}
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc(tmp1);
			if(tmp1!=NULL) Rfree(tmp1);
			tmp1=NULL;
			break;
		}
	}
}
void change_name_label(char **tmp)
{
	char *temp,*tmp1=NULL;
	int x;

	if(*tmp==NULL) return;
	for(x=0;x<(RDAstrlen(*tmp));++x)
	{
		temp=*tmp+x;
		if(!strncmp(temp,"CODE",4))
		{
			temp+=4;
			tmp1=Rmalloc(RDAstrlen("LABEL")+(temp!=NULL ? 
				RDAstrlen(temp):0)+1+x);
			if(x>0 && x<RDAstrlen(*tmp))
			{
				sprintf(tmp1,"%.*s%s%s",x,*tmp,"LABEL",
					(temp!=NULL ? temp:""));
			} else if(x>0 && x>=RDAstrlen(*tmp))
			{
				sprintf(tmp1,"%s%s",*tmp,"LABEL");
			} else if(x==0)
			{
				sprintf(tmp1,"%s%s","LABEL",(temp!=NULL ? temp:""));
			}
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc(tmp1);
			if(tmp1!=NULL) Rfree(tmp1);
			tmp1=NULL;
			break;
		}
	}
}
static char *ReturnLabel(char *name)
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
void makeheader(RDAreport *tmp,RDAezrpt *ez,RDAezsrt *sort,int num)
{
	RDAimage *i=NULL;
	RDApval *pval;
	int x;
	short longest=0;
	char *temp=NULL,*label=NULL;
	char *pat=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG makeheader Make Header for [%s] on Report [%s].",sort->name,ez->name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	pat=stralloc("[DIMENSION #* CODE]");
	longest=detail_length(ez);
	i=Rmalloc(sizeof(RDAimage));
	i->breakname=Rmalloc(RDAstrlen(sort->name)+10);
	sprintf(i->breakname,"%s HEADER",sort->name);
	i->type=0;
	i->line=Rmalloc(longest+4);
	label=ReturnScreenLabel(sort->name);
	if(!patmatch(pat,sort->name))
	{
		if(num>0)
		{
			sprintf(i->line,"%*s%s @VVVV@@",num*2,"",label);
		} else sprintf(i->line,"%s @VVVV@@",label);
		if(label!=NULL) Rfree(label);
		i->numpvals=3;
		i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
		pval=i->pvals;
		pval->symbol_type=0;
		pval->name=stralloc(sort->name);
		pval->type=0;
		pval->fieldname=NULL;
		++pval;
		pval->symbol_type=1;
		pval->name=stralloc("CARRIAGE RETURN");
		pval->type=0;
		pval->fieldname=NULL;
		++pval;
		pval->symbol_type=1;
		pval->name=stralloc("LINEFEED");
		pval->type=0;
	} else {
		if(num>0)
		{
			sprintf(i->line,"%*s@VVV @VVVV @VVVV@@",num*2,"");
		} else sprintf(i->line,"@VVV @VVVV @VVVV@@");
		if(label!=NULL) Rfree(label);
		i->numpvals=5;
		i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
		pval=i->pvals;
		temp=stralloc(sort->name);
		change_name_label(&temp);
		pval->symbol_type=0;
		pval->name=stralloc(temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		pval->type=0;
		pval->fieldname=NULL;
		++pval;
		pval->symbol_type=0;
		pval->name=stralloc(sort->name);
		pval->type=0;
		pval->fieldname=NULL;
		++pval;
		temp=stralloc(sort->name);
		change_name_desc(&temp);
		pval->symbol_type=0;
		pval->name=stralloc(temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		pval->type=0;
		pval->fieldname=NULL;
		++pval;
		pval->symbol_type=1;
		pval->name=stralloc("CARRIAGE RETURN");
		pval->type=0;
		pval->fieldname=NULL;
		++pval;
		pval->symbol_type=1;
		pval->name=stralloc("LINEFEED");
		pval->type=0;
	}
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	if(i->line!=NULL) Rfree(i->line);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	if(i->breakname!=NULL) Rfree(i->breakname);
	if(i!=NULL) Rfree(i);
	if(pat!=NULL) Rfree(pat);
}
void makedetailline(RDAreport *tmp,RDAezrpt *ez)
{
	RDAezdtl *detail;
	short longest=0,count=0,virtual_count=0;
	RDAimage *i=NULL;
	RDApval *pval;
	int x,y,z;
	char *temp=NULL,*temp1=NULL;
	RDAezsrt *lastsort=NULL,*sort=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG makedetailline Make Detail Line(s) for EZ-Report [%s].",ez->name); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(ez->sort!=NULL)
	{
		for(z=0,sort=ez->sort;z<ez->numsrt;++z,++sort)
		{
			if(sort->total<3) lastsort=sort;
		}
	} 
	if(lastsort!=NULL)
	{
		i=Rmalloc(sizeof(RDAimage));
		i->breakname=Rmalloc(RDAstrlen(lastsort->name)+8);
		sprintf(i->breakname,"%s FOOTER",lastsort->name);
		i->line=NULL;
		i->type=0;
		i->pvals=NULL;
		longest=detail_length(ez);
		i->line=Rmalloc(longest+4);
		memset(i->line,0,longest+4);
		count=0;
		x=0;
		while(x<ez->numdtl)
		{
			detail=ez->detail+x;
			if(detail->start<count)
			{
				strcat(i->line,"@@");
				if(i->pvals!=NULL)
				{
					i->pvals=Rrealloc(i->pvals,(i->numpvals+2)*sizeof(RDApval));
				} else {
					i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
				}
				pval=i->pvals+i->numpvals;
				pval->symbol_type=1;
				pval->name=stralloc("CARRIAGE RETURN");
				pval->type=0;
				pval->fieldname=NULL;
				++pval;
				pval->symbol_type=1;
				pval->name=stralloc("LINEFEED");
				pval->type=0;
				pval->fieldname=NULL;
				i->numpvals+=2;
				addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
				memset(i->line,0,longest+4);
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->name!=NULL) Rfree(pval->name);
					if(pval->fieldname!=NULL) Rfree(pval->fieldname);
				}
				if(i->pvals!=NULL) Rfree(i->pvals);
				i->numpvals=0;
				i->pvals=NULL;
				count=detail->start;
			} else count=detail->start;
			temp=NULL;
			memset(stemp,0,101);
			switch(detail->fieldtype)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case BOOLNS:
				case CHARACTERS:
					temp=Rmalloc(detail->length);
					memset(temp,'L',detail->length-1);
					break;
				case DOLLARS_NOCENTS:
					temp=Rmalloc(detail->length);
					memset(stemp,'R',detail->length-2);
					sprintf(temp,",%s",stemp);
					memset(stemp,0,RDAstrlen(stemp));
					break;
				case DOLLARS:
					temp=Rmalloc(detail->length);
#ifndef NO_COMMAS
					memset(stemp,'R',detail->length-3);
					sprintf(temp,".,%s",stemp);
#endif 
#ifdef NO_COMMAS
					memset(stemp,'R',detail->length-2);
					sprintf(temp,".%s",stemp);
#endif

					memset(stemp,0,RDAstrlen(stemp));
					break;
				case DECIMALV:
				case SDECIMALV:
					temp=Rmalloc(detail->length);
					memset(stemp,0,101);
					if(detail->length>8)
					{
						memset(stemp,'R',detail->length-6);
						sprintf(temp,"%s.RRRR",stemp);
						memset(stemp,0,RDAstrlen(stemp));
					} else if(detail->length>3)
					{
						memset(stemp,'R',detail->length-3);
						sprintf(temp,"%s.R",stemp);
						memset(stemp,0,RDAstrlen(stemp));
					} else {
						memset(temp,'R',detail->length-1);
					}
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case SHORTV:
				case SSHORTV:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp=Rmalloc(detail->length);
					memset(temp,'R',detail->length-1);
					break;
				default:
					prterr("Error field type [%d] is invalid for detail title #1 [%s] title #2 [%s].",detail->type,detail->title1,detail->title2);
					break;
			}
			sprintf(&i->line[RDAstrlen(i->line)],"%*s@%s",count-RDAstrlen(i->line),"",(temp!=NULL ? temp:""));
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			if(i->pvals!=NULL)
			{
				i->pvals=Rrealloc(i->pvals,(i->numpvals+1)*sizeof(RDApval));
			} else {
				i->pvals=Rmalloc((i->numpvals+1)*sizeof(RDApval));
			}
			pval=i->pvals+i->numpvals;
			pval->symbol_type=0;
			if(!detail->type)
			{
				temp1=stralloc(detail->fieldname);
				ReplaceBrackets(temp1);
				switch(detail->fieldtype)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case BOOLNS:
					case CHARACTERS:
						temp=Rmalloc(RDAstrlen(temp1)+10);
						sprintf(temp,"[LAST %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
						temp=Rmalloc(RDAstrlen(temp1)+11);
						sprintf(temp,"[TOTAL %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
					case SSHORTV:
						temp=Rmalloc(RDAstrlen(temp1)+10);
						sprintf(temp,"[LAST %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						temp=Rmalloc(RDAstrlen(temp1)+10);
						sprintf(temp,"[LAST %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					default:
						prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
/* switch to get accumulator name */
				sprintf(stemp,"[VIRTUAL FIELD #%d]",virtual_count++);
				ReplaceBrackets(stemp);
				switch(detail->fieldtype)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case BOOLNS:
					case CHARACTERS:
						temp=Rmalloc(RDAstrlen(stemp)+10);
						sprintf(temp,"[LAST %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
						temp=Rmalloc(RDAstrlen(stemp)+11);
						sprintf(temp,"[TOTAL %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
					case SSHORTV:
						temp=Rmalloc(RDAstrlen(stemp)+10);
						sprintf(temp,"[LAST %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						temp=Rmalloc(RDAstrlen(stemp)+10);
						sprintf(temp,"[LAST %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					default:
						prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
				}
				memset(stemp,0,RDAstrlen(stemp));
			}
			pval->type=0;
			pval->fieldname=NULL;
			++i->numpvals;
			count+=detail->length;
			++x;
		}
	} else {
		i=Rmalloc(sizeof(RDAimage));
		i->breakname=stralloc("DETAIL LINES");
		i->line=NULL;
		i->type=0;
		i->pvals=NULL;
		longest=detail_length(ez);
		i->line=Rmalloc(longest+4);
		memset(i->line,0,longest+4);
		count=0;
		x=0;
		while(x<ez->numdtl)
		{
			detail=ez->detail+x;
			if(detail->start<count)
			{
				strcat(i->line,"@@");
				if(i->pvals!=NULL)
				{
					i->pvals=Rrealloc(i->pvals,(i->numpvals+2)*sizeof(RDApval));
				} else {
					i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
				}
				pval=i->pvals+i->numpvals;
				pval->symbol_type=1;
				pval->name=stralloc("CARRIAGE RETURN");
				pval->type=0;
				pval->fieldname=NULL;
				++pval;
				pval->symbol_type=1;
				pval->name=stralloc("LINEFEED");
				pval->type=0;
				pval->fieldname=NULL;
				i->numpvals+=2;
				addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
				memset(i->line,0,longest+4);
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->name!=NULL) Rfree(pval->name);
					if(pval->fieldname!=NULL) Rfree(pval->fieldname);
				}
				if(i->pvals!=NULL) Rfree(i->pvals);
				i->numpvals=0;
				i->pvals=NULL;
				count=detail->start;
			} else count=detail->start;
			temp=NULL;
			memset(stemp,0,101);
			switch(detail->fieldtype)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case BOOLNS:
				case CHARACTERS:
					temp=Rmalloc(detail->length);
					memset(temp,'L',detail->length-1);
					break;
				case DOLLARS_NOCENTS:
					temp=Rmalloc(detail->length);
					memset(stemp,'R',detail->length-2);
					sprintf(temp,",%s",stemp);
					memset(stemp,0,RDAstrlen(stemp));
					break;
				case DOLLARS:
					temp=Rmalloc(detail->length);
#ifndef NO_COMMAS
					memset(stemp,'R',detail->length-3);
					sprintf(temp,".,%s",stemp);
#endif 
#ifdef NO_COMMAS
					memset(stemp,'R',detail->length-2);
					sprintf(temp,".%s",stemp);
#endif
					memset(stemp,0,RDAstrlen(stemp));
					break;
				case DECIMALV:
				case SDECIMALV:
					temp=Rmalloc(detail->length);
					memset(stemp,0,101);
					if(detail->length>8)
					{
						memset(stemp,'R',detail->length-6);
						sprintf(temp,"%s.RRRR",stemp);
						memset(stemp,0,RDAstrlen(stemp));
					} else if(detail->length>3)
					{
						memset(stemp,'R',detail->length-3);
						sprintf(temp,"%s.R",stemp);
						memset(stemp,0,RDAstrlen(stemp));
					} else {
						memset(temp,'R',detail->length-1);
					}
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case SHORTV:
				case SSHORTV:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp=Rmalloc(detail->length);
					memset(temp,'R',detail->length-1);
					break;
				default:
					prterr("Error field type [%d] is invalid for detail title #1 [%s] title #2 [%s].",detail->type,detail->title1,detail->title2);
					break;
			}
			sprintf(&i->line[RDAstrlen(i->line)],"%*s@%s",count-RDAstrlen(i->line),"",(temp!=NULL ? temp:""));
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			if(i->pvals!=NULL)
			{
				i->pvals=Rrealloc(i->pvals,(i->numpvals+1)*sizeof(RDApval));
			} else {
				i->pvals=Rmalloc((i->numpvals+1)*sizeof(RDApval));
			}
			pval=i->pvals+i->numpvals;
			pval->symbol_type=0;
			if(!detail->type)
			{
				if(detail->fieldname!=NULL) pval->name=stralloc(detail->fieldname); else pval->name=NULL;
			} else {
				sprintf(stemp,"[VIRTUAL FIELD #%d]",virtual_count++);
				pval->name=stralloc(stemp);
				memset(stemp,0,RDAstrlen(stemp));
			}
			pval->type=0;
			pval->fieldname=NULL;
			++i->numpvals;
			count+=detail->length;
			++x;
		}
	}
	strcat(i->line,"@@");
	if(i->pvals!=NULL)
	{
		i->pvals=Rrealloc(i->pvals,(i->numpvals+2)*sizeof(RDApval));
	} else {
		i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
		i->numpvals=0;
	}
	pval=i->pvals+i->numpvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	i->numpvals+=2;
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	memset(i->line,0,longest+4);
	if(i->line!=NULL) Rfree(i->line);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	if(i->breakname!=NULL) Rfree(i->breakname);
	if(i!=NULL) Rfree(i);
}
void makefooters(RDAreport *tmp,RDAezrpt *ez)
{
	RDAezdtl *detail;
	short longest=0,count=0,virtual_count=0;
	short start_footer=0,end_footer=0;
	RDAimage *i=NULL;
	RDApval *pval;
	int x,z,y;
	char *temp=NULL,printed=FALSE,*temp1=NULL;
	RDAezsrt *sort,*lastsort=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG makefooters Make Footers for EZ-Report [%s].",ez->name);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	i=Rmalloc(sizeof(RDAimage));
	longest=detail_length(ez);
/* FROM HERE */
	if(ez->sort!=NULL)
	{
	if(ez->numsrt>0) lastsort=ez->sort+(ez->numsrt-1);
	for(z=ez->numsrt;z>0;--z)
	{
		sort=ez->sort+z-1;
		if(sort->total<3)
		{
			if((sort->total==1 || sort->total==2) &&
				z<(ez->numsrt)) 
			{
				start_footer=0;
				temp=Rmalloc(RDAstrlen(sort->name)+8);
				sprintf(temp,"%s FOOTER",sort->name);
				i->breakname=stralloc(temp);
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				i->line=NULL;
				i->type=0;
				i->pvals=NULL;
				i->numpvals=0;
				i->line=Rmalloc(longest+4);
				memset(i->line,0,longest+4);
				count=0;
				virtual_count=0;
				x=0;
				while(x<ez->numdtl)
				{
					detail=ez->detail+x;
				
					if(detail->start<count)
					{
						strcat(i->line,"@@");
						if(i->pvals!=NULL)
						{
							i->pvals=Rrealloc(i->pvals,(i->numpvals+2)*sizeof(RDApval));
						} else {
							i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
						}
						pval=i->pvals+i->numpvals;
						pval->symbol_type=1;
						pval->name=stralloc("CARRIAGE RETURN");
						pval->type=0;
						pval->fieldname=NULL;
						++pval;
						pval->symbol_type=1;
						pval->name=stralloc("LINEFEED");
						pval->type=0;
						pval->fieldname=NULL;
						i->numpvals+=2;
						addimageline(tmp,i->breakname,i->line,FALSE,NULL,
							i->numpvals,i->pvals);
						memset(i->line,0,longest+4);
						count=detail->start;
						for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
						{
							if(pval->name!=NULL) Rfree(pval->name);
							if(pval->fieldname!=NULL) Rfree(pval->fieldname);
						}
						if(i->pvals!=NULL) Rfree(i->pvals);
						i->pvals=NULL;
						i->numpvals=0;
						memset(i->line,0,longest+4);
					} else count=detail->start;
					temp=NULL;
					/*  Removed by DDC (08/03/2004): I don't like the look of having the keys show up on a different break level.  Also, some room has to be made for the break field to be displayed on the image line.
					if(detail->type) CompareName=detail->expression;
						else CompareName=detail->fieldname;
					for(y=0,testsort=ez->sort;y<ez->numsrt;++y,++testsort)
					{
						if(testsort->total<3)
						{
							if(!RDAstrcmp(testsort->name,CompareName) && RDAstrcmp(CompareName,lastsort->name))
							{
								break;
							}
						}
					}
					if((y<ez->numsrt && ez->numsrt!=0) ||
						detail->fieldtype==DOLLARS || 
						detail->fieldtype==DOLLARS_NOCENTS
						|| detail->fieldtype==DECIMALV 
						|| detail->fieldtype==SDECIMALV 
						|| detail->fieldtype==DOUBLEV
						|| detail->fieldtype==SDOUBLEV)
					*/
					if(detail->fieldtype==DOLLARS || 
						detail->fieldtype==DOLLARS_NOCENTS
						|| detail->fieldtype==DECIMALV 
						|| detail->fieldtype==SDECIMALV 
						|| detail->fieldtype==DOUBLEV
						|| detail->fieldtype==SDOUBLEV)
					{
						printed=TRUE;
						temp=Rmalloc(detail->length+1);
						if(temp!=NULL) memset(temp,'-',detail->length);
						sprintf(&i->line[RDAstrlen(i->line)],"%*s%-*s",count-RDAstrlen(i->line),"",detail->length,(temp!=NULL ? temp:""));
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else {
						sprintf(&i->line[RDAstrlen(i->line)],"%*s%-*s",count-RDAstrlen(i->line),"",detail->length,"");
					}
					count+=detail->length;
					++x;
					if(detail->type) ++virtual_count;
				}
				strcat(i->line,"@@");
				if(i->pvals!=NULL)
				{
					i->pvals=Rrealloc(i->pvals,
						(i->numpvals+2)*sizeof(RDApval));
				} else {
					i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
				}
				pval=i->pvals+i->numpvals;
				pval->symbol_type=1;
				pval->name=stralloc("CARRIAGE RETURN");
				pval->type=0;
				pval->fieldname=NULL;
				++pval;
				pval->symbol_type=1;
				pval->name=stralloc("LINEFEED");
				pval->type=0;
				pval->fieldname=NULL;
				i->numpvals+=2;
				addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
				memset(i->line,0,longest+4);
				i->type=0;
				i->pvals=NULL;
				i->numpvals=0;
				count=0;
				virtual_count=0;
				x=0;
				while(x<ez->numdtl)
				{
					detail=ez->detail+x;
					if(detail->start<count)
					{
						strcat(i->line,"@@");
						if(i->pvals!=NULL)
						{
							i->pvals=Rrealloc(i->pvals,(i->numpvals+2)*sizeof(RDApval));
						} else {
							i->numpvals=0;
							i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
						}
						pval=i->pvals+i->numpvals;
						pval->symbol_type=1;
						pval->name=stralloc("CARRIAGE RETURN");
						pval->type=0;
						pval->fieldname=NULL;
						++pval;
						pval->symbol_type=1;
						pval->name=stralloc("LINEFEED");
						pval->type=0;
						pval->fieldname=NULL;
						i->numpvals+=2;
						addimageline(tmp,i->breakname,i->line,FALSE,NULL,
							i->numpvals,i->pvals);
						memset(i->line,0,longest+4);
						count=detail->start;
						for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
						{
							if(pval->name!=NULL) Rfree(pval->name);
							if(pval->fieldname!=NULL) Rfree(pval->fieldname);
						}
						if(i->pvals!=NULL) Rfree(i->pvals);
						i->pvals=NULL;
						i->numpvals=0;
						count=detail->start;
						if(printed)
						{
							sprintf(i->line,"@@");
							i->numpvals=2;
							i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
							pval=i->pvals;
							pval->symbol_type=1;
							pval->name=stralloc("CARRIAGE RETURN");
							pval->type=0;
							pval->fieldname=NULL;
							++pval;
							pval->symbol_type=1;
							pval->name=stralloc("LINEFEED");
							pval->type=0;
							addimageline(tmp,i->breakname,i->line,FALSE,NULL,
								i->numpvals,i->pvals);
							memset(i->line,0,longest+4);
						}
						memset(i->line,0,longest+4);
						if(i->pvals!=NULL)
						{
							for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
							{
								if(pval->name!=NULL) Rfree(pval->name);
								if(pval->fieldname!=NULL) 
									Rfree(pval->fieldname);
							}
							Rfree(i->pvals);
							i->pvals=NULL;
							i->numpvals=0;
						}
					} else count=detail->start;
					temp=NULL;
					/* DDC (08/03/2004) See above comment.  If I don't want the dashes, I don't want the fields under them, either.
					if(detail->type) CompareName=detail->expression;
						else CompareName=detail->fieldname;
					for(y=0,testsort=ez->sort;y<ez->numsrt;++y,++testsort)
					{
						if(testsort->total<3)
						{
							if(!RDAstrcmp(testsort->name,CompareName) && RDAstrcmp(CompareName,lastsort->name))
							{
								break;
							}
						}
					}
					if((y<ez->numsrt && ez->numsrt!=0) ||
						detail->fieldtype==DOLLARS || 
						detail->fieldtype==DOLLARS_NOCENTS
						|| detail->fieldtype==DECIMALV 
						|| detail->fieldtype==SDECIMALV 
						|| detail->fieldtype==DOUBLEV
						|| detail->fieldtype==SDOUBLEV)
					*/
					if(detail->fieldtype==DOLLARS || 
						detail->fieldtype==DOLLARS_NOCENTS
						|| detail->fieldtype==DECIMALV 
						|| detail->fieldtype==SDECIMALV 
						|| detail->fieldtype==DOUBLEV
						|| detail->fieldtype==SDOUBLEV)
					{
						/* START ADDITION - DDC (08/03/2004) */
						if(start_footer==0)
						{
							start_footer=count; /* or detail->start */
							temp1=ReturnLabel(sort->name);
							memset(stemp,0,101);
							switch(sort->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case BOOLNS:
								case CHARACTERS:
									temp=Rmalloc(sort->length);
									memset(temp,'L',sort->length-1);
									break;
								case DOLLARS:
									temp=Rmalloc(sort->length);
#ifndef NO_COMMAS
									memset(stemp,'R',sort->length-3);
									sprintf(temp,".,%s",stemp);
#endif
#ifdef NO_COMMAS
									memset(stemp,'R',sort->length-2);
									sprintf(temp,".%s",stemp);
#endif
									memset(stemp,0,RDAstrlen(stemp));
									break;
								case DOLLARS_NOCENTS:
									temp=Rmalloc(sort->length);
									memset(stemp,'R',sort->length-2);
									sprintf(temp,",%s",stemp);
									memset(stemp,0,RDAstrlen(stemp));
									break;
								case DECIMALV:
								case SDECIMALV:
									temp=Rmalloc(sort->length);
									memset(stemp,0,101);
									if(sort->length>8)
									{
										memset(stemp,'R',sort->length-6);
										sprintf(temp,"%s.RRRR",stemp);
										memset(stemp,0,RDAstrlen(stemp));
									} else if(sort->length>3)
									{
										memset(stemp,'R',sort->length-3);
										sprintf(temp,"%s.R",stemp);
										memset(stemp,0,RDAstrlen(stemp));
									} else {
										memset(temp,'R',sort->length-1);
									}
									break;
								case DOUBLEV:
								case SDOUBLEV:
								case SHORTV:
								case SSHORTV:
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									temp=Rmalloc(sort->length);
									memset(temp,'R',sort->length-1);
									break;
								default:
									prterr("Error field type [%d] is invalid for sort field named [%s].",sort->type,(temp1!=NULL ? temp1:""));
									break;
							}
							if(start_footer>((RDAstrlen(temp1))+4+sort->length))
							{
								start_footer=count-((RDAstrlen(temp1))+4+sort->length);
								sprintf(&i->line[start_footer],"%s%s@%s%s",(temp1!=NULL ? temp1:"")," ",(temp!=NULL ? temp:"")," : ");
							} else if(start_footer>=(3+sort->length)) {
								start_footer=count-(3+sort->length);
								sprintf(&i->line[start_footer],"@%s%s",(temp!=NULL ? temp:"")," : ");
							} else {
								start_footer=(-1);
							}
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
							if(temp1!=NULL) Rfree(temp1);
							if(start_footer!=(-1))
							{
								if(i->pvals!=NULL)
								{
									i->pvals=Rrealloc(i->pvals,(i->numpvals+1)*sizeof(RDApval));
								} else {
									i->numpvals=0;
									i->pvals=Rmalloc((i->numpvals+1)*sizeof(RDApval));
								}
								pval=i->pvals+i->numpvals;
								pval->symbol_type=0;
								temp1=stralloc(sort->name);
								ReplaceBrackets(temp1);
								switch(sort->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case ZIPCODE:
									case PHONE:
									case CUSTOMTYPE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case OPTIONALFIELDS:
									case BOOLNS:
									case CHARACTERS:
										temp=Rmalloc(RDAstrlen(temp1)+10);
										sprintf(temp,"[LAST %s]",temp1);
										pval->name=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case DECIMALV:
									case SDECIMALV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DOUBLEV:
									case SDOUBLEV:
										temp=Rmalloc(RDAstrlen(temp1)+11);
										sprintf(temp,"[TOTAL %s]",temp1);
										pval->name=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case SHORTV:
									case SSHORTV:
										temp=Rmalloc(RDAstrlen(temp1)+10);
										sprintf(temp,"[LAST %s]",temp1);
										pval->name=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
										break;
									case SCROLLEDLIST:
									case LONGV:
									case SLONGV:
										temp=Rmalloc(RDAstrlen(temp1)+10);
										sprintf(temp,"[LAST %s]",temp1);
										pval->name=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
										break;
									default:
										prterr("Error field type [%d] is invalid for sort field named [%s].",sort->type,(temp1!=NULL ? temp1:""));
								}
								if(temp1!=NULL) Rfree(temp1);
								memset(stemp,0,RDAstrlen(stemp));
								pval->type=0;
								pval->fieldname=NULL;
								++i->numpvals;
							}
						}
						/* END OF ADDITION - DDC (08/03/2004) */
						printed=TRUE;
						memset(stemp,0,101);
						switch(detail->fieldtype)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case BOOLNS:
							case CHARACTERS:
								temp=Rmalloc(detail->length);
								memset(temp,'L',detail->length-1);
								break;
							case DOLLARS:
								temp=Rmalloc(detail->length);
#ifndef NO_COMMAS
								memset(stemp,'R',detail->length-3);
								sprintf(temp,".,%s",stemp);
#endif
#ifdef NO_COMMAS
								memset(stemp,'R',detail->length-2);
								sprintf(temp,".%s",stemp);
#endif
								memset(stemp,0,RDAstrlen(stemp));
								break;
							case DOLLARS_NOCENTS:
								temp=Rmalloc(detail->length);
								memset(stemp,'R',detail->length-2);
								sprintf(temp,",%s",stemp);
								memset(stemp,0,RDAstrlen(stemp));
								break;
							case DECIMALV:
							case SDECIMALV:
								temp=Rmalloc(detail->length);
								memset(stemp,0,101);
								if(detail->length>8)
								{
									memset(stemp,'R',detail->length-6);
									sprintf(temp,"%s.RRRR",stemp);
									memset(stemp,0,RDAstrlen(stemp));
								} else if(detail->length>3)
								{
									memset(stemp,'R',detail->length-3);
									sprintf(temp,"%s.R",stemp);
									memset(stemp,0,RDAstrlen(stemp));
								} else {
									memset(temp,'R',detail->length-1);
								}
								break;
							case DOUBLEV:
							case SDOUBLEV:
							case SHORTV:
							case SSHORTV:
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								temp=Rmalloc(detail->length);
								memset(temp,'R',detail->length-1);
								break;
							default:
								prterr("Error field type [%d] is invalid for detail line title #1 [%s] title #2.",detail->type,detail->title1,detail->title2);
								break;
						}
						sprintf(&i->line[RDAstrlen(i->line)],"%*s@%-*s",count-RDAstrlen(i->line),"",detail->length-1,(temp!=NULL ? temp:""));
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						if(i->pvals!=NULL)
						{
							i->pvals=Rrealloc(i->pvals,(i->numpvals+1)*sizeof(RDApval));
						} else {
							i->numpvals=0;
							i->pvals=Rmalloc((i->numpvals+1)*sizeof(RDApval));
						}
						pval=i->pvals+i->numpvals;
						pval->symbol_type=0;
						if(!detail->type)
						{
							temp1=stralloc(detail->fieldname);
							ReplaceBrackets(temp1);
							switch(detail->fieldtype)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case BOOLNS:
								case CHARACTERS:
									temp=Rmalloc(RDAstrlen(temp1)+10);
									sprintf(temp,"[LAST %s]",temp1);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								case DECIMALV:
								case SDECIMALV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DOUBLEV:
								case SDOUBLEV:
									temp=Rmalloc(RDAstrlen(temp1)+11);
									sprintf(temp,"[TOTAL %s]",temp1);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								case SHORTV:
								case SSHORTV:
									temp=Rmalloc(RDAstrlen(temp1)+10);
									sprintf(temp,"[LAST %s]",temp1);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									temp=Rmalloc(RDAstrlen(temp1)+10);
									sprintf(temp,"[LAST %s]",temp1);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								default:
									prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
							}
							if(temp1!=NULL) Rfree(temp1);
						} else {
/* switch to get accumulator name */
							sprintf(stemp,"[VIRTUAL FIELD #%d]",virtual_count);
							ReplaceBrackets(stemp);
							switch(detail->fieldtype)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case CUSTOMTYPE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case BOOLNS:
								case CHARACTERS:
									temp=Rmalloc(RDAstrlen(stemp)+10);
									sprintf(temp,"[LAST %s]",stemp);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								case DECIMALV:
								case SDECIMALV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DOUBLEV:
								case SDOUBLEV:
									temp=Rmalloc(RDAstrlen(stemp)+11);
									sprintf(temp,"[TOTAL %s]",stemp);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								case SHORTV:
								case SSHORTV:
									temp=Rmalloc(RDAstrlen(stemp)+10);
									sprintf(temp,"[LAST %s]",stemp);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									temp=Rmalloc(RDAstrlen(stemp)+10);
									sprintf(temp,"[LAST %s]",stemp);
									pval->name=stralloc(temp);
									if(temp!=NULL) Rfree(temp);
									break;
								default:
									prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
							}
							memset(stemp,0,RDAstrlen(stemp));
						}
						pval->type=0;
						pval->fieldname=NULL;
						++i->numpvals;
						end_footer=count+detail->length;
					} else {
						sprintf(&i->line[RDAstrlen(i->line)],"%*s%-*s",count-RDAstrlen(i->line),"",detail->length,"");
					}
					count+=detail->length;
					++x;
					if(detail->type) ++virtual_count;
				}
				/* START ADDITION - DDC (08/04/2004) */
				if(start_footer==(-1))
				{
					start_footer=count; /* or detail->start */
					temp1=ReturnLabel(sort->name);
					memset(stemp,0,101);
					switch(sort->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case BOOLNS:
						case CHARACTERS:
							temp=Rmalloc(sort->length);
							memset(temp,'L',sort->length-1);
							break;
						case DOLLARS:
							temp=Rmalloc(sort->length);
#ifndef NO_COMMAS
							memset(stemp,'R',sort->length-3);
							sprintf(temp,".,%s",stemp);
#endif
#ifdef NO_COMMAS
							memset(stemp,'R',sort->length-2);
							sprintf(temp,".%s",stemp);
#endif
							memset(stemp,0,RDAstrlen(stemp));
							break;
						case DOLLARS_NOCENTS:
							temp=Rmalloc(sort->length);
							memset(stemp,'R',sort->length-2);
							sprintf(temp,",%s",stemp);
							memset(stemp,0,RDAstrlen(stemp));
							break;
						case DECIMALV:
						case SDECIMALV:
							temp=Rmalloc(sort->length);
							memset(stemp,0,101);
							if(sort->length>8)
							{
								memset(stemp,'R',sort->length-6);
								sprintf(temp,"%s.RRRR",stemp);
								memset(stemp,0,RDAstrlen(stemp));
							} else if(sort->length>3)
							{
								memset(stemp,'R',sort->length-3);
								sprintf(temp,"%s.R",stemp);
								memset(stemp,0,RDAstrlen(stemp));
							} else {
								memset(temp,'R',sort->length-1);
							}
							break;
						case DOUBLEV:
						case SDOUBLEV:
						case SHORTV:
						case SSHORTV:
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							temp=Rmalloc(sort->length);
							memset(temp,'R',sort->length-1);
							break;
						default:
							prterr("Error field type [%d] is invalid for sort field named [%s].",sort->type,(temp1!=NULL ? temp1:""));
							break;
					}
					if((end_footer+4+(RDAstrlen(temp1))+sort->length)<longest)
					{
						sprintf(&i->line[end_footer],"%s%s%s@%s"," : ",(temp1!=NULL ? temp1:"")," ",(temp!=NULL ? temp:""));
					} else if((end_footer+sort->length+3)<=longest) {
						sprintf(&i->line[end_footer],"%s@%s"," : ",(temp!=NULL ? temp:""));
					/*
					} else {
						sprintf(&i->line[end_footer],"%s@%-*s"," : ",(longest-3-end_footer),(temp!=NULL ? temp:""));
					*/
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					if(temp1!=NULL) Rfree(temp1);
					if(start_footer!=(-1))
					{
						if(i->pvals!=NULL)
						{
							i->pvals=Rrealloc(i->pvals,(i->numpvals+1)*sizeof(RDApval));
						} else {
							i->numpvals=0;
							i->pvals=Rmalloc((i->numpvals+1)*sizeof(RDApval));
						}
						pval=i->pvals+i->numpvals;
						pval->symbol_type=0;
						temp1=stralloc(sort->name);
						ReplaceBrackets(temp1);
						switch(sort->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case PLAINTEXT:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case BOOLNS:
							case CHARACTERS:
								temp=Rmalloc(RDAstrlen(temp1)+10);
								sprintf(temp,"[LAST %s]",temp1);
								pval->name=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DOUBLEV:
							case SDOUBLEV:
								temp=Rmalloc(RDAstrlen(temp1)+11);
								sprintf(temp,"[TOTAL %s]",temp1);
								pval->name=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
								break;
							case SHORTV:
							case SSHORTV:
								temp=Rmalloc(RDAstrlen(temp1)+10);
								sprintf(temp,"[LAST %s]",temp1);
								pval->name=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								temp=Rmalloc(RDAstrlen(temp1)+10);
								sprintf(temp,"[LAST %s]",temp1);
								pval->name=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
								break;
							default:
								prterr("Error field type [%d] is invalid for sort field named [%s].",sort->type,(temp1!=NULL ? temp1:""));
						}
						if(temp1!=NULL) Rfree(temp1);
						memset(stemp,0,RDAstrlen(stemp));
						pval->type=0;
						pval->fieldname=NULL;
						++i->numpvals;
					}
				}
				/* END OF ADDITION - DDC (08/04/2004) */
				if(sort->total!=2)
				{
					strcat(i->line,"@@");
					if(i->pvals!=NULL)
					{
						i->pvals=Rrealloc(i->pvals,
							(i->numpvals+2)*sizeof(RDApval));
					} else {
						i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
					}
					pval=i->pvals+i->numpvals;
					pval->symbol_type=1;
					pval->name=stralloc("CARRIAGE RETURN");
					pval->type=0;
					pval->fieldname=NULL;
					++pval;
					pval->symbol_type=1;
					pval->name=stralloc("LINEFEED");
					pval->type=0;
					pval->fieldname=NULL;
					i->numpvals+=2;
				} else {
					strcat(i->line,"@@@");
					if(i->pvals!=NULL)
					{
						i->pvals=Rrealloc(i->pvals,
							(i->numpvals+3)*sizeof(RDApval));
					} else {
						i->pvals=Rmalloc((i->numpvals+3)*sizeof(RDApval));
					}
					pval=i->pvals+i->numpvals;
					pval->symbol_type=1;
					pval->name=stralloc("CARRIAGE RETURN");
					pval->type=0;
					pval->fieldname=NULL;
					++pval;
					pval->symbol_type=1;
					pval->name=stralloc("LINEFEED");
					pval->type=0;
					pval->fieldname=NULL;
					++pval;
					pval->symbol_type=1;
					pval->name=stralloc("FORMFEED");
					pval->type=0;
					pval->fieldname=NULL;
					i->numpvals+=3;
				}
				addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,
					i->pvals);
				memset(i->line,0,longest+4);
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->name!=NULL) Rfree(pval->name);
					if(pval->fieldname!=NULL) Rfree(pval->fieldname);
				}
				if(i->pvals!=NULL) Rfree(i->pvals);
				i->pvals=NULL;
				i->numpvals=0;
				if(printed)
				{
					sprintf(i->line,"@@");
					i->numpvals=2;
					i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
					pval=i->pvals;
					pval->symbol_type=1;
					pval->name=stralloc("CARRIAGE RETURN");
					pval->type=0;
					pval->fieldname=NULL;
					++pval;
					pval->symbol_type=1;
					pval->name=stralloc("LINEFEED");
					pval->type=0;
					addimageline(tmp,i->breakname,i->line,FALSE,NULL,
						i->numpvals,i->pvals);
				}
				if(i->line!=NULL) Rfree(i->line);
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->name!=NULL) Rfree(pval->name);
					if(pval->fieldname!=NULL) Rfree(pval->fieldname);
				}
				if(i->pvals!=NULL) Rfree(i->pvals);
				i->pvals=NULL;
				i->numpvals=0;
				if(i->breakname!=NULL) Rfree(i->breakname);
			}
		}
	}
		if(i->line!=NULL) Rfree(i->line);
		if(i->breakname!=NULL) Rfree(i->breakname);
	}
/* TO HERE */

	i->breakname=stralloc("REPORT FOOTER");
	if(temp!=NULL) Rfree(temp);
	temp=NULL;
	i->line=NULL;
	i->type=0;
	i->pvals=NULL;
	i->numpvals=0;
	i->line=Rmalloc(longest+4);
	memset(i->line,0,longest+4);
	count=0;
	virtual_count=0;
	x=0;
	while(x<ez->numdtl)
	{
		detail=ez->detail+x;
	
		if(detail->start<count)
		{
			strcat(i->line,"@@");
			if(i->pvals!=NULL)
			{
				i->pvals=Rrealloc(i->pvals,(i->numpvals+2)*sizeof(RDApval));
			} else {
				i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
			}
			pval=i->pvals+i->numpvals;
			pval->symbol_type=1;
			pval->name=stralloc("CARRIAGE RETURN");
			pval->type=0;
			pval->fieldname=NULL;
			++pval;
			pval->symbol_type=1;
			pval->name=stralloc("LINEFEED");
			pval->type=0;
			pval->fieldname=NULL;
			i->numpvals+=2;
			addimageline(tmp,i->breakname,i->line,FALSE,NULL,
				i->numpvals,i->pvals);
			memset(i->line,0,longest+4);
			count=detail->start;
			for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
			{
				if(pval->name!=NULL) Rfree(pval->name);
				if(pval->fieldname!=NULL) Rfree(pval->fieldname);
			}
			if(i->pvals!=NULL) Rfree(i->pvals);
			i->pvals=NULL;
			i->numpvals=0;
			memset(i->line,0,longest+4);
		} else count=detail->start;
		temp=NULL;
		if(detail->fieldtype==DOLLARS || 
			detail->fieldtype==DOLLARS_NOCENTS
			|| detail->fieldtype==DECIMALV 
			|| detail->fieldtype==SDECIMALV 
			|| detail->fieldtype==DOUBLEV
			|| detail->fieldtype==SDOUBLEV)
		{
			printed=TRUE;
			temp=Rmalloc(detail->length+1);
			if(temp!=NULL) memset(temp,'=',detail->length);
			sprintf(&i->line[RDAstrlen(i->line)],"%*s%-*s",count-RDAstrlen(i->line),"",detail->length,(temp!=NULL ? temp:""));
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
		} else {
			sprintf(&i->line[RDAstrlen(i->line)],"%*s%-*s",count-RDAstrlen(i->line),"",detail->length,"");
		}
		count+=detail->length;
		++x;
		if(detail->type) ++virtual_count;
	}
	strcat(i->line,"@@");
	if(i->pvals!=NULL)
	{
		i->pvals=Rrealloc(i->pvals,
			(i->numpvals+2)*sizeof(RDApval));
	} else {
		i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
	}
	pval=i->pvals+i->numpvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	i->numpvals+=2;
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	memset(i->line,0,longest+4);
	i->type=0;
	i->pvals=NULL;
	i->numpvals=0;
	count=0;
	virtual_count=0;
	x=0;
	while(x<ez->numdtl)
	{
		detail=ez->detail+x;
		if(detail->start<count)
		{
			strcat(i->line,"@@");
			if(i->pvals!=NULL)
			{
				i->pvals=Rrealloc(i->pvals,(i->numpvals+2)*sizeof(RDApval));
			} else {
				i->numpvals=0;
				i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
			}
			pval=i->pvals+i->numpvals;
			pval->symbol_type=1;
			pval->name=stralloc("CARRIAGE RETURN");
			pval->type=0;
			pval->fieldname=NULL;
			++pval;
			pval->symbol_type=1;
			pval->name=stralloc("LINEFEED");
			pval->type=0;
			pval->fieldname=NULL;
			i->numpvals+=2;
			
			addimageline(tmp,i->breakname,i->line,FALSE,NULL,
				i->numpvals,i->pvals);
			memset(i->line,0,longest+4);
			count=detail->start;
			for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
			{
				if(pval->name!=NULL) Rfree(pval->name);
				if(pval->fieldname!=NULL) Rfree(pval->fieldname);
			}
			if(i->pvals!=NULL) Rfree(i->pvals);
			i->pvals=NULL;
			i->numpvals=0;
			count=detail->start;
			if(printed)
			{
				sprintf(i->line,"@@");
				i->numpvals=2;
				i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
				pval=i->pvals;
				pval->symbol_type=1;
				pval->name=stralloc("CARRIAGE RETURN");
				pval->type=0;
				pval->fieldname=NULL;
				++pval;
				pval->symbol_type=1;
				pval->name=stralloc("LINEFEED");
				pval->type=0;
				addimageline(tmp,i->breakname,i->line,FALSE,NULL,
					i->numpvals,i->pvals);
				memset(i->line,0,longest+4);
			}
			memset(i->line,0,longest+4);
			if(i->pvals!=NULL)
			{
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->name!=NULL) Rfree(pval->name);
					if(pval->fieldname!=NULL) 
						Rfree(pval->fieldname);
				}
				Rfree(i->pvals);
				i->pvals=NULL;
				i->numpvals=0;
			}
		} else count=detail->start;
		temp=NULL;
		if(detail->fieldtype==DOLLARS || 
			detail->fieldtype==DOLLARS_NOCENTS
			|| detail->fieldtype==DECIMALV 
			|| detail->fieldtype==SDECIMALV 
			|| detail->fieldtype==DOUBLEV
			|| detail->fieldtype==SDOUBLEV)
		{
			printed=TRUE;
			memset(stemp,0,101);
			switch(detail->fieldtype)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case BOOLNS:
				case CHARACTERS:
					temp=Rmalloc(detail->length);
					memset(temp,'L',detail->length-1);
					break;
				case DOLLARS:
					temp=Rmalloc(detail->length);
#ifndef NO_COMMAS
					memset(stemp,'R',detail->length-3);
					sprintf(temp,".,%s",stemp);
#endif
#ifdef NO_COMMAS
					memset(stemp,'R',detail->length-2);
					sprintf(temp,".%s",stemp);
#endif
					memset(stemp,0,RDAstrlen(stemp));
					break;
				case DOLLARS_NOCENTS:
					temp=Rmalloc(detail->length);
					memset(stemp,'R',detail->length-2);
					sprintf(temp,",%s",stemp);
					memset(stemp,0,RDAstrlen(stemp));
					break;
				case DECIMALV:
				case SDECIMALV:
					temp=Rmalloc(detail->length);
					memset(stemp,0,101);
					if(detail->length>8)
					{
						memset(stemp,'R',detail->length-6);
						sprintf(temp,"%s.RRRR",stemp);
						memset(stemp,0,RDAstrlen(stemp));
					} else if(detail->length>3)
					{
						memset(stemp,'R',detail->length-3);
						sprintf(temp,"%s.R",stemp);
						memset(stemp,0,RDAstrlen(stemp));
					} else {
						memset(temp,'R',detail->length-1);
					}
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case SHORTV:
				case SSHORTV:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp=Rmalloc(detail->length);
					memset(temp,'R',detail->length-1);
					break;
				default:
					prterr("Error field type [%d] is invalid for detail line title #1 [%s] title #2.",detail->type,detail->title1,detail->title2);
					break;
			}
			sprintf(&i->line[RDAstrlen(i->line)],"%*s@%-*s",count-RDAstrlen(i->line),"",detail->length-1,(temp!=NULL ? temp:""));
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
			if(i->pvals!=NULL)
			{
				i->pvals=Rrealloc(i->pvals,(i->numpvals+1)*sizeof(RDApval));
			} else {
				i->numpvals=0;
				i->pvals=Rmalloc((i->numpvals+1)*sizeof(RDApval));
			}
			pval=i->pvals+i->numpvals;
			pval->symbol_type=0;
			if(!detail->type)
			{
				temp1=stralloc(detail->fieldname);
				ReplaceBrackets(temp1);
				switch(detail->fieldtype)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case BOOLNS:
					case CHARACTERS:
						temp=Rmalloc(RDAstrlen(temp1)+10);
						sprintf(temp,"[LAST %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
						temp=Rmalloc(RDAstrlen(temp1)+11);
						sprintf(temp,"[TOTAL %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
					case SSHORTV:
						temp=Rmalloc(RDAstrlen(temp1)+10);
						sprintf(temp,"[LAST %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						temp=Rmalloc(RDAstrlen(temp1)+10);
						sprintf(temp,"[LAST %s]",temp1);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					default:
						prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
/* switch to get accumulator name */
				sprintf(stemp,"[VIRTUAL FIELD #%d]",virtual_count);
				ReplaceBrackets(stemp);
				switch(detail->fieldtype)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case BOOLNS:
					case CHARACTERS:
						temp=Rmalloc(RDAstrlen(stemp)+10);
						sprintf(temp,"[LAST %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
						temp=Rmalloc(RDAstrlen(stemp)+11);
						sprintf(temp,"[TOTAL %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
					case SSHORTV:
						temp=Rmalloc(RDAstrlen(stemp)+10);
						sprintf(temp,"[LAST %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						temp=Rmalloc(RDAstrlen(stemp)+10);
						sprintf(temp,"[LAST %s]",stemp);
						pval->name=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						break;
					default:
						prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
				}
				memset(stemp,0,RDAstrlen(stemp));
			}
			pval->type=0;
			pval->fieldname=NULL;
			++i->numpvals;
		} else {
			sprintf(&i->line[RDAstrlen(i->line)],"%*s%-*s",count-RDAstrlen(i->line),"",detail->length,"");
		}
		count+=detail->length;
		++x;
		if(detail->type) ++virtual_count;
	}
	strcat(i->line,"@@");
	if(i->pvals!=NULL)
	{
		i->pvals=Rrealloc(i->pvals,
			(i->numpvals+2)*sizeof(RDApval));
	} else {
		i->pvals=Rmalloc((i->numpvals+2)*sizeof(RDApval));
	}
	pval=i->pvals+i->numpvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	i->numpvals+=2;
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,
		i->pvals);
	memset(i->line,0,longest+4);
	for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	i->pvals=NULL;
	i->numpvals=0;
	if(printed)
	{
		sprintf(i->line,"@@");
		i->numpvals=2;
		i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
		pval=i->pvals;
		pval->symbol_type=1;
		pval->name=stralloc("CARRIAGE RETURN");
		pval->type=0;
		pval->fieldname=NULL;
		++pval;
		pval->symbol_type=1;
		pval->name=stralloc("LINEFEED");
		pval->type=0;
		addimageline(tmp,i->breakname,i->line,FALSE,NULL,
			i->numpvals,i->pvals);
	}
	if(i->line!=NULL) Rfree(i->line);
	for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	i->pvals=NULL;
	i->numpvals=0;
	if(i->breakname!=NULL) Rfree(i->breakname);
	if(i->line!=NULL) Rfree(i->line);














	i->line=Rmalloc(longest+1);
	sprintf(i->line,"@@@");
	i->numpvals=3;
	i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("FORMFEED");
	pval->type=0;
	if(ez->print)
	{
		i->breakname=stralloc("PAGE FOOTER");
		addimageline(tmp,i->breakname,i->line,FALSE,NULL,
			i->numpvals,i->pvals);
		if(i->breakname!=NULL) Rfree(i->breakname);
		i->breakname=NULL;
	} else {
		i->breakname=stralloc("REPORT FOOTER");
		addimageline(tmp,i->breakname,i->line,FALSE,NULL,
			i->numpvals,i->pvals);
	}
	if(i->line!=NULL) Rfree(i->line);
	for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
	{
		if(pval->name!=NULL) Rfree(pval->name);
		if(pval->fieldname!=NULL) Rfree(pval->fieldname);
	}
	if(i->pvals!=NULL) Rfree(i->pvals);
	i->pvals=NULL;
	i->numpvals=0;
	if(i->breakname!=NULL) Rfree(i->breakname);
	if(i!=NULL) Rfree(i);
}
void SetDependentVirtuals(RDAreport *r,char *modx)
{
	int x,y,vc=0;
	RDAvirtual *v,*v1;
	APPlib *virnames=NULL;
	char *libx=NULL;
	DFvirtual *d=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG SetDependentVirtuals Find and Load Dependent Virtual Fields for Report [%s] using Module [%s]'s virtual fields.",r->name,modx);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,modx);
#endif
	virnames=getunsortedlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virnames!=NULL)
	{
		vc=0;
		while(r->virflds!=NULL && r->numvirtuals>0)
		{
			if(vc>=r->numvirtuals) break;
			for(x=0;x<virnames->numlibs;++x)
			{
				v=r->virflds+vc;
				if(!isEMPTY(v->expression))
				{
					if(RDAstrstr(v->expression,virnames->libs[x])!=NULL) 
					{
						for(y=0,v1=r->virflds;y<r->numvirtuals;++y,++v1)
						{
							if(!RDAstrcmp(v1->name,
								virnames->libs[x])) break;
						}	
						if(y>=r->numvirtuals)
						{
							d=getDFvirtual(modx,virnames->libs[x]);
							if(d!=NULL)
							{
								addvirtualline(r,d->name,d->type,d->length,d->expression,d->when);
								FreeDFvirtual(d);
								d=NULL;
							}
						}
					}
				}
			}
			++vc;
		}
		freeapplib(virnames);
	}
}
static short makerange(RDAezrpt *ez,char *name)
{
	short rv=FALSE;
	RDAezsrt *sort=NULL;
	RDAezdtl *detail=NULL;
	int x;
	char *temp=NULL;

	if(isEMPTY(name)) return(FALSE);
	temp=Rmalloc(RDAstrlen(name)+3);
	sprintf(temp,"[%s]",name);
	if(ez!=NULL)
	{
		if(ez->sort!=NULL)
		{
			for(x=0,sort=ez->sort;x<ez->numsrt;++x,++sort)
			{
				if(!RDAstrcmp(temp,sort->name)) break;
			}
			if(x<ez->numsrt) rv=TRUE;
		}
		if(rv==FALSE && ez->detail!=NULL)
		{
			for(x=0,detail=ez->detail;x<ez->numdtl;++x,++detail)
			{
				if(!RDAstrcmp(temp,detail->fieldname)) break;
			}
			if(x<ez->numdtl) rv=TRUE;
		}
	}
	return(rv);
}
void addaccumlineNoDuplicates(RDAreport *report,char *accumname,short accum_type,char *fieldname,short fieldtype)
{
	int x=0;
	RDAaccum *accum=NULL;

	for(x=0,accum=report->accum;x<report->numaccums;x++,accum++)
	{
		if(!RDAstrcmp(accumname,accum->name)) break;
	}
	if(x>=report->numaccums)
	{
		addaccumline(report,accumname,accum_type,fieldname,fieldtype);
	}
}
RDAreport *xEZRPTtoRPT(char *module,RDAezrpt *ez,EZfield *holdfields,int holdnum,int line,char *file)
{
	char *temp=NULL,*from=NULL,*to=NULL,*libx=NULL,addvf=TRUE;
	char *temp1=NULL;
	RDAreport *tmp=NULL;
	int x=0,y,z,virtualcount=0,sortlength=0,tlength=0,scount=0;
	APPlib *filenames=NULL;
	RDAezsrt *sort,*lastsort=NULL;
	RDAezdtl *detail;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short no_parts=0,coa_count=0,coa=FALSE,db_engine=0;
	RDAacct *acc;
	RDAvirtual *v;
	double pagewidth=8.5,page_length=0.0;
	short make_range=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG EZRPTtoRPT Converting EZ-Report Definition Module [%s] Report [%s] to an RDAreport Definition at line [%d] program [%s].",module,(ez->name!=NULL ? ez->name:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef USE_MYISAM
	db_engine=NRDMyIsamEngine;
#endif
#ifndef USE_MYISAM
	if(ez->numsrt>7)
	{
		db_engine=NRDLocalCtreeEngine;
	} else {
#if !defined(WIN32) && !defined(LINUX) && !defined(SUNX86)
		db_engine=NRDInformixIsamEngine;
#endif
#if defined(WIN32) || defined(LINUX) || defined(SUNX86)
		db_engine=NRDLocalCtreeEngine;
#endif
	}
#endif
	tmp=RDAreportNEW(ez->name,ez->title,ez->desc,db_engine,NULL,FALSE,TRUE,FALSE,FALSE,FALSE,NULL,NULL,(ez->range_screen==TRUE ? TRUE:FALSE),TRUE,TRUE);
	tmp->type=TRUE;
	tmp->remove_file=FALSE;
	page_length=ez->page_length;
	if(tmp->select!=NULL) Rfree(tmp->select);
	sortlength=0;
	tmp->select=Rmalloc(RDAstrlen(ez->majmod)+RDAstrlen(ez->majfil)+26);
	sprintf(tmp->select,"([%s][%s][DELETEFLAG]=FALSE) ",ez->majmod,ez->majfil);
	sortlength=RDAstrlen(tmp->select);
	if((!RDAstrcmp(ez->printsize,"COMPRESSED TEXT")) || (!RDAstrcmp(ez->printsize,"COMPRESSED TEXT (DUPLEX)")))
	{
		pagewidth=8.5;
	} else if((!RDAstrcmp(ez->printsize,"LANDSCAPE NORMAL TEXT")) || (!RDAstrcmp(ez->printsize,"LANDSCAPE NORMAL TEXT (DUPLEX)")))
	{
		pagewidth=11.0;
	} else if((!RDAstrcmp(ez->printsize,"LANDSCAPE COMPRESSED TEXT")) || (!RDAstrcmp(ez->printsize,"LANDSCAPE COMPRESSED TEXT (DUPLEX)")))
	{
		pagewidth=11.0;
	} else pagewidth=8.5;
	adddisplay(tmp,pagewidth,page_length,0,ez->alignment_test,(ez->alignment_test ? 1:0),NULL,1,0);
	tmp->display->dumprinter=TRUE;
	if(!RDAstrcmp(ez->printsize,"COMPRESSED TEXT (DUPLEX)"))
	{
		tmp->display->dumprinter_type=1;
	} else if(!RDAstrcmp(ez->printsize,"LANDSCAPE NORMAL TEXT (DUPLEX)"))
	{
		tmp->display->dumprinter_type=2;
	} else if(!RDAstrcmp(ez->printsize,"LANDSCAPE COMPRESSED TEXT (DUPLEX)"))
	{
		tmp->display->dumprinter_type=3;
	} else if(!RDAstrcmp(ez->printsize,"NORMAL TEXT"))
	{
		tmp->display->dumprinter_type=4;
	} else if(!RDAstrcmp(ez->printsize,"COMPRESSED TEXT"))
	{
		tmp->display->dumprinter_type=5;
	} else if(!RDAstrcmp(ez->printsize,"LANDSCAPE NORMAL TEXT"))
	{
		tmp->display->dumprinter_type=6;
	} else if(!RDAstrcmp(ez->printsize,"LANDSCAPE COMPRESSED TEXT"))
	{
		tmp->display->dumprinter_type=7;
	} else tmp->display->dumprinter_type=0;
	tmp->display->body_count=0;
	if(ez->detail!=NULL)
	{
		makepageheader(tmp,ez);
	}
	if(ez->incvir!=NULL)
	{
		coa_count=0;
		for(x=0,i=ez->incvir;x<ez->num;++x,++i)
		{
			if(!RDAstrcmp(i->module,"FINMGT") && !RDAstrcmp(i->file,"FINCOA") && ez->account_report)
			{
				establishCOAconflds(tmp,i->module,i->file,i->keyname,
					ez->majmod,ez->majfil,coa_count,holdfields,holdnum);
				++coa_count;
			} else {
				if(x)
				{
					establishconflds(tmp,i->module,i->file,i->keyname,
						ez->majmod,ez->majfil,holdfields,holdnum,(ez->range_screen==TRUE ? TRUE:FALSE));
				} else {
					establishconflds(tmp,i->module,i->file,i->keyname,
						NULL,NULL,holdfields,holdnum,(ez->range_screen==TRUE ? TRUE:FALSE));
				}
			}
		}
	}
	if(ez->detail!=NULL)
	{
		for(y=0,detail=ez->detail;y<ez->numdtl;++y,++detail)
		{
			if(detail->type==0)
			{
				if(ez->sort!=NULL)
				{
				for(x=0,sort=ez->sort;x<ez->numsrt;++x,++sort)
				{
					if(sort->total<3)
					{
						if(!RDAstrcmp(sort->name,detail->fieldname))
						{
							break;
						}
					}
				}
				}
				if(x>=ez->numsrt || ez->numsrt==0)
				{
					temp1=stralloc(detail->fieldname);
					ReplaceBrackets(temp1);
					switch(detail->fieldtype)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case CUSTOMTYPE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case BOOLNS:
						case CHARACTERS:
							temp=Rmalloc(RDAstrlen(temp1)+8);
							sprintf(temp,"LAST %s",temp1);
							addaccumlineNoDuplicates(tmp,temp,3,detail->fieldname,detail->fieldtype);
							if(temp!=NULL) Rfree(temp);
							break;
						case DECIMALV:
						case SDECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DOUBLEV:
						case SDOUBLEV:
							temp=Rmalloc(RDAstrlen(temp1)+9);
							sprintf(temp,"TOTAL %s",temp1);
							addaccumlineNoDuplicates(tmp,temp,7,detail->fieldname,detail->fieldtype);
							if(temp!=NULL) Rfree(temp);
							break;
						case SHORTV:
						case SSHORTV:
							temp=Rmalloc(RDAstrlen(temp1)+8);
							sprintf(temp,"LAST %s",temp1);
							addaccumlineNoDuplicates(tmp,temp,3,detail->fieldname,detail->fieldtype);
							if(temp!=NULL) Rfree(temp);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							temp=Rmalloc(RDAstrlen(temp1)+8);
							sprintf(temp,"LAST %s",temp1);
							addaccumlineNoDuplicates(tmp,temp,3,detail->fieldname,detail->fieldtype);
							if(temp!=NULL) Rfree(temp);
							break;
						default:
							prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
					}
					if(temp1!=NULL) Rfree(temp1);
				}
			}
		}
	}
	lastsort=NULL;
	if(ez->sort!=NULL)
	{
		for(x=0,sort=ez->sort;x<ez->numsrt;++x,++sort)
		{
			if(sort->total<3)
			{
				lastsort=sort;
				addsortline(tmp,0,sort->name); /* ASCENDING ALWAYS */
				addcontrolline(tmp,sort->name,sort->type);
				temp=Rmalloc(RDAstrlen(sort->name)+8);
				temp1=stralloc(sort->name);
				ReplaceBrackets(temp1);
				sprintf(temp,"LAST %s",temp1);
				addaccumlineNoDuplicates(tmp,temp,3,sort->name,sort->type);
				if(temp!=NULL) Rfree(temp);
				if(temp1!=NULL) Rfree(temp1);
				if((sort->total==1 || sort->total==2) &&
					x<(ez->numsrt-1)) 
				{
					if(ez->detail!=NULL)
					{
						makeheader(tmp,ez,sort,scount);
					}
					++scount;
				}
			}
			if(sort->range)
			{
				switch(sort->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(sort->from.string_value!=NULL)
						{
							from=stralloc(sort->from.string_value);
						} else from=NULL;
						if(sort->to.string_value!=NULL)
						{
							to=stralloc(sort->to.string_value);
						} else to=NULL;
						if(sort->from.string_value==NULL && sort->to.string_value[0]=='~')
						{
							/* skip it */
						} else {
						if(RDAstrcmp(from,to))
						{
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+24);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"AND ((%s>=\"%s\") AND (%s<=\"%s\")) ",
									sort->name,(from!=NULL ? from:""),
									sort->name,(to!=NULL ? to:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+20);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"((%s>=\"%s\") AND (%s<=\"%s\")) ",
									sort->name,(from!=NULL ? from:""),
									sort->name,(to!=NULL ? to:""));
							}
						} else {
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+10);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"AND (%s=\"%s\") ",
									sort->name,(from!=NULL ? from:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+6);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"(%s=\"%s\") ",sort->name,(from!=NULL ? from:""));
							}
						}
						}
						break;
					case BOOLNS:
						if(*sort->from.string_value!=*sort->to.string_value)
						{
							from=Rmalloc(2);
							sprintf(from,"%d",*sort->from.string_value);
							to=Rmalloc(2);
							sprintf(to,"%d",*sort->to.string_value);
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+20);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"AND ((%s>=%s) AND (%s<=%s)) ",sort->name,(from!=NULL ? from:""),sort->name,(to!=NULL ? to:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+16);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"((%s>=%s) AND (%s<=%s)) ",sort->name,(from!=NULL ? from:""),sort->name,(to!=NULL ? to:""));
							}
						} else {
							if(tmp->select!=NULL)
							{
								tlength=(+RDAstrlen(sort->name)+(*sort->from.string_value ? 7:10));
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								if(*sort->from.string_value)
								{
									sprintf(&tmp->select[sortlength],"AND (%s) ",
										sort->name);
								} else {
									sprintf(&tmp->select[sortlength],"AND (!(%s)) ",
										sort->name);
								}
							} else {
								tlength=(+RDAstrlen(sort->name)+(*sort->from.string_value ? 3:6));
								tmp->select=Rmalloc(sortlength+tlength+1);
								if(*sort->from.string_value)
								{
									sprintf(&tmp->select[sortlength],"(%s) ",
										sort->name);
								} else {
									sprintf(&tmp->select[sortlength],"(!(%s)) ",
										sort->name);
								}
							}
						}
						break;
					case CHARACTERS:
						from=Rmalloc(2);
						sprintf(from,"%c",*sort->from.string_value);
						to=Rmalloc(2);
						sprintf(to,"%c",*sort->to.string_value);
						if(*sort->from.string_value!=*sort->to.string_value)
						{
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+20);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"AND ((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+16);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							}
						} else  {
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+8);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"AND (%s=%s) ",sort->name,(from!=NULL ? from:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+4);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"(%s=%s) ",sort->name,(from!=NULL ? from:""));
							}
						}
						break;
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
						sprintf(stemp,"%.0f",*sort->from.float_value);
						from=stralloc(stemp);
						sprintf(stemp,"%.0f",*sort->to.float_value);
						to=stralloc(stemp);
						if(*sort->from.float_value!=*sort->to.float_value)
						{
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+16);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+16);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							}
						} else {
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+8);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"AND (%s=%s) ",sort->name,(from!=NULL ? from:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+4);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"(%s=%s) ",sort->name,(from!=NULL ? from:""));
							}
						}
						break;
					case DECIMALV:
					case SDECIMALV:
						sprintf(stemp,"%f",*sort->from.float_value);
						from=stralloc(stemp);
						sprintf(stemp,"%f",*sort->to.float_value);
						to=stralloc(stemp);
						if(*sort->from.float_value!=*sort->to.float_value)
						{
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+20);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"AND ((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+16);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							}
						} else {
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+8);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"AND (%s=%s) ",
									sort->name,(from!=NULL ? from:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+4);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"(%s=%s) ",
									sort->name,(from!=NULL ? from:""));
							}
						}
						break;
					case SHORTV:
					case SSHORTV:
						sprintf(stemp,"%d",*sort->from.short_value);
						from=stralloc(stemp);
						sprintf(stemp,"%d",*sort->to.short_value);
						to=stralloc(stemp);
						if(*sort->from.short_value!=*sort->to.short_value)
						{
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+20);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"AND ((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+16);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							}
						} else {
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+8);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"AND (%s=%s) ",sort->name,(from!=NULL ? from:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+4);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"(%s=%s) ",sort->name,(from!=NULL ? from:""));
							}
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						sprintf(stemp,"%d",*sort->from.integer_value);
						from=stralloc(stemp);
						sprintf(stemp,"%d",*sort->to.integer_value);
						to=stralloc(stemp);
						if(*sort->from.integer_value!=*sort->to.integer_value)
						{
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+20);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"AND ((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(to)+(RDAstrlen(sort->name)*2)+16);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],
									"((%s>=%s) AND (%s<=%s)) ",sort->name,
									(from!=NULL ? from:""),sort->name,
									(to!=NULL ? to:""));
							}
						} else {
							if(tmp->select!=NULL)
							{
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+8);
								tmp->select=Rrealloc(tmp->select,sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"AND (%s=%s) ",sort->name,(from!=NULL ? from:""));
							} else {
								tlength=(RDAstrlen(from)+RDAstrlen(sort->name)+4);
								tmp->select=Rmalloc(sortlength+tlength+1);
								sprintf(&tmp->select[sortlength],"(%s=%s) ",sort->name,(from!=NULL ? from:""));
							}
						}
						break;
					default:
						prterr("Error field type [%d] not found for sort [%s].",sort->type,sort->name);
						break;
				}
				sortlength+=tlength;
				if(from!=NULL) Rfree(from);
				if(to!=NULL) Rfree(to);
			}
		}
		if(lastsort!=NULL)
		{
			temp=Rmalloc(RDAstrlen(lastsort->name)+13);
			temp1=stralloc(lastsort->name);
			ReplaceBrackets(temp1);
			sprintf(temp,"FREQUENCY %s",temp1);
			addaccumlineNoDuplicates(tmp,temp,1,lastsort->name,lastsort->type);
			if(temp1!=NULL) Rfree(temp1);
			if(temp!=NULL) Rfree(temp);
		}
	}
	addvirtuallineADV(tmp,"OPTIONAL REPORT TITLE",0,0,NULL,1,2);
	temp=Rmalloc(RDAstrlen(ez->title)+160);
	sprintf(temp,"IF([OPTIONAL REPORT TITLE]!=\"\") THEN RETURN_VALUE=[OPTIONAL REPORT TITLE] ELSE RETURN_VALUE=\"%s\" FI",(ez->title!=NULL ? ez->title:""));
	addvirtuallineADV(tmp,"REPORT TITLE",0,0,temp,1,FALSE);
	if(temp!=NULL) Rfree(temp);
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,"\"Executed By: \"+[LOGIN IDENTIFICATION]",1,FALSE);
	if(tmp->select!=NULL) tmp->select[RDAstrlen(tmp->select)-1]=0;
	if(ez->detail!=NULL)
	{
	for(x=0,detail=ez->detail;x<ez->numdtl;++x,++detail)
	{
		if(detail->type) 
		{
			sprintf(stemp,"VIRTUAL FIELD #%d",virtualcount);
			addvirtuallineADV(tmp,stemp,detail->fieldtype,detail->length,detail->expression,0,(ez->range_screen==TRUE ? TRUE:FALSE));
			sprintf(stemp,"[VIRTUAL FIELD #%d]",virtualcount);
			++virtualcount;
			temp1=stralloc(stemp);
			ReplaceBrackets(temp1);
			switch(detail->fieldtype)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case BOOLNS:
				case CHARACTERS:
					temp=Rmalloc(RDAstrlen(temp1)+8);
					sprintf(temp,"LAST %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,3,stemp,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				case DECIMALV:
				case SDECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case SDOUBLEV:
					temp=Rmalloc(RDAstrlen(temp1)+9);
					sprintf(temp,"TOTAL %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,7,stemp,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				case SHORTV:
				case SSHORTV:
					temp=Rmalloc(RDAstrlen(temp1)+8);
					sprintf(temp,"LAST %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,3,stemp,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp=Rmalloc(RDAstrlen(temp1)+8);
					sprintf(temp,"LAST %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,3,stemp,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				default:
					prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
			}
			if(temp1!=NULL) Rfree(temp1);
		} else {
			temp1=stralloc(detail->fieldname);
			ReplaceBrackets(temp1);
			switch(detail->fieldtype)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case BOOLNS:
				case CHARACTERS:
					temp=Rmalloc(RDAstrlen(temp1)+8);
					sprintf(temp,"LAST %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,3,detail->fieldname,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				case DECIMALV:
				case SDECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case SDOUBLEV:
					temp=Rmalloc(RDAstrlen(temp1)+9);
					sprintf(temp,"TOTAL %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,7,detail->fieldname,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				case SHORTV:
				case SSHORTV:
					temp=Rmalloc(RDAstrlen(temp1)+8);
					sprintf(temp,"LAST %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,3,detail->fieldname,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp=Rmalloc(RDAstrlen(temp1)+8);
					sprintf(temp,"LAST %s",temp1);
					addaccumlineNoDuplicates(tmp,temp,3,detail->fieldname,detail->fieldtype);
					if(temp!=NULL) Rfree(temp);
					break;
				default:
					prterr("Error field type [%d] is invalid for detail [%s].",detail->fieldtype,detail->fieldname);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		memset(stemp,0,101);
	}
	}
	if(filenames!=NULL) freeapplib(filenames);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(ez->majmod)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,ez->majmod);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,ez->majmod);
#endif
	filenames=getunsortedlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(filenames!=NULL)
	{
		for(y=0;y<filenames->numlibs;++y)
		{
			d=getDFvirtual(ez->majmod,filenames->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				coa=FALSE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						if(!RDAstrcmp(a->file,"FINCOA") &&
							!RDAstrcmp(a->module,"FINMGT"))
						{
							coa=TRUE;
							break;
						}
					}
					if(!d->vtype)
					{
						for(z=0,a=d->incvir;z<d->num;++z,++a)
						{
							if(!RDAstrcmp(a->file,"FINCOA") &&
								!RDAstrcmp(a->module,"FINMGT")) coa=TRUE;
							for(x=0,i=ez->incvir;x<ez->num;++x,++i)
							{
								if(!RDAstrcmp(i->module,a->module) && 
									!RDAstrcmp(i->file,a->file)) break;
							}
							if(x>=ez->num) 
							{
								addvf=FALSE;
								break;
							}
						}
					} else {
						for(z=0,a=d->incvir;z<d->num;++z,++a)
						{
							for(x=0,i=ez->incvir;x<ez->num;++x,++i)
							{
								if(!RDAstrcmp(i->module,a->module) && 
									!RDAstrcmp(i->file,a->file)) break;
							}
							if(x<ez->num) break;
						}
						if(z<d->num) addvf=TRUE;
							else addvf=FALSE;
					}
				}
				if(addvf || (ez->account_report && coa))
				{
					if(coa)
					{
						addvf=TRUE;
					} else {
						addvf=FALSE;
						for(x=0,sort=ez->sort;x<ez->numsrt;++x,++sort)
						{
							temp=Rmalloc(RDAstrlen(d->name)+3);
							sprintf(temp,"[%s]",d->name);	
							if(!RDAstrcmp(sort->name,temp)) 
							{
								addvf=TRUE;
								if(temp!=NULL) Rfree(temp);
								temp=NULL;
								break;
							}
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						}
						if(!addvf)
						{
							for(x=0,detail=ez->detail;x<ez->numdtl;++x,++detail)
							{
								if(detail->fieldname!=NULL)
								{
									if(RDAstrstr(detail->fieldname,d->name)!=NULL)
									{
										addvf=TRUE;
									}
								}
								if(detail->expression!=NULL)
								{
									if(RDAstrstr(detail->expression,d->name)!=NULL)
									{
										addvf=TRUE;
									}
								}
								if(addvf) break;
							}
						}
					}
					if(addvf)
					{
						make_range=makerange(ez,d->name);
						addvirtuallineADV(tmp,d->name,d->type,d->length,
							d->expression,d->when,(make_range==TRUE ? TRUE:FALSE));
					}
				}
				FreeDFvirtual(d);
				d=NULL;
			}
		}
		freeapplib(filenames);
		filenames=NULL;
	}
	if(ez->detail!=NULL)
	{
		makedetailline(tmp,ez);
		makefooters(tmp,ez);
	}
	if(ez->account_report)
	{
		no_parts=0;
		if(EXPENDITURE_ACCOUNT!=NULL)
		{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
					z<EXPENDITURE_ACCOUNT->num;++z,++acc)
				{
					if(acc->num>no_parts) no_parts=acc->num;
				}
			}
		}
		if(REVENUE_ACCOUNT!=NULL)
		{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				for(z=0,acc=REVENUE_ACCOUNT->codetype;
					z<REVENUE_ACCOUNT->num;++z,++acc)
				{
					if(acc->num>no_parts) no_parts=acc->num;
				}
			}
		}
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
				{
					if(acc->num>no_parts) no_parts=acc->num;
				}
			}
		}
		memset(stemp,0,101);
		for(x=0;x<no_parts;++x)
		{
			for(z=0;z<3;++z)
			{
				switch(z)
				{
					default:
					case 0:
						sprintf(stemp,"DIMENSION #%d CODE",x);
						break;
					case 1:
						sprintf(stemp,"DIMENSION #%d",x);
						break;
					case 2:
						sprintf(stemp,"DIMENSION #%d DESCRIPTION",x);
						break;
				}
				for(y=0,v=tmp->virflds;y<tmp->numvirtuals;++y,++v)
				{
					if(!RDAstrcmp(v->name,stemp)) break;
				}	
				if(y>=tmp->numvirtuals)
				{
					d=getDFvirtual(module,stemp);
					if(d!=NULL)
					{
						addvirtualline(tmp,d->name,d->type,d->length,d->expression,d->when);
						FreeDFvirtual(d);
						d=NULL;
					}
				}
			}
		}
	}
	SetDependentVirtuals(tmp,module);
/* MAKE RANGE SCREEN */
	if(ez->range_screen==TRUE)
	{
		makeEZRPTRangeSCreen(module,ez);
	}
/* SJS took this out because of prtidea changes and using RDA_PFILE instead
   of STDOUT
	if(diagrptgen || diagrptgen_field) 
	{
		SEERDAreport(tmp,FALSE);
	}
*/
	return(tmp);
}
void xestablishCOAconflds(RDAreport *tmp,char *modulename,char *filename,
	char *keyname,char *cmod,char *cfile,short count,EZfield *holdfields,
	int numezf,int line,char *file)
{
	int y,z;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0,holdnum=0;
	RDAconfld *cf,*holdconfld=NULL;
	EZfield *ezf;
	char *filx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG establishCOAconflds Establishing Connecting Fields for an Account Type EZ-Report COA's at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!GETBIN(modulename,filename,&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
			fields=NULL;
		}
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->part!=NULL && !RDAstrcmp(keyx->name,keyname))
				{
					holdnum=keyx->numparts;
					holdconfld=Rmalloc(keyx->numparts*sizeof(RDAconfld));
					for(z=0,cf=holdconfld,part=keyx->part;z<keyx->numparts;
						++z,++cf,++part)
					{
						if(part->name!=NULL) 
						{
							cf->fldname=stralloc(part->name);
							Rfree(part->name);
						} else cf->fldname=NULL;
						cf->conname=NULL;
					}
					Rfree(keyx->part);
				}
				if(keyx->name!=NULL) Rfree(keyx->name);
			}
			Rfree(keys);
			keys=NULL;
		}
		if(holdconfld!=NULL)
		{
			if(cmod!=NULL && cfile!=NULL)
			{
				if(!GETBIN(cmod,cfile,&engine,&nofields,&fields,
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
								Rfree(keyx->part);
							}
						}
						Rfree(keys);
					}
					if(fields!=NULL)
					{
						for(z=0,cf=holdconfld;z<holdnum;++z,++cf)
						{
							for(y=0,f=fields;y<nofields;++y,++f)
							{
								if(!RDAstrcmp(cf->fldname,f->name))
								{
									cf->conname=Rmalloc(RDAstrlen(filename)+
										RDAstrlen(f->name)+5);
									sprintf(cf->conname,"[%s][%s]",
										filename,f->name);
									break;
								}
							}
							if(y>=nofields)
							{
								if(!RDAstrcmp(cf->fldname,"ACCOUNT TYPE"))
								{
									cf->conname=stralloc("[ACCOUNT TYPE]");
								} else if(!RDAstrcmp(cf->fldname,"DIMENSION"))
								{
									sprintf(stemp,"[DIMENSION #%d]",count);
									cf->conname=stralloc(stemp);
									memset(stemp,0,101);
								} else if(!RDAstrcmp(cf->fldname,"NAME"))
								{
									sprintf(stemp,"[DIMENSION #%d CODE]",
										count);
									cf->conname=stralloc(stemp);
									memset(stemp,0,101);
								} else {
									for(y=0,ezf=holdfields;y<numezf;++y,++ezf)
									{
										if(!RDAstrcmp(ezf->name,cf->fldname))
										{
											filx=stripfilename(ezf->wholename);
											cf->conname=Rmalloc(RDAstrlen(filx)+RDAstrlen(ezf->name)+5);
											sprintf(cf->conname,"[[%s][%s]]",filx,ezf->name);
											if(filx!=NULL) Rfree(filx);
											filx=NULL;
											break;
										}
									}
								}
							}
						}
						if(fields!=NULL)
						{
							for(y=0,f=fields;y<nofields;++y,++f)
							{
								if(f->name!=NULL) Rfree(f->name);
							}
							Rfree(fields);
							fields=NULL;nofields=0;
						}
					}
				}
			}
		}
	}
	if(cmod!=NULL && cfile!=NULL) 
	{
		addsearchline(tmp,modulename,filename,1,cmod,cfile,holdnum,holdconfld,keyname,FALSE,NULL);
	} else {
		addsearchline(tmp,modulename,filename,0,NULL,NULL,0,NULL,keyname,FALSE,NULL);
	}
	if(holdconfld!=NULL)
	{
		for(y=0,cf=holdconfld;y<holdnum;++y,++cf)
		{
			if(cf->conname!=NULL) Rfree(cf->conname);
			if(cf->fldname!=NULL) Rfree(cf->fldname);
		}
		Rfree(holdconfld);
		holdconfld=NULL;
		holdnum=0;
	}
}
void xMAKEPAGEHEADER(RDAreport *tmp,char *name,char *title,int line,
	short longest,char *file)
{
	RDAimage *i=NULL;
	RDApval *pval;
	int x;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG MAKEPAGEHEADER Making Page Header for [%s] [%s] at line [%d] program [%s].",name,title,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	i=Rmalloc(sizeof(RDAimage));
	i->breakname=stralloc("PAGE HEADER");
	if(RDAstrlen(XPERT_SETUP->company_name)>RDAstrlen(title))
	{
		if((RDAstrlen(XPERT_SETUP->company_name)+36)>longest) 
			longest=(RDAstrlen(XPERT_SETUP->company_name)+36);
	} else {
		if((RDAstrlen(title)+36)>longest) 
			longest=(RDAstrlen(title)+36);
	}
	if(longest<55) longest=55;
	i->line=Rmalloc(longest+4);
	i->numpvals=4;
	i->pvals=Rmalloc(4*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=0;
	pval->name=stralloc("[ORGANIZATION NAME]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=0;
	pval->name=stralloc("[PAGE NO]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	temp=Rmalloc(RDAstrlen(XPERT_SETUP->company_name));
	memset(temp,'C',RDAstrlen(XPERT_SETUP->company_name)-1);
	sprintf(i->line,"%*s@%s%*sPage: @RRRRRRR@@",
		((longest-RDAstrlen(XPERT_SETUP->company_name))/2),"",temp,
		((longest-RDAstrlen(XPERT_SETUP->company_name))/2)-15,"");
	if(temp!=NULL) Rfree(temp);
	temp=NULL;
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	if(i->pvals!=NULL) 
	{
		for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(i->pvals);
	}
	if(i->line!=NULL) Rfree(i->line);
	i->line=Rmalloc(longest+4);
	i->numpvals=3;
	i->pvals=Rmalloc(3*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=0;
	pval->name=stralloc("[SYSTEM DATE]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	if(title!=NULL)
	{
		sprintf(i->line,"%*s%s%*sDate: @LLLLLLLL@@",
			(longest-RDAstrlen(title))/2,"",title,(longest-
			RDAstrlen(title))/2-15,"");
	} else {
		sprintf(i->line,"%*sDate: @LLLLLLLL@@",longest-15,"");
	}
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	if(i->pvals!=NULL)
	{
		for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(i->pvals);
	}
	if(i->line!=NULL) Rfree(i->line);
	i->line=Rmalloc(longest+4);
	i->numpvals=4;
	i->pvals=Rmalloc(4*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=0;
	pval->name=stralloc("[LOGIN IDENTIFICATION]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=0;
	pval->name=stralloc("[SYSTEM TIME]");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	sprintf(i->line,"%*sExecuted By: @LLLLLLLLL%*sTime: @LLLLLLLL@@",
		(longest-23)/2,"",((longest-23)/2)-15,"");
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,i->numpvals,i->pvals);
	if(i->line!=NULL) Rfree(i->line);
	if(i->pvals!=NULL)
	{
		for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(i->pvals);
	}
	i->line=Rmalloc(longest+4);
	sprintf(i->line,"@@");
	i->numpvals=2;
	i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	addimageline(tmp,i->breakname,i->line,FALSE,NULL,
		i->numpvals,i->pvals);
	if(i->line!=NULL) Rfree(i->line);
	if(i->pvals!=NULL)
	{
		for(x=0,pval=i->pvals;x<i->numpvals;++x,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(i->pvals);
	}
	i->numpvals=2;
	i->pvals=Rmalloc(i->numpvals*sizeof(RDApval));
	pval=i->pvals;
	pval->symbol_type=1;
	pval->name=stralloc("CARRIAGE RETURN");
	pval->type=0;
	pval->fieldname=NULL;
	++pval;
	pval->symbol_type=1;
	pval->name=stralloc("LINEFEED");
	pval->type=0;
	pval->fieldname=NULL;
	if(i->line!=NULL) Rfree(i->line);
	if(i->breakname!=NULL) Rfree(i->breakname);
	Rfree(i);
}
void xadddisplay(RDAreport *tmp,double pwidth,double plength,short device,
	short alignment,short testpages,char *devicename,char devicenameeval,
	short bodycount,short starting_page_no,int numpvars,RDApvar *pvars,
	char dumprinter,short dumprinter_type,short count_invisible,
	char doscopy,char *device_spec,int line,char *file)
{
	RDAdisplay *disp=NULL;
	RDApvar *f=NULL,*t=NULL;
	int x=0;
	

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG xadddisplay at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
/* Setting the Display Information */

	tmp->display=Rmalloc(sizeof(RDAdisplay));
	disp=tmp->display; 
	disp->body_count=bodycount; /* Number of Detail Lines Printed */
	disp->starting_page=starting_page_no; /* Starting Page Number */
	disp->page_width=pwidth; /* Page Width in Inches */
	disp->page_length=plength; /* Page Length in Inches */
	disp->device=device; /* Output Device Type: 
						0-Printer,
						1-Ascii,
						2-Append,
						3-Datafile,
						4-Update Datafile,
						5-Popen */
	disp->date_order=alignment; /* Alignment Test True or False */
	disp->test_pages=testpages; /* Number of Alignment Test Pages */
	if(!isEMPTY(devicename)) disp->set_lpp=stralloc(devicename);
		else disp->set_lpp=NULL; /* Actual Output Device Name */
/* SJSADD we will need to pass this as arguments and change the places where 
          it is called after standards are established for use of these 
          values when making a ezrpt or mb2ezrpt to report. */
	disp->set_lpp_eval=devicenameeval;
	disp->dumprinter=dumprinter;
	disp->dumprinter_type=dumprinter_type;
	disp->count_invisible=count_invisible; /* count unprinted detail sort records in BODY COUNT */
	disp->doscopy=doscopy; 
	if(!isEMPTY(device_spec)) disp->device_spec=stralloc(device_spec);
		else disp->device_spec=NULL; 
	if(pvars!=NULL && numpvars>0)
	{
		disp->num=numpvars;
		disp->pvars=Rmalloc(numpvars*sizeof(RDApvar));
		for(x=0,f=pvars,t=disp->pvars;x<numpvars;++x,++f,++t)
		{
			if(f->name!=NULL) t->name=stralloc(f->name);
				else t->name=NULL;
			if(f->command!=NULL) t->command=stralloc(f->command);
				else t->command=NULL;
			t->chgsize=f->chgsize;
		}
	} else {
		disp->num=0; /* Number of Output Device structures */
		disp->pvars=NULL;
	}
}
int xaddsearchconfld(RDAsearch *s,char *fldname,char *conname,int line,
	char *file)
{
	RDAconfld *c;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addsearchconfld at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(s!=NULL)
	{
		if(s->confld!=NULL)
		{
			s->confld=Rrealloc(s->confld,(s->num+1)*sizeof(RDAconfld));
		} else {
			s->confld=Rmalloc(sizeof(RDAconfld));
			s->num=0;
		}
		c=s->confld+s->num;
		if(!isEMPTY(fldname)) c->fldname=stralloc(fldname);
			else c->fldname=NULL;
		if(!isEMPTY(conname)) c->conname=stralloc(conname);
			else c->conname=NULL;
		return(++s->num);
	} else {
		prterr("Error RDAsearch is NULL at line [%d] program [%s].",line,file);
		return(-1);
	}
}
int xaddimagepval(RDAimage *i,short symbol_type,char *name,int type,
	char *fieldname,int line,char *file)
{
	RDApval *p;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diagrptgen_field)
	{
		prterr("DIAG addimagepval at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(i!=NULL)
	{
		if(i->pvals!=NULL)
		{
			i->pvals=Rrealloc(i->pvals,(i->numpvals+1)*sizeof(RDApval));
		} else {
			i->pvals=Rmalloc(sizeof(RDApval));
			i->numpvals=0;
		}
		p=i->pvals+i->numpvals;
		p->symbol_type=symbol_type;
		if(!isEMPTY(name)) p->name=stralloc(name);
			else p->name=NULL;
		p->type=type;
		if(!isEMPTY(fieldname)) p->fieldname=stralloc(fieldname);
			else p->fieldname=NULL;
		return(++i->numpvals);
	} else {
		prterr("Error RDAimage is NULL at line [%d] program [%s].",line,file);
		return(-1);
	}
}
