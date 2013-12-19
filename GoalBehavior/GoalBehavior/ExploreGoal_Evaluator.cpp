#include "stdafx.h"

#include "ExploreGoal_Evaluator.h"

ExploreGoal_Evaluator::ExploreGoal_Evaluator(void)
{
}


ExploreGoal_Evaluator::~ExploreGoal_Evaluator(void)
{

}

//Este es muy simple solo se le da un rango muy pequeño para que se cumpla cuando los demás no se puedan cumplir
double ExploreGoal_Evaluator::CalculateDesirability(Bot* pBot)
{
  const double Desirability = 0.05;

  return Desirability;
}

void ExploreGoal_Evaluator::SetGoal(Bot* bot)
  {

	  bot->getBrain()->AddSubgoalGetExplore(bot);
  }