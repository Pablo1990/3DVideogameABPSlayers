#include "stdafx.h"
#include "Bot.h"


Bot::Bot(double salud_,double posx_,double posy_,double arma_)
{
	salud=salud_;
	Pos.first=posx_;
	Pos.second=posy_;
	arma=arma_;
}


Bot::~Bot(void)
{
}

double Bot:: getArma()
{
	return arma;
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