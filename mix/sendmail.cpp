#include <mix.hpp>

#define RDA_SENDMAIL()	xRDA_SENDMAIL(__LINE__,__FILE__)
void xRDA_SENDMAIL(int line,char *file)
{
	APPlib *args=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDA_SENDMAIL from line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	args=APPlibNEW();
	xExecute2Program("msmtp",args,NULL,0,FALSE,__LINE__,__FILE__);
	if(args!=NULL) freeapplib(args);
}
