/* evaluate.c - Expression Parser and Evaluation Functions */
#include <evaluate.hpp>
#include<cctype>
#include<climits>
#include<quotes.hpp>
#include<misc.hpp>
#include<fcntl.h>
#include<b64encode.hpp>
#include<b64decode.hpp>

#ifndef WIN32
#ifdef UBUNTU_OS
#include<ctaocrypt/config.h>
#include<ctaocrypt/md5.h>
#else
#include<cyassl/ctaocrypt/md5.h>
#endif 
#endif 

#ifndef WIN32
#include<sys/stat.h>
#endif

/*lint -e537 */
/*lint +e537 */
#define countifs(a)	xcountifs(a,__LINE__,__FILE__)
#define OPERATOR		1
#define NUMBER			2
#define STRING			3
/* Set to virtual memory buffer size */
#define CHUNKSIZE	4096
#define NEW_EVAL

typedef struct stat Statb;

char *func="@?;:+->*<%=!/^()&|#${}`~abcdefghijklmnopqrstuvwxyz_";
char ComputeVirtualType=TRUE;
#define arith(a,b,c,d,e)	xarith(a,b,c,d,e,__LINE__,__FILE__)

RDAeval *xRDAevalNEW(char *string,int line,char *file)
{
	RDAeval *tmp;
	int x=0,y=0;

#ifdef USE_RDA_DIAGNOSTICS

#ifdef ALL_DIAGNOSTICS
	if(diageval)
	{
		prterr("DIAG RDAevalNEW Allocating RDAeval Structure for [%s] at line [%d] program [%s].",string,line,file);
	}
#endif
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAeval));
	if(string!=NULL)
	{
		x=RDAstrlen(string)+1;
		y=round(x/CHUNKSIZE);
		if(x<(CHUNKSIZE*y)) x=CHUNKSIZE*y;
		tmp->token=Rmalloc(x);
		memset(tmp->token,0,x);
	} else {
		tmp->token=NULL;
	}
	tmp->prog=string;
	tmp->type=OPERATOR;
	return(tmp);
}
void xFreeRDAeval(RDAeval *tmp,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS

#ifdef ALL_DIAGNOSTICS
	if(diageval)
	{
		prterr("DIAG FreeRDAeval Freeing RDAeval Structure at line [%d] program [%s].",line,file);
	}
#endif
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(tmp!=NULL)
	{
		if(tmp->token!=NULL) Rfree(tmp->token);
		Rfree(tmp);
	}
}

