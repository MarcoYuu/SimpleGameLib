// InputAsDirectInput.cpp
// DirectInputを用いた入力クラス実装
// 簡易説明
// 			DirectInputを用いた入力クラスの実装
//
// 0.1
//
#pragma warning(disable:4290)
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#include <stdexcept>
#include <cstring>
#include <cassert>

#include "input_direct_input.h"
#include "../other/utility.h"

// 自分のライブラリの名前空間
namespace yuu
{
// 入力関連
namespace input
{
//--------------------------------------------------------------------------------------
//DirectInputの基本デバイスクラス
//--------------------------------------------------------------------------------------
// DirectInput本体を保持するクラス
//
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
//デフォルトコントローラー
//-----------------------------------------------------------------//
class DefaultController : public IController
{
	typedef IController base;
private:
	KeyBoard		m_keyboard;		// <キーボードデバイス
	GamePad			m_gamepad;		// <ゲームパッドデバイス
	PointingDevice	m_mouse;		// <ポインティングデバイス

	struct ButtonSet
	{
		KEYBOARD_BUTTON key;
		GAMEPAD_BUTTON pad;
		MOUSE_BUTTON mouse;
	};
	ButtonSet m_Setting[CB_BUTTON_NUM];

	DefaultController(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse);

public:
	static Controller create(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse);
	~DefaultController() {}

	void update();
	bool getButtonState(CONTROL_BUTTON ctrl, STATE_TYPE get_type = PRESENT);
	void getAnalogAxisState(double &x, double &y, double &z);
	void getAnalogRotateState(double &rx, double &ry, double &rz);
	void getRelativePos(double &x, double &y);
	void getAbsScreenPos(double &x, double &y);
	void getWheelMove(double &z);

	void setDefault();
	void setConfigKeyBoard(CONTROL_BUTTON ctrl, KEYBOARD_BUTTON key);
	void setConfigMouse(CONTROL_BUTTON ctrl, MOUSE_BUTTON key);
	void setConfigGamepad(CONTROL_BUTTON ctrl, GAMEPAD_BUTTON key);
};

DefaultController::DefaultController(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse)
	: m_keyboard(keyboard)
	, m_gamepad(gamepad)
	, m_mouse(mouse)
{
	setDefault();
}
Controller DefaultController::create(KeyBoard keyboard, GamePad gamepad, PointingDevice mouse)
{
	return Controller(new DefaultController(keyboard, gamepad, mouse));
}
void DefaultController::setDefault()
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
	m_Setting[CB_BUTTON_4].mouse	= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_4].pad		= GB_BUTTON_4;

