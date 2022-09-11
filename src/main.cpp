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

//Размер окна
glm::ivec2 g_window(640, 480);

//При измении окна
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    glViewport(0, 0, g_window.x, g_window.y);
}

//проверка нажатия кнопок
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv){

    if (!glfwInit()) { //Инициализация библиотеки GLFW
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    //Установка используемой версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Создание окна, размером g_window.x на g_window.y, с именем Game, в оконном режиме, и контекст использования общих ресурсов
    GLFWwindow* PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", NULL, NULL);

    if (!PWindow){//Проверка создания
        glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
        std::cout << "PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, glfwWindowSizeCallback);//Передача в GLFW функции изменения размера окна приложения
    glfwSetKeyCallback(PWindow, glfwKeyCallback);//Передача в GLFW функции отслеживающей нажатия
    glfwMakeContextCurrent(PWindow);//Указывает контекст окна активным
	
	if(!gladLoadGL()){//загрузка OpenGL
        glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
		std::cout<<"gladLoadGL failed"<<std::endl;
		return -1;
	}

    //Вывод версии
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.5,0.5,0.5,1);//цвет заполнения
    {
        ResourceManager resManager(argv[0]);//Передача пути к программе

        //Загрузка шейдера
        auto pSpriteShaderProgram = resManager.loadShader("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram) {
            std::cerr << "ERROR SpriteShader" << std::endl;
            return -1;
        }

        //Загрузка текстур
        auto tex2 = resManager.loadTexture("OPA", "res/textures/RORAP.png");
        auto tex = resManager.loadTexture("Def", "res/textures/RPSF.png");
        
        //загрузка текстурного атласа
        std::vector<std::string> subTextureNames = { "sci", "roc", "fuc", "pap"};
        auto pTextAtlas = resManager.loadTextureAtlas("DefAtlas", "res/textures/RPSF.png", subTextureNames, 32, 32);

        //Создание спрайта из атласа
        auto pSprite = resManager.loadSprite("newSprite", "DefAtlas", "SpriteShader", 64, 64, "fuc");
        pSprite->setPosition(glm::vec2(300, 100));

        //Создание спрайта из текстуры
        auto pSprite2 = resManager.loadSprite("POP", "OPA", "SpriteShader", 64, 64);
        pSprite2->setPosition(glm::vec2(300, 200));
       
        //Создание анимированого спрайта
        auto pAnimSprite = resManager.loadAnimSprite("newAnimSprite", "DefAtlas", "SpriteShader", 64, 64, "sci");
        std::vector<std::pair<std::string, uint64_t>> anime; //State для анимированого спрайта
        anime.emplace_back(std::make_pair<std::string, uint64_t>("sci", 125000000));//Стадии, с именем из атласа
        anime.emplace_back(std::make_pair<std::string, uint64_t>("roc", 125000000));
        anime.emplace_back(std::make_pair<std::string, uint64_t>("fuc", 125000000));
        anime.emplace_back(std::make_pair<std::string, uint64_t>("pap", 125000000));
        pAnimSprite->insertState("anime", std::move(anime));//Загрузка в спрайт
        pAnimSprite->setState("anime");//Переключение на состояние
        pAnimSprite->setPosition(glm::vec2(100, 100));

        //Матрица для шейдера
        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_window.x), 0.f, static_cast<float>(g_window.y), -100.f, 100.f);

        //Активация шейдера
        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex",0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        //Таймер
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(PWindow)) {
            auto currTime = std::chrono::high_resolution_clock::now();//Обновление таймера
            uint64_t dura = std::chrono::duration_cast<std::chrono::nanoseconds>(currTime - lastTime).count();//Просчёт изменившегося времени
            pAnimSprite->update(dura);//Обновление анимированого спрайта
            lastTime = currTime;//сдвиг таймера

            //Заполнение цветом указаном в glClearColor
            glClear(GL_COLOR_BUFFER_BIT);

            //рендер спрайтов
            pSprite->render();
            pSprite2->render();
            pAnimSprite->render();


            glfwSwapBuffers(PWindow);//Меняет буферы отрисовки
            glfwPollEvents();//Обработка событий
        }
    }
    glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
    return 0;
}