void xSEERDAresult(RDAresult *r,int line,char *file)
{
	RDAeval *eval=NULL;

	if(r!=NULL)
	{
		eval=r->eval;
		if(r->which==STRING)
		{
			prterr("DIAG SEERDAresult Which [%s] Value [%s] for Token [%s] pointing to prog [%c] type [%d] at line [%d] program [%s].",(r->which==STRING ? "String":"Number"),(r->value.string_value!=NULL ? r->value.string_value:""),eval->token,*eval->prog,eval->type,line,file);
		} else if(r->which==NUMBER)
		{
			prterr("DIAG SEERDAresult Which [%s] Value [%.0f] for Token [%s] pointing to prog [%c] type [%d] at line [%d] program [%s].",(r->which==STRING ? "String":"Number"),(r->value.float_value!=NULL ? *r->value.float_value:0),eval->token,*eval->prog,eval->type,line,file);
		}
	} else {
		prterr("DIAG SEERDAresult RDAresult is NULL at line [%d] program [%s].",line,file);
	}
}
void xFreeRDAresult(RDAresult *result,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS

#ifdef ALL_DIAGNOSTICS
	if(diageval)
	{
		prterr("DIAG FreeRDAresult Freeing RDAresult Structure at line [%d] program [%s].",line,file);
	}
#endif 
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(result!=NULL)
	{
		if(result->which==STRING)
		{
			if(result->value.string_value!=NULL) 
				Rfree(result->value.string_value);
		} else if(result->which==NUMBER)
		{
			if(result->value.float_value!=NULL) 
				Rfree(result->value.float_value);
		}
		Rfree(result);
	}
}
RDAresult *xRDAresultNEW(short which,RDAeval *eval,int line,char *file)
{
	RDAresult *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS

#ifdef ALL_DIAGNOSTICS
	if(diageval)
	{
		prterr("DIAG RDAresultNEW Allocating RDAresult Structure at line [%d] program [%s].",line,file);
	}
#endif
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAresult));
	tmp->which=which;
	tmp->eval=eval;
	if(tmp->which==STRING)
	{
		tmp->value.string_value=NULL;
#ifdef NEW_EVAL
		tmp->dlen=0;
#endif
	} else if(tmp->which==NUMBER)
	{
		tmp->value.float_value=Rmalloc(sizeof(double));
		*tmp->value.float_value=0;
#ifdef NEW_EVAL
		tmp->dlen=sizeof(double);
#endif
	} else {
		tmp->value.float_value=NULL;
		tmp->value.string_value=NULL;
		tmp->which=STRING;	
	}
	return(tmp);
}
void rwhite(char *string)
{
	char *s;

	for (s=string;*string;++string)
	{
		if(!isspace((int)*string))
		{
			*s=(*string);
			++s;
		}
	}
	*s=0;
}
void xget_exp(RDAresult *result,void (*subfunc)(...),void *arg,int line,char *file)
{
	RDAeval *tmp;

#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG get_exp at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	get_token(result);
	tmp=result->eval;
	if(!*tmp->token)
	{
		return;
	}
	level2(result,subfunc,arg);
}
void xarith(int o,RDAresult *r,RDAresult *h,void (*subfunc)(...),void *arg,int line,char *file)
{
	double t,ex;

#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG arith Operation [%c] [%d] at line [%d] program [%s].",o,o,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef XXX 
	prterr("arith r ");SEERDAresult(r);prterr("arith operator [%c] ",o);prterr("arith h ");SEERDAresult(h);
#endif
	if(r->which==NUMBER)
	{
		switch(o)
		{
			case '@' :
				if(h->which==NUMBER)
				{
					if((*r->value.float_value)!=(*h->value.float_value))
					{
						*r->value.float_value=1;
					} else {
						*r->value.float_value=0;
					}
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '%' :
				if(h->which==NUMBER)
				{
					*r->value.float_value=fmod(*r->value.float_value,*h->value.float_value);
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '-' :
				if(h->which==NUMBER)
				{
					*r->value.float_value=(*r->value.float_value-*h->value.float_value);
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '+' :
				if(h->which==NUMBER)
				{
					*r->value.float_value=(*r->value.float_value+*h->value.float_value);
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '#' :
				if(h->which==NUMBER)
				{
					if((*r->value.float_value)<=(*h->value.float_value))
					{
						*r->value.float_value=1;
					} else {
						*r->value.float_value=0;
					}
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '<' :
				if(h->which==NUMBER)
				{
					if(*r->value.float_value<*h->value.float_value)
					{
						*r->value.float_value=1;
					} else {
						*r->value.float_value=0;
					}
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '$' :
				if(h->which==NUMBER)
				{
					*r->value.float_value=(((*r->value.float_value)>=(*h->value.float_value))?1:0);
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '>' :
				if(h->which==NUMBER)
				{
					if(*r->value.float_value>*h->value.float_value)
					{
						*r->value.float_value=1;
					} else {
						*r->value.float_value=0;
					}
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '=' :
				if(h->which==NUMBER)
				{
					if(*r->value.float_value==*h->value.float_value)
					{
						*r->value.float_value=1;
					} else {
						*r->value.float_value=0;
					}
				} else *r->value.float_value=0;
				break;
			case '*' :
				if(h->which==NUMBER)
				{
					*r->value.float_value=(*r->value.float_value)*(*h->value.float_value);
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '/' :
				if(h->which==NUMBER)
				{
					if(*h->value.float_value!=0) *r->value.float_value=(*r->value.float_value)/(*h->value.float_value); else *r->value.float_value=0;
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '&' :
				if(h->which==NUMBER)
				{
					if(*r->value.float_value!=0 && *h->value.float_value!=0) *r->value.float_value=1; else *r->value.float_value=0;
				} else {
					if(*r->value.float_value!=0 && !isEMPTY(h->value.string_value)) *r->value.float_value=1; else *r->value.float_value=0;
				}
				break;
			case '|' :
				if(h->which==NUMBER)
				{
					if(*r->value.float_value!=0 || *h->value.float_value!=0) *r->value.float_value=1; else *r->value.float_value=0;
				} else {
					if(*r->value.float_value!=0 || !isEMPTY(h->value.string_value)) *r->value.float_value=1; else *r->value.float_value=0;
				}
				break;
			case '^' :
				if(h->which==NUMBER)
				{
					ex=(*r->value.float_value);
					if(*h->value.float_value==0)
					{
						*r->value.float_value=1;
						break;
					}
					for(t=(*h->value.float_value-1);t>0;--t) *r->value.float_value=(*r->value.float_value)*(ex);
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case (-10):
				/* decode_base64_to_file(filename,base64_string); */
				decode_base64_to_file(r->value.string_value,h->value.string_value);
				break;
			default:
				prterr("Error Unrecognized Arithmetic Type [%d].",o);
				break;
		}
	} else if(r->which==STRING)
	{
		switch(o)
		{
			case '@' :
				if(h->which==STRING)
				{
					if(RDAstrcmp(r->value.string_value,h->value.string_value))
					{
						t=TRUE;
					} else t=FALSE;
					if(r->value.string_value!=NULL) Rfree(r->value.string_value);
					r->value.string_value=NULL;	
					r->value.float_value=Rmalloc(sizeof(double));
					*r->value.float_value=t;
#ifdef NEW_EVAL
					r->dlen=sizeof(double);
#endif
					r->which=NUMBER;
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '%' :
				break;
			case '-' :
				break;
			case '+' :
				if(h->which==STRING)
				{
					if(!isEMPTY(h->value.string_value))
					{
						if(r->value.string_value!=NULL)
						{
							r->value.string_value=Rrealloc(r->value.string_value,
								RDAstrlen(r->value.string_value)+
								RDAstrlen(h->value.string_value)+1);
							strcat(r->value.string_value,h->value.string_value);
						} else {
							r->value.string_value=stralloc(h->value.string_value);
						}
					}
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '#' :
				if(h->which==STRING)
				{
					if(RDAstrcmp(r->value.string_value,h->value.string_value)<=0)
					{
						t=TRUE;
					} else t=FALSE;
					if(r->value.string_value!=NULL) Rfree(r->value.string_value);
					r->value.string_value=NULL;	
					r->value.float_value=Rmalloc(sizeof(double));
					*r->value.float_value=t;
#ifdef NEW_EVAL
					r->dlen=sizeof(double);
#endif
					r->which=NUMBER;
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '<' :
				if(h->which==STRING)
				{
					if(RDAstrcmp(r->value.string_value,h->value.string_value)<0)
					{
						t=TRUE;
					} else t=FALSE;
					if(r->value.string_value!=NULL) Rfree(r->value.string_value);
					r->value.string_value=NULL;	
					r->value.float_value=Rmalloc(sizeof(double));
					*r->value.float_value=t;
#ifdef NEW_EVAL
					r->dlen=sizeof(double);
#endif
					r->which=NUMBER;
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '>' :
				if(h->which==STRING)
				{
					if(RDAstrcmp(r->value.string_value,h->value.string_value)>0)
					{
						t=TRUE;
					} else t=FALSE;
					if(r->value.string_value!=NULL) Rfree(r->value.string_value);
					r->value.string_value=NULL;	
					r->value.float_value=Rmalloc(sizeof(double));
					*r->value.float_value=t;
#ifdef NEW_EVAL
					r->dlen=sizeof(double);
#endif
					r->which=NUMBER;
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '$' :
				if(h->which==STRING)
				{
					if(RDAstrcmp(r->value.string_value,h->value.string_value)>=0)
					{
						t=TRUE;
					} else t=FALSE;
					if(r->value.string_value!=NULL) Rfree(r->value.string_value);
					r->value.string_value=NULL;	
					r->value.float_value=Rmalloc(sizeof(double));
					*r->value.float_value=t;
#ifdef NEW_EVAL
					r->dlen=sizeof(double);
#endif
					r->which=NUMBER;
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '=' :
				if(h->which==STRING)
				{
					if(!RDAstrcmp(r->value.string_value,h->value.string_value))
					{
						t=TRUE;
					} else t=FALSE;
					if(r->value.string_value!=NULL) Rfree(r->value.string_value);
					r->value.string_value=NULL;	
					r->value.float_value=Rmalloc(sizeof(double));
					*r->value.float_value=t;
#ifdef NEW_EVAL
					r->dlen=sizeof(double);
#endif
					r->which=NUMBER;
				} else {
					prterr("Error invalid types being compared.");
				}
				break;
			case '&' :
				if(h->which==STRING)
				{
					if(!isEMPTY(r->value.string_value) &&
						!isEMPTY(h->value.string_value))
					{
						t=TRUE;
					} else {
						t=FALSE;
					}
				} else {
					if(!isEMPTY(r->value.string_value) && 
						*h->value.float_value)
					{
						t=TRUE;
					} else {
						t=FALSE;
					}
				}
				if(r->value.string_value!=NULL) Rfree(r->value.string_value);
				r->value.string_value=NULL;	
				r->value.float_value=Rmalloc(sizeof(double));
				*r->value.float_value=t;
				r->which=NUMBER;
#ifdef NEW_EVAL
				r->dlen=sizeof(double);
#endif
				break;
			case '|' :
				if(h->which==STRING)
				{
					if(!isEMPTY(r->value.string_value) ||
						!isEMPTY(h->value.string_value))
					{
						t=TRUE;
					} else {
						t=FALSE;
					}
				} else {
					if(!isEMPTY(r->value.string_value) ||	
						*h->value.float_value)
					{
						t=TRUE;
					} else {
						t=FALSE;
					}
				}
				if(r->value.string_value!=NULL) Rfree(r->value.string_value);
				r->value.string_value=NULL;	
				r->value.float_value=Rmalloc(sizeof(double));
				*r->value.float_value=t;
#ifdef NEW_EVAL
				r->dlen=sizeof(double);
#endif
				r->which=NUMBER;
				break;
			case 'n' : /* CONTAINS STRING */
				if(h->which==STRING)
				{
					if(RDAstrstr(r->value.string_value,h->value.string_value)==NULL)
					{
						t=FALSE;
					} else {
						t=TRUE;
					}
				} else {
					t=FALSE;
					prterr("Error invalid types being compared.");
				}	 
				if(r->value.string_value!=NULL) Rfree(r->value.string_value);
				r->value.float_value=Rmalloc(sizeof(double));
				*r->value.float_value=t;
#ifdef NEW_EVAL
				r->dlen=sizeof(double);
#endif
				r->which=NUMBER;
				break;
			case 'o' : /* DOES NOT CONTAIN STRING */
				if(h->which==STRING)
				{
					if(RDAstrstr(r->value.string_value,h->value.string_value)==NULL)
					{
						t=TRUE;
					} else {
						t=FALSE;
					}
				} else {
					t=FALSE;
					prterr("Error invalid types being compared.");
				}	 
				if(r->value.string_value!=NULL) Rfree(r->value.string_value);
				r->value.float_value=Rmalloc(sizeof(double));
				*r->value.float_value=t;
#ifdef NEW_EVAL
				r->dlen=sizeof(double);
#endif
				r->which=NUMBER;
				break;
			case '*' :
			case '/' :
			case '^' :
				break;
			case (-10):
				/* decode_base64_to_file(filename,base64_string); */
				decode_base64_to_file(r->value.string_value,h->value.string_value);
				break;
			default:
				prterr("Error Unrecognized Arithmetic Type [%d].",o);
				break;
		}
	}
#ifdef XXX 
	prterr("arich r ");SEERDAresult(r);prterr("arith h ");SEERDAresult(h);
#endif
}
void level2(RDAresult *result,void (*subfunc)(...),void *arg)
{
	char op=0;
	RDAresult *hold;
	RDAeval *tmp;

	hold=RDAresultNEW(result->which,result->eval);
	tmp=result->eval;
#ifdef XXX 
	prterr("level2 result ");SEERDAresult(result);
#endif
	level3(result,subfunc,arg);
	if(result->which==STRING)
	{
		if(tmp->type==OPERATOR)
		{
			op=(*tmp->token);
		}
	} else {
		op=(*tmp->token);
	}
	while(op=='<' || op=='>' || op=='=' || op=='$' || op=='#' || op=='@' || 
		op=='m' || op=='n' || op=='o' || op=='p' || op=='y')
	{
		get_token(hold);
		level3(hold,subfunc,arg);
		arith(op,result,hold,subfunc,arg);
		if(result->which==STRING)
		{
			if(tmp->type==OPERATOR)
			{
				op=(*tmp->token);
			} else op=0;
		} else {
			op=(*tmp->token);
		}
	}
	FreeRDAresult(hold);
	
#ifdef XXX 
	prterr("level2 result ");SEERDAresult(result);
#endif
}
void level3(RDAresult *result,void (*subfunc)(...),void *arg)
{
	char op=0;
	RDAresult *hold;
	RDAeval *eval;

#ifdef XXX 
	prterr("level3 result ");SEERDAresult(result);
#endif
	hold=RDAresultNEW(result->which,result->eval);
	eval=result->eval;
	level4(result,subfunc,arg);
	if(result->which==STRING)
	{
		if(eval->type==OPERATOR)
		{
			op=(*eval->token);
		}
	} else {
			op=(*eval->token);
	}
	while(op=='&' || op=='|')
	{
		get_token(hold);
		level4(hold,subfunc,arg);
		arith(op,result,hold,subfunc,arg);
		if(result->which==STRING)
		{
			if(eval->type==OPERATOR)
			{
				op=(*eval->token);
			} else op=0;
		} else op=(*eval->token);
	}
	FreeRDAresult(hold);
#ifdef XXX 
	prterr("level3 result ");SEERDAresult(result);
#endif
}
void level4(RDAresult *result,void (*subfunc)(...),void *arg)
{
	char op=0;
	RDAresult *hold;
	RDAeval *tmp;

#ifdef XXX 
	prterr("level4 result ");SEERDAresult(result);
#endif
	hold=RDAresultNEW(result->which,result->eval);
	tmp=result->eval;
	level5(result,subfunc,arg);
	if(result->which==STRING)
	{
		if(tmp->type==OPERATOR)
		{
			op=(*tmp->token);
		}
	} else {
			op=(*tmp->token);
	}
	while(op=='+' || op=='-' || op=='!')
	{
		get_token(hold);
		level5(hold,subfunc,arg);
		arith(op,result,hold,subfunc,arg);
		if(result->which==STRING)
		{
			if(tmp->type==OPERATOR)
			{
				op=(*tmp->token);
			} else op=0;
		} else op=(*tmp->token);
	}
	FreeRDAresult(hold);
#ifdef XXX 
	prterr("level4 result ");SEERDAresult(result);
#endif
}
void level5(RDAresult *result,void (*subfunc)(...),void *arg)
{
	char op=0;
	RDAresult *hold;
	RDAeval *tmp;

#ifdef XXX 
	prterr("level5 result ");SEERDAresult(result);
#endif
	hold=RDAresultNEW(result->which,result->eval);
	tmp=result->eval;
	level6(result,subfunc,arg);
	if(result->which==STRING)
	{
		if(tmp->type==OPERATOR)
		{
			op=(*tmp->token);
		} 
	} else op=(*tmp->token);
	while(op=='*' || op=='/' || op=='%')
	{
		get_token(hold);
		level6(hold,subfunc,arg);
		arith(op,result,hold,subfunc,arg);
		if(result->which==STRING)
		{
			if(tmp->type==OPERATOR)
			{
				op=(*tmp->token);
			} else op=0;
		} else op=(*tmp->token);
	}
	FreeRDAresult(hold);
#ifdef XXX 
	prterr("level5 result ");SEERDAresult(result);
#endif
}
void level6(RDAresult *result,void (*subfunc)(...),void *arg)
{
	char op=0;
	RDAresult *hold;
	RDAeval *tmp;

#ifdef XXX 
	prterr("level6 result ");SEERDAresult(result);
#endif
	hold=RDAresultNEW(result->which,result->eval);
	tmp=result->eval;
	level7(result,subfunc,arg);
	if(result->which==STRING)
	{
		if(tmp->type==OPERATOR)
		{
			op=(*tmp->token);
		} 
	} else op=(*tmp->token);
	while(op=='^' || op==(-10))
	{
		get_token(hold);
		level7(hold,subfunc,arg);
		arith(op,result,hold,subfunc,arg);
		if(result->which==STRING)
		{
			if(tmp->type==OPERATOR)
			{
				op=(*tmp->token);
			} else op=0;
		} else op=(*tmp->token);
	}
	FreeRDAresult(hold);
#ifdef XXX 
	prterr("level6 result ");SEERDAresult(result);
#endif
}
#define COMPUTEGenericCounter(a)	xCOMPUTEGenericCounter(a,__LINE__,__FILE__)
int xCOMPUTEGenericCounter(char *,int,char *);
int xCOMPUTEGenericCounter(char *expression,int line,char *file)
{
	char *module=NULL,*name=NULL,*libname=NULL;
	int retval=0;

#ifdef USE_RDA_DIAGNOSTICS

	if(diagsec)
	{
		prterr("DIAG COMPUTEGenericSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(isEMPTY(expression))
	{
		prterr("No [Module][Filename] to use......");
		return(1);
	} else {
		module=stripmodulename(expression);
		name=stripfilename(expression);
	}
	if(isEMPTY(module) || isEMPTY(name))
	{
		prterr("Error Module or Generic Setup Variable name is NULL.");
		if(module!=NULL) Rfree(module);
		if(name!=NULL) Rfree(name);
		return(1);
	}
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+15);
#ifndef WIN32
	sprintf(libname,"%s/%s/",CURRENTDIRECTORY,(module!=NULL ? module:""));
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s\\",CURRENTDIRECTORY,(module!=NULL ? module:""));
#endif
	retval=getnexttran(libname,name,(-1));
	if(libname!=NULL) Rfree(libname);
	if(module!=NULL) Rfree(module);
	if(name!=NULL) Rfree(name);
	return((retval+1));
}
static char *MergeAccountString(char *s)
{
	char *temp=NULL,*temp1=NULL,*acct=NULL,*image=NULL;
	char *m=NULL;
	short atype=0,deftype=0;

	if(isEMPTY(s)) return(NULL);
	temp1=stralloc(s);
	temp1[1]=0;
	atype=atoi(temp1);
	temp1[3]=0;
	deftype=atoi(&temp1[2]);
	for(temp=temp1+4;*temp;++temp)
	{
		if(*temp=='|') 
		{
			*temp=0;
			break;
		}
	}
	acct=stralloc(&temp1[4]);
	if(RDAstrlen(s)>(5+(RDAstrlen(acct)))) 
	{
		image=stralloc(&s[(5+(RDAstrlen(acct)))]);
	}
	switch(atype)
	{
		default:
		case 0:
			atype=EXPENDITURE;
			break;
		case 1:
			atype=REVENUE;
			break;
		case 2:
			atype=BALANCESHEET;
			break;
		case 3:
			atype=BEGINNINGBALANCE;
			break;
		case 4:
			atype=CUSTOMTYPE;
			break;
	}
	if(!isEMPTY(acct) && !isEMPTY(image))
	{
		m=MergeImage(acct,image,atype,deftype);
	} else m=stralloc(acct);
	if(acct!=NULL) Rfree(acct);
	if(image!=NULL) Rfree(image);
	if(temp1!=NULL) Rfree(temp1);
	return(m);
}
char HexToAscii(char f,char s)
{
	char hex[5],*stop=NULL;
	hex[0]='0';
	hex[1]='x';
	hex[2]=f;
	hex[3]=s;
	hex[4]=0;
	return(strtol(hex,&stop,16));
}
void level7(RDAresult *result,void (*subfunc)(...),void *arg)
{
	char op=0;
	RDAeval *tmp;
	char *wordstring=NULL,*temp=NULL,pComputeVirtualType=FALSE;
	char *temp1=NULL;
	double fltval=0;
	short t=FALSE;
	time_t time_value=0;
	struct tm time_struct;
	NRDfield *field=NULL;
	int hours=0,minutes=0,seconds=0,hold=0;
	int len=0,i=0,count=0,x=0;
	Md5 my_md5;
	unsigned char digest[MD5_DIGEST_SIZE];
	unsigned char my_i[MD5_DIGEST_SIZE];
	FILE *fp=NULL;
	Statb statb;
	base64_encodestate enstate;
	base64_decodestate destate;

	tmp=result->eval;
	if(result->which==STRING)
	{
		if(tmp->type==OPERATOR)
		{
			op=(*tmp->token);
		} 
	} else op=(*tmp->token);
#ifdef XXX 
	prterr("op [%c] [%d] ",op,op);prterr("level7 result ");SEERDAresult(result);
#endif


	if(op=='+' || op=='-' || op=='!' || op=='?' || op==':' || op=='!' ||
		op=='{' || op=='}' || op==';' || op=='~' || op=='`' ||
		op=='a' || op=='b' || op=='c' || op=='d' || op=='e' ||
		op=='f' || op=='g' || op=='h' || op=='i' || op=='j' ||
		op=='k' || op=='l' || op=='m' || op=='n' || op=='o' || 
		op=='p' || op=='q' || op=='r' || op=='s' || op=='t' || 
		op=='u' || op=='v' || op=='w' || op=='x' || op=='y' || 
		op=='z' || op=='_' || op==(-1) || op==(-2) || op==(-3) ||
		op==(-4) || op==(-5) || op==(-6) || op==(-7) || op==(-8) || op==(-9) || op==(-11) || op==(-12))
	{	
	
		get_token(result);
		level2(result,subfunc,arg);
		if(result->which==NUMBER)
		{
		
			if(result->value.float_value==NULL)
			{
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=0;
			}
			if(op=='-') 
			{
			
				(*result->value.float_value)=
					((-1)*(*result->value.float_value));
			} else if(op=='!') /* NOT */
			{
			
				if(*result->value.float_value!=0) *result->value.float_value=0; else *result->value.float_value=1;
			} else if(op=='{') /* ROUNDUP */
			{
			
				*result->value.float_value=
					RDAroundup(*result->value.float_value);
			} else if(op=='}') /* ROUND */
			{
			
				*result->value.float_value=
					round(*result->value.float_value);
			} else if(op=='?') /* SPELL AMOUNT */
			{
			
				wordstring=spell(*result->value.float_value);
				if(result->value.float_value!=NULL) 
					Rfree(result->value.float_value);
			
				result->value.string_value=stralloc(wordstring);
				if(wordstring!=NULL) Rfree(wordstring);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(wordstring)+1;
#endif
				result->which=STRING;
			} else if(op==';') /* DECIMAL_TO_STRING */
			{
			
				memset(stemp,0,101);
				sprintf(stemp,"%.08f",*result->value.float_value);
				if(result->value.float_value!=NULL)
					Rfree(result->value.float_value);
				result->value.string_value=stralloc(stemp);
				memset(stemp,0,RDAstrlen(stemp)+1);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
				result->which=STRING;
			} else if(op==(-12)) /* Wt BCrypt Value */
			{
				if(BCryptValue!=NULL)
				{
					temp=BCryptValue(result->value.string_value);
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
					result->value.string_value=temp;
#ifdef NEW_EVAL
					result->dlen=RDAstrlen(temp)+1;
#endif
					result->which=STRING;
				}
			} else if(op==(-11)) /* READ_FILE_TO_BASE_SIXTYFOUR */
			{
				temp=encode_base64_from_file(result->value.string_value);
				if(result->value.string_value!=NULL) Rfree(result->value.string_value);
				result->value.string_value=temp;
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(temp)+1;
#endif
				result->which=STRING;
			} else if(op=='~') /* VALUE_TO_TIME */
			{
			
				memset(stemp,0,101);
/*line -e524*/
				time_value=(long)*result->value.float_value;
/*line +e524*/
				hours=time_value/3600;
				hold=time_value-(3600*hours);
				minutes=hold/60;
				seconds=hold-(60*minutes);			
				sprintf(stemp,"%02d:%02d:%02d",hours,minutes,seconds);
				if(result->value.float_value!=NULL) 
					Rfree(result->value.float_value);
				result->which=STRING;
				result->value.string_value=stralloc(stemp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
				memset(stemp,0,101);
			} else if((op=='f') /* BOOLEAN_TO_STRING */
				|| (op=='g')    /* SHORT_TO_STRING */
				|| (op=='h')   /* INTEGER_TO_STRING */
				|| (op=='i'))   /* FLOAT_TO_STRING */
			{
			
				memset(stemp,0,101);
				if(op=='i') 
				{
					sprintf(stemp,"%.08f",*result->value.float_value);
				} else {
					sprintf(stemp,"%.0f",*result->value.float_value);
				}
				if(result->value.float_value!=NULL)
					Rfree(result->value.float_value);
				result->value.string_value=stralloc(stemp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
				memset(stemp,0,101);
				result->which=STRING;
			} else if(op=='x') /* INTEGER_TO_ENCRYPTEDA */
			{
			
				len=15; 
				temp=famt_inttoencrypted1(*result->value.float_value,len,'0');
				if(result->value.float_value!=NULL)
					Rfree(result->value.float_value);
				result->value.string_value=stralloc(temp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(temp)+1;
#endif
				if(temp!=NULL) Rfree(temp);
				memset(stemp,0,101);
				result->which=STRING;
			} else if(op==-9) /* DOLLARS_TO_TEXT */
			{
			
				memset(stemp,0,101);
				sprintf(stemp,"%.02f",*result->value.float_value/100);
				result->value.string_value=stralloc(stemp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
				memset(stemp,0,101);
				result->which=STRING;

			} else if(op=='s') /* DOLLARS_TO_STRING */
			{
			
				memset(stemp,0,101);
				sprintf(stemp,"%.02f",
					((*result->value.float_value<0)?(*result->value.float_value*(-1)/100):*result->value.float_value/100));
				len=RDAstrlen(stemp); 
				if((len-4)>0)
				{
					len=(int)(((len-4)/3)+len+2); 
				} else {
					len=len+2; 
				}
				temp=famt(*result->value.float_value,len);
				if(result->value.float_value!=NULL)
					Rfree(result->value.float_value);
				result->value.string_value=stralloc(temp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(temp)+1;
#endif
				if(temp!=NULL) Rfree(temp);
				memset(stemp,0,101);
				result->which=STRING;
			} else if(op=='t') /* DOLLARSNS_TO_STRING */
			{
			
				memset(stemp,0,101);
				sprintf(stemp,"%.2f%c",
					((*result->value.float_value<0)?(*result->value.float_value*(-1)/100):(*result->value.float_value/100)),
					((*result->value.float_value<0)?'-':' '));
				if(result->value.float_value!=NULL)
					Rfree(result->value.float_value);
				result->value.string_value=stralloc(stemp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
				memset(stemp,0,101);
				result->which=STRING;
			} else if(op=='m') /* COMPUTE_TABLE_BASE */
			{
			
			} else if(op=='v')   /* DECIMAL_TO_STRING */
			{
			
				memset(stemp,0,101);
				sprintf(stemp,"%-f",*result->value.float_value);
				if(result->value.float_value!=NULL)
					Rfree(result->value.float_value);
				result->value.string_value=stralloc(stemp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
				memset(stemp,0,101);
				result->which=STRING;
			} else if(op=='y') /* GENERIC_COUNTER */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=COMPUTEGenericCounter(wordstring);
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='_') /* MD5SUM */
			{
			
				InitMd5(&my_md5);
				if(RDAstrlen(result->value.string_value)>MD5_DIGEST_SIZE) 
				{
					result->value.string_value[MD5_DIGEST_SIZE]=0;
				}
				memset(my_i,0,MD5_DIGEST_SIZE);
				memcpy(my_i,result->value.string_value,RDAstrlen(result->value.string_value));
				Md5Update(&my_md5,my_i,RDAstrlen(result->value.string_value));
				Md5Final(&my_md5,digest);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.string_value=Rmalloc(33);
				temp=result->value.string_value;
				for(i=0;i<16;++i)
				{
					sprintf(temp,"%02x",digest[i]);
					temp+=2;
				}
#ifdef NEW_EVAL
				result->dlen=MD5_DIGEST_SIZE+1;
#endif
				result->which=STRING;
			} else if(op==(-1)) /* READ_FILE */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp=stralloc(result->value.string_value);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);

#if !defined(LINUX_SUSE6_1)
					if(stat(temp,&statb)!=(-1))
#else
					if(_xstat(_STAT_VER,temp,&statb)!=(-1))
#endif
					{
#ifndef WIN32
#ifdef _POSIX_SOURCE
						if(!S_ISREG(statb.st_mode))
#else
						if((statb.st_mode&S_IFMT)!=S_IFREG)
#endif
						{
							prterr("Error File [%s] is not a regular readable file!",temp);
						} else {
#endif
						if(statb.st_size>0)
						{
							fp=fopen(temp,"r");
							if(fp!=NULL)
							{
								len=(int)(statb.st_size);
								result->value.string_value=Rmalloc(len+1);
#ifdef NEW_EVAL
								result->dlen=len+1;
#endif
								len=fread(result->value.string_value,sizeof(char),len,fp);
								if(!len)
								{
									prterr("Error:  Failed to read characters to the end of file [%s].",temp);
								}
								fclose(fp);
							  
#ifdef USE_RDA_DIAGNOSTICS

								/* Temp Testing?  This may need removed for performance. */
								if(diageval)
								{
									fp=fopen("/tmp/readfile.out","wb");
									fwrite(result->value.string_value,sizeof(char),len,fp);
									fclose(fp);
								}	
#endif /* ifdef USE_RDA_DIAGNOSTICS */

							}
						} else {
							prterr("Error File [%s] is an empty file!",temp);
						}
#ifndef WIN32
						}
#endif
					}
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-2)) /* BASE64_ENCODE */
			{
			
				len=0;
				if(!isEMPTY(result->value.string_value))
				{
					base64_init_encodestate(&enstate);
					temp=Rmalloc(result->dlen);
					len=result->dlen-1;
					memcpy(temp,result->value.string_value,result->dlen);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=result->dlen*2;
					result->value.string_value=Rmalloc(result->dlen);
#else
					result->value.string_value=Rmalloc(RDAstrlen(temp)*2);
#endif
					temp1=Rmalloc(result->dlen*2);
					len=base64_encode_block(temp,len,temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
					memset(temp1,0,RDAstrlen(temp)+1);
					len=base64_encode_blockend(temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
#ifdef USE_RDA_DIAGNOSTICS

					/* Temp Testing?  This may need removed for performance. */
					if(diageval)
					{
						fp=fopen("/tmp/post-encode.b64","wb");
						fwrite(result->value.string_value,sizeof(char),strlen(result->value.string_value),fp);
						fclose(fp);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(temp1!=NULL) Rfree(temp1);
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-3)) /* BASE64_DECODE */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					base64_init_decodestate(&destate);
					temp=stralloc(result->value.string_value);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.string_value=Rmalloc(RDAstrlen(temp)+1);
#ifdef NEW_EVAL
					result->dlen=RDAstrlen(temp)+1;
#endif
					base64_decode_block(temp,RDAstrlen(temp),result->value.string_value,&destate);
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-4)) /* READ_FILE_HEX */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp=stralloc(result->value.string_value);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);

#if !defined(LINUX_SUSE6_1)
					if(stat(temp,&statb)!=(-1))
#else
					if(_xstat(_STAT_VER,temp,&statb)!=(-1))
#endif
					{
						if(statb.st_size>0)
						{
							fp=fopen(temp,"rb");
							if(fp!=NULL)
							{
								len=(int)(statb.st_size);
								temp1=Rmalloc(len+1);
								result->value.string_value=Rmalloc((len*2)+1);
#ifdef NEW_EVAL
								result->dlen=len*2+1;
#endif
								len=fread(temp1,sizeof(char),len,fp);
								if(!len)
								{
									prterr("Error:  Failed to read characters to the end of file [%s].",temp);
								}
								fclose(fp);
								count=0;
								for(x=0;x<len;++x)
								{
									sprintf(&result->value.string_value[count],"%02x",temp1[x]);

									count+=2;
								}
#ifdef USE_RDA_DIAGNOSTICS

								/* Temp Testing?  This may need removed for performance. */
								if(diageval)
								{
									fp=fopen("/tmp/readfilehex.out","wb");
									fwrite(result->value.string_value,sizeof(char),(len*2),fp);
									fclose(fp);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								if(temp1!=NULL) Rfree(temp1);
								

							}
						} else {
							prterr("Error File [%s] is an empty file!",temp);
						}
					}
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-5)) /* HEX_TO_BASE64_ENCODE */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp1=Rmalloc(result->dlen);
					memcpy(temp1,result->value.string_value,result->dlen);
					memset(result->value.string_value,0,result->dlen);
					count=0;
					for(x=0;x<result->dlen-1;x+=2)
					{
						sprintf(&result->value.string_value[count],"%c",HexToAscii(temp1[x],temp1[x+1]));
						++count;
					}
					if(temp1!=NULL) Rfree(temp1);
					base64_init_encodestate(&enstate);
					temp=Rmalloc(result->dlen);
					len=count;
					memcpy(temp,result->value.string_value,result->dlen);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=result->dlen*2;
					result->value.string_value=Rmalloc(result->dlen);
#else
					result->value.string_value=Rmalloc(RDAstrlen(temp)*2);
#endif
					temp1=Rmalloc(result->dlen*2);
					len=base64_encode_block(temp,len,temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
					memset(temp1,0,RDAstrlen(temp)+1);
					len=base64_encode_blockend(temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
#ifdef USE_RDA_DIAGNOSTICS

					/* Temp Testing?  This may need removed for performance. */
					if(diageval)
					{
						fp=fopen("/tmp/post-encode.b64","wb");
						fwrite(result->value.string_value,sizeof(char),strlen(result->value.string_value),fp);
						fclose(fp);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(temp1!=NULL) Rfree(temp1);
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-6)) /* BASE64_DECODE_TO_HEX */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					base64_init_decodestate(&destate);
					temp=stralloc(result->value.string_value);
					len=RDAstrlen(temp)+1;
					temp1=Rmalloc(RDAstrlen(temp)+1);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.string_value=Rmalloc(len*2+1);
#ifdef NEW_EVAL
					result->dlen=len*2+1;
#endif
					base64_decode_block(temp,len,temp1,&destate);
					count=0;
					for(x=0;x<len;++x)
					{
						sprintf(&result->value.string_value[count],"%02x",temp1[x]);
						count+=2;
					}
					if(temp!=NULL) Rfree(temp);
					if(temp1!=NULL) Rfree(temp1);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-7)) /* STRING_TO_HEX */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp1=Rmalloc(result->dlen);
					memcpy(temp1,result->value.string_value,result->dlen);
					len=result->dlen-1;
					count=0;
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
					result->value.string_value=Rmalloc(2*len+1);
#ifdef NEW_EVAL
					result->dlen=len*2+1;
#endif
					for(x=0;x<len;++x)
					{
						sprintf(&result->value.string_value[count],"%02x",temp1[x]);
						count+=2;
					}
					if(temp1!=NULL) Rfree(temp1);
				} else {
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-8)) /* HEX_TO_STRING */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp1=Rmalloc(result->dlen);
					memcpy(temp1,result->value.string_value,result->dlen);
					memset(result->value.string_value,0,result->dlen);
					count=0;
					for(x=0;x<result->dlen-1;x+=2)
					{
						sprintf(&result->value.string_value[count],"%c",HexToAscii(temp1[x],temp1[x+1]));
						++count;
					}
					if(temp1!=NULL) Rfree(temp1);
				} else {
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op=='w') /* GENERIC SETUP VARIABLE */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				field=COMPUTEGenericSetup(wordstring,subfunc,arg);
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
#ifdef NEW_EVAL
							result->dlen=0;
#endif
							if(field->data.string_value!=NULL)
							{
								result->value.string_value=stralloc(field->data.string_value);
#ifdef NEW_EVAL
								result->dlen=RDAstrlen(result->value.string_value)+1;
#endif
							}
							result->which=STRING;
							break;
						case BOOLNS:
						case CHARACTERS:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.string_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.float_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SHORTV:
						case SSHORTV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.short_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.integer_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case BUTTONS:
						default:
							prterr("Error field type [%d] invalid for cell value [%s].",field->type,field->name);
							break;
					}
					FreeNRDfield(field);
					Rfree(field);
				} else {
#ifdef USE_RDA_DIAGNOSTICS

					if(diageval)
					{
						prterr("Error Compute_Generic_Setup Failed....");
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.float_value=Rmalloc(sizeof(double));
					*result->value.float_value=0;
#ifdef NEW_EVAL
					result->dlen=0;
#endif
					result->which=NUMBER;
				}
			
				if(wordstring!=NULL) Rfree(wordstring);
			}
			
		} else if(result->which==STRING)
		{
			
			if(op==':') /* STRING_TO_VALUE */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					fltval=atof(result->value.string_value);
				} else fltval=0;
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.string_value=NULL;
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
#ifdef XXX
				prterr("level7 after : result ");SEERDAresult(result);
#endif
			} else if(op=='!') /* NOT */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					t=FALSE;
				} else {
					t=TRUE;
				}
				if(result->value.string_value!=NULL) 
					Rfree(result->value.string_value);
				result->value.string_value=NULL;	
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=t;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='z') /* ADD_DASHES */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp=adddashes(result->value.string_value);
				} else temp=NULL;
				if(result->value.string_value!=NULL) Rfree(result->value.string_value);
				result->value.string_value=stralloc(temp);
#ifdef NEW_EVAL
				if(temp!=NULL)
				{
					result->dlen=RDAstrlen(temp)+1;
				} else result->dlen=0;
#endif
				if(temp!=NULL) Rfree(temp);
				result->which=STRING;
			} else if(op=='`')  /* DATE_TO_VALUE */
			{
			
				time_struct.tm_mon=0;
				time_struct.tm_mday=0;
				time_struct.tm_year=0;
				time_struct.tm_hour=0;
				time_struct.tm_min=0;
				time_struct.tm_sec=0;
				time_struct.tm_wday=0;
				time_struct.tm_yday=0;
				time_struct.tm_isdst=0;
				if(!isEMPTY(result->value.string_value))
				{
					temp=strtok(result->value.string_value," /:");
					if(temp!=NULL) time_struct.tm_mon=Ratoi(temp)-1;
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_mday=Ratoi(temp);
					temp=strtok(NULL," /:");
					if(temp!=NULL) 
					{
						time_struct.tm_year=Ratoi(temp);
						if(time_struct.tm_year>1900)
						{
							time_struct.tm_year=(time_struct.tm_year-1900);
						}
					}
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_hour=Ratoi(temp);
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_min=Ratoi(temp);
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_sec=Ratoi(temp);
				}
				time_value=mktime(&time_struct);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=time_value;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='a') /* EVALUATEbol */
			{
			
				pComputeVirtualType=ComputeVirtualType;
				ComputeVirtualType=TRUE;
/* remove quotes or \quotes around result->value.string_value */
				wordstring=reset_quotes(result->value.string_value);
				if(!isEMPTY(wordstring))
				{
					fltval=EVALUATEbol(wordstring,subfunc,arg);
				} else fltval=0;
				ComputeVirtualType=pComputeVirtualType;
				pComputeVirtualType=TRUE;
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				*result->value.float_value=fltval;
				result->which=NUMBER;
			} else if(op=='b') /* EVALUATEsht */
			{
			
/* remove quotes or \quotes around result->value.string_value */
				pComputeVirtualType=ComputeVirtualType;
				ComputeVirtualType=TRUE;
				wordstring=reset_quotes(result->value.string_value);
				if(!isEMPTY(wordstring))
				{
					fltval=EVALUATEsht(wordstring,subfunc,arg);
				} else fltval=0;
				ComputeVirtualType=pComputeVirtualType;
				pComputeVirtualType=TRUE;
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='c') /* EVALUATEint */
			{
			
				pComputeVirtualType=ComputeVirtualType;
				ComputeVirtualType=TRUE;
/* remove quotes or \quotes around result->value.string_value */
				wordstring=reset_quotes(result->value.string_value);
				if(!isEMPTY(wordstring))
				{
					fltval=EVALUATEint(wordstring,subfunc,arg);
				}
				if(wordstring!=NULL) Rfree(wordstring);
				ComputeVirtualType=pComputeVirtualType;
				pComputeVirtualType=TRUE;
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='d') /* EVALUATEdbl */
			{
			
				pComputeVirtualType=ComputeVirtualType;
				ComputeVirtualType=TRUE;
/* remove quotes or \quotes around result->value.string_value */
				wordstring=reset_quotes(result->value.string_value);
				if(!isEMPTY(wordstring))
				{
					fltval=EVALUATEdbl(wordstring,subfunc,arg);
				} else fltval=0;
				ComputeVirtualType=pComputeVirtualType;
				pComputeVirtualType=TRUE;
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='e') /* EVALUATEstr */
			{
			
				pComputeVirtualType=ComputeVirtualType;
				ComputeVirtualType=TRUE;
/* remove quotes or \quotes around result->value.string_value */
				wordstring=reset_quotes(result->value.string_value);
				if(!isEMPTY(wordstring))
				{
					temp=EVALUATEstr(wordstring,subfunc,arg);
				} else temp=NULL;
				ComputeVirtualType=pComputeVirtualType;
				pComputeVirtualType=TRUE;
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				if(temp!=NULL)
				{
					result->value.string_value=stralloc(temp);
#ifdef NEW_EVAL
					result->dlen=RDAstrlen(temp)+1;
#endif
					Rfree(temp);
				} else {
					result->value.string_value=NULL;
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op=='j') /* MERGE_ACCOUNT */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				if(result->value.string_value!=NULL) Rfree(result->value.string_value);
				result->value.string_value=MergeAccountString(wordstring);	
#ifdef NEW_EVAL
				if(result->value.string_value!=NULL) result->dlen=RDAstrlen(result->value.string_value)+1;
				else result->dlen=0;
#endif /* NEW_EVAL */
			} else if(op=='k') /* STRING_TO UPPER */
			{
			
				if(!isEMPTY(result->value.string_value))
					stoupper(result->value.string_value);
			} else if(op=='l') /* STRING_TO_LOWER */
			{
			
				if(!isEMPTY(result->value.string_value))
					stolower(result->value.string_value);
			} else if(op=='m') /* COMPUTE_TABLE_BASE */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				fltval=COMPUTERDATABLE(wordstring,subfunc,arg,
					RDATABLEBASE);
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='n') /* COMPUTE_TABLE_RATE */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				fltval=COMPUTERDATABLE(wordstring,subfunc,arg,
					RDATABLERATE);
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='q') /* COMPUTE_TABLE */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				fltval=COMPUTERDATABLE(wordstring,subfunc,arg,
					RDATABLECOMP);
#ifdef USE_RDA_DIAGNOSTICS

				if(diageval)
				{
					prterr("DIAG COMPUTE_TABLE returning [%.08f] ",fltval);
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='o') /* COMPUTE_MATRIX_ROWCOLUMN */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				field=COMPUTE_MATRIX_ROWCOLUMN(wordstring);
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
#ifdef NEW_EVAL
							result->dlen=0;
#endif
							if(field->data.string_value!=NULL)
							{
								result->value.string_value=stralloc(field->data.string_value);
#ifdef NEW_EVAL
								result->dlen=RDAstrlen(result->value.string_value)+1;
#endif
							}
							result->which=STRING;
							break;
						case BOOLNS:
						case CHARACTERS:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.string_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.float_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SHORTV:
						case SSHORTV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.short_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.integer_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case BUTTONS:
						default:
							prterr("Error field type [%s] invalid for cell value [%s].",field->type,field->name);
							break;
					}
					FreeNRDfield(field);
					Rfree(field);
				} else {
#ifdef USE_RDA_DIAGNOSTICS

					if(diagsec || diagmatrix)
					{
					prterr("Error Compute_Matrix_RowColumn Failed....");
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.float_value=Rmalloc(sizeof(double));
					*result->value.float_value=0;
#ifdef NEW_EVAL
					result->dlen=0;
#endif
					result->which=NUMBER;

				}
				if(wordstring!=NULL) Rfree(wordstring);
			} else if(op=='p') /* COMPUTE_MATRIX_LINEARLY */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				field=COMPUTE_MATRIX_LINEARLY(wordstring);
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
#ifdef NEW_EVAL
							result->dlen=0;
#endif
							if(field->data.string_value!=NULL)
							{
								result->value.string_value=stralloc(field->data.string_value);
#ifdef NEW_EVAL
								result->dlen=RDAstrlen(result->value.string_value)+1;
#endif
							}
							result->which=STRING;
							break;
						case BOOLNS:
						case CHARACTERS:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.string_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.float_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SHORTV:
						case SSHORTV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.short_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.integer_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case BUTTONS:
						default:
							prterr("Error field type [%d] invalid for cell value [%s].",field->type,field->name);
							break;
					}
					FreeNRDfield(field);
					Rfree(field);
				} else {
					prterr("Error Compute_Matrix_Linearly Failed....");
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.float_value=Rmalloc(sizeof(double));
					*result->value.float_value=0;
#ifdef NEW_EVAL
					result->dlen=0;
#endif
					result->which=NUMBER;
				}
				if(wordstring!=NULL) Rfree(wordstring);
			} else if(op=='r') /* STRING_LENGTH */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				fltval=RDAstrlen(wordstring);
				if(wordstring!=NULL) Rfree(wordstring);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.string_value=NULL;
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=fltval;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='u') /* DATE_TO_YEARDAY */
			{
			
				time_struct.tm_mon=0;
				time_struct.tm_mday=0;
				time_struct.tm_year=0;
				time_struct.tm_hour=0;
				time_struct.tm_min=0;
				time_struct.tm_sec=0;
				time_struct.tm_wday=0;
				time_struct.tm_yday=0;
				time_struct.tm_isdst=0;
				if(!isEMPTY(result->value.string_value))
				{
					temp=strtok(result->value.string_value," /:");
					if(temp!=NULL) time_struct.tm_mon=Ratoi(temp)-1;
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_mday=Ratoi(temp);
					temp=strtok(NULL," /:");
					if(temp!=NULL)
					{
						if(RDAstrlen(temp)==4)
						{
							time_struct.tm_year=Ratoi(temp)-1900;
						} else {
							time_struct.tm_year=Ratoi(temp);
						}
					}
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_hour=Ratoi(temp);
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_min=Ratoi(temp);
					temp=strtok(NULL," /:");
					if(temp!=NULL) time_struct.tm_sec=Ratoi(temp);
				}
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
#ifdef SJSDIAG
				prterr("level7(): BEFORE mktime(): mon=%d day=%d year=%d hour=%d min=%d sec=%d wday=%d yday=%d isdst=%d",
					time_struct.tm_mon,
					time_struct.tm_mday,
					time_struct.tm_year,
					time_struct.tm_hour,
					time_struct.tm_min,
					time_struct.tm_sec,
					time_struct.tm_wday,
					time_struct.tm_yday,
					time_struct.tm_isdst);
#endif 
				time_value=mktime(&time_struct);
#ifdef SJSDIAG
				prterr("level7(): AFTER mktime(): mon=%d day=%d year=%d hour=%d min=%d sec=%d wday=%d yday=%d isdst=%d",
					time_struct.tm_mon,
					time_struct.tm_mday,
					time_struct.tm_year,
					time_struct.tm_hour,
					time_struct.tm_min,
					time_struct.tm_sec,
					time_struct.tm_wday,
					time_struct.tm_yday,
					time_struct.tm_isdst);
#endif 
				time_struct=*localtime(&time_value);
#ifdef SJSDIAG
				prterr("level7(): AFTER localtime(): mon=%d day=%d year=%d hour=%d min=%d sec=%d wday=%d yday=%d isdst=%d",
					time_struct.tm_mon,
					time_struct.tm_mday,
					time_struct.tm_year,
					time_struct.tm_hour,
					time_struct.tm_min,
					time_struct.tm_sec,
					time_struct.tm_wday,
					time_struct.tm_yday,
					time_struct.tm_isdst);
#endif 

				*result->value.float_value=time_struct.tm_yday+1;
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='y') /* GENERIC_COUNTER */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.float_value=Rmalloc(sizeof(double));
				*result->value.float_value=COMPUTEGenericCounter(wordstring);
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				result->which=NUMBER;
			} else if(op=='_') /* MD5SUM */
			{
			
				InitMd5(&my_md5);
				if(RDAstrlen(result->value.string_value)>MD5_DIGEST_SIZE) 
				{
					result->value.string_value[MD5_DIGEST_SIZE]=0;
				}
				memset(my_i,0,MD5_DIGEST_SIZE);
				memcpy(my_i,result->value.string_value,RDAstrlen(result->value.string_value));
				Md5Update(&my_md5,my_i,RDAstrlen(result->value.string_value));
				Md5Final(&my_md5,digest);
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				result->value.string_value=Rmalloc(33);
				temp=result->value.string_value;
				for(i=0;i<16;++i)
				{
					sprintf(temp,"%02x",digest[i]);
					temp+=2;
				}
#ifdef NEW_EVAL
				result->dlen=MD5_DIGEST_SIZE+1;
#endif
				result->which=STRING;
			} else if(op==(-1)) /* READ_FILE */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp=stralloc(result->value.string_value);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);

#if !defined(LINUX_SUSE6_1)
					if(stat(temp,&statb)!=(-1))
#else
					if(_xstat(_STAT_VER,temp,&statb)!=(-1))
#endif
					{
#ifndef WIN32
#ifdef _POSIX_SOURCE
						if(!S_ISREG(statb.st_mode))
#else
						if((statb.st_mode&S_IFMT)!=S_IFREG)
#endif
						{
							prterr("Error File [%s] is not a regular readable file!",temp);
						} else {
#endif
						if(statb.st_size>0)
						{
							fp=fopen(temp,"r");
							if(fp!=NULL)
							{
								len=(int)(statb.st_size);
								result->value.string_value=Rmalloc(len+1);
#ifdef NEW_EVAL
								result->dlen=len+1;
#endif
								len=fread(result->value.string_value,sizeof(char),len,fp);
								if(!len)
								{
									prterr("Error:  Failed to read characters to the end of file [%s].",temp);
								}
								fclose(fp);
#ifdef USE_RDA_DIAGNOSTICS

								/* Temp Testing?  This may need removed for performance. */
								if(diageval)
								{
									fp=fopen("/tmp/readfile.out","wb");
									fwrite(result->value.string_value,sizeof(char),len,fp);
									fclose(fp);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								

							}
						} else {
							prterr("Error File [%s] is an empty file!",temp);
						}
#ifndef WIN32
						}
#endif
					}
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-2)) /* BASE64_ENCODE */
			{
			
				len=0;
				if(!isEMPTY(result->value.string_value))
				{
					base64_init_encodestate(&enstate);
					temp=Rmalloc(result->dlen);
					len=result->dlen-1;
					memcpy(temp,result->value.string_value,result->dlen);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=result->dlen*2;
					result->value.string_value=Rmalloc(result->dlen);
#else
					result->value.string_value=Rmalloc(RDAstrlen(temp)*2);
#endif
					temp1=Rmalloc(result->dlen*2);
					len=base64_encode_block(temp,len,temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
					memset(temp1,0,RDAstrlen(temp)+1);
					len=base64_encode_blockend(temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
#ifdef USE_RDA_DIAGNOSTICS

					/* Temp Testing?  This may need removed for performance. */
					if(diageval)
					{
						fp=fopen("/tmp/post-encode.b64","wb");
						fwrite(result->value.string_value,sizeof(char),strlen(result->value.string_value),fp);
						fclose(fp);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(temp1!=NULL) Rfree(temp1);
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-3)) /* BASE64_DECODE */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					base64_init_decodestate(&destate);
					temp=stralloc(result->value.string_value);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.string_value=Rmalloc(RDAstrlen(temp)+1);
#ifdef NEW_EVAL
					result->dlen=RDAstrlen(temp)+1;
#endif
					base64_decode_block(temp,RDAstrlen(temp),result->value.string_value,&destate);
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-4)) /* READ_FILE_HEX */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp=stralloc(result->value.string_value);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);

