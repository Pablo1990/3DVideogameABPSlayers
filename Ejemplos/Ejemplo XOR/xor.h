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
	int* getDigito1();
	int* getDigito2();
	double* getResultado();
	double Fitness();
	void PutWeights(vector<double> &w){m_ItsBrain.PutWeights(w);}
	int GetNumberOfWeights()const{return m_ItsBrain.GetNumberOfWeights();}
	void aumentoFitness() {fitness++;}

private:
	CNeuralNet m_ItsBrain;
	int digito1[4];
	int digito2[4];
	double resultado[4];
	double fitness;
};

