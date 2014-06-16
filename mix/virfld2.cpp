#include<mix.hpp>
#include<fin.hpp>
#include<pay.hpp>
#include<appmgt.hpp>
#include<venpmt.hpp>
#include<purord.hpp>
#ifndef WIN32
#include<uuid/uuid.h>
#endif 

char Pay_Balance_of_Contract=FALSE,Dump_Deferred_Pay_Balance=FALSE;

char *WorkStationIPAddr()
{
#ifndef WIN32
	char *temp=NULL,*temp1=NULL,buffer[512];
	char *ipaddr=NULL,*wipaddr=NULL;
	int vnc_num=0;
	FILE *fp=NULL;
	int x=0;

	temp=RDA_GetEnv("DISPLAY");
	if(isEMPTY(temp)) return(NULL);
	for(temp1=temp;*temp1;++temp1)
	{
		if(*temp1=='.') 
		{
			*temp1=0;
			break;
		}
	}
	temp1=temp+1;
	vnc_num=atoi(temp1);
	temp1=NULL;
	for(x=0;x<9;++x)
	{
		memset(stemp,0,101);
		sprintf(stemp,"ifconfig eth%d | grep \"inet addr:\"",x);
		fp=popen(stemp,"r");
		if(fp!=NULL)
		{
			fgets(buffer,sizeof(buffer),fp);
			if(!isEMPTY(buffer))
			{
				temp1=RDAstrstr(buffer,"inet addr:");
				if(temp1!=NULL)
				{
					temp1+=10;
					ipaddr=stralloc(temp1);
				}
				pclose(fp);
			}
		}
		if(!isEMPTY(ipaddr)) break;
	}
	for(temp1=ipaddr;*temp1;++temp1)
	{
		if(*temp1==' ') 
		{
			*temp1=0;
			break;
		}
	}
	memset(stemp,0,101);
	sprintf(stemp,"%s:59%d",ipaddr,vnc_num);
	if(ipaddr!=NULL) Rfree(ipaddr);
	ipaddr=stralloc(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"netstat -n| grep ESTABLISHED | grep %s",ipaddr);
	fp=popen(stemp,"r");
	if(fp!=NULL)
	{
		fgets(buffer,sizeof(buffer),fp);
		pclose(fp);
	}
	temp1=RDAstrstr(buffer,ipaddr);
	temp1+=RDAstrlen(ipaddr);
	while(TRUE)
	{
		if(*temp1==' ') ++temp1;
		else break;
	}
	wipaddr=stralloc(temp1);
	for(temp1=wipaddr;*temp1;++temp1)
	{
		if(*temp1==':')
		{
			*temp1=0;
			break;
		}
	}
	return(wipaddr);
#else
	return(NULL);
#endif 
}
static char CSHUSER_BOOLEANS(char *name)
{
	short fileno=(-1);
	char retval=FALSE;

	if(XPERT_SETUP->CSHDWR==FALSE) return(0);
	fileno=OPNNRD("CSHDWR","CSHUSER");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(fileno,1))
		{
			FINDFLDGETCHAR(fileno,name,&retval);
		} 
		CLSNRD(fileno);
	}
	return(retval);
}

