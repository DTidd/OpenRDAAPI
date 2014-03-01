/* server.c - functions for managing DBservsers */
#include<nrdp.hpp>

void xFreeNRDconnect(int line,char *file)
{
	NRDserver *s;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FreeNRDconnect at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(dbcons.s!=NULL)
	{
		for(x=0,s=dbcons.s;x<dbcons.num;++x,++s)
		{
			switch(s->engine)
			{
				case NRDRdaEngine: 
					break;
#ifdef INFORMIX_ISAM
				case NRDInformixIsamEngine:
					break;
#endif
#ifdef USE_MYISAM
				case NRDMyIsamEngine:
					break;
#endif
#ifdef USE_MYSQL
				case NRDMySQLXpertEngine:
				case NRDMySQLEngine:
				case NRDMySQLAutoEngine:
					break;
#endif
#ifdef USE_BERKELEY_BTREE
				case NRDBerkeleyBtree:
					break;
#endif
#ifdef USE_CTREE
				case NRDCtreeEngine: 
				case NRDLocalCtreeEngine: 
					if(UnRegisterCtree(s->name))
					{
						prterr("Error C-tree Couldn't UnRegister Server [%s].",s->name);
					}
					break;
#endif
				default:
					prterr("Error Unknown Database Engine [[%d].",s->engine);
					break;
			}
			if(s->name!=NULL) Rfree(s->name);
		}
		Rfree(dbcons.s);
	}
	dbcons.s=NULL;
	dbcons.num=0;
}
short xRecordNRDconnect(char *name,short engine,int line,char *file)
{
	NRDserver *s=NULL;
	int x,y;
	char *tmp=NULL;

	if(engine==NRDCtreeEngine)
	{
		if(isEMPTY(name))
		{
			tmp=RDA_GetEnv("CTREE_DEFAULT_SERVER");
			name=stralloc(tmp);	
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG RecordNRDconnect for [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(dbcons.s!=NULL)
	{
		if(dbcons.cur>(-1) && dbcons.cur<dbcons.num)
		{
			s=dbcons.s+dbcons.cur;
			if(engine==NRDLocalCtreeEngine) 
			{
				if(!RDAstrcmp(s->name,"LOCAL")) return(0);
			} else {
				if(!RDAstrcmp(s->name,name)) return(0);
			}
		}
		for(x=0,s=dbcons.s;x<dbcons.num;++x,++s)
		{
			if(engine==NRDLocalCtreeEngine)
			{
				if(!RDAstrcmp(s->name,"LOCAL")) y=TRUE;
					else y=FALSE;
			} else {
				if(!RDAstrcmp(s->name,name)) y=TRUE;
					else y=FALSE;
			}
			if(y==TRUE)
			{
				dbcons.cur=x;
				switch(engine)
				{
					case NRDRdaEngine: 
						break;
#ifdef INFORMIX_ISAM
					case NRDInformixIsamEngine:
						break;
#endif
#ifdef USE_MYISAM
					case NRDMyIsamEngine:
						break;
#endif
#ifdef USE_MYSQL
					case NRDMySQLXpertEngine:
					case NRDMySQLEngine:
					case NRDMySQLAutoEngine:
						break;
#endif
#ifdef USE_BERKELEY_BTREE
					case NRDBerkeleyBtree:
						break;
#endif
#ifdef USE_CTREE
					case NRDCtreeEngine: 
					case NRDLocalCtreeEngine: 
						if(SwitchCtree(s->name))
						{
							prterr("Error C-tree Couldn't Switch Server [%s].",s->name);
							return(-1);
						}
						break;
#endif
					default:
						prterr("Error Unknown Database Engine [[%d].",engine);
						break;
				}
				return(0);
			}
		}
	}
	if(dbcons.s!=NULL)
	{
		dbcons.s=Rrealloc(dbcons.s,(dbcons.num+1)*sizeof(NRDserver));
	} else {
		dbcons.s=Rmalloc(sizeof(NRDserver));
		dbcons.num=0;
	}
	s=dbcons.s+dbcons.num;
	if(engine==NRDLocalCtreeEngine)
	{
		s->name=stralloc("LOCAL");
	} else {
		if(!isEMPTY(name)) s->name=stralloc(name);
			else s->name=NULL;
	}
	dbcons.cur=dbcons.num;
	s->engine=engine;
	switch(engine)
	{
		case NRDRdaEngine: 
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			break;
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine: 
			if(RegisterCtree(name))
			{
				prterr("Error C-tree Couldn't Register Server [%s].",name);
				return(-1);
			}
/* ORGINAL */
			if(INTISAMX(IBUFFctree,MAXFILESctree,NODESECTSctree,DBUFFctree,USERPRF_LOCLIB,USERLOGIN,NULL,s->name))
			{
				prterr("Error C-tree InitISAM failed %d %s.",isam_err,ctdb_errors(isam_err));
				return(-1);
			}
			break;
		case NRDLocalCtreeEngine: 
			if(RegisterCtree(s->name))
			{
				prterr("Error C-tree Couldn't Register Server [%s].",s->name);
				return(-1);
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diagnrd)
			{
				prterr("DIAG Setting Ctree Local Buffer Sizes to Index Buffers=[%d]  Max Files=[%d]  Node Sectors=[%d]  Data Buffers=[%d] at line [%d] program [%s].",IBUFFctree,MAXFILESctree,NODESECTSctree,DBUFFctree,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			
			if(INTISAMX(IBUFFctree,MAXFILESctree,NODESECTSctree,DBUFFctree,USERPRF_LOCLIB,USERLOGIN,NULL,s->name))
			{
				prterr("Error C-tree InitISAM failed %d %s.",isam_err,ctdb_errors(isam_err));
				return(-1);
			}
			break;
#endif
		default:
			prterr("Error Unknown Database Engine [[%d].",engine);
			break;
	}
	++dbcons.num;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG RecordNRDconnect Successful for [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(0);
}
void xtarget2NRDbuf(short fileno,short keyno,void *target1,int line,char *file)
{
	short x;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	int length=0,pos=0;
	char *target=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG target2NRDbuf For Module [%s] File [%d] [%s] Using Keyno [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	key=NRDbuf[fileno].key+(keyno-1);
	target=(char *)target1;
	pos=0;
	for(x=0;x<key->numparts;++x)
	{
		part=key->part+x;
		field=FLDNRD(fileno,part->name);
		if(field!=NULL)
		{
/* increment allocation space for target by field length */
/* write field data into target */
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT: /* variable length field type */
					length=RDAstrlen(&target[pos]);	
					if(RDAstrlen(&target[pos])>0)
					{
						QUICKALLOC(field->data.string_value,field->dlen,length+1);
						memset(field->data.string_value,0,field->dlen);
						memcpy(field->data.string_value,&target[pos],length+1);
						unpad(field->data.string_value);
					} else if(field->dlen!=0 && field->data.string_value!=NULL)
					{
						memset(field->data.string_value,0,field->dlen);
					}
					if(field->data.string_value!=NULL)
					{
						field->dlen=RDAstrlen(field->data.string_value)+1;
						if(field->dlen>field->len && field->len>0)
						{
							field->data.string_value[field->len]=0;
						}
					} else field->dlen=0;
					pos+=length;
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
					length=RDAstrlen(&target[pos]);	
					if(RDAstrlen(&target[pos])>0)
					{
						QUICKALLOC(field->data.string_value,field->dlen,length+1);
						memset(field->data.string_value,0,field->dlen);
						memcpy(field->data.string_value,&target[pos],length);
						unpad(field->data.string_value);
					} else if(field->dlen!=0 && field->data.string_value!=NULL)
					{
						memset(field->data.string_value,0,field->dlen);
					}
					if(field->data.string_value!=NULL)
					{
						field->dlen=RDAstrlen(field->data.string_value)+1;
						if(field->dlen>field->len && field->len>0)
						{
							field->data.string_value[field->len]=0;
						}
					} else field->dlen=0;
					pos+=length;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					memcpy(field->data.float_value,&target[pos],
						sizeof(double));
					pos+=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					memcpy(field->data.short_value,&target[pos],
						sizeof(short));
					pos+=sizeof(short);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					memcpy(field->data.integer_value,&target[pos],
						sizeof(int));
					pos+=sizeof(int);
					break;
				case BOOLNS:
				case CHARACTERS:
					*field->data.string_value=target[pos];
					pos+=1;
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Field [%s].",field->type,field->name);
					break;
			}
		}
	}
}
