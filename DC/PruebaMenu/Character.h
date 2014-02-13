#pragma once
#include <irrlicht.h>
#include <vector>
#include "Weapon.h"
#include "ThrowableItem.h"
#include "Shield.h"
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
	
	Weapon* get_weapon();
	void set_weapon(Weapon* w);
	
	virtual void manage_collision(Weapon* w);
	virtual void calculate_joint();
	virtual void defend();
	virtual void no_defend();

	void do_transformations_and_joints(vector3df position, vector3df rotation, vector3df scale);
	void add_to_scene(vector3df position, vector3df rotation, vector3df scale);
	void add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera);
	bool detect_collision(ISceneNode* a, vector<IBoneSceneNode*> b);
	bool no_weapon();
	void attack(float first_x, float first_y, float last_x, float last_y);
	int heal_or_fire(ISceneNode* camp_fire, ISceneNode* heal, IrrlichtDevice* d);
	void movement(ICameraSceneNode* camera);

	//Salud
	double get_health();
	void set_health(double);

	//Posicion
	vector3df get_position();
	void set_position(double,double,double);

protected:
	IAnimatedMesh *character_mesh;
	IAnimatedMeshSceneNode *character_node;
	ISceneManager *scene_manager;
	vector<IBoneSceneNode *> extremity;
	vector<IBoneSceneNode *> head;
	vector<IBoneSceneNode *> body;
	Shield *sh;
	Weapon *weapon;
	double health;
	bool heal_flag;
	int heal_count;
	float heal_time;
};

