/* reqaddvm.c - Vendor Payment Master Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

extern short vennum;
extern char editven;
extern MakeBrowseList *mbl_venmsr;
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void venyrfunc(RDArsrc *),getrecord(RDArsrc *,RDATData *);
static void venadrfunc(RDArsrc *);
void venmsrm(short,void *targetkey);

static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(vennum,mtnrsrc);
	if(QUITvfy(vennum,previous))
	{
		desc=QUITvfydesc(vennum,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_up,quit_record,FALSE,2,mtnrsrc,previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}
void venmsrm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW("VENPMT","ADD VENDOR MASTER MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,vennum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	file2rsrc(vennum,mtnrsrc,editven);
	ZERNRD(vennum);
	previous=RDATDataNEW(vennum);
	FINDRSCSETFUNC(mtnrsrc,"[VENMSR][VENDOR IDENTIFICATION]",getrecord,previous);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(vennum);
			} else {
				filerecord2rsrc(vennum,mtnrsrc);
				GetRDATData(vennum,previous);
			}
			break;
		case 2:
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(vennum,1,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(vennum);
			} else {
				filerecord2rsrc(vennum,mtnrsrc);
				GetRDATData(vennum,previous);
			}
			break;
	}
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editven)
	{
		addrfcbrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_venmsr!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
		}
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfkwrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addrfcbrsrc(mtnrsrc,"VENDOR ADDRESS",TRUE,venadrfunc,NULL);
	addrfcbrsrc(mtnrsrc,"VENDOR YEAR",TRUE,venyrfunc,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_venmsr==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL,*vendorid1=NULL,*vendorid2=NULL,*vendorid3=NULL;
	char *vendorid4=NULL,*invid1=NULL,*invid2=NULL;
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0,vdeleteflag=0;
	short yrnum=0,addnum=0,invnum=0,vchnum=0;
	short ef_yr=0,ef_add=0,ef_inv=0,ef_vch=0;
	int yearid1=0,yearid2=0,yearid3=0;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(vennum,mtnrsrc);
	if(ADVRECORDsec(vennum,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(vennum,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(vennum,0,NULL,previous);
		FINDFLDGETCHAR(vennum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETSTRING(vennum,"VENDOR IDENTIFICATION",&vendorid1);
			addnum=OPNNRDsec("VENPMT","VENADR",TRUE,TRUE,FALSE);	
			yrnum=OPNNRDsec("VENPMT","VENYR",TRUE,TRUE,FALSE);	
			invnum=OPNNRDsec("VENPMT","VENINV",TRUE,TRUE,FALSE);
			vchnum=OPNNRDsec("VENPMT","VENVCH",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW("VENPMT","DIAGNOSTIC SCREEN");
			addDFincvir(tmprsrc,"VENPMT","VENADR",NULL,addnum);
			addDFincvir(tmprsrc,"VENPMT","VENYR",NULL,yrnum);
			addDFincvir(tmprsrc,"VENPMT","VENINV",NULL,invnum);
			addDFincvir(tmprsrc,"VENPMT","VENVCH",NULL,vchnum);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			if(addnum!=(-1))
			{
				ZERNRD(addnum);
				FINDFLDSETSTRING(addnum,"VENDOR IDENTIFICATION",vendorid1);
				ef_add=ADVGTENRDsec(addnum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_add)
				{
					FINDFLDGETCHAR(addnum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(addnum,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							prev=RDATDataNEW(addnum);
							FINDFLDSETCHAR(addnum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(addnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
							if(prev!=NULL) FreeRDATData(prev);
						}
					}
					ef_add=ADVNXTNRDsec(addnum,1,SCRNvirtualSubData,tmprsrc);
					if(vendorid2!=NULL) Rfree(vendorid2);
				}
			}
			if(yrnum!=(-1))
			{
				ZERNRD(yrnum);
				FINDFLDSETSTRING(yrnum,"VENDOR IDENTIFICATION",vendorid1);
				ef_yr=ADVGTENRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_yr)
				{
					FINDFLDGETCHAR(yrnum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(yrnum,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							prev=RDATDataNEW(yrnum);
							FINDFLDGETINT(yrnum,"CALENDAR YEAR",&yearid1);
							FINDFLDSETCHAR(yrnum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(yrnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
							if(prev!=NULL) FreeRDATData(prev);
							if(invnum!=(-1))
							{
								ZERNRD(invnum);
								FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid1);
								FINDFLDSETINT(invnum,"CALENDAR YEAR",yearid1);
								ef_inv=ADVGTENRDsec(invnum,1,SCRNvirtualSubData,tmprsrc);
								while(!ef_inv)
								{
									FINDFLDGETCHAR(invnum,"DELETEFLAG",&pdeleteflag);
									if(!pdeleteflag)
									{
										FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&vendorid3);
										FINDFLDGETINT(invnum,"CALENDAR YEAR",&yearid2);
										if(!RDAstrcmp(vendorid3,vendorid1) && 
											yearid2==yearid1)
										{
										  FINDFLDGETSTRING(invnum,"INVOICE IDENTIFICATION",&invid1);
										  prev=RDATDataNEW(invnum);
										  FINDFLDSETCHAR(invnum,"DELETEFLAG",deleteflag);
										  ADVWRTTRANSsec(invnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
										  if(prev!=NULL) FreeRDATData(prev);
										  if(vchnum!=(-1))
										  {
										    ZERNRD(vchnum);
										    FINDFLDSETSTRING(vchnum,"VENDOR IDENTIFICATION",vendorid1);
										    FINDFLDSETINT(vchnum,"CALENDAR YEAR",yearid1);
										    FINDFLDSETSTRING(vchnum,"INVOICE IDENTIFICATION",invid1);
										    ef_vch=ADVGTENRDsec(vchnum,1,SCRNvirtualSubData,tmprsrc);
										    while(!ef_vch)
										    {
 										      FINDFLDGETCHAR(vchnum,"DELETEFLAG",&vdeleteflag);
										      if(!vdeleteflag)
										      {
										        FINDFLDGETSTRING(vchnum,"VENDOR IDENTIFICATION",&vendorid4);
										        FINDFLDGETINT(vchnum,"CALENDAR YEAR",&yearid3);
										        FINDFLDGETSTRING(vchnum,"INVOICE IDENTIFICATION",&invid2);
										        if(!RDAstrcmp(vendorid4,vendorid1) && 
										          yearid3==yearid1 && 
										          (!RDAstrcmp(invid2,invid1)))
										        {
												  prev=RDATDataNEW(vchnum);
										          FINDFLDSETCHAR(vchnum,"DELETEFLAG",deleteflag);
										          ADVWRTTRANSsec(vchnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
												  if(prev!=NULL) FreeRDATData(prev);
										        }
										      }
										      if(vendorid4!=NULL) { Rfree(vendorid4); vendorid4=NULL; }
										      if(invid2!=NULL) { Rfree(invid2); invid2=NULL; }
										      ef_inv=ADVNXTNRDsec(invnum,1,SCRNvirtualSubData,tmprsrc);
										    }
										  }
										}
									}
									ef_inv=ADVNXTNRDsec(invnum,1,SCRNvirtualSubData,tmprsrc);
									if(vendorid3!=NULL) { Rfree(vendorid3); vendorid3=NULL; }
									if(invid1!=NULL) { Rfree(invid1); invid1=NULL; }
								}
							}
						}
					}
					ef_yr=ADVNXTNRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc); 	
					if(vendorid2!=NULL) { Rfree(vendorid2); vendorid2=NULL; }
				}
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			if(yrnum!=(-1)) CLSNRD(yrnum);
			if(invnum!=(-1)) CLSNRD(invnum);
			if(vchnum!=(-1)) CLSNRD(vchnum);
			if(addnum!=(-1)) CLSNRD(addnum);
		} else venadrfunc(mtnrsrc);
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(vendorid3!=NULL) Rfree(vendorid3);
		if(vendorid4!=NULL) Rfree(vendorid4);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		updatebrowse(update_list,mbl_venmsr,vennum,"PURORD",mtnrsrc);
		if(mbl_venmsr!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(vennum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(WindowCount==1)
	{
		CLSNRD(vennum);
		ShutdownSubsystems();
		exit(0);	
	}
	if(previous!=NULL) FreeRDATData(previous);
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void venadrfunc(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	rsrc2filerecord(vennum,mtnrsrc);
	MTNSCRN_EXECUTEPROGRAM("venadr",mtnrsrc,"VENADR KEY",vennum,1,NULL);
}
static void venyrfunc(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	rsrc2filerecord(vennum,mtnrsrc);
	MTNSCRN_EXECUTEPROGRAM("venyr",mtnrsrc,"VENYR KEY",vennum,1,NULL);
}
static void getrecord(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *vendorid1=NULL,*vendorid2=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[VENMSR][VENDOR IDENTIFICATION]",&vendorid1);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[VENMSR][VENDOR IDENTIFICATION]",&vendorid2);
	if(RDAstrcmp(vendorid2,vendorid1))
	{
		ZERNRD(vennum);
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid2);
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(vennum,1);
			ClearRDATData(previous);
			updatefilersrc(vennum,mtnrsrc);	
		} else {
			GetRDATData(vennum,previous);
			updatefilerecord2rsrc(vennum,mtnrsrc);	
		}
		updateSCRNvirtuals(mtnrsrc);
	}
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
