/* reqapp.c - Approve Requisitions Vendor / Item / Item Number */
#ifndef WIN32
#define __NAM__ "reqapp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqapp.exe"
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

#define DIAGNOSTICS

static char *module="REQMGT",**selecttype=NULL,*reportname=NULL;
static char *select_expression=NULL,*locationid=NULL,*requisitionid=NULL;
static short reqitm=(-1),reqmst=(-1),vennum=(-1),reqcat=(-1),reqloc=(-1);
static short reqacc=(-1),eyrnum=(-1),byrnum=(-1);
static int select_type=0;
static APPlib *approvals=NULL;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static void save_record(RDArsrc *,short,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
DBsort *dbsort=NULL;
RDAfinmgt *fmgt=NULL;
RDArsrc *mainrsrc=NULL;
static void selectm(void *,DBsort *);
static int CHANGE_COUNT=0;

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static short testreqitems(PassableStruct *);

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
					str_length=(temp!=NULL ? RDAstrlen(temp):0);
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
					if(!isEMPTY(temp) && v->dlen<length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<v->dlen && !isEMPTY(temp))
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
	AddFileFields(list,reqloc);
	AddFileFields(list,reqmst);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"REQLOC",NULL,dumb);
	addDFincvir(tmprsrc,module,"REQMST",NULL,dumb);
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
void select_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE,ef1=FALSE,pmonth=0;
	char deleteflag=FALSE,*curdate=NULL,*curtime=NULL,*dedid=NULL;
	char *reqid=NULL,*reqid1=NULL,*locid=NULL,*locid1=NULL,dflag=FALSE;
	int x,numfields=0,appstat=0,itemnum=0,procyrid=0;
	double amt=0.0;
	NRDfield *fields=NULL,*field=NULL;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;
	short approve_status=FALSE;
	REQACC *ACCOUNTS=NULL;
	PassableStruct *p=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Approve Requisitions...",NULL);
		addDFincvir(tmprsrc,module,"REQMST",NULL,reqmst);
		addDFincvir(tmprsrc,module,"REQITM",NULL,reqitm);
		addDFincvir(tmprsrc,module,"REQACC",NULL,reqitm);
		addDFincvir(tmprsrc,"FINMGT","FINEYR",NULL,eyrnum);
		addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		numfields=NUMFLDS(reqmst);
		fields=FLDPOINTER(reqmst);
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				curdate=GETCURRENTDATE10();
				curtime=GETCURRENTTIME();
				LOCNRDFILE(reqmst);
				FINDFLDGETSTRING(dbsort->fileno,"LOCATION IDENTIFICATION",
					&locid);
				FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid);
				FINDFLDGETSTRING(dbsort->fileno,"REQUISITION IDENTIFICATION",
					&reqid);
				FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid);
				if(!ADVEQLNRDsec(reqmst,1,ReqChgSubData,p)) prev=RDATDataNEW(reqmst);
					else prev=NULL;
				FINDFLDGETSHORT(dbsort->fileno,"APPROVAL STATUS",&approve_status);
				for(x=0,field=fields;x<numfields;++x,++field)
				{
					if(!RDAstrcmp(field->name,"APPROVED BY"))
					{
						if(approve_status)
						{
							FINDFLDSETSTRING(reqmst,"APPROVED BY",USERLOGIN);
						} else FINDFLDSETSTRING(reqmst,"APPROVED BY",NULL);
					} else if(!RDAstrcmp(field->name,"APPROVED"))
					{
						if(approve_status)
						{
							FINDFLDSETCHAR(reqmst,"APPROVED",TRUE);
						} else FINDFLDSETCHAR(reqmst,"APPROVED",FALSE);
					} else if(!RDAstrcmp(field->name,"APPROVED DATE"))
					{
						if(approve_status)
						{
							FINDFLDSETSTRING(reqmst,"APPROVED DATE",curdate);
						} else FINDFLDSETSTRING(reqmst,"APPROVED DATE",NULL);
					} else if(!RDAstrcmp(field->name,"APPROVED TIME"))
					{
						if(approve_status)
						{
							FINDFLDSETSTRING(reqmst,"APPROVED TIME",curtime);
						} else FINDFLDSETSTRING(reqmst,"APPROVED TIME",NULL);
					} else {
						COPYFIELD(dbsort->fileno,reqmst,field->name);
					} 
				}
				if(approve_status==2)
				{
					FINDFLDSETCHAR(reqmst,"SUBMITTED",FALSE);
				}
				ADVWRTTRANSsec(reqmst,0,NULL,prev,ReqChgSubData,p);
