#include "stdafx.h"
#include "Window_Scene.h"


Window_Scene::Window_Scene(Bot *enem,Bot *j)
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
	

   //Pintamos los dos bots
	j1=sf::CircleShape(12.5F);
	radio_vision=sf::CircleShape(100.0F);
	bot=sf::CircleShape(12.5F);
	j1.setFillColor(sf::Color::Blue);
	bot.setFillColor(sf::Color::Red);
	radio_vision.setOutlineColor(sf::Color::Green);
	radio_vision.setOutlineThickness(1.0);
	radio_vision.setFillColor(sf::Color::Transparent);
	j1.setPosition(j->getposX(),j->getposY());
	bot.setPosition(enem->getposX(),enem->getposY());

	
	//cout<<"centro"<<j1.getOrigin().x<<" "<<j1.getOrigin().x<<endl;
	
	radio_vision.setPosition((bot.getPosition().x+bot.getRadius())-radio_vision.getRadius(),(bot.getPosition().y+bot.getRadius())-radio_vision.getRadius());
	//Cargamos los item en el mapa
	list<Item> items2=enem->items;
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
		window.draw(radio_vision);
		window.draw(j1); //Dibujamos la figura
		window.draw(bot);

		//Ponemos el texto de Salud:
		font.loadFromFile("img/OpenSans.ttf");
		Text_Sb.setFont(font);
		Text_Sba.setFont(font);
		Text_Sj.setFont(font);
		Text_Sja.setFont(font);
		Text_Sb.setCharacterSize(10);
		Text_Sba.setCharacterSize(10);
		Text_Sj.setCharacterSize(10);
		Text_Sja.setCharacterSize(10);
		Text_Sb.setColor(sf::Color::Red);
		Text_Sba.setColor(sf::Color::Red);
		Text_Sj.setColor(sf::Color::Blue);
		Text_Sja.setColor(sf::Color::Blue);
		Text_Sb.setPosition(520,50);
		Text_Sba.setPosition(520,80);
		Text_Sj.setPosition(520,110);
			Text_Sja.setPosition(520,140);

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


		window.draw(Text_Sba);
		window.draw(Text_Sja);
		window.draw(Text_Sb);
		window.draw(Text_Sj);
}


Window_Scene::~Window_Scene(void)
{
}
void Window_Scene::cargarEscenario(Bot *enem,Bot *j)
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
		
	j1.setPosition(j->getposX(),j->getposY());
	bot.setPosition(enem->getposX(),enem->getposY());
		radio_vision.setPosition((bot.getPosition().x+bot.getRadius())-radio_vision.getRadius(),(bot.getPosition().y+bot.getRadius())-radio_vision.getRadius());
		window.draw(radio_vision);

		//Actualizamos la posicion


		window.draw(j1);
		window.draw(bot);
		

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
		window.draw(Text_Sja);
		 window.display();
}

void Window_Scene::moverJugador(sf::Event mov,Bot *jug_,Bot * enem_)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && j1.getPosition().y >=25)
	{
		j1.move(0,(-25));
		jug_->setPosition(jug_->getposX(),jug_->getposY()-25);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && j1.getPosition().x >=25)
	{
		j1.move((-25),0);
		jug_->setPosition(jug_->getposX()-25,jug_->getposY());
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && j1.getPosition().x <475)
	{
		j1.move(25,0);
		jug_->setPosition(jug_->getposX()+25,jug_->getposY());
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && j1.getPosition().y < 475)
	{
		j1.move(0,25);
		
		jug_->setPosition(jug_->getposX(),jug_->getposY()+25);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ((abs( jug_->getposX()-enem_->getposX())<=25) && (abs( jug_->getposY()-enem_->getposY())<=25)))
	{
		enem_->setSalud(enem_->getSalud()-5);
	}
	
}