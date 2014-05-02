#include "xor.h"


xor::xor(void)
{
	fitness = 0;
	for(int i = 0; i<4; i++) {
		switch(i){
			case 0: digito1[i]= 0; digito2[i] = 0;
				break;
			case 1: digito1[i] = 1; digito2[i] = 0;
				break;
			case 2: digito1[i] = 0; digito2[i] = 1;
				break;
			case 3: digito1[i] = 1; digito2[i] = 1;
				break;
		}
		resultado[i] = 0;
	}
	aciertos = 0;
}


xor::~xor(void)
{
}

void xor::Reset()
{
	aciertos = 0;
}

bool xor::Update()
{
	for(int i = 0; i<4; i++){
		vector<double> inputs;
		
		inputs.push_back(digito1[i]);
		inputs.push_back(digito2[i]);
		vector<double> output = m_ItsBrain.Update(inputs);
		if (output.size() < CParams::iNumOutputs) 
		{
			cerr<<"ERROR"<<endl;
			return false;
		}
		resultado[i] = output[0];
	}
	
	return true;
}

int* xor::getDigito1(){
	return digito1;
}

int* xor::getDigito2(){
	return digito2;
}

double* xor::getResultado(){
	return resultado;
}

double xor::Fitness(){
	return fitness;
}