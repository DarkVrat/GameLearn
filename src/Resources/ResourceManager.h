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
	~ResourceManager() = default;

	//Конструктор получающий argv[0] в main для получения пути к игре
	ResourceManager(const std::string& executablePath);

	ResourceManager(ResourceManager&&) = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//функции создания и получения указателей на ресурсы
	std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName);

	std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePatn);
	std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);

	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "Default");
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::AnimSprite> loadAnimSprite(const std::string& animSpriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "Default");
	std::shared_ptr<Renderer::AnimSprite> getAnimSprite(const std::string& animSpriteName);

	//создание текстурного атласа
	std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,std::string texturePatn,std::vector<std::string> subTextures,const unsigned subWidth,const unsigned subHeigth);

private:
	//функция получения данных из файла
	std::string getFileString(const std::string& relativeFilePath) const;

	//Объявление типов Map для хранения Shared_ptr указателей наших типов.
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimSprite>> AnimSpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	//Хранение всех данных загруженных в программу
	SpriteMap m_sprite;
	TexturesMap m_textures;
	AnimSpriteMap m_animSprite;
	ShaderProgramsMap m_shaderPrograms;
	//путь к папке с игрой
	std::string m_path;
};