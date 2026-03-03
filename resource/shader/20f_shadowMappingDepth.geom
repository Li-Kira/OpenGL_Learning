// #version 410 core

// layout(triangles, invocations = 5) in;
// layout(triangle_strip, max_vertices = 3) out;

// layout (std140) uniform LightSpaceMatrices
// {
//     mat4 lightSpaceMatrices[16];
// };
// /*
// uniform mat4 lightSpaceMatrices[16];
// */

// void main()
// {          
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
// 		gl_Layer = gl_InvocationID;
// 		EmitVertex();
// 	}
// 	EndPrimitive();
// }  
#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

uniform int cascadeCount;

void main()
{          
    for (int layer = 0; layer < cascadeCount + 1; ++layer)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = lightSpaceMatrices[layer] * gl_in[i].gl_Position;
            gl_Layer = layer;
            EmitVertex();
        }
        EndPrimitive();
    }
}