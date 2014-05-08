#include "stdafx.h"
#include "Bot.h"

using namespace std;

Bot::Bot(vector<int*> itemsX, vector<int*> itemsY, double x, double y)
{
	salud=100;
	arma=15;
	fitness = 0;
	posX= x;
	posY = y;
	armasCogidas = 0;
	crearListaObjetos(itemsX, itemsY);
}
void Bot::crearListaObjetos(vector<int*> itemsX, vector<int*> itemsY){

	for(int i=0; i<itemsX.size(); i++){
		this->itemsX.push_back(itemsX[i]);
		this->itemsY.push_back(itemsY[i]);
	}
}

Bot::~Bot(void)
{
	
}

void Bot:: Reset(double x, double y)
{
	posX = x;
	posY = y;
	armasCogidas=0;
	fitness = 0;
}

bool Bot :: Update()
{
	vector<double> inputs;
	double x = 0;
	double y = 0;
	getPosMasCercano(x, y);
	inputs.push_back(x);
	inputs.push_back(y);
	vector<double> output = m_ItsBrain.Update(inputs);
	if (output.size() < CParams::iNumOutputs) 
	{
		cerr<<"ERROR"<<endl;
		return false;
	}
	//me muevo hacia la izquierda
	movimientoX = 0;
	if(output[0]>=0.5)
		movimientoX = dimCasilla;

	if(output[1]>=0.5)
		movimientoX += -dimCasilla;


	movimientoY = 0;
	if(output[2]>=0.5)
		movimientoY = dimCasilla;

	if(output[3]>=0.5)
		movimientoY += -dimCasilla;

	return true;
}

void Bot :: getPosMasCercano(double& x, double &y){
	int distancia=9999.9;
	for (int i = 0; i < itemsX.size(); i++)
	{
		double distanciaAct = getDistanciaABot(*itemsX[i], *itemsY[i]);
		if(distanciaAct < distancia){
			distancia = distanciaAct;
			x = *itemsX[i];
			y = *itemsY[i];
		}
	}
	getPosRelativaABot(x,y);
	posEntreCeroYUno(x);
	posEntreCeroYUno(y);
}
void Bot :: posEntreCeroYUno(double &y){
	y = y/(sqrt(2)*dimMapa);
	Clamp(y, -1, 1);
}

void Bot :: getPosRelativaABot(double &x, double &y){
	x =x - posX;
	y = y - posY;
}

double Bot::getDistanciaABot(double x, double y){
	return sqrt(pow(posX, 2)+pow(posY,2)) - sqrt(pow(x, 2)+pow(y,2));
}

double Bot::Fitness()
{
	return fitness;
}

void Bot::mover(){
	if(posX + movimientoX <= dimMapa && posX + movimientoX >= 0)
		posX += movimientoX;
	if(posY + movimientoY <= dimMapa && posY + movimientoY >= 0)
		posY += movimientoY;

	movimientoX = 0;
	movimientoY = 0;
}

bool Bot ::estoyEnObjeto(){
	std::vector<int*>::iterator it2 = itemsY.begin();
	for(std::vector<int*>::iterator it = itemsX.begin(); it != itemsX.end(); ++it) {
		if(**it == posX && **it2 == posY)
		{
			**it = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
			**it2 = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
			armasCogidas++;
			return true;
		}
		it2++;
	}
	return false;
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



double Bot::getSalud()
{
	return salud;
}

/*
std::pair<double,double> Bot::DarPosSalud()
{
	for (std::list<Item>::iterator it = items.begin();
       it != items.end();
       ++it)
		{
			if((*it).typeItem==1)
			{
				return (*it).Pos;
			}
		}

}
*/
/*
bool Bot::MoverseAEnemigo()
{
	bool muevo=false;
	int ran=rand()%8;
	int posx=0;
	int posy=0;
	std::pair<double,double> pos=getEnem()->getPos();
	//cout<<"rand"<<ran<<pos.first<<"   "<<pos.second<<";"<<endl;
	//cout<<"Me muevo"<<endl;
	while(muevo ==false)
	{
		switch (ran)
		{
		case 0:	posx=pos.first-25;
				posy=pos.second-25;
				if(posx>=0  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 1:	posx=pos.first;
				posy=pos.second-25;
		if(posx>=0  && posy>=0)
		{
			muevo=true;
			Pos.first=posx;
			Pos.second=posy;
		}		
		break;
		case 2:	posx=pos.first+25;
				posy=pos.second-25;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 3:	posx=pos.first+25;
				posy=pos.second;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;

		case 4:	posx=pos.first+25;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 5:	posx=pos.first;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 6:	posx=pos.first-25;
				posy=pos.second+25;
				if(posx>=0  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 7:	posx=pos.first-25;
			posy=pos.second;
			if(posx>=0  && posy<=475)
			{
				muevo=true;
				Pos.first=posx;
				Pos.second=posy;
			}		
			break;
		
		
		default:
			break;
		}
		ran=rand()%8;
	}
	return true;


}
*/
void Bot::setArma(double est_arma)
{
	arma=est_arma;

}

void Bot::setSalud(double salud_)
{
	salud=salud_;

}

