#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemyMagic.h"
#include "ScoreManager.h"

bool Enemy::Start()
{
	m_modelRender.Init("Assets/modelData/enemy1.tkm");
	m_characterController.Init(25.0f, 75.0f, m_position);
	m_modelRender.SetScale(1.5f, 1.5f, 1.5f);
	m_modelRender.SetPosition(m_position);
	m_player = FindGO<Player>("player");
	return true;
}

void Enemy::Update()
{
	if (m_player == nullptr) return;

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	Chase();
	Turn();

	Gravity();
	CheckHit();

	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();

}

Vector3 Enemy::GetToPlayerXZ()const
{
	Vector3 toPlayer = m_player->position - m_position;
	toPlayer.y = 0.0f;
	return toPlayer;
}

void Enemy::Chase()
{
	Vector3 toPlayer = GetToPlayerXZ();
	
	//触れるくらい近ければ止まる
	if (toPlayer.Length() <= m_attackRange)
	{
		return;
	}

	toPlayer.Normalize();
	m_moveSpeed += toPlayer * m_walkSpeed;

}

void Enemy::CheckHit()
{
	//高さが離れすぎると当てないようにする
	float heightDiff = fabsf(m_player->position.y - m_position.y);
	if (heightDiff > 100.0f)
	{
		return;
	}
	if (GetToPlayerXZ().Length() <= m_attackRange)
	{
		m_player->Damage(m_attackDamage);
		
	}
}

void Enemy::Gravity()
{
	if (m_characterController.IsOnGround() == false)
	{
		m_moveSpeed.y -= 50.0f;
	}
	else {}
}

void Enemy::Turn()
{
	Vector3 dir = GetToPlayerXZ();
	if (dir.LengthSq() > 0.001f)
	{
		dir.Normalize();
		m_rotation.SetRotationYFromDirectionXZ(dir);
	}

}

void Enemy::Damage(int damage)
{
	if (IsDead())return;

	m_hp -= damage;

	if (m_hp <= 0)
	{
		ScoreManager* score = FindGO<ScoreManager>("scoreManager");
		if (score != nullptr)
		{
			score->AddKillScore(m_killScore);
		}

		DeleteGO(this);
	}
}



void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


