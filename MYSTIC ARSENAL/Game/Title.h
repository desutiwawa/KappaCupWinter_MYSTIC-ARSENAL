#pragma once
class Title :public IGameObject
{
public:
	Title() {}
	~Title() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_bg;
	SpriteRender m_glow;
	SpriteRender m_pressA;
	float m_timer = 0.0f;

};

