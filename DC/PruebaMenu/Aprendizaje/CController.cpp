#include "CController.h"

//---------------------------------------constructor---------------------
//
//	initilaize the npc, their brains and the GA factory
//
//-----------------------------------------------------------------------
CController::CController(HWND hwndMain,ISceneManager *sm, vector3df posHealth): m_NumNpc(CParams::iNumSweepers), 
	m_pGA(NULL),
	m_iTicks(0),
	m_hwndMain(hwndMain),
	m_iGenerations(0),
	cxClient(CParams::WindowWidth),
	cyClient(CParams::WindowHeight)
{
	//Debemos definiir para cada npc un enemigo
	Position last_corner(1894.93, 1, 1294.88);
	//let's create the bots
	for (int i=0; i<m_NumNpc; ++i)
	{
		Npc* n = new Npc(sm,new Sword(0,0,sm),posHealth);
		n->add_to_scene(core::vector3df(0,100,0), core::vector3df(0, 270, 0), core::vector3df(0.55, 0.55, 0.55));
		m_vecNpc.push_back(n);
	}
	
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
	m_vecNpc[m_vecJornadasLocales[num]]->setEnem(m_vecNpc[m_vecJornadasVisitantes[num]]);
	m_vecNpc[m_vecJornadasVisitantes[num]]->setEnem(m_vecNpc[m_vecJornadasLocales[num]]);
	
	m_vecNpc[m_vecJornadasLocales[num]]->set_position(100, 10, 100);
	m_vecNpc[m_vecJornadasVisitantes[num]]->set_position(1000,10,300);
}

void CController::invisPlayers(int num){
	m_vecNpc[m_vecJornadasLocales[num]]->set_position(0, 100, 0);
	m_vecNpc[m_vecJornadasVisitantes[num]]->set_position(0,100,0);
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
			if(m_vecNpc[i]->get_position().X == 0 && m_vecNpc[i]->get_position().Y == 100 && m_vecNpc[i]->get_position().Z == 0)
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
		if(duelosRestantes==0){
			//update the stats to be used in our stat window
			m_vecAvFitness.push_back(m_pGA->AverageFitness());
			m_vecBestFitness.push_back(m_pGA->BestFitness());

			//increment the generation counter
			++m_iGenerations;

			//reset cycles
			m_iTicks = 0;

			//run the GA to create a new population
			m_vecThePopulation = m_pGA->Epoch(m_vecThePopulation);

			//insert the new (hopefully)improved brains back into the sweepers
			//and reset their positions etc
			for (int i=0; i<m_NumNpc; ++i)
			{
				m_vecNpc[i]->PutWeights(m_vecThePopulation[i].vecWeights);
				m_vecNpc[i]->Reset(core::vector3df((((rand()%100) * 10)+(i+1*10)),10,((rand()%100) * 10)+10));
				m_vecNpcHealth[i] = 100;
				m_vecNpcEnemiesHealth[i]=100;
				invisPlayers(i);
			}
			asignarEnemigo(0);
		}
		else{
			m_iTicks = 0;
			asignarEnemigo(m_vecJornadasLocales.size() - duelosRestantes);
			m_vecNpcHealth[m_vecJornadasLocales[m_vecJornadasLocales.size() - duelosRestantes]] = 100;
			m_vecNpcEnemiesHealth[m_vecJornadasVisitantes[m_vecJornadasLocales.size() - duelosRestantes]]=100;
			
			m_vecNpcHealth[m_vecJornadasVisitantes[m_vecJornadasLocales.size() - duelosRestantes]] = 100;
			m_vecNpcEnemiesHealth[m_vecJornadasLocales[m_vecJornadasLocales.size() - duelosRestantes]]=100;

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

