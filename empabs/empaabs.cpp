/* empaabs.c - Add Employee Absentee*/
#ifndef WIN32
#define __NAM__ "empaabs.lnx"
#endif
#ifdef WIN32
#define __NAM__ "empaabs.exe"
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

static short filenum=(-1),empabtr=(-1),empadef=(-1);
static short dmgnum=(-1),payjmst=(-1),posmstr=(-1),poslmst=(-1);
static short posgrs=(-1),poscls=(-1),posper=(-1),posdedc=(-1);
static short possmst=(-1),posngms=(-1);
static void EmpAbsSubData(char **,PassableStruct *);
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_perdmg=NULL,*mbl_empabtr=NULL;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void doexit1(MakeBrowseList *);
static short selectfunction(MakeBrowseList *);
static RDArsrc *definelist=NULL;
static APPlib *prevempabs=NULL;
static char *module="EMPABS";
static int counter=0;
static void set_defaults(RDArsrc *,short,PassableStruct *);
static void reset_defaults(RDArsrc *,PassableStruct *);
static void setfiles(RDArsrc *,PassableStruct *);
static DBsort *dbsort=NULL;
RDAfinmgt *fmgt=NULL;
static void computejobs(RDArsrc *);
static void select_backend(DBsort *);
static void savelist(RDArsrc *,PassableStruct *),savelistcb(RDArsrc *,PassableStruct *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *,PassableStruct *),quit_record(RDArsrc *,PassableStruct *);
static void quit_record_free(RDArsrc *,PassableStruct *);
static void quit_record_freens(RDArsrc *,PassableStruct *);
static void save_check(RDArsrc *,PassableStruct *),quit_check(RDArsrc *,PassableStruct *);
static short check_perid(RDArsrc *,short,PassableStruct *p);
static void check_peridcb(RDArsrc *,PassableStruct *);
static void check_unitscb(RDArsrc *,PassableStruct *);
static void getsupportingrecords(RDArsrc *,short,PassableStruct *);
static void changeper(RDArsrc *,short,short,PassableStruct *);
static void browse_perdmg(RDArsrc *,PassableStruct *);
static void browse_empabtr(RDArsrc *,PassableStruct *);
static char *personnel_identification=NULL,replacement_worker=FALSE;
static double absentee_units=0.0;
static char *audit_reportname=NULL;

