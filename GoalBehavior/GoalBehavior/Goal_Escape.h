#pragma once

#include "Goal_Think.h"

class Goal_Think;
class Bot;



class Goal_Escape : public Goal_Composite
{
public:
	Goal_Escape(Bot*);
	~Goal_Escape(void);
	 void Activate();

  int Process();

  void Terminate(){estado=completed;}
  Bot* d;
};

