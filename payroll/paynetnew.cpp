/* paynet.c - Compute Payroll */
#ifndef WIN32
#define __NAM__ "paynetnew.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paynetnew.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
#include <fin.hpp>
#include <pay.hpp>
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
	char *current_deduction;
};
typedef struct PassableStructs PassableStruct;

struct JobStructs
{
	int jobno;
	double gross;
	double amount;
	double affect_base;
	double base;
	double net;
	double dirdep;
};
typedef struct JobStructs JobStruct;
struct DeductionStructs
{
	char *dedid;
	int comp_level;
	double annual_base;
	double annual_affect_base;
	double base;
	double affect_base;
	double gross;
	double amount;
	double ratio;
	char *computation;
	char *rate_selection;
	char *gross_amt;
	char *affect_select;
	double amount_used;
	double gross_used;
	char reduce_net;
	int numjobs;
	JobStruct *jobs;
};
typedef struct DeductionStructs DeductionStruct;
struct ComputeStructs
{
	int num;
	DeductionStruct *d;
	int numjob;
	JobStruct *job;
};
typedef struct ComputeStructs ComputeStruct;

static char *module="PAYROLL";
static short payjmst=(-1),payjprm=(-1),payjpms=(-1),paymstr=(-1),prsnnl=(-1);
static short posmstr=(-1),payppms=(-1),pgrat=(-1),payjcsm=(-1),pgross=(-1);
static short payjfsm=(-1),posrate=(-1),payjrcs=(-1),payjrfs=(-1),payddms=(-1);
static short paydtyp=(-1),paydedm=(-1),paytyp=(-1);
static short paydcsm=(-1),paydfsm=(-1),paydpms=(-1),paypcsm=(-1);
static short paypfsm=(-1),payjdpm=(-1);
static short ExecuteSubData(char *,short (*)(...),char **,PassableStruct *,char *);
static RDAfinmgt *fmgt=NULL;
static APPlib *errorlist=NULL,*warnlist=NULL;
static RDApayroll *PAYROLL_SETUP=NULL;
static int DO_AUDIT=FALSE;
static char *audit_reportname=NULL,Use_New_Affect_Select=FALSE;
double ctotal_net=0;


#define SeeJobStruct(a)	xSeeJobStruct(a,__LINE__,__FILE__)
void xSeeJobStruct(JobStruct *j,int line,char *file)
{
	TRACE;
	if(j!=NULL)
	{
		prterr("DIAG SeeJobStruct line [%d] program [%s]:",line,file);
		TRACE;
		prterr("     Job Number: [%d]\n",j->jobno);
		TRACE;
		prterr("     Base:       [%f]\n",j->base);
		TRACE;
		prterr("     A Base:     [%f]\n",j->affect_base);
		TRACE;
		prterr("     Net:        [%f]\n",j->net);
		TRACE;
		prterr("     Gross:      [%f]\n",j->gross);
		prterr("     Amount:     [%f]\n",j->amount);
		TRACE;
	} else {
		prterr("DIAG SeeJobStruct is NULL at line [%d] program [%s].",line,file);
	}
}


#define SeeDeductionStruct(a)	xSeeDeductionStruct(a,__LINE__,__FILE__)
void xSeeDeductionStruct(DeductionStruct *d,int line,char *file)
{
	int x=0;
	JobStruct *job=NULL;

	TRACE;
	if(d!=NULL)
	{
		TRACE;
		prterr("DIAG SeeDeductionStruct for Deduction Identification [%s] at line [%d] program [%s]:.",d->dedid,line,file);
		prterr("Computation Level [%d] Annual Base [%f] Annual Affect Base [%f] Base [%f] Affect Base [%f] Gross [%.0f] Amount [%.0f] Ratio [%f] Reduce Net [%s].\n",d->comp_level,d->annual_base,d->annual_affect_base,d->base,d->affect_base,d->gross,d->amount,d->ratio,(d->reduce_net==TRUE?"TRUE":"FALSE"));
		if(d->jobs!=NULL)
		{
		prterr("numjobs [%d] ",d->numjobs);TRACE;
		for(x=0,job=d->jobs;x<d->numjobs;++x,++job)
		{
			SeeJobStruct(job);
		}
		}
	} else {
		prterr("DIAG SeeDeductionStruct is NULL at line [%d] program [%s].",line,file);
	}
}
void SetDJobBase(DeductionStruct *d,int jobno,double gross_amt)
{
	JobStruct *job=NULL;
	int x;

	if(d->jobs!=NULL)
	{
		for(x=0,job=d->jobs;x<d->numjobs;++x,++job)
		{
			if(job->jobno==jobno) break;
		}
		if(x<d->numjobs)
		{
			job->base+=gross_amt;
			job->affect_base+=gross_amt;
			job->net+=gross_amt;
		} else {
			d->jobs=Rrealloc(d->jobs,(d->numjobs+1)*sizeof(JobStruct));
			job=d->jobs+d->numjobs;
			job->jobno=jobno;
			job->base=gross_amt;
			job->affect_base=gross_amt;
			job->net=gross_amt;
			++d->numjobs;
		}
	} else {
		d->jobs=Rmalloc(sizeof(JobStruct));
		job=d->jobs;
		job->jobno=jobno;
		job->base=gross_amt;
		job->affect_base=gross_amt;
		job->net=gross_amt;
		d->numjobs=1;
	}
}
#define SeeComputeStruct(a)	xSeeComputeStruct(a,__LINE__,__FILE__)
void xSeeComputeStruct(ComputeStruct *c,int line,char *file)
{
	int x;
	DeductionStruct *d;

	if(c!=NULL)
	{
		prterr("DIAG SeeComputeStruct at line [%d] program [%s].",line,file);
		for(x=0,d=c->d;x<c->num;++x,++d)
		{
			SeeDeductionStruct(d);
		}
	} else {
		prterr("DIAG SeeComputeStruct is NULL at line [%d] program [%s].",line,file);
	}
}
static void ComputeSubData(char **,PassableStruct *);

