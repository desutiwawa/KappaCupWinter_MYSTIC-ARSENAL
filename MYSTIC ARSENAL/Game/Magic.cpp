#include "stdafx.h"
#include "Magic.h"
#include "Enemy.h"


bool Magic::Start()
{
	m_modelRender.Init("Assets/modelData/magic1.tkm");
	m_modelRender.SetShadowCasterFlag(false);

	return true;
}

void Magic::Update()
{
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

			if (diff.Length() < 60.0f)
			{
				enemy->Damage(1);
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
