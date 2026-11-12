#pragma once
class ScoreManager: public IGameObject
{
public:

	static const int MAGIC_EXP_USE = 1;
	static const int MAGIC_EXP_HIT = 3;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void StartRound();
	void EndRound();

	void AddKillScore(int score) { if (m_isPlaying)m_currentScore += score; }
	void AddMagicExp(int exp) { if (m_isPlaying)m_magicExp += exp; }

	int GetTotalScore() const { return m_totalScore; }
	int GetCurrntScore() const { return m_currentScore; }
	int GetMagicExp() const { m_magicExp; }


private:
	FontRender m_fontRender;
	int m_totalScore = 0;
	int m_currentScore = 0;
	int m_magicExp = 0;
	bool m_isPlaying = false;

};

