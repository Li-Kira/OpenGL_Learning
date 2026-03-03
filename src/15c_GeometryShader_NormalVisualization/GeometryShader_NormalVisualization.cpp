#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
const char* WINDOW_TITLE = "15c_GeometryShader_NormalVisualization";

bool cursorEnabled = false;
static bool tabKeyProcessed = false; 
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

    stbi_set_flip_vertically_on_load(true);
    std::string modelPath = "res/model/backpack/backpack.obj";
    Model backpack(modelPath); 
    Shader shader("res/shader/15c_default.vert", "res/shader/15c_default.frag");
    Shader normalShader("res/shader/15c_normalVisualization.vert", "res/shader/15c_normalVisualization.frag", "res/shader/15c_normalVisualization.geom");

    // unsigned int texture1 = TextureUtils::loadTexture("res/texture/backpackDiffuse.jpg", true, GL_REPEAT, GL_REPEAT);
    // shader.use(); 
    // shader.setInt("texture_diffuse1", 0);

    // Camera settings
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
    mainCamera = &camera;  // 设置全局相机指针
    camera.SetAspectRatio((float)SCR_WIDTH / (float)SCR_HEIGHT);
    // 设置窗口用户指针
    glfwSetWindowUserPointer(window, &camera);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float fpsUpdateTime = 0.0f;
    int frameCount = 0;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    bool isShowNormal = true;

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

        // ------------------------ ImGui ------------------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui control window
        ImGui::Begin("Geometry Shader Controls");
        ImGui::Checkbox("Show Normal", &isShowNormal);
        ImGui::End();

        // ------------------------ Render ------------------------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
        shader.use();
        shader.setMat4("u_ModelMatrix", model);
        shader.setMat4("u_ViewMatrix", view);
        shader.setMat4("u_ProjectionMatrix", projection);

        // draw model as usual
        backpack.Draw(shader);

        if (isShowNormal)
        {
            normalShader.use();
            normalShader.setMat4("u_ModelMatrix", model);
            normalShader.setMat4("u_ViewMatrix", view);
            normalShader.setMat4("u_ProjectionMatrix", projection);
            backpack.Draw(normalShader);
        }

        // ------------------------ Render ImGui ------------------------
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    TextureUtils::clearCache();

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
    // 切换鼠标指针状态
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        if (!tabKeyProcessed)
        {
            cursorEnabled = !cursorEnabled;
            if (cursorEnabled)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                Logger::log(Logger::Level::Info, "Cursor enabled");
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true;
                Logger::log(Logger::Level::Info, "Cursor disabled");
            }
            tabKeyProcessed = true;
        }
    }
    else
    {
        tabKeyProcessed = false;
    }

    if (!cursorEnabled)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
    }
        
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    if (cursorEnabled) return;

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
    if (cursorEnabled) return;

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