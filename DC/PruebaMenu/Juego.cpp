#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego(video::E_DRIVER_TYPE d)
{
	this->driverType = d;
	this->skyboxNode = 0;
	crouch = false;
}


Juego::~Juego(void)
{
	if (mapSelector)
		mapSelector->drop();

	if (metaSelector)
		metaSelector->drop();
}

void Juego::run()
{
	bool collision_flag = false;
	core::dimension2d<u32> resolution(1366, 768);
	
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=true;
	params.EventReceiver = this;

	device = createDeviceEx(params);
	if (!device)
		return;

	if (device->getFileSystem()->existFile("test1.pk3"))
		device->getFileSystem()->addFileArchive("test1.pk3");
	else
		device->getFileSystem()->addFileArchive("../media/test1.pk3");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	//device->getCursorControl()->setVisible(false);


	device->setWindowCaption(L"DC Demo");
	core::dimension2d<u32> size = device->getVideoDriver()->getScreenSize();
	const int lwidth = size.Width - 20;
	const int lheight = 16;
	core::rect<int> pos(10, size.Height-lheight-10, 10+lwidth, size.Height-10);

	device->getGUIEnvironment()->addImage(pos);
	statusText = device->getGUIEnvironment()->addStaticText(L"Loading...",	pos, true);
	statusText->setOverrideColor(video::SColor(255,205,200,200));

	s32 now = 0;

	s32 lastfps = 0;
	sceneStartTime = device->getTimer()->getTime();
	wchar_t tmp[255];
	backColor.set(255,90,90,156);

	loadSceneData();
	switchToNextScene();
	s32 timeForThisScene = -1;

	int head_hit = 0;
	int spine_hit = 0;
	int ex_hit = 0;

		scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
		core::line3d<f32> ray;

	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{

			now = device->getTimer()->getTime();


			player->movement(camera);
			if(player->get_weapon())
				player->get_weapon()->finish_animation();

			if(npc)
				npc->manage_collision(player->get_weapon());

			if(player)
				switch(player->heal_or_fire(campFire, heal_camp, device))
				{

					case 0:
						swprintf(tmp, 255, L"NORMAL", head_hit);
						statusText->setText(tmp);
						break;
					case 1:
						swprintf(tmp, 255, L"ARDO", head_hit);
						statusText->setText(tmp);
						break;
				
					case 2:
						swprintf(tmp, 255, L"CURACION", head_hit);
						statusText->setText(tmp);
						break;
				}
			
			driver->beginScene(timeForThisScene != -1, true, backColor);
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();


		
			

			
	
		}
	}
}

void Juego::switchToNextScene()
{
	camera = 0;
	scene::ISceneManager* sm = device->getSceneManager();


	SKeyMap keyMap[10];
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[4].Action = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[6].Action = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;

	keyMap[8].Action = EKA_JUMP_UP;
	keyMap[8].KeyCode = KEY_KEY_J;

	keyMap[9].Action = EKA_CROUCH;
	keyMap[9].KeyCode = KEY_LCONTROL;

	try
	{
		camera = sm->addCameraSceneNodeFPS(0, 100.0f, .4f, ID_IsNotPickable, keyMap, 10, false, 3.f);
		camera->bindTargetAndRotation(true);
		camera->setPosition(core::vector3df(108,140,-140));
		camera->setFarValue(5000.0f);
	
		Sword *sw2 = new Sword(0,0,sm);
		player = new Player(sm, sw2, mapSelector, camera);
		player->get_weapon()->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,50,90), core::vector3df(0.008,0.008,0.008), camera);
		player->add_to_camera(vector3df(30, -70, 20/*-15*/), vector3df(0,180,0), vector3df(0.55, 0.55, 0.55), camera);
	

			collider =
			sm->createCollisionResponseAnimator(
			metaSelector, camera, core::vector3df(25,50,25),
			core::vector3df(0, quakeLevelMesh ? -10.f : 0.0f,0),
				core::vector3df(0,45,0), 0.005f);
	
		camera->addAnimator(collider);
	}
	catch(...)
	{}
}


