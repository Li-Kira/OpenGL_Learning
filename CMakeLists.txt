cmake_minimum_required(VERSION 4.0)
project(OpenGL)

set(CMAKE_CXX_STANDARD 14)

find_path(GLM_INCLUDE_DIR glm/glm.hpp
        PATHS /opt/homebrew/include /usr/local/include
)

include_directories(external)
include_directories(external/include)
include_directories(external/include/glad)
include_directories(external/include/GLFW)
include_directories(external/include/KHR)
include_directories(external/include/stb)
include_directories(include)

add_executable(OpenGL
    external/include/glad/glad.h
    external/include/GLFW/glfw3.h
    external/include/GLFW/glfw3native.h
    external/include/KHR/khrplatform.h
    external/include/stb/stb_image.cpp
    external/include/stb/stb_image.h
    external/include/stb/stb_image_write.h
    include/Logger.h
    include/Shader.h
    resource/shader/fullScreen.frag
    resource/shader/fullScreen.vert
    resource/shader/triangle.frag
    resource/shader/triangle.vert
    src/00_Basic/Basic.cpp
    src/glad.c)
