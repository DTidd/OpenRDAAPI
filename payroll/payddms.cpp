/* payddms.c - Xpert Payroll Deduction Description Master */
/*lint -library */
#ifndef WIN32
#define __NAM__ "payddms.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payddms.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <ldval.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *addravl;
	RDArsrc *mtnrsrc;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void doexit1(MakeBrowseList *);
static void doexit2(MakeBrowseList *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short bennum=(-1),vennum=(-1),venadrnum=(-1);
static char editable=TRUE;
static MakeBrowseList *mbl_payddms=NULL;
static MaintainMaster *MASTER=NULL;
static void browse_payddms(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,short,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void BUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void payddmsm(short,void *);
static void makeaddrlist(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void setaddresses(RDArsrc *,mtnstruct *,short);
static void list2file(RDArsrc *,mtnstruct *,short);
static void changeADDRlist(RDArsrc *,mtnstruct *);
static char getbennid(RDArsrc *,short,short);
static short setvenid(RDArsrc *,mtnstruct *);
static void check_venid(RDArsrc *,mtnstruct *);
static MakeBrowseList *mbl_venmsr=NULL;
static void browse_venmsr(RDArsrc *,mtnstruct *);

static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,BUEQLfunction,mtn,
		AUEQLfunction,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,BUEQLfunction,mtn,
		AUEQLfunction,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,(void *)mtnrsrc,NEfunction,mtn,BUEQLfunction,mtn,AUEQLfunction,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	getbennid(mtnrsrc,TRUE,FALSE);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,FALSE);
	ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void addholdvalues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvalues Module [%s] File [%s] ",modulex,filex);
	}
	if(!GETBIN(modulex,filex,&engine,&nofields,&fields,
		&nokeys,&keys)) 
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
			Rfree(keys);
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+
							RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					} else 	{
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modulex,
						filex,f->name);
					addAPPlib(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
static APPlib *makefieldvallist(RDArsrc *r)
{
	APPlib *tmp=NULL,*virflist=NULL;
	char addvf=FALSE,*libx=NULL,deleteflag=FALSE,active=FALSE,*dedid=NULL;
	int x,y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short dumb=0;
	RDArsrc *tmprsrc=NULL;

	tmp=APPlibNEW();
	tmprsrc=RDArsrcNEW(MASTER->module,"TEMP");
	addDFincvir(tmprsrc,MASTER->module,"PAYJMST",NULL,dumb);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRS",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRAT",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSPER",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYJCSM",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYJFSM",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYJPRM",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYMSTR",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYJRCS",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYJRFS",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYDDMS",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYDDAC",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYDTYP",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYDEDM",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYDCSM",NULL,dumb);
	addDFincvir(tmprsrc,MASTER->module,"PAYDFSM",NULL,dumb);
	for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
	{
		addholdvalues(tmp,i->module,i->file);
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/PAYROLL.VIR",CURRENTDIRECTORY);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(MASTER->module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(tmp,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	loadglobals(tmp);
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	ZERNRD(MASTER->passkey->fileno);
	dumb=ADVFRSNRDsec(MASTER->passkey->fileno,1,SCRNvirtualSubData,r);
	while(!dumb)
	{
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(MASTER->passkey->fileno,"ACTIVE",&active);
		if(!deleteflag && active)
		{
			FINDFLDGETSTRING(MASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",
				&dedid);
			libx=Rmalloc(RDAstrlen(dedid)+16);
			sprintf(libx,"[%s][CHECK GROSS]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+11);
			sprintf(libx,"[%s][AMOUNT]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+9);
			sprintf(libx,"[%s][BASE]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+22);
			sprintf(libx,"[%s][COMPUTATION LEVEL]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+23);
			sprintf(libx,"[%s][ANNUAL AFFECT BASE]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+16);
			sprintf(libx,"[%s][AFFECT BASE]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+10);
			sprintf(libx,"[%s][RATIO]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+14);
			sprintf(libx,"[%s][CHECK NET]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			if(libx!=NULL) Rfree(libx);
			if(dedid!=NULL) Rfree(dedid);
		}
		dumb=ADVNXTNRDsec(MASTER->passkey->fileno,1,SCRNvirtualSubData,r);
	}
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load=NULL;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist(r);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void doexit1(MakeBrowseList *blist)
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
}
static void doexit2(MakeBrowseList *blist)
{
	if(blist!=NULL)
	{
		if(blist->definelist!=NULL)
		{
			killwindow(blist->definelist);
		}
		if(blist->searchrsrc!=NULL)
		{
			killwindow(blist->searchrsrc);
		}
	}
}
static void okfunc1(MakeBrowseList *b,void *targetkey)
{
	char *vendorid=NULL;
	mtnstruct *mtn=NULL;

	mtn=(mtnstruct *)b->funcargs;
	ZERNRD(vennum);
	if(ADVEQLNRDKEYsec(vennum,1,targetkey,SCRNvirtualSubData,mtn->mtnrsrc))
	{
		KEYNRD(vennum,1);
	} else {
		if(vendorid!=NULL) Rfree(vendorid);
		FINDFLDGETSTRING(vennum,"VENDOR IDENTIFICATION",&vendorid);
	}
	filerecord2rsrc(vennum,mtn->mtnrsrc);
	FINDRSCSETSTRING(mtn->mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]",vendorid);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid);
	if(venadrnum!=(-1))
	{
		ZERNRD(venadrnum);
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtn->mtnrsrc)) 
			KEYNRD(venadrnum,1);
	}
	computeallSCRNvirtuals(mtn->mtnrsrc);
	updateallrsrc(mtn->mtnrsrc);
	makeaddrlist(mtn->mtnrsrc,mtn,TRUE);
}
static short selectfunction(MakeBrowseList *blist)
{
	return(TRUE);
}
static void browse_venmsr(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(mtnrsrc);
	if(mbl_venmsr==NULL)
	{
		mainrsrc=RDArsrcNEW("PAYROLL","DEDUCTION DESCRIPTION VENDOR MASTER BROWSE");
		searchrsrc=RDArsrcNEW("PAYROLL","DEDUCTION DESCRIPTION VENDOR MASTER SEARCH BROWSE");
		definelist=RDArsrcNEW("PAYROLL","DEDUCTION DESCRIPTION VENDOR MASTER DEFINE LIST");
		addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum);
		addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_venmsr=ADVbrowse(vennum,1,mainrsrc,searchrsrc,definelist,
			NULL,selectfunction,doexit2,okfunc1,NULL,
			mtn,TRUE,NULL,0,1,1,FALSE);
	} else {
		mtn->mtnrsrc=mtnrsrc;
		mbl_venmsr->funcargs=mtn;
		if(APPmakescrn(mbl_venmsr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit1(mbl_venmsr);
		}
	}
}
static void check_venid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *venid=NULL,*venid1=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]",&venid);
	readallwidgets(mtnrsrc);
	if(setvenid(mtnrsrc,mtn))
	{
		FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]",&venid1);
		if(RDAstrcmp(venid,venid1))
		{
			makeaddrlist(mtnrsrc,mtn,TRUE);
		}
	} else {
		makeaddrlist(mtnrsrc,mtn,TRUE);
	}
	if(venid!=NULL) Rfree(venid);
	if(venid1!=NULL) Rfree(venid1);
}
static short setvenid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *venid=NULL;
	char *edesc=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]",&venid);
	FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",venid);
	if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(vennum,1);
		edesc=Rmalloc(RDAstrlen(venid)+126);
		sprintf(edesc,"The Vendor Identification [%s] is invalid.  It doesn't exist.  Please enter a correct identification or cancel this operation.",(venid==NULL?"":venid));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
		if(venid!=NULL) Rfree(venid);
		if(vennum!=(-1)) updatefilerecord2rsrc(vennum,mtnrsrc);
		updateSCRNvirtuals(mtnrsrc);
		return(FALSE);
	} else {
		if(venid!=NULL) Rfree(venid);
		if(vennum!=(-1)) updatefilerecord2rsrc(vennum,mtnrsrc);
		updateSCRNvirtuals(mtnrsrc);
		return(TRUE);
	}
}
static void changeADDRlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seladdr=0;
	char *vendorid1=NULL;

	readwidget(mtnrsrc,"ADDRESS IDENTIFICATIONS");
	FINDRSCGETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",&seladdr);
	readwidget(mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]",&vendorid1);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",mtn->addravl->libs[seladdr]);
	FINDRSCSETSTRING(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]",mtn->addravl->libs[seladdr]);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",mtn->addravl->libs[seladdr]);
	updatersrc(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]");
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,FALSE);
	ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,(void *)mtnrsrc);
	if(vendorid1!=NULL) Rfree(vendorid1);
}
static void makeaddrlist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid=NULL,*addrid1=NULL;
	char delflag=FALSE;
	int x=0;

	if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
	mtn->addravl=APPlibNEW();
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]",&vendorid1);
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	ef=ADVGTENRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&addrid);
			addAPPlib(mtn->addravl,addrid);
		}
		ef=ADVNXTNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	}
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]",&addrid1);
	if(mtn->addravl->numlibs<1)
	{
		addAPPlib(mtn->addravl,"No Addresses Defined.");
	} else {
		for(x=0;x<mtn->addravl->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addravl->libs[x]))
			break;
		}
		if(x>=mtn->addravl->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",mtn->addravl->libs[x]);
	FINDRSCSETSTRING(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]",mtn->addravl->libs[x]);
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
	if(update) updatefilerecord2rsrc(venadrnum,mtnrsrc);
	else filerecord2rsrc(venadrnum,mtnrsrc);
	FINDRSCLISTAPPlib(mtnrsrc,"ADDRESS IDENTIFICATIONS",x,mtn->addravl);
	if(update)
	{
		updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
		updatersrc(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]");
	}
	if(addrid!=NULL) Rfree(addrid);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void getaddr(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *addrid1=NULL;
	int x;

	FINDFLDGETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",&addrid1);
	if(RDAstrcmp(mtn->addravl->libs[0],"No Addresses Defined."))
	{
		for(x=0;x<mtn->addravl->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addravl->libs[x]))
			break;
		}
		if(x>=mtn->addravl->numlibs)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ADDRESS IDENTIFICATION!","The Vendor Address Identification entered does not exist for the selected Vendor.",NULL,NULL,TRUE,NULL);
		} else {
			FINDRSCSETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",x);
			updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
		}
	}
	if(addrid1!=NULL) Rfree(addrid1);
}
static void getaddrcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *addrid1=NULL;

	readwidget(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]",&addrid1);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",addrid1);
	if(addrid1!=NULL) Rfree(addrid1);
	getaddr(mtnrsrc,mtn);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,FALSE);
	ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,(void *)mtnrsrc);
}
void payddmsm(short dowhich,void *targetkey)
{
	int which_type=0;
	char *name=NULL;
	mtnstruct *mtn=NULL;
	short nofields=0,nokeys=0,x=0,y=0;
	short edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->addravl=NULL;
	mtn->mtnrsrc=mtnrsrc;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	nokeys=NUMKEYS(MASTER->passkey->fileno);
	keys=KEYPOINTER(MASTER->passkey->fileno);
	nofields=NUMFLDS(MASTER->passkey->fileno);
	fields=FLDPOINTER(MASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"ADDRESS IDENTIFICATION"))
			{
				edit_rsrc=editable;
				mtn->addravl=APPlibNEW();
				addAPPlib(mtn->addravl,"No Addresses Defined.");
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addstdrsrc(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]",VARIABLETEXT,0,NULL,edit_rsrc);
					which_type=0;
					addlstrsrc(mtnrsrc,"ADDRESS IDENTIFICATIONS",
						&which_type,edit_rsrc,
						changeADDRlist,mtn->addravl->numlibs,
						&mtn->addravl->libs,mtn);
					FINDRSCSETFUNC(mtnrsrc,"[PAYDDMS][ADDRESS IDENTIFICATION]",getaddrcb,mtn);
				}
			} else {
				edit_rsrc=editable;
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MASTER);
			}
		}
	}
	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->part!=NULL)
			{
				for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
				{
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[PAYDDMS][VENDOR IDENTIFICATION]",check_venid,mtn);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					makeaddrlist(mtnrsrc,mtn,FALSE);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setaddresses(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setaddresses(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				makeaddrlist(mtnrsrc,mtn,FALSE);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setaddresses(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					makeaddrlist(mtnrsrc,mtn,FALSE);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setaddresses(mtnrsrc,mtn,FALSE);
				}
			} else {	
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setaddresses(mtnrsrc,mtn,FALSE);
			}	
			break;
	}
	getbennid(mtnrsrc,FALSE,FALSE);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"LOAD VALUE COMPUTATION",TRUE,loadvalue,
		"[PAYDDMS][COMPUTATION]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR COMPUTATION",TRUE,loadoperand,
		"[PAYDDMS][COMPUTATION]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER COMPUTATION",TRUE,loadgrouper,
		"[PAYDDMS][COMPUTATION]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL COMPUTATION",TRUE,loadcontrol,
		"[PAYDDMS][COMPUTATION]");
	addbtnrsrc(mtnrsrc,"LOAD VALUE RATE SELECTION",TRUE,loadvalue,
		"[PAYDDMS][RATE SELECTION]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR RATE SELECTION",TRUE,loadoperand,
		"[PAYDDMS][RATE SELECTION]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER RATE SELECTION",TRUE,loadgrouper,
		"[PAYDDMS][RATE SELECTION]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL RATE SELECTION",TRUE,loadcontrol,
		"[PAYDDMS][RATE SELECTION]");
	addbtnrsrc(mtnrsrc,"LOAD VALUE GROSS AMOUNT",TRUE,loadvalue,
		"[PAYDDMS][GROSS AMOUNT]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR GROSS AMOUNT",TRUE,loadoperand,
		"[PAYDDMS][GROSS AMOUNT]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER GROSS AMOUNT",TRUE,loadgrouper,
		"[PAYDDMS][GROSS AMOUNT]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL GROSS AMOUNT",TRUE,loadcontrol,
		"[PAYDDMS][GROSS AMOUNT]");
	addbtnrsrc(mtnrsrc,"LOAD VALUE AFFECT SELECT",TRUE,loadvalue,
		"[PAYDDMS][AFFECT SELECT]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR AFFECT SELECT",TRUE,loadoperand,
		"[PAYDDMS][AFFECT SELECT]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER AFFECT SELECT",TRUE,loadgrouper,
		"[PAYDDMS][AFFECT SELECT]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL AFFECT SELECT",TRUE,loadcontrol,
		"[PAYDDMS][AFFECT SELECT]");
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_payddms!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_payddms,mtn);
	addbtnrsrc(mtnrsrc,"BROWSE VENDORS",TRUE,browse_venmsr,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD VALUE COMPUTATION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD OPERATOR COMPUTATION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD GROUPER COMPUTATION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD CONTROL COMPUTATION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD VALUE RATE SELECTION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD OPERATOR RATE SELECTION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD GROUPER RATE SELECTION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD CONTROL RATE SELECTION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD VALUE GROSS AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD OPERATOR GROSS AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD GROUPER GROSS AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD CONTROL GROSS AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD VALUE AFFECT SELECT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD OPERATOR AFFECT SELECT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD GROUPER AFFECT SELECT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD CONTROL AFFECT SELECT",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE VENDORS",FALSE);
	}
	if(mbl_payddms!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_payddms==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);	
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn,TRUE);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selected=0;

	FINDRSCGETINT(mtnrsrc,"ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->addravl->libs[selected],"No Addresses Defined."))
	{
		FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",mtn->addravl->libs[selected]);
	} else {
		FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",NULL);
	}
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,TRUE,mtn);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,FALSE,mtn);
}
static void save_check(RDArsrc *mtnrsrc,short update_list,mtnstruct *mtn)
{
	char *desc=NULL;

	if(!getbennid(mtnrsrc,TRUE,TRUE))
	{
		return;
	}
	if(!SAVE_CHECK(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MASTER->save_expression))
	{
		if(EVALUATEbol(MASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MASTER->save_warning))
	{
		if(EVALUATEbol(MASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,(update_list ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_payddms,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_payddms!=NULL)
		{
			quit_record(mtnrsrc,mtn);
		} else {
			GetRDATData(MASTER->passkey->fileno,mtn->previous);
		}
	}
}
static void setaddresses(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL;
	char delflag=FALSE;
	int x=0;

	if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
	mtn->addravl=APPlibNEW();
	FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",&addrid1);
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	ef=GTENRDsec(venadrnum,1);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(mtn->addravl,addrid2);
		}
		ef=NXTNRDsec(venadrnum,1);
	}
	if(mtn->addravl->numlibs<1)
	{
		addAPPlib(mtn->addravl,"No Addresses Defined");
	} else {
		for(x=0;x<mtn->addravl->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addravl->libs[x]))
			break;
		}
		if(x>=mtn->addravl->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",mtn->addravl->libs[x]);
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
	filerecord2rsrc(venadrnum,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"ADDRESS IDENTIFICATIONS",x,mtn->addravl))
	{
		updatersrc(mtnrsrc,"ADDRESS IDENTIFICATIONS");
	}
	if(addrid1!=NULL) Rfree(addrid1);
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,FALSE,mtn);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
/*
		if(mtn->mtnrsrc!=NULL) mtn->mtnrsrc=NULL;
*/
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount<2)
	{
		if(mbl_venmsr!=NULL) FREE_BROWSE_EXIT(mbl_venmsr);
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn,TRUE);
	getbennid(mtnrsrc,TRUE,FALSE);
}
static void BUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	setaddresses(mtnrsrc,mtn,TRUE);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	getbennid(mtnrsrc,TRUE,TRUE);
}
static char getbennid(RDArsrc *mtnrsrc,short update,short display_diag)
{
	char *benid=NULL,*edesc=NULL;
	char benefit_type=FALSE;
	char return_value=TRUE;	

	FINDRSCGETCHAR(mtnrsrc,"[PAYDDMS][BENEFIT]",&benefit_type);
	if(benefit_type==TRUE)
	{
		FINDRSCGETSTRING(mtnrsrc,"[PAYDDMS][DEDUCTION DESCRIPTION IDENTIFICATION]",&benid);
		FINDFLDSETSTRING(bennum,"DEDUCTION DESCRIPTION IDENTIFICATION",benid);
		if(ADVEQLNRDsec(bennum,1,SCRNvirtualSubData,mtnrsrc) && benid!=NULL)
		{
			KEYNRD(bennum,1);
			if(display_diag)
			{
				edesc=Rmalloc(RDAstrlen(benid)+150);
				sprintf(edesc,"The Benefit Deduction Description Identification [%s] is invalid.  It doesn't exist.  Please enter a correct identification or cancel this operation.",benid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEDUCTION DESCRIPTION IDENTIFICATION!",edesc,NULL,NULL,TRUE,NULL);
				if(edesc!=NULL) Rfree(edesc);
			}
			return_value=FALSE;
		}
		if(benid!=NULL) Rfree(benid);
		if(update) updatefilerecord2rsrc(bennum,mtnrsrc);
	} else {
		ZERNRD(bennum);
		if(update) updatefilerecord2rsrc(bennum,mtnrsrc);
	}
	return(return_value);
}
void browse_payddms(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->addravl!=NULL) freeapplib(mtn->addravl);
		if(mtn->mtnrsrc!=NULL) mtn->mtnrsrc=NULL;
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc); 
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,
		SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_payddms=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,"PAYROLL","MTN PAYDDMS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew("PAYROLL","MTN PAYDDMS");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PAYDDMS",MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"PAYDDMS",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		vennum=APPReturnSupportingFileno(MASTER,"VENPMT","VENMSR",1);
		venadrnum=APPReturnSupportingFileno(MASTER,"VENPMT","VENADR",1);
		bennum=APPReturnSupportingFileno(MASTER,"BFTMGT","BFTDESC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				payddmsm(1,NULL);
				break;
			case 1:
				browse_payddms(NULL,NULL);
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
	if(WindowCount<2)
	{
		if(mbl_venmsr!=NULL) FREE_BROWSE_EXIT(mbl_venmsr);
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mbl_payddms!=NULL) mbl_payddms=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			payddmsm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		payddmsm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	payddmsm(2,NULL);
}
