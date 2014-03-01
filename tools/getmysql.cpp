/* menu.c - Program to execute a specific menu */
#ifndef WIN32
#define __NAM__ "getmysql.lnx"
#endif
#ifdef WIN32
#define __NAM__ "getmysql.exe"
#endif
#include <app.h>
#include <mix.h>
#include <nrdp.h>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char AutoEngine=FALSE;

NRDfield DetermineField(char *s)
{
	NRDfield fldx;
	char *temp=NULL;

	if(RDAstrstr(s,"auto_increment") || RDAstrstr(s,"AUTO_INCREMENT")) AutoEngine=TRUE;
	fldx.dlen=0;
	if(RDAstrstr(s,"varchar")) fldx.type=0;
	else if(RDAstrstr(s,"char")) fldx.type=1;
	else if(RDAstrstr(s," text") || RDAstrstr(s,"longtext") || RDAstrstr(s,"blob")) 
	{
		fldx.type=SCROLLEDTEXT;
		fldx.len=0;
	} else if(RDAstrstr(s,"tinyint")) fldx.type=BOOLNS;
	else if(RDAstrstr(s,"int")) fldx.type=LONGV;
	else if(RDAstrstr(s,"decimal(2,0)")) fldx.type=DOUBLEV;
	else if(RDAstrstr(s,"decimal(") && RDAstrstr(s,",0)")) fldx.type=DOUBLEV;
	else if(RDAstrstr(s,"decimal")) fldx.type=SDECIMALV;
	else if(RDAstrstr(s,"double"))
	{
		fldx.type=SDOUBLEV;
		fldx.len=15;
	} else if(RDAstrstr(s,"datetime")) 
	{
		fldx.type=1;
		fldx.len=19;
	} else if(RDAstrstr(s,"date"))
	{
		fldx.type=1;
		fldx.len=10;
	} else fldx.type=0;

	fldx.name=stralloc(&s[3]);
	for(temp=fldx.name;*temp;++temp) 
	{
		if(*temp=='`')
		{
			*temp=0;
			break;
		}
	}	
	if(!RDAstrstr(s,"datetime") && !RDAstrstr(s,"text"))
	{
	temp=RDAstrstr(s,"(");
	if(temp!=NULL)
	{
		++temp;
		fldx.len=atoi(temp);
	}
	}
	switch(fldx.type)
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
			fldx.data.string_value=NULL;
			fldx.dlen=0;
			break;
		case BOOLNS:
			fldx.data.string_value=Rmalloc(1);
			*fldx.data.string_value=FALSE;
			fldx.dlen=1;
			break;
		case CHARACTERS:
			fldx.data.string_value=Rmalloc(1);
			*fldx.data.string_value=0;
			fldx.dlen=1;
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			fldx.data.float_value=Rmalloc(sizeof(double));
			*fldx.data.float_value=0.0;
			fldx.dlen=sizeof(double);
			break;
		case SHORTV:
		case SSHORTV:
			fldx.data.short_value=Rmalloc(sizeof(short));
			*fldx.data.short_value=0;
			fldx.dlen=sizeof(short);
			break;
		case LONGV:
		case SLONGV:
			fldx.data.integer_value=Rmalloc(sizeof(int));
			*fldx.data.integer_value=0;
			fldx.dlen=sizeof(int);
			break;
		default:
			break;
	}
	if(fldx.len>200) fldx.len=0;
	if(fldx.type==0 && fldx.len!=0) fldx.type=1;
	return(fldx);
}
NRDkey *PrimaryKey(char *s)
{
	NRDkey *key=Rmalloc(sizeof(NRDkey));
	NRDpart *part=NULL;
	char *temp=NULL,*temp1=NULL,*temp2=NULL;
	int x=0,y=0,len=0;

	key->name=stralloc("PRIMARY KEY");
	temp1=stralloc(&s[17]);
	for(temp=temp1;*temp;++temp) if(*temp==',') ++x;
	if(x==0) x=1;
	key->numparts=x;
	key->part=Rmalloc(x*sizeof(NRDpart));
	len=0;
	for(y=0;y<x;++y)
	{
		part=key->part+y;
		part->offset=0;
		temp2=stralloc(&temp1[len]);
		for(temp=temp2;*temp;++temp)
		{
			++len;
			if(*temp=='`')
			{
				*temp=0;
				len+=2;
				part->name=stralloc(temp2);
				part->offset=0;
				break;
			}
		}
		if(temp2!=NULL) Rfree(temp2);
	}
	return(key);
}

