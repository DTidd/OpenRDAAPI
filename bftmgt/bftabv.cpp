/* bftabv.c - Benefit Management's Add Benefit Vouchers */
#ifndef WIN32
#define __NAM__ "bftabv.lnx"
#endif
#ifdef WIN32
#define __NAM__ "bftabv.exe"
#endif
#include <app.hpp>

#include <rptgen.hpp>
#include <ldval.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static APPlib *banklist=NULL,*DEFtypes=NULL;
static char *module="BFTMGT",**selecttype=NULL,*reportname=NULL;
static char *select_expression=NULL,*bank_id=NULL;
static char **monthlist=NULL,**fmonthlist=NULL;
static short bftmstr=(-1),bftdesc=(-1),bftmdac=(-1),bftddac=(-1);
static short payddsv=(-1),payddac=(-1),payddms=(-1),venmst=(-1),bnkacc=(-1);
static short finbacc=(-1),finbyr=(-1),finbact=(-1),venadr=(-1),yearnum=(-1);
static short perdmg=(-1),paydtyp=(-1),paydedm=(-1),paypcsm=(-1);
static short paypfsm=(-1),paydcsm=(-1),paydfsm=(-1);
static int select_type=0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static void save_record(RDArsrc *,short,DBsort *);
static void save_check(RDArsrc *,short,DBsort *);
static short check_code(RDArsrc *,DBsort *,short);
static short check_debcode(RDArsrc *,DBsort *);
static short check_credcode(RDArsrc *,DBsort *);
static void debcodecb(RDArsrc *,DBsort *);
static void credcodecb(RDArsrc *,DBsort *);
static void clearwidget(RDArsrc *,char *);
static void quit_record(RDArsrc *,DBsort *);
static void save_record_up(RDArsrc *,DBsort *);
static void save_record_nup(RDArsrc *,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
static void changemonth(RDArsrc *);
static void DEFtypecb(RDArsrc *);
static void setscrlist(RDArsrc *,short);
static void getvendor(RDArsrc *,DBsort *);
static void getperid(RDArsrc *,DBsort *);
static void getaddr(RDArsrc *,DBsort *);
static void getbank(RDArsrc *,DBsort *);
/*
static char *getvendoraddress(RDArsrc *,DBsort *,char *);
*/
static void setfiles(RDArsrc *,DBsort *);
static void getfile(RDArsrc *,DBsort *);
static void readscreen(RDArsrc *,DBsort *);
static void getprevious(RDArsrc *,DBsort *);
static void getnext(RDArsrc *,DBsort *);
static void resetdefs(RDArsrc *,DBsort *);
static RDAfinmgt *fmgt=NULL;
DBsort *dbsort=NULL;
static void selectm(short,void *,int,char **,DBsort *);
static int VOUCHER_COUNT=0;

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static void BftAbvSubData(char **tmp,PassableStruct *p)
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
		prterr("DIAG BftAbvSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
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
						temp=EVALUATEstr(v->expression,BftAbvSubData,p);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=temp;
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
						if(RDAstrlen(gn)>(start-1+length))
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
						*v->value.short_value=EVALUATEsht(v->expression,BftAbvSubData,p);
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
						*v->value.integer_value=EVALUATEint(v->expression,BftAbvSubData,p);
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
						*v->value.float_value=EVALUATEdbl(v->expression,BftAbvSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,BftAbvSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,BftAbvSubData,p);
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
		prterr("DIAG BftAbvSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
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
	AddFileFields(list,bftmdac);
	AddFileFields(list,bftddac);
	AddFileFields(list,bftmstr);
	AddFileFields(list,bftdesc);
	AddFileFields(list,payddsv);
	AddFileFields(list,payddac);
	AddFileFields(list,payddms);
	AddFileFields(list,paydtyp);
	AddFileFields(list,venmst);
	AddFileFields(list,venadr);
	AddFileFields(list,perdmg);
	AddFileFields(list,paydedm);
	AddFileFields(list,paypcsm);
	AddFileFields(list,paypfsm);
	AddFileFields(list,paydcsm);
	AddFileFields(list,paydfsm);
	AddFileFields(list,bnkacc);
	AddFileFields(list,finbacc);
	AddFileFields(list,finbyr);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"BFTMDAC",NULL,dumb);
	addDFincvir(tmprsrc,module,"BFTDDAC",NULL,dumb);
	addDFincvir(tmprsrc,module,"BFTMSTR",NULL,dumb);
	addDFincvir(tmprsrc,module,"BFTDESC",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDDSV",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDDAC",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDDMS",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDTYP",NULL,dumb);
	addDFincvir(tmprsrc,"VENPMT","VENMSR",NULL,dumb);
	addDFincvir(tmprsrc,"VENPMT","VENADR",NULL,dumb);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDEDM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDCSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDFSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYPCSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYPFSM",NULL,dumb);
	addDFincvir(tmprsrc,"BNKREC","BNKACC",NULL,dumb);
	addDFincvir(tmprsrc,"FINMGT","FINBACC",NULL,finbacc);
	addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,finbyr);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/BFTMGT.VIR",CURRENTDIRECTORY);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
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
	short ef=FALSE,update_diag=FALSE,deftype=0,cpmonth=0,fpmonth=0;
	char deleteflag=FALSE,*curdate=NULL,*curtime=NULL,*dedid=NULL;
	char *perid=NULL;
	int x,numfields=0,trans=0,fiscal_year=0;
	NRDfield *fields=NULL,*field=NULL;
	double amt=0,total=0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev_file=NULL;
	PassableStruct *p=NULL;
	char *e=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Creating Vendor Withholding Vouchers...",NULL);
		addDFincvir(tmprsrc,module,"BFTMDAC",NULL,bftmdac);
		addDFincvir(tmprsrc,module,"BFTDDAC",NULL,bftddac);
		addDFincvir(tmprsrc,module,"BFTMSTR",NULL,bftmstr);
		addDFincvir(tmprsrc,module,"BFTDESC",NULL,bftdesc);
		addDFincvir(tmprsrc,"PAYROLL","PAYDDAC",NULL,payddac);
		addDFincvir(tmprsrc,"PAYROLL","PAYDDSV",NULL,payddsv);
		addDFincvir(tmprsrc,"VENPMT","VENMSR",NULL,venmst);
		addDFincvir(tmprsrc,"VENPMT","VENADR",NULL,venadr);
		addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,perdmg);
		addDFincvir(tmprsrc,"PAYROLL","PAYDEDM",NULL,paydedm);
		addDFincvir(tmprsrc,"PAYROLL","PAYDCSM",NULL,paydcsm);
		addDFincvir(tmprsrc,"PAYROLL","PAYDFSM",NULL,paydfsm);
		addDFincvir(tmprsrc,"PAYROLL","PAYPCSM",NULL,paypcsm);
		addDFincvir(tmprsrc,"PAYROLL","PAYPFSM",NULL,paypfsm);
		addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,finbyr);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
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
		curdate=GETCURRENTDATE10();
		numfields=NUMFLDS(dbsort->fileno);
		fields=FLDPOINTER(dbsort->fileno);
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(!deleteflag)
			{
				for(x=0,field=fields;x<numfields;++x,++field)
				{
					if(!RDAstrcmp(field->name,"FISCAL YEAR"))
					{
						FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",
							&fiscal_year);
						FINDFLDSETINT(payddsv,"FISCAL YEAR",fiscal_year);
					} else if(!RDAstrcmp(field->name,"CALENDAR PROCESSING MONTH"))
					{
						FINDFLDGETSHORT(dbsort->fileno,"CALENDAR PROCESSING MONTH",&cpmonth);
						FINDFLDSETSHORT(payddsv,"CALENDAR PROCESSING MONTH",cpmonth);
					} else if(!RDAstrcmp(field->name,"FISCAL PROCESSING MONTH"))
					{
						FINDFLDGETSHORT(dbsort->fileno,"FISCAL PROCESSING MONTH",&fpmonth);
						FINDFLDSETSHORT(payddsv,"FISCAL PROCESSING MONTH",fpmonth);
					} else if(!RDAstrcmp(field->name,"TRANSACTION NUMBER"))
					{
						FINDFLDSETINT(payddsv,"TRANSACTION NUMBER",trans);
					} else if(!RDAstrcmp(field->name,"PERSONNEL IDENTIFICATION"))
					{
						FINDFLDGETSTRING(dbsort->fileno,
							"PERSONNEL IDENTIFICATION",&perid);
					} else if(!RDAstrcmp(field->name,
						"DEDUCTION DESCRIPTION IDENTIFICATION"))
					{
						FINDFLDGETSTRING(dbsort->fileno,
							"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
						FINDFLDSETSTRING(payddsv,
							"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
					} else if(!RDAstrcmp(field->name,"DEFINITION TYPE"))
					{
						FINDFLDGETSHORT(dbsort->fileno,
							"DEFINITION TYPE",&deftype);
						FINDFLDSETSHORT(payddsv,"DEFINITION TYPE",deftype);
					} else if(!RDAstrcmp(field->name,"AMOUNT"))
					{
						FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amt);
						FINDFLDSETDOUBLE(payddsv,"AMOUNT",amt);
					} else if(!RDAstrcmp(field->name,"ENTRY DATE"))
					{
						FINDFLDSETSTRING(payddsv,"ENTRY DATE",curdate);
					} else if(!RDAstrcmp(field->name,"ENTRY TIME"))
					{
						FINDFLDSETSTRING(payddsv,"ENTRY TIME",curtime);
					} else if(!RDAstrcmp(field->name,"SOURCE USER"))
					{
						FINDFLDSETSTRING(payddsv,"SOURCE USER",USERLOGIN);
					} else if(!RDAstrcmp(field->name,"DELETEFLAG"))
					{
						FINDFLDSETCHAR(payddsv,"DELETEFLAG",FALSE);
					} else {
						COPYFIELD(dbsort->fileno,payddsv,field->name);
					} 
				}
				ADVWRTTRANSsec(payddsv,1,"TRANSACTION NUMBER",NULL,BftAbvSubData,p);
				++VOUCHER_COUNT;
				ADVupdateundist(payddsv,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),finbyr,fiscal_year,deftype,"EXPENDITURES",FISCALPERIOD[fpmonth],amt,BftAbvSubData,p);
				FINDFLDSETSTRING(bftmdac,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETSTRING(bftmdac,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
				FINDFLDSETSHORT(bftmdac,"DEFINITION TYPE",deftype);
				if(!ADVEQLNRDsec(bftmdac,1,BftAbvSubData,p))
				{
					LOCNRD(bftmdac);
					prev_file=RDATDataNEW(bftmdac);
					FINDFLDGETDOUBLE(bftmdac,"UNPROCESSED BALANCE",&total);
					total-=amt;
					FINDFLDSETDOUBLE(bftmdac,"UNPROCESSED BALANCE",total);
					FINDFLDGETDOUBLE(bftmdac,"PROCESSED BALANCE",&total);
					total+=amt;
					FINDFLDSETDOUBLE(bftmdac,"PROCESSED BALANCE",total);
					ADVWRTTRANSsec(bftmdac,0,NULL,prev_file,BftAbvSubData,p);
					UNLNRD(bftmdac);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				} else {
					prterr("Error BFTMDAC record unavailable for updating balances: Personnel Identification of [%s], Deduction Description Identification of [%s], and Definition Type of [%d].",(perid==NULL?"":perid),(dedid=NULL?"":dedid),deftype);
				}
				FINDFLDSETSTRING(bftddac,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
				FINDFLDSETSHORT(bftddac,"DEFINITION TYPE",deftype);
				if(!ADVEQLNRDsec(bftddac,1,BftAbvSubData,p))
				{
					LOCNRD(bftddac);
					prev_file=RDATDataNEW(bftddac);
					FINDFLDGETDOUBLE(bftddac,"UNPROCESSED BALANCE",&total);
					total-=amt;
					FINDFLDSETDOUBLE(bftddac,"UNPROCESSED BALANCE",total);
					FINDFLDGETDOUBLE(bftddac,"PROCESSED BALANCE",&total);
					total+=amt;
					FINDFLDSETDOUBLE(bftddac,"PROCESSED BALANCE",total);
					ADVWRTTRANSsec(bftddac,0,NULL,prev_file,BftAbvSubData,p);
					UNLNRD(bftmdac);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				} else {
					prterr("Error BFTDDAC record unavailable for updating balances: Deduction Description Identification of [%s] and Definition Type of [%d].",(dedid=NULL?"":dedid),deftype);
				}
				FINDFLDSETSTRING(bftmstr,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETSTRING(bftmstr,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
				if(!ADVEQLNRDsec(bftmstr,1,BftAbvSubData,p))
				{
					LOCNRD(bftmstr);
					prev_file=RDATDataNEW(bftmstr);
					FINDFLDGETDOUBLE(bftmstr,"UNPROCESSED BALANCE",&total);
					total-=amt;
					FINDFLDSETDOUBLE(bftmstr,"UNPROCESSED BALANCE",total);
					FINDFLDGETDOUBLE(bftmstr,"PROCESSED BALANCE",&total);
					total+=amt;
					FINDFLDSETDOUBLE(bftmstr,"PROCESSED BALANCE",total);
					ADVWRTTRANSsec(bftmstr,0,NULL,prev_file,BftAbvSubData,p);
					UNLNRD(bftmstr);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				} else {
					prterr("Error BFTMSTR record unavailable for updating balances: Personnel Identification of [%s] and Deduction Description Identification of [%s].",(perid==NULL?"":perid),(dedid=NULL?"":dedid));
				}
				FINDFLDSETSTRING(bftdesc,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
				if(!ADVEQLNRDsec(bftdesc,1,BftAbvSubData,p))
				{
					LOCNRD(bftdesc);
					prev_file=RDATDataNEW(bftdesc);
					FINDFLDGETDOUBLE(bftdesc,"UNPROCESSED BALANCE",&total);
					total-=amt;
					FINDFLDSETDOUBLE(bftdesc,"UNPROCESSED BALANCE",total);
					FINDFLDGETDOUBLE(bftdesc,"PROCESSED BALANCE",&total);
					total+=amt;
					FINDFLDSETDOUBLE(bftdesc,"PROCESSED BALANCE",total);
					ADVWRTTRANSsec(bftdesc,0,NULL,prev_file,BftAbvSubData,p);
					UNLNRD(bftdesc);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				} else {
					prterr("Error BFTDESC record unavailable for updating balances: Deduction Description Identification of [%s].",(dedid=NULL?"":dedid));
				}
				FINDFLDSETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
				FINDFLDSETSHORT(payddac,"DEFINITION TYPE",deftype);
				if(!ADVEQLNRDsec(payddac,1,BftAbvSubData,p))
				{
					LOCNRD(payddac);
					prev_file=RDATDataNEW(payddac);
					FINDFLDGETDOUBLE(payddac,"AMOUNT",&total);
					total-=amt;
					FINDFLDSETDOUBLE(payddac,"AMOUNT",total);
					ADVWRTTRANSsec(payddac,0,NULL,prev_file,BftAbvSubData,p);
					UNLNRD(payddac);
					if(prev_file!=NULL) FreeRDATData(prev_file);
				} else {
					prterr("Error PAYDDAC record unavailable for updating balances: Deduction Description Identification of [%s] and Definition Type of [%d].",(dedid=NULL?"":dedid),deftype);
				}
				update_diag=TRUE;
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(p!=NULL) Rfree(p);
		if(dedid!=NULL) Rfree(dedid);
		if(perid!=NULL) Rfree(perid);
		if(curdate!=NULL) Rfree(curdate);
		if(curtime!=NULL) Rfree(curtime);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
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
	d=getDFvirtual(module,"ADD BENEFIT VOUCHERS SORT SEQUENCE");
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
		addDBfield(dbsrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsrt,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	numfields=NUMFLDS(payddsv);
	FIELDS=FLDPOINTER(payddsv);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(RDAstrcmp(field->name,"DELETEFLAG"))
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
			}
		}
		addDBfilefield(dbsrt,bftmstr,"PERSONNEL IDENTIFICATION");
	}
	key=KEYNUM(payddsv,1);
	if(key!=NULL)
	{
		addDBkeypart(dbsrt,keyno,"PERSONNEL IDENTIFICATION");
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
}
/*
static char *getvendoraddress(RDArsrc *parent,DBsort *dbsort,char *vendorid)
{
	short ef=FALSE;
	char billing=FALSE,deleteflag=FALSE,*addrid=NULL;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=parent;
	p->a=dbsort;
	ZERNRD(venadr);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
	ef=ADVGTENRDsec(venadr,1,BftAbvSubData,p);
	while(!ef)
	{
		FINDFLDGETCHAR(venadr,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETCHAR(venadr,"DEFAULT BILLING",&billing);
			if(billing) 
			{
				FINDFLDGETSTRING(venadr,"ADDRESS IDENTIFICATION",&addrid);
				break;
			}
		}
		ef=ADVNXTNRDsec(venadr,1,BftAbvSubData,p);
	}
	if(p!=NULL) Rfree(p);
	return(addrid);
}
*/
void addsortrecord(DBsort *dbsrt,RDArsrc *mainrsrc,char *perid,
			char *ded_descid,short deftype,char *cash_code,
			char *liability_code,int fiscal_year,short fpmonth,
			int calyear,short cpmonth,char *vendorid,
			char *venadrid,char *bankid,char *descript,double amt)
{
	char deleteflag=FALSE;
	PassableStruct *p=NULL;

	ZERNRD(dbsrt->fileno);
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		p=Rmalloc(sizeof(PassableStruct));
		p->r=mainrsrc;
		p->a=dbsort;
		computevirtual(csortsequence,BftAbvSubData,p);
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
	FINDFLDSETSTRING(dbsrt->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(dbsrt->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",ded_descid);
	FINDFLDSETSHORT(dbsrt->fileno,"DEFINITION TYPE",deftype);
/*
	FINDFLDSETSTRING(dbsrt->fileno,"DEBIT CODE",cash_code);
	FINDFLDSETSTRING(dbsrt->fileno,"CREDIT CODE",liability_code);
*/
	FINDFLDSETSTRING(dbsrt->fileno,"DEBIT CODE",liability_code);
	FINDFLDSETSTRING(dbsrt->fileno,"CREDIT CODE",cash_code);
	FINDFLDSETDOUBLE(dbsrt->fileno,"AMOUNT",amt);
	FINDFLDSETSTRING(dbsrt->fileno,"DESCRIPTION",descript);
	FINDFLDSETSTRING(dbsrt->fileno,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(dbsrt->fileno,"ADDRESS IDENTIFICATION",venadrid);
/*
	addressid=getvendoraddress(mainrsrc,dbsort,vendorid);
*/
	FINDFLDSETINT(dbsrt->fileno,"FISCAL YEAR",fiscal_year);
	FINDFLDSETSHORT(dbsrt->fileno,"FISCAL PROCESSING MONTH",fpmonth);
	FINDFLDSETINT(dbsrt->fileno,"CALENDAR YEAR",calyear);
	FINDFLDSETSHORT(dbsrt->fileno,"CALENDAR PROCESSING MONTH",cpmonth);
	FINDFLDSETSTRING(dbsrt->fileno,"BANK IDENTIFICATION",bankid);
	WRTNRD(dbsrt->fileno,1,"TRANSACTION NUMBER");
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	selectm(0,targetkey,0,NULL,blist->funcargs);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	selectm(2,NULL,0,NULL,blist->funcargs);
} 
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","BENEFIT VOUCHER RECORDS NOT PROCESSED!",
			"Are you sure that you want to quit without processing the Benefit Vouchers?",
			doexit,NULL,FALSE,blist);
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
	DBsort *x=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

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
	if(WindowCount<=2) select_backend(x);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static void dobrowse(RDArsrc *mainrsrc,DBsort *dbsort)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	brsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS BROWSE");
	srsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS SEARCH BROWSE");
	drsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS DEFINE LIST");
	addDFincvir(drsrc,module,"BFTMDAC",NULL,bftmdac);
	addDFincvir(drsrc,module,"BFTDDAC",NULL,bftddac);
	addDFincvir(drsrc,module,"BFTMSTR",NULL,bftmstr);
	addDFincvir(drsrc,module,"BFTDESC",NULL,bftdesc);
	addDFincvir(drsrc,"PAYROLL","PAYDDMS",NULL,payddms);
	addDFincvir(drsrc,"PAYROLL","PAYDTYP",NULL,paydtyp);
	addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmst);
	addDFincvir(drsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(drsrc,"PAYROLL","PAYDEDM",NULL,paydedm);
	addDFincvir(drsrc,"PAYROLL","PAYDCSM",NULL,paydcsm);
	addDFincvir(drsrc,"PAYROLL","PAYDFSM",NULL,paydfsm);
	addDFincvir(drsrc,"PAYROLL","PAYPCSM",NULL,paypcsm);
	addDFincvir(drsrc,"PAYROLL","PAYPFSM",NULL,paypfsm);
	addDFincvir(drsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(srsrc,module,"BFTMDAC",NULL,bftmdac);
	addDFincvir(srsrc,module,"BFTDDAC",NULL,bftddac);
	addDFincvir(srsrc,module,"BFTMSTR",NULL,bftmstr);
	addDFincvir(srsrc,module,"BFTDESC",NULL,bftdesc);
	addDFincvir(srsrc,"PAYROLL","PAYDDMS",NULL,payddms);
	addDFincvir(srsrc,"PAYROLL","PAYDTYP",NULL,paydtyp);
	addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmst);
	addDFincvir(srsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(srsrc,"BNKREC","BNKACC",NULL,bnkacc);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	ZERNRD(dbsort->fileno);
	addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
	mbl_trans=ADVbrowse(dbsort->fileno,1,
		brsrc,srsrc,drsrc,NULL,selectfunction,
		doexit_test,okfunc,new_record,dbsort,
		TRUE,NULL,0,0,0,TRUE);
}
static void nobrowse(RDArsrc *mainrsrc,DBsort *dbsort)
{
	CLSDBsort(dbsort);
	FreeDBsort(dbsort);
}
void addvouchers(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,etest=FALSE,dodiag=FALSE;
	short testfile=FALSE,cpmonth=0,fpmonth=0,deftype=0,keyrange=FALSE;
	int selected_bank=0,x,fiscal_year=0,calyear=0;
	long size_of_file=0;
	char deleteflag=FALSE,*sortname=NULL,*benefit_desc=NULL;
	char *perid=NULL,*notif_formula=NULL,*vendorid=NULL;
	char *venaddrid=NULL,*paydtypeid=NULL;
	char *cash_code=NULL,*liability_code=NULL,*descript=NULL;
	char *detail_desc=NULL;
	double unprocbal=0.0,amt=0.0/* ,amount=0.0 */;
	RDAvirtual *v=NULL;
	PassableStruct *p=NULL;
	char *e=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"BANK IDENTIFICATIONS",&selected_bank);
	FINDRSCGETINT(mainrsrc,"SELECT TYPE",&select_type);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&select_expression);
	FINDRSCGETINT(mainrsrc,"FISCAL YEAR",&fiscal_year);
	FINDRSCGETINT(mainrsrc,"CALENDAR YEAR",&calyear);
	FINDRSCGETSHORT(mainrsrc,"FISCAL PROCESSING MONTHS",&fpmonth);
	FINDRSCGETSHORT(mainrsrc,"CALENDAR PROCESSING MONTHS",&cpmonth);
/*
	FINDRSCGETSHORT(mainrsrc,"MONTHS",&pmonth);
	pmonth+=1;
*/
	if(bank_id!=NULL) Rfree(bank_id);
	if(RDAstrcmp(banklist->libs[selected_bank],"No Banks Available"))
	{
		bank_id=stralloc(banklist->libs[selected_bank]);
	} else bank_id=NULL;
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(bftmdac,rs);
	ReadRangeScreen(bftddac,rs);
	ReadRangeScreen(bftmstr,rs);
	ReadRangeScreen(bftdesc,rs);
	ReadRangeScreen(payddac,rs);
	ReadRangeScreen(payddms,rs);
	ReadRangeScreen(paydtyp,rs);
	ReadRangeScreen(venmst,rs);
	ReadRangeScreen(venadr,rs);
	ReadRangeScreen(perdmg,rs);
	ReadRangeScreen(paydedm,rs);
	ReadRangeScreen(paydcsm,rs);
	ReadRangeScreen(paydfsm,rs);
	ReadRangeScreen(paypcsm,rs);
	ReadRangeScreen(paypfsm,rs);
	tmprsrc=diagscrn(bftmdac,"DIAGNOSTIC SCREEN",module,
		"Selecting Benefits",NULL);
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
	p->r=mainrsrc;
	p->a=dbsort; 
	ZERNRD(bftmdac);
	keyrange=setfilekeyFROMrangersrc(bftmdac,1,rs);
	if(keyrange)
	{
		ef=ADVGTENRDsec(bftmdac,1,BftAbvSubData,p);
	} else {
		ef=ADVFRSNRDsec(bftmdac,1,BftAbvSubData,p);
	}
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETSTRING(bftmdac,"DEDUCTION DESCRIPTION IDENTIFICATION",&benefit_desc);
		if(!isEMPTY(benefit_desc)) 
		{
		FINDFLDGETCHAR(bftmdac,"DELETEFLAG",&deleteflag);
		if(!deleteflag) 
		{
		FINDFLDGETSTRING(bftmdac,"PERSONNEL IDENTIFICATION",&perid);
		FINDFLDGETSHORT(bftmdac,"DEFINITION TYPE",&deftype);
		FINDFLDGETDOUBLE(bftmdac,"UNPROCESSED BALANCE",&unprocbal);
		if(unprocbal>0.0) 
		{
		testfile=testfilerangersrc(bftmdac,rs);
		if(!testfile)
		{
			COPYFIELD(bftmdac,bftmstr,"DEDUCTION DESCRIPTION IDENTIFICATION");
			COPYFIELD(bftmdac,bftmstr,"PERSONNEL IDENTIFICATION");
			if(ADVEQLNRDsec(bftmstr,1,BftAbvSubData,p)) KEYNRD(bftmstr,1);
			FINDFLDGETCHAR(bftmstr,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
			testfile=testfilerangersrc(bftmstr,rs);
			if(!testfile)
			{
				COPYFIELD(bftmdac,bftddac,"DEDUCTION DESCRIPTION IDENTIFICATION");
				COPYFIELD(bftmdac,bftddac,"DEFINITION TYPE");
				if(ADVEQLNRDsec(bftddac,1,BftAbvSubData,p)) KEYNRD(bftddac,1);
				FINDFLDGETCHAR(bftddac,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				testfile=testfilerangersrc(bftddac,rs);
				if(!testfile)
				{
				COPYFIELD(bftddac,payddac,"DEDUCTION DESCRIPTION IDENTIFICATION");
				COPYFIELD(bftddac,payddac,"DEFINITION TYPE");
				if(ADVEQLNRDsec(payddac,1,BftAbvSubData,p)) KEYNRD(payddac,1);
				FINDFLDGETCHAR(payddac,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				FINDFLDGETSTRING(payddac,"CASH CODE",&cash_code);
				FINDFLDGETSTRING(payddac,"LIABILITY CODE",&liability_code);
/*
				FINDFLDGETDOUBLE(payddac,"AMOUNT",&amount);
				if(amount>0.0)
				{
*/
				testfile=testfilerangersrc(payddac,rs);
				if(!testfile)
				{
				COPYFIELD(bftddac,bftdesc,"DEDUCTION DESCRIPTION IDENTIFICATION");
				if(ADVEQLNRDsec(bftdesc,1,BftAbvSubData,p)) KEYNRD(bftdesc,1);
				FINDFLDGETCHAR(bftdesc,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				testfile=testfilerangersrc(bftdesc,rs);
				if(!testfile)
				{
					COPYFIELD(bftmstr,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
					if(ADVEQLNRDsec(payddms,1,BftAbvSubData,p)) KEYNRD(payddms,1);
					FINDFLDGETCHAR(payddms,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					FINDFLDGETCHAR(payddms,"ACTIVE",&deleteflag);
					if(deleteflag)
					{
					testfile=testfilerangersrc(payddms,rs);
					if(!testfile)
					{
					FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&paydtypeid);
					FINDFLDSETSTRING(paydtyp,"DEDUCTION TYPE",paydtypeid);
					if(ADVEQLNRDsec(paydtyp,1,BftAbvSubData,p)) KEYNRD(paydtyp,1);
					FINDFLDGETCHAR(paydtyp,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					testfile=testfilerangersrc(paydtyp,rs);
					if(!testfile)
					{
/*
						FINDFLDGETSTRING(payddms,"SHORT DESCRIPTION",&descript);
*/
						FINDFLDGETSTRING(payddms,"VENDOR IDENTIFICATION",&vendorid);
						FINDFLDSETSTRING(venmst,"VENDOR IDENTIFICATION",vendorid);
						if(ADVEQLNRDsec(venmst,1,BftAbvSubData,p)) KEYNRD(venmst,1);
						FINDFLDGETCHAR(venmst,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
						FINDFLDGETCHAR(venmst,"ACTIVE",&deleteflag);
						if(deleteflag)
						{
						testfile=testfilerangersrc(venmst,rs);
						if(!testfile)
						{
							FINDFLDGETSTRING(payddms,"ADDRESS IDENTIFICATION",&venaddrid);
							FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
							FINDFLDSETSTRING(venadr,"VENDOR ADDRESS",venaddrid);
							if(ADVEQLNRDsec(venadr,1,BftAbvSubData,p)) KEYNRD(venmst,1);
							FINDFLDGETCHAR(venadr,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
							testfile=testfilerangersrc(venadr,rs);
							if(!testfile)
							{
							FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",perid);
							if(ADVEQLNRDsec(perdmg,1,BftAbvSubData,p)) KEYNRD(perdmg,1);
							FINDFLDGETCHAR(perdmg,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
							FINDFLDGETCHAR(perdmg,"CURRENT EMPLOYEE",&deleteflag);
							if(deleteflag)
							{
							testfile=testfilerangersrc(perdmg,rs);
							if(!testfile)
							{
							FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",perid);
							FINDFLDSETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",benefit_desc);
							if(ADVEQLNRDsec(paydedm,1,BftAbvSubData,p)) KEYNRD(paydedm,1);
							FINDFLDGETCHAR(paydedm,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
							testfile=testfilerangersrc(paydedm,rs);
							if(!testfile)
							{
							FINDFLDSETSTRING(paypcsm,"PERSONNEL IDENTIFICATION",perid);
							FINDFLDSETINT(paypcsm,"CALENDAR YEAR",calyear);
							if(ADVEQLNRDsec(paypcsm,1,BftAbvSubData,p)) KEYNRD(paypcsm,1);
							FINDFLDGETCHAR(paypcsm,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
							testfile=testfilerangersrc(paypcsm,rs);
							if(!testfile)
							{
							FINDFLDSETSTRING(paypfsm,"PERSONNEL IDENTIFICATION",perid);
							FINDFLDSETINT(paypfsm,"FISCAL YEAR",fiscal_year);
							if(ADVEQLNRDsec(paypfsm,1,BftAbvSubData,p)) KEYNRD(paypfsm,1);
							FINDFLDGETCHAR(paypfsm,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
							testfile=testfilerangersrc(paypfsm,rs);
							if(!testfile)
							{
							FINDFLDSETSTRING(paydcsm,"PERSONNEL IDENTIFICATION",perid);
							FINDFLDSETSTRING(paydcsm,"DEDUCTION DESCRIPTION IDENTIFICATION",benefit_desc);
							FINDFLDSETINT(paydcsm,"CALENDAR YEAR",calyear);
							if(ADVEQLNRDsec(paydcsm,1,BftAbvSubData,p)) KEYNRD(paydcsm,1);
							FINDFLDGETCHAR(paydcsm,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
							testfile=testfilerangersrc(paydcsm,rs);
							if(!testfile)
							{
							FINDFLDSETSTRING(paydfsm,"PERSONNEL IDENTIFICATION",perid);
							FINDFLDSETSTRING(paydfsm,"DEDUCTION DESCRIPTION IDENTIFICATION",benefit_desc);
							FINDFLDSETINT(paydfsm,"FISCAL YEAR",fiscal_year);
							if(ADVEQLNRDsec(paydfsm,1,BftAbvSubData,p)) KEYNRD(paydfsm,1);
							FINDFLDGETCHAR(paydfsm,"DELETEFLAG",&deleteflag);
							if(!deleteflag)
							{
							testfile=testfilerangersrc(paydfsm,rs);
							if(!testfile)
							{
								if(!isEMPTY(select_expression))
								{
									etest=EVALUATEbol(select_expression,BftAbvSubData,p);
								} else etest=TRUE;
								if(etest)
								{
									for(x=0,v=mainrsrc->virflds;
										x<mainrsrc->num;++x,++v)
									{
									if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))BftAbvSubData,p)) break;
									}
									if(x>=mainrsrc->num)
									{
									FINDFLDGETSTRING(bftdesc,"NOTIFICATION FORMULA",&notif_formula);
									if(!isEMPTY(notif_formula))
									{
									amt=EVALUATEdbl(notif_formula,BftAbvSubData,p);
									amt=round(amt);
									if(amt>000.0)
									{
									FINDFLDGETSTRING(bftdesc,"DETAIL DESCRIPTION",&detail_desc);
									if(!isEMPTY(detail_desc))
									{
										descript=EVALUATEstr(detail_desc,BftAbvSubData,p);
									} else {
										FINDFLDGETSTRING(payddms,"SHORT DESCRIPTION",&descript);
									}
									addsortrecord(dbsort,mainrsrc,perid,
										benefit_desc,deftype,cash_code,
										liability_code,fiscal_year,
										fpmonth,calyear,cpmonth,vendorid,venaddrid,bank_id,descript,amt);
									dodiag=TRUE;
									} else {
									if(diagapps)
									{

										prterr("Skipped by Zero Value in Calculated Amount of Notification Formula: (first 200 characters)\n[%.200s].",(notif_formula==NULL?"":notif_formula));
									}
									}
									} else {
									if(diagapps)
									{

										prterr("Skipped by EMPTY Notification Formula.");
									}
									}
									} else {
									if(diagapps)
									{

										prterr("Skipped by Ranging on Virtual Field [%s].",v->name);
									}
									}
								} else {
								if(diagapps)
								{
									prterr("Skipped by Optional Select Expression: (first 200 characters)\n[%.200s]",(select_expression==NULL?"":select_expression));
								}
								}
							} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Deduction Fiscal Year Summary [PAYROLL][PAYDFSM], Personnel ID=[%s], Deduction Description ID=[%s], Fiscal Year=[%5d].",(perid==NULL?"":perid),(benefit_desc==NULL?"":benefit_desc),fiscal_year);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of DELETED Deduction Fiscal Year Summary [PAYROLL][PAYDFSM], Personnel ID=[%s], Deduction Description ID=[%s], Fiscal Year=[%5d].",(perid==NULL?"":perid),(benefit_desc==NULL?"":benefit_desc),fiscal_year);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Deduction Calendar Year Summary [PAYROLL][PAYDCSM], Personnel ID=[%s], Deduction Description ID=[%s], Calendar Year=[%5d].",(perid==NULL?"":perid),(benefit_desc==NULL?"":benefit_desc),calyear);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of DELETED Deduction Calendar Year Summary [PAYROLL][PAYDCSM], Personnel ID=[%s], Deduction Description ID=[%s], Calendar Year=[%5d].",(perid==NULL?"":perid),(benefit_desc==NULL?"":benefit_desc),calyear);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Personnel Fiscal Year Summary [PAYROLL][PAYPFSM], Personnel ID=[%s], Fiscal Year=[%5d].",(perid==NULL?"":perid),fiscal_year);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of DELETED Personnel Fiscal Year Summary [PAYROLL][PAYPFSM], Personnel ID=[%s], Fiscal Year=[%5d].",(perid==NULL?"":perid),fiscal_year);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Personnel Calendar Year Summary [PAYROLL][PAYPCSM], Personnel ID=[%s], Calendar Year=[%5d].",(perid==NULL?"":perid),calyear);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of DELETED Personnel Calendar Year Summary [PAYROLL][PAYPCSM], Personnel ID=[%s], Calendar Year=[%5d].",(perid==NULL?"":perid),calyear);
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Personnel Deduction Master Record [PAYROLL][PAYDEDM], Personnel ID=[%s], Deduction Identification=[%s],.",(perid==NULL?"":perid),(benefit_desc==NULL?"":benefit_desc));
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of DELETED Personnel Deduction Master Record [PAYROLL][PAYDEDM], Personnel ID=[%s], Deduction Identification=[%s],.",(perid==NULL?"":perid),(benefit_desc==NULL?"":benefit_desc));
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Personnel Demographics Record [PRSNNL][PERDMG], Personnel ID=[%s].",(perid==NULL?"":perid));
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of DELETED Personnel Demographics Record [PRSNNL][PERDMG], Personnel ID=[%s].",(perid==NULL?"":perid));
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of non-CURRENT EMPLOYEE Personnel Demographics Record [PRSNNL][PERDMG], Personnel ID=[%s].",(perid==NULL?"":perid));
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Vendor Address [VENPMT][VENADR], Vendor ID=[%s], Address ID=[%s].",(vendorid==NULL?"":vendorid),(venaddrid==NULL?"":venaddrid));
							}
							}
							} else {
							if(diagapps)
							{
								prterr("Skipped because of DELETED Vendor Address Record [VENPMT][VENADR], Vendor ID=[%s], Address ID=[%s].",(vendorid==NULL?"":vendorid),(venaddrid==NULL?"":venaddrid));
							}
							}
						} else {
						if(diagapps)
						{
							prterr("Skipped by Ranging on Vendor Master Record [VENPMT][VENMSR], Vendor ID=[%s].",(vendorid==NULL?"":vendorid));
						}
						}
						} else {
						if(diagapps)
						{
							prterr("Skipped because of non-ACTIVE Vendor Master Record [VENPMT][VENMSR], Vendor ID=[%s].",(vendorid==NULL?"":vendorid));
						}
						}
						} else {
						if(diagapps)
						{
							prterr("Skipped because of DELETED Vendor Master Record [VENPMT][VENMSR], Vendor ID=[%s].",(vendorid==NULL?"":vendorid));
						}
						}
					} else {
					if(diagapps)
					{
						prterr("Skipped by Ranging on Deduction Type Master [PAYROLL][PAYDTYP], Deduction Type=[%s].",paydtypeid);
					}
					}
					} else {
					if(diagapps)
					{
						prterr("Skipped because of DELETED Deduction Type Master [PAYROLL][PAYDTYP], Deduction Type=[%s].",paydtypeid);
					}
					}
					} else {
					if(diagapps)
					{
						prterr("Skipped by Ranging on Deduction Description Master [PAYROLL][PAYDDMS], Deduction Description ID=[%s].",benefit_desc);
					}
					}
					} else {
					if(diagapps)
					{
						prterr("Skipped because of non-ACTIVE Deduction Description Master [PAYROLL][PAYDDMS], Deduction Description ID=[%s].",benefit_desc);
					}
					}
					} else {
					if(diagapps)
					{
						prterr("Skipped because of DELETED Deduction Description Master [PAYROLL][PAYDDMS], Deduction Description ID=[%s].",benefit_desc);
					}
					}
				} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on Benefit Description Master Record [BFTMGT][BFTDESC], Benefit Description ID=[%s]",(benefit_desc==NULL?"":benefit_desc));;
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped because of DELETED Benefit Description Master Record [BFTMGT][BFTDESC], Benefit Description ID=[%s]",(benefit_desc==NULL?"":benefit_desc));;
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on Deduction Description Account Code Record [PAYROLL][PAYDDAC], Deduction Description ID=[%s], Definition Type=[%d]",(benefit_desc==NULL?"":benefit_desc),deftype);
				}
				}
/*
				} else {
				if(diagapps)
				{
					prterr("Skipped because of Deduction Description Account Code Record's [PAYROLL][PAYDDAC] Amount less than 0.0, Deduction Description ID=[%s], Definition Type=[%d]",(benefit_desc==NULL?"":benefit_desc),deftype);
				}
				}
*/
				} else {
				if(diagapps)
				{
					prterr("Skipped because of DELETED on Deduction Description Account Code Record [PAYROLL][PAYDDAC], Deduction Description ID=[%s], Definition Type=[%d].",(benefit_desc==NULL?"":benefit_desc),deftype);
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on Benefit Description Account Code Record [BFTMGT][BFTDDAC], Benefit Description ID=[%s], Definition Type=[%d].",(benefit_desc==NULL?"":benefit_desc),deftype);
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped because of DELETED on Benefit Description Account Code Record [BFTMGT][BFTDDAC], Benefit Description ID=[%s], Definition Type=[%d].",(benefit_desc==NULL?"":benefit_desc),deftype);
				}
				}
			} else {
			if(diagapps)
			{
				prterr("Skipped by Ranging on Benefit Master Record [BFTMGT][BFTMSTR], Benefit Description ID=[%s], Personnel ID=[%d].",(benefit_desc==NULL?"":benefit_desc),(perid==NULL?"":perid));
			}
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped because of DELETED on Benefit Master Record [BFTMGT][BFTMSTR], Benefit Description ID=[%s], Personnel ID=[%d].",(benefit_desc==NULL?"":benefit_desc),(perid==NULL?"":perid));
			}
			}
		} else {
		if(diagapps)
		{
			prterr("Skipped by Ranging on Benefit Personnel Description Account Master Record [BFTMGT][BFTMDAC],  Benefit Description ID=[%s], Personnel ID=[%s], Definition Type=[%d]",(benefit_desc==NULL?"":benefit_desc),(perid==NULL?"":perid),deftype);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because of Benefit Personnel Description Account Master Record's [BFTMGT][BFTMDAC] Unprocessed Balance Amount is less than ($ 0.00) (actually amount=[$%.02f]), Benefit Description ID=[%s], Personnel ID=[%s], Definition Type=[%d]",(unprocbal/100),(benefit_desc==NULL?"":benefit_desc),(perid==NULL?"":perid),deftype);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because of DELETED on Benefit Personnel Description Account Master Record [BFTMGT][BFTMDAC], Deduction Description ID=[%s], Personnel ID=[%s], Definition Type=[%d].",(benefit_desc==NULL?"":benefit_desc),(perid==NULL?"":perid),deftype);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because of EMPTY Benefit Description ID in Benefit Personnel Description Account Master Record [BFTMGT][BFTMDAC], Deduction Description ID=[%s], Personnel ID=[%s], Definition Type=[%d].",(benefit_desc==NULL?"":benefit_desc),(perid==NULL?"":perid),deftype);
		}
		}
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		}
		if(keyrange)
		{
			ef=ADVNXTNRDsec(bftmdac,1,BftAbvSubData,p);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(bftmdac,1,rs);
			}
		} else {
			ef=ADVNXTNRDsec(bftmdac,1,BftAbvSubData,p);
		}
/*
		ef=ADVNXTNRDsec(bftmdac,1,BftAbvSubData,p);
*/
	}
	if(p!=NULL) Rfree(p);
	if(benefit_desc!=NULL) Rfree(benefit_desc);
	if(detail_desc!=NULL) Rfree(detail_desc);
	if(perid!=NULL) Rfree(perid);
	if(cash_code!=NULL) Rfree(cash_code);
	if(liability_code!=NULL) Rfree(liability_code);
	if(descript!=NULL) Rfree(descript);
	if(vendorid!=NULL) Rfree(vendorid);
	if(venaddrid!=NULL) Rfree(venaddrid);
	if(bank_id!=NULL) Rfree(bank_id);
	if(notif_formula!=NULL) Rfree(notif_formula);
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
				brsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS BROWSE");
				srsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS DEFINE LIST");
				addDFincvir(drsrc,module,"BFTMDAC",NULL,bftmdac);
				addDFincvir(drsrc,module,"BFTDDAC",NULL,bftddac);
				addDFincvir(drsrc,module,"BFTMSTR",NULL,bftmstr);
				addDFincvir(drsrc,module,"BFTDESC",NULL,bftdesc);
				addDFincvir(drsrc,"PAYROLL","PAYDDMS",NULL,payddms);
				addDFincvir(drsrc,"PAYROLL","PAYDTYP",NULL,paydtyp);
				addDFincvir(drsrc,"VENPMT","VENMSR",NULL,venmst);
				addDFincvir(drsrc,"VENPMT","VENADR",NULL,venadr);
				addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,perdmg);
				addDFincvir(drsrc,"BNKREC","BNKACC",NULL,bnkacc);
				addDFincvir(srsrc,module,"BFTMDAC",NULL,bftmdac);
				addDFincvir(srsrc,module,"BFTDDAC",NULL,bftddac);
				addDFincvir(srsrc,module,"BFTMSTR",NULL,bftmstr);
				addDFincvir(srsrc,module,"BFTDESC",NULL,bftdesc);
				addDFincvir(srsrc,"PAYROLL","PAYDDMS",NULL,payddms);
				addDFincvir(srsrc,"PAYROLL","PAYDTYP",NULL,paydtyp);
				addDFincvir(srsrc,"VENPMT","VENMSR",NULL,venmst);
				addDFincvir(srsrc,"VENPMT","VENADR",NULL,venadr);
				addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,perdmg);
				addDFincvir(srsrc,"BNKREC","BNKACC",NULL,bnkacc);
				GET_SCREEN_VIRTUALS(drsrc,1);
				GET_SCREEN_VIRTUALS(srsrc,2);
				ZERNRD(dbsort->fileno);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				mbl_trans=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,selectfunction,
					doexit_test,okfunc,new_record,dbsort,
					TRUE,NULL,0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
	} else {
/*
		ERRORDIALOG("Add Benefit Vouchers Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
*/
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","NO BENEFIT VOUCHER RECORDS SELECTED!","No records selected using the given select criteria.  Do you wish to process manual amounts?  Otherwise, please try again." ,dobrowse,nobrowse,FALSE,2,mainrsrc,dbsort,NULL);
	}
}
static void makeselecttype()
{
	int x=0; 

	selecttype=Rmalloc(2*sizeof(char *));
	selecttype[0]=stralloc("Automatic");
	selecttype[1]=stralloc("Process Exceptions");
	monthlist=Rmalloc(12*sizeof(char *));
	for(x=0;x<12;++x)
	{
		monthlist[x]=stralloc(CALENDAR_MONTHS[x]);
	}
	fmonthlist=Rmalloc(14*sizeof(char *));
	for(x=0;x<14;++x)
	{
		fmonthlist[x]=stralloc(FISCALPERIOD[x]);
	}
}
static void freeselecttype()
{
	short x;

	if(selecttype!=NULL)
	{
		for(x=0;x<2;++x) Rfree(selecttype[x]);
		Rfree(selecttype);
	}
	if(monthlist!=NULL)
	{
		for(x=0;x<12;++x) Rfree(monthlist[x]);
		Rfree(monthlist);
	}
	if(fmonthlist!=NULL)
	{
		for(x=0;x<14;++x) Rfree(fmonthlist[x]);
		Rfree(fmonthlist);
		fmonthlist=NULL;
	}
}
void quitselect(RDArsrc *mainrsrc)
{
	APPlib *args=NULL;

	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	CLSNRD(bftmdac);
	CLSNRD(bftddac);
	CLSNRD(bftmstr);
	CLSNRD(bftdesc);
	CLSNRD(payddsv);
	CLSNRD(payddms);
	CLSNRD(paydtyp);
	CLSNRD(payddac);
	CLSNRD(venmst);
	CLSNRD(venadr);
	CLSNRD(perdmg);
	CLSNRD(paydedm);
	CLSNRD(paydcsm);
	CLSNRD(paydfsm);
	CLSNRD(paypcsm);
	CLSNRD(paypfsm);
	CLSNRD(bnkacc);
	CLSNRD(finbyr);
	CLSNRD(finbact);
	CLSNRD(finbacc);
	CLSNRD(yearnum);
	if(bank_id!=NULL) Rfree(bank_id);
	if(banklist!=NULL) freeapplib(banklist);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(selecttype!=NULL) freeselecttype();
	if(VOUCHER_COUNT>0)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(reportname!=NULL) Rfree(reportname);
	ShutdownSubsystems();
	exit(0);
}
static void changemonth(RDArsrc *mainrsrc)
{
	int selmonth=0,year=0;

	FINDRSCGETINT(mainrsrc,"CALENDAR PROCESSING MONTHS",&selmonth);
	readwidget(mainrsrc,"CALENDAR YEAR");
	FINDRSCGETINT(mainrsrc,"CALENDAR YEAR",&year);
	if(selmonth>=fmgt->month) year+=1;
	FINDRSCSETINT(mainrsrc,"FISCAL YEAR",year);
	updatersrc(mainrsrc,"FISCAL YEAR");
	FINDRSCSETINT(mainrsrc,"FISCAL PROCESSING MONTHS",selmonth+1);
	updatersrc(mainrsrc,"FISCAL PROCESSING MONTHS");
	updateSCRNvirtuals(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x,cxmonth=0,fxmonth=0,cyear=0,fyear=0;

	if(InitializeSubsystems(argc,argv,module,"ADD BENEFIT VOUCHERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("BVAUDIT");
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if((bftmdac=APPOPNNRD(module,"BFTMDAC",TRUE,TRUE))==(-1)) return;
	if((bftddac=APPOPNNRD(module,"BFTDDAC",TRUE,TRUE))==(-1)) return;
	if((bftmstr=APPOPNNRD(module,"BFTMSTR",TRUE,TRUE))==(-1)) return;
	if((bftdesc=APPOPNNRD(module,"BFTDESC",TRUE,TRUE))==(-1)) return;
	if((payddsv=APPOPNNRD("PAYROLL","PAYDDSV",TRUE,TRUE))==(-1)) return;
	if((payddac=APPOPNNRD("PAYROLL","PAYDDAC",TRUE,TRUE))==(-1)) return;
	if((payddms=APPOPNNRD("PAYROLL","PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((paydtyp=APPOPNNRD("PAYROLL","PAYDTYP",TRUE,FALSE))==(-1)) return;
	if((venmst=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((venadr=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((paydedm=APPOPNNRD("PAYROLL","PAYDEDM",TRUE,FALSE))==(-1)) return;
	if((paydcsm=APPOPNNRD("PAYROLL","PAYDCSM",TRUE,FALSE))==(-1)) return;
	if((paydfsm=APPOPNNRD("PAYROLL","PAYDFSM",TRUE,FALSE))==(-1)) return;
	if((paypcsm=APPOPNNRD("PAYROLL","PAYPCSM",TRUE,FALSE))==(-1)) return;
	if((paypfsm=APPOPNNRD("PAYROLL","PAYPFSM",TRUE,FALSE))==(-1)) return;
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if((finbyr=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
	if((finbact=APPOPNNRD("FINMGT","FINBACT",TRUE,TRUE))==(-1)) return;
	if((finbacc=APPOPNNRD("FINMGT","FINBACC",TRUE,TRUE))==(-1)) return;
	if((yearnum=APPOPNNRD("FINMGT","FINYEAR",TRUE,TRUE))==(-1)) return;
	makeselecttype();
	mainrsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS");
	addDFincvir(mainrsrc,module,"BFTMDAC",NULL,bftmdac);
	addDFincvir(mainrsrc,module,"BFTDDAC",NULL,bftddac);
	addDFincvir(mainrsrc,module,"BFTMSTR",NULL,bftmstr);
	addDFincvir(mainrsrc,module,"BFTDESC",NULL,bftdesc);
	addDFincvir(mainrsrc,"PAYROLL","PAYDDMS",NULL,payddms);
	addDFincvir(mainrsrc,"PAYROLL","PAYDTYP",NULL,paydtyp);
	addDFincvir(mainrsrc,"PAYROLL","PAYDDAC",NULL,payddac);
	addDFincvir(mainrsrc,"PAYROLL","PAYDEDM",NULL,paydedm);
	addDFincvir(mainrsrc,"PAYROLL","PAYDCSM",NULL,paydcsm);
	addDFincvir(mainrsrc,"PAYROLL","PAYDFSM",NULL,paydfsm);
	addDFincvir(mainrsrc,"PAYROLL","PAYPCSM",NULL,paypcsm);
	addDFincvir(mainrsrc,"PAYROLL","PAYPFSM",NULL,paypfsm);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,venmst);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	file2rangersrc(bftddac,mainrsrc);
	file2rangersrc(bftmdac,mainrsrc);
	file2rangersrc(bftmstr,mainrsrc);
	file2rangersrc(bftdesc,mainrsrc);
	file2rangersrc(payddac,mainrsrc);
	file2rangersrc(payddms,mainrsrc);
	file2rangersrc(paydtyp,mainrsrc);
	file2rangersrc(venmst,mainrsrc);
	file2rangersrc(venadr,mainrsrc);
	file2rangersrc(perdmg,mainrsrc);
	file2rangersrc(paydedm,mainrsrc);
	file2rangersrc(paydcsm,mainrsrc);
	file2rangersrc(paydfsm,mainrsrc);
	file2rangersrc(paypcsm,mainrsrc);
	file2rangersrc(paypfsm,mainrsrc);
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,
		&selecttype,NULL);
	cxmonth=val(&CURRENT_DATE[0],2)-1;
	if(cxmonth>11) cxmonth=0;
	fxmonth=cxmonth+1;
	addlstrsrc(mainrsrc,"CALENDAR PROCESSING MONTHS",&cxmonth,TRUE,changemonth,12,&monthlist,NULL);
	addlstrsrc(mainrsrc,"FISCAL PROCESSING MONTHS",&fxmonth,TRUE,NULL,14,&fmonthlist,NULL);
/*
	addlstrsrc(mainrsrc,"MONTHS",&xmonth,TRUE,changemonth,12,&monthlist,NULL);
*/
	cyear=(val(&CURRENT_DATE10[6],4));
	if(cxmonth>fmgt->month) fyear=cyear+1;
		else fyear=cyear;
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&cyear,TRUE);
	addstdrsrc(mainrsrc,"FISCAL YEAR",LONGV,4,&fyear,TRUE);
	banklist=ADVMakeNRDlistAllSTRING(bnkacc,"BNKREC",
		"Creating Bank Identification List",1,"BANK IDENTIFICATION",
		SCRNvirtualSubData,mainrsrc);
	if(banklist->numlibs<1)
	{
		addAPPlib(banklist,"No Banks Available");
	}
	x=0;
	addlstrsrc(mainrsrc,"BANK IDENTIFICATIONS",&x,TRUE,
		NULL,banklist->numlibs,&banklist->libs,NULL);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,addvouchers,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void readscreen(RDArsrc *mtnrsrc,DBsort *t)
{
        int selected=0;
        short type=0;

	ZERNRD(t->fileno);
	readallwidgets(mtnrsrc);
	rsrc2singlefilerecord(t->fileno,mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(t->fileno,"DEFINITION TYPE",type);
}
static void resetdefs(RDArsrc *mtnrsrc,DBsort *t)
{
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	getfile(mtnrsrc,t);
}
static void setfiles(RDArsrc *mtnrsrc,DBsort *t)
{
	PassableStruct *p=NULL;

	readscreen(mtnrsrc,t);
	getsupportingfiles(mtnrsrc,t);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,BftAbvSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *desc=NULL;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	COPYFIELD(dbsort->fileno,bftmdac,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,bftmdac,"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,bftmdac,"DEFINITION TYPE");
	if(ADVEQLNRDsec(bftmdac,1,BftAbvSubData,p)) KEYNRD(bftmdac,1);
	filerecord2rsrc(bftmdac,mtnrsrc);

	COPYFIELD(dbsort->fileno,bftddac,"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,bftddac,"DEFINITION TYPE");
	if(ADVEQLNRDsec(bftddac,1,BftAbvSubData,p)) KEYNRD(bftddac,1);
	filerecord2rsrc(bftddac,mtnrsrc);

	COPYFIELD(dbsort->fileno,bftdesc,"DEDUCTION DESCRIPTION IDENTIFICATION");
	if(ADVEQLNRDsec(bftdesc,1,BftAbvSubData,p)) KEYNRD(bftdesc,1);
	filerecord2rsrc(bftdesc,mtnrsrc);

	COPYFIELD(dbsort->fileno,payddac,"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payddac,"DEFINITION TYPE");
	if(ADVEQLNRDsec(payddac,1,BftAbvSubData,p)) KEYNRD(payddac,1);
	filerecord2rsrc(payddac,mtnrsrc);

	COPYFIELD(dbsort->fileno,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
	if(ADVEQLNRDsec(payddms,1,BftAbvSubData,p)) KEYNRD(payddms,1);
	filerecord2rsrc(payddms,mtnrsrc);

	COPYFIELD(payddms,paydtyp,"DEDUCTION TYPE");
	if(ADVEQLNRDsec(paydtyp,1,BftAbvSubData,p)) KEYNRD(paydtyp,1);
	filerecord2rsrc(paydtyp,mtnrsrc);

	COPYFIELD(dbsort->fileno,bnkacc,"BANK IDENTIFICATION");
	if(ADVEQLNRDsec(bnkacc,1,BftAbvSubData,p)) KEYNRD(bnkacc,1);
	filerecord2rsrc(bnkacc,mtnrsrc);

	COPYFIELD(dbsort->fileno,venmst,"VENDOR IDENTIFICATION");
	if(ADVEQLNRDsec(venmst,1,BftAbvSubData,p)) KEYNRD(venmst,1);
	filerecord2rsrc(venmst,mtnrsrc);

	COPYFIELD(dbsort->fileno,venadr,"VENDOR IDENTIFICATION");
	COPYFIELD(dbsort->fileno,venadr,"ADDRESS IDENTIFICATION");
	if(ADVEQLNRDsec(venadr,1,BftAbvSubData,p)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);

	COPYFIELD(dbsort->fileno,perdmg,"PERSONNEL IDENTIFICATION");
	if(ADVEQLNRDsec(perdmg,1,BftAbvSubData,p)) KEYNRD(perdmg,1);
	filerecord2rsrc(perdmg,mtnrsrc);

	COPYFIELD(dbsort->fileno,paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,paydedm,"PERSONNEL IDENTIFICATION");
	if(ADVEQLNRDsec(paydedm,1,BftAbvSubData,p)) KEYNRD(paydedm,1);
	filerecord2rsrc(paydedm,mtnrsrc);

	COPYFIELD(dbsort->fileno,paydcsm,"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,paydcsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,paydcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(paydcsm,1,BftAbvSubData,p)) KEYNRD(paydcsm,1);
	filerecord2rsrc(paydcsm,mtnrsrc);

	COPYFIELD(dbsort->fileno,paydfsm,"DEDUCTION DESCRIPTION IDENTIFICATION");
	COPYFIELD(dbsort->fileno,paydfsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,paydfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(paydfsm,1,BftAbvSubData,p)) KEYNRD(paydfsm,1);
	filerecord2rsrc(paydfsm,mtnrsrc);

	COPYFIELD(dbsort->fileno,paypcsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,paypcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(paypcsm,1,BftAbvSubData,p)) KEYNRD(paypcsm,1);
	filerecord2rsrc(paypcsm,mtnrsrc);

	COPYFIELD(dbsort->fileno,paypfsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,paypfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(paypfsm,1,BftAbvSubData,p)) KEYNRD(paypfsm,1);
	filerecord2rsrc(paypfsm,mtnrsrc);

	COPYFIELD(dbsort->fileno,finbacc,"DEFINITION TYPE");
	FIELDCOPY(dbsort->fileno,"DEBIT CODE",finbacc,"ACCOUNT CODE");
	if(ADVEQLNRDsec(finbacc,1,BftAbvSubData,p)) KEYNRD(finbacc,1);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",desc);

	COPYFIELD(dbsort->fileno,finbacc,"DEFINITION TYPE");
	FIELDCOPY(dbsort->fileno,"CREDIT CODE",finbacc,"ACCOUNT CODE");
	if(ADVEQLNRDsec(finbacc,1,BftAbvSubData,p)) KEYNRD(finbacc,1);
	FINDFLDGETSTRING(finbacc,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",desc);
	if(desc!=NULL) Rfree(desc);
	if(p!=NULL) Rfree(p);
}
static void getprevious(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *perid=NULL,*dedid=NULL;
	int fyear=0,trans=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETSTRING(mtnrsrc,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&trans);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	if(perid!=NULL) Rfree(perid);
	FINDFLDSETSTRING(dbsort->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(dedid!=NULL) Rfree(dedid);
	FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",fyear);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",trans);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		if(LTENRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
			setscrlist(mtnrsrc,dbsort->fileno);
		}
	} else {
		PRVNRD(dbsort->fileno,1);
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
		setscrlist(mtnrsrc,dbsort->fileno);
	}
	getsupportingfiles(mtnrsrc,dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,BftAbvSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getnext(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *perid=NULL,*dedid=NULL;
	int fyear=0,trans=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETSTRING(mtnrsrc,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&trans);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	if(perid!=NULL) Rfree(perid);
	FINDFLDSETSTRING(dbsort->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(dedid!=NULL) Rfree(dedid);
	FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",fyear);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",trans);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		if(GTENRD(dbsort->fileno,1))
		{
			KEYNRD(dbsort->fileno,1);
		} else {
			singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
			setscrlist(mtnrsrc,dbsort->fileno);
		}
	} else {
		NXTNRD(dbsort->fileno,1);
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
		setscrlist(mtnrsrc,dbsort->fileno);
	}
	getsupportingfiles(mtnrsrc,dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,BftAbvSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *dedid=NULL;
	int fyear=0,trans=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&trans);
	FINDFLDSETSTRING(dbsort->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(dedid!=NULL) Rfree(dedid);
	FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",fyear);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",trans);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
		setscrlist(mtnrsrc,dbsort->fileno);
	}
	getsupportingfiles(mtnrsrc,dbsort);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,BftAbvSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
	updateallrsrc(mtnrsrc);
}
static void getvendor(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *vendorid=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"VENDOR IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&vendorid);
	FINDFLDSETSTRING(venmst,"VENDOR IDENTIFICATION",vendorid);
	if(vendorid!=NULL) Rfree(vendorid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(venmst,1,BftAbvSubData,p)) KEYNRD(venmst,1);
	if(p!=NULL) Rfree(p);
	updatefilerecord2rsrc(venmst,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void getperid(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *id=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"PERSONNEL IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&id);
	FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",id);
	if(id!=NULL) Rfree(id);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(perdmg,1,BftAbvSubData,p)) KEYNRD(perdmg,1);
	if(p!=NULL) Rfree(p);
	updatefilerecord2rsrc(perdmg,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void getbank(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *bankid=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"BANK IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"BANK IDENTIFICATION",&bankid);
	FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankid);
	if(bankid!=NULL) Rfree(bankid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(bnkacc,1,BftAbvSubData,p)) KEYNRD(bnkacc,1);
	if(p!=NULL) Rfree(p);
	updatefilerecord2rsrc(bnkacc,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void getaddr(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *vendorid=NULL,*addrid=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"VENDOR IDENTIFICATION");
	readwidget(mtnrsrc,"ADDRESS IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&vendorid);
	FINDRSCGETSTRING(mtnrsrc,"ADDRESS IDENTIFICATION",&addrid);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",addrid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(venadr,1,BftAbvSubData,p)) KEYNRD(venadr,1);
	if(p!=NULL) Rfree(p);
	filerecord2rsrc(venadr,mtnrsrc);
}
static void DEFtypecb(RDArsrc *mtnrsrc)
{
	int x=0;
	short deftype=0,deftype1;

	FINDRSCGETDEFTYPE(mtnrsrc,"DEBIT CODE",&deftype1);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	deftype=(short)x;
	if(deftype!=deftype1)
	{
		FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",deftype);
		FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",deftype);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		updatersrc(mtnrsrc,"DEBIT CODE");
		updatersrc(mtnrsrc,"CREDIT CODE");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		updateSCRNvirtuals(mtnrsrc);
	}
}
static void setscrlist(RDArsrc *mtnrsrc,short fileno)
{
	int x=0;
	short d=0;

	FINDFLDGETSHORT(fileno,"DEFINITION TYPE",&d);
	x=d;
	if(DEFtypes->numlibs<=x)
	{
		x=0;
		d=0;
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",d);
}
void selectm(short dowhich,void *targetkey,int argc,char **argv,DBsort *t)
{
	int x=0,y=0,which_type=0;
	RDArsrc *mtnrsrc=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	short nofields=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	RDAacct *acct=NULL;
	PassableStruct *p=NULL;

	mtnrsrc=RDArsrcNEW(module,"ADD BENEFIT VOUCHERS MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"BFTDESC",NULL,bftdesc);
	addDFincvir(mtnrsrc,module,"BFTMDAC",NULL,bftmdac);
	addDFincvir(mtnrsrc,module,"BFTDDAC",NULL,bftddac);
	addDFincvir(mtnrsrc,"PAYROLL","PAYDDMS",NULL,payddms);
	addDFincvir(mtnrsrc,"PAYROLL","PAYDTYP",NULL,paydtyp);
	addDFincvir(mtnrsrc,"PAYROLL","PAYDDAC",NULL,payddac);
	addDFincvir(mtnrsrc,"PAYROLL","PAYDEDM",NULL,paydedm);
	addDFincvir(mtnrsrc,"PAYROLL","PAYDCSM",NULL,paydcsm);
	addDFincvir(mtnrsrc,"PAYROLL","PAYDFSM",NULL,paydfsm);
	addDFincvir(mtnrsrc,"PAYROLL","PAYPCSM",NULL,paypcsm);
	addDFincvir(mtnrsrc,"PAYROLL","PAYPFSM",NULL,paypfsm);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,venmst);
	addDFincvir(mtnrsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mtnrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(mtnrsrc,"VENPMT","VENADR",NULL,venadr);
	addDFincvir(mtnrsrc,"FINMGT","FINBACC",NULL,finbacc);
	addDFincvir(mtnrsrc,"FINMGT","FINBYR",NULL,finbyr);
	addDFincvir(mtnrsrc,"FINMGT","FINYEAR",NULL,yearnum);
	ZERNRD(t->fileno);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=TRUE;
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(t->fileno,fldx->name,&edit_rsrc))
				{
					DEFtypes=APPlibNEW();
					if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
					{
						for(y=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
							y<BALANCE_SHEET_ACCOUNT->num;++y,++acct)
						{
							addAPPlib(DEFtypes,acct->name);
						}
					}
					if(DEFtypes->numlibs<1)
					{
						addAPPlib(DEFtypes,"No Definition Types");
					}
					which_type=0;
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,edit_rsrc,
						DEFtypecb,DEFtypes->numlibs,
						&DEFtypes->libs,t);
				}
			} else {
				ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,TRUE,TRUE);
				FINDRSCSETFUNC(mtnrsrc,fldx->name,setfiles,t);
			}
		}
	}
	if(bftddac!=(-1)) file2rsrc(bftddac,mtnrsrc,FALSE);
	if(bftmstr!=(-1)) file2rsrc(bftmstr,mtnrsrc,FALSE);
	if(bftdesc!=(-1)) file2rsrc(bftdesc,mtnrsrc,FALSE);
	if(payddms!=(-1)) file2rsrc(payddms,mtnrsrc,FALSE);
	if(paydtyp!=(-1)) file2rsrc(paydtyp,mtnrsrc,FALSE);
	if(venmst!=(-1)) file2rsrc(venmst,mtnrsrc,FALSE);
	if(venadr!=(-1)) file2rsrc(venadr,mtnrsrc,FALSE);
	if(perdmg!=(-1)) file2rsrc(perdmg,mtnrsrc,FALSE);
	if(paydedm!=(-1)) file2rsrc(paydedm,mtnrsrc,FALSE);
	if(paydcsm!=(-1)) file2rsrc(paydcsm,mtnrsrc,FALSE);
	if(paydfsm!=(-1)) file2rsrc(paydfsm,mtnrsrc,FALSE);
	if(paypcsm!=(-1)) file2rsrc(paypcsm,mtnrsrc,FALSE);
	if(paypfsm!=(-1)) file2rsrc(paypfsm,mtnrsrc,FALSE);
	if(bnkacc!=(-1)) file2rsrc(bnkacc,mtnrsrc,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"VENDOR IDENTIFICATION",getvendor,t);
	FINDRSCSETFUNC(mtnrsrc,"ADDRESS IDENTIFICATION",getaddr,t);
	FINDRSCSETFUNC(mtnrsrc,"PERSONNEL IDENTIFICATION",getperid,t);
	FINDRSCSETFUNC(mtnrsrc,"BANK IDENTIFICATION",getbank,t);
	FINDRSCSETFUNC(mtnrsrc,"DEBIT CODE",debcodecb,t);
	FINDRSCSETFUNC(mtnrsrc,"CREDIT CODE",credcodecb,t);
	key=KEYNUM(t->fileno,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			FINDRSCSETFUNC(mtnrsrc,part->name,getfile,t);
		}
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	ADVSCRNvirtual2rsrc(mtnrsrc,BftAbvSubData,p);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* this option isn't used */
			break;
		case 2:
			ReadRDAScrolledLists(mtnrsrc);
			if(EQLNRD(t->fileno,1))
			{
				rsrc2singlefilerecord(t->fileno,mtnrsrc);
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				setscrlist(mtnrsrc,t->fileno);
			}
			break;
		default:
		case 0:
			if(EQLNRDKEY(t->fileno,1,targetkey))
			{
				ZERNRD(dbsort->fileno);
				ReadRDAScrolledLists(mtnrsrc);
				if(EQLNRD(t->fileno,1))
				{
					rsrc2singlefilerecord(t->fileno,mtnrsrc);
				} else {
					singlefilerecord2rsrc(t->fileno,mtnrsrc);
					setscrlist(mtnrsrc,t->fileno);
				}
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				setscrlist(mtnrsrc,t->fileno);
			}
			break;
	}
	getsupportingfiles(mtnrsrc,t);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"PREVIOUS",TRUE,getprevious,t);
	addbtnrsrc(mtnrsrc,"NEXT",TRUE,getnext,t);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,t);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,BftAbvSubData,p,FALSE);
	if(p!=NULL) Rfree(p);
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
static void clearwidget(RDArsrc *mtnrsrc,char *widgetname)
{
        FINDRSCSETSTRING(mtnrsrc,widgetname,NULL);
        updatersrc(mtnrsrc,widgetname);
}
static short check_code(RDArsrc *mtnrsrc,DBsort *dbsort,short acctwidget)
{
	short deftype1=0;
	int seldeftype=0,procyrid=0;
	char *debcode1=NULL,*credcode1=NULL,*tmp=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"DEFINITION TYPES");
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype1=(short)seldeftype;
	readwidget(mtnrsrc,"FISCAL YEAR");
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&procyrid);
	readwidget(mtnrsrc,"DEBIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"DEBIT CODE",&debcode1);
	readwidget(mtnrsrc,"CREDIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"CREDIT CODE",&credcode1);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	switch(acctwidget)
	{
		case 1:
			if(debcode1!=NULL && RDAstrlen(debcode1)>0)
			{
				if((tmp=ADVCHECKACCT(finbyr,finbacc,debcode1,BAL_ACCT,deftype1,procyrid,BftAbvSubData,p))!=NULL)
				{
					ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"DEBIT CODE",NULL);
					Rfree(tmp);
					if(credcode1!=NULL) Rfree(credcode1);
					if(debcode1!=NULL) Rfree(debcode1);
					FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
					return(FALSE);
				} else {
					FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
					FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
					Rfree(tmp);
				}
			}
			break;
		case 2:
			if(credcode1!=NULL && RDAstrlen(credcode1)>0)
			{
				if((tmp=ADVCHECKACCT(finbyr,finbacc,credcode1,BAL_ACCT,deftype1,procyrid,BftAbvSubData,p))!=NULL)
				{
					ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,clearwidget,NULL,TRUE,2,mtnrsrc,"CREDIT CODE",NULL);
					Rfree(tmp);
					if(credcode1!=NULL) Rfree(credcode1);
					if(debcode1!=NULL) Rfree(debcode1);
					FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
					return(FALSE);
				} else {
					FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
					FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
					Rfree(tmp);
				}
			}
			break;
		default:
			prterr("\"acctwidget\" value of [%d] out of range.",acctwidget);
			break;
	}
	if(p!=NULL) Rfree(p);
	if(credcode1!=NULL) Rfree(credcode1);
	if(debcode1!=NULL) Rfree(debcode1); 
	return(TRUE);
}
static void debcodecb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	check_debcode(mtnrsrc,dbsort);
	updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_debcode(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	return(check_code(mtnrsrc,dbsort,1));
}
static void credcodecb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	check_credcode(mtnrsrc,dbsort);
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_credcode(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	return(check_code(mtnrsrc,dbsort,2));
}
static void save_check(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int seldeftype=0;
	short deftype=0;
	double amount=0.0;
	char delflag=FALSE;
	char *benefitid1=NULL,*warnmessage=NULL;
	int procyrid=0;
	char *tmp=NULL,active=FALSE;
	PassableStruct *p=NULL;

	readscreen(mtnrsrc,dbsort);
	FINDRSCGETCHAR(mtnrsrc,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		FINDRSCGETDOUBLE(mtnrsrc,"AMOUNT",&amount);
		if(amount==0.0)
		{
                	WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TRANSACTION AMOUNT!","The Transaction Amount of [$  0.00] is an invalid amount for a Transaction Record. There must be a proper amount before this record can be saved.",NULL,NULL,FALSE,NULL);
			return;
		}
        	FINDRSCGETSTRING(mtnrsrc,"DEDUCTION DESCRIPTION IDENTIFICATION",&benefitid1);
        	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
		deftype=(short)seldeftype;
        	if(!RDAstrcmp(DEFtypes->libs[seldeftype],"No Definition Types"))
        	{
			warnmessage=Rmalloc(250);
			sprintf(warnmessage,"There is not a valid DEFINITION TYPE available for selection.  Before this Transaction can be saved there must be a valid DEFINITION TYPE available for selection.  Check to see if the  BALANCE SHEET ACCOUNT CODE DEFINITION has at least one DEFINITION TYPE available.");
                	WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",warnmessage,NULL,NULL,FALSE,NULL);
			prterr(warnmessage);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(benefitid1!=NULL) Rfree(benefitid1);
                	return;
		}
		p=Rmalloc(sizeof(PassableStruct));
		p->r=mtnrsrc;
		p->a=dbsort;
		if(payddms!=(-1))
		{
			FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
			if(ADVEQLNRDsec(payddms,1,BftAbvSubData,p))
			{
				KEYNRD(payddms,1);
				warnmessage=Rmalloc(RDAstrlen(benefitid1)+310);
				sprintf(warnmessage,"There is not a valid DEDUCTION DESCRIPTION record for the Deduction Description Identification of [%s] in the data file [PAYROLL][PAYDDMS].  Before this Transaction can be saved there must be a valid DEDUCTION DESCRIPTION IDENTIFICATION record.",(benefitid1==NULL?"":benefitid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","NO DEDUCTION DESCRIPTION RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr(warnmessage);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(benefitid1!=NULL) Rfree(benefitid1);
				return;
			} else { 
				FINDFLDGETCHAR(payddms,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(benefitid1)+290);
					sprintf(warnmessage,"The DEDUCTION DESCRIPTION record for the Deduction Description Identification of [%s] is in a Deleted State in the data file [PAYROLL][PAYDDMS].  Before this Transaction can be saved there must be a valid DEDUCTION DESCRIPTION record.",(benefitid1==NULL?"":benefitid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED DEDUCTION DESCRIPTION RECORD!",
					warnmessage,NULL,NULL,TRUE,NULL);
					prterr(warnmessage);
					if(warnmessage!=NULL) Rfree(warnmessage);
					if(benefitid1!=NULL) Rfree(benefitid1);
					return;
				}
			}
		}
		if(bftdesc!=(-1))
		{
			FINDFLDSETSTRING(bftdesc,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
			if(ADVEQLNRDsec(bftdesc,1,BftAbvSubData,p))
			{
				KEYNRD(bftdesc,1);
				warnmessage=Rmalloc(RDAstrlen(benefitid1)+250);
				sprintf(warnmessage,"There is not a valid BENEFIT DESCRIPTION record for the Deduction Description Identification of [%s].  Before this Transaction can be saved there must be a valid BENEFIT DESCRIPTION record.",(benefitid1==NULL?"":benefitid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","NO BENEFIT DESCRIPTION IDENTIFICATION RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr(warnmessage);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(benefitid1!=NULL) Rfree(benefitid1);
				return;
			} else { 
				FINDFLDGETCHAR(bftdesc,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(benefitid1)+230);
					sprintf(warnmessage,"The BENEFIT DESCRIPTION record for the Deduction Description Identification of [%s] is in a Deleted State.  Before this Transaction can be saved there must be a valid BENEFIT DESCRIPTION record.",(benefitid1==NULL?"":benefitid1))
;
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BENEFIT MASTER RECORD!",
					warnmessage,NULL,NULL,TRUE,NULL);
					prterr(warnmessage);
					if(warnmessage!=NULL) Rfree(warnmessage);
					if(benefitid1!=NULL) Rfree(benefitid1);
					return;
				}
			}
		}
		if(payddac!=(-1))
		{
			FINDFLDSETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
			FINDFLDSETSHORT(payddac,"DEFINITION TYPE",deftype);
			if(ADVEQLNRDsec(payddac,1,BftAbvSubData,p))
			{
				KEYNRD(payddac,1);
				warnmessage=Rmalloc(RDAstrlen(benefitid1)+340);
				sprintf(warnmessage,"There is not a valid BENEFIT DEDUCTION DEFINITION MASTER record for the Deduction Description Identification of [%s] and Definition Type of [%2d].  Before this Transaction can be saved there must be a valid record in the data file [BFTMGT][BFTDDAC].",(benefitid1==NULL?"":benefitid1),deftype);
				WARNINGDIALOG("WARNING DIALOG SCREEN","NO BENEFIT DEDUCTION DEFINITION TYPE RECORD!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr(warnmessage);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(benefitid1!=NULL) Rfree(benefitid1);
				return;
			} else { 
				FINDFLDGETCHAR(payddac,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(benefitid1)+340);
					sprintf(warnmessage,"The BENEFIT PERSONNEL DEDUCTION DEFINITION MASTER record is in a deleted state for the Deduction Description Identification of [%s], and Definition Type of [%2d].  Before this Transaction can be saved there must be a valid record in the data file [BFTMGT][BFTMDAC].",(benefitid1==NULL?"":benefitid1),deftype);
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BENEFIT DEDUCTION DEFINITION TYPE RECORD!",
					warnmessage,NULL,NULL,TRUE,NULL);
					prterr(warnmessage);
					if(warnmessage!=NULL) Rfree(warnmessage);
					if(benefitid1!=NULL) Rfree(benefitid1);
					return;
				}
			}
		}
		if(benefitid1!=NULL) Rfree(benefitid1);
		if(!FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&procyrid))
		{
			FINDFLDSETINT(yearnum,"FISCAL YEAR",procyrid);
			KEYNRD(yearnum,1);
			if(!ADVEQLNRDsec(yearnum,1,BftAbvSubData,p))
			{
				FINDFLDGETCHAR(yearnum,"ACTIVE",&active);
				if(active==FALSE)
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","NON-ACTIVE FISCAL YEAR!","The FISCAL YEAR entered is not active.\nIt exist as a FINANCIAL YEAR but is presently set up as inactive.\n",NULL,NULL,TRUE,NULL);
					prterr("The FISCAL YEAR entered is not active.  It exist as a FINANCIAL YEAR but is presently set up as inactive.");
					return;
				}
				FINDFLDGETCHAR(yearnum,"DELETEFLAG",&delflag);
				if(delflag==TRUE)
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED FISCAL YEAR!","The FISCAL YEAR entered is deleted.\nIt exist as a FINANCIAL YEAR but is presently set up in a deletion state.\n",NULL,NULL,TRUE,NULL);
					prterr("The FISCAL YEAR entered is deleted. It exist as a FINANCIAL YEAR but is presently set up in a deletion state.");
					return;
				}
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!","The FISCAL YEAR entered is not valid.\nIt does not exist as a FINANCIAL YEAR.\n",NULL,NULL,TRUE,NULL);
				return;
			}
		}
		if(!FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&tmp))
		{
			FINDFLDSETSTRING(venmst,"VENDOR IDENTIFICATION",tmp);
			KEYNRD(venmst,1);
			if(!ADVEQLNRDsec(venmst,1,BftAbvSubData,p))
			{
				FINDFLDGETCHAR(venmst,"DELETEFLAG",&delflag);
				if(delflag==TRUE)
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!","The VENDOR IDENTIFICATION entered is deleted.\nIt exist as a VENDOR but is presently set up in a deletion state.\n",NULL,NULL,TRUE,NULL);
					prterr("The VENDOR IDENTIFICATION entered is deleted.  It exist as a VENDOR but is presently set up in a deletion state.");
					if(tmp!=NULL) Rfree(tmp);
					return;
				}
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!","The VENDOR IDENTIFICATION entered is not valid.\nIt does not exist as a VENDOR.\n",NULL,NULL,TRUE,NULL);
				if(tmp!=NULL) Rfree(tmp);
				return;
			}
		}
		if(!FINDRSCGETSTRING(mtnrsrc,"BANK IDENTIFICATION",&tmp))
		{
			FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",tmp);
			KEYNRD(bnkacc,1);
			if(!ADVEQLNRDsec(bnkacc,1,BftAbvSubData,p))
			{
				FINDFLDGETCHAR(bnkacc,"DELETEFLAG",&delflag);
				if(delflag)
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BANK IDENTIFICATION!","The BANK IDENTIFICATION entered is flagged to be delected in the supporting BNKACC file.  Therefore, this record will not be saved\n",NULL,NULL,TRUE,NULL);
					prterr("The BANK IDENTIFICATION entered is flagged to be deleted in the supporting BNKACC file.");
					if(tmp!=NULL) Rfree(tmp);
					return;
				}
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BANK IDENTIFICATION!","The BANK IDENTIFICATION entered is not valid.",NULL,NULL,TRUE,NULL);
				if(tmp!=NULL) Rfree(tmp);
				return;
			}
		}
		if(p!=NULL) Rfree(p);	
		if(tmp!=NULL) Rfree(tmp);
		if(check_debcode(mtnrsrc,dbsort)==TRUE)
		{
			if(check_credcode(mtnrsrc,dbsort)==FALSE) return; 
		} else return;
	}
	save_record(mtnrsrc,update_list,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN","SECURITY","Browse Diagnostic",NULL);
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
	if(WindowCount<2) select_backend(dbsort);
}
