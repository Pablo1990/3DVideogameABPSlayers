#include "Juego.h"

#include <iostream>
using namespace std;

Juego::Juego(video::E_DRIVER_TYPE d)
{
	this->driverType = d;
	this->skyboxNode = 0;
	this->types = new double[6];
	crouch = false;
	this->armas = new std::list<Weapon*>;
}
// Values used to identify individual GUI elements


Juego::~Juego(void)
{
	if (mapSelector)
		mapSelector->drop();

	if (metaSelector)
		metaSelector->drop();
}

void Juego::setEstado(int es)
{
	estado=es;
}
int Juego::getEstado()
{
	return estado;
}
void Juego::run()
{
	
	bool collision_flag = false;
	core::dimension2d<u32> resolution(566, 468);
	
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=false;
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
	
	core::rect<int> pos2(80, 150, 80, size.Height-10);
	

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
	int attack_count = 0;
		
		
	if(estado==1)
	{
		
	
		
	int unavez=0;
	hud=new Hud(device);
		hud->drawHud(device,npc,player);
	

	cycles = 0;
	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{


					now = device->getTimer()->getTime();


					player->movement(camera);
					if(player->get_weapon())
						player->get_weapon()->finish_animation();

			if(npc)
			{
				npc->manage_collision(player->get_weapon(), device);
				npc->heal_or_fire(campFire, heal_camp, device);

				if(cycles % 1500 && !npc->get_is_dead())

				{
					mente->Arbitrate();
					mente->ProcessSubgoals();
				}

				
				//npc->way_to(pf.getCamino());
				npc->restore_condition(device);
				//swprintf(tmp, 255, L"NpcHealth %f", player->get_position().Y);
				

				//statusText->setText(tmp);
				//if(player)
					//npc->face_target(player->get_character_node());

				if(npc->get_weapon())
				{
					npc->get_weapon()->finish_animation();

				}

				if(npc->get_is_dead() && npc->get_character_node()->isVisible())
				{
					npc->die(device);
					npc->remove_character_node();
				}
			}
		

			if(player)
			{
				player->heal_or_fire(campFire, heal_camp, device);
				player->fall_down(device);
				player->manage_collision(npc->get_weapon(), device);
				player->restore_condition(device);

				
				swprintf(tmp, 255, L"NpcHealth X:%f Y:%f Z:%f", player->get_position().X, player->get_position().Y, 
					player->get_position().Z);
				
				statusText->setText(tmp);
			}

			if(cycles % 1500 == 0)
			{
				this->replace_random_item(this->device, this->mapSelector);
			}
				/*switch(player->heal_or_fire(campFire, heal_camp, device))
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
				}*/
	
/*
			if(unavez>3 && unavez<51)
			{

				
							case 2:
								swprintf(tmp, 255, L"CURACION", head_hit);
								statusText->setText(tmp);
								break;
						}*/
				
					driver->beginScene(timeForThisScene != -1, true, backColor);
		
					hud->setSkinTransparency( guienv->getSkin());
					hud->setHud(npc,player);
					smgr->drawAll();
					guienv->drawAll();
					driver->endScene();
					if(cycles + 1 == INT_MAX)
					{
						cycles = 0;
					}

					
					cycles++;
					

				}
			}
	}
	else if(estado==2)
	{
		CParams();		
		CController* controller=new CController(NULL,smgr,heal_camp->getAbsolutePosition(),armas,types, mapSelector, device, campFire, heal_camp);
		while(device->run() && driver)
			{
				if (device->isWindowActive())
				{
					
					now = device->getTimer()->getTime();
					controller->Update();
					if(cycles % 150 == 0)
					{
							this->replace_random_item(this->device, this->mapSelector);
					}
					driver->beginScene(timeForThisScene != -1, true, backColor);
					smgr->drawAll();
					guienv->drawAll();
					driver->endScene();
					if(cycles + 1 == INT_MAX)
					{
						cycles = 0;

					}

					cycles++;

				}
			}
	
	}
	else if(estado==3)
	{
			int unavez=0;
		
	

			cycles = 0;
			npc=new Npc(device->getSceneManager(),new Sword(4,7,device->getSceneManager()),heal_camp->getPosition(), device, mapSelector);
			npc->setEnem(player);
			npc->add_to_scene(core::vector3df(100,10,100), core::vector3df(0, 270, 0), core::vector3df(0.55, 0.55, 0.55));
			npc->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df(0.02, 0.02, 0.02));
			npc->setItems(armas, types);
			vector <double> vecPesos = npc->getPesosDeFichero();
			hud=new Hud(device);
			hud->drawHud(device,npc,player);
			CGenAlg* m_pGA = new CGenAlg(1,
		CParams::dMutationRate,
		CParams::dCrossoverRate,
		vecPesos.size());

		//Get the weights from the GA and insert into the sweepers brains
		vector<SGenome> m_vecThePopulation = m_pGA->GetChromos();

		npc->PutWeights(m_vecThePopulation[0].vecWeights);
		cout<<npc->GetNumberOfWeights()<<endl;
	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{


					now = device->getTimer()->getTime();


					player->movement(camera);
					if(player->get_weapon())
						player->get_weapon()->finish_animation();
				
					
			if(npc)
			{
				
				if(!npc->get_is_dead())
				{
					npc->Update();
				}

				npc->restore_condition(device);


				if(npc->get_is_dead() && npc->get_character_node()->isVisible())
				{
					npc->die(device);
					npc->remove_character_node();
				}
			}

			if(player)
			{
				player->heal_or_fire(campFire, heal_camp, device);
				player->fall_down(device);
				player->manage_collision(npc->get_weapon(), device);
				player->restore_condition(device);

				
				swprintf(tmp, 255, L"NpcHealth X:%f Y:%f Z:%f", player->get_position().X, player->get_position().Y, 
					player->get_position().Z);
				
				statusText->setText(tmp);
			}

			if(cycles % 1500 == 0)
			{
				this->replace_random_item(this->device, this->mapSelector);
			}
				
					driver->beginScene(timeForThisScene != -1, true, backColor);
		
					hud->setSkinTransparency( guienv->getSkin());
					hud->setHud(npc,player);
					smgr->drawAll();
					guienv->drawAll();
					driver->endScene();
					if(cycles + 1 == INT_MAX)
					{
						cycles = 0;
					}

					
					cycles++;
					

				}
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
		camera->setPosition(core::vector3df(1000,80,1000));
		camera->setFarValue(5000.0f);
		if(estado==1 || estado==3)
		{
			Sword *sw2 = new Sword(4,7,sm);
			player = new Player(sm, sw2, mapSelector, camera);
			player->get_weapon()->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,50,90), core::vector3df(0.008,0.008,0.008), camera);
			player->add_to_camera(vector3df(30, -70, 20/*-15*/), vector3df(0,180,0), vector3df(0.55, 0.55, 0.55), camera);
			player->set_types(types);
	}
		if(estado==1)
		{
			
			//IA
			npc->setEnem(player);
			mente=new Goal_Think();
			npc->setBrain(mente);
			mente->setDueño(npc);
		}
			
		
		
			collider =
			sm->createCollisionResponseAnimator(
			metaSelector, camera, core::vector3df(25,50,25),
			core::vector3df(0, quakeLevelMesh ? -10.f : 0.0f,0),
				core::vector3df(0,45,0), 0.005f);
	
		camera->addAnimator(collider);
		//((ISceneNodeAnimatorCameraFPS*)camera)->setMoveSpeed();
		//IDEA:
		/* Para la ralentizar el movimiento dividirlo entre dos, para paralizar poner a 0 y ver que pasa.
		   El valor por defecto es .4f */
		
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
	campFire->setPosition(core::vector3df(800,0,700));
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
	heal_camp->setPosition(core::vector3df(1650,0,1000));
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

	if(estado==1)
	{
	
	npc = new Npc(sm,new Sword(0,0,sm),heal_camp->getPosition(), device, mapSelector);
	
	Position p1(npc->get_position().X, 0, npc->get_position().Z);
	Position p2(npc->get_position().X, 0, npc->get_position().Z);
	Pathfinding* pf = new Pathfinding(p1, p2);
	Position last_corner(1894.93, 150, 1294.88);
	
	vector<vector<Position>> obstacles;
	vector<Position> v2;
		vector<Position> bridge;
	vector<Position> obs_1;
	vector<Position> obs_2;
	vector<Position> obs_3; 
	v2.push_back(last_corner);
	//Puente

	//bridge.push_back(Position(347,0,578));
	//bridge.push_back(Position(1290,72,755));

	//////Primer obstaculo
	//Position p3(890,0,137);
	//Position p4(1024,136,289);

	//obs_1.push_back(p3);
	//obs_1.push_back(p4);

	//////Segundo obstaculo
	//obs_2.push_back(Position(890,0,1023));
	//obs_2.push_back(Position(1024,136,1180));

	//////TercerObstaculo
	//obs_3.push_back(Position(1547,0,626));
	//obs_3.push_back(Position(1830,72,720));


	obstacles.push_back(v2);


	////obstacles.push_back(v2);
	////obstacles.push_back(bridge);
	////obstacles.push_back(obs_1);
	//obstacles.push_back(obs_2);
	//obstacles.push_back(obs_3);




	pf->setMapa(obstacles);

	//vector<Position> way_points = pf.AEstrella(500);

	//pf.imprimirCamino();

		npc->set_pathfinding(pf);
		npc->add_to_scene(core::vector3df(100,10,100), core::vector3df(0, 270, 0), core::vector3df(0.55, 0.55, 0.55));

		//Sword *sw3 = new Sword(4,7,sm);
		//Spear *sw3 = new Spear(4,5,sm);
		//Bow *sw3 = new Bow(4,5,sm, mapSelector, device);
	
	
	

			Sword *sw3 = new Sword(4,7,sm);
			//Spear *sw3 = new Spear(4,5,sm);
		//Bow *sw3 = new Bow(4,5,sm, mapSelector, device);
			//ThrowableItem *sw3 = new ThrowableItem(sm, mapSelector, device, ThrowableItem::RED_SHROOM);
	
			npc->set_weapon(sw3);
			npc->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df( 0.02, 0.02, 0.02));
			//npc->get_weapon()->set_resist(0);

	}

	//SWORD: position 40, 100, 0; rotation 180, -50, 90; scale 0.02, 0.02, 0.02
	//SPEAR: position 10, 100, -20; rotation 90,-50,90, scale 2.5, 2.5, 2.5
	//BOW: position 40, 100, 0; rotation 180, -50, 90, scale 0.02, 0.02, 0.02
	//armas =std::list<Weapon*>();
