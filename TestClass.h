#pragma once

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "audio/audio_creator.h"
#include "input/input_direct_input.h"
#include "graphic/graphics.h"
#include "other/frame_rate_counter.h"
#include "other/memory_manage_list.h"
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
	TestClass(WINDOW_SIZE);

	static const size_t DefaultScreenWidth =1280;
	static const size_t DefaultScreenHeight =960;
	static Point2f NormalizeCoord(Point2f &point);
	static float WindowSizeRatio();

protected:
	void initializeResources();
	void update(float time);
	void draw(float time);

private:
	FrameRateCounter<> counter;
	SceneManager manager;

	static Size real_win_size;
};


//-----------------------------------------------------------------------------------------------
// テストオブジェクト
//-----------------------------------------------------------------------------------------------
class TestObject{
public:
	TestObject(GraphicDevice dev);

	void update(Controller controller);
	void draw();

public:
	class Bullet{
	private:
		Point2f position;
		Vector2f velocity;
		Vector2f accel;

		TestObject *parent;

	public:
		Bullet(TestObject *p);
		void init(const Point2f &pos, const Point2f &vel, const Point2f &acl);
		bool update();
		void draw();
	};

private:
	GraphicDevice device;
	SpriteBatch batch;
	Texture tex;

	int count;
	float rot;

	Point2f position;
	Vector2f velocity;

	static const int BULLET_NUM =2000;

	MemoryManageList<Bullet> bullets;
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
	Color color;

	float rot;

	TestObject object;

private:
	void fade();
	bool isFade() const;
};

