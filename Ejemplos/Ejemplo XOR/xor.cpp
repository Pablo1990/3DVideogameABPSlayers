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
	inputs.push_back(digito1);
	inputs.push_back(digito2);
	vector<double> output = m_ItsBrain.Update(inputs);
	if (output.size() < CParams::iNumOutputs) 
	{
		cerr<<"ERROR"<<endl;
		return false;
	}

	if(output[0]>=0.9)
		resultado = 1;
	else
		resultado = 0;

	
	return true;
}

int xor::getDigito1(){
	return digito1;
}

int xor::getDigito2(){
	return digito2;
}

double xor::getResultado(){
	return resultado;
}

double xor::Fitness(){
	return fitness;
}