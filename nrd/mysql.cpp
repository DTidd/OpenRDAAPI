/*lint -e525*/
#include <nrdp.hpp>

#if(defined(USE_MYSQL))
#ifdef USE_MARIADB
#include <mysql/mysql.h>
#else
#include <mysql.h>
#endif

char *MySQL_Username=NULL,*MySQL_Password=NULL;
char *MySQL_Server=NULL,*MySQL_Database=NULL;
unsigned int MySQL_Port=0;
#define MySelect(a,b,c,d)	xMySelect("SELECT",a,b,c,d,__LINE__,__FILE__)
#define MyDelete(a,b,c,d)	xMySelect("DELETE",a,b,c,d,__LINE__,__FILE__)
static char *xMySelect(char *,short,short,char *,char *,int,char *);
#define MySelectAll(a,b,c,d)	xMySelectAll(a,b,c,d,__LINE__,__FILE__)
static char *xMySelectAll(short,short,char *,char *,int,char *);

int xLogSQL(char *callingfunc,char *value,int line,char *file);
#define LogSQL(a,b)	xLogSQL(a,b,__LINE__,__FILE__)

int xLogSQL(char *callingfunc,char *value,int line,char *file)
{
	FILE *fp=NULL;
	
	if(value==NULL)
	{
		prterr("LogSQL:  SQL String is NULL.\n");
		return(0);
	}
	if(NULL==(fp=fopen("nrdlog.sql","a+"))) 
	{
		prterr("Cannot open SQL log file.\n");
		return(1);
	}else{
		fprintf(fp,"------BEGIN SQL STATEMENT---%s:%d---\n",callingfunc,line);
		fwrite(value,1,(RDAstrlen(value)),fp);
		fprintf(fp,"\n-------END SQL STATEMENT---%s:%d---\n\n",callingfunc,line);
		fclose(fp);
		return(0);
	}
}

static char *MySQLTableName(char *x)
{
	char *n=NULL;

	n=Rmalloc(RDAstrlen(MySQL_Database)+RDAstrlen(x)+1);
	sprintf(n,"%s%s",(MySQL_Database!=NULL ? MySQL_Database:""),(x!=NULL ? x:""));
	return(n);
}

static char *WriteSQL(NRDbuffer *NRDtmp)
{
	int x=0,y=0,len=0;
	NRDfield *f=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	char *temp=NULL,*temp1=NULL;

	temp1=MySQLTableName(NRDtmp->name);
	temp=Rmalloc(RDAstrlen(temp1)+19);
	sprintf(temp,"CREATE TABLE `%s` (\n",temp1);
	if(temp1!=NULL) Rfree(temp1);
	len=RDAstrlen(temp);
	for(x=0,f=NRDtmp->field;x<NRDtmp->nofields;++x,++f)
	{
		switch(f->type)
		{
			case SCROLLEDTEXT:
				if(f->len!=0)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%d",f->len);
					temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+13);
					sprintf(&temp[len],"\t`%s` text(%s),\n",f->name,stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(f->name)+11);
					sprintf(&temp[len],"\t`%s` text,\n",f->name);
				}
				len=RDAstrlen(temp);
				break;
			case VARIABLETEXT:
				memset(stemp,0,101);
				sprintf(stemp,"%d",f->len);
				temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+36);
				sprintf(&temp[len],"\t`%s` varchar(%s) DEFAULT '',\n",f->name,stemp);
				len=RDAstrlen(temp);
				break;
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
				memset(stemp,0,101);
				sprintf(stemp,"%d",f->len);
				if(f->len>0)
				{
					temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+38);
					sprintf(&temp[len],"\t`%s` char(%s) DEFAULT '',\n",f->name,stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+36);
					sprintf(&temp[len],"\t`%s` varchar(%s) DEFAULT '',\n",f->name,stemp);
				}
				len=RDAstrlen(temp);
				break;
			case BOOLNS:
				memset(stemp,0,101);
				sprintf(stemp,"%d",f->len);
				temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+30);
				sprintf(&temp[len],"\t`%s` tinyint(%s) DEFAULT '0',\n",f->name,stemp);
				len=RDAstrlen(temp);
				break;
			case CHARACTERS:
				sprintf(stemp,"%d",f->len);
				temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+29);
				sprintf(&temp[len],"\t`%s` char(%s) DEFAULT '',\n",f->name,stemp);
				len=RDAstrlen(temp);
				break;
			case DECIMALV:
			case SDECIMALV:
				sprintf(stemp,"%d",f->len);
				temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+28);
				sprintf(&temp[len],"\t`%s` decimal(%s) DEFAULT '0',\n",f->name,stemp);
				len=RDAstrlen(temp);
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS_NOCENTS:
			case DOLLARS:
				sprintf(stemp,"%d",f->len);
				temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+30);
				sprintf(&temp[len],"\t`%s` decimal(%s,0) DEFAULT '0',\n",f->name,stemp);
				len=RDAstrlen(temp);
				break;
			case SHORTV:
			case SSHORTV:
				sprintf(stemp,"%d",f->len);
				temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+29);
				sprintf(&temp[len],"\t`%s` smallint(%s) DEFAULT '0',\n",f->name,stemp);
				len=RDAstrlen(temp);
				break;
			case LONGV:
			case SLONGV:
				sprintf(stemp,"%d",f->len);
				
				temp=Rrealloc(temp,len+RDAstrlen(f->name)+RDAstrlen(stemp)+28);
				sprintf(&temp[len],"\t`%s` int(%s) DEFAULT '0',\n",f->name,stemp);
				len=RDAstrlen(temp);
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s].",f->type,f->name);
				break;
		}
	}
#ifdef USE_MYSQL_RECORDNUMBER
	temp=Rrealloc(temp,len+51);
	sprintf(&temp[len],"\t`MYSQL_RECORD_NUMBER` int(11) AUTO_INCREMENT,\n");
	len=RDAstrlen(temp);
