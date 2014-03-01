/* paybc2.c - Xpert Benefit's Coordinator Maintain Benefit */
/*lint -library */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct dedstructs
{
	RDATData *previous;
};
typedef struct dedstructs dedstruct;

extern short getsupportingrecords(RDArsrc *,MaintainMaster *,short,short);
extern short paydedm,perdmg,payddms;
extern MakeBrowseList *mbl_paydedm;
MakeBrowseList *mbl_payddms=NULL;
extern char editable;
extern char *PERID;
extern MaintainMaster *MTNMASTER;
static void quit_record(RDArsrc *,dedstruct *);
static void quit_record_test(RDArsrc *,dedstruct *);
static void save_record_quit(RDArsrc *mtnrsrc,dedstruct *mtn);
static void save_record(RDArsrc *,dedstruct *,short);
static void save_record_nup(RDArsrc *,dedstruct *);
static void save_record_up(RDArsrc *,dedstruct *);
static void save_record_now(RDArsrc *,dedstruct *);
static void save_record_now1(RDArsrc *,dedstruct *);
static void getrecordcb(RDArsrc *,dedstruct *);
static void readscreen(RDArsrc *,dedstruct *);
static void save_check(RDArsrc *,dedstruct *,short);
static void setfiles(RDArsrc *,MaintainMaster *);
void paydedmm(short,void *);

/* BROWSE PAYDDMS STARTING */
static void doexit_browse_payddms(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_payddms(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_payddms(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 

	ZERNRD(payddms);
	if(ADVEQLNRDKEYsec(payddms,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(payddms,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",&posid);
	}
	filerecord2rsrc(payddms,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PAYDEDM][DEDUCTION DESCRIPTION IDENTIFICATION]",posid);
	FINDFLDSETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_payddms(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_payddms(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_payddms(MakeBrowseList *BL)
{
	ZERNRD(payddms);
}
static short break_browse_payddms(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_payddms(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_payddms!=NULL)
	{
		doexit_browse_payddms(mbl_payddms);
		mbl_payddms=NULL;
	}
	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT DEDUCTION DESCRIPTION IDENTIFICATION BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT DEDUCTION DESCRIPTION IDENTIFICATION SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT DEDUCTION DESCRIPTION IDENTIFICATION DEFINE LIST");
	ZERNRD(payddms);
	addDFincvir(searchrsrc,"PAYROLL","PAYDDMS",NULL,payddms);
	addDFincvir(definelist,"PAYROLL","PAYDDMS",NULL,payddms);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_payddms=xbrowse(payddms,1,mainrsrc,searchrsrc,definelist,init_browse_payddms,break_browse_payddms,select_browse_payddms,quit_browse_payddms,okfunc_browse_payddms,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* BROWSE PAYDDMS ENDING */
static void setfiles(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER)
{
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(paydedm,mtnrsrc);
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,FALSE);
	ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	ZERNRD(paydedm);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(paydedm,mtnrsrc);
}
void paydedmm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	int x;
	short nofields=0;
	dedstruct *mtn=NULL;
	char *ddid=NULL,*name=NULL;
	
	mtnrsrc=RDArsrcNEW(MTNMASTER->module,"BENEFITS COORDINATOR MAINTAIN SCREEN");
	mtn=Rmalloc(sizeof(dedstruct));
	mtn->previous=NULL;
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	ZERNRD(paydedm);
	mtn->previous=RDATDataNEW(paydedm);
	nofields=NUMFLDS(paydedm);
	fields=FLDPOINTER(paydedm);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			nonlstmakefld(mtnrsrc,paydedm,fldx,TRUE);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
			}
			sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,setfiles,MTNMASTER);
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[PAYDEDM][PERSONNEL IDENTIFICATION]",
		getrecordcb,mtn);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			/* not used */
			break;
		case 2:
			FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",PERID);
			FINDFLDSETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",ddid);
			if(ddid!=NULL) Rfree(ddid);
			FINDRSCSETSTRING(mtnrsrc,"[PAYDEDM][PERSONNEL IDENTIFICATION]",PERID);
			if(ADVEQLNRDsec(paydedm,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(paydedm,1);
				ReadRDAScrolledLists(mtnrsrc);
				if(ADVEQLNRDsec(paydedm,1,SCRNvirtualSubData,mtnrsrc))
				{
					rsrc2filerecord(paydedm,mtnrsrc);
					FINDFLDSETCHAR(paydedm,"ACTIVE",TRUE);
					FINDRSCSETCHAR(mtnrsrc,"[PAYDEDM][ACTIVE]",TRUE);		
				} else {
					filerecord2rsrc(paydedm,mtnrsrc);
					GetRDATData(paydedm,mtn->previous);
				}
			} else {
				filerecord2rsrc(paydedm,mtnrsrc);
				GetRDATData(paydedm,mtn->previous);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(paydedm,1,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(paydedm);
				ReadRDAScrolledLists(mtnrsrc);
				if(ADVEQLNRDsec(paydedm,1,SCRNvirtualSubData,mtnrsrc))
				{
					rsrc2filerecord(paydedm,mtnrsrc);
				} else {
					filerecord2rsrc(paydedm,mtnrsrc);
					GetRDATData(paydedm,mtn->previous);
				}
			} else {
				filerecord2rsrc(paydedm,mtnrsrc);
				GetRDATData(paydedm,mtn->previous);
			}
			break;
	}
	FINDRSCSETEDITABLE(mtnrsrc,"[PAYDEDM][PERSONNEL IDENTIFICATION]",FALSE);
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"BROWSE PAYDDMS",TRUE,browse_payddms,mtn);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,TRUE,quit_record,mtn,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,dedstruct *mtn,short update)
{
	char *desc=NULL;

	readscreen(mtnrsrc,mtn);
	if(!SAVE_CHECK(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MTNMASTER->save_expression))
	{
		if(EVALUATEbol(MTNMASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
				ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MTNMASTER->save_warning))
	{
		if(EVALUATEbol(MTNMASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",desc,(update ? save_record_now:save_record_now1),NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update);
}
static void save_record(RDArsrc *mtnrsrc,dedstruct *mtn,short update_list)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(paydedm,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(paydedm,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(paydedm,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_paydedm,paydedm,MTNMASTER->module,mtnrsrc);
		quit_record(mtnrsrc,mtn);
	}
}
static void save_record_now(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void save_record_quit(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecordcb(RDArsrc *mtnrsrc,dedstruct *mtn)
{
	char *perid=NULL,*ddid=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[PAYDEDM][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETSTRING(mtnrsrc,"[PAYDEDM][DEDUCTION DESCRIPTION IDENTIFICATION]",&ddid);
	ZERNRD(paydedm);
	FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",ddid);
	if(ADVEQLNRDsec(paydedm,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(paydedm,1);
		updatefilersrc(paydedm,mtnrsrc);	
		ClearRDATData(mtn->previous);
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,FALSE);
	} else {
		updatefilerecord2rsrc(paydedm,mtnrsrc);	
		GetRDATData(paydedm,mtn->previous);
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,TRUE);
	}
	if(perid!=NULL) Rfree(perid);
	if(ddid!=NULL) Rfree(ddid);
	updateSCRNvirtuals(mtnrsrc);
}
