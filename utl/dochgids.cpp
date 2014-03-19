#ifndef WIN32
#define __NAM__ "dochgids.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dochgids.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
#include <fin.hpp>
#include <pay.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module=NULL;
short filenum=0,chgfilenum=0;

void quitconv(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(filenum!=(-1)) CLSNRD(filenum);
	if(chgfilenum!=(-1)) CLSNRD(chgfilenum);
	if(module!=NULL) Rfree(module);
	ShutdownSubsystems();
}
short convertfield(RDArsrc *tmprsrc,RangeScreen *rs,short range_screen)
{
	short retval=FALSE,a=FALSE,ef=FALSE,rngkey=FALSE;
	char delflag=FALSE,*selectexp=NULL,*temp=NULL,*fieldname=NULL;
	char *matchvalue=NULL,*newvalue=NULL,*tmp=NULL,*tmp2=NULL;
	NRDfield *field=NULL;
	int length=0,x,len=0,len1=0,chgtype=0;

	if(range_screen) rngkey=setfilekeyFROMrangersrc(chgfilenum,1,rs);
	if(rngkey && range_screen)	
	{
		ef=ADVGTENRDsec(chgfilenum,1,SCRNvirtualSubData,tmprsrc);
	} else ef=ADVBEGNRDsec(chgfilenum,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETCHAR(chgfilenum,"DELETEFLAG",&delflag);
		if(range_screen)
		{
			a=testfilerangersrc(chgfilenum,rs);
		} else a=FALSE;
		if(!delflag && !a)
		{
			FINDFLDGETSTRING(chgfilenum,"SELECT EXPRESSION",&selectexp);
			if(!isEMPTY(selectexp))
			{
				a=EVALUATEbol(selectexp,SCRNvirtualSubData,tmprsrc);
			} else a=TRUE;
			if(a)
			{
				FINDFLDGETSTRING(chgfilenum,"FIELD NAME",&fieldname);
				if(!isEMPTY(fieldname))
				{
					FINDFLDGETSTRING(chgfilenum,"MATCH VALUE",&matchvalue);
					FINDFLDGETSTRING(chgfilenum,"NEW VALUE",&newvalue);
					FINDFLDGETINT(chgfilenum,"CHANGE TYPE",&chgtype);
					field=FLDNRD(filenum,fieldname);
					if(field!=NULL)
					{
						switch(field->type)
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
								if(!chgtype)
								{
									temp=EVALUATEstr(newvalue,SCRNvirtualSubData,tmprsrc);
									if(!isEMPTY(temp))
									{
										length=RDAstrlen(temp);
										QUICKALLOC(field->data.string_value,field->dlen,length);
										memset(field->data.string_value,0,field->dlen);
										memcpy(field->data.string_value,temp,length+1);
									} else if(field->dlen>0 && field->data.string_value!=NULL)
									{
										memset(field->data.string_value,0,field->dlen);
									}
									if(temp!=NULL) Rfree(temp);
								} else if(!isEMPTY(field->data.string_value))
								{
									while(TRUE)
									{
										if(!RDAstrstr(field->data.string_value,matchvalue)) break;
										len=RDAstrlen(matchvalue);
										len1=RDAstrlen(field->data.string_value);
										for(x=0;x<(len1-len+1);++x)
										{
											if(!strncmp(&field->data.string_value[x],matchvalue,len))
											{
												retval=TRUE;
												length=len1-3+RDAstrlen(newvalue);
												if(tmp!=NULL) Rfree(tmp);
												tmp=Rmalloc(length);
												if(x<1)
												{ 
													sprintf(tmp,"%s%s",newvalue,&field->data.string_value[len]);
												} else if(x<(len1-len))
												{
													sprintf(tmp,"%.*s%s%s",x,&field->data.string_value[0],newvalue,&field->data.string_value[x+len]);
												} else {
													if(tmp2!=NULL) Rfree(tmp2);
													tmp2=stralloc(field->data.string_value);
													tmp2[RDAstrlen(tmp2)-len]=0;
													sprintf(tmp,"%s%s",tmp2,
														newvalue);
												}
												if(!isEMPTY(tmp))
												{
													len=RDAstrlen(tmp);
													QUICKALLOC(field->data.string_value,field->dlen,len);
													memset(field->data.string_value,0,field->dlen);
													memcpy(field->data.string_value,tmp,len+1);
												} else if(field->data.string_value!=NULL)
												{
													memset(field->data.string_value,0,field->dlen);
												}
												if(tmp!=NULL) Rfree(tmp);
												break;
											}
										}
									}
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								*field->data.string_value=EVALUATEbol(newvalue,SCRNvirtualSubData,tmprsrc);
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								*field->data.float_value=EVALUATEdbl(newvalue,SCRNvirtualSubData,tmprsrc);
								break;
							case SHORTV:
							case SSHORTV:
								*field->data.short_value=EVALUATEsht(newvalue,SCRNvirtualSubData,tmprsrc);
								break;
							case LONGV:
							case SLONGV:
								*field->data.integer_value=EVALUATEint(newvalue,SCRNvirtualSubData,tmprsrc);
								break;
							default:
								prterr("Error Field Type [%d] is invalid in Field [%s].",field->type,field->name);
								break;
						}
					} else {
						prterr("Error Field [%s] not found in [%s][%s].",fieldname,MODULENAME(filenum),FILENAME(filenum));
					}
				} else {
					prterr("Error Field Name is NULL in CHANGE RECORD.");
				}
			}
		}
		if(rngkey && range_screen)
		{
			ef=ADVNXTNRDsec(chgfilenum,1,SCRNvirtualSubData,tmprsrc);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(chgfilenum,1,rs);
			}
		} else  ef=ADVSEQNRDsec(chgfilenum,SCRNvirtualSubData,tmprsrc);
	}
	if(selectexp!=NULL) Rfree(selectexp);
	if(fieldname!=NULL) Rfree(fieldname);
	if(matchvalue!=NULL) Rfree(matchvalue);
	if(newvalue!=NULL) Rfree(newvalue);
	if(tmp!=NULL) Rfree(tmp);
	if(tmp2!=NULL) Rfree(tmp2);
	return(retval);
}
void selectconv(RDArsrc *mainrsrc,short *rscreen)
{
	short ef=FALSE,dodiag=FALSE,rngkey=FALSE,range_screen=TRUE,a=FALSE;
	RDArsrc *tmprsrc=NULL;
	RangeScreen *rs=NULL;
	char *e=NULL,delflag=FALSE;
	RDATData *prev=FALSE;

	if(mainrsrc!=NULL)
	{
		readallwidgets(mainrsrc);
		rs=RangeScreenNew(mainrsrc);
		ReadRangeScreen(filenum,rs);
		ReadRangeScreen(chgfilenum,rs);
	}
	range_screen=*rscreen;
	tmprsrc=diagscrn(filenum,"DIAGNOSTIC SCREEN",module,
		"Selecting Records to Convert",NULL);
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
	addDFincvir(tmprsrc,MODULENAME(filenum),FILENAME(filenum),NULL,filenum);
	addDFincvir(tmprsrc,MODULENAME(chgfilenum),FILENAME(chgfilenum),NULL,
		chgfilenum);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	LOCNRDFILE(filenum);
	ZERNRD(filenum);
	if(range_screen)
	{
			rngkey=setfilekeyFROMrangersrc(filenum,1,rs);
		if(rngkey && range_screen) 
		{
			ef=ADVGTENRDsec(filenum,1,SCRNvirtualSubData,tmprsrc);
		} else ef=ADVBEGNRDsec(filenum,SCRNvirtualSubData,tmprsrc);
	} else ef=ADVBEGNRDsec(filenum,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&delflag);
		if(range_screen) a=testfilerangersrc(filenum,rs);
			else a=FALSE;
		if(!delflag && !a)
		{
			prev=RDATDataNEW(filenum);
			a=convertfield(tmprsrc,rs,range_screen);
			if(a) 
			{
				if(!ADVWRTTRANSsec(filenum,0,NULL,prev,SCRNvirtualSubData,tmprsrc)) RCKNRD(filenum);
				dodiag=TRUE;
			}
			if(prev!=NULL) FreeRDATData(prev);
		} else dodiag=FALSE;
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
			if(!range_screen) ForceWindowUpdate(tmprsrc);
		}
		if(rngkey && range_screen)
		{
			ef=ADVNXTNRDsec(filenum,1,SCRNvirtualSubData,tmprsrc);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(filenum,1,rs);
			}
		} else ef=ADVSEQNRDsec(filenum,SCRNvirtualSubData,tmprsrc);
	}
	UNLNRDFILE(filenum);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	quitconv(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	short range_screen=FALSE;
	char *s=NULL;

	if(argc>=4)
	{
		if(InitializeSubsystems(argc,argv,argv[1],argv[2])) 
		{
			return;
		}
		module=stralloc(argv[1]);
	} else {
		fprintf(stdout,"Error not enough arguments....");
		return;
	}
	s=RDA_GetEnv("RANGE_SCREEN");
	if(!isEMPTY(s)) range_screen=TRUE;
		else range_screen=FALSE;
	if((filenum=APPOPNNRD(argv[3],argv[4],TRUE,TRUE))==(-1)) return;
	if((chgfilenum=APPOPNNRD(argv[5],argv[6],TRUE,FALSE))==(-1)) return;
	if(!range_screen)
	{
		selectconv(NULL,&range_screen);
	}
	mainrsrc=RDArsrcNEW(argv[1],s);
	addDFincvir(mainrsrc,argv[3],argv[4],NULL,filenum);
	addDFincvir(mainrsrc,argv[5],argv[6],NULL,chgfilenum);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	file2rangersrc(filenum,mainrsrc);
	file2rangersrc(chgfilenum,mainrsrc);
	DefaultScreens(mainrsrc);
	range_screen=TRUE;
	addrfexrsrc(mainrsrc,"SELECT",TRUE,selectconv,&range_screen);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitconv,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitconv,NULL,TRUE);
}
