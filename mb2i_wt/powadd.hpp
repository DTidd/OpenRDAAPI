/******************************************************************************
				POWER ADD FUNCTIONS HEADER
					powadd.h
******************************************************************************/
#ifndef POWADD_H
#define POWADD_H
#include<rptgen.hpp>
#include<mix.hpp>

struct ADDPassableStructs
{
        RDArsrc *r;
        void *a;
        void (*quitfunc)(...);
        RDArsrc *fromrsrc;
        void *fromarg;
        PowerAdd *POWERADD;
        void *arg;
};
typedef struct ADDPassableStructs ADDPassableStruct;

#define  FINDEDITWDGT(a,b)	xFINDEDITWDGT(a,b,__LINE__,__FILE__)
AddEditWdgt *xFINDEDITWDGT(PowerAdd *,char *,int,char *);
#define PowerAddNew(a,b)	xPowerAddNew(a,b,__LINE__,__FILE__)
PowerAdd *xPowerAddNew(char *module,char *name,int line,char *file);
#define FreePowerAdd(a)	{xFreePowerAdd(a,__LINE__,__FILE__);a=NULL;}
void xFreePowerAdd(PowerAdd *,int,char *);
#define getPowerAdd(a,b)	xgetPowerAdd(a,b,__LINE__,__FILE__)
short xgetPowerAdd(char *,PowerAdd *,int,char *);
#define writePowerAdd(a,b)	xwritePowerAdd(a,b,__LINE__,__FILE__)
short xwritePowerAdd(char *,PowerAdd *,int,char *);
#define getPowerAdddesc(a,b)	xgetPowerAdddesc(a,b,__LINE__,__FILE__)
char *xgetPowerAdddesc(char *,char *,int,char *);
#define findPowerAdd(a)	xfindPowerAdd(a,__LINE__,__FILE__)
short xfindPowerAdd(PowerAdd *,int,char *);
#define MakePowerAddButtons(a,b)	xMakePowerAddButtons(a,b,NULL,__LINE__,__FILE__)
#define MakePowerAddButtonsADV(a,b,c)	xMakePowerAddButtons(a,b,c,__LINE__,__FILE__)
void xMakePowerAddButtons(RDArsrc *,PowerAdd *,void (*)(...),int,char *);
void PowerAddButtonFunction(RDArsrc *,MaintainButton *);
#define OPEN_POWERADD_FILES(a)	xOPEN_POWERADD_FILES(a,__LINE__,__FILE__)
short xOPEN_POWERADD_FILES(PowerAdd *,int,char *);
#define CLOSE_POWERADD_FILES(a)	xCLOSE_POWERADD_FILES(a,__LINE__,__FILE__)
void xCLOSE_POWERADD_FILES(PowerAdd *,int,char *);
#define GET_POWERADD_INCVARS(a,b)	xGET_POWERADD_INCVARS(a,b,__LINE__,__FILE__)
void xGET_POWERADD_INCVARS(RDArsrc *,PowerAdd *,int,char *);
#define SAVE_POWERADD_CHECK(a,b,c,d)	xSAVE_POWERADD_CHECK(a,b,c,d,__LINE__,__FILE__)
char xSAVE_POWERADD_CHECK(RDArsrc *,PowerAdd *,void (*)(...),void *,int,char *);
#define GET_POWERADD_SUPPORTING(a,b,c,d,e,f)	xGET_POWERADD_SUPPORTING(a,b,c,d,e,f,__LINE__,__FILE__)
void xGET_POWERADD_SUPPORTING(RDArsrc *,PowerAdd *,void (*)(...),void *,short,short,int,char *);
#define ReturnPowerAddSupportingFileno(a,b,c,d)	xReturnPowerAddSupportingFileno(a,b,c,d,__LINE__,__FILE__)
short xReturnPowerAddSupportingFileno(PowerAdd *,char *,char *,short,int,char *);
#define SetPowerAddSupportingWritable(a,b,c,d,e,f)	xSetPowerAddSupportingWritable(a,b,c,d,e,f,__LINE__,__FILE__)
short xSetPowerAddSupportingWritable(PowerAdd *,char *,char *,short,short,char *,int,char *);
#define APPReturnPowerAddSupportingFileno(a,b,c,d)	xAPPReturnPowerAddSupportingFileno(a,b,c,d,__LINE__,__FILE__)
short xAPPReturnPowerAddSupportingFileno(PowerAdd *,char *,char *,short,int,char *);
#define AllocateEditWdgts(a)	xAllocateEditWdgts(a,__LINE__,__FILE__)
void xAllocateEditWdgts(PowerAdd *,int,char *);
#define FreeEditWdgts(a)	xFreeEditWdgts(a,__LINE__,__FILE__)
void xFreeEditWdgts(PowerAdd *,int,char *);
#define GetAddEditWdgts(a,b)	xGetAddEditWdgts(a,b,__LINE__,__FILE__)
void xGetAddEditWdgts(RDArsrc *,PowerAdd *,int,char *);
#define ShowPowerAddEditWdgt(a)	xShowPowerAddEditWdgt(a,__LINE__,__FILE__)
void xShowPowerAddEditWdgt(AddEditWdgt *,int,char *);
#define ShowPowerAdd(a)	xShowPowerAdd(a,__LINE__,__FILE__)
void xShowPowerAdd(PowerAdd *,int,char *);
#define DefaultAddEditWdgts(a,b,c,d,e)	xDefaultAddEditWdgts(a,b,c,NULL,d,e,__LINE__,__FILE__)
#define ADVDefaultAddEditWdgts(a,b,c,d,e,f)	xDefaultAddEditWdgts(a,b,c,d,e,f,__LINE__,__FILE__)
void xDefaultAddEditWdgts(RDArsrc *,PowerAdd *,short,char *,void (*)(...),void *,int,char *);
#define SetAddEditWdgts(a,b)	xSetAddEditWdgts(a,b,__LINE__,__FILE__)
void xSetAddEditWdgts(RDArsrc *,PowerAdd *,int,char *);
#define SET_POWERADD(r,m,s,a) xSET_POWERADD(r,m,s,a,__LINE__,__FILE__)
void xSET_POWERADD(RDArsrc *,PowerAdd *,void (*)(...),void *,int,char *);
#define SET_POWERADD_MGT(r,m,v,s,a) xSET_POWERADD_MGT(r,m,v,s,a,__LINE__,__FILE__)
void xSET_POWERADD_MGT(RDArsrc *,PowerAdd *,char *,void (*)(...),void *,int,char *);
#define SET_POWERADD_FILE2RSRCS(r,m) xSET_POWERADD_FILE2RSRCS(r,m,__LINE__,__FILE__)
void xSET_POWERADD_FILE2RSRCS(RDArsrc *,PowerAdd *,int,char *);
#define SET_POWERADD_KEYFUNC(a,b,c,d)	xSET_POWERADD_KEYFUNC(a,b,c,d,__LINE__,__FILE__)
void xSET_POWERADD_KEYFUNC(RDArsrc *,PowerAdd  *,void (*)(...),void *,int,char *);

