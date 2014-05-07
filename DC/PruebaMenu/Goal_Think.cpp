
#include "Goal_Think.h"
#include "EscapeGoal_Evaluator.h";
#include "AttackTargetGoal_Evaluator.h"
#include "GetHealthGoal_Evaluator.h"
#include "GetWeaponGoal_Evaluator.h"
#include "ExploreGoal_Evaluator.h"



Goal_Think::Goal_Think(int lvl)
{
	EscapeGoal_Evaluator *e=new EscapeGoal_Evaluator(lvl);
	GetWeaponGoal_Evaluator *e2=new GetWeaponGoal_Evaluator(lvl);
	AttackTargetGoal_Evaluator *e3=new AttackTargetGoal_Evaluator(lvl);
	ExploreGoal_Evaluator *e4=new ExploreGoal_Evaluator(lvl);
	GetHealthGoal_Evaluator *e5=new GetHealthGoal_Evaluator(lvl);

	Evaluators.push_back(e);
	Evaluators.push_back(e2);
	Evaluators.push_back(e3);
	Evaluators.push_back(e4);
	Evaluators.push_back(e5);

}


Goal_Think::~Goal_Think(void)
{
	std::list<Goal_Evaluator*>::iterator it;
	for(it = Evaluators.begin(); it != Evaluators.end(); it++)
	{
		if((*it))
		{
			delete (*it);
			(*it) = 0;
		}
	}
	Evaluators.clear();
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
void Goal_Think::setDue�o(Npc* d)
{
	due�o=d;
}
void Goal_Think::AddSubgoalGetAtackTarget(Npc *b)
{
	GoalAtack_Target *a=new GoalAtack_Target(b);
	AddSubGoal(a);
}
void Goal_Think::AddSubgoalGetExplore(Npc *b)
{
	Goal_Explore *a=new Goal_Explore(b);
	AddSubGoal(a);
}
void Goal_Think::AddSubgoalGetItem(Npc *b,int tipoitem)
{
	Goal_GetItem *a=new Goal_GetItem(b,tipoitem);
	AddSubGoal(a);
}
void  Goal_Think::AddSubgoalEscape(Npc *b)
{
	Goal_Escape *e=new Goal_Escape(b);
	AddSubGoal(e);

}
void Goal_Think::Arbitrate()
{
  double best = 0;
  Goal_Evaluator* MostDesirable = NULL;
  
  //iterate through all the evaluators to see which produces the highest score
 std::list<Goal_Evaluator*>::iterator curDes = Evaluators.begin();
 
  for (curDes; curDes != Evaluators.end(); ++curDes)
  {
   
	  
	  double desirabilty= (*curDes)->CalculateDesirability(due�o);

    if (desirabilty >= best)
    {
      best = desirabilty;
      MostDesirable = *curDes;
    }
  }
 
 MostDesirable->SetGoal(due�o);
}
