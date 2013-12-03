#pragma once
#include <irrlicht.h>
#include <vector>
#include "Weapon.h"
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

	void do_transformations_and_joints(vector3df position, vector3df rotation, vector3df scale);
	void add_to_scene(vector3df position, vector3df rotation, vector3df scale);
	void add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera);
	bool detect_collision(ISceneNode* a, vector<IBoneSceneNode*> b);
	bool no_weapon();
	void attack(float first_x, float first_y, float last_x, float last_y);

protected:
	IAnimatedMesh *character_mesh;
	IAnimatedMeshSceneNode *character_node;
	ISceneManager *scene_manager;
	vector<IBoneSceneNode *> extremity;
	vector<IBoneSceneNode *> head;
	vector<IBoneSceneNode *> body;
	Weapon *weapon;
};

