/* Import File Definitions from MYSQL Server */
#ifndef WIN32
#define __NAM__ "imysqlsvr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "imysqlsvr.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include<cstdlib>
#include<io.h>
#endif
#include<mknrd.hpp>
#if(defined(USE_MYSQL))
#include <mysql.h>
#endif 
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *DatabaseName=NULL,*ServerName=NULL;

static short ImportDatabase()
{
#if(defined(USE_MYSQL))
	int x=0,y=0,z=0;
	APPlib *tables=NULL;
	MYSQL *db=NULL;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	char *temp=NULL;
	NRDfield *field=NULL,*fields=NULL;
	NRDkey *key=NULL,*keys=NULL;
	NRDpart *part=NULL;
	short nofields=0,nokeys=0,noparts=0;

	if(diagnrd) { prterr("DIAG Open file [%s] using MySQL Server Engine.",DatabaseName); }
	db=mysql_init(NULL);
	if(!db)
	{
		prterr("Error:  Couldn't initialize MYSQL structure.");
		return(-1);
	} else {
		mysql_options(db,MYSQL_READ_DEFAULT_GROUP,"XPERT");
		if(!mysql_real_connect(db,ServerName,MySQL_Username,MySQL_Password,DatabaseName,0,NULL,CLIENT_MULTI_RESULTS))
		{
			prterr("Error: mysql_real_connect failed for %s with %s.",DatabaseName,mysql_error(db));
			mysql_close(db);
			db=NULL;
			return(-1);
		}	
	}
	if(db!=NULL)
	{
		tables=APPlibNEW();
		memset(stemp,0,101);
		result=mysql_list_tables(db,stemp);
		if(result!=NULL)
		{
			while((row=mysql_fetch_row(result)))
			{
				addAPPlib(tables,row[0]);
			}
		}
		mysql_free_result(result);
		result=NULL;
	}
	mysql_close(db);
	db=mysql_init(NULL);
	if(!db)
	{
		prterr("Error:  Couldn't initialize MYSQL structure.");
		return(-1);
	} else {
		mysql_options(db,MYSQL_READ_DEFAULT_GROUP,"XPERT");
		if(!mysql_real_connect(db,ServerName,MySQL_Username,MySQL_Password,"Information_schema",0,NULL,CLIENT_MULTI_RESULTS))
		{
			prterr("Error: mysql_real_connect failed for %s with %s.",DatabaseName,mysql_error(db));
			mysql_close(db);
			db=NULL;
			return(-1);
		}	
	}
	if(db!=NULL)
	{

	for(x=0;x<tables->numlibs;++x)
	{
		temp=Rmalloc(16+(RDAstrlen(tables->libs[x]))+1);
		sprintf(temp,"SELECT * FROM `COLUMNS` WHERE TABLE_SCHEMA=\"%s\" AND TABLE_NAME=\"%s\"",DatabaseName,tables->libs[x]);
		if(!mysql_query(db,temp))
		{
			result=mysql_store_result(db);
			if(result!=NULL)
			{
				nofields=mysql_num_rows(result);
				fields=Rmalloc(nofields*sizeof(NRDfield));
				while(row=mysql_fetch_row(result))
				{
					for(y=0;y<nofields;++y)
					{
						prterr("\nrow [%d][%s]",y,row[y]);
					}
					TRACE;
				}
				prterr("\n\n\n");
			}
		}
		if(temp!=NULL) Rfree(temp);
	}
#endif 
	return(0);
}
static void doexit(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(DatabaseName!=NULL) Rfree(DatabaseName);
	if(ServerName!=NULL) Rfree(ServerName);
	ShutdownSubsystems();
	std::exit;
}
static void doselect(RDArsrc *mainrsrc)
{
	readwidget(mainrsrc,"DATABASE");
	FINDRSCGETSTRING(mainrsrc,"DATABASE",&DatabaseName);
	readwidget(mainrsrc,"SERVER NAME");
	FINDRSCGETSTRING(mainrsrc,"SERVER NAME",&ServerName);
	if(isEMPTY(DatabaseName)) return;
	ImportDatabase();
	doexit(mainrsrc);
}
static void checkname(RDArsrc *mainrsrc)
{
	readwidget(mainrsrc,"DATABASE");
	FINDRSCGETSTRING(mainrsrc,"DATABASE",&DatabaseName);
	if(isEMPTY(DatabaseName)) 
	{
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT",FALSE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT",TRUE);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,"DATABASE","IMPORT MYSQL SERVER DATABASE")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW("DATABASE","IMPORT MYSQL SERVER DATABASE");
	addstdrsrc(mainrsrc,"SERVER NAME",VARIABLETEXT,80,&ServerName,TRUE);
	addstdrsrc(mainrsrc,"DATABASE",VARIABLETEXT,80,&DatabaseName,TRUE);
	FINDRSCSETFUNC(mainrsrc,"DATABASE",checkname,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,doselect,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	checkname(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
