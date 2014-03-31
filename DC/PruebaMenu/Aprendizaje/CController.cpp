#include "CController.h"

//---------------------------------------constructor---------------------
//
//	initilaize the npc, their brains and the GA factory
//
//-----------------------------------------------------------------------
CController::CController(HWND hwndMain,ISceneManager *sm, vector3df posHealth,std::list<Weapon*>* armas, double* types, ITriangleSelector* mapSelector,IrrlichtDevice *device,ISceneNode* camp_fire_,ISceneNode* heal_): m_NumNpc(CParams::iNumSweepers), 
	m_pGA(NULL),
	m_iTicks(0),
	m_hwndMain(hwndMain),
	m_iGenerations(0),
	cxClient(CParams::WindowWidth),
	cyClient(CParams::WindowHeight)
{
	//Debemos definiir para cada npc un enemigo
	Position last_corner(1894.93, 1, 1294.88);
	duelosRestantes = 0;
	//let's create the bots
	for (int i=0; i<20; ++i)
	{
		Npc* n = new Npc(sm,new Sword(4,7,sm),posHealth, device, mapSelector,camp_fire_,heal_);	
		n->add_to_scene(core::vector3df(0,200,0), core::vector3df(0, 270, 0), core::vector3df(0.55, 0.55, 0.55));
		n->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df(0.02, 0.02, 0.02));
		n->setItems(armas, types);
		m_vecNpc.push_back(n);
		m_vecNpcHealth.push_back(100);
		m_vecNpcEnemiesHealth.push_back(100);
	}
	firstBlood = false;
	device1=device;
	//asociar enemigos unos con otros
	assignEnemies();

	//get the total number of weights used in the sweepers
	//NN so we can initialise the GA
	m_NumWeightsInNN = m_vecNpc[0]->GetNumberOfWeights();
	
	//initialize the Genetic Algorithm class
	m_pGA = new CGenAlg(m_NumNpc,
		CParams::dMutationRate,
		CParams::dCrossoverRate,
		m_NumWeightsInNN);

	//Get the weights from the GA and insert into the sweepers brains
	m_vecThePopulation = m_pGA->GetChromos();

	for (int i=0; i<m_NumNpc; i++) {
		m_vecNpc[i]->PutWeights(m_vecThePopulation[i].vecWeights);
		invisPlayers(i);
	}
	
	asignarEnemigo(0);


	//create a pen for the graph drawing
	m_BluePen  = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_RedPen   = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_GreenPen = CreatePen(PS_SOLID, 1, RGB(0, 150, 0));

	m_OldPen	= NULL;
}


//--------------------------------------destructor-------------------------------------
//
//--------------------------------------------------------------------------------------
CController::~CController()
{
	if(m_pGA)
	{
		delete		m_pGA;
	}

	DeleteObject(m_BluePen);
	DeleteObject(m_RedPen);
	DeleteObject(m_GreenPen);
	DeleteObject(m_OldPen);
}

void CController::assignEnemies(){
	for(int i=0; i<m_NumNpc; i++){
		for(int j=i; j<m_NumNpc; j++){
			if(j!=i){
				m_vecJornadasVisitantes.push_back(j);
				m_vecJornadasLocales.push_back(i);
				duelosRestantes++;
			}
		}
	}
	
}

void CController::asignarEnemigo(int num){
	for(int i=0; i<m_NumNpc; i++){
		invisPlayers(i);
	}

	m_vecNpc[m_vecJornadasLocales[num]]->setEnem(m_vecNpc[m_vecJornadasVisitantes[num]]);

	m_vecNpc[m_vecJornadasVisitantes[num]]->setEnem(m_vecNpc[m_vecJornadasLocales[num]]);
	
	m_vecNpc[m_vecJornadasLocales[num]]->get_character_node()->setPosition(vector3df(100, 10, 100));
	m_vecNpc[m_vecJornadasVisitantes[num]]->get_character_node()->setPosition(vector3df(200,10,600));
	//m_vecNpc[m_vecJornadasLocales[num]]->set_position(100, 10, 300);
	//m_vecNpc[m_vecJornadasVisitantes[num]]->set_position(1400,10,100);
}

void CController::invisPlayers(int num){
	m_vecNpc[m_vecJornadasLocales[num]]->get_character_node()->setPosition(vector3df(0, 200, 0));
	m_vecNpc[m_vecJornadasVisitantes[num]]->get_character_node()->setPosition(vector3df(0,200,0));
	//m_vecNpc[m_vecJornadasLocales[num]]->set_position(0, 200, 0);
	//m_vecNpc[m_vecJornadasVisitantes[num]]->set_position(0,200,0);
}

