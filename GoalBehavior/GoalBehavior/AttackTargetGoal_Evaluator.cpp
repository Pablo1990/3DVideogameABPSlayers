#include "stdafx.h"
#include "AttackTargetGoal_Evaluator.h"


AttackTargetGoal_Evaluator::AttackTargetGoal_Evaluator(void)
{
	name="Attack";
}


AttackTargetGoal_Evaluator::~AttackTargetGoal_Evaluator(void)
{
	
}

double AttackTargetGoal_Evaluator::CalculateDesirability(Bot* pBot)
{
  double Desirability = 0.0;

  //only do the calculation if there is a target present
  if (pBot->isEnemigoPresent())
  {
	  const double Tweaker = pBot->getProbAttack();

	Desirability = Tweaker *Goal_Evaluator::Health(pBot) *Goal_Evaluator::WeaponHealth(pBot);
	 
  }
  Desirability=clamp(Desirability, 0, 1);
  return Desirability;
}
void AttackTargetGoal_Evaluator::SetGoal(Bot* bot)
{

	bot->getBrain()->AddSubgoalGetAtackTarget(bot);
}




