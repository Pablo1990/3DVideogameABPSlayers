#include "stdafx.h"
#include "Bot.h"


Bot::Bot(int salud_,int posx_,int posy_,bool arma_)
{
	salud=salud_;
	posx=posx_;
	posy=posy_;
	arma=arma_;

}


Bot::~Bot(void)
{
}

bool Bot:: getArma()
{
	return arma;
}
int Bot::getSalud()
{
	return salud;
}
int Bot::getPosx()
{
	return posx;


}
int Bot::getPosy()
{
	return posy;


}
void Bot::setArma(bool est_arma)
{
	arma=est_arma;

}
void Bot::setPosition(int posx_,int posy_)
{
	posx=posx_;
	posy=posy_;
}
void Bot::setSalud(int salud_)
{
	salud=salud_;

}