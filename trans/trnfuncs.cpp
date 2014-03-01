#include <transp.hpp>
#include <nrd.hpp>
#include<xpert.hpp>
#include<quotes.hpp>

short DATLOG_FILENO=(-1);
short DATTRN_FILENO=(-1);
char *TRN_MODULE=NULL;
char *TRN_PROCESS=NULL;
int TRN_LOGNUMBER=(-1);

char dotrans=FALSE;

void xINITTRANS(int line,char *file)
{
	char *test=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG INITTRANS Initializing Transaction Processing at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	TRN_MODULE=NULL;
	TRN_LOGNUMBER=(-1);
	TRN_PROCESS=NULL;
	DATLOG_FILENO=(-1);
	DATTRN_FILENO=(-1);
	if(XPERT_SETUP->TRANSACTIONS==TRUE) dotrans=TRUE;
		else dotrans=FALSE;
	if(dotrans==TRUE)
	{
		test=RDA_GetEnv("NOTRANSACTIONS");
		if(test!=NULL) dotrans=FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		if(dotrans==TRUE) { prterr("DIAG INITTRANS Transaction Processing Enabled at line [%d] program [%s].",line,file); }
			else { prterr("DIAG INITTRANS Transaction Processing Disabled at line [%d] program [%s].",line,file); }
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(dotrans)
	{
		DATLOG_FILENO=OPNNRD("DATABASE","DATLOG");
		if(DATLOG_FILENO==(-1)) 
		{
			prterr("ERROR:  Cannot record transactions because DATLOG doesn't exist.");
			dotrans=FALSE;
		}
	} 
	if(dotrans)
	{
		DATTRN_FILENO=OPNNRD("DATABASE","DATTRN");
		if(DATTRN_FILENO==(-1)) 
		{
			prterr("ERROR:  Cannot record transactions because DATTRN doesn't exist.");
			dotrans=FALSE;
		}
	} 
}
void xOPNLOG(char *module,char *process,int line,char *file)
{
	char *cdate=NULL,*ctime=NULL;

	if(dotrans==FALSE) return;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG OPNLOG Opening transaction Log for Module [%s] Process [%s] at line [%d] program [%s].",module,process,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */

	ZERNRD(DATLOG_FILENO);
	TRN_MODULE=stralloc(module);
	FINDFLDSETSTRING(DATLOG_FILENO,"MODULE",TRN_MODULE);
	FINDFLDSETSTRING(DATLOG_FILENO,"USER IDENTIFICATION",USERLOGIN);
	FINDFLDSETSTRING(DATLOG_FILENO,"PROCESS NAME",process);
	FINDFLDSETCHAR(DATLOG_FILENO,"DELETEFLAG",FALSE);
	FINDFLDSETCHAR(DATLOG_FILENO,"HAS TRANSACTIONS",FALSE);
	cdate=GETCURRENTDATE10();
	FINDFLDSETSTRING(DATLOG_FILENO,"START DATE",cdate);
	ctime=GETCURRENTTIME();
	FINDFLDSETSTRING(DATLOG_FILENO,"START TIME",ctime);
	WRTNRD(DATLOG_FILENO,TRUE,"LOG NUMBER");
	FINDFLDGETINT(DATLOG_FILENO,"LOG NUMBER",&TRN_LOGNUMBER);
	if(diagtrans)
	{
		prterr("DIAG Module [%s] User [%s] Log Number [%d] for Process [%s] has started [%s] [%s].",
			(TRN_MODULE!=NULL ? TRN_MODULE:""),
			(USERLOGIN!=NULL ? USERLOGIN:""),TRN_LOGNUMBER,
			cdate,ctime);
	}
	if(cdate!=NULL) Rfree(cdate);
	if(ctime!=NULL) Rfree(ctime);
}
void xCLSLOG(int line,char *file)
{
	char *cdate=NULL,*ctime=NULL;

	if(dotrans==FALSE) return;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG Close Transaction Processing at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	ZERNRD(DATLOG_FILENO);
	FINDFLDSETSTRING(DATLOG_FILENO,"MODULE",TRN_MODULE);
	FINDFLDSETSTRING(DATLOG_FILENO,"USER IDENTIFICATION",USERLOGIN);
	FINDFLDSETINT(DATLOG_FILENO,"LOG NUMBER",TRN_LOGNUMBER);
	if(!EQLNRD(DATLOG_FILENO,1))
	{
		cdate=GETCURRENTDATE10();
		FINDFLDSETSTRING(DATLOG_FILENO,"END DATE",cdate);
		ctime=GETCURRENTTIME();
		FINDFLDSETSTRING(DATLOG_FILENO,"END TIME",ctime);
		WRTNRD(DATLOG_FILENO,FALSE,"");
		if(cdate!=NULL) Rfree(cdate);
		if(ctime!=NULL) Rfree(ctime);
	}
}
void cpfldinfo(NRDfield *ffld,NRDfield *tfld)
{
	ffld->name=stralloc(tfld->name);
	ffld->type=tfld->type;
	ffld->len=tfld->len;
	ffld->dlen=0;
	switch(tfld->type)
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
			if(!isEMPTY(tfld->data.string_value))
			{
				if(!isEMPTY(ffld->data.string_value))
				{
					QUICKALLOC(ffld->data.string_value,ffld->dlen,tfld->dlen);
				} else {
					ffld->data.string_value=Rmalloc(tfld->dlen);
					ffld->dlen=tfld->dlen;
				}
				memcpy(ffld->data.string_value,tfld->data.string_value,tfld->dlen);
			} else if(ffld->dlen>0)
			{
				memset(ffld->data.string_value,0,ffld->dlen);
			} else {
				ffld->data.string_value=NULL;
				ffld->dlen=0;
			}
			break;
		case CHARACTERS:
		case BOOLNS:
			if(ffld->data.string_value==NULL) 
			{
				ffld->data.string_value=Rmalloc(1);
				ffld->dlen=1;
			}
			*ffld->data.string_value=*tfld->data.string_value;
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			if(ffld->data.float_value==NULL) 
			{
				ffld->data.float_value=Rmalloc(sizeof(double));
				ffld->dlen=sizeof(double);
			}
			*ffld->data.float_value=*tfld->data.float_value;
			break;
		case SHORTV:
		case SSHORTV:
			if(ffld->data.short_value==NULL) 
			{
				ffld->data.short_value=Rmalloc(sizeof(short));
				ffld->dlen=sizeof(short);
			}
			*ffld->data.short_value=*tfld->data.short_value;
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(ffld->data.integer_value==NULL) 
			{
				ffld->data.integer_value=Rmalloc(sizeof(int));
				ffld->dlen=sizeof(int);
			}
			*ffld->data.integer_value=*tfld->data.integer_value;
			break;
		default:
			prterr("Error Field Type [%d] is invalid for Field [%s].",tfld->type,tfld->name);
			break;
	}
}
char *getkeyinfo(short fileno,short keyno)
{
	char *string=NULL;
	int x,count=0,length=0;
	NRDfield *tmp=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	char *temp1=NULL;

	key=KEYNUM(fileno,keyno);
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		tmp=FLDNRD(fileno,part->name);
		if(tmp!=NULL)
		{
			switch(tmp->type)
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
					temp1=Rmalloc(RDAstrlen(tmp->data.string_value)+3);
					sprintf(temp1,"\"%s\"",(tmp->data.string_value!=NULL ? tmp->data.string_value:""));
					sub_quotes(&temp1);
					length=(temp1!=NULL ? RDAstrlen(temp1):0)+2;
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+length);
					} else {
						string=Rmalloc(length);
					} 
					sprintf(&string[count],"%s\t",(temp1!=NULL ? temp1:""));
					count+=(length-1);
					if(temp1!=NULL) Rfree(temp1);
					break;
				case CHARACTERS:
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+5);
					} else {
						string=Rmalloc(5);
					} 
					sprintf(&string[count],"\"%c\"\t",*tmp->data.string_value);
					count+=4;
					break;
				case BOOLNS:
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+5);
					} else {
						string=Rmalloc(5);
					} 
					sprintf(&string[count],"\"%1d\"\t",*tmp->data.string_value);
					count+=4;
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
					memset(stemp,0,101);
					sprintf(stemp,"%.0f",*tmp->data.float_value);
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+RDAstrlen(stemp)+4);
					} else {
						string=Rmalloc(RDAstrlen(stemp)+4);
					} 
					sprintf(&string[count],"\"%s\"\t",stemp);
					count+=(RDAstrlen(stemp)+3);
					break;
				case DECIMALV:
				case SDECIMALV:
					memset(stemp,0,101);
					sprintf(stemp,"%f",*tmp->data.float_value);
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+RDAstrlen(stemp)+4);
					} else {
						string=Rmalloc(RDAstrlen(stemp)+4);
					} 
					sprintf(&string[count],"\"%s\"\t",stemp);
					count+=(RDAstrlen(stemp)+3);
					break;
				case SHORTV:
				case SSHORTV:
					memset(stemp,0,101);
					sprintf(stemp,"%d",*tmp->data.short_value);
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+RDAstrlen(stemp)+4);
					} else {
						string=Rmalloc(RDAstrlen(stemp)+4);
					} 
					sprintf(&string[count],"\"%s\"\t",stemp);
					count+=(RDAstrlen(stemp)+3);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					memset(stemp,0,101);
					sprintf(stemp,"%d",*tmp->data.integer_value);
					if(string!=NULL)
					{
						string=Rrealloc(string,RDAstrlen(string)+RDAstrlen(stemp)+4);
					} else {
						string=Rmalloc(RDAstrlen(stemp)+4);
					} 
					sprintf(&string[count],"\"%s\"\t",stemp);
					count+=(RDAstrlen(stemp)+3);
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",tmp->type,tmp->name,MODULENAME(fileno),FILENAME(fileno));
					break;
			}
		} else {
			prterr("Error Key Part [%s] not found in field list for Module [%s] File [%s].",part->name,MODULENAME(fileno),FILENAME(fileno));
		}
	}
	string[count-1]=0;
	string[count]=0;
	return(string);
}
void WriteDATTRNLog(short fileno,NRDfield *from,NRDfield *to)
{
	NRDkey *key=NULL,*keys=NULL;
	NRDpart *part=NULL;
	NRDfield *tmp=NULL;
	char xtemp[121],vtemp[121];
	int x=0;

	if(dotrans==FALSE) return;
	ZERNRD(DATLOG_FILENO);
	FINDFLDSETSTRING(DATLOG_FILENO,"MODULE",TRN_MODULE);
	FINDFLDSETSTRING(DATLOG_FILENO,"USER IDENTIFICATION",USERLOGIN);
	FINDFLDSETINT(DATLOG_FILENO,"LOG NUMBER",TRN_LOGNUMBER);
	if(EQLNRD(DATLOG_FILENO,1)) KEYNRD(DATLOG_FILENO,1);
	FINDFLDSETCHAR(DATLOG_FILENO,"HAS TRANSACTIONS",TRUE);
	WRTNRD(DATLOG_FILENO,0,NULL);
	ZERNRD(DATTRN_FILENO);
	FINDFLDSETSTRING(DATTRN_FILENO,"MODULE",TRN_MODULE);
	FINDFLDSETSTRING(DATTRN_FILENO,"USER IDENTIFICATION",USERLOGIN);
	FINDFLDSETINT(DATTRN_FILENO,"LOG NUMBER",TRN_LOGNUMBER);
	FINDFLDSETSTRING(DATTRN_FILENO,"CHANGED MODULE",MODULENAME(fileno));
	FINDFLDSETSTRING(DATTRN_FILENO,"CHANGED FILE",FILENAME(fileno));
	FINDFLDSETSTRING(DATTRN_FILENO,"CHANGED FIELD",to->name);
	FINDFLDSETSHORT(DATTRN_FILENO,"CHANGED FIELD TYPE",to->type);
	FINDFLDSETINT(DATTRN_FILENO,"CHANGED FIELD LENGTH",to->len);
	keys=KEYPOINTER(fileno);
	key=keys;
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		tmp=FLDNRD(fileno,part->name);
		memset(xtemp,0,121);
		sprintf(xtemp,"KEY FIELD %d NAME",(x+1));
		FINDFLDSETSTRING(DATTRN_FILENO,xtemp,part->name);
		memset(xtemp,0,121);
		memset(vtemp,0,121);
		sprintf(xtemp,"KEY FIELD %d VALUE",(x+1));
		switch(tmp->type)
		{
			case SCROLLEDTEXT:
				prterr("ERROR:  Wasn't expecting ScrolledText as a key part.");
				break;
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
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,tmp->data.string_value);
				break;
			case CHARACTERS:
				sprintf(vtemp,"%c",*tmp->data.string_value);
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			case BOOLNS:
				sprintf(vtemp,"%s",(*tmp->data.string_value ? "TRUE":"FALSE"));
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			case DOUBLEV:
			case SDOUBLEV:
				sprintf(vtemp,"%.0f",*tmp->data.float_value);
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			case DECIMALV:
			case SDECIMALV:
				sprintf(vtemp,"%f",*tmp->data.float_value);
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			case DOLLARS:
				sprintf(vtemp,"%.02f",*tmp->data.float_value/100);
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			case DOLLARS_NOCENTS:
				sprintf(vtemp,"%.0f",*tmp->data.float_value);
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			case SHORTV:
			case SSHORTV:
				sprintf(vtemp,"%d",*tmp->data.short_value);
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				sprintf(vtemp,"%d",*tmp->data.integer_value);
				FINDFLDSETSTRING(DATTRN_FILENO,xtemp,vtemp);
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s].",to->type,to->name);
				break;
		}
	}	
	if(from!=NULL)
	{
		memset(vtemp,0,121);
		switch(from->type)
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
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",from->data.string_value);
				break;
			case CHARACTERS:
				sprintf(vtemp,"%c",*from->data.string_value);
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			case BOOLNS:
				sprintf(vtemp,"%s",(*from->data.string_value ? "TRUE":"FALSE"));
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			case DECIMALV:
			case SDECIMALV:
				sprintf(vtemp,"%.f",*from->data.float_value);
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			case DOUBLEV:
			case SDOUBLEV:
				sprintf(vtemp,"%.0f",*from->data.float_value);
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			case DOLLARS:
				sprintf(vtemp,"%.02f",*from->data.float_value/100);
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			case DOLLARS_NOCENTS:
				sprintf(vtemp,"%.0f",*from->data.float_value);
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			case SHORTV:
			case SSHORTV:
				sprintf(vtemp,"%d",*from->data.short_value);
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				sprintf(vtemp,"%d",*from->data.integer_value);
				FINDFLDSETSTRING(DATTRN_FILENO,"PREVIOUS VALUE",vtemp);
				break;
			default:
				prterr("Error From Field Type [%d] is invalid for Field [%s].",from->type,from->name);
				break;
		}
	} else {
		FINDFLDSETCHAR(DATTRN_FILENO,"NO PREVIOUS VALUE",TRUE);
	}
	memset(vtemp,0,121);
	switch(to->type)
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
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",to->data.string_value);
			break;
		case CHARACTERS:
			sprintf(vtemp,"%c",*to->data.string_value);
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		case BOOLNS:
			sprintf(vtemp,"%s",(*to->data.string_value ? "TRUE":"FALSE"));
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		case DECIMALV:
		case SDECIMALV:
			sprintf(vtemp,"%f",*to->data.float_value);
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		case DOUBLEV:
		case SDOUBLEV:
			sprintf(vtemp,"%.0f",*to->data.float_value);
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		case DOLLARS_NOCENTS:
			sprintf(vtemp,"%.0f",*to->data.float_value);
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		case DOLLARS:
			sprintf(vtemp,"%.02f",*to->data.float_value/100);
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		case SHORTV:
		case SSHORTV:
			sprintf(vtemp,"%d",*to->data.short_value);
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			sprintf(vtemp,"%d",*to->data.integer_value);
			FINDFLDSETSTRING(DATTRN_FILENO,"NEW VALUE",vtemp);
			break;
		default:
			prterr("Error To Field Type [%d] is invalid for Field [%s].",to->type,to->name);
			break;
	}
	WRTNRD(DATTRN_FILENO,TRUE,"TRANSACTION NUMBER");
}
short xWRTNRDtrans(short fileno,short trnmode,char *fieldname,
	int line,char *file)
{
	NRDfield *fielddata=NULL,*tmp=NULL,*fld=NULL,*field=NULL;
	int numflds=0,x;
	short return_val=(-1);
	char *temp1=NULL,*temp2=NULL;

	LOCNRDFILE(fileno);
	if(dotrans==TRUE)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagtrans)
		{
			prterr("DIAG WRTNRDtrans Writing Record in File [%d] [%s] transaction mode [%d] transaction field [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),trnmode,fieldname,line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(!isEMPTY(fieldname))
		{
			field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
			if(field->type!=SCROLLEDLIST && field->type!=LONGV && 
				field->type!=SLONGV && field->type!=SHORTV &&
				field->type!=SSHORTV && field->type!=DOUBLEV &&
				field->type!=SDOUBLEV) field=NULL;
		} else field=NULL;
		if(trnmode && field!=NULL)/*get next tran num automatically*/
		{
			if(DATABASE_ENGINE(fileno)==NRDMySQLAutoEngine)
			{
				return_val=WRTNRD(fileno,trnmode,fieldname);
				numflds=NUMFLDS(fileno);
				fielddata=FLDPOINTER(fileno);
				for(x=0,fld=fielddata;x<numflds;++x,++fld)
				{
					WriteDATTRNLog(fileno,NULL,fld);
				}		
/* Write all fields to DATTRN_FILENO */
			} else {
				SetTrnModeField(fileno,fieldname);
				numflds=NUMFLDS(fileno);
				fielddata=FLDPOINTER(fileno);
				for(x=0,fld=fielddata;x<numflds;++x,++fld)
				{
					WriteDATTRNLog(fileno,NULL,fld);
				}		
				return_val=WRTNRD(fileno,0,NULL);
/* Write all fields to DATTRN_FILENO */
			}
		} else {
			numflds=NUMFLDS(fileno);
			if(numflds>0)
			{
				fielddata=Rmalloc(numflds*sizeof(NRDfield));
				for(x=0,fld=fielddata;x<numflds;++x,++fld) 
				{
					tmp=FLDNUM(fileno,x);
					cpfldinfo(fld,tmp);
				}
				if(!EQLNRD(fileno,1))
				{
/* changing existing record */
					for(x=0,fld=fielddata;x<numflds;++x,++fld) 
					{
						tmp=FLDNUM(fileno,x);
						switch(tmp->type)
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
								if(RDAstrcmp(tmp->data.string_value,fld->data.string_value))
								{
/* Write Changed String value to DATTRN_FILENO */
									WriteDATTRNLog(fileno,tmp,fld);
									if(tmp->data.string_value!=NULL) Rfree(tmp->data.string_value);
									tmp->data.string_value=fld->data.string_value;
									if(!isEMPTY(tmp->data.string_value))
									{
										tmp->dlen=RDAstrlen(tmp->data.string_value)+1;
									} else {
										tmp->dlen=0;
									}
								} else {
									if(fld->data.string_value!=NULL) Rfree(fld->data.string_value);
									fld->dlen=0;
								}
								if(fld->name!=NULL) Rfree(fld->name);
								break;
							case CHARACTERS:
								if(*tmp->data.string_value!=*fld->data.string_value)
								{
/* Write Changed CHAR value to DATTRN_FILENO */
									WriteDATTRNLog(fileno,tmp,fld);
									*tmp->data.string_value=*fld->data.string_value;
								}
								Rfree(fld->data.string_value);
								if(fld->name!=NULL) Rfree(fld->name);
								break;
							case BOOLNS:
								if(*tmp->data.string_value!=*fld->data.string_value)
								{
/* Write Changed BOOLEAN value to DATTRN_FILENO */
									WriteDATTRNLog(fileno,tmp,fld);
									*tmp->data.string_value=*fld->data.string_value;
								}
								Rfree(fld->data.string_value);
								if(fld->name!=NULL) Rfree(fld->name);
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								if(*tmp->data.float_value!=*fld->data.float_value)
								{
/* Write Changed FLOAT value to DATTRN_FILENO */
									WriteDATTRNLog(fileno,tmp,fld);
									*tmp->data.float_value=*fld->data.float_value;
								}
								Rfree(fld->data.float_value);
								if(fld->name!=NULL) Rfree(fld->name);
								break;
							case SHORTV:
							case SSHORTV:
								if(*tmp->data.short_value!=*fld->data.short_value)
								{
/* Write Changed SHORT value to DATTRN_FILENO */
									WriteDATTRNLog(fileno,tmp,fld);
									*tmp->data.short_value=*fld->data.short_value;
								}
								Rfree(fld->data.short_value);
								if(fld->name!=NULL) Rfree(fld->name);
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								if(*tmp->data.integer_value!=*fld->data.integer_value)
								{
/* Write Changed INT value to DATTRN_FILENO */
									WriteDATTRNLog(fileno,tmp,fld);
									*tmp->data.integer_value=*fld->data.integer_value;
								}
								Rfree(fld->data.integer_value);
								if(fld->name!=NULL) Rfree(fld->name);
								break;
							default:
								prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",tmp->type,tmp->name,MODULENAME(fileno),FILENAME(fileno),line,file);
								break;
						}
					}
					if(fielddata!=NULL) Rfree(fielddata);
					fielddata=NULL;
				} else {
					if(fielddata!=NULL)
					{
						for(x=0,fld=fielddata;x<numflds;++x,++fld) 
						{
							WriteDATTRNLog(fileno,NULL,fld);
							FreeNRDfield(fld);
						}
						Rfree(fielddata);
					}
/* creating new record */
/* Write all fields to DATTRN_FILENO */
				}
				return_val=WRTNRD(fileno,0,NULL);
			}
		}
	} else {
		return_val=WRTNRD(fileno,trnmode,fieldname);	
	}
	UNLNRDFILE(fileno);
	return(return_val);
}
RDATData *xRDATDataNEW(short fileno,int line,char *file)
{
	RDATData *tmp=NULL;
	NRDfield *field,*field1,*fields=NULL;
	int x,numflds=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG RDATDataNEW for Fileno [%d] [%s] [%s] at line [%d] program [%s].",fileno,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDATData));
	numflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		tmp->num=numflds;
		tmp->field=Rmalloc(tmp->num*sizeof(NRDfield));
		for(x=0,field=tmp->field,field1=fields;x<tmp->num;++x,++field,
			++field1)
		{
			cpfldinfo(field,field1);
		}
	} else {
		tmp->num=0;
		tmp->field=NULL;
	}
	return(tmp);
}
RDATData *xGetNewRecordRDADTata(short fileno,short keyno,RDATData *previous,
	int line,char *file)
{
	RDATData *tmp=NULL;
	NRDfield *field,*field1,*fields=NULL,*prefield;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG GetNewRecordRDADTata for Fileno [%d] [%s] [%s] at line [%d] program [%s].",fileno,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	LOCNRDFILE(fileno);
	tmp=RDATDataNEW(fileno);
	if(EQLNRD(fileno,keyno)) 
	{
		KEYNRD(fileno,keyno);
	} else {
		fields=FLDPOINTER(fileno);
		if(fields!=NULL)
		{
			if(previous!=NULL)
			{
				if(previous->field!=NULL)
				{
					for(x=0,prefield=previous->field,field=tmp->field,
						field1=fields;x<tmp->num;++x,++prefield,++field,
						++field1)
					{
						if(!COMPARENRDfield(prefield,field))
						{
/* cpfldinfo's arguments are backwards */
							cpfldinfo(field,field1);
						} 
					}
				}
			}
		}
	}
	return(tmp);
}
void xFreeRDATData(RDATData *r,int line,char *file)
{
	int x;
	NRDfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FreeRDADTata at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		if(r->field!=NULL)
		{
			for(x=0,f=r->field;x<r->num;++x,++f)
			{
				FreeNRDfield(f);
			}
			Rfree(r->field);
		}
		Rfree(r);
		r=NULL;
	}
}
void xClearRDATData(RDATData *r,int line,char *file)
{
	int x;
	NRDfield *field;

	if(r!=NULL)
	{
		if(r->field!=NULL)
		{
			for(x=0,field=r->field;x<r->num;++x,++field)
			{
				ZeroNRDfield(field);
			}
		}
	}
}
void xGetRDATData(short fileno,RDATData *r,int line,char *file)
{
	int x,numflds;
	NRDfield *fields=NULL,*field,*field1;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG GetRDATData for Fileno [%d] [%s] [%s] at line [%d] program [%s].",fileno,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)
	{
		numflds=NUMFLDS(fileno);
		fields=FLDPOINTER(fileno);
		for(x=0,field1=fields,field=r->field;x<numflds;++x,++field1,
			++field)
		{
			NRD2NRD(field1,field);
		}
	} else {
		prterr("Error RDATData structure wasn't initialized in GetRDATData at line [%d] program [%s].",line,file);
	}
}
void xRDATData2File(short fileno,RDATData *d,int line,char *file)
{
	int x;
	NRDfield *fldx,*tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG RDATData2File at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(d!=NULL)
	{
		for(x=0,fldx=d->field;x<d->num;++x,++fldx)
		{
			tmp=FLDNRD(fileno,fldx->name);
			if(tmp!=NULL)
			{
				NRD2NRD(fldx,tmp);
			}
		}
	}
}
int getnexttran(char *dirx,char *name,int num)
{
	FILE *fp;
	int next=0,x;
	char *tmp=NULL,temp[512];
	char *filename=NULL,*t=NULL;

	filename=Rmalloc(RDAstrlen(dirx)+RDAstrlen(name)+12);
#ifndef WIN32
	sprintf(filename,"%s%s",dirx,name);
#endif
#ifdef WIN32
	sprintf(filename,"%s%s",dirx,name);
#endif
	t=filename;
	for(;*t;++t) if(strchr(" `'()",*t)) *t='_';
	if(num<1)
	{
		fp=fopen(filename,"r");
		if(fp!=NULL)
		{
#if (defined(UNIXWARE21) || defined(UNIXWARE7))
			fgets(temp,sizeof(temp)-1,fp);
			tmp=strtok(temp,"\r\n");
			if(!isEMPTY(tmp))
			{
				next=Ratoi(tmp);
			} else next=0;
#else
			while(fgets(temp,sizeof(temp)-1,fp))
			{
				tmp=strtok(temp,"\r\n");
				if(!isEMPTY(tmp))
				{
					next=Ratoi(tmp);
				} else next=0;
				break;
			}
#endif
			fclose(fp);
			if(unlink(filename)==(-1)) if(errno!=ENOENT) { prterr("Error [%s] can't remove file [%s].",errname(),filename); }
		} else if(!RDAstrcmp(name,"NXTTRN.TRN"))
		{
			for(x=0;findfile(dirx,"*.TRN",&tmp,(int)x+1);++x) 
			{
				if((RDAstrlen(tmp)-4)>0)
				{
					tmp[RDAstrlen(tmp)-4]=0;
					if(!isEMPTY(tmp))
					{
						if(next<Ratoi(tmp)) next=Ratoi(tmp);
					}
				}
			}
			if(tmp!=NULL) Rfree(tmp);
		}
		if(!RDAstrcmp(name,"NXTTRN.TRN")) ++next;
	} else {
		next=num;
	}
#ifndef WIN32
	fp=fopen(filename,"w");
#else
	fp=fopen(filename,"wbc");
#endif
	if(fp==NULL)
	{
		prterr("Error cannot create file [%s].",filename);
	} else {
		fprintf(fp,"%d\n",next);
		fclose(fp);
	}
	if(filename!=NULL) Rfree(filename);
	return(next);
}
NRDfield *xRDATDataFIELD(RDATData *previous,char *name,int line,char *file)
{
	NRDfield *tmp=NULL;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG RDATDataFIELD at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(previous!=NULL)
	{
		if(previous->field!=NULL)
		{
			for(x=0,tmp=previous->field;x<previous->num;++x,++tmp)
			{
				if(!RDAstrcmp(tmp->name,name)) return(tmp);
			}
			tmp=NULL;
		}
	}
	return(tmp);
}
short xFINDRDATDataGETCHAR(RDATData *previous,char *fieldname,char *value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETCHAR Get Character Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*(char *)value=*(char *)field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value>-127 &&
					*field->data.float_value<128)
				{
					*(char *)value=*(char *)field->data.float_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				if(*field->data.short_value>-127 &&
					*field->data.short_value<128)
				{
					*(char *)value=*(char *)field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataGETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(*field->data.integer_value>-127 &&
					*field->data.integer_value<128)
				{
					*(char *)value=*(char *)field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETCHAR Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataGETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETCHAR Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETSHORT(RDATData *previous,char *fieldname,short *value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETSHORT Get Short Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value<SHRT_MAX)
				{
/*lint -e524*/
					*value=*field->data.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(*field->data.integer_value<SHRT_MAX)
				{
/*lint -e524*/
					*value=*field->data.integer_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETSHORT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataGETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETSHORT Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETINT(RDATData *previous,char *fieldname,int *value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETINT Get Integer Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value<INT_MAX)
				{
/*lint -e524*/
					*value=*field->data.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagtrans)
					{
						prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDRDATDataGETINT at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*value=*field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETINT Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETINT Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETDOUBLE(RDATData *previous,char *fieldname,double *value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETDOUBLE Get Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error Field Type [%d] for Field [%s] requires library function FINDRDATDataGETSTRING instead of FINDRDATDataGETVALUE at line [%d] program [%s].",field->type,fieldname,line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*value=*field->data.float_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*value=*field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETDOUBLE Read Value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETDOUBLE Field [%s] not found couldn't read value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataGETSTRING(RDATData *previous,char *fieldname,char **value,int line,char *file)
{
	NRDfield *field;
	short h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG FINDRDATDataGETSTRING Getting string value of [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				if(field->data.string_value!=NULL)
				{
					h=RDAstrlen(*value);
					if(field->dlen>h || *value==NULL)
					{
						*value=Rrealloc(*value,field->dlen+1);
					}
					memcpy(*value,field->data.string_value,field->dlen+1);
				} else {
					if(*value!=NULL) Rfree(*value);
					*value=NULL;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans)
				{
					prterr("DIAG FINDRDATDataGETSTRING Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETDOUBLE.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SHORTV:
			case SSHORTV:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETSHORT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETINT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				prterr("Error the FINDRDATDataGETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataGETCHAR.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("Error FINDRDATDataGETSTRING Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETSTRING(RDATData *previous,char *fieldname,char *value,
	int line,char *file)
{
	NRDfield *field;
	unsigned h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETSTRING Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				if(!isEMPTY((char *)value))
				{
					
					h=RDAstrlen((char *)value);
					QUICKALLOC(field->data.string_value,field->dlen,h+1);
					memcpy(field->data.string_value,(char *)value,h+1);
					if(field->dlen>field->len && field->len>0)
					{
						field->data.string_value[field->len]=0;
					}
				} else if(field->dlen>0)
				{
					memset(field->data.string_value,0,field->dlen);
				} else {
					field->data.string_value=NULL;
					field->dlen=0;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSTRING Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case BOOLNS:
			case CHARACTERS:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETDOUBLE.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SHORTV:
			case SSHORTV:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETSHORT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case LONGV:
			case SLONGV:
				prterr("Error the FINDRDATDataSETSTRING at line [%d] program [%s] should be replaced with FINDRDATDataSETINT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETSTRING Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETCHAR(RDATData *previous,char *fieldname,char value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETCHAR Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error the FINDRDATDataSETCHAR at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
				}
				*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETCHAR Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETCHAR Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETSHORT(RDATData *previous,char *fieldname,short value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETSHORT Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error the FINDRDATDataSETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(value<128 && value>-127)
				{
					*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETSHORT at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETSHORT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETSHORT Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETINT(RDATData *previous,char *fieldname,int value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETINT Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error the FINDRDATDataSETINT at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
					*field->data.string_value=0;
				}
				if(value>-127 && value<128)
				{
					*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETINT at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				if(value<SHRT_MAX)
				{
					*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETINT at line [%d] program [%s] should be replaced with FINDRDATDataSETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETINT Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETINT Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDRDATDataSETDOUBLE(RDATData *previous,char *fieldname,double value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FINDRDATDataSETDOUBLE Setting value for Field [%s] at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	field=RDATDataFIELD(previous,fieldname);
	if(field!=NULL)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case ZIPCODE:
			case CUSTOMTYPE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PLAINTEXT: /* plain text field type */
				prterr("Error the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
					*field->data.string_value=0;
				}
				if(value>-127 && value<128)
				{
/*lint -e524*/
					*field->data.string_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				if(value<SHRT_MAX)
				{
/*lint -e524*/
					*field->data.short_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case LONGV:
			case SLONGV:
				if(value<INT_MAX)
				{
/*lint -e524*/
					*field->data.integer_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd)
					{
						prterr("DIAG FINDRDATDataSETDOUBLE Found Field [%s] and set value at line [%d] program [%s].",fieldname,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDRDATDataSETDOUBLE at line [%d] program [%s] should be replaced with FINDRDATDataSETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("Error FINDRDATDataSETDOUBLE Field [%s] not found couldn't set value at line [%d] program [%s].",fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
char *READFIELD(FILE *fp,short *exit)
{
	int count=0;
	char *tread=NULL,*tmp=NULL,quotes=FALSE;
	char *prevtread=NULL;

	if(!*exit)
	{
		tread=Rmalloc(2);
		prevtread=Rmalloc(2);
		memset(prevtread,0,2);
		while(TRUE)
		{
			if(!fread(tread,sizeof(char),1,fp)) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagtrans) { prterr("DIAG READFIELD reached end of file."); }
#endif /* USE_RDA_DIAGNOSTICS */
				*exit=TRUE;
				break;
			}
			if(!quotes && tread[0]=='\"' && count==0) 
			{
				quotes=TRUE;
			} else if(quotes && strchr("\"",tread[0]) && !strchr("\\",prevtread[0]))
			{
				break;
			} else if(quotes && strchr("\t",tread[0]) && strchr("\"",prevtread[0]))
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]) && count>0) 
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]))
			{
/*
			if(!quotes && tread[0]=='\"') 
			{
				quotes=TRUE;
			} else if(quotes && strchr("\"",tread[0]))
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]) && count>0) 
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]))
			{
*/
			} else {
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+2);
				} else tmp=Rmalloc(2);
				tmp[count]=tread[0];
				tmp[count+1]=0;
				++count;
			}
			memset(prevtread,tread[0],1);
			memset(tread,0,2);
		}
		if(tread!=NULL) Rfree(tread);
		if(prevtread!=NULL) Rfree(prevtread);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG READFIELD returning [%s].",(tmp!=NULL ? tmp:"NULL"));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=reset_quotes(tmp);
	return(tmp);
}
