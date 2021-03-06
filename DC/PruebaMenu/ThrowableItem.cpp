#include "ThrowableItem.h"


ThrowableItem::ThrowableItem(ISceneManager *sm, ITriangleSelector *ms, IrrlichtDevice *d, int typ)
	:RangeWeapon(red_shroom_path, 0, 0, sm, ms, d, RED_SHROOM_TYPE,red_shroom_path)
{
	try
	{
		shot = false;
		shield = true;
		switch(typ)
		{
		case RED_SHROOM:
			this->weapon_mesh = sm->getMesh(red_shroom_path); 
			this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
			this->ty = RED_SHROOM_TYPE;
			this->set_ammo_mesh(red_shroom_path);
			break;
		
		case BLUE_SHROOM:
			this->weapon_mesh = sm->getMesh(blue_shroom_path); 
			this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
			this->ty = BLUE_SHROOM_TYPE;
			this->set_ammo_mesh(blue_shroom_path);
			break;

		case YELLOW_SHROOM:
			this->weapon_mesh = sm->getMesh(yellow_shroom_path); 
			this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
			this->ty = YELLOW_SHROOM_TYPE;
			this->set_ammo_mesh(yellow_shroom_path);
			break;

		case STONE:
			this->weapon_mesh = sm->getMesh(stone_path); 
			this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
			this->ty = STONE_TYPE;
			this->set_ammo_mesh(stone_path);
			break;

		case TORCH:
			this->weapon_mesh = sm->getMesh(torch_path); 
			this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
			this->ty = TORCH_TYPE;
			this->set_ammo_mesh(torch_path);
			break;
		}
		this->resist = 15;
	}
	catch(...)
	{}
}


ThrowableItem::~ThrowableItem(void)
{

}


bool ThrowableItem::attack(float first_x, float first_y, float last_x, float last_y)
{
	try
	{
		if(!shot && weapon_node)
		{
			scene::ICameraSceneNode* camera = this->scene_manager->getActiveCamera();



			if (!camera )
				return false;
			f32 far = camera->getFarValue();
			this->shoot_anim(weapon_node->getScale(), camera->getRotation(), camera->getPosition(), 
				camera->getTarget() - camera->getPosition(), camera->getFarValue());
			shot = true;
			resist = 0;
			return true;
		}
		return false;
	}
	catch(...)
	{
		return false;
	}
	return false;
}

bool ThrowableItem::attack(int type, IAnimatedMeshSceneNode* node, vector3df player_position)
{
	if(device)
	{
		if(!shot && weapon_node)
		{
			
			this->shoot_anim(weapon_node->getScale(), node->getRotation(), node->getPosition(), vector3df(player_position.X,
				player_position.Y + 100, player_position.Z), node->getPosition().getDistanceFrom(player_position) );
			this->resist = 0;
			return true;
		}
	}
	return false;
}