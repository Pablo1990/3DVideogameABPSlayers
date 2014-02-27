
#include "AttackTargetGoal_Evaluator.h"


AttackTargetGoal_Evaluator::AttackTargetGoal_Evaluator(void)
{
	name="Attack";
}


AttackTargetGoal_Evaluator::~AttackTargetGoal_Evaluator(void)
{
	
}

double AttackTargetGoal_Evaluator::CalculateDesirability(Npc* pBot)
{
  double Desirability = 0.0;
   double Distance = Goal_Evaluator::DistanceToEnem(pBot);
  //only do the calculation if there is a target present
  if (pBot->isEnemigoPresent())
  {
    const double Tweaker = 0.8;

	if(Distance != 0)
	{
		Desirability = (Tweaker *Goal_Evaluator::Health(pBot) *Goal_Evaluator::WeaponHealth(pBot)) / Distance;
	}
	else
	{
		Desirability = (Tweaker *Goal_Evaluator::Health(pBot) *Goal_Evaluator::WeaponHealth(pBot));
	}
	 
  }
  Desirability=clamp(Desirability, 0, 1);
  return Desirability;
}
void AttackTargetGoal_Evaluator::SetGoal(Npc* bot)
{

	bot->getBrain()->AddSubgoalGetAtackTarget(bot);
}




