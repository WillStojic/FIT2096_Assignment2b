/*	FIT2096 - Example Code
*	Game.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class is the heart of our game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Direct3D.h"
#include "FirstPersonCamera.h"
#include "InputController.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "StaticObject.h"
#include "Player.h"
#include "BulletFactory.h"
#include <vector>

#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

class Game
{
private:
	Direct3D* m_renderer;
	InputController* m_input;
	FirstPersonCamera* m_currentCam;		
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	CollisionManager* m_collisionManager;

	Shader* m_diffuseTexturedShader;
	Shader* m_diffuseTexturedFogShader;

	// Sprites / Fonts
	SpriteBatch* m_spriteBatch;
	SpriteFont* m_arialFont12;
	SpriteFont* m_arialFont18;

	// Our game data.
	Player* m_player;
	StaticObject* m_ground;
	BulletFactory* m_bulletFactory;

	std::wstring m_gemsCollectedText;
	std::wstring m_scoreText;
	std::wstring m_ammoText;
	Texture* m_hurtOverlay;
	Texture* m_healthBar;
	
	// Splitting initialisation up into several steps
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void LoadFonts();
	void InitGameWorld();

	void InitUI();
	void DrawUI();
	void RefreshUI();

	void CheckGameOver();

public:
	Game();	
	~Game();

	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	void Shutdown(); //Cleanup everything we initialised
};

#endif