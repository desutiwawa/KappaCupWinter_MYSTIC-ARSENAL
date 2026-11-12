#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
	modelRender.Init("Assets/modelData/magicArena/magicArena.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());

	//星空(SkyCubeと同じ自作のテクスチャを使用する
	m_skyTexture.InitFromDDSFile(L"Assets/modelData/magicArena/starSky.dds");
	ModelInitData skyInit;
	skyInit.m_tkmFilePath = "Assets/modelData/preset/sky.tkm";
	skyInit.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";
	skyInit.m_vsEntryPointFunc = "VSMain";
	skyInit.m_psEntryPointFunc = "PSMain";
	skyInit.m_expandShaderResoruceView[0] = &m_skyTexture;
	skyInit.m_expandConstantBuffer = &m_skyLuminance;
	skyInit.m_expandConstantBufferSize = sizeof(m_skyLuminance);
	m_skyRender.InitForwardRendering(skyInit);
	m_skyRender.SetShadowCasterFlag(false);
	m_skyRender.SetTRS(g_vec3Zero, g_quatIdentity, g_vec3One * 2500.0f);
	m_skyRender.Update();


	return true;
}

void BackGround::Render(RenderContext& rc)
{
	m_skyRender.Draw(rc);
	modelRender.Draw(rc);
}
