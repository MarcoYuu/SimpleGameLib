#include "game.h"
#include "../other/utility.h"
#include "../other/frame_rate_counter.h"
#include "../input/input_direct_input.h"
#include <string>

namespace yuu{
namespace game{

using namespace app;
using namespace input;
using namespace graphic;

//-----------------------------------------------------------------------------------------------
// ゲーム部品
//-----------------------------------------------------------------------------------------------
GraphicDevice IGameComponent::getGraphicDevice() const
{
	return m_device;
}
Controller IGameComponent::getController() const
{
	return m_controller;
}
const Game& IGameComponent::getGame() const{
	return *m_game;
}

void IGameComponent::initialize( const Game &game )
{
	m_game =&game;
	m_device =game.getGraphicDevice();
	m_controller =game.getController();
}

void IGameComponent::setPriority( int p )
{
	m_priority =p;
}

bool IGameComponent::operator<( const IGameComponent& rhs ) const
{
	return m_priority<rhs.m_priority;
}


//-----------------------------------------------------------------------------------------------
// ゲームクラス
//-----------------------------------------------------------------------------------------------
Game::Game(const tstring &title, graphic::WINDOW_SIZE size, bool fullscreen, bool vsync)
	: m_device()
	, m_components()
	, m_controller()
	, m_counter()
	, m_prev_time()
{
	// ウィンドウの作成
	Size w_size =GetSize(size);
	Window window =WindowBase::create(title, (int)w_size.x, (int)w_size.y, SIMPLE2, !fullscreen);
	window->setTitle(title);
	setWindow(window);

	// グラフィックデバイスの作成
	m_device =GraphicDeviceManager::create(window->getHandle(), size, !fullscreen, vsync);
	
	// カウンタの作成
	if(ClockFreqCounter::isAvailable())
		m_counter.reset(new ClockFreqCounter()); 
	else
		m_counter.reset(new MultiMediaCounter()); 

	// コントローラの作成
	m_controller =CreateDirectInputController(window->getHandle());
}

Game::~Game()
{
}

GraphicDevice Game::getGraphicDevice() const
{
	return m_device;
}

Controller Game::getController() const
{
	return m_controller;
}

bool Game::isActive() const
{
	return getWindow()->getHandle() == ::GetActiveWindow();
}

void Game::addComponent( GameComponent component, int priority )
{
	component->initialize(*this);
	component->setPriority(priority);
	m_components.insert(component);
}

void Game::removeComponent( GameComponent component )
{
	m_components.erase(component);
}

void Game::initialize()
{
	initializeResources();
	getWindow()->show();
	m_prev_time =m_counter->getTimeInSeconds();
}

void Game::mainProcess()
{
	double curr =m_counter->getTimeInSeconds();
	double diff =curr-m_prev_time;
	m_prev_time =curr;

	m_controller->update();

	update((float)diff);
	ComponentIterator end =m_components.end();
	for(ComponentIterator it =m_components.begin();it != end;++it)
		(*it)->update((float)diff);

	//描画開始
	m_device->device()->BeginScene();
	{
		draw((float)diff);
		for(ComponentIterator it =m_components.begin();it != end;++it)
			(*it)->draw((float)diff);
	}
	//描画終了
	m_device->device()->EndScene();
	m_device->device()->Present(NULL, NULL, NULL, NULL);
}

}
}