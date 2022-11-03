#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <rapidjson/error/en.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::SpriteMap ResourceManager::m_sprite;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::StateAnimationMap ResourceManager::m_stateAnimation;
ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;

std::string ResourceManager::m_path;

//����� ���������� argv[0] � main ��� ��������� ���� � ����
void ResourceManager::setExecutablePath(const std::string& executablePath){
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

void ResourceManager::unloadAllRes(){
	m_shaderPrograms.clear();
	m_sprite.clear();
	m_textures.clear();
}

//������� ��������� ������ �� �����
std::string ResourceManager::getFileString(const std::string& relativeFilePath)  {
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
std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName,const std::string& subTextureName){
	//����� � �������� ��������
	auto pTexture = getTexture(textureName);
	if (!pTexture) std::cerr << "Cant find the textureName " << textureName << " for sprite " << spriteName << std::endl;
	
	//����� � �������� �������
	auto pShader = getShader(shaderName);
	if (!pShader) std::cerr << "Cant find the shaderName " << shaderName << " for sprite " << spriteName << std::endl;
	
	//�������� ������� � ������� ��������� �� ������
	std::shared_ptr<Renderer::Sprite> newSprite = m_sprite.emplace(spriteName,std::make_shared<Renderer::Sprite>(pTexture,subTextureName,pShader)).first->second;
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


//-------------------------------StateAnimation------------------------------------//
std::shared_ptr<Renderer::StateAnimation> ResourceManager::loadStateAnimation(const std::string& stateName, std::vector<std::pair<std::string, uint64_t>> frames, std::vector<std::string> sources, bool uninterrupted) {
	std::shared_ptr<Renderer::StateAnimation> newStateAnimation = m_stateAnimation.emplace(stateName, std::make_shared<Renderer::StateAnimation>(frames, sources, uninterrupted)).first->second;
	return nullptr;
}

std::shared_ptr<Renderer::StateAnimation> ResourceManager::getStateAnimation(const std::string& stateName) {
	StateAnimationMap::const_iterator it = m_stateAnimation.find(stateName);
	if (it != m_stateAnimation.end()) return it->second;
	std::cerr << "Cant find the spriteName " << stateName << std::endl;
	return nullptr;
}
//-------------------------------StateAnimation------------------------------------//



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
			glm::vec2 leftBottomUV(static_cast<float>(currentTexOffsetX+0.01f)/textureWidth,static_cast<float>(currentTexOffsetY-subHeigth + 0.01f)/textureHeight);
			glm::vec2 rightTopUV  (static_cast<float>(currentTexOffsetX+subHeigth-0.01f)/textureWidth,static_cast<float>(currentTexOffsetY - 0.01f)/textureHeight);
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

rapidjson::Document ResourceManager::loadJSONDoc(const std::string& JSONPath){
	const std::string JSONString = getFileString(JSONPath);
	if (JSONString.empty()) {
		std::cerr << "No JSON resources file" << std::endl;
		return nullptr;
	}
	rapidjson::Document JSONDoc;
	rapidjson::ParseResult parseResult = JSONDoc.Parse(JSONString.c_str());
	if (!parseResult) {
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "in JSON resources file: " << JSONPath << std::endl;
		return nullptr;
	}
	return JSONDoc;
}

bool ResourceManager::loadJSONResurces(const std::string& JSONPath){
	const rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	const std::string check = JSONDoc["checkJSON"].GetString();

	auto shadersIt = JSONDoc.FindMember("shader");
	if (shadersIt != JSONDoc.MemberEnd()) {
		for (const auto& currentShader : shadersIt->value.GetArray()) {
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();

			loadShader(name, filePath_v, filePath_f);
		}
	}

	auto textureAtlasesIt = JSONDoc.FindMember("textureAtlases");
	if (textureAtlasesIt != JSONDoc.MemberEnd()) {
		for (const auto& currentTextureAtlases : textureAtlasesIt->value.GetArray()) {
			const std::string name = currentTextureAtlases["name"].GetString();
			const std::string filePath = currentTextureAtlases["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlases["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = currentTextureAtlases["subTextureHeight"].GetUint();
			
			const auto subTexturesArray=currentTextureAtlases["subTextures"].GetArray();
			std::vector<std::string> subTexturesVector;
			subTexturesVector.reserve(subTexturesArray.Size());
			for (const auto& currentSubTexture : subTexturesArray) {
				subTexturesVector.emplace_back(currentSubTexture.GetString());
			}

			loadTextureAtlas(name, filePath, std::move(subTexturesVector), subTextureWidth, subTextureHeight);
		}
	}

	auto spriteIt = JSONDoc.FindMember("sprites");
	if (spriteIt != JSONDoc.MemberEnd()) {
		for (const auto& currentSprite : spriteIt->value.GetArray()) {
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			
			for (const auto& current : currentSprite["createSprites"].GetArray()) {
				const std::string nameSprite = current["nameSprite"].GetString();
				const std::string subTexture = current["subTexture"].GetString();

				loadSprite(nameSprite, textureAtlas, shader, subTexture);
			}
		}
	}

	auto stateAnimationIt = JSONDoc.FindMember("StateAnimation");
	if (stateAnimationIt != JSONDoc.MemberEnd()) {
		for (const auto& currentStateAnimation : stateAnimationIt->value.GetArray()) {
			const std::string nameState = currentStateAnimation["nameState"].GetString();
			const bool uninterrupted = currentStateAnimation["uninterrupted"].GetBool();
			const std::string nextState = currentStateAnimation["nextState"].GetString();

			const auto sourcesArray = currentStateAnimation["sources"].GetArray();
			std::vector<std::string> sourcesVector;
			sourcesVector.reserve(sourcesArray.Size());
			for (const auto& currentSource : sourcesArray) {
				sourcesVector.emplace_back(currentSource.GetString());
			}

			const auto framesArray = currentStateAnimation["frames"].GetArray();
			std::vector<std::pair<std::string,uint64_t>> framesVector;
			framesVector.reserve(framesArray.Size());
			for (const auto& currentFrame : framesArray) {
				framesVector.emplace_back(std::make_pair<std::string,uint64_t>(currentFrame["sprite"].GetString(), currentFrame["duration"].GetUint()));
			}

			loadStateAnimation(nameState, framesVector, sourcesVector, uninterrupted);
		}
	}

	return checkJSONResurces(check);
}

bool ResourceManager::checkJSONResurces(const std::string& JSONPath){
	const rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	auto It = JSONDoc.FindMember("shader");
	if (It != JSONDoc.MemberEnd()) {
		for (const auto& current : It->value.GetArray()) {
			if (!getShader(current.GetString())) {
				std::cerr << " can't find shader: " << current.GetString() << std::endl;
				unloadAllRes();
				return false;
			}
		}
	}

	It = JSONDoc.FindMember("textureAtlases");
	if (It != JSONDoc.MemberEnd()) {
		for (const auto& current : It->value.GetArray()) {
			if (!getTexture(current.GetString())) {
				std::cerr << " can't find texture: " << current.GetString() << std::endl;
				unloadAllRes();
				return false;
			}
		}
	}

	It = JSONDoc.FindMember("sprites");
	if (It != JSONDoc.MemberEnd()) {
		for (const auto& current : It->value.GetArray()) {
			if (!getSprite(current.GetString())) {
				std::cerr << " can't find sprite: " << current.GetString() << std::endl;
				unloadAllRes();
				return false;
			}
		}
	}

	It = JSONDoc.FindMember("StateAnimation");
	if (It != JSONDoc.MemberEnd()) {
		for (const auto& current : It->value.GetArray()) {
			if (!getStateAnimation(current.GetString())) {
				std::cerr << " can't find stateAnimation: " << current.GetString() << std::endl;
				unloadAllRes();
				return false;
			}
		}
	}

	return true;
}




