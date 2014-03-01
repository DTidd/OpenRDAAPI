/*lint -library*/
/*lint -e525*/
#include <nrdp.hpp>
#include <mysql.h>
/*ADDLIB misc */

short main(int argc,char **argv)
{
	APPlib *args=NULL,*tables=NULL;
	FILE *fp=NULL;
	short err=-1;
	int x=0,y=0;
	MYSQL *mysql;
	MYSQL_ROW mysql_row;
	MYSQL_RES *mysql_result;
	char *name=NULL,*modname=NULL,*temp=NULL;

	if(argc<1)
	{
		prterr("Error:  No Database specified.");
		return(err);
	}
	args=APPlibNEW();
	addAPPlib(args,argv[1]);
	if(argc<2)
	{
		addAPPlib(args,"--datadir=.");
	} else {
		temp=Rmalloc(11+(RDAstrlen(argv[2]))+1);
		sprintf(temp,"--datadir=%s",argv[2]);
		addAPPlib(args,temp);
		if(temp!=NULL) Rfree(temp);
	}
	/*
	if(mysql_server_init(args->numlibs,args->libs,NULL))
	if(mysql_library_init(args->numlibs,args->libs,NULL))
	*/
	if(mysql_library_init(args->numlibs,args->libs,NULL))
	{
		fprintf(stderr,"Error:  Unable to start embedded mysql server.");
		return(err);
	}

	modname=Rmalloc(4+1);
	sprintf(modname,"test");
	name=Rmalloc(7+1);
	sprintf(name,"PERTEST");
	if(diagnrd) { fprintf(stderr,"DIAG Open file [%s] using MySQL Embedded Engine.",name); }
	mysql=mysql_init(NULL);
	if(!mysql)
	{
		fprintf(stderr,"Error:  Couldn't initialize MYSQL structure.");
	} else {
		/*
		mysql_options(mysql,MYSQL_READ_DEFAULT_GROUP,"XPERT");
		mysql_options(mysql,MYSQL_OPT_USE_EMBEDDED_CONNECTION,NULL);
		if(!mysql_real_connect(mysql,NULL,NULL,NULL,name,0,NULL,0))
		*/
		if(!mysql_real_connect(mysql,NULL,NULL,NULL,modname,0,NULL,0))
		{
			prterr("Error: mysql_real_connect failed for %s->%s with %s.",modname,name,mysql_error(mysql));
			return(err);
		}	
	}
	if(mysql!=NULL)
	{
		tables=APPlibNEW();
		mysql_result=mysql_list_tables(mysql,NULL);
		if(mysql_result!=NULL)
		{
			while((mysql_row=mysql_fetch_row(mysql_result)))
			{
				addAPPlib(tables,mysql_row[0]))
			}
			mysql_free_result(mysql_result);
		} else {
			fprintf(stderr,"Error: Unable to find table names in the database.");
		}
	}
	if(mysql!=NULL)
	{
		if(argc<3)
		{
			fp=fopen("dumpmysql.out","w");
		} else {
			fp=fopen(argv[3],"w");
		}
		if(fp!=NULL)
		{
			for(y=0;y<tables->numlibs;y++)
			{
				temp=Rmalloc(16+(strlen(tables->libs[y]))+1);
				sprintf(temp,"SELECT * FROM %s",tables->libs[y]);
				if(!mysql_query(mysql,temp))
				{
					mysql_result=mysql_use_result(mysql);
					if(mysql_result!=NULL)
					{
						while((mysql_row=mysql_fetch_row(mysql_result)))
						{
							for(x=0;x<mysql_num_fields(mysql_result);x++)
							{
								if(x<((mysql_num_fields(mysql_result))-1))
								{
									fprintf(fp,"%s\t",mysql_row[x]);
								} else {
									fprintf(fp,"%s\n",mysql_row[x]);
								}
							}
						}
						mysql_free_result(mysql_result);
					} else {
						mysql_free_result(mysql_result);
						fprintf(stderr,"Error: No results found in table %s.\n",tables->libs[y]);
					}
				}
				if(temp!=NULL) Rfree(temp);
			}
			fclose(fp);
		} else {
			fprintf(stderr,"Error:  Cannot open text file for writing.");
		}
	} else {
		fprintf(stderr,"I lost the connection!!");
	}
	mysql_close(mysql);
	mysql_library_end();
	freeapplib(args);
	freeapplib(tables);
	if(modname!=NULL) Rfree(modname);
	if(name!=NULL) Rfree(name);
	return (err);
}
/*lint +e525*/
