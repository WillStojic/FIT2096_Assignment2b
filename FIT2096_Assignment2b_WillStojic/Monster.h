/*	FIT2096 - Assignment 1 Sample Solution
*	Monster.h
*	Created by Mike Yeates - 2017 - Monash University
*	A simpe object which has a health, can attack, and can be hit.
*/

#ifndef MONSTER_H
#define MONSTER_H

#include "GameObject.h"
#include "TextureManager.h"

class Monster : public GameObject
{
private:

	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	Texture* GetTextureForVar();
public:
	Monster();
	Monster(Mesh* mesh, Shader* shader, TextureManager* textureManager, Vector3 position);
	~Monster();

	void Update(float timestep);
	void Update(float timestep, GameObject* player);
};

#endif