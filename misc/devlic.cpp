#include<misc.hpp>

/* CheckDevLicense - Used to check available DEV_LICENSE values */
short CheckDevLicense(char *l)
{
	int psswd=0;

	if(isEMPTY(l)) return(FALSE);
	sprintf(stemp,"%.4s%.2s%.2s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],
		&CURRENT_DATE10[3]);
	psswd=atoi(stemp)-1225;
	if(psswd==(atoi(l)))
	{
		return(TRUE);
	} 
	return(FALSE);
}
