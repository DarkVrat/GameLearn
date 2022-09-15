#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimSprite.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

//����������� ���������� argv[0] � main ��� ��������� ���� � ����
ResourceManager::ResourceManager(const std::string& executablePath) {
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

//������� ��������� ������ �� �����
std::string ResourceManager::getFileString(const std::string& relativeFilePath) const {
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "Failed to open: " << relativeFilePath << std::endl;
		return std::string{};
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}


 //-------------------------------Shader------------------------------------//
//�������� ������� � ������
std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexString = getFileString(vertexPath);//������ �� ����������� �������
	if (vertexString.empty()) {//�������� �������
		std::cerr << "No vertex" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);//������ �� ������������ �������
	if (fragmentString.empty()) {//�������� �������
		std::cerr << "No fragment" << std::endl;
		return nullptr;
	}

	//�������� � ���� ������� ������� � ��������� � MAP
	std::shared_ptr<Renderer::ShaderProgram>&newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled()) return newShader;//�������� � ������� ������� ��� �������� ����������

	std::cerr << "Can't load shader" << std::endl;
	return nullptr;
}

//�������� ������� �� ������
std::shared_ptr<Renderer::ShaderProgram>  ResourceManager::getShader(const std::string& shaderName) {
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);//�����
	if (it != m_shaderPrograms.end()) return it->second;//�������� ������� � ������� ���� ����
	std::cerr << "Cant find the shader " << shaderName << std::endl;
	return nullptr;
}
//-------------------------------Shader------------------------------------//



 //-------------------------------Texture-----------------------------------//
//�������� �������� � ������
std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePatn) {
	int channels = 0;
	int widht = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);//��� �������� �������� � ���������� �������������, ��� false ����������� ����� ����������
	unsigned char* pixels=stbi_load(std::string(m_path + "/" + texturePatn).c_str(), &widht, &height, &channels, 0);//�������� ��������, � ��� �� ���������� ���������
	if (!pixels) { //�������� ����������
		std::cerr << "ERROR TEXTUR LOAD" << textureName <<std::endl;
		return nullptr;
	}

	//�������� �������� � ���� � ������� ������
	std::shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(widht, height,pixels, channels,GL_NEAREST,GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);//������������ ������
	return newTexture;//�������� ��������
}

//�������� �������� �� ������
std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName) {
	TexturesMap::const_iterator it = m_textures.find(textureName);//�����
	if (it != m_textures.end()) return it->second;//���������
	std::cerr << "Cant find the textureName " << textureName << std::endl;
	return nullptr;
}
//-------------------------------Texture-----------------------------------//



 //-------------------------------Sprite------------------------------------//
//�������� ������� � ������
std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight,const std::string& subTextureName){
	//����� � �������� ��������
	auto pTexture = getTexture(textureName);
	if (!pTexture) std::cerr << "Cant find the textureName " << textureName << " for sprite " << spriteName << std::endl;
	
	//����� � �������� �������
	auto pShader = getShader(shaderName);
	if (!pShader) std::cerr << "Cant find the shaderName " << shaderName << " for sprite " << spriteName << std::endl;
	
	//�������� ������� � ������� ��������� �� ������
	std::shared_ptr<Renderer::Sprite> newSprite = m_sprite.emplace(textureName,std::make_shared<Renderer::Sprite>(pTexture,subTextureName,pShader,glm::vec2(0.f, 0.f),glm::vec2(spriteWidth, spriteHeight),0.f)).first->second;
	return newSprite;
}

//�������� ������� �� ������
std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName){
	SpriteMap::const_iterator it = m_sprite.find(spriteName);//�����
	if (it != m_sprite.end()) return it->second;//���������
	std::cerr << "Cant find the spriteName " << spriteName << std::endl;
	return nullptr;
}
//-------------------------------Sprite------------------------------------//



 //-------------------------------AnimSprite--------------------------------//
//�������� ������������� ������� � ������
std::shared_ptr<Renderer::AnimSprite> ResourceManager::loadAnimSprite(const std::string& animSpriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName){
	//�������� ��������
	auto pTexture = getTexture(textureName);
	if (!pTexture) std::cerr << "Cant find the textureName " << textureName << " for sprite " << animSpriteName << std::endl;
	
	//�������� �������
	auto pShader = getShader(shaderName);
	if (!pShader) std::cerr << "Cant find the shaderName " << shaderName << " for sprite " << animSpriteName << std::endl;
	
	//�������� � �������� ������������� �������
	std::shared_ptr<Renderer::AnimSprite> newSprite = m_animSprite.emplace(textureName,std::make_shared<Renderer::AnimSprite>(	pTexture,subTextureName,pShader,glm::vec2(0.f, 0.f),glm::vec2(spriteWidth, spriteHeight),0.f)).first->second;
	return newSprite;
}

//�������� ������������� ������� �� ������
std::shared_ptr<Renderer::AnimSprite> ResourceManager::getAnimSprite(const std::string& animSpriteName){
	auto it = m_animSprite.find(animSpriteName);
	if (it != m_animSprite.end()) return it->second;
	std::cerr << "Cant find the spriteName " << animSpriteName << std::endl;
	return nullptr;
}
//-------------------------------AnimSprite--------------------------------//


//�������� ����������� ������, ���������� ����� �� ������������� � ��� ���������� ���������
std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTextureAtlas(std::string textureName,std::string texturePatn,std::vector<std::string> subTextures,const unsigned subWidth,const unsigned subHeigth) {
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePatn));//�������� ��������
	if (pTexture) {
		const unsigned int textureWidth = pTexture->getWidth();	//��������� ������
		const unsigned int textureHeight = pTexture->getHeight(); //��������� ������
		unsigned int currentTexOffsetX = 0;//��������� X ����������
		unsigned int currentTexOffsetY = textureHeight;//��������� Y ����������

		for (auto& currentSubTexName : subTextures) {//������� �� ������� �������� ��� ����������
			//�������� ����� ������ � ������ ������� ���������� ����������
			glm::vec2 leftBottomUV(	static_cast<float>(currentTexOffsetX)/textureWidth,				static_cast<float>(currentTexOffsetY-subHeigth)/textureHeight);
			glm::vec2 rightTopUV(	static_cast<float>(currentTexOffsetX+ subHeigth)/textureWidth,	static_cast<float>(currentTexOffsetY)/textureHeight);
			//�������� �������� � ����������� 
			pTexture->addSubTexture(std::move(currentSubTexName), leftBottomUV, rightTopUV);
			currentTexOffsetX += subWidth; //������� X
			if (currentTexOffsetX >= textureWidth) {//��� ������� �� ���� X ����������� ������������ � 0 � ��������� Y
				currentTexOffsetX = 0;
				currentTexOffsetY -= subHeigth;
			}
		}
	}
	return pTexture;//������� �������
}