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
	Bot *jugador=new Bot();
	Bot *enemigoPrincipal=new Bot();
	vector<Bot*> bots;
	bots.push_back(jugador);
	bots.push_back(enemigoPrincipal);
	CController* controller = new CController(bots);
	enemigoPrincipal->setEnem(jugador);
	jugador->setEnem(enemigoPrincipal);
	
	
	Window_Scene scene=Window_Scene();
	scene.cargarObjetos(bots);
    //El programa se ejecutara mientras la ventana este abierta
	scene.window.display();

	while (scene.window.isOpen())
    {
        //Reogemos y procesamos los eventos del sistema operativo
        sf::Event event;
		while (true)
        {
			scene.window.pollEvent(event);
            //Si el evento es cerrar la ventana, cerramos la ventana que creamos antes
            if (event.type == sf::Event::Closed){
              scene.window.close();
			  return 0;
			}

			//system("PAUSE");
			
			cout<<"----------"<<endl;

			//scene.moverJugador(event,jugador,enemigoPrincipal);
			controller->Update();
			if(jugador->getSalud()==0 )
			{
				
				cout<<"Fin del juego: Gana El Bot"<<endl;
				scene.window.close();
			}
			else if (enemigoPrincipal->getSalud()==0 )
			{
				cout<<"Fin del juego: Gana El Jugador"<<endl;
				scene.window.close();
			}

			//scene.cargarEscenario(bots);
        }
		
    }

 system("PAUSE");
  return 0;
}
