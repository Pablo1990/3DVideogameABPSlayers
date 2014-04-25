#pragma once
#include <math.h>

#include "CNeuralNet.h"
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"

const int dimMapa = 475;
const int dimCasilla = 25;

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
	void crearListaObjetos();
	void getPosMasCercano(double &, double &);
	double getDistanciaABot(double x, double y);
	void getPosRelativaABot(double &, double &);
	void posEntreCeroYUno(double&);
	bool estoyEnObjeto();


private:
	CNeuralNet m_ItsBrain;
	double posX;
	double posY;
	double movimientoX;
	double movimientoY;
	double fitness;
	vector<int> itemsX;
	vector<int> itemsY;
};

