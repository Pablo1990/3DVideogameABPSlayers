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
			int cont = 0;
			for (int j=0; j<4; j++){
				if(m_vecSweepers[i].getResultado()[j]>=0.5){
					if(1 == (m_vecSweepers[i].getDigito1()[j] != m_vecSweepers[i].getDigito2()[j]))
					{
						//aumento fitness
						//if(cont>1)
							m_vecSweepers[i].aumentoFitness();
						cont++;
					}
				}
				else {
					if(0 == (m_vecSweepers[i].getDigito1()[j] != m_vecSweepers[i].getDigito2()[j]))
					{
						//aumento fitness
						//if(cont>1)
							m_vecSweepers[i].aumentoFitness();
						cont++;
					}

				}
			}
			string c = "Aciertos" + itos(cont) + "\n";
			OutputDebugString(c.c_str());
			if(cont==4){
				OutputDebugString("Todos Correctos\n");
				return false;
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
void CController::plotNeuralNet(HDC surface){
	CNeuralNet aux;
	//for(int i=0; i<m_NumSweepers; i++){
	int numIndividuo = m_pGA->getFittest(); //cogemos el que tiene mayor fitness
	aux = m_vecSweepers[numIndividuo].GetNeuralNet();
		for (int i=0; i<aux.m_NumHiddenLayers + 1; ++i)
		{
			//for each neuron
			for (int j=0; j<aux.m_vecLayers[i].m_NumNeurons; ++j)
			{
				//for each weight
				string s =  "NeuronWeights" + itos(i+1);
				TextOut(surface, 380, i*20, s.c_str(), s.size());
				s =  "[";
				TextOut(surface, 500, i*20, s.c_str(), s.size());
				for (int k=0; k<aux.m_vecLayers[i].m_vecNeurons[j].m_NumInputs; ++k)
				{
					s =ftos(aux.m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k])+",";
					TextOut(surface, 500 + (80*k +5), i*20, s.c_str(), s.size());
				}
				s = "]";
				TextOut(surface, 500 + (80*aux.m_vecLayers[i].m_vecNeurons[j].m_NumInputs)+5, i*20, s.c_str(), s.size());
			}
		}

		string s = "Aciertos " +itos(m_vecSweepers[numIndividuo].getAciertos());
		TextOut(surface, 200, 0, s.c_str(), s.size());
		s = "inputs1 ";
		TextOut(surface, 200, 20, s.c_str(), s.size());
		s = "inputs2 ";
		TextOut(surface, 200, 40, s.c_str(), s.size());
		s = "ouputs";
		TextOut(surface, 10, 60, s.c_str(), s.size());
		s = "por capas";	
		TextOut(surface, 10, 80, s.c_str(), s.size());
		for(int j = 0; j<4; j++){
			s = ", " + itos(m_vecSweepers[numIndividuo].getDigito1()[j]);	
			TextOut(surface, 255 + 20*j, 20, s.c_str(), s.size());
			s = ", " + itos(m_vecSweepers[numIndividuo].getDigito2()[j]);	
			TextOut(surface, 255 + 20*j, 40, s.c_str(), s.size());
			for (int k=0; k<aux.m_NumHiddenLayers + 1; ++k)
			{
				//for each neuron
				for (int p=0; p<aux.m_vecLayers[k].m_NumNeurons; ++p)
				{
					s = " " + ftos(aux.outputs[j][k][p]);	
					TextOut(surface, 100 + 80*p + 80*(j*aux.m_vecLayers[0].m_NumNeurons), 60 + 20*k, s.c_str(), s.size());
				}
			}
		}
	//}
}
void CController::Render(HDC surface)
{
	//render the stats
	string s = "Generation:          " + itos(m_iGenerations);
	TextOut(surface, 5, 0, s.c_str(), s.size());

	PlotStats(surface);

	plotNeuralNet(surface);
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

