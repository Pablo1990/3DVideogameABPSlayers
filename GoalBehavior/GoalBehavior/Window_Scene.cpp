#include "stdafx.h"
#include "Window_Scene.h"


Window_Scene::Window_Scene(Bot *enem,Bot *j,sf::RenderWindow &window)
{
	
	
	 //Dibujamos las celdas
		window.clear(sf::Color::White);
		
		int pos=0,pos2=0;
		int indice=0;
		for(int i=0;i<20;i++)
		{
			
			for(int j=0;j<20;j++)
			{
					celdas[indice].setSize(sf::Vector2f(25,25));
					celdas[indice].setOutlineColor(sf::Color::Black);
					celdas[indice].setOutlineThickness(1);
					celdas[indice].setPosition(pos2, pos);
					window.draw(celdas[indice]);
					pos2=pos2+25;
					indice++;
			
			}
			pos2=0;	
			pos=pos+25;	
		}
	

   //Pintamos los dos bots
		j1=sf::CircleShape(12.5F);
		bot=sf::CircleShape(12.5F);
	j1.setFillColor(sf::Color::Blue);
	bot.setFillColor(sf::Color::Red);
	bot.setPosition(475,475);
	//Cargamos los item en el mapa
	list<Item> items2=enem->items;
	//Tenemos seis items en el mapa
	
	int k=0;
	for (std::list<Item>::iterator it = items2.begin(); it != items2.end(); ++it)
		{
			itemspr[k].setPosition((*it).Pos.first*25,(*it).Pos.second*25);
			sf::Texture textura;
				// Cargamos la textura desde un archivo
		if((*it).typeItem==2)
			textura.loadFromFile("img/arma.png");
		else
			textura.loadFromFile("img/salud.png");

		itemspr[k].setTexture(textura);
			
		itemspr[k].setScale(0.4,0.4);
		window.draw(itemspr[k]);
		k++;
	}
		window.draw(j1); //Dibujamos la figura
		window.draw(bot);
		
}


Window_Scene::~Window_Scene(void)
{
}
