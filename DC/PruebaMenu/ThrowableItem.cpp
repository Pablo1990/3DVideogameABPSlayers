#include "ThrowableItem.h"


ThrowableItem::ThrowableItem(ISceneManager *sm, ITriangleSelector *ms, IrrlichtDevice *d, int typ)
	:RangeWeapon(red_shroom_path, 0, 0, sm, ms, d, RED_SHROOM_TYPE,red_shroom_path)
{
	shot = false;
	switch(typ)
	{
	case RED_SHROOM:
		this->weapon_mesh = sm->getMesh(red_shroom_path); 
		this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
		this->ty = RED_SHROOM_TYPE;
		ammo_mesh = sm->getMesh(red_shroom_path);
		break;
		
	case BLUE_SHROOM:
		this->weapon_mesh = sm->getMesh(blue_shroom_path); 
		this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
		this->ty = BLUE_SHROOM_TYPE;
		ammo_mesh = sm->getMesh(blue_shroom_path);
		break;

	case YELLOW_SHROOM:
		this->weapon_mesh = sm->getMesh(yellow_shroom_path); 
		this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
		this->ty = YELLOW_SHROOM_TYPE;
		ammo_mesh = sm->getMesh(yellow_shroom_path);
		break;
	}
}


ThrowableItem::~ThrowableItem(void)
{

}


void ThrowableItem::attack(float first_x, float first_y, float last_x, float last_y)
{
	if(!shot)
	{
		this->shoot_anim(weapon_node->getScale());
		shot = true;
	}
}
