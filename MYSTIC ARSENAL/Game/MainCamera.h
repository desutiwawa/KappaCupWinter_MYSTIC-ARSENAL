#pragma once

class Player;

class MainCamera :public IGameObject
{
public:
	MainCamera() {};
	~MainCamera() {};
	bool Start();
	void Update();

	const Quaternion& GetRotation() const { return m_rotation; }

private:
	Player* m_player;
	//Vector3 m_toCameraPos;
	float m_yaw = 0.0f;
	float m_pitch = 0.0f;
	Quaternion m_rotation;
	float m_cameraHeight = 150.0f;
	float m_sensi = 2.0f;

};

