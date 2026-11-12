#include "stdafx.h"
#include "Magic.h"
#include "Enemy.h"
#include"ScoreManager.h"
#include"Game.h"

bool Magic::Start()
{
	m_modelRender.Init("Assets/modelData/magic1.tkm");
	m_modelRender.SetShadowCasterFlag(false);

	return true;
}

void Magic::Update()
{
	Game* game = FindGO<Game>("game");
	if (game != nullptr && game->IsTimeStopped()) return;

	float dt = g_gameTime->GetFrameDeltaTime();
	m_position += m_direction * m_speed * dt;
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//敵との当たり判定
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy)
		{
			if (enemy->IsDead()) return true;
			Vector3 diff = enemy->GetPosition() - m_position;
			diff.y += 50.0f;

			if (diff.Length() < 70.0f)
			{
				enemy->Damage(1);
				ScoreManager* score = FindGO<ScoreManager>("scoreManager");
				if (score != nullptr)
				{
					score->AddMagicExp(ScoreManager::MAGIC_EXP_HIT);
				}

				m_isHit = true;

				return false;
			}
			return true;
		});

	m_lifeTimer += dt;

	if (m_isHit or m_lifeTimer > 3.0f)
	{
		DeleteGO(this);
	}


}


void Magic::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
