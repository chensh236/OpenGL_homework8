#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Mouse.h"
using namespace std;
/**
* @brief 这个类用于初始化GLFWwindow
*/



class initialWindow {
public:

    /**
    * @brief 主要函数，初始化窗口
    * @param window 初始化后返回的窗口指针
    * @param windowsWidth 传入的窗口宽度
    * @param windowsHiehgt 传入的窗口高度
    * @param head 窗口的标题
    */
    static int initial(GLFWwindow* &window, int windowsWidth, int windowsHeight, const char* head) {
        // 进行GLFW的初始化
        // OpenGL版本4.5
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // 进行窗口对象的创建，大小为1920*1080
        window = glfwCreateWindow(windowsWidth, windowsHeight, head, NULL, NULL);
        if (window == NULL)
        {
            cout << "Fail to create GLFW window!" << endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, getFrameBufferSize);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetMouseButtonCallback(window, mouseClick_callback);
        glfwSetCursorPosCallback(window, mouseMove_callback);
        //对GLAD进行初始化
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout << "Failed to initialize GLAD" << endl;
            Sleep(100);
            return -1;
        }
        return 0;
    }

private:
    /**
    * @brief 检查窗口的大小是否改变
    * @param window 传入的窗口指针
    * @param windowsWidth 传入的窗口宽度
    * @param windowsHiehgt 传入的窗口高度
    */
    static void getFrameBufferSize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (Camera::getInstance()->firstMouse)
        {
            Camera::getInstance()->lastX = xpos;
            Camera::getInstance()->lastY = ypos;
            Camera::getInstance()->firstMouse = false;
        }

        float xoffset = xpos - Camera::getInstance()->lastX;
        float yoffset = Camera::getInstance()->lastY - ypos;
        Camera::getInstance()->lastX = xpos;
        Camera::getInstance()->lastY = ypos;
        Camera::getInstance()->lookAround(xoffset, yoffset);
    }

    /**
    * @brief 通过滚轮进行视图放大缩小
    * @param window 传入的窗口指针
    * @param xoffset x方向的偏移量
    * @param yoffset y方向的偏移量
    */
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        // 缩小
        if (Camera::getInstance()->fov >= 1.0f && Camera::getInstance()->fov <= 45.0f)
            Camera::getInstance()->fov -= yoffset;
        // 避免溢出
        if (Camera::getInstance()->fov <= 1.0f)
            Camera::getInstance()->fov = 1.0f;
        if (Camera::getInstance()->fov >= 45.0f)
            Camera::getInstance()->fov = 45.0f;
    }

    //static void mousePress_callback(GLFWwindow* window, double x_pos, double y_pos) {
    //    Mouse::getInstance()->pos_x = x_pos;
    //    Mouse::getInstance()->pos_y = y_pos;

    //}

    //static void mouseMove_callback(GLFWwindow* window, int button, int action, int mods) {
    //    Mouse::getInstance()->left = (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT);
    //    Mouse::getInstance()->right = (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT);
    //}
    static void mouseMove_callback(GLFWwindow* window, double pos_x, double pos_y) {
        Mouse::getInstance()->pos_x = pos_x;
        Mouse::getInstance()->pos_y = pos_y;
    }

    static void mouseClick_callback(GLFWwindow* window, int button, int action, int mods) {
        Mouse::getInstance()->isPress = action == GLFW_PRESS;
        Mouse::getInstance()->left = (button == GLFW_MOUSE_BUTTON_LEFT);
    }
};