#pragma once

#include <list>
#include <cmath>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <game/game.h>
#include <other/refference_count.h>

namespace yuu{
namespace game{

class IScene;
class SceneManagerComponent;
typedef boost::intrusive_ptr<IScene> Scene;
typedef boost::intrusive_ptr<SceneManagerComponent> SceneManager;

//-----------------------------------------------------------------------------------------------
// シーンマネージャ
//-----------------------------------------------------------------------------------------------
class SceneManagerComponent :public IGameComponent, boost::noncopyable
{
public:
	SceneManagerComponent();
	~SceneManagerComponent();
	static SceneManager create();

	void addScene(Scene scene);
	void update(float time);
	void draw(float time);
	void removeScene( Scene scene );
	void removeScene( IScene *scene );
	size_t getSceneNum() const;

private:
	typedef std::list<Scene> SceneList;
	SceneList m_scene;
};

//-----------------------------------------------------------------------------------------------
// ゲームシーン
//-----------------------------------------------------------------------------------------------
// アクティブ：
// 　　シーンスタックの一番上にいてかつフォーカスを得ている状態です
// 　　フォーカスを得ることができないとき、getState()はACTIVEをかえす
// 　　可能性がありますが、isActive()はfalseとなります
// 非アクティブ：
// 　　シーンスタックで二番目以降のシーンの状態です
// 　　この状態では描画・更新ともに基本的には行われませんが
// 　　トップのシーンがPopup状態の時、その下のシーンは描画のみ行われます
enum SceneState{
	TRANSITION_ON,	// シーンがアクティブになっている途中です		getState()!= 0,1 減少中
	TRANSITION_OFF,	// シーンが非アクティブになっている途中です	getState()!= 0,1 上昇中
	ACTIVE,			// シーンは完全にアクティブです				getState()!= 0
	HIDDEN			// シーンは完全に非アクティブです				getState()!= 1 
};

class IScene :public IRefferenceCount<IScene>, boost::noncopyable
{
public:
	virtual ~IScene(){}

	bool isPopup() const;
	bool isExitting() const;
	bool isActive() const;
	bool hasFocus() const;
	SceneState getState() const;
	double getTransitionState() const; 

	// マネージャによって更新時に常に呼び出される
	// オーバーライド先で必ず呼び出さなくてはならない
	// 第二引数がtrueのときアクティブでなくなる可能性がある
	// 第三引数がtrueのときsetTransitionOffTimeに指定した
	// 時間以内にアクティブでなくなる可能性がある
	virtual void update(float time , bool other_has_focus, bool covered_by_other);

	// アクティブな時のみ呼び出される
	virtual void updateOnActive(float time){}

	// 状態がHIDDENでないときのみ呼び出される
	// いかなる時も描画が必要な時、updateをハンドルし
	// 第三引数を常にfalseに指定する必要がある
	virtual void draw(float time){}

	GraphicDevice getGraphicDevice() const;
	Controller getController() const;

	// このシーンを終了します
	void exit();

protected:
	IScene(SceneManagerComponent &manager);
	SceneManagerComponent& getSceneManager() const; 

	void setIsPopup(bool flag);
	void setTransitionOnTime(double time);
	void setTransitionOffTime(double time);

private:
	bool m_is_popup;
	bool m_is_exitting;
	bool m_have_focus;
	double m_transition_on_time;
	double m_transition_off_time;
	double m_transition_state; //Active:0, Hidden:1 

	SceneState m_state;
	SceneManagerComponent &m_manager;

	bool updateTransition(float time, double transition_time, int direction);
};

}
}
