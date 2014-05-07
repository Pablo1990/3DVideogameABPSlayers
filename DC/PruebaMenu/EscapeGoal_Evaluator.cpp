
#include "EscapeGoal_Evaluator.h"


EscapeGoal_Evaluator::EscapeGoal_Evaluator(int lvl)
{
	name="Escape";
	switch(lvl)
	{
		case Goal_Evaluator::LEVEL1_MIND:
			this->Tweaker = 0.6;
			break;
		case Goal_Evaluator::LEVEL2_MIND:
			this->Tweaker = 0.3;
			break;
		default:
			this->Tweaker = 0.6;
			break;
	}
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
    //const double Tweaker = 0.6;

	Desirability = Tweaker *(1-Goal_Evaluator::Health(pBot))*(1-Goal_Evaluator::WeaponHealth(pBot));
  }
  Desirability=clamp(Desirability, 0, 1);
  return Desirability;
}
void EscapeGoal_Evaluator::SetGoal(Npc* bot)
{

	bot->getBrain()->AddSubgoalEscape(bot);
}
