#include "Goal.h" 
#include <iostream>
#include <list>

using namespace std;

class Goal;

class Goal_Composite : public Goal
{
public:  
	Goal_Composite(){};
	~Goal_Composite(){};
		virtual void Activate()=0;
		virtual int Process()=0;
		virtual void Terminate()=0;
		void AddSubGoal(Goal *g);
		int ProcessSubgoals();
		void RemoveAllSubgoals();
private:
		list <Goal* > m_SubGoals;


};