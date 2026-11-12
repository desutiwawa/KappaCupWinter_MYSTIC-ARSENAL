#pragma once

class Player:public IGameObject
{
public:
	Player(){};
	~Player();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Move();
	void Rotation();
	void Damage(int damage);

	void SetArmTransform(const Vector3& pos, const Quaternion& rot)
	{
		modelRender.SetPosition(pos);
		modelRender.SetRotation(rot);
		modelRender.Update();
	}



	void Shot();

	enum EnAnimationClip
	{
		animation_Walk,
		animation_Attack,
		animation_Num
	};

//private:
	ModelRender modelRender;
	FontRender fontRender;
	AnimationClip animationClips[animation_Num];
	CharacterController characterController;
	Vector3 position;
	Vector3 moveSpeed;
	Quaternion rotation;

	int hp = 10;
	int mp = 100;
	float invincibleTimer = 0.0f;
	bool IsDead() const { return hp <= 0; }

};

