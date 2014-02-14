// InputAsDirectInput.cpp
// DirectInputを用いた入力クラス実装
// 簡易説明
// 			DirectInputを用いた入力クラスの実装
//
// 0.1
//

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

#include <dinput.h>

#include <stdexcept>
#include <cstring>
#include <cassert>

#include "input_direct_input.h"
#include "../other/utility.h"
#include "../other/com_release.h"

// 自分のライブラリの名前空間
namespace yuu
{
	// 入力関連
	namespace input
	{
		class DirectInputDevice;
		class DirectInputController;
		class DirectInputKeyBoard;
		class DirectInputMouse;
		class DirectInputGamePad;

		//--------------------------------------------------------------------------------------
		//DirectInputのデバイスクラス
		//--------------------------------------------------------------------------------------
		class DirectInputDevice
			: boost::noncopyable
		{
			IDirectInput8 *m_pDirectInput;
			bool initialized;

			//生成禁止
			DirectInputDevice()
				: m_pDirectInput(NULL),
				initialized(false)
			{}

		public:
			~DirectInputDevice()
			{
				SafeRelease(m_pDirectInput);
			}

			// インスタンス取得
			// インスタンスへの参照
			static DirectInputDevice &instance()
			{
				static DirectInputDevice instance;
				return instance;
			}

			// デバイス取得
			// デバイスへのポインタ
			IDirectInput8 *getIDirentInput() const
			{
				return m_pDirectInput;
			}

			// 初期化
			// ウィンドウハンドル
			// インスタンスハンドル
			// 成否
			void init()
			{
				if(initialized)
					return;

				HINSTANCE hInst =::GetModuleHandle(0);

				//インタフェースの取得
				if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDirectInput, 0)))
					throw std::runtime_error("Directinputの初期化に失敗しました");

