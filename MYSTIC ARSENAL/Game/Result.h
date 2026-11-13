#pragma once
class Result:public IGameObject
{
public:
	bool Start() override;
	void Update()override;
	void Render(RenderContext& rc) override;

	void SetGameOver(bool isGameOver) { m_isGameOver = isGameOver; }






private:

	float m_timer = 0.0f;
	float m_fadeTime = 1.0f;
	bool m_isGameOver = false;

	//ゲームオーバー用
	SpriteRender m_black;
	FontRender m_titleFont;
	FontRender m_scoreFont;
	FontRender m_totalFont;
	FontRender m_pressFont;


	//ゲームクリア用
	SpriteRender m_clearBg;
	SpriteRender m_pressA;
	FontRender m_bonusFont;





	
};