#endif /* USE_MYSQL_RECORDNUMBER */
	for(y=0;y<NRDtmp->nokeys;++y)
	{
		key=NRDtmp->key+y;
		if(!y) 
		{
			temp=Rrealloc(temp,len+15);
			sprintf(&temp[len],"\tPRIMARY KEY (");
		} else {
			temp=Rrealloc(temp,len+RDAstrlen(key->name)+12);
			sprintf(&temp[len],"\tKEY `%s` ( ",key->name);
		}
		len=RDAstrlen(temp);
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if((x+1)<key->numparts)
			{
				temp=Rrealloc(temp,len+RDAstrlen(part->name)+4);
				sprintf(&temp[len],"`%s`, ",part->name);
			} else {
				temp=Rrealloc(temp,len+RDAstrlen(part->name)+3);
				sprintf(&temp[len],"`%s`",part->name);
			}
			len=RDAstrlen(temp);
		}
		if((y+1)<NRDtmp->nokeys)
		{
			temp=Rrealloc(temp,len+3);
			sprintf(&temp[len],"),\n");
		} else {
			temp=Rrealloc(temp,len+2);
			sprintf(&temp[len],")\n");
		}
		len=RDAstrlen(temp);
	}
	temp=Rrealloc(temp,len+19);
	sprintf(&temp[len],") ENGINE=MyISAM;\n");
	len=RDAstrlen(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("[%s]",(temp!=NULL ? temp:""));TRACE;
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(temp);
}
void assignMYSQL_fields(NRDbuffer *NRDtmp)
{
	NRDfield *field;
	int y=0;
	MYSQL_FIELD *sql_fld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG assignMYSQL_fields Assigning Current C-tree Record in Module [%s] File [%s] to Database Engine.",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	y=0;
	mysql_field_seek(NRDtmp->result,0);
	while((sql_fld=mysql_fetch_field(NRDtmp->result))!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{
			prterr("NRDtmp->row[%d] [%s] ",y,(NRDtmp->row[y]!=NULL ? NRDtmp->row[y]:""));TRACE;
		}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef USE_MYSQL_RECORDNUMBER
		if(!RDAstrcmp(sql_fld->name,"MYSQL_RECORD_NUMBER"))
		{
			NRDtmp->currec=atoi(NRDtmp->row[y]);
		} else {
#endif /* USE_MYSQL_RECORDNUMBER */
			field=FINDFLD(NRDtmp,sql_fld->name);
			if(field!=NULL)
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					prterr("Field [%d] name [%s] type [%d] [%s] len [%d].",y,field->name,field->type,fieldtypedesc[field->type],field->len);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT: /* variable length field type */
						if(sql_fld->length && !isEMPTY(NRDtmp->row[y]))
						{
							QUICKALLOC(field->data.string_value,field->dlen,sql_fld->length+1);
							memset(field->data.string_value,0,field->dlen);
							strcpy(field->data.string_value,NRDtmp->row[y]);
							unpad(field->data.string_value);
						} else if(field->data.string_value!=NULL)
						{
							memset(field->data.string_value,0,field->dlen);
						}
						break;
					case PLAINTEXT: /* plain text field type */
					case ZIPCODE:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(sql_fld->length && !isEMPTY(NRDtmp->row[y]))
						{
							QUICKALLOC(field->data.string_value,field->dlen,sql_fld->length+1);
							memset(field->data.string_value,0,field->dlen);
							strcpy(field->data.string_value,NRDtmp->row[y]);
							if(field->dlen>field->len) field->data.string_value[field->len]=0;
							unpad(field->data.string_value);
						} else if(field->data.string_value!=NULL)
						{
							memset(field->data.string_value,0,field->dlen);
						}
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						if(NRDtmp->row[y]!=NULL)
						{
							*field->data.float_value=atof(NRDtmp->row[y]);
						} else *field->data.float_value=0;
						break;
					case SHORTV:
					case SSHORTV:
						if(NRDtmp->row[y]!=NULL)
						{
							*field->data.short_value=atoi(NRDtmp->row[y]);
						} else *field->data.short_value=0;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(NRDtmp->row[y]!=NULL)
						{
							*field->data.integer_value=atoi(NRDtmp->row[y]);
						} else *field->data.integer_value=0;
						break;
					case BOOLNS:
						if(NRDtmp->row[y]!=NULL)
						{
							if(atoi(NRDtmp->row[y])) *field->data.string_value=TRUE;
								else *field->data.string_value=FALSE;
						} else *field->data.string_value=FALSE;
						break;
					case CHARACTERS:
						memcpy(field->data.string_value,&NRDtmp->row[y],1);
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
						break;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd)
				{
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef USE_MYSQL_RECORDNUMBER
			}
#endif /* USE_MYSQL_RECORDNUMBER */
		}
		++y;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG All Fields Assigned in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}