void CController::updateFitnessFinal(Npc* uno,Npc* dos)
{
	//Daremos 20 por victoria x victoria se toma que la diferencia con respecto al otro sea mayor
	double diff=uno->get_health()-dos->get_health();
	double difFit=0;
	double premio1=0;
	double premio2=0;
	if(diff<0)
	{
		//Gana el dos
		premio2=premio2+20;
		if(uno->Fitness()>=dos->Fitness())
		{
			difFit=abs(uno->Fitness()-dos->Fitness());
			if(difFit>50)
			{
				premio2=premio2+20;
				premio1=premio1-20;
			}
				
		}
		else
		{
			difFit=abs(uno->Fitness()-dos->Fitness());
			if(difFit>50)
			{
				premio2=premio2-15;
				premio1=premio1-5;
			}		
					
		}
		
	}

	else if(diff>0)
	{
		
		//Gana el uno
		premio1=premio1+20;
		if(uno->Fitness()>=dos->Fitness())
		{
			difFit=abs(uno->Fitness()-dos->Fitness());
			if(difFit>50)
			{
				premio1=premio1-15;
				premio2=premio2-5;
			}
			
				
		}
		else
		{
			difFit=abs(uno->Fitness()-dos->Fitness());
			if(difFit>50)
			{
				premio1=premio1+20;
				premio2=premio2-20;
			}		
					
		}
	}
		
	else
	{
		//Empate

		if(uno->Fitness()>=dos->Fitness())
		{
			difFit=abs(uno->Fitness()-dos->Fitness());
			if(difFit>50)
			{
				premio1=premio1-10;
				premio2=premio2+10;
			}
			
				
		}
		else
		{
			difFit=abs(uno->Fitness()-dos->Fitness());
			if(difFit>50)
			{
				premio1=premio1+10;
				premio2=premio2-10;
			}		
					
		}
	}
	if(uno->Fitness()+premio1>0)
		uno->setFitness(uno->Fitness()+premio1);
	else
		uno->setFitness(0);

	if(dos->Fitness()+premio2>0)
		dos->setFitness(dos->Fitness()+premio2);
	else
		dos->setFitness(0);


}
void CController::updateNpcFitness(int numNpc){
	//my health decrease or increase

	m_vecNpc[numNpc]->setFitness(m_vecNpc[numNpc]->Fitness()-(m_vecNpcHealth[numNpc]-m_vecNpc[numNpc]->get_health())/10);
	m_vecNpcHealth[numNpc] = m_vecNpc[numNpc]->get_health();

	//enemy health decrease
	//value of the difference will increase fitness twice
	if(m_vecNpc[numNpc]->getEnemigo()->get_health()<m_vecNpcEnemiesHealth[numNpc]){
		m_vecNpc[numNpc]->setFitness(m_vecNpc[numNpc]->Fitness()+(m_vecNpcEnemiesHealth[numNpc]-m_vecNpc[numNpc]->getEnemigo()->get_health())/5);
		m_vecNpcEnemiesHealth[numNpc] = m_vecNpc[numNpc]->getEnemigo()->get_health();
	}

}

