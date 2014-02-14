#pragma once

#include "Goal_Think.h"

class Goal_Think;
class Npc;

class GoalAtack_Target: public Goal_Composite
{

public:

	~GoalAtack_Target();
	GoalAtack_Target(Npc* dueño);

  void Activate();

  int Process();

  void Terminate(){estado=completed;}
  Npc* d;
	

};

