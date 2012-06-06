#pragma once

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "app/windows_app_base.h"
#include "audio/audio_creator.h"
#include "input/input_direct_input.h"
#include "graphic/graphics_device.h"
#include "graphic/sprite_batch.h"
#include "graphic/texture.h"
#include "other/frame_rate_counter.h"

using namespace yuu;
using namespace yuu::app;
using namespace yuu::audio;
using namespace yuu::input;
using namespace yuu::graphic;

class TestClass : public WindowsGameBase
{
private:
	GraphicDevice device;
	SpriteBatch batch;
	Texture tex;
	Controller controller;
	FrameRateCounter<> counter;

public:
	TestClass() {}

	//初期化
	void initialize();
	//メインプロセス
	void mainProcess();
};
