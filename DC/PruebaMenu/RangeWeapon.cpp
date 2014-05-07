#include "RangeWeapon.h"


RangeWeapon::RangeWeapon(const char* path, int dmg, int sp, ISceneManager *sm, ITriangleSelector* ms, 	IrrlichtDevice *d, int ty, const char* ammo_path):Weapon(path, dmg, sp, sm, ty)
{
	mapSelector = ms;
	device = d;

	//Obtener mesh
	this->ammo_mesh = sm->getMesh(ammo_path);

	try
	{
		campFire = sm->addParticleSystemSceneNode(false);
		campFire->setPosition(core::vector3df(100,120,600));
		campFire->setScale(core::vector3df(2,2,2));

		scene::IParticleEmitter* em = campFire->createBoxEmitter(
			core::aabbox3d<f32>(-7,0,-7,7,1,7),
			core::vector3df(0.0f,0.06f,0.0f),
			80,100, video::SColor(1,255,255,255),video::SColor(1,255,255,255), 800,2000);

		em->setMinStartSize(core::dimension2d<f32>(20.0f, 10.0f));
		em->setMaxStartSize(core::dimension2d<f32>(20.0f, 10.0f));
		campFire->setEmitter(em);
		em->drop();

		scene::IParticleAffector* paf = campFire->createFadeOutParticleAffector();
		campFire->addAffector(paf);
		paf->drop();

		campFire->setMaterialFlag(video::EMF_LIGHTING, false);
		campFire->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		campFire->setMaterialTexture(0, sm->getVideoDriver()->getTexture("../media/fireball.bmp"));
		campFire->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

		campFire->setVisible(false);
		this->distance = 150;
	}
	catch(...)
	{}
}


RangeWeapon::~RangeWeapon(void)
{

}


void RangeWeapon::attack(float first_x, float first_y, float last_x, float last_y)
{

}

void RangeWeapon::shoot_anim(vector3df scale, vector3df rotation, vector3df start, vector3df end, f32 far_value)
{
	if(!scene_manager)
		return;

	//scene::ICameraSceneNode* camera = this->scene_manager->getActiveCamera();



		
	try
	{
		SParticleImpact imp;
		imp.when = 0;

		// get line of camera

		//core::vector3df start = camera->getPosition();
		//core::vector3df end = (camera->getTarget() - start);
		end.normalize();
		start += end*8.0f;
		end = start + (end * far_value);
		
		core::triangle3df triangle;

		core::line3d<f32> line(start, end);

		// get intersection point with map
		scene::ISceneNode* hitNode;
		if (scene_manager->getSceneCollisionManager()->getCollisionPoint(
			line, mapSelector, end, triangle, hitNode))
		{
			// collides with wall
			core::vector3df out = triangle.getNormal();
			out.setLength(0.03f);

			imp.when = 1;
			imp.outVector = out;
			imp.pos = end;
			imp.collision_flag  = false;
			imp.x = start.X;
			imp.z = start.Z;
		}
		else
		{
			// doesnt collide with wall
			//core::vector3df start = camera->getPosition();
			//core::vector3df end = (camera->getTarget() - start);
			end.normalize();
			start += end*8.0f;
			end = start + (end * far_value);
		}

		// create fire ball
		//IAnimatedMeshSceneNode* node = 0;

		imp.node = scene_manager->addAnimatedMeshSceneNode(ammo_mesh,0,-1,start,vector3df(0,0,0), scale);
		//imp.node->setScale(scale);
		//imp.node->setDebugDataVisible(EDS_BBOX_ALL);

		imp.node->setRotation(rotation);

	
		imp.node->setMaterialFlag(video::EMF_LIGHTING, false);
		imp.node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	
		f32 length = (f32)(end - start).getLength();
		const f32 speed = 0.6f;
		u32 time = (u32)(length / speed);

		scene::ISceneNodeAnimator* anim = 0;

		// set flight line
		anim = scene_manager->createFlyStraightAnimator(start, end, time);
		imp.node->addAnimator(anim);
		anim->drop();

		anim = scene_manager->createDeleteAnimator(time);
		imp.node->addAnimator(anim);
		anim->drop();

		if (imp.when)
		{
			// create impact note
			imp.when = device->getTimer()->getTime() + (time - 100);
			Impacts.push_back(imp);
		
		}
	}
	catch(...)
	{}
}

void RangeWeapon::finish_animation()
{

	try
	{
		u32 now = device->getTimer()->getTime();
		scene::ISceneManager* sm = device->getSceneManager();

		for (s32 i=0; i<(s32)Impacts.size(); ++i)
			if (now > Impacts[i].when)
			{
				try
				{
					// create smoke particle system
					scene::IParticleSystemSceneNode* pas = 0;

					pas = sm->addParticleSystemSceneNode(false, 0, -1, Impacts[i].pos);

					pas->setParticleSize(core::dimension2d<f32>(10.0f, 10.0f));

					scene::IParticleEmitter* em = pas->createBoxEmitter(
						core::aabbox3d<f32>(-5,-5,-5,5,5,5),
						Impacts[i].outVector, 20,40, video::SColor(50,255,255,255),video::SColor(50,255,255,255),
						1200,1600, 20);

					pas->setEmitter(em);
					em->drop();

					scene::IParticleAffector* paf = campFire->createFadeOutParticleAffector();
					pas->addAffector(paf);
					paf->drop();

					pas->setMaterialFlag(video::EMF_LIGHTING, false);
					pas->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
					pas->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/smoke.bmp"));
					pas->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

					scene::ISceneNodeAnimator* anim = sm->createDeleteAnimator(2000);
					pas->addAnimator(anim);
					anim->drop();

		

					// delete entry
					Impacts.erase(i);
					i--;
				}
				catch(...)
				{}
			}
	}
	catch(...)
	{
	}
}

bool RangeWeapon::is_animated()
{
	return false;
}

array<SParticleImpact> RangeWeapon::get_impacts()
{
	return Impacts;
}

void RangeWeapon::set_collision_flag(bool flag, int index)
{
	if(index < Impacts.size())
		Impacts[index].collision_flag = flag;
}

void RangeWeapon::set_ammo_mesh(const char* path)
{
	try
	{
		ammo_mesh = scene_manager->getMesh(path);
	}
	catch(...)
	{}

}

void RangeWeapon::attack(int type,IAnimatedMeshSceneNode* node, vector3df player_position)
{
}

void RangeWeapon::set_impact_at(int index, bool flag)
{
	this->Impacts[index].collision_flag = flag;
}

bool RangeWeapon::get_impact_at(int index)
{
	return Impacts[index].collision_flag;
}

ISceneNode* RangeWeapon::get_impact_node_at(int index)
{
	return Impacts[index].node;
}

int RangeWeapon::get_distance_multiplier(int index, int x, int z)
{
	if(sqrt(pow(Impacts[index].x - x, 2) +
		pow(Impacts[index].z - z, 2)) > 2)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}