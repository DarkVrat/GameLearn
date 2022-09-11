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

//Конструктор получающий argv[0] в main для получения пути к игре
ResourceManager::ResourceManager(const std::string& executablePath) {
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

//функция получения данных из файла
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
//Загрузка шейдера в память
std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexString = getFileString(vertexPath);//Данные из вертексного шейдера
	if (vertexString.empty()) {//Проверка наличия
		std::cerr << "No vertex" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);//Данные из фрагментного шейдера
	if (fragmentString.empty()) {//Проверка наличия
		std::cerr << "No fragment" << std::endl;
		return nullptr;
	}

	//Создание в куче объекта шейдера и помещение в MAP
	std::shared_ptr<Renderer::ShaderProgram>&newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled()) return newShader;//Проверка и возврат шейдера при успешном выполнении

	std::cerr << "Can't load shader" << std::endl;
	return nullptr;
}

//загрузка шейдера из памяти
std::shared_ptr<Renderer::ShaderProgram>  ResourceManager::getShader(const std::string& shaderName) {
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);//поиск
	if (it != m_shaderPrograms.end()) return it->second;//проверка наличия и возврат если есть
	std::cerr << "Cant find the shader " << shaderName << std::endl;
	return nullptr;
}
//-------------------------------Shader------------------------------------//



 //-------------------------------Texture-----------------------------------//
//Загрузка текстуры в память
std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePatn) {
	int channels = 0;
	int widht = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);//Для загрузки текстуры в правильном ориентривании, при false изображение будет перевёрнуто
	unsigned char* pixels=stbi_load(std::string(m_path + "/" + texturePatn).c_str(), &widht, &height, &channels, 0);//загрузка пикселей, а так же заполнение иформации
	if (!pixels) { //Проверка успешности
		std::cerr << "ERROR TEXTUR LOAD" << textureName <<std::endl;
		return nullptr;
	}

	//Создание текстуры в куче и возврат ссылки
	std::shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(widht, height,pixels, channels,GL_NEAREST,GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);//Освобождение памяти
	return newTexture;//передача текстуры
}

//загрузка текстуры из памяти
std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName) {
	TexturesMap::const_iterator it = m_textures.find(textureName);//поиск
	if (it != m_textures.end()) return it->second;//получение
	std::cerr << "Cant find the textureName " << textureName << std::endl;
	return nullptr;
}
//-------------------------------Texture-----------------------------------//



 //-------------------------------Sprite------------------------------------//
//Загрузка спрайта в память
std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight,const std::string& subTextureName){
	//Поиск и проверка текстуры
	auto pTexture = getTexture(textureName);
	if (!pTexture) std::cerr << "Cant find the textureName " << textureName << " for sprite " << spriteName << std::endl;
	
	//Поиск и проверка шейдера
	auto pShader = getShader(shaderName);
	if (!pShader) std::cerr << "Cant find the shaderName " << shaderName << " for sprite " << spriteName << std::endl;
	
	//создание спрайта и возврат указателя на спрайт
	std::shared_ptr<Renderer::Sprite> newSprite = m_sprite.emplace(textureName,std::make_shared<Renderer::Sprite>(pTexture,subTextureName,pShader,glm::vec2(0.f, 0.f),glm::vec2(spriteWidth, spriteHeight),0.f)).first->second;
	return newSprite;
}

//загрузка спрайта из памяти
std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName){
	SpriteMap::const_iterator it = m_sprite.find(spriteName);//поиск
	if (it != m_sprite.end()) return it->second;//получение
	std::cerr << "Cant find the spriteName " << spriteName << std::endl;
	return nullptr;
}
//-------------------------------Sprite------------------------------------//



 //-------------------------------AnimSprite--------------------------------//
//Загрузка анимационного спрайта в память
std::shared_ptr<Renderer::AnimSprite> ResourceManager::loadAnimSprite(const std::string& animSpriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName){
	//Загрузка текстуры
	auto pTexture = getTexture(textureName);
	if (!pTexture) std::cerr << "Cant find the textureName " << textureName << " for sprite " << animSpriteName << std::endl;
	
	//Загрузка шейдера
	auto pShader = getShader(shaderName);
	if (!pShader) std::cerr << "Cant find the shaderName " << shaderName << " for sprite " << animSpriteName << std::endl;
	
	//Создание и передача анимационного спрайта
	std::shared_ptr<Renderer::AnimSprite> newSprite = m_animSprite.emplace(textureName,std::make_shared<Renderer::AnimSprite>(	pTexture,subTextureName,pShader,glm::vec2(0.f, 0.f),glm::vec2(spriteWidth, spriteHeight),0.f)).first->second;
	return newSprite;
}

//Загрузка анимационного спрайта из память
std::shared_ptr<Renderer::AnimSprite> ResourceManager::getAnimSprite(const std::string& animSpriteName){
	auto it = m_animSprite.find(animSpriteName);
	if (it != m_animSprite.end()) return it->second;
	std::cerr << "Cant find the spriteName " << animSpriteName << std::endl;
	return nullptr;
}
//-------------------------------AnimSprite--------------------------------//


//загрузка текстурного атласа, текстурный атлас не привязывается к уже загруженым текстурам
std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTextureAtlas(std::string textureName,std::string texturePatn,std::vector<std::string> subTextures,const unsigned subWidth,const unsigned subHeigth) {
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePatn));//Загрузка текстуры
	if (pTexture) {
		const unsigned int textureWidth = pTexture->getWidth();	//получение ширины
		const unsigned int textureHeight = pTexture->getHeight(); //Получение высоты
		unsigned int currentTexOffsetX = 0;//Положение X Координаты
		unsigned int currentTexOffsetY = textureHeight;//Положение Y координаты

		for (auto& currentSubTexName : subTextures) {//Проходя по вектору названий для сабтекстур
			//Отмеряем левую нижнюю и правую верхнюю координату подтекстур
			glm::vec2 leftBottomUV(	static_cast<float>(currentTexOffsetX)/textureWidth,				static_cast<float>(currentTexOffsetY-subHeigth)/textureHeight);
			glm::vec2 rightTopUV(	static_cast<float>(currentTexOffsetX+ subHeigth)/textureWidth,	static_cast<float>(currentTexOffsetY)/textureHeight);
			//помещаем текстуру в подтекстуры 
			pTexture->addSubTexture(std::move(currentSubTexName), leftBottomUV, rightTopUV);
			currentTexOffsetX += subWidth; //Двигаем X
			if (currentTexOffsetX >= textureWidth) {//При проходе по всем X координатам возвращаемся в 0 и уменьшаем Y
				currentTexOffsetX = 0;
				currentTexOffsetY -= subHeigth;
			}
		}
	}
	return pTexture;//Возврат тектуры
}