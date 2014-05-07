#pragma once

#include "Npc.h"
#include "amp.h"
#include "math.h"
#include <iostream>
#include <string>
#include <utility>
#include <iomanip>

using namespace Concurrency::direct3d;
using namespace std;
class Npc;

//Para estandarizar la distancia entre 0 y 1 La distancia de ancho y largo 20 celdas
/* 
Haría falta sacar las distancia euclidea entre el punto máximo 


*/
const double distanciae_maxima=sqrt((pow((0-1894.93),2))+(pow((0-1294.88),2)));

class Goal_Evaluator
{
public:
	static const int LEVEL1_MIND = 0;
	static const int LEVEL2_MIND = 1;
	Goal_Evaluator(void);
	~Goal_Evaluator(void);
	virtual double CalculateDesirability(Npc* bot)=0;
	virtual void SetGoal(Npc* bot)=0;
	double DistanceToEnem(Npc* pBot);
  double Health(Npc* pNpc);
  
   double DistanceToItem(Npc* pNpc);
    double DistanceToItemHealth(Npc* pBot);
  double WeaponHealth(Npc* pNpc);
  inline float clamp(float x, float a, float b)
{

   return max(a, min(x, b));

}
 std::string name;
protected:
	double Tweaker;
};







