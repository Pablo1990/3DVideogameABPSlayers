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
		Bot *jugador=new Bot(100.0,0.0,0.0,10.0);
	Bot *enemigoPrincipal=new Bot(100.0,20,20,10.0);
	
	enemigoPrincipal->setEnem(jugador);
	jugador->setEnem(enemigoPrincipal);
	
	Goal_Think *g=new Goal_Think();
	
	g->setDueño(enemigoPrincipal);
	enemigoPrincipal->setBrain(g);
	Window_Scene scene=Window_Scene(enemigoPrincipal,jugador);

    //El programa se ejecutara mientras la ventana este abierta
	scene.window.display();
	int i=0;
		 while (scene.window.isOpen())
    {
        //Reogemos y procesamos los eventos del sistema operativo
        sf::Event event;
        while (scene.window.pollEvent(event))
        {
            //Si el evento es cerrar la ventana, cerramos la ventana que creamos antes
            if (event.type == sf::Event::Closed)
              scene.window.close();

			scene.moverJugador(event);
			
			scene.cargarEscenario();
        }
		
		
		
		 
		
		i++;
	  
    }

 //System("PAUSE");
  return 0;
}
