/* redolog.c - Redo Transaction Log */
#ifndef WIN32
#define __NAM__ "redolog.lnx"
#endif
#ifdef WIN32
#define __NAM__ "redolog.exe"
#endif
#include <app.hpp>
#include <fcntl.h>
#ifdef UNIXWARE7
#include <cerrno>
#endif
#ifdef UNIXWARE21
#include <cerrno>
#endif
#include <mix.hpp>
#include <trans.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *TRN_MOD=NULL,*TRN_USERID=NULL;
int TRN_LOG=(-1);
short datlog=(-1),dattrn=(-1);
char DEVL=FALSE;

void quitonerr()
{
	if(datlog!=(-1)) CLSNRD(datlog);
	if(dattrn!=(-1)) CLSNRD(dattrn);
	ShutdownSubsystems();
}
short SetDATTRNValue()
{
}

short CanRedoLog()
{
	char delflag=FALSE,*t_mod=NULL,*t_userid=NULL;
	char *chg_mod=NULL,*chg_fi=NULL,*chg_fe=NULL,*kn=NULL,*kv=NULL;
	char tempx[128],tempy[128],c=0,*pv=NULL,npv=FALSE;
	char Has_Transaction=FALSE;
	int t_log=(-1),i=0;
	short fileno=(-1),ef=0,retval=(-1),x=0,s=0;
	double d=0.0;
	NRDfield *field=NULL;

	FINDFLDGETCHAR(datlog,"HAS TRANSACTIONS",&Has_Transaction);
	if(DEVL==TRUE && Has_Transaction==TRUE) return(TRUE);
	else if(DEVL==TRUE) return(-1);
	ZERNRD(dattrn);
	FINDFLDSETSTRING(dattrn,"MODULE",TRN_MOD);
	FINDFLDSETSTRING(dattrn,"USER IDENTIFICATION",TRN_USERID);
	FINDFLDSETINT(dattrn,"LOG NUMBER",TRN_LOG);
	ef=NXTNRD(dattrn,1);
	while(!ef)
	{
		FINDFLDGETSTRING(dattrn,"MODULE",&t_mod);
		FINDFLDGETSTRING(dattrn,"USER IDENTIFICATION",&t_userid);
		FINDFLDGETINT(dattrn,"LOG NUMBER",&t_log);
		if(RDAstrcmp(TRN_MOD,t_mod) || RDAstrcmp(TRN_USERID,t_userid) || (TRN_LOG!=t_log)) break;
		FINDFLDGETCHAR(dattrn,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(dattrn,"CHANGED MODULE",&chg_mod);
			FINDFLDGETSTRING(dattrn,"CHANGED FILE",&chg_fi);
			FINDFLDGETSTRING(dattrn,"CHANGED FIELD",&chg_fe);
			FINDFLDGETCHAR(dattrn,"NO PREVIOUS VALUE",&npv);
			fileno=OPNNRDsec(chg_mod,chg_fi,TRUE,TRUE,FALSE);
			if(fileno==(-1))
			{
				prterr("ERROR: Unable to Open File [%s] [%s]",(chg_mod!=NULL ? chg_mod:""),(chg_fi!=NULL ? chg_fi:""));
				retval=FALSE;
				break;
			} else {
				ZERNRD(fileno);
				for(x=0;x<9;++x)
				{
					memset(tempx,0,128);
					memset(tempy,0,128);
					sprintf(tempx,"KEY FIELD %d NAME",(x+1));
					sprintf(tempy,"KEY FIELD %d VALUE",(x+1));
					FINDFLDGETSTRING(dattrn,tempx,&kn);
					if(isEMPTY(kn)) break;
					FINDFLDGETSTRING(dattrn,tempy,&kv);
					field=FLDNRD(fileno,kn);
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
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
								FINDFLDSETSTRING(fileno,kn,kv);
								break;
							case CHARACTERS:
								c=kv[0];
								FINDFLDSETCHAR(fileno,kn,c);
								break;
							case BOOLNS:
								if(!RDAstrcmp(kv,"TRUE")) c=TRUE;
								else c==FALSE;
								FINDFLDSETCHAR(fileno,kn,c);
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS_NOCENTS:
								d=atof(kv);
								FINDFLDSETDOUBLE(fileno,kn,d);
								break;
							case DOLLARS:
								d=atof(kv)*100;
								FINDFLDSETDOUBLE(fileno,kn,d);
								break;
							case SHORTV:
							case SSHORTV:
								s=atoi(kv);
								FINDFLDSETSHORT(fileno,kn,s);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								i=atoi(kv);
								FINDFLDSETSHORT(fileno,kn,i);
								break;
							default:
								prterr("Error Key Field Type [%d] is invalid for Field [%s].",field->type,field->name);
								retval=FALSE;
								break;
						}
					}	
				}
				if(EQLNRD(fileno,1)) 
				{
					KEYNRD(fileno,1);
					retval=FALSE;
				} else if(npv==TRUE)
				{
					retval=TRUE;
				} else {
					field=FLDNRD(fileno,chg_fe);
					if(field!=NULL)
					{
						FINDFLDGETSTRING(dattrn,"NEW VALUE",&pv);
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
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
								if(RDAstrcmp(field->data.string_value,pv)) 
								{
									retval=FALSE;
								} else retval=TRUE;
								break;
							case CHARACTERS:
								if(*field->data.string_value!=pv[0]) 
								{
									retval=FALSE;
								} else retval=TRUE;
								break;
							case BOOLNS:
								if(!RDAstrcmp(pv,"TRUE") && *field->data.string_value!=TRUE)
								{
									retval=FALSE;
								} else if(!RDAstrcmp(pv,"FALSE") && *field->data.string_value!=FALSE)
								{
									retval=FALSE;
								} else retval=TRUE;
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS_NOCENTS:
								d=atof(pv);
								if(*field->data.float_value!=d)
								{
									retval=FALSE;
								} else retval=TRUE;
								break;
							case DOLLARS:
								d=atof(pv)*100;
								if(*field->data.float_value!=d)
								{
									retval=FALSE;
								} else retval=TRUE;
								break;
							case SHORTV:
							case SSHORTV:
								s=atoi(pv);
								if(*field->data.short_value!=s)
								{
									retval=FALSE;
								} else retval=TRUE;
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								i=atoi(pv);
								if(*field->data.integer_value!=i)
								{
									retval=FALSE;
								} else retval=TRUE;
								break;
							default:
								prterr("Error Key Field Type [%d] is invalid for Field [%s].",field->type,field->name);
								retval=FALSE;
								break;
						}
					} else {
						prterr("ERROR:  Changed Field [%s] not found in File [%s][%s].",(chg_fe!=NULL ? chg_fe:""),MODULENAME(fileno),FILENAME(fileno));
						retval=FALSE;
						break;
					}
				}
				CLSNRD(fileno);
				fileno=(-1);
			}
		}
		if(retval==FALSE) break;
		ef=NXTNRD(dattrn,1);
	}
	if(pv!=NULL) Rfree(pv);
	if(kv!=NULL) Rfree(kv);
	if(kn!=NULL) Rfree(kn);
	if(chg_mod!=NULL) Rfree(chg_mod);
	if(chg_fi!=NULL) Rfree(chg_fi);
	if(chg_fe!=NULL) Rfree(chg_fe);
	if(t_mod!=NULL) Rfree(t_mod);
	if(t_userid!=NULL) Rfree(t_userid);
	return(retval);
}
void RedoLog()
{
	char delflag=FALSE,*t_mod=NULL,*t_userid=NULL;
	char *chg_mod=NULL,*chg_fi=NULL,*chg_fe=NULL,*kn=NULL,*kv=NULL;
	char tempx[128],tempy[128],c=0,*pv=NULL,npv=FALSE,*last_chg_mod=NULL,*last_chg_fi=NULL;
	char kv1[9][121],kv2[9][121],last_npv=FALSE;
	int t_log=(-1),i=0;
	short fileno=(-1),ef=0,retval=(-1),x=0,s=0,y=0;
	double d=0.0;
	NRDfield *field=NULL;
	RDATData *prev=NULL,*last=NULL;

	memset(&kv1,0,sizeof(kv1));
	memset(&kv2,0,sizeof(kv2));
	ZERNRD(dattrn);
	FINDFLDSETSTRING(dattrn,"MODULE",TRN_MOD);
	FINDFLDSETSTRING(dattrn,"USER IDENTIFICATION",TRN_USERID);
	FINDFLDSETINT(dattrn,"LOG NUMBER",TRN_LOG);
	ef=NXTNRD(dattrn,1);
	while(!ef)
	{
		FINDFLDGETSTRING(dattrn,"MODULE",&t_mod);
		FINDFLDGETSTRING(dattrn,"USER IDENTIFICATION",&t_userid);
		FINDFLDGETINT(dattrn,"LOG NUMBER",&t_log);
		if(RDAstrcmp(TRN_MOD,t_mod) || RDAstrcmp(TRN_USERID,t_userid) || (TRN_LOG!=t_log)) break;
		FINDFLDGETCHAR(dattrn,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(dattrn,"CHANGED MODULE",&chg_mod);
			FINDFLDGETSTRING(dattrn,"CHANGED FILE",&chg_fi);
			FINDFLDGETSTRING(dattrn,"CHANGED FIELD",&chg_fe);
			FINDFLDGETCHAR(dattrn,"NO PREVIOUS VALUE",&npv);
			if(RDAstrcmp(chg_mod,last_chg_mod) || RDAstrcmp(chg_fi,last_chg_fi))
			{
				if(fileno!=(-1))
				{
					if(last_npv==(-1))
					{
						WRTTRANS(fileno,0,NULL,prev);
					}
					CLSNRD(fileno);
					fileno=(-1);
				}
				if(last_chg_mod!=NULL) Rfree(last_chg_mod);
				if(last_chg_fi!=NULL) Rfree(last_chg_fi);
				last_chg_mod=stralloc(chg_mod);
				last_chg_fi=stralloc(chg_fi);
				last_npv=(-1);
				fileno=OPNNRDsec(chg_mod,chg_fi,TRUE,TRUE,FALSE);
				memset(&kv1,0,sizeof(kv1));
				memset(&kv2,0,sizeof(kv2));
				ZERNRD(fileno);
				for(x=0;x<9;++x)
				{
					memset(tempx,0,128);
					memset(tempy,0,128);
					sprintf(tempx,"KEY FIELD %d NAME",(x+1));
					sprintf(tempy,"KEY FIELD %d VALUE",(x+1));
					FINDFLDGETSTRING(dattrn,tempx,&kn);
					if(isEMPTY(kn)) break;
					FINDFLDGETSTRING(dattrn,tempy,&kv);
					field=FLDNRD(fileno,kn);
					sprintf(&kv1[x][0],"%s",(kv!=NULL ? kv:""));
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
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
								FINDFLDSETSTRING(fileno,kn,kv);
								break;
							case CHARACTERS:
								c=kv[0];
								FINDFLDSETCHAR(fileno,kn,c);
								break;
							case BOOLNS:
								if(!RDAstrcmp(kv,"TRUE")) c=TRUE;
								else c==FALSE;
								FINDFLDSETCHAR(fileno,kn,c);
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS_NOCENTS:
								d=atof(kv);
								FINDFLDSETDOUBLE(fileno,kn,d);
								break;
							case DOLLARS:
								d=atof(kv)*100;
								FINDFLDSETDOUBLE(fileno,kn,d);
								break;
							case SHORTV:
							case SSHORTV:
								s=atoi(kv);
								FINDFLDSETSHORT(fileno,kn,s);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								i=atoi(kv);
								FINDFLDSETSHORT(fileno,kn,i);
								break;
							default:
								prterr("Error Key Field Type [%d] is invalid for Field [%s].",field->type,field->name);
								break;
						}
					}	
				}
				if(EQLNRD(fileno,1)) 
				{
					KEYNRD(fileno,1);
				} else if(npv==TRUE && last_npv==(-1))
				{
					prev=RDATDataNEW(fileno);
					FINDFLDSETCHAR(fileno,"DELETEFLAG",TRUE);
					WRTTRANS(fileno,0,NULL,prev);
					last_npv=npv;
				} else if(npv==FALSE)
				{
					prev=RDATDataNEW(fileno);
					field=FLDNRD(fileno,chg_fe);
					if(field!=NULL)
					{
						FINDFLDGETSTRING(dattrn,"PREVIOUS VALUE",&pv);
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
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
								FINDFLDSETSTRING(fileno,chg_fe,pv);
								break;
							case CHARACTERS:
								FINDFLDSETCHAR(fileno,chg_fe,pv[0]);
								break;
							case BOOLNS:
								if(!RDAstrcmp(pv,"TRUE"))
								{
									FINDFLDSETCHAR(fileno,chg_fe,TRUE);
								} else FINDFLDSETCHAR(fileno,chg_fe,FALSE);
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS_NOCENTS:
								d=atof(pv);
								FINDFLDSETDOUBLE(fileno,chg_fe,d);
								break;
							case DOLLARS:
								d=atof(pv)*100;
								FINDFLDSETDOUBLE(fileno,chg_fe,d);
								break;
							case SHORTV:
							case SSHORTV:
								s=atoi(pv);
								FINDFLDSETSHORT(fileno,chg_fe,s);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								i=atoi(pv);
								FINDFLDSETINT(fileno,chg_fe,i);
								break;
							default:
								prterr("Error Key Field Type [%d] is invalid for Field [%s].",field->type,field->name);
								break;
						}
					} else {
						prterr("ERROR:  Changed Field [%s] not found in File [%s][%s].",(chg_fe!=NULL ? chg_fe:""),MODULENAME(fileno),FILENAME(fileno));
						break;
					}
				}
			} else {
				for(x=0;x<9;++x)
				{
					memset(tempx,0,128);
					memset(tempy,0,128);
					sprintf(tempx,"KEY FIELD %d NAME",(x+1));
					sprintf(tempy,"KEY FIELD %d VALUE",(x+1));
					FINDFLDGETSTRING(dattrn,tempx,&kn);
					if(isEMPTY(kn)) break;
					FINDFLDGETSTRING(dattrn,tempy,&kv);
					if(isEMPTY(kn)) break;
					sprintf(&kv2[x][0],"%s",(kv!=NULL ? kv:""));
				}
				for(y=0;y<x;++y)
				{
					if(RDAstrcmp(kv1[y],kv2[y])) break;
				}
				if(y<x)
				{
					WRTTRANS(fileno,0,NULL,prev);
					if(prev!=NULL) Rfree(prev);
					memset(&kv1,0,sizeof(kv1));
					memset(&kv2,0,sizeof(kv2));
					last_npv=(-1);
					ZERNRD(fileno);
					for(x=0;x<9;++x)
					{
						memset(tempx,0,128);
						memset(tempy,0,128);
						sprintf(tempx,"KEY FIELD %d NAME",(x+1));
						sprintf(tempy,"KEY FIELD %d VALUE",(x+1));
						FINDFLDGETSTRING(dattrn,tempx,&kn);
						if(isEMPTY(kn)) break;
						FINDFLDGETSTRING(dattrn,tempy,&kv);
						field=FLDNRD(fileno,kn);
						sprintf(&kv1[x][0],"%s",(kv!=NULL ? kv:""));
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									FINDFLDSETSTRING(fileno,kn,kv);
									break;
								case CHARACTERS:
									c=kv[0];
									FINDFLDSETCHAR(fileno,kn,c);
									break;
								case BOOLNS:
									if(!RDAstrcmp(kv,"TRUE")) c=TRUE;
									else c==FALSE;
									FINDFLDSETCHAR(fileno,kn,c);
									break;
								case DECIMALV:
								case SDECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS_NOCENTS:
									d=atof(kv);
									FINDFLDSETDOUBLE(fileno,kn,d);
									break;
								case DOLLARS:
									d=atof(kv)*100;
									FINDFLDSETDOUBLE(fileno,kn,d);
									break;
								case SHORTV:
								case SSHORTV:
									s=atoi(kv);
									FINDFLDSETSHORT(fileno,kn,s);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									i=atoi(kv);
									FINDFLDSETSHORT(fileno,kn,i);
									break;
								default:
									prterr("Error Key Field Type [%d] is invalid for Field [%s].",field->type,field->name);
									break;
							}
						}	
					}
					if(EQLNRD(fileno,1)) 
					{
						KEYNRD(fileno,1);
					} else if(npv==TRUE && last_npv==(-1))
					{
						prev=RDATDataNEW(fileno);
						FINDFLDSETCHAR(fileno,"DELETEFLAG",TRUE);
						WRTTRANS(fileno,0,NULL,prev);
						last_npv=npv;
					} else if(npv==FALSE)
					{
						prev=RDATDataNEW(fileno);
						field=FLDNRD(fileno,chg_fe);
						if(field!=NULL)
						{
							FINDFLDGETSTRING(dattrn,"PREVIOUS VALUE",&pv);
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
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									FINDFLDSETSTRING(fileno,chg_fe,pv);
									break;
								case CHARACTERS:
									FINDFLDSETCHAR(fileno,chg_fe,pv[0]);
									break;
								case BOOLNS:
									if(!RDAstrcmp(pv,"TRUE"))
									{
										FINDFLDSETCHAR(fileno,chg_fe,TRUE);
									} else FINDFLDSETCHAR(fileno,chg_fe,FALSE);
									break;
								case DECIMALV:
								case SDECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS_NOCENTS:
									d=atof(pv);
									FINDFLDSETDOUBLE(fileno,chg_fe,d);
									break;
								case DOLLARS:
									d=atof(pv)*100;
									FINDFLDSETDOUBLE(fileno,chg_fe,d);
									break;
								case SHORTV:
								case SSHORTV:
									s=atoi(pv);
									FINDFLDSETSHORT(fileno,chg_fe,s);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									i=atoi(pv);
									FINDFLDSETINT(fileno,chg_fe,i);
									break;
								default:
									prterr("Error Key Field Type [%d] is invalid for Field [%s].",field->type,field->name);
									break;
							}
						} else {
							prterr("ERROR:  Changed Field [%s] not found in File [%s][%s].",(chg_fe!=NULL ? chg_fe:""),MODULENAME(fileno),FILENAME(fileno));
							break;
						}
					}
				} else if(npv==FALSE)
				{
					memset(&kv2,0,sizeof(kv2));
					field=FLDNRD(fileno,chg_fe);
					if(field!=NULL)
					{
						FINDFLDGETSTRING(dattrn,"PREVIOUS VALUE",&pv);
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
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case CUSTOMTYPE:
								FINDFLDSETSTRING(fileno,chg_fe,pv);
								break;
							case CHARACTERS:
								FINDFLDSETCHAR(fileno,chg_fe,pv[0]);
								break;
							case BOOLNS:
								if(!RDAstrcmp(pv,"TRUE"))
								{
									FINDFLDSETCHAR(fileno,chg_fe,TRUE);
								} else FINDFLDSETCHAR(fileno,chg_fe,FALSE);
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS_NOCENTS:
								d=atof(pv);
								FINDFLDSETDOUBLE(fileno,chg_fe,d);
								break;
							case DOLLARS:
								d=atof(pv)*100;
								FINDFLDSETDOUBLE(fileno,chg_fe,d);
								break;
							case SHORTV:
							case SSHORTV:
								s=atoi(pv);
								FINDFLDSETSHORT(fileno,chg_fe,s);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								i=atoi(pv);
								FINDFLDSETINT(fileno,chg_fe,i);
								break;
							default:
								prterr("Error Key Field Type [%d] is invalid for Field [%s].",field->type,field->name);
								break;
						}
					} else {
						prterr("ERROR:  Changed Field [%s] not found in File [%s][%s].",(chg_fe!=NULL ? chg_fe:""),MODULENAME(fileno),FILENAME(fileno));
						break;
					}
				}
			}
		}
		ef=NXTNRD(dattrn,1);
	}
	if(fileno!=(-1)) CLSNRD(fileno);
	if(prev!=NULL) Rfree(prev);
	if(pv!=NULL) Rfree(pv);
	if(kv!=NULL) Rfree(kv);
	if(kn!=NULL) Rfree(kn);
	if(chg_mod!=NULL) Rfree(chg_mod);
	if(chg_fi!=NULL) Rfree(chg_fi);
	if(chg_fe!=NULL) Rfree(chg_fe);
	if(t_mod!=NULL) Rfree(t_mod);
	if(t_userid!=NULL) Rfree(t_userid);
	ZERNRD(datlog);
	FINDFLDSETSTRING(datlog,"MODULE",TRN_MOD);
	FINDFLDSETSTRING(datlog,"USER IDENTIFICATION",TRN_USERID);
	FINDFLDSETINT(datlog,"LOG NUMBER",TRN_LOG);
	if(!EQLNRD(datlog,1))
	{
		prev=RDATDataNEW(datlog);
		FINDFLDSETCHAR(datlog,"UNDONE",FALSE);
		FINDFLDSETSTRING(datlog,"UNDONE BY","");
		FINDFLDSETSTRING(datlog,"UNDONE DATE","");
		FINDFLDSETSTRING(datlog,"UNDONE TIME","");
	}
	if(prev!=NULL) FreeRDATData(prev);
}

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	short cu=0;
	RDArsrc *mainrsrc=NULL;
	char xtemp[1024],*temp=NULL,*temp1=NULL;

	if(argc<4) 
	{
		ShutdownSubsystems();
		return;
	}
	TRN_MOD=stralloc(argv[1]);
	TRN_USERID=stralloc(argv[2]);
	TRN_LOG=atoi(argv[3]);
	if(InitializeSubsystems(argc,argv,"DATABASE","UNDO TRANSACTION LOG")) 
	{
		ShutdownSubsystems();
		return;
	}
	temp=RDA_GetEnv("DEV_LICENSE");
	if(temp!=NULL)
	{
		temp1=strtok(temp," ");
		if(CheckDevLicense(temp1)) 
		{
			DEVL=TRUE;
		}
	}
	if((datlog=APPOPNNRD("DATABASE","DATLOG",TRUE,TRUE))==(-1)) 
	{
		ShutdownSubsystems();
		return;
	}
	if((dattrn=APPOPNNRD("DATABASE","DATTRN",TRUE,TRUE))==(-1)) 
	{
		ShutdownSubsystems();
		return;
	}
	ZERNRD(datlog);
	FINDFLDSETSTRING(datlog,"MODULE",TRN_MOD);
	FINDFLDSETSTRING(datlog,"USER IDENTIFICATION",TRN_USERID);
	FINDFLDSETINT(datlog,"LOG NUMBER",TRN_LOG);
	if(EQLNRD(datlog,1))
	{
		memset(xtemp,0,1024);
		sprintf(xtemp,"The Log [%s] for [%s] number [%d] was not found.  Check these keys for accuracy.",(TRN_MOD!=NULL ? TRN_MOD:""),(TRN_USERID!=NULL ? TRN_USERID:""),TRN_LOG);
		ERRORDIALOG("Database Log (DATLOG) Not Found",xtemp,quitonerr,TRUE);
	} else if((cu=CanRedoLog())==1)
	{
		RedoLog();
		if(datlog!=(-1)) CLSNRD(datlog);
		if(dattrn!=(-1)) CLSNRD(dattrn);
		ShutdownSubsystems();
	} else if(cu==0)
	{
		memset(xtemp,0,1024);
		sprintf(xtemp,"The Log [%s] for [%s] number [%d] was found, but new values didn't match current database values, so there must exist newer logs that must be redone in reverse chronological order.",(TRN_MOD!=NULL ? TRN_MOD:""),(TRN_USERID!=NULL ? TRN_USERID:""),TRN_LOG);
		ERRORDIALOG("Unable to Redo Database Log (DATLOG)",xtemp,quitonerr,TRUE);
	} else {
		ShutdownSubsystems();
	}
}

