/* mkezcd.c - Generate Report C Code for speed of development*/
#ifndef WIN32
#define __NAM__ "mkezcd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkezcd.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
#include <mkmsc.hpp>
#include <mkez.hpp>
#include <packlib.hpp>
/*ADDLIB tool */
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


APPlib *rptavl=NULL,*modlstx=NULL;
char *module=NULL;

static void makeezrptcode(char *modx,RDAezrpt *r,FILE *fp)
{
/*
	int z,q,numlines=0;
	RDAaccval *accval=NULL;
*/
	int x=0,q=0;
	char *funcname=NULL;
	RDAezdtl *d=NULL;
	DFincvir *i=NULL;
	RDAezsrt *s=NULL;
	int overall_length=0;
	APPlib *tempapplib=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"/* %s's - Make (%s) Report */\n",
			(module!=NULL?module:""),
			(r->name!=NULL?r->name:""));
		fprintf(fp,"#include <cstdio>\n");
		fprintf(fp,"#include <rptgen.hpp>\n");
		fprintf(fp,"#include <mkez.hpp>\n");
		fprintf(fp,"#include <mkmsc.hpp>\n");
		fprintf(fp,"/*COMPMEM 40960 */\n");
		fprintf(fp,"\n");
		fprintf(fp,"extern char *module;\n");
		fprintf(fp,"extern APPlib *errorlist;\n");
		funcname=fixname(r->name);
		fprintf(fp,"\n");
		fprintf(fp,"static void EZRPT_%s()\n",(funcname!=NULL?funcname:""));
		if(funcname!=NULL) Rfree(funcname);
		fprintf(fp,"{\n");
		fprintf(fp,"\tshort y=0;\n");
		fprintf(fp,"\tRDAezrpt *tmp=NULL;\n");
		fprintf(fp,"\tRDAezsrt *s=NULL;\n");
		fprintf(fp,"\tchar *t=NULL,*temp1=NULL;\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\ttmp=RDAezrptNEW(\"%s\");\n",(r->name!=NULL ? r->name:""));
		fprintf(fp,"\n");

		fprintf(fp,"\tmemset(stemp,0,101);\n");
		fprintf(fp,"\ttmp->title=Rmalloc(%d+1);\n",(r->title!=NULL?RDAstrlen(r->title):3));
		fprintf(fp,"\tsprintf(tmp->title,\"%s\");\n",(r->title!=NULL?r->title:""));
		fprintf(fp,"\ttmp->desc=Rmalloc(%d+1);\n",(r->desc!=NULL?RDAstrlen(r->desc):3));
		fprintf(fp,"\tsprintf(tmp->desc,\"%s\");\n",(r->desc!=NULL?r->desc:""));
		fprintf(fp,"\ttmp->majmod=stralloc(\"%s\");\n",(r->majmod!=NULL ? r->majmod:""));
		fprintf(fp,"\ttmp->majfil=stralloc(\"%s\");\n",(r->majfil!=NULL ? r->majfil:""));
		fprintf(fp,"\ttmp->devicename=stralloc(\"%s\");\n",(r->devicename!=NULL ? r->devicename:""));
		fprintf(fp,"\ttmp->printsize=stralloc(\"%s\");\n",(r->printsize!=NULL ? r->printsize:""));
		fprintf(fp,"\ttmp->page_length=%.03f;\n",r->page_length);
		fprintf(fp,"\ttmp->print=%d;\n",r->print);
		fprintf(fp,"\ttmp->account_report=%d;\n",r->account_report);
		fprintf(fp,"\ttmp->skip_finmgt=%s;\n",(r->skip_finmgt ? "TRUE":"FALSE"));
		fprintf(fp,"\ttmp->alignment_test=%d;\n",r->alignment_test);
		fprintf(fp,"\ttmp->range_screen=%s;\n",(r->range_screen ? "TRUE":"FALSE"));
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");



		if(r->incvir!=NULL)
		{
			for(x=0,i=r->incvir;x<r->num;++x,++i)
			{
				fprintf(fp,"\taddEZincvir(tmp,\"%s\",\"%s\",\"%s\");\n",
					(i->module!=NULL ? i->module:""),
					(i->file!=NULL ? i->file:""),
					(i->keyname!=NULL ? i->keyname:""));	
			}
		}

		if(r->sort!=NULL)
		{
			for(x=0,s=r->sort;x<r->numsrt;++x,++s)
			{
        			fprintf(fp,"\ty=addEZsort(tmp,\"%s\",%d,%d,%d,%d,%d);\n",
					(s->name!=NULL?s->name:""),s->total,s->start,s->length,
					s->type,s->range);
				fprintf(fp,"\ts=tmp->sort+(y-1);\n");
				switch(s->type)
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
						fprintf(fp,"\taddEZsortFROMTOSTRING(s,\"%s\",\"%s\");\n",
							(s->from.string_value!=NULL ? 
							s->from.string_value:""),(s->to.string_value!=NULL ?
							s->to.string_value:""));
						break;
					case CHARACTERS:
						fprintf(fp,"\taddEZsortFROMTOBOOLNS(s,%c,%c);\n",
							*s->from.string_value,*s->to.string_value);
						break;
					case BOOLNS:
						fprintf(fp,"\taddEZsortFROMTOBOOLNS(s,%d,%d);\n",
							*s->from.string_value,*s->to.string_value);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						fprintf(fp,"\taddEZsortFROMTODOUBLE(s,%f,%f);\n",
							*s->from.float_value,*s->to.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						fprintf(fp,"\taddEZsortFROMTOSHORT(s,%d,%d);\n",
							*s->from.short_value,*s->to.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						fprintf(fp,"\taddEZsortFROMTOINT(s,%d,%d);\n",
							*s->from.integer_value,*s->to.integer_value);
						break;
					default:
						prterr("Error Type [%d] is invalid in Sort [%s].",s->type,s->name);
						break;
				}
			}
		}
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");


		if(r->detail!=NULL)
		{
			for(x=0,d=r->detail;x<r->numdtl;++x,++d)
			{
				overall_length=0;
				tempapplib=break_expression2(d->expression,&overall_length);
				if(tempapplib!=NULL)
				{
					fprintf(fp,"\ttemp1=Rmalloc(%d+1);\n",overall_length);
					fprintf(fp,"\tsprintf(temp1,\"");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						fprintf(fp,"%%s");
					}
					fprintf(fp,"\",\n");
					for(q=0;q<tempapplib->numlibs;++q)
					{
						if(q!=(tempapplib->numlibs-1))
						{
							fprintf(fp,"\t\t%s,\n",tempapplib->libs[q]);
						} else {
							fprintf(fp,"\t\t%s);\n",tempapplib->libs[q]);
						}
					}
					freeapplib(tempapplib);
				}

        			fprintf(fp,"\tADVaddEZdetail(tmp,\"%s\",\"%s\",%d,%d,%d,%s,%d,\"%s\",temp1);\n",
					(d->title1!=NULL ? d->title1:""),
					(d->title2!=NULL ? d->title2:""),
					d->start,d->length,d->type,
					(d->range ? "TRUE":"FALSE"),
					d->fieldtype,(d->fieldname!=NULL ?
					d->fieldname:""));
				fprintf(fp,"\tif(temp1!=NULL) Rfree(temp1);\n");
				fprintf(fp,"\n");
        			fprintf(fp,"\tif(t!=NULL) Rfree(t);\n");
			}
		}
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");

		fprintf(fp,"\n");
		fprintf(fp,"\tif(tmp!=NULL)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tif(WRTRDAezrpt(\"%s\",tmp))\n",(modx!=NULL ? modx:""));
		fprintf(fp,"\t\t{\n");

		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");
		fprintf(fp,"\t\t\ttemp1=Rmalloc(%d+%d+100+1);\n",RDAstrlen(r->name),RDAstrlen(modx));
		fprintf(fp,"\t\t\tsprintf(temp1,\"REPORT WRITE ERROR: Module [%s] Report [%s], Can Not Save Report!\");\n",(modx!=NULL?modx:""),(r->name!=NULL?r->name:""));
		fprintf(fp,"\t\t\tprterr(temp1);\n");
		fprintf(fp,"\t\t\tif(errorlist!=NULL)\n");
		fprintf(fp,"\t\t\t{\n");
		fprintf(fp,"\t\t\t\taddERRlist(&errorlist,temp1);\n");
		fprintf(fp,"\t\t\t}\n");
		fprintf(fp,"\t\t\tif(temp1!=NULL) Rfree(temp1);\n");

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\tFreeRDAezrpt(tmp);\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");
	}

}
static void quit_printRDAezrptcode(RDArsrc *prsrc)
{
        if(diagrptgen)
        {
                prterr("DIAG quit_printRDAezrptcode Quitting Print RDA Report Code");
        }
        free_rsrc(prsrc);
}
static void printRDAezrptcode(RDArsrc *prsrc,RDAezrpt *r)
{
	char *outdevice=NULL;
	FILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
#ifndef WIN32
		fp=popen(outdevice,"w");
#endif
#ifdef WIN32
		fp=fopen(outdevice,"a+");
#endif
                if(fp!=NULL)
                {
			if(outdevice!=NULL) Rfree(outdevice);
			makeezrptcode(module,r,fp);
#ifndef WIN32
			pclose(fp);
#endif
#ifdef WIN32
			fclose(fp);
#endif
		} else {
			ERRORDIALOG("OUTPUT DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
			prterr("Error PrintRDAezrpt popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
			if(outdevice!=NULL) Rfree(outdevice);
		}
	} else {
		ERRORDIALOG("OUTPUT DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
	}
}
static void GetDevicePrintRDAezrptcode(RDAezrpt *r)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW("GUI","PACKAGE BINARY");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAezrptcode,r);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAezrptcode,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PACKAGE BINARY. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
void okreport(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	RDAezrpt *report=NULL;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(RDAstrcmp(rptavl->libs[selected],"No Definitions Available"))
	{
		module=stralloc(modlstx->libs[selectedm]);
		report=GETRDAezrpt(modlstx->libs[selectedm],rptavl->libs[selected]);
		if(report==NULL)
		{
			ERRORDIALOG("Cannot Read Report","Error in permissions, users doesn't have access to read the report definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			return;
		} else {
			GetDevicePrintRDAezrptcode(report);
		}
	} else { 
		return;
	}
}
void setvalues(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected))
	{
		return;
	}
	if(!RDAstrcmp(rptavl->libs[selected],"No Report Definitions Available"))
	{
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm))
	{
		return;
	}
	temp=getezrpttitle(modlstx->libs[selectedm],rptavl->libs[selected]);
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE",temp))
	{
		updatersrc(mainrsrc,"TITLE");
	}
	if(temp!=NULL) Rfree(temp);
	temp=getezrptdesc(modlstx->libs[selectedm],rptavl->libs[selected]);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
	{
		updatersrc(mainrsrc,"DESCRIPTION");
	}
	if(temp!=NULL) Rfree(temp);
}
void getrptentries(RDArsrc *mainrsrc)
{
	int x,selected=0;
	char *tmp=NULL,*dirx=NULL,*temp=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=APPlibNEW();
	if(module!=NULL) Rfree(module);
	module=stralloc(modlstx->libs[selected]);
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
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"REPORTS AVAILABLE",0,rptavl))
		updatersrc(mainrsrc,"REPORTS AVAILABLE");
	}
	setvalues(mainrsrc);
}
void getmodulelist(RDArsrc *mainrsrc)
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
void quitrptcode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(modlstx!=NULL) freeapplib(modlstx);
	if(rptavl!=NULL) freeapplib(rptavl);
	if(module!=NULL) Rfree(module);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;
	RDAezrpt *report=NULL;

	if(argc>2)
	{
		if(InitializeSubsystems(argc,argv,argv[1],"MAKE EZRPT CODE"))
		{
			RDAAPPMAINLOOP();
			return;
		}
		module=stralloc(argv[1]);
		report=GETRDAezrpt(module,argv[2]);
		if(report==NULL)
		{
			prterr("Error in permissions, users doesn't have access to read the report definition [%s][%s].  Check the permissions, and retry.",argv[1],argv[2]);
			if(module!=NULL) Rfree(module);
			ShutdownSubsystems();
		} else {
			makeezrptcode(module,report,stdout);
			if(module!=NULL) Rfree(module);
			ShutdownSubsystems();
		}
	} else {
		if(InitializeSubsystems(argc,argv,"UTILITIES","MAKE EZRPT CODE"))
		{
			ShutdownSubsystems();
			return;
		}
		getmodulelist(mainrsrc);
		mainrsrc=RDArsrcNEW("TOOLS","MAKE EZRPT CODE");
		x=0;
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
		addstdrsrc(mainrsrc,"TITLE",VARIABLETEXT,0,NULL,FALSE);
		addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
		getrptentries(mainrsrc);
		setvalues(mainrsrc);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,okreport,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitrptcode,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		FINDRSCSETINPUTFOCUS(mainrsrc,"MODULE LIST");
		APPmakescrn(mainrsrc,TRUE,quitrptcode,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
