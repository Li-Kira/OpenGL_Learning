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
#include "Model.h"
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
const char* WINDOW_TITLE = "19_Gamma_Correction";

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int floorTexture = TextureUtils::loadTexture(
        "res/texture/wood.png",  // path
        true,                    // flipVertically
        GL_REPEAT,               // wrapS
        GL_REPEAT,               // wrapT
        false                    // gammaCorrection
    );

    unsigned int floorTextureGammaCorrected = TextureUtils::loadTexture(
        "res/texture/wood.png",  // path
        true,                    // flipVertically
        GL_REPEAT,               // wrapS
        GL_REPEAT,               // wrapT
        true                    // gammaCorrection
    );

    Shader shader("res/shader/19_gammaCorrection.vert", "res/shader/19_gammaCorrection.frag");

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("floorTexture", 0);

    // lighting info
    // -------------
    glm::vec3 lightPositions[] = {
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3 (1.0f, 0.0f, 0.0f),
        glm::vec3 (3.0f, 0.0f, 0.0f)
    };
    glm::vec3 lightColors[] = {
        glm::vec3(0.25),
        glm::vec3(0.50),
        glm::vec3(0.75),
        glm::vec3(1.00)
    };

    // Camera settings
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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

    bool gammaEnabled = true;
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

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
        ImGui::Begin("Gamma Correction");
        ImGui::Checkbox("Gamma Enabled", &gammaEnabled);
        ImGui::End();

        // ------------------------ Render ------------------------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        shader.use();
        shader.setMat4("u_ViewMatrix", view);
        shader.setMat4("u_ProjectionMatrix", projection);
        // set light uniforms
        glUniform3fv(glGetUniformLocation(shader.ID, "lightPositions"), 4, &lightPositions[0][0]);
        glUniform3fv(glGetUniformLocation(shader.ID, "lightColors"), 4, &lightColors[0][0]);
        shader.setVec3("viewPos", camera.Position);
        shader.setInt("gamma", gammaEnabled);
        
        // floor
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gammaEnabled ? floorTextureGammaCorrected : floorTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ------------------------ Render ImGui ------------------------
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    TextureUtils::clearCache();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
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