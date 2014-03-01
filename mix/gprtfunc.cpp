#include <guip.hpp>

void printrsrcs(RDArsrc *s)
{
	SEERSRCS(s,TRUE);
}
void printscrndef(RDAscrn *s)
{
	SEESCRN(s,TRUE);
}
void printdefault(RDAdefault *s)
{
	SEEDEFAULTS(s,TRUE);
}
