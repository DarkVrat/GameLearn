#include "MainGameClass.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
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

void MainGameClass::update(uint64_t duration){
    for (auto current : m_GObject) {
        current.update(duration);
    }
}

void MainGameClass::render() {
    for (auto current : m_GObject) {
        current.render();
    }
}

void MainGameClass::setKey(const int key, const int action) {
    m_keys[key] = action;
}

bool MainGameClass::init() {
    ResourceManager::loadJSONResurces("res/resJSON/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShader("spriteShader");

    m_GObject.emplace_back(glm::vec2(0, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(128, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(256, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(384, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(512, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(640, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(768, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(896, 100), glm::vec2(128, 128), 0.f, "Attack1");
    m_GObject.emplace_back(glm::vec2(1024, 100), glm::vec2(128, 128), 0.f, "Attack1");

    //Матрица для шейдера
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);

    //Активация шейдера
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    return true;
}

void MainGameClass::Events(){
    if (m_keys[GLFW_KEY_1] == GLFW_PRESS) m_GObject[0].attack();
    if (m_keys[GLFW_KEY_2] == GLFW_PRESS) m_GObject[1].attack();
    if (m_keys[GLFW_KEY_3] == GLFW_PRESS) m_GObject[2].attack();
    if (m_keys[GLFW_KEY_4] == GLFW_PRESS) m_GObject[3].attack();
    if (m_keys[GLFW_KEY_5] == GLFW_PRESS) m_GObject[4].attack();
    if (m_keys[GLFW_KEY_6] == GLFW_PRESS) m_GObject[5].attack();
    if (m_keys[GLFW_KEY_7] == GLFW_PRESS) m_GObject[6].attack();
    if (m_keys[GLFW_KEY_8] == GLFW_PRESS) m_GObject[7].attack();
    if (m_keys[GLFW_KEY_9] == GLFW_PRESS) m_GObject[8].attack();

    if (m_keys[GLFW_KEY_1] == GLFW_RELEASE) m_GObject[0].idle();
    if (m_keys[GLFW_KEY_2] == GLFW_RELEASE) m_GObject[1].idle();
    if (m_keys[GLFW_KEY_3] == GLFW_RELEASE) m_GObject[2].idle();
    if (m_keys[GLFW_KEY_4] == GLFW_RELEASE) m_GObject[3].idle();
    if (m_keys[GLFW_KEY_5] == GLFW_RELEASE) m_GObject[4].idle();
    if (m_keys[GLFW_KEY_6] == GLFW_RELEASE) m_GObject[5].idle();
    if (m_keys[GLFW_KEY_7] == GLFW_RELEASE) m_GObject[6].idle();
    if (m_keys[GLFW_KEY_8] == GLFW_RELEASE) m_GObject[7].idle();
    if (m_keys[GLFW_KEY_9] == GLFW_RELEASE) m_GObject[8].idle();
}
