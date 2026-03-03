#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <iostream>
#include <vector>
#include <filesystem>
#include "Shader.h"
#include "Camera.h"
#include "ModelPlus.h"
#include "Logger.h"
#include "FileUtils.h"
#include "TextureUtils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime);
void ShowFPS(GLFWwindow* window, float deltaTime);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_TITLE = "16b_Render_Asteroids";

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
Camera* mainCamera = nullptr;  // 全局相机指针

int main() {
    // initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL)
    {
        Logger::log(Logger::Level::Error, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::log(Logger::Level::Error, "Failed to initialize GLAD");
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    Model rock("res/model/rock/rock.obj");
    Model planet("res/model/planet/planet.obj");
    Shader shader("res/shader/16b_asteroids.vert", "res/shader/16b_asteroids.frag");

    // generate a large list of semi-random model transformation matrices
    // ------------------------------------------------------------------
    unsigned int amount = 10000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 50.0f;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100) / 100.0f - offset);
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));
        
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        
        modelMatrices[i] = model;
    }

    // ------------------------ Camera settings ------------------------
    Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
    mainCamera = &camera;  // 设置全局相机指针
    camera.SetAspectRatio((float)SCR_WIDTH / (float)SCR_HEIGHT);
    // 设置窗口用户指针
    glfwSetWindowUserPointer(window, &camera);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float fpsUpdateTime = 0.0f;
    int frameCount = 0;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // ------------------------ fps ------------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frameCount++;
        fpsUpdateTime += deltaTime;

        // 每秒更新一次帧数显示
        if (fpsUpdateTime >= 1.0f) {
            ShowFPS(window, frameCount / fpsUpdateTime);
            frameCount = 0;
            fpsUpdateTime = 0.0f;
        }

        // ------------------------ Process Input ------------------------
        ProcessInput(window, camera, deltaTime);

        // ------------------------ Render ------------------------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        shader.use();
        shader.setMat4("u_ViewMatrix", view);
        shader.setMat4("u_ProjectionMatrix", projection);

        // draw planet
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        shader.setMat4("u_ModelMatrix", model);
        planet.Draw(shader);

        // draw meteorites
        for (unsigned int i = 0; i < amount; i++)
        {
            shader.setMat4("u_ModelMatrix", modelMatrices[i]);
            rock.Draw(shader);
        }   
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    TextureUtils::clearCache();

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
    // 相机移动控制
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!camera) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// 显示帧数函数
void ShowFPS(GLFWwindow* window, float fps) {
    std::string title = std::string(WINDOW_TITLE) + " - FPS: " + std::to_string(static_cast<int>(fps));
    glfwSetWindowTitle(window, title.c_str());
}