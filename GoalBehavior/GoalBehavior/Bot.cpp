#include "stdafx.h"
#include "Bot.h"


Bot::Bot(double salud_,double posx_,double posy_,double arma_, double probabilities_[5])
{
	salud=salud_;
	Pos.first=posx_;
	Pos.second=posy_;
	arma=arma_;
	
	for(int j=0; j<5; j++)
	{
		probabilities[j] = probabilities_[j];
	}

	//Meto aqui los items para poner por el mapa
		
		pair<double,double> pos;
		pos.first=15*25;
		pos.second=10*25;
		Item i=Item(pos,2);
		//item
		items.push_front(i);
		//item
		pos.first=19*25;
		pos.second=5*25;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=5*25;
		pos.second=15*25;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=12*25;
		pos.second=5*25;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=4*25;
		pos.second=5*25;
		i=Item::Item(pos,2);
		items.push_front(i);
		//item
		pos.first=15*25;
		pos.second=0*25;
		i=Item::Item(pos,1);
		items.push_front(i);

}


Bot::~Bot(void)
{
}

double Bot:: getArma()
{
	return arma;
}

Bot* Bot::getEnem()
{

	return enemigo;
}

void Bot::setEnem(Bot* bot_)
{
	
	enemigo=bot_;
}
void Bot::setBrain(Goal_Think* brain_)
{
	
	mente=brain_;
}


//Si el bot enemigo esta dentro de un rango entonces dirá que si esta presente y entonces pueda realizar el objetivo de ataque.
bool Bot::isEnemigoPresent()
{
	if(enemigo !=NULL)
	{
		int x_E=enemigo->Pos.first;
		int y_E=enemigo->Pos.second;
		int x=Pos.first;
		int y=Pos.second;
		int distaux=sqrt(pow((x-x_E),2)+pow((y-y_E),2));
		if(distaux <=Distancia_Max_Vision)
		{
			return true;
		}
		//cout<<"entro Distancia:"<<distaux<<endl;
	}
	return false;

}

double Bot::getProbAttack(){
	return probabilities[0];
}
	
double Bot::getProbEscape(){
	return probabilities[1];
}
	
double Bot::getProbExplore(){
	return probabilities[2];
}
	
double Bot::getProbGetHealth(){
	return probabilities[3];
}
	
double Bot::getProbGetWeapon(){
	return probabilities[4];
}