#if !defined(LINUX_SUSE6_1)
					if(stat(temp,&statb)!=(-1))
#else
					if(_xstat(_STAT_VER,temp,&statb)!=(-1))
#endif
					{
						if(statb.st_size>0)
						{
							fp=fopen(temp,"rb");
							if(fp!=NULL)
							{
								len=(int)(statb.st_size);
								temp1=Rmalloc(len+1);
								result->value.string_value=Rmalloc((len*2)+1);
#ifdef NEW_EVAL
								result->dlen=len*2+1;
#endif
								len=fread(temp1,sizeof(char),len,fp);
								if(!len)
								{
									prterr("Error:  Failed to read characters to the end of file [%s].",temp);
								}
								fclose(fp);
								count=0;
								for(x=0;x<len;++x)
								{
									sprintf(&result->value.string_value[count],"%02x",temp1[x]);
									count+=2;
								}
#ifdef USE_RDA_DIAGNOSTICS

								/* Temp Testing?  This may need removed for performance. */
								if(diageval)
								{
									fp=fopen("/tmp/readfilehex.out","wb");
									fwrite(result->value.string_value,sizeof(char),(len*2),fp);
									fclose(fp);
								}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
								if(temp1!=NULL) Rfree(temp1);
							}
						} else {
							prterr("Error File [%s] is an empty file!",temp);
						}
					}
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-5)) /* HEX_TO_BASE64_ENCODE */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp1=Rmalloc(result->dlen);
					memcpy(temp1,result->value.string_value,result->dlen);
					memset(result->value.string_value,0,result->dlen);
					count=0;
					for(x=0;x<result->dlen-1;x+=2)
					{
						sprintf(&result->value.string_value[count],"%c",HexToAscii(temp1[x],temp1[x+1]));
						++count;
					}
					if(temp1!=NULL) Rfree(temp1);
					base64_init_encodestate(&enstate);
					temp=Rmalloc(result->dlen);
					len=count;
					memcpy(temp,result->value.string_value,result->dlen);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=result->dlen*2;
					result->value.string_value=Rmalloc(result->dlen);
