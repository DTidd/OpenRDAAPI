/* diagscrn.c - Diagnostic Screen Functions */
#include<mix.hpp>
#ifdef UNIXWARE7
#include <ctime>
#endif
#ifdef UNIXWARE21
#include <ctime>
#endif

int diagnostic_selected=0,diagnostic_read=0,diagnostic_total=0;
short suspend_diagnostic=FALSE,abort_diagnostic=FALSE;
short seconds_2_increment=5;
time_t dstart_time,delapsed_time,destimated_time,destimated_completion;
time_t dcurrent_time,dremaining_time,est_duration;
char *diagvariable=NULL;

void stop_diagnostic(RDArsrc *parent)
{
	abort_diagnostic=TRUE;
	FINDRSCSETSTRING(parent,"STATUS","Aborted By User");
	updatersrc(parent,"STATUS");
}
void xRtime(time_t x,char **string)
{
	int hours=0,minutes=0,seconds=0,hold=0;
	int t;
	char strtemp[101];

	if(x<0) x=0;
	hours=(x/3600);
	hold=x-(hours*3600);
	minutes=(hold/60);
	seconds=hold-(minutes*60);
	sprintf(strtemp,"%d:%02d:%02d",hours,minutes,seconds);
	t=RDAstrlen(strtemp);
	if(RDAstrlen(*string)<t)
	{
		*string=Rrealloc(*string,t+1);
	}
	memcpy(*string,strtemp,t+1);
	hours=0;minutes=0;hold=0;seconds=0;
}
short update_diagnostic(RDArsrc *r,short selected)
{
	RDArmem *member;
	int x;
	int tmp_selected=0;
	time_t tmp_time=0;
	time_t dest_comp,dest_dur;
	char strtemp[101];

	if(!IsScreenDisplayed(r)) return(FALSE);
	if(!abort_diagnostic)
	{
	tmp_selected=diagnostic_selected;
	if(selected) ++diagnostic_selected;
	++diagnostic_read;
	time(&tmp_time);
	if(r!=NULL && ((tmp_time-dcurrent_time)>=seconds_2_increment))
	{
		checkforinterrupt(r);
		if(abort_diagnostic) return(TRUE);
		if(!suspend_diagnostic)
		{
			if(tmp_selected!=diagnostic_selected)
			{
				memset(strtemp,0,101);
				sprintf(strtemp,"%-d",diagnostic_selected);
				x=FINDRSC(r,"SELECTED");
				if(x!=(-1))
				{
					member=r->rscs+x;
					QUICKALLOC(member->value.string_value,member->dlen,(RDAstrlen(strtemp)+1));
					memcpy(member->value.string_value,strtemp,(RDAstrlen(strtemp))+1);
					updatemember(member);
				}
			}
			memset(strtemp,0,101);
			sprintf(strtemp,"%-d",diagnostic_read);
			x=FINDRSC(r,"TOTAL READ");
			if(x!=(-1))
			{
				member=r->rscs+x;
				QUICKALLOC(member->value.string_value,member->dlen,(RDAstrlen(strtemp)+1));
				memcpy(member->value.string_value,strtemp,(RDAstrlen(strtemp)+1));
				updatemember(member);
			}
			FINDRSCSETINT(r,"PROGRESS BAR",diagnostic_read);
			updatersrc(r,"PROGRESS BAR");
			memset(strtemp,0,101);
			time(&dcurrent_time);
			memset(strtemp,0,101);
			x=FINDRSC(r,"CURRENT TIME");
			if(x!=(-1))
			{
#ifndef WIN32
#ifdef _POSIX_SOURCE
				strftime(strtemp,sizeof(strtemp),"%T",localtime(&dcurrent_time));
#else
				ascftime(strtemp,"%T",localtime(&dcurrent_time));
#endif
#endif
#ifdef WIN32
				strftime(strtemp,sizeof(strtemp),"%X",localtime(&dcurrent_time));
#endif
				member=r->rscs+x;
				QUICKALLOC(member->value.string_value,member->dlen,(RDAstrlen(strtemp)+1));
				memcpy(member->value.string_value,strtemp,(RDAstrlen(strtemp)+1));
				updatemember(member);
			}
			memset(strtemp,0,101);
			dest_comp=destimated_completion;
			dest_dur=est_duration;
			delapsed_time=dcurrent_time-dstart_time;
/*lint -e790 */
/*lint -e524 */
			if(diagnostic_read!=0)
			{
				est_duration=(double)(delapsed_time*diagnostic_total)/diagnostic_read;
			} else est_duration=0;
/*lint +e790 */
/*lint +e524 */
			dremaining_time=est_duration-delapsed_time;
			destimated_completion=dstart_time+est_duration;
			x=FINDRSC(r,"ELAPSED TIME");
			if(x!=(-1))
			{
				member=r->rscs+x;
				if(member->value.string_value==NULL) 
				{
					member->value.string_value=Rmalloc(9);
					member->dlen=8;
				}
				xRtime(delapsed_time,&member->value.string_value);
				updatemember(member);
			}
			memset(strtemp,0,101);
			x=FINDRSC(r,"REMAINING TIME");
			if(x!=(-1))
			{
				member=r->rscs+x;
				if(member->value.string_value==NULL) 
				{
					member->value.string_value=Rmalloc(9);
					member->dlen=8;
				}
				xRtime(dremaining_time,&member->value.string_value);
				updatemember(member);
			}
			memset(strtemp,0,101);
			if(dest_dur!=est_duration)
			{
				x=FINDRSC(r,"ESTIMATED DURATION");
				if(x!=(-1))
				{
					member=r->rscs+x;
					if(member->value.string_value==NULL) 
					{
						member->value.string_value=Rmalloc(9);
						member->dlen=8;
					}
					xRtime(est_duration,&member->value.string_value);
					updatemember(member);
				}
			}
			memset(strtemp,0,101);
			if(dest_comp!=destimated_completion)
			{
				x=FINDRSC(r,"ESTIMATED COMPLETION TIME");
				if(x!=(-1))
				{
#ifndef WIN32
#ifdef _POSIX_SOURCE
					strftime(strtemp,sizeof(strtemp),"%T",
						localtime(&destimated_completion));
#else
					ascftime(strtemp,"%T",
						localtime(&destimated_completion));
#endif
#endif
#ifdef WIN32
					strftime(strtemp,sizeof(strtemp),"%X",
						localtime(&destimated_completion));
#endif
					member=r->rscs+x;
					QUICKALLOC(member->value.string_value,member->dlen,(RDAstrlen(strtemp)+1));
					memcpy(member->value.string_value,strtemp,(RDAstrlen(strtemp)+1));
					updatemember(member);
				}
			}
		}
		ForceWindowUpdate(r);
	}
	if(diagvariable!=NULL)
	{
		if(!isEMPTY(diagvariable)) x=Ratoi(diagvariable);
			else x=0;
		if(x!=0 && x==diagnostic_read) std::exit;
	}
	}
	if(abort_diagnostic) return(TRUE);
		else return(FALSE);
}
void resumediagnostic(RDArsrc *parent)
{
	suspend_diagnostic=FALSE;
	FINDRSCSETSTRING(parent,"STATUS","Running");
	updatersrc(parent,"STATUS");
}
void suspenddiagnostic(RDArsrc *parent)
{
	suspend_diagnostic=TRUE;
	FINDRSCSETSTRING(parent,"STATUS","Suspended");
	updatersrc(parent,"STATUS");
}
void setincrement(RDArsrc *r)
{
	readwidget(r,"INCREMENT");
	FINDRSCGETSHORT(r,"INCREMENT",&seconds_2_increment);
	updatersrc(r,"INCREMENT");
}
RDArsrc *xdiagscrn(short fileno,char *screen,char *module,char *name,void (*quitfunc)(...),int counters)
{
	RDArsrc *tmp=NULL;
	char *temp=NULL,*dstatus=NULL;
	char strtemp[101];
	int x=0;
	RDArmem *member=NULL;

	abort_diagnostic=FALSE;
	diagvariable=RDA_GetEnv("DIAGVAR");
	if(RDAstrlen(diagvariable)<1) diagvariable=NULL;
	diagnostic_selected=0,diagnostic_read=0,diagnostic_total=0;
	suspend_diagnostic=FALSE;
	time(&dstart_time);
	time(&destimated_completion);
	delapsed_time=0;
	dremaining_time=0;
	est_duration=0;
	dcurrent_time=dstart_time;
	destimated_time=0;
	tmp=RDArsrcNEW(module,screen);
	suspend_diagnostic=FALSE;
	if(fileno!=(-1)) diagnostic_total=SIZNRD(fileno);
		else diagnostic_total=counters;
	dstatus=stralloc("Running");
	addstdrsrc(tmp,"PROCESS NAME",VARIABLETEXT,0,name,FALSE);
	addstdrsrc(tmp,"STATUS",VARIABLETEXT,0,dstatus,FALSE);
	if(dstatus!=NULL) Rfree(dstatus);
	memset(strtemp,0,101);
	sprintf(strtemp,"%-d",diagnostic_selected);
	addstdrsrc(tmp,"SELECTED",VARIABLETEXT,15,strtemp,FALSE);
	memset(strtemp,0,101);
	sprintf(strtemp,"%-d",diagnostic_read);
	addstdrsrc(tmp,"TOTAL READ",VARIABLETEXT,15,strtemp,FALSE);
	x=0;
	addstdrsrc(tmp,"PROGRESS BAR",PROGRESS_BAR,6,x,FALSE);
	x=FINDRSC(tmp,"PROGRESS BAR");
	if(x!=(-1)) 
	{
		member=tmp->rscs+x;
		member->items=diagnostic_total;
	}
	memset(strtemp,0,101);
	sprintf(strtemp,"%-d",diagnostic_total);
	addstdrsrc(tmp,"TOTAL ALL",VARIABLETEXT,15,strtemp,FALSE);
	memset(strtemp,0,101);
#ifndef WIN32
#ifdef _POSIX_SOURCE
	strftime(strtemp,sizeof(strtemp),"%T",localtime(&dstart_time));
#else
	ascftime(strtemp,"%T",localtime(&dstart_time));
#endif
#endif
#ifdef WIN32
	strftime(strtemp,sizeof(strtemp),"%X",localtime(&dstart_time));
#endif
	temp=stralloc(strtemp);
	addstdrsrc(tmp,"START TIME",VARIABLETEXT,8,temp,FALSE);
	addstdrsrc(tmp,"CURRENT TIME",VARIABLETEXT,8,temp,FALSE);
	strcpy(strtemp,"00:00:00");
	addstdrsrc(tmp,"ELAPSED TIME",VARIABLETEXT,8,strtemp,FALSE);
	addstdrsrc(tmp,"ESTIMATED COMPLETION TIME",VARIABLETEXT,8,temp,FALSE);
	addstdrsrc(tmp,"ESTIMATED DURATION",VARIABLETEXT,8,strtemp,FALSE);
	addstdrsrc(tmp,"REMAINING TIME",VARIABLETEXT,8,strtemp,FALSE);
	if(temp!=NULL) Rfree(temp);
	temp=NULL;
	seconds_2_increment=5;
	addstdrsrc(tmp,"INCREMENT",SHORTV,3,&seconds_2_increment,TRUE);
	FINDRSCSETFUNC(tmp,"INCREMENT",setincrement,tmp);
	addbtnrsrc(tmp,"RESUME",TRUE,resumediagnostic,NULL);
	addbtnrsrc(tmp,"SUSPEND",TRUE,suspenddiagnostic,NULL);
	if(quitfunc!=NULL) 
	{
		addrfkwrsrc(tmp,"QUIT",TRUE,quitfunc,NULL);
		FINDRSCSETINPUTFOCUS(tmp,"QUIT");
	}
	addbtnrsrc(tmp,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(tmp,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	return(tmp);
}
void xcreate_diagnostics(RDArsrc *tmprsrc,char *screen,short fileno,
	char *module,void (*quitfunc)(...),short modal,char *name,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG ctreate_diagnostics for Screen [%s] Module [%s] File [%d] [%s] at line [%d] program [%s].",screen,module,fileno,FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmprsrc=diagscrn(fileno,screen,module,name,quitfunc);
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,(modal ? TRUE:FALSE)))
		{
		}
	}
}
