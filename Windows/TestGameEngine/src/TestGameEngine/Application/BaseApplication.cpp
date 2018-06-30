#include "BaseApplication.h"

Application::CBaseApplication::CBaseApplication()
{
}

Application::CBaseApplication::~CBaseApplication()
{
}

bool Application::CBaseApplication::init()
{
	return false;
}

void Application::CBaseApplication::release()
{
}

void Application::CBaseApplication::releaseAllStates()
{
}

bool Application::CBaseApplication::run()
{
	return false;
}

bool Application::CBaseApplication::popState()
{
	return false;
}

bool Application::CBaseApplication::resetToMenuState(const std::string & initialGameState)
{
	return false;
}

void Application::CBaseApplication::changeState()
{
}
