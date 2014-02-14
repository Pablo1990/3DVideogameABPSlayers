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

class Npc;
class Goal_Evaluator;


class Goal_Think : public Goal_Composite
{
public:
	void Activate();
	void Terminate();
	int Process();
	void Arbitrate();
	bool Present(int type);//Comprobaremos todos nuestros subojetivos para comprobar si esta ya presente(true) o no(false)
	void setDueño(Npc* d);
	void AddSubgoalGetAtackTarget(Npc *);
	void AddSubgoalGetExplore(Npc *b);
	void AddSubgoalEscape(Npc *b);
	void AddSubgoalGetItem(Npc *b,int tipoitem);
	Goal_Think(void);
	~Goal_Think(void);
private:
		Npc* dueño;
		std::list<Goal_Evaluator*> Evaluators;

};