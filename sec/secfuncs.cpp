#include <sec.hpp>

short DEPART_FILENO=(-1);
short SEC_TOKEN_FILENO=(-1);
short SEC_USERS_FILENO=(-1);
short MODSEC_FILENO=(-1);
short PROCSEC_FILENO=(-1);
short FILESEC_FILENO=(-1);
short FLDSEC_FILENO=(-1);
short SEC_FINEACC=(-1);
short SEC_FINRACC=(-1);
short SEC_FINBACC=(-1);

short UsersDoMenuStyle=(-1);
char *UsersScanDirectory=NULL;
char *UsersDefaultPrinter=NULL;
char *UsersDepartment=NULL;
char *UsersSiteID=NULL;
char *UsersLocationID=NULL;

char *SECURITY_USERLOGIN=NULL;
#define _SKIP_RDA_NOGUI_ 

void setSECFIELDS(SECmodule *smod,SECfile *sfile,short fileno)
{
	NRDfield *f,*fields=NULL;
	int y;
	short nofields=0;

	fields=FLDPOINTER(fileno);
	nofields=NUMFLDS(fileno);
	if(fields!=NULL)
	{
		for(y=0,f=fields;y<nofields;++y,++f)
		{
			GetFIELDSEC(smod,sfile,f->name);
		}
	}
}
short xOPNNRDsec(char *modname,char *filename,char aread,char awrite,
	char aprint,int line,char *file)
