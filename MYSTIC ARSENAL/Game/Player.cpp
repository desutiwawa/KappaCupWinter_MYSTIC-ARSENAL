#include "stdafx.h"
#include "Player.h"
#include "Magic.h"
#include "Game.h"
#include "ScoreManager.h"


Player::~Player()
{

}

bool Player::Start()
{

	modelRender.Init("Assets/modelData/PlayerArm.tkm");

	//animationClips[animation_Walk].Load("Enter Assets");
	//animationClips[animation_Walk].SetLoopFlag(true);


	//modelRender.Init("Assets/modelData/");
	characterController.Init(25.0f, 50.0f, position);

	fontRender.SetPosition(-905.0f, -240.0f, 0.0f);
	fontRender.SetScale(0.9f);

	m_hpFrame.Init("Assets/sprite/hp_frame.dds", 512.0f, 64.0f);
	m_hpFrame.SetPosition(Vector3(-660.0f, -310.0f, 0.0f));
	m_hpFrame.Update();

	// HPバー本体
	m_hpBar.Init("Assets/sprite/hp_bar.dds", 448.0f, 36.0f);
	m_hpBar.SetPivot(Vector2(0.0f, 0.5f));                    // 基準を「左端の真ん中」にする
	m_hpBar.SetPosition(Vector3(-884.0f, -310.0f, 0.0f));     // 枠の内側の左端
	m_hpBar.Update();


	//MP
	mpFontRender.SetPosition(-905.0f, -360.0f, 0.0f);
	mpFontRender.SetScale(0.9f);

	m_mpFrame.Init("Assets/sprite/mp_frame.dds", 512.0f, 64.0f);
	m_mpFrame.SetPosition(Vector3(-660.0f, -430.0f, 0.0f));
	m_mpFrame.Update();

	m_mpBar.Init("Assets/sprite/mp_bar.dds", 448.0f, 36.0f);
	m_mpBar.SetPivot(Vector2(0.0f, 0.5f));
	m_mpBar.SetPosition(Vector3(-884.0f, -430.0f, 0.0f));
	m_mpBar.Update();

	return true;
}

void Player::Update()
{

	UpdateUI();

	Game* game = FindGO<Game>("game");
	if (game != nullptr && game->IsTimeStopped()) return;

	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= g_gameTime->GetFrameDeltaTime();

	}

	modelRender.Update();

	Move();
	Shot();
	if (mp < maxMp)
	{
		mpRecoverTimer += mpRecover * g_gameTime->GetFrameDeltaTime();
		while (mpRecoverTimer >= 1.0f && mp < maxMp)
		{
			mp++;
			mpRecoverTimer -= 1.0f;
		}
	}
	else
	{
		mpRecoverTimer = 0.0f;
	}



	if (characterController.IsOnGround() == false)
	{
		moveSpeed.y -= 50.0f;

	}
	else {}

	

}


void Player::UpdateUI()
{
	wchar_t text[32];

	// HP
	swprintf_s(text, L"HP %d / %d", hp, maxHp);
	fontRender.SetText(text);
	m_hpBar.SetScale(Vector3((float)hp / (float)maxHp, 1.0f, 1.0f));
	m_hpBar.Update();

	// MP
	swprintf_s(text, L"MP %d / %d", mp, maxMp);
	mpFontRender.SetText(text);
	m_mpBar.SetScale(Vector3((float)mp / (float)maxMp, 1.0f, 1.0f));
	m_mpBar.Update();


}




void Player::Move()
{
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();
	right.Normalize();

	right *= stickL.x * 500.0f;
	forward *= stickL.y * 500.0f;

	moveSpeed += right + forward;

	//moveSpeed.x += stickL.x * 120.0f;
	//moveSpeed.z += stickL.y * 120.0f;

	position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

	modelRender.SetPosition(position);


}


void Player::Shot()
{
	if (mp < 10)return;
	if (g_pad[0]->IsTrigger(enButtonB))
		//if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		// 画面中央 = カメラの位置からカメラの前方向へ伸ばした線上。
		// その線上から発射すれば、どの距離でも球は画面中央に重なる。
		mp -= 10;
		Vector3 forward = g_camera3D->GetForward();
		forward.Normalize();
		Vector3 pos = g_camera3D->GetPosition();
		Magic* magic = NewGO<Magic>(0, "magic");
		magic->SetPosition(pos + forward * 80.0f);	// 目の前すぐだと球で画面が埋まるので少し前から出す
		magic->SetDirection(forward);

		SoundSource* se = NewGO<SoundSource>(0);
			se->Init(sound_Fire);
		se->SetVolume(0.2f);
		se->Play(false);

		ScoreManager* score = FindGO<ScoreManager>("scoreManager");
		if (score != nullptr)
		{
			score->AddMagicExp(ScoreManager::MAGIC_EXP_USE);

		}

	}
}


void Player::Rotation()
{

}

void Player::Damage(int damage)
{
	if (invincibleTimer > 0.0f)return;
	hp -= damage;
	if (hp < 0)hp = 0;
	invincibleTimer = 1.0f;
}



void Player::Render(RenderContext& rc)
{
	
	m_hpFrame.Draw(rc);
	m_hpBar.Draw(rc);
	fontRender.Draw(rc);

	m_mpFrame.Draw(rc);
	m_mpBar.Draw(rc);
	mpFontRender.Draw(rc);


	modelRender.Draw(rc);
	
}



