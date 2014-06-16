/* paybc1.c - Payroll Benefits Coordinator Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

struct mtnstructs
{
	RDATData *previous;
	MakeBrowseList *mbl_paydedm;
};
typedef struct mtnstructs mtnstruct;

MakeBrowseList *mbl_paydedm,*mbl_poslmst=NULL,*mbl_sfsnum=NULL;
MakeBrowseList *mbl_poscls=NULL,*mbl_ethnum=NULL,*mbl_statnum=NULL;
MakeBrowseList *mbl_perhlth=NULL,*mbl_perlife=NULL,*mbl_perflex=NULL;
MakeBrowseList *mbl_perdntl=NULL,*mbl_perret=NULL;
char *PERID=NULL;
extern void browse_dmg(RDArsrc *,mtnstruct *);
extern MaintainMaster *MTNMASTER;
extern short perdmg,ethnum,statnum,sfsnum,payddms,paydedm,poslmst,poscls;
extern short perhlth,perlife,perflex,perdntl,perret;
extern char editable;
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void getrecord(RDArsrc *,mtnstruct *);
short getsupportingrecords(RDArsrc *,MaintainMaster *,short,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(MakeBrowseList *);
static void CopyAddress2Mailing(RDArsrc *);
static void CopyAddress2Emergency(RDArsrc *);
static void readscreen(RDArsrc *,mtnstruct *);
extern void paydedmm(short,void *);
static void initfunc(MakeBrowseList *);
static short selectfunc(MakeBrowseList *);
static void okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static void dmgsetfiles(RDArsrc *,MaintainMaster *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);

static void dmgsetfiles(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER)
{
        readallwidgets(mtnrsrc);
        ReadRDAScrolledLists(mtnrsrc);
        rsrc2filerecord(perdmg,mtnrsrc);
        getsupportingrecords(mtnrsrc,MTNMASTER,TRUE,FALSE);
        ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);
	short x=TRUE;

	keyno=KEYNUMBER(perdmg,(member->rscrname+9));
	if(keyno==(-1)) keyno=1;
	GET_PREVIOUSNM(member->parent,mtn->previous,SCRNvirtualSubData,member->parent,perdmg,keyno,MTNMASTER->inheritrsrcs);
	FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&PERID);
        getsupportingrecords(member->parent,MTNMASTER,TRUE,FALSE);
        ADVupdateSCRNvirtuals(member->parent,SCRNvirtualSubData,member->parent);
	updatebrowse(x,mtn->mbl_paydedm,paydedm,MTNMASTER->module,(RDArsrc *)member->parent);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);
	short x=TRUE;

	keyno=KEYNUMBER(perdmg,(member->rscrname+5));
	if(keyno==(-1)) keyno=1;
	GET_NEXTNM(member->parent,mtn->previous,SCRNvirtualSubData,member->parent,perdmg,keyno,MTNMASTER->inheritrsrcs);
	FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&PERID);
        getsupportingrecords(member->parent,MTNMASTER,TRUE,FALSE);
        ADVupdateSCRNvirtuals(member->parent,SCRNvirtualSubData,member->parent);
/*
	updateSCRNvirtuals(member->parent);
*/
	updatebrowse(x,mtn->mbl_paydedm,paydedm,MTNMASTER->module,(RDArsrc *)member->parent);
}
/* BROWSE ETHNIC CODE STARTING */
static void doexit_browse_ethnum(MakeBrowseList *blist)
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
static void quit_browse_ethnum(MakeBrowseList *BL)
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
static void okfunc_browse_ethnum(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 

	ZERNRD(ethnum);
	if(ADVEQLNRDKEYsec(ethnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(ethnum,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(ethnum,"ETHNIC CODE",&posid);
	}
	filerecord2rsrc(ethnum,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][ETHNIC CODE]",posid);
	FINDFLDSETSTRING(perdmg,"ETHNIC CODE",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_ethnum(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_ethnum(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_ethnum(MakeBrowseList *BL)
{
	ZERNRD(ethnum);
}
static short break_browse_ethnum(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_ethnum(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_ethnum!=NULL)
	{
		doexit_browse_ethnum(mbl_ethnum);
		mbl_ethnum=NULL;
	}
	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT ETHNIC CODE BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT ETHNIC CODE SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT ETHNIC CODE DEFINE LIST");
	ZERNRD(ethnum);
	addDFincvir(searchrsrc,"PRSNNL","PERETH",NULL,ethnum);
	addDFincvir(definelist,"PRSNNL","PERETH",NULL,ethnum);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_ethnum=xbrowse(ethnum,1,mainrsrc,searchrsrc,definelist,init_browse_ethnum,break_browse_ethnum,select_browse_ethnum,quit_browse_ethnum,okfunc_browse_ethnum,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* BROWSE ETHNIC CODE ENDING */
/* BROWSE STATUS TITLE STARTING */
static void doexit_browse_statnum(MakeBrowseList *blist)
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
static void quit_browse_statnum(MakeBrowseList *BL)
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
static void okfunc_browse_statnum(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 

	ZERNRD(statnum);
	if(ADVEQLNRDKEYsec(statnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(statnum,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(statnum,"STATUS TITLE",&posid);
	}
	filerecord2rsrc(statnum,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][STATUS TITLE]",posid);
	FINDFLDSETSTRING(perdmg,"STATUS TITLE",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_statnum(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_statnum(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_statnum(MakeBrowseList *BL)
{
	ZERNRD(statnum);
}
static short break_browse_statnum(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_statnum(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_statnum!=NULL)
	{
		doexit_browse_statnum(mbl_statnum);
		mbl_statnum=NULL;
	}
	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT STATUS TITLE BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT STATUS TITLE SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT STATUS TITLE DEFINE LIST");
	ZERNRD(statnum);
	addDFincvir(searchrsrc,"PRSNNL","PERSTAT",NULL,statnum);
	addDFincvir(definelist,"PRSNNL","PERSTAT",NULL,statnum);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_statnum=xbrowse(statnum,1,mainrsrc,searchrsrc,definelist,init_browse_statnum,break_browse_statnum,select_browse_statnum,quit_browse_statnum,okfunc_browse_statnum,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* BROWSE STATUS TITLE ENDING */
/* BROWSE CLASSES STARTING */
static void doexit_browse_poscls(MakeBrowseList *blist)
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
static void quit_browse_poscls(MakeBrowseList *BL)
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
static void okfunc_browse_poscls(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 

	ZERNRD(poscls);
	if(ADVEQLNRDKEYsec(poscls,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(poscls,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(poscls,"CLASS IDENTIFICATION",&posid);
	}
	filerecord2rsrc(poscls,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][PERSONNEL CLASS]",posid);
	FINDFLDSETSTRING(perdmg,"PERSONNEL CLASS",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_poscls(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_poscls(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_poscls(MakeBrowseList *BL)
{
	ZERNRD(poscls);
}
static short break_browse_poscls(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_poscls(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_poscls!=NULL)
	{
		doexit_browse_poscls(mbl_poscls);
		mbl_poscls=NULL;
	}
	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT CLASS IDENTIFICATION BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT CLASS IDENTIFICATION SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT CLASS IDENTIFICATION DEFINE LIST");
	ZERNRD(poscls);
	addDFincvir(searchrsrc,"POSTRK","POSCLS",NULL,poscls);
	addDFincvir(definelist,"POSTRK","POSCLS",NULL,poscls);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_poscls=xbrowse(poscls,1,mainrsrc,searchrsrc,definelist,init_browse_poscls,break_browse_poscls,select_browse_poscls,quit_browse_poscls,okfunc_browse_poscls,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* BROWSE CLASSES ENDING */
/* BROWSE PAY LOCATIONS STARTING */
static void doexit_browse_poslmst(MakeBrowseList *blist)
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
static void quit_browse_poslmst(MakeBrowseList *BL)
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
static void okfunc_browse_poslmst(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 

	ZERNRD(poslmst);
	if(ADVEQLNRDKEYsec(poslmst,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(poslmst,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(poslmst,"LOCATION IDENTIFICATION",&posid);
	}
	filerecord2rsrc(poslmst,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][LOCATION IDENTIFICATION]",posid);
	FINDFLDSETSTRING(perdmg,"LOCATION IDENTIFICATION",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_poslmst(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_poslmst(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_poslmst(MakeBrowseList *BL)
{
	ZERNRD(poslmst);
}
static short break_browse_poslmst(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_poslmst(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_poslmst!=NULL)
	{
		doexit_browse_poslmst(mbl_poslmst);
		mbl_poslmst=NULL;
	}
	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION DEFINE LIST");
	ZERNRD(poslmst);
	addDFincvir(searchrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(definelist,"POSTRK","POSLMST",NULL,poslmst);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_poslmst=xbrowse(poslmst,1,mainrsrc,searchrsrc,definelist,init_browse_poslmst,break_browse_poslmst,select_browse_poslmst,quit_browse_poslmst,okfunc_browse_poslmst,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* BROWSE PAY LOCATIONS ENDING */
/* BROWSE STATE FILING STATUS  STARTING */
static void doexit_browse_sfsnum(MakeBrowseList *blist)
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
static void quit_browse_sfsnum(MakeBrowseList *BL)
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
static void okfunc_browse_sfsnum(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 

	ZERNRD(sfsnum);
	if(ADVEQLNRDKEYsec(sfsnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(sfsnum,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(sfsnum,"STATE FILING STATUS",&posid);
	}
	filerecord2rsrc(sfsnum,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][STATE FILING STATUS]",posid);
	FINDFLDSETSTRING(perdmg,"STATE FILING STATUS",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_sfsnum(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_sfsnum(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_sfsnum(MakeBrowseList *BL)
{
	ZERNRD(sfsnum);
}
static short break_browse_sfsnum(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_sfsnum(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_poslmst!=NULL)
	{
		doexit_browse_sfsnum(mbl_sfsnum);
		mbl_sfsnum=NULL;
	}
	mainrsrc=RDArsrcNEW("PRSNNL","SELECT STATE FILING STATUS BROWSE");
	searchrsrc=RDArsrcNEW("PRSNNL","SELECT STATE FILING STATUS SEARCH BROWSE");
	definelist=RDArsrcNEW("PRSNNL","SELECT STATE FILING STATUS DEFINE LIST");
	ZERNRD(sfsnum);
	addDFincvir(searchrsrc,"PRSNNL","PERSFS",NULL,sfsnum);
	addDFincvir(definelist,"PRSNNL","PERSFS",NULL,sfsnum);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_sfsnum=xbrowse(sfsnum,1,mainrsrc,searchrsrc,definelist,init_browse_sfsnum,break_browse_sfsnum,select_browse_sfsnum,quit_browse_sfsnum,okfunc_browse_sfsnum,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* BROWSE STATE FILING STATUS ENDING */
/* LOAD HEALTH PLAN STARTING */
static void doexit_browse_perhlth(MakeBrowseList *blist)
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
static void quit_browse_perhlth(MakeBrowseList *BL)
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
static void okfunc_browse_perhlth(MakeBrowseList *BL,void *targetkey)
{
	char *hlth_pln=NULL; 

	ZERNRD(perhlth);
	if(ADVEQLNRDKEYsec(perhlth,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(perhlth,1);
	} else {
		if(hlth_pln!=NULL) { Rfree(hlth_pln); hlth_pln=NULL; }
		FINDFLDGETSTRING(perhlth,"HEALTH PLAN",&hlth_pln);
	}
	filerecord2rsrc(perhlth,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][HEALTH PLAN]",hlth_pln);
	FINDFLDSETSTRING(perdmg,"HEALTH PLAN",hlth_pln);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_perhlth(BL);
	if(hlth_pln!=NULL) Rfree(hlth_pln);
}
static short select_browse_perhlth(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_perhlth(MakeBrowseList *BL)
{
	ZERNRD(perhlth);
}
static short break_browse_perhlth(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_perhlth(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_perhlth!=NULL)
	{
		doexit_browse_perhlth(mbl_perhlth);
		mbl_perhlth=NULL;
	}
	mainrsrc=RDArsrcNEW("PRSNNL","SELECT HEALTH PLAN BROWSE");
	searchrsrc=RDArsrcNEW("PRSNNL","HEALTH PLAN SEARCH BROWSE");
	definelist=RDArsrcNEW("PRSNNL","HEALTH PLAN DEFINE LIST");
	ZERNRD(perhlth);
	addDFincvir(searchrsrc,"PRSNNL","PERHLTH",NULL,perhlth);
	addDFincvir(definelist,"PRSNNL","PERHLTH",NULL,perhlth);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_perhlth=xbrowse(perhlth,1,mainrsrc,searchrsrc,definelist,init_browse_perhlth,break_browse_perhlth,select_browse_perhlth,quit_browse_perhlth,okfunc_browse_perhlth,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* LOAD HEALTH PLAN ENDING */
/* LOAD DENTAL PLAN STARTING */
static void doexit_browse_perdntl(MakeBrowseList *blist)
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
static void quit_browse_perdntl(MakeBrowseList *BL)
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
static void okfunc_browse_perdntl(MakeBrowseList *BL,void *targetkey)
{
	char *dntl_pln=NULL; 

	ZERNRD(perdntl);
	if(ADVEQLNRDKEYsec(perdntl,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(perdntl,1);
	} else {
		if(dntl_pln!=NULL) { Rfree(dntl_pln); dntl_pln=NULL; }
		FINDFLDGETSTRING(perdntl,"DENTAL PLAN",&dntl_pln);
	}
	filerecord2rsrc(perdntl,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][DENTAL PLAN]",dntl_pln);
	FINDFLDSETSTRING(perdmg,"DENTAL PLAN",dntl_pln);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_perdntl(BL);
	if(dntl_pln!=NULL) Rfree(dntl_pln);
}
static short select_browse_perdntl(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_perdntl(MakeBrowseList *BL)
{
	ZERNRD(perdntl);
}
static short break_browse_perdntl(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_perdntl(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_perdntl!=NULL)
	{
		doexit_browse_perdntl(mbl_perdntl);
		mbl_perdntl=NULL;
	}
	mainrsrc=RDArsrcNEW("PRSNNL","SELECT DENTAL PLAN BROWSE");
	searchrsrc=RDArsrcNEW("PRSNNL","DENTAL PLAN SEARCH BROWSE");
	definelist=RDArsrcNEW("PRSNNL","DENTAL PLAN DEFINE LIST");
	ZERNRD(perdntl);
	addDFincvir(searchrsrc,"PRSNNL","PERDNTL",NULL,perdntl);
	addDFincvir(definelist,"PRSNNL","PERDNTL",NULL,perdntl);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_perdntl=xbrowse(perdntl,1,mainrsrc,searchrsrc,definelist,init_browse_perdntl,break_browse_perdntl,select_browse_perdntl,quit_browse_perdntl,okfunc_browse_perdntl,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* LOAD DENTAL PLAN ENDING */
/* LOAD LIFE INSURANCE PLAN STARTING */
static void doexit_browse_perlife(MakeBrowseList *blist)
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
static void quit_browse_perlife(MakeBrowseList *BL)
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
static void okfunc_browse_perlife(MakeBrowseList *BL,void *targetkey)
{
	char *lifeins_pln=NULL; 

	ZERNRD(perlife);
	if(ADVEQLNRDKEYsec(perlife,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(perlife,1);
	} else {
		if(lifeins_pln!=NULL) { Rfree(lifeins_pln); lifeins_pln=NULL; }
		FINDFLDGETSTRING(perlife,"LIFE INSURANCE PLAN",&lifeins_pln);
	}
	filerecord2rsrc(perlife,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][LIFE INSURANCE PLAN]",lifeins_pln);
	FINDFLDSETSTRING(perdmg,"LIFE INSURANCE PLAN",lifeins_pln);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_perlife(BL);
	if(lifeins_pln!=NULL) Rfree(lifeins_pln);
}
static short select_browse_perlife(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_perlife(MakeBrowseList *BL)
{
	ZERNRD(perlife);
}
static short break_browse_perlife(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_perlife(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_perlife!=NULL)
	{
		doexit_browse_perlife(mbl_perlife);
		mbl_perlife=NULL;
	}
	mainrsrc=RDArsrcNEW("PRSNNL","SELECT LIFE INSURANCE PLAN BROWSE");
	searchrsrc=RDArsrcNEW("PRSNNL","LIFE INSURANCE PLAN SEARCH BROWSE");
	definelist=RDArsrcNEW("PRSNNL","LIFE INSURANCE PLAN DEFINE LIST");
	ZERNRD(perlife);
	addDFincvir(searchrsrc,"PRSNNL","PERLIFE",NULL,perlife);
	addDFincvir(definelist,"PRSNNL","PERLIFE",NULL,perlife);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_perlife=xbrowse(perlife,1,mainrsrc,searchrsrc,definelist,init_browse_perlife,break_browse_perlife,select_browse_perlife,quit_browse_perlife,okfunc_browse_perlife,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* LOAD LIFE INSURANCE PLAN ENDING */
/* LOAD FLEXIBLE SPENDING PLAN STARTING */
static void doexit_browse_perflex(MakeBrowseList *blist)
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
static void quit_browse_perflex(MakeBrowseList *BL)
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
static void okfunc_browse_perflex(MakeBrowseList *BL,void *targetkey)
{
	char *flexspend_pln=NULL; 

	ZERNRD(perflex);
	if(ADVEQLNRDKEYsec(perflex,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(perflex,1);
	} else {
		if(flexspend_pln!=NULL) { Rfree(flexspend_pln); flexspend_pln=NULL; }
		FINDFLDGETSTRING(perflex,"FLEXIBLE SPENDING PLAN",&flexspend_pln);
	}
	filerecord2rsrc(perflex,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][FLEXIBLE SPENDING PLAN]",flexspend_pln);
	FINDFLDSETSTRING(perdmg,"FLEXIBLE SPENDING PLAN",flexspend_pln);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_perflex(BL);
	if(flexspend_pln!=NULL) Rfree(flexspend_pln);
}
static short select_browse_perflex(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_perflex(MakeBrowseList *BL)
{
	ZERNRD(perflex);
}
static short break_browse_perflex(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_perflex(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_perflex!=NULL)
	{
		doexit_browse_perflex(mbl_perflex);
		mbl_perflex=NULL;
	}
	mainrsrc=RDArsrcNEW("PRSNNL","SELECT FLEXIBLE SPENDING BROWSE");
	searchrsrc=RDArsrcNEW("PRSNNL","FLEXIBLE SPENDING SEARCH BROWSE");
	definelist=RDArsrcNEW("PRSNNL","FLEXIBLE SPENDING DEFINE LIST");
	ZERNRD(perflex);
	addDFincvir(searchrsrc,"PRSNNL","PERFLEX",NULL,perflex);
	addDFincvir(definelist,"PRSNNL","PERFLEX",NULL,perflex);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_perflex=xbrowse(perflex,1,mainrsrc,searchrsrc,definelist,init_browse_perflex,break_browse_perflex,select_browse_perflex,quit_browse_perflex,okfunc_browse_perflex,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* LOAD FLEXIBLE SPENDING PLAN ENDING */
/* LOAD RETIREMENT PLAN STARTING */
static void doexit_browse_perret(MakeBrowseList *blist)
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
static void quit_browse_perret(MakeBrowseList *BL)
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
static void okfunc_browse_perret(MakeBrowseList *BL,void *targetkey)
{
	char *retire_pln=NULL; 

	ZERNRD(perret);
	if(ADVEQLNRDKEYsec(perret,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(perret,1);
	} else {
		if(retire_pln!=NULL) { Rfree(retire_pln); retire_pln=NULL; }
		FINDFLDGETSTRING(perret,"RETIREMENT PLAN",&retire_pln);
	}
	filerecord2rsrc(perret,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PERDMG][RETIREMENT PLAN]",retire_pln);
	FINDFLDSETSTRING(perdmg,"RETIREMENT PLAN",retire_pln);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
        getsupportingrecords(BL->funcargs,MTNMASTER,TRUE,FALSE);
	quit_browse_perret(BL);
	if(retire_pln!=NULL) Rfree(retire_pln);
}
static short select_browse_perret(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_perret(MakeBrowseList *BL)
{
	ZERNRD(perret);
}
static short break_browse_perret(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_perret(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_perret!=NULL)
	{
		doexit_browse_perret(mbl_perret);
		mbl_perret=NULL;
	}
	mainrsrc=RDArsrcNEW("PRSNNL","SELECT PERRET BROWSE");
	searchrsrc=RDArsrcNEW("PRSNNL","SELECT PERRET SEARCH BROWSE");
	definelist=RDArsrcNEW("PRSNNL","SELECT PERRET DEFINE LIST");
	ZERNRD(perret);
	addDFincvir(searchrsrc,"PRSNNL","PERRET",NULL,perret);
	addDFincvir(definelist,"PRSNNL","PERRET",NULL,perret);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_perret=xbrowse(perret,1,mainrsrc,searchrsrc,definelist,init_browse_perret,break_browse_perret,select_browse_perret,quit_browse_perret,okfunc_browse_perret,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* LOAD RETIREMENT PLAN ENDING */
void mtndmg(short dowhich,void *targetkey)
{
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	char *name=NULL,*filename=NULL;
	short nofields=0,x,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL;
	short nokeys=0;

	mtnrsrc=RDArsrcNEW(MTNMASTER->module,"BENEFITS COORDINATOR");
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mtnrsrc,"PRSNNL","PERETH",NULL,ethnum);
	addDFincvir(mtnrsrc,"PRSNNL","PERSTAT",NULL,statnum);
	addDFincvir(mtnrsrc,"PRSNNL","PERSFS",NULL,sfsnum);
	addDFincvir(mtnrsrc,"PRSNNL","PERHLTH",NULL,perhlth);
	addDFincvir(mtnrsrc,"PRSNNL","PERDNTL",NULL,perdntl);
	addDFincvir(mtnrsrc,"PRSNNL","PERLIFE",NULL,perlife);
	addDFincvir(mtnrsrc,"PRSNNL","PERFLEX",NULL,perflex);
	addDFincvir(mtnrsrc,"PRSNNL","PERRET",NULL,perret);
	addDFincvir(mtnrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(mtnrsrc,"POSTRK","POSCLS",NULL,poscls);
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"BENEFITS COORDINATOR SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"BENEFITS COORDINATOR DEFINE LIST");
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(searchrsrc,MTNMASTER->module,"PAYDEDM",NULL,paydedm);
	addDFincvir(searchrsrc,MTNMASTER->module,"PAYDDMS",NULL,payddms);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(definelist,MTNMASTER->module,"PAYDEDM",NULL,paydedm);
	addDFincvir(definelist,MTNMASTER->module,"PAYDDMS",NULL,payddms);
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMASTER);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	ZERNRD(perdmg);
	mtn=Rmalloc(sizeof(mtnstruct));
	if(ethnum!=(-1)) file2rsrc(ethnum,mtnrsrc,FALSE);
	if(statnum!=(-1)) file2rsrc(statnum,mtnrsrc,FALSE);
	if(sfsnum!=(-1)) file2rsrc(sfsnum,mtnrsrc,FALSE);
	if(poslmst!=(-1)) file2rsrc(poslmst,mtnrsrc,FALSE);
	if(poscls!=(-1)) file2rsrc(poscls,mtnrsrc,FALSE);
	if(perhlth!=(-1)) file2rsrc(perhlth,mtnrsrc,FALSE);
	if(perlife!=(-1)) file2rsrc(perlife,mtnrsrc,FALSE);
	if(perflex!=(-1)) file2rsrc(perflex,mtnrsrc,FALSE);
	if(perdntl!=(-1)) file2rsrc(perdntl,mtnrsrc,FALSE);
	if(perret!=(-1)) file2rsrc(perret,mtnrsrc,FALSE);
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(perdmg);
	nofields=NUMFLDS(perdmg);
	fields=FLDPOINTER(perdmg);
	if(fields!=NULL)
	{
		edit_rsrc=editable;
		filename=FILENAME(perdmg);
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			nonlstmakefld(mtnrsrc,perdmg,fldx,edit_rsrc);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
			}
			sprintf(name,"[%s][%s]",filename,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,dmgsetfiles,MTNMASTER);
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[PERDMG][PERSONNEL IDENTIFICATION]",getrecord,mtn);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(perdmg,1);
				ReadRDAScrolledLists(mtnrsrc);
				if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mtnrsrc))
				{
					rsrc2filerecord(perdmg,mtnrsrc);
				} else {
					GetRDATData(perdmg,mtn->previous);
					filerecord2rsrc(perdmg,mtnrsrc);
				}
			} else {
				GetRDATData(perdmg,mtn->previous);
				filerecord2rsrc(perdmg,mtnrsrc);
			}
			break;
		case 2:
			ReadRDAScrolledLists(mtnrsrc);
			if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mtnrsrc))
			{
				rsrc2filerecord(perdmg,mtnrsrc);
			} else {
				GetRDATData(perdmg,mtn->previous);
				filerecord2rsrc(perdmg,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(perdmg,1,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(perdmg);
				ReadRDAScrolledLists(mtnrsrc);
				if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mtnrsrc))
				{
					rsrc2filerecord(perdmg,mtnrsrc);
				} else {
					GetRDATData(perdmg,mtn->previous);
					filerecord2rsrc(perdmg,mtnrsrc);
				}
			} else {
				GetRDATData(perdmg,mtn->previous);
				filerecord2rsrc(perdmg,mtnrsrc);
			}
			break;
	}
	FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&PERID);
	getsupportingrecords(mtnrsrc,MTNMASTER,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	}
	addbtnrsrc(mtnrsrc,"LOAD PERDMG",TRUE,browse_dmg,mtn);
	addbtnrsrc(mtnrsrc,"BROWSE LOCATIONS",TRUE,browse_poslmst,mtn);
	addbtnrsrc(mtnrsrc,"BROWSE CLASSES",TRUE,browse_poscls,mtn);
	addbtnrsrc(mtnrsrc,"LOAD PERSFS",TRUE,browse_sfsnum,mtn);
	addbtnrsrc(mtnrsrc,"BROWSE STATUS TITLE",TRUE,browse_statnum,mtn);
	addbtnrsrc(mtnrsrc,"BROWSE ETHNIC CODE",TRUE,browse_ethnum,mtn);
	addbtnrsrc(mtnrsrc,"LOAD PERHLTH",TRUE,browse_perhlth,mtn);
	addbtnrsrc(mtnrsrc,"LOAD PERLIFE",TRUE,browse_perlife,mtn);
	addbtnrsrc(mtnrsrc,"LOAD PERFLEX",TRUE,browse_perflex,mtn);
	addbtnrsrc(mtnrsrc,"LOAD PERDNTL",TRUE,browse_perdntl,mtn);
	addbtnrsrc(mtnrsrc,"LOAD PERRET",TRUE,browse_perret,mtn);
	addbtnrsrc(mtnrsrc,"COPY TO MAILING ADDRESS",TRUE,CopyAddress2Mailing,NULL);
	addbtnrsrc(mtnrsrc,"COPY TO EMERGENCY ADDRESS",TRUE,CopyAddress2Emergency,NULL);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	computeallSCRNvirtuals(mtnrsrc);
	keys=KEYPOINTER(perdmg);
	nokeys=NUMKEYS(perdmg);
	ADVMakePrevNextButtons(mtnrsrc,keys,nokeys,getpreviouscb,mtn,getnextcb,mtn);
	mtn->mbl_paydedm=ADVbrowse(paydedm,1,mtnrsrc,searchrsrc,definelist,
		initfunc,selectfunc,quit_record_test,okfunc,new_record,
		mtn,TRUE,NULL,0,0,0,0);
}
static void CopyAddress2Mailing(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][ADDRESS #1]","[PERDMG][MAILING ADDRESS #1]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][ADDRESS #2]","[PERDMG][MAILING ADDRESS #2]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][CITY]","[PERDMG][MAILING CITY]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][STATE]","[PERDMG][MAILING STATE]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][ZIP CODE]","[PERDMG][MAILING ZIP CODE]");
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void CopyAddress2Emergency(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][ADDRESS #1]","[PERDMG][EMERGENCY ADDRESS #1]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][ADDRESS #2]","[PERDMG][EMERGENCY ADDRESS #2]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][CITY]","[PERDMG][EMERGENCY CITY]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][STATE]","[PERDMG][EMERGENCY STATE]");
	COPYRSRC2RSRC(mtnrsrc,"[PERDMG][ZIP CODE]","[PERDMG][EMERGENCY ZIP CODE]");
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short x=TRUE;

	ZERNRD(perdmg);
	DefaultResetScreen(mtnrsrc,TRUE);
	ADV4SET_KEYFUNC(mtnrsrc,MTNMASTER,mtn->previous,SCRNvirtualSubData,(void *)mtnrsrc,NULL,NULL,NULL,NULL,AUEQLfunction,mtn);
	FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&PERID);
        getsupportingrecords(mtnrsrc,MTNMASTER,TRUE,FALSE);
        ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc);
	updatebrowse(x,mtn->mbl_paydedm,paydedm,MTNMASTER->module,mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{

	ZERNRD(perdmg);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(perdmg,mtnrsrc);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
short getsupportingrecords(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,
	short update_rsrcs,short display_diags)
{
	char delflag=TRUE,return_value=TRUE;

	COPYFIELD(perdmg,ethnum,"ETHNIC CODE");
	if(ADVEQLNRDsec(ethnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(ethnum,1);
		if(update_rsrcs) updatefilerecord2rsrc(ethnum,mtnrsrc);
		else filerecord2rsrc(ethnum,mtnrsrc);
		if(display_diags)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Ethnic Code","Record cannot be saved until a valid ethnic code is selected.",NULL,NULL,TRUE,NULL);
			
			return_value=FALSE;
		}
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(ethnum,mtnrsrc);
		else filerecord2rsrc(ethnum,mtnrsrc);
		if(display_diags)
		{
			FINDFLDGETCHAR(ethnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Ethnic Code","Record cannot be saved until a valid ethnic code is selected.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
		}

	}
	FIELDCOPY(perdmg,"PERSONNEL CLASS",poscls,"CLASS IDENTIFICATION");
	if(ADVEQLNRDsec(poscls,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(poscls,1);
		if(update_rsrcs) updatefilerecord2rsrc(poscls,mtnrsrc);
		else filerecord2rsrc(poscls,mtnrsrc);
		if(display_diags)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Personnel Class","Record cannot be saved until a valid class is selected.",NULL,NULL,TRUE,NULL);
			return_value=FALSE;
		}
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(poscls,mtnrsrc);
		else filerecord2rsrc(poscls,mtnrsrc);
		if(display_diags)
		{
			FINDFLDGETCHAR(poscls,"DELETEFLAG",&delflag);
			if(delflag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Class Identification","Record cannot be saved until a valid class is selected.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
		}
	}
	FIELDCOPY(perdmg,"LOCATION IDENTIFICATION",poslmst,"LOCATION IDENTIFICATION");
	if(ADVEQLNRDsec(poslmst,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(poslmst,1);
		if(update_rsrcs) updatefilerecord2rsrc(poslmst,mtnrsrc);
		else filerecord2rsrc(poslmst,mtnrsrc);
		if(display_diags)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Location Identification","Record cannot be saved until a valid location is selected.",NULL,NULL,TRUE,NULL);
			return_value=FALSE;
		}
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(poslmst,mtnrsrc);
		else filerecord2rsrc(poslmst,mtnrsrc);
		if(display_diags)
		{
			FINDFLDGETCHAR(poslmst,"DELETEFLAG",&delflag);
			if(delflag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Location Identification","Record cannot be saved until a valid location is selected.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
		}
	}
	COPYFIELD(perdmg,statnum,"STATUS TITLE");
	if(ADVEQLNRDsec(statnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(statnum,1);
		if(update_rsrcs) updatefilerecord2rsrc(statnum,mtnrsrc);
		else filerecord2rsrc(statnum,mtnrsrc);
		if(display_diags)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Status Title","Record cannot be saved until a valid status title is selected.",NULL,NULL,TRUE,NULL);
			return_value=FALSE;
		}
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(statnum,mtnrsrc);
		else filerecord2rsrc(statnum,mtnrsrc);
		if(display_diags)
		{
			FINDFLDGETCHAR(statnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid Status Title","Record cannot be saved until a valid status title is selected.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
		}
	}
	COPYFIELD(perdmg,sfsnum,"STATE FILING STATUS");
	if(ADVEQLNRDsec(sfsnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(sfsnum,1);
		if(update_rsrcs) updatefilerecord2rsrc(sfsnum,mtnrsrc);
		else filerecord2rsrc(sfsnum,mtnrsrc);
		if(display_diags)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid State Filing Status","Record cannot be saved until a valid state filing status is selected.",NULL,NULL,TRUE,NULL);
			return_value=FALSE;
		}
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(sfsnum,mtnrsrc);
		else filerecord2rsrc(sfsnum,mtnrsrc);
		if(display_diags)
		{
			FINDFLDGETCHAR(sfsnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","Invalid State Filing Status","Record cannot be saved until a valid state filing status is selected.",NULL,NULL,TRUE,NULL);
				return_value=FALSE;
			}
		}
	}
	COPYFIELD(perdmg,perhlth,"HEALTH PLAN");
	if(ADVEQLNRDsec(perhlth,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(perhlth,1);
		if(update_rsrcs) updatefilerecord2rsrc(perhlth,mtnrsrc);
		else filerecord2rsrc(perhlth,mtnrsrc);
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(perhlth,mtnrsrc);
		else filerecord2rsrc(perhlth,mtnrsrc);
	}
	COPYFIELD(perdmg,perdntl,"DENTAL PLAN");
	if(ADVEQLNRDsec(perdntl,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(perdntl,1);
		if(update_rsrcs) updatefilerecord2rsrc(perdntl,mtnrsrc);
		else filerecord2rsrc(perdntl,mtnrsrc);
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(perdntl,mtnrsrc);
		else filerecord2rsrc(perdntl,mtnrsrc);
	}
	COPYFIELD(perdmg,perlife,"LIFE INSURANCE PLAN");
	if(ADVEQLNRDsec(perlife,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(perlife,1);
		if(update_rsrcs) updatefilerecord2rsrc(perlife,mtnrsrc);
		else filerecord2rsrc(perlife,mtnrsrc);
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(perlife,mtnrsrc);
		else filerecord2rsrc(perlife,mtnrsrc);
	}
	COPYFIELD(perdmg,perflex,"FLEXIBLE SPENDING PLAN");
	if(ADVEQLNRDsec(perflex,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(perflex,1);
		if(update_rsrcs) updatefilerecord2rsrc(perflex,mtnrsrc);
		else filerecord2rsrc(perflex,mtnrsrc);
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(perflex,mtnrsrc);
		else filerecord2rsrc(perflex,mtnrsrc);
	}
	COPYFIELD(perdmg,perret,"RETIREMENT PLAN");
	if(ADVEQLNRDsec(perret,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(perret,1);
		if(update_rsrcs) updatefilerecord2rsrc(perret,mtnrsrc);
		else filerecord2rsrc(perret,mtnrsrc);
	} else {
		if(update_rsrcs) updatefilerecord2rsrc(perret,mtnrsrc);
		else filerecord2rsrc(perret,mtnrsrc);
	}
	return(return_value);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	readscreen(mtnrsrc,mtn);
	if(!getsupportingrecords(mtnrsrc,MTNMASTER,TRUE,TRUE)) return;
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL;
	short x=TRUE;

	if(ADVRECORDsec(perdmg,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(perdmg,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(perdmg,0,NULL,mtn->previous);
		FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&PERID);
		updatebrowse(x,mtn->mbl_paydedm,paydedm,MTNMASTER->module,mtnrsrc);
		GetRDATData(perdmg,mtn->previous);
  	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(PERID!=NULL) Rfree(PERID);
	if(mtn!=NULL)
	{
		if(mtn->mbl_paydedm!=NULL) 
		{
			if(mtn->mbl_paydedm->searchrsrc!=NULL)
			{
				killwindow(mtn->mbl_paydedm->searchrsrc);
				free_rsrc(mtn->mbl_paydedm->searchrsrc);
			}
			if(mtn->mbl_paydedm->definelist!=NULL)
			{
				killwindow(mtn->mbl_paydedm->definelist);
				free_rsrc(mtn->mbl_paydedm->definelist);
			}
			FREE_BROWSE_EXIT(mtn->mbl_paydedm);
		}
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) 
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	CLOSE_MASTER_FILES(MTNMASTER);
	if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
	if(ethnum!=(-1)) CLSNRD(ethnum);
	if(statnum!=(-1)) CLSNRD(statnum);
	if(sfsnum!=(-1)) CLSNRD(sfsnum);
	if(perlife!=(-1)) CLSNRD(perlife);
	if(perflex!=(-1)) CLSNRD(perflex);
	if(perdntl!=(-1)) CLSNRD(perdntl);
	if(perhlth!=(-1)) CLSNRD(perhlth);
	if(perret!=(-1)) CLSNRD(perret);
	ShutdownSubsystems();
}
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short x=TRUE;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[PERDMG][PERSONNEL IDENTIFICATION]",&PERID)) return;
	ZERNRD(perdmg);
	FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",PERID);
	if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(perdmg,1);
		updatefilerecord2rsrc(perdmg,mtnrsrc);
		ClearRDATData(mtn->previous);
       		getsupportingrecords(mtnrsrc,MTNMASTER,TRUE,FALSE);
	} else {
		GetRDATData(perdmg,mtn->previous);
		updatefilerecord2rsrc(perdmg,mtnrsrc);
       		getsupportingrecords(mtnrsrc,MTNMASTER,TRUE,TRUE);
	}
	FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&PERID);
	updateSCRNvirtuals(mtnrsrc);
	updatebrowse(x,mtn->mbl_paydedm,paydedm,MTNMASTER->module,mtnrsrc);
}
void quit_record_test(MakeBrowseList *blist)
{
	char *desc=NULL;
	mtnstruct *mtn=NULL;

	mtn=blist->funcargs;
	readscreen(blist->mainrsrc,mtn);
	if(QUITvfy(perdmg,mtn->previous))
	{
		desc=QUITvfydesc(perdmg,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_up,quit_record,
			FALSE,2,blist->mainrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(blist->mainrsrc,mtn);
	}
}
static void initfunc(MakeBrowseList *blist)
{
	ZERNRD(paydedm);
	FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",PERID);
}
static short selectfunc(MakeBrowseList *blist)
{
	char *perid=NULL;
	short x=TRUE;

	FINDFLDGETSTRING(paydedm,"PERSONNEL IDENTIFICATION",&perid);
	if(RDAstrcmp(perid,PERID)) x=FALSE;
	if(perid!=NULL) Rfree(perid);
	return(x);
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	mtnstruct *mtn;

	mtn=(mtnstruct *)blist->funcargs;
	mbl_paydedm=mtn->mbl_paydedm;
	mbl_paydedm=mbl_paydedm; /* gets rid of not accessed Error code 1 */
	paydedmm(0,targetkey);
}
static void new_record(RDArsrc *m,MakeBrowseList *blist)
{
	mtnstruct *mtn;

	mtn=(mtnstruct *)blist->funcargs;
	mbl_paydedm=mtn->mbl_paydedm;
	mbl_paydedm=mbl_paydedm; /* gets rid of not accessed Error code 1 */
	paydedmm(2,NULL);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,
	void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(perdmg,mtnrsrc);
}