#else
					result->value.string_value=Rmalloc(RDAstrlen(temp)*2);
#endif
					temp1=Rmalloc(result->dlen*2);
					len=base64_encode_block(temp,len,temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
					memset(temp1,0,RDAstrlen(temp)+1);
					len=base64_encode_blockend(temp1,&enstate);
					sprintf(result->value.string_value,"%s%s",result->value.string_value,temp1);
#ifdef USE_RDA_DIAGNOSTICS

					/* Temp Testing?  This may need removed for performance. */
					if(diageval)
					{
						fp=fopen("/tmp/post-encode.b64","wb");
						fwrite(result->value.string_value,sizeof(char),strlen(result->value.string_value),fp);
						fclose(fp);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(temp1!=NULL) Rfree(temp1);
					if(temp!=NULL) Rfree(temp);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-6)) /* BASE64_DECODE_TO_HEX */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					base64_init_decodestate(&destate);
					temp=stralloc(result->value.string_value);
					len=RDAstrlen(temp)+1;
					temp1=Rmalloc(RDAstrlen(temp)+1);
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.string_value=Rmalloc(len*2+1);
#ifdef NEW_EVAL
					result->dlen=len*2+1;
#endif
					base64_decode_block(temp,len,temp1,&destate);
					count=0;
					for(x=0;x<len;++x)
					{
						sprintf(&result->value.string_value[count],"%02x",temp1[x]);
						count+=2;
					}
					if(temp!=NULL) Rfree(temp);
					if(temp1!=NULL) Rfree(temp1);
				} else {
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-7)) /* STRING_TO_HEX */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp1=Rmalloc(result->dlen);
					memcpy(temp1,result->value.string_value,result->dlen);
					len=result->dlen-1;
					count=0;
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
					result->value.string_value=Rmalloc(2*len+1);
