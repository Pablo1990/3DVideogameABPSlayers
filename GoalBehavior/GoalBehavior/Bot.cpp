#include "stdafx.h"
#include "Bot.h"


Bot::Bot(double salud_,double posx_,double posy_,double arma_)
{
	salud=salud_;
	Pos.first=posx_;
	Pos.second=posy_;
	arma=arma_;
	//Meto aqui los items para poner por el mapa
		
		pair<double,double> pos;
		pos.first=15;
		pos.second=10;
		Item i=Item(pos,2);
		//item
		items.push_front(i);
		//item
		pos.first=19;
		pos.second=5;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=5;
		pos.second=15;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=12;
		pos.second=5;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=4;
		pos.second=5;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=15;
		pos.second=0;
		i=Item::Item(pos,1);
		items.push_front(i);

}


Bot::~Bot(void)
{
}

double Bot:: getArma()
{
	return arma;
}

Bot* Bot::getEnem()
{

	return enemigo;
}

void Bot::setEnem(Bot* bot_)
{
	
	enemigo=bot_;
}
void Bot::setBrain(Goal_Think* brain_)
{
	
	mente=brain_;
}


//Si el bot enemigo esta dentro de un rango entonces dirá que si esta presente y entonces pueda realizar el objetivo de ataque.
bool Bot::isEnemigoPresent()
{
	if(enemigo !=NULL)
	{
		int x_E=enemigo->Pos.first;
		int y_E=enemigo->Pos.second;
		int x=Pos.first;
		int y=Pos.second;
		int distaux=sqrt(pow((x-x_E),2)+pow((y-y_E),2));
		if(distaux <=Distancia_Max_Vision)
		{
			return true;
		}
	}
	return false;

}

double Bot::getSalud()
{
	return salud;
}
pair<double,double> Bot::getPos()
{
	return Pos;


}

void Bot::setArma(double est_arma)
{
	arma=est_arma;

}
void Bot::setPosition(double posx_,double posy_)
{
	Pos.first=posx_;
	Pos.second=posy_;
}
void Bot::setSalud(double salud_)
{
	salud=salud_;

}

Goal_Think* Bot::getBrain()
{
	return mente;
}