#include "TestClass.h"

TestClass::TestClass()
	:Game(_T("test app"), WS_1280x960, false, true)
	,p()
	,rot(0.0f)
{

}

void TestClass::initializeResources()
{
	//グラフィック関係の作成
	batch  = SpriteBatchSystem::create(getGraphicDevice(), 3);
	tex    = TextureManager::create(getGraphicDevice(), _T("test.png"));

	//入力デバイスの初期化
	controller = CreateDirectInputController(getWindow()->getHandle());
	controller->setConfigMouse(CB_BUTTON_0, (MOUSE_BUTTON)BUTTON_NONE);
}

void TestClass::draw(float time)
{
	GraphicDevice device =getGraphicDevice();

	//FPS表示
	OutputDebugString(
		(boost::lexical_cast<tstring>(counter.getFPS())
		+ tstring(_T("\n"))).c_str());

	//バッファクリア
	device->device()->Clear(
		0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(80, 5, 46), 1.0f, 0);

	//描画開始
	device->device()->BeginScene();
	{
		//スプライト描画開始
		batch->setTexture(tex);
		batch->blendMode(SpriteBatchSystem::BLEND_DEFAULT);
		batch->begin();
		{
			batch->draw(p, Color::Yellow, 1.0f, rot);
			batch->draw(Point2f(200, 200), Color::Cyan, 3, p.x / 100);
			batch->draw(Point2f(300 + 50 * cos(rot / 10), 300 + 50 * sin(rot / 10)));
		}
		batch->end();
		batch->blendMode(SpriteBatchSystem::BLEND_NOUSE_ALPHA);
	}
	//描画終了
	device->device()->EndScene();
	device->device()->Present(NULL, NULL, NULL, NULL);
}

void TestClass::update(float time)
{
	//入力＆描画テスト用変数更新
	rot += 0.3f;
	controller->update();

	if(controller->getButtonState(CB_BUTTON_UP))
		p.y -= 5.0f;
	else if(controller->getButtonState(CB_BUTTON_DOWN))
		p.y += 5.0f;
	if(controller->getButtonState(CB_BUTTON_LEFT))
		p.x -= 5.0f;
	else if(controller->getButtonState(CB_BUTTON_RIGHT))
		p.x += 5.0f;

	//終了判定
	if(controller->getButtonState(CB_BUTTON_0))
	{
		getWindow()->destroy();
	}
}
