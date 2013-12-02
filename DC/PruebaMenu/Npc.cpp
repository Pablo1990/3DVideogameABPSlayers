#include "Npc.h"


Npc::Npc(ISceneManager *sm): Character(knight_path, sm)
{
}

Npc::Npc(ISceneManager *sm, Weapon* w): Character(knight_path, sm, w)
{
}


Npc::~Npc(void)
{
}

void Npc::manage_collision(Weapon *w)
{
	if(w != NULL && !w->get_collision_flag() && w->is_animated())
	{
		if(this->detect_collision(w->get_weapon_node(), this->head))
		{
			w->set_collision_flag(true);
		}
		else if(this->detect_collision(w->get_weapon_node(), this->body))
		{
			w->set_collision_flag(true);
		}
		else if(this->detect_collision(w->get_weapon_node(), this->extremity))
		{
			w->set_collision_flag(true);
		}
	}
}