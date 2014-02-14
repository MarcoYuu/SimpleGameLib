
#include "scene_manager.h"

namespace yuu{
	namespace game{

		//-----------------------------------------------------------------------------------------------
		// ゲームシーン
		//-----------------------------------------------------------------------------------------------
		SceneBase::SceneBase( SceneManagerComponent &manager ) 
			: m_is_popup(false)
			, m_is_exitting(false)
			, m_have_focus(false)
			, m_transition_on_time(0)
			, m_transition_off_time(0)
			, m_transition_state(1.0)
			, m_state(TRANSITION_ON)
			, m_manager(manager)
		{}

		bool SceneBase::isPopup() const
		{
			return m_is_popup;
		}

		bool SceneBase::isExitting() const
		{
			return m_is_exitting;
		}

		bool SceneBase::isActive() const
		{
			return m_have_focus && (m_state==TRANSITION_ON||m_state==ACTIVE);
		}

		bool SceneBase::hasFocus() const
		{
			return m_have_focus;
		}

		yuu::game::SceneState SceneBase::getState() const
		{
			return m_state;
		}

		yuu::graphic::GraphicDevice SceneBase::getGraphicDevice() const
		{
			return m_manager.getGraphicDevice();
		}

		yuu::input::Controller SceneBase::getController() const
		{
			return m_manager.getController();
		}

		SceneManagerComponent& SceneBase::getSceneManager() const
		{
			return m_manager;
		}

		double SceneBase::getTransitionState() const
		{
			return m_transition_state;
		}

		void SceneBase::setIsPopup( bool flag )
		{
			m_is_popup =flag;
		}

		void SceneBase::setTransitionOnTime( double time )
		{
			m_transition_on_time =time;
		}

		void SceneBase::setTransitionOffTime( double time )
		{
			m_transition_off_time =time;
		}


		void SceneBase::update( float time , bool other_has_focus, bool covered_by_other )
		{
			// フォーカスの更新
			this->m_have_focus = !other_has_focus;

			// シーンが終了へ向かう時
			if (m_is_exitting)
			{
				m_state = TRANSITION_OFF;
				if (!updateTransition(time, m_transition_off_time, 1))
					m_manager.removeScene(this);
			}
			// ほかのシーンが上にあるとき
			else if (covered_by_other)
			{
				if (updateTransition(time, m_transition_off_time, 1))
					m_state = TRANSITION_OFF;
				else
					m_state = HIDDEN;
			}
			// それ以外の時
			else
			{
				// 基本的に何もないならアクティブ状態へ移行
				if (updateTransition(time, m_transition_on_time, -1))
					m_state = TRANSITION_ON;
				// 完全アクティブ
				else
					m_state = ACTIVE;
			}
		}

		bool SceneBase::updateTransition( float time, double transition_time, int direction )
		{
			double transition_delta;

			// 待ちが0なら一気に移動、それ以外ならtransition_timeで移行完了するようにする
			if (transition_time == 0)
				transition_delta = 1;
			else
				transition_delta = (double)time/transition_time;

			// 移行位置を更新します。
			m_transition_state += transition_delta * direction;

			// 移行の最後(1 or 0)に到達したかどうか
			if (((direction < 0) && (m_transition_state <= 0)) ||
				((direction > 0) && (m_transition_state >= 1)))
			{
				m_transition_state = min(max(m_transition_state,0),1);

				// 移行終了
				return false;
			}

			// まだ移行中
			return true;
		}


		void SceneBase::exit()
		{
			// 終了までの猶予があるときはexittingをtrueにしておくだけ
			if (m_transition_off_time == 0)
				m_manager.removeScene(this);
			else
				m_is_exitting = true;
		}

		//-----------------------------------------------------------------------------------------------
		// シーンマネージャ
		//-----------------------------------------------------------------------------------------------
		SceneManagerComponent::SceneManagerComponent() 
			:GameComponentBase()
		{
		}

		yuu::game::SceneManager SceneManagerComponent::create()
		{
			return SceneManager(new SceneManagerComponent());
		}

		SceneManagerComponent::~SceneManagerComponent()
		{
			m_scene.clear();
		}

		void SceneManagerComponent::update( float time )
		{
			// ある画面の更新の過程で別の画面を追加または削除する場合の
			// 混乱を防ぐため、マスター画面リストのコピーを作成します。
			SceneList to_update =m_scene;

			//bool other_has_focus = !getGame().isActive();
			bool other_has_focus   = false;
			bool covered_by_other  = false;

			// 更新を待機する画面が存在する限りループします。
			while (to_update.size() > 0)
			{
				// 最上位の画面を待機リストからポップ オフします。
				Scene scene = to_update.back();
				to_update.pop_back();

				// 画面を更新します。
				scene->update(time, other_has_focus, covered_by_other);

				//if (scene->getState()==TRANSITION_ON||scene->getState()==ACTIVE)
				if (scene->isActive())
				{
					// この画面が最初に表示されたアクティブな画面である場合、
					// 入力を処理する機会が提供されます。
					if (!other_has_focus)
					{
						scene->updateOnActive(time);
						other_has_focus = true;
					}

					// この画面がアクティブな非ポップアップ画面の場合、
					// 以降の画面がこの画面に覆われることを通知します。
					if (!scene->isPopup())
						covered_by_other = true;
				}
			}
		}

		void SceneManagerComponent::draw( float time )
		{
			SceneList::iterator it =m_scene.begin(), end =m_scene.end();
			for (;it!=end;++it)
			{
				// 完全に隠れたものに関しては描画しない
				if ((*it)->getState() == HIDDEN)
					continue;

				(*it)->draw(time);
			}
		}

		void SceneManagerComponent::removeScene( Scene scene )
		{
			m_scene.remove_if([scene](Scene s){return s==scene;});
		}

		void SceneManagerComponent::removeScene( SceneBase *scene )
		{
			m_scene.remove_if([scene](Scene s){return s==scene;});
		}

		void SceneManagerComponent::addScene( Scene scene )
		{
			m_scene.push_back(scene);
		}

		size_t SceneManagerComponent::getSceneNum() const
		{
			return m_scene.size();
		}
	}
}