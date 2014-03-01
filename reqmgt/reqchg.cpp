/* reqchg.c - Change Requisition Vendor / Item / Item Number */
#ifndef WIN32
#define __NAM__ "reqchg.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqchg.exe"
#endif
#include <app.hpp>

#include <reqmgt.hpp>
#include <ldval.hpp>
/*CPP_OPART reqfunc */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


static char *module="REQMGT",**selecttype=NULL,*reportname=NULL;
static char *select_expression=NULL;
static short reqitm=(-1),reqmst=(-1),vennum=(-1),finbacc=(-1),finbyr=(-1),fineyr=(-1),fineacc=(-1);
static short reqcat=(-1),reqloc=(-1),reqacc=(-1);
static int select_type=0;
static APPlib *methods=NULL,*categories=NULL;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static void save_record(RDArsrc *,short,DBsort *);
static void quit_record(RDArsrc *,DBsort *);
static void save_record_up(RDArsrc *,DBsort *);
static void save_check(RDArsrc *,short,DBsort *);
static void save_record_nup(RDArsrc *,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
static void getvendor(RDArsrc *,DBsort *);
DBsort *dbsort=NULL;
RDAfinmgt *fmgt=NULL;
RDArsrc *mainrsrc=NULL;
static void selectm(void *,DBsort *);
static void checkvendorcb(RDArsrc *);
static short checkvendor(RDArsrc *,short);
static int CHANGE_COUNT=0;
static REQACC *ACCOUNTS=NULL;

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static void ReqChgSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn;
	int y;
	RDAvirtual *v;
	DBsort *dbsort;
	RDArsrc *r;

	r=p->r;
	dbsort=p->a;
	if(diageval)
	{
		prterr("DIAG ReqChgSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
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
				occurrence=atoi(gn);
				gn=strtok(NULL,",]");
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
					temp=stralloc(field->data.string_value);
					str_length=(temp!=NULL?RDAstrlen(temp):0);
					if(length<1) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(!isEMPTY(temp) && str_length>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(temp))
					{
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
			}
			if(start<1) start=1;
		}
		temp=GETCURRENTDATE10();
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
				start=atoi(gn);
				gn=strtok(NULL,",]");
				length=atoi(gn);
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
						temp=EVALUATEstr(v->expression,ReqChgSubData,p);
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
							start=atoi(gn);
							gn=strtok(NULL,"]");
							length=atoi(gn);
						}
						if(start<1) start=1;

					}
					if(length==0) length=v->dlen;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					temp=stralloc(v->value.string_value);
					if(!isEMPTY(temp) && v->dlen>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(temp))
					{
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=EVALUATEsht(v->expression,ReqChgSubData,p);
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
						*v->value.integer_value=EVALUATEint(v->expression,ReqChgSubData,p);
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
						*v->value.float_value=EVALUATEdbl(v->expression,ReqChgSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,ReqChgSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,ReqChgSubData,p);
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
								start=atoi(gn);
								gn=strtok(NULL,"]");
								length=atoi(gn);
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
		prterr("DIAG ReqChgSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
APPlib *makefieldvallist()
{
	APPlib *list=NULL,*virflist=NULL;
	RDArsrc *tmprsrc=NULL;
	short dumb=0,addvf=FALSE;
	char *libx=NULL;
	int y,x,z;
	DFincvir *i,*a;
	DFvirtual *d;

	list=APPlibNEW();
	AddFileFields(list,reqitm);
	AddFileFields(list,reqmst);
	AddFileFields(list,reqloc);
	AddFileFields(list,vennum);
	AddFileFields(list,finbacc);
	AddFileFields(list,finbyr);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"REQITM",NULL,dumb);
	addDFincvir(tmprsrc,module,"REQMST",NULL,dumb);
	addDFincvir(tmprsrc,module,"REQLOC",NULL,dumb);
	addDFincvir(tmprsrc,"CATALOGUE","CATMST",NULL,dumb);
	addDFincvir(tmprsrc,"VENPMT","VENMSR",NULL,dumb);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/REQMGT.VIR",CURRENTDIRECTORY);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(list,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	addAPPlibNoDuplicates(list,"[SYSTEM DATE]");
	addAPPlibNoDuplicates(list,"[SYSTEM TIME]");
	addAPPlibNoDuplicates(list,"[LOGIN IDENTIFICATION]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION NAME]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ADDRESS #1]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ADDRESS #2]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION CITY]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION STATE]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION ZIP]");
	addAPPlibNoDuplicates(list,"[ORGANIZATION PHONE]");
	addAPPlibNoDuplicates(list,"[STARTING FISCAL MONTH]");
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	SORTAPPlib(list);
	return(list);
}
void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist();
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
static void checkvendorcb(RDArsrc *mainrsrc)
{
	char *venid=NULL;

	readwidget(mainrsrc,"VENDOR IDENTIFICATION");
	FINDRSCGETSTRING(mainrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",venid);
	if(venid!=NULL) Rfree(venid);
	checkvendor(mainrsrc,TRUE);
	updatefilerecord2rsrc(vennum,mainrsrc);	
	updateSCRNvirtuals(mainrsrc);
}
static short checkvendor(RDArsrc *mainrsrc,short dg)
{
	char delflag=FALSE,*venid=NULL,*warnmessage=NULL;

	if(vennum!=(-1))
	{
		FINDFLDGETSTRING(reqitm,"VENDOR IDENTIFICATION",&venid);
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",venid);
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc))
		{
			KEYNRD(vennum,1);
			if(dg)
			{
				warnmessage=Rmalloc(RDAstrlen(venid)+170);
				sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Vendor in the [VENPMT][VENMSR] data file.",(venid==NULL?"":venid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The VENDOR IDENTIFICATION of [%s] entered does not exist in the [VENPMT][VENMSR] data file.",(venid==NULL?"":venid));
				if(warnmessage!=NULL) Rfree(warnmessage);
			}
			if(venid!=NULL) Rfree(venid);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(vennum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				if(dg)
				{
					warnmessage=Rmalloc(RDAstrlen(venid)+200);
					sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is deleted.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a deleted state.",(venid==NULL?"":venid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The VENDOR IDENTIFICATION of [%s] entered is deleted.  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a deleted state.",(venid==NULL?"":venid));
					if(warnmessage!=NULL) Rfree(warnmessage);
				}
				if(venid!=NULL) Rfree(venid);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(vennum,"ACTIVE",&delflag);
				if(!delflag)
				{
					if(dg)
					{
						warnmessage=Rmalloc(RDAstrlen(venid)+200);
						sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not Active.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a non-Active state.",(venid==NULL?"":venid));
						WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
						prterr("The VENDOR IDENTIFICATION of [%s] entered is not Active.  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a non-Active state.",(venid==NULL?"":venid));
						if(warnmessage!=NULL) Rfree(warnmessage);
					}
					if(venid!=NULL) Rfree(venid);
					return(FALSE);
				} else {
					FINDFLDGETCHAR(vennum,"CATALOGUE VENDOR",&delflag);
					if(!delflag)
					{
						if(dg)
						{
							warnmessage=Rmalloc(RDAstrlen(venid)+250);
							sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not a Requisition Vendor.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up as a non-Requisition Vendor.",(venid==NULL?"":venid));
							WARNINGDIALOG("WARNING DIALOG SCREEN","NON_CATALOGUE VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
							prterr("The VENDOR IDENTIFICATION of [%s] entered is not a Requisition Vendor .  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up as a non-Requisition Vendor.",(venid==NULL?"":venid));
							if(warnmessage!=NULL) Rfree(warnmessage);
						}
						if(venid!=NULL) Rfree(venid);
						return(FALSE);
					}
				}
			}
		}
		if(venid!=NULL) Rfree(venid);
		if(warnmessage!=NULL) Rfree(warnmessage);
	}
	return(TRUE);
}
void select_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE;
	char changed=TRUE,deleteflag=FALSE,*curdate=NULL,*curtime=NULL,*dedid=NULL;
	int x,numfields=0,trans=0;
	NRDfield *fields=NULL,*field=NULL;
	double amt=0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;
	PassableStruct *p=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Changing Requisition Items...",NULL);
		addDFincvir(tmprsrc,module,"REQITM",NULL,reqitm);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		curdate=GETCURRENTDATE10();
		numfields=NUMFLDS(reqitm);
		fields=FLDPOINTER(reqitm);
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			curdate=GETCURRENTDATE10();
			curtime=GETCURRENTTIME();
			if(!deleteflag)
			{
				LOCNRDFILE(reqitm);
				COPYFIELD(dbsort->fileno,reqitm,"LOCATION IDENTIFICATION");
				COPYFIELD(dbsort->fileno,reqitm,"REQUISITION IDENTIFICATION");
				COPYFIELD(dbsort->fileno,reqitm,"ITEM NUMBER");
				if(!ADVEQLNRDsec(reqitm,1,ReqChgSubData,p)) prev=RDATDataNEW(reqitm);
					else prev=NULL;
				for(x=0,field=fields;x<numfields;++x,++field)
				{
					if(!RDAstrcmp(field->name,"ITEM NUMBER"))
					{
						FINDFLDGETINT(dbsort->fileno,"ITEM NUMBER",&trans);
						FINDFLDSETINT(reqitm,"ITEM NUMBER",trans);
					} else if(!RDAstrcmp(field->name,
						"REQUISITION IDENTIFICATION"))
					{
						FINDFLDGETSTRING(dbsort->fileno,
							"REQUISITION IDENTIFICATION",&dedid);
						FINDFLDSETSTRING(reqitm,
							"REQUISITION IDENTIFICATION",dedid);
					} else if(!RDAstrcmp(field->name,"AMOUNT"))
					{
						FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amt);
						FINDFLDSETDOUBLE(reqitm,"AMOUNT",amt);
					} else if(!RDAstrcmp(field->name,"CHANGED DATE"))
					{
						FINDFLDSETSTRING(reqitm,"CHANGED DATE",curdate);
					} else if(!RDAstrcmp(field->name,"CHANGED TIME"))
					{
						FINDFLDSETSTRING(reqitm,"CHANGED TIME",curtime);
					} else if(!RDAstrcmp(field->name,"CHANGED"))
					{
						FINDFLDSETCHAR(reqitm,"CHANGED",changed);	
					} else if(!RDAstrcmp(field->name,"CHANGED BY"))
					{
						FINDFLDSETSTRING(reqitm,"CHANGED BY",USERLOGIN);
					} else {
						COPYFIELD(dbsort->fileno,reqitm,field->name);
					} 
				}
				ADVWRTTRANSsec(reqitm,0,NULL,prev,ReqChgSubData,p);
				if(prev!=NULL) FreeRDATData(prev);
				UNLNRDFILE(reqitm);
				++CHANGE_COUNT;
				update_diag=TRUE;
			}
			if(tmprsrc!=NULL)
				update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(p!=NULL) Rfree(p);
		if(dedid!=NULL) Rfree(dedid);
		if(curdate!=NULL) Rfree(curdate);
		if(curtime!=NULL) Rfree(curtime);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
			tmprsrc=NULL;
		}
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		dbsort=NULL;
	}
}
void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field,*FIELDS=NULL;
	int x=0,numfields=0;
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"CHANGE REQUISITION SORT SEQUENCE");
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
		addDBfield(dbsrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsrt,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	numfields=NUMFLDS(reqitm);
	FIELDS=FLDPOINTER(reqitm);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!RDAstrcmp(field->name,"DELETEFLAG"))
			{
			} else if(!strncmp(field->name,"CHANGED",7))
			{
			} else {
				addDBfield(dbsrt,field->name,field->type,field->len);
			}
		}
	}
	key=KEYNUM(reqitm,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt,int qty,double unitrate,double total,
	double percdisc,RDArsrc *mainrsrc)
{
	char flag=FALSE,deleteflag=FALSE,*temp=NULL,atc=FALSE;
	NRDfield *field,*FIELDS=NULL;
	int x=0,numfields=0,selcat=0,selmethod=0;
	short mthd=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsrt->fileno);
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		p=Rmalloc(sizeof(PassableStruct));
		p->r=mainrsrc;
		p->a=dbsort;
		computevirtual(csortsequence,ReqChgSubData,p);
		if(p!=NULL) Rfree(p);
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
				FINDFLDSETSTRING(dbsrt->fileno,csortsequence->name,
					csortsequence->value.string_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				FINDFLDSETCHAR(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				FINDFLDSETDOUBLE(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				FINDFLDSETSHORT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				FINDFLDSETINT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.integer_value);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
	}
	numfields=NUMFLDS(reqitm);
	FIELDS=FLDPOINTER(reqitm);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!RDAstrcmp(field->name,"DELETEFLAG"))
			{
			} else if(!strncmp(field->name,"CHANGED",7))
			{
			} else if(!RDAstrcmp(field->name,"CATEGORY IDENTIFICATION"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET CATEGORY IDENTIFICATION",&flag);
				if(flag)
				{
					FINDRSCGETINT(mainrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
					FINDFLDSETSTRING(dbsrt->fileno,"CATEGORY IDENTIFICATION",
						categories->libs[selcat]);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else if(!RDAstrcmp(field->name,"ITEM IDENTIFICATION"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET ITEM IDENTIFICATION",&flag);
				if(flag)
				{
					FINDRSCGETSTRING(mainrsrc,"ITEM IDENTIFICATION",&temp);
					FINDFLDSETSTRING(dbsrt->fileno,"ITEM IDENTIFICATION",
						temp);
					if(temp!=NULL) Rfree(temp);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else if(!RDAstrcmp(field->name,"ADDRESS IDENTIFICATION"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET ADDRESS IDENTIFICATION",&flag);
				if(flag)
				{
					FINDRSCGETSTRING(mainrsrc,"ADDRESS IDENTIFICATION",&temp);
					FINDFLDSETSTRING(dbsrt->fileno,"ADDRESS IDENTIFICATION",
						temp);
					if(temp!=NULL) Rfree(temp);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else if(!RDAstrcmp(field->name,"ITEM NAME"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET ITEM NAME",&flag);
				if(flag)
				{
					FINDRSCGETSTRING(mainrsrc,"ITEM NAME",&temp);
					FINDFLDSETSTRING(dbsrt->fileno,"ITEM NAME",temp);
					if(temp!=NULL) Rfree(temp);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else if(!RDAstrcmp(field->name,"VENDOR IDENTIFICATION"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET VENDOR IDENTIFICATION",&flag);
				if(flag)
				{
					FINDRSCGETSTRING(mainrsrc,"VENDOR IDENTIFICATION",&temp);
					FINDFLDSETSTRING(dbsrt->fileno,"VENDOR IDENTIFICATION",
						temp);
					if(temp!=NULL) Rfree(temp);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else if(!RDAstrcmp(field->name,"VENDOR ITEM NUMBER"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET VENDOR ITEM NUMBER",&flag);
				if(flag)
				{
					FINDRSCGETSTRING(mainrsrc,"VENDOR ITEM NUMBER",&temp);
					FINDFLDSETSTRING(dbsrt->fileno,"VENDOR ITEM NUMBER",
						temp);
					if(temp!=NULL) Rfree(temp);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else if(!RDAstrcmp(field->name,"ADD TO CATALOGUE"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET ADD TO CATALOGUE",&flag);
				if(flag)
				{
					FINDRSCGETCHAR(mainrsrc,"ADD TO CATALOGUE",&atc);
					FINDFLDSETCHAR(dbsrt->fileno,"ADD TO CATALOGUE",atc);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else if(!RDAstrcmp(field->name,"PERCENTAGE DISCOUNT"))
			{
				FINDFLDSETDOUBLE(dbsrt->fileno,"PERCENTAGE DISCOUNT",percdisc);
			} else if(!RDAstrcmp(field->name,"UNIT RATE"))
			{
				FINDFLDSETDOUBLE(dbsrt->fileno,"UNIT RATE",unitrate);
			} else if(!RDAstrcmp(field->name,"AMOUNT"))
			{
				FINDFLDSETDOUBLE(dbsrt->fileno,"AMOUNT",total);
			} else if(!RDAstrcmp(field->name,"QUANTITY"))
			{
				FINDFLDSETINT(dbsrt->fileno,"QUANTITY",qty);
			} else if(!RDAstrcmp(field->name,"METHOD"))
			{
				FINDRSCGETCHAR(mainrsrc,"SET METHOD",&flag);
				if(flag)
				{
					FINDRSCGETINT(mainrsrc,"METHODS",&selmethod);
					mthd=(short)selmethod;
					FINDFLDSETSHORT(dbsrt->fileno,"METHOD",mthd);
				} else COPYFIELD(reqitm,dbsrt->fileno,field->name);
			} else {
				COPYFIELD(reqitm,dbsrt->fileno,field->name);
			}
		}
	}
	WRTNRD(dbsrt->fileno,0,NULL);
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	selectm(targetkey,blist->funcargs);
}
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without processing these Requisition Changes?",doexit,NULL,FALSE,blist);
	} else {
		doexit(blist);
	}
}
static void doexit(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	dbsort=blist->funcargs;
	brsrc=blist->mainrsrc;
	drsrc=blist->definelist;
	srsrc=blist->searchrsrc;
	FREE_BROWSE_EXIT(blist);
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
	if(dbsort!=NULL)
	{
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static void saveexit(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	dbsort=blist->funcargs;
	brsrc=blist->mainrsrc;
	drsrc=blist->definelist;
	srsrc=blist->searchrsrc;
	FREE_BROWSE_EXIT(blist);
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
		free_rsrc(brsrc);
		brsrc=NULL;
	}
	if(WindowCount<=2) select_backend(dbsort);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
void addvouchers(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,dodiag=FALSE,testfile=FALSE,pmonth=0,keyrange=FALSE;
	char deleteflag=FALSE,*sortname=NULL,submitted=FALSE,active=FALSE;
	char verified=FALSE,approved=FALSE,unitrateflag=FALSE;
	char percdiscflag=FALSE;
	char *locid=NULL,*reqid=NULL,*venid=NULL,*catid=NULL;
	char *e=NULL;
	int x,qty=0,itemnum=0,procyrid=0;
	long size_of_file=0;
	double percdisc=0.0,amt=0,total=0.0,unitrate=0.0;
	RDAvirtual *v=NULL;
	PassableStruct *p=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"SELECT TYPE",&select_type);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&select_expression);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(reqitm,rs);
	ReadRangeScreen(reqmst,rs);
	ReadRangeScreen(reqloc,rs);
	ReadRangeScreen(reqcat,rs);
	ReadRangeScreen(vennum,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(reqitm,"DIAGNOSTIC SCREEN",module,
		"Selecting Requisition Items",NULL);
	addDFincvir(tmprsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(tmprsrc,module,"REQITM",NULL,reqitm);
	addDFincvir(tmprsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(tmprsrc,module,"REQACC",NULL,reqacc);
	addDFincvir(tmprsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(tmprsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(tmprsrc,"FINMGT","FINEYR",NULL,fineyr);
	addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,finbyr);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(tmprsrc!=NULL)
	{
		if(!ADVmakescrn(tmprsrc,TRUE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=tmprsrc;
	p->a=dbsort;
	ZERNRD(reqitm);
/*
	ef=ADVFRSNRDsec(reqitm,1,ReqChgSubData,p);
*/
	keyrange=setfilekeyFROMrangersrc(reqitm,1,rs);
	if(keyrange)
	{
		ef=ADVGTENRDsec(reqitm,1,ReqChgSubData,p);
	} else {
		ef=ADVBEGNRDsec(reqitm,ReqChgSubData,p);
	}
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETSTRING(reqitm,"LOCATION IDENTIFICATION",&locid);
		FINDFLDGETSTRING(reqitm,"REQUISITION IDENTIFICATION",&reqid);
		FINDFLDGETINT(reqitm,"ITEM NUMBER",&itemnum);
		if(diagapps)
		{
			prterr("Selecting and Checking Requisition Item Record ([REQMGT][REQITM], Location ID=[%s], Requisition ID=[%s], and  Item [%d].",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
		}
		FINDFLDGETCHAR(reqitm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		FINDFLDGETCHAR(reqitm,"SOURCE USER VERIFIED",&verified);
		if(verified)
		{
		FINDFLDGETDOUBLE(reqitm,"AMOUNT",&amt);
		if(amt!=0.0)
		{
			testfile=testfilerangersrc(reqitm,rs);
			if(!testfile)
			{
				FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid);
				FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid);
				if(ADVEQLNRDsec(reqmst,1,ReqChgSubData,p))
				{
					KEYNRD(reqmst,1);
					if(diagapps)
					{
						prterr("WARNING: No Requisition Master Record found ([REQMGT][REQMST]) for Location ID=[%s], Requisition ID=[%s].",(locid==NULL?"":locid),(reqid==NULL?"":reqid));
					}
				}
				FINDFLDGETCHAR(reqmst,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				FINDFLDGETCHAR(reqmst,"SUBMITTED",&submitted);
				if(submitted)
				{
				FINDFLDGETCHAR(reqmst,"APPROVED",&approved);
				if(!approved || TRUE)
				{
					testfile=testfilerangersrc(reqmst,rs);
					if(!testfile)
					{
						FINDFLDSETSTRING(reqloc,"LOCATION IDENTIFICATION",locid);
						if(ADVEQLNRDsec(reqloc,1,ReqChgSubData,p))
						{
							KEYNRD(reqloc,1);
							if(diagapps)
							{
								prterr("WARNING: No Requisition Location Record found ([REQMGT][REQLOC]) for Location ID=[%s].",(locid==NULL?"":locid));
							}
						}
						FINDFLDGETCHAR(reqloc,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
						FINDFLDGETCHAR(reqloc,"ACTIVE",&active);
						if(active)
						{
							testfile=testfilerangersrc(reqloc,rs);
							if(!testfile)
							{
								FINDFLDGETSTRING(reqitm,"CATEGORY IDENTIFICATION",&catid);
								FINDFLDSETSTRING(reqcat,"CATEGORY IDENTIFICATION",catid);
								if(ADVEQLNRDsec(reqcat,1,ReqChgSubData,p))
								{
									KEYNRD(reqcat,1);
									if(diagapps)
									{
										prterr("WARNING: No Category Master Record found ([CATALOGUE][CATMST]) for Category ID=[%s].",(catid==NULL?"":catid));
									}
								}
								FINDFLDGETCHAR(reqcat,"DELETEFLAG",&deleteflag);
								if(!deleteflag)
								{
									testfile=testfilerangersrc(reqcat,rs);
									if(!testfile)
									{
										FINDFLDGETSTRING(reqitm,"VENDOR IDENTIFICATION",&venid);
										FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",venid);
										if(ADVEQLNRDsec(vennum,1,ReqChgSubData,p)) 
										{
											KEYNRD(vennum,1);
											if(diagapps)
											{
												prterr("WARNING: No Vendor Master Record found ([VENPMT][VENMST]) for Vendor ID=[%s].",(venid==NULL?"":venid));
											}
										}
										FINDFLDGETCHAR(vennum,"DELETEFLAG",
											&deleteflag);
										if(!deleteflag)
										{
											testfile=testfilerangersrc(vennum,rs);
											if(!testfile)
											{
												for(x=0,v=mainrsrc->virflds;
													x<mainrsrc->num;++x,++v)
												{
													if(ADVtestvirtualrangersrc(v,
														rs,(void (*)(...))ReqChgSubData,p)) break;
												}
												if(x>=mainrsrc->num)
												{
													FINDRSCGETCHAR(mainrsrc,"SET UNIT RATE",&unitrateflag);
													if(unitrateflag)
													{
														FINDRSCGETDOUBLE(mainrsrc,"UNIT RATE",&unitrate);
													} else FINDFLDGETDOUBLE(reqitm,"UNIT RATE",&unitrate);
													FINDRSCGETCHAR(mainrsrc,"SET PERCENTAGE DISCOUNT",&percdiscflag);
													if(unitrateflag)
													{
														FINDRSCGETDOUBLE(mainrsrc,"PERCENTAGE DISCOUNT",&percdisc);
													} else FINDFLDGETDOUBLE(reqitm,"UNIT RATE",&percdisc);
													ACCOUNTS=ADVGetREQACC(reqacc,locid,reqid,itemnum,ReqChgSubData,p);
													if(ACCOUNTS!=NULL)
													{
														FINDFLDGETDOUBLE(reqitm,"AMOUNT",&total);
														ComputeREQACC(ACCOUNTS,total);
														MoveREQACC(ACCOUNTS);
														FINDFLDGETINT(reqitm,"QUANTITY",&qty);
														if(unitrate!=0.0 && unitrateflag)
														{
															if(percdisc!=0.0)
															{
																total=round(((unitrate*qty)*100.0)-(((unitrate*qty)*100.0)*percdisc/100));
															} else {
																total=round((unitrate*qty)*100.0);
															}
														} else FINDFLDGETDOUBLE(reqitm,"AMOUNT",&total);
														ComputeREQACC(ACCOUNTS,total);
														FINDFLDGETINT(reqitm,"FISCAL YEAR",&procyrid);
														FINDFLDGETSHORT(reqitm,"PROCESSING MONTH",&pmonth);
														if(ADVCHGCHECKREQACC(ACCOUNTS,fineyr,procyrid,pmonth,fmgt,ReqChgSubData,p))
														{
															addsortrecord(dbsort,qty,unitrate,total,percdisc,mainrsrc);
															ADVREQACCDIFF_UPDATE_UNDIST(ACCOUNTS,procyrid,pmonth,fineyr,finbyr,ReqChgSubData,p);
															dodiag=TRUE;
														} else {
															if(diagapps)
															{
																prterr("Skipped Requisition [%s][%s] Item [%d] because of Appropriations too low for transactions.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
															}
														}
														FreeREQACC(ACCOUNTS);
														ACCOUNTS=NULL;
													} else {
														if(diagapps)
														{
															prterr("Skipped Requisition [%s][%s] Item [%d] because of No Account Records Available.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
														}
													}
												} else {
													if(diagapps)
													{
														prterr("Skipped Requisition [%s][%s] Item [%d] because of Ranging on a Virtual Field [%s].",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum,(v->name==NULL?"":v->name));
													}
												}
											} else {
												if(diagapps)
												{
													prterr("Skipped Requisition [%s][%s] Item [%d] because of Ranging on a Field in [VENPMT][VENMSR] (Vendor Master) data file.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
												}
											}
										} else {
											if(diagapps)
											{
												prterr("Skipped Requisition [%s][%s] Item [%d] because of Deleted [VENPMT][VENMSR] (Vendor Master) record for Vendor Identification of [%s].",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum,(venid==NULL?"":venid));
											}
										}
									} else {
										if(diagapps)
										{
											prterr("Skipped Requisition [%s][%s] Item [%d] because of Ranging on a Field in [CATALOGUE][CATEGORY] (Category Master) data file.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
										}
									}
								} else {
									if(diagapps)
									{
										prterr("Skipped Requisition [%s][%s] Item [%d] because of Deleted [CATALOGUE][CATMST] (Category Master) record for Category Identification of [%s].",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum,(catid==NULL?"":catid));
									}
								}
							} else {
								if(diagapps)
								{
									prterr("Skipped Requisition [%s][%s] Item [%d] because of Ranging on a Field in [REQMGT][REQLOC] (Requisition Location Master) data file.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
								}
							}
						} else {
							if(diagapps)
							{
								prterr("Skipped Requisition [%s][%s] Item [%d] because of In-Active [REQLOC][REQLOC] (Requisition Location Master) record.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
							}
						}
						} else {
							if(diagapps)
							{
								prterr("Skipped Requisition [%s][%s] Item [%d] because of Deleted [REQLOC][REQLOC] (Requisition Location Master) record.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
							}
						}
					} else {
						if(diagapps)
						{
							prterr("Skipped Requisition [%s][%s] Item [%d] because of Ranging on a Field in [REQMGT][REQMST] (Requisition Master) data file.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
						}
					}
				} else {
					if(diagapps)
					{
						prterr("Skipped Requisition [%s][%s] Item [%d] because of Approved [REQLOC][REQMST] (Requisition Master) record.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped Requisition [%s][%s] Item [%d] because of Non-Submitted [REQLOC][REQMST] (Requisition Master) record.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped Requisition [%s][%s] Item [%d] because of Deleted [REQLOC][REQMST] (Requisition Master) record.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
					}
				}
			} else {
				if(diagapps)
				{
					prterr("Skipped Requisition [%s][%s] Item [%d] because of Ranging on a Field in [REQMGT][REQITM] (Requisition Item Master) data file.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
				}
			}
		} else {
			if(diagapps)
			{
				prterr("Skipped Requisition [%s][%s] Item [%d] because of Zero Amount value in [REQLOC][REQITM] (Requisition Item Master)'s AMOUNT field.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("Skipped Requisition [%s][%s] Item [%d] because of Non-Source User Verified [REQLOC][REQITM] (Requisition Item Master) record.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("Skipped Requisition [%s][%s] Item [%d] because of Deleted [REQLOC][REQITM] (Requisition Item Master) record.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemnum);
			}
		}
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		}        
		if(keyrange)
		{
			ef=ADVNXTNRDsec(reqitm,1,ReqChgSubData,p);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(reqitm,1,rs);
			}
		} else {
			ef=ADVSEQNRDsec(reqitm,ReqChgSubData,p);
		}
/*
		ef=ADVNXTNRDsec(reqitm,1,ReqChgSubData,p);
*/
	}
	if(p!=NULL) Rfree(p);
	if(locid!=NULL) Rfree(locid);
	if(reqid!=NULL) Rfree(reqid);
	if(venid!=NULL) Rfree(venid);
	if(catid!=NULL) Rfree(catid);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		switch(select_type)
		{
			default:
			case 0: /* Automatic */
				select_backend(dbsort);
				break;
			case 1: /* Process Exceptions */
				brsrc=RDArsrcNEW(module,"CHANGE REQUISITIONS BROWSE");
				srsrc=RDArsrcNEW(module,"CHANGE REQUISITIONS SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"CHANGE REQUISITIONS DEFINE LIST");
				addDFincvir(drsrc,module,"REQLOC",NULL,reqloc);
				addDFincvir(drsrc,module,"REQMST",NULL,reqmst);
				addDFincvir(drsrc,"CATALOGUE","CATMST",NULL,reqcat);
				addDFincvir(drsrc,"VENPMT","VENMSR",NULL,vennum);
				addDFincvir(srsrc,module,"REQLOC",NULL,reqloc);
				addDFincvir(srsrc,module,"REQMST",NULL,reqmst);
				addDFincvir(srsrc,"CATALOGUE","CATMST",NULL,reqcat);
				addDFincvir(srsrc,"VENPMT","VENMSR",NULL,vennum);
				GET_SCREEN_VIRTUALS(drsrc,1);
				GET_SCREEN_VIRTUALS(srsrc,2);
				ZERNRD(dbsort->fileno);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				mbl_trans=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,
					selectfunction,doexit_test,
					okfunc,NULL,dbsort,TRUE,NULL,
					0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
	} else {
		ERRORDIALOG("Change Requisition Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static void makeselecttype()
{
	selecttype=Rmalloc(2*sizeof(char *));
	selecttype[0]=stralloc("Automatic");
	selecttype[1]=stralloc("Process Exceptions");
}
static void freeselecttype()
{
	short x;

	if(selecttype!=NULL)
	{
		for(x=0;x<2;++x) Rfree(selecttype[x]);
		Rfree(selecttype);
		selecttype=NULL;
	}
}
void quitselect(RDArsrc *mainrsrc)
{
	APPlib *args=NULL;

	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	CLSNRD(reqitm);
	CLSNRD(reqmst);
	CLSNRD(reqloc);
	CLSNRD(reqcat);
	CLSNRD(reqacc);
	CLSNRD(vennum);
	CLSNRD(finbacc);
	CLSNRD(finbyr);
	CLSNRD(fineyr);
	CLSNRD(fineacc);
	if(ACCOUNTS!=NULL) FreeREQACC(ACCOUNTS);
	if(categories!=NULL) freeapplib(categories);
	if(methods!=NULL) freeapplib(methods);
	if(selecttype!=NULL) freeselecttype();
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(CHANGE_COUNT>0)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(reportname!=NULL) Rfree(reportname);
	ShutdownSubsystems();
}
static void setcatid(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET CATEGORY IDENTIFICATION");
	FINDRSCGETCHAR(mainrsrc,"SET CATEGORY IDENTIFICATION",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"CATEGORY IDENTIFICATIONS",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"CATEGORY IDENTIFICATIONS",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"CATEGORY IDENTIFICATIONS",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"CATEGORY IDENTIFICATIONS",FALSE);
	}
}
static void setitemid(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET ITEM IDENTIFICATION");
	FINDRSCGETCHAR(mainrsrc,"SET ITEM IDENTIFICATION",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"ITEM IDENTIFICATION",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"ITEM IDENTIFICATION",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"ITEM IDENTIFICATION",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"ITEM IDENTIFICATION",FALSE);
	}
}
static void setitemname(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET ITEM NAME");
	FINDRSCGETCHAR(mainrsrc,"SET ITEM NAME",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"ITEM NAME",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"ITEM NAME",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"ITEM NAME",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"ITEM NAME",FALSE);
	}
}
static void setvenid(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET VENDOR IDENTIFICATION");
	FINDRSCGETCHAR(mainrsrc,"SET VENDOR IDENTIFICATION",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"VENDOR IDENTIFICATION",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"VENDOR IDENTIFICATION",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"VENDOR IDENTIFICATION",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"VENDOR IDENTIFICATION",FALSE);
	}
}
static void setvenitemnum(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET VENDOR ITEM NUMBER");
	FINDRSCGETCHAR(mainrsrc,"SET VENDOR ITEM NUMBER",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"VENDOR ITEM NUMBER",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"VENDOR ITEM NUMBER",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"VENDOR ITEM NUMBER",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"VENDOR ITEM NUMBER",FALSE);
	}
}
static void setmethod(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET METHOD");
	FINDRSCGETCHAR(mainrsrc,"SET METHOD",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"METHODS",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"METHODS",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"METHODS",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"METHODS",FALSE);
	}
}
static void setunitrate(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET UNIT RATE");
	FINDRSCGETCHAR(mainrsrc,"SET UNIT RATE",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"UNIT RATE",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"UNIT RATE",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"UNIT RATE",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"UNIT RATE",FALSE);
	}
}
static void setaddrid(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET ADDRESS IDENTIFICATION");
	FINDRSCGETCHAR(mainrsrc,"SET ADDRESS IDENTIFICATION",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"ADDRESS IDENTIFICATION",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"ADDRESS IDENTIFICATION",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"ADDRESS IDENTIFICATION",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"ADDRESS IDENTIFICATION",FALSE);
	}
}
static void setpercdisc(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET PERCENTAGE DISCOUNT");
	FINDRSCGETCHAR(mainrsrc,"SET PERCENTAGE DISCOUNT",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"PERCENTAGE DISCOUNT",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"PERCENTAGE DISCOUNT",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"PERCENTAGE DISCOUNT",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"PERCENTAGE DISCOUNT",FALSE);
	}
}
static void setaddcat(RDArsrc *mainrsrc)
{
	char t=FALSE;

	readwidget(mainrsrc,"SET ADD TO CATALOGUE");
	FINDRSCGETCHAR(mainrsrc,"SET ADD TO CATALOGUE",&t);
	if(t)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"ADD TO CATALOGUE",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"ADD TO CATALOGUE",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"ADD TO CATALOGUE",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"ADD TO CATALOGUE",FALSE);
	}
}
static void changecategory(RDArsrc *mainrsrc)
{
	int selcat=0;

	FINDRSCGETINT(mainrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	FINDFLDSETSTRING(reqcat,"CATEGORY IDENTIFICATION",categories->libs[selcat]);
	if(ADVEQLNRDsec(reqcat,1,SCRNvirtualSubData,mainrsrc))
	{
		ZERNRD(reqcat);
	}
	updatefilerecord2rsrc(reqcat,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char nv=FALSE;
	int which_type=0;
	double dolamt=0.0;

	if(InitializeSubsystems(argc,argv,module,"CHANGE REQUISITIONS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("REQCHNG");
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if((reqitm=APPOPNNRD(module,"REQITM",TRUE,TRUE))==(-1)) return;
	if((reqmst=APPOPNNRD(module,"REQMST",TRUE,FALSE))==(-1)) return;
	if((reqloc=APPOPNNRD(module,"REQLOC",TRUE,FALSE))==(-1)) return;
	if((reqacc=APPOPNNRD(module,"REQACC",TRUE,FALSE))==(-1)) return;
	if((reqcat=APPOPNNRD("CATALOGUE","CATMST",TRUE,FALSE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((fineyr=APPOPNNRD("FINMGT","FINEYR",TRUE,TRUE))==(-1)) return;
	if((fineacc=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((finbacc=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((finbyr=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
	makeselecttype();
	mainrsrc=RDArsrcNEW(module,"CHANGE REQUISITIONS");
	addDFincvir(mainrsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(mainrsrc,module,"REQITM",NULL,reqitm);
	addDFincvir(mainrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(mainrsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum);
	file2rangersrc(reqitm,mainrsrc);
	file2rangersrc(reqmst,mainrsrc);
	file2rangersrc(reqloc,mainrsrc);
	file2rangersrc(reqcat,mainrsrc);
	file2rangersrc(vennum,mainrsrc);
	file2rsrc(vennum,mainrsrc,FALSE);
	file2rsrc(reqcat,mainrsrc,FALSE);
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,
		&selecttype,NULL);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	addstdrsrc(mainrsrc,"SET CATEGORY IDENTIFICATION",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET CATEGORY IDENTIFICATION",setcatid,NULL);
	addstdrsrc(mainrsrc,"SET METHOD",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET METHOD",setmethod,NULL);
	which_type=0;
	if(methods!=NULL) freeapplib(methods);
	methods=APPlibNEW();
	addAPPlib(methods,"NOT ASSIGNED");
	addAPPlib(methods,"PURCHASE ORDER");
	addAPPlib(methods,"INVENTORY ISSUE");
	addlstrsrc(mainrsrc,"METHODS",&which_type,TRUE,NULL,methods->numlibs,
		&methods->libs,NULL);
	if(categories!=NULL) freeapplib(categories);
	ZERNRD(reqcat);
	categories=ADVMakeNRDlistAllSTRING(reqcat,module,
		"Creating Category List",1,"CATEGORY IDENTIFICATION",
		SCRNvirtualSubData,mainrsrc);
	if(categories->numlibs<1)
	{
		addAPPlib(categories,"No Categories");
	}
	addlstrsrc(mainrsrc,"CATEGORY IDENTIFICATIONS",&which_type,TRUE,
		changecategory,categories->numlibs,&categories->libs,NULL);
	addstdrsrc(mainrsrc,"SET ITEM IDENTIFICATION",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET ITEM IDENTIFICATION",setitemid,NULL);
	addstdrsrc(mainrsrc,"ITEM IDENTIFICATION",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"SET ITEM NAME",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET ITEM NAME",setitemname,NULL);
	addstdrsrc(mainrsrc,"ITEM NAME",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"SET VENDOR IDENTIFICATION",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET VENDOR IDENTIFICATION",setvenid,NULL);
	addstdrsrc(mainrsrc,"VENDOR IDENTIFICATION",VARIABLETEXT,0,NULL,TRUE);
	FINDRSCSETFUNC(mainrsrc,"VENDOR IDENTIFICATION",checkvendorcb,NULL);
	addstdrsrc(mainrsrc,"SET ADDRESS IDENTIFICATION",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET ADDRESS IDENTIFICATION",setaddrid,NULL);
	addstdrsrc(mainrsrc,"ADDRESS IDENTIFICATION",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"SET VENDOR ITEM NUMBER",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET VENDOR ITEM NUMBER",setvenitemnum,NULL);
	addstdrsrc(mainrsrc,"VENDOR ITEM NUMBER",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"SET UNIT RATE",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET UNIT RATE",setunitrate,NULL);
	addstdrsrc(mainrsrc,"UNIT RATE",DOLLARS,20,&dolamt,TRUE);
	addstdrsrc(mainrsrc,"SET PERCENTAGE DISCOUNT",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET PERCENTAGE DISCOUNT",setpercdisc,NULL);
	addstdrsrc(mainrsrc,"PERCENTAGE DISCOUNT",DECIMALV,5,&dolamt,TRUE);
	addstdrsrc(mainrsrc,"SET ADD TO CATALOGUE",BOOLNS,1,&nv,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SET ADD TO CATALOGUE",setaddcat,NULL);
	addstdrsrc(mainrsrc,"ADD TO CATALOGUE",BOOLNS,1,&nv,TRUE);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,ResetGroupDefaults,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,addvouchers,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	setcatid(mainrsrc);
	setitemid(mainrsrc);
	setitemname(mainrsrc);
	setvenid(mainrsrc);
	setvenitemnum(mainrsrc);
	setunitrate(mainrsrc);
	setaddrid(mainrsrc);
	setpercdisc(mainrsrc);
	setaddcat(mainrsrc);
	setmethod(mainrsrc);
	RDAAPPMAINLOOP();
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(reqmst!=(-1))
	{
		COPYFIELD(dbsort->fileno,reqmst,"LOCATION IDENTIFICATION");
		COPYFIELD(dbsort->fileno,reqmst,"REQUISITION IDENTIFICATION");
		if(ADVEQLNRDsec(reqmst,1,ReqChgSubData,p)) KEYNRD(reqmst,1);
		filerecord2rsrc(reqmst,mtnrsrc);
	}
	if(reqloc!=(-1))
	{
		COPYFIELD(dbsort->fileno,reqloc,"LOCATION IDENTIFICATION");
		if(ADVEQLNRDsec(reqloc,1,ReqChgSubData,p)) KEYNRD(reqloc,1);
		filerecord2rsrc(reqloc,mtnrsrc);
	}
	if(reqcat!=(-1))
	{
		COPYFIELD(dbsort->fileno,reqcat,"CATEGORY IDENTIFICATION");
		if(ADVEQLNRDsec(reqcat,1,ReqChgSubData,p)) KEYNRD(reqcat,1);
		filerecord2rsrc(reqcat,mtnrsrc);
	}
	if(vennum!=(-1))
	{
		COPYFIELD(dbsort->fileno,vennum,"VENDOR IDENTIFICATION");
		if(ADVEQLNRDsec(vennum,1,ReqChgSubData,p)) KEYNRD(vennum,1);
		filerecord2rsrc(vennum,mtnrsrc);
	}
	if(p!=NULL) Rfree(p);
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *dedid=NULL,*locid=NULL;
	int trans=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"LOCATION IDENTIFICATION",&locid);
	FINDRSCGETSTRING(mtnrsrc,"REQUISITION IDENTIFICATION",&dedid);
	FINDRSCGETINT(mtnrsrc,"ITEM NUMBER",&trans);
	FINDFLDSETSTRING(dbsort->fileno,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(dbsort->fileno,"REQUISITION IDENTIFICATION",dedid);
	FINDFLDSETINT(dbsort->fileno,"ITEM NUMBER",trans);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
	}
	getsupportingfiles(mtnrsrc,dbsort);
	if(locid!=NULL) Rfree(locid);
	if(dedid!=NULL) Rfree(dedid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getvendor(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *vendorid=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"VENDOR IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&vendorid);
	FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid);
	if(vendorid!=NULL) Rfree(vendorid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(vennum,1,ReqChgSubData,p)) KEYNRD(vennum,1);
	if(p!=NULL) Rfree(p);	
	updatefilerecord2rsrc(vennum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
void setunitamt(RDArsrc *mtnrsrc)
{
	int quantity=0;
	double unitrate=0.0,total=0.0;

	readwidget(mtnrsrc,"QUANTITY");
	if(FINDRSCGETINT(mtnrsrc,"QUANTITY",&quantity)) return;
	readwidget(mtnrsrc,"UNIT RATE");
	if(FINDRSCGETDOUBLE(mtnrsrc,"UNIT RATE",&unitrate)) return;
	total=(unitrate*quantity)*100.0;
	FINDRSCSETDOUBLE(mtnrsrc,"AMOUNT",total);
	updatersrc(mtnrsrc,"AMOUNT");
	updateSCRNvirtuals(mtnrsrc);
}
void selectm(void *targetkey,DBsort *t)
{
	int x,which_type=0,itemnum=0;
	RDArsrc *mtnrsrc=NULL;
	NRDkey *key;
	NRDpart *part;
	char *temp=NULL,*locid=NULL,*reqid=NULL;
	short nofields;
	NRDfield *fields=NULL,*fldx=NULL;
	double total=0.0;
	PassableStruct *p=NULL;

	mtnrsrc=RDArsrcNEW(module,"CHANGE REQUISITIONS MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(mtnrsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(mtnrsrc,module,"REQACC",NULL,reqacc);
	addDFincvir(mtnrsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mtnrsrc,"FINMGT","FINEYR",NULL,fineyr);
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"METHOD"))
			{
				which_type=0;
				addlstrsrc(mtnrsrc,"METHODS",&which_type,TRUE,NULL,
					methods->numlibs,&methods->libs,NULL);
			} else if(!RDAstrcmp(fldx->name,"CATEGORY IDENTIFICATION"))
			{
				which_type=0;
				addlstrsrc(mtnrsrc,"CATEGORY IDENTIFICATIONS",
					&which_type,TRUE,changecategory,categories->numlibs,
					&categories->libs,NULL);
			} else {
				makefieldrsrc(mtnrsrc,fldx->name,fldx->type,
					fldx->len,TRUE);
			}
		}
	}
	FINDRSCSETEDITABLE(mtnrsrc,"LOCATION IDENTIFICATION",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"REQUISITION IDENTIFICATION",FALSE);
	nofields=0;
	if(reqmst!=(-1)) file2rsrc(reqmst,mtnrsrc,FALSE);
	if(reqloc!=(-1)) file2rsrc(reqloc,mtnrsrc,FALSE);
	if(reqcat!=(-1)) file2rsrc(reqcat,mtnrsrc,FALSE);
	if(vennum!=(-1)) file2rsrc(vennum,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"VENDOR IDENTIFICATION",getvendor,t);
	FINDRSCSETFUNC(mtnrsrc,"UNIT RATE",setunitamt,NULL);
	key=KEYNUM(t->fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			FINDRSCSETFUNC(mtnrsrc,part->name,getfile,t);
		}
	}
	DefaultScreens(mtnrsrc);
	if(EQLNRDKEY(t->fileno,1,targetkey))
	{
		ZERNRD(dbsort->fileno);
	} else {
		singlefilerecord2rsrc(t->fileno,mtnrsrc);
		FINDFLDGETSTRING(t->fileno,"CATEGORY IDENTIFICATION",&temp);
		for(x=0;x<categories->numlibs;++x)
		{
			if(!RDAstrcmp(categories->libs[x],temp)) break;
		}
		if(x<categories->numlibs)
		{
			FINDRSCSETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",x);
		}
		if(temp!=NULL) Rfree(temp);
		nofields=0;
		FINDFLDGETSHORT(t->fileno,"METHOD",&nofields);
		x=nofields;
		FINDRSCSETINT(mtnrsrc,"METHODS",x);
	}
	getsupportingfiles(mtnrsrc,t);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	FINDFLDGETSTRING(t->fileno,"LOCATION IDENTIFICATION",&locid);
	FINDFLDGETSTRING(t->fileno,"REQUISITION IDENTIFICATION",&reqid);
	FINDFLDGETINT(t->fileno,"ITEM NUMBER",&itemnum);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	ACCOUNTS=ADVGetREQACC(reqacc,locid,reqid,itemnum,ReqChgSubData,p);
	if(p!=NULL) Rfree(p);
	if(ACCOUNTS!=NULL)
	{
		FINDFLDGETDOUBLE(t->fileno,"AMOUNT",&total);
		ComputeREQACC(ACCOUNTS,total);
		MoveREQACC(ACCOUNTS);
	}
	if(locid!=NULL) Rfree(locid);
	if(reqid!=NULL) Rfree(reqid);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quit_record,dbsort,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_check(mtnrsrc,TRUE,dbsort);
}
static void save_record_nup(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_check(mtnrsrc,FALSE,dbsort);
}
static void save_check(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int selcat=0,selmethod=0;
	int procyrid=0;
	short mthd=0;
	short pmonth=0;
	double total=0.0;
	PassableStruct *p=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	FINDRSCGETINT(mtnrsrc,"METHODS",&selmethod);
	mthd=(short)selmethod;
	FINDFLDSETSHORT(dbsort->fileno,"METHOD",mthd);
	FINDFLDSETSTRING(dbsort->fileno,"CATEGORY IDENTIFICATION",
		categories->libs[selcat]);
	ComputeREQACC(ACCOUNTS,total);
	FINDFLDGETINT(reqitm,"FISCAL YEAR",&procyrid);
	FINDFLDGETSHORT(reqitm,"PROCESSING MONTH",&pmonth);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVCHGCHECKREQACC(ACCOUNTS,fineyr,procyrid,pmonth,fmgt,ReqChgSubData,p))
	{
		save_record(mtnrsrc,update_list,dbsort);
	}
	if(p!=NULL) Rfree(p);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;

	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,"Browse Diagnostic",NULL);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE))
					ForceWindowUpdate(tmprsrc);
			}
			xmakebrowselist(mbl_trans,tmprsrc);
			if(mbl_trans->list->num<=valuex) valuex=0;
			mbl_trans->list->keyvalue=mbl_trans->list->key[valuex];
			if(!FINDRSCSETLIST(mbl_trans->mainrsrc,"BROWSE LIST",valuex,
				mbl_trans->list->num,&mbl_trans->list->string))
			{
				updatersrc(mbl_trans->mainrsrc,"BROWSE LIST");
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
				tmprsrc=NULL;
			}
			ForceWindowUpdate(mtnrsrc);
		}
	}
	if(mtnrsrc!=NULL) killwindow(mtnrsrc);
	quit_record(mtnrsrc,dbsort);
}
static void quit_record(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mtnrsrc=NULL;
	if(ACCOUNTS!=NULL) FreeREQACC(ACCOUNTS);
	if(WindowCount<2) select_backend(dbsort);
}
