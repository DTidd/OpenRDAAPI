/* subpos.c - Xpert Position Addenda Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "subpos.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subpos.exe"
#endif
#include <app.hpp>



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

struct mtnstructs
{
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void quit_record(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static short check_peridmsr(char *,char *,char *);
static short check_periddmg(char *,char *,char *);
static short check_posid(char *,char *,char *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *);
static void getsupportingrecords(RDArsrc *);
static void subposm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short filenum=(-1),msrnum=(-1),dmgnum=(-1),posnum=(-1);
static char editable=TRUE;
static char *module="SUBMGT";
static MakeBrowseList *mbl_pos=NULL;
static MaintainMaster *MTNMASTER=NULL;
static void browse_subpos(RDArsrc *,mtnstruct *);

static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,mtn->previous))
	{
		desc=QUITvfydesc(filenum,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_quit,
			quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
static void browse_subpos(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL) 
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	addDFincvir(mainrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posnum);
	addDFincvir(searchrsrc,module,"SUBPOS",NULL,filenum);
	addDFincvir(searchrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,posnum);
	addDFincvir(definelist,module,"SUBPOS",NULL,filenum);
	addDFincvir(definelist,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,posnum);
	ApplyPassKey(MTNMASTER->passkey);
	if(dmgnum!=(-1))
	{
		COPYFIELD(filenum,dmgnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmgnum,1)) 
		{
			ZERNRD(dmgnum);
		} else {
			file2rsrc(dmgnum,mainrsrc,FALSE);
			filerecord2rsrc(dmgnum,mainrsrc);
		}
	}
	if(msrnum!=(-1))
	{
		COPYFIELD(filenum,msrnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(msrnum,1)) 
		{
			ZERNRD(msrnum);
		} else {
			file2rsrc(msrnum,mainrsrc,FALSE);
			filerecord2rsrc(msrnum,mainrsrc);
		}
	}
	if(posnum!=(-1))
	{
		COPYFIELD(filenum,posnum,"POSITION IDENTIFICATION");
		if(EQLNRDsec(posnum,1)) 
		{
			ZERNRD(posnum);
		} else {
			file2rsrc(posnum,mainrsrc,FALSE);
			filerecord2rsrc(posnum,mainrsrc);
		}
	}
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	SCRNvirtual2rsrc(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_pos=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,
		definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
		MTNMASTER->passkey);
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN SUBPOS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SUBPOS",&editable))==(-1)) return;
	if((msrnum=APPOPNNRD(module,"SUBMASTR",TRUE,FALSE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((posnum=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN SUBPOS");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SUBPOS",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				subposm(1,NULL);
				break;
			case 1:
				browse_subpos(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
void doexit(MakeBrowseList *blist)
{
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
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(posnum!=(-1)) CLSNRD(posnum);
		if(msrnum!=(-1)) CLSNRD(msrnum);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	mbl_pos=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			subposm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		subposm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	subposm(2,NULL);
} 
static void subposm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	mtn=Rmalloc(sizeof(mtnstruct));
	ZERNRD(filenum);
	mtn->previous=RDATDataNEW(filenum);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[SUBPOS][PERSONNEL IDENTIFICATION]",getrecordcb,
		mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBPOS][POSITION IDENTIFICATION]",getrecordcb,
		mtn);
	if(msrnum!=(-1)) file2rsrc(msrnum,mtnrsrc,FALSE);
	if(dmgnum!=(-1)) file2rsrc(dmgnum,mtnrsrc,FALSE);
	if(posnum!=(-1)) file2rsrc(posnum,mtnrsrc,FALSE);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,mtn->previous);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			break;
		case 0:
		default:
			if(EQLNRDKEYsec(filenum,MTNMASTER->passkey->keyno,targetkey))
			{
				ZERNRD(filenum);
				ApplyPassKey(MTNMASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,mtn->previous);
			}
			break;
	}
	getsupportingrecords(mtnrsrc);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(mtnrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mtnrsrc,module,"SUBPOS",NULL,filenum);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posnum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_pos!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
		}
	}
	if(mbl_pos==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_subpos,mtn);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_pos==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static short check_peridmsr(char *warnmessage,char *header,char *perid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(msrnum,"PERSONNEL IDENTIFICATION",perid1);
	if(EQLNRDsec(msrnum,1))
	{
		sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  It does not exist in the [SUBMGT][SUBMASTR] file.  Please select a valid Personnel Identification.",(perid1==NULL?"":perid1));
		sprintf(header,"INVALID PERSONNEL IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(msrnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Personnel Identification of [%s] is deleted value.  It exists in the [SUBMGT][SUBMASTR] but is in a delted state.  Please select a valid Personnel Identification.",(perid1==NULL?"":perid1));
			sprintf(header,"DELETED PERSONNEL IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_periddmg(char *warnmessage,char *header,char *perid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
	if(EQLNRDsec(dmgnum,1))
	{
		sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  It does not exist in the [PRSNNL][PERDMG] file.  Please select a valid Personnel Identification.",(perid1==NULL?"":perid1));
		sprintf(header,"INVALID PERSONNEL IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value. It exists in the [PRSNNL][PERDMG] file but is in a deleted state.  Please select a valid Personnel Identification from this file.",(perid1==NULL?"":perid1));
			sprintf(header,"DELETED PERSONNEL IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static short check_posid(char *warnmessage,char *header,char *posid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(posnum,"POSITION IDENTIFICATION",posid1);
	if(EQLNRDsec(posnum,1))
	{
		sprintf(warnmessage,"The Position Identification of [%s] is an invalid value.  It does not exist in the [POSTRK][POSMSTR] file.  Please select a valid Position Identification.",(posid1==NULL?"":posid1));
		sprintf(header,"INVALID POSITION IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(posnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Position Identification of [%s] is a deleted value. It exists for the [POSTRK][POSMSTR] file but is in a deleted state.  Please select a valid Position Identification from this file.",(posid1==NULL?"":posid1));
			sprintf(header,"DELETED POSITION IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char delflag=FALSE,*warnmessage=NULL,*header=NULL;
	char *perid1=NULL,*posid1=NULL;
	short return_value=FALSE;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[SUBPOS][DELETEFLAG]",&delflag);
	FINDRSCGETSTRING(mtnrsrc,"[SUBPOS][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(mtnrsrc,"[SUBPOS][POSITION IDENTIFICATION]",&posid1);
	if(!delflag)
	{
		warnmessage=Rmalloc(500);
		header=Rmalloc(60);
		if(check_peridmsr(warnmessage,header,perid1))
		{
			return_value=TRUE;
		} else if(check_periddmg(warnmessage,header,perid1))
		{
			return_value=TRUE;
		} else if(check_posid(warnmessage,header,posid1))
		{
			return_value=TRUE;
		}
	}
	if(return_value) WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(perid1!=NULL) Rfree(perid1);
	if(posid1!=NULL) Rfree(posid1);
	if(return_value==FALSE) save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL;

	rsrc2filerecord(filenum,mtnrsrc);
	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		WRTTRANS(filenum,0,NULL,mtn->previous);
		updatebrowse(update_list,mbl_pos,filenum,module,mtnrsrc);
		if(mbl_pos!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(filenum,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(msrnum!=(-1)) CLSNRD(msrnum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(posnum!=(-1)) CLSNRD(posnum);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid1=NULL,*posid1=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[SUBPOS][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(mtnrsrc,"[SUBPOS][POSITION IDENTIFICATION]",&posid1);
	ZERNRD(filenum);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(filenum,"POSITION IDENTIFICATION",posid1);
	getrecord(mtnrsrc,mtn);
	if(perid1!=NULL) Rfree(perid1);
	if(posid1!=NULL) Rfree(posid1);
	computeallSCRNvirtuals(mtnrsrc);
   	updateallrsrc(mtnrsrc);
}
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno)) 
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		ClearRDATData(mtn->previous);
	} else {
		filerecord2rsrc(filenum,mtnrsrc);	
		GetRDATData(filenum,mtn->previous);
	}
	getsupportingrecords(mtnrsrc);
}
static void getsupportingrecords(RDArsrc *mtnrsrc)
{
	if(msrnum!=(-1))
	{
		COPYFIELD(filenum,msrnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(msrnum,1)) KEYNRD(msrnum,1);
		filerecord2rsrc(msrnum,mtnrsrc);
	}
	if(dmgnum!=(-1))
	{
		COPYFIELD(filenum,dmgnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmgnum,1)) KEYNRD(dmgnum,1);
		filerecord2rsrc(dmgnum,mtnrsrc);
	}
	if(posnum!=(-1))
	{
		COPYFIELD(filenum,posnum,"POSITION IDENTIFICATION");
		if(EQLNRDsec(posnum,1)) KEYNRD(posnum,1);
		filerecord2rsrc(posnum,mtnrsrc);
	}
}
