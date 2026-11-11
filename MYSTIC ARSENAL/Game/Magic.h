#pragma once
class Magic : public IGameObject
{
public:
	Magic() {};
	~Magic() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetDirection(const Vector3& dir)
	{
		m_direction = dir;
		m_direction.Normalize();
	}


private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_direction = Vector3::AxisZ;
	float m_speed = 1500.0f;
	float m_lifeTimer = 0.0f;
	bool m_isHit = false;



};

