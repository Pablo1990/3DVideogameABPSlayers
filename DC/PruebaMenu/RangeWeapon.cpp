#include "RangeWeapon.h"


RangeWeapon::RangeWeapon(const char* path, int dmg, int sp, ISceneManager *sm, ITriangleSelector* ms, 	IrrlichtDevice *d, int ty):Weapon(path, dmg, sp, sm, ty)
{
	mapSelector = ms;
	device = d;

	// create camp fire
	IVideoDriver* driver = device->getVideoDriver();
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
	campFire->setMaterialTexture(0, driver->getTexture("../media/fireball.bmp"));
	campFire->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

}


RangeWeapon::~RangeWeapon(void)
{
}

void RangeWeapon::finish_animation()
{
}

void RangeWeapon::attack(float first_x, float first_y, float last_x, float last_y)
{
}

void RangeWeapon::shoot_anim()
{
	scene::ICameraSceneNode* camera = this->scene_manager->getActiveCamera();

	if (!camera )
		return;

	SParticleImpact imp;
	imp.when = 0;

	// get line of camera

	core::vector3df start = camera->getPosition();
	core::vector3df end = (camera->getTarget() - start);
	end.normalize();
	start += end*8.0f;
	end = start + (end * camera->getFarValue());

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
	}
	else
	{
		// doesnt collide with wall
		core::vector3df start = camera->getPosition();
		core::vector3df end = (camera->getTarget() - start);
		end.normalize();
		start += end*8.0f;
		end = start + (end * camera->getFarValue());
	}

	// create fire ball
	scene::ISceneNode* node = 0;
	node = scene_manager->addBillboardSceneNode(0,
		core::dimension2d<f32>(25,25), start);

	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/fireball.bmp"));
	node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	
	f32 length = (f32)(end - start).getLength();
	const f32 speed = 0.6f;
	u32 time = (u32)(length / speed);

	scene::ISceneNodeAnimator* anim = 0;

	// set flight line

	anim = scene_manager->createFlyStraightAnimator(start, end, time);
	node->addAnimator(anim);
	anim->drop();

	anim = scene_manager->createDeleteAnimator(time);
	node->addAnimator(anim);
	anim->drop();

	if (imp.when)
	{
		// create impact note
		imp.when = device->getTimer()->getTime() + (time - 100);
		Impacts.push_back(imp);
		
	}
}
