#pragma once


#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include <list>


const int Distancia_Max_Vision=4*25;


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
		std::pair<double,double> Pos;
		double arma;//Sera el valor del estado del arma cuando sea 0 es cuando el bot no tendrá arma y 1 si está el arma en perfecto estado
	
		Bot* enemigo;
		
		
public:
	Bot(double,double,double,double);
	void setSalud(double);
	void setPosition(double,double);
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
	std::pair<double,double> getPos();
	double getArma();
	

	std::list<Item> items;
	~Bot(void);
};


