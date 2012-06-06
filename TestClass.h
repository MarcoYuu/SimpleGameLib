#pragma once

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "audio/audio_creator.h"
#include "input/input_direct_input.h"
#include "graphic/graphics.h"
#include "other/frame_rate_counter.h"
#include "game/game.h"

using namespace yuu;
using namespace yuu::app;
using namespace yuu::game;
using namespace yuu::audio;
using namespace yuu::input;
using namespace yuu::graphic;

class TestClass : public Game
{
public:
	TestClass();

protected:
	void initializeResources();
	void update(float time);
	void draw(float time);

private:
	SpriteBatch batch;
	Texture tex;
	Controller controller;
	FrameRateCounter<> counter;

	Point2f p;
	float rot;
};
