#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
          MovementSpeed(2.5f),
          MouseSensitivity(0.1f),
          Fov(45.0f),
          AspectRatio(16.0f / 9.0f),
          NearPlane(0.1f),
          FarPlane(100.0f)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    void SetAspectRatio(float aspectRatio) {
        AspectRatio = aspectRatio;
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix() {
        return glm::perspective(glm::radians(Fov), AspectRatio, NearPlane, FarPlane);
    }

    void ProcessKeyboard(Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true)
    {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
            {
                Pitch = 89.0f;
            }
            if (Pitch < -89.0f)
            {
                Pitch = -89.0f;
            }
        }

        updateCameraVectors();        
    }

    void ProcessMouseScroll(float yOffset) 
    {
        Fov -= (float)yOffset;
        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 45.0f)
            Fov = 45.0f;
    }

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float Fov;
    float AspectRatio;
    float NearPlane;
    float FarPlane;


private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        
        // Calculate the new Front vector
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
