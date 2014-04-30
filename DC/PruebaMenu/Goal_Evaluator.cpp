
#include "Goal_Evaluator.h"


Goal_Evaluator::Goal_Evaluator(void)
{
	
}


Goal_Evaluator::~Goal_Evaluator(void)
{
}


  //returns a value between 0 and 1 based on the bot's health. The better
  //the health, the higher the rating
  double Goal_Evaluator:: Health(Npc* pBot)
  {
  
	  double health=pBot->get_health()/100.0;
	  return health;
  }
  //returns a value between 0 and 1 based on the bot's distance to the
  //given item. The farther the item, the higher the rating. If there is no
  //item of the given type present in the game world at the time this method
  //is called the value returned is 1

   double Goal_Evaluator:: DistanceToItem(Npc* pBot)
  {
	  double distancia=1.0;
	  if(pBot->getItems()->empty())
	  {
		
		return 1.0;

	  }

	  else
	  {
			std::list<Weapon*> lista= *pBot->getItems();
			for (std::list<Weapon*>::iterator it = lista.begin();
			  it != lista.end();
       it++)
		{
			if((*it)!=NULL){
				if(!(*it)->no_weapon())
				{
					double distaux=sqrt((pow((pBot->get_position().X-(*it)->get_absolute_position().X),2))+(pow((pBot->get_position().Z-(*it)->get_absolute_position().Z),2)));
				
					//Estandarizamos
				
						distaux=distaux/distanciae_maxima;
						if(distaux <=distancia)
							distancia=distaux;
				}
			}
			
		}
	  
	  }
	  return distancia;
  }

   //returns a value between 0 and 1 based on the bot's distance to the
  //given item. The farther the item, the higher the rating. If there is no
  //item of the given type present in the game world at the time this method
  //is called the value returned is 1
   double Goal_Evaluator:: DistanceToItemHealth(Npc* pBot)
  {
	  double distancia=1.0;
	  
			
	  distancia=sqrt((pow((pBot->get_position().X-pBot->DarPosSalud().X),2))+(pow((pBot->get_position().Z-pBot->DarPosSalud().Z),2)));
				
	  //Estandarizamos
				distancia=distancia/distanciae_maxima;
				
			
		
	  
	  	  return distancia;
  }
   
   double Goal_Evaluator:: DistanceToEnem(Npc* pBot)
  {
	  double distancia=1.0;
	  
			
	  distancia=sqrt((pow((pBot->get_position().X-pBot->getEnem()->get_position().X),2))+(pow((pBot->get_position().Z-pBot->getEnem()->get_position().Z),2)));
				
	  //Estandarizamos
				distancia=distancia/distanciae_maxima;
				
			
		
	  
	  	  return distancia;
  }
  //Nos devolverá un valor entre 0 e 1 segun el estado de salud del arma en el caso de valer 0 si no tiene arma el bot o 1 si se encuentra en salud máxima de arma por cada ataque el arma se desgastará en 1 valiendo
  //su valor como máximo 15
  double  Goal_Evaluator::WeaponHealth(Npc* pBot)
  {

	  double estado_arma= 0;
	  if(pBot->get_weapon())
		  estado_arma = pBot->get_weapon()->get_resist()/15;
	  
	  return estado_arma;
  }
