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
const double distanciae_maxima=141.42135623730;

class Goal_Evaluator
{
public:
	Goal_Evaluator(void);
	~Goal_Evaluator(void);
	virtual double CalculateDesirability(Npc* bot)=0;
	virtual void SetGoal(Npc* bot)=0;

  double Health(Npc* pNpc);
  
   double DistanceToItem(Npc* pNpc);
    double DistanceToItemHealth(Npc* pBot);
  double WeaponHealth(Npc* pNpc);
  inline float clamp(float x, float a, float b)
{

   return max(a, min(x, b));

}
 std::string name;
		
};







