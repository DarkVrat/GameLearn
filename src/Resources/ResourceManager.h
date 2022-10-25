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
	//метод получающий argv[0] в main дл€ получени€ пути к игре
	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllRes();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//функции создани€ и получени€ указателей на ресурсы
	static std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName);

	static std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePatn);
	static std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);

	static std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "Default");
	static std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::AnimSprite> loadAnimSprite(const std::string& animSpriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "Default");
	static std::shared_ptr<Renderer::AnimSprite> getAnimSprite(const std::string& animSpriteName);

	//создание текстурного атласа
	static std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,std::string texturePatn,std::vector<std::string> subTextures,const unsigned subWidth,const unsigned subHeigth);

private:
	//функци€ получени€ данных из файла
	static std::string getFileString(const std::string& relativeFilePath) ;

	//ќбъ€вление типов Map дл€ хранени€ Shared_ptr указателей наших типов.
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimSprite>> AnimSpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	//’ранение всех данных загруженных в программу
	static SpriteMap m_sprite;
	static TexturesMap m_textures;
	static AnimSpriteMap m_animSprite;
	static ShaderProgramsMap m_shaderPrograms;
	//путь к папке с игрой
	static std::string m_path;
};