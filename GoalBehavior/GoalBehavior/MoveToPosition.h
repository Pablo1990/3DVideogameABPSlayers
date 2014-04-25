#pragma once
#include "Goal_Think.h"

class Goal_Think;
class Bot;

class MoveToPosition : public Goal
{
public:
	MoveToPosition(Bot*,int);
	~MoveToPosition(void);
	void Activate();
	void AddSubGoal(Goal* g){};
  int Process();

  void Terminate(){estado=completed;}
  Bot* d;
  int whoItem;
};