#ifdef NEW_EVAL
					result->dlen=len*2+1;
#endif
					for(x=0;x<len;++x)
					{
						sprintf(&result->value.string_value[count],"%02x",temp1[x]);
						count+=2;
					}
					if(temp1!=NULL) Rfree(temp1);
				} else {
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==-9) /* DOLLARS_TO_TEXT */
			{
			
				memset(stemp,0,101);
				sprintf(stemp,"%.02f",*result->value.float_value/100);
				result->value.string_value=stralloc(stemp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
				memset(stemp,0,101);
				result->which=STRING;
			} else if(op==(-8)) /* HEX_TO_STRING */
			{
			
				if(!isEMPTY(result->value.string_value))
				{
					temp1=Rmalloc(result->dlen);
					memcpy(temp1,result->value.string_value,result->dlen);
					memset(result->value.string_value,0,result->dlen);
					count=0;
					for(x=0;x<result->dlen-1;x+=2)
					{
						sprintf(&result->value.string_value[count],"%c",HexToAscii(temp1[x],temp1[x+1]));
						++count;
					}
					if(temp1!=NULL) Rfree(temp1);
				} else {
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
				result->which=STRING;
			} else if(op==(-12)) /* Wt BCrypt Value */
			{
				if(BCryptValue!=NULL)
				{
					temp=BCryptValue(result->value.string_value);
					if(result->value.string_value!=NULL) Rfree(result->value.string_value);
					result->value.string_value=temp;
#ifdef NEW_EVAL
					result->dlen=RDAstrlen(temp)+1;
#endif
					result->which=STRING;
				}
			} else if(op==(-11)) /* READ_FILE_TO_BASE_SIXTYFOUR */
			{
				temp=encode_base64_from_file(result->value.string_value);
				if(result->value.string_value!=NULL) Rfree(result->value.string_value);
				result->value.string_value=temp;
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(temp)+1;
#endif
				result->which=STRING;
			
			} else if(op=='w') /* GENERIC SETUP VARIABLE */
			{
			
				wordstring=reset_quotes(result->value.string_value);
				field=COMPUTEGenericSetup(wordstring,subfunc,arg);
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
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case ZIPCODE:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
#ifdef NEW_EVAL
							result->dlen=0;
#endif
							if(field->data.string_value!=NULL)
							{
								result->value.string_value=stralloc(field->data.string_value);
#ifdef NEW_EVAL
								result->dlen=RDAstrlen(result->value.string_value)+1;
#endif
							}
							result->which=STRING;
							break;
						case BOOLNS:
						case CHARACTERS:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.string_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.float_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SHORTV:
						case SSHORTV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.short_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(result->value.string_value!=NULL)
								Rfree(result->value.string_value);
							result->value.float_value=Rmalloc(sizeof(double));
							*result->value.float_value=*field->data.integer_value;
#ifdef NEW_EVAL
							result->dlen=sizeof(double);
#endif
							result->which=NUMBER;
							break;
						case BUTTONS:
						default:
							prterr("Error field type [%d] invalid for cell value [%s].",field->type,field->name);
							break;
					}
					FreeNRDfield(field);
					Rfree(field);
				} else {
					prterr("Error Generic_Setup_Variable Failed....");
					if(result->value.string_value!=NULL)
						Rfree(result->value.string_value);
					result->value.float_value=Rmalloc(sizeof(double));
					*result->value.float_value=0;
#ifdef NEW_EVAL
					result->dlen=0;
#endif
					result->which=NUMBER;
				}
			
				if(wordstring!=NULL) Rfree(wordstring);
			}
			
		}
			
	} else {
			
		if(op=='(')
		{
			
#ifdef XXX 
			prterr("op [%c] [%d] ",op,op);prterr("level7 after ( result ");SEERDAresult(result);
#endif
			get_token(result);
			level2(result,subfunc,arg);
			if(*tmp->token!=')')
			{
#ifdef ZZZ
				prterr("Error Unbalanced Token [%c] in Expression [%s].",*tmp->token,tmp->prog);
#endif
				prterr("Error Unbalanced Token [%c]",op);
				prterr("      in Expression [%s].\n",tmp->prog);
			}
#ifdef XXX 
			prterr("op [%c] [%d] ",op,op);prterr("level7 after ) error result ");SEERDAresult(result);
#endif
			get_token(result);
#ifdef XXX 
			prterr("op [%c] [%d] ",op,op);prterr("level7 after get_token result ");SEERDAresult(result);
#endif
		} else {
			
			if(result->which==NUMBER)
			{
				if(result->value.float_value==NULL)
				{
					result->value.float_value=Rmalloc(sizeof(double));
					*result->value.float_value=0;
				}
#ifdef NEW_EVAL
				result->dlen=sizeof(double);
#endif
				if(!isEMPTY(tmp->token))
				{
					*result->value.float_value=atof(tmp->token);
				} else {
					*result->value.float_value=0;
				}
			} else if(result->which==STRING)
			{
			
				if(result->value.string_value!=NULL)
					Rfree(result->value.string_value);
				if(!isEMPTY(tmp->token)) 
				{
					result->value.string_value=stralloc(tmp->token);
#ifdef NEW_EVAL
					result->dlen=RDAstrlen(tmp->token)+1;
#endif
				} else {
					result->value.string_value=NULL;
#ifdef NEW_EVAL
					result->dlen=0;
#endif
				}
			}
			get_token(result);
			
		}
			
	}
			
