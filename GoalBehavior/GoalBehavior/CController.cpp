#include "stdafx.h"
#include "CController.h"



//---------------------------------------constructor---------------------
//
//	initilaize the sweepers, their brains and the GA factory
//
//-----------------------------------------------------------------------
CController::CController(HWND hwndMain): m_NumSweepers(CParams::iNumSweepers), 
	m_pGA(NULL),
	m_bFastRender(false),
	m_iTicks(0),
	m_NumMines(CParams::iNumMines),
	m_hwndMain(hwndMain),
	m_iGenerations(0),
	cxClient(CParams::WindowWidth),
	cyClient(CParams::WindowHeight)
{
	//let's create the mine sweepers
	for (int i=0; i<m_NumSweepers; ++i)
	{
		m_vecSweepers.push_back(xor());
	}

	//get the total number of weights used in the sweepers
	//NN so we can initialise the GA
	m_NumWeightsInNN = m_vecSweepers[0].GetNumberOfWeights();

	//initialize the Genetic Algorithm class
	m_pGA = new CGenAlg(m_NumSweepers,
		CParams::dMutationRate,
		CParams::dCrossoverRate,
		m_NumWeightsInNN);

	//Get the weights from the GA and insert into the sweepers brains
	m_vecThePopulation = m_pGA->GetChromos();

	for (int i=0; i<m_NumSweepers; i++)

		m_vecSweepers[i].PutWeights(m_vecThePopulation[i].vecWeights);

	//initialize mines in random positions within the application window
	for (int i=0; i<m_NumMines; ++i)
	{
		m_vecMines.push_back(SVector2D(RandFloat() * cxClient,
			RandFloat() * cyClient));
	}

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
	if (m_iTicks++ < CParams::iNumTicks)
	{
		for (int i=0; i<m_NumSweepers; ++i)
		{
			//update the NN and position
			if (!m_vecSweepers[i].Update())
			{
				//error in processing the neural net
				MessageBox(m_hwndMain, "Wrong amount of NN inputs!", "Error", MB_OK);

				return false;
			}

			updateFitness(i);

			//update the chromos fitness score
			m_vecThePopulation[i].dFitness = m_vecSweepers[i].Fitness();

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
		for (int i=0; i<m_NumSweepers; ++i)
		{
			m_vecSweepers[i].PutWeights(m_vecThePopulation[i].vecWeights);

			m_vecSweepers[i].Reset();
		}
	}

	return true;
}

void CController::updateFitness(int i){
	if(m_vecSweepers[i].estoyEnObjeto())
		m_vecSweepers[i].aumentoFitness();
}