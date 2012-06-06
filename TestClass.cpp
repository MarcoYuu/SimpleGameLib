#include "TestClass.h"

void TestClass::initialize()
{
	//�E�B���h�E�̍쐬
	Window window = WindowBase::create(_T("test class"), 800, 600, SIMPLE2, true);
	setWindow(window);
	window->setTitle(_T("Test Class Title"));

	//�O���t�B�b�N�f�o�C�X�̍쐬
	device = GraphicDeviceManager::create(window->getHandle(), WS_800x600, true, true);
	batch  = SpriteBatchSystem::create(device, 3);
	tex    = TextureManager::create(device, _T("test.png"));

	//���̓f�o�C�X�̏�����
	controller = CreateDirectInputController(window->getHandle());
	controller->setConfigMouse(CB_BUTTON_0, (MOUSE_BUTTON)BUTTON_NONE);

	//�E�B���h�E�̕\��
	window->show();
}

void TestClass::mainProcess()
{
	//FPS�\��
	OutputDebugString(
		(boost::lexical_cast<tstring>(counter.getFPS())
		+ tstring(_T("\n"))).c_str());

	//�o�b�t�@�N���A
	device->device()->Clear(
		0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(80, 5, 46), 1.0f, 0);

	//���́��`��e�X�g�p�ϐ��X�V
	static Point2f p;
	static float rot =0.0f;
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

	//�I������
	if(controller->getButtonState(CB_BUTTON_0))
	{
		getWindow()->destroy();
	}

	//�`��J�n
	device->device()->BeginScene();
	{
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
	//�`��I��
	device->device()->EndScene();
	device->device()->Present(NULL, NULL, NULL, NULL);
}
