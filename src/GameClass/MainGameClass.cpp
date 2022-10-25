#include "MainGameClass.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimSprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <iostream>

MainGameClass::MainGameClass(const glm::ivec2& window):m_GState(E_GAME_STATE::Active),m_window(window) {
	m_keys.fill(false);
}

MainGameClass::~MainGameClass(){
}

void MainGameClass::render(){
    ResourceManager::getAnimSprite("newAnimSprite")->render();
    ResourceManager::getSprite("newSprite")->render();
    ResourceManager::getSprite("POP")->render();
}

void MainGameClass::update(const uint64_t delta){
    ResourceManager::getAnimSprite("newAnimSprite")->update(delta);
}

void MainGameClass::setKey(const int key, const int action){
	m_keys[key] = action;
}

bool MainGameClass::init(){
    //Загрузка шейдера
    auto pSpriteShaderProgram = ResourceManager::loadShader("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    if (!pSpriteShaderProgram) {
        std::cerr << "ERROR SpriteShader" << std::endl;
        return false;
    }

    //Загрузка текстур
    auto tex2 = ResourceManager::loadTexture("OPA", "res/textures/RORAP.png");
    auto tex = ResourceManager::loadTexture("Def", "res/textures/RPSF.png");

    //загрузка текстурного атласа
    std::vector<std::string> subTextureNames = { "sci", "roc", "fuc", "pap" };
    auto pTextAtlas = ResourceManager::loadTextureAtlas("DefAtlas", "res/textures/RPSF.png", subTextureNames, 32, 32);

    //Создание спрайта из атласа
    auto pSprite = ResourceManager::loadSprite("newSprite", "DefAtlas", "SpriteShader", 64, 64, "fuc");
    pSprite->setPosition(glm::vec2(300, 100));


    //Создание спрайта из текстуры
    auto pSprite2 = ResourceManager::loadSprite("POP", "OPA", "SpriteShader", 64, 64);
    pSprite2->setPosition(glm::vec2(300, 200));

    //Создание анимированого спрайта
    auto pAnimSprite = ResourceManager::loadAnimSprite("newAnimSprite", "DefAtlas", "SpriteShader", 64, 64, "sci");
    std::vector<std::pair<std::string, uint64_t>> anime; //State для анимированого спрайта
    anime.emplace_back(std::make_pair<std::string, uint64_t>("sci", 125000000));//Стадии, с именем из атласа
    anime.emplace_back(std::make_pair<std::string, uint64_t>("roc", 125000000));
    anime.emplace_back(std::make_pair<std::string, uint64_t>("fuc", 125000000));
    anime.emplace_back(std::make_pair<std::string, uint64_t>("pap", 125000000));
    pAnimSprite->insertState("anime", std::move(anime));//Загрузка в спрайт
    pAnimSprite->setState("anime");//Переключение на состояние
    pAnimSprite->setPosition(glm::vec2(100, 100));

    //Матрица для шейдера
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);

    //Активация шейдера
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

	return true;
}
