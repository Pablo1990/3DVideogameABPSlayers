#pragma once

#include "Goal_Think.h"

class Goal_Think;
class Npc;



class Goal_Escape : public Goal_Composite
{
public:
	Goal_Escape(Npc*);
	~Goal_Escape(void);
	 void Activate();

  int Process();

  void Terminate(){estado=completed;}
  Npc* d;
};

