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

	void manage_collision(Weapon *w, IrrlichtDevice* d); 
	void getPesosDeFichero();
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


	//Inputs npc
	double getPosPrX()
	{
		return Clamp(this->get_position().X/1894.93,0,1);
	}
	double getPosPrY()
	{
		return Clamp(this->get_position().Z/1294.88,0,1);
	}

	//Inputs enemigo
	double getPosEnemX()
	{
		return Clamp(enemigo->get_position().X/1894.93,0,1);
	}
	double getPosEnemY()
	{
		return Clamp(enemigo->get_position().Z/1294.88,0,1);
	}
	double getOrienPr()
	{
		return fmod(this->character_node->getRotation().Y,360)/360;
	}

	double getOrienEnem()
	{
		return fmod(enemigo->character_node->getRotation().Y,360)/360;
	}

	//Inputs salud
	double getSaludEnem()
	{
		return Clamp(enemigo->get_health()/100,0,1);
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

	double* getPosXItems()
	{
		return itemsPx;
	}
	double* getPosYItems()
	{
		return itemsPy;
	}
	double* getTypeItems()
	{
		return itemsType;
	}

	//the npc fitness score. 
	double			m_dFitness;
    
	IrrlichtDevice* device;
	ITriangleSelector* mapSelector;
	Pathfinding* path;
	Position init_pos;
	Position end_pos;
	int status; //0 = Hacia salud, 1 = Hacia arma, 2 = Hacia enemigo, 3 = Explorar, 4 = freeattack?

};

