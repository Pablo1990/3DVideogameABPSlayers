
#include "GetHealthGoal_Evaluator.h"


GetHealthGoal_Evaluator::GetHealthGoal_Evaluator(int lvl)
{
	name="GetHealth";
	switch(lvl)
	{
		case Goal_Evaluator::LEVEL1_MIND:
			this->Tweaker = 0.4;
			break;
		case Goal_Evaluator::LEVEL2_MIND:
			this->Tweaker = 0.2;
			break;
		default:
			this->Tweaker = 0.4;
			break;
	}
}


GetHealthGoal_Evaluator::~GetHealthGoal_Evaluator(void)
{
}

double GetHealthGoal_Evaluator::CalculateDesirability(Npc* pBot)
{
  
	//first grab the distance to the closest instance of a health item
	double Distance = Goal_Evaluator::DistanceToItemHealth(pBot);

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
    //const double Tweaker = 0.4;

    //the desirability of finding a health item is proportional to the amount
    //of health remaining and inversely proportional to the distance from the
    //nearest instance of a health item.
	float Desirability=0.0;
	
	if(Distance!=0)
	{
		Desirability = Tweaker * (1-Goal_Evaluator::Health(pBot)) /Distance;
		
	}
	else
	{
		Desirability = Tweaker * (1-Goal_Evaluator::Health(pBot));
	
	}

    //ensure the value is in the range 0 to 1

	Desirability=clamp(Desirability, 0.0, 1.0);
    return Desirability;
  }
	return 0;
 
}

 void GetHealthGoal_Evaluator::SetGoal(Npc* bot)
  {

	  bot->getBrain()->AddSubgoalGetItem(bot,1);
  }