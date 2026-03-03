# OpenGL

## Mac Environment Setting

``` bash
brew install glfw glew

echo >> /Users/likirac/.zprofile
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> /Users/likirac/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"
```

## Api

``` cpp
unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

### 绘制方法

以下是 OpenGL 中主要的绘制方法及其用法：

1. glDrawArrays

    ``` cpp
    void glDrawArrays(GLenum mode, GLint first, GLsizei count);
    ```

    用法：使用当前绑定的顶点数组对象 (VAO) 和顶点缓冲对象 (VBO) 进行绘制

    - mode：绘制模式（GL_POINTS, GL_LINES, GL_TRIANGLES 等）
    - first：起始顶点索引
    - count：绘制的顶点数量

    示例：

    ``` cpp
    glDrawArrays(GL_TRIANGLES, 0, 36); // 绘制36个顶点
    ```

2. glDrawElements

    ``` cpp
    void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
    ```

    用法：使用索引缓冲对象 (EBO) 进行绘制，更高效（避免重复顶点）

    - mode：绘制模式
    - count：索引数量
    - type：索引数据类型（GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT）
    - indices：索引数据偏移量

    示例：

    ``` cpp
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 使用6个索引绘制三角形
    ```

3. glDrawArraysInstanced

    ``` cpp
    void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
    ```

    用法：实例化渲染，多次绘制相同几何体但使用不同的实例数据
    - instancecount：实例数量

    示例：

    ``` cpp
    // 实例化渲染（适合大量重复对象）
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1000); // 绘制1000个立方体
    ```

4. glDrawElementsInstanced

    ``` cpp
    void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
    ```

    用法：带索引的实例化渲染

5. glMultiDrawArrays

    ``` cpp
    void glMultiDrawArrays(GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount);
    ```

    用法：一次性进行多个 glDrawArrays 调用

6. glMultiDrawElements

    ``` cpp
    void glMultiDrawElements(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount);
    ```

    用法：一次性进行多个 glDrawElements 调用

7. glDrawRangeElements

    ``` cpp
    void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
    ```

    用法：优化版本的 glDrawElements，指定索引范围

常用绘制模式：

- GL_POINTS：点
- GL_LINES：线段
- GL_LINE_STRIP：连续线段
- GL_LINE_LOOP：闭合线段
- GL_TRIANGLES：三角形
- GL_TRIANGLE_STRIP：三角形带
- GL_TRIANGLE_FAN：三角形扇

## lighting

lighting_maps_diffuse_map

每当我们应用一个不等比缩放时（注意：等比缩放不会破坏法线，因为法线的方向没被改变，仅仅改变了法线的长度，而这很容易通过标准化来修复），法向量就不会再垂直于对应的表面了，这样光照就会被破坏。

修复这个行为的诀窍是使用一个为法向量专门定制的模型矩阵。这个矩阵称之为法线矩阵(Normal Matrix)，它使用了一些线性代数的操作来移除对法向量错误缩放的影响。如果你想知道这个矩阵是如何计算出来的，建议去阅读这个文章。

法线矩阵被定义为「模型矩阵左上角3x3部分的逆矩阵的转置矩阵」。真是拗口，如果你不明白这是什么意思，别担心，我们还没有讨论逆矩阵(Inverse Matrix)和转置矩阵(Transpose Matrix)。注意，大部分的资源都会将法线矩阵定义为应用到模型-观察矩阵(Model-view Matrix)上的操作，但是由于我们只在世界空间中进行操作（不是在观察空间），我们只使用模型矩阵。

在顶点着色器中，我们可以使用inverse和transpose函数自己生成这个法线矩阵，这两个函数对所有类型矩阵都有效。注意我们还要把被处理过的矩阵强制转换为3×3矩阵，来保证它失去了位移属性以及能够乘以vec3的法向量。
Normal = mat3(transpose(inverse(model))) * aNormal;

lighting_maps_specular_map

light_casters_directional

light_casters_point

light_casters_spot

light_casters_spot_soft

multiple_lights

## Model Loading

## Advanced opengl

1.1.depth_testing
深度测试在片元着色器之前就被执行了，因此在片元着色器无法为像素上的深度缓冲区设置深度
深度z在不同距离上的精度不同，离的越近精度越高

防止z-fighting
1. 将物体偏移一点距离
2. 将近平面移远一点
3. 提高深度缓冲精度


1.2.depth_testing_view

2.stencil_testing

3.1.blending_discard
使用shader中的discard指令来渲染全透明的物体

3.2.blending_sort
先绘制所有不透明的物体。
对所有透明的物体排序。
按顺序绘制所有透明的物体。
排序算法：次序无关透明度(Order Independent Transparency, OIT)
不透明物体需要用不透明的帧缓冲来渲染： https://github.com/WhizZest/learnOpenGL/tree/master/8_guest/8_3.oit_instanced

4.face_culling_exercise1

5.1.framebuffers

在mac环境下，需要使用以下设置来配置framebuffer
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH * 2, SCR_HEIGHT * 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH * 2, SCR_HEIGHT * 2);

帧缓存的保存对象有两种，纹理和渲染缓冲对象

渲染缓冲对象：由于渲染缓冲对象通常都是只写的，它们会经常用于深度和模板附件，因为大部分时间我们都不需要从深度和模板缓冲中读取值，只关心深度和模板测试。我们需要深度和模板值用于测试，但不需要对它们进行采样，所以渲染缓冲对象非常适合它们。

6.1.cubemaps_skybox
绘制天空盒时，我们需要将它变为场景中的第一个渲染的物体，并且禁用深度写入。这样子天空盒就会永远被绘制在其它物体的背后了。这样子能够工作，但不是非常高效。如果我们先渲染天空盒，我们就会对屏幕上的每一个像素运行一遍片段着色器，即便只有一小部分的天空盒最终是可见的。可以使用提前深度测试(Early Depth Testing)轻松丢弃掉的片段能够节省我们很多宝贵的带宽。
在坐标系统小节中我们说过，透视除法是在顶点着色器运行之后执行的，将gl_Position的xyz坐标除以w分量。我们又从深度测试小节中知道，相除结果的z分量等于顶点的深度值。使用这些信息，我们可以将输出位置的z分量等于它的w分量，让z分量永远等于1.0，这样子的话，当透视除法执行之后，z分量会变为w / w = 1.0。
void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
最终的标准化设备坐标将永远会有一个等于1.0的z值：最大的深度值。结果就是天空盒只会在没有可见物体的地方渲染了（只有这样才能通过深度测试，其它所有的东西都在天空盒前面）。

我们还要改变一下深度函数，将它从默认的GL_LESS改为GL_LEQUAL。深度缓冲将会填充上天空盒的1.0值，所以我们需要保证天空盒在值小于或等于深度缓冲而不是小于时通过深度测试。

我们希望移除观察矩阵中的位移部分，让移动不会影响天空盒的位置向量。
通过取4x4矩阵左上角的3x3矩阵来移除变换矩阵的位移部分。我们可以将观察矩阵转换为3x3矩阵（移除位移），再将其转换回4x4矩阵，来达到类似的效果。


所以，我们将会最后渲染天空盒，以获得轻微的性能提升。

6.2.cubemaps_environment_mapping

加载反射贴图到模型
https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/06%20Cubemaps/

8.advanced_glsl_ubo
UBO 的主要优势在于减少 CPU-GPU 通信开销和确保数据一致性，特别适合需要被多个着色器频繁访问的全局数据。比如让4个着色器共享相同的视图和投影矩阵，使用 UBO 可以显著提升性能。

9.1.geometry_shader_houses

9.2.geometry_shader_exploding

9.3.geometry_shader_normals

10.1.instancing_quads

10.2.asteroids

10.3.asteroids_instanced

layout (location = 3) in mat4 instanceMatrix;
我们不再使用模型uniform变量，改为一个mat4的顶点属性，让我们能够存储一个实例化数组的变换矩阵。然而，当我们顶点属性的类型大于vec4时，就要多进行一步处理了。顶点属性最大允许的数据大小等于一个vec4。因为一个mat4本质上是4个vec4，我们需要为这个矩阵预留4个顶点属性。因为我们将它的位置值设置为3，矩阵每一列的顶点属性位置值就是3、4、5和6。

11.1.anti_aliasing_msaa

11.2.anti_aliasing_offscreen

## Advanced lighting

1.advanced_lighting
2.gamma_correction
3.1.1.shadow_mapping_depth
3.1.2.shadow_mapping_base
3.1.3.shadow_mapping
直射（太阳）光一般用正交投影矩阵，其它用透视投影矩阵，因为直射光一般距离很远，我们一般认为他对物体的边缘投影都是平行的。
透视投影因此更经常用在点光源和聚光灯上，而正交投影经常用在定向光上。另一个细微差别是，透视投影矩阵，将深度缓冲视觉化经常会得到一个几乎全白的结果。这个是因为透视投影将深度值转换为非线性值，其变化范围大多集中在近平面附近。为了可以像使用正交投影一样合适地观察深度值，你必须先将非线性深度值转变为线性值，

阴影失真->对当前用于对比的深度进行偏移来解决
阴影悬浮->当渲染深度贴图（阴影贴图）时候使用正面剔除
过采样（光的视锥范围以外的区域也会被判定为处于阴影之中）->原因是我们之前将深度贴图的环绕方式设置成了GL_REPEAT,我们可以配置一个纹理边界颜色，然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER,仍有一部分是阴影区域，那部分区域的坐标超出了光的正交视锥的远平面,只要投影向量的z坐标大于1.0，我们就把shadow的值强制设为0.0：

PCF:译作百分比渐进滤波(percentage-closer filtering)，这个术语涵盖了多种滤波函数，能生成更柔和的阴影，减少锯齿块。其核心思想是多次采样深度贴图，每一次采样的纹理坐标都稍有不同，独立判断每个采样点的阴影状态后，将子结果混合取平均，最终获得相对柔和的阴影

3.2.1.point_shadows
3.2.2.point_shadows_soft
3.3.csm
4.normal_mapping
由于法线贴图总是指向z轴，也就是我们通常看到的蓝色(0,0,1)，所以直接采样使用法线贴图在正面看不会有问题，但是一旦转到了其它的轴向就会有问题。
稍微有点难的解决方案是，在一个不同的坐标空间中进行光照，这个坐标空间里，法线贴图向量总是指向这个坐标空间的正z方向；所有的光照向量都相对与这个正z方向进行变换。这样我们就能始终使用同样的法线贴图，不管朝向问题。这个坐标空间叫做切线空间（tangent space）
法线贴图中的法线向量定义在切线空间中，在切线空间中，法线永远指着正z方向，它就像法线贴图向量的局部空间。
当物体表面在其它轴向的时候，可以计算出一种矩阵，把法线从切线空间变换到一个不同的空间，这样它们就能和表面法线方向对齐了
这种矩阵叫做TBN矩阵这三个字母分别代表tangent、bitangent和normal向量。这是建构这个矩阵所需的向量。要建构这样一个把切线空间转变为不同空间的变异矩阵，我们需要三个相互垂直的向量，它们沿一个表面的法线贴图对齐于：上、右、前；这和我们在摄像机教程中做的类似。

现在我们有了TBN矩阵，如果来使用它呢？通常来说有两种方式使用它，我们会把这两种方式都说明一下：

1.我们直接使用TBN矩阵，这个矩阵可以把切线坐标空间的向量转换到世界坐标空间。因此我们把它传给片段着色器中，把通过采样得到的法线坐标左乘上TBN矩阵，转换到世界坐标空间中，这样所有法线和其他光照变量就在同一个坐标系中了。
2.我们也可以使用TBN矩阵的逆矩阵，这个矩阵可以把世界坐标空间的向量转换到切线坐标空间。因此我们使用这个矩阵左乘其他光照变量，把他们转换到切线空间，这样法线和其他光照变量再一次在一个坐标系中了。

1.
normal = texture(normalMap, fs_in.TexCoords).rgb;
normal = normalize(normal * 2.0 - 1.0);   
normal = normalize(fs_in.TBN * normal);
因为最后的normal现在在世界空间中了，就不用改变其他像素着色器的代码了，因为光照代码就是假设法线向量在世界空间中。
2.
vs_out.TBN = transpose(mat3(T, B, N));

void main()
{           
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   

    vec3 lightDir = fs_in.TBN * normalize(lightPos - fs_in.FragPos);
    vec3 viewDir  = fs_in.TBN * normalize(viewPos - fs_in.FragPos);    
    [...]
}

注意，这里我们使用transpose函数，而不是inverse函数。正交矩阵（每个轴既是单位向量同时相互垂直）的一大属性是一个正交矩阵的置换矩阵与它的逆矩阵相等。这个属性很重要因为逆矩阵的求得比求置换开销大；结果却是一样的。

在像素着色器中我们不用对法线向量变换，但我们要把其他相关向量转换到切线空间，它们是lightDir和viewDir。这样每个向量还是在同一个空间（切线空间）中了。

第二种方法看似要做的更多，它还需要在像素着色器中进行更多的乘法操作，所以为何还用第二种方法呢？

将向量从世界空间转换到切线空间有个额外好处，我们可以把所有相关向量在顶点着色器中转换到切线空间，不用在像素着色器中做这件事。这是可行的，因为lightPos和viewPos不是每个fragment运行都要改变，对于fs_in.FragPos，我们也可以在顶点着色器计算它的切线空间位置。基本上，不需要把任何向量在像素着色器中进行变换，而第一种方法中就是必须的，因为采样出来的法线向量对于每个像素着色器都不一样。

所以现在不是把TBN矩阵的逆矩阵发送给像素着色器，而是将切线空间的光源位置，观察位置以及顶点位置发送给像素着色器。这样我们就不用在像素着色器里进行矩阵乘法了。这是一个极佳的优化，因为顶点着色器通常比像素着色器运行的少。这也是为什么这种方法是一种更好的实现方式的原因。
out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform vec3 lightPos;
uniform vec3 viewPos;

[...]

void main()
{    
    [...]
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vec3(model * vec4(position, 0.0));
}

最后
当在更大的网格上计算切线向量的时候，它们往往有很大数量的共享顶点，当法向贴图应用到这些表面时将切线向量平均化通常能获得更好更平滑的结果。这样做有个问题，就是TBN向量可能会不能互相垂直，这意味着TBN矩阵不再是正交矩阵了。法线贴图可能会稍稍偏移，但这仍然可以改进。

使用叫做格拉姆-施密特正交化过程（Gram-Schmidt process）的数学技巧，我们可以对TBN向量进行重正交化，这样每个向量就又会重新垂直了。在顶点着色器中我们这样做：
vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
// re-orthogonalize T with respect to N
T = normalize(T - dot(T, N) * N);
// then retrieve perpendicular vector B with the cross product of T and N
vec3 B = cross(T, N);

mat3 TBN = mat3(T, B, N)
这样稍微花费一些性能开销就能对法线贴图进行一点提升。看看最后的那个附加资源： Normal Mapping Mathematics视频，里面有对这个过程的解释。
https://www.youtube.com/watch?v=4FaWLgsctqY


5.1.parallax_mapping
视差贴图属于位移贴图(Displacement Mapping)技术的一种，它对根据储存在纹理中的几何信息对顶点进行位移或偏移。一种实现的方式是比如有1000个顶点，根据纹理中的数据对平面特定区域的顶点的高度进行位移。这样的每个纹理像素包含了高度值纹理叫做高度贴图

视差贴图尝试通过对从fragment到观察者的方向向量V¯进行缩放

问题：
1.当表面的高度变化很快的时候，看起来就不会真实
2.视差贴图的另一个问题是，当表面被任意旋转以后很难指出从P¯获取哪一个坐标。我们在视差贴图中使用了另一个坐标空间，这个空间P¯向量的x和y元素总是与纹理表面对齐。如果你看了法线贴图教程，你也许猜到了，我们实现它的方法，是的，我们还是在切线空间中实现视差贴图。

位移贴图和教程一开始的那个高度贴图相比是颜色是相反的，是因为使用反色高度贴图（也叫深度贴图）去模拟深度比模拟高度更容易。

实现：
我们定义了一个叫做ParallaxMapping的函数，它把fragment的纹理坐标和切线空间中的fragment到观察者的方向向量为输入。这个函数返回经位移的纹理坐标。然后我们使用这些经位移的纹理坐标进行diffuse和法线贴图的采样。最后fragment的diffuse颜色和法线向量就正确的对应于表面的经位移的位置上了。
void main()
{           
    // Offset texture coordinates with Parallax Mapping
    vec3 viewDir   = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);

    // then sample textures with new texture coords
    vec3 diffuse = texture(diffuseMap, texCoords);
    vec3 normal  = texture(normalMap, texCoords);
    normal = normalize(normal * 2.0 - 1.0);
    // proceed with lighting code
    [...]    
}

我们用本来的纹理坐标texCoords从高度贴图中来采样，得到当前fragment的高度H(A)。然后计算出P¯，x和y元素在切线空间中，viewDir向量除以它的z元素，用fragment的高度对它进行缩放。我们同时引入额一个height_scale的uniform，来进行一些额外的控制，因为视差效果如果没有一个缩放参数通常会过于强烈。然后我们用P¯减去纹理坐标来获得最终的经过位移纹理坐标。

有一个地方需要注意，就是viewDir.xy除以viewDir.z那里。因为viewDir向量是经过了标准化的，viewDir.z会在0.0到1.0之间的某处。当viewDir大致平行于表面时，它的z元素接近于0.0，除法会返回比viewDir垂直于表面的时候更大的P¯向量。所以，从本质上，相比正朝向表面，当带有角度地看向平面时，我们会更大程度地缩放P¯的大小，从而增加纹理坐标的偏移；这样做在视角上会获得更大的真实度。

有些人更喜欢不在等式中使用viewDir.z，因为普通的视差贴图会在角度上产生不尽如人意的结果；这个技术叫做有偏移量限制的视差贴图（Parallax Mapping with Offset Limiting）。
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height =  texture(depthMap, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
    return texCoords - p;    
}

5.2.steep_parallax_mapping
陡峭视差映射(Steep Parallax Mapping)是视差映射的扩展，原则是一样的，但不是使用一个样本而是多个样本来确定向量P¯到B。即使在陡峭的高度变化的情况下，它也能得到更好的结果，原因在于该技术通过增加采样的数量提高了精确性。
陡峭视差贴图同样有自己的问题。因为这个技术是基于有限的样本数量的，我们会遇到锯齿效果以及图层之间有明显的断层：
两种最流行的解决方法叫做Relief Parallax Mapping和Parallax Occlusion Mapping，Relief Parallax Mapping更精确一些，但是比Parallax Occlusion Mapping性能开销更多。因为Parallax Occlusion Mapping的效果和前者差不多但是效率更高，因此这种方式更经常使用



5.3.parallax_occlusion_mapping
你可以看到大部分和陡峭视差映射一样，不一样的地方是有个额外的步骤，两个深度层的纹理坐标围绕着交叉点的线性插值。这也是近似的，但是比陡峭视差映射更精确。

视差遮蔽映射的代码基于陡峭视差映射，所以并不难：

[...] // steep parallax mapping code here

// get texture coordinates before collision (reverse operations)
vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

// get depth after and before collision for linear interpolation
float afterDepth  = currentDepthMapValue - currentLayerDepth;
float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

// interpolation of texture coordinates
float weight = afterDepth / (afterDepth - beforeDepth);
vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

return finalTexCoords;
在对（位移的）表面几何进行交叉，找到深度层之后，我们获取交叉前的纹理坐标。然后我们计算来自相应深度层的几何之间的深度之间的距离，并在两个值之间进行插值。线性插值的方式是在两个层的纹理坐标之间进行的基础插值。函数最后返回最终的经过插值的纹理坐标。

视差遮蔽映射的效果非常好，尽管有一些可以看到的轻微的不真实和锯齿的问题，这仍是一个好交易，因为除非是放得非常大或者观察角度特别陡，否则也看不到。

大多数时候视差贴图用在地面和墙壁表面，这种情况下查明表面的轮廓并不容易，同时观察角度往往趋向于垂直于表面。这样视差贴图的不自然也就很难能被注意到了，对于提升物体的细节可以起到难以置信的效果。
6.hdr
HDR渲染：我们允许用更大范围的颜色值渲染从而获取大范围的黑暗与明亮的场景细节，最后将所有HDR值转换成在[0.0, 1.0]范围的LDR(Low Dynamic Range,低动态范围)。转换HDR值到LDR值得过程叫做色调映射(Tone Mapping)，现在现存有很多的色调映射算法，这些算法致力于在转换过程中保留尽可能多的HDR细节。这些色调映射算法经常会包含一个选择性倾向黑暗或者明亮区域的参数。

在实现HDR渲染之前，我们首先需要一些防止颜色值在每一个片段着色器运行后被限制约束的方法。当帧缓冲使用了一个标准化的定点格式(像GL_RGB)为其颜色缓冲的内部格式，OpenGL会在将这些值存入帧缓冲前自动将其约束到0.0到1.0之间。这一操作对大部分帧缓冲格式都是成立的，除了专门用来存放被拓展范围值的浮点格式
当一个帧缓冲的颜色缓冲的内部格式被设定成了GL_RGB16F, GL_RGBA16F, GL_RGB32F 或者GL_RGBA32F时，这些帧缓冲被叫做浮点帧缓冲(Floating Point Framebuffer)，浮点帧缓冲可以存储超过0.0到1.0范围的浮点值，所以非常适合HDR渲染。
默认的帧缓冲默认一个颜色分量只占用8位(bits)。当使用一个使用32位每颜色分量的浮点帧缓冲时(使用GL_RGB32F 或者GL_RGBA32F)，我们需要四倍的内存来存储这些颜色。所以除非你需要一个非常高的精确度，32位不是必须的，使用GLRGB16F就足够了。

7.bloom

提取亮色
第一步我们要从渲染出来的场景中提取两张图片。我们可以渲染场景两次，每次使用一个不同的着色器渲染到不同的帧缓冲中，但我们可以使用一个叫做MRT（Multiple Render Targets，多渲染目标）的小技巧，这样我们就能指定多个像素着色器输出；有了它我们还能够在一个单独渲染处理中提取头两个图片。在像素着色器的输出前，我们指定一个布局location标识符
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;



8.1.deferred_shading
我们现在一直使用的光照方式叫做正向渲染(Forward Rendering)或者正向着色法(Forward Shading)，它是我们渲染物体的一种非常直接的方式，在场景中我们根据所有光源照亮一个物体，之后再渲染下一个物体，以此类推。它非常容易理解，也很容易实现，但是同时它对程序性能的影响也很大，因为对于每一个需要渲染的物体，程序都要对每一个光源每一个需要渲染的片段进行迭代

延迟着色法的其中一个缺点就是它不能进行混合(Blending)，因为G缓冲中所有的数据都是从一个单独的片段中来的，而混合需要对多个片段的组合进行操作。延迟着色法另外一个缺点就是它迫使你对大部分场景的光照使用相同的光照算法，你可以通过包含更多关于材质的数据到G缓冲中来减轻这一缺点。

为了克服这些缺点(特别是混合)，我们通常分割我们的渲染器为两个部分：一个是延迟渲染的部分，另一个是专门为了混合或者其他不适合延迟渲染管线的着色器效果而设计的的正向渲染的部分。为了展示这是如何工作的，我们将会使用正向渲染器渲染光源为一个小立方体，因为光照立方体会需要一个特殊的着色器(会输出一个光照颜色)。

结合延迟渲染与正向渲染
这里用正向渲染渲染光源的时候，需要将延迟渲染中的深度信息保留下来，首先复制出在几何渲染阶段中储存的深度信息，并输出到默认的帧缓冲的深度缓冲，然后我们才渲染光立方体。这样之后只有当它在之前渲染过的几何体上方的时候，光立方体的片段才会被渲染出来。我们可以使用glBlitFramebuffer复制一个帧缓冲的内容到另一个帧缓冲中

8.2.deferred_shading_volumes
一个着色器运行时总是执行一个if语句所有的分支从而保证着色器运行都是一样的，这使得我们之前的半径检测优化完全变得无用，我们仍然在对所有光源计算光照！

延迟渲染确实带来一些缺点：大内存开销，没有MSAA和混合(仍需要正向渲染的配合)。
9.ssao

SSAO背后的原理很简单：对于铺屏四边形(Screen-filled Quad)上的每一个片段，我们都会根据周边深度值计算一个遮蔽因子(Occlusion Factor)。这个遮蔽因子之后会被用来减少或者抵消片段的环境光照分量。遮蔽因子是通过采集片段周围球型核心(Kernel)的多个深度样本，并和当前片段深度值对比而得到的。高于片段深度值样本的个数就是我们想要的遮蔽因子。

gBuffer：
由于我们已经有了逐片段位置和法线数据(G缓冲中)，我们只需要更新一下几何着色器，让它包含片段的线性深度就行了
提取出来的线性深度是在观察空间中的，所以之后的运算也是在观察空间中。确保G缓冲中的位置和法线都在观察空间中(乘上观察矩阵也一样)。观察空间线性深度值之后会被保存在gPositionDepth颜色缓冲的alpha分量中，省得我们再声明一个新的颜色缓冲纹理。
这里的gBuffer中的position的颜色换成需要设置为浮点数据；这样从0.1到50.0范围深度值都不会被限制在[0.0, 1.0]之间了。如果你不用浮点值存储这些深度数据，确保你首先将值除以FAR来标准化它们，再存储到gPositionDepth纹理中，并在以后的着色器中用相似的方法重建它们。同样需要注意的是GL_CLAMP_TO_EDGE的纹理封装方法。这保证了我们不会不小心采样到在屏幕空间中纹理默认坐标区域之外的深度值。

采样半球：
假设我们有一个单位半球，我们可以获得一个拥有最大64样本值的采样核心：

std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
std::default_random_engine generator;
std::vector<glm::vec3> ssaoKernel;
for (GLuint i = 0; i < 64; ++i)
{
    glm::vec3 sample(
        randomFloats(generator) * 2.0 - 1.0, 
        randomFloats(generator) * 2.0 - 1.0, 
        randomFloats(generator)
    );
    sample = glm::normalize(sample);
    sample *= randomFloats(generator);
    GLfloat scale = GLfloat(i) / 64.0; 
    ssaoKernel.push_back(sample);  
}
我们在切线空间中以-1.0到1.0为范围变换x和y方向，并以0.0和1.0为范围变换样本的z方向(如果以-1.0到1.0为范围，取样核心就变成球型了)。由于采样核心将会沿着表面法线对齐，所得的样本矢量将会在半球里。
目前，所有的样本都是平均分布在采样核心里的，但是我们更愿意将更多的注意放在靠近真正片段的遮蔽上，也就是将核心样本靠近原点分布。我们可以用一个加速插值函数实现它：

   ...[接上函数]
   scale = lerp(0.1f, 1.0f, scale * scale);
   sample *= scale;
   ssaoKernel.push_back(sample);  
lerp被定义为：

GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
{
    return a + f * (b - a);
}

旋转随机核心：
通过引入一些随机性到采样核心上，我们可以大大减少获得不错结果所需的样本数量。我们可以对场景中每一个片段创建一个随机旋转向量，但这会很快将内存耗尽。所以，更好的方法是创建一个小的随机旋转向量纹理平铺在屏幕上。我们接下来创建一个包含随机旋转向量的4x4纹理；记得设定它的封装方法为GL_REPEAT，从而保证它合适地平铺在屏幕上


## PBR

1.1.lighting
1.2.lighting_textured
2.1.1.ibl_irradiance_conversion
2.1.2.ibl_irradiance
2.2.1.ibl_specular
2.2.2.ibl_specular_textured


## Tip
一个快速检测光照类型的工具：
w分量等于1.0，它是光的位置（这样变换和投影才能正确应用）
w分量等于0.0，则它是光的方向向量（不想让位移有任何的效果，因为它仅仅代表的是方向）