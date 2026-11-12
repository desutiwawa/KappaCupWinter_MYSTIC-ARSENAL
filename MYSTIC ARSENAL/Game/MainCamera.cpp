#include "stdafx.h"
#include "MainCamera.h"
#include "Player.h"
#include "Game.h"


bool MainCamera::Start()
{
	//m_toCameraPos.Set(0.0f, 10.0f, 20.0f);
	m_player = FindGO<Player>("player");

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}

void MainCamera::Update()
{
	Game* game = FindGO<Game>("game");
	if (game != nullptr && game->IsTimeStopped()) return;

	Vector3 target = m_player->position;
	target.y += 80.0f;

	//Vector3 toCameraPosOld = m_toCameraPos;
	
	m_yaw += g_pad[0]->GetRStickXF() * m_sensi;
	m_pitch -= g_pad[0]->GetRStickYF() * m_sensi;

	if (m_pitch > 80.0f)
	{
		m_pitch = 80.0f;
	}
	if (m_pitch < -80.0f)
	{
		m_pitch = -80.0f;
	}

	Quaternion qYaw, qPitch;
	qYaw.SetRotationDegY(m_yaw);
	qPitch.SetRotationDegX(m_pitch);
	m_rotation.Multiply(qPitch, qYaw);

	Vector3 forward = Vector3::AxisZ;
	m_rotation.Apply(forward);

	Vector3 cameraPos = m_player->position;
	cameraPos.y += m_cameraHeight;

	g_camera3D->SetPosition(cameraPos);
	g_camera3D->SetTarget(cameraPos + forward * 100.0f);
	g_camera3D->Update();


	m_player->SetArmTransform(cameraPos, m_rotation);
	


	/*;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * );
	qRot.Apply(m_toCameraPos);

	Vector3 axisX;
	axisX.Cross(Vector3::AxisY,m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.2f)
	{
		m_toCameraPos = toCameraPosOld;
	}

	Vector3 pos = target + m_toCameraPos;

	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	g_camera3D->Update();*/


}
