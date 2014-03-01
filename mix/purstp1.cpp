/* purstp1.c - PurOrd Setup Variables */
#include<purord.hpp>

short xgetpurordbin(RDApurord *purord,int line,char *file)
{
/*
	short version=FALSE;
*/
	short x,test=FALSE;
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/PURORD.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PURORD.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG getpurordbin Reading PurOrd Setup Variables from [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,"PURORD SETUP",TRUE);
	if(bin==NULL) 
	{
		prterr("Error PurOrd Setup Variables were not found in Library [%s] or the Library doesn't exist at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
/*
	if(x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read PurOrd Setup Variables at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(x==(-1700)) version=TRUE;
	else version=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Use_Taxes=TRUE;
		else purord->Use_Taxes=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Use_Tax1=TRUE;
		else purord->Use_Tax1=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Sep_Tax1_Vendor=TRUE;
		else purord->Sep_Tax1_Vendor=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax1_Vendor=BINgetstring(bin);
		else purord->Default_Tax1_Vendor=NULL;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax1_Address=BINgetstring(bin);
		else purord->Default_Tax1_Address=NULL;
	purord->Tax1_Rate=BINgetdouble(bin);
	test=BINgetshort(bin);
	if(test) purord->Use_Tax2=TRUE;
		else purord->Use_Tax2=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Sep_Tax2_Vendor=TRUE;
		else purord->Sep_Tax2_Vendor=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax2_Vendor=BINgetstring(bin);
		else purord->Default_Tax2_Vendor=NULL;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax2_Address=BINgetstring(bin);
		else purord->Default_Tax2_Address=NULL;
	purord->Tax2_Rate=BINgetdouble(bin);
	test=BINgetshort(bin);
	if(test) purord->Use_Tax3=TRUE;
		else purord->Use_Tax3=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Sep_Tax3_Vendor=TRUE;
		else purord->Sep_Tax3_Vendor=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax3_Vendor=BINgetstring(bin);
		else purord->Default_Tax3_Vendor=NULL;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax3_Address=BINgetstring(bin);
		else purord->Default_Tax3_Address=NULL;
	purord->Tax3_Rate=BINgetdouble(bin);
	test=BINgetshort(bin);
	if(test) purord->Use_Tax4=TRUE;
		else purord->Use_Tax4=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Sep_Tax4_Vendor=TRUE;
		else purord->Sep_Tax4_Vendor=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax4_Vendor=BINgetstring(bin);
		else purord->Default_Tax4_Vendor=NULL;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax4_Address=BINgetstring(bin);
		else purord->Default_Tax4_Address=NULL;
	purord->Tax4_Rate=BINgetdouble(bin);
	test=BINgetshort(bin);
	if(test) purord->Use_Tax5=TRUE;
		else purord->Use_Tax5=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Sep_Tax5_Vendor=TRUE;
		else purord->Sep_Tax5_Vendor=FALSE;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax5_Vendor=BINgetstring(bin);
		else purord->Default_Tax5_Vendor=NULL;
	test=BINgetshort(bin);
	if(test) purord->Default_Tax5_Address=BINgetstring(bin);
		else purord->Default_Tax5_Address=NULL;
	purord->Tax5_Rate=BINgetdouble(bin);
	if(version)
	{
		test=BINgetshort(bin);
		if(test) purord->admin_fee=BINgetstring(bin);
			else purord->admin_fee=NULL;
	}
*/
	if(x!=(-1700))
	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read PurOrd Setup Variables at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
/*
	if(x==(-1700)) version=1;
*/
	test=BINgetshort(bin);
	if(test) purord->use_fiscal_months=TRUE;
		else purord->use_fiscal_months=FALSE;
	BINfree(bin);
	if(libname!=NULL) Rfree(libname);
	return(0);
}
short xwritepurordbin(RDApurord *purord,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/PURORD.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PURORD.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG writepurordbin Writing PurOrd Setup Variables to Library [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1700);	/*type identifier*/
/*
	BINaddshort(bin,(purord->Use_Taxes ? TRUE:FALSE));
	BINaddshort(bin,(purord->Use_Tax1 ? TRUE:FALSE));
	BINaddshort(bin,(purord->Sep_Tax1_Vendor ? TRUE:FALSE));
	if(!isEMPTY(purord->Default_Tax1_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax1_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(purord->Default_Tax1_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax1_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,purord->Tax1_Rate);
	BINaddshort(bin,(purord->Use_Tax2 ? TRUE:FALSE));
	BINaddshort(bin,(purord->Sep_Tax2_Vendor ? TRUE:FALSE));
	if(!isEMPTY(purord->Default_Tax2_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax2_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(purord->Default_Tax2_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax2_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,purord->Tax2_Rate);
	BINaddshort(bin,(purord->Use_Tax3 ? TRUE:FALSE));
	BINaddshort(bin,(purord->Sep_Tax3_Vendor ? TRUE:FALSE));
	if(!isEMPTY(purord->Default_Tax3_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax3_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(purord->Default_Tax3_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax3_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,purord->Tax3_Rate);
	BINaddshort(bin,(purord->Use_Tax4 ? TRUE:FALSE));
	BINaddshort(bin,(purord->Sep_Tax4_Vendor ? TRUE:FALSE));
	if(!isEMPTY(purord->Default_Tax4_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax4_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(purord->Default_Tax4_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax4_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,purord->Tax4_Rate);
	BINaddshort(bin,(purord->Use_Tax5 ? TRUE:FALSE));
	BINaddshort(bin,(purord->Sep_Tax5_Vendor ? TRUE:FALSE));
	if(!isEMPTY(purord->Default_Tax5_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax5_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(purord->Default_Tax5_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->Default_Tax5_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,purord->Tax5_Rate);
	if(!isEMPTY(purord->admin_fee))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,purord->admin_fee);
	} else BINaddshort(bin,FALSE);
*/
	BINaddshort(bin,(purord->use_fiscal_months? TRUE:FALSE));
	if(writelibbin(libname,bin,"PURORD SETUP"))
	{
		prterr("Error Can't write PurOrd Setup Variables in binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
RDApurord *xRDApurordNEW(int line,char *file)
{
	RDApurord *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDApurordNEW Creating a structure for the Fincancial Management Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDApurord));
/*
	tmp->Use_Taxes=FALSE;
	tmp->Use_Tax1=FALSE;
	tmp->Sep_Tax1_Vendor=FALSE;
	tmp->Default_Tax1_Vendor=NULL;
	tmp->Default_Tax1_Address=NULL;
	tmp->Tax1_Rate=0.0;
	tmp->Use_Tax2=FALSE;
	tmp->Sep_Tax2_Vendor=FALSE;
	tmp->Default_Tax2_Vendor=NULL;
	tmp->Default_Tax2_Address=NULL;
	tmp->Tax2_Rate=0.0;
	tmp->Use_Tax3=FALSE;
	tmp->Sep_Tax3_Vendor=FALSE;
	tmp->Default_Tax3_Vendor=NULL;
	tmp->Default_Tax3_Address=NULL;
	tmp->Tax3_Rate=0.0;
	tmp->Use_Tax4=FALSE;
	tmp->Sep_Tax4_Vendor=FALSE;
	tmp->Default_Tax4_Vendor=NULL;
	tmp->Default_Tax4_Address=NULL;
	tmp->Tax4_Rate=0.0;
	tmp->Use_Tax5=FALSE;
	tmp->Sep_Tax5_Vendor=FALSE;
	tmp->Default_Tax5_Vendor=NULL;
	tmp->Default_Tax5_Address=NULL;
	tmp->Tax5_Rate=0.0;
	tmp->admin_fee=NULL;
*/
	tmp->use_fiscal_months=TRUE;
	return(tmp);
}
void xfree_purord(RDApurord *purord,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG free_purord freeing memory used for the PurOrd Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(purord!=NULL) 
	{
/*
		if(purord->Default_Tax1_Vendor!=NULL) Rfree(purord->Default_Tax1_Vendor);
		if(purord->Default_Tax1_Address!=NULL) Rfree(purord->Default_Tax1_Address);
		if(purord->Default_Tax2_Vendor!=NULL) Rfree(purord->Default_Tax2_Vendor);
		if(purord->Default_Tax2_Address!=NULL) Rfree(purord->Default_Tax2_Address);
		if(purord->Default_Tax3_Vendor!=NULL) Rfree(purord->Default_Tax3_Vendor);
		if(purord->Default_Tax3_Address!=NULL) Rfree(purord->Default_Tax3_Address);
		if(purord->Default_Tax4_Vendor!=NULL) Rfree(purord->Default_Tax4_Vendor);
		if(purord->Default_Tax4_Address!=NULL) Rfree(purord->Default_Tax4_Address);
		if(purord->Default_Tax5_Vendor!=NULL) Rfree(purord->Default_Tax5_Vendor);
		if(purord->Default_Tax5_Address!=NULL) Rfree(purord->Default_Tax5_Address);
		if(purord->admin_fee!=NULL) Rfree(purord->admin_fee);
*/

		Rfree(purord);
	}
}
short xGetPurOrdSetup(RDApurord *pursetup,void (*qfunc)(void *),void *arg,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GetPurOrdSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(getpurordbin(pursetup)==(-1))
	{
		ERRORDIALOG("PurOrd Setup Not Available","The PurOrd Setup File cannot opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		if(qfunc!=NULL) qfunc(arg);
		return(-1);
	}
	return(0);
}
