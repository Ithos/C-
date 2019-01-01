#pragma once

#ifndef GEOMETRYITEM_H
#define GEOMETRYITEM_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector3D>
#include <list>

#include <ConfigurationManager.h>
#include <ShaderManager.h>
#include "WorldItem.h"

namespace GeometryEngine
{

	struct VertexData
	{
		QVector3D position;
		QVector3D color;
		QVector2D texCoord;
	};

	class GeometryItem : public WorldItem
	{
	public:
		GeometryItem(const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
		virtual ~GeometryItem();
		virtual void DrawItem(const QMatrix4x4& projection);
		virtual void Update(const QMatrix4x4& projectionMatrix) override { DrawItem(projectionMatrix); };
		

	protected:
		virtual void initGeometry() = 0;
		virtual void initShaders() = 0;
		virtual void initProgram();
		virtual void initItem();
		virtual void drawGeometry() = 0;
		virtual void setProgramParameters(const QMatrix4x4& projection) = 0;
		

		QOpenGLBuffer* mpArrayBuf;
		QOpenGLBuffer* mpIndexBuf;
		QOpenGLShaderProgram* mpProgram;
		Configuration::ConfigurationManager* mpConfInstance;
		std::list<std::string> mVertexShaderKeyList;
		std::list<std::string> mFragmentShaderKeyList;
		ShaderFiles::ShaderManager* mpShaderManager;
		
	
	};

}

#endif