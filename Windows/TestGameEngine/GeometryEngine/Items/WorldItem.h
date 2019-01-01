#pragma once

#ifndef WORLDITEM_H
#define WORLDITEM_H

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <unordered_set>

namespace GeometryEngine
{

	class WorldItem : protected QOpenGLFunctions
	{
	public:
		WorldItem(const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), 
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

		virtual ~WorldItem();

		bool AddChild(WorldItem* child);
		bool SetParent( WorldItem* parent);
		bool RemoveChild(WorldItem* child);
		bool RemoveParent();
		bool FindChild(WorldItem* child) const;
		WorldItem* GetParent() const { return mpParent; }
		const QVector3D& GetPosition() const;
		const QVector3D& GetRotation() const { return mRotation; }
		const QVector3D& GetScale() const { return mScale; }
		void Move(const QVector3D& vector, bool updateChildren = true);
		void Rotate(const QVector3D& rot, bool updateChildren = true);
		void Scale(const QVector3D& scale, bool updateChildren = true);
		void SetPosition(const QVector3D& vector, bool delayUpdate = false);
		void SetRotation(const QVector3D& rot, bool delayUpdate = false);
		void SetScale(const QVector3D& scale, bool delayUpdate = false);
		virtual void CalculateModelMatrix(bool calculateChildren = false);
		virtual void UpdateModelMatrix(bool updateChildren = false);
		virtual void Update(const QMatrix4x4& projectionMatrix) {};

	protected:

		QVector3D mPosition;
		QVector3D mRotation;
		QVector3D mScale;
		std::unordered_set< WorldItem* > mpChildren;
		WorldItem* mpParent; // DONT DELETE
		QMatrix4x4 mModelMatrix;

	private:
		void init(const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent);
	};
}

#endif