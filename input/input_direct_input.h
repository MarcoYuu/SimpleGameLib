// InputAsDirectInput.h
// DirectInputを用いた入力クラスヘッダ
// 簡易説明
// 			DirectInputを用いた入力クラスのヘッダ
//
// 0.1
#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

#include <dinput.h>
#include "input_interface.h"

// 自分のライブラリの名前空間
namespace yuu
{
// 入力関連
namespace input
{
//--------------------------------------------------------------------------------------
//DirectInputの初期化
//--------------------------------------------------------------------------------------
// 入力の初期化
//
// 入力デバイスを初期化します。
// 初期化に失敗した場合例外を投げます。
// CreateAudio
// なし
void InitInput();
Controller CreateDirectInputController(HWND hWnd);

//--------------------------------------------------------------------------------------
//キーボード入力DirectInput
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
	static KeyBoard Create(HWND hWnd);

	void update();
	bool getKeyState(KEYBOARD_BUTTON key, STATE_TYPE get_type);
};

//--------------------------------------------------------------------------------------
//ゲームパッド入力 by DirectInput
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
	static GamePad Create(HWND hWnd);

	void update();
	bool getButtonState(GAMEPAD_BUTTON button, STATE_TYPE get_type = PRESENT);
	bool getAnalogAxisState(double &x, double &y, double &z);
	bool getAnalogRotateState(double &rx, double &ry, double &rz);
};

//--------------------------------------------------------------------------------------
//マウス入力 by DirectInput
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
	static PointingDevice Create(HWND hWnd);

	void update();
	bool getButtonState(MOUSE_BUTTON button, STATE_TYPE get_type = PRESENT);
	bool getRelativePos(double &x, double &y);
	bool getAbsScreenPos(double &x, double &y);
	bool getWheelMove(double &z);
};
}
}