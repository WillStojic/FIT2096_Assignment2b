/*	FIT2096 - Assignment 1 Sample Solution
*	Game.h
*	Created by Mike Yeates - 2017 - Monash University
*/

#include "Game.h"
#include "TexturedShader.h"
#include "StaticObject.h"
#include <sstream>

#include "DirectXTK/CommonStates.h"

Game::Game()
{
	m_renderer = NULL;
	m_currentCam = NULL;
	m_input = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_collisionManager = NULL;
	m_diffuseTexturedShader = NULL;
	m_diffuseTexturedFogShader = NULL;
	m_ground = NULL;
	m_bulletFactory = NULL;
	m_player = NULL;
	m_gameBoard = NULL;

	m_spriteBatch = NULL;
	m_arialFont12 = NULL;
	m_arialFont18 = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{
	m_renderer = renderer;	
	m_input = input;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();
	m_currentCam = new FirstPersonCamera(m_input);

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	m_player = new Player(m_input);
	m_bulletFactory = new BulletFactory(m_meshManager, m_textureManager, m_diffuseTexturedFogShader);

	LoadFonts();
	InitUI();
	InitGameWorld();
	RefreshUI();

	m_collisionManager = new CollisionManager(m_player, m_bulletFactory, m_gameBoard);

	GameOver = false;

	return true;
}

bool Game::InitShaders()
{
	m_diffuseTexturedShader = new TexturedShader();
	if (!m_diffuseTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedFogShader = new TexturedShader();
	if (!m_diffuseTexturedFogShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedFogPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ground.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/player_capsule.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ruby.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ammoBlock.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/bullet.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/floor_tile.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/wall_tile.obj"))
		return false;

	return true;
}

bool Game::LoadTextures()
{
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ground.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_red.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redDarker.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redLighter.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redOrange.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redPink.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_hurtOverlay.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/health.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/bullet.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_blue.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_disabled.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_green.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_orange.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_purple.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_red.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_white.png"))
		return false;

	return true;
}

void Game::LoadFonts()
{
	// There's a few different size fonts in there, you know
	m_arialFont12 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");
	m_arialFont18 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-18pt.spritefont");
}

void Game::InitUI()
{
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());

	m_hurtOverlay = m_textureManager->GetTexture("Assets/Textures/sprite_hurtOverlay.png");
	m_healthBar = m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png");
}

void Game::RefreshUI()
{
	// Ensure text in UI matches latest scores etc (call this after data changes)
	// Concatenate data into our label string using a wide string stream
	if (m_player)
	{
		std::wstringstream ssGems;
		std::wstringstream ssScore;
		std::wstringstream ssAmmo;

		ssGems << "Gems Collected: " << floorf(m_player->GetGems());
		ssScore << "Score: " << floorf(m_player->GetScore());
		ssAmmo << "Ammo: " << floorf(m_player->GetAmmo());

		m_gemsCollectedText = ssGems.str();
		m_scoreText = ssScore.str();
		m_ammoText = ssAmmo.str();
	}
}

void Game::InitGameWorld()
{
	//creates ground mesh
	m_ground = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
											m_diffuseTexturedShader,
											m_textureManager->GetTexture("Assets/Textures/ground.png"),
											Vector3(0, -0.5, 0));

	//creates game board
	m_gameBoard = new GameBoard(m_meshManager, m_textureManager, m_diffuseTexturedShader, m_player, m_bulletFactory);

}

void Game::Update(float timestep)
{
	// Look how short this function can be when we make objects responsible for their own logic.
	// Our only job out here is to Update the board and player, and check if the game is over.

	m_input->BeginUpdate();

	m_gameBoard->Update(timestep);
	
	m_player->Update(timestep, m_currentCam, m_bulletFactory);

	m_bulletFactory->Update(timestep);
	
	m_collisionManager->CheckCollisions();

	RefreshUI();

	//ensures the Game Over text doesn't update once it is already game over. This stops you
	//from being able to raise your score by collecting gems after all the enemies are dead.
	if (!GameOver)
		CheckGameOver();

	m_currentCam->Update(timestep);

	m_input->CenterCursorInWindow();

	//lets the user quit the game
	if (m_input->GetKeyDown(VK_ESCAPE))
		PostQuitMessage(1);

	m_input->EndUpdate();
}

void Game::Render()
{
	m_renderer->BeginScene(0.8f, 1.0f, 0.9f, 1.0f);

	m_ground->Render(m_renderer, m_currentCam);

	m_gameBoard->Render(m_renderer, m_currentCam);

	m_player->Render(m_renderer, m_currentCam);
	
	m_bulletFactory->Render(m_renderer, m_currentCam);

	DrawUI();

	m_renderer->EndScene();		
}

void Game::CheckGameOver()
{
	// Checks the three conditions that can end the game and creates the string.
	std::wstringstream ssGameOver;

	if (m_player->GetHealth() <= 0.0f)
	{
		ssGameOver << "You are dead.";
		GameOver = true;
	}

	if (m_gameBoard->GetEnemiesDefeated())
	{
		ssGameOver << "You have defeated every enemy!";
		GameOver = true;
	}

	if (GameOver)
	{
		ssGameOver << "\nYou scored " << floorf(m_player->GetScore()) << " and defeated " << floorf(m_player->GetNumberOfMonstersDefeated()) << " monsters.\n";
		ssGameOver << "You collected " << floorf(m_player->GetGems()) << " gems.";
		ssGameOver << "\nPress 'Escape' to quit the game.";

		m_gameOverText = ssGameOver.str();
	}
}

void Game::DrawUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	// Do UI drawing between the Begin and End calls

	// Draws text relating to important player variables
	m_arialFont18->DrawString(m_spriteBatch, m_gemsCollectedText.c_str(), Vector2(780, 680), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, m_scoreText.c_str(), Vector2(570, 680), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, m_ammoText.c_str(), Vector2(340, 680), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));

	// draws the health bar, the X scale is directly links with player health, so it will increase or decrease it length
	m_spriteBatch->Draw(m_healthBar->GetShaderResourceView(), Vector2(300, 640), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector2(0, 0), 
						Vector2(m_player->GetHealth()/10, 1));

	//overlay will appear when player is at a third of their health
	if (m_player->GetHealth() < 34) {
		m_spriteBatch->Draw(m_hurtOverlay->GetShaderResourceView(), Vector2(0, 0), Color(0.5f, 0.5f, 0.5f));
	}

	//draws gameover text in either red or green depending on player health
	if (GameOver && m_player->GetHealth() > 0)
		m_arialFont18->DrawString(m_spriteBatch, m_gameOverText.c_str(), Vector2(400, 300), Color(0.0f, 1.0f, 0.0f), 0, Vector2(1, 1));
	else
		m_arialFont18->DrawString(m_spriteBatch, m_gameOverText.c_str(), Vector2(400, 300), Color(1.0f, 0.0f, 0.2f), 0, Vector2(1, 1));

	m_spriteBatch->End();
}

