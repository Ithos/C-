#pragma once


#ifndef GEOMETRYCUBE_H
#define GEOMETRYCUBE_H

#include "../GeometryItem.h"

namespace GeometryEngine
{
	class Cube : public GeometryItem
	{
	public:
		Cube( const Material& mat, float size = 1.0f, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
		Cube(const Cube& ref);
		virtual ~Cube();
		virtual void Copy(const Cube& ref);
		virtual GeometryItem* Clone() const override;
	protected:
		void initGeometry() override;

		float mSize;


	};
}

#endif