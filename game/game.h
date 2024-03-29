#pragma once

#include <set>

#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include "../app/application_base.h"
#include "../graphic/graphics.h"
#include "../input/input_interface.h"
#include "../other/refference_count.h"
#include "../other/timer.h"

namespace yuu{
	namespace game{

		using namespace app;
		using namespace input;
		using namespace graphic;

		class Game;

		//-----------------------------------------------------------------------------------------------
		// ゲーム部品
		//-----------------------------------------------------------------------------------------------
		// 
		// Game::addComponent()後にすべてのメソッドが有効になる
		// コンポーネントとして追加されるまではメソッドの有効性は保証されない
		// 
		class GameComponentBase : public RefferenceCount<GameComponentBase>
		{
			friend class Game;
		public:
			virtual ~GameComponentBase(){}

			GraphicDevice getGraphicDevice() const;
			Controller getController() const;
			const Game& getGame() const;

		protected:
			virtual void update(float time)=0;
			virtual void draw(float time)=0;

		private:
			void initialize(const Game &game);
			void setPriority(int p);

			const Game *m_game;
			GraphicDevice m_device;
			Controller m_controller;

			int m_priority;
			bool operator<(const GameComponentBase& rhs) const;
		};
		typedef boost::intrusive_ptr<GameComponentBase> GameComponent;

		//-----------------------------------------------------------------------------------------------
		// ゲームクラス
		//-----------------------------------------------------------------------------------------------
		class Game : public app::WindowsGameBase
		{
		public:
			Game(const tstring &title, BackBufferSize size, bool fullscreen, bool vsync);
			virtual ~Game();

			GraphicDevice getGraphicDevice() const;
			Controller getController() const;

			bool isActive() const;

		protected:
			void addComponent(GameComponent, int priority =0);
			void removeComponent(GameComponent);

			void setController(Controller controller);

			//-----------------------------------------------------------------------------------------------
			// 起動後は自動的に更新ループが駆動するため以下のメソッドを場合に応じてオーバーライドする
			//-----------------------------------------------------------------------------------------------

			//-----------------------------------------------------------------------------------------------
			// キーボードやコントローラーの読み込みデバイスの作成や
			// テクスチャなどのロード等ウィンドウ表示前にする必要のあることを記述する
			//-----------------------------------------------------------------------------------------------
			virtual void initializeResources(){}

			//-----------------------------------------------------------------------------------------------
			// 定期的に呼ばれ、前回からの経過時間を引数とする。
			// ゲームの状態を更新する。
			//-----------------------------------------------------------------------------------------------
			virtual void update(float time){}

			//-----------------------------------------------------------------------------------------------
			// 定期的に呼ばれ、前回からの経過時間を引数とする。
			// 描画を行う。描画開始・終了は呼ばれるためほかで呼んではいけない。
			// このオーバーライド→内包するコンポーネントの描画(優先順位順)の順に行われる
			//-----------------------------------------------------------------------------------------------
			virtual void draw(float time){}

		private:
			typedef boost::scoped_ptr<ICountTime> Counter;
			typedef std::multiset<GameComponent> ComponentList; 
			typedef ComponentList::iterator ComponentIterator;

			GraphicDevice m_device;
			ComponentList m_components;
			Controller m_controller;
			Counter m_counter;
			double m_prev_time;

			void initialize();
			void mainProcess();
		};
	}
}