/* convdate.c - Program to convert the Dates thru the Xpert Package */
/*lint -library */
#ifndef WIN32
#define __NAM__ "convdate.lnx"
#endif
#ifdef WIN32
#define __NAM__ "convdate.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="DATABASE",*DateName=NULL,*DateValue=NULL;
char *OnThisModule=NULL,*OnThisFile=NULL,*DateExpn=NULL;

short MySubData(char **tmp,char *modulename)
{
        short start=1,length=0,str_length=0;
        char *temp=NULL,*gn=NULL;

        if(diagvirtual || diageval)
        {
                prterr("DIAG MySubData() Substituting for [%s].",*tmp);
        }
	if(!RDAstrcmp(modulename,"CHANGE DATE NAME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(DateName);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		return(0);
	}
	if(!RDAstrcmp(modulename,"CHANGE DATE VALUE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(DateValue);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		return(0);
	}
	if(!RDAstrcmp(modulename,"CHANGE CURRENT MODULE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(OnThisModule);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		return(0);
	}
	if(!RDAstrcmp(modulename,"CHANGE CURRENT FILENAME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(OnThisFile);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		return(0);
	}
	return(1);
}
void ConvDateSubData(char **tmp,RDArsrc *r)
{
	char *modulename=NULL;

	if(diagvirtual || diageval)
	{
		prterr("DIAG ConvDateSubData Substituting for [%s].",*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
		if(SCRNExecuteSubData(SCRNvirtualFIELDSubData,tmp,r,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ConvDateSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(MySubData(tmp,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ConvDateSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(SCRNExecuteSubData(SCRNvirtualVIRTUALSubData,tmp,r,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ConvDateSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(ScrolledListSubData(tmp,r)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ConvDateSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(GLOBALSubData(tmp,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ConvDateSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			prterr("Error ConvDateSubData [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
			if(diagvirtual || diageval)
			{
					prterr("DIAG ConvDateSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
	} else {
		prterr("Error ConvDateSubData [%s] not found.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=stralloc("\"\"");
		if(diagvirtual || diageval)
		{
			prterr("DIAG ConvDateSubData Returning [%s].",*tmp);
		}
		if(modulename!=NULL) Rfree(modulename);
	}
}

static void DateTest(char *mod,char *fil)
{
	short fileno=(-1),engine=0,nofields=0,nokeys=0;
	short keyno=(-1),ef=FALSE,offset=0,offcount=(-1);
	int x=0,y=0,z=0;
	NRDfield *fieldx=NULL,*fld=NULL,*field=NULL;
	NRDkey *keyx=NULL,*key; 
	NRDpart *part;
	DBsort *sort=NULL;	
	char *temp=NULL,*serverx=NULL;
	RDArsrc *diagscrn=NULL;

	if(OnThisModule!=NULL) Rfree(OnThisModule);
	OnThisModule=stralloc(mod);
	if(OnThisFile!=NULL) Rfree(OnThisFile);
	OnThisFile=stralloc(fil);
	fileno=OPNNRD(mod,fil);
	if(fileno==(-1)) 
	{
		prterr("Error:  Failed to open file [%s][%s]",(mod!=NULL ? mod:""),(fil!=NULL ? fil:""));
		return;
	}
	fieldx=FLDPOINTER(fileno);
	nofields=NUMFLDS(fileno);
	if(fieldx!=NULL)
	{
		for(x=0,fld=fieldx;x<nofields;++x,++fld)

		{
			if(fld->type==DATES) break;
		}
		if(x<nofields) /* found at least one date */
		{
		} else { /* Doesn't contain any dates */
			CLSNRD(fileno);
			return;
		}
	} else {
		prterr("Error:  No fields in file?");
		CLSNRD(fileno);
		return;
	}
	temp=unique_name();
	sort=DBsortNEW(mod,temp,NRDRdaEngine);
	if(temp!=NULL) Rfree(temp);
	fieldx=NULL;
	nofields=0;
	engine=0;
	keyx=NULL;
	nokeys=0;
	if(serverx!=NULL) Rfree(serverx);
	if(ADVGETBIN(mod,fil,&engine,&nofields,&fieldx,&nokeys,&keyx,&serverx))
	{
		prterr("Error:  Couldn't obtain binary definition of [%s][%s].",mod,fil);
	}
	if(keyx!=NULL)
	{
		key=keyx;
		keyno=addDBkey(sort,key->name);
		if(key->part!=NULL)
		{
			for(y=0,part=key->part;y<key->numparts;++y,++part)
			{
				addDBkeypart(sort,keyno,part->name);	
			}
		}
	} else {
		prterr("Error: No key parts???");
		return;
	}	
	if(fieldx!=NULL)
	{
		for(x=0,fld=fieldx;x<nofields;++x,++fld)
		{
			if(fld->type==DATES)
			{
				addDBfield(sort,fld->name,fld->type,10);
			} else {
				addDBfield(sort,fld->name,fld->type,fld->len);
			}
		}
	}
	sort->fileno=OPNDBsort(sort);
	if(sort->fileno!=(-1))
	{
		temp=Rmalloc(RDAstrlen(mod)+RDAstrlen(fil)+200);
		sprintf(temp,"Converting Dates in Database [%s][%s]",(mod!=NULL ? mod:""),(fil!=NULL ? fil:""));
		diagscrn=diagscrn(fileno,"DIAGNOSTIC SCREEN","DATABASE",temp,NULL);
		if(diagapps) 
		{
			prterr("DateTest(): %s",temp);TRACE;
		}
		addDFincvir(diagscrn,mod,fil,NULL,fileno);
		if(diagscrn!=NULL)
		{
			if(!ADVmakescrn(diagscrn,TRUE))
			{
				ForceWindowUpdate(diagscrn);
			}
		}
		if(temp!=NULL) Rfree(temp);
		if(DateValue!=NULL) Rfree(DateValue);
		if(DateName!=NULL) Rfree(DateName);
		ZERNRD(fileno);
		ef=BEGNRD(fileno);
		while(!ef)
		{
			if(diagapps) 
			{
				SEENRDRECORD(fileno);TRACE;
			}
			if(fieldx!=NULL)
			{
				for(x=0,fld=fieldx;x<nofields;++x,++fld)
				{
					if(fld->type==DATES)
					{
						if(diagapps) { prterr("DateTest(): Testing fieldlength of FieldName: (%s)",fld->name);TRACE; }
						if(DateName!=NULL) Rfree(DateName);
						DateName=stralloc(fld->name);
						FINDFLDGETSTRING(fileno,fld->name,&DateValue);
						if(RDAstrlen(DateValue)==8)
						{
							temp=EVALUATEstr(DateExpn,ConvDateSubData,diagscrn);
							if(diagapps) { prterr("FieldName (%s): From: (%s) To: (%s)",fld->name,(DateValue!=NULL?DateValue:""),(temp!=NULL?temp:""));TRACE; }
							FINDFLDSETSTRING(sort->fileno,fld->name,temp);
							if(temp!=NULL) Rfree(temp);
						} else { 
							if(((RDAstrlen(DateValue))!=10) && ((RDAstrlen(DateValue))>0))
							{
								if(!diagscrn) { SEENRDRECORD(fileno);TRACE; }
								prterr("WARNING: Possible Invalid Field Data for (%s) based on Length of (%d).",fld->name,RDAstrlen(DateValue));TRACE; 
							}
							if(diagapps) { prterr("DateTest(): Field not changed, reason: %s - length of %d",fld->name,RDAstrlen(DateValue));TRACE; }
							COPYFIELD(fileno,sort->fileno,fld->name);
						}
					} else {
						COPYFIELD(fileno,sort->fileno,fld->name);
					}
				}
				WRTNRD(sort->fileno,0,NULL);
			}
			ef=SEQNRD(fileno);
			if(diagscrn!=NULL) update_diagnostic(diagscrn,TRUE);
		}
		CLSNRD(fileno);
		if(diagscrn!=NULL)
		{
			killwindow(diagscrn);
			free_rsrc(diagscrn);
		}
		DELETENRD(mod,fil,engine,serverx);
		if(fieldx!=NULL)
		{
			for(x=0,fld=fieldx;x<nofields;++x,++fld)
			{
				if(fld->type==DATES)
				{
					fld->len=10;
				}
			}
		}	
		for(x=0,key=keyx;x<nokeys;++x,++key)
		{
			for(z=0,part=key->part;z<key->numparts;++z,++part)
			{
				offset=0;
				offcount=(-1);
				for(y=0,field=fieldx;y<nofields;++y,++field)
				{
					if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
					{
						++offcount;
					}
					if(!RDAstrcmp(field->name,part->name))
					{
						if(engine==NRDCtreeEngine || engine==NRDLocalCtreeEngine)
						{
							if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
							{
								part->offset=offcount;
							} else part->offset=offset;
						} else part->offset=offset;
						break;
					}
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
							if(engine==NRDInformixIsamEngine)
							{ 
								offset+=field->len;
							} else offset+=field->len+1;
							break;
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							offset+=field->len;
							break;
						case DECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DOUBLEV:
						case SDECIMALV:
						case SDOUBLEV:
							offset+=sizeof(double);
							break;
						case SHORTV:
						case SSHORTV:
							offset+=sizeof(short);
							break;
						case LONGV:
						case SLONGV:
							offset+=sizeof(int);
							break;
						case BOOLNS:
						case CHARACTERS:
							offset+=1;
							break;
						case BUTTONS:
						case SCROLLEDLIST:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
				}
			}
		}
		fileno=CRTNRD(mod,engine,fil,nofields,fieldx,nokeys,keyx,serverx);
		if(fileno<0)
		{
			prterr("Error:  Unable to make changes to the database!!!!");
		} else {
			temp=Rmalloc(RDAstrlen(mod)+RDAstrlen(fil)+200);
			sprintf(temp,"Writing New Database [%s][%s]",(mod!=NULL ? mod:""),(fil!=NULL ? fil:""));
			diagscrn=diagscrn(sort->fileno,"DIAGNOSTIC SCREEN","DATABASE",temp,NULL);
			addDFincvir(diagscrn,mod,fil,NULL,fileno);
			if(diagscrn!=NULL)
			{
				if(!ADVmakescrn(diagscrn,TRUE))
				{
					ForceWindowUpdate(diagscrn);
				}
			}
			ZERNRD(sort->fileno);
			ef=BEGNRD(sort->fileno);
			while(!ef)
			{
				if(fieldx!=NULL)
				{
					for(x=0,fld=fieldx;x<nofields;++x,++fld)
					{
						COPYFIELD(sort->fileno,fileno,fld->name);
					}
					WRTNRD(fileno,0,NULL);
				}
				ef=SEQNRD(sort->fileno);
				if(diagscrn!=NULL) update_diagnostic(diagscrn,TRUE);
			}			
			CLSNRD(fileno);
			if(diagscrn!=NULL)
			{
				killwindow(diagscrn);
				free_rsrc(diagscrn);
			}
		}
	} else {
		prterr("Error: Unable to create sort file....");
	}
	CLSDBsort(sort);
	FreeDBsort(sort);
	if(fieldx!=NULL)
	{
		for(x=0,fld=fieldx;x<nofields;++x,++fld)
		{
			if(fld->name!=NULL) Rfree(fld->name);
		}
		Rfree(fieldx);
	}
	if(keyx!=NULL)
	{
		for(x=0,key=keyx;x<nokeys;++x,++key)
		{
			if(key->name!=NULL) Rfree(key->name);
			if(key->part!=NULL)
			{
				for(y=0,part=key->part;y<key->numparts;++y,++part)
				{
					if(part->name!=NULL) Rfree(part->name);
				}
				Rfree(key->part);
			}
		}
		Rfree(keyx);
	}
	if(serverx!=NULL) Rfree(serverx);
}
static void convdate(RDArsrc *mainrsrc)
{
	APPlib *modlst=NULL;
	APPlib *file_def=NULL;
	char rng_mod=FALSE,*mod_from=NULL,*mod_to=NULL;
	char rng_fil=FALSE,*fil_from=NULL,*fil_to=NULL;
	int x,y;
	char *tmp=NULL,*dirx=NULL,*libx=NULL;

	readallwidgets(mainrsrc);	
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	FINDRSCGETCHAR(mainrsrc,"RANGE ON MODULE",&rng_mod);
	FINDRSCGETSTRING(mainrsrc,"FROM MODULE",&mod_from);
	FINDRSCGETSTRING(mainrsrc,"TO MODULE",&mod_to);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON FILENAME",&rng_fil);
	FINDRSCGETSTRING(mainrsrc,"FROM FILENAME",&fil_from);
	FINDRSCGETSTRING(mainrsrc,"TO FILENAME",&fil_to);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&DateExpn);

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(rng_mod==TRUE)
		{
			if((RDAstrcmp(tmp,mod_from)>=0) &&
				(RDAstrcmp(tmp,mod_to)<=0))
			{
				addAPPlib(modlst,tmp);
			}
		} else { 
			addAPPlib(modlst,tmp);
		}
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(mod_from!=NULL) Rfree(mod_from);
	if(mod_to!=NULL) Rfree(mod_to);	
	if(modlst->numlibs>0)
	{
		SORTAPPlib(modlst);
		for(x=0;x<modlst->numlibs;++x)
		{
			if(file_def!=NULL) freeapplib(file_def); 
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
			file_def=getlibnames(libx,TRUE);
			if(libx!=NULL) Rfree(libx);
			if(file_def!=NULL)
			{
				for(y=0;y<file_def->numlibs;++y)
				{
					if(rng_fil==TRUE)
					{
						if((RDAstrcmp(fil_from,file_def->libs[y])>=0) && (RDAstrcmp(fil_to,file_def->libs[y])<=0))
						{
							DateTest(modlst->libs[x],file_def->libs[y]);
						}
					} else {
						DateTest(modlst->libs[x],file_def->libs[y]);
					}
				}
				freeapplib(file_def);
			}
		}	
	}
	if(modlst!=NULL) freeapplib(modlst);
}
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(DateName!=NULL) Rfree(DateName);
	if(DateValue!=NULL) Rfree(DateValue);
	if(OnThisModule!=NULL) Rfree(OnThisModule);
	if(OnThisFile!=NULL) Rfree(OnThisFile);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char b=FALSE;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"CONVERT DATE")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"CONVERT DATE");
	addstdrsrc(mainrsrc,"RANGE ON MODULE",BOOLNS,1,&b,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"RANGE ON FILENAME",BOOLNS,1,&b,TRUE);
	addstdrsrc(mainrsrc,"FROM FILENAME",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO FILENAME",VARIABLETEXT,0,NULL,TRUE);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,convdate,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
