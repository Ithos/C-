#pragma once

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include "SceneManager.h"


/* QTDIR se define en vcxproj.user, hay que definir la carpeta correcta para configuración*/
namespace GeometryEngine
{
	class GeometryEngine
	{
	public:
		static GeometryEngine* GetInstance();
		static void Release();
		SceneManager* GetSceneManager() { return mpSceneManager; }

	protected:
		GeometryEngine();
		virtual ~GeometryEngine();
		void close();
		void init();

		static GeometryEngine* mpInstance;
		SceneManager* mpSceneManager;
	};
}

#endif // GEOMETRYENGINE_H
