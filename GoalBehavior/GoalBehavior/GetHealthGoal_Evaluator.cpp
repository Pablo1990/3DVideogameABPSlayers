#include "stdafx.h"
#include "GetHealthGoal_Evaluator.h"


GetHealthGoal_Evaluator::GetHealthGoal_Evaluator(void)
{
}


GetHealthGoal_Evaluator::~GetHealthGoal_Evaluator(void)
{
}

double GetHealthGoal_Evaluator::CalculateDesirability(Bot* pBot)
{
  
	//first grab the distance to the closest instance of a health item
	double Distance = Goal_Evaluator::DistanceToItem(pBot, 1);

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
	  const double Tweaker = pBot->getProbGetHealth();

    //the desirability of finding a health item is proportional to the amount
    //of health remaining and inversely proportional to the distance from the
    //nearest instance of a health item.
	float Desirability = Tweaker * (1-Goal_Evaluator::Health(pBot)) /
		(Goal_Evaluator::DistanceToItem(pBot, 1));

    //ensure the value is in the range 0 to 1

	Desirability=clamp(Desirability, 0.0, 1.0);
    return Desirability;
  }
	return 0;
 
}

 void GetHealthGoal_Evaluator::SetGoal(Bot* bot)
  {

	  bot->getBrain()->AddSubgoalGetItem(bot,1);
  }