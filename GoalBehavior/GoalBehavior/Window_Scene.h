#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Bot.h";

class Window_Scene
{
public:
	Window_Scene(Bot*,Bot*,sf::RenderWindow &window);
	~Window_Scene(void);
	 sf::CircleShape j1;
	 sf:: CircleShape bot;
	  sf::RectangleShape celdas [400];
	  sf::Sprite itemspr[6];
};

