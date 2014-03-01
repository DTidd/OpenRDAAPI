#include <doheader.hpp>

char doheaders=FALSE;

short xDoHeaders(int line,char *file)
{
        char *tmp=NULL;

        tmp=RDA_GetEnv("HEADERS");
        if(!isEMPTY(tmp))
        {
                stoupper(tmp);
                if(!RDAstrcmp(tmp,""))
                {
                        doheaders=TRUE;
                } else {
                        doheaders=FALSE;
                }
        } else {
                doheaders=TRUE;
        }
        return(doheaders);
}
