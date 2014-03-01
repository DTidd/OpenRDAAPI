/* ezrpt.c - Easy Line Report Generator Front End */
#ifndef WIN32
#define __NAM__ "ezrpt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "ezrpt.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include<io.h>
#endif
#include <cnvscn.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>
/*ADDLIB cscn */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define ALLOW_INTERNAL_VIRTUALS

static char *module="RPTGEN",*holdmodule=NULL,*holdfile=NULL;
static APPlib *modlstx=NULL,*rptavl=NULL,*detaillst=NULL,*sortlst=NULL;
static APPlib *wtypes=NULL,*file_defs=NULL,*totals=NULL,*fieldlst=NULL,*ftypes=NULL;
static APPlib *device_list=NULL,*print_size=NULL;
static RDAezrpt *ezrpt=NULL;
static EZfield *holdfields=NULL;
static int numfields=0;
static void savesort(RDArsrc *,RDArsrc *);
static void quitsort(RDArsrc *,RDArsrc *),quitsorttest(RDArsrc *,RDArsrc *);
static void savedetail(RDArsrc *,RDArsrc *);
static void quitdetail(RDArsrc *,RDArsrc *),quitdetailtest(RDArsrc *,RDArsrc *);
static void quitrpt(RDArsrc *,RDArsrc *),quitrpttest(RDArsrc *,RDArsrc *);
static void saverpt(RDArsrc *,RDArsrc *);
static void deleterpt(RDArsrc *),deleterpttest(RDArsrc *);
static void getrptentries(RDArsrc *),dofilelist(RDArsrc *,short *);
static void setvalues(RDArsrc *),printezrptdef(RDArsrc *,RDAezrpt *);
static void changeTYPES(RDArsrc *,RDArsrc *);
#define makefieldvallist(a)	xmakefieldvallist(a,__LINE__,__FILE__)
static APPlib *xmakefieldvallist(char,int,char *);
static short DIAGNOSTICS=FALSE,init_value=FALSE;
static RDAdefault *sortdefaults=NULL,*detaildefaults=NULL,*rptdefaults=NULL;
static char changeddetails=FALSE,changedsorts=FALSE,changedezrpt=FALSE;

char *sequences[6]={"Ascending","Ascending Lower","Ascending Upper","Descending","Descending Lower","Descending Upper"};

static void Apply_RangeDimension0Codes(char *screen_module,char *screen_name)
{
	RDAcnvscn *csn=NULL;
	RDAacct *acct=NULL;
	RDAaccpart *accpart=NULL;
	char *temp=NULL;

	if(diagapps)
	{
		prterr("DIAG Apply_RangeDimension0Codes(): Screen Module: [%s] Name: [%s]",screen_module,screen_name);
	}
	if(EXPENDITURE_ACCOUNT==NULL) return;
	if(EXPENDITURE_ACCOUNT->num>1) return;
	acct=EXPENDITURE_ACCOUNT->codetype;
	accpart=acct->acct;
	temp=Rmalloc(RDAstrlen(accpart->name)+11);
	sprintf(temp,"Range on %s's",(accpart->name!=NULL ? accpart->name:""));
	
	csn=RDAcnvscnNEW();
	csn->set_which=0; 
	csn->match_type=0;
	csn->pat_scn=FALSE;
	csn->scn_pattern=NULL;
	csn->rng_scn=TRUE;
	csn->from_scn=stralloc(screen_name);
	csn->to_scn=stralloc(screen_name);
	csn->all_dirs=FALSE;
	csn->all_libs=FALSE;
	csn->directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+10);
