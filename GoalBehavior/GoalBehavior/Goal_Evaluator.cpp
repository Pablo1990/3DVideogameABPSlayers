#include "stdafx.h"
#include "Goal_Evaluator.h"


Goal_Evaluator::Goal_Evaluator(void)
{
	//Meter items
}


Goal_Evaluator::~Goal_Evaluator(void)
{
}


  //returns a value between 0 and 1 based on the bot's health. The better
  //the health, the higher the rating
  double Goal_Evaluator:: Health(Bot* pBot)
  {
  
	  double health=pBot->getSalud()/100.0;
	  return health;
  }
  //returns a value between 0 and 1 based on the bot's distance to the
  //given item. The farther the item, the higher the rating. If there is no
  //item of the given type present in the game world at the time this method
  //is called the value returned is 1
   double Goal_Evaluator:: DistanceToItem(Bot* pBot, int ItemType)
  {
	  double distancia=1;

	 
	  if(items.empty())
	  {
		return 1;

	  }
	  else
	  {
		for (std::list<Item>::iterator it = items.begin();
       it != items.end();
       ++it)
		{
			if((*it).typeItem==ItemType)
			{
				int distaux=sqrt((pow((pBot->getPos().first-(*it).Pos.first),2))+(pow((pBot->getPos().second-(*it).Pos.second),2)));
				//Estandarizamos
				distaux=distaux/distanciae_maxima;
				if(distaux <distancia)
					distancia=distaux;
			}
		}
	  
	  }
	  
	  return distancia;
  }

  //Nos devolverá un valor entre 0 e 1 segun el estado de salud del arma en el caso de valer 0 si no tiene arma el bot o 1 si se encuentra en salud máxima de arma por cada ataque el arma se desgastará en 1 valiendo
  //su valor como máximo 10
  double  Goal_Evaluator::WeaponHealth(Bot* pBot)
  {
	  double estado_arma=pBot->getArma()/10;
	  return 0;
  }
