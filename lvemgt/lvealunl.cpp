/* lvealu.c - Leave Management Add Leave Used */
#ifndef WIN32
#define __NAM__ "lvealunl.lnx"
#endif
#ifdef WIN32
#define __NAM__ "lvealunl.exe"
#endif
#include <app.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <addlists.h>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static short filenum=0,lmsrnum=0,ldescnum=0,ltypenum=0,dmgnum=0;
static short jmstnum=0,posmstr=0,lvejob=(-1);
static MakeBrowseList *mbl_perdmg=NULL;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static short selectfunction(MakeBrowseList *);
static RDArsrc *definelist=NULL;
static APPlib *prevlve=NULL;
static APPlib *lvedesclist=NULL;
static APPlib *jobnolist=NULL;
static char *module="LVEMGT";
static int counter=0;
static void set_defaults(RDArsrc *);
static void savelist(RDArsrc *,RDArsrc *),savelistcb(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_record(RDArsrc *),quit_record(RDArsrc *);
static void quit_record_free(RDArsrc *),quit_record_freens(RDArsrc *);
static void save_check(RDArsrc *),quit_check(RDArsrc *);
static short check_perid(RDArsrc *,short),check_lveid(RDArsrc *,short);
static short check_jobno(RDArsrc *,short);
static void check_lveidcb(RDArsrc *),check_peridcb(RDArsrc *);
static void check_jobnocb(RDArsrc *);
static void getsupportingrecords(RDArsrc *,short);
static void changeper(RDArsrc *,short);
static void changelve(RDArsrc *,short);
static void changelvecb(RDArsrc *);
static void changejobno(RDArsrc *,short);
static void changejobnocb(RDArsrc *);
static void browse_perdmg(RDArsrc *);
char *personnel_identification=NULL,*leave_description=NULL;
int job_number=(-1);
char *pareportname=NULL,*audit_reportname=NULL;

static void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,RDArsrc *parent)
{
	short ef=0,earntype=0;
	char *username=NULL,deleteflag=FALSE,verified=FALSE;
	short update_diag=FALSE;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(prevlve!=NULL) freeapplib(prevlve);
	prevlve=APPlibNEW();
	if(tmprsrc!=NULL)
	{
		if(!ADVmakescrn(tmprsrc,TRUE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	ef=FRSNRDsec(filenum,1);
	while(!ef)
	{
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		FINDFLDGETSHORT(filenum,"TYPE",&earntype);
		FINDFLDGETCHAR(filenum,"SOURCE USER VERIFIED",&verified);
		if(!deleteflag && earntype==1 && !verified)
		{
			FINDFLDGETSTRING(filenum,"SOURCE USER",&username);
			if(!RDAstrcmp(username,USERLOGIN))
			{
				makedflist();
				update_diag=TRUE;
			}
		}
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
		ef=NXTNRDsec(filenum,1);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
        if(prevlve->numlibs<1)
        {
                addAPPlib(prevlve,"No Leaves Used added this session");
        }
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS LEAVES USED",(prevlve->numlibs-1),prevlve))
	{
		updatersrc(parent,"PREVIOUS LEAVES USED");
	}
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(prevlve!=NULL)
	{
		if(prevlve->numlibs==1 && !RDAstrcmp(prevlve->libs[0],
			"No Leaves Used added this session"))
		{
			freeapplib(prevlve);
			prevlve=APPlibNEW();
		}
	} else {
		prevlve=APPlibNEW();
	}
	for(x=1,length=0;x<(definelist->numrscs-5);x+=2)
	{
		pos_member=definelist->rscs+x;
		len_member=definelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(definelist->numrscs-5))
	{
		for(x=1;x<(definelist->numrscs-5);x+=2)
		{
			pos_member=definelist->rscs+x;
			len_member=definelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(filenum,s);
					if(field!=NULL)
					{
						switch(field->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case EXPENDITURE:
							case BALANCESHEET:
							case REVENUE:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(*len_member->value.short_value<RDAstrlen(field->data.string_value))
								{
									sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
								} else {
									sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
								}
								break;
							case DECIMALV:
							case SDECIMALV:
								sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,*field->data.float_value);
								break;
							case SHORTV:
							case SSHORTV:
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.short_value);
								break;
							case LONGV:
							case SLONGV:
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.integer_value);
								break;
							case DOLLARS:
								temp=famt(*field->data.float_value,*len_member->value.short_value);
								sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
								if(temp!=NULL) Rfree(temp);
								break;
							case DOUBLEV:
							case SDOUBLEV:
								sprintf(&hold[lcount],"%*.0f ",*len_member->value.short_value,*field->data.float_value);
								break;
							case BOOLNS:
								sprintf(&hold[lcount],"%c ",(*field->data.string_value ? 'Y':'N'));
								break;
							case CHARACTERS:
								sprintf(&hold[lcount],"%c ",*field->data.string_value);
								break;
							case BUTTONS:
							case SCROLLEDLIST:
								prterr("Huh? Do a %s?",standardfieldtypes[field->type]);
								break;
							default:
								prterr("Invalid Field Type [%d]",field->type);
								break;
						
						}
						lcount+=*len_member->value.short_value+1;
					} else {
						prterr("Error field [%s] not found.",s);
						if(s!=NULL) Rfree(s);
					}
				}
			}
		}
		++cur;
	}
	addAPPlib(prevlve,hold);
	++counter;
	if(hold!=NULL) Rfree(hold);
}
void definelistfunc(RDArsrc *parent)
{
	updateSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void set_defaults(RDArsrc *parent)
{
	int selprev=0;
	char *perid1=NULL,*lvedescid1=NULL;

	FINDRSCGETINT(parent,"PREVIOUS LEAVES USED",&selprev);
	DefaultResetScreen(parent,TRUE);
	changeper(parent,FALSE);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	getsupportingrecords(parent,TRUE);
	if(perid1!=NULL) Rfree(perid1);
	if(perid1!=NULL) Rfree(perid1);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void check_peridcb(RDArsrc *parent)
{
	char *perid1=NULL;

	readwidget(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]");
	readwidget(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	if(perid1!=NULL) Rfree(perid1);
	check_perid(parent,FALSE);
	changeper(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static short check_perid(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*lvedescid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE IDENTIFICATION]",&lvedescid1);
	if(dmgnum!=(-1))
	{
		ZERNRD(dmgnum);
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(dmgnum,1))
		{
			KEYNRD(dmgnum,1);
			warnmessage=Rmalloc(RDAstrlen(perid1)+145);
			sprintf(warnmessage,"The Personnel Identification of [%s] is an invalid value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
			getsupportingrecords(parent,updatesupporting);
			if(perid1!=NULL) Rfree(perid1);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+150);
				sprintf(warnmessage,"The Personnel Identification of [%s] is a deleted value.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
				FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
				getsupportingrecords(parent,updatesupporting);
				if(perid1!=NULL) Rfree(perid1);
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				return(FALSE);
			} else {
				FINDFLDGETCHAR(dmgnum,"CURRENT EMPLOYEE",&delflag);
				if(!delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(perid1)+250);
					sprintf(warnmessage,"The Personnel Identification of [%s] is not a CURRENT EMPLOYEE in the [PRSNNL][PERDMG] data file.  Please select a valid Personnel Identification to save this record.",(perid1==NULL?"":perid1));
					WARNINGDIALOG("WARNING DIALOG SCREEN","NOT CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					getsupportingrecords(parent,updatesupporting);
					if(perid1!=NULL) Rfree(perid1);
					if(lvedescid1!=NULL) Rfree(lvedescid1);
					return(FALSE);
				}
			}
		}
	} else {
		if(perid1!=NULL) Rfree(perid1);
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return(FALSE);
	}
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	getsupportingrecords(parent,updatesupporting);
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	return(TRUE);
}
static void check_lveidcb(RDArsrc *parent)
{
	char *perid1=NULL,*lvedescid1=NULL;
	int x=0;

	readwidget(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	readwidget(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(AddList) /* treat like a Search Resource */
	{
		if(lvedescid1!=NULL)
		{
			if(RDAstrlen(lvedescid1)!=0)
			{
				for(x=0;x<lvedesclist->numlibs;++x)
				{
					if(strncmp(lvedescid1,lvedesclist->libs[x],RDAstrlen(lvedescid1))<=0)
					{
						break;
					}
				}
				if(x>=lvedesclist->numlibs) x=(lvedesclist->numlibs-1);
			}
		}
		FINDRSCSETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",x);
		FINDRSCSETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedesclist->libs[x]);
		updatersrc(parent,"LEAVE DESCRIPTION IDENTIFICATIONS");
		updatersrc(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
		FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedesclist->libs[x]);
		FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
		getsupportingrecords(parent,TRUE);
	} else {
		check_lveid(parent,TRUE);
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	updateSCRNvirtuals(parent);
}
static short check_lveid(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*lvedescid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;
	
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(ldescnum!=(-1))
	{
		ZERNRD(ldescnum);
		FINDFLDSETSTRING(ldescnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(ldescnum,1))
		{
			KEYNRD(ldescnum,1);
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+145);
			sprintf(warnmessage,"The Leave Description Identification of [%s] is an invalid value.  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
			FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
			getsupportingrecords(parent,updatesupporting);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(ldescnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(lvedescid1)+150);
				sprintf(warnmessage,"The Leave Description Identification of [%s] is a deleted value.  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
				FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
				getsupportingrecords(parent,updatesupporting);
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				if(perid1!=NULL) Rfree(perid1);
				return(FALSE);
			}
		}
	} else {
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	}
	if(lmsrnum!=(-1))
	{
		ZERNRD(lmsrnum);
		FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(lmsrnum,1))
		{
			KEYNRD(lmsrnum,1);
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+RDAstrlen(perid1)+190);
			sprintf(warnmessage,"The Leave Description Identification of [%s] is an invalid value for the Personnel Identification of [%s].  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1),(perid1==NULL
?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
			FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
			getsupportingrecords(parent,updatesupporting);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(lmsrnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(lvedescid1)+RDAstrlen(perid1)+200);
				sprintf(warnmessage,"The Leave Description Identification of [%s] is a deleted value for the Personnel Identification of [%s].  Please select a valid Leave Description Identification to save this record.",(lvedescid1==NULL?"":lvedescid1),(perid1==NULL
?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
				FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
				getsupportingrecords(parent,updatesupporting);
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				if(perid1!=NULL) Rfree(perid1);
				return(FALSE);
			}
		}
	} else {
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	} 
	FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	getsupportingrecords(parent,updatesupporting);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	if(perid1!=NULL) Rfree(perid1);
	return(TRUE);
}
static void check_jobnocb(RDArsrc *parent)
{
	char *perid1=NULL,*jobnostr=NULL;
	int x=0,jobno1=(-1);

	readwidget(parent,"[LVETRAN][JOB NUMBER]");
	readwidget(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]");
	FINDRSCGETINT(parent,"[LVETRAN][JOB NUMBER]",&jobno1);
	jobnostr=Rmalloc(7);
	sprintf(jobnostr,"%6d",jobno1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(AddList) /* treat like a Search Resource */
	{
		if(jobnostr!=NULL)
		{
			for(x=0;x<jobnolist->numlibs;++x)
			{
				if(strncmp(jobnostr,jobnolist->libs[x],RDAstrlen(jobnostr))<=0)
				{
					break;
				}
			}
			if(x>=jobnolist->numlibs) x=(jobnolist->numlibs-1);
		}
		FINDRSCSETINT(parent,"JOB NUMBERS",x);
		jobno1=Ratoi(jobnostr);
		FINDRSCSETINT(parent,"[LVETRAN][JOB NUMBER]",jobno1);
		updatersrc(parent,"JOB NUMBERS");
		updatersrc(parent,"[LVETRAN][JOB NUMBER]");
		getsupportingrecords(parent,TRUE);
	} else {
		check_jobno(parent,TRUE);
	}
	if(jobnostr!=NULL) Rfree(jobnostr);
	if(perid1!=NULL) Rfree(perid1);
	updateSCRNvirtuals(parent);
}
static short check_jobno(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*warnmessage=NULL;
	int jobno1=(-1);
	char delflag=FALSE;

	FINDRSCGETINT(parent,"[LVETRAN][JOB NUMBER]",&jobno1);
	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	if(jmstnum!=(-1))
	{
		ZERNRD(jmstnum);
		FINDFLDSETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETINT(jmstnum,"JOB NUMBER",jobno1);
		if(EQLNRDsec(jmstnum,1))
		{
			KEYNRD(jmstnum,1);
			warnmessage=Rmalloc(RDAstrlen(perid1)+150);
			sprintf(warnmessage,"The Job Number of [%8d] is an invalid value for the Personnel Identification of [%s].  Please select a valid Job Number to save this record.",jobno1,(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			getsupportingrecords(parent,updatesupporting);
			if(perid1!=NULL) Rfree(perid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(jmstnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(perid1)+152);
				sprintf(warnmessage,"The Job Number of [%8d] is a deleted value for the Personnel Identification of [%s].  Please select a valid Job Number to save this record.",jobno1,(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				getsupportingrecords(parent,updatesupporting);
				if(perid1!=NULL) Rfree(perid1);
                                return(FALSE);
			}
		}
	} else {
		if(perid1!=NULL) Rfree(perid1);
		return(FALSE);
	}
	getsupportingrecords(parent,updatesupporting);
	if(perid1!=NULL) Rfree(perid1);
	return(TRUE);
}
static void getsupportingrecords(RDArsrc *parent,short updatesupporting)
{
	char *perid1=NULL,*lvedescid1=NULL;
	int jobno1=(-1),seljobno=0;

	FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
		jobno1=Ratoi(jobnolist->libs[seljobno]);
	} else {
		FINDRSCGETINT(parent,"[LVETRAN][[JOB NUMBER]",&jobno1);
	}
	FINDFLDSETINT(filenum,"JOB NUMBER",jobno1);
	if(dmgnum!=(-1))
	{
		FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(dmgnum,1)) KEYNRD(dmgnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(dmgnum,parent);
		} else {
			filerecord2rsrc(dmgnum,parent);
		}
	}
        if(jmstnum!=(-1))
        {
		FINDFLDSETINT(jmstnum,"JOB NUMBER",jobno1);
		FINDFLDSETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",perid1);
		if(EQLNRDsec(jmstnum,1)) KEYNRD(jmstnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(jmstnum,parent);
		} else {
			filerecord2rsrc(jmstnum,parent);
		}
	}
	if(lvejob!=(-1))
	{
		FINDFLDSETINT(lvejob,"JOB NUMBER",jobno1);
		FINDFLDSETSTRING(lvejob,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETSTRING(lvejob,"LEAVE DESCRIPTION IDENTIFICATION",
			lvedescid1);
		if(EQLNRDsec(lvejob,1)) KEYNRD(lvejob,1);
		if(updatesupporting) updatefilerecord2rsrc(lvejob,parent);
			else filerecord2rsrc(lvejob,parent);
	}
        if(posmstr!=(-1))
        {
                COPYFIELD(jmstnum,posmstr,"POSITION IDENTIFICATION");
                if(EQLNRDsec(posmstr,1)) KEYNRD(posmstr,1);
                if(updatesupporting)
                {
                        updatefilerecord2rsrc(posmstr,parent);
                } else {
                        filerecord2rsrc(posmstr,parent);
                }
        }
	if(lmsrnum!=(-1))
	{
		FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(lmsrnum,1)) KEYNRD(lmsrnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(lmsrnum,parent);
		} else {
			filerecord2rsrc(lmsrnum,parent);
		}
	}
	if(ldescnum!=(-1))
	{
		FINDFLDSETSTRING(ldescnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(EQLNRDsec(ldescnum,1)) KEYNRD(ldescnum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(ldescnum,parent);
		} else {
			filerecord2rsrc(ldescnum,parent);
		}
	}
	if(ltypenum!=(-1))
	{
		COPYFIELD(ldescnum,ltypenum,"LEAVE TYPE IDENTIFICATION");
		if(EQLNRDsec(ltypenum,1)) KEYNRD(ltypenum,1);
		if(updatesupporting)
		{
			updatefilerecord2rsrc(ltypenum,parent);
		} else {
			filerecord2rsrc(ltypenum,parent);
		}
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
}
static void save_check(RDArsrc *parent)
{
	int sellvedesc=0,seljobno=0;
	char *perid1=NULL,*lvedescid1=NULL,*warnmessage=NULL,allnegbal=TRUE;
	double amount=0.0,beginningamt=0.0,tempamt=0.0;
	double earningsamt=0.0,upearningsamt=0.0,usedamt=0.0,upusedamt=0.0;
	int jobno1=0;

	readallwidgets(parent);
	if(check_perid(parent,FALSE)==FALSE) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
		if((RDAstrcmp(lvedesclist->libs[sellvedesc],"No Leave Descriptions Available")) &&
		(RDAstrcmp(lvedesclist->libs[sellvedesc],"Not Applicable")))
		{
			lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
		}
		FINDRSCSETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedescid1);
		FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
		jobno1=Ratoi(jobnolist->libs[seljobno]);
	} else {
		FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
		FINDRSCGETINT(parent,"[LVETRAN][JOB NUMBER]",&jobno1);
	}
	FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	FINDFLDSETINT(filenum,"JOB NUMBER",jobno1);
	if(check_lveid(parent,FALSE)==FALSE)
	{
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	if(check_jobno(parent,FALSE)==FALSE)
	{
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	getsupportingrecords(parent,TRUE);
	FINDRSCGETDOUBLE(parent,"[LVETRAN][AMOUNT]",&amount);
	if(amount==0.0)
	{
		ERRORDIALOG("INVALID AMOUNT!","This Leave Used Transaction can not be saved with an AMOUNT of [0.00].",NULL,FALSE);
		if(perid1!=NULL) Rfree(perid1);
		if(lvedescid1!=NULL) Rfree(lvedescid1);
		return;
	}
	FINDFLDGETCHAR(ldescnum,"ALLOW NEG BALANCES",&allnegbal);
	if(!allnegbal)
	{
		FINDFLDGETDOUBLE(lvejob,"BEGINNING BALANCE",&beginningamt);
		FINDFLDGETDOUBLE(lvejob,"PROCESSED EARNINGS",&earningsamt);
		FINDFLDGETDOUBLE(lvejob,"UNPROCESSED EARNINGS",&upearningsamt);
		FINDFLDGETDOUBLE(lvejob,"PROCESSED USED",&usedamt);
		FINDFLDGETDOUBLE(lvejob,"UNPROCESSED USED",&upusedamt);
		tempamt=beginningamt+earningsamt+upearningsamt-usedamt-upusedamt-amount;
		if(tempamt<0.0)
		{
			warnmessage=Rmalloc(RDAstrlen(lvedescid1)+50+363);
			sprintf(warnmessage,"The Leave Description Identification of [%s] is not allowed to have a Negative Balance.  This Transaction would make the Balance of this Leave Description to be negative.  The calculation of (Beginning Balance[%10f] + Processed[%10f] and Unprocessed[%10f] Earnings - Processed[%10f] and Unprocessed[%10f] Used) is used to in the determinization of this balance.",(lvedescid1==NULL?"":lvedescid1),beginningamt,earningsamt,upearningsamt,usedamt,upusedamt);
			WARNINGDIALOG("WARNING DIALOG SCREEN","CAN'T ALLOW NEGATIVE BALANCE!",warnmessage,NULL,NULL,TRUE,NULL);
			if(perid1!=NULL) Rfree(perid1);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		}
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	save_record(parent);
}
static void check_lvevalues(double units,double *avl,double *dockedunits)
{
	double begbal=0.0,earnings=0.0,upearnings=0.0;
	double used=0.0,upused=0.0,total=0.0;

	FINDFLDGETDOUBLE(lvejob,"BEGINNING BALANCE",&begbal);
	FINDFLDGETDOUBLE(lvejob,"PROCESSED EARNINGS",&earnings);
	FINDFLDGETDOUBLE(lvejob,"UNPROCESSED EARNINGS",&upearnings);
	FINDFLDGETDOUBLE(lvejob,"PROCESSED USED",&used);
	FINDFLDGETDOUBLE(lvejob,"UNPROCESSED USED",&upused);
	total=begbal+earnings+upearnings-used-upused;
	*avl=(total-units);
	if(total>0.0 && total<units) *dockedunits=units-total;
	else if(total<=0.0) *dockedunits=units;
}
static void save_record(RDArsrc *parent)
{
	int x=0,lf=0,sellvedesc=0,seljobno=0,jobno1=(-1);
	double amount=0.0,tempamt=0.0,avl=0.0,docked_units=0.0;
	char *rcddesc=NULL,deleteflag=FALSE;
	char *perid1=NULL,*lvedescid1=NULL;
	char *date=NULL,*timex=NULL;
	RDArmem *member=NULL;
	RDATData *prev=NULL;
	NRDfield *field=NULL;

	ZERNRD(filenum);
	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
		lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
	} else {
		FINDRSCGETSTRING(parent,"[LVEMGT][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	}
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
		jobno1=Ratoi(jobnolist->libs[seljobno]);
	} else {
		FINDRSCGETINT(parent,"[LVETRAN][JOB NUMBER]",&jobno1);
	}
	rsrc2filerecord(filenum,parent);
	FINDFLDSETSTRING(filenum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(filenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(filenum,"SOURCE USER VERIFIED",deleteflag);
	FINDFLDSETINT(filenum,"JOB NUMBER",jobno1);
	FINDFLDSETSHORT(filenum,"TYPE",1);
	FINDFLDGETDOUBLE(filenum,"AMOUNT",&amount);
	if((x=FINDRSC(parent,"[LVETRAN][ENTRY DATE]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			if(member->field_length>8) date=GETCURRENTDATE10();
				else date=GETCURRENTDATE();
			lf=RDAstrlen(date);
			if(date!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,date,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
		}
	} else {
		field=FLDNRD(filenum,"ENTRY DATE");
		if(field!=NULL)
		{
			if(field->len>8) date=GETCURRENTDATE10();
				else date=GETCURRENTDATE();
		} else date=GETCURRENTDATE();
		FINDFLDSETSTRING(filenum,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date);
	if((x=FINDRSC(parent,"[FINREC][ENTRY TIME]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			timex=GETCURRENTTIME();
			lf=RDAstrlen(timex);
			if(timex!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,timex,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
		}
	} else {
		timex=GETCURRENTTIME();
		FINDFLDSETSTRING(filenum,"ENTRY TIME",timex);
	}
	if(timex!=NULL) Rfree(timex);
	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  RECORDsec() failed without a description returned!");
		}
	} else {
		ZERNRD(lvejob);
		COPYFIELD(filenum,lvejob,"PERSONNEL IDENTIFICATION");
		COPYFIELD(filenum,lvejob,"LEAVE DESCRIPTION IDENTIFICATION");
		COPYFIELD(filenum,lvejob,"JOB NUMBER");
		if(EQLNRDsec(lvejob,1)) KEYNRD(lvejob,1);
		check_lvevalues(amount,&avl,&docked_units);
		FINDFLDSETDOUBLE(filenum,"DOCK UNITS",docked_units);
		if(!WRTTRANS(filenum,1,"TRANSACTION NUMBER",NULL))
		{
			FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
			LOCNRDFILE(lmsrnum);
			if(EQLNRDsec(lmsrnum,1))
			{
			} else {
				prev=RDATDataNEW(lmsrnum);
				FINDFLDGETDOUBLE(lmsrnum,"UNPROCESSED USED",&tempamt);
				tempamt+=amount;
				FINDFLDSETDOUBLE(lmsrnum,"UNPROCESSED USED",tempamt);
				if(WRTTRANSsec(lmsrnum,0,NULL,prev))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the field [LVEMGT][LVEMSTR][UNPROCESSED USED].",NULL,NULL,TRUE,NULL);
				}
				if(prev!=NULL) FreeRDATData(prev);
			}
			UNLNRDFILE(lmsrnum);
			FINDFLDSETSTRING(lvejob,"PERSONNEL IDENTIFICATION",perid1);
			FINDFLDSETSTRING(lvejob,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
			FINDFLDSETINT(lvejob,"JOB NUMBER",jobno1);
			LOCNRDFILE(lvejob);
			if(EQLNRDsec(lvejob,1))
			{
			} else {
				prev=RDATDataNEW(lvejob);
				FINDFLDGETDOUBLE(lvejob,"UNPROCESSED USED",&tempamt);
				tempamt+=amount;
				FINDFLDSETDOUBLE(lvejob,"UNPROCESSED USED",tempamt);
				if(WRTTRANSsec(lvejob,0,NULL,prev))
				{
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to update the field [LVEMGT][LVEJOB][UNPROCESSED USED].",NULL,NULL,TRUE,NULL);
				}
				if(prev!=NULL) FreeRDATData(prev);
			}
			UNLNRDFILE(lvejob);
			makedflist();
		} else {
			ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed, the record you attempted to save was not created. Please try again.",NULL,FALSE);
		}
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS LEAVES USED",(prevlve->numlibs-1),
		prevlve))
	{
		updatersrc(parent,"PREVIOUS LEAVES USED");
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	updateSCRNvirtuals(parent);
}
static void quit_check(RDArsrc *parent)
{
	quit_record(parent);
}
static void quit_record_freens(RDArsrc *parent)
{
	counter=0;
	quit_record_free(parent);
}
static void quit_record_free(RDArsrc *parent)
{
	APPlib *args=NULL;

	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist);
	}
	if(parent!=NULL) free_rsrc(parent);
	if(mbl_perdmg!=NULL) FREE_BROWSE_EXIT(mbl_perdmg);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(dmgnum!=(-1)) CLSNRD(dmgnum);
	if(jmstnum!=(-1)) CLSNRD(jmstnum);
	if(lvejob!=(-1)) CLSNRD(lvejob);
	if(posmstr!=(-1)) CLSNRD(posmstr);
	if(lmsrnum!=(-1)) CLSNRD(lmsrnum);
	if(ldescnum!=(-1)) CLSNRD(ldescnum);
	if(prevlve!=NULL) freeapplib(prevlve);
	if(lvedesclist!=NULL) freeapplib(lvedesclist);
	if(leave_description!=NULL) Rfree(leave_description);
	if(personnel_identification!=NULL) Rfree(personnel_identification);
	if(counter>0)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,audit_reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(audit_reportname!=NULL) Rfree(audit_reportname);
}
static void quit_record(RDArsrc *parent)
{
	quit_record_free(parent);
	ShutdownSubsystems();
	exit(0);
}
static void changeper(RDArsrc *parent,short check)
{
	short ef=0;
	short keyn=0,keynumber=0;
	int sellvedesc=0,seljobno=0,jobno1=(-1);
	char active=TRUE,deleteflag=FALSE;
/*
	char *e=NULL;
	RDArsrc *tmprsrc=NULL;
	short listupdated=FALSE;
*/
	char *perid1=NULL,*perid2=NULL,*lvedescid1=NULL,*jobnostr=NULL;

	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		if(RDAstrcmp(perid1,personnel_identification) || personnel_identification==NULL)
		{
			if(lvedesclist!=NULL) freeapplib(lvedesclist);
			lvedesclist=APPlibNEW();
			if(jobnolist!=NULL) freeapplib(jobnolist);
			jobnolist=APPlibNEW();
			FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
                        if((keyn=KEYNUMBER(lmsrnum,"LVEMSTR KEY"))!=(-1))
                        {
                                keynumber=keyn;
                        } else {
                                keynumber=1;
                        }
			ZERNRD(lmsrnum);
			FINDFLDSETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",perid1);
			ef=GTENRDsec(lmsrnum,keynumber);
/*
			e=stralloc("Updating Leave Descriptions List");
			tmprsrc=diagscrn(lmsrnum,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
					ForceWindowUpdate(tmprsrc);
			}
*/
			while(!ef)
			{
/*
				listupdated=FALSE;
*/
				FINDFLDGETSTRING(lmsrnum,"PERSONNEL IDENTIFICATION",&perid2);
				if(RDAstrcmp(perid2,perid1)) break;
				FINDFLDGETSTRING(lmsrnum,"LEAVE DESCRIPTION IDENTIFICATION",&lvedescid1);
				FINDFLDGETCHAR(lmsrnum,"ACTIVE",&active);
				FINDFLDGETCHAR(lmsrnum,"DELETEFLAG",&deleteflag);
				if(active && !deleteflag)
				{
					FINDFLDSETSTRING(ldescnum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
					if(EQLNRDsec(ldescnum,1))
					{
						KEYNRD(ldescnum,1);
					} else {
						FINDFLDGETCHAR(ldescnum,"ACTIVE",&active);
						FINDFLDGETCHAR(ldescnum,"DELETEFLAG",&deleteflag);
						if(active && !deleteflag)
						{
							addAPPlib(lvedesclist,lvedescid1);
/*
							listupdated=TRUE;
*/
						}
					}
				}
				if(lvedescid1!=NULL) Rfree(lvedescid1);
				ef=NXTNRDsec(lmsrnum,keynumber);
/*
				if(tmprsrc!=NULL)
					update_diagnostic(tmprsrc,listupdated);
*/
			}
/*
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
*/
			FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
			if((keyn=KEYNUMBER(jmstnum,"JOBMSTR KEY"))!=(-1))
			{
				keynumber=keyn;
			} else {
				keynumber=1;
			}
			ZERNRD(jmstnum);
			FINDFLDSETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",perid1);
			ef=GTENRDsec(jmstnum,keynumber);
/*
			e=stralloc("Updating Job Numbers List");
			tmprsrc=diagscrn(jmstnum,"DIAGNOSTIC SCREEN",module,e,NULL);
			if(e!=NULL) Rfree(e);
			if(tmprsrc!=NULL)
			{
				if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
					ForceWindowUpdate(tmprsrc);
			}
*/
			while(!ef)
			{
/*
				listupdated=FALSE;
*/
				FINDFLDGETSTRING(jmstnum,"PERSONNEL IDENTIFICATION",&perid2);
				if(RDAstrcmp(perid2,perid1)) break;
				FINDFLDGETINT(jmstnum,"JOB NUMBER",&jobno1);
				FINDFLDGETCHAR(jmstnum,"ACTIVE",&active);
				FINDFLDGETCHAR(jmstnum,"DELETEFLAG",&deleteflag);
				if(active && !deleteflag)
				{
					jobnostr=Rrealloc(jobnostr,9);
					sprintf(jobnostr,"%8d",jobno1);
					addAPPlib(jobnolist,jobnostr);
/*
					listupdated=TRUE;
*/
				}
				if(jobnostr!=NULL) Rfree(jobnostr);
				ef=NXTNRDsec(jmstnum,keynumber);
/*
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,listupdated);
*/
			}
/*
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
*/
		}
		if(lvedesclist->numlibs<1) addAPPlib(lvedesclist,"No Leave Descriptions Available");
		if(sellvedesc>=lvedesclist->numlibs) sellvedesc=0;
		if(!FINDRSCLISTAPPlib(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",sellvedesc,lvedesclist))
		{
			updatersrc(parent,"LEAVE DESCRIPTION IDENTIFICATIONS");
		}
		if(jobnolist->numlibs<1) addAPPlib(jobnolist,"No Jobs Available");
		if(seljobno>=jobnolist->numlibs) seljobno=0;
		if(!FINDRSCLISTAPPlib(parent,"JOB NUMBERS",seljobno,jobnolist))
		{
			updatersrc(parent,"JOB NUMBERS");
		}
		if(personnel_identification!=NULL) Rfree(personnel_identification);
		personnel_identification=stralloc(perid1);
		if(leave_description!=NULL) Rfree(leave_description);
	}
	changelve(parent,check);
	changejobno(parent,check);
	if(jobnostr!=NULL) Rfree(jobnostr)
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
}
static void changelvecb(RDArsrc *parent)
{
	changelve(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void changelve(RDArsrc *parent,short check)
{
	char *perid1=NULL,*lvedescid1=NULL;
	int sellvedesc=0;

	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
		if((RDAstrcmp(lvedesclist->libs[sellvedesc],"No Leave Descriptions Available")) &&
			(RDAstrcmp(lvedesclist->libs[sellvedesc],"Not Applicable")))
		{
			lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
		}
		FINDRSCSETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedescid1);
		updatersrc(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
		FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
		FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(leave_description!=NULL) Rfree(leave_description);
		leave_description=stralloc(lvedescid1);
		getsupportingrecords(parent,TRUE);
	} else {
		FINDRSCGETSTRING(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
		FINDFLDSETSTRING(filenum,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
		if(check==TRUE)
		{
		if(check_lveid(parent,TRUE)==FALSE)
		{
			if(perid1!=NULL) Rfree(perid1);
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			return;
		}
		}
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
}
static void changejobnocb(RDArsrc *parent)
{
	changejobno(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void changejobno(RDArsrc *parent,short check)
{
	char *perid1=NULL,*jobnostr=NULL;
	int jobno1=(-1),seljobno=0;

	if(FINDRSCGETSTRING(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(AddList==TRUE)
	{
		FINDRSCGETINT(parent,"JOB NUMBERS",&seljobno);
		if((RDAstrcmp(jobnolist->libs[seljobno],"No Jobs Available")) &&
			(RDAstrcmp(jobnolist->libs[seljobno],"Not Applicable")))
		{
			jobnostr=stralloc(jobnolist->libs[seljobno]);
		}
		jobno1=Ratoi(jobnostr);
		if(jobnostr!=NULL) Rfree(jobnostr);
		FINDRSCSETINT(parent,"[LVETRAN][JOB NUMBER]",jobno1);
		updatersrc(parent,"[LVETRAN][JOB NUMBER]");
		if(job_number!=jobno1) job_number=jobno1;
		getsupportingrecords(parent,TRUE);
	} else {
		FINDRSCGETINT(parent,"[LVETRAN][JOB NUMBER]",&jobno1);
		if(check==TRUE)
		{
		if(check_jobno(parent,TRUE)==FALSE)
		{
			if(perid1!=NULL) Rfree(perid1);
			return;
		}
		}
	}
	if(perid1!=NULL) Rfree(perid1);
}
void browse_perdmg(RDArsrc *parent)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	readallwidgets(parent);
	if(mbl_perdmg==NULL)
	{
		mainrsrc=RDArsrcNEW(module,"ADD LEAVE USED PERSONNEL BROWSE");
		searchrsrc=RDArsrcNEW(module,"ADD LEAVE USED PERSONNEL SEARCH BROWSE");
		definelist=RDArsrcNEW(module,"ADD LEAVE USED PERSONNEL DEFINE LIST");
		addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
		addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mbl_perdmg=ADVbrowse(dmgnum,1,mainrsrc,searchrsrc,definelist,
			NULL,selectfunction,NULL,okfunc,NULL,
			parent,TRUE,NULL,0,1,1,TRUE);
	} else {
		if(APPmakescrn(mbl_perdmg->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit(mbl_perdmg);
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
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	char *perid1=NULL;

	ZERNRD(dmgnum);
	if(EQLNRDKEYsec(dmgnum,1,targetkey))
	{
		KEYNRD(dmgnum,1);
	} else {
		FINDFLDGETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",&perid1);
	}
	FINDRSCSETSTRING(b->funcargs,"[LVETRAN][PERSONNEL IDENTIFICATION]",perid1);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid1);
	changeper(b->funcargs,TRUE);
	computeallSCRNvirtuals(b->funcargs);
	updateallrsrc(b->funcargs);
}
void main(int argc,char **argv)
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	int start=0;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL;

	if(InitializeSubsystems(argc,argv,module,"ADD LEAVE USED")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	GetAddList();
	if(argc>2)
	{
		audit_reportname=stralloc(argv[1]);
	} else {
		audit_reportname=stralloc("LUAUDIT");
	}
	if((filenum=APPOPNNRD(module,"LVETRAN",TRUE,TRUE))==(-1)) return;
	if((lmsrnum=APPOPNNRD(module,"LVEMSTR",TRUE,TRUE))==(-1)) return;
	if((lvejob=APPOPNNRD(module,"LVEJOB",TRUE,TRUE))==(-1)) return;
	if((jmstnum=APPOPNNRD("PAYROLL","PAYJMST",TRUE,FALSE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((ldescnum=APPOPNNRD(module,"LVEDSC",TRUE,FALSE))==(-1)) return;
	if((ltypenum=APPOPNNRD(module,"LVETPMS",TRUE,FALSE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	nokeys=NUMKEYS(filenum);
	keys=KEYPOINTER(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(AddList)
	{
		parent=RDArsrcNEW(module,"ADD LEAVE USED");
	} else {
		parent=RDArsrcNEW(module,"ADD LEAVE USED W/O LISTS");
	}
	definelist=RDArsrcNEW(module,"ADD LEAVE USED DEFINE LIST");
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"PERSONNEL IDENTIFICATION"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addstdrsrc(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"[LVETRAN][PERSONNEL IDENTIFICATION]",check_peridcb,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"JOB NUMBER"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						jobnolist=APPlibNEW();
						addlstrsrc(parent,"JOB NUMBERS",
							&start,TRUE,changejobnocb,
							jobnolist->numlibs,
							&jobnolist->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"JOB NUMBERS",FALSE);
					}
					addstdrsrc(parent,"[LVETRAN][JOB NUMBER]",LONGV,6,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"[LVETRAN][JOB NUMBER]",check_jobnocb,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"LEAVE DESCRIPTION IDENTIFICATION"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					if(AddList==TRUE)
					{
						lvedesclist=APPlibNEW();
						addlstrsrc(parent,"LEAVE DESCRIPTION IDENTIFICATIONS",
							&start,TRUE,
							changelvecb,lvedesclist->numlibs,
							&lvedesclist->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(parent,"LEAVE DESCRIPTIONS",FALSE);
					}
					addstdrsrc(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",VARIABLETEXT,0,NULL,(edit_rsrc ? TRUE:FALSE));
					FINDRSCSETFUNC(parent,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",check_lveidcb,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"STARTING DATE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addstdrsrc(parent,"[LVETRAN][STARTING DATE]",DATES,fldx->len,NULL,(edit_rsrc ? TRUE:FALSE));
					addstdrsrc(parent,"STARTING TIME",TIMES,8,NULL,(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"ENDING DATE"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addstdrsrc(parent,"[LVETRAN][ENDING DATE]",DATES,fldx->len,NULL,(edit_rsrc ? TRUE:FALSE));
					addstdrsrc(parent,"ENDING TIME",TIMES,8,NULL,(edit_rsrc ? TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"TYPE"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,filenum,fldx,edit_rsrc);
			}
		}
	}
	file2rsrc(jmstnum,parent,FALSE);
	file2rsrc(dmgnum,parent,FALSE);
	file2rsrc(ldescnum,parent,FALSE);
	file2rsrc(lmsrnum,parent,FALSE);
	file2rsrc(lvejob,parent,FALSE);
	nofields=0;
	nokeys=0;
	if(prevlve!=NULL) freeapplib(prevlve);
	prevlve=APPlibNEW();
	addAPPlib(prevlve,"No Leaves Used added this session");
	addlstrsrc(parent,"PREVIOUS LEAVES USED",&x,TRUE,NULL,prevlve->numlibs,
		&prevlve->libs,NULL);
	fldx=FLDNRD(filenum,"ENTRY DATE");
	if(fldx!=NULL)
	{
		if(fldx->len>8)
		{
		FINDRSCSETSTRING(parent,"[LVETRAN][ENTRY DATE]",CURRENT_DATE10);
		} else {
		FINDRSCSETSTRING(parent,"[LVETRAN][ENTRY DATE]",CURRENT_DATE);
		}
	} else {
		FINDRSCSETSTRING(parent,"[LVETRAN][ENTRY DATE]",CURRENT_DATE);
	}
	FINDRSCSETSTRING(parent,"[LVETRAN][ENTRY TIME]",CURRENT_TIME);
/*
	savelist(definelist,parent);
*/
	addDFincvir(parent,module,"LVETRAN",NULL,filenum);
	addDFincvir(parent,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(parent,module,"LVEDSC",NULL,ldescnum);
	addDFincvir(parent,module,"LVEMSTR",NULL,lmsrnum);
	addDFincvir(parent,module,"LVEJOB",NULL,lvejob);
	addDFincvir(parent,module,"LVETPMS",NULL,ltypenum);
	addDFincvir(parent,"PAYROLL","PAYJMST",NULL,jmstnum);
	addDFincvir(definelist,module,"LVETRAN",NULL,filenum);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	addbtnrsrc(parent,"BROWSE PERSONNEL",TRUE,browse_perdmg,NULL);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addbtnrsrc(parent,"QUIT",TRUE,quit_check,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(parent);
	APPmakescrn(parent,FALSE,quit_record_freens,NULL,TRUE);
	RDAAPPMAINLOOP();
}
