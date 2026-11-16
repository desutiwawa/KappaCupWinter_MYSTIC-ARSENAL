#pragma once
class Reticle:public IGameObject
{
public:

	Reticle() {}
	~Reticle() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	SpriteRender m_spriteRender;

};

