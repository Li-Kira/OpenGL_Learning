
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main() {
    fColor = aColor;
    // from top to bottom
    vec2 pos = aPos * (gl_InstanceID / 100.0);

    // from top to bottom， from left to right
    int gridWidth = 10;
    int gridHeight = 10;
    int instanceID = gl_InstanceID;
    int row = instanceID / gridWidth;
    int col = instanceID % gridWidth; 

    // 计算缩放因子：从左上角到右下角逐渐变小，对角线缩放
    // 或者根据你的需求调整方向
    float scaleX = 1.0 - float(col) / float(gridWidth - 1);    // 从左到右变小
    float scaleY = float(row) / float(gridHeight - 1);   // 从上到下变小
    
    // 组合缩放因子（可以根据需要调整）
    float scale = (scaleX + scaleY) * 0.5;  // 平均缩放
    // 或者使用其他组合方式：
    // scale = scaleX * scaleY;        // 乘积缩放
    // scale = min(scaleX, scaleY);    // 取最小值
    // float scale = max(scaleX, scaleY);    // 取最大值
    
    // 应用缩放
    pos = aPos * scale;

    gl_Position = vec4(pos + aOffset, 0.0, 1.0);
}

