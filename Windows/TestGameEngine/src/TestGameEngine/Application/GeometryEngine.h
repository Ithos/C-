#pragma once

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector3D>

struct VertexData
{
	QVector3D position;
	QVector3D color;
	QVector2D texCoord;
};

class GeometryEngine : protected QOpenGLFunctions
{
public:
	GeometryEngine();
	virtual ~GeometryEngine();

	void drawCubeGeometry(QOpenGLShaderProgram *program);

private:
	void initCubeGeometry();

	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
};

#endif // GEOMETRYENGINE_H
