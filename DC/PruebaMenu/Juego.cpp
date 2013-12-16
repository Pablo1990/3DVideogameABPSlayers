#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego(video::E_DRIVER_TYPE d)
{
	this->driverType = d;
	this->skyboxNode = 0;
	this->model2 = 0;
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

			if((int)camera->getPosition().X >= 80 && (int)camera->getPosition().Z <= 605 && (int)camera->getPosition().Z >= 590
				&& (int)camera->getPosition().X <= 106)
			{
				const s32 nowfps = driver->getFPS();

				swprintf(tmp, 255, L"Tengo una piedra");

				statusText->setText(tmp);
			}


			driver->beginScene(timeForThisScene != -1, true, backColor);
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();


		
			
			player->get_weapon()->finish_animation();
			npc->manage_collision(player->get_weapon());
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
			
			if(anim1 != NULL)
			{
				// TODO mejorar esto
				if (collision(gun, model1_head) &&  ! collision_flag)
				{
					head_hit++;
					swprintf(tmp, 255, L"Golpe en la cabeza: %i", head_hit);
					statusText->setText(tmp);
					collision_flag = true;
				}
				else if ((collision(gun, model1_high_spine) || collision(gun, model1_low_spine) ||collision(gun, model1_Neck)) &&  ! collision_flag)
				{
					spine_hit++;
					swprintf(tmp, 255, L"Golpe en el cuerpo: %i", spine_hit);
					statusText->setText(tmp);
					collision_flag = true;
				}
				else if ((collision(gun, model1_pelvis) || collision(gun, model1_RThight) || collision(gun, model1_RFoot) || collision(gun, model1_RCalf)
					|| collision(gun, model1_LThight) || collision(gun, model1_LFoot) || collision(gun, model1_LCalf)
					|| collision(gun,model1_RUpperarm) || collision(gun, model1_RForearm) || collision(gun, model1_RHand)
					|| collision(gun,model1_LUpperarm) || collision(gun, model1_LForearm) || collision(gun, model1_LHand))
					&& ! collision_flag)
				{
					ex_hit++;
					swprintf(tmp, 255, L"Golpe en las extremidades: %i", ex_hit);
					statusText->setText(tmp);
					collision_flag = true;
				}


				if(anim1->hasFinished())
				{	
					gun->setPosition(core::vector3df(15,-10,20));
					gun->setRotation(core::vector3df(0,50,90));
					gun->removeAnimators();
					anim1->drop();
					anim1=NULL;
					collision_flag = false;
				}


		
			}
		}
	}
}

void Juego::switchToNextScene()
{
	camera = 0;
	scene::ISceneManager* sm = device->getSceneManager();


	SKeyMap keyMap[9];
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

	camera = sm->addCameraSceneNodeFPS(0, 100.0f, .4f, ID_IsNotPickable, keyMap, 9, false, 3.f);
			
	camera->setPosition(core::vector3df(108,140,-140));
	camera->setFarValue(5000.0f);
	
	
	Sword *sw2 = new Sword(0,0,sm);
	player = new Player(sm, sw2, mapSelector);
	player->get_weapon()->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,50,90), core::vector3df(0.008,0.008,0.008), camera);
	player->add_to_camera(vector3df(0, -70, -15), vector3df(0,180,0), vector3df(0.55, 0.55, 0.55), camera);
	/*
	gun = sm->addAnimatedMeshSceneNode(gunmesh, camera, -1);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	gun->setDebugDataVisible(scene::EDS_BBOX_ALL);
	
	gun->setScale(core::vector3df(0.008,0.008,0.008));
	gun->setPosition(core::vector3df(15,-10,20)); 
	gun->setRotation(core::vector3df(0,50,90));
	*/
	/*
	if (model1_selector)
	{
		scene::ISceneNodeAnimator* characterCollision =
			sm->createCollisionResponseAnimator(metaModelSelector, camera, core::vector3df(25,50,25),
			core::vector3df(0, model1->getMesh() ? -10.f : 0.0f,0),
			core::vector3df(0,45,0), 0.005f);
		camera->addAnimator(characterCollision);
		characterCollision->drop();
	}*/

	scene::ISceneNodeAnimatorCollisionResponse* collider =
		sm->createCollisionResponseAnimator(
		metaSelector, camera, core::vector3df(25,50,25),
		core::vector3df(0, quakeLevelMesh ? -10.f : 0.0f,0),
			core::vector3df(0,45,0), 0.005f);
	
	camera->addAnimator(collider);
	collider->drop();
}

