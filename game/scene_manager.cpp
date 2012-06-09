#include "scene_manager.h"

namespace yuu{
namespace game{

//-----------------------------------------------------------------------------------------------
// �Q�[���V�[��
//-----------------------------------------------------------------------------------------------
IScene::IScene( SceneManagerComponent &manager ) : m_is_popup(false)
	, m_is_exitting(false)
	, m_have_focus(false)
	, m_transition_on_time(0)
	, m_transition_off_time(0)
	, m_transition_state(1.0)
	, m_state(TRANSITION_ON)
	, m_manager(manager)
{}

bool IScene::isPopup() const
{
	return m_is_popup;
}

bool IScene::isExitting() const
{
	return m_is_exitting;
}

bool IScene::isActive() const
{
	return m_have_focus && (m_state==TRANSITION_ON||m_state==ACTIVE);
}

bool IScene::hasFocus() const
{
	return m_have_focus;
}

yuu::game::SceneState IScene::getState() const
{
	return m_state;
}

yuu::graphic::GraphicDevice IScene::getGraphicDevice() const
{
	return m_manager.getGraphicDevice();
}

yuu::input::Controller IScene::getController() const
{
	return m_manager.getController();
}

SceneManagerComponent& IScene::getSceneManager() const
{
	return m_manager;
}

void IScene::setIsPopup( bool flag )
{
	m_is_popup =flag;
}

void IScene::setTransitionOnTime( double time )
{
	m_transition_on_time =time;
}

void IScene::setTransitionOffTime( double time )
{
	m_transition_off_time =time;
}


void IScene::update( float time , bool other_has_focus, bool covered_by_other )
{
	// �t�H�[�J�X�̍X�V
	this->m_have_focus = !other_has_focus;

	// �V�[�����I���֌�������
	if (m_is_exitting)
	{
		m_state = TRANSITION_OFF;
		if (!updateTransition(time, m_transition_off_time, 1))
			m_manager.removeScene(this);
	}
	// �ق��̃V�[������ɂ���Ƃ�
	else if (covered_by_other)
	{
		if (updateTransition(time, m_transition_off_time, 1))
			m_state = TRANSITION_OFF;
		else
			m_state = HIDDEN;
	}
	// �V�[�����A�N�e�B�u�̎�
	else
	{
		// �A�N�e�B�u��Ԃֈڍs��
		if (updateTransition(time, m_transition_on_time, -1))
			m_state = TRANSITION_ON;
		// ���S�A�N�e�B�u
		else
			m_state = ACTIVE;
	}
}

bool IScene::updateTransition( float time, double transition_time, int direction )
{
	double transitio_delta;

	// �҂���0�Ȃ��C�Ɉړ��A����ȊO�Ȃ�transition_time�ňڍs��������悤�ɂ���
	if (transition_time == 0)
		transitio_delta = 1;
	else
		transitio_delta = (double)time/transition_time;

	// �ڍs�ʒu���X�V���܂��B
	m_transition_state += transitio_delta * direction;

	// �ڍs�̍Ō�(1 or 0)�ɓ��B�������ǂ���
	if (((direction < 0) && (m_transition_state <= 0)) ||
		((direction > 0) && (m_transition_state >= 1)))
	{
		m_transition_state = min(max(m_transition_state,0),1);

		// �ڍs�I��
		return false;
	}

	// �܂��ڍs��
	return true;
}


void IScene::exit()
{
	// �I���܂ł̗P�\������Ƃ���exitting��true�ɂ��Ă�������
	if (m_transition_off_time == 0)
		m_manager.removeScene(this);
	else
		m_is_exitting = true;
}

//-----------------------------------------------------------------------------------------------
// �V�[���}�l�[�W��
//-----------------------------------------------------------------------------------------------
SceneManagerComponent::SceneManagerComponent() :IGameComponent()
{

}

SceneManagerComponent::~SceneManagerComponent()
{
	m_scene.clear();
}

void SceneManagerComponent::update( float time )
{
	// �����ʂ̍X�V�̉ߒ��ŕʂ̉�ʂ�ǉ��܂��͍폜����ꍇ��
	// ������h�����߁A�}�X�^�[��ʃ��X�g�̃R�s�[���쐬���܂��B
	SceneList to_update =m_scene;

	//bool other_has_focus = !getGame().isActive();
	bool other_has_focus   = false;
	bool covered_by_other  = false;

	// �X�V��ҋ@�����ʂ����݂�����胋�[�v���܂��B
	while (to_update.size() > 0)
	{
		// �ŏ�ʂ̉�ʂ�ҋ@���X�g����|�b�v �I�t���܂��B
		Scene scene = to_update.back();
		to_update.pop_back();

		// ��ʂ��X�V���܂��B
		scene->update(time, other_has_focus, covered_by_other);

		//if (scene->getState()==TRANSITION_ON||scene->getState()==ACTIVE)
		if (scene->isActive())
		{
			// ���̉�ʂ��ŏ��ɕ\�����ꂽ�A�N�e�B�u�ȉ�ʂł���ꍇ�A
			// ���͂���������@��񋟂���܂��B
			if (!other_has_focus)
			{
				scene->updateOnActive(time);
				other_has_focus = true;
			}

			// ���̉�ʂ��A�N�e�B�u�Ȕ�|�b�v�A�b�v��ʂ̏ꍇ�A
			// �ȍ~�̉�ʂ����̉�ʂɕ����邱�Ƃ�ʒm���܂��B
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
		// ���S�ɉB�ꂽ���̂Ɋւ��Ă͕`�悵�Ȃ�
		if ((*it)->getState() == HIDDEN)
			continue;

		(*it)->draw(time);
	}
}

void SceneManagerComponent::removeScene( Scene scene )
{
	m_scene.remove_if([scene](Scene s){return s==scene;});
}

void SceneManagerComponent::removeScene( IScene *scene )
{
	m_scene.remove_if([scene](Scene s){return s==scene;});
}

void SceneManagerComponent::addScene( Scene scene )
{
	m_scene.push_back(scene);
}

}
}