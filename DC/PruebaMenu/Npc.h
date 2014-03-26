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

#include "Goal_Think.h"

#include <IMeshManipulator.h>

class Goal_Think;

class Npc: public Character
{
public:
	Npc(ISceneManager *sm,vector3df);
	Npc(ISceneManager *sm, Weapon* w,vector3df);
	~Npc(void);

	void manage_collision(Weapon *w, IrrlichtDevice* d); 

	bool MoverseAItemSalud();
	bool MoverseAItemArma();
	bool MoverseAEnemigo();
	bool Move_Explore();
	bool Move_ToFreeAttack();
	vector3df DarPosSalud();
	vector3df DarPosArmaCercana();
	void attack(int type);
	
	void add_weapon_to_node(vector3df position, vector3df rotation, vector3df scale);
	
	//Enemigo
	void setEnem(Player*);
	Player* getEnem();
	bool isEnemigoPresent();

	//Mente
	void setBrain(Goal_Think*);
	Goal_Think* getBrain();

	//Lista de items de arma
	std::list<Weapon*> getItems();
	void setItems(std::list<Weapon*> );

	void pick_weapon();

	//Moverse a un punto
	void move_to(Position p);
	void way_to(vector<Position> vp);

	void face_target(ISceneNode* target);
	void face_target(vector3df targt_pos);
private:
	Goal_Think *mente;
	Player *player;
	std::list<Weapon*> items;
	vector3df posHealth;
	Weapon* near_weapon;
	bool is_moving;
	int steps_count;	
};

