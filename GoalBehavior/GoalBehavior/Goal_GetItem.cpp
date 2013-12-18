#include "stdafx.h"
#include "Goal_GetItem.h"


Goal_GetItem::Goal_GetItem(Bot *_p,int _item)
{
	dueño=_p;
	item=_item;
}


Goal_GetItem::~Goal_GetItem(void)
{
}
void Goal_GetItem::Activate()
{

}

  int Goal_GetItem::Process()
  {
	return 0;
  }