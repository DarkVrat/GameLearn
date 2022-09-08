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
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName);

	std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePatn);
	std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);

	std::shared_ptr<Renderer::Sprite> loadSprite(	const std::string& spriteName, 
													const std::string& textureName, 
													const std::string& shaderName, 
													const unsigned int spriteWidth,
													const unsigned int spriteHeight,
													const std::string& subTextureName = "Default");
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::AnimSprite> loadAnimSprite(	const std::string& animSpriteName,
														const std::string& textureName,
														const std::string& shaderName,
														const unsigned int spriteWidth,
														const unsigned int spriteHeight,
														const std::string& subTextureName = "Default");
	std::shared_ptr<Renderer::AnimSprite> getAnimSprite(const std::string& animSpriteName);

	std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(	std::string textureName,
															std::string texturePatn,
															std::vector<std::string> subTextures,
															const unsigned subWidth,
															const unsigned subHeigth);

private:
	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	SpriteMap m_sprite;

	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimSprite>> AnimSpriteMap;
	AnimSpriteMap m_animSprite;

	std::string m_path;
};