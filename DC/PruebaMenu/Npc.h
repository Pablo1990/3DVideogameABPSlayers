#pragma once

#include "Character.h"
#include "Paths.h"
#include "Bow.h"
#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include "Player.h"
#include "Position.h"
#include <math.h>
#include "Pathfinding.h"

#include "Goal_Think.h"
#include "Aprendizaje\CNeuralNet.h"

#include <IMeshManipulator.h>

using namespace std;

class Goal_Think;

class Npc: public Character
{
public:
	Npc(ISceneManager *sm,vector3df, IrrlichtDevice* d, ITriangleSelector* mp);
	Npc(ISceneManager *sm, Weapon* w,vector3df, IrrlichtDevice* d, ITriangleSelector* mp);
	Npc(ISceneManager *sm, Weapon* w,vector3df, IrrlichtDevice* d, ITriangleSelector* mp,ISceneNode* camp_fire,ISceneNode* heal);
	~Npc(void);

	void manage_collision(Weapon *w, IrrlichtDevice* d, SoundEffect* sound = NULL); 
	vector<double> getPesosDeFichero();
	bool MoverseAItemSalud();
	bool MoverseAItemArma();
	bool MoverseAEnemigo();
	bool Move_Explore();
	bool Move_ToFreeAttack();
	vector3df DarPosSalud();
	vector3df DarPosArmaCercana();

	void attack(int type);
	void attackBot(int type);
	void add_weapon_to_node(vector3df position, vector3df rotation, vector3df scale);

	//Enemigo
	void setEnem(Player*);
	void setEnem(Npc*);
	Player* getEnem();
	Npc* getEnemigo();
	bool isEnemigoPresent();

	//Mente
	void setBrain(Goal_Think*);
	Goal_Think* getBrain();

	//Lista de items de arma
	std::list<Weapon*>* getItems();
	void setItems(std::list<Weapon*>*, double* );

	void pick_weapon();

	//Moverse a un punto
	void move_to(Position p);
	void way_to(vector<Position> vp);

	void face_target(ISceneNode* target);
	void face_target(vector3df targt_pos);

	void defend();
	void no_defend();

    
	//-------------------------------------Clamp()-----------------------------------------
//
//	clamps the first argument between the second two
//
//-------------------------------------------------------------------------------------
double Clamp(double arg, double min, double max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
	return arg;
}
	//Aprendizaje

	//resetea los valores a los iniciales, para la siguiente iteracion
	void Reset(); 

	//Metodos Aprendizaje

	//updates the ANN with information from the sweepers enviroment
	bool			Update();

	void			IncrementFitness(){++m_dFitness;}

	double		    Fitness()const{return m_dFitness;}
	void			setFitness(double fit) {m_dFitness=fit;}

	void              PutWeights(vector<double> &w){m_ItsBrain.PutWeights(w);}

	int               GetNumberOfWeights()const{return m_ItsBrain.GetNumberOfWeights();}

	vector<int>       CalculateSplitPoints()const{return m_ItsBrain.CalculateSplitPoints();}
    
    
	void drop_shield();
	void pick_shield();
    void set_pathfinding(Pathfinding *pf);
		void Npc :: posEntreCeroYUno(double &x,double &y)
	{
		
		y = y/(sqrt(2)*1294.88);
		Clamp(y, -1, 1);
		x = x/(sqrt(2)*1894.93);
		Clamp(x, -1, 1);
	}

void Npc :: getPosRelativaABot(double &x, double &y){
	x =x - this->get_position().X;
	y = y - this->get_position().Z;
}

double Npc::getDistanciaABot(double x, double y){

	return sqrt(pow(this->get_position().X-x,2) + pow(this->get_position().Z-y,2));
}
	
	//Inputs enemigo
	double getPosEnemX()
	{
		if(enemigo!=NULL)
		{
			double x=enemigo->get_position().X;
			double y=enemigo->get_position().Z;
			getPosRelativaABot(x,y);
			return x;
		}			
		else
		{
			double x=player->get_position().X;
			double y=player->get_position().Z;
			getPosRelativaABot(x,y);
			return x;
		}
			
	}
	double getPosEnemY()
	{
		if(enemigo!=NULL)
		{
			double x=enemigo->get_position().X;
			double y=enemigo->get_position().Z;
			getPosRelativaABot(x,y);
			return y;
		}			
		else
		{
			double x=player->get_position().X;
			double y=player->get_position().Z;
			getPosRelativaABot(x,y);
			return y;
		}
	}
	
private:
	Goal_Think *mente;
	Player *player;
	Npc* enemigo;
	std::list<Weapon*>* items;
	ISceneNode* camp_fire;
	ISceneNode* heal;
	double desgastes[4];
	double itemsPx[6];
	double itemsPy[6];
	double* itemsType;

	vector3df posHealth;
	Weapon* near_weapon;
	bool is_moving;
	int steps_count;

	//Aprendizaje
	CNeuralNet  m_ItsBrain;




	//Hacer orientacion con respecto a mi
	double getOrienEnem()
	{
		if(enemigo!=NULL)
			return (fmod(enemigo->character_node->getRotation().Y,360) - fmod(this->character_node->getRotation().Y, 360))/360;
		else
			return (fmod(player->get_character_node()->getRotation().Y,360) - fmod(this->character_node->getRotation().Y, 360))/360;
	}

	//Inputs salud
	double getSaludEnem()
	{
		if(enemigo!=NULL)
			return Clamp(enemigo->get_health()/100,0,1);
		else
			return Clamp(player->get_health()/100,0,1);
	}
	double getSaludPr()
	{
		return Clamp(this->get_health()/100,0,1);
	}

	//Inputs desgaste
	
	double* getDesgastePr()
	{
		desgastes[0] = dynamic_cast<Sword*>(this->get_weapon()) ? get_weapon()->get_resist()/15 : 0;
		desgastes[1] = dynamic_cast<Bow*>(this->get_weapon()) ? get_weapon()->get_resist()/15 : 0;
		desgastes[2] = dynamic_cast<Spear*>(this->get_weapon()) ? get_weapon()->get_resist()/15 : 0;
		desgastes[3] = dynamic_cast<ThrowableItem*>(this->get_weapon()) ? get_weapon()->get_resist() : 0;

		
		return desgastes;
	}
	
	void getPosItemMasCercano(double &x, double &y){
		double distancia = 9999999;
		double distanciaAct;
		for(int i=0; i<6; i++){
			distanciaAct = getDistanciaABot(itemsPx[i], itemsPy[i]);
			if(distancia > distanciaAct){
				x = itemsPx[i];
				y = itemsPy[i];
				distancia = distanciaAct;
			}
		}
	}

	//Creemos que debemos quitarlo
	/*double* getTypeItems()
	{
		return itemsType;
	}*/

	//the npc fitness score. 
	double			m_dFitness;
    
	IrrlichtDevice* device;
	ITriangleSelector* mapSelector;
	Pathfinding* path;
	Position init_pos;
	Position end_pos;
	int status; //0 = Hacia salud, 1 = Hacia arma, 2 = Hacia enemigo, 3 = Explorar, 4 = freeattack?

};

