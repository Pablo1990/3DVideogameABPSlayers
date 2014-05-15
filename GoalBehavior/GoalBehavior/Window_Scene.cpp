#include "stdafx.h"
#include "Window_Scene.h"


Window_Scene::Window_Scene()
{
	//Dibujamos las celdas
	window.create(sf::VideoMode(1300,700), "Prueba de objetivos");
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

void Window_Scene::setIndividuoElegido(int x, int y, vector<Bot*> bots){
	
		bool ele = false;
		x = (x/dimCasilla)*25;
		y = (y/dimCasilla)*25;
	if(x<=dimMapa && y<=dimMapa){
		for(int i = bots.size()-1; i>=0; i--){
			if(!ele && botCircles[i].getPosition().x == x && y == botCircles[i].getPosition().y){
				ele = true;
				bots[i]->setElegido(true);
				botCircles[i].setFillColor(sf::Color::Yellow);
			}
			else{
				bots[i]->setElegido(false);
				botCircles[i].setFillColor(sf::Color::Blue);
			}
		}
	}
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
		textAprendizaje << "Distancia a cercano: ("<<x<<","<<y<<")";
		if(bots[i]->getElegido())
			textAprendizaje<<" elegido";
		
		textAprendizaje<<endl;
	}
	textAprendizaje<<"Cajas cogidas "<<controller->cajas<<endl;
	Text_Aprendizaje.setString(textAprendizaje.str());
	plotNeuralNet(bots, controller);
	window.draw(Text_Aprendizaje);
	window.display();
}

void Window_Scene::plotNeuralNet(vector<Bot*> bots, CController* controller){

	CNeuralNet aux;
	int numIndividuo = 0; //cogemos el que tiene mayor fitness

	for(int i=bots.size()-1; i>=0; i--){
		if(bots[i]->getElegido()){
			numIndividuo = i;
			break;
		}
	}

	aux = bots[numIndividuo]->GetNeuralNet();
	std::stringstream textAprendizaje;
	Text_Red.setFont(font);
	Text_Red.setCharacterSize(10);
	Text_Red.setColor(sf::Color::Magenta);
	Text_Red.setPosition(10, 550);
	int pos=120;
	sf:: Color  c;
	int cont=0;
    NeuronCircles.clear();
	for (int i=0; i<aux.m_NumHiddenLayers + 1; ++i)
	{
		//for each neuron
		pos=750;

	   if(i==aux.m_NumHiddenLayers)
		{
			c=sf::Color::Red;
		}
		else
		{
			c=sf::Color::Black;
		}

		for (int j=0; j<aux.m_vecLayers[i].m_NumNeurons; ++j)
		{
			//for each weight
			textAprendizaje <<"NeuronWeights" + itos(i+1) <<" ";
			textAprendizaje <<  "[";
			if(i==0 && j==0)
			{
				pos=820;

			}
			for (int k=0; k<aux.m_vecLayers[i].m_vecNeurons[j].m_NumInputs; ++k)
			{
				textAprendizaje <<ftos(aux.m_vecLayers[i].m_vecNeurons[j].m_vecWeight[k])+",";
				if(i==0 && j==0)
				{
					if(k<aux.m_vecLayers[i].m_vecNeurons[j].m_NumInputs-1)
					{
						c=sf::Color::Green;
						NeuronCircles.push_back(sf::CircleShape(9.5F));
						//NeuronAristas.push_back(sf::RectangleShape(sf::Vector2f(120, 100)));
						//NeuronAristas[cont].setFillColor(c);
						NeuronCircles[cont].setFillColor(c);
						pos=pos+(50);
						NeuronCircles[cont].setPosition(pos,(50+(70*i)));
						//window.draw(NeuronAristas[cont]);
						window.draw(NeuronCircles[cont]);
						cont++;
					}	
				}
				
			}
			if(i==0 && j==0)
			{
					pos=750;
			}
			if(i==aux.m_NumHiddenLayers)
			{
				c=sf::Color::Red;
			}
			else
			{
				c=sf::Color::Black;
			}
			
			NeuronCircles.push_back(sf::CircleShape(9.5F));
			NeuronCircles[cont].setFillColor(c);
			pos=pos+(50);
			NeuronCircles[cont].setPosition(pos,(50+(70*(i+1))));
			window.draw(NeuronCircles[cont]);
			cont++;
			textAprendizaje << "]"<<" - ";
		}
		textAprendizaje <<endl;
	}

	textAprendizaje << "Aciertos " +itos(bots[numIndividuo]->getArmasCogidas());
	textAprendizaje<<endl;

	double x, y;
	bots[numIndividuo]->getPosMasCercano(x,y);
	textAprendizaje << "distanciaEnXY: "<<"("<<x<<", "<<y<<")"<<endl;
	textAprendizaje << "ouputs: ";

	for (int k=0; k<aux.m_NumHiddenLayers + 1; ++k)
	{
		textAprendizaje << "Capa "<<k;
		//for each neuron
		for (int p=0; p<aux.m_vecLayers[k].m_NumNeurons; ++p)
		{
			textAprendizaje << " " + ftos(aux.outputs[k][p]);	
		}
		textAprendizaje<<endl;
	}
	
	textAprendizaje<<"Fitness "<<bots[numIndividuo]->Fitness()<<endl;

	Text_Red.setString(textAprendizaje.str());
	window.draw(Text_Red);
	//window.display();
	//}
}
