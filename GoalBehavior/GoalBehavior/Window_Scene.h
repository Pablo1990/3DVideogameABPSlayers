#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Bot.h";

class Window_Scene
{
public:
	Window_Scene(Bot*,Bot*);
	~Window_Scene(void);
	 sf::CircleShape j1;
	 sf:: CircleShape bot;
	 sf::CircleShape radio_vision;
	  sf::RectangleShape celdas [400];
	  sf::Sprite itemspr[6];
	   sf::RenderWindow window;
	   sf::Texture textura_arma;
	   sf::Texture textura_salud;
	sf::Font font;
	sf::Text Text_Sb;
		sf::Text Text_Sj;
		sf::Text Text_Sba;
		sf::Text Text_Sja;
		void moverJugador(sf::Event);
		
	void cargarEscenario();

};

