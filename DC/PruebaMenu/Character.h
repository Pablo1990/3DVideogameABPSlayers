#pragma once
#include <irrlicht.h>
#include <vector>
#include <iostream>
#include "Weapon.h"
#include <list>
#include "ThrowableItem.h"
#include "Shield.h"
#include "Bow.h"
#include "Spear.h"
#include "Sword.h"
#include "SoundEffect.h"
#include <string>
using namespace irr;
using namespace scene;
using namespace core;
using namespace std;

class Character
{
public:
	Character(const char* path, ISceneManager *sm);
	Character(const char* path, ISceneManager *sm, Weapon* w);
	~Character(void);
	
	ISceneNode* get_character_node();

	Weapon* get_weapon();
	void set_weapon(Weapon* w);
	
	virtual void manage_collision(Weapon* w, IrrlichtDevice* d, SoundEffect* sound = NULL);
	virtual void calculate_joint();
	virtual void defend();
	virtual void no_defend();

	void do_transformations_and_joints(vector3df position, vector3df rotation, vector3df scale);
	void add_to_scene(vector3df position, vector3df rotation, vector3df scale);
	void add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera);

	bool detect_collision(ISceneNode* a, vector<IBoneSceneNode*> b);
	void fall_down(IrrlichtDevice* device);
	void reset_fall_time(IrrlichtDevice* device);

	bool no_weapon();
	void attack(float first_x, float first_y, float last_x, float last_y);
	int heal_or_fire(ISceneNode* camp_fire, ISceneNode* heal, IrrlichtDevice* d);
	bool get_is_dead();
	void remove_character_node();

	void movement(ICameraSceneNode* camera);
	void die(IrrlichtDevice* device);

	//Salud
	double get_health();
	void set_health(double);

	//Posicion
	vector3df get_position();
	void set_position(double,double,double);

	//Usado para controlar cambios de estado
	virtual void restore_condition(IrrlichtDevice* d);

	void set_types(double* ty);
	//void replace_random_item(int index, std::list<Weapon*>* armas, IrrlichtDevice* device, 	scene::ITriangleSelector* mapSelector);
	void Character::delete_item(int index, std::list<Weapon*>* armas);


protected:

	
	IAnimatedMesh *character_mesh;//Borrar al final
	IAnimatedMeshSceneNode *character_node;
	ISceneManager *scene_manager;//No se borra aqui, solo es una referencia
	vector<IBoneSceneNode *> extremity;
	vector<IBoneSceneNode *> head;
	vector<IBoneSceneNode *> body;
	f32 last_height;
	int last_fall_time;
	Shield *sh;
	Weapon *weapon;
	double health;
	bool heal_flag;
	int heal_count;
	float heal_time;
	int slow;
	bool paralysis;
	int paralysis_start;
	int slow_start;
	bool is_dead;
	int heal_tick;
	int fire_tick;
	double* types;//Solo es una referencia

};

