/* venavch2.c - Add Vendor Master */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <fin.h>
#include <venpmt.h>

extern MaintainMaster *VENMSR_MASTER,*VENINV_MASTER;
extern void browse_venmsr(RDArsrc *,RDATData *);
extern char editable;
extern int getcurrentcalyear(void);
MakeBrowseList *mbl_venmsr=NULL;
char which_modal=FALSE;
extern RDAvenpmt *venpmt;
static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void venadrfunc(RDArsrc *);
void venmsrm(short,void *targetkey);
extern void veninvm(short,void *);
extern int Calendar_Year;

static void getinvoice(RDArsrc *mtnrsrc)
{
	void *target=NULL;
	short l=0;

	target=GETNRDKEY(VENMSR_MASTER->passkey->fileno,1,&l);
	readwidget(mtnrsrc,"CALENDAR YEAR");
	FINDRSCGETINT(mtnrsrc,"CALENDAR YEAR",&Calendar_Year);
	veninvm(1,target);
	if(target!=NULL) Rfree(target);
}
/*
static void save_record(RDArsrc *parent,RDATData *previous,short update_list)
{
	char *rcddesc=NULL,*vendorid1=NULL,*vendorid2=NULL,*vendorid3=NULL;
	char *vendorid4=NULL,*invid1=NULL,*invid2=NULL;
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0,vdeleteflag=0;
	short yrnum=0,addnum=0,invnum=0,vchnum=0;
	short ef_yr=0,ef_add=0,ef_inv=0,ef_vch=0;
	int yearid1=0,yearid2=0,yearid3=0;
	RDATData *sub_prev=NULL;

	readallwidgets(parent);
	rsrc2filerecord(filenum,parent);
	if(ADVRECORDsec(filenum,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(filenum,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETSTRING(filenum,"VENDOR IDENTIFICATION",&vendorid1);
			addnum=OPNNRDsec("VENPMT","VENADR",TRUE,TRUE,FALSE);
			if(addnum!=(-1))
			{
				ZERNRD(addnum);
				FINDFLDSETSTRING(addnum,"VENDOR IDENTIFICATION",vendorid1);
				ef_add=ADVGTENRDsec(addnum,1,SCRNvirtualSubData,parent);
				while(!ef_add)
				{
					FINDFLDGETCHAR(addnum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(addnum,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							sub_prev=RDATDataNEW(addnum);
							FINDFLDSETCHAR(addnum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(addnum,0,NULL,sub_prev,SCRNvirtualSubData,parent);		
							if(sub_prev!=NULL) FreeRDATData(sub_prev);
						}
					}
					ef_add=ADVNXTNRDsec(addnum,1,SCRNvirtualSubData,parent);
					if(vendorid2!=NULL) { Rfree(vendorid2); vendorid2=NULL; } 
				}
				CLSNRD(addnum);
			}
			yrnum=OPNNRDsec("VENPMT","VENYR",TRUE,TRUE,FALSE);	
			invnum=OPNNRDsec("VENPMT","VENINV",TRUE,TRUE,FALSE);
			vchnum=OPNNRDsec("VENPMT","VENVCH",TRUE,TRUE,FALSE);
			if(yrnum!=(-1))
			{
				ZERNRD(yrnum);
				FINDFLDSETSTRING(yrnum,"VENDOR IDENTIFICATION",vendorid1);
				ef_yr=ADVGTENRDsec(yrnum,1,SCRNvirtualSubData,parent);
				while(!ef_yr)
				{
					FINDFLDGETCHAR(yrnum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(yrnum,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							sub_prev=RDATDataNEW(yrnum);
							FINDFLDSETCHAR(yrnum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(yrnum,0,NULL,sub_prev,SCRNvirtualSubData,parent);
							if(sub_prev!=NULL) FreeRDATData(sub_prev);
							if(invnum!=(-1))
							{
								ZERNRD(invnum);
								FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid1);
								FINDFLDSETINT(invnum,"CALENDAR YEAR",yearid1);
								ef_inv=ADVGTENRDsec(invnum,1,SCRNvirtualSubData,parent);
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
										  sub_prev=RDATDataNEW(invnum);
										  FINDFLDGETSTRING(invnum,"INVOICE IDENTIFICATION",&invid1);
						
										  FINDFLDSETCHAR(invnum,"DELETEFLAG",deleteflag);
										  ADVWRTTRANSsec(invnum,0,NULL,sub_prev,SCRNvirtualSubData,parent);
										  if(sub_prev!=NULL) FreeRDATData(sub_prev);
										  if(vchnum!=(-1))
										  {
										    ZERNRD(vchnum);
										    FINDFLDSETSTRING(vchnum,"VENDOR IDENTIFICATION",vendorid1);
										    FINDFLDSETINT(vchnum,"CALENDAR YEAR",yearid1);
										    FINDFLDSETSTRING(vchnum,"INVOICE IDENTIFICATION",invid1);
										    ef_vch=ADVGTENRDsec(vchnum,1,SCRNvirtualSubData,parent);
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
												  sub_prev=RDATDataNEW(vchnum);
										          FINDFLDSETCHAR(vchnum,"DELETEFLAG",deleteflag);
										          ADVWRTTRANSsec(vchnum,0,NULL,sub_prev,SCRNvirtualSubData,parent);
												  if(sub_prev!=NULL) FreeRDATData(sub_prev);
										        }
										      }
										      if(vendorid4!=NULL) { Rfree(vendorid4); vendorid4=NULL; }
										      if(invid2!=NULL) { Rfree(invid2); invid2=NULL; }
										      ef_inv=ADVNXTNRDsec(invnum,1,SCRNvirtualSubData,parent);
										    }
										  }
										}
									}
									ef_inv=ADVNXTNRDsec(invnum,1,SCRNvirtualSubData,parent);
									if(vendorid3!=NULL) { Rfree(vendorid3); vendorid3=NULL; }
									if(invid1!=NULL) { Rfree(invid1); invid1=NULL; }
								}
							}
						}
					}
					ef_yr=ADVNXTNRDsec(yrnum,1,SCRNvirtualSubData,parent); 	
					if(vendorid2!=NULL) { Rfree(vendorid2); vendorid2=NULL; }
				}
			}
			if(yrnum!=(-1)) CLSNRD(yrnum);
			if(invnum!=(-1)) CLSNRD(invnum);
			if(vchnum!=(-1)) CLSNRD(vchnum);
		} else venadrfunc(parent);
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(vendorid3!=NULL) Rfree(vendorid3);
		if(vendorid4!=NULL) Rfree(vendorid4);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		updatebrowse(update_list,mbl_venmsr,filenum,"VENPMT",parent);
		if(mbl_venmsr!=NULL) quit_record(parent,previous);
			else GetRDATData(filenum,previous);
	}
}
*/
void venmsrm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	int currentcalyear=0;

	mtnrsrc=RDArsrcNEW(VENMSR_MASTER->module,VENMSR_MASTER->maintain_screen);
	ZERNRD(VENMSR_MASTER->passkey->fileno);
	previous=RDATDataNEW(VENMSR_MASTER->passkey->fileno);
	nokeys=NUMKEYS(VENMSR_MASTER->passkey->fileno);
	keys=KEYPOINTER(VENMSR_MASTER->passkey->fileno);
	nofields=NUMFLDS(VENMSR_MASTER->passkey->fileno);
	fields=FLDPOINTER(VENMSR_MASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,VENMSR_MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			nonlstmakefld(mtnrsrc,VENMSR_MASTER->passkey->fileno,fldx,edit_rsrc);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(VENMSR_MASTER->mainfile));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(VENMSR_MASTER->mainfile));
			}
			sprintf(name,"[%s][%s]",VENMSR_MASTER->mainfile,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,SETFILES,VENMSR_MASTER);
		}
	}
	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->part!=NULL)
			{
				for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
				{
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(VENMSR_MASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(VENMSR_MASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",VENMSR_MASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	currentcalyear=getcurrentcalyear();
	addstdrsrc(mtnrsrc,"CALENDAR YEAR",LONGV,4,&currentcalyear,TRUE);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,VENMSR_MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(VENMSR_MASTER->passkey);
			if(ADVEQLNRDsec(VENMSR_MASTER->passkey->fileno,VENMSR_MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(VENMSR_MASTER->passkey->fileno,VENMSR_MASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,VENMSR_MASTER->passkey);
				if(ADVEQLNRDsec(VENMSR_MASTER->passkey->fileno,VENMSR_MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,VENMSR_MASTER->passkey);
				} else {
					GetRDATData(VENMSR_MASTER->passkey->fileno,previous);
					filerecord2rsrc(VENMSR_MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(VENMSR_MASTER->passkey->fileno,previous);
				filerecord2rsrc(VENMSR_MASTER->passkey->fileno,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(VENMSR_MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,VENMSR_MASTER->passkey);
			if(ADVEQLNRDsec(VENMSR_MASTER->passkey->fileno,VENMSR_MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,VENMSR_MASTER->passkey);
			} else {
				GetRDATData(VENMSR_MASTER->passkey->fileno,previous);
				filerecord2rsrc(VENMSR_MASTER->passkey->fileno,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(VENMSR_MASTER->passkey->fileno,VENMSR_MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(VENMSR_MASTER->passkey->fileno);
				ApplyPassKey(VENMSR_MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,VENMSR_MASTER->passkey);
				if(ADVEQLNRDsec(VENMSR_MASTER->passkey->fileno,VENMSR_MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,VENMSR_MASTER->passkey);
				} else {
					GetRDATData(VENMSR_MASTER->passkey->fileno,previous);
					filerecord2rsrc(VENMSR_MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(VENMSR_MASTER->passkey->fileno,previous);
				filerecord2rsrc(VENMSR_MASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,VENMSR_MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_venmsr!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_venmsr,previous);
	addbtnrsrc(mtnrsrc,"INVOICES",TRUE,getinvoice,NULL);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,VENMSR_MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_venmsr!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,which_modal,quit_record,previous,
		(mbl_venmsr==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(VENMSR_MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(VENMSR_MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=VENMSR_MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,VENMSR_MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
        short keyno=(-1);

        keyno=KEYNUMBER(VENMSR_MASTER->passkey->fileno,(member->rscrname+5));
        if(keyno==(-1))
        {
                keyno=VENMSR_MASTER->passkey->keyno;
        }
        ADV2GET_NEXT(member->parent,VENMSR_MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,VENMSR_MASTER,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *VENMSR_MASTER)
{
	SET_MASTER(mtnrsrc,VENMSR_MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(VENMSR_MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(VENMSR_MASTER->passkey->fileno,mtnrsrc);
}
static void save_record_now(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *desc=NULL;

	if(!SAVE_CHECK(mtnrsrc,VENMSR_MASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(VENMSR_MASTER->save_expression))
	{
		if(EVALUATEbol(VENMSR_MASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(VENMSR_MASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(VENMSR_MASTER->save_warning))
	{
		if(EVALUATEbol(VENMSR_MASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(VENMSR_MASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"SAVE WARNING",desc,
				(update?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update);
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *rcddesc=NULL,*vendorid1=NULL,*vendorid2=NULL,*vendorid3=NULL;
	char *vendorid4=NULL,*invid1=NULL,*invid2=NULL;
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0,vdeleteflag=0;
	char dist=FALSE,cleared=FALSE,*debcode1=NULL,*credcode1=NULL;
	short venyr=(-1),venadr=(-1),veninv=(-1),venvch=(-1);
	short fineyr=(-1),finryr=(-1),finbyr=(-1);
	short ef_yr=0,ef_add=0,ef_inv=0,ef_vch=0;
	short accttype=0,deftype=0,p_month=0;
	double amount=0.0;
	int yearid1=0,yearid2=0,yearid3=0,procyrid=0;
	RDATData *prev=NULL;

	if(ADVRECORDsec(VENMSR_MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(VENMSR_MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(VENMSR_MASTER->passkey->fileno,0,NULL,previous);
		FINDFLDGETCHAR(VENMSR_MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETSTRING(VENMSR_MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
			venadr=OPNNRDsec(VENMSR_MASTER->module,"VENADR",TRUE,TRUE,FALSE);	
			if(venadr!=(-1))
			{
				ZERNRD(venadr);
				FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
				ef_add=ADVGTENRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
				while(!ef_add)
				{
					FINDFLDGETCHAR(venadr,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							prev=RDATDataNEW(venadr);
							FINDFLDSETCHAR(venadr,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(venadr,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
							if(prev!=NULL) FreeRDATData(prev);
						}
					}
					ef_add=ADVNXTNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
					if(vendorid2!=NULL) { Rfree(vendorid2); vendorid2=NULL; } 
				}
			}
			if(venadr!=(-1)) CLSNRD(venadr);
			venyr=OPNNRDsec(VENMSR_MASTER->module,"VENYR",TRUE,TRUE,FALSE);
			veninv=OPNNRDsec(VENMSR_MASTER->module,"VENINV",TRUE,TRUE,FALSE);
			venvch=OPNNRDsec(VENMSR_MASTER->module,"VENVCH",TRUE,TRUE,FALSE);
			fineyr=OPNNRDsec("FINMGT","FINEYR",TRUE,TRUE,FALSE);
			finryr=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			finbyr=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			if(venyr!=(-1))
			{
				ZERNRD(venyr);
				FINDFLDSETSTRING(venyr,"VENDOR IDENTIFICATION",vendorid1);
				ef_yr=ADVGTENRDsec(venyr,1,SCRNvirtualSubData,mtnrsrc);
				while(!ef_yr)
				{
					FINDFLDGETCHAR(venyr,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(venyr,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							FINDFLDGETINT(venyr,"CALENDAR YEAR",&yearid1);
							prev=RDATDataNEW(venyr);
							FINDFLDSETCHAR(venyr,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(venyr,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);
							if(prev!=NULL) FreeRDATData(prev);
							if(veninv!=(-1))
							{
								ZERNRD(veninv);
								FINDFLDSETSTRING(veninv,"VENDOR IDENTIFICATION",vendorid1);
								FINDFLDSETINT(veninv,"CALENDAR YEAR",yearid1);
								ef_inv=ADVGTENRDsec(veninv,1,SCRNvirtualSubData,mtnrsrc);
								while(!ef_inv)
								{
									FINDFLDGETCHAR(veninv,"DELETEFLAG",&pdeleteflag);
									if(!pdeleteflag)
									{
										FINDFLDGETSTRING(veninv,"VENDOR IDENTIFICATION",&vendorid3);
										FINDFLDGETINT(veninv,"CALENDAR YEAR",&yearid2);
										if(!RDAstrcmp(vendorid3,vendorid1) && 
											yearid2==yearid1)
										{
										  FINDFLDGETSTRING(veninv,"INVOICE IDENTIFICATION",&invid1);
										  prev=RDATDataNEW(veninv);
										  FINDFLDSETCHAR(veninv,"DELETEFLAG",deleteflag);
										  ADVWRTTRANSsec(veninv,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
										  if(prev!=NULL) FreeRDATData(prev);
										  if(venvch!=(-1) && fineyr!=(-1) && finryr!=(-1) && finbyr!=(-1))
										  {
										    ZERNRD(venvch);
										    FINDFLDSETSTRING(venvch,"VENDOR IDENTIFICATION",vendorid1);
										    FINDFLDSETINT(venvch,"CALENDAR YEAR",yearid1);
										    FINDFLDSETSTRING(venvch,"INVOICE IDENTIFICATION",invid1);
										    ef_vch=ADVGTENRDsec(venvch,1,SCRNvirtualSubData,mtnrsrc);
										    while(!ef_vch)
										    {
 										      FINDFLDGETCHAR(venvch,"DELETEFLAG",&vdeleteflag);
										      if(!vdeleteflag)
										      {
										        FINDFLDGETSTRING(venvch,"VENDOR IDENTIFICATION",&vendorid4);
										        FINDFLDGETINT(venvch,"CALENDAR YEAR",&yearid3);
										        FINDFLDGETSTRING(venvch,"INVOICE IDENTIFICATION",&invid2);
										        if(!RDAstrcmp(vendorid4,vendorid1) && 
										          yearid3==yearid1 && 
										          (!RDAstrcmp(invid2,invid1)))
										        {
												  prev=RDATDataNEW(venvch);
												  FINDFLDGETCHAR(venvch,"DISTRIBUTED",&dist);
												  FINDFLDGETCHAR(venvch,"CLEARED",&cleared);
												  if(dist==FALSE || cleared==FALSE)
												  {
												    FINDFLDGETSHORT(venvch,"ACCOUNT TYPE",&accttype);
												    FINDFLDGETSHORT(venvch,"DEFINITION TYPE",&deftype);
												    FINDFLDGETINT(venvch,"FISCAL YEAR",&procyrid);
/*
												    FINDFLDGETSHORT(venvch,"PROCESSING MONTH",&p_month);
*/
												    FINDFLDGETSHORT(venvch,"FISCAL PROCESSING MONTH",&p_month);
												    FINDFLDGETDOUBLE(venvch,"AMOUNT",&amount);
												    FINDFLDGETSTRING(venvch,"DEBIT CODE",&debcode1);
												    FINDFLDGETSTRING(venvch,"CREDIT CODE",&credcode1);
												    switch(accttype)
												    {
												        case EXP_ACCT:
												           if(credcode1!=NULL && RDAstrlen(credcode1)>0)
												           {
													   if(venpmt->use_fiscal_months==TRUE)
													   {
												               ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   } else {
												               ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   }
												           } else {
													   if(venpmt->use_fiscal_months==TRUE)
													   {
												               ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   } else {
												               ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   }
												           }
												           break;
												        case REV_ACCT:
												           if(debcode1!=NULL && RDAstrlen(debcode1)>0)
												           {
													   if(venpmt->use_fiscal_months==TRUE)
													   {
												               ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   } else {
												               ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   }
												           } else {
													   if(venpmt->use_fiscal_months==TRUE)
													   {
												              ADVupdateundist(venvch,"ACCOUNT CODE",NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   } else {
												              ADVupdateundist(venvch,"ACCOUNT CODE",NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
													   }
												           }
												           break;
												        case BAL_ACCT:
													   	   if(amount<0.0)
									  					   {
														   if(venpmt->use_fiscal_months==TRUE)
														   {
													         	ADVupdateundist(venvch,NULL,(dist==FALSE?"ACCOUNT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"DEBITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
														   } else {
													         	ADVupdateundist(venvch,NULL,(dist==FALSE?"ACCOUNT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"DEBITS",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
														   } 
														   } else {
														   if(venpmt->use_fiscal_months==TRUE)
														   {
													         	ADVupdateundist(venvch,NULL,(cleared==FALSE?"CREDIT CODE":NULL),(dist==FALSE?"ACCOUNT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
														   } else {
													         	ADVupdateundist(venvch,NULL,(cleared==FALSE?"CREDIT CODE":NULL),(dist==FALSE?"ACCOUNT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
														   }
														   }
														   break;
														default:
													   	   prterr("Invalid Account Type [%d]",accttype);
													       break;
												    } 
												    if(debcode1!=NULL) Rfree(debcode1);
												    if(credcode1!=NULL) Rfree(credcode1);
												  }
												  FINDFLDSETCHAR(venvch,"DELETEFLAG",deleteflag);
												  ADVWRTTRANSsec(venvch,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
												  if(prev!=NULL) FreeRDATData(prev);
										        }
										      }
										      if(vendorid4!=NULL) Rfree(vendorid4);
										      if(invid2!=NULL) Rfree(invid2);
										      ef_vch=ADVNXTNRDsec(venvch,1,SCRNvirtualSubData,mtnrsrc);
										    }
										  }
										}
									}
									ef_inv=ADVNXTNRDsec(veninv,1,SCRNvirtualSubData,mtnrsrc);
									if(vendorid3!=NULL) Rfree(vendorid3);
									if(invid1!=NULL) Rfree(invid1); 
								}
							}
						}
					}
					ef_yr=ADVNXTNRDsec(venyr,1,SCRNvirtualSubData,mtnrsrc); 	
					if(vendorid2!=NULL) Rfree(vendorid2);
				}
			}
			if(venyr!=(-1)) CLSNRD(venyr);
			if(veninv!=(-1)) CLSNRD(veninv);
			if(venvch!=(-1)) CLSNRD(venvch);
			if(fineyr!=(-1)) CLSNRD(fineyr);
			if(finryr!=(-1)) CLSNRD(finryr);
			if(finbyr!=(-1)) CLSNRD(finbyr);
		} else venadrfunc(mtnrsrc);
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(vendorid3!=NULL) Rfree(vendorid3);
		if(vendorid4!=NULL) Rfree(vendorid4);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		DELETE_SUBORDINATES(mtnrsrc,VENMSR_MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_venmsr,VENMSR_MASTER->passkey->fileno,VENMSR_MASTER->module,mtnrsrc);
		if(mbl_venmsr!=NULL)
		{
			quit_record(mtnrsrc,previous);
		} else {
			GetRDATData(VENMSR_MASTER->passkey->fileno,previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(VENMSR_MASTER);
		if(VENMSR_MASTER!=NULL) FreeMaintainMaster(VENMSR_MASTER);
		if(VENINV_MASTER!=NULL) FreeMaintainMaster(VENINV_MASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL) 
	{ 
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,VENMSR_MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
static void venadrfunc(RDArsrc *mtnrsrc)
{
        readallwidgets(mtnrsrc);
        rsrc2filerecord(VENMSR_MASTER->passkey->fileno,mtnrsrc);
        MTNSCRN_EXECUTEPROGRAM("venadr",mtnrsrc,"VENADR KEY",VENMSR_MASTER->passkey->fileno,1,NULL);
}
