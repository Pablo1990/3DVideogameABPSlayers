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
	Weapon(const char* path, int dmg, int sp, ISceneManager *sm);
	virtual void attack(float first_x, float first_y, float last_x, float last_y);
	void add_to_scene(vector3df position, vector3df rotation, vector3df scale, bool pickable);
	void add_to_camera(vector3df position,vector3df rotation, vector3df scale, ICameraSceneNode* camera);
	~Weapon(void);
protected:
	int damage;
	int speed;
	IAnimatedMesh* weapon_mesh;
	IAnimatedMeshSceneNode *weapon_node;
	ISceneManager *scene_manager;
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

