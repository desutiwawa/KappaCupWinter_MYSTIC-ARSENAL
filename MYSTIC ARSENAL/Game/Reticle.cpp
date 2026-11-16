#include "stdafx.h"
#include "Reticle.h"


bool Reticle::Start()
{
	m_spriteRender.Init("Assets/sprite/reticle_Fire.dds",200.0f,200.0f);
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.7f));

	return true;
}

void Reticle::Update()
{
	m_spriteRender.Update();
}

void Reticle::Render(RenderContext& rc)
{

	m_spriteRender.Draw(rc);
}
