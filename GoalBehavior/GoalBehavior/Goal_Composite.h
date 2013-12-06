#include "Goal.h" 
#include <iostream>
#include <list>
using namespace std;

class Goal_Composite : public Goal
{
public:  
	Goal_Composite(){};
	~Goal_Composite(){};
		virtual void Activate()=0;
		virtual int Process()=0;
		virtual void Terminate()=0;
		virtual void AddSubGoal(Goal *g)=0;
private:
		list <Goal* > m_subgoals;


};