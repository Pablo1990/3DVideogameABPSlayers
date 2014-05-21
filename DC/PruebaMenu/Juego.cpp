#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego(video::E_DRIVER_TYPE d, int w, int h, bool f, float v)
{
	this->driverType = d;
	this->skyboxNode = 0;
	this->types = new double[6];
	crouch = false;
	this->armas = new std::list<Weapon*>;
	paused = false;
	level = 0;
	this->width = w;
	this->height = h;
	this->fullscreen = f;
	this->volume = v;
	this->selected_weapon = 0;
}
// Values used to identify individual GUI elements


Juego::~Juego(void)
{


	if(sound)
	{
		delete sound;
		sound = 0;
	} 

	if(pf)
	{
		delete pf;
		pf = 0;
	}

	if(hud)
	{
		delete hud;
		hud = 0;
	}

	if(statusText)
	{
		statusText->remove();
		statusText = 0;
	}

	if(statusText2)
	{
		statusText2->remove();
		statusText2 = 0;
	}

	if(campFire)
	{
		campFire->getParent()->removeChild(campFire);
		campFire = 0;
	}

	if(heal_camp)
	{
		heal_camp->getParent()->removeChild(heal_camp);
		heal_camp = 0;
	}


	if(skyboxNode)
	{
		skyboxNode->getParent()->removeChild(skyboxNode);
		skyboxNode = 0;
	}

	if(npc)
	{
		delete npc;
		npc = 0;
	}

	if(mente)
	{
		delete mente;
		mente = 0;
	}

	if(player)
	{
		delete player;
		player = 0;
	}

	if(selector)
	{
		selector->drop();
		selector = 0;
	}

	if(collider)
	{
		collider->drop();
		collider = 0;
	}

	if(armas)
	{
		std::list<Weapon*>::iterator it;
		for(it = armas->begin(); it != armas->end(); it++)
		{
			if((*it))
			{
				delete (*it);
				(*it) = 0;
			}
		}
		delete armas;
	}

	if(types)
	{
		delete[] types;
		types = 0;
	}


	if(camera)
	{
		camera->getParent()->removeChild(camera);
		camera = 0;
	}

	if (mapSelector)
	{
		mapSelector->drop();
		mapSelector = 0;
	}

	if (metaSelector)
	{
		metaSelector->drop();
		metaSelector = 0;
	}

		if(metaModelSelector)
	{
		metaModelSelector->removeAllTriangleSelectors();
		metaModelSelector->drop();
		metaModelSelector = 0;
	}

	if(quakeLevelMesh)
	{
		quakeLevelMesh->drop();
		quakeLevelMesh = 0;
	}

	if(quakeLevelNode)
	{
		quakeLevelNode->remove();
		quakeLevelNode = 0;
	}
	
	
	if(device)
	{
		device->closeDevice();
		device->run();
	}
	
}

void Juego::setEstado(int es)
{
	estado=es;
}

int Juego::getEstado()
{
	return estado;
}

void Juego::switch_to_next_level()
{
	win_condition = 0;
	this->cycles = 0;
	device->getTimer()->setTime(0);
	ISceneManager* sm = device->getSceneManager();
	if(npc)
		delete npc;

	npc = new Npc(sm,  new Sword(4,7,sm),heal_camp->getPosition(), device, mapSelector);
	npc->set_health(100);
	npc->set_pathfinding(pf);
	npc->add_to_scene(core::vector3df(100,10,100), core::vector3df(0, 270, 0), core::vector3df(0.55, 0.55, 0.55));
	npc->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df( 0.02, 0.02, 0.02));

	camera->bindTargetAndRotation(true);
	camera->setFarValue(5000.0f);

	
	if(player)
		delete player;

	player = new Player(sm, mapSelector, camera);
	player->set_health(100);
	switch(selected_weapon)
	{
		case 1:
			player->pick_weapon(camera, SPEAR_TYPE, device);
			break;
		case 2:
			player->pick_weapon(camera, BOW_TYPE, device);
			break;
		default:
			player->pick_weapon(camera, SWORD_TYPE, device);
			break;
	}

	player->add_to_camera(vector3df(30, -70, 20/*-15*/), vector3df(0,180,0), vector3df(0.55, 0.55, 0.55), camera);


	camera->setPosition(core::vector3df(1000,80,1000));


	if(mente)
		delete mente;

	npc->setEnem(player);
	mente=new Goal_Think(this->level);
	npc->setBrain(mente);
	mente->setDueño(npc);
	

	replace_random_item(device, mapSelector);
	player->set_types(types);
	npc->setItems(armas, types);

}

