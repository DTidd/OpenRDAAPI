/*lint -library*/
/*lint -e525*/
#include <nrdp.hpp>

#ifdef USE_MYSQL
#include <mysql.h>
#endif


short download_blob_data(char *db_module,char *db_file,char *field_list,char *key_values,char *encoding_type,char *filename)
{
#ifdef USE_MYSQL
	APPlib *sql_fields=NULL,*sql_values=NULL;
	FILE *fp=NULL;
	short err=-1;
	int x=0,y=0,z=0;
	MYSQL *mysql;
	MYSQL_ROW mysqlRow;
	MYSQL_RES *mysqlResult;
	char *temp=NULL,*mytemp;

	unsigned long *lengths;
	unsigned int num_fields;

	unsigned long long rows_returned=0;

	unsigned int db_module_length=0;
	unsigned int db_file_length=0;
	unsigned int field_list_length=0;
	unsigned int key_values_length=0;

	if(db_module==NULL)
	{
		prterr("Error:  db_module is NULL.\n");
		return(-1);
	}
	
	if(db_file==NULL)
	{
		prterr("Error:  db_file is NULL.\n");
		return(-1);
	}
	
	if(field_list==NULL)
	{
		prterr("Error:  field_list is NULL.\n");
		return(-1);
	}
	
	if(key_values==NULL)
	{
		prterr("Error:  key_values is NULL.\n");
		return(-1);
	}

	if(encoding_type==NULL)
	{
		prterr("Error:  encoding_type is NULL.\n");
		return(-1);
	}

	if(filename==NULL)
	{
		prterr("Error:  filename is NULL.\n");
		return(-1);
	}


	db_module_length=strlen(db_module);
	if(db_module_length==0)
	{
		prterr("Error:  db_module is empty.\n");
		return(-1);
	}
	
	db_file_length=strlen(db_file);
	if(db_file_length==0)
	{
		prterr("Error:  db_file is empty.\n");
		return(-1);
	}
	
	field_list_length=strlen(field_list);
	if(field_list_length==0)
	{
		prterr("Error:  field_list is empty.\n");
		return(-1);
	}
	
	key_values_length=strlen(key_values);
	if(key_values_length==0)
	{
		prterr("Error:  key_values is empty.\n");
		return(-1);
	}

	if((strlen(encoding_type))==0)
	{
		prterr("Error:  encoding_type is empty.\n");
		return(-1);
	}

	if((strlen(filename))==0)
	{
		prterr("Error:  filename is empty.\n");
		return(-1);
	}

	mysql=mysql_init(NULL);
	if(!mysql)
	{
		prterr("Error:  Couldn't initialize MYSQL structure.");
	} else {
#ifdef WIN32			
			mysql_options(mysql,MYSQL_READ_DEFAULT_FILE,"rda\\my.cnf");
#else
			mysql_options(mysql,MYSQL_READ_DEFAULT_FILE,"rda/my.cnf");
#endif
			mysql_options(mysql,MYSQL_READ_DEFAULT_GROUP,db_module);
	
		if(!mysql_real_connect(mysql,NULL,NULL,NULL,db_module,0,NULL,0))
		{
			prterr("Error: mysql_real_connect failed for %s->%s with %s.",db_module,db_file,mysql_error(mysql));
			return(err);
		}	
	}
	if(mysql!=NULL)
	{
		mytemp=Rmalloc(field_list_length+key_values_length);
		memset(mytemp,0,(field_list_length+key_values_length));
		sql_fields=APPlibNEW();
		for(x=0;x<field_list_length;x++)
		{
			if((field_list[x]=='|')&&(y==0))
			{
				y=1;
			} else if((field_list[x]=='|')&&(y==1)) {
				strncpy(mytemp,field_list+z,(x-(z+1)));
				addAPPlib(sql_fields,mytemp);
				y=0;
				z=(x+1);
				memset(mytemp,0,(field_list_length+key_values_length));

			} else if(x==(field_list_length-1)) {
				strncpy(mytemp,field_list+z,field_list_length);
				addAPPlib(sql_fields,mytemp);
				memset(mytemp,0,(field_list_length+key_values_length));
			}
		}

#ifdef USE_RDA_DIAGNOSTICS
		if((diagmix)||(diagnrd)||(diagmisc))
		{
			SEEAPPlib(sql_fields);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		sql_values=APPlibNEW();
		z=0;
		for(x=0;x<key_values_length;x++)
		{
			if((key_values[x]=='|')&&(y==0))
			{
				y=1;
			} else if((key_values[x]=='|')&&(y==1)) {
				strncpy(mytemp,key_values+z,(x-(z+1)));
				addAPPlib(sql_values,mytemp);
				y=0;
				z=(x+1);
				memset(mytemp,0,(field_list_length+key_values_length));
				} else if(x==(key_values_length-1)) {
				strncpy(mytemp,key_values+z,key_values_length);
				addAPPlib(sql_values,mytemp);
				memset(mytemp,0,(field_list_length+key_values_length));
			}
		}
		if(mytemp!=NULL) Rfree(mytemp);

#ifdef USE_RDA_DIAGNOSTICS
		if((diagmix)||(diagnrd)||(diagmisc))
		{
			SEEAPPlib(sql_values);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

		temp=Rmalloc((db_module_length+db_file_length+field_list_length+key_values_length)*2+520);
		memset(temp,0,((db_module_length+db_file_length+field_list_length+key_values_length)*2+520));
		sprintf(temp,"SELECT ");
		for(x=0;x<sql_fields->numlibs;x++) 
		{
			if(x==(sql_fields->numlibs-1))
			{
				sprintf(temp,"%s%s FROM %s ",temp,sql_fields->libs[x],db_file);
			}else{
					sprintf(temp,"%s%s,",temp,sql_fields->libs[x]);
			}
		}
		
		for(x=0;x<sql_values->numlibs;x++) 
		{
			if(x==0)
			{
				sprintf(temp,"%sWHERE (%s = \"%s\" ",temp,sql_fields->libs[x],sql_values->libs[x]);
			}else if(strlen(sql_values->libs[x])!=0) {
				sprintf(temp,"%sAND %s = \"%s\" ",temp,sql_fields->libs[x],sql_values->libs[x]);
			}
		}
		sprintf(temp,"%s)",temp);
		

		if(!mysql_real_query(mysql,temp,RDAstrlen(temp)))
		{
			mysqlResult=mysql_store_result(mysql);
			if(mysqlResult!=NULL)
			{
				rows_returned=mysql_num_rows(mysqlResult);
				if(rows_returned>1) 
				{
					prterr("Error:  More than one row returned.  Returned [%d] rows.  Please check your key information to insure only one row is downloaded.\n",rows_returned);
				}else{
					while((mysqlRow=mysql_fetch_row(mysqlResult)))
					{
						num_fields=mysql_num_fields(mysqlResult);
						lengths = mysql_fetch_lengths(mysqlResult);
	
						/*
						printf("num_fields = [%d]  x = [%d]\n",num_fields,x);
						printf("FILE SIZE = %d\n", lengths[(num_fields-1)]);
						printf("FILE DATA: [\n%s\n",mysqlRow[(num_fields-1)]);
						*/
#ifdef WIN32
						if((stricmp(encoding_type,"RAW"))==0) 
#else
						if((strcasecmp(encoding_type,"RAW"))==0) 
#endif
						{
							fp=fopen(filename,"w");
							if(fp!=NULL)
							{
								fwrite(mysqlRow[(num_fields-1)],sizeof(char),(lengths[(num_fields-1)]),fp); 
							} else {
								prterr("Error:  Cannot open file [%s] for writing.",filename);
							}
							if(fp!=NULL) fclose(fp); 
#ifdef WIN32
						}else if((stricmp(encoding_type,"BASE64_DECODE"))==0) {
#else
						}else if((strcasecmp(encoding_type,"BASE64_DECODE"))==0) {
#endif
							decode_base64_to_file(filename,mysqlRow[(num_fields-1)]);
						}else{
							fp=fopen(filename,"w");
							if(fp!=NULL)
							{
								fwrite(mysqlRow[(num_fields-1)],sizeof(char),(lengths[(num_fields-1)]),fp); 
							} else {
								prterr("Error:  Cannot open file [%s] for writing.",filename);
							}
							if(fp!=NULL) fclose(fp); 
						}

					}
				}
				mysql_free_result(mysqlResult);
			} else {
				mysql_free_result(mysqlResult);
				prterr("Error: No results found in table %s.\n",db_file);
			}
		}else {
			prterr("Error:  Query Failed.\nQuery:  [%s].\n",temp);
		}

		if(temp!=NULL) Rfree(temp);
			
	} else {
		prterr("Error:  MySQL Server Connection Lost!");
	}
	mysql_close(mysql);
	mysql_library_end();
	freeapplib(sql_fields);
	freeapplib(sql_values);
	return (err);
#else
	return(-1);
#endif
}

/*lint +e525*/
