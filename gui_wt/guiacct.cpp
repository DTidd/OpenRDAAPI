/* guiacct.c - Gui Account Code Control Functions */
#include<gui.hpp>
#include<guip.hpp>

char *accttypes[5]=
{
	"Expenditure",
	"Revenue",
	"Balance Sheet",
	"Beginning Balance",
	"Custom Input Fields"
};

RDAacctmstr *EXPENDITURE_ACCOUNT=NULL;
RDAacctmstr *REVENUE_ACCOUNT=NULL;
RDAacctmstr *BALANCE_SHEET_ACCOUNT=NULL;
RDAacctmstr *BEGINNING_BALANCE_ACCOUNT=NULL;
RDAacctmstr *CUSTOM_INPUTS_ACCOUNT=NULL;

RDAacctmstr *xRDAacctNEW(short type,int line,char *file)
{
	RDAacctmstr *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG RDAacctNEW Creating Account Code Definitions (RDAacctmstr Structure) for type [%d] [%s] at line [%d] program [%s].",type,accttypes[type],line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAacctmstr));
	tmp->type=type;
	tmp->num=0;
	tmp->codetype=NULL;
	return(tmp);
}
void xFreeRDAaccpart(RDAaccpart **accpart,int line,char *file)
{
	RDAaccpart *accpart1=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG FreeRDAaccpart Freeing Account Code Definition Parts (RDAaccpart Structure) at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	accpart1=*accpart;
	if(accpart1!=NULL)
	{
		if(accpart1->name!=NULL) 
		{
			Rfree(accpart1->name);
		}
		if(accpart1->definition!=NULL) 
		{
			Rfree(accpart1->definition);
		}
	}
}
void xFreeRDAacct(RDAacct **acc,int line,char *file)
{
	short x;
	RDAaccpart *part=NULL;
	RDAacct *acc1=NULL;

	acc1=*acc;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG FreeRDAacct Freeing Account Code Definitions (RDAacct Structure) for acct name [%s] at line [%d] program [%s].",acc1->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(acc1!=NULL)
	{
		if(acc1->name!=NULL) Rfree(acc1->name);
		if(acc1->acct!=NULL)
		{
			for(x=0,part=acc1->acct;x<acc1->num;++x,++part)
			{
				FreeRDAaccpart(&part);
			}
			Rfree(acc1->acct);
			acc1->acct=NULL;
			acc1->num=0;
		}
	}
}
void xFreeRDAacctmstr2(RDAacctmstr **accmstr,int line,char *file)
{
	short y;
	RDAacct *acc=NULL;
	RDAacctmstr *accmstr1=NULL;

	accmstr1=*accmstr;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG FreeRDAacctmstr Freeing Account Code Definitions (RDAacctmstr Structure) for type [%d] [%s] at line [%d] program [%s].",accmstr1->type,accttypes[accmstr1->type],line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(accmstr1!=NULL)
	{
		if(accmstr1->codetype!=NULL)
		{
			for(y=0,acc=accmstr1->codetype;y<accmstr1->num;++y,++acc)
			{
				FreeRDAacct(&acc);
			}
			Rfree(accmstr1->codetype);
			accmstr1->num=0;
			accmstr1->codetype=NULL;
		}
		Rfree(*accmstr);
		*accmstr=NULL;
	}
}
void xFreeRDAacctmstr(RDAacctmstr *accmstr,int line,char *file)
{
	short x,y;
	RDAaccpart *part;
	RDAacct *acc;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG FreeRDAacct Freeing Account Code Definitions (RDAacctmstr Structure) for type [%d] [%s] at line [%d] program [%s].",accmstr->type,accttypes[accmstr->type],line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(accmstr!=NULL)
	{
		if(accmstr->codetype!=NULL)
		{
			for(y=0,acc=accmstr->codetype;y<accmstr->num;++y,++acc)
			{
				if(acc->name!=NULL) Rfree(acc->name);
				if(acc->acct!=NULL)
				{
					for(x=0,part=acc->acct;x<acc->num;++x,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
						if(part->definition!=NULL) Rfree(part->definition);
					}
					Rfree(acc->acct);
					acc->num=0;
				}
			}
			Rfree(accmstr->codetype);
			accmstr->num=0;
			accmstr->codetype=NULL;
		}
		Rfree(accmstr);
		accmstr=NULL;
	}
}
short xgetacctbin(RDAacctmstr *accmstr,int line,char *file)
{
	short x,y,test=0,version=0;
	BIN *bin;
	char *libname=NULL;
	RDAaccpart *part;
	RDAacct *acc;

	if(accmstr->type<0 || accmstr->type>4)
	{
		prterr("Error in getacctbin invalid account type [%d] at line [%d] program [%s].",accmstr->type,line,file);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG getacctbin Reading Account Code Definitions for [%d] [%s] at line [%d] program [%s].",accmstr->type,accttypes[accmstr->type],line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+14);
#ifndef WIN32
	sprintf(libname,"%s/rda/ACCT.ACC",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\ACCT.ACC",CURRENTDIRECTORY);
#endif
	if(accmstr->type<3) test=TRUE;
		else test=FALSE;
	bin=getlibbin(libname,accttypes[accmstr->type],test);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(test || diaggui || diaggui_field)
		{
			prterr("Error Account Code Definitions for [%d] [%s] are not found in Account Code Definition Library [%s].",accmstr->type,accttypes[accmstr->type],libname);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1700))
	{
		prterr("Error invalid version or bad binary in Account Code Definitions for [%d] [%s] or in Account Code Definition Library [%s].",accmstr->type,accttypes[accmstr->type],libname);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(x==(-1700)) version=1;
	else version=0;
	if(libname!=NULL) Rfree(libname);
	accmstr->num=BINgetshort(bin);
	if(accmstr->num>0)
	{
		accmstr->codetype=Rmalloc(accmstr->num*sizeof(RDAacct));
		for(y=0,acc=accmstr->codetype;y<accmstr->num;++y,++acc)
		{
			test=BINgetshort(bin);
			if(test) acc->name=BINgetstring(bin);
				else acc->name=NULL;
			test=0;
			acc->num=BINgetshort(bin);
			if(accmstr->num>0)
			{
				acc->acct=Rmalloc(acc->num*sizeof(RDAaccpart));
				for(x=0,part=acc->acct;x<acc->num;++x,++part)
				{
					part->name=BINgetstring(bin);
					part->length=BINgetshort(bin);
					part->definition=BINgetstring(bin);
					if(version>0)
					{
						test=BINgetshort(bin);
						if(test) part->default_zero=TRUE;
							else part->default_zero=FALSE;
						test=BINgetshort(bin);
						if(test) part->force_zero=TRUE;
							else part->force_zero=FALSE;  
						test=BINgetshort(bin);
						if(test) part->last_dimension=TRUE;
							else part->last_dimension=FALSE;  
					} else {
							part->force_zero=FALSE;
							part->default_zero=FALSE;
							if((x+1)==acc->num) part->last_dimension=TRUE;
								else part->last_dimension=FALSE;
					}
				}
			} else {
				acc->acct=NULL;
			}
		}
	} else {
		accmstr->codetype=NULL;
		accmstr->num=0;
	}
	BINfree(bin);
	return(0);
}
short xwriteacctbin(RDAacctmstr *accmstr,int line,char *file)
{
	short x,y;
	BIN *bin;
	RDAaccpart *part;
	char *libname=NULL;
	RDAacct *acc;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG writeacctbin Writing Account Code Definitions into Library for account type [%d] [%s] line [%d] program [%s].",accmstr->type,accttypes[accmstr->type],line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1700);	/*type identifier*/
	BINaddshort(bin,accmstr->num);
	if(accmstr->num>0)
	{
		for(y=0,acc=accmstr->codetype;y<accmstr->num;++y,++acc)
		{
			if(acc->name!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,acc->name);
			} else {
				BINaddshort(bin,FALSE);
			}
			if(acc->acct==NULL) acc->num=0;
			BINaddshort(bin,acc->num);
			if(acc->num>0 && acc->acct!=NULL)
			{
				for(x=0,part=acc->acct;x<acc->num;++x,++part)
				{
					BINaddstring(bin,part->name);
					BINaddshort(bin,part->length);
					BINaddstring(bin,part->definition);
					BINaddshort(bin,(part->default_zero ? TRUE:FALSE));
					BINaddshort(bin,(part->force_zero ? TRUE:FALSE));
					BINaddshort(bin,(part->last_dimension ? TRUE:FALSE));
				}
			}	
		}
	}
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+14);
#ifndef WIN32
	sprintf(libname,"%s/rda/ACCT.ACC",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\ACCT.ACC",CURRENTDIRECTORY);
#endif
	if(writelibbin(libname,bin,accttypes[accmstr->type]))
	{
		prterr("Error Can't write Account Code Definition Binary [%s] into Account Code Binary Library [%s].",accttypes[accmstr->type],libname);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
