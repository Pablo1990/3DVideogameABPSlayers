// GoalBehavior.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "Bot.h"
#include "Window_Scene.h"
#include "stdio.h"
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>



int _tmain(int argc, _TCHAR* argv[])
{
		//Declaramos los bot y la memoria del juego
	CParams();
	vector<Bot*> bots;
	vector<int*> itemsX;
	vector<int*> itemsY;
	for(int i=0; i<numObjetos; i++){
		itemsX.push_back(new int(RandInt(0, dimMapa/dimCasilla)*dimCasilla));
		itemsY.push_back(new int(RandInt(0, dimMapa/dimCasilla)*dimCasilla));
	}

	double x = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
	double y = RandInt(0, dimMapa/dimCasilla)*dimCasilla;
	for(int i=0; i<CParams::iNumSweepers; i++)
		bots.push_back(new Bot(itemsX, itemsY, x, y));

	CController* controller = new CController(bots);
	
	
	Window_Scene scene=Window_Scene();
	scene.cargarObjetos(bots);
    //El programa se ejecutara mientras la ventana este abierta
	scene.window.display();
	bool pPulsado = false;
	bool siguienteIter = false;
	while (scene.window.isOpen())
    {
		sf::Event event;
		scene.window.pollEvent(event);
		if(event.type == sf::Event::KeyPressed){
			if(event.key.code == sf::Keyboard::P){
				pPulsado = !pPulsado;
			}

			if(event.key.code == sf::Keyboard::Space){
				siguienteIter = true;
			}
		}

		if (!pPulsado || siguienteIter)
        {
			siguienteIter = false;
			scene.window.pollEvent(event);
            //Si el evento es cerrar la ventana, cerramos la ventana que creamos antes
            if (event.type == sf::Event::Closed){
              scene.window.close();
			  return 0;
			}

			//system("PAUSE");
		
			cout<<"----------"<<endl;
			
			controller->Update();
			
			scene.cargarEscenario(bots, controller);

        }
		
    }

 system("PAUSE");
  return 0;
}
