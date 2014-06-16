#include <sec.hpp>

short numSECMOD=0;
SECmodule *SECMOD=NULL;
short DO_SECURITY_FILENO=(-1);
short ARCHWORK_FILENO=(-1);
short ARCHUSER_FILENO=(-1);

char *RDA_ReturnAccountCodeStuff(short which)
{
	NRDfield *acctcode=NULL,*atype=NULL;
	char *s=NULL;
	short accttype=0;

	if(DO_SECURITY_FILENO>(-1))
	{
		acctcode=FLDNRD(DO_SECURITY_FILENO,"ACCOUNT CODE");
		atype=FLDNRD(DO_SECURITY_FILENO,"ACCOUNT TYPE");
		if(atype!=NULL)
		{
			accttype=*atype->data.short_value;
		} else {
			accttype=acctcode->type;
			if(acctcode->type==EXPENDITURE) accttype=0;
			else if(acctcode->type==REVENUE) accttype=1;
			else if(acctcode->type==BALANCESHEET) accttype=2;
		}
		if(acctcode!=NULL)
		{
			switch(accttype)
			{
				case 0:
					if(SEC_FINEACC==(-1)) SEC_FINEACC=OPNNRD("FINMGT","FINEACC");
					if(SEC_FINEACC!=(-1))
					{
						ZERNRD(SEC_FINEACC);
						COPYFIELD(DO_SECURITY_FILENO,SEC_FINEACC,"DEFINITION TYPE");
						FINDFLDSETSTRING(SEC_FINEACC,"ACCOUNT CODE",(acctcode->data.string_value!=NULL ? acctcode->data.string_value:""));
						if(!EQLNRD(SEC_FINEACC,1))
						{
							switch(which)
							{
								default:
								case 0:
									FINDFLDGETSTRING(SEC_FINEACC,"USER IDENTIFICATION",&s);
									break;
								case 1:
									FINDFLDGETSTRING(SEC_FINEACC,"GRANT IDENTIFICATION",&s);
									break;
								case 2:
									FINDFLDGETSTRING(SEC_FINEACC,"REFERENCE TYPE",&s);
									break;
								case 3:
									s=stralloc(acctcode->data.string_value);
									break;
							}
							return(s);
						}
					}
					break;
				case 1:
					if(SEC_FINRACC==(-1)) SEC_FINRACC=OPNNRD("FINMGT","FINRACC");
					if(SEC_FINRACC!=(-1))
					{
						ZERNRD(SEC_FINRACC);
						COPYFIELD(DO_SECURITY_FILENO,SEC_FINRACC,"DEFINITION TYPE");
						FINDFLDSETSTRING(SEC_FINRACC,"ACCOUNT CODE",(acctcode->data.string_value!=NULL ? acctcode->data.string_value:""));
						if(!EQLNRD(SEC_FINRACC,1))
						{
							switch(which)
							{
								default:
								case 0:
									FINDFLDGETSTRING(SEC_FINRACC,"USER IDENTIFICATION",&s);
									break;
								case 1:
									FINDFLDGETSTRING(SEC_FINRACC,"GRANT IDENTIFICATION",&s);
									break;
								case 2:
									FINDFLDGETSTRING(SEC_FINRACC,"REFERENCE TYPE",&s);
									break;
								case 3:
									s=stralloc(acctcode->data.string_value);
									break;
							}
							return(s);
						}
					}
					break;
				case 2:
					if(SEC_FINBACC==(-1)) SEC_FINBACC=OPNNRD("FINMGT","FINBACC");
					if(SEC_FINBACC!=(-1))
					{
						ZERNRD(SEC_FINBACC);
						COPYFIELD(DO_SECURITY_FILENO,SEC_FINBACC,"DEFINITION TYPE");
						FINDFLDSETSTRING(SEC_FINBACC,"ACCOUNT CODE",(acctcode->data.string_value!=NULL ? acctcode->data.string_value:""));
						if(!EQLNRD(SEC_FINBACC,1))
						{
							switch(which)
							{
								default:
								case 0:
									FINDFLDGETSTRING(SEC_FINBACC,"USER IDENTIFICATION",&s);
									break;
								case 1:
									break;
								case 2:
									FINDFLDGETSTRING(SEC_FINBACC,"REFERENCE TYPE",&s);
									break;
								case 3:
									s=stralloc(acctcode->data.string_value);
									break;
							}
							return(s);
						}
					}
					break;
				default:
					break;
			}
		}
	}
	return(NULL);	
}
SECmodule *GetMODSEC(char *module)
{
	SECmodule *tmp=NULL;
	char deleteflag=FALSE;

	if(MODSEC_FILENO==(-1)) return(NULL);
	if(FINDFLDSETSTRING(MODSEC_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN)) 
		return(NULL);
	if(FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME",module)) return NULL;
	if(!EQLNRD(MODSEC_FILENO,1))
	{
		FINDFLDGETCHAR(MODSEC_FILENO,"DELETEFLAG",&deleteflag);
		if(deleteflag) return(NULL);
		if(SECMOD!=NULL)
		{
			SECMOD=Rrealloc(SECMOD,(numSECMOD+1)*sizeof(SECmodule));
		} else {
			SECMOD=Rmalloc(sizeof(SECmodule));
			numSECMOD=0;
		}
		tmp=SECMOD+numSECMOD;
		tmp->module=stralloc(module);
		tmp->allfiles=FALSE;
		tmp->allprocs=FALSE;
		tmp->can_delete=FALSE;
		tmp->write=FALSE;
		tmp->print=FALSE;
		tmp->read=FALSE;
		FINDFLDGETCHAR(MODSEC_FILENO,"EXECUTE FUNCTIONS",&tmp->execfunc);
		FINDFLDGETCHAR(MODSEC_FILENO,"ALL PROCESSES",&tmp->allprocs);
		FINDFLDGETCHAR(MODSEC_FILENO,"ALL FILES",&tmp->allfiles);
		FINDFLDGETCHAR(MODSEC_FILENO,"READ DATA",&tmp->read);
		FINDFLDGETCHAR(MODSEC_FILENO,"WRITE DATA",&tmp->write);
		FINDFLDGETCHAR(MODSEC_FILENO,"PRINT DATA",&tmp->print);
		FINDFLDGETCHAR(MODSEC_FILENO,"DELETE DATA",&tmp->can_delete);
		tmp->num=0;
		tmp->files=NULL;
		++numSECMOD;
		return(tmp);
	}
	return(NULL);
}
SECmodule *FINDMODSEC(char *module)
{
	int x;
	SECmodule *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG FINDMODSEC Get Security Records for Module [%s].",(module!=NULL ? module:""));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(module==NULL) 
	{
		prterr("Error Module name cannot be NULL.");
		return(NULL);
	}
	if(SECMOD!=NULL)
	{
		for(x=0,tmp=SECMOD;x<numSECMOD;++x,++tmp)
		{
			if(!RDAstrcmp(tmp->module,module)) break;
		}
		if(x<numSECMOD) return(tmp);
	}
	return(GetMODSEC(module));
}
short xMODsecurity(char *module,short *allprocs,short *allfiles,
	char exec_funcs,char read_data,char write_data,char print_data,
	int line,char *file)
{
	SECmodule *m=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG MODsecurity Checking Module Level Security for [%s] to access [%s] module at line [%d] program [%s].",SECURITY_USERLOGIN,module,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp("root",SECURITY_USERLOGIN))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec)
		{
			prterr("DIAG MODsecurity Bypassed by Root at line [%d] program [%s].",line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		*allprocs=TRUE;*allfiles=TRUE;
		return(TRUE);
	}
	m=FINDMODSEC(module);
	if(m==NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG MODsecurity Denied Access to Module [%s] (No Module Security Defined) at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		*allprocs=FALSE;*allfiles=FALSE;
		return(FALSE);
	}
	if(((m->execfunc && exec_funcs) || (!exec_funcs) 
		|| m->allprocs)  && ((m->read && read_data) || (!read_data)) 
		&& ((m->write && write_data) || (!write_data)) 
		&& ((m->print && print_data) || (!print_data))) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG MODsecurity Granted Access to Module [%s] at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(m->allprocs==TRUE) *allprocs=TRUE; else *allprocs=FALSE;
		if(m->allfiles==TRUE) *allfiles=TRUE; else *allfiles=FALSE;
		return(TRUE);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG MODsecurity Denied Access to Module [%s] at line [%d] program [%s].",module,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(FALSE);
}
/*---------------------------------------------------------------------------
	Process Level Security Functions
---------------------------------------------------------------------------*/
short xPROCsecurity(char *module,char *process,int line,char *file)
/* test's process security */
{
	char deleteflag=FALSE,exec_func=FALSE;
	SECmodule *m;


#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xPROCsecurity Process Security Module [%s] Process [%s] at line [%d] program [%s].",module,process,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(SECURITY_USERLOGIN,"root")) return(TRUE);
	m=FINDMODSEC(module);
	if(m==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG PROCsecurity Denied Access to Module [%s] at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(FALSE);
	}
	if(!m->execfunc)
	{
		if(!RDAstrcmp(module,"UTILITIES") && 
			!RDAstrcmp(process,"MAIN MENU"))
		{
			return(TRUE);
		} else if(!RDAstrcmp(module,"UTILITIES") && 
			!RDAstrcmp(process,"TEXT BROWSER"))
		{
			return(TRUE);
		}	
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG PROCsecurity Denied Access to Module [%s] at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(FALSE);
	}
	if(m->allprocs) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG PROCsecurity Granted Access to All Processes in Module [%s] at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(TRUE);
	}
	if(PROCSEC_FILENO==(-1)) 
	{
		prterr("Error Module [SECURITY] File [PROCSEC] not available at line [%d] program [%s].",line,file);
		prterr("DIAG PROCsecurity Denied Access to Process [%s] at line [%d] program [%s].",process,line,file);
		return(FALSE);
	}
	if(FINDFLDSETSTRING(PROCSEC_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN)) 
	{
		prterr("Error Field [USER IDENTIFICATION] not found in File [PROCSEC] at line[%d] program [%s].",line,file);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG PROCsecurity Denied Access to Process [%s] at line [%d] program [%s].",process,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return FALSE;
	}
	if(FINDFLDSETSTRING(PROCSEC_FILENO,"MODULE NAME",module)) 
	{
		prterr("Error Field [MODULE NAME] not found in File [PROCSEC] at line[%d] program [%s].",line,file);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG PROCsecurity Denied Access to Process [%s] at line [%d] program [%s].",process,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	 	return FALSE;
	}
	if(FINDFLDSETSTRING(PROCSEC_FILENO,"PROCESS NAME",process)) 
	{
		prterr("Error Field [PROCESS NAME] not found in File [PROCSEC] at line[%d] program [%s].",line,file);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG PROCsecurity Denied Access to Process [%s] at line [%d] program [%s].",process,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return FALSE;
	}
	if(!EQLNRD(PROCSEC_FILENO,1))
	{
		FINDFLDGETCHAR(PROCSEC_FILENO,"DELETEFLAG",&deleteflag);
		if(deleteflag) 
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("DIAG PROCsecurity Denied Access to Process [%s] at line [%d] program [%s].",process,line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(FALSE);
		}
		if(FINDFLDGETCHAR(PROCSEC_FILENO,"EXECUTE PROCESS",&exec_func)) 
		{
			prterr("Error Field [EXECUTE PROCESS] not found in File [PROCSEC] at line[%d] program [%s].",line,file);
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("DIAG PROCsecurity Denied Access to Process [%s] at line [%d] program [%s].",process,line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(FALSE);
		}
		if(exec_func) 
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec || diagsec_field)
			{
				prterr("DIAG PROCsecurity Granted Access to Process [%s] at line [%d] program [%s].",process,line,file);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(TRUE);
		}
	} 
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG PROCsecurity Denied Access to Process [%s] at line [%d] program [%s].",process,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

	return(FALSE);
}
/*---------------------------------------------------------------------------
	Record Level Security Functions
---------------------------------------------------------------------------*/
short ARCHIVESECURITY(short fileno,short which)
{
	char *userid=NULL,delflag=FALSE;

	if(RDAstrcmp(MODULENAME(fileno),"ARCHIVE")) 
	{
		return(TRUE);
	}
	if(RDAstrcmp(FILENAME(fileno),"ARCHIVES") && RDAstrcmp(FILENAME(fileno),"ARCHWORK")) 
	{
		return(TRUE);
	}
	if(ARCHWORK_FILENO==(-1)) ARCHWORK_FILENO=OPNNRD("ARCHIVE","ARCHWORK");
	if(ARCHUSER_FILENO==(-1)) ARCHUSER_FILENO=OPNNRD("ARCHIVE","ARCHUSER");
	if(ARCHWORK_FILENO!=(-1))
	{
		ZERNRD(ARCHWORK_FILENO);
		COPYFIELD(fileno,ARCHWORK_FILENO,"WORK GROUP");
		if(EQLNRD(ARCHWORK_FILENO,1))
		{
			 return(TRUE);
		}
		FINDFLDGETSTRING(ARCHWORK_FILENO,"USER IDENTIFICATION",&userid);
		if(!RDAstrcmp(userid,USERLOGIN)) 
		{
			if(userid!=NULL) Rfree(userid);
			return(TRUE);
		} else if(ARCHUSER_FILENO!=(-1))
		{
			if(userid!=NULL) Rfree(userid);
			ZERNRD(ARCHUSER_FILENO);
			COPYFIELD(fileno,ARCHUSER_FILENO,"WORK GROUP");
			FINDFLDSETSTRING(ARCHUSER_FILENO,"USER IDENTIFICATION",USERLOGIN);
			if(EQLNRD(ARCHUSER_FILENO,1)) 
			{
				return(FALSE);
			}
			FINDFLDGETCHAR(ARCHUSER_FILENO,"DELETEFLAG",&delflag);
			if(delflag==TRUE) 
			{
				return(FALSE);
			}
			FINDFLDGETCHAR(ARCHUSER_FILENO,"READ",&delflag);
			if(delflag==FALSE) 
			{
				return(FALSE);
			}
			if(which==TRUE)
			{
				FINDFLDGETCHAR(ARCHUSER_FILENO,"WRITE",&delflag);
				if(delflag==FALSE) 
				{
					return(FALSE);
				}
			}
		}
	}
	return(TRUE);
}
short xRECORDsec(short fileno,void (*SubFunc)(...),void *args,short which,int line,char *file)
/* test's record level security */
{
	char *m,*f;
	SECmodule *smod;
	SECfile *sfile;
	short retval=TRUE;

	if(!RDAstrcmp(SECURITY_USERLOGIN,"root")) return(FALSE);
	DO_SECURITY_FILENO=fileno;
	m=MODULENAME(fileno);
	smod=FINDMODSEC(m);
	if(smod==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG RECORDsec Denied Access to Module [%s] at line [%d] program [%s].",MODULENAME(fileno),line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		DO_SECURITY_FILENO=(-1);
		return(-1);
	}
	if(!ARCHIVESECURITY(fileno,which))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG RECORDsec used ARCHIVESECURITY to Deny Access at line [%d] program [%s].",line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	if(smod->allfiles) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG RECORDsec Granted Access to All Files in Module [%s] at line [%d] program [%s].",MODULENAME(fileno),line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		DO_SECURITY_FILENO=(-1);
		return(0);
	}
	f=FILENAME(fileno);
	sfile=FINDFILESEC(smod,f);
	if(sfile==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG RECORDsec Denied Access to Module [%s] File [%s] at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		DO_SECURITY_FILENO=(-1);
		return(-1);
	}
	if(!isEMPTY(sfile->p_expression))
	{
		if(SubFunc!=NULL)
		{
			retval=PP_EVALUATEbol(sfile->p_expression,SubFunc,args);
		} else {
			retval=PP_EVALUATEbol(sfile->p_expression,subfielddata,&fileno);
		}
	}
	DO_SECURITY_FILENO=(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG RECORDsec Returning %s Access to record in Module [%s] File [%s] at line [%d] program [%s].",(!retval ? "Granted":"Denied"),MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(!retval);
}
char *xRECORDsecDesc(short fileno,void (*SubFunc)(...),void *args,short which,int line,char *file)
{
	char *m,*f,*x=NULL,*return_string=NULL;
	SECmodule *smod;
	SECfile *sfile;

	m=MODULENAME(fileno);
	smod=FINDMODSEC(m);
	if(smod==NULL) return(NULL);
	if(!ARCHIVESECURITY(fileno,which))
	{
		return("ARCHIVESECURITY denied access to work group.");
	}
	if(smod->allfiles) return(NULL);
	f=FILENAME(fileno);
	sfile=FINDFILESEC(smod,f);
	if(sfile==NULL) return(NULL);
	DO_SECURITY_FILENO=fileno;
	if(!isEMPTY(sfile->expression))
	{
		if(SubFunc!=NULL)
		{
			x=OpenRDA_translate(sfile->expression,SubFunc,args,FALSE);
			if(!isEMPTY(x))
			{
				return_string=OpenRDA_translate(sfile->expression,
					SubFunc,args,TRUE);
			}
		} else {
			x=OpenRDA_translate(sfile->expression,subfielddata,&fileno,FALSE);
			if(!isEMPTY(x))
			{
				return_string=OpenRDA_translate(sfile->expression,
					subfielddata,&fileno,TRUE);
			}
		}
	}
	DO_SECURITY_FILENO=(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG RECORDsecDesc Returning [%s] for Module [%s] File [%s] at line [%d] program [%s].",return_string,MODULENAME(fileno),FILENAME(fileno),line,file);	
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(return_string);
}
/*---------------------------------------------------------------------------
	File Level Security Functions
---------------------------------------------------------------------------*/
SECfile *GetFILESEC(SECmodule *smod,char *filename)
{
	SECfile *tmp=NULL;
	char deleteflag=FALSE;

	if(FILESEC_FILENO==(-1)) 
	{
		prterr("Error Module [SECURITY] File [FILESEC] is not available.");
		return(NULL);
	}
	if(FINDFLDSETSTRING(FILESEC_FILENO,"MODULE NAME",smod->module)) 
	{
		prterr("Error Field [MODULE NAME] not found in File [FILESEC].");
		return(NULL);
	}
	if(FINDFLDSETSTRING(FILESEC_FILENO,"FILE NAME",filename)) 
	{
		prterr("Error Field [FILE NAME] not found in File [FILESEC].");
		return(NULL);
	}
	if(FINDFLDSETSTRING(FILESEC_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN)) 
	{
		prterr("Error Field [USER IDENTIFICATION] not found in File [FILESEC].");
		return(NULL);
	}
	if(!EQLNRD(FILESEC_FILENO,1))
	{
		FINDFLDGETCHAR(FILESEC_FILENO,"DELETEFLAG",&deleteflag);
		if(deleteflag) 
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagsec)
			{
				prterr("DIAG FILESEC Record deleted Access Denied.");
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			return(NULL);
		}
		if(smod->files!=NULL)
		{
			smod->files=Rrealloc(smod->files,(smod->num+1)*sizeof(SECfile));
		} else {
			smod->files=Rmalloc(sizeof(SECfile));
			smod->num=0;
		}
		tmp=smod->files+smod->num;
		tmp->file=stralloc(filename);
		tmp->expression=NULL;
		FINDFLDGETCHAR(FILESEC_FILENO,"READ FILE",&tmp->read);
		FINDFLDGETCHAR(FILESEC_FILENO,"WRITE FILE",&tmp->write);
		FINDFLDGETCHAR(FILESEC_FILENO,"PRINT FILE",&tmp->print);
		FINDFLDGETCHAR(FILESEC_FILENO,"DELETE FILE",&tmp->can_delete);
		FINDFLDGETCHAR(FILESEC_FILENO,"FIELD SECURITY",&tmp->fieldsecurity);
		FINDFLDGETSTRING(FILESEC_FILENO,"EXPRESSION",&tmp->expression);
		if(!isEMPTY(tmp->expression))
		{
			tmp->p_expression=PP_translate(tmp->expression);
		} else tmp->p_expression=NULL;
		tmp->fields=NULL;
		tmp->num=0;
		++smod->num;
		return(tmp);
	}
	return(NULL);
}
SECfile *FINDFILESEC(SECmodule *smod,char *filename)
{
	SECfile *tmp=NULL;
	int x;

	if(smod->files!=NULL)
	{
		for(x=0,tmp=smod->files;x<smod->num;++x,++tmp)
		{
			if(!RDAstrcmp(tmp->file,filename)) break;
		}
		if(x<smod->num) return(tmp);
	}
	return(GetFILESEC(smod,filename));
}
short xFILEsecurity(char *module,char *filename,char aread,char awrite,
	char aprint,int line,char *file)
{
	SECmodule *smod=NULL;
	SECfile *sfile=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xFILEsecurity File Security Module [%s] Filename [%s] at line [%d] program [%s].",module,filename,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(SECURITY_USERLOGIN,"root")) return(TRUE);
	if(filename==NULL) return(FALSE);
	smod=FINDMODSEC(module);
	if(smod==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG FILEsecurity Denied Access to Module [%s] at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(FALSE);
	}
	if((aread && !smod->read) || (awrite && !smod->write)
		|| (aprint && !smod->print)) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG FILEsecurity Denied Access to Module [%s] at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(FALSE);
	}
	if(smod->allfiles) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG FILEsecurity Granted Access to All Files in Module [%s] at line [%d] program [%s].",module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(TRUE);
	}
	sfile=FINDFILESEC(smod,filename);
	if(sfile==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG FILEsecurity Denied Access to Module [%s] File [%s] at line [%d] program [%s].",module,filename,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(FALSE);
	}
	if(((aread && sfile->read) || !aread) && ((awrite && sfile->write)
		|| !awrite) && ((aprint && sfile->print) || !aprint)) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec || diagsec_field)
		{
			prterr("DIAG FILEsecurityu Granted Access to Module [%s] File [%s] at line [%d] program [%s].",module,filename,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(TRUE);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG FILEsecurityu Denied Access to Module [%s] File [%s] at line [%d] program [%s].",module,filename,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(FALSE);
}
/*---------------------------------------------------------------------------
	Field Level Security Functions
---------------------------------------------------------------------------*/
SECfield *GetFIELDSEC(SECmodule *smod,SECfile *sfile,char *fieldname)
{
	SECfield *tmp=NULL;
	char deleteflag=FALSE;

	if(FLDSEC_FILENO==(-1)) return(NULL);
	if(FINDFLDSETSTRING(FLDSEC_FILENO,"MODULE NAME",smod->module)) return NULL;
	if(FINDFLDSETSTRING(FLDSEC_FILENO,"FILE NAME",sfile->file)) return NULL;
	if(FINDFLDSETSTRING(FLDSEC_FILENO,"USER IDENTIFICATION",SECURITY_USERLOGIN)) return(NULL);
	if(FINDFLDSETSTRING(FLDSEC_FILENO,"FIELD NAME",fieldname)) return(NULL);
	if(!EQLNRD(FLDSEC_FILENO,1))
	{
		FINDFLDGETCHAR(FLDSEC_FILENO,"DELETEFLAG",&deleteflag);
		if(deleteflag) return(NULL);
		if(sfile->fields!=NULL)
		{
			sfile->fields=Rrealloc(sfile->fields,(sfile->num+1)*
				sizeof(SECfield));
		} else {
			sfile->fields=Rmalloc(sizeof(SECfield));
			sfile->num=0;
		}
		tmp=sfile->fields+sfile->num;
		tmp->name=stralloc(fieldname);
		FINDFLDGETCHAR(FLDSEC_FILENO,"READ FIELD",&tmp->read);
		FINDFLDGETCHAR(FLDSEC_FILENO,"WRITE FIELD",&tmp->write);
		FINDFLDGETCHAR(FLDSEC_FILENO,"PRINT FIELD",&tmp->print);
		++sfile->num;
		return(tmp);
	}
	return(NULL);
}
SECfield *FINDFIELDSEC(SECmodule *smod,SECfile *sfile,char *fieldname)
{
	SECfield *tmp=NULL;
	int x;

	if(sfile->fields!=NULL)
	{
		for(x=0,tmp=sfile->fields;x<sfile->num;++x,++tmp)
		{
			if(!RDAstrcmp(tmp->name,fieldname)) break;
		}
		if(x<sfile->num) return(tmp);
	}
	return(GetFIELDSEC(smod,sfile,fieldname));
}
short xFIELDsecurity(char *module,char *filename,char *field,char aread,
	char awrite,char aprint,int line,char *file)
{
	SECmodule *smod=NULL;
	SECfile *sfile=NULL;
	SECfield *sfield=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG xFIELDsecurity for Field [%s] File Security Module [%s] Filename [%s] at line [%d] program [%s].",field,module,filename,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!RDAstrcmp(SECURITY_USERLOGIN,"root")) return(TRUE);
	if(filename==NULL) return(FALSE);
	smod=FINDMODSEC(module);
	if(smod==NULL) return(FALSE);
	if(smod->allfiles) return(TRUE);
	sfile=FINDFILESEC(smod,filename);
	if(sfile==NULL) return(FALSE);
	if(!sfile->fieldsecurity) return(TRUE);
	sfield=FINDFIELDSEC(smod,sfile,field);
	if(sfield==NULL) return(FALSE);
	if(((aread && sfield->read) || !aread) && ((awrite && sfield->write)
		|| !awrite) && ((aprint && sfield->print) || !aprint)) return(TRUE);
	return(FALSE);
}
NRDfield *xFLDNRDsec(short fileno,char *fieldname,
	short aread,short awrite,short aprint,int line,char *file)
{
	NRDfield *field=NULL;
	char *modx=NULL,*filx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG FLDNRDsec File [%d] [%s] Fieldname [%s] Request Read [%s] Write [%s] Print [%s] at line [%d] program [%s].",
			fileno,FILENAME(fileno),fieldname,
			(aread ? "True":"False"),(awrite ? "True":"False"),
			(aprint ? "True":"False"),line,file); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	field=FLDNRD(fileno,fieldname);
	if(field!=NULL)
	{
		modx=MODULENAME(fileno);
		filx=FILENAME(fileno);
		if(FIELDsecurity(modx,filx,field->name,aread,awrite,aprint))
		{
			return(field);
		} else return(NULL);
	}
	return(field);
}
NRDfield *xFLDNUMsec(short fileno,short fieldno,short aread,short awrite,short aprint,int line,char *file)
{
	NRDfield *field=NULL;
	char *modx=NULL,*filx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec || diagsec_field)
	{
		prterr("DIAG FLDNUMsec File [%d] [%s] Field No. [%d] Request Read [%s] Write [%s] Print [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),fieldno,(aread ? "True":"False"),(awrite ? "True":"False"),(aprint ? "True":"False"),line,file); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	field=FLDNUM(fileno,fieldno);
	if(field!=NULL)
	{
		modx=MODULENAME(fileno);
		filx=FILENAME(fileno);
		if(FIELDsecurity(modx,filx,field->name,aread,awrite,aprint))
		{
			return(field);
		} else return(NULL);
	}
	return(field);
}
