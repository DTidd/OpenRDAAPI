#include <mix.hpp>

int xGETCURRENTYEAR(int fiscal_starting_month,int line,char *file)
{
	char *tmp=NULL;
	int period=0;
	time_t ltime;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GETCURRENTYEAR function called with argument [%d] in program [%s] line [%d]",fiscal_starting_month,file,line);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	time(&ltime);
	if(fiscal_starting_month>(-1))
	{
		tmp=Rmalloc(3);
#ifdef _POSIX_SOURCE
		strftime(tmp,3,"%m",localtime(&ltime));
#else
		ascftime(tmp,"%m",localtime(&ltime));
#endif
		if(!isEMPTY(tmp)) period=Ratoi(tmp);
	}
	if(tmp!=NULL) tmp=Rrealloc(tmp,5);
	else tmp=Rmalloc(5);
#ifdef _POSIX_SOURCE
	strftime(tmp,5,"%Y",localtime(&ltime));
#else
	ascftime(tmp,"%Y",localtime(&ltime));
#endif
	if(!isEMPTY(tmp))
	{
	if(fiscal_starting_month>(-1))
	{
/* NOTE: fiscal_starting_month is set up as 0=JANUARY, 1=FEBRUARY, ...*/
		if(period<=fiscal_starting_month)
		{
			period=Ratoi(tmp);
		} else {
		 	period=(Ratoi(tmp)+1);
		}
	} else {
		period=Ratoi(tmp);
	}
	} else {
		period=0;
	}
	if(tmp!=NULL) Rfree(tmp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GETCURRENTYEAR function returning the year [%d] to program [%s] line [%d]",period,file,line);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(period);
}
