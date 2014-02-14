
#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>
#include "../other/refference_count.h"

namespace yuu
{
namespace input
{
//--------------------------------------------------------------------------------------
//入力インタフェース
//--------------------------------------------------------------------------------------

class IController;
class IKeyBoardInput;
class IGamePadInput;
class IPointingDeviceInput;

typedef boost::intrusive_ptr<IController> Controller;
typedef boost::intrusive_ptr<IKeyBoardInput> KeyBoard;
typedef boost::intrusive_ptr<IGamePadInput> GamePad;
typedef boost::intrusive_ptr<IPointingDeviceInput> PointingDevice;

//-----------------------------------------------------------------------------------
//ボタン列挙子
//-----------------------------------------------------------------------------------
//
// ボタンの取得方法
//
enum ButtonState
{
	PRESENT,	// 現在の状態を取得する
	JUST_DOWN,	// 押された直後かを取得する
	JUST_UP		// 離された直後かを取得する
};

//-----------------------------------------------------------------------------------
// 
// コントローラーのボタン
// あるボタンに対し入力割り当てない設定をする場合
// BUTTON_NONEを各入力デバイスの列挙型にキャストして
// 渡さなければならない
// 
enum ControllerButton
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
//-----------------------------------------------------------------------------------
//
// キーボードのボタン
// 
enum KeyboardButton
{
	KB_BUTTON_A,
	KB_BUTTON_B,
	KB_BUTTON_C,
	KB_BUTTON_D,
	KB_BUTTON_E,
	KB_BUTTON_F,
	KB_BUTTON_G,
	KB_BUTTON_H,
	KB_BUTTON_I,
	KB_BUTTON_J,
	KB_BUTTON_K,
	KB_BUTTON_L,
	KB_BUTTON_M,
	KB_BUTTON_N,
	KB_BUTTON_O,
	KB_BUTTON_P,
	KB_BUTTON_Q,
	KB_BUTTON_R,
	KB_BUTTON_S,
	KB_BUTTON_T,
	KB_BUTTON_U,
	KB_BUTTON_V,
	KB_BUTTON_W,
	KB_BUTTON_X,
	KB_BUTTON_Y,
	KB_BUTTON_Z,
	KB_BUTTON_SHIFT,
	KB_BUTTON_CTRL,
	KB_BUTTON_ESCAPE,
	KB_BUTTON_DELETE,
	KB_BUTTON_LEFT,
	KB_BUTTON_RIGHT,
	KB_BUTTON_UP,
	KB_BUTTON_DOWN,
	KB_BUTTON_1,
	KB_BUTTON_2,
	KB_BUTTON_3,
	KB_BUTTON_4,
	KB_BUTTON_5,
	KB_BUTTON_6,
	KB_BUTTON_7,
	KB_BUTTON_8,
	KB_BUTTON_9,
	KB_BUTTON_0,
	KB_BUTTON_NUM
};
//-----------------------------------------------------------------------------------
//
// マウスのボタン
// 
enum MouseButton
{
	MB_BUTTON_0,
	MB_BUTTON_1,
	MB_BUTTON_2,
	MB_BUTTON_3,
	MB_BUTTON_NUM
};
//-----------------------------------------------------------------------------------
//
// ゲームパッドのボタン
// 
enum GamepadButton
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
	: public yuu::RefferenceCount<IController>
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
	virtual bool getButtonState(ControllerButton ctrl, ButtonState get_type = PRESENT) = 0;
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
	virtual void setConfigKeyBoard(ControllerButton ctrl, KeyboardButton key) = 0;
	// ポインティングデバイスの入力をボタンに設定する
	// STATE_TYPE
	// 設定されるボタン
	// 設定するマウスのボタン
	// なし
	virtual void setConfigMouse(ControllerButton ctrl, MouseButton key) = 0;
	// ゲームパッドの入力をボタンに設定する
	// STATE_TYPE
	// 設定されるボタン
	// 設定するゲームパッドののボタン
	// なし
	virtual void setConfigGamepad(ControllerButton ctrl, GamepadButton key) = 0;
};

//-----------------------------------------------------------------//
//キーボード入力処理インタフェース
//-----------------------------------------------------------------//
// キーボードインタフェース
//
// 仮想的なキーボードクラス
//
class IKeyBoardInput
	: public yuu::RefferenceCount<IKeyBoardInput>
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
	virtual bool getKeyState(KeyboardButton key, ButtonState get_type = PRESENT) = 0;
};

//-----------------------------------------------------------------//
//ゲームパッド入力処理インタフェース
//-----------------------------------------------------------------//
// ゲームパッドインタフェース
//
// 仮想的なゲームパッドクラス
//
class IGamePadInput
	: public yuu::RefferenceCount<IGamePadInput>
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
	virtual bool getButtonState(GamepadButton button, ButtonState get_type = PRESENT) = 0;
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
	: public yuu::RefferenceCount<IPointingDeviceInput>
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
	virtual bool getButtonState(MouseButton button, ButtonState get_type = PRESENT) = 0;
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