#define RUNPOWERADD(a,b,c,d,e,f)	xRUNPOWERADD(a,b,c,d,e,f,FALSE,__LINE__,__FILE__)
#define RUNPOWERADDADV(a,b,c,d,e,f,g)	xRUNPOWERADD(a,b,c,d,e,f,g,__LINE__,__FILE__)
void xRUNPOWERADD(char *,char *,int,void (*)(...),RDArsrc *,void *,short,int,char *);
#define RUNMAINTAIN(a,b,c,d,e,f)	xRUNMAINTAIN(a,b,c,d,e,f,0,__LINE__,__FILE__)
#define RUNMAINTAINADV(a,b,c,d,e,f,g)	xRUNMAINTAIN(a,b,c,d,e,f,g,__LINE__,__FILE__)
void xRUNMAINTAIN(char *,char *,int,void (*)(...),RDArsrc *,void *,short,int,char *);

short AddEditWdgtSubData(char **,PowerAdd *);
void updateallPowerAdd(RDArsrc *,PowerAdd *);
#define GET_POWERADD_SCREEN_VIRTUALS(a,b,c,d,e) xGET_POWERADD_SCREEN_VIRTUALS(a,b,c,d,e,__LINE__,__FILE__)
void xGET_POWERADD_SCREEN_VIRTUALS(RDArsrc *,short,short,short,PowerAdd *,int,char *);
void READEditWdgts(PowerAdd *);
void POWERADDSubData(char **,PowerAdd *);
void PP_translate_POWERADD(PowerAdd *);
#define PowerAdd2PassKey(a,b)	xPowerAdd2PassKey(a,b,__LINE__,__FILE__)
APPlib *xPowerAdd2PassKey(MaintainButton *,PowerAdd *,int,char *);
#define PowerAdd2ClearPassKey(a)	xPowerAdd2ClearPassKey(a,__LINE__,__FILE__)
APPlib *xPowerAdd2ClearPassKey(MaintainButton *,int,char *);
void PowerMasterBrowseFreeFunction(MakeBrowseList *);
#define MULTIPOWERADD(a,b,c,d,e,f)	xMULTIPOWERADD(a,b,c,d,e,f,FALSE,__LINE__,__FILE__)
#define MULTIPOWERADDADV(a,b,c,d,e,f,g)	xMULTIPOWERADD(a,b,c,d,e,f,g,__LINE__,__FILE__)
void xMULTIPOWERADD(char *,char *,int,void (*)(...),RDArsrc *,void *,short,int,char *);
#define CopyEditWdgt2EditWdgt(a,b)	xCopyEditWdgt2EditWdgt(a,b,__LINE__,__FILE__)
void xCopyEditWdgt2EditWdgt(AddEditWdgt *,AddEditWdgt *,int,char *);

