// GoalBehavior.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>


int _tmain(int argc, _TCHAR* argv[])
{
	 //Creamos una ventana de 200x200 con el titulo "SFML works!"
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
     
    //Creamos una figura geométrica ( un circulo )
    //con las funciones internas de SFML2
    sf::CircleShape shape(100.f);
     
    //Lo rellenamos de verde
    shape.setFillColor(sf::Color::Green);
 
    //El programa se ejecutara mientras la ventana este abierta
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
 
        //Dibujamos
        window.clear();     //Limpiamos la ventana
        window.draw(shape); //Dibujamos la figura
        window.display();   //Mostramos todo lo que se ha dibujado
    }
 
    return 0;   //Finalizamos la aplicación
}

