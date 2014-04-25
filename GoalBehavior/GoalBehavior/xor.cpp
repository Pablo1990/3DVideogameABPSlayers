#include "xor.h"


xor::xor(void)
{
	fitness = 0;
	posX = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
	posY = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
	crearListaObjetos();
}

void xor::crearListaObjetos(){
	itemsX.push_back(15*dimCasilla);
	itemsY.push_back(10*dimCasilla);
	itemsX.push_back(19*dimCasilla);
	itemsY.push_back(5*dimCasilla);
	itemsX.push_back(5*dimCasilla);
	itemsY.push_back(15*dimCasilla);
	itemsX.push_back(15*dimCasilla);
	itemsY.push_back(10*dimCasilla);
	itemsX.push_back(12*dimCasilla);
	itemsY.push_back(5*dimCasilla);
	itemsX.push_back(4*dimCasilla);
	itemsY.push_back(5*dimCasilla);
	itemsX.push_back(15*dimCasilla);
	itemsY.push_back(0*dimCasilla);
}

xor::~xor(void)
{
}

void xor::Reset()
{
	posX = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
	posY = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
}

bool xor::Update()
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
	if(output[0]>=0.6)
		movimientoX = dimCasilla;

	movimientoY = 0;
	if(output[1]>=0.6)
		movimientoY = dimCasilla;

	return true;
}

void xor::getPosMasCercano(double& x, double &y){
	int distancia=9999.9;
	for (int i = 0; i < itemsX.size(); i++)
	{
		double distanciaAct = getDistanciaABot(itemsX[i], itemsY[i]);
		if(distanciaAct < distancia){
			distancia = distanciaAct;
			x = itemsX[i];
			y = itemsY[i];
		}
	}
	getPosRelativaABot(x,y);
	posEntreCeroYUno(x);
	posEntreCeroYUno(y);
}

void xor::posEntreCeroYUno(double &y){
	double y = y/(sqrt(2)*dimMapa);
	Clamp(y, 0, 1);
}

void xor::getPosRelativaABot(double &x, double &y){
	x = abs(x - posX);
	y = abs(y - posY);
}

double xor::getDistanciaABot(double x, double y){
	return abs(sqrt(pow(posX, 2)+pow(posY,2)) - sqrt(pow(x, 2)+pow(y,2)));
}

double xor::Fitness(){
	return fitness;
}

bool xor::estoyEnObjeto(){
	for (int i = 0; i < itemsX.size(); i++)
		if(itemsX[i] == posX && itemsY[i] == posY)
		{
			itemsX[i] = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
			itemsY[i] = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
			return true;
		}
}