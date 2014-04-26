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
	CController* controller = new CController();
	enemigoPrincipal->setEnem(jugador);
	jugador->setEnem(enemigoPrincipal);
	
	
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

			//system("PAUSE");
			
			cout<<"----------"<<endl;

			scene.moverJugador(event,jugador,enemigoPrincipal);
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

			scene.cargarEscenario(enemigoPrincipal,jugador);
        }
		
    }

 system("PAUSE");
  return 0;
}