short LOCNRDFILEmysqlxpert(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short LOCNRDFILEmysql(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short LOCNRDmysqlxpert(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short LOCNRDmysql(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short UNLNRDFILEmysqlxpert(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;

	return(0);
}
short UNLNRDFILEmysql(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;

	return(0);
}
short UNLNRDmysqlxpert(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short UNLNRDmysql(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short DELNRDmysqlxpert(short fileno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG DELNRDmysql Deleting record with equal key  with MySQL Server D-Base Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	temp=MyDelete(fileno,1,"=","ASC");
	if(diagmysql)
	{
		LogSQL("DELNRDmysqlxpert",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
	return(rv);
}
short DELNRDmysql(short fileno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG DELNRDmysql Deleting record with equal key  with MySQL Server D-Base Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	temp=MyDelete(fileno,1,"=","ASC");
	if(diagmysql)
	{
		LogSQL("DELNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
	return(rv);
}
static char *xMySelectAll(short fileno,short keyno,char *operand,char *d,int line,char *file)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	char *temp=NULL,*temp1=NULL;
	short a=0,len=27;
	NRDfield *field=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG MySelect Fileno [%d] Keyno [%d] Operand [%s] Direction [%s] from Line [%d] Program [%s].\n",fileno,keyno,operand,d,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	key=NRDtmp->key+(keyno-1);
	temp1=MySQLTableName(NRDtmp->name);
	if(keyno!=(-1))
	{
		temp=Rmalloc(RDAstrlen(temp1)+31);
		sprintf(temp,"SELECT * FROM `%s` WHERE (",temp1);
	} else {
		temp=Rmalloc(RDAstrlen(temp1)+31);
		sprintf(temp,"SELECT * FROM `%s` ",temp1);
	}
	len=RDAstrlen(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(keyno!=(-1))
	{
	if(key->part!=NULL)
	{
		for(a=0,part=key->part;a<key->numparts;++a,++part)
		{
			field=FINDFLD(NRDtmp,part->name);
			if((a+1)<key->numparts)
			{
				temp=Rrealloc(temp,len+RDAstrlen(field->name)+4);
				sprintf(&temp[len],"`%s`,",field->name);
			} else {
				temp=Rrealloc(temp,len+RDAstrlen(field->name)+3);
				sprintf(&temp[len],"`%s`",field->name);
			}
			len=RDAstrlen(temp);
		}
	}
	temp=Rrealloc(temp,len+RDAstrlen(operand)+3);
	sprintf(&temp[len],")%s(",operand);
	len=RDAstrlen(temp);
	if(key->part!=NULL)
	{
	for(a=0,part=key->part;a<key->numparts;++a,++part)
	{
		field=FINDFLD(NRDtmp,part->name);
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
				if(!isEMPTY(field->data.string_value))
				{
					temp1=Rmalloc(RDAstrlen(field->data.string_value)*2+1);
					if(!mysql_real_escape_string(NRDtmp->db,temp1,field->data.string_value,RDAstrlen(field->data.string_value)))
					{
						prterr("mysql_real_escape_string failed %s [%s] ",(field->data.string_value!=NULL ? field->data.string_value:""),(temp1!=NULL ? temp1:""));TRACE;
					}
				} else temp1=NULL;
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(temp1)+4);
					sprintf(&temp[len],"'%s',",(temp1!=NULL ? temp1:""));
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(temp1)+3);
					sprintf(&temp[len],"'%s'",(temp1!=NULL ? temp1:""));
				}
				if(temp1!=NULL) Rfree(temp1);
				len=RDAstrlen(temp);
				break;
			case BOOLNS:
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+2);
					sprintf(&temp[len],"%d,",(*field->data.string_value==TRUE ? TRUE:FALSE));
				} else {
					temp=Rrealloc(temp,len+1);
					sprintf(&temp[len],"%d",(*field->data.string_value==TRUE ? TRUE:FALSE));
				}
				len=RDAstrlen(temp);
				break;
			case CHARACTERS:
				temp=Rrealloc(temp,len+4);
				sprintf(&temp[len],"'%c'",*field->data.string_value);
				len=RDAstrlen(temp);
				break;
			case DECIMALV:
			case SDECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS_NOCENTS:
			case DOLLARS:
				sprintf(stemp,"%f",*field->data.float_value);
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
					sprintf(&temp[len],"%s,",stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
					sprintf(&temp[len],"%s",stemp);
				}
				len=RDAstrlen(temp);
				break;
			case SHORTV:
			case SSHORTV:
				sprintf(stemp,"%d",*field->data.short_value);
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
					sprintf(&temp[len],"%s,",stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
					sprintf(&temp[len],"%s",stemp);
				}
				len=RDAstrlen(temp);
				break;
			case LONGV:
			case SLONGV:
				sprintf(stemp,"%d",*field->data.integer_value);
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
					sprintf(&temp[len],"%s,",stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
					sprintf(&temp[len],"%s",stemp);
				}
				len=RDAstrlen(temp);
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s].",field->type,field->name);
				break;
		}
	}
	}
	temp=Rrealloc(temp,len+12);
	sprintf(&temp[len],") ORDER BY");
	len=RDAstrlen(temp);
	for(a=0,part=key->part;a<key->numparts;++a,++part)
	{
		if((a+1)<key->numparts)
		{
			temp=Rrealloc(temp,len+RDAstrlen(part->name)+RDAstrlen(d)+6);
			sprintf(&temp[len]," `%s` %s,",part->name,(d!=NULL ? d:""));
		} else {
			temp=Rrealloc(temp,len+RDAstrlen(part->name)+RDAstrlen(d)+5);
			sprintf(&temp[len]," `%s` %s",part->name,(d!=NULL ? d:""));
		}
		len=RDAstrlen(temp);
	}
	temp=Rrealloc(temp,len+9);
	sprintf(&temp[len]," LIMIT 1");
	len=RDAstrlen(temp);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("[%s] ",(temp!=NULL ? temp:""));TRACE;
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(temp);
}
short BEGNRDmysql(short fileno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG BEGNRDmysql Searching in Module [%s] File [%d] [%s] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	ZERNRD(fileno);
	temp=MySelectAll(fileno,(-1),">","ASC");
	if(diagmysql)
	{
		LogSQL("BEGNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_store_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
short SEQNRDmysql(short fileno)
{
	NRDbuffer *NRDtmp;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG SEQNRDmysql Searching in Module [%s] File [%d] [%s] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;

	if(NRDtmp->result!=NULL)
	{
		NRDtmp->row=mysql_fetch_row(NRDtmp->result);
		if(NRDtmp->row!=NULL)
		{
			assignMYSQL_fields(NRDtmp);
			rv=0;
		} else {
			rv=(-1);
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	} else {
		rv=(-1);
		NRDtmp->result=NULL;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
static char *xMySelect(char *what,short fileno,short keyno,char *operand,char *d,int line,char *file)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	char *temp=NULL,*temp1=NULL;
	short a=0,len=27;
	NRDfield *field=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG MySelect Fileno [%d] Keyno [%d] Operand [%s] Direction [%s] from Line [%d] Program [%s].\n",fileno,keyno,operand,d,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	key=NRDtmp->key+(keyno-1);
	temp1=MySQLTableName(NRDtmp->name);
	temp=Rmalloc(RDAstrlen(temp1)+25+RDAstrlen(what));
	sprintf(temp,"%s * FROM `%s` WHERE (",(what!=NULL ? what:""),temp1);
	len=RDAstrlen(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(key->part!=NULL)
	{
		for(a=0,part=key->part;a<key->numparts;++a,++part)
		{
			field=FINDFLD(NRDtmp,part->name);
			if((a+1)<key->numparts)
			{
				temp=Rrealloc(temp,len+RDAstrlen(field->name)+4);
				sprintf(&temp[len],"`%s`,",field->name);
			} else {
				temp=Rrealloc(temp,len+RDAstrlen(field->name)+3);
				sprintf(&temp[len],"`%s`",field->name);
			}
			len=RDAstrlen(temp);
		}
	}
	temp=Rrealloc(temp,len+RDAstrlen(operand)+3);
	sprintf(&temp[len],")%s(",operand);
	len=RDAstrlen(temp);
	if(key->part!=NULL)
	{
	for(a=0,part=key->part;a<key->numparts;++a,++part)
	{
		field=FINDFLD(NRDtmp,part->name);
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
				if(!isEMPTY(field->data.string_value))
				{
					temp1=Rmalloc(RDAstrlen(field->data.string_value)*2+1);
					if(!mysql_real_escape_string(NRDtmp->db,temp1,field->data.string_value,RDAstrlen(field->data.string_value)))
					{
						prterr("mysql_real_escape_string failed %s [%s] ",(field->data.string_value!=NULL ? field->data.string_value:""),(temp1!=NULL ? temp1:""));TRACE;
					}
				} else temp1=NULL;
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(temp1)+4);
					sprintf(&temp[len],"'%s',",(temp1!=NULL ? temp1:""));
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(temp1)+3);
					sprintf(&temp[len],"'%s'",(temp1!=NULL ? temp1:""));
				}
				if(temp1!=NULL) Rfree(temp1);
				len=RDAstrlen(temp);
				break;
			case BOOLNS:
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+2);
					sprintf(&temp[len],"%d,",(*field->data.string_value==TRUE ? TRUE:FALSE));
				} else {
					temp=Rrealloc(temp,len+1);
					sprintf(&temp[len],"%d",(*field->data.string_value==TRUE ? TRUE:FALSE));
				}
				len=RDAstrlen(temp);
				break;
			case CHARACTERS:
				temp=Rrealloc(temp,len+4);
				sprintf(&temp[len],"'%c'",*field->data.string_value);
				len=RDAstrlen(temp);
				break;
			case DECIMALV:
			case SDECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS_NOCENTS:
			case DOLLARS:
				sprintf(stemp,"%f",*field->data.float_value);
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
					sprintf(&temp[len],"%s,",stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
					sprintf(&temp[len],"%s",stemp);
				}
				len=RDAstrlen(temp);
				break;
			case SHORTV:
			case SSHORTV:
				sprintf(stemp,"%d",*field->data.short_value);
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
					sprintf(&temp[len],"%s,",stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
					sprintf(&temp[len],"%s",stemp);
				}
				len=RDAstrlen(temp);
				break;
			case LONGV:
			case SLONGV:
				sprintf(stemp,"%d",*field->data.integer_value);
				if((a+1)<key->numparts)
				{
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
					sprintf(&temp[len],"%s,",stemp);
				} else {
					temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
					sprintf(&temp[len],"%s",stemp);
				}
				len=RDAstrlen(temp);
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s].",field->type,field->name);
				break;
		}
	}
	}
	temp=Rrealloc(temp,len+12);
	sprintf(&temp[len],") ORDER BY");
	len=RDAstrlen(temp);
	for(a=0,part=key->part;a<key->numparts;++a,++part)
	{
		if((a+1)<key->numparts)
		{
			temp=Rrealloc(temp,len+RDAstrlen(part->name)+RDAstrlen(d)+6);
			sprintf(&temp[len]," `%s` %s,",part->name,(d!=NULL ? d:""));
		} else {
			temp=Rrealloc(temp,len+RDAstrlen(part->name)+RDAstrlen(d)+5);
			sprintf(&temp[len]," `%s` %s",part->name,(d!=NULL ? d:""));
		}
		len=RDAstrlen(temp);
	}
	temp=Rrealloc(temp,len+9);
	sprintf(&temp[len]," LIMIT 1");
	len=RDAstrlen(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("[%s] ",(temp!=NULL ? temp:""));TRACE;
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(temp);
}
short GTENRDmysql(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG GTENRDmysql Searching for Greater Than or Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	temp=MySelect(fileno,keyno,">=","ASC");
	if(diagmysql)
	{
		LogSQL("GTENRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
short PRVNRDmysql(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG PRVNRDmysql Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	temp=MySelect(fileno,keyno,"<","DESC");
	if(diagmysql)
	{
		LogSQL("PRVNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
short LSTNRDmysql(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG LSTNRDmysql Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	temp=MySelect(fileno,keyno,">","ASC");
	if(diagmysql)
	{
		LogSQL("LSTNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
short LTENRDmysql(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG LTENRDmysql Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	temp=MySelect(fileno,keyno,"<=","ASC");
	if(diagmysql)
	{
		LogSQL("LTENRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
short FRSNRDmysql(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FRSNRDmysql Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	ZERNRD(fileno);
	temp=MySelect(fileno,keyno,">","ASC");
	if(diagmysql)
	{
		LogSQL("FRSNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
short NXTNRDmysql(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG NXTNRDmysql Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	temp=MySelect(fileno,keyno,">","ASC");
	if(diagmysql)
	{
		LogSQL("NXTNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
long MySQL_SIZNRD(short fileno)
{
	long tmp=0;
	char *temp=NULL,*temp1=NULL;
	NRDbuffer *NRDtmp;
	
	NRDtmp=NRDbuf+fileno;
	temp1=MySQLTableName(NRDtmp->name);
	temp=Rmalloc(RDAstrlen(NRDtmp->field->name)+RDAstrlen(temp1)+20);
	sprintf(temp,"SELECT %s FROM `%s`;\n",NRDtmp->field->name,temp1);
	if(temp1!=NULL) Rfree(temp1);
	if(diagmysql)
	{
		LogSQL("MySQL_SIZNRD",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		/* This may need to be mysql_store_result to improve speed for remote connections */
		NRDtmp->result=mysql_store_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			/* Casting from ulonglong to long */
			tmp=(long)mysql_num_rows(NRDtmp->result);
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
	return(tmp);
}
long SIZNRDmysqlxpert(short fileno)
{
	return(MySQL_SIZNRD(fileno));
}
long SIZNRDmysql(short fileno)
{
	return(MySQL_SIZNRD(fileno));
}
static void AssignTarget(short fileno,short keyno,void *target)
{
	NRDbuffer *NRDtmp;
	NRDfield *field=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	char *MyTarget=NULL;
	short y=0;
	int pos=0;

	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	MyTarget=(char *)target;
	key=NRDtmp->key+(keyno-1);
	for(y=0,part=key->part;y<key->numparts;++y,++part)
	{
		field=FINDFLD(NRDtmp,part->name);
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
				QUICKALLOC(field->data.string_value,field->dlen,field->len+1);
				memset(field->data.string_value,0,field->dlen);
				memcpy(field->data.string_value,&MyTarget[pos],field->len);
				unpad(field->data.string_value);
				pos+=field->len;
				break;
			case BOOLNS:
				memcpy(field->data.string_value,&MyTarget[pos],1);
				pos+=1;
				break;
			case CHARACTERS:
				memcpy(field->data.string_value,&MyTarget[pos],1);
				pos+=1;
				break;
			case DECIMALV:
			case SDECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS_NOCENTS:
			case DOLLARS:
				memcpy(field->data.float_value,&MyTarget[pos],sizeof(double));
				pos+=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				memcpy(field->data.short_value,&MyTarget[pos],sizeof(short));
				pos+=sizeof(short);
				break;
			case LONGV:
			case SLONGV:
				memcpy(field->data.integer_value,&MyTarget[pos],sizeof(int));
				pos+=sizeof(int);
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s].",field->type,field->name);
				break;
		}
	}
}
short EQLNRDKEYmysql(short fileno,short keyno,void *target)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG EQLNRDKEYmysql Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using MySQL Server Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	AssignTarget(fileno,keyno,target);
	return(EQLNRDmysql(fileno,keyno));
}
short EQLNRDmysql(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	char *temp=NULL;
	short rv=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG EQLNRDmysql Searching for equal key  with MySQL Server D-Base Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if((NRDtmp->nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	temp=MySelect(fileno,keyno,"=","ASC");
	if(diagmysql)
	{
		LogSQL("EQLNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		NRDtmp->result=mysql_use_result(NRDtmp->db);
		if(NRDtmp->result!=NULL)
		{
			NRDtmp->row=mysql_fetch_row(NRDtmp->result);
			if(NRDtmp->row!=NULL)
			{
				assignMYSQL_fields(NRDtmp);
				rv=0;
			}
			mysql_free_result(NRDtmp->result);
			NRDtmp->result=NULL;
		}
	}
	if(temp!=NULL) Rfree(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(rv);
}
short CLSNRDmysqlxpert(short fileno)
{
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Closing file [%s] using MySQL Embedded Engine.",NRDbuf[fileno].name); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if(NRDtmp->db!=NULL)
	{
		mysql_close(NRDtmp->db);
		return(0);
	}
	return(-1);
}
short CLSNRDmysql(short fileno)
{
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Closing file [%s] using MySQL Engine.",NRDbuf[fileno].name); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->result!=NULL) mysql_free_result(NRDtmp->result);
	NRDtmp->result=NULL;
	if(NRDtmp->db!=NULL)
	{
		mysql_close(NRDtmp->db);
		return(0);
	}
	return(-1);
}
MYSQL *OPNNRDmysqlxpert(NRDbuffer *NRDtmp)
{
	short err=-1;
	char *temp1=NULL;
	char *temp2=NULL;
	int x=0,y=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Open file [%s] using MySQL Server (Xpert) Engine.",NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp->db=mysql_init(NULL);
	if(!NRDtmp->db)
	{
		prterr("Error:  Couldn't initialize MYSQL structure.");
		return(NULL);
	} else {

			memset(stemp,0,101);
			sprintf(stemp,"%s",CURRENTDIRECTORY);
#ifdef WIN32			
			strcat(stemp,"\\rda\\my.cnf");
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#else
			strcat(stemp,"/rda/my.cnf");
			prterr("DIAG:  my.cnf stemp value [%s]",stemp);
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#endif
			
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_GROUP,NRDtmp->modname);
		
			if((RDAstrstr(NRDtmp->servername,":"))!=NULL)
			{
				y=strlen(NRDtmp->servername);
				temp2=Rmalloc((y+1)*(sizeof(char)));
				for(x=0;x<y;x++)
				{
					if(NRDtmp->servername[x]==':')
					{
						strncpy(temp2,NRDtmp->servername+(x+1),(y-x));
						MySQL_Port=atoi(temp2);
						
						MySQL_Server=Rmalloc((x+1)*(sizeof(char)));
						memset(MySQL_Server,0,(x+1));
						strncpy(MySQL_Server,NRDtmp->servername,(x));
					}
				}
				memset(temp2,0,(y+1));
			}else{
				MySQL_Server=stralloc(NRDtmp->servername);
				MySQL_Port=0;
			}

#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("OPNNRDmysqlxpert:  MySQL_Server [%s]   MySQL_Port [%d]\n",MySQL_Server,MySQL_Port); }
#endif /* USE_RDA_DIAGNOSTICS */
		if(!mysql_real_connect(NRDtmp->db,MySQL_Server,MySQL_Username,MySQL_Password,NRDtmp->modname,MySQL_Port,NULL,CLIENT_MULTI_RESULTS))
		{
			prterr("Error: mysql_real_connect failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(NRDtmp->db));
			mysql_close(NRDtmp->db);
			NRDtmp->db=NULL;
			return(NRDtmp->db);
		}	
	}
	if(NRDtmp->db!=NULL)
	{
		temp1=MySQLTableName(NRDtmp->name);
		NRDtmp->result=mysql_list_tables(NRDtmp->db,temp1);
		if(NRDtmp->result!=NULL)
		{
			while((NRDtmp->row=mysql_fetch_row(NRDtmp->result)))
			{
				if(!strcmp(temp1,NRDtmp->row[0]))
				{
					err=0;
					break;
				}
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		mysql_free_result(NRDtmp->result);
		NRDtmp->result=NULL;
	}
	if(err)
	{
		prterr("Error: Unable to find table %s in the database.",NRDtmp->name);
		return(NULL);
	}
	return(NRDtmp->db);
}
MYSQL *OPNNRDmysql(NRDbuffer *NRDtmp)
{
	short err=-1;
	char *temp1=NULL;
	char *temp2=NULL;
	int x=0,y=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Open file [%s] using MySQL Server Engine.",NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp->db=mysql_init(NULL);
	if(!NRDtmp->db)
	{
		prterr("Error:  Couldn't initialize MYSQL structure.");
	} else {
			memset(stemp,0,101);
			sprintf(stemp,"%s",CURRENTDIRECTORY);
#ifdef WIN32			
			strcat(stemp,"\\rda\\my.cnf");
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#else
			strcat(stemp,"/rda/my.cnf");
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#endif
			
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_GROUP,NRDtmp->modname);

			if((RDAstrstr(NRDtmp->servername,":"))!=NULL)
			{
				y=strlen(NRDtmp->servername);
				temp2=Rmalloc((y+1)*(sizeof(char)));
				for(x=0;x<y;x++)
				{
					if(NRDtmp->servername[x]==':')
					{
						strncpy(temp2,NRDtmp->servername+(x+1),(y-x));
						MySQL_Port=atoi(temp2);
						
						MySQL_Server=Rmalloc((x+1)*(sizeof(char)));
						memset(MySQL_Server,0,(x+1));
						strncpy(MySQL_Server,NRDtmp->servername,(x));
					}
				}
				memset(temp2,0,(y+1));
			} else {
				MySQL_Server=stralloc(NRDtmp->servername);
				MySQL_Port=0;
			}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("OPNNRDmysql:  MySQL_Server [%s]   MySQL_Port [%d]\n",(MySQL_Server!=NULL ? MySQL_Server:""),MySQL_Port); }
#endif /* USE_RDA_DIAGNOSTICS */
		if(!mysql_real_connect(NRDtmp->db,MySQL_Server,MySQL_Username,MySQL_Password,NRDtmp->modname,MySQL_Port,NULL,CLIENT_MULTI_RESULTS))
		{
			prterr("Error: mysql_real_connect failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(NRDtmp->db));
			mysql_close(NRDtmp->db);
			NRDtmp->db=NULL;
			return(NRDtmp->db);
		}	
	}
	if(NRDtmp->db!=NULL)
	{
		temp1=MySQLTableName(NRDtmp->name);
		NRDtmp->result=mysql_list_tables(NRDtmp->db,temp1);
		if(NRDtmp->result!=NULL)
		{
			while((NRDtmp->row=mysql_fetch_row(NRDtmp->result)))
			{
				if(!strcmp(temp1,NRDtmp->row[0]))
				{
					err=0;
					break;
				}
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		mysql_free_result(NRDtmp->result);
		NRDtmp->result=NULL;
	}
	if(err)
	{
		prterr("Error: Unable to find table %s in the database.",NRDtmp->name);
	}
	return(NRDtmp->db);
}
short CRTNRDmysqlxpert(NRDbuffer *NRDtmp)
{
	char *temp=NULL;
	char *temp2=NULL;
	int x=0,y=0;
	MYSQL *db=NULL;
	short r=(-1);

	db=mysql_init(NULL);
	if(db!=NULL)
	{

		memset(stemp,0,101);
		sprintf(stemp,"%s",CURRENTDIRECTORY);
#ifdef WIN32			
		strcat(stemp,"\\rda\\my.cnf");
		mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#else
		strcat(stemp,"/rda/my.cnf");
		prterr("DIAG:  my.cnf stemp value [%s]",stemp);
		mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#endif
			
		mysql_options(db,MYSQL_READ_DEFAULT_GROUP,NRDtmp->modname);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("NRDtmp->servername [%s]\n",NRDtmp->servername); }
#endif /* USE_RDA_DIAGNOSTICS */
		if((RDAstrstr(NRDtmp->servername,":"))!=NULL)
		{
			y=strlen(temp);
			temp2=Rmalloc((y+1)*(sizeof(char)));
			for(x=0;x<y;x++)
			{
				if(temp[x]==':')
				{
					strncpy(temp2,temp+(x+1),(y-x));
					MySQL_Port=atoi(temp2);
					
					MySQL_Server=Rmalloc((x+1)*(sizeof(char)));
					memset(MySQL_Server,0,(x+1));
					strncpy(MySQL_Server,temp,(x));
				}
			}
			memset(temp2,0,(y+1));
		}else{
				MySQL_Server=stralloc(NRDtmp->servername);
				MySQL_Port=0;
		}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("CRTNRDmysqlxpert:  MySQL_Server [%s]   MySQL_Port [%d]\n",MySQL_Server,MySQL_Port); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(mysql_real_connect(db,MySQL_Server,MySQL_Username,MySQL_Password,NULL,MySQL_Port,NULL,CLIENT_MULTI_RESULTS))
	{
		temp=Rmalloc(RDAstrlen(NRDtmp->modname)+36);
		sprintf(temp,"CREATE DATABASE IF NOT EXISTS '%s';\n",NRDtmp->modname);
		if(diagmysql)
		{
			LogSQL("CRTNRDmysqlxpert",temp);
		}
		if(!mysql_real_query(db,temp,RDAstrlen(temp)))
		{
		} else {
			prterr("Error: mysql_real_query failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(db));
			return(r);
		}
		if(temp!=NULL) Rfree(temp);
	} else {
		mysql_close(db);
		return(-1);
	}
	mysql_close(db);
	} else return(r);
	NRDtmp->db=mysql_init(NULL);
	if(!NRDtmp->db)
	{
		prterr("Error:  Couldn't initialize MYSQL structure.");
		return(r);
	} else {

			memset(stemp,0,101);
			sprintf(stemp,"%s",CURRENTDIRECTORY);
#ifdef WIN32			
			strcat(stemp,"\\rda\\my.cnf");
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#else
			strcat(stemp,"/rda/my.cnf");
			prterr("DIAG:  my.cnf stemp value [%s]",stemp);
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#endif
			
			mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_GROUP,NRDtmp->modname);
			if((RDAstrstr(NRDtmp->servername,":"))!=NULL)
			{
				y=strlen(temp);
				temp2=Rmalloc((y+1)*(sizeof(char)));
				for(x=0;x<y;x++)
				{
					if(temp[x]==':')
					{
						strncpy(temp2,temp+(x+1),(y-x));
						MySQL_Port=atoi(temp2);
						
						MySQL_Server=Rmalloc((x+1)*(sizeof(char)));
						memset(MySQL_Server,0,(x+1));
						strncpy(MySQL_Server,temp,(x));
					}
				}
				memset(temp2,0,(y+1));
			}else{
				MySQL_Server=stralloc(NRDtmp->servername);
				MySQL_Port=0;
			}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("CRTNRDmysqlxpert:  MySQL_Server [%s]   MySQL_Port [%d]\n",MySQL_Server,MySQL_Port); }
#endif /* USE_RDA_DIAGNOSTICS */
		if(!mysql_real_connect(NRDtmp->db,MySQL_Server,MySQL_Username,MySQL_Password,NRDtmp->modname,MySQL_Port,NULL,CLIENT_MULTI_RESULTS))
		{
			prterr("Error: mysql_real_connect failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(NRDtmp->db));
			mysql_close(NRDtmp->db);
			NRDtmp->db=NULL;
			return(r);
		}	
	}
	temp=WriteSQL(NRDtmp);
	if(diagmysql)
	{
		LogSQL("CRTNRDmysqlxpert",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
		prterr("Passed: mysql_real_query for %s->%s with %s.",NRDtmp->modname,NRDtmp->name);
		r=0;
	} else {
		prterr("Error: mysql_real_query failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(NRDtmp->db));
	}
	mysql_close(NRDtmp->db);
	NRDtmp->db=NULL;
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
	return(r);
}
short CRTNRDmysql(NRDbuffer *NRDtmp)
{
	char *temp=NULL;
	char *temp2=NULL;
	int x=0,y=0;
	MYSQL *db=NULL;
	short r=(-1);

	db=mysql_init(NULL);
	if(db!=NULL)
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s",CURRENTDIRECTORY);
#ifdef WIN32			
		strcat(stemp,"\\rda\\my.cnf");
		mysql_options(db,MYSQL_READ_DEFAULT_FILE,stemp);
#else
		strcat(stemp,"/rda/my.cnf");
		prterr("DIAG:  my.cnf stemp value [%s]",stemp);
		mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#endif
			
		mysql_options(db,MYSQL_READ_DEFAULT_GROUP,NRDtmp->modname);
		if((RDAstrstr(NRDtmp->servername,":"))!=NULL)
		{
			y=strlen(temp);
			temp2=Rmalloc((y+1)*(sizeof(char)));
			for(x=0;x<y;x++)
			{
				if(temp[x]==':')
				{
					strncpy(temp2,temp+(x+1),(y-x));
					MySQL_Port=atoi(temp2);
					
					MySQL_Server=Rmalloc((x+1)*(sizeof(char)));
					memset(MySQL_Server,0,(x+1));
					strncpy(MySQL_Server,temp,(x));
				}
			}
			memset(temp2,0,(y+1));
		}else{
			MySQL_Server=stralloc(NRDtmp->servername);
			MySQL_Port=0;
		}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("CRTNRDmysql:  MySQL_Server [%s]   MySQL_Port [%d]\n",MySQL_Server,MySQL_Port); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(mysql_real_connect(db,MySQL_Server,MySQL_Username,MySQL_Password,NULL,MySQL_Port,NULL,CLIENT_MULTI_RESULTS))
	{
		temp=Rmalloc(RDAstrlen(NRDtmp->modname)+36);
		sprintf(temp,"CREATE DATABASE IF NOT EXISTS `%s`;\n",NRDtmp->modname);
		if(diagmysql)
		{
			LogSQL("CRTNRDmysql",temp);
		}
		if(!mysql_real_query(db,temp,RDAstrlen(temp)))
		{
		} else {
			prterr("Error: mysql_real_query failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(db));
			return(r);
		}
		if(temp!=NULL) Rfree(temp);
	}
	mysql_close(db);
	} else return(r);
	NRDtmp->db=mysql_init(NULL);
	if(!NRDtmp->db)
	{
		prterr("Error:  Couldn't initialize MYSQL structure.");
		return(r);
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("CRTNRDmysql:  MySQL_Server [%s]   MySQL_Port [%d]\n",MySQL_Server,MySQL_Port); }
#endif /* USE_RDA_DIAGNOSTICS */

		memset(stemp,0,101);
		sprintf(stemp,"%s",CURRENTDIRECTORY);
#ifdef WIN32			
		strcat(stemp,"\\rda\\my.cnf");
		mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#else
		strcat(stemp,"/rda/my.cnf");
		prterr("DIAG:  my.cnf stemp value [%s]",stemp);
		mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_FILE,stemp);
#endif
			
		mysql_options(NRDtmp->db,MYSQL_READ_DEFAULT_GROUP,NRDtmp->modname);
		if(!mysql_real_connect(NRDtmp->db,MySQL_Server,MySQL_Username,MySQL_Password,NRDtmp->modname,MySQL_Port,NULL,CLIENT_MULTI_RESULTS))
		{
			prterr("Error: mysql_real_connect failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(NRDtmp->db));
			mysql_close(NRDtmp->db);
			NRDtmp->db=NULL;
			return(r);
		}	
	}
	temp=WriteSQL(NRDtmp);
	if(diagmysql)
	{
		LogSQL("CRTNRDmysql",temp);
	}
	if(!mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{
			prterr("Passed: mysql_real_query for %s->%s with %s.",NRDtmp->modname,NRDtmp->name);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		r=0;
#ifdef USE_RDA_DIAGNOSTICS
	} else if(diagnrd)
	{
		prterr("Error: mysql_real_query failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(NRDtmp->db));
#endif /* USE_RDA_DIAGNOSTICS */
	}
	mysql_close(NRDtmp->db);
	NRDtmp->db=NULL;
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
	return(r);
}
void EXITNRDmysql()
{
	mysql_server_end();
}
void INITNRDmysql()
{
	mysql_server_init(0,NULL,NULL);
}
short DELETENRDmysql(char *s,char *m,char *f)
{
	MYSQL *db=NULL;
	char *temp=NULL,*temp1=NULL;
	char *temp2=NULL;
	int x=0,y=0;
	

	db=mysql_init(NULL);

	memset(stemp,0,101);
	sprintf(stemp,"%s",CURRENTDIRECTORY);
#ifdef WIN32			
	strcat(stemp,"\\rda\\my.cnf");
	mysql_options(db,MYSQL_READ_DEFAULT_FILE,stemp);
#else
	strcat(stemp,"/rda/my.cnf");
	prterr("DIAG:  my.cnf stemp value [%s]",stemp);
	mysql_options(db,MYSQL_READ_DEFAULT_FILE,stemp);
#endif
			
	mysql_options(db,MYSQL_READ_DEFAULT_GROUP,m);
	if((RDAstrstr(s,":"))!=NULL)
	{
		y=strlen(s);
		temp2=Rmalloc((y+1)*(sizeof(char)));
		for(x=0;x<y;x++)
		{
			if(s[x]==':')
			{
				strncpy(temp2,s+(x+1),(y-x));
				MySQL_Port=atoi(temp2);
				
				MySQL_Server=Rmalloc((x+1)*(sizeof(char)));
				memset(MySQL_Server,0,(x+1));
				strncpy(MySQL_Server,s,(x));
			}
		}
		memset(temp2,0,(y+1));
	}else{
		MySQL_Server=stralloc(s);
		MySQL_Port=0;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DELETENRDmysql:  MySQL_Server [%s]   MySQL_Port [%d]\n",MySQL_Server,MySQL_Port); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(mysql_real_connect(db,MySQL_Server,MySQL_Username,MySQL_Password,m,MySQL_Port,NULL,0))
	{
		temp1=MySQLTableName(f);
		temp=Rmalloc(RDAstrlen(temp1)+30);
		sprintf(temp,"DROP TABLE IF EXISTS `%s`",temp1);
		if(diagmysql)
		{
			LogSQL("DELETENRDmysql",temp);
		}
		if(mysql_real_query(db,temp,RDAstrlen(temp)))
		{
		}
		if(temp!=NULL) Rfree(temp);
		if(temp1!=NULL) Rfree(temp1);
	}
	if(temp2!=NULL) Rfree(temp2);
	if(MySQL_Server!=NULL) Rfree(MySQL_Server);
	mysql_close(db);
}
short WRTNRDallmysql(short fileno,short trnmode,char *fieldname)
{
	NRDbuffer *NRDtmp;
	short numflds=0,x=0;
	NRDfield *field=NULL,*fields=NULL;
	short return_value=0;
	int len=0;
	char *temp=NULL,*temp1=NULL;
	char *temp2=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Writing current record to file [%s] trnmode [%d] fieldname [%s] using MySQL Server Engine.",NRDbuf[fileno].name,trnmode,(fieldname!=NULL ? fieldname:""));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	numflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	temp1=MySQLTableName(NRDtmp->name);
	if(trnmode==TRUE && !isEMPTY(fieldname))
	{
		temp=Rmalloc(RDAstrlen(temp1)+18);
		sprintf(temp,"INSERT INTO `%s` (",temp1);
	} else {
/* Work REPLACE into mix */
		temp=Rmalloc(RDAstrlen(temp1)+25);
		sprintf(temp,"REPLACE INTO `%s` (",temp1);
	}
	if(temp1!=NULL) Rfree(temp1);
	len=RDAstrlen(temp);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<numflds;++x,++field)
		{
			if((x+1)<numflds)
			{
				temp=Rrealloc(temp,len+RDAstrlen(field->name)+4);
				sprintf(&temp[len],"`%s`,",field->name);
			} else {
				temp=Rrealloc(temp,len+RDAstrlen(field->name)+3);
				sprintf(&temp[len],"`%s`",field->name);
			}
			len=RDAstrlen(temp);
		}
	}
	temp=Rrealloc(temp,len+11);
	sprintf(&temp[len],") VALUES (");
	len=RDAstrlen(temp);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<numflds;++x,++field)
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
					if(!isEMPTY(field->data.string_value))
					{
						temp1=Rmalloc(RDAstrlen(field->data.string_value)*2+1);
						mysql_real_escape_string(NRDtmp->db,temp1,field->data.string_value,RDAstrlen(field->data.string_value));
					} else temp1=NULL;
					if(!isEMPTY(temp1))
					{
						if((x+1)<numflds)
						{
							temp=Rrealloc(temp,len+RDAstrlen(temp1)+4);
							sprintf(&temp[len],"'%s',",(temp1!=NULL ? temp1:""));
						} else {
							temp=Rrealloc(temp,len+RDAstrlen(temp1)+3);
							sprintf(&temp[len],"'%s'",(temp1!=NULL ? temp1:""));
						}
					} else {
						if((x+1)<numflds)
						{
							temp=Rrealloc(temp,len+6);
							sprintf(&temp[len],"NULL,");
						} else {
							temp=Rrealloc(temp,len+5);
							sprintf(&temp[len],"NULL");
						}
					}
					if(temp1!=NULL) Rfree(temp1);
					len=RDAstrlen(temp);
					break;
				case BOOLNS:
					if((x+1)<numflds)
					{
						temp=Rrealloc(temp,len+2);
						sprintf(&temp[len],"%d,",(*field->data.string_value==TRUE ? TRUE:FALSE));
					} else {
						temp=Rrealloc(temp,len+1);
						sprintf(&temp[len],"%d",(*field->data.string_value==TRUE ? TRUE:FALSE));
					}
					len=RDAstrlen(temp);
					break;
				case CHARACTERS:
					if((x+1)<numflds)
					{
						temp=Rrealloc(temp,len+5);
						sprintf(&temp[len],"'%c',",*field->data.string_value);
					} else {
						temp=Rrealloc(temp,len+4);
						sprintf(&temp[len],"'%c'",*field->data.string_value);
					}
					len=RDAstrlen(temp);
					break;
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					sprintf(stemp,"%f",*field->data.float_value);
					if((x+1)<numflds)
					{
						temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
						sprintf(&temp[len],"%s,",stemp);
					} else {
						temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
						sprintf(&temp[len],"%s",stemp);
					}
					len=RDAstrlen(temp);
					break;
				case DOLLARS_NOCENTS:
					sprintf(stemp,"%.0f",*field->data.float_value);
					if((x+1)<numflds)
					{
						temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
						sprintf(&temp[len],"%s,",stemp);
					} else {
						temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
						sprintf(&temp[len],"%s",stemp);
					}
					len=RDAstrlen(temp);
					break;
				case DOLLARS:
					sprintf(stemp,"%.02f",*field->data.float_value);
					if((x+1)<numflds)
					{
						temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
						sprintf(&temp[len],"%s,",stemp);
					} else {
						temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
						sprintf(&temp[len],"%s",stemp);
					}
					len=RDAstrlen(temp);
					break;
				case SHORTV:
				case SSHORTV:
					if(trnmode==TRUE && !RDAstrcmp(fieldname,field->name) && NRDtmp->db_engine==NRDMySQLAutoEngine)
					{
						if((x+1)<numflds)
						{
							temp=Rrealloc(temp,len+9);
							sprintf(&temp[len],"DEFAULT,");
						} else {
							temp=Rrealloc(temp,len+8);
							sprintf(&temp[len],"DEFAULT");
						}
					} else {
						sprintf(stemp,"%d",*field->data.short_value);
						if((x+1)<numflds)
						{
							temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
							sprintf(&temp[len],"%s,",stemp);
						} else {
							temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
							sprintf(&temp[len],"%s",stemp);
						}
					}
					len=RDAstrlen(temp);
					break;
				case LONGV:
				case SLONGV:
					if(trnmode==TRUE && !RDAstrcmp(fieldname,field->name) && NRDtmp->db_engine==NRDMySQLAutoEngine)
					{
						if((x+1)<numflds)
						{
							temp=Rrealloc(temp,len+9);
							sprintf(&temp[len],"DEFAULT,");
						} else {
							temp=Rrealloc(temp,len+8);
							sprintf(&temp[len],"DEFAULT");
						}
					} else {
						sprintf(stemp,"%d",*field->data.integer_value);
						if((x+1)<numflds)
						{
							temp=Rrealloc(temp,len+RDAstrlen(stemp)+2);
							sprintf(&temp[len],"%s,",stemp);
						} else {
							temp=Rrealloc(temp,len+RDAstrlen(stemp)+1);
							sprintf(&temp[len],"%s",stemp);
						}
					}
					len=RDAstrlen(temp);
					break;
				default:
					prterr("Error Field Type [%d] is invalid in Field [%s].",field->type,field->name);
					break;
			}
		}
	}
	temp=Rrealloc(temp,len+4);
	sprintf(&temp[len],");\n");
	len=RDAstrlen(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("[%s] ",(temp!=NULL ? temp:""));TRACE;
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(diagmysql)
	{
		LogSQL("WRTNRDallmysql",temp);
	}
	if(mysql_real_query(NRDtmp->db,temp,RDAstrlen(temp)))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{
			prterr("Error: mysql_real_query INSERT/REPLACE failed for %s->%s with %s.",NRDtmp->modname,NRDtmp->name,mysql_error(NRDtmp->db));
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return_value=(-1);
	}
	if(trnmode && !isEMPTY(fieldname) && NRDtmp->db_engine==NRDMySQLAutoEngine)
	{

/* SOMEHOW THE WAY OUR OPENSS DATABASES ARE WRITTEN THIS ISN'T WORKING PROPERLY */


		field=FLDNRD(fileno,fieldname);
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
			case BOOLNS:
			case CHARACTERS:
				break;
			case DOLLARS_NOCENTS:
			case DOLLARS:
				*field->data.float_value=mysql_insert_id(NRDtmp->db);
				break;
			case DOUBLEV:
			case SDOUBLEV:
				*field->data.float_value=mysql_insert_id(NRDtmp->db);
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=mysql_insert_id(NRDtmp->db);
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=mysql_insert_id(NRDtmp->db);
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s].",field->type,field->name);
				break;
		}
		}
	}
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
	return(return_value);
}
short WRTNRDmysqlxpert(short fileno,short trnmode,char *fieldname)
{
	WRTNRDallmysql(fileno,trnmode,fieldname);
}
short WRTNRDmysql(short fileno,short trnmode,char *fieldname)
{
	WRTNRDallmysql(fileno,trnmode,fieldname);
}
#else /* NOT USE_MYSQL */
#endif /* USE_MYSQL */
/*lint +e525*/
