#pragma once


#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include "Goal_Think.h"



class Goal_Think;

class Bot
{
private: 
		double salud;
		pair<double,double> Pos;
		double arma;//Sera el valor del estado del arma cuando sea 0 es cuando el bot no tendrá arma y 1 si está el arma en perfecto estado
		Goal_Think *  mente;
		
public:
	Bot(double,double,double,double);
	void setSalud(double);
	void setPosition(double,double);
	void setArma(double);
	double getSalud();
	pair<double,double> getPos();
	double getArma();
	Goal_Think* getBrain();
	~Bot(void);
};


