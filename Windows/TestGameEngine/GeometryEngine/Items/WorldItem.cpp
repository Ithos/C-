#include "WorldItem.h"

GeometryEngine::WorldItem::WorldItem(const QVector3D& pos, const QVector3D & rot, const QVector3D & scale, WorldItem* parent) : mpParent(parent)
{
	init(pos, rot, scale, parent);
}


GeometryEngine::WorldItem::~WorldItem()
{
	for (auto iter = mpChildren.begin(); iter != mpChildren.end(); iter++)
	{
		(*iter)->RemoveParent();
	}

	this->RemoveParent();
}

bool GeometryEngine::WorldItem::AddChild( WorldItem* child)
{
	if (child->GetParent() == nullptr && 
		mpChildren.find(child) == mpChildren.end()
		&& child->SetParent(this))
	{
		mpChildren.insert(child);
		return true;
	}

	return false;
}

bool GeometryEngine::WorldItem::SetParent( WorldItem* parent)
{
	if (mpParent == nullptr && mpChildren.find(parent) ==mpChildren.end())
	{
		mpParent = parent;
		return true;
	}

	return false;
}

bool GeometryEngine::WorldItem::RemoveChild( WorldItem* child)
{
	if (child->GetParent() == this &&
		mpChildren.find(child) != mpChildren.end())
	{
		mpChildren.erase(child);
		child->RemoveParent();
		return true;
	}

	return false;
}

bool GeometryEngine::WorldItem::RemoveParent()
{
	if (mpParent != nullptr)
	{
		mpParent->RemoveChild(this);
		mpParent = nullptr;
		return true;
	}

	return false;
}

bool GeometryEngine::WorldItem::FindChild(WorldItem* child) const
{
	return mpChildren.find(child) != mpChildren.end();
}

const QVector3D & GeometryEngine::WorldItem::GetPosition() const
{
	if (mpParent == nullptr)
	{
		return mPosition;
	}
	else
	{
		return mpParent->GetPosition() + mPosition;
	}
}

void GeometryEngine::WorldItem::Move(const QVector3D & vector, bool updateChildren)
{
	mPosition += vector;

	UpdateModelMatrix(updateChildren);
}

void GeometryEngine::WorldItem::Rotate(const QVector3D & rot, bool updateChildren)
{
	mRotation += rot;

	UpdateModelMatrix();

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->Rotate(rot);
		}
	}
}

void GeometryEngine::WorldItem::Scale(const QVector3D & scale, bool updateChildren)
{
	mScale *= scale;

	UpdateModelMatrix();

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->Scale(scale);
		}
	}
}

void GeometryEngine::WorldItem::SetPosition(const QVector3D & pos, bool delayUpdate)
{
	mPosition.setX(pos.x());
	mPosition.setY(pos.y());
	mPosition.setZ(pos.z());

	if(!delayUpdate)
	{
		CalculateModelMatrix();
	}
}

void GeometryEngine::WorldItem::SetRotation(const QVector3D & rot, bool delayUpdate)
{
	mRotation.setX(rot.x());
	mRotation.setY(rot.y());
	mRotation.setZ(rot.z());

	if (!delayUpdate)
	{
		CalculateModelMatrix();
	}
}

void GeometryEngine::WorldItem::SetScale(const QVector3D & scale, bool delayUpdate)
{
	mScale.setX(scale.x());
	mScale.setY(scale.y());
	mScale.setZ(scale.z());

	if (!delayUpdate)
	{
		CalculateModelMatrix();
	}
}

void GeometryEngine::WorldItem::init(const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent)
{
	initializeOpenGLFunctions();

	SetPosition(pos, true);
	SetRotation(rot, true);
	SetScale(scale, true);

	CalculateModelMatrix();

	SetParent(parent);
}

void GeometryEngine::WorldItem::CalculateModelMatrix(bool calculateChildren)
{
	mModelMatrix.setToIdentity();
	UpdateModelMatrix();

	if (calculateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->CalculateModelMatrix(calculateChildren);
		}
	}
}

void GeometryEngine::WorldItem::UpdateModelMatrix(bool updateChildren)
{
	mModelMatrix.translate(GetPosition());
	mModelMatrix.rotate(QQuaternion(mRotation.x(), QVector3D(1.0f, 0.0f, 0.0f)));
	mModelMatrix.rotate(QQuaternion(mRotation.y(), QVector3D(0.0f, 1.0f, 0.0f)));
	mModelMatrix.rotate(QQuaternion(mRotation.z(), QVector3D(0.0f, 0.0f, 1.0f)));
	mModelMatrix.scale(mScale);

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->UpdateModelMatrix(updateChildren);
		}
	}
}
