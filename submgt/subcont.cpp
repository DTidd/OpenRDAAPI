/* subcont.c - Xpert Position Addenda Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "subcont.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subcont.exe"
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
	APPlib *rsptypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void quit_record(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static short check_peridmsr(char *,char *,char *);
static short check_periddmg(char *,char *,char *);
static short check_empid(char *,char *,char *);
static short check_posid(char *,char *,char *);
static short check_locid(char *,char *,char *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *);
static void empidcb(RDArsrc *);
static void posidcb(RDArsrc *);
static void locidcb(RDArsrc *);
static void getsupportingrecords(RDArsrc *);
static void subcontm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short filenum=(-1),msrnum=(-1),dmgnum=(-1);
static short dmg2num=(-1),posnum=(-1),locnum=(-1);
static char editable=TRUE;
static char *module="SUBMGT";
static MakeBrowseList *mbl_subcont=NULL;
static MaintainMaster *MTNMASTER=NULL;
static void browse_subcont(RDArsrc *,mtnstruct *);
static void setrsptype(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);

static void setrsptype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short selrsptype=0;
	int x=0;
	char *message=NULL;

	FINDFLDGETSHORT(filenum,"RESPONSE TYPE",&selrsptype);
	if((-1)<selrsptype && selrsptype<mtn->rsptypes->numlibs)
	{
		x=selrsptype;
	} else {
		message=Rmalloc(110+RDAstrlen(mtn->rsptypes->libs[0]));
		sprintf(message,"The record selected contains an invalid responce type.  Therefore, the RESPONSE TYPES list will default to [%s].",mtn->rsptypes->libs[0]);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID RESPONSE TYPE",message,NULL,NULL,TRUE,NULL);
		if(message!=NULL) Rfree(message);
		x=0;
	}
	FINDRSCSETINT(mtnrsrc,"RESPONSE TYPES",x);
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
        save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readscreen(mtnrsrc,mtn);
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
static void browse_subcont(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->rsptypes!=NULL) Rfree(mtn->rsptypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	addDFincvir(mainrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(searchrsrc,module,"SUBCTACT",NULL,filenum);
	addDFincvir(searchrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(definelist,module,"SUBCTACT",NULL,filenum);
	addDFincvir(definelist,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
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
	mbl_subcont=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,
		new_record,NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN SUBCTACT")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SUBCTACT",&editable))==(-1)) return;
	if((msrnum=APPOPNNRD(module,"SUBMASTR",TRUE,FALSE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((dmg2num=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((posnum=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((locnum=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN SUBCTACT");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SUBCTACT",module);
                doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				subcontm(1,NULL);
				break;
			case 1:
				browse_subcont(NULL,NULL);
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
		if(posnum!=(-1)) CLSNRD(posnum);
		if(locnum!=(-1)) CLSNRD(locnum);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	mbl_subcont=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			subcontm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		subcontm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	subcontm(2,NULL);
} 
static APPlib *makersplist()
{
	APPlib *rsptypes=NULL;

	rsptypes=APPlibNEW();
	addAPPlib(rsptypes,"Accepted");
	addAPPlib(rsptypes,"Refused");
	addAPPlib(rsptypes,"Other");
	return(rsptypes);
}
static void subcontm(short dowhich,void *targetkey)
{
	char *date=NULL,*timex=NULL;
	int which_type=0;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	short nofields=0,x=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;

	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	ZERNRD(filenum);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->rsptypes=NULL;
	mtn->previous=RDATDataNEW(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=editable;
			if(!RDAstrcmp(fldx->name,"RESPONSE TYPE"))
			{
				mtn->rsptypes=makersplist();
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"RESPONSE TYPES",
						&which_type,edit_rsrc,
						NULL,mtn->rsptypes->numlibs,
						&mtn->rsptypes->libs,NULL);
				}
			} else {
				nonlstmakefld(mtnrsrc,filenum,fldx,edit_rsrc);
			}
		}
	}
	nofields=0;
	FINDRSCSETFUNC(mtnrsrc,"[SUBCTACT][PERSONNEL IDENTIFICATION]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBCTACT][TRANSACTION NUMBER]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBCTACT][EMPLOYEE IDENTIFICATION]",empidcb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[SUBCTACT][POSITION IDENTIFICATION]",posidcb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[SUBCTACT][LOCATION IDENTIFICATION]",locidcb,NULL);
	if(msrnum!=(-1)) file2rsrc(msrnum,mtnrsrc,FALSE);
	if(dmgnum!=(-1)) file2rsrc(dmgnum,mtnrsrc,FALSE);
	if(dmg2num!=(-1)) singlefile2rsrc(dmg2num,mtnrsrc,FALSE);
	if(posnum!=(-1)) file2rsrc(posnum,mtnrsrc,FALSE);
	if(locnum!=(-1)) file2rsrc(locnum,mtnrsrc,FALSE);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][ENTRY TIME]",timex);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				setrsptype(mtnrsrc,mtn);
				GetRDATData(filenum,mtn->previous);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][SOURCE USER]",USERLOGIN);
			FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][ENTRY DATE]",date);
			FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][ENTRY TIME]",timex);
			break;
		case 0:
		default:
			if(EQLNRDKEYsec(filenum,MTNMASTER->passkey->keyno,targetkey))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey(MTNMASTER->passkey);		
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[SUBCTACT][ENTRY TIME]",timex);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				setrsptype(mtnrsrc,mtn);
				GetRDATData(filenum,mtn->previous);
			}
			break;
	}
	getsupportingrecords(mtnrsrc);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,dmg2num);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posnum);
	addDFincvir(mtnrsrc,"POSTRK","POSLMST",NULL,locnum);
	addDFincvir(mtnrsrc,module,"SUBMASTR",NULL,msrnum);
	addDFincvir(mtnrsrc,module,"SUBCTACT",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_subcont!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
		}
	}
	if(mbl_subcont==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_subcont,mtn);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_subcont==NULL?TRUE:FALSE));
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"RESPONSE TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(filenum,"RESPONSE TYPE",type);
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
static short check_empid(char *warnmessage,char *header,char *perid2)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(dmg2num,"PERSONNEL IDENTIFICATION",perid2);
	if(EQLNRDsec(dmg2num,1))
	{
		sprintf(warnmessage,"The Employee Identification of [%s] is an invalid value.  It does not exist as a Personnel Identification in the [PRSNNL][PERDMG] file.  Please select a valid Personnel Identification.",(perid2==NULL?"":perid2));
		sprintf(header,"INVALID EMPLOYEE IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(dmg2num,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Employee Identification of [%s] is a deleted value.  It exists as a Personnel Identification in the [PRSNNL][PERDMG] file but is in a deleted state.  Please select a valid Personnel Identification from this file.",(perid2==NULL?"":perid2));
			sprintf(header,"DELETED EMPLOYEE JOB NUMBER!");
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
static short check_locid(char *warnmessage,char *header,char *locid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(locnum,"LOCATION IDENTIFICATION",locid1);
	if(EQLNRDsec(locnum,1))
	{
		sprintf(warnmessage,"The Location Identification of [%s] is an invalid value.  It does not exist in the [POSTRK][POSLMST] file.  Please select a valid Location Identification.",(locid1==NULL?"":locid1));
		sprintf(header,"INVALID LOCATION IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(locnum,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Location Identification of [%s] is a deleted value. It exists for the [POSTRK][POSLMST] file but is in a deleted state.  Please select a valid Location Identification from this file.",(locid1==NULL?"":locid1));
			sprintf(header,"DELETED LOCATION IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char delflag=FALSE,*warnmessage=NULL,*header=NULL;
	char *perid1=NULL,*perid2=NULL,*posid1=NULL,*locid1=NULL;
	int transno=0;
	short return_value=FALSE;

	readscreen(mtnrsrc,mtn);
	FINDRSCGETCHAR(mtnrsrc,"[SUBCTACT][DELETEFLAG]",&delflag);
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][EMPLOYEE IDENTIFICATION]",&perid2);
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][POSITION IDENTIFICATION]",&posid1);
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][LOCATION IDENTIFICATION]",&locid1);
	FINDRSCGETINT(mtnrsrc,"[SUBCTACT][TRANSACTION NUMBER]",&transno);
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
		} else if(check_empid(warnmessage,header,perid2))
		{
			return_value=TRUE;
		} else if(check_posid(warnmessage,header,posid1))
		{
			return_value=TRUE;
		} else if(check_locid(warnmessage,header,locid1))
		{
			return_value=TRUE;
		}
	}
	if(return_value) WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(posid1!=NULL) Rfree(posid1);
	if(locid1!=NULL) Rfree(locid1);
	if(return_value==FALSE) save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL;

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
		updatebrowse(update_list,mbl_subcont,filenum,module,mtnrsrc);
		if(mbl_subcont!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(filenum,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->rsptypes!=NULL) Rfree(mtn->rsptypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(msrnum!=(-1)) CLSNRD(msrnum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(dmg2num!=(-1)) CLSNRD(dmg2num);
		if(posnum!=(-1)) CLSNRD(posnum);
		if(locnum!=(-1)) CLSNRD(locnum);
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
static void empidcb(RDArsrc *mtnrsrc)
{
	char *empid=NULL,*warnmessage=NULL,*header=NULL;

	readwidget(mtnrsrc,"[SUBCTACT][EMPLOYEE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][EMPLOYEE IDENTIFICATION]",&empid);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_empid(warnmessage,header,empid))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(empid!=NULL) Rfree(empid);
	if(EQLNRDsec(dmg2num,1)) KEYNRD(dmg2num,1);
	updatesinglerecord2rsrc(dmg2num,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void posidcb(RDArsrc *mtnrsrc)
{
	char *posid=NULL,*warnmessage=NULL,*header=NULL;

	readwidget(mtnrsrc,"[SUBCTACT][POSITION IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][POSITION IDENTIFICATION]",&posid);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_posid(warnmessage,header,posid))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(posid!=NULL) Rfree(posid);
	if(EQLNRDsec(posnum,1)) KEYNRD(posnum,1);
	updatefilerecord2rsrc(posnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void locidcb(RDArsrc *mtnrsrc)
{
	char *locid=NULL,*warnmessage=NULL,*header=NULL;

	readwidget(mtnrsrc,"[SUBCTACT][LOCATION IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][LOCATION IDENTIFICATION]",&locid);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_locid(warnmessage,header,locid))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(locid!=NULL) Rfree(locid);
	if(EQLNRDsec(locnum,1)) KEYNRD(locnum,1);
	updatefilerecord2rsrc(locnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL;
	int transno=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[SUBCTACT][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETINT(mtnrsrc,"[SUBCTACT][TRANSACTION NUMBER]",&transno);
	ZERNRD(filenum);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(filenum,"TRANSACTION NUMBER",transno);
	getrecord(mtnrsrc,mtn);
	if(perid!=NULL) Rfree(perid);
	computeallSCRNvirtuals(mtnrsrc);
   	updateallrsrc(mtnrsrc);
}
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *date=NULL,*timex=NULL;

	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
		FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		ClearRDATData(mtn->previous);
	} else {
		filerecord2rsrc(filenum,mtnrsrc);	
		setrsptype(mtnrsrc,mtn);
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
	if(posnum!=(-1))
	{
		COPYFIELD(filenum,posnum,"POSITION IDENTIFICATION");
		if(EQLNRDsec(posnum,1)) KEYNRD(posnum,1);
		filerecord2rsrc(posnum,mtnrsrc);
	}
	if(locnum!=(-1))
	{
		COPYFIELD(filenum,locnum,"LOCATION IDENTIFICATION");
		if(EQLNRDsec(locnum,1)) KEYNRD(locnum,1);
		filerecord2rsrc(locnum,mtnrsrc);
	}
}
