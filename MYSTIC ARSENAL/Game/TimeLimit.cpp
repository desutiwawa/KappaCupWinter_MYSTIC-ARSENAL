#include "stdafx.h"
#include "TimeLimit.h"
#include"Game.h"


bool TimeLimit::Start()
{
	m_remainingTime = m_timelimit;
	m_fontRender.SetPosition(-90.0f, 520.0f, 0.0f);
	m_fontRender.SetScale(1.2f);

	return true;
}

void TimeLimit::Update()
{
	Game* game = FindGO<Game>("game");
	if (game != nullptr && game->IsTimeStopped()) return;

	if (!IsTimeUp())
	{
		m_remainingTime -= g_gameTime->GetFrameDeltaTime();
		if (m_remainingTime < 0.0f)
		{
			m_remainingTime = 0.0f;

		}
	}

	wchar_t text[32];
	if (IsTimeUp())
	{
		swprintf_s(text, L"TIME UP");
	}
	else
	{
		//1.2秒なら2秒と表示
		int sec = (int)ceilf(m_remainingTime);
		swprintf_s(text, L"%d:%02d", sec / 60, sec % 60);

	}
	m_fontRender.SetText(text);




}

void TimeLimit::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);
}



