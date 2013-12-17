#include "stdafx.h"
#include "GetWeaponGoal_Evaluator.h"


GetWeaponGoal_Evaluator::GetWeaponGoal_Evaluator(void)
{
}


GetWeaponGoal_Evaluator::~GetWeaponGoal_Evaluator(void)
{
}

double GetWeaponGoal_Evaluator::CalculateDesirability(Bot* pBot)
{
  
	
  //Nos dar� la distancia del tipo arma m�s cercano al bot
  double Distance = Goal_Evaluator::DistanceToItem(pBot,2);

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
    const double Tweaker = 0.3;

    double Health, WeaponStrength;

	Health = Goal_Evaluator::Health(pBot);

	//Calcular� la salud del arma que tiene el usuario siendo 0 si no tiene o esta muy da�ada
	WeaponStrength = Goal_Evaluator::WeaponHealth(pBot);

	double Desirability = (Tweaker * Health * (1.0-WeaponStrength)) / Distance;

    //ensure the value is in the range 0 to 1
    Desirability=clamp(Desirability, 0, 1);

    return Desirability;

}
}
 void GetWeaponGoal_Evaluator::SetGoal(Bot* bot)
  {

	  //Goal de WeaponGOal
	  bot->getBrain()->AddSubGoal(NULL);
  }