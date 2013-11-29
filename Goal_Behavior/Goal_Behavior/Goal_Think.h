#pragma once
#include "Goal_Composite.h" 

class Goal_Think : public Goal_Composite<Bot>
{
public:
	void Activate();
	void Terminate();
	int Process();
	void Arbitrate();

	Goal_Think(void);
	~Goal_Think(void);
};

