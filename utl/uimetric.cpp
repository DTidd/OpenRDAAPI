/* UIMetric - Used to answer the question, "How is the software being used?" */
#ifndef __NAM__
#ifndef WIN32
#define __NAM__ "uimetric.lnx"
#endif
#ifdef WIN32
#define __NAM__ "uimetric.exe"
#endif
#endif

#include <app.hpp>
#include <mix.hpp>
#include <sec.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *DropList[6]={"Query","Reports","Maintenance","Setup","Other","Task Bar"};
char sum_all_users=FALSE;

struct UIProcesss
{
	int OPTION;
	char *NAME;
	int DROP;
	int COUNT;
};
typedef struct UIProcesss UIProcess;
struct UIModules
{
	char *MODULE;
	int numUIP;
	UIProcess *UIP;
	int GROUP;
	int QUERY;
	int RPTGEN;
	int MAINTENANCE;
	int SETUP;
	int OTHER;
	int DOCKWINDOW;
	APPlib *plist;
};
typedef struct UIModules UIModule;

struct UIMetrics
{
	char *USER;
	char *USERNAME;
	int numUIM;
	UIModule *UIM;
	int QUERY;
	int RPTGEN;
	int MAINTENANCE;
	int SETUP;
	int OTHER;
	int DOCKWINDOW;
};
typedef struct UIMetrics UIMetric;

int numMetric=0;
UIMetric *myMetric=NULL;

