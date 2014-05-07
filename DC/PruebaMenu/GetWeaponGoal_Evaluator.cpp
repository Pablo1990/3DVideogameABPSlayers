
#include "GetWeaponGoal_Evaluator.h"


GetWeaponGoal_Evaluator::GetWeaponGoal_Evaluator(int lvl)
{
	name="GetWeapon";
	switch(lvl)
	{
		case Goal_Evaluator::LEVEL1_MIND:
			this->Tweaker = 0.3;
			break;
		case Goal_Evaluator::LEVEL2_MIND:
			this->Tweaker = 0.3;
			break;
		default:
			this->Tweaker = 0.6;
			break;
	}
}


GetWeaponGoal_Evaluator::~GetWeaponGoal_Evaluator(void)
{

}

double GetWeaponGoal_Evaluator::CalculateDesirability(Npc* pBot)
{
  
	
  //Nos dará la distancia del tipo arma más cercano al bot
  double Distance = Goal_Evaluator::DistanceToItem(pBot);
  //if the distance feature is rated with a value of 1 it means that the
  //item is either not present on the map or too far away to be worth
  //considering, therefore the desirability is zero
  if (Distance == 1)
  {
    return 0;
  }
  else
  {
    //value used to tweak the desirability
    //const double Tweaker = 0.3;

    double Health, WeaponStrength;

	Health = Goal_Evaluator::Health(pBot);

	//Calculará la salud del arma que tiene el usuario siendo 0 si no tiene o esta muy dañada
	WeaponStrength = Goal_Evaluator::WeaponHealth(pBot);
	double Desirability=0.0;
	if(Distance!=0)
	{
		Desirability = (Tweaker * Health * (1.0-WeaponStrength)) / Distance;
		
	}
	else
	{
	
		Desirability = (Tweaker * Health * (1.0-WeaponStrength));
	}

		

    //ensure the value is in the range 0 to 1
    Desirability=clamp(Desirability, 0, 1);

    return Desirability;

}
}
 void GetWeaponGoal_Evaluator::SetGoal(Npc* bot)
  {

	  //Goal de WeaponGOal
	  bot->getBrain()->AddSubgoalGetItem(bot,2);
  }