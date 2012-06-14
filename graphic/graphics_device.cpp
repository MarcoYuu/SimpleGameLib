#include <cassert>
#include "graphics_device.h"
#include "../other/utility.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{

Size GetSize( WINDOW_SIZE size )
{
	switch(size)
	{
	case WS_320x240:	return Size(320, 240);
	case WS_640x480:	return Size(640, 480);
	case WS_800x600:	return Size(800, 600);
	case WS_1024x768:	return Size(1024, 768);
	case WS_1280x960:	return Size(1280, 960);
	case WS_1920x1080:	return Size(1920, 1080);
	default:			return Size(640, 480);
	}
}

//--------------------------------------------------------------------------------------------------
//DirectX9�f�o�C�X�Ǘ��N���X
//--------------------------------------------------------------------------------------------------
GraphicDeviceManager::GraphicDeviceManager()
	: m_Direct3D9(0),
	  m_Direct3D9Device(0)
{}

GraphicDeviceManager::~GraphicDeviceManager()
{}

GraphicDevice GraphicDeviceManager::create(HWND hWnd, WINDOW_SIZE size, bool bWindowed, bool bVSync)
{
	Size back_buf_size =GetSize(size);

	GraphicDevice device = new GraphicDeviceManager();
	try
	{
		device->Init(hWnd, (int)back_buf_size.x, (int)back_buf_size.y, bWindowed, bVSync);
	}
	catch(...)
	{
		return NULL;
	}
	return device;
}

void GraphicDeviceManager::Init(HWND hWnd, int WindowWidth, int WindowHeight, bool bWindowed, bool bVSync)
{
	static bool initialized = false;

	if(initialized)
		return;

	size.set((float)WindowWidth, (float)WindowHeight);

	IDirect3DDevice9 *device = 0;
	try
	{
		//Direct3D�̏�����
		if(!(m_Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION)))
			throw std::runtime_error("Direct3D9�̏������Ɏ��s");

		//�f�B�X�v���C���[�h�̎擾
		D3DDISPLAYMODE d3ddm;
		if(FAILED(m_Direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			throw std::runtime_error("�f�B�X�v���C���[�h�̎擾�Ɏ��s");

		D3DPRESENT_PARAMETERS d3dpp =
		{
			static_cast<UINT>(WindowWidth),	//�o�b�N�o�b�t�@��
			static_cast<UINT>(WindowHeight),//�o�b�N�o�b�t�@����
			d3ddm.Format,					//��ʃt�H�[�}�b�g���
			1,								//�o�b�N�o�b�t�@��
			D3DMULTISAMPLE_NONE,			//�}���`�T���v�����ǂ����邩
			0,								//�}���`�T���v���̕i��
			D3DSWAPEFFECT_DISCARD,			//�X���b�v�̍ۂ̏���
			NULL,							//��ʂ�`�悷��E�B���h�E�n���h��
			bWindowed ? TRUE : FALSE,			//�X�N���[�����[�h	TRUE:�E�B���h�E,FALSE:�t���X�N���[��
			TRUE,							//�[�x��X�e���V���o�b�t�@�̍쐬
			D3DFMT_D24S8,					//�X�e���V���o�b�t�@�t�H�[�}�b�g
			0,								//�o�b�t�@�]�����̃I�v�V�����t���O
			D3DPRESENT_RATE_DEFAULT,		//�t���X�N���[���ł̃��t���b�V�����[�g
			bVSync ?							//�X���b�v�^�C�~���O
			D3DPRESENT_INTERVAL_DEFAULT :
			D3DPRESENT_INTERVAL_IMMEDIATE
		};

		//�f�o�C�X�̍쐬
		if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
											hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
			if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
												hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
				if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
													hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
					if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
														hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
						throw std::runtime_error("Direct3D9�̏������Ɏ��s");
	}
	catch(std::runtime_error &e)
	{
		SafeRelease(device);
		throw e;
	}
	m_Direct3D9Device = device;
}

}
}