void Game::Shutdown()
{
	if (m_gameBoard)
	{
		delete m_gameBoard;
		m_gameBoard = NULL;
	}

	if (m_bulletFactory)
	{
		delete m_bulletFactory;
		m_bulletFactory = NULL;
	}

	if (m_player)
	{
		delete m_player;
		m_player = NULL;
	}

	if (m_ground)
	{
		delete m_ground;
		m_ground = NULL;
	}

	if (m_collisionManager)
	{
		delete m_collisionManager;
		m_collisionManager = NULL;
	}

	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_diffuseTexturedShader)
	{
		m_diffuseTexturedShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_diffuseTexturedFogShader)
	{
		m_diffuseTexturedFogShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_meshManager)
	{
		m_meshManager->Release();
		delete m_meshManager;
		m_meshManager = NULL;
	}

	if (m_textureManager)
	{
		m_textureManager->Release();
		delete m_textureManager;
		m_textureManager = NULL;
	}

	if (m_spriteBatch)
	{
		delete m_spriteBatch;
		m_spriteBatch = NULL;
	}

	if (m_arialFont12)
	{
		delete m_arialFont12;
		m_arialFont12 = NULL;
	}

	if (m_arialFont18)
	{
		delete m_arialFont18;
		m_arialFont18 = NULL;
	}
}