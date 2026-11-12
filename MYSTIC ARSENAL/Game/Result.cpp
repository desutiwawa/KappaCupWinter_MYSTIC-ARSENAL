#include "stdafx.h"
#include "Result.h"
#include "Game.h"
#include "Title.h"
#include "ScoreManager.h"


bool Result::Start()
{
	m_black.Init("Assets/spriteblack.dds", 1920.0f, 1080.0f );
	m_black.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_black.Update();

	ScoreManager* score = FindGO<ScoreManager>("scoreManager");
	wchar_t text[64];

	m_titleFont.SetText(L"GAME OVER");
	m_titleFont.SetPosition(-330.0f, 250.0f, 0.0f);
	m_titleFont.SetScale(3.0f);
	m_titleFont.SetColor(1.0f, 0.25f, 0.3f, 1.0f);

	swprintf_s(text, L"SCORE  %d", score->GetCurrntScore());
	m_scoreFont.SetText(text);
	m_scoreFont.SetPosition(-200.0f, 40.0f, 0.0f);
	m_scoreFont.SetScale(1.5f);

	swprintf_s(text, L"TOTAL  %d", score->GetTotalScore());
	m_totalFont.SetText(text);
	m_totalFont.SetPosition(-200.0f, -40.0f, 0.0f);
	m_totalFont.SetScale(1.5f);

	m_pressFont.SetText(L"PRESS A");
	m_pressFont.SetPosition(-110.0f, -300.0f, 0.0f);
	m_pressFont.SetScale(1.2f);
	return true;

}


void Result::Update()
{
	// 画面の暗転(ゲームの時間は止まっていても、ここは進む)
	m_timer += g_gameTime->GetFrameDeltaTime();
	float alpha = m_timer / m_fadeTime;
	if (alpha > 1.0f) alpha = 1.0f;
	m_black.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_black.Update();

	if (m_timer < m_fadeTime) return;   // 暗転中は入力を受け付けない

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		DeleteGO(FindGO<Game>("game"));
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}


void Result::Render(RenderContext& rc)
{
	m_black.Draw(rc);
	if (m_timer < m_fadeTime) return;
	m_titleFont.Draw(rc);
	m_scoreFont.Draw(rc);
	m_totalFont.Draw(rc);
	m_pressFont.Draw(rc);
}


