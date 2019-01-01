#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <list>
#include "GeometryScene.h"

namespace GeometryEngine
{
	class GeometryScene;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		GeometryScene* CreateScene();
		bool AddScene(GeometryScene* scene);
		bool RemoveScene(GeometryScene* scene);
		bool RemoveScene(unsigned int sceneId);
		int GetSceneCount() { return mSceneList.size(); }
		bool SetActiveScene(GeometryScene* scene);
		bool SetActiveScene(unsigned int sceneID);
		GeometryScene* GetActiveScene() { return mpActiveScene; }
		void ClearScenes();

	protected:
		std::list<GeometryScene*> mSceneList;
		GeometryScene* mpActiveScene;
	};
}

#endif