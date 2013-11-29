#include "stdafx.h"
#include "Goal.h"

Goal<class Bot>::Goal()
{

 }

Goal<class Bot>::~Goal(void)
{

}

int  Goal<typename Bot>:: getTipo()
{
	return tipo;
}

bool  Goal<typename Bot>:: isactive()
{

	if(estado==1)
		return true;
	return false;
}
bool  Goal<typename Bot>:: isinactive()
{

	if(estado==0)
		return true;
	return false;
}
bool  Goal<typename Bot>:: iscomplete()
{
	if(estado==2)
		return true;
	return false;
}

bool  Goal<typename Bot>::hasfailed()
{

	if(estado==3)
		return true;
	return false;
}