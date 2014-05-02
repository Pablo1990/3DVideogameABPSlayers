#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <math.h>
#include "Bot.h";

class Window_Scene
{
public:
	Window_Scene();
	~Window_Scene(void);
	 vector<sf:: CircleShape> botCircles;
	  sf::RectangleShape celdas [((dimMapa+dimCasilla)/dimCasilla)*((dimMapa+dimCasilla)/dimCasilla)];
	  sf::Sprite itemspr[numObjetos];
	   sf::RenderWindow window;
	   sf::Texture textura_arma;
	   sf::Texture textura_salud;
	sf::Font font;
		sf::Text Text_Aprendizaje;
	void cargarEscenario(vector<Bot*>, CController *);
	void cargarObjetos(vector<Bot*>);

};