void FreeUIMetric()
{
	UIMetric *tmp=NULL;
	UIModule *m=NULL;
	UIProcess *p=NULL;
	int x=0,y=0,z=0;

	if(myMetric!=NULL)
	{
		for(x=0,tmp=myMetric;x<numMetric;++x,++tmp)
		{
			if(tmp->UIM!=NULL)
			{
				for(y=0,m=tmp->UIM;y<tmp->numUIM;++y,++m)
				{
					if(m->UIP!=NULL)
					{
						for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
						{
							if(p->NAME!=NULL) Rfree(p->NAME);
						}
						Rfree(m->UIP);
					}
					if(m->MODULE!=NULL) Rfree(m->MODULE);
					if(m->plist!=NULL) freeapplib(m->plist);
				}
				Rfree(tmp->UIM);
			}
			if(tmp->USER!=NULL) Rfree(tmp->USER);
			if(tmp->USERNAME!=NULL) Rfree(tmp->USERNAME);
		}
		Rfree(myMetric);
	}
	myMetric=NULL;
	numMetric=0;
}
void addUIMetric(char *user,char *uname,char *module,int drop,int option,char *name)
{
	int x=0,y=0,z=0;
	UIMetric *tmp=NULL;
	UIModule *m=NULL;
	UIProcess *p=NULL;

	if(myMetric!=NULL)
	{
		for(x=0,tmp=myMetric;x<numMetric;++x,++tmp)
		{
			if(!RDAstrcmp(tmp->USER,user))
			{
				break;
			}
		}
		if(x>=numMetric)
		{
			myMetric=Rrealloc(myMetric,(numMetric+1)*sizeof(UIMetric));
			tmp=myMetric+numMetric;
			tmp->USER=stralloc(user);
			tmp->USERNAME=stralloc(uname);
			tmp->numUIM=0;
			tmp->UIM=NULL;
			tmp->QUERY=0;
			tmp->RPTGEN=0;
			tmp->MAINTENANCE=0;
			tmp->SETUP=0;
			tmp->OTHER=0;
			tmp->DOCKWINDOW=0;
			++numMetric;
			switch(drop)
			{
				case 0: /* QUERY */
					++tmp->QUERY;
					break;
				case 1: /* RPTGEN */
					++tmp->RPTGEN;
					break;
				case 2: /* MAINTENANCE */
					++tmp->MAINTENANCE;
					break;
				case 3: /* SETUP */
					++tmp->SETUP;
					break;
				case 4: /* OTHER */
					++tmp->OTHER;
					break;
				default:
				case 5: /* DOCKWINDOW */
					++tmp->DOCKWINDOW;
					break;
			}
			tmp->UIM=Rmalloc(sizeof(UIModule));
			tmp->numUIM=1;
			m=tmp->UIM;
			m->MODULE=stralloc(module);
			m->plist=NULL;
			m->numUIP=0;
			m->UIP=NULL;
			m->QUERY=0;
			m->RPTGEN=0;
			m->MAINTENANCE=0;
			m->SETUP=0;
			m->OTHER=0;
			m->DOCKWINDOW=0;
			switch(drop)
			{
				case 0: /* QUERY */
					++m->QUERY;
					break;
				case 1: /* RPTGEN */
					++m->RPTGEN;
					break;
				case 2: /* MAINTENANCE */
					++m->MAINTENANCE;
					break;
				case 3: /* SETUP */
					++m->SETUP;
					break;
				case 4: /* OTHER */
					++m->OTHER;
					break;
				default:
				case 5: /* DOCKWINDOW */
					++m->DOCKWINDOW;
					break;
			}
			if(m->UIP!=NULL)
			{
				for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
				{
					if(p->OPTION==option)
					{	
						break;
					}
				}
				if(z>=m->numUIP)
				{
					m->UIP=Rrealloc(m->UIP,(m->numUIP+1)*sizeof(UIProcess));
					p=m->UIP+m->numUIP;
					++m->numUIP;
					p->OPTION=option;
					p->NAME=stralloc(name);
					p->DROP=drop;
					p->COUNT=1;
				} else {
					++p->COUNT;		
				}
			} else {
				m->UIP=Rmalloc(sizeof(UIProcess));
				p=m->UIP;
				++m->numUIP;
				p->OPTION=option;
				p->NAME=stralloc(name);
				p->DROP=drop;
				p->COUNT=1;
			}
		} else {
			switch(drop)
			{
				case 0: /* QUERY */
					++tmp->QUERY;
					break;
				case 1: /* RPTGEN */
					++tmp->RPTGEN;
					break;
				case 2: /* MAINTENANCE */
					++tmp->MAINTENANCE;
					break;
				case 3: /* SETUP */
					++tmp->SETUP;
					break;
				case 4: /* OTHER */
					++tmp->OTHER;
					break;
				default:
				case 5: /* DOCKWINDOW */
					++tmp->DOCKWINDOW;
					break;
			}
			if(tmp->UIM!=NULL)
			{
				for(y=0,m=tmp->UIM;y<tmp->numUIM;++y,++m)
				{
					if(!RDAstrcmp(m->MODULE,module))
					{
						break;
					}
				}
				if(y>=tmp->numUIM)
				{
					tmp->UIM=Rrealloc(tmp->UIM,(tmp->numUIM+1)*sizeof(UIModule));
					m=tmp->UIM+tmp->numUIM;
					++tmp->numUIM;
					m->MODULE=stralloc(module);
					m->plist=NULL;
					m->numUIP=0;
					m->UIP=NULL;
					m->QUERY=0;
					m->RPTGEN=0;
					m->MAINTENANCE=0;
					m->SETUP=0;
					m->OTHER=0;
					m->DOCKWINDOW=0;
					switch(drop)
					{
						case 0: /* QUERY */
							++m->QUERY;
							break;
						case 1: /* RPTGEN */
							++m->RPTGEN;
							break;
						case 2: /* MAINTENANCE */
							++m->MAINTENANCE;
							break;
						case 3: /* SETUP */
							++m->SETUP;
							break;
						case 4: /* OTHER */
							++m->OTHER;
							break;
						default:
						case 5: /* DOCKWINDOW */
							++m->DOCKWINDOW;
							break;
					}
					if(m->UIP!=NULL)
					{
						for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
						{
							if(p->OPTION==option)
							{	
								break;
							}
						}
						if(z>=m->numUIP)
						{
							m->UIP=Rrealloc(m->UIP,(m->numUIP+1)*sizeof(UIProcess));
							p=m->UIP+m->numUIP;
							++m->numUIP;
							p->OPTION=option;
							p->NAME=stralloc(name);
							p->DROP=drop;
							p->COUNT=1;
						} else {
							++p->COUNT;		
						}
					} else {
						m->UIP=Rmalloc(sizeof(UIProcess));
						p=m->UIP;
						m->numUIP=1;
						p->OPTION=option;
						p->NAME=stralloc(name);
						p->DROP=drop;
						p->COUNT=1;
					}
				} else {
					switch(drop)
					{
						case 0: /* QUERY */
							++m->QUERY;
							break;
						case 1: /* RPTGEN */
							++m->RPTGEN;
							break;
						case 2: /* MAINTENANCE */
							++m->MAINTENANCE;
							break;
						case 3: /* SETUP */
							++m->SETUP;
							break;
						case 4: /* OTHER */
							++m->OTHER;
							break;
						default:
						case 5: /* DOCKWINDOW */
							++m->DOCKWINDOW;
							break;
					}
					if(m->UIP!=NULL)
					{
						for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
						{
							if(p->OPTION==option)
							{	
								break;
							}
						}
						if(z>=m->numUIP)
						{
							m->UIP=Rrealloc(m->UIP,(m->numUIP+1)*sizeof(UIProcess));
							p=m->UIP+m->numUIP;
							++m->numUIP;
							p->OPTION=option;
							p->NAME=stralloc(name);
							p->DROP=drop;
							p->COUNT=1;
						} else {
							++p->COUNT;		
						}
					} else {
						m->UIP=Rmalloc(sizeof(UIProcess));
						p=m->UIP;
						++m->numUIP;
						p->OPTION=option;
						p->NAME=stralloc(name);
						p->DROP=drop;
						p->COUNT=1;
					}
				}
			} else {
				tmp->UIM=Rmalloc(sizeof(UIModule));
				tmp->numUIM=1;
				m=tmp->UIM;
				m->MODULE=stralloc(module);
				m->plist=NULL;
				m->numUIP=0;
				m->UIP=NULL;
				m->QUERY=0;
				m->RPTGEN=0;
				m->MAINTENANCE=0;
				m->SETUP=0;
				m->OTHER=0;
				m->DOCKWINDOW=0;
				switch(drop)
				{
					case 0: /* QUERY */
						++m->QUERY;
						break;
					case 1: /* RPTGEN */
						++m->RPTGEN;
						break;
					case 2: /* MAINTENANCE */
						++m->MAINTENANCE;
						break;
					case 3: /* SETUP */
						++m->SETUP;
						break;
					case 4: /* OTHER */
						++m->OTHER;
						break;
					default:
					case 5: /* DOCKWINDOW */
						++m->DOCKWINDOW;
						break;
				}
				if(m->UIP!=NULL)
				{
					for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
					{
						if(p->OPTION==option)
						{	
							break;
						}
					}
					if(z>=m->numUIP)
					{
						m->UIP=Rrealloc(m->UIP,(m->numUIP+1)*sizeof(UIProcess));
						p=m->UIP+m->numUIP;
						++m->numUIP;
						p->OPTION=option;
						p->NAME=stralloc(name);
						p->DROP=drop;
						p->COUNT=1;
					} else {
						++p->COUNT;		
					}
				} else {
					m->UIP=Rmalloc(sizeof(UIProcess));
					p=m->UIP;
					m->numUIP=1;
					p->OPTION=option;
					p->NAME=stralloc(name);
					p->DROP=drop;
					p->COUNT=1;
				}
			}
		}
	} else {
		myMetric=Rmalloc(sizeof(UIMetric));
		tmp=myMetric;
		tmp->USER=stralloc(user);
		tmp->USERNAME=stralloc(uname);
		tmp->numUIM=0;
		tmp->UIM=NULL;
		tmp->QUERY=0;
		tmp->RPTGEN=0;
		tmp->MAINTENANCE=0;
		tmp->SETUP=0;
		tmp->OTHER=0;
		tmp->DOCKWINDOW=0;
		switch(drop)
		{
			case 0: /* QUERY */
				++tmp->QUERY;
				break;
			case 1: /* RPTGEN */
				++tmp->RPTGEN;
				break;
			case 2: /* MAINTENANCE */
				++tmp->MAINTENANCE;
				break;
			case 3: /* SETUP */
				++tmp->SETUP;
				break;
			case 4: /* OTHER */
				++tmp->OTHER;
				break;
			default:
			case 5: /* DOCKWINDOW */
				++tmp->DOCKWINDOW;
				break;
		}
		if(tmp->UIM!=NULL)
		{
			for(y=0,m=tmp->UIM;y<tmp->numUIM;++y,++m)
			{
				if(!RDAstrcmp(m->MODULE,module))
				{
					break;
				}
			}
			if(y>=tmp->numUIM)
			{
				tmp->UIM=Rrealloc(tmp->UIM,(tmp->numUIM+1)*sizeof(UIModule));
				m=tmp->UIM+tmp->numUIM;
				++tmp->numUIM;
				m->plist=NULL;
				m->numUIP=0;
				m->UIP=NULL;
				m->QUERY=0;
				m->RPTGEN=0;
				m->MAINTENANCE=0;
				m->SETUP=0;
				m->OTHER=0;
				m->DOCKWINDOW=0;
				switch(drop)
				{
					case 0: /* QUERY */
						++m->QUERY;
						break;
					case 1: /* RPTGEN */
						++m->RPTGEN;
						break;
					case 2: /* MAINTENANCE */
						++m->MAINTENANCE;
						break;
					case 3: /* SETUP */
						++m->SETUP;
						break;
					case 4: /* OTHER */
						++m->OTHER;
						break;
					default:
					case 5: /* DOCKWINDOW */
						++m->DOCKWINDOW;
						break;
				}
				if(m->UIP!=NULL)
				{
					for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
					{
						if(p->OPTION==option)
						{	
							break;
						}
					}
					if(z>=m->numUIP)
					{
						m->UIP=Rrealloc(m->UIP,(m->numUIP+1)*sizeof(UIProcess));
						p=m->UIP+m->numUIP;
						++m->numUIP;
						p->OPTION=option;
						p->NAME=stralloc(name);
						p->DROP=drop;
						p->COUNT=1;
					} else {
						++p->COUNT;		
					}
				} else {
					m->UIP=Rmalloc(sizeof(UIProcess));
					p=m->UIP;
					++m->numUIP;
					p->OPTION=option;
					p->NAME=stralloc(name);
					p->DROP=drop;
					p->COUNT=1;
				}
			} else {
				switch(drop)
				{
					case 0: /* QUERY */
						++m->QUERY;
						break;
					case 1: /* RPTGEN */
						++m->RPTGEN;
						break;
					case 2: /* MAINTENANCE */
						++m->MAINTENANCE;
						break;
					case 3: /* SETUP */
						++m->SETUP;
						break;
					case 4: /* OTHER */
						++m->OTHER;
						break;
					default:
					case 5: /* DOCKWINDOW */
						++m->DOCKWINDOW;
						break;
				}
				if(m->UIP!=NULL)
				{
					for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
					{
						if(p->OPTION==option)
						{	
							break;
						}
					}
					if(z>=m->numUIP)
					{
						m->UIP=Rrealloc(m->UIP,(m->numUIP+1)*sizeof(UIProcess));
						p=m->UIP+m->numUIP;
						++m->numUIP;
						p->OPTION=option;
						p->NAME=stralloc(name);
						p->DROP=drop;
						p->COUNT=1;
					} else {
						++p->COUNT;		
					}
				} else {
					m->UIP=Rmalloc(sizeof(UIProcess));
					p=m->UIP;
					m->numUIP=1;
					p->OPTION=option;
					p->NAME=stralloc(name);
					p->DROP=drop;
					p->COUNT=1;
				}
			}
		} else {
			tmp->UIM=Rmalloc(sizeof(UIModule));
			tmp->numUIM=1;
			m=tmp->UIM;
			m->MODULE=stralloc(module);
			m->plist=NULL;
			switch(drop)
			{
				case 0: /* QUERY */
					++m->QUERY;
					break;
				case 1: /* RPTGEN */
					++m->RPTGEN;
					break;
				case 2: /* MAINTENANCE */
					++m->MAINTENANCE;
					break;
				case 3: /* SETUP */
					++m->SETUP;
					break;
				case 4: /* OTHER */
					++m->OTHER;
					break;
				default:
				case 5: /* DOCKWINDOW */
					++m->DOCKWINDOW;
					break;
			}
			if(m->UIP!=NULL)
			{
				for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
				{
					if(p->OPTION==option)
					{	
						break;
					}
				}
				if(z>=m->numUIP)
				{
					m->UIP=Rrealloc(m->UIP,(m->numUIP+1)*sizeof(UIProcess));
					p=m->UIP+m->numUIP;
					++m->numUIP;
					p->OPTION=option;
					p->NAME=stralloc(name);
					p->DROP=drop;
					p->COUNT=1;
				} else {
					++p->COUNT;		
				}
			} else {
				m->UIP=Rmalloc(sizeof(UIProcess));
				p=m->UIP;
				m->numUIP=1;
				p->OPTION=option;
				p->NAME=stralloc(name);
				p->DROP=drop;
				p->COUNT=1;
			}
		}
		numMetric=1;
	}
}
void quituimetric(RDArsrc *r)
{
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(myMetric!=NULL) FreeUIMetric();
	myMetric=NULL;
	numMetric=0;
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short ef=FALSE,drop=0;
	int len=0,option=0,l=0;
	char *user=NULL,*filename=NULL,delflag=FALSE,buffer[512];
	char *temp=NULL,*temp1=NULL,module[16],*uname=NULL;
	FILE *fp=NULL;
	RDArsrc *mainrsrc=NULL;
	UIMetric *tmp=NULL;
	UIModule *m=NULL;
	UIProcess *p=NULL;
	int x=0,y=0,z=0;
	int Q=0,R=0,M=0,S=0,O=0,D=0;
	short MENUITEM_FILENO=(-1),USERSFILENO=(-1);

	if(InitializeSubsystems(argc,argv,"UTILITIES","USER INTERFACE METRICS")) 
	{
		ShutdownSubsystems();
        	return(1);
	}
	if(argc<2)
	{
		ShutdownSubsystems();
        	return(1);
	}
	if(argc>2)
	{
		if(!RDAstrcmp(argv[2],"SUM_ALL_USERS"))
		{
			sum_all_users=TRUE;
			user=NULL;
		} else user=stralloc(argv[2]);
	}
	MENUITEM_FILENO=OPNNRD("UTILITIES","MENUITEM");
	USERSFILENO=OPNNRD("SECURITY","USERS");
	if(MENUITEM_FILENO==(-1))
	{
		prterr("Error:  Cannot open UTILITIES -> MENUITEM.");
		exit(1);
	}
	myMetric=NULL;
	numMetric=0;
	if(USERSFILENO!=(-1))
	{
		ZERNRD(USERSFILENO);
		if(!isEMPTY(user) && sum_all_users==FALSE)
		{
			FINDFLDSETSTRING(USERSFILENO,"USER IDENTIFICATION",user);
			if(!EQLNRD(USERSFILENO,1))
			{
				filename=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(user)+RDAstrlen(argv[1])+20);
				sprintf(filename,"%s/%s/%s.csv",CURRENTDIRECTORY,(user!=NULL ? user:""),argv[1]);
#ifndef WIN32
				fp=fopen(filename,"r+b");
#else
				fp=fopen(filename,"rbc");
#endif
				if(fp!=NULL)
				{
					while((len=readline(fp,buffer,512))!=(-1))
					{
						option=0;
						temp=stralloc(buffer);
						for(temp1=temp;*temp1;++temp1) 
						{
							if(*temp1==',') 
							{
								*temp1=0;
								--temp1;
								*temp1=0;
								break;
							}
						}
						temp[15]=0;
						memset(module,0,16);
						sprintf(module,"%s",&temp[1]);
						temp1=RDAstrstr(buffer,",");
						if(temp1!=NULL)
						{
							++temp1;
							option=atoi(temp1);
						}									
						ZERNRD(MENUITEM_FILENO);
						FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",module);
						FINDFLDSETINT(MENUITEM_FILENO,"ITEM NUMBER",option);
						if(!EQLNRD(MENUITEM_FILENO,1))
						{
							FINDFLDGETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",&drop);
							FINDFLDGETSTRING(MENUITEM_FILENO,"NAME",&temp);
						}
						FINDFLDGETSTRING(USERSFILENO,"USER NAME",&uname);
						addUIMetric(user,uname,module,drop,option,temp);
						if(temp!=NULL) Rfree(temp);
						if(uname!=NULL) Rfree(uname);
					}
					fclose(fp);
				}
			}
		} else {
			ef=FRSNRD(USERSFILENO,1);
			while(!ef)
			{
				FINDFLDGETCHAR(USERSFILENO,"DELETEFLAG",&delflag);
				if(!delflag)
				{
					FINDFLDGETSTRING(USERSFILENO,"USER IDENTIFICATION",&user);
					if(!isEMPTY(user))
					{
						filename=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(user)+RDAstrlen(argv[1])+17);
						sprintf(filename,"%s/%s/%s.csv",CURRENTDIRECTORY,(user!=NULL ? user:""),argv[1]);
#ifndef WIN32
						fp=fopen(filename,"r+b");
#else
						fp=fopen(filename,"rbc");
#endif
						if(fp!=NULL)
						{
							while((len=readline(fp,buffer,512))!=(-1))
							{
								option=0;
								temp=stralloc(buffer);
								for(temp1=temp;*temp1;++temp1) 
								{
									if(*temp1==',') 
									{
										*temp1=0;
										--temp1;
										*temp1=0;
										break;
									}
								}
								if(RDAstrlen(temp)>15) temp[15]=0;
								memset(module,0,16);
								sprintf(module,"%s",&temp[1]);
								temp1=RDAstrstr(buffer,",");
								if(temp1!=NULL)
								{
									++temp1;
									option=atoi(temp1);
								}									
								ZERNRD(MENUITEM_FILENO);
								FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",module);
								FINDFLDSETINT(MENUITEM_FILENO,"ITEM NUMBER",option);
								if(!EQLNRD(MENUITEM_FILENO,1))
								{
									FINDFLDGETSHORT(MENUITEM_FILENO,"DROP DOWN LIST",&drop);
									FINDFLDGETSTRING(MENUITEM_FILENO,"NAME",&temp);
								}
								if(sum_all_users==FALSE)
								{
									FINDFLDGETSTRING(USERSFILENO,"USER NAME",&uname);
									addUIMetric(user,uname,module,drop,option,temp);
									if(uname!=NULL) Rfree(uname);
								} else {
									addUIMetric("ALL","SUMMARY OF ALL USERS",module,drop,option,temp);
								}
								if(temp!=NULL) Rfree(temp);
							}
							fclose(fp);
						}
					}
				}
				ef=NXTNRD(USERSFILENO,1);
			}
		}
	}
	CLSNRD(MENUITEM_FILENO);
	CLSNRD(USERSFILENO);
	if(myMetric==NULL)
	{
		ERRORDIALOG("USER INTERFACE METRIC FAILED","The User Interface Metric failed to mine data, and aborted.",NULL,TRUE);
	} else {
		mainrsrc=RDArsrcNEW("UTILITIES","USER INTERFACE METRICS");
		mainrsrc->scn=RDAscrnNEW("UTILITIES","USER INTERFACE METRICS");
		ADVaddwdgt(mainrsrc->scn,12,"","","","",600,1260,0,"","","","");

		if(myMetric!=NULL)
		{
			for(x=0,tmp=myMetric;x<numMetric;++x,++tmp)
			{
				Q+=tmp->QUERY;
				R+=tmp->RPTGEN;
				M+=tmp->MAINTENANCE;
				S+=tmp->SETUP;
				O+=tmp->OTHER;
				D+=tmp->DOCKWINDOW;
				ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,"","User Identification:","","",0,0,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"USER IDENTIFICATION %d",(x+1));
				addstdrsrc(mainrsrc,stemp,1,15,tmp->USER,FALSE);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,15,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"USER NAME %d",(x+1));
				addstdrsrc(mainrsrc,stemp,1,40,tmp->USERNAME,FALSE);
				ADVaddwdgt(mainrsrc->scn,5,""," User Name:","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,40,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,"","              QUERY:","","",0,0,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"QUERY %d",(x+1));
				addstdrsrc(mainrsrc,stemp,8,8,&tmp->QUERY,FALSE);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,"","   RPTGEN:","","",0,0,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"RPTGEN %d",(x+1));
				addstdrsrc(mainrsrc,stemp,8,8,&tmp->RPTGEN,FALSE);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,""," MAINTENANCE:","","",0,0,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"MAINTENANCE %d",(x+1));
				addstdrsrc(mainrsrc,stemp,8,8,&tmp->MAINTENANCE,FALSE);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,""," SETUP:","","",0,0,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"SETUP %d",(x+1));
				addstdrsrc(mainrsrc,stemp,8,8,&tmp->SETUP,FALSE);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,""," OTHER:","","",0,0,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"OTHER %d",(x+1));
				addstdrsrc(mainrsrc,stemp,8,8,&tmp->OTHER,FALSE);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,""," TASK BAR:","","",0,0,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"DOCKWINDOW %d",(x+1));
				addstdrsrc(mainrsrc,stemp,8,8,&tmp->DOCKWINDOW,FALSE);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
				if(tmp->UIM!=NULL)
				{
					ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
					ADVaddwdgt(mainrsrc->scn,5,"","          # Modules:","","",0,0,0,"","","","");
					memset(stemp,0,101);
					sprintf(stemp,"MODULES %d",(x+1));
					addstdrsrc(mainrsrc,stemp,8,8,&tmp->numUIM,FALSE);
					ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,2,0,"","","","");
					ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
					for(y=0,m=tmp->UIM;y<tmp->numUIM;++y,++m)
					{
						ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,"","             MODULE:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,1,15,m->MODULE,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,15,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,"","Processes:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s PROCESSES %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,8,8,&m->numUIP,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,"","              QUERY:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s QUERY %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,8,8,&m->QUERY,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,"","   RPTGEN:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s RPTGEN %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,8,8,&m->RPTGEN,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,""," MAINTENANCE:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s MAINTENANCE %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,8,8,&m->MAINTENANCE,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,""," SETUP:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s SETUP %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,8,8,&m->SETUP,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,""," OTHER:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s OTHER %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,8,8,&m->OTHER,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,""," TASK BAR:","","",0,0,0,"","","","");
						memset(stemp,0,101);
						sprintf(stemp,"%s DOCKWINDOW %d",m->MODULE,(y+1));
						addstdrsrc(mainrsrc,stemp,8,8,&m->DOCKWINDOW,FALSE);
						ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
						if(m->UIP!=NULL)
						{
							m->plist=APPlibNEW();
							for(z=0,p=m->UIP;z<m->numUIP;++z,++p)
							{
								memset(stemp,0,101);
								sprintf(stemp,"%12s %-60s %08d %08d",DropList[p->DROP],(p->NAME!=NULL ? p->NAME:""),p->OPTION,p->COUNT);
								addAPPlib(m->plist,stemp);
							}
							SORTAPPlib(m->plist);
							ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
							memset(stemp,0,101);
							sprintf(stemp," %12s %-60s %8s %8s","Process Area","Process Name","Item #","Count");
							ADVaddwdgt(mainrsrc->scn,5,"",stemp,"","",0,0,0,"","","","");
							ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
							ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
							memset(stemp,0,101);
							sprintf(stemp,"%s PROCESS LIST %d",m->MODULE,(y+1));
							addlstrsrc(mainrsrc,stemp,&l,TRUE,NULL,m->plist->numlibs,&m->plist->libs,NULL);
							ADVaddwdgt(mainrsrc->scn,7,stemp,"","","",10,0,0,"","","","");
							ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
						}
						ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
						ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
					}
				}
				ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,11,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
			}
		}
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","TOTAL ALL:    QUERY:","","",0,0,0,"","","","");
		memset(stemp,0,101);
		sprintf(stemp,"QUERY");
		addstdrsrc(mainrsrc,stemp,8,8,&Q,FALSE);
		ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","   RPTGEN:","","",0,0,0,"","","","");
		memset(stemp,0,101);
		sprintf(stemp,"RPTGEN");
		addstdrsrc(mainrsrc,stemp,8,8,&R,FALSE);
		ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,""," MAINTENANCE:","","",0,0,0,"","","","");
		memset(stemp,0,101);
		sprintf(stemp,"MAINTENANCE");
		addstdrsrc(mainrsrc,stemp,8,8,&M,FALSE);
		ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,""," SETUP:","","",0,0,0,"","","","");
		memset(stemp,0,101);
		sprintf(stemp,"SETUP");
		addstdrsrc(mainrsrc,stemp,8,8,&S,FALSE);
		ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,""," OTHER:","","",0,0,0,"","","","");
		memset(stemp,0,101);
		sprintf(stemp,"OTHER");
		addstdrsrc(mainrsrc,stemp,8,8,&O,FALSE);
		ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,""," TASK BAR:","","",0,0,0,"","","","");
		memset(stemp,0,101);
		sprintf(stemp,"DOCKWINDOW");
		addstdrsrc(mainrsrc,stemp,8,8,&D,FALSE);
		ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,8,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,13,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		addbtnrsrc(mainrsrc,"QUIT",TRUE,quituimetric,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		APPmakescrn(mainrsrc,TRUE,quituimetric,NULL,TRUE);
	}
}
