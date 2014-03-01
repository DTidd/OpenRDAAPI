/* nonlstmakefld - the else part of testing for scrolled lists ...*/
#include <cstdio>
#include <mix.hpp>

void xnonlstmakefld(RDArsrc *mtnrsrc,short filenum,NRDfield *fldx,short edit_rsrc,short type,int line,char *file)
{
	char *name=NULL,*temp=NULL,editable=FALSE;

	temp=FILENAME(filenum);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG nonlstmakefld for Module [%s] Screen [%s] Filename [%d] [%s] Field Name [%s] Type [%d] Editable [%s] at line [%d] program [%s].",mtnrsrc->module,mtnrsrc->screen,filenum,temp,fldx->name,fldx->type,(edit_rsrc ? "True":"False"),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!type)
	{
		if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
		{
			editable=(char)edit_rsrc;
			name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(temp));
			sprintf(name,"[%s][%s]",temp,fldx->name);
			makefieldrsrc(mtnrsrc,name,fldx->type,fldx->len,editable);
			if(name!=NULL) Rfree(name);
		}
	} else {
		editable=(char)edit_rsrc;
		name=stralloc(fldx->name);
		makefieldrsrc(mtnrsrc,name,fldx->type,fldx->len,editable);
		if(name!=NULL) Rfree(name);
	}
}
