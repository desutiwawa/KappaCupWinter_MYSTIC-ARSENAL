#include "stdafx.h"
#include "ScoreManager.h"


bool ScoreManager::Start()
{
	m_fontRender.SetPosition(560.0f, 520.0f, 0.0f);
	m_fontRender.SetScale(1.0f);
	
	return true;
}

void ScoreManager::StartRound()
{
	m_currentScore = 0;
	m_magicExp = 0;
	m_isPlaying = true;
}

void ScoreManager::EndRound(int bonus)
{
	if (!m_isPlaying)return;
	m_currentScore += bonus;
	m_totalScore += m_currentScore;
	m_isPlaying = false;

}
void ScoreManager::Update()
{
	wchar_t text[64];
	swprintf_s(text, L"SCORE %d", m_currentScore);
	m_fontRender.SetText(text);
}


void ScoreManager::Render(RenderContext& rc)
{
	if (m_isPlaying)
	{
		m_fontRender.Draw(rc);

	}
}