	m_Setting[CB_BUTTON_5].key		= KB_BUTTON_D;
	m_Setting[CB_BUTTON_5].mouse	= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_5].pad		= GB_BUTTON_5;

	m_Setting[CB_BUTTON_6].key		= KB_BUTTON_SHIFT;
	m_Setting[CB_BUTTON_6].mouse	= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_6].pad		= GB_BUTTON_6;

	m_Setting[CB_BUTTON_7].key		= KB_BUTTON_CTRL;
	m_Setting[CB_BUTTON_7].mouse	= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_7].pad		= GB_BUTTON_7;

	ButtonSet none;
	none.key   = (KEYBOARD_BUTTON)BUTTON_NONE;
	none.pad   = (GAMEPAD_BUTTON)BUTTON_NONE;
	none.mouse = (MOUSE_BUTTON)BUTTON_NONE;

	for(int i = CB_BUTTON_8; i < CB_BUTTON_LEFT; ++i)
		m_Setting[i] = none;

	m_Setting[CB_BUTTON_LEFT].key		= KB_BUTTON_LEFT;
	m_Setting[CB_BUTTON_LEFT].mouse		= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_LEFT].pad		= GB_BUTTON_LEFT;

	m_Setting[CB_BUTTON_RIGHT].key		= KB_BUTTON_RIGHT;
	m_Setting[CB_BUTTON_RIGHT].mouse	= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_RIGHT].pad		= GB_BUTTON_RIGHT;

	m_Setting[CB_BUTTON_UP].key			= KB_BUTTON_UP;
	m_Setting[CB_BUTTON_UP].mouse		= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_UP].pad			= GB_BUTTON_RIGHT;

	m_Setting[CB_BUTTON_DOWN].key		= KB_BUTTON_DOWN;
	m_Setting[CB_BUTTON_DOWN].mouse		= (MOUSE_BUTTON)BUTTON_NONE;
	m_Setting[CB_BUTTON_DOWN].pad		= GB_BUTTON_DOWN;
}
void DefaultController::update()
{
	if(m_keyboard != NULL)
		m_keyboard->update();
	if(m_gamepad != NULL)
		m_gamepad->update();
	if(m_mouse != NULL)
		m_mouse->update();
}
bool DefaultController::getButtonState(CONTROL_BUTTON ctrl, STATE_TYPE get_type)
{
	bool key(false), pad(false), mouse(false);

	if(m_keyboard != NULL)
	{
		if(m_Setting[ctrl].key != (KEYBOARD_BUTTON)BUTTON_NONE)
			key = m_keyboard->getKeyState(m_Setting[ctrl].key, get_type);
	}
	if(m_gamepad != NULL)
	{
		if(m_Setting[ctrl].pad != (GAMEPAD_BUTTON)BUTTON_NONE)
			pad = m_gamepad->getButtonState(m_Setting[ctrl].pad, get_type);
	}
	if(m_mouse != NULL)
	{
		if(m_Setting[ctrl].mouse != (MOUSE_BUTTON)BUTTON_NONE)
			mouse = m_mouse->getButtonState(m_Setting[ctrl].mouse, get_type);
	}

	return (key || pad || mouse);
}
void DefaultController::getAnalogAxisState(double &x, double &y, double &z)
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
void DefaultController::getAnalogRotateState(double &rx, double &ry, double &rz)
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
void DefaultController::getRelativePos(double &x, double &y)
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
void DefaultController::getAbsScreenPos(double &x, double &y)
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
void DefaultController::getWheelMove(double &z)
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

void DefaultController::setConfigKeyBoard(CONTROL_BUTTON ctrl, KEYBOARD_BUTTON key)
{
	m_Setting[ctrl].key = key;
}
void DefaultController::setConfigMouse(CONTROL_BUTTON ctrl, MOUSE_BUTTON key)
{
	m_Setting[ctrl].mouse = key;
}
void DefaultController::setConfigGamepad(CONTROL_BUTTON ctrl, GAMEPAD_BUTTON key)
{
	m_Setting[ctrl].pad = key;
}

//--------------------------------------------------------------------------------------
//DirectInputの初期化
//--------------------------------------------------------------------------------------
void InitInput()
{
	DirectInputDevice::instance().init();
}

Controller CreateDirectInputController(HWND hWnd)
{
	InitInput();

	return Controller(
			   DefaultController::create(
				   DirectInputKeyBoard::Create(hWnd),
				   DirectInputGamePad::Create(hWnd),
				   DirectInputMouse::Create(hWnd)
			   )
		   );
}

//--------------------------------------------------------------------------------------
//キーボード入力 by DirectInput
//--------------------------------------------------------------------------------------
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

KeyBoard DirectInputKeyBoard::Create(HWND hWnd)
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

bool DirectInputKeyBoard::getKeyState(KEYBOARD_BUTTON key, STATE_TYPE get_type)
{
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

//--------------------------------------------------------------------------------------
//ゲームパッド入力 by DirectInput
//--------------------------------------------------------------------------------------
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

GamePad DirectInputGamePad::Create(HWND hWnd)
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

bool DirectInputGamePad::getButtonState(GAMEPAD_BUTTON button, STATE_TYPE get_type)
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

//-----------------------------------------------------------------//
//マウス入力 by DirectInput
//-----------------------------------------------------------------//
DirectInputMouse::DirectInputMouse(HWND hWnd)
	: m_pMouse(NULL), m_hWnd(hWnd)
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
PointingDevice DirectInputMouse::Create(HWND hWnd)
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
bool DirectInputMouse::getButtonState(MOUSE_BUTTON button, STATE_TYPE get_type)
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
}
}