/* subemp.c - Xpert Position Addenda Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "subemp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subemp.exe"
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
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *);
static void getsupportingrecords(RDArsrc *);
static void subempm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short filenum=(-1),msrnum=(-1),dmgnum=(-1),dmg2num=(-1);
static char editable=TRUE;
static char *module="SUBMGT";
static MakeBrowseList *mbl_subemp=NULL;
static MaintainMaster *MTNMASTER=NULL;
static void browse_subemp(RDArsrc *,mtnstruct *);

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
static void browse_subemp(RDArsrc *mtnrsrc,mtnstruct *mtn)
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
	addDFincvir(mainrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(mainrsrc,module,"SUBEMP",NULL,filenum);
	addDFincvir(definelist,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(definelist,module,"SUBEMP",NULL,filenum);
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
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	SCRNvirtual2rsrc(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_subemp=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,
		new_record,NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN SUBEMP")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SUBEMP",&editable))==(-1)) return;
	if((msrnum=APPOPNNRD(module,"SUBMASTR",TRUE,FALSE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((dmg2num=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN SUBEMP");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SUBEMP",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				subempm(1,NULL);
				break;
			case 1:
				browse_subemp(NULL,NULL);
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
		if(dmg2num!=(-1)) CLSNRD(dmg2num);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	mbl_subemp=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			subempm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else { 
		subempm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	subempm(2,NULL);
} 
static void subempm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	ZERNRD(filenum);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=RDATDataNEW(filenum);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[SUBEMP][PERSONNEL IDENTIFICATION]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBEMP][EMPLOYEE IDENTIFICATION]",getrecordcb,mtn);
	if(msrnum!=(-1)) file2rsrc(msrnum,mtnrsrc,FALSE);
	if(dmgnum!=(-1)) file2rsrc(dmgnum,mtnrsrc,FALSE);
	if(dmg2num!=(-1)) singlefile2rsrc(dmg2num,mtnrsrc,FALSE);
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
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
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
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,dmg2num);
	addDFincvir(mtnrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mtnrsrc,module,"SUBEMP",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_subemp!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
		}
	}
	if(mbl_subemp==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_subemp,mtn);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_subemp==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char delflag=FALSE,*warnmessage=NULL,*perid1=NULL,*perid2=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"[SUBEMP][DELETEFLAG]",&delflag);
	FINDRSCGETSTRING(mtnrsrc,"[SUBEMP][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(mtnrsrc,"[SUBEMP][EMPLOYEE IDENTIFICATION]",&perid2);
	if(!delflag)
	{
		FINDFLDSETSTRING(msrnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(msrnum,1))
		{
			warnmessage=Rmalloc(RDAstrlen(perid1)+180);
			sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETSTRING(filenum,"EMPLOYEE IDENTIFICATION",perid2);
			getsupportingrecords(mtnrsrc);
			if(perid1!=NULL) Rfree(perid1);
			if(perid2!=NULL) Rfree(perid2);
			return;
		} else {
			FINDFLDGETCHAR(msrnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+180);
				sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
				FINDFLDSETSTRING(filenum,"EMPLOYEE IDENTIFICATION",perid2);
				getsupportingrecords(mtnrsrc);
				if(perid1!=NULL) Rfree(perid1);
				if(perid2!=NULL) Rfree(perid2);
				return;
                        }
		}
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(dmgnum,1))
		{
			warnmessage=Rmalloc(RDAstrlen(perid1)+180);
			sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETSTRING(filenum,"EMPLOYEE IDENTIFICATION",perid2);
			getsupportingrecords(mtnrsrc);
			if(perid1!=NULL) Rfree(perid1);
			if(perid2!=NULL) Rfree(perid2);
			return;
		} else {
			FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+180);
				sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value for the [PERDMG] file.  Please select a valid Personnel Identification from this file to save this record.",(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
				FINDFLDSETSTRING(filenum,"EMPLOYEE IDENTIFICATION",perid2);
				getsupportingrecords(mtnrsrc);
				if(perid1!=NULL) Rfree(perid1);
				if(perid2!=NULL) Rfree(perid2);
				return;
			}
		}
		FINDFLDSETSTRING(dmg2num,"PERSONNEL IDENTIFICATION",perid2);
		if(EQLNRDsec(dmg2num,1))
		{
			warnmessage=Rmalloc(RDAstrlen(perid2)+180);
			sprintf(warnmessage,"The Employee Identification of [%s] is an invalid value for the [PERDMG] file.  Please select a valid Personnel Identification from the [PERDMG] file to save this record.",(perid2==NULL?"":perid2));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EMPLOYEE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETSTRING(filenum,"EMPLOYEE IDENTIFICATION",perid2);
			getsupportingrecords(mtnrsrc);
			if(perid1!=NULL) Rfree(perid1);
			if(perid2!=NULL) Rfree(perid2);
			return;
		} else {
			FINDFLDGETCHAR(dmg2num,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+180);
				sprintf(warnmessage,"The Employee Identification of [%s] is a deleted value.  Please select a valid Employee Identification to save this record.",(perid2==NULL?"":perid2));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED EMPLOYEE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
				FINDFLDSETSTRING(filenum,"EMPLOYEE IDENTIFICATION",perid2);
				getsupportingrecords(mtnrsrc);
				if(perid1!=NULL) Rfree(perid1);
				if(perid2!=NULL) Rfree(perid2);
				return;
			}
		}
	}
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	save_record(mtnrsrc,mtn,update_list);
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
		updatebrowse(update_list,mbl_subemp,filenum,module,mtnrsrc);
		if(mbl_subemp!=NULL) quit_record(mtnrsrc,mtn);
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
		if(dmg2num!=(-1)) CLSNRD(dmg2num);
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
	char *perid=NULL,*perid2=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[SUBEMP][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETSTRING(mtnrsrc,"[SUBEMP][EMPLOYEE IDENTIFICATION]",&perid2);
	ZERNRD(filenum);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(filenum,"EMPLOYEE IDENTIFICATION",perid2);
	getrecord(mtnrsrc,mtn);
	if(perid!=NULL) Rfree(perid);
	if(perid2!=NULL) Rfree(perid2);
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
	if(dmg2num!=(-1))
	{
		FIELDCOPY(filenum,"EMPLOYEE IDENTIFICATION",dmg2num,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmg2num,1)) KEYNRD(dmg2num,1);
		singlefilerecord2rsrc(dmg2num,mtnrsrc);
	}
}