char run_changevalue_buttons(RDArsrc *,PowerAdd *);
void MTNrun_prequit_buttonsbl(RDArsrc *,MTNPassableStruct *,short);
void MTNrun_presave_buttons(RDArsrc *,MTNPassableStruct *,short);
void MTNrun_postsave_buttons(RDArsrc *,MTNPassableStruct *,short);
void MTNrun_prequit_buttonsmtn(RDArsrc *,MTNPassableStruct *,short);
char *GETASSIGNNEXTNEWID(PowerAdd *,AddEditWdgt *,char *,short,void (*)(...),void *);
#define ASSIGNNEWID(a,b,c,d,e)	xASSIGNNEWID(a,b,c,d,e,__LINE__,__FILE__)
void xASSIGNNEWID(PowerAdd *,AddEditWdgt *,char *,void (*)(...),void *,int,char *);
void UndoASSIGNID(PowerAdd *,AddEditWdgt *,void (*)(...),void *);
#define REMOVENEWID(a,b,c,d)	xREMOVENEWID(a,b,c,d,__LINE__,__FILE__)
void xREMOVENEWID(PowerAdd *,AddEditWdgt *,void (*)(...),void *,int,char *);

short PowerMasterBrowseSupBreakFunctionLPM(RDAsearch *,RDArsrc *);
short PowerMasterBrowseSupBreakFunction(RDAsearch *,RDArsrc *);
short PowerMasterBrowseSupBreakFunction2(MakeBrowseList *);
void PowerMasterBrowseSupInitFunction(RDAsearch *);
void PowerMasterBrowseSupInitFunctionLPM(RDAsearch *);
short PowerMasterBrowseTestInherited(MakeBrowseList *,RDArmem *);
#define ComputeAddEditWdgts(a,b,c,d,e,f)	xComputeAddEditWdgts(a,b,c,d,e,f,__LINE__,__FILE__)
void xComputeAddEditWdgts(RDArsrc *,PowerAdd *,short,char *,void (*)(...),void *,int,char *);

#endif /* POWADD_H */
