

#include "ExploreGoal_Evaluator.h"

ExploreGoal_Evaluator::ExploreGoal_Evaluator(int lvl)
{
	name="Explore";
	switch(lvl)
	{
		case Goal_Evaluator::LEVEL1_MIND:
			this->Tweaker = 0.05;
			break;
		case Goal_Evaluator::LEVEL2_MIND:
			this->Tweaker = 0.05;
			break;
		default:
			this->Tweaker = 0.05;
			break;
	}
}


ExploreGoal_Evaluator::~ExploreGoal_Evaluator(void)
{

}

//Este es muy simple solo se le da un rango muy pequeño para que se cumpla cuando los demás no se puedan cumplir
double ExploreGoal_Evaluator::CalculateDesirability(Npc* pBot)
{
  const double Desirability = Tweaker;

  return Desirability;
}

void ExploreGoal_Evaluator::SetGoal(Npc* bot)
  {

	  bot->getBrain()->AddSubgoalGetExplore(bot);
  }