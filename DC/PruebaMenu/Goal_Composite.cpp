

#include "Goal_Composite.h"
using namespace std;

void Goal_Composite::AddSubGoal(Goal *goal)
{

	m_SubGoals.push_front(goal);
}
int Goal_Composite::ProcessSubgoals()
{
	
  //remove all completed and failed goals from the front of the subgoal list
  while (!m_SubGoals.empty() &&
         (m_SubGoals.front()->iscomplete() || m_SubGoals.front()->hasfailed()))
  {
    m_SubGoals.front()->Terminate();
    delete m_SubGoals.front();
    m_SubGoals.pop_front();
	
  }

  //if any subgoals remain, process the one at the front of the list
  if (!m_SubGoals.empty())
  {
    //grab the status of the frontmost subgoal
    int StatusOfSubGoals = m_SubGoals.front()->Process();

    //we have to test for the special case where the frontmost subgoal
    //reports "completed" and the subgoal list contains additional goals.
    //When this is the case, to ensure the parent keeps processing its
    //subgoal list,the "active" status is returned
	//2 is completed
    if (StatusOfSubGoals == 2 && m_SubGoals.size() > 1)
    {
      return 1;
    }

    return StatusOfSubGoals;
  }

 
  else
  {
	  // completed is 2
	  
	//cout<<"Objetivo Completado siguiente"<<endl;
    return 2;
  }
}


void Goal_Composite::RemoveAllSubgoals()
{
	for (std::list<Goal*>::iterator it = m_SubGoals.begin();
       it != m_SubGoals.end();
       ++it)
  {
    (*it)->Terminate();

    delete *it;
  }

  m_SubGoals.clear();
}

