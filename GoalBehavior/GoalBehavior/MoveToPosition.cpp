#include "stdafx.h"
#include "MoveToPosition.h"


MoveToPosition::MoveToPosition(Bot * b,int whoitem)
{
	d=b;
	estado=inactive;
	whoItem=whoitem;
}


MoveToPosition::~MoveToPosition(void)
{
}

void MoveToPosition::Activate()
{
	estado=active;
	cout<<"Activado Ir hacia Objetivo"<<endl;
}

int MoveToPosition::Process()
{
	Activate();
	//Si queremos ir al lado de item de salud
	if(whoItem==1)
	{
		//Nos movemos al lado del item de salud
		d->MoverseAItemSalud();
		Terminate();
		
	}
	else if(whoItem==2)
	{
		//Nos moveremos al item de arma más cercano
		d->MoverseAItemArma();
		Terminate();
	}
	//Ir hacia Bot
	else
	{
	
	}
	return estado;
}