static ComputeStruct *ComputeStructNew()
{
	ComputeStruct *tmp=NULL;

	tmp=Rmalloc(sizeof(ComputeStruct));
	tmp->num=0;
	tmp->d=NULL;
	tmp->numjob=0;
	tmp->job=NULL;
	return(tmp);
}
#ifdef DIDNT_NEED
static JobStruct *GetJobComputeStruct(ComputeStruct *c,int job)
{
	JobStruct *j=NULL;
	int x;

	if(c!=NULL)
	{
		if(c->job!=NULL)
		{
			for(x=0,j=c->job;x<c->numjob;++x,++j)
			{
				if(j->jobno==job) return(j);
			}
		}
	}
	return(j);
}
#endif /* DIDNT NEED */
static int AddJobComputeStruct(ComputeStruct *c,int job,double gross,
	double dedamt,double dirdep)
{
	JobStruct *j;
	int x;

	if(c!=NULL)
	{
	if(c->job!=NULL)
	{
		for(x=0,j=c->job;x<c->numjob;++x,++j)
		{
			if(j->jobno==job)
			{
				j->net+=(gross-dedamt);
				j->dirdep+=(dirdep);
				return(x+1);
			}
		}
		c->job=Rrealloc(c->job,(c->numjob+1)*sizeof(JobStruct));
		j=c->job+c->numjob;
		j->jobno=job;
		j->net+=(gross-dedamt);
		j->dirdep+=(dirdep);
		++c->numjob;
		return(c->numjob);
	} else {
		c->job=Rmalloc(sizeof(JobStruct));
		c->numjob=1;
		j=c->job;
		j->jobno=job;
		j->net+=(gross-dedamt);
		j->dirdep+=dirdep;
		return(c->numjob);
	}
	}
	return(-1);
}
static void FreeComputeStruct(ComputeStruct *c)
{
	DeductionStruct *d;
	int x;

	if(c!=NULL)
	{
		if(c->job!=NULL) Rfree(c->job);
		if(c->d!=NULL)
		{
			for(x=0,d=c->d;x<c->num;++x,++d)
			{
				if(d->jobs!=NULL) Rfree(d->jobs);
				if(d->dedid!=NULL) Rfree(d->dedid);
				if(d->computation!=NULL) Rfree(d->computation);
				if(d->rate_selection!=NULL) Rfree(d->rate_selection);
				if(d->gross_amt!=NULL) Rfree(d->gross_amt);
				if(d->affect_select!=NULL) Rfree(d->affect_select);
			}
			Rfree(c->d);
		}
		Rfree(c);
		c=NULL;
	}
}
static int addDeductionStruct(ComputeStruct *c,char *dedid,short comp_level,
	char *computation,char *rate_selection,char *gross_amt,
	char *affect_select,char reduce_net)
{
	DeductionStruct *d;

	if(c->d!=NULL)
	{
		c->d=Rrealloc(c->d,(c->num+1)*sizeof(DeductionStruct));
	} else {
		c->d=Rmalloc(sizeof(DeductionStruct));
		c->num=0;
	}
	d=c->d+c->num;
	if(!isEMPTY(dedid)) d->dedid=stralloc(dedid);
		else d->dedid=NULL;
	if(!isEMPTY(computation)) d->computation=stralloc(computation);
		else d->computation=NULL;
	if(!isEMPTY(rate_selection)) d->rate_selection=stralloc(rate_selection);
		else d->rate_selection=NULL;
	if(!isEMPTY(gross_amt)) d->gross_amt=stralloc(gross_amt);
		else d->gross_amt=NULL;
	if(!isEMPTY(affect_select)) d->affect_select=stralloc(affect_select);
		else d->affect_select=NULL;
	d->comp_level=comp_level;
	d->reduce_net=reduce_net;
	d->numjobs=0;
	d->jobs=NULL;
	d->amount_used=0;
	d->gross_used=0;
	d->base=0;
	d->affect_base=0;
	d->gross=0;
	d->amount=0;
	d->annual_affect_base=0;
	d->annual_base=0;
	d->ratio=0;
	++c->num;
	return(c->num);
}
static void SortComputeStruct(ComputeStruct *c)
{
	int x,y,comp_level;
	char reduce_net;
	DeductionStruct *d1,*d2;
	char *tmp;
	JobStruct *job;

	if(diagapps) SeeComputeStruct(c);
	for(x=0;x<c->num-1;++x) for(y=x+1;y<c->num;++y)
	{
		d1=c->d+x;
		d2=c->d+y;
		if(d2->comp_level<d1->comp_level)
		{
			job=d1->jobs;
			d1->jobs=d2->jobs;
			d2->jobs=job;
			comp_level=d1->comp_level;
			d1->comp_level=d2->comp_level;
			d2->comp_level=comp_level;
			reduce_net=d1->reduce_net;
			d1->reduce_net=d2->reduce_net;
			d2->reduce_net=reduce_net;
			tmp=d1->dedid;
			d1->dedid=d2->dedid;
			d2->dedid=tmp;
			tmp=d1->computation;
			d1->computation=d2->computation;
			d2->computation=tmp;
			tmp=d1->rate_selection;
			d1->rate_selection=d2->rate_selection;
			d2->rate_selection=tmp;
			tmp=d1->gross_amt;
			d1->gross_amt=d2->gross_amt;
			d2->gross_amt=tmp;
			tmp=d1->affect_select;
			d1->affect_select=d2->affect_select;
			d2->affect_select=tmp;
		}
	} 
	if(diagapps) SeeComputeStruct(c);
}
static ComputeStruct *GetPersonnelComputeStruct(PassableStruct *pass_struct,char *perid)
{
	ComputeStruct *tmp=NULL;
	short ef=0,comp_level=0;
	char *tperid=NULL,deleteflag=FALSE,active=FALSE,*dedid=NULL;
	char *computation=NULL,*rate_selection=NULL,*gross_amt=NULL;
	char reduce_net=FALSE;
	char *affect_select=NULL;

	tmp=ComputeStructNew();
	ZERNRD(paydedm);
	FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",perid);
	ef=ADVGTENRDsec(paydedm,1,ComputeSubData,pass_struct);
	while(!ef)
	{
		FINDFLDGETSTRING(paydedm,"PERSONNEL IDENTIFICATION",&tperid);
		if(RDAstrcmp(perid,tperid)) break;
		FINDFLDGETCHAR(paydedm,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(paydedm,"ACTIVE",&active);
		if(!deleteflag && active)
		{
			FINDFLDGETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			if(pass_struct->current_deduction!=NULL) Rfree(pass_struct->current_deduction);
			pass_struct->current_deduction=stralloc(dedid);
			FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",
				dedid);
			if(!ADVEQLNRDsec(payddms,1,ComputeSubData,pass_struct)) 
			{
				FINDFLDGETCHAR(payddms,"DELETEFLAG",&deleteflag);
				FINDFLDGETCHAR(payddms,"ACTIVE",&active);
				if(!deleteflag && active)
				{
					COPYFIELD(payddms,paydtyp,"DEDUCTION TYPE");
					if(!ADVEQLNRDsec(paydtyp,1,ComputeSubData,pass_struct)) 
					{
						FINDFLDGETCHAR(paydtyp,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
							FINDFLDGETSHORT(paydtyp,"COMPUTATION LEVEL",
								&comp_level);
							FINDFLDGETCHAR(payddms,"REDUCE NET",
								&reduce_net);
							FINDFLDGETSTRING(payddms,"COMPUTATION",
								&computation);
							FINDFLDGETSTRING(payddms,"RATE SELECTION",
								&rate_selection);
							FINDFLDGETSTRING(payddms,"GROSS AMOUNT",
								&gross_amt);
							FINDFLDGETSTRING(payddms,"AFFECT SELECT",
								&affect_select);
							addDeductionStruct(tmp,dedid,comp_level,
								computation,rate_selection,gross_amt,
								affect_select,reduce_net);
						}
					}
				}
			}
		}
		if(computation!=NULL) Rfree(computation);
		if(affect_select!=NULL) Rfree(affect_select);
		if(gross_amt!=NULL) Rfree(gross_amt);
		if(rate_selection!=NULL) Rfree(rate_selection);
		if(dedid!=NULL) Rfree(dedid);
		if(tperid!=NULL) Rfree(tperid);
		ef=ADVNXTNRDsec(paydedm,1,ComputeSubData,pass_struct);
	}
	if(computation!=NULL) Rfree(computation);
	if(affect_select!=NULL) Rfree(affect_select);
	if(gross_amt!=NULL) Rfree(gross_amt);
	if(rate_selection!=NULL) Rfree(rate_selection);
	if(dedid!=NULL) Rfree(dedid);
	if(tperid!=NULL) Rfree(tperid);
	SortComputeStruct(tmp);
	if(diagapps) SeeComputeStruct(tmp);
	return(tmp);
}
short FIELDSubData(char **tmp,PassableStruct *p,char *modulename)
{
	char *filename=NULL,*fieldname=NULL;
	char *temp=NULL,*gn=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	NRDfield *field=NULL;

	if(diageval)
	{
		prterr("DIAG FIELDSubData Substituting for [%.255s] (First 255 characters).",*tmp);
	}
	filename=stripfilename(*tmp);
	if(filename!=NULL)
	{
		fieldname=stripfieldname(*tmp);
		if(fieldname!=NULL)
		{
			memset(stemp,0,101);
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
			fileno=SCRNFILENUMBER(p->r,modulename,filename,occurrence);
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
							memset(*tmp,0,length+3);
						}
						if(!isEMPTY(temp) && str_length>length)
						{
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(length))
								temp[start-1+length]=0;
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else if(start<=str_length && !isEMPTY(temp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else {
							memset(*tmp,0,RDAstrlen(*tmp)+1);
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
							memset(*tmp,0,RDAstrlen(*tmp)+1);
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
							memset(*tmp,0,RDAstrlen(*tmp)+1);
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
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%c",*field->data.string_value);
						break;
					default:
						prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						if(fieldname!=NULL) Rfree(fieldname);
						if(filename!=NULL) Rfree(filename);
						return(-1);
				}
			} else {
				prterr("Error FIELDSubData Field [%.255s] (First 255 characters) not found.",*tmp);
				if(fieldname!=NULL) Rfree(fieldname);
				if(filename!=NULL) Rfree(filename);
				return(1);
			}
			if(fieldname!=NULL) Rfree(fieldname);
			if(filename!=NULL) Rfree(filename);
			return(0);	
		} else {
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
	} else {
		if(filename!=NULL) Rfree(filename);
		return(1);
	}
}
short STRUCTSubData(char **tmp,PassableStruct *p,char *modulename)
{
	char *filename=NULL;
	short str_length=0;
	DeductionStruct *d;
	ComputeStruct *c;
	int x;
	int retval=0;

	if(diageval)
	{
		prterr("DIAG STRUCTSubData Substituting for [%.255s] (First 255 characters).",*tmp);
	}
	filename=stripfilename(*tmp);
	if(!isEMPTY(filename))
	{
		c=(ComputeStruct *)p->a;
		if(c!=NULL)
		{
			if(c->d!=NULL)
			{
				memset(stemp,0,101);
				if(!RDAstrcmp(filename,"ANNUAL BASE"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						if(d->annual_base<0) 
						{
							sprintf(stemp,"(%f)",d->annual_base);
						} else sprintf(stemp,"%f",d->annual_base);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"ANNUAL AFFECT BASE"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						if(d->annual_base<0) 
						{
							sprintf(stemp,"(%f)",d->annual_affect_base);
						} else sprintf(stemp,"%f",d->annual_affect_base);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"CHECK GROSS"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->gross<0)
						{
							sprintf(stemp,"(%f)",d->gross);
						} else {
							sprintf(stemp,"%f",d->gross);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"AMOUNT"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->amount<0)
						{
							sprintf(stemp,"(%f)",d->amount);
						} else {
							sprintf(stemp,"%f",d->amount);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"BASE"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->base<0)
						{
							sprintf(stemp,"(%f)",d->base);
						} else {
							sprintf(stemp,"%f",d->base);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"AFFECT BASE"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->affect_base<0)
						{
							sprintf(stemp,"(%f)",d->affect_base);
						} else {
							sprintf(stemp,"%f",d->affect_base);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"COMPUTATION LEVEL"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%d",d->comp_level);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"RATIO"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%f",d->ratio);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else if(!RDAstrcmp(filename,"CHECK NET"))
				{
					for(x=0,d=c->d;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->dedid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(ctotal_net<0)
						{
							sprintf(stemp,"(%f)",ctotal_net);
						} else {
							sprintf(stemp,"%f",ctotal_net);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("0");
						if(diageval)
						{
							prterr("Error Deduction Description Identification [%s] not found, or %s isn't available for this employee.",modulename,*tmp);
						}
						retval=-2;
					}
				} else {
					if(filename!=NULL) Rfree(filename);
					return(1);
				}
			} else {
				if(filename!=NULL) Rfree(filename);
				return(1);
			}
		} else {
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
	} else {
		if(filename!=NULL) Rfree(filename);
		return(1);
	}
	if(filename!=NULL) Rfree(filename);
	return(retval);
}
short VIRTUALSubData(char **tmp,PassableStruct *p,char *modulename)
{
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn;
	int y;
	RDAvirtual *v;

	/* virtual field */
	if(diageval)
	{
		prterr("DIAG VIRTUALSubData Substituting for [%.255s] (First 255 characters).",*tmp);
	}
	for(y=0,v=p->r->virflds;y<p->r->num;++y,++v)
	{
		if(!RDAstrcmp(modulename,v->name)) break;
	}
	if(y<p->r->num)
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
					temp=PP_EVALUATEstr(v->expression,ComputeSubData,p);
					if(v->value.string_value!=NULL) 
						Rfree(v->value.string_value);
					v->value.string_value=temp;
					temp=NULL;
					if(v->value.string_value!=NULL)
					{
						v->dlen=RDAstrlen(v->value.string_value);
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
				if(length<1) length=v->dlen;
				temp=stralloc(v->value.string_value);
				if(RDAstrlen(*tmp)<(length+3))
				{
					*tmp=Rrealloc(*tmp,length+3);
				}
				memset(*tmp,0,RDAstrlen(*tmp)+1);
				if(!isEMPTY(temp) && v->dlen>length)
				{
					gn=temp+(start-1);
					if(RDAstrlen(gn)>(length))
						temp[start-1+length]=0;
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"%s\"",&temp[start-1]);
				} else if(start<v->dlen && !isEMPTY(temp))
				{
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"%s\"",&temp[start-1]);
				} else {
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					*v->value.short_value=PP_EVALUATEsht(v->expression,ComputeSubData,p);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					*v->value.integer_value=PP_EVALUATEint(v->expression,ComputeSubData,p);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					*v->value.float_value=PP_EVALUATEdbl(v->expression,ComputeSubData,p);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					*tmp=Rrealloc(*tmp,str_length+1);
				}
				memcpy(*tmp,stemp,str_length+1);
				break;
			case BOOLNS:
				if(!v->computed) 
				{
					v->computed=TRUE;
					*v->value.string_value=PP_EVALUATEbol(v->expression,ComputeSubData,p);
				}
				if(RDAstrlen(*tmp)<2)
				{
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					*tmp=Rrealloc(*tmp,2);
				}
				sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
				break;
			case CHARACTERS:
				if(!v->computed) 
				{
					v->computed=TRUE;
					*v->value.string_value=PP_EVALUATEbol(v->expression,ComputeSubData,p);
				}
				if(RDAstrlen(*tmp)<2)
				{
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					*tmp=Rrealloc(*tmp,2);
				}
				sprintf(*tmp,"%c",*v->value.string_value);
				break;
			default:
				prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
				return(-1);
		}
	} else {
		return(1);
	}
	return(0);
}
short PAYGLOBALSubData(char **tmp,PassableStruct *p,char *modulename)
{
	char *temp=NULL,*gn=NULL;
	short start=1,length=0,str_length=0;

	if(diageval)
	{
		prterr("DIAG PAYGLOBALSubData Substituting for [%.255s] (First 255 characters).",*tmp);
	}
	if(!RDAstrcmp(modulename,"CURRENT DEDUCTION IDENTIFICATION"))
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
		temp=stralloc(p->current_deduction);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
	} else if(!RDAstrcmp(modulename,"PAY BALANCE OF CONTRACT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(Pay_Balance_of_Contract ? TRUE:FALSE));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
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
		temp=GETCURRENTDATE();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			*tmp=Rrealloc(*tmp,length+3);
		} else memset(*tmp,0,RDAstrlen(*tmp)+1);
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
	} else {
		return(1);
	}
	return(0);
}
static short ExecuteSubData(char *name,short (*func)(...),char **tmp,PassableStruct *p,char *modulename)
{
	short retval=FALSE;

/*lint -e746*/
	if(func==ScrolledListSubData)
	{
		retval=func(tmp,p->r);
	} else {
		retval=func(tmp,p,modulename);
	}
/*lint +e746*/
	if(retval<1) 
	{
		if(retval==(-1))
		{
			prterr("Error Fatal Error from [%s] encountered substitution aborted .....",name);
		}
		if(diageval || diageval_field || diagapps)
		{
			prterr("DIAG ExecuteSubData Returning [%.255s] (First 255 characters).",*tmp);
		}
	}
	return(retval);
}
static void ComputeSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL;

	if(diageval || diageval_field || diagapps)
	{
		prterr("DIAG ComputeSubData Substituting for [%.255s] (First 255 characters).",*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
/* Returns 0 for found and no errors */
/* Return (-1) for Errors but quit looking */
/* Return (1) for not found and continue looking */	
		if(ExecuteSubData("FIELDSubData",FIELDSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData("STRUCTSubData",STRUCTSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData("VIRTUALSubData",VIRTUALSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData("PAYGLOBALSubData",PAYGLOBALSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(GLOBALSubData(tmp,modulename)==0)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData("ScrolledListSubData",ScrolledListSubData,tmp,p,NULL)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
	}
/* Error Message Here */
	prterr("Error ComputeSubData [%.255s] (First 255 characters) not found.",*tmp);
	if(*tmp!=NULL) Rfree(*tmp);
	*tmp=stralloc("\"\"");
	if(diageval || diageval_field || diagapps)
	{
		prterr("DIAG ComputeSubData Returning [%.255s] (First 255 characters).",*tmp);
	}
	if(modulename!=NULL) Rfree(modulename);
}
#define set_prsnnl_files_for_expression(a,b,f,g)	xset_prsnnl_files_for_expression(a,b,f,g,__LINE__,__FILE__)
void xset_prsnnl_files_for_expression(char *payid,char *perid,
	RDArsrc *rsrc,PassableStruct *pass_struct,int line,char *file)
{
	if(diagapps)
	{
		prterr("DIAG set_prsnnl_files_for_expression payid [%s] perid [%s] line [%d] program [%s].",payid,perid,line,file);
	}
	FINDFLDSETSTRING(prsnnl,"PERSONNEL IDENTIFICATION",perid);
	if(ADVEQLNRDsec(prsnnl,1,ComputeSubData,pass_struct)) KEYNRD(prsnnl,1);

	FINDFLDSETSTRING(paypcsm,"PERSONNEL IDENTIFICATION",perid);
	COPYFIELD(paymstr,paypcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(paypcsm,1,ComputeSubData,pass_struct)) KEYNRD(paypcsm,1);
	if(paytyp!=(-1))
	{
		COPYFIELD(paymstr,paytyp,"PAY TYPE");
		if(ADVEQLNRDsec(paytyp,1,ComputeSubData,pass_struct)) KEYNRD(paytyp,1);
	}
	FINDFLDSETSTRING(paypfsm,"PERSONNEL IDENTIFICATION",perid);
	COPYFIELD(paymstr,paypfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(paypfsm,1,ComputeSubData,pass_struct)) KEYNRD(paypfsm,1);
}
#define set_jobrate_files_for_expression(a,b,c,d,f,g)	xset_jobrate_files_for_expression(a,b,c,d,f,g,__LINE__,__FILE__)
void xset_jobrate_files_for_expression(char *payid,char *perid,int jobno,
	char *rateid,RDArsrc *rsrc,PassableStruct *pass_struct,
	int line,char *file)
{
	char *posid=NULL;

	if(diagapps)
	{
		prterr("DIAG set_jobrate_files_for_expression payid [%s] perid [%s] jobno [%d] rateid [%s] line [%d] program [%s].",payid,perid,jobno,rateid,line,file);
	}
	if(jobno!=(-1))
	{
		FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
		if(ADVEQLNRDsec(payjmst,1,ComputeSubData,pass_struct)) KEYNRD(payjmst,1);
		FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&posid);
		FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",posid);
		if(ADVEQLNRDsec(posmstr,1,ComputeSubData,pass_struct)) KEYNRD(posmstr,1);
		COPYFIELD(payjmst,pgross,"GROSS IDENTIFICATION");
		if(ADVEQLNRDsec(pgross,1,ComputeSubData,pass_struct)) KEYNRD(pgross,1);
		FINDFLDSETSTRING(payjcsm,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETINT(payjcsm,"JOB NUMBER",jobno);
		COPYFIELD(paymstr,payjcsm,"CALENDAR YEAR");
		if(ADVEQLNRDsec(payjcsm,1,ComputeSubData,pass_struct)) KEYNRD(payjcsm,1);
		FINDFLDSETSTRING(payjfsm,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETINT(payjfsm,"JOB NUMBER",jobno);
		COPYFIELD(paymstr,payjfsm,"FISCAL YEAR");
		if(ADVEQLNRDsec(payjfsm,1,ComputeSubData,pass_struct)) KEYNRD(payjfsm,1);
		FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETINT(payjpms,"JOB NUMBER",jobno);
		FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",payid);
		if(ADVEQLNRDsec(payjpms,1,ComputeSubData,pass_struct)) KEYNRD(payjpms,1);
		if(posid!=NULL) Rfree(posid);
		if(!isEMPTY(rateid) && jobno!=(-1))
		{
			FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETINT(payjrcs,"JOB NUMBER",jobno);
			FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",rateid);
			COPYFIELD(paymstr,payjrcs,"CALENDAR YEAR");
			if(ADVEQLNRDsec(payjrcs,1,ComputeSubData,pass_struct)) KEYNRD(payjrcs,1);
			FINDFLDSETSTRING(payjrfs,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETINT(payjrfs,"JOB NUMBER",jobno);
			FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",rateid);
			COPYFIELD(paymstr,payjrfs,"FISCAL YEAR");
			if(ADVEQLNRDsec(payjrfs,1,ComputeSubData,pass_struct)) KEYNRD(payjrfs,1);
			FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rateid);
			if(ADVEQLNRDsec(posrate,1,ComputeSubData,pass_struct)) KEYNRD(posrate,1);
			COPYFIELD(payjmst,pgrat,"GROSS IDENTIFICATION");
			FINDFLDSETSTRING(pgrat,"RATE IDENTIFICATION",rateid);
			if(ADVEQLNRDsec(pgrat,1,ComputeSubData,pass_struct)) KEYNRD(pgrat,1);
		}
	}
}
#define set_ded_files_for_expression(a,b,e,f,g)	xset_ded_files_for_expression(a,b,e,f,g,__LINE__,__FILE__)
void xset_ded_files_for_expression(char *payid,char *perid,
	char *dedid,RDArsrc *rsrc,PassableStruct *pass_struct,
	int line,char *file)
{
	if(diagapps)
	{
		prterr("DIAG set_ded_files_for_expression payid [%s] perid [%s] dedid [%s] line [%d] program [%s].",payid,perid,dedid,line,file);
	}
	CLEAR_SCREEN_VIRTUAL(rsrc);
	FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(ADVEQLNRDsec(payddms,1,ComputeSubData,pass_struct)) KEYNRD(payddms,1);
	COPYFIELD(payddms,paydtyp,"DEDUCTION TYPE");
	if(ADVEQLNRDsec(paydtyp,1,ComputeSubData,pass_struct)) KEYNRD(paydtyp,1);
	FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	if(ADVEQLNRDsec(paydedm,1,ComputeSubData,pass_struct)) KEYNRD(paydedm,1);
	FINDFLDSETSTRING(paydcsm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(paydcsm,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	COPYFIELD(paymstr,paydcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(paydcsm,1,ComputeSubData,pass_struct)) KEYNRD(paydcsm,1);
	FINDFLDSETSTRING(paydfsm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(paydfsm,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
	COPYFIELD(paymstr,paydfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(paydfsm,1,ComputeSubData,pass_struct)) KEYNRD(paydfsm,1);
}
static short selectbpaymstr(MakeBrowseList *blist)
{
	char delflag=FALSE;
	
	FINDFLDGETCHAR(paymstr,"DELETEFLAG",&delflag);
	return((short)!delflag);
}
static void quitbpaymstr(MakeBrowseList *blist)
{
	APPlib *args=NULL;
	char *payid=NULL,*temp=NULL;

	ZERNRD(paymstr);
	if(ADVEQLNRDKEYsec(paymstr,1,blist->list->keyvalue,SCRNvirtualSubData,blist->mainrsrc)) KEYNRD(paymstr,1);
	FINDFLDGETSTRING(paymstr,"PAY IDENTIFICATION",&payid);
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
	CLSNRD(payjmst);
	CLSNRD(paypcsm);
	CLSNRD(payjfsm);
	CLSNRD(payjdpm);
	CLSNRD(payjprm);
	CLSNRD(payppms);
	CLSNRD(payjpms);
	CLSNRD(paymstr);
	if(paytyp!=(-1)) CLSNRD(paytyp);
	CLSNRD(prsnnl);
	CLSNRD(payjrcs);
	CLSNRD(payjrfs);
	CLSNRD(posmstr);
	CLSNRD(pgross);
	CLSNRD(pgrat);
	CLSNRD(posrate);
	CLSNRD(payjcsm);
	CLSNRD(payddms);
	CLSNRD(paydtyp);
	CLSNRD(paydedm);
	CLSNRD(paydcsm);
	CLSNRD(paydfsm);
	CLSNRD(paydpms);
	CLSNRD(paypfsm);
	if(errorlist!=NULL) freeapplib(errorlist);
	if(warnlist!=NULL) freeapplib(warnlist);
	if(DO_AUDIT && PAYROLL_SETUP->paynet_audit)
	{
		if(!isEMPTY(payid))
		{
			temp=Rmalloc(RDAstrlen(payid)+20);
			sprintf(temp,"PAY_IDENTIFICATION=%s",payid);
			RDA_PutEnv(temp);
		}
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,audit_reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(payid!=NULL) Rfree(payid);
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(PAYROLL_SETUP!=NULL) free_payroll(PAYROLL_SETUP);
	if(audit_reportname!=NULL) Rfree(audit_reportname);
	ShutdownSubsystems();
}
static void okbpaymstr(MakeBrowseList *b,void *targetkey)
{
	RDArsrc *diagrsrc=NULL;
	short ef=FALSE,ef1=FALSE,payppms_keyno=1,payjprm_keyno=1,upd_diag=FALSE;
	char *payid=NULL,*tpayid=NULL,deleteflag=FALSE,benefits_computed=FALSE;
	char *perid=NULL,*tperid=NULL,*rateid=NULL,*exprssn=NULL;
	char *curdate=NULL,*curtime=NULL,suv=FALSE,*e=NULL;
	NRDfield *field=NULL;
	char OKval=FALSE,*lastrateid=NULL;
	ComputeStruct *CompStruct=NULL;
	DeductionStruct *d=NULL,*d1=NULL;
	double q,gross_amt=0,amt1=0,amt2=0,last_annual_base=0;
	double total_net=0;
	char *tmp=NULL;
	int x,y,jobno=0,z=0,w=0;
	JobStruct *job=NULL,*job1=NULL;
	PassableStruct *pass_struct=NULL;
	RDATData *prev_file=NULL;
	char *dedtype=NULL;


	ZERNRD(paymstr);
	if(ADVEQLNRDKEYsec(paymstr,1,targetkey,SCRNvirtualSubData,b->mainrsrc)) KEYNRD(paymstr,1);
	diagrsrc=diagscrn(payppms,"DIAGNOSTIC SCREEN",module,"Selecting Personnel Payroll Masters",stop_diagnostic);
	pass_struct=Rmalloc(sizeof(PassableStruct));
	pass_struct->r=diagrsrc;
	pass_struct->a=NULL;
	pass_struct->current_deduction=NULL;
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,module,"PAYJPRM",NULL,payjprm);
	addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(diagrsrc,"POSTRK","POSGRS",NULL,pgross);
	addDFincvir(diagrsrc,"POSTRK","POSGRAT",NULL,pgrat);
	addDFincvir(diagrsrc,"POSTRK","POSRATE",NULL,posrate);
	addDFincvir(diagrsrc,module,"PAYJCSM",NULL,payjcsm);
	addDFincvir(diagrsrc,module,"PAYJFSM",NULL,payjfsm);
	addDFincvir(diagrsrc,module,"PAYMSTR",NULL,paymstr);
	if(paytyp!=(-1))
	{
		addDFincvir(diagrsrc,module,"PAYTYP",NULL,paytyp);
	}
	addDFincvir(diagrsrc,module,"PAYJRCS",NULL,payjrcs);
	addDFincvir(diagrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(diagrsrc,module,"PAYDTYP",NULL,paydtyp);
	addDFincvir(diagrsrc,module,"PAYDEDM",NULL,paydedm);
	addDFincvir(diagrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(diagrsrc,module,"PAYDCSM",NULL,paydcsm);
	addDFincvir(diagrsrc,module,"PAYDFSM",NULL,paydfsm);
	addDFincvir(diagrsrc,module,"PAYPCSM",NULL,paypcsm);
	addDFincvir(diagrsrc,module,"PAYPFSM",NULL,paypfsm);
	addDFincvir(diagrsrc,module,"PAYDPMS",NULL,paydpms);
	pass_struct->r=diagrsrc;
	pass_struct->a=NULL;
	GET_ALL_SCREEN_VIRTUALS(diagrsrc,0);
	if(diagrsrc!=NULL)
	{
		if(!ADVmakescrn(diagrsrc,TRUE))
		{
			ForceWindowUpdate(diagrsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(diagrsrc->module)+RDAstrlen(diagrsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",diagrsrc->module,diagrsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	FINDFLDGETSTRING(paymstr,"PAY IDENTIFICATION",&payid);
	ZERNRD(payppms);
	payppms_keyno=KEYNUMBER(payppms,"RDA PAYPPMS KEY");
	if(payppms_keyno<1) payppms_keyno=1;
	FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(payppms,payppms_keyno,ComputeSubData,pass_struct);
	if(errorlist==NULL)
	{
		errorlist=APPlibNEW();
	} else {
		if(!RDAstrcmp(errorlist->libs[0],"No Errors Detected") && errorlist->numlibs==1)
		{
			freeapplib(errorlist);
			errorlist=APPlibNEW();
		}
	}
	if(warnlist==NULL)
	{
		warnlist=APPlibNEW();
	} else {
		if(!RDAstrcmp(warnlist->libs[0],"No Warnings Detected") && warnlist->numlibs==1)
		{
			freeapplib(warnlist);
			warnlist=APPlibNEW();
		}
	}
	while(!ef)
	{
		if(abort_diagnostic) break;
		upd_diag=FALSE;
		FINDFLDGETSTRING(payppms,"PAY IDENTIFICATION",&tpayid);
		if(RDAstrcmp(payid,tpayid))
		{
			total_net=0.0;
			ctotal_net=0.0;
			break;
		} 
		FINDFLDGETSTRING(payppms,"PERSONNEL IDENTIFICATION",&perid);
		FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		FINDFLDGETCHAR(payppms,"BENEFITS COMPUTED",&benefits_computed);
		if(!benefits_computed)
		{
/* Get Personnel's Computation Structure */
			CompStruct=GetPersonnelComputeStruct(pass_struct,perid);
/* Get PErsonnel's Gross Earnings (base) */
			if(diagapps) SeeComputeStruct(CompStruct);
			pass_struct->a=CompStruct;
			set_prsnnl_files_for_expression(payid,perid,diagrsrc,pass_struct);
			ZERNRD(payjprm);
			FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
			FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",perid);
			payjprm_keyno=KEYNUMBER(payjprm,"RDA PAYJPRM KEY");
			if(payjprm_keyno<1)
			{
				prterr("DIAG missing RDA PAYJPRM KEY from the [PAYROLL][PAYJPRM] data file."); 
/*
				payjprm_keyno=1;
*/
			}
			ef1=ADVGTENRDsec(payjprm,payjprm_keyno,ComputeSubData,pass_struct);
			while(!ef1)
			{
				FINDFLDGETSTRING(payjprm,"PAY IDENTIFICATION",&tpayid);
				FINDFLDGETSTRING(payjprm,"PERSONNEL IDENTIFICATION",&tperid);
				if(RDAstrcmp(payid,tpayid) || RDAstrcmp(perid,tperid))
				{
					last_annual_base=0.0;
					if(lastrateid!=NULL) Rfree(lastrateid);
					break;
				}
				FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",tperid);
				FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",tpayid);
				FINDFLDGETINT(payjprm,"JOB NUMBER",&jobno);
				FINDFLDSETINT(payjpms,"JOB NUMBER",jobno);
				if(ADVEQLNRDsec(payjpms,1,ComputeSubData,pass_struct)) KEYNRD(payjpms,1);
				FINDFLDGETCHAR(payjprm,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				FINDFLDGETDOUBLE(payjprm,"GROSS",&gross_amt);
				if(gross_amt!=0.0)
				{
				total_net+=gross_amt;
				FINDFLDGETCHAR(payjpms,"SOURCE USER VERIFIED",&suv);
				if(suv)
				{
/*
					FINDFLDGETINT(payjprm,"JOB NUMBER",&jobno);
*/
					FINDFLDGETSTRING(payjprm,"RATE IDENTIFICATION",&rateid);
					AddJobComputeStruct(CompStruct,jobno,gross_amt,0.0,0.0);
					set_jobrate_files_for_expression(payid,perid,jobno,rateid,diagrsrc,pass_struct);
					for(x=0,d=CompStruct->d;x<CompStruct->num;++x,++d)
					{
/* set files for expression */
						if(pass_struct->current_deduction!=NULL) Rfree(pass_struct->current_deduction);
						pass_struct->current_deduction=stralloc(d->dedid);
						set_ded_files_for_expression(payid,perid,d->dedid,diagrsrc,pass_struct);
						if(!isEMPTY(d->rate_selection))
						{
							if(diageval || diageval_field)
							{
								prterr("DIAG EVALUATEbol Rate Selection.");
							}
							OKval=EVALUATEbol(d->rate_selection,ComputeSubData,pass_struct); 
							if(OKval) 
							{
								d->base+=gross_amt;
								SetDJobBase(d,jobno,gross_amt);
								FINDFLDGETSTRING(posrate,"ANNUAL BASE",&exprssn);

								if(!isEMPTY(exprssn))
								{
									if(diageval || diageval_field)
									{
										prterr("DIAG EVALUATEdbl Annual Base.");
									}
									last_annual_base=EVALUATEdbl(exprssn,
										ComputeSubData,pass_struct);
									d->annual_base+=last_annual_base;
								} else d->annual_base+=gross_amt;
								if(exprssn!=NULL) Rfree(exprssn);
							}
						} else {
							d->base+=gross_amt;
							SetDJobBase(d,jobno,gross_amt);
							FINDFLDGETSTRING(posrate,"ANNUAL BASE",&exprssn);
							if(!isEMPTY(exprssn))
							{
								if(diageval || diageval_field)
								{
									prterr("DIAG EVALUATEdbl Annual Base.");
								}
								last_annual_base=EVALUATEdbl(exprssn,
									ComputeSubData,pass_struct);
								d->annual_base+=last_annual_base;
								Rfree(exprssn);
							} else d->annual_base+=gross_amt; 
						}
						if(lastrateid!=NULL) Rfree(lastrateid);
						lastrateid=stralloc(rateid);
					}
					if(lastrateid!=NULL) Rfree(lastrateid);
					last_annual_base=0.0;
				} else {
					if(diagapps)
					{
						prterr("DIAG Skipped PAYJPRM (Payroll Job Pay Rate Master) because of SOURCE USER VERIFED=FALSE in payid=[%s]  perid=[%s]  jobno=[%d]  rateid=[%s].",payid,perid,jobno,rateid);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("DIAG Skipped PAYJPRM (Payroll Job Pay Rate Master) because of GROSS AMOUNT=0.00 in payid=[%s]  perid=[%s]  jobno=[%d]  rateid=[%s].",payid,perid,jobno,rateid);
					}
				}
				} else {
					if(diagapps)
					{
						prterr("DIAG Skipped PAYJPRM (Payroll Job Pay Rate Master) because of DELETEFLAG=TRUE in payid=[%s]  perid=[%s]  jobno=[%d]  rateid=[%s].",payid,perid,jobno,rateid);
					}
				}
				ef1=ADVNXTNRDsec(payjprm,payjprm_keyno,ComputeSubData,pass_struct);
			}
			if(lastrateid!=NULL) Rfree(lastrateid);
			if(rateid!=NULL) Rfree(rateid);
			for(x=0,d=CompStruct->d;x<CompStruct->num;++x,++d)
			{
				d->affect_base=d->base;
				d->annual_affect_base=d->annual_base;
			}
/* Compute DeductionStruct's */
			if(diagapps) SeeComputeStruct(CompStruct);
			deleteflag=FALSE;
			if((CompStruct->num)<1)
			{        
				tmp=Rmalloc(RDAstrlen(perid)+100+21);
				sprintf(tmp,"NO DEDUCTION DESCRIPTIONS PROCESSED: Personnel ID [%-30s]",(perid==NULL?"":perid));
				addAPPlib(warnlist,tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
			for(x=0,d=CompStruct->d;x<CompStruct->num;++x,++d)
			{
				if(pass_struct->current_deduction!=NULL) Rfree(pass_struct->current_deduction);
				pass_struct->current_deduction=stralloc(d->dedid);
				if(diageval || diageval_field)
				{
					prterr("DIAG Compute Struct Deduction [%d] [%s] Compute Level [%d] .",x,d->dedid,d->comp_level);
				}
				if(d->jobs==NULL)
				{
					if(diagapps)
					{
						prterr("DIAG Skipping calculation of the Deduction Description Identification: [%s] because of (d->jobs==NULL) and moving on!",d->dedid);
					}
				} else {
/* set files for expression */
					set_ded_files_for_expression(payid,perid,d->dedid,diagrsrc,pass_struct);
/* Write PAYDPMS records */
					if(d->base!=0.0)
					{
						d->annual_affect_base=(d->annual_base/d->base)*d->affect_base;
					} else {
						d->annual_affect_base=0;
					}
					if(d->annual_affect_base!=0.0)
					{
						d->ratio=d->affect_base/d->annual_affect_base;
					} else d->ratio=0;
/* Moved below statements to test the standard computations - RADDOG */
					if(d->jobs!=NULL)
					{
						ctotal_net=0;
						for(y=0,job=d->jobs;y<d->numjobs;++y,++job)
						{
							ctotal_net+=job->net;
						}
					}
/* Moved above statements to test the standard computations - RADDOG */
					d->gross=round(d->affect_base);
					if(!isEMPTY(d->gross_amt))
					{
						if(diageval || diageval_field)
						{
							prterr("DIAG EVALUATEdbl Gross Amount.");
						}
						d->gross=EVALUATEdbl(d->gross_amt,ComputeSubData,pass_struct);
						d->gross=round(d->gross);
					}
					if(!isEMPTY(d->computation))
					{
						if(diageval || diageval_field)
						{
							prterr("DIAG EVALUATEdbl Computation.");
						}
						d->amount=EVALUATEdbl(d->computation,ComputeSubData,pass_struct);
						d->amount=round(d->amount);
					} else d->amount=0;
					if(diagapps || diageval || diageval_field) {SeeDeductionStruct(d);TRACE;}
					if(d->jobs!=NULL)
					{
						ZERNRD(payjdpm);
						FINDFLDSETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",perid);
						FINDFLDSETSTRING(payjdpm,"DEDUCTION DESCRIPTION IDENTIFICATION",d->dedid);
						FINDFLDSETSTRING(payjdpm,"PAY IDENTIFICATION",payid);
						d->amount_used=0;
						d->gross_used=0;
/* Moved these commented lines up */
/*
						ctotal_net=0;
						for(y=0,job=d->jobs;y<d->numjobs;++y,++job)
						{
							ctotal_net+=job->net;
						}
*/
						for(y=0,job=d->jobs;y<d->numjobs;++y,++job)
						{
							FINDFLDSETINT(payjdpm,"JOB NUMBER",job->jobno);
							if(!ADVEQLNRDsec(payjdpm,1,ComputeSubData,pass_struct)) 
							{
								prev_file=RDATDataNEW(payjdpm);
								FINDFLDGETCHAR(payjdpm,"DELETEFLAG",&deleteflag);
								if(deleteflag)
								{
									KEYNRD(payjdpm,1);
								}
							} else { 
								prev_file=NULL;
							}
							deleteflag=FALSE;
							FINDFLDSETCHAR(payjdpm,"DELETEFLAG",deleteflag);
							FINDFLDSETCHAR(payjdpm,"UPDATED",deleteflag);
							FINDFLDSETSTRING(payjdpm,"SOURCE USER",USERLOGIN);
							if((y+1)<d->numjobs)
							{
								if(d->base!=0)
								{
/* SMYTH CHANGE TO TEST */
									amt1=round((d->gross*job->net)/ctotal_net);
									amt2=round((d->amount*job->net)/ctotal_net);
								} else { 
									amt1=0;
									amt2=0;
								}
							} else {

								amt1=d->gross-d->gross_used;
								amt2=d->amount-d->amount_used;
							}
							FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&dedtype);
							if(!strncmp(dedtype,"DIRECT DEP",10))
							{
								AddJobComputeStruct(CompStruct,job->jobno,0.0,(d->reduce_net ? amt2:0.0),amt2);
							} else {
								AddJobComputeStruct(CompStruct,job->jobno,0.0,(d->reduce_net ? amt2:0.0),0.0);
							}
							job->amount=amt2;
							job->gross=amt1;
							d->amount_used+=amt2;
							d->gross_used+=amt1;
							FINDFLDSETDOUBLE(payjdpm,"GROSS",amt1);
							FINDFLDSETDOUBLE(payjdpm,"AMOUNT",amt2);
							if(amt1!=0.0 || amt2!=0.0)
							{
								ADVWRTTRANSsec(payjdpm,0,NULL,prev_file,ComputeSubData,pass_struct);
							}
/* SMYTH */
							if(amt2!=0.0 && d->reduce_net==TRUE) 
							{
								total_net=total_net-amt2;
								for(w=(x+1);w<CompStruct->num;++w)
								{
									d1=CompStruct->d+w;
									for(z=0,job1=d1->jobs;z<d1->numjobs;++z,++job1)
									{
										if(job->jobno==job1->jobno) 
										{
											job1->net=job1->net-amt2;
											if(job1->net<0)
											{
												prterr("Error Job [%d] Net of [%f] for Deduction [%s] reduced by deduction [%s] of amount [%f] ",job1->jobno,job1->net,d1->dedid,d->dedid,amt2);TRACE;
											}
											break;
										}

									}
							
								}
							}
							if(prev_file!=NULL) FreeRDATData(prev_file);
						}
					}
					ZERNRD(paydpms);
					FINDFLDSETSTRING(paydpms,"PERSONNEL IDENTIFICATION",perid);
					FINDFLDSETSTRING(paydpms,"DEDUCTION DESCRIPTION IDENTIFICATION",d->dedid);
					FINDFLDSETSTRING(paydpms,"PAY IDENTIFICATION",payid);
					if(!ADVEQLNRDsec(paydpms,1,ComputeSubData,pass_struct))
					{
						prev_file=RDATDataNEW(paydpms); 
						FINDFLDGETCHAR(paydpms,"DELETEFLAG",&deleteflag);
						if(deleteflag)
						{
							KEYNRD(paydpms,1);
						}
					} else {
						prev_file=NULL;
					}
					deleteflag=FALSE;
					FINDFLDSETCHAR(paydpms,"DELETEFLAG",deleteflag);
#ifdef SJSTEST
					d->gross=round(d->gross);
					d->amount=round(d->amount);
#endif
					FINDFLDSETDOUBLE(paydpms,"GROSS",d->gross);
					FINDFLDSETDOUBLE(paydpms,"AMOUNT",d->amount);
					FINDFLDSETCHAR(paydpms,"UPDATED",deleteflag);
					FINDFLDSETSTRING(paydpms,"SOURCE USER",USERLOGIN);
					if(PAYROLL_SETUP->paynet_audit)
					{
						FINDFLDSETCHAR(paydpms,"SOURCE USER VERIFIED",FALSE);
					} else {
						FINDFLDSETCHAR(paydpms,"SOURCE USER VERIFIED",TRUE);
					}
					if(d->amount!=0.0 || d->gross!=0.0)
					{
						ADVWRTTRANSsec(paydpms,0,NULL,prev_file,ComputeSubData,pass_struct);
						DO_AUDIT=TRUE;
					}
					if(prev_file!=NULL) FreeRDATData(prev_file);
					if(!isEMPTY(d->affect_select))
					{
						for(y=x;y<CompStruct->num;++y)
						{
							d1=CompStruct->d+y;
							if(Use_New_Affect_Select==FALSE)
							{
								set_ded_files_for_expression(payid,perid,d1->dedid,diagrsrc,pass_struct);
							} else {
								CLEAR_SCREEN_VIRTUAL(diagrsrc);
								FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",d1->dedid);
								if(ADVEQLNRDsec(payddms,1,ComputeSubData,pass_struct)) KEYNRD(payddms,1);
							}
							if(diageval || diageval_field)
							{
								prterr("DIAG EVALUATEdbl Affect Select.");
							}
							q=EVALUATEdbl(d->affect_select,
								ComputeSubData,pass_struct);
							d1->affect_base=d1->affect_base-q;
							if(q!=0.0)
							{
								for(z=0,job1=d1->jobs;z<d1->numjobs;++z,++job1)
								{
									job1->affect_base=(job1->affect_base-((q*job1->base/d1->base)));
								}
							}
						}
					}
					if(d->amount!=d->amount_used ||
						d->gross!=d->gross_used)
					{
						prterr("ERROR AMOUNT!=AMOUNT USED and/or GROSS!=GROSS USED ");TRACE;
					} 
				}
			}
			if(total_net<0.00)
			{        
				tmp=Rmalloc(RDAstrlen(perid)+100+21);
				sprintf(tmp,"INVALID NET AMOUNT: Personnel ID [%-30s] Net Amount [%20.02f]",(perid==NULL?"":perid),total_net/100);
				addAPPlib(errorlist,tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
			FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid);
			if(!ADVEQLNRDsec(payppms,1,ComputeSubData,pass_struct))
			{
				if(CompStruct!=NULL)
				{
				if(CompStruct->job!=NULL)
				{
				prev_file=RDATDataNEW(payppms);
				FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
				if(deleteflag)
				{
					KEYNRD(payppms,1);
				}
				benefits_computed=TRUE;
				deleteflag=FALSE;
				FINDFLDSETCHAR(payppms,"DELETEFLAG",deleteflag);
				FINDFLDSETCHAR(payppms,"BENEFITS COMPUTED",benefits_computed);
				field=FLDNRD(payppms,"BENEFITS COMPUTED DATE");
				if(field->len==8)
				{
					curdate=GETCURRENTDATE();
					FINDFLDSETSTRING(payppms,"BENEFITS COMPUTED DATE",curdate);
				} else {
					curdate=GETCURRENTDATE10();
					FINDFLDSETSTRING(payppms,"BENEFITS COMPUTED DATE",curdate);
				}
				if(curdate!=NULL) Rfree(curdate);
				curtime=GETCURRENTTIME();
				FINDFLDSETSTRING(payppms,"BENEFITS COMPUTED TIME",curtime);
				if(curtime!=NULL) Rfree(curtime);
				FINDFLDSETSTRING(payppms,"BENEFITS COMPUTED BY",USERLOGIN);
				ADVWRTTRANSsec(payppms,0,NULL,prev_file,ComputeSubData,pass_struct);
				if(prev_file!=NULL) FreeRDATData(prev_file);
				}
				}
			}
			if(CompStruct!=NULL)
			{
				field=FLDNRD(payjpms,"CHECK NET");
				if(CompStruct->job!=NULL && field!=NULL)
				{
					for(z=0,job1=CompStruct->job;z<CompStruct->numjob;++z,++job1)
					{
						ZERNRD(payjpms);
						FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",perid);
						FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",payid);
						FINDFLDSETINT(payjpms,"JOB NUMBER",job1->jobno);
						if(!ADVEQLNRDsec(payjpms,1,ComputeSubData,pass_struct))
						{
						prev_file=RDATDataNEW(payjpms);
						FINDFLDSETDOUBLE(payjpms,"CHECK NET",job1->net);
						FINDFLDSETDOUBLE(payjpms,"DIRECT DEPOSIT NET",job1->dirdep);							
						ADVWRTTRANSsec(payjpms,0,NULL,prev_file,ComputeSubData,pass_struct);
						if(prev_file!=NULL) FreeRDATData(prev_file);
						}
					}
				}
			}
#ifdef SJSTEST 
			SeeComputeStruct(CompStruct);
#else
			if(diagapps) SeeComputeStruct(CompStruct);
#endif
			FreeComputeStruct(CompStruct);
			CompStruct=NULL;
			upd_diag=TRUE;
		} else {
			if(diagapps)
			{
				prterr("DIAG Skipped PAYPPMS (Payroll Personnel Pay Master) because of BENEFITS COMPUTED=TRUE in payid=[%s]  perid=[%s].",payid,perid);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("DIAG Skipped PAYPPMS (Payroll Personnel Pay Master) because of DELETEFLAG=TRUE in payid=[%s]  perid=[%s].",payid,perid);
			}
		}
		if(diagrsrc!=NULL) update_diagnostic(diagrsrc,(upd_diag ? TRUE:FALSE));
		ef=ADVNXTNRDsec(payppms,payppms_keyno,ComputeSubData,pass_struct);
		total_net=0.0;
		ctotal_net=0.0;
	}
	if(diagrsrc!=NULL)
	{
		if(!abort_diagnostic) killwindow(diagrsrc);
		abort_diagnostic=FALSE;
		free_rsrc(diagrsrc);
	}
	if(perid!=NULL) Rfree(perid);
	if(tperid!=NULL) Rfree(tperid);
	if(payid!=NULL) Rfree(payid);
	if(tpayid!=NULL) Rfree(tpayid);
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
        FINDRSCLISTAPPlib(b->mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(b->mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
        FINDRSCLISTAPPlib(b->mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(b->mainrsrc,"WARNING LIST");
	if(pass_struct!=NULL) Rfree(pass_struct);
}
static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void printerrorlist(RDArsrc *parent,FILE *fp)
{
	int x;
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		fprintf(fp,"%*sXPERT COMPUTE PAYROLL ERROR LIST          Date: %*s\n",24,"",8,date);
		fprintf(fp,"%*sExecuted By: %s%*sTime: %s\n\n",
			((80-(15+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(15+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
		for(x=0;x<errorlist->numlibs;++x)
		{
			fprintf(fp,"(%5d) %s\n",(x+1),errorlist->libs[x]);
		}
		fprintf(fp,"\f");
	}
}
void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL;
	FILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
			printerrorlist(parent,fp);
			pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}
static void printwarnlist(RDArsrc *parent,FILE *fp)
{
	int x;
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		fprintf(fp,"%*sXPERT COMPUTE PAYROLL WARNING LIST        Date: %*s\n",22,"",8,date);
		fprintf(fp,"%*sExecuted By: %s%*sTime: %s\n\n",
			((80-(15+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(15+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
		for(x=0;x<warnlist->numlibs;++x)
		{
			fprintf(fp,"(%5d) %s\n",(x+1),warnlist->libs[x]);
		}
		fprintf(fp,"\f");
	}
}
void printwarnlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL;
	FILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
			printwarnlist(parent,fp);
			pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}
static void print_list(RDArsrc *parent,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	if(diaggui)
	{
		prterr("DIAG print error/warnlist Displaying the Print Screen in preparation to print Error or Warning List.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,parent);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print(prsrc);
	}
}
int main(int argc,char **argv)
{
	RDArsrc *mrsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	int start=0;
	RDAGenericSetup *GSV=NULL;
	char *libx=NULL;

	if(InitializeSubsystems(argc,argv,module,"COMPUTE PAYROLL")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		audit_reportname=stralloc(argv[1]);
	} else audit_reportname=stralloc("PAYNETA");
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	GSV=RDAGenericSetupNew(module,"USE NEW AFFECT SELECT");
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif
	if(getRDAGenericSetupbin(libx,GSV))
	{
		Use_New_Affect_Select=FALSE;
	} else {
		Use_New_Affect_Select=*GSV->value.string_value;
	}
	FreeRDAGenericSetup(GSV);	
	PAYROLL_SETUP=RDApayrollNEW();
	getpayrollbin(PAYROLL_SETUP);
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((payjprm=APPOPNNRD(module,"PAYJPRM",TRUE,FALSE))==(-1)) return;
	if((payppms=APPOPNNRD(module,"PAYPPMS",TRUE,TRUE))==(-1)) return;
	if((payjpms=APPOPNNRD(module,"PAYJPMS",TRUE,TRUE))==(-1)) return;
	if((paymstr=APPOPNNRD(module,"PAYMSTR",TRUE,FALSE))==(-1)) return;
	paytyp=APPOPNNRD(module,"PAYTYP",TRUE,FALSE);
	if((payjrcs=APPOPNNRD(module,"PAYJRCS",TRUE,FALSE))==(-1)) return;
	if((payjrfs=APPOPNNRD(module,"PAYJRFS",TRUE,FALSE))==(-1)) return;
	if((prsnnl=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((pgross=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((pgrat=APPOPNNRD("POSTRK","POSGRAT",TRUE,FALSE))==(-1)) return;
	if((posrate=APPOPNNRD("POSTRK","POSRATE",TRUE,FALSE))==(-1)) return;
	if((payjcsm=APPOPNNRD(module,"PAYJCSM",TRUE,FALSE))==(-1)) return;
	if((payjfsm=APPOPNNRD(module,"PAYJFSM",TRUE,FALSE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((paydtyp=APPOPNNRD(module,"PAYDTYP",TRUE,FALSE))==(-1)) return;
	if((paydedm=APPOPNNRD(module,"PAYDEDM",TRUE,FALSE))==(-1)) return;
	if((paydcsm=APPOPNNRD(module,"PAYDCSM",TRUE,FALSE))==(-1)) return;
	if((paydfsm=APPOPNNRD(module,"PAYDFSM",TRUE,FALSE))==(-1)) return;
	if((paydpms=APPOPNNRD(module,"PAYDPMS",TRUE,TRUE))==(-1)) return;
	if((paypcsm=APPOPNNRD(module,"PAYPCSM",TRUE,FALSE))==(-1)) return;
	if((paypfsm=APPOPNNRD(module,"PAYPFSM",TRUE,FALSE))==(-1)) return;
	if((payjdpm=APPOPNNRD(module,"PAYJDPM",TRUE,TRUE))==(-1)) return;
	mrsrc=RDArsrcNEW(module,"COMPUTE PAYROLL BROWSE");
	srsrc=RDArsrcNEW(module,"COMPUTE PAYROLL SEARCH BROWSE");
	drsrc=RDArsrcNEW(module,"COMPUTE PAYROLL DEFINE LIST");
	addDFincvir(drsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(srsrc,module,"PAYMSTR",NULL,paymstr);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	errorlist=APPlibNEW();
	addAPPlib(errorlist,"No Errors Detected");
	addlstrsrc(mrsrc,"ERROR LIST",&start,TRUE,NULL,errorlist->numlibs,&errorlist->libs,NULL);
/*lint -e611 */
	addrfcbrsrc(mrsrc,"PRINT ERROR LIST",TRUE,print_list,(void *)printerrorlistcb);
	warnlist=APPlibNEW();
	addAPPlib(warnlist,"No Warnings Detected");
	addlstrsrc(mrsrc,"WARNING LIST",&start,TRUE,NULL,warnlist->numlibs,&warnlist->libs,NULL);
	addrfcbrsrc(mrsrc,"PRINT WARNING LIST",TRUE,print_list,(void *)printwarnlistcb);
	ADVSbrowse(paymstr,1,mrsrc,srsrc,drsrc,NULL,NULL,selectbpaymstr,quitbpaymstr,
		okbpaymstr,NULL,NULL,TRUE,NULL,0,0,2,FALSE);
	RDAAPPMAINLOOP();
}
