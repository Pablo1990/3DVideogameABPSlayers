#pragma once
#include "Bot.h";
template<class Bot>
class Goal
{

public:
	Goal(void);
	~Goal(void);
	virtual void Activate()=0;
	virtual int Process()=0;
	virtual void Terminate()=0;
	virtual void AddSubGoal(Goal* g)=0;

	int getTipo();
	bool isactive();
	bool isinactive();
	bool iscomplete();
	bool hasfailed();
	


private:

	Bot* dueño;
	int estado;//Si es 1 es activado, 2 es completo, 0 inactivo,3 error
	int tipo;
		
};