bool Juego::collision(ISceneNode* a, ISceneNode* b)
{
	return a->getTransformedBoundingBox().
		intersectsWithBox(b->getTransformedBoundingBox());
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

		scene::IAnimatedMesh* mesh = 0;
		mesh = sm->getMesh("../media/knight/mesh/fantasy Knight.x");
		if (mesh)
		{
			model1 = sm->addAnimatedMeshSceneNode(mesh);
			//model1->setDebugDataVisible(scene::EDS_BBOX_ALL);
			if (model1)
			{
				model1->setRotation(core::vector3df(0, 270, 0));
				model1->setScale(core::vector3df(0.55, 0.55, 0.55));
				//model1->setMD2Animation(scene::EMAT_RUN);

				model1->setMaterialFlag(video::EMF_LIGHTING, false);
				//model1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
				model1->addShadowVolumeSceneNode();
				//model1->setLoopMode(false);
				u32 joints = model1->getJointCount();
				//Caja cabeza
				this->model1_head = model1->getJointNode("Bip01_Head");
				//Cajas cuerpo
				this->model1_high_spine = model1->getJointNode("Bip01_Spine1");
				this->model1_low_spine = model1->getJointNode("Bip01_Spine");	
				this->model1_Neck = model1->getJointNode("Bip01_Neck");


				//Cajas pierna derecha
				this->model1_pelvis = model1->getJointNode("Bip01_Pelvis");
				this->model1_RThight = model1->getJointNode("Bip01_R_Thigh");
				this->model1_RCalf = model1->getJointNode("Bip01_R_Calf");
				this->model1_RFoot = model1->getJointNode("Bip01_R_Foot");

				//Cajas pierna izquierda
				this->model1_LThight = model1->getJointNode("Bip01_L_Thigh");
				this->model1_LCalf = model1->getJointNode("Bip01_L_Calf");
				this->model1_LFoot = model1->getJointNode("Bip01_L_Foot");

				//Cajas brazo derecho
				//this->model1_RClavicle = model1->getJointNode("Bip01_R_Clavicle");
				this->model1_RUpperarm = model1->getJointNode("Bip01_R_UpperArm");
				this->model1_RForearm = model1->getJointNode("Bip01_R_Forearm");
				this->model1_RHand = model1->getJointNode("Bip01_R_Hand");


				//Cajas brazo izquierdo
				//this->model1_LClavicle = model1->getJointNode("Bip01_L_Clavicle");
				this->model1_LUpperarm = model1->getJointNode("Bip01_L_UpperArm");
				this->model1_LForearm = model1->getJointNode("Bip01_L_Forearm");
				this->model1_LHand = model1->getJointNode("Bip01_L_Hand");

				model1->setDebugDataVisible(scene::EDS_BBOX_ALL);

				model1_selector = sm->createOctreeTriangleSelector(model1->getMesh(), model1, 128);
				metaModelSelector = sm->createMetaTriangleSelector();
				metaModelSelector->addTriangleSelector(model1_selector);
			}
		}

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

		// create walk-between-portals animation
		core::vector3df waypoint[2];
		waypoint[0].set(-250, 0, 100);
		waypoint[1].set(550, 0, 100);

		/*if (model1)
		{
			anim = device->getSceneManager()->createFlyStraightAnimator(
				waypoint[0], waypoint[1], 8000, true);
			model1->addAnimator(anim);
			//anim->drop();

			model1->setRotation(core::vector3df(0, 90, 0));
			anim = device->getSceneManager()->createFlyStraightAnimator(
				waypoint[1], waypoint[0], 8000, true);
			model1->addAnimator(anim);
			anim->drop();
		}*/
	}

	dropped_sword = new Sword(0,0,sm);
	dropped_sword->add_to_scene(core::vector3df(180,70,180), core::vector3df(0,0,0), core::vector3df(0.008,0.008,0.008), true);

	

	gunmesh = sm->getMesh("../media/sword.3DS"); 
	gunmesh->setMaterialFlag(video::EMF_LIGHTING, false);
	droppedGun = sm->addAnimatedMeshSceneNode(gunmesh,0, IDFlag_IsPickable);
	if(droppedGun)
	{
		droppedGun->setScale(core::vector3df(0.008,0.008,0.008));
		droppedGun->setPosition(core::vector3df(140,40,120)); 
		selector = sm->createTriangleSelector(droppedGun);
		droppedGun->setTriangleSelector(selector);
		selector->drop();
	}
		
		

	// create meta triangle selector with all triangles selectors in it.
	metaSelector = sm->createMetaTriangleSelector();
	metaSelector->addTriangleSelector(mapSelector);

	// create camp fire

	campFire = sm->addParticleSystemSceneNode(false);
	campFire->setPosition(core::vector3df(100,70,600));
	campFire->setScale(core::vector3df(20,20,20));

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


	dropped_bow = new Bow(0,0,sm, mapSelector, device);
	dropped_bow->add_to_scene(core::vector3df(230,70,180), core::vector3df(90,0,0), core::vector3df(0.05,0.05,0.05), true);
	//dropped_bow->add_to_scene(core::vector3df(230,70,180), core::vector3df(90,0,0), core::vector3df(1,1,1), true);
	//dropped_bow->get_weapon_node()->setMaterialTexture(0,driver->getTexture("../media/azul.jpg"));

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::RED_SHROOM);
	dropped_red_shroom->add_to_scene(core::vector3df(280,70,180), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true);
	
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
				/*gun = smgr->addAnimatedMeshSceneNode(gunmesh, camera, -1);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	
				gun->setScale(core::vector3df(0.008,0.008,0.008));
				gun->setPosition(core::vector3df(15,-10,20)); 
				gun->setRotation(core::vector3df(0,50,90));				*/
				player->pick_weapon(camera, selectedSceneNode, device);
			}

		}
	}
	else if (event.EventType == EET_MOUSE_INPUT_EVENT &&
		 event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
	{

		lastX = player->get_weapon()->get_absolute_position().X;
		lastY = player->get_weapon()->get_absolute_position().Y;
		player->attack(firstX, firstY, lastX, lastY);
		//AQUI VA EL MOVIMIENTO DE LA ESPADA
		/*if(gun != NULL && gun->getAnimators().empty())
		{
			
			lastX = gun->getAbsolutePosition().X;
			lastY = gun->getAbsolutePosition().Y;

			float difX, difY;
			difX = abs(abs(firstX) - abs(lastX));
			difY = abs(abs(firstY) - abs(lastY));

			if(difY < 3 && difX < 3)
			{
				gun->setRotation(core::vector3df(0,0,0));
				scene::ISceneNodeAnimator* anim = 0;
				anim1 = 0;

				anim1 = device->getSceneManager()->createFlyStraightAnimator(
				gun->getPosition(), core::vector3df(gun->getPosition().X, gun->getPosition().Y, gun->getPosition().Z + 20), 150, false, true);

				gun->setLoopMode(false);

				gun->addAnimator(anim1);
			}
			else
			{
				if(difY < difX)//Estocada lateral
				{
					if(firstX < lastX) //Hacia la derecha
					{
						gun->setRotation(core::vector3df(10,0,0));
						anim1 = 0;
		
		

						anim1 = device->getSceneManager()->createFlyStraightAnimator(
						core::vector3df(gun->getPosition().X - 30, gun->getPosition().Y , gun->getPosition().Z), core::vector3df(gun->getPosition().X + 40, gun->getPosition().Y , gun->getPosition().Z), 300, false, true);

						gun->setLoopMode(false);
			
						gun->addAnimator(anim1);

					}
					else//Hacia la izquierda
					{
						gun->setRotation(core::vector3df(10,0,0));
						anim1 = 0;
		
		

						anim1 = device->getSceneManager()->createFlyStraightAnimator(
						core::vector3df(gun->getPosition().X + 30, gun->getPosition().Y , gun->getPosition().Z), core::vector3df(gun->getPosition().X - 40, gun->getPosition().Y , gun->getPosition().Z), 300, false, true);

						gun->setLoopMode(false);
			
						gun->addAnimator(anim1);
					}
				}
				else//Estocada vertical
				{
						gun->setRotation(core::vector3df(0,0,12));
						anim1 = 0;
		
		

						anim1 = device->getSceneManager()->createFlyStraightAnimator(
						core::vector3df(gun->getPosition().X, gun->getPosition().Y + 40 , gun->getPosition().Z), core::vector3df(gun->getPosition().X, gun->getPosition().Y - 30, gun->getPosition().Z), 300, false, true);

						gun->setLoopMode(false);
			
						gun->addAnimator(anim1);
				}
			}
		}*/
	}else if((event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN))
	{
	
		firstX = player->get_weapon()->get_absolute_position().X;
		firstY = player->get_weapon()->get_absolute_position().Y;

	}
	else
	if (device->getSceneManager()->getActiveCamera())
	{
		device->getSceneManager()->getActiveCamera()->OnEvent(event);
		return true;
	}

	return false;
}

