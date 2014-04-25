#pragma once
#include <math.h>

#include "CNeuralNet.h"
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"

using namespace std;

class xor
{
public:
	xor(void);
	~xor(void);
	bool Update();
	void Reset();
	double getPosObjetoCercanoX();
	double getPosObjetoCercanoY();
	double Fitness();
	void PutWeights(vector<double> &w){m_ItsBrain.PutWeights(w);}
	int GetNumberOfWeights()const{return m_ItsBrain.GetNumberOfWeights();}
	void aumentoFitness() {fitness++;}

private:
	CNeuralNet m_ItsBrain;
	double objetoCercanoX;
	double objetoCercanoY;
	double posX;
	double posY;
	double movimientoX;
	double movimientoY;
	double fitness;
};

