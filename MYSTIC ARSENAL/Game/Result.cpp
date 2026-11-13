#include "stdafx.h"
#include "Result.h"
#include "Game.h"
#include "Title.h"
#include "ScoreManager.h"


bool Result::Start()
{
	ScoreManager* score = FindGO<ScoreManager>("scoreManager");
	wchar_t text[64];

	if (m_isGameOver)
	{

	m_black.Init("Assets/sprite/black.dds", 1920.0f, 1080.0f );
	m_black.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_black.Update();

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
	

	}
	else
	{
	//ゲームクリア
		m_clearBg.Init("Assets/sprite/gameClear_bg.dds", 1920.0f, 1080.0f);

		m_pressA.Init("Assets/sprite/press_a.dds", 1000.0f, 120.0f);
		m_pressA.SetPosition(Vector3(0.0f, -430.0f, 0.0f));
		m_pressA.Update();

		int bonus = score->GetBonusScore();
		int total = score->GetCurrntScore();   
		int base = total - bonus;

		
		swprintf_s(text, L"%d", base);
		m_scoreFont.SetText(text);
		m_scoreFont.SetPosition(20.0f, 215.0f, 0.0f);
		m_scoreFont.SetScale(1.6f);

		swprintf_s(text, L"+%d", bonus);
		m_bonusFont.SetText(text);
		m_bonusFont.SetPosition(20.0f, 135.0f, 0.0f);
		m_bonusFont.SetScale(1.6f);
		m_bonusFont.SetColor(0.6f, 0.95f, 1.0f, 1.0f);

		swprintf_s(text, L"%d", total);
		m_totalFont.SetText(text);
		m_totalFont.SetPosition(20.0f, 45.0f, 0.0f);
		m_totalFont.SetScale(2.0f);
		m_totalFont.SetColor(1.0f, 0.85f, 0.4f, 1.0f);
	}
	
	
	return true;
}


void Result::Update()
{
	
	m_timer += g_gameTime->GetFrameDeltaTime();
	float alpha = m_timer / m_fadeTime;
	if (alpha > 1.0f) alpha = 1.0f;

	if (m_isGameOver)
	{
		m_black.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
		m_black.Update();
	}
	else
	{
		m_clearBg.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
		m_clearBg.Update();
		float flash = 0.35f + 0.65f * (0.5f + 0.5f * sinf(m_timer * 6.28f));
		m_pressA.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, flash));
		m_pressA.Update();
	}

	if (m_timer < m_fadeTime) return;  

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		DeleteGO(FindGO<Game>("game"));
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}


void Result::Render(RenderContext& rc)
{
	if (m_isGameOver)
	{
		m_black.Draw(rc);
		if (m_timer < m_fadeTime) return;
		m_titleFont.Draw(rc);
		m_scoreFont.Draw(rc);
		m_totalFont.Draw(rc);
		m_pressFont.Draw(rc);

	}
	else
	{
		m_clearBg.Draw(rc);
		if (m_timer < m_fadeTime) return;
		m_scoreFont.Draw(rc);
		m_bonusFont.Draw(rc);
		m_totalFont.Draw(rc);
		m_pressA.Draw(rc);
	}


}


