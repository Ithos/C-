#pragma once

#ifndef GEOMETRYCOLORMATERIAL_H
#define GEOMETRYCOLORMATERIAL_H

#include "Material.h"

namespace GeometryEngine
{
	class ColorMaterial : public Material
	{
	public:
		ColorMaterial(const QVector3D& emissive = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0, 1.0f), const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), float shininess = 10.0f, bool isLit = true, bool customLight = false);
		ColorMaterial(const ColorMaterial& mat);
		virtual ~ColorMaterial();
		const QVector3D& GetEmissive() const { return mEmissive; }
		void SetEmissive(const QVector3D& emissive) { mEmissive.setX(emissive.x()); mEmissive.setY(emissive.y()); mEmissive.setZ(emissive.z()); }
		virtual Material* Clone() const override;
		

	protected:
		QVector3D mEmissive;

		virtual void initShaders();

		virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryItem& parent) override;
		virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
		virtual void copy(const ColorMaterial& mat);
	};
}
#endif