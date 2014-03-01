/* lveass1.c - Leave Management's Assign Leave*/
#include <cstdio>

#include <mix.hpp>
#include <ldval.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


extern void getsupportingrecords(RDArsrc *,short,short);
extern short empabtr,empabs,prsnnl,jobmst;
extern short lvetpms,lvedsc,lvemstr,lvetran,lvejob;
extern MakeBrowseList *mbl_empabtr;
extern int LEAVE_TRANSACTION_COUNT;
extern char *module;
extern void quit_record(RDArsrc *);
extern MaintainMaster *MTNMASTER;
void select_lves(RDArsrc *);
static RDArsrc *empabtrrsrc;
static char *personnel_identification=NULL,*leave_description=NULL;
/*
static MaintainMaster *MTNMASTER1=NULL;
static MakeBrowseList *mbl_lves;
static short test_lveid(short);
static void doexit(MakeBrowseList *);
static void browse_lves(RDArsrc *,RDArsrc *);
*/
static void save_record(RDArsrc *,RDArsrc *,short);
static void save_check(RDArsrc *,RDArsrc *,short);
static void quit_record1(RDArsrc *);
static void save_record_up(RDArsrc *,RDArsrc *);
static void save_record_nup(RDArsrc *,RDArsrc *);
static void getsupportingfiles(RDArsrc *,short);
static void lvesm(short,void *,RDArsrc *);
static void changeper(RDArsrc *);
static void changelve(RDArsrc *);
static short check_lveid(RDArsrc *);
static void check_lveidcb(RDArsrc *);
static void check_lvevalues(double,double *,double *);
static void lveselected(RDArsrc *,RDArsrc *,short);
static APPlib *lvedesclist=NULL;

