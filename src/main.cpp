#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>


#include <chrono>
#include <iostream>
#include "Resources/ResourceManager.h"
#include "GameClass/MainGameClass.h"

//������ ����
glm::ivec2 g_window(640, 480);
MainGameClass g_Game(g_window);

//��� ������� ����
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    glViewport(0, 0, g_window.x, g_window.y);
}

//�������� ������� ������
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_Game.setKey(key, action);
}

int main(int argc, char** argv){

    if (!glfwInit()) { //������������� ���������� GLFW
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    //��������� ������������ ������ OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //�������� ����, �������� g_window.x �� g_window.y, � ������ Game, � ������� ������, � �������� ������������� ����� ��������
    GLFWwindow* PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", NULL, NULL);

    if (!PWindow){//�������� ��������
        glfwTerminate();//����������� GLFW � ������������� ����������� �������������
        std::cout << "PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, glfwWindowSizeCallback);//�������� � GLFW ������� ��������� ������� ���� ����������
    glfwSetKeyCallback(PWindow, glfwKeyCallback);//�������� � GLFW ������� ������������� �������
    glfwMakeContextCurrent(PWindow);//��������� �������� ���� ��������
	
	if(!gladLoadGL()){//�������� OpenGL
        glfwTerminate();//����������� GLFW � ������������� ����������� �������������
		std::cout<<"gladLoadGL failed"<<std::endl;
		return -1;
	}

    //����� ������
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.5,0.5,0.5,1);//���� ����������
    {
        ResourceManager::setExecutablePath(argv[0]);//�������� ���� � ���������
       
        g_Game.init();

        //������
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(PWindow)) {
            auto currTime = std::chrono::high_resolution_clock::now();//���������� �������
            uint64_t dura = std::chrono::duration_cast<std::chrono::nanoseconds>(currTime - lastTime).count();//������� ������������� �������
            
            g_Game.update(dura);

            lastTime = currTime;//����� �������

            //���������� ������ �������� � glClearColor
            glClear(GL_COLOR_BUFFER_BIT);

            g_Game.render();


            glfwSwapBuffers(PWindow);//������ ������ ���������
            glfwPollEvents();//��������� �������
        }
        ResourceManager::unloadAllRes();
    }
    glfwTerminate();//����������� GLFW � ������������� ����������� �������������
    return 0;
}