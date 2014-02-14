
#include "../app/application_base.h"

// 自分のライブラリの名前空間
namespace yuu
{
	// アプリケーション
	namespace app
	{

		//--------------------------------------------------------------------------------
		//アプリケーションの基礎クラス
		//--------------------------------------------------------------------------------
		void IApplication::run()
		{
			//アプリケーションの初期化
			initialize();

			//メインループ開始
			startMainLoop();
		}

		//--------------------------------------------------------------------------------
		//通常ウィンドウズアプリインタフェース
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
		//通常ゲームアプリ
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

			//メッセージを待たないループ
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