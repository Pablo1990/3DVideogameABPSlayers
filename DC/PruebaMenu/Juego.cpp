#include "Juego.h"


Juego::Juego(video::E_DRIVER_TYPE d)
{
	this->driverType = d;
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
	core::dimension2d<u32> resolution ( 1366, 768 );
	
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=true;
	params.EventReceiver = this;

	device = createDeviceEx(params);
	if (!device)
		return;

	if (device->getFileSystem()->existFile("map-20kdm2.pk3"))
		device->getFileSystem()->addFileArchive("map-20kdm2.pk3");
	else
		device->getFileSystem()->addFileArchive("../media/map-20kdm2.pk3");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	device->getCursorControl()->setVisible(false);


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

			if(anim1 != NULL)
			{
				if(anim1->hasFinished())
				{
					
					gun->setPosition(core::vector3df(15,-10,20));
					gun->setRotation(core::vector3df(0,50,90));
					gun->removeAnimators();
					anim1->drop();
					anim1=NULL;
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

	camera = sm->addCameraSceneNodeFPS(0, 100.0f, .4f, -1, keyMap, 9, false, 3.f);
			
	camera->setPosition(core::vector3df(108,140,-140));
	camera->setFarValue(5000.0f);

	gun = sm->addAnimatedMeshSceneNode(gunmesh, camera, -1);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves

	gun->setScale(core::vector3df(0.008,0.008,0.008));
	gun->setPosition(core::vector3df(15,-10,20)); 
	gun->setRotation(core::vector3df(0,50,90)); 

	scene::ISceneNodeAnimatorCollisionResponse* collider =
		sm->createCollisionResponseAnimator(
		metaSelector, camera, core::vector3df(25,50,25),
		core::vector3df(0, quakeLevelMesh ? -10.f : 0.0f,0),
			core::vector3df(0,45,0), 0.005f);


	camera->addAnimator(collider);
	collider->drop();
}


void Juego::loadSceneData()
{
	// load quake level

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* sm = device->getSceneManager();

	// Quake3 Shader controls Z-Writing
	sm->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true);

	quakeLevelMesh = (scene::IQ3LevelMesh*) sm->getMesh("maps/20kdm2.bsp");

	if (quakeLevelMesh)
	{
		u32 i;

		//move all quake level meshes (non-realtime)
		core::matrix4 m;
		m.setTranslation(core::vector3df(-1300,-70,-1249));

		for ( i = 0; i != scene::quake3::E_Q3_MESH_SIZE; ++i )
			sm->getMeshManipulator()->transform(quakeLevelMesh->getMesh(i), m);

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
	}

	gunmesh = sm->getMesh("../media/sword.3DS"); 


	// create meta triangle selector with all triangles selectors in it.
	metaSelector = sm->createMetaTriangleSelector();
	metaSelector->addTriangleSelector(mapSelector);


	// create camp fire

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
	else
	if (event.EventType == EET_MOUSE_INPUT_EVENT &&
		 event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
	{
		//AQUI VA EL MOVIMIENTO DE LA ESPADA
		if(gun->getAnimators().empty())
		{
			
			lastX = gun->getAbsolutePosition().X;
			lastY = gun->getAbsolutePosition().Y;

			int difX, difY;
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
		}
	}else if((event.EventType == EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN))
	{
		firstX = gun->getAbsolutePosition().X;
		firstY = gun->getAbsolutePosition().Y;

	}
	else
	if (device->getSceneManager()->getActiveCamera())
	{
		device->getSceneManager()->getActiveCamera()->OnEvent(event);
		return true;
	}

	return false;
}

