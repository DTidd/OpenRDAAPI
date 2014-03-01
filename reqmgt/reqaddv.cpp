/* reqaddv.c - Xpert Vendor Payment Master Program*/
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

extern char editven,*module;
extern short reqitm,vennum;
extern MakeBrowseList *mbl_venmsr;
extern void setaddresses(RDArsrc *);
extern void venmsrm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short selectfunction(MakeBrowseList *);
static short breakfunction(MakeBrowseList *);
static char **run_type;
void browse_venmsr(RDArsrc *);
extern void setaddresses(RDArsrc *);

static void makerun_type()
{
	run_type=Rmalloc(4*sizeof(char *));
	run_type[0]=stralloc("Load Vendor");
	run_type[1]=stralloc("Edit Vendors");
	run_type[2]=stralloc("Browse Vendor Years");
	run_type[3]=stralloc("Browse Vendor Address");
}
static void freerun_type()
{
	short x;

	if(run_type!=NULL)
	{
		for(x=0;x<4;++x) Rfree(run_type[x]);
		Rfree(run_type);
		run_type=NULL;
	}
}
void browse_venmsr(RDArsrc *rsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(rsrc);
	if(mbl_venmsr==NULL)
	{
		mainrsrc=RDArsrcNEW(module,"ADD REQUISITION VENDOR MASTER BROWSE");
		searchrsrc=RDArsrcNEW(module,"ADD REQUISITION VENDOR MASTER SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"ADD REQUISITION VENDOR MASTER DEFINE LIST");
		addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum); 
		addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum); 
		addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum); 
		GET_SCREEN_VIRTUALS(mainrsrc,0);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		makerun_type();
		addlstrsrc(mainrsrc,"RUN FUNCTION",0,TRUE,NULL,4,&run_type,NULL);
		mbl_venmsr=ADVSbrowse(vennum,1,mainrsrc,searchrsrc,definelist
			,NULL,selectfunction,breakfunction,NULL,
			okfunc,new_record,rsrc,TRUE,NULL,0,1,1,FALSE);
	} else {
		mbl_venmsr->funcargs=rsrc;
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
static short breakfunction(MakeBrowseList *blist)
{
	char deleteflag=FALSE,reqven=FALSE;
	char *vendorid=NULL;

	if(diagapps)
	{
		FINDFLDGETSTRING(vennum,"VENDOR IDENTIFICATION",&vendorid);
		prterr("Testing Vendor Identification=[%s] for inclusion in Browse List.",vendorid);
	}
	FINDFLDGETCHAR(vennum,"DELETEFLAG",&deleteflag);
	if(deleteflag)
	{
		if(diagapps)
		{
			prterr("Skipped Browse for Vendor Identification=[%s], Record marked as DELETEFLAG=TRUE.",vendorid);
		}
		return(FALSE);
	}
	FINDFLDGETCHAR(vennum,"CATALOGUE VENDOR",&reqven);
	if(!reqven)
	{
		if(diagapps)
		{
			prterr("Skipped Browse for Vendor Identification=[%s], Record marked as CATALOGUE VENDOR=FALSE.",vendorid);
		}
		return(FALSE);
	}
	if(diagapps)
	{
		prterr("Selected for Browse the Vendor Identification=[%s].",vendorid);
	}
	return(TRUE);
}
static void doexit(MakeBrowseList *blist)
{
	if(blist->definelist!=NULL) 
	{
		killwindow(blist->definelist);
		free_rsrc(blist->definelist);
		blist->definelist=NULL;
	}
	if(blist->searchrsrc!=NULL) 
	{
		killwindow(blist->searchrsrc);
		free_rsrc(blist->searchrsrc);
		blist->searchrsrc=NULL;
	}
	if(blist->mainrsrc!=NULL)
	{
		free_rsrc(blist->mainrsrc);
		blist->mainrsrc=NULL;
	}
	freerun_type();
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	int run_func=0;
	char *vendorid=NULL; 
	double percdisc=0.0;

	if(!FINDRSCGETINT(b->mainrsrc,"RUN FUNCTION",&run_func))
	{
		switch(run_func)
		{
			case 0:
				ZERNRD(vennum);
				if(ADVEQLNRDKEYsec(vennum,1,targetkey,SCRNvirtualSubData,b->mainrsrc)) ZERNRD(vennum);
				FINDFLDGETSTRING(vennum,"VENDOR IDENTIFICATION",&vendorid);
				filerecord2rsrc(vennum,b->funcargs);
				FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR IDENTIFICATION]",
					vendorid);
				FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",vendorid);
				if(vendorid!=NULL) Rfree(vendorid);
				FINDFLDGETDOUBLE(vennum,"DEFAULT PERCENTAGE DISCOUNT",&percdisc);
				FINDFLDSETDOUBLE(reqitm,"PERCENTAGE DISCOUNT",percdisc);
				FINDRSCSETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",percdisc);
				setaddresses(b->funcargs);
				computeallSCRNvirtuals(b->funcargs);
				updateallrsrc(b->funcargs);
				break;
			case 1:
				venmsrm(0,targetkey);
				break;
			case 2:	
				BROWSESCRN_EXECUTEPROGRAM("venyr",targetkey,"VENYR KEY",
					vennum,1,NULL);
				break;
			case 3: 
				BROWSESCRN_EXECUTEPROGRAM("venadr",targetkey,"VENADR KEY",
					vennum,1,NULL);
				break;
			default:
				prterr("Invalid Run Function [%d]",run_func);
				break;
		}
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	venmsrm(2,NULL);
}
