#pragma once

#include "Goal_Think.h"

class Goal_Think;
class Bot;

class GoalAtack_Target: public Goal_Composite
{

public:

	~GoalAtack_Target();
	GoalAtack_Target(Bot* due�o);

  void Activate();

  int Process();

  void Terminate(){estado=completed;}
  Bot* d;
	

};