double Bot::getSalud()
{
	return salud;
}
pair<double,double> Bot::getPos()
{
	return Pos;


}
pair<double,double> Bot::DarPosSalud()
{
	for (std::list<Item>::iterator it = items.begin();
       it != items.end();
       ++it)
		{
			if((*it).typeItem==1)
			{
				return (*it).Pos;
			}
		}

}
pair<double,double> Bot::DarPosArmaCercana()
{
	int distancia=9999.9;
	pair<double,double> pos;
	for (std::list<Item>::iterator it = items.begin();
       it != items.end();
       ++it)
		{
			if((*it).typeItem==2)
			{
				
				double distaux=sqrt((pow((Pos.first-(*it).Pos.first),2))+(pow((Pos.second-(*it).Pos.second),2)));
				//Estandarizamos
				
				
				if(distaux <=distancia)
				{
					distancia=distaux;
					pos=(*it).Pos;
				}
			}
		}
	return pos;

}
bool Bot::MoverseAItemArma()
{
	bool muevo=false;
	int ran=rand()%8;
	int posx=0;
	int posy=0;
	pair<double,double> pos=DarPosArmaCercana();
	//cout<<"rand"<<ran<<pos.first<<"   "<<pos.second<<";"<<endl;
	//cout<<"Me muevo"<<endl;
	while(muevo ==false)
	{
		switch (ran)
		{
		case 0:	posx=pos.first-25;
				posy=pos.second-25;
				if(posx>=0  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 1:	posx=pos.first;
				posy=pos.second-25;
		if(posx>=0  && posy>=0)
		{
			muevo=true;
			Pos.first=posx;
			Pos.second=posy;
		}		
		break;
		case 2:	posx=pos.first+25;
				posy=pos.second-25;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 3:	posx=pos.first+25;
				posy=pos.second;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;

		case 4:	posx=pos.first+25;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 5:	posx=pos.first;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 6:	posx=pos.first-25;
				posy=pos.second+25;
				if(posx>=0  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 7:	posx=pos.first-25;
			posy=pos.second;
			if(posx>=0  && posy<=475)
			{
				muevo=true;
				Pos.first=posx;
				Pos.second=posy;
			}		
			break;
		
		
		default:
			break;
		}
		ran=rand()%8;
	}
	
	return true;

}
bool Bot::MoverseAEnemigo()
{
	bool muevo=false;
	int ran=rand()%8;
	int posx=0;
	int posy=0;
	pair<double,double> pos=getEnem()->getPos();
	//cout<<"rand"<<ran<<pos.first<<"   "<<pos.second<<";"<<endl;
	//cout<<"Me muevo"<<endl;
	while(muevo ==false)
	{
		switch (ran)
		{
		case 0:	posx=pos.first-25;
				posy=pos.second-25;
				if(posx>=0  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 1:	posx=pos.first;
				posy=pos.second-25;
		if(posx>=0  && posy>=0)
		{
			muevo=true;
			Pos.first=posx;
			Pos.second=posy;
		}		
		break;
		case 2:	posx=pos.first+25;
				posy=pos.second-25;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 3:	posx=pos.first+25;
				posy=pos.second;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;

		case 4:	posx=pos.first+25;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 5:	posx=pos.first;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 6:	posx=pos.first-25;
				posy=pos.second+25;
				if(posx>=0  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 7:	posx=pos.first-25;
			posy=pos.second;
			if(posx>=0  && posy<=475)
			{
				muevo=true;
				Pos.first=posx;
				Pos.second=posy;
			}		
			break;
		
		
		default:
			break;
		}
		ran=rand()%8;
	}
	return true;


}
bool Bot::Move_Explore()
{
	bool  muevo=false;
	int ran=rand()%8;
	int posx=0;
	int posy=0;
	pair<double,double> pos=getPos();
	
	while(muevo ==false)
	{
		
		switch (ran)
		{
		case 0:	posx=pos.first-25;
				posy=pos.second-25;
				if(posx>=0  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 1:	posx=pos.first;
				posy=pos.second-25;
		if(posx>=0  && posy>=0)
		{
			muevo=true;
			Pos.first=posx;
			Pos.second=posy;
		}		
		break;
		case 2:	posx=pos.first+25;
				posy=pos.second-25;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 3:	posx=pos.first+25;
				posy=pos.second;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;

		case 4:	posx=pos.first+25;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 5:	posx=pos.first;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 6:	posx=pos.first-25;
				posy=pos.second+25;
				if(posx>=0  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 7:	posx=pos.first-25;
			posy=pos.second;
			if(posx>=0  && posy<=475)
			{
				muevo=true;
				Pos.first=posx;
				Pos.second=posy;
			}		
			break;
		default:
			break;
		}
		ran=rand()%8;
	}
	//int pos
	return true;

}
bool Bot::Move_ToFreeAttack()
{
	bool  muevo=false;
	int ran=rand()%8;
	int posx=0;
	int posy=0;
	pair<double,double> pos=getPos();
	cout<<pos.first<<" "<<pos.second<<endl;
	while(muevo ==false)
	{
		
		switch (ran)
		{
		case 0:	posx=pos.first-(25*5);
				posy=pos.second-(25*5);
				if(posx>=0  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 1:	posx=pos.first;
				posy=pos.second-(25*5);
		if(posx>=0  && posy>=0)
		{
			muevo=true;
			Pos.first=posx;
			Pos.second=posy;
		}		
		break;
		case 2:	posx=pos.first+(25*5);
				posy=pos.second-(25*5);
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 3:	posx=pos.first+(25*5);
				posy=pos.second;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;

		case 4:	posx=pos.first+(25*5);
				posy=pos.second+(25*5);
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 5:	posx=pos.first;
				posy=pos.second+(25*5);
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 6:	posx=pos.first-(25*5);
				posy=pos.second+(25*5);
				if(posx>=0  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 7:	posx=pos.first-(25*5);
			posy=pos.second;
			if(posx>=0  && posy<=475)
			{
				muevo=true;
				Pos.first=posx;
				Pos.second=posy;
			}		
			break;
		default:
			break;
		}
		ran=rand()%8;
	}
	//int pos
	return true;

}
bool Bot::MoverseAItemSalud()
{
	bool muevo=false;
	int ran=rand()%8;
	int posx=0;
	int posy=0;
	pair<double,double> pos=DarPosSalud();
	while(muevo ==false)
	{
		switch (ran)
		{
		case 0:	posx=pos.first-25;
				posy=pos.second-25;
				if(posx>=0  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 1:	posx=pos.first;
				posy=pos.second-25;
		if(posx>=0  && posy>=0)
		{
			muevo=true;
			Pos.first=posx;
			Pos.second=posy;
		}		
		break;
		case 2:	posx=pos.first+25;
				posy=pos.second-25;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 3:	posx=pos.first+25;
				posy=pos.second;
				if(posx<=475  && posy>=0)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;

		case 4:	posx=pos.first+25;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 5:	posx=pos.first;
				posy=pos.second+25;
				if(posx<=475  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 6:	posx=pos.first-25;
				posy=pos.second+25;
				if(posx>=0  && posy<=475)
				{
					muevo=true;
					Pos.first=posx;
					Pos.second=posy;
				}		
				break;
		case 7:	posx=pos.first-25;
			posy=pos.second;
			if(posx>=0  && posy<=475)
			{
				muevo=true;
				Pos.first=posx;
				Pos.second=posy;
			}		
			break;
		
		
		default:
			break;
		}
		ran=rand()%8;
	}
	//int pos
	return true;

}
void Bot::setArma(double est_arma)
{
	arma=est_arma;

}
void Bot::setPosition(double posx_,double posy_)
{
	Pos.first=posx_;
	Pos.second=posy_;
}
void Bot::setSalud(double salud_)
{
	salud=salud_;

}

Goal_Think* Bot::getBrain()
{
	return mente;
}