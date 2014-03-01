#include <misc.hpp>

double Rint(double amt)
{
	modf(amt,&amt);
	if(amt==(-0.0)) amt=0.0;
	return amt;
}
