
#include "AttackTargetGoal_Evaluator.h"


AttackTargetGoal_Evaluator::AttackTargetGoal_Evaluator(int lvl)
{
	name="Attack";
	switch(lvl)
	{
		case Goal_Evaluator::LEVEL1_MIND:
			this->Tweaker = 0.7;
			break;
		case Goal_Evaluator::LEVEL2_MIND:
			this->Tweaker = 0.9;
			break;
		default:
			this->Tweaker = 0.7;
			break;
	}
}


AttackTargetGoal_Evaluator::~AttackTargetGoal_Evaluator(void)
{
	
}

double AttackTargetGoal_Evaluator::CalculateDesirability(Npc* pBot)
{
  double Desirability = 0.0;
   double Distance = Goal_Evaluator::DistanceToEnem(pBot);
  //only do the calculation if there is a target present
   if (pBot->isEnemigoPresent() && pBot->get_resistance() > 6)
  {
    //const double Tweaker = 0.8;

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