#ifdef XXX 
	prterr("op [%c] [%d]  ",op,op);prterr("level7 result ");SEERDAresult(result);
#endif
}
short is_in(char ch,char *s)
{
	while(*s) if(*s++==ch) return(TRUE);
	return(FALSE);	
}
short iswhite(char c)
{
	if(c==' ' || c==9) return(TRUE);
		else return(FALSE);
}
short isdelim(char c)
{
	if(is_in(c,func) || c==9 || c=='\r' || c==0 || c==' ' || c<0) return(TRUE);
		else return(FALSE);
}
void xget_token(RDAresult *result,int line,char *file)
{
	char *temp,*temp2;
	RDAeval *tmp;

	tmp=result->eval;
	temp=tmp->token;
#ifdef XXX
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG get_token reading token [%s] at line [%d] program [%s] from expression: \n",tmp->token,line,file);
		temp2=tmp->prog;
		while(TRUE)
		{
			if(RDAstrlen(temp2)<256)
			{
				prterr("%s\n",temp2);
				break;
			} else {
				prterr("%.256s\n",temp2);
			}
			temp2=temp2+256;
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#endif
	if((is_in(*tmp->prog,func)) || *tmp->prog<0) /* Reading Operator's and Functions */
	{
		*temp=*tmp->prog;
		++tmp->prog;
		if(*temp=='=')
		{
			if(*tmp->prog=='>')
			{
				*temp='$';
				++tmp->prog;
			} else if(*tmp->prog=='<')
			{
				*temp='#';
				++tmp->prog;
			} else if(*tmp->prog=='!')
			{
				*temp='@';
				++tmp->prog;
			}
		} else if(*temp=='>')
		{
			if(*tmp->prog=='=') 
			{
				*temp='$';
				++tmp->prog;
			}
		} else if(*temp=='<')
		{
			if(*tmp->prog=='=') 
			{
				*temp='#';
				++tmp->prog;
			}
		} else if(*temp=='!')
		{
			if(*tmp->prog=='=') 
			{
				*temp='@';
				++tmp->prog;
			}
		}
		++temp;
/* made this conditional */
		tmp->type=OPERATOR;
	} else if(*tmp->prog=='\"') /* Reading Standard strings */
	{
		++tmp->prog;
		while(TRUE) 
		{
			temp2=tmp->prog;
			--temp2;
			if(*tmp->prog=='\"' && *temp2!='\\')
			{
				break;
			}
			*temp=*tmp->prog;
			++temp;++tmp->prog;
		}	
		++tmp->prog;
		if(result->which==NUMBER)
		{
			if(result->value.float_value!=NULL) Rfree(result->value.float_value);
			result->value.string_value=NULL;
			result->which=STRING;
		}
		tmp->type=STRING;
	} else if(isdigit(*tmp->prog)) /* Reading Numericals */
	{
		while(!isdelim(*tmp->prog) || *tmp->prog=='.') 
			*temp++=*tmp->prog++;
		if(result->which==STRING)
		{
			if(result->value.string_value!=NULL) Rfree(result->value.string_value);
			result->value.float_value=Rmalloc(sizeof(double));
			*result->value.float_value=0;
			result->which=NUMBER;
		}
		tmp->type=NUMBER;
	}
	*temp=0;
#ifdef XXX
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG get_token Returning Token [%s] which [%d] at line [%d] program [%s] from expression: \n",tmp->token,result->which,line,file);
		temp2=tmp->prog;
		while(TRUE)
		{
			if(RDAstrlen(temp2)<256)
			{
				prterr("%s\n",temp2);
				break;
			} else {
				prterr("%.256s\n",temp2);
			}
			temp2=temp2+256;
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#endif
}
RDAresult *xPerformeval(short type,char *string,void (*subfunc)(...),void *arg,int line,char *file)
{
	RDAresult *result=NULL;
	RDAeval *eval=NULL;
	char *terminate_tab_string=NULL,*string1=NULL,*string2=NULL;
	short t=FALSE;
#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	string2=stralloc(string);
	terminate_tab_string=strchr(string2,'\t');
	if(terminate_tab_string!=NULL) *terminate_tab_string=0;
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG eval Evaluating Type [%d] at line [%d] program [%s].",type,line,file);
		temp=string2;
		prterr("Expression:\n");
		while(TRUE)
		{
			if(RDAstrlen(temp)<256)
			{
				prterr("%s\n",temp);
				break;
			} else { 
				prterr("%.256s\n",temp);
			}
			temp=temp+256;
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	string1=OpenRDA_translate(string2,subfunc,arg,TRUE);
	if(string2!=NULL) Rfree(string2);
	terminate_tab_string=strchr(string1,'\t');
	if(terminate_tab_string!=NULL) *terminate_tab_string=0;
	eval=RDAevalNEW(string1);
	result=RDAresultNEW(type,eval);
	eval->prog=string1;
	get_exp(result,subfunc,arg);
	if(type!=result->which)
	{
#ifdef USE_RDA_DIAGNOSTICS

		if(diageval)
		{
			prterr("Matching Types.");
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(result->which==STRING)
		{
			if(!isEMPTY(result->value.string_value))
			{
				Rfree(result->value.string_value);
				t=TRUE;
			} else t=FALSE;
			result->value.string_value=NULL;	
			result->value.float_value=Rmalloc(sizeof(double));
			*result->value.float_value=t;
#ifdef NEW_EVAL
			result->dlen=sizeof(double);
#endif
			result->which=NUMBER;
		} else {
			memset(stemp,0,101);
			if(result->value.float_value!=NULL)
			{
				sprintf(stemp,"%.08f",*result->value.float_value);
				Rfree(result->value.float_value);
			}
			if(!isEMPTY(stemp))
			{
				result->value.string_value=stralloc(stemp);
#ifdef NEW_EVAL
				result->dlen=RDAstrlen(stemp)+1;
#endif
			} else {
				result->value.string_value=NULL;
#ifdef NEW_EVAL
				result->dlen=0;
#endif
			}
			result->which=STRING;
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diageval)
	{
		prterr("DIAG xeval Evaluated Expression:");
/*
		if(!isEMPTY(string1))
		{
			string2=string1;
			while(TRUE)
			{
				if(RDAstrlen(string2)<256)
				{
					prterr("%s\n",string2);
					break;
				} else { 
					prterr("%.256s\n",string2);
				}
				string2=string2+256;
			}
		} else prterr("(NULL)\n");
*/
		prterr("     Type [%d] ",type);
		if(result->which==STRING)
		{
			prterr("Returning Result [%s] at line [%d] program [%s].\n",(result->value.string_value!=NULL?result->value.string_value:""),line,file);
		} else if(result->which==NUMBER)
		{
			prterr("Returning Result [%.08f] at line [%d] program [%s].\n",*result->value.float_value,line,file);
		}
	}
/*
	if(diageval)
	{
		if(result->which==STRING)
		{
			prterr("DIAG xeval Evaluated Expression [%s] Type [%d] Returning Result [%s] at line [%d] program [%s].",string1,type,(result->value.string_value!=NULL ? result->value.string_value:""),line,file);
		} else if(result->which==NUMBER)
		{
			prterr("DIAG xeval Evaluated Expression [%s] Type [%d] Returning Result [%.08f] at line [%d] program [%s].",string1,type,*result->value.float_value,line,file);
		}
	}
*/
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(eval!=NULL) FreeRDAeval(eval);
	if(string1!=NULL) Rfree(string1);
	return(result);
}
short xcountifs(char *expression,int line,char *file)
{
	char *holdstring=NULL,*temp=NULL;
	short retval=1,x;

#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG countifs Counting Breaks to Skip at line [%d] program [%s].",line,file);
		temp=expression;
		prterr("Expression:\n");
		while(TRUE)
		{
			if(RDAstrlen(temp)<256)
			{
				prterr("%s\n",temp);
				break;
			} else {
				prterr("%.256s\n",temp);
			}
			temp=temp+256;
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	holdstring=expression;
	while(TRUE)
	{
		holdstring=strchr(holdstring,'\t');
		if(holdstring==NULL)
		{
			prterr("Error missing expression in nested if's at line [%d] program [%s].",line,file); 
			temp=expression;
			prterr("Expression:\n");
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else {
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
			return(-1);
		}
		++retval;
		++holdstring;
		if(!strncmp(holdstring,"IF",2))
		{
			x=countifs(holdstring);
			if(x<0) 
			{
				prterr("Error missing expression in nested if's at line [%d] program [%s].",line,file); 
				temp=expression;
				prterr("Expression:\n");
				while(TRUE)
				{
					if(RDAstrlen(temp)<256)
					{
						prterr("%s\n",temp);
						break;
					} else { 
						prterr("%.256s\n",temp);
					}
					temp=temp+256;
				}
				return(-1);
			}
			while(x)
			{
				holdstring=strchr(holdstring,'\t');
				if(holdstring==NULL)
				{
					prterr("Error missing expression in nested if's at line [%d] program [%s].",line,file); 
					temp=expression;
					prterr("Expression:\n");
					while(TRUE)
					{
						if(RDAstrlen(temp)<256)
						{
							prterr("%s\n",temp);
							break;
						} else {
							prterr("%.256s\n",temp);
						}
						temp=temp+256;
					}
					break;
				}
				++holdstring;
				++retval;
				--x;
			}
			if(x>0) 
			{
				prterr("Error Inaccurate count of if-then-else elements.");
				break;
			}
		}
		if(!strncmp(holdstring,"FI",2)) break;
	}		
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG countifs Returning Count of [%d] to Skip.",retval);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(retval);
}
RDAresult *xStepThroughIF(short type,char *expression,void (*subfunc)(...),void *arg,int line,char *file)
{
	char *holdstring=NULL;
	RDAresult *result=NULL;
	short count=0;
#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	if(isEMPTY(expression))
	{
		prterr("Error no expression no evaluate in StepThroughIF at line [%d] program [%s].",line,file);
		return(result);
	}
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG StepThroughIF Evaluating IF THEN ELSE Construct for Type [%d] at line [%d] program [%s].",type,line,file);
		temp=expression;
		prterr("Expression:\n");
		while(TRUE)
		{
			if(RDAstrlen(temp)<256)
			{
				prterr("%s\n",temp);
				break;
			} else { 
				prterr("%.256s\n",temp);
			}
			temp=temp+256;
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	holdstring=expression;
	if(!strncmp(holdstring,"IF",2))
	{ 
		holdstring=strchr(holdstring,'\t');
		if(holdstring==NULL)
		{
			prterr("Error missing expression following if at line [%d] program [%s].",line,file); 
			return(result);
		}
		++holdstring;
		result=Performeval(NUMBER,holdstring,subfunc,arg);
		if(result==NULL)
		{
			prterr("Error:  Fatal Error expecting an expression [%s] at line [%d] program [%s].",(expression!=NULL ? expression:""),line,file);
			return(NULL);
		}
		if(*result->value.float_value)
		{
			if(result!=NULL) FreeRDAresult(result);
			holdstring=strchr(holdstring,'\t'); /* then */
			if(holdstring==NULL) 
			{
				prterr("Error Then Missing in expression [%s] at line [%d] program [%s].",expression,line,file); 
				return(NULL);
			}
			++holdstring;
			holdstring=strchr(holdstring,'\t'); /* expression following then */
			if(holdstring==NULL) 
			{
				prterr("Error Then Expression Missing in expression [%s] at line [%d] program [%s].",expression,line,file); 
				return(NULL);
			}
			++holdstring;
			return(StepThroughIF(type,holdstring,subfunc,arg));
		} else {
			if(result!=NULL) FreeRDAresult(result);
			holdstring=strchr(holdstring,'\t'); /* then */
			if(holdstring==NULL) 
			{
				prterr("Error Then Missing in expression [%s] at line [%d] program [%s].",expression,line,file); 
				return(NULL);
			}
			++holdstring;
			holdstring=strchr(holdstring,'\t'); /* expression following then */
			if(holdstring==NULL) 
			{
				prterr("Error Then Expression Missing in expression [%s] at line [%d] program [%s].",expression,line,file); 
				return(NULL);
			}
			++holdstring;
			if(!strncmp(holdstring,"IF",2))
			{
				count=countifs(holdstring);
				if(count<0)
				{
					prterr("Error missing expression in nested if's in [%s] at line [%d] program [%s].",expression,line,file); 
					return(NULL);
				}
				--count;
				while(count)
				{
					holdstring=strchr(holdstring,'\t');
					if(holdstring==NULL)
					{
						prterr("Error missing expression in nested if's in [%s] at line [%d] program [%s].",expression,line,file); 
						return(NULL);
					}
					++holdstring;
					--count;
				}
			}
			holdstring=strchr(holdstring,'\t'); /* else */
			if(holdstring==NULL) 
			{
				prterr("Error Else Missing in expression [%s] at line [%d] program [%s].",expression,line,file); 
				return(NULL);
			}
			++holdstring;
			holdstring=strchr(holdstring,'\t'); /* expression following else */
			if(holdstring==NULL) 
			{
				prterr("Error Else Expression Missing in expression [%s] at line [%d] program [%s].",expression,line,file); 
				return(NULL);
			}
			++holdstring;
			return(StepThroughIF(type,holdstring,subfunc,arg));
		}
	} else if(!strncmp(&holdstring[0],"ELSE",4))
	{
		holdstring=strchr(holdstring,'\t'); /* expression following else */
		++holdstring;
		return(StepThroughIF(type,holdstring,subfunc,arg));
	} else if(!strncmp(&holdstring[0],"FI",2))
	{
		return(NULL);
	} else if(!strncmp(&holdstring[0],"RETURN_VALUE",12))
	{
		holdstring+=13;
	} 
	if(holdstring!=NULL)
	{
		return(Performeval(type,holdstring,subfunc,arg));
	} else {
		return(NULL);
	}
}
double xEVALUATEdbl(char *expression,void (*subfunc)(...),void *arg,short do_presort,int line,char *file)
{
	double ret_val=0;
	char *tmp=NULL;
	RDAresult *result=NULL;
#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEdbl Evaluating for Double Value at line [%d] file [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(expression))
	{
#ifdef USE_RDA_DIAGNOSTICS

		if(diageval || diageval_field)
		{
			temp=expression;
			prterr("Expression:\n");
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else { 
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(do_presort==TRUE)
		{
			tmp=OpenRDA_translate(expression,subfunc,arg,FALSE);
		} else tmp=expression;
		if(!isEMPTY(tmp))
		{
			result=StepThroughIF(NUMBER,tmp,subfunc,arg);
			if(result!=NULL)
			{
				ret_val=*result->value.float_value;
				FreeRDAresult(result);
			} 
			if(tmp!=NULL && do_presort==TRUE) Rfree(tmp);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEdbl Returning [%.08f] to line [%d] program [%s].",ret_val,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(ret_val);
}
short xEVALUATEsht(char *expression,void (*subfunc)(...),void *arg,short do_presort,int line,char *file)
{
	short ret_val=0;
	char *tmp=NULL;
	RDAresult *result=NULL;
#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEsht Evaluating for Short Value at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(expression))
	{
#ifdef USE_RDA_DIAGNOSTICS

		if(diageval || diageval_field)
		{
			temp=expression;
			prterr("Expression:\n");
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else { 
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(do_presort==TRUE)
		{
			tmp=OpenRDA_translate(expression,subfunc,arg,FALSE);
		} else tmp=expression;
		if(!isEMPTY(tmp))
		{
			result=StepThroughIF(NUMBER,tmp,subfunc,arg);
			if(result!=NULL)
			{
				while(*result->value.float_value>(SHRT_MAX)) 
					*result->value.float_value-=(SHRT_MAX);
				ret_val=(short)*result->value.float_value;
				FreeRDAresult(result);
			}
			if(tmp!=NULL && do_presort==TRUE) Rfree(tmp);
		} 
	}
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEsht Returning [%d] to line [%d] program [%s].",ret_val,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(ret_val);
}
int xEVALUATEint(char *expression,void (*subfunc)(...),void *arg,short do_presort,int line,char *file)
{
	int ret_val=0;
	char *tmp=NULL;
	RDAresult *result=NULL;
#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEint Evaluating for Integer Value at line [%d] file [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(expression))
	{
#ifdef USE_RDA_DIAGNOSTICS

		if(diageval || diageval_field)
		{
			temp=expression;
			prterr("Expression:\n");
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else { 
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(do_presort==TRUE)
		{
			tmp=OpenRDA_translate(expression,subfunc,arg,FALSE);
		} else tmp=expression;
		if(!isEMPTY(tmp))
		{
			result=StepThroughIF(NUMBER,tmp,subfunc,arg);
			if(result!=NULL)
			{
				while(*result->value.float_value>(INT_MAX)) 
					*result->value.float_value-=(INT_MAX);
				ret_val=(int)*result->value.float_value;
				FreeRDAresult(result);
			} 
			if(tmp!=NULL && do_presort==TRUE) Rfree(tmp);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEint Returning [%d] to line [%d] program [%s].",ret_val,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(ret_val);
}
short xEVALUATEbol(char *expression,void (*subfunc)(...),void *arg,short do_presort,int line,char *file)
{
	short ret_val=0;
	char *tmp=NULL;
	RDAresult *result=NULL;
#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEbol Evaluating for Boolean Value at line [%d] file [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(expression))
	{
#ifdef USE_RDA_DIAGNOSTICS

		if(diageval || diageval_field)
		{
			prterr("DIAG EVALUATEbol Evaluating for Boolean Value at line [%d] file [%s].",line,file);
			temp=expression;
			prterr("Expression:\n");
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else { 
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(do_presort==TRUE)
		{
			tmp=OpenRDA_translate(expression,subfunc,arg,FALSE);
		} else tmp=expression;
		if(!isEMPTY(tmp))
		{
			result=StepThroughIF(NUMBER,tmp,subfunc,arg);
			if(result!=NULL)
			{
				ret_val=*result->value.short_value;
				if(*result->value.float_value) ret_val=TRUE;
					else ret_val=FALSE;
				FreeRDAresult(result);
			} 
			if(tmp!=NULL && do_presort==TRUE) Rfree(tmp);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEbol Returning [%s] to line [%d] program [%s].",(ret_val ? "True":"False"),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(ret_val);
}
char *xEVALUATEstr(char *expression,void (*subfunc)(...),void *arg,short do_presort,int line,char *file)
{
	char *ret_val=NULL;
	char *tmp=NULL;
	RDAresult *result=NULL;
#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEstr Evaluating for String Value [%.80s] at line [%d] file [%s].",(expression!=NULL?expression:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(expression))
	{
#ifdef USE_RDA_DIAGNOSTICS

		if(diageval || diageval_field)
		{
			temp=expression;
			prterr("Expression:\n");
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else { 
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(do_presort==TRUE)
		{
			tmp=OpenRDA_translate(expression,subfunc,arg,FALSE);
		} else tmp=expression;
		if(!isEMPTY(tmp))
		{
			result=StepThroughIF(STRING,tmp,subfunc,arg);
			if(result!=NULL)
			{
				if(result->value.string_value!=NULL) 
				{
/*
					ret_val=stralloc(result->value.string_value);
*/
					ret_val=reset_quotes(result->value.string_value);
				}
				FreeRDAresult(result);
			}	 
			if(tmp!=NULL && do_presort==TRUE) Rfree(tmp);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval || diageval_field)
	{
		prterr("DIAG EVALUATEstr Returning [%s] to line [%d] program [%s].",ret_val,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(ret_val);
}