/* if REQMST is Rejected Backout the Encumbrances after this */
				FINDFLDGETINT(reqmst,"APPROVAL STATUS",&appstat);
				if(appstat==2) /* rejected requisitions */
				{
					ZERNRD(reqitm);
					FINDFLDSETSTRING(reqitm,"LOCATION IDENTIFICATION",locid);
					FINDFLDSETSTRING(reqitm,"REQUISITION IDENTIFICATION",
						reqid);
					ef1=ADVGTENRDsec(reqitm,1,ReqChgSubData,p);
					while(!ef1)
					{
						FINDFLDGETSTRING(reqitm,"LOCATION IDENTIFICATION",
							&locid1);
						FINDFLDGETSTRING(reqitm,"REQUISITION IDENTIFICATION",
							&reqid1);
						if(RDAstrcmp(locid,locid1) || RDAstrcmp(reqid,reqid1)) 
							break;
						FINDFLDGETCHAR(reqitm,"DELETEFLAG",&dflag);
						if(!dflag)
						{
							FINDFLDGETINT(reqitm,"ITEM NUMBER",&itemnum);
							ACCOUNTS=ADVGetREQACC(reqacc,locid1,reqid1,itemnum,ReqChgSubData,p);
							if(ACCOUNTS!=NULL)
							{
								FINDFLDGETDOUBLE(reqitm,"AMOUNT",&amt);
								ComputeREQACC(ACCOUNTS,amt);
								FINDFLDGETINT(reqitm,"FISCAL YEAR",&procyrid);
								FINDFLDGETSHORT(reqitm,"PROCESSING MONTH",
									&pmonth);
								ADVREQACC_BACKOUT_UNDIST(ACCOUNTS,procyrid,
									pmonth,eyrnum,byrnum,
									ReqChgSubData,p);
								FreeREQACC(ACCOUNTS);
								ACCOUNTS=NULL;
							}
						}
						ef1=ADVNXTNRDsec(reqitm,1,ReqChgSubData,p);
					}
					if(locid1!=NULL) Rfree(locid1);
					if(reqid1!=NULL) Rfree(reqid1);
				}
