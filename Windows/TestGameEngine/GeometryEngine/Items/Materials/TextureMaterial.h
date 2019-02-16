#pragma once

#ifndef GEOMETRYTEXMATERIAL_H
#define GEOMETRYTEXMATERIAL_H

#include <qopengltexture.h>

#include <list>
#include <Textures.h>

#include "Material.h"

namespace GeometryEngine
{
	struct TextureConstant
	{
		static const std::string ERROR_TEXTURE;
		static const std::string TEST_BLUE_CHIP_TEXTURE;
		static const std::string TEST_DOWN_TEXTURE;
		static const std::string TEST_UP_TEXTURE;
		static const std::string TEST_FORWARD_TEXTURE;
		static const std::string TEST_BACK_TEXTURE;
		static const std::string TEST_LEFT_TEXTURE;
		static const std::string TEST_RIGHT_TEXTURE;
	};

	class TextureParameters
	{
	public:
		TextureParameters(const std::string& texDir, int vertex, bool getFromConf = false) : Texture(nullptr), mpConfInstance(nullptr), mpTexDirManager(nullptr)
		{
			initManagers();
			TextureDir = getFromConf ? mpTexDirManager->GetTextureDir(texDir) : texDir;
			VertexNumber = vertex;
		}

		TextureParameters(const TextureParameters& ori)
		{
			initManagers();
			TextureDir = ori.TextureDir;
			VertexNumber = ori.VertexNumber;
			ori.Texture == nullptr ? this->Texture = nullptr : Build();
		}

		virtual ~TextureParameters()
		{
			delete Texture;
		}

	private:
		std::string TextureDir;
		int VertexNumber;
		QOpenGLTexture* Texture;
		Configuration::ConfigurationManager* mpConfInstance;
		TexturesFiles::Textures * mpTexDirManager;

		friend class TextureMaterial;

		void initManagers()
		{
			mpConfInstance = Configuration::ConfigurationManager::GetInstance();
			mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
		}

		void Build();
		
	};

	class TextureMaterial : public Material
	{
	public:
		TextureMaterial(const std::string& texDir, const QVector3D& ambient = QVector3D(1.0f, 1.0, 1.0f), const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), float shininess = 10.0f, bool isLit = true, bool customLight = false);

		TextureMaterial(const std::list< TextureParameters* > & textureDirs, const QVector3D& ambient = QVector3D(1.0f, 1.0, 1.0f), const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), float shininess = 10.0f, bool isLit = true, bool customLight = false);

		TextureMaterial(const TextureMaterial& mat);

		virtual Material* Clone() const override;

		virtual ~TextureMaterial();
		virtual void AddTexture(const TextureParameters& texDir);
		virtual void AddTextures(std::list< TextureParameters*> textureDirs);
		virtual void InsertTexture(const TextureParameters& texDir, int index);
		virtual void DeleteTexture(int index);
		virtual void DeleteAllTextures();
		
	protected:
		static const int TEXTURE_UNIT = 0;

		std::list<TextureParameters *> mTexturesList;

		TexturesFiles::Textures * mpTexDirManager;

		virtual void initTextures(const std::list<TextureParameters* > & textureDirs);
		virtual void initMaterial(const std::list<TextureParameters* >& textureDirs);
		virtual void initShaders() override;
		virtual TextureParameters* buildTexture(const TextureParameters& texDir);

		virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryItem& parent) override;
		virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
		virtual void copy(const TextureMaterial& mat);
	};
}

#endif