void Juego::loadSceneData()
{
	// load quake level

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* sm = device->getSceneManager();

	// Quake3 Shader controls Z-Writing
	sm->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true);

	quakeLevelMesh = (scene::IQ3LevelMesh*) sm->getMesh("maps/test1.bsp");

	if (quakeLevelMesh)
	{
		u32 i;

		//move all quake level meshes (non-realtime)
		/*core::matrix4 m;
		m.setTranslation(core::vector3df(-1300,-70,-1249));

		for ( i = 0; i != scene::quake3::E_Q3_MESH_SIZE; ++i )
			sm->getMeshManipulator()->transform(quakeLevelMesh->getMesh(i), m);
		*/

		quakeLevelNode = sm->addOctreeSceneNode(
				quakeLevelMesh->getMesh( scene::quake3::E_Q3_MESH_GEOMETRY));
		if (quakeLevelNode)
		{
			//quakeLevelNode->setPosition(core::vector3df(-1300,-70,-1249));
			quakeLevelNode->setVisible(true);

			// create map triangle selector
			mapSelector = sm->createOctreeTriangleSelector(quakeLevelMesh->getMesh(0),
				quakeLevelNode, 128);

			// if not using shader and no gamma it's better to use more lighting, because
			// quake3 level are usually dark
			quakeLevelNode->setMaterialType ( video::EMT_LIGHTMAP_M4 );

			// set additive blending if wanted
			
		}

		// the additional mesh can be quite huge and is unoptimized
		scene::IMesh * additional_mesh = quakeLevelMesh->getMesh ( scene::quake3::E_Q3_MESH_ITEMS );

		for ( i = 0; i!= additional_mesh->getMeshBufferCount (); ++i )
		{
			scene::IMeshBuffer *meshBuffer = additional_mesh->getMeshBuffer ( i );
			const video::SMaterial &material = meshBuffer->getMaterial();

			//! The ShaderIndex is stored in the material parameter
			s32 shaderIndex = (s32) material.MaterialTypeParam2;

			// the meshbuffer can be rendered without additional support, or it has no shader
			const scene::quake3::IShader *shader = quakeLevelMesh->getShader ( shaderIndex );
			if ( 0 == shader )
			{
				continue;
			}
			// Now add the MeshBuffer(s) with the current Shader to the Manager
			sm->addQuake3SceneNode ( meshBuffer, shader );
		}

		npc = new Npc(sm);
		npc->add_to_scene(core::vector3df(100,70,100), core::vector3df(0, 270, 0), core::vector3df(0.55, 0.55, 0.55));

		

		scene::ISceneNodeAnimator* anim = 0;

		// create sky box
		driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
		skyboxNode = sm->addSkyBoxSceneNode(
			driver->getTexture("../irrlicht-1.8/media/irrlicht2_up.jpg"),
			driver->getTexture("../irrlicht-1.8/media/irrlicht2_dn.jpg"),
			driver->getTexture("../irrlicht-1.8/media/irrlicht2_lf.jpg"),
			driver->getTexture("../irrlicht-1.8/media/irrlicht2_rt.jpg"),
			driver->getTexture("../irrlicht-1.8/media/irrlicht2_ft.jpg"),
			driver->getTexture("../irrlicht-1.8/media/irrlicht2_bk.jpg"));
		driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	
	}
		

	// create meta triangle selector with all triangles selectors in it.
	metaSelector = sm->createMetaTriangleSelector();
	metaSelector->addTriangleSelector(mapSelector);

	// create camp fire

	campFire = sm->addParticleSystemSceneNode(false,0,IDFlag_IsPickable);
	campFire->setPosition(core::vector3df(-500,70,340));
	campFire->setScale(core::vector3df(20,20,20));
	campFire->setName(std::to_string(TORCH_TYPE).c_str());

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

	//Zona de curacion
	
	heal_camp = sm->addParticleSystemSceneNode(false);
	heal_camp->setPosition(core::vector3df(-1000,70,-200));
	heal_camp->setScale(core::vector3df(20,20,20));

	em = heal_camp->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7),
		core::vector3df(0.0f,0.06f,0.0f),
		80,100, video::SColor(1,255,255,255),video::SColor(1,255,255,255), 800,2000);

	em->setMinStartSize(core::dimension2d<f32>(20.0f, 10.0f));
	em->setMaxStartSize(core::dimension2d<f32>(20.0f, 10.0f));
	heal_camp->setEmitter(em);
	em->drop();

	paf = heal_camp->createFadeOutParticleAffector();
	heal_camp->addAffector(paf);
	paf->drop();

	heal_camp->setMaterialFlag(video::EMF_LIGHTING, false);
	heal_camp->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	heal_camp->setMaterialTexture(0, driver->getTexture("../media/particlegreen.jpg"));
	heal_camp->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);


	dropped_sword = new Sword(0,0,sm);
	dropped_sword->add_to_scene(core::vector3df(180,70,180), core::vector3df(0,0,0), core::vector3df(0.008,0.008,0.008), true);

	dropped_bow = new Bow(0,0,sm, mapSelector, device);
	dropped_bow->add_to_scene(core::vector3df(230,70,180), core::vector3df(90,0,0), core::vector3df(0.05,0.05,0.05), true);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::RED_SHROOM);
	dropped_red_shroom->add_to_scene(core::vector3df(280,70,180), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::YELLOW_SHROOM);
	dropped_red_shroom->add_to_scene(core::vector3df(280,70,230), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::BLUE_SHROOM);
	dropped_red_shroom->add_to_scene(core::vector3df(280,70,280), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true);

	dropped_spear = new Spear(0,0,sm);
	dropped_spear->add_to_scene( core::vector3df(330,90,180), core::vector3df(90,0,0), core::vector3df(1.5,1.5,1.5), true);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::STONE);
	dropped_red_shroom->add_to_scene(core::vector3df(280,70,330), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::TORCH);
	dropped_red_shroom->add_to_scene(core::vector3df(-500,70,200), core::vector3df(90,0,0), core::vector3df(4,4,4), true);
	dropped_red_shroom->add_to_scene(core::vector3df(-500,70,500), core::vector3df(90,180,0), core::vector3df(4,4,4), true);

}

