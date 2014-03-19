/* dochg.c - Do Change */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dochg.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dochg.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <dfchg.hpp>
/*CPP_OPART dfchgcm */
/*CPP_OPART dochg1 */
/*CPP_OPART dochg2 */
/*CPP_OPART dochg3 */
/*CPP_OPART dochg4 */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *devlicense=NULL;
short dev_license=FALSE;
void doexit(void);
CHGstruct *CHGSTRUCT=NULL;
extern void dochg(void);

void shutdownit()
{
	ShutdownSubsystems();
}
static short findCHGstruct(CHGstruct *CHG)
{
	short ef=(-1);
	char *dashes=NULL,*libname=NULL;
	
	dashes=adddashes(CHG->module);
	libname=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
	sprintf(libname,"%s/%s/%s.CHG",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s\\%s.CHG",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
	if(!getCHGstruct(libname,CHG)) ef=FALSE;
	if(ef)
	{
		if(libname!=NULL)
		{
			libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(CHG->module)+11);
		}  else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(CHG->module)+11);
		}
#ifndef WIN32
		sprintf(libname,"%s/site/%s.CHG",CURRENTDIRECTORY,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\site\\%s.CHG",CURRENTDIRECTORY,dashes);
#endif
		if(!getCHGstruct(libname,CHG)) ef=FALSE;
	}
	if(ef)
	{
		if(libname!=NULL)
		{
			libname=Rrealloc(libname,RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(CHG->module)+10);
		}  else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(CHG->module)+10);
		}
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.CHG",CURRENTDIRECTORY,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.CHG",CURRENTDIRECTORY,dashes);
#endif
		if(!getCHGstruct(libname,CHG)) ef=FALSE;
	}
	if(ef)
	{
		prterr("Error Can't find change [%s] [%s].",CHG->module,CHG->name);
	}
	if(dashes!=NULL) Rfree(dashes);
	return(ef);
}
static void PP_translate_CHGSTRUCT(CHGstruct *CHG)
{
	int x,y;
	char *temp=NULL;
	CHGfile *chgfile;
	RDAsearch *s;

	if(CHG!=NULL)
	{
		switch(CHG->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				CHG->from.string_value=NULL;
				CHG->to.string_value=NULL;
				CHG->from_dlen=0;
				CHG->to_dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				CHG->from.string_value=Rmalloc(1);
				*CHG->from.string_value=0;
				CHG->to.string_value=Rmalloc(1);
				*CHG->to.string_value=0;
				CHG->from_dlen=1;
				CHG->to_dlen=1;
				break;
			case DECIMALV:
			case SDECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DOUBLEV:
			case SDOUBLEV:
				CHG->from.float_value=Rmalloc(sizeof(double));
				*CHG->from.float_value=0.0;
				CHG->to.float_value=Rmalloc(sizeof(double));
				*CHG->to.float_value=0.0;
				CHG->from_dlen=sizeof(double);
				CHG->to_dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				CHG->from.short_value=Rmalloc(sizeof(short));
				*CHG->from.short_value=0;
				CHG->to.short_value=Rmalloc(sizeof(short));
				*CHG->to.short_value=0;
				CHG->from_dlen=sizeof(short);
				CHG->to_dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				CHG->from.integer_value=Rmalloc(sizeof(int));
				*CHG->from.integer_value=0;
				CHG->to.integer_value=Rmalloc(sizeof(int));
				*CHG->to.integer_value=0;
				CHG->from_dlen=0;
				CHG->to_dlen=0;
				break;
			default:
				prterr("Error field type [%d] is incorrect.",CHG->type);
				break;
			
		}
		if(!isEMPTY(CHG->expression))
		{
			temp=PP_translate(CHG->expression);
			Rfree(CHG->expression);
			if(temp!=NULL) 
			{
				CHG->expression=stralloc(temp);
				Rfree(temp);
			}	
		}
		if(CHG->chgfiles!=NULL)
		{
			for(y=0,chgfile=CHG->chgfiles;y<CHG->numfiles;++y,++chgfile)
			{
				if(chgfile->supfil!=NULL)
				{
					for(x=0,s=chgfile->supfil;x<chgfile->num;++x,++s)
					{
						if(!isEMPTY(s->expression))
						{
							temp=PP_translate(s->expression);
							Rfree(s->expression);
							if(temp!=NULL)
							{
								s->expression=stralloc(temp);
								Rfree(temp);
							}
						}
					}
				}
				if(!isEMPTY(chgfile->expression))
				{
					temp=PP_translate(chgfile->expression);
					Rfree(chgfile->expression);
					if(temp!=NULL)
					{
						chgfile->expression=stralloc(temp);
						Rfree(temp);
					}
				}
			}
		}
	}
}
static short OPEN_CHANGE_FILES(CHGstruct *chg)
{
	int x,y;
	CHGfile *chgfile=NULL;
	RDAsearch *s=NULL;

	if(chg!=NULL)
	{
		for(x=0,chgfile=chg->chgfiles;x<chg->numfiles;++x,++chgfile)
		{
			if((chgfile->fileno=APPOPNNRD(chgfile->module,chgfile->filename,TRUE,TRUE))==(-1)) return(FALSE);
			if(!isEMPTY(chgfile->keyname)) chgfile->keyno=KEYNUMBER(chgfile->fileno,chgfile->keyname);
			if((chgfile->keyno==(-1)) || (chgfile->keyno<1)) chgfile->keyno=1;
			if(chgfile->supfil!=NULL)
			{
				for(y=0,s=chgfile->supfil;y<chgfile->num;++y,++s)
				{
					if((s->fileno=APPOPNNRD(s->module,s->filename,TRUE,FALSE))==(-1)) return(FALSE);
					if(!isEMPTY(s->subkeyname)) s->keyno=KEYNUMBER(s->fileno,s->subkeyname);
					if((s->keyno==(-1)) || (s->keyno<1)) s->keyno=1;
				}
			}
		}
	}
	return(TRUE);
}
static void CLOSE_CHANGE_FILES(CHGstruct *chg)
{
	int x,y;
	CHGfile *chgfile=NULL;
	RDAsearch *s=NULL;

	if(chg!=NULL)
	{
		for(x=0,chgfile=chg->chgfiles;x<chg->numfiles;++x,++chgfile)
		{
			if(chgfile->fileno!=(-1)) CLSNRD(chgfile->fileno);
			if(chgfile->supfil!=NULL)
			{
				for(y=0,s=chgfile->supfil;y<chgfile->num;++y,++s)
				{
					if(s->fileno!=(-1)) CLSNRD(s->fileno);
				}
			}
		}
	}
}
static void StartDochg()
{
	if(!OPEN_CHANGE_FILES(CHGSTRUCT))
	{
		doexit();
		return;
	}
	PP_translate_CHGSTRUCT(CHGSTRUCT);
	dochg();
}
static void setdevlicense(RDArsrc *r)
{
	readwidget(r,"DEV_LICENSE");
	FINDRSCGETSTRING(r,"DEV_LICENSE",&devlicense);
	if(CheckDevLicense(devlicense))
	{
		dev_license=TRUE;
	} else {
		dev_license=FALSE;
	}
	FINDRSCSETEDITABLE(r,"CONTINUE",(dev_license==TRUE ? TRUE:FALSE));
	FINDRSCSETSENSITIVE(r,"CONTINUE",(dev_license==TRUE ? TRUE:FALSE));
	updateallrsrc(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *warnmessage=NULL;
	char *tmp=NULL,*temp1=NULL;
	RDArsrc *dl_rsrc=NULL;

	if(InitializeSubsystems(argc,argv,argv[1],argv[2]))
	{
		ShutdownSubsystems();
		return;
	}
	if(argc<3) 
	{
		ShutdownSubsystems();
		return;
	}
	CHGSTRUCT=CHGstructNEW(argv[1],argv[2]);
	if(findCHGstruct(CHGSTRUCT))
	{
		warnmessage=Rmalloc(300+RDAstrlen(argv[1])+RDAstrlen(argv[2]));
		sprintf(warnmessage,"Error Change Definition not found, this process cannot be executed without it's definition [%s] in the [%s.CHG] library.",argv[2],argv[1]);
		ERRORDIALOG("CHANGE NOT FOUND.",warnmessage,NULL,TRUE); 
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit();
	} else {
		tmp=RDA_GetEnv("DEV_LICENSE");
		if(tmp!=NULL)
		{
			temp1=strtok(tmp," ");
			devlicense=stralloc(temp1);
			if(!CheckDevLicense(devlicense))
			{
				if(devlicense!=NULL) Rfree(devlicense);
				dl_rsrc=RDArsrcNEW("DATABASE","DOCHG ACCESS");
				addstdrsrc(dl_rsrc,"DEV_LICENSE",VARIABLETEXT,0,devlicense,TRUE);
				FINDRSCSETFUNC(dl_rsrc,"DEV_LICENSE",setdevlicense,NULL);
				addbtnrsrc(dl_rsrc,"CONTINUE",TRUE,StartDochg,NULL);
				addbtnrsrc(dl_rsrc,"QUIT",TRUE,doexit,NULL);
				addbtnrsrc(dl_rsrc,"HELP",TRUE,screenhelp,NULL);
				dl_rsrc->scn=RDAscrnNEW("DATABASE","DOCHG ACCESS");
				ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,5,"","Implementation of the Need for an Access Code was done to ensure data integrity.","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,5,"","If an identification needs to be changed, and you require an Access Code contact your","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,5,"","Account Manager at RDA Systems for pricing and scheduling of data conversion.","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,5,"","Access Code (Dev_License):","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,0,"DEV_LICENSE","Development License","","",0,40,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,3,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,6,"CONTINUE","Continue","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,6,"QUIT","Cancel","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,6,"HELP","Help","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,4,"","","","",0,0,0,"","","","");
				ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
				setdevlicense(dl_rsrc);
				APPmakescrn(dl_rsrc,TRUE,doexit,NULL,TRUE);
			} else {
				StartDochg();
			}
		} else {
			dl_rsrc=RDArsrcNEW("DATABASE","DOCHG ACCESS");
			addstdrsrc(dl_rsrc,"DEV_LICENSE",VARIABLETEXT,0,devlicense,TRUE);
			FINDRSCSETFUNC(dl_rsrc,"DEV_LICENSE",setdevlicense,NULL);
			addbtnrsrc(dl_rsrc,"CONTINUE",TRUE,StartDochg,NULL);
			addbtnrsrc(dl_rsrc,"QUIT",TRUE,doexit,NULL);
			addbtnrsrc(dl_rsrc,"HELP",TRUE,screenhelp,NULL);
			dl_rsrc->scn=RDAscrnNEW("DATABASE","DOCHG ACCESS");
			ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,5,"","Implementation of the Need for an Access Code was done to ensure data integrity.","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,5,"","If an identification needs to be changed, and you require an Access Code contact your","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,5,"","Account Manager at RDA Systems for pricing and scheduling of data conversion.","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,5,"","Access Code (Dev_License):","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,0,"DEV_LICENSE","Development License","","",0,40,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,3,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,6,"CONTINUE","Continue","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,6,"QUIT","Cancel","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,6,"HELP","Help","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,4,"","","","",0,0,0,"","","","");
			ADVaddwdgt(dl_rsrc->scn,2,"","","","",0,0,0,"","","","");
			setdevlicense(dl_rsrc);
			APPmakescrn(dl_rsrc,TRUE,doexit,NULL,TRUE);
		}
	}
}
void doexit(void)
{
	CLOSE_CHANGE_FILES(CHGSTRUCT);
	if(CHGSTRUCT!=NULL) 
	{
		switch(CHGSTRUCT->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case BOOLNS:
			case CHARACTERS:
				if(CHGSTRUCT->from.string_value!=NULL)
					Rfree(CHGSTRUCT->from.string_value);
				if(CHGSTRUCT->to.string_value!=NULL)
					Rfree(CHGSTRUCT->to.string_value);
				break;
			case DECIMALV:
			case SDECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DOUBLEV:
			case SDOUBLEV:
				if(CHGSTRUCT->from.float_value!=NULL)
					Rfree(CHGSTRUCT->from.float_value);
				if(CHGSTRUCT->to.float_value!=NULL)
					Rfree(CHGSTRUCT->to.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				if(CHGSTRUCT->from.short_value!=NULL)
					Rfree(CHGSTRUCT->from.short_value);
				if(CHGSTRUCT->to.short_value!=NULL)
					Rfree(CHGSTRUCT->to.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(CHGSTRUCT->from.integer_value!=NULL)
					Rfree(CHGSTRUCT->from.integer_value);
				if(CHGSTRUCT->to.integer_value!=NULL)
					Rfree(CHGSTRUCT->to.integer_value);
				break;
			default:
				prterr("Error field type [%d] is incorrect.",CHGSTRUCT->type);
				break;
			
		}
		FreeCHGstruct(CHGSTRUCT);
	}
	ShutdownSubsystems();
}
