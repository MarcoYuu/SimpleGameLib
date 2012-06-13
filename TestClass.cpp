#include "TestClass.h"
#include <cstdlib>

//-----------------------------------------------------------------------------------------------
// ゲームクラステスト
//-----------------------------------------------------------------------------------------------
TestClass::TestClass()
	:Game(_T("test app"), WS_1280x960, false, true)
	,manager()
{

}

void TestClass::initializeResources()
{
	//入力デバイスの初期化
	getController()->setConfigMouse(CB_BUTTON_0, (MOUSE_BUTTON)BUTTON_NONE);

	//コンポーネントの追加
	manager =SceneManagerComponent::create();
	addComponent(manager);
	manager->addScene(Scene(new TestScene(*manager)));

	counter.setAverageNum(30);
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
}

void TestClass::update(float time)
{
	if(manager->getSceneNum()==0)
		manager->addScene(TestScene::create(*manager));

	//終了判定
	if(getController()->getButtonState(CB_BUTTON_0))
	{
		getWindow()->destroy();
	}
}

//-----------------------------------------------------------------------------------------------
// ゲームシーンテスト
//-----------------------------------------------------------------------------------------------
TestScene::TestScene( SceneManagerComponent &manager ) 
	: base(manager)
	, rot(0)
	, color((Color::byte)rand()%256,(Color::byte)rand()%256,(Color::byte)rand()%256,(Color::byte)128)
{
	batch  = SpriteBatchSystem::create(getGraphicDevice(), 30);
	tex[0] = TextureManager::create(getGraphicDevice(), _T("white.bmp"));
	tex[1] = TextureManager::create(getGraphicDevice(), _T("test.png"));
	
	this->setTransitionOffTime(0.8);
	this->setTransitionOnTime(0.8);
}

yuu::game::Scene TestScene::create( SceneManagerComponent &manager )
{
	return Scene(new TestScene(manager));
}

void TestScene::update( float time , bool other_has_focus, bool covered_by_other )
{
	// 隠れても描画するようにするには base::update(time, other_has_focus, false);
	base::update(time, other_has_focus, covered_by_other);
}

void TestScene::updateOnActive( float time )
{
	//入力＆描画テスト用変数更新
	rot += 1.0f;

	Controller controller =getController();

	const float speed =10.0f;
	if(controller->getButtonState(CB_BUTTON_UP))
		p.y -= speed;
	else if(controller->getButtonState(CB_BUTTON_DOWN))
		p.y += speed;
	if(controller->getButtonState(CB_BUTTON_LEFT))
		p.x -= speed;
	else if(controller->getButtonState(CB_BUTTON_RIGHT))
		p.x += speed;

	//シーン生成
	if(controller->getButtonState(CB_BUTTON_1, JUST_DOWN))
	{
		getSceneManager().addScene(Scene(new TestScene(getSceneManager())));
	}

	//シーン終了判定
	if(getController()->getButtonState(CB_BUTTON_2, JUST_DOWN))
	{
		if(getSceneManager().getSceneNum() != 1)
			exit();
	}
}

void TestScene::draw( float time )
{
	GraphicDevice device =getGraphicDevice();

	//フェードアウト中は書かない 
	if(!isFade())
	{
		//スプライト描画開始
		batch->setTexture(tex[1]);
		batch->blendMode(SpriteBatchSystem::BLEND_ADDITION1);
		batch->begin();
		{
			batch->draw(p, Color::Yellow, 1.0f, rot);
			batch->draw(Point2f(200, 200), Color::Cyan, 3, p.x / 100);
			batch->draw(Point2f(300 + 50 * cos(rot / 10), 300 + 50 * sin(rot / 10)));

			for (int i=0; i<9; ++i){
				batch->draw(
					Point2f(600 + 500 * cos(rot / 30 +i*0.7f), 500 + 350 * sin(rot / 30+i*0.7f)),
					color&0x80ffffff,4,0);
			}
		}
		batch->end();
	}

	// フェードアウト
	fade();

	batch->blendMode(SpriteBatchSystem::BLEND_NOUSE_ALPHA);
}

bool TestScene::isFade() const{
	return !((getState() == ACTIVE) 
		|| (getState()==TRANSITION_ON && getTransitionState()<=0.5) 
		|| (getState()==TRANSITION_OFF && getTransitionState()<=0.5));
}

void TestScene::fade()
{
	if(getState() == TRANSITION_OFF)
	{
		batch->setTexture(tex[0]);
		batch->blendMode(SpriteBatchSystem::BLEND_DEFAULT);
		Size win_size =getGraphicDevice()->getBackBufferSize();
		double alpha =getTransitionState();
		batch->begin();
		batch->draw(
			Point2f(win_size.x/2, win_size.y/2), 
			Color((byte)0,(byte)0,(byte)0,(byte)(255*(-(alpha*2-1)*(alpha*2-1)+1))), 
			win_size.x, 0);
		batch->end();
	}else if(getState() == TRANSITION_ON){
		batch->setTexture(tex[0]);
		batch->blendMode(SpriteBatchSystem::BLEND_DEFAULT);
		Size win_size =getGraphicDevice()->getBackBufferSize();
		double alpha =1-getTransitionState();
		batch->begin();
		batch->draw(
			Point2f(win_size.x/2, win_size.y/2), 
			Color((byte)0,(byte)0,(byte)0,(byte)(255*(-(alpha*2-1)*(alpha*2-1)+1))), 
			win_size.x, 0);
		batch->end();
	}
}
