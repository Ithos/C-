#pragma once

#ifndef GEOMETRYSPHERE_H
#define GEOMETRYSPHERE_H

#include "../GeometryItem.h"
#include <math.h>

namespace GeometryEngine
{
	class Sphere : public GeometryItem
	{
	public:
		Sphere(const Material& mat, float size = 1.0f, int verticalResolution = 6, int horizontalResolution = 12, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
		Sphere(const Sphere& const ref);
		virtual ~Sphere();
		virtual void Copy(const Sphere& ref);
		virtual GeometryItem* Clone() const override;
	protected:
		void initGeometry() override;
		float mSize;
		int mNumPsiStep;
		int mNumPhiStep;
		VertexData* mVertices;
		GLushort * mIndices;
	};
}
#endif