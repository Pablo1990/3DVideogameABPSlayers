#include "CController.h"


//these hold the geometry of the sweepers and the mines
const int	 NumSweeperVerts = 16;
const SPoint sweeper[NumSweeperVerts] = {SPoint(-1, -1),
	SPoint(-1, 1),
	SPoint(-0.5, 1),
	SPoint(-0.5, -1),

	SPoint(0.5, -1),
	SPoint(1, -1),
	SPoint(1, 1),
	SPoint(0.5, 1),

	SPoint(-0.5, -0.5),
	SPoint(0.5, -0.5),

	SPoint(-0.5, 0.5),
	SPoint(-0.25, 0.5),
	SPoint(-0.25, 1.75),
	SPoint(0.25, 1.75),
	SPoint(0.25, 0.5),
	SPoint(0.5, 0.5)};



const int NumMineVerts = 4;
const SPoint mine[NumMineVerts] = {SPoint(-1, -1),
	SPoint(-1, 1),
	SPoint(1, 1),
	SPoint(1, -1)};



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
		m_vecSweepers.push_back(CMinesweeper());
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

	//fill the vertex buffers
	for (int i=0; i<NumSweeperVerts; ++i)
	{
		m_SweeperVB.push_back(sweeper[i]);
	}

	for (int i=0; i<NumMineVerts; ++i)
	{
		m_MineVB.push_back(mine[i]);
	}

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


//---------------------WorldTransform--------------------------------
//
//	sets up the translation matrices for the mines and applies the
//	world transform to each vertex in the vertex buffer passed to this
//	method.
//-------------------------------------------------------------------
void CController::WorldTransform(vector<SPoint> &VBuffer, SVector2D vPos)
{
	//create the world transformation matrix
	C2DMatrix matTransform;

	//scale
	matTransform.Scale(CParams::dMineScale, CParams::dMineScale);

	//translate
	matTransform.Translate(vPos.x, vPos.y);

	//transform the ships vertices
	matTransform.TransformSPoints(VBuffer);
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

			//see if it's found a mine
			cout<<"El resultado de "<<m_vecSweepers[i].getDigito1()<<" xor "<<m_vecSweepers[i].getDigito2()<<" ha sido "<<m_vecSweepers[i].getResultado()<<endl;
			if(m_vecSweepers[i].getResultado() == (m_vecSweepers[i].getDigito1() ^ m_vecSweepers[i].getDigito2()))
			{
				//aumento fitness
				m_vecSweepers[i].aumentoFitness();
				cout<<"CORRECTO"<<endl;
			}
			else{
				cout<<"INCORRECTO"<<endl;
			}

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
//------------------------------------Render()--------------------------------------
//
//----------------------------------------------------------------------------------
void CController::Render(HDC surface)
{
	//render the stats
	string s = "Generation:          " + itos(m_iGenerations);
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
	string s = "Best Fitness:       " + ftos(m_pGA->BestFitness());
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

