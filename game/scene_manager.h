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
// �V�[���}�l�[�W��
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
// �Q�[���V�[��
//-----------------------------------------------------------------------------------------------
// �A�N�e�B�u�F
// �@�@�V�[���X�^�b�N�̈�ԏ�ɂ��Ă��t�H�[�J�X�𓾂Ă����Ԃł�
// �@�@�t�H�[�J�X�𓾂邱�Ƃ��ł��Ȃ��Ƃ��AgetState()��ACTIVE��������
// �@�@�\��������܂����AisActive()��false�ƂȂ�܂�
// ��A�N�e�B�u�F
// �@�@�V�[���X�^�b�N�œ�Ԗڈȍ~�̃V�[���̏�Ԃł�
// �@�@���̏�Ԃł͕`��E�X�V�Ƃ��Ɋ�{�I�ɂ͍s���܂���
// �@�@�g�b�v�̃V�[����Popup��Ԃ̎��A���̉��̃V�[���͕`��̂ݍs���܂�
enum SceneState{
	TRANSITION_ON,	// �V�[�����A�N�e�B�u�ɂȂ��Ă���r���ł�		getState()!= 0,1 ������
	TRANSITION_OFF,	// �V�[������A�N�e�B�u�ɂȂ��Ă���r���ł�	getState()!= 0,1 �㏸��
	ACTIVE,			// �V�[���͊��S�ɃA�N�e�B�u�ł�				getState()!= 0
	HIDDEN			// �V�[���͊��S�ɔ�A�N�e�B�u�ł�				getState()!= 1 
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

	// �}�l�[�W���ɂ���čX�V���ɏ�ɌĂяo�����
	// �I�[�o�[���C�h��ŕK���Ăяo���Ȃ��Ă͂Ȃ�Ȃ�
	// ��������true�̂Ƃ��A�N�e�B�u�łȂ��Ȃ�\��������
	// ��O������true�̂Ƃ�setTransitionOffTime�Ɏw�肵��
	// ���Ԉȓ��ɃA�N�e�B�u�łȂ��Ȃ�\��������
	virtual void update(float time , bool other_has_focus, bool covered_by_other);

	// �A�N�e�B�u�Ȏ��̂݌Ăяo�����
	virtual void updateOnActive(float time){}

	// ��Ԃ�HIDDEN�łȂ��Ƃ��̂݌Ăяo�����
	// �����Ȃ鎞���`�悪�K�v�Ȏ��Aupdate���n���h����
	// ��O���������false�Ɏw�肷��K�v������
	virtual void draw(float time){}

	GraphicDevice getGraphicDevice() const;
	Controller getController() const;

	// ���̃V�[�����I�����܂�
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
