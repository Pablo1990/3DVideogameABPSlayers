#include "stdafx.h"
#include "Goal_Think.h"


Goal_Think::Goal_Think(void)
{
}


Goal_Think::~Goal_Think(void)
{
}

void Goal_Think:: Activate()
{


}
int Goal_Think:: Process()
{
	return 0;

}
void Goal_Think::Terminate()
{

}
bool Goal_Think:: Present(int type)
{

	return false;
}

void Goal_Think::Arbitrate()
{
  double best = 0;
  Goal_Evaluator* MostDesirable = NULL;

  //iterate through all the evaluators to see which produces the highest score
 list<Goal_Evaluator*>::iterator curDes = Evaluators.begin();
  for (curDes; curDes != Evaluators.end(); ++curDes)
  {
    double desirabilty = (*curDes)->CalculateDesirability(dueño);

    if (desirabilty >= best)
    {
      best = desirabilty;
      MostDesirable = *curDes;
    }
  }

  MostDesirable->SetGoal(dueño);
}
