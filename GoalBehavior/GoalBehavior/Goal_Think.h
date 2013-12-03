#pragma once
#include "Goal_Composite.h";
class Bot;
class Goal_Think : public Goal_Composite
{
public:
	void Activate();
	void Terminate();
	int Process();
	void Arbitrate();
	bool Present(int type);//Comprobaremos todos nuestros subojetivos para comprobar si esta ya presente(true) o no(false).


	Goal_Think(void);
	~Goal_Think(void);
private:
		Bot* dueño;

};