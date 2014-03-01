/* puravenm.c - Xpert Vendor Payment Master Program*/
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

extern char editven;
extern short msrnum,vennum,venadrnum;
extern MakeBrowseList *mbl_venmsr;
extern void setaddresses(RDArsrc *);
extern void venmsrm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short selectfunction(MakeBrowseList *);
void browse_venmsr(RDArsrc *);
extern MaintainMaster *VENMSR_MTNMASTER,*PURMST_MTNMASTER;

void browse_venmsr(RDArsrc *poaddrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(poaddrsrc);
	if(mbl_venmsr==NULL)
	{
		mainrsrc=RDArsrcNEW(VENMSR_MTNMASTER->module,VENMSR_MTNMASTER->browse_screen);
		searchrsrc=RDArsrcNEW(VENMSR_MTNMASTER->module,VENMSR_MTNMASTER->search_browse);
		definelist=RDArsrcNEW(VENMSR_MTNMASTER->module,VENMSR_MTNMASTER->define_list);
		GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,VENMSR_MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
		GET_SCREEN_VIRTUALS(mainrsrc,0);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		MakeRunFunction(mainrsrc,VENMSR_MTNMASTER);
		DefaultScreens(mainrsrc);
		mbl_venmsr=ADVbrowse(vennum,1,mainrsrc,searchrsrc,definelist
			,NULL,selectfunction,NULL,okfunc,new_record
			,poaddrsrc,TRUE,NULL,0,1,1,FALSE);
	} else {
		if(APPmakescrn(mbl_venmsr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_venmsr);
		}
	}
}
static short selectfunction(MakeBrowseList *blist)
{
	return(TRUE);
}
static void doexit(MakeBrowseList *blist)
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
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;
	char *vendorid=NULL; 

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			ZERNRD(vennum);
			if(ADVEQLNRDKEYsec(vennum,1,targetkey,SCRNvirtualSubData,blist->mainrsrc))
			{
				KEYNRD(vennum,1);
			} else {
				if(vendorid!=NULL) Rfree(vendorid); 
				FINDFLDGETSTRING(vennum,"VENDOR IDENTIFICATION",&vendorid);
			}
			filerecord2rsrc(vennum,blist->funcargs);
			FINDRSCSETSTRING(blist->funcargs,"[PURMST][VENDOR IDENTIFICATION]",vendorid);
			FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid);
			if(venadrnum!=(-1))
			{
				ZERNRD(venadrnum);
				FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
				if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,blist->mainrsrc)) KEYNRD(venadrnum,1);
			}
			computeallSCRNvirtuals(blist->funcargs);
			updateallrsrc(blist->funcargs);
			setaddresses(blist->funcargs);
		} else {
			ExecuteListFunc(run_func-1,targetkey,VENMSR_MTNMASTER);
		}
	} else {
		ZERNRD(vennum);
		if(ADVEQLNRDKEYsec(vennum,1,targetkey,SCRNvirtualSubData,blist->mainrsrc))
		{
			KEYNRD(vennum,1);
		} else {
			if(vendorid!=NULL) Rfree(vendorid); 
			FINDFLDGETSTRING(vennum,"VENDOR IDENTIFICATION",&vendorid);
		}
		filerecord2rsrc(vennum,blist->funcargs);
		FINDRSCSETSTRING(blist->funcargs,"[PURMST][VENDOR IDENTIFICATION]",vendorid);
		FINDFLDSETSTRING(msrnum,"VENDOR IDENTIFICATION",vendorid);
		if(venadrnum!=(-1))
		{
			ZERNRD(venadrnum);
			FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
			if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,blist->mainrsrc)) KEYNRD(venadrnum,1);
		}
		computeallSCRNvirtuals(blist->funcargs);
		updateallrsrc(blist->funcargs);
		setaddresses(blist->funcargs);
	}
	if(vendorid!=NULL) Rfree(vendorid);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	venmsrm(2,NULL);
}
