/* subcsub1.c - Substitution Managment's Contact Substitutes */
#include <cstdio>

#include <mix.hpp>
#include <ldval.hpp>
#include <fin.hpp>
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

#ifdef XXX
extern void quit_record(RDArsrc *);
#endif

extern void getsupportingrecords(RDArsrc *,short,short);
extern short empabtr,empabs;
extern short posmstr,poslmst,subwrk;
extern short subprsnnl,subprsnnl2,subjobmst,subposmstr;
extern short subctact,submst,subemp,subpos,subloc;
extern short subcls,subposlmst,subposper,subposgrs,subposdedc;
extern short subpossmst,subposngms;
extern MakeBrowseList *mbl_empabtr;
extern RDAfinmgt *fmgt;
extern int SUBCTACT_TRANSACTION_COUNT;
extern char *module;
extern MaintainMaster *MTNMASTER;
void select_subs(RDArsrc *);
static RDArsrc *empabtrrsrc;
static MaintainMaster *MTNMASTER1=NULL;
static APPlib *rsptypes=NULL;
static APPlib *subjobnolist=NULL;
static APPlib *makersplist(void);
static char *subpersonnel_identification=NULL,*class_identification=NULL;
static void setrsptype(RDArsrc *,DBsort *);
static void setsublist(RDArsrc *,DBsort *);
static void setsubjoblist(RDArsrc *,DBsort *);
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_subs;
static char *select_expression=NULL;
static short check_subid(RDArsrc *,DBsort *);
static void save_record(RDArsrc *,short,DBsort *);
static void save_check(RDArsrc *,short,DBsort *);
static void quit_record1(RDArsrc *,DBsort *);
static void save_record_up(RDArsrc *,DBsort *);
static void save_record_nup(RDArsrc *,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static void subsm(short,void *,DBsort *);
static void browse_subs(RDArsrc *,DBsort *);
static void changesubjobno(RDArsrc *,DBsort *,short);
static void changesubjobnocb(RDArsrc *,DBsort *);
static short check_subjobno(RDArsrc *,DBsort *);
static void changesubper(RDArsrc *,DBsort *);
static void doexit(MakeBrowseList *);
static void subselected(RDArsrc *,DBsort *);
static void SubSelSubData(char **,PassableStruct *);

void SubSelSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y=0;
	DBsort *dbsort=NULL;
	RDAvirtual *v=NULL;
	RDArsrc *r=NULL;

	r=p->r;
	dbsort=p->a;
	if(diageval)
	{
		prterr("DIAG SubSelSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
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
					temp=stralloc(field->data.string_value);
					str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
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
						temp=EVALUATEstr(v->expression,SubSelSubData,p);
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
					temp=stralloc(v->value.string_value);
					if(!isEMPTY(temp) && v->dlen>length)
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
						*v->value.short_value=EVALUATEsht(v->expression,SubSelSubData,p);
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
						*v->value.integer_value=EVALUATEint(v->expression,SubSelSubData,p);
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
						*v->value.float_value=EVALUATEdbl(v->expression,SubSelSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,SubSelSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,SubSelSubData,p);
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
		prterr("DIAG SubSelSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
static APPlib *makersplist(void)
{
	APPlib *rsptypes=NULL;

	rsptypes=APPlibNEW();
	addAPPlib(rsptypes,"Accepted");
	addAPPlib(rsptypes,"Refused");
	addAPPlib(rsptypes,"Other");
	return(rsptypes);
}
static short check_subunits(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *warnmessage=NULL;
	double sub_units=0.0,sub_units_unass=0.0;

	FINDRSCGETDOUBLE(mtnrsrc,"UNITS",&sub_units);
	FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units_unass);
	if(sub_units>sub_units_unass)
	{
		warnmessage=Rmalloc(230);
		sprintf(warnmessage,
			"The Substitute Units of [%f] is greater than total Substitute Units Unassigned of [%f]. Please select a valid number of Substitution Units to save this record.",sub_units,sub_units_unass);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID NUMBER OF UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return(FALSE);
	} else if(sub_units<=0.0)
	{
		warnmessage=Rmalloc(230);
		sprintf(warnmessage,
			"The Substitute Units of [%f] is an invalid number of units. Please select a valid number of Substitution Units to save this record.",sub_units);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID NUMBER OF UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return(FALSE);
	}
	return(TRUE);
}
static short check_subid(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *subperid1=NULL,*subperid2=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	short ef=0;

	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&subperid1);
	ef=FRSNRD(dbsort->fileno,1);
	while(!ef)
	{
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&subperid2);
		if(!RDAstrcmp(subperid2,subperid1)) break;
		ef=NXTNRD(dbsort->fileno,1);
	}
	if(RDAstrcmp(subperid2,subperid1))
	{
		KEYNRD(dbsort->fileno,1);
		warnmessage=Rmalloc(RDAstrlen(subperid1)+230);
		sprintf(warnmessage,
			"The Personnel Identification of [%s] is an invalid value.  It does not exist as a validated Substitute.  Please select a valid Personnel Identification to save this record.",
			(subperid1==NULL?"":subperid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(subperid1!=NULL) Rfree(subperid1);
		if(subperid2!=NULL) Rfree(subperid2);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(subperid1)+215);
			sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value in the sort file.  Please select a valid Personnel Identification to save this record."
				,(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(subperid1!=NULL) Rfree(subperid1);
			if(subperid2!=NULL) Rfree(subperid2);
			return(FALSE);
		}
	}
	if(subperid1!=NULL) Rfree(subperid1);
	if(subperid2!=NULL) Rfree(subperid2);
	return(TRUE);
}
static void changesubper(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	short ef=0;
	short keyn=0,keynumber=0;
	int selsubjobno=0,subjobno1=(-1);
	char active=TRUE,deleteflag=FALSE;
	char *e=NULL;
	char *subperid1=NULL,*subperid2=NULL,*subjobnostr=NULL,*classid2=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&subperid1)) return;
	if(RDAstrcmp(subperid1,subpersonnel_identification) || subpersonnel_identification==NULL)
	{
		if(subjobnolist!=NULL) freeapplib(subjobnolist);
		subjobnolist=APPlibNEW();
		FINDRSCGETINT(mtnrsrc,"JOB NUMBERS",&selsubjobno);
		if((keyn=KEYNUMBER(subjobmst,"JOBMSTR KEY"))!=(-1))
		{
			keynumber=keyn;
		} else {
			keynumber=1;
		}
		ZERNRD(subjobmst);
		FINDFLDSETSTRING(subjobmst,"PERSONNEL IDENTIFICATION",subperid1);
		ef=GTENRDsec(subjobmst,keynumber);
		e=stralloc("Updating Job Numbers List");
		tmprsrc=diagscrn(subjobmst,"DIAGNOSTIC SCREEN",module,e,NULL);
		if(e!=NULL) Rfree(e);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		while(!ef)
		{
			listupdated=FALSE;
			FINDFLDGETSTRING(subjobmst,"PERSONNEL IDENTIFICATION",&subperid2);
			if(RDAstrcmp(subperid2,subperid1)) break;
			FINDFLDGETINT(subjobmst,"JOB NUMBER",&subjobno1);
			FINDFLDGETCHAR(subjobmst,"ACTIVE",&active);
			FINDFLDGETCHAR(subjobmst,"DELETEFLAG",&deleteflag);
			FINDFLDGETSTRING(subjobmst,"POSITION IDENTIFICATION",
				&classid2);
			if(active && !deleteflag && !RDAstrcmp(classid2,class_identification))
			{
				subjobnostr=Rrealloc(subjobnostr,9);
				sprintf(subjobnostr,"%8d",subjobno1);
				addAPPlib(subjobnolist,subjobnostr);
				listupdated=TRUE;
			} else {
				listupdated=FALSE;
			}
			ef=NXTNRDsec(subjobmst,keynumber);
			if(tmprsrc!=NULL)
			update_diagnostic(tmprsrc,listupdated);
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
		if(subjobnolist->numlibs<1) addAPPlib(subjobnolist,"No Jobs Available");
	}
	if(selsubjobno>=subjobnolist->numlibs) selsubjobno=0;
	FINDRSCLISTAPPlib(mtnrsrc,"JOB NUMBERS",selsubjobno,subjobnolist);
	updatersrc(mtnrsrc,"JOB NUMBERS");
	if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
	subpersonnel_identification=stralloc(subperid1);
	changesubjobno(mtnrsrc,dbsort,FALSE);
	if(subperid1!=NULL) Rfree(subperid1);
	if(subperid2!=NULL) Rfree(subperid2);
}
static void changesubjobnocb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	changesubjobno(mtnrsrc,dbsort,TRUE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void changesubjobno(RDArsrc *mtnrsrc,DBsort *dbsort,short update)
{
	char *subperid1=NULL,*subjobnostr=NULL;
	int subjobno1=(-1),selsubjobno=0;

	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&subperid1);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBERS",&selsubjobno);
	if((RDAstrcmp(subjobnolist->libs[selsubjobno],"No Jobs Available")) &&
		(RDAstrcmp(subjobnolist->libs[selsubjobno],"Not Applicable")))
	{
		subjobnostr=stralloc(subjobnolist->libs[selsubjobno]);
	}
	subjobno1=Ratoi(subjobnostr);
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",subjobno1);
	if(update)
	{
		FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",subperid1);
		getsupportingfiles(mtnrsrc,dbsort);
	}
	if(subperid1!=NULL) Rfree(subperid1);
	return;
}
static short check_subjobno(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *subperid1=NULL,*warnmessage=NULL;
	int subjobno1=(-1);
	char delflag=FALSE;

	FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&subjobno1);
	FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&subperid1);
	if(subjobmst!=(-1))
	{
		ZERNRD(subjobmst);
		FINDFLDSETSTRING(subjobmst,"PERSONNEL IDENTIFICATION",subperid1);
		FINDFLDSETINT(subjobmst,"JOB NUMBER",subjobno1);
		if(EQLNRDsec(subjobmst,1))
		{
			KEYNRD(subjobmst,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+150);
			sprintf(warnmessage,
				"The Job Number of [%8d] is an invalid value for the Personnel Identification of [%s].  Please select a valid Job Number to save this record.",subjobno1,
				(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingfiles(mtnrsrc,dbsort);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(subjobmst,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+152);
				sprintf(warnmessage,
					"The Job Number of [%8d] is a deleted value for the Personnel Identification of [%s].  Please select a valid Job Number to save this record.",subjobno1,
					(subperid1==NULL?"":subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingfiles(mtnrsrc,dbsort);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	if(subperid1!=NULL) Rfree(subperid1);
	return(TRUE);
}
static void setsubjoblist(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *subjobnostr=NULL,*subperid1=NULL;
	int x=0,subjobno1=(-1);

	FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&subjobno1);
	FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&subperid1);
	subjobnostr=Rmalloc(9);
	sprintf(subjobnostr,"%8d",subjobno1);
	x=FINDAPPLIBELEMENT(subjobnolist,subjobnostr);
	if(x==(-1)) x=0;
	FINDRSCSETINT(mtnrsrc,"JOB NUMBERS",x);
	updatersrc(mtnrsrc,"JOB NUMBERS");
	if(RDAstrcmp(subjobnolist->libs[x],"No Jobs Available"))
	{
		subjobno1=Ratoi(subjobnolist->libs[x]);
	} else {
		subjobno1=0;
	}
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",subjobno1);
	if(subperid1!=NULL) Rfree(subperid1);
	if(subjobnostr!=NULL) Rfree(subjobnostr);
}
static void setrsptype(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	short selrsptype=0;
	int x=0;
	char *message=NULL;

	FINDFLDGETSHORT(dbsort->fileno,"RESPONSE TYPE",&selrsptype);
	if((-1)<selrsptype && selrsptype<rsptypes->numlibs)
	{
		x=selrsptype;
	} else {
		message=Rmalloc(110+RDAstrlen(rsptypes->libs[0]));
		sprintf(message,"The record selected contains an invalid responce type.  Therefore, the RESPONSE TYPES list will default to [%s].",
			(rsptypes->libs[0]==NULL?"":rsptypes->libs[0]));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID RESPONSE TYPE",message,NULL,NULL,TRUE,NULL);
		if(message!=NULL) Rfree(message);
		x=0;
	}
	FINDRSCSETINT(mtnrsrc,"RESPONSE TYPES",x);
}
static void setsublist(RDArsrc *mtnrsrc,DBsort *dbsort)
{
        changesubper(mtnrsrc,dbsort);
        setsubjoblist(mtnrsrc,dbsort);
	setrsptype(mtnrsrc,dbsort);
}
static void makeDBsortfile(DBsort *dbsort)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field,*FIELDS=NULL;
	int x=0,numfields=0;
	short keyno=(-1);

	addDBfield(dbsort,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsort,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"CONTACT SUBSTITUTES SORT SEQUENCE");
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
	numfields=NUMFLDS(subctact);
	FIELDS=FLDPOINTER(subctact);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!RDAstrcmp(field->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(field->name,"TRANSACTION NUMBER"))
			{
			} else {

				addDBfield(dbsort,field->name,field->type,field->len);
			}
		}
		addDBfield(dbsort,"UNITS",SDECIMALV,8);
		addDBfield(dbsort,"EMPLOYEE TRANSACTION NUMBER",LONGV,6);
		addDBfield(dbsort,"EMPLOYEE JOB NUMBER",LONGV,6);
		addDBfield(dbsort,"JOB NUMBER",LONGV,6);
		addDBfield(dbsort,"STARTING DATE",DATES,8);
		addDBfield(dbsort,"ENDING DATE",DATES,8);
	}
	key=KEYNUM(subctact,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if(RDAstrcmp(part->name,"TRANSACTION NUMBER"))
			{
				addDBkeypart(dbsort,keyno,part->name);
			}
		}
	}
}
static void addsortrecord(PassableStruct *p,char *subperid,int subjobno,
		char *empperid,char *locid,char *posid,int transno,int jobno,
		char *startdate,char *enddate,double units)
{
	char deleteflag=FALSE;
	char *date=NULL,*timex=NULL;
	DBsort *dbsort=NULL;

	dbsort=p->a;
	FINDFLDSETCHAR(dbsort->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,SubSelSubData,p);
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
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",subperid);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",subjobno);
	FINDFLDSETSTRING(dbsort->fileno,"EMPLOYEE IDENTIFICATION",empperid);
	FINDFLDSETINT(dbsort->fileno,"EMPLOYEE TRANSACTION NUMBER",transno);
	FINDFLDSETINT(dbsort->fileno,"EMPLOYEE JOB NUMBER",jobno);
	FINDFLDSETSTRING(dbsort->fileno,"POSITION IDENTIFICATION",posid);
	FINDFLDSETSTRING(dbsort->fileno,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(dbsort->fileno,"STARTING DATE",startdate);
	FINDFLDSETSTRING(dbsort->fileno,"ENDING DATE",enddate);
	FINDFLDSETDOUBLE(dbsort->fileno,"UNITS",units);
	FINDFLDSETSTRING(dbsort->fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETSHORT(dbsort->fileno,"RESPONSE TYPE",1); /* Refused */
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDFLDSETSTRING(dbsort->fileno,"CONTACT DATE",date);
	FINDFLDSETSTRING(dbsort->fileno,"CONTACT TIME",timex);
	FINDFLDSETSTRING(dbsort->fileno,"ENTRY DATE",date);
	FINDFLDSETSTRING(dbsort->fileno,"ENTRY TIME",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	WRTNRD(dbsort->fileno,0,NULL);
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	subsm(0,targetkey,dbsort);
}
static void doexit(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	DBsort *dbsort=NULL;

	if(blist!=NULL)
	{
		dbsort=blist->funcargs;
		brsrc=blist->mainrsrc;
		srsrc=blist->searchrsrc;
		drsrc=blist->definelist;
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
		if(mbl_subs!=NULL) mbl_subs=NULL;
/* You may want to get both MTNMASTER's at the beginning and fail their if one isn't present.... Looking at it, why does it matter which one you start with when you want to close and free the sort file????? */
		if(MTNMASTER1!=NULL) 
		{
			if(MTNMASTER1->start_with==1)
			{
				if(dbsort!=NULL)
				{
					CLSDBsort(dbsort);
					FreeDBsort(dbsort);
					dbsort=NULL;
				}
				FreeMaintainMaster(MTNMASTER1);
				MTNMASTER1=NULL;
			}
		}
	}
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
void select_subs(RDArsrc *eabsmtnrsrc)
{
	RDArsrc *tmprsrc=NULL;
	DBsort *dbsort=NULL;
	RangeScreen *rs=NULL;
	short ef=FALSE,ef2=FALSE,testfile=FALSE;
	char *perid=NULL,*subperid=NULL,*subperid2=NULL,*subposid=NULL;
	char *posid=NULL,*locid=NULL,do_diag=FALSE;
	char blocked=FALSE,rest_emp=FALSE,rest_loc=FALSE,rest_pos=FALSE;
	int subjobno=0,transno=(-1),jobno=(-1);
	char deleteflag=FALSE,*sortname=NULL,*classid2=NULL;
	char *startdate=NULL,*enddate=NULL;
	double sub_units_unass=0.0;
	long size_of_file=0;
	PassableStruct *p=NULL;

	empabtrrsrc=eabsmtnrsrc;
	FINDRSCGETSTRING(empabtrrsrc,"[POSMSTR][POSITION IDENTIFICATION]",&class_identification);
	FINDRSCGETSTRING(empabtrrsrc,"SUBSTITUTE EXPRESSION",&select_expression);
	FINDFLDGETINT(empabtr,"TRANSACTION NUMBER",&transno);
	FINDFLDGETINT(empabtr,"JOB NUMBER",&jobno);
	FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&perid);
	FINDFLDGETSTRING(empabs,"STARTING DATE",&startdate);
	FINDFLDGETSTRING(empabs,"ENDING DATE",&enddate);
	FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units_unass);
	FINDFLDGETSTRING(posmstr,"POSITION IDENTIFICATION",&posid);
	FINDFLDGETSTRING(poslmst,"LOCATION IDENTIFICATION",&locid);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	rs=RangeScreenNew(empabtrrsrc);
	ReadRangeScreen(submst,rs);
	ReadRangeScreen(subprsnnl,rs);
	ReadRangeScreen(subjobmst,rs);
	ReadRangeScreen(subposlmst,rs);
	ReadRangeScreen(subposper,rs);
	ReadRangeScreen(subcls,rs);
	ReadRangeScreen(subposgrs,rs);
	ReadRangeScreen(subposdedc,rs);
	ReadRangeScreen(subpossmst,rs);
	ReadRangeScreen(subposngms,rs);
	tmprsrc=diagscrn(subjobmst,"DIAGNOSTIC SCREEN",module,
		"Selecting Substitutes",NULL);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=tmprsrc;
	p->a=dbsort;
	addDFincvir(tmprsrc,module,"SUBMASTR",NULL,submst);
	addDFincvir(tmprsrc,module,"SUBEMP",NULL,subemp);
	addDFincvir(tmprsrc,module,"SUBLOC",NULL,subloc);
	addDFincvir(tmprsrc,module,"SUBPOS",NULL,subpos);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,subprsnnl);
	addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,subjobmst);
	addDFincvir(tmprsrc,"POSTRK","POSLMST",NULL,subposlmst);
	addDFincvir(tmprsrc,"POSTRK","POSCLS",NULL,subcls);
	addDFincvir(tmprsrc,"POSTRK","POSGRS",NULL,subposgrs);
	addDFincvir(tmprsrc,"POSTRK","POSPER",NULL,subposper);
	addDFincvir(tmprsrc,"POSTRK","POSDEDC",NULL,subposdedc);
	addDFincvir(tmprsrc,"POSTRK","POSSMST",NULL,subpossmst);
	addDFincvir(tmprsrc,"POSTRK","POSNGMS",NULL,subposngms);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE)) 
			ForceWindowUpdate(tmprsrc);
	}
	ZERNRD(submst);
	for(ef=FRSNRDsec(submst,1);!ef;ef=NXTNRDsec(submst,1))
	{
		do_diag=FALSE;
		FINDFLDGETSTRING(submst,"PERSONNEL IDENTIFICATION",&subperid);
		FINDFLDGETCHAR(submst,"RESTRICTED EMPLOYEE",&rest_emp);
		FINDFLDGETCHAR(submst,"RESTRICTED POSITION",&rest_pos);
		FINDFLDGETCHAR(submst,"RESTRICTED LOCATION",&rest_loc);
		FINDFLDGETCHAR(submst,"DELETEFLAG",&deleteflag);
		unpad(subperid);
		unpad(perid);
		testfile=testfilerangersrc(submst,rs);
		if(!deleteflag && RDAstrcmp(perid,subperid) && !testfile)
		{
			FINDFLDSETSTRING(subprsnnl,"PERSONNEL IDENTIFICATION",subperid);
			if(!EQLNRDsec(subprsnnl,1))
			{
			FINDFLDGETCHAR(subprsnnl,"DELETEFLAG",&deleteflag);
			testfile=testfilerangersrc(subprsnnl,rs);
			if(!deleteflag && !testfile)
			{
			ZERNRD(subjobmst);
			FINDFLDSETSTRING(subjobmst,"PERSONNEL IDENTIFICATION",subperid);
			for(ef2=GTENRDsec(subjobmst,1);!ef2;ef2=NXTNRDsec(subjobmst,1))
			{
				FINDFLDGETSTRING(subjobmst,"PERSONNEL IDENTIFICATION",&subperid2);
				if(RDAstrcmp(subperid2,subperid)) break;
				FINDFLDGETINT(subjobmst,"JOB NUMBER",&subjobno);
				FINDFLDGETCHAR(subjobmst,"DELETEFLAG",&deleteflag);
				testfile=testfilerangersrc(subjobmst,rs);
				if(!deleteflag && !testfile) 
				{
				FINDFLDGETSTRING(subjobmst,"POSITION IDENTIFICATION",&subposid);
				if(!RDAstrcmp(subposid,class_identification))
				{
					FINDFLDSETSTRING(subposmstr,"POSITION IDENTIFICATION",subposid);
					if(!EQLNRDsec(subposmstr,1))
					{
						FINDFLDGETCHAR(subposmstr,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							FINDFLDGETSTRING(subposmstr,"CLASS IDENTIFICATION",&classid2);
							FINDFLDSETSTRING(subcls,"CLASS IDENTIFICATION",classid2);
							if(!EQLNRDsec(subcls,1))
							{
							FINDFLDGETCHAR(subcls,"DELETEFLAG",&deleteflag);
							testfile=testfilerangersrc(subcls,rs);
							if(!deleteflag && !testfile)
							{
							COPYFIELD(subposmstr,subposlmst,"LOCATION IDENTIFICATION");
							if(!EQLNRDsec(subposlmst,1))
							{
							FINDFLDGETCHAR(subposlmst,"DELETEFLAG",&deleteflag);
							testfile=testfilerangersrc(subposlmst,rs);
							if(!deleteflag && !testfile)
							{
							COPYFIELD(subjobmst,subposper,"POSITION IDENTIFICATION");
							COPYFIELD(subjobmst,subposper,"PERSONNEL IDENTIFICATION");
							COPYFIELD(subjobmst,subposper,"JOB NUMBER");
							if(!EQLNRDsec(subposper,1))
							{
							FINDFLDGETCHAR(subposper,"DELETEFLAG",&deleteflag);
							testfile=testfilerangersrc(subposper,rs);
							if(!deleteflag && !testfile)
							{
							COPYFIELD(subposmstr,subposgrs,"GROSS IDENTIFICATION");
							if(!EQLNRDsec(subposgrs,1))
							{
							FINDFLDGETCHAR(subposgrs,"DELETEFLAG",&deleteflag);
							testfile=testfilerangersrc(subposgrs,rs);
							if(!deleteflag && !testfile)
							{
							COPYFIELD(subposmstr,subposdedc,"DEDUCTION CLASS IDENTIFICATION");
							if(!EQLNRDsec(subposdedc,1))
							{
							FINDFLDGETCHAR(subposdedc,"DELETEFLAG",&deleteflag);
							testfile=testfilerangersrc(subposdedc,rs);
							if(!deleteflag && !testfile)
							{
							COPYFIELD(subposmstr,subpossmst,"SERVICE AREA IDENTIFICATION");
							if(!EQLNRDsec(subpossmst,1))
							{
								FINDFLDGETCHAR(subpossmst,"DELETEFLAG",&deleteflag);
								testfile=testfilerangersrc(subpossmst,rs);
								if(deleteflag || testfile)
								{
									if(diagapps)
									{
									prterr("Warning Ranging on Service Area Identification Master failed or a deleted Service Area Identification Master Record.  Record not Skipped but Substitute Units calculated from the [EMPABS][EMPADEF] JOB UNITS to SUBSTITUTE UNITS Expression Evaluation Questionable if it contains fields from [POSTRK][POSSMST] data file.");SEENRDRECORD(subpossmst);
									}
								}
							} else {
								KEYNRD(subpossmst,1);
								if(diagapps)
								{
								prterr("Warning no Service Area Identification Master found.  Record not Skipped but Substitute Units calculated from the [EMPABS][EMPADEF] JOB UNITS to SUBSTITUTE UNITS Expression Evaluation Questionable if it contains fields from [POSTRK][POSSMST] data file.");SEENRDRECORD(subpossmst);
								}
							}
							COPYFIELD(subposmstr,subposngms,"NEGOTIATING GROUP IDENTIFICATION");
							if(!EQLNRDsec(subposngms,1)) 
							{
								FINDFLDGETCHAR(subposngms,"DELETEFLAG",&deleteflag);
								testfile=testfilerangersrc(subposngms,rs);
								if(deleteflag || testfile)
								{
									if(diagapps)
									{
									prterr("Warning Ranging on Negotiating Group Identification Master failed or a deleted Negotiating Group Identification Master Record.  Record not Skipped but Substitute Units calculated from the [EMPABS][EMPADEF] JOB UNITS to SUBSTITUTE UNITS Expression Evaluation Questionable if it contains fields from [POSTRK][POSNGMS] data file.");SEENRDRECORD(subposngms);
									}
								}
								
							} else {
								KEYNRD(subposngms,1);
								if(diagapps)
								{
								prterr("Warning No Negotiating Group Identification Master found.  Record not Skipped but Substitute Units calculated from the [EMPABS][EMPADEF] JOB UNITS to SUBSTITUTE UNITS Expression Evaluation Questionable if it contains fields from [POSTRK][POSNGMS] data file.");SEENRDRECORD(subposngms);
								}
							}
								FINDFLDSETSTRING(subemp,"PERSONNEL IDENTIFICATION",subperid);
								FINDFLDSETSTRING(subemp,"EMPLOYEE IDENTIFICATION",perid);
								if(!EQLNRDsec(subemp,1))
								{
									FINDFLDGETCHAR(subemp,"DELETEFLAG",&deleteflag);
									FINDFLDGETCHAR(subemp,"EMPLOYEE BLOCK",&blocked);
									if(!deleteflag && blocked)
									{
										if(diagapps)
										{
										prterr("Skipped because the SUBSTITUTE EMPLOYEE record is BLOCKED for this EMPLOYEE PERSONNEL IDENTIFICATION.");KEYNRD(subemp,1);SEENRDRECORD(subemp);
										}
										continue;
									} else if(deleteflag && rest_emp)
									{
										if(diagapps)
										{
										prterr("Skipped because RESTRICTED EMPLOYEE is true and the SUBSTITUTE EMPLOYEE record for this EMPLOYEE PERSONNEL IDENTIFICATION is in a state of deletion.");KEYNRD(subemp,1);SEENRDRECORD(subemp);
										}
										continue;
									}
								} else {
									if(rest_emp)
									{
										if(diagapps)
										{
										prterr("Skipped because RESTRICTED EMPLOYEE is true and there is no SUBSTITUTE EMPLOYEE record for this EMPLOYEE PERSONNEL IDENTIFICATION.");KEYNRD(subemp,1);SEENRDRECORD(subemp);
										}
										continue;
									} 
								}
								FINDFLDSETSTRING(subloc,"PERSONNEL IDENTIFICATION",subperid);
								FINDFLDSETSTRING(subloc,"LOCATION IDENTIFICATION",locid);
								if(!EQLNRDsec(subloc,1))
								{
									FINDFLDGETCHAR(subloc,"DELETEFLAG",&deleteflag);
									FINDFLDGETCHAR(subloc,"LOCATION BLOCK",&blocked);
									if(!deleteflag && blocked)
									{
										prterr("Skipped because the SUBSTITUTE LOCATION record is BLOCKED for this LOCATION IDENTIFICATION.");KEYNRD(subloc,1);SEENRDRECORD(subloc);
										continue;
									} else if(deleteflag && rest_loc)
									{
										if(diagapps)
										{
										prterr("Skipped because RESTRICTED LOCATION is true and the SUBSTITUTE LOCATION record for this LOCATION IDENTIFICATION is in a state of deletion.");KEYNRD(subloc,1);SEENRDRECORD(subloc);
										}
										continue;
									}
								} else {
									if(rest_loc)
									{
										if(diagapps)
										{
										prterr("Skipped because RESTRICTED LOCATION is true and there is no SUBSTITUTE LOCATION record for this LOCATION IDENTIFICATION.");KEYNRD(subloc,1);SEENRDRECORD(subloc);
										}
										continue;
									} 
								}
								FINDFLDSETSTRING(subpos,"PERSONNEL IDENTIFICATION",subperid);
								FINDFLDSETSTRING(subpos,"POSITION IDENTIFICATION",posid);
								if(!EQLNRDsec(subpos,1))
								{
									FINDFLDGETCHAR(subpos,"DELETEFLAG",&deleteflag);
									FINDFLDGETCHAR(subpos,"POSITION BLOCK",&blocked);
									if(!deleteflag && blocked)
									{
										if(diagapps)
										{
										prterr("Skipped because the SUBSTITUTE POSITION record is BLOCKED for this POSITION IDENTIFICATION.");KEYNRD(subpos,1);SEENRDRECORD(subpos);
										}
										continue;
									} else if(deleteflag && rest_pos)
									{
										if(diagapps)
										{
										prterr("Skipped because RESTRICTED POSITION is true and the SUBSTITUTE POSITION record for this POSITION IDENTIFICATION is in a state of deletion.");KEYNRD(subpos,1);SEENRDRECORD(subpos);
										}
										continue;
									}
								} else {
									if(rest_pos)
									{
										if(diagapps)
										{
										prterr("Skipped because RESTRICTED POSITION is true and there is no SUBSTITUTE POSITION record for this POSITION IDENTIFICATION.");KEYNRD(subpos,1);SEENRDRECORD(subpos);
										}
										continue;
									} 
								}
/* might need to check all subwkmas records to check if new starting date is */
/* between the range of one of the non-deleted subwkmas transactions starting */ 
/* date and ending date */
/*
								addsortrecord(p2->pass,
*/
								addsortrecord(p,
									subperid,subjobno,perid,locid,
									posid,transno,jobno,startdate,
									enddate,sub_units_unass);
								do_diag=TRUE;
							} else {
								if(diagapps)
								{
								prterr("Skipped by Ranging on Deduction Class Identification Master or a deleted Deduction Class Identification Master Record.");KEYNRD(subposdedc,1);SEENRDRECORD(subposdedc);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by not finding Deduction Class Identification Record.");KEYNRD(subposdedc,1);SEENRDRECORD(subposdedc);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by Ranging on Gross Identification Master or a deleted Gross Identification Master Record.");KEYNRD(subposgrs,1);SEENRDRECORD(subposgrs);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by not finding Gross Identification Record.");KEYNRD(subposgrs,1);SEENRDRECORD(subposgrs);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by Ranging on Position Personnel Master or a deleted Position Personnel Master Record.");KEYNRD(subposper,1);SEENRDRECORD(subposper);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by not finding Position Personnel Record.");KEYNRD(subposper,1);SEENRDRECORD(subposper);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by Ranging on Location Identification Master or a deleted Location Identification Master Record.");KEYNRD(subposlmst,1);SEENRDRECORD(subposlmst);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by not finding Location Identification Record.");KEYNRD(subposlmst,1);SEENRDRECORD(subposlmst);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by Ranging on Class Identification Master or a deleted Class Identification Master Record.");KEYNRD(subcls,1);SEENRDRECORD(subcls);
								}
							}
							} else {
								if(diagapps)
								{
								prterr("Skipped by not finding Class Identification Record.");KEYNRD(subcls,1);SEENRDRECORD(subcls);
								}
							}
						} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Position Identification Master or a deleted Position Identification or an incorrect position Master Record.");KEYNRD(subposmstr,1);SEENRDRECORD(subposmstr);
							}
						}
					} else {
						if(diagapps)
						{
						prterr("Skipped by not finding Position Identification Record.");KEYNRD(subposmstr,1);SEENRDRECORD(subposmstr);
						}
					}
				} else {
					if(diagapps)
					{
					prterr("Skipped by Comparing Position Identification [%s] selected to Job Master Position Identification [%s].",(class_identification==NULL?"":class_identification),(subposid==NULL?"":subposid));KEYNRD(subjobmst,1);SEENRDRECORD(subjobmst);
					}
				}
				} else {
					if(diagapps)
					{
					prterr("Skipped by Ranging on Job Master or deleted Job Master Record.");KEYNRD(subjobmst,1);SEENRDRECORD(subjobmst);
					}
				}
			}
			} else {
				if(diagapps)
				{
				prterr("Skipped by Ranging on Personnel Master or deleted Personnel Master Record.");KEYNRD(subprsnnl,1);SEENRDRECORD(subprsnnl);
				}
			}
			} else {
				if(diagapps)
				{
				prterr("Skipped by not finding Personnel Master Record.");KEYNRD(subprsnnl,1);SEENRDRECORD(subprsnnl);
				}
			}
		} else {
			if(diagapps)
			{
			prterr("Skipped by Ranging on Substitute Master or a deleted Substitute Master Record.");KEYNRD(submst,1);SEENRDRECORD(submst);
			}
		}
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(do_diag ? TRUE:FALSE));
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(subperid!=NULL) Rfree(subperid);
	if(posid!=NULL) Rfree(posid);
	if(subposid!=NULL) Rfree(subposid);
	if(locid!=NULL) Rfree(locid);
	if(startdate!=NULL) Rfree(startdate);
	if(enddate!=NULL) Rfree(enddate);
	if(rs!=NULL) FreeRangeScreen(rs);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		MTNMASTER1=MaintainMasterNew(module,"CONTACT SUBS");
		if(findMaintainMaster(MTNMASTER1))
		{
			ERRORDIALOG("MAINTAIN MTNMASTER1 NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
			prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","CONTACT SUBSTITUTES",module);
			doexit(NULL);
		} else {
			MTNMASTER1->passkey=READPassKey(empabtr);
			switch(MTNMASTER1->start_with)
			{
				default:
				case 0:
					subsm(1,NULL,dbsort);
					break;
				case 1:
					browse_subs(NULL,dbsort);
					break;
			}
		}
	} else {
		if(perid!=NULL) Rfree(perid);
		ERRORDIALOG("Contact Substitues Error","No records selected.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		dbsort=NULL;
	}
}
static void browse_subs(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	char *empid=NULL;
	int emptransno=0,empjobno=0;
	double sub_units_unass=0.0;

	if(mtnrsrc!=NULL)
	{
		Rfree(mtnrsrc);
		if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
	}
	brsrc=RDArsrcNEW(module,MTNMASTER1->browse_screen);
	srsrc=RDArsrcNEW(module,MTNMASTER1->search_browse);
	drsrc=RDArsrcNEW(module,MTNMASTER1->define_list);
	addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,subprsnnl);
	addDFincvir(drsrc,"PAYROLL","PAYJMST",NULL,subjobmst);
	addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,subposmstr);
	addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,subprsnnl);
	addDFincvir(srsrc,"PAYROLL","PAYJMST",NULL,subjobmst);
	addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,subposmstr);
	FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDGETINT(empabtr,"TRANSACTION NUMBER",&emptransno);
	FINDFLDGETINT(empabtr,"JOB NUMBER",&empjobno);
	FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units_unass);
	addstdrsrc(brsrc,"EMPLOYEE IDENTIFICATION",VARIABLETEXT,0,empid,FALSE);
	addstdrsrc(brsrc,"EMPLOYEE TRANSACTION NUMBER",LONGV,6,&emptransno,FALSE);
	addstdrsrc(brsrc,"EMPLOYEE JOB NUMBER",LONGV,6,&empjobno,FALSE);
	addstdrsrc(brsrc,"SUBSTITUTE UNITS UNASSIGNED",SDECIMALV,8,&sub_units_unass,FALSE);
	if(empid!=NULL) Rfree(empid);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	ZERNRD(dbsort->fileno);
	mbl_subs=ADVbrowse(dbsort->fileno,1,brsrc,srsrc,drsrc,NULL,
		selectfunction,doexit,okfunc,NULL,dbsort,TRUE,NULL,
		0,0,0,TRUE);
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *subperid=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&subperid);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",subperid);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
		changesubper(mtnrsrc,dbsort);
		check_subid(mtnrsrc,dbsort);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
		setsublist(mtnrsrc,dbsort);
	}
	if(subperid!=NULL) Rfree(subperid);
	getsupportingfiles(mtnrsrc,dbsort);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