bool Juego::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

	if (event.EventType == EET_KEY_INPUT_EVENT &&
		event.KeyInput.Key == KEY_ESCAPE &&
		event.KeyInput.PressedDown == false)
	{
		// user wants to quit.
		device->closeDevice();
	}
	else if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_G && event.KeyInput.PressedDown == true && player != NULL)
	{
		/*camera->removeChild(gun);
		gun = NULL;*/

		player->drop_weapon(camera);
	}
	else if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_E && event.KeyInput.PressedDown == true)
	{
		try
		{
			scene::ISceneManager* smgr = device->getSceneManager();
			scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
			core::line3d<f32> ray;
			ray.start = camera->getPosition();
			ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;
				// This call is all you need to perform ray/triangle collision on every scene node
			// that has a triangle selector, including the Quake level mesh.  It finds the nearest
			// collision point/triangle, and returns the scene node containing that point.
			// Irrlicht provides other types of selection, including ray/triangle selector,
			// ray/box and ellipse/triangle selector, plus associated helpers.
			// See the methods of ISceneCollisionManager
			// Tracks the current intersection point with the level or a mesh
			core::vector3df intersection;
			// Used to show with triangle has been hit
			core::triangle3df hitTriangle;

			scene::IAnimatedMeshSceneNode * selectedSceneNode =
				(IAnimatedMeshSceneNode*)collMan->getSceneNodeAndCollisionPointFromRay(
						ray,
						intersection, // This will be the position of the collision
						hitTriangle, // This will be the triangle hit in the collision
						IDFlag_IsPickable, // This ensures that only nodes that we have
								// set up to be pickable are considered
						0); // Check the entire scene (this is actually the implicit default)

			// If the ray hit anything, move the billboard to the collision position
			// and draw the triangle that was hit.

		

			if(selectedSceneNode)
			{
				if(camera->getPosition().getDistanceFrom(selectedSceneNode->getPosition()) <= 110)
				{
					player->pick_weapon(camera, selectedSceneNode, device);
				}

			}
		}
		catch(...)
		{}
	}
	else if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_LCONTROL && event.KeyInput.PressedDown == true)
	{
		if(!crouch)
		{
			try
			{
				camera->removeAnimator(collider);

				collider =
					device->getSceneManager()->createCollisionResponseAnimator(
					metaSelector, camera, core::vector3df(25,10,25),
					core::vector3df(0, quakeLevelMesh ? -10.f : 0.0f,0),
						core::vector3df(0,45,0), 0.005f);
	
				camera->addAnimator(collider);
				crouch = true;
			}
			catch(...)
			{}
		}
	}
	else if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_LCONTROL && event.KeyInput.PressedDown == false)
	{
		if(crouch)
		{
			try
			{
				camera->removeAnimator(collider);

				collider =
					device->getSceneManager()->createCollisionResponseAnimator(
					metaSelector, camera, core::vector3df(25,40,25),
					core::vector3df(0, quakeLevelMesh ? -10.f : 0.0f,0),
						core::vector3df(0,45,0), 0.005f);
	
				camera->addAnimator(collider);
				camera->setPosition(camera->getPosition() + vector3df(0,40,0));
				crouch = false;
			}
			catch(...)
			{}
		}
	}
	else if (event.EventType == EET_MOUSE_INPUT_EVENT &&
		 event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
	{
		if(player)
		{
			lastX = player->get_weapon()->get_absolute_position().X;
			lastY = player->get_weapon()->get_absolute_position().Y;
			player->attack(firstX, firstY, lastX, lastY);
		}
		
	}else if((event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN))
	{
		if(player)
		{
			firstX = player->get_weapon()->get_absolute_position().X;
			firstY = player->get_weapon()->get_absolute_position().Y;
		}

	}
	else if (event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
	{
		if(player)
			player->no_defend();
	}
	else
	if((event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN))
	{
		if(player)
			player->defend();
	}
	else
	if (device && device->getSceneManager()->getActiveCamera())
	{
		device->getSceneManager()->getActiveCamera()->OnEvent(event);
		return true;
	}

	return false;
}

