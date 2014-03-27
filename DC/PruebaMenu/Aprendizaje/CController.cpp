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
		m_vecNpcHealth.push_back(100);
		m_vecNpc.push_back(new Npc(sm,new Sword(0,0,sm),posHealth));
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

	for (int i=0; i<m_NumNpc; i++)
		m_vecNpc[i]->PutWeights(m_vecThePopulation[i].vecWeights);


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
	int v1 = rand()%m_NumNpc;
	int v2 = rand()%m_NumNpc;

	while(v1==v2){
		v1 = rand()%m_NumNpc;
		v2 = rand()%m_NumNpc;
	}

	for(int i=0; i<m_NumNpc/2; i++)
	{
		m_vecNpc[v1%m_NumNpc]->setEnem(m_vecNpc[v2%m_NumNpc]);
		m_vecNpc[v2%m_NumNpc]->setEnem(m_vecNpc[v1%m_NumNpc]);
		v1++;
		v2++;
	}
	
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
	m_vecNpc[numNpc]->setFitness(m_vecNpc[numNpc]->Fitness()-(m_vecNpcHealth[numNpc]-m_vecNpc[numNpc]->get_health()));
	m_vecNpcHealth[numNpc] = m_vecNpc[numNpc]->get_health();

	//enemy health decrease
	//value of the difference will increase fitness twice
	if(m_vecNpc[numNpc]->getEnemigo()->get_health()<m_vecNpcEnemiesHealth[numNpc]){
		m_vecNpc[numNpc]->setFitness(m_vecNpc[numNpc]->Fitness()+(m_vecNpcEnemiesHealth[numNpc]-m_vecNpc[numNpc]->getEnemigo()->get_health())*2);
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
		}
		
		assignEnemies();
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

