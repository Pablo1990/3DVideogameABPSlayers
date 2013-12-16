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

	
    sf::RenderWindow window(sf::VideoMode(700,500), "Prueba de objetivos");
		//Declaramos los bot y la memoria del juego
		Bot *jugador=new Bot(100.0,0.0,0.0,10.0);
	Bot *enemigoPrincipal=new Bot(100.0,20,20,10.0);
	
	enemigoPrincipal->setEnem(jugador);
	jugador->setEnem(enemigoPrincipal);
	
	Goal_Think *g=new Goal_Think();
	
	g->setDueño(enemigoPrincipal);
	enemigoPrincipal->setBrain(g);
	Window_Scene scene=Window_Scene(enemigoPrincipal,jugador,window);

    //El programa se ejecutara mientras la ventana este abierta
	window.display();

		 while (window.isOpen())
    {
        //Reogemos y procesamos los eventos del sistema operativo
        sf::Event event;
        while (window.pollEvent(event))
        {
            //Si el evento es cerrar la ventana, cerramos la ventana que creamos antes
            if (event.type == sf::Event::Closed)
              window.close();
        }
		/*
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A))
		{
			
		 scene.j1.setFillColor(sf::Color::Green);
		 cout<<"Pulsada Y"<<endl;
		}*/
	  
    }

  system("PAUSE");
  return 0;
}