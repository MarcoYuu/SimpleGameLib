// InputAsDirectInput.h
// DirectInputを用いた入力クラスヘッダ
// 簡易説明
// 			DirectInputを用いた入力クラスのヘッダ
//
// 0.1
#pragma once

#include <app/windows_window_base.h>
#include <input/input_interface.h>

// 自分のライブラリの名前空間
namespace yuu
{
// 入力関連
namespace input
{
//--------------------------------------------------------------------------------------
//DirectInputによるコントローラー
//--------------------------------------------------------------------------------------

// 正しく取得できなかったときはNULLが返る
Controller CreateDirectInputController(app::Window window);

}
}