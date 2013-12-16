#pragma once

#include "Goal_Composite.h";
#include "Goal_Evaluator.h";


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
	Goal_Think(void);
	~Goal_Think(void);
private:
		Bot* dueño;
		list<Goal_Evaluator*> Evaluators;

};