#pragma once
class BackGround : public IGameObject
{
public:
	BackGround() {};
	~BackGround() {};

	bool Start();
	void Render(RenderContext& rc);


private:
	BackGround* m_bg = nullptr;
	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;

	ModelRender m_skyRender;
	Texture m_skyTexture;
	float m_skyLuminance = 1.0f;

};