#ifndef WIN32
	sprintf(csn->directory,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(csn->directory,"%s\\site",CURRENTDIRECTORY);
#endif
	csn->library=stralloc(screen_module);
	memset(stemp,0,101);
	sprintf(stemp,"RANGE ON [DIMENSION #0 CODE]");
	csn->from.resource_name=stralloc(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"Range on Dimension #0 Code");
	csn->from.label=stralloc(stemp);
	csn->from.type=9;
	csn->from.pixmap=NULL;
	csn->from.XHTML_Label=NULL;
	csn->from.expression=NULL;
	csn->from.editable_expression=NULL;
	csn->from.sensitive_expression=NULL;
	csn->from.transversal_expression=NULL;
	csn->from.rows=0;
	csn->from.cols=0;
	csn->to.type=9;
	sprintf(stemp,"RANGE ON [DIMENSION #0 CODE]");
	csn->to.resource_name=stralloc(stemp);
	csn->to.label=stralloc(temp);
	csn->to.pixmap=NULL;
	csn->to.XHTML_Label=NULL;
	csn->to.expression=NULL;
	csn->to.editable_expression=NULL;
	csn->to.sensitive_expression=NULL;
	csn->to.transversal_expression=NULL;
	csn->to.rows=0;
	csn->to.cols=0;
	ADVExecute_RDAcnvscn(csn,FALSE);
	FreeRDAcnvscn(csn);
	if(temp!=NULL) Rfree(temp);
}
static void Apply_RangeDimensionECodes(char *screen_module,char *screen_name)
{
	RDAcnvscn *csn=NULL;
	RDAacct *acct=NULL;
	RDAaccpart *accpart=NULL;
	char *temp=NULL;
	int x=0;


	if(diagapps)
	{
		prterr("DIAG Apply_RangeDimensionECodes(): Screen Module: [%s] Name: [%s]",screen_module,screen_name);
	}
	if(EXPENDITURE_ACCOUNT==NULL) return;
	if(EXPENDITURE_ACCOUNT->num>1) return;
	acct=EXPENDITURE_ACCOUNT->codetype;
	for(x=0;x<acct->num;++x)
	{
	accpart=acct->acct+x;
	temp=Rmalloc(RDAstrlen(accpart->name)+11);
	sprintf(temp,"Range on %s's",(accpart->name!=NULL ? accpart->name:""));
	
	csn=RDAcnvscnNEW();
	csn->set_which=0; 
	csn->match_type=0;
	csn->pat_scn=FALSE;
	csn->scn_pattern=NULL;
	csn->rng_scn=TRUE;
	csn->from_scn=stralloc(screen_name);
	csn->to_scn=stralloc(screen_name);
	csn->all_dirs=FALSE;
	csn->all_libs=FALSE;
	csn->directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+10);
#ifndef WIN32
	sprintf(csn->directory,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(csn->directory,"%s\\site",CURRENTDIRECTORY);
#endif
	csn->library=stralloc(screen_module);
	memset(stemp,0,101);
	sprintf(stemp,"RANGE ON [DIMENSION #%d CODE]",x);
	csn->from.resource_name=stralloc(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"Range on Dimension #%d Code",x);
	csn->from.label=stralloc(stemp);
	csn->from.type=9;
	csn->from.pixmap=NULL;
	csn->from.XHTML_Label=NULL;
	csn->from.expression=NULL;
	csn->from.editable_expression=NULL;
	csn->from.sensitive_expression=NULL;
	csn->from.transversal_expression=NULL;
	csn->from.rows=0;
	csn->from.cols=0;
	csn->to.type=9;
	sprintf(stemp,"RANGE ON [DIMENSION #%d CODE]",x);
	csn->to.resource_name=stralloc(stemp);
	csn->to.label=stralloc(temp);
	csn->to.pixmap=NULL;
	csn->to.XHTML_Label=NULL;
	csn->to.expression=NULL;
	csn->to.editable_expression=NULL;
	csn->to.sensitive_expression=NULL;
	csn->to.transversal_expression=NULL;
	csn->to.rows=0;
	csn->to.cols=0;
	ADVExecute_RDAcnvscn(csn,FALSE);
	FreeRDAcnvscn(csn);
	if(temp!=NULL) Rfree(temp);
	}
}
static void Apply_RangeDimensionRCodes(char *screen_module,char *screen_name)
{
	RDAcnvscn *csn=NULL;
	RDAacct *acct=NULL;
	RDAaccpart *accpart=NULL;
	char *temp=NULL;
	int x=0;


	if(diagapps)
	{
		prterr("DIAG Apply_RangeDimensionRCodes(): Screen Module: [%s] Name: [%s]",screen_module,screen_name);
	}
	if(REVENUE_ACCOUNT==NULL) return;
	if(REVENUE_ACCOUNT->num>1) return;
	acct=REVENUE_ACCOUNT->codetype;
	for(x=0;x<acct->num;++x)
	{
	accpart=acct->acct+x;
	temp=Rmalloc(RDAstrlen(accpart->name)+11);
	sprintf(temp,"Range on %s's",(accpart->name!=NULL ? accpart->name:""));
	
	csn=RDAcnvscnNEW();
	csn->set_which=0; 
	csn->match_type=0;
	csn->pat_scn=FALSE;
	csn->scn_pattern=NULL;
	csn->rng_scn=TRUE;
	csn->from_scn=stralloc(screen_name);
	csn->to_scn=stralloc(screen_name);
	csn->all_dirs=FALSE;
	csn->all_libs=FALSE;
	csn->directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+10);
#ifndef WIN32
	sprintf(csn->directory,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(csn->directory,"%s\\site",CURRENTDIRECTORY);
#endif
	csn->library=stralloc(screen_module);
	memset(stemp,0,101);
	sprintf(stemp,"RANGE ON [DIMENSION #%d CODE]",x);
	csn->from.resource_name=stralloc(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"Range on Dimension #%d Code",x);
	csn->from.label=stralloc(stemp);
	csn->from.type=9;
	csn->from.pixmap=NULL;
	csn->from.XHTML_Label=NULL;
	csn->from.expression=NULL;
	csn->from.editable_expression=NULL;
	csn->from.sensitive_expression=NULL;
	csn->from.transversal_expression=NULL;
	csn->from.rows=0;
	csn->from.cols=0;
	csn->to.type=9;
	sprintf(stemp,"RANGE ON [DIMENSION #%d CODE]",x);
	csn->to.resource_name=stralloc(stemp);
	csn->to.label=stralloc(temp);
	csn->to.pixmap=NULL;
	csn->to.XHTML_Label=NULL;
	csn->to.expression=NULL;
	csn->to.editable_expression=NULL;
	csn->to.sensitive_expression=NULL;
	csn->to.transversal_expression=NULL;
	csn->to.rows=0;
	csn->to.cols=0;
	ADVExecute_RDAcnvscn(csn,FALSE);
	FreeRDAcnvscn(csn);
	if(temp!=NULL) Rfree(temp);
	}
}
static void Apply_RangeDimensionBCodes(char *screen_module,char *screen_name)
{
	RDAcnvscn *csn=NULL;
	RDAacct *acct=NULL;
	RDAaccpart *accpart=NULL;
	char *temp=NULL;
	int x=0;


	if(diagapps)
	{
		prterr("DIAG Apply_RangeDimensionBCodes(): Screen Module: [%s] Name: [%s]",screen_module,screen_name);
	}
	if(BALANCE_SHEET_ACCOUNT==NULL) return;
	if(BALANCE_SHEET_ACCOUNT->num>1) return;
	acct=BALANCE_SHEET_ACCOUNT->codetype;
	for(x=0;x<acct->num;++x)
	{
	accpart=acct->acct+x;
	temp=Rmalloc(RDAstrlen(accpart->name)+11);
	sprintf(temp,"Range on %s's",(accpart->name!=NULL ? accpart->name:""));
	
	csn=RDAcnvscnNEW();
	csn->set_which=0; /* Current */
	csn->match_type=0;
	csn->pat_scn=FALSE;
	csn->scn_pattern=NULL;
	csn->rng_scn=TRUE;
	csn->from_scn=stralloc(screen_name);
	csn->to_scn=stralloc(screen_name);
	csn->all_dirs=FALSE;
	csn->all_libs=FALSE;
	csn->directory=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+10);
#ifndef WIN32
	sprintf(csn->directory,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(csn->directory,"%s\\site",CURRENTDIRECTORY);
#endif
	csn->library=stralloc(screen_module);
	memset(stemp,0,101);
	sprintf(stemp,"RANGE ON [DIMENSION #%d CODE]",x);
	csn->from.resource_name=stralloc(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"Range on Dimension #%d Code",x);
	csn->from.label=stralloc(stemp);
	csn->from.type=9;
	csn->from.pixmap=NULL;
	csn->from.XHTML_Label=NULL;
	csn->from.expression=NULL;
	csn->from.editable_expression=NULL;
	csn->from.sensitive_expression=NULL;
	csn->from.transversal_expression=NULL;
	csn->from.rows=0;
	csn->from.cols=0;
	csn->to.type=9;
	sprintf(stemp,"RANGE ON [DIMENSION #%d CODE]",x);
	csn->to.resource_name=stralloc(stemp);
	csn->to.label=stralloc(temp);
	csn->to.pixmap=NULL;
	csn->to.XHTML_Label=NULL;
	csn->to.expression=NULL;
	csn->to.editable_expression=NULL;
	csn->to.sensitive_expression=NULL;
	csn->to.transversal_expression=NULL;
	csn->to.rows=0;
	csn->to.cols=0;
	ADVExecute_RDAcnvscn(csn,FALSE);
	FreeRDAcnvscn(csn);
	if(temp!=NULL) Rfree(temp);
	}
}
static void EZRPT_ConvScrn(char *screen_module,char *screen_name,short acct_type)
{
	if(diagapps)
	{
		prterr("DIAG EZRPT_ConvScrn(): Screen Module: [%s] Name: [%s] Account Type: [%d]",screen_module,screen_name,acct_type);
	}
	switch(acct_type)
	{
		default:
		case -1:
			Apply_RangeDimension0Codes(screen_module,screen_name);
			break;
		case 0: /* Expenditure */
			Apply_RangeDimensionECodes(screen_module,screen_name);
			break;
		case 1: /* Revenue */
			Apply_RangeDimensionRCodes(screen_module,screen_name);
			break;
		case 2: /* Balance Sheet */
			Apply_RangeDimensionBCodes(screen_module,screen_name);
			break;
		case 3: /* Beginning Balance */
			Apply_RangeDimension0Codes(screen_module,screen_name);
			break;
	}
}
static void printavailrsrc(RDArsrc *rptrsrc,RDAezrpt *ezrpt)
{
	RDA_PFILE *fp=NULL;
	int x=0,pages=0;
	char *outdevice=NULL,*message=NULL,*module=NULL;
	EZfield *f=NULL;

	outdevice=DefaultPrinter();
	fp=RDA_popen(outdevice,"w");
	if(fp!=NULL)
	{
		SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
		FINDRSCGETINT(rptrsrc,"MODULE LIST",&x);
		module=stralloc(modlstx->libs[x]);
		RDA_fprintf(fp,"%*sXPERT EZ-REPORT RESOURCES                  Page: %*d\r\n",27,"",4,++pages);
		RDA_fprintf(fp,"%*sName [%s]  Module [%s]\r\n",
			((80-(19+RDAstrlen(ezrpt->name)+RDAstrlen(module)))/2),""
			,ezrpt->name,module);
		RDA_fprintf(fp,"%*sTitle [%s]\r\n\r\n",
			((80-(9+RDAstrlen(ezrpt->title)))/2),"",ezrpt->title);
	} else {
		message=Rmalloc(RDAstrlen(outdevice)+69);
		sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
		WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
		prterr(message);
		if(message!=NULL) { Rfree(message); message=NULL; }
		return;
	}
	if(holdfields!=NULL)
	{
		for(x=0,f=holdfields;x<numfields;++x,++f)
		{
			RDA_fprintf(fp,"%-50s %-20s %4d\r\n",
				(f->wholename!=NULL ? f->wholename:""),
				fieldtypedesc[f->type],f->len);
		}
	}
	RDA_fprintf(fp,"\f");
	SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
	RDA_pclose(fp);
	if(outdevice!=NULL) outdevice=NULL;
	if(message!=NULL) message=NULL;
	if(module!=NULL) module=NULL;
}
static void printezrptdef(RDArsrc *rptrsrc,RDAezrpt *ezrpt)
{
	RDA_PFILE *fp=NULL;
	int x=0,y=0,z=0,pages=0;
	RDAezsrt *fields;
	RDAezdtl *fields1;
	char *outdevice=NULL,*message=NULL,*module=NULL;

	outdevice=DefaultPrinter();
	fp=RDA_popen(outdevice,"w");
	if(fp!=NULL)
	{
		SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
		FINDRSCGETINT(rptrsrc,"MODULE LIST",&x);
		module=stralloc(modlstx->libs[x]);
		RDA_fprintf(fp,"%*sXPERT EZ-REPORT DEFINITION                 Page: %*d\n",27,"",4,++pages);
		RDA_fprintf(fp,"%*sName [%s]  Module [%s]\n",
			((80-(19+RDAstrlen(ezrpt->name)+RDAstrlen(module)))/2),""
			,ezrpt->name,module);
		RDA_fprintf(fp,"%*sTitle [%s]\n\n",
			((80-(9+RDAstrlen(ezrpt->title)))/2),"",ezrpt->title);
	} else {
		message=Rmalloc(RDAstrlen(outdevice)+69);
		sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
		WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
		prterr(message);
		if(message!=NULL) { Rfree(message); message=NULL; }
		return;
	}
	RDA_fprintf(fp,"Description:\n");
	if(RDAstrlen(ezrpt->desc)<77)
	{
		RDA_fprintf(fp,"   %s\n",ezrpt->desc);
	} else {
		for(x=0;x<RDAstrlen(ezrpt->desc);)
		{
			if((RDAstrlen(ezrpt->desc)-x)<77)
			z=(RDAstrlen(ezrpt->desc)-x);
			else z=77;
			for(y=0;y<z;++y)
			{
				stemp[y]=ezrpt->desc[x];
				++x;
			}
			stemp[y]=0;
			RDA_fprintf(fp,"   %s\n",stemp);
		}
	}
	RDA_fprintf(fp,"Major Module: [%s]  Major File: [%s]\n"
		,ezrpt->majmod,ezrpt->majfil);
	RDA_fprintf(fp,"Sort / Select Definitions:  ");
	if(ezrpt->sort!=NULL)
	{
		RDA_fprintf(fp,"\n");
		for(x=0,fields=ezrpt->sort;x<ezrpt->numsrt;++x,++fields)
		{
			RDA_fprintf(fp,"   Name: [%s]  Total: [%s]  Start: [%d]  Length: [%d] \n",
				(fields->name!=NULL?fields->name:""),
				(fields->total!=(-1)?totals->libs[fields->total]:"Not Available"),
				fields->start,fields->length);
			if(fields->range==TRUE)
			{
				RDA_fprintf(fp,"      Range Values: ");
				switch(fields->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						RDA_fprintf(fp,"From: [%s] To: [%s]",
							(fields->from.string_value!=NULL?fields->from.string_value:""),
							(fields->to.string_value!=NULL?fields->to.string_value:""));
						break;
					case BOOLNS:
					case CHARACTERS:
						RDA_fprintf(fp,"From: [%c] To: [%c]",
							*fields->from.string_value,
							*fields->to.string_value);
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS_NOCENTS:
						RDA_fprintf(fp,"From: [%f] To: [%f]",
							*fields->from.float_value,*fields->to.float_value);
						break;
					case DOLLARS:
						RDA_fprintf(fp,"From: [%.02f] To: [%.02f]",
							(*fields->from.float_value/100),(*fields->to.float_value/100));
						break;
					case SHORTV:
					case SSHORTV:
						RDA_fprintf(fp,"From: [%d] To: [%d]",
							*fields->from.short_value,*fields->to.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						RDA_fprintf(fp,"From: [%d] To: [%d]",
							*fields->from.integer_value,*fields->to.integer_value);
						break;
					default :
						break;
				}
				RDA_fprintf(fp,"\n");
			}
		}
	} else {
		RDA_fprintf(fp,"No Sorts Defined\n");
	}
	RDA_fprintf(fp,"Device: [%s]\n",ezrpt->devicename);
	RDA_fprintf(fp,"Print Size: [%s]\n",ezrpt->printsize);
	RDA_fprintf(fp,"Alignment Test: [%s]\n"
		,(ezrpt->alignment_test==TRUE ? "TRUE":"FALSE"));
	RDA_fprintf(fp,"Print Header / Footer: [%s]\n"
		,(ezrpt->print==TRUE ? "TRUE":"FALSE"));
	RDA_fprintf(fp,"Page Length: [%.03f]\n",ezrpt->page_length);
	RDA_fprintf(fp,"Detail Line Definition:  ");
	if(ezrpt->detail!=NULL)
	{
		RDA_fprintf(fp,"\n");
		for(x=0,fields1=ezrpt->detail;x<ezrpt->numdtl;++x,++fields1)
		{
			RDA_fprintf(fp,"   Title1: [%s] Title2: [%s] Start: [%d] Length: [%d]\n",
				(fields1->title1!=NULL ? fields1->title1:""),
				(fields1->title2!=NULL ? fields1->title2:""),
				fields1->start,fields1->length);
			RDA_fprintf(fp,"      Value: [%s] Show on Range Screen: [%s]\n",
				(fields1->type ? 
				fields1->expression:fields1->fieldname),
				(fields1->range==TRUE ? "TRUE":"FALSE"));
		}
	} else {
		RDA_fprintf(fp,"No Detail Defined\n");
	}
	SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
	RDA_pclose(fp);
	if(outdevice!=NULL) outdevice=NULL;
	if(message!=NULL) message=NULL;
	if(module!=NULL) module=NULL;
}
static char *addholdvalues(APPlib *tmp,char *modulex,char *filex,short first)
{
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	EZfield *ezf;
	short countkey=0,z,x,engine=0,nofields=0,nokeys=0,addfile=FALSE;
	int y;
	char *temp=NULL,*return_string=NULL;

	if(diagapps)
	{
		prterr("DIAG addholdvalues Module [%s] File [%s] ",modulex,filex);
	}
	if(!GETBIN(modulex,filex,&engine,&nofields,&fields,
		&nokeys,&keys)) 
	{
		if(keys!=NULL)
		{
			if(first)
			{
				addfile=TRUE;
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(return_string==NULL) return_string=stralloc(keyx->name);
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
			} else {
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->part!=NULL)
					{
						countkey=0;
						for(z=0,part=keyx->part;z<keyx->numparts;
							++z,++part)
						{
							if(holdfields!=NULL)
							{
								for(x=0,ezf=holdfields;x<numfields;++x,++ezf)
								{
									if(!RDAstrcmp(ezf->name,part->name))
									{
										++countkey;
										break;
									}
								}
							}
							if(part->name!=NULL) Rfree(part->name);
						}
						if(countkey==keyx->numparts) 
						{
							addfile=TRUE;
							if(return_string==NULL) 
								return_string=stralloc(keyx->name);
						}
						if(keyx->name!=NULL) Rfree(keyx->name);
					}
					Rfree(keyx->part);
				}
			}
			Rfree(keys);
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(addfile)
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

					if(addAPPlibNoDuplicates(tmp,temp)!=(-1))
					{
					if(holdfields!=NULL)
					{
						holdfields=Rrealloc(holdfields,(numfields+1)*
							sizeof(EZfield));
					} else {
						holdfields=Rmalloc(sizeof(EZfield));
						numfields=0;
					}
					ezf=holdfields+numfields;
					if(f->name!=NULL)
					{
						ezf->name=stralloc(f->name);
					} else ezf->name=NULL;	
					ezf->type=f->type;
					ezf->len=f->len;
					++numfields;
					ezf->wholename=stralloc(temp);
					}
					if(temp!=NULL) Rfree(temp);	
					}
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	if(!addfile)
	{
		if(return_string!=NULL) Rfree(return_string);
		return_string=NULL;
	}
	return(return_string);
}
static APPlib *xmakefieldvallist(char skip_finmgt,int line,char *file)
{
	APPlib *tmp=NULL,*filenames=NULL;
	int x,y,z,no_parts;
	char *libx=NULL,addvf=TRUE,*keyname=NULL,*tmpx=NULL;
	char *temp=NULL;
	EZfield *f,*ezf;
#ifdef ALLOW_INTERNAL_VIRTUALS
	RDAezdtl *ezdtlfields=NULL;
#endif /* ALLOW_INTERNAL_VIRTUALS */
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	RDAacct *acc;
	RDArsrc *tmprsrc=NULL;

	if(DIAGNOSTICS)
	{
		prterr("DIAG makefieldvallist at line [%d] program [%s].",line,file);
	}
	if(ezrpt->incvir!=NULL)
	{
		for(x=0,i=ezrpt->incvir;x<ezrpt->num;++x,++i)
		{
			if(i->module!=NULL) Rfree(i->module);
			if(i->file!=NULL) Rfree(i->file);
			if(i->keyname!=NULL) Rfree(i->keyname);
		}
		Rfree(ezrpt->incvir);
		ezrpt->incvir=NULL;
		ezrpt->num=0;
	}
	if(holdfields!=NULL)
	{
		for(x=0,f=holdfields;x<numfields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
			if(f->wholename!=NULL) Rfree(f->wholename);
		}
		Rfree(holdfields);
		holdfields=NULL;
		numfields=0;
	}
	tmp=APPlibNEW();
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(holdmodule)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,holdmodule);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,holdmodule);
#endif
	filenames=getunsortedlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	keyname=addholdvalues(tmp,holdmodule,holdfile,TRUE);
	if(ezrpt->incvir!=NULL)
	{
		ezrpt->incvir=Rrealloc(ezrpt->incvir,(ezrpt->num+1)*sizeof(DFincvir));
	} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
	i=ezrpt->incvir+ezrpt->num;
	i->module=stralloc(holdmodule);
	i->file=stralloc(holdfile);
	if(keyname!=NULL) 
	{
		i->keyname=stralloc(keyname);
		Rfree(keyname);
	} else i->keyname=NULL;
	++ezrpt->num;
	for(x=0;x<filenames->numlibs;++x)
	{
		if(RDAstrcmp(filenames->libs[x],holdfile) && atoi(filenames->libs[x])==0)  
		{
			if(!RDAstrcmp(holdmodule,"FINMGT"))
			{	
				if(!RDAstrcmp(holdfile,"FINBACC") || !RDAstrcmp(holdfile,"FINBYR")
					|| !RDAstrcmp(holdfile,"FINBACT"))
				{
					if(RDAstrcmp(filenames->libs[x],"FINEYR") &&
						RDAstrcmp(filenames->libs[x],"FINEACT") &&
						RDAstrcmp(filenames->libs[x],"FINRYR") &&
						RDAstrcmp(filenames->libs[x],"FINRACT"))
					{
						if((keyname=addholdvalues(tmp,holdmodule,
							filenames->libs[x],FALSE))!=NULL)
						{
							if(ezrpt->incvir!=NULL)
							{
								ezrpt->incvir=Rrealloc(ezrpt->incvir,
									(ezrpt->num+1)*sizeof(DFincvir));
							} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
							i=ezrpt->incvir+ezrpt->num;
							i->module=stralloc(holdmodule);
							i->file=stralloc(filenames->libs[x]);
							i->keyname=stralloc(keyname);
							++ezrpt->num;
							Rfree(keyname);
						}
					}
				} else if(!RDAstrcmp(holdfile,"FINRACC") || 
					!RDAstrcmp(holdfile,"FINRYR") || 
					!RDAstrcmp(holdfile,"FINRACT"))
				{
					if(RDAstrcmp(filenames->libs[x],"FINEYR") &&
						RDAstrcmp(filenames->libs[x],"FINEACT") &&
						RDAstrcmp(filenames->libs[x],"FINBYR") &&
						RDAstrcmp(filenames->libs[x],"FINBACT"))
					{
						if((keyname=addholdvalues(tmp,holdmodule,
							filenames->libs[x],FALSE))!=NULL)
						{
							if(ezrpt->incvir!=NULL)
							{
								ezrpt->incvir=Rrealloc(ezrpt->incvir,
									(ezrpt->num+1)*sizeof(DFincvir));
							} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
							i=ezrpt->incvir+ezrpt->num;
							i->module=stralloc(holdmodule);
							i->file=stralloc(filenames->libs[x]);
							i->keyname=stralloc(keyname);
							++ezrpt->num;
							Rfree(keyname);
						}
					}
				} else if(!RDAstrcmp(holdfile,"FINEACC") ||
					!RDAstrcmp(holdfile,"FINEYR") || 
					!RDAstrcmp(holdfile,"FINEACT"))	
				{
					if(RDAstrcmp(filenames->libs[x],"FINRYR") &&
						RDAstrcmp(filenames->libs[x],"FINRACT") &&
						RDAstrcmp(filenames->libs[x],"FINBYR") &&
						RDAstrcmp(filenames->libs[x],"FINBACT"))
					{
						if((keyname=addholdvalues(tmp,holdmodule,
							filenames->libs[x],FALSE))!=NULL)
						{
							if(ezrpt->incvir!=NULL)
							{
								ezrpt->incvir=Rrealloc(ezrpt->incvir,
									(ezrpt->num+1)*sizeof(DFincvir));
							} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
							i=ezrpt->incvir+ezrpt->num;
							i->module=stralloc(holdmodule);
							i->file=stralloc(filenames->libs[x]);
							i->keyname=stralloc(keyname);
							++ezrpt->num;
							Rfree(keyname);
						}
					}
				} else {
					if((keyname=addholdvalues(tmp,holdmodule,
						filenames->libs[x],FALSE))!=NULL)
					{
						if(ezrpt->incvir!=NULL)
						{
							ezrpt->incvir=Rrealloc(ezrpt->incvir,
								(ezrpt->num+1)*sizeof(DFincvir));
						} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
						i=ezrpt->incvir+ezrpt->num;
						i->module=stralloc(holdmodule);
						i->file=stralloc(filenames->libs[x]);
						i->keyname=stralloc(keyname);
						++ezrpt->num;
						Rfree(keyname);
					}
				}
			} else {
				if((keyname=addholdvalues(tmp,holdmodule,
					filenames->libs[x],FALSE))!=NULL)
				{
					if(ezrpt->incvir!=NULL)
					{
						ezrpt->incvir=Rrealloc(ezrpt->incvir,
							(ezrpt->num+1)*sizeof(DFincvir));
					} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
					i=ezrpt->incvir+ezrpt->num;
					i->module=stralloc(holdmodule);
					i->file=stralloc(filenames->libs[x]);
					i->keyname=stralloc(keyname);
					++ezrpt->num;
					Rfree(keyname);
				}
			}
		}
	}
	if(filenames!=NULL) freeapplib(filenames);
