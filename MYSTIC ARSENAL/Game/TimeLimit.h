#pragma once
class TimeLimit:public IGameObject
{
public:

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	bool IsTimeUp() const { return m_remainingTime <= 0.0f; }
	float GetRemainingTime() const { return m_remainingTime; }


private:
	FontRender m_fontRender;
	//残り時間
	float m_remainingTime = 0.0f;
	//制限時間
	float m_timelimit = 60.0f;

};

