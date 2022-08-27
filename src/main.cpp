#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int g_windowX = 640;
int g_windowY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowX = width;
    g_windowY = height;
    glViewport(0, 0, g_windowX, g_windowY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    if (!glfwInit()) {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* PWindow = glfwCreateWindow(g_windowX, g_windowY, "Game", NULL, NULL);

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
	glClearColor(0,1,0,1);
	
    while (!glfwWindowShouldClose(PWindow)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(PWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}