void Juego::run()
{
	
	bool collision_flag = false;
	core::dimension2d<u32> resolution(width, height);
	
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=this->fullscreen;
	params.EventReceiver = this;

	device = createDeviceEx(params);

	device->getTimer()->setTime(0);
	
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

	wchar_t tmp[255];
	
	backColor.set(255,90,90,156);
	
	loadSceneData();
	switchToNextScene();
	s32 timeForThisScene = -1;


	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
	core::line3d<f32> ray;

		
		
	if(estado==1 || estado == 4)
	{
		
	
		
	int unavez=0;

	hud=new Hud(device, sound);
	hud->drawHud(device,npc,player);
	hud->drawMenu(device);
	hud->borrarMenu(device);
	hud->set_level(this->level);

	
	cntinue = true;

	cycles = 0;
	sound->set_volume(volume);
	if(sound)
		sound->play_background();

	win_condition = 0;
	device->setEventReceiver(this);
	while(device->run() && driver && cntinue)
	{
		if (device->isWindowActive())
		{
			if(win_condition == 0)

			if(!paused)
			{


				player->movement(camera);
				if(player->get_weapon())
					player->get_weapon()->finish_animation();

				if(npc && win_condition == 0)
				{
					npc->manage_collision(player->get_weapon(), device, sound);
					npc->heal_or_fire(campFire, heal_camp, device);

					if(cycles % 1500 && !npc->get_is_dead())
					{
						mente->Arbitrate();
						mente->ProcessSubgoals();
					}

				
					npc->restore_condition(device);
				
					if(npc->get_weapon())
					{
						npc->get_weapon()->finish_animation();
					}

					if(npc->get_is_dead() && win_condition == 0)
					{
						npc->die(device);
						npc->remove_character_node();
						this->win_condition = 1;
						this->level++;
						GameData gd;
						gd.save_game(this->level);
						if(sound)
							sound->win_sound();

						paused = true;
						this->camera->setInputReceiverEnabled(false);
						hud->setVisibleHudF();
						this->sound->pause_background_sounds();
						hud->ActivaMenu();
						hud->show_end_menu();
						this->device->getTimer()->stop();
					}
				}
		

				if(player && win_condition == 0)
				{
					player->heal_or_fire(campFire, heal_camp, device);
					player->fall_down(device);
					player->manage_collision(npc->get_weapon(), device);
					player->restore_condition(device);

				
					swprintf(tmp, 255, L"NpcHealth X:%f Y:%f Z:%f -- Nivel: %i -- Resistencia: %i"
						, player->get_position().X, player->get_position().Y, 
						player->get_position().Z, this->level, player->get_resistance());
				
					statusText->setText(tmp);

					if(player->get_is_dead() && win_condition == 0)
					{
						win_condition = -1;
						GameData gd;
						gd.save_game(this->level);
						if(sound)
							sound->lose_sound();
						paused = true;
						this->camera->setInputReceiverEnabled(false);
						hud->setVisibleHudF();
						hud->ActivaMenu();
						this->sound->pause_background_sounds();
						hud->show_end_menu();
						this->device->getTimer()->stop();
					}
				}

					if(cycles % 1500 == 0)
						this->replace_random_item(this->device, this->mapSelector);
					
				
					
		
					hud->setSkinTransparency( guienv->getSkin());
					hud->setHud(npc,player);
					
					if(cycles + 1 == INT_MAX)
						cycles = 0;
					
					
					cycles++;
					
				}
				driver->beginScene(timeForThisScene != -1, true, backColor);
				smgr->drawAll();
				guienv->drawAll();
				driver->endScene();
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
		
		CParams();	

			cycles = 0;
			npc=new Npc(device->getSceneManager(),new Sword(4,7,device->getSceneManager()),heal_camp->getPosition(), device, mapSelector);
			npc->setEnem(player);
			npc->add_to_scene(core::vector3df(100,10,100), core::vector3df(0, 270, 0), core::vector3df(0.55, 0.55, 0.55));
			npc->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df(0.02, 0.02, 0.02));
			npc->setItems(armas, types);
			vector <double> vecPesos = npc->getPesosDeFichero();
			hud=new Hud(device, sound);
			hud->drawHud(device,npc,player);
			hud->drawMenu(device);
			hud->borrarMenu(device);
			hud->set_level(this->level);
			vector<int> SplitPoints = npc->CalculateSplitPoints();
			CGenAlg* m_pGA = new CGenAlg(1,
		CParams::dMutationRate,
		CParams::dCrossoverRate,
		vecPesos.size(), SplitPoints);

		//Get the weights from the GA and insert into the sweepers brains
		vector<SGenome> m_vecThePopulation = m_pGA->GetChromos();

		npc->PutWeights(m_vecThePopulation[0].vecWeights);
		cout<<npc->GetNumberOfWeights()<<endl;
	while(device->run() && driver && cntinue)
	{
		if (device->isWindowActive())
		{


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
				player->manage_collision(npc->get_weapon(), device, sound);
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
		if(estado==1 || estado==3 || estado == 4)
		{
			

			player = new Player(sm, mapSelector, camera);
			switch(selected_weapon)
			{
				case 1:
					player->pick_weapon(camera, SPEAR_TYPE, device);
					break;
				case 2:
					player->pick_weapon(camera, BOW_TYPE, device);
					break;
				default:
					player->pick_weapon(camera, SWORD_TYPE, device);
					break;
			}
			
			player->add_to_camera(vector3df(30, -70, 20/*-15*/), vector3df(0,180,0), vector3df(0.55, 0.55, 0.55), camera);
			player->set_types(types);
	}
		if(estado==1 || estado == 4)
		{
			
			//IA
			npc->setEnem(player);
			mente=new Goal_Think(this->level);
			npc->setBrain(mente);
			mente->setDueño(npc);
		}
			
		
		
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

	if(estado==1 || estado == 4)
	{
	
	npc = new Npc(sm,new Sword(0,0,sm),heal_camp->getPosition(), device, mapSelector);
	
	Position p1(npc->get_position().X, 0, npc->get_position().Z);
	Position p2(npc->get_position().X, 0, npc->get_position().Z);
	pf = new Pathfinding(p1, p2);
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

		Sword *sw3 = new Sword(4,7,sm);
		npc->set_weapon(sw3);
		npc->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df( 0.02, 0.02, 0.02));
		

	}

	//SWORD: position 40, 100, 0; rotation 180, -50, 90; scale 0.02, 0.02, 0.02
	//SPEAR: position 10, 100, -20; rotation 90,-50,90, scale 2.5, 2.5, 2.5
	//BOW: position 40, 100, 0; rotation 180, -50, 90, scale 0.02, 0.02, 0.02

	//Meto armas
		srand((unsigned)time(0)); 

	this->add_random_item(vector3df(100,0,100));
	this->add_random_item(vector3df(1000,0,100));
	this->add_random_item(vector3df(1700,0,100));
	this->add_random_item(vector3df(700,0,1000));
	this->add_random_item(vector3df(100,0,1000));

	types[5] = HEAL_TYPE;

	if(estado==1 || estado == 4 )
		npc->setItems(armas, types);
	
	//Music
	sound = new SoundEffect(game_music_path);
}

bool Juego::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

	if (event.EventType == EET_KEY_INPUT_EVENT &&
		event.KeyInput.Key == KEY_ESCAPE &&
		event.KeyInput.PressedDown == false && estado != 2)
	{
		// user wants to quit.
		
		if(paused)
		{
			paused = false;
			this->device->getCursorControl()->setPosition(0.5f,0.5f);
			this->camera->setInputReceiverEnabled(true);
			this->sound->resume_background_sounds();
			hud->setVisibleHudT();
			hud->borrarMenu(device);
			//this->device->getTimer()->setTime(time_store);
			GameData gd;
			gd.save_config(sound->get_volume(), height, width, fullscreen);
			this->device->getTimer()->start();
		}
		else
		{
			paused = true;
			this->camera->setInputReceiverEnabled(false);
			hud->setVisibleHudF();
			this->sound->pause_background_sounds();
			this->hud->ActivaMenu();
			time_store = this->device->getTimer()->getTime();
			this->device->getTimer()->stop();
		}
	}
	else if(event.EventType == EET_KEY_INPUT_EVENT &&
		event.KeyInput.Key == KEY_KEY_P &&
		event.KeyInput.PressedDown == false && estado != 2)
	{
		if(paused)
		{
			paused = false;
			this->device->getCursorControl()->setPosition(0.5f,0.5f);
			this->camera->setInputReceiverEnabled(true);
			this->sound->resume_background_sounds();
			hud->setVisibleHudT();
			hud->borrarMenu(device);
			//this->device->getTimer()->setTime(time_store);
			GameData gd;
			gd.save_config(sound->get_volume(), height, width, fullscreen);
			this->device->getTimer()->start();
		}
		else
		{
			paused = true;
			this->camera->setInputReceiverEnabled(false);
			hud->setVisibleHudF();
			this->sound->pause_background_sounds();
			this->hud->ActivaMenu();
			time_store = this->device->getTimer()->getTime();
			this->device->getTimer()->stop();
		}
	}
	else if(estado != 2 && event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_G && event.KeyInput.PressedDown == true && player != NULL && !paused)
	{
		/*camera->removeChild(gun);
		gun = NULL;*/

		player->drop_weapon(camera);
	}
	else if(estado != 2 && event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_E && event.KeyInput.PressedDown == true && !paused)
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
						
					}
				}

			}
		}
		catch(...)
		{}
	}
	else if(estado != 2 && event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_LCONTROL && event.KeyInput.PressedDown == true && !paused)
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
	else if(estado != 2 && event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_LCONTROL && event.KeyInput.PressedDown == false && !paused)
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
	else if(estado != 2 && event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_KEY_J && !paused)
	{
		player->reset_fall_time(this->device);
	}
	else if (estado != 2 && event.EventType == EET_MOUSE_INPUT_EVENT &&
		 event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP && !paused)
	{
		if(player)
		{
			lastX = player->get_weapon()->get_absolute_position().X;
			lastY = player->get_weapon()->get_absolute_position().Z;
			player->attack(firstX, firstY, lastX, lastY);
		}
		
	}else if(estado != 2 && (event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) && !paused)
	{
		if(player)
		{
			firstX = player->get_weapon()->get_absolute_position().X;
			firstY = player->get_weapon()->get_absolute_position().Z;
		}

	}
	else if (estado != 2 && event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP && !paused)
	{
		if(player)
			player->no_defend();
	}
	else
	if(estado != 2 && (event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) && !paused)
	{
		if(player)
			player->defend();
	}
	else
	if (estado != 2 && event.EventType == EET_GUI_EVENT && paused)
    {
		GameData gd;
        s32 id = event.GUIEvent.Caller->getID();
		switch(event.GUIEvent.EventType)
        {
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
					case GUI_ID_CONTINUAR_BUTTON:
						if(paused)
						{
							paused = false;
							this->device->getCursorControl()->setPosition(0.5f,0.5f);
							this->camera->setInputReceiverEnabled(true);
							this->sound->resume_background_sounds();
							hud->setVisibleHudT();
							//this->device->getTimer()->setTime(time_store);
							hud->borrarMenu(device);
							gd.save_config(sound->get_volume(), height, width, fullscreen);
							this->device->getTimer()->start();
		
						}
						break;
					case GUI_ID_MENU_BUTTON:
						this->estado = 0;
						cntinue = false;
						gd.save_config(sound->get_volume(), height, width, fullscreen);
						this->device->getTimer()->start();
						break;
					case GUI_ID_QUIT_BUTTON:
						this->estado = -1;
						cntinue = false;
						gd.save_config(sound->get_volume(), height, width, fullscreen);
						this->device->getTimer()->start();
					break;
					case GUI_ID_OPCIONES_BUTTON:
						hud->show_audio_menu();
						break;
					case GUI_ID_VOLVER_BUTTON:
						hud->show_main_buttons();
						break;
					case GUI_ID_NEXT_LEVEL:
						this->switch_to_next_level();
						paused = false;
						this->device->getCursorControl()->setPosition(0.5f,0.5f);
						this->camera->setInputReceiverEnabled(true);
						this->sound->resume_background_sounds();
						hud->setVisibleHudT();
						//this->device->getTimer()->setTime(time_store);
						hud->borrarMenu(device);
						hud->set_level(this->level);
						this->device->getTimer()->start();
						break;
				}
			break;
			case EGET_SCROLL_BAR_CHANGED:
				if(id == GUI_ID_VOLUME_SCROLLBAR)
				{
					int pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					sound->set_volume(pos / 100.0);
				}
				break;
		}
		return false;
	}
	else
	if (device && device->getSceneManager()->getActiveCamera() && !paused)
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
			if((*it)!=NULL){
				if((*it)->no_weapon())
				{
					vector3df position = (*it)->get_main_position();
			
			

				
					delete (*it);
					(*it) = 0;
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
			}
		}
	}
	catch(...)
	{}
	
}

void Juego::set_level(int lvl)
{
	this->level = lvl;
}

void Juego::set_weapon(int w)
{
	this->selected_weapon = w;
}