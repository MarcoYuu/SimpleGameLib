// InputInterface.h
// 入力インタフェースヘッダ
// 簡易説明
// 			入力インタフェースのヘッダ
//
// 0.1
#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

#include <dinput.h>
#include <stdexcept>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"

// 自分のライブラリの名前空間
namespace yuu
{
// 入力関連
namespace input
{
//--------------------------------------------------------------------------------------
//入力インタフェース
//--------------------------------------------------------------------------------------

class IController;
class IKeyBoardInput;
class IGamePadInput;
class IPointingDeviceInput;

// _IControllerクラスへのポインタ
typedef boost::intrusive_ptr<IController> Controller;
// _IKeyBoardInputクラスへのポインタ
typedef boost::intrusive_ptr<IKeyBoardInput> KeyBoard;
// _IGamePadInputクラスへのポインタ
typedef boost::intrusive_ptr<IGamePadInput> GamePad;
// _IPointingDeviceInputクラスへのポインタ
typedef boost::intrusive_ptr<IPointingDeviceInput> PointingDevice;

//-----------------------------------------------------------------------------------
//ボタン列挙子
//-----------------------------------------------------------------------------------
// ボタンの取得方法
enum STATE_TYPE
{
	PRESENT,	// <現在の状態を取得する
	JUST_DOWN,	// <押された直後かを取得する
	JUST_UP		// <離された直後かを取得する
};
// コントローラーのボタン
enum CONTROL_BUTTON
{
	CB_BUTTON_0,
	CB_BUTTON_1,
	CB_BUTTON_2,
	CB_BUTTON_3,
	CB_BUTTON_4,
	CB_BUTTON_5,
	CB_BUTTON_6,
	CB_BUTTON_7,
	CB_BUTTON_8,
	CB_BUTTON_9,
	CB_BUTTON_10,
	CB_BUTTON_11,
	CB_BUTTON_12,
	CB_BUTTON_13,
	CB_BUTTON_14,
	CB_BUTTON_15,
	CB_BUTTON_LEFT,
	CB_BUTTON_RIGHT,
	CB_BUTTON_UP,
	CB_BUTTON_DOWN,
	CB_BUTTON_NUM,
	BUTTON_NONE = 0xffffffff
};
// キーボードのボタン
enum KEYBOARD_BUTTON
{
	KB_BUTTON_A         = DIK_A,
	KB_BUTTON_B         = DIK_B,
	KB_BUTTON_C         = DIK_C,
	KB_BUTTON_D         = DIK_D,
	KB_BUTTON_E         = DIK_E,
	KB_BUTTON_F         = DIK_F,
	KB_BUTTON_G         = DIK_G,
	KB_BUTTON_H         = DIK_H,
	KB_BUTTON_I         = DIK_I,
	KB_BUTTON_J         = DIK_J,
	KB_BUTTON_K         = DIK_K,
	KB_BUTTON_L         = DIK_L,
	KB_BUTTON_M         = DIK_M,
	KB_BUTTON_N         = DIK_N,
	KB_BUTTON_O         = DIK_O,
	KB_BUTTON_P         = DIK_P,
	KB_BUTTON_Q         = DIK_Q,
	KB_BUTTON_R         = DIK_R,
	KB_BUTTON_S         = DIK_S,
	KB_BUTTON_T         = DIK_T,
	KB_BUTTON_U         = DIK_U,
	KB_BUTTON_V         = DIK_V,
	KB_BUTTON_W         = DIK_W,
	KB_BUTTON_X         = DIK_X,
	KB_BUTTON_Y         = DIK_Y,
	KB_BUTTON_Z         = DIK_Z,
	KB_BUTTON_SHIFT		= DIK_LSHIFT,
	KB_BUTTON_CTRL		= DIK_LCONTROL,
	KB_BUTTON_ESCAPE	= DIK_ESCAPE,
	KB_BUTTON_DELETE	= DIK_DELETE,
	KB_BUTTON_LEFT		= DIK_LEFT,
	KB_BUTTON_RIGHT		= DIK_RIGHT,
	KB_BUTTON_UP		= DIK_UP,
	KB_BUTTON_DOWN		= DIK_DOWN,
	KB_BUTTON_1         = DIK_1,
	KB_BUTTON_2         = DIK_2,
	KB_BUTTON_3         = DIK_3,
	KB_BUTTON_4         = DIK_4,
	KB_BUTTON_5         = DIK_5,
	KB_BUTTON_6         = DIK_6,
	KB_BUTTON_7         = DIK_7,
	KB_BUTTON_8         = DIK_8,
	KB_BUTTON_9         = DIK_9,
	KB_BUTTON_0         = DIK_0,
	KB_BUTTON_NUM
};
// マウスのボタン
enum MOUSE_BUTTON
{
	MB_BUTTON_0,
	MB_BUTTON_1,
	MB_BUTTON_2,
	MB_BUTTON_3,
	MB_BUTTON_NUM
};
// ゲームパッドのボタン
enum GAMEPAD_BUTTON
{
	GB_BUTTON_0,
	GB_BUTTON_1,
	GB_BUTTON_2,
	GB_BUTTON_3,
	GB_BUTTON_4,
	GB_BUTTON_5,
	GB_BUTTON_6,
	GB_BUTTON_7,
	GB_BUTTON_8,
	GB_BUTTON_9,
	GB_BUTTON_10,
	GB_BUTTON_11,
	GB_BUTTON_12,
	GB_BUTTON_13,
	GB_BUTTON_14,
	GB_BUTTON_15,
	GB_BUTTON_16,
	GB_BUTTON_17,
	GB_BUTTON_18,
	GB_BUTTON_19,
	GB_BUTTON_20,
	GB_BUTTON_21,
	GB_BUTTON_22,
	GB_BUTTON_23,
	GB_BUTTON_24,
	GB_BUTTON_25,
	GB_BUTTON_26,
	GB_BUTTON_27,
	GB_BUTTON_28,
	GB_BUTTON_29,
	GB_BUTTON_30,
	GB_BUTTON_31,
	GB_BUTTON_LEFT,
	GB_BUTTON_RIGHT,
	GB_BUTTON_UP,
	GB_BUTTON_DOWN,
	GB_BUTTON_NUM,
	GB_ANALOG_X,
	GB_ANALOG_Y,
	GB_ANALOG_Z
};

//-----------------------------------------------------------------//
//コントローラーインタフェース
//-----------------------------------------------------------------//
// コントローラインタフェース
//
// 仮想的なゲームコントローラクラス
//
class IController
	: public yuu::IRefferenceCount<IController>
	, boost::noncopyable
{
public:
	virtual ~IController() {}

	// 入力更新
	// なし
	virtual void update() = 0;
	// ボタンの状態を取得
	// STATE_TYPE
	// 取得するボタン
	// 取得状態
	// ボタンの状態
	virtual bool getButtonState(CONTROL_BUTTON ctrl, STATE_TYPE get_type = PRESENT) = 0;
	// 軸の状態を取得
	// STATE_TYPE
	// 水平方向　:-1.0～1.0で値を返すこと
	// 垂直方向　:-1.0～1.0で値を返すこと
	// 鉛直方向　:-1.0～1.0で値を返すこと
	// なし
	virtual void getAnalogAxisState(double &x, double &y, double &z) = 0;
	// 軸の回転を取得
	// STATE_TYPE
	// 水平方向
	// 垂直方向
	// 鉛直方向
	// なし
	virtual void getAnalogRotateState(double &rx, double &ry, double &rz) = 0;
	// 変化量を取得
	// 平行方向
	// 垂直方向
	// 状態
	virtual void getRelativePos(double &x, double &y) = 0;
	// 絶対位置を取得
	// 平行方向
	// 垂直方向
	// 状態
	virtual void getAbsScreenPos(double &x, double &y) = 0;
	// ホイールの変化量を取得
	// 変化量
	// 状態
	virtual void getWheelMove(double &z) = 0;

	// ボタン設定を初期状態に戻す
	// なし
	virtual void setDefault() = 0;
	// キーボードの入力をボタンに設定する
	// STATE_TYPE
	// 設定されるボタン
	// 設定するキーボードのキー
	// なし
	virtual void setConfigKeyBoard(CONTROL_BUTTON ctrl, KEYBOARD_BUTTON key) = 0;
	// ポインティングデバイスの入力をボタンに設定する
	// STATE_TYPE
	// 設定されるボタン
	// 設定するマウスのボタン
	// なし
	virtual void setConfigMouse(CONTROL_BUTTON ctrl, MOUSE_BUTTON key) = 0;
	// ゲームパッドの入力をボタンに設定する
	// STATE_TYPE
	// 設定されるボタン
	// 設定するゲームパッドののボタン
	// なし
	virtual void setConfigGamepad(CONTROL_BUTTON ctrl, GAMEPAD_BUTTON key) = 0;
};

//-----------------------------------------------------------------//
//キーボード入力処理インタフェース
//-----------------------------------------------------------------//
// キーボードインタフェース
//
// 仮想的なキーボードクラス
//
class IKeyBoardInput
	: public yuu::IRefferenceCount<IKeyBoardInput>
	, boost::noncopyable
{
public:
	virtual ~IKeyBoardInput() {}

	// 入力情報を更新
	// なし
	virtual void update() = 0;

	// キーの情報を取得
	// 取得するキー
	// 取得状態
	// 状態
	virtual bool getKeyState(KEYBOARD_BUTTON key, STATE_TYPE get_type = PRESENT) = 0;
};

//-----------------------------------------------------------------//
//ゲームパッド入力処理インタフェース
//-----------------------------------------------------------------//
// ゲームパッドインタフェース
//
// 仮想的なゲームパッドクラス
//
class IGamePadInput
	: public yuu::IRefferenceCount<IGamePadInput>
	, boost::noncopyable
{
public:
	virtual ~IGamePadInput() {}

	// 入力情報を更新
	// なし
	virtual void update() = 0;
	// ボタンの情報を取得
	// 取得するボタン
	// 取得状態
	// 状態
	virtual bool getButtonState(GAMEPAD_BUTTON button, STATE_TYPE get_type = PRESENT) = 0;
	// 軸の状態を取得
	// STATE_TYPE
	// 水平方向　:-1.0～1.0で値を返すこと
	// 垂直方向　:-1.0～1.0で値を返すこと
	// 鉛直方向　:-1.0～1.0で値を返すこと
	// なし
	virtual bool getAnalogAxisState(double &x, double &y, double &z) = 0;
	// 軸の状態を取得
	// STATE_TYPE
	// 水平方向
	// 垂直方向
	// 鉛直方向
	// なし
	virtual bool getAnalogRotateState(double &rx, double &ry, double &rz) = 0;
};

//-----------------------------------------------------------------//
//ポインティングデバイスの入力処理インタフェース
//-----------------------------------------------------------------//
// ポインティングデバイスインタフェース
//
// 仮想的なポインティングデバイスクラス
//
class IPointingDeviceInput
	: public yuu::IRefferenceCount<IPointingDeviceInput>
	, boost::noncopyable
{
public:
	virtual ~IPointingDeviceInput() {}

	// 入力情報を更新
	// なし
	virtual void update() = 0;
	// ボタンの情報を取得
	// 取得するボタン
	// 取得状態
	// 状態
	virtual bool getButtonState(MOUSE_BUTTON button, STATE_TYPE get_type = PRESENT) = 0;
	// 変化量を取得
	// 平行方向
	// 垂直方向
	// 状態
	virtual bool getRelativePos(double &x, double &y) = 0;
	// 絶対位置を取得
	// 平行方向
	// 垂直方向
	// 状態
	virtual bool getAbsScreenPos(double &x, double &y) = 0;
	// ホイールの変化量を取得
	// 変化量
	// 状態
	virtual bool getWheelMove(double &z) = 0;
};
}
}