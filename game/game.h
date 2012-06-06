#pragma once

#include "../app/windows_app_base.h"
#include "../graphic/graphics.h"
#include "../other/frame_rate_counter.h"
#include "../other/refference_count.h"

#include <set>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

namespace yuu{
namespace game{

using namespace app;
using namespace graphic;

//-----------------------------------------------------------------------------------------------
// ゲーム部品
//-----------------------------------------------------------------------------------------------
class Game;
class IGameComponent : public IRefferenceCount<IGameComponent>
{
	friend class Game;
public:
	virtual ~IGameComponent(){}

protected:
	virtual void update(float time)=0;
	virtual void draw(float time)=0;
	GraphicDevice getGraphicDevice();

private:
	void setParent(Game *game);
	void setPriority(int p);
	Game *m_game;
	int m_priority;
	bool operator<(const IGameComponent& rhs) const;
};
typedef boost::intrusive_ptr<IGameComponent> GameComponent;

//-----------------------------------------------------------------------------------------------
// ゲームクラス
//-----------------------------------------------------------------------------------------------
class Game : public app::WindowsGameBase
{
public:
	Game(const tstring &title, WINDOW_SIZE size, bool fullscreen, bool vsync);
	virtual ~Game();

	GraphicDevice getGraphicDevice();

protected:
	void addComponent(GameComponent, int priority =0);
	void removeComponent(GameComponent);

	//-----------------------------------------------------------------------------------------------
	// 起動後は自動的に更新ループが駆動するため以下のメソッドを場合に応じてオーバーライドする
	//-----------------------------------------------------------------------------------------------

	//************************************
	// キーボードやコントローラーの読み込みデバイスの作成や
	// テクスチャなどのロード等ウィンドウ表示前にする必要のあることを記述する
	//************************************
	virtual void initializeResources(){}

	//************************************
	// 定期的に呼ばれ、前回からの経過時間を引数とする。
	// ゲームの状態を更新する。
	//************************************
	virtual void update(float time){}

	//************************************
	// 定期的に呼ばれ、前回からの経過時間を引数とする。
	// 描画を行う。
	//************************************
	virtual void draw(float time){}

private:
	typedef boost::scoped_ptr<ICountTime> Counter;
	typedef std::multiset<GameComponent> ComponentList; 
	typedef ComponentList::iterator ComponentIterator;

	GraphicDevice m_device;
	ComponentList m_components;
	Counter m_counter;
	double m_prev_time;

	void initialize();
	void mainProcess();
};

}
}