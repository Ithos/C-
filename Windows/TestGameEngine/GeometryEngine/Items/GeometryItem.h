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
#include "Materials\Material.h"

namespace GeometryEngine
{
	class Material;

	class GeometryItem : public WorldItem
	{
	public:
		GeometryItem(const Material& mat, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
		GeometryItem(const GeometryItem& ref);
		virtual ~GeometryItem();
		virtual void DrawItem(const QMatrix4x4& projection, const QMatrix4x4& view);
		virtual void Update(const QMatrix4x4& projection, const QMatrix4x4& view) override { DrawItem(projection, view); }
		virtual Material* GetMaterialPtr() { return mpMaterial; }
		virtual QOpenGLBuffer* GetArrayBuffer() { return mpArrayBuf; }
		virtual QOpenGLBuffer* GetIndexBuffer() { return mpIndexBuf; }
		virtual unsigned int GetVertexNumber() { return mTotalVertexNumber; }
		virtual unsigned int GetIndexNumber() { return mTotalIndexNumber; }
		virtual void SetMaterial(Material* mat);
		virtual void Copy(const GeometryItem& ref);
		virtual GeometryItem* Clone() const = 0;

	protected:
		virtual void initGeometry() = 0;
		virtual void initItem();
		

		QOpenGLBuffer* mpArrayBuf;
		QOpenGLBuffer* mpIndexBuf;
		Material* mpMaterial;
		Configuration::ConfigurationManager* mpConfInstance;
		std::string mVertexShaderKey;
		std::string mFragmentShaderKey;
		ShaderFiles::ShaderManager* mpShaderManager;
		unsigned int mTotalVertexNumber;
		unsigned int mTotalIndexNumber;
		
	
	};

}

#endif