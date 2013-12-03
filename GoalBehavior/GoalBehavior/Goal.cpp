#include "stdafx.h"
#include "Goal.h"



int  Goal:: getTipo()
{
	return tipo;
}

bool  Goal:: isactive()
{

	if(estado==1)
		return true;
	return false;
}
bool  Goal:: isinactive()
{

	if(estado==0)
		return true;
	return false;
}
bool  Goal:: iscomplete()
{
	if(estado==2)
		return true;
	return false;
}

bool  Goal::hasfailed()
{

	if(estado==3)
		return true;
	return false;
}