/*	dropped_sword = new Sword(0,0,sm);
	dropped_sword->add_to_scene(core::vector3df(180,5,180), core::vector3df(0,0,0), core::vector3df(0.008,0.008,0.008), true, -1);
	//armas.push_front(dropped_sword);

	dropped_bow = new Bow(0,0,sm, mapSelector, device);
	dropped_bow->add_to_scene(core::vector3df(230,5,180), core::vector3df(90,0,0), core::vector3df(0.05,0.05,0.05), true, -1);
	//armas.push_front(dropped_bow);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::RED_SHROOM);
	dropped_red_shroom->add_to_scene(core::vector3df(280,8,180), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, -1);
	//armas.push_front(dropped_red_shroom);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::YELLOW_SHROOM);
	dropped_red_shroom->add_to_scene(core::vector3df(280,8,230), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, -1);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::BLUE_SHROOM);
	dropped_red_shroom->add_to_scene(core::vector3df(280,8,280), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, -1);

	dropped_spear = new Spear(0,0,sm);
	dropped_spear->add_to_scene( core::vector3df(330,30,180), core::vector3df(90,0,0), core::vector3df(1.5,1.5,1.5), true, -1);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::STONE);
	dropped_red_shroom->add_to_scene(core::vector3df(280,8,330), core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, -1);

	dropped_red_shroom = new ThrowableItem(sm, mapSelector, device, ThrowableItem::TORCH);
	dropped_red_shroom->add_to_scene(core::vector3df(800,8,550), core::vector3df(90,0,0), core::vector3df(4,4,4), true, -1);
	dropped_red_shroom->add_to_scene(core::vector3df(800,8,850), core::vector3df(90,180,0), core::vector3df(4,4,4), true, -1);
	*/

	//Meto armas
		srand((unsigned)time(0)); 

	this->add_random_item(vector3df(100,0,100));
	this->add_random_item(vector3df(1000,0,100));
	this->add_random_item(vector3df(1700,0,100));
	this->add_random_item(vector3df(700,0,1000));
	this->add_random_item(vector3df(100,0,1000));

	types[5] = HEAL_TYPE;

	if(estado==1 )
		npc->setItems(armas, types);
	
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
					if(last_drop == -1 || device->getTimer()->getTime() - last_drop > 3000)
					{

						last_drop = device->getTimer()->getTime(); 
					

						player->pick_weapon(camera, selectedSceneNode, device, armas);
						//cout << "RECOJO EL NODO " << ((std::string)selectedSceneNode->getName()).substr(strcspn(selectedSceneNode->getName(), "_") + 1) << endl;
						//this->replace_random_item(atoi(((std::string)selectedSceneNode->getName()).substr(strcspn(selectedSceneNode->getName(), "_") + 1).c_str()));
					}
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
	else if(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_J)
	{
		player->reset_fall_time(this->device);
	}
	else if (event.EventType == EET_MOUSE_INPUT_EVENT &&
		 event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
	{
		if(player)
		{
			lastX = player->get_weapon()->get_absolute_position().X;
			lastY = player->get_weapon()->get_absolute_position().Z;
			player->attack(firstX, firstY, lastX, lastY);
		}
		
	}else if((event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN))
	{
		if(player)
		{
			firstX = player->get_weapon()->get_absolute_position().X;
			firstY = player->get_weapon()->get_absolute_position().Z;
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

void Juego::add_random_item(vector3df position)
{

	scene::ISceneManager* sm = device->getSceneManager();
	position.Y = 8;

	switch(rand()%7)
	{
		case 0:
			armas->push_back( new Spear(6,5,sm));
			position.Y = 25;
			(*(--armas->end()))->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(1.5,1.5,1.5), true, armas->size() - 1);
			break;
		case 1:
			armas->push_back( new Sword(4,7,sm));
			(*(--armas->end()))->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.008,0.008,0.008), true, armas->size() - 1);
			break;
		case 2:
			armas->push_back(new Bow(4,4,sm, mapSelector, device));
			(*(--armas->end()))->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 3:
			armas->push_back(new ThrowableItem(sm, mapSelector, device, ThrowableItem::RED_SHROOM));
			(*(--armas->end()))->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 4:
			armas->push_back(new ThrowableItem(sm, mapSelector, device, ThrowableItem::BLUE_SHROOM));
			(*(--armas->end()))->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 5:
			armas->push_back(new ThrowableItem(sm, mapSelector, device, ThrowableItem::YELLOW_SHROOM));
			(*(--armas->end()))->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 6:
			armas->push_back(new ThrowableItem(sm, mapSelector, device, ThrowableItem::STONE));
			(*(--armas->end()))->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
	}

	if(armas->size() - 1 < 6 && armas->size() >= 0)
	{
		types[armas->size()] = (*(--armas->end()))->get_type();
	}

}

