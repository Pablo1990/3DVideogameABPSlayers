#pragma once
#include <irrlicht.h>
#include <string>
using namespace irr;
using namespace std;
using namespace core;
using namespace scene;

class Weapon
{
public:
	
	Weapon(const char* path, int dmg, int sp, ISceneManager *sm, int t);
	~Weapon(void);

	void set_weapon_node(IAnimatedMeshSceneNode* wn);
	vector3df get_absolute_position();
	bool get_collision_flag();
	void set_collision_flag(bool cf);
	IAnimatedMeshSceneNode* get_weapon_node();

	int get_damage();
	void set_damage(int d);

	virtual void finish_animation();
	virtual void attack(float first_x, float first_y, float last_x, float last_y);
	virtual void attack(int type, IAnimatedMeshSceneNode* node, vector3df player_position);
	bool with_shield();

	void add_to_scene(vector3df position, vector3df rotation, vector3df scale, bool pickable);
	void add_to_camera(vector3df position,vector3df rotation, vector3df scale, ISceneNode* camera);
	void add_to_node(vector3df position,vector3df rotation, vector3df scale, ISceneNode* node);
	virtual bool is_animated();

	//Obtener y cambiar el desgaste de arma
	void set_resist(double);
	double get_resist();

	int get_type();


protected:
	int damage;
	int speed;
	double resist;

	bool collision_flag;
	IAnimatedMesh* weapon_mesh;
	IAnimatedMeshSceneNode *weapon_node;
	ISceneManager *scene_manager;
	int ty;
	vector3df main_rotation;
	vector3df main_position;
	bool shield;

private:
	enum
	{
		// I use this ISceneNode ID to indicate a scene node that is
		// not pickable by getSceneNodeAndCollisionPointFromRay()
		ID_IsNotPickable = 0,

		// I use this flag in ISceneNode IDs to indicate that the
		// scene node can be picked by ray selection.
		IDFlag_IsPickable = 1 << 0,

		// I use this flag in ISceneNode IDs to indicate that the
		// scene node can be highlighted.  In this example, the
		// homonids can be highlighted, but the level mesh can't.
		IDFlag_IsHighlightable = 1 << 1
	};
};