/* open's a file and index file with security */
{
	SECmodule *smod=NULL;
	SECfile *sfile=NULL;
	short fileno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xOPNNRDsec Open Module [%s] File [%s] with Security at line [%d] program [%s].",modname,filename,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(SECURITY_USERLOGIN,"root")) return(OPNNRD(modname,filename));
	if(filename==NULL) return(-1);
	smod=FINDMODSEC(modname);
	if(smod==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG OPNNRDsec Returning Access Denied to Module [%s] at line [%d] program [%s].",modname,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	if((aread && !smod->read) || (awrite && !smod->write)
		|| (aprint && !smod->print)) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG OPNNRDsec Returning Access Denied to Module [%s] at line [%d] program [%s].",modname,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	if(smod->allfiles) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG OPNNRDsec Returning Access Granted to ALL FILES in Module [%s] at line [%d] program [%s].",modname,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(OPNNRD(modname,filename));
	}
	sfile=FINDFILESEC(smod,filename);
	if(sfile==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG OPNNRDsec Returning Access Denied to Module [%s] File [%s] at line [%d] program [%s].",modname,filename,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	if(((aread && sfile->read) || !aread) && ((awrite && sfile->write)
		|| !awrite) && ((aprint && sfile->print) || !aprint)) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG FILEsecurity Returning Access Granted to Module [%s] File [%s] at line [%d] program [%s].",modname,filename,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		fileno=OPNNRD(modname,filename);
		if(sfile->fields==NULL) 
		{
			setSECFIELDS(smod,sfile,fileno);
		}
		return(fileno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG OPNNRDsec Returning Access Denied to Module [%s] File [%s] at line [%d] program [%s].",modname,filename,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(-1);
}
short xEQLNRDKEYsec(short fileno,short keyno,void *target,void (*SubFunc)(...),void *args,int line,char *file)
/* find's a record with an equal key using record level security */
{
	short x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xEQLNRDKEYsec Get Equal Record with Equal Key using Security for file [%d] [%s] key [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=EQLNRDKEY(fileno,keyno,target);
	if(!x) 
	{
		if(SubFunc!=NULL)
		{
			if(ADVRECORDsec(fileno,SubFunc,args)) x=(-1);
		} else {
			if(RECORDsec(fileno)) x=(-1);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG EQLNRDKEYsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xEQLNRDsec(short fileno,short keyno,void (*SubFunc)(...),void *args,int line,char *file)
/* finds a an equal record using record level security */
{
	short x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xEQLNRDsec Get Equal Record with using Security for file [%d] [%s] key [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=EQLNRD(fileno,keyno);
	if(!x) 
	{
		if(SubFunc!=NULL)
		{
			if(ADVRECORDsec(fileno,SubFunc,args)) x=(-1);
		} else if(RECORDsec(fileno)) x=(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG EQLNRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xBEGNRDsec(short fileno,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x,y=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xBEGNRDsec Get First Record in Physical Sequence using Security for File [%d] [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=BEGNRD(fileno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=SEQNRD(fileno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG BEGNRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xGTENRDsec(short fileno,short keyno,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x,y=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xGTENRDsec Get Record Greater Than or Equal using Security for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=GTENRD(fileno,keyno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=NXTNRD(fileno,keyno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG GTENRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xPRVNRDsec(short fileno,short keyno,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x,y=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xPRVNRDsec Get Record Previous Than Equal using Security for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=PRVNRD(fileno,keyno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=PRVNRD(fileno,keyno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG PRVNRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xLSTNRDsec(short fileno,short keyno,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x,y=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xLSTNRDsec Get Record Less Than Equal using Security for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=LSTNRD(fileno,keyno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=PRVNRD(fileno,keyno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG LSTNRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xLTENRDsec(short fileno,short keyno,void (*SubFunc)(...),void *args,int line,char *file)
/* finds a record less than or equal to using security */
{
	short x,y=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xLTENRDsec Get Record Less Than or Equal using Security for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=LTENRD(fileno,keyno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=PRVNRD(fileno,keyno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG LTENRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xFRSNRDsec(short fileno,short keyno,void (*SubFunc)(...),void *args,int line,char *file)
/* finds the first record in key sequence using record level security */
{
	short x=FALSE,y=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xFRSNRDsec Get First Record in Sorted Sequence using Security for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=FRSNRD(fileno,keyno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=NXTNRD(fileno,keyno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG FRSNRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xSEQNRDsec(short fileno,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x,y=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xSEQNRDsec Get Next Record in Physical Sequence using Security for file [%d] [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=SEQNRD(fileno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=SEQNRD(fileno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG SEQNRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
short xNXTNRDsec(short fileno,short keyno,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x,y=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xNXTNRDsec Get Next Record in Sorted Sequence using Security for file [%d] [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	x=NXTNRD(fileno,keyno);
	while(!x)
	{
		if(SubFunc!=NULL)
		{
			y=ADVRECORDsec(fileno,SubFunc,args);
		} else y=RECORDsec(fileno);
		if(!y) break;
		x=NXTNRD(fileno,keyno);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG NXTNRDsec Returning [%s] to record in Module [%s] File [%s] at line [%d] program [%s].",(x ? "Denied":"Granted"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(x);
}
/*----------------------------------------------------------------------------
	Field Level Security Functions
----------------------------------------------------------------------------*/
short xFIELDscrn(short fileno,char *fieldname,short *editable,int line,char *file)
{
	SECmodule *smod=NULL;
	SECfile *sfile=NULL;
	SECfield *sfield=NULL;
	char *module=NULL,*filename;
	NRDfield *suv=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG Checking field [%s] for screen access editable request [%s] at line [%d] program [%s].",fieldname,(editable ? "Yes":"No"),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(SECURITY_USERLOGIN,"root")) return(TRUE);
	if(!TESTFILENO(fileno,line,file)) return(FALSE);
	module=MODULENAME(fileno);
	smod=FINDMODSEC(module);
	if(smod==NULL) return(FALSE);
	if(!RDAstrcmp(fieldname,"DELETEFLAG") && strncmp(module,"RPTGEN",4) && !isEMPTY(module))
	{
		suv=FLDNRD(fileno,"SOURCE USER VERIFIED");
		if(suv!=NULL)
		{
			if(*suv->data.string_value)
			{
				if(smod->can_delete==FALSE) *editable=FALSE;
					else *editable=TRUE;
			} else {
				*editable=TRUE;
			}
		} else {
			if(smod->can_delete==FALSE) *editable=FALSE;
				else *editable=TRUE;
		}
		if(smod->allfiles) return(TRUE);
	} else if(!RDAstrcmp(fieldname,"DELETEFLAG"))
	{
		*editable=TRUE;
		if(smod->allfiles) return(TRUE);
	} else {
		if(smod->allfiles) return(TRUE);
	}
	filename=FILENAME(fileno);
	sfile=FINDFILESEC(smod,filename);
	if(sfile==NULL) return(FALSE);
	if(!sfile->read) return(FALSE);
	if(*editable && !sfile->write) *editable=FALSE;
	if(!RDAstrcmp(fieldname,"DELETEFLAG"))
	{
		if(sfile->can_delete==FALSE) *editable=FALSE;
		suv=FLDNRD(fileno,"SOURCE USER VERIFIED");
		if(suv!=NULL)
		{
			if(*suv->data.string_value)
			{
				if(sfile->can_delete==FALSE) *editable=FALSE;
					else *editable=TRUE;
			} else {
				*editable=TRUE;
			}
		} else {
			if(sfile->can_delete==FALSE) *editable=FALSE;
				else *editable=TRUE;
		}
	}
	if(!sfile->fieldsecurity) return(TRUE);
	sfield=FINDFIELDSEC(smod,sfile,fieldname);
	if(sfield==NULL) return(FALSE);
	if(!sfield->read) return(FALSE);
	if(*editable && !sfield->write) *editable=FALSE;
	return(TRUE);
}
NRDfield *FIELDfromRDATData(RDATData *p,char *field)
{
	int x=0;
	NRDfield *f=NULL;

	if(p!=NULL)
	{
		for(x=0,f=p->field;x<p->num;++x,++f)
		{
			if(!RDAstrcmp(f->name,"DELETEFLAG")) break;
		}
	}
	return(f);
}
short ABLEtoDELETE(char *module,char *filename)
{
	SECmodule *m=NULL;
	SECfile *f=NULL;

	m=FINDMODSEC(module);
	if(m!=NULL)
	{
		if(m->can_delete==FALSE) return(FALSE);
		if(m->allfiles==TRUE) return(TRUE);
	} else {
		return(FALSE);
	}
	f=FINDFILESEC(m,filename);
	if(f!=NULL)
	{
		if(f->can_delete==TRUE) return(TRUE);
	} else {
		return(FALSE);
	}
	return(FALSE);
}
short DELETEFLAGsec(short fileno,RDATData *previous)
{
	NRDfield *ffld=NULL,*pfld=NULL;

	ffld=FLDNRD(fileno,"DELETEFLAG");
	pfld=FIELDfromRDATData(previous,"DELETEFLAG");
	if(ffld!=NULL && pfld!=NULL)
	{
		if(COMPARENRDfield(ffld,pfld) && ABLEtoDELETE(MODULENAME(fileno),FILENAME(fileno)))
		{
			return(FALSE);
		} else {
			return(TRUE);
		}
	} else {
		if(ABLEtoDELETE(MODULENAME(fileno),FILENAME(fileno))) return(TRUE);
	}
	return(FALSE);
}
short xWRTNRDsec(short fileno,short trnmode,char *fieldname,
	void (*SubFunc)(...),void *args,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xWRTNRDsec Write Current Record using Security for file [%d] [%s] trnmode [%d] fieldname [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),trnmode,fieldname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SubFunc!=NULL)
	{
		if(!ADV2RECORDsec(fileno,SubFunc,args,TRUE)) 
		{
			return(WRTNRD(fileno,trnmode,fieldname));
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("DIAG Record in Module [%s] File [%s] contains illegal values at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	} else {
		if(!ADV2RECORDsec(fileno,NULL,NULL,TRUE))
		{
			return(WRTNRD(fileno,trnmode,fieldname));
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("DIAG Record in Module [%s] File [%s] contains illegal values at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	}
}
void SET_SECURITY_USERLOGIN()
{
	char *temp=NULL;
	char s=FALSE,d=FALSE;
	
	if(!RDAstrcmp(USERLOGIN,"root"))
	{
		SECURITY_USERLOGIN=stralloc(USERLOGIN);
		return;
	}
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	temp=Users_Department();
	FINDFLDSETSTRING(DEPART_FILENO,"DEPARTMENT",temp);
	if(!EQLNRD(DEPART_FILENO,1))
	{
		FINDFLDGETCHAR(DEPART_FILENO,"DELETEFLAG",&d);
		if(d==FALSE) 
		{
			FINDFLDGETCHAR(DEPART_FILENO,"SECURITY",&s);
		}
	}
	if(s==FALSE) 
	{
		SECURITY_USERLOGIN=USERLOGIN;
		if(temp!=NULL) Rfree(temp);
	} else {
		SECURITY_USERLOGIN=stralloc(temp);
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG SECURITY_USERLOGIN [%s].",(SECURITY_USERLOGIN!=NULL ? SECURITY_USERLOGIN:""));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
/*---------------------------------------------------------------------------
GENERAL SECURITY FUNCTIONS 
---------------------------------------------------------------------------*/
short xInitializeSecurity(int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG InitializeSecurity Initializing Security System at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	DEPART_FILENO=(-1);
	MODSEC_FILENO=(-1);
	PROCSEC_FILENO=(-1);
	FILESEC_FILENO=(-1);
	FLDSEC_FILENO=(-1);
/*
	SEC_USERS_FILENO=(-1);
	SEC_TOKEN_FILENO=(-1);
*/
	SEC_FINEACC=(-1);
	SEC_FINBACC=(-1);
	SEC_FINRACC=(-1);
	UsersScanDirectory=NULL;
	UsersDefaultPrinter=NULL;
	UsersDepartment=NULL;
	UsersSiteID=NULL;
	UsersLocationID=NULL;
	UsersDoMenuStyle=(-1);
	numSECMOD=0;
	SECMOD=NULL;
	if(RDAstrcmp("root",USERLOGIN))
	{
		if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
		if(SEC_TOKEN_FILENO==(-1)) SEC_TOKEN_FILENO=OPNNRD("SECURITY","TOKEN");
		if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
		if((MODSEC_FILENO=OPNNRD("SECURITY","MODSEC"))==(-1)) return(-1);
		if((PROCSEC_FILENO=OPNNRD("SECURITY","PROCSEC"))==(-1))
		{
			CLSNRD(SEC_USERS_FILENO);
			CLSNRD(SEC_TOKEN_FILENO);
			CLSNRD(DEPART_FILENO);
			CLSNRD(MODSEC_FILENO);
			prterr("Error cannot Initialize Security at line [%d] program [%s].",line,file);
			return(-1);
		}
		if((FILESEC_FILENO=OPNNRD("SECURITY","FILESEC"))==(-1))
		{
			CLSNRD(SEC_USERS_FILENO);
			CLSNRD(SEC_TOKEN_FILENO);
			CLSNRD(DEPART_FILENO);
			CLSNRD(MODSEC_FILENO);
			CLSNRD(PROCSEC_FILENO);
			prterr("Error cannot Initialize Security at line [%d] program [%s].",line,file);
			return(-1);
		}
		if((FLDSEC_FILENO=OPNNRD("SECURITY","FLDSEC"))==(-1))
		{
			CLSNRD(SEC_USERS_FILENO);
			CLSNRD(SEC_TOKEN_FILENO);
			CLSNRD(DEPART_FILENO);
			CLSNRD(MODSEC_FILENO);
			CLSNRD(PROCSEC_FILENO);
			CLSNRD(FILESEC_FILENO);
			prterr("Error cannot Initialize Security at line [%d] program [%s].",line,file);
			return(-1);
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec)
		{
			prterr("DIAG Security successfully initialized at line [%d] program [%s].",line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec)
		{
			prterr("DIAG User Logged in as ROOT, Bypassing Security at line [%d] program [%s].",line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	}
	SET_SECURITY_USERLOGIN();
	return(0);
}
void xShutdownSecurity(int line,char *file)
{
	SECmodule *smod;
	SECfile *sfile;
	SECfield *sfield;
	int x,y,z;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG ShutdownSecurity Stopping Security and Closing Security Files at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	FreeInternalRDATables();
	Free_Internal_RDAMATRIXES();
	FreeInternalGenericSetup();
	if(SEC_USERS_FILENO!=(-1)) CLSNRD(SEC_USERS_FILENO);
	if(SEC_TOKEN_FILENO!=(-1)) CLSNRD(SEC_TOKEN_FILENO);
	if(DEPART_FILENO!=(-1)) CLSNRD(DEPART_FILENO);
	if(MODSEC_FILENO!=(-1)) CLSNRD(MODSEC_FILENO);
	if(PROCSEC_FILENO!=(-1)) CLSNRD(PROCSEC_FILENO);
	if(FILESEC_FILENO!=(-1)) CLSNRD(FILESEC_FILENO);
	if(FLDSEC_FILENO!=(-1)) CLSNRD(FLDSEC_FILENO);
	if(ARCHWORK_FILENO!=(-1)) CLSNRD(ARCHWORK_FILENO);
	if(ARCHUSER_FILENO!=(-1)) CLSNRD(ARCHUSER_FILENO);
	if(SEC_FINEACC!=(-1)) CLSNRD(SEC_FINEACC);
	if(SEC_FINRACC!=(-1)) CLSNRD(SEC_FINRACC);
	if(SEC_FINBACC!=(-1)) CLSNRD(SEC_FINBACC);
	UsersDoMenuStyle=(-1);
/*
	if(UsersScanDirectory!=NULL) Rfree(UsersScanDirectory);
	if(UsersDefaultPrinter!=NULL) Rfree(UsersDefaultPrinter);
	if(UsersDepartment!=NULL) Rfree(UsersDepartment);
	if(UsersSiteID!=NULL) Rfree(UsersSiteID);
	if(UsersLocationID!=NULL) Rfree(UsersLocationID);
*/
	if(SECMOD!=NULL)
	{
		for(x=0,smod=SECMOD;x<numSECMOD;++x,++smod)
		{
			if(smod->module!=NULL) Rfree(smod->module);
			if(smod->files!=NULL)
			{
				for(y=0,sfile=smod->files;y<smod->num;++y,++sfile)
				{
					if(sfile->file!=NULL) Rfree(sfile->file);
					if(sfile->fields!=NULL)
					{
						for(z=0,sfield=sfile->fields;z<sfile->num;
							++z,++sfield)
						{
							if(sfield->name!=NULL) Rfree(sfield->name);
						}
						Rfree(sfile->fields);
					}
					if(sfile->expression!=NULL) Rfree(sfile->expression);
					if(sfile->p_expression!=NULL) Rfree(sfile->p_expression);
				}
				Rfree(smod->files);
			}
		}
		Rfree(SECMOD);
		SECMOD=NULL;
		numSECMOD=0;
	}
}
short xWRTNRDtranssec(short fileno,short trnmode,char *fieldname,void (*SubFunc)(...),void *args,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xWRTNRDtranssec Write Record using Transaction Processing and Security for File [%d] [%s] trnmode [%d] fieldname [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),trnmode,fieldname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SubFunc!=NULL)
	{
		if(!ADVRECORDsec(fileno,SubFunc,args)) 
		{
			return(WRTNRDtrans(fileno,trnmode,fieldname));
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("Error Record in Module [%s] File [%s] contains illegal values at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	} else {
		if(!RECORDsec(fileno)) 
		{
			return(WRTNRDtrans(fileno,trnmode,fieldname));
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("Error Record in Module [%s] File [%s] contains illegal values at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	}
}
short Users_DomenuStyle()
{
	short style=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Domenu Style.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(UsersDoMenuStyle==(-1))
	{
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSHORT(SEC_USERS_FILENO,"DOMENU STYLE",&style);
		}
		UsersDoMenuStyle=style;
	}
	} else {
		style=UsersDoMenuStyle;
	}
	return(style);
}
char *Department_Supervisor(char *dept)
{
	char *s=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning Department's Supervisor.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			COPYFIELD(SEC_USERS_FILENO,DEPART_FILENO,"DEPARTMENT");
			if(!EQLNRD(DEPART_FILENO,1))
			{
				FINDFLDGETSTRING(DEPART_FILENO,"SUPERVISOR",&s);
			}
		}
	}
	return(s);
}
char *Users_Scan_Directory()
{
	char *scandir=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Scan Directory.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(isEMPTY(UsersScanDirectory))
	{
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"SCAN DIRECTORY",&scandir);
		}
		UsersScanDirectory=stralloc(scandir);
	}
	} else {
		scandir=stralloc(UsersScanDirectory);
	}
	return(scandir);
}
char Users_BrowseListLabel()
{
#ifndef __GTK_H__
	char use_label=FALSE;
#endif
#ifdef __GTK_H__
	char use_label=TRUE;
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Browse List Label selection.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"USE BROWSE LIST LABEL",&use_label);
		}
	}
	return(use_label);
}

char *xUsers_Department(int line,char *file)
{
	char *department=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Department from line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef XXXXX
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL); 
#endif /* _SKIP_RDA_NOGUI_ */
#endif /* XXXXX */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(isEMPTY(UsersDepartment))
	{
		if(SEC_USERS_FILENO!=(-1))
		{
			FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
			if(!EQLNRD(SEC_USERS_FILENO,1))
			{
				FINDFLDGETSTRING(SEC_USERS_FILENO,"DEPARTMENT",&department);
			}
		}
		if(isEMPTY(department))
		{
			if(!RDAstrcmp(SECURITY_USERLOGIN,"root"))
			{
				department=stralloc("admin");
			} else if(!RDAstrcmp(SECURITY_USERLOGIN,"GUEST"))
			{
				department=stralloc("guest");
			}
		}
		UsersDepartment=stralloc(department);
	} else {
		department=stralloc(UsersDepartment);
	}
	return(department);
}
char *DefaultPrinter()
{
	if(isEMPTY(UsersDefaultPrinter))
	{
#ifndef WIN32
		UsersDefaultPrinter=stralloc("viewpdf.lnx");
#else
		UsersDefaultPrinter=stralloc("viewpdf.exe");
#endif
	}
	return(UsersDefaultPrinter);
}
short xWRTTRANSsec(short fileno,short trnmode,char *fieldname,
	RDATData *previous,void (*SubFunc)(...),void *args,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xWRTTRANSsec Write Record using Transaction Processing and Security for File [%d] [%s] trnmode [%d] fieldname [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),trnmode,fieldname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SubFunc!=NULL)
	{
		if(!ADVRECORDsec(fileno,SubFunc,args))
		{
			return(WRTTRANS(fileno,trnmode,fieldname,previous));
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("Error Record in Module [%s] File [%s] contains illegal values at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	} else {
		if(!RECORDsec(fileno))
		{
			return(WRTTRANS(fileno,trnmode,fieldname,previous));
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("Error Record in Module [%s] File [%s] contains illegal values at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	}
}
#ifdef RADDOG
void xCLSNRDsec(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG CLSNRDsec for fileno [%d] at line [%d] program [%s].",fileno,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	CLSNRD(fileno);
}
#endif
char *DefaultMediaOutDevice()
{
	char *outputdevice=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG DefaultFileOutDevice Reading/Returning User's Preferred Output Device.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"DEFAULT FILE OUTPUT DEVICE",&outputdevice);
		}
	}
	if(isEMPTY(outputdevice))
	{
#ifdef RS6000
		outputdevice=stralloc("/dev/fd0");
#endif
#ifdef SCO50P
		outputdevice=stralloc("/dev/fd0");
#endif
#ifdef UNIXWARE7
		outputdevice=stralloc("/dev/rdsk/f03ht");
#endif
#ifdef UNIXWARE21
		outputdevice=stralloc("/dev/fd0");
#endif
#ifdef LINUX
		outputdevice=stralloc("A:");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		outputdevice=stralloc("A:");
#endif
#ifdef SUNX86
		outputdevice=stralloc("/dev/fd0");
#endif
#ifdef WIN32
		outputdevice=stralloc("A:");
#endif
	}
	return(outputdevice);
}
char Users_Able_to_Approve()
{
	char approve=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Department.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"ABLE TO APPROVE",&approve);
		}
	} else {
		if(!RDAstrcmp(USERLOGIN,"root"))
		{
			approve=TRUE;
		} else approve=FALSE;
	}
	return(approve);
}
double Users_Approval_Limit()
{
	char approve=FALSE;
	double alimit=0.0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Department.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(0.0);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"ABLE TO APPROVE",&approve);
			if(approve==TRUE)
			{
				FINDFLDGETDOUBLE(SEC_USERS_FILENO,"APPROVAL LIMIT",&alimit);
			}
		}
	} else {
		if(!RDAstrcmp(USERLOGIN,"root"))
		{
			approve=TRUE;
			alimit=9999999999999999.0;
		} else if(!RDAstrcmp(SECURITY_USERLOGIN,"GUEST"))
		{
			approve=FALSE;
		} else approve=FALSE;
	}
	return(alimit);
}
char *Users_Phone()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Phone.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PHONE NUMBER",&phone);
		}
	}
	return(phone);
}
char *Users_Phone_Extension()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Phone.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PHONE EXTENSION",&phone);
		}
	}
	return(phone);
}
char *Users_Login()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Web Login.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"WEB LOGIN",&phone);
		}
	}
	return(phone);
}
char *Users_Password()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Web Password.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"WEB PASSWORD",&phone);
		}
	}
	return(phone);
}
char *Users_Email()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Email Address.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"EMAIL",&phone);
		}
	}
	return(phone);
}
char *Users_SiteID()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Site Identification.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(isEMPTY(UsersSiteID))
	{
		if(SEC_USERS_FILENO!=(-1))
		{
			FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
			if(!EQLNRD(SEC_USERS_FILENO,1))
			{
				FINDFLDGETSTRING(SEC_USERS_FILENO,"SITE IDENTIFICATION",&phone);
			}
			UsersSiteID=stralloc(phone);
		}
	} else {
		phone=stralloc(UsersSiteID);
	}
	return(phone);
}
char *Users_Supervisor()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Supervisor.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			COPYFIELD(SEC_USERS_FILENO,DEPART_FILENO,"DEPARTMENT");
			if(!EQLNRD(DEPART_FILENO,1))
			{
				FINDFLDGETSTRING(DEPART_FILENO,"SUPERVISOR",&phone);
			}
		}
	}
	return(phone);
}
char *UsersVenpmtSignatureFilename()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Venpmt Signature File.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"VENPMT SIGNATURE FILENAME",&phone);
		}
	}
	return(phone);
}
char *UsersVenpmtSignaturePassword()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Venpmt Signature File.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"VENPMT SIGNATURE PASSWORD",&phone);
		}
	}
	return(phone);
}
char *UsersPayrollSignatureFilename()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Payroll Signature File.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PAYROLL SIGNATURE FILENAME",&phone);
		}
	}
	return(phone);
}
char *UsersPayrollSignaturePassword()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Payroll Signature File.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PAYROLL SIGNATURE PASSWORD",&phone);
		}
	}
	return(phone);
}
char *UsersPurordSignatureFilename()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Purord Signature File.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PURORD SIGNATURE FILENAME",&phone);
		}
	}
	return(phone);
}
char *UsersPurordSignaturePassword()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Purord Signature File.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"PURORD SIGNATURE PASSWORD",&phone);
		}
	}
	return(phone);
}
short UsersPushButtonStyle()
{
	short style=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's PushButton Style.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(-1);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSHORT(SEC_USERS_FILENO,"PUSHBUTTON STYLE",&style);
		}
	}
	return(style);
}
char UsersAllowPostActivity()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Allow Post Activity.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"ALLOW POST ACTIVITY",&style);
		}
	}
	return(style);
}
char UsersAllowMultipleEntries()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Allow Multiple Entries.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"ALLOW MULTIPLE ENTRIES",&style);
		}
	}
	return(style);
}
char UsersAllowBeginningBalances()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Allow Beginning Balances.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"ALLOW BEGINNING BALANCES",&style);
		}
	}
	return(style);
}
char *Users_Name()
{
	char *current_RDAuser_name=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Name.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(NULL); 
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"USER NAME",&current_RDAuser_name);
		}
	}
	return(current_RDAuser_name);
}
char UsersJournalEntryApprove()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Journal Entry Approval.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"JOURNAL ENTRY APPROVAL",&style);
		}
	}
	return(style);
}
char UsersVoucherApprove()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Voucher Approval.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"VOUCHER APPROVAL",&style);
		}
	}
	return(style);
}
char AdminEdits()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Admin Edits.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"ADMIN EDITS",&style);
		}
	}
	return(style);
}
char SubmitRequisitions()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Submit Requisitions.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"SUBMIT REQUISITIONS",&style);
		}
	}
	return(style);
}
char ApproveRequisitions()
{
	char style=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Approve Requisitions.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"APPROVE REQUISITIONS",&style);
		}
	}
	return(style);
}
char UsersAutoComplete()
{
	char style=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Auto Complete.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"AUTO COMPLETE",&style);
		}
	}
	return(style);
}
char UsersDisengageWorkFlowNavigation()
{
	char style=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Disengage Work Flow Navigation.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"DISENGAGE WORK FLOW NAVIGATION",&style);
		}
	}
	return(style);
}
char UsersIgnoreRestrictedProcessConstraints()
{
	char style=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Ignore Restricted Process Constraints.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"IGNORE RESTRICTED PROCESS CONSTRAINTS",&style);
		}
	}
	return(style);
}
char UsersWorkstationValidated()
{
	char style=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Workstation has been validated.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"WORKSTATION VALIDATED",&style);
		}
	}
	return(style);
}
char *UsersWorkstationLogin()
{
	char *style=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Workstation Login.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"WORKSTATION LOGIN",&style);
		}
	}
	return(style);
}
char *UsersWorkstationPassword()
{
	char *style=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Workstation Password.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"WORKSTATION PASSWORD",&style);
		}
	}
	return(style);
}
char *UsersWorkstationDocuments()
{
	char *style=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Workstation Documents.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETSTRING(SEC_USERS_FILENO,"WORKSTATION DOCUMENTS",&style);
		}
	}
	return(style);
}
char *Users_LocationID()
{
	char *phone=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's Location Identification.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(isEMPTY(UsersLocationID))
	{
		if(SEC_USERS_FILENO!=(-1))
		{
			FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
			if(!EQLNRD(SEC_USERS_FILENO,1))
			{
				FINDFLDGETSTRING(SEC_USERS_FILENO,"LOCATION IDENTIFICATION",&phone);
			}
			UsersLocationID=stralloc(phone);
		}
	} else {
		phone=stralloc(UsersLocationID);
	}
	return(phone);
}
char UsersDisplayPurgeFunctions()
{
	char style=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagmix)
	{
		prterr("DIAG Reading/Returning User's visibilty of Purge Functions.");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef _SKIP_RDA_NOGUI_
	if(RDA_NOGUI) return(FALSE);
#endif /* _SKIP_RDA_NOGUI_ */
	if(SEC_USERS_FILENO==(-1)) SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	if(DEPART_FILENO==(-1)) DEPART_FILENO=OPNNRD("SECURITY","DEPARTMENT");
	if(SEC_USERS_FILENO!=(-1))
	{
		FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
		if(!EQLNRD(SEC_USERS_FILENO,1))
		{
			FINDFLDGETCHAR(SEC_USERS_FILENO,"DISPLAY PURGE FUNCTIONS",&style);
		}
	}
	return(style);
}
