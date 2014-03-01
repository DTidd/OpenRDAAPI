/* paybc.c - Payroll Benefits Coordinator */
/*lint -library */
#ifndef WIN32
#define __NAM__ "paybc.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paybc.exe"
#endif
#include <app.hpp>


#include <powadd.hpp>

#include <cstdlib>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*CPP_OPART paybc1 */
/*CPP_OPART paybc2 */
/*ADDLIB add */
/*ADDLIB rpt */
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
	MakeBrowseList *mbl_paydedm;
};
typedef struct mtnstructs mtnstruct;

struct PassingStructs
{
	RDArsrc *mtnrsrc;
	mtnstruct *mtn;
};
typedef struct PassingStructs PassingStruct;

extern void mtndmg(short,void *);
extern char *PERID;
extern short getsupportingrecords(RDArsrc *,MaintainMaster *,short,short);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void CHECK_MANAGEDID(void);
char UsingManagedID=FALSE;
short perdmg=(-1),ethnum=(-1),statnum=(-1),sfsnum=(-1),paydedm=(-1);
short payddms=(-1),paydtyp=(-1),poslmst=(-1),poscls=(-1);
short perhlth=(-1),perlife=(-1),perflex=(-1),perdntl=(-1);
short perret=(-1);
MaintainMaster *MTNMASTER;
char editable=TRUE;
void browse_dmg(RDArsrc *,mtnstruct *);
static short selectpersonnel(MakeBrowseList *);
MakeBrowseList *mbl_perdmg=NULL;

static void personnel_init(MakeBrowseList *blist)
{
	PassingStruct *p=NULL;
	
	p=blist->funcargs;
	if(p!=NULL) 
	{
		ZERNRD(blist->fileno);
		if(!UsingManagedID && !isEMPTY(PERID))
		{
			FINDFLDSETSTRING(blist->fileno,"PERSONNEL IDENTIFICATION",PERID);
		}
	} 
}
static short selectpersonnel(MakeBrowseList *blist)
{
	char *empid1=NULL;
	PassingStruct *p=NULL;
	short r=TRUE;
	
	p=blist->funcargs;
	if(p!=NULL) 
	{
		if(!UsingManagedID && !isEMPTY(PERID))
		{
			FINDFLDGETSTRING(blist->fileno,"PERSONNEL IDENTIFICATION",&empid1);
			if(strncmp(empid1,PERID,RDAstrlen(PERID))) r=FALSE;
			if(empid1!=NULL) Rfree(empid1);
		}
	} 
	return(r);
}
void browse_dmg(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *searchrsrc=NULL,*definelist=NULL;
	RDArsrc *mainrsrc=NULL;
	PassingStruct *p=NULL;

	ForceWindowUpdate(mtnrsrc);
	readwidget(mtnrsrc,"[PERDMG][PERSONNEL IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[PERDMG][PERSONNEL IDENTIFICATION]",&PERID);
	if(mbl_perdmg!=NULL) PowerMasterBrowseFreeFunction(mbl_perdmg);
	p=Rmalloc(sizeof(PassingStruct));
	p->mtn=mtn;
	p->mtnrsrc=mtnrsrc;
	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT PERSONNEL BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT PERSONNEL SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT PERSONNEL DEFINE LIST");
	ZERNRD(perdmg);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(searchrsrc,"PRSNNL","PERETH",NULL,ethnum);
	addDFincvir(searchrsrc,"PRSNNL","PERSTAT",NULL,statnum);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mbl_perdmg=ADVbrowse(perdmg,1,mainrsrc,searchrsrc,definelist,personnel_init,selectpersonnel,doexit,okfunc,NULL,p,TRUE,NULL,1,1,1,0);
}

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *warnmessage=NULL,*module=NULL,*mtnname=NULL;

	module="PAYROLL";
	mtnname="MTN BENEFITS";
	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"BENEFITS COORDINATOR")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	USE_DIAGNOSTIC_SCREENS=FALSE;
	mbl_perdmg=NULL;
	MTNMASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MTNMASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MTNMASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN BENEFITS",MTNMASTER->module);
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((paydedm=APPOPNNRDEDITABLE(MTNMASTER->module,"PAYDEDM",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MTNMASTER))
                {
                        doexit(NULL);
                        return;
                }
		MTNMASTER->passkey=READPassKey(paydedm);
		if((ethnum=APPOPNNRD("PRSNNL","PERETH",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		if((statnum=APPOPNNRD("PRSNNL","PERSTAT",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		if((sfsnum=APPOPNNRD("PRSNNL","PERSFS",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		if((perhlth=APPOPNNRD("PRSNNL","PERHLTH",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		if((perlife=APPOPNNRD("PRSNNL","PERLIFE",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		if((perflex=APPOPNNRD("PRSNNL","PERFLEX",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		if((perdntl=APPOPNNRD("PRSNNL","PERDNTL",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		if((perret=APPOPNNRD("PRSNNL","PERRET",TRUE,FALSE))==(-1))
		{
                        doexit(NULL);
			return;
		}
		CHECK_MANAGEDID();
		SetSupportingWritable(MTNMASTER,"PRSNNL","PERDMG",1,TRUE,&editable);
		perdmg=APPReturnSupportingFileno(MTNMASTER,"PRSNNL","PERDMG",1);
		payddms=APPReturnSupportingFileno(MTNMASTER,MTNMASTER->module,"PAYDDMS",1);
		paydtyp=APPReturnSupportingFileno(MTNMASTER,MTNMASTER->module,"PAYDTYP",1);
		poslmst=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSLMST",1);
		poscls=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSCLS",1);
		mtndmg(1,NULL);
	}
	RDAAPPMAINLOOP();
}
static void doexit(MakeBrowseList *blist)
{
	PassingStruct *p=NULL;

	p=blist->funcargs;
	if(p!=NULL) Rfree(p);
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	PassingStruct *p=NULL;
	short x=TRUE;

	p=b->funcargs;
	if(p!=NULL) 
	{
		if(ADVEQLNRDKEYsec(perdmg,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
		{
			KEYNRD(perdmg,1);
			updatefilerecord2rsrc(perdmg,p->mtnrsrc);
			ClearRDATData(p->mtn->previous);
			getsupportingrecords(p->mtnrsrc,MTNMASTER,TRUE,FALSE);
		} else {
			filerecord2rsrc(perdmg,p->mtnrsrc);
			updatefilerecord2rsrc(perdmg,p->mtnrsrc);
			GetRDATData(perdmg,p->mtn->previous);
			getsupportingrecords(p->mtnrsrc,MTNMASTER,TRUE,TRUE);
		}
		FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&PERID);
		updateSCRNvirtuals(p->mtnrsrc);
		updatebrowse(x,p->mtn->mbl_paydedm,paydedm,MTNMASTER->module,p->mtnrsrc);
		Rfree(p);
	}
}
static void CHECK_MANAGEDID()
{
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;


		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PRSNNL")+11);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PRSNNL");
#endif
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("PRSNNL","MANAGE PERSONNEL IDENTIFICATIONS");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				UsingManagedID=*gsv->value.string_value;
				/*printf("\nUsingManagedID = %d\n",UsingManagedID);*/
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);


}
