#pragma once
#include "Goal_Think.h"

class Goal_Think;
class Npc;

class MoveToPosition : public Goal
{
public:
	MoveToPosition(Npc*,int);
	~MoveToPosition(void);
	void Activate();
	void AddSubGoal(Goal* g){};
  int Process();

  void Terminate(){estado=completed;}
  Npc* d;
  int whoItem;
};

