#include "game.h"
#include "../other/utility.h"
#include "../other/frame_rate_counter.h"
#include <string>

namespace yuu{
namespace game{

using namespace app;
using namespace graphic;

//-----------------------------------------------------------------------------------------------
// ゲーム部品
//-----------------------------------------------------------------------------------------------
GraphicDevice IGameComponent::getGraphicDevice()
{
	return m_game->getGraphicDevice();
}

void IGameComponent::setParent( Game *game )
{
	m_game =game;
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
{
	Size w_size =GetSize(size);
	Window window =WindowBase::create(title, (int)w_size.x, (int)w_size.y, SIMPLE2, !fullscreen);
	window->setTitle(title);
	setWindow(window);

	m_device =GraphicDeviceManager::create(window->getHandle(), size, !fullscreen, vsync);
	if(ClockFreqCounter::isAvailable())
		m_counter.reset(new ClockFreqCounter()); 
	else
		m_counter.reset(new MultiMediaCounter()); 
}

Game::~Game()
{
}

GraphicDevice Game::getGraphicDevice()
{
	return m_device;
}

void Game::addComponent( GameComponent component, int priority )
{
	component->setParent(this);
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

	update((float)diff);
	draw((float)diff);
	ComponentIterator end =m_components.end();
	for(ComponentIterator it =m_components.begin();it != end;++it)
	{
		(*it)->update((float)diff);
		(*it)->draw((float)diff);
	}
}

}
}