void PP_translate_GUI(char **expression)
{
	char *temp_expression=NULL;

	if(!isEMPTY(*expression))
	{
		temp_expression=PP_translate(*expression);
		Rfree(*expression);
		*expression=stralloc(temp_expression);
		if(temp_expression!=NULL) Rfree(temp_expression);
	} 
}
void PP_translate_GUIALL(RDArsrc *r)
{
	int x=0;
	RDArmem *member=NULL;

	for(x=0,member=r->rscs;x<r->numrscs;++x,++member)
	{
		PP_translate_GUI(&member->editable_expression);
		PP_translate_GUI(&member->sensitive_expression);
		PP_translate_GUI(&member->transversal_expression);
	}
}
short xaddDFincvir(RDArsrc *r,char *modulex,char *filex,char *keyx,
	short fileno,int line,char *file)
{
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG addDFincvir Adding Files to Associate Virtual fields with Module [%s] File [%s] Key [%s] at line [%d] program [%s].",(modulex!=NULL ? modulex:""),(filex!=NULL ? filex:""),(keyx!=NULL ? keyx:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r->incvir!=NULL)
	{
		r->incvir=Rrealloc(r->incvir,(r->numvir+1)*sizeof(DFincvir));
	} else {
		r->incvir=Rmalloc(sizeof(DFincvir));
		r->numvir=0;
	}
	i=r->incvir+r->numvir;
	if(modulex!=NULL)
	{
		i->module=stralloc(modulex);
	} else i->module=NULL;
	if(filex!=NULL)
	{
		i->file=stralloc(filex);
	} else i->file=NULL;
	if(keyx!=NULL) i->keyname=stralloc(keyx);
		else i->keyname=NULL;
	i->fileno=fileno;
	i->v=NULL;
	return(++r->numvir);
}
RDArsrc *xcopyRDAvirtual(RDArsrc *fromr,RDArsrc *tor,int line,char *file)
{
	RDAvirtual *v1,*v2;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG copyDFincvir Coping Resource DFincvirs at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(tor!=NULL)
	{
		if(fromr!=NULL)
		{
		if(fromr->virflds!=NULL)
		{
			tor->virflds=Rmalloc(fromr->num*sizeof(RDAvirtual));
			tor->num=fromr->num;
			for(x=0,v1=fromr->virflds,v2=tor->virflds;x<fromr->num;
				++x,++v1,++v2)
			{
				v2->when=v1->when;
				v2->name=stralloc(v1->name);
				v2->type=v1->type;
				v2->len=v1->len;
				v2->expression=stralloc(v1->expression);
				v2->computed=v1->computed;
				v2->range=v1->range;
				switch(v2->type)
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
					v2->value.string_value=stralloc(v1->value.string_value);
					if(!isEMPTY(v2->value.string_value))
					{
						v2->dlen=RDAstrlen(v2->value.string_value)+1;
					} else v2->dlen=0;
					break;
				case BOOLNS:
				case CHARACTERS:
					v2->value.string_value=Rmalloc(1);
					*v2->value.string_value=*v1->value.string_value;
					v2->dlen=1;
					break;
				case DECIMALV:
				case SDECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case SDOUBLEV:
					v2->value.float_value=Rmalloc(sizeof(double));
					*v2->value.float_value=*v1->value.float_value;
					v2->dlen=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					v2->value.short_value=Rmalloc(sizeof(short));
					*v2->value.short_value=*v1->value.short_value;
					v2->dlen=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					v2->value.integer_value=Rmalloc(sizeof(int));
					*v2->value.integer_value=*v1->value.integer_value;
					v2->dlen=sizeof(int);
					break;
				default:
					prterr("Error field type [%d] not found for virtual field [%s] at line [%d] program [%s].",v1->type,v1->name,line,file);
					v2->dlen=0;
					break;
				}
			}
		}
		}
	}
	return(tor);
}
RDArsrc *xcopyDFincvir(RDArsrc *fromr,RDArsrc *tor,int line,char *file)
{
	DFincvir *i;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG copyDFincvir Coping Resource DFincvirs at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(tor!=NULL)
	{
		if(fromr!=NULL)
		{
		if(fromr->incvir!=NULL)
		{
			for(x=0,i=fromr->incvir;x<fromr->numvir;++x,++i)
			{
				addDFincvir(tor,i->module,i->file,i->keyname,i->fileno);
			}
		}
		}
	}
	return(tor);
}
short xGLOBALSubData(char **tmp,char *modulename,int line,char *file)
{
        short start=1,length=0,str_length=0;
        char *temp=NULL,*gn=NULL,*temp1=NULL,stemp[101];
	short svalue=0;
	char bvalue=0;
	double dvalue=0.0;
        RDAfinmgt *fmgt=NULL;
        RDApayroll *pay=NULL;
        RDAappmgt *appmgt=NULL;
        RDAvenpmt *venpmt=NULL;
        RDApurord *purord=NULL;
#ifndef WIN32
	uuid_t out;
#endif

#ifdef USE_RDA_DIAGNOSTICS
        if(diagvirtual || diageval)
        {
                prterr("DIAG GLOBALSubData() Substituting for [%s] at line [%d] program [%s].",*tmp,line,file);
        }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(modulename,"ACCOUNT GRANT IDENTIFICATION"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=RDA_ReturnAccountGrantIdentification();
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ACCOUNT REFERENCE TYPE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=RDA_ReturnAccountReferenceType();
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"CURRENT SUBSIDIARY ACCOUNT CODE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=RDA_ReturnCurrentSubsidiaryAccount();
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ACCOUNT MANAGER"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=RDA_ReturnAccountManager();
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"LOGIN IDENTIFICATION"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"SYSTEM DATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	} 
	if(!RDAstrcmp(modulename,"SYSTEM DATE-YYYY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"SYSTEM TIME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"CURRENT PROCESS ID"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		memset(stemp,0,101);
		sprintf(stemp,"%d",RGETPID());
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"CURRENT DIRECTORY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
#ifdef WIN32
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+3);
		sprintf(temp,"%s/",CURRENTDIRECTORY);
#endif
#ifndef WIN32
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+2);
		sprintf(temp,"%s/",CURRENTDIRECTORY);
#endif
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"SYSTEM TYPE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
#if defined(WIN32)
		temp=stralloc("WIN32");
#elif defined(UNIXWARE7)
		temp=stralloc("UNIXWARE7");
#elif defined(UNIXWARE21)
		temp=stralloc("UNIXWARE21");
#elif defined(SCO50P)
		temp=stralloc("SCO5OP");
#elif defined(SCO50)
		temp=stralloc("SCO5O");
#elif defined(SC0386)
		temp=stralloc("SCO386");
#elif defined(RS6000)
		temp=stralloc("RS6000");
#elif defined(LINUX)
		temp=stralloc("LINUX");
#elif defined(LINUX2_2) || defined(UBUNTU_OS)
		temp=stralloc("LINUX2_2");
#elif defined(SUNX86)
		temp=stralloc("SUNX86");
#else
		temp=stralloc("UNKNOWN");
#endif
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"IGNORE RESTRICTED PROCESS CONSTRAINTS"))
	{
		bvalue=UsersIgnoreRestrictedProcessConstraints();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"DISENGAGE WORK FLOW NAVIGATION"))
	{
		bvalue=UsersDisengageWorkFlowNavigation();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"AUTO COMPLETE"))
	{
		bvalue=UsersAutoComplete();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ABLE TO APPROVE"))
	{
		bvalue=Users_Able_to_Approve();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PUSHBUTTON STYLE"))
	{
		svalue=UsersPushButtonStyle();
		memset(stemp,0,101);
		sprintf(stemp,"%d",svalue);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"DOMENU STYLE"))
	{
		svalue=Users_DomenuStyle();
		memset(stemp,0,101);
		sprintf(stemp,"%d",svalue);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"APPROVAL LIMIT"))
	{
		dvalue=Users_Approval_Limit();
		memset(stemp,0,101);
		sprintf(stemp,"%f",dvalue);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS WEB LOGIN"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Login();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS WEB PASSWORD"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Password();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS EMAIL"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Email();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+2);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS PHONE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Phone();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS PHONE EXTENSION"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Phone_Extension();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS SCAN DIRECTORY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Scan_Directory();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ALLOW BEGINNING BALANCES"))
	{
		bvalue=UsersAllowBeginningBalances();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ALLOW POST ACTIVITY"))
	{
		bvalue=UsersAllowPostActivity();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"JOURNAL ENTRY APPROVAL"))
	{
		bvalue=UsersJournalEntryApprove();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"VOUCHER APPROVAL"))
	{
		bvalue=UsersVoucherApprove();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"APPROVE REQUISITIONS"))
	{
		bvalue=ApproveRequisitions();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"SUBMIT REQUISITIONS"))
	{
		bvalue=SubmitRequisitions();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ADMINISTRATIVE EDITS"))
	{
		bvalue=AdminEdits();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ALLOW MULTIPLE ENTRIES"))
	{
		bvalue=UsersAllowMultipleEntries();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"USE BROWSE LIST LABEL"))
	{
		bvalue=Users_BrowseListLabel();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS SUPERVISOR"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Supervisor();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS LOCATION IDENTIFICATION"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_LocationID();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS SITE IDENTIFICATION"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_SiteID();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS NAME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
					length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Name();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"WORKSTATION IP ADDRESS"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
					length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=WorkStationIPAddr();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS DEPARTMENT"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
					length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=Users_Department();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"DISPLAY PURGE FUNCTIONS"))
	{
		bvalue=UsersDisplayPurgeFunctions();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"WORKSTATION VALIDATED"))
	{
		bvalue=UsersWorkstationValidated();
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"WORKSTATION LOGIN"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
					length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=UsersWorkstationLogin();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"WORKSTATION PASSWORD"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
					length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=UsersWorkstationPassword();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"WORKSTATION DOCUMENTS"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
					length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=UsersWorkstationDocuments();
		memset(stemp,0,101);
		sprintf(stemp,"%s",(temp!=NULL?temp:""));
		if(temp!=NULL) Rfree(temp);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		temp=stralloc(stemp);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"DEFAULT PRINTER"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=DefaultPrinter();
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION NAME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #1"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #2"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION CITY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION STATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if((start<=str_length) && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION ZIP"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION PHONE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"ORGANIZATION FAX"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->fax);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"USE CENTURY THRESHOLD"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->use_century_threshold ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"USER INTERFACE TYPE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",GUI_INTERFACE);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CENTURY THRESHOLD"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->century_threshold ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"SIMPLE MENU"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->SimpleMenu);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	} 
	if(!RDAstrcmp(modulename,"ORGANIZATION TYPE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->OrgType);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"SOFTWARE TYPE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->software_type);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"TRANSACTIONS SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->TRANSACTIONS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"SIMPLE SCREENS"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(SIMPLE_SCREENS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"DEV_LICENSE"))
	{
		sprintf(*tmp,"%d",FALSE);
		temp=getenv("DEV_LICENSE");
		if(temp==NULL) temp=RDA_GetEnv("DEV_LICENSE");
		if(temp!=NULL)
		{
			temp1=strtok(temp," ");
			if(CheckDevLicense(temp1))
			{
				sprintf(*tmp,"%d",TRUE);
			} else {
				sprintf(*tmp,"%d",FALSE);
			}
		}
		return(0);
	}
	if(!RDAstrcmp(modulename,"EXTENSIONS SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->EXTENSIONS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"HRM SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->HRM ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(XPERT_SETUP->FINMGT==TRUE)
	{
		if(!RDAstrcmp(modulename,"STARTING FISCAL MONTH"))
		{
			memset(stemp,0,101);
			fmgt=RDAfinmgtNEW();
			getfinmgtbin(fmgt);
			sprintf(stemp,"%d",fmgt->month);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_finmgt(fmgt);
			fmgt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"FINMGT BUDGET TYPE"))
		{
/* 0=ANNUAL, 1=MONTHLY */
			memset(stemp,0,101);
			fmgt=RDAfinmgtNEW();
			getfinmgtbin(fmgt);
			sprintf(stemp,"%d",fmgt->budget);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_finmgt(fmgt);
			fmgt=NULL;
			return(0);
		}
	}
	if(XPERT_SETUP->PAYROLL==TRUE)
	{
		if(!RDAstrcmp(modulename,"PAYROLL MANAGE EARNINGS"))
		{
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			memset(stemp,0,101);
			sprintf(stemp,"%d",(pay->manage_earnings==TRUE ? TRUE:FALSE));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"PAY BALANCE OF CONTRACT"))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%d",(Pay_Balance_of_Contract ? TRUE:FALSE));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			return(0);
		}
		if(!RDAstrcmp(modulename,"DUMP DEFERRED PAY BALANCE"))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%d",(Dump_Deferred_Pay_Balance ? TRUE:FALSE));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			return(0);
		}
		if(!RDAstrcmp(modulename,"USE PAYROLL FUND"))
		{
			memset(stemp,0,101);
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			sprintf(stemp,"%d",pay->use_payroll_fund);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"PAYSEL AUDIT"))
		{
			memset(stemp,0,101);
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			sprintf(stemp,"%d",(pay->paysel_audit ? 1:0));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		} 
		if(!RDAstrcmp(modulename,"PAYNET AUDIT"))
		{
			memset(stemp,0,101);
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			sprintf(stemp,"%d",(pay->paynet_audit ? 1:0));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		} 
		if(!RDAstrcmp(modulename,"PAYMTS AUDIT"))
		{
			memset(stemp,0,101);
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			sprintf(stemp,"%d",(pay->paymts_audit ? 1:0));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"SUBSEL AUDIT"))
		{
			memset(stemp,0,101);
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			sprintf(stemp,"%d",(pay->subsel_audit ? 1:0));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"DIRECT DEPOSIT TYPE"))
		{
			memset(stemp,0,101);
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			sprintf(stemp,"%d",pay->direct_deposit_type);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		} 
		if(!RDAstrcmp(modulename,"EIN NUMBER"))
		{
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			if(!isEMPTY(pay->ein))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(pay->ein)))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(pay->ein)+3);
				}
				sprintf(*tmp,"\"%s\"",pay->ein);
			} else sprintf(*tmp,"\"\"");
			free_payroll(pay);
			pay=NULL;
			return(0);
		} 
		if(!RDAstrcmp(modulename,"PAYJAMS ROLLOVER"))
		{
			memset(stemp,0,101);
			pay=RDApayrollNEW();
			getpayrollbin(pay);
			sprintf(stemp,"%d",(pay->payjams_rollover ? 1:0));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_payroll(pay);
			pay=NULL;
			return(0);
		}

	}
	if(XPERT_SETUP->VENPMT==TRUE)
	{
		if(!RDAstrcmp(modulename,"VENPMT EIN NUMBER"))
		{		 
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->ein))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->ein)))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->ein)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->ein);
			} else sprintf(*tmp,"\"\"");
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"USE VENPMT CLEARING FUND"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",(venpmt->use_venpmt_fund ? 1:0));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		} 
		if(!RDAstrcmp(modulename,"USE VENPMT FISCAL SUMMARY"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",(venpmt->use_fiscal_summary ? 1:0));
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"USE VENPMT FISCAL MONTHS"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->use_fiscal_months);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP USE TAXES"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Use_Taxes);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP ADMIN FEE"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->admin_fee)+2))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->admin_fee)+3);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+3);
			sprintf(*tmp,"\"%s\"",(venpmt->admin_fee!=NULL ? venpmt->admin_fee:""));
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP USE TAX1"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Use_Tax1);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP SEPERATE TAX1 VENDOR"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Sep_Tax1_Vendor);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP TAX1 RATE"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%f",venpmt->Tax1_Rate);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX1 VENDOR"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax1_Vendor))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax1_Vendor)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax1_Vendor)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax1_Vendor);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX1 ADDRESS"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax1_Address))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax1_Address)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax1_Address)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax1_Address);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP USE TAX2"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Use_Tax2);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP SEPERATE TAX2 VENDOR"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Sep_Tax2_Vendor);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP TAX2 RATE"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%f",venpmt->Tax2_Rate);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX2 VENDOR"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax2_Vendor))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax2_Vendor)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax2_Vendor)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax2_Vendor);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX2 ADDRESS"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax2_Address))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax2_Address)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax2_Address)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax2_Address);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP USE TAX3"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Use_Tax3);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP SEPERATE TAX3 VENDOR"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Sep_Tax3_Vendor);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP TAX3 RATE"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%f",venpmt->Tax3_Rate);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX3 VENDOR"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax3_Vendor))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax3_Vendor)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax3_Vendor)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax3_Vendor);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX3 ADDRESS"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax3_Address))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax3_Address)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax3_Address)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax3_Address);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP USE TAX4"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Use_Tax4);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP SEPERATE TAX4 VENDOR"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Sep_Tax4_Vendor);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP TAX4 RATE"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%f",venpmt->Tax4_Rate);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX4 VENDOR"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax4_Vendor))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax4_Vendor)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax4_Vendor)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax4_Vendor);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX4 ADDRESS"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax4_Address))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax4_Address)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax4_Address)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax4_Address);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP USE TAX5"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Use_Tax5);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP SEPERATE TAX5 VENDOR"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%d",venpmt->Sep_Tax5_Vendor);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP TAX5 RATE"))
		{
			memset(stemp,0,101);
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			sprintf(stemp,"%f",venpmt->Tax5_Rate);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX5 VENDOR"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax5_Vendor))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax5_Vendor)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax5_Vendor)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax5_Vendor);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"VENPMT SETUP DEFAULT TAX5 ADDRESS"))
		{
			venpmt=RDAvenpmtNEW();
			getvenpmtbin(venpmt);
			if(!isEMPTY(venpmt->Default_Tax5_Address))
			{
				if(RDAstrlen(*tmp)<(RDAstrlen(venpmt->Default_Tax5_Address)+2))
				{
					*tmp=Rrealloc(*tmp,RDAstrlen(venpmt->Default_Tax5_Address)+3);
				}
				sprintf(*tmp,"\"%s\"",venpmt->Default_Tax5_Address);
			} else { 
				sprintf(*tmp,"\"\"");
			}
			free_venpmt(venpmt);
			venpmt=NULL;
			return(0);
		}
	}
	if(XPERT_SETUP->PURORD==TRUE)
	{
		if(!RDAstrcmp(modulename,"USE PURORD FISCAL MONTHS"))
		{
			memset(stemp,0,101);
			purord=RDApurordNEW();
			getpurordbin(purord);
			sprintf(stemp,"%d",purord->use_fiscal_months);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_purord(purord);
			purord=NULL;
			return(0);
		}
	}
	if(XPERT_SETUP->APPMGT==TRUE)
	{
		if(!RDAstrcmp(modulename,"USE PERSONNEL INTERVIEW"))
		{
			memset(stemp,0,101);
			appmgt=RDAappmgtNEW();
			getappmgtbin(appmgt);
			sprintf(stemp,"%d",appmgt->use_personnel_interview);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_appmgt(appmgt);
			appmgt=NULL;
			return(0);
		}
		if(!RDAstrcmp(modulename,"USE ADMINISTRATIVE INTERVIEW"))
		{
			memset(stemp,0,101);
			appmgt=RDAappmgtNEW();
			getappmgtbin(appmgt);
			sprintf(stemp,"%d",appmgt->use_administrative_interview);
			if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
			}
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"%s",stemp);
			free_appmgt(appmgt);
			appmgt=NULL;
			return(0);
		}
	}
	if(XPERT_SETUP->RLSTMGT==TRUE)
	{
		if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER PHONE"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER ADDRESS 1"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER ADDRESS 2"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER CITY"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER STATE"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER ZIP"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT TREASURER"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT TREASURER PHONE"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT TREASURER ADDRESS 1"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT TREASURER ADDRESS 2"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT TREASURER CITY"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT TREASURER STATE"))
		{
			return(0);
		} 
		if(!RDAstrcmp(modulename,"RLSTMGT TREASURER ZIP"))
		{
			return(0);
		}
	}
	if(!RDAstrcmp(modulename,"UTILITIES SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->UTILITIES ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"OPENRDA 4.0 SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",TRUE);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"OPENRDA SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",FALSE);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"FINMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->FINMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BNKREC SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->BNKREC ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"VENPMT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->VENPMT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PURORD SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->PURORD? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CATALOGUE SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->CATALOGUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"REQMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->REQMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PRSNNL SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->PRSNNL ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"POSTRK SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->POSTRK ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PAYROLL SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->PAYROLL ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"SUBMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->SUBMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"INVCTL SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->INVCTL ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"LVEMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->LVEMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"EMPABS SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->EMPABS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BFTMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->BFTMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"IIDMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->IIDMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"APPMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->APPMGT? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BUDPREP SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->BUDPREP ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PAYENC SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->PAYENC ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"FIXASS SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->FIXASS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"INVCTL SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->INVCTL ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"OCCTAX SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->OCCTAX ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"MISCBILL SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->MISCBILL ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"TAXCLT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->TAXCLT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PPTMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->PPTMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"RLSTMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->RLSTMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"STUDFIN SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->STUDFIN ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"VEHMTN SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->VEHMTN ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"WRKORD SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->WRKORD ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"VERSION DATE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s",(VERSION_DATE!=NULL?VERSION_DATE:""));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PROGRAM NAME"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s",(PROGRAM_NAME!=NULL?PROGRAM_NAME:""));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"DMVREG SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->DMVREG ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"RLVEMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->RLVEMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHDWR SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->CSHDWR ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"RREQMGT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->RREQMGT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ARCHIVE SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->ARCHIVE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CIS SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->CIS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ASP SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->ASP ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BARCODING SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->BARCODING ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CREDITCARDS SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->CREDITCARDS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"FOODSVC SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->FOODSVC ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ROLLACCT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->ROLLACCT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ACCOUNTING METHOD"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->AccountingMethod);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BLDPRMT SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->BLDPRMT ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CITWEB SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->CITWEB? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PROPERTY SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->PROPERTY ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"UTLBLL SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->UTLBLL ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"LVSIMP SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->LVSIMP ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"VIOLATION SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->VIOLATION ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"SHOW NEW IN DEFINE LIST"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->ShowDefineListNew);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	} 
	if(!RDAstrcmp(modulename,"USE END DATE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->UseEndDate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	} 
	if(!RDAstrcmp(modulename,"END DATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->EndDate);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"PROGRAM DIRECTORY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->ProgDir);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"DATA DIRECTORY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->DataDir);
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
		return(0);
	}
	if(!RDAstrcmp(modulename,"NO USERS"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->NoUsers));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"TOOLS SETUP"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(XPERT_SETUP->TOOLS ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER BLDPRMT"))
	{
		bvalue=CSHUSER_BOOLEANS("BLDPRMT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER BLDPRMT DEPOSIT"))
	{
		bvalue=CSHUSER_BOOLEANS("BLDPRMT DEPOSIT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER BLDPRMT DISTRIBUTION"))
	{
		bvalue=CSHUSER_BOOLEANS("BLDPRMT DISTRIBUTION");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER FINMGT"))
	{
		bvalue=CSHUSER_BOOLEANS("FINMGT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER FINMGT DEPOSIT"))
	{
		bvalue=CSHUSER_BOOLEANS("FINMGT DEPOSIT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER FINMGT DISTRIBUTION"))
	{
		bvalue=CSHUSER_BOOLEANS("FINMGT DISTRIBUTION");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER MISCBILL"))
	{
		bvalue=CSHUSER_BOOLEANS("MISCBILL");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER MISCBILL DEPOSIT"))
	{
		bvalue=CSHUSER_BOOLEANS("MISCBILL DEPOSIT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER MISCBILL DISTRIBUTION"))
	{
		bvalue=CSHUSER_BOOLEANS("MISCBILL DISTRIBUTION");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER OCCTAX"))
	{
		bvalue=CSHUSER_BOOLEANS("OCCTAX");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER OCCTAX DEPOSIT"))
	{
		bvalue=CSHUSER_BOOLEANS("OCCTAX DEPOSIT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER OCCTAX DISTRIBUTION"))
	{
		bvalue=CSHUSER_BOOLEANS("OCCTAX DISTRIBUTION");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER PROPERTY"))
	{
		bvalue=CSHUSER_BOOLEANS("PROPERTY");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER PROPERTY DEPOSIT"))
	{
		bvalue=CSHUSER_BOOLEANS("PROPERTY DEPOSIT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER PROPERTY DISTRIBUTION"))
	{
		bvalue=CSHUSER_BOOLEANS("PROPERTY DISTRIBUTION");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER RLSTMGT"))
	{
		bvalue=CSHUSER_BOOLEANS("RLSTMGT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER RLSTMGT DEPOSIT"))
	{
		bvalue=CSHUSER_BOOLEANS("RLSTMGT DEPOSIT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER RLSTMGT DISTRIBUTION"))
	{
		bvalue=CSHUSER_BOOLEANS("RLSTMGT DISTRIBUTION");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER UTLBLL"))
	{
		bvalue=CSHUSER_BOOLEANS("UTLBLL");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER UTLBLL DEPOSIT"))
	{
		bvalue=CSHUSER_BOOLEANS("UTLBLL DEPOSIT");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER UTLBLL DISTRIBUTION"))
	{
		bvalue=CSHUSER_BOOLEANS("UTLBLL DISTRIBUTION");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CSHUSER POST ACTIVITY"))
	{
		bvalue=CSHUSER_BOOLEANS("POST ACTIVITY");
		memset(stemp,0,101);
		sprintf(stemp,"%d",(bvalue==TRUE ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"UUID GENERATE"))
	{
#ifndef WIN32
		uuid_generate_time(out);
		memset(stemp,0,101);
		memcpy(stemp,out,16);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
#else 
		memset(stemp,0,101);
		sprintf(*tmp,"%s",stemp);
		return(0);
#endif
	}

	return(1);
}
