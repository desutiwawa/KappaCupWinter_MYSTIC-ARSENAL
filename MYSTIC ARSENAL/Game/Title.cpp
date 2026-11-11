#include "stdafx.h"
#include "Title.h"
#include "Game.h"


bool Title::Start()
{
	m_bg.Init("Assets/sprite/title_bg.dds", 1920.0f, 1080.0f);

	//下の絵に光を足す処理
	m_glow.Init("Assets/sprite/title_glow.dds", 1920.0f, 1080.0f, AlphaBlendMode_Add);
	m_pressA.Init("Assets/sprite/title_pressA.dds", 912.0f, 144.0f);
	m_pressA.SetPosition(Vector3(0.0f, -132.0f, 0.0f));

	m_bg.Update();
	m_glow.Update();
	m_pressA.Update();
	return true;

}


void Title::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime();

	float pulse = 0.15f + 0.10f * (0.5f + 0.5f * sinf(m_timer * 3.14f));
	m_glow.SetMulColor(Vector4(pulse, pulse, pulse, 1.0f));

	float flash = 0.35f + 0.65f * (0.5f + 0.5f * sinf(m_timer * 6.28f));
	m_pressA.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, flash));

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}

}

void Title::Render(RenderContext& rc)
{
	m_bg.Draw(rc);
	m_glow.Draw(rc);
	m_pressA.Draw(rc);
}
