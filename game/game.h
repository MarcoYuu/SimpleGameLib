#pragma once

#include <set>

#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include <app/application_base.h>
#include <graphic/graphics.h>
#include <input/input_interface.h>
#include <other/refference_count.h>
#include <other/timer.h>

namespace yuu{
namespace game{

using namespace app;
using namespace input;
using namespace graphic;

//-----------------------------------------------------------------------------------------------
// �Q�[�����i
//-----------------------------------------------------------------------------------------------
// 
// Game::addComponent()��ɂ��ׂẴ��\�b�h���L���ɂȂ�
// �R���|�[�l���g�Ƃ��Ēǉ������܂ł̓��\�b�h�̗L�����͕ۏ؂���Ȃ�
// 
class Game;
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
// �Q�[���N���X
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
	// �`����s���B�`��J�n�E�I���͌Ă΂�邽�߂ق��ŌĂ�ł͂����Ȃ��B
	// ���̃I�[�o�[���C�h�������R���|�[�l���g�̕`��(�D�揇�ʏ�)�̏��ɍs����
	//************************************
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