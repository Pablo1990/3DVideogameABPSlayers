#pragma once
#include <irrlicht.h>
#include <vector>
#include "Weapon.h"
using namespace irr;
using namespace scene;
using namespace core;
using namespace std;
class Character
{
public:
	Character(char* path, ISceneManager *sm);
	Character(char* path, ISceneManager *sm, Weapon* w);
	~Character(void);
	void add_to_scene(vector3df position, vector3df rotation, vector3df scale);
	void add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera);
	bool detect_collision(ISceneNode* a, vector<ISceneNode*> b);
	virtual void manage_collision();
	virtual void calculate_joint();
	void attack(float first_x, float first_y, float last_x, float last_y);
protected:
	IAnimatedMesh* character_mesh;
	IAnimatedMeshSceneNode *character_node;
	ISceneManager *scene_manager;
	vector<IBoneSceneNode *> extremity;
	vector<IBoneSceneNode *> head;
	vector<IBoneSceneNode *> body;
	Weapon *weapon;
};