				initialized = true;
			}

			bool isInitialized()
			{
				return initialized;
			}
		};

		//-----------------------------------------------------------------//
		// コントローラー by DirectInput
		//-----------------------------------------------------------------//
		class DirectInputController : public IController
		{
			typedef IController base;
		private:
			KeyBoard		m_keyboard;		// <キーボードデバイス
			GamePad			m_gamepad;		// <ゲームパッドデバイス
			PointingDevice	m_mouse;		// <ポインティングデバイス

			struct ButtonSet
			{
				KeyboardButton key;
				GamepadButton pad;
				MouseButton mouse;
			};
			ButtonSet m_Setting[CB_BUTTON_NUM];

			DirectInputController(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse);

		public:
			static Controller create(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse);
			~DirectInputController() {}

			void update();
			bool getButtonState(ControllerButton ctrl, ButtonState get_type = PRESENT);
			void getAnalogAxisState(double &x, double &y, double &z);
			void getAnalogRotateState(double &rx, double &ry, double &rz);
			void getRelativePos(double &x, double &y);
			void getAbsScreenPos(double &x, double &y);
			void getWheelMove(double &z);

			void setDefault();
			void setConfigKeyBoard(ControllerButton ctrl, KeyboardButton key);
			void setConfigMouse(ControllerButton ctrl, MouseButton key);
			void setConfigGamepad(ControllerButton ctrl, GamepadButton key);
		};

		DirectInputController::DirectInputController(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse)
			: m_keyboard(keyboard)
			, m_gamepad(gamepad)
			, m_mouse(mouse)
		{
			setDefault();
		}
		Controller DirectInputController::create(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse)
		{
			return Controller(new DirectInputController(keyboard, gamepad, mouse));
		}
		void DirectInputController::setDefault()
		{
			m_Setting[CB_BUTTON_0].key		= KB_BUTTON_Z;
			m_Setting[CB_BUTTON_0].mouse	= MB_BUTTON_0;
			m_Setting[CB_BUTTON_0].pad		= GB_BUTTON_0;

			m_Setting[CB_BUTTON_1].key		= KB_BUTTON_X;
			m_Setting[CB_BUTTON_1].mouse	= MB_BUTTON_1;
			m_Setting[CB_BUTTON_1].pad		= GB_BUTTON_1;

			m_Setting[CB_BUTTON_2].key		= KB_BUTTON_C;
			m_Setting[CB_BUTTON_2].mouse	= MB_BUTTON_2;
			m_Setting[CB_BUTTON_2].pad		= GB_BUTTON_2;

			m_Setting[CB_BUTTON_3].key		= KB_BUTTON_A;
			m_Setting[CB_BUTTON_3].mouse	= MB_BUTTON_3;
			m_Setting[CB_BUTTON_3].pad		= GB_BUTTON_3;

			m_Setting[CB_BUTTON_4].key		= KB_BUTTON_S;
			m_Setting[CB_BUTTON_4].mouse	= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_4].pad		= GB_BUTTON_4;

			m_Setting[CB_BUTTON_5].key		= KB_BUTTON_D;
			m_Setting[CB_BUTTON_5].mouse	= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_5].pad		= GB_BUTTON_5;

			m_Setting[CB_BUTTON_6].key		= KB_BUTTON_SHIFT;
			m_Setting[CB_BUTTON_6].mouse	= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_6].pad		= GB_BUTTON_6;

			m_Setting[CB_BUTTON_7].key		= KB_BUTTON_CTRL;
			m_Setting[CB_BUTTON_7].mouse	= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_7].pad		= GB_BUTTON_7;

			ButtonSet none;
			none.key   = (KeyboardButton)BUTTON_NONE;
			none.pad   = (GamepadButton)BUTTON_NONE;
			none.mouse = (MouseButton)BUTTON_NONE;

			for(int i = CB_BUTTON_8; i < CB_BUTTON_LEFT; ++i)
				m_Setting[i] = none;

			m_Setting[CB_BUTTON_LEFT].key		= KB_BUTTON_LEFT;
			m_Setting[CB_BUTTON_LEFT].mouse		= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_LEFT].pad		= GB_BUTTON_LEFT;

			m_Setting[CB_BUTTON_RIGHT].key		= KB_BUTTON_RIGHT;
			m_Setting[CB_BUTTON_RIGHT].mouse	= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_RIGHT].pad		= GB_BUTTON_RIGHT;

			m_Setting[CB_BUTTON_UP].key			= KB_BUTTON_UP;
			m_Setting[CB_BUTTON_UP].mouse		= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_UP].pad			= GB_BUTTON_RIGHT;

			m_Setting[CB_BUTTON_DOWN].key		= KB_BUTTON_DOWN;
			m_Setting[CB_BUTTON_DOWN].mouse		= (MouseButton)BUTTON_NONE;
			m_Setting[CB_BUTTON_DOWN].pad		= GB_BUTTON_DOWN;
		}
		void DirectInputController::update()
		{
			if(m_keyboard != NULL)
				m_keyboard->update();
			if(m_gamepad != NULL)
				m_gamepad->update();
			if(m_mouse != NULL)
				m_mouse->update();
		}
		bool DirectInputController::getButtonState(ControllerButton ctrl, ButtonState get_type)
		{
			bool key(false), pad(false), mouse(false);

			if(m_keyboard != NULL)
			{
				if(m_Setting[ctrl].key != (KeyboardButton)BUTTON_NONE)
					key = m_keyboard->getKeyState(m_Setting[ctrl].key, get_type);
			}
			if(m_gamepad != NULL)
			{
				if(m_Setting[ctrl].pad != (GamepadButton)BUTTON_NONE)
					pad = m_gamepad->getButtonState(m_Setting[ctrl].pad, get_type);
			}
			if(m_mouse != NULL)
			{
				if(m_Setting[ctrl].mouse != (MouseButton)BUTTON_NONE)
					mouse = m_mouse->getButtonState(m_Setting[ctrl].mouse, get_type);
			}

			return (key || pad || mouse);
		}
		void DirectInputController::getAnalogAxisState(double &x, double &y, double &z)
		{
			if(m_gamepad != NULL)
			{
				m_gamepad->getAnalogAxisState(x, y, z);
			}
			else
			{
				x = 0;
				y = 0;
				z = 0;
			}
		}
		void DirectInputController::getAnalogRotateState(double &rx, double &ry, double &rz)
		{
			if(m_gamepad != NULL)
			{
				m_gamepad->getAnalogRotateState(rx, ry, rz);
			}
			else
			{
				rx = 0;
				ry = 0;
				rz = 0;
			}
		}
		void DirectInputController::getRelativePos(double &x, double &y)
		{
			if(m_mouse != NULL)
			{
				m_mouse->getRelativePos(x, y);
			}
			else
			{
				x = 0;
				y = 0;
			}
		}
		void DirectInputController::getAbsScreenPos(double &x, double &y)
		{
			if(m_mouse != NULL)
			{
				m_mouse->getAbsScreenPos(x, y);
			}
			else
			{
				x = 0;
				y = 0;
			}
		}
		void DirectInputController::getWheelMove(double &z)
		{
			if(m_mouse != NULL)
			{
				m_mouse->getWheelMove(z);
			}
			else
			{
				z = 0;
			}
		}
		void DirectInputController::setConfigKeyBoard(ControllerButton ctrl, KeyboardButton key)
		{
			m_Setting[ctrl].key = key;
		}
		void DirectInputController::setConfigMouse(ControllerButton ctrl, MouseButton key)
		{
			m_Setting[ctrl].mouse = key;
		}
		void DirectInputController::setConfigGamepad(ControllerButton ctrl, GamepadButton key)
		{
			m_Setting[ctrl].pad = key;
		}

		//--------------------------------------------------------------------------------------
		// キーボード入力 by DirectInput
		//--------------------------------------------------------------------------------------
		class DirectInputKeyBoard : public IKeyBoardInput
		{
			IDirectInputDevice8 *m_pKeyBoardDev;

			BYTE m_KeyState[256];
			BYTE m_PrevKeyState[256];

		public:
			//コンストラクタ･デストラクタ
			DirectInputKeyBoard(HWND hWnd);
			~DirectInputKeyBoard(void);

			// 参照カウンタ付ポインタを作成
			// 入力を受け付けるウィンドウのハンドル
			// 作成できなかった場合NULL
			static KeyBoard create(HWND hWnd);

			void update();
			bool getKeyState(KeyboardButton key, ButtonState get_type);

			static int convert(KeyboardButton);
		};

		DirectInputKeyBoard::DirectInputKeyBoard(HWND hWnd)
			: m_pKeyBoardDev(NULL)
		{
			DirectInputDevice &m_DInput = DirectInputDevice::instance();

			//初期化されている
			assert(m_DInput.isInitialized());

			std::memset(m_KeyState, 0, 256);
			std::memset(m_PrevKeyState, 0, 256);

			//キーボードデバイスの取得
			if(FAILED(m_DInput.getIDirentInput()->CreateDevice(GUID_SysKeyboard, &m_pKeyBoardDev, 0)))
				throw std::runtime_error("キーボードデバイスが取得できませんでした");

			//デバイスの状態を設定
			m_pKeyBoardDev->SetDataFormat(&c_dfDIKeyboard);
			m_pKeyBoardDev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

			update();
		}
		DirectInputKeyBoard::~DirectInputKeyBoard(void)
		{
			if(m_pKeyBoardDev)
				m_pKeyBoardDev->Unacquire();

			SafeRelease(m_pKeyBoardDev);
		}
		KeyBoard DirectInputKeyBoard::create(HWND hWnd)
		{
			KeyBoard key;
			try
			{
				key = new DirectInputKeyBoard(hWnd);
			}
			catch(...)
			{
				key = 0;
			}
			return key;
		}
		void DirectInputKeyBoard::update()
		{
			std::memcpy(m_PrevKeyState, m_KeyState, 256);
			m_pKeyBoardDev->Acquire();
			m_pKeyBoardDev->GetDeviceState(256, m_KeyState);
		}
		bool DirectInputKeyBoard::getKeyState(KeyboardButton key, ButtonState get_type)
		{
			key =(KeyboardButton)convert(key);
			switch(get_type)
			{
			case PRESENT:
				return (m_KeyState[key] & 0x80) ?
					true : false;
			case JUST_DOWN:
				return (m_PrevKeyState[key] & 0x80) ?
					false : ((m_KeyState[key] & 0x80) ? true : false);
			case JUST_UP:
				return (m_PrevKeyState[key] & 0x80) ?
					((m_KeyState[key] & 0x80) ? false : true) : false;
			default:
				return false;
			}
		}
		int DirectInputKeyBoard::convert(KeyboardButton button){
			switch(button){
			case KB_BUTTON_A:       return DIK_A;
			case KB_BUTTON_B:       return DIK_B;
			case KB_BUTTON_C:       return DIK_C;
			case KB_BUTTON_D:       return DIK_D;
			case KB_BUTTON_E:       return DIK_E;
			case KB_BUTTON_F:       return DIK_F;
			case KB_BUTTON_G:       return DIK_G;
			case KB_BUTTON_H:       return DIK_H;
			case KB_BUTTON_I:       return DIK_I;
			case KB_BUTTON_J:       return DIK_J;
			case KB_BUTTON_K:       return DIK_K;
			case KB_BUTTON_L:       return DIK_L;
			case KB_BUTTON_M:       return DIK_M;
			case KB_BUTTON_N:       return DIK_N;
			case KB_BUTTON_O:       return DIK_O;
			case KB_BUTTON_P:       return DIK_P;
			case KB_BUTTON_Q:       return DIK_Q;
			case KB_BUTTON_R:       return DIK_R;
			case KB_BUTTON_S:       return DIK_S;
			case KB_BUTTON_T:       return DIK_T;
			case KB_BUTTON_U:       return DIK_U;
			case KB_BUTTON_V:       return DIK_V;
			case KB_BUTTON_W:       return DIK_W;
			case KB_BUTTON_X:       return DIK_X;
			case KB_BUTTON_Y:       return DIK_Y;
			case KB_BUTTON_Z:       return DIK_Z;
			case KB_BUTTON_SHIFT:   return DIK_LSHIFT;
			case KB_BUTTON_CTRL:    return DIK_LCONTROL;
			case KB_BUTTON_ESCAPE:	return DIK_ESCAPE;
			case KB_BUTTON_DELETE:	return DIK_DELETE;
			case KB_BUTTON_LEFT:    return DIK_LEFT;
			case KB_BUTTON_RIGHT:   return DIK_RIGHT;
			case KB_BUTTON_UP:      return DIK_UP;
			case KB_BUTTON_DOWN:    return DIK_DOWN;
			case KB_BUTTON_1:       return DIK_1;
			case KB_BUTTON_2:       return DIK_2;
			case KB_BUTTON_3:       return DIK_3;
			case KB_BUTTON_4:       return DIK_4;
			case KB_BUTTON_5:       return DIK_5;
			case KB_BUTTON_6:       return DIK_6;
			case KB_BUTTON_7:       return DIK_7;
			case KB_BUTTON_8:       return DIK_8;
			case KB_BUTTON_9:       return DIK_9;
			case KB_BUTTON_0:       return DIK_0;
			}
			return -1;
		}

		//--------------------------------------------------------------------------------------
		// ゲームパッド入力 by DirectInput
		//--------------------------------------------------------------------------------------
		class DirectInputGamePad : public IGamePadInput
		{
			IDirectInputDevice8 *m_pGamePad;

			DIJOYSTATE m_GamePadState;
			DIJOYSTATE m_PrevGamePadState;

			//パッド列挙関数
			static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *lpddi, LPVOID lpContext);
			//軸の設定
			static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef);

		public:
			//コンストラクタ･デストラクタ
			DirectInputGamePad(HWND hWnd);
			~DirectInputGamePad();

			// 参照カウンタ付ポインタを作成
			// 入力を受け付けるウィンドウのハンドル
			// 作成できなかった場合NULL
			static GamePad create(HWND hWnd);

			void update();
			bool getButtonState(GamepadButton button, ButtonState get_type = PRESENT);
			bool getAnalogAxisState(double &x, double &y, double &z);
			bool getAnalogRotateState(double &rx, double &ry, double &rz);
		};

		DirectInputGamePad::DirectInputGamePad(HWND hWnd)
			: m_pGamePad(NULL)
		{
			DirectInputDevice &dinput = DirectInputDevice::instance();

			//初期化されている
			assert(dinput.isInitialized());

			std::memset(&m_GamePadState, 0, sizeof(DIJOYSTATE));
			std::memset(&m_PrevGamePadState, 0, sizeof(DIJOYSTATE));

			//デバイスの取得（パッド
			dinput.getIDirentInput()->EnumDevices(
				DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, (void **)this, DIEDFL_ATTACHEDONLY);

			if(m_pGamePad)
			{
				//デバイスの状態を設定
				m_pGamePad->EnumObjects(EnumAxesCallback, (void **)this, DIDFT_AXIS);
				m_pGamePad->SetDataFormat(&c_dfDIJoystick);
				m_pGamePad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			}
			else
			{
				throw std::runtime_error("ゲームパッドの取得ができませんでした");
			}

			update();
		}
		DirectInputGamePad::~DirectInputGamePad()
		{
			if(m_pGamePad)
				m_pGamePad->Unacquire();

			SafeRelease(m_pGamePad);
		}
		GamePad DirectInputGamePad::create(HWND hWnd)
		{
			GamePad pad;
			try
			{
				pad = new DirectInputGamePad(hWnd);
			}
			catch(...)
			{
				pad = 0;
			}
			return pad;
		}
		void DirectInputGamePad::update()
		{
			m_PrevGamePadState = m_GamePadState;

			// ジョイスティックの状態を取得
			m_pGamePad->Acquire();
			m_pGamePad->Poll();
			m_pGamePad->GetDeviceState(sizeof(DIJOYSTATE), &m_GamePadState);
		}
		bool DirectInputGamePad::getButtonState(GamepadButton button, ButtonState get_type)
		{
			switch(get_type)
			{
			case PRESENT:
				switch(button)
				{
				case GB_BUTTON_LEFT:
					return (m_GamePadState.lX < -1000 / 2) ? true : false;
				case GB_BUTTON_RIGHT:
					return (m_GamePadState.lX > 1000 / 2) ? true : false;
				case GB_BUTTON_UP:
					return (m_GamePadState.lY > 1000 / 2) ? true : false;
				case GB_BUTTON_DOWN:
					return (m_GamePadState.lY < -1000 / 2) ? true : false;
				case GB_ANALOG_X:
				case GB_ANALOG_Y:
				case GB_ANALOG_Z:
					return false;
				default:
					return (m_GamePadState.rgbButtons[button] & 0x80) ? true : false;
				}
			case JUST_DOWN:
				switch(button)
				{
				case GB_BUTTON_LEFT:
					return (m_PrevGamePadState.lX < -1000 / 2) ?
						false : ((m_GamePadState.lX < -1000 / 2) ? true : false);
				case GB_BUTTON_RIGHT:
					return (m_PrevGamePadState.lX > 1000 / 2) ?
						false : ((m_GamePadState.lX > 1000 / 2) ? true : false);
				case GB_BUTTON_UP:
					return (m_PrevGamePadState.lY > 1000 / 2) ?
						false : ((m_GamePadState.lY > 1000 / 2) ? true : false);
				case GB_BUTTON_DOWN:
					return (m_PrevGamePadState.lY < -1000 / 2) ?
						false : ((m_GamePadState.lY < -1000 / 2) ? true : false);
				case GB_ANALOG_X:
				case GB_ANALOG_Y:
				case GB_ANALOG_Z:
					return false;
				default:
					return (m_PrevGamePadState.rgbButtons[button] & 0x80) ?
						false : ((m_GamePadState.rgbButtons[button] & 0x80) ? true : false);
				}
			case JUST_UP:
				switch(button)
				{
				case GB_BUTTON_LEFT:
					return (m_PrevGamePadState.lX < -1000 / 2) ?
						((m_GamePadState.lX < -1000 / 20) ? false : true) : false;
				case GB_BUTTON_RIGHT:
					return (m_PrevGamePadState.lX > 1000 / 2) ?
						((m_GamePadState.lX > 1000 / 2) ? false : true) : false;
				case GB_BUTTON_UP:
					return (m_PrevGamePadState.lY > 1000 / 2) ?
						((m_GamePadState.lY > 1000 / 2) ? false : true) : false;
				case GB_BUTTON_DOWN:
					return (m_PrevGamePadState.lY < -1000 / 2) ?
						((m_GamePadState.lY < -1000 / 2) ? false : true) : false;
				case GB_ANALOG_X:
				case GB_ANALOG_Y:
				case GB_ANALOG_Z:
					return false;
				default:
					return (m_PrevGamePadState.rgbButtons[button] & 0x80) ?
						((m_GamePadState.rgbButtons[button] & 0x80) ? false : true) : false;
				}
			default:
				return false;
			}
		}
		bool DirectInputGamePad::getAnalogAxisState(double &x, double &y, double &z)
		{
			x = m_GamePadState.lX / 1000.0;
			y = m_GamePadState.lY / 1000.0;
			z = m_GamePadState.lZ / 1000.0;
			return true;
		}
		bool DirectInputGamePad::getAnalogRotateState(double &rx, double &ry, double &rz)
		{
			rx = m_GamePadState.lRx;
			ry = m_GamePadState.lRy;
			rz = m_GamePadState.lRz;
			return true;
		}
		BOOL CALLBACK DirectInputGamePad::EnumJoysticksCallback(
			const DIDEVICEINSTANCE *lpddi, LPVOID lpContext)
		{
			DirectInputDevice &dinput = DirectInputDevice::instance();

			HRESULT hr;
			DirectInputGamePad &This = *pointer_cast<DirectInputGamePad>(lpContext);

			//列挙されたジョイスティックへのインターフェイスを取得する
			hr = dinput.getIDirentInput()->CreateDevice(
				lpddi->guidInstance, &This.m_pGamePad, NULL);

			if(FAILED(hr))
				return DIENUM_CONTINUE;

			//ジョイスティックの能力を調べる
			DIDEVCAPS DevCaps = {};
			DevCaps.dwSize = sizeof(DIDEVCAPS);

			hr = This.m_pGamePad->GetCapabilities(&DevCaps);
			if(FAILED(hr))
			{
				//ジョイスティック能力の取得に失敗
				SafeRelease(This.m_pGamePad);
				return DIENUM_CONTINUE;
			}

			//このデバイスを使用
			return DIENUM_STOP;
		}
		BOOL CALLBACK DirectInputGamePad::EnumAxesCallback(
			LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef)
		{
			HRESULT hr;
			DirectInputGamePad &This = *pointer_cast<DirectInputGamePad>(lpvRef);

			// 軸の値の範囲を設定（-1000～1000）
			DIPROPRANGE diprg;
			ZeroMemory(&diprg, sizeof(diprg));
			diprg.diph.dwSize       = sizeof(diprg);
			diprg.diph.dwHeaderSize = sizeof(diprg.diph);
			diprg.diph.dwObj        = lpddoi->dwType;
			diprg.diph.dwHow        = DIPH_BYID;
			diprg.lMin              = -1000;
			diprg.lMax              = +1000;

			hr = This.m_pGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);
			if(FAILED(hr))
				return DIENUM_STOP;

			return DIENUM_CONTINUE;
		}
		//--------------------------------------------------------------------------------------
		// マウス入力 by DirectInput
		//--------------------------------------------------------------------------------------
		class DirectInputMouse : public IPointingDeviceInput
		{
			IDirectInputDevice8 *m_pMouse;

			DIMOUSESTATE m_MouseState;
			DIMOUSESTATE m_PrevMouseState;

			HWND m_hWnd;

		public:
			DirectInputMouse(HWND hWnd);
			~DirectInputMouse();

			// 参照カウンタ付ポインタを作成
			// 入力を受け付けるウィンドウのハンドル
			// 作成できなかった場合NULL
			static PointingDevice create(HWND hWnd);

			void update();
			bool getButtonState(MouseButton button, ButtonState get_type = PRESENT);
			bool getRelativePos(double &x, double &y);
			bool getAbsScreenPos(double &x, double &y);
			bool getWheelMove(double &z);
		};

		DirectInputMouse::DirectInputMouse(HWND hWnd)
			: m_pMouse(NULL)
			, m_hWnd(hWnd)
		{
			DirectInputDevice &m_DInput = DirectInputDevice::instance();

			//初期化されている
			assert(m_DInput.isInitialized());

			std::memset(&m_MouseState, 0, sizeof(DIMOUSESTATE));
			std::memset(&m_PrevMouseState, 0, sizeof(DIMOUSESTATE));

			if(FAILED(m_DInput.getIDirentInput()->CreateDevice(GUID_SysMouse, &m_pMouse, 0)))
				throw std::runtime_error("マウスが取得できませんでした");

			//デバイスの状態を設定
			m_pMouse->SetDataFormat(&c_dfDIMouse);
			m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

			update();
		}
		DirectInputMouse::~DirectInputMouse()
		{
			if(m_pMouse)
				m_pMouse->Unacquire();
			SafeRelease(m_pMouse);
		}
		PointingDevice DirectInputMouse::create(HWND hWnd)
		{
			PointingDevice mouse;
			try
			{
				mouse = new DirectInputMouse(hWnd);
			}
			catch(...)
			{
				mouse = 0;
			}
			return mouse;
		}
		void DirectInputMouse::update()
		{
			m_PrevMouseState = m_MouseState;
			m_pMouse->Acquire();
			m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
		}
		bool DirectInputMouse::getButtonState(MouseButton button, ButtonState get_type)
		{
			switch(get_type)
			{
			case PRESENT:
				return (m_MouseState.rgbButtons[button] & 0x80) ?
					true : false;
			case JUST_DOWN:
				return (m_PrevMouseState.rgbButtons[button] & 0x80) ?
					false : ((m_MouseState.rgbButtons[button] & 0x80) ? true : false);
			case JUST_UP:
				return (m_PrevMouseState.rgbButtons[button] & 0x80) ?
					((m_MouseState.rgbButtons[button] & 0x80) ? false : true) : false;
			default:
				return false;
			}
		}
		bool DirectInputMouse::getRelativePos(double &x, double &y)
		{
			x = m_MouseState.lX;
			y = m_MouseState.lY;
			return true;
		}
		bool DirectInputMouse::getAbsScreenPos(double &x, double &y)
		{
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(m_hWnd, &point);

			x = (double)point.x;
			y = (double)point.y;

			return true;
		}
		bool DirectInputMouse::getWheelMove(double &z)
		{
			z = m_MouseState.lZ;
			return true;
		}

		//--------------------------------------------------------------------------------------
		//DirectInputの初期化
		//--------------------------------------------------------------------------------------
		Controller CreateDirectInputController(app::Window window)
		{
			try{
				DirectInputDevice::instance().init();
				Controller result = Controller(
					DirectInputController::create(
					DirectInputKeyBoard::create((HWND)window->getHandle()),
					DirectInputGamePad::create((HWND)window->getHandle()),
					DirectInputMouse::create((HWND)window->getHandle()))
					);
				return result;
			}catch(...){
				return NULL;
			}
		}
	}
}