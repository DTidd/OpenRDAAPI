/* venfuncs.c - Venpmt Setup Variable Functions */
#include<venpmt.hpp>

short xgetvenpmtbin(RDAvenpmt *venpmt,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *libname=NULL;
	short version=0;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/VENPMT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\VENPMT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG getvenpmtbin Reading Venpmt Setup Variables from [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,"VENPMT SETUP",TRUE);
	if(bin==NULL) 
	{
		prterr("Error Venpmt Setup Variables were not found in Library [%s] or the Library doesn't exist at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1698) && x!=(-1699) && x!=(-1700) && x!=(-1701))
	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read Venpmt Setup Variables at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(x==(-1698)) version=4;
	else if(x==(-1699)) version=3;
	else if(x==(-1700)) version=2;
	else if(x==(-1701)) version=1;
	test=BINgetshort(bin);
	if(test) venpmt->use_venpmt_fund=TRUE;
		else venpmt->use_venpmt_fund=FALSE;
	test=BINgetshort(bin);
	if(test) venpmt->ein=BINgetstring(bin);
		else venpmt->ein=NULL;
	if(version>1)
	{
		test=BINgetshort(bin);
		if(test) venpmt->use_fiscal_summary=TRUE;
			else venpmt->use_fiscal_summary=FALSE;
	} else {
		venpmt->use_fiscal_summary=FALSE;
	}
	if(version>2)
	{
		test=BINgetshort(bin);
		if(test) venpmt->Use_Taxes=TRUE;
			else venpmt->Use_Taxes=FALSE;

		test=BINgetshort(bin);
		if(test) venpmt->Use_Tax1=TRUE;
			else venpmt->Use_Tax1=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Sep_Tax1_Vendor=TRUE;
			else venpmt->Sep_Tax1_Vendor=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax1_Vendor=BINgetstring(bin);
			else venpmt->Default_Tax1_Vendor=NULL;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax1_Address=BINgetstring(bin);
			else venpmt->Default_Tax1_Address=NULL;
		venpmt->Tax1_Rate=BINgetdouble(bin);

		test=BINgetshort(bin);
		if(test) venpmt->Use_Tax2=TRUE;
			else venpmt->Use_Tax2=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Sep_Tax2_Vendor=TRUE;
			else venpmt->Sep_Tax2_Vendor=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax2_Vendor=BINgetstring(bin);
			else venpmt->Default_Tax2_Vendor=NULL;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax2_Address=BINgetstring(bin);
			else venpmt->Default_Tax2_Address=NULL;
		venpmt->Tax2_Rate=BINgetdouble(bin);

		test=BINgetshort(bin);
		if(test) venpmt->Use_Tax3=TRUE;
			else venpmt->Use_Tax3=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Sep_Tax3_Vendor=TRUE;
			else venpmt->Sep_Tax3_Vendor=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax3_Vendor=BINgetstring(bin);
			else venpmt->Default_Tax3_Vendor=NULL;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax3_Address=BINgetstring(bin);
			else venpmt->Default_Tax3_Address=NULL;
		venpmt->Tax3_Rate=BINgetdouble(bin);

		test=BINgetshort(bin);
		if(test) venpmt->Use_Tax4=TRUE;
			else venpmt->Use_Tax4=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Sep_Tax4_Vendor=TRUE;
			else venpmt->Sep_Tax4_Vendor=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax4_Vendor=BINgetstring(bin);
			else venpmt->Default_Tax4_Vendor=NULL;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax4_Address=BINgetstring(bin);
			else venpmt->Default_Tax4_Address=NULL;
		venpmt->Tax4_Rate=BINgetdouble(bin);

		test=BINgetshort(bin);
		if(test) venpmt->Use_Tax5=TRUE;
			else venpmt->Use_Tax5=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Sep_Tax5_Vendor=TRUE;
			else venpmt->Sep_Tax5_Vendor=FALSE;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax5_Vendor=BINgetstring(bin);
			else venpmt->Default_Tax5_Vendor=NULL;
		test=BINgetshort(bin);
		if(test) venpmt->Default_Tax5_Address=BINgetstring(bin);
			else venpmt->Default_Tax5_Address=NULL;
		venpmt->Tax5_Rate=BINgetdouble(bin);

		test=BINgetshort(bin);
		if(test) venpmt->admin_fee=BINgetstring(bin);
			else venpmt->admin_fee=NULL;
	} else {
		venpmt->Use_Taxes=TRUE;

		venpmt->Use_Tax1=FALSE;
		venpmt->Sep_Tax1_Vendor=FALSE;
		venpmt->Default_Tax1_Vendor=NULL;
		venpmt->Default_Tax1_Address=NULL;
		venpmt->Tax1_Rate=0.0;

		venpmt->Use_Tax2=FALSE;
		venpmt->Sep_Tax2_Vendor=FALSE;
		venpmt->Default_Tax2_Vendor=NULL;
		venpmt->Default_Tax2_Address=NULL;
		venpmt->Tax2_Rate=0.0;

		venpmt->Use_Tax3=FALSE;
		venpmt->Sep_Tax3_Vendor=FALSE;
		venpmt->Default_Tax3_Vendor=NULL;
		venpmt->Default_Tax3_Address=NULL;
		venpmt->Tax3_Rate=0.0;

		venpmt->Use_Tax4=FALSE;
		venpmt->Sep_Tax4_Vendor=FALSE;
		venpmt->Default_Tax4_Vendor=NULL;
		venpmt->Default_Tax4_Address=NULL;
		venpmt->Tax4_Rate=0.0;

		venpmt->Use_Tax5=FALSE;
		venpmt->Sep_Tax5_Vendor=FALSE;
		venpmt->Default_Tax5_Vendor=NULL;
		venpmt->Default_Tax5_Address=NULL;
		venpmt->Tax5_Rate=0.0;

		venpmt->admin_fee=NULL;
	}
	if(version>3)
	{
		test=BINgetshort(bin);
		if(test) venpmt->use_fiscal_months=TRUE;
			else venpmt->use_fiscal_months=FALSE;
	} else {
		venpmt->use_fiscal_months=FALSE;
	}
	BINfree(bin);
	if(libname!=NULL) Rfree(libname);
	return(0);
}
short xwritevenpmtbin(RDAvenpmt *venpmt,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/VENPMT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\VENPMT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG writevenpmtbin Writing Venpmt Setup Variables to Library [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1698);	/*type identifier*/
	BINaddshort(bin,(venpmt->use_venpmt_fund?TRUE:FALSE));
	if(!isEMPTY(venpmt->ein))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->ein);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,(venpmt->use_fiscal_summary?TRUE:FALSE));

	BINaddshort(bin,(venpmt->Use_Taxes ? TRUE:FALSE));

	BINaddshort(bin,(venpmt->Use_Tax1 ? TRUE:FALSE));
	BINaddshort(bin,(venpmt->Sep_Tax1_Vendor ? TRUE:FALSE));
	if(!isEMPTY(venpmt->Default_Tax1_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax1_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(venpmt->Default_Tax1_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax1_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,venpmt->Tax1_Rate);

	BINaddshort(bin,(venpmt->Use_Tax2 ? TRUE:FALSE));
	BINaddshort(bin,(venpmt->Sep_Tax2_Vendor ? TRUE:FALSE));
	if(!isEMPTY(venpmt->Default_Tax2_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax2_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(venpmt->Default_Tax2_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax2_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,venpmt->Tax2_Rate);

	BINaddshort(bin,(venpmt->Use_Tax3 ? TRUE:FALSE));
	BINaddshort(bin,(venpmt->Sep_Tax3_Vendor ? TRUE:FALSE));
	if(!isEMPTY(venpmt->Default_Tax3_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax3_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(venpmt->Default_Tax3_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax3_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,venpmt->Tax3_Rate);

	BINaddshort(bin,(venpmt->Use_Tax4 ? TRUE:FALSE));
	BINaddshort(bin,(venpmt->Sep_Tax4_Vendor ? TRUE:FALSE));
	if(!isEMPTY(venpmt->Default_Tax4_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax4_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(venpmt->Default_Tax4_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax4_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,venpmt->Tax4_Rate);

	BINaddshort(bin,(venpmt->Use_Tax5 ? TRUE:FALSE));
	BINaddshort(bin,(venpmt->Sep_Tax5_Vendor ? TRUE:FALSE));
	if(!isEMPTY(venpmt->Default_Tax5_Vendor))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax5_Vendor);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(venpmt->Default_Tax5_Address))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->Default_Tax5_Address);
	} else BINaddshort(bin,FALSE);
	BINadddouble(bin,venpmt->Tax5_Rate);

	if(!isEMPTY(venpmt->admin_fee))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,venpmt->admin_fee);
	} else BINaddshort(bin,FALSE);

	BINaddshort(bin,(venpmt->use_fiscal_months?TRUE:FALSE));

	if(writelibbin(libname,bin,"VENPMT SETUP"))
	{
		prterr("Error Can't write Venpmt Setup Variables in binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
RDAvenpmt *xRDAvenpmtNEW(int line,char *file)
{
	RDAvenpmt *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDAvenpmtNEW Creating a structure for the Fincancial Management Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAvenpmt));
	tmp->use_venpmt_fund=FALSE;
	tmp->ein=NULL;
	tmp->use_fiscal_summary=TRUE;
	tmp->use_fiscal_months=TRUE;
	tmp->Use_Taxes=TRUE;
	return(tmp);
}
void xfree_venpmt(RDAvenpmt *venpmt,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG free_venpmt freeing memory used for the Venpmt Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(venpmt!=NULL) 
	{
		if(venpmt->ein!=NULL) Rfree(venpmt->ein);
		if(venpmt->Default_Tax1_Vendor!=NULL) Rfree(venpmt->Default_Tax1_Vendor);
		if(venpmt->Default_Tax1_Address!=NULL) Rfree(venpmt->Default_Tax1_Address);
		if(venpmt->Default_Tax2_Vendor!=NULL) Rfree(venpmt->Default_Tax2_Vendor);
		if(venpmt->Default_Tax2_Address!=NULL) Rfree(venpmt->Default_Tax2_Address);
		if(venpmt->Default_Tax3_Vendor!=NULL) Rfree(venpmt->Default_Tax3_Vendor);
		if(venpmt->Default_Tax3_Address!=NULL) Rfree(venpmt->Default_Tax3_Address);
		if(venpmt->Default_Tax4_Vendor!=NULL) Rfree(venpmt->Default_Tax4_Vendor);
		if(venpmt->Default_Tax4_Address!=NULL) Rfree(venpmt->Default_Tax4_Address);
		if(venpmt->Default_Tax5_Vendor!=NULL) Rfree(venpmt->Default_Tax5_Vendor);
		if(venpmt->Default_Tax5_Address!=NULL) Rfree(venpmt->Default_Tax5_Address);
		if(venpmt->admin_fee!=NULL) Rfree(venpmt->admin_fee);
		Rfree(venpmt);
	}
}
short xGetVenpmtSetup(RDAvenpmt *vensetup,void (*qfunc)(void *),void *arg,int line,
	char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GetVenpmtSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(getvenpmtbin(vensetup)==(-1))
	{
		ERRORDIALOG("Venpmt Setup Not Available","The Venpmt Setup File cannot opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		if(qfunc!=NULL) qfunc(arg);
		return(-1);
	}
	return(0);
}
