#include "SceneManager.h"

GeometryEngine::SceneManager::SceneManager() : mpActiveScene(nullptr)
{
}

GeometryEngine::SceneManager::~SceneManager()
{
	for (auto it = mSceneList.begin(); it != mSceneList.end(); ++it)
	{
		delete((*it));
	}

	mSceneList.clear();
	mpActiveScene = nullptr;
}

GeometryEngine::GeometryScene * GeometryEngine::SceneManager::CreateScene()
{
	return new GeometryScene(this);
}

bool GeometryEngine::SceneManager::AddScene(GeometryScene * scene)
{
	for (auto it = mSceneList.begin(); it != mSceneList.end(); ++it)
	{
		if (scene == (*it))
		{
			return false;
		}
	}

	mSceneList.push_back(scene);
	return true;
}

bool GeometryEngine::SceneManager::RemoveScene(GeometryScene * scene)
{
	bool found = false;

	for (auto it = mSceneList.begin(); it != mSceneList.end(); ++it)
	{
		found = (scene == (*it));

		if (found)
		{
			if ((*it) == mpActiveScene)
			{
				mpActiveScene = nullptr;
			}

			delete ((*it));
			mSceneList.erase(it);
			return found;
		}
	}

	return found;
}

bool GeometryEngine::SceneManager::RemoveScene(unsigned int sceneId)
{
	if (mSceneList.size() > sceneId )
		return false;

	auto it = mSceneList.begin();
	std::advance(it, sceneId);

	if ((*it) == mpActiveScene)
	{
		mpActiveScene = nullptr;
	}

	delete ((*it));
	mSceneList.erase(it);
	return true;
	
}

bool GeometryEngine::SceneManager::SetActiveScene(GeometryScene * scene)
{
	bool found = false;

	for (auto it = mSceneList.begin(); it != mSceneList.end(); ++it)
	{
		found = (scene == (*it));

		if (found)
		{
			mpActiveScene = scene;
			break;
		}
	}

	return found;
}

bool GeometryEngine::SceneManager::SetActiveScene(unsigned int sceneID)
{
	if(mSceneList.size() > sceneID)
		return false;

	auto it = mSceneList.begin();
	std::advance(it, sceneID);
	mpActiveScene = (*it);

	return true;
}

void GeometryEngine::SceneManager::ClearScenes()
{
	for (auto it = mSceneList.begin(); it != mSceneList.end(); ++it)
	{
		delete((*it));
	}

	mSceneList.clear();
}
