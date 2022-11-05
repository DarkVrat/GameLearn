#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <rapidjson/document.h>
#include "../Renderer/Sprite.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/StateAnimation.h"

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class StateAnimation;
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

	static std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName = "Default");
	static std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::StateAnimation> loadStateAnimation(const std::string& spriteName, std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> frames, std::vector<std::string> sources, std::string nextState, bool uninterrupted);
	static std::shared_ptr<Renderer::StateAnimation> getStateAnimation(const std::string& spriteName);

	//�������� ����������� ������
	static std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,std::string texturePatn,std::vector<std::string> subTextures,const unsigned subWidth,const unsigned subHeigth);

	static rapidjson::Document loadJSONDoc(const std::string& JSONPath);
	static bool loadJSONResurces(const std::string& JSONPath);
	static bool checkJSONResurces(const std::string& JSONPath);

private:
	//������� ��������� ������ �� �����
	static std::string getFileString(const std::string& relativeFilePath) ;

	//���������� ����� Map ��� �������� Shared_ptr ���������� ����� �����.
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::StateAnimation>> StateAnimationMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	//�������� ���� ������ ����������� � ���������
	static SpriteMap m_sprite;
	static StateAnimationMap m_stateAnimation;
	static TexturesMap m_textures;
	static ShaderProgramsMap m_shaderPrograms;
	//���� � ����� � �����
	static std::string m_path;
};