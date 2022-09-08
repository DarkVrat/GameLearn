#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <iostream>
#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimSprite.h"

glm::ivec2 g_window(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    glViewport(0, 0, g_window.x, g_window.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv){
    

    if (!glfwInit()) {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", NULL, NULL);

    if (!PWindow){
        glfwTerminate();
        std::cout << "PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(PWindow, glfwKeyCallback);
    glfwMakeContextCurrent(PWindow);
	
	if(!gladLoadGL()){
        glfwTerminate();
		std::cout<<"gladLoadGL failed"<<std::endl;
		return -1;
	}

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.5,0.5,0.5,1);
    {
        ResourceManager resManager(argv[0]);
        auto pDefaultShaderProgram = resManager.loadShader("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "IDINAHUY" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resManager.loadShader("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram) {
            std::cerr << "IDINAHUY SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resManager.loadTexture("Def", "res/textures/RPSF.png");

        std::vector<std::string> subTextureNames = { "sci", "roc", "fuc", "pap"};
        auto pTextAtlas = resManager.loadTextureAtlas("DefAtlas", "res/textures/RPSF.png", subTextureNames, 32, 32);

        auto pSprite = resManager.loadSprite("newSprite", "DefAtlas", "SpriteShader", 64, 64, "fuc");
        pSprite->setPosition(glm::vec2(300, 100));

        auto pAnimSprite = resManager.loadAnimSprite("newAnimSprite", "DefAtlas", "SpriteShader", 64, 64, "sci");
        std::vector<std::pair<std::string, uint64_t>> anime;
        anime.emplace_back(std::make_pair<std::string, uint64_t>("sci", 125000000));
        anime.emplace_back(std::make_pair<std::string, uint64_t>("roc", 125000000));
        anime.emplace_back(std::make_pair<std::string, uint64_t>("fuc", 125000000));
        anime.emplace_back(std::make_pair<std::string, uint64_t>("pap", 125000000));

        pAnimSprite->insertState("anime", std::move(anime));

        pAnimSprite->setState("anime");
        pAnimSprite->setPosition(glm::vec2(100, 100));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_window.x), 0.f, static_cast<float>(g_window.y), -100.f, 100.f);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(PWindow)) {
            auto currTime = std::chrono::high_resolution_clock::now();
            uint64_t dura = std::chrono::duration_cast<std::chrono::nanoseconds>(currTime - lastTime).count();
            pAnimSprite->update(dura);
            lastTime = currTime;

            glClear(GL_COLOR_BUFFER_BIT);

            pSprite->render();
            pAnimSprite->render();

            glfwSwapBuffers(PWindow);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}