/* PERSONNEL EXTRA CONNECTIONS */
	if(!RDAstrcmp(holdmodule,"PAYROLL") || !RDAstrcmp(holdmodule,"LVEMGT")
		|| !RDAstrcmp(holdmodule,"SUBMGT") 
		|| !RDAstrcmp(holdmodule,"EMPABS")
		|| !RDAstrcmp(holdmodule,"POSTRK") 
		|| !RDAstrcmp(holdmodule,"ANNMGT") 
		|| !RDAstrcmp(holdmodule,"APPMGT")
		|| !RDAstrcmp(holdmodule,"BUDPREP"))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PRSNNL")+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,"PRSNNL");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,"PRSNNL");
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(filenames!=NULL)
		{
		for(x=0;x<filenames->numlibs;++x)
		{
			if(RDAstrcmp(filenames->libs[x],holdfile))  
			{
			if((keyname=addholdvalues(tmp,"PRSNNL",
				filenames->libs[x],FALSE))!=NULL)
			{
				if(ezrpt->incvir!=NULL)
				{
					ezrpt->incvir=Rrealloc(ezrpt->incvir,
						(ezrpt->num+1)*sizeof(DFincvir));
				} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
				i=ezrpt->incvir+ezrpt->num;
				i->module=stralloc("PRSNNL");
				i->file=stralloc(filenames->libs[x]);
				i->keyname=stralloc(keyname);
				++ezrpt->num;
				Rfree(keyname);
			}
			}
		}
		}
	}
/* POSTRK EXTRA CONNECTIONS */
	if(!RDAstrcmp(holdmodule,"PAYROLL") || !RDAstrcmp(holdmodule,"LVEMGT")
		|| !RDAstrcmp(holdmodule,"SUBMGT") 
		|| !RDAstrcmp(holdmodule,"EMPABS")
		|| !RDAstrcmp(holdmodule,"APPMGT")
		|| !RDAstrcmp(holdmodule,"BUDPREP"))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("POSTRK")+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,"POSTRK");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,"POSTRK");
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(filenames!=NULL)
		{
		for(x=0;x<filenames->numlibs;++x)
		{
			if(RDAstrcmp(filenames->libs[x],holdfile))  
			{
			if((keyname=addholdvalues(tmp,"POSTRK",
				filenames->libs[x],FALSE))!=NULL)
			{
				if(ezrpt->incvir!=NULL)
				{
					ezrpt->incvir=Rrealloc(ezrpt->incvir,
						(ezrpt->num+1)*sizeof(DFincvir));
				} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
				i=ezrpt->incvir+ezrpt->num;
				i->module=stralloc("POSTRK");
				i->file=stralloc(filenames->libs[x]);
				i->keyname=stralloc(keyname);
				++ezrpt->num;
				Rfree(keyname);
			}
			}
		}
		}
	}
/* PURORD EXTRA CONNECTIONS */
	if(!RDAstrcmp(holdmodule,"VENPMT")
		|| !RDAstrcmp(holdmodule,"FIXASS") 
		|| !RDAstrcmp(holdmodule,"INVCTL")
		|| !RDAstrcmp(holdmodule,"RFPMGT"))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PURORD")+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,"PURORD");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,"PURORD");
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(filenames!=NULL)
		{
		for(x=0;x<filenames->numlibs;++x)
		{
			if(RDAstrcmp(filenames->libs[x],holdfile))  
			{
			if((keyname=addholdvalues(tmp,"PURORD",
				filenames->libs[x],FALSE))!=NULL)
			{
				if(ezrpt->incvir!=NULL)
				{
					ezrpt->incvir=Rrealloc(ezrpt->incvir,
						(ezrpt->num+1)*sizeof(DFincvir));
				} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
				i=ezrpt->incvir+ezrpt->num;
				i->module=stralloc("PURORD");
				i->file=stralloc(filenames->libs[x]);
				i->keyname=stralloc(keyname);
				++ezrpt->num;
				Rfree(keyname);
			}
			}
		}
		}
	}
/* VENPMT EXTRA CONNECTIONS */
	if(!RDAstrcmp(holdmodule,"PAYROLL")
		|| !RDAstrcmp(holdmodule,"PURORD") 
		|| !RDAstrcmp(holdmodule,"REQMGT")
		|| !RDAstrcmp(holdmodule,"BFTMGT") 
		|| !RDAstrcmp(holdmodule,"CATALOGUE")
		|| !RDAstrcmp(holdmodule,"BUDPREP"))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("VENPMT")+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,"VENPMT");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,"VENPMT");
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(filenames!=NULL)
		{
		for(x=0;x<filenames->numlibs;++x)
		{
			if(RDAstrcmp(filenames->libs[x],holdfile))  
			{
			if((keyname=addholdvalues(tmp,"VENPMT",
				filenames->libs[x],FALSE))!=NULL)
			{
				if(ezrpt->incvir!=NULL)
				{
					ezrpt->incvir=Rrealloc(ezrpt->incvir,
						(ezrpt->num+1)*sizeof(DFincvir));
				} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
				i=ezrpt->incvir+ezrpt->num;
				i->module=stralloc("VENPMT");
				i->file=stralloc(filenames->libs[x]);
				i->keyname=stralloc(keyname);
				++ezrpt->num;
				Rfree(keyname);
			}
			}
		}
		}
	}
/* PAYROLL EXTRA CONNECTIONS */
	if(!RDAstrcmp(holdmodule,"LVEMGT")
		|| !RDAstrcmp(holdmodule,"SUBMGT") 
		|| !RDAstrcmp(holdmodule,"EMPABS")
		|| !RDAstrcmp(holdmodule,"POSTRK") 
		|| !RDAstrcmp(holdmodule,"BUDPREP"))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,"PAYROLL");
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(filenames!=NULL)
		{
		for(x=0;x<filenames->numlibs;++x)
		{
			if(RDAstrcmp(filenames->libs[x],holdfile))  
			{
			if((keyname=addholdvalues(tmp,"PAYROLL",
				filenames->libs[x],FALSE))!=NULL)
			{
				if(ezrpt->incvir!=NULL)
				{
					ezrpt->incvir=Rrealloc(ezrpt->incvir,
						(ezrpt->num+1)*sizeof(DFincvir));
				} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
				i=ezrpt->incvir+ezrpt->num;
				i->module=stralloc("PAYROLL");
				i->file=stralloc(filenames->libs[x]);
				i->keyname=stralloc(keyname);
				++ezrpt->num;
				Rfree(keyname);
			}
			}
		}
		}
	}
/* FINMGT EXTRA CONNECTIONS */
	if((skip_finmgt==FALSE) && (
		!RDAstrcmp(holdmodule,"VENPMT") ||
		!RDAstrcmp(holdmodule,"PURORD") ||
		!RDAstrcmp(holdmodule,"PAYROLL") ||
		!RDAstrcmp(holdmodule,"POSTRK") ||
		!RDAstrcmp(holdmodule,"REQMGT") ||
		!RDAstrcmp(holdmodule,"RLSTMGT") ||
		!RDAstrcmp(holdmodule,"PROPERTY") ||
		!RDAstrcmp(holdmodule,"MISCBILL") ||
		!RDAstrcmp(holdmodule,"OCCTAX") ||
		!RDAstrcmp(holdmodule,"BNKREC") ||
		!RDAstrcmp(holdmodule,"FIXASS") ||
		!RDAstrcmp(holdmodule,"INVCTL") ||
		!RDAstrcmp(holdmodule,"BUDPREP")))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,"FINMGT");
#endif
		filenames=getunsortedlibnames(libx,FALSE);
		if(libx!=NULL) Rfree(libx);
		if(filenames!=NULL)
		{
		for(x=0;x<filenames->numlibs;++x)
		{
			if(RDAstrcmp(filenames->libs[x],holdfile) &&
				RDAstrcmp(filenames->libs[x],"FINEYR") &&
				RDAstrcmp(filenames->libs[x],"FINRYR") &&
				RDAstrcmp(filenames->libs[x],"FINBYR") &&
				RDAstrcmp(filenames->libs[x],"FINEACT") &&
				RDAstrcmp(filenames->libs[x],"FINRACT") &&
				RDAstrcmp(filenames->libs[x],"FINBREF") &&
				RDAstrcmp(filenames->libs[x],"FINBACT") &&
				RDAstrcmp(filenames->libs[x],"FINJEN") &&
				RDAstrcmp(filenames->libs[x],"FINREC"))  
			{
			if((keyname=addholdvalues(tmp,"FINMGT",
				filenames->libs[x],FALSE))!=NULL)
			{
				if(ezrpt->incvir!=NULL)
				{
					ezrpt->incvir=Rrealloc(ezrpt->incvir,
						(ezrpt->num+1)*sizeof(DFincvir));
				} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
				i=ezrpt->incvir+ezrpt->num;
				i->module=stralloc("FINMGT");
				i->file=stralloc(filenames->libs[x]);
				i->keyname=stralloc(keyname);
				++ezrpt->num;
				Rfree(keyname);
			}
			}
		}
		}
	}
	if(ezrpt->account_report)
	{
		no_parts=0;
		if(RDAstrcmp(ezrpt->majfil,"FINBACC") && RDAstrcmp(ezrpt->majfil,"FINBYR")
			&& RDAstrcmp(ezrpt->majfil,"FINBACT") && 
			RDAstrcmp(ezrpt->majfil,"FINRACC") && RDAstrcmp(ezrpt->majfil,"FINRYR")
			&& RDAstrcmp(ezrpt->majfil,"FINRACT"))
		{
			if(EXPENDITURE_ACCOUNT!=NULL)
			{
				if(EXPENDITURE_ACCOUNT->codetype!=NULL)
				{
					for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;
						z<EXPENDITURE_ACCOUNT->num;++z,++acc)
					{
						if(acc->num>no_parts) no_parts=acc->num;
					}
				}
			}
		}
		if(RDAstrcmp(ezrpt->majfil,"FINBACC") && RDAstrcmp(ezrpt->majfil,"FINBYR")
			&& RDAstrcmp(ezrpt->majfil,"FINBACT") && 
			RDAstrcmp(ezrpt->majfil,"FINEACC") && RDAstrcmp(ezrpt->majfil,"FINEYR")
			&& RDAstrcmp(ezrpt->majfil,"FINEACT"))
		{
			if(REVENUE_ACCOUNT!=NULL)
			{
				if(REVENUE_ACCOUNT->codetype!=NULL)
				{
					for(z=0,acc=REVENUE_ACCOUNT->codetype;
						z<REVENUE_ACCOUNT->num;++z,++acc)
					{
						if(acc->num>no_parts) no_parts=acc->num;
					}
				}
			}
		}
		if(RDAstrcmp(ezrpt->majfil,"FINEACC") && RDAstrcmp(ezrpt->majfil,"FINEYR")
			&& RDAstrcmp(ezrpt->majfil,"FINEACT") && 
			RDAstrcmp(ezrpt->majfil,"FINRACC") && RDAstrcmp(ezrpt->majfil,"FINRYR")
			&& RDAstrcmp(ezrpt->majfil,"FINRACT"))
		{
			if(BALANCE_SHEET_ACCOUNT!=NULL)
			{
				if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
				{
					for(z=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
						z<BALANCE_SHEET_ACCOUNT->num;++z,++acc)
					{
						if(acc->num>no_parts) no_parts=acc->num;
					}
				}
			}
		}
		for(z=0;z<no_parts;++z)
		{
			if((keyname=addholdvalues(tmp,"FINMGT",
				"FINCOA",TRUE))!=NULL)
			{
				if(ezrpt->incvir!=NULL)
				{
					ezrpt->incvir=Rrealloc(ezrpt->incvir,
						(ezrpt->num+1)*sizeof(DFincvir));
				} else ezrpt->incvir=Rmalloc(sizeof(DFincvir));
				i=ezrpt->incvir+ezrpt->num;
				i->module=stralloc("FINMGT");
				i->file=stralloc("FINCOA");
				i->keyname=stralloc(keyname);
				++ezrpt->num;
				Rfree(keyname);
			}
		}
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(holdmodule)+10);
	tmpx=adddashes(holdmodule);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
	filenames=getunsortedlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(filenames!=NULL)
	{
#ifdef RS6000
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN","Collecting Appropriate Virtual Fields",NULL,filenames->numlibs);
#else
		tmprsrc=NULL;
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN","Collecting Appropriate Virtual Fields",NULL,filenames->numlibs);
#endif
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
		}
		for(y=0;y<filenames->numlibs;++y)
		{
			d=getDFvirtual(holdmodule,filenames->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
				if(!RDAstrcmp(d->incvir->module,holdmodule) &&  
				   d->incvir->file==NULL) 
				{
					addvf=TRUE;
				} else {
					if(!d->vtype)
					{
						for(z=0,a=d->incvir;z<d->num;++z,++a)
						{
							for(x=0,i=ezrpt->incvir;x<ezrpt->num;++x,++i)
							{
								if(!RDAstrcmp(i->module,a->module) && 
									!RDAstrcmp(i->file,a->file)) break;
							}
							if(x>=ezrpt->num) 
							{
								addvf=FALSE;
								break;
							}
							if(!addvf) break;
						}
					} else {
						for(z=0,a=d->incvir;z<d->num;++z,++a)
						{
							for(x=0,i=ezrpt->incvir;x<ezrpt->num;++x,++i)
							{
								if(!RDAstrcmp(i->module,a->module) && 
									!RDAstrcmp(i->file,a->file)) break;
							}
							if(x<ezrpt->num) break;
						}
						if(z<d->num) addvf=TRUE;
							else addvf=FALSE;
					}
				}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					if(addAPPlibNoDuplicates(tmp,libx)!=(-1))
					{
					if(holdfields!=NULL)
					{
						holdfields=Rrealloc(holdfields,(numfields+1)*sizeof(EZfield));
					} else {
						holdfields=Rmalloc(sizeof(EZfield));
						numfields=0;
					}
					ezf=holdfields+numfields;
					if(d->name!=NULL)
					{
						ezf->name=stralloc(d->name);
					} else ezf->name=NULL;	
					ezf->type=d->type;
					ezf->len=d->length;
					++numfields;
					ezf->wholename=stralloc(libx);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
					} else {
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
					}
					if(libx!=NULL) Rfree(libx);
				} else {
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
				}
				FreeDFvirtual(d);
				d=NULL;
			}
		}
		freeapplib(filenames);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	addAPPlibNoDuplicates(tmp,"[PAGE NO]");
	addAPPlibNoDuplicates(tmp,"[BODY COUNT]");
	addAPPlibNoDuplicates(tmp,"[BREAK LEVEL]");
	addAPPlibNoDuplicates(tmp,"[PRIMARY FILE NUMBER]");
	addAPPlibNoDuplicates(tmp,"[WRITE COUNT]");
	loadglobals(tmp);

#ifdef ALLOW_INTERNAL_VIRTUALS
	if(ezrpt->detail!=NULL)
	{
		y=0;
		for(x=0,ezdtlfields=ezrpt->detail;x<ezrpt->numdtl;++x,++ezdtlfields)
		{
			if(ezdtlfields->type!=0) 
			{
				temp=Rmalloc(16+6+1);
				sprintf(temp,"[VIRTUAL FIELD #%d]",y);
				addAPPlibNoDuplicates(tmp,temp);
				if(temp!=NULL) Rfree(temp);
				++y;
			}
		}
	}
