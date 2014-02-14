
#include "../app/application_base.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
	// �A�v���P�[�V����
	namespace app
	{

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