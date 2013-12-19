#pragma once


#include "Goal_Think.h"
#include <math.h>

class Goal_Think;
class Bot;

class Goal_RealizarAtaque : public Goal
{
public:
	Goal_RealizarAtaque(Bot *);
	~Goal_RealizarAtaque(void);
	void AddSubGoal(Goal* g){};
	 void Activate();
	int Process();
  void Terminate(){estado=completed;}
  Bot* d;
};

