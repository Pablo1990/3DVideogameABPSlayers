#include "stdafx.h"
#include "Window_Scene.h"


Window_Scene::Window_Scene()
{
	//Dibujamos las celdas
	window.create(sf::VideoMode(800,500), "Prueba de objetivos");
	window.clear(sf::Color::White);

	int pos=0,pos2=0;
	int indice=0;
	for(int i=0;i<(dimMapa+dimCasilla)/dimCasilla;i++)
	{
		for(int j=0;j<(dimMapa+dimCasilla)/dimCasilla;j++)
		{
			celdas[indice].setSize(sf::Vector2f(dimCasilla,dimCasilla));
			celdas[indice].setOutlineColor(sf::Color::Black);
			celdas[indice].setOutlineThickness(1);
			celdas[indice].setPosition(pos2, pos);
			window.draw(celdas[indice]);
			pos2=pos2+dimCasilla;
			indice++;

		}
		pos2=0;	
		pos=pos+dimCasilla;	
	}

	textura_arma.loadFromFile("img/arma.png");
	font.loadFromFile("img/OpenSans.ttf");

}

void Window_Scene::cargarObjetos(vector<Bot*> bots){

	for(int i=0; i<bots.size(); i++){
		botCircles.push_back(sf::CircleShape(12.5F));
		botCircles[i].setFillColor(sf::Color::Blue);
		botCircles[i].setPosition(bots[i]->getposX(), bots[i]->getposY());
		window.draw(botCircles[i]);
	}


	//cout<<"centro"<<j1.getOrigin().x<<" "<<j1.getOrigin().x<<endl;

	//Cargamos los item en el mapa
	//Tenemos seis items en el mapa
	
	int k=0;
	
	
	for (int i=0; i<bots[0]->itemsX.size(); i++)
	{
		itemspr[k].setPosition(*bots[0]->itemsX[i], *bots[0]->itemsY[i]);
		itemspr[k].setTexture(textura_arma);
		itemspr[k].setScale(0.4,0.4);
		window.draw(itemspr[k]);
		k++;
	}

	Text_Aprendizaje.setFont(font);
	Text_Aprendizaje.setCharacterSize(10);
	Text_Aprendizaje.setColor(sf::Color::Magenta);
	Text_Aprendizaje.setPosition(510, 170);

	std::stringstream textAprendizaje;
	textAprendizaje << "--------------------" <<endl;
	for(int i=0; i<bots.size(); i++){
		textAprendizaje << "Fitness "<<i<<": "<< bots[i]->Fitness()<<endl;
	}
	Text_Aprendizaje.setString(textAprendizaje.str());
	window.draw(Text_Aprendizaje);
	window.display();
}

Window_Scene::~Window_Scene(void)
{
}
void Window_Scene::cargarEscenario(vector<Bot*> bots, CController* controller)
{
	window.clear(sf::Color::White);
	//Pintar celdas

	for(int i=0;i<((dimMapa+dimCasilla)/dimCasilla) * ((dimMapa+dimCasilla)/dimCasilla);i++)
	{		
		window.draw(celdas[i]);

	}
	for(int i=0;i<bots[0]->itemsX.size();i++)
	{
		itemspr[i].setPosition(*bots[0]->itemsX[i], *bots[0]->itemsY[i]);
		window.draw(itemspr[i]);
	}

	for(int i=0; i<bots.size(); i++){
		botCircles[i].setPosition(bots[i]->getposX(),bots[i]->getposY());
		window.draw(botCircles[i]);
	}

	Text_Aprendizaje.setFont(font);
	Text_Aprendizaje.setCharacterSize(10);
	Text_Aprendizaje.setColor(sf::Color::Magenta);
	Text_Aprendizaje.setPosition(510, 10);

	std::stringstream textAprendizaje;
	textAprendizaje << "--------------------" <<endl;
	textAprendizaje << "Generacion: "<<controller->getGeneracion()<<endl;
	double x,y = 0;
	for(int i=0; i<bots.size(); i++){
		textAprendizaje << "Fitness "<<i<<": "<< bots[i]->Fitness()<<" - ";
		bots[i]->getPosMasCercano(x,y);
		textAprendizaje << "Distancia a cercano: ("<<x<<","<<y<<")"<<endl;
	}
	Text_Aprendizaje.setString(textAprendizaje.str());
	window.draw(Text_Aprendizaje);
	window.display();
}
