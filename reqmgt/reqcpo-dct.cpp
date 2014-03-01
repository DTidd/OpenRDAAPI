/* reqcpo.c - Create Purchase Order */
#ifndef WIN32
#define __NAM__ "reqcpo-dct.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqcpo-dct.exe"
#endif
#include <app.hpp>

#include <ldval.hpp>
#include <reqmgt.hpp>
#include <purord.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
/*ADDLIB misc */
/*CPP_OPART reqfunc */


static char *module="REQMGT",**selecttype=NULL,*reportname=NULL;
static char *select_expression=NULL;
static short reqitm=(-1),reqmst=(-1),catmst=(-1),reqloc=(-1),reqacc=(-1);
static short vennum=(-1),venadrnum=(-1),venadr2num=(-1),fsclyr=(-1);
static short purmst=(-1),pursit=(-1),purlin=(-1),purtrn=(-1),puracc=(-1);
static short fixmst=(-1),purids=(-1);
static int select_type=0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_purlin=NULL;
static void save_record(RDArsrc *,short,DBsort *);
static void save_record_up(RDArsrc *,DBsort *);
static void save_record_nup(RDArsrc *,DBsort *);
static void quit_record(RDArsrc *,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *);
static void getsupportingpurmstfiles(RDArsrc *);
static void settype(RDArsrc *,DBsort *);
static void gettypes(RDArsrc *);
static void setpoidcb(RDArsrc *);
static void setpoid(RDArsrc *,short);
static void test_select(RDArsrc *);
static APPlib *type=NULL;
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
RDAfinmgt *fmgt=NULL;
static APPlib *sitesavl=NULL;
static APPlib *ordadravl=NULL;
static APPlib *billadravl=NULL;
static void selectm(void *,DBsort *);
static int POLINE_COUNT=0;
static RDATData *prev_purmst=NULL;
static void checkvendorcb(RDArsrc *);
static short checkvendor(RDArsrc *);
static void addpo(RDArsrc *);
static void savepurmst(RDArsrc *);
static void savepurmstrsp(RDArsrc *);
static short purmst_test(RDArsrc *);
static void savecheck_purmst(RDArsrc *);
static void loadaddresslist(RDArsrc *);
static void setaddresses(RDArsrc *);
static void setsiteid(RDArsrc *);
static MakeBrowseList *mbl_venmsr=NULL;
static void doexit_venmsr(MakeBrowseList *),okfunc_venmsr(MakeBrowseList *,void *);
static short selectfunction_venmsr(MakeBrowseList *);
static void browse_venmsr(RDArsrc *);
static int FiscalYear=0;
static char *POID=NULL,*VENDORID=NULL,*ADDRID=NULL;

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static void ReqCPOSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y;
	RDAvirtual *v=NULL;
	DBsort *dbsort=NULL;
	RDArsrc *r=NULL;

	r=p->r;
	dbsort=p->a;
	if(diageval)
	{
		prterr("DIAG ReqCPOSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
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
						temp=EVALUATEstr(v->expression,ReqCPOSubData,p);
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
						sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=EVALUATEsht(v->expression,ReqCPOSubData,p);
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
						*v->value.integer_value=EVALUATEint(v->expression,ReqCPOSubData,p);
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
						*v->value.float_value=EVALUATEdbl(v->expression,ReqCPOSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,ReqCPOSubData,p);
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
						*v->value.string_value=EVALUATEbol(v->expression,ReqCPOSubData,p);
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
		prterr("DIAG ReqCPOSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
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
	addDFincvir(tmprsrc,module,"REQITM",NULL,dumb);
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
	short ef=FALSE,update_diag=FALSE;
	char deleteflag=FALSE,*curdate=NULL,*curtime=NULL;
	char delflag=FALSE,*locid=NULL,*reqid=NULL;
	int itemno=0;
	NRDfield *field=NULL,*FIELDS=NULL;
	int x=0,numfields=0,qty=0,fixqty=0,lineno=0,type=0;
	double total=0.0,outcost=0.0,outamt=0.0,cost=0.0,itemrate=0.0,itemamt=0.0,percent=0.0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;
	REQACC *REQACCvalue=NULL;
	ReqAcctStruct *a=NULL;
	PassableStruct *p=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Creating Purchase Orders",NULL);
		addDFincvir(tmprsrc,module,"REQITM",NULL,reqitm);
		addDFincvir(tmprsrc,module,"REQACC",NULL,reqacc);
		addDFincvir(tmprsrc,"PURORD","PURMST",NULL,purmst);
		addDFincvir(tmprsrc,"PURORD","PURLIN",NULL,purlin);
		addDFincvir(tmprsrc,"PURORD","PURACC",NULL,puracc);
		addDFincvir(tmprsrc,"PURORD","PURTRN",NULL,purtrn);
		addDFincvir(tmprsrc,"FIXASS","FIXMST",NULL,fixmst);
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
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				curdate=GETCURRENTDATE10();
				curtime=GETCURRENTTIME();
				LOCNRDFILE(purlin);
				numfields=NUMFLDS(purlin);
				FIELDS=FLDPOINTER(purlin);
				if(FIELDS!=NULL)
				{
					for(x=0,field=FIELDS;x<numfields;++x,++field)
					{
						if(!RDAstrcmp(field->name,"DELETEFLAG"))
						{
						} else {
							COPYFIELD(dbsort->fileno,purlin,field->name);
						}
					}
				}
				FINDFLDSETCHAR(purlin,"DELETEFLAG",FALSE);
				FINDFLDSETINT(purlin,"RECEIVED QUANTITY",0);
				FINDFLDSETCHAR(purlin,"REQUISITIONED ITEM",TRUE);
				FINDFLDSETSTRING(purlin,"SOURCE USER",USERLOGIN);
				FINDFLDSETSTRING(purlin,"ENTRY DATE",curdate);
				FINDFLDSETSTRING(purlin,"ENTRY TIME",curtime);
				FINDFLDSETSTRING(purlin,"REFERENCE IDENTIFICATION",NULL);
				FINDFLDGETINT(dbsort->fileno,"ITEM NUMBER",&itemno);
				FINDFLDSETINT(purlin,"REQUISITION ITEM NUMBER",itemno);
				/* WRITE TO PURLIN */
				FINDFLDGETINT(reqitm,"QUANTITY",&qty);
				FINDFLDGETDOUBLE(reqitm,"AMOUNT",&itemamt);
				FINDFLDGETDOUBLE(reqitm,"PERCENTAGE DISCOUNT",&percent);
				FINDFLDGETDOUBLE(reqitm,"UNIT RATE",&itemrate);
				FINDFLDSETINT(purlin,"QUANTITY",qty);
				FINDFLDSETINT(purlin,"PERCENTAGE DISCOUNT",percent);
				FINDFLDSETDOUBLE(purlin,"OUTSTANDING LINE AMOUNT",itemamt);
				if(percent == 0.0) 
				{
				cost = qty * itemrate * 100;
				} else {
				cost = qty * itemrate *100 - (qty * itemrate * 100 * percent /100);
				}
				cost = itemamt;
				FINDFLDSETDOUBLE(purlin,"COST",itemamt);
				FINDFLDSETDOUBLE(purlin,"OUTSTANDING LINE COST",itemamt);
				FINDFLDSETDOUBLE(purlin,"TOTAL",itemamt);
				if(!ADVWRTTRANSsec(purlin,1,"LINE NUMBER",NULL,ReqCPOSubData,p))
				{
					++POLINE_COUNT;
					update_diag=TRUE;
					UNLNRDFILE(purlin);
					LOCNRDFILE(purmst);
					FINDFLDSETINT(purmst,"FISCAL YEAR",FiscalYear);
					COPYFIELD(dbsort->fileno,purmst,"PO IDENTIFICATION");
					FINDFLDGETDOUBLE(dbsort->fileno,"TOTAL",&total);
					if(ADVEQLNRDsec(purmst,1,ReqCPOSubData,p))
					{
						KEYNRD(purmst,1);
						FINDFLDSETDOUBLE(purmst,"OUTSTANDING AMOUNT",total);
						FINDFLDSETDOUBLE(purmst,"TOTAL LINE AMOUNT",total);
						FINDFLDSETDOUBLE(purmst,"OUTSTANDING COST",total);
						FINDFLDSETDOUBLE(purmst,"TOTAL LINE COST",total);
					} else {
						prev=RDATDataNEW(purmst);
						FINDFLDGETDOUBLE(purmst,"OUTSTANDING AMOUNT",&total);
						outamt+=total;
						outcost+=cost;
						FINDFLDSETDOUBLE(purmst,"OUTSTANDING AMOUNT",outamt);
						FINDFLDSETDOUBLE(purmst,"TOTAL LINE AMOUNT",outamt);
						FINDFLDSETDOUBLE(purmst,"OUTSTANDING COST",outamt);
						FINDFLDSETDOUBLE(purmst,"TOTAL LINE COST",outamt);
					}
					ADVWRTTRANSsec(purmst,0,NULL,prev,ReqCPOSubData,p);
					UNLNRDFILE(purmst);
					if(prev!=NULL) Rfree(prev);

					FINDFLDGETINT(purlin,"LINE NUMBER",&lineno);
					FINDFLDGETSTRING(dbsort->fileno,"LOCATION IDENTIFICATION",&locid);
					FINDFLDGETSTRING(dbsort->fileno,"REQUISITION IDENTIFICATION",&reqid);
					LOCNRDFILE(puracc);
					LOCNRDFILE(reqacc);
					REQACCvalue=NULL;
					REQACCvalue=GetREQACC(reqacc,locid,reqid,itemno);
					if(REQACCvalue!=NULL)
					{
						ComputeREQACC(REQACCvalue,total);
						for(x=0,a=REQACCvalue->a;
							x<REQACCvalue->num;++x,++a)
						{
							if(a->enc>0)
							{
								FINDFLDSETSTRING(reqacc,"LOCATION IDENTIFICATION",locid);
								FINDFLDSETSTRING(reqacc,"REQUISITION IDENTIFICATION",reqid);
								FINDFLDSETINT(reqacc,"ITEM NUMBER",itemno);
								FINDFLDSETINT(reqacc,"ACCOUNT NUMBER",a->accno);
								if(ADVEQLNRDsec(reqacc,1,ReqCPOSubData,p)) KEYNRD(reqacc,1);
								FINDFLDSETINT(puracc,"FISCAL YEAR",FiscalYear);
								COPYFIELD(dbsort->fileno,puracc,"PO IDENTIFICATION");
								FINDFLDSETINT(puracc,"LINE NUMBER",lineno);
								FINDFLDSETSHORT(puracc,"ACCOUNT TYPE",0);
								FINDFLDSETSHORT(puracc,"DEFINITION TYPE",a->deftype);
								FINDFLDSETSTRING(puracc,"ACCOUNT CODE",a->account);
								FINDFLDSETSTRING(puracc,"DEBIT CODE",a->debit);
								FINDFLDSETSTRING(puracc,"CREDIT CODE",a->credit);
								FINDFLDSETSTRING(puracc,"DEFAULT DEBIT CODE",a->defdebit);
								FINDFLDSETSTRING(puracc,"DEFAULT CREDIT CODE",a->defcredit);
								FINDFLDSETSTRING(puracc,"DEFAULT PAYABLE CODE",a->defpayable);
								FINDFLDSETSTRING(puracc,"DEFAULT BANK IDENTIFICATION",a->defbankid);

								FINDFLDSETSTRING(puracc,"DEFAULT CLEARING CASH",a->defccash);
								FINDFLDSETSTRING(puracc,"DEFAULT CLEARING DUE FROM",a->defcdfrom);
								FINDFLDSETSHORT(puracc,"DEFAULT CLEARING DEFINITION TYPE",a->cdeftype);
/*
								COPYFIELD(reqacc,puracc,"DEFAULT DEBIT CODE");
								COPYFIELD(reqacc,puracc,"DEFAULT CREDIT CODE");
								COPYFIELD(reqacc,puracc,"DEFAULT PAYABLE CODE");
								COPYFIELD(reqacc,puracc,"DEFAULT BANK IDENTIFICATION");
*/
								FINDFLDSETCHAR(puracc,"DELETEFLAG",FALSE);
								/* WRITE TO PURACC */
								FINDFLDSETCHAR(puracc,"ACCOUNT VERIFIED",TRUE);
								FINDFLDSETSTRING(puracc,"ACCOUNT VERIFIED BY",USERLOGIN);
								FINDFLDSETSTRING(puracc,"ACCOUNT VERIFIED DATE",curdate);
								FINDFLDSETSTRING(puracc,"ACCOUNT VERIFIED TIME",curtime);

								FINDFLDSETDOUBLE(puracc,"AMOUNT",outamt);
								FINDFLDSETDOUBLE(puracc,"OUTSTANDING ACCOUNT AMOUNT",outamt);
								FINDFLDSETDOUBLE(puracc,"COST",outamt);
								FINDFLDSETDOUBLE(puracc,"OUTSTANDING ACCOUNT COST",outamt);


								ADVWRTTRANSsec(puracc,1,"ACCOUNT NUMBER",NULL,ReqCPOSubData,p);
								COPYFIELD(puracc,purtrn,"FISCAL YEAR");
								COPYFIELD(puracc,purtrn,"PO IDENTIFICATION");
								COPYFIELD(puracc,purtrn,"LINE NUMBER");
								COPYFIELD(puracc,purtrn,"ACCOUNT NUMBER");
								COPYFIELD(dbsort->fileno,purtrn,"PROCESSING MONTH");
								FINDFLDSETDOUBLE(purtrn,"AMOUNT",a->enc);
								FINDFLDSETSTRING(purtrn,"SOURCE USER",USERLOGIN);
								FINDFLDSETSTRING(purtrn,"ENTRY DATE",curdate);
								FINDFLDSETSTRING(purtrn,"ENTRY TIME",curtime);
								FINDFLDSETCHAR(purtrn,"DISTRIBUTED",FALSE);
								FINDFLDSETCHAR(purtrn,"SOURCE USER VERIFIED",FALSE);
								ADVWRTTRANSsec(purtrn,1,"TRANSACTION NUMBER",NULL,ReqCPOSubData,p);
							} else {
								if(diagapps)
								{
									prterr("DIAG Application: No Purchase Order Accounts or Transactions written for the Requisition Account Record of Location Identification [%s], Requisition Identification of [%s], Item Number of [%d], and Account Number of [%d] because the Encumbrance Amount is not greater than 0.00",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno,a->accno);SeeReqAcctStruct(a);
								}
							}
						}
					} else {
						if(diagapps)
						{
							prterr("DIAG Application: No Requsition Account Structure could be gotten for the Requisition of Location Identification [%s], Requisition Identification [%s], and Item Number [%d] so no Purchase Order Accounts Written and no Purchase Order Transactions Written.",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
						}
					}
					UNLNRDFILE(puracc);
					UNLNRDFILE(reqacc);
					FreeREQACC(REQACCvalue);
					LOCNRDFILE(reqitm);
					FINDFLDSETSTRING(reqitm,"LOCATION IDENTIFICATION",locid);
					FINDFLDSETSTRING(reqitm,"REQUISITION IDENTIFICATION",reqid);
					FINDFLDSETINT(reqitm,"ITEM NUMBER",itemno);
					if(!ADVEQLNRDsec(reqitm,1,ReqCPOSubData,p))
					{ 
						prev=RDATDataNEW(reqitm);
						FINDFLDSETCHAR(reqitm,"ISSUED",TRUE);
						FINDFLDSETSTRING(reqitm,"ISSUED DATE",curdate);
						FINDFLDSETSTRING(reqitm,"ISSUED TIME",curtime);
						FINDFLDSETSTRING(reqitm,"ISSUED BY",USERLOGIN);
						ADVWRTTRANSsec(reqitm,0,NULL,prev,ReqCPOSubData,p);
					}
					UNLNRDFILE(reqitm);
					if(prev!=NULL) Rfree(prev);
					if(fixmst!=(-1))
					{
						FINDFLDGETINT(dbsort->fileno,"TYPE",&type);
						if(type==1) /* Fixed Asset Type */
						{
							LOCNRDFILE(fixmst);
							COPYFIELD(dbsort->fileno,fixmst,"ITEM IDENTIFICATION");
/*
							FIELDCOPY(dbsort->fileno,"ITEM IDENTIFICATION",fixmst,"ASSET IDENTIFICATION");
*/
							FINDFLDGETINT(dbsort->fileno,"QUANTITY",&qty);
							if(ADVEQLNRDsec(fixmst,1,ReqCPOSubData,p))
							{
								KEYNRD(fixmst,1);
								COPYFIELD(dbsort->fileno,fixmst,"CATEGORY IDENTIFICATION");
								COPYFIELD(dbsort->fileno,fixmst,"DESCRIPTION");
								COPYFIELD(dbsort->fileno,fixmst,"COMMENTS");
								FINDFLDSETINT(fixmst,"QUANTITY",0);
								FINDFLDSETINT(fixmst,"UNRECEIVED QUANTITY",qty);
								FINDFLDSETSTRING(fixmst,"USE",NULL);
								FINDFLDSETCHAR(fixmst,"DELETEFLAG",FALSE);
							} else {
								prev=RDATDataNEW(fixmst);
								FINDFLDGETCHAR(fixmst,"DELETEFLAG",&delflag);
								if(!delflag)
								{
									FINDFLDGETINT(fixmst,"UNRECEIVED QUANTITY",&fixqty);
									fixqty+=qty;
									FINDFLDSETINT(fixmst,"UNRECEIVED QUANTITY",fixqty);
								} else {
									KEYNRD(fixmst,1);
									FINDFLDSETCHAR(fixmst,"DELETEFLAG",FALSE);
									COPYFIELD(dbsort->fileno,fixmst,"CATEGORY IDENTIFICATION");
									COPYFIELD(dbsort->fileno,fixmst,"DESCRIPTION");
									COPYFIELD(dbsort->fileno,fixmst,"COMMENTS");
									FINDFLDSETINT(fixmst,"UNRECEIVED QUANTITY",qty);
									FINDFLDSETSTRING(fixmst,"USE",NULL);
								}
							}
							ADVWRTTRANSsec(fixmst,0,NULL,prev,ReqCPOSubData,p);
							UNLNRDFILE(fixmst);
							if(prev!=NULL) Rfree(prev);
						}
					}
				} else {
					UNLNRDFILE(purlin);
				}
			}
			if(tmprsrc!=NULL)
				update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			if(curdate!=NULL) Rfree(curdate);
			if(curtime!=NULL) Rfree(curtime);
			if(locid!=NULL) Rfree(locid);
			if(reqid!=NULL) Rfree(reqid);
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
	}
}
void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	NRDfield *field=NULL,*FIELDS=NULL;
	int x=0,numfields=0;
	short keyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"CREATE PO SORT SEQUENCE");
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
	numfields=NUMFLDS(purlin);
	FIELDS=FLDPOINTER(purlin);
	if(FIELDS!=NULL)
	{
		for(x=0,field=FIELDS;x<numfields;++x,++field)
		{
			if(!RDAstrcmp(field->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(field->name,"RECEIVED QUANTITY"))
			{
			} else if(!RDAstrcmp(field->name,"REQUISITIONED ITEM"))
			{
			} else if(!RDAstrcmp(field->name,"REQUISITION TRANSACTION NUMBER"))
			{
			} else if(!RDAstrcmp(field->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(field->name,"ENTRY DATE"))
			{
			} else if(!RDAstrcmp(field->name,"ENTRY TIME"))
			{
			} else {
				addDBfield(dbsrt,field->name,field->type,field->len);
			}
		}
	}
	addDBfilefield(dbsrt,purmst,"VENDOR IDENTIFICATION");
	addDBfilefield(dbsrt,reqitm,"ITEM NUMBER");
	addDBfield(dbsrt,"PROCESSING MONTH",SHORTV,2);
	key=KEYNUM(purlin,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
}
void addsortrecord(DBsort *dbsrt,RDArsrc *mainrsrc,int fiscalyr,short procmo,
	char *poid,char *locid,int itemno,char *itemid,char *desc,
	char *comments,char *reqid,char *venid,char *venitmno,
	char *catid,int qty,double rate,double total)
{
	char deleteflag=FALSE;
	PassableStruct *p=NULL;

	ZERNRD(dbsrt->fileno);
	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		p=Rmalloc(sizeof(PassableStruct));
		p->r=mainrsrc;
		p->a=dbsrt;
		computevirtual(csortsequence,ReqCPOSubData,p);
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
	FINDFLDSETINT(dbsrt->fileno,"FISCAL YEAR",fiscalyr);
	FINDFLDSETSTRING(dbsrt->fileno,"PO IDENTIFICATION",poid);
	FINDFLDSETSTRING(dbsrt->fileno,"VENDOR IDENTIFICATION",venid);
	FINDFLDSETINT(dbsrt->fileno,"ITEM NUMBER",itemno);
	FINDFLDSETSTRING(dbsrt->fileno,"ITEM IDENTIFICATION",itemid);
	FINDFLDSETSTRING(dbsrt->fileno,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(dbsrt->fileno,"CATEGORY IDENTIFICATION",catid);
	FINDFLDSETSTRING(dbsrt->fileno,"REQUISITION IDENTIFICATION",reqid);
	FINDFLDSETSTRING(dbsrt->fileno,"VENDOR ITEM NUMBER",venitmno);
	FINDFLDSETSTRING(dbsrt->fileno,"DESCRIPTION",desc);
	FINDFLDSETSTRING(dbsrt->fileno,"COMMENTS",comments);
	FINDFLDSETINT(dbsrt->fileno,"QUANTITY",qty);
	FINDFLDSETDOUBLE(dbsrt->fileno,"UNIT RATE",rate);
	FINDFLDSETDOUBLE(dbsrt->fileno,"TOTAL",total);
	FINDFLDSETSHORT(dbsrt->fileno,"PROCESSING MONTH",procmo);
	FINDFLDSETINT(dbsrt->fileno,"TYPE",0);
	WRTNRD(dbsrt->fileno,1,"LINE NUMBER");
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
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without Creating this Purchase Order?",doexit,NULL,FALSE,blist);
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
	if(brsrc!=NULL) free_rsrc(brsrc);
	if(WindowCount<=2) select_backend(dbsort);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static void savepurmstrsp(RDArsrc *mainrsrc)
{
	savepurmst(mainrsrc);
	addpo(mainrsrc);
}
static void test_select(RDArsrc *mainrsrc)
{
	if(purmst_test(mainrsrc)==TRUE)
	{
		if(ADVEQLNRDsec(purmst,1,SCRNvirtualSubData,mainrsrc))
		{
			WARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"PURCHASE ORDER MASTER DOES NOT EXIST!",
				"Purchase Order Lines can not be added until the Purchase Order Master has been saved.  The PO Identification entered does not presently exist in the PURMST data file.  Do you wish to create this Purchase Order Master record?",
				savepurmstrsp,NULL,FALSE,mainrsrc);
		} else {
			addpo(mainrsrc);
		}
	}
}
static void addpo(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,dodiag=FALSE,testfile=FALSE;
	short method=0;
	char deleteflag=FALSE,issued=FALSE,suv=FALSE,changed=FALSE;
	char *sortname=NULL/*,submitted=FALSE*/,active=FALSE;
	char *locid=NULL,*reqid=NULL;
	char *addrid=NULL,*desc=NULL,*comments=NULL;
	char *venid=NULL,*venitmno=NULL,*catid=NULL,*itemid=NULL;
	int itemno=0,fiscalyr=0,qty=0;
	short procmo=0,appstat=0;
	double total=0.0,rate=0.0;
	char approved=FALSE,hfrfp=FALSE,proaccept=FALSE;
	int x;
	long size_of_file=0;
	RDAvirtual *v=NULL;
	PassableStruct *p=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"[PURMST][VENDOR IDENTIFICATION]",&VENDORID);
	FINDRSCGETINT(mainrsrc,"[PURMST][FISCAL YEAR]",&FiscalYear);
	FINDRSCGETSTRING(mainrsrc,"[PURMST][PO IDENTIFICATION]",&POID);
	FINDRSCGETINT(mainrsrc,"SELECT TYPE",&select_type);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&select_expression);
	FINDRSCGETINT(mainrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",&x);
	if(RDAstrcmp(ordadravl->libs[x],"No Address Defined"))
	{
		ADDRID=stralloc(ordadravl->libs[x]);
	} else {
		ADDRID=NULL;
	}
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(reqitm,rs);
	ReadRangeScreen(reqmst,rs);
	ReadRangeScreen(reqloc,rs);
	tmprsrc=diagscrn(reqitm,"DIAGNOSTIC SCREEN",module,
		"Selecting Requisitioned Items",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(tmprsrc);
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=dbsort;
	ZERNRD(reqitm);
	ef=ADVFRSNRDsec(reqitm,1,ReqCPOSubData,p);
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETSTRING(reqitm,"LOCATION IDENTIFICATION",&locid);
		FINDFLDGETSTRING(reqitm,"REQUISITION IDENTIFICATION",&reqid);
		FINDFLDGETINT(reqitm,"ITEM NUMBER",&itemno);
		FINDFLDGETCHAR(reqitm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		FINDFLDGETCHAR(reqitm,"SOURCE USER VERIFIED",&suv);
		if(suv)
		{
		FINDFLDGETCHAR(reqitm,"CHANGED",&changed);
		if(!changed)
		{
		FINDFLDGETCHAR(reqitm,"ISSUED",&issued);
		if(!issued)
		{
		FINDFLDGETSHORT(reqitm,"METHOD",&method);
		if(method==1)
		{
		FINDFLDGETCHAR(reqitm,"HELD FOR RFP",&hfrfp);
		FINDFLDGETCHAR(reqitm,"PROPOSAL ACCEPTED",&proaccept);
		if(!hfrfp || (hfrfp && proaccept))
		{
			FINDFLDGETSTRING(reqitm,"VENDOR IDENTIFICATION",&venid);
			FINDFLDGETSTRING(reqitm,"ADDRESS IDENTIFICATION",&addrid);
			FINDFLDGETINT(reqitm,"FISCAL YEAR",&fiscalyr);
			if(!RDAstrcmp(venid,VENDORID))
			{
			if(!RDAstrcmp(addrid,ADDRID))
			{
			if(fiscalyr==FiscalYear)
			{
			testfile=testfilerangersrc(reqitm,rs);
			if(!testfile)
			{
				FINDFLDGETSTRING(reqitm,"LOCATION IDENTIFICATION",&locid);
				FINDFLDGETSTRING(reqitm,"REQUISITION IDENTIFICATION",&reqid);
				FINDFLDGETINT(reqitm,"ITEM NUMBER",&itemno);
				FINDFLDGETSTRING(reqitm,"ITEM NAME",&desc);
				FINDFLDGETSTRING(reqitm,"DESCRIPTION",&comments);
				FINDFLDGETSTRING(reqitm,"CATEGORY IDENTIFICATION",&catid);
				FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid);
				FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid);
				if(ADVEQLNRDsec(reqmst,1,ReqCPOSubData,p)) KEYNRD(reqmst,1);
				FINDFLDGETCHAR(reqmst,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
/*
				FINDFLDGETCHAR(reqmst,"SUBMITTED",&submitted);
				if(submitted)
				{
*/
				FINDFLDGETCHAR(reqmst,"APPROVED",&approved);
				if(approved)
				{
				FINDFLDGETSHORT(reqmst,"APPROVAL STATUS",&appstat);
				if(appstat==1)
				{
					testfile=testfilerangersrc(reqmst,rs);
					if(!testfile)
					{
						COPYFIELD(reqmst,reqloc,"LOCATION IDENTIFICATION");
						if(ADVEQLNRDsec(reqloc,1,ReqCPOSubData,p)) KEYNRD(reqloc,1);
						FINDFLDGETCHAR(reqloc,"DELETEFLAG",&deleteflag);
						FINDFLDGETCHAR(reqloc,"ACTIVE",&active);
						if(!deleteflag)
						{
						if(active)
						{
							testfile=testfilerangersrc(reqloc,rs);
							if(!testfile)
							{
								for(x=0,v=mainrsrc->virflds;
									x<mainrsrc->num;++x,++v)
								{
									if(ADVtestvirtualrangersrc(v,
										rs,(void (*)(...))ReqCPOSubData,p)) break;
								}
								if(x>=mainrsrc->num)
								{
									FINDFLDGETSHORT(reqitm,"PROCESSING MONTH",&procmo);
									FINDFLDGETSTRING(reqitm,"VENDOR ITEM NUMBER",&venitmno);
									FINDFLDGETSTRING(reqitm,"ITEM IDENTIFICATION",&itemid);
									FINDFLDGETINT(reqitm,"QUANTITY",&qty);
									FINDFLDGETDOUBLE(reqitm,"UNIT RATE",&rate);
									FINDFLDGETDOUBLE(reqitm,"AMOUNT",&total);
									addsortrecord(dbsort,mainrsrc,FiscalYear,procmo,POID,locid,itemno,itemid,desc,comments,
										reqid,VENDORID,venitmno,catid,qty,rate,total);
									dodiag=TRUE;
								} else {
								if(diagapps)
								{
									prterr("Skipped by Ranging on Virtual Field [%s].",v->name);
								}

								}
							} else {
							if(diagapps)
							{
								prterr("Skipped the [REQMGT][REQLOC] (Requisition Location Master) data file record Location Identification [%s] because of Ranging on Field Values.",
								(locid==NULL?"":locid));
							}
							}
						} else {
						if(diagapps)
						{
							prterr("Skipped the [REQMGT][REQLOC] (Requisition Location Master) data file record Location Identification [%s] because of ACTIVE field value of FALSE.",
							(locid==NULL?"":locid));
						}
						}
						} else {
						if(diagapps)
						{
							prterr("Skipped the [REQMGT][REQLOC] (Requisition Location Master) data file record Location Identification [%s] because of DELETEFLAG field value of TRUE.",
							(locid==NULL?"":locid));
						}
						}
					} else {
					if(diagapps)
					{
						prterr("Skipped the [REQMGT][REQMST] (Requisition Master) data file record Location Identification [%s] and Requisition Identification [%s] because of Ranging on Field Values.",
						(locid==NULL?"":locid),(reqid==NULL?"":reqid));
					}
					}
				} else {
				if(diagapps)
				{
					prterr("Skipped the [REQMGT][REQMST] (Requisition Master) data file record Location Identification [%s] and Requisition Identification [%s] because of APPROVAL STATUS field value not equal to Approved [1].",
					(locid==NULL?"":locid),(reqid==NULL?"":reqid));
				}
				}
				} else {
				if(diagapps)
				{
					prterr("Skipped the [REQMGT][REQMST] (Requisition Master) data file record Location Identification [%s] and Requisition Identification [%s] because of APPROVED field value of FALSE.",
					(locid==NULL?"":locid),(reqid==NULL?"":reqid));
				}
				}
/*
				} else {
				if(diagapps)
				{
					prterr("Skipped the [REQMGT][REQMST] (Requisition Master) data file record Location Identification [%s] and Requisition Identification [%s] because of SUBMITTED field value of FALSE.",
					(locid==NULL?"":locid),(reqid==NULL?"":reqid));
				}
				}
*/
				} else {
				if(diagapps)
				{
					prterr("Skipped the [REQMGT][REQMST] (Requisition Master) data file record Location Identification [%s] and Requisition Identification [%s] because of DELETEFLAG field value of TRUE.",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid));
				}
				}
			} else {
				if(diagapps)
				{
					prterr("Skipped the [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] because of Ranging on Field Values.",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
				}
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped because the FISCAL YEAR field in the [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is [%d] which doesn't match the selected FISCAL YEAR value of [%d].",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),
				itemno,fiscalyr,FiscalYear);
			}
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped because the Vendor ADDRESS IDENTIFICATION field in the [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is [%s] which doesn't match the Vendor ADDRESS IDENTIFICATION value of [%s] for the VENDOR IDENTIFICATION of [%s].",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),
				itemno,(venid==NULL?"":venid),
				(ADDRID==NULL?"":ADDRID),
				(VENDORID==NULL?"":VENDORID));
			}
			}
			} else {
			if(diagapps)
			{
				prterr("Skipped because the VENDOR IDENTIFICATION field in the [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is [%s] which doesn't match the VENDOR IDENTIFICATION value of [%s].",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),
				itemno,(venid==NULL?"":venid),
				(VENDORID==NULL?"":VENDORID));
			}
			}
		} else {
		if(diagapps)
		{
			prterr("Skipped because the HELD FOR RFP field is set TRUE and the PROPOSAL ACCEPTED field is set to FALSE in the [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d].",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because the METHOD field of [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is not set to Purchase Order [1].",(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because the ISSUED field of [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is set to TRUE.",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because the CHANGED field of [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is set to TRUE.",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because the SOURCE USER VERIFIED field of [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is set to FALSE.",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
		}
		}
		} else {
		if(diagapps)
		{
			prterr("Skipped because the DELETEFLAG field of [REQMGT][REQITM] (Requisition Item Master) data file record Location Identification [%s], Requisition Identification [%s], and Item Number [%d] is set to TRUE.",
				(locid==NULL?"":locid),(reqid==NULL?"":reqid),itemno);
		}
		}
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		}
		ef=ADVNXTNRDsec(reqitm,1,ReqCPOSubData,p);
	}
	if(p!=NULL) Rfree(p);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(venid!=NULL) Rfree(venid);
	if(addrid!=NULL) Rfree(addrid);
	if(venitmno!=NULL) Rfree(venitmno);
	if(catid!=NULL) Rfree(catid);
	if(reqid!=NULL) Rfree(reqid);
	if(desc!=NULL) Rfree(desc);
	if(comments!=NULL) Rfree(comments);
	if(locid!=NULL) Rfree(locid);
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
				brsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDER LINES BROWSE");
				file2rsrc(purmst,brsrc,FALSE);
				filerecord2rsrc(purmst,brsrc);
				file2rsrc(vennum,brsrc,FALSE);
				filerecord2rsrc(vennum,brsrc);
				file2rsrc(fsclyr,brsrc,FALSE);
				filerecord2rsrc(fsclyr,brsrc);
				addDFincvir(brsrc,"PURORD","PURMST",NULL,purmst);
				addDFincvir(brsrc,"VENPMT","VENMSR",NULL,vennum);
				addDFincvir(brsrc,"FINMGT","FINYEAR",NULL,fsclyr);
				drsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDER LINES DEFINE LIST");
				addDFincvir(drsrc,"PURORD","PURMST",NULL,purmst);
				addDFincvir(drsrc,"PURORD","PURSIT",NULL,pursit);
				addDFincvir(drsrc,"VENPMT","VENMSR",NULL,vennum);
				addDFincvir(drsrc,"FINMGT","FINYEAR",NULL,fsclyr);
				addDFincvir(drsrc,module,"REQITM",NULL,reqitm);
				addDFincvir(drsrc,module,"REQLOC",NULL,reqloc);
				addDFincvir(drsrc,module,"REQMST",NULL,reqmst);
				srsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDER LINES SEARCH BROWSE");
				addDFincvir(srsrc,"PURORD","PURMST",NULL,purmst);
				addDFincvir(srsrc,"PURORD","PURSIT",NULL,pursit);
				addDFincvir(srsrc,"VENPMT","VENMSR",NULL,vennum);
				addDFincvir(srsrc,"FINMGT","FINYEAR",NULL,fsclyr);
				addDFincvir(srsrc,module,"REQITM",NULL,reqitm);
				addDFincvir(srsrc,module,"REQLOC",NULL,reqloc);
				addDFincvir(srsrc,module,"REQMST",NULL,reqmst);
				GET_SCREEN_VIRTUALS(brsrc,0);
				GET_SCREEN_VIRTUALS(drsrc,1);
				GET_SCREEN_VIRTUALS(srsrc,2);
				ZERNRD(dbsort->fileno);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_purlin);
				mbl_purlin=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,
					selectfunction,doexit_test,
					okfunc,NULL,dbsort,TRUE,NULL,
					0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_purlin);
				break;
		}
	} else {
		ERRORDIALOG("CREATE PURCHASE ORDERS ERROR!","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
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
	}
}
void quitselect(RDArsrc *mainrsrc)
{
	char *poid=NULL;
	APPlib *args=NULL;

	readwidget(mainrsrc,"[PURMST][PO IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PURMST][PO IDENTIFICATION]",&poid);
	ADVUndoIDifnoPO(purids,purmst,poid,SCRNvirtualSubData,mainrsrc);
	if(poid!=NULL) Rfree(poid);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(reqitm!=(-1)) CLSNRD(reqitm);
	if(reqmst!=(-1)) CLSNRD(reqmst);
	if(reqloc!=(-1)) CLSNRD(reqloc);
	if(catmst!=(-1)) CLSNRD(catmst);
	if(reqacc!=(-1)) CLSNRD(reqacc);
	if(purmst!=(-1)) CLSNRD(purmst);
	if(purlin!=(-1)) CLSNRD(purlin);
	if(pursit!=(-1)) CLSNRD(pursit);
	if(puracc!=(-1)) CLSNRD(puracc);
	if(purtrn!=(-1)) CLSNRD(purtrn);
	if(purids!=(-1)) CLSNRD(purids);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(venadrnum!=(-1)) CLSNRD(venadrnum);
	if(venadr2num!=(-1)) CLSNRD(venadr2num);
	if(fsclyr!=(-1)) CLSNRD(fsclyr);
	if(fixmst!=(-1)) CLSNRD(fixmst);
	if(selecttype!=NULL) freeselecttype();
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(sitesavl!=NULL) freeapplib(sitesavl);
	if(ordadravl!=NULL) freeapplib(ordadravl);
	if(billadravl!=NULL) freeapplib(billadravl);
	if(prev_purmst!=NULL) FreeRDATData(prev_purmst);
	if(POID!=NULL) Rfree(POID);
	if(VENDORID!=NULL) Rfree(VENDORID);
	if(ADDRID!=NULL) Rfree(ADDRID);
	if(POLINE_COUNT>0)
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
short selectbpurmst(MakeBrowseList *blist)
{
	char closed=FALSE,delflag=FALSE;

	FINDFLDGETCHAR(purmst,"CLOSED",&closed);
	FINDFLDGETCHAR(purmst,"DELETEFLAG",&delflag);
	if(!closed && !delflag) return(TRUE);
	else return(FALSE);
}
void quitbpurmst(MakeBrowseList *blist)
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
	if(blist->mainrsrc!=NULL) free_rsrc(blist->mainrsrc);
}
void okbpurmst(MakeBrowseList *b,void *targetkey)
{
	char *poid=NULL,*temp=NULL;

	readwidget(b->funcargs,"[PURMST][PO IDENTIFICATION]");
	FINDRSCGETSTRING(b->funcargs,"[PURMST][PO IDENTIFICATION]",&poid);
	ADVUndoIDifnoPO(purids,purmst,poid,SCRNvirtualSubData,b->funcargs);
	if(poid!=NULL) Rfree(poid);
	ZERNRD(purmst);
	if(ADVEQLNRDKEYsec(purmst,1,targetkey,SCRNvirtualSubData,b->funcargs))
	{
		FINDFLDGETSTRING(purmst,"PO IDENTIFICATION",&poid);
		KEYNRD(purmst,1);
		ClearRDATData(prev_purmst);
		if(ADVCheckPOID(purids,poid,SCRNvirtualSubData,b->funcargs))
		{
			ADVAssignPOID(purids,poid,SCRNvirtualSubData,b->funcargs);	
		} else {
			temp=ADVGetAssignNextPOID(purids,temp,SCRNvirtualSubData,b->funcargs);
			if(poid!=NULL) Rfree(poid);
			poid=NULL;
			if(temp!=NULL)
			{
				poid=stralloc(temp);
				Rfree(temp);
			}
			FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
			FINDRSCSETSTRING(b->funcargs,"[PURMST][PO IDENTIFICATION]",poid);
			updatersrc(b->funcargs,"[PURMST][PO IDENTIFICATION]");
			if(!isEMPTY(poid)) 
			{
				setpoid(b->funcargs,FALSE);
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!","There are no valid PO Identifications Available.  Please create more PO Identifications.",NULL,NULL,TRUE,NULL);
			}
		}
		if(poid!=NULL) Rfree(poid);
	} else {
		loadaddresslist(b->funcargs);
		updatefilerecord2rsrc(purmst,b->funcargs);
		setsiteid(b->funcargs);
		GetRDATData(purmst,prev_purmst);
	}
	getsupportingpurmstfiles(b->funcargs);
	updateSCRNvirtuals(b->funcargs);
}
void browse_purmst(RDArsrc *mainrsrc)
{
	RDArsrc *mrsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	mrsrc=RDArsrcNEW(module,"PO MASTER BROWSE");
	srsrc=RDArsrcNEW(module,"PO MASTER SEARCH BROWSE");
	drsrc=RDArsrcNEW(module,"PO MASTER DEFINE LIST");
	addDFincvir(drsrc,module,"PURMST",NULL,purmst);
	addDFincvir(srsrc,module,"PURMST",NULL,purmst);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	ADVbrowse(purmst,1,mrsrc,srsrc,drsrc,NULL,selectbpurmst,quitbpurmst,
		okbpurmst,NULL,mainrsrc,TRUE,NULL,2,2,2,FALSE);
}
static void setpoidcb(RDArsrc *mainrsrc)
{
	setpoid(mainrsrc,TRUE);
	updateSCRNvirtuals(mainrsrc);
}
static void setpoid(RDArsrc *mainrsrc,short update)
{
	char *poid=NULL,*temp=NULL;
	int fiscalyr=0;

	if(update)
	{
		readwidget(mainrsrc,"[PURMST][FISCAL YEAR]");
		readwidget(mainrsrc,"[PURMST][PO IDENTIFICATION]");
	}
	FINDRSCGETINT(mainrsrc,"[PURMST][FISCAL YEAR]",&fiscalyr);
	FINDRSCGETSTRING(mainrsrc,"[PURMST][PO IDENTIFICATION]",&poid);
	ADVUndoIDifnoPO(purids,purmst,poid,SCRNvirtualSubData,mainrsrc);
	ZERNRD(purmst);
	FINDFLDSETINT(purmst,"FISCAL YEAR",fiscalyr);
	FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
	if(ADVEQLNRDsec(purmst,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(purmst,1);
		ClearRDATData(prev_purmst);
		if(ADVCheckPOID(purids,poid,SCRNvirtualSubData,mainrsrc))
		{
			ADVAssignPOID(purids,poid,SCRNvirtualSubData,mainrsrc);	
		} else {
			temp=ADVGetAssignNextPOID(purids,poid,SCRNvirtualSubData,mainrsrc);
			if(poid!=NULL) Rfree(poid);
			poid=NULL;
			if(temp!=NULL)
			{
				poid=stralloc(temp);
				Rfree(temp);
			}
			FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
			FINDRSCSETSTRING(mainrsrc,"[PURMST][PO IDENTIFICATION]",poid);
			updatersrc(mainrsrc,"[PURMST][PO IDENTIFICATION]");
			if(!isEMPTY(poid))
			{
				setpoid(mainrsrc,FALSE);
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!","There are no valid PO Identifications Available.  Please create more PO Identifications.",NULL,NULL,TRUE,NULL);
			}
		}
	} else {
		loadaddresslist(mainrsrc);
		if(update) updatefilerecord2rsrc(purmst,mainrsrc);
		else filerecord2rsrc(purmst,mainrsrc);
		setsiteid(mainrsrc);
                GetRDATData(purmst,prev_purmst);
	}
	getsupportingpurmstfiles(mainrsrc);
	if(poid!=NULL) Rfree(poid);
}
static void savecheck_purmst(RDArsrc *mainrsrc)
{
	if(purmst_test(mainrsrc)==TRUE) savepurmst(mainrsrc);
}
static short purmst_test(RDArsrc *mainrsrc)
{
	char deleteflag=FALSE,*warnmessage=NULL;
	char *poid=NULL;
	int yearid=0,selected=0;

	readfilewidgets(purmst,mainrsrc);
	FINDRSCGETINT(mainrsrc,"[PURMST][FISCAL YEAR]",&yearid);
	FINDFLDSETINT(fsclyr,"FISCAL YEAR",yearid);
	if(ADVEQLNRDsec(fsclyr,1,SCRNvirtualSubData,mainrsrc))
	{
		warnmessage=Rmalloc(180);
		sprintf(warnmessage,"There is not a valid FISCAL YEAR record for the Financial Fiscal Year of [%4d].  Before this record can be saved there must be a valid FISCAL YEAR entered.",yearid);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
		if(poid!=NULL) Rfree(poid);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(fsclyr,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			warnmessage=Rmalloc(185);
			sprintf(warnmessage,"The FISCAL YEAR record for the Financial Fiscal Year of [%4d] is in a state of deletion.  Before this record can be saved there must be a valid FISCAL YEAR entered.",yearid);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
			if(poid!=NULL) Rfree(poid);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(fsclyr,"ACTIVE",&deleteflag);
			if(!deleteflag)
			{
				warnmessage=Rmalloc(180);
				sprintf(warnmessage,"The FISCAL YEAR record for the Finanical Fiscal Year of [%4d] is not active.  Before this record can be saved there must be a valid FISCAL YEAR entered.",yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
				if(poid!=NULL) Rfree(poid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return(FALSE);
			}
		}
	}
	FINDRSCGETSTRING(mainrsrc,"[PURMST][PO IDENTIFICATION]",&poid);
	FINDFLDSETSTRING(purmst,"PO IDENTIFICATION",poid);
	if(!ADVEQLNRDsec(purmst,1,SCRNvirtualSubData,mainrsrc))
	{
		FINDFLDGETCHAR(purmst,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			warnmessage=Rmalloc(RDAstrlen(poid)+230);
			sprintf(warnmessage,"The Purchase Order Master record for the PO Identification of [%s] and the Fiscal Year of [%4d] is in a state of deletion.  Before this record can be saved there must a new or valid PO Identification.",(poid==NULL?"":poid),yearid);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PO IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(poid!=NULL) Rfree(poid);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(purmst,"CLOSED",&deleteflag);
			if(deleteflag)
			{
				warnmessage=Rmalloc(RDAstrlen(poid)+300);
				sprintf(warnmessage,"The method in the Requisition Item Master must be set to PURCHSASE ORDER or the Purchase Order Master record for the PO Identification of [%s] and Fiscal Year of [%4d] has been closed.  A Purchase Order Line cannot be added to a Closed Purchase Order.  A new or valid PO Identification must be selected before proceeding further.",(poid==NULL?"":poid),yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","CLOSED PURCHASE ORDER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(poid!=NULL) Rfree(poid);
				return(FALSE);
			}
		}
	}
	if(poid!=NULL) Rfree(poid);
	FINDRSCGETINT(mainrsrc,"SITE IDENTIFICATIONS",&selected);
	if(!RDAstrcmp(sitesavl->libs[selected],"No Sites Defined"))
	{
		warnmessage=Rmalloc(170);
		sprintf(warnmessage,"There are no SITE IDENTIFICATIONs available in the [PURORD][PURSIT] Setup data file.  A Purchase Order cannot be saved with out a valid Site Identification.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","CLOSED PURCHASE ORDER!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(poid!=NULL) Rfree(poid);
		return(FALSE);
	}
	if(checkvendor(mainrsrc)==FALSE)
	{
		return(FALSE);
	}
	return(TRUE);
}
static void savepurmst(RDArsrc *mainrsrc)
{
	char *rcddesc=NULL;
	int selected=0;
	char *cdate=NULL,*ctime=NULL;

	rsrc2filerecord(purmst,mainrsrc);
	FINDRSCGETINT(mainrsrc,"SITE IDENTIFICATIONS",&selected);
	if(RDAstrcmp(sitesavl->libs[selected],"No Sites Defined"))
	{
		FINDFLDSETSTRING(purmst,"SITE IDENTIFICATION",sitesavl->libs[selected]);
	} else {
		FINDFLDSETSTRING(purmst,"SITE IDENTIFICATION",NULL);
	}
	FINDRSCGETINT(mainrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(ordadravl->libs[selected],"No Address Defined"))
	{
		FINDFLDSETSTRING(purmst,"PO VENDOR ADDRESS IDENTIFICATION",ordadravl->libs[selected]);
	} else {
		FINDFLDSETSTRING(purmst,"PO VENDOR ADDRESS IDENTIFICATION",NULL);
	}
	FINDRSCGETINT(mainrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(billadravl->libs[selected],"No Address Defined"))
	{
		FINDFLDSETSTRING(purmst,"BILLING VENDOR ADDRESS IDENTIFICATION",billadravl->libs[selected]);
	} else {
		FINDFLDSETSTRING(purmst,"BILLING VENDOR ADDRESS IDENTIFICATION",billadravl->libs[selected]);
	}
	cdate=GETCURRENTDATE10();
	FINDFLDSETSTRING(purmst,"ENTRY DATE",cdate);
	ctime=GETCURRENTTIME();
	FINDFLDSETSTRING(purmst,"ENTRY TIME",ctime);
	FINDFLDSETSTRING(purmst,"SOURCE USER",USERLOGIN);
	if(ADVRECORDsec(purmst,SCRNvirtualSubData,mainrsrc))
	{
		rcddesc=ADVRECORDsecDesc(purmst,SCRNvirtualSubData,mainrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(purmst,0,NULL,prev_purmst);
	}
	GetRDATData(purmst,prev_purmst);
}
static void getsiteid(RDArsrc *mainrsrc)
{
	int sitesel=0;

	FINDRSCGETINT(mainrsrc,"SITE IDENTIFICATIONS",&sitesel);
	if(sitesavl->numlibs>sitesel &&
	RDAstrcmp(sitesavl->libs[sitesel],""))
	{
		FINDFLDSETSTRING(purmst,"SITE IDENTIFICATION",
			sitesavl->libs[sitesel]);
		FINDFLDSETSTRING(pursit,"SITE IDENTIFICATION",
			sitesavl->libs[sitesel]);
	} else {
		FINDFLDSETSTRING(purmst,"SITE IDENTIFICATION",NULL);
		FINDFLDSETSTRING(pursit,"SITE IDENTIFICATION",NULL);
	}
	if(ADVEQLNRDsec(pursit,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(pursit,1);
	updatefilerecord2rsrc(pursit,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
static void setaddresses(RDArsrc *mainrsrc)
{
	short ef;
	char *vendorid=NULL,*vendorid2=NULL;
	char *addrid=NULL,*poadrid=NULL,*billadrid=NULL;
	char ordboolval=FALSE,billboolval=FALSE;
	int selordadr=0,selbilladr=0;

	FINDFLDGETSTRING(purmst,"PO VENDOR ADDRESS IDENTIFICATION",&poadrid);
	selordadr=FINDAPPLIBELEMENT(ordadravl,poadrid);
	if(selordadr==(-1))
	{
		ZERNRD(venadrnum);
		FINDFLDGETSTRING(purmst,"VENDOR IDENTIFICATION",&vendorid);
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
		ef=ADVGTENRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
			if(RDAstrcmp(vendorid2,vendorid)) break;
			FINDFLDGETCHAR(venadrnum,"DEFAULT ORDER",&ordboolval);
			if(ordboolval==TRUE)
			{
				FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&poadrid);
				selordadr=FINDAPPLIBELEMENT(ordadravl,poadrid);
				return;
			}
			ef=ADVNXTNRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc);
		}
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(ordboolval==FALSE)
		{
			selordadr=0;
		}
	}
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",ordadravl->libs[selordadr]);
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(venadrnum,1);
	} else {
		updatefilerecord2rsrc(venadrnum,mainrsrc);
	}
	FINDRSCSETINT(mainrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",selordadr);
	updatersrc(mainrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS");
	FINDFLDGETSTRING(purmst,"BILLING VENDOR ADDRESS IDENTIFICATION",&billadrid);
	selbilladr=FINDAPPLIBELEMENT(billadravl,billadrid);
	if(selbilladr==(-1))
	{
		ZERNRD(venadr2num);
		FINDFLDGETSTRING(purmst,"VENDOR IDENTIFICATION",&vendorid);
		FINDFLDSETSTRING(venadr2num,"VENDOR IDENTIFICATION",vendorid);
		ef=ADVGTENRDsec(venadr2num,1,SCRNvirtualSubData,mainrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(venadr2num,"VENDOR IDENTIFICATION",&vendorid2);
			if(RDAstrcmp(vendorid2,vendorid)) break;
			FINDFLDGETCHAR(venadr2num,"DEFAULT BILLING",&billboolval);
			if(billboolval==TRUE)
			{
				FINDFLDGETSTRING(venadr2num,"ADDRESS IDENTIFICATION",&billadrid);
				selbilladr=FINDAPPLIBELEMENT(billadravl,billadrid);
				return;
			}
			ef=ADVNXTNRDsec(venadr2num,1,SCRNvirtualSubData,mainrsrc);
		}
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(billboolval==FALSE)
		{
			selbilladr=0;
		}
	}
	ZERNRD(venadr2num);
	FINDFLDSETSTRING(venadr2num,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadr2num,"ADDRESS IDENTIFICATION",billadravl->libs[selbilladr]);
	if(ADVEQLNRDsec(venadr2num,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(venadr2num,1);
	} else {
		updatesinglerecord2rsrc(venadr2num,mainrsrc);
	}
	FINDRSCSETINT(mainrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",selbilladr);
	updatersrc(mainrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS");
	if(poadrid!=NULL) Rfree(poadrid);
	if(billadrid!=NULL) Rfree(billadrid);
	if(addrid!=NULL) Rfree(addrid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void setaddressescb(RDArsrc *mainrsrc)
{
	int selordadr=0,selbilladr=0;
	char *ordadrid=NULL,*billadrid=NULL;

	FINDRSCGETINT(mainrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",&selordadr);
	if(RDAstrcmp(ordadravl->libs[selordadr],"No Address Defined"))
	{
		ordadrid=stralloc(ordadravl->libs[selordadr]);
	}
	FINDFLDSETSTRING(purmst,"PO VENDOR ADDRESS IDENTIFICATION",ordadrid);
	FINDRSCGETINT(mainrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",&selbilladr);
	if(RDAstrcmp(billadravl->libs[selbilladr],"No Address Defined"))
	{
		billadrid=stralloc(billadravl->libs[selbilladr]);
	}
	FINDFLDSETSTRING(purmst,"BILLING VENDOR ADDRESS IDENTIFICATION",billadrid);
	if(ordadrid!=NULL) Rfree(ordadrid);
	if(billadrid!=NULL) Rfree(billadrid);
	setaddresses(mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
static void loadaddresslist(RDArsrc *mainrsrc)
{
	short ef;
	char *vendorid=NULL,*vendorid2=NULL;
	char *addrid=NULL;
	char delflag=FALSE;

	if(ordadravl!=NULL) freeapplib(ordadravl);
	ordadravl=APPlibNEW();
	if(billadravl!=NULL) freeapplib(billadravl);
	billadravl=APPlibNEW();
	ZERNRD(venadrnum);
	FINDFLDGETSTRING(purmst,"VENDOR IDENTIFICATION",&vendorid);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
	ef=ADVGTENRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid)) break;
		FINDFLDGETCHAR(venadrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid!=NULL) Rfree(addrid);
			FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&addrid);
			addAPPlib(ordadravl,addrid);
			addAPPlib(billadravl,addrid);
		}
		ef=ADVNXTNRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc);
	}
	if(ordadravl->numlibs<1)
	addAPPlib(ordadravl,"No Address Defined");
	if(!FINDRSCLISTAPPlib(mainrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",0,
		ordadravl))
	{
		updatersrc(mainrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS");
	}
	if(billadravl->numlibs<1)
	addAPPlib(billadravl,"No Address Defined");
	if(!FINDRSCLISTAPPlib(mainrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",0,
		billadravl))
	{

		updatersrc(mainrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS");
	}
	if(addrid!=NULL) Rfree(addrid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(vendorid2!=NULL) Rfree(vendorid2);
	setaddresses(mainrsrc);
}
static void setsiteid(RDArsrc *mainrsrc)
{
	char *temp=NULL;
	int x=0;
	
	if(pursit!=(-1))
	{
		FINDFLDGETSTRING(purmst,"SITE IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(pursit,"SITE IDENTIFICATION",sitesavl,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mainrsrc,"SITE IDENTIFICATIONS",x);
		updatersrc(mainrsrc,"SITE IDENTIFICATIONS");
	}
	if(temp!=NULL) Rfree(temp);
}
static short selectfunction_venmsr(MakeBrowseList *blist)
{
	char active=FALSE;

	FINDFLDGETCHAR(vennum,"ACTIVE",&active);
	if(!active) return(FALSE);
		else return(TRUE);
}
static void doexit_venmsr(MakeBrowseList *blist)
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
static void okfunc_venmsr(MakeBrowseList *b,void *targetkey)
{
	char *vendorid=NULL;

	ZERNRD(vennum);
	if(ADVEQLNRDKEYsec(vennum,1,targetkey,SCRNvirtualSubData,b->funcargs))
	{
		KEYNRD(vennum,1);
	} else {
		if(vendorid!=NULL) Rfree(vendorid);
		FINDFLDGETSTRING(vennum,"VENDOR IDENTIFICATION",&vendorid);
	}
	filerecord2rsrc(vennum,b->funcargs);
	FINDRSCSETSTRING(b->funcargs,"[PURMST][VENDOR IDENTIFICATION]",vendorid); 
	FINDFLDSETSTRING(purmst,"VENDOR IDENTIFICATION",vendorid);
	if(venadrnum!=(-1))
	{
		ZERNRD(venadrnum);
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,b->funcargs)) KEYNRD(venadrnum,1);
	}
	computeallSCRNvirtuals(b->funcargs);
	updateallrsrc(b->funcargs);
	loadaddresslist(b->funcargs);
}
void browse_venmsr(RDArsrc *cporsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(cporsrc);
	if(mbl_venmsr==NULL)
	{
		mainrsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDERS VENDOR BROWSE");
		searchrsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDERS VENDOR SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"CREATE PURCHASE ORDERS VENDOR DEFINE LIST");
		addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum);
		addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_venmsr=ADVbrowse(vennum,1,mainrsrc,searchrsrc,definelist
			,NULL,selectfunction_venmsr,doexit_venmsr,okfunc_venmsr,NULL
			,cporsrc,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_venmsr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_venmsr(mbl_venmsr);
		}
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int which_type=0,x=0,nofields=0;
        NRDfield *fldx=NULL,*fields=NULL;
        short edit_rsrc=TRUE;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"CREATE PURCHASE ORDERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("REQCPO");
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	if((reqacc=APPOPNNRD(module,"REQACC",TRUE,FALSE))==(-1)) return;
	if((reqitm=APPOPNNRD(module,"REQITM",TRUE,TRUE))==(-1)) return;
	if((reqmst=APPOPNNRD(module,"REQMST",TRUE,FALSE))==(-1)) return;
	if((reqloc=APPOPNNRD(module,"REQLOC",TRUE,FALSE))==(-1)) return;
	if((catmst=APPOPNNRD("CATALOGUE","CATMST",TRUE,FALSE))==(-1)) return;
	if((purmst=APPOPNNRD("PURORD","PURMST",TRUE,TRUE))==(-1)) return;
	if((purlin=APPOPNNRD("PURORD","PURLIN",TRUE,TRUE))==(-1)) return;
	if((puracc=APPOPNNRD("PURORD","PURACC",TRUE,TRUE))==(-1)) return;
	if((purtrn=APPOPNNRD("PURORD","PURTRN",TRUE,TRUE))==(-1)) return;
	if((pursit=APPOPNNRD("PURORD","PURSIT",TRUE,FALSE))==(-1)) return;
	if((purids=APPOPNNRD("PURORD","PURIDS",TRUE,TRUE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((venadrnum=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((venadr2num=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((fsclyr=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if(XPERT_SETUP->FIXASS==TRUE)
	{
	if((fixmst=OPNNRDsec("FIXASS","FIXMST",TRUE,TRUE,FALSE))==(-1))
	{
		if((fixmst=OPNNRD("FIXASS","FIXMST"))>(-1))
		{
			ERRORDIALOG("Security Access Denied","Security Access denied to the [FIXMST] file or the file cannot otherwise be opened.\n",NULL,TRUE);
			ShutdownSecurity();
			RDAAPPMAINLOOP();
			return;
		} else {
			CLSNRD(fixmst);
			fixmst=(-1);
		}
	}
	} else fixmst=(-1);
	makeselecttype();
	mainrsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDERS");
	if(prev_purmst!=NULL) FreeRDATData(prev_purmst);
	prev_purmst=RDATDataNEW(purmst);
	addDFincvir(mainrsrc,module,"REQITM",NULL,reqitm);
	addDFincvir(mainrsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(mainrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(mainrsrc,"CATALOGUE","CATMST",NULL,catmst);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadrnum);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadr2num);
	addDFincvir(mainrsrc,"PURORD","PURMST",NULL,purmst);
	addDFincvir(mainrsrc,"PURORD","PURIDS",NULL,purids);
	addDFincvir(mainrsrc,"PURORD","PURSIT",NULL,pursit);
	addDFincvir(mainrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	file2rangersrc(reqitm,mainrsrc);
	file2rangersrc(reqmst,mainrsrc);
	file2rangersrc(reqloc,mainrsrc);
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,
		&selecttype,NULL);
	nofields=NUMFLDS(purmst);
	fields=FLDPOINTER(purmst);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"SITE IDENTIFICATION"))
			{
				sitesavl=ADVMakeNRDlistAllSTRING(pursit,module,"Creating Site List",1,"SITE IDENTIFICATION",SCRNvirtualSubData,mainrsrc);
				if(sitesavl->numlibs<1)
				{
					addAPPlib(sitesavl,"No Sites Defined");
				}
				if(FIELDscrn(purmst,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mainrsrc,"SITE IDENTIFICATIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						getsiteid,sitesavl->numlibs,
						&sitesavl->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"PO VENDOR ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(purmst,fldx->name,&edit_rsrc))
				{
					which_type=0;
					if(ordadravl!=NULL) freeapplib(ordadravl);
					ordadravl=APPlibNEW();
					addAPPlib(ordadravl,"No Address Defined");
					addlstrsrc(mainrsrc,
						"PO VENDOR ADDRESS IDENTIFICATIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						setaddressescb,ordadravl->numlibs,
						&ordadravl->libs,NULL);
                                }
			} else if(!RDAstrcmp(fldx->name,
				"BILLING VENDOR ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(purmst,fldx->name,&edit_rsrc))
				{
					which_type=0;
					if(billadravl!=NULL) freeapplib(billadravl);
					billadravl=APPlibNEW();
					addAPPlib(billadravl,"No Address Defined");
					addlstrsrc(mainrsrc,
						"BILLING VENDOR ADDRESS IDENTIFICATIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						setaddressescb,
						billadravl->numlibs,
						&billadravl->libs,NULL);
				} 
			} else if(!RDAstrcmp(fldx->name,"OUTSTANDING AMOUNT"))
			{
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!strncmp(fldx->name,"CLOSED",6))
			{
			} else if(!strncmp(fldx->name,"PRINTED",7))
			{
			} else {
				nonlstmakefld(mainrsrc,purmst,fldx,TRUE);
			}
		}
	}
	if(vennum!=(-1)) file2rsrc(vennum,mainrsrc,FALSE);
	if(venadrnum!=(-1)) file2rsrc(venadrnum,mainrsrc,FALSE);
	if(venadr2num!=(-1)) singlefile2rsrc(venadr2num,mainrsrc,FALSE);
	if(fsclyr!=(-1)) file2rsrc(fsclyr,mainrsrc,FALSE);
	FINDRSCSETFUNC(mainrsrc,"[PURMST][VENDOR IDENTIFICATION]",checkvendorcb,NULL);
	FINDRSCSETFUNC(mainrsrc,"[PURMST][FISCAL YEAR]",setpoidcb,NULL);
	FINDRSCSETFUNC(mainrsrc,"[PURMST][PO IDENTIFICATION]",setpoidcb,NULL);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	setpoid(mainrsrc,FALSE);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"BROWSE VENDORS",TRUE,browse_venmsr,NULL);
	addbtnrsrc(mainrsrc,"BROWSE",TRUE,browse_purmst,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SAVE",TRUE,savecheck_purmst,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,test_select,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void checkvendorcb(RDArsrc *mainrsrc)
{
	char *venid=NULL;

	readwidget(mainrsrc,"[PURMST][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PURMST][VENDOR IDENTIFICATION]",&venid);
	FINDFLDSETSTRING(purmst,"VENDOR IDENTIFICATION",venid);
	if(venid!=NULL) Rfree(venid);
	checkvendor(mainrsrc);
	loadaddresslist(mainrsrc);
	getsupportingpurmstfiles(mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
static short checkvendor(RDArsrc *mainrsrc)
{
	char delflag=FALSE,*venid=NULL,*warnmessage=NULL;

	if(vennum!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"[PURMST][VENDOR IDENTIFICATION]",&venid);
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",venid);
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc))
		{
			KEYNRD(vennum,1);
			warnmessage=Rmalloc(RDAstrlen(venid)+170);
			sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Vendor in the [VENPMT][VENMSR] data file.",(venid==NULL?"":venid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			prterr("The VENDOR IDENTIFICATION of [%s] entered does not exist in the [VENPMT][VENMSR] data file.",(venid==NULL?"":venid));
			if(venid!=NULL) Rfree(venid);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(vennum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(venid)+200);
				sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is deleted.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a deleted state.",(venid==NULL?"":venid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The VENDOR IDENTIFICATION of [%s] entered is deleted.  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a deleted state.",(venid==NULL?"":venid));
				if(venid!=NULL) Rfree(venid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(vennum,"ACTIVE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(venid)+200);
					sprintf(warnmessage,"The VENDOR IDENTIFICATION of [%s] entered is not Active.  It exists as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a non-Active state.",(venid==NULL?"":venid));
					WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE VENDOR IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The VENDOR IDENTIFICATION of [%s] entered is not Active.  It exist as a Vendor in the [VENPMT][VENMSR] data file but is presently set up in a non-Active state.",(venid==NULL?"":venid));
					if(venid!=NULL) Rfree(venid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return(FALSE);
				}
			}
		}
		if(venid!=NULL) Rfree(venid);
		if(warnmessage!=NULL) Rfree(warnmessage);
	}
	return(TRUE);
}
static void getsupportingpurmstfiles(RDArsrc *mainrsrc)
{
	if(vennum!=(-1))
	{
		COPYFIELD(purmst,vennum,"VENDOR IDENTIFICATION");
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(vennum,1);
		updatefilerecord2rsrc(vennum,mainrsrc);
	}
	if(venadrnum!=(-1))
	{
		COPYFIELD(purmst,venadrnum,"VENDOR IDENTIFICATION");
		FIELDCOPY(purmst,"PO VENDOR ADDRESS IDENTIFICATION",venadrnum,"ADDRESS IDENTIFICATION");
		if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(venadrnum,1);
		updatefilerecord2rsrc(venadrnum,mainrsrc);
	}
	if(venadr2num!=(-1))
	{
		COPYFIELD(purmst,venadr2num,"VENDOR IDENTIFICATION");
		FIELDCOPY(purmst,"BILLING VENDOR ADDRESS IDENTIFICATION",venadrnum,"ADDRESS IDENTIFICATION");
		if(ADVEQLNRDsec(venadr2num,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(venadr2num,1);
		updatesinglerecord2rsrc(venadr2num,mainrsrc);
	}
	if(pursit!=(-1))
	{
		COPYFIELD(purmst,pursit,"SITE IDENTIFICATION");
		if(ADVEQLNRDsec(pursit,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(pursit,1);
		updatefilerecord2rsrc(pursit,mainrsrc);
	}
	if(fsclyr!=(-1))
	{
		COPYFIELD(purmst,fsclyr,"FISCAL YEAR");
		if(ADVEQLNRDsec(fsclyr,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(fsclyr,1);
		updatefilerecord2rsrc(fsclyr,mainrsrc);
	}
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(reqloc!=(-1))
	{
		COPYFIELD(dbsort->fileno,reqloc,"LOCATION IDENTIFICATION");
		if(ADVEQLNRDsec(reqloc,1,ReqCPOSubData,p)) KEYNRD(reqloc,1);
		filerecord2rsrc(reqloc,mtnrsrc);
	}
	if(reqmst!=(-1))
	{
		COPYFIELD(dbsort->fileno,reqmst,"LOCATION IDENTIFICATION");
		COPYFIELD(dbsort->fileno,reqmst,"REQUISITION IDENTIFICATION");
		if(ADVEQLNRDsec(reqmst,1,ReqCPOSubData,p)) KEYNRD(reqmst,1);
		filerecord2rsrc(reqmst,mtnrsrc);
	}
	if(fsclyr!=(-1))
	{
		COPYFIELD(dbsort->fileno,fsclyr,"FISCAL YEAR");
		if(ADVEQLNRDsec(fsclyr,1,ReqCPOSubData,p)) KEYNRD(fsclyr,1);
		filerecord2rsrc(fsclyr,mtnrsrc);
	}
	if(vennum!=(-1))
	{
		COPYFIELD(dbsort->fileno,vennum,"VENDOR IDENTIFICATION");
		if(ADVEQLNRDsec(vennum,1,ReqCPOSubData,p)) KEYNRD(vennum,1);
		filerecord2rsrc(vennum,mtnrsrc);
	}
	if(p!=NULL) Rfree(p);
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *poid=NULL;
	int fiscalyr=0,lineno=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fiscalyr);
	FINDRSCGETSTRING(mtnrsrc,"PO IDENTIFICATION",&poid);
	FINDRSCGETINT(mtnrsrc,"LINE NUMBER",&lineno);
	ZERNRD(dbsort->fileno);
	FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",fiscalyr);
	FINDFLDSETSTRING(dbsort->fileno,"REQUISITION IDENTIFICATION",poid);
	FINDFLDSETINT(dbsort->fileno,"LINE NUMBER",lineno);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
		settype(mtnrsrc,dbsort);
	}
	getsupportingfiles(mtnrsrc,dbsort);
	if(poid!=NULL) Rfree(poid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void gettypes(RDArsrc *mtnrsrc)
{
	if(type!=NULL) freeapplib(type);
	type=APPlibNEW();
	addAPPlib(type,"Not Applicable");
	addAPPlib(type,"Fixed Assets");
	addAPPlib(type,"Inventory Control");
}
static void settype(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	int seltype=0;
	char *warnmessage=NULL;

	FINDFLDGETINT(dbsort->fileno,"TYPE",&seltype);
	if(seltype>=type->numlibs)
	{
		warnmessage=Rmalloc(4+131);
		sprintf(warnmessage,"There is an invalid value of [%4d] in the field TYPE being loaded in from the file.  Resetting TYPE to \"Not Applicable\"=[   0].",seltype);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		seltype=0;
	}
	FINDRSCSETINT(mtnrsrc,"TYPES",seltype);
}
void selectm(void *targetkey,DBsort *t)
{
	int x,which_type=0;
	short nofields=0;
	RDArsrc *mtnrsrc=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	NRDfield *fields=NULL,*fldx=NULL;

	mtnrsrc=RDArsrcNEW(module,"CREATE PURCHASE ORDER LINES MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(mtnrsrc,module,"REQMST",NULL,reqmst);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mtnrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"TYPE"))
			{
				gettypes(mtnrsrc);
				which_type=0;
				addlstrsrc(mtnrsrc,"TYPES",
					&which_type,
					TRUE,NULL,type->numlibs,
					&type->libs,NULL);
			} else if(!RDAstrcmp(fldx->name,"REFERENCE IDENTIFICATION"))
			{
				makefieldrsrc(mtnrsrc,fldx->name,
					fldx->type,fldx->len,TRUE);
			} else if(!RDAstrcmp(fldx->name,"COMMENTS"))
			{
				makefieldrsrc(mtnrsrc,fldx->name,
					fldx->type,fldx->len,TRUE);
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
				makefieldrsrc(mtnrsrc,fldx->name,
					fldx->type,fldx->len,TRUE);
			} else {
				makefieldrsrc(mtnrsrc,fldx->name,
					fldx->type,fldx->len,FALSE);
			}
		}
	}
	nofields=0;
	if(purmst!=(-1)) file2rsrc(purmst,mtnrsrc,FALSE);
	if(fsclyr!=(-1)) file2rsrc(fsclyr,mtnrsrc,FALSE);
	if(reqitm!=(-1)) file2rsrc(reqitm,mtnrsrc,FALSE);
	if(reqmst!=(-1)) file2rsrc(reqmst,mtnrsrc,FALSE);
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
		ZERNRD(t->fileno);
	} else {
		singlefilerecord2rsrc(t->fileno,mtnrsrc);
		settype(mtnrsrc,t);
	}
	getsupportingfiles(mtnrsrc,t);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quit_record,t,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_record(mtnrsrc,TRUE,dbsort);
}
static void save_record_nup(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_record(mtnrsrc,FALSE,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0,seltypes=0;
	RDArsrc *tmprsrc=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"TYPES",&seltypes);
	FINDFLDSETINT(dbsort->fileno,"TYPE",seltypes);
	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_purlin->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_purlin->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN","SECURITY","Browse Diagnostic",NULL);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE))
					ForceWindowUpdate(tmprsrc);
			}
			xmakebrowselist(mbl_purlin,tmprsrc);
			if(mbl_purlin->list->num<=valuex) valuex=0;
			mbl_purlin->list->keyvalue=mbl_purlin->list->key[valuex];
			if(!FINDRSCSETLIST(mbl_purlin->mainrsrc,"BROWSE LIST",valuex,
				mbl_purlin->list->num,&mbl_purlin->list->string))
			{
				updatersrc(mbl_purlin->mainrsrc,"BROWSE LIST");
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			ForceWindowUpdate(mtnrsrc);
		}
	}
	quit_record(mtnrsrc,dbsort);
}
static void quit_record(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(type!=NULL) freeapplib(type);
}
