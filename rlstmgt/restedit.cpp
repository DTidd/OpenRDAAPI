/* restedit.c - Xpert Real Estate Editor Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restedit.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restedit.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <fin.h>
#include <rlstmgt.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs 
{
	APPlib *owntypes;
	RDATData *previous;
	RDATData *prev_rstmst;
	RDATData *prev_taxclt;
	APPlib *books;
	APPlib *class;
	APPlib *districts;
	APPlib *lands;
};
typedef struct mtnstructs mtnstruct;

static void pptownm(void);
short ownfile=(-1),filenum=0,rstmst=0,taxmst=0;
short distnum=0,classnum=0,landnum=0,deednum=0;
char editable=TRUE,edit_rstmst=TRUE,edit_taxclt=TRUE;
char *module="RLSTMGT";
static void save_record(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *);
static void save_function(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
static void setlists(RDArsrc *,mtnstruct *),dobooklist(RDArsrc *,mtnstruct *);
static void doclasslist(RDArsrc *,mtnstruct *);
static void dodistrictlist(RDArsrc *,mtnstruct *);
static void dolandlist(RDArsrc *,mtnstruct *);
static void changeclass(RDArsrc *,mtnstruct *);
static void changelanduse(RDArsrc *,mtnstruct *);
static void changebooks(RDArsrc *,mtnstruct *);
static void changedistrict(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void getnextbyname(RDArsrc *,mtnstruct *);
static void getpreviousbyname(RDArsrc *,mtnstruct *);
static void getpreviousbyrealid(RDArsrc *,mtnstruct *);
static void getnextbyrealid(RDArsrc *,mtnstruct *);
static char *ScreenName=NULL;
static void getowner(RDArsrc *,mtnstruct *);
static void getproperty(RDArsrc *,mtnstruct *);
static void delete_supporting(short,char *,char *);

void main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"REAL ESTATE EDITOR (Single)")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		ScreenName=stralloc(argv[1]);
	} else ScreenName=stralloc("REAL ESTATE EDITOR (Single)");
	if((filenum=APPOPNNRDEDITABLE(module,"RESTOWN",&editable))==(-1)) return;
	if((rstmst=APPOPNNRDEDITABLE(module,"RESTMST",&edit_rstmst))==(-1)) return;
	if((taxmst=APPOPNNRDEDITABLE("TAXCLT","TAXMST",&edit_taxclt))==(-1)) 
		return;
	if((distnum=APPOPNNRD(module,"RESTDIST",TRUE,FALSE))==(-1)) return;
	if((classnum=APPOPNNRD(module,"RESTCLAS",TRUE,FALSE))==(-1)) return;
	if((landnum=APPOPNNRD(module,"RESTLNDV",TRUE,FALSE))==(-1)) return;
	if((deednum=APPOPNNRD(module,"RESTDEED",TRUE,FALSE))==(-1)) return;
	if((ownfile=APPOPNNRD(module,"RESTOWN",TRUE,TRUE))==(-1)) return;
	pptownm();
	RDAAPPMAINLOOP();
}
static void resetdefs(RDArsrc *mtnrsrc)
{
	ZERNRD(rstmst);
	ZERNRD(filenum);
	ZERNRD(taxmst);
	DefaultResetScreen(mtnrsrc,TRUE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void dobooklist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->books!=NULL)
	{ 
		freeapplib(mtn->books);
		mtn->books=NULL; 
	}
	ZERNRD(deednum);
	mtn->books=ADVMakeNRDlistAllSTRING(deednum,module,"Creating Book List",
		1,"BOOK IDENTIFICATION",SCRNvirtualSubData,mtnrsrc);
	if(mtn->books->numlibs<1)
	{
		addAPPlib(mtn->books,"No Books Defined");
	}
}
static void doclasslist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->class!=NULL)
	{ 
		freeapplib(mtn->class);
		mtn->class=NULL; 
	}
	ZERNRD(classnum);
	mtn->class=ADVMakeNRDlistAllSTRING(classnum,module,"Creating Class List",
		1,"CLASS IDENTIFICATION",SCRNvirtualSubData,mtnrsrc);
	if(mtn->class->numlibs<1)
	{
		addAPPlib(mtn->class,"No Class Identifications");
	}
}
static void dodistrictlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->districts!=NULL)
	{ 
		freeapplib(mtn->districts);
		mtn->districts=NULL; 
	}
	ZERNRD(distnum);
	mtn->districts=ADVMakeNRDlistAllSTRING(distnum,module,
		"Creating District List",1,"DISTRICT IDENTIFICATION",
		SCRNvirtualSubData,mtnrsrc);
	if(mtn->districts->numlibs<1)
	{
		addAPPlib(mtn->districts,"No Districts Defined");
	}
}
static void dolandlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->lands!=NULL)
	{ 
		freeapplib(mtn->lands);
		mtn->lands=NULL; 
	}
	ZERNRD(landnum);
	mtn->lands=ADVMakeNRDlistAllSTRING(landnum,module,
		"Creating Use Code List",1,"LAND USE VALUE CODE",
		SCRNvirtualSubData,mtnrsrc);
	if(mtn->lands->numlibs<1)
	{
		addAPPlib(mtn->lands,"No Use Codes Defined");
	}
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *temp=NULL;
	int x=0;

	if(deednum!=(-1))
	{
		FINDFLDGETSTRING(rstmst,"BOOK IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(deednum,"BOOK IDENTIFICATION",mtn->books,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"BOOK IDENTIFICATIONS",x);
		changebooks(mtnrsrc,mtn);
	}
	if(classnum!=(-1))
	{
		FINDFLDGETSTRING(rstmst,"CLASS IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(classnum,"CLASS IDENTIFICATION",mtn->class,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"CLASS IDENTIFICATIONS",x);
		changeclass(mtnrsrc,mtn);
	}
	if(distnum!=(-1))
	{
		FINDFLDGETSTRING(rstmst,"DISTRICT IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(distnum,"DISTRICT IDENTIFICATION",
			mtn->districts,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"DISTRICT IDENTIFICATIONS",x);
		changedistrict(mtnrsrc,mtn);
	}
	if(landnum!=(-1))
	{
		FINDFLDGETSTRING(rstmst,"LAND USE VALUE CODE",&temp);
		x=FINDFLDLISTELEMENT(landnum,"LAND USE VALUE CODE",mtn->lands,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"LAND USE VALUE CODES",x);
		changelanduse(mtnrsrc,mtn);
	}
	if(temp!=NULL) Rfree(temp);
}
static void changeclass(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(classnum);
	FINDRSCGETINT(mtnrsrc,"CLASS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->class->libs[selected],"No Class Identifications"))
	{
		FINDFLDSETSTRING(classnum,"CLASS IDENTIFICATION",
			mtn->class->libs[selected]);
		if(ADVEQLNRDsec(classnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(classnum,1);
		}
	}
	updatefilerecord2rsrc(classnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void changedistrict(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(distnum);
	FINDRSCGETINT(mtnrsrc,"DISTRICT IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->districts->libs[selected],"No Districts Defined"))
	{
		FINDFLDSETSTRING(distnum,"DISTRICT IDENTIFICATION",
			mtn->districts->libs[selected]);
		if(ADVEQLNRDsec(distnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(distnum,1);
		}
	}
	updatefilerecord2rsrc(distnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void changebooks(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(deednum);
	FINDRSCGETINT(mtnrsrc,"BOOK IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->books->libs[selected],"No Books Defined"))
	{
		FINDFLDSETSTRING(deednum,"BOOK IDENTIFICATION",
			mtn->books->libs[selected]);
		if(ADVEQLNRDsec(deednum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(deednum,1);
		}
	}
	updatefilerecord2rsrc(deednum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void changelanduse(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(landnum);
	FINDRSCGETINT(mtnrsrc,"LAND USE VALUE CODES",&selected);
	if(RDAstrcmp(mtn->lands->libs[selected],"No Use Codes Defined"))
	{
		FINDFLDSETSTRING(landnum,"LAND USE VALUE CODE",
			mtn->lands->libs[selected]);
		if(ADVEQLNRDsec(landnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(landnum,1);
		}
	}
	updatefilerecord2rsrc(landnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readscreen(mtnrsrc,mtn);
	if(QUITvfy(filenum,mtn->previous))
	{
		desc=QUITvfydesc(filenum,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_check,quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else if(QUITvfy(rstmst,mtn->prev_rstmst)) 
	{
		desc=QUITvfydesc(rstmst,mtn->prev_rstmst);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_check,quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else if(QUITvfy(taxmst,mtn->prev_taxclt))
	{
		desc=QUITvfydesc(taxmst,mtn->prev_taxclt);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_check,quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
void pptownm()
{
	RDArsrc *mtnrsrc=NULL;
	int nofields=0,x,which_type=0;
	NRDfield *testfield=NULL,*testfield1=NULL,*fields=NULL,*fldx;
	short edit_rsrc=FALSE;
	mtnstruct *mtn=NULL;
	
	mtnrsrc=RDArsrcNEW(module,ScreenName);
	addDFincvir(mtnrsrc,module,"RESTOWN",NULL,filenum);
	addDFincvir(mtnrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(mtnrsrc,"TAXCLT","TAXMST",NULL,taxmst);
	addDFincvir(mtnrsrc,module,"RESTLNDV",NULL,landnum);
	addDFincvir(mtnrsrc,module,"RESTDIST",NULL,distnum);
	addDFincvir(mtnrsrc,module,"RESTCLAS",NULL,classnum);
	addDFincvir(mtnrsrc,module,"RESTDEED",NULL,deednum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->owntypes=APPlibNEW();
	mtn->books=NULL;
	mtn->class=NULL;
	mtn->districts=NULL;
	mtn->lands=NULL;
	addAPPlib(mtn->owntypes,"Percentage");
	addAPPlib(mtn->owntypes,"Flat Amount");
	ZERNRD(filenum);
	ZERNRD(rstmst);
	ZERNRD(taxmst);
	mtn->previous=RDATDataNEW(filenum);
	mtn->prev_rstmst=RDATDataNEW(rstmst);
	mtn->prev_taxclt=RDATDataNEW(taxmst);
	nofields=NUMFLDS(rstmst);
	fields=FLDPOINTER(rstmst);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)edit_rstmst;
			if(!RDAstrcmp(fldx->name,"BOOK IDENTIFICATION"))
			{
				if(FIELDscrn(rstmst,fldx->name,&edit_rsrc))
				{
					dobooklist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"BOOK IDENTIFICATIONS",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changebooks,mtn->books->numlibs,
						&mtn->books->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"CLASS IDENTIFICATION"))
			{
				if(FIELDscrn(rstmst,fldx->name,&edit_rsrc))
				{
					doclasslist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"CLASS IDENTIFICATIONS",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changeclass,mtn->class->numlibs,
						&mtn->class->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DISTRICT IDENTIFICATION"))
			{
				if(FIELDscrn(rstmst,fldx->name,&edit_rsrc))
				{
					dodistrictlist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"DISTRICT IDENTIFICATIONS",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changedistrict,mtn->districts->numlibs,
						&mtn->districts->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"LAND USE VALUE CODE"))
			{
				if(FIELDscrn(rstmst,fldx->name,&edit_rsrc))
				{
					dolandlist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"LAND USE VALUE CODES",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changelanduse,mtn->lands->numlibs,
						&mtn->lands->libs,mtn);
				}
			} else if(!strncmp(fldx->name,"TAX AGED BALANCE",16) ||
				!strncmp(fldx->name,"PENALTY AGED BALANCE",20) ||
				!strncmp(fldx->name,"INTEREST AGED BALANCE",21) ||
				!strncmp(fldx->name,"UNPROCESSED",11))
			{
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else {
				nonlstmakefld(mtnrsrc,rstmst,fldx,edit_rsrc);
			}
		}
	}
	nofields=0;
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			testfield=FLDNRD(rstmst,fldx->name);
			if(testfield==NULL || !RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
				edit_rsrc=(short)editable;
				if(!RDAstrcmp(fldx->name,"OWNERSHIP TYPE"))
				{
				} else if(!strncmp(fldx->name,"TAX AGED BALANCE",16) ||
					!strncmp(fldx->name,"PENALTY AGED BALANCE",20) ||
					!strncmp(fldx->name,"INTEREST AGED BALANCE",21) ||
					!strncmp(fldx->name,"UNPROCESSED",11))
				{
				} else {
					nonlstmakefld(mtnrsrc,filenum,fldx,edit_rsrc);
					if(!RDAstrcmp(fldx->name,"REAL ESTATE IDENTIFICATION"))
					{
						FINDRSCSETFUNC(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",getrecord,mtn);
					}
				}
			}
		}
	}
	nofields=0;
	nofields=NUMFLDS(taxmst);
	fields=FLDPOINTER(taxmst);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			testfield=FLDNRD(rstmst,fldx->name);
			testfield1=FLDNRD(filenum,fldx->name);
			if((testfield==NULL && testfield1==NULL) ||
				!RDAstrcmp(fldx->name,"NAME"))
			{
				edit_rsrc=(short)edit_taxclt;
				nonlstmakefld(mtnrsrc,taxmst,fldx,edit_rsrc);
			}
		}
	}
	file2rsrc(deednum,mtnrsrc,FALSE);
	file2rsrc(classnum,mtnrsrc,FALSE);
	file2rsrc(distnum,mtnrsrc,FALSE);
	file2rsrc(landnum,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",
		getproperty,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",
		getowner,mtn);
	DefaultScreens(mtnrsrc);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable || edit_rstmst || edit_taxclt)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_function,mtn);
	}
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,NULL);
	addbtnrsrc(mtnrsrc,"PREVIOUS BY NAME",TRUE,getpreviousbyname,mtn);
	addbtnrsrc(mtnrsrc,"NEXT BY NAME",TRUE,getnextbyname,mtn);
	addbtnrsrc(mtnrsrc,"PREVIOUS BY IDENTIFICATION",TRUE,
		getpreviousbyrealid,mtn);
	addbtnrsrc(mtnrsrc,"NEXT BY IDENTIFICATION",TRUE,getnextbyrealid,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,TRUE);
}
static void getpreviousbyname(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;

	if(diagapps)
	{
		prterr("DIAG getprevious by name.");
	}
	ZERNRD(filenum);
	readwidget(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
	if(ADVEQLNRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,2);
		if(ADVLTENRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,2);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	} else {
		if(ADVPRVNRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,2);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	}
	if(rstmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realestateid1);
		FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid1);
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstmst,1);
			ClearRDATData(mtn->prev_rstmst);
		} else {
			GetRDATData(rstmst,mtn->prev_rstmst);
			setlists(mtnrsrc,mtn);
			filerecord2rsrc(rstmst,mtnrsrc);	
		}
	}
	if(taxmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",ownerid);
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(taxmst,1);
			ClearRDATData(mtn->prev_taxclt);
		} else {
			GetRDATData(taxmst,mtn->prev_taxclt);
			filerecord2rsrc(taxmst,mtnrsrc);
		}
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getpreviousbyrealid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;

	if(diagapps)
	{
		prterr("DIAG getprevious by real estate id.");
	}
	ZERNRD(filenum);
	readwidget(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
	if(ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,1);
		if(ADVLTENRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,1);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	} else {
		if(ADVPRVNRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,1);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	}
	if(rstmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realestateid1);
		FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid1);
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstmst,1);
			ClearRDATData(mtn->prev_rstmst);
		} else {
			GetRDATData(rstmst,mtn->prev_rstmst);
			setlists(mtnrsrc,mtn);
			filerecord2rsrc(rstmst,mtnrsrc);	
		}
	}
	if(taxmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",ownerid);
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(taxmst,1);
			ClearRDATData(mtn->prev_taxclt);
		} else {
			GetRDATData(taxmst,mtn->prev_taxclt);
			filerecord2rsrc(taxmst,mtnrsrc);
		}
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getnextbyrealid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;

	if(diagapps)
	{
		prterr("DIAG getnext by real estate identification.");
	}
	ZERNRD(filenum);
	readwidget(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
	if(ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,1);
		if(ADVGTENRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,1);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	} else {
		if(ADVNXTNRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,1);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	}
	if(rstmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realestateid1);
		FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid1);
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstmst,1);
			ClearRDATData(mtn->prev_rstmst);
		} else {
			GetRDATData(rstmst,mtn->prev_rstmst);
			setlists(mtnrsrc,mtn);
			filerecord2rsrc(rstmst,mtnrsrc);	
		}
	}
	if(taxmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",ownerid);
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(taxmst,1);
			ClearRDATData(mtn->prev_taxclt);
		} else {
			GetRDATData(taxmst,mtn->prev_taxclt);
			filerecord2rsrc(taxmst,mtnrsrc);
		}
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getnextbyname(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;

	if(diagapps)
	{
		prterr("DIAG getnext by name.");
	}
	ZERNRD(filenum);
	readwidget(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
	if(ADVEQLNRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,2);
		if(ADVGTENRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,2);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	} else {
		if(ADVNXTNRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(filenum,2);
			ClearRDATData(mtn->previous);
		} else {
			GetRDATData(filenum,mtn->previous);
			filerecord2rsrc(filenum,mtnrsrc);	
		}
	}
	if(rstmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realestateid1);
		FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid1);
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstmst,1);
			ClearRDATData(mtn->prev_rstmst);
		} else {
			GetRDATData(rstmst,mtn->prev_rstmst);
			setlists(mtnrsrc,mtn);
			filerecord2rsrc(rstmst,mtnrsrc);	
		}
	}
	if(taxmst!=(-1))
	{
		FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",ownerid);
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(taxmst,1);
			ClearRDATData(mtn->prev_taxclt);
		} else {
			GetRDATData(taxmst,mtn->prev_taxclt);
			filerecord2rsrc(taxmst,mtnrsrc);
		}
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short nofields=0,x=0;
	NRDfield *testfield=NULL,*fields=NULL,*fldx=NULL;
	int owntype=0;
	double ownamt=100;

	ZERNRD(filenum);
	ZERNRD(rstmst);
	ZERNRD(taxmst);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(rstmst,mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	rsrc2filerecord(taxmst,mtnrsrc);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!strncmp(fldx->name,"TAX AGED BALANCE",16) ||
				!strncmp(fldx->name,"PENALTY AGED BALANCE",20) ||
				!strncmp(fldx->name,"INTEREST AGED BALANCE",21) ||
				!strncmp(fldx->name,"UNPROCESSED",11) ||
				!RDAstrcmp(fldx->name,"NAME"))
			{
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
				COPYFIELD(filenum,rstmst,fldx->name);
			} else {
				testfield=FLDNRD(rstmst,fldx->name);
				if(testfield!=NULL)
				{
					COPYFIELD(rstmst,filenum,fldx->name);
				}
			}
		}
	}
	nofields=NUMFLDS(taxmst);
	fields=FLDPOINTER(taxmst);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(RDAstrcmp(fldx->name,"DELETEFLAG") &&
				RDAstrcmp(fldx->name,"NAME"))
			{
				testfield=FLDNRD(rstmst,fldx->name);
				if(testfield!=NULL)
				{
					COPYFIELD(rstmst,taxmst,fldx->name);
				} else {
					testfield=FLDNRD(filenum,fldx->name);
					if(testfield!=NULL)
					{
						COPYFIELD(filenum,taxmst,fldx->name);
					}
				}
			}
		}
	}
	list2file(mtnrsrc,mtn);
	FINDFLDSETINT(filenum,"OWNERSHIP TYPE",owntype);
	FINDFLDSETDOUBLE(filenum,"OWNED AMOUNT",ownamt);
}
static void save_function(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,mtn);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *ownerid=NULL,*temp=NULL,*warnmessage=NULL,delflag=FALSE;
	char *realestateid=NULL,*temp1=NULL;
	double amt=0;

	FINDFLDGETCHAR(filenum,"DELETEFLAG",&delflag);
	FINDRSCGETDOUBLE(mtnrsrc,"REAL ESTATE OWNER TOTAL AMOUNT",&amt);
	FINDRDATDataGETSTRING(mtn->previous,"OWNER IDENTIFICATION",&temp);
	FINDRDATDataGETSTRING(mtn->previous,"REAL ESTATE IDENTIFICATION",&temp1);
	FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realestateid);
	FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
	if(amt!=0.0 && RDAstrcmp(temp,ownerid) && !RDAstrcmp(realestateid,temp))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID OWNER IDENTIFICATION!","Cannot Change Owner Identification when there is an outstanding balance.",NULL,NULL,TRUE,NULL);
		prterr("Error cannot change the owner identification when there is an outstanding balance.");
		if(ownerid!=NULL) Rfree(ownerid);
		if(temp!=NULL) Rfree(temp);
		if(temp1!=NULL) Rfree(temp1);
		if(realestateid!=NULL) Rfree(realestateid);
		return;
	}
	if(ownerid!=NULL) Rfree(ownerid);
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(realestateid!=NULL) Rfree(realestateid);
	if(!delflag)
	{
		if(deednum!=(-1))
		{
			FINDFLDGETSTRING(rstmst,"BOOK IDENTIFICATION",&temp);
			FINDFLDSETSTRING(deednum,"BOOK IDENTIFICATION",temp);
			if(ADVEQLNRDsec(deednum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(deednum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The BOOK IDENTIFICATION of [%s] entered is not valid.  It does not exist as a BOOK.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BOOK IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The BOOK IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTDEED] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(deednum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The BOOK IDENTIFICATION of [%s] entered is deleted.  It exists as a BOOK, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BOOK IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The BOOK IDENTIFICATION of [%s] entered is deleted.  It exist as a BOOK, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(classnum!=(-1))
		{
			FINDFLDGETSTRING(rstmst,"CLASS IDENTIFICATION",&temp);
			FINDFLDSETSTRING(classnum,"CLASS IDENTIFICATION",temp);
			if(ADVEQLNRDsec(classnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(classnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The CLASS IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Classification.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLASS IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The CLASS IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTCLAS] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(classnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The CLASS IDENTIFICATION of [%s] entered is deleted.  It exists as a Classification, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED CLASS IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The CLASS IDENTIFICATION of [%s] entered is deleted.  It exists as a Classification, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(distnum!=(-1))
		{
			FINDFLDGETSTRING(rstmst,"DISTRICT IDENTIFICATION",&temp);
			FINDFLDSETSTRING(distnum,"DISTRICT IDENTIFICATION",temp);
			if(ADVEQLNRDsec(distnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(distnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The DISTRICT IDENTIFICATION of [%s] entered is not valid.  It does not exist as a District.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DISTRICT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The DISTRICT IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTDIST] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(distnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The DISTRICT IDENTIFICATION of [%s] entered is deleted.  It exists as a District, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED DISTRICT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The DISTRICT IDENTIFICATION of [%s] entered is deleted.  It exist as a District, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(landnum!=(-1))
		{
			FINDFLDGETSTRING(rstmst,"LAND USE VALUE CODE",&temp);
			FINDFLDSETSTRING(landnum,"LAND USE VALUE CODE",temp);
			if(ADVEQLNRDsec(landnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(landnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The LAND USE VALUE CODE of [%s] entered is not valid.  It does not exist as a Use Code.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LAND USE VALUE CODE!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The LAND USE VALUE CODE of [%s] entered does not exist in the [RLSTMGT][RESTLNDV] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(landnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The LAND USE VALUE CODE of [%s] entered is deleted.  It exists as a Use Code, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LAND USE VALUE CODE!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The LAND USE VALUE CODE of [%s] entered is deleted.  It exist as a Use Code, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	save_record(mtnrsrc,mtn);
}
static double GetAmountOwed(short ownfile)
{
	int x=0;
	double amt1=0,amt2=0;

	memset(stemp,0,101);
	for(x=0;x<4;++x)
	{
		sprintf(stemp,"TAX AGED BALANCE #%d",x);
		amt1=0;
		FINDFLDGETDOUBLE(ownfile,stemp,&amt1);
		amt2+=amt1;
		sprintf(stemp,"PENALTY AGED BALANCE #%d",x);
		amt1=0;
		FINDFLDGETDOUBLE(ownfile,stemp,&amt1);
		amt2+=amt1;
		sprintf(stemp,"INTEREST AGED BALANCE #%d",x);
		amt1=0;
		FINDFLDGETDOUBLE(ownfile,stemp,&amt1);
		amt2+=amt1;
	}
	amt1=0;
	FINDFLDGETDOUBLE(ownfile,"UNPROCESSED TAX AMOUNT",&amt1);
	amt2+=amt1;
	amt1=0;
	FINDFLDGETDOUBLE(ownfile,"UNPROCESSED PENALTY AMOUNT",&amt1);
	amt2+=amt1;
	amt1=0;
	FINDFLDGETDOUBLE(ownfile,"UNPROCESSED INTEREST AMOUNT",&amt1);
	amt2+=amt1;
	amt1=0;
	FINDFLDGETDOUBLE(ownfile,"UNPROCESSED PAID AMOUNT",&amt1);
	amt2-=amt1;
	return(amt2);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *rcddesc=NULL,*realestateid1=NULL;
	char *realid=NULL;
	char *ownerid=NULL,*temp=NULL,*ownid1=NULL;
	int owntype=0;
	double ownamt=100;
	char deleteflag=0,vald=FALSE;
	char *tmpreal=NULL,*tmpown=NULL;
	short ef=0;
	double amount=0.0;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL;

	if(ADVRECORDsec(filenum,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(filenum,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else if(ADVRECORDsec(rstmst,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(rstmst,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else if(ADVRECORDsec(taxmst,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(taxmst,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(rstmst,0,NULL,mtn->prev_rstmst);
		WRTTRANS(taxmst,0,NULL,mtn->prev_taxclt);
		FINDRDATDataGETSTRING(mtn->previous,"REAL ESTATE IDENTIFICATION",&tmpreal);
		FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realid);
		if(ownfile!=(-1))
		{
			tmprsrc=RDArsrcNEW(module,NULL);
			addDFincvir(tmprsrc,module,"RESTOWN",NULL,ownfile);
			addDFincvir(tmprsrc,module,"RESTMST",NULL,rstmst);
			addDFincvir(tmprsrc,"TAXCLT","TAXMST",NULL,taxmst);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			FINDFLDSETSTRING(ownfile,"REAL ESTATE IDENTIFICATION",realid);
			temp=NULL;
			FINDFLDSETSTRING(ownfile,"OWNER IDENTIFICATION",temp);
			ef=ADVGTENRDsec(ownfile,1,SCRNvirtualSubData,tmprsrc);
			while(!ef)
			{
				FINDFLDGETSTRING(ownfile,"REAL ESTATE IDENTIFICATION",&tmpreal);
				if(RDAstrcmp(realid,tmpreal)) break;
				FINDFLDGETSTRING(ownfile,"OWNER IDENTIFICATION",&tmpown);
				if(RDAstrcmp(ownerid,tmpown))
				{
					FINDFLDGETCHAR(ownfile,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
						LOCNRDFILE(ownfile);
						prev=RDATDataNEW(ownfile);
						vald=FALSE;
						FINDFLDSETCHAR(ownfile,"CURRENT OWNER",vald);
						amount=GetAmountOwed(ownfile);
						if(amount!=0.0)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","CAN'T DELETE OWNER!","This owner cannot be deleted at this time.  There is an outstanding balance associated with this owner.  The owner is recorded as not being the current owner, though we must keep track of the balances for the treasurer.",NULL,NULL,TRUE,NULL);
						} else {
							vald=TRUE;
							FINDFLDSETCHAR(ownfile,"DELETEFLAG",vald);
							vald=FALSE;
						}
						ADVWRTTRANSsec(ownfile,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
						if(prev!=NULL) FreeRDATData(prev);
						delete_supporting(ownfile,realid,temp);
						UNLNRDFILE(ownfile);
					}
				}
				ef=ADVNXTNRDsec(ownfile,1,SCRNvirtualSubData,tmprsrc);
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			if(tmpown!=NULL) Rfree(tmpown);
		}
		if(temp!=NULL) Rfree(temp);
		if(ownerid!=NULL) Rfree(ownerid);
		if(realid!=NULL) Rfree(realid);
		if(tmpreal!=NULL) Rfree(tmpreal);
		FINDFLDSETINT(filenum,"OWNERSHIP TYPE",owntype);
		FINDFLDSETDOUBLE(filenum,"OWNED AMOUNT",ownamt);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			vald=FALSE;
			FINDFLDSETCHAR(filenum,"CURRENT OWNER",vald);
		} else {
/*
			vald=TRUE;
			FINDFLDSETCHAR(filenum,"CURRENT OWNER",vald);
*/
		}
		WRTTRANS(filenum,0,NULL,mtn->previous);
		if(deleteflag)
		{
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",
				&realestateid1);
			FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownid1);
			delete_supporting(filenum,realestateid1,ownid1);
		}
		if(ownid1!=NULL) Rfree(ownid1);
		if(realestateid1!=NULL) Rfree(realestateid1);
		GetRDATData(filenum,mtn->previous);
		GetRDATData(rstmst,mtn->prev_rstmst);
		GetRDATData(taxmst,mtn->prev_taxclt);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->prev_rstmst!=NULL) FreeRDATData(mtn->prev_rstmst);
		if(mtn->prev_taxclt!=NULL) FreeRDATData(mtn->prev_taxclt);
		if(mtn->owntypes!=NULL) freeapplib(mtn->owntypes);
		if(mtn->books!=NULL) freeapplib(mtn->books);
		if(mtn->class!=NULL) freeapplib(mtn->class);
		if(mtn->districts!=NULL) freeapplib(mtn->lands);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(ownfile!=(-1)) CLSNRD(ownfile);
		if(rstmst!=(-1)) CLSNRD(rstmst);
		if(taxmst!=(-1)) CLSNRD(taxmst);
		if(distnum!=(-1)) CLSNRD(distnum);
		if(classnum!=(-1)) CLSNRD(classnum);
		if(landnum!=(-1)) CLSNRD(landnum);
		if(deednum!=(-1)) CLSNRD(deednum);
		if(mtnrsrc!=NULL)
		{
			killwindow(mtnrsrc);
			free_rsrc(mtnrsrc); 
		}
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc); 
	}
}
static void getproperty(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;
	char *temp=NULL;

	if(diagapps)
	{
		prterr("DIAG getproperty.");
	}
	readwidget(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",&realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	if(rstmst!=(-1))
	{
		FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid1);
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstmst,1);
			list2file(mtnrsrc,mtn);
			ClearRDATData(mtn->prev_rstmst);
		} else {
			GetRDATData(rstmst,mtn->prev_rstmst);
			setlists(mtnrsrc,mtn);
			filerecord2rsrc(rstmst,mtnrsrc);	
			FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",
				realestateid1);
			FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
			if(!isEMPTY(ownerid))
			{
				if(ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
				{
					KEYNRD(filenum,1);
					ClearRDATData(mtn->previous);
				} else { 
					GetRDATData(filenum,mtn->previous);
					filerecord2rsrc(filenum,mtnrsrc);	
				}
			} else {
				if(ADVGTENRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
				{
					KEYNRD(filenum,1);
					ClearRDATData(mtn->previous);
				} else { 
					FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",
						&temp);
					if(!RDAstrcmp(realestateid1,temp))
					{
						GetRDATData(filenum,mtn->previous);
						filerecord2rsrc(filenum,mtnrsrc);	
					} else {
						ZERNRD(filenum);
						FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
						FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
						ClearRDATData(mtn->previous);
					}
					if(temp!=NULL) Rfree(temp);
				}
				FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
			}
		}
	}
	if(taxmst!=(-1))
	{
		FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",ownerid);
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(taxmst,1);
			ClearRDATData(mtn->prev_taxclt);
		} else {
			GetRDATData(taxmst,mtn->prev_taxclt);
			filerecord2rsrc(taxmst,mtnrsrc);
		}
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getowner(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;
	char *temp=NULL;

	if(diagapps)
	{
		prterr("DIAG getowner.");
	}
	ZERNRD(taxmst);
	ZERNRD(filenum);
	readwidget(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",&realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	if(taxmst!=(-1))
	{
		FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",ownerid);
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(taxmst,1);
			ClearRDATData(mtn->prev_taxclt);
		} else {
			GetRDATData(taxmst,mtn->prev_taxclt);
			filerecord2rsrc(taxmst,mtnrsrc);
			FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",
				realestateid1);
			FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
			if(!isEMPTY(realestateid1))
			{
				if(ADVEQLNRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
				{
					KEYNRD(filenum,2);
					ClearRDATData(mtn->previous);
				} else {
					GetRDATData(filenum,mtn->previous);
					filerecord2rsrc(filenum,mtnrsrc);	
				}
			} else {
				if(ADVGTENRDsec(filenum,2,SCRNvirtualSubData,mtnrsrc)) 
				{
					KEYNRD(filenum,2);
					ClearRDATData(mtn->previous);
				} else {
					FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&temp);		
					if(!RDAstrcmp(ownerid,temp))
					{
						GetRDATData(filenum,mtn->previous);
						filerecord2rsrc(filenum,mtnrsrc);
					} else {
						ZERNRD(filenum);
						FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
						FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
						ClearRDATData(mtn->previous);
					}
					if(temp!=NULL) Rfree(temp);
				}
			}
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realestateid1);
			FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownerid);
			if(rstmst!=(-1))
			{
				FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid1);
				if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
				{
					KEYNRD(rstmst,1);
					ClearRDATData(mtn->prev_rstmst);
				} else {
					GetRDATData(rstmst,mtn->prev_rstmst);
					setlists(mtnrsrc,mtn);
					filerecord2rsrc(rstmst,mtnrsrc);	
				}
			}
		}
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	FINDRSCGETINT(mtnrsrc,"DISTRICT IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->districts->libs[selected],"No Districts Defined"))
	{
		FINDFLDSETSTRING(rstmst,"DISTRICT IDENTIFICATION",
			mtn->districts->libs[selected]);
	}
	FINDRSCGETINT(mtnrsrc,"BOOK IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->books->libs[selected],"No Books Defined"))
	{
		FINDFLDSETSTRING(rstmst,"BOOK IDENTIFICATION",
			mtn->books->libs[selected]);
	}
	FINDRSCGETINT(mtnrsrc,"CLASS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->class->libs[selected],"No Class Identifications"))
	{
		FINDFLDSETSTRING(rstmst,"CLASS IDENTIFICATION",
			mtn->class->libs[selected]);
	}
	FINDRSCGETINT(mtnrsrc,"LAND USE VALUE CODES",&selected);
	if(RDAstrcmp(mtn->lands->libs[selected],"No Use Codes Defined"))
	{
		FINDFLDSETSTRING(rstmst,"LAND USE VALUE CODE",
			mtn->lands->libs[selected]);
	}
}
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;

	if(diagapps)
	{
		prterr("DIAG getrecord.");
	}
	readwidget(mtnrsrc,"[RESTOWN][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
	if(ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,1);
		ClearRDATData(mtn->previous);
	} else {
		GetRDATData(filenum,mtn->previous);
		filerecord2rsrc(filenum,mtnrsrc);	
	}
	FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",realestateid1);
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(rstmst,1);
		ClearRDATData(mtn->prev_rstmst);
	} else {
		GetRDATData(rstmst,mtn->prev_rstmst);
		setlists(mtnrsrc,mtn);
		filerecord2rsrc(rstmst,mtnrsrc);	
	}
	if(taxmst!=(-1))
	{
		FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",ownerid);
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(taxmst,1);
			ClearRDATData(mtn->prev_taxclt);
		} else {
			GetRDATData(taxmst,mtn->prev_taxclt);
			filerecord2rsrc(taxmst,mtnrsrc);
		}
	}
	if(ownerid!=NULL) Rfree(ownerid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void delete_supporting(short ownfile,char *realestateid1,char *ownid1)
{
	char *realestateid2=NULL;
	char *realestateid3=NULL,*realestateid4=NULL;
	char *ownid2=NULL,*ownid3=NULL,*ownid4=NULL;
	char deleteflag=0,pdeleteflag=0,vdeleteflag=0;
	char tdeleteflag=0,dist=FALSE;
	short yrnum=0,paidnum=0,ynum=0,taxdwr=0;
	short ryrnum=0,byrnum=0,trannum=0;
	short ef_yr=0,ef_paid=0,ef_tran=0;
	short deftype=0,p_month=0,type=0;
	int yearid1=0,yearid2=0,yearid3=0;
	int tran1=0,tran2=0;
	double amount=0.0;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL;

	taxdwr=OPNNRDsec("TAXCLT","TAXMST",TRUE,TRUE,FALSE);
	ynum=OPNNRDsec(module,"RESTYEAR",TRUE,TRUE,FALSE);
	yrnum=OPNNRDsec(module,"RESTOYR",TRUE,TRUE,FALSE);
	trannum=OPNNRDsec(module,"RESTRAN",TRUE,TRUE,FALSE);
	paidnum=OPNNRDsec(module,"RESTPAID",TRUE,TRUE,FALSE);
	ryrnum=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
	byrnum=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
	tmprsrc=RDArsrcNEW(module,NULL);
	addDFincvir(tmprsrc,"TAXCLT","TAXMST",NULL,taxdwr);
	addDFincvir(tmprsrc,module,"RESTOWN",NULL,ownfile);
	addDFincvir(tmprsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(tmprsrc,module,"RESTYEAR",NULL,ynum);
	addDFincvir(tmprsrc,module,"RESTOYR",NULL,yrnum);
	addDFincvir(tmprsrc,module,"RESTTRAN",NULL,trannum);
	addDFincvir(tmprsrc,module,"RESTPAID",NULL,paidnum);
	addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
	addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(yrnum!=(-1))
	{
		ZERNRD(yrnum);
		FINDFLDSETSTRING(yrnum,"REAL ESTATE IDENTIFICATION",
			realestateid1);
		FINDFLDSETSTRING(yrnum,"OWNER IDENTIFICATION",ownid1);
		ef_yr=ADVGTENRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc);
		while(!ef_yr)
		{
			FINDFLDGETSTRING(yrnum,"REAL ESTATE IDENTIFICATION",
				&realestateid2);
			FINDFLDGETSTRING(yrnum,"OWNER IDENTIFICATION",
				&ownid2);
			if(RDAstrcmp(realestateid2,realestateid1) ||
				RDAstrcmp(ownid1,ownid2)) break;
			FINDFLDGETCHAR(yrnum,"DELETEFLAG",&pdeleteflag);
			if(!pdeleteflag)
			{
				FINDFLDGETINT(yrnum,"FISCAL YEAR",&yearid1);
				prev=RDATDataNEW(yrnum);
				FINDFLDSETCHAR(yrnum,"DELETEFLAG",deleteflag);
				ADVWRTTRANSsec(yrnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
				if(prev!=NULL) FreeRDATData(prev);
				if(trannum!=(-1))
				{
					ZERNRD(trannum);
					FINDFLDSETSTRING(trannum,"REAL ESTATE IDENTIFICATION",realestateid1);
					FINDFLDSETSTRING(trannum,"OWNER IDENTIFICATION",ownid1);
					FINDFLDSETINT(trannum,"FISCAL YEAR",yearid1);
					ef_tran=ADVGTENRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
					while(!ef_tran)
					{
						FINDFLDGETSTRING(trannum,"REAL ESTATE IDENTIFICATION",&realestateid3);
						FINDFLDGETSTRING(trannum,"OWNER IDENTIFICATION",&ownid3);
						FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid2);
						if(RDAstrcmp(realestateid1,realestateid3) || RDAstrcmp(ownid1,ownid3) || yearid1!=yearid2) break;
						FINDFLDGETCHAR(trannum,"DELETEFLAG",&tdeleteflag);
						if(!tdeleteflag)
						{
							FINDFLDGETINT(trannum,"TRANSACTION NUMBER",&tran1);
							FINDFLDGETCHAR(trannum,"UPDATED",&vdeleteflag);
							if(!vdeleteflag)
							{
								FINDFLDGETDOUBLE(trannum,"AMOUNT",&amount);
								ADVUpdateRealEstateTransactions(rstmst,ynum,filenum,yrnum,trannum,-amount,SCRNvirtualSubData,tmprsrc);
							}
							prev=RDATDataNEW(trannum);
							FINDFLDSETCHAR(trannum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(trannum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
							if(prev!=NULL) FreeRDATData(prev);
							if(paidnum!=(-1) && ryrnum!=(-1) && byrnum!=(-1))
							{
								ZERNRD(paidnum);
								FINDFLDSETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",realestateid1);
								FINDFLDSETSTRING(paidnum,"OWNER IDENTIFICATION",ownid1);
								FINDFLDSETINT(paidnum,"FISCAL YEAR",yearid1);
								FINDFLDSETINT(paidnum,"TRANSACTION NUMBER",tran1);
								ef_paid=ADVGTENRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
								while(!ef_paid)
								{
									FINDFLDGETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",&realestateid4);
									FINDFLDGETSTRING(paidnum,"OWNER IDENTIFICATION",&ownid4);
									FINDFLDGETINT(paidnum,"FISCAL YEAR",&yearid3);
									FINDFLDGETINT(paidnum,"TRANSACTION NUMBER",&tran2);
									if(RDAstrcmp(realestateid4,realestateid1) || RDAstrcmp(ownid1,ownid4) || yearid1!=yearid3 || tran1!=tran2) break;
									FINDFLDGETCHAR(paidnum,"DELETEFLAG",&vdeleteflag);
									if(!vdeleteflag)
									{
										prev=RDATDataNEW(paidnum);
										FINDFLDGETCHAR(paidnum,"DISTRIBUTED",&dist);
										if(dist==FALSE)
										{
											FINDFLDGETSHORT(paidnum,"TRANSACTION TYPE",&type);
											FINDFLDGETSHORT(paidnum,"DEFINITION TYPE",&deftype);
						    				FINDFLDGETSHORT(paidnum,"PROCESSING MONTH",&p_month);
						    				FINDFLDGETDOUBLE(paidnum,"AMOUNT",&amount);
											if(!type)
											{
												ADVupdateundist(trannum,NULL,"REVENUE CODE","CASH CODE",NULL,ryrnum,byrnum,yearid1,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
											} else {
												ADVupdateundist(trannum,"ACCOUNT CODE","REVENUE CODE","CASH CODE",NULL,ryrnum,byrnum,yearid1,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
											}
										}
										ADVUpdateRealEstatePayments(taxdwr,rstmst,ynum,filenum,yrnum,trannum,paidnum,-amount,SCRNvirtualSubData,tmprsrc);
										FINDFLDSETCHAR(paidnum,"DELETEFLAG",deleteflag);
										ADVWRTTRANSsec(paidnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
										if(prev!=NULL) FreeRDATData(prev);
									}
									ef_paid=ADVNXTNRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
								}
							}
						}	
						ef_tran=ADVNXTNRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
					}
				}
			}
			ef_yr=ADVNXTNRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc);
		}
	}
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	if(taxdwr!=(-1)) CLSNRD(taxdwr);
	if(ynum!=(-1)) CLSNRD(ynum);
	if(yrnum!=(-1)) CLSNRD(yrnum);
	if(trannum!=(-1)) CLSNRD(trannum);
	if(paidnum!=(-1)) CLSNRD(paidnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(realestateid2!=NULL) Rfree(realestateid2);
	if(realestateid3!=NULL) Rfree(realestateid3);
	if(realestateid4!=NULL) Rfree(realestateid4);
	if(ownid2!=NULL) Rfree(ownid2);
	if(ownid3!=NULL) Rfree(ownid3);
	if(ownid4!=NULL) Rfree(ownid4);
}
