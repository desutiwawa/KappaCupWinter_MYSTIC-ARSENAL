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

};

