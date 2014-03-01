/* lvealusw.c - Leave Management Add Leave Used Sub-Worked*/
#ifndef WIN32
#define __NAM__ "lvealusw.lnx"
#endif
#ifdef WIN32
#define __NAM__ "lvealusw.exe"
#endif
#include <app.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <rptgen.h>
#include <addlists.h>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static short filenum=0,lmsrnum=0,eabsnum=0,ldescnum=0,ltypenum=0,dmgnum=0;
static short posper=0,posgrs=0,poscls=0,posdedc=0,possmst=0,posngms=0;
static short submstr=0,subwknum=0,subdmgnum=0,jmstnum=0,posmstr=0,poslmsr=0;
static short subemp=0,subpos=0,subloc=0,subjmstnum=0/*,subposmstr=0*/;
static MakeBrowseList *mbl_perdmg=NULL;
static MakeBrowseList *mbl_submstr=NULL;
static void doexit(MakeBrowseList *);
static void okfunc0(MakeBrowseList *,void *),okfunc1(MakeBrowseList *,void *);
static short selectfunction(MakeBrowseList *);
static RDArsrc *definelist=NULL;
static APPlib *prevlve=NULL;
static APPlib *lvedesclist=NULL;
static APPlib *jobnolist=NULL;
static APPlib *subjobnolist=NULL;
static char *module="LVEMGT";
static int counter=0;
static RDAfinmgt *fmgt=NULL;
static void reset_defaults(RDArsrc *);
static void set_defaults(RDArsrc *,short);
static void savelist(RDArsrc *,RDArsrc *),savelistcb(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *),quit_record(RDArsrc *);
static void quit_record_free(RDArsrc *),quit_record_freens(RDArsrc *);
static void save_check(RDArsrc *),quit_check(RDArsrc *);
static short check_perid(RDArsrc *,short),check_lveid(RDArsrc *,short);
static short check_subid(RDArsrc *,short),check_jobno(RDArsrc *,short);
static short check_subjobno(RDArsrc *,short);
static void check_lveidcb(RDArsrc *),check_peridcb(RDArsrc *);
static void check_jobnocb(RDArsrc *),check_subidcb(RDArsrc *);
static void check_subjobnocb(RDArsrc *);
static void getsupportingrecords(RDArsrc *,short);
static void compute_sub_units(RDArsrc *);
static void compute_abs_units(RDArsrc *);
static void changeper(RDArsrc *);
static void changesub(RDArsrc *,short);
static void changelve(RDArsrc *);
static void changelvecb(RDArsrc *);
static void changejobno(RDArsrc *);
static void changejobnocb(RDArsrc *);
static void changesubjobno(RDArsrc *);
static void changesubjobnocb(RDArsrc *);
static void browse_perdmg(RDArsrc *);
static void browse_submstr(RDArsrc *);
static void SubLveData(char **,PassableStruct *);
static char trueflag=TRUE;
char *personnel_identification=NULL,*leave_description=NULL;
char *subpersonnel_identification=NULL;
int job_number=(-1);
char *pareportname=NULL,*audit_reportname=NULL;