#endif /* ALLOW_INTERNAL_VIRTUALS */

/*
	if(tmp->numlibs<1)
	{
		addAPPlib(tmp,"No Field Values Available");
	} else SORTAPPlib(tmp);
*/
	SORTAPPlib(tmp);
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;
	EZfield *f;

	ldvalrsrc=RDArsrcNEW(module,"LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=APPlibNEW();
	if(holdfields!=NULL)
	{
		for(x=0,f=holdfields;x<numfields;++x,++f)
		{
			addAPPlibNoDuplicates(loadlist,f->wholename);
		}
	}
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	} else SORTAPPlib(loadlist);
	x=0;
	if(loadlist->numlibs>1)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP",13)) break;
		}
	}
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void setmajorfile(RDArsrc *rptrsrc)
{
	int selectedm=0,selectedf=0;
	char skip_finmgt=FALSE;

	if(FINDRSCGETINT(rptrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(rptrsrc,"FILE LIST",&selectedf)) return;
	FINDRSCGETCHAR(rptrsrc,"SKIP FINMGT",&skip_finmgt);
	if(holdmodule!=NULL) Rfree(holdmodule);
	holdmodule=stralloc(modlstx->libs[selectedm]);
	if(holdfile!=NULL) Rfree(holdfile);
	holdfile=stralloc(file_defs->libs[selectedf]);
	if(fieldlst!=NULL) freeapplib(fieldlst);
	fieldlst=makefieldvallist(skip_finmgt);
}
static void makesortlist()
{
	int l=0;
	short x;
	RDAezsrt *fields;
	char *s;

	if(sortlst!=NULL) freeapplib(sortlst);
	sortlst=APPlibNEW();
	if(ezrpt->sort!=NULL)
	{
		for(x=0,fields=ezrpt->sort;x<ezrpt->numsrt;++x,++fields)
		{
			l=(fields->total!=(-1) ? RDAstrlen(totals->libs[fields->total]):14)+RDAstrlen(fields->name)+45;
			s=Rmalloc(l);
			sprintf(s,"Name: [%s] Total: [%s] Start [%d] Length: [%d]",
				(fields->name!=NULL ? fields->name:""),
				(fields->total!=(-1) ? totals->libs[fields->total]:"Not Available"),
				fields->start,fields->length);
			addAPPlib(sortlst,s);
			Rfree(s);
		}
	}
	if(sortlst->numlibs<1)
	{
		addAPPlib(sortlst,"No Sorts Defined");
	}
}
static void dofilelist(RDArsrc *rptrsrc,short *init)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(rptrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(rptrsrc,"FILE LIST",&selectedf);
	if(file_defs!=NULL) freeapplib(file_defs);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
	file_defs=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(file_defs==NULL)
	{
		file_defs=APPlibNEW();
		addAPPlib(file_defs,"Contains No File Definitions");
	}
	if(selectedf>=file_defs->numlibs) selectedf=0;
	holdmodule=stralloc(modlstx->libs[selected]);
	holdfile=stralloc(file_defs->libs[selectedf]);
	if(!FINDRSCLISTAPPlib(rptrsrc,"FILE LIST",selectedf,file_defs))
		updatersrc(rptrsrc,"FILE LIST");
	if(*init) setmajorfile(rptrsrc);
}
static void makespacesorts(int pos)
{
	short x,y;
	RDAezsrt *temp,*temp1,*new_fields;

	++ezrpt->numsrt;
	new_fields=Rmalloc(ezrpt->numsrt*sizeof(RDAezsrt));
	y=0;
	for(x=0,temp=ezrpt->sort;x<(ezrpt->numsrt-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->total=0;
			temp1->start=0;
			temp1->length=0;
			temp1->type=(-1);
			temp1->range=FALSE;
			++y;
			temp1=new_fields+y;
		}
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->length=temp->length;
		temp1->start=temp->start;
		temp1->total=temp->total;
		temp1->range=temp->range;
		switch(temp->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(temp->from.string_value!=NULL)
				{
					temp1->from.string_value=stralloc(temp->from.string_value);
					Rfree(temp->from.string_value);
				} else temp1->from.string_value=NULL;
				if(temp->to.string_value!=NULL)
				{
					temp1->to.string_value=stralloc(temp->to.string_value);
					Rfree(temp->to.string_value);
				} else temp1->to.string_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(temp1->from.string_value==NULL)
					temp1->from.string_value=Rmalloc(1);
				*temp1->from.string_value=*temp->from.string_value;
				Rfree(temp->from.string_value);
				if(temp1->to.string_value==NULL)
					temp1->to.string_value=Rmalloc(1);
				*temp1->to.string_value=*temp->to.string_value;
				Rfree(temp->to.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(temp1->from.float_value==NULL) 
					temp1->from.float_value=Rmalloc(sizeof(double));
				*temp1->from.float_value=*temp->from.float_value;
				if(temp->from.float_value!=NULL) Rfree(temp->from.float_value);
				if(temp1->to.float_value==NULL)
					temp1->to.float_value=Rmalloc(sizeof(double));
				*temp1->to.float_value=*temp->to.float_value;
				if(temp->to.float_value!=NULL) Rfree(temp->to.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				if(temp1->from.short_value==NULL) 
					temp1->from.short_value=Rmalloc(sizeof(short));
				*temp1->from.short_value=*temp->from.short_value;
				if(temp->from.short_value!=NULL) Rfree(temp->from.short_value);
				if(temp1->to.short_value==NULL)
					temp1->to.short_value=Rmalloc(sizeof(short));
				*temp1->to.short_value=*temp->to.short_value;
				if(temp->to.short_value!=NULL) Rfree(temp->to.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(temp1->from.integer_value==NULL) 
					temp1->from.integer_value=Rmalloc(sizeof(int));
				*temp1->from.integer_value=*temp->from.integer_value;
				if(temp->from.integer_value!=NULL) 
					Rfree(temp->from.integer_value);
				if(temp1->to.integer_value==NULL)
					temp1->to.integer_value=Rmalloc(sizeof(int));
				*temp1->to.integer_value=*temp->to.integer_value;
				if(temp->to.integer_value!=NULL) Rfree(temp->to.integer_value);
				break;
			default:
				prterr("Error field type [%d] not found for field [%s].",
					temp->type,temp->name);
				break;
		}
		++y;
	}
	Rfree(ezrpt->sort);
	ezrpt->sort=Rmalloc(ezrpt->numsrt*sizeof(RDAezsrt));
	for(x=0,temp1=ezrpt->sort,temp=new_fields;x<ezrpt->numsrt;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->length=temp->length;
		temp1->start=temp->start;
		temp1->total=temp->total;
		temp1->range=temp->range;
		switch(temp->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(temp->from.string_value!=NULL)
				{
					temp1->from.string_value=stralloc(temp->from.string_value);
					Rfree(temp->from.string_value);
				} else temp1->from.string_value=NULL;
				if(temp->to.string_value!=NULL)
				{
					temp1->to.string_value=stralloc(temp->to.string_value);
					Rfree(temp->to.string_value);
				} else temp1->to.string_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(temp1->from.string_value==NULL)
					temp1->from.string_value=Rmalloc(1);
				*temp1->from.string_value=*temp->from.string_value;
				Rfree(temp->from.string_value);
				if(temp1->to.string_value==NULL)
					temp1->to.string_value=Rmalloc(1);
				*temp1->to.string_value=*temp->to.string_value;
				Rfree(temp->to.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(temp1->from.float_value==NULL) 
					temp1->from.float_value=Rmalloc(sizeof(double));
				*temp1->from.float_value=*temp->from.float_value;
				if(temp->from.float_value!=NULL) Rfree(temp->from.float_value);
				if(temp1->to.float_value==NULL)
					temp1->to.float_value=Rmalloc(sizeof(double));
				*temp1->to.float_value=*temp->to.float_value;
				if(temp->to.float_value!=NULL) Rfree(temp->to.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				if(temp1->from.short_value==NULL) 
					temp1->from.short_value=Rmalloc(sizeof(short));
				*temp1->from.short_value=*temp->from.short_value;
				if(temp->from.short_value!=NULL) Rfree(temp->from.short_value);
				if(temp1->to.short_value==NULL)
					temp1->to.short_value=Rmalloc(sizeof(short));
				*temp1->to.short_value=*temp->to.short_value;
				if(temp->to.short_value!=NULL) Rfree(temp->to.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(temp1->from.integer_value==NULL) 
					temp1->from.integer_value=Rmalloc(sizeof(int));
				*temp1->from.integer_value=*temp->from.integer_value;
				if(temp->from.integer_value!=NULL) 
					Rfree(temp->from.integer_value);
				if(temp1->to.integer_value==NULL)
					temp1->to.integer_value=Rmalloc(sizeof(int));
				*temp1->to.integer_value=*temp->to.integer_value;
				if(temp->to.integer_value!=NULL) Rfree(temp->to.integer_value);
				break;
			default:
				prterr("Error field type [%d] not found for field [%s].",
					temp->type,temp->name);
				break;
		}
	}
	Rfree(new_fields);
}
static void quitsort(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	if(sortrsrc!=NULL)
	{
		killwindow(sortrsrc);
		free_rsrc(sortrsrc);
	}
	if(changedsorts) changedezrpt=TRUE;
	if(sortdefaults!=NULL) FreeRDAdefault(sortdefaults);
}
static void quitsortcls(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	changedsorts=FALSE;
	quitsort(sortrsrc,rptrsrc);
}
static void quitsorttest(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	readallwidgets(sortrsrc);
	if(COMPARE_RSRCS(sortrsrc,sortdefaults,2) || changedsorts)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the SORT VALUES.\nDo you want to Save these changes?",savesort,quitsortcls,FALSE,2,sortrsrc,rptrsrc,NULL);
	} else {
		quitsort(sortrsrc,rptrsrc);
	}
}
static void save_sort_above(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	int x;

	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&x)) return;
	makespacesorts(x);
	savesort(sortrsrc,rptrsrc);
}
static void save_sort_below(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	int x;

	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&x)) return;
	makespacesorts(++x);
	FINDRSCSETINT(rptrsrc,"SORT LIST",x);
	updatersrc(rptrsrc,"SORT LIST");
	savesort(sortrsrc,rptrsrc);
}
static void quitsortabovetest(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	readallwidgets(sortrsrc);
	if(COMPARE_RSRCS(sortrsrc,sortdefaults,2) || changedsorts)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the SORT VALUES.\nDo you want to Save these changes?",save_sort_above,quitsortcls,FALSE,2,sortrsrc,rptrsrc,NULL);
	} else {
		quitsort(sortrsrc,rptrsrc);
	}
}
static void quitsortbelowtest(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	readallwidgets(sortrsrc);
	if(COMPARE_RSRCS(sortrsrc,sortdefaults,2) || changedsorts)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the SORT VALUES.\nDo you want to Save these changes?",save_sort_below,quitsortcls,FALSE,2,sortrsrc,rptrsrc,NULL);
	} else {
		quitsort(sortrsrc,rptrsrc);
	}
}
static void savesort(RDArsrc *sortrsrc,RDArsrc *rptrsrc)
{
	int x,value;
	short first_type=0;
	RDAezsrt *sort;
	char range=FALSE,*from=NULL,*to=NULL;

	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&value)) return;
	sort=ezrpt->sort+value;
	first_type=sort->type;
	readallwidgets(sortrsrc);
	FINDRSCGETINT(sortrsrc,"FIELD LIST",&x);
	sort->name=stralloc(fieldlst->libs[x]);
	FINDRSCGETINT(sortrsrc,"TOTAL TYPES",&x);
	sort->total=(short)x;
	FINDRSCGETSHORT(sortrsrc,"START",&sort->start);
	FINDRSCGETSHORT(sortrsrc,"LENGTH",&sort->length);
	FINDRSCGETCHAR(sortrsrc,"RANGE",&range);
	sort->range=range;
	FINDRSCGETSTRING(sortrsrc,"FROM",&from);
	FINDRSCGETSTRING(sortrsrc,"TO",&to);
	x=0;
	FINDRSCGETINT(sortrsrc,"FIELD TYPES",&x);
	sort->type=(short)x;
	switch(sort->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			if(first_type!=(-1))
			{
				if(sort->from.string_value!=NULL) Rfree(sort->from.string_value);
				if(sort->to.string_value!=NULL) Rfree(sort->to.string_value);
			}
			if(from!=NULL) 
			{
				sort->from.string_value=stralloc(from);
				Rfree(from);
			} else sort->from.string_value=NULL;
			if(to!=NULL)
			{
				sort->to.string_value=stralloc(to);
				Rfree(to);
			} else sort->to.string_value=NULL;
			break;
		case BOOLNS:
			sort->total=3;
		case CHARACTERS:
			if(sort->from.string_value==NULL || first_type==(-1))
				sort->from.string_value=Rmalloc(1);
			if(from!=NULL)
			{
				if(sort->type==BOOLNS)
				{
					if(from[0]=='n' || from[0]=='N' || from[0]=='0')
					{
						*sort->from.string_value=FALSE;
					} else *sort->from.string_value=TRUE;
				} else *sort->from.string_value=from[0];
				Rfree(from);
			} else *sort->from.string_value=0;
			if(sort->to.string_value==NULL || first_type==(-1)) 
				sort->to.string_value=Rmalloc(1);
			if(to!=NULL)
			{
				if(sort->type==BOOLNS)
				{
					if(to[0]=='n' || to[0]=='N' || to[0]=='0')
					{
						*sort->to.string_value=FALSE;
					} else *sort->to.string_value=TRUE;
				} else *sort->to.string_value=to[0];
				Rfree(to);
			} else *sort->to.string_value=0;
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			if(first_type==(-1) || sort->from.float_value==NULL)
			{
				sort->from.float_value=Rmalloc(sizeof(double));
			}
			*sort->from.float_value=atof(from);
			if(first_type==(-1) || sort->to.float_value==NULL)
			{
				sort->to.float_value=Rmalloc(sizeof(double));
			}
			*sort->to.float_value=atof(to);
			break;
		case SHORTV:
		case SSHORTV:
			if(first_type==(-1) || sort->from.short_value==NULL)
			{
				sort->from.short_value=Rmalloc(sizeof(short));
			}
			*sort->from.short_value=(short)Ratoi(from);
			if(first_type==(-1) || sort->to.short_value==NULL)
			{
				sort->to.short_value=Rmalloc(sizeof(short));
			}
			*sort->to.short_value=(short)Ratoi(to);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(first_type==(-1) || sort->from.integer_value==NULL)
			{
				sort->from.integer_value=Rmalloc(sizeof(int));
			}
			*sort->from.integer_value=Ratoi(from);
			if(first_type==(-1) || sort->to.integer_value==NULL)
			{
				sort->to.integer_value=Rmalloc(sizeof(int));
			}
			*sort->to.integer_value=Ratoi(to);
			break;
		default:
			prterr("Error field type [%d] not found for field [%s].",
				sort->type,sort->name);
			break;
	}
	makesortlist();
	if(!FINDRSCLISTAPPlib(rptrsrc,"SORT LIST",value,sortlst))
		updatersrc(rptrsrc,"SORT LIST");
	if(COMPARE_RSRCS(sortrsrc,sortdefaults,2) || changedsorts)
	{
		changedsorts=TRUE;
	}
	quitsort(sortrsrc,rptrsrc);
}
static void estfieldvalues(RDArsrc *sortrsrc)
{
	short start=0,length=0;
	char *from=NULL,*to=NULL;
	int x,selected=0,ftype=0;
	EZfield *f;
	
	if(FINDRSCGETINT(sortrsrc,"FIELD LIST",&selected)) return;
	for(x=0,f=holdfields;x<numfields;++x,++f)
	{
		if(!RDAstrcmp(f->wholename,fieldlst->libs[selected])) break;
	}	
	if(x<numfields)
	{
		ftype=f->type;
		if(!FINDRSCSETINT(sortrsrc,"FIELD TYPES",ftype))
			updatersrc(sortrsrc,"FIELD TYPES");
		if(!FINDRSCSETSHORT(sortrsrc,"START",start))
			updatersrc(sortrsrc,"START");
		length=f->len;
		if(!FINDRSCSETSHORT(sortrsrc,"LENGTH",length))
			updatersrc(sortrsrc,"LENGTH");
		switch(f->type)
		{
		case BOOLNS:
			if(f->len>0)
			{
				from=Rmalloc(f->len+1);
				to=Rmalloc(f->len+1);
				sprintf(from,"%c",'N');
				sprintf(to,"%c",'Y');
			} else {
				from=Rmalloc(2);
				to=Rmalloc(2);
				sprintf(from,"%c",'N');
				sprintf(to,"%c",'Y');
			}
			break;
		case CHARACTERS:
			if(f->len>0)
			{
				from=NULL;
				to=Rmalloc(f->len+1);
				memset(to,'~',f->len);
			} else {
				from=NULL;
				to=Rmalloc(2);
				to[0]='~';
			}
			break;
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case SDECIMALV:
			if(f->len>0)
			{
				from=Rmalloc(f->len+1);
				to=Rmalloc(f->len+1);
				sprintf(from,"%*d",f->len,0);
				memset(to,'9',f->len);
			} else {
				from=Rmalloc(21);
				sprintf(from,"%20d",0);
				to=Rmalloc(21);
				memset(to,'9',20);
			}
			break;
		case SHORTV:
		case SSHORTV:
			if(f->len>0)
			{
				from=Rmalloc(f->len+1);
				to=Rmalloc(f->len+1);
				sprintf(from,"%*d",f->len,0);
				memset(to,'9',f->len);
			} else {
				from=Rmalloc(5);
				sprintf(from,"%4d",0);
				to=Rmalloc(5);
				memset(to,'9',4);
			}
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(f->len>0)
			{
				from=Rmalloc(f->len+1);
				to=Rmalloc(f->len+1);
				sprintf(from,"%*d",f->len,0);
				memset(to,'9',f->len);
			} else {
				from=Rmalloc(11);
				sprintf(from,"%10d",0);
				to=Rmalloc(11);
				memset(to,'9',10);
			}
			break;
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			if(f->len>0)
			{
				from=NULL;
				to=Rmalloc(f->len+1);
				memset(to,'~',f->len);
			} else {
				from=NULL;
				to=Rmalloc(41);
				memset(to,'~',40);
			}
			break;
		default:
			prterr("Error invalid field type.");
			break;
		}
		if(!FINDRSCSETSTRING(sortrsrc,"FROM",from))
			updatersrc(sortrsrc,"FROM");
		if(!FINDRSCSETSTRING(sortrsrc,"TO",to))
			updatersrc(sortrsrc,"TO");
		if(from!=NULL) Rfree(from);
		if(to!=NULL) Rfree(to);
	}
}
static void setrangefunc(RDArsrc *sortrsrc)
{
	char range=FALSE;

	readwidget(sortrsrc,"RANGE");
	FINDRSCGETCHAR(sortrsrc,"RANGE",&range);
	if(range==FALSE)
	{
		FINDRSCSETEDITABLE(sortrsrc,"FROM",FALSE);
		FINDRSCSETEDITABLE(sortrsrc,"TO",FALSE);
		FINDRSCSETSENSITIVE(sortrsrc,"FROM",FALSE);
		FINDRSCSETSENSITIVE(sortrsrc,"TO",FALSE);
	} else {
		FINDRSCSETEDITABLE(sortrsrc,"FROM",TRUE);
		FINDRSCSETEDITABLE(sortrsrc,"TO",TRUE);
		FINDRSCSETSENSITIVE(sortrsrc,"FROM",TRUE);
		FINDRSCSETSENSITIVE(sortrsrc,"TO",TRUE);
	}
}
static void sortscreen(RDArsrc *rptrsrc,char *name,short total,short start,
	short length,short type,char range,void *from,void *to,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0;
	char *from1=NULL,*to1=NULL;
	RDArsrc *sortrsrc=NULL;

	sortrsrc=RDArsrcNEW(module,"EZ MAINTAIN SORT");
	for(x=0;x<fieldlst->numlibs;++x)
	{
		if(!RDAstrcmp(fieldlst->libs[x],name)) break;
	}
	if(fieldlst->numlibs<=x) x=0;
	addlstrsrc(sortrsrc,"FIELD LIST",&x,TRUE,estfieldvalues,
		fieldlst->numlibs,&fieldlst->libs,NULL);
	x=total;
	addlstrsrc(sortrsrc,"TOTAL TYPES",&x,TRUE,NULL,
		totals->numlibs,&totals->libs,NULL);
	if(type!=(-1))
	{
		x=type;
	}
	addlstrsrc(sortrsrc,"FIELD TYPES",&x,FALSE,NULL,ftypes->numlibs,
		&ftypes->libs,NULL);
	addstdrsrc(sortrsrc,"RANGE",BOOLNS,1,&range,TRUE);
	FINDRSCSETFUNC(sortrsrc,"RANGE",setrangefunc,NULL);
/* these are disabled, need to change rptconv */
/* I think though it to make this work, would further confuse dfrpt.c to the user */
/* dfrpt.c is more important for them to truly realize Xpert's ability */
	addstdrsrc(sortrsrc,"START",SHORTV,3,&start,TRUE);
	addstdrsrc(sortrsrc,"LENGTH",SHORTV,3,&length,TRUE);
	switch(type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case ZIPCODE:
		case PHONE:
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			if(from!=NULL) from1=stralloc(from);
				else from1=NULL;
			if(to!=NULL) to1=stralloc(to);
				else to1=NULL;
			break;
		case BOOLNS:
			from1=Rmalloc(2);
			to1=Rmalloc(2);
			sprintf(from1,"%c",(*(char *)from ? 'Y':'N'));
			sprintf(to1,"%c",(*(char *)to ? 'Y':'N'));
			break;
		case CHARACTERS:
			from1=Rmalloc(2);
			to1=Rmalloc(2);
			sprintf(from1,"%c",*(char *)from);
			sprintf(to1,"%c",*(char *)to);
			break;
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case SDECIMALV:
			if(from!=NULL) sprintf(stemp,"%*.0f",length,*(double *)from);
				else sprintf(stemp,"%*.0f",length,(double)0);
			from1=stralloc(stemp);
			memset(stemp,0,RDAstrlen(stemp));
			if(to!=NULL) sprintf(stemp,"%*.0f",length,*(double *)to);
				else sprintf(stemp,"%*.0f",length,(double)0);
			to1=stralloc(stemp);
			memset(stemp,0,RDAstrlen(stemp));
			break;
		case SHORTV:
		case SSHORTV:
			if(from!=NULL) sprintf(stemp,"%*d",length,*(short *)from);
				else sprintf(stemp,"%*d",length,0);
			from1=stralloc(stemp);
			memset(stemp,0,RDAstrlen(stemp));
			if(to!=NULL) sprintf(stemp,"%*d",length,*(short *)to);
				else sprintf(stemp,"%*d",length,(short)0);
			to1=stralloc(stemp);
			memset(stemp,0,RDAstrlen(stemp));
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(from!=NULL) sprintf(stemp,"%*d",length,*(int *)from);
				else sprintf(stemp,"%*d",length,(int)0);
			from1=stralloc(stemp);
			memset(stemp,0,RDAstrlen(stemp));
			if(to!=NULL) sprintf(stemp,"%*d",length,*(int *)to);
				else sprintf(stemp,"%*d",length,(int)0);
			to1=stralloc(stemp);
			memset(stemp,0,RDAstrlen(stemp));
			break;
		default:
			prterr("field type [%d] not found for field [%s].",type,name);
			break;
	}
	addstdrsrc(sortrsrc,"FROM",VARIABLETEXT,0,from1,TRUE);
	addstdrsrc(sortrsrc,"TO",VARIABLETEXT,0,to1,TRUE);
	if(from1!=NULL) Rfree(from1);
	from1=NULL;
	if(to1!=NULL) Rfree(to1);
	to1=NULL;
	addrfcbrsrc(sortrsrc,"SAVE",TRUE,savefunc,rptrsrc);
	addrfcbrsrc(sortrsrc,"QUIT",TRUE,quitsorttest,rptrsrc);
	addbtnrsrc(sortrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(sortrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(name==NULL || (from==NULL && to==NULL)) estfieldvalues(sortrsrc);
	setrangefunc(sortrsrc);
	changedsorts=FALSE;
	sortdefaults=GetDefaults(sortrsrc);
	APPmakescrn(sortrsrc,TRUE,quitsort,rptrsrc,FALSE);
}
static void add_sort_above(RDArsrc *rptrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&selected)) return;
	if(sortlst->numlibs==1 && !RDAstrcmp(sortlst->libs[0],"No Sorts Defined")) return;
	sortscreen(rptrsrc,NULL,0,0,0,0,FALSE,NULL,NULL,save_sort_above,quitsortabovetest);
}
static void add_sort_below(RDArsrc *rptrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&selected)) return;
	if(sortlst->numlibs==1 && !RDAstrcmp(sortlst->libs[0],"No Sorts Defined")) return;
	sortscreen(rptrsrc,NULL,0,0,0,0,FALSE,NULL,NULL,save_sort_below,quitsortbelowtest);
}
static void copy_sort(RDArsrc *rptrsrc)
{
	int x;
	RDAezsrt *f;

	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&x)) return;
	if(ezrpt->sort==NULL)
	{
		ezrpt->sort=Rmalloc(sizeof(RDAezsrt));
		f=ezrpt->sort;
		f->name=NULL;
		f->total=0;
		f->start=0;
		f->length=0;
		f->type=(-1);
		f->range=0;
		ezrpt->numsrt=1;
		sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,f->range,NULL,NULL,save_sort_below,quitsortbelowtest);
	} else {
	f=ezrpt->sort+x;
	switch(f->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
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
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.string_value,f->to.string_value,save_sort_below,quitsortbelowtest);
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.float_value,f->to.float_value,save_sort_below,quitsortbelowtest);
			break;
		case SHORTV:
		case SSHORTV:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.short_value,f->to.short_value,save_sort_below,quitsortbelowtest);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.integer_value,f->to.integer_value,save_sort_below,quitsortbelowtest);
			break;
		default:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,f->range,NULL,NULL,save_sort_below,quitsortbelowtest);
			break;
	}
	}
}
static void edit_sort(RDArsrc *rptrsrc)
{
	int x;
	RDAezsrt *f;

	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&x)) return;
	if(ezrpt->sort==NULL)
	{
		ezrpt->sort=Rmalloc(sizeof(RDAezsrt));
		f=ezrpt->sort;
		f->name=NULL;
		f->total=0;
		f->start=0;
		f->length=0;
		f->type=(-1);
		f->range=0;
		ezrpt->numsrt=1;
		sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,f->range,NULL,NULL,savesort,quitsorttest);
	} else {
	f=ezrpt->sort+x;
	switch(f->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
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
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.string_value,f->to.string_value,savesort,quitsorttest);
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.float_value,f->to.float_value,savesort,quitsorttest);
			break;
		case SHORTV:
		case SSHORTV:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.short_value,f->to.short_value,savesort,quitsorttest);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,
				f->range,f->from.integer_value,f->to.integer_value,savesort,quitsorttest);
			break;
		default:
			sortscreen(rptrsrc,f->name,f->total,f->start,f->length,f->type,f->range,NULL,NULL,savesort,quitsorttest);
			break;
	}
	}
}
static void delete_sort(RDArsrc *rptrsrc)
{
	short x,y;
	RDAezsrt *temp,*temp1,*new_fields;
	int value;

	readallwidgets(rptrsrc);
	if(FINDRSCGETINT(rptrsrc,"SORT LIST",&value)) return;
	if(ezrpt->numsrt==1) return;
	new_fields=Rmalloc((ezrpt->numsrt-1)*sizeof(RDAezsrt));
	y=0;
	for(x=0,temp=ezrpt->sort;x<ezrpt->numsrt;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			temp1->type=temp->type;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->length=temp->length;
			temp1->start=temp->start;
			temp1->total=temp->total;
			temp1->range=temp->range;
			switch(temp->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(temp->from.string_value!=NULL)
					{
						temp1->from.string_value=stralloc(temp->from.string_value);
						Rfree(temp->from.string_value);
					} else temp1->from.string_value=NULL;
					if(temp->to.string_value!=NULL)
					{
						temp1->to.string_value=stralloc(temp->to.string_value);
						Rfree(temp->to.string_value);
					} else temp1->to.string_value=NULL;
					break;
				case BOOLNS:
				case CHARACTERS:
					temp1->from.string_value=Rmalloc(1);
					*temp1->from.string_value=*temp->from.string_value;
					Rfree(temp->from.string_value);
					temp1->to.string_value=Rmalloc(1);
					*temp1->to.string_value=*temp->to.string_value;
					Rfree(temp->to.string_value);
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					temp1->from.float_value=Rmalloc(sizeof(double));
					*temp1->from.float_value=*temp->from.float_value;
					if(temp->from.float_value!=NULL) 
						Rfree(temp->from.float_value);
					temp1->to.float_value=Rmalloc(sizeof(double));
					*temp1->to.float_value=*temp->to.float_value;
					if(temp->to.float_value!=NULL) Rfree(temp->to.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					temp1->from.short_value=Rmalloc(sizeof(short));
					*temp1->from.short_value=*temp->from.short_value;
					if(temp->from.short_value!=NULL) 
						Rfree(temp->from.short_value);
					temp1->to.short_value=Rmalloc(sizeof(short));
					*temp1->to.short_value=*temp->to.short_value;
					if(temp->to.short_value!=NULL) 
						Rfree(temp->to.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					temp1->from.integer_value=Rmalloc(sizeof(int));
					*temp1->from.integer_value=*temp->from.integer_value;
					if(temp->from.integer_value!=NULL) 
						Rfree(temp->from.integer_value);
					temp1->to.integer_value=Rmalloc(sizeof(int));
					*temp1->to.integer_value=*temp->to.integer_value;
					if(temp->to.integer_value!=NULL) 
						Rfree(temp->to.integer_value);
					break;
				default:
					prterr("field type [%d] not found for field [%s].",
						temp->type,temp->name);
					break;
			}
			++y;
		}
	}
	Rfree(ezrpt->sort);
	--ezrpt->numsrt;
	ezrpt->sort=Rmalloc(ezrpt->numsrt*sizeof(RDAezsrt));
	for(x=0,temp1=ezrpt->sort,temp=new_fields;x<ezrpt->numsrt;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->length=temp->length;
		temp1->start=temp->start;
		temp1->total=temp->total;
		temp1->range=temp->range;
		switch(temp->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(temp->from.string_value!=NULL)
				{
					temp1->from.string_value=stralloc(temp->from.string_value);
					Rfree(temp->from.string_value);
				} else temp1->from.string_value=NULL;
				if(temp->to.string_value!=NULL)
				{
					temp1->to.string_value=stralloc(temp->to.string_value);
					Rfree(temp->to.string_value);
				} else temp1->to.string_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				temp1->from.string_value=Rmalloc(1);
				*temp1->from.string_value=*temp->from.string_value;
				Rfree(temp->from.string_value);
				temp1->to.string_value=Rmalloc(1);
				*temp1->to.string_value=*temp->to.string_value;
				Rfree(temp->to.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				temp1->from.float_value=Rmalloc(sizeof(double));
				*temp1->from.float_value=*temp->from.float_value;
				if(temp->from.float_value!=NULL) Rfree(temp->from.float_value);
				temp1->to.float_value=Rmalloc(sizeof(double));
				*temp1->to.float_value=*temp->to.float_value;
				if(temp->to.float_value!=NULL) Rfree(temp->to.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				temp1->from.short_value=Rmalloc(sizeof(short));
				*temp1->from.short_value=*temp->from.short_value;
				if(temp->from.short_value!=NULL) Rfree(temp->from.short_value);
				temp1->to.short_value=Rmalloc(sizeof(short));
				*temp1->to.short_value=*temp->to.short_value;
				if(temp->to.short_value!=NULL) Rfree(temp->to.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				temp1->from.integer_value=Rmalloc(sizeof(int));
				*temp1->from.integer_value=*temp->from.integer_value;
				if(temp->from.integer_value!=NULL) 
					Rfree(temp->from.integer_value);
				temp1->to.integer_value=Rmalloc(sizeof(int));
				*temp1->to.integer_value=*temp->to.integer_value;
				if(temp->to.integer_value!=NULL) Rfree(temp->to.integer_value);
				break;
			default:
				prterr("field type [%d] not found for field [%s].",
					temp->type,temp->name);
				break;
		}
	}
	Rfree(new_fields);
	makesortlist();
	if(!FINDRSCLISTAPPlib(rptrsrc,"SORT LIST",value,sortlst))
		updatersrc(rptrsrc,"SORT LIST");
	changedezrpt=TRUE;
}
static void makedetaillist()
{
	int l=0;
	short x,y;
	RDAezdtl *fields;
	char *s=NULL;
	char *temp=NULL;

	if(detaillst!=NULL) freeapplib(detaillst);
	detaillst=APPlibNEW();
	if(ezrpt->detail!=NULL)
	{
		y=0;
		for(x=0,fields=ezrpt->detail;x<ezrpt->numdtl;++x,++fields)
		{
			if(fields->type!=0) 
			{
				temp=Rmalloc(17+6+1);
				sprintf(temp,"[VIRTUAL FIELD #%d]",y);
				++y;
			} else {
				temp=stralloc(fields->fieldname);
			}
			l=RDAstrlen(temp)+(fields->type ? RDAstrlen(fields->expression):RDAstrlen(fields->fieldname))+RDAstrlen(fields->title1)+RDAstrlen(fields->title2)+80;
			s=Rmalloc(l);
			sprintf(s,"Name: [%s] Title1: [%s] Title2: [%s] Start: [%d] Length: [%d] Value: [%s]",
				(temp!=NULL?temp:""),
				(fields->title1!=NULL ? fields->title1:""),
				(fields->title2!=NULL ? fields->title2:""),
				fields->start,fields->length,(fields->type ? 
					fields->expression:fields->fieldname));
			if(temp!=NULL) Rfree(temp);
/*
			l=(fields->type ? RDAstrlen(fields->expression):RDAstrlen(fields->fieldname))+RDAstrlen(fields->title1)+RDAstrlen(fields->title2)+57;
			s=Rmalloc(l);
			sprintf(s,"Title1: [%s] Title2: [%s] Start [%d] Length: [%d] Value [%s]",
				(fields->title1!=NULL ? fields->title1:""),
				(fields->title2!=NULL ? fields->title2:""),
				fields->start,fields->length,(fields->type ? 
					fields->expression:fields->fieldname));
*/
			addAPPlib(detaillst,s);
			Rfree(s);
		}
	}
	if(detaillst->numlibs<1)
	{
		addAPPlib(detaillst,"No Detail Defined");
	}
}
static void makespacedetails(int pos)
{
	short x,y;
	RDAezdtl *temp,*temp1,*new_fields;

	++ezrpt->numdtl;
	new_fields=Rmalloc(ezrpt->numdtl*sizeof(RDAezdtl));
	y=0;
	for(x=0,temp=ezrpt->detail;x<(ezrpt->numdtl-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->title1=NULL;
			temp1->title2=NULL;
			temp1->type=0;
			temp1->start=0;
			temp1->length=0;
			temp1->fieldname=NULL;
			temp1->expression=NULL;
			temp1->fieldtype=0;
			temp1->range=FALSE;
			++y;
			temp1=new_fields+y;
		}
		temp1->type=temp->type;
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
		temp1->start=temp->start;
		temp1->length=temp->length;
		if(temp->title1!=NULL)
		{
			temp1->title1=stralloc(temp->title1);
			Rfree(temp->title1);
		} else temp1->title1=NULL;
		if(temp->title2!=NULL)
		{
			temp1->title2=stralloc(temp->title2);
			Rfree(temp->title2);
		} else temp->title2=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->fieldtype=temp->fieldtype;
		temp1->range=temp->range;
		++y;
	}
	Rfree(ezrpt->detail);
	ezrpt->detail=Rmalloc(ezrpt->numdtl*sizeof(RDAezdtl));
	for(x=0,temp1=ezrpt->detail,temp=new_fields;x<ezrpt->numdtl;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
		temp1->start=temp->start;
		temp1->length=temp->length;
		if(temp->title1!=NULL)
		{
			temp1->title1=stralloc(temp->title1);
			Rfree(temp->title1);
		} else temp1->title1=NULL;
		if(temp->title2!=NULL)
		{
			temp1->title2=stralloc(temp->title2);
			Rfree(temp->title2);
		} else temp->title2=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->fieldtype=temp->fieldtype;
		temp1->range=temp->range;
	}
	Rfree(new_fields);
}
static void quitdetail(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	if(detailrsrc!=NULL)
	{
		killwindow(detailrsrc);
		free_rsrc(detailrsrc);
	}
	if(changeddetails) changedezrpt=TRUE;
	if(detaildefaults!=NULL) FreeRDAdefault(detaildefaults);
}
static void quitdetailcls(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	changeddetails=FALSE;
	quitdetail(detailrsrc,rptrsrc);
}
static void quitdetailtest(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	readallwidgets(detailrsrc);
	if(COMPARE_RSRCS(detailrsrc,detaildefaults,2) || changeddetails)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the DETAIL VALUES.\nDo you want to Save these changes?",savedetail,quitdetailcls,FALSE,2,detailrsrc,rptrsrc,NULL);
	} else {
		quitdetail(detailrsrc,rptrsrc);
	}
}
static void checksavedetail(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	short length=0;

	readallwidgets(detailrsrc);
	FINDRSCGETSHORT(detailrsrc,"LENGTH",&length);
	if(length<1)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","Legnth to Print is Zero!","Thelength entered is zero, cannot print a field with a printed length of zero.",savedetail,NULL,FALSE,2,detailrsrc,rptrsrc,NULL);
	} else {
		savedetail(detailrsrc,rptrsrc);
	}
}
static void savedetail(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	int x,value;
	RDAezdtl *detail;

	if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&value)) return;
	detail=ezrpt->detail+value;
	FINDRSCGETSTRING(detailrsrc,"TITLE1",&detail->title1);
	FINDRSCGETSTRING(detailrsrc,"TITLE2",&detail->title2);
	FINDRSCGETSHORT(detailrsrc,"START",&detail->start);
	FINDRSCGETSHORT(detailrsrc,"LENGTH",&detail->length);
	FINDRSCGETINT(detailrsrc,"TYPES",&x);
	detail->type=(short)x;
	FINDRSCGETCHAR(detailrsrc,"SET RANGE",&detail->range);
	FINDRSCGETINT(detailrsrc,"FIELD TYPES",&x);
	detail->fieldtype=(short)x;
	switch(detail->type)
	{
		default:
		case 0:
			FINDRSCGETINT(detailrsrc,"FIELD LIST",&x);
			detail->fieldname=stralloc(fieldlst->libs[x]);
			if(detail->expression!=NULL) Rfree(detail->expression);
			break;
		case 1:
			FINDRSCGETSTRING(detailrsrc,"EXPRESSION",&detail->expression);
			if(detail->fieldname!=NULL) Rfree(detail->fieldname);
			break;
	}
	makedetaillist();
	if(!FINDRSCLISTAPPlib(rptrsrc,"DETAIL LIST",value,detaillst))
		updatersrc(rptrsrc,"DETAIL LIST");
	if(COMPARE_RSRCS(detailrsrc,detaildefaults,2) || changeddetails)
	{
		changeddetails=TRUE;
	}
	quitdetail(detailrsrc,rptrsrc);
}
static void save_detail_above(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	int x;
	short length=0;

	readallwidgets(detailrsrc);
	FINDRSCGETSHORT(detailrsrc,"LENGTH",&length);
	if(length<1)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","Length to Print is Zero!","The length entered is zero, cannot print a field with a printed length of zero.",savedetail,NULL,FALSE,2,detailrsrc,rptrsrc,NULL);
	} else {
		if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&x)) return;
		makespacedetails(x);
		savedetail(detailrsrc,rptrsrc);
	}
}
static void save_detail_below(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	int x;
	short length=0;

	readallwidgets(detailrsrc);
	FINDRSCGETSHORT(detailrsrc,"LENGTH",&length);
	if(length<1)
	{
		ADVWARNINGDIALOG("WARNING DIALOG SCREEN","Legnth to Print is Zero!","Thelength entered is zero, cannot print a field with a printed length of zero.",savedetail,NULL,FALSE,2,detailrsrc,rptrsrc,NULL);
	} else {
		if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&x)) return;
		makespacedetails(++x);
		FINDRSCSETINT(rptrsrc,"DETAIL LIST",x);
		updatersrc(rptrsrc,"DETAIL LIST");
		savedetail(detailrsrc,rptrsrc);
	}
}
static void quitdetailabovetest(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	readallwidgets(detailrsrc);
	if(COMPARE_RSRCS(detailrsrc,detaildefaults,2) || changeddetails)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the DETAIL VALUES.\nDo you want to Save these changes?",save_detail_above,quitdetailcls,FALSE,2,detailrsrc,rptrsrc,NULL);
	} else {
		quitdetail(detailrsrc,rptrsrc);
	}
}
static void quitdetailbelowtest(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	readallwidgets(detailrsrc);
	if(COMPARE_RSRCS(detailrsrc,detaildefaults,2) || changeddetails)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the DETAIL VALUES.\nDo you want to Save these changes?",save_detail_below,quitdetailcls,FALSE,2,detailrsrc,rptrsrc,NULL);
	} else {
		quitdetail(detailrsrc,rptrsrc);
	}
}
static void estfieldvalues1(RDArsrc *detailrsrc)
{
	short length=0;
	int x,selected=0,ftype=0,typex=0;
	EZfield *f;
	
	if(FINDRSCGETINT(detailrsrc,"FIELD LIST",&selected)) return;
	if(FINDRSCGETINT(detailrsrc,"TYPES",&typex)) return;
	if(typex<1)
	{
		for(x=0,f=holdfields;x<numfields;++x,++f)
		{
			if(!RDAstrcmp(f->wholename,fieldlst->libs[selected])) break;
		}	
		if(x<numfields)
		{
			ftype=(int)f->type;
			if(!FINDRSCSETINT(detailrsrc,"FIELD TYPES",ftype))
				updatersrc(detailrsrc,"FIELD TYPES");
			length=f->len;
			if(!FINDRSCSETSHORT(detailrsrc,"LENGTH",length))
				updatersrc(detailrsrc,"LENGTH");
		}
	}
}
static void detailscreen(RDArsrc *rptrsrc,char *title1,char *title2,short start,short length,short type,char range,short fieldtype,char *fieldname,char *expression,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int typex,selected;
	RDAezdtl *detail;
	RDArsrc *detailrsrc=NULL;

	if(!start)
	{
		if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&selected)) return;
		if(ezrpt->detail!=NULL && ezrpt->numdtl>selected)
		{
			detail=ezrpt->detail+selected;
			start=detail->start+detail->length+1;
		} else start=0;
	}
	detailrsrc=RDArsrcNEW(module,"EZ MAINTAIN DETAIL");
	addstdrsrc(detailrsrc,"TITLE1",VARIABLETEXT,0,(title1!=NULL ? title1:NULL),
		TRUE);
	addstdrsrc(detailrsrc,"TITLE2",VARIABLETEXT,0,(title2!=NULL ? title2:NULL),
		TRUE);
	addstdrsrc(detailrsrc,"START",SHORTV,3,&start,TRUE);
	addstdrsrc(detailrsrc,"LENGTH",SHORTV,3,&length,TRUE);
	if(wtypes!=NULL) freeapplib(wtypes);
	wtypes=APPlibNEW();
	addAPPlib(wtypes,"Load Field");
	addAPPlib(wtypes,"Evaluate Expression");
	for(typex=0;typex<fieldlst->numlibs;++typex)
	{
		if(!RDAstrcmp(fieldlst->libs[typex],fieldname)) break;
	}
	if(fieldlst->numlibs<=typex) typex=0;
	addlstrsrc(detailrsrc,"FIELD LIST",&typex,TRUE,estfieldvalues1,
		fieldlst->numlibs,&fieldlst->libs,NULL);
	typex=fieldtype;
	addstdrsrc(detailrsrc,"SET RANGE",BOOLNS,1,&range,TRUE);
	addlstrsrc(detailrsrc,"FIELD TYPES",&typex,TRUE,NULL,ftypes->numlibs,
		&ftypes->libs,NULL);
	addsctrsrc(detailrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),
		TRUE);
	typex=type;
	addlstrsrc(detailrsrc,"TYPES",&typex,TRUE,changeTYPES,
		wtypes->numlibs,&wtypes->libs,rptrsrc);
	addbtnrsrc(detailrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(detailrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(detailrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(detailrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addrfcbrsrc(detailrsrc,"SAVE",TRUE,savefunc,rptrsrc);
	addrfcbrsrc(detailrsrc,"QUIT",TRUE,quitfunctest,rptrsrc);
	addbtnrsrc(detailrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(detailrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	estfieldvalues1(detailrsrc);
	changeddetails=FALSE;
	detaildefaults=GetDefaults(detailrsrc);
	changeTYPES(detailrsrc,rptrsrc);
	APPmakescrn(detailrsrc,TRUE,quitdetail,rptrsrc,FALSE);
}
static void changeTYPES(RDArsrc *detailrsrc,RDArsrc *rptrsrc)
{
	int type=0;
	char range_screen=FALSE;

	FINDRSCGETINT(detailrsrc,"TYPES",&type);
	if(type==0)
	{
		readwidget(rptrsrc,"RANGE SCREEN");
		FINDRSCGETCHAR(rptrsrc,"RANGE SCREEN",&range_screen);
		if(range_screen)
		{
			FINDRSCSETEDITABLE(detailrsrc,"SET RANGE",TRUE);
			FINDRSCSETSENSITIVE(detailrsrc,"SET RANGE",TRUE);
		} else {
			FINDRSCSETEDITABLE(detailrsrc,"SET RANGE",FALSE);
			FINDRSCSETSENSITIVE(detailrsrc,"SET RANGE",FALSE);
		}
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETEDITABLE(detailrsrc,"EXPRESSION",FALSE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD CONTROL",FALSE);
		FINDRSCSETEDITABLE(detailrsrc,"FIELD TYPES",FALSE);
		FINDRSCSETSENSITIVE(detailrsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD CONTROL",FALSE);
		FINDRSCSETSENSITIVE(detailrsrc,"FIELD LIST",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"FIELD LIST",TRUE);
	} else {
		FINDRSCSETSENSITIVE(detailrsrc,"SET RANGE",FALSE);
		FINDRSCSETEDITABLE(detailrsrc,"SET RANGE",FALSE);
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(detailrsrc,"EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(detailrsrc,"LOAD CONTROL",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"EXPRESSION",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD VALUE",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"LOAD CONTROL",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"FIELD TYPES",TRUE);
		FINDRSCSETEDITABLE(detailrsrc,"FIELD LIST",FALSE);
		FINDRSCSETSENSITIVE(detailrsrc,"FIELD LIST",FALSE);
	}
}
static void add_detail_above(RDArsrc *rptrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&selected)) return;
	if(detaillst->numlibs==1 && !RDAstrcmp(detaillst->libs[0],"No Detail Defined")) return;
	detailscreen(rptrsrc,NULL,NULL,0,0,0,FALSE,0,NULL,NULL,save_detail_above,quitdetailabovetest);
}
static void add_detail_below(RDArsrc *rptrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&selected)) return;
	if(detaillst->numlibs==1 && !RDAstrcmp(detaillst->libs[0],"No Detail Defined")) return;
	detailscreen(rptrsrc,NULL,NULL,0,0,0,FALSE,0,NULL,NULL,save_detail_below,quitdetailbelowtest);
}
static void copy_detail(RDArsrc *rptrsrc)
{
	int x;
	RDAezdtl *d;

	if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&x)) return;
	if(ezrpt->detail==NULL)
	{
		ezrpt->detail=Rmalloc(sizeof(RDAezdtl));
		d=ezrpt->detail+x;
		d->title1=NULL;
		d->title2=NULL;
		d->start=0;
		d->length=0;
		d->type=0;
		d->fieldname=NULL;
		d->expression=NULL;
		d->range=FALSE;
		d->fieldtype=0;
		ezrpt->detail=d;
		ezrpt->numdtl=1;
	}
	d=ezrpt->detail+x;
	detailscreen(rptrsrc,d->title1,d->title2,0,d->length,d->type,d->range,d->fieldtype,d->fieldname,d->expression,save_detail_below,quitdetailbelowtest);
}
static void edit_detail(RDArsrc *rptrsrc)
{
	int x;
	RDAezdtl *d;

	if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&x)) return;
	if(ezrpt->detail==NULL)
	{
		ezrpt->detail=Rmalloc(sizeof(RDAezdtl));
		d=ezrpt->detail+x;
		d->title1=NULL;
		d->title2=NULL;
		d->start=0;
		d->length=0;
		d->type=0;
		d->fieldname=NULL;
		d->expression=NULL;
		d->range=FALSE;
		d->fieldtype=0;
		ezrpt->detail=d;
		ezrpt->numdtl=1;
	}
	d=ezrpt->detail+x;
	detailscreen(rptrsrc,d->title1,d->title2,d->start,d->length,d->type,d->range,d->fieldtype,d->fieldname,d->expression,checksavedetail,quitdetailtest);
}
static void delete_detail(RDArsrc *rptrsrc)
{
	short x,y;
	RDAezdtl *temp,*temp1,*new_fields;
	int value;

	readallwidgets(rptrsrc);
	if(FINDRSCGETINT(rptrsrc,"DETAIL LIST",&value)) return;
	if(ezrpt->numdtl==1) return;
	new_fields=Rmalloc((ezrpt->numdtl-1)*sizeof(RDAezdtl));
	y=0;
	for(x=0,temp=ezrpt->detail;x<ezrpt->numdtl;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			if(temp->title1!=NULL)
			{
				temp1->title1=stralloc(temp->title1);
				Rfree(temp->title1);
			} else temp1->title1=NULL;
			if(temp->title2!=NULL)
			{
				temp1->title2=stralloc(temp->title2);
				Rfree(temp->title2);
			} else temp1->title2=NULL;
			if(temp->fieldname!=NULL)
			{
				temp1->fieldname=stralloc(temp->fieldname);
				Rfree(temp->fieldname);
			} else temp1->fieldname=NULL;
			temp1->start=temp->start;
			temp1->length=temp->length;
			temp1->type=temp->type;
			temp1->range=temp->range;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->fieldtype=temp->fieldtype;
			++y;
		}
	}
	Rfree(ezrpt->detail);
	--ezrpt->numdtl;
	ezrpt->detail=Rmalloc(ezrpt->numdtl*sizeof(RDAezdtl));
	for(x=0,temp1=ezrpt->detail,temp=new_fields;x<ezrpt->numdtl;++x,
		++temp,++temp1)
	{
		if(temp->title1!=NULL)
		{
			temp1->title1=stralloc(temp->title1);
			Rfree(temp->title1);
		} else temp1->title1=NULL;
		if(temp->title2!=NULL)
		{
			temp1->title2=stralloc(temp->title2);
			Rfree(temp->title2);
		} else temp1->title2=NULL;
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
		temp1->start=temp->start;
		temp1->length=temp->length;
		temp1->type=temp->type;
		temp1->range=temp->range;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->fieldtype=temp->fieldtype;
	}
	Rfree(new_fields);
	makedetaillist();
	if(!FINDRSCLISTAPPlib(rptrsrc,"DETAIL LIST",value,detaillst))
		updatersrc(rptrsrc,"DETAIL LIST");
	changedezrpt=TRUE;
}
static void quitrpt(RDArsrc *rptrsrc,RDArsrc *mainrsrc)
{
	int x;
	EZfield *f;

	if(holdfields!=NULL)
	{
		for(x=0,f=holdfields;x<numfields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
			if(f->wholename!=NULL) Rfree(f->wholename);
		}
		Rfree(holdfields);
		numfields=0;
	}
	if(file_defs!=NULL) freeapplib(file_defs);
	if(sortlst!=NULL) freeapplib(sortlst);
	if(detaillst!=NULL) freeapplib(detaillst);
	if(fieldlst!=NULL) freeapplib(fieldlst);
	if(totals!=NULL) freeapplib(totals);
	if(ftypes!=NULL) freeapplib(ftypes);
	if(device_list!=NULL) freeapplib(device_list);
	if(print_size!=NULL) freeapplib(print_size);
	FreeRDAezrpt(ezrpt);
	if(rptrsrc!=NULL)
	{
		killwindow(rptrsrc);
		free_rsrc(rptrsrc);
	}
	changedsorts=FALSE;
	changeddetails=FALSE;
	changedezrpt=FALSE;
	if(rptdefaults!=NULL) FreeRDAdefault(rptdefaults);
}
static void quitrpttest(RDArsrc *rptrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(rptrsrc);
	if(COMPARE_RSRCS(rptrsrc,rptdefaults,2) || changedezrpt)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the EZ REPORT DEFINITION.\nDo you want to Save these changes?",saverpt,quitrpt,FALSE,2,rptrsrc,mainrsrc,NULL);
	} else {
		quitrpt(rptrsrc,mainrsrc);
	}
}
static void saverpt(RDArsrc *rptrsrc,RDArsrc *mainrsrc)
{
	char *rptmodule=NULL,align_test=FALSE,print_header=TRUE,acct_report=FALSE;
	char *temp=NULL;
	int selected_device=0,selected_printsize=0;
	RDAreport *report=NULL;
	short AcctType=(-1);
	int x;
	EZfield *f;
	
	readallwidgets(rptrsrc);
	FINDRSCGETSTRING(rptrsrc,"REPORT MODULE",&rptmodule);
	FINDRSCGETSTRING(rptrsrc,"REPORT NAME",&ezrpt->name);
	if(rptmodule!=NULL && ezrpt->name!=NULL)
	{
		FINDRSCGETSTRING(rptrsrc,"TITLE",&ezrpt->title);
		FINDRSCGETSTRING(rptrsrc,"DESCRIPTION",&ezrpt->desc);
		FINDRSCGETINT(rptrsrc,"DEVICE LIST",&selected_device);
		FINDRSCGETINT(rptrsrc,"PRINT SIZE",&selected_printsize);
		FINDRSCGETDOUBLE(rptrsrc,"PAGE LENGTH",&ezrpt->page_length);
		FINDRSCGETCHAR(rptrsrc,"PRINT HEADER",&print_header);
		FINDRSCGETCHAR(rptrsrc,"ACCOUNT REPORT",&acct_report);
		FINDRSCGETCHAR(rptrsrc,"SKIP FINMGT",&ezrpt->skip_finmgt);
		FINDRSCGETCHAR(rptrsrc,"RANGE SCREEN",&ezrpt->range_screen);
		ezrpt->account_report=(short)acct_report;
		ezrpt->print=(short)print_header;
		FINDRSCGETCHAR(rptrsrc,"ALIGNMENT TEST",&align_test);
		ezrpt->alignment_test=(short)align_test;
		if(ezrpt->devicename!=NULL) Rfree(ezrpt->devicename);
		ezrpt->devicename=stralloc(device_list->libs[selected_device]);
		if(ezrpt->printsize!=NULL) Rfree(ezrpt->printsize);
		ezrpt->printsize=stralloc(print_size->libs[selected_printsize]);
		ezrpt->majmod=stralloc(holdmodule);
		ezrpt->majfil=stralloc(holdfile);
		if(WRTRDAezrpt(rptmodule,ezrpt))
		{
			ERRORDIALOG("Cannot Write EZ-Report","Error in permissions, user cannot write the EZ-report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
		} else {
			report=EZRPTtoRPT(rptmodule,ezrpt,holdfields,numfields);
			if(report!=NULL)
			{
				if(WRTRDAreport(rptmodule,report))
				{
					ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
					FreeRDAreport(report);
				} else {
					FreeRDAreport(report);
					if(ezrpt->account_report)
					{
						temp=Rmalloc(RDAstrlen(ezrpt->name)+14);
						sprintf(temp,"%s RANGE SCREEN",ezrpt->name);
						if(!RDAstrcmp(ezrpt->majmod,"FINMGT"))
						{
							if(!RDAstrcmp(ezrpt->majfil,"FINEACC") ||
								!RDAstrcmp(ezrpt->majfil,"FINEACT") ||
								!RDAstrcmp(ezrpt->majfil,"FINEYR"))
							{
								AcctType=0;
							} else if(!RDAstrcmp(ezrpt->majfil,"FINRACC") ||
								!RDAstrcmp(ezrpt->majfil,"FINRACT") ||
								!RDAstrcmp(ezrpt->majfil,"FINRYR"))
							{
								AcctType=1;
							} else if(!RDAstrcmp(ezrpt->majfil,"FINBACC") ||
								!RDAstrcmp(ezrpt->majfil,"FINBACT") ||
								!RDAstrcmp(ezrpt->majfil,"FINBYR"))
							{
								AcctType=2;
							}
								
						} else if(!RDAstrcmp(ezrpt->majmod,"BUDPREP"))
						{
							if(!RDAstrcmp(ezrpt->majfil,"BUDEACC")) AcctType=0;
							else if(!RDAstrcmp(ezrpt->majfil,"BUDRACC")) AcctType=1;
						}
						EZRPT_ConvScrn(rptmodule,temp,AcctType);
						if(temp!=NULL) Rfree(temp);
					}
				}
			} else {
				prterr("Error EZ Define Report Conversion to Report Binary Failed.");
			}
		}
	}
	if(holdfields!=NULL)
	{
		for(x=0,f=holdfields;x<numfields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
			if(f->wholename!=NULL) Rfree(f->wholename);
		}
		Rfree(holdfields);
		numfields=0;
	}
	if(rptmodule!=NULL) Rfree(rptmodule);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(sortlst!=NULL) freeapplib(sortlst);
	if(detaillst!=NULL) freeapplib(detaillst);
	if(fieldlst!=NULL) freeapplib(fieldlst);
	getrptentries(mainrsrc);
	quitrpt(rptrsrc,mainrsrc);
}
static void setprintsize(RDArsrc *rptrsrc)
{
	int selected=0,value=0;

	if(FINDRSCGETINT(rptrsrc,"DEVICE LIST",&selected)) return;
	if(FINDRSCGETINT(rptrsrc,"PRINT SIZE",&value)) return;
	if(print_size!=NULL) freeapplib(print_size);
	print_size=APPlibNEW();
	addAPPlib(print_size,"NORMAL TEXT (DUPLEX)");
	addAPPlib(print_size,"COMPRESSED TEXT (DUPLEX)");
	addAPPlib(print_size,"LANDSCAPE NORMAL TEXT (DUPLEX)");
	addAPPlib(print_size,"LANDSCAPE COMPRESSED TEXT (DUPLEX)");
	addAPPlib(print_size,"NORMAL TEXT");
	addAPPlib(print_size,"COMPRESSED TEXT");
	addAPPlib(print_size,"LANDSCAPE NORMAL TEXT");
	addAPPlib(print_size,"LANDSCAPE COMPRESSED TEXT");
	if(print_size->numlibs<1)
	{
		addAPPlib(print_size,"Not Available");
	}
	if(value>=print_size->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(rptrsrc,"PRINT SIZE",value,print_size))
		updatersrc(rptrsrc,"PRINT SIZE");
}
static APPlib *makedevicelist()
{
	char *libx=NULL;
	APPlib *out_devs=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	out_devs=getunsortedlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	addAPPlibNoDuplicates(out_devs,"screen");
#ifdef RS6000
	addAPPlibNoDuplicates(out_devs,"rdaout.rsc");
#elif defined(UNIXWARE7)
	addAPPlibNoDuplicates(out_devs,"rdaout.unw");
#elif defined(UNIXWARE21)
	addAPPlibNoDuplicates(out_devs,"rdaout.unw");
#elif defined(LINUX)
	addAPPlibNoDuplicates(out_devs,"rdaout.lnx");
#elif defined(LINUX2_2) || defined(UBUNTU_OS)
	addAPPlibNoDuplicates(out_devs,"rdaout.lnx");
#elif defined(SCO386)
	addAPPlibNoDuplicates(out_devs,"rdaout.sco");
#elif defined(SCO50)
	addAPPlibNoDuplicates(out_devs,"rdaout.sco");
#elif defined(SCO50P)
	addAPPlibNoDuplicates(out_devs,"rdaout.sco");
#elif defined(SUNX86)
	addAPPlibNoDuplicates(out_devs,"rdaout.sun");
#endif
	SORTAPPlib(out_devs);
	return(out_devs);
}
static void set_account_report(RDArsrc *rptrsrc)
{
	char acct_report=FALSE;

	readwidget(rptrsrc,"ACCOUNT REPORT");
	FINDRSCGETCHAR(rptrsrc,"ACCOUNT REPORT",&acct_report);
	ezrpt->account_report=(short)acct_report;
	if(acct_report)
	{
		FINDRSCSETCHAR(rptrsrc,"SKIP FINMGT",FALSE);
		FINDRSCSETEDITABLE(rptrsrc,"SKIP FINMGT",FALSE);
		FINDRSCSETSENSITIVE(rptrsrc,"SKIP FINMGT",FALSE);
	} else {
		FINDRSCSETEDITABLE(rptrsrc,"SKIP FINMGT",TRUE);
		FINDRSCSETSENSITIVE(rptrsrc,"SKIP FINMGT",TRUE);
	}
	updatersrc(rptrsrc,"SKIP FINMGT");
}
static void dfscn(RDArsrc *r)
{
	ExecuteProgram("dfscn",NULL);
}
static void dfmenu(RDArsrc *r)
{
	ExecuteProgram("dfmenu",NULL);
}
static void reportscreen(RDArsrc *mainrsrc,char *rptmodule,char *rptname,char *title,
	char *desc,char *majmod,char *majfil,char *devicename,char *printsize,
	double page_length,short printheader,short alignmenttest,
	short account_report,char range_screen,char skip_finmgt)
{
	int x;
	char align_test=FALSE,p_header=TRUE,*defaultprinter=NULL,acctreport;
	RDArsrc *rptrsrc=NULL;
	char *major_module=NULL;

	if(fieldlst!=NULL) freeapplib(fieldlst);
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x)
	{
		addAPPlib(ftypes,standardfieldtypes[x]);
	}
	if(totals!=NULL) freeapplib(totals);
	totals=APPlibNEW();
	addAPPlib(totals,"Sort Only");
	addAPPlib(totals,"Sort & Accumulate");
	addAPPlib(totals,"Sort, Accumulate & Page Break");
	addAPPlib(totals,"Select Only");
	rptrsrc=RDArsrcNEW(module,"EZ MAINTAIN REPORT");
	addstdrsrc(rptrsrc,"REPORT MODULE",PLAINTEXT,15,(rptmodule!=NULL ? rptmodule:""),TRUE);
	addstdrsrc(rptrsrc,"REPORT NAME",PLAINTEXT,40,(rptname!=NULL ? rptname:""),TRUE);
	addstdrsrc(rptrsrc,"TITLE",VARIABLETEXT,0,(title!=NULL ? title:""),TRUE);
	addsctrsrc(rptrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	acctreport=account_report;
	if(acctreport) ezrpt->account_report=TRUE;
	addstdrsrc(rptrsrc,"ACCOUNT REPORT",BOOLNS,1,&acctreport,TRUE);
	addstdrsrc(rptrsrc,"RANGE SCREEN",BOOLNS,1,&range_screen,TRUE);
	FINDRSCSETFUNC(rptrsrc,"ACCOUNT REPORT",set_account_report,NULL);
	addstdrsrc(rptrsrc,"SKIP FINMGT",BOOLNS,1,&skip_finmgt,TRUE);
	if(device_list!=NULL) freeapplib(device_list);
	device_list=makedevicelist();
	defaultprinter=DefaultPrinter();
	if(defaultprinter!=NULL)
	{
		addAPPlibNoDuplicates(device_list,defaultprinter);
	}
	SORTAPPlib(device_list);
	if(devicename!=NULL)
	{
		for(x=0;x<device_list->numlibs;++x)
		{
			if(!RDAstrcmp(device_list->libs[x],devicename)) break;
		}
	} else {
		for(x=0;x<device_list->numlibs;++x)
		{
			if(!RDAstrcmp(device_list->libs[x],defaultprinter)) break;
		}
	}
	if(x>=device_list->numlibs) x=0;
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	defaultprinter=NULL;
	addlstrsrc(rptrsrc,"DEVICE LIST",&x,TRUE,setprintsize,device_list->numlibs,
		&device_list->libs,NULL);
	if(print_size!=NULL) freeapplib(print_size);
	print_size=APPlibNEW();
	addlstrsrc(rptrsrc,"PRINT SIZE",&x,TRUE,NULL,print_size->numlibs,
		&print_size->libs,NULL);
	setprintsize(rptrsrc);
	if(printsize!=NULL)
	{
		for(x=0;x<print_size->numlibs;++x)
		{
			if(!RDAstrcmp(print_size->libs[x],printsize)) break;
		}
	} 
	if(x>=print_size->numlibs) x=0;
	FINDRSCSETINT(rptrsrc,"PRINT SIZE",x);
	p_header=printheader;
	addstdrsrc(rptrsrc,"PRINT HEADER",BOOLNS,1,&p_header,TRUE);
	align_test=alignmenttest;
	addstdrsrc(rptrsrc,"ALIGNMENT TEST",BOOLNS,1,&align_test,TRUE);
	addstdrsrc(rptrsrc,"PAGE LENGTH",DECIMALV,5,&page_length,TRUE);
	if(!isEMPTY(majmod)) major_module=stralloc(majmod);
		else major_module=stralloc(rptmodule);
	for(x=0;x<modlstx->numlibs;++x) 
	{
		if(!RDAstrcmp(modlstx->libs[x],major_module)) break;
	}
	if(x>=modlstx->numlibs) x=0;
	init_value=TRUE;
	addlstrsrc(rptrsrc,"MODULE LIST",&x,TRUE,dofilelist,
		modlstx->numlibs,&modlstx->libs,&init_value);
	if(major_module!=NULL) Rfree(major_module);
	if(file_defs!=NULL) freeapplib(file_defs);
	file_defs=APPlibNEW();
	x=0;
	addlstrsrc(rptrsrc,"FILE LIST",&x,TRUE,setmajorfile,file_defs->numlibs,
		&file_defs->libs,NULL);
	init_value=FALSE;
	dofilelist(rptrsrc,&init_value);
	init_value=TRUE;
	for(x=0;x<file_defs->numlibs;++x)
	{
		if(!RDAstrcmp(file_defs->libs[x],majfil)) break;
	}
	if(x>=file_defs->numlibs) x=0;
	FINDRSCSETINT(rptrsrc,"FILE LIST",x);
	if(sortlst!=NULL) freeapplib(sortlst);
	sortlst=APPlibNEW();
	addlstrsrc(rptrsrc,"SORT LIST",0,TRUE,NULL,sortlst->numlibs,
		&sortlst->libs,NULL);
	addbtnrsrc(rptrsrc,"ADD SORT ABOVE",TRUE,add_sort_above,NULL);
	addbtnrsrc(rptrsrc,"ADD SORT BELOW",TRUE,add_sort_below,NULL);
	addbtnrsrc(rptrsrc,"DELETE SORT",TRUE,delete_sort,NULL);
	addbtnrsrc(rptrsrc,"SELECT SORT",TRUE,edit_sort,NULL);
	addbtnrsrc(rptrsrc,"COPY SORT",TRUE,copy_sort,NULL);
	makesortlist();
	FINDRSCLISTAPPlib(rptrsrc,"SORT LIST",0,sortlst);
	if(detaillst!=NULL) freeapplib(detaillst);
	detaillst=APPlibNEW();
	addlstrsrc(rptrsrc,"DETAIL LIST",0,TRUE,NULL,detaillst->numlibs,
		&detaillst->libs,NULL);
	addbtnrsrc(rptrsrc,"ADD DETAIL ABOVE",TRUE,add_detail_above,NULL);
	addbtnrsrc(rptrsrc,"ADD DETAIL BELOW",TRUE,add_detail_below,NULL);
	addbtnrsrc(rptrsrc,"DELETE DETAIL",TRUE,delete_detail,NULL);
	addbtnrsrc(rptrsrc,"SELECT DETAIL",TRUE,edit_detail,NULL);
	addbtnrsrc(rptrsrc,"COPY DETAIL",TRUE,copy_detail,NULL);
	addbtnrsrc(rptrsrc,"DFSCN",TRUE,dfscn,NULL);
	addbtnrsrc(rptrsrc,"DFMENU",TRUE,dfmenu,NULL);
	addrfcbrsrc(rptrsrc,"SAVE",TRUE,saverpt,mainrsrc);
	addrfcbrsrc(rptrsrc,"QUIT",TRUE,quitrpttest,mainrsrc);
	addbtnrsrc(rptrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(rptrsrc,"PRINT AVAILABLE",TRUE,printavailrsrc,ezrpt);
	addbtnrsrc(rptrsrc,"PRINT DEFINITION",TRUE,printezrptdef,ezrpt);
	addbtnrsrc(rptrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	makedetaillist();
	FINDRSCLISTAPPlib(rptrsrc,"DETAIL LIST",0,detaillst);
	setmajorfile(rptrsrc);
	rptdefaults=GetDefaults(rptrsrc);
	changedezrpt=FALSE;
	APPmakescrn(rptrsrc,TRUE,quitrpt,mainrsrc,FALSE);
}
static void newreport(RDArsrc *mainrsrc)
{
	int selectedm=0;

	ezrpt=RDAezrptNEW(NULL);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	holdmodule=stralloc(modlstx->libs[selectedm]);
	holdfile=NULL;
	reportscreen(mainrsrc,modlstx->libs[selectedm],NULL,NULL,NULL,NULL,NULL,NULL,NULL,10,TRUE,FALSE,FALSE,TRUE,FALSE);
}
static void okreport(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(rptavl->libs[selected],"No Report Definitions Available"))
	{
		newreport(mainrsrc);
	} else {
		if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
		ezrpt=GETRDAezrpt(modlstx->libs[selectedm],rptavl->libs[selected]);
		if(ezrpt==NULL)
		{
			ERRORDIALOG("Cannot Read EZ-Report","Error in permissions, the user cannot read the EZ-report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Error Report [%s] not readable.",rptavl->libs[selected]);
			return;
		}
		holdmodule=stralloc(modlstx->libs[selectedm]);
		if(ezrpt->majfil!=NULL)
		{
			holdfile=stralloc(ezrpt->majfil);
		} else holdfile=NULL;
		reportscreen(mainrsrc,modlstx->libs[selectedm],rptavl->libs[selected]
			,ezrpt->title,ezrpt->desc,ezrpt->majmod
			,ezrpt->majfil,ezrpt->devicename,ezrpt->printsize,
			ezrpt->page_length,ezrpt->print,ezrpt->alignment_test,
			ezrpt->account_report,ezrpt->range_screen,ezrpt->skip_finmgt);
	}
}
static void getmodulelist(RDArsrc *mainrsrc)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlstx,tmp);
	}
	if(modlstx->numlibs<1)
	{
		addAPPlib(modlstx,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlstx);
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",0,modlstx))
		{
			updatersrc(mainrsrc,"MODULE LIST");
		}
	}
}
static void getrptentries(RDArsrc *mainrsrc)
{
	int x,selected=0,selr=0;
	char *tmp=NULL,*dirx=NULL,*temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selr)) return;
	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
	for(x=0;findfile(dirx,"*.EZRPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-6]=0;
		temp=stripdashes(tmp);
		addAPPlib(rptavl,temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	}
	if(rptavl->numlibs<1)
	{
		addAPPlib(rptavl,"No Report Definitions Available");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(rptavl);
	if(selr>=rptavl->numlibs) selr=0;
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"REPORTS AVAILABLE",selr,rptavl))
			updatersrc(mainrsrc,"REPORTS AVAILABLE");
	}
	setvalues(mainrsrc);
}
static void deleterpttest(RDArsrc *mainrsrc)
{
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE EZ REPORT DEFINITION?","Are you sure that you want to DELETE this EZ REPORT DEFINITION?",deleterpt,NULL,FALSE,mainrsrc);
}
static void deleterpt(RDArsrc *mainrsrc)
{
	char *fname=NULL,*dashes=NULL;
	int selected=0,selectedm=0;


	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(RDAstrcmp(rptavl->libs[selected],"No Report Definitions Available"))
	
        {
		if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
		dashes=adddashes(rptavl->libs[selected]);
		fname=Rmalloc(RDAstrlen(modlstx->libs[selectedm])+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
		sprintf(fname,"%s/%s/%s.EZRPT",CURRENTDIRECTORY,modlstx->libs[selectedm],dashes);
#endif
#ifdef WIN32
		sprintf(fname,"%s\\%s\\%s.EZRPT",CURRENTDIRECTORY,modlstx->libs[selectedm],dashes);
#endif
		if(dashes!=NULL) Rfree(dashes);
		if(access(fname,02))
		{
			ERRORDIALOG("Cannot Delete EZ-Report","Error in permissions, the user doesn't have access to delete the EZ-Report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
		} else {
		if(unlink(fname)==(-1))
		{
			prterr("Error Cannot Remove Report Definition File [%s].",fname);
		} else {
			getrptentries(mainrsrc);
		}
		}
		if(fname!=NULL) Rfree(fname);
	}
}
static void quitezrpt(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	r=NULL;
	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=NULL;
	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=NULL;
	ShutdownSubsystems();
}
static void execrpt(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(rptavl->numlibs==1 && !RDAstrcmp(rptavl->libs[0],"No Report Definitions Available")) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlstx->libs[selectedm]);
	addAPPlib(args,rptavl->libs[selected]);
	ExecuteProgram("doreport",args);
	if(args!=NULL) freeapplib(args);
}
static void setvalues(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(!RDAstrcmp(rptavl->libs[selected],"No Report Definitions Available"))
	{
		temp=NULL;
	} else {
		temp=getezrpttitle(modlstx->libs[selectedm],rptavl->libs[selected]);
	}
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE",temp))
		updatersrc(mainrsrc,"TITLE");
	if(temp!=NULL) Rfree(temp);
	if(!RDAstrcmp(rptavl->libs[selected],"No Report Definitions Available"))
	{
		temp=NULL;
	} else temp=getezrptdesc(modlstx->libs[selectedm],rptavl->libs[selected]);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"EZ DEFINE REPORTS")) 
	{
		return;
	}
	getmodulelist(mainrsrc);
	mainrsrc=RDArsrcNEW(module,"EZ DEFINE REPORTS");
	if(argc>1)
	{
		for(x=0;x<modlstx->numlibs;++x)
		{
			if(!RDAstrcmp(argv[1],modlstx->libs[x])) break;
		}
		if(x>=modlstx->numlibs) x=0;
	}
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,getrptentries,
		modlstx->numlibs,&modlstx->libs,NULL);
	x=0;
	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=APPlibNEW();
	addlstrsrc(mainrsrc,"REPORTS AVAILABLE",&x,TRUE,setvalues,
		rptavl->numlibs,&rptavl->libs,NULL);
	getrptentries(mainrsrc);
	if(argc>2)
	{
		for(x=0;x<rptavl->numlibs;++x)
		{
			if(!RDAstrcmp(argv[2],rptavl->libs[x])) break;
		}
		if(x>=rptavl->numlibs) x=0;
		FINDRSCSETINT(mainrsrc,"REPORTS AVAILABLE",x);
	}
	addstdrsrc(mainrsrc,"TITLE",VARIABLETEXT,0,NULL,FALSE);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newreport,NULL);
	addbtnrsrc(mainrsrc,"RUN REPORT",TRUE,execrpt,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okreport,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitezrpt,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deleterpttest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setvalues(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitezrpt,NULL,TRUE);
}
