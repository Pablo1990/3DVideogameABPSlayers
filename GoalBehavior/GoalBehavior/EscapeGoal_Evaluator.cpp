#include "stdafx.h"
#include "EscapeGoal_Evaluator.h"


EscapeGoal_Evaluator::EscapeGoal_Evaluator(void)
{
}


EscapeGoal_Evaluator::~EscapeGoal_Evaluator(void)
{
}
double EscapeGoal_Evaluator::CalculateDesirability(Bot* pBot)
{
  double Desirability = 0.0;

  //only do the calculation if there is a target present
  if (pBot->isEnemigoPresent())
  {
	  const double Tweaker = pBot->getProbEscape();

	Desirability = Tweaker *(1-Goal_Evaluator::Health(pBot))*(1-Goal_Evaluator::WeaponHealth(pBot));
  }
  Desirability=clamp(Desirability, 0, 1);
  return Desirability;
}
void EscapeGoal_Evaluator::SetGoal(Bot* bot)
{

	//bot->getBrain()->AddSubgoalEscape;
}