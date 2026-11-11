#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class MainCamera;
class BackGround;


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


	ModelRender m_modelRender;
	Vector3 m_pos;
};

