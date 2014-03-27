#ifndef CCONTROLLER_H
#define CCONTROLLER_H

//------------------------------------------------------------------------
//
//	Name: CController.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Controller class for 'Smart Sweeper' example from the book 
//        Game AI Programming with Neural Nets and Genetic Algorithms. 
//
//------------------------------------------------------------------------
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <windows.h>

#include "../Npc.h"
#include "CGenAlg.h"
#include "utils.h"
#include "CParams.h"

using namespace std;



class CController
{

private:

	//storage for the population of genomes
	vector<SGenome>	     m_vecThePopulation;

	//and the minesweepers
	vector<Npc*> m_vecNpc;

	vector<double> m_vecNpcHealth;

	vector<double> m_vecNpcEnemiesHealth;

	//pointer to the GA
	CGenAlg*		         m_pGA;

	int					         m_NumNpc;

	int					         m_NumWeightsInNN;

	//stores the average fitness per generation for use 
	//in graphing.
	vector<double>		   m_vecAvFitness;

	//stores the best fitness per generation
	vector<double>		   m_vecBestFitness;

	//pens we use for the stats
	HPEN				m_RedPen;
	HPEN				m_BluePen;
	HPEN				m_GreenPen;
	HPEN				m_OldPen;
	
	//handle to the application window
	HWND				m_hwndMain;

	//toggles the speed at which the simulation runs
	bool				m_bFastRender;
	
	//cycles per generation
	int					m_iTicks;

	//generation counter
	int					m_iGenerations;

	//window dimensions
	int         cxClient, cyClient;

	//this function plots a graph of the average and best fitnesses
	//over the course of a run
	void   PlotStats(HDC surface);

	bool firstBlood; //le dara un bonus y acabará el ciclo

public:

	CController(HWND hwndMain,ISceneManager *sm, vector3df posHealth);

	~CController();

	void		Render(HDC surface);

	bool		Update();

	void updateNpcFitness(int numNpc);

	void assignEnemies();

	void updateFitnessFinal(Npc*,Npc*);

};


#endif
	
