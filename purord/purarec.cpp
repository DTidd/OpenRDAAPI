/* purarec.c - Purchase Orders Add Received Items */
#ifndef WIN32
#define __NAM__ "purarec.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purarec.exe"
#endif
#include <app.hpp>
#include <cstdlib>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB add */
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

static short filenum=(-1),purmst=(-1),purlin=(-1);
static short finyear=(-1),venmsr=(-1),fixcat=(-1),fixsts=(-1),invitm=(-1),invrec=(-1),inviloc=(-1);
static void RecSubData(char **,PassableStruct *);
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_venmsr=NULL,*mbl_purmst=NULL,*mbl_purlin=NULL;
static MakeBrowseList *mbl_items=NULL,*mbl_venitems=NULL,*mbl_finyear=NULL;
static void doexit(MakeBrowseList *);
static void doexit1(MakeBrowseList *);
static short selectfunc(MakeBrowseList *);
static RDArsrc *definelist=NULL;
static APPlib *prev_purrec=NULL;
static char *module="PURORD";
static int counter=0;
static void set_defaults(RDArsrc *,PassableStruct *);
static DBsort *dbsort=NULL;
RDAfinmgt *fmgt=NULL;
static void computelines(RDArsrc *,PassableStruct *);
static void select_backend(DBsort *,RDArsrc *);
static void savelist(RDArsrc *,PassableStruct *),savelistcb(RDArsrc *,PassableStruct *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *,PassableStruct *),quit_record(RDArsrc *,PassableStruct *);
static void quit_record_free(RDArsrc *,PassableStruct *),quit_record_freens(RDArsrc *,PassableStruct *);
static void save_check(RDArsrc *,PassableStruct *),quit_check(RDArsrc *,PassableStruct *);
static short check_poid(RDArsrc *,short,PassableStruct *);
static void check_poidcb(RDArsrc *,PassableStruct *);
static void getsupportingrecords(RDArsrc *,short,PassableStruct *);
static void changepoid(RDArsrc *,short,PassableStruct *);
static void changepoidcb(RDArsrc *,PassableStruct *);
static void browse_finyear(RDArsrc *,PassableStruct *);
static void browse_purmst(RDArsrc *,PassableStruct *);
static void browse_venmsr(RDArsrc *,PassableStruct *);
static void browse_purlin(RDArsrc *,PassableStruct *);
static void browse_items(RDArsrc *,PassableStruct *);
static void browse_venitems(RDArsrc *,PassableStruct *);
static int fiscal_year=(-1);
static char *po_identification=NULL;
static int purrec_quantity=0;
static char *item_identification=NULL,*vendor_item_number=NULL;
static short fa_source=(-1);
static double fa_cap_thresh=0.0;
static char *getnextserial(RDArsrc *parent);
/*
static int select_item_identification=(-1),select_vendor_item=(-1);
*/
static char *audit_reportname=NULL;

void RecSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y=0;
	RDAvirtual *v=NULL;
	DBsort *dbsort=NULL;
	RDArsrc *r=NULL;

	r=p->r;
	dbsort=p->a;
	if(diageval)
	{
		prterr("DIAG RecSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	fieldname=stripfieldname(*tmp);
	if(fieldname!=NULL)
	{
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
		} else {
			prterr("Error Field [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
	} else if(!RDAstrcmp(modulename,"LOGIN IDENTIFICATION"))
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
		temp=stralloc(USERLOGIN);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
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
		temp=GETCURRENTDATE();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
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
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION NAME"))
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
		temp=stralloc(XPERT_SETUP->company_name);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #1"))
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
		temp=stralloc(XPERT_SETUP->addr1);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #2"))
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
		temp=stralloc(XPERT_SETUP->addr2);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION CITY"))
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
		temp=stralloc(XPERT_SETUP->city);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION STATE"))
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
		temp=stralloc(XPERT_SETUP->state);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ZIP"))
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
		temp=stralloc(XPERT_SETUP->zip);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION PHONE"))
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
		temp=stralloc(XPERT_SETUP->phone);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else { /* virtual field */
		for(y=0,v=r->virflds;y<r->num;++y,++v)
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
						temp=EVALUATEstr(v->expression,RecSubData,p);
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
						*v->value.short_value=EVALUATEsht(v->expression,RecSubData,p);
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
						*v->value.integer_value=EVALUATEint(v->expression,RecSubData,p);
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
						*v->value.float_value=EVALUATEdbl(v->expression,RecSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,RecSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,RecSubData,p);
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
			field=FLDNRD(dbsort->fileno,modulename);
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
				if(ScrolledListSubData(tmp,r)==(-1))
				{
					prterr("Error Field [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
				}
			}
		}
	}
	Rfree(modulename);
	}
	if(diageval)
	{
		prterr("DIAG RecSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
static void savelistcb(RDArsrc *definelist,PassableStruct *p)
{
	savelist(definelist,p);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,PassableStruct *p)
{
	short ef=0;
	char *username=NULL,deleteflag=FALSE,verified=FALSE;

	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(prev_purrec!=NULL) freeapplib(prev_purrec);
	prev_purrec=APPlibNEW();
	ef=ADVFRSNRDsec(filenum,1,RecSubData,p);
	while(!ef)
	{
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(filenum,"SOURCE USER VERIFIED",&verified);
		if(!deleteflag && !verified)
		{
			FINDFLDGETSTRING(filenum,"SOURCE USER",&username);
			if(!RDAstrcmp(username,USERLOGIN))
			{
				makedflist();
			}
		}
		ef=ADVNXTNRDsec(filenum,1,RecSubData,p);
	}
        if(prev_purrec->numlibs<1)
        {
                addAPPlib(prev_purrec,"No PO Item Receives added this session");
        }
	if(!FINDRSCLISTAPPlib(p->r,"PREVIOUS PO ITEMS RECEIVED",(prev_purrec->numlibs-1),prev_purrec))
	{
		updatersrc(p->r,"PREVIOUS PO ITEMS RECEIVED");
	}
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member=NULL,*pos_member=NULL;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field=NULL;

	if(prev_purrec!=NULL)
	{
		if(prev_purrec->numlibs==1 && !RDAstrcmp(prev_purrec->libs[0],
                	"No PO Item Receives added this session"))
		{
			freeapplib(prev_purrec);
			prev_purrec=APPlibNEW();
		}
	} else {
		prev_purrec=APPlibNEW();
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
					}
					if(s!=NULL) Rfree(s);
				}
			}
		}
		++cur;
	}
	addAPPlib(prev_purrec,hold);
	++counter;
	if(hold!=NULL) Rfree(hold);
}
void definelistfunc(RDArsrc *parent)
{
	updateSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static char *getnextserial(RDArsrc *parent)
{
	char *prefix=NULL,*suffix=NULL,*ser_num=NULL,zero_fill=FALSE;
	double start_number=0;

	readallwidgets(parent);
	FINDRSCGETSTRING(parent,"FIXED ASSETS PREFIX",&prefix);
	FINDRSCGETSTRING(parent,"FIXED ASSETS SUFFIX",&suffix);
	FINDRSCGETCHAR(parent,"FIXED ASSETS ZERO FILL",&zero_fill);
	FINDRSCGETDOUBLE(parent,"FIXED ASSETS START NUMBER",&start_number);
	if(prefix==NULL)
	{
		prefix=Rmalloc(3+1);
		sprintf(prefix,"");
	}
	if(suffix==NULL)
	{
		suffix=Rmalloc(3+1);
		sprintf(suffix,"");
	}
	ser_num=Rmalloc(RDAstrlen(prefix)+RDAstrlen(suffix)+5+1);
	if(zero_fill)
	{
		sprintf(ser_num,"%s%05.0f%s",prefix,start_number,suffix);
	} else {
		sprintf(ser_num,"%s%f%s",prefix,start_number,suffix);
	}
	start_number+=1;
	FINDRSCSETDOUBLE(parent,"FIXED ASSETS START NUMBER",start_number);
	updatersrc(parent,"FIXED ASSETS START NUMBER");
	return(ser_num);
	if(ser_num!=NULL) Rfree(ser_num);
	if(prefix!=NULL) Rfree(prefix);
	if(suffix!=NULL) Rfree(suffix);
}
static void set_defaults(RDArsrc *parent,PassableStruct *p)
{
	int selprev=0;

	FINDRSCGETINT(parent,"PREVIOUS PO ITEMS RECEIVED",&selprev);
	DefaultResetScreen(parent,TRUE);
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	getsupportingrecords(parent,TRUE,p);
	changepoid(parent,TRUE,p);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void check_poidcb(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	check_poid(parent,FALSE,p);
	getsupportingrecords(parent,TRUE,p);
	changepoid(parent,FALSE,p);
	updateSCRNvirtuals(parent);
}
static void changepoidcb(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	getsupportingrecords(parent,TRUE,p);
	changepoid(parent,FALSE,p);
	updateSCRNvirtuals(parent);
}
static void changevenidcb(RDArsrc *parent,PassableStruct *p)
{
	char *venid1=NULL,*poid1=NULL,*poid2=NULL;
	char *itemid1=NULL,*itemid2=NULL;
	char *venitemno1=NULL,*venitemno2=NULL;
	char *warnmessage=NULL,deleteflag=TRUE;
	int fiscalyr1=(-1),fiscalyr2=(-1);
	short ef=FALSE,keyno=(-1);

	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	FINDRSCGETSTRING(parent,"VENDOR IDENTIFICATION",&venid1);
	ZERNRD(venmsr);
	FINDFLDSETSTRING(venmsr,"VENDOR IDENTIFICATION",venid1);
	if(!ADVEQLNRDsec(venmsr,1,RecSubData,p))
	{
		FINDFLDGETCHAR(venmsr,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			warnmessage=Rmalloc(RDAstrlen(venid1)+250);
			sprintf(warnmessage,"The Vendor Identification of [%s] is in a state of Deletion.  It does exist in the [VENPMT][VENMSR] data file but is presently marked Deleted.  Please select a valid Vendor Identification.",(venid1==NULL?"":venid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
		} else {
			FINDFLDGETCHAR(venmsr,"ACTIVE",&deleteflag);
			if(!deleteflag)
			{
				warnmessage=Rmalloc(RDAstrlen(venid1)+250);
				sprintf(warnmessage,"The Vendor Identification of [%s] is in a state of Non-Active.  It does exist in the [VENPMT][VENMSR] data file but is marked as In-Active.  Please select a valid Vendor Identification.",(venid1==NULL?"":venid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","NON-ACTIVE VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
			} else {
				FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1);
				FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1);
				FINDRSCGETSTRING(parent,"ITEM IDENTIFICATION",&itemid1);
				FINDRSCGETSTRING(parent,"VENDOR ITEM NUMBER",&venitemno1);
				ZERNRD(purlin);
				if(fiscalyr1!=0 && fiscalyr1!=(-1))
				{
					FINDFLDSETINT(purlin,"FISCAL YEAR",fiscalyr1);
					if(!isEMPTY(poid1))
					{
						FINDFLDSETSTRING(purlin,"PO IDENTIFICATION",poid1);
					}
				}
				keyno=KEYNUMBERBYNAME("PURORD","PURLIN","PURLIN");
				if(keyno==(-1)) keyno=1;
				ef=ADVGTENRDsec(purlin,keyno,RecSubData,p);
				while(!ef)
				{
					if(fiscalyr1!=0 && fiscalyr1!=(-1))
					{
						FINDFLDGETINT(purlin,"FISCAL YEAR",&fiscalyr2);
						if(fiscalyr2!=fiscalyr1) break;
					}
					if(!isEMPTY(poid1))
					{
						FINDFLDGETSTRING(purlin,"PO IDENTIFICATION",&poid2);
						if(RDAstrcmp(poid2,poid1)) break;
					}
					if(!isEMPTY(itemid1))
					{
						FINDFLDGETSTRING(purlin,"ITEM IDENTIFICATION",&itemid2);
						if(RDAstrcmp(itemid2,itemid1)) break;
					} else {
						if(!isEMPTY(venitemno1))
						{
							FINDFLDGETSTRING(purlin,"ITEM IDENTIFICATION",&venitemno2);
							if(RDAstrcmp(venitemno2,venitemno1)) break;
						}
					}
					ef=ADVNXTNRDsec(purlin,keyno,RecSubData,p);
				}
			}
		}
	} else {
		warnmessage=Rmalloc(RDAstrlen(venid1)+200);
		sprintf(warnmessage,"The Vendor Identification of [%s] is an invalid Vendor ID.  It does not exist in the [VENPMT][VENMSR] data file.  Please select a valid Vendor Identification.",(venid1==NULL?"":venid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
	}
	if(venid1!=NULL) Rfree(venid1);
	getsupportingrecords(parent,TRUE,p);
	changepoid(parent,FALSE,p);
	updateSCRNvirtuals(parent);
}
static short check_poid(RDArsrc *parent,short updatesupporting,PassableStruct *p)
{
	char *poid1=NULL,*warnmessage=NULL;
	int fiscalyr1=(-1);
	char delflag=FALSE;
	
	FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1);
	FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1);
	if(purmst!=(-1))
	{
		ZERNRD(purmst);
		FINDFLDSETINT(purmst,"FISCAL YEAR",fiscalyr1);
		FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid1);
		if(ADVEQLNRDsec(purmst,1,RecSubData,p))
		{
			KEYNRD(purmst,1);
			warnmessage=Rmalloc(RDAstrlen(poid1)+5+145);
			sprintf(warnmessage,"The PO Identification of [%s] is an invalid value for the Fiscal Year of [%5d].  Please select a valid PO Identification to save this record.",(poid1==NULL?"":poid1),fiscalyr1);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETINT(filenum,"FISCAL YEAR",fiscalyr1);
			FINDFLDSETSTRING(filenum,"PO IDENTIFICATION",poid1);
			getsupportingrecords(parent,updatesupporting,p);
			if(poid1!=NULL) Rfree(poid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(purmst,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(poid1)+150);
				sprintf(warnmessage,"The PO Identification of [%s] is a deleted value for the Fiscal Year of [%5d].  Please select a valid PO Identification to save this record.",(poid1==NULL?"":poid1),fiscalyr1);
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PO IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETINT(filenum,"FISCAL YEAR",fiscalyr1);
				FINDFLDSETSTRING(filenum,"PO IDENTIFICATION",poid1);
				getsupportingrecords(parent,updatesupporting,p);
				if(poid1!=NULL) Rfree(poid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(purmst,"CLOSED",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(poid1)+250);
					sprintf(warnmessage,"The PO Identification of [%s] for the Fiscal Year of [%5d] is Closed in the [PURORD][PURMST] data file.  Please select a valid PO Identification to save this record.",(poid1==NULL?"":poid1),fiscalyr1);
					WARNINGDIALOG("WARNING DIALOG SCREEN","CLOSED PO",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(parent,updatesupporting,p);
					if(poid1!=NULL) Rfree(poid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(poid1!=NULL) Rfree(poid1);
		return(FALSE);
	}
	FINDFLDSETINT(filenum,"FISCAL YEAR",fiscalyr1);
	FINDFLDSETSTRING(filenum,"PO IDENTIFICATION",poid1);
/*
	getsupportingrecords(parent,updatesupporting,p);
*/
	if(poid1!=NULL) Rfree(poid1);
	return(TRUE);
}
static void getsupportingrecords(RDArsrc *parent,short updatesupporting,PassableStruct *p)
{
	char *id=NULL;
	int num=(-1);

	FINDRSCGETINT(parent,"FISCAL YEAR",&num);
	if(purmst!=(-1))
	{
		FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&id);
		ZERNRD(purmst);
		FINDFLDSETINT(purmst,"FISCAL YEAR",num);
		FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",id);
		if(ADVEQLNRDsec(purmst,1,RecSubData,p)) KEYNRD(purmst,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(purmst,parent);
		} else {
			filerecord2rsrc(purmst,parent);
		}
	}
	if(finyear!=(-1))
	{
		ZERNRD(finyear);
		FINDFLDSETINT(finyear,"FISCAL YEAR",num);
		if(ADVEQLNRDsec(finyear,1,RecSubData,p)) KEYNRD(finyear,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(finyear,parent);
		} else {
			filerecord2rsrc(finyear,parent);
		}
	}
	if(venmsr!=(-1))
	{
		FINDRSCGETSTRING(parent,"VENDOR IDENTIFICATION",&id);
		ZERNRD(venmsr);
		FINDFLDSETSTRING(venmsr,"VENDOR IDENTIFICATION",id);
		if(ADVEQLNRDsec(venmsr,1,RecSubData,p)) KEYNRD(venmsr,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(venmsr,parent);
		} else {
			filerecord2rsrc(venmsr,parent);
		}
	}
	if(id!=NULL) Rfree(id);
}
static void select_backend(DBsort *dbsort,RDArsrc *parent)
{
	short ef=FALSE,update_diag=FALSE,invrec=(-1),fixmst=(-1),fixitem=(-1);
	char *poid=NULL,*locid=NULL,*catid=NULL,*itemid=NULL,deleteflag=FALSE;
	char *statid=NULL,*fund=NULL,*venid=NULL,*ser_num=NULL,cap_asset=FALSE;
	char *desc=NULL,*comments=NULL,*acq_date=NULL;
	int fiscalyr=(-1),lineno=(-1),type=0;
	int initqty=0,rec_qty=0,fa_qty=1,tot_qty=0,x=0;
	short def_type=0;
	char *curdate=NULL,*curtime=NULL;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev_file=NULL,*prev_fa_file=NULL,*prev=NULL,*invrec_file=NULL;
	double unitrate=0.0,cur_value=0.0,tot_cur_value=0.0,line_amt=0.0;
	double fixmst_cur_value=0.0,q=0,cf=0.0;
	PassableStruct *p=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Creating Items Received",NULL);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		
		if(XPERT_SETUP->INVCTL)
		{
			invitm=OPNNRDsec("INVCTL","INVITM",TRUE,TRUE,FALSE);
			inviloc=OPNNRDsec("INVCTL","INVILOC",TRUE,TRUE,FALSE);
			invrec=OPNNRDsec("INVCTL","INVREC",TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->FIXASS)
		{
			fixmst=OPNNRDsec("FIXASS","FIXMST",TRUE,TRUE,FALSE);
			fixitem=OPNNRDsec("FIXASS","FIXITEM",TRUE,TRUE,FALSE);
		}
		addDFincvir(tmprsrc,module,"PURREC",NULL,filenum);
		addDFincvir(tmprsrc,module,"PURLIN",NULL,purlin);
		addDFincvir(tmprsrc,"INVCTL","INVITM",NULL,invitm);
		addDFincvir(tmprsrc,"INVCTL","INVILOC",NULL,inviloc);
		addDFincvir(tmprsrc,"INVCTL","INVREC",NULL,invrec);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(curdate!=NULL) Rfree(curdate);
			curdate=GETCURRENTDATE10();
			if(!deleteflag)
			{
				FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",&fiscalyr);
				FINDFLDGETSTRING(dbsort->fileno,"PO IDENTIFICATION",&poid);
				FINDFLDGETINT(dbsort->fileno,"LINE NUMBER",&lineno);
				FINDFLDGETINT(dbsort->fileno,"INCOMING RECEIVED QUANTITY",&rec_qty);
				ZERNRD(filenum);
				FINDFLDSETINT(filenum,"FISCAL YEAR",fiscalyr);
				FINDFLDSETSTRING(filenum,"PO IDENTIFICATION",poid);
				FINDFLDSETINT(filenum,"LINE NUMBER",lineno);
				FINDFLDSETINT(filenum,"QUANTITY",rec_qty);
				FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
				FINDFLDSETSTRING(filenum,"ENTRY DATE",curdate);
				FINDFLDSETSTRING(filenum,"ENTRY TIME",curtime);
				if(!ADVWRTTRANSsec(filenum,1,"RECEIVE NUMBER",NULL,RecSubData,p))
				{
					update_diag=TRUE;
					ZERNRD(purlin);
					FINDFLDSETINT(purlin,"FISCAL YEAR",fiscalyr);
					FINDFLDSETSTRING(purlin,"PO IDENTIFICATION",poid);
					FINDFLDSETINT(purlin,"LINE NUMBER",lineno);
					LOCNRDFILE(purlin);
					if(!ADVEQLNRDsec(purlin,1,RecSubData,p))
					{
						prev_file=RDATDataNEW(purlin);
						FINDFLDGETINT(purlin,"RECEIVED QUANTITY",&initqty);
						initqty+=rec_qty;
						FINDFLDSETINT(purlin,"RECEIVED QUANTITY",initqty);
						if(!ADVWRTTRANSsec(purlin,0,NULL,prev_file,RecSubData,p))
						{
						}
						FINDFLDGETINT(purlin,"TYPE",&type);
						if(invrec!=(-1))
						{
							if(type==2) /* Inventory Control */
							{
								FINDFLDGETSTRING(purlin,"LOCATION IDENTIFICATION",&locid);
								FINDFLDGETSTRING(purlin,"CATEGORY IDENTIFICATION",&catid);
								FINDFLDGETSTRING(purlin,"ITEM IDENTIFICATION",&itemid);
								ZERNRD(invrec);
								FINDFLDSETSTRING(invrec,"LOCATION IDENTIFICATION",locid);
								FINDFLDSETSTRING(invrec,"CATEGORY IDENTIFICATION",catid);
								FINDFLDSETSTRING(invrec,"ITEM IDENTIFICATION",itemid);
								q=rec_qty;
								FINDFLDGETDOUBLE(purlin,"UNIT RATE",&unitrate);
								FINDFLDSETSTRING(invrec,"SOURCE USER",USERLOGIN);
								FINDFLDSETSTRING(invrec,"ENTRY DATE",curdate);
								FINDFLDSETSTRING(invrec,"ENTRY TIME",curtime);
								FINDFLDSETSTRING(invrec,"DATE RECEIVED",curdate);
								memset(stemp,0,101);
								sprintf(stemp,"%.4s-%.2s-%.2s",&curdate[7],&curdate[0],&curdate[3]);
								FINDFLDSETSTRING(invrec,"RECEIVED DATE",stemp);
								if(!ADVEQLNRDsec(invrec,1,RecSubData,p))
								{
									invrec_file=RDATDataNEW(invrec);
								} else {
									invrec_file=NULL;
									KEYNRD(invrec,1);
								}
								ZERNRD(invitm);
								FINDFLDSETSTRING(invitm,"CATEGORY IDENTIFICATION",catid);
								FINDFLDSETSTRING(invitm,"ITEM IDENTIFICATION",itemid);
								if(ADVEQLNRDsec(invitm,1,RecSubData,p)) KEYNRD(invitm,1);
								FINDFLDGETDOUBLE(invitm,"CONVERSION FACTOR",&cf);
								if(cf==0.0) cf=1.0;
								FINDFLDGETDOUBLE(invrec,"QUANTITY",&q);
								q+=rec_qty*cf;
								FINDFLDSETDOUBLE(invrec,"QUANTITY",q);
								FINDFLDSETDOUBLE(invrec,"QUANTITY TO WAREHOUSE",q);
								FINDFLDGETDOUBLE(invrec,"VALUE",&q);
								FINDFLDSETDOUBLE(invrec,"VALUE",q+rec_qty*unitrate*100);
								FINDFLDSETDOUBLE(invrec,"VALUE TO WAREHOUSE",q+rec_qty*unitrate*100);
								if(ADVWRTTRANSsec(invrec,0,NULL,NULL,RecSubData,p))
								{
									ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed on the file INVREC.",NULL,FALSE);
									prterr("Failed to write to INVREC.");
								}
								ZERNRD(inviloc);
								FINDFLDSETSTRING(inviloc,"LOCATION IDENTIFICATION",locid);
								FINDFLDSETSTRING(inviloc,"CATEGORY IDENTIFICATION",catid);
								FINDFLDSETSTRING(inviloc,"ITEM IDENTIFICATION",itemid);
								if(!ADVEQLNRDsec(inviloc,1,RecSubData,p))
								{
									prev=RDATDataNEW(inviloc);
								} else {
									prev=NULL;
									KEYNRD(inviloc,1);
								}
								FINDFLDGETDOUBLE(inviloc,"ORDERED QUANTITY",&q);
								q-=rec_qty*cf;
								FINDFLDSETDOUBLE(inviloc,"ORDERED QUANTITY",q);
								FINDFLDGETDOUBLE(inviloc,"ORDERED VALUE",&q);
								q-=(rec_qty*unitrate*100);
								FINDFLDSETDOUBLE(inviloc,"ORDERED VALUE",q);
								FINDFLDGETDOUBLE(inviloc,"QUANTITY",&q);
								q+=rec_qty*cf;
								FINDFLDSETDOUBLE(inviloc,"QUANTITY",q);
								FINDFLDGETDOUBLE(inviloc,"VALUE",&q);
								q+=(rec_qty*unitrate*100);
								FINDFLDSETDOUBLE(inviloc,"VALUE",q);
								ADVWRTTRANSsec(inviloc,0,NULL,prev,RecSubData,p);
								if(prev!=NULL) FreeRDATData(prev);
								ZERNRD(invitm);
								FINDFLDSETSTRING(invitm,"CATEGORY IDENTIFICATION",catid);
								FINDFLDSETSTRING(invitm,"ITEM IDENTIFICATION",itemid);
								if(!ADVEQLNRDsec(invitm,1,RecSubData,p))
								{
									prev=RDATDataNEW(invitm);
								} else {
									prev=NULL;
									KEYNRD(invitm,1);
								}
								FINDFLDGETDOUBLE(invitm,"ORDERED QUANTITY",&q);
								q-=rec_qty*cf;
								FINDFLDSETDOUBLE(invitm,"ORDERED QUANTITY",q);
								FINDFLDGETDOUBLE(invitm,"ORDERED VALUE",&q);
								q-=(rec_qty*unitrate*100);
								FINDFLDSETDOUBLE(invitm,"ORDERED VALUE",q);
								FINDFLDGETDOUBLE(invitm,"QUANTITY",&q);
								q+=rec_qty*cf;
								FINDFLDSETDOUBLE(invitm,"QUANTITY",q);
								FINDFLDGETDOUBLE(invitm,"VALUE",&q);
								q+=(rec_qty*unitrate*100);
								FINDFLDSETDOUBLE(invitm,"VALUE",q);
								ADVWRTTRANSsec(invitm,0,NULL,prev,RecSubData,p);
								if(prev!=NULL) FreeRDATData(prev);
							}
						}
						if((fixmst!=(-1)) && (fixitem!=(-1)))
						{
							if(type==1) /* Fixed Assets */
							{
								FINDFLDGETSTRING(purlin,"LOCATION IDENTIFICATION",&locid);
								FINDRSCGETSTRING(parent,"FIXED ASSETS CATEGORY IDENTIFICATION",&catid);
								FINDRSCGETSTRING(parent,"FIXED ASSETS STATUS IDENTIFICATION",&statid);
								FINDRSCGETSTRING(parent,"FIXED ASSETS ACQUISITION DATE",&acq_date);
								FINDRSCGETSHORT(parent,"FIXED ASSETS DEFINITION TYPE",&def_type);
								FINDRSCGETSTRING(parent,"FIXED ASSETS FUND",&fund);
								FINDFLDGETSTRING(purlin,"ITEM IDENTIFICATION",&itemid);
								FINDRSCGETSTRING(parent,"VENDOR IDENTIFICATION",&venid);
								FINDFLDGETDOUBLE(purlin,"COST",&line_amt);
								FINDFLDGETDOUBLE(purlin,"UNIT RATE",&unitrate);
								FINDFLDGETSTRING(purlin,"DESCRIPTION",&desc);
								FINDFLDGETSTRING(purlin,"COMMENTS",&comments);
							/* Writing FIXITEM */
								for(x=0;x<rec_qty;x++)
								{
/*
									ZERNRD(fixitem);
*/
									prev_fa_file=RDATDataNEW(fixitem);
									FINDFLDSETSTRING(fixitem,"LOCATION IDENTIFICATION",locid);
									FINDFLDSETSTRING(fixitem,"ITEM",itemid);
									FINDFLDSETSTRING(fixitem,"VENDOR IDENTIFICATION",venid);
									FINDFLDSETINT(fixitem,"QUANTITY",fa_qty);
									if(fa_source==0)
									{
										cur_value=unitrate*100;
									} else {
										cur_value=round(line_amt/rec_qty);
									}
									tot_cur_value+=cur_value;
									FINDFLDSETDOUBLE(fixitem,"CURRENT VALUE",cur_value);
									FINDFLDSETDOUBLE(fixitem,"PURCHASE AMOUNT",cur_value);
									FINDFLDSETDOUBLE(fixitem,"REPLACEMENT AMOUNT",cur_value);
									FINDFLDSETSTRING(fixitem,"SOURCE USER",USERLOGIN);
									FINDFLDSETSTRING(fixitem,"ENTRY DATE",curdate);
									FINDFLDSETSTRING(fixitem,"ENTRY TIME",curtime);
									if(fa_cap_thresh>cur_value)
									{
										cap_asset=FALSE;
									} else {
										cap_asset=TRUE;
									}
									FINDFLDSETCHAR(fixitem,"CAPITAL ASSET",cap_asset);
									FINDFLDSETSTRING(fixitem,"FUND",fund);
									FINDFLDSETSTRING(fixitem,"ACQUISITION DATE",acq_date);
									FINDFLDSETSTRING(fixitem,"PO IDENTIFICATION",poid);
									FINDFLDSETINT(fixitem,"PO LINE NUMBER",lineno);
									FINDFLDSETINT(fixitem,"PO FISCAL YEAR",fiscalyr);
									FINDFLDSETINT(fixitem,"DEFINITION TYPE",def_type);
									FINDFLDSETSTRING(fixitem,"STATUS",statid);
									ser_num=getnextserial(parent);
									FINDFLDSETSTRING(fixitem,"SERIAL NUMBER",ser_num);
									if(ADVWRTTRANSsec(fixitem,0,NULL,prev_fa_file,RecSubData,p))
									{
										ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed on the file FIXITEM.",NULL,FALSE);
										prterr("Failed to write to FIXITEM.");
									}
									if(ser_num!=NULL) Rfree(ser_num);
									if(prev_fa_file!=NULL) FreeRDATData(prev_fa_file);
									/* End of FIXITEM write loop */
								}
								/* Writes to FIXMST */
								ZERNRD(fixmst);
								prev_fa_file=RDATDataNEW(fixmst);
								FINDFLDSETSTRING(fixmst,"ITEM",itemid);
								if(!ADVEQLNRDsec(fixmst,1,RecSubData,p))
								{
									FINDFLDGETINT(fixmst,"TOTAL QUANTITY",&tot_qty);
									tot_qty+=rec_qty;
									FINDFLDSETINT(fixmst,"TOTAL QUANTITY",tot_qty);
									FINDFLDGETDOUBLE(fixmst,"TOTAL CURRENT VALUE",&fixmst_cur_value);
									fixmst_cur_value+=tot_cur_value;
									FINDFLDSETDOUBLE(fixmst,"TOTAL CURRENT VALUE",fixmst_cur_value);
									FINDFLDGETDOUBLE(fixmst,"TOTAL REPLACEMENT AMOUNT",&fixmst_cur_value);
									fixmst_cur_value+=tot_cur_value;
									FINDFLDSETDOUBLE(fixmst,"TOTAL REPLACEMENT AMOUNT",fixmst_cur_value);
									FINDFLDGETDOUBLE(fixmst,"TOTAL PURCHASE AMOUNT",&fixmst_cur_value);
									fixmst_cur_value+=tot_cur_value;
									FINDFLDSETDOUBLE(fixmst,"TOTAL PURCHASE AMOUNT",fixmst_cur_value);
								} else {
									KEYNRD(fixmst,1);
									FINDFLDSETSTRING(fixmst,"CATEGORY IDENTIFICATION",catid);
									FINDFLDSETSTRING(fixmst,"DESCRIPTION",desc);
									FINDFLDSETSTRING(fixmst,"COMMENTS",comments);
									FINDFLDSETINT(fixmst,"TOTAL QUANTITY",rec_qty);
									FINDFLDSETDOUBLE(fixmst,"TOTAL CURRENT VALUE",tot_cur_value);
									FINDFLDSETDOUBLE(fixmst,"TOTAL PURCHASE AMOUNT",tot_cur_value);
									FINDFLDSETDOUBLE(fixmst,"TOTAL REPLACEMENT AMOUNT",tot_cur_value);
								}
								if(ADVWRTTRANSsec(fixmst,0,NULL,prev_fa_file,RecSubData,p))
								{
									ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed on the file FIXITEM.",NULL,FALSE);
									prterr("Failed to write to FIXITEM.");
								}
							}
							if(prev_fa_file!=NULL) FreeRDATData(prev_fa_file);
							if(catid!=NULL) Rfree(catid);
							if(locid!=NULL) Rfree(locid);
							if(statid!=NULL) Rfree(statid);
							if(venid!=NULL) Rfree(venid);
							if(acq_date!=NULL) Rfree(acq_date);
							if(desc!=NULL) Rfree(desc);
							if(comments!=NULL) Rfree(comments);
							/* End of Fixed Assets */
						}
					}
					UNLNRDFILE(purlin);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				} else {
				}
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			if(update_diag)	makedflist();
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(curtime!=NULL) Rfree(curtime);
		if(curdate!=NULL) Rfree(curdate);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
		if(p!=NULL) Rfree(p);
		if(invrec!=(-1)) CLSNRD(invrec);
		if(invitm!=(-1)) CLSNRD(invitm);
		if(inviloc!=(-1)) CLSNRD(inviloc);
		if(fixmst!=(-1)) CLSNRD(fixmst);
		if(fixitem!=(-1)) CLSNRD(fixitem);
	}
}
static void save_check(RDArsrc *parent,PassableStruct *p)
{
	char *poid1=NULL,*warnmessage=NULL;
	int quantity=0;
	long size_of_file=0;

	readallwidgets(parent);
	if(check_poid(parent,FALSE,p)==FALSE) return;
	getsupportingrecords(parent,TRUE,p);
	FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1);
	FINDRSCGETINT(parent,"QUANTITY",&quantity);
	if(quantity==0)
	{
		ERRORDIALOG("INVALID QUANTITY!","This PO Items Received Transaction can not be saved with an QUANTITY of [$  0.00].",NULL,FALSE);
		if(poid1!=NULL) Rfree(poid1);
		return;
	}
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file<=0)
	{
		warnmessage=Rmalloc(RDAstrlen(poid1)+250);
		sprintf(warnmessage,"No PO Lines can be updated for the PO Identification of [%s] and the record cannot be saved.  Please select a valid PO Identification and Fiscal Year with valid Lines and Quantities to save this record.",(poid1==NULL?"":poid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO PO LINES!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(poid1!=NULL) Rfree(poid1);
		return;
	}
	if(poid1!=NULL) Rfree(poid1);
	save_record(parent,p);
}
static void save_record(RDArsrc *parent,PassableStruct *p)
{
	int x=0,lf=0;
	char *rcddesc=NULL,deleteflag=FALSE;
	char *poid1=NULL;
	int fiscalyr1=(-1);
	char *date=NULL,*timex=NULL;
	RDArmem *member=NULL;

	if(FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1)) return;
	if(FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1)) return;
	rsrc2filerecord(filenum,parent);
	FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(filenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(filenum,"SOURCE USER VERIFIED",deleteflag);
	if((x=FINDRSC(parent,"ENTRY DATE"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
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
	if((x=FINDRSC(parent,"ENTRY TIME"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
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
	if(ADVRECORDsec(filenum,RecSubData,p))
	{
		rcddesc=ADVRECORDsecDesc(filenum,RecSubData,p);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  ADVRECORDsec() failed without a description returned!");
		}
	} else {
		select_backend(dbsort,parent);
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS PO ITEMS RECEIVED",(prev_purrec->numlibs-1),
		prev_purrec))
	{
		updatersrc(parent,"PREVIOUS PO ITEMS RECEIVED");
	}
	if(poid1!=NULL) Rfree(poid1);
	updateSCRNvirtuals(parent);
}
static void quit_check(RDArsrc *parent,PassableStruct *p)
{
	quit_record(parent,p);
}
static void quit_record_freens(RDArsrc *parent,PassableStruct *p)
{
        counter=0;
        quit_record_free(parent,p);
}
static void quit_record_free(RDArsrc *parent,PassableStruct *p)
{
	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist);
	}
	if(mbl_finyear!=NULL) FREE_BROWSE_EXIT(mbl_finyear);
	if(mbl_venmsr!=NULL) FREE_BROWSE_EXIT(mbl_venmsr);
	if(mbl_purmst!=NULL) FREE_BROWSE_EXIT(mbl_purmst);
	if(mbl_items!=NULL) FREE_BROWSE_EXIT(mbl_items);
	if(mbl_venitems!=NULL) FREE_BROWSE_EXIT(mbl_venitems);
	if(mbl_purlin!=NULL) mbl_purlin=NULL;
	if(filenum!=(-1)) CLSNRD(filenum);
	if(purlin!=(-1)) CLSNRD(purlin);
	if(purmst!=(-1)) CLSNRD(purmst);
	if(finyear!=(-1)) CLSNRD(finyear);
	if(venmsr!=(-1)) CLSNRD(venmsr);
	if(fixcat!=(-1)) CLSNRD(fixcat);
	if(fixsts!=(-1)) CLSNRD(fixsts);
	if(invrec!=(-1)) CLSNRD(invrec);
	if(invitm!=(-1)) CLSNRD(invitm);
	if(inviloc!=(-1)) CLSNRD(inviloc);
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(dbsort!=NULL)
	{
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
	if(p!=NULL) Rfree(p);
	if(prev_purrec!=NULL) freeapplib(prev_purrec);
	if(po_identification!=NULL) Rfree(po_identification);
	if(item_identification!=NULL) Rfree(item_identification);
	if(vendor_item_number!=NULL) Rfree(vendor_item_number);
	if(counter>0) RUNREPORT(module,audit_reportname,NULL,NULL,FALSE,TRUE,NULL);
	if(audit_reportname!=NULL) Rfree(audit_reportname);
}
static void quit_record(RDArsrc *parent,PassableStruct *p)
{
	quit_record_free(parent,p);
	ShutdownSubsystems();
	exit(0);
}
static void makeDBsortfile(DBsort *dbsort)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	NRDfield *field=NULL,*FIELDS=NULL;
	int x=0,numfields=0;
	short keyno=(-1);

	addDBfield(dbsort,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsort,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"ADD PO LINES RECEIVED SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL)
		{
			csortsequence->expression=stralloc(d->expression);
		} else csortsequence->expression=NULL;
		csortsequence->computed=FALSE;
		csortsequence->range=FALSE;
		csortsequence->dlen=0;
		switch(csortsequence->type)
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
				csortsequence->value.string_value=NULL;
				csortsequence->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				csortsequence->value.string_value=Rmalloc(1);
				*csortsequence->value.string_value=0;
				csortsequence->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				csortsequence->value.float_value=Rmalloc(sizeof(double));
				*csortsequence->value.float_value=0;
				csortsequence->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				csortsequence->value.short_value=Rmalloc(sizeof(short));
				*csortsequence->value.short_value=0;
				csortsequence->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				csortsequence->value.integer_value=Rmalloc(sizeof(int));
				*csortsequence->value.integer_value=0;
				csortsequence->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
		addDBfield(dbsort,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsort,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	numfields=NUMFLDS(purlin);
	FIELDS=FLDPOINTER(purlin);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!RDAstrcmp(field->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(field->name,"ENTRY DATE"))
			{
			} else if(!RDAstrcmp(field->name,"ENTRY TIME"))
			{
			} else if(!RDAstrcmp(field->name,"TOTAL"))
			{
			} else if(!RDAstrcmp(field->name,"UNIT RATE"))
			{
			} else if(!RDAstrcmp(field->name,"SOURCE USER"))
			{
			} else {
				addDBfield(dbsort,field->name,field->type,field->len);
			}
		}
		addDBfield(dbsort,"INCOMING RECEIVED QUANTITY",LONGV,6);
	}
	key=KEYNUM(purlin,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsort,keyno,part->name);
		}
	}
}
void setpurlinrecord(int fiscalyr,char *poid,int lineno,
	int line_qty,int prev_rec_qty,int rec_qty)
{
/*
	ZERNRD(purlin);
*/
	FINDFLDSETINT(purlin,"FISCAL YEAR",fiscalyr);
	FINDFLDSETSTRING(purlin,"PO IDENTIFICATION",poid);
	FINDFLDSETINT(purlin,"LINE NUMBER",lineno);
	FINDFLDSETINT(purlin,"QUANTITY",line_qty);
	FINDFLDSETINT(purlin,"RECEIVED QUANTITY",(prev_rec_qty+rec_qty));
}
void addsortrecord(DBsort *dbsort,RDArsrc *parent,int fiscalyr,char *poid,
	int lineno,int line_qty,int prev_rec_qty,int rec_qty,
	PassableStruct *p)
{
	char deleteflag=FALSE;
	short nofields=0,x=0;
	NRDfield *fields=NULL,*fldx=NULL;

	ZERNRD(dbsort->fileno);
	FINDFLDSETCHAR(dbsort->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,RecSubData,p);
		switch(csortsequence->type)
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
			FINDFLDSETSTRING(dbsort->fileno,csortsequence->name,
					csortsequence->value.string_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				FINDFLDSETCHAR(dbsort->fileno,csortsequence->name,
					*csortsequence->value.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				FINDFLDSETDOUBLE(dbsort->fileno,csortsequence->name,
					*csortsequence->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				FINDFLDSETSHORT(dbsort->fileno,csortsequence->name,
					*csortsequence->value.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				FINDFLDSETINT(dbsort->fileno,csortsequence->name,
					*csortsequence->value.integer_value);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
	}
	FINDFLDSETINT(purlin,"FISCAL YEAR",fiscalyr);
	FINDFLDSETSTRING(purlin,"PO IDENTIFICATION",poid);
	FINDFLDSETINT(purlin,"LINE NUMBER",lineno);
	if(ADVEQLNRDsec(purlin,1,RecSubData,p)) KEYNRD(purlin,1);
	nofields=NUMFLDS(purlin);
	fields=FLDPOINTER(purlin);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			COPYFIELD(purlin,dbsort->fileno,fldx->name);
		}
	}
/*
	FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",fiscalyr);
	FINDFLDSETSTRING(dbsort->fileno,"PO IDENTIFICATION",poid);
	FINDFLDSETINT(dbsort->fileno,"LINE NUMBER",lineno);
*/
	FINDFLDSETINT(dbsort->fileno,"QUANTITY",line_qty);
	FINDFLDSETINT(dbsort->fileno,"RECEIVED QUANTITY",prev_rec_qty);
	FINDFLDSETINT(dbsort->fileno,"INCOMING RECEIVED QUANTITY",rec_qty);
	WRTNRD(dbsort->fileno,0,NULL);
}
static void computelines(RDArsrc *parent,PassableStruct *q)
{
	RDArsrc *tmprsrc=NULL;
	short ef=FALSE,dodiag=FALSE;
	char deleteflag=FALSE,active=TRUE,*sortname=NULL;
/*
	char selitemid=FALSE,selvenitemno=FALSE;
*/
	char *poid1=NULL,*poid2=NULL,*venid1=NULL;
	char *venitemno1=NULL,*venitemno2=NULL,*itemid1=NULL,*itemid2=NULL;
	int fiscalyr1=(-1),fiscalyr2=(-1),lineno1=(-1);
	int qty=0,prev_rec_qty=0,rec_qty=0,max_rec_qty=0;
	PassableStruct *p=NULL;

	FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1);
	FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1);
	FINDRSCGETINT(parent,"QUANTITY",&rec_qty);
	FINDRSCGETSTRING(parent,"VENDOR IDENTIFICATION",&venid1);
	FINDRSCGETSTRING(parent,"VENDOR ITEM NUMBER",&venitemno1);
	FINDRSCGETSTRING(parent,"ITEM IDENTIFICATION",&itemid1);
	if(dbsort!=NULL)
	{
		ef=GTENRD(dbsort->fileno,1);
		while(!ef)
		{
			DELNRD(dbsort->fileno);
			ef=NXTNRD(dbsort->fileno,1);
		}
	} else {
		sortname=unique_name();
		dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
		if(sortname!=NULL) Rfree(sortname);
        	makeDBsortfile(dbsort);
        	dbsort->fileno=OPNDBsort(dbsort);
		q->a=dbsort;
	}
	tmprsrc=diagscrn(purlin,"DIAGNOSTIC SCREEN",module,
		"Computing PO Items Received Transactions",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
		ForceWindowUpdate(tmprsrc);
	} else {
		tmprsrc=RDArsrcNEW(module,"TEMP");
	}
	addDFincvir(tmprsrc,module,"PURREC",NULL,filenum);
	addDFincvir(tmprsrc,module,"PURMST",NULL,purmst);
	addDFincvir(tmprsrc,module,"PURLIN",NULL,purlin);
	addDFincvir(tmprsrc,"FINMGT","FINYEAR",NULL,finyear);
	addDFincvir(tmprsrc,"VENPMT","VENMSR",NULL,venmsr);
	GET_SCREEN_VIRTUALS(tmprsrc,0);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=tmprsrc;
	p->a=dbsort;
	ZERNRD(purlin);
	FINDFLDSETINT(purlin,"FISCAL YEAR",fiscalyr1);
	FINDFLDSETSTRING(purlin,"PO IDENTIFICATION",poid1);
	ef=ADVGTENRDsec(purlin,1,RecSubData,p);
	while(!ef)
	{
		FINDFLDGETINT(purlin,"FISCAL YEAR",&fiscalyr2);
		FINDFLDGETSTRING(purlin,"PO IDENTIFICATION",&poid2);
		if(RDAstrcmp(poid1,poid2) || fiscalyr1!=fiscalyr2)
		{
			break;
		}
		dodiag=FALSE;
		FINDFLDGETCHAR(purlin,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			if(rec_qty==0)
			{
				break;
			}
			FINDFLDGETINT(purlin,"QUANTITY",&qty);
			FINDFLDGETINT(purlin,"RECEIVED QUANTITY",&prev_rec_qty);
			FINDFLDGETINT(purlin,"LINE NUMBER",&lineno1);
			if((qty-prev_rec_qty)>0)
			{
				if(qty-prev_rec_qty-rec_qty<0)
				{
					max_rec_qty=(qty-prev_rec_qty);
				} else {
					max_rec_qty=rec_qty;
				}
				ZERNRD(purmst);
				FINDFLDSETINT(purmst,"FISCAL YEAR",fiscalyr1);
				FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid1);
				if(!ADVEQLNRDsec(purmst,1,RecSubData,p))
				{
				FINDFLDGETCHAR(purmst,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				ZERNRD(venmsr);
				COPYFIELD(purmst,venmsr,"VENDOR IDENTIFICATION");
				if(!ADVEQLNRDsec(venmsr,1,RecSubData,p))
				{
				FINDFLDGETCHAR(venmsr,"DELETEFLAG",&deleteflag);
				FINDFLDGETCHAR(venmsr,"ACTIVE",&active);
				if(!deleteflag && active)
				{
				ZERNRD(finyear);
				FINDFLDSETINT(finyear,"FISCAL YEAR",fiscalyr1);
				if(!ADVEQLNRDsec(finyear,1,RecSubData,p))
				{
				FINDFLDGETCHAR(finyear,"DELETEFLAG",&deleteflag);
				FINDFLDGETCHAR(finyear,"ACTIVE",&active);
				if(!deleteflag && active)
				{
/*
					FINDRSCGETCHAR(parent,"SELECT ITEM",&selitemid);
					if(selitemid)
					{
*/
						FINDRSCGETSTRING(parent,"ITEM IDENTIFICATION",&itemid1);
						FINDFLDGETSTRING(purlin,"ITEM IDENTIFICATION",&itemid2);
						if(RDAstrcmp(itemid2,itemid1))
						{
							if(diagapps)
							{
								prterr("Skipped by Item Identifcation of [%s] in Purchase Order Line Master not matching preset value of [%s].",(itemid2=NULL?"":itemid2),(itemid1==NULL?"":itemid1));KEYNRD(purlin,1);SEENRDRECORD(purlin);
							}
							ef=ADVNXTNRDsec(purlin,1,RecSubData,p);
							continue;
						}
/*
					}
					FINDRSCGETCHAR(parent,"SELECT VENDOR ITEM",&selvenitemno);
					if(selvenitemno)
					{
*/
						FINDRSCGETSTRING(parent,"VENDOR ITEM NUMBER",&venitemno1);
						FINDFLDGETSTRING(purlin,"VENDOR ITEM NUMBER",&venitemno2);
						if(RDAstrcmp(venitemno2,venitemno1))
						{
							if(diagapps)
							{
								prterr("Skipped by Vendor Item Number of [%s] in Purchase Order Line Master not matching preset value of [%s].",(venitemno2=NULL?"":venitemno2),(venitemno1==NULL?"":venitemno1));KEYNRD(purlin,1);SEENRDRECORD(purlin);
							}
							ef=ADVNXTNRDsec(purlin,1,RecSubData,p);
							continue;
						}
/*
					}
*/
					setpurlinrecord(fiscalyr1,poid1,lineno1,qty,prev_rec_qty,max_rec_qty);
					addsortrecord(dbsort,tmprsrc,fiscalyr1,poid1,lineno1,qty,prev_rec_qty,max_rec_qty,p);
					rec_qty-=max_rec_qty;
					dodiag=TRUE;
				} else {
					if(diagapps)
					{
						prterr("Skipped by Deleted or Inactive Fiscal Year Master Record.");KEYNRD(finyear,1);SEENRDRECORD(finyear);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped by not finding Fiscal YearMaster reocord.");KEYNRD(finyear,1);SEENRDRECORD(finyear);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped by Deleted or Inactive Vendor Identification Master Record.");KEYNRD(venmsr,1);SEENRDRECORD(venmsr);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped by not finding Vendor Identification Master reocord.");KEYNRD(venmsr,1);SEENRDRECORD(venmsr);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped by Deleted Purchase Order Identification Master Record.");KEYNRD(purmst,1);SEENRDRECORD(purmst);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped by not finding Purchase Order Identification Master reocord.");KEYNRD(purmst,1);SEENRDRECORD(purmst);
					}
				}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Received all Quantities of Purchase Order Line Record.");KEYNRD(purlin,1);SEENRDRECORD(purlin);
				}
			}
		} else {
			if(diagapps)
			{
				prterr("Skipped by Deleted Purchase Order Line Master Record.");KEYNRD(purlin,1);SEENRDRECORD(purlin);
			}
		}
		if(tmprsrc!=NULL)
		{
			update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		}
		ef=ADVNXTNRDsec(purlin,1,RecSubData,p);
	}
	if(p!=NULL) Rfree(p);
	if(poid1!=NULL) Rfree(poid1);
	if(poid2!=NULL) Rfree(poid2);
	if(itemid1!=NULL) Rfree(itemid1);
	if(itemid2!=NULL) Rfree(itemid2);
	if(venitemno1!=NULL) Rfree(venitemno1);
	if(venitemno2!=NULL) Rfree(venitemno2);
/*
	updatebrowse(TRUE,mbl_empabtr,dbsort->fileno,module,parent);
*/
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
}
static void changepoid(RDArsrc *parent,short override,PassableStruct *p)
{
	char *poid1=NULL/*,selitemid=FALSE,selvenitemno=FALSE*/;
	char *itemid1=NULL,*venitemno1=NULL;
	int fiscalyr1=(-1),quantity1=0;

	if(FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1)) return;
	if(FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1)) return;
	if(FINDRSCGETINT(parent,"QUANTITY",&quantity1)) return;
/*
	if(FINDRSCGETCHAR(parent,"SELECT ITEM",&selitemid)) return;
	if(FINDRSCGETCHAR(parent,"SELECT VENDOR ITEM",&selvenitemno)) return;
*/
	if(FINDRSCGETSTRING(parent,"ITEM IDENTIFICATION",&itemid1)) return;
	if(FINDRSCGETSTRING(parent,"VENDOR ITEM NUMBER",&venitemno1)) return;
	if(RDAstrcmp(poid1,po_identification) || fiscalyr1!=fiscal_year || 
		quantity1!=purrec_quantity || 
/*
		selitemid!=select_item_identification ||
		selvenitemno!=select_vendor_item || 
*/
		RDAstrcmp(itemid1,item_identification) || 
		RDAstrcmp(venitemno1,vendor_item_number) || override)
	{
		if(mbl_finyear!=NULL) FREE_BROWSE_EXIT(mbl_finyear);
		if(mbl_venmsr!=NULL) FREE_BROWSE_EXIT(mbl_venmsr);
		if(mbl_purmst!=NULL) FREE_BROWSE_EXIT(mbl_purmst);
		if(mbl_items!=NULL) FREE_BROWSE_EXIT(mbl_items);
		if(mbl_venitems!=NULL) FREE_BROWSE_EXIT(mbl_venitems);
		computelines(parent,p);
		if(po_identification!=NULL) Rfree(po_identification);
		po_identification=stralloc(poid1);
		fiscal_year=fiscalyr1;
		purrec_quantity=quantity1;
/*
		select_item_identification=selitemid;
		select_vendor_item=selvenitemno;
*/
		if(item_identification!=NULL) Rfree(item_identification);
		item_identification=stralloc(itemid1);
		if(vendor_item_number!=NULL) Rfree(vendor_item_number);
		vendor_item_number=stralloc(venitemno1);
		getsupportingrecords(parent,TRUE,p);
	}
	if(poid1!=NULL) Rfree(poid1);
}
static short selectfunc(MakeBrowseList *b)
{
	return(TRUE);
}
static void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL)
	{
		if(blist->definelist!=NULL)
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL)
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
	}
}
static void doexit1(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

	if(blist!=NULL)
	{
		brsrc=blist->mainrsrc;
		drsrc=blist->definelist;
		srsrc=blist->searchrsrc;
		if(blist!=NULL) FREE_BROWSE_EXIT(blist);
		if(drsrc!=NULL)
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
		}
		if(srsrc!=NULL)
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			free_rsrc(brsrc);
		}
	}
}
static void browse_purlin(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

        readallwidgets(parent);
        rsrc2filerecord(filenum,parent);
        getsupportingrecords(parent,TRUE,p);
	brsrc=RDArsrcNEW(module,"ADD PO RECEIVED LINES BROWSE");
	drsrc=RDArsrcNEW(module,"ADD PO RECEIVED LINES DEFINE LIST");
	srsrc=RDArsrcNEW(module,"ADD PO RECEIVED LINES SEARCH BROWSE");
	addDFincvir(drsrc,module,"PURMST",NULL,purmst);
	addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmsr);
	addDFincvir(srsrc,module,"PURMST",NULL,purmst);
	addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmsr);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	mbl_purlin=ADVbrowse(dbsort->fileno,1,brsrc,srsrc,drsrc,NULL,
		selectfunc,doexit1,NULL,NULL,NULL,TRUE,NULL,
		0,0,0,TRUE);
	FINDRSCSETEDITABLE(brsrc,"SEARCH BUTTON",FALSE);
}
/*
*/
static void initfunc_purmst(MakeBrowseList *b)
{
	char *poid1=NULL;
	int fiscalyr1=(-1);
	RDArsrc *parent=NULL;
	PassableStruct *p=NULL;

	if(b!=NULL)
	{
		p=(PassableStruct *)b->funcargs;
		parent=p->r;
		FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1);
		FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1);
		if(fiscalyr1==(-1) || fiscalyr1==0)
		{
			return;
		} else {
			FINDFLDSETINT(b->fileno,"FISCAL YEAR",fiscalyr1);
			if(isEMPTY(poid1))
			{
				if(poid1!=NULL) Rfree(poid1);
				return;
			} else {
				FINDFLDSETSTRING(b->fileno,"PO IDENTIFICATION",poid1);
				if(poid1!=NULL) Rfree(poid1);
				return;
			}
		}
	}
}
static short breakfunc_purmst(MakeBrowseList *b)
{
	char *venid1=NULL,*venid2=NULL;
	RDArsrc *parent=NULL;
	PassableStruct *p=NULL;

	if(b!=NULL)
	{
		p=(PassableStruct *)b->funcargs;
		parent=p->r;
		FINDRSCGETSTRING(parent,"VENDOR IDENTIFICATION",&venid1);
		if(!isEMPTY(venid1))
		{
			FINDFLDGETSTRING(b->fileno,"VENDOR IDENTIFICATION",&venid2);
			if(RDAstrcmp(venid2,venid1))
			{
				if(venid1!=NULL) Rfree(venid1);
				if(venid2!=NULL) Rfree(venid2);
				return(FALSE);
			} else {
				if(venid1!=NULL) Rfree(venid1);
				if(venid2!=NULL) Rfree(venid2);
				return(TRUE);
			}
		} else {
			if(venid1!=NULL) Rfree(venid1);
			return(TRUE);
		}
	} else {
		return(FALSE);
	}
}
static short selectfunc_purmst(MakeBrowseList *b)
{
	char delflag=FALSE;

	if(b!=NULL)
	{
		FINDFLDGETCHAR(b->fileno,"DELETFLAG",&delflag);
		if(delflag)
		{
			return(FALSE);
		} else {
			return(TRUE);
		}
	} else {
		return(FALSE);
	}
}
static void okfunc_purmst(MakeBrowseList *b,void *targetkey)
{
	char *poid1=NULL,*venid1=NULL;
	int fiscalyr1=(-1);
	PassableStruct *p=NULL;

	if(b!=NULL)
	{
		p=(PassableStruct *)b->funcargs;
		FINDRSCGETINT(p->r,"FISCAL YEAR",&fiscalyr1);
		FINDRSCGETSTRING(p->r,"PO IDENTIFICATION",&poid1);
		FINDRSCGETSTRING(p->r,"VENDOR IDENTIFICATION",&venid1);
		ZERNRD(purmst);
		if(ADVEQLNRDKEYsec(purmst,1,targetkey,RecSubData,p))
		{
			KEYNRD(purmst,1);
		} else {
			FINDFLDGETINT(purmst,"FISCAL YEAR",&fiscalyr1);
			FINDFLDGETSTRING(purmst,"PO IDENTIFICATION",&poid1);
			FINDFLDGETSTRING(purmst,"VENDOR IDENTIFICATION",&venid1);
		}
		FINDRSCSETINT(p->r,"FISCAL YEAR",fiscalyr1);
		FINDRSCSETSTRING(p->r,"PO IDENTIFICATION",poid1);
		FINDRSCSETSTRING(p->r,"VENDOR IDENTIFICATION",venid1);
		rsrc2singlefilerecord(filenum,p->r);
		if(poid1!=NULL) Rfree(poid1);
		if(venid1!=NULL) Rfree(venid1);
		getsupportingrecords(p->r,FALSE,p);
		changepoid(p->r,FALSE,p);
		computeallSCRNvirtuals(p->r);
		updateallrsrc(p->r);
	}
}
void browse_finyear(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	char *poid=NULL,*venid=NULL;

	readallwidgets(parent);
	FINDRSCSETINT(parent,"FISCAL YEAR",(-1));
	if(mbl_finyear==NULL)
	{
		brsrc=RDArsrcNEW(module,"ADD PO RECEIVED YEAR BROWSE");
		srsrc=RDArsrcNEW(module,"ADD PO RECEIVED YEAR SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,"ADD PO RECEIVED YEAR DEFINE LIST");
		addDFincvir(brsrc,module,"PURMST",NULL,purmst);
		addDFincvir(brsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(brsrc,"VENPMT","VENMSR",NULL,venmsr);
		addDFincvir(drsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmsr);
		addDFincvir(srsrc,module,"PURMST",NULL,purmst);
		addDFincvir(srsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmsr);
		GET_SCREEN_VIRTUALS(brsrc,0);
		GET_SCREEN_VIRTUALS(drsrc,1);
		GET_SCREEN_VIRTUALS(srsrc,2);
		if(purmst!=(-1))
		{
			FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid);
			FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
			if(ADVEQLNRDsec(purmst,1,RecSubData,p))
			{
				ZERNRD(purmst);
			} else {
				file2rsrc(purmst,brsrc,FALSE);
				filerecord2rsrc(purmst,brsrc);
			}
			if(poid!=NULL) Rfree(poid);
		}
		if(venmsr!=(-1))
		{
			FINDRSCGETSTRING(parent,"VENDOR IDENTIFICATION",&venid);
			FINDFLDSETSTRING(venmsr,"VENDOR IDENTIFICATION",venid);
			if(ADVEQLNRDsec(venmsr,1,RecSubData,p))
			{
				ZERNRD(venmsr);
			} else {
				file2rsrc(venmsr,brsrc,FALSE);
				filerecord2rsrc(venmsr,brsrc);
			}
			if(venid!=NULL) Rfree(venid);
		}
		mbl_finyear=ADVSbrowse(purmst,1,brsrc,srsrc,drsrc,
			initfunc_purmst,breakfunc_purmst,selectfunc_purmst,
			NULL,okfunc_purmst,NULL,p,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_finyear->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_finyear);
		}
	}
}
void browse_purmst(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	int fiscalyr=(-1);
	char *venid=NULL;

	readallwidgets(parent);
	FINDRSCSETSTRING(parent,"PO IDENTIFICATION",NULL);
	if(mbl_purmst==NULL)
	{
		brsrc=RDArsrcNEW(module,"ADD PO RECEIVED BROWSE");
		srsrc=RDArsrcNEW(module,"ADD PO RECEIVED SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,"ADD PO RECEIVED DEFINE LIST");
		addDFincvir(brsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(brsrc,"VENPMT","VENMSR",NULL,venmsr);
		addDFincvir(drsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmsr);
		addDFincvir(srsrc,module,"PURMST",NULL,purmst);
		addDFincvir(srsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmsr);
		GET_SCREEN_VIRTUALS(brsrc,0);
		GET_SCREEN_VIRTUALS(drsrc,1);
		GET_SCREEN_VIRTUALS(srsrc,2);
		if(finyear!=(-1))
		{
			FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr);
			FINDFLDSETINT(finyear,"FISCAL YEAR",fiscalyr);
			if(ADVEQLNRDsec(finyear,1,RecSubData,p))
			{
				ZERNRD(finyear);
			} else {
				file2rsrc(finyear,brsrc,FALSE);
				filerecord2rsrc(finyear,brsrc);
			}
		}
		if(venmsr!=(-1))
		{
			FINDRSCGETSTRING(parent,"VENDOR IDENTIFICATION",&venid);
			FINDFLDSETSTRING(venmsr,"VENDOR IDENTIFICATION",venid);
			if(ADVEQLNRDsec(venmsr,1,RecSubData,p))
			{
				ZERNRD(venmsr);
			} else {
				file2rsrc(venmsr,brsrc,FALSE);
				filerecord2rsrc(venmsr,brsrc);
			}
			if(venid!=NULL) Rfree(venid);
		}
		mbl_purmst=ADVSbrowse(purmst,1,brsrc,srsrc,drsrc,
			initfunc_purmst,breakfunc_purmst,selectfunc_purmst,
			NULL,okfunc_purmst,NULL,p,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_purmst->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_purmst);
		}
	}
}
void browse_venmsr(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	int fiscalyr=(-1);
	char *poid=NULL;

	readallwidgets(parent);
	FINDRSCSETSTRING(parent,"VENDOR IDENTIFICATION",NULL);
	if(mbl_venmsr==NULL)
	{
		brsrc=RDArsrcNEW(module,"ADD PO RECEIVED VENDOR BROWSE");
		srsrc=RDArsrcNEW(module,"ADD PO RECEIVED VENDOR SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,"ADD PO RECEIVED VENDOR DEFINE LIST");
		addDFincvir(brsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(brsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmsr);
		addDFincvir(srsrc,module,"PURMST",NULL,purmst);
		addDFincvir(srsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmsr);
		GET_SCREEN_VIRTUALS(brsrc,0);
		GET_SCREEN_VIRTUALS(drsrc,1);
		GET_SCREEN_VIRTUALS(srsrc,2);
		if(finyear!=(-1))
		{
			FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr);
			FINDFLDSETINT(finyear,"FISCAL YEAR",fiscalyr);
			if(ADVEQLNRDsec(finyear,1,RecSubData,p))
			{
				ZERNRD(finyear);
			} else {
				file2rsrc(finyear,brsrc,FALSE);
				filerecord2rsrc(finyear,brsrc);
			}
		}
		if(purmst!=(-1))
		{
			FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid);
			FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
			if(ADVEQLNRDsec(purmst,1,RecSubData,p))
			{
				ZERNRD(purmst);
			} else {
				file2rsrc(purmst,brsrc,FALSE);
				filerecord2rsrc(purmst,brsrc);
			}
			if(poid!=NULL) Rfree(poid);
		}
		mbl_venmsr=ADVSbrowse(purmst,1,brsrc,srsrc,drsrc,
			initfunc_purmst,breakfunc_purmst,selectfunc_purmst,
			NULL,okfunc_purmst,NULL,p,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_venmsr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_venmsr);
		}
	}
}
static void initfunc_items(MakeBrowseList *b)
{
	char *poid1=NULL;
	int fiscalyr1=(-1);
	RDArsrc *parent=NULL;
	PassableStruct *p=NULL;

	if(b!=NULL)
	{
		p=(PassableStruct *)b->funcargs;
		parent=p->r;
		readallwidgets(parent);
		FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1);
		if(fiscalyr1==(-1) || fiscalyr1==0)
		{
			return;
		} else {
			FINDFLDSETINT(b->fileno,"FISCAL YEAR",fiscalyr1);
			FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1);
			if(isEMPTY(poid1))
			{
				if(poid1!=NULL) Rfree(poid1);
				return;
			} else {
				FINDFLDSETSTRING(b->fileno,"PO IDENTIFICATION",poid1);
				if(poid1!=NULL) Rfree(poid1);
				return;
			}
		}
	}
}
static short breakfunc_items(MakeBrowseList *b)
{
	int fiscalyr1=(-1),fiscalyr2=(-1);
	char *poid1=NULL,*poid2=NULL;
	RDArsrc *parent=NULL;
	PassableStruct *p=NULL;

	if(b!=NULL)
	{
		p=(PassableStruct *)b->funcargs;
		parent=p->r;
		FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr1);
		if(fiscalyr1!=(-1) && fiscalyr1!=0)
		{
			FINDFLDGETINT(b->fileno,"FISCAL YEAR",&fiscalyr2);
			if(fiscalyr2!=fiscalyr1)
			{
				return(FALSE);
			} else {
				FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid1);
				if(!isEMPTY(poid1))
				{
					FINDFLDGETSTRING(b->fileno,"PO IDENTIFICATION",&poid2);
					if(RDAstrcmp(poid2,poid1)) 
					{
						if(poid1!=NULL) Rfree(poid1);
						if(poid2!=NULL) Rfree(poid2);
						return(FALSE);
					} else {
						return(TRUE);
					}
				} else {
					return(TRUE);
				}
			}
		} else {
			return(TRUE);
		}
	} else {
		return(FALSE);
	}
}
static short selectfunc_items(MakeBrowseList *b)
{
	char delflag=FALSE;

	if(b!=NULL)
	{
		FINDFLDGETCHAR(b->fileno,"DELETFLAG",&delflag);
		if(delflag)
		{
			return(FALSE);
		} else {
			return(TRUE);
		}
	} else {
		return(FALSE);
	}
}
static void okfunc_items(MakeBrowseList *b,void *targetkey)
{
	int fiscalyr1=(-1),linenum=0;
	char *poid1=NULL,*venid1=NULL,*itemid1=NULL,*venitemno1=NULL,*catid=NULL;
	PassableStruct *p=NULL;

	if(b!=NULL)
	{
		p=(PassableStruct *)b->funcargs;
		FINDRSCGETINT(p->r,"FISCAL YEAR",&fiscalyr1);
		FINDRSCGETSTRING(p->r,"PO IDENTIFICATION",&poid1);
		FINDRSCGETSTRING(p->r,"ITEM IDENTIFICATION",&itemid1);
		FINDRSCGETSTRING(p->r,"VENDOR ITEM NUMBER",&venitemno1);
		ZERNRD(purlin);
		if(ADVEQLNRDKEYsec(purlin,1,targetkey,RecSubData,p))
		{
			KEYNRD(purlin,1);
		} else {
			FINDFLDGETINT(purlin,"FISCAL YEAR",&fiscalyr1);
			FINDFLDGETSTRING(purlin,"PO IDENTIFICATION",&poid1);
			FINDFLDGETSTRING(purlin,"ITEM IDENTIFICATION",&itemid1);
			FINDFLDGETSTRING(purlin,"VENDOR ITEM NUMBER",&venitemno1);
			FINDFLDGETSTRING(purlin,"CATEGORY IDENTIFICATION",&catid);
			FINDFLDGETINT(purlin,"LINE NUMBER",&linenum);
			FINDFLDSETINT(purmst,"FISCAL YEAR",fiscalyr1);
			FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid1);
			if(ADVEQLNRDsec(purmst,1,RecSubData,p)) KEYNRD(purmst,1);
			FINDFLDGETSTRING(purmst,"VENDOR IDENTIFICATION",&venid1);
			FINDRSCSETSTRING(p->r,"VENDOR IDENTIFICATION",venid1);
		}	
		FINDRSCSETINT(p->r,"[PURLIN][LINE NUMBER]",linenum);
		FINDRSCSETINT(p->r,"FISCAL YEAR",fiscalyr1);
		FINDRSCSETSTRING(p->r,"PO IDENTIFICATION",poid1);
		FINDRSCSETSTRING(p->r,"ITEM IDENTIFICATION",itemid1);
		FINDRSCSETSTRING(p->r,"VENDOR ITEM NUMBER",venitemno1);
		FINDRSCSETSTRING(p->r,"FIXED ASSETS CATEGORY IDENTIFICATION",catid);
		rsrc2filerecord(purlin,p->r);
		if(poid1!=NULL) Rfree(poid1);
		if(venid1!=NULL) Rfree(venid1);
		if(itemid1!=NULL) Rfree(itemid1);
		if(venitemno1!=NULL) Rfree(venitemno1);
		getsupportingrecords(p->r,FALSE,p);
		changepoid(p->r,FALSE,p);
		computeallSCRNvirtuals(p->r);
		updateallrsrc(p->r);
	}
}
static void browse_items(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	int fiscalyr=(-1);
	char *poid=NULL;

	readallwidgets(parent);
	FINDRSCSETSTRING(parent,"ITEM IDENTIFICATION",NULL);
	FINDRSCSETSTRING(parent,"VENDOR ITEM NUMBER",NULL);
	if(mbl_items==NULL)
	{
		brsrc=RDArsrcNEW(module,"ADD PO RECEIVED ITEMS BROWSE");
		srsrc=RDArsrcNEW(module,"ADD PO RECEIVED ITEMS SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,"ADD PO RECEIVED ITEMS DEFINE LIST");
		addDFincvir(brsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(brsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmsr);
		addDFincvir(srsrc,module,"PURMST",NULL,purmst);
		addDFincvir(srsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmsr);
		GET_SCREEN_VIRTUALS(brsrc,0);
		GET_SCREEN_VIRTUALS(drsrc,1);
		GET_SCREEN_VIRTUALS(srsrc,2);
		if(finyear!=(-1))
		{
			FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr);
			FINDFLDSETINT(finyear,"FISCAL YEAR",fiscalyr);
			if(ADVEQLNRDsec(finyear,1,RecSubData,p))
			{
				ZERNRD(finyear);
			} else {
				file2rsrc(finyear,brsrc,FALSE);
				filerecord2rsrc(finyear,brsrc);
			}
		}
		if(purmst!=(-1))
		{
			FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid);
			FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
			if(ADVEQLNRDsec(purmst,1,RecSubData,p))
			{
				ZERNRD(purmst);
			} else {
				file2rsrc(purmst,brsrc,FALSE);
				filerecord2rsrc(purmst,brsrc);
			}
			if(poid!=NULL) Rfree(poid);
		}
		mbl_items=ADVSbrowse(purlin,1,brsrc,srsrc,drsrc,
			initfunc_items,breakfunc_items,selectfunc_items,
			NULL,okfunc_items,NULL,p,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_items->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_items);
		}
	}
}
static void browse_venitems(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	short keyno=(-1);
	int fiscalyr=(-1);
	char *poid=NULL;

	readallwidgets(parent);
	FINDRSCSETSTRING(parent,"ITEM IDENTIFICATION",NULL);
	FINDRSCSETSTRING(parent,"VENDOR ITEM NUMBER",NULL);
	if(mbl_venitems==NULL)
	{
		brsrc=RDArsrcNEW(module,"ADD PO RECEIVED VENDOR ITEMS BROWSE");
		srsrc=RDArsrcNEW(module,"ADD PO RECEIVED VENDOR ITEMS SEARCH BROWSE");
		drsrc=RDArsrcNEW(module,"ADD PO RECEIVED VENDOR ITEMS DEFINE LIST");
		addDFincvir(brsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(brsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,module,"PURMST",NULL,purmst);
		addDFincvir(drsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmsr);
		addDFincvir(srsrc,module,"PURMST",NULL,purmst);
		addDFincvir(srsrc,"FINMGT","FINYEAR",NULL,finyear);
		addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmsr);
		GET_SCREEN_VIRTUALS(brsrc,0);
		GET_SCREEN_VIRTUALS(drsrc,1);
		GET_SCREEN_VIRTUALS(srsrc,2);
		if(finyear!=(-1))
		{
			FINDRSCGETINT(parent,"FISCAL YEAR",&fiscalyr);
			FINDFLDSETINT(finyear,"FISCAL YEAR",fiscalyr);
			if(ADVEQLNRDsec(finyear,1,RecSubData,p))
			{
				ZERNRD(finyear);
			} else {
				file2rsrc(finyear,brsrc,FALSE);
				filerecord2rsrc(finyear,brsrc);
			}
		}
		if(purmst!=(-1))
		{
			FINDRSCGETSTRING(parent,"PO IDENTIFICATION",&poid);
			FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
			if(ADVEQLNRDsec(purmst,1,RecSubData,p))
			{
				ZERNRD(purmst);
			} else {
				file2rsrc(purmst,brsrc,FALSE);
				filerecord2rsrc(purmst,brsrc);
			}
			if(poid!=NULL) Rfree(poid);
		}
		keyno=KEYNUMBERBYNAME("PURORD","PURLIN","PURLIN");
		if(keyno==(-1)) keyno=1;
		mbl_venitems=ADVSbrowse(purlin,keyno,brsrc,srsrc,drsrc,
			initfunc_items,breakfunc_items,selectfunc_items,
			NULL,okfunc_items,NULL,p,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_venitems->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_venitems);
		}
	}
}
static void acqfunc(RDArsrc *mainrsrc)
{
	char *acq_date=NULL;

	readwidget(mainrsrc, "FIXED ASSETS ACQUISITION DATE");
	FINDRSCGETSTRING(mainrsrc, "FIXED ASSETS ACQUISITION DATE", &acq_date);
	if((RDAstrlen(acq_date))==10)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"SAVE",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"SAVE",FALSE);
	}
	if(acq_date!=NULL) Rfree(acq_date);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	int zero=0,qty=0;
	char *name=NULL,*sortname=NULL/* ,flag=FALSE */;
	char zero_fill=FALSE;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL;
	PassableStruct *p=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"ADD PO ITEMS RECEIVED")) 
	{
		RDAAPPMAINLOOP();
		return;
	}

	if(XPERT_SETUP->FIXASS)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FIXASS")+11);
		#ifndef WIN32
			sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FIXASS");
		#endif
		#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FIXASS");
		#endif

		gsv=RDAGenericSetupNew("FIXASS","FIXED ASSET ITEM PURCHASE AMOUNT SOURCE");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				fa_source=*gsv->value.short_value;
			} else {
				fa_source=FALSE;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("FIXASS","CAPITAL ASSET THRESHOLD");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				fa_cap_thresh=*gsv->value.float_value;
			} else {
				fa_cap_thresh=500000;
			}
		}
		if(diagapps)
		{
			prterr("Capital Asset Threshold: %.0f\n",fa_cap_thresh);
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);
	}

	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if(argc>2)
	{
		audit_reportname=stralloc(argv[1]);
	} else {
		audit_reportname=stralloc("RECADT");
	}
	if((filenum=APPOPNNRD(module,"PURREC",TRUE,TRUE))==(-1)) return;
	if((purlin=APPOPNNRD(module,"PURLIN",TRUE,FALSE))==(-1)) return;
	if((purmst=APPOPNNRD(module,"PURMST",TRUE,FALSE))==(-1)) return;
	if((finyear=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if((venmsr=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	nokeys=NUMKEYS(filenum);
	keys=KEYPOINTER(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	parent=RDArsrcNEW(module,"ADD PO ITEMS RECEIVED W/O LISTS");
	if(XPERT_SETUP->FIXASS)
	{
		if(diagapps)
		{
			prterr("Adding Fixed Assets fields.\n");
		}
		addstdrsrc(parent,"FIXED ASSETS ZERO FILL",BOOLNS,1,&zero_fill,TRUE);
		addstdrsrc(parent,"FIXED ASSETS CATEGORY IDENTIFICATION",PLAINTEXT,15,NULL,TRUE);
		addstdrsrc(parent,"FIXED ASSETS ACQUISITION DATE",DATES,10,CURRENT_DATE10,TRUE);
		FINDRSCSETFUNC(parent,"FIXED ASSETS ACQUISITION DATE",acqfunc,NULL);
		addstdrsrc(parent,"FIXED ASSETS STATUS IDENTIFICATION",PLAINTEXT,15,NULL,TRUE);
		addstdrsrc(parent,"FIXED ASSETS DEFINITION TYPE",SHORTV,2,NULL,TRUE);
		addstdrsrc(parent,"FIXED ASSETS FUND",PLAINTEXT,15,NULL,TRUE);
		addstdrsrc(parent,"FIXED ASSETS PREFIX",PLAINTEXT,5,NULL,TRUE);
		addstdrsrc(parent,"FIXED ASSETS SUFFIX",PLAINTEXT,5,NULL,TRUE);
		addstdrsrc(parent,"FIXED ASSETS START NUMBER",DOUBLEV,5,NULL,TRUE);
		FINDRSCSETEDITABLE(parent,"FIXED ASSETS START NUMBER",TRUE);
		if(diagapps)
		{
			prterr("Finished adding Fixed Assets fields.\n");
		}
	}
	definelist=RDArsrcNEW(module,"ADD PO ITEMS RECEIVED DEFINE LIST");
	addDFincvir(parent,module,"PURREC",NULL,filenum);
	addDFincvir(parent,module,"PURLIN",NULL,purlin);
	addDFincvir(parent,module,"PURMST",NULL,purmst);
	addDFincvir(parent,"FINMGT","FINYEAR",NULL,finyear);
	addDFincvir(parent,"VENPMT","VENMSR",NULL,venmsr);
	addDFincvir(definelist,module,"PURREC",NULL,filenum);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
        makeDBsortfile(dbsort);
        dbsort->fileno=OPNDBsort(dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=parent;
	p->a=dbsort;
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,p);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(finyear,parent,FALSE);
					addstdrsrc(parent,"FISCAL YEAR",LONGV,4,&zero,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"FISCAL YEAR",changepoidcb,p);
				}
			} else if(!RDAstrcmp(fldx->name,"PO IDENTIFICATION"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(purmst,parent,FALSE);
					addstdrsrc(parent,"PO IDENTIFICATION",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"PO IDENTIFICATION",check_poidcb,p);
				}
			} else if(!RDAstrcmp(fldx->name,"QUANTITY"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addstdrsrc(parent,"QUANTITY",SLONGV,8,&qty,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"QUANTITY",changepoidcb,p);
				}
			} else if(!RDAstrcmp(fldx->name,"INVENTORY LOCATION IDENTIFICATION"))
			{
			} else if(!RDAstrcmp(fldx->name,"INVENTORY SUB-LOCATION IDENTIFICATION"))
			{
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				edit_rsrc=TRUE;
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+3);
				} else name=Rmalloc(RDAstrlen(fldx->name)+3);
				sprintf(name,"%s",fldx->name);
				makefieldrsrc(parent,name,fldx->type,
					fldx->len,TRUE);
			}
		}
	}
	if(name!=NULL) Rfree(name);
	file2rsrc(purlin,parent,FALSE);
	file2rsrc(venmsr,parent,FALSE);
	p->r=parent;
	addstdrsrc(parent,"VENDOR IDENTIFICATION",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(parent,"VENDOR IDENTIFICATION",changevenidcb,p);
	addstdrsrc(parent,"VENDOR ITEM NUMBER",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(parent,"VENDOR ITEM NUMBER",changepoidcb,p);
	addstdrsrc(parent,"ITEM IDENTIFICATION",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(parent,"ITEM IDENTIFICATION",changepoidcb,p);
/*
	addstdrsrc(parent,"SELECT VENDOR ITEM",BOOLNS,1,&flag,TRUE);
	FINDRSCSETFUNC(parent,"VENDOR ITEM NUMBER",changepoidcb,p);
	addstdrsrc(parent,"SELECT ITEM",BOOLNS,1,&flag,TRUE);
	FINDRSCSETFUNC(parent,"ITEM IDENTIFICATION",changepoidcb,p);
*/
	nofields=0;
	nokeys=0;
	prev_purrec=APPlibNEW();
	addAPPlib(prev_purrec,"No PO Item Receives added this session");
	addlstrsrc(parent,"PREVIOUS PO ITEMS RECEIVED",&x,TRUE,NULL,prev_purrec->numlibs,
		&prev_purrec->libs,NULL);
	FINDRSCSETSTRING(parent,"ENTRY DATE",CURRENT_DATE);
	FINDRSCSETSTRING(parent,"ENTRY TIME",CURRENT_TIME);
	savelist(definelist,p);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	p->r=parent;
	addbtnrsrc(parent,"BROWSE YEARS",TRUE,browse_finyear,p);
	addbtnrsrc(parent,"BROWSE PO'S",TRUE,browse_purmst,p);
	addbtnrsrc(parent,"BROWSE VENDORS",TRUE,browse_venmsr,p);
	addbtnrsrc(parent,"BROWSE VENDOR ITEMS",TRUE,browse_venitems,p);
	addbtnrsrc(parent,"BROWSE ITEMS",TRUE,browse_items,p);
	addbtnrsrc(parent,"BROWSE PO LINES",TRUE,browse_purlin,p);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"DEFINE PREVIOUS LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_defaults,p);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,p);
	addrfcbrsrc(parent,"QUIT",TRUE,quit_check,p);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(parent,p);
	APPmakescrn(parent,FALSE,quit_record_freens,p,TRUE);
	if(XPERT_SETUP->FIXASS)
	{
		acqfunc(parent);
	}
	RDAAPPMAINLOOP();
}
