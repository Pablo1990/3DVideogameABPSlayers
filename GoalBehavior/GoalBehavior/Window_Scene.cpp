#include "stdafx.h"
#include "Window_Scene.h"


Window_Scene::Window_Scene()
{
	 //Dibujamos las celdas
	window.create(sf::VideoMode(700,500), "Prueba de objetivos");
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
	

   //Pintamos los dos bot
	
}

void Window_Scene::cargarObjetos(vector<Bot*> bots){

	for(int i=0; i<bots.size(); i++){
			botCircles[i] = sf::CircleShape(12.5F);
			botCircles[i].setFillColor(sf::Color::Blue);
			botCircles[i].setPosition(bots[i]->getposX(), bots[i]->getposY());
			window.draw(botCircles[i]);
	}

	
	//cout<<"centro"<<j1.getOrigin().x<<" "<<j1.getOrigin().x<<endl;
	
	//Cargamos los item en el mapa
	list<Item> items2=bots[0]->items;
	//Tenemos seis items en el mapa
	
	int k=0;
	for (std::list<Item>::iterator it = items2.begin(); it != items2.end(); ++it)
		{
			itemspr[k].setPosition((*it).Pos.first,(*it).Pos.second);
			
				// Cargamos la textura desde un archivo
		if((*it).typeItem==2)
		{
			textura_arma.loadFromFile("img/arma.png");
			itemspr[k].setTexture(textura_arma);
		}
			
		else
		{
			
			textura_salud.loadFromFile("img/salud.png");
			itemspr[k].setTexture(textura_salud);
		}

		
			
		itemspr[k].setScale(0.4,0.4);
		window.draw(itemspr[k]);
		k++;
	}

	Text_Aprendizaje.setCharacterSize(10);
	Text_Aprendizaje.setColor(sf::Color::Magenta);
	Text_Aprendizaje.setPosition(520, 170);

	std::stringstream textAprendizaje;
	textAprendizaje << "--------------------" <<endl;
	for(int i=0; i<bots.size(); i++){
		textAprendizaje << "Fitness "<<i<<": "<< bots[i]->Fitness()<<endl;
	}
	Text_Aprendizaje.setString(textAprendizaje.str());
	window.draw(Text_Aprendizaje);
}

Window_Scene::~Window_Scene(void)
{
}
void Window_Scene::cargarEscenario(vector<Bot*> bots)
{
	window.clear(sf::Color::White);
	//Pintar celdas
	
		for(int i=0;i<400;i++)
		{		
					window.draw(celdas[i]);
				
		}
		for(int i=0;i<6;i++)
		{
			window.draw(itemspr[i]);
		}
		
		for(int i=0; i<bots.size(); i++){
			botCircles[i].setPosition(bots[i]->getposX(),bots[i]->getposY());
			window.draw(botCircles[i]);
		}
		
	/*
		//tEXTO
			int salud=enem->getSalud();
		std::stringstream bot_;
		std::stringstream jug;
		std::stringstream bota_;
		std::stringstream juga;
	// Obtenemos las cadenas desde los puntos
	bot_ << "Salud Bot "<< salud;
	Text_Sb.setString(bot_.str());
		salud=j->getSalud();
		
	jug << "Salud Jugador "<< salud;
		Text_Sj.setString(jug.str());
		salud=enem->getArma();

	bota_<<"Estado  Arma de Bot "<<salud;
	Text_Sba.setString(bota_.str());
		salud=j->getArma();
	juga << "Estado  Arma de Jugador "<< salud;
		Text_Sja.setString(juga.str());


		window.draw(Text_Sb);
		window.draw(Text_Sj);
		window.draw(Text_Sba);
		window.draw(Text_Sja);*/
		Text_Aprendizaje.setCharacterSize(10);
	Text_Aprendizaje.setColor(sf::Color::Magenta);
	Text_Aprendizaje.setPosition(520, 170);

	std::stringstream textAprendizaje;
	textAprendizaje << "--------------------" <<endl;
	for(int i=0; i<bots.size(); i++){
		textAprendizaje << "Fitness "<<i<<": "<< bots[i]->Fitness()<<endl;
	}
	Text_Aprendizaje.setString(textAprendizaje.str());
	window.draw(Text_Aprendizaje);
		window.display();
}
