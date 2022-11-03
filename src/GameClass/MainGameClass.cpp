#include "MainGameClass.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <rapidjson/document.h>

MainGameClass::MainGameClass(const glm::ivec2& window) :m_GState(E_GAME_STATE::Active), m_window(window) {
    m_keys.fill(false);
}

MainGameClass::~MainGameClass() {
}

void MainGameClass::render() {

}

void MainGameClass::setKey(const int key, const int action) {
    m_keys[key] = action;
}

bool MainGameClass::init() {
    ResourceManager::loadJSONResurces("res/resJSON/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShader("spriteShader");

    auto pStr = ResourceManager::getSprite("S_Idle_1");

    //Матрица для шейдера
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);

    //Активация шейдера
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    return true;
}