//-------------------------------------Update-----------------------------
//
//	This is the main workhorse. The entire simulation is controlled from here.
//
//	The comments should explain what is going on adequately.
//-------------------------------------------------------------------------
bool CController::Update()
{
	//run the sweepers through CParams::iNumTicks amount of cycles. During
	//this loop each sweepers NN is constantly updated with the appropriate
	//information from its surroundings. The output from the NN is obtained
	//and the sweeper is moved. If it encounters a mine its fitness is
	//updated appropriately,
	if (m_iTicks++ < CParams::iNumTicks && !firstBlood)
	{
		for (int i=0; i<m_NumNpc; ++i)
		{
			if(m_vecJornadasLocales[m_vecJornadasLocales.size()-duelosRestantes]!=i && m_vecJornadasVisitantes[m_vecJornadasVisitantes.size()-duelosRestantes]!=i)
				continue;

			//update the NN and position
			if (!m_vecNpc[i]->Update())
			{
				//error in processing the neural net
				MessageBox(m_hwndMain, "Wrong amount of NN inputs!", "Error", MB_OK);

				return false;
			}

			updateNpcFitness(i);

			//update the chromos fitness score
			m_vecThePopulation[i].dFitness = m_vecNpc[i]->Fitness();

		}
	}
	//Another generation has been completed.
	//Time to run the GA and update the sweepers with their new NNs
	else
	{
		

		invisPlayers(m_vecJornadasLocales.size() - duelosRestantes);
		duelosRestantes--;
		cout<<"Duelos restantes "<<duelosRestantes<<endl;
		
	
		if(duelosRestantes==0){
			updateFitnessFinal(m_vecNpc[m_vecJornadasLocales[m_vecJornadasLocales.size() - (duelosRestantes+1)]], m_vecNpc[m_vecJornadasVisitantes[m_vecJornadasLocales.size() - (duelosRestantes+1)]]);
			std::ofstream fs;

			fs.open("pesos.txt", std::ios_base::app);
			//ofstream fs("pesos.txt"); 
			cout<<"Generacion " <<m_iGenerations<<endl;
			fs<<"Generacion " <<m_iGenerations<<endl;
			//update the stats to be used in our stat window
			m_vecAvFitness.push_back(m_pGA->AverageFitness());
			m_vecBestFitness.push_back(m_pGA->BestFitness());

			 
			
			fs<<"Best fitness: " <<m_pGA->BestFitness()<<endl;
			
			fs<<"Best pesos: ";
			for(int i = 0; i<m_pGA->GetBestWeights().size(); i++){
				//cout<<m_pGA->GetBestWeights()[i]<<" ";
				fs<<m_pGA->GetBestWeights()[i]<<" ";
			}
			fs<<endl;

			fs.close();
			//increment the generation counter
			++m_iGenerations;
				/*	 // Crea un fichero de salida
			  

			   // Enviamos una cadena al fichero de salida:
			   fs <<"Best fitness: " <<m_pGA->BestFitness()<<endl;
			   // Cerrar el fichero, 
			   // para luego poder abrirlo para lectura:
			   fs.close();*/
			//reset cycles
			m_iTicks = 0;

			//run the GA to create a new population
			m_vecThePopulation = m_pGA->Epoch(m_vecThePopulation);

			//insert the new (hopefully)improved brains back into the sweepers
			//and reset their positions etc
			duelosRestantes = (m_NumNpc-1)* (m_NumNpc/2);
			for (int i=0; i<m_NumNpc; ++i)
			{
				m_vecNpc[i]->PutWeights(m_vecThePopulation[i].vecWeights);
				m_vecNpc[i]->Reset();
				m_vecNpcHealth[i] = 100;
				m_vecNpcEnemiesHealth[i]=100;
				invisPlayers(i);
			}
			asignarEnemigo(0);

		}
		else{
			updateFitnessFinal(m_vecNpc[m_vecJornadasLocales[m_vecJornadasLocales.size() - (duelosRestantes+1)]], m_vecNpc[m_vecJornadasVisitantes[m_vecJornadasLocales.size() - (duelosRestantes+1)]]);
			m_vecNpc[m_vecJornadasLocales[m_vecJornadasLocales.size() - (duelosRestantes+1)]]->Reset();
			m_iTicks = 0;
			m_vecNpcHealth[m_vecJornadasLocales[m_vecJornadasLocales.size() - (duelosRestantes+1)]] = 100;
			m_vecNpcEnemiesHealth[m_vecJornadasVisitantes[m_vecJornadasLocales.size() - (duelosRestantes+1)]]=100;
			
			m_vecNpcHealth[m_vecJornadasVisitantes[m_vecJornadasLocales.size() - (duelosRestantes+1)]] = 100;
			m_vecNpcEnemiesHealth[m_vecJornadasLocales[m_vecJornadasLocales.size() - (duelosRestantes+1)]]=100;
			asignarEnemigo(m_vecJornadasLocales.size() - duelosRestantes);

		}
	}

	return true;
}
//------------------------------------Render()--------------------------------------
//
//----------------------------------------------------------------------------------
void CController::Render(HDC surface)
{
	//render the stats
	std::string s = "Generation:          " + itos(m_iGenerations);
	TextOut(surface, 5, 0, s.c_str(), s.size());

	PlotStats(surface);
}
//--------------------------PlotStats-------------------------------------
//
//  Given a surface to draw on this function displays stats and a crude
//  graph showing best and average fitness
//------------------------------------------------------------------------
void CController::PlotStats(HDC surface)
{
	std::string s = "Best Fitness:       " + ftos(m_pGA->BestFitness());
	TextOut(surface, 5, 20, s.c_str(), s.size());

	s = "Average Fitness: " + ftos(m_pGA->AverageFitness());
	TextOut(surface, 5, 40, s.c_str(), s.size());

	//render the graph
	float HSlice = (float)cxClient/(m_iGenerations+1);
	float VSlice = (float)cyClient/((m_pGA->BestFitness()+1)*2);

	//plot the graph for the best fitness
	float x = 0;

	m_OldPen = (HPEN)SelectObject(surface, m_RedPen);

	MoveToEx(surface, 0, cyClient, NULL);

	for (int i=0; i<m_vecBestFitness.size(); ++i)
	{
		LineTo(surface, x, cyClient - VSlice*m_vecBestFitness[i]);

		x += HSlice;
	}

	//plot the graph for the average fitness
	x = 0;

	SelectObject(surface, m_BluePen);

	MoveToEx(surface, 0, cyClient, NULL);

	for (int i=0; i<m_vecAvFitness.size(); ++i)
	{
		LineTo(surface, (int)x, (int)(cyClient - VSlice*m_vecAvFitness[i]));

		x += HSlice;
	}

	//replace the old pen
	SelectObject(surface, m_OldPen);
}

