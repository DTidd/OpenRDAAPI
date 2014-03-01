#include <mkmenu.hpp>

void xConvMenu(RDAmenu *menu,char *New_Extension,int line,char *file)
{
	int x=0,length=0;
	char *tmp=NULL,*tmp2=NULL;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG ConvMenu at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(menu!=NULL)
	{
		if(menu->command!=NULL)
		{
			for(x=0;x<(RDAstrlen(menu->command)-3);++x)
			{
				if(!strncmp(&menu->command[x],".sco",4) || 
					!strncmp(&menu->command[x],".lnx",4) ||
					!strncmp(&menu->command[x],".rsc",4)
					|| !strncmp(&menu->command[x],".unw",4)
					|| !strncmp(&menu->command[x],".exe",4) 
					|| !strncmp(&menu->command[x],".EXE",4)
					|| !strncmp(&menu->command[x],".EXT",4))
				{
					length=RDAstrlen(menu->command)-3+
						RDAstrlen(New_Extension);
					if(tmp!=NULL) Rfree(tmp);
					tmp=Rmalloc(length);
					if(x<1)
					{
						sprintf(tmp,"%s%s",New_Extension
							,menu->command);
					} else if(x<(RDAstrlen(menu->command)-4))
					{
						sprintf(tmp,"%.*s%s%s",x,&menu->command[0],
							New_Extension,&menu->command[x+4]);
					} else {
						if(tmp2!=NULL) Rfree(tmp2);
						tmp2=stralloc(menu->command);
						tmp2[RDAstrlen(tmp2)-4]=0;
						sprintf(tmp,"%s%s",tmp2,
							New_Extension);
					}
					if(menu->command!=NULL) Rfree(menu->command); 
					if(tmp!=NULL)
					{
						menu->command=stralloc(tmp);
						Rfree(tmp);
					}
				}
			}
		}
	} else {
		prterr("Error RDAmenu is NULL at line [%d] program [%s].",line,file);
	}
}
