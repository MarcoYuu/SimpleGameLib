#include "TestClass.h"

#include <cstdlib>
#include <math.h>

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
	, color((Color::byte)rand()%256,(Color::byte)rand()%256,(Color::byte)rand()%256,(Color::byte)128)
	, rot(0)
	, object(getGraphicDevice())
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
	Controller controller =getController();

	rot +=1.0f;

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

	object.update(getController());
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
			Size size =getGraphicDevice()->getBackBufferSize();
			for (int i=0; i<9; ++i){
				batch->draw(
					Point2f(
					size.x/2 + 500 * cos(rot / 30 +i*0.7f), 
					size.y/2 + 350 * sin(rot / 30+i*0.7f)),
					color&0x80ffffff,4,0);
			}
		}
		batch->end();

		// オブジェクト描画
		object.draw();
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
	switch(getState())
	{
	case TRANSITION_OFF:
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
	}break;

	case TRANSITION_ON:
	{
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
	}break;
	}
}

//-----------------------------------------------------------------------------------------------
// テストオブジェクト
//-----------------------------------------------------------------------------------------------
TestObject::TestObject( GraphicDevice dev ) 
	: device(dev)
	, batch()
	, tex()
	, count(0)
	, rot(0)
	, position(0,0)
	, velocity(10,10)
	, bullets(BULLET_NUM, sizeof(Bullet))
{
	batch =SpriteBatchSystem::create(dev, BULLET_NUM);
	tex =TextureManager::create(dev, _T("test.png"));
}

void TestObject::update( Controller controller )
{
	if(controller->getButtonState(CB_BUTTON_UP))
		position.y -= velocity.y;
	else if(controller->getButtonState(CB_BUTTON_DOWN))
		position.y += velocity.y;
	if(controller->getButtonState(CB_BUTTON_LEFT))
		position.x -= velocity.x;
	else if(controller->getButtonState(CB_BUTTON_RIGHT))
		position.x += velocity.x;

	// 弾発射
	const int interval =2;
	if(controller->getButtonState(CB_BUTTON_6) && count > interval)
	{
		Point2f vel;
		Bullet* b;

		for (int i=0;i<4;++i)
		{
			vel.set(10*cos(rot+i*(float)M_PI/4),10*sin(rot+i*(float)M_PI/4));
			b =bullets.construct<Bullet>(this);
			b!=NULL?b->init(position, vel, vel*0.01f):0;
			b =bullets.construct<Bullet>(this);
			b!=NULL?b->init(position, -vel, -vel*0.01f):0;

			vel.set(10*cos(-rot-i*(float)M_PI/4),10*sin(-rot-i*(float)M_PI/4));
			b =bullets.construct<Bullet>(this);
			b!=NULL?b->init(position, vel, vel*0.01f):0;
			b =bullets.construct<Bullet>(this);
			b!=NULL?b->init(position, -vel, -vel*0.01f):0;
		}
	}
	++count;
	rot += 0.1f;

	MemoryManageList<Bullet>::iterator it=bullets.begin(), end =bullets.end();
	while (it!=end)
	{
		if(!it->update()){
			it =bullets.erase(it);
			continue;
		}
		++it;
	}
}

void TestObject::draw()
{
	batch->setTexture(tex);
	batch->blendMode(SpriteBatchSystem::BLEND_ADDITION1);
	batch->begin();
	batch->draw(position);
	MemoryManageList<Bullet>::iterator it=bullets.begin(), end =bullets.end();
	while (it!=end)
	{
		it->draw();
		++it;
	}
	batch->end();
}

TestObject::Bullet::Bullet( TestObject *p ) 
	: position(0,0)
	, velocity(0,0)
	, accel(0,0)
	, parent(p)
{

}

void TestObject::Bullet::init( const Point2f &pos, const Point2f &vel, const Point2f &acl )
{
	position =pos;
	velocity =vel;
	accel    =acl;
}

bool TestObject::Bullet::update()
{
	position +=velocity;
	velocity +=accel;
	const int padding =90;
	Size win =parent->device->getBackBufferSize();
	if (win.x +padding < position.x || 
		win.y +padding <position.y ||
		-padding > position.x || 
		-padding >position.y)
	{
		return false;
	}
	return true;
}

void TestObject::Bullet::draw()
{
	parent->batch->draw(position, Color::White&0x80ffffff, 0.5, 0);
}
