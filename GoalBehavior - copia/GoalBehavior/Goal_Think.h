#pragma once

#include "Goal_Composite.h";
#include "Goal_Evaluator.h";
#include "GoalAtack_Target.h"
#include "Goal_Explore.h"
#include "Goal_GetItem.h";
#include "Goal_Escape.h"
#include "MoveToPosition.h";
#include "Goal_GetArma.h";
#include "Goal_GetSalud.h";
#include "Goal_Vagar.h";
#include "Goal_MarcarHuida.h";
#include "Goal_RealizarAtaque.h";

class Bot;
class Goal_Evaluator;


class Goal_Think : public Goal_Composite
{
public:
	void Activate();
	void Terminate();
	int Process();
	void Arbitrate();
	bool Present(int type);//Comprobaremos todos nuestros subojetivos para comprobar si esta ya presente(true) o no(false)
	void setDueño(Bot* d);
	void AddSubgoalGetAtackTarget(Bot *);
	void AddSubgoalGetExplore(Bot *b);
	void AddSubgoalEscape(Bot *b);
	void AddSubgoalGetItem(Bot *b,int tipoitem);
	Goal_Think(void);
	~Goal_Think(void);
private:
		Bot* dueño;
		list<Goal_Evaluator*> Evaluators;

};