void subsm(short dowhich,void *targetkey,DBsort *dbsort)
{
	RDArsrc *mtnrsrc=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	int which_type=0;
	double sub_units=0.0;
	short nofields=0,x=0,edit_rsrc=FALSE;
	char editable=TRUE,*name=NULL;
	char *empid=NULL,*locid=NULL,*posid=NULL;
	char *curdate=NULL,*curtime=NULL,*startdate=NULL,*enddate=NULL;
	int transno=0,jobno=0;
	NRDfield *fields=NULL,*fldx=NULL;

	mtnrsrc=RDArsrcNEW(module,MTNMASTER1->maintain_screen);
	nofields=NUMFLDS(dbsort->fileno);
	fields=FLDPOINTER(dbsort->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=TRUE;
			if(!RDAstrcmp(fldx->name,"RESPONSE TYPE"))
			{
				if(rsptypes!=NULL) freeapplib(rsptypes);
				rsptypes=makersplist();
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"RESPONSE TYPES",
						&which_type,edit_rsrc,
						NULL,rsptypes->numlibs,
						&rsptypes->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"JOB NUMBER"))
			{
				if(subjobnolist!=NULL) freeapplib(subjobnolist);
				subjobnolist=APPlibNEW();
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"JOB NUMBERS",
						&which_type,edit_rsrc,
						changesubjobnocb,
						subjobnolist->numlibs,
						&subjobnolist->libs,dbsort);
				}
			} else if(!RDAstrcmp(fldx->name,"ENTRY DATE"))
			{
			} else if(!RDAstrcmp(fldx->name,"ENTRY TIME"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					editable=(char)edit_rsrc;
					if(name==NULL)
					{
						name=Rmalloc(RDAstrlen(fldx->name)+1);
					} else {
						name=Rrealloc(name,RDAstrlen(fldx->name)+1);
					}
					sprintf(name,"%s",fldx->name);
					makefieldrsrc(mtnrsrc,name,fldx->type,fldx->len,editable);
				}
			}
		}
	}
	nofields=0;
	if(name!=NULL) Rfree(name);
	FINDRSCSETEDITABLE(mtnrsrc,"EMPLOYEE IDENTIFICATION",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"POSITION IDENTIFICATION",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"LOCATION IDENTIFICATION",FALSE);
	if(subprsnnl!=(-1)) singlefile2rsrc(subprsnnl,mtnrsrc,FALSE);
	if(subprsnnl2!=(-1)) file2rsrc(subprsnnl2,mtnrsrc,FALSE);
	if(subjobmst!=(-1)) file2rsrc(subjobmst,mtnrsrc,FALSE);
	if(subposmstr!=(-1)) file2rsrc(subposmstr,mtnrsrc,FALSE);
	if(subposlmst!=(-1)) file2rsrc(subposlmst,mtnrsrc,FALSE);
	addDFincvir(mtnrsrc,"SECURITY","USERS",NULL,dbsort->fileno); 
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,subprsnnl);
	addDFincvir(mtnrsrc,"PAYROLL","PAYJMST",NULL,subjobmst);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,subposmstr);
	addDFincvir(mtnrsrc,"POSTRK","POSLMST",NULL,subposlmst);
	key=KEYNUM(dbsort->fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			FINDRSCSETFUNC(mtnrsrc,part->name,getfile,dbsort);
		}
	}
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* Defaulting from the merge of screendefaults &
				empabtr record*/
			FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&empid);
			FINDRSCSETSTRING(mtnrsrc,"EMPLOYEE IDENTIFICATION",empid);
			FINDFLDGETINT(empabtr,"TRANSACTION NUMBER",&transno);
			FINDRSCSETINT(mtnrsrc,"EMPLOYEE TRANSACTION NUMBER",transno);
			FINDFLDGETINT(empabtr,"JOB NUMBER",&jobno);
			FINDRSCSETINT(mtnrsrc,"EMPLOYEE JOB NUMBER",jobno);
			FINDFLDGETSTRING(posmstr,"POSITION IDENTIFICATION",&posid);
			FINDRSCSETSTRING(mtnrsrc,"POSITION IDENTIFICATION",posid);
			FINDFLDGETSTRING(poslmst,"LOCATION IDENTIFICATION",&locid);
			FINDRSCSETSTRING(mtnrsrc,"LOCATION IDENTIFICATION",locid);
			FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units);
			FINDRSCSETDOUBLE(mtnrsrc,"UNITS",sub_units);
			FINDFLDGETSTRING(empabs,"STARTING DATE",&startdate);
			FINDRSCSETSTRING(mtnrsrc,"STARTING DATE",startdate);
			FINDFLDGETSTRING(empabs,"ENDING DATE",&enddate);
			FINDRSCSETSTRING(mtnrsrc,"ENDING DATE",enddate);
			curdate=GETCURRENTDATE();
			curtime=GETCURRENTTIME();
			FINDRSCSETSTRING(mtnrsrc,"CONTACT DATE",curdate);
			FINDRSCSETSTRING(mtnrsrc,"CONTACT TIME",curtime);
			if(curdate!=NULL) Rfree(curdate);
			if(curtime!=NULL) Rfree(curtime);
			if(empid!=NULL) Rfree(empid);
			if(posid!=NULL) Rfree(posid);
			if(locid!=NULL) Rfree(locid);
			if(startdate!=NULL) Rfree(startdate);
			if(enddate!=NULL) Rfree(enddate);
			rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
			changesubper(mtnrsrc,dbsort);
			break;
		case 2:
			FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&empid);
			FINDRSCSETSTRING(mtnrsrc,"EMPLOYEE IDENTIFICATION",empid);
			FINDFLDGETINT(empabtr,"TRANSACTION NUMBER",&transno);
			FINDRSCSETINT(mtnrsrc,"EMPLOYEE TRANSACTION NUMBER",transno);
			FINDFLDGETINT(empabtr,"JOB NUMBER",&jobno);
			FINDRSCSETINT(mtnrsrc,"EMPLOYEE JOB NUMBER",jobno);
			FINDFLDGETSTRING(posmstr,"POSITION IDENTIFICATION",&posid);
			FINDRSCSETSTRING(mtnrsrc,"POSITION IDENTIFICATION",posid);
			FINDFLDGETSTRING(poslmst,"LOCATION IDENTIFICATION",&locid);
			FINDRSCSETSTRING(mtnrsrc,"LOCATION IDENTIFICATION",locid);
			FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units);
			FINDRSCSETDOUBLE(mtnrsrc,"UNITS",sub_units);
			FINDFLDGETSTRING(empabs,"STARTING DATE",&startdate);
			FINDRSCSETSTRING(mtnrsrc,"STARTING DATE",startdate);
			FINDFLDGETSTRING(empabs,"ENDING DATE",&enddate);
			FINDRSCSETSTRING(mtnrsrc,"ENDING DATE",enddate);
			curdate=GETCURRENTDATE();
			curtime=GETCURRENTTIME();
			FINDRSCSETSTRING(mtnrsrc,"CONTACT DATE",curdate);
			FINDRSCSETSTRING(mtnrsrc,"CONTACT TIME",curtime);
			if(curdate!=NULL) Rfree(curdate);
			if(curtime!=NULL) Rfree(curtime);
			if(empid!=NULL) Rfree(empid);
			if(posid!=NULL) Rfree(posid);
			if(locid!=NULL) Rfree(locid);
			if(startdate!=NULL) Rfree(startdate);
			if(enddate!=NULL) Rfree(enddate);
			rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
			changesubper(mtnrsrc,dbsort);
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(dbsort->fileno,1,targetkey)) 
			{
				KEYNRD(dbsort->fileno,1);
				singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
				changesubper(mtnrsrc,dbsort);
			} else {
				singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
				setsublist(mtnrsrc,dbsort);
			}
			break;
	}
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	getsupportingfiles(mtnrsrc,dbsort);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,dbsort);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,dbsort);
	if(mbl_subs==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_subs,dbsort);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record1,dbsort);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mtnrsrc,TRUE,quit_record1,dbsort,FALSE);
}
static void subselected(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *curdate=NULL,*curtime=NULL;
	double sub_units_unass=0.0,sub_units=0.0;
	void *targetkey=NULL;
	RDATData *prev_empabtr=NULL;

	FINDRSCGETDOUBLE(mtnrsrc,"UNITS",&sub_units);
	FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units_unass);
	if(check_subjobno(mtnrsrc,dbsort)==FALSE)
	{
		return;
	}
	curdate=GETCURRENTDATE();
	curtime=GETCURRENTTIME();
	LOCNRDFILE(empabtr);
	if(!EQLNRDsec(empabtr,1))
	{
		prev_empabtr=RDATDataNEW(empabtr);
		FINDFLDSETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",(sub_units_unass-sub_units));
		if((sub_units_unass-sub_units)<=0.0)
		{
			FINDFLDSETCHAR(empabtr,"SUBSTITUTE FINALED",TRUE);
			FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED DATE",curdate);
			FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED TIME",curtime);
			FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED BY",USERLOGIN);
		}
		if(!WRTTRANSsec(empabtr,0,NULL,prev_empabtr))
		{
			quit_record1(mtnrsrc,dbsort);
			if(mbl_subs!=NULL)
			{
				if((sub_units_unass-sub_units)<=0.0)
				{
					doexit(mbl_subs);
				} else {
					FINDRSCSETDOUBLE(mbl_subs->mainrsrc,"SUBSTITUTE UNITS UNASSIGNED",(sub_units_unass-sub_units));
					updatersrc(mbl_subs->mainrsrc,"SUBSTITUTE UNITS UNASSIGNED");
					
				}
			}
			if(empabtrrsrc!=NULL)
			{
				if(mbl_empabtr!=NULL)
				{
					updatebrowse(TRUE,mbl_empabtr,empabtr,module,NULL);
					targetkey=(void *)mbl_empabtr->list->keyvalue;
					EQLNRDKEYsec(empabtr,1,targetkey);
					if(!RDAstrcmp(empabtrrsrc->screen,MTNMASTER->maintain_screen))
					{
						updatefilerecord2rsrc(empabtr,empabtrrsrc);
						getsupportingrecords(empabtrrsrc,TRUE,TRUE);
						if((sub_units_unass-sub_units)<=0.0)
						{
#ifdef XXX
							quit_record(empabtrrsrc);
#endif
						} else {
       							updateSCRNvirtuals(empabtrrsrc);
						}
					} else {
       						updateSCRNvirtuals(mbl_empabtr->mainrsrc);
					}
				} else {
					updatefilerecord2rsrc(empabtr,empabtrrsrc);
					getsupportingrecords(empabtrrsrc,TRUE,TRUE);
       					updateSCRNvirtuals(empabtrrsrc);
				}
			}
		} else {
			/* WRTTRANSsec ERRORDIALOG() goes here */
		}
	} else {
		/* EQLNRDsec() failed ERRORDIALOG() goes here */
	}
	UNLNRDFILE(empabtr);
	if(prev_empabtr!=NULL) FreeRDATData(prev_empabtr);
	if(curdate!=NULL) Rfree(curdate);
	if(curtime!=NULL) Rfree(curtime);
}
static void save_record_up(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *tmp=NULL;
	
	tmp=stralloc(mtnrsrc->screen);
	save_check(mtnrsrc,TRUE,dbsort);
	if(tmp!=NULL) Rfree(tmp);
}
static void save_record_nup(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_check(mtnrsrc,FALSE,dbsort);
}
static void save_check(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	if(check_subid(mtnrsrc,dbsort)==FALSE)
	{
		return;
	}
	if(check_subunits(mtnrsrc,dbsort)==FALSE)
	{
		return;
	}
	save_record(mtnrsrc,update_list,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0,selresponse=0;
	char *curdate=NULL,*curtime=NULL;
	double sub_units_unass=0.0,units=0.0,tempunits=0.0;
	int selsubjobno=0,subjobno=0,temptimes=0;
	short response=0;
	RDArsrc *tmprsrc=NULL;
	char *perid=NULL,*subperid=NULL,*warnmessage=NULL,*date=NULL,*timex=NULL;
	RDATData *prev_subfile=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"RESPONSE TYPES",&selresponse);
	response=(short)selresponse;
	FINDFLDSETSHORT(dbsort->fileno,"RESPONSE TYPE",response);
	if(!WRTNRD(dbsort->fileno,0,NULL))
	{
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&subperid);
		FINDFLDGETDOUBLE(dbsort->fileno,"UNITS",&units);
		COPYFIELD(dbsort->fileno,subctact,"PERSONNEL IDENTIFICATION");
		COPYFIELD(dbsort->fileno,subctact,"LOCATION IDENTIFICATION");
		COPYFIELD(dbsort->fileno,subctact,"POSITION IDENTIFICATION");
		COPYFIELD(dbsort->fileno,subctact,"EMPLOYEE IDENTIFICATION");
		COPYFIELD(dbsort->fileno,subctact,"CONTACT DATE");
		COPYFIELD(dbsort->fileno,subctact,"CONTACT TIME");
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		FINDFLDSETSTRING(subctact,"ENTRY DATE",date);
		FINDFLDSETSTRING(subctact,"ENTRY TIME",timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		FINDFLDSETSTRING(subctact,"SOURCE USER",USERLOGIN);
		COPYFIELD(dbsort->fileno,subctact,"DESCRIPTION");
		FINDFLDSETSHORT(subctact,"RESPONSE TYPE",response);
		if(response==0) /* Accepted Assignment */
		{
			FINDFLDGETSTRING(dbsort->fileno,"EMPLOYEE IDENTIFICATION",&perid);
			FINDFLDGETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",&sub_units_unass);
			if(sub_units_unass<units)
			{
				warnmessage=Rmalloc(RDAstrlen(perid)+255);
				sprintf(warnmessage,"Substitute Units are more that Substitute Units Unassigned for the Employee Absentee for the Personnel Identification of [%s].  Please select a valid number of Substitute Units to save this record.",
					(perid==NULL?"":perid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(perid!=NULL) Rfree(perid);
				if(subperid!=NULL) Rfree(subperid);
				return;
			}
		}
		if(perid!=NULL) Rfree(perid);
		if(!WRTTRANSsec(subctact,1,"TRANSACTION NUMBER",NULL))
		{
			++SUBCTACT_TRANSACTION_COUNT;
			if(response==0) /* Accepted Assignment */
			{
/* Killing All of the Select Substitutes Screens and updating the empabs dbsort */ 
/* record with the Selected Substitute.*/
				FINDRSCGETINT(mtnrsrc,"JOB NUMBERS",&selsubjobno);
				if((!RDAstrcmp(subjobnolist->libs[selsubjobno],"No Jobs Available")) || 
					(!RDAstrcmp(subjobnolist->libs[selsubjobno],"Not Applicable")))
				{
					warnmessage=Rmalloc(RDAstrlen(subperid)+150);
					sprintf(warnmessage,"No Job Number Available for the Selected Substitute Personnel Identification of [%s].  Please select a valid Job Number to save this record.",
						(subperid==NULL?"":subperid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					if(subperid!=NULL) Rfree(subperid);
					if(perid!=NULL) Rfree(perid);
					return;
				}
				subjobno=Ratoi(subjobnolist->libs[selsubjobno]);
				ZERNRD(subwrk);
				curdate=GETCURRENTDATE();
				curtime=GETCURRENTTIME();
				COPYFIELD(dbsort->fileno,subwrk,"PERSONNEL IDENTIFICATION");
				FINDFLDSETINT(subwrk,"JOB NUMBER",subjobno);
				COPYFIELD(dbsort->fileno,subwrk,"POSITION IDENTIFICATION");
				COPYFIELD(dbsort->fileno,subwrk,"EMPLOYEE IDENTIFICATION");
				COPYFIELD(dbsort->fileno,subwrk,"EMPLOYEE TRANSACTION NUMBER");
				COPYFIELD(dbsort->fileno,subwrk,"EMPLOYEE JOB NUMBER");
				COPYFIELD(dbsort->fileno,subwrk,"UNITS");
				COPYFIELD(dbsort->fileno,subwrk,"STARTING DATE");
				COPYFIELD(dbsort->fileno,subwrk,"ENDING DATE");
				FINDFLDSETCHAR(subwrk,"DELETEFLAG",FALSE);
				FINDFLDSETCHAR(subwrk,"SOURCE USER VERIFIED",FALSE);
				FINDFLDSETSTRING(subwrk,"SOURCE USER",USERLOGIN);
				FINDFLDSETSTRING(subwrk,"ENTRY DATE",curdate);
				FINDFLDSETSTRING(subwrk,"ENTRY TIME",curtime);
				WRTTRANSsec(subwrk,1,"TRANSACTION NUMBER",NULL);
				UNLNRD(subwrk);
				if(curdate!=NULL) Rfree(curdate);
				if(curtime!=NULL) Rfree(curtime);
				COPYFIELD(dbsort->fileno,submst,"PERSONNEL IDENTIFICATION");
				LOCNRD(submst);
				if(!EQLNRD(submst,1))
				{
					prev_subfile=RDATDataNEW(submst);
					FINDFLDGETDOUBLE(submst,"ACCEPTED UNITS",&tempunits);
					tempunits+=units;
					FINDFLDSETDOUBLE(submst,"ACCEPTED UNITS",tempunits);
					FINDFLDGETINT(submst,"TIMES ACCEPTED",&temptimes);
					temptimes+=1;
					FINDFLDSETINT(submst,"TIMES ACCEPTED",temptimes);
					if(WRTTRANSsec(submst,0,NULL,prev_subfile))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBMASTR][ACCEPTED UNITS] and [SUBMGT][SUBMASTR][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
					}
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","RECORD ERROR OCCURRED!","The Equal Record function failed when attempting to Select the [SUBMGT][SUBMASTR] record.",NULL,NULL,TRUE,NULL);
				}
				UNLNRD(submst);
				if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
				ZERNRD(subemp);
				COPYFIELD(dbsort->fileno,subemp,"PERSONNEL IDENTIFICATION");
				COPYFIELD(dbsort->fileno,subemp,"EMPLOYEE IDENTIFICATION");
				LOCNRD(subemp);
				if(!EQLNRD(subemp,1))
				{
					prev_subfile=RDATDataNEW(subemp);
					FINDFLDGETDOUBLE(subemp,"ACCEPTED UNITS",&tempunits);
					tempunits+=units;
					FINDFLDSETDOUBLE(subemp,"ACCEPTED UNITS",tempunits);
					FINDFLDGETINT(subemp,"TIMES ACCEPTED",&temptimes);
					temptimes+=1;
					FINDFLDSETINT(subemp,"TIMES ACCEPTED",temptimes);
					if(WRTTRANSsec(subemp,0,NULL,prev_subfile))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][ACCEPTED UNITS] and [SUBMGT][SUBEMP][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
					}
				} else {
					FINDFLDSETCHAR(subemp,"DELETEFLAG",FALSE);
					FINDFLDSETCHAR(subemp,"EMPLOYEE BLOCK",FALSE);
					FINDFLDSETDOUBLE(subemp,"ACCEPTED UNITS",units);
					temptimes=1;
					FINDFLDSETINT(subemp,"TIMES ACCEPTED",temptimes);
					if(WRTTRANSsec(subemp,0,NULL,NULL))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][ACCEPTED UNITS] and [SUBMGT][SUBEMP][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
					}
				}
				UNLNRD(subemp);
				if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
				ZERNRD(subpos);
				COPYFIELD(dbsort->fileno,subpos,"PERSONNEL IDENTIFICATION");
				COPYFIELD(dbsort->fileno,subpos,"POSITION IDENTIFICATION");
				LOCNRD(subpos);
				if(!EQLNRD(subpos,1))
				{
					prev_subfile=RDATDataNEW(subpos);
					FINDFLDGETDOUBLE(subpos,"ACCEPTED UNITS",&tempunits);
					tempunits+=units;
					FINDFLDSETDOUBLE(subpos,"ACCEPTED UNITS",tempunits);
					FINDFLDGETINT(subpos,"TIMES ACCEPTED",&temptimes);
					temptimes+=1;
					FINDFLDSETINT(subpos,"TIMES ACCEPTED",temptimes);
					if(WRTTRANSsec(subpos,0,NULL,prev_subfile))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][ACCEPTED UNITS] and [SUBMGT][SUBPOS][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
					}
				} else {
					FINDFLDSETCHAR(subpos,"DELETEFLAG",FALSE);
					FINDFLDSETCHAR(subpos,"POSITION BLOCK",FALSE);
					FINDFLDSETDOUBLE(subpos,"ACCEPTED UNITS",units);
					temptimes=1;
					FINDFLDSETINT(subpos,"TIMES ACCEPTED",temptimes);
					if(WRTTRANSsec(subpos,0,NULL,NULL))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][ACCEPTED UNITS] and [SUBMGT][SUBPOS][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
					}
				}
				UNLNRD(subpos);
				if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
				ZERNRD(subloc);
				COPYFIELD(dbsort->fileno,subloc,"PERSONNEL IDENTIFICATION");
				COPYFIELD(dbsort->fileno,subloc,"LOCATION IDENTIFICATION");
				LOCNRD(subloc);
				if(!EQLNRD(subloc,1))
				{
					prev_subfile=RDATDataNEW(subloc);
					FINDFLDGETDOUBLE(subloc,"ACCEPTED UNITS",&tempunits);
					tempunits+=units;
					FINDFLDSETDOUBLE(subloc,"ACCEPTED UNITS",tempunits);
					FINDFLDGETINT(subloc,"TIMES ACCEPTED",&temptimes);
					temptimes+=1;
					FINDFLDSETINT(subloc,"TIMES ACCEPTED",temptimes);
					if(WRTTRANSsec(subloc,0,NULL,prev_subfile))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][ACCEPTED UNITS] and [SUBMGT][SUBLOC][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
					}
				} else {
					FINDFLDSETCHAR(subloc,"DELETEFLAG",FALSE);
					FINDFLDSETCHAR(subloc,"LOCATION BLOCK",FALSE);
					FINDFLDSETDOUBLE(subloc,"ACCEPTED UNITS",units);
					temptimes=1;
					FINDFLDSETINT(subloc,"TIMES ACCEPTED",temptimes);
					if(WRTTRANSsec(subloc,0,NULL,NULL))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][ACCEPTED UNITS] and [SUBMGT][SUBLOC][TIMES ACCEPTED].",NULL,NULL,TRUE,NULL);
					}
				}
				UNLNRD(subloc);
				if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
				subselected(mtnrsrc,dbsort);
			} else {
				if(response==1) /* Refused Assignment */
				{
					ZERNRD(submst);
					COPYFIELD(dbsort->fileno,submst,"PERSONNEL IDENTIFICATION");
					LOCNRD(submst);
					if(!EQLNRD(submst,1))
					{
						prev_subfile=RDATDataNEW(submst);
						FINDFLDGETDOUBLE(submst,"REFUSED UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(submst,"REFUSED UNITS",tempunits);
						FINDFLDGETINT(submst,"TIMES REFUSED",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(submst,"TIMES REFUSED",temptimes);
						if(WRTTRANSsec(submst,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBMASTR][REFUSED UNITS] and [SUBMGT][SUBMASTR][TIMES REFUSED].",NULL,NULL,TRUE,NULL);
						}
					} else {
						WARNINGDIALOG("WARNING DIALOG SCREEN","RECORD ERROR OCCURRED!","The Equal Record function failed when attempting to Select the [SUBMGT][SUBMASTR] record.",NULL,NULL,TRUE,NULL);
					}
					UNLNRD(submst);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
					ZERNRD(subemp);
					COPYFIELD(dbsort->fileno,subemp,"PERSONNEL IDENTIFICATION");
					COPYFIELD(dbsort->fileno,subemp,"EMPLOYEE IDENTIFICATION");
					LOCNRD(subemp);
					if(!EQLNRD(subemp,1))
					{
						prev_subfile=RDATDataNEW(subemp);
						FINDFLDGETDOUBLE(subemp,"REFUSED UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(subemp,"REFUSED UNITS",tempunits);
						FINDFLDGETINT(subemp,"TIMES REFUSED",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(subemp,"TIMES REFUSED",temptimes);
						if(WRTTRANSsec(subemp,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][REFUSED UNITS] and [SUBMGT][SUBEMP][TIMES REFUSED].",NULL,NULL,TRUE,NULL);
						}
					} else {
						FINDFLDSETCHAR(subemp,"DELETEFLAG",FALSE);
						FINDFLDSETCHAR(subemp,"EMPLOYEE BLOCK",FALSE);
						FINDFLDSETDOUBLE(subemp,"REFUSED UNITS",units);
						temptimes=1;
						FINDFLDSETINT(subemp,"TIMES REFUSED",temptimes);
						if(WRTTRANSsec(subemp,0,NULL,NULL))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][REFUSED UNITS] and [SUBMGT][SUBEMP][TIMES REFUSED].",NULL,NULL,TRUE,NULL);
						}
					}
					UNLNRD(subemp);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
					ZERNRD(subpos);
					COPYFIELD(dbsort->fileno,subpos,"PERSONNEL IDENTIFICATION");
					COPYFIELD(dbsort->fileno,subpos,"POSITION IDENTIFICATION");
					LOCNRD(subpos);
					if(!EQLNRD(subpos,1))
					{
						prev_subfile=RDATDataNEW(subpos);
						FINDFLDGETDOUBLE(subpos,"REFUSED UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(subpos,"REFUSED UNITS",tempunits);
						FINDFLDGETINT(subpos,"TIMES REFUSED",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(subpos,"TIMES REFUSED",temptimes);
						if(WRTTRANSsec(subpos,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][REFUSED UNITS] and [SUBMGT][SUBPOS][TIMES REFUSED].",NULL,NULL,TRUE,NULL);
						}
					} else {
						FINDFLDSETCHAR(subpos,"DELETEFLAG",FALSE);
						FINDFLDSETCHAR(subpos,"POSITION BLOCK",FALSE);
						FINDFLDSETDOUBLE(subpos,"REFUSED UNITS",units);
						temptimes=1;
						FINDFLDSETINT(subpos,"TIMES REFUSED",temptimes);
						if(WRTTRANSsec(subpos,0,NULL,NULL))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][REFUSED UNITS] and [SUBMGT][SUBPOS][TIMES REFUSED].",NULL,NULL,TRUE,NULL);
						}
					}
					UNLNRD(subpos);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
					ZERNRD(subloc);
					COPYFIELD(dbsort->fileno,subloc,"PERSONNEL IDENTIFICATION");
					COPYFIELD(dbsort->fileno,subloc,"LOCATION IDENTIFICATION");
					LOCNRD(subloc);
					if(!EQLNRD(subloc,1))
					{
						prev_subfile=RDATDataNEW(subloc);
						FINDFLDGETDOUBLE(subloc,"REFUSED UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(subloc,"REFUSED UNITS",tempunits);
						FINDFLDGETINT(subloc,"TIMES REFUSED",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(subloc,"TIMES REFUSED",temptimes);
						if(WRTTRANSsec(subloc,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][REFUSED UNITS] and [SUBMGT][SUBLOC][TIMES REFUSED].",NULL,NULL,TRUE,NULL);
						}
					} else {
						FINDFLDSETCHAR(subloc,"DELETEFLAG",FALSE);
						FINDFLDSETCHAR(subloc,"LOCATION BLOCK",FALSE);
						FINDFLDSETDOUBLE(subloc,"REFUSED UNITS",units);
						temptimes=1;
						FINDFLDSETINT(subloc,"TIMES REFUSED",temptimes);
						if(WRTTRANSsec(subloc,0,NULL,NULL))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][REFUSED UNITS] and [SUBMGT][SUBLOC][TIMES REFUSED].",NULL,NULL,TRUE,NULL);
						}
					}
					UNLNRD(subloc);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
				} else if(response==2) /* Other */
				{
					ZERNRD(submst);
					COPYFIELD(dbsort->fileno,submst,"PERSONNEL IDENTIFICATION");
					LOCNRD(submst);
					if(!EQLNRD(submst,1))
					{
						prev_subfile=RDATDataNEW(submst);
						FINDFLDGETDOUBLE(submst,"OTHER UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(submst,"OTHER UNITS",tempunits);
						FINDFLDGETINT(submst,"TIMES OTHER",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(submst,"TIMES OTHER",temptimes);
						if(WRTTRANSsec(submst,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBMASTR][OTHER UNITS] and [SUBMGT][SUBMASTR][TIMES OTHER].",NULL,NULL,TRUE,NULL);
						}
					} else {
						WARNINGDIALOG("WARNING DIALOG SCREEN","RECORD ERROR OCCURRED!","The Equal Record function failed when attempting to Select the [SUBMGT][SUBMASTR] record.",NULL,NULL,TRUE,NULL);
					}
					UNLNRD(submst);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
					ZERNRD(subemp);
					COPYFIELD(dbsort->fileno,subemp,"PERSONNEL IDENTIFICATION");
					COPYFIELD(dbsort->fileno,subemp,"EMPLOYEE IDENTIFICATION");
					LOCNRD(subemp);
					if(!EQLNRD(subemp,1))
					{
						prev_subfile=RDATDataNEW(subemp);
						FINDFLDGETDOUBLE(subemp,"OTHER UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(subemp,"OTHER UNITS",tempunits);
						FINDFLDGETINT(subemp,"TIMES OTHER",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(subemp,"TIMES OTHER",temptimes);
						if(WRTTRANSsec(subemp,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][OTHER UNITS] and [SUBMGT][SUBEMP][TIMES OTHER].",NULL,NULL,TRUE,NULL);
						}
					} else {
						FINDFLDSETCHAR(subemp,"DELETEFLAG",FALSE);
						FINDFLDSETCHAR(subemp,"EMPLOYEE BLOCK",FALSE);
						FINDFLDSETDOUBLE(subemp,"OTHER UNITS",units);
						temptimes=1;
						FINDFLDSETINT(subemp,"TIMES OTHER",temptimes);
						if(WRTTRANSsec(subemp,0,NULL,NULL))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBEMP][OTHER UNITS] and [SUBMGT][SUBEMP][TIMES OTHER].",NULL,NULL,TRUE,NULL);
						}
					}
					UNLNRD(subemp);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
					ZERNRD(subpos);
					COPYFIELD(dbsort->fileno,subpos,"PERSONNEL IDENTIFICATION");
					COPYFIELD(dbsort->fileno,subpos,"POSITION IDENTIFICATION");
					LOCNRD(subpos);
					if(!EQLNRD(subpos,1))
					{
						prev_subfile=RDATDataNEW(subpos);
						FINDFLDGETDOUBLE(subpos,"OTHER UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(subpos,"OTHER UNITS",tempunits);
						FINDFLDGETINT(subpos,"TIMES OTHER",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(subpos,"TIMES OTHER",temptimes);
						if(WRTTRANSsec(subpos,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][OTHER UNITS] and [SUBMGT][SUBPOS][TIMES OTHER].",NULL,NULL,TRUE,NULL);
						}
					} else {
						FINDFLDSETCHAR(subpos,"DELETEFLAG",FALSE);
						FINDFLDSETCHAR(subpos,"POSITION BLOCK",FALSE);
						FINDFLDSETDOUBLE(subpos,"OTHER UNITS",units);
						temptimes=1;
						FINDFLDSETINT(subpos,"TIMES OTHER",temptimes);
						if(WRTTRANSsec(subpos,0,NULL,NULL))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBPOS][OTHER UNITS] and [SUBMGT][SUBPOS][TIMES OTHER].",NULL,NULL,TRUE,NULL);
						}
					}
					UNLNRD(subpos);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
					ZERNRD(subloc);
					COPYFIELD(dbsort->fileno,subloc,"PERSONNEL IDENTIFICATION");
					COPYFIELD(dbsort->fileno,subloc,"LOCATION IDENTIFICATION");
					LOCNRD(subloc);
					if(!EQLNRD(subloc,1))
					{
						prev_subfile=RDATDataNEW(subloc);
						FINDFLDGETDOUBLE(subloc,"OTHER UNITS",&tempunits);
						tempunits+=units;
						FINDFLDSETDOUBLE(subloc,"OTHER UNITS",tempunits);
						FINDFLDGETINT(subloc,"TIMES OTHER",&temptimes);
						temptimes+=1;
						FINDFLDSETINT(subloc,"TIMES OTHER",temptimes);
						if(WRTTRANSsec(subloc,0,NULL,prev_subfile))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][OTHER UNITS] and [SUBMGT][SUBLOC][TIMES OTHER].",NULL,NULL,TRUE,NULL);
						}
					} else {
						FINDFLDSETCHAR(subloc,"DELETEFLAG",FALSE);
						FINDFLDSETCHAR(subloc,"LOCATION BLOCK",FALSE);
						FINDFLDSETDOUBLE(subloc,"OTHER UNITS",units);
						temptimes=1;
						FINDFLDSETINT(subloc,"TIMES OTHER",temptimes);
						if(WRTTRANSsec(subloc,0,NULL,NULL))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the fields [SUBMGT][SUBLOC][OTHER UNITS] and [SUBMGT][SUBLOC][TIMES OTHER].",NULL,NULL,TRUE,NULL);
						}
					}
					UNLNRD(subloc);
					if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
				}
/* Have Not Found a Substitute so updateing Select Substitute Browse Screen*/ 
/* with new changes. */
				if(update_list==TRUE)
				{
				if(mbl_subs->mainrsrc!=NULL)
				{
					FINDRSCGETINT(mbl_subs->mainrsrc,"BROWSE LIST",&valuex);
					tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN","SECURITY","Browse Diagnostic",NULL);
					if(tmprsrc!=NULL)
					{
						if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
							ForceWindowUpdate(tmprsrc);
					}
					xmakebrowselist(mbl_subs,tmprsrc);
					if(mbl_subs->list->num<=valuex) valuex=0;
					mbl_subs->list->keyvalue=mbl_subs->list->key[valuex];
					if(!FINDRSCSETLIST(mbl_subs->mainrsrc,"BROWSE LIST",valuex,
						mbl_subs->list->num,&mbl_subs->list->string))
					{
						updatersrc(mbl_subs->mainrsrc,"BROWSE LIST");
					}
					if(tmprsrc!=NULL)
					{
						killwindow(tmprsrc);
						free_rsrc(tmprsrc);
					}
					ForceWindowUpdate(mtnrsrc);
				}
				}
				quit_record1(mtnrsrc,dbsort);
			}
		} else {
			WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to write to the file [SUBMGT][SUBCTACT].  Only the changes to the SORT FILE has been written.",NULL,NULL,TRUE,NULL);
		}
	} else {
		WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to write to the SORT FILE.  No changes have been saved.",NULL,NULL,TRUE,NULL);
		/* Error Dialog Needs to be put here for inability to Write record */
	}
	if(subperid!=NULL) Rfree(subperid);
}
static void quit_record1(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	if(subjobnolist!=NULL) freeapplib(subjobnolist);
	if(rsptypes!=NULL) freeapplib(rsptypes);
	if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
	if(mbl_subs==NULL)
	{
		if(dbsort!=NULL)
		{
			CLSDBsort(dbsort);
			FreeDBsort(dbsort);
			dbsort=NULL;
		}
		if(MTNMASTER1!=NULL)
		{
			FreeMaintainMaster(MTNMASTER1);
			MTNMASTER1=NULL;
		}
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	COPYFIELD(dbsort->fileno,submst,"PERSONNEL IDENTIFICATION");
	if(EQLNRDsec(submst,1)) KEYNRD(submst,1);
	filerecord2rsrc(submst,mtnrsrc);

	COPYFIELD(dbsort->fileno,subprsnnl,"PERSONNEL IDENTIFICATION");
	if(EQLNRDsec(subprsnnl,1)) KEYNRD(subprsnnl,1);
	singlefilerecord2rsrc(subprsnnl,mtnrsrc);

	FIELDCOPY(dbsort->fileno,"EMPLOYEE IDENTIFICATION",subprsnnl2,"PERSONNEL IDENTIFICATION");
	if(EQLNRDsec(subprsnnl2,1)) KEYNRD(subprsnnl2,1);
	filerecord2rsrc(subprsnnl2,mtnrsrc);

	COPYFIELD(dbsort->fileno,subjobmst,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,subjobmst,"JOB NUMBER");
	if(EQLNRDsec(subjobmst,1)) KEYNRD(subjobmst,1);
	filerecord2rsrc(subjobmst,mtnrsrc);

	COPYFIELD(subjobmst,subposmstr,"POSITION IDENTIFICATION");
	if(EQLNRDsec(subposmstr,1)) KEYNRD(subposmstr,1);
	filerecord2rsrc(subposmstr,mtnrsrc);

	COPYFIELD(subposmstr,subposlmst,"LOCATION IDENTIFICATION");
	if(EQLNRDsec(subposlmst,1)) KEYNRD(subposlmst,1);
	filerecord2rsrc(subposlmst,mtnrsrc);
}
