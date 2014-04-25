#include "xor.h"


xor::xor(void)
{
	fitness = 0;
	digito1 = RandInt(0,1);
	digito2 = RandInt(0,1);
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
	resultado = output[0];
	
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