#include "Initial.h"
#include "Shader.h"
#include <stack>
#include <vector>
unsigned int VAO, VBO;
#define POINT_MAX 20
bool isRed = false;
float buff_point[6010];
// ************************设定好的参数************************************
const unsigned int windowsWidth = 1000;
const unsigned int windowsHeight = 1000;
const char* head = "Bezier";
glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
/**
* @brief 按下Esc退出
* @param window 传入的窗口指针
* @param deltaTime 添加的新参数
*/

long long int multiple_result[100];
bool overflow = false;

// Bezier函数
float Bezier(int i, int n, float t) {
    if (n >= POINT_MAX || i >= POINT_MAX || n - i >= POINT_MAX) {
        overflow = true;
        return 0.0f;
    }
    long long int n_result = multiple_result[n];
    long long int i_result = multiple_result[i];
    long long int ni_result = multiple_result[n - i];

    float alpha = n_result / i_result / ni_result;
    return (alpha * pow(t, i) * pow(1 - t, n - i));
}
void escapePress(GLFWwindow *window, float& deltaTime);
int buff_set(int count, glm::vec2 cur, glm::vec3 color) {
    buff_point[count * 6] = (cur.x / windowsWidth - 0.5f) * 2.0f;
    buff_point[count * 6 + 1] = -(cur.y / windowsHeight - 0.5f) * 2.0f;
    buff_point[count * 6 + 2] = 0.0f;
    buff_point[count * 6 + 3] = color.x;
    buff_point[count * 6 + 4] = color.y;
    buff_point[count * 6 + 5] = color.z;
    return count + 1;
}
void draw(int count, bool helping = false);
int main()
{
    // 初始化窗口
    GLFWwindow* window = NULL;
    if (initialWindow::initial(window, windowsWidth, windowsHeight, head) == -1) return -1;
    // 初始化rgb值
    Shader shader("rgb.vs", "rgb.frag");
    /**
    * IMGUI
    */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    stack<glm::vec2> press_points;
    vector<glm::vec2> press_points_vec;
    double deltaTime = 0.0;
    // 计算阶乘
    for (int i = 0; i < POINT_MAX; ++i) {
        if (i == 0) multiple_result[i] = 1;
        else if (i == 1) multiple_result[i] = 1;
        else if (i == 2) multiple_result[i] = 2;
        else multiple_result[i] = multiple_result[i - 1] * i;
    }
    // 用于辅助线插值
    float helping_t = 0.0f;
    while (!glfwWindowShouldClose(window))
    {

        // 按键控制
        float rand = 0.0f;
        escapePress(window, rand);

        // 背景颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 左键
        if (Mouse::getInstance()->isPress && Mouse::getInstance()->left) {
            if (press_points.size() < POINT_MAX) {
                press_points.push(glm::vec2(Mouse::getInstance()->pos_x, Mouse::getInstance()->pos_y));
                press_points_vec.clear();
                isRed = false;
            }
            else {
                isRed = true;
            }
            Mouse::getInstance()->isPress = false;
        }
        // 右键
        if (Mouse::getInstance()->isPress && !Mouse::getInstance()->left) {
            if(press_points.size() > 0)press_points.pop();
            press_points_vec.clear();
            deltaTime = glfwGetTime();
            isRed = false;
            Mouse::getInstance()->isPress = false;
        }
        shader.use();
        // 获得所有的点
        stack<glm::vec2> tmp = press_points;
        int count = 0;
        bool fill = press_points_vec.empty();
        while (tmp.size() != 0) {
            glm::vec2 point = tmp.top();
            if (fill) press_points_vec.insert(press_points_vec.begin(), point);
            tmp.pop();
            if (isRed) {
                count = buff_set(count, point, red);
            }
            else {
                count = buff_set(count, point, white);
            }
        }
        // 画出白点
        draw(count, true);
        // 曲线
        if (press_points.size() > 1) {
            // 曲线
            count = 0;
            // 0.005/ 0.002/ 0.001都可以
            for (float t = 0.0f; t < 1.0f; t += 0.001) {
                glm::vec2 cur = glm::vec2(0.0f, 0.0f);
                for (int i = 0; i < press_points_vec.size(); i++) {
                    cur += press_points_vec[i] * Bezier(i, press_points_vec.size() - 1, t);
                    if (overflow) break;
                }
                if (overflow) break;
                count = buff_set(count, cur, yellow);
            }
            if (overflow) break;
            draw(count);
            count = 0;
            double currentTime = glfwGetTime();
            if (currentTime - deltaTime > 2.0f) {
                //--------------- 画出辅助线--------------------------------
                vector<glm::vec2> helping_points1 = press_points_vec, helping_points2;
                count = 0;
                helping_t = helping_t > 1.0f ? 0.0f : helping_t + 0.01f;
                while (helping_points1.size() != 0) {
                    for (unsigned int i = 0; i < helping_points1.size() - 1; ++i) {
                        glm::vec2 cur = (1.0f - helping_t) * helping_points1[i] + helping_t * helping_points1[1 + i];
                        count = buff_set(count, cur, white);
                        helping_points2.push_back(cur);
                    }
                    draw(count, true);
                    count = 0;
                    helping_points1.clear();
                    helping_points1 = helping_points2;
                    helping_points2.clear();
                }
                glm::vec2 cur = glm::vec2(0.0f, 0.0f);
                for (unsigned int i = 0; i < press_points_vec.size(); i++) {
                    cur += press_points_vec[i] * Bezier(i, press_points_vec.size() - 1, helping_t);
                }
                count = buff_set(count, cur, red);
                draw(1);
                count = 0;
            }
        }
        
        count = 0;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

/**
* @brief 进行键盘按键的检测
* @param window 窗口指针
* @param deltaTime 按键持续时间
*/
void escapePress(GLFWwindow *window, float& deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // 添加WSAD方向
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(RIGHT, deltaTime);
}

void draw(int count, bool helping) {


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * count, buff_point, GL_STATIC_DRAW);
    glPointSize(6);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_POINTS, 0, count);
    if (helping) glDrawArrays(GL_LINE_STRIP, 0, count);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VBO);
}