// BasewindowsApp.cpp
// �A�v���P�[�V�����N���X����
//
// 0.2

#pragma warning(disable:4290)

#include <cassert>
#include <stdexcept>
#include "windows_app_base.h"
#include "../other/utility.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// �A�v���P�[�V����
namespace app
{
//--------------------------------------------------------------------------------
//�E�B���h�E�̊��N���X
//--------------------------------------------------------------------------------
WindowBase::WindowBase(tstring Name, int Width, int Height, DWORD style, bool consider_frame)
	: m_hWnd(NULL)
	, m_name(Name)
	, m_title(Name)
	, m_width(Width)
	, m_height(Height)
{
	if(!initWindowClass())
		throw std::runtime_error("�E�B���h�E�̓o�^�Ɏ��s���܂���");
	if(!initWindowInstance(style, consider_frame))
		throw std::runtime_error("�E�B���h�E�쐬�Ɏ��s���܂���");

	//���g�̃n���h�����v���p�e�B�ɒǉ�
	::SetProp(m_hWnd, L"THIS_PTR", static_cast<HANDLE>(this));
}
Window WindowBase::create(tstring Name, int Width, int Height, DWORD style, bool consider_frame)
{
	return Window(new WindowBase(Name, Width, Height, style, consider_frame));
}

bool WindowBase::initWindowClass()
{
	WNDCLASSEX wc;

	//�E�B���h�E�N���X�̃p�����[�^
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= static_cast<WNDPROC>(dummyProcedure);	//�_�~�[�v���V�[�W����
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= ::GetModuleHandle(0);
	wc.hIcon			= static_cast<HICON>(LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	wc.hCursor			= static_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	wc.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_name.c_str();
	wc.hIconSm			= static_cast<HICON>(LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));

	//windowclass��o�^�ł��Ȃ�������False
	if(0 == ::RegisterClassEx(&wc))
		return false;

	return true;
}
bool WindowBase::initWindowInstance(DWORD style, bool consider_frame)
{
	//�E�B���h�E���[�h��������t���[���̕����l������
	int width = m_width;
	int height = m_height;

	if(consider_frame)
	{
		width  +=::GetSystemMetrics(SM_CXDLGFRAME) * 2;
		height +=::GetSystemMetrics(SM_CYDLGFRAME) * 2 +::GetSystemMetrics(SM_CYCAPTION);
	}

	//�E�B���h�E���쐬
	m_hWnd = CreateWindow(
				 m_name.c_str(),
				 m_title.c_str(),
				 style,				//�E�B���h�E�̎��
				 CW_USEDEFAULT,		//�w���W
				 CW_USEDEFAULT,		//�x���W
				 m_width,			//��
				 m_height,			//����
				 NULL,				//�e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
				 NULL,				//���j���[�n���h���A�N���X���j���[���g���Ƃ���NULL
				 ::GetModuleHandle(0),	//�C���X�^���X�n���h��
				 NULL
			 );

	//�E�B���h�E�n���h�����擾�ł��Ȃ�������false
	if(!m_hWnd)
		return false;

	return true;
}
void WindowBase::show(bool bShow)
{
	//�E�B���h�E�\��
	::UpdateWindow(m_hWnd);

	if(bShow)
		::ShowWindow(m_hWnd, SW_SHOW);
	else
		::ShowWindow(m_hWnd, SW_HIDE);
}
void WindowBase::destroy()
{
	::DestroyWindow(m_hWnd);
}
void WindowBase::setSize(int x, int y, int Width, int Height)
{
	m_width = Width;
	m_height = Height;

	::MoveWindow(m_hWnd, x, y, Width, Height, 1);
	this->show(true);
}
void WindowBase::setTitle(tstring title)
{
	m_title = title;
	SetWindowText(m_hWnd, title.c_str());
}
LRESULT CALLBACK WindowBase::dummyProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	//�v���p�e�B���Q�Ƃ����g�̃|�C���^���擾
	WindowBase *pWindow = static_cast<WindowBase *>(::GetProp(hWnd, L"THIS_PTR"));

	//�E�B���h�E�̔j����
	if(msg == WM_DESTROY)
	{
		if(pWindow != 0)
			pWindow->windowProcedure(hWnd, msg, wp, lp);

		//�֘A�t���𖕏�����
		::RemoveProp(hWnd, L"THIS_PTR");
		::PostQuitMessage(0);
		return 0;
	}

	//�擾�ł��Ȃ�������f�t�H���g�̃v���V�[�W��
	if(pWindow == 0)
		return ::DefWindowProc(hWnd, msg, wp, lp);

	//���ۂ̓��������v���V�[�W�����Ăяo��
	return pWindow->windowProcedure(hWnd, msg, wp, lp);
}
LRESULT WindowBase::windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	//���̊֐����I�[�o�[���[�h���v���V�[�W����ύX�\
	return ::DefWindowProc(hWnd, msg, wp, lp);
}

int WindowBase::getWidth() const
{
	return m_width;
}

int WindowBase::getHeight() const
{
	return m_height;
}

HWND WindowBase::getHandle() const
{
	return m_hWnd;
}

bool WindowBase::isActive()
{
	return m_hWnd == ::GetActiveWindow();
}

void WindowBase::showCursor(bool show)
{
	::ShowCursor(show ? TRUE : FALSE);
}

//--------------------------------------------------------------------------------
//�A�v���P�[�V�����̊�b�N���X
//--------------------------------------------------------------------------------
void IApplication::run()
{
	//�A�v���P�[�V�����̏�����
	initialize();

	//���C�����[�v�J�n
	startMainLoop();
}
//--------------------------------------------------------------------------------
//�ʏ�E�B���h�E�Y�A�v���C���^�t�F�[�X
//--------------------------------------------------------------------------------
WindowsAppBase::WindowsAppBase()
	: base(), m_Window(NULL)
{}

void WindowsAppBase::initialize()
{
	setWindow(WindowBase::create(_T("test")));
	getWindow()->show();
}

void WindowsAppBase::startMainLoop()
{
	assert(getWindow() != NULL);

	MSG msg;
	BOOL bRet;

	while((bRet = ::GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if(bRet == -1)
		{
			::MessageBox(NULL, L"Error on GetMassage()", L"Error", MB_OK);
			break;
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void WindowsAppBase::setWindow(Window window)
{
	m_Window = window;
}

Window WindowsAppBase::getWindow()
{
	return m_Window;
}

const Window WindowsAppBase::getWindow() const
{
	return m_Window;
}

//--------------------------------------------------------------------------------
//�ʏ�Q�[���A�v��
//--------------------------------------------------------------------------------
WindowsGameBase::WindowsGameBase()
	: base()
{}
void WindowsGameBase::initialize()
{
	setWindow(WindowBase::create(_T("test")));
	getWindow()->show();
}
void WindowsGameBase::startMainLoop()
{
	assert(getWindow() != NULL);

	//���b�Z�[�W��҂��Ȃ����[�v
	MSG msg;
	do
	{
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		mainProcess();
	}
	while(msg.message != WM_QUIT);
}
void WindowsGameBase::mainProcess()
{
	::Sleep(10);
}
}
}