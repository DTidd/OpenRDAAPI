/* RDA XPERT/MBGUI license functions */
#include<rdalic.hpp>

#ifdef WIN32

#include<windows.h>

#define COMPUTERNAME	TEXT("SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName\\")

BOOL
GetRegString (
  LPSTR pszKey,
  LPSTR pszValue,
  LPSTR pszData
  )
{

    HKEY hKey;
    DWORD dwDataSize = MAX_PATH - 1;
    DWORD dwValueType = REG_SZ;

    RegOpenKeyEx(
       HKEY_LOCAL_MACHINE,    // handle of open key 
       pszKey,                // address of name of subkey to open 
       0,                    // reserved 
       KEY_QUERY_VALUE,        // security access mask 
       &hKey                 // address of handle of open key 
       );    

    RegQueryValueEx(
        hKey,         // handle of key to query 
        pszValue,     // address of name of value to query 
        0,             // reserved 
        &dwValueType,// address of buffer for value type 
        pszData,     // address of data buffer 
        &dwDataSize  // address of data buffer size 
        );

    if (pszData[dwDataSize] != '\0')
        pszData[dwDataSize] = '\0';

    return TRUE;
}

#endif /* WIN32 */


RDAlicense *xgetRDAlicbin(BIN *bin,int line,char *file)
{
	short x,test=0;
	char *libname=NULL;
/*
	short version=0;
*/
	RDAlicense *rdal=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglic)
	{
		prterr("DIAG getRDAlicbin Reading License Binary at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(bin==NULL) return(NULL);
	x=BINgetshort(bin);
	if(x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for [RDA LICENSE] in [%s] at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(NULL);
	}
/*
	if(x==(-1701)) version=1;
*/

	if(rdal==NULL) rdal=RDAlicNEW();
	test=BINgetshort(bin);
	if(test) rdal->used=TRUE;
		else rdal->used=FALSE;
	rdal->process_count=BINgetint(bin);
	test=BINgetshort(bin);
	if(test)
	{
		rdal->display=BINgetstring(bin);
	} else {
		rdal->display=NULL;
	}
	return(rdal);
}
BIN *xwriteRDAlicbin(RDAlicense *rdal,int line,char *file)
{
	BIN *bin=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglic)
	{
		prterr("DIAG writeRDAlicbin Writing [RDALIC SETUP] Binary to Binary Library [RDALIC.STP] at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	if(rdal==NULL) return(NULL);
	BINaddshort(bin,-1701);	
	BINaddshort(bin,(rdal->used==FALSE?FALSE:TRUE));
	BINaddint(bin,rdal->process_count);
	if(rdal->display!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,rdal->display);	
	} else {
		BINaddshort(bin,FALSE);
	}
	return(bin);
}
VF xgetRDAlicvfile(int line,char *file)
{
	char *libname=NULL;
	VF vf=NULL;
	char x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglic)
	{
		prterr("DIAG getRDAlicbin Reading License Binary at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libname,"%s/rda/RDALIC.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\RDALIC.STP",CURRENTDIRECTORY);	
#endif
	vf=VFopen(libname);
	if(vf==NULL)
	{ 
		x=VFcreat(libname);  /* if file doesn't exist create one */
		if(x!=0)
		{
			prterr("ERROR: Can not create new [RDA LICENSE] vfile named [%s] at line [%d] program [%s].",libname,line,file);
			return(NULL);
		} 
		vf=VFopen(libname);
		if(vf==NULL)
		{
			prterr("ERROR: Can not open new [RDA LICENSE] vfile named [%s] at line [%d] program [%s].",libname,line,file);
			return(NULL);
		}
	} 
	return(vf);
}
RDAlicense *xRDAlicNEW(int line,char *file)
{
	RDAlicense *rdal=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglic)
	{
		prterr("DIAG RDAlicNEW Creating Structure for RDA License Setup at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	rdal=Rmalloc(sizeof(RDAlicense));
	rdal->used=FALSE;
	rdal->process_count=0;
	rdal->display=NULL;
	return(rdal);
}
void xFree_RDAlic(RDAlicense *rdal,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaglic)
	{
		prterr("DIAG Free_RDAlic Freeing RDA License Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rdal!=NULL)
	{
		if(rdal->display!=NULL) Rfree(rdal->display);
		Rfree(rdal);
	}
}
/*------------------------------------------------------------------------
	xINITRDALIC(): Function to determine if license valid already.
  ------------------------------------------------------------------------*/
int xINITRDALIC(int line,char *file)
{
	RDAlicense *rdal=NULL;
	long x=0,first_empty=(-1),newrec=0;
	char *temp=NULL;
	char *display=NULL;
	VF vf=NULL;
	BIN *bin=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglic)
	{
		prterr("DIAG INITRDALIC RDA License testing and update function at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(XPERT_SETUP->NoUsers==0)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaglic)
		{
			prterr("DIAG INITRDALIC ERROR: Number of RDA License(s) is ZERO at line [%d] program [%s].",line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(1);
	}
#ifndef WIN32
	temp=RDA_GetEnv("DISPLAY");
	if(!isEMPTY(temp))
	{
#endif /* WIN32 */
#ifdef WIN32
	GetRegString(COMPUTERNAME,"ComputerName",stemp);
	if(!isEMPTY(stemp))
	{
#endif /* WIN32 */
#ifndef WIN32
		display=stralloc(temp);
#endif
#ifdef WIN32
		display=stralloc(stemp);
#endif
	} else {
		temp=RDA_GetEnv("RDA_NOGUI");
		if(!isEMPTY(temp))
		{
			display=stralloc("RDA_NOGUI");
		} else {
#ifndef SKIP_YWINDOWS
			display=stralloc("YWIN");
#endif
#ifdef SKIP_YWINDOWS
			prterr("DIAG INITRDALIC ERROR: RDA License can't determine DISPLAY at line [%d] program [%s].",line,file);
			if(display!=NULL) Rfree(display);
			return(1);
#endif
		}
	}
	vf=getRDAlicvfile();
	if(vf!=NULL)
	{
		for(x=0;x<(XPERT_SETUP->NoUsers);++x)
		{
			bin=VFread(vf,(long)(x+1));
			if(bin==NULL)
			{
				/* what to do if VFread returns unfound record number */
				break;
			}
			rdal=getRDAlicbin(bin);
			if(rdal!=NULL)
			{
				if(rdal->used==TRUE)
				{
					if(!RDAstrcmp(display,rdal->display))
					{
						/* found matching license */ 
						/* add to process count */
						rdal->process_count+=1;
						bin=writeRDAlicbin(rdal);
						newrec=VFwrite(vf,(x+1),bin);
						Free_RDAlic(rdal);
						if(display!=NULL) Rfree(display);
						if(bin!=NULL) BINfree(bin);
						if(newrec!=0)
						{
							return(0);
						} else { 
							return(1);
						}
					}
				} else {
				/* saving first empty slot found incase needed, performace tweek */
					if(first_empty==(-1))
					{
						first_empty=(x+1);
					}
				}
			} else {
				prterr("DIAG INITRDALIC ERROR: can't allocated binary memory at line [%d] program [%s].",line,file);
				if(display!=NULL) Rfree(display);
				if(bin!=NULL) BINfree(bin);
				return(1);
			}
		}
		if(x>=(XPERT_SETUP->NoUsers) && first_empty==(-1))
		{
			prterr("DIAG INITRDALIC: Number of RDA License(s) of [%d] EXCEEDED at line [%d] program [%s].",(XPERT_SETUP->NoUsers),line,file);
			if(rdal!=NULL) Free_RDAlic(rdal);
			if(display!=NULL) Rfree(display);
			if(bin!=NULL) BINfree(bin);
			return(1);
		}
		/* extra slot available and needed */
		if(first_empty!=(-1))
		{
			/* extra slot available and needed */
			rdal=RDAlicNEW();
			rdal->used=TRUE;
			rdal->process_count=1;
			if(rdal->display!=NULL) Rfree(rdal->display);
			rdal->display=stralloc(display);
			bin=writeRDAlicbin(rdal);
			newrec=VFwrite(vf,first_empty,bin);
			if(rdal!=NULL) Free_RDAlic(rdal);
			if(display!=NULL) Rfree(display);
			if(bin!=NULL) BINfree(bin);
			return(0);
		} else {
			/* extra not available but needed */
			if(x<(XPERT_SETUP->NoUsers)) 
			{
				rdal=RDAlicNEW();
				rdal->used=TRUE;
				rdal->process_count=1;
				if(rdal->display!=NULL) Rfree(rdal->display);
				rdal->display=stralloc(display);
				bin=writeRDAlicbin(rdal);
				newrec=VFwrite(vf,0,bin);
				if(rdal!=NULL) Free_RDAlic(rdal);
				if(display!=NULL) Rfree(display);
				if(bin!=NULL) BINfree(bin);
				if(newrec!=0)
				{
					return(0);
				} else {
					return(1);
				}
			} else {
				if(rdal!=NULL) Free_RDAlic(rdal);
				if(display!=NULL) Rfree(display);
				if(bin!=NULL) BINfree(bin);
				prterr("DIAG INITRDALIC: Number of RDA License(s) of [%d] EXCEEDED at line [%d] program [%s].",(XPERT_SETUP->NoUsers),line,file);
				return(1);
			}
		}
	}
	return(1);
}
int xShutdownRDALIC(int line,char *file)
{
	RDAlicense *rdal=NULL;
	long x=0,newrec=0;
	char *display=NULL;
	char *temp=NULL;
	VF vf=NULL;
	BIN *bin=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaglic)
	{
		prterr("DIAG SHUTDOWNRDALIC RDA License testing and update function at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(RDA_NOGUI==TRUE) return(0);
#ifndef WIN32
	temp=RDA_GetEnv("DISPLAY");
	if(!isEMPTY(temp))
	{
		display=stralloc(temp);
	} else {
#endif
#ifdef WIN32
	GetRegString(COMPUTERNAME,"ComputerName",stemp);
	if(!isEMPTY(stemp))
	{
		display=stralloc(stemp);
	} else {
#endif /* WIN32 */
		temp=RDA_GetEnv("RDA_NOGUI");
		if(!isEMPTY(temp))
		{
			display=stralloc("RDA_NOGUI");
		} else {
#ifndef SKIP_YWINDOWS
			display=stralloc("YWIN");
#endif
#ifdef SKIP_YWINDOWS
			prterr("DIAG INITRDALIC ERROR: RDA License can't determine DISPLAY at line [%d] program [%s].",line,file);
			if(display!=NULL) Rfree(display);
			return(1);
#endif
		}
	}
	vf=getRDAlicvfile();
	if(vf!=NULL)
	{
		for(x=0;x<(XPERT_SETUP->NoUsers);++x)
		{
			bin=VFread(vf,(long)(x+1));
			if(bin==NULL)
			{
			/* what to do if VFreadReUse returns unfound record number */
				break;
			}
			rdal=getRDAlicbin(bin);
			if(rdal!=NULL)
			{
				if(rdal->used==TRUE)
				{
					if(!RDAstrcmp(display,rdal->display))
					{
						/* found matching license */ 
						/* add to process count */
						--rdal->process_count;
						if(rdal->process_count==0)
						{
							rdal->used=FALSE;
							if(rdal->display!=NULL) Rfree(rdal->display);
							rdal->display=stralloc("UNUSED");
							
/*
							VFdelrec(vf,(x+1));
*/
							bin=writeRDAlicbin(rdal);
						} else {
							bin=writeRDAlicbin(rdal);
						}
						newrec=VFwrite(vf,(x+1),bin);
						Free_RDAlic(rdal);
						if(display!=NULL) Rfree(display);
						if(bin!=NULL) BINfree(bin);
						if(newrec!=0)
						{
							return(0);
						} else { 
							return(1);
						}
					}
				}
			}
			if(rdal!=NULL) Free_RDAlic(rdal);
		}
		if(display!=NULL) Rfree(display);
		if(bin!=NULL) BINfree(bin);
		return(0);
	}
	if(display!=NULL) Rfree(display);
	if(bin!=NULL) BINfree(bin);
	return(0);
}
