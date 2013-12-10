#pragma once

#include "Bot.h"
#include "amp.h"
#include "math.h"
#include <iostream>
#include <string>
#include <utility>
#include <iomanip>

using namespace Concurrency::direct3d;

class Bot;
//Para estandarizar la distancia entre 0 y 1
const double distanciae_maxima=141.4213562;
struct Item
{

	pair<double,double> Pos;
	int typeItem;//1 Salud , 2 Arma , 3 Adversario
}; 

class Goal_Evaluator
{
public:
	Goal_Evaluator(void);
	~Goal_Evaluator(void);
	virtual double CalculateDesirability(Bot* bot)=0;
	virtual void SetGoal(Bot* bot)=0;

  double Health(Bot* pBot);
  
   double DistanceToItem(Bot* pBot, int ItemType);

  double WeaponHealth(Bot* pBot);
  inline float clamp(float x, float a, float b)

{

    return x < a ? a : (x > b ? b : x);

}

private:
		list<Item> items;
};







