#include "Bow.h"


Bow::Bow(int dmg, int sp, ISceneManager *sm, ITriangleSelector *ms, IrrlichtDevice *d):RangeWeapon(bow_path, dmg, sp, sm, ms, d, BOW_TYPE)
{
	last_shot = -2000;
}


Bow::~Bow(void)
{
	last_shot = -2000;
}

void Bow::attack(float first_x, float first_y, float last_x, float last_y)
{
	u32 now = device->getTimer()->getTime();
	if(now -last_shot > reload_time)
	{
		this->shoot_anim();
		last_shot = device->getTimer()->getTime();
	}
}
void Bow::finish_animation()
{
	u32 now = device->getTimer()->getTime();
	scene::ISceneManager* sm = device->getSceneManager();

	for (s32 i=0; i<(s32)Impacts.size(); ++i)
		if (now > Impacts[i].when)
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

}