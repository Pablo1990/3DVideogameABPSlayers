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
	
	Bot *jugador=new Bot(100.0,475,475,10.0);
	Bot *enemigoPrincipal=new Bot(100.0,13*25,2*25,10.0);
	
	enemigoPrincipal->setEnem(jugador);
	jugador->setEnem(enemigoPrincipal);
	
	Goal_Think *g=new Goal_Think();
	
	g->setDueño(enemigoPrincipal);
	enemigoPrincipal->setBrain(g);
	Window_Scene scene=Window_Scene(enemigoPrincipal,jugador);

    //El programa se ejecutara mientras la ventana este abierta
	scene.window.display();

		 while (scene.window.isOpen())
    {
        //Reogemos y procesamos los eventos del sistema operativo
        sf::Event event;
		while (scene.window.pollEvent(event))
        {
            //Si el evento es cerrar la ventana, cerramos la ventana que creamos antes
            if (event.type == sf::Event::Closed)
              scene.window.close();

			g->Arbitrate();
			g->ProcessSubgoals();
			scene.moverJugador(event,jugador,enemigoPrincipal);
			if((jugador->getSalud()==0 || enemigoPrincipal->getSalud()==0 ))
			{
				
				cout<<"Fin del juego"<<endl;
				scene.window.close();
			}

			scene.cargarEscenario(enemigoPrincipal,jugador);
        }
		
    }

 system("PAUSE");
  return 0;
}
