#pragma once

#ifndef GEOMETRYSCENE_H
#define GEOMETRYSCENE_H

#include <QOpenGLFunctions>
#include <unordered_set>
#include "Items/WorldItem.h"
#include "Items/GraphicItems/Camera.h"
#include "SceneManager.h"

namespace GeometryEngine
{
	class SceneManager;
	class WorldItem;

	class GeometryScene: protected QOpenGLFunctions
	{
	public:
		GeometryScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
		virtual ~GeometryScene();

		virtual void ResizeScene(int w, int h, int formerW, int formerH);
		virtual void Draw();
		virtual bool AddItem(WorldItem* item);
		virtual bool RemoveItem(WorldItem* item);
		virtual bool AddCamera(Camera* item);
		virtual bool RemoveCamera(Camera* item);
		virtual void InitializeGL();

	protected:
		std::unordered_set< WorldItem* > mItemList;// DO NOT DELETE
		QMatrix4x4 mProjection;
		SceneManager* mpParentManager; // DO NOT DELETE
		QVector4D mClearColor;
		std::unordered_set< Camera* > mCameras;// DO NOT DELETE
		/// TODO -- main Camera ?? ///

		/// TODO -- move to the camera -- ///
		GLdouble mFoView;
		GLdouble mZNear;
		GLdouble mZFar;
	};
}

#endif