/* if REQMST is Rejected Backout the Encumbrances before this */
				if(locid!=NULL) Rfree(locid);
				if(reqid!=NULL) Rfree(reqid);
				++CHANGE_COUNT;
				if(prev!=NULL) Rfree(prev);
				UNLNRDFILE(reqmst);
				update_diag=TRUE;
			}
			if(tmprsrc!=NULL)
				update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			if(curdate!=NULL) Rfree(curdate);
			if(curtime!=NULL) Rfree(curtime);
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(dedid!=NULL) Rfree(dedid);
		if(p!=NULL) Rfree(p);
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
	d=getDFvirtual(module,"APPROVE REQUISITION SORT SEQUENCE");
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
	numfields=NUMFLDS(reqmst);
	FIELDS=FLDPOINTER(reqmst);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!RDAstrcmp(field->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(field->name,"APPROVED COMMENTS"))
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
			} else if(!strncmp(field->name,"APPROVED",8))
			{
			} else {
				addDBfield(dbsrt,field->name,field->type,field->len);
			}
		}
	}
	key=KEYNUM(reqmst,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt,RDArsrc *mainrsrc)
{
	char deleteflag=FALSE,*temp=NULL;
	NRDfield *field,*FIELDS=NULL;
	int x=0,numfields=0,a=0;
	short appstat=FALSE;
	PassableStruct *p=NULL;

	ZERNRD(dbsrt->fileno);
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		p=Rmalloc(sizeof(PassableStruct));	
		p->r=mainrsrc;
		p->a=dbsrt;
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
	numfields=NUMFLDS(reqmst);
	FIELDS=FLDPOINTER(reqmst);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!RDAstrcmp(field->name,"DELETEFLAG"))
			{
/*
			} else if(!strncmp(field->name,"SUBMITTED",9))
			{
*/
			} else if(!RDAstrcmp(field->name,"APPROVAL STATUS"))
			{
				FINDRSCGETINT(mainrsrc,"APPROVAL STATUS",&a);
				appstat=(short)a;
				FINDFLDSETSHORT(dbsrt->fileno,"APPROVAL STATUS",appstat);
			} else if(!RDAstrcmp(field->name,"APPROVED COMMENTS"))
			{
				FINDRSCGETSTRING(mainrsrc,"APPROVED COMMENTS",&temp);
				FINDFLDSETSTRING(dbsrt->fileno,"APPROVED COMMENTS",temp);
			} else if(!strncmp(field->name,"APPROVED",8))
			{
			} else {
				COPYFIELD(reqmst,dbsrt->fileno,field->name);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
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
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without approving these Requisitions?",doexit,NULL,FALSE,blist);
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
static short testreqitems(PassableStruct *p)
{
	char *locid1=NULL,*locid2=NULL,*reqid1=NULL,*reqid2=NULL;
	char deleteflag=FALSE,held4rfp=FALSE,propaccpt=FALSE;
	short ef=FALSE,retval=FALSE;

	FINDFLDGETSTRING(reqmst,"LOCATION IDENTIFICATION",&locid1);
	FINDFLDGETSTRING(reqmst,"REQUISITION IDENTIFICATION",&reqid1);
	ZERNRD(reqitm);
	FINDFLDSETSTRING(reqitm,"LOCATION IDENTIFICATION",locid1);
	FINDFLDSETSTRING(reqitm,"REQUISITION IDENTIFICATION",reqid1);
	ef=ADVGTENRDsec(reqitm,1,ReqChgSubData,p);
	while(!ef)
	{
		FINDFLDGETCHAR(reqitm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		FINDFLDGETSTRING(reqitm,"LOCATION IDENTIFICATION",&locid2);
		FINDFLDGETSTRING(reqitm,"REQUISITION IDENTIFICATION",&reqid2);
		if(RDAstrcmp(locid1,locid2) || RDAstrcmp(reqid1,reqid2)) break;
		FINDFLDGETCHAR(reqitm,"HELD FOR RFP",&held4rfp);
		FINDFLDGETCHAR(reqitm,"PROPOSAL ACCEPTED",&propaccpt);
		if((held4rfp && !propaccpt))
		{
			retval=FALSE;
			break;
		} else {
			retval=TRUE;
		}
		}
		ef=ADVNXTNRDsec(reqitm,1,ReqChgSubData,p);
	}
	if(reqid1!=NULL) Rfree(reqid1);
	if(reqid2!=NULL) Rfree(reqid2);
	if(locid1!=NULL) Rfree(locid1);
	if(locid2!=NULL) Rfree(locid2);
	return(retval);
}
void addvouchers(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,dodiag=FALSE,testfile=FALSE;
	char deleteflag=FALSE,*sortname=NULL,submitted=FALSE,active=FALSE;
	char suv=TRUE,approved=FALSE;
	int x;
	long size_of_file=0;
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
	ReadRangeScreen(reqmst,rs);
	ReadRangeScreen(reqloc,rs);
	tmprsrc=diagscrn(reqmst,"DIAGNOSTIC SCREEN",module,
		"Selecting Requisitions",NULL);
	addDFincvir(tmprsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(tmprsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(tmprsrc,module,"REQITM",NULL,reqitm);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=tmprsrc;
	p->a=dbsort;
	ZERNRD(reqmst);
	ef=ADVFRSNRDsec(reqmst,1,ReqChgSubData,p);
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETCHAR(reqmst,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(reqmst,"SOURCE USER VERIFIED",&suv);
		FINDFLDGETCHAR(reqmst,"SUBMITTED",&submitted);
		FINDFLDGETCHAR(reqmst,"APPROVED",&approved);
		if(!deleteflag)
		{
		if(suv)
		{
		if(submitted)
		{
		if(!approved)
		{
			testfile=testfilerangersrc(reqmst,rs);
			if(!testfile)
			{
				COPYFIELD(reqmst,reqloc,"LOCATION IDENTIFICATION");
				if(ADVEQLNRDsec(reqloc,1,ReqChgSubData,p)) KEYNRD(reqloc,1);
				FINDFLDGETCHAR(reqloc,"DELETEFLAG",&deleteflag);
				FINDFLDGETCHAR(reqloc,"ACTIVE",&active);
				if(!deleteflag)
				{
				if(active)
				{
				if(testreqitems(p))
				{
					testfile=testfilerangersrc(reqloc,rs);
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
							addsortrecord(dbsort,mainrsrc);
							dodiag=TRUE;
						} else {
#ifdef DIAGNOSTICS
							if(diagapps)
							{
								prterr("Record Skipped Because of Evaluation of Virtual Field");
							}
#endif
						}
					} else {
#ifdef DIAGNOSTICS
						if(diagapps)
						{
							prterr("Record Skipped Because of Ranging on REQLOC data file.");
						}
#endif
					}
				} else {
#ifdef DIAGNOSTICS
					if(diagapps)
					{
						prterr("Record Skipped Because of a REQITM (Held for RFP and Proposal Not Accepted) or Method equals Zero in the REQRecord.");
					}
#endif
				}
				} else {
#ifdef DIAGNOSTICS
					if(diagapps)
					{
						prterr("Record Skipped Because of Non-Active REQLOC Record.");
					}
#endif
				}
				} else {
#ifdef DIAGNOSTICS
					if(diagapps)
					{
						prterr("Record Skipped Because of Deleted REQLOC Record.");
					}
#endif
				}
			} else {
#ifdef DIAGNOSTICS
				if(diagapps)
				{
					prterr("Record Skipped Because of Ranging on REQLOC data file.");
				}
#endif
			}
		} else {
#ifdef DIAGNOSTICS
			if(diagapps)
			{
				prterr("Record Skipped Because of Approved REQMST Record.");
			}
#endif
		}
		} else {
#ifdef DIAGNOSTICS
			if(diagapps)
			{
				prterr("Record Skipped Because of Not Submitted REQMST Record.");
			}
#endif
		}
		} else {
#ifdef DIAGNOSTICS
			if(diagapps)
			{
				prterr("Record Skipped Because of non Source User Verified REQMST Record.");
			}
#endif
		}
		} else {
#ifdef DIAGNOSTICS
			if(diagapps)
			{
				prterr("Record Skipped Because of Deleted REQMST Record.");
			}
#endif
		}
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		}
		ef=ADVNXTNRDsec(reqmst,1,ReqChgSubData,p);
	}
	if(p!=NULL) Rfree(p);
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
				brsrc=RDArsrcNEW(module,"APPROVE REQUISITIONS BROWSE");
				srsrc=RDArsrcNEW(module,"APPROVE REQUISITIONS SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"APPROVE REQUISITIONS DEFINE LIST");
				addDFincvir(drsrc,module,"REQLOC",NULL,reqloc);
				addDFincvir(drsrc,module,"REQMST",NULL,reqmst);
				addDFincvir(srsrc,module,"REQLOC",NULL,reqloc);
				addDFincvir(srsrc,module,"REQMST",NULL,reqmst);
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
		ERRORDIALOG("Approve Requisition Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
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
	CLSNRD(eyrnum);
	CLSNRD(byrnum);
	CLSNRD(vennum);
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
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"APPROVE REQUISITIONS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("REQAPP");
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if((reqitm=APPOPNNRD(module,"REQITM",TRUE,TRUE))==(-1)) return;
	if((reqmst=APPOPNNRD(module,"REQMST",TRUE,FALSE))==(-1)) return;
	if((reqloc=APPOPNNRD(module,"REQLOC",TRUE,FALSE))==(-1)) return;
	if((reqcat=APPOPNNRD("CATALOGUE","CATMST",TRUE,FALSE))==(-1)) return;
	if((reqacc=APPOPNNRD(module,"REQACC",TRUE,FALSE))==(-1)) return;
	if((eyrnum=APPOPNNRD("FINMGT","FINEYR",TRUE,FALSE))==(-1)) return;
	if((byrnum=APPOPNNRD("FINMGT","FINBYR",TRUE,FALSE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	makeselecttype();
	mainrsrc=RDArsrcNEW(module,"APPROVE REQUISITIONS");
	addDFincvir(mainrsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(mainrsrc,module,"REQLOC",NULL,reqloc);
	file2rangersrc(reqmst,mainrsrc);
	file2rangersrc(reqloc,mainrsrc);
	approvals=APPlibNEW();
	addAPPlib(approvals,"Awaiting Approval");
	addAPPlib(approvals,"Approved");
	addAPPlib(approvals,"Rejected");
	addlstrsrc(mainrsrc,"APPROVAL STATUS",&select_type,TRUE,NULL,
		approvals->numlibs,&approvals->libs,NULL);
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,
		&selecttype,NULL);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"APPROVED COMMENTS",0,NULL,TRUE);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,addvouchers,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	PassableStruct *p=NULL;

	if(reqloc!=(-1))
	{
		p=Rmalloc(sizeof(PassableStruct));
		p->r=mtnrsrc;
		p->a=dbsort;
		COPYFIELD(dbsort->fileno,reqloc,"LOCATION IDENTIFICATION");
		if(ADVEQLNRDsec(reqloc,1,ReqChgSubData,p)) KEYNRD(reqloc,1);
		filerecord2rsrc(reqloc,mtnrsrc);
		if(p!=NULL) Rfree(p);
	}
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *dedid=NULL,*locid=NULL;
	int a=0;
	short appstat=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"LOCATION IDENTIFICATION",&locid);
	FINDRSCGETSTRING(mtnrsrc,"REQUISITION IDENTIFICATION",&dedid);
	FINDFLDSETSTRING(dbsort->fileno,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(dbsort->fileno,"REQUISITION IDENTIFICATION",dedid);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
		FINDFLDGETSHORT(dbsort->fileno,"APPROVAL STATUS",&appstat);
		a=appstat;
		FINDRSCSETINT(mtnrsrc,"APPROVAL STATUS",a);
	}
	if(locid!=NULL) Rfree(locid);
	if(dedid!=NULL) Rfree(dedid);
	getsupportingfiles(mtnrsrc,dbsort);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void inititems(MakeBrowseList *blist)
{
	FINDFLDSETSTRING(reqitm,"LOCATION IDENTIFICATION",locationid);
	FINDFLDSETSTRING(reqitm,"REQUISITION IDENTIFICATION",requisitionid);
}
static short selectitems(MakeBrowseList *blist)
{
	char *locid=NULL,*reqid=NULL;

	FINDFLDGETSTRING(reqitm,"LOCATION IDENTIFICATION",&locid);
	FINDFLDGETSTRING(reqitm,"REQUISITION IDENTIFICATION",&reqid);
	if(RDAstrcmp(locid,locationid) || RDAstrcmp(reqid,requisitionid))
	{
		if(locid!=NULL) Rfree(locid);
		if(reqid!=NULL) Rfree(reqid);
		return(FALSE);
	}
	if(locid!=NULL) Rfree(locid);
	if(reqid!=NULL) Rfree(reqid);
	return(TRUE);
}
static void savereqmstnup(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	if(mtnrsrc!=NULL) killwindow(mtnrsrc);
	dbsort=blist->funcargs;
	brsrc=blist->mainrsrc;
	drsrc=blist->definelist;
	srsrc=blist->searchrsrc;
	save_record(mtnrsrc,FALSE,dbsort);
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
	if(locationid!=NULL) Rfree(locationid);
	if(requisitionid!=NULL) Rfree(requisitionid);
}
static void savereqmstup(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	dbsort=blist->funcargs;
	brsrc=blist->mainrsrc;
	drsrc=blist->definelist;
	srsrc=blist->searchrsrc;
	save_record(mtnrsrc,TRUE,dbsort);
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
	if(locationid!=NULL) Rfree(locationid);
	if(requisitionid!=NULL) Rfree(requisitionid);
}
static void doexititem(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

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
	if(locationid!=NULL) Rfree(locationid);
	if(requisitionid!=NULL) Rfree(requisitionid);
}
void selectm(void *targetkey,DBsort *t)
{
	int x,which_type=0;
	short appstat=FALSE,nofields=0;
	RDArsrc *mtnrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	NRDkey *key;
	NRDpart *part;
	MakeBrowseList *mbl_item=NULL;
	NRDfield *fields=NULL,*fldx=NULL;

	mtnrsrc=RDArsrcNEW(module,"APPROVE REQUISITIONS MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(mtnrsrc,module,"REQMST",NULL,reqmst);
	searchrsrc=RDArsrcNEW(module,"APPROVE REQUISITION ITEMS SEARCH BROWSE");
	addDFincvir(searchrsrc,module,"REQITM",NULL,reqitm);
	addDFincvir(searchrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(searchrsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(searchrsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum);
	definelist=RDArsrcNEW(module,"APPROVE REQUISITION ITEMS DEFINE LIST");
	addDFincvir(definelist,module,"REQITM",NULL,reqitm);
	addDFincvir(definelist,module,"REQLOC",NULL,reqloc);
	addDFincvir(definelist,module,"REQMST",NULL,reqmst);
	addDFincvir(definelist,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"APPROVAL STATUS"))
			{
				which_type=0;
				addlstrsrc(mtnrsrc,"APPROVAL STATUS",&which_type,TRUE,NULL,
					approvals->numlibs,&approvals->libs,NULL);
			} else {
				makefieldrsrc(mtnrsrc,fldx->name,fldx->type,
					fldx->len,FALSE);
			}
		}
	}
	nofields=0;
	FINDRSCSETEDITABLE(mtnrsrc,"APPROVED COMMENTS",TRUE);
	if(reqloc!=(-1)) file2rsrc(reqloc,mtnrsrc,FALSE);
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
		FINDFLDGETSHORT(t->fileno,"APPROVAL STATUS",&appstat);
		x=appstat;
		FINDRSCSETINT(mtnrsrc,"APPROVAL STATUS",x);
	}
	FINDFLDGETSTRING(t->fileno,"LOCATION IDENTIFICATION",&locationid);
	FINDFLDGETSTRING(t->fileno,"REQUISITION IDENTIFICATION",&requisitionid);
	getsupportingfiles(mtnrsrc,t);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mtnrsrc,"SAVE",TRUE,savereqmstup,mbl_item);
	addrfexrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,savereqmstnup,mbl_item);
	computeallSCRNvirtuals(mtnrsrc);
	ZERNRD(reqitm);
	COPYFIELD(t->fileno,reqitm,"LOCATION IDENTIFICATION");
	COPYFIELD(t->fileno,reqitm,"REQUISITION IDENTIFICATION");
	mbl_item=ADVbrowse(reqitm,1,mtnrsrc,searchrsrc,definelist,inititems,
		selectitems,doexititem,NULL,NULL,t,TRUE,NULL,0,0,0,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"SAVE",savereqmstup,mbl_item);
	FINDRSCSETFUNC(mtnrsrc,"SAVE NO UPDATE",savereqmstnup,mbl_item);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0,a=0;
	short appstat=0;
	RDArsrc *tmprsrc=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"APPROVAL STATUS",&a);
	appstat=(short)a;
	FINDFLDSETSHORT(dbsort->fileno,"APPROVAL STATUS",appstat);
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
}
