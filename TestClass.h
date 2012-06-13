#pragma once

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "audio/audio_creator.h"
#include "input/input_direct_input.h"
#include "graphic/graphics.h"
#include "other/frame_rate_counter.h"
#include "game/game.h"
#include "game/scene_manager.h"

using namespace yuu;
using namespace yuu::app;
using namespace yuu::game;
using namespace yuu::audio;
using namespace yuu::input;
using namespace yuu::graphic;

//-----------------------------------------------------------------------------------------------
// ゲームクラステスト
//-----------------------------------------------------------------------------------------------
class TestClass : public Game
{
public:
	TestClass();

protected:
	void initializeResources();
	void update(float time);
	void draw(float time);

private:
	FrameRateCounter<> counter;
	SceneManager manager;
};

//-----------------------------------------------------------------------------------------------
// ゲームシーンテスト
//-----------------------------------------------------------------------------------------------
class TestScene : public IScene{
public:
	TestScene(SceneManagerComponent &manager);
	static Scene create(SceneManagerComponent &manager);

	void update(float time , bool other_has_focus, bool covered_by_other);
	void updateOnActive(float time);
	void draw(float time);

private:
	typedef IScene base;

	SpriteBatch batch;
	Texture tex[2];

	Point2f p;
	float rot;
	Color color;

private:
	void fade();
	bool isFade() const;
};
