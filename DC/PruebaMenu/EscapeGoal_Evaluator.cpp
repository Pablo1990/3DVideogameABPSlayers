
#include "EscapeGoal_Evaluator.h"


EscapeGoal_Evaluator::EscapeGoal_Evaluator(void)
{
	name="Escape";
}


EscapeGoal_Evaluator::~EscapeGoal_Evaluator(void)
{
	
}
double EscapeGoal_Evaluator::CalculateDesirability(Npc* pBot)
{
  double Desirability = 0.0;

  //only do the calculation if there is a target present
  if (pBot->isEnemigoPresent())
  {
    const double Tweaker = 0.6;

	Desirability = Tweaker *(1-Goal_Evaluator::Health(pBot))*(1-Goal_Evaluator::WeaponHealth(pBot));
  }
  Desirability=clamp(Desirability, 0, 1);
  return Desirability;
}
void EscapeGoal_Evaluator::SetGoal(Npc* bot)
{

	bot->getBrain()->AddSubgoalEscape(bot);
}
