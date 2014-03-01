/* empaabsw.c - Add Employee Absentee Substitute Worked*/
#ifndef WIN32
#define __NAM__ "empaabsw.lnx"
#endif
#ifdef WIN32
#define __NAM__ "empaabsw.exe"
#endif
#include <app.hpp>
#include <cstdlib>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
/*CPP_OPART empaabw1 */
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

/*
extern MakeBrowseList *mbl_empabtr;
*/
extern void browse_empabtr(RDArsrc *,PassableStruct *);
short filenum=(-1),empabtr=(-1),empadef=(-1);
short dmgnum=(-1),payjmst=(-1),posmstr=(-1),poslmst=(-1);
short posgrs=(-1),poscls=(-1),posper=(-1),posdedc=(-1);
short possmst=(-1),posngms=(-1);
short subdmgnum=(-1),submstr=(-1),subjmstnum=(-1);
short subwrk=(-1),subemp=(-1),subpos=(-1),subloc=(-1);
char *module="EMPABS";
DBsort *dbsort=NULL;
RDArsrc *mainrsrc=NULL;
void getsupportingrecords(RDArsrc *,short,PassableStruct *);
MakeBrowseList *mbl_submstr=NULL;
static void test_empabtr(RDArsrc *,PassableStruct *);
static void browse_submstr(RDArsrc *,PassableStruct *);
static short sub_selectfunc(MakeBrowseList *);
static void okfunc2(MakeBrowseList *,void *);
void EmpAbsSubData(char **,PassableStruct *);
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_perdmg=NULL;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static short selectfunction(MakeBrowseList *);
static RDArsrc *definelist=NULL;
static APPlib *prevempabs=NULL;
static APPlib *subjobnolist=NULL;
static int counter=0;
static void set_defaults(RDArsrc *,short,PassableStruct *);
static void reset_defaults(RDArsrc *,PassableStruct *);
static RDAfinmgt *fmgt=NULL;
static void computejobs(RDArsrc *);
static void select_backend(DBsort *);
static void savelist(RDArsrc *,PassableStruct *),savelistcb(RDArsrc *,PassableStruct *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *,PassableStruct *),quit_record(RDArsrc *,PassableStruct *);
static void quit_record_free(RDArsrc *,PassableStruct *),quit_record_freens(RDArsrc *,PassableStruct *);
static void save_check(RDArsrc *,PassableStruct *),quit_check(RDArsrc *,PassableStruct *);
static short check_perid(RDArsrc *,short,PassableStruct *);
static void check_peridcb(RDArsrc *,PassableStruct *);
static void check_unitscb(RDArsrc *,PassableStruct *);
static void changeper(RDArsrc *,short,short,PassableStruct *);
static void changesub(RDArsrc *,short,PassableStruct *);
static void changesubjobno(RDArsrc *,short,PassableStruct *);
static void check_subjobnocb(RDArsrc *,PassableStruct *);
static void browse_perdmg(RDArsrc *,PassableStruct *);
static void browse_submstrcb(RDArsrc *,PassableStruct *);
static char *personnel_identification=NULL;
static char *subpersonnel_identification=NULL;
static int subjob_number=(-1);
static double absentee_units=0.0;
static char *audit_reportname=NULL;
static void setfiles(RDArsrc *,PassableStruct *);

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
void definelistfunc(RDArsrc *mainrsrc)
{
	updateSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent,PassableStruct *p)
{
	set_defaults(parent,TRUE,p);
}
static void set_defaults(RDArsrc *mainrsrc,short update,PassableStruct *p)
{
	int selprev=0;
	char *perid1=NULL;

	FINDRSCGETINT(mainrsrc,"PREVIOUS EMPLOYEE ABSENTEES",&selprev);
	DefaultResetScreen(mainrsrc,TRUE);
	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
/*
	changeper(mainrsrc,TRUE,update,p);
*/
	changesub(mainrsrc,TRUE,p);
	if(perid1!=NULL) Rfree(perid1);
	computeallSCRNvirtuals(mainrsrc);
	updateallrsrc(mainrsrc);
}
static void check_peridcb(RDArsrc *mainrsrc,PassableStruct *p)
{
	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
	check_perid(mainrsrc,FALSE,p);
	changeper(mainrsrc,FALSE,TRUE,p);
	updateSCRNvirtuals(mainrsrc);
}
static void check_unitscb(RDArsrc *mainrsrc,PassableStruct *p)
{
	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
	changeper(mainrsrc,FALSE,TRUE,p);
	updateSCRNvirtuals(mainrsrc);
}
static short check_perid(RDArsrc *mainrsrc,short updatesupporting,PassableStruct *p)
{
	char *perid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1);
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
			getsupportingrecords(mainrsrc,updatesupporting,p);
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
				getsupportingrecords(mainrsrc,updatesupporting,p);
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
					getsupportingrecords(mainrsrc,updatesupporting,p);
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
	getsupportingrecords(mainrsrc,updatesupporting,p);
	if(perid1!=NULL) Rfree(perid1);
	return(TRUE);
}
static short check_subid(RDArsrc *mainrsrc,short updatesupporting,PassableStruct *p)
{
	char *subperid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(subdmgnum!=(-1))
	{
		ZERNRD(subdmgnum);
		FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",subperid1);
		if(ADVEQLNRDsec(subdmgnum,1,EmpAbsSubData,p))
		{
			KEYNRD(subdmgnum,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is an invalid value.  It does not exist in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record.",
				(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(mainrsrc,updatesupporting,p);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(subdmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is a deleted value in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record."
					,(subperid1==NULL?"" :subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(mainrsrc,updatesupporting,p);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(subdmgnum,"CURRENT EMPLOYEE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
					sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is not a CURRENT EMPLOYEE in the [PRSNNL][PERDMG] data file.  Please select a valid Substitute Personnel Identification to save this record."
						,(subperid1==NULL?"":subperid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","NOT CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(mainrsrc,updatesupporting,p);
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
		if(ADVEQLNRDsec(submstr,1,EmpAbsSubData,p))
		{
			KEYNRD(submstr,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is an invalid value.  It does not exist in the [SUBMGT][SUBMASTR] data file.  Please select a valid Substitute Personnel Identification to save this record."
				,(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(mainrsrc,updatesupporting,p);
			if(subperid1!=NULL) Rfree(subperid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(submstr,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
				sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is a deleted value in the [SUBMGT][SUBMASTR] data file.   Please select a valid Substitute Personnel Identification to save this record."
					,(subperid1==NULL?"":subperid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(mainrsrc,updatesupporting,p);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(submstr,"ACTIVE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
					sprintf(warnmessage,"The Substitute Personnel Identification of [%s] is not ACTIVE in the [SUBMGT][SUBMASTR] data file.   Please select a valid Substitute Personnel Identification to save this record."
						,(subperid1==NULL?"":subperid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED SUBSTITUTE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(mainrsrc,updatesupporting,p);
					if(subperid1!=NULL) Rfree(subperid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	getsupportingrecords(mainrsrc,updatesupporting,p);
	if(subperid1!=NULL) Rfree(subperid1);
	return(TRUE);
}
static void changesub(RDArsrc *mainrsrc,short update,PassableStruct *p)
{
	short ef=0;
	short keyn=0,keynumber=0;
	char active=TRUE,deleteflag=FALSE;
	char *e=NULL;
	char *subperid1=NULL,*subperid2=NULL,*subjobnostr=NULL;
	int selsubjobno=0,subjobno1=(-1);
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
/*
	if(check_subid(mainrsrc,TRUE,p)==FALSE)
	{
		if(subperid1!=NULL) Rfree(subperid1);
		return;
	}
*/
	if(RDAstrcmp(subperid1,subpersonnel_identification) || subpersonnel_identification==NULL)
	{
		getsupportingrecords(mainrsrc,update,p);
			if(subjobnolist!=NULL) freeapplib(subjobnolist);
			subjobnolist=APPlibNEW();
			FINDRSCGETINT(mainrsrc,"SUBSTITUTE JOB NUMBERS",&selsubjobno);
			if((keyn=KEYNUMBER(subjmstnum,"JOBMSTR KEY"))!=(-1))
			{
				keynumber=keyn;
			} else {
				keynumber=1;
			}
			ZERNRD(subjmstnum);
			FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",subperid1);
			ef=ADVGTENRDsec(subjmstnum,keynumber,EmpAbsSubData,p);
			e=stralloc("Updating Substitute Job Numbers List");
			tmprsrc=diagscrn(subjmstnum,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
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
				ef=ADVNXTNRDsec(subjmstnum,keynumber,EmpAbsSubData,p);
				if(tmprsrc!=NULL)
					update_diagnostic(tmprsrc,listupdated);
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			if(subjobnolist->numlibs<1) addAPPlib(subjobnolist,"No Jobs Available");
			if(selsubjobno>=subjobnolist->numlibs) selsubjobno=0;
			if(!FINDRSCLISTAPPlib(mainrsrc,"SUBSTITUTE JOB NUMBERS",selsubjobno,subjobnolist))
			{
				updatersrc(mainrsrc,"SUBSTITUTE JOB NUMBERS");
			}
			changesubjobno(mainrsrc,update,p);
/*
		if(!initial) compute_sub_units(mainrsrc);
		if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
		subpersonnel_identification=stralloc(subperid1);
*/
	}
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	if(subperid1!=NULL) Rfree(subperid1);
	if(subperid2!=NULL) Rfree(subperid2);
	changeper(mainrsrc,update,update,p);
}
static void check_subidcb(RDArsrc *mainrsrc,PassableStruct *p)
{
	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
	if(check_subid(mainrsrc,TRUE,p)==FALSE)
	{
		return;
	}
	changesub(mainrsrc,FALSE,p);
	updateSCRNvirtuals(mainrsrc);
}
static short check_subjobno(RDArsrc *mainrsrc,short updatesupporting,PassableStruct *p)
{
	char *subperid1=NULL,*warnmessage=NULL;
	int subjobno1=(-1);
	char delflag=FALSE;

	FINDRSCGETINT(mainrsrc,"SUBSTITUTE JOB NUMBER",&subjobno1);
	FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
	if(subjmstnum!=(-1))
	{
		ZERNRD(subjmstnum);
		FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",subperid1);
		FINDFLDSETINT(subjmstnum,"JOB NUMBER",subjobno1);
		if(ADVEQLNRDsec(subjmstnum,1,EmpAbsSubData,p))
		{
			KEYNRD(subjmstnum,1);
			warnmessage=Rmalloc(RDAstrlen(subperid1)+300);
			sprintf(warnmessage,"The Job Number of [%8d] is an invalid value for the Substitute Personnel Identification of [%s].  Please select a valid Job Number to save this record.",subjobno1,(subperid1==NULL?"":subperid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUBSTITUTE JOBNUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(mainrsrc,updatesupporting,p);
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
				getsupportingrecords(mainrsrc,updatesupporting,p);
				if(subperid1!=NULL) Rfree(subperid1);
				return(FALSE);
			}
		}
	} else {
		if(subperid1!=NULL) Rfree(subperid1);
		return(FALSE);
	}
	getsupportingrecords(mainrsrc,updatesupporting,p);
	if(subperid1!=NULL) Rfree(subperid1);
	return(TRUE);
}
static void check_subjobnocb(RDArsrc *mainrsrc,PassableStruct *p)
{
	char *subperid1=NULL,*subjobnostr=NULL;
	int x=0,subjobno1=(-1);

	readwidget(mainrsrc,"SUBSTITUTE JOB NUMBER");
	readwidget(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION");
	FINDRSCGETINT(mainrsrc,"SUBSTITUTE JOB NUMBER",&subjobno1);
	subjobnostr=Rmalloc(7);
	sprintf(subjobnostr,"%6d",subjobno1);
	FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
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
	FINDRSCSETINT(mainrsrc,"SUBSTITUTE JOB NUMBERS",x);
	subjobno1=Ratoi(subjobnostr);
	FINDRSCSETINT(mainrsrc,"SUBSTITUTE JOB NUMBER",subjobno1);
	updatersrc(mainrsrc,"SUBSTITUTE JOB NUMBERS");
	updatersrc(mainrsrc,"SUBSTITUTE JOB NUMBER");
	getsupportingrecords(mainrsrc,TRUE,p);
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	if(subperid1!=NULL) Rfree(subperid1);
	updateSCRNvirtuals(mainrsrc);
}
static void changesubjobno(RDArsrc *mainrsrc,short update,PassableStruct *p)
{
	char deleteflag=FALSE,*subperid1=NULL,*subjobnostr=NULL;
	int subjobno1=(-1),selsubjobno=0;
	short ef=FALSE;

	if(FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1)) return;
	FINDRSCGETINT(mainrsrc,"SUBSTITUTE JOB NUMBERS",&selsubjobno);
	if((RDAstrcmp(subjobnolist->libs[selsubjobno],"No Jobs Available")) &&
		(RDAstrcmp(subjobnolist->libs[selsubjobno],"Not Applicable")))
	{
		subjobnostr=stralloc(subjobnolist->libs[selsubjobno]);
	}
	subjobno1=Ratoi(subjobnostr);
	if(subjobnostr!=NULL) Rfree(subjobnostr);
	FINDRSCSETINT(mainrsrc,"SUBSTITUTE JOB NUMBER",subjobno1);
	updatersrc(mainrsrc,"SUBSTITUTE JOB NUMBER");
	getsupportingrecords(mainrsrc,update,p);
	if(dbsort!=NULL)
	{
		ef=BEGNRD(dbsort->fileno);
		while(!ef)
		{
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDSETINT(dbsort->fileno,"SUBSTITUTE JOB NUMBER",subjobno1);
				WRTNRD(dbsort->fileno,0,NULL);
			}
			ef=SEQNRD(dbsort->fileno);
		}
	}
	if(subperid1!=NULL) Rfree(subperid1);
}
static void changesubjobnocb(RDArsrc *mainrsrc,PassableStruct *p)
{
	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
	changesubjobno(mainrsrc,TRUE,p);
	updateSCRNvirtuals(mainrsrc);
}
static void okfunc2(MakeBrowseList *b,void *targetkey)
{
	char *perid1=NULL;
	PassableStruct *p=NULL;

	p=(PassableStruct *)b->funcargs;
	ZERNRD(submstr);
	if(ADVEQLNRDKEYsec(submstr,1,targetkey,EmpAbsSubData,p))
	{
		KEYNRD(submstr,1);
	} else {
		FINDFLDGETSTRING(submstr,"PERSONNEL IDENTIFICATION",&perid1);
	}
	FINDRSCSETSTRING(p->r,"SUBSTITUTE PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(filenum,"SUBSTITUTE PERSONNEL IDENTIFICATION",perid1);
	changesub(p->r,FALSE,p);
	computeallSCRNvirtuals(p->r);
	updateallrsrc(p->r);
	if(perid1!=NULL) Rfree(perid1);
}
static void browse_submstrcb(RDArsrc *mainrsrc,PassableStruct *p)
{
	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
	browse_submstr(mainrsrc,p);
}
static void browse_submstr(RDArsrc *rsrc,PassableStruct *p)
{
	RDArsrc *browsersrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_submstr==NULL)
	{
		browsersrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUBSTITUTE BROWSE");
		searchrsrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUBSTITUTE SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUBSTITUTE DEFINE LIST");
		addDFincvir(searchrsrc,"SUBMGT","SUBMASTR",NULL,submstr);
		addDFincvir(definelist,"SUBMGT","SUBMASTR",NULL,submstr);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_submstr=ADVSbrowse(submstr,1,browsersrc,searchrsrc,definelist,
			NULL,NULL,sub_selectfunc,NULL,okfunc2,NULL,
			p,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_submstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_submstr);
		}
	}
}
void getsupportingrecords(RDArsrc *mainrsrc,short updatesupporting,PassableStruct *p)
{
	char *temp=NULL;
	int num=0;

	if(dmgnum!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",&temp);
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",temp);
		if(ADVEQLNRDsec(dmgnum,1,EmpAbsSubData,p)) KEYNRD(dmgnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(dmgnum,mainrsrc);
		} else {
			filerecord2rsrc(dmgnum,mainrsrc);
		}
	}
	if(subdmgnum!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
		FINDFLDSETSTRING(subdmgnum,"PERSONNEL IDENTIFICATION",temp);
		if(ADVEQLNRDsec(subdmgnum,1,EmpAbsSubData,p)) KEYNRD(subdmgnum,1);
		if(updatesupporting)
		{
			updatesinglerecord2rsrc(subdmgnum,mainrsrc);
		} else {
			singlefilerecord2rsrc(subdmgnum,mainrsrc);
		}
	}
	if(submstr!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
		FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",temp);
		if(ADVEQLNRDsec(submstr,1,EmpAbsSubData,p)) KEYNRD(submstr,1);
		if(updatesupporting)
		{
			updatesinglerecord2rsrc(submstr,mainrsrc);
		} else {
			singlefilerecord2rsrc(submstr,mainrsrc);
		}
	}
	if(subjmstnum!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&temp);
		FINDRSCGETINT(mainrsrc,"SUBSTITUTE JOB NUMBER",&num);
		FINDFLDSETSTRING(subjmstnum,"PERSONNEL IDENTIFICATION",temp);
		FINDFLDSETINT(subjmstnum,"JOB NUMBER",num);
		if(ADVEQLNRDsec(subjmstnum,1,EmpAbsSubData,p)) KEYNRD(subjmstnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(subjmstnum,mainrsrc);
		} else {
			filerecord2rsrc(subjmstnum,mainrsrc);
		}
        }
	if(temp!=NULL) Rfree(temp);
}
static void select_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE;
	char deleteflag=FALSE;
	char *curdate=NULL,*curtime=NULL,*e=NULL,*curdate10=NULL;
	double total_sub_units=0.0,sub_units_unass=0.0;
	double sub_units=0.0,acc_units=0.0;
	int times_acc=0,x,numfields=0;
	char *perid=NULL,*lperid=NULL;
	int transno=0,jobno=0,ltransno=0,ljobno=0;
        NRDfield *fields=NULL,*field=NULL,*fldx=NULL;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev_file=NULL;
	PassableStruct *p=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Creating Actual Job Absentees",NULL);
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
		addDFincvir(tmprsrc,module,"EMPABS",NULL,filenum);
		addDFincvir(tmprsrc,module,"EMPABTR",NULL,empabtr);
		addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dmgnum);
		addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,payjmst);
		addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,posmstr);
		addDFincvir(tmprsrc,"POSTRK","POSPER",NULL,posper);
		addDFincvir(tmprsrc,"SUBMGT","SUBMASTR",NULL,submstr);
		addDFincvir(tmprsrc,"SUBMGT","SUBWKMAS",NULL,subwrk);
		addDFincvir(tmprsrc,"SUBMGT","SUBPOS",NULL,subpos);
		addDFincvir(tmprsrc,"SUBMGT","SUBEMP",NULL,subemp);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		numfields=NUMFLDS(empabtr);
		fields=FLDPOINTER(empabtr);
		ef=FRSNRD(dbsort->fileno,1);
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&lperid); 
		FINDFLDGETINT(dbsort->fileno,"TRANSACTION NUMBER",&ltransno); 
		FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&ljobno); 
		while(!ef)
		{
			SEENRDRECORD(dbsort->fileno);TRACE;
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(curdate10!=NULL) Rfree(curdate10);
			curdate10=GETCURRENTDATE10();
			if(curdate!=NULL) Rfree(curdate);
			curdate=GETCURRENTDATE();
			if(!deleteflag)
			{
				FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&perid); 
				FINDFLDGETINT(dbsort->fileno,"TRANSACTION NUMBER",&transno); 
				FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&jobno); 
				if(!RDAstrcmp(perid,lperid) && transno==ltransno && 
					jobno==ljobno)
				{
					ZERNRD(empabtr);
					for(x=0,field=fields;x<numfields;++x,++field)
					{
						if(!RDAstrcmp(field->name,"TRANSACTION NUMBER"))
						{
							COPYFIELD(filenum,empabtr,"TRANSACTION NUMBER");
						} else {
							COPYFIELD(dbsort->fileno,empabtr,field->name);
						}
					}
					FINDFLDGETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS",&sub_units); 
					total_sub_units+=sub_units;
					FINDFLDGETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS UNASSIGNED",&sub_units_unass);
				} else {
					if(total_sub_units>=sub_units_unass)
					{
						FINDFLDSETCHAR(empabtr,"SUBSTITUTE FINALED",TRUE);
						FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED BY",USERLOGIN);
						fldx=FLDNRD(empabtr,"SUBSTITUTE FINALED DATE");
						if(fldx!=NULL)
						{
							if(fldx->len>8)
							{
								FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED DATE",curdate10);
							} else {
								FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED DATE",curdate);
							}
						} else {
							FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED DATE",curdate);
						}
						FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED TIME",curtime);
					}
					FINDFLDSETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",(sub_units_unass-total_sub_units));
					FINDFLDSETSTRING(empabtr,"SOURCE USER",USERLOGIN);
					fldx=FLDNRD(empabtr,"ENTRY DATE");
					if(fldx!=NULL)
					{
						if(fldx->len>8)
						{
							FINDFLDSETSTRING(empabtr,"ENTRY DATE",curdate10);
						} else FINDFLDSETSTRING(empabtr,"ENTRY DATE",curdate);
					}
					FINDFLDSETSTRING(empabtr,"ENTRY TIME",curtime);
					ADVWRTTRANSsec(empabtr,0,NULL,NULL,EmpAbsSubData,p);
					total_sub_units=0.0;
					FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&lperid); 
					FINDFLDGETINT(dbsort->fileno,"TRANSACTION NUMBER",&ltransno); 
					FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&ljobno); 
					FINDFLDGETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS",&sub_units); 
					total_sub_units+=sub_units;
				}
				ZERNRD(subwrk);
				FIELDCOPY(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",subwrk,"PERSONNEL IDENTIFICATION"); 
				FIELDCOPY(dbsort->fileno,"SUBSTITUTE JOB NUMBER",subwrk,"JOB NUMBER"); 
				FIELDCOPY(dbsort->fileno,"PERSONNEL IDENTIFICATION",subwrk,"EMPLOYEE IDENTIFICATION"); 
				FIELDCOPY(dbsort->fileno,"JOB NUMBER",subwrk,"EMPLOYEE JOB NUMBER"); 
				FIELDCOPY(filenum,"TRANSACTION NUMBER",subwrk,"EMPLOYEE TRANSACTION NUMBER"); 
				COPYFIELD(dbsort->fileno,subwrk,"POSITION IDENTIFICATION"); 
				FINDFLDGETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS",&sub_units); 
				FINDFLDSETDOUBLE(subwrk,"UNITS",sub_units); 
/*
				COPYFIELD(dbsort->fileno,subwrk,"STARTING DATE"); 
				COPYFIELD(dbsort->fileno,subwrk,"ENDING DATE"); 
*/
				COPYFIELD(filenum,subwrk,"STARTING DATE"); 
				COPYFIELD(filenum,subwrk,"ENDING DATE"); 
				fldx=FLDNRD(subwrk,"ENTRY DATE");
				if(fldx!=NULL)
				{
					if(fldx->len>8)
					{
						FINDFLDSETSTRING(subwrk,"ENTRY DATE",curdate10);
					} else FINDFLDSETSTRING(subwrk,"ENTRY DATE",curdate);
				}
				FINDFLDSETSTRING(subwrk,"ENTRY TIME",curtime);
				FINDFLDSETSTRING(subwrk,"SOURCE USER",USERLOGIN);
				ADVWRTTRANSsec(subwrk,1,"TRANSACTION NUMBER",NULL,EmpAbsSubData,p);
				ZERNRD(submstr);
				FIELDCOPY(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",submstr,"PERSONNEL IDENTIFICATION"); 
				if(!ADVEQLNRDsec(submstr,1,EmpAbsSubData,p))
				{
					prev_file=RDATDataNEW(submstr);
					FINDFLDGETINT(submstr,"TIMES ACCEPTED",&times_acc);
					times_acc+=1;
					FINDFLDSETINT(submstr,"TIMES ACCEPTED",times_acc);
					FINDFLDGETDOUBLE(submstr,"ACCEPTED UNITS",&acc_units);
					acc_units+=sub_units;
					FINDFLDSETDOUBLE(submstr,"ACCEPTED UNITS",acc_units);
					if(!ADVWRTTRANSsec(submstr,0,NULL,prev_file,EmpAbsSubData,p))
					{
						if(prev_file!=NULL) FreeRDATData(prev_file);
						ZERNRD(subloc);
						FIELDCOPY(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",subloc,"PERSONNEL IDENTIFICATION"); 
						COPYFIELD(dbsort->fileno,subloc,"LOCATION IDENTIFICATION"); 
						if(!ADVEQLNRDsec(subloc,1,EmpAbsSubData,p))
						{
							prev_file=RDATDataNEW(subloc);
							FINDFLDGETINT(subloc,"TIMES ACCEPTED",&times_acc);
							times_acc+=1;
							FINDFLDSETINT(subloc,"TIMES ACCEPTED",times_acc);
							FINDFLDGETDOUBLE(subloc,"ACCEPTED UNITS",&acc_units);
							acc_units+=sub_units;
							FINDFLDSETDOUBLE(subloc,"ACCEPTED UNITS",acc_units);
							ADVWRTTRANSsec(subloc,0,NULL,prev_file,EmpAbsSubData,p);
							if(prev_file!=NULL) FreeRDATData(prev_file);
						} else {
							times_acc=1;
							FINDFLDSETINT(subloc,"TIMES ACCEPTED",times_acc);
							FINDFLDSETDOUBLE(subloc,"ACCEPTED UNITS",sub_units);
							ADVWRTTRANSsec(subloc,0,NULL,NULL,EmpAbsSubData,p);
						}
						ZERNRD(subpos);
						FIELDCOPY(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",subpos,"PERSONNEL IDENTIFICATION"); 
						COPYFIELD(dbsort->fileno,subpos,"POSITION IDENTIFICATION"); 
						if(!ADVEQLNRDsec(subpos,1,EmpAbsSubData,p))
						{
							prev_file=RDATDataNEW(subpos);
							FINDFLDGETINT(subpos,"TIMES ACCEPTED",&times_acc);
							times_acc+=1;
							FINDFLDSETINT(subpos,"TIMES ACCEPTED",times_acc);
							FINDFLDGETDOUBLE(subpos,"ACCEPTED UNITS",&acc_units);
							acc_units+=sub_units;
							FINDFLDSETDOUBLE(subpos,"ACCEPTED UNITS",acc_units);
							ADVWRTTRANSsec(subpos,0,NULL,prev_file,EmpAbsSubData,p);
							if(prev_file!=NULL) FreeRDATData(prev_file);
						} else {
							times_acc=1;
							FINDFLDSETINT(subpos,"TIMES ACCEPTED",times_acc);
							FINDFLDSETDOUBLE(subpos,"ACCEPTED UNITS",sub_units);
							ADVWRTTRANSsec(subpos,0,NULL,NULL,EmpAbsSubData,p);
						}
						ZERNRD(subemp);
						FIELDCOPY(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",subemp,"PERSONNEL IDENTIFICATION"); 
						FIELDCOPY(dbsort->fileno,"PERSONNEL IDENTIFICATION",subemp,"EMPLOYEE IDENTIFICATION"); 
						if(!ADVEQLNRDsec(subemp,1,EmpAbsSubData,p))
						{
							prev_file=RDATDataNEW(subemp);
							FINDFLDGETINT(subemp,"TIMES ACCEPTED",&times_acc);
							times_acc+=1;
							FINDFLDSETINT(subemp,"TIMES ACCEPTED",times_acc);
							FINDFLDGETDOUBLE(subemp,"ACCEPTED UNITS",&acc_units);
							acc_units+=sub_units;
							FINDFLDSETDOUBLE(subemp,"ACCEPTED UNITS",acc_units);
							ADVWRTTRANSsec(subemp,0,NULL,prev_file,EmpAbsSubData,p);
							if(prev_file!=NULL) FreeRDATData(prev_file);
						} else {
							times_acc=1;
							FINDFLDSETINT(subemp,"TIMES ACCEPTED",times_acc);
							FINDFLDSETDOUBLE(subemp,"ACCEPTED UNITS",sub_units);
							ADVWRTTRANSsec(subemp,0,NULL,NULL,EmpAbsSubData,p);
						}
					}
					if(prev_file!=NULL) FreeRDATData(prev_file);
				}
				update_diag=TRUE;
			}
			if(tmprsrc!=NULL)
				update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(!RDAstrcmp(perid,lperid) && transno==ltransno && 
			jobno==ljobno)
		{
			if(total_sub_units>=sub_units_unass)
			{
				FINDFLDSETCHAR(empabtr,"SUBSTITUTE FINALED",TRUE);
				FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED BY",USERLOGIN);
				fldx=FLDNRD(empabtr,"SUBSTITUTE FINALED DATE");
				if(fldx!=NULL)
				{
					if(fldx->len>8)
					{
						FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED DATE",curdate10);
					} else FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED DATE",curdate);
				}
				FINDFLDSETSTRING(empabtr,"SUBSTITUTE FINALED TIME",curtime);
			}
			FINDFLDSETDOUBLE(empabtr,"SUBSTITUTE UNITS UNASSIGNED",(sub_units_unass-total_sub_units));
			FINDFLDSETSTRING(empabtr,"SOURCE USER",USERLOGIN);
			fldx=FLDNRD(empabtr,"ENTRY DATE");
			if(fldx!=NULL)
			{
				if(fldx->len>8)
				{
					FINDFLDSETSTRING(empabtr,"ENTRY DATE",curdate10);
				} else FINDFLDSETSTRING(empabtr,"ENTRY DATE",curdate);
			}
			FINDFLDSETSTRING(empabtr,"ENTRY TIME",curtime);
			ADVWRTTRANSsec(empabtr,0,NULL,NULL,EmpAbsSubData,p);
		}
		if(curtime!=NULL) Rfree(curtime);
		if(curdate!=NULL) Rfree(curdate);
		if(perid!=NULL) Rfree(perid);
		if(lperid!=NULL) Rfree(lperid);
		if(p!=NULL) Rfree(p);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
}
static void save_check(RDArsrc *mainrsrc,PassableStruct *p)
{
	char *perid1=NULL,*warnmessage=NULL;
	double units=0.0;
	long size_of_file=0;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1);
	if(check_perid(mainrsrc,FALSE,p)==FALSE) return;
	getsupportingrecords(mainrsrc,TRUE,p);
	FINDRSCGETDOUBLE(mainrsrc,"[EMPABS][UNITS]",&units);
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
        if(check_subid(mainrsrc,FALSE,p)==FALSE)
        {
                return;
        }
        if(check_subjobno(mainrsrc,FALSE,p)==FALSE)
        {
                return;
        }
	save_record(mainrsrc,p);
}
static void save_record(RDArsrc *mainrsrc,PassableStruct *p)
{
	int x=0,lf=0;
	double units=0.0;
	char *rcddesc=NULL,deleteflag=FALSE;
	char *perid1=NULL;
	char *date=NULL,*timex=NULL;
	RDArmem *member=NULL;
	NRDfield *field=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1)) return;
	rsrc2filerecord(filenum,mainrsrc);
	FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(filenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(filenum,"SOURCE USER VERIFIED",deleteflag);
	FINDFLDGETDOUBLE(filenum,"UNITS",&units);
	if((x=FINDRSC(mainrsrc,"[EMPABS][ENTRY DATE]"))!=(-1))
	{
		member=mainrsrc->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			if(member->field_length>8) date=GETCURRENTDATE10();
				else date=GETCURRENTDATE();
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
		field=FLDNRD(filenum,"ENTRY DATE");
		if(field!=NULL)
		{
			if(field->len>8) date=GETCURRENTDATE10();
				else date=GETCURRENTDATE();
		}
		FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date);
	if((x=FINDRSC(mainrsrc,"[EMPABS][ENTRY TIME]"))!=(-1))
	{
		member=mainrsrc->rscs+x;
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
	if(!FINDRSCLISTAPPlib(mainrsrc,"PREVIOUS EMPLOYEE ABSENTEES",(prevempabs->numlibs-1),
		prevempabs))
	{
		updatersrc(mainrsrc,"PREVIOUS EMPLOYEE ABSENTEES");
	}
	if(perid1!=NULL) Rfree(perid1);
	updateSCRNvirtuals(mainrsrc);
}
static void quit_check(RDArsrc *mainrsrc,PassableStruct *p)
{
	quit_record(mainrsrc,p);
}
static void quit_record_freens(RDArsrc *mainrsrc,PassableStruct *p)
{
        counter=0;
        quit_record_free(mainrsrc,p);
}
static void quit_record_free(RDArsrc *mainrsrc,PassableStruct *p)
{
	APPlib *exeargs=NULL;

	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist);
	}
	if(p!=NULL) Rfree(p);
	if(mbl_perdmg!=NULL) FREE_BROWSE_EXIT(mbl_perdmg);
	if(mbl_submstr!=NULL) FREE_BROWSE_EXIT(mbl_submstr);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(dmgnum!=(-1)) CLSNRD(dmgnum);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(subdmgnum!=(-1)) CLSNRD(subdmgnum);
	if(submstr!=(-1)) CLSNRD(submstr);
	if(subwrk!=(-1)) CLSNRD(subwrk);
	if(subemp!=(-1)) CLSNRD(subemp);
	if(subpos!=(-1)) CLSNRD(subpos);
	if(subloc!=(-1)) CLSNRD(subloc);
	if(subjmstnum!=(-1)) CLSNRD(subjmstnum);
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
	if(subjobnolist!=NULL) freeapplib(subjobnolist);
	if(personnel_identification!=NULL) Rfree(personnel_identification);
	if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
	if(counter>0) 
	{
		exeargs=APPlibNEW();
		addAPPlib(exeargs,module);
		addAPPlib(exeargs,audit_reportname);
		ExecuteProgram("doreport",exeargs);
		if(exeargs!=NULL) freeapplib(exeargs);
/*
		RUNREPORT(module,audit_reportname,NULL,NULL,FALSE,TRUE,NULL);
*/
	}
	if(audit_reportname!=NULL) Rfree(audit_reportname);
}
static void quit_record(RDArsrc *mainrsrc,PassableStruct *p)
{
	quit_record_free(mainrsrc,p);
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
	addDBfield(dbsort,"ABSENTEE UNITS",SDECIMALV,8);
	addDBfield(dbsort,"SUBSTITUTE TRANSACTION NUMBER",LONGV,6);
	field=FLDNRD(submstr,"PERSONNEL IDENTIFICATION");
	if(field!=NULL)
	{
		addDBfield(dbsort,"SUBSTITUTE PERSONNEL IDENTIFICATION",field->type,field->len);
	} else {
		prterr("Error field not found.  Must use resolve error before proper recording of Substitute information can take place.");
	}
	addDBfield(dbsort,"SUBSTITUTE JOB NUMBER",LONGV,6);
	addDBfield(dbsort,"SUBSTITUTE UNITS",SDECIMALV,8);
	addDBfilefield(dbsort,posmstr,"POSITION IDENTIFICATION");
	addDBfilefield(dbsort,poslmst,"LOCATION IDENTIFICATION");
	addDBfield(dbsort,"STARTING DATE",DATES,8);
	addDBfield(dbsort,"ENDING DATE",DATES,8);
	key=KEYNUM(empabtr,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsort,keyno,part->name);
		}
		addDBkeypart(dbsort,keyno,"SUBSTITUTE TRANSACTION NUMBER");
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
	int transno,int jobno,char *posid,char *locid,
	double job_units,double sub_units,double lve_units,
	double abs_units,char *subperid,int subjobno,
	char *startdate, char *enddate,PassableStruct *p)
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
	FINDFLDSETDOUBLE(dbsort->fileno,"ABSENTEE UNITS",abs_units);
	FINDFLDSETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS",sub_units);
	FINDFLDSETDOUBLE(dbsort->fileno,"LEAVE UNITS UNASSIGNED",lve_units);
	FINDFLDSETSTRING(dbsort->fileno,"SUBSTITUTE PERSONNEL IDENTIFICATION",subperid);
	FINDFLDSETINT(dbsort->fileno,"SUBSTITUTE JOB NUMBER",subjobno);
	FINDFLDSETDOUBLE(dbsort->fileno,"SUBSTITUTE UNITS UNASSIGNED",sub_units);
	FINDFLDSETSTRING(dbsort->fileno,"POSITION IDENTIFICATION",posid);
	FINDFLDSETSTRING(dbsort->fileno,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(dbsort->fileno,"STARTING DATE",startdate);
	FINDFLDSETSTRING(dbsort->fileno,"ENDING DATE",enddate);
	WRTNRD(dbsort->fileno,1,"SUBSTITUTE TRANSACTION NUMBER");
}
static void computejobs(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL;
	short ef=FALSE,dodiag=FALSE;
	char deleteflag=FALSE,*sortname=NULL,*expressn=NULL;
	char *perid1=NULL,*perid2=NULL,*subperid1=NULL,*e=NULL;
	char *posid=NULL,*locid=NULL,*startdate=NULL,*enddate=NULL;
	char curremp=FALSE,active=FALSE,replace=FALSE;
	int jobno=0,transno1=0,subjobno1=0;
	double empabtr_units=0.0,sub_units=0.0,lve_units=0.0,abs_units=0.0;
	PassableStruct *p=NULL;

	FINDRSCGETSTRING(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETINT(mainrsrc,"[EMPABS][TRANSACTION NUMBER]",&transno1);
	FINDRSCGETDOUBLE(mainrsrc,"[EMPABS][UNITS]",&abs_units);
	if(dbsort!=NULL)
	{
		ef=FRSNRDsec(dbsort->fileno,1);
		while(!ef)
		{
			DELNRD(dbsort->fileno);
			ef=NXTNRDsec(dbsort->fileno,1);
		}
	} else {
		sortname=unique_name();
		dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
		if(sortname!=NULL) Rfree(sortname);
        	makeDBsortfile(dbsort);
        	dbsort->fileno=OPNDBsort(dbsort);
	}
	tmprsrc=diagscrn(empabtr,"DIAGNOSTIC SCREEN",module,
		"Computing Employee Absentees Transactions",NULL);
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
			for(ef=ADVGTENRDsec(payjmst,1,EmpAbsSubData,p);!ef;ef=ADVNXTNRDsec(payjmst,1,EmpAbsSubData,p))
			{
				dodiag=FALSE;
				FINDFLDGETSTRING(payjmst,"PERSONNEL IDENTIFICATION",&perid2);
				if(RDAstrcmp(perid2,perid1)) break;
				FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
				FINDFLDGETCHAR(payjmst,"ACTIVE",&active);
				FINDFLDGETCHAR(payjmst,"SUBSTITUTE WHEN ABSENT",&replace);
				if(!deleteflag && active && replace)
				{
					SEENRDRECORD(payjmst);TRACE;
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
					if(ADVEQLNRDsec(poslmst,1,EmpAbsSubData,p)) KEYNRD(poslmst,1);

					COPYFIELD(posmstr,poscls,"CLASS IDENTIFICATION");
					if(ADVEQLNRDsec(poscls,1,EmpAbsSubData,p)) KEYNRD(poscls,1);

					COPYFIELD(posmstr,posdedc,"DEDUCTION CLASS IDENTIFICATION");
					if(ADVEQLNRDsec(posdedc,1,EmpAbsSubData,p)) KEYNRD(posdedc,1);

					COPYFIELD(posmstr,possmst,"SERVICE AREA IDENTIFICATION");
					if(ADVEQLNRDsec(possmst,1,EmpAbsSubData,p)) KEYNRD(possmst,1);

					COPYFIELD(posmstr,posngms,"NEGOTIATING GROUP IDENTIFICATION");
					if(ADVEQLNRDsec(posngms,1,EmpAbsSubData,p)) KEYNRD(posngms,1);

					FINDFLDGETCHAR(posngms,"DELETEFLAG",&deleteflag);
					COPYFIELD(payjmst,posgrs,"GROSS IDENTIFICATION");
					if(!ADVEQLNRDsec(posgrs,1,EmpAbsSubData,p))

					{
					FINDFLDGETCHAR(posgrs,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					COPYFIELD(payjmst,empadef,"GROSS IDENTIFICATION");
					if(!ADVEQLNRDsec(empadef,1,EmpAbsSubData,p))
					{
						TRACE;
						FINDFLDGETCHAR(empadef,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							FINDFLDGETSTRING(empadef,"ABSENTEE UNITS TO ABSENTEE JOB UNITS FORMULA",&expressn);
							if(isEMPTY(expressn))
							{
								if(diagapps)
								{
									prterr("Warning: ABSENTEE UNITS TO JOB UNITS FORMULA is Empty.");
								}
							}
							empabtr_units=EVALUATEdbl(expressn,EmpAbsSubData,p);
							if(empabtr_units==0.0)
							{
								if(diagapps)
								{
									prterr("Warning: The expression ABSENTEE UNITS TO JOB UNITS FORMULA {%s} Evaulated to Zero.",(expressn==NULL?"":expressn));
								}
							}
							setempabtrrecord(perid1,transno1,jobno,empabtr_units);
							FINDFLDGETSTRING(empadef,"ABSENTEE JOB UNITS TO SUBSTITUTE UNITS FORMULA",&expressn);
							if(isEMPTY(expressn))
							{
								if(diagapps)
								{
									prterr("Warning: ABSENTEE JOB UNITS TO SUBSTITUTE UNITS FORMULA is Empty.");
								}
							}
							sub_units=EVALUATEdbl(expressn,EmpAbsSubData,p);
							if(sub_units==0.0)
							{
								if(diagapps)
								{
									prterr("Warning: The expression ABSENTEE JOB UNITS TO SUBSTITUTE UNITS FORMULA {%s} Evaulated to Zero.",(expressn==NULL?"":expressn));
								}
							}
							FINDFLDGETSTRING(empadef,"ABSENTEE JOB UNITS TO LEAVE UNITS FORMULA",&expressn);
							if(isEMPTY(expressn))
							{
								prterr("Warning: ABSENTEE UNITS TO LEAVE UNITS FORMULA is Empty.");
							}
							lve_units=EVALUATEdbl(expressn,EmpAbsSubData,p);
							if(lve_units==0.0)
							{
								if(diagapps)
								{
									prterr("Warning: The expression ABSENTEE JOB UNITS TO LEAVE UNITS FORMULA {%s} Evaulated to Zero.",(expressn==NULL?"":expressn));
								}
							}
							FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1);
							FINDRSCGETINT(mainrsrc,"SUBSTITUTE JOB NUMBER",&subjobno1);
							FINDRSCGETSTRING(mainrsrc,"[EMPABS][STARTING DATE]",&startdate);
							FINDRSCGETSTRING(mainrsrc,"[EMPABS][ENDING DATE]",&enddate);
							FINDFLDGETSTRING(posmstr,"POSITION IDENTIFICATION",&posid);
							FINDFLDGETSTRING(poslmst,"LOCATION IDENTIFICATION",&locid);
							addsortrecord(dbsort,mainrsrc,perid1,transno1,jobno,posid,locid,empabtr_units,
								sub_units,lve_units,abs_units,subperid1,subjobno1,startdate,enddate,p);
							dodiag=TRUE;
						} else if(diagapps)
						{
							prterr("Skipped by Deleted Employee Absentee Definition Master Record.");KEYNRD(empadef,1);SEENRDRECORD(empadef);
						}
					} else if(diagapps)
					{
						prterr("Skipped by not finding Employee Absentee Definition Master reocord.");KEYNRD(empadef,1);SEENRDRECORD(empadef);
					}
					} else if(diagapps)
					{
						prterr("Skipped by Deleted Position Gross Identification Master Record.");KEYNRD(posgrs,1);SEENRDRECORD(posgrs);
					}
					} else if(diagapps)
					{
						prterr("Skipped by not finding Position Gross Identification Master reocord.");KEYNRD(posgrs,1);SEENRDRECORD(posgrs);
					}
					} else if(diagapps)
					{
						prterr("Skipped by Deleted Position Identification Master Record.");KEYNRD(posmstr,1);SEENRDRECORD(posmstr);
					}
					} else if(diagapps)
					{
						prterr("Skipped by not finding Position Identification Master reocord.");KEYNRD(posmstr,1);SEENRDRECORD(posmstr);
					}
					} else if(diagapps)
					{
						prterr("Skipped by Deleted Position Personnel Identification Master Record.");KEYNRD(posper,1);SEENRDRECORD(posper);
					}
					} else if(diagapps)
					{
						prterr("Skipped by not finding Position Personnel Identification Master reocord.");KEYNRD(posper,1);SEENRDRECORD(posper);
					}
				} else if(diagapps)
				{
					prterr("Skipped because of deleted, inactive, or non-Substitute When Absent Job Master Record.");KEYNRD(payjmst,1);SEENRDRECORD(payjmst);
				}
				if(tmprsrc!=NULL)
				{
					update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
				}
			}
		} else if(diagapps)
		{
			prterr("Skipped because of deleted, inactive, or non-Current Employee Personnel Identification Demographics Master Record.");KEYNRD(dmgnum,1);SEENRDRECORD(dmgnum);
		}
	} else if(diagapps)
	{
		prterr("Skipped by not finding Personnel Identification Demographics Master reocord.");KEYNRD(dmgnum,1);SEENRDRECORD(dmgnum);
	}
	if(p!=NULL) Rfree(p);
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(subperid1!=NULL) Rfree(subperid1);
	if(posid!=NULL) Rfree(posid);
	if(locid!=NULL) Rfree(locid);
	if(startdate!=NULL) Rfree(startdate);
	if(enddate!=NULL) Rfree(enddate);
	if(expressn!=NULL) Rfree(expressn);
/*
	updatebrowse(TRUE,mbl_empabtr,dbsort->fileno,module,mainrsrc);
*/
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
}
static void changeper(RDArsrc *mainrsrc,short override,short update,PassableStruct *p)
{
	char *perid1=NULL,*subperid1=NULL;
	double units1=0.0;
	int subjobno1=0;

	if(FINDRSCGETSTRING(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(FINDRSCGETDOUBLE(mainrsrc,"[EMPABS][UNITS]",&units1)) return;
	if(FINDRSCGETSTRING(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",&subperid1)) return;
	if(FINDRSCGETINT(mainrsrc,"SUBSTITUTE JOB NUMBER",&subjobno1)) return;
	if(RDAstrcmp(perid1,personnel_identification) || units1!=absentee_units || 
		RDAstrcmp(subperid1,subpersonnel_identification) || 
		subjobno1!=subjob_number || override)
	{
		computejobs(mainrsrc);
		if(personnel_identification!=NULL) Rfree(personnel_identification);
		personnel_identification=stralloc(perid1);
		absentee_units=units1;
		if(subpersonnel_identification!=NULL) Rfree(subpersonnel_identification);
		subpersonnel_identification=stralloc(subperid1);
		subjob_number=subjobno1;
		getsupportingrecords(mainrsrc,update,p);
	}
	if(perid1!=NULL) Rfree(perid1);
}
static void browse_perdmg(RDArsrc *mainrsrc,PassableStruct *p)
{
	RDArsrc *browsersrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(mainrsrc);
	if(mbl_perdmg==NULL)
	{
		browsersrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE PERSONNEL BROWSE");
		searchrsrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE PERSONNEL SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE PERSONNEL DEFINE LIST");
		addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
		addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_perdmg=ADVSbrowse(dmgnum,1,browsersrc,searchrsrc,definelist,
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
static short sub_selectfunc(MakeBrowseList *blist)
{
	char *perid=NULL,*subperid=NULL,delflag=FALSE;
	PassableStruct *p=NULL;

	p=(PassableStruct *)blist->funcargs;
	FINDFLDGETCHAR(submstr,"DELETEFLAG",&delflag);
	if(delflag) return(FALSE);
	FINDRSCGETSTRING(p->r,"[EMPABS][PERSONNEL IDENTIFICATION]",&perid);
	FINDFLDGETSTRING(submstr,"PERSONNEL IDENTIFICATION",&subperid);
	if(!RDAstrcmp(perid,subperid))
	{
		if(perid!=NULL) Rfree(perid);
		if(subperid!=NULL) Rfree(subperid);
		return(FALSE);
	}
	if(perid!=NULL) Rfree(perid);
	if(subperid!=NULL) Rfree(subperid);
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
static void test_empabtr(RDArsrc *mainrsrc,PassableStruct *p)
{
	readallwidgets(mainrsrc);
	rsrc2filerecord(filenum,mainrsrc);
	if(check_perid(mainrsrc,TRUE,p)==TRUE)
	{
		updateSCRNvirtuals(mainrsrc);
		browse_empabtr(mainrsrc,p);
	} else {
		updateSCRNvirtuals(mainrsrc);
	}
}
static void setfiles(RDArsrc *parent,PassableStruct *p)
{
	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	getsupportingrecords(parent,TRUE,p);
	updateSCRNvirtuals(parent);
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
	NRDfield *fields=NULL,*fldx=NULL,*field=NULL;
	NRDkey *keys=NULL;
	PassableStruct *p=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"ADD EMPLOYEE ABSENTEE SUB-WORKED")) 
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
		audit_reportname=stralloc("EASWADT");
	}
	if((filenum=APPOPNNRD(module,"EMPABS",TRUE,TRUE))==(-1)) return;
	if((empabtr=APPOPNNRD(module,"EMPABTR",TRUE,TRUE))==(-1)) return;
	if((empadef=APPOPNNRD(module,"EMPADEF",TRUE,TRUE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payjmst=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((subdmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((submstr=APPOPNNRD("SUBMGT","SUBMASTR",TRUE,TRUE))==(-1)) return;
	if((subwrk=APPOPNNRD("SUBMGT","SUBWKMAS",TRUE,TRUE))==(-1)) return;
	if((subpos=APPOPNNRD("SUBMGT","SUBPOS",TRUE,TRUE))==(-1)) return;
	if((subemp=APPOPNNRD("SUBMGT","SUBEMP",TRUE,TRUE))==(-1)) return;
	if((subloc=APPOPNNRD("SUBMGT","SUBLOC",TRUE,TRUE))==(-1)) return;
	if((subjmstnum=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
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
	mainrsrc=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUB-WORKED");
	definelist=RDArsrcNEW(module,"ADD EMPLOYEE ABSENTEE SUB-WORKED DEFINE LIST");
	addDFincvir(mainrsrc,module,"EMPABS",NULL,filenum);
	addDFincvir(mainrsrc,module,"EMPABTR",NULL,empabtr);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(definelist,module,"EMPABS",NULL,filenum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
        makeDBsortfile(dbsort);
        dbsort->fileno=OPNDBsort(dbsort);
/*
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
*/
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
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
					file2rsrc(dmgnum,mainrsrc,FALSE);
					addstdrsrc(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(mainrsrc,"[EMPABS][PERSONNEL IDENTIFICATION]",check_peridcb,p);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(fldx->name,"REPLACEMENT WORKER"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(mainrsrc,filenum,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
				}
				sprintf(name,"[%s][%s]",filename,fldx->name);
				FINDRSCSETFUNC(mainrsrc,name,setfiles,p);
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mainrsrc,"[EMPABS][UNITS]",check_unitscb,p);
	prevempabs=APPlibNEW();
	addAPPlib(prevempabs,"No Employee Absentees added this session");
	addlstrsrc(mainrsrc,"PREVIOUS EMPLOYEE ABSENTEES",&x,
		TRUE,NULL,prevempabs->numlibs,&prevempabs->libs,NULL);
	field=FLDNRD(filenum,"ENTRY DATE");
	if(field!=NULL)
	{
		if(field->len>8)
		{
			FINDRSCSETSTRING(mainrsrc,"[EMPABS][ENTRY DATE]",CURRENT_DATE10);
		} else {
			FINDRSCSETSTRING(mainrsrc,"[EMPABS][ENTRY DATE]",CURRENT_DATE);
		}
	}
	FINDRSCSETSTRING(mainrsrc,"[EMPABS][ENTRY TIME]",CURRENT_TIME);
	addstdrsrc(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"SUBSTITUTE JOB NUMBER",LONGV,6,&x,TRUE);
	FINDRSCSETFUNC(mainrsrc,"SUBSTITUTE PERSONNEL IDENTIFICATION",check_subidcb,p);
	FINDRSCSETFUNC(mainrsrc,"SUBSTITUTE JOB NUMBER",check_subjobnocb,p);
	file2rsrc(subjmstnum,mainrsrc,FALSE);
	singlefile2rsrc(subdmgnum,mainrsrc,FALSE);
	if(subjobnolist==NULL) subjobnolist=APPlibNEW();
	addlstrsrc(mainrsrc,"SUBSTITUTE JOB NUMBERS",&x,TRUE,
		changesubjobnocb,subjobnolist->numlibs,
		&subjobnolist->libs,p);
	savelist(definelist,p);
	SetVirFieldFuncs(mainrsrc);
	SCRNvirtual2rsrc(mainrsrc);
	addbtnrsrc(mainrsrc,"BROWSE PERSONNEL",TRUE,browse_perdmg,p);
	addbtnrsrc(mainrsrc,"BROWSE SUBSTITUTES",TRUE,browse_submstrcb,p);
	addbtnrsrc(mainrsrc,"BROWSE EMPLOYEE JOB ABSENTEES",TRUE,test_empabtr,p);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"DEFINE PREVIOUS LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(mainrsrc,"RESET DEFAULTS",TRUE,reset_defaults,p);
	addrfcbrsrc(mainrsrc,"SAVE",TRUE,save_check,p);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quit_check,p);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	FINDFLDSETINT(filenum,"TRANSACTION NUMBER",transno);
	FINDRSCSETINT(mainrsrc,"[EMPABS][TRANSACTION NUMBER]",transno);
	set_defaults(mainrsrc,FALSE,p);
	APPmakescrn(mainrsrc,FALSE,quit_record_freens,p,TRUE);
	RDAAPPMAINLOOP();
}
