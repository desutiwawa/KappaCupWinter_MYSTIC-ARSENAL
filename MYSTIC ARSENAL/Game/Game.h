#pragma once

#include "Level3DRender/LevelRender.h"

#include "TimeLimit.h"


class Player;
class MainCamera;
class BackGround;
class TimeLimit;

enum EnSound
{
	sound_Fire = 0,

};

class Game : public IGameObject
{
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	bool IsTimeStopped() const { return m_isTimeStopped; }
	

private:
	Player* player;
	MainCamera* mainCamera;
	BackGround* backGround;
	TimeLimit* timelimit = nullptr;


	ModelRender m_modelRender;
	Vector3 m_pos;
	bool m_isFinished = false;
	bool m_isTimeStopped = false;


	void SpawnEnemy();
	bool IsSpawnablePosition(const Vector3& pos)const;
	

	float m_spawnTimer = 0.0f;
	float m_spawnInterval = 3.0f;        // 何秒ごとに出すか
	float m_spawnDistanceMin = 1500.0f;  // プレイヤーからの最短距離(15m)
	float m_spawnDistanceMax = 2500.0f;  // プレイヤーからの最長距離(25m)
	int   m_maxEnemyCount = 20;          // 同時に出せる最大数(出しすぎ防止)
};

