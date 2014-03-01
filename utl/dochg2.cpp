/* dochg2.c - dochg_usekey */
#include<dfchg.hpp>

void dochg_usekey(CHGfile *chgfile)
{
	if(diagapps)
	{
		prterr("DIAG dochg_usekey [%s][%s].",(chgfile->module!=NULL ? 
			chgfile->module:""),(chgfile->filename!=NULL ? 
			chgfile->filename:""));
	}
}