#ifdef CPPMAIN
void c_main(int argc,char **argv)
#else
void main(int argc,char **argv)
#endif
{
	FILE *fp=NULL;
	char *module=NULL,*filename=NULL,*temp=NULL;
	NRDpart *part=NULL;
	NRDkey *key=NULL,*keys=NULL;
	NRDfield *field=NULL,*fields=NULL,fldx;
	short nofields=0,nokeys=0,dbengine=0;
	short line_style=(-1);
	char buffer[4096];
	short x=0,y=0;
	

	if(InitializeSubsystems(argc,argv,"DATABASE","GET MYSQL DATABASES")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc<3) return;
	fp=fopen(argv[1],"r");
	if(fp!=NULL)
	{
		while((readline(fp,buffer,4095))!=(-1))
		{
			if(diagapps)
			{
				prterr("%s\n",(buffer!=NULL ? buffer:""));
			}
			if(!strncmp(buffer,"USE ",4))
			{
				module=stralloc(&buffer[4]);
				module[RDAstrlen(module)-1]=0;
			} else if(!strncmp(buffer,"-- ",3))
			{
				line_style=-1;
			} else if(!strncmp(buffer,"-- FOREIGN KEYS",15))
			{
				line_style=-1;
			} else if(!strncmp(buffer,"ALTER TABLE ",12))
			{
				line_style=-1;
			} else if(!strncmp(buffer,"  ADD CONSTRAINT",16))
			{
				line_style=-1;
			} else if(!strncmp(buffer,"CREATE TABLE ",13))
			{
				filename=stralloc(&buffer[14]);
				for(temp=filename;*temp;++temp) 
				{
					if(*temp=='`') 
					{
						*temp=0;
						break;
					}
				}
				line_style=0;
			} else if(!strncmp(buffer,"  PRIMARY KEY  (",16))
			{
				line_style=1;
			} else if(!strncmp(buffer,"  PRIMARY KEY (",15))
			{
				line_style=1;
			} else if(!strncmp(buffer,"  UNIQUE KEY ",13))
			{
				line_style=-1;
			} else if(!strncmp(buffer,"  CONSTRAINT ",13))
			{
				line_style=-1;
			} else if(!strncmp(buffer,"  KEY ",6))
			{
				line_style=2;
			} else if(!strncmp(buffer,") ENGINE=",9)) 
			{
				if(nofields>0 && nokeys>0)
				{
				if(AutoEngine) dbengine=NRDMySQLAutoEngine;
				else dbengine=NRDMySQLEngine;
				if(putnrd(module,filename,dbengine,nofields,fields,nokeys,keys,argv[2]))
				{
					prterr("Error creating File Definition [%s] [%s].  Check permissions and existance of Binary Library [%s.FIL].",module,filename,module);
				}
				}
				AutoEngine=FALSE;
				for(x=0,field=fields;x<nofields;++x,++field)
				{
					if(field->name!=NULL) Rfree(field->name);
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
							if(field->data.string_value!=NULL) Rfree(field->data.string_value);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							if(field->data.float_value!=NULL) Rfree(field->data.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							if(field->data.short_value!=NULL) Rfree(field->data.short_value);
							break;
						case LONGV:
						case SLONGV:
							if(field->data.integer_value!=NULL) Rfree(field->data.integer_value);
							break;
						default:
							break;
					}
				}
				if(field!=NULL) Rfree(fields);
				nofields=0;
				for(x=0,key=keys;x<nokeys;++x,++key)
				{
					if(key->name!=NULL) Rfree(key->name);
					for(y=0,part=key->part;y<key->numparts;++y,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(key->part);
					key->numparts=0;
				}
				if(keys!=NULL) Rfree(keys);
				nokeys=0;
				if(filename!=NULL) Rfree(filename);
				line_style=(-1);
			}
			switch(line_style)
			{
				default: /* lines to skip */
				case -1:
					break;
				case 0: /* Expecting a Field */
					if(strncmp(buffer,"CREATE TABLE ",13))
					{
						fldx=DetermineField(buffer);
						if(fields!=NULL)
						{
							fields=Rrealloc(fields,((nofields+1)*sizeof(NRDfield)));
						} else fields=Rmalloc(sizeof(NRDfield));
						field=fields+nofields;
						field->name=stralloc(fldx.name);
						field->type=fldx.type;
						field->len=fldx.len;
						field->dlen=fldx.dlen;
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
								field->data.string_value=NULL;
								break;
							case BOOLNS:
							case CHARACTERS:
								field->data.string_value=Rmalloc(1);
								*field->data.string_value=0;
								break;
							case DECIMALV:
							case DOUBLEV:
							case SDOUBLEV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case SDECIMALV:
								field->data.float_value=Rmalloc(sizeof(double));
								*field->data.float_value=0;
								break;
							case SHORTV:
							case SSHORTV:
								field->data.short_value=Rmalloc(sizeof(short));
								*field->data.short_value=0;
								break;
							case LONGV:
							case SLONGV:
								field->data.integer_value=Rmalloc(sizeof(int));
								*field->data.integer_value=0;
								break;
							default:
								break;
						}
						++nofields;
						
					}
					break;
				case 1: /* Expecting Primary Key */
					keys=PrimaryKey(buffer);
					nokeys=1;
					break;
				case 2: /* Expecting Key */
					break;
			}
		}
		fclose(fp);
	}
	ShutdownSubsystems();
	exit(0);
}
