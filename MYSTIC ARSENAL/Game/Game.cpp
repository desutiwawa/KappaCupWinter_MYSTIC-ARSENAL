#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "MainCamera.h"
#include "BackGround.h"
#include "Enemy.h"
#include "TimeLimit.h"
#include "ScoreManager.h"
#include "Magic.h"
#include "Result.h"


Game::~Game()
{
	DeleteGO(player);
	DeleteGO(mainCamera);
	DeleteGO(backGround);
	DeleteGO(timelimit);
	QueryGOs<Enemy>("enemy", [](Enemy* e) {DeleteGO(e);return true;});
	QueryGOs<Magic>("magic", [](Magic* m) {DeleteGO(m);return true;});


}


bool Game::Start()
{
	g_soundEngine->ResistWaveFileBank(sound_Fire, "Assets/sound/fire_shot.wav");


	backGround = NewGO<BackGround>(0, "backGround");
	player = NewGO<Player>(0,"player");
	mainCamera = NewGO<MainCamera>(0, "mainCamera");
	for (int i = 0; i < 3; ++i)
	{
		Enemy* enemy = NewGO<Enemy>(0, "enemy");
		enemy->SetPosition(Vector3(100.0f + 400.0f * i, 0.0f, 0.0f));
	}
	
	timelimit = NewGO<TimeLimit>(0, "timeLimit");
	FindGO<ScoreManager>("scoreManager")->StartRound();


	
	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
	if (!m_isFinished && timelimit->IsTimeUp())
	{
		m_isFinished = true;
		FindGO<ScoreManager>("scoreManager")->EndRound();


	}

	if (m_isFinished) return;

	ScoreManager* score = FindGO<ScoreManager>("scoreManager");

	if (player->IsDead())
	{
		m_isFinished = true;
		m_isFinished = true;
		score->EndRound();
		Result* result = NewGO<Result>(10, "result");
		result->SetGameOver(true);
		return;
	}
	
	if (timelimit->IsTimeUp())
	{
		m_isFinished = true;
		score->EndRound();
	}


}



void Game::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
}