/* purapo.c - Xpert Add Purchase Order Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "purapo.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purapo.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <purord.hpp>
#include <fin.hpp>
/*CPP_OPART purapom */
/*CPP_OPART puraline */
/*CPP_OPART puratran */
/*CPP_OPART puraven */
/*CPP_OPART puravenm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

short purids=(-1),msrnum=(-1),sitenum=(-1),vennum=(-1),venadrnum=(-1);
short venadr2num=(-1);
short venadrkeyno=(-1);
short linenum=(-1),accnum=(-1),trannum=(-1);
short eyrnum=(-1),ryrnum=(-1),byrnum=(-1);
short expnum=(-1),revnum=(-1),balnum=(-1),finyrnum=(-1);
short catnum=(-1),ctlgnum=(-1),cvennum=(-1),cvpnum=(-1);
short fixmst=(-1),banknum=(-1);
RDAfinmgt *finsetup=NULL;
RDAvenpmt *venpmt=NULL;
RDApurord *purord=NULL;
void browse_purmst(RDArsrc *mtnrsrc,RDATData *previous);
char *reportname=NULL,*module="PURORD";
extern int PO_COUNT;
extern char *POID;
int yearid=0;
char editmsr=TRUE,editline=TRUE,edittrans=TRUE,editven=TRUE;
extern MakeBrowseList *mbl_purmsr;
extern void purmsrm(short,void *);
extern void readscreen(RDArsrc *,RDATData *);
extern APPlib *acctavl,*debitavl,*creditavl,*sitesavl;
extern void get_POIDS(RDArsrc *,RDATData *);
extern void seteditable(RDArsrc *,RDATData *,short);
extern void setlists(RDArsrc *,RDATData *,short);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static short selectfunc(MakeBrowseList *b);
MaintainMaster *VENMSR_MTNMASTER=NULL,*PURMST_MTNMASTER=NULL;

struct HoldIts
{
	RDArsrc *mtnrsrc;
	RDATData *previous;
};
typedef struct HoldIts HoldIt;

void browse_purmst(RDArsrc *mtnrsrc,RDATData *previous)
{
	HoldIt *h=NULL;
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	h=Rmalloc(sizeof(HoldIt));
	h->mtnrsrc=mtnrsrc;
	h->previous=previous;
	mainrsrc=RDArsrcNEW(PURMST_MTNMASTER->module,PURMST_MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(PURMST_MTNMASTER->module,PURMST_MTNMASTER->search_browse);
	definelist=RDArsrcNEW(PURMST_MTNMASTER->module,PURMST_MTNMASTER->define_list);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,PURMST_MTNMASTER);
	mbl_purmsr=ADVSbrowse(PURMST_MTNMASTER->passkey->fileno,1,
		mainrsrc,searchrsrc,definelist,
		NULL,NULL,selectfunc,doexit,okfunc,NULL,h,
		TRUE,NULL,1,1,1,FALSE);
}
int main(int argc,char **argv)
{
	PassKey *tmpPassKeyPURMST=NULL;
	PassKey *tmpPassKeyVENMSR=NULL;
	short keyno=(-1);
	char *warnmessage=NULL;
	char *mtnname_purmst="ADD PURMST";
	char *mtnname_venmsr="ADD PO VENMSR";

	if(InitializeSubsystems(argc,argv,module,"ADD PURCHASE ORDERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	GetAddList();
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("POAUDIT");
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
	venpmt=RDAvenpmtNEW();
	if(getvenpmtbin(venpmt)==(-1)) return;
	purord=RDApurordNEW();
	if(getpurordbin(purord)==(-1)) return;
	PURMST_MTNMASTER=MaintainMasterNew("PURORD","ADD PURMST");
	if(findMaintainMaster(PURMST_MTNMASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(PURMST_MTNMASTER->module)+RDAstrlen(mtnname_purmst));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname_purmst,PURMST_MTNMASTER->module);
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((msrnum=APPOPNNRDEDITABLE(PURMST_MTNMASTER->module,PURMST_MTNMASTER->mainfile,
			&editmsr))==(-1)) return;
		SetSupportingWritable(PURMST_MTNMASTER,"PURORD","PURLIN",1,TRUE,&editline); 
		SetSupportingWritable(PURMST_MTNMASTER,"PURORD","PURACC",1,TRUE,&edittrans); 
		SetSupportingWritable(PURMST_MTNMASTER,"PURORD","PURTRN",1,TRUE,&edittrans); 
		SetSupportingWritable(PURMST_MTNMASTER,"VENPMT","VENMSR",1,TRUE,&editven); 
		if(!OPEN_MASTER_FILES(PURMST_MTNMASTER))
		{
			doexit(NULL);
			return;
		}
		if((purids=APPReturnSupportingFileno(PURMST_MTNMASTER,"PURORD","PURIDS",1))==(-1)) return;
		if((sitenum=APPReturnSupportingFileno(PURMST_MTNMASTER,"PURORD","PURSIT",1))==(-1)) return;
		if((linenum=APPReturnSupportingFileno(PURMST_MTNMASTER,"PURORD","PURLIN",1))==(-1)) return;
		if((accnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"PURORD","PURACC",1))==(-1)) return;
		if((trannum=APPReturnSupportingFileno(PURMST_MTNMASTER,"PURORD","PURTRN",1))==(-1)) return;
		if((vennum=APPReturnSupportingFileno(PURMST_MTNMASTER,"VENPMT","VENMSR",1))==(-1)) return;
		if((venadrnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"VENPMT","VENADR",1))==(-1)) return;
		if((venadr2num=APPReturnSupportingFileno(PURMST_MTNMASTER,"VENPMT","VENADR",2))==(-1)) return;
		if((expnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"FINMGT","FINEACC",1))==(-1)) return;
		if((revnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"FINMGT","FINRACC",1))==(-1)) return;
		if((balnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"FINMGT","FINBACC",1))==(-1)) return;
		if((eyrnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"FINMGT","FINEYR",1))==(-1)) return;
		if((ryrnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"FINMGT","FINRYR",1))==(-1)) return;
		if((byrnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"FINMGT","FINBYR",1))==(-1)) return;
		if((finyrnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"FINMGT","FINYEAR",1))==(-1)) return;
		if((catnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"CATALOGUE","CATMST",1))!=(-1))
		{
			if((ctlgnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"CATALOGUE","CTLGMST",1))==(-1)) return;
			if((cvennum=APPReturnSupportingFileno(PURMST_MTNMASTER,"CATALOGUE","CTLGVEN",1))==(-1)) return;
			if((cvpnum=APPReturnSupportingFileno(PURMST_MTNMASTER,"CATALOGUE","CTLGVP",1))==(-1)) return;
		}
		fixmst=APPReturnSupportingFileno(PURMST_MTNMASTER,"FIXASS","FIXMST",1);
		banknum=APPReturnSupportingFileno(PURMST_MTNMASTER,"BNKREC","BNKACC",1);
		keyno=KEYNUMBER(msrnum,"PURMST KEY");
		if(keyno==(-1))
		{
			keyno=1;
		}
		tmpPassKeyPURMST=PassKeyNew();
		tmpPassKeyPURMST->fileno=msrnum;
		tmpPassKeyPURMST->keyno=keyno;
		PURMST_MTNMASTER->passkey=tmpPassKeyPURMST;
		PURMST_MTNMASTER->inheritrsrcs=TRUE;
		VENMSR_MTNMASTER=MaintainMasterNew("VENPMT","ADD PO VENMSR");
		if(findMaintainMaster(VENMSR_MTNMASTER))
		{
			warnmessage=Rmalloc(300+RDAstrlen(VENMSR_MTNMASTER->module)+RDAstrlen(mtnname_venmsr));
			sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname_venmsr,VENMSR_MTNMASTER->module);
			ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.",warnmessage,NULL,TRUE);
			prterr("%s",warnmessage);
			if(warnmessage!=NULL) Rfree(warnmessage);
			doexit(NULL);
		} else {
			keyno=KEYNUMBER(vennum,"VENDOR ID KEY");
			if(keyno==(-1))
			{
				keyno=1;
			}
			venadrkeyno=KEYNUMBER(venadrnum,"VENDOR ADDRESS KEY");
			if(venadrkeyno==(-1))
			{
				venadrkeyno=1;
			}
			tmpPassKeyVENMSR=PassKeyNew();
			tmpPassKeyVENMSR->fileno=vennum;
			tmpPassKeyVENMSR->keyno=keyno;
			VENMSR_MTNMASTER->passkey=tmpPassKeyVENMSR;
			VENMSR_MTNMASTER->inheritrsrcs=TRUE;
			PO_COUNT=0;
			purmsrm(1,NULL);
			RDAAPPMAINLOOP();
		}
	}
}
static short selectfunc(MakeBrowseList *b)
{
	char flag=FALSE;

	FINDFLDGETCHAR(msrnum,"DELETEFLAG",&flag);
	if(flag) return(FALSE);
	FINDFLDGETCHAR(msrnum,"CLOSED",&flag);
	if(flag) return(FALSE);
	return(TRUE);
}
static void doexit(MakeBrowseList *b)
{
	if(venpmt!=NULL) free_venpmt(venpmt);
	if(purord!=NULL) free_purord(purord);
	if(b->funcargs!=NULL) Rfree(b->funcargs);
	if(b->definelist!=NULL) 
	{
		killwindow(b->definelist);
		free_rsrc(b->definelist);
	}
	if(b->searchrsrc!=NULL) 
	{
		killwindow(b->searchrsrc);
		free_rsrc(b->searchrsrc);
	}
	if(b->mainrsrc!=NULL)
	{
		free_rsrc(b->mainrsrc);
	}
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	HoldIt *h;
	char *temp=NULL,*poid1=NULL;

	if(b->funcargs!=NULL)
	{
		h=(HoldIt *)b->funcargs;
		mtnrsrc=h->mtnrsrc;
		previous=h->previous;
		Rfree(b->funcargs);
	}
	if(b->mainrsrc!=NULL) free_rsrc(b->mainrsrc);
	if(b->definelist!=NULL) 
	{
		killwindow(b->definelist);
		free_rsrc(b->definelist);
	}
	if(b->searchrsrc!=NULL) 
	{
		killwindow(b->searchrsrc);
		free_rsrc(b->searchrsrc);
	}	
	get_POIDS(mtnrsrc,previous);
	readwidget(mtnrsrc,"[PURMST][FISCAL YEAR]");
	readwidget(mtnrsrc,"[PURMST][PO IDENTIFICATION]");
	FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&yearid);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&poid1);
	ZERNRD(msrnum);
	if(ADVEQLNRDKEYsec(PURMST_MTNMASTER->passkey->fileno,1,targetkey,SCRNvirtualSubData,mtnrsrc))
	{
		FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",poid1);
		FINDFLDSETINT(PURMST_MTNMASTER->passkey->fileno,"FISCAL YEAR",yearid);
		ClearRDATData(previous);
		ReadRDAScrolledLists(mtnrsrc);
		if(ADVCheckPOID(purids,poid1,SCRNvirtualSubData,mtnrsrc))
		{
			ADVAssignPOID(purids,poid1,SCRNvirtualSubData,mtnrsrc);	
		} else {
			temp=ADVGetAssignNextPOID(purids,poid1,SCRNvirtualSubData,mtnrsrc);
			if(poid1!=NULL) Rfree(poid1);
			if(temp!=NULL)
			{
				poid1=stralloc(temp);
				Rfree(temp);
			}
			FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",poid1);
			if(poid1!=NULL) Rfree(poid1);
			setlists(mtnrsrc,previous,FALSE);
			filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
			seteditable(mtnrsrc,previous,FALSE);
			GET_SUPPORTING(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
		}
	} else {
		GetRDATData(msrnum,previous);
		setlists(mtnrsrc,previous,FALSE);
		filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
		seteditable(mtnrsrc,previous,FALSE);
		GET_SUPPORTING(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	}
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
