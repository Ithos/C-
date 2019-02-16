#pragma once

#ifndef GEOMETRYSCENE_H
#define GEOMETRYSCENE_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <unordered_set>
#include "Items/WorldItem.h"
#include "Items\GeometryItem.h"
#include "Items/GraphicItems/Camera.h"
#include "Items\GraphicItems\Light.h"
#include "SceneManager.h"


namespace GeometryEngine
{
	class SceneManager;
	class WorldItem;

	class GeometryScene: protected QOpenGLExtraFunctions
	{
	public:
		GeometryScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
		virtual ~GeometryScene();

		virtual void ResizeScene(int w, int h, int formerW, int formerH);
		virtual void Draw();
		virtual bool AddItem(GeometryItem* item);
		virtual bool RemoveItem(GeometryItem* item);
		virtual bool AddCamera(Camera* item);
		virtual bool RemoveCamera(Camera* item);
		virtual bool AddLight(Light* item);
		virtual bool RemoveLight(Light* item);
		virtual void InitializeGL();

	protected:
		std::unordered_set< GeometryItem* > mItemList;// DO NOT DELETE
		QMatrix4x4 mProjection;
		SceneManager* mpParentManager; // DO NOT DELETE
		QVector4D mClearColor;
		std::unordered_set< Camera* > mCameras;// DO NOT DELETE
		std::unordered_set< Light* > mLights;// DO NOT DELETE
		

		virtual void DrawItem(Camera* cam, GeometryItem* item);
		virtual void ApplyLight(Camera* cam);
		virtual void GeometryPass();
		virtual void PrepareGeomPass();
		virtual void DisableDepth();
		virtual void PrepareLightPass();
		virtual void LightPass();

		////////////////////////////////////////////////////////////////
		// GEOMETRY BUFFER DEBUG
		///////////////////////////////////////////////////////////////

		void gBufferDebug(Camera* cam);
	};
}

#endif