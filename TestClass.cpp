#include "TestClass.h"
#include <cstdlib>

//-----------------------------------------------------------------------------------------------
// �Q�[���N���X�e�X�g
//-----------------------------------------------------------------------------------------------
TestClass::TestClass()
	:Game(_T("test app"), WS_1280x960, false, true)
	,p()
	,rot(0.0f)
	,manager()
{

}

void TestClass::initializeResources()
{
	//�O���t�B�b�N�֌W�̍쐬
	batch  = SpriteBatchSystem::create(getGraphicDevice(), 10);
	tex    = TextureManager::create(getGraphicDevice(), _T("test.png"));

	//���̓f�o�C�X�̏�����
	getController()->setConfigMouse(CB_BUTTON_0, (MOUSE_BUTTON)BUTTON_NONE);

	//�R���|�[�l���g�̒ǉ�
	manager =SceneManagerComponent::create();
	addComponent(manager);

	counter.setAverageNum(30);
}
void TestClass::draw(float time)
{
	GraphicDevice device =getGraphicDevice();

	//FPS�\��
	OutputDebugString(
		(boost::lexical_cast<tstring>(counter.getFPS())
		+ tstring(_T("\n"))).c_str());

	//�o�b�t�@�N���A
	device->device()->Clear(
	0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
	D3DCOLOR_XRGB(80, 5, 46), 1.0f, 0);

	//�X�v���C�g�`��J�n
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

void TestClass::update(float time)
{
	//���́��`��e�X�g�p�ϐ��X�V
	rot += 0.3f;

	Controller controller =getController();

	if(controller->getButtonState(CB_BUTTON_UP))
		p.y -= 5.0f;
	else if(controller->getButtonState(CB_BUTTON_DOWN))
		p.y += 5.0f;
	if(controller->getButtonState(CB_BUTTON_LEFT))
		p.x -= 5.0f;
	else if(controller->getButtonState(CB_BUTTON_RIGHT))
		p.x += 5.0f;

	//�I������
	if(controller->getButtonState(CB_BUTTON_0))
	{
		getWindow()->destroy();
	}

	//�V�[������
	if(controller->getButtonState(CB_BUTTON_1, JUST_DOWN))
	{
		manager->addScene(Scene(new TestScene(*manager)));
	}
}

//-----------------------------------------------------------------------------------------------
// �Q�[���V�[���e�X�g
//-----------------------------------------------------------------------------------------------
TestScene::TestScene( SceneManagerComponent &manager ) 
	: base(manager)
	, rot(0)
	, color((Color::byte)rand()%256,(Color::byte)rand()%256,(Color::byte)rand()%256,(Color::byte)128)
{
	batch  = SpriteBatchSystem::create(getGraphicDevice(), 10);
	tex    = TextureManager::create(getGraphicDevice(), _T("test.png"));
}

yuu::game::Scene TestScene::create( SceneManagerComponent &manager )
{
	return Scene(new TestScene(manager));
}

void TestScene::update( float time , bool other_has_focus, bool covered_by_other )
{
	// �B��Ă��`�悷��悤��
	base::update(time, other_has_focus, false);
}

void TestScene::updateOnActive( float time )
{
	rot +=1.0f;
	//�I������
	if(getController()->getButtonState(CB_BUTTON_2, JUST_DOWN))
	{
		exit();
	}
}

void TestScene::draw( float time )
{
	GraphicDevice device =getGraphicDevice();

	//�X�v���C�g�`��J�n
	batch->setTexture(tex);
	batch->blendMode(SpriteBatchSystem::BLEND_ADDITION1);
	batch->begin();
	{
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30), 500 + 350 * sin(rot / 30)),
			color&0x80ffffff
		);
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30+0.7f), 500 + 350 * sin(rot / 30+0.7f)),
			color&0x80ffffff
		);
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30+1.5f), 500 + 350 * sin(rot / 30+1.5f)),
			color&0x80ffffff
		);
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30+2.2f), 500 + 350 * sin(rot / 30+2.2f)),
			color&0x80ffffff
		);
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30+0.3f), 500 + 350 * sin(rot / 30+0.3f)),
			color&0x80ffffff
			);
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30+1.0f), 500 + 350 * sin(rot / 30+1.0f)),
			color&0x80ffffff
			);
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30+1.8f), 500 + 350 * sin(rot / 30+1.8f)),
			color&0x80ffffff
			);
		batch->draw(
			Point2f(600 + 500 * cos(rot / 30+2.5f), 500 + 350 * sin(rot / 30+2.5f)),
			color&0x80ffffff
			);
	}
	batch->end();
	batch->blendMode(SpriteBatchSystem::BLEND_NOUSE_ALPHA);
}
