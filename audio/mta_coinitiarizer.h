// MTA_CoInitiarizer.h
// COM初期化クラスヘッダ
// 簡易説明
//
// 			Init()を呼び出すとMTAに初期化し、プログラム終了時にCOMを終了させる。
// 			マルチスレッドは安全ではないかもしれない
//
// 0.2
#pragma once

#if defined(_WIN32) | defined(_WIN64)
#include <ObjBase.h>
#include <cstdlib>

// 自分のライブラリの名前空間
namespace yuu
{
	// COM初期化クラス
	//
	// Init()を呼び出すとMTAに初期化し、プログラム終了時にCOMを終了させる。
	class MTA_CoInitiarizer
	{
		friend int ::atexit(void ( *)(void));

	public:
		// COMの初期化
		// なし
		static void Init()
		{
			static bool initialized = false;

			//初期化されていなかったらCOMを初期化
			if(!initialized)
			{
				initialized = true;
				CoInitializeEx(0, COINIT_MULTITHREADED);

				//プログラム終了時に終了処理
				std::atexit(MTA_CoInitiarizer::UnInit);
			}
		}
	private:
		static void UnInit()
		{
			//終了処理
			CoUninitialize();
		}
	};
}
#endif