void EmpAbsSubData(char **tmp,PassableStruct *p)
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
		prterr("DIAG EmpAbsSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
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
						temp=EVALUATEstr(v->expression,EmpAbsSubData,p);
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
						*v->value.short_value=EVALUATEsht(v->expression,EmpAbsSubData,p);
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
						*v->value.integer_value=EVALUATEint(v->expression,EmpAbsSubData,p);
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
						*v->value.float_value=EVALUATEdbl(v->expression,EmpAbsSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,EmpAbsSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,EmpAbsSubData,p);
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
		prterr("DIAG EmpAbsSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
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
	if(prevempabs!=NULL) freeapplib(prevempabs);
	prevempabs=APPlibNEW();
	ef=ADVFRSNRDsec(filenum,1,EmpAbsSubData,p);
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
		ef=ADVNXTNRDsec(filenum,1,EmpAbsSubData,p);
	}
        if(prevempabs->numlibs<1)
        {
                addAPPlib(prevempabs,"No Employee Absentees added this session");
        }
	if(!FINDRSCLISTAPPlib(p->r,"PREVIOUS EMPLOYEE ABSENTEES",(prevempabs->numlibs-1),prevempabs))
	{
		updatersrc(p->r,"PREVIOUS EMPLOYEE ABSENTEES");
	}
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(prevempabs!=NULL)
	{
		if(prevempabs->numlibs==1 && !RDAstrcmp(prevempabs->libs[0],
			"No Employee Absentees added this session"))
		{
			freeapplib(prevempabs);
			prevempabs=APPlibNEW();
		}
	} else {
		prevempabs=APPlibNEW();
	}
	for(x=1,length=0;x<(definelist->numrscs-7);x+=2)
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
	while(cur<(definelist->numrscs-7))
	{
		for(x=1;x<(definelist->numrscs-7);x+=2)
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
	addAPPlib(prevempabs,hold);
	++counter;
	if(hold!=NULL) Rfree(hold);
}
void definelistfunc(RDArsrc *parent)
{
	updateSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent,PassableStruct *p)
{
	set_defaults(parent,TRUE,p);
}
static void set_defaults(RDArsrc *parent,short update,PassableStruct *p)
{
	int selprev=0;
	char *perid1=NULL;

	FINDRSCGETINT(parent,"PREVIOUS EMPLOYEE ABSENTEES",&selprev);
	DefaultResetScreen(parent,TRUE);
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	changeper(parent,TRUE,update,p);
	getsupportingrecords(parent,update,p);
	if(perid1!=NULL) Rfree(perid1);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void check_peridcb(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	check_perid(parent,FALSE,p);
	changeper(parent,FALSE,TRUE,p);
	updateSCRNvirtuals(parent);
}
static void check_unitscb(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	changeper(parent,FALSE,TRUE,p);
	updateSCRNvirtuals(parent);
}
static short check_perid(RDArsrc *parent,short updatesupporting,PassableStruct *p)
{
	char *perid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1);
	if(dmgnum!=(-1))
	{
		ZERNRD(dmgnum);
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(ADVEQLNRDsec(dmgnum,1,EmpAbsSubData,p))
		{
			KEYNRD(dmgnum,1);
			warnmessage=Rmalloc(RDAstrlen(perid1)+145);
			sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
			getsupportingrecords(parent,updatesupporting,p);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+150);
				sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
				getsupportingrecords(parent,updatesupporting,p);
				if(perid1!=NULL) Rfree(perid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(dmgnum,"CURRENT EMPLOYEE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(perid1)+250);
					sprintf(warnmessage,"The Personnel Identification of [%s] is not a CURRENT EMPLOYEE in the [PRSNNL][PERDMG] data file.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","NOT CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(parent,updatesupporting,p);
					if(perid1!=NULL) Rfree(perid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	}
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	getsupportingrecords(parent,updatesupporting,p);
	if(perid1!=NULL) Rfree(perid1);
	return(TRUE);
}
static void setfiles(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	getsupportingrecords(parent,TRUE,p);
	updateSCRNvirtuals(parent);
}
static void getsupportingrecords(RDArsrc *parent,short updatesupporting,PassableStruct *p)
{
	char *perid1=NULL;

	FINDRSCGETSTRING(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1);
	if(dmgnum!=(-1))
	{
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(ADVEQLNRDsec(dmgnum,1,EmpAbsSubData,p)) KEYNRD(dmgnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(dmgnum,parent);
		} else {
			filerecord2rsrc(dmgnum,parent);
		}
	}
	if(perid1!=NULL) Rfree(perid1);
}
static void select_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE;
	char deleteflag=FALSE;
	char *curdate=NULL,*curtime=NULL,*e=NULL;
        int x,numfields=0;
        NRDfield *fields=NULL,*field=NULL;
	RDArsrc *tmprsrc=NULL;
	PassableStruct *p=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Creating Job Absentees",NULL);
		if(tmprsrc!=NULL)
		{
			if(!ADVmakescrn(tmprsrc,TRUE))
			{
				ForceWindowUpdate(tmprsrc);
			} else {
				e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
				sprintf(e,"The Make Screen function failed fo r the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
				prterr("Error %s.",e);
				if(e!=NULL) Rfree(e);
			}
		}
		addDFincvir(tmprsrc,module,"EMPABS",NULL,filenum);
		addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dmgnum);
		addDFincvir(tmprsrc,module,"EMPABTR",NULL,empabtr);
		addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,payjmst);
		addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,posmstr);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		numfields=NUMFLDS(empabtr);
		fields=FLDPOINTER(empabtr);
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(curdate!=NULL) Rfree(curdate);
			curdate=GETCURRENTDATE();
			if(!deleteflag)
			{
				ZERNRD(empabtr);
				for(x=0,field=fields;x<numfields;++x,++field)
				{
					if(!RDAstrcmp(field->name,"ENTRY DATE"))
					{
						FINDFLDSETSTRING(empabtr,"ENTRY DATE",curdate);
					} else if(!RDAstrcmp(field->name,"ENTRY TIME"))
					{
						FINDFLDSETSTRING(empabtr,"ENTRY TIME",curtime);
					} else if(!RDAstrcmp(field->name,"SOURCE USER"))
					{
						FINDFLDSETSTRING(empabtr,"SOURCE USER",USERLOGIN);
					} else if(!RDAstrcmp(field->name,"TRANSACTION NUMBER"))
					{
						COPYFIELD(filenum,empabtr,"TRANSACTION NUMBER");
					} else {
						COPYFIELD(dbsort->fileno,empabtr,field->name);
					}
				}
				ADVWRTTRANSsec(empabtr,0,NULL,NULL,EmpAbsSubData,p);
				update_diag=TRUE;
			}
			if(tmprsrc!=NULL)
				update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(curtime!=NULL) Rfree(curtime);
		if(curdate!=NULL) Rfree(curdate);
		if(p!=NULL) Rfree(p);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
}
static void save_check(RDArsrc *parent,PassableStruct *p)
{
	char *perid1=NULL,*warnmessage=NULL;
	double units=0.0;
	long size_of_file=0;

	readallwidgets(parent);
	FINDRSCGETSTRING(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1);
	if(check_perid(parent,FALSE,p)==FALSE) return;
	getsupportingrecords(parent,TRUE,p);
	FINDRSCGETDOUBLE(parent,"[EMPABS][UNITS]",&units);
	if(units==0.0)
	{
		ERRORDIALOG("INVALID UNITS!","This Employee Absentee Transaction can not be saved with an UNITS of [$  0.00].",NULL,FALSE);
		if(perid1!=NULL) Rfree(perid1);
		return;
	}
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file<=0)
	{
		warnmessage=Rmalloc(RDAstrlen(perid1)+250);
		sprintf(warnmessage,"No Job Transaction can be established for the Personnel Identification of [%s] and the record cannot be saved.  Please select a Personnel Identification with valid Jobs and FTE Amounts to save this record.",(perid1==NULL?"":perid1))
;
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO JOB TRANSACTIONS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid1!=NULL) Rfree(perid1);
		return;
	}
	if(perid1!=NULL) Rfree(perid1);
	save_record(parent,p);
}
static void save_record(RDArsrc *parent,PassableStruct *p)
{
	int x=0,lf=0;
	double units=0.0;
	char *rcddesc=NULL,deleteflag=FALSE;
	char *perid1=NULL;
	char *date=NULL,*timex=NULL;
	RDArmem *member=NULL;

	if(FINDRSCGETSTRING(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1)) return;
	rsrc2filerecord(filenum,parent);
	FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(filenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(filenum,"SOURCE USER VERIFIED",deleteflag);
	FINDFLDGETDOUBLE(filenum,"UNITS",&units);
	if((x=FINDRSC(parent,"[EMPABS][ENTRY DATE]"))!=(-1))
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
	if((x=FINDRSC(parent,"[EMPABS][ENTRY TIME]"))!=(-1))
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
	if(ADVRECORDsec(filenum,EmpAbsSubData,p))
	{
		rcddesc=ADVRECORDsecDesc(filenum,EmpAbsSubData,p);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  RECORDsec() failed without a description returned!");
		}
	} else {
		if(!WRTTRANS(filenum,1,"TRANSACTION NUMBER",NULL))
		{
			makedflist();
			select_backend(dbsort);
		} else {
			ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed, the records you attempted to save were not created. Please try again.",NULL,FALSE);
		}
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS EMPLOYEE ABSENTEES",(prevempabs->numlibs-1),
		prevempabs))
	{
		updatersrc(parent,"PREVIOUS EMPLOYEE ABSENTEES");
	}
	if(perid1!=NULL) Rfree(perid1);
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
	if(p!=NULL) Rfree(p);
	if(mbl_perdmg!=NULL) FREE_BROWSE_EXIT(mbl_perdmg);
	if(mbl_empabtr!=NULL) mbl_empabtr=NULL;
	if(filenum!=(-1)) CLSNRD(filenum);
	if(dmgnum!=(-1)) CLSNRD(dmgnum);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(empabtr!=(-1)) CLSNRD(empabtr);
	if(empadef!=(-1)) CLSNRD(empadef);
	if(posmstr!=(-1)) CLSNRD(posmstr);
	if(poslmst!=(-1)) CLSNRD(poslmst);
	if(posgrs!=(-1)) CLSNRD(posgrs);
	if(poscls!=(-1)) CLSNRD(poscls);
	if(posper!=(-1)) CLSNRD(posper);
	if(posdedc!=(-1)) CLSNRD(posdedc);
	if(possmst!=(-1)) CLSNRD(possmst);
	if(posngms!=(-1)) CLSNRD(posngms);
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(dbsort!=NULL)
	{
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
	if(prevempabs!=NULL) freeapplib(prevempabs);
	if(personnel_identification!=NULL) Rfree(personnel_identification);
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
	d=getDFvirtual(module,"EMPLOYEE JOB ABSENTEE SORT SEQUENCE");
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
		} else { 
			csortsequence->expression=NULL;
		}
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
	numfields=NUMFLDS(empabtr);
	FIELDS=FLDPOINTER(empabtr);
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
			} else if(!RDAstrcmp(field->name,"SOURCE USER"))
			{
			} else if(!strncmp(field->name,"SUBSTITUTE FINALED",18))
			{
			} else if(!strncmp(field->name,"LEAVE FINALED",13))
			{
			} else {
				addDBfield(dbsort,field->name,field->type,field->len);
			}
		}
	}
	key=KEYNUM(empabtr,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsort,keyno,part->name);
		}
	}
}
void setempabtrrecord(char *perid,int transno,int jobno,double job_units)
{
	ZERNRD(empabtr);
	FINDFLDSETSTRING(empabtr,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(empabtr,"TRANSACTION NUMBER",transno);
	FINDFLDSETINT(empabtr,"JOB NUMBER",jobno);
	FINDFLDSETDOUBLE(empabtr,"UNITS",job_units);
}
void addsortrecord(DBsort *dbsort,RDArsrc *mainrsrc,char *perid,
	int transno,int jobno,double job_units,
	double sub_units_unass,double lve_units_unass,char replacewrk,
	PassableStruct *p)
{
	char deleteflag=FALSE;

	ZERNRD(dbsort->fileno);
	FINDFLDSETCHAR(dbsort->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,EmpAbsSubData,p);
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
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",transno);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETDOUBLE(dbsort->fileno,"UNITS",job_units);
	FINDFLDSETDOUBLE(dbsort->fileno,"LEAVE UNITS UNASSIGNED",lve_units_unass);
	if(replacewrk==TRUE)
	{
		FINDFLDSETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS UNASSIGNED",sub_units_unass);
	}
	WRTNRD(dbsort->fileno,0,NULL);
}
static void computejobs(RDArsrc *parent)
{
	RDArsrc *tmprsrc=NULL;
	short ef=FALSE,dodiag=FALSE;
	char deleteflag=FALSE,*expressn=NULL;
	char *perid1=NULL,*perid2=NULL,*e=NULL;
	char curremp=FALSE,active=FALSE,replace=FALSE;
	int jobno=0,transno1=0;
	double empabtr_units=0.0,sub_units=0.0,lve_units=0.0;
	PassableStruct *p=NULL;

	FINDRSCGETSTRING(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETINT(parent,"[EMPABS][TRANSACTION NUMBER]",&transno1);
	ef=GTENRD(dbsort->fileno,1);
	while(!ef)
	{
		DELNRD(dbsort->fileno);
		ef=NXTNRD(dbsort->fileno,1);
	}
	tmprsrc=diagscrn(empabtr,"DIAGNOSTIC SCREEN",module,
		"Computing Employee Absenties Transactions",NULL);
	if(tmprsrc!=NULL)
	{
		if(!ADVmakescrn(tmprsrc,TRUE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed fo r the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	} else {
		tmprsrc=RDArsrcNEW(module,"TEMP");
	}
	addDFincvir(tmprsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(tmprsrc,module,"EMPABTR",NULL,empabtr);
	addDFincvir(tmprsrc,module,"EMPADEF",NULL,empadef);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,payjmst);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(tmprsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(tmprsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(tmprsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(tmprsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(tmprsrc,"POSTRK","POSNGMS",NULL,posngms);
	addDFincvir(tmprsrc,"POSTRK","POSSMST",NULL,possmst);
	addDFincvir(tmprsrc,"POSTRK","POSPER",NULL,posper);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=tmprsrc;
	p->a=dbsort;
	ZERNRD(dmgnum);
	FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
	if(!ADVEQLNRDsec(dmgnum,1,EmpAbsSubData,p))
	{
		dodiag=FALSE;
		FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(dmgnum,"CURRENT EMPLOYEE",&curremp);
		if(!deleteflag && curremp)
		{
			ZERNRD(payjmst);
			FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid1);
/*
			for(ef=ADVGTENRDsec(payjmst,1,EmpAbsSubData,p);!ef;ef=NXTNRDsec(payjmst,1,EmpAbsSubData,p))
*/
			ef=ADVGTENRDsec(payjmst,1,EmpAbsSubData,p);
			while(!ef)
			{
				dodiag=FALSE;
				FINDFLDGETSTRING(payjmst,"PERSONNEL IDENTIFICATION",&perid2);
				if(RDAstrcmp(perid2,perid1)) break;
				FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
				FINDFLDGETCHAR(payjmst,"ACTIVE",&active);
				if(!deleteflag && active)
				{
					COPYFIELD(payjmst,posper,"PERSONNEL IDENTIFICATION");
					COPYFIELD(payjmst,posper,"POSITION IDENTIFICATION");
					FINDFLDGETINT(payjmst,"JOB NUMBER",&jobno);
					FINDFLDSETINT(posper,"JOB NUMBER",jobno);
					if(!ADVEQLNRDsec(posper,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(posper,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(payjmst,posmstr,"POSITION IDENTIFICATION");
					if(!ADVEQLNRDsec(posmstr,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(posmstr,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(posmstr,poslmst,"LOCATION IDENTIFICATION");
					if(!ADVEQLNRDsec(poslmst,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(poslmst,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(posmstr,poscls,"CLASS IDENTIFICATION");
					if(!ADVEQLNRDsec(poscls,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(poscls,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(posmstr,posdedc,"DEDUCTION CLASS IDENTIFICATION");
					if(!ADVEQLNRDsec(posdedc,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(posdedc,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(posmstr,possmst,"SERVICE AREA IDENTIFICATION");
					if(!ADVEQLNRDsec(possmst,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(possmst,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(posmstr,posngms,"NEGOTIATING GROUP IDENTIFICATION");
					if(!ADVEQLNRDsec(posngms,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(posngms,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(payjmst,posgrs,"GROSS IDENTIFICATION");
					if(!ADVEQLNRDsec(posgrs,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(posgrs,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(payjmst,empadef,"GROSS IDENTIFICATION");
					if(!ADVEQLNRDsec(empadef,1,EmpAbsSubData,p))
					{
						FINDFLDGETCHAR(empadef,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							FINDFLDGETSTRING(empadef,"ABSENTEE UNITS TO ABSENTEE JOB UNITS FORMULA",&expressn);
							if(isEMPTY(expressn))
							{
#ifdef DIAGNOSTICS
								prterr("Skipped because ABSENTEE UNITS TO JOB UNITS FORMULA is Empty.");
#endif
							}
							empabtr_units=EVALUATEdbl(expressn,EmpAbsSubData,p);
							if(empabtr_units==0.0)
							{
#ifdef DIAGNOSTICS
								prterr("Skipped because the expression ABSENTEE UNITS TO JOB UNITS FORMULA {%s} Evaulated to Zero.",(expressn==NULL?"":expressn));
#endif
							}
							setempabtrrecord(perid1,transno1,jobno,empabtr_units);
							FINDFLDGETSTRING(empadef,"ABSENTEE JOB UNITS TO SUBSTITUTE UNITS FORMULA",&expressn);
							if(isEMPTY(expressn))
							{
#ifdef DIAGNOSTICS
								prterr("Warning: ABSENTEE JOB UNITS TO SUBSTITUTE UNITS FORMULA is Empty.");
#endif
							}
							sub_units=EVALUATEdbl(expressn,EmpAbsSubData,p);
							if(sub_units==0.0)
							{
#ifdef DIAGNOSTICS
								prterr("Warning: The expression ABSENTEE JOB UNITS TO SUBSTITUTE UNITS FORMULA {%s} Evaulated to Zero.",(expressn==NULL?"":expressn));
#endif
							}
							FINDFLDGETSTRING(empadef,"ABSENTEE JOB UNITS TO LEAVE UNITS FORMULA",&expressn);
							if(isEMPTY(expressn))
							{
#ifdef DIAGNOSTICS
								prterr("Warning: ABSENTEE UNITS TO LEAVE UNITS FORMULA is Empty.");
#endif
							}
							lve_units=EVALUATEdbl(expressn,EmpAbsSubData,p);
							if(lve_units==0.0)
							{
#ifdef DIAGNOSTICS
								prterr("Warning: The expression ABSENTEE JOB UNITS TO LEAVE UNITS FORMULA {%s} Evaulated to Zero.",(expressn==NULL?"":expressn));
#endif
							}
							FINDRSCGETCHAR(parent,"[EMPABS][REPLACEMENT WORKER]",&replace);
							addsortrecord(dbsort,parent,perid1,transno1,jobno,empabtr_units,sub_units,lve_units,replace,p);
							dodiag=TRUE;
						} else {
#ifdef DIAGNOSTICS
							prterr("Skipped by Deleted Employee Absentee Definition Master Record.");KEYNRD(empadef,1);SEENRDRECORD(empadef);
#endif
						}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Employee Absentee Definition Master reocord.");KEYNRD(empadef,1);SEENRDRECORD(empadef);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Position Gross Identification Master Record.");KEYNRD(posgrs,1);SEENRDRECORD(posgrs);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Position Gross Identification Master reocord.");KEYNRD(posgrs,1);SEENRDRECORD(posgrs);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Position Negotiating Group Identification Master Record.");KEYNRD(posngms,1);SEENRDRECORD(posngms);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Position Negotiation Group Identification Master reocord.");KEYNRD(posngms,1);SEENRDRECORD(posngms);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Position Service Area Identification Master Record.");KEYNRD(possmst,1);SEENRDRECORD(possmst);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Position Service Area Identification Master reocord.");KEYNRD(possmst,1);SEENRDRECORD(possmst);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Position Deduction Class Identification Master Record.");KEYNRD(posdedc,1);SEENRDRECORD(posdedc);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Position Deduction Class Identification Master reocord.");KEYNRD(posdedc,1);SEENRDRECORD(posdedc);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Position Class Identification Master Record.");KEYNRD(poscls,1);SEENRDRECORD(poscls);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Position Class Identification Master reocord.");KEYNRD(poscls,1);SEENRDRECORD(poscls);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Location Identification Master Record.");KEYNRD(poslmst,1);SEENRDRECORD(poslmst);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Location Identification Master reocord.");KEYNRD(poslmst,1);SEENRDRECORD(poslmst);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Position Identification Master Record.");KEYNRD(posmstr,1);SEENRDRECORD(posmstr);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Position Identification Master reocord.");KEYNRD(posmstr,1);SEENRDRECORD(posmstr);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by Deleted Position Personnel Identification Master Record.");KEYNRD(posper,1);SEENRDRECORD(posper);
#endif
					}
					} else {
#ifdef DIAGNOSTICS
						prterr("Skipped by not finding Position Personnel Identification Master reocord.");KEYNRD(posper,1);SEENRDRECORD(posper);
#endif
					}
				} else {
#ifdef DIAGNOSTICS
					prterr("Skipped because of deleted, inactive, or non-Substitute When Absent Job Master Record.");KEYNRD(payjmst,1);SEENRDRECORD(payjmst);
#endif
				}
				if(tmprsrc!=NULL)
				{
					update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
				}
				ef=ADVNXTNRDsec(payjmst,1,EmpAbsSubData,p);
			}
		} else {
#ifdef DIAGNOSTICS
			prterr("Skipped because of deleted, inactive, or non-Current Employee Personnel Identification Demographics Master Record.");KEYNRD(dmgnum,1);SEENRDRECORD(dmgnum);
#endif
		}
	} else {
#ifdef DIAGNOSTICS
		prterr("Skipped by not finding Personnel Identification Demographics Master reocord.");KEYNRD(dmgnum,1);SEENRDRECORD(dmgnum);
#endif
	}
	if(p!=NULL) Rfree(p);
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(expressn!=NULL) Rfree(expressn);
/*
	updatebrowse(TRUE,mbl_empabtr,dbsort->fileno,module,parent);
*/
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
}
static void changeper(RDArsrc *parent,short override,short update,PassableStruct *p)
{
	char *perid1=NULL,replace=FALSE;
	double units1=0.0;

	if(FINDRSCGETSTRING(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(FINDRSCGETDOUBLE(parent,"[EMPABS][UNITS]",&units1)) return;
	if(FINDRSCGETCHAR(parent,"[EMPABS][REPLACEMENT WORKER]",&replace)) return;
	if(RDAstrcmp(perid1,personnel_identification) || 
		units1!=absentee_units || replace!=replacement_worker || override)
	{
		computejobs(parent);
		if(personnel_identification!=NULL) Rfree(personnel_identification);
		personnel_identification=stralloc(perid1);
		absentee_units=units1;
		replacement_worker=replace;
		getsupportingrecords(parent,update,p);
	}
	if(perid1!=NULL) Rfree(perid1);
}
void browse_perdmg(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(parent);
	if(mbl_perdmg==NULL)
	{
		mainrsrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE PERSONNEL BROWSE");
		searchrsrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE PERSONNEL SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE PERSONNEL DEFINE LIST");
		addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
		addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_perdmg=ADVSbrowse(dmgnum,1,mainrsrc,searchrsrc,definelist,
			NULL,NULL,selectfunction,NULL,okfunc,NULL,
			p,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_perdmg->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_perdmg);
		}
	}
}
static short selectfunction(MakeBrowseList *blist)
{
	char delflag=FALSE;

	FINDFLDGETCHAR(blist->fileno,"DELETEFLAG",&delflag);
	return((short)!delflag);
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
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	char *perid1=NULL;
	PassableStruct *p;

	p=(PassableStruct *)b->funcargs;
	ZERNRD(dmgnum);
	if(ADVEQLNRDKEYsec(dmgnum,1,targetkey,EmpAbsSubData,p))
	{
		KEYNRD(dmgnum,1);
	} else {
		FINDFLDGETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",&perid1);
	}
	FINDRSCSETSTRING(p->r,"[EMPABS][PERSONNEL IDENTIFICATION]",perid1);
	rsrc2filerecord(filenum,p->r);
	if(perid1!=NULL) Rfree(perid1);
	changeper(p->r,FALSE,TRUE,p);
	computeallSCRNvirtuals(p->r);
	updateallrsrc(p->r);
}
static void browse_empabtr(RDArsrc *parent,PassableStruct *p)
{
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

        readallwidgets(parent);
        rsrc2filerecord(filenum,parent);
        getsupportingrecords(parent,TRUE,p);
	brsrc=RDArsrcNEW(module,"ADD EMPLOYEE JOB ABSENTEE BROWSE");
	drsrc=RDArsrcNEW(module,"ADD EMPLOYEE JOB ABSENTEE DEFINE LIST");
	srsrc=RDArsrcNEW(module,"ADD EMPLOYEE JOB ABSENTEE SEARCH BROWSE");
	addDFincvir(brsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(brsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(drsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(drsrc,module,"EMPABTR",NULL,empabtr);
	addDFincvir(drsrc,module,"EMPADEF",NULL,empadef);
	addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(drsrc,"PAYROLL","PAYJMST",NULL,payjmst);
	addDFincvir(drsrc,"POSTRK","POSPER",NULL,posper);
	addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(drsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(drsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(drsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(drsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(drsrc,"POSTRK","POSSMST",NULL,possmst);
	addDFincvir(drsrc,"POSTRK","POSNGMS",NULL,posngms);
	addDFincvir(srsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(srsrc,module,"EMPABTR",NULL,empabtr);
	addDFincvir(srsrc,module,"EMPADEF",NULL,empadef);
	addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(srsrc,"PAYROLL","PAYJMST",NULL,payjmst);
	addDFincvir(srsrc,"POSTRK","POSPER",NULL,posper);
	addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(srsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(srsrc,"POSTRK","POSGRS",NULL,posgrs);
	addDFincvir(srsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(srsrc,"POSTRK","POSDEDC",NULL,posdedc);
	addDFincvir(srsrc,"POSTRK","POSSMST",NULL,possmst);
	file2rsrc(filenum,brsrc,FALSE);
	filerecord2rsrc(filenum,brsrc);
	file2rsrc(dmgnum,brsrc,FALSE);
	filerecord2rsrc(dmgnum,brsrc);
	GET_SCREEN_VIRTUALS(brsrc,0);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	mbl_empabtr=ADVSbrowse(dbsort->fileno,1,brsrc,srsrc,drsrc,NULL,
		NULL,selectfunction,doexit1,NULL,NULL,NULL,TRUE,NULL,
		0,0,0,TRUE);
	FINDRSCSETEDITABLE(brsrc,"SEARCH BUTTON",FALSE);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	char *sortname=NULL,*name=NULL,*filename=NULL;
	int transno=1;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL;
	PassableStruct *p=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"ADD EMPLOYEE ABSENTEE")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if(argc>2)
	{
		audit_reportname=stralloc(argv[1]);
	} else {
		audit_reportname=stralloc("EAAUDIT");
	}
	if((filenum=APPOPNNRD(module,"EMPABS",TRUE,TRUE))==(-1)) return;
	if((empabtr=APPOPNNRD(module,"EMPABTR",TRUE,TRUE))==(-1)) return;
	if((empadef=APPOPNNRD(module,"EMPADEF",TRUE,TRUE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payjmst=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((poslmst=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	if((posgrs=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((poscls=APPOPNNRD("POSTRK","POSCLS",TRUE,FALSE))==(-1)) return;
	if((posper=APPOPNNRD("POSTRK","POSPER",TRUE,FALSE))==(-1)) return;
	if((posdedc=APPOPNNRD("POSTRK","POSDEDC",TRUE,FALSE))==(-1)) return;
	if((possmst=APPOPNNRD("POSTRK","POSSMST",TRUE,FALSE))==(-1)) return;
	if((posngms=APPOPNNRD("POSTRK","POSNGMS",TRUE,FALSE))==(-1)) return;
	filename=FILENAME(filenum);
	nokeys=NUMKEYS(filenum);
	keys=KEYPOINTER(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	parent=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE");
	definelist=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE DEFINE LIST");
	addDFincvir(parent,module,"EMPABS",NULL,filenum);
	addDFincvir(parent,module,"EMPABTR",NULL,empabtr);
	addDFincvir(parent,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(definelist,module,"EMPABS",NULL,filenum);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
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
			if(!RDAstrcmp(fldx->name,"PERSONNEL IDENTIFICATION"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					file2rsrc(dmgnum,parent,FALSE);
					addstdrsrc(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"[EMPABS][PERSONNEL IDENTIFICATION]",check_peridcb,p);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,filenum,fldx,edit_rsrc);
                                if(name!=NULL)
                                {
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				}
				sprintf(name,"[%s][%s]",filename,fldx->name);
				FINDRSCSETFUNC(parent,name,setfiles,p);
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(parent,"[EMPABS][UNITS]",check_unitscb,p);
	FINDRSCSETFUNC(parent,"[EMPABS][REPLACEMENT WORKER]",check_unitscb,p);
	prevempabs=APPlibNEW();
	addAPPlib(prevempabs,"No Employee Absentees added this session");
	addlstrsrc(parent,"PREVIOUS EMPLOYEE ABSENTEES",&x,TRUE,NULL,prevempabs->numlibs,
		&prevempabs->libs,NULL);
	FINDRSCSETSTRING(parent,"[EMPABS][ENTRY DATE]",CURRENT_DATE);
	FINDRSCSETSTRING(parent,"[EMPABS][ENTRY TIME]",CURRENT_TIME);
	savelist(definelist,p);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	addbtnrsrc(parent,"BROWSE PERSONNEL",TRUE,browse_perdmg,p);
	addbtnrsrc(parent,"BROWSE EMPLOYEE JOB ABSENTEES",TRUE,browse_empabtr,p);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"DEFINE PREVIOUS LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,reset_defaults,p);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,p);
	addrfcbrsrc(parent,"QUIT",TRUE,quit_check,p);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	FINDFLDSETINT(filenum,"TRANSACTION NUMBER",transno);
	FINDRSCSETINT(parent,"[EMPABS][TRANSACTION NUMBER]",transno);
	set_defaults(parent,FALSE,p);
	APPmakescrn(parent,FALSE,quit_record_freens,p,TRUE);
	RDAAPPMAINLOOP();
}
