
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec3 Normal;
out vec3 FragPos;

void main() {
    // Normal = normal;
    // 当模型矩阵包含缩放（特别是 非均匀缩放，即x、y、z方向缩放比例不同）时，
    // 直接用法线矩阵乘以法线向量会导致法线方向不正确。
    // 这个组合确保了法线向量在变换后仍然与表面垂直
    Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    FragPos = vec3(u_ModelMatrix * vec4(position, 1.0));

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0);
     
}

