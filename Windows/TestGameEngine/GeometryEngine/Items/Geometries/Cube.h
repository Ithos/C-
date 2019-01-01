#pragma once


#ifndef GEOMETRYCUBE_H
#define GEOMETRYCUBE_H

#include "../GeometryItem.h"

namespace GeometryEngine
{
	class Cube : public GeometryItem
	{
	public:
		Cube(const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
		virtual ~Cube();
	protected:
		void initGeometry() override;
		void initShaders() override;
		void drawGeometry() override;
		void setProgramParameters(const QMatrix4x4& projection) override;


	};
}

#endif