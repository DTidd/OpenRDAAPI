/* restaint.c - Apply Interest */
#ifndef WIN32
#define __NAM__ "restaint.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restaint.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rptgen.h>
#include <ldval.h>
#include <rlstmgt.h>
/*ADDLIB rpt */
/*ADDLIB add */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static APPlib *DEFtypes=NULL;
static char *module="RLSTMGT",**applytype=NULL,*reportname=NULL;
static char *apply_expression=NULL,**monthlist=NULL;
static short rstmst=(-1),rstci=(-1),classnum=(-1);
static short COUNT=FALSE;
static short baccnum=(-1),raccnum=(-1),ryrnum=(-1),byrnum=(-1);
static short rstrn=(-1),fsclyr=(-1);
static int apply_type=0,xmonth=0;
double AMOUNT=0.0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
static void save_record(RDArsrc *,short,DBsort *);
static void quit_record(RDArsrc *,DBsort *);
static void save_record_up(RDArsrc *,DBsort *);
static void save_record_nup(RDArsrc *,DBsort *);
static void save_check(RDArsrc *,short,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static int getcurrmonth(void);
DBsort *dbsort=NULL;
static void applym(short,void *,DBsort *);
static void applyinterest(RDArsrc *);
static void changeDEFtypes(RDArsrc *,DBsort *);
static short chkaccountcode(RDArsrc *,DBsort *);
static void chkaccountcodecb(RDArsrc *,DBsort *);
static short chkrevenuecode(RDArsrc *,DBsort *);
static void chkrevenuecodecb(RDArsrc *,DBsort *);
static short chkcashcode(RDArsrc *,DBsort *);
static void chkcashcodecb(RDArsrc *,DBsort *);
static void clearwidget(RDArsrc *,char *);
static short checktrantype(RDArsrc *,DBsort *);
static void changetrantype(RDArsrc *,DBsort *);
static RDAfinmgt *fmgt=NULL;
static void readscreen(RDArsrc *,DBsort *);
static void setfiles(RDArsrc *,DBsort *);

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static void ApplySubData(char **tmp,PassableStruct *p)
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
		prterr("DIAG ApplySubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
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
				case DOLLARS_NOCENTS:
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
	} else if(!RDAstrcmp(modulename,"AMOUNT"))
	{
		if(AMOUNT<0) sprintf(stemp,"(%f)",AMOUNT);
			else sprintf(stemp,"%f",AMOUNT);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
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
						temp=EVALUATEstr(v->expression,ApplySubData,p);
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
					} else if(start<v->dlen&& !isEMPTY(temp))
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
						*v->value.short_value=EVALUATEsht(v->expression,ApplySubData,p);
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
						*v->value.integer_value=EVALUATEint(v->expression,ApplySubData,p);
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
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.float_value=EVALUATEdbl(v->expression,ApplySubData,p);
						if(v->type==DOLLARS ||
							v->type==DOUBLEV ||
							v->type==DOLLARS_NOCENTS ||
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
						*v->value.string_value=EVALUATEbol(v->expression,ApplySubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,ApplySubData,p);
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
					case DOLLARS_NOCENTS:
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
		prterr("DIAG ApplySubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
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
	AddFileFields(list,rstmst);
	AddFileFields(list,classnum);
	AddFileFields(list,rstci);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"RESTMSTR",NULL,dumb);
	addDFincvir(tmprsrc,module,"RESTCLAS",NULL,dumb);
	addDFincvir(tmprsrc,module,"RESTINT",NULL,dumb);
	addDFincvir(tmprsrc,module,"RESTINAM",NULL,dumb);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/RLSTMGT.VIR",CURRENTDIRECTORY);
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
static void clearwidget(RDArsrc *mtnrsrc,char *widgetname)
{
#ifdef CLEARWDGT
	FINDRSCSETSTRING(mtnrsrc,widgetname,NULL);
	updatersrc(mtnrsrc,widgetname);
#endif
}
static void chkaccountcodecb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *acctcode=NULL;
	short d=0;
	int x=0,yearid=0;

	readwidget(mtnrsrc,"TAX YEAR");
	FINDRSCGETINT(mtnrsrc,"TAX YEAR",&yearid);
	FINDFLDSETINT(dbsort->fileno,"TAX YEAR",yearid);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"ACCOUNT CODE");
	FINDRSCGETSTRING(mtnrsrc,"ACCOUNT CODE",&acctcode);
	FINDFLDSETSTRING(dbsort->fileno,"ACCOUNT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkaccountcode(mtnrsrc,dbsort);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkaccountcode(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL;
	short deftype=0;
	int yearid=0;
	PassableStruct *p=NULL;

	FINDFLDGETINT(dbsort->fileno,"TAX YEAR",&yearid);
	FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETSTRING(dbsort->fileno,"ACCOUNT CODE",&acctcode);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if((tmp=ADVCHECKACCT(ryrnum,raccnum,acctcode,REV_ACCT,deftype,yearid,ApplySubData,p))!=NULL)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ACCOUNT CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"ACCOUNT CODE",NULL);
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		Rfree(tmp);
		return(FALSE);
	}
	if(p!=NULL) Rfree(p);
	FINDFLDGETSTRING(raccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	return(TRUE);
}
static void chkrevenuecodecb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *acctcode=NULL;
	short d=0;
	int x=0,yearid=0;

	readwidget(mtnrsrc,"TAX YEAR");
	FINDRSCGETINT(mtnrsrc,"TAX YEAR",&yearid);
	FINDFLDSETINT(dbsort->fileno,"TAX YEAR",yearid);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"REVENUE CODE");
	FINDRSCGETSTRING(mtnrsrc,"REVENUE CODE",&acctcode);
	FINDFLDSETSTRING(dbsort->fileno,"REVENUE CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkrevenuecode(mtnrsrc,dbsort);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkrevenuecode(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL,*tmp2=NULL;
	short deftype=0;
	int yearid=0;
	PassableStruct *p=NULL;

	FINDFLDGETINT(dbsort->fileno,"TAX YEAR",&yearid);
	FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETSTRING(dbsort->fileno,"REVENUE CODE",&acctcode);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if((tmp=ADVCHECKACCT(byrnum,baccnum,acctcode,BAL_ACCT,deftype,yearid,ApplySubData,p))!=NULL)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"REVENUE CODE",NULL);
		clearwidget(mtnrsrc,"REVENUE CODE DESCRIPTION");
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		Rfree(tmp);
		return(FALSE);
	}
	if(RDAstrcmp(tmp=ADVGETBALREF(baccnum,acctcode,deftype,ApplySubData,p),"REVENUE"))
	{
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode)+200);
		sprintf(tmp2,"The Balance Sheet Account [%s] exists and is active for the REVENUE CODE, but it's REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [REVENUE] is required.",acctcode,tmp);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE CODE!",tmp2,NULL,NULL,TRUE,NULL);
		clearwidget(mtnrsrc,"REVENUE CODE");
		clearwidget(mtnrsrc,"REVENUE CODE DESCRIPTION");
		if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		return(FALSE);
	}
	if(p!=NULL) Rfree(p);
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"REVENUE CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"REVENUE CODE DESCRIPTION");
	return(TRUE);
}
static void chkcashcodecb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *acctcode=NULL;
	short d=0;
	int x=0,yearid=0;

	readwidget(mtnrsrc,"TAX YEAR");
	FINDRSCGETINT(mtnrsrc,"TAXL YEAR",&yearid);
	FINDFLDSETINT(dbsort->fileno,"TAX YEAR",yearid);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"CASH CODE");
	FINDRSCGETSTRING(mtnrsrc,"CASH CODE",&acctcode);
	FINDFLDSETSTRING(dbsort->fileno,"CASH CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkcashcode(mtnrsrc,dbsort);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkcashcode(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *acctcode=NULL,*tmp=NULL,*desc=NULL,*tmp2=NULL;
	short deftype=0;
	int yearid=0;
	PassableStruct *p=NULL;

	FINDFLDGETINT(dbsort->fileno,"TAX YEAR",&yearid);
	FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&deftype);
	FINDFLDGETSTRING(dbsort->fileno,"CASH CODE",&acctcode);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if((tmp=ADVCHECKACCT(byrnum,baccnum,acctcode,BAL_ACCT,deftype,yearid,ApplySubData,p))!=NULL)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"CASH CODE",NULL);
		clearwidget(mtnrsrc,"CASH CODE DESCRIPTION");
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		Rfree(tmp);
		return(FALSE);
	}
	if(RDAstrcmp(tmp=ADVGETBALREF(baccnum,acctcode,deftype,ApplySubData,p),"CASH"))
	{
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode)+200);
		sprintf(tmp2,"The Balance Sheet Account [%s] exists and is active for the CASH CODE, but it's REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [CASH] is required.",acctcode,tmp);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE!",tmp2,NULL,NULL,TRUE,NULL);
		clearwidget(mtnrsrc,"CASH CODE");
		clearwidget(mtnrsrc,"CASH CODE DESCRIPTION");
		if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		if(acctcode!=NULL) { Rfree(acctcode); acctcode=NULL; }
		return(FALSE);
	}
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	return(TRUE);
}
static void changeDEFtypes(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	short deftype1=0,deftype2=0;
	int seldeftype2=0;

	readwidget(mtnrsrc,"ACCOUNT CODE");
	FINDRSCGETDEFTYPE(mtnrsrc,"ACCOUNT CODE",&deftype1);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype2);
	deftype2=(short)seldeftype2;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"ACCOUNT CODE",deftype2);
		updatersrc(mtnrsrc,"ACCOUNT CODE");
		FINDRSCSETSTRING(mtnrsrc,"REVENUE CODE",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"REVENUE CODE",deftype2);
		updatersrc(mtnrsrc,"REVENUE CODE");
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"CASH CODE",deftype2);
		updatersrc(mtnrsrc,"CASH CODE");
	}
	updateSCRNvirtuals(mtnrsrc);
}
static void changetrantype(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	readwidget(mtnrsrc,"REFERENCE IDENTIFICATION");
	readwidget(mtnrsrc,"[RESTMSTR][CLASS IDENTIFICATION]");
	if(checktrantype(mtnrsrc,dbsort))
		clearwidget(mtnrsrc,"REFERENCE IDENTIFICATION");
	updateSCRNvirtuals(mtnrsrc);
}
static short checktrantype(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *refid=NULL,*classid=NULL,*tmp2=NULL;
	char deleteflag=FALSE,compute_when=FALSE;
	PassableStruct *p=NULL;

	FINDRSCGETSTRING(mtnrsrc,"REFERENCE IDENTIFICATION",&refid);
	FINDRSCGETSTRING(mtnrsrc,"[RESTMSTR][CLASS IDENTIFICATION]",&classid);
	FINDFLDSETSTRING(rstci,"CLASS IDENTIFICATION",classid);
	FINDFLDSETSTRING(rstci,"INTEREST IDENTICATION",refid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(rstci,1,ApplySubData,p))
	{
		KEYNRD(rstci,1);
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		tmp2=Rmalloc(RDAstrlen(refid)+RDAstrlen(classid)+200);
		sprintf(tmp2,"The Reference Identification [%s] is not found for Class [%s] in the Real Estate Class Interest Master.",refid,classid);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REFERENCE IDENTIFICATION!",tmp2,NULL,NULL,TRUE,NULL);
		clearwidget(mtnrsrc,"REFERENCE IDENTIFICATION");
		if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
		if(classid!=NULL) Rfree(classid);
		if(refid!=NULL) Rfree(refid);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(rstci,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
			tmp2=Rmalloc(RDAstrlen(refid)+RDAstrlen(classid)+200);
			sprintf(tmp2,"The Reference Identification [%s] is found for Class [%s], but is marked for deletion in the Real Estate Class Interest Master.",refid,classid);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED REFERENCE IDENTIFICATION!",tmp2,NULL,NULL,TRUE,NULL);
			clearwidget(mtnrsrc,"REFERENCE IDENTIFICATION");
			if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
			if(classid!=NULL) Rfree(classid);
			if(refid!=NULL) Rfree(refid);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(rstci,"COMPUTE WHEN APPLIED",&compute_when);
			if(!compute_when)
			{
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				tmp2=Rmalloc(RDAstrlen(refid)+RDAstrlen(classid)+200);
				sprintf(tmp2,"The Reference Identification [%s] is found for Class [%s], but is not to compute during APPLY INTEREST in the Real Estate Class Interest Master.",refid,classid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID COMPUTE DEFINITION!",tmp2,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"REFERENCE IDENTIFICATION");
				if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
				if(classid!=NULL) Rfree(classid);
				if(refid!=NULL) Rfree(refid);
				return(FALSE);
			}
		}
	}
	if(p!=NULL) Rfree(p);
	if(classid!=NULL) Rfree(classid);
	if(refid!=NULL) Rfree(refid);
	return(TRUE);
}
void apply_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE,trantype=2;
	char deleteflag=FALSE;
	char *curdate=NULL,*curtime=NULL;
	RDArsrc *tmprsrc=NULL;
	double amount=0;
	PassableStruct *p=NULL;

	if(DEFtypes!=NULL) 
	{
		freeapplib(DEFtypes);
		DEFtypes=NULL;
	}
	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Applying Interest",NULL);
		addDFincvir(tmprsrc,module,"RESTMSTR",NULL,rstmst);
		addDFincvir(tmprsrc,module,"RESTRAN",NULL,rstrn);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			curdate=GETCURRENTDATE();
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amount);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(!deleteflag && amount!=0.0)
			{
				ZERNRD(rstrn); /* Leave to Default File Spec's */
				COPYFIELD(dbsort->fileno,rstmst,"REAL ESTATE IDENTIFICATION");
				COPYFIELD(dbsort->fileno,rstmst,"OWNER IDENTIFICATION");
				COPYFIELD(dbsort->fileno,rstmst,"TAX YEAR");
				if(ADVEQLNRDsec(rstmst,1,ApplySubData,p)) KEYNRD(rstmst,1);
				COPYFIELD(dbsort->fileno,rstrn,"REAL ESTATE IDENTIFICATION");
				COPYFIELD(dbsort->fileno,rstrn,"OWNER IDENTIFICATION");
				COPYFIELD(dbsort->fileno,rstrn,"TAX YEAR");
				COPYFIELD(dbsort->fileno,rstrn,"DEFINITION TYPE");
				COPYFIELD(dbsort->fileno,rstrn,"ACCOUNT CODE");
				COPYFIELD(dbsort->fileno,rstrn,"REVENUE CODE");
				COPYFIELD(dbsort->fileno,rstrn,"CASH CODE");
				COPYFIELD(dbsort->fileno,rstrn,"MONTH");
				COPYFIELD(dbsort->fileno,rstrn,"REFERENCE IDENTIFICATION");
				COPYFIELD(dbsort->fileno,rstrn,"DESCRIPTION");
				FINDFLDSETSHORT(rstrn,"TRANSACTION TYPE",trantype);
				FINDFLDSETDOUBLE(rstrn,"AMOUNT",amount);
				FINDFLDSETDOUBLE(rstrn,"OUTSTANDING AMOUNT",amount);
				FINDFLDSETSTRING(rstrn,"SOURCE USER",USERLOGIN);
				FINDFLDSETSTRING(rstrn,"ENTRY DATE",curdate);
				FINDFLDSETSTRING(rstrn,"ENTRY TIME",curtime);
				ADVWRTTRANSsec(rstrn,1,"TRANSACTION NUMBER",NULL,ApplySubData,p);
				ADVUpdateRealEstateTransactions(rstmst,(-1),rstrn,amount,
					ApplySubData,p);
				COUNT=TRUE;
				update_diag=TRUE;
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			if(curdate!=NULL) Rfree(curdate);
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(p!=NULL) Rfree(p);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
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
	NRDfield *field,*fields=NULL;
	int x=0;
	short keyno=(-1),nofields=0;

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual("RLSTMGT","APPLY INTEREST SORT SEQUENCE");
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
			case DOLLARS_NOCENTS:
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
	nofields=NUMFLDS(rstrn);
	fields=FLDPOINTER(rstrn);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<nofields;++x,++field)
		{
			if(RDAstrcmp(field->name,"OUTSTANDING AMOUNT") &&
				strncmp(field->name,"SOURCE USER",11) &&
				RDAstrcmp(field->name,"TRANSACTION TYPE") &&
				RDAstrcmp(field->name,"DELETEFLAG"))
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
			}
		}
	}
	key=KEYNUM(rstrn,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt,char *realestateid,char *ownerid,
	int TaxYear,char *interestid,double gross,short deftype,char *acctcode,
	char *revcode,char *cashcode,char *desc,PassableStruct *p)
{
	char deleteflag=FALSE;
	short ymonth=0;

	ymonth=(short)xmonth;
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,ApplySubData,p);
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
			case DOLLARS_NOCENTS:
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
	FINDFLDSETSTRING(dbsrt->fileno,"REAL ESTATE IDENTIFICATION",realestateid);
	FINDFLDSETSTRING(dbsrt->fileno,"OWNER IDENTIFICATION",ownerid);
	FINDFLDSETINT(dbsrt->fileno,"TAX YEAR",TaxYear);
	FINDFLDSETSHORT(dbsrt->fileno,"MONTH",ymonth);
	FINDFLDSETSTRING(dbsrt->fileno,"REFERENCE IDENTIFICATION",interestid);
	FINDFLDSETSHORT(dbsrt->fileno,"DEFINITION TYPE",deftype);
	FINDFLDSETSTRING(dbsrt->fileno,"ACCOUNT CODE",acctcode);
	FINDFLDSETSTRING(dbsrt->fileno,"REVENUE CODE",revcode);
	FINDFLDSETSTRING(dbsrt->fileno,"CASH CODE",cashcode);
	FINDFLDSETSTRING(dbsrt->fileno,"DESCRIPTION",desc);
	FINDFLDSETDOUBLE(dbsrt->fileno,"AMOUNT",gross);
	WRTNRD(dbsrt->fileno,1,"TRANSACTION NUMBER");
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	applym(0,targetkey,blist->funcargs);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	applym(2,NULL,blist->funcargs);
} 
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without processing the Applied Interest?",NULL,doexit,FALSE,blist);
	} else {
		doexit(blist);
	}
}
static void doexit(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*drsrc=NULL,*srsrc=NULL;

	dbsort=blist->funcargs;
	drsrc=blist->definelist;
	srsrc=blist->searchrsrc;
	brsrc=blist->mainrsrc;
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
	DBsort *x;
	RDArsrc *brsrc,*srsrc,*drsrc;

	x=blist->funcargs;
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
	}
	if(WindowCount<=2) apply_backend(x);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static void applyinterest(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,etest=FALSE,dodiag=FALSE,ef1=FALSE,testfile=FALSE;
	short deftype=0;
	char *ownerid=NULL,*classid=NULL,*classid1=NULL,*exprssn=NULL;
	char *interestid=NULL,*acctcode=NULL,*revcode=NULL,*cashcode=NULL;
	char deleteflag=FALSE,*sortname=NULL,*realestateid=NULL,*desc=NULL;
	char *realestateid1=NULL,compute_when=FALSE;
	int x,TaxYear=0;
	long size_of_file=0;
	PassableStruct *p=NULL;
	RDAvirtual *v=NULL;
	RDAacct *acc=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"APPLY TYPES",&apply_type);
	FINDRSCGETINT(mainrsrc,"MONTHS",&xmonth);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&apply_expression);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(rstmst,rs);
	ReadRangeScreen(classnum,rs);
	ReadRangeScreen(rstci,rs);
	tmprsrc=diagscrn(rstmst,"DIAGNOSTIC SCREEN",module,
		"Selecting Real Estate.....",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=dbsort;
	ZERNRD(rstmst);
	ef=ADVFRSNRDsec(rstmst,1,ApplySubData,p);
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETCHAR(rstmst,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			testfile=testfilerangersrc(rstmst,rs);
		} else testfile=TRUE;
		if(!testfile)
		{
			FINDFLDGETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",
				&realestateid);
			FINDFLDGETSTRING(rstmst,"OWNER IDENTIFICATION",&ownerid);
			FINDFLDGETSTRING(rstmst,"CLASS IDENTIFICATION",&classid);
			FINDFLDGETINT(rstmst,"TAX YEAR",&TaxYear);
			FINDFLDSETSTRING(classnum,"CLASS IDENTIFICATION",classid);
			if(!ADVEQLNRDsec(classnum,1,ApplySubData,p))
			{
				FINDFLDGETCHAR(classnum,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					testfile=testfilerangersrc(classnum,rs);
				} else testfile=TRUE;
				if(!deleteflag && !testfile)
				{
					ZERNRD(rstci);
					FINDFLDSETSTRING(rstci,"CLASS IDENTIFICATION",classid);
					ef1=ADVGTENRDsec(rstci,1,ApplySubData,p);
					while(!ef1)
					{
						FINDFLDGETSTRING(rstci,"CLASS IDENTIFICATION",&classid1);
						FINDFLDGETCHAR(rstci,"COMPUTE WHEN APPLIED",&compute_when);
						FINDFLDGETCHAR(rstci,"DELETEFLAG",&deleteflag);
						if(RDAstrcmp(classid,classid1)) break;
						if(!deleteflag && compute_when)
						{
							FINDFLDGETSTRING(rstci,"INTEREST IDENTIFICATION",&interestid);
							testfile=testfilerangersrc(rstci,rs);
							if(!testfile)
							{
								if(apply_expression!=NULL && RDAstrlen(apply_expression)>0)
								{
									etest=EVALUATEbol(apply_expression,ApplySubData,p);
								} else etest=TRUE;
								if(etest)
								{
									for(x=0,v=mainrsrc->virtual;
										x<mainrsrc->num;++x,++v)
									{
										if(!v->computed) computevirtual(v,ApplySubData,p);
										if(testvirtualrangersrc(v,rs)) break;
									}
									if(x>=mainrsrc->num)
									{
										ZERNRD(dbsort->fileno);
										dodiag=TRUE;
										FINDFLDGETSTRING(rstci,
											"INTEREST FORMULA",
											&exprssn);
										AMOUNT=0.0;
										if(exprssn!=NULL)
										{
											AMOUNT=EVALUATEdbl(exprssn,ApplySubData,p);
											AMOUNT=round(AMOUNT);
											Rfree(exprssn);
										} else if(diagapps)
										{
											prterr("Skipped because INTEREST FORMULA is empty.");SEENRDRECORD(rstci);
										}
										if(AMOUNT!=0.0)
										{
											FINDFLDGETSHORT(rstci,"DEFAULT DEFINITION TYPE",&deftype);
											FINDFLDGETSTRING(rstci,"DEFAULT ACCOUNT CODE",&acctcode);
											FINDFLDGETSTRING(rstci,"DEFAULT REVENUE CODE",&revcode);
											FINDFLDGETSTRING(rstci,"DEFAULT CASH CODE",&cashcode);
											FINDFLDGETSTRING(rstci,"NAME",&desc);
											addsortrecord(dbsort,realestateid,ownerid,TaxYear,interestid,AMOUNT,deftype,acctcode,revcode,cashcode,desc,p);
											if(acctcode!=NULL) Rfree(acctcode);
											if(revcode!=NULL) Rfree(revcode);
											if(cashcode!=NULL) Rfree(cashcode);
											if(desc!=NULL) Rfree(desc);
										} else if(diagapps)
										{
											prterr("Skipped because INTEREST FORMULA Evaluated to ZERO ($0.00).");SEENRDRECORD(rstci);
										}
									} else if(diagapps)
									{
										prterr("Skipped by Ranging on Virtual Field [%s].",v->name);SEENRDRECORD(rstci);
									}
								} else if(diagapps)
								{
									prterr("Skipped by Optional Select Expression.");
								}
							} else if(diagapps)
							{
								prterr("Skipped by Ranging on Class Interest Master or deleted class interest master.");
							}
						} else if(diagapps)
						{
							prterr("Skipped by finding a deleted Class Interest.");SEENRDRECORD(rstci);
						}
						CLEAR_SCREEN_VIRTUAL(mainrsrc);
						ef1=ADVNXTNRDsec(rstci,1,ApplySubData,p);
					}
				} else if(diagapps)
				{
					prterr("Skipped by Ranging on Class Master or deleted Class.");KEYNRD(classnum,1);SEENRDRECORD(classnum);
				}
			} else if(diagapps)
			{
				prterr("Skipped by not finding Class Identifications.");KEYNRD(classnum,1);SEENRDRECORD(classnum);
			}
		} else if(diagapps)
		{
			prterr("Skipped by Ranging on Real Estate Master or deleted real estate master.");KEYNRD(rstmst,1);SEENRDRECORD(rstmst);
		}
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		ef=ADVNXTNRDsec(rstmst,1,ApplySubData,p);
	}
	if(p!=NULL) Rfree(p);
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(realestateid!=NULL) Rfree(realestateid);
	if(ownerid!=NULL) Rfree(ownerid);
	if(classid!=NULL) Rfree(classid);
	if(classid1!=NULL) Rfree(classid1);
	if(interestid!=NULL) Rfree(interestid);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		switch(apply_type)
		{
			default:
			case 0: /* Automatic */
				apply_backend(dbsort);
				break;
			case 1: /* Process Exceptions */
				DEFtypes=APPlibNEW();
				if(REVENUE_ACCOUNT!=NULL)
				{
					if(REVENUE_ACCOUNT->codetype!=NULL)
					{
						for(x=0,acc=REVENUE_ACCOUNT->codetype;
							x<REVENUE_ACCOUNT->num;++x,++acc)
						{
							addAPPlib(DEFtypes,acc->name);
						}
					}
				} 
				if(DEFtypes->numlibs<1)
				{
					addAPPlib(DEFtypes,"No Definitions Available");
				}
				brsrc=RDArsrcNEW(module,"APPLY INTEREST BROWSE");
				srsrc=RDArsrcNEW(module,"APPLY INTEREST SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"APPLY INTEREST DEFINE LIST");
				addDFincvir(drsrc,module,"RESTMSTR",NULL,rstmst);
				addDFincvir(drsrc,module,"RESTCLAS",NULL,classnum);
				addDFincvir(srsrc,module,"RESTMSTR",NULL,rstmst);
				addDFincvir(srsrc,module,"RESTCLAS",NULL,classnum);
				GET_SCREEN_VIRTUALS(drsrc,1);
				GET_SCREEN_VIRTUALS(srsrc,2);
				ZERNRD(dbsort->fileno);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				mbl_trans=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,selectfunction,
					doexit_test,okfunc,new_record,
					dbsort,FALSE,NULL,0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
	} else {
		ERRORDIALOG("Apply Interest Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static void makeapplytype()
{
	applytype=Rmalloc(2*sizeof(char *));
	applytype[0]=stralloc("Automatic");
	applytype[1]=stralloc("Process Exceptions");
	monthlist=Rmalloc(12*sizeof(char *));
	monthlist[0]=stralloc("January");
	monthlist[1]=stralloc("February");
	monthlist[2]=stralloc("March");
	monthlist[3]=stralloc("April");
	monthlist[4]=stralloc("May");
	monthlist[5]=stralloc("June");
	monthlist[6]=stralloc("July");
	monthlist[7]=stralloc("August");
	monthlist[8]=stralloc("September");
	monthlist[9]=stralloc("October");
	monthlist[10]=stralloc("November");
	monthlist[11]=stralloc("December");
}
static void freeapplytype()
{
	short x;

	if(applytype!=NULL)
	{
		for(x=0;x<2;++x) Rfree(applytype[x]);
		Rfree(applytype);
		applytype=NULL;
	}
	if(monthlist!=NULL)
	{
		for(x=0;x<12;++x) Rfree(monthlist[x]);
		Rfree(monthlist);
		monthlist=NULL;
	}
}
void quitselect(RDArsrc *mainrsrc)
{
	if(COUNT)
	{
		RUNREPORT("RLSTMGT",reportname,NULL,NULL,TRUE,TRUE,NULL);
	}
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(rstmst!=(-1)) CLSNRD(rstmst);
	if(classnum!=(-1)) CLSNRD(classnum);
	if(rstci!=(-1)) CLSNRD(rstci);
	if(baccnum!=(-1)) CLSNRD(baccnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(raccnum!=(-1)) CLSNRD(raccnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(applytype!=NULL) freeapplytype();
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(!COUNT)
	{
		ShutdownSubsystems();
		exit(0);
	}
}
void main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"APPLY INTEREST")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("APPLYINT");
	fmgt=RDAfinmgtNEW();
	if(GetFinmgtSetup(fmgt,NULL,NULL)==(-1))
	{
		free_finmgt(fmgt);
		fmgt=NULL;
	}
	if((rstmst=APPOPNNRD(module,"RESTMSTR",TRUE,FALSE))==(-1)) return;
	if((rstci=APPOPNNRD(module,"RESTINT",TRUE,FALSE))==(-1)) return;
	if((rstrn=APPOPNNRD(module,"RESTRAN",TRUE,TRUE))==(-1)) return;
	if((classnum=APPOPNNRD(module,"RESTCLAS",TRUE,FALSE))==(-1)) return;
	if((fsclyr=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if((raccnum=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
	if((ryrnum=APPOPNNRD("FINMGT","FINRYR",TRUE,TRUE))==(-1)) return;
	if((baccnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((byrnum=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
	makeapplytype();
	mainrsrc=RDArsrcNEW(module,"APPLY INTEREST");
	addDFincvir(mainrsrc,module,"RESTMSTR",NULL,rstmst);
	addDFincvir(mainrsrc,module,"RESTCLAS",NULL,classnum);
	addDFincvir(mainrsrc,module,"RESTINT",NULL,rstci);
	file2rangersrc(rstmst,mainrsrc);
	file2rangersrc(classnum,mainrsrc);
	file2rangersrc(rstci,mainrsrc);
	addlstrsrc(mainrsrc,"APPLY TYPES",&apply_type,TRUE,NULL,2,
		&applytype,NULL);
	xmonth=getcurrmonth();
	addlstrsrc(mainrsrc,"MONTHS",&xmonth,TRUE,NULL,12,&monthlist,NULL);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,applyinterest,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,FALSE,quitselect,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void getprevious(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *realestateid=NULL,*ownerid=NULL;
	int yearid=0,tranno=0,which_type=0;
	short x=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"REAL ESTATE IDENTIFICATION",&realestateid);
	FINDRSCGETSTRING(mtnrsrc,"OWNER IDENTIFICATION",&ownerid);
	FINDRSCGETINT(mtnrsrc,"TAX YEAR",&yearid);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&tranno);
	FINDFLDSETSTRING(dbsort->fileno,"DISTRICT IDENTIFICATION",realestateid);
	FINDFLDSETSTRING(dbsort->fileno,"OWNER IDENTIFICATION",ownerid);
	FINDFLDSETINT(dbsort->fileno,"TAX YEAR",yearid);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",tranno);
	if(realestateid!=NULL) Rfree(realestateid);
	if(ownerid!=NULL) Rfree(ownerid);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		if(LTENRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
			FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&x);
			which_type=(int)x;
			if(which_type>=DEFtypes->numlibs)
			{
				addAPPlib(DEFtypes,"Invalid Definition Type");
				which_type=DEFtypes->numlibs-1;
			}
			FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",which_type);
			FINDFLDGETSHORT(dbsort->fileno,"MONTH",&x);
			which_type=(int)x;
			FINDRSCSETINT(mtnrsrc,"MONTHS",which_type);
			chkaccountcode(mtnrsrc,dbsort);
			chkrevenuecode(mtnrsrc,dbsort);
			chkcashcode(mtnrsrc,dbsort);
		}
	} else {
		if(PRVNRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
			FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&x);
			which_type=(int)x;
			if(which_type>=DEFtypes->numlibs)
			{
				addAPPlib(DEFtypes,"Invalid Definition Type");
				which_type=DEFtypes->numlibs-1;
			}
			FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",which_type);
			FINDFLDGETSHORT(dbsort->fileno,"MONTH",&x);
			which_type=(int)x;
			FINDRSCSETINT(mtnrsrc,"MONTHS",which_type);
			chkaccountcode(mtnrsrc,dbsort);
			chkrevenuecode(mtnrsrc,dbsort);
			chkcashcode(mtnrsrc,dbsort);
		}
	}
	getsupportingfiles(mtnrsrc,dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,ApplySubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getnext(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *realestateid=NULL,*ownerid=NULL;
	int yearid=0,tranno=0,which_type=0;
	short x=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"REAL ESTATE IDENTIFICATION",&realestateid);
	FINDRSCGETSTRING(mtnrsrc,"OWNER IDENTIFICATION",&ownerid);
	FINDRSCGETINT(mtnrsrc,"TAX YEAR",&yearid);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&tranno);
	FINDFLDSETSTRING(dbsort->fileno,"DISTRICT IDENTIFICATION",realestateid);
	FINDFLDSETSTRING(dbsort->fileno,"OWNER IDENTIFICATION",ownerid);
	FINDFLDSETINT(dbsort->fileno,"TAX YEAR",yearid);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",tranno);
	if(realestateid!=NULL) Rfree(realestateid);
	if(ownerid!=NULL) Rfree(ownerid);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		if(GTENRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
			FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&x);
			which_type=(int)x;
			if(which_type>=DEFtypes->numlibs)
			{
				addAPPlib(DEFtypes,"Invalid Definition Type");
				which_type=DEFtypes->numlibs-1;
			}
			FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",which_type);
			FINDFLDGETSHORT(dbsort->fileno,"MONTH",&x);
			which_type=(int)x;
			FINDRSCSETINT(mtnrsrc,"MONTHS",which_type);
			chkaccountcode(mtnrsrc,dbsort);
			chkrevenuecode(mtnrsrc,dbsort);
			chkcashcode(mtnrsrc,dbsort);
		}
	} else {
		if(NXTNRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
			FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&x);
			which_type=(int)x;
			if(which_type>=DEFtypes->numlibs)
			{
				addAPPlib(DEFtypes,"Invalid Definition Type");
				which_type=DEFtypes->numlibs-1;
			}
			FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",which_type);
			FINDFLDGETSHORT(dbsort->fileno,"MONTH",&x);
			which_type=(int)x;
			FINDRSCSETINT(mtnrsrc,"MONTHS",which_type);
			chkaccountcode(mtnrsrc,dbsort);
			chkrevenuecode(mtnrsrc,dbsort);
			chkcashcode(mtnrsrc,dbsort);
		}
	}
	getsupportingfiles(mtnrsrc,dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,ApplySubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *realestateid=NULL,*ownerid=NULL;
	int yearid=0,tranno=0,which_type=0;
	short x=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"REAL ESTATE IDENTIFICATION",&realestateid);
	FINDRSCGETSTRING(mtnrsrc,"OWNER IDENTIFICATION",&ownerid);
	FINDRSCGETINT(mtnrsrc,"TAX YEAR",&yearid);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&tranno);
	FINDFLDSETSTRING(dbsort->fileno,"REAL ESTATE IDENTIFICATION",realestateid);
	FINDFLDSETSTRING(dbsort->fileno,"OWNER IDENTIFICATION",ownerid);
	FINDFLDSETINT(dbsort->fileno,"TAX YEAR",yearid);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",tranno);
	if(realestateid!=NULL) Rfree(realestateid);
	if(ownerid!=NULL) Rfree(ownerid);
	if(EQLNRD(dbsort->fileno,1)) 
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);	
		FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&x);
		which_type=(int)x;
		if(which_type>=DEFtypes->numlibs)
		{
			addAPPlib(DEFtypes,"Invalid Definition Type");
			which_type=DEFtypes->numlibs-1;
		}
		FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",which_type);
		FINDFLDGETSHORT(dbsort->fileno,"MONTH",&x);
		which_type=(int)x;
		FINDRSCSETINT(mtnrsrc,"MONTHS",which_type);
		chkaccountcode(mtnrsrc,dbsort);
		chkrevenuecode(mtnrsrc,dbsort);
		chkcashcode(mtnrsrc,dbsort);
	}
	getsupportingfiles(mtnrsrc,dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,ApplySubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static int getcurrmonth(void)
{
	char *monthstr=NULL;
	int currmonth=0;

	monthstr=Rmalloc(3);
	monthstr[0]=CURRENT_DATE[0];
	monthstr[1]=CURRENT_DATE[1];
	monthstr[2]=0;
	currmonth=(Ratoi(monthstr)-1);
	if(monthstr!=NULL) Rfree(monthstr);
	return(currmonth);
}
static void readscreen(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	int selected=0;
	short type=0;

	readallwidgets(mtnrsrc);
	ZERNRD(dbsort->fileno);
	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",type);
	FINDRSCGETINT(mtnrsrc,"MONTHS",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(dbsort->fileno,"MONTH",type);
}
static void setfiles(RDArsrc *mtnrsrc,DBsort *t)
{
	PassableStruct *p=NULL;

	readscreen(mtnrsrc,t);
	getsupportingfiles(mtnrsrc,t);	
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,ApplySubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
void applym(short dowhich,void *targetkey,DBsort *t)
{
	char *realestateid=NULL,*ownerid=NULL;
	int x=0,yearid=0,tranno=0;
	RDArsrc *mtnrsrc=NULL;
	short nofields=0,deftype=0;
	NRDfield *fldx=NULL,*fields=NULL;
	short ymonth=0;
	PassableStruct *p=NULL;

	mtnrsrc=RDArsrcNEW(module,"APPLY INTEREST MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"RESTMSTR",NULL,rstmst);
	addDFincvir(mtnrsrc,module,"RESTCLAS",NULL,classnum);
	addDFincvir(mtnrsrc,module,"RESTINT",NULL,rstci);
	addDFincvir(mtnrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
/* do DEFINITION TYPE LIST */
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				yearid=0;
				addlstrsrc(mtnrsrc,"DEFINITION TYPES",&yearid,
					TRUE,changeDEFtypes,DEFtypes->numlibs,&DEFtypes->libs,NULL);
			} else if(!RDAstrcmp(fldx->name,"MONTH"))
			{
				yearid=getcurrmonth();
				addlstrsrc(mtnrsrc,"MONTHS",&yearid,TRUE,setfiles,12,
					&monthlist,t);
			} else {
				ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,TRUE,TRUE);
				FINDRSCSETFUNC(mtnrsrc,fldx->name,setfiles,t);
			}
		}
	}
	if(rstmst!=(-1)) file2rsrc(rstmst,mtnrsrc,FALSE);
	if(classnum!=(-1)) file2rsrc(classnum,mtnrsrc,FALSE);
	if(rstci!=(-1)) file2rsrc(rstci,mtnrsrc,FALSE);
	if(fsclyr!=(-1)) file2rsrc(fsclyr,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"REAL ESTATE IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"OWNER IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"TAX YEAR",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"TRANSACTION NUMBER",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"ACCOUNT CODE",chkaccountcodecb,t);
	FINDRSCSETFUNC(mtnrsrc,"REVENUE CODE",chkrevenuecodecb,t);
	FINDRSCSETFUNC(mtnrsrc,"CASH CODE",chkcashcodecb,t);
	FINDRSCSETFUNC(mtnrsrc,"REFERENCE IDENTIFICATION",changetrantype,t);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"REVENUE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CASH CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	yearid=GETCURRENTFISCALYEAR(fmgt->month);
	FINDRSCSETINT(mtnrsrc,"TAX YEAR",yearid);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* this option isn't used */
			break;
		case 2:
			FINDRSCGETSTRING(mtnrsrc,"REAL ESTATE IDENTIFICATION",
				&realestateid);
			FINDRSCGETSTRING(mtnrsrc,"OWNER IDENTIFICATION",&ownerid);
			FINDRSCGETINT(mtnrsrc,"TAX YEAR",&yearid);
			FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&tranno);
			FINDRSCSETSHORT(mtnrsrc,"MONTHS",xmonth);
			FINDFLDSETSTRING(t->fileno,"REAL ESTATE IDENTIFICATION",
				realestateid);
			FINDFLDSETSTRING(t->fileno,"OWNER IDENTIFICATION",ownerid);
			FINDFLDSETINT(t->fileno,"TAX YEAR",yearid);
			FINDFLDSETINT(t->fileno,"TRANSACTION NUMBER",tranno);
			ymonth=(short)xmonth;
			FINDFLDSETINT(t->fileno,"MONTH",ymonth);
			if(realestateid!=NULL) Rfree(realestateid);
			if(ownerid!=NULL) Rfree(ownerid);
			break;
		default:
		case 0:
			if(EQLNRDKEY(t->fileno,1,targetkey)) 
			{
				KEYNRD(t->fileno,1);
			} else { 
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				FINDFLDGETSHORT(t->fileno,"DEFINITION TYPE",&deftype);
				x=(int)deftype;
				if(x>=DEFtypes->numlibs)
				{
					addAPPlib(DEFtypes,"Invalid Definition Type");
					x=DEFtypes->numlibs-1;
				}
				FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
				FINDFLDGETSHORT(t->fileno,"MONTH",&ymonth);
				x=(int)ymonth;
				FINDRSCSETINT(mtnrsrc,"MONTHS",x);
				chkaccountcode(mtnrsrc,t);
				chkrevenuecode(mtnrsrc,t);
				chkcashcode(mtnrsrc,t);
			}
			break;
	}
	SetVirFieldFuncs(mtnrsrc);
	getsupportingfiles(mtnrsrc,t);
	addbtnrsrc(mtnrsrc,"PREVIOUS",TRUE,getprevious,t);
	addbtnrsrc(mtnrsrc,"NEXT",TRUE,getnext,t);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,ApplySubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	APPmakescrn(mtnrsrc,FALSE,quit_record,dbsort,FALSE);
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
	char *temp=NULL,*warnmessage=NULL,delflag=FALSE,*ownid=NULL;
	int yearid=0,selected=0;
	short type=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",type);
	FINDRSCGETINT(mtnrsrc,"MONTHS",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(dbsort->fileno,"MONTH",type);
	FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		p=Rmalloc(sizeof(PassableStruct));
		p->r=mtnrsrc;
		p->a=dbsort;
		if(rstmst!=(-1))
		{
			FINDFLDGETSTRING(dbsort->fileno,"REAL ESTATE IDENTIFICATION",&temp);
			FINDFLDGETSTRING(dbsort->fileno,"OWNER IDENTIFICATION",&ownid);
			FINDFLDGETINT(dbsort->fileno,"TAX YEAR",&yearid);
			FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",temp);
			FINDFLDSETSTRING(rstmst,"OWNER IDENTIFICATION",ownid);
			FINDFLDSETINT(rstmst,"TAX YEAR",yearid);
			if(ADVEQLNRDsec(rstmst,1,ApplySubData,p))
			{
				KEYNRD(rstmst,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+137);
				sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Real Estate Master.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The REAL ESTATE IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTMSTR] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(rstmst,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exists as a Real Estate, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exist as a Real Estate, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(fsclyr!=(-1))
		{
			FINDFLDGETINT(dbsort->fileno,"TAX YEAR",&yearid);
			FINDFLDSETINT(fsclyr,"FISCAL YEAR",yearid);
			if(ADVEQLNRDsec(fsclyr,1,ApplySubData,p))
			{
				KEYNRD(fsclyr,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+137);
				sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is not valid.  It does not exist as a Fiscal Year.",yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The FISCAL YEAR of [%d] entered does not exist in the [FINMGT][FINYEAR] data file.",yearid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(fsclyr,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is deleted.  It exists as a Fiscal Year, but is presently set up in a deleted state.",yearid);
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The FISCAL YEAR of [%d] entered is deleted.  It exist as a Fiscal Year, but is presently set up in a deleted state.",yearid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				} else {
					FINDFLDGETCHAR(fsclyr,"ACTIVE",&delflag);
					if(!delflag)
					{
						warnmessage=Rmalloc(RDAstrlen(temp)+140);
						sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is not active.  It exists as a Fiscal Year, but is presently set up in an inactive state.",yearid);
						WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
						prterr("The FISCAL YEAR of [%d] entered is not active.  It exists as a Fiscal Year, but is presently set up in an inactive state.",yearid);
						if(warnmessage!=NULL) Rfree(warnmessage);
						return;
					}
				}
			}
		}
		if(p!=NULL) Rfree(p);
		if(chkaccountcode(mtnrsrc,dbsort)) 
		{
			if(chkrevenuecode(mtnrsrc,dbsort))
			{
				if(chkcashcode(mtnrsrc,dbsort))
				{
					if(!checktrantype(mtnrsrc,dbsort)) return;
				} else return;
			} else return;
		} else return;
	}
	if(temp!=NULL) Rfree(temp);
	if(ownid!=NULL) Rfree(ownid);
	save_record(mtnrsrc,update_list,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0,selected=0;
	short type=0;
	RDArsrc *tmprsrc=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",type);
	FINDRSCGETINT(mtnrsrc,"MONTHS",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(dbsort->fileno,"MONTH",type);
	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN","RLSTMGT","Browse Diagnostic",NULL);
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
	if(WindowCount<2) apply_backend(dbsort);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *exprssn=NULL;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	COPYFIELD(dbsort->fileno,rstmst,"REAL ESTATE IDENTIFICATION");
	COPYFIELD(dbsort->fileno,rstmst,"OWNER IDENTIFICATION");
	COPYFIELD(dbsort->fileno,rstmst,"TAX YEAR");
	if(ADVEQLNRDsec(rstmst,1,ApplySubData,p)) KEYNRD(rstmst,1);
	filerecord2rsrc(rstmst,mtnrsrc);
	COPYFIELD(dbsort->fileno,classnum,"CLASS IDENTIFICATION");
	if(ADVEQLNRDsec(classnum,1,ApplySubData,p)) KEYNRD(classnum,1);
	filerecord2rsrc(classnum,mtnrsrc);
	COPYFIELD(dbsort->fileno,rstci,"CLASS IDENTIFICATION");
	COPYFIELD(dbsort->fileno,rstci,"INTEREST IDENTIFICATION");
	if(ADVEQLNRDsec(rstci,1,ApplySubData,p)) KEYNRD(rstci,1);
	filerecord2rsrc(rstci,mtnrsrc);
	FIELDCOPY(dbsort->fileno,"TAX YEAR",fsclyr,"FISCAL YEAR");
	if(ADVEQLNRDsec(fsclyr,1,ApplySubData,p)) KEYNRD(fsclyr,1);
	filerecord2rsrc(fsclyr,mtnrsrc);
	FINDFLDGETSTRING(rstci,"INTEREST FORMULA",&exprssn);
	AMOUNT=0.0;
	if(exprssn!=NULL)
	{
		AMOUNT=EVALUATEdbl(exprssn,ApplySubData,p);
		AMOUNT=round(AMOUNT);
		Rfree(exprssn);
	}
	if(p!=NULL) Rfree(p);
	FINDFLDSETDOUBLE(dbsort->fileno,"AMOUNT",AMOUNT);
	FINDRSCSETDOUBLE(mtnrsrc,"AMOUNT",AMOUNT);
}
