// GoalBehavior.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "Bot.h"
#include "Window_Scene.h"
#include "stdio.h"
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>

#define DIMENSIONS 5
#define N 20
#define PLA_ITS 100
#define NUM_INPUTS 100
#define BOTMOVEMENTS 500

bool h(double weights[DIMENSIONS+1], double x[DIMENSIONS]) {
    double sum = weights[0];
    
    for (int i = 1; i < DIMENSIONS+1; ++i)
        sum += weights[i] * x[i-1];

    return sum >= 0;
}

void updateWeights(double weights[DIMENSIONS+1], double inputs[DIMENSIONS], double correct) {
    weights[0] += correct;
    
    for (int i = 1; i < DIMENSIONS+1; ++i)
        weights[i] += correct*inputs[i-1];
}

bool training(double inputs[N][DIMENSIONS], bool onfireInputs[N], double currentInput[DIMENSIONS]) {
    double errors[N];
    double weights[DIMENSIONS+1];
    double bestWeights[DIMENSIONS+1];
    int errorsCount = 0;
    int randomInput;
    int minErrorsCount = N+1;
    
    weights[0] = 1;     // Se inicializa con peso=1 porque es el threshold
    for (int i = 1; i <= DIMENSIONS; i++)
        weights[i] = 0;
    
    for (int i=0; i < PLA_ITS; i++) {
        errorsCount = 0;

        for (int j = 0; j < N; j++) {            
            if (h(weights, inputs[j]) == onfireInputs[j]) {  // Se comprueba si hay error
                errors[errorsCount] = j;
                errorsCount++;
            }
        }

        if (errorsCount > 0) {  // Se actualiza un peso en caso de haber error
            if (errorsCount < minErrorsCount) {  // Se guardan los mejores pesos
                minErrorsCount = errorsCount;
                std::copy(&weights[0], &weights[DIMENSIONS+1], bestWeights);
            }

            randomInput = errors[rand()%errorsCount];
            updateWeights(weights, inputs[randomInput], (onfireInputs[randomInput]? -1 : 1));
        }
        else
            return h(weights, currentInput);
    }

    return h(bestWeights, currentInput);
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int cont=0;
	double inputs[NUM_INPUTS][DIMENSIONS];
	double result[DIMENSIONS];
	double weights[DIMENSIONS+1];

	//INICIALIZAR WEIGHTS

	
	double probabilities[] = {0.8, 0.6, 0.05, 0.4, 0.3};
	

	for(int j=0; j<NUM_INPUTS; j++)
	{
		cont=0;
		//Probabilidades: Attack, Escape, Explore, GetHealth, GetWeapon.
		double randProbabilities[] = {fRand(0,1), fRand(0,1), fRand(0,1), fRand(0,1), fRand(0,1)};

		
		//Declaramos los bot y la memoria del juego
		Bot *jugador=new Bot(100.0,475,3*25,4.0, probabilities);
		Bot *enemigoPrincipal=new Bot(100.0,13*25,3*25,4.0, randProbabilities);
	
		enemigoPrincipal->setEnem(jugador);
		jugador->setEnem(enemigoPrincipal);
	
		Goal_Think *g=new Goal_Think();
		Goal_Think *g2 = new Goal_Think();

		g2->setDueño(jugador);
		g->setDueño(enemigoPrincipal);

		jugador->setBrain(g2);
		enemigoPrincipal->setBrain(g);
		//Window_Scene scene=Window_Scene(enemigoPrincipal,jugador);

		//El programa se ejecutara mientras la ventana este abierta
		//scene.window.display();
		 while (cont<BOTMOVEMENTS)
		{
			cont--;
			//Reogemos y procesamos los eventos del sistema operativo
			
				//Si el evento es cerrar la ventana, cerramos la ventana que creamos antes
				

				g->Arbitrate();
				g->ProcessSubgoals();
				g2->Arbitrate();
				g2->ProcessSubgoals();
				//scene.moverJugador(event,jugador,enemigoPrincipal);
				if((jugador->getSalud()==0 || enemigoPrincipal->getSalud()==0 ))
				{
				
					cout<<"Fin del juego"<<endl;
					break;
				}
		 }

		 for (int i = 0; i < DIMENSIONS; i++)
		 {
			 inputs[j][i] = probabilities[i];
		 }

		 result[j] = enemigoPrincipal->getSalud()*10 + enemigoPrincipal->getArma() - enemigoPrincipal->getEnem()->getSalud()*10 - enemigoPrincipal->getEnem()->getArma();
		
	}

	training

 system("PAUSE");
  return 0;
}
