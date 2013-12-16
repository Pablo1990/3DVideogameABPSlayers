#pragma once


#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include "Goal_Think.h"

const int Distancia_Max_Vision=4;

class Goal_Think;

struct Item
{
	Item(pair<double,double> pos_,int type_)
	{
		Pos=pos_;
		typeItem=type_;
	}
	pair<double,double> Pos;
	int typeItem;//1 Salud , 2 Arma
}; 


class Bot
{
private: 
		double salud;
		pair<double,double> Pos;
		double arma;//Sera el valor del estado del arma cuando sea 0 es cuando el bot no tendrá arma y 1 si está el arma en perfecto estado
		Goal_Think *  mente;
		Bot* enemigo;
		
		
public:
	Bot(double,double,double,double);
	void setSalud(double);
	void setPosition(double,double);
	void setArma(double);
	Bot* getEnem();
	void setEnem(Bot*);
	bool isEnemigoPresent();
	void setBrain(Goal_Think*);
	double getSalud();
	pair<double,double> getPos();
	double getArma();
	Goal_Think* getBrain();

	list<Item> items;
	~Bot(void);
};


