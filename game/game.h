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
// �Q�[�����i
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
// �Q�[���N���X
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
	// �N����͎����I�ɍX�V���[�v���쓮���邽�߈ȉ��̃��\�b�h���ꍇ�ɉ����ăI�[�o�[���C�h����
	//-----------------------------------------------------------------------------------------------

	//************************************
	// �L�[�{�[�h��R���g���[���[�̓ǂݍ��݃f�o�C�X�̍쐬��
	// �e�N�X�`���Ȃǂ̃��[�h���E�B���h�E�\���O�ɂ���K�v�̂��邱�Ƃ��L�q����
	//************************************
	virtual void initializeResources(){}

	//************************************
	// ����I�ɌĂ΂�A�O�񂩂�̌o�ߎ��Ԃ������Ƃ���B
	// �Q�[���̏�Ԃ��X�V����B
	//************************************
	virtual void update(float time){}

	//************************************
	// ����I�ɌĂ΂�A�O�񂩂�̌o�ߎ��Ԃ������Ƃ���B
	// �`����s���B
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