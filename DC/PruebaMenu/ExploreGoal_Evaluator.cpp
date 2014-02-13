

#include "ExploreGoal_Evaluator.h"

ExploreGoal_Evaluator::ExploreGoal_Evaluator(void)
{
	name="Explore";
}


ExploreGoal_Evaluator::~ExploreGoal_Evaluator(void)
{

}

//Este es muy simple solo se le da un rango muy pequeño para que se cumpla cuando los demás no se puedan cumplir
double ExploreGoal_Evaluator::CalculateDesirability(Npc* pBot)
{
  const double Desirability = 0.05;

  return Desirability;
}

void ExploreGoal_Evaluator::SetGoal(Npc* bot)
  {

	  bot->getBrain()->AddSubgoalGetExplore(bot);
  }