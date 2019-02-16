#include "Sphere.h"

GeometryEngine::Sphere::Sphere(const Material & mat, float size, int verticalResolution , int horizontalResolution,  const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	GeometryItem(mat, pos, rot, scale, parent), mSize(size), mNumPsiStep(verticalResolution), mNumPhiStep(horizontalResolution), mVertices(nullptr), mIndices(nullptr)
{
	initItem();
}

GeometryEngine::Sphere::Sphere(const Sphere & const ref) : GeometryItem((const GeometryItem&) ref)
{
	this->Copy(ref);
}

GeometryEngine::Sphere::~Sphere()
{
	if (mVertices != nullptr)
	{
		delete[] mVertices;
		mVertices = nullptr;
	}

	if (mIndices != nullptr)
	{
		delete[] mIndices;
		mIndices = nullptr;
	}
}

void GeometryEngine::Sphere::Copy(const Sphere & ref)
{
	this->GeometryItem::Copy(ref);

	this->mSize = ref.mSize;
	this->mNumPsiStep = ref.mNumPsiStep;
	this->mNumPhiStep = ref.mNumPhiStep;
	this->mVertices = ref.mVertices;
	this->mIndices = ref.mIndices;

	initItem();
}

GeometryEngine::GeometryItem * GeometryEngine::Sphere::Clone() const
{
	return new Sphere((*this));
}

void GeometryEngine::Sphere::initGeometry()
{
	int totalVertexNum = (mNumPsiStep + 1) * (mNumPhiStep + 1);
	
	if (mVertices == nullptr)
	{
		mVertices = new VertexData[totalVertexNum];

		float psiStep = 3.14159265f / mNumPsiStep;
		float phiStep = (2 * 3.14159265f) / mNumPhiStep;

		float psiAngle = 0;

		for (int i = 0; i <= mNumPsiStep; ++i)
		{
			float phiAngle = 0.0f;
			for (int j = 0; j <= mNumPhiStep; ++j)
			{
				mVertices[i * (mNumPhiStep + 1) + j] =
					VertexData{
						QVector3D(std::sin(psiAngle) * std::cos(phiAngle) * mSize, std::sin(psiAngle) * std::sin(phiAngle) * mSize, std::cos(psiAngle) * mSize),
						QVector3D(std::sin(psiAngle) * std::cos(phiAngle), std::sin(psiAngle) * std::sin(phiAngle), std::cos(psiAngle)),//QVector3D(0.5f, 0.5f, 0.5f),
						QVector2D((float)j / (mNumPhiStep), (float)i / mNumPsiStep),
						QVector3D(std::sin(psiAngle) * std::cos(phiAngle), std::sin(psiAngle) * std::sin(phiAngle), std::cos(psiAngle))
						};

				phiAngle += phiStep;
			}

			psiAngle += psiStep;
		}
	}

	int totalIndicesNum = mNumPsiStep * mNumPhiStep * 6 - 2;

	if (mIndices == nullptr)
	{
		mIndices = new GLushort[totalIndicesNum];

		int arrayIndex = 0;
		for (int i = 0; i < mNumPsiStep; ++i)
		{
			for (int j = 0; j < mNumPhiStep; ++j)
			{

				mIndices[arrayIndex++] = i * (mNumPhiStep + 1) + j;
				mIndices[arrayIndex++] = (i + 1) * (mNumPhiStep + 1) + j;
				mIndices[arrayIndex++] = i * (mNumPhiStep + 1) + j + 1;
				mIndices[arrayIndex++] = (i + 1) * (mNumPhiStep + 1) + j + 1;


				if ( i != mNumPsiStep - 1 || j != mNumPhiStep - 1)
				{
					mIndices[arrayIndex++] = (i + 1) * (mNumPhiStep + 1) + j + 1;
					if (j != mNumPhiStep - 1)
					{
						mIndices[arrayIndex++] = i * (mNumPhiStep + 1) + j + 1;
					}
					else
					{
						mIndices[arrayIndex++] = (i + 1) * (mNumPhiStep + 1) + j + 1 - mNumPhiStep;
					}
				}
			}
		}
	}

	// Transfer vertex data to VBO 0
	mpArrayBuf->bind();
	mpArrayBuf->allocate(mVertices, totalVertexNum * sizeof(VertexData));

	// Transfer index data to VBO 1
	mpIndexBuf->bind();
	mpIndexBuf->allocate(mIndices, totalIndicesNum * sizeof(GLushort));

	mTotalVertexNumber = totalIndicesNum;
	mTotalIndexNumber = totalIndicesNum;
}
