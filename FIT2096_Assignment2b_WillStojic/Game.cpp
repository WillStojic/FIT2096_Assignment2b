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
	//m_collisionManager = new CollisionManager(&m_player, &placeHolder);

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	LoadFonts();
	InitUI();
	InitGameWorld();
	RefreshUI();

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
	//creats ground mesh
	m_ground = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
											m_diffuseTexturedShader,
											m_textureManager->GetTexture("Assets/Textures/ground.png"),
											Vector3::Zero);

	// A player will select a random starting position.
	// We need to tell the player about the board it is standing on so it can validate movement
	// and ask the board what type of tile it is standing on.
	m_player = new Player(m_input, m_currentCam);
}

void Game::Update(float timestep)
{
	// Look how short this function can be when we make objects responsible for their own logic.
	// Our only job out here is to Update the board and player, and check if the game is over.

	m_input->BeginUpdate();
	
	m_player->Update(timestep);

	//m_collisionManager->CheckCollisions();

	RefreshUI();

	CheckGameOver();

	m_currentCam->Update(timestep);

	m_input->CenterCursorInWindow();

	if (m_input->GetKeyDown(VK_ESCAPE))
		PostQuitMessage(1);

	m_input->EndUpdate();
}

void Game::Render()
{
	m_renderer->BeginScene(0.8f, 1.0f, 0.9f, 1.0f);

	m_ground->Render(m_renderer, m_currentCam);

	m_player->Render(m_renderer, m_currentCam);

	DrawUI();

	m_renderer->EndScene();		
}

void Game::CheckGameOver()
{
	// Checks the three conditions that can end the game and informs the user

	const char* msg = "";

	if (m_player->GetHealth() <= 0.0f)
	{
		msg = "You've run out of health.";
	}

	if (msg != "")
	{
		std::stringstream ss;
		ss << msg << " You scored " << m_player->GetScore() << " and defeated " << m_player->GetNumberOfMonstersDefeated() << " monsters.\n";
		ss << "You collected " << m_player->GetGems() << " gems.";

		// Message Boxes are a blocking call which makes life a little easier here
		MessageBox(NULL, ss.str().c_str(), "Game Over", MB_OK);
		PostQuitMessage(0);

		// From now on, an in-game UI should be used instead of a message box
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

	// Here's how we draw a sprite over our game
	m_spriteBatch->Draw(m_healthBar->GetShaderResourceView(), Vector2(300, 640), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector2(0, 0), 
						Vector2(m_player->GetHealth()/5, 1));

	if (m_player->GetHealth() < 34) {
		m_spriteBatch->Draw(m_hurtOverlay->GetShaderResourceView(), Vector2(0, 0), Color(0.5f, 0.5f, 0.5f));
	}

	m_spriteBatch->End();
}

void Game::Shutdown()
{
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