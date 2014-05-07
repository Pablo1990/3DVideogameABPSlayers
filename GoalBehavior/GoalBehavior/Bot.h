#pragma once


#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include <list>
#include <math.h>

#include "CNeuralNet.h"
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"


const int Distancia_Max_Vision=4*25;
const int dimMapa = 475;
const int dimCasilla = 25;
const int numObjetos = 10;

using namespace std;

struct Item
{
	Item(std::pair<double,double> pos_,int type_)
	{
		Pos=pos_;
		typeItem=type_;
	}
	std::pair<double,double> Pos;
	int typeItem;//1 Salud , 2 Arma
}; 


class Bot
{
private: 
		double salud;
		CNeuralNet m_ItsBrain;
		double posX;
		double posY;
		double arma;//Sera el valor del estado del arma cuando sea 0 es cuando el bot no tendrá arma y 1 si está el arma en perfecto estado
		double fitness;
		double movimientoX;
		double movimientoY;
		Bot* enemigo;
		int armasCogidas;
		
		
public:
	
	vector<int*> itemsX;
	vector<int*> itemsY;
	Bot(vector<int*> itemsX, vector<int*> itemsY);
	void Reset();
	bool Update();
	double Fitness();
	void PutWeights(vector<double> &w){m_ItsBrain.PutWeights(w);}
	int GetNumberOfWeights()const{return m_ItsBrain.GetNumberOfWeights();}
	void aumentoFitnessRecogidas() {fitness+=armasCogidas*4;}
	void aumentoFitness() {fitness++;}
	int getArmasCogidas() {return armasCogidas;}
	void disminuyoFitness() {if(fitness>0) fitness--;}
	void crearListaObjetos(vector<int*> itemsX, vector<int*> itemsY);
	void getPosMasCercano(double &, double &);
	double getDistanciaABot(double x, double y);
	void getPosRelativaABot(double &, double &);
	void posEntreCeroYUno(double&);
	CNeuralNet GetNeuralNet() {return m_ItsBrain; }
	bool estoyEnObjeto();
	void mover();
	void setPosition(double x, double y)
	{
		posX=x;
		posY=y;
	}
	double getposX()
	{
		return posX;
	}
	double getposY()
	{
		return posY;
	}
	void setSalud(double);
	
	bool MoverseAItemSalud();
	bool MoverseAItemArma();
	bool MoverseAEnemigo();
	bool Move_Explore();
	bool Move_ToFreeAttack();
	
	std::pair<double,double> DarPosSalud();
	std::pair<double,double> DarPosArmaCercana();
	void setArma(double);
	Bot* getEnem();
	void setEnem(Bot*);
	bool isEnemigoPresent();
	
	double getSalud();

	double getArma();
	

	std::list<Item> items;
	~Bot(void);
};


