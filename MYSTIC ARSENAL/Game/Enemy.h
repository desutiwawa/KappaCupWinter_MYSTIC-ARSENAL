#pragma once


class Player;

class Enemy : public IGameObject
{
public:
	Enemy() {}
	~Enemy() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(const Vector3& pos) { m_position = pos; }

	const Vector3& GetPosition() const{ return m_position; }
	
	void Damage(int damage);

	bool IsDead() const{ return m_hp <= 0; }


private:
	void Chase();
	void Gravity();
	void Attack();
	void Shot();
	void UpdateState();
	void Turn();
	void CheckHit();
	Vector3 GetToPlayerXZ() const;

	enum States
	{
		state_Move,
		state_Attack,

	};

	ModelRender m_modelRender;
	CharacterController m_characterController;
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Quaternion m_rotation;

	Player* m_player = nullptr;

	int m_hp = 3;

	States m_state = state_Move;
	float m_distance = 300.0f;
	float m_walkSpeed = 200.0f;
	float m_attackRange = 80.0f;
	float m_hitRange = 100.0f;
	float m_attackInterval = 3.0f;
	float m_attackStopTime = 1.0f;
	float m_stateTimer = 0.0f;
	float m_cooldown = 1.5f;
	int m_attackDamage = 1;
	bool m_isHit = false;
	bool m_isShot = false;

};

