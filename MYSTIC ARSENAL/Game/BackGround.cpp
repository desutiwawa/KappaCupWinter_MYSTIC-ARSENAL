#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
	modelRender.Init("Assets/modelData/mysticCave/mysticCave.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
	return true;
}

void BackGround::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}
