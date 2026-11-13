#include "stdafx.h"
#include <ctime>
#include "Game.h"
#include "Player.h"
#include "MainCamera.h"
#include "BackGround.h"
#include "Enemy.h"
#include "TimeLimit.h"
#include "ScoreManager.h"
#include "Magic.h"
#include "Result.h"


namespace
{
	// min〜max のランダムな小数を返す
	float Random(float min, float max)
	{
		return min + (max - min) * (float)rand() / (float)RAND_MAX;
	}
}

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
	srand((unsigned int)time(nullptr));
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
	

	if (m_isFinished) return;

	ScoreManager* score = FindGO<ScoreManager>("scoreManager");

	if (player->IsDead())
	{
		m_isFinished = true;
		m_isTimeStopped = true;
		score->EndRound();
		Result* result = NewGO<Result>(10, "result");
		result->SetGameOver(true);
		return;
	}
	
	if (timelimit->IsTimeUp())
	{
		m_isFinished = true;
		m_isTimeStopped = true;
		score->EndRound(score->GetBonusScore());
		Result* result = NewGO<Result>(10, "result");
		result->SetGameOver(false);

	}

	m_spawnTimer += g_gameTime->GetFrameDeltaTime();
	if (m_spawnTimer >= m_spawnInterval)
	{
		m_spawnTimer -= m_spawnInterval;
		SpawnEnemy();
	}

}

void Game::SpawnEnemy()
{
	// 今いる敵の数を数えて、多すぎたら出さない
	int count = 0;
	QueryGOs<Enemy>("enemy", [&](Enemy*) { count++; return true; });
	if (count >= m_maxEnemyCount) return;

	Vector3 playerPos = player->position;

	// 条件に合う場所が見つかるまで最大10回やり直す
	for (int i = 0; i < 10; i++)
	{
		float angle = Random(0.0f, 2.0f * 3.14159265f);
		float dist = Random(m_spawnDistanceMin, m_spawnDistanceMax);

		Vector3 pos;
		pos.x = playerPos.x + cosf(angle) * dist;
		pos.y = 0.0f;
		pos.z = playerPos.z + sinf(angle) * dist;

		if (!IsSpawnablePosition(pos)) continue;

		Enemy* enemy = NewGO<Enemy>(0, "enemy");
		enemy->SetPosition(pos);
		return;
	}
	// 10回とも見つからなければ今回は出さない(3秒後にまた試す)
}

bool Game::IsSpawnablePosition(const Vector3& pos) const
{
	// ステージ中心からの距離(床は XZ 平面)
	float r = sqrtf(pos.x * pos.x + pos.z * pos.z);

	if (r > 4000.0f) return false;                  // 外周の壁(半径約44m)より内側だけ
	if (fabsf(r - 1300.0f) < 150.0f) return false;  // 内側の低い壁がある帯
	if (fabsf(r - 3400.0f) < 150.0f) return false;  // 外側の低い壁がある帯

	// クリスタルのモノリス(中心から23mの位置に6本)の中に出さない
	for (int k = 0; k < 6; k++)
	{
		float a = (30.0f + 60.0f * k) * 3.14159265f / 180.0f;
		float dx = pos.x - 2300.0f * cosf(a);
		float dz = pos.z - 2300.0f * sinf(a);
		if (dx * dx + dz * dz < 300.0f * 300.0f) return false;
	}
	return true;
}



void Game::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
}