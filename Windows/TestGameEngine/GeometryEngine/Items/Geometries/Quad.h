#pragma once

#ifndef GEOMETRYQUAD_H
#define GEOMETRYQUAD_H

#include "../GeometryItem.h"

namespace GeometryEngine
{
	class Quad : public GeometryItem
	{
	public:
		Quad(const Material& mat, float width, float height, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
		Quad(const Quad& ref);
		virtual ~Quad() {};
		virtual void Copy(const Quad& ref);
		virtual GeometryItem* Clone() const override;

	protected:
		float mWidth;
		float mHeight;
		void initGeometry() override;
	};
}
#endif