void Juego::replace_random_item(IrrlichtDevice *device, 	scene::ITriangleSelector* mapSelector)
{
	try
	{
		
		scene::ISceneManager* sm = device->getSceneManager();
		std::list<Weapon*>::iterator it = armas->begin();
		std::list<Weapon*>::iterator it_aux = armas->begin();

		Weapon *w;
		for(int i = 0; i < armas->size(); i++)
		{

			if((*it)->no_weapon())
			{
				vector3df position = (*it)->get_main_position();
			
			
				armas->remove(*it);
				srand((unsigned)time(0)); 
				int r = rand();
				r = r % 7;

			/*	if(i == 0)
				{
					it_aux = armas->begin();
				}
				else
				{
					it_aux++;
				}*/

				it_aux = armas->begin();
				for(int j = 0; j < i; j++)
				{
					it_aux++;
				}
		
				switch(r)
				{
					case 0:
						it_aux = armas->insert(it_aux, new Spear(6,5,sm));
						position.Y = 25;
						(*it_aux)->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(1.5,1.5,1.5), true, armas->size() - 1);
					/*armas.push_back( new Spear(0,0,sm));
					position.Y = 25;
					(*(--armas.end()))->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(1.5,1.5,1.5), true, armas.size() - 1);*/
					break;
				case 1:
					it_aux = armas->insert(it_aux, new Sword(4,7,sm));
					(*it_aux)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.008,0.008,0.008), true, armas->size() - 1);
					break;
				case 2:
					it_aux = armas->insert(it_aux, new Bow(4,4,sm, mapSelector, device));
					(*it_aux)->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
					break;
				case 3:
					it_aux = armas->insert(it_aux, new ThrowableItem(sm, mapSelector, device, ThrowableItem::RED_SHROOM));
					(*it_aux)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
					break;
				case 4:
					it_aux = armas->insert(it_aux, new ThrowableItem(sm, mapSelector, device, ThrowableItem::BLUE_SHROOM));
					(*it_aux)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
					break;
				case 5:
					it_aux = armas->insert(it_aux, new ThrowableItem(sm, mapSelector, device, ThrowableItem::YELLOW_SHROOM));
					(*it_aux)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
					break;
				case 6:
					it_aux = armas->insert(it_aux, new ThrowableItem(sm, mapSelector, device, ThrowableItem::STONE));
					(*it_aux)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
					break;
				}
		
				(*it_aux)->set_main_position(position);
				(*it_aux)->set_no_weapon(false);
				(*it_aux)->get_weapon_node()->setName((std::to_string((*it_aux)->get_type()) + '_' + std::to_string(i)).c_str());
				this->types[i] = (*it_aux)->get_type();

				it = it_aux;
			}
			else
			{
				it++;
			}


			//int number;
			//int type;

			//number = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(strcspn((*it)->get_weapon_node()->getName(), "_") + 1).c_str());
			//type = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(0, strcspn((*it)->get_weapon_node()->getName(), "_")).c_str());
		
		}
			for(it = armas->begin(); it != armas->end(); ++it)
			{
				/*if(i >= index && it != --armas.end())
				{
					number = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(strcspn((*it)->get_weapon_node()->getName(), "_") + 1).c_str());
					number--;
					type = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(0, strcspn((*it)->get_weapon_node()->getName(), "_")).c_str());
					//cout << "DA NAME " << type << "_" << number << endl;
					(*it)->get_weapon_node()->setName((std::to_string(type) + '_' + std::to_string(number)).c_str());
				}*/
//				cout << "DA NAME " << (*it)->get_weapon_node()->getName() << endl;
	

			}
		
	//		cout << "DA NAME " << (*it)->get_weapon_node()->getName() << endl;
		
	}
	catch(...)
	{}
	
}