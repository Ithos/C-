#include "GeometryEngine.h"

GeometryEngine::GeometryEngine* GeometryEngine::GeometryEngine::mpInstance = nullptr;

GeometryEngine::GeometryEngine * GeometryEngine::GeometryEngine::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new GeometryEngine();
	}

	return mpInstance;
}

void GeometryEngine::GeometryEngine::Release()
{
	if (mpInstance != nullptr)
	{
		mpInstance->close();
		delete mpInstance;
	}
}

GeometryEngine::GeometryEngine::GeometryEngine()
{
	init();
}

GeometryEngine::GeometryEngine::~GeometryEngine()
{
	delete mpSceneManager;
}

void GeometryEngine::GeometryEngine::close()
{
}

void GeometryEngine::GeometryEngine::init()
{
	mpSceneManager = new SceneManager();
}