void SubLveData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	short mainfileno=(-1);
	char *temp=NULL,*gn=NULL;
	int y=0;
	RDAvirtual *v=NULL;
	RDArsrc *r=NULL;

	r=p->r;
	mainfileno=*(short *)p->a;
	if(diageval)
	{
		prterr("DIAG SubLveData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	if(filename!=NULL)
	{
		fieldname=stripfieldname(*tmp);
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+
			RDAstrlen(fieldname)+6))
		{
			temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+
				RDAstrlen(fieldname)+7);
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				occurrence=Ratoi(gn);
				gn=strtok(NULL,",]");
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
		if(fileno!=(-1))
		{
			field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
		} else field=NULL;
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
					str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
					if(length<1) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(field->data.string_value!=NULL && str_length>length)
					{
						temp=stralloc(field->data.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(start-1+length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else {
						sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(*field->data.short_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.short_value);
					} else {
						sprintf(stemp,"%d",*field->data.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					if(*field->data.integer_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.integer_value);
					} else {
						sprintf(stemp,"%d",*field->data.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(*field->data.float_value<0)
					{
						sprintf(stemp,"(%f)",*field->data.float_value);
					} else {
						sprintf(stemp,"%f",*field->data.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(stemp,"%d",(*field->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(*tmp,"%c",*field->data.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
	} else if(!RDAstrcmp(modulename,"LOGIN IDENTIFICATION"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(USERLOGIN)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(USERLOGIN)+3);
		}
		sprintf(*tmp,"\"%s\"",(USERLOGIN!=NULL ? USERLOGIN:""));
	} else if(!RDAstrcmp(modulename,"STARTING FISCAL MONTH"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",fmgt->month);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
	} else if(!RDAstrcmp(modulename,"SYSTEM DATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		filename=GETCURRENTDATE();
		str_length=(filename!=NULL ? RDAstrlen(filename):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+2))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(filename!=NULL && str_length>length)
		{
			gn=filename+(start-1);
			if(RDAstrlen(gn)>(start-1+length))
				filename[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&filename[start-1]);
		} else {
			sprintf(*tmp,"\"%s\"",(filename!=NULL ? filename:""));
		}
		if(filename!=NULL) Rfree(filename);
	} else if(!RDAstrcmp(modulename,"ORGANIZATION NAME"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(XPERT_SETUP->company_name)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(XPERT_SETUP->company_name)+3);
		}
		sprintf(*tmp,"\"%s\"",(XPERT_SETUP->company_name!=NULL ? XPERT_SETUP->company_name:""));
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #1"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(XPERT_SETUP->addr1)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(XPERT_SETUP->addr1)+3);
		}
		sprintf(*tmp,"\"%s\"",(XPERT_SETUP->addr1!=NULL ? XPERT_SETUP->addr1:""));
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #2"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(XPERT_SETUP->addr2)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(XPERT_SETUP->addr2)+3);
		}
		sprintf(*tmp,"\"%s\"",(XPERT_SETUP->addr2!=NULL ? XPERT_SETUP->addr2:""));
	} else if(!RDAstrcmp(modulename,"ORGANIZATION CITY"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(XPERT_SETUP->city)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(XPERT_SETUP->city)+3);
		}
		sprintf(*tmp,"\"%s\"",(XPERT_SETUP->city!=NULL ? XPERT_SETUP->city:""));
	} else if(!RDAstrcmp(modulename,"ORGANIZATION STATE"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(XPERT_SETUP->state)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(XPERT_SETUP->state)+3);
		}
		sprintf(*tmp,"\"%s\"",(XPERT_SETUP->state!=NULL ? XPERT_SETUP->state:""));
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ZIP"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(XPERT_SETUP->zip)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(XPERT_SETUP->zip)+3);
		}
		sprintf(*tmp,"\"%s\"",(XPERT_SETUP->zip!=NULL ? XPERT_SETUP->zip:""));
	} else if(!RDAstrcmp(modulename,"ORGANIZATION PHONE"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(XPERT_SETUP->phone)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(XPERT_SETUP->phone)+3);
		}
		sprintf(*tmp,"\"%s\"",(XPERT_SETUP->phone!=NULL ? XPERT_SETUP->phone:""));
	} else if(!RDAstrcmp(modulename,"SYSTEM TIME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=GETCURRENTTIME();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(temp!=NULL && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(start-1+length))
				temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"%s\"",(temp!=NULL ? temp:""));
		}
		if(temp!=NULL) Rfree(temp);
	} else { /* virtual field */
		for(y=0,v=r->virtual;y<r->num;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<r->num)
		{
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						temp=EVALUATEstr(v->expression,SubLveData,p);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=temp;
						temp=NULL;
						if(v->value.string_value!=NULL)
						{
							v->dlen=RDAstrlen(v->value.string_value)+1;
							if(v->dlen>v->len && v->len>0)
							{
								v->value.string_value[v->len]=0;
							}
						}
					}
					if(RDAstrlen(*tmp)>(RDAstrlen(v->name)+2))
					{
						temp=*tmp+(RDAstrlen(v->name)+3);
						gn=strtok(temp,",]");
						if(gn!=NULL)
						{
							start=Ratoi(gn);
							gn=strtok(NULL,"]");
							length=Ratoi(gn);
						}
						if(start<1) start=1;

					}
					if(length==0) length=v->dlen;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if((RDAstrlen(*tmp)>(RDAstrlen(v->name)+2)) && v->value.string_value!=NULL)
					{
						temp=stralloc(v->value.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(start-1+length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
					} else {
						sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=EVALUATEsht(v->expression,SubLveData,p);
					}
					if(*v->value.short_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.short_value);
					} else {
						sprintf(stemp,"%d",*v->value.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.integer_value=EVALUATEint(v->expression,SubLveData,p);
					}
					if(*v->value.integer_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.integer_value);
					} else {
						sprintf(stemp,"%d",*v->value.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.float_value=EVALUATEdbl(v->expression,SubLveData,p);
						if(v->type==DOLLARS ||
							v->type==DOUBLEV ||
							v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}	
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,SubLveData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(stemp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,SubLveData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(*tmp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		} else {
			field=FLDNRD(mainfileno,modulename);
			if(field!=NULL)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
						{
							temp=*tmp+RDAstrlen(modulename)+3;
							gn=strtok(temp,",]");
							if(gn!=NULL)
							{
								start=Ratoi(gn);
								gn=strtok(NULL,"]");
								length=Ratoi(gn);
							}
							if(start<1) start=1;

						}
						str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
						if(length==0) length=str_length;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if(field->data.string_value!=NULL && str_length>length)
						{
							temp=stralloc(field->data.string_value);
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(start-1+length))
								temp[start-1+length]=0;
							if(RDAstrlen(temp)>(start-1+length))
								temp[start-1+length]=0;
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} else {
							sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(*field->data.short_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.short_value);
						} else {
							sprintf(stemp,"%d",*field->data.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SLONGV:
					case SCROLLEDLIST:
					case LONGV:
						if(*field->data.integer_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.integer_value);
						} else {
							sprintf(stemp,"%d",*field->data.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case DECIMALV:
					case SDECIMALV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SDOUBLEV:
					case DOLLARS:
					case DOUBLEV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%.0f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%.0f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%c",*field->data.string_value);
						break;
					default:
						prterr("Error field type %d is invalid for %s.",
							field->type,field->name); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=NULL;
						break;
				}
			} else {
				prterr("Error Field [%s] not found.",*tmp);
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
		}
	}
	Rfree(modulename);
	}
	if(diageval)
	{
		prterr("DIAG SubLveData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
static void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent);
	if(definelist->swidget!=NULL) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,RDArsrc *parent)
{
	short ef=0,earntype=0;
	char *username=NULL,deleteflag=FALSE,verified=FALSE;

	if(definelist->swidget!=NULL)
		readallwidgets(definelist);
	if(prevlve!=NULL) freeapplib(prevlve);
	prevlve=APPlibNEW();
	ef=FRSNRDsec(filenum,1);
	while(!ef)
	{
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		FINDFLDGETSHORT(filenum,"TYPE",&earntype);
		FINDFLDGETCHAR(filenum,"SOURCE USER VERIFIED",&verified);
		if(!deleteflag && earntype==1 && !verified)
		{
			FINDFLDGETSTRING(filenum,"SOURCE USER",&username);
			if(!RDAstrcmp(username,USERLOGIN))
			{
				makedflist();
			}
		}
		ef=NXTNRDsec(filenum,1);
	}
        if(prevlve->numlibs<1)
        {
                addAPPlib(prevlve,"No Leaves Used added this session");
        }
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS LEAVES USED",(prevlve->numlibs-1),prevlve))
	{
		updatersrc(parent,"PREVIOUS LEAVES USED");
	}
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(prevlve!=NULL)
	{
		if(prevlve->numlibs==1 && !RDAstrcmp(prevlve->libs[0],
			"No Leaves Used added this session"))
		{
			freeapplib(prevlve);
			prevlve=APPlibNEW();
		}
	} else {
		prevlve=APPlibNEW();
	}
	for(x=1,length=0;x<(definelist->numrscs-5);x+=2)
	{
		pos_member=definelist->rscs+x;
		len_member=definelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(definelist->numrscs-5))
	{
		for(x=1;x<(definelist->numrscs-5);x+=2)
		{
			pos_member=definelist->rscs+x;
			len_member=definelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(filenum,s);
					if(field!=NULL)
					{
						switch(field->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case EXPENDITURE:
							case BALANCESHEET:
							case REVENUE:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(*len_member->value.short_value<RDAstrlen(field->data.string_value))
								{
									sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
								} else {
									sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
								}
								break;
							case DECIMALV:
							case SDECIMALV:
								sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,*field->data.float_value);
								break;
							case SHORTV:
							case SSHORTV:
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.short_value);
								break;
							case LONGV:
							case SLONGV:
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.integer_value);
								break;
							case DOLLARS:
								temp=famt(*field->data.float_value,*len_member->value.short_value);
								sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
								if(temp!=NULL) Rfree(temp);
								break;
							case DOUBLEV:
							case SDOUBLEV:
								sprintf(&hold[lcount],"%*.0f ",*len_member->value.short_value,*field->data.float_value);
								break;
							case BOOLNS:
								sprintf(&hold[lcount],"%c ",(*field->data.string_value ? 'Y':'N'));
								break;
							case CHARACTERS:
								sprintf(&hold[lcount],"%c ",*field->data.string_value);
								break;
							case BUTTONS:
							case SCROLLEDLIST:
								prterr("Huh? Do a %s?",standardfieldtypes[field->type]);
								break;
							default:
								prterr("Invalid Field Type [%d]",field->type);
								break;
						
						}
						lcount+=*len_member->value.short_value+1;
					} else {
						prterr("Error field [%s] not found.",s);
						if(s!=NULL) Rfree(s);
					}
				}
			}
		}
		++cur;
	}
	addAPPlib(prevlve,hold);
	++counter;
	if(hold!=NULL) Rfree(hold);
}
void definelistfunc(RDArsrc *parent)
{
	updateSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent,FALSE);
}
static void set_defaults(RDArsrc *parent,short initial)
{
	int selprev=0;
	char *perid1=NULL,*lvedescid1=NULL;

	FINDRSCGETINT(parent,"PREVIOUS LEAVES USED",&selprev);
	DefaultResetScreen(parent,TRUE);
	changeper(parent);
	changesub(parent,initial);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	getsupportingrecords(parent,TRUE);
	if(perid1!=NULL) Rfree(perid1);
	if(perid1!=NULL) Rfree(perid1);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void check_peridcb(RDArsrc *parent)
{
	readwidget(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]");
	readwidget(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	check_perid(parent,FALSE);
	changeper(parent);
	updateSCRNvirtuals(parent);
}
static short check_perid(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*lvedescid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE IDENTIFICATION]",&lvedescid1);
	if(dmgnum!=(-1))
	{
		ZERNRD(dmgnum);
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(dmgnum,1))
		{
			KEYNRD(dmgnum,1);
			warnmessage=Rmalloc(RDAstrlen(perid1)+300);
			sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(perid1!=NULL) Rfree(perid1);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+300);
				sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(perid1!=NULL) Rfree(perid1);
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(dmgnum,"CURRENT EMPLOYEE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(perid1)+300);
					sprintf(warnmessage,"The Personnel Identification of [%s] is not a CURRENT EMPLOYEE in the [PRSNNL][PERDMG] data file.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","NOT CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(parent,updatesupporting);
					if(lvedescid1!=NULL) Rfree(lvedescid1);
					if(perid1!=NULL) Rfree(perid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(perid1!=NULL) Rfree(perid1);
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return(FALSE);
	}
	getsupportingrecords(parent,updatesupporting);
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	return(TRUE);
}
static void check_lveidcb(RDArsrc *parent)
{
	char *perid1=NULL,*lvedescid1=NULL;
	int x=0;

	readwidget(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	readwidget(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(AddList) /* treat like a Search Resource */
	{
		if(lvedescid1!=NULL)
		{
			if(RDAstrlen(lvedescid1)!=0)
			{
				for(x=0;x<lvedesclist->numlibs;++x)
				{
					if(strncmp(lvedescid1,lvedesclist->libs[x],RDAstrlen(lvedescid1))<=0)
					{
						break;
					}
				}
				if(x>=lvedesclist->numlibs) x=(lvedesclist->numlibs-1);
			}
		}
		FINDRSCSETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",x);
		FINDRSCSETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedesclist->libs[x]);
		updatersrc(parent,"LEAVE DESCRIPTION IDENTIFICATIONS");
		updatersrc(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
		getsupportingrecords(parent,TRUE);
	} else {
		check_lveid(parent,TRUE);
	}
	compute_sub_units(parent);
	compute_abs_units(parent);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	if(perid1!=NULL) Rfree(perid1);
	updateSCRNvirtuals(parent);
}
static short check_lveid(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*lvedescid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(ldescnum!=(-1))
	{
		ZERNRD(ldescnum);
		FINDFLDSETSTRING(ldescnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(ldescnum,1))
		{
			KEYNRD(ldescnum,1);
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+300);
			sprintf(warnmessage,"The Leave Description Identification of [%s] is an invalid value.  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(ldescnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(lvedescid1)+300);
				sprintf(warnmessage,"The Leave Description Identification of [%s] is a deleted value.  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				if(perid1!=NULL) Rfree(perid1);
				return(FALSE);
			}
		}
	} else {
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	}
	if(lmsrnum!=(-1))
	{
		ZERNRD(lmsrnum);
		FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(lmsrnum,1))
		{
			KEYNRD(lmsrnum,1);
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+RDAstrlen(perid1)+300);
			sprintf(warnmessage,"The Leave Description Identification of [%s] is an invalid value for the Personnel Identification of [%s].  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1),(perid1==NULL
?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(lmsrnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(lvedescid1)+RDAstrlen(perid1)+300);
				sprintf(warnmessage,"The Leave Description Identification of [%s] is a deleted value for the Personnel Identification of [%s].  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1),(perid1==NULL
?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				if(perid1!=NULL) Rfree(perid1);
				return(FALSE);
			}
		}
	} else {
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	} 
	getsupportingrecords(parent,updatesupporting);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	if(perid1!=NULL) Rfree(perid1);
	return(TRUE);
}
static void check_subidcb(RDArsrc *parent)
{
	readwidget(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION");
	changesub(parent,FALSE);
	updateSCRNvirtuals(parent);
}
static short check_subid(RDArsrc *parent,short updatesupporting)
{
	char *subperid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(subdmgnum!=(-1))
	{
		ZERNRD(subdmgnum);
		FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",subperid1);
		if(EQLNRDsec(subdmgnum,1))
		{
			KEYNRD(subdmgnum,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is an invalid value.  It does not exist in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record.",
				(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(subdmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is a deleted value in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record.",(subperid1==NULL?"":subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(subdmgnum,"CURRENT EMPLOYEE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
					sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is not a CURRENT EMPLOYEE in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record.",(subperid1==NULL?"":subperid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","NOT CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(parent,updatesupporting);
					if(subperid1!=NULL) Rfree(subperid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	if(submstr!=(-1))
	{
		ZERNRD(submstr);
		FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",subperid1);
		if(EQLNRDsec(submstr,1))
		{
			KEYNRD(submstr,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is an invalid value.  It does not exist in the [SUBMGT][SUBMASTR] data file.  Please select a valid Substitute Personnel Identification to save this record.",
				(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(submstr,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is a deleted value in the [SUBMGT][SUBMASTR] data file.   Please select a valid Substitute Personnel Identification to save this record.",(subperid1==NULL?"":subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(submstr,"ACTIVE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
					sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is not ACTIVE in the [SUBMGT][SUBMASTR] data file.   Please select a valid Substitute Personnel Identification to save this record.",(subperid1==NULL?"":subperid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(parent,updatesupporting);
					if(subperid1!=NULL) Rfree(subperid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	getsupportingrecords(parent,updatesupporting);
	if(subperid1!=NULL) Rfree(subperid1);
	return(TRUE);
}
static void check_jobnocb(RDArsrc *parent)
{
	char *perid1=NULL,*jobnostr=NULL;
	int x=0,jobno1=(-1);

	readwidget(parent,"JOB NUMBER");
	readwidget(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]");
	FINDRSCGETINT(parent,"JOB NUMBER",&jobno1);
	jobnostr=Rmalloc(7);
	sprintf(jobnostr,"%6d",jobno1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(AddList) /* treat like a Search Resource */
	{
		if(jobnostr!=NULL)
		{
			for(x=0;x<jobnolist->numlibs;++x)
			{
				if(strncmp(jobnostr,jobnolist->libs[x],RDAstrlen(jobnostr))<=0)
				{
					break;
				}
			}
			if(x>=jobnolist->numlibs) x=(jobnolist->numlibs-1);
		}
		FINDRSCSETINT(parent,"JOB NUMBERS",x);
		jobno1=Ratoi(jobnostr);
		FINDRSCSETINT(parent,"JOB NUMBER",jobno1);
		updatersrc(parent,"JOB NUMBERS");
		updatersrc(parent,"JOB NUMBER");
		getsupportingrecords(parent,TRUE);
	} else {
		check_jobno(parent,TRUE);
	}
	if(jobnostr!=NULL) Rfree(jobnostr);
	if(perid1!=NULL) Rfree(perid1);
	updateSCRNvirtuals(parent);
}
static short check_jobno(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*warnmessage=NULL;
	int jobno1=(-1);
	char delflag=FALSE;
	
	FINDRSCGETINT(parent,"JOB NUMBER",&jobno1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(jmstnum!=(-1))
	{
		ZERNRD(jmstnum);
		FINDFLDSETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETINT(jmstnum,"JOB NUMBER",jobno1);
		if(EQLNRDsec(jmstnum,1))
		{
			KEYNRD(jmstnum,1);
			warnmessage=Rmalloc(RDAstrlen(perid1)+300);
			sprintf(warnmessage,"The Job Number of [%8d] is an invalid value for the Personnel Identification of [%s].  Please select a valid Job Number to save this record.",jobno1,(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(jmstnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+300);
				sprintf(warnmessage,"The Job Number of [%8d] is a deleted value for the Personnel Identification of [%s].  Please select a valid Job Number to save this record.",jobno1,(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(perid1!=NULL) Rfree(perid1);
				return(FALSE);
			}
		}
	} else {
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	}
	getsupportingrecords(parent,updatesupporting);
	if(perid1!=NULL) Rfree(perid1);
	return(TRUE);
}
static void check_subjobnocb(RDArsrc *parent)
{
	char *subperid1=NULL,*subjobnostr=NULL;
	int x=0,subjobno1=(-1);

	readwidget(parent,"SUBSTITUTE JOB NUMBER");
	readwidget(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION");
	FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBER",&subjobno1);
	subjobnostr=Rmalloc(7);
	sprintf(subjobnostr,"%6d",subjobno1);
	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(AddList) /* treat like a Search Resource */
	{
		if(subjobnostr!=NULL)
		{
			for(x=0;x<subjobnolist->numlibs;++x)
			{
				if(strncmp(subjobnostr,subjobnolist->libs[x],RDAstrlen(subjobnostr))<=0)
				{
					break;
				}
			}
			if(x>=subjobnolist->numlibs) x=(subjobnolist->numlibs-1);
		}
		FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBERS",x);
		subjobno1=Ratoi(subjobnostr);
		FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBER",subjobno1);
		updatersrc(parent,"SUBSTITUTE JOB NUMBERS");
		updatersrc(parent,"SUBSTITUTE JOB NUMBER");
		getsupportingrecords(parent,TRUE);
	} else {
		check_subjobno(parent,TRUE);
	}
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	if(subperid1!=NULL) Rfree(subperid1);
	updateSCRNvirtuals(parent);
}
static short check_subjobno(RDArsrc *parent,short updatesupporting)
{
	char *subperid1=NULL,*warnmessage=NULL;
	int subjobno1=(-1);
	char delflag=FALSE;
	
	FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBER",&subjobno1);
	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(subjmstnum!=(-1))
	{
		ZERNRD(subjmstnum);
		FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",subperid1);
		FINDFLDSETINT(subjmstnum,"JOB NUMBER",subjobno1);
		if(EQLNRDsec(subjmstnum,1))
		{
			KEYNRD(subjmstnum,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Job Number of [%8d] is an invalid value for the Substitute Personnel Identification of [%s].  Please select a valid Job Number to save this record.",subjobno1,(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(subjmstnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Job Number of [%8d] is a deleted value for the Substitute Personnel Identification of [%s].  Please select a valid Job Number to save this record.",subjobno1,(subperid1==NULL?"":subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	getsupportingrecords(parent,updatesupporting);
	if(subperid1!=NULL) Rfree(subperid1);
	return(TRUE);
}
static void getsupportingrecords(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*subperid1=NULL,*lvedescid1=NULL,*temp=NULL;
	int jobno1=(-1)/*,subjobno1=(-1)*/;

	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	if(dmgnum!=(-1))
	{
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(dmgnum,1)) KEYNRD(dmgnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(dmgnum,parent);
		} else {
			filerecord2rsrc(dmgnum,parent);
		}
	}
	if(jmstnum!=(-1))
	{
		FINDRSCGETINT(parent,"JOB NUMBER",&jobno1);
		FINDFLDSETINT(jmstnum,"JOB NUMBER",jobno1);
		FINDFLDSETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(jmstnum,1)) KEYNRD(jmstnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(jmstnum,parent);
		} else {
			filerecord2rsrc(jmstnum,parent);
		}
	}
	if(posmstr!=(-1))
	{
		COPYFIELD(jmstnum,posmstr,"POSITION IDENTFICATION");
		if(EQLNRDsec(posmstr,1)) KEYNRD(posmstr,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(posmstr,parent);
		} else {
			filerecord2rsrc(posmstr,parent);
		}
	}
	if(poslmsr!=(-1))
	{
		COPYFIELD(posmstr,poslmsr,"LOCATION IDENTIFICATION");
		if(EQLNRDsec(poslmsr,1)) KEYNRD(poslmsr,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(poslmsr,parent);
		} else {
			filerecord2rsrc(poslmsr,parent);
		}
	}
	if(lmsrnum!=(-1))
	{
		FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(lmsrnum,1)) KEYNRD(lmsrnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(lmsrnum,parent);
		} else {
			filerecord2rsrc(lmsrnum,parent);
		}
	}
	if(posper!=(-1))
	{
		COPYFIELD(posmstr,posper,"POSITION IDENTIFICATION");
		FINDFLDSETSTRING(posper,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETINT(posper,"JOB NUMBER",jobno1);
		if(EQLNRDsec(posper,1)) KEYNRD(posper,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(posper,parent);
		} else {
			filerecord2rsrc(posper,parent);
		}
	}
	if(posgrs!=(-1))
	{
		COPYFIELD(posmstr,posgrs,"GROSS IDENTIFICATION");
		if(EQLNRDsec(posgrs,1)) KEYNRD(posgrs,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(posgrs,parent);
		} else {
			filerecord2rsrc(posgrs,parent);
		}
	}
	if(poscls!=(-1))
	{
		COPYFIELD(posmstr,poscls,"CLASS IDENTIFICATION");
		if(EQLNRDsec(poscls,1)) KEYNRD(poscls,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(poscls,parent);
		} else {
			filerecord2rsrc(poscls,parent);
		}
	}
	if(posdedc!=(-1))
	{
		COPYFIELD(posmstr,posdedc,"DEDUCTION CLASS IDENTIFICATION");
		if(EQLNRDsec(posdedc,1)) KEYNRD(posdedc,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(posdedc,parent);
		} else {
			filerecord2rsrc(posdedc,parent);
		}
	}
	if(possmst!=(-1))
	{
		COPYFIELD(posmstr,possmst,"SERVICE AREA IDENTIFICATION");
		if(EQLNRDsec(possmst,1)) KEYNRD(possmst,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(possmst,parent);
		} else {
			filerecord2rsrc(possmst,parent);
		}
	}
	if(posngms!=(-1))
	{
		COPYFIELD(posmstr,posngms,"NEGOTIATING GROUP IDENTIFICATION");
		if(EQLNRDsec(posngms,1)) KEYNRD(posngms,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(posngms,parent);
		} else {
			filerecord2rsrc(posngms,parent);
		}
	}
	if(ldescnum!=(-1))
	{
		FINDFLDSETSTRING(ldescnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(ldescnum,1)) KEYNRD(ldescnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(ldescnum,parent);
		} else {
			filerecord2rsrc(ldescnum,parent);
		}
	}
	if(ltypenum!=(-1))
	{
		COPYFIELD(ldescnum,ltypenum,"LEAVE TYPE IDENTIFICATION");
		if(EQLNRDsec(ltypenum,1)) KEYNRD(ltypenum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(ltypenum,parent);
		} else {
			filerecord2rsrc(ltypenum,parent);
		}
	}
	if(subdmgnum!=(-1))
	{
		FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
		FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",temp);
		if(EQLNRDsec(subdmgnum,1)) KEYNRD(subdmgnum,1);
		if(updatesupporting)
		{
			updatesinglerecord2rsrc(subdmgnum,parent);
		} else {
			singlefilerecord2rsrc(subdmgnum,parent);
		}
	}
	/*
	if(subjmstnum!=(-1))
	{
		FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBER",&subjobno1);
		FINDFLDSETINT(subjmstnum,"JOB NUMBER",subjobno1);
		FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",subperid1);
		if(EQLNRDsec(subjmstnum,1)) KEYNRD(subjmstnum,1);
		if(updatesupporting)
		{
			updatesinglerecord2rsrc(subjmstnum,parent);
		} else {
			singlefilerecord2rsrc(subjmstnum,parent);
		}
	}
	if(subposmstr!=(-1))
	{
		COPYFIELD(subjmstnum,subposmstr,"POSITION IDENTFICATION");
		if(EQLNRDsec(subposmstr,1)) KEYNRD(subposmstr,1);
		if(updatesupporting)
		{
			updatesinglerecord2rsrc(subposmstr,parent);
		} else {
			filerecord2rsrc(subposmstr,parent);
		}
	}
	*/
	if(perid1!=NULL) Rfree(perid1);
	if(subperid1!=NULL) Rfree(subperid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	if(temp!=NULL) Rfree(temp);
}
static void save_check(RDArsrc *parent)
{
	int sellvedesc=0;
	char *lvedescid1=NULL,*warnmessage=NULL,allnegbal=TRUE;
	double amount=0.0,beginningamt=0.0,tempamt=0.0;
	double earningsamt=0.0,upearningsamt=0.0,usedamt=0.0,upusedamt=0.0;

	readallwidgets(parent);
	if(check_perid(parent,FALSE)==FALSE) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
		if((RDAstrcmp(lvedesclist->libs[sellvedesc],"No Leave Descriptions Available")) &&
		(RDAstrcmp(lvedesclist->libs[sellvedesc],"Not Applicable")))
		{
			lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
		}
	} else {
		FINDRSCGETSTRING(parent,"[LVEMGT][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	}
	FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	if(check_lveid(parent,FALSE)==FALSE)
	{
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	if(check_subid(parent,FALSE)==FALSE)
	{
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	if(check_jobno(parent,FALSE)==FALSE)
	{
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	if(check_subjobno(parent,FALSE)==FALSE)
	{
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	getsupportingrecords(parent,TRUE);
	FINDRSCGETDOUBLE(parent,"[LVETRAN][AMOUNT]",&amount);
	if(amount==0.0)
	{
		ERRORDIALOG("INVALID AMOUNT!","This Leave Used Transaction can not be saved with an AMOUNT of [$  0.00].",NULL,FALSE);
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	FINDFLDGETCHAR(ldescnum,"ALLOW NEG BALANCES",&allnegbal);
	if(!allnegbal)
	{
		FINDFLDGETDOUBLE(lmsrnum,"BEGINNING BALANCE",&beginningamt);
		FINDFLDGETDOUBLE(lmsrnum,"PROCESSED EARNINGS",&earningsamt);
		FINDFLDGETDOUBLE(lmsrnum,"UNPROCESSED EARNINGS",&upearningsamt);
		FINDFLDGETDOUBLE(lmsrnum,"PROCESSED USED",&usedamt);
		FINDFLDGETDOUBLE(lmsrnum,"UNPROCESSED USED",&upusedamt);
		tempamt=beginningamt+earningsamt+upearningsamt-usedamt-upusedamt-amount;
		if(tempamt<0.0)
		{
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+50+401);
			sprintf(warnmessage,
				"The Leave Description Identification of [%s] is not allowed to have a Negative Balance.  This Transaction would make the Balance of this Leave Description to be negative.  The calculation of (Beginning Balance[%10.3f] + Processed[%10.3f] and Unprocessed[%10.3f] Earnings - Processed[%10.3f] and Unprocessed[%10.3f] Used) is used to in the determinization of this balance.",
				(lvedescid1==NULL?"":lvedescid1),beginningamt/100,earningsamt/100,upearningsamt/100,usedamt/100,upusedamt/100);
			WARNINGDIALOG("WARNING DIALOG SCREEN","CAN'T ALLOW NEGATIVE BALANCE!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			return;
		}
	}
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	save_record(parent);
}
static void save_record(RDArsrc *parent)
{
	int x=0,lf=0,sellvedesc=0,seljobno=0,jobno1=(-1);
	int selsubjobno=0,subjobno1=(-1);
	double amount=0.0,tempamt=0.0,sub_units=0.0,abs_units=0.0;
	int temptimes=0;
	char *rcddesc=NULL,deleteflag=FALSE;
	char *perid1=NULL,*lvedescid1=NULL,*subperid1=NULL;
	char *date=NULL,*timex=NULL,*temp=NULL;
	char flag=FALSE;
	RDArmem *member=NULL;

	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
		if((RDAstrcmp(lvedesclist->libs[sellvedesc],"No Leave Descriptions Available")) &&
		(RDAstrcmp(lvedesclist->libs[sellvedesc],"Not Applicable")))
		{
			lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
		} else {
			FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		}
	} else {
		FINDRSCGETSTRING(parent,"[LVEMGT][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	}
	if(FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
		jobno1=Ratoi(jobnolist->libs[seljobno]);
		FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBERS",&selsubjobno);
		subjobno1=Ratoi(subjobnolist->libs[selsubjobno]);
	} else {
		FINDRSCGETINT(parent,"JOB NUMBER",&jobno1);
		FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBER",&subjobno1);
	}
	rsrc2filerecord(filenum,parent);
	FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(filenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(filenum,"SOURCE USER VERIFIED",deleteflag);
	FINDFLDSETSHORT(filenum,"TYPE",1);
	FINDFLDGETDOUBLE(filenum,"AMOUNT",&amount);
	if((x=FINDRSC(parent,"[FINREC][ENTRY DATE]"))!=(-1))
	{
		member=parent->rscs+x;
		if(member->w==NULL || (member->w!=NULL && (member->value.string_value==NULL || RDAstrlen(member->value.string_value)<1)))
		{
			date=GETCURRENTDATE();
			lf=RDAstrlen(date);
			if(date!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,date,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
		}
	} else {
		date=GETCURRENTDATE();
		FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date);
	if((x=FINDRSC(parent,"[FINREC][ENTRY TIME]"))!=(-1))
	{
		member=parent->rscs+x;
		if(member->w==NULL || (member->w!=NULL && (member->value.string_value==NULL || RDAstrlen(member->value.string_value)<1)))
		{
			timex=GETCURRENTTIME();
			lf=RDAstrlen(timex);
			if(timex!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,timex,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
		}
	} else {
		timex=GETCURRENTTIME();
		FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
	}
	if(timex!=NULL) Rfree(timex);
	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  RECORDsec() failed without a description returned!");
		}
	} else {
		if(!WRTNRDtrans(filenum,1,"TRANSACTION NUMBER"))
		{
			makedflist();
			FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
			LOCNRD(lmsrnum);
			if(!EQLNRDsec(lmsrnum,1))
			{
				FINDFLDGETDOUBLE(lmsrnum,"UNPROCESSED USED",&tempamt);
				tempamt+=sub_units;
				FINDFLDSETDOUBLE(lmsrnum,"UNPROCESSED USED",tempamt);
				if(WRTNRDtranssec(lmsrnum,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the field [LVEMGT][LVEMSTR][UNPROCESSED USED].",NULL,NULL,TRUE,NULL);
				}
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","RECORD ERROR OCCURRED!","The Equal Record function failed when attempting to Select the [LVEMGT][LVEMSTR] record.",NULL,NULL,TRUE,NULL);
			}
			UNLNRD(lmsrnum);
			FINDFLDSETSTRING(subwknum,"PERSONNEL IDENTIFICATION",subperid1);
			FINDFLDSETSTRING(subwknum,"EMPLOYEE IDENTIFICATION",perid1);
			FINDFLDSETINT(subwknum,"EMPLOYEE JOB NUMBER",jobno1);
			COPYFIELD(jmstnum,subwknum,"POSITION IDENTIFICATION");
			FINDFLDSETINT(subwknum,"JOB NUMBER",subjobno1);
			FINDFLDSETINT(jmstnum,"JOB NUMBER",jobno1);
			COPYFIELD(filenum,subwknum,"ENTRY DATE");
			COPYFIELD(filenum,subwknum,"ENTRY TIME");
			FINDRSCGETSTRING(parent,"[LVETRAN][STARTING DATE]",&temp);
			FINDFLDSETSTRING(subwknum,"STARTING DATE",temp);
			FINDRSCGETSTRING(parent,"[LVETRAN][ENDING DATE]",&temp);
			FINDFLDSETSTRING(subwknum,"ENDING DATE",temp);
			FINDFLDSETSTRING(subwknum,"SOURCE USER",USERLOGIN);
			FINDRSCGETDOUBLE(parent,"SUBSTITUTE UNITS",&sub_units);
			FINDFLDSETDOUBLE(subwknum,"UNITS",sub_units);
			FINDFLDSETCHAR(subwknum,"DELETEFLAG",deleteflag);
			if(WRTNRDtrans(subwknum,1,"TRANSACTION NUMBER"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to write the [SUBMGT][SUBWKMAS] transaction record.",NULL,NULL,TRUE,NULL);
			}
			FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",subperid1);
			LOCNRD(submstr);
			if(!EQLNRD(submstr,1))
			{
				FINDFLDGETDOUBLE(submstr,"ACCEPTED UNITS",&tempamt);
				tempamt+=sub_units;
				FINDFLDSETDOUBLE(submstr,"ACCEPTED UNITS",tempamt);
				FINDFLDGETINT(submstr,"TIMES ACCEPTED",&temptimes);
				temptimes+=1;
				FINDFLDSETINT(submstr,"TIMES ACCEPTED",temptimes);
				if(WRTNRDtranssec(submstr,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBMASTR][ACCEPTED UNITS] and [SUBMGT][SUBMASTR][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
				}
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","RECORD ERROR OCCURRED!","The Equal Record function failed when attempting to Select the [SUBMGT][SUBMASTR] record.",NULL,NULL,TRUE,NULL);
			}
			UNLNRD(submstr);
			FINDFLDSETSTRING(subemp,"PERSONNEL IDENTIFICATION",subperid1);
			FINDFLDSETSTRING(subemp,"EMPLOYEE IDENTIFICATION",perid1);
			LOCNRD(subemp);
			if(!EQLNRD(subemp,1))
			{
				FINDFLDGETDOUBLE(subemp,"ACCEPTED UNITS",&tempamt);
				tempamt+=sub_units;
				FINDFLDSETDOUBLE(subemp,"ACCEPTED UNITS",tempamt);
				FINDFLDGETINT(subemp,"TIMES ACCEPTED",&temptimes);
				temptimes+=1;
				FINDFLDSETINT(subemp,"TIMES ACCEPTED",temptimes);
				if(WRTNRDtranssec(subemp,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][ACCEPTED UNITS] and [SUBMGT][SUBEMP][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
				}
			} else {
				FINDFLDSETCHAR(subemp,"DELETEFLAG",FALSE);
				FINDFLDSETCHAR(subemp,"EMPLOYEE BLOCK",FALSE);
				FINDFLDSETDOUBLE(subemp,"ACCEPTED UNITS",sub_units);
				temptimes=1;
				FINDFLDSETINT(subemp,"TIMES ACCEPTED",temptimes);
				if(WRTNRDtranssec(subemp,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][ACCEPTED UNITS] and [SUBMGT][SUBEMP][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
				}
			}
			UNLNRD(subemp);
			FINDFLDSETSTRING(subpos,"PERSONNEL IDENTIFICATION",subperid1);
			COPYFIELD(jmstnum,subpos,"POSITION IDENTIFICATION");
			LOCNRD(subpos);
			if(!EQLNRD(subpos,1))
			{
				FINDFLDGETDOUBLE(subpos,"ACCEPTED UNITS",&tempamt);
				tempamt+=sub_units;
				FINDFLDSETDOUBLE(subpos,"ACCEPTED UNITS",tempamt);
				FINDFLDGETINT(subpos,"TIMES ACCEPTED",&temptimes);
				temptimes+=1;
				FINDFLDSETINT(subpos,"TIMES ACCEPTED",temptimes);
				if(WRTNRDtranssec(subpos,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][ACCEPTED UNITS] and [SUBMGT][SUBPOS][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
				}
			} else {
				FINDFLDSETCHAR(subpos,"DELETEFLAG",FALSE);
				FINDFLDSETCHAR(subpos,"POSITION BLOCK",FALSE);
				FINDFLDSETDOUBLE(subpos,"ACCEPTED UNITS",sub_units);
				temptimes=1;
				FINDFLDSETINT(subpos,"TIMES ACCEPTED",temptimes);
				if(WRTNRDtranssec(subpos,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][ACCEPTED UNITS] and [SUBMGT][SUBPOS][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
				}
			}
			UNLNRD(subpos);
			FINDFLDSETSTRING(subloc,"PERSONNEL IDENTIFICATION",subperid1);
			COPYFIELD(posmstr,subloc,"LOCATION IDENTIFICATION");
			LOCNRD(subloc);
			if(!EQLNRD(subloc,1))
			{
				FINDFLDGETDOUBLE(subloc,"ACCEPTED UNITS",&tempamt);
				tempamt+=sub_units;
				FINDFLDSETDOUBLE(subloc,"ACCEPTED UNITS",tempamt);
				FINDFLDGETINT(subloc,"TIMES ACCEPTED",&temptimes);
				temptimes+=1;
				FINDFLDSETINT(subloc,"TIMES ACCEPTED",temptimes);
				if(WRTNRDtranssec(subloc,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][ACCEPTED UNITS] and [SUBMGT][SUBLOC][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
				}
			} else {
				FINDFLDSETCHAR(subloc,"DELETEFLAG",FALSE);
				FINDFLDSETCHAR(subloc,"LOCATION BLOCK",FALSE);
				FINDFLDSETDOUBLE(subloc,"ACCEPTED UNITS",sub_units);
				temptimes=1;
				FINDFLDSETINT(subloc,"TIMES ACCEPTED",temptimes);
				if(WRTNRDtranssec(subloc,0,NULL))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][ACCEPTED UNITS] and [SUBMGT][SUBLOC][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
				}
			}
			UNLNRD(subloc);
			FINDFLDSETSTRING(eabsnum,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETINT(eabsnum,"JOB NUMBER",jobno1);
			COPYFIELD(filenum,eabsnum,"ENTRY DATE");
			COPYFIELD(filenum,eabsnum,"ENTRY TIME");
			FIELDCOPY(filenum,"TRANSACTION NUMBER",eabsnum,"LEAVE TRANSACTION NUMBER");
			FINDFLDSETSTRING(eabsnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
			FINDFLDSETSTRING(eabsnum,"LEAVE SELECTED BY",USERLOGIN);
			FINDFLDSETCHAR(eabsnum,"LEAVE SELECTED",TRUE);
			FIELDCOPY(filenum,"ENTRY DATE",eabsnum,"LEAVE SELECTED DATE");
			FIELDCOPY(filenum,"ENTRY TIME",eabsnum,"LEAVE SELECTED TIME");
			FIELDCOPY(subwknum,"TRANSACTION NUMBER",eabsnum,"SUBSTITUTE TRANSACTION NUMBER");
			COPYFIELD(filenum,eabsnum,"DESCRIPTION");
			FIELDCOPY(filenum,"ENTRY DATE",eabsnum,"SUBSTITUTE SELECTED DATE");
			FIELDCOPY(filenum,"ENTRY TIME",eabsnum,"SUBSTITUTE SELECTED TIME");
			COPYFIELD(filenum,eabsnum,"ENTRY TIME");
			FINDFLDSETSTRING(eabsnum,"SOURCE USER",USERLOGIN);
			FINDFLDSETSTRING(eabsnum,"SUBSTITUTE SELECTED BY",USERLOGIN);
			FINDFLDSETCHAR(eabsnum,"SUBSTITUTE SELECTED",TRUE);
			FINDFLDSETCHAR(eabsnum,"DELETEFLAG",deleteflag);
			FINDFLDSETCHAR(eabsnum,"SOURCE USER VERIFIED",deleteflag);
			FINDFLDSETCHAR(eabsnum,"REPLACEMENT WORKER",TRUE);
			FINDRSCGETCHAR(parent,"ACTUALLY ABSENT",&flag);
			FINDFLDSETCHAR(eabsnum,"ACTUALLY ABSENT",flag);
			FINDRSCGETSTRING(parent,"[LVETRAN][STARTING DATE]",&temp);
			FINDFLDSETSTRING(eabsnum,"STARTING DATE",temp);
			FINDRSCGETSTRING(parent,"STARTING TIME",&temp);
			FINDFLDSETSTRING(eabsnum,"STARTING TIME",temp);
			FINDRSCGETSTRING(parent,"[LVETRAN][ENDING DATE]",&temp);
			FINDFLDSETSTRING(eabsnum,"ENDING DATE",temp);
			FINDRSCGETSTRING(parent,"ENDING TIME",&temp);
			FINDFLDSETSTRING(eabsnum,"ENDING TIME",temp);
			FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
			FINDFLDSETSTRING(eabsnum,"SUBSTITUTE PERSONNEL IDENTIFICATION",temp);
			FINDRSCGETDOUBLE(parent,"ABSENTEE UNITS",&abs_units);
			FINDFLDSETDOUBLE(eabsnum,"UNITS",abs_units);
			if(WRTNRDtrans(eabsnum,1,"TRANSACTION NUMBER"))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to write the [EMPABS][EMPABTR] transaction record.",NULL,NULL,TRUE,NULL);
			}
			if(temp!=NULL) Rfree(temp);
		} else {
			ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed, the record you attempted to save was not created. Please try again.",NULL,FALSE);
		}
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS LEAVES USED",(prevlve->numlibs-1),
		prevlve))
	{
		updatersrc(parent,"PREVIOUS LEAVES USED");
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	if(subperid1!=NULL) Rfree(subperid1);
	if(temp!=NULL) Rfree(temp);
	updateSCRNvirtuals(parent);
}
static void quit_check(RDArsrc *parent)
{
	quit_record(parent);
}
static void quit_record_freens(RDArsrc *parent)
{
	counter=0;
	quit_record_free(parent);
}
static void quit_record_free(RDArsrc *parent)
{
	if(definelist!=NULL) 
	{
		if(definelist->swidget!=NULL) killwindow(definelist);
		free_rsrc(definelist);
	}
	if(parent!=NULL) free_rsrc(parent);
	if(mbl_perdmg!=NULL) FREE_BROWSE_EXIT(mbl_perdmg);
	if(mbl_submstr!=NULL) FREE_BROWSE_EXIT(mbl_submstr);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(eabsnum!=(-1)) CLSNRD(eabsnum);
	if(submstr!=(-1)) CLSNRD(submstr);
	if(subwknum!=(-1)) CLSNRD(subwknum);
	if(subemp!=(-1)) CLSNRD(subemp);
	if(subpos!=(-1)) CLSNRD(subpos);
	if(subloc!=(-1)) CLSNRD(subloc);
	if(jmstnum!=(-1)) CLSNRD(jmstnum);
	if(posmstr!=(-1)) CLSNRD(posmstr);
	if(poslmsr!=(-1)) CLSNRD(poslmsr);
	if(posper!=(-1)) CLSNRD(posper);
	if(posgrs!=(-1)) CLSNRD(posgrs);
	if(poscls!=(-1)) CLSNRD(poscls);
	if(posdedc!=(-1)) CLSNRD(posdedc);
	if(possmst!=(-1)) CLSNRD(possmst);
	if(posngms!=(-1)) CLSNRD(posngms);
	if(dmgnum!=(-1)) CLSNRD(dmgnum);
	if(subdmgnum!=(-1)) CLSNRD(subdmgnum);
	if(lmsrnum!=(-1)) CLSNRD(lmsrnum);
	if(ldescnum!=(-1)) CLSNRD(ldescnum);
	if(prevlve!=NULL) freeapplib(prevlve);
	if(lvedesclist!=NULL) freeapplib(lvedesclist);
	if(leave_description!=NULL) Rfree(leave_description);
	if(personnel_identification!=NULL) Rfree(personnel_identification);
	if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
	if(counter>0) RUNREPORT(module,audit_reportname,NULL,NULL,FALSE,TRUE,NULL);
	if(audit_reportname!=NULL) Rfree(audit_reportname);
}
static void quit_record(RDArsrc *parent)
{
	quit_record_free(parent);
	ShutdownSubsystems();
	exit(0);
}
static void changeper(RDArsrc *parent)
{
	short ef=0;
	short keyn=0,keynumber=0;
	int sellvedesc=0,seljobno=0,jobno1=(-1);
	char active=TRUE,deleteflag=FALSE;
	char *e=NULL;
	char *perid1=NULL,*perid2=NULL,*lvedescid1=NULL,*jobnostr=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		if(RDAstrcmp(perid1,personnel_identification) || personnel_identification==NULL)
		{
			if(lvedesclist!=NULL) freeapplib(lvedesclist);
			lvedesclist=APPlibNEW();
			if(jobnolist!=NULL) freeapplib(jobnolist);
			jobnolist=APPlibNEW();
			FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
                        if((keyn=KEYNUMBER(lmsrnum,"LVEMSTR KEY"))!=(-1))
                        {
                                keynumber=keyn;
                        } else {
                                keynumber=1;
                        }
			ZERNRD(lmsrnum);
			FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
			ef=GTENRDsec(lmsrnum,keynumber);
			e=stralloc("Updating Leave Descriptions List");
			tmprsrc=diagscrn(lmsrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
					if(tmprsrc->swidget!=NULL)
						ForceWindowUpdate(tmprsrc);
			}
			while(!ef)
			{
				listupdated=FALSE;
				FINDFLDGETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",&perid2);
				if(RDAstrcmp(perid2,perid1)) break;
				FINDFLDGETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",&lvedescid1);
				FINDFLDGETCHAR(lmsrnum,"ACTIVE",&active);
				FINDFLDGETCHAR(lmsrnum,"DELETEFLAG",&deleteflag);
				if(active && !deleteflag)
				{
					FINDFLDSETSTRING(ldescnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
					if(EQLNRDsec(ldescnum,1))
					{
						KEYNRD(ldescnum,1);
					} else {
						FINDFLDGETCHAR(ldescnum,"ACTIVE",&active);
						FINDFLDGETCHAR(ldescnum,"DELETEFLAG",&deleteflag);
						if(active && !deleteflag)
						{
							addAPPlib(lvedesclist,lvedescid1);
							listupdated=TRUE;
						}
					}
				}
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				ef=NXTNRDsec(lmsrnum,keynumber);
				if(tmprsrc!=NULL)
					if(tmprsrc->swidget!=NULL)
						update_diagnostic(tmprsrc,listupdated);
			}
			if(tmprsrc!=NULL)
			{
				if(tmprsrc->swidget!=NULL) killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
                        if((keyn=KEYNUMBER(jmstnum,"JOBMSTR KEY"))!=(-1))
                        {
                                keynumber=keyn;
                        } else {
                                keynumber=1;
                        }
			ZERNRD(jmstnum);
			FINDFLDSETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",perid1);
			ef=GTENRDsec(jmstnum,keynumber);
			e=stralloc("Updating Job Numbers List");
			tmprsrc=diagscrn(jmstnum,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
					if(tmprsrc->swidget!=NULL)
						ForceWindowUpdate(tmprsrc);
			}
			while(!ef)
			{
				listupdated=FALSE;
				FINDFLDGETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",&perid2);
				if(RDAstrcmp(perid2,perid1)) break;
				FINDFLDGETINT(jmstnum,"JOB NUMBER",&jobno1);
				FINDFLDGETCHAR(jmstnum,"ACTIVE",&active);
				FINDFLDGETCHAR(jmstnum,"DELETEFLAG",&deleteflag);
				if(active && !deleteflag)
				{
					jobnostr=Rrealloc(jobnostr,9);
					sprintf(jobnostr,"%8d",jobno1);
					addAPPlib(jobnolist,jobnostr);
					listupdated=TRUE;
				}
				if(jobnostr!=NULL) Rfree(jobnostr);
				ef=NXTNRDsec(jmstnum,keynumber);
				if(tmprsrc!=NULL)
					if(tmprsrc->swidget!=NULL)
						update_diagnostic(tmprsrc,listupdated);
			}
			if(tmprsrc!=NULL)
			{
				if(tmprsrc->swidget!=NULL) killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
		}
		if(lvedesclist->numlibs<1) addAPPlib(lvedesclist,"No Leave Descriptions Available");
		if(sellvedesc>=lvedesclist->numlibs) sellvedesc=0;
		if(!FINDRSCLISTAPPlib(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",sellvedesc,lvedesclist))
		{
			updatersrc(parent,"LEAVE DESCRIPTION IDENTIFICATIONS");
		}
		if(jobnolist->numlibs<1) addAPPlib(jobnolist,"No Jobs Available");
		if(seljobno>=jobnolist->numlibs) seljobno=0;
		if(!FINDRSCLISTAPPlib(parent,"JOB NUMBERS",seljobno,jobnolist))
		{
			updatersrc(parent,"JOB NUMBERS");
		}
		if(personnel_identification!=NULL) Rfree(personnel_identification);
		personnel_identification=stralloc(perid1);
		if(leave_description!=NULL) Rfree(leave_description);
		job_number=(-1);
	}
	changelve(parent);
	changejobno(parent);
	if(jobnostr!=NULL) Rfree(jobnostr);
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
}
static void compute_sub_units(RDArsrc *parent)
{
	char *expressn=NULL,*warnmessage=NULL,*lvedescid1=NULL;
	double sub_units=0.0;
	PassableStruct *p=NULL;

	readwidget(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=parent;
	p->a=&filenum;
	FINDFLDGETSTRING(ldescnum,"LEAVE UNITS TO SUBSTITUTE UNITS FORMULA",&expressn);
	if(expressn!=NULL && RDAstrlen(expressn)>0)
	{
		sub_units=EVALUATEdbl(expressn,SubLveData,p);
		if(sub_units==0.0)
		{
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+RDAstrlen(expressn)+255);
			sprintf(warnmessage,"The Substitute's Units Evaluated to [0.0] from the expression [%s] in [LVEMGT][LVEDSC][LVEAVE UNITS TO SUBSTITUTE UNITS FORMULA] file field for the Leave Description Identification of [%s].",
			expressn,(lvedescid1==NULL?"":lvedescid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","SUBSTITUE UNITS EVALUTED TO ZERO!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
		}
	} else {
		warnmessage=Rmalloc(RDAstrlen(lvedescid1)+210);
		sprintf(warnmessage,"The expression in [LVEMGT][LVEDSC][LEAVE UNITS TO SUBSTITUTE UNITS FORMULA] file field is empty for the Leave Description Identification of [%s].",(lvedescid1==NULL?"":lvedescid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO LEAVE UNITS TO SUBSTITUTE UNITS FORMULA!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
	}
	if(expressn!=NULL) Rfree(expressn);
	if(p!=NULL) Rfree(p);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	FINDRSCSETDOUBLE(parent,"SUBSTITUTE UNITS",sub_units);
	updatersrc(parent,"SUBSTITUTE UNITS");
}
static void compute_abs_units(RDArsrc *parent)
{
	char *expressn=NULL,*warnmessage=NULL,*lvedescid1=NULL;
	double abs_units=0.0;
	PassableStruct *p=NULL;

	readwidget(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=parent;
	p->a=&filenum;
	FINDFLDGETSTRING(ldescnum,"LEAVE UNITS TO ABSENTEE UNITS FORMULA",&expressn);
	if(expressn!=NULL && RDAstrlen(expressn)>0)
	{
		abs_units=EVALUATEdbl(expressn,SubLveData,p);
		if(abs_units==0.0)
		{
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+RDAstrlen(expressn)+255);
			sprintf(warnmessage,"The Absentee Units Evaluated to [0.0] from the expression [%s] in [LVEMGT][LVEDSC][LVEAVE UNITS TO SUBSTITUTE UNITS FORMULA] file field for the Leave Description Identification of [%s].",
			expressn,(lvedescid1==NULL?"":lvedescid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","ABSENTEE UNITS EVALUTED TO ZERO!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
		}
	} else {
		warnmessage=Rmalloc(RDAstrlen(lvedescid1)+210);
		sprintf(warnmessage,"The expression in [LVEMGT][LVEDSC][LEAVE UNITS TO SUBSTITUTE UNITS FORMULA] file field is empty for the Leave Description Identification of [%s].",(lvedescid1==NULL?"":lvedescid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO LEAVE UNITS TO ABSENTEE UNITS FORMULA!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
	}
	if(expressn!=NULL) Rfree(expressn);
	if(p!=NULL) Rfree(p);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	FINDRSCSETDOUBLE(parent,"ABSENTEE UNITS",abs_units);
	updatersrc(parent,"ABSENTEE UNITS");
}
static void changesub(RDArsrc *parent,short initial)
{
	short ef=0;
	short keyn=0,keynumber=0;
	char active=TRUE,deleteflag=FALSE;
	char *e=NULL;
	char *subperid1=NULL,*subperid2=NULL,*subjobnostr=NULL;
	int selsubjobno=0,subjobno1=(-1);
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
/*
	if(check_subid(parent,TRUE)==FALSE)
	{
		if(subperid1!=NULL) Rfree(subperid1);
		return;
	}
*/
	if(RDAstrcmp(subperid1,subpersonnel_identification) || subpersonnel_identification==NULL)
	{
		getsupportingrecords(parent,(initial==TRUE?FALSE:TRUE));
		if(AddList==TRUE)
		{
			if(subjobnolist!=NULL) freeapplib(subjobnolist);
			subjobnolist=APPlibNEW();
			FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBERS",&selsubjobno);
			if((keyn=KEYNUMBER(subjmstnum,"JOBMSTR KEY"))!=(-1))
			{
				keynumber=keyn;
			} else {
				keynumber=1;
			}
			ZERNRD(subjmstnum);
			FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",subperid1);
			ef=GTENRDsec(subjmstnum,keynumber);
			e=stralloc("Updating Substitute Job Numbers List");
			tmprsrc=diagscrn(subjmstnum,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
					if(tmprsrc->swidget!=NULL)
						ForceWindowUpdate(tmprsrc);
			}
			while(!ef)
			{
				listupdated=FALSE;
				FINDFLDGETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",&subperid2);
				if(RDAstrcmp(subperid2,subperid1)) break;
				FINDFLDGETINT(subjmstnum,"JOB NUMBER",&subjobno1);
				FINDFLDGETCHAR(subjmstnum,"ACTIVE",&active);
				FINDFLDGETCHAR(subjmstnum,"DELETEFLAG",&deleteflag);
				if(active && !deleteflag)
				{
					subjobnostr=Rrealloc(subjobnostr,9);
					sprintf(subjobnostr,"%8d",subjobno1);
					addAPPlib(subjobnolist,subjobnostr);
					listupdated=TRUE;
				}
				if(subjobnostr!=NULL) Rfree(subjobnostr);
				ef=NXTNRDsec(subjmstnum,keynumber);
				if(tmprsrc!=NULL)
					if(tmprsrc->swidget!=NULL)
						update_diagnostic(tmprsrc,listupdated);
			}
			if(tmprsrc!=NULL)
			{
				if(tmprsrc->swidget!=NULL) killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			if(subjobnolist->numlibs<1) addAPPlib(subjobnolist,"No Jobs Available");
			if(selsubjobno>=subjobnolist->numlibs) selsubjobno=0;
			if(!FINDRSCLISTAPPlib(parent,"SUBSTITUTE JOB NUMBERS",selsubjobno,subjobnolist))
			{
				updatersrc(parent,"SUBSTITUTE JOB NUMBERS");
			}
			changesubjobno(parent);
		}
		if(!initial) compute_sub_units(parent);
		if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
		subpersonnel_identification=stralloc(subperid1);
	}
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	if(subperid1!=NULL) Rfree(subperid1);
	if(subperid2!=NULL) Rfree(subperid2);
}
static void changelvecb(RDArsrc *parent)
{
	changelve(parent);
	compute_abs_units(parent);
	compute_sub_units(parent);
	updateSCRNvirtuals(parent);
}
static void changelve(RDArsrc *parent)
{
	char *perid1=NULL,*lvedescid1=NULL;
	int sellvedesc=0;

	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
		if((RDAstrcmp(lvedesclist->libs[sellvedesc],"No Leave Descriptions Available")) &&
			(RDAstrcmp(lvedesclist->libs[sellvedesc],"Not Applicable")))
		{
			lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
		}
		FINDRSCSETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedescid1);
		updatersrc(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
		FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(leave_description!=NULL) Rfree(leave_description);
		leave_description=stralloc(lvedescid1);
		getsupportingrecords(parent,TRUE);
	} else {
		FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
		FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(check_lveid(parent,TRUE)==FALSE)
		{
			if(perid1!=NULL) Rfree(perid1);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			return;
		}
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
}
static void changejobnocb(RDArsrc *parent)
{
	changejobno(parent);
	updateSCRNvirtuals(parent);
}
static void changejobno(RDArsrc *parent)
{
	char *perid1=NULL,*jobnostr=NULL;
	int jobno1=(-1),seljobno=0;

	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
		if((RDAstrcmp(jobnolist->libs[seljobno],"No Jobs Available")) &&
			(RDAstrcmp(jobnolist->libs[seljobno],"Not Applicable")))
		{
			jobnostr=stralloc(jobnolist->libs[seljobno]);
		}
		jobno1=Ratoi(jobnostr);
		if(jobnostr!=NULL) Rfree(jobnostr);
		FINDRSCSETINT(parent,"JOB NUMBER",jobno1);
		updatersrc(parent,"JOB NUMBER");
		if(job_number!=jobno1) job_number=jobno1;
		getsupportingrecords(parent,TRUE);
	} else {
		FINDRSCGETINT(parent,"JOB NUMBER",&jobno1);
		if(check_jobno(parent,TRUE)==FALSE)
		{
			if(perid1!=NULL) Rfree(perid1);
			return;
		}
	}
	if(perid1!=NULL) Rfree(perid1);
}
static void changesubjobnocb(RDArsrc *parent)
{
	changesubjobno(parent);
	updateSCRNvirtuals(parent);
}
static void changesubjobno(RDArsrc *parent)
{
	char *subperid1=NULL,*subjobnostr=NULL;
	int subjobno1=(-1),selsubjobno=0;

	if(FINDRSCGETSTRING(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBERS",&selsubjobno);
		if((RDAstrcmp(subjobnolist->libs[selsubjobno],"No Jobs Available")) &&
			(RDAstrcmp(subjobnolist->libs[selsubjobno],"Not Applicable")))
		{
			subjobnostr=stralloc(subjobnolist->libs[selsubjobno]);
		}
		subjobno1=Ratoi(subjobnostr);
		if(subjobnostr!=NULL) Rfree(subjobnostr);
		FINDRSCSETINT(parent,"SUBSTITUTE JOB NUMBER",subjobno1);
		updatersrc(parent,"SUBSTITUTE JOB NUMBER");
		getsupportingrecords(parent,TRUE);
	} else {
		FINDRSCGETINT(parent,"SUBSTITUTE JOB NUMBER",&subjobno1);
		if(check_subjobno(parent,TRUE)==FALSE)
		{
			if(subperid1!=NULL) Rfree(subperid1);
			return;
		}
	}
	if(subperid1!=NULL) Rfree(subperid1);
}
void browse_perdmg(RDArsrc *parent)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(parent);
	if(mbl_perdmg==NULL)
	{
		mainrsrc=RDArsrcNEW(module,"ADD LEAVE USED PERSONNEL BROWSE");
		searchrsrc=RDArsrcNEW(module,"ADD LEAVE USED PERSONNEL SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"ADD LEAVE USED PERSONNEL DEFINE LIST");
		addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
		addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_perdmg=ADVbrowse(dmgnum,1,mainrsrc,searchrsrc,definelist,
			NULL,selectfunction,NULL,okfunc0,NULL,
			parent,TRUE,NULL,0,1,1,TRUE);
	} else {
		if(APPmakescrn(mbl_perdmg->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_perdmg);
		}
	}
}
void browse_submstr(RDArsrc *parent)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(parent);
	if(mbl_submstr==NULL)
	{
		mainrsrc=RDArsrcNEW(module,"ADD LEAVE USED SUBSTITUTE BROWSE");
		searchrsrc=RDArsrcNEW(module,"ADD LEAVE USED SUBSTITUTE SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"ADD LEAVE USED SUBSTITUTE DEFINE LIST");
		addDFincvir(searchrsrc,"SUBMGT","SUBMASTR",NULL,submstr);
		addDFincvir(definelist,"SUBMGT","SUBMASTR",NULL,submstr);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_submstr=ADVbrowse(submstr,1,mainrsrc,searchrsrc,definelist,
			NULL,selectfunction,NULL,okfunc1,NULL,
			parent,TRUE,NULL,0,1,1,TRUE);
	} else {
		if(APPmakescrn(mbl_submstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_submstr);
		}
	}
}
static short selectfunction(MakeBrowseList *blist)
{
	return(TRUE);
}
static void doexit(MakeBrowseList *blist)
{
	if(blist->definelist!=NULL)
	{
		if(blist->definelist->swidget!=NULL) killwindow(blist->definelist);
		free_rsrc(blist->definelist);
	}
	if(blist->searchrsrc!=NULL)
	{
		if(blist->searchrsrc->swidget!=NULL) killwindow(blist->searchrsrc);
		free_rsrc(blist->searchrsrc);
	}
	if(blist->mainrsrc!=NULL)
	{
		free_rsrc(blist->mainrsrc);
	}
}
static void okfunc0(MakeBrowseList *b,void *targetkey)
{
	char *perid1=NULL;

	ZERNRD(dmgnum);
	if(EQLNRDKEYsec(dmgnum,1,targetkey))
	{
		KEYNRD(dmgnum,1);
	} else {
		FINDFLDGETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",&perid1);
	}
	FINDRSCSETSTRING(b->funcargs,"[LVETRAN][PERSONNEL IDENTIFICATION]",perid1);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	changeper(b->funcargs);
	computeallSCRNvirtuals(b->funcargs);
	updateallrsrc(b->funcargs);
}
static void okfunc1(MakeBrowseList *b,void *targetkey)
{
	char *perid1=NULL,*lvedescid1=NULL;

	ZERNRD(submstr);
	if(EQLNRDKEYsec(submstr,1,targetkey))
	{
		KEYNRD(submstr,1);
	} else {
		FINDFLDGETSTRING(submstr,"PERSONNEL IDENTIFICATION",&perid1);
	}
	FINDRSCSETSTRING(b->funcargs,"SUBSTITUTE PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(filenum,"SUBSTITUTE PERSONNEL IDENTIFICATION",perid1);
	changesub(b->funcargs,FALSE);
	computeallSCRNvirtuals(b->funcargs);
	updateallrsrc(b->funcargs);
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
}
void main(int argc,char **argv)
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	int start=0;
	double sub_units=0.0,abs_units=0.0;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL;

	if(InitializeSubsystems(argc,argv,module,"ADD LEAVE USED SUB-WORKED")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	GetAddList();
	if(argc>2)
	{
		audit_reportname=stralloc(argv[1]);
	} else {
		audit_reportname=stralloc("LUAUDIT");
	}
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if((filenum=APPOPNNRD(module,"LVETRAN",TRUE,TRUE))==(-1)) return;
	if((lmsrnum=APPOPNNRD(module,"LVEMSTR",TRUE,TRUE))==(-1)) return;
	if((eabsnum=APPOPNNRD("EMPABS","EMPABTR",TRUE,TRUE))==(-1)) return;
	if((submstr=APPOPNNRD("SUBMGT","SUBMASTR",TRUE,TRUE))==(-1)) return;
	if((subwknum=APPOPNNRD("SUBMGT","SUBWKMAS",TRUE,TRUE))==(-1)) return;
	if((subemp=APPOPNNRD("SUBMGT","SUBEMP",TRUE,TRUE))==(-1)) return;
	if((subpos=APPOPNNRD("SUBMGT","SUBPOS",TRUE,TRUE))==(-1)) return;
	if((subloc=APPOPNNRD("SUBMGT","SUBLOC",TRUE,TRUE))==(-1)) return;
	if((jmstnum=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((poslmsr=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	if((posper=APPOPNNRD("POSTRK","POSPER",TRUE,FALSE))==(-1)) return;
	if((posgrs=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((poscls=APPOPNNRD("POSTRK","POSCLS",TRUE,FALSE))==(-1)) return;
	if((posdedc=APPOPNNRD("POSTRK","POSDEDC",TRUE,FALSE))==(-1)) return;
	if((possmst=APPOPNNRD("POSTRK","POSSMST",TRUE,FALSE))==(-1)) return;
	if((posngms=APPOPNNRD("POSTRK","POSNGMS",TRUE,FALSE))==(-1)) return;
	if((subjmstnum=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	/* if((subposmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return; */
	if((ldescnum=APPOPNNRD(module,"LVEDSC",TRUE,FALSE))==(-1)) return;
	if((ltypenum=APPOPNNRD(module,"LVETPMS",TRUE,FALSE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((subdmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	nokeys=NUMKEYS(filenum);
	keys=KEYPOINTER(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(AddList)
	{
		parent=RDArsrcNEW(module,"ADD LEAVE USED SUB-WORKED");
	} else {
		parent=RDArsrcNEW(module,"ADD LEAVE USED SUB-WORKED W/O LISTS");
	}
	definelist=RDArsrcNEW(module,"ADD LEAVE USED SUB-WORKED DEFINE LIST");
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"PERSONNEL IDENTIFICATION"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(dmgnum,parent,FALSE);
					addstdrsrc(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",check_peridcb,NULL);
					singlefile2rsrc(subdmgnum,parent,FALSE);
					addstdrsrc(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					addstdrsrc(parent,"SUBSTITUTE UNITS",SDECIMALV,8,&sub_units,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"SUBSTITUTE PERSONNEL IDENTIFICATION",check_subidcb,NULL);
					file2rsrc(jmstnum,parent,FALSE);
					file2rsrc(posmstr,parent,FALSE);
					file2rsrc(poslmsr,parent,FALSE);
/*
					singlefile2rsrc(subjmstnum,parent,FALSE);
					singlefile2rsrc(subposmstr,parent,FALSE);
*/
					if(AddList==TRUE)
					{
						jobnolist=APPlibNEW();
						addlstrsrc(parent,"JOB NUMBERS",
							&start,TRUE,
							changejobnocb,jobnolist->numlibs,
							&jobnolist->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"JOB NUMBERS",FALSE);
						subjobnolist=APPlibNEW();
						if(subjobnolist->numlibs<1) addAPPlib(subjobnolist,"No Jobs Available");
						addlstrsrc(parent,"SUBSTITUTE JOB NUMBERS",
							&start,TRUE,
							changesubjobnocb,subjobnolist->numlibs,
							&subjobnolist->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"SUBSTITUTE JOB NUMBERS",FALSE);
					}
					addstdrsrc(parent,"JOB NUMBER",LONGV,6,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"JOB NUMBER",check_jobnocb,NULL);
					addstdrsrc(parent,"SUBSTITUTE JOB NUMBER",LONGV,6,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"SUBSTITUTE JOB NUMBER",check_subjobnocb,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"LEAVE DESCRIPTION IDENTIFICATION"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(ldescnum,parent,FALSE);
					file2rsrc(lmsrnum,parent,FALSE);
					if(AddList==TRUE)
					{
						lvedesclist=APPlibNEW();
						addlstrsrc(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",
							&start,TRUE,
							changelvecb,lvedesclist->numlibs,
							&lvedesclist->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"LEAVE DESCRIPTIONS",FALSE);
					}
					addstdrsrc(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",check_lveidcb,NULL);
					addstdrsrc(parent,"ABSENTEE UNITS",SDECIMALV,8,&abs_units,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",check_lveidcb,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"STARTING DATE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addstdrsrc(parent,"[LVETRAN][STARTING DATE]",DATES,8,NULL,(edit_rsrc ? TRUE:FALSE));
					addstdrsrc(parent,"STARTING TIME",TIMES,8,NULL,(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"ENDING DATE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addstdrsrc(parent,"[LVETRAN][ENDING DATE]",DATES,8,NULL,(edit_rsrc ? TRUE:FALSE));
					addstdrsrc(parent,"ENDING TIME",TIMES,8,NULL,(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"TYPE"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,filenum,fldx,edit_rsrc);
			}
		}
	}
	nofields=0;
	nokeys=0;
	if(prevlve!=NULL) freeapplib(prevlve);
	prevlve=APPlibNEW();
	addAPPlib(prevlve,"No Leaves Used added this session");
	addlstrsrc(parent,"PREVIOUS LEAVES USED",&x,TRUE,NULL,prevlve->numlibs,
		&prevlve->libs,NULL);
	addstdrsrc(parent,"ACTUALLY ABSENT",BOOLNS,1,&trueflag,TRUE);
	FINDRSCSETSTRING(parent,"[LVETRAN][ENTRY DATE]",CURRENT_DATE);
	FINDRSCSETSTRING(parent,"[LVETRAN][ENTRY TIME]",CURRENT_TIME);
	savelist(definelist,parent);
	addDFincvir(parent,module,"LVETRAN",NULL,filenum);
	addDFincvir(parent,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(parent,"PAYROLL","PAYJMST",NULL,jmstnum);
	addDFincvir(parent,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(parent,"POSTRK","POSPER",NULL,posper);
	addDFincvir(parent,"POSTRK","POSLMST",NULL,poslmsr);
	addDFincvir(parent,"POSTRK","POSSMST",NULL,possmst);
	addDFincvir(parent,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(parent,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(parent,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(parent,"POSTRK","POSNGMS",NULL,posngms);
	addDFincvir(parent,module,"LVEDSC",NULL,ldescnum);
	addDFincvir(parent,module,"LVEMSTR",NULL,lmsrnum);
	addDFincvir(parent,module,"LVETPMS",NULL,ltypenum);
	addDFincvir(definelist,module,"LVETRAN",NULL,filenum);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	addbtnrsrc(parent,"BROWSE PERSONNEL",TRUE,browse_perdmg,NULL);
	addbtnrsrc(parent,"BROWSE SUBSTITUTE PERSONNEL",TRUE,browse_submstr,NULL);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addbtnrsrc(parent,"QUIT",TRUE,quit_check,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(parent,TRUE);
	APPmakescrn(parent,FALSE,quit_record_freens,NULL,TRUE);
	RDAAPPMAINLOOP();
}
