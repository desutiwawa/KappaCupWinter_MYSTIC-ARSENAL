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
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	

private:
	Player* player;
	MainCamera* mainCamera;
	BackGround* backGround;
	TimeLimit* timelimit = nullptr;


	ModelRender m_modelRender;
	Vector3 m_pos;
	bool m_isFinished = false;

	
};

