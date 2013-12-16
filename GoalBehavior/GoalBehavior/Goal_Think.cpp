#include "stdafx.h"
#include "Goal_Think.h"
#include "EscapeGoal_Evaluator.h";
#include "AttackTargetGoal_Evaluator.h"
#include "GetHealthGoal_Evaluator.h"
#include "GetWeaponGoal_Evaluator.h"
#include "ExploreGoal_Evaluator.h"


Goal_Think::Goal_Think()
{
	EscapeGoal_Evaluator *e=new EscapeGoal_Evaluator();
	GetWeaponGoal_Evaluator *e2=new GetWeaponGoal_Evaluator();
	AttackTargetGoal_Evaluator *e3=new AttackTargetGoal_Evaluator();
	ExploreGoal_Evaluator *e4=new ExploreGoal_Evaluator();
	GetHealthGoal_Evaluator *e5=new GetHealthGoal_Evaluator();

	Evaluators.push_back(e);
	Evaluators.push_back(e2);
	Evaluators.push_back(e3);
	Evaluators.push_back(e4);
	Evaluators.push_back(e5);

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
void Goal_Think::setDueño(Bot* d)
{
	dueño=d;
}
void Goal_Think::Arbitrate()
{
  double best = 0;
  Goal_Evaluator* MostDesirable = NULL;
  
  //iterate through all the evaluators to see which produces the highest score
 list<Goal_Evaluator*>::iterator curDes = Evaluators.begin();
  for (curDes; curDes != Evaluators.end(); ++curDes)
  {
   
	  
	  double desirabilty= (*curDes)->CalculateDesirability(dueño);
	

    if (desirabilty >= best)
    {
      best = desirabilty;
      MostDesirable = *curDes;
    }
  }
 
 MostDesirable->SetGoal(dueño);
}
