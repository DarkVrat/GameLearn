#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimSprite;
}

class ResourceManager {
public:
	//����� ���������� argv[0] � main ��� ��������� ���� � ����
	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllRes();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//������� �������� � ��������� ���������� �� �������
	static std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName);

	static std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePatn);
	static std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);

	static std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "Default");
	static std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::AnimSprite> loadAnimSprite(const std::string& animSpriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "Default");
	static std::shared_ptr<Renderer::AnimSprite> getAnimSprite(const std::string& animSpriteName);

	//�������� ����������� ������
	static std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,std::string texturePatn,std::vector<std::string> subTextures,const unsigned subWidth,const unsigned subHeigth);

private:
	//������� ��������� ������ �� �����
	static std::string getFileString(const std::string& relativeFilePath) ;

	//���������� ����� Map ��� �������� Shared_ptr ���������� ����� �����.
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimSprite>> AnimSpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	//�������� ���� ������ ����������� � ���������
	static SpriteMap m_sprite;
	static TexturesMap m_textures;
	static AnimSpriteMap m_animSprite;
	static ShaderProgramsMap m_shaderPrograms;
	//���� � ����� � �����
	static std::string m_path;
};