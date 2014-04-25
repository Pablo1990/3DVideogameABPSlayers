#include "xor.h"


xor::xor(void)
{
	fitness = 0;
	digito1 = RandInt(0,1);
	digito2 = RandInt(0,1);
	resultado = 0;
}


xor::~xor(void)
{
}

void xor::Reset()
{
	digito1 = RandInt(0,1);
	digito2 = RandInt(0,1);
}

bool xor::Update()
{
	vector<double> inputs;
	inputs.push_back(getPosObjetoCercanoX());
	inputs.push_back(getPosObjetoCercanoY());
	vector<double> output = m_ItsBrain.Update(inputs);
	if (output.size() < CParams::iNumOutputs) 
	{
		cerr<<"ERROR"<<endl;
		return false;
	}
	//me muevo hacia la izquierda
	movimientoX = 0;
	if(output[0]>=0.6)
		movimientoX = 25;

	movimientoY = 0;
	if(output[1]>=0.6)
		movimientoY = 25;

	return true;
}

double xor::getPosObjetoCercanoX(){
	return;
}

double xor::Fitness(){
	return fitness;
}