static void changeper(RDArsrc *mtnrsrc)
{
	short ef=0;
	short keyn=0,keynumber=0;
	int sellvedesc=0,xl=0;
	char active=TRUE,deleteflag=FALSE;
	char *e=NULL;
	char *perid1=NULL,*perid2=NULL,*lvedescid1=NULL;
	short listupdated=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	if(RDAstrcmp(perid1,personnel_identification) || personnel_identification==NULL)
	{
		if(lvedesclist!=NULL) freeapplib(lvedesclist);
		lvedesclist=APPlibNEW();
		FINDRSCGETINT(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
 		if((keyn=KEYNUMBER(lvemstr,"LVEMSTR KEY"))!=(-1))
 		{
 			keynumber=keyn;
 		} else {
 			keynumber=1;
 		}
		ZERNRD(lvemstr);
		FINDFLDSETSTRING(lvemstr,"PERSONNEL IDENTIFICATION",perid1);
		ef=GTENRDsec(lvemstr,keynumber);
		e=stralloc("Updating Leave Descriptions List");
		tmprsrc=diagscrn(lvemstr,"DIAGNOSTIC SCREEN",module,e,NULL);
		if(e!=NULL) Rfree(e);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(tmprsrc);
		}
		while(!ef)
		{
			listupdated=FALSE;
			FINDFLDGETSTRING(lvemstr,"PERSONNEL IDENTIFICATION",&perid2);
			if(RDAstrcmp(perid2,perid1)) break;
			FINDFLDGETSTRING(lvemstr,"LEAVE DESCRIPTION IDENTIFICATION",&lvedescid1);
			FINDFLDGETCHAR(lvemstr,"ACTIVE",&active);
			FINDFLDGETCHAR(lvemstr,"DELETEFLAG",&deleteflag);
			if(active && !deleteflag)
			{
				FINDFLDSETSTRING(lvedsc,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
				if(EQLNRDsec(lvedsc,1))
				{
					KEYNRD(lvedsc,1);
				} else {
					FINDFLDGETCHAR(lvedsc,"ACTIVE",&active);
					FINDFLDGETCHAR(lvedsc,"DELETEFLAG",&deleteflag);
					if(active && !deleteflag)
					{
						addAPPlib(lvedesclist,lvedescid1);
						listupdated=TRUE;
					}
				}
			}
			if(lvedescid1!=NULL) Rfree(lvedescid1);
			ef=NXTNRDsec(lvemstr,keynumber);
			if(tmprsrc!=NULL)
				update_diagnostic(tmprsrc,listupdated);
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
		if(lvedesclist->numlibs<1) addAPPlib(lvedesclist,"No Leave Descriptions Available");
		FINDFLDGETSTRING(empabtr,"LEAVE DESCRIPTION IDENTIFICATION",&lvedescid1);
		if(lvedescid1!=NULL)
		{
			if(RDAstrlen(lvedescid1)!=0)
			{
				for(xl=0;xl<lvedesclist->numlibs;++xl)
				{
					if(strncmp(lvedescid1,lvedesclist->libs[xl],RDAstrlen(lvedescid1))<=0)
					{
						break;
					}
				}
				if(xl>=lvedesclist->numlibs) xl=(lvedesclist->numlibs-1);
				sellvedesc=xl;
			}
		}
		if(sellvedesc>=lvedesclist->numlibs) sellvedesc=0;
		if(!FINDRSCLISTAPPlib(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",sellvedesc,lvedesclist))
		{
			updatersrc(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS");
		}
		if(personnel_identification!=NULL) Rfree(personnel_identification);
		personnel_identification=stralloc(perid1);
		if(leave_description!=NULL) Rfree(leave_description);
	}
	changelve(mtnrsrc);
	if(perid1!=NULL) Rfree(perid1);
	if(perid2!=NULL) Rfree(perid2);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
}
static check_lveunits(RDArsrc *mtnrsrc) 
{
	char *perid=NULL,*lveid=NULL,*warnmessage=NULL,allownegs=FALSE;
	double lve_units=0.0,lve_units_unass=0.0,available_units=0.0;
	double docked_units;

	FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lveid);
	FINDRSCGETDOUBLE(mtnrsrc,"[LVETRAN][AMOUNT]",&lve_units);
	FINDFLDGETDOUBLE(empabtr,"LEAVE UNITS UNASSIGNED",&lve_units_unass);
	FINDFLDGETCHAR(lvedsc,"ALLOW NEG BALANCES",&allownegs);
	if(lve_units>lve_units_unass)
	{
		warnmessage=Rmalloc(230);
		sprintf(warnmessage,
			"The Leave Units of [%f] is greater than total Leave Units Unassigned of [%f]. Please select a valid number of Leave Units to save this record.",lve_units,lve_units_unass);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID NUMBER OF UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid!=NULL) Rfree(perid);
		if(lveid!=NULL) Rfree(lveid);
		return(FALSE);
	} else if(lve_units<=0.0)
	{
		warnmessage=Rmalloc(230);
		sprintf(warnmessage,
			"The Leave Units of [%f] is an invalid number of units. Please select a valid number of Leave Units to save this record.",lve_units);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID NUMBER OF UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid!=NULL) Rfree(perid);
		if(lveid!=NULL) Rfree(lveid);
		return(FALSE);
	} else {
		check_lvevalues(lve_units,&available_units,&docked_units);
		if(available_units<0.0 && !allownegs)
		{
			warnmessage=Rmalloc(RDAstrlen(perid)+RDAstrlen(lveid)+20+290);
			sprintf(warnmessage,
				"The Leave Units of [%f] is greater than the number of units earned of [%f] for the Personnel Identification of [%s] for the Leave Description Identification of [%s].  Please select a valid number of Leave Units to save this record.",
				lve_units,(available_units+lve_units),(perid==NULL?"":perid),(lveid==NULL?"":lveid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID NUMBER OF UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(perid!=NULL) Rfree(perid);
			if(lveid!=NULL) Rfree(lveid);
			return(FALSE);
		}
	}
	if(perid!=NULL) Rfree(perid);
	if(lveid!=NULL) Rfree(lveid);
	return(TRUE);
}
static short check_lveid(RDArsrc *mtnrsrc)
{
	char *perid1=NULL,*lveid1=NULL,*lvetype=NULL,*warnmessage=NULL;
	char delflag=FALSE,allowneg=FALSE;
	int jobno=0;
	double avl=0.0,docked=0.0,lve_units=0.0;

	FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
	FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lveid1);
	FINDRSCGETDOUBLE(mtnrsrc,"[LVETRAN][AMOUNT]",&lve_units);
	FINDRSCGETINT(mtnrsrc,"[LVETRAN][JOB NUMBER]",&jobno);
	ZERNRD(lvejob);
	FINDFLDSETSTRING(lvejob,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(lvejob,"LEAVE DESCRIPTION IDENTIFICATION",lveid1);
	FINDFLDSETINT(lvejob,"JOB NUMBER",jobno);
	if(EQLNRDsec(lvejob,1))
	{
		KEYNRD(lvejob,1);
		warnmessage=Rmalloc(RDAstrlen(lveid1)+RDAstrlen(perid1)+255);
		sprintf(warnmessage,
			"The Leave Description Identification of [%s] for the Personnel Identification of [%s] is an invalid value.  It does not exist in the [LVEMGT][LVEJOB] date file.  Please select a valid Leave Description Identification to save this record.",
			(lveid1==NULL?"":lveid1),(perid1==NULL?"":perid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid1!=NULL) Rfree(perid1);
		if(lveid1!=NULL) Rfree(lveid1);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(lvejob,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(lveid1)+RDAstrlen(perid1)+255);
			sprintf(warnmessage,
				"The Leave Description Identification of [%s] for the Personnel Identification of [%s] is a deleted value in the file [LVEMGT][LVEJOB] date file.  Please select a valid Leave Description Identification to save this record.",
				(lveid1==NULL?"":lveid1),(perid1==NULL?"":perid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(perid1!=NULL) Rfree(perid1);
			if(lveid1!=NULL) Rfree(lveid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(lvejob,"ACTIVE",&delflag);
			if(!delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(lveid1)+RDAstrlen(perid1)+215);
				sprintf(warnmessage,
					"The Leave Description Identification of [%s] for the Personnel Identification of [%s] is not Active in the file [LVEMGT][LVEJOB] date file.  Please select a valid Leave Description Identification to save this record.",
					(lveid1==NULL?"":lveid1),(perid1==NULL?"":perid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(perid1!=NULL) Rfree(perid1);
				if(lveid1!=NULL) Rfree(lveid1);
				return(FALSE);
			}
		}
	}
	ZERNRD(lvedsc);
	FINDFLDSETSTRING(lvedsc,"LEAVE DESCRIPTION IDENTIFICATION",lveid1);
	if(EQLNRDsec(lvedsc,1))
	{
		KEYNRD(lvedsc,1);
		warnmessage=Rmalloc(RDAstrlen(lveid1)+255);
		sprintf(warnmessage,
			"The Leave Description Identification of [%s] is an invalid value.  It does not exist in the [LVEMGT][LVEDSC] date file.  Please select a valid Leave Description Identification to save this record.",
			(lveid1==NULL?"":lveid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid1!=NULL) Rfree(perid1);
		if(lveid1!=NULL) Rfree(lveid1);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(lvedsc,"DELETEFLAG",&delflag);
		FINDFLDGETCHAR(lvedsc,"ALLOW NEG BALANCES",&allowneg);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(lveid1)+255);
			sprintf(warnmessage,
				"The Leave Description Identification of [%s] is a deleted value in the file [LVEMGT][LVEDSC] date file.  Please select a valid Leave Description Identification to save this record.",
				(lveid1==NULL?"":lveid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(perid1!=NULL) Rfree(perid1);
			if(lveid1!=NULL) Rfree(lveid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(lvedsc,"ACTIVE",&delflag);
			if(!delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(lveid1)+215);
				sprintf(warnmessage,
					"The Leave Description Identification of [%s] is not Active in the file [LVEMGT][LVEDSC] date file.  Please select a valid Leave Description Identification to save this record.",
					(lveid1==NULL?"":lveid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE LEAVE DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(perid1!=NULL) Rfree(perid1);
				if(lveid1!=NULL) Rfree(lveid1);
				return(FALSE);
			}
		}
	}
	FINDFLDGETSTRING(lvedsc,"LEAVE TYPE IDENTIFICATION",&lvetype);
	ZERNRD(lvetpms);
	FINDFLDSETSTRING(lvetpms,"LEAVE TYPE IDENTIFICATION",lvetype);
	if(EQLNRDsec(lvetpms,1))
	{
		KEYNRD(lvetpms,1);
		warnmessage=Rmalloc(RDAstrlen(lvetype)+RDAstrlen(lveid1)+290);
		sprintf(warnmessage,
			"The Leave Type Identification of [%s] in the [LVEMGT][LVEDSC] for the Leave Description Identification of [%s] is an invalid value.  It does not exist in the [LVEMGT][LVETPMS] date file.  Please select a valid Leave Description Identification to save this record.",
			(lvetype==NULL?"":lvetype),(lveid1==NULL?"":lveid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LEAVE TYPE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid1!=NULL) Rfree(perid1);
		if(lveid1!=NULL) Rfree(lveid1);
		if(lvetype!=NULL) Rfree(lvetype);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(lvetpms,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(lvetype)+RDAstrlen(lveid1)+290);
			sprintf(warnmessage,
				"The Leave Type Identification of [%s] in the [LVEMGT][LVEDSC] for the Leave Description Identification of [%s] is a deleted value.  It exist in the [LVEMGT][LVETPMS] date file but is marked deleted.  Please select a valid Leave Description Identification to save this record.",
				(lvetype==NULL?"":lvetype),(lveid1==NULL?"":lveid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LEAVE TYPE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(perid1!=NULL) Rfree(perid1);
			if(lveid1!=NULL) Rfree(lveid1);
			if(lvetype!=NULL) Rfree(lvetype);
			return(FALSE);
		}
	}
	if(lvetype!=NULL) Rfree(lvetype);
	check_lvevalues(lve_units,&avl,&docked);
	if(avl<0.0 && !allowneg) 
	{
		warnmessage=Rmalloc(RDAstrlen(perid1)+RDAstrlen(lveid1)+20+290);
		sprintf(warnmessage,
			"The Leave Units of [%f] is greater than the number of units earned of [%f] for the Personnel Identification of [%s] for the Leave Description Identification of [%s].  Please select a valid number of Leave Units to save this record.",
			lve_units,(avl+lve_units),(perid1==NULL?"":perid1),(lveid1==NULL?"":lveid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID NUMBER OF UNITS!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return(FALSE);
	}
	if(perid1!=NULL) Rfree(perid1);
	if(lveid1!=NULL) Rfree(lveid1);
	return(TRUE);
}
static void check_lveidcb(RDArsrc *mtnrsrc)
{
	char *perid1=NULL,*lvedescid1=NULL;
	int x=0;

	readwidget(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	readwidget(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",&lvedescid1);
	FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1);
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
	FINDRSCSETINT(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",x);
	FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedesclist->libs[x]);
	updatersrc(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS");
	updatersrc(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	FINDFLDSETSTRING(lvetran,"LEAVE DESCRIPTION IDENTIFICATION",lvedesclist->libs[x]);
	FINDFLDSETSTRING(lvetran,"PERSONNEL IDENTIFICATION",perid1);
	getsupportingfiles(mtnrsrc,TRUE);
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	updateSCRNvirtuals(mtnrsrc);
}
static void changelvecb(RDArsrc *mtnrsrc)
{
	changelve(mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void changelve(RDArsrc *mtnrsrc)
{
	char *perid1=NULL,*lvedescid1=NULL;
	int sellvedesc=0;

	if(FINDRSCGETSTRING(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]",&perid1)) return;
	FINDRSCGETINT(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
	if((RDAstrcmp(lvedesclist->libs[sellvedesc],"No Leave Descriptions Available")) &&
		(RDAstrcmp(lvedesclist->libs[sellvedesc],"Not Applicable")))
	{
		lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
	}
	FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedescid1);
	updatersrc(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]");
	FINDFLDSETSTRING(lvetran,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETSTRING(lvetran,"LEAVE DESCRIPTION IDENTIFICATION",lvedescid1);
	if(leave_description!=NULL) Rfree(leave_description);
	leave_description=stralloc(lvedescid1);
	getsupportingfiles(mtnrsrc,TRUE);
	if(perid1!=NULL) Rfree(perid1);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
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
/*
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	RDArsrc *empabtrrsrc=NULL;

	empatrrsrc=(RDArsrc *)blist->funcargs;
	lvesm(0,targetkey,empabtrrsrc);
}
static void doexit(MakeBrowseList *blist)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	if(blist!=NULL)
	{
		brsrc=blist->mainrsrc;
		srsrc=blist->searchrsrc;
		drsrc=blist->definelist;
		FREE_BROWSE_EXIT(blist);
		if(drsrc!=NULL) 
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
		}
		if(srsrc!=NULL) 
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			free_rsrc(brsrc);
		}
		if(mbl_lves!=NULL) mbl_lves=NULL;
		if(MTNMASTER1!=NULL) FreeMaintainMaster(MTNMASTER1);
	}
}
static short test_lveid(short fileno)
{
	char *lveid=NULL,*lvetype=NULL,deleteflag=TRUE,active=FALSE;
	double avl=0.0,dockedunits=0.0;

	FINDFLDGETCHAR(fileno,"DELETEFLAG",&deleteflag);
	FINDFLDGETCHAR(fileno,"ACTIVE",&active);
	if(!deleted && active)
	{
		FINDFLDGETSTRING(fileno,"LEAVE DESCRIPTION IDENTIFICATION",&lveid);
		ZERNRD(lvedsc);
		FINDFLDSETSTRING(lvedsc,"LEAVE DESCRIPTION IDENTIFICATION",lveid);
		if(!EQLNRDsec(lvedsc))
		{
			FINDFLDGETCHAR(lvedsc,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(lvedsc,"ACTIVE",&active);
			if(!deleted && active)
			{
				FINDFLDGETSTRING(lvedsc,"LEAVE TYPE IDENTIFICATION",&lveid);
				ZERNRD(lvetpms);
				FINDFLDSETSTRING(lvetpms,"LEAVE DESCRIPTION IDENTIFICATION",lveid);
				if(!EQLNRDsec(lvetpms))
				{
					FINDFLDGETCHAR(lvetpms,"DELETEFLAG",&deleteflag);
					if(!deleted)
					{
						FINDFLDGETCHAR(lvedsc,"ALLOW NEG BALANCES",&active);
						if(!active)
						{
							if(check_lvevalues(0.0,&avl,&dockedunits)>0.0)
							{
								if(lveid!=NULL) Rfree(lveid);
								if(lvetype!=NULL) Rfree(lvetype);
								return(TRUE);
							}
						}
					}
				}
			}
		}
	}
	if(lveid!=NULL) Rfree(lveid);
	if(lvetype!=NULL) Rfree(lvetype);
	return(FALSE);
}
static short selectfunction(MakeBrowseList *b)
{
	return(test_lveid(b->fileno));
}
*/
void select_lves(RDArsrc *eabsmtnrsrc)
{
	empabtrrsrc=eabsmtnrsrc;
/*
	MTNMASTER1=MaintainMasterNew(module,"ASSIGN LEAVES");
	if(findMaintainMaster(MTNMASTER1))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER1 NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","ASSIGN LEAVES",module);
		doexit(NULL);
	} else {
		MTNMASTER1->passkey=READPassKey(empabtr);
		switch(MTNMASTER1->start_with)
		{
			default:
			case 0:
				lvesm(1,NULL,empabtrrsrc);
				break;
			case 1:
				browse_lves(NULL,empabtrrsrc);
				break;
		}
	}
*/
	lvesm(1,NULL,empabtrrsrc);
}
/*
static void browse_lves(RDArsrc *mtnrsrc,RDArsrc *empabtrrsrc)
{
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	char *empid=NULL;
	int emptransno=0,empjobno=0;
	double lve_units_unass=0.0;

	if(mtnrsrc!=NULL) Rfree(mtnrsrc);
	brsrc=RDArsrcNEW(module,MTNMASTER1->browse_screen);
	srsrc=RDArsrcNEW(module,MTNMASTER1->search_browse);
	drsrc=RDArsrcNEW(module,MTNMASTER1->define_list);
	addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDGETINT(empabtr,"TRANSACTION NUMBER",&emptransno);
	FINDFLDGETINT(empabtr,"JOB NUMBER",&empjobno);
	FINDFLDGETDOUBLE(empabtr,"LEAVE UNITS UNASSIGNED",&lve_units_unass);
	addstdrsrc(brsrc,"EMPLOYEE IDENTIFICATION",VARIABLETEXT,0,empid,FALSE);
	addstdrsrc(brsrc,"EMPLOYEE TRANSACTION NUMBER",LONGV,6,&emptransno,FALSE);
	addstdrsrc(brsrc,"EMPLOYEE JOB NUMBER",LONGV,6,&empjobno,FALSE);
	addstdrsrc(brsrc,"LEAVE UNITS UNASSIGNED",SDECIMALV,8,&lve_units_unass,FALSE);
	if(empid!=NULL) Rfree(empid);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	ZERNRD(lvemstr);
	mbl_lves=ADVSbrowse(lvemstr,1,brsrc,srsrc,drsrc,NULL,NULL,
		selectfunction,doexit,okfunc,NULL,empabtrrsrc,TRUE,NULL,
		0,0,0,TRUE);
}
*/
void lvesm(short dowhich,void *targetkey,RDArsrc *empabtrrsrc)
{
	RDArsrc *mtnrsrc=NULL;
	int which_type=0,jobno=0;
	double lve_units=0.0;
	short nofields=0,x=0,edit_rsrc=FALSE;
	char *empid=NULL;
	char *startdate=NULL,*enddate=NULL,*desc=NULL;
	NRDfield *fields=NULL,*fldx=NULL;

	mtnrsrc=RDArsrcNEW(module,"ASSIGN LEAVE MAINTAIN SCREEN");
	nofields=NUMFLDS(lvetran);
	fields=FLDPOINTER(lvetran);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"PERSONNEL IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(lvetran,fldx->name,&edit_rsrc))
				{
					nonlstmakefld(mtnrsrc,lvetran,fldx,(edit_rsrc==TRUE?TRUE:FALSE));
				}
			} else if(!RDAstrcmp(fldx->name,"LEAVE DESCRIPTION IDENTIFICATION"))
			{
				if(lvedesclist!=NULL) freeapplib(lvedesclist);
				lvedesclist=APPlibNEW();
				edit_rsrc=TRUE;
				if(FIELDscrn(lvetran,fldx->name,&edit_rsrc))
				{
					which_type=0;
					lvedesclist=APPlibNEW();
					addlstrsrc(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						changelvecb,
						lvedesclist->numlibs,
						&lvedesclist->libs,NULL);
					nonlstmakefld(mtnrsrc,lvetran,fldx,(edit_rsrc==TRUE?TRUE:FALSE));
					FINDRSCSETFUNC(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",check_lveidcb,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"TRANSACTION NUMBER"))
			{
			} else if(!RDAstrcmp(fldx->name,"ENTRY DATE"))
			{
			} else if(!RDAstrcmp(fldx->name,"ENTRY TIME"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"UPDATED DATE"))
			{
			} else if(!RDAstrcmp(fldx->name,"UPDATED TIME"))
			{
			} else if(!RDAstrcmp(fldx->name,"UPDATED BY"))
			{
			} else if(!RDAstrcmp(fldx->name,"UPDATED"))
			{
			} else {
				edit_rsrc=TRUE;
				if(FIELDscrn(lvetran,fldx->name,&edit_rsrc))
				{
					nonlstmakefld(mtnrsrc,lvetran,fldx,(edit_rsrc==TRUE?TRUE:FALSE));
				}
			}
		}
	}
	nofields=0;
	if(lvemstr!=(-1)) file2rsrc(lvemstr,mtnrsrc,FALSE);
	if(lvejob!=(-1)) file2rsrc(lvejob,mtnrsrc,FALSE);
	if(lvedsc!=(-1)) file2rsrc(lvedsc,mtnrsrc,FALSE);
	if(lvetpms!=(-1)) file2rsrc(lvetpms,mtnrsrc,FALSE);
	if(empabtr!=(-1)) file2rsrc(empabtr,mtnrsrc,FALSE);
	if(empabs!=(-1)) file2rsrc(empabs,mtnrsrc,FALSE);
	if(prsnnl!=(-1)) file2rsrc(prsnnl,mtnrsrc,FALSE);
	if(jobmst!=(-1)) file2rsrc(jobmst,mtnrsrc,FALSE);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mtnrsrc,"PAYROLL","PAYJMST",NULL,jobmst);
	addDFincvir(mtnrsrc,module,"LVEMSTR",NULL,lvemstr);
	addDFincvir(mtnrsrc,module,"LVEJOB",NULL,lvejob);
	addDFincvir(mtnrsrc,module,"LVEDSC",NULL,lvedsc);
	addDFincvir(mtnrsrc,"EMPABS","EMPABS",NULL,empabs);
	addDFincvir(mtnrsrc,"EMPABS","EMPABTR",NULL,empabtr);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* Defaulting from the merge of screendefaults &
				empabtr record*/
			FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&empid);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]",empid);
			FINDFLDGETDOUBLE(empabtr,"LEAVE UNITS UNASSIGNED",&lve_units);
			FINDRSCSETDOUBLE(mtnrsrc,"[LVETRAN][AMOUNT]",lve_units);
			FINDFLDGETSTRING(empabs,"STARTING DATE",&startdate);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][STARTING DATE]",startdate);
			FINDFLDGETSTRING(empabs,"ENDING DATE",&enddate);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][ENDING DATE]",enddate);
			FINDFLDGETSTRING(empabs,"DESCRIPTION",&desc);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][DESCRIPTION]",desc);
			FINDFLDGETINT(empabtr,"JOB NUMBER",&jobno);
			FINDRSCSETINT(mtnrsrc,"[LVETRAN][JOB NUMBER]",jobno);
			COPYFIELD(empabtr,lvetran,"LEAVE DESCRIPTION IDENTIFICATION");
			if(startdate!=NULL) Rfree(startdate);
			if(enddate!=NULL) Rfree(enddate);
			if(desc!=NULL) Rfree(desc);
			if(empid!=NULL) Rfree(empid);
			rsrc2filerecord(lvetran,mtnrsrc);
			changeper(mtnrsrc);
			break;
		case 2:
			FINDFLDGETSTRING(empabtr,"PERSONNEL IDENTIFICATION",&empid);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][PERSONNEL IDENTIFICATION]",empid);
			FINDFLDGETDOUBLE(empabtr,"LEAVE UNITS UNASSIGNED",&lve_units);
			FINDRSCSETDOUBLE(mtnrsrc,"[LVETRAN][AMOUNT]",lve_units);
			FINDFLDGETSTRING(empabs,"STARTING DATE",&startdate);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][STARTING DATE]",startdate);
			FINDFLDGETSTRING(empabs,"ENDING DATE",&enddate);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][ENDING DATE]",enddate);
			FINDFLDGETSTRING(empabs,"DESCRIPTION",&desc);
			FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][DESCRIPTION]",desc);
			FINDFLDGETINT(empabtr,"JOB NUMBER",&jobno);
			FINDRSCSETINT(mtnrsrc,"[LVETRAN][JOB NUMBER]",jobno);
			COPYFIELD(empabtr,lvetran,"LEAVE DESCRIPTION IDENTIFICATION");
			if(startdate!=NULL) Rfree(startdate);
			if(enddate!=NULL) Rfree(enddate);
			if(desc!=NULL) Rfree(desc);
			if(empid!=NULL) Rfree(empid);
			rsrc2filerecord(lvetran,mtnrsrc);
			changeper(mtnrsrc);
			break;
		default:
		case 0:
			COPYFIELD(empabtr,lvetran,"LEAVE DESCRIPTION IDENTIFICATION");
			if(EQLNRDKEYsec(lvetran,1,targetkey)) 
			{
				KEYNRD(lvetran,1);
				filerecord2rsrc(lvetran,mtnrsrc);
				changeper(mtnrsrc);
			} else {
				filerecord2rsrc(lvetran,mtnrsrc);
				changeper(mtnrsrc);
			}
			break;
	}
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(empabtr!=(-1)) filerecord2rsrc(empabtr,mtnrsrc);
	if(empabs!=(-1)) filerecord2rsrc(empabs,mtnrsrc);
	if(prsnnl!=(-1)) filerecord2rsrc(prsnnl,mtnrsrc);
	if(jobmst!=(-1)) filerecord2rsrc(jobmst,mtnrsrc);
	getsupportingfiles(mtnrsrc,FALSE);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,empabtrrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,empabtrrsrc);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record1,NULL);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mtnrsrc,TRUE,quit_record1,NULL,FALSE);
}
static void lveselected(RDArsrc *mtnrsrc,RDArsrc *empabtrrsrc,short update_list)
{
	char *curdate=NULL,*curtime=NULL;
	double lve_units_unass=0.0,lve_units=0.0;
	void *targetkey=NULL;
	RDATData *prev_empabtr=NULL;

	curdate=GETCURRENTDATE();
	curtime=GETCURRENTTIME();
	FINDRSCGETDOUBLE(mtnrsrc,"[LVETRAN][AMOUNT]",&lve_units);
	FINDFLDGETDOUBLE(empabtr,"LEAVE UNITS UNASSIGNED",&lve_units_unass);
	LOCNRDFILE(empabtr);
	if(!EQLNRDsec(empabtr,1))
	{
		prev_empabtr=RDATDataNEW(empabtr);
		FINDFLDSETDOUBLE(empabtr,"LEAVE UNITS UNASSIGNED",(lve_units_unass-lve_units));
		if((lve_units_unass-lve_units)<=0.0)
		{
			FINDFLDSETCHAR(empabtr,"LEAVE FINALED",TRUE);
			FINDFLDSETSTRING(empabtr,"LEAVE FINALED DATE",curdate);
			FINDFLDSETSTRING(empabtr,"LEAVE FINALED TIME",curtime);
			FINDFLDSETSTRING(empabtr,"LEAVE FINALED BY",USERLOGIN);
		}
		if(!WRTTRANSsec(empabtr,0,NULL,prev_empabtr))
		{
			if(curdate!=NULL) Rfree(curdate);
			if(curtime!=NULL) Rfree(curtime);
			if(empabtrrsrc!=NULL)
			{
				if(mbl_empabtr!=NULL)
				{
					if(!RDAstrcmp(empabtrrsrc->screen,MTNMASTER->maintain_screen))
					{
						updatefilerecord2rsrc(empabtr,empabtrrsrc);
						getsupportingrecords(empabtrrsrc,TRUE,TRUE);
						if((lve_units_unass-lve_units)<=0.0)
						{
							quit_record(empabtrrsrc);
						} else {
       							updateSCRNvirtuals(empabtrrsrc);
						}
					} else {
       						updateSCRNvirtuals(empabtrrsrc);
					}
					updatebrowse(update_list,mbl_empabtr,empabtr,module,empabtrrsrc);
					targetkey=(void *)mbl_empabtr->list->keyvalue;
					EQLNRDKEYsec(empabtr,1,targetkey);
				} else {
					updatefilerecord2rsrc(empabtr,empabtrrsrc);
					getsupportingrecords(empabtrrsrc,TRUE,TRUE);
       					updateSCRNvirtuals(empabtrrsrc);
				}
			}
		}
	}
	UNLNRDFILE(empabtr);
	if(prev_empabtr!=NULL) FreeRDATData(prev_empabtr);
}
static void save_record_up(RDArsrc *mtnrsrc,RDArsrc *empabtrrsrc)
{
	save_check(mtnrsrc,empabtrrsrc,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDArsrc *empabtrrsrc)
{
	save_check(mtnrsrc,empabtrrsrc,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,RDArsrc *empabtrrsrc,short update_list)
{
	int sellvedesc=0;
	char *lvedescid1=NULL;

	readfilewidgets(lvetran,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"LEAVE DESCRIPTION IDENTIFICATIONS",&sellvedesc);
	if((RDAstrcmp(lvedesclist->libs[sellvedesc],"No Leave Descriptions Available")) &&
	(RDAstrcmp(lvedesclist->libs[sellvedesc],"Not Applicable")))
	{
		lvedescid1=stralloc(lvedesclist->libs[sellvedesc]);
	}
	FINDRSCSETSTRING(mtnrsrc,"[LVETRAN][LEAVE DESCRIPTION IDENTIFICATION]",lvedescid1);
	rsrc2filerecord(lvetran,mtnrsrc);
	if(lvedescid1!=NULL) Rfree(lvedescid1);
	if(check_lveid(mtnrsrc)==FALSE) return;
	if(check_lveunits(mtnrsrc)==FALSE) return;
	save_record(mtnrsrc,empabtrrsrc,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDArsrc *empabtrrsrc,short update_list)
{
	int jobno=0;
	double units=0.0,initunits=0.0;
	RDATData *prev_subfile=NULL;
	char *perid=NULL,*lveid=NULL,*curdate=NULL,*curtime=NULL;
	double lve_units=0.0,avl=0.0,docked_units=0.0;
	char *rcddesc=NULL,*ud=NULL;
	short src=8;

	if(RECORDsec(lvetran))
	{
		rcddesc=RECORDsecDesc(lvetran);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			prterr("Security Access Deined [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Didn't write record.");
		}
	} else {
		FINDFLDSETSHORT(lvetran,"TYPE",1);
		curdate=GETCURRENTDATE();
		curtime=GETCURRENTTIME();
		FINDFLDSETSTRING(lvetran,"ENTRY DATE",curdate);
		FINDFLDSETSTRING(lvetran,"ENTRY TIME",curtime);
		if(curtime!=NULL) Rfree(curtime);
		if(curdate!=NULL) Rfree(curdate);
		FINDFLDSETSTRING(lvetran,"SOURCE USER",USERLOGIN);
		FINDFLDGETDOUBLE(lvetran,"AMOUNT",&lve_units);
		ZERNRD(lvejob);
		COPYFIELD(lvetran,lvejob,"PERSONNEL IDENTIFICATION");
		COPYFIELD(lvetran,lvejob,"LEAVE DESCRIPTION IDENTIFICATION");
		COPYFIELD(lvetran,lvejob,"JOB NUMBER");
		if(EQLNRDsec(lvejob,1)) KEYNRD(lvejob,1);
		check_lvevalues(lve_units,&avl,&docked_units);
		FINDFLDSETDOUBLE(lvetran,"DOCK UNITS",docked_units);
		FINDFLDSETSHORT(lvetran,"SOURCE",src);
		ud=Users_Department();
		FINDFLDSETSTRING(lvetran,"USER DEPARTMENT",ud);
		if(ud!=NULL) Rfree(ud);
		if(!WRTTRANS(lvetran,1,"TRANSACTION NUMBER",NULL))
		{
			++LEAVE_TRANSACTION_COUNT;
			FINDFLDGETSTRING(lvetran,"PERSONNEL IDENTIFICATION",&perid);
			FINDFLDGETSTRING(lvetran,"LEAVE DESCRIPTION IDENTIFICATION",&lveid);
			FINDFLDGETINT(lvetran,"JOB NUMBER",&jobno);
			FINDFLDGETDOUBLE(lvetran,"AMOUNT",&units);
			ZERNRD(lvemstr);
			FINDFLDSETSTRING(lvemstr,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETSTRING(lvemstr,"LEAVE DESCRIPTION IDENTIFICATION",lveid);
			LOCNRDFILE(lvemstr);
			if(!EQLNRDsec(lvemstr,1))
			{
				prev_subfile=RDATDataNEW(lvemstr);
				FINDFLDGETDOUBLE(lvemstr,"UNPROCESSED USED",&initunits);
				initunits+=units;
				FINDFLDSETDOUBLE(lvemstr,"UNPROCESSED USED",initunits);
				if(!WRTTRANSsec(lvemstr,0,NULL,prev_subfile))
				{
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to write to the [LVEMGT][LVEMSTR] date file.  No changes to this file have been saved.",NULL,NULL,TRUE,NULL);
				}
				if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
			}
			UNLNRDFILE(lvemstr);
			ZERNRD(lvejob);
			FINDFLDSETSTRING(lvejob,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETSTRING(lvejob,"LEAVE DESCRIPTION IDENTIFICATION",lveid);
			FINDFLDSETINT(lvejob,"JOB NUMBER",jobno);
			LOCNRDFILE(lvejob);
			if(!EQLNRDsec(lvejob,1))
			{
				prev_subfile=RDATDataNEW(lvejob);
				FINDFLDGETDOUBLE(lvejob,"UNPROCESSED USED",&initunits);
				initunits+=units;
				FINDFLDSETDOUBLE(lvejob,"UNPROCESSED USED",initunits);
				if(!WRTTRANSsec(lvejob,0,NULL,prev_subfile))
				{
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to write to the [LVEMGT][LVEJOB] date file.  No changes to this file have been saved.",NULL,NULL,TRUE,NULL);
				}
				if(prev_subfile!=NULL) FreeRDATData(prev_subfile);
			}
			UNLNRDFILE(lvejob);
			lveselected(mtnrsrc,empabtrrsrc,update_list);
			quit_record1(mtnrsrc);
		} else {
			WARNINGDIALOG("WARNING DIALOG SCREEN","WRITE ERROR OCCURRED!","The write function failed when attempting to write to the file [LVEMGT][LVETRAN].  no changes have been written.",NULL,NULL,TRUE,NULL);
		}
	}
	if(perid!=NULL) Rfree(perid);
	if(lveid!=NULL) Rfree(lveid);
}
static void quit_record1(RDArsrc *mtnrsrc)
{
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(lvedesclist!=NULL) freeapplib(lvedesclist);
	if(leave_description!=NULL) Rfree(leave_description);
	if(personnel_identification!=NULL) Rfree(personnel_identification);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,short update)
{
	COPYFIELD(lvetran,lvemstr,"PERSONNEL IDENTIFICATION");
	COPYFIELD(lvetran,lvemstr,"LEAVE DESCRIPTION IDENTIFICATION");
	if(EQLNRDsec(lvemstr,1)) KEYNRD(lvemstr,1);
	if(update) updatefilerecord2rsrc(lvemstr,mtnrsrc);
	else filerecord2rsrc(lvemstr,mtnrsrc);

	COPYFIELD(lvetran,lvejob,"PERSONNEL IDENTIFICATION");
	COPYFIELD(lvetran,lvejob,"LEAVE DESCRIPTION IDENTIFICATION");
	COPYFIELD(lvetran,lvejob,"JOB NUMBER");
	if(EQLNRDsec(lvejob,1)) KEYNRD(lvejob,1);
	if(update) updatefilerecord2rsrc(lvejob,mtnrsrc);
	else filerecord2rsrc(lvejob,mtnrsrc);

	COPYFIELD(lvetran,lvedsc,"LEAVE DESCRIPTION IDENTIFICATION");
	if(EQLNRDsec(lvedsc,1)) KEYNRD(lvedsc,1);
	if(update) updatefilerecord2rsrc(lvedsc,mtnrsrc);
	else filerecord2rsrc(lvedsc,mtnrsrc);

	COPYFIELD(lvedsc,lvetpms,"LEAVE TYPE IDENTIFICATION");
	if(EQLNRDsec(lvetpms,1)) KEYNRD(lvetpms,1);
	if(update) updatefilerecord2rsrc(lvetpms,mtnrsrc);
	else filerecord2rsrc(